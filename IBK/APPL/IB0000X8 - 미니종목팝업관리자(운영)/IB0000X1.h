// IB0000X1.h : main header file for the IB0000X1 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB0000X1App
// See IB0000X1.cpp for the implementation of this class
//

class CIB0000X1App : public CWinApp
{
public:
	CIB0000X1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB0000X1App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB0000X1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


