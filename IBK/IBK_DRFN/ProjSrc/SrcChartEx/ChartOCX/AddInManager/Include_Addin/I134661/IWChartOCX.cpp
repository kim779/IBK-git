// IWChartOCX.cpp: implementation of the CIWChartOCX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartOCX.h"

#include "../../../Include_Chart/DLL_Load/AddInManager.h"	// for CAddInManager
#include "../../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../../Include/GlobalSharedData.h"				// for ST_GSHAREDFORMAT
#include "../../../Include_Chart/Color.h"					// for CGraphRegionColor
#include "../../EnvironmentData.h"							// for CStyleData
#include "../../ChartStyleDataController.h"					// for CChartStyleDataController
#include "../IAddInDll.h"									// for IAddInDll
#include "_AddInCommand.h"									// for EAI_ALL_APPLY_STYLE
#include "IChartCtrl.h"										// for IChartCtrl
#include "IWAddInToolMgr.h"									// for CIWAddInToolMgr
#include "IWPacketManager.h"								// for CIWPacketManager
#include "IWChartManager.h"									// for CIWChartManager
#include "IWGraphInfoManager.h"								// for CIWGraphInfoManager

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// (2004.10.08, ��¿�)	Chart OCX�� Pointer�� �����Ѵ�.
//		������ �� OCX�� Ctrl Class�̸�, StdAfx.h�� Macro�� �����Ͽ� �̿��Ѵ�.
CIWChartOCX::CIWChartOCX( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp)
{
	m_pIChartCtrl		= p_pIChartCtrl;
	m_pAddInManagerImp	= p_pAddInManagerImp;
	if( !m_pIChartCtrl)
	{
		AfxMessageBox( "CIWChartOCX Null Wrapping Error!");
		return;
	}
}

CIWChartOCX::~CIWChartOCX()
{
}


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Get ChartCtrl Address for Special Usage
//////////////////////////////////////////////////////////////////////
CCommonInterface *CIWChartOCX::GetCommonInterface( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetCommonInterface();
}


//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
IPacketManager *CIWChartOCX::GetIPacketManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CPacketList *pPacketList = m_pIChartCtrl->GetPacketList();
	if( !pPacketList) return NULL;
	return CIWPacketManager::CreateObject( pPacketList);
}
IChartManager *CIWChartOCX::GetIChartManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CMainBlock *pMainBlock = m_pIChartCtrl->GetMainBlock();
	if( !pMainBlock) return NULL;
	return CIWChartManager::CreateObject( pMainBlock);
}
IGraphInfoManager *CIWChartOCX::GetIGraphInfoManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CIndicatorList *pIndicatorList = m_pIChartCtrl->GetIndicatorList();
	if( !pIndicatorList) return NULL;
	return CIWGraphInfoManager::CreateObject( pIndicatorList);
}
IAddInToolMgr *CIWChartOCX::GetIAddInToolMgr( void)
{
	if( !m_pAddInManagerImp) return NULL;
	return CIWAddInToolMgr::CreateObject( m_pAddInManagerImp);
}

//////////////////////////////////////////////////////////////////////
// (2006/4/18 - Seung-Won, Bae) Get/Set OCX's Properties
//////////////////////////////////////////////////////////////////////
BOOL CIWChartOCX::GetAllProperties3( IPropertyMap *p_pIProperties, BOOL p_bWithFull)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->GetAllProperties3( p_pIProperties, p_bWithFull);
}
BOOL CIWChartOCX::SetAllProperties3( IPropertyMap *p_pIProperties)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetAllProperties3( p_pIProperties, FALSE);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, ��¿�) Windows ���� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.11] ReDrawing�� ���� Interface�� �����Ѵ�.
// 	void CIWChartOCX::InvalidateControl( void)
// 	{
// 		if( m_pIChartCtrl) m_pIChartCtrl->InvalidateControl();
// 	}
long CIWChartOCX::InvalidateControl( void)
{
	if( !m_pIChartCtrl) return 0;

	return m_pIChartCtrl->InvalidateControl();
}
void CIWChartOCX::InvalidateControlWithResetBlockRegion( void)
{
	if( m_pIChartCtrl) m_pIChartCtrl->InvalidateOleControl( true);
}

// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
HWND CIWChartOCX::GetOcxHwnd( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetSafeHwnd();
	return NULL;
}

// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
HWND CIWChartOCX::AddSetCapture( void)
{
	if( m_pIChartCtrl)
	{
		CWnd *pWnd = m_pIChartCtrl->AddSetCapture();
		if( pWnd) return pWnd->GetSafeHwnd();
	}
	return NULL;
}

// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
BOOL CIWChartOCX::RemoveSetCapture( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->RemoveSetCapture();
	return FALSE;
}

// [04/10/25] ���ο� Cursor�� �����ϸ鼭, ���� ������ Cursor�� Ȯ���Ѵ�.
HCURSOR CIWChartOCX::SetCursorHandle( HCURSOR p_hNewCursor)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->SetCursorHandle( p_hNewCursor);
}

// [04/10/26] ���� ������ Cursor�� Ȯ���Ѵ�.
HCURSOR CIWChartOCX::GetCursorHandle( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetCursorHandle();
}

// [04.10.25] Chart ���� Font�� �����Ѵ�.
CFont *CIWChartOCX::GetChartFont( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetChartFont();
}

//////////////////////////////////////////////////////////////////////
// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
const char *CIWChartOCX::GetUserFilePath( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetUserFilePath();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetHtsPath
 - Created at  :  2005-07-14   10:59
 - Author      :  ������
 - Description :  Hts�� Dir Path�� �˷��ش�.
 -----------------------------------------------------------------------------------------*/
const char *CIWChartOCX::GetHtsPath( void)
{
	if(!m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetBaseDeskPath();
}

//sy 2006.02.21. OCX Path
const char *CIWChartOCX::GetOCXPath( void)
{
	if(!m_pIChartCtrl) return NULL;
	return  m_pIChartCtrl->GetOCXPath();
}

// (2006/1/18 - Seung-Won, Bae) Set the Design Info
void CIWChartOCX::SetStrBlockRegion( const char *p_szBlockRegion)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetStrBlockRegion( p_szBlockRegion);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) Chart�� �� ���¸� Ȯ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.11] Ư����Ʈ�� ������� Ȯ���ϴ� Interface�� �����Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
BOOL CIWChartOCX::IsOnSpecialChart( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsRunningOneChart();
	return FALSE;
}

// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOCX::HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->HasToolItem( p_eToolItem);
	return FALSE;
}

// (2006/1/20 - Seung-Won, Bae) Get Numerical Inquiry Option Setting
UINT CIWChartOCX::GetNumericalInquiryOption( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetNumericalInquiryOption();
	return 0;
}

// (2006/1/17 - Seung-Won, Bae) Check RunTimeMode
BOOL CIWChartOCX::IsRuntimeMode( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsRunTimeMode();
}

// (2006/1/21 - Seung-Won, Bae) Check Graph Menu Usable
BOOL CIWChartOCX::IsGraphMenuShow( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsGraphMenuShow();
}

// (2006/1/22 - Seung-Won, Bae) Retrieve Bong Count Margin.
short CIWChartOCX::GetBongCountMargin( void) const
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetBongCountMargin();
}

// (2006/1/22 - Seung-Won, Bae) Retrieve Graph RButton Type.
short CIWChartOCX::GetGraphRButtonType( void) const
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetGraphRButtonType();
}

// (2006/2/6 - Seung-Won, Bae) Get OCX Region except ToolBar Region.
CRect CIWChartOCX::GetOcxRegion( void) const
{
	if( !m_pIChartCtrl) return CRect( 0, 0, 0, 0);
	return m_pIChartCtrl->GetOcxRegion();
}

// (2006/2/6 - Seung-Won, Bae) Check ScrollBar
BOOL CIWChartOCX::IsUseToolScrollBar( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsUseToolScrollBar();
}

// (2006/6/24 - Seung-Won, Bae) Get Tooltip's Background color.
COLORREF CIWChartOCX::GetToolTipBackColor( void)
{
	if( !m_pIChartCtrl) return RGB( 0, 0, 0);
	return m_pIChartCtrl->GetToolTipBackColor();
}

