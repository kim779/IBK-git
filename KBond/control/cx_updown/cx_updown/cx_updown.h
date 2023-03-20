// cx_updown.h : main header file for the cx_updown DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ccx_updownApp
// See cx_updown.cpp for the implementation of this class
//

class Ccx_updownApp : public CWinApp
{
public:
	Ccx_updownApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
