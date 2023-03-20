// SignalSearch.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SignalSearch.h"
#include "ScreenWnd.h"

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
// CSignalSearchApp

BEGIN_MESSAGE_MAP(CSignalSearchApp, CWinApp)
	//{{AFX_MSG_MAP(CSignalSearchApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSignalSearchApp construction

CSignalSearchApp::CSignalSearchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSignalSearchApp object

CSignalSearchApp theApp;
HINSTANCE g_hInstance;

#include "../../inc/ISystemUtilMng.h"
BOOL gDOChartOCXRegister()
{
	//�ڵ������ ������ ������ �����ؿ�.
	BOOL bReg = (BOOL)AfxGetApp()->GetProfileInt("STBasic", "ChartRegist", 1);
	if(bReg==0) return TRUE;
	
	CString szDir, szCFG;
	GetDir_DrfnData(szDir);
	szCFG = szDir + "\\chartmetatable.dat";
	char aProgID[100]={0,}, aOcxName[100]={0,};
	if(::GetPrivateProfileString("Chart Version", "ProgID", "", aProgID, 100, szCFG)==0)
		return FALSE;
	
	if(::GetPrivateProfileString("Chart Version", "ProgName", "", aOcxName, 100, szCFG)==0)
		return FALSE;
	
	CLSID clsid;
	ISystemUtilManager2* pMng = (ISystemUtilManager2*)AfxGetPctrInterface(UUID_ISystemUtilManager);
	if(pMng)
	{
		GetDir_DrfnBin(szDir);		
		CString szOcxName;
		szOcxName = szDir + "\\" + aOcxName;
		
		if(pMng->IsRegisteredEx(aProgID, clsid, aOcxName, szOcxName)==FALSE)
		{			
			pMng->DLLRegister(szOcxName);
		}
		return pMng->IsRegistered(aProgID, clsid);
	}
	return TRUE;
}

BOOL CSignalSearchApp::InitInstance() 
{
	g_hInstance = theApp.m_hInstance;
	gDOChartOCXRegister();
	
	return CWinApp::InitInstance();
}

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

extern "C" __declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);

	//20120112 �̹��� ���ҽ�üũ����߰� >>
	BOOL bResult	= CUtils::CheckResource();
	if(!bResult)	return FALSE;
	//20120112 �̹��� <<

	CScreenWnd* pWnd = new CScreenWnd(parent);
	if(pWnd->Create(NULL, "SIGNALSEARCH", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), parent, 101) == FALSE)
		return NULL;

	AfxSetResourceHandle(hOld);
	
	return pWnd;
}