// (2004.02.11, ��¿�) ��ġ��ȸâ�� 1�� Block �������� �̵����� �ʰ�,
//		Script���� ������ Property�� ���� OCX ���ݿ� ���� �̵��� �� �ֵ��� �����Ѵ�.
//		(Property�� ��ȸ�ϴ� Interface�� �����Ѵ�.)
BOOL CIWChartOCX::IsOcxAreaNumericalInquiry( void)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsOcxAreaNumericalInquiry();
}

// (2006/3/29 - Seung-Won, Bae) Dynamic Caption Setting for each Site.
BOOL CIWChartOCX::IsOnCaptionFlagOfNIDLG( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsOnCaptionFlagOfNIDLG();
}

// (2006/6/27 - Seung-Won, Bae) Retrive the Back Color of NIDLG
COLORREF CIWChartOCX::GetNIDLGBackColor( void)
{
	if( !m_pIChartCtrl) return RGB( 0, 0, 0);
	return m_pIChartCtrl->GetStyleValueNIDLGColor();
}

// (2006/7/4 - Seung-Won, Bae) Retrieve the Packet Setting Info
const char *CIWChartOCX::GetPacketInfoStr( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetPacketInfoStr();
}

//////////////////////////////////////////////////////////////////////
// (2006/1/24 - Seung-Won, Bae) Interface for Analysis Tool Option
//////////////////////////////////////////////////////////////////////
BOOL CIWChartOCX::IsShowTrendLineText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowTrendLineText();
	return FALSE;
}
BOOL CIWChartOCX::IsShowHorizonTextLeft( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowHorizonTextLeft();
	return FALSE                                                       ;
}
BOOL CIWChartOCX::IsShowHorizonTextRight( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowHorizonTextRight();
	return FALSE;
}
BOOL CIWChartOCX::IsShowVerticalText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowVerticalText();
	return FALSE;
}
BOOL CIWChartOCX::IsShowThreeText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowThreeText();
	return FALSE;
}
BOOL CIWChartOCX::IsShowFourText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowFourText();
	return FALSE;
}
BOOL CIWChartOCX::IsShowFiboText( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsShowFiboText();
	return FALSE;
}
COLORREF CIWChartOCX::GetAnalysisToolColor( void) const
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetStyleValueAnalysisToolColor();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) Data Index �� Position ���� Interface                                                                                                                                                                                             
//////////////////////////////////////////////////////////////////////

// [04/10/11] ������ �� Data�� Index�� ���ϴ� Interface�� �����Ѵ�.
int CIWChartOCX::GetLastRealDataIndex( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetDataCountForToolBar();
	return -1;
}

// [04/10/11] ScrollBar���� ������ Scroll Index�� ���ϴ� Interface�� �����Ѵ�. (��������)
BOOL CIWChartOCX::GetMaxScrollRange( int &p_nMaxScrollRange)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetScrollRange( p_nMaxScrollRange);
	return FALSE;
}

// [04/10/11] Chart�� ���� ���̴� View ������ ù Data Index�� �� Data Index�� ���ϰų�, �����ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOCX::GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}
BOOL CIWChartOCX::SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->SetStartEndIndex( p_nStartIndex, p_nEndIndex);
	return FALSE;
}

// [04/10/27] Data ��ǥ�� Screen ��ǥ�� ��ȯ�ϴ� Interface�� �����Ѵ�.
int CIWChartOCX::ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin, int p_nYPositionMax, int p_nYPositionMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->ConvertDataToYPosition( p_dData, p_dDataMax, p_dDataMin, p_nYPositionMax, p_nYPositionMin, p_bLog, p_bReverse);
	return 0;
}
double CIWChartOCX::ConvertYPositionToData( int p_nYPosition, int p_nYPositionMax, int p_nYPositionMin, double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->ConvertYPositionToData( p_nYPosition, p_nYPositionMax, p_nYPositionMin, p_dDataMax, p_dDataMin, (p_bLog ? true: false), (p_bReverse ? true: false));
	return 0;
}

