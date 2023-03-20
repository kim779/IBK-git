

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for C_Tick.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __C_Tick_h_h__
#define __C_Tick_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IGrpWnd_FWD_DEFINED__
#define __IGrpWnd_FWD_DEFINED__
typedef interface IGrpWnd IGrpWnd;

#endif 	/* __IGrpWnd_FWD_DEFINED__ */


#ifndef __GrpWnd_FWD_DEFINED__
#define __GrpWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class GrpWnd GrpWnd;
#else
typedef struct GrpWnd GrpWnd;
#endif /* __cplusplus */

#endif 	/* __GrpWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __C_Tick_LIBRARY_DEFINED__
#define __C_Tick_LIBRARY_DEFINED__

/* library C_Tick */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_C_Tick,0x8BA76AB3,0x1787,0x4424,0xB0,0x0C,0x08,0x53,0x83,0x4B,0xFB,0xB4);

#ifndef __IGrpWnd_DISPINTERFACE_DEFINED__
#define __IGrpWnd_DISPINTERFACE_DEFINED__

/* dispinterface IGrpWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IGrpWnd,0xC34A1D1B,0x1E59,0x4117,0x88,0xCA,0x12,0x3F,0x45,0x56,0xBD,0xBE);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C34A1D1B-1E59-4117-88CA-123F4556BDBE")
    IGrpWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IGrpWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGrpWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGrpWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGrpWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGrpWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGrpWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGrpWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGrpWnd * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IGrpWndVtbl;

    interface IGrpWnd
    {
        CONST_VTBL struct IGrpWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGrpWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGrpWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGrpWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGrpWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGrpWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGrpWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGrpWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IGrpWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_GrpWnd,0x0C369B36,0x8806,0x42C3,0x82,0xF9,0x15,0x5B,0xC0,0xBD,0x0B,0x9F);

#ifdef __cplusplus

class DECLSPEC_UUID("0C369B36-8806-42C3-82F9-155BC0BD0B9F")
GrpWnd;
#endif
#endif /* __C_Tick_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


