
#pragma once

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

#define castDVAL(x) static_cast<double>(x)
#define castIVAL(x) static_cast<int>(x)
#include "../../../../H/axislog.h"
//#include <vadefs.h>
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