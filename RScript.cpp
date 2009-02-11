/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-03 04:23:11 +0900 (é‡‘, 03 11 2006) $
 */

// RScript.cpp : DLL ƒGƒNƒXƒ|[ƒg‚ÌƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“


// ƒƒ‚: Proxy/Stub î•ñ
//  proxy/stub ƒR[ƒh‚ğƒIƒuƒWƒFƒNƒg DLL “à‚ÖŒ‹‡‚·‚é‚½‚ß‚É‚ÍAƒvƒƒWƒFƒNƒg‚Ö 
//  dlldatax.c ƒtƒ@ƒCƒ‹‚ğ’Ç‰Á‚µ‚Ä‚­‚¾‚³‚¢B ‚±‚Ìƒtƒ@ƒCƒ‹‚É‘Î‚µ‚ÄƒvƒŠƒRƒ“ƒpƒCƒ‹ 
//  Ï‚İƒwƒbƒ_[‚ª–³Œø‚Å‚ ‚é‚±‚Æ‚ğŠm”F‚µ‚ÄAƒvƒƒWƒFƒNƒg‚Ì’è‹`‚Ö _MERGE_PROXYSTUB 
//  ‚ğ’Ç‰Á‚µ‚Ä‚­‚¾‚³‚¢B  
//
//  DCOM ƒTƒ|[ƒg‚ğŠÜ‚Ş WinNT4.0 ‚Ü‚½‚Í Win95 ‚ªÀs’†‚Å‚Í‚È‚¢ê‡‚ÍA
//  ˆÈ‰º‚Ì’è‹`‚ğ dlldatax.c ‚©‚çíœ‚µ‚Ä‚­‚¾‚³‚¢B
//  #define _WIN32_WINNT 0x0400
//
//  ‚³‚ç‚ÉAMIDL ‚ğ /Oicf ƒXƒCƒbƒ`‚È‚µ‚ÅÀs‚µ‚Ä‚¢‚éê‡‚ÍA 
//  ˆÈ‰º‚Ì’è‹`‚ğ dlldatax.c ‚©‚çíœ‚µ‚Ä‚­‚¾‚³‚¢B
//  #define USE_STUBLESS_PROXY
//
//  ˆÈ‰º‚Ìƒtƒ@ƒCƒ‹‚ğƒAƒEƒgƒvƒbƒg‚É’Ç‰Á‚·‚é‚±‚Æ‚É‚æ‚Á‚Ä RScript.idl —p‚ÌƒJƒXƒ^ƒ€ 
//  ƒrƒ‹ƒh‚Ìƒ‹[ƒ‹‚ğC³‚µ‚Ä‚­‚¾‚³‚¢B 
//      RScript_p.c
//      dlldata.c
//  •ÊX‚Ì proxy/stub DLL ‚ğƒrƒ‹ƒh‚·‚é‚½‚ß‚É‚ÍAƒvƒƒWƒFƒNƒg‚ÌƒfƒBƒŒƒNƒgƒŠ‚Å 
//      nmake -f RScriptps.mak ‚ğÀs‚µ‚Ä‚­‚¾‚³‚¢B

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
// DLL ƒGƒ“ƒgƒŠ ƒ|ƒCƒ“ƒg

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
// DLL ‚ª OLE ‚É‚æ‚Á‚ÄƒAƒ“ƒ[ƒh‰Â”\‚©‚Ç‚¤‚©‚ğ’²‚×‚é‚½‚ß‚Ég—p‚³‚ê‚Ü‚·

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
// —v‹‚³‚ê‚½Œ^‚ÌƒIƒuƒWƒFƒNƒg‚ğì¬‚·‚é‚½‚ß‚ÉƒNƒ‰ƒX ƒtƒ@ƒNƒgƒŠ‚ğ•Ô‚µ‚Ü‚·

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - ƒVƒXƒeƒ€ ƒŒƒWƒXƒgƒŠ‚ÖƒGƒ“ƒgƒŠ‚ğ’Ç‰Á‚µ‚Ü‚·

STDAPI DllRegisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hRes = PrxDllRegisterServer();
    if (FAILED(hRes))
        return hRes;
#endif
    // ƒIƒuƒWƒFƒNƒgAƒ^ƒCƒvƒ‰ƒCƒuƒ‰ƒŠ‚¨‚æ‚Ñƒ^ƒCƒvƒ‰ƒCƒuƒ‰ƒŠ“à‚Ì‘S‚Ä‚ÌƒCƒ“ƒ^[ƒtƒFƒCƒX‚ğ“o˜^‚µ‚Ü‚·
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - ƒVƒXƒeƒ€ ƒŒƒWƒXƒgƒŠ‚©‚çƒGƒ“ƒgƒŠ‚ğíœ‚µ‚Ü‚·

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    return _Module.UnregisterServer(TRUE);
}


