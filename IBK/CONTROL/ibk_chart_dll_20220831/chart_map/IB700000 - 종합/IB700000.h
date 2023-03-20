// IB700000.h : main header file for the IB700000 DLL
//

#if !defined(AFX_IB700000_H__EA3EC5C0_2122_48C3_A5C9_4475694821A7__INCLUDED_)
#define AFX_IB700000_H__EA3EC5C0_2122_48C3_A5C9_4475694821A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB700000App
// See IB700000.cpp for the implementation of this class
//

class CIB700000App : public CWinApp
{
public:
	CIB700000App();
	~CIB700000App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB700000App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB700000App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	HINSTANCE	m_hView;

	CWnd* Create(CWnd*);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IB700000_H__EA3EC5C0_2122_48C3_A5C9_4475694821A7__INCLUDED_)
