// IWChartOCX.cpp: implementation of the CIWChartOCX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartOCX.h"

#include "../../../Include/GlobalSharedData.h"				// for ST_GSHAREDFORMAT
#include "../../../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "../../../Include_Chart/Conversion.h"				// for CDataConversion
#include "../IAddInDll.h"							// for IAddInDll
#include "IChartCtrl.h"								// for IChartCtrl
#include "IWAddInToolMgr.h"							// for CIWAddInToolMgr
#include "IWPacketManager.h"						// for CIWPacketManager
#include "IWChartManager.h"							// for CIWChartManager
#include "IWGraphInfoManager.h"						// for CIWGraphInfoManager
#include "IWString.h"								// for CIWString
#include "IWStringList.h"							// for CIWStringList
#include "IWPacketListManager.h"					// for CIWPacketListManager

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
IChartOCX *CIWChartOCX::CreateObject( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp)
{
	IChartOCX *pIChartOCX = new CIWChartOCX( p_pIChartCtrl, p_pAddInManagerImp);
	pIChartOCX->AddRef();
	return pIChartOCX;
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Get ChartCtrl Address for Special Usage
//////////////////////////////////////////////////////////////////////
CCommonInterface *CIWChartOCX::GetCommonInterface( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetCommonInterface();
}
COutsideCommonInterface *CIWChartOCX::GetMChartInterface( void)
{
	if( !m_pIChartCtrl) return NULL;
	return m_pIChartCtrl->GetMChartInterface();
}


//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
IPacketManager *CIWChartOCX::GetIDefaultPacketManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CPacketList *pPacketList = m_pIChartCtrl->GetPacketList();
	if( !pPacketList) return NULL;
	// PacketListManager Setting : �������� - ojtaso (20070307)
	IPacketManager* pIPacketList = CIWPacketManager::CreateObject( pPacketList);
	pIPacketList->SetPacketListManager(m_pIChartCtrl->GetPacketListManager());
	return pIPacketList;
}
IChartManager *CIWChartOCX::GetIChartManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CMainBlock *pMainBlock = m_pIChartCtrl->GetMainBlock();
	if( !pMainBlock) return NULL;

	//{{ XScaleManager Setting : XScale - LYH (20070326)
	IChartManager* pChartManager = CIWChartManager::CreateObject( pMainBlock);
	pChartManager->SetXScaleManager(m_pIChartCtrl->GetXScaleManager());
	return pChartManager;
	//}}
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
IPacketListManager *CIWChartOCX::GetIPacketListManager( void)
{
	if( !m_pIChartCtrl) return NULL;
	CPacketListManager *pPacketListManager = m_pIChartCtrl->GetPacketListManager();
	if( !pPacketListManager) return NULL;
	return CIWPacketListManager::CreateObject( pPacketListManager);;
}


