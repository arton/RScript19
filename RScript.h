/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Feb 14 23:49:36 2009
 */
/* Compiler settings for C:\home\arton\projects\RScript19\trunk\RScript.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __RScript_h__
#define __RScript_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IRubyEngine_FWD_DEFINED__
#define __IRubyEngine_FWD_DEFINED__
typedef interface IRubyEngine IRubyEngine;
#endif 	/* __IRubyEngine_FWD_DEFINED__ */


#ifndef __IRubyWrapper_FWD_DEFINED__
#define __IRubyWrapper_FWD_DEFINED__
typedef interface IRubyWrapper IRubyWrapper;
#endif 	/* __IRubyWrapper_FWD_DEFINED__ */


#ifndef __IRubyScript_FWD_DEFINED__
#define __IRubyScript_FWD_DEFINED__
typedef interface IRubyScript IRubyScript;
#endif 	/* __IRubyScript_FWD_DEFINED__ */


#ifndef __IRubyize_FWD_DEFINED__
#define __IRubyize_FWD_DEFINED__
typedef interface IRubyize IRubyize;
#endif 	/* __IRubyize_FWD_DEFINED__ */


#ifndef __RubyScript_FWD_DEFINED__
#define __RubyScript_FWD_DEFINED__

#ifdef __cplusplus
typedef class RubyScript RubyScript;
#else
typedef struct RubyScript RubyScript;
#endif /* __cplusplus */

#endif 	/* __RubyScript_FWD_DEFINED__ */


#ifndef __RubyWrapper_FWD_DEFINED__
#define __RubyWrapper_FWD_DEFINED__

#ifdef __cplusplus
typedef class RubyWrapper RubyWrapper;
#else
typedef struct RubyWrapper RubyWrapper;
#endif /* __cplusplus */

#endif 	/* __RubyWrapper_FWD_DEFINED__ */


#ifndef __Rubyize_FWD_DEFINED__
#define __Rubyize_FWD_DEFINED__

#ifdef __cplusplus
typedef class Rubyize Rubyize;
#else
typedef struct Rubyize Rubyize;
#endif /* __cplusplus */

#endif 	/* __Rubyize_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "activscp.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IRubyEngine_INTERFACE_DEFINED__
#define __IRubyEngine_INTERFACE_DEFINED__

