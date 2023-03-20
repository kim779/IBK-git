// KTBChartTool.cpp: implementation of the CKTBChartTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../AddInManager/Include_AddIn/AddInCommand.h"		// for EAI_BAS_OCXELEMENTMENU
#include "../Include_Chart/Block.h"							// for CBlock
#include "OutsideCommonInterface.h"							// for _CHARTDLL_CHARTTYPE_PRICE
#include "KTBChart.h"										// for g_strOcxPath
#include "KTBChartCtl.h"									// for CKTBChartCtrl
#include "PacketList.h"										// for CPacketList
#include "MainBlock.h"										// for CMainBlock
#include "ToolBarManager.h"									// for IToolBarManager
#include "CfgFileVersionChecker.h"							// for CCfgFileVersionChecker
#include "MessageDefine.h"									// for UM_ANALYSIS_FUNCTION
#include "XScaleManager.h"

/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolFuncGraphStyle
 - Created at  :  2004-11-03   13:55
 - Author      :  ������
 - Description :  �׷������� ����
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::RunToolFuncGraphStyle(const int nParam)
{
	long lResult = 0;
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_BAS_OCXELEMENTMENU, CMenuType::MENU_DRAWSTYLE, nParam);
	return lResult;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RunToolFuncLineThickness
 - Created at  :  2004-11-03   13:55
 - Author      :  ������
 - Description :  ������ ����
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::RunToolFuncLineThickness(const int nParam)
{
	long lResult = 0;
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_BAS_OCXELEMENTMENU, CMenuType::MENU_THICKNESS, nParam);
	return lResult;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RunToolFuncGraphColor
 - Created at  :  2004-11-29   11:35
 - Author      :  ������
 - Description :  �׷��� Į�� ����
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::RunToolFuncGraphColor(COLORREF clrGraphColor)
{
	long lResult = 0;
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_BAS_OCXELEMENTMENU, CMenuType::MENU_COLOR, clrGraphColor);
	return lResult;
}

void CKTBChartCtrl::BehaviorBlock(long pBlockBehaviorData)
{
	CBlockBehaviorData* pBehaviorData = (CBlockBehaviorData*)pBlockBehaviorData;
	if( !pBehaviorData) return;
	if( m_pAddInManager)
	{
		CBlockIndex sourceindex = pBehaviorData->GetSourceBlockIndex();
		CBlockIndex targetindex = pBehaviorData->GetTargetBlockIndex();
		m_pAddInManager->OnBlockChanged( pBehaviorData->GetBlockBehavior(), sourceindex.GetRow(), sourceindex.GetColumn(), targetindex.GetRow(), targetindex.GetColumn());
	}
}

void CKTBChartCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// (2004.11.09, ��¿�) KeyBoard Push�� AddIn Module���� �����Ͽ� ó����� �Ѵ�.
	if( m_pAddInManager) m_pAddInManager->OnKeyDown( nChar, nRepCnt, nFlags);

	COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKTBChartCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	// (2004.11.09, ��¿�) KeyBoard Push�� AddIn Module���� �����Ͽ� ó����� �Ѵ�.
	if( m_pAddInManager) m_pAddInManager->OnKeyUp( nChar, nRepCnt, nFlags);
	
	COleControl::OnKeyUp(nChar, nRepCnt, nFlags);
}

