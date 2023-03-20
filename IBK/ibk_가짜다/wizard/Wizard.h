#if !defined(AFX_WIZARD_H__1AA5FA4A_A05A_47CA_89E3_41B94D10DA01__INCLUDED_)
#define AFX_WIZARD_H__1AA5FA4A_A05A_47CA_89E3_41B94D10DA01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Wizard.h : main header file for WIZARD.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols
#include "Guard.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardApp : See Wizard.cpp for implementation.

class CWizardApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();

	void	SetRegistry(CString keys)	{ 
		CString slog;
		slog.Format("[axwizard][SetRegistry] keys =[%s]\n", keys);
		OutputDebugString(slog);
		SetRegistryKey(keys);
	}
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARD_H__1AA5FA4A_A05A_47CA_89E3_41B94D10DA01__INCLUDED)
