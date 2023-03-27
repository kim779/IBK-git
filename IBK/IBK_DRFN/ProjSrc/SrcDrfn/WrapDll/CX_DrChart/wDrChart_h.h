

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for wDrChart.odl:
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


#ifndef __wDrChart_h_h__
#define __wDrChart_h_h__

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

#ifndef __IDrChartCtl_FWD_DEFINED__
#define __IDrChartCtl_FWD_DEFINED__
typedef interface IDrChartCtl IDrChartCtl;

#endif 	/* __IDrChartCtl_FWD_DEFINED__ */


#ifndef __IDrChartCtlEvents_FWD_DEFINED__
#define __IDrChartCtlEvents_FWD_DEFINED__
typedef interface IDrChartCtlEvents IDrChartCtlEvents;

#endif 	/* __IDrChartCtlEvents_FWD_DEFINED__ */


#ifndef __DrChartCtl_FWD_DEFINED__
#define __DrChartCtl_FWD_DEFINED__

#ifdef __cplusplus
typedef class DrChartCtl DrChartCtl;
#else
typedef struct DrChartCtl DrChartCtl;
#endif /* __cplusplus */

#endif 	/* __DrChartCtl_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __WDrChart_LIBRARY_DEFINED__
#define __WDrChart_LIBRARY_DEFINED__

/* library WDrChart */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_WDrChart,0x6790DD3A,0x96F7,0x4CB1,0xA3,0xC8,0x76,0x7F,0x80,0xE2,0x87,0xF3);

#ifndef __IDrChartCtl_DISPINTERFACE_DEFINED__
#define __IDrChartCtl_DISPINTERFACE_DEFINED__

/* dispinterface IDrChartCtl */
/* [uuid] */ 


DEFINE_GUID(DIID_IDrChartCtl,0x75A47147,0x9C93,0x4E1D,0xA7,0xA1,0x25,0xC6,0xFC,0xAE,0x8E,0xE9);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("75A47147-9C93-4E1D-A7A1-25C6FCAE8EE9")
    IDrChartCtl : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDrChartCtlVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDrChartCtl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDrChartCtl * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDrChartCtl * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDrChartCtl * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDrChartCtl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDrChartCtl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDrChartCtl * This,
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
    } IDrChartCtlVtbl;

    interface IDrChartCtl
    {
        CONST_VTBL struct IDrChartCtlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDrChartCtl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDrChartCtl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDrChartCtl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDrChartCtl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDrChartCtl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDrChartCtl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDrChartCtl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDrChartCtl_DISPINTERFACE_DEFINED__ */


#ifndef __IDrChartCtlEvents_DISPINTERFACE_DEFINED__
#define __IDrChartCtlEvents_DISPINTERFACE_DEFINED__

/* dispinterface IDrChartCtlEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID_IDrChartCtlEvents,0x3490D5B1,0x840B,0x474a,0xAF,0xBC,0xBB,0x25,0x78,0xA4,0x73,0x6A);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3490D5B1-840B-474a-AFBC-BB2578A4736A")
    IDrChartCtlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDrChartCtlEventsVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDrChartCtlEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDrChartCtlEvents * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDrChartCtlEvents * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDrChartCtlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDrChartCtlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDrChartCtlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDrChartCtlEvents * This,
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
    } IDrChartCtlEventsVtbl;

    interface IDrChartCtlEvents
    {
        CONST_VTBL struct IDrChartCtlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDrChartCtlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDrChartCtlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDrChartCtlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDrChartCtlEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDrChartCtlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDrChartCtlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDrChartCtlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDrChartCtlEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_DrChartCtl,0xE1B10A45,0x0CE0,0x4682,0x80,0x93,0x27,0x15,0x2F,0xB5,0xEF,0x1A);

#ifdef __cplusplus

class DECLSPEC_UUID("E1B10A45-0CE0-4682-8093-27152FB5EF1A")
DrChartCtl;
#endif
#endif /* __WDrChart_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


