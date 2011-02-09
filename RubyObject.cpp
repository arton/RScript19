/*
 *  Copyright(c) 2000,2006 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-14 02:21:15 +0900 (火, 14 11 2006) $
 */

#include "stdafx.h"
#ifdef _MERGE_PROXYSTUB
#include "RScript.h"
#include "RubyScript.h"
#include "RubyWrapper.h"
#else
#include "GRScript.h"
#include "RScriptCore.h"
#include "ScrError.h"
#endif

#include "win32ole.h"
#include "RubyObject.h"

/////////////////////////////////////////////////////////////////////////////
// RubyObject
CRubyObject* CRubyObject::CreateRubyObject(IRubyEngine* pengine, VALUE val)
{
	CRubyObject* pResult = NULL;
	pResult = new CRubyObject(pengine, val);
	return pResult;
}

CRubyObject::CRubyObject(IRubyEngine* pengine, VALUE val, bool fRubyObject, IDispatch* pDisp) :
	  m_pEngine(pengine), 
#ifndef __IRubyWrapper_INTERFACE_DEFINED__
	  m_idMethodDefined(rb_intern("respond_to?")),
	  m_idMethods(rb_intern("methods")),
	  m_idPrivMethods(rb_intern("private_methods")),
	  m_idSize(rb_intern("size")),
#else
	  m_pEngineVal(pengine),
#endif
	  m_valueKlass(val),
	  // AutoRelease Object Is Temporary Ruby Object (none global named item)
	  m_fRubyObject(fRubyObject),
	  m_lCount(1)
{
	ATLTRACE(_T("This Module = %08X, Dispatch=%08X\n"), val, pDisp);
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	char sz[24];
	wsprintfA(sz, "%08X%08X", pengine, val);
        IRubyWrapper* pWrapper = CRubyWrapper::GetWrapper();
	pWrapper->RegisterObject(reinterpret_cast<LPBYTE>(sz), val);
	pWrapper->Release();
        HRESULT hr = CoCreateFreeThreadedMarshaler(this, &m_pUnkMarshaler.p);
	ATLTRACE(_T("CRubyObject::CreateFreeThreadMarshaler Result = %08X\n"), hr);
#endif
	m_mapMethods["self"] = DISPID_VALUE;
        m_pDisp.p = pDisp;
}

CRubyObject::~CRubyObject()
{
	ATLTRACE(_T("Destroy RubyObject\n"));
	if (m_pDisp.p)
		  m_pDisp.Release();
	if (!m_fRubyObject && m_pEngine)
		m_pEngine->ClearModuleObject();
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	char sz[24];
	wsprintfA(sz, "%08X%08X", m_pEngineVal, m_valueKlass);
    IRubyWrapper* pWrapper = CRubyWrapper::GetWrapper();
	pWrapper->UnregisterObject(reinterpret_cast<LPBYTE>(sz), m_valueKlass);
	pWrapper->Release();
	m_pUnkMarshaler.Release();
#endif
}

HRESULT  STDMETHODCALLTYPE CRubyObject::QueryInterface(
		const IID & riid,  
		void **ppvObj)
{
	if (!ppvObj) return E_POINTER;
	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch) || IsEqualIID(riid, IID_IDispatchEx))
	{
		m_lCount++;
		*ppvObj = this;
		return S_OK;
	}
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	HRESULT hr = m_pUnkMarshaler.p->QueryInterface(riid, ppvObj);
	ATLTRACE(_T("FreeThreadMarshaler QI result = %08X\n"), hr);
	return hr;
#else
	return E_NOINTERFACE;
#endif
}

ULONG  STDMETHODCALLTYPE CRubyObject::AddRef()
{
	return ++m_lCount;
}

ULONG  STDMETHODCALLTYPE CRubyObject::Release()
{
	m_lCount--;
	if (m_lCount <= 0)
	{
		delete this;
		return 0;
	}
	return m_lCount;
}

