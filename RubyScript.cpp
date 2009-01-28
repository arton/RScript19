/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-15 12:49:36 +0900 (水, 15 11 2006) $
 */

#include "stdafx.h"
#include "RScript.h"
#include "RubyScript.h"
#include "RubyWrapper.h"

#include "win32ole.h"
#include "ItemDisp.h"
#include "Initializer.h"
#include "ScrError.h"
#include "RubyObject.h"
/////////////////////////////////////////////////////////////////////////////
// CRubyScript
HRESULT CRubyScript::FinalConstruct()
{
	ATLTRACE(_T("Create RubyScript On Thread:%d\n"), GetCurrentThreadId());
	m_pWrapper = CRubyWrapper::GetWrapper();
	m_valueModule = Qnil;
	if (s_idInstanceEval == Qnil)
	{
		s_idInstanceEval = intern("instance_eval");
		s_idModuleEval = intern("module_eval");
	}
	HRESULT hr = CRScriptCore::FinalConstruct();
	return hr;
}

void CRubyScript::FinalRelease()
{
	if (m_pModuleObject)
	{
		m_pModuleObject->ClearEngine();
	}
	if (m_pRubyObject)
	{
		delete m_pRubyObject;
		m_pRubyObject = NULL;
	}
	for (RubyObjMapIter it = m_mapObj.begin(); it != m_mapObj.end(); it++)
	{
		(*it).second->Release();
	}
	m_mapObj.clear();
	char sz[16];
	wsprintfA(sz, "X%08X", m_valueModule);
	m_pWrapper->UnregisterObject(reinterpret_cast<LPBYTE>(sz), m_valueModule);
	m_pWrapper->Release();
	CRScriptCore::FinalRelease();
#if 0 // never activate this codes. for VB's developing situation.
	s_crit.Lock();
	if (s_listEngine.size() == 0)
	{
		CInitializer::GetInstance().Destroy();
	}
	s_crit.Unlock();
#endif

	ATLTRACE(_T("Destroyed RubyScript On Thread:%d\n"), GetCurrentThreadId());
}

