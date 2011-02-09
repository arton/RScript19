/*
 *  Copyright(c) 2000, 2006 arton
 *
 *  $Date: 2006-12-03 04:56:14 +0900 (日, 03 12 2006) $
 */
/*
 *  (c) 1995 Microsoft Corporation. All rights reserved.
 *  Developed by ActiveWare Internet Corp., http://www.ActiveWare.com
 *
 *  Other modifications Copyright (c) 1997, 1998 by Gurusamy Sarathy
 *  <gsar@umich.edu> and Jan Dubois <jan.dubois@ibm.net>
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License or the Artistic License, as specified in the README file
 *  of the Perl distribution.
 *
 */

/*
  $Date: 2006-12-03 04:56:14 +0900 (日, 03 12 2006) $
  modified for win32ole (ruby) by Masaki.Suketa <masaki.suketa@nifty.ne.jp>
 */

#include "stdafx.h"
#ifdef _MERGE_PROXYSTUB
#include "RScript.h"
#else
#include "GRScript.h"
#endif
#include "RScriptCore.h"

#include "win32ole.h"
#include "Initializer.h"
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
#include "RubyWrapper.h"
#endif
#include "RubyObject.h"
/////////////////////////////////////////////////////////////////////////////
// CRubyScript
VALUE CRScriptCore::s_valueWin32Ole;
VALUE CRScriptCore::s_valueWin32OleEx;
VALUE CRScriptCore::s_valueWIN32OLERuntimeError;
VALUE CRScriptCore::s_valueActiveRubyScript;
bool CRScriptCore::s_fTrace(false);
bool CRScriptCore::s_fRubyize(false);

// Helper Functions from Win32Ole
 /*
  $Date: 2006-12-03 04:56:14 +0900 (日, 03 12 2006) $
  modified for win32ole (ruby) by M.Suketa <CQN02273@nifty.ne.jp>
 */
// modified for ActiveScript (ruby) by arton
//
VALUE __cdecl CRScriptCore::rubyize(VALUE, VALUE v)
{
	bool f;
	switch (v)
	{
	case Qtrue:
		f = true;
		break;
	case Qfalse:
		f = false;
		break;
	default:
        rb_raise(rb_eTypeError, "not valid value");
		break;
	}
#ifdef __IRubyEngine_INTERFACE_DEFINED__
	IRubyEngine* pEngine = CRubyWrapper::GetCWrapper()->GetCurrentEngine();
	if (!pEngine) return Qnil;
	pEngine->SetRubyize(f);
#else
	s_fRubyize = f;
#endif
	return v;
}

void __cdecl CRScriptCore::ole_free(oledata* pole)
{
	ATLTRACE(_("Freeing Win32OleEx Object %08x\n"), pole);
	if(pole->pDispatch) {
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
		CRubyWrapper::GetCWrapper()->ReleaseOle(pole);
		// never call Release, because not explicit addrefed
#else
		pole->pDispatch->Release();
		pole->pDispatch = NULL;
#endif
    }
}

VALUE CRScriptCore::ole_ary_m_entry(VALUE val, long* pid)
{
    VALUE obj = Qnil;
    int i = 0;
    obj = val;
    while(TYPE(obj) == T_ARRAY) {
        obj = rb_ary_entry(obj, pid[i]);
        i++;
    }
    return obj;
}

void CRScriptCore::ole_set_safe_array(long n, SAFEARRAY* psa, long* pid, long* pub, VALUE val, long dim, IRubyEngine* pEngine)
{
    if(n < 0) return;
    VALUE val1;
    VARIANT var;
    VariantInit(&var);
    if(n == dim) {
        val1 = ole_ary_m_entry(val, pid);
        ole_val2variant(val1, &var, pEngine);
        SafeArrayPutElement(psa, pid, &var);
    }
    pid[n] += 1;
    if (pid[n] < pub[n]) {
        ole_set_safe_array(dim, psa, pid, pub, val, dim, pEngine);
    }
    else {
        pid[n] = 0;
        ole_set_safe_array(n-1, psa, pid, pub, val, dim, pEngine);
    }
}

