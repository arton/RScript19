/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-03 04:23:11 +0900 (金, 03 11 2006) $
 */

// RScript.cpp : DLL �G�N�X�|�[�g�̃C���v�������e�[�V����


// ����: Proxy/Stub ���
//  proxy/stub �R�[�h���I�u�W�F�N�g DLL ���֌������邽�߂ɂ́A�v���W�F�N�g�� 
//  dlldatax.c �t�@�C����ǉ����Ă��������B ���̃t�@�C���ɑ΂��ăv���R���p�C�� 
//  �ς݃w�b�_�[�������ł��邱�Ƃ��m�F���āA�v���W�F�N�g�̒�`�� _MERGE_PROXYSTUB 
//  ��ǉ����Ă��������B  
//
//  DCOM �T�|�[�g���܂� WinNT4.0 �܂��� Win95 �����s���ł͂Ȃ��ꍇ�́A
//  �ȉ��̒�`�� dlldatax.c ����폜���Ă��������B
//  #define _WIN32_WINNT 0x0400
//
//  ����ɁAMIDL �� /Oicf �X�C�b�`�Ȃ��Ŏ��s���Ă���ꍇ�́A 
//  �ȉ��̒�`�� dlldatax.c ����폜���Ă��������B
//  #define USE_STUBLESS_PROXY
//
//  �ȉ��̃t�@�C�����A�E�g�v�b�g�ɒǉ����邱�Ƃɂ���� RScript.idl �p�̃J�X�^�� 
//  �r���h�̃��[�����C�����Ă��������B 
//      RScript_p.c
//      dlldata.c
//  �ʁX�� proxy/stub DLL ���r���h���邽�߂ɂ́A�v���W�F�N�g�̃f�B���N�g���� 
//      nmake -f RScriptps.mak �����s���Ă��������B

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
// DLL �G���g�� �|�C���g

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
// DLL �� OLE �ɂ���ăA�����[�h�\���ǂ����𒲂ׂ邽�߂Ɏg�p����܂�

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
// �v�����ꂽ�^�̃I�u�W�F�N�g���쐬���邽�߂ɃN���X �t�@�N�g����Ԃ��܂�

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - �V�X�e�� ���W�X�g���փG���g����ǉ����܂�

STDAPI DllRegisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hRes = PrxDllRegisterServer();
    if (FAILED(hRes))
        return hRes;
#endif
    // �I�u�W�F�N�g�A�^�C�v���C�u��������у^�C�v���C�u�������̑S�ẴC���^�[�t�F�C�X��o�^���܂�
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - �V�X�e�� ���W�X�g������G���g�����폜���܂�

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    return _Module.UnregisterServer(TRUE);
}


