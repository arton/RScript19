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
#define _INIT_GIPLIP
#include "RubyWrapper.h"
#include "RScriptCore.h"
#include "Win32Ole.h"
#include "Initializer.h"
#include "ScrError.h"
#include "RubyObject.h"

/////////////////////////////////////////////////////////////////////////////
// CRubyWrapper
GIT _init_git;
CRubyWrapper* CRubyWrapper::s_pWrapper = NULL;
GIP(IRubyWrapper) CRubyWrapper::s_gip;

HRESULT CRubyWrapper::FinalConstruct()
{
	CRScriptCore::InitializeEnvironment();
	try
	{
		m_idSize = ::rb_intern("size");
		m_idMethods = ::rb_intern("methods");
		m_idMethodDefined = ::rb_intern("method_defined?");
		m_idDelete = ::rb_intern("delete");
		m_valList = rb_hash_new();
		rb_global_variable(&m_valList);
	}
	catch (...)
	{
		ATLTRACE(_T("Exception for class\n"));
	}
	ATLTRACE(_T("RScript Initalizer was Created\n"));

	return S_OK;
}

void CRubyWrapper::FinalRelease()
{
	ATLTRACE(_T("CRubyWrapper::FinalRelease CurrentCount=%d\n"), m_listOle.size());
	for (OleListIter it = m_listOle.begin(); it != m_listOle.end(); it++)
	{
		oledata* pole = it->pole;
		if (pole->pDispatch)
		{
			pole->pDispatch->Release();
			pole->pDispatch = NULL;
		}
	}
	m_listOle.clear();
}

void CRubyWrapper::AddOle(VALUE module, oledata* pole)
{
	m_listOle.push_back(OlePair(pole, module));
}

bool CRubyWrapper::ReleaseOle(oledata* pole)
{
	bool released(false);
	for (OleListIter it = m_listOle.begin(); it != m_listOle.end(); it++)
	{
		if (it->pole == pole)
		{
			if (pole->pDispatch)
			{
				pole->pDispatch->Release();
				pole->pDispatch = NULL;
			}
			m_listOle.erase(it);
			released = true;
			break;
		}
	}
	return released;
}

void CRubyWrapper::Globalize()
{
	s_pWrapper = this;
	if (s_gip.IsOK())
	{
		Unglobalize();
	}
	HRESULT hr = s_gip.Globalize(this);
	ATLTRACE(_T("Create GIPPED Wrapper = %08X\n"), hr);
}

void CRubyWrapper::Unglobalize()
{
	if (s_gip.IsOK())
	{
		s_gip.Unglobalize();
	}
}

IRubyWrapper* CRubyWrapper::GetWrapper()
{
	IRubyWrapper* pResult = NULL;
	HRESULT hr = s_gip.Localize(&pResult);
	ATLTRACE(_T("Localize Wrapper = %08X, Thread=%08X\n"), hr, GetCurrentThreadId());
	return pResult;
}