void CRScriptCore::ole_val2variant(VALUE val, VARIANT* var, IRubyEngine* pEngine, VARTYPE nil_to)
{
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (pEngine && pEngine->GetRubyize() == S_OK)
	{
		pEngine->SetRubyize(false);
		V_VT(var) = VT_DISPATCH;
		V_DISPATCH(var) = CRubyObject::CreateRubyObject(pEngine, val);
		return;
	}
#else
	if (s_fRubyize)
	{
		s_fRubyize = false;
		V_VT(var) = VT_DISPATCH;
		V_DISPATCH(var) = CRubyObject::CreateRubyObject(pEngine, val);
		return;
	}
#endif

	USES_CONVERSION;

    oledata *pole;
    if(rb_obj_is_kind_of(val, s_valueWin32Ole) || rb_obj_is_kind_of(val, s_valueWin32OleEx)) {
        Data_Get_Struct(val, struct oledata, pole);
        pole->pDispatch->AddRef();
        V_VT(var) = VT_DISPATCH;
        V_DISPATCH(var) = pole->pDispatch;
        return;
    }

    if (rb_obj_is_kind_of(val, rb_cTime)) {
		SYSTEMTIME st;
		ZeroMemory(&st, sizeof(SYSTEMTIME));
		st.wYear = (WORD)FIX2INT(rb_funcall(val, rb_intern("year"), 0));
		st.wMonth = (WORD)FIX2INT(rb_funcall(val, rb_intern("month"), 0));
		st.wDay = (WORD)FIX2INT(rb_funcall(val, rb_intern("mday"), 0));
		st.wHour = (WORD)FIX2INT(rb_funcall(val, rb_intern("hour"), 0));
		st.wMinute = (WORD)FIX2INT(rb_funcall(val, rb_intern("min"), 0));
		st.wSecond = (WORD)FIX2INT(rb_funcall(val, rb_intern("sec"), 0));
        V_VT(var) = VT_DATE;
        SystemTimeToVariantTime(&st, &var->dblVal);
        return;
    }
	ATLTRACE(_T("Type Of val = %08X\n"), TYPE(val));
    switch (TYPE(val)) {
    case T_ARRAY:
    {
        long dim(0);
        int  i(0);

        VALUE val1 = val;
        while(TYPE(val1) == T_ARRAY) {
            val1 = rb_ary_entry(val1, 0);
            dim += 1;
        }
        SAFEARRAYBOUND* psab = reinterpret_cast<SAFEARRAYBOUND*>(_alloca(dim * sizeof(SAFEARRAYBOUND)));

		long* pub = reinterpret_cast<long*>(_alloca(dim * sizeof(long)));
		long* pid = reinterpret_cast<long*>(_alloca(dim * sizeof(long)));
        val1 = val;
        i = 0;
        while(TYPE(val1) == T_ARRAY) {
            psab[i].cElements = RARRAY_LEN(val1);
            psab[i].lLbound = 0;
            pub[i] = psab[i].cElements;
            pid[i] = 0;
            i ++;
            val1 = rb_ary_entry(val1, 0);
        }
        /* Create and fill VARIANT array */
        SAFEARRAY* psa = SafeArrayCreate(VT_VARIANT, dim, psab);
        if (psa == NULL)
		{
			V_VT(var) = VT_ERROR;
			V_ERROR(var) = E_OUTOFMEMORY;
		}
        else
		{
            SafeArrayLock(psa);
            ole_set_safe_array(dim-1, psa, pid, pub, val, dim-1, pEngine);
            SafeArrayUnlock(psa);
            V_VT(var) = VT_VARIANT | VT_ARRAY;
            V_ARRAY(var) = psa;
        }
        break;
    }
    case T_STRING:
        V_VT(var) = VT_BSTR;
        V_BSTR(var) = SysAllocString(A2W(StringValuePtr(val)));
        break;
    case T_FIXNUM:
        V_VT(var) = VT_I4;
        V_I4(var) = NUM2INT(val);
        break;
    case T_BIGNUM:
		V_VT(var) = VT_R8;
        V_R8(var) = rb_big2dbl(val);
		break;
    case T_FLOAT:
        V_VT(var) = VT_R8;
        V_R8(var) = NUM2DBL(val);
        break;
    case T_TRUE:
        V_VT(var) = VT_BOOL;
        V_BOOL(var) = VARIANT_TRUE;
        break;
    case T_FALSE:
        V_VT(var) = VT_BOOL;
        V_BOOL(var) = VARIANT_FALSE;
        break;
    case T_NIL:
		if (nil_to == VT_ERROR)
		{
			V_VT(var) = VT_ERROR;
			V_ERROR(var) = DISP_E_PARAMNOTFOUND;
		}
		else
		{
			V_VT(var) = VT_EMPTY;
		}
        break;
    default:
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
		if (pEngine)
		{
			V_VT(var) = VT_DISPATCH;
			V_DISPATCH(var) = CRubyObject::CreateRubyObject(pEngine, val);
		}
		else
		{
			ATLTRACE(_T("Without pEngine, ole_val2variant was called\n"));
			VariantInit(var);	// VT_EMPTY
		}
#else
		V_VT(var) = VT_DISPATCH;
		V_DISPATCH(var) = CRubyObject::CreateRubyObject(pEngine, val);
#endif
        break;
    }
}

