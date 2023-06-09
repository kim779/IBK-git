// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8F8686CB_BC5B_4B7C_A662_FC82ECAC8FA5__INCLUDED_)
#define AFX_STDAFX_H__8F8686CB_BC5B_4B7C_A662_FC82ECAC8FA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../mxtrace.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8F8686CB_BC5B_4B7C_A662_FC82ECAC8FA5__INCLUDED_)
#include "../../../../H/axislog.h"
//#include <vadefs.h>
//
//static CString Int2CString(CString stmp, int ival)
//{
//	CString slog;
//	slog.Format("[%s = %d]", stmp, ival);
//	return slog;
//}
//
//static void LOG_OUTP(int scnt, ...)
//{
//	va_list argList;
//	va_start(argList, scnt);
//
//	CString sTmp, sResult;
//
//	for (int i = 0; i < scnt; i++)
//	{
//		if (i == 0)
//			sTmp.Format("<%s>", va_arg(argList, LPCTSTR));
//		else
//			sTmp.Format("[%s]", va_arg(argList, LPCTSTR));
//
//		sResult += sTmp;
//		if (i == 0)
//			sResult += "   ";
//		else
//			sResult += " ";
//	}
//	va_end(argList);
//
//	OutputDebugString("\r\n" + sResult);
//}