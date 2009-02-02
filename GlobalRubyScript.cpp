/*
 *  Copyright(c) 2000, 2009 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *  $Date: 2006-11-15 12:49:36 +0900 (水, 15 11 2006) $
 */

#include "stdafx.h"
#include "GRScript.h"
#include "GlobalRubyScript.h"
#include "ItemDisp.h"
#include "win32ole.h"
#include "RubyObject.h"
#include "ScrError.h"

/////////////////////////////////////////////////////////////////////////////
// CGlobalRubyScript
// This Object is tightly bound to Windows Script Host
//
HRESULT CGlobalRubyScript::FinalConstruct()
{
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(),
		&m_hThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
	HRESULT h = CRScriptCore::FinalConstruct();
	//
	m_idMethodDefined = rb_intern("method_defined?");

	return h;
}

void CGlobalRubyScript::FinalRelease()
{
	if (m_pRubyObject)
	{
		m_pRubyObject->ClearEngine();
	}
	CloseHandle(m_hThread);
	CRScriptCore::FinalRelease();
	ruby_finalize();
}

void CGlobalRubyScript::MakeScope()
{

}

HRESULT STDMETHODCALLTYPE CGlobalRubyScript::ClearModuleObject( void)
{
	m_pRubyObject = NULL;
	return S_OK;
}

IDispatch* CGlobalRubyScript::GetOuterDispatch(IDispatch*)
{
	if (!m_pRubyObject)
	{
		m_pRubyObject = new CRubyObject(this, rb_mKernel, true);
	}
	return m_pRubyObject;
}

IDispatch* CGlobalRubyScript::GetGlobalDispatch()
{
	return GetOuterDispatch(NULL);
}

