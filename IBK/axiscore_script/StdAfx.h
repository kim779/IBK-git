// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7313C33A_F72C_481F_9E7D_CCF762597E05__INCLUDED_)
#define AFX_STDAFX_H__7313C33A_F72C_481F_9E7D_CCF762597E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#define DF_UPDATE				"IBKS_Update.exe"
#define DF_SUPDATE				"IBKS_SUpdate.exe"
#define DF_UPDATE_VC			"VC_redist.x86.exe"
#define DF_Win7_64				"windows6.1-KB976932-X64.exe"
#define DF_Win7_86				"windows6.1-KB976932-X86.exe"

#define DF_URL_DOWN				"https://www.ibks.com/admin/common/download.jsp?filepath=/files/hts&filename=IBKS_Update.exe"
#define DF_URL_STAFF_DOWN		"https://www.ibks.com/admin/common/download.jsp?filepath=/files/hts&filename=IBKS_SUpdate.exe"

#define DF_URL_Win7_64			"https://www.ibks.com/admin/common/download.jsp?filepath=/files/hts&filename=windows6.1-KB976932-X64.exe"
#define DF_URL_Win7_86			"https://www.ibks.com/admin/common/download.jsp?filepath=/files/hts&filename=windows6.1-KB976932-X86.exe"

#define DF_URL_DEVSTAFF_DOWN    "https://www.ibks.com/admin/common/download.jsp?filepath=/files/hts&filename=IBKS_SDUpdate.exe"
							 
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7313C33A_F72C_481F_9E7D_CCF762597E05__INCLUDED_)
