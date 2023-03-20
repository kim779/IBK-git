#if !defined(AFX_STDAFX_H__DC3B90BF_A245_4178_8C60_EE04C4E104D1__INCLUDED_)
#define AFX_STDAFX_H__DC3B90BF_A245_4178_8C60_EE04C4E104D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Comon Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>			// MFC database classes
#include <afxdao.h>			// MFC DAO database classes

// (2004.10.08, ��¿�) AddIn DLL���� �����Ǵ� Chart Object��, OCX�� Interface Wrapper Class��
//		Ÿ Site Source�� �����ϱ� ���Ͽ� Macro Name�� �����Ѵ�.
#define MChartOcx	CKTBChartCtrl
// (2004.10.11, ��¿�) AddIn DLL���� �����Ǵ� Chart Object��, OCX�� Interface Wrapper Class��
//		OCX Control Class Accessing�� Ÿ Site�� Source�� �����ϱ� ���Ͽ� Macro Header Path�� �����Ѵ�.
#define MChartOcxInclude	"../KTBChart/OCX2/KTBChartCtl.h"

#pragma warning(disable: 2039)

// type casting�� ���� warning�� ���ش�.
#pragma warning(disable: 4800)

// stl header���� ���� warning�� ���ش�.
#pragma warning(disable: 4786)

//#include "../Include/LoadDRDebuger.h"
#include "../../../SrcDrfn/Inc/LoadDRDebuger.h"
extern CDRDebugerLoader *g_pDRDebuger;
extern CDRDebugerLoader* gfnGetDRDebuger();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DC3B90BF_A245_4178_8C60_EE04C4E104D1__INCLUDED_)
