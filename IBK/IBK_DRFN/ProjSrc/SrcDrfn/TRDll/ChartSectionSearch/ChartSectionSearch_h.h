

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for ChartSectionSearch.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0626 
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


#ifndef __ChartSectionSearch_h_h__
#define __ChartSectionSearch_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if _CONTROL_FLOW_GUARD_XFG
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IDlgForm_FWD_DEFINED__
#define __IDlgForm_FWD_DEFINED__
typedef interface IDlgForm IDlgForm;

#endif 	/* __IDlgForm_FWD_DEFINED__ */


#ifndef __DlgForm_FWD_DEFINED__
#define __DlgForm_FWD_DEFINED__

#ifdef __cplusplus
typedef class DlgForm DlgForm;
#else
typedef struct DlgForm DlgForm;
#endif /* __cplusplus */

#endif 	/* __DlgForm_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ChartSectionSearch_LIBRARY_DEFINED__
#define __ChartSectionSearch_LIBRARY_DEFINED__

/* library ChartSectionSearch */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_ChartSectionSearch,0xEBE1E62F,0xE28B,0x4ABD,0xA9,0x82,0x1A,0x90,0xEA,0x4A,0x75,0xBC);

#ifndef __IDlgForm_DISPINTERFACE_DEFINED__
#define __IDlgForm_DISPINTERFACE_DEFINED__

/* dispinterface IDlgForm */
/* [uuid] */ 


DEFINE_GUID(DIID_IDlgForm,0xE17149DD,0x8EEE,0x491D,0xA7,0x6B,0x5A,0xC8,0xED,0xE5,0x11,0xCD);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E17149DD-8EEE-491D-A76B-5AC8EDE511CD")
    IDlgForm : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDlgFormVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDlgForm * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDlgForm * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDlgForm * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDlgForm * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDlgForm * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDlgForm * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDlgForm * This,
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
    } IDlgFormVtbl;

    interface IDlgForm
    {
        CONST_VTBL struct IDlgFormVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDlgForm_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDlgForm_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDlgForm_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDlgForm_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDlgForm_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDlgForm_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDlgForm_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDlgForm_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_DlgForm,0x096B4533,0xF4DB,0x49F1,0xB1,0xD6,0x7A,0x3B,0x0C,0xDD,0xC5,0x6B);

#ifdef __cplusplus

class DECLSPEC_UUID("096B4533-F4DB-49F1-B1D6-7A3B0CDDC56B")
DlgForm;
#endif
#endif /* __ChartSectionSearch_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