VALUE CRScriptCore::ole_variant2val(VARIANT* pvar, IRubyEngine* pEngine, IServiceProvider* pProv)
{
	USES_CONVERSION;
    VALUE obj = Qnil;
    HRESULT hr;

    while ( V_VT(pvar) == (VT_BYREF | VT_VARIANT) )
        pvar = V_VARIANTREF(pvar);
        
    if(V_ISARRAY(pvar)) {
        SAFEARRAY *psa = V_ISBYREF(pvar) ? *V_ARRAYREF(pvar) : V_ARRAY(pvar);
        long i;
        long *pID, *pLB, *pUB;
        VARIANT variant;
        VALUE val;
        VALUE val2;

        int dim = SafeArrayGetDim(psa);
        VariantInit(&variant);
        V_VT(&variant) = (V_VT(pvar) & ~VT_ARRAY) | VT_BYREF;

        pID = reinterpret_cast<long*>(_alloca(dim * sizeof(long)));
        pLB = reinterpret_cast<long*>(_alloca(dim * sizeof(long)));
        pUB = reinterpret_cast<long*>(_alloca(dim * sizeof(long)));

        obj = Qnil;

        for(i = 0; i < dim; ++i) {
            SafeArrayGetLBound(psa, i+1, &pLB[i]);
            SafeArrayGetLBound(psa, i+1, &pID[i]);
            SafeArrayGetUBound(psa, i+1, &pUB[i]);
        }

        hr = SafeArrayLock(psa);
        if (SUCCEEDED(hr)) {
            val2 = rb_ary_new();
            while (i >= 0) {
                hr = SafeArrayPtrOfIndex(psa, pID, &V_BYREF(&variant));
                if (FAILED(hr))
                    break;

                val = ole_variant2val(&variant, pEngine);
                rb_ary_push(val2, val);
                for (i = dim-1 ; i >= 0 ; --i) {
                    if (++pID[i] <= pUB[i])
                        break;

                    pID[i] = pLB[i];
                    if (i > 0) {
						if (obj == Qnil)
							obj = rb_ary_new();
                        rb_ary_push(obj, val2);
                        val2 = rb_ary_new();
                    }
                }
            }
            SafeArrayUnlock(psa);
        }
        return (obj == Qnil) ? val2 : obj;
    }

    switch(V_VT(pvar) & ~VT_BYREF){
    case VT_EMPTY:
        break;
    case VT_NULL:
        break;
    case VT_UI1:
        if(V_ISBYREF(pvar)) 
            obj = INT2NUM((long)*V_UI1REF(pvar));
        else 
            obj = INT2NUM((long)V_UI1(pvar));
        break;

    case VT_I2:
        if(V_ISBYREF(pvar))
            obj = INT2NUM((long)*V_I2REF(pvar));
        else 
            obj = INT2NUM((long)V_I2(pvar));
        break;

    case VT_I4:
        if(V_ISBYREF(pvar))
            obj = INT2NUM((long)*V_I4REF(pvar));
        else 
            obj = INT2NUM((long)V_I4(pvar));
        break;

    case VT_R4:
        if(V_ISBYREF(pvar))
            obj = rb_float_new(*V_R4REF(pvar));
        else
            obj = rb_float_new(V_R4(pvar));
        break;

    case VT_R8:
        if(V_ISBYREF(pvar))
            obj = rb_float_new(*V_R8REF(pvar));
        else
            obj = rb_float_new(V_R8(pvar));
        break;

    case VT_BSTR:
    {
        char *p;
		LPOLESTR pw;
        if(V_ISBYREF(pvar))
            pw = *V_BSTRREF(pvar);
        else
            pw = V_BSTR(pvar);
		if (!pw) p = "";
		else p = W2A(pw);
        obj = rb_str_new2(p);
        break;
    }

    case VT_ERROR:
        if(V_ISBYREF(pvar))
            obj = INT2NUM(*V_ERRORREF(pvar));
        else
            obj = INT2NUM(V_ERROR(pvar));
        break;

    case VT_BOOL:
        if (V_ISBYREF(pvar))
            obj = (*V_BOOLREF(pvar) ? Qtrue : Qfalse);
        else
            obj = (V_BOOL(pvar) ? Qtrue : Qfalse);
        break;

    case VT_DISPATCH:
    {
        IDispatch *pDispatch;

        if (V_ISBYREF(pvar))
            pDispatch = *V_DISPATCHREF(pvar);
        else
            pDispatch = V_DISPATCH(pvar);

		obj = ole_createWin32OleEx(pDispatch, pEngine, pProv);
        break;
    }

    case VT_UNKNOWN:
    {
        /* get IDispatch interface from IUnknown interface */
        IUnknown *punk;

        if (V_ISBYREF(pvar))
            punk = *V_UNKNOWNREF(pvar);
        else
            punk = V_UNKNOWN(pvar);

		obj = ole_createWin32OleEx(punk, pEngine, pProv);
        break;
    }

    case VT_DATE:
    {
        DATE date;
        if(V_ISBYREF(pvar))
            date = *V_DATEREF(pvar);
        else
            date = V_DATE(pvar);
		SYSTEMTIME st;
		VariantTimeToSystemTime(date, &st);
		char szTime[20];
		int cb = wsprintfA(szTime,
            "%4.4d/%02.2d/%02.2d %02.2d:%02.2d:%02.2d",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        obj =  rb_str_new(szTime, cb);
        break;
    }
    case VT_CY:
    default:
    {
        VARIANT variant;
        VariantInit(&variant);
        HRESULT hr = VariantChangeTypeEx(&variant, pvar, 
                                  LOCALE_SYSTEM_DEFAULT, 0, VT_BSTR);
        if (hr == S_OK && V_VT(&variant) == VT_BSTR) {
            char *p = W2A(V_BSTR(&variant));
            obj = rb_str_new2(p);
        }
        VariantClear(&variant);
        break;
    }
    }
    return obj;
}

VALUE CRScriptCore::ole_createWin32OleEx(IUnknown* pUnk, IRubyEngine* pEngine, IServiceProvider* pProv)
{
	VALUE obj = Qnil;
	if (!pUnk) return obj;
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	_ASSERT(pEngine);
	VALUE module;
	pEngine->GetModule(&module);
	CRubyWrapper* pWrapper = CRubyWrapper::GetCWrapper();
	IDispatchEx* pDispEx;
	if (pProv && pUnk->QueryInterface(IID_IDispatchEx, (void**)&pDispEx) == S_OK)
	{
		oledataex* polex;
		obj = Data_Make_Struct(s_valueWin32OleEx, struct oledataex,
			0, (void(*)(void))ole_free, polex);
		polex->pDispatchEx = pDispEx;
		polex->pServiceProvider = pProv;
		polex->pEngine = pEngine;
		pWrapper->AddOle(module, polex);
		return obj;
	}
#endif
	IDispatch *pDispatch;
	if (pUnk->QueryInterface(IID_IDispatch, (void **)&pDispatch) == S_OK)
	{
		struct oledataex *pole;
		obj = Data_Make_Struct(s_valueWin32OleEx, struct oledataex,
			0, (void(*)(void))ole_free,pole);
		pole->pDispatch = pDispatch;
		pole->pEngine = pEngine;
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
		pWrapper->AddOle(module, pole);
#endif
	}
	return obj;
}

VALUE CRScriptCore::ole_hresult2msg(HRESULT hr)
{
    VALUE msg = Qnil;
    char *p_msg;
    DWORD dwCount;

    dwCount = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                            FORMAT_MESSAGE_FROM_SYSTEM |
                            FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL, hr, LOCALE_SYSTEM_DEFAULT,
                            (LPTSTR)&p_msg, 0, NULL);
    if (dwCount > 0) {
		/* remove dots and CRs/LFs */
		while (dwCount > 0 &&
               (p_msg[dwCount-1] < ' ' || p_msg[dwCount-1] == '.')) {
			p_msg[--dwCount] = '\0';
		}
		if (p_msg[0] != '\0') {
			msg = rb_str_new2(p_msg);
		}
    }
    return msg;
}