//////////////////////////////////////////////////////////////////////
// (2006/8/15 - Seung-Won, Bae) Create Buffer
//////////////////////////////////////////////////////////////////////
IStringList *CIWChartOCX::CreateObjectIStringList( void)
{
	CIWStringList::CStringTList *pStringTList = new CIWStringList::CStringTList;
	return CIWStringList::CreateObject( pStringTList, TRUE);
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
HFONT CIWChartOCX::GetChartFont( void)
{
	if( !m_pIChartCtrl) return NULL;
	CFont *pFont = m_pIChartCtrl->GetChartFont();
	return pFont ? ( HFONT)pFont->GetSafeHandle() : NULL;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
IString *CIWChartOCX::GetUserFilePathString( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWString::CreateObject( m_pIChartCtrl->GetUserFilePath());
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetHtsPath
 - Created at  :  2005-07-14   10:59
 - Author      :  ������
 - Description :  Hts�� Dir Path�� �˷��ش�.
 -----------------------------------------------------------------------------------------*/
IString *CIWChartOCX::GetHtsPathString( void)
{
	if(!m_pIChartCtrl) return NULL;
	return CIWString::CreateObject( m_pIChartCtrl->GetBaseDeskPath());
}

//sy 2006.02.21. OCX Path
IString *CIWChartOCX::GetOCXPathString( void)
{
	if(!m_pIChartCtrl) return NULL;
	return  CIWString::CreateObject( m_pIChartCtrl->GetOCXPath());
}

// (2006/8/19 - Seung-Won, Bae) Default File (Data) Folder Path
IString *CIWChartOCX::GetDefaultPathString( void)
{
	if(!m_pIChartCtrl) return NULL;
	CString strDefaultPath;
	m_pIChartCtrl->GetDefaultPath( strDefaultPath);
	return  CIWString::CreateObject( strDefaultPath);
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

BOOL CIWChartOCX::SetNumericalInquiryOption( UINT p_uOption)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetNumericalInquiryOption( p_uOption);
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
RECT CIWChartOCX::GetOcxRegion( void) const
{
	RECT rctEmpty = { 0, 0, 0, 0};
	if( !m_pIChartCtrl) return rctEmpty;
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

// (2006/7/4 - Seung-Won, Bae) Retrieve the Packet Setting Info
IString *CIWChartOCX::GetPacketInfoString( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWString::CreateObject( m_pIChartCtrl->GetPacketInfoStr());
}

// (2006/7/25 - Seung-Won, Bae) Get Normalized Minute Time Rule
BOOL CIWChartOCX::GetNormalizedMinuteTimeRule( void)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->GetNormalizedMinuteTimeRule();
}

BOOL CIWChartOCX::SetNormalizedMinuteTimeRule( BOOL p_bEnable)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetNormalizedMinuteTimeRule( p_bEnable);
}

// (2006/7/26 - Seung-Won, Bae) Check Logging State
BOOL CIWChartOCX::IsInLogging( void)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsInLogging();
}

// (2006/7/26 - Seung-Won, Bae) Check Mode in Real Usage
BOOL CIWChartOCX::IsInUseRealData( void)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsInUseRealData();
}

// (2006/11/7 - Seung-Won, Bae) Show/Hide All Graph Title
BOOL CIWChartOCX::IsHideAllTitle( void)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsHideAllTitle();
}
BOOL CIWChartOCX::SetHideAllTitle( BOOL p_bHideAllTitle)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetHideAllTitle( p_bHideAllTitle);
}

// (2006/11/7 - Seung-Won, Bae) Show/Hide Price Min/Max Data Text
BOOL CIWChartOCX::IsShowPriceMinMaxText( void)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsShowPriceMinMaxText();
}
BOOL CIWChartOCX::ShowPriceMinMaxText( BOOL p_bShowPriceMinMaxText)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->ShowPriceMinMaxText( p_bShowPriceMinMaxText);
}

// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
BOOL CIWChartOCX::IsMultipleSameNameIndicator( void)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->IsMultipleSameNameIndicator();
}
void CIWChartOCX::SetMultipleSameNameIndicator( BOOL p_bEnable)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetMultipleSameNameIndicator( p_bEnable);
}