// [04/11/09] Scroll/Zoom�� ���� ó���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOCX::ScrollTo( int p_nScrollValue, BOOL p_bRelative)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->ScrollTo( p_nScrollValue, p_bRelative);
	return FALSE;
}
BOOL CIWChartOCX::ZoomTo( int p_nZoomValue, BOOL p_bRelative)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->ZoomTo( p_nZoomValue, p_bRelative);
	return FALSE;
}

// (2005.01.13, ��¿�) ��Ʈ�� ��ü���� Mode ���θ� Ȯ�� �� �����ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOCX::CheckFullView( void) const
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsWholeView();
}
void CIWChartOCX::SetFullView( BOOL p_bFullView)
{
	if( !m_pIChartCtrl) return;
	if( ( p_bFullView && m_pIChartCtrl->IsWholeView())
		|| ( !p_bFullView && !m_pIChartCtrl->IsWholeView())) return;

	m_pIChartCtrl->SetWholeView( p_bFullView);
	m_pIChartCtrl->InvalidateControl();
}

// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
void CIWChartOCX::DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd)
{
	if( m_pIChartCtrl) m_pIChartCtrl->DragZoomIn( p_ptStart, p_ptEnd);
}

//////////////////////////////////////////////////////////////////////
// [04.10.11] ���� ���� Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] �־��� Global ������ �����Ǿ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOCX::CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->CheckGSharedSetting( p_szGSharedName, p_bStart, p_bGet, p_bSet);
}

// [04.10.11] �־��� Global ������ ������ �߻���Ű�� Interface�� �����Ѵ�.
BOOL CIWChartOCX::FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->FireGSharedData( p_szGSharedName, p_szGSharedData);
}

// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
// (2005/8/10 - Seung-Won, Bae) Retrieve the Share Info Item Setting
BOOL CIWChartOCX::GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter)
{
	if( !m_pIChartCtrl) return FALSE;
	m_pIChartCtrl->GetShareInfoItem( p_szShareName, p_nShareType, p_nShareArea, p_lCodeFilter);
	return TRUE;
}

// (2005/8/10 - Seung-Won, Bae) Fire Shared Data
BOOL CIWChartOCX::FireSharedData( const char *p_szShareName, const char *p_szSharedData)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->FireSharedData( p_szShareName, p_szSharedData);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
void CIWChartOCX::DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor)
{
	if( !m_pIChartCtrl) return;
	CBitmap bufferBitmap;
	m_pIChartCtrl->OnDrawForPrint( *p_pDC, bufferBitmap, p_rctSource, p_bUseScreenColor);
}

