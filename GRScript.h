/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Feb 14 23:47:48 2009
 */
/* Compiler settings for C:\home\arton\projects\RScript19\trunk\GRScript.idl:
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

#ifndef __GRScript_h__
#define __GRScript_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IGlobalRubyScript_FWD_DEFINED__
#define __IGlobalRubyScript_FWD_DEFINED__
typedef interface IGlobalRubyScript IGlobalRubyScript;
#endif 	/* __IGlobalRubyScript_FWD_DEFINED__ */


#ifndef __GlobalRubyScript_FWD_DEFINED__
#define __GlobalRubyScript_FWD_DEFINED__

#ifdef __cplusplus
typedef class GlobalRubyScript GlobalRubyScript;
#else
typedef struct GlobalRubyScript GlobalRubyScript;
#endif /* __cplusplus */

#endif 	/* __GlobalRubyScript_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "activscp.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __GRSCRIPTLib_LIBRARY_DEFINED__
#define __GRSCRIPTLib_LIBRARY_DEFINED__

/* library GRSCRIPTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_GRSCRIPTLib;

#ifndef __IGlobalRubyScript_INTERFACE_DEFINED__
#define __IGlobalRubyScript_INTERFACE_DEFINED__

/* interface IGlobalRubyScript */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IGlobalRubyScript;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0AC0D357-E866-11D3-8E82-0000F45A3C05")
    IGlobalRubyScript : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IGlobalRubyScriptVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGlobalRubyScript __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGlobalRubyScript __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGlobalRubyScript __RPC_FAR * This);
        
        END_INTERFACE
    } IGlobalRubyScriptVtbl;

    interface IGlobalRubyScript
    {
        CONST_VTBL struct IGlobalRubyScriptVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGlobalRubyScript_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGlobalRubyScript_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGlobalRubyScript_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGlobalRubyScript_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_GlobalRubyScript;

#ifdef __cplusplus

class DECLSPEC_UUID("0A383007-32FE-4ec0-97D6-59892ECD5413")
GlobalRubyScript;
#endif
#endif /* __GRSCRIPTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
