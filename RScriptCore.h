/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-12-02 15:21:54 +0900 (Sat, 02 12 2006) $
 */

#ifndef CRScriptCORE_H
#define CRScriptCORE_H

class CRScriptCore;
class CEventSink;
class CItemHolder;
class CInitializer;
struct oledata;
struct oleparam;
class CRubyProc;

#ifndef __IRubyEngine_INTERFACE_DEFINED__
#define IRubyEngine CRScriptCore
#endif

typedef std::map<std::wstring, CEventSink*> EventMap;
typedef std::map<std::wstring, CEventSink*>::iterator EventMapIter;
typedef std::list<CRScriptCore*> EngineList;
typedef std::list<CRScriptCore*>::iterator EngineListIter;
typedef std::map<std::wstring, CItemHolder*> ItemMap;
typedef std::map<std::wstring, CItemHolder*>::iterator ItemMapIter;
typedef std::list<oledata*> OleObjList;
typedef std::list<oledata*>::iterator OleObjListIter;
typedef std::list<IServiceProvider*> SPList;
typedef std::list<ITypeLib*> TypeLibList;
typedef std::list<ITypeLib*>::iterator TypeLibListIter;

class CRScriptCore;

class CScriptlet
{
public:
	CScriptlet(LPCOLESTR code, LPCOLESTR item, LPCOLESTR subitem, LPCOLESTR event, ULONG startline);
	HRESULT Add(CRScriptCore*);
private:
	std::wstring m_code;
	std::wstring m_item;
	std::wstring m_subitem;
	std::wstring m_event;
	ULONG m_startline;
};

typedef std::list<CScriptlet> ScriptletList;
typedef std::list<CScriptlet>::iterator ScriptletListIter;

