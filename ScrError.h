/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-04 17:29:08 +0900 (土, 04 11 2006) $
 */

#ifndef RSCRERROR_HEADER
#define RSCRERROR_HEADER

class CScrError : public IActiveScriptError
{
public:
	CScrError(VALUE info);
	CScrError(VALUE info, LPCSTR str, int offset = 0);
	CScrError(LPCSTR);
	
	HRESULT  STDMETHODCALLTYPE QueryInterface(
		const IID & riid,  
		void **ppvObj)
	{
		if (!ppvObj) return E_POINTER;
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IActiveScriptError))
		{
			InterlockedIncrement(&m_lRefCount);
			*ppvObj = this;
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	ULONG  STDMETHODCALLTYPE AddRef()
	{
		return InterlockedIncrement(&m_lRefCount);
	}
	ULONG  STDMETHODCALLTYPE Release()
	{
		if (InterlockedDecrement(&m_lRefCount) == 0)
		{
			delete this;
			return 0;
		}
		return m_lRefCount;
	}
 	HRESULT STDMETHODCALLTYPE GetExceptionInfo(
		EXCEPINFO *pexcepinfo  // structure for exception information
	);
	HRESULT STDMETHODCALLTYPE GetSourcePosition(
	    DWORD *pdwSourceContext,  // context cookie
		ULONG *pulLineNumber,     // line number of error
		LONG *pichCharPosition    // character position of error
	);
	HRESULT STDMETHODCALLTYPE GetSourceLineText(
		BSTR *pbstrSourceLine  // address of buffer for source line
	);

private:
	int GetErrorLine();
	void SetSource(LPCSTR);
	LONG m_lRefCount;
	int m_nLine;
	std::string m_strMessage;
	std::string m_strBacktrace;
	std::string m_strSource;
};

#endif
