/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-03 04:23:11 +0900 ( 03 11 2006) $
 */


#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "RScript.h"
#include "RScript_i.c"

#include "RubyScript.h"
#include "RubyWrapper.h"

#include "dlldatax.h"
#include "Rubyize.h"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif

DEFINE_GUID(SID_GetCaller, 0x4717cc40, 0xbcb9, 0x11d0, 0x93, 0x36, 0x0, 0xa0, 0xc9, 0xd, 0xca, 0xa9);

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Rubyize, CRubyize)
OBJECT_ENTRY(CLSID_RubyScript, CRubyScript)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    lpReserved;
#ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(hInstance, dwReason, lpReserved))
        return FALSE;
#endif
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_RSCRIPTLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////

STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllCanUnloadNow() != S_OK)
        return S_FALSE;
#endif
	if (_Module.GetLockCount()==0)
	{
		return S_OK;
	}
    return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////

STDAPI DllRegisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hRes = PrxDllRegisterServer();
    if (FAILED(hRes))
        return hRes;
#endif
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    return _Module.UnregisterServer(TRUE);
}


