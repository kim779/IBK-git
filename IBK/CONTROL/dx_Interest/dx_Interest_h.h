

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for dx_Interest.odl:
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


#ifndef __dx_Interest_h_h__
#define __dx_Interest_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IComponent_FWD_DEFINED__
#define __IComponent_FWD_DEFINED__
typedef interface IComponent IComponent;

#endif 	/* __IComponent_FWD_DEFINED__ */


#ifndef __Component_FWD_DEFINED__
#define __Component_FWD_DEFINED__

#ifdef __cplusplus
typedef class Component Component;
#else
typedef struct Component Component;
#endif /* __cplusplus */

#endif 	/* __Component_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __dx_Interest_LIBRARY_DEFINED__
#define __dx_Interest_LIBRARY_DEFINED__

/* library dx_Interest */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_dx_Interest,0x44AA5DCD,0x247C,0x41B7,0xB6,0x82,0xDE,0x95,0xD0,0xCC,0x5D,0xF8);

#ifndef __IComponent_DISPINTERFACE_DEFINED__
#define __IComponent_DISPINTERFACE_DEFINED__

/* dispinterface IComponent */
/* [uuid] */ 


DEFINE_GUID(DIID_IComponent,0x5C298934,0x6200,0x4DFD,0x98,0x18,0x80,0x29,0xA7,0xCC,0x04,0xFB);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5C298934-6200-4DFD-9818-8029A7CC04FB")
    IComponent : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IComponentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComponent * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComponent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComponent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IComponent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IComponent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IComponent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IComponent * This,
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
    } IComponentVtbl;

    interface IComponent
    {
        CONST_VTBL struct IComponentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComponent_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IComponent_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IComponent_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IComponent_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IComponent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IComponent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IComponent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IComponent_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Component,0xEEF4E51B,0xEDC4,0x49F9,0xB8,0xA2,0xF6,0x01,0xFA,0x92,0x44,0xA0);

#ifdef __cplusplus

class DECLSPEC_UUID("EEF4E51B-EDC4-49F9-B8A2-F601FA9244A0")
Component;
#endif
#endif /* __dx_Interest_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


