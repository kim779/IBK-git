// cx_SimpleEasy.h : main header file for the CX_SIMPLEEASY DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCx_SimpleEasyApp
// See cx_SimpleEasy.cpp for the implementation of this class
//

class CCx_SimpleEasyApp : public CWinApp
{
public:
	CCx_SimpleEasyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCx_SimpleEasyApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCx_SimpleEasyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};