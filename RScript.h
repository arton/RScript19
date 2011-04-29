

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Apr 29 13:31:24 2011
 */
/* Compiler settings for RScript.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
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

#ifdef __cplusplus
extern "C"{
#endif 


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
            /* [out] */ IUnknown **ppv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckInterfaceIsSafety( 
            /* [in] */ CLSID clsid,
            /* [in] */ IUnknown *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckActiveXIsSafety( 
            /* [in] */ CLSID clsid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnterScript( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LeaveScript( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnScriptError( 
            /* [in] */ IActiveScriptError *pError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterServiceProvider( 
            /* [in] */ IServiceProvider *pProvider) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterServiceProvider( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSecurityLevel( 
            /* [retval][out] */ long *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearModuleObject( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRubyize( 
            /* [in] */ boolean f) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRubyize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetModule( 
            /* [retval][out] */ unsigned long *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DisconnectObjects( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DefineGlobalObject( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRubyEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRubyEngine * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRubyEngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRubyEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateObject )( 
            IRubyEngine * This,
            /* [in] */ CLSID clsid,
            /* [string][in] */ LPCWSTR pszHost,
            /* [in] */ REFIID riid,
            /* [out] */ IUnknown **ppv);
        
        HRESULT ( STDMETHODCALLTYPE *CheckInterfaceIsSafety )( 
            IRubyEngine * This,
            /* [in] */ CLSID clsid,
            /* [in] */ IUnknown *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE *CheckActiveXIsSafety )( 
            IRubyEngine * This,
            /* [in] */ CLSID clsid);
        
        HRESULT ( STDMETHODCALLTYPE *EnterScript )( 
            IRubyEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *LeaveScript )( 
            IRubyEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnScriptError )( 
            IRubyEngine * This,
            /* [in] */ IActiveScriptError *pError);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterServiceProvider )( 
            IRubyEngine * This,
            /* [in] */ IServiceProvider *pProvider);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterServiceProvider )( 
            IRubyEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSecurityLevel )( 
            IRubyEngine * This,
            /* [retval][out] */ long *pResult);
        
        HRESULT ( STDMETHODCALLTYPE *ClearModuleObject )( 
            IRubyEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetRubyize )( 
            IRubyEngine * This,
            /* [in] */ boolean f);
        
        HRESULT ( STDMETHODCALLTYPE *GetRubyize )( 
            IRubyEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetModule )( 
            IRubyEngine * This,
            /* [retval][out] */ unsigned long *pResult);
        
        HRESULT ( STDMETHODCALLTYPE *DisconnectObjects )( 
            IRubyEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *DefineGlobalObject )( 
            IRubyEngine * This);
        
        END_INTERFACE
    } IRubyEngineVtbl;

    interface IRubyEngine
    {
        CONST_VTBL struct IRubyEngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRubyEngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRubyEngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRubyEngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRubyEngine_CreateObject(This,clsid,pszHost,riid,ppv)	\
    ( (This)->lpVtbl -> CreateObject(This,clsid,pszHost,riid,ppv) ) 

#define IRubyEngine_CheckInterfaceIsSafety(This,clsid,pUnk)	\
    ( (This)->lpVtbl -> CheckInterfaceIsSafety(This,clsid,pUnk) ) 

#define IRubyEngine_CheckActiveXIsSafety(This,clsid)	\
    ( (This)->lpVtbl -> CheckActiveXIsSafety(This,clsid) ) 

#define IRubyEngine_EnterScript(This)	\
    ( (This)->lpVtbl -> EnterScript(This) ) 

#define IRubyEngine_LeaveScript(This)	\
    ( (This)->lpVtbl -> LeaveScript(This) ) 

#define IRubyEngine_OnScriptError(This,pError)	\
    ( (This)->lpVtbl -> OnScriptError(This,pError) ) 

#define IRubyEngine_RegisterServiceProvider(This,pProvider)	\
    ( (This)->lpVtbl -> RegisterServiceProvider(This,pProvider) ) 

#define IRubyEngine_UnregisterServiceProvider(This)	\
    ( (This)->lpVtbl -> UnregisterServiceProvider(This) ) 

#define IRubyEngine_GetSecurityLevel(This,pResult)	\
    ( (This)->lpVtbl -> GetSecurityLevel(This,pResult) ) 

#define IRubyEngine_ClearModuleObject(This)	\
    ( (This)->lpVtbl -> ClearModuleObject(This) ) 

#define IRubyEngine_SetRubyize(This,f)	\
    ( (This)->lpVtbl -> SetRubyize(This,f) ) 

#define IRubyEngine_GetRubyize(This)	\
    ( (This)->lpVtbl -> GetRubyize(This) ) 

#define IRubyEngine_GetModule(This,pResult)	\
    ( (This)->lpVtbl -> GetModule(This,pResult) ) 

#define IRubyEngine_DisconnectObjects(This)	\
    ( (This)->lpVtbl -> DisconnectObjects(This) ) 

#define IRubyEngine_DefineGlobalObject(This)	\
    ( (This)->lpVtbl -> DefineGlobalObject(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




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
            /* [string][in] */ unsigned char *name,
            /* [out] */ unsigned long *pid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE rb_module_new( 
            /* [in] */ unsigned long parent,
            /* [out] */ unsigned long *p) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SearchMethod( 
            /* [in] */ unsigned long val,
            /* [string][in] */ unsigned char *pName,
            /* [out] */ DISPID *pDispID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE rb_invoke( 
            /* [in] */ IRubyEngine *pengine,
            /* [in] */ unsigned long Module,
            /* [in] */ unsigned long func,
            /* [in] */ DISPPARAMS *pDispParams,
            /* [unique][out][in] */ VARIANT *pVarResult,
            /* [out] */ IActiveScriptError **ppError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SearchClass( 
            /* [string][in] */ unsigned char *pName,
            /* [out] */ unsigned long *pval) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE rb_funcall_with_string2( 
            /* [in] */ IRubyEngine *pengine,
            /* [in] */ unsigned long val,
            /* [in] */ unsigned long id,
            /* [in] */ long line,
            /* [in] */ long cb,
            /* [size_is][in] */ unsigned char *p,
            /* [in] */ boolean TraceRequired,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ IActiveScriptError **ppError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterObject( 
            /* [string][in] */ unsigned char *pName,
            /* [in] */ unsigned long Module) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterObject( 
            /* [string][in] */ unsigned char *pName,
            /* [in] */ unsigned long Module) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Disconnect( 
            /* [in] */ IRubyEngine *pengin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DefineGlobalObject( 
            /* [in] */ IRubyEngine *pengine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DefineConstant( 
            /* [string][in] */ unsigned char *pName,
            /* [in] */ unsigned long Module,
            /* [in] */ VARIANT *pVar,
            /* [in] */ IRubyEngine *pengine) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRubyWrapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRubyWrapper * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRubyWrapper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRubyWrapper * This);
        
        HRESULT ( STDMETHODCALLTYPE *rb_intern )( 
            IRubyWrapper * This,
            /* [string][in] */ unsigned char *name,
            /* [out] */ unsigned long *pid);
        
        HRESULT ( STDMETHODCALLTYPE *rb_module_new )( 
            IRubyWrapper * This,
            /* [in] */ unsigned long parent,
            /* [out] */ unsigned long *p);
        
        HRESULT ( STDMETHODCALLTYPE *SearchMethod )( 
            IRubyWrapper * This,
            /* [in] */ unsigned long val,
            /* [string][in] */ unsigned char *pName,
            /* [out] */ DISPID *pDispID);
        
        HRESULT ( STDMETHODCALLTYPE *rb_invoke )( 
            IRubyWrapper * This,
            /* [in] */ IRubyEngine *pengine,
            /* [in] */ unsigned long Module,
            /* [in] */ unsigned long func,
            /* [in] */ DISPPARAMS *pDispParams,
            /* [unique][out][in] */ VARIANT *pVarResult,
            /* [out] */ IActiveScriptError **ppError);
        
        HRESULT ( STDMETHODCALLTYPE *SearchClass )( 
            IRubyWrapper * This,
            /* [string][in] */ unsigned char *pName,
            /* [out] */ unsigned long *pval);
        
        HRESULT ( STDMETHODCALLTYPE *rb_funcall_with_string2 )( 
            IRubyWrapper * This,
            /* [in] */ IRubyEngine *pengine,
            /* [in] */ unsigned long val,
            /* [in] */ unsigned long id,
            /* [in] */ long line,
            /* [in] */ long cb,
            /* [size_is][in] */ unsigned char *p,
            /* [in] */ boolean TraceRequired,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ IActiveScriptError **ppError);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterObject )( 
            IRubyWrapper * This,
            /* [string][in] */ unsigned char *pName,
            /* [in] */ unsigned long Module);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterObject )( 
            IRubyWrapper * This,
            /* [string][in] */ unsigned char *pName,
            /* [in] */ unsigned long Module);
        
        HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            IRubyWrapper * This,
            /* [in] */ IRubyEngine *pengin);
        
        HRESULT ( STDMETHODCALLTYPE *DefineGlobalObject )( 
            IRubyWrapper * This,
            /* [in] */ IRubyEngine *pengine);
        
        HRESULT ( STDMETHODCALLTYPE *DefineConstant )( 
            IRubyWrapper * This,
            /* [string][in] */ unsigned char *pName,
            /* [in] */ unsigned long Module,
            /* [in] */ VARIANT *pVar,
            /* [in] */ IRubyEngine *pengine);
        
        END_INTERFACE
    } IRubyWrapperVtbl;

    interface IRubyWrapper
    {
        CONST_VTBL struct IRubyWrapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRubyWrapper_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRubyWrapper_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRubyWrapper_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRubyWrapper_rb_intern(This,name,pid)	\
    ( (This)->lpVtbl -> rb_intern(This,name,pid) ) 

#define IRubyWrapper_rb_module_new(This,parent,p)	\
    ( (This)->lpVtbl -> rb_module_new(This,parent,p) ) 

#define IRubyWrapper_SearchMethod(This,val,pName,pDispID)	\
    ( (This)->lpVtbl -> SearchMethod(This,val,pName,pDispID) ) 

#define IRubyWrapper_rb_invoke(This,pengine,Module,func,pDispParams,pVarResult,ppError)	\
    ( (This)->lpVtbl -> rb_invoke(This,pengine,Module,func,pDispParams,pVarResult,ppError) ) 

#define IRubyWrapper_SearchClass(This,pName,pval)	\
    ( (This)->lpVtbl -> SearchClass(This,pName,pval) ) 

#define IRubyWrapper_rb_funcall_with_string2(This,pengine,val,id,line,cb,p,TraceRequired,pVarResult,ppError)	\
    ( (This)->lpVtbl -> rb_funcall_with_string2(This,pengine,val,id,line,cb,p,TraceRequired,pVarResult,ppError) ) 

#define IRubyWrapper_RegisterObject(This,pName,Module)	\
    ( (This)->lpVtbl -> RegisterObject(This,pName,Module) ) 

#define IRubyWrapper_UnregisterObject(This,pName,Module)	\
    ( (This)->lpVtbl -> UnregisterObject(This,pName,Module) ) 

#define IRubyWrapper_Disconnect(This,pengin)	\
    ( (This)->lpVtbl -> Disconnect(This,pengin) ) 

#define IRubyWrapper_DefineGlobalObject(This,pengine)	\
    ( (This)->lpVtbl -> DefineGlobalObject(This,pengine) ) 

#define IRubyWrapper_DefineConstant(This,pName,Module,pVar,pengine)	\
    ( (This)->lpVtbl -> DefineConstant(This,pName,Module,pVar,pengine) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




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
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRubyScript * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRubyScript * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRubyScript * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRubyScript * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRubyScript * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRubyScript * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRubyScript * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IRubyScriptVtbl;

    interface IRubyScript
    {
        CONST_VTBL struct IRubyScriptVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRubyScript_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRubyScript_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRubyScript_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRubyScript_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRubyScript_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRubyScript_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRubyScript_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


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
            /* [retval][out] */ BSTR *pVersion) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_RubyVersion( 
            /* [retval][out] */ BSTR *pVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE rubyize( 
            /* [in] */ VARIANT val,
            /* [retval][out] */ VARIANT *pObj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE erubyize( 
            /* [in] */ BSTR script,
            /* [retval][out] */ VARIANT *pObj) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRubyizeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRubyize * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRubyize * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRubyize * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRubyize * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRubyize * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRubyize * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRubyize * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IRubyize * This,
            /* [retval][out] */ BSTR *pVersion);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RubyVersion )( 
            IRubyize * This,
            /* [retval][out] */ BSTR *pVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *rubyize )( 
            IRubyize * This,
            /* [in] */ VARIANT val,
            /* [retval][out] */ VARIANT *pObj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *erubyize )( 
            IRubyize * This,
            /* [in] */ BSTR script,
            /* [retval][out] */ VARIANT *pObj);
        
        END_INTERFACE
    } IRubyizeVtbl;

    interface IRubyize
    {
        CONST_VTBL struct IRubyizeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRubyize_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRubyize_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRubyize_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRubyize_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRubyize_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRubyize_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRubyize_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRubyize_get_Version(This,pVersion)	\
    ( (This)->lpVtbl -> get_Version(This,pVersion) ) 

#define IRubyize_get_RubyVersion(This,pVersion)	\
    ( (This)->lpVtbl -> get_RubyVersion(This,pVersion) ) 

#define IRubyize_rubyize(This,val,pObj)	\
    ( (This)->lpVtbl -> rubyize(This,val,pObj) ) 

#define IRubyize_erubyize(This,script,pObj)	\
    ( (This)->lpVtbl -> erubyize(This,script,pObj) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




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

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


