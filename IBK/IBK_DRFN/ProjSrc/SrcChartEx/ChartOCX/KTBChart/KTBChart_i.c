

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for KTBChart.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_IBKChartLib,0xD64827B8,0x55F0,0x4399,0xBE,0xDD,0x0D,0x2A,0xD9,0xC6,0x54,0x51);


MIDL_DEFINE_GUID(IID, DIID__DIBKChart,0x5E2211A8,0x2AFB,0x4338,0xB4,0x0D,0xE9,0x00,0x73,0x0A,0x9D,0xDF);


MIDL_DEFINE_GUID(IID, DIID__DIBKChartEvents,0x6390F0D0,0x6F41,0x4492,0x8B,0x66,0x50,0x22,0xD0,0x5B,0x8E,0xF8);


MIDL_DEFINE_GUID(CLSID, CLSID_IBKChart,0xB98261E1,0xB73D,0x4bb1,0x90,0xD6,0xE6,0xC8,0xBA,0xAE,0xEB,0xD5);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



