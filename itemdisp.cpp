/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 */

#include "stdafx.h"
#ifdef _MERGE_PROXYSTUB
#include "RScript.h"
#else
#include "GRScript.h"
#endif
#include "RScriptCore.h"

#include "ItemDisp.h"

IDispatch* CItemHolder::GetDispatch() 
{ 
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (m_pGIPDispEx.IsOK())
	{
		IDispatchEx* p;
		HRESULT hr = m_pGIPDispEx.Localize(&p);
		return p;
	}
	if (m_pGIPDisp.IsOK())
	{
		IDispatch* p;
		HRESULT hr = m_pGIPDisp.Localize(&p);
		return p;
	}
#endif
	if (m_pDispEx)
	{
		m_pDispEx->AddRef();
		return m_pDispEx;
	}
	if (m_pDisp)
	{
		m_pDisp->AddRef();
		return m_pDisp;
	}
	return NULL;
}

CItemHolder::~CItemHolder()
{
	Empty();
}

void CItemHolder::Empty()
{
	if (m_pDispEx)
	{
		m_pDispEx->Release();
		m_pDispEx = NULL;
	}
	if (m_pDisp) 
	{
		m_pDisp->Release();
		m_pDisp = NULL;
	}
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (m_pGIPDispEx.IsOK())
	{
		m_pGIPDispEx.Unglobalize();
	}
	if (m_pGIPDisp.IsOK())
	{
		 m_pGIPDisp.Unglobalize();
	}
#endif
}

IDispatch* CItemHolder::GetDispatch(IActiveScriptSite* pSite, LPOLESTR pstrName, bool fSameApt)
{
	IDispatchEx* pDispEx = m_pDispEx;
	IDispatch* pDisp = m_pDisp;
	ATLTRACE(_("CItemHolder::GetDispatch in Thread:%08X name=%ls sameptr=%d\n"), GetCurrentThreadId(), pstrName, fSameApt);
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (fSameApt)
	{
#endif
		if (pDisp == NULL && pDispEx == NULL)
		{
			ATLTRACE(_("CItemHolder::GetDispatch in Thread:%08X\n"), GetCurrentThreadId());
			IUnknown* pUnk = NULL;
			ITypeInfo* pTypeInfo = NULL;
			HRESULT hr = pSite->GetItemInfo(pstrName, SCRIPTINFO_IUNKNOWN, &pUnk, &pTypeInfo);
			if (hr == S_OK)
			{
				if (pTypeInfo) pTypeInfo->Release();
				if (pUnk->QueryInterface(IID_IDispatchEx, (void**)&pDispEx) == S_OK)
				{
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
					HRESULT hr = m_pGIPDispEx.Globalize(pDispEx);
					ATLTRACE(_T("Globalize Item = %08X\n"), hr);
#endif
					m_pDispEx = pDispEx;
				}
				else
				{
					pDispEx = NULL;
					if (pUnk->QueryInterface(IID_IDispatch, (void**)&pDisp) == S_OK)
					{
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
						HRESULT hr = m_pGIPDisp.Globalize(pDisp);
						ATLTRACE(_T("Globalize Item = %08X\n"), hr);
#endif
						m_pDisp = pDisp;
					}
				}
				pUnk->Release();
			}
		}

		if (pDispEx)
		{
			pDispEx->AddRef();
		}
		else if (pDisp)
		{
			pDisp->AddRef();
		}
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	}
	else
	{
		if (m_pGIPDisp.IsOK() == false && m_pGIPDispEx.IsOK() == false)
		{
			ATLTRACE(_("CItemHolder::GetDispatch in Thread:%08X\n"), GetCurrentThreadId());
			IUnknown* pUnk = NULL;
			ITypeInfo* pTypeInfo = NULL;
			HRESULT hr = pSite->GetItemInfo(pstrName, SCRIPTINFO_IUNKNOWN, &pUnk, &pTypeInfo);
			if (hr == S_OK)
			{
				if (pTypeInfo) pTypeInfo->Release();
				if (pUnk->QueryInterface(IID_IDispatchEx, (void**)&pDispEx) != S_OK)
				{
					pDispEx = NULL;
					if (pUnk->QueryInterface(IID_IDispatch, (void**)&pDisp) != S_OK)
					{
						pDisp = NULL;
					}
				}
				pUnk->Release();
			}
		}
		if (m_pGIPDispEx.IsOK())
		{
			IDispatchEx* p;
			HRESULT hr = m_pGIPDispEx.Localize(&p);
			ATLTRACE(_("Localize DispEx = %08X\n"), hr);
			return p;
		}
		if (m_pGIPDisp.IsOK())
		{
			IDispatch* p;
			HRESULT hr = m_pGIPDisp.Localize(&p);
			ATLTRACE(_("Localize Disp = %08X\n"), hr);
			return p;
		}
	}
#endif
	return (pDispEx) ? pDispEx : pDisp;
}