// IDispatch
HRESULT STDMETHODCALLTYPE CRubyObject::GetTypeInfoCount( 
		/* [out] */ UINT __RPC_FAR *pctinfo)
{
	ATLTRACENOTIMPL(_T("CRubyObject::GetTypeInfoCount"));
}
        
HRESULT STDMETHODCALLTYPE CRubyObject::GetTypeInfo( 
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	ATLTRACENOTIMPL(_T("CRubyObject::GetTypeInfo"));
}
        
HRESULT STDMETHODCALLTYPE CRubyObject::GetIDsOfNames( 
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
		/* [in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
        if (!m_fRubyObject && m_pDisp.p)
        {
                HRESULT hr = m_pDisp->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);
                if (hr == S_OK) return hr;
        }
	IRubyWrapper* p = CRubyWrapper::GetWrapper();
#else
	int stacktop;
	if (reinterpret_cast<LPBYTE>(&stacktop) > CRScriptCore::GetCurrentEngine()->GetStackTop())
		_alloca(reinterpret_cast<LPBYTE>(&stacktop) - CRScriptCore::GetCurrentEngine()->GetStackTop());
#endif
	HRESULT hr = S_OK;
	USES_CONVERSION;
	for (int i = 0; i < cNames; i++)
	{
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
		IRubyEngine* pEngine = CRubyWrapper::GetCWrapper()->GetCurrentEngine();
		ATLTRACE(_T("GetIDsOfNames MyEngine:%08X, pEngine:%08X, RubyObject:%d, Name=%s\n"), m_pEngine, pEngine, m_fRubyObject, OLE2T(*(rgszNames + i)));
#endif
		LPSTR psz = W2A(*(rgszNames + i));
		RubyMethodMapIter it = m_mapMethods.find(psz);
		if (it != m_mapMethods.end())
		{
			DISPID dispid = (*it).second;
			*(rgDispId + i) = dispid;
			if (dispid == DISPID_UNKNOWN)
				hr = DISP_E_UNKNOWNNAME;
		}
		else
		{
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
			HRESULT h;
			do
			{
				h = p->SearchMethod(m_valueKlass, reinterpret_cast<unsigned char*>(psz), rgDispId + i);
				if (h == RPC_E_CALL_REJECTED)
				{
					CRScriptCore::ForceChangeThreadContext();
					Sleep(100);
				}
			}
			while (h == RPC_E_CALL_REJECTED);
			ATLTRACE(_T("Result of CRubyObject::SearchMethod = %08X\n"), h);
#else
			VALUE obj = rb_funcall(m_valueKlass, m_idMethods, 0);
			*(rgDispId + i) = searchMethod(obj, psz);
			if (*(rgDispId + i) == DISPID_UNKNOWN && m_valueKlass == rb_mKernel)
			{
				VALUE obj = rb_funcall(m_valueKlass, m_idPrivMethods, 0);
				*(rgDispId + i) = searchMethod(obj, psz);
			}
#endif
			if (*(rgDispId + i) == DISPID_UNKNOWN)
			{
				hr = DISP_E_UNKNOWNNAME;
			}
			else
				m_mapMethods[psz] = *(rgDispId + i);	// This makes to prevant to append new methods.

		}
	}
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	p->Release();
#endif
	return hr;
}

DISPID CRubyObject::searchMethod(VALUE obj, LPCSTR psz)
{
	int imax = FIX2INT(rb_funcall(obj, m_idSize, 0));
	for (int j = 0; j < imax; j++)
	{
		VALUE v = rb_ary_entry(obj, j);
		if (SYMBOL_P(v))
		{
			v = rb_sym_to_s(v);
		}
		char* pmname = StringValuePtr(v);
		ATLTRACE(_T("Kernel Func=%s\n"), pmname);
		if (stricmp(psz, pmname) == 0)
		{
			return ::rb_intern(pmname);
		}
	}
	return DISPID_UNKNOWN;
}
        
HRESULT STDMETHODCALLTYPE CRubyObject::Invoke( 
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
		/* [out] */ VARIANT __RPC_FAR *pVarResult,
		/* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
		/* [out] */ UINT __RPC_FAR *puArgErr)
{
	if (!pDispParams) return E_POINTER;
	if (pVarResult)
	{
		VariantInit(pVarResult);
	}
	ATLTRACE(_T("CRubyObject::Invoke dispid=%d\n"), dispIdMember);
	IActiveScriptError* pError = NULL;
	HRESULT hr = S_OK;
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (!m_fRubyObject)
	{
               if (m_pDisp.p)
               {
                   hr = m_pDisp->Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
                   if (hr == S_OK)
                   {
                       return hr;
                   }
               }
        }

        if (dispIdMember == DISPID_VALUE && wFlags == DISPATCH_METHOD)
	{
		dispIdMember = rb_intern("call");
	}
	IRubyWrapper* p = CRubyWrapper::GetWrapper();
	if (m_pEngine) 
	{
		m_pEngine->EnterScript();
		do
		{
			// This makes reentrant call, so never return RPC_E_CALL_REJECTED, here.
			hr = p->rb_invoke(m_pEngine, m_valueKlass, dispIdMember, pDispParams, pVarResult, &pError);
			if (hr == RPC_E_CALL_REJECTED) 
			{
				CRScriptCore::ForceChangeThreadContext();
			 	Sleep(100);
			}
		}
		while (hr == RPC_E_CALL_REJECTED);
	}
	p->Release();
	if (hr == S_OK)
	{
		if (pError)
		{
			if (m_pEngine) m_pEngine->OnScriptError(pError);
			pError->Release();
		}
	}
	else
	{
		if (pExcepInfo)
		{
			memset(pExcepInfo, 0, sizeof(EXCEPINFO));
			pExcepInfo->wCode = 0x0201;
			pExcepInfo->bstrSource = SysAllocString(L"RubyScript");
			pExcepInfo->bstrDescription = SysAllocString(L"Exception");
			pExcepInfo->scode = DISP_E_EXCEPTION;
		}
	}
#else
	int stacktop;
	if (reinterpret_cast<LPBYTE>(&stacktop) > CRScriptCore::GetCurrentEngine()->GetStackTop())
		_alloca(reinterpret_cast<LPBYTE>(&stacktop) - CRScriptCore::GetCurrentEngine()->GetStackTop());

	if (m_pEngine) 
	{
		m_pEngine->EnterScript();
	}
	dispIdMember = ConvertDispID(m_valueKlass, dispIdMember, wFlags);

	register int args(pDispParams->cArgs);
	SInvokeParam* pParam = reinterpret_cast<SInvokeParam*>(_alloca(sizeof(SInvokeParam) + sizeof(VALUE) * args));
	pParam->pengine = m_pEngine;
	pParam->module = (VALUE)m_valueKlass;
	pParam->id = (ID)dispIdMember;
	pParam->cArgs = args;
	pParam->val[0] = Qnil;
	if (pParam->cArgs > 0)
	{
		for (int i = 0; i < args; i++)
		{
			pParam->val[i] = CRScriptCore::ole_variant2val(&pDispParams->rgvarg[args - i - 1], m_pEngine);
		}
	}
	int sstat(0);
	try
	{
		VALUE val = rb_protect(InvokeRuby, (VALUE)pParam, &sstat);
		if (sstat)
		{
			pError = FetchErrorInfo();
			if (pError)
			{
				if (m_pEngine)
					m_pEngine->OnScriptError(pError);
				pError->Release();
			}
		}
		else if (pVarResult)
		{
			if (dispIdMember == DISPID_NEWENUM)
			{
				GetEnum(val, pVarResult, pError);
			}
			else
			{
				GetValue(val, pVarResult, pError);
			}
		}
	}
	catch (...)
	{
		if (pExcepInfo)
		{
			memset(pExcepInfo, 0, sizeof(EXCEPINFO));
			pExcepInfo->wCode = 0x0201;
			pExcepInfo->bstrSource = SysAllocString(L"RubyScript");
			pExcepInfo->bstrDescription = SysAllocString(L"Exception");
			pExcepInfo->scode = DISP_E_EXCEPTION;
		}
	}
#endif
	if (m_pEngine) m_pEngine->LeaveScript();

	return hr;
}