HRESULT STDMETHODCALLTYPE CGlobalRubyScript::InterruptScriptThread( 
            /* [in] */ SCRIPTTHREADID stidThread,
            /* [in] */ const EXCEPINFO __RPC_FAR *pexcepinfo,
            /* [in] */ DWORD dwFlags)
{
	if (stidThread == SCRIPTTHREADID_ALL || stidThread == SCRIPTTHREADID_BASE || stidThread == m_dwThreadID)
	{
		SuspendThread(m_hThread);
		if (m_threadState != SCRIPTTHREADSTATE_NOTINSCRIPT)
		{
			InterruptThread(m_hThread);
		}
		ResumeThread(m_hThread);
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CGlobalRubyScript::SetScriptState( 
            /* [in] */ SCRIPTSTATE ss)
{
	HRESULT hr = CRScriptCore::SetScriptState(ss);
	if (hr == S_OK)
	{
		switch (ss)
		{
		case SCRIPTSTATE_STARTED:
		case SCRIPTSTATE_CONNECTED:
			if (m_strScriptPersistent.length() > 0)
			{
				ParseText(m_nStartLinePersistent, m_strScriptPersistent.c_str(), 
					L"", NULL, NULL, 0);
			}
			break;
		}
	}
	return hr;
}

void CGlobalRubyScript::DefineConstant(LPOLESTR name, VARIANT* pvar)
{
	USES_CONVERSION;
	LPSTR pname = W2A(name);
	*pname = toupper(*pname);
	if (rb_is_const_id(rb_intern(pname)))
	{
		rb_define_const(GetModuleValue(), pname, ole_variant2val(pvar, this));
	}
}

void CGlobalRubyScript::AddNamedItem(LPCOLESTR pstrName)
{
	int stacktop;
	if (reinterpret_cast<LPBYTE>(&stacktop) > s_pStackTop)
		_alloca(reinterpret_cast<LPBYTE>(&stacktop) - s_pStackTop);

	USES_CONVERSION;
	LPSTR pName = W2A(pstrName);
	int cb = lstrlenA(pName);
	LPSTR pCapitalizedName = pName;
	if (islower(*pName))
	{
		pCapitalizedName = lstrcpyA(reinterpret_cast<char*>(_alloca(cb + 1)), pName);
		*pCapitalizedName = toupper(*pName);
	}
	char* pstr = reinterpret_cast<char*>(_alloca(cb * 5 + 96));
	if (isupper(*pCapitalizedName))
		sprintf(pstr, "$%s = WIN32OLE.attach(\"%s\", %d)\r\n%s = $%s unless Object.const_defined?('%s')\r\n", pName, pName, m_lCookie, pCapitalizedName, pName, pCapitalizedName);
	else
		sprintf(pstr, "$%s = WIN32OLE.attach(\"%s\", %d)\r\n", pName, pName, m_lCookie);
	rb_eval_string(pstr);
	sprintf(pstr, "$_asr_default_object = $%s", pName);
	rb_eval_string(pstr);
	//
	if (lstrcmpiW(pstrName, L"WScript") == 0)
	{
		SetupArgs(pstrName);
	}
	if (m_strGlobalObjectName == pstrName)
	{
		DefineGlobalProperties(pName);
		DefineGlobalMethods(pName);
	}
}

HRESULT CGlobalRubyScript::ParseText(int StartLine, LPCSTR pstrCode, LPCOLESTR pstrItemName, EXCEPINFO *pExcepInfo, VARIANT* pVarResult, DWORD dwFlags)
{
	int stacktop;
	if (reinterpret_cast<LPBYTE>(&stacktop) > s_pStackTop)
		_alloca(reinterpret_cast<LPBYTE>(&stacktop) - s_pStackTop);

	if ((dwFlags & SCRIPTTEXT_ISEXPRESSION) && pVarResult != NULL)
	{
		return EvalExpression(StartLine, pstrCode, pExcepInfo, pVarResult);
	}

	HRESULT hr = S_OK;
	TCHAR szScriptFile[_MAX_PATH], szTempPath[_MAX_PATH + 4];
	GetTempPath(_MAX_PATH, szTempPath);
	GetTempFileName(szTempPath, _T("RSC"), 0, szScriptFile);
	lstrcat(szScriptFile, _T(".rb"));
	EnterScript();
	TraceOn();
	try
	{
		DWORD dw;
                USES_CONVERSION;
		HANDLE h = CreateFile(szScriptFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(h, pstrCode, strlen(pstrCode), &dw, NULL);
		CloseHandle(h);
		LPSTR ps = T2A(szScriptFile);
		for (LPSTR p = ps; *p; p++)
		{
			if (IsDBCSLeadByte(*p)) p++;
			else if (*p == '\\')
				*p = '/';
		}
		int state(0);
		rb_load_protect(rb_str_new2(ps), 0, &state);
		if (state)
		{
			RaiseError(StartLine, pstrCode);
		}
	}
	catch (...)
	{
		SetExceptionInfo(pExcepInfo);
		hr = DISP_E_EXCEPTION;
	}
	TraceOff();
	LeaveScript();
	DeleteFile(szScriptFile);

	return hr;
}
void CGlobalRubyScript::RaiseError(int StartLine, LPCSTR pstrCode)
{
	VALUE errinfo = rb_errinfo();
	if (!NIL_P(errinfo) && rb_obj_is_kind_of(errinfo, rb_eSystemExit) == Qfalse)
	{
		CScrError* pError = new CScrError(errinfo, pstrCode, StartLine - 1);
		OnScriptError(pError);
	}
}

void CGlobalRubyScript::SetExceptionInfo(EXCEPINFO* pExcepInfo)
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

HRESULT CGlobalRubyScript::EvalExpression(int StartLine, LPCSTR pstrCode, EXCEPINFO *pExcepInfo, VARIANT* pVarResult)
{
	HRESULT hr = S_OK;

	EnterScript();
	TraceOn();

	try
	{
		int state(0);
		VALUE result = rb_eval_string_protect(pstrCode, &state);
		if (state)
		{
			RaiseError(StartLine, pstrCode);
		}
		else
		{
			ole_val2variant(result, pVarResult, this);
		}
	}
	catch (...)
	{
		SetExceptionInfo(pExcepInfo);
		hr = DISP_E_EXCEPTION;
	}
	TraceOff();
	LeaveScript();

	return hr;
}

void CGlobalRubyScript::SetupArgs(LPCOLESTR pstrName)
{
	if (m_fArgInitialized) return;

	ItemMapIter it = m_mapItem.find(pstrName);
	if (it == m_mapItem.end()) return;

	IDispatch* pDisp = (*it).second->GetDispatch();
	if (!pDisp) return;

	static const LPCOLESTR szArguments[] = { L"Arguments", L"ScriptFullName" };
	DISPID dispId;
	DISPPARAMS param = { NULL, NULL, 0, 0, };
	VARIANT vResult;
	VariantInit(&vResult);
	// Windows Script Host never understand after the second name.
	HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, const_cast<LPOLESTR*>(&szArguments[1]), 1, LOCALE_SYSTEM_DEFAULT, &dispId);
	if (hr == S_OK)
	{
		hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET | DISPATCH_METHOD, &param, &vResult, NULL, NULL);
		if (hr == S_OK)
		{
			USES_CONVERSION;
			ruby_script(W2A(vResult.bstrVal));
			VariantClear(&vResult);
		}
	}
	hr = pDisp->GetIDsOfNames(IID_NULL, const_cast<LPOLESTR*>(&szArguments[0]), 1, LOCALE_SYSTEM_DEFAULT, &dispId);
	if (hr == S_OK)
	{
		hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET | DISPATCH_METHOD, &param, &vResult, NULL, NULL);
		if (hr == S_OK)
		{
			if (vResult.vt == VT_DISPATCH)
			{
				VARIANT vUnk;
				VariantInit(&vUnk);
				hr = vResult.pdispVal->Invoke(DISPID_NEWENUM, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET | DISPATCH_METHOD, &param, &vUnk, NULL, NULL);
				if (hr == S_OK)
				{
					if (vUnk.vt == VT_UNKNOWN)
					{
						FillArgs(vUnk.punkVal);
					}
					VariantClear(&vUnk);
				}
			}
			VariantClear(&vResult);
		}
	}
	pDisp->Release();
}

