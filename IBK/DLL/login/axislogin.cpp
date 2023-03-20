// axislogin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "axislogin.h"
#include "axislogin.h"
#include <winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CAxisloginApp

BEGIN_MESSAGE_MAP(CAxisloginApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisloginApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisloginApp construction

CAxisloginApp::CAxisloginApp()
{
	m_login = NULL;

	WORD	wVersionRequested;
	WSADATA	wsaData;
	char	wb[128];
	PHOSTENT hostinfo;

	m_host = _T("");
	m_user = _T("");
	m_term = _T("");
	m_brno = _T("");
	m_cogb = _T("");

	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (LOBYTE(wsaData.wVersion) == 2 || HIBYTE(wsaData.wVersion) == 2)
		{
			if (gethostname(wb, sizeof(wb)) != SOCKET_ERROR)
			{
				if ((hostinfo = gethostbyname(wb)) != NULL)
					m_host = CString(inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));
			}
		}
		WSACleanup( );
	}

	if (!m_host.IsEmpty())
	{
		int	idx;
		CString	tmps, text;

		text = m_host;
		m_host.Empty();
		for (int ii = 0; ii < 4; ii++)
		{
			idx = text.Find('.');
			if (idx != -1)
			{
				tmps.Format("%03d", atoi(text.Left(idx)));
				text = text.Mid(++idx);
			}
			else
			{
				tmps.Format("%03d", atoi(text));
				text.Empty();
			}
			m_host += tmps;
		}
	}
}

CAxisloginApp::~CAxisloginApp()
{
	if (m_login)
		delete m_login;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisloginApp object

CAxisloginApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxisloginApp initialization

BOOL CAxisloginApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}