/* interface IRubyEngine */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IRubyEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1AEE0BE2-04AC-11d4-8EA8-0000F45A3C05")
    IRubyEngine : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateObject( 
            /* [in] */ CLSID clsid,
            /* [string][in] */ LPCWSTR pszHost,
            /* [in] */ REFIID riid,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckInterfaceIsSafety( 
            /* [in] */ CLSID clsid,
            /* [in] */ IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckActiveXIsSafety( 
            /* [in] */ CLSID clsid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnterScript( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LeaveScript( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnScriptError( 
            /* [in] */ IActiveScriptError __RPC_FAR *pError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterServiceProvider( 
            /* [in] */ IServiceProvider __RPC_FAR *pProvider) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterServiceProvider( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSecurityLevel( 
            /* [retval][out] */ long __RPC_FAR *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearModuleObject( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRubyize( 
            /* [in] */ boolean f) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRubyize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetModule( 
            /* [retval][out] */ unsigned long __RPC_FAR *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DisconnectObjects( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DefineGlobalObject( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRubyEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRubyEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRubyEngine __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRubyEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateObject )( 
            IRubyEngine __RPC_FAR * This,
            /* [in] */ CLSID clsid,
            /* [string][in] */ LPCWSTR pszHost,
            /* [in] */ REFIID riid,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppv);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckInterfaceIsSafety )( 
            IRubyEngine __RPC_FAR * This,
            /* [in] */ CLSID clsid,
            /* [in] */ IUnknown __RPC_FAR *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckActiveXIsSafety )( 
            IRubyEngine __RPC_FAR * This,
            /* [in] */ CLSID clsid);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnterScript )( 
            IRubyEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LeaveScript )( 
            IRubyEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnScriptError )( 
            IRubyEngine __RPC_FAR * This,
            /* [in] */ IActiveScriptError __RPC_FAR *pError);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterServiceProvider )( 
            IRubyEngine __RPC_FAR * This,
            /* [in] */ IServiceProvider __RPC_FAR *pProvider);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterServiceProvider )( 
            IRubyEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSecurityLevel )( 
            IRubyEngine __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pResult);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearModuleObject )( 
            IRubyEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRubyize )( 
            IRubyEngine __RPC_FAR * This,
            /* [in] */ boolean f);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRubyize )( 
            IRubyEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetModule )( 
            IRubyEngine __RPC_FAR * This,
            /* [retval][out] */ unsigned long __RPC_FAR *pResult);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisconnectObjects )( 
            IRubyEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DefineGlobalObject )( 
            IRubyEngine __RPC_FAR * This);
        
        END_INTERFACE
    } IRubyEngineVtbl;

    interface IRubyEngine
    {
        CONST_VTBL struct IRubyEngineVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRubyEngine_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRubyEngine_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRubyEngine_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRubyEngine_CreateObject(This,clsid,pszHost,riid,ppv)	\
    (This)->lpVtbl -> CreateObject(This,clsid,pszHost,riid,ppv)

#define IRubyEngine_CheckInterfaceIsSafety(This,clsid,pUnk)	\
    (This)->lpVtbl -> CheckInterfaceIsSafety(This,clsid,pUnk)

#define IRubyEngine_CheckActiveXIsSafety(This,clsid)	\
    (This)->lpVtbl -> CheckActiveXIsSafety(This,clsid)

#define IRubyEngine_EnterScript(This)	\
    (This)->lpVtbl -> EnterScript(This)

#define IRubyEngine_LeaveScript(This)	\
    (This)->lpVtbl -> LeaveScript(This)

#define IRubyEngine_OnScriptError(This,pError)	\
    (This)->lpVtbl -> OnScriptError(This,pError)

#define IRubyEngine_RegisterServiceProvider(This,pProvider)	\
    (This)->lpVtbl -> RegisterServiceProvider(This,pProvider)

#define IRubyEngine_UnregisterServiceProvider(This)	\
    (This)->lpVtbl -> UnregisterServiceProvider(This)

#define IRubyEngine_GetSecurityLevel(This,pResult)	\
    (This)->lpVtbl -> GetSecurityLevel(This,pResult)

#define IRubyEngine_ClearModuleObject(This)	\
    (This)->lpVtbl -> ClearModuleObject(This)

#define IRubyEngine_SetRubyize(This,f)	\
    (This)->lpVtbl -> SetRubyize(This,f)

#define IRubyEngine_GetRubyize(This)	\
    (This)->lpVtbl -> GetRubyize(This)

#define IRubyEngine_GetModule(This,pResult)	\
    (This)->lpVtbl -> GetModule(This,pResult)

#define IRubyEngine_DisconnectObjects(This)	\
    (This)->lpVtbl -> DisconnectObjects(This)

#define IRubyEngine_DefineGlobalObject(This)	\
    (This)->lpVtbl -> DefineGlobalObject(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IRubyEngine_CreateObject_Proxy( 
    IRubyEngine __RPC_FAR * This,
    /* [in] */ CLSID clsid,
    /* [string][in] */ LPCWSTR pszHost,
    /* [in] */ REFIID riid,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppv);


void __RPC_STUB IRubyEngine_CreateObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_CheckInterfaceIsSafety_Proxy( 
    IRubyEngine __RPC_FAR * This,
    /* [in] */ CLSID clsid,
    /* [in] */ IUnknown __RPC_FAR *pUnk);


void __RPC_STUB IRubyEngine_CheckInterfaceIsSafety_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_CheckActiveXIsSafety_Proxy( 
    IRubyEngine __RPC_FAR * This,
    /* [in] */ CLSID clsid);


void __RPC_STUB IRubyEngine_CheckActiveXIsSafety_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_EnterScript_Proxy( 
    IRubyEngine __RPC_FAR * This);


void __RPC_STUB IRubyEngine_EnterScript_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_LeaveScript_Proxy( 
    IRubyEngine __RPC_FAR * This);


void __RPC_STUB IRubyEngine_LeaveScript_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_OnScriptError_Proxy( 
    IRubyEngine __RPC_FAR * This,
    /* [in] */ IActiveScriptError __RPC_FAR *pError);


void __RPC_STUB IRubyEngine_OnScriptError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_RegisterServiceProvider_Proxy( 
    IRubyEngine __RPC_FAR * This,
    /* [in] */ IServiceProvider __RPC_FAR *pProvider);


void __RPC_STUB IRubyEngine_RegisterServiceProvider_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_UnregisterServiceProvider_Proxy( 
    IRubyEngine __RPC_FAR * This);


void __RPC_STUB IRubyEngine_UnregisterServiceProvider_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_GetSecurityLevel_Proxy( 
    IRubyEngine __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pResult);


void __RPC_STUB IRubyEngine_GetSecurityLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_ClearModuleObject_Proxy( 
    IRubyEngine __RPC_FAR * This);


void __RPC_STUB IRubyEngine_ClearModuleObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_SetRubyize_Proxy( 
    IRubyEngine __RPC_FAR * This,
    /* [in] */ boolean f);


void __RPC_STUB IRubyEngine_SetRubyize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_GetRubyize_Proxy( 
    IRubyEngine __RPC_FAR * This);


void __RPC_STUB IRubyEngine_GetRubyize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_GetModule_Proxy( 
    IRubyEngine __RPC_FAR * This,
    /* [retval][out] */ unsigned long __RPC_FAR *pResult);


void __RPC_STUB IRubyEngine_GetModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_DisconnectObjects_Proxy( 
    IRubyEngine __RPC_FAR * This);


void __RPC_STUB IRubyEngine_DisconnectObjects_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyEngine_DefineGlobalObject_Proxy( 
    IRubyEngine __RPC_FAR * This);


void __RPC_STUB IRubyEngine_DefineGlobalObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRubyEngine_INTERFACE_DEFINED__ */


#ifndef __IRubyWrapper_INTERFACE_DEFINED__
#define __IRubyWrapper_INTERFACE_DEFINED__

/* interface IRubyWrapper */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IRubyWrapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("66D56EE1-0249-11D4-8EA4-0000F45A3C05")
    IRubyWrapper : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE rb_intern( 
            /* [string][in] */ unsigned char __RPC_FAR *name,
            /* [out] */ unsigned long __RPC_FAR *pid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE rb_module_new( 
            /* [in] */ unsigned long parent,
            /* [out] */ unsigned long __RPC_FAR *p) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SearchMethod( 
            /* [in] */ unsigned long val,
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [out] */ DISPID __RPC_FAR *pDispID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE rb_invoke( 
            /* [in] */ IRubyEngine __RPC_FAR *pengine,
            /* [in] */ unsigned long Module,
            /* [in] */ unsigned long func,
            /* [in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [unique][out][in] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ IActiveScriptError __RPC_FAR *__RPC_FAR *ppError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SearchClass( 
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [out] */ unsigned long __RPC_FAR *pval) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE rb_funcall_with_string2( 
            /* [in] */ IRubyEngine __RPC_FAR *pengine,
            /* [in] */ unsigned long val,
            /* [in] */ unsigned long id,
            /* [in] */ long line,
            /* [in] */ long cb,
            /* [size_is][in] */ unsigned char __RPC_FAR *p,
            /* [in] */ boolean TraceRequired,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ IActiveScriptError __RPC_FAR *__RPC_FAR *ppError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterObject( 
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterObject( 
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Disconnect( 
            /* [in] */ IRubyEngine __RPC_FAR *pengin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DefineGlobalObject( 
            /* [in] */ IRubyEngine __RPC_FAR *pengine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DefineConstant( 
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module,
            /* [in] */ VARIANT __RPC_FAR *pVar,
            /* [in] */ IRubyEngine __RPC_FAR *pengine) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRubyWrapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRubyWrapper __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRubyWrapper __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRubyWrapper __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *rb_intern )( 
            IRubyWrapper __RPC_FAR * This,
            /* [string][in] */ unsigned char __RPC_FAR *name,
            /* [out] */ unsigned long __RPC_FAR *pid);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *rb_module_new )( 
            IRubyWrapper __RPC_FAR * This,
            /* [in] */ unsigned long parent,
            /* [out] */ unsigned long __RPC_FAR *p);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SearchMethod )( 
            IRubyWrapper __RPC_FAR * This,
            /* [in] */ unsigned long val,
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [out] */ DISPID __RPC_FAR *pDispID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *rb_invoke )( 
            IRubyWrapper __RPC_FAR * This,
            /* [in] */ IRubyEngine __RPC_FAR *pengine,
            /* [in] */ unsigned long Module,
            /* [in] */ unsigned long func,
            /* [in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [unique][out][in] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ IActiveScriptError __RPC_FAR *__RPC_FAR *ppError);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SearchClass )( 
            IRubyWrapper __RPC_FAR * This,
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [out] */ unsigned long __RPC_FAR *pval);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *rb_funcall_with_string2 )( 
            IRubyWrapper __RPC_FAR * This,
            /* [in] */ IRubyEngine __RPC_FAR *pengine,
            /* [in] */ unsigned long val,
            /* [in] */ unsigned long id,
            /* [in] */ long line,
            /* [in] */ long cb,
            /* [size_is][in] */ unsigned char __RPC_FAR *p,
            /* [in] */ boolean TraceRequired,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ IActiveScriptError __RPC_FAR *__RPC_FAR *ppError);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterObject )( 
            IRubyWrapper __RPC_FAR * This,
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterObject )( 
            IRubyWrapper __RPC_FAR * This,
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IRubyWrapper __RPC_FAR * This,
            /* [in] */ IRubyEngine __RPC_FAR *pengin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DefineGlobalObject )( 
            IRubyWrapper __RPC_FAR * This,
            /* [in] */ IRubyEngine __RPC_FAR *pengine);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DefineConstant )( 
            IRubyWrapper __RPC_FAR * This,
            /* [string][in] */ unsigned char __RPC_FAR *pName,
            /* [in] */ unsigned long Module,
            /* [in] */ VARIANT __RPC_FAR *pVar,
            /* [in] */ IRubyEngine __RPC_FAR *pengine);
        
        END_INTERFACE
    } IRubyWrapperVtbl;

    interface IRubyWrapper
    {
        CONST_VTBL struct IRubyWrapperVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRubyWrapper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRubyWrapper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRubyWrapper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRubyWrapper_rb_intern(This,name,pid)	\
    (This)->lpVtbl -> rb_intern(This,name,pid)

#define IRubyWrapper_rb_module_new(This,parent,p)	\
    (This)->lpVtbl -> rb_module_new(This,parent,p)

#define IRubyWrapper_SearchMethod(This,val,pName,pDispID)	\
    (This)->lpVtbl -> SearchMethod(This,val,pName,pDispID)

#define IRubyWrapper_rb_invoke(This,pengine,Module,func,pDispParams,pVarResult,ppError)	\
    (This)->lpVtbl -> rb_invoke(This,pengine,Module,func,pDispParams,pVarResult,ppError)

#define IRubyWrapper_SearchClass(This,pName,pval)	\
    (This)->lpVtbl -> SearchClass(This,pName,pval)

#define IRubyWrapper_rb_funcall_with_string2(This,pengine,val,id,line,cb,p,TraceRequired,pVarResult,ppError)	\
    (This)->lpVtbl -> rb_funcall_with_string2(This,pengine,val,id,line,cb,p,TraceRequired,pVarResult,ppError)

#define IRubyWrapper_RegisterObject(This,pName,Module)	\
    (This)->lpVtbl -> RegisterObject(This,pName,Module)

#define IRubyWrapper_UnregisterObject(This,pName,Module)	\
    (This)->lpVtbl -> UnregisterObject(This,pName,Module)

#define IRubyWrapper_Disconnect(This,pengin)	\
    (This)->lpVtbl -> Disconnect(This,pengin)

#define IRubyWrapper_DefineGlobalObject(This,pengine)	\
    (This)->lpVtbl -> DefineGlobalObject(This,pengine)

#define IRubyWrapper_DefineConstant(This,pName,Module,pVar,pengine)	\
    (This)->lpVtbl -> DefineConstant(This,pName,Module,pVar,pengine)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IRubyWrapper_rb_intern_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [string][in] */ unsigned char __RPC_FAR *name,
    /* [out] */ unsigned long __RPC_FAR *pid);


void __RPC_STUB IRubyWrapper_rb_intern_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_rb_module_new_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [in] */ unsigned long parent,
    /* [out] */ unsigned long __RPC_FAR *p);


void __RPC_STUB IRubyWrapper_rb_module_new_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_SearchMethod_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [in] */ unsigned long val,
    /* [string][in] */ unsigned char __RPC_FAR *pName,
    /* [out] */ DISPID __RPC_FAR *pDispID);


void __RPC_STUB IRubyWrapper_SearchMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_rb_invoke_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [in] */ IRubyEngine __RPC_FAR *pengine,
    /* [in] */ unsigned long Module,
    /* [in] */ unsigned long func,
    /* [in] */ DISPPARAMS __RPC_FAR *pDispParams,
    /* [unique][out][in] */ VARIANT __RPC_FAR *pVarResult,
    /* [out] */ IActiveScriptError __RPC_FAR *__RPC_FAR *ppError);


void __RPC_STUB IRubyWrapper_rb_invoke_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_SearchClass_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [string][in] */ unsigned char __RPC_FAR *pName,
    /* [out] */ unsigned long __RPC_FAR *pval);


void __RPC_STUB IRubyWrapper_SearchClass_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_rb_funcall_with_string2_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [in] */ IRubyEngine __RPC_FAR *pengine,
    /* [in] */ unsigned long val,
    /* [in] */ unsigned long id,
    /* [in] */ long line,
    /* [in] */ long cb,
    /* [size_is][in] */ unsigned char __RPC_FAR *p,
    /* [in] */ boolean TraceRequired,
    /* [out] */ VARIANT __RPC_FAR *pVarResult,
    /* [out] */ IActiveScriptError __RPC_FAR *__RPC_FAR *ppError);


