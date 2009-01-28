// Copyright(c) 2000 arton
//
#include <windows.h>
#include <ole2.h>
#include <initguid.h>
#include <activscp.h>
#include <stdio.h>

class CMyObj : public IDispatch {
public:
  enum {
    DISPID_DO = 1,
  };
  CMyObj() : m_lCount(1)
    {
    }
  ~CMyObj()
    {
    }
  HRESULT  STDMETHODCALLTYPE QueryInterface(
    const IID & riid,  
    void **ppvObj)
    {
      if (!ppvObj) return E_POINTER;
      if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch))
	{
	  m_lCount++;
	  *ppvObj = this;
	  return S_OK;
	}
      return E_NOINTERFACE;
    }
  ULONG STDMETHODCALLTYPE AddRef()
    {
      return ++m_lCount;
    }
  ULONG STDMETHODCALLTYPE Release()
    {
      --m_lCount;
      if (m_lCount <= 0)
	{
	  delete this;
	  return 0;
	}
      return m_lCount;
    }
  HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
		/* [out] */ UINT __RPC_FAR *pctinfo)
    {
      return E_NOTIMPL;
    }
  HRESULT STDMETHODCALLTYPE GetTypeInfo( 
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
    {
      return E_NOTIMPL;
    }
  HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
		/* [in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
    {
      HRESULT hr = S_OK;
      for (UINT i = 0; i < cNames; i++)
	{
	  if (_wcsicmp(*(rgszNames + i), L"do") == 0
	      || _wcsicmp(*(rgszNames + i), L"domethod") == 0)
	    {
	      *(rgDispId + i) = DISPID_DO;
	    }
	  else
	    {
	      *(rgDispId + i) = DISPID_UNKNOWN;
	      hr = DISP_E_MEMBERNOTFOUND;
	    }
	}
      return hr;
    }
  HRESULT STDMETHODCALLTYPE Invoke( 
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
		/* [out] */ VARIANT __RPC_FAR *pVarResult,
		/* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
		/* [out] */ UINT __RPC_FAR *puArgErr)
    {
      HRESULT hr = S_OK;
      if (dispIdMember == DISPID_DO)
	{
	  VARIANTARG* pv = pDispParams->rgvarg;
	  if (pv->vt == (VT_VARIANT | VT_BYREF))
	    pv = pv->pvarVal;
	  printf("%d\n", pv->iVal);
	}
      else
	{
	  hr = DISP_E_MEMBERNOTFOUND;
	}
      return hr;
    }
  
private:
  long m_lCount;
};