CRubyWrapper* CRubyWrapper::GetCWrapper()
{
	return s_pWrapper;
}
//////////////////////////////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE CRubyWrapper::rb_intern( 
            /* [string][in] */ unsigned char __RPC_FAR *name,
            /* [out] */ unsigned long __RPC_FAR *pid)
{
	*pid = ::rb_intern(reinterpret_cast<char*>(name));
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CRubyWrapper::rb_module_new( 
            /* [in] */ unsigned long parent,
            /* [out] */ unsigned long __RPC_FAR *p)
{
	*p = ::rb_module_new();
	if (parent != Qnil)
	{
		rb_extend_object(*p, parent);
	}
	return S_OK;
}
        
static VALUE safe_funcall(VALUE Args)
{
	VALUE* pArgs = (VALUE*)Args;
	return rb_funcall2(*pArgs, *(pArgs + 1), 3, pArgs + 2);
}

HRESULT STDMETHODCALLTYPE CRubyWrapper::SearchMethod( 
			/* [in] */ unsigned long val,
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [out] */ DISPID __RPC_FAR *pDispID)
{
	ATLTRACE(_T("CRubyWrapper::SearchMethod = %hs\n"), pName);

	if (m_fTracing)
		CRScriptCore::TraceOff();
	HRESULT hr = S_OK;
#if 0
	VALUE obj = rb_funcall(val, m_idMethodDefined, 1, rb_str_new2(reinterpret_cast<char*>(pName)));
	if (obj == Qtrue)
	{
		*pDispID = ::rb_intern(reinterpret_cast<char*>(pName));
	}
	else
	{
		*pDispID = DISPID_UNKNOWN;
	}
#else
	VALUE obj = rb_funcall(val, m_idMethods, 0);
	int imax = FIX2INT(rb_funcall(obj, m_idSize, 0));
	*(pDispID) = DISPID_UNKNOWN;
	for (int j = 0; j < imax; j++)
	{
		VALUE v = rb_ary_entry(obj, j);
		char* pmname = StringValuePtr(v);
		if (stricmp(reinterpret_cast<char*>(pName), pmname) == 0)
		{
			*(pDispID) = ::rb_intern(pmname);
			break;
		}
	}
	if (*(pDispID) == DISPID_UNKNOWN)
	{
		hr = DISP_E_UNKNOWNNAME;
	}
#endif
	if (m_fTracing)
		CRScriptCore::TraceOn();
	return hr;
}

// search specified object
HRESULT STDMETHODCALLTYPE CRubyWrapper::SearchClass( 
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [out] */ unsigned long __RPC_FAR *pval)
{
	VALUE val = rb_eval_string(reinterpret_cast<char*>(pName));
	int tp = TYPE(val);
	if (tp == T_OBJECT || tp == T_CLASS || tp == T_ICLASS || tp == T_MODULE)
	{
		*pval = val;
	}
	else
	{
		*pval = Qnil;
	}
	return S_OK;
}

// Not generic, only for eval under val (module_eval or instance_eval)
// The evaluation itself is insecur but, the string always uses a thead that set safelevel > 2,
// so, the evaluation itself is did by safelevel = 0 setting.
HRESULT STDMETHODCALLTYPE CRubyWrapper::rb_funcall_with_string2( 
            /* [in] */ IRubyEngine __RPC_FAR *pengine,
            /* [in] */ unsigned long val,
            /* [in] */ unsigned long id,
            /* [in] */ long line,
            /* [in] */ long cb,
            /* [size_is][in] */ unsigned char __RPC_FAR *pstr,
            /* [in] */ boolean TraceRequired,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ IActiveScriptError __RPC_FAR *__RPC_FAR *ppError)
{
	IRubyEngine* pSavedEngine = m_pCurrentEngine;
	m_pCurrentEngine = pengine;
	VariantInit(pVarResult);
	ATLTRACE(_T("Wrapper::rb_funcall2 in Thread:%08X\n"), GetCurrentThreadId());
	int sstat(0);
	VALUE valArgs[5];
	valArgs[0] = val;
	valArgs[1] = id;
	valArgs[2] = rb_str_new((char*)pstr, cb);
	valArgs[3] = rb_str_new("ActiveScriptRuby", 16);
	valArgs[4] = INT2NUM(line);
	HRESULT hr = S_OK;
	try
	{
		if (TraceRequired)
		{
			m_fTracing = true;
			CRScriptCore::TraceOn();
		}
		ATLTRACE(_T(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> enter instance_eval\n"));
		int savesafe = ruby_safe_level;
		ruby_safe_level = 0;
		VALUE val = rb_protect(safe_funcall, (VALUE)&valArgs[0], &sstat);
		ruby_safe_level = savesafe;
		ATLTRACE(_T("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< leave instance_eval\n"));
		if (sstat)
		{
			*ppError = FetchErrorInfo(reinterpret_cast<LPCSTR>((line > 0) ? pstr : NULL));
		}
		else
		{
			STypeConvParam tparam;
			tparam.pengine = pengine;
			tparam.pvar = pVarResult;
			tparam.val = val;
			rb_protect(val2var, (VALUE)&tparam, &sstat);
			if (sstat)
			{
				*ppError = FetchErrorInfo(NULL);
			}
		}
		if (TraceRequired)
		{
			m_fTracing = false;
			CRScriptCore::TraceOff();
		}
	}
	catch (...)
	{
		hr = DISP_E_EXCEPTION;
	}
	m_pCurrentEngine = pSavedEngine;
	return hr;
}

HRESULT STDMETHODCALLTYPE CRubyWrapper::RegisterObject( 
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module)
{
	rb_hash_aset(m_valList, rb_str_new2(reinterpret_cast<LPSTR>(pName)), Module);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyWrapper::UnregisterObject( 
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module)
{
	for (OleListIter it = m_listOle.begin(); it != m_listOle.end();)
	{
		if (it->module == Module)
		{
			oledata* pole = it->pole;
			if (pole->pDispatch)
			{
				pole->pDispatch->Release();
				pole->pDispatch = NULL;
			}
			it = m_listOle.erase(it);
		}
		else
		{
			it++;
		}
	}

	rb_funcall(m_valList, m_idDelete, 1, rb_str_new2(reinterpret_cast<LPSTR>(pName)));
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRubyWrapper::rb_invoke( 
            /* [in] */ IRubyEngine* pengine,
            /* [in] */ unsigned long module,
            /* [in] */ unsigned long func,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ IActiveScriptError __RPC_FAR *__RPC_FAR *ppError)
{
	if (m_fTracing)
		CRScriptCore::TraceOff();

	HRESULT hr = S_OK;

	IRubyEngine* pSavedEngine = m_pCurrentEngine;
	m_pCurrentEngine = pengine;

	func = CRubyObject::ConvertDispID(module, func, DISPATCH_PROPERTYGET);

	register int args(pDispParams->cArgs);
	SInvokeParam* pParam = reinterpret_cast<SInvokeParam*>(_alloca(sizeof(SInvokeParam) + sizeof(VALUE) * args));
	pParam->pengine = pengine;
	pParam->module = (VALUE)module;
	pParam->id = (ID)func;
	pParam->cArgs = args;
	pParam->val[0] = Qnil;
	if (pParam->cArgs > 0)
	{
		for (int i = 0; i < args; i++)
		{
			pParam->val[i] = CRScriptCore::ole_variant2val(&pDispParams->rgvarg[args - i - 1], pengine);
		}
	}
	
	int sstat(0);
	try
	{
		VALUE val = rb_protect(InvokeRuby, (VALUE)pParam, &sstat);
		if (sstat)
		{
			*ppError = FetchErrorInfo(NULL);
		}
		else if (pVarResult)
		{
			if (func == DISPID_NEWENUM)
			{
				GetEnum(pengine, val, pVarResult, ppError);
			}
			else
			{
				GetValue(pengine, val, pVarResult, ppError);
			}
		}
	}
	catch (...)
	{
		hr = DISP_E_EXCEPTION;
	}

	m_pCurrentEngine = pSavedEngine;

	if (m_fTracing)
		CRScriptCore::TraceOn();

	return hr;
}

HRESULT STDMETHODCALLTYPE CRubyWrapper::Disconnect(IRubyEngine __RPC_FAR *pengine)
{
	return pengine->DisconnectObjects();
}

HRESULT STDMETHODCALLTYPE CRubyWrapper::DefineGlobalObject( 
            /* [in] */ IRubyEngine __RPC_FAR *pengine)
{
	IRubyEngine* pSavedEngine = m_pCurrentEngine;
	m_pCurrentEngine = pengine;

	HRESULT hr = pengine->DefineGlobalObject();

	m_pCurrentEngine = pSavedEngine;
	return hr;
}

HRESULT STDMETHODCALLTYPE CRubyWrapper::DefineConstant( 
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module,
            /* [in] */ VARIANT __RPC_FAR *pVar,
            /* [in] */ IRubyEngine __RPC_FAR *pengine)
{
	IRubyEngine* pSavedEngine = m_pCurrentEngine;
	m_pCurrentEngine = pengine;

	if (rb_is_const_id(::rb_intern(reinterpret_cast<LPSTR>(pName))))
	{
		rb_define_const(rb_singleton_class(Module), reinterpret_cast<LPSTR>(pName), CRScriptCore::ole_variant2val(pVar, pengine));
		ATLTRACE("constant %s is defined\n", pName);
	}

	m_pCurrentEngine = pSavedEngine;

	return S_OK;
}

IActiveScriptError* CRubyWrapper::FetchErrorInfo(LPCSTR pscript)
{
	IActiveScriptError* pResult = NULL;
	if (!NIL_P(ruby_errinfo))
	{
		if (pscript)
			pResult = new CScrError(ruby_errinfo, pscript);
		else
			pResult = new CScrError(ruby_errinfo);
	}
	return pResult;
}

void CRubyWrapper::GetValue(IRubyEngine *pengine, VALUE val, VARIANT* pVarResult, IActiveScriptError** ppError)
{
	int sstat(0);
	STypeConvParam tparam;
	tparam.pengine = pengine;
	tparam.pvar = pVarResult;
	tparam.val = val;
	rb_protect(val2var, (VALUE)&tparam, &sstat);
	if (sstat)
	{
		*ppError = FetchErrorInfo(NULL);
	}
}

void CRubyWrapper::GetEnum(IRubyEngine *pengine, VALUE val, VARIANT* pVarResult, IActiveScriptError** ppError)
{
	int sstat(0);
	VARIANT v;
	VariantInit(&v);
	STypeConvParam tparam;
	tparam.pengine = pengine;
	tparam.pvar = &v;
	tparam.val = val;
	rb_protect(val2var, (VALUE)&tparam, &sstat);
	if (sstat)
	{
		*ppError = FetchErrorInfo(NULL);
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
			p->Init(pVar, pVar + (pAry->rgsabound[0].cElements), NULL, AtlFlagCopy);
			p->QueryInterface(IID_IEnumVARIANT, (void**)&pVarResult->punkVal);
			pVarResult->vt = VT_UNKNOWN;
			SafeArrayUnlock(pAry);
			VariantClear(&v);
		}
	}
}

VALUE CRubyWrapper::add_item(VALUE item, VALUE ary)
{
	rb_ary_push(ary, item);
	return Qnil;
}

VALUE CRubyWrapper::InvokeRuby(unsigned long Param)
{
	SInvokeParam* pParam = (SInvokeParam*)Param;
	ATLTRACE(_T("CRubyWrapper::InvokeRuby  Thread:%08X\n"), GetCurrentThreadId());
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

VALUE CRubyWrapper::val2var(unsigned long Param)
{
	STypeConvParam* pParam = (STypeConvParam*)Param;
	CRScriptCore::ole_val2variant(pParam->val, pParam->pvar, pParam->pengine);
	return Qnil;
}