void __RPC_STUB IRubyWrapper_rb_funcall_with_string2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_RegisterObject_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [string][in] */ unsigned char __RPC_FAR *pName,
    /* [in] */ unsigned long Module);


void __RPC_STUB IRubyWrapper_RegisterObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_UnregisterObject_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [string][in] */ unsigned char __RPC_FAR *pName,
    /* [in] */ unsigned long Module);


void __RPC_STUB IRubyWrapper_UnregisterObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_Disconnect_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [in] */ IRubyEngine __RPC_FAR *pengin);


void __RPC_STUB IRubyWrapper_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_DefineGlobalObject_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [in] */ IRubyEngine __RPC_FAR *pengine);


void __RPC_STUB IRubyWrapper_DefineGlobalObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRubyWrapper_DefineConstant_Proxy( 
    IRubyWrapper __RPC_FAR * This,
    /* [string][in] */ unsigned char __RPC_FAR *pName,
    /* [in] */ unsigned long Module,
    /* [in] */ VARIANT __RPC_FAR *pVar,
    /* [in] */ IRubyEngine __RPC_FAR *pengine);


void __RPC_STUB IRubyWrapper_DefineConstant_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRubyWrapper_INTERFACE_DEFINED__ */



#ifndef __RSCRIPTLib_LIBRARY_DEFINED__
#define __RSCRIPTLib_LIBRARY_DEFINED__

