// ChartAnalysisToolAddIn.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ChartAnalysisToolAddIn.h"

#include "AnalysisAddInImp.h"						// for CAnalysisAddInImp
#include "SymbolAddInImp.h"							// for CSymbolAddInImp
#include "CrosshairsDragAddInImp.h"					// for CCrosshairsDragAddInImp
#include "CrosshairsOverAddInImp.h"					// for CCrosshairsOverAddInImp
#include "CrosshairsToolAddInImp.h"					// for CCrosshairsToolAddInImp
#include "NumericalInquiryDragAddInImp.h"			// for CNumericalInquiryDragAddInImp
#include "NumericalInquiryOverAddInImp.h"			// for CNumericalInquiryOverAddInImp
#include "NumericalInquiryToolAddInImp.h"			// for CNumericalInquiryToolAddInImp
#include "ToolTipAddInImp.h"						// for CToolTipAddInImp

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
// CChartAnalysisToolAddInApp

BEGIN_MESSAGE_MAP(CChartAnalysisToolAddInApp, CWinApp)
	//{{AFX_MSG_MAP(CChartAnalysisToolAddInApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartAnalysisToolAddInApp construction

CChartAnalysisToolAddInApp::CChartAnalysisToolAddInApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChartAnalysisToolAddInApp object

CChartAnalysisToolAddInApp theApp;

// (2004.10.08, ��¿�) Chart AddIn DLL�� Interface Version�� �����ϴ� Macro�� �����Ѵ�.
#define _CHART_ADDIN_IVERSION( nOVersion)										\
	int		g_nChartAddInVersion = 1##nOVersion - 1000000;						\
	char	g_szChartAddInIVersion[ 50] = "ChartAddIn Version : " #nOVersion;
// (2004.10.08, ��¿�) Chart AddIn DLL�� Version�� Static String���� �����Ͽ� DLL Module File�� ��õǵ��� �Ѵ�.
//		���� int�ε� �����Ͽ� GetAddInIVersion���� �̿��� �� �ֵ��� �Ѵ�.
// �ش� site���� Addin Define ���� - ojtaso (20090907)
//#if defined _ADDIN_SK
//_CHART_ADDIN_IVERSION( 913101)
//#elif defined _ADDIN_DB
//_CHART_ADDIN_IVERSION( 423101)
//#elif defined _ADDIN_KC
_CHART_ADDIN_IVERSION( 133101)
//#elif defined _ADDIN_SS
//_CHART_ADDIN_IVERSION( 993101)
//#else
//_CHART_ADDIN_IVERSION( 000000)
//#endif

// [04/10/07] Chart AddIn DLL�� Version�� �˷��ִ� Interface�� �����Ѵ�.
ADDIN_API int GetAddInIVersion(void)
{
	return g_nChartAddInVersion;
}

// [04/10/07] Chart AddIn DLL�� Version�� �˷��ִ� Interface�� �����Ѵ�.
#define GET_ADDIN_OBJECT( CChartAddInImp)															\
	if( !pIChartAddIn)																				\
		if( *( WORD *)( const char *)CChartAddInImp::m_strAddInItemName == wID)						\
			if( CChartAddInImp::m_strAddInItemName == strAddInItem)									\
				pIChartAddIn = new CChartAddInImp( pIChartOCX, p_pIAddInDllBase);

ADDIN_API IChartAddIn *InitAddInDll( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDllBase *p_pIAddInDllBase)
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

	GET_ADDIN_OBJECT( CAnalysisAddInImp);
	GET_ADDIN_OBJECT( CSymbolAddInImp);
	GET_ADDIN_OBJECT( CCrosshairsDragAddInImp);
	GET_ADDIN_OBJECT( CCrosshairsToolAddInImp);
	GET_ADDIN_OBJECT( CCrosshairsOverAddInImp);
	GET_ADDIN_OBJECT( CNumericalInquiryDragAddInImp);
	GET_ADDIN_OBJECT( CNumericalInquiryToolAddInImp);
	GET_ADDIN_OBJECT( CNumericalInquiryOverAddInImp);
	GET_ADDIN_OBJECT( CToolTipAddInImp);

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
