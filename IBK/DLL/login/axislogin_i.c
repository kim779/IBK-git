

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, LIBID_Axislogin,0x867AB9FE,0x413D,0x48A7,0x9D,0x20,0xA4,0x0D,0xB2,0xD1,0x77,0x7B);


MIDL_DEFINE_GUID(IID, DIID_ILogin,0xA1E4E0F4,0xC347,0x450D,0xB0,0xEE,0x47,0x62,0x26,0x38,0xD3,0x9D);


MIDL_DEFINE_GUID(CLSID, CLSID_Login,0x6575B9CC,0x0360,0x49B8,0xAB,0x26,0x26,0x21,0x10,0x96,0x21,0x7C);


MIDL_DEFINE_GUID(IID, DIID_ILedger,0x613E41B4,0x8728,0x49E8,0x87,0x11,0xFB,0xE9,0x2D,0x8D,0xB9,0x38);


MIDL_DEFINE_GUID(CLSID, CLSID_Ledger,0xD4DDFCF5,0xFFF9,0x42BB,0xAC,0x87,0x9E,0x15,0xEB,0x6F,0x80,0xF1);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



