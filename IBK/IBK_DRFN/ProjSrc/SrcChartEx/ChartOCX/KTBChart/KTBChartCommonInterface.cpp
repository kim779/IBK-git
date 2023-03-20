
#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../AddInManager/Include_AddIn/AddInCommand.h"		// for EAI_ALL_ON_CHANGE_CURSOR_MODE
#include "KTBChartCtl.h"
#include "PacketList.h"										// for CPacketList
#include "Conversion.h"
#include "DataMath.h"
#include "ToolBarManager.h"									// for IToolBarManager
#include "OutsideCommonInterfaceFunction.h"
#include "XScaleManager.h"
#include "MainBlock.h"										// for CMainBlock

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*-----------------------------------------------------------------------------------------
 - Function    :  InvalidateOleControl
 - Created at  :  2004-11-11   09:51
 - Author      :  ������
 - Description :  OCX�� Invalidate�Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::InvalidateOleControl(const bool bIsResetBlockRegion)
{
	//sy 2006.04.24.
	if(bIsResetBlockRegion)
		m_pMainBlock->SetAllBlockRegion(GetMainBlockRegion());

	this->InvalidateControl();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetDataCountForToolBar
 - Created at  :  2004-10-28   13:56
 - Author      :  ������
 - Description :  ZoomIn,Out, Slide���� ����� GetDataCount()(protected access)�� �Լ� �����(public access).
 -----------------------------------------------------------------------------------------*/
int CKTBChartCtrl::GetDataCountForToolBar() const
{
	return GetMaxDataCount();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolBarRegion
 - Created at  :  2004-10-28   17:24
 - Author      :  ������
 - Description :  ���ٸ� ������ ������ �������ش�.
 -----------------------------------------------------------------------------------------*/
CRect CKTBChartCtrl::GetToolBarRegion() const
{
	CRect region(0, 0, 0, 0);
	GetClientRect(&region);

	// (2004.05.25, ��¿�) ���� ToolBar�� ���̰�, ����� Interface�� �����Ѵ�.
	//		������ Flag�� �ξ�, ToolBar�� OCX�� �������� �Ʒ��ʿ� ��ġ�ǵ��� �Ѵ�.
	// (2006/4/14 - Seung-Won, Bae) Add Real Toolbar Size.
	if( !m_bShowInnerToolBar && m_pToolBarManager) region.bottom += m_pToolBarManager->GetToolBarRect().Height();

	return region;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetRscFont
 - Created at  :  2004-10-29   09:29
 - Author      :  ������
 - Description :  ���ҽ���Ʈ�� �����Ѵ�. (���ٿ��� ��ġ��ȸâ, Status Bar, Radar Chart��� ���)
 -----------------------------------------------------------------------------------------*/
CFont* CKTBChartCtrl::GetRscFont() const
{
	return m_pRscFont;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetChartWindowPointer
 - Created at  :  2004-10-19   14:35
 - Author      :  ������
 - Description :  ����Dll���� ����Dialog�� �����Ҷ� �ʿ��� �θ������� �����͸� �Ѱ��ش�.
 -----------------------------------------------------------------------------------------*/
CWnd* CKTBChartCtrl::GetChartWindowPointer() const
{
	return (CWnd*)this;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetMinCountPerPage
 - Created at  :  2004-10-25   17:52
 - Author      :  ������
 - Description :  ���������� �ּ�Data������ �����ϴ� �������̽�
 -----------------------------------------------------------------------------------------*/
int CKTBChartCtrl::GetMinCountPerPage() const
{
	return m_nMinDataCountOnOnePage;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  IsWholeView
 - Created at  :  2004-11-04   13:42
 - Author      :  ������
 - Parameters  :  None
 - Return Value:  bool - true:��ü����, false:����ü����. 
 - Description :  ��ü���� ����
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::IsWholeView() const
{
	if(m_bWholeView)
		return true ;
	else
		return false ;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartEndIndex
 - Created at  :  2004-10-27   16:05
 - Author      :  ������
 - Description :  ��ũ�ѹٿ� ���� ���� Start/End Index�� ���κ�Ͽ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
// (2007/5/23 - Seung-Won, Bae) Support to Only ToolBar. (CToolBarManagerImplementation)
BOOL CKTBChartCtrl::SetStartEndIndex(const int nStartIndex, const int nEndIndex)
{
	if( IsWholeView() || !m_pxScaleManager) return FALSE;

	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartIndex, nEndIndex);

	// (2008/12/21 - Seung-Won, Bae) do not call invalidate on real processing.
	if( !m_bOnRealProcessing) InvalidateControl();

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetPageCountNextStep
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :
 -----------------------------------------------------------------------------------------*/
int CKTBChartCtrl::GetPageCountNextStep(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin, int nMax)
{
	return m_pxScaleManager->GetPageCountNextStep(nSliderPos_Prev, nSliderPos_New, nSBCode, nMin, nMax);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetReverse
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :
 -----------------------------------------------------------------------------------------*/
//BOOL CKTBChartCtrl::GetReverse()
//{
//	return m_pxScaleManager->GetReverse();
//}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetStateWholeView
 - Created at  :  2004-11-09   16:56
 - Author      :  ������
 - Description :  ��Ʈ�� ��ü���� ���¸� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetStateWholeView(const int nParam /* = 0 */)
{
	if(nParam == 1)
		m_bWholeView = TRUE;
	else
		m_bWholeView = FALSE;
}

double CKTBChartCtrl::ConvertYPositionToData(const int nYPosition, const int nYPositionTop,
			const int nYPositionBottom, const double& dDataMax, const double& dDataMin, const bool bLog, const bool bReverse)
{
	double dMax = dDataMax;
	double dMin = dDataMin;

	// 1. Check Reverse (Swap Data Min/Max)
	if( bReverse)
	{
		double dData	= dMax;
		dMax			= dMin;
		dMin		= dData;
	}

	// 2. Make Log Data
	if( bLog)
	{
		dMax = CMath::Log( dMax);
		dMin = CMath::Log( dMin);
	}

	// 3. Get Data
	double dData = CDataConversion::AbsoluteToRelative_Y( CRect( 0, nYPositionTop, 0, nYPositionBottom),
		nYPosition, dMin, dMax);

	// 4. Check Log
	if( bLog) return CMath::Exp( dData);
	return dData;
}


//////////////////////////////////////////////////////////////////////////
// IndicatorConfiguration.dll��.

/*-----------------------------------------------------------------------------------------
 - Function    :  InvalidateOCX
 - Created at  :  2005-01-21   16:57
 - Author      :  ������
 - Description :  OCX�� Invalidate�Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::InvalidateOCX(bool bErase /* = true */)
{
	this->Invalidate((bErase? TRUE:FALSE));
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RestoreChartCfg
 - Created at  :  2005-01-21   17:11
 - Author      :  ������
 - Parameters  :  nIndex - �����ϰ��� �ϴ� ��Ʈȭ���� �ε���
 - Return Value:  bool - true:��������, false:��������
 - Description :  ���ڰ��� �ε����� �ش��ϴ� ��Ʈȭ������ �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::RestoreChartCfg(const int nIndex)
{
	CChartCfg* pChartCfg = m_SaveChartcfg.GetChartCfg(nIndex, true);
	
	if(!pChartCfg)
		return false;
	return ResetSaveChartCfg(pChartCfg, false);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  AddRemoveGraph
 - Created at  :  2005-01-24   09:58
 - Author      :  ������
 - Description :  ��ǥ�׷��� �߰�/������ �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::AddRemoveGraph(LPCTSTR pcszGraphName)
{
	return AddChartBlock(pcszGraphName) ? true : false;
}

void CKTBChartCtrl::SetGraphSelectedDataIndex(int nDataIndex, LPCTSTR pcszGraphName)
{
	CString strGraphName = pcszGraphName;

	// �ش� index�� ��¥�� ���´�
	int nDate = (int)m_pPacketList->GetDataFromIndex( _MTEXT( C0_DATE_TIME), nDataIndex);
	CString strDate = CDataConversion::IntToString(nDate);
	// news �϶� �̺�Ʈ �߻�
	if(strGraphName == "News")
		NeoRequestMapLoad(3, "", strDate);
	// �ŷ������϶� �̺�Ʈ �߻�
	else if(strGraphName == _MTEXT( C2_DEAL_LIST))
		NeoRequestMapLoad(4, "", strDate);
}

/*
void CKTBChartCtrl::SetGraphMargin( LPCTSTR pcszIndicatorName, const bool bIsAddMargin)
{
	if(m_pIndicatorList == NULL) return ;

	int nMargin = GetMarginInIndicatorInfo( pcszIndicatorName);
	// (2004.04.19, ��¿�) Margin ������ 0������ ���, Margin�� ���� 0���� �����Ͽ�,
	//		�������� ������ �Ѵ�.
	if(nMargin <= 0) nMargin = 0;

	// margin �߰�
	if(bIsAddMargin)	m_nGlanceBalanceMargin = nMargin;
	// margin ����
	else				m_nGlanceBalanceMargin = 0;

	// ����(04/11/16) ���ǹ� ����
	// ����(04/09/07) SetScrollData�κ� ����
	SetScrollData(SETSCROLL_TR);
	InvalidateControl();
}
*/

void CKTBChartCtrl::SetCursorExchange(CCursorShape::CHARTOBJECT enumChartObject)
{
	// 1. ��û�� Cursor Type�� ���� �˸��� Cursor�� Loading�Ͽ� ����Ѵ�.
	if( enumChartObject == CCursorShape::OBJECT_NONE)
	{
		if( m_hCursor) 
		{
			TRACE( "\r\nCursor cleared in CKTBChartCtrl::SetCursorExchange()");
		}
		m_hCursor = NULL;
	}
	else
	{
		// 1.1  ��û�� Cursor Type�� �´� Resource ID�� Ȯ���Ѵ�.
		WORD wCursorID = 0;
		switch(enumChartObject)
		{
			case CCursorShape::BLOCKPOS_SPLITH:	
				wCursorID = IDC_SPLITV;
				break;
			case CCursorShape::BLOCKPOS_SPLITV:	
				wCursorID = IDC_SPLITH;
				break;
			case CCursorShape::BLOCKPOS_SPLITA:	
				wCursorID = IDC_SPLITA;
				break;
			case CCursorShape::OBJECT_SELECT:	
				wCursorID = IDC_HANDLE;
				break;
			case CCursorShape::OBJECT_MOVE:		
				wCursorID = IDC_GRAB;
				break;
			case CCursorShape::VIEW_ZOOM_IN:			
				wCursorID = IDC_ZOOMIN;
				break;
			case CCursorShape::WHOLE_VIEW:		
				wCursorID = IDC_WHOLEVIEW;
				break;
			case CCursorShape::TOOL_DRAW:		
				wCursorID = IDC_PEN;
				break;
			case CCursorShape::TOOL_SIZEALL:	
				wCursorID = IDC_MOVEALL;
				break;
			case CCursorShape::TOOL_SIZENESW:	
				wCursorID = IDC_MOVENESW;
				break;
			case CCursorShape::TOOL_NODROP:		
				wCursorID = IDC_NODROP;
				break;
			case CCursorShape::OBJECT_REMOVE:		
				wCursorID = IDC_REMOVE;
				break;
			// Graph �и�/���� - ojtaso (20070731)
			case CCursorShape::OBJECT_INSERT:		
				wCursorID = IDC_INSERT;
				break;
		}
		// 1.2 Cursor Resource�� Loading�Ѵ�.
		ML_SET_DEFAULT_RES();
		m_hCursor = ::LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( wCursorID));
		TRACE( "\r\nCursor Changed in CKTBChartCtrl::SetCursorExchange() : [%d]", m_hCursor);
	}

	// 2. Loading�� Cursor�� �����Ѵ�.
	// (2006/10/31 - Seung-Won, Bae) Do not call OnSetCursor() directly
	SendMessage( WM_SETCURSOR, ( WPARAM)GetSafeHwnd(), 0);
}

int  CKTBChartCtrl::GetUserMargin(void) const
{
	return m_nBongCountMargin;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetDefaultPath
 - Created at  :  2005-08-02
 - Author      :  �輱��
 - Description :  default ���
 -----------------------------------------------------------------------------------------*/
CString CKTBChartCtrl::GetDefaultPath()
{
	return (GetBaseDeskPath() + DATA_FILE_FOLDER);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetUserPath
 - Created at  :  2005-08-02
 - Author      :  �輱��
 - Description :  user ���
 -----------------------------------------------------------------------------------------*/
CString	CKTBChartCtrl::GetUserPath()
{
	return GetUserFilePath();
}

// (2009/9/2 - Seung-Won, Bae) for Image Path
CString CKTBChartCtrl::GetImagePath( void)
{
	CString strImagePath( "\\image\\chart\\");
	
	// (2009/9/5 - Seung-Won, Bae) Support Old MChart Mode
	char szMChartMode[ 256];
	DWORD dwError = GetPrivateProfileString( "Chart Version", "MChartMode", "", szMChartMode, 255, GetDefaultPath() + "ChartMetaTable.dat");
	CString strChartMode( szMChartMode);
	if(			strChartMode == "FX")		strImagePath = "\\System\\Image\\";
	else if(	strChartMode == "WORLDON")	strImagePath = "\\image\\chart_img\\";
	else if(	strChartMode == "HTS")		strImagePath = "\\image\\";

	//return GetBaseDeskPath() + strImagePath;
	//return GetBaseDeskPath() + "\\Hanaro" + strImagePath;	//@Solomon:Directory
	return GetBaseDeskPath() + "\\Icss" + strImagePath;	//IBK
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetNumericalInquiryOption
 - Created at  :  2005-07-14   20:59
 - Author      :  ������
 - Description :  Mouse Drag Mode Info Flag
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetNumericalInquiryOption( const long p_nNumericalInquiryOption, const bool bOutside)
{
	m_nNumericalInquiryOption = p_nNumericalInquiryOption;

	// (2006/2/28 - Seung-Won, Bae) in Drag Zoom Flag (Not NIDLG Flag), Clear Cross Line Flag.
	// (2006/3/3 - Seung-Won, Bae) Do not check. Because It can use with Script and Inner ToolBar's Controlling.
	//		So check in EnvSetup DLL
//	if( !( m_nNumericalInquiryOption & NUMERICALINQUIRYDLG)) m_nNumericalInquiryOption &= ~CROSSLINEVIEW;

	// (2006/2/9 - Seung-Won, Bae) Notify AddIn Tools
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_ALL_ON_CHANGE_CURSOR_MODE, m_nNumericalInquiryOption);

	// (2006/3/4 - Seung-Won, Bae) Notify to ToolBar for UI
	if( m_pToolBarManager) m_pToolBarManager->OnNumericalInquiryOptionChanged( m_nNumericalInquiryOption);

//	if(bOutside)
//		m_OutsideCommonInterfaceFunction.MChart_SetInquiryDataWindowTypeToOutside(nWindowType);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetShowTrendLineValue
 - Created at  :  2005-07-14   20:58
 - Author      :  ������
 - Description :  �߼����� ǥ�� 
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetShowTrendLineValue(const int nPosition, const bool bOutside)
{
	if(bOutside)
		m_OutsideCommonInterfaceFunction.MChart_SetShowTrendLineValueToOutside(nPosition);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetAutoSave
 - Created at  :  2005-07-14   20:58
 - Author      :  ������
 - Description :  ��Ʈ����� �ڵ� ���� ����
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetAutoSave(const bool bAutoSave, const bool bOutside)
{
	if(bOutside)
		m_OutsideCommonInterfaceFunction.MChart_SetAutoSaveToOutside(bAutoSave);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetShowMinMaxPrice
 - Created at  :  
 - Author      :  
 - Description :  �ְ�/������ǥ��
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetShowMinMaxPrice(const bool bShow, const bool bOutside)
{
	//���� �߰�
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetImaginaryCandle
 - Created at  :  2005-07-18   18:38
 - Author      :  ������
 - Description :  ���������
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::GetImaginaryCandle()
{
	/*bool bImaginaryCandle = false;
	if(!m_OutsideCommonInterfaceFunction.MChart_GetImaginaryCandleToOutside(bImaginaryCandle))
		return m_bNormalizedMinuteTimeRule;

	return bImaginaryCandle;*/
	return m_bNormalizedMinuteTimeRule;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetBongCountMargin
 - Created at  :  2005-07-18   18:39
 - Author      :  ������
 - Description :  ������ ����
 -----------------------------------------------------------------------------------------*/
int	CKTBChartCtrl::GetBongCountMargin()
{
	return m_nBongCountMargin;

	/*int nMargin = 0;
	bool bResult = m_OutsideCommonInterfaceFunction.MChart_GetRightMarginToOutside(nMargin);

	if(false == bResult)
		nMargin = m_nBongCountMargin;
	return nMargin;*/
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetShowIndicatorValue
 - Created at  :  2005-07-18   18:40
 - Author      :  ������
 - Description :  ��ǥ���� - ��ǥ�� ǥ��
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::GetShowIndicatorValue()
{
	return (m_bUseDatatoTitle ? true : false);

	/*bool bShowIndicatorValue = true;
	bool bResult = m_OutsideCommonInterfaceFunction.MChart_GetShowIndicatorValueToOutside(bShowIndicatorValue);

	if(false == bResult)
		bShowIndicatorValue = m_bUseDatatoTitle ? true : false;
	return bShowIndicatorValue;*/
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetShowTrendLineValue
 - Created at  :  2005-07-18   18:41
 - Author      :  ������
 - Description :  �߼����� ǥ��(0: , 1: , 2: , 3: )
 -----------------------------------------------------------------------------------------*/
int	 CKTBChartCtrl::GetShowTrendLineValue()
{
	// ��ô��!
	return 1;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetAutoSave
 - Created at  :  2005-07-18   18:42
 - Author      :  ������
 - Description :  ��Ʈ����� �ڵ� ���� ����
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::GetAutoSave()
{
	bool bAutoSave = true;
	bool bResult = m_OutsideCommonInterfaceFunction.MChart_GetAutoSaveToOutside(bAutoSave);
	return bAutoSave;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetShowMinMaxPrice
 - Created at  :  
 - Author      :  
 - Description :  �ְ�/������ ǥ��
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::GetShowMinMaxPrice()
{
	//���� �߰�
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  InvalidateRect
 - Created at  :  2005-07-14   20:58
 - Author      :  
 - Description :  
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::InvalidateRect(const CRect& region, const bool  bErase)
{
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SaveChartCfgInCurrentOCX
 - Created at  :  2005-08-02
 - Author      :  �輱��
 - Description :  ȯ�漳�� ���� �Լ� - ���� ocx�� ȯ���� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::SaveChartCfgInCurrentOCX()
{
	return SaveChartCfg();
}

void CKTBChartCtrl::SetGraphTypeStyle(int nGraphType, int nDrawStyle)
{
	if(!m_pToolBarManager)
		return;
	if(!m_pToolBarManager->GetToolOption(CToolOptionInfo::T_GRAPH_STYLE))
		return;

	m_pToolBarManager->SetGraphType((CGraphBaseData::GRAPHTYPE) nGraphType, nDrawStyle);
}

void CKTBChartCtrl::SetGraphThickness(int nGraphThickness1)
{
	if(!m_pToolBarManager)
		return;	
	if(!m_pToolBarManager->GetToolOption(CToolOptionInfo::T_GRAPH_THICKNESS))
		return;

	m_pToolBarManager->SetLineThickness(nGraphThickness1);
}

//sy 2006.04.20.
// head ~ index������ �����Ѵ�
bool CKTBChartCtrl::RemoveChartCfgFromIndexToEnd(const int nTailIndex)
{
	return m_SaveChartcfg.RemoveFromIndexToEnd(nTailIndex);
}
//sy end
