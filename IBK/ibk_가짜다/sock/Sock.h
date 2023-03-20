#if !defined(AFX_SOCK_H__FB0EC72F_3285_4AA5_A14C_068973850496__INCLUDED_)
#define AFX_SOCK_H__FB0EC72F_3285_4AA5_A14C_068973850496__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Sock.h : main header file for SOCK.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSockApp : See Sock.cpp for implementation.

class CSockApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCK_H__FB0EC72F_3285_4AA5_A14C_068973850496__INCLUDED)
