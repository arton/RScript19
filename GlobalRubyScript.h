/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-15 12:49:36 +0900 (水, 15 11 2006) $
 */
	
#ifndef __GLOBALRUBYSCRIPT_H_
#define __GLOBALRUBYSCRIPT_H_

#include "resource.h"
#include "RScriptCore.h"
/////////////////////////////////////////////////////////////////////////////
// CGlobalRubyScript

class CRubyObject;
class ATL_NO_VTABLE CGlobalRubyScript : 
	public CComCoClass<CGlobalRubyScript, &CLSID_GlobalRubyScript>,
	public IGlobalRubyScript,
	public CRScriptCore
{
public:
	CGlobalRubyScript() :
	  m_pRubyObject(NULL), m_fArgInitialized(false)
	{
		ATLTRACE(_("GlobalRuby \n"));
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GLOBALRUBYSCRIPT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGlobalRubyScript)
	COM_INTERFACE_ENTRY(IGlobalRubyScript)
	COM_INTERFACE_ENTRY_CHAIN(CRScriptCore)
END_COM_MAP()

	HRESULT FinalConstruct();

	void FinalRelease();

	// IActiveScript
	HRESULT STDMETHODCALLTYPE SetScriptState( 
            /* [in] */ SCRIPTSTATE ss);

// IGlobalRubyScript
public:
	HRESULT STDMETHODCALLTYPE InterruptScriptThread( 
            /* [in] */ SCRIPTTHREADID stidThread,
            /* [in] */ const EXCEPINFO __RPC_FAR *pexcepinfo,
            /* [in] */ DWORD dwFlags);

	virtual void AddNamedItem(LPCOLESTR);
	HRESULT STDMETHODCALLTYPE ClearModuleObject( void);
protected:
	HRESULT ParseText(int, LPCSTR, LPCOLESTR, EXCEPINFO *, VARIANT*, DWORD);
	HRESULT EvalExpression(int, LPCSTR, EXCEPINFO *, VARIANT*);
	void RaiseError(int, LPCSTR);
	void SetExceptionInfo(EXCEPINFO* pExcepInfo);
	virtual void MakeScope();
	virtual IDispatch* GetOuterDispatch(IDispatch*);
	virtual IDispatch* GetGlobalDispatch();
	virtual void DefineConstant(LPOLESTR, VARIANT*);
private:
	HANDLE m_hThread;
	void SetupArgs(LPCOLESTR);
	void FillArgs(IUnknown*);
	ID m_idMethodDefined;
	CRubyObject* m_pRubyObject;
	void ExpandArg(BSTR, std::list<std::string>&);
	bool m_fArgInitialized;
};

#endif //__GLOBALRUBYSCRIPT_H_
