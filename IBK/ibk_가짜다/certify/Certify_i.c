

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Certify.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_CERTIFYLib,0x4EC81D5B,0x30E3,0x43C2,0xA6,0x76,0x4A,0xAD,0x73,0xF5,0xD0,0x90);


MIDL_DEFINE_GUID(IID, DIID__DCertify,0x240CE94C,0x9426,0x4C02,0xBA,0x78,0xCB,0xB4,0x38,0x5B,0xDE,0x8E);


MIDL_DEFINE_GUID(IID, DIID__DCertifyEvents,0xF976A329,0x037A,0x407A,0xA0,0x73,0x38,0xE2,0xB5,0x0B,0x61,0x4D);


MIDL_DEFINE_GUID(CLSID, CLSID_Certify,0x5D693FFF,0x52BF,0x4DDA,0x85,0xA6,0x14,0x29,0x88,0x7D,0x20,0xAB);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