VALUE CRScriptCore::ole_excepinfo2msg(EXCEPINFO* pExInfo)
{
	USES_CONVERSION;
    char error_code[40];
    char *pSource = NULL;
    char *pDescription = NULL;
    VALUE error_msg;
    if(pExInfo->pfnDeferredFillIn != NULL) {
        (*pExInfo->pfnDeferredFillIn)(pExInfo);
    }
    if (pExInfo->bstrSource != NULL) {
        pSource = W2A(pExInfo->bstrSource);
		SysFreeString(pExInfo->bstrSource);
    }
    if (pExInfo->bstrDescription != NULL) {
        pDescription = W2A(pExInfo->bstrDescription);
		SysFreeString(pExInfo->bstrDescription);
    }
    if(pExInfo->wCode == 0) {
        sprintf(error_code, "\n    OLE rb_compile_error:%lX in ", pExInfo->scode);
    }
    else{
        sprintf(error_code, "\n    OLE rb_compile_error:%u in ", pExInfo->wCode);
    }
    error_msg = rb_str_new2(error_code);
    if(pSource != NULL) {
        rb_str_cat(error_msg, pSource, strlen(pSource));
    }
    else {
        rb_str_cat(error_msg, "<Unknown>", 9);
    }
    rb_str_cat(error_msg, "\n    ", 5);
    if(pDescription != NULL) {
        rb_str_cat(error_msg, pDescription, strlen(pDescription));
    }
    else {
        rb_str_cat(error_msg, "<No Description>", 16);
    }
	if (pExInfo->bstrHelpFile != NULL) {
		SysFreeString(pExInfo->bstrHelpFile);
	}
    return error_msg;
}

void CRScriptCore::ole_raise(HRESULT hr, VALUE ecs, LPCSTR fmt, ...)
{
    va_list args;
    char buf[512];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf)/sizeof(char), fmt, args);
    va_end(args);

    VALUE err_msg = ole_hresult2msg(hr);
    if(err_msg != Qnil) {
        rb_raise(ecs, "%s\n%s", buf, StringValuePtr(err_msg));
    }
    else {
        rb_raise(ecs, "%s", buf);
    }
}

//
// Check Newly Create Object is safe or not.
//
VALUE __cdecl CRScriptCore::fole_s_new(int argc, VALUE* argv, VALUE self)
{
    VALUE err_msg, svr_name, host_name;
    CLSID   clsid;
    IDispatch *pDispatch = NULL;
    IDispatchEx *pDispatchEx = NULL;
    VALUE obj;

	rb_scan_args(argc, argv, "11", &svr_name, &host_name);

	USES_CONVERSION;
    /* get CLSID from OLE server name */
    LPOLESTR pBuf  = A2W(StringValuePtr(svr_name));
    HRESULT hr = CLSIDFromProgID(pBuf, &clsid);
    if(hr != S_OK)
	{
        err_msg = rb_str_new2("Unknown OLE server : ");
        rb_str_concat(err_msg, svr_name);
        ole_raise(hr, s_valueWIN32OLERuntimeError, StringValuePtr(err_msg));
    }
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	CRubyWrapper* pWrapper = CRubyWrapper::GetCWrapper();
	IRubyEngine* pEngine = pWrapper->GetCurrentEngine();
	if (pEngine)
	{
		HRESULT hr = pEngine->CheckActiveXIsSafety(clsid);
		if (hr != S_OK)
		{
			err_msg = rb_str_new2("Running ActiveX is disallowed : ");
			rb_str_concat(err_msg, svr_name);
			ole_raise(hr, s_valueWIN32OLERuntimeError, StringValuePtr(err_msg));
		}
	}
#endif

    /* get IDispatch interface */
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (pEngine)
	{
		IUnknown* pUnk = NULL;
		LPWSTR psz;
		if (host_name == Qnil)
			psz = NULL;
		else
			psz = A2W(StringValuePtr(host_name));
		hr = pEngine->CreateObject(clsid, psz, IID_IUnknown, &pUnk);
		if (hr == S_OK)
		{
			hr = pUnk->QueryInterface(IID_IDispatchEx, (void**)&pDispatchEx);
			if (hr != S_OK)
			{
				pDispatchEx = NULL;
				hr = pUnk->QueryInterface(IID_IDispatch, (void**)&pDispatch);
			}
			pUnk->Release();
		}
	}
	else
	{
#endif
		MULTI_QI mqi;
		mqi.pIID = &IID_IDispatch;
		mqi.pItf = NULL;
		COSERVERINFO svrInfo;
		if (host_name != Qnil)
		{
			memset(&svrInfo, 0, sizeof(COSERVERINFO));
			svrInfo.pwszName = A2W(StringValuePtr(host_name));
			hr = CoCreateInstanceEx(clsid, NULL, CLSCTX_REMOTE_SERVER, &svrInfo, 1, &mqi);
		}
		else
		{
			hr = CoCreateInstanceEx(clsid, NULL, CLSCTX_SERVER, NULL, 1, &mqi);
		}
		if (hr == S_OK)
		{
			hr = mqi.hr;
			pDispatch = reinterpret_cast<IDispatch*>(mqi.pItf);
		}
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	}
#endif
    if(FAILED(hr) || (!pDispatch && !pDispatchEx))
	{
        err_msg = rb_str_new2("Fail to create WIN32OLE object from ");
        rb_str_concat(err_msg, svr_name);
        ole_raise(hr, s_valueWIN32OLERuntimeError, StringValuePtr(err_msg));
    }

    /* create WIN32OLE or WIN32OLEEX object */
	oledataex *pole;
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	obj = Data_Make_Struct(self, oledataex, 0, (void(*)(void))ole_free, pole);
	if (pDispatchEx)
	{
		pole->pDispatchEx = pDispatchEx;
	}
	else
	{
		pole->pDispatch = pDispatch;
	}
	if (pEngine)
	{
		VALUE module;
		pEngine->GetModule(&module);
		pWrapper->AddOle(module, pole);
		pole->pEngine = pEngine;
		if (pDispatchEx)
		{
			IServiceProvider* p;
			if (pEngine->QueryInterface(IID_IServiceProvider, (void**)&p) == S_OK)
			{
				pole->pServiceProvider = p;
				// Release because the extent of this object is same as the engine.
				p->Release();
			}
		}
	}
#else
	obj = Data_Make_Struct(s_valueWin32OleEx, struct oledataex,0,(void(*)(void))ole_free,pole);
	pole->pDispatch = pDispatch;
#endif
    return obj;
}

