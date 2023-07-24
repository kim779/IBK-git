

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for axislogin.odl:
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


#ifndef __axislogin_h_h__
#define __axislogin_h_h__

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

#ifndef __ILogin_FWD_DEFINED__
#define __ILogin_FWD_DEFINED__
typedef interface ILogin ILogin;

#endif 	/* __ILogin_FWD_DEFINED__ */


#ifndef __Login_FWD_DEFINED__
#define __Login_FWD_DEFINED__

#ifdef __cplusplus
typedef class Login Login;
#else
typedef struct Login Login;
#endif /* __cplusplus */

#endif 	/* __Login_FWD_DEFINED__ */


#ifndef __ILedger_FWD_DEFINED__
#define __ILedger_FWD_DEFINED__
typedef interface ILedger ILedger;

#endif 	/* __ILedger_FWD_DEFINED__ */


#ifndef __Ledger_FWD_DEFINED__
#define __Ledger_FWD_DEFINED__

#ifdef __cplusplus
typedef class Ledger Ledger;
#else
typedef struct Ledger Ledger;
#endif /* __cplusplus */

#endif 	/* __Ledger_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Axislogin_LIBRARY_DEFINED__
#define __Axislogin_LIBRARY_DEFINED__

/* library Axislogin */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Axislogin,0x867AB9FE,0x413D,0x48A7,0x9D,0x20,0xA4,0x0D,0xB2,0xD1,0x77,0x7B);

#ifndef __ILogin_DISPINTERFACE_DEFINED__
#define __ILogin_DISPINTERFACE_DEFINED__

/* dispinterface ILogin */
/* [uuid] */ 


DEFINE_GUID(DIID_ILogin,0xA1E4E0F4,0xC347,0x450D,0xB0,0xEE,0x47,0x62,0x26,0x38,0xD3,0x9D);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A1E4E0F4-C347-450D-B0EE-47622638D39D")
    ILogin : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ILoginVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogin * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogin * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogin * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILogin * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILogin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILogin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILogin * This,
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
    } ILoginVtbl;

    interface ILogin
    {
        CONST_VTBL struct ILoginVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogin_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogin_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogin_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogin_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILogin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILogin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILogin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ILogin_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Login,0x6575B9CC,0x0360,0x49B8,0xAB,0x26,0x26,0x21,0x10,0x96,0x21,0x7C);

#ifdef __cplusplus

class DECLSPEC_UUID("6575B9CC-0360-49B8-AB26-26211096217C")
Login;
#endif

#ifndef __ILedger_DISPINTERFACE_DEFINED__
#define __ILedger_DISPINTERFACE_DEFINED__

/* dispinterface ILedger */
/* [uuid] */ 


DEFINE_GUID(DIID_ILedger,0x613E41B4,0x8728,0x49E8,0x87,0x11,0xFB,0xE9,0x2D,0x8D,0xB9,0x38);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("613E41B4-8728-49E8-8711-FBE92D8DB938")
    ILedger : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ILedgerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILedger * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILedger * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILedger * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILedger * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILedger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILedger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILedger * This,
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
    } ILedgerVtbl;

    interface ILedger
    {
        CONST_VTBL struct ILedgerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILedger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILedger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILedger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILedger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILedger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILedger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILedger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ILedger_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Ledger,0xD4DDFCF5,0xFFF9,0x42BB,0xAC,0x87,0x9E,0x15,0xEB,0x6F,0x80,0xF1);

#ifdef __cplusplus

class DECLSPEC_UUID("D4DDFCF5-FFF9-42BB-AC87-9E15EB6F80F1")
Ledger;
#endif
#endif /* __Axislogin_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


