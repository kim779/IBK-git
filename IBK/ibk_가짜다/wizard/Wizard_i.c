

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_WIZARDLib,0x71D618DE,0xD3F1,0x4EB8,0x86,0x2C,0x79,0xD3,0x06,0x06,0x20,0x8F);


MIDL_DEFINE_GUID(IID, DIID__DWizard,0x11B7DB3B,0x047C,0x49D3,0x91,0xFB,0x89,0x4C,0xB1,0xBE,0x9C,0x76);


MIDL_DEFINE_GUID(IID, DIID__DWizardEvents,0x4C00CF12,0x7418,0x4B50,0x8A,0xBF,0x32,0x2D,0x0F,0x61,0xEC,0xFB);


MIDL_DEFINE_GUID(CLSID, CLSID_Wizard,0x7B26DE02,0xE5EA,0x4ED5,0xAA,0x20,0xFB,0x09,0x40,0x62,0x36,0x6C);


MIDL_DEFINE_GUID(IID, DIID_IxScreen,0x2819C94F,0x275F,0x4FE0,0xA2,0x76,0x84,0x56,0x0E,0x42,0x85,0xB5);


MIDL_DEFINE_GUID(CLSID, CLSID_xScreen,0x238D9944,0x44DD,0x42DB,0x86,0xD4,0xDC,0x6F,0xE0,0xDF,0xB3,0x2B);


MIDL_DEFINE_GUID(IID, DIID_IxSystem,0x0325C6BE,0x9B41,0x4C2D,0x97,0x94,0xED,0xD6,0xBE,0x0A,0x30,0x4D);


MIDL_DEFINE_GUID(CLSID, CLSID_xSystem,0x83F7E2C8,0x8CB1,0x4F4A,0xB0,0x39,0xD8,0xBF,0x89,0x00,0xBE,0xE3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