VALUE __cdecl CRScriptCore::fole_s_connect(VALUE self, VALUE svr_name)
{
	if (rb_safe_level() >= 3)
	{
		rb_raise(rb_eSecurityError, "Connecting ActiveX is disallowed.");
	}

    CLSID   clsid;
    IDispatch *pDispatch;
	LPWSTR pDispName = NULL;
    IUnknown *pUnknown;
    VALUE obj;
    VALUE err_msg;
	HRESULT hr;

	USES_CONVERSION;
	// Check New moniker
	if (strchr(StringValuePtr(svr_name), ':') > 0
		&& strrchr(StringValuePtr(svr_name), ':')  > 0)
	{
		pDispName = A2W(StringValuePtr(svr_name));
	}
	else
	{
		/* get CLSID from OLE server name */
		LPOLESTR pBuf  = A2W(StringValuePtr(svr_name));
		hr = CLSIDFromProgID(pBuf, &clsid);
		if(hr != S_OK)
		{
			if (hr != S_OK)
			{
				err_msg = rb_str_new2("Unknown OLE server : ");
				rb_str_concat(err_msg, svr_name);
				ole_raise(hr, s_valueWIN32OLERuntimeError, StringValuePtr(err_msg));
			}
		}
    }
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	IRubyEngine* pEngine = CRubyWrapper::GetCWrapper()->GetCurrentEngine();
	if (pEngine)
	{
		hr = pEngine->CheckActiveXIsSafety(clsid);
		if (hr != S_OK)
		{
			err_msg = rb_str_new2("Running ActiveX is disallowed : ");
			rb_str_concat(err_msg, svr_name);
			ole_raise(hr, s_valueWIN32OLERuntimeError, StringValuePtr(err_msg));
		}
	}
#endif
	if (pDispName)
		hr = CoGetObject(pDispName, NULL, IID_IUnknown, (void**)&pUnknown);
	else
		hr = GetActiveObject(clsid, 0, &pUnknown);
    if (FAILED(hr)) {
        err_msg = rb_str_new2("Not Running OLE server : ");
        rb_str_concat(err_msg, svr_name);
        ole_raise(hr, s_valueWIN32OLERuntimeError, StringValuePtr(err_msg));
    }
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (pEngine)
	{
		hr = pEngine->CheckInterfaceIsSafety(clsid, pUnknown);
		if (hr != S_OK)
		{
			pUnknown->Release();
			err_msg = rb_str_new2("Fail to connect WIN32OLE server : ");
			rb_str_concat(err_msg, svr_name);
			ole_raise(hr, s_valueWIN32OLERuntimeError, StringValuePtr(err_msg));
		}
	}
#endif
	hr = pUnknown->QueryInterface(IID_IDispatch, (void **)&pDispatch);
	if(FAILED(hr)) {
		pUnknown->Release();
		err_msg = rb_str_new2("Fail to create WIN32OLE server : ");
		rb_str_concat(err_msg, svr_name);
		ole_raise(hr, s_valueWIN32OLERuntimeError, StringValuePtr(err_msg));
	}
	pUnknown->Release();

    /* create WIN32OLE object */
	struct oledataex *pole;
	obj = Data_Make_Struct(s_valueWin32OleEx,struct oledataex,0,(void(*)(void))ole_free,pole);
	pole->pDispatch = pDispatch;
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	pole->pEngine = pEngine;
	if (pEngine)
	{
		VALUE module;
		pEngine->GetModule(&module);
		CRubyWrapper::GetCWrapper()->AddOle(module, pole);
	}
#endif	
    return obj;
}

//
// Attach is always allowed because the objects are given by IE itself.
//
VALUE __cdecl CRScriptCore::foleex_attach(VALUE self, VALUE ItemName, VALUE Cookie)
{
    ATLTRACE(_T("attach %s - %d\n"), StringValuePtr(ItemName), FIX2LONG(Cookie));
	CRScriptCore* p = CRScriptCore::GetEngine(FIX2LONG(Cookie));
	if (!p) return Qnil;
	return p->GetOleObject(self, StringValuePtr(ItemName));
}

