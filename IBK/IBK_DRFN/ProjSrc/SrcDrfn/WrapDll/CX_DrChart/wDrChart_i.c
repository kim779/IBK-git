

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for wDrChart.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_WDrChart,0x6790DD3A,0x96F7,0x4CB1,0xA3,0xC8,0x76,0x7F,0x80,0xE2,0x87,0xF3);


MIDL_DEFINE_GUID(IID, DIID_IDrChartCtl,0x75A47147,0x9C93,0x4E1D,0xA7,0xA1,0x25,0xC6,0xFC,0xAE,0x8E,0xE9);


MIDL_DEFINE_GUID(IID, DIID_IDrChartCtlEvents,0x3490D5B1,0x840B,0x474a,0xAF,0xBC,0xBB,0x25,0x78,0xA4,0x73,0x6A);


MIDL_DEFINE_GUID(CLSID, CLSID_DrChartCtl,0xE1B10A45,0x0CE0,0x4682,0x80,0x93,0x27,0x15,0x2F,0xB5,0xEF,0x1A);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