//////////////////////////////////////////////////////////////////////
// ����(05/06/01) �׷����� �߰�,����,���� ���� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2006/6/11 - Seung-Won, Bae) ��ǥ�� �߰�/�����Ѵ�.
BOOL CIWChartOCX::AddChartBlock( const char *p_szIndicatorName)
{
	if( !m_pIChartCtrl) return FALSE;
	CString strGraphName;
	return m_pIChartCtrl->AddChartBlock( p_szIndicatorName, strGraphName);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  AddPacket
 - Created at  :  2005-06-01   10:12
 - Author      :  ������
 - Description :  ��Ŷ�� �߰��Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CIWChartOCX::AddPacket(const char* pcszPacketOption)
{
	if(!m_pIChartCtrl)
		return;
	m_pIChartCtrl->AddPacketAdapter(pcszPacketOption);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  AddGraph
 - Created at  :  2005-06-01   13:35
 - Author      :  ������
 - Description :  ��ǥ�� �߰��� �Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CIWChartOCX::AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale)
{
	if(!m_pIChartCtrl)
		return;
	m_pIChartCtrl->AddGraphAdapter(nRow, nColumn, nVertScaleGroup, szGraphOption, bCreateBlock, bCreateVertScaleGroup, bUseAutoScale);
	return;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeSubGraphData
 - Created at  :  2005-06-01   16:45
 - Author      :  ������
 - Description :  ��ǥ���� �׷��� �Ӽ�(��, �β�, ����)�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle)
{
	if(!m_pIChartCtrl)
		return false;
	return m_pIChartCtrl->ChangeSubGraphData(pcszGraphName, pcszSubGraphName, clrLineColor, nThickness, nDrawStyle);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartPeriodOfPacket
 - Created at  :  2005-06-07   15:55
 - Author      :  ������
 - Description :  Packet�� �������ڸ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace)
{
	if(!m_pIChartCtrl)
		return false;
	return m_pIChartCtrl->SetStartPeriodOfPacketAdapter(pcszPacketName, lStartPeriod, bAddSpace);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartIndexOfPacket
 - Created at  :  2005-08-27   17:44
 - Author      :  ������
 - Description :  Packet�� �����ε����� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace)
{
	if(!m_pIChartCtrl)
		return false;
	return m_pIChartCtrl->SetStartIndexOfPacket(NULL, pcszPacketName, nStartDataIndex, bAddSpace);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  HasGraph
 - Created at  :  2005-06-07   15:56
 - Author      :  ������
 - Description :  �ش� �׷����� �ִ��� Ȯ���Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::HasGraph(const char* pcszGraphName)
{
	if(!m_pIChartCtrl)
		return false;
	return m_pIChartCtrl->HasGraphAdapter(pcszGraphName);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/12
// Modifier		: 
// Comments		: Check the graph in Back of Special Graph
//-----------------------------------------------------------------------------
BOOL CIWChartOCX::HasGraphInSpecialBack( const char *p_szGraphName)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->HasGraphInSpecialBack( p_szGraphName);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RemoveGraph
 - Created at  :  2005-11-07
 - Author      :  �輱��
 - Description :  �ش� �׷����� �����ϱ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::RemoveGraph(const char* pcszGraphName)
{
	if(m_pIChartCtrl == NULL)
		return false;
	return (m_pIChartCtrl->RemoveGraph( pcszGraphName) ? true: false);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RemovePacket
 - Created at  :  2005-11-07
 - Author      :  �輱��
 - Description :  �ش� packet �����ϱ�.
					bIsOnlyDataClear = true : data �� ����
					bIsOnlyDataClear = false : CPacket ����
 -----------------------------------------------------------------------------------------*/
bool CIWChartOCX::RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear)
{
	if(m_pIChartCtrl == NULL)
		return false;

	if(!bIsOnlyDataClear)
		return (m_pIChartCtrl->RemovePacket(pcszPacketName) ? true: false);
	return (m_pIChartCtrl->ClearPacketData(pcszPacketName, true) ? true: false);
}

//////////////////////////////////////////////////////////////////////
// ����(05/08/22) ������ ó���Ҽ� �ִ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------------------------
 - Function    :  SetRealData
 - Created at  :  2005-08-22   20:09
 - Author      :  ������
 - Parameters  :  pcszPacketName - ��Ŷ��
 -                dData          - ������Ÿ
 -                nOption        - 1: Update, 2:Append
 - Description :  ������Ÿ�� ó���ϴ� Interface�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CIWChartOCX::SetRealData(const char* pcszPacketName, double dData, short nOption)
{
	if(!m_pIChartCtrl)
		return;
	m_pIChartCtrl->SetRealData(pcszPacketName, dData, nOption);
}

// (2006/1/21 - Seung-Won, Bae) Save Current Graph State
BOOL CIWChartOCX::SaveChartCfg( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->SaveChartCfg();
	return FALSE;
}
// (2006/1/23 - Seung-Won, Bae) Set Logging Flag
void CIWChartOCX::SetLoggingFlag( const BOOL p_bInLogging)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetLoggingFlag( p_bInLogging);
}

// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
void CIWChartOCX::ReleaseOtherToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption)
{
	if( m_pIChartCtrl) m_pIChartCtrl->ReleaseOtherToolUI( p_eToolOption);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Notify from AddIn
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/18
// Modifier		: 
// Comments		: for Event from AddIn
//-----------------------------------------------------------------------------
void CIWChartOCX::FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData)
{
	if( !m_pIChartCtrl) return;
	m_pIChartCtrl->NeoAddInEvent( p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);
}

// [04.10.11] DLL Container���� Message�ϴ� Interface�� �����Ѵ�.
LRESULT CIWChartOCX::SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SendEventMsgToDLL( message, wParam, lParam);
	return 0;
}