VALUE CRScriptCore::fole_propertyput(VALUE self, VALUE property, VALUE value)
{
    oledataex *pole;
    unsigned int index;
    HRESULT hr;
    EXCEPINFO excepinfo;
    DISPID dispID = DISPID_VALUE;
    DISPID dispIDParam = DISPID_PROPERTYPUT;
    USHORT wFlags = DISPATCH_PROPERTYPUT;
    DISPPARAMS dispParams;
    VARIANTARG propertyValue[2];
    LCID    lcid = LOCALE_SYSTEM_DEFAULT;
    dispParams.rgdispidNamedArgs = &dispIDParam;
    dispParams.rgvarg = propertyValue;
    dispParams.cNamedArgs = 1;
    dispParams.cArgs = 1;

    VariantInit(&propertyValue[0]);
    VariantInit(&propertyValue[1]);
    memset(&excepinfo, 0, sizeof(excepinfo));

    Data_Get_Struct(self, struct oledataex, pole);

    /* get ID from property name */
	USES_CONVERSION;
	if (pole->pServiceProvider)
	{
		BSTR bstr = SysAllocString(A2W(StringValuePtr(property)));
		hr = pole->pDispatchEx->GetDispID(bstr, fdexNameCaseSensitive, &dispID);
                ATLTRACE(_T("win32ole propput call %ls, id=%x, result=%x\n"), bstr, dispID, hr);
		SysFreeString(bstr);
	}
	else
	{
		LPOLESTR pw = A2W(StringValuePtr(property));
		hr = pole->pDispatch->GetIDsOfNames(IID_NULL, &pw, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
	}
	if (hr != S_OK)
	{
		dispID = DISPID_VALUE;
        dispParams.cArgs = 2;
        V_VT(&propertyValue[1]) = VT_BSTR;
        V_BSTR(&propertyValue[1]) = SysAllocString(A2W(StringValuePtr(property)));
    }
    /* set property value */
	VARIANT vResult;
	VariantInit(&vResult);
    ole_val2variant(value, &propertyValue[0], pole->pEngine);
	if (pole->pServiceProvider)
	{
		hr = pole->pDispatchEx->InvokeEx(dispID, LOCALE_SYSTEM_DEFAULT,
			wFlags, &dispParams, &vResult, &excepinfo, pole->pServiceProvider);
		if (hr == S_FALSE)
		{
			hr = pole->pDispatchEx->InvokeEx(dispID, LOCALE_SYSTEM_DEFAULT,
				wFlags, &dispParams, &vResult, &excepinfo, NULL);
		}
	}
	else
	{
		UINT ul(0);
		hr = pole->pDispatch->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, 
			wFlags, &dispParams, &vResult, &excepinfo, &ul);
	}
	VariantClear(&vResult);
    for(index = 0; index < dispParams.cArgs; ++index)
        VariantClear(&propertyValue[index]);

    return Qnil;
}

/**
 * helper functions from Win32ole
 */
VALUE CRScriptCore::hash2named_arg(VALUE pair, oleparam* pOp)
{
	USES_CONVERSION;
    unsigned int index, i;
    VALUE key, value;
	const char* name;
    index = pOp->dp.cNamedArgs;

    /*-------------------------------------
      the data-type of key must be String
    ---------------------------------------*/
    key = rb_ary_entry(pair, 0);
	if (SYMBOL_P(key))
	{
		name = rb_id2name(SYM2ID(key));
	} 
	else if (TYPE(key) == T_STRING)
	{
		name = StringValuePtr(key);
	}
	else
	{
        /* clear name of dispatch parameters */
        for(i = 1; i < index + 1; i++) {
            SysFreeString(pOp->pNamedArgs[i]);
        }
        /* clear dispatch parameters */
        for(i = 0; i < index; i++ ) {
            VariantClear(&(pOp->dp.rgvarg[i]));
        }
        /* raise an exception */
        Check_Type(key, T_STRING);
    }

    /* pNamedArgs[0] is <method name>, so "index + 1" */
    pOp->pNamedArgs[index + 1] = SysAllocString(A2W(name));
    value = rb_ary_entry(pair, 1);
    VariantInit(&(pOp->dp.rgvarg[index]));
    ole_val2variant(value, &(pOp->dp.rgvarg[index]), pOp->pole->pEngine);

    pOp->dp.cNamedArgs += 1;
    return Qnil;
}

VALUE CRScriptCore::set_argv(VARIANTARG* realargs, unsigned int beg, unsigned int end, struct oledataex* pole, IServiceProvider* pProv)
{
	VALUE argv = rb_const_get(s_valueWin32Ole, rb_intern("ARGV"));

    Check_Type(argv, T_ARRAY);
    rb_ary_clear(argv);
    while (end-- > beg) {
		rb_ary_push(argv, ole_variant2val(&realargs[end], pole->pEngine, pProv));
        VariantClear(&realargs[end]);
    }
    return argv;
}

static HRESULT invoke(struct oledataex* pole, DISPID DispID, LCID lcid, WORD wFlags,
					  VARIANT* result, EXCEPINFO& excepinfo, unsigned int& argErr, oleparam& op)
{
	HRESULT hr;
	if (pole->pServiceProvider)
	{
		hr = pole->pDispatchEx->InvokeEx(DispID, lcid,
			wFlags, &op.dp, result, &excepinfo, pole->pServiceProvider);
		if (hr == S_FALSE)
		{
			hr = pole->pDispatchEx->InvokeEx(DispID, lcid,
				wFlags, &op.dp, result, &excepinfo, NULL);
		}
		else if (hr == S_OK)
		{
			op.pProv = pole->pServiceProvider;
		}
	}
	else
	{
		hr = pole->pDispatch->Invoke(DispID, IID_NULL, lcid, wFlags,
			&op.dp, result, &excepinfo, &argErr);
	}
	return hr;
}

