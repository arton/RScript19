// dlldata.c �p�̃��b�p

#ifdef _MERGE_PROXYSTUB // proxy stub DLL �̌���

#define REGISTER_PROXY_DLL //DllRegisterServer�A��

#define _WIN32_WINNT 0x0500	//WinNT 4.0 �܂��� DCOM ���T�|�[�g���� Win95 �p
#define USE_STUBLESS_PROXY	//MIDL �̃I�v�V������ /Oicf ���w�肵���ꍇ�̂ݒ�`

#pragma comment(lib, "rpcns4.lib")
#pragma comment(lib, "rpcrt4.lib")

#define DllMain				PrxDllMain
#define DllRegisterServer	PrxDllRegisterServer
#define DllUnregisterServer PrxDllUnregisterServer
#define DllGetClassObject   PrxDllGetClassObject
#define DllCanUnloadNow     PrxDllCanUnloadNow

#include "dlldata.c"
#include "RScript_p.c"

#ifdef _NOPROXY //midl ���쐬���� dlldata.c �͂���܂���

#define STRICT 1
#include <ole2.h>

BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{return TRUE;}

STDAPI PrxDllCanUnloadNow(void){return S_OK;}

STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{return CLASS_E_CLASSNOTAVAILABLE;}

STDAPI PrxDllRegisterServer(void){return S_OK;}

STDAPI PrxDllUnregisterServer(void){return S_OK;}

#endif //!PROXY_DELEGATION

#endif //_MERGE_PROXYSTUB