/* library RSCRIPTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_RSCRIPTLib;

#ifndef __IRubyScript_INTERFACE_DEFINED__
#define __IRubyScript_INTERFACE_DEFINED__

/* interface IRubyScript */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRubyScript;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5DBEF577-E278-11D3-8E7A-0000F45A3C05")
    IRubyScript : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IRubyScriptVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRubyScript __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRubyScript __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRubyScript __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRubyScript __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRubyScript __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRubyScript __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRubyScript __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IRubyScriptVtbl;

    interface IRubyScript
    {
        CONST_VTBL struct IRubyScriptVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRubyScript_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRubyScript_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRubyScript_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRubyScript_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRubyScript_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRubyScript_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRubyScript_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRubyScript_INTERFACE_DEFINED__ */


#ifndef __IRubyize_INTERFACE_DEFINED__
#define __IRubyize_INTERFACE_DEFINED__

/* interface IRubyize */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRubyize;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0A4CBEBD-C46B-4A7C-A1E2-AD474C330C7A")
    IRubyize : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR __RPC_FAR *pVersion) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_RubyVersion( 
            /* [retval][out] */ BSTR __RPC_FAR *pVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE rubyize( 
            /* [in] */ VARIANT val,
            /* [retval][out] */ VARIANT __RPC_FAR *pObj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE erubyize( 
            /* [in] */ BSTR script,
            /* [retval][out] */ VARIANT __RPC_FAR *pObj) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRubyizeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRubyize __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRubyize __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRubyize __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRubyize __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRubyize __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRubyize __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRubyize __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Version )( 
            IRubyize __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVersion);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RubyVersion )( 
            IRubyize __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *rubyize )( 
            IRubyize __RPC_FAR * This,
            /* [in] */ VARIANT val,
            /* [retval][out] */ VARIANT __RPC_FAR *pObj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *erubyize )( 
            IRubyize __RPC_FAR * This,
            /* [in] */ BSTR script,
            /* [retval][out] */ VARIANT __RPC_FAR *pObj);
        
        END_INTERFACE
    } IRubyizeVtbl;

    interface IRubyize
    {
        CONST_VTBL struct IRubyizeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRubyize_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRubyize_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRubyize_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRubyize_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRubyize_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRubyize_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRubyize_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRubyize_get_Version(This,pVersion)	\
    (This)->lpVtbl -> get_Version(This,pVersion)

#define IRubyize_get_RubyVersion(This,pVersion)	\
    (This)->lpVtbl -> get_RubyVersion(This,pVersion)

#define IRubyize_rubyize(This,val,pObj)	\
    (This)->lpVtbl -> rubyize(This,val,pObj)

#define IRubyize_erubyize(This,script,pObj)	\
    (This)->lpVtbl -> erubyize(This,script,pObj)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget][id] */ HRESULT STDMETHODCALLTYPE IRubyize_get_Version_Proxy( 
    IRubyize __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVersion);


