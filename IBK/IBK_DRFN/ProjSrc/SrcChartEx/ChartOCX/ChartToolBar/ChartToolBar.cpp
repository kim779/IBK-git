// ChartToolBar.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ChartToolBar.h"

#include "../Include_Chart/Dll_Load/ToolBarManager.h"	// for CToolBarManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/BlockBaseEnum.h"				// for CToolOptionInfo
#include "Definition.h"									// for TB_BUTTONPARAM_LEFTSCROLL

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
// CChartToolBarApp

BEGIN_MESSAGE_MAP(CChartToolBarApp, CWinApp)
	//{{AFX_MSG_MAP(CChartToolBarApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartToolBarApp construction

CChartToolBarApp::CChartToolBarApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChartToolBarApp object

CChartToolBarApp theApp;


/*-----------------------------------------------------------------------------------------
 - Function    :  IsToggleButton
 - Created at  :  2004-11-09   09:46
 - Author      :  ������
 - Description :  ��۹�ư���� ����۹�ư������ �ľ��Ѵ�.
 - Update	   :  ����(05/04/14) �ڵ���ũ�ѹ�ư�� ��۹�ư���� �߰��Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool IsToggleButton(CToolOptionInfo::TOOLOPTION enumToolOption, const CString &p_strParam)
{
	switch(enumToolOption) 
	{
		case CToolOptionInfo::T_WHOLEVIEW:					// ��ü����
		case CToolOptionInfo::T_NIDLG_DRAG:					// ��ġ��ȸâ	// (2006/6/28 - Seung-Won, Bae) Rename for Actual Function with Old Map Usage.
		case CToolOptionInfo::T_CROSSHAIRS_DRAG:			// ���ڼ�
		case CToolOptionInfo::T_RADAR:						// ���̴� ��Ʈ
		case CToolOptionInfo::T_PATTERN:					// ���Ϻм�
		case CToolOptionInfo::T_MAGNIFYINGGLASS:			// ������
		case CToolOptionInfo::T_SHOW_OUTERTOOLBAR:			// �ܺ����� ���̱�
		case CToolOptionInfo::T_MINMAX_FULLDATA:			// ��ü �ִ� �ּ� ����
		case CToolOptionInfo::T_CHART_RAINBOW:				// ���κ���
		case CToolOptionInfo::T_CHART_BALANCE:				// �ϸ����ǥ
		case CToolOptionInfo::T_CHART_PANDF:				// P&F
		case CToolOptionInfo::T_CHART_THREELINEBREAK:		// �Ｑ��ȯ��
		case CToolOptionInfo::T_CHART_REVERSE:				// ���ð�
		case CToolOptionInfo::T_CHART_BARFORSALE:			// ���Ź�
		case CToolOptionInfo::T_SYMBOL_BULET:				// ��ȣDlg 
		case CToolOptionInfo::T_SYMBOL_SQUARE:				// �簢����
		case CToolOptionInfo::T_SYMBOL_CIRCLE:				// ��������
		case CToolOptionInfo::T_SYMBOL_TRIANGLE:			// �ﰢ��
		case CToolOptionInfo::T_SYMBOL_TEXT:				// ����
		case CToolOptionInfo::T_TOOL_LINE_PROFIT:			// �߼��� ���ͷ�
		case CToolOptionInfo::T_TOOL_LINE:					// �߼���
		case CToolOptionInfo::T_TOOL_LINE_CROSS:			// ���ڼ�
		case CToolOptionInfo::T_TOOL_LINE_VERT:				// ������
		case CToolOptionInfo::T_TOOL_LINE_HORZ:				// ����
		case CToolOptionInfo::T_TOOL_LINE_TRISECT:			// ���м�
		case CToolOptionInfo::T_TOOL_LINE_QUARTER:			// ���м�
		case CToolOptionInfo::T_TOOL_LINE_RESISTANCE:		// �������׼�
		case CToolOptionInfo::T_TOOL_FIBONACCI_ARC:			// �Ǻ���ġ �ݿ�
		case CToolOptionInfo::T_TOOL_FIBO_CIRCLE:			// �Ǻ���ġ ��
		case CToolOptionInfo::T_TOOL_FIBONACCI_FAN:			// �Ǻ���ġ �Ҷ���
		case CToolOptionInfo::T_TOOL_FIBO_RETRACE:			// �Ǻ���ġ �ǵ���(������)
		case CToolOptionInfo::T_TOOL_FIBONACCI_TIME:		// �Ǻ���ġ �ð���
		case CToolOptionInfo::T_TOOL_GANN_LINE45:			// ������(���45��)
		case CToolOptionInfo::T_TOOL_GANN_LINE315:			// ������(����45��)
		case CToolOptionInfo::T_TOOL_GANN_FAN45:			// ����(���45��)
		case CToolOptionInfo::T_TOOL_GANN_FAN135:			// ����(�»�45��)
		case CToolOptionInfo::T_TOOL_GANN_FAN225:			// ����(����45��)
		case CToolOptionInfo::T_TOOL_GANN_FAN315:			// ����(����45��)
		case CToolOptionInfo::T_TOOL_GANN_GRID:				// ���׸���
		case CToolOptionInfo::T_TOOL_ANDREWS_PITCHFORK:		// ����� ��ġ��ũ
		case CToolOptionInfo::T_TOOL_ELLIOT_WAVE_LINE:		// �������� �ĵ���
		case CToolOptionInfo::T_TOOL_CYCLE_LINES:			// ����Ŭ ����
		case CToolOptionInfo::T_TOOL_CUT_LINE:				// ������ �м�����
		case CToolOptionInfo::T_TOOL_SPEED_LINE:			// ���ǵ� ����
		case CToolOptionInfo::T_TOOL_LINEAR_REGRESSION:		// ���� ȸ�ͼ�(ȸ�ͼ�)
		case CToolOptionInfo::T_TOOL_RAFF_REGRESSION:		// Raff ä�μ�(ȸ��ä��)
		case CToolOptionInfo::T_INQUIRY_STATUS:				// ��ġ��ȸ����Ÿ����
		case CToolOptionInfo::T_NIDLG_OVER:					// NIDLG Cursor
		case CToolOptionInfo::T_CROSSHAIRS_OVER:			// CrossLine Cursor
		case CToolOptionInfo::T_TOOL_LINE_EXT1:				// Single Direction Trend Line
		case CToolOptionInfo::T_TOOL_LINE_EXT2:				// Double Direction Trend Line
		case CToolOptionInfo::T_TOOL_LINE_CROSS_WITH_NIDLG:	// CorssLine Tool with NIDLG
		case CToolOptionInfo::T_NIDLG_TOOL:					// NIDLG Tool
		case CToolOptionInfo::T_CROSSHAIRS_TOOL:			// CrossLine Cursor Tool
		case CToolOptionInfo::T_CROSSHAIRS_TOOL_WITH_NIDLG:	// CrossLine Cursor with NIDLG Tool
		case CToolOptionInfo::T_ENABLE_TIMEMARK:			// TimeMark Enabling Tool
															return true;
		// (2006/11/25 - Seung-Won, Bae) Support AutoScroll Tool Button with Left & Right Scroller
		case CToolOptionInfo::T_AUTOSCROLL:					return p_strParam == _MTEXT( C4_LEFT_AUTO_SCROLL) || p_strParam == _MTEXT( C4_RIGHT_AUTO_SCROLL);
	}
	return false;
}

// (2006/2/8 - Seung-Won, Bae) Create from Regular DLL
class CCommonInterface;
interface IToolBarManager;
IToolBarManager * WINAPI CreateToolBarManager( IChartCtrl *p_pIChartCtrl)
{
	return new CToolBarManager( p_pIChartCtrl);
}