// (2007/3/27 - Seung-Won, Bae) Check TR Indicator
BOOL CIWChartOCX::IsTRIndicator( const char *p_szGroupName, const char *p_szIndicatorName)
{
	if( m_pIChartCtrl) return g_iMetaTable.IsMarketOrServerIndex( p_szIndicatorName);
	return FALSE;
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
BOOL CIWChartOCX::SetAnalysisToolTextFlag( BOOL p_bTrendLine, BOOL p_bHorizonLeft, BOOL p_bHorizonRight, BOOL p_bVerticalLine, BOOL p_bThreeLine, BOOL p_bFourLine, BOOL p_bFiboLine)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetAnalysisToolTextFlag( p_bTrendLine, p_bHorizonLeft, p_bHorizonRight, p_bVerticalLine, p_bThreeLine, p_bFourLine, p_bFiboLine);
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
void CIWChartOCX::DragZoomIn( const POINT p_ptStart, const POINT p_ptEnd)
{
	if( m_pIChartCtrl) m_pIChartCtrl->DragZoomIn( p_ptStart, p_ptEnd);
}

// (2006/7/27 - Seung-Won, Bae) Reset Scroll
BOOL CIWChartOCX::SetScrollData( const int nType, const BOOL bIsHorzScaleDataCheck, const BOOL bIsRedraw)
{
	// (2007/5/25 - Seung-Won, Bae) Support only Common for Real and Env for Right Margin.
	static int nAllowScroll = -1;
	if( -1 == nAllowScroll) switch( _NAIMCODE)
	{
		case 0:
		case 5914:	nAllowScroll = 1;
					break;
		default:	nAllowScroll = 0;
					break;
	}

	if( !m_pIChartCtrl || !nAllowScroll) return FALSE;
	m_pIChartCtrl->ResetScroll();
	return TRUE;
}

// (2006/8/16 - Seung-Won, Bae) Right Virtual Candle
int	CIWChartOCX::GetBongCountMargin( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetBongCountMargin();
}

BOOL CIWChartOCX::SetBongCountMargin( const int p_nCount)
{
	if( !m_pIChartCtrl) return 0;
	m_pIChartCtrl->SetBongCountMargin( p_nCount);
	return TRUE;
}

// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
BOOL CIWChartOCX::SetDrawBaseLine(BOOL bDrawBaseLine) 
{
	return m_pIChartCtrl->SetDrawBaseLine(bDrawBaseLine); 
}

bool CIWChartOCX::GetDrawBaseLine() 
{ 
	return m_pIChartCtrl->GetDrawBaseLine(); 
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
// (2006/7/25 - Seung-Won, Bae) Set Shared Data with manual.
BOOL CIWChartOCX::AddLocalShareInfo( const char *p_szLSharedName, int p_nSettingType, const char *p_szLocalSharedData)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->AddLocalShareInfo( p_szLSharedName, p_nSettingType, p_szLocalSharedData);
}

// (2006/7/25 - Seung-Won, Bae) Get Local Shared Data.
IString *CIWChartOCX::GetLocalSharedDataString( const char *p_szLSharedName)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWString::CreateObject( m_pIChartCtrl->GetLocalSharedData( p_szLSharedName));
}

// (2006/7/26 - Seung-Won, Bae) Remove Local Shared Info with manual.
BOOL CIWChartOCX::RemoveLocalShareInfo( const char *p_szLSharedName)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->RemoveLocalShareInfo( p_szLSharedName);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
void CIWChartOCX::DrawForImage( HDC p_hDC, RECT p_rctSource, BOOL p_bUseScreenColor)
{
	if( !m_pIChartCtrl) return;
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	CBitmap bufferBitmap;
	m_pIChartCtrl->OnDrawForPrint( *pDC, bufferBitmap, p_rctSource, p_bUseScreenColor);
}

