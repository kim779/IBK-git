// CX_FILEDIALOG.h : main header file for the CX_FILEDIALOG DLL
//

#if !defined(AFX_CX_FILEDIALOG_H__DCD10492_98FA_44A6_A50B_70AE3170CB0E__INCLUDED_)
#define AFX_CX_FILEDIALOG_H__DCD10492_98FA_44A6_A50B_70AE3170CB0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_FILEDIALOGApp
// See CX_FILEDIALOG.cpp for the implementation of this class
//

class CCX_FILEDIALOGApp : public CWinApp
{
public:
	CCX_FILEDIALOGApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_FILEDIALOGApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_FILEDIALOGApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_FILEDIALOG_H__DCD10492_98FA_44A6_A50B_70AE3170CB0E__INCLUDED_)
