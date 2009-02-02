/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-04 17:29:08 +0900 (土, 04 11 2006) $
 */

#include "stdafx.h"
#ifdef _MERGE_PROXYSTUB
#include "RScript.h"
#else
#include "GRScript.h"
#endif
#include "RScriptCore.h"

#include "ScrError.h"
///////////////////////////////////////////////////////////////////////
CScrError::CScrError(VALUE v, LPCSTR src, int offset) :
	m_lRefCount(1), m_nLine(0)
{
	CreateErrorBase(v);        
	if (m_nLine > 0)
	{
		SetSource(src);
	}
	m_nLine += offset;
}

CScrError::CScrError(VALUE v) : 
	m_lRefCount(1), m_nLine(0)
{
	CreateErrorBase(v);        
}

CScrError::CScrError(LPCSTR psz) :
	m_lRefCount(1),
	m_strMessage(psz), m_nLine(0)
{
}

void CScrError::CreateErrorBase(VALUE v)
{
	VALUE msg = rb_funcall(v, rb_intern("message"), 0);
	m_strMessage = StringValuePtr(msg);
	ATLTRACE(_T("Create Error Object For %hs\n"), m_strMessage.c_str());
	VALUE bt = rb_funcall(v, rb_intern("backtrace"), 0);
        if (!NIL_P(bt))
        {
#if defined(_DEBUG)
            for (int i = 0; i < RARRAY_LEN(bt); i++)
            {
                VALUE e = rb_ary_entry(bt, i);
                ATLTRACE("%hs\n", StringValuePtr(e));
            }
#endif
            bt = rb_ary_entry(bt, 0);
            m_strBacktrace = StringValuePtr(bt);
            ATLTRACE("bt=%d, %hs\n", FIX2INT(rb_funcall(bt, rb_intern("size"), 0)), m_strBacktrace.c_str());
        }
        else
        {
            m_strBacktrace = "";
        }
	m_nLine = GetErrorLine();
}

int CScrError::GetErrorLine()
{
	if (m_strBacktrace.length() > 0)
	{
		LPCSTR p = m_strBacktrace.c_str();
		if (lstrlenA(p) > 2 && *(p + 1) == ':') p += 2;
		do
		{
			p = strchr(p, ':');
		}
		while (p && (*(p + 1) == '\\' || *(p + 1) == '/'));	// void drive letter
		if (p) 
		{
			return atoi(p + 1);
		}
	}
	return 0;
}

void CScrError::SetSource(LPCSTR p)
{
	LPSTR p2;
	for (int i = 0; i < m_nLine; i++)
	{
		p2 = strchr(p, '\n');
		if (!p2 || *(p2 + 1) == '\0')
			break;
		p = p2 + 1;
	}

	int len;
	p2 = strchr(p, '\n');
	if (!p2) 
	{
		len = strlen(p);
	}
	else if (*(p2 - 1) == '\r')
	{
		len = p2 - p - 1;
	}
	else
	{
		len = p2 - p;
	}
	m_strSource.append(p, len);
}

HRESULT STDMETHODCALLTYPE CScrError::GetExceptionInfo(
		EXCEPINFO *pExcepInfo  // structure for exception information
	)
{
	if (!pExcepInfo) return E_POINTER;
	USES_CONVERSION;
	memset(pExcepInfo, 0, sizeof(EXCEPINFO));
	pExcepInfo->wCode = 0x0200;// + istat;
	pExcepInfo->bstrSource = SysAllocString(L"RScript");
	pExcepInfo->bstrDescription = SysAllocString(A2W(m_strMessage.c_str()));
	pExcepInfo->scode = DISP_E_EXCEPTION;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CScrError::GetSourcePosition(
	    DWORD *pdwSourceContext,  // context cookie
		ULONG *pulLineNumber,     // line number of error
		LONG *pichCharPosition    // character position of error
	)
{
	if (pdwSourceContext)
	{
		*pdwSourceContext = 0;
	}
	if (pulLineNumber)
	{
		*pulLineNumber = m_nLine;
	}
	if (pichCharPosition)
	{
		*pichCharPosition = 0;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CScrError::GetSourceLineText(
		BSTR *pbstrSourceLine  // address of buffer for source line
	)
{
	if (!pbstrSourceLine) return E_POINTER;
	*pbstrSourceLine = NULL;
	USES_CONVERSION;
	if (m_strSource.length() > 0)
	{
		*pbstrSourceLine = SysAllocString(A2W(m_strSource.c_str()));
	}
	return S_OK;
}