//////////////////////////////////////////////////////////////////////
// ����(05/06/01) �׷����� �߰�,����,���� ���� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2006/6/11 - Seung-Won, Bae) ��ǥ�� �߰�/�����Ѵ�.
IString *CIWChartOCX::AddChartBlock( const char *p_szIndicatorName)
{
	if( !m_pIChartCtrl) return FALSE;
	CString strGraphName;
	if( !m_pIChartCtrl->AddChartBlock( p_szIndicatorName, strGraphName)) return NULL;
	return CIWString::CreateObject( strGraphName);
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
BOOL CIWChartOCX::RemoveChart( const char *p_szIndicator)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->RemoveChart( p_szIndicator);
}
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
// Get
COLORREF CIWChartOCX::GetStyleValueGraphBGColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueGraphBGColor();
}
COLORREF CIWChartOCX::GetStyleValueGraphBorderColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueGraphBorderColor();
}
COLORREF CIWChartOCX::GetStyleValueGraphGridColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueGraphGridColor();
}
int CIWChartOCX::GetStyleValueGridHeight( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueGridHeight();
}
int CIWChartOCX::GetStyleValueGridWidth( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueGridWidth();
}
COLORREF CIWChartOCX::GetStyleValueGridHLineColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueGridHLineColor();
}
COLORREF CIWChartOCX::GetStyleValueGridVLineColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueGridVLineColor();
}
COLORREF CIWChartOCX::GetStyleValueScaleTextColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueScaleTextColor();
}
int CIWChartOCX::GetFirstBlockHGridLineType( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetFirstBlockHGridLineType();
}
int CIWChartOCX::GetFirstBlockVGridLineType( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetFirstBlockVGridLineType();
}
COLORREF CIWChartOCX::GetStyleValueScaleBGColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueScaleBGColor();
}
COLORREF CIWChartOCX::GetStyleValueScaleBorderColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueScaleBorderColor();
}
COLORREF CIWChartOCX::GetStyleValueAnalysisToolColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueAnalysisToolColor();
}
COLORREF CIWChartOCX::GetStyleValueNIDLGColor( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueNIDLGColor();
}
// SetGraphRegionColor
BOOL CIWChartOCX::SetStyleValueGraphBGColor( COLORREF p_clrGraphBG, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueGraphBGColor( p_clrGraphBG, p_bWithUpdate);
}
BOOL CIWChartOCX::SetStyleValueGraphBorderColor( COLORREF p_clrGraphBorder, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueGraphBorderColor( p_clrGraphBorder, p_bWithUpdate);
}
BOOL CIWChartOCX::SetStyleValueGraphGridColor( COLORREF	p_clrGraphGrid, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueGraphGridColor( p_clrGraphGrid, p_bWithUpdate);
}
// SetGraphBackgroundGap
BOOL CIWChartOCX::SetStyleValueGridHeight( int p_nGridHeight, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueGridHeight( p_nGridHeight, p_bWithUpdate);
}
BOOL CIWChartOCX::SetStyleValueGridWidth( int p_nGridWidth, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueGridWidth( p_nGridWidth, p_bWithUpdate);
}
// SetScaleColor
BOOL CIWChartOCX::SetStyleValueGridHLineColor( COLORREF	p_clrGridHLine, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueGridHLineColor( p_clrGridHLine, p_bWithUpdate);
}
BOOL CIWChartOCX::SetStyleValueGridVLineColor( COLORREF	p_clrGridVLine, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueGridVLineColor( p_clrGridVLine, p_bWithUpdate);
}
BOOL CIWChartOCX::SetStyleValueScaleTextColor( COLORREF p_clrScaleText, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueScaleTextColor( p_clrScaleText, p_bWithUpdate);
}
// SetAllHorzScaleGridType
BOOL CIWChartOCX::SetAllHorzScaleGridType( int p_nGridHLineType, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetAllHorzScaleGridType( p_nGridHLineType);
}
// SetAllVertScaleGridType
BOOL CIWChartOCX::SetAllVertScaleGridType( int p_nGridVLineType, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetAllVertScaleGridType( p_nGridVLineType);
}
// Updated in OnDraw
BOOL CIWChartOCX::SetStyleValueScaleBGColor( COLORREF p_clrScaleBG)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueScaleBGColor( p_clrScaleBG);
}
BOOL CIWChartOCX::SetStyleValueScaleBorderColor( COLORREF p_clrScaleBorder)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueScaleBorderColor( p_clrScaleBorder);
}
// Updated in EAI_ALL_APPLY_STYLE
BOOL CIWChartOCX::SetStyleValueAnalysisToolColor( COLORREF p_clrAnalysisTool)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueAnalysisToolColor( p_clrAnalysisTool);
}
BOOL CIWChartOCX::SetStyleValueNIDLGColor( COLORREF	p_clrNIDL)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueNIDLGColor( p_clrNIDL);
}

//////////////////////////////////////////////////////////////////////
// (2006/8/16 - Seung-Won, Bae) Set/Get the Graph Title Option
//////////////////////////////////////////////////////////////////////
BOOL CIWChartOCX::GetGraphTitleOption( BOOL &p_bWithConditionValue, BOOL &p_bWithLastValue)
{
	if( !m_pIChartCtrl) return FALSE;
	m_pIChartCtrl->GetGraphTitleOption( p_bWithConditionValue, p_bWithLastValue);
	return TRUE;
}
BOOL CIWChartOCX::SetGraphTitleOption( BOOL p_bWithConditionValue, BOOL p_bWithLastValue)
{
	if( !m_pIChartCtrl) return FALSE;
	m_pIChartCtrl->SetGraphTitleOption( p_bWithConditionValue, p_bWithLastValue);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// (2006/8/15 - Seung-Won, Bae) with SaveChartCFG
//////////////////////////////////////////////////////////////////////
BOOL CIWChartOCX::SaveChartCFG_RemoveFromIndexToEnd( const int p_nIndex)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SaveChartCFG_RemoveFromIndexToEnd( p_nIndex);
}
BOOL CIWChartOCX::SaveChartCFG_ResetWithCurrentStyle( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->SaveChartCFG_ResetWithCurrentStyle();
}
BOOL CIWChartOCX::SaveChartCFG_RestoreChartOfIndex( const int p_nIndex)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->RestoreChartCfg( p_nIndex);
}
int CIWChartOCX::SaveChartCFG_SaveNow( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->SaveChartCFG_SaveNow();
}

