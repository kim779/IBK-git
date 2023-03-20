

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, LIBID_Cx_SimpleEasy,0x26637FAF,0x9F12,0x40C6,0xB6,0xE3,0x90,0xDB,0x82,0x93,0x86,0xF5);


MIDL_DEFINE_GUID(IID, DIID_IDlgQr,0xD3BD38F3,0x216F,0x477C,0x8B,0xC7,0xEB,0x07,0x41,0x2C,0xB9,0xB9);


MIDL_DEFINE_GUID(CLSID, CLSID_DlgQr,0x9A7384D3,0x4783,0x4DB0,0xA1,0x0D,0x71,0x71,0x60,0x8E,0x4C,0x1A);


MIDL_DEFINE_GUID(IID, DIID_IMainWnd,0xE046FA1A,0x7D7E,0x48A9,0x88,0xD9,0xDC,0xF3,0x17,0xAC,0x1C,0x76);


MIDL_DEFINE_GUID(CLSID, CLSID_MainWnd,0x6CB2C0F2,0x5594,0x452D,0x98,0x9E,0x18,0x35,0x02,0x46,0x2C,0xF5);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