class MyScriptSite : public IActiveScriptSite
{
public:
  MyScriptSite() : m_lCount(1)
    {
    }
  HRESULT  STDMETHODCALLTYPE QueryInterface(
    const IID & riid,  
    void **ppvObj)
    {
      if (!ppvObj) return E_POINTER;
      if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IActiveScriptSite))
	{
	  m_lCount++;
	  *ppvObj = this;
	  return S_OK;
	}
      return E_NOINTERFACE;
    }
  ULONG STDMETHODCALLTYPE AddRef()
    {
      return ++m_lCount;
    }
  ULONG STDMETHODCALLTYPE Release()
    {
      --m_lCount;
      if (m_lCount <= 0)
	{
	  delete this;
	  return 0;
	}
      return m_lCount;
    }
  HRESULT STDMETHODCALLTYPE GetLCID(LCID*)
    {
      return E_NOTIMPL;
    }
  HRESULT STDMETHODCALLTYPE GetItemInfo(
    LPCOLESTR pstrName,     // address of item name
    DWORD dwReturnMask,     // bit mask for information retrieval
    IUnknown **ppunkItem,   // address of pointer to item's IUnknown
    ITypeInfo **ppTypeInfo  // address of pointer to item's ITypeInfo
    )
    {
      if ((dwReturnMask & SCRIPTINFO_IUNKNOWN)&& _wcsicmp(pstrName, L"MyCustomObj") == 0)
	{
	  *ppunkItem = new CMyObj;
	  return S_OK;
	}
      return TYPE_E_ELEMENTNOTFOUND;
    }
  HRESULT STDMETHODCALLTYPE GetDocVersionString(
    BSTR *pbstrVersionString  // address of document version string
    )
    {
      *pbstrVersionString = SysAllocString(L"1.0");
      return S_OK;
    }
  HRESULT STDMETHODCALLTYPE OnScriptTerminate(
    const VARIANT *pvarResult,   // address of script results
    const EXCEPINFO *pexcepinfo  // address of structure with exception information
    )
    {
      return S_OK;
    }
  HRESULT STDMETHODCALLTYPE OnStateChange(
    SCRIPTSTATE ssScriptState  // new state of engine
    )
    {
      return S_OK;
    }
  HRESULT STDMETHODCALLTYPE OnScriptError(
    IActiveScriptError *pase  // address of error interface
    )
    {
      BSTR b;
      pase->GetSourceLineText(&b);
      printf("err:%ls\n", b);
      SysFreeString(b);
      return S_OK;
    }
  HRESULT STDMETHODCALLTYPE OnEnterScript(void)
    {
      return S_OK;
    }
  HRESULT STDMETHODCALLTYPE OnLeaveScript(void)
    {
      return S_OK;
    }
private:
  long m_lCount;
};

void CallScript(LPWSTR Engine, LPCWSTR ScriptText)
{
  CLSID clsid;
  //  Create Script Engine
  HRESULT hr = ::CLSIDFromProgID( Engine, &clsid );
  if (hr != S_OK)
  {
    printf("CLSIDFromProgID(%ls) = %08X\n", Engine, hr);
    return;
  }
  IActiveScript* pAS;
  hr = ::CoCreateInstance( clsid, 0, CLSCTX_ALL, IID_IActiveScript, reinterpret_cast<void **>( &pAS ));
  //  Initialize Script Engine
  MyScriptSite* pASS = new MyScriptSite;
  hr = pAS->SetScriptSite( pASS );
  //  Fetch Parser Interface
  IActiveScriptParse* pASP;
  hr = pAS->QueryInterface( IID_IActiveScriptParse, ( void ** )&pASP );
  pASP->InitNew();
  hr = pAS->AddNamedItem( L"MyCustomObj", SCRIPTITEM_GLOBALMEMBERS | SCRIPTITEM_ISVISIBLE  );
  hr = pAS->SetScriptState( SCRIPTSTATE_STARTED  );  
  hr = pAS->SetScriptState( SCRIPTSTATE_CONNECTED  );  
  //  Run Script Text
  hr = pASP->ParseScriptText( ScriptText, 0, 0, 0, 0, 0, 0, 0, 0 );
  pASP->Release();
  hr = pAS->Close();
  pAS->Release();

}

int main(int argc, char* argv[])
{
  CoInitialize(NULL);

  // Script Texts
  LPCWSTR myRubyScriptText( L"0.upto 5 do |i|; @MyCustomObj.do( i ); end;" );

  LPCWSTR myJavaScriptText( L"for (i = 0; i <= 5; i++) {"
			    L"  MyCustomObj.domethod(i);"
			    L"} "
			  );   // JavaScript can't take reserved word as method name.

  LPCWSTR myVBScriptText( L"For i = 0 To 5      \r\n"
			  L"  MyCustomObj.do i  \r\n"
			  L"Next" 
                        );	// VBScript needs CR/LF in each line.

  printf("Start RubyScript\n");
  CallScript(L"RubyScript", myRubyScriptText);

  printf("Start JavaScript\n");
  CallScript(L"JavaScript", myJavaScriptText);  

  printf("Start VBScript\n");
  CallScript(L"VBScript", myVBScriptText);
  
  CoUninitialize();
  return 0;
}
