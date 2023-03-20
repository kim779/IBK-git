// ChartGuideLineAddIn.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "./Include_Addin.h"			// for IAddInManager
#include "ChartGuideLineAddInImp.h"

#include "ChartGuideLineAddIn.h"		// for CChartGuideLineAddInImp
#include "SimpleGuideLine.h"			// for CSimpleGuideLine

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
// CChartGuideLineAddInApp

BEGIN_MESSAGE_MAP(CChartGuideLineAddInApp, CWinApp)
	//{{AFX_MSG_MAP(CChartGuideLineAddInApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartGuideLineAddInApp construction

CChartGuideLineAddInApp::CChartGuideLineAddInApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChartGuideLineAddInApp object

CChartGuideLineAddInApp theApp;
HINSTANCE g_hInstance;

#define _CHART_ADDIN_IVERSION( nOVersion)										\
	int		g_nChartAddInVersion = 1##nOVersion - 1000000;						\
	char	g_szChartAddInIVersion[ 50] = "ChartAddIn Version : " #nOVersion;
_CHART_ADDIN_IVERSION( 133721)

ADDIN_API int GetAddInIVersion(void)
{
	return g_nChartAddInVersion;
}

#define GET_ADDIN_OBJECT( CChartAddInImp)															\
	if( !pIChartAddIn)																				\
		if( *( WORD *)( const char *)CChartAddInImp::m_strAddInItemName == wID)						\
			if( CChartAddInImp::m_strAddInItemName == strAddInItem)									\
				pIChartAddIn = new CChartAddInImp( pIChartOCX, p_pIAddInDllBase);


ADDIN_API IChartAddIn *InitAddInDll( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive)
{
	// 0. NULL Pointer�� Ȯ���Ѵ�.
	// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
	if( !p_szAddInItem || !p_pIAddInManager) return NULL;

	// 1. Chart Ocx�� Interface�� ���Ѵ�.
	IChartOCX *pIChartOCX = p_pIAddInManager->GetIChartOCX();
	if( !pIChartOCX) return NULL;

	// 2. Chart Ocx�� Interface�� �̿��Ͽ� AddIn Object�� �����Ѵ�.
	CString strAddInItem( p_szAddInItem);
	strAddInItem.MakeUpper();
	IChartAddIn *pIChartAddIn = NULL;
	WORD wID = *( WORD *)( const char *)strAddInItem;

	GET_ADDIN_OBJECT( CChartGuideLineAddInImp);
	GET_ADDIN_OBJECT( CSimpleGuideLine);

	// 3. Chart Ocx�� Interface�� Release�Ѵ�.
	pIChartOCX->Release();

	if( pIChartAddIn)
	{
		pIChartAddIn->AddRef();

		// (2004.10.13, ��¿�) ������ AddIn Object�� �⺻ Chart Manager���� ����� ��ϵ��� ������,
		//		AddIn ������ ��ҽ�Ų��.
		if( !pIChartAddIn->HasFullManager())
		{
			pIChartAddIn->Release();
			pIChartAddIn = NULL;
		}
	}

	// 4. ������ AddIn Object�� Interface�� Return�Ѵ�.
	return pIChartAddIn;
}

BOOL CChartGuideLineAddInApp::InitInstance() 
{
	g_hInstance = theApp.m_hInstance;	
	
	return CWinApp::InitInstance();
}