VALUE CRScriptCore::ole_invoke(int argc, VALUE* argv, VALUE self, WORD wFlags)
{
	ATLTRACE(_T("ole_invoke in thread:%08X\n"), GetCurrentThreadId());
    LCID    lcid = LOCALE_SYSTEM_DEFAULT;
    struct oledataex *pole;
    HRESULT hr;
    VALUE cmd;
    VALUE paramS;
    VALUE param;
    VALUE obj;
    VALUE error_msg;

    DISPID DispID;
    DISPID* pDispID;
    EXCEPINFO excepinfo;
	VARIANTARG* prgvarg = NULL;
    VARIANT result;
    VARIANTARG* realargs = NULL;

    unsigned int argErr = 0;
	unsigned int cNamedArgs;
	struct oleparam op;

    memset(&excepinfo, 0, sizeof(EXCEPINFO));

    VariantInit(&result);

    op.dp.rgvarg = NULL;
    op.dp.rgdispidNamedArgs = NULL;
    op.dp.cNamedArgs = 0;
    op.dp.cArgs = 0;
	op.pProv = NULL;

    rb_scan_args(argc, argv, "1*", &cmd, &paramS);
    Data_Get_Struct(self, struct oledataex, pole);
	op.pole = pole;

    /*-----------------------------------------------------------
      get IDs from method name (or property name) and named args 
    -------------------------------------------------------------*/
	USES_CONVERSION;
	LPOLESTR pwMethodName = A2W(StringValuePtr(cmd));
	if (pole->pServiceProvider)
	{
		ATLTRACE(_T("Invoke DispatchEx %08\n"), pole->pDispatchEx);
		BSTR bstr = SysAllocString(pwMethodName);
		hr = pole->pDispatchEx->GetDispID(bstr, fdexNameCaseSensitive, &DispID);
                ATLTRACE(_T("win32ole invoke call %ls, id=%x, result=%x\n"), bstr, DispID, hr);
		SysFreeString(bstr);
	}
	else
	{
		hr = pole->pDispatch->GetIDsOfNames(IID_NULL, &pwMethodName, 1, lcid, &DispID);
	}
	if (hr != S_OK)
	{
        error_msg = rb_str_new2("Unknown property or method : ");
        rb_str_concat(error_msg, cmd);
        ole_raise(hr, rb_eNoMethodError, StringValuePtr(error_msg));
	}

    /* pick up last argument of method */
    param = rb_ary_entry(paramS, argc-2);

    /* if last arg is hash object */
    if(TYPE(param) == T_HASH) {
		/*------------------------------------------ 
          hash object ==> named dispatch parameters 
        --------------------------------------------*/
		cNamedArgs = NUM2INT(rb_funcall(param, rb_intern("length"), 0));
        op.dp.cArgs = cNamedArgs + argc - 2;
        op.pNamedArgs = ALLOCA_N(OLECHAR*, cNamedArgs + 1);
        op.dp.rgvarg = ALLOCA_N(VARIANTARG, op.dp.cArgs);
        rb_iterate(rb_each, param, (VALUE(*)(...))hash2named_arg, (VALUE)&op);

        pDispID = ALLOCA_N(DISPID, cNamedArgs + 1);
        op.pNamedArgs[0] = SysAllocString(A2W(StringValuePtr(cmd)));
		hr = pole->pDispatch->GetIDsOfNames(IID_NULL, 
                                                    op.pNamedArgs,
                                                    op.dp.cNamedArgs + 1,
                                                    lcid, pDispID);
        for(int i = 0; i < op.dp.cNamedArgs + 1; i++) {
            SysFreeString(op.pNamedArgs[i]);
            op.pNamedArgs[i] = NULL;
        }
        if(FAILED(hr)) {
            /* clear dispatch parameters */
            for(int i = 0; i < op.dp.cArgs; i++ ) {
                VariantClear(&op.dp.rgvarg[i]);
            }
            ole_raise(hr, s_valueWIN32OLERuntimeError, 
                      "failed to get named argument info: `%s'",
                      StringValuePtr(cmd));
        }
        op.dp.rgdispidNamedArgs = &(pDispID[1]);
    }
    else 
	{
        cNamedArgs = 0;
        op.dp.cArgs = argc - 1;
        op.pNamedArgs = ALLOCA_N(OLECHAR*, 1);
        if (op.dp.cArgs > 0) {
            op.dp.rgvarg  = ALLOCA_N(VARIANTARG, op.dp.cArgs);
        }
    }
    /*--------------------------------------
      non hash args ==> dispatch parameters 
     ----------------------------------------*/
    if(op.dp.cArgs > cNamedArgs) {
        realargs = ALLOCA_N(VARIANTARG, op.dp.cArgs-cNamedArgs+1);
        for(int i = cNamedArgs; i < op.dp.cArgs; i++) {
            int n = op.dp.cArgs - i + cNamedArgs - 1;
            VariantInit(&realargs[n]);
            VariantInit(&op.dp.rgvarg[n]);
            param = rb_ary_entry(paramS, i-cNamedArgs);
            
			ole_val2variant(param, &realargs[n], pole->pEngine);
            V_VT(&op.dp.rgvarg[n]) = VT_VARIANT | VT_BYREF;
			V_VARIANTREF(&op.dp.rgvarg[n]) = &realargs[n];

        }
    }
    /* apparent you need to call propput, you need this */
    if (wFlags & DISPATCH_PROPERTYPUT) {
        if (op.dp.cArgs == 0)
            return ResultFromScode(E_INVALIDARG);

        op.dp.cNamedArgs = 1;
        op.dp.rgdispidNamedArgs = ALLOCA_N( DISPID, 1 );
        op.dp.rgdispidNamedArgs[0] = DISPID_PROPERTYPUT;
    }

    hr = invoke(pole, DispID, lcid, wFlags, &result, excepinfo, argErr, op);
    if (FAILED(hr)) {
        /* retry to call args by value */
        if(op.dp.cArgs > cNamedArgs) {
            for(int i = cNamedArgs; i < op.dp.cArgs; i++) {
                int n = op.dp.cArgs - i + cNamedArgs - 1;
                param = rb_ary_entry(paramS, i-cNamedArgs);
                ole_val2variant(param, &op.dp.rgvarg[n], pole->pEngine);
            }
            memset(&excepinfo, 0, sizeof(EXCEPINFO));
            VariantInit(&result);
			hr = invoke(pole, DispID, lcid, wFlags, &result, excepinfo, argErr, op);

            /* mega kludge. if a method in WORD is called and we ask
             * for a result when one is not returned then
             * hResult == DISP_E_EXCEPTION. this only happens on
             * functions whose DISPID > 0x8000 */
			// If so, this is a bug of the callee, I don't like this kludge (arton)
            if ((hr == DISP_E_EXCEPTION || hr == DISP_E_MEMBERNOTFOUND) && DispID > 0x8000) {
                memset(&excepinfo, 0, sizeof(EXCEPINFO));
				hr = invoke(pole, DispID, lcid, wFlags, NULL, excepinfo, argErr, op);
            }
            for(int i = cNamedArgs; i < op.dp.cArgs; i++) {
                int n = op.dp.cArgs - i + cNamedArgs - 1;
                VariantClear(&op.dp.rgvarg[n]);
            }
        }
        if (FAILED(hr)) {
            /* retry after converting nil to VT_EMPTY */
            if (op.dp.cArgs > cNamedArgs) {
                for(int i = cNamedArgs; i < op.dp.cArgs; i++) {
                    int n = op.dp.cArgs - i + cNamedArgs - 1;
                    param = rb_ary_entry(paramS, i-cNamedArgs);
                    ole_val2variant(param, &op.dp.rgvarg[n], pole->pEngine, VT_EMPTY);
                }
                memset(&excepinfo, 0, sizeof(EXCEPINFO));
                VariantInit(&result);
				hr = invoke(pole, DispID, lcid, wFlags, &result, excepinfo, argErr, op);
                for(int i = cNamedArgs; i < op.dp.cArgs; i++) {
                    int n = op.dp.cArgs - i + cNamedArgs - 1;
                    VariantClear(&op.dp.rgvarg[n]);
                }
            }
        }
    }
    /* clear dispatch parameter */
    if(op.dp.cArgs > cNamedArgs) {
        set_argv(realargs, cNamedArgs, op.dp.cArgs, pole, op.pProv);
    }
    else {
        for(int i = 0; i < op.dp.cArgs; i++) {
            VariantClear(&op.dp.rgvarg[i]);
        }
    }

    if (FAILED(hr)) {
        VALUE v = ole_excepinfo2msg(&excepinfo);
        ole_raise(hr, s_valueWIN32OLERuntimeError, "%s%s",
                  StringValuePtr(cmd), StringValuePtr(v));
    }
    obj = ole_variant2val(&result, pole->pEngine, op.pProv);
    VariantClear(&result);
    return obj;
}