HRESULT STDMETHODCALLTYPE CRubyScript::ClearModuleObject( void)
{
	m_pModuleObject = NULL;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyScript::SetRubyize( 
            /* [in] */ boolean f)
{
	m_fRubyize = (f) ? true : false;
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CRubyScript::GetRubyize( void)
{
	return (m_fRubyize) ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE CRubyScript::GetModule( 
            /* [retval][out] */ unsigned long __RPC_FAR *pResult)
{
	if (!pResult) return E_POINTER;
	*pResult = m_valueModule;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyScript::DisconnectObjects( void)
{
	for (OleObjListIter itole = m_listOleObj.begin(); itole != m_listOleObj.end(); itole++)
	{
		IDispatch* p = (*itole)->pDispatch;
		if (p) 
		{
			p->Release();
			(*itole)->pDispatch = NULL;
		}
	}
	m_listOleObj.clear();
	return S_OK;
}

IActiveScriptSite* CRubyScript::GetSite()
{
	IActiveScriptSite* pSite = NULL;
	if (GetCurrentThreadId() == m_dwThreadID)
	{
		pSite = m_pSite;
		pSite->AddRef();
	}
	else
	{
		HRESULT hr = m_pGippedSite.Localize(&pSite);
		ATLTRACE(_T("Localize ScriptSite = %08X\n"), hr);
	}
	return pSite;
}

// IActiveScript
HRESULT STDMETHODCALLTYPE CRubyScript::SetScriptSite( 
            /* [in] */ IActiveScriptSite __RPC_FAR *pass)
{ 
	HRESULT hr = CRScriptCore::SetScriptSite(pass);
	if (hr == S_OK)
	{
		hr = m_pGippedSite.Globalize(pass);
		ATLTRACE(_T("Globalize ActiveScriptSite = %08X\n"), hr);
		if (m_pScM)
		{
			// Treat Ruby As Java
			DWORD dwPolicy(0), dwTemp(0);
			hr = m_pScM->ProcessUrlAction(URLACTION_JAVA_CURR_MAX,
				reinterpret_cast<LPBYTE>(&dwPolicy),sizeof(dwPolicy),
				(BYTE *)NULL, 0, PUAF_DEFAULT, 0);
			if (dwPolicy == URLPOLICY_JAVA_HIGH)
			{
				m_dwSecurityLevel = 3;
			}
			else if (dwPolicy == URLPOLICY_JAVA_MEDIUM)
			{
				m_dwSecurityLevel = 3;
			}
			else
			{
				m_dwSecurityLevel = 1;
			}
#ifdef SCM_CAN_GLOBALIZE
			// Windows98 's GIT interface crushs when call to the code.
			hr = m_pGippedScM.Globalize(m_pScM);
			ATLTRACE(_T("Globalize SecurityManager = %08X\n"), hr);
#endif
			hr = S_OK;
		}
	}
	return hr;
}

HRESULT STDMETHODCALLTYPE CRubyScript::SetScriptState( 
            /* [in] */ SCRIPTSTATE ss)
{
	HRESULT hr = CRScriptCore::SetScriptState(ss);
	if (hr == S_OK)
	{
		switch (ss)
		{
		case SCRIPTSTATE_STARTED:
		case SCRIPTSTATE_CONNECTED:
			if (m_strScriptPersistent.length() > 0)
			{
				VARIANT v;
				VariantInit(&v);
				EnterScript();
				hr = Invoke(m_valueModule, m_nStartLinePersistent, m_strScriptPersistent.length(), 
					m_strScriptPersistent.c_str(), &v, NULL, SCRIPTTEXT_ISVISIBLE);
				LeaveScript();
			}
			break;
		case SCRIPTSTATE_INITIALIZED:
		case SCRIPTSTATE_DISCONNECTED:
			m_pWrapper->Disconnect(this);
			break;
		}
	}
	return hr;
}


void CRubyScript::Uninitialize()
{
	if (m_pGippedSite.IsOK())
	{
		m_pGippedSite.Unglobalize();
	}
#ifdef SCM_CAN_GLOBALIZE
	if (m_pGippedScM.IsOK())
	{
		m_pGippedScM.Unglobalize();
	}
#endif
	CRScriptCore::Uninitialize();
}

HRESULT STDMETHODCALLTYPE CRubyScript::Close( void)
{
	if (m_pGippedSite.IsOK())
	{
		m_pGippedSite.Unglobalize();
	}
#ifdef SCM_CAN_GLOBALIZE
	if (m_pGippedScM.IsOK())
	{
		m_pGippedScM.Unglobalize();
	}
#endif
	return CRScriptCore::Close();
}

HRESULT STDMETHODCALLTYPE CRubyScript::InterruptScriptThread( 
            /* [in] */ SCRIPTTHREADID stidThread,
            /* [in] */ const EXCEPINFO __RPC_FAR *pexcepinfo,
            /* [in] */ DWORD dwFlags)
{
	ATLTRACE(_T("InterruptScriptThread\n"));
	if (stidThread == SCRIPTTHREADID_ALL || stidThread == SCRIPTTHREADID_BASE || stidThread == m_dwThreadID)
	{
		if (CInitializer::InRuby())
		{
			HANDLE h = CRubyWrapper::GetCWrapper()->GetHandle();
			SuspendThread(h);
			InterruptThread(h);
			ResumeThread(h);
		}
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyScript::DefineGlobalObject( void)
{
	USES_CONVERSION;
	LPSTR pName = W2A(m_strGlobalObjectName.c_str());
	DefineGlobalProperties(pName);
	DefineGlobalMethods(pName);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyScript::Clone( 
            /* [out] */ IActiveScript __RPC_FAR *__RPC_FAR *ppscript)
{
	ATLTRACE(_T("Clone\n"));
	if (!ppscript) return E_POINTER;

	HRESULT hr = S_OK;

	CComObject<CRubyScript>* p;
	hr = CComObject<CRubyScript>::CreateInstance(&p);
	if (hr == S_OK)
	{
		hr = p->QueryInterface(IID_IActiveScript, (void**)ppscript);
		if (hr != S_OK)
		{
			delete p;
		}
		else
		{
			VALUE newmod = Qnil;
			m_pWrapper->rb_module_new(m_valueModule, &newmod);
			p->setModule(newmod);
			p->CopyNamedItem(m_mapItem);
			p->CopyPersistent(m_nStartLinePersistent, m_strScriptPersistent);
		}
	}
	return (hr == S_OK) ? S_OK : E_UNEXPECTED;
}

//
HRESULT CRubyScript::ParseText(int StartLine, LPCSTR pstrCode, LPCOLESTR pstrName, EXCEPINFO *pExcepInfo, VARIANT* pvarResult, DWORD dwFlags)
{
	ATLTRACE("parseText in %X\n", m_valueModule);
	EnterScript();
	HRESULT hr;
// Remote Scripting :
//    STATE_START
//         ParseScriptText -global ns-
//    STATE_UNINITIALIZED
//      - other engine
//    STATE_RUN
//      call parsed method
//    ...
#ifdef FAKE_PERSISTENT
	if (!pstrName || !*pstrName)
		hr = Invoke(rb_mKernel, StartLine, strlen(pstrCode), pstrCode, pvarResult, pExcepInfo, dwFlags);
	else
#endif
		hr = Invoke(m_valueModule, StartLine, strlen(pstrCode), pstrCode, pvarResult, pExcepInfo, dwFlags);
	LeaveScript();

	return hr;
}

//
// Call Ruby Wrapper through Proxy/Stub so, really invoking Ruby in Ruby's Own Thread.
//   - calling condition:
//			caller must call EnterScript()/LeaveScript() outside this function.
//
HRESULT CRubyScript::Invoke(VALUE val, int StartLine, int cbScript, LPCSTR pszScript, VARIANT* pvResult, EXCEPINFO FAR* pExcepInfo, DWORD dwFlags)
{
	IActiveScriptError* pError = NULL;
	VARIANT v;
	VARIANT* pv = pvResult;
	if (!pv)
	{
		pv = &v;
	}
	VariantInit(pv);

	ATLTRACE(_T("Invoke flag = %08X\n"), dwFlags);
	// Enter Ruby's thread
	// dwFlag = ASP -> 0xc0
	//          WSC -> 0x42
	//          IE  -> 0x82
	//          ScriptingControl -> 0x02
	ID id = ((dwFlags & (SCRIPTTEXT_ISPERSISTENT | SCRIPTTEXT_ISVISIBLE)) == (SCRIPTTEXT_ISPERSISTENT | SCRIPTTEXT_ISVISIBLE)) ? s_idModuleEval : s_idInstanceEval;
	HRESULT hr;
	int retry(MAX_RETRY);
	if (m_dwBaseSecurityLevel >= 1 || m_dwSecurityLevel >= 1)
	{
		static const char last[] = "\r\nend\r\nth.value";
		char* szScr = new char[cbScript + 64];
		int cb = wsprintfA(szScr, "th = Thread.start do\r\n$SAFE=%d\r\n", (m_dwSecurityLevel) ? m_dwSecurityLevel : m_dwBaseSecurityLevel);
		memcpy(szScr + cb, pszScript, cbScript);
		strcpy(szScr + cb + cbScript, last);
		do
		{
			hr = m_pWrapper->rb_funcall_with_string2(this, val, id, StartLine - 2, 
				cb + cbScript + sizeof(last), reinterpret_cast<unsigned char*>(szScr), false, pv, &pError);
			if (hr == RPC_E_CALL_REJECTED)
			{
				ForceChangeThreadContext();
				Sleep(100);
				retry--;
			}

		}
		while (hr == RPC_E_CALL_REJECTED && retry >= 0);
		delete[] szScr;
	}
	else
	{
		hr = m_pWrapper->rb_funcall_with_string2(this, val, id, StartLine, 
			cbScript + 1, reinterpret_cast<unsigned char*>(const_cast<LPSTR>(pszScript)), false, pv, &pError);
	}

	if (hr == S_OK)
	{
		if (pError)
		{
			OnScriptError(pError);
			pError->Release();
		}
		if (!pvResult)
			VariantClear(&v);
	}
	else
	{
		if (hr == RPC_E_CALL_REJECTED)
		{
			CScrError* pError = new CScrError("Script engine is too busy.");
			OnScriptError(pError);
			pError->Release();
		}
		if (pExcepInfo)
		{
			memset(pExcepInfo, 0, sizeof(EXCEPINFO));
			pExcepInfo->wCode = 0x0201;
			pExcepInfo->bstrSource = SysAllocString(L"RubyScript");
			pExcepInfo->bstrDescription = SysAllocString(L"Exception");
			pExcepInfo->scode = DISP_E_EXCEPTION;
		}
	}
	return hr;
}

// called by the site.
// Search Ruby class by pstrItemName
HRESULT CRubyScript::InternalGetScriptDispatch(LPCOLESTR pstrItemName, IDispatch** ppdisp)
{
	HRESULT hr = S_FALSE;
	*ppdisp = NULL;
	USES_CONVERSION;
	unsigned long val = Qnil;
	RubyObjMapIter it = m_mapObj.find(pstrItemName);
	if (it != m_mapObj.end())
	{
		*ppdisp = (*it).second;
		hr = S_OK;
	}
	else
	{
		hr = m_pWrapper->SearchClass(reinterpret_cast<LPBYTE>(W2A(pstrItemName)), &val);
		if (hr == S_OK)
		{
			if (val != Qnil)
			{
				CRubyObject* p = CRubyObject::CreateRubyObject(this, val);
				m_mapObj[pstrItemName] = p;
				*ppdisp = p;
				hr = S_OK;
			}
		}
	}
	return hr;
}

HRESULT STDMETHODCALLTYPE CRubyScript::CheckActiveXIsSafety(CLSID clsid)
{
	HRESULT hr = S_OK;
    if ((m_dwSafety & INTERFACE_USES_SECURITY_MANAGER) /*&& m_pGippedScM.IsOK()*/)
	{
#ifdef SCM_CAN_GLOBALIZE
		LIP(IInternetHostSecurityManager) lipscm(m_pGippedScM, hr);
		if (hr != S_OK) 
		{
			ATLTRACE(_("Failed localize SecurityManager %08X\n"), hr);
			return hr;
		}

		// Ask security manager if we can create objects.
#endif
		DWORD dwPolicy;
#ifdef SCM_CAN_GLOBALIZE
        if (SUCCEEDED(hr = lipscm->ProcessUrlAction(
#endif
		if (SUCCEEDED(hr = m_pScM->ProcessUrlAction(
			URLACTION_ACTIVEX_RUN, (BYTE *)&dwPolicy, sizeof(dwPolicy), 
			(BYTE *)&clsid, sizeof(clsid), 0, 0)))
		{
			if (URLPOLICY_ALLOW != dwPolicy)
				hr = ResultFromScode(E_ACCESSDENIED);
		}
	}
	return hr;
}

HRESULT STDMETHODCALLTYPE CRubyScript::EnterScript( void)
{
	CRScriptCore::EnterScript();
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CRubyScript::LeaveScript( void)
{
	CRScriptCore::LeaveScript();
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CRubyScript::OnScriptError( 
            /* [in] */ IActiveScriptError __RPC_FAR *pError)
{
	CRScriptCore::OnScriptError(pError);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyScript::RegisterServiceProvider( 
            /* [in] */ IServiceProvider __RPC_FAR *pProvider)
{
	PushServiceProvider(pProvider);
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CRubyScript::UnregisterServiceProvider( void)
{
	PopServiceProvider();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyScript::GetSecurityLevel( 
            /* [retval][out] */ long __RPC_FAR *pResult)
{
	if (!pResult) return E_POINTER;
	if (m_dwBaseSecurityLevel >= 1 || m_dwSecurityLevel >= 1)
	{
		*pResult = (m_dwSecurityLevel) ? m_dwSecurityLevel : m_dwBaseSecurityLevel;
	}
	else
	{
		*pResult = 0;
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyScript::CreateObject( 
            /* [in] */ CLSID clsid,
            /* [string][in] */ LPCWSTR pszHost,
            /* [in] */ REFIID riid,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk)
{
	HRESULT hr;

	*ppUnk = NULL;
	MULTI_QI mqi;
	mqi.pIID = &IID_IUnknown;
	mqi.pItf = NULL;
	if (pszHost)
	{
		COAUTHINFO cai = { RPC_C_AUTHN_NONE, RPC_C_AUTHZ_NONE, 0,
			RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IMPERSONATE, 0, EOAC_NONE };
		COSERVERINFO svrInfo = { 0, const_cast<OLECHAR*>(pszHost), &cai, 0 };
		hr = CoCreateInstanceEx(clsid, NULL, CLSCTX_REMOTE_SERVER, &svrInfo, 1, &mqi);
	}
	else
	{
		hr = CoCreateInstanceEx(clsid, NULL, CLSCTX_SERVER, NULL, 1, &mqi);
	}
    if (hr != S_OK)
	{
		return hr;
	}
	if (mqi.hr != S_OK)
		return mqi.hr;
	hr = CheckInterfaceIsSafety(clsid, mqi.pItf);
	if (hr != S_OK)
	{
		mqi.pItf->Release();
	}
	else
	{
		*ppUnk = mqi.pItf;
	}
	return hr;
}

HRESULT STDMETHODCALLTYPE CRubyScript::CheckInterfaceIsSafety( 
            /* [in] */ CLSID clsid,
            /* [in] */ IUnknown __RPC_FAR *pUnk)
{
	HRESULT hr = S_OK;
#if 0
    // Must goto LError after this point on failure to free *pUnk.
    if ((m_dwSafety & INTERFACE_USES_SECURITY_MANAGER) 
#ifdef SCM_CAN_GLOBALIZE
		&& m_pGippedScM.IsOK()
#endif
	)
	{
#ifdef SCM_CAN_GLOBALIZE
		LIP(IInternetHostSecurityManager) lipscm(m_pGippedScM, hr);
		if (hr != S_OK) 
		{
			ATLTRACE(_("Failed localize SecurityManager %08X\n"), hr);
			return hr;
		}
#endif
		// Query the security manager to see if this object is safe to use.
		DWORD dwPolicy, *pdwPolicy;
		DWORD cbPolicy;
		CONFIRMSAFETY csafe;
		csafe.pUnk = pUnk;
		csafe.clsid = clsid;
		csafe.dwFlags = 0;
#ifdef SCM_CAN_GLOBALIZE
		if (FAILED(hr = lipscm->QueryCustomPolicy(
			GUID_CUSTOM_CONFIRMOBJECTSAFETY, (BYTE**)&pdwPolicy,
			&cbPolicy, (LPBYTE)&csafe, sizeof(csafe), 0)))
#else
		if (FAILED(hr = m_pScM->QueryCustomPolicy(
			GUID_CUSTOM_CONFIRMOBJECTSAFETY, (BYTE**)&pdwPolicy,
			&cbPolicy, (LPBYTE)&csafe, sizeof(csafe), 0)))
#endif
		{
			return hr;
		}

		dwPolicy = URLPOLICY_DISALLOW;
		if (pdwPolicy)
		{
			if (sizeof(DWORD) <= cbPolicy)
				dwPolicy = *pdwPolicy;
			CoTaskMemFree(pdwPolicy);
		}

		if (URLPOLICY_ALLOW != dwPolicy)
		{
			return ResultFromScode(E_FAIL);
		}
	}
	else 
#endif
	if ((m_dwSafety & (INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACESAFE_FOR_UNTRUSTED_CALLER))
		|| rb_safe_level() >= 3)
	{
		// PERFORM OLD SAFETY CHECKS.
		IObjectSafety* pObjSafety = NULL;
		if (pUnk->QueryInterface(IID_IObjectSafety, (void**)&pObjSafety) != S_OK)
		{
			return E_ACCESSDENIED;
		}
		DWORD dwSafety(0), dwSupported(0);
		hr = pObjSafety->GetInterfaceSafetyOptions(IID_IDispatch, &dwSupported, &dwSafety);
		if (hr == S_OK)
		{
			dwSafety = m_dwSafety & dwSupported;
		} 
		else 
		{
			dwSupported = INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACESAFE_FOR_UNTRUSTED_CALLER;
			dwSafety = (m_dwSafety & (INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACESAFE_FOR_UNTRUSTED_CALLER));
		}
		hr = pObjSafety->SetInterfaceSafetyOptions(IID_IDispatch, dwSupported, dwSafety);
		pObjSafety->Release();
	}
	if (m_pScM)
	{
		IObjectWithSite* pSite = NULL;
		HRESULT hr = pUnk->QueryInterface(IID_IObjectWithSite, (void**)&pSite);
		if (hr == S_OK)
		{
			pSite->SetSite(m_pScM);
			pSite->Release();
		}
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////////
void CRubyScript::MakeScope()
{
	char sz[16];
	if (m_valueModule == Qnil)
	{
		m_pWrapper->rb_module_new(Qnil, &m_valueModule);
		wsprintf(sz, "X%08X", m_valueModule);
		m_pWrapper->RegisterObject(reinterpret_cast<LPBYTE>(sz), m_valueModule);
	}
}

void CRubyScript::DefineConstant(LPOLESTR name, VARIANT* pvar)
{
	ATLTRACE("constant %ls in %X\n", name, m_valueModule);
	USES_CONVERSION;
	LPSTR pname = W2A(name);
	*pname = toupper(*pname);
	m_pWrapper->DefineConstant(reinterpret_cast<LPBYTE>(pname), m_valueModule, pvar, this);
}
        
void CRubyScript::AddNamedItem(LPCOLESTR pstrName)
{
	ItemMapIter it = m_mapItem.find(pstrName);
	if (it == m_mapItem.end()) return;

	// Check NamedItem
	ATLTRACE(_T("AddNamedItem %ls\n"), pstrName);
	IDispatch* pDisp = (*it).second->GetDispatch(m_pSite, const_cast<LPOLESTR>(pstrName), GetCurrentThreadId() == m_dwThreadID);
	if (!pDisp) return;

	pDisp->Release();

	USES_CONVERSION;
	LPSTR pName = W2A(pstrName);
	int cb = lstrlenA(pName);
	LPSTR pCapitalizedName = pName;
	if (islower(*pName))
	{
		pCapitalizedName = lstrcpyA(reinterpret_cast<char*>(_alloca(cb + 1)), pName);
		*pCapitalizedName = toupper(*pName);
	}
	char* pstr = reinterpret_cast<char*>(_alloca(cb * 5 + 64));
	if (isupper(*pCapitalizedName))
		cb = sprintf(pstr, "@%s = WIN32OLEEX.attach(\"%s\", %d)\r\n%s = @%s\r\n", pName, pName, m_lCookie, pCapitalizedName, pName);
	else
		cb = sprintf(pstr, "@%s = WIN32OLEEX.attach(\"%s\", %d)\r\n", pName, pName, m_lCookie);
	if (m_strGlobalObjectName == pstrName)
	{
		cb += sprintf(pstr + cb, "@_asr_default_object = @%s", pName);
	}
	EnterScript();
	IActiveScriptError* pError = NULL;

	VARIANT v;
	VariantInit(&v);
	HRESULT hr;
	ATLTRACE(_("Enter AddNamedItem For %s\n"), pstr);
	do
	{
		hr = m_pWrapper->rb_funcall_with_string2(this, m_valueModule, s_idInstanceEval, 1,
			cb + 1, reinterpret_cast<unsigned char*>(pstr), false, &v, &pError);
		if (hr == RPC_E_CALL_REJECTED)
		{
			ForceChangeThreadContext();
		}
	}
	while (hr == RPC_E_CALL_REJECTED);
	if (m_strGlobalObjectName == pstrName)
	{
		do
		{
			hr = m_pWrapper->DefineGlobalObject(this);
			if (hr == RPC_E_CALL_REJECTED)
			{
				ForceChangeThreadContext();
			}
		}
		while (hr == RPC_E_CALL_REJECTED);
	}
	ATLTRACE(_("Leave AddNamedItem For %s\n"), pName);
	VariantClear(&v);
	if (hr == S_OK && pError)
	{
		OnScriptError(pError);
		pError->Release();
	}
	LeaveScript();
}
        
IDispatch* CRubyScript::GetOuterDispatch(IDispatch* pDisp)
{
	if (!m_pModuleObject)
	{
		ATLTRACE(_T("No Dispatch for GetOuterDispatch pDisp=%08X\n"), pDisp);
		m_pModuleObject = new CRubyObject(this, m_valueModule, false, pDisp);
		ATLTRACE(_T("No Dispatch for GetOuterDispatch - exit\n"));
	}
	return m_pModuleObject;
}

IDispatch* CRubyScript::GetGlobalDispatch()
{
#ifdef FAKE_PERSISTENT
	if (!m_pRubyObject)
		m_pRubyObject = new CRubyObject(this, rb_mKernel, false);
	return m_pRubyObject;
#else
	return GetOuterDispatch(NULL);
#endif
}

VALUE CRubyScript::GetModuleValue()
{
	return m_valueModule;
}

        
