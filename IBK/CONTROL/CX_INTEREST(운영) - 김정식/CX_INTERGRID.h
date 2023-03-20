// CX_INTERGRID.h : main header file for the CX_INTERGRID DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#include "../../h/axiscgrp.h"

/////////////////////////////////////////////////////////////////////////////
// CCX_INTERGRIDApp
// See CX_INTERGRID.cpp for the implementation of this class
//

class CCX_INTERGRIDApp : public CWinApp
{
public:
	CCX_INTERGRIDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_INTERGRIDApp)
	public:
	virtual BOOL InitInstance();
	virtual int	ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_INTERGRIDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};