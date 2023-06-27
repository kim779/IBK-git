// IBXXXX99.h : main header file for the IBXXXX99 DLL
//

#if !defined(AFX_IBXXXX99_H__4F418395_13F8_4B4D_8845_F25E58F8BDB0__INCLUDED_)
#define AFX_IBXXXX99_H__4F418395_13F8_4B4D_8845_F25E58F8BDB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIBXXXX99App
// See IBXXXX99.cpp for the implementation of this class
//

class CIBXXXX99App : public CWinApp
{
public:
	CIBXXXX99App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIBXXXX99App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIBXXXX99App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IBXXXX99_H__4F418395_13F8_4B4D_8845_F25E58F8BDB0__INCLUDED_)
