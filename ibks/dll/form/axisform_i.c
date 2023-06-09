

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for axisform.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_Axisform,0xFB34F483,0x60AC,0x4585,0xA2,0xD8,0x71,0x1C,0xC8,0x9D,0x90,0x49);


MIDL_DEFINE_GUID(IID, DIID_IAxisForm,0x1AED3C4B,0x5A8A,0x48F3,0x82,0x1D,0xB0,0xE0,0xC6,0xC8,0x2D,0x3F);


MIDL_DEFINE_GUID(CLSID, CLSID_AxisForm,0x3E389F8A,0x3C8B,0x4E82,0xB5,0xB3,0x8B,0x98,0x9B,0xC8,0x78,0x5F);


MIDL_DEFINE_GUID(IID, DIID_IfmAvi,0xC7240887,0x09C7,0x472F,0xAA,0xB4,0x51,0x98,0xC2,0x0B,0x1A,0x01);


MIDL_DEFINE_GUID(CLSID, CLSID_fmAvi,0xE5C415C3,0xDC41,0x4015,0x92,0x08,0x7D,0x85,0xB9,0x01,0x26,0xC7);


MIDL_DEFINE_GUID(IID, DIID_IfmBase,0x6E6307E3,0x9BA5,0x489A,0xB7,0xA2,0xB6,0xF4,0xAE,0x0F,0x37,0x52);


MIDL_DEFINE_GUID(CLSID, CLSID_fmBase,0x1B43B685,0x9F9B,0x446E,0xA7,0x9A,0xA1,0x35,0xA9,0xCB,0xBB,0x22);


MIDL_DEFINE_GUID(IID, DIID_IfmBox,0xD012FFFE,0x4679,0x4802,0xB5,0x8A,0xDD,0x30,0x14,0xA0,0x71,0x86);


MIDL_DEFINE_GUID(CLSID, CLSID_fmBox,0xA3406029,0x876E,0x4D7E,0x97,0x1E,0xCF,0x4A,0x3C,0x6A,0xBE,0x74);


MIDL_DEFINE_GUID(IID, DIID_IfmBrowser,0x3BEC7DED,0x7805,0x4A75,0x90,0x68,0x1F,0x9D,0xC0,0x80,0xD8,0x3E);


MIDL_DEFINE_GUID(CLSID, CLSID_fmBrowser,0x2389DA5C,0x3CC1,0x4860,0xAA,0xA1,0x1E,0x99,0x77,0x1A,0x1A,0xC2);


MIDL_DEFINE_GUID(IID, DIID_IfmButton,0x6ED765AC,0x16A4,0x4038,0xA1,0x81,0x7D,0x5D,0x0C,0x00,0xFD,0x3A);


MIDL_DEFINE_GUID(CLSID, CLSID_fmButton,0xD3D48E2D,0xED6F,0x4BE8,0xAB,0x9A,0x0E,0x8E,0x35,0xD3,0x1F,0x36);


MIDL_DEFINE_GUID(IID, DIID_IfmCheck,0x7E2C96E8,0xFBD9,0x4332,0x83,0x1A,0x8F,0xC5,0x0D,0x65,0x55,0x4B);


MIDL_DEFINE_GUID(CLSID, CLSID_fmCheck,0xDE98FF5A,0xC342,0x472F,0x8C,0x00,0x19,0x39,0x92,0xCD,0x37,0xF1);


MIDL_DEFINE_GUID(IID, DIID_IfmCombo,0x27F81344,0x9295,0x457A,0xB5,0xFD,0x10,0xE3,0x4D,0x2D,0xE2,0x5C);


MIDL_DEFINE_GUID(CLSID, CLSID_fmCombo,0xB8FBE3AE,0xB5EF,0x45A7,0x91,0xB7,0x66,0x7D,0x49,0x2B,0x30,0x34);


MIDL_DEFINE_GUID(IID, DIID_IfmEdit,0xCD6C18AB,0xF621,0x4BFB,0xA3,0x7D,0x6D,0x2D,0x80,0xD3,0x1B,0xAA);


MIDL_DEFINE_GUID(CLSID, CLSID_fmEdit,0xC0DA2002,0xE373,0x4CC3,0x8D,0x00,0xD6,0x34,0x23,0xFE,0xD2,0xC6);


MIDL_DEFINE_GUID(IID, DIID_IfmEditEx,0xCA3A7032,0x2513,0x44B9,0x85,0xBB,0xE1,0x00,0x6A,0x5F,0xC4,0xBE);


MIDL_DEFINE_GUID(CLSID, CLSID_fmEditEx,0x06E81FC9,0x0C6D,0x406F,0xA7,0xB0,0x61,0xD9,0x9B,0xCC,0x94,0xE5);


MIDL_DEFINE_GUID(IID, DIID_IfmGrid,0xED8E198C,0x161B,0x401A,0x87,0xD0,0x2E,0x46,0x27,0xE2,0xEF,0x96);


MIDL_DEFINE_GUID(CLSID, CLSID_fmGrid,0x931B6CC4,0xF888,0x4720,0x84,0xDE,0x0B,0x48,0x2B,0x76,0x70,0xF0);


MIDL_DEFINE_GUID(IID, DIID_IfmGroup,0x0AE7F991,0x14C7,0x43C3,0x99,0xB1,0x46,0x75,0xDA,0xC7,0x48,0x2B);


MIDL_DEFINE_GUID(CLSID, CLSID_fmGroup,0xDE6BF64F,0x33D7,0x45BA,0x80,0x31,0x10,0x30,0x99,0x81,0x84,0x0D);


