

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon May 29 17:12:44 2017
 */
/* Compiler settings for CX_FILEDIALOG.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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


#ifndef __CX_FILEDIALOG_h_h__
#define __CX_FILEDIALOG_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICtrlWnd_FWD_DEFINED__
#define __ICtrlWnd_FWD_DEFINED__
typedef interface ICtrlWnd ICtrlWnd;
#endif 	/* __ICtrlWnd_FWD_DEFINED__ */


#ifndef __CtrlWnd_FWD_DEFINED__
#define __CtrlWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class CtrlWnd CtrlWnd;
#else
typedef struct CtrlWnd CtrlWnd;
#endif /* __cplusplus */

#endif 	/* __CtrlWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_FILEDIALOG_LIBRARY_DEFINED__
#define __CX_FILEDIALOG_LIBRARY_DEFINED__

/* library CX_FILEDIALOG */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_FILEDIALOG,0xB18C3362,0xFD7A,0x4EBC,0xA5,0x7F,0xFD,0x41,0x55,0x03,0x81,0x51);

#ifndef __ICtrlWnd_DISPINTERFACE_DEFINED__
#define __ICtrlWnd_DISPINTERFACE_DEFINED__

/* dispinterface ICtrlWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_ICtrlWnd,0xC22E2C6B,0x43EB,0x44A4,0x92,0x21,0x01,0xCB,0x8A,0x8A,0x0D,0x75);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C22E2C6B-43EB-44A4-9221-01CB8A8A0D75")
    ICtrlWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ICtrlWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICtrlWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICtrlWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICtrlWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICtrlWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICtrlWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICtrlWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICtrlWnd * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ICtrlWndVtbl;

    interface ICtrlWnd
    {
        CONST_VTBL struct ICtrlWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICtrlWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICtrlWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICtrlWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICtrlWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICtrlWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICtrlWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICtrlWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ICtrlWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_CtrlWnd,0x23D7C761,0x896B,0x4F15,0xAC,0x90,0xC9,0xD1,0x40,0x38,0x29,0xCF);

#ifdef __cplusplus

class DECLSPEC_UUID("23D7C761-896B-4F15-AC90-C9D1403829CF")
CtrlWnd;
#endif
#endif /* __CX_FILEDIALOG_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


