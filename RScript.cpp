/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-03 04:23:11 +0900 (é, 03 11 2006) $
 */

// RScript.cpp : DLL GNX|[gÌCve[V


// : Proxy/Stub îñ
//  proxy/stub R[hðIuWFNg DLL àÖ·é½ßÉÍAvWFNgÖ 
//  dlldatax.c t@CðÇÁµÄ­¾³¢B ±Ìt@CÉÎµÄvRpC 
//  ÏÝwb_[ª³øÅ é±ÆðmFµÄAvWFNgÌè`Ö _MERGE_PROXYSTUB 
//  ðÇÁµÄ­¾³¢B  
//
//  DCOM T|[gðÜÞ WinNT4.0 Ü½Í Win95 ªÀsÅÍÈ¢êÍA
//  ÈºÌè`ð dlldatax.c ©çíµÄ­¾³¢B
//  #define _WIN32_WINNT 0x0400
//
//  ³çÉAMIDL ð /Oicf XCb`ÈµÅÀsµÄ¢éêÍA 
//  ÈºÌè`ð dlldatax.c ©çíµÄ­¾³¢B
//  #define USE_STUBLESS_PROXY
//
//  ÈºÌt@CðAEgvbgÉÇÁ·é±ÆÉæÁÄ RScript.idl pÌJX^ 
//  rhÌ[ðC³µÄ­¾³¢B 
//      RScript_p.c
//      dlldata.c
//  ÊXÌ proxy/stub DLL ðrh·é½ßÉÍAvWFNgÌfBNgÅ 
//      nmake -f RScriptps.mak ðÀsµÄ­¾³¢B

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
OBJECT_ENTRY(CLSID_RubyScript, CRubyScript)
OBJECT_ENTRY(CLSID_Rubyize, CRubyize)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Gg |Cg

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
// DLL ª OLE ÉæÁÄA[hÂ\©Ç¤©ð²×é½ßÉgp³êÜ·

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
// v³ê½^ÌIuWFNgðì¬·é½ßÉNX t@NgðÔµÜ·

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - VXe WXgÖGgðÇÁµÜ·

STDAPI DllRegisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hRes = PrxDllRegisterServer();
    if (FAILED(hRes))
        return hRes;
#endif
    // IuWFNgA^CvCu¨æÑ^CvCuàÌSÄÌC^[tFCXðo^µÜ·
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - VXe WXg©çGgðíµÜ·

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    return _Module.UnregisterServer(TRUE);
}


