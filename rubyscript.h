/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-15 12:49:36 +0900 (水, 15 11 2006) $
 */

#ifndef __RUBYSCRIPT_H_
#define __RUBYSCRIPT_H_

#include "resource.h"
#include "RScriptCore.h"

#include "giplip.h"

class CRubyScript;
class CRubyObject;
typedef std::map<std::wstring, CRubyObject*> RubyObjMap;
typedef std::map<std::wstring, CRubyObject*>::iterator RubyObjMapIter;
typedef std::map<std::string, ID> RubyMethodMap;
typedef std::map<std::string, ID>::iterator RubyMethodMapIter;
/////////////////////////////////////////////////////////////////////////////
// CRubyScript
class ATL_NO_VTABLE CRubyScript : 
	public CComCoClass<CRubyScript, &CLSID_RubyScript>,
	public IRubyEngine,
	public CRScriptCore
{
public:
	CRubyScript() :
	  m_valueModule(Qnil),
	  m_pWrapper(NULL),
	  m_dwSecurityLevel(0),
	  m_pModuleObject(NULL),
	  m_pRubyObject(NULL),
	  m_fRubyize(false)
	{
	}

	enum {
		MAX_RETRY = 10000,
	};

DECLARE_REGISTRY_RESOURCEID(IDR_RUBYSCRIPT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRubyScript)
	COM_INTERFACE_ENTRY(IRubyEngine)
	COM_INTERFACE_ENTRY_CHAIN(CRScriptCore)
END_COM_MAP()

	HRESULT FinalConstruct();

	void FinalRelease();

public:
	// IRubyEngine
	HRESULT STDMETHODCALLTYPE CreateObject( 
            /* [in] */ CLSID clsid,
            /* [string][in] */ LPCWSTR pszHost,
            /* [in] */ REFIID riid,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppv);

	HRESULT STDMETHODCALLTYPE CheckInterfaceIsSafety( 
            /* [in] */ CLSID clsid,
            /* [in] */ IUnknown __RPC_FAR *pUnk);

	HRESULT STDMETHODCALLTYPE CheckActiveXIsSafety(CLSID);

	HRESULT STDMETHODCALLTYPE EnterScript( void);
        
	HRESULT STDMETHODCALLTYPE LeaveScript( void);
        
	HRESULT STDMETHODCALLTYPE OnScriptError( 
            /* [in] */ IActiveScriptError __RPC_FAR *pError);

	HRESULT STDMETHODCALLTYPE RegisterServiceProvider( 
            /* [in] */ IServiceProvider __RPC_FAR *pProvider);
        
	HRESULT STDMETHODCALLTYPE UnregisterServiceProvider( void);

	HRESULT STDMETHODCALLTYPE GetSecurityLevel( 
            /* [retval][out] */ long __RPC_FAR *pResult);

	HRESULT STDMETHODCALLTYPE ClearModuleObject( void);

	HRESULT STDMETHODCALLTYPE SetRubyize( 
            /* [in] */ boolean f);
        
	HRESULT STDMETHODCALLTYPE GetRubyize( void);

	HRESULT STDMETHODCALLTYPE GetModule( 
            /* [retval][out] */ unsigned long __RPC_FAR *pResult);

	HRESULT STDMETHODCALLTYPE DisconnectObjects( void);

	// IActiveScript
	HRESULT STDMETHODCALLTYPE SetScriptSite( 
            /* [in] */ IActiveScriptSite __RPC_FAR *pass);

	HRESULT STDMETHODCALLTYPE SetScriptState( 
            /* [in] */ SCRIPTSTATE ss);

	HRESULT STDMETHODCALLTYPE Clone( 
		/* [out] */ IActiveScript __RPC_FAR *__RPC_FAR *ppscript);

    HRESULT STDMETHODCALLTYPE Close( void);

	HRESULT STDMETHODCALLTYPE InterruptScriptThread( 
            /* [in] */ SCRIPTTHREADID stidThread,
            /* [in] */ const EXCEPINFO __RPC_FAR *pexcepinfo,
            /* [in] */ DWORD dwFlags);

	HRESULT STDMETHODCALLTYPE DefineGlobalObject( void);
	//
 	virtual HRESULT Invoke(VALUE, int, int, LPCSTR, VARIANT*, EXCEPINFO FAR* pExcepInfo, DWORD dwFlags = 0);
	virtual void AddNamedItem(LPCOLESTR);
	virtual void Uninitialize();
	virtual IActiveScriptSite* GetSite();
	virtual HRESULT InternalGetScriptDispatch(LPCOLESTR pstrItemName, IDispatch** ppdisp);
	virtual VALUE GetModuleValue();
protected:
	inline void CopyPersistent(int n, std::string& s) { m_nStartLinePersistent = n; m_strScriptPersistent = s; }
	HRESULT ParseText(int, LPCSTR, LPCOLESTR, EXCEPINFO *, VARIANT*, DWORD);
	virtual void MakeScope();
	virtual IDispatch* GetOuterDispatch(IDispatch*);
	virtual IDispatch* GetGlobalDispatch();
	virtual void DefineConstant(LPOLESTR, VARIANT*);
	inline ID intern(char* p)
	{
		unsigned long id;
		m_pWrapper->rb_intern(reinterpret_cast<unsigned char*>(p), &id);
		return (ID)id;
	}
	inline setModule(VALUE v) { m_valueModule = v; }
	bool m_fRubyize;
private:
	GIP(IActiveScriptSite) m_pGippedSite;
	GIP(IInternetHostSecurityManager) m_pGippedScM;
	DWORD m_dwSecurityLevel;
	IRubyWrapper* m_pWrapper;
	RubyObjMap m_mapObj;
	VALUE m_valueModule;
	VALUE m_valueHash;
	RubyMethodMap m_mapMethods;
	CRubyObject* m_pModuleObject;
	CRubyObject* m_pRubyObject;
};

#endif //__RUBYSCRIPT_H_
