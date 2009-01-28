// GetIDSOfNameTest.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"

static int assert_count = 0;
static int failed_count = 0;

void assert(int boolean, char* sz)
{
	assert_count++;
	if (!boolean)
	{
		failed_count++;
		puts(sz);
	}
}

void assert(LPCWSTR w1, LPCWSTR w2, char* sz)
{
	assert_count++;
	if (wcscmp(w1, w2))
	{
		failed_count++;
		puts(sz);
	}
}

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
		DWORD n;
		char sz[128];
		n = sprintf(sz, "err:%ls\n", b);
		SysFreeString(b);
		puts(sz);
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

static LPCWSTR script_text = 
L"class TestClass\n"
L"  def rstring\n"
L"    'Hello World !'\n"
L"  end\n"
L"  def ri\n"
L"    100\n"
L"  end\n"
L"end\n"
L"\n"
L"def gfunc()\n"
L"  'Hello Ruby !'\n"
L"end\n";

IActiveScript* CreateActiveScript(LPCWSTR engine)
{
	CLSID clsid;
	//  ScriptEngineのインスタンス化
	HRESULT hr = ::CLSIDFromProgID(engine, &clsid);
	IActiveScript* pAS;
	hr = ::CoCreateInstance( clsid, 0, CLSCTX_ALL, IID_IActiveScript, reinterpret_cast<void **>( &pAS ));
	//  初期化とMyScriptSiteの登録
	MyScriptSite* pASS = new MyScriptSite;
	hr = pAS->SetScriptSite(pASS);
	return pAS;
}

IActiveScriptParse* PrepareActiveScript(IActiveScript* pAS)
{
	IActiveScriptParse* pASP;
	HRESULT hr = pAS->QueryInterface(IID_IActiveScriptParse, ( void ** )&pASP);
	pASP->InitNew();
	hr = pAS->SetScriptState(SCRIPTSTATE_STARTED);  
	hr = pAS->SetScriptState(SCRIPTSTATE_CONNECTED);  
	return pASP;
}

void TestInvocation(IActiveScript* pAS, IActiveScriptParse* pASP)
{
	//  スクリプトテキストの解析と実行
	HRESULT hr = pASP->ParseScriptText(script_text, 0, 0, 0, 0, 0, 0, 0, 0);
	IDispatch* pDisp = NULL;
	hr = pAS->GetScriptDispatch(NULL, &pDisp);
	DISPID id = DISPID_UNKNOWN;
	LPOLESTR rgszName[] = { L"gfunc" };
	hr = pDisp->GetIDsOfNames(IID_NULL, rgszName, 1, LOCALE_SYSTEM_DEFAULT, &id);
	DISPPARAMS dispparams = { NULL, NULL, 0, 0 };
	VARIANT vResult;
	VariantInit(&vResult);
	hr = pDisp->Invoke(id, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &vResult, NULL, NULL);
	assert(L"Hello Ruby !", vResult.bstrVal, "gfunc");
}

void CleanUp(IActiveScript* pAS, IActiveScriptParse* pASP)
{
	pASP->Release();
	HRESULT hr = pAS->Close();
	pAS->Release();
}

void testGlobal()
{
	IActiveScript* pAS = CreateActiveScript(L"GlobalRubyScript");
	IActiveScriptParse* pASP = PrepareActiveScript(pAS);
	TestInvocation(pAS, pASP);
	CleanUp(pAS, pASP);
}

void testModule()
{
	IActiveScript* pAS = CreateActiveScript(L"RubyScript");
	IActiveScriptParse* pASP = PrepareActiveScript(pAS);
	TestInvocation(pAS, pASP);
	CleanUp(pAS, pASP);
}

int main(int argc, char* argv[])
{
	CoInitialize(NULL);

	testGlobal();
	testModule();

	CoUninitialize();

	char sz[256];
	sprintf(sz, "assert: %d, failed: %d\n", assert_count, failed_count);
	OutputDebugStringA(sz);

	return 0;
}