MIDL_DEFINE_GUID(IID, DIID_IfmLabel,0x4846F8F7,0x758E,0x4CF7,0x87,0x0F,0xD1,0xAE,0x21,0x8F,0x75,0xB1);


MIDL_DEFINE_GUID(CLSID, CLSID_fmLabel,0x59AA397A,0xB4E7,0x4305,0x97,0x8F,0xD4,0x12,0xFC,0x0F,0x5E,0x41);


MIDL_DEFINE_GUID(IID, DIID_IfmMemo,0x8B328127,0x73D1,0x4510,0xA6,0x76,0xB2,0xF5,0x6B,0x24,0xC3,0x51);


MIDL_DEFINE_GUID(CLSID, CLSID_fmMemo,0x70A38E58,0xD3A5,0x4741,0x80,0x61,0xD5,0x76,0x1E,0xEB,0xC2,0x5B);


MIDL_DEFINE_GUID(IID, DIID_IfmObject,0x45FDC0D0,0xA48C,0x403C,0xAD,0x72,0xE3,0x83,0xFC,0xCD,0x78,0x4B);


MIDL_DEFINE_GUID(CLSID, CLSID_fmObject,0xF806408F,0x3F0E,0x477D,0x99,0x34,0x81,0x80,0xD1,0xF0,0x3E,0xBF);


MIDL_DEFINE_GUID(IID, DIID_IfmOut,0xCCBF17FF,0xB2D8,0x4122,0xA2,0xD1,0x80,0xFD,0xD7,0xB5,0xBA,0x8C);


MIDL_DEFINE_GUID(CLSID, CLSID_fmOut,0xA8996EFA,0xF4B6,0x4570,0xBD,0x8F,0xBF,0xD7,0xB3,0x0B,0xE8,0x18);


MIDL_DEFINE_GUID(IID, DIID_IfmPanel,0xF7303843,0x6D61,0x43A1,0xBE,0xB7,0xC0,0x3A,0x14,0x1B,0xC8,0x4B);


MIDL_DEFINE_GUID(CLSID, CLSID_fmPanel,0x9003C204,0x7A44,0x4402,0x9A,0x9D,0xBA,0xC8,0xBE,0x04,0x90,0x2C);


MIDL_DEFINE_GUID(IID, DIID_IfmRadio,0x233923D8,0xD9A5,0x4F28,0xAF,0xE2,0xAB,0xB4,0x81,0x96,0x03,0xBD);


MIDL_DEFINE_GUID(CLSID, CLSID_fmRadio,0x24327C60,0xD2E2,0x4173,0xB9,0x7E,0x4F,0x70,0xB6,0x51,0xA6,0xCE);


MIDL_DEFINE_GUID(IID, DIID_IfmSheet,0x59E54F8E,0xA9E5,0x4799,0x8F,0x09,0x79,0xFE,0x7B,0x3F,0x99,0x9A);


MIDL_DEFINE_GUID(CLSID, CLSID_fmSheet,0xB2D6D863,0x086A,0x4E32,0x9B,0x11,0x30,0xC0,0xB6,0x67,0x5D,0x24);


MIDL_DEFINE_GUID(IID, DIID_IfmSysm,0x156A22B2,0xE279,0x4A7E,0xAC,0x31,0x95,0xF0,0xE3,0x0D,0xBB,0x1B);


MIDL_DEFINE_GUID(CLSID, CLSID_fmSysm,0x5187066F,0x8B3C,0x41FA,0x88,0x20,0x73,0x4E,0x84,0x3D,0x90,0x47);


MIDL_DEFINE_GUID(IID, DIID_IfmTab,0x299E30A5,0x4A51,0x412C,0xB8,0x3A,0xDE,0x20,0x95,0xDB,0x2B,0xE6);


MIDL_DEFINE_GUID(CLSID, CLSID_fmTab,0xFC57D917,0x0FD2,0x41C0,0x8A,0x5C,0x4D,0xF8,0xB0,0x2E,0x0D,0x21);


MIDL_DEFINE_GUID(IID, DIID_IfmTable,0xFC28F962,0x0228,0x436F,0x86,0x59,0xB3,0x3D,0x87,0xF2,0x56,0x90);


MIDL_DEFINE_GUID(CLSID, CLSID_fmTable,0x64C80050,0x9551,0x4C7D,0xB8,0xA3,0x33,0xCB,0x38,0xAB,0x81,0xCE);


MIDL_DEFINE_GUID(IID, DIID_IfmTreeView,0x890032BC,0x3F92,0x434A,0xB5,0x53,0x83,0x53,0xCC,0xD9,0x6F,0xBD);


MIDL_DEFINE_GUID(CLSID, CLSID_fmTreeView,0x30C8755B,0xCDE0,0x4491,0xB6,0xC7,0xAE,0xB5,0xB2,0x08,0x4E,0x2C);


MIDL_DEFINE_GUID(IID, DIID_IfmUserTab,0x7BDDAA17,0x02D4,0x405B,0xAF,0xF2,0xA1,0xBD,0x45,0x0A,0xDA,0x24);


MIDL_DEFINE_GUID(CLSID, CLSID_fmUserTab,0xADA5BB3E,0x584E,0x4EFE,0x9F,0x52,0xC7,0x1F,0xDD,0x54,0x6A,0xDA);


MIDL_DEFINE_GUID(IID, DIID_IfmCtrl,0xB625BCF2,0x8767,0x4EE7,0xA4,0x57,0x77,0xB6,0xBF,0x36,0x3B,0xB7);


MIDL_DEFINE_GUID(CLSID, CLSID_fmCtrl,0x3FA1F829,0x3E10,0x4D45,0xAB,0x3B,0x3D,0xDA,0x97,0xAA,0xD7,0x63);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