//////////////////////////////////////////////////////////////////////
// (2006/3/23 - Seung-Won, Bae) Set/Get StyleData
//////////////////////////////////////////////////////////////////////

void CIWChartOCX::GetStyleData( CStyleData_AI &p_styleData)
{
	if( !m_pIChartCtrl) return;
	CStyleData styleData;


	CMainBlock *pMainBlock = m_pIChartCtrl->GetMainBlock();
	if( !pMainBlock) return;

	CIndicatorList *pIndicatorList = m_pIChartCtrl->GetIndicatorList();
	if( !pIndicatorList) return;

	CAddInManager *pAddInManager = m_pIChartCtrl->GetAddInManager();
	if( !pAddInManager) return;

	styleData.SetStyleName("��ȭ�齺Ÿ��");

	//��ǥ ���� color
	CChartStyleDataController( pMainBlock, pIndicatorList).GetStyleData_Indicator( styleData);

	//graph ������ ���õ� ��
	styleData.SetGraphBGColor( m_pIChartCtrl->GetStyleValueGraphBGColor());
	styleData.SetGraphBorderColor( m_pIChartCtrl->GetStyleValueGraphBorderColor());
	//graph �׸��� ������ �ٵ��� �κ�
	styleData.SetGraphGridColor( m_pIChartCtrl->GetStyleValueGraphGridColor());
	styleData.SetGridHeight( m_pIChartCtrl->GetStyleValueGridHeight());
	styleData.SetGridWidth( m_pIChartCtrl->GetStyleValueGridWidth());
	//scale ���� ��
	styleData.SetScaleBGColor( m_pIChartCtrl->GetStyleValueScaleBGColor());
	styleData.SetScaleBorderColor( m_pIChartCtrl->GetStyleValueScaleBorderColor());
	styleData.SetScaleTextColor( m_pIChartCtrl->GetStyleValueScaleTextColor());
	styleData.SetScaleGridColor( m_pIChartCtrl->GetStyleValueGridHLineColor());
	//scale type
	styleData.SetDivideWidth( m_pIChartCtrl->GetFirstBlockHGridLineType());
	styleData.SetDivideHeight( m_pIChartCtrl->GetFirstBlockVGridLineType());
	//�м������� ��ġ��ȸâ ���û�.
	styleData.SetAnalyzeColor( m_pIChartCtrl->GetStyleValueAnalysisToolColor());
	styleData.SetNumericalColor( m_pIChartCtrl->GetStyleValueNIDLGColor());

	pMainBlock->GetAllSubGraphTitle( _MTEXT( C2_PRICE_MA), styleData.GetMATitleListData().GetPriceMATitleList());
	pMainBlock->GetAllSubGraphTitle( _MTEXT( C2_VOLUME_MA), styleData.GetMATitleListData().GetTradeMATitleList());


	p_styleData.m_styleName			= styleData.GetStyleName();
	p_styleData.m_priceUpColor		= styleData.GetPriceUpColor();
	p_styleData.m_priceDownColor	= styleData.GetPriceDownColor();
	p_styleData.m_priceEqualColor	= styleData.GetPriceEqualColor();
	p_styleData.m_priceUpFill		= styleData.GetPriceUpFill();
	p_styleData.m_priceDownFill		= styleData.GetPriceDownFill();
	p_styleData.m_tradeUpColor		= styleData.GetTradeUpColor();
	p_styleData.m_tradeDownColor	= styleData.GetTradeDownColor();
	p_styleData.m_tradeEqualColor	= styleData.GetTradeEqualColor();
	p_styleData.m_tradeUpFill		= styleData.GetTradeUpFill();
	p_styleData.m_tradeDownFill		= styleData.GetTradeDownFill();
	p_styleData.m_priceColor1		= styleData.GetPriceMAColor( 0);
	p_styleData.m_priceColor2		= styleData.GetPriceMAColor( 1);
	p_styleData.m_priceColor3		= styleData.GetPriceMAColor( 2);
	p_styleData.m_priceColor4		= styleData.GetPriceMAColor( 3);
	p_styleData.m_priceColor5		= styleData.GetPriceMAColor( 4);
	p_styleData.m_priceColor6		= styleData.GetPriceMAColor( 5);
	p_styleData.m_priceColor7		= styleData.GetPriceMAColor( 6);
	p_styleData.m_priceColor8		= styleData.GetPriceMAColor( 7);
	p_styleData.m_priceColor9		= styleData.GetPriceMAColor( 8);
	p_styleData.m_priceColor10		= styleData.GetPriceMAColor( 9);
	p_styleData.m_tradeColor1		= styleData.GetTradeMAColor( 0);
	p_styleData.m_tradeColor2		= styleData.GetTradeMAColor( 1);
	p_styleData.m_tradeColor3		= styleData.GetTradeMAColor( 2);
	p_styleData.m_tradeColor4		= styleData.GetTradeMAColor( 3);
	p_styleData.m_graphBGColor		= styleData.GetGraphBGColor();
	p_styleData.m_graphBorderColor	= styleData.GetGraphBorderColor();
	p_styleData.m_graphGridColor	= styleData.GetGraphGridColor();
	p_styleData.m_scaleBGColor		= styleData.GetScaleBGColor();
	p_styleData.m_scaleBorderColor	= styleData.GetScaleBorderColor();
	p_styleData.m_scaleTextColor	= styleData.GetScaleTextColor();
	p_styleData.m_scaleGridColor	= styleData.GetScaleGridColor();
	p_styleData.m_gridWidth			= styleData.GetGridWidth();
	p_styleData.m_gridHeight		= styleData.GetGridHeight();
	p_styleData.m_divideWidth		= styleData.GetDivideWidth();
	p_styleData.m_divideHeight		= styleData.GetDivideHeight();
	p_styleData.m_analyzeColor		= styleData.GetAnalyzeColor();
	p_styleData.m_numericalColor	= styleData.GetNumericalColor();
}

