#if !defined(AFX_CERTIFY_H__9AAA3929_20DC_4AD4_B457_971592632689__INCLUDED_)
#define AFX_CERTIFY_H__9AAA3929_20DC_4AD4_B457_971592632689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Certify.h : main header file for CERTIFY.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

struct	_pwdR {
	char	ret[1];		// return code
				// '0' : success
				// 'X' : stop key in
	char	msg[80];	// message
	char	pwdn[1];	// invalid passwd count
};

#define	checkPass	0
#define	rebootAxis	1
#define	encryptPass	2

/////////////////////////////////////////////////////////////////////////////
// CCertifyApp : See Certify.cpp for implementation.

class CCertifyApp : public COleControlModule
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

#endif // !defined(AFX_CERTIFY_H__9AAA3929_20DC_4AD4_B457_971592632689__INCLUDED)
