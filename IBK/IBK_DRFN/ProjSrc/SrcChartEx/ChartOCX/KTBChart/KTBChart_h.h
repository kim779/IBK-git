

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for KTBChart.odl:
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


#ifndef __KTBChart_h_h__
#define __KTBChart_h_h__

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

#ifndef ___DIBKChart_FWD_DEFINED__
#define ___DIBKChart_FWD_DEFINED__
typedef interface _DIBKChart _DIBKChart;

#endif 	/* ___DIBKChart_FWD_DEFINED__ */


#ifndef ___DIBKChartEvents_FWD_DEFINED__
#define ___DIBKChartEvents_FWD_DEFINED__
typedef interface _DIBKChartEvents _DIBKChartEvents;

#endif 	/* ___DIBKChartEvents_FWD_DEFINED__ */


#ifndef __IBKChart_FWD_DEFINED__
#define __IBKChart_FWD_DEFINED__

#ifdef __cplusplus
typedef class IBKChart IBKChart;
#else
typedef struct IBKChart IBKChart;
#endif /* __cplusplus */

#endif 	/* __IBKChart_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __IBKChartLib_LIBRARY_DEFINED__
#define __IBKChartLib_LIBRARY_DEFINED__

/* library IBKChartLib */
/* [control][helpstring][helpfile][version][uuid] */ 

#pragma external_header(push)
#pragma external_header(pop)
#pragma external_header(push)
#pragma external_header(push)
#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma external_header(push)
#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma warning(pop)
#pragma external_header(pop)
#pragma warning(pop)
#pragma external_header(pop)
#pragma region Desktop Family
#pragma endregion
#pragma external_header(pop)

DEFINE_GUID(LIBID_IBKChartLib,0xD64827B8,0x55F0,0x4399,0xBE,0xDD,0x0D,0x2A,0xD9,0xC6,0x54,0x51);

#ifndef ___DIBKChart_DISPINTERFACE_DEFINED__
#define ___DIBKChart_DISPINTERFACE_DEFINED__

/* dispinterface _DIBKChart */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DIBKChart,0x5E2211A8,0x2AFB,0x4338,0xB4,0x0D,0xE9,0x00,0x73,0x0A,0x9D,0xDF);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5E2211A8-2AFB-4338-B40D-E900730A9DDF")
    _DIBKChart : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIBKChartVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIBKChart * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIBKChart * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIBKChart * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIBKChart * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIBKChart * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIBKChart * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIBKChart * This,
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
    } _DIBKChartVtbl;

    interface _DIBKChart
    {
        CONST_VTBL struct _DIBKChartVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIBKChart_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DIBKChart_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DIBKChart_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DIBKChart_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DIBKChart_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DIBKChart_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DIBKChart_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIBKChart_DISPINTERFACE_DEFINED__ */


#ifndef ___DIBKChartEvents_DISPINTERFACE_DEFINED__
#define ___DIBKChartEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DIBKChartEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DIBKChartEvents,0x6390F0D0,0x6F41,0x4492,0x8B,0x66,0x50,0x22,0xD0,0x5B,0x8E,0xF8);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6390F0D0-6F41-4492-8B66-5022D05B8EF8")
    _DIBKChartEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIBKChartEventsVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIBKChartEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIBKChartEvents * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIBKChartEvents * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIBKChartEvents * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIBKChartEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIBKChartEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIBKChartEvents * This,
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
    } _DIBKChartEventsVtbl;

    interface _DIBKChartEvents
    {
        CONST_VTBL struct _DIBKChartEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIBKChartEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DIBKChartEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DIBKChartEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DIBKChartEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DIBKChartEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DIBKChartEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DIBKChartEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIBKChartEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_IBKChart,0xB98261E1,0xB73D,0x4bb1,0x90,0xD6,0xE6,0xC8,0xBA,0xAE,0xEB,0xD5);

#ifdef __cplusplus

class DECLSPEC_UUID("B98261E1-B73D-4bb1-90D6-E6C8BAAEEBD5")
IBKChart;
#endif
#endif /* __IBKChartLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


