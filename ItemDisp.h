/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-03 04:23:11 +0900 (金, 03 11 2006) $
 */

#ifndef ITEMHOLDER_HEADER
#define ITEMHOLDER_HEADER

#ifdef __IRubyWrapper_INTERFACE_DEFINED__
#include "giplip.h"
#endif

class CItemHolder
{
public:
	CItemHolder(DWORD dwFlag = 0) : m_dwFlag(dwFlag), m_pDisp(NULL), m_pDispEx(NULL)
	{
	}
	~CItemHolder();
	inline DWORD GetFlag() const { return m_dwFlag; }
	inline bool IsSource() const { return (m_dwFlag | SCRIPTITEM_ISSOURCE) ? true : false; }
	IDispatch* GetDispatch();
	inline bool IsOK()
	{
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
		return (m_pGIPDisp.IsOK() || m_pGIPDispEx.IsOK() || m_pDispEx || m_pDisp);
#else
		return (m_pDispEx || m_pDisp);
#endif
	}
	IDispatch* GetDispatch(IActiveScriptSite* pSite, LPOLESTR pstrName, bool fSameApt);
	void Empty();
private:
	DWORD m_dwFlag;
	IDispatch* m_pDisp;
	IDispatchEx* m_pDispEx;
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	GIP(IDispatch) m_pGIPDisp;
	GIP(IDispatchEx) m_pGIPDispEx;
#endif
};


#endif
