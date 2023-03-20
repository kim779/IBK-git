// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>            // MFC socket extensions
#include <afxcontrolbars.h>

struct  usrinfo {
       char    gubn[2];                /* 업무 구분                    */
                                       /* MY: 관심 종목                */
                                       /* UL: 사용자 데이타 목록       */
                                       /* UD: 사용자 데이타            */
       char    dirt[1];                /* 전송 방향                    */
                                       /* U: PC=>HOST  D: HOST=>PC     */
       char    cont[1];                /* 연속 구분                    */
                                       /* F: First     M: Middle       */
                                       /* L: Last      C: Cancel       */
				       /* D: delete file               */	
       char    name[80];               /* file name (필요시만)         */
       char    nblc[5];                /* 블럭 갯수 or                 */
                                       /* 화일 SIZE (max. 64K)         */
       char    retc[1];                /* O: 정상      E: 에러         */
       char    emsg[40];               /* error message                */
};

struct  d_list {                        /* 사용자 데이타 LIST           */
       char    name[80];               /* file name                    */
       char    size[5];                /* file size                    */
};

#define L_usr	sizeof(struct usrinfo)
#define L_lst	sizeof(struct d_list)