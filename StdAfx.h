/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 */

#if !defined(AFX_STDAFX_H__5DBEF56C_E278_11D3_8E7A_0000F45A3C05__INCLUDED_)
#define AFX_STDAFX_H__5DBEF56C_E278_11D3_8E7A_0000F45A3C05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
//#define _ATL_APARTMENT_THREADED
#define _ATL_DEBUG_QI
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

#include <dispex.h>
#include <string>
#include <stdarg.h>
#include <map>
#include <list>

#include "ruby.h"
extern "C" {
#include "ruby/encoding.h"
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ 

#endif // !defined(AFX_STDAFX_H__5DBEF56C_E278_11D3_8E7A_0000F45A3C05__INCLUDED)
