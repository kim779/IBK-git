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
       char    gubn[2];                /* ���� ����                    */
                                       /* MY: ���� ����                */
                                       /* UL: ����� ����Ÿ ���       */
                                       /* UD: ����� ����Ÿ            */
       char    dirt[1];                /* ���� ����                    */
                                       /* U: PC=>HOST  D: HOST=>PC     */
       char    cont[1];                /* ���� ����                    */
                                       /* F: First     M: Middle       */
                                       /* L: Last      C: Cancel       */
				       /* D: delete file               */	
       char    name[80];               /* file name (�ʿ�ø�)         */
       char    nblc[5];                /* �� ���� or                 */
                                       /* ȭ�� SIZE (max. 64K)         */
       char    retc[1];                /* O: ����      E: ����         */
       char    emsg[40];               /* error message                */
};

struct  d_list {                        /* ����� ����Ÿ LIST           */
       char    name[80];               /* file name                    */
       char    size[5];                /* file size                    */
};

#define L_usr	sizeof(struct usrinfo)
#define L_lst	sizeof(struct d_list)