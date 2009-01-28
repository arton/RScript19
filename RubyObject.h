/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-14 02:21:15 +0900 (火, 14 11 2006) $
 */

#ifndef RUBYOBJECT_HEADER
#define RUBYOBJECT_HEADER

class CRubyObject;
typedef std::map<std::string, ID> RubyMethodMap;
typedef std::map<std::string, ID>::iterator RubyMethodMapIter;
typedef std::map<std::string, CRubyObject*> RubyObjectMap;
typedef std::map<std::string, CRubyObject*>::iterator RubyObjectMapIter;
// COM -> Ruby wrapper dispatch
// This class has no TypeLibrary. So I can't use typelib marshaling.
// Instead it, IRubyEngine can handle GIT marshaling for the caller object,
// So, in this class, I simply put Free Thread Marshaler.
// 
// In GlobalRubyScript, there is no need to marshal interface (only local),
// so no Free Thread Marshaler is required.
class CRubyObject : public IDispatchEx
{
public:
	static CRubyObject* CreateRubyObject(IRubyEngine* pengine, VALUE val);

	CRubyObject(IRubyEngine* pengine, VALUE val, bool wrapperObject = true, IDispatch* pDisp = NULL);
	~CRubyObject();

	HRESULT  STDMETHODCALLTYPE QueryInterface(
		const IID & riid,  
		void **ppvObj);
	ULONG  STDMETHODCALLTYPE AddRef();
	ULONG  STDMETHODCALLTYPE Release();
	// IDispatch
    HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
		/* [out] */ UINT __RPC_FAR *pctinfo);
        
	HRESULT STDMETHODCALLTYPE GetTypeInfo( 
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
    HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
		/* [in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
	HRESULT STDMETHODCALLTYPE Invoke( 
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
		/* [out] */ VARIANT __RPC_FAR *pVarResult,
		/* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
		/* [out] */ UINT __RPC_FAR *puArgErr);

	HRESULT STDMETHODCALLTYPE GetDispID( 
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid)
	{
		HRESULT hr = GetIDsOfNames(IID_NULL, &bstrName, 1, LOCALE_SYSTEM_DEFAULT, pid);
		return hr;
	}
        
	HRESULT STDMETHODCALLTYPE InvokeEx( 
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller)
	{
		if (pspCaller)
			m_pEngine->RegisterServiceProvider(pspCaller);
		HRESULT hr = Invoke(id, IID_NULL, lcid, wFlags, pdp, pvarRes, pei, NULL);
		if (pspCaller)
			m_pEngine->UnregisterServiceProvider();
		return hr;
	}
        
	HRESULT STDMETHODCALLTYPE DeleteMemberByName( 
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex)
	{
		ATLTRACENOTIMPL(_T("DeleteMemberByName"));
	}

	HRESULT STDMETHODCALLTYPE DeleteMemberByDispID( 
            /* [in] */ DISPID id)
	{
		ATLTRACENOTIMPL(_T("DeleteMemberByDispID"));
	}
        
	HRESULT STDMETHODCALLTYPE GetMemberProperties( 
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex)
	{
		ATLTRACENOTIMPL(_T("GetMemberProperties"));
	}
        
	HRESULT STDMETHODCALLTYPE GetMemberName( 
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName)
	{
		ATLTRACENOTIMPL(_T("GetMemberName"));
	}
        
	HRESULT STDMETHODCALLTYPE GetNextDispID( 
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid)
	{
		ATLTRACENOTIMPL(_T("GetNextDispID"));
	}
        
	HRESULT STDMETHODCALLTYPE GetNameSpaceParent( 
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk)
	{
		ATLTRACENOTIMPL(_T("GetNameSpaceParent"));
	}

	inline void ClearEngine() { m_pEngine = NULL; }
	static DISPID ConvertDispID(VALUE, DISPID, WORD);
private:
	IRubyEngine* m_pEngine;
	CComPtr<IDispatch> m_pDisp;
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	IRubyEngine* m_pEngineVal;
	CComPtr<IUnknown> m_pUnkMarshaler;
#else
	void GetValue(VALUE, VARIANT*, IActiveScriptError*);
	void GetEnum(VALUE, VARIANT*, IActiveScriptError*);
	ID m_idMethodDefined;
	ID m_idMethods, m_idPrivMethods;
	ID m_idSize;
	static VALUE add_item(VALUE item, VALUE ary);
	static VALUE InvokeRuby(VALUE Param);
	static VALUE val2var(VALUE Param);
	static IActiveScriptError* FetchErrorInfo();
#endif
	long m_lCount;
	VALUE m_valueKlass;
	bool m_fRubyObject;
	RubyMethodMap m_mapMethods;
};

#endif