DISPID CRubyObject::ConvertDispID(VALUE module, DISPID id, WORD wFlags)
{
	if (id == DISPID_VALUE)
	{
		if (wFlags == DISPATCH_METHOD)
		{
			id = rb_intern("call");
		}
		else if (wFlags & DISPATCH_PROPERTYGET)
		{
			if (rb_respond_to(module, rb_intern("value")) == Qtrue)
			{
				id = rb_intern("value");
			}
		    else
			{
	         	id = rb_intern("to_s");
			}
		}
	}
	return id;
}

#ifndef __IRubyWrapper_INTERFACE_DEFINED__
void CRubyObject::GetValue(VALUE val, VARIANT* pVarResult, IActiveScriptError* pError)
{
	int sstat(0);
	STypeConvParam tparam;
	tparam.pengine = NULL;
	tparam.pvar = pVarResult;
	tparam.val = val;
	rb_protect(val2var, (VALUE)&tparam, &sstat);
	if (sstat)
	{
		pError = FetchErrorInfo();
		if (pError)
		{
			if (m_pEngine)
				m_pEngine->OnScriptError(pError);
			pError->Release();
		}
	}
}

void CRubyObject::GetEnum(VALUE val, VARIANT* pVarResult, IActiveScriptError* pError)
{
	int sstat(0);
	VARIANT v;
	VariantInit(&v);
	STypeConvParam tparam;
	tparam.pengine = NULL;
	tparam.pvar = &v;
	tparam.val = val;
	rb_protect(val2var, (VALUE)&tparam, &sstat);
	if (sstat)
	{
		pError = FetchErrorInfo();
		if (pError)
		{
			if (m_pEngine)
				m_pEngine->OnScriptError(pError);
			pError->Release();
		}
	}
	else
	{
		if (v.vt & VT_ARRAY)
		{
			SAFEARRAY* pAry = (v.vt & VT_BYREF) ? *v.pparray : v.parray;
			SafeArrayLock(pAry);
			typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > > enumvar;
			enumvar* p = new enumvar;
			VARIANT* pVar = reinterpret_cast<VARIANT*>(pAry->pvData);
			p->Init(pVar, pVar + (pAry->rgsabound[0].cElements), this, AtlFlagCopy);
			p->QueryInterface(IID_IEnumVARIANT, (void**)&pVarResult->punkVal);
			pVarResult->vt = VT_UNKNOWN;
			SafeArrayUnlock(pAry);
			VariantClear(&v);
		}
	}
}