// ���ǰ� ����� ���� ���� - ojtaso (20070913)
void CIWChartOCX::ReCalculateIndicator(void)
{
	m_pIChartCtrl->ReCalculateIndicator();
}

// (2009/5/13 - Seung-Won, Bae) for Right Margin
void CIWChartOCX::SetRightMarginWidth( int nRightMarginWidth)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetRightMarginWidth( nRightMarginWidth);
}
int CIWChartOCX::GetRightMarginWidth( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetRightMarginWidth();
}

// (2009/5/17 - Seung-Won, Bae) for Future Time
void CIWChartOCX::SetFutureTimeCount( int nFutureTimeCount)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetFutureTimeCount( nFutureTimeCount);
}
int CIWChartOCX::GetFutureTimeCount( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetFutureTimeCount();
}

// (2009/9/1 - Seung-Won, Bae) Define Chart Mode.
int CIWChartOCX::GetChartMode( void)
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetChartMode();
	return CChartInfo::HTS;
}

IString *CIWChartOCX::GetCurrentRQ( void)
{
	if( !m_pIChartCtrl) return NULL;

	IString *pIString = CIWString::CreateObject( m_pIChartCtrl->GetCurrentRQ());
	return pIString;
}

void CIWChartOCX::SetMouseWheelIndex( int nMouseWheelIndex)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetMouseWheelIndex( nMouseWheelIndex);
}

int CIWChartOCX::GetMouseWheelIndex( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetMouseWheelIndex();
}

int	CIWChartOCX::GetPriceMinMaxShowType( void)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetPriceMinMaxShowType();
}

void CIWChartOCX::SetPriceMinMaxShowType( int nMinMaxRatio)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetPriceMinMaxShowType( nMinMaxRatio);
}

int CIWChartOCX::GetFirstBlockHScaleGridType_TimeGubun( int nType)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetFirstBlockHScaleGridType_TimeGubun(nType);
}

BOOL CIWChartOCX::GetFirstBlockHScaleType_TimeGubun( int nType)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetFirstBlockHScaleType_TimeGubun(nType);
}

COLORREF CIWChartOCX::GetStyleValueHScaleGridColor_TimeGubun( int nType)
{
	if( !m_pIChartCtrl) return 0;
	return m_pIChartCtrl->GetStyleValueHScaleGridColor_TimeGubun(nType);
}

int CIWChartOCX::GetStyleValueScaleLineThickness_TimeGubun( int nType)
{
	if( !m_pIChartCtrl) return 1;
	return m_pIChartCtrl->GetStyleValueScaleLineThickness_TimeGubun(nType);
}

BOOL CIWChartOCX::SetAllHScaleGridType_TimeGubun( int p_nHScaleGridType, BOOL  p_bCheckTime, int p_nHScaleTimeType )
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetAllHScaleGridType_TimeGubun( p_nHScaleGridType,  p_bCheckTime, p_nHScaleTimeType );
}

BOOL CIWChartOCX::SetStyleValueHScaleGridColor_TimeGubun( COLORREF	p_clrHScaleGridColor,COLORREF	p_clrHScaleGrid_WeekColor,COLORREF	p_clrHScaleGrid_MonColor,COLORREF	p_clrHScaleGrid_YearColor, BOOL p_bWithUpdate)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueHScaleGridColor_TimeGubun( p_clrHScaleGridColor, p_clrHScaleGrid_WeekColor, p_clrHScaleGrid_MonColor, p_clrHScaleGrid_YearColor, p_bWithUpdate);
}

BOOL CIWChartOCX::SetStyleValueScaleLineThickness_TimeGubun(int ntype , int p_nLineThickness)
{
	if( !m_pIChartCtrl) return FALSE;
	return m_pIChartCtrl->SetStyleValueScaleLineThickness_TimeGubun(ntype, p_nLineThickness);
}
