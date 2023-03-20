// CX_FILEDIALOG.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CX_FILEDIALOG.h"
#include "CtrlWnd.h"

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
// CCX_FILEDIALOGApp

BEGIN_MESSAGE_MAP(CCX_FILEDIALOGApp, CWinApp)
	//{{AFX_MSG_MAP(CCX_FILEDIALOGApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCX_FILEDIALOGApp construction

CCX_FILEDIALOGApp::CCX_FILEDIALOGApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCX_FILEDIALOGApp object

CCX_FILEDIALOGApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCX_FILEDIALOGApp initialization

BOOL CCX_FILEDIALOGApp::InitInstance()
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

__declspec(dllexport) CWnd* APIENTRY axCreate(CWnd* pWizard, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CCtrlWnd* pCtrlWnd = new CCtrlWnd(pWizard, pParam);

	if (!pCtrlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP, pParam->rect, pWizard, 100))
	{
		TRACE("InterestMenu Create Failed\n");
		return 0;
	}

	if (!pCtrlWnd->Initialize(FALSE))
	{
		TRACE("InterestMenu Initialize Failed\n");
		return 0;
	}

	return pCtrlWnd;
}

__declspec(dllexport) CWnd* APIENTRY axCreateDLL(CWnd* pWizard, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CCtrlWnd* pCtrlWnd = new CCtrlWnd(pWizard, pParam);

	if (!pCtrlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP, pParam->rect, pWizard, 100))
	{
		TRACE("InterestMenu Create Failed\n");
		return 0;
	}

	if (!pCtrlWnd->Initialize(TRUE))
	{
		TRACE("InterestMenu Initialize Failed\n");
		return 0;
	}

	return pCtrlWnd;
}