VALUE CRScriptCore::foleex_each(VALUE self)
{
    LCID    lcid = LOCALE_SYSTEM_DEFAULT;

    struct oledataex *pole;
	IServiceProvider* pProv = NULL;

    unsigned int argErr;
    EXCEPINFO excepinfo;
    DISPPARAMS dispParams;
    VARIANT result, variant;
    HRESULT hr;
    IEnumVARIANT *pEnum = NULL;

    VALUE obj;

    VariantInit(&result);
    dispParams.rgvarg = NULL;
    dispParams.rgdispidNamedArgs = NULL;
    dispParams.cNamedArgs = 0;
    dispParams.cArgs = 0;
    memset(&excepinfo, 0, sizeof(excepinfo));
    
    Data_Get_Struct(self, struct oledataex, pole);
    hr = pole->pDispatch->Invoke(DISPID_NEWENUM,
                                         IID_NULL, lcid,
                                         DISPATCH_METHOD | DISPATCH_PROPERTYGET,
                                         &dispParams, &result,
                                         &excepinfo, &argErr);

    if (FAILED(hr)) {
        ole_raise(hr, s_valueWIN32OLERuntimeError, "Fail to get IEnum Interface");
    }

    if (result.vt == VT_UNKNOWN || result.vt == VT_DISPATCH)
        hr = result.punkVal->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
    if (FAILED(hr) || !pEnum) {
        VariantClear(&result);
        ole_raise(hr, s_valueWIN32OLERuntimeError, "Fail to get IEnum Interface");
    }

    VariantInit(&variant);
    while(pEnum->Next(1, &variant, NULL) == S_OK) {
        obj = ole_variant2val(&variant, pole->pEngine, pole->pServiceProvider);
        rb_yield(obj);
        VariantClear(&variant);
        VariantInit(&variant);
    }
	pEnum->Release();
    return Qnil;
}

VALUE CRScriptCore::foleex_missing(int argc, VALUE* argv, VALUE self)
{
    ID id;
    const char* mname;
    int n;
    id = rb_to_id(argv[0]);
    mname = rb_id2name(id);
    if(!mname) {
        rb_raise(s_valueWIN32OLERuntimeError, "Fail : Unknown method or property");
    }
    n = strlen(mname);
    if(mname[n-1] == '=') {
        argv[0] = rb_str_new(mname, n-1);
        return fole_propertyput(self, argv[0], argv[1]);
    }
    else {
        argv[0] = rb_str_new2(mname);
        return ole_invoke(argc, argv, self, DISPATCH_METHOD|DISPATCH_PROPERTYGET);
    }
}

VALUE CRScriptCore::foleex_release(VALUE self)
{
    oledata *pole;
    Data_Get_Struct(self, struct oledata, pole);
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	CRubyWrapper::GetCWrapper()->ReleaseOle(pole);
	// never call Release, because not explicit addrefed
#else
	pole->pDispatch->Release();
#endif
    pole->pDispatch = NULL;
    return Qnil;
}
