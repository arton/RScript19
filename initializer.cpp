/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-03 04:23:11 +0900 (金, 03 11 2006) $
 */

#include "stdafx.h"
#include <process.h>
#include "RScript.h"
#include "Initializer.h"
#include "Win32Ole.h"
#include "RScriptCore.h"
#include "RubyWrapper.h"

CInitializer CInitializer::GlobalInitializer;
bool CInitializer::g_fInRuby(false);
//
// Create STA and run Ruby only in it.
// 
class CRubyWrapperFilter : public IMessageFilter
{
public:
	CRubyWrapperFilter() : m_pPrevious(NULL)
	{
	}
	~CRubyWrapperFilter()
	{
		ATLTRACE(_T("Destroy MessageFilter\n"));
	}

	HRESULT  STDMETHODCALLTYPE QueryInterface(
		const IID & riid,  
		void **ppvObj)
	{
		if (InlineIsEqualGUID(riid, IID_IUnknown) || InlineIsEqualGUID(riid, IID_IMessageFilter))
		{
			*ppvObj = this;
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	ULONG  STDMETHODCALLTYPE AddRef() { 
		return 1;
	}
	ULONG  STDMETHODCALLTYPE Release() { 
		return 1; 
	}
	//
	DWORD STDMETHODCALLTYPE HandleInComingCall(
		DWORD dwCallType,      //Type of incoming call
		HTASK threadIDCaller,  //Task handle calling this task
		DWORD dwTickCount,     //Elapsed tick count
		LPINTERFACEINFO lpInterfaceInfo //Pointer to INTERFACEINFO structure
		)
	{
		switch (dwCallType)
		{
		case CALLTYPE_TOPLEVEL_CALLPENDING:
			if (lpInterfaceInfo) 
			{
#ifdef _DEBUG
				WCHAR sz[64];
				StringFromGUID2(lpInterfaceInfo->iid, sz, 64);
				ATLTRACE(_T("HandleInComingCall for %08X LPINFO method=%d, unk=%08X, IID=%ls\n"), threadIDCaller, lpInterfaceInfo->wMethod, lpInterfaceInfo->pUnk, sz);
#endif
				if (InlineIsEqualGUID(IID_IRubyWrapper, lpInterfaceInfo->iid)
					&&	lpInterfaceInfo->wMethod > 4 && lpInterfaceInfo->wMethod < 10)
					return SERVERCALL_REJECTED;
			}
			ATLTRACE(_T("HandleInComingCall for %08X result = %d\n"), threadIDCaller, SERVERCALL_RETRYLATER);
			return SERVERCALL_ISHANDLED;
		default:
			break;
		}
		return SERVERCALL_ISHANDLED;
	}
	DWORD STDMETHODCALLTYPE RetryRejectedCall(
		HTASK threadIDCallee,  //Server task handle
		DWORD dwTickCount,     //Elapsed tick count
		DWORD dwRejectType     //Returned rejection message
		)
	{
		if (m_pPrevious)
			return m_pPrevious->RetryRejectedCall(threadIDCallee, dwTickCount, dwRejectType);
		return 1000;
	}
	DWORD STDMETHODCALLTYPE MessagePending(
		HTASK threadIDCallee,  //Called applications task handle
		DWORD dwTickCount,     //Elapsed tick count
		DWORD dwPendingType    //Call type
		)
	{
		if (m_pPrevious)
			return m_pPrevious->MessagePending(threadIDCallee, dwTickCount, dwPendingType);
		Sleep(100);
		return PENDINGMSG_WAITNOPROCESS;
	}

	void Initialize()
	{
		HRESULT hr = CoRegisterMessageFilter(this, &m_pPrevious);
		if (hr != S_OK)
		{
			m_pPrevious = NULL;
		}
	}
private:
	IMessageFilter* m_pPrevious;
} g_Filter;

unsigned __stdcall RubyWrapper(void * pParam)
{
	MSG msg;
	// Create Thread Queue
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	g_Filter.Initialize();
	{
		CComObject<CRubyWrapper>* p = NULL;
		HRESULT hr = CComObject<CRubyWrapper>::CreateInstance(&p);
		ATLTRACE(_T("Create Wrapper = %08X in thread %08X\n"), hr, GetCurrentThreadId());
		if (hr == S_OK)
		{
			p->Globalize();
		}
		HANDLE h = *reinterpret_cast<HANDLE*>(pParam);
		SetEvent(h);

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			if (msg.message == WM_TIMER)
			{
				if (msg.wParam && p->GetTimerID() == msg.wParam) break;
			}
			CInitializer::SetInRuby(true);
			DispatchMessage(&msg);
			CInitializer::SetInRuby(false);
		}
		ruby_finalize();
		if (p)
		{
			p->Unglobalize();
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				DispatchMessage(&msg);
			}
		}
	}

	try
	{
		CoUninitialize();
	}
	catch (...)
	{
		TerminateThread(GetCurrentThread(), -1);
	}
	_endthreadex(0);
	return 0;
}

CInitializer::CInitializer()
	: m_fInit(false), m_dwRubyThread(0)
{
}

void CInitializer::InitNew()
{
	if (m_fInit) return;
	Lock();
	if (m_fInit)
	{
		Unlock();
		return;
	}
	m_fInit = true;

	HANDLE hEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	long h = (long)_beginthreadex(NULL, 0, RubyWrapper, &hEvt, 0, reinterpret_cast<UINT*>(&m_dwRubyThread));
	if (h != -1)
	{
		DWORD dw = WaitForSingleObject(hEvt, 10000);
		ATLTRACE(_T("Wait = %08X\n"), dw);
		CRubyWrapper::GetCWrapper()->SetHandle((HANDLE)h);
	}
	CloseHandle(hEvt);
	Unlock();
}

void CInitializer::Destroy()
{
	if (m_fInit)
	{
		m_fInit = false;
	}
	if (m_dwRubyThread)
	{
		CRubyWrapper* p = CRubyWrapper::GetCWrapper();
		CloseHandle(p->GetHandle());
		PostThreadMessage(m_dwRubyThread, WM_QUIT, 0, 0);
		m_dwRubyThread = 0;
	}
}

CInitializer::~CInitializer()
{
	Destroy();
	ATLTRACE(_("RScript Initalizer was Destroyed\n"));
}