IActiveScriptError* CRubyObject::FetchErrorInfo()
{
	IActiveScriptError* pResult = NULL;
        VALUE errinfo = rb_errinfo();
	if (!NIL_P(errinfo))
	{
		pResult = new CScrError(errinfo);
	}
	return pResult;
}

VALUE CRubyObject::add_item(VALUE item, VALUE ary)
{
	rb_ary_push(ary, item);
	return Qnil;
}

VALUE CRubyObject::InvokeRuby(VALUE Param)
{
	SInvokeParam* pParam = (SInvokeParam*)Param;
	ATLTRACE(_T("CRubyObject::InvokeRuby\n"));
	VALUE v;
	if (pParam->id == DISPID_NEWENUM)
	{
		VALUE ary = rb_ary_new();
		for (int i = 0; i < pParam->cArgs; i++)
		{
			rb_ary_push(ary, pParam->val[i]);
		}
		rb_iterate(rb_each, pParam->module, (VALUE(*)(...))add_item, ary);
		v = ary;
	}
	else
	{
		v = rb_funcall2(pParam->module, pParam->id, pParam->cArgs, pParam->val);
	}
	return v;
}

VALUE CRubyObject::val2var(unsigned long Param)
{
	STypeConvParam* pParam = (STypeConvParam*)Param;
	CRScriptCore::ole_val2variant(pParam->val, pParam->pvar, pParam->pengine);
	return Qnil;
}
#endif
