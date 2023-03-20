// IB100400.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IB100400.h"

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
// CIB100400App

BEGIN_MESSAGE_MAP(CIB100400App, CWinApp)
	//{{AFX_MSG_MAP(CIB100400App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIB100400App construction

CIB100400App::CIB100400App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void CIB100400App::DeleteBufferKey(CString sKey)
{
	m_SaveMapList.RemoveKey(sKey); 
}

void CIB100400App::SaveBuffer(CString sKey, CString sData)
{
	m_SaveMapList.SetAt(sKey, sData); 
}

CString CIB100400App::GetBuffer(CString sKey)
{
	CString sResult = _T("");
	m_SaveMapList.Lookup(sKey, sResult); 

	return sResult;
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CIB100400App object

CIB100400App theApp;

#include "MainWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainWnd *pMainWnd = new CMainWnd(parent);

	if (!pMainWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
						CRect(0, 0, MAP_WIDTH, MAP_HEIGHT), parent, 100, NULL))
	{
		delete pMainWnd;
		return NULL;
	}	
	
	return pMainWnd;
}

int CIB100400App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	m_SaveMapList.RemoveAll(); 
	return CWinApp::ExitInstance();
}