void CGlobalRubyScript::FillArgs(IUnknown* pUnk)
{
	CComQIPtr<IEnumVARIANT, &IID_IEnumVARIANT> pEnum(pUnk);
	if (!pEnum) return;

	std::list<std::string> listArgs;
	ULONG Fetched; 
	VARIANT vResult;
	VariantInit(&vResult);
	for (; pEnum->Next(1, &vResult, &Fetched) == S_OK; VariantClear(&vResult))
	{
		ExpandArg(vResult.bstrVal, listArgs);
	}
	if (listArgs.size() > 0)
	{
		char** argv = reinterpret_cast<char**>(_alloca(listArgs.size() * sizeof(char*)));
		char** pt = argv;
		for (std::list<std::string>::iterator it = listArgs.begin(); it != listArgs.end(); it++)
		{
			*pt++ = const_cast<LPSTR>((*it).c_str());
		}
		ruby_set_argv(listArgs.size(), argv);
		m_fArgInitialized = true;
	}
}

void CGlobalRubyScript::ExpandArg(BSTR str, std::list<std::string>& list)
{
	USES_CONVERSION;
        char* p = W2A(str);
	if (str && (wcschr(str, L'?') || wcschr(str, L'*')))
	{
		WIN32_FIND_DATAA fdata;
		HANDLE h = FindFirstFileA(p, &fdata);
		if (h != INVALID_HANDLE_VALUE)
		{
			int l = -1;
			for (char* pn = p; *pn; pn = CharNextA(pn))
			{
				if (*pn == '\\')
				{
					l = pn - p;
				}
			}
			l++;
			char* buff = reinterpret_cast<char*>(_alloca(l * sizeof(char) + sizeof(fdata.cFileName) + 4));
			do
			{
				if (l > 0)
					strncpy(buff, p, l);
				strcpy(buff + l, fdata.cFileName);
				list.push_back(buff);
			}
			while (FindNextFileA(h, &fdata));
			FindClose(h);
			return;
		}
	}
	list.push_back(p);
}