void CIWChartOCX::SetStyleData( CStyleData_AI p_styleData)
{
	if( !m_pIChartCtrl) return;

	CStyleData styleData;
	styleData.SetStyleName(			p_styleData.m_styleName	);
	styleData.SetPriceUpColor(		p_styleData.m_priceUpColor);
	styleData.SetPriceDownColor(	p_styleData.m_priceDownColor);
	styleData.SetPriceEqualColor(	p_styleData.m_priceEqualColor);
	styleData.SetPriceUpFill(		p_styleData.m_priceUpFill);
	styleData.SetPriceDownFill(		p_styleData.m_priceDownFill);
	styleData.SetTradeUpColor(		p_styleData.m_tradeUpColor);
	styleData.SetTradeDownColor(	p_styleData.m_tradeDownColor);
	styleData.SetTradeEqualColor(	p_styleData.m_tradeEqualColor);
	styleData.SetTradeUpFill(		p_styleData.m_tradeUpFill);
	styleData.SetTradeDownFill(		p_styleData.m_tradeDownFill);
	styleData.SetPriceMAColor( 0,	p_styleData.m_priceColor1);
	styleData.SetPriceMAColor( 1,	p_styleData.m_priceColor2);
	styleData.SetPriceMAColor( 2,	p_styleData.m_priceColor3);
	styleData.SetPriceMAColor( 3,	p_styleData.m_priceColor4);
	styleData.SetPriceMAColor( 4,	p_styleData.m_priceColor5);
	styleData.SetPriceMAColor( 5,	p_styleData.m_priceColor6);
	styleData.SetPriceMAColor( 6,	p_styleData.m_priceColor7);
	styleData.SetPriceMAColor( 7,	p_styleData.m_priceColor8);
	styleData.SetPriceMAColor( 8,	p_styleData.m_priceColor9);
	styleData.SetPriceMAColor( 9,	p_styleData.m_priceColor10);
	styleData.SetTradeMAColor( 0,	p_styleData.m_tradeColor1);
	styleData.SetTradeMAColor( 1,	p_styleData.m_tradeColor2);
	styleData.SetTradeMAColor( 2,	p_styleData.m_tradeColor3);
	styleData.SetTradeMAColor( 3,	p_styleData.m_tradeColor4);
	styleData.SetGraphBGColor(		p_styleData.m_graphBGColor);
	styleData.SetGraphBorderColor(	p_styleData.m_graphBorderColor);
	styleData.SetGraphGridColor(	p_styleData.m_graphGridColor);
	styleData.SetScaleBGColor(		p_styleData.m_scaleBGColor);
	styleData.SetScaleBorderColor(	p_styleData.m_scaleBorderColor);
	styleData.SetScaleTextColor(	p_styleData.m_scaleTextColor);
	styleData.SetScaleGridColor(	p_styleData.m_scaleGridColor);
	styleData.SetGridWidth(			p_styleData.m_gridWidth);
	styleData.SetGridHeight(		p_styleData.m_gridHeight);
	styleData.SetDivideWidth(		p_styleData.m_divideWidth);
	styleData.SetDivideHeight(		p_styleData.m_divideHeight);
	styleData.SetAnalyzeColor(		p_styleData.m_analyzeColor);
	styleData.SetNumericalColor(	p_styleData.m_numericalColor);

	CMainBlock *pMainBlock = m_pIChartCtrl->GetMainBlock();
	if( !pMainBlock) return;

	CIndicatorList *pIndicatorList = m_pIChartCtrl->GetIndicatorList();
	if( !pIndicatorList) return;

	CAddInManager *pAddInManager = m_pIChartCtrl->GetAddInManager();
	if( !pAddInManager) return;

	// styleData�� ocx �� �����Ѵ�.
	CChartStyleDataController( pMainBlock, pIndicatorList).SetStyleData_Indicator( styleData);
	//graph ������ ���õ� ��
	m_pIChartCtrl->SetStyleValueGraphBGColor( styleData.GetGraphBGColor());
	m_pIChartCtrl->SetStyleValueGraphBorderColor( styleData.GetGraphBorderColor());
	//graph �׸��� ������ �ٵ��� �κ�
	m_pIChartCtrl->SetStyleValueGraphGridColor( styleData.GetGraphGridColor());
	m_pIChartCtrl->SetStyleValueGridHeight( styleData.GetGridHeight());
	m_pIChartCtrl->SetStyleValueGridWidth( styleData.GetGridWidth());
	//scale ���� ��
	m_pIChartCtrl->SetStyleValueScaleBGColor( styleData.GetScaleBGColor());
	m_pIChartCtrl->SetStyleValueScaleBorderColor( styleData.GetScaleBorderColor());
	m_pIChartCtrl->SetStyleValueScaleTextColor( styleData.GetScaleTextColor());
	m_pIChartCtrl->SetStyleValueGridHLineColor( styleData.GetScaleGridColor());
	m_pIChartCtrl->SetStyleValueGridVLineColor( styleData.GetScaleGridColor());
	//Analysis tool
	m_pIChartCtrl->SetStyleValueAnalysisToolColor( styleData.GetAnalyzeColor());
	// ��ġ��ȸâ.
	m_pIChartCtrl->SetStyleValueNIDLGColor( styleData.GetNumericalColor());

	// ocx �� ����� data �� �ش� dll�� �����Ų��.
	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	pMainBlock->SetBaseScaleGridType( ( CScaleBaseData::SCALEGRIDTYPE)styleData.GetDivideHeight());
	pMainBlock->SetAllVertScaleGridType( ( CScaleBaseData::SCALEGRIDTYPE)styleData.GetDivideHeight());
	pMainBlock->SetAllHorzScaleGridType( ( CScaleBaseData::SCALEGRIDTYPE)styleData.GetDivideWidth());
	pMainBlock->SetGraphRegionColor( CGraphRegionColor( styleData.GetGraphBGColor(), styleData.GetGraphGridColor(), styleData.GetScaleBorderColor()));
	pMainBlock->SetScaleColor( styleData.GetScaleGridColor(), styleData.GetScaleGridColor(), styleData.GetScaleTextColor());
	pMainBlock->SetGraphBackgroundGap( styleData.GetGridHeight(), styleData.GetGridWidth());

	// (2006/1/26 - Seung-Won, Bae) Apply Style to AddIn Tools
	pAddInManager->OnAddInToolCommand( EAI_ALL_APPLY_STYLE);

	m_pIChartCtrl->InvalidateControl();
}
