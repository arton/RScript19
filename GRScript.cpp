// GRScript.cpp : DLL �G�N�X�|�[�g�̃C���v�������e�[�V����


// ����: Proxy/Stub ���
//  �ʁX�� proxy/stub DLL ���r���h���邽�߂ɂ́A�v���W�F�N�g�̃f�B���N�g���� 
//      nmake -f GRScriptps.mak �����s���Ă��������B

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "GRScript.h"
#include "GRScript_i.c"
#include "GlobalRubyScript.h"
#include "Rubyize.h"

DEFINE_GUID(SID_GetCaller, 0x4717cc40, 0xbcb9, 0x11d0, 0x93, 0x36, 0x0, 0xa0, 0xc9, 0xd, 0xca, 0xa9);

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_SimpleRubyize, CRubyize)
OBJECT_ENTRY(CLSID_GlobalRubyScript, CGlobalRubyScript)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL �G���g�� �|�C���g

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_GRSCRIPTLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// DLL �� OLE �ɂ���ăA�����[�h�\���ǂ����𒲂ׂ邽�߂Ɏg�p����܂�

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// �v�����ꂽ�^�̃I�u�W�F�N�g���쐬���邽�߂ɃN���X �t�@�N�g����Ԃ��܂�

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - �V�X�e�� ���W�X�g���փG���g����ǉ����܂�

STDAPI DllRegisterServer(void)
{
    // �I�u�W�F�N�g�A�^�C�v���C�u��������у^�C�v���C�u�������̑S�ẴC���^�[�t�F�C�X��o�^���܂�
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - �V�X�e�� ���W�X�g������G���g�����폜���܂�

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


