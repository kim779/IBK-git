

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for cx_SimpleEasy.odl:
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


#ifndef __cx_SimpleEasy_h_h__
#define __cx_SimpleEasy_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDlgQr_FWD_DEFINED__
#define __IDlgQr_FWD_DEFINED__
typedef interface IDlgQr IDlgQr;

#endif 	/* __IDlgQr_FWD_DEFINED__ */


#ifndef __DlgQr_FWD_DEFINED__
#define __DlgQr_FWD_DEFINED__

#ifdef __cplusplus
typedef class DlgQr DlgQr;
#else
typedef struct DlgQr DlgQr;
#endif /* __cplusplus */

#endif 	/* __DlgQr_FWD_DEFINED__ */


#ifndef __IMainWnd_FWD_DEFINED__
#define __IMainWnd_FWD_DEFINED__
typedef interface IMainWnd IMainWnd;

#endif 	/* __IMainWnd_FWD_DEFINED__ */


#ifndef __MainWnd_FWD_DEFINED__
#define __MainWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class MainWnd MainWnd;
#else
typedef struct MainWnd MainWnd;
#endif /* __cplusplus */

#endif 	/* __MainWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Cx_SimpleEasy_LIBRARY_DEFINED__
#define __Cx_SimpleEasy_LIBRARY_DEFINED__

/* library Cx_SimpleEasy */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Cx_SimpleEasy,0x26637FAF,0x9F12,0x40C6,0xB6,0xE3,0x90,0xDB,0x82,0x93,0x86,0xF5);

#ifndef __IDlgQr_DISPINTERFACE_DEFINED__
#define __IDlgQr_DISPINTERFACE_DEFINED__

/* dispinterface IDlgQr */
/* [uuid] */ 


DEFINE_GUID(DIID_IDlgQr,0xD3BD38F3,0x216F,0x477C,0x8B,0xC7,0xEB,0x07,0x41,0x2C,0xB9,0xB9);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D3BD38F3-216F-477C-8BC7-EB07412CB9B9")
    IDlgQr : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDlgQrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDlgQr * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDlgQr * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDlgQr * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDlgQr * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDlgQr * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDlgQr * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDlgQr * This,
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
    } IDlgQrVtbl;

    interface IDlgQr
    {
        CONST_VTBL struct IDlgQrVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDlgQr_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDlgQr_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDlgQr_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDlgQr_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDlgQr_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDlgQr_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDlgQr_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDlgQr_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_DlgQr,0x9A7384D3,0x4783,0x4DB0,0xA1,0x0D,0x71,0x71,0x60,0x8E,0x4C,0x1A);

#ifdef __cplusplus

class DECLSPEC_UUID("9A7384D3-4783-4DB0-A10D-7171608E4C1A")
DlgQr;
#endif

#ifndef __IMainWnd_DISPINTERFACE_DEFINED__
#define __IMainWnd_DISPINTERFACE_DEFINED__

/* dispinterface IMainWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IMainWnd,0xE046FA1A,0x7D7E,0x48A9,0x88,0xD9,0xDC,0xF3,0x17,0xAC,0x1C,0x76);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E046FA1A-7D7E-48A9-88D9-DCF317AC1C76")
    IMainWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMainWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMainWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMainWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMainWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMainWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMainWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMainWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMainWnd * This,
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
    } IMainWndVtbl;

    interface IMainWnd
    {
        CONST_VTBL struct IMainWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMainWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMainWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMainWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMainWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMainWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMainWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMainWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMainWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MainWnd,0x6CB2C0F2,0x5594,0x452D,0x98,0x9E,0x18,0x35,0x02,0x46,0x2C,0xF5);

#ifdef __cplusplus

class DECLSPEC_UUID("6CB2C0F2-5594-452D-989E-183502462CF5")
MainWnd;
#endif
#endif /* __Cx_SimpleEasy_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