class CRScriptCore :
	public IObjectSafetyImpl<CRScriptCore, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public IActiveScript,
	public IActiveScriptParse,
	public IServiceProvider,
	public IActiveScriptParseProcedure,
	public CComObjectRoot
{
	friend class CInitializer;
public:
	CRScriptCore();

DECLARE_GET_CONTROLLING_UNKNOWN()

BEGIN_COM_MAP(CRScriptCore)
	COM_INTERFACE_ENTRY(IActiveScriptParse)
	COM_INTERFACE_ENTRY(IActiveScript)
	COM_INTERFACE_ENTRY(IActiveScriptParseProcedure)
	COM_INTERFACE_ENTRY(IServiceProvider)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	HRESULT FinalConstruct();

	void FinalRelease();

	CComPtr<IUnknown> m_pUnkMarshaler;

	// IObjectSafety
		HRESULT STDMETHODCALLTYPE SetInterfaceSafetyOptions( REFIID riid, DWORD dwOptionsSetMask, DWORD dwEnabledOptions );
		HRESULT STDMETHODCALLTYPE GetInterfaceSafetyOptions( REFIID riid, DWORD* pdwSupportedOptions, DWORD* pdwEnabledOptions );

	// IActiveScript
        HRESULT STDMETHODCALLTYPE SetScriptSite( 
            /* [in] */ IActiveScriptSite __RPC_FAR *pass);
        
        HRESULT STDMETHODCALLTYPE GetScriptSite( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        HRESULT STDMETHODCALLTYPE SetScriptState( 
            /* [in] */ SCRIPTSTATE ss);
        
        HRESULT STDMETHODCALLTYPE GetScriptState( 
            /* [out] */ SCRIPTSTATE __RPC_FAR *pssState);
        
        HRESULT STDMETHODCALLTYPE Close( void);
        
        HRESULT STDMETHODCALLTYPE AddNamedItem( 
            /* [in] */ LPCOLESTR pstrName,
            /* [in] */ DWORD dwFlags);
        
        HRESULT STDMETHODCALLTYPE AddTypeLib( 
            /* [in] */ REFGUID rguidTypeLib,
            /* [in] */ DWORD dwMajor,
            /* [in] */ DWORD dwMinor,
            /* [in] */ DWORD dwFlags);
        
        HRESULT STDMETHODCALLTYPE GetScriptDispatch( 
            /* [in] */ LPCOLESTR pstrItemName,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp);
        
        HRESULT STDMETHODCALLTYPE GetCurrentScriptThreadID( 
            /* [out] */ SCRIPTTHREADID __RPC_FAR *pstidThread);
        
        HRESULT STDMETHODCALLTYPE GetScriptThreadID( 
            /* [in] */ DWORD dwWin32ThreadId,
            /* [out] */ SCRIPTTHREADID __RPC_FAR *pstidThread);
        
        HRESULT STDMETHODCALLTYPE GetScriptThreadState( 
            /* [in] */ SCRIPTTHREADID stidThread,
            /* [out] */ SCRIPTTHREADSTATE __RPC_FAR *pstsState);
        
        HRESULT STDMETHODCALLTYPE InterruptScriptThread( 
            /* [in] */ SCRIPTTHREADID stidThread,
            /* [in] */ const EXCEPINFO __RPC_FAR *pexcepinfo,
            /* [in] */ DWORD dwFlags) = 0;
        
        HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IActiveScript __RPC_FAR *__RPC_FAR *ppscript);

	// IActiveScriptParse
		HRESULT STDMETHODCALLTYPE InitNew( void);
        
        HRESULT STDMETHODCALLTYPE AddScriptlet( 
            /* [in] */ LPCOLESTR pstrDefaultName,
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ LPCOLESTR pstrItemName,
            /* [in] */ LPCOLESTR pstrSubItemName,
            /* [in] */ LPCOLESTR pstrEventName,
            /* [in] */ LPCOLESTR pstrDelimiter,
#if defined(_M_X64)
			/* [in] */ DWORDLONG dwSourceContextCookie,
#else
            /* [in] */ DWORD dwSourceContextCookie,
#endif
            /* [in] */ ULONG ulStartingLineNumber,
            /* [in] */ DWORD dwFlags,
            /* [out] */ BSTR __RPC_FAR *pbstrName,
            /* [out] */ EXCEPINFO __RPC_FAR *pexcepinfo);
        
        HRESULT STDMETHODCALLTYPE ParseScriptText( 
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ LPCOLESTR pstrItemName,
            /* [in] */ IUnknown __RPC_FAR *punkContext,
            /* [in] */ LPCOLESTR pstrDelimiter,
#if defined(_M_X64)
			/* [in] */ DWORDLONG dwSourceContextCookie,
#else
			/* [in] */ DWORD dwSourceContextCookie,
#endif
            /* [in] */ ULONG ulStartingLineNumber,
            /* [in] */ DWORD dwFlags,
            /* [out] */ VARIANT __RPC_FAR *pvarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pexcepinfo);

    // IActiveScriptParseProcedure
	HRESULT STDMETHODCALLTYPE ParseProcedureText( 
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ LPCOLESTR pstrFormalParams,
            /* [in] */ LPCOLESTR pstrProcedureName,
            /* [in] */ LPCOLESTR pstrItemName,
            /* [in] */ IUnknown __RPC_FAR *punkContext,
            /* [in] */ LPCOLESTR pstrDelimiter,
#if defined(_M_X64)
			/* [in] */ DWORDLONG dwSourceContextCookie,
#else
			/* [in] */ DWORD dwSourceContextCookie,
#endif
            /* [in] */ ULONG ulStartingLineNumber,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp);

	// IServiceProvider
	HRESULT STDMETHODCALLTYPE QueryService(
			REFGUID guidService,
			REFIID riid,
			void **ppv);

	// ActiveScriptRuby private
	inline void EnterScript()
	{
#ifndef __IRubyEngine_INTERFACE_DEFINED__
		s_pRScriptCore = this;
#endif
		m_threadState = SCRIPTTHREADSTATE_RUNNING;
		m_pSite->OnEnterScript();
	}

	inline void LeaveScript()
	{
		m_threadState = SCRIPTTHREADSTATE_NOTINSCRIPT;
		m_pSite->OnLeaveScript();
	}

	static CRScriptCore* GetEngine(long);
	virtual HRESULT STDMETHODCALLTYPE ClearModuleObject( void) = 0;
#ifndef __IRubyEngine_INTERFACE_DEFINED__
	static CRScriptCore* GetCurrentEngine();
#endif
	VALUE GetOleObject(VALUE, char*);
	VALUE GetOleObject(VALUE, LPCOLESTR);
	inline DWORD GetThreadID() const { return m_dwThreadID; }
	inline SCRIPTTHREADSTATE GetThreadState() const { return m_threadState; }
	virtual void AddNamedItem(LPCOLESTR) = 0;
	void BindNamedItem();
	void UnbindNamedItem();
	virtual void Uninitialize();
	HRESULT OnScriptError(IActiveScriptError* p);

	virtual HRESULT Invoke(VALUE, int, int, LPCSTR, VARIANT*, EXCEPINFO FAR* pExcepInfo, DWORD dwFlags = 0);
	virtual HRESULT InternalGetScriptDispatch(LPCOLESTR pstrItemName, IDispatch** ppdisp);

	static VALUE ole_ary_m_entry(VALUE val, long* pid);
	static void ole_set_safe_array(long n, SAFEARRAY* psa, long* pid, long* pub, VALUE val, long dim, IRubyEngine* pv);
	static void ole_val2variant(VALUE val, VARIANT* var, IRubyEngine* pv, VARTYPE nil_to = VT_ERROR);
	static VALUE ole_variant2val(VARIANT* pvar, IRubyEngine* pv, IServiceProvider* pp = NULL);
	static VALUE fole_propertyput(VALUE self, VALUE property, VALUE value);
	static VALUE ole_invoke(int argc, VALUE* argv, VALUE self, WORD wFlags);
	static VALUE foleex_missing(int argc, VALUE* argv, VALUE self);
	static VALUE foleex_each(VALUE self);
	static void ole_raise(HRESULT hr, VALUE ecs, LPCSTR p_msg, ...);
	static VALUE ole_excepinfo2msg(EXCEPINFO* pExInfo);
	static VALUE ole_hresult2msg(HRESULT h);
	static VALUE hash2named_arg(VALUE pair, oleparam* pOp);
	static VALUE set_argv(VARIANTARG* realargs, unsigned int beg, unsigned int end, struct oledataex* pole, IServiceProvider* pProv);
	static VALUE __cdecl fole_s_new(int arcgc, VALUE* argv, VALUE self);
	static VALUE __cdecl fole_s_connect(VALUE self, VALUE svr_name);
	static VALUE __cdecl foleex_attach(VALUE self, VALUE ItemName, VALUE Cookie);
	static void __cdecl ole_free(oledata* pole);
	static VALUE __cdecl foleex_release(VALUE self);
	static VALUE ole_createWin32OleEx(IUnknown*, IRubyEngine* pv, IServiceProvider* pp = NULL);
	static VALUE s_valueWin32Ole;
	static VALUE s_valueWin32OleEx;
	static VALUE s_valueWIN32OLERuntimeError;
	static VALUE s_valueActiveRubyScript;
	static bool s_fTrace;
	static bool s_fRubyize;

	static void TraceOff();
	static void TraceOn();
	static VALUE __cdecl trace_hook(VALUE, VALUE, VALUE, VALUE, VALUE, VALUE, VALUE);
	static VALUE __cdecl trace_set(VALUE, VALUE);
	static VALUE __cdecl rubyize(VALUE, VALUE);
	static void ForceChangeThreadContext();
		inline static int GetExceptionLine() { return s_lastline; }
	virtual IActiveScriptSite* GetSite();
#ifndef __IRubyEngine_INTERFACE_DEFINED__
	inline void RegisterServiceProvider(IServiceProvider* psp) { PushServiceProvider(psp); }
	inline void UnregisterServiceProvider() { PopServiceProvider(); }
	inline LPBYTE GetStackTop() const { return s_pStackTop; }
#endif
	void PushServiceProvider(IServiceProvider* psp);
	void PopServiceProvider();

	//
	static void InitializeEnvironment();
	virtual VALUE GetModuleValue();
protected:
	inline long GetCookie() const { return m_lCookie; }
	HRESULT Connect();
	void ConnectEvents();
	void Disconnect(bool fSinkOnly = false);
	virtual HRESULT ParseText(int, LPCSTR, LPCOLESTR, EXCEPINFO *, VARIANT*, DWORD) = 0;
	virtual IDispatch* GetOuterDispatch(IDispatch*) = 0;
	virtual IDispatch* GetGlobalDispatch() = 0;
	virtual void MakeScope() = 0;
	virtual void DefineGlobalProperties(LPCSTR);
	virtual void DefineGlobalMethods(LPCSTR);
	virtual void DefineConstant(LPOLESTR, VARIANT*) = 0;
	static VALUE __cdecl GlobalGetter(ID id);
	static void __cdecl GlobalSetter(VALUE val, ID id, VALUE* var);
	// ruby wrapping methods
	static VALUE __cdecl funcall(VALUE v[]);
#if defined(__IRubyEngine_INTERFACE_DEFINED__)
	static VALUE eval_string(char*, int);
#endif
	void InterruptThread(HANDLE);
	void CopyNamedItem(ItemMap&);
	long m_lCookie;
	int m_nStartLinePersistent;
	std::string m_strScriptPersistent;
	std::wstring m_strGlobalObjectName;
	IActiveScriptSite* m_pSite;
	ItemMap m_mapItem;
	EventMap m_mapEvent;
	SCRIPTSTATE m_state;
	SCRIPTTHREADSTATE m_threadState;
	DWORD m_dwThreadID;
	DWORD m_dwSafety;
	DWORD m_dwBaseSecurityLevel;
	IInternetHostSecurityManager* m_pScM;
	IServiceProvider* m_pProv;
	OleObjList m_listOleObj;
	TypeLibList m_listTypeLib;
	SPList m_listServiceProvider;
	ScriptletList m_listScriptlets;
	static HRESULT LoadTypeLib(REFGUID, DWORD, DWORD, ITypeLib**);
	static int s_lastline;
	static bool s_fRaiseException;
	static CComAutoCriticalSection s_crit;
	static EngineList s_listEngine;
	static long s_lCookie;
#ifndef __IRubyEngine_INTERFACE_DEFINED__
	static CRScriptCore* s_pRScriptCore;
	static LPBYTE s_pStackTop;
#else
	static ID s_idInstanceEval;
	static ID s_idModuleEval;
#endif
	static rb_encoding* s_pEncoding;
};

#endif