// Analysis Function Message
// ��� ���� ���� �ܺ� �޼��� (���ο��� �߻�)
// (2006/1/31 - Seung-Won, Bae) Tool Command(Function) Message (Old Name : OnAnalysisFunction)
//		Bypass to RunToolFunction()
LRESULT CKTBChartCtrl::OnToolFunction( WPARAM wParam, LPARAM lParam)
{
	return RunToolFunction( ( CToolOptionInfo::TOOLOPTION)wParam, (int)lParam);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RunToolFunction
 - Created at  :  2004-11-10   09:18
 - Author      :  ������
 - Parameters  :  nParam - 1: ����, 0: ����.
 - Return Value:  bool - true: ����, false: ����.
 - Description :  ����� ���� �� ������ �Ѵ�.
 -----------------------------------------------------------------------------------------*/
// (2006/1/31 - Seung-Won, Bae) Main Routine for Tool Command(Function).
BOOL CKTBChartCtrl::_RunToolFunction( short nToolCommand, short nOption)
{
	return RunToolFunction( ( const enum CToolOptionInfo::TOOLOPTION)nToolCommand, nOption);
}
bool CKTBChartCtrl::RunToolFunction( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam)
{
	CToolOptionInfo::TOOLOPTION enumToolOption = p_eToolOption;
	int nParam = p_nParam;

	//------------------------------------------------���������� ����Ǵ� �κ�
	switch(enumToolOption)
	{
		//sy 2004.05.12.
		// -> Ư����Ʈ�� �Ϲ���ǥ�� �߰� ����� Ʋ����. 
		//    �׷��� ��� ����� <Ư����Ʈ> �������� �ϴ� ������ �־���.
		case CToolOptionInfo::T_CHART_PANDF:	// P&F
												return AddOrDeleteChart_Special( _MTEXT( C2_P_AND_F));

		case CToolOptionInfo::T_CHART_REVERSE:	// ���ð�
												return AddOrDeleteChart_Special( _MTEXT( C2_INVERSE_TIME_LINE));

		case CToolOptionInfo::T_CHART_THREELINEBREAK:	// �Ｑ��ȯ��
														return AddOrDeleteChart_Special( _MTEXT( C2_THREE_LINE_BREAK));

		case CToolOptionInfo::T_CHART_BALANCE:	// �ϸ����ǥ
												return (AddChartBlock( _MTEXT( C2_OBVIOUSLY_BALANCE_CHART)) == TRUE ? true: false);

		case CToolOptionInfo::T_CHART_BARFORSALE:	// ���Ź�
													return (AddChartBlock( _MTEXT( C2_OVERHANGING_SUPPLY)) == TRUE ? true: false);

		case CToolOptionInfo::T_CHART_RAINBOW:	// �׹���Ʈ
												return (AddChartBlock( _MTEXT( C2_RAINBOW_CHART)) == TRUE ? true: false);
		//sy end

		case CToolOptionInfo::T_RESTORE:	//sy 2004.05.12.
											//sy 2005.10.26.
											//nOption = 2 : ��ǥ �ʱ�ȭ �Ǵ� �޽��� ����.
											//OnReturnOrg2(nParam == 2 ? false: true);
											//NeoRestore();
											OnReturnOrg(nParam == 2 ? false: true);
											//sy end
											break;

		case CToolOptionInfo::T_PREV:	if(nParam != 0)
										{
											if(GetXScaleManager()->GetReverse() == FALSE)
											{
												NeoFireDataUpandDown(0);
											}
											else
											{
												NeoFireDataUpandDown(1);
											}
										}
										break;

		case CToolOptionInfo::T_NEXT:	if(nParam != 0)
										{
											if(GetXScaleManager()->GetReverse() == FALSE)
											{
												NeoFireDataUpandDown(1);
											}
											else
											{
												NeoFireDataUpandDown(0);
											}
										}
										break;

		case CToolOptionInfo::T_DAILY:
		case CToolOptionInfo::T_WEEKLY:
		case CToolOptionInfo::T_MONTHLY:	NeoRequestBongData(enumToolOption, nParam);
											break;

		case CToolOptionInfo::T_WHOLEVIEW:	SetWholeView( nParam == 1);
											break;

		case CToolOptionInfo::T_DISPLAY_RESTORE:	// Display�� ����
													OnReturnOrgDisplay();
													break;

		case CToolOptionInfo::T_MINMAX_FULLDATA:	// ��ü �ִ��ּ� ����
													OnFullMinMax(nParam);
													break;

		case CToolOptionInfo::T_RADAR:				// ���̴� ��Ʈ
													m_bRadar = !m_bRadar;
													break;

		case CToolOptionInfo::T_CHART_INVERT:	// ��Ʈ������
		{
												if(!m_pMainBlock) return false;
												int nR = 0, nC = 0, nV = 0, nG = 0;
												CString strGraphName;
												if( !m_pMainBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, strGraphName, "DEFAULT")) return FALSE;
												CBlock *pBlock = m_pMainBlock->GetBlock( CBlockIndex( nR, nC));
												if( pBlock)
												{
													pBlock->SetVScaleInvert( nV, !pBlock->IsVScaleInvert( nV));
													InvalidateControl();
												}
												break;
		}
		// (2006/3/29 - Seung-Won, Bae) Mouse Drag Tools controled by Only m_nNumericalInquiryOption.
		case CToolOptionInfo::T_NIDLG_DRAG:					SetNumericalInquiryOption( p_nParam ? m_nNumericalInquiryOption | NUMERICALINQUIRYDLG					: m_nNumericalInquiryOption & ~NUMERICALINQUIRYDLG);
															return true;
		case CToolOptionInfo::T_CROSSHAIRS_DRAG:			SetNumericalInquiryOption( p_nParam ? m_nNumericalInquiryOption | CROSSLINEVIEW							: m_nNumericalInquiryOption & ~CROSSLINEVIEW);
															return true;

		case CToolOptionInfo::T_PRINT:	
		case CToolOptionInfo::T_PRINT_CHART:	OnPrint();
												return true;
	}
	//------------------------------------------------���������� ����Ǵ� �κ� ��.

	// (2006/1/26 - Seung-Won, Bae) Do Analysis Tool Fucntion
	if( m_pAddInManager) m_pAddInManager->OnToolCommand( enumToolOption, nParam);

	return true;
}
