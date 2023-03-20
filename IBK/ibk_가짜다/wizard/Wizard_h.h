

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Wizard.odl:
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


#ifndef __Wizard_h_h__
#define __Wizard_h_h__

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

#ifndef ___DWizard_FWD_DEFINED__
#define ___DWizard_FWD_DEFINED__
typedef interface _DWizard _DWizard;

#endif 	/* ___DWizard_FWD_DEFINED__ */


#ifndef ___DWizardEvents_FWD_DEFINED__
#define ___DWizardEvents_FWD_DEFINED__
typedef interface _DWizardEvents _DWizardEvents;

#endif 	/* ___DWizardEvents_FWD_DEFINED__ */


#ifndef __Wizard_FWD_DEFINED__
#define __Wizard_FWD_DEFINED__

#ifdef __cplusplus
typedef class Wizard Wizard;
#else
typedef struct Wizard Wizard;
#endif /* __cplusplus */

#endif 	/* __Wizard_FWD_DEFINED__ */


#ifndef __IxScreen_FWD_DEFINED__
#define __IxScreen_FWD_DEFINED__
typedef interface IxScreen IxScreen;

#endif 	/* __IxScreen_FWD_DEFINED__ */


#ifndef __xScreen_FWD_DEFINED__
#define __xScreen_FWD_DEFINED__

#ifdef __cplusplus
typedef class xScreen xScreen;
#else
typedef struct xScreen xScreen;
#endif /* __cplusplus */

#endif 	/* __xScreen_FWD_DEFINED__ */


#ifndef __IxSystem_FWD_DEFINED__
#define __IxSystem_FWD_DEFINED__
typedef interface IxSystem IxSystem;

#endif 	/* __IxSystem_FWD_DEFINED__ */


#ifndef __xSystem_FWD_DEFINED__
#define __xSystem_FWD_DEFINED__

#ifdef __cplusplus
typedef class xSystem xSystem;
#else
typedef struct xSystem xSystem;
#endif /* __cplusplus */

#endif 	/* __xSystem_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __WIZARDLib_LIBRARY_DEFINED__
#define __WIZARDLib_LIBRARY_DEFINED__

/* library WIZARDLib */
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

DEFINE_GUID(LIBID_WIZARDLib,0x71D618DE,0xD3F1,0x4EB8,0x86,0x2C,0x79,0xD3,0x06,0x06,0x20,0x8F);

#ifndef ___DWizard_DISPINTERFACE_DEFINED__
#define ___DWizard_DISPINTERFACE_DEFINED__

/* dispinterface _DWizard */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DWizard,0x11B7DB3B,0x047C,0x49D3,0x91,0xFB,0x89,0x4C,0xB1,0xBE,0x9C,0x76);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("11B7DB3B-047C-49D3-91FB-894CB1BE9C76")
    _DWizard : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWizardVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWizard * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWizard * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWizard * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWizard * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWizard * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWizard * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWizard * This,
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
    } _DWizardVtbl;

    interface _DWizard
    {
        CONST_VTBL struct _DWizardVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWizard_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWizard_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWizard_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWizard_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWizard_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWizard_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWizard_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWizard_DISPINTERFACE_DEFINED__ */


#ifndef ___DWizardEvents_DISPINTERFACE_DEFINED__
#define ___DWizardEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DWizardEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DWizardEvents,0x4C00CF12,0x7418,0x4B50,0x8A,0xBF,0x32,0x2D,0x0F,0x61,0xEC,0xFB);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4C00CF12-7418-4B50-8ABF-322D0F61ECFB")
    _DWizardEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWizardEventsVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWizardEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWizardEvents * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWizardEvents * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWizardEvents * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWizardEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWizardEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWizardEvents * This,
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
    } _DWizardEventsVtbl;

    interface _DWizardEvents
    {
        CONST_VTBL struct _DWizardEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWizardEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWizardEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWizardEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWizardEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWizardEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWizardEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWizardEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWizardEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Wizard,0x7B26DE02,0xE5EA,0x4ED5,0xAA,0x20,0xFB,0x09,0x40,0x62,0x36,0x6C);

#ifdef __cplusplus

class DECLSPEC_UUID("7B26DE02-E5EA-4ED5-AA20-FB094062366C")
Wizard;
#endif

#ifndef __IxScreen_DISPINTERFACE_DEFINED__
#define __IxScreen_DISPINTERFACE_DEFINED__

/* dispinterface IxScreen */
/* [uuid] */ 


DEFINE_GUID(DIID_IxScreen,0x2819C94F,0x275F,0x4FE0,0xA2,0x76,0x84,0x56,0x0E,0x42,0x85,0xB5);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2819C94F-275F-4FE0-A276-84560E4285B5")
    IxScreen : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IxScreenVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IxScreen * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IxScreen * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IxScreen * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IxScreen * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IxScreen * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IxScreen * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IxScreen * This,
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
    } IxScreenVtbl;

    interface IxScreen
    {
        CONST_VTBL struct IxScreenVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IxScreen_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IxScreen_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IxScreen_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IxScreen_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IxScreen_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IxScreen_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IxScreen_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IxScreen_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_xScreen,0x238D9944,0x44DD,0x42DB,0x86,0xD4,0xDC,0x6F,0xE0,0xDF,0xB3,0x2B);

#ifdef __cplusplus

class DECLSPEC_UUID("238D9944-44DD-42DB-86D4-DC6FE0DFB32B")
xScreen;
#endif

#ifndef __IxSystem_DISPINTERFACE_DEFINED__
#define __IxSystem_DISPINTERFACE_DEFINED__

/* dispinterface IxSystem */
/* [uuid] */ 


DEFINE_GUID(DIID_IxSystem,0x0325C6BE,0x9B41,0x4C2D,0x97,0x94,0xED,0xD6,0xBE,0x0A,0x30,0x4D);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0325C6BE-9B41-4C2D-9794-EDD6BE0A304D")
    IxSystem : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IxSystemVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IxSystem * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IxSystem * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IxSystem * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IxSystem * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IxSystem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IxSystem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IxSystem * This,
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
    } IxSystemVtbl;

    interface IxSystem
    {
        CONST_VTBL struct IxSystemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IxSystem_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IxSystem_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IxSystem_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IxSystem_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IxSystem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IxSystem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IxSystem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IxSystem_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_xSystem,0x83F7E2C8,0x8CB1,0x4F4A,0xB0,0x39,0xD8,0xBF,0x89,0x00,0xBE,0xE3);

#ifdef __cplusplus

class DECLSPEC_UUID("83F7E2C8-8CB1-4F4A-B039-D8BF8900BEE3")
xSystem;
#endif
#endif /* __WIZARDLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