void __RPC_STUB IRubyize_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IRubyize_get_RubyVersion_Proxy( 
    IRubyize __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVersion);


void __RPC_STUB IRubyize_get_RubyVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IRubyize_rubyize_Proxy( 
    IRubyize __RPC_FAR * This,
    /* [in] */ VARIANT val,
    /* [retval][out] */ VARIANT __RPC_FAR *pObj);


void __RPC_STUB IRubyize_rubyize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IRubyize_erubyize_Proxy( 
    IRubyize __RPC_FAR * This,
    /* [in] */ BSTR script,
    /* [retval][out] */ VARIANT __RPC_FAR *pObj);


void __RPC_STUB IRubyize_erubyize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRubyize_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_RubyScript;

#ifdef __cplusplus

class DECLSPEC_UUID("067BEC55-61A2-4cb3-A029-3BC4E18469CE")
RubyScript;
#endif

EXTERN_C const CLSID CLSID_RubyWrapper;

#ifdef __cplusplus

class DECLSPEC_UUID("38DBE621-054C-4a2e-A993-2B05CECFCB71")
RubyWrapper;
#endif

EXTERN_C const CLSID CLSID_Rubyize;

#ifdef __cplusplus

class DECLSPEC_UUID("6CB14BDE-8273-4551-810D-9809AD559228")
Rubyize;
#endif
#endif /* __RSCRIPTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
