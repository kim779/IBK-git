

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Sock.odl:
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


#ifndef __Sock_h_h__
#define __Sock_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSock_FWD_DEFINED__
#define ___DSock_FWD_DEFINED__
typedef interface _DSock _DSock;

#endif 	/* ___DSock_FWD_DEFINED__ */


#ifndef ___DSockEvents_FWD_DEFINED__
#define ___DSockEvents_FWD_DEFINED__
typedef interface _DSockEvents _DSockEvents;

#endif 	/* ___DSockEvents_FWD_DEFINED__ */


#ifndef __Sock_FWD_DEFINED__
#define __Sock_FWD_DEFINED__

#ifdef __cplusplus
typedef class Sock Sock;
#else
typedef struct Sock Sock;
#endif /* __cplusplus */

#endif 	/* __Sock_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SOCKLib_LIBRARY_DEFINED__
#define __SOCKLib_LIBRARY_DEFINED__

/* library SOCKLib */
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

DEFINE_GUID(LIBID_SOCKLib,0xE1C627D8,0x0777,0x4315,0x92,0x4C,0x0B,0xBB,0x53,0xDF,0x83,0x3C);

#ifndef ___DSock_DISPINTERFACE_DEFINED__
#define ___DSock_DISPINTERFACE_DEFINED__

/* dispinterface _DSock */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DSock,0xFE099FDD,0x53B7,0x43BB,0xA6,0x89,0x30,0xB2,0x73,0xC4,0x55,0x19);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FE099FDD-53B7-43BB-A689-30B273C45519")
    _DSock : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSockVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSock * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSock * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSock * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSock * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSock * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSock * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSock * This,
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
    } _DSockVtbl;

    interface _DSock
    {
        CONST_VTBL struct _DSockVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSock_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSock_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSock_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSock_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSock_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSock_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSock_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSock_DISPINTERFACE_DEFINED__ */


#ifndef ___DSockEvents_DISPINTERFACE_DEFINED__
#define ___DSockEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DSockEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DSockEvents,0xB284D742,0x9317,0x4A57,0x82,0x73,0x9F,0x82,0xEB,0x69,0xB2,0xD3);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B284D742-9317-4A57-8273-9F82EB69B2D3")
    _DSockEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSockEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSockEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSockEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSockEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSockEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSockEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSockEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSockEvents * This,
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
    } _DSockEventsVtbl;

    interface _DSockEvents
    {
        CONST_VTBL struct _DSockEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSockEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSockEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSockEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSockEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSockEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSockEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSockEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSockEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Sock,0xB89D8EB8,0x316D,0x49EE,0xB3,0xA7,0x4C,0xB0,0x86,0xD1,0x85,0xE7);

#ifdef __cplusplus

class DECLSPEC_UUID("B89D8EB8-316D-49EE-B3A7-4CB086D185E7")
Sock;
#endif
#endif /* __SOCKLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


