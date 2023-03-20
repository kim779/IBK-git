

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

MIDL_DEFINE_GUID(IID, LIBID_Axislogin,0x74E42E36,0xB966,0x4D43,0x99,0x5A,0xB3,0xC4,0xB9,0x62,0xBA,0xD2);


MIDL_DEFINE_GUID(IID, DIID_ILogin,0xEE69E4E4,0x5BF8,0x4CAD,0x9C,0x8E,0x54,0x25,0x0C,0xF3,0x3A,0x1A);


MIDL_DEFINE_GUID(CLSID, CLSID_Login,0x8BC6BE27,0xEB22,0x4221,0x95,0x6C,0x8F,0xA5,0xBB,0x32,0x22,0x49);


MIDL_DEFINE_GUID(IID, DIID_ILedger,0xF26FD729,0xBC91,0x46EF,0xB4,0xD2,0x6A,0xA9,0xFA,0xAE,0xB0,0x17);


MIDL_DEFINE_GUID(CLSID, CLSID_Ledger,0x5BEC8BAF,0x65AE,0x4B13,0x95,0x7F,0x08,0x3D,0x36,0x45,0xF8,0x79);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



