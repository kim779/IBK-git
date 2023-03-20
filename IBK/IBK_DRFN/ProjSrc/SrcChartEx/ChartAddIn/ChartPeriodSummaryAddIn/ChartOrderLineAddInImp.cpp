// ChartOrderLineAddInImp.cpp: implementation of the CChartOrderLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "ChartOrderLineAddInImp.h"

#include "./Include_Chart/Dll_Load/IMetaTable.h"			// for g_iMetaTable
#include "./Include_Chart/MessageDefine.h"					// for UM_CURSOR_EXCHANGE
#include "./Include_Chart/Conversion.h"
#include "./Include_AddIn/Include_AddIn.h"					// for Ixxxxxx.h
#include "./Include_AddIn/I133691/_IPacketListManager.h"	// for IPacketListManager

#include "OrderLineConfigDlg.h"
#include "OrderLineManDlg.h"
#include "DlgConfirm.h"
#include "LineObj.h"									// for CLineObj

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartOrderLineAddInImp::m_strAddInItemName = "ORDER_LINE";

// (2010/4/17 - Seung-Won, Bae) Make it static to share the Order Line Info within all Chart.
CStringList	CChartOrderLineAddInImp::m_slSavedOrderLines;		// ����� ��� �ֹ���
CTypedPtrList< CObList, CChartOrderLineAddInImp *>	CChartOrderLineAddInImp::m_alOrderLineAddInManager;

typedef struct
{
	CDialog* pDlgConfirm;
	CLineObj* pLine;
}stPROCEEDSIGN;

// UINT THREAD_PROCEEDSIGN(LPVOID lpVoid)
// {
// 	CLineObj* pLine = (CLineObj*)lpVoid;
// 	if(pLine)
// 	{
// 		_MHWND( NULL);
// 		ML_SET_LANGUAGE_RES();
// 		CDlgConfirm	dlg;
// 		if(pLine->m_nState == STAT_UP)
// 		{
// 			if(pLine->m_bUpBuy)
// 				dlg.SetData(pLine->m_strName, pLine->m_nState, 2);
// 			else if(pLine->m_bUpSell)
// 				dlg.SetData(pLine->m_strName, pLine->m_nState, 1);
// 		}
// 		else
// 		{
// 			if(pLine->m_bDownBuy)
// 				dlg.SetData(pLine->m_strName, pLine->m_nState, 2);
// 			else if(pLine->m_bDownSell)
// 				dlg.SetData(pLine->m_strName, pLine->m_nState, 1);
// 		}
// 			
// 		int nRet = dlg.DoModal();
// 	}
// 	return 0;
// }

UINT THREAD_PROCEEDSIGN(LPVOID lpVoid)
{
	stPROCEEDSIGN* pThreadInfo = (stPROCEEDSIGN*)lpVoid;
	CLineObj* pLine = (CLineObj*)pThreadInfo->pLine;
	if(pLine)
	{
//		_MHWND( NULL);
//		ML_SET_LANGUAGE_RES();
		CDlgConfirmList* pDlg = (CDlgConfirmList*)pThreadInfo->pDlgConfirm;

		if(pLine->m_nState == STAT_UP)
		{
			if(pLine->m_bUpBuy)
				pDlg->ExSetData(pLine->m_strName, pLine->m_nState, 2);
			else if(pLine->m_bUpSell)
				pDlg->ExSetData(pLine->m_strName, pLine->m_nState, 1);
		}
		else
		{
			if(pLine->m_bDownBuy)
				pDlg->ExSetData(pLine->m_strName, pLine->m_nState, 2);
			else if(pLine->m_bDownSell)
				pDlg->ExSetData(pLine->m_strName, pLine->m_nState, 1);
		}
		
		//int nRet = dlg.DoModal();
		pDlg->ShowWindow(SW_SHOW);
	}
	delete pThreadInfo;
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CChartOrderLineAddInImp::CChartOrderLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_nDateType			= 0;		// ��¥ Ÿ��, ���غ�

	m_bLog				= FALSE;	// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	m_bReverse			= FALSE;	// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	m_dViewMin			= 0.0;		// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	m_dViewMax			= 0.0;		// View���� ��� Graph�� View�� Maximum Scale ���̴�.
	m_nStartIndex		= 0;
	m_nEndIndex			= 0;

	m_nCommandType		= 2;		// Tool State Mode
	m_dwMouseActionID	= 0;		// Current Mouse Action ID
	m_pCurSelectedLine	= NULL;

	m_hOcxWnd			= NULL;

	m_pDlgConfirm		= NULL;		//@Solomon100319

	// (2010/4/17 - Seung-Won, Bae) Make it static to share the Order Line Info within all Chart.
	m_alOrderLineAddInManager.AddTail( this);
}

CChartOrderLineAddInImp::~CChartOrderLineAddInImp()
{
	// (2010/4/17 - Seung-Won, Bae) Make it static to share the Order Line Info within all Chart.
	POSITION psnAddIn = m_alOrderLineAddInManager.Find( this);
	if( psnAddIn) m_alOrderLineAddInManager.RemoveAt( psnAddIn);

	//@Solomon100319
	if(m_pDlgConfirm) delete m_pDlgConfirm; m_pDlgConfirm=NULL;

	DeleteAll();
}

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartOrderLineAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnRealPacketEnd)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnKeyDown)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnLButtonDblClk)
	ONADDINEVENT( OnRButtonDown)
	ONADDINEVENT( OnRButtonUp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
END_ADDIN_EVENT_MAP()


const LPSTR szDateType[] =
{
	 "��","��","��","��","��","ƽ"
};

//////////////////////////////////////////////////////////////////////
// Chart Data and View Base Info Collecting
//////////////////////////////////////////////////////////////////////

// Chart View Info Collection
BOOL CChartOrderLineAddInImp::GetChartBasicData( void)
{
	// �� Data View�� Start/End Index�� ���Ѵ�.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex)) return FALSE;

	int nR = 0, nC = 0, nV = 0, nG = 0;
	// ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
	if( !m_pIChartManager->GetIndexOfGraph( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG)) return FALSE;

	// Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( pIBlock)
	{
		m_bLog = pIBlock->IsLogScale();
		m_bReverse = pIBlock->IsVScaleInvert( nV);

		pIBlock->GetVScaleGroupMinMax( nV, m_dViewMin, m_dViewMax);
	   	pIBlock->GetGraphRegions( m_rctGraphRegion, m_rctBlockRegion);
		m_rctGraphDrwingRegion = pIBlock->GetGraphDrwingRegion();

		pIBlock->Release();
		pIBlock = NULL;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Current Tool State
//////////////////////////////////////////////////////////////////////

// Selection Change
CLineObj *CChartOrderLineAddInImp::ChangeSelection( CLineObj *p_pNewSelection)
{
	// Release Prev Selection
	CLineObj *pPrevSelection = m_pCurSelectedLine;
	if( m_pCurSelectedLine) m_pCurSelectedLine->m_bSelected = FALSE;
	// Set New Selection
	m_pCurSelectedLine = p_pNewSelection;
	if( m_pCurSelectedLine) m_pCurSelectedLine->m_bSelected = TRUE;

	return pPrevSelection;
}

// Tool Mode Change
BOOL CChartOrderLineAddInImp::ChangeToolMode( int p_nNewMode)
{
	if( m_nCommandType == p_nNewMode) return FALSE;
	CLineObj *pPrevSelection = ChangeSelection( NULL);
	m_nCommandType = p_nNewMode;
	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINE, m_nCommandType == 1);
	return ( BOOL)pPrevSelection;
}

//////////////////////////////////////////////////////////////////////
// Select
//////////////////////////////////////////////////////////////////////

BOOL CChartOrderLineAddInImp::SelectLine( POINT &p_pt, DWORD p_dwMouseActionID)
{
	CLineObj *pNewSelection = NULL;
	POSITION posCurrent = NULL;
	POSITION pos = m_LineList.GetHeadPosition();
	while( pos)
	{
		posCurrent = pos;
		pNewSelection = m_LineList.GetNext( pos);
		if( pNewSelection->IsInRegion( p_pt))
		{
			m_dwMouseActionID = p_dwMouseActionID;
			pNewSelection->ButtonDownAt( p_pt);
			m_LineList.RemoveAt( posCurrent);
			m_LineList.AddTail( pNewSelection);

			m_ptCursor = p_pt;
			break;
		}
		pNewSelection = NULL;
	}

	// DrawCursor()�� �Ҹ��� ���� ������ ���� ����� ����.... 
	if( ChangeSelection( pNewSelection)) m_pIChartOCX->InvalidateControl();
	return ( BOOL)pNewSelection;
}

//////////////////////////////////////////////////////////////////////
// Delete
//////////////////////////////////////////////////////////////////////

// Delete Selected Object
BOOL CChartOrderLineAddInImp::DeleteSelection( BOOL p_bUnregisterOrderManager)
{
	CLineObj *pPrevSelection = ChangeSelection( NULL);
	if( !pPrevSelection) return FALSE;

	POSITION pos = m_LineList.Find( pPrevSelection);
	if( !pos) return TRUE;	// (2008/3/26 - Seung-Won, Bae) for Clear Grip ?

	m_LineList.RemoveAt(pos);
	if( p_bUnregisterOrderManager) UnregPS( ( long)pPrevSelection);
	delete pPrevSelection;

	return TRUE;
}

// Delete All Object
BOOL CChartOrderLineAddInImp::DeleteAll( void)
{
	BOOL bResult = !m_LineList.IsEmpty();
	ChangeSelection( NULL);
	CLineObj *pLine = NULL;
	while( !m_LineList.IsEmpty())
	{
		pLine = m_LineList.RemoveHead();

		//@Solomon100218-alzioyes. �ֹ��� �ִ� ���¿��� ȭ������� �״���������.
		//if(pLine) UnregPS( ( long)pLine);
		delete pLine;
	}
	return bResult;
}

void CChartOrderLineAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( strcmp(lpszRQ, "DEFAULT")) return;
	m_strRQ = lpszRQ;	// 20081107 ����ǥ

	// Load Chart Data Info II
	m_nDateType = m_pIDefaultPacketManager->GetBaseTimeDataType();
	m_strDateType = szDateType[ m_nDateType];

	LoadOrderLine();	// ���� ����� �ֹ��� �ε�	
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CChartOrderLineAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	if( strcmp( p_szRQ, "DEFAULT")) return;
	m_strRQ = p_szRQ;	// 20081107 ����ǥ

	IPacket *pIPacketForCur = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( pIPacketForCur)
	{
		OnUpdatePacketDataMultiItem2( pIPacketForCur);
		pIPacketForCur->Release();
	}
}

void CChartOrderLineAddInImp::OnUpdatePacketDataMultiItem2( IPacket *pIPacketForCur)
{
	int nUPDOWN = 0;
	CPoint ptPrev, ptCur;
	double dCur = 0, dPrev = 0, dCurOnLine = 0, dPrevOnLine = 0;
	int nIndex = pIPacketForCur->GetDataCount() - 1;

	if( !pIPacketForCur->GetData( nIndex - 1, dPrev)) return;	
	if( !pIPacketForCur->GetData( nIndex, dCur)) return;	

	CLineObj *pLine = NULL;
	POSITION pos = m_LineList.GetHeadPosition();
	while( pos)
	{
		pLine = m_LineList.GetNext( pos);

		// Previous
		ptPrev.x = GetXPosition( nIndex - 1);
		ptPrev.y = GetYPositionOnLine( ( int&)ptPrev.x, pLine->m_ptAStart, pLine->m_ptAEnd);
		dPrevOnLine = m_pIChartOCX->ConvertYPositionToData( ptPrev.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

		// Current
		ptCur.x = GetXPosition( nIndex);
		ptCur.y = GetYPositionOnLine( ( int&)ptCur.x, pLine->m_ptAStart, pLine->m_ptAEnd);
		dCurOnLine = m_pIChartOCX->ConvertYPositionToData( ptCur.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

		if( ( dPrevOnLine > dPrev) && ( dCurOnLine < dCur))			// ���� ����
			nUPDOWN = UP;
		else if( ( dPrevOnLine < dPrev) && ( dCurOnLine > dCur))	// ���� ����
			nUPDOWN = DOWN;

		if( pLine->m_nState == STAT_DOWN && nUPDOWN != DOWN && dCur > pLine->m_dAlertPrice)
			pLine->m_nState = STAT_NONE;

		if(pLine->m_nState == STAT_UP && nUPDOWN != UP && dCur < pLine->m_dAlertPrice)
			pLine->m_nState = STAT_NONE;

		if( nUPDOWN == UP)	//��½�
		{
			if( pLine->m_nState == STAT_NONE)
			{
				pLine->m_dAlertPrice = dCur;
				pLine->m_nState = STAT_UP;
				if( pLine->m_bUpBuy || pLine->m_bUpSell)
				{
					// 2008.09.01 ������  �ֹ� ���� ������ �׳� Contianer�� Event�� ������ �޼��� �ڽ��� �ȶ��� ����
					//		�ʿ��ϸ� Container���� �ϵ��� �Ѵ�.	<<
					//      && ���� ������ ������ �ʾҴµ�, arg4�� ���ݵ� ������ ����
					switch( m_pIChartOCX->GetChartMode())
					{
						//case CChartInfo::HTS:	AfxBeginThread(THREAD_PROCEEDSIGN, pLine);
						case CChartInfo::HTS:
							{
								//@Solomon100319
								if(!m_pDlgConfirm)
								{
									_MHWND( NULL);
									ML_SET_LANGUAGE_RES();

									HWND hOcx = m_pIChartOCX->GetOcxHwnd();
									CDlgConfirmList *pDlgConfirm = new CDlgConfirmList;
									pDlgConfirm->Create(CDlgConfirmList::IDD, CWnd::FromHandle(hOcx));
									m_pDlgConfirm = (CDialog*)pDlgConfirm;
								}
								stPROCEEDSIGN *pstInfo = new stPROCEEDSIGN;
								pstInfo->pDlgConfirm = m_pDlgConfirm;
								pstInfo->pLine = pLine;
								//AfxBeginThread(THREAD_PROCEEDSIGN, pLine);
								AfxBeginThread(THREAD_PROCEEDSIGN, pstInfo);
								
							}
							break;
						case CChartInfo::FX:	if( pLine->m_bUpBuy)
													m_pIChartOCX->FireAddInEvent( "ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, pLine->m_dAlertPrice, 1/*0:�ŵ�, 1:�ż�*/);
												else
													m_pIChartOCX->FireAddInEvent( "ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, pLine->m_dAlertPrice, 0/*0:�ŵ�, 1:�ż�*/);
												break;
						case CChartInfo::WORLD_ON:
						{
							// 20081107 ����ǥ >>
							CString strPacketType;
							IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( m_strRQ);
							if( pIPacketManager)
							{
								ILPCSTR strType;
								IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT( C0_CLOSE));
								if( pIPacket) strType = pIPacket->GetType();
								pIPacket->Release();
								pIPacketManager->Release();
								strPacketType = strType;
							}
							CString strPrice;
							strPrice = CDataConversion::DoubleToString( pLine->m_dAlertPrice, strPacketType);
			
							CString strData;
							strData.Format("%s,%s", m_strCode, strPrice);
							// 20081107 ����ǥ <<
							if( pLine->m_bUpBuy)
								m_pIChartOCX->FireAddInEvent( "ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, strData, 0, 1/*0:�ŵ�, 1:�ż�*/);
							else
								m_pIChartOCX->FireAddInEvent( "ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, strData, 0, 0/*0:�ŵ�, 1:�ż�*/);
							break;
						}
					}
				}
			}
		}
		else if( nUPDOWN == DOWN)	//��½�
		{
			if( pLine->m_nState == STAT_NONE)
			{
				pLine->m_dAlertPrice = dCur;
				pLine->m_nState = STAT_DOWN;
				if( pLine->m_bDownBuy || pLine->m_bDownSell)
				{
					// 2008.09.01 ������  �ֹ� ���� ������ �׳� Contianer�� Event�� ������ �޼��� �ڽ��� �ȶ��� ����
					//		�ʿ��ϸ� Container���� �ϵ��� �Ѵ�.	<<
					//      && ���� ������ ������ �ʾҴµ�, arg4�� ���ݵ� ������ ����
					switch( m_pIChartOCX->GetChartMode())
					{
						//case CChartInfo::HTS:	AfxBeginThread( THREAD_PROCEEDSIGN, pLine);
						case CChartInfo::HTS:
							{
								//@Solomon100319
								if(!m_pDlgConfirm)
								{
									_MHWND( NULL);
									ML_SET_LANGUAGE_RES();

									HWND hOcx = m_pIChartOCX->GetOcxHwnd();
									CDlgConfirmList *pDlgConfirm = new CDlgConfirmList;
									pDlgConfirm->Create(CDlgConfirmList::IDD, CWnd::FromHandle(hOcx));
									m_pDlgConfirm = (CDialog*)pDlgConfirm;
								}
								stPROCEEDSIGN *pstInfo = new stPROCEEDSIGN;
								pstInfo->pDlgConfirm = m_pDlgConfirm;
								pstInfo->pLine = pLine;
								//AfxBeginThread(THREAD_PROCEEDSIGN, pLine);
								AfxBeginThread(THREAD_PROCEEDSIGN, pstInfo);
							}
							break;
						case CChartInfo::FX:	if(pLine->m_bDownBuy)
													m_pIChartOCX->FireAddInEvent( "ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, pLine->m_dAlertPrice, 1/*0:�ŵ�, 1:�ż�*/);
												else
													m_pIChartOCX->FireAddInEvent( "ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, m_strCode, pLine->m_dAlertPrice, 0/*0:�ŵ�, 1:�ż�*/);
												break;
						case CChartInfo::WORLD_ON:
						{
							// 20081107 ����ǥ >>
							CString strPacketType;
							IPacketManager *pIPacketManager = m_pIPacketListManager->GetPacketManager( m_strRQ);
							if( pIPacketManager)
							{
								ILPCSTR strType;
								IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT( C0_CLOSE));
								if( pIPacket) strType = pIPacket->GetType();
								pIPacket->Release();
								pIPacketManager->Release();
								strPacketType = strType;
							}
							CString strPrice;
							strPrice = CDataConversion::DoubleToString( pLine->m_dAlertPrice, strPacketType);
							
							CString strData;
							strData.Format("%s,%s", m_strCode, strPrice);
							// 20081107 ����ǥ <<
							if(pLine->m_bDownBuy)
								m_pIChartOCX->FireAddInEvent( "ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, strData, 0, 1/*0:�ŵ�, 1:�ż�*/);
							else
								m_pIChartOCX->FireAddInEvent( "ChartPeriodSummaryAddIn.dll:ORDER_LINE", 1, strData, 0, 0/*0:�ŵ�, 1:�ż�*/);
							break;
						}
					}
				}
			}
		}
	}
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CChartOrderLineAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;

	// Tool Mode Change - Object Creattion Mode Reset.
	if(eToolOption == CToolOptionInfo::T_ORDER_LINE)// �ֹ���
	{
		BOOL bResult = FALSE;
		if( p_nCmdOption)	bResult = ChangeToolMode( 1);
		else			bResult = ChangeToolMode( 2);
		if( bResult) m_pIChartOCX->InvalidateControl();
		return;
	}

	if( eToolOption == CToolOptionInfo::T_TOOL_SELECTION || eToolOption == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{
		if( ChangeToolMode( 2)) m_pIChartOCX->InvalidateControl();
		return;
	}

	if( eToolOption == CToolOptionInfo::T_DELETE_ALL )
	{
		DeleteAll();
		return;
	}

	if( eToolOption == CToolOptionInfo::T_ORDER_LINE_MANAGER)// �ֹ��� ����
	{
		OnOLMan();
		return;
	}

	if( eToolOption == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ORDER_LINE, m_nCommandType == 1);		// T_ORDER_LINE ���� ��ü �ʿ�
		return;
	}
}

BOOL CChartOrderLineAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if( p_eCommandID == EAI_TOOLORDERMGR_DELETE_THIS_TOOL && !strcmp( p_szParam, "ORDER_LINE") && p_lParam)
	{
		BOOL bResult = ( BOOL)ChangeSelection( ( CLineObj *)p_lParam);
		if( DeleteSelection( FALSE)) bResult = TRUE;
		if( bResult) m_pIChartOCX->InvalidateControl();
		return TRUE;
	}
	// 20081007 JS.Kim	����ڽð��� ����
	else if( p_eCommandID == EAI_USER_CHANGE_TIMEDIFF )
	{
		RecalcLineXPoint(p_szParam, (int)p_lParam);
		return FALSE;		// �ٸ� AddIn������ ó���� �� �ֵ��� FALSE�� return�Ѵ�.
	}

	return FALSE;
}

void CChartOrderLineAddInImp::OnResetChart()
{
	ChangeToolMode( 2);
	LoadOrderLine();	// ���� ����� �ֹ��� �ε�
}

BOOL CChartOrderLineAddInImp::OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty()) return FALSE;
	if( !m_nCommandType) return FALSE;

	CRect rctRegion = m_pIChartManager->GetGraphRegionSumInColumn( point);
	if( rctRegion.IsRectEmpty()) return FALSE;
	if( !m_rctBlockRegion.PtInRect( point)) return FALSE;

	if( m_nCommandType == 1)	// drawing
	{
 		CPoint ptAt = point;
 		int ix = 0;
 		if( GetIndexFromPoint(ptAt, ix) == R_ERROR) return FALSE;

		if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
		m_dwMouseActionID = p_dwMouseActionID;

		m_ptCursor = point;
		CLineObj* pLine = new CLineObj( m_hOcxWnd);
 		pLine->m_Num = m_LineList.GetCount();
		pLine->m_nDateType = m_nDateType;
		pLine->m_strCode = m_strCode;		// ���� �ڵ�
		pLine->m_strName = m_strCodeName;	// �����
		pLine->m_strCandle = m_strDateType;	// ���غ�
		pLine->GenerateSerial();

 		pLine->ButtonDownAt(ptAt);
		pLine->m_CurClickedPosition = E_PT;
 		pLine->SetPos(ptAt, E_PT);
		
		ChangeSelection( pLine);

		// 20081007 JS.Kim	����ڽð��� ���� <<
		IPacket *pPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
		if( pPacket) 
		{
			pLine->m_nTimeDiff = pPacket->GetBaseTimeDifference();
			pPacket->Release();
		}
		//>>

		m_LineList.AddTail(pLine);
		RegPS( ( long)pLine);
		return TRUE;
	}
	
	if( m_nCommandType == 2)
	{
		if( SelectLine( point, p_dwMouseActionID))
		{
			if( m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase))
				m_dwMouseActionID = p_dwMouseActionID;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CChartOrderLineAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase)) return FALSE;
	if( p_dwMouseActionID != m_dwMouseActionID) return TRUE;
	if( !m_nCommandType) return TRUE;
  	if( !m_pCurSelectedLine) return TRUE;
	if( !( MK_LBUTTON & nFlags)) return TRUE;

	CPoint pt = point;
	if( !m_rctGraphRegion.PtInRect( point))
	{
		if( pt.x < m_rctGraphRegion.left)	pt.x = m_rctGraphRegion.left;
		if( m_rctGraphRegion.right < pt.x)	pt.x = m_rctGraphRegion.right;
		if( pt.y < m_rctGraphRegion.top)	pt.y = m_rctGraphRegion.top;
		if( m_rctGraphRegion.bottom < pt.y)	pt.y = m_rctGraphRegion.bottom;
	}

	int ix = 0;
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//long lx = 0;
	__int64 lx = 0;
	// 2011.01.06 by SYS <<
	if( GetIndexFromPoint( pt, ix) == R_ERROR) return TRUE;
	if( !IsValidIndex( ix)) return TRUE;
	if( GetValueFromIndex( ix, lx) == R_ERROR) return TRUE;

	m_ptCursor = pt;
	m_pCurSelectedLine->SetPos( pt, m_pCurSelectedLine->m_CurClickedPosition);

	//////////////////////////////////////////////////////////////////////////
	// �������� �������� ������ ��
	if( m_pCurSelectedLine->m_ptAStart.x > m_pCurSelectedLine->m_ptAEnd.x)	// ��ġ ����
	{
		if(m_pCurSelectedLine->m_CurClickedPosition == E_PT)
			m_pCurSelectedLine->m_CurClickedPosition = S_PT;
		else if(m_pCurSelectedLine->m_CurClickedPosition == S_PT)
			m_pCurSelectedLine->m_CurClickedPosition = E_PT;

		CPoint ptTemp = m_pCurSelectedLine->m_ptAStart;
		m_pCurSelectedLine->m_ptAStart = m_pCurSelectedLine->m_ptAEnd;
		m_pCurSelectedLine->m_ptAEnd = ptTemp;
	}

	// ������ �� ����
	m_pCurSelectedLine->m_ptAIndexedStart = m_pCurSelectedLine->m_ptAStart;
	m_pCurSelectedLine->m_ptAIndexedEnd = m_pCurSelectedLine->m_ptAEnd;

	// ������ �� ����
	GetIndexFromPoint( m_pCurSelectedLine->m_ptAStart, m_pCurSelectedLine->m_nSIndex);
 	GetValueFromIndex( m_pCurSelectedLine->m_nSIndex, m_pCurSelectedLine->m_lSDate);
	m_pCurSelectedLine->m_dSData = m_pIChartOCX->ConvertYPositionToData( m_pCurSelectedLine->m_ptAIndexedStart.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	// ���� �� ����
	GetIndexFromPoint( m_pCurSelectedLine->m_ptAEnd, m_pCurSelectedLine->m_nEIndex);
 	GetValueFromIndex( m_pCurSelectedLine->m_nEIndex, m_pCurSelectedLine->m_lEDate);
	m_pCurSelectedLine->m_dEData = m_pIChartOCX->ConvertYPositionToData( m_pCurSelectedLine->m_ptAIndexedEnd.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

	return TRUE;
}

BOOL CChartOrderLineAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !OnMouseMove( p_hDC, MK_LBUTTON, point, p_dwMouseActionID, NULL)) return FALSE;
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase)) return FALSE;
  	if( !m_pCurSelectedLine) return FALSE;
	if( p_dwMouseActionID != m_dwMouseActionID) return TRUE;
	if( !m_nCommandType) return TRUE;

	if( m_pCurSelectedLine->m_nSIndex == m_pCurSelectedLine->m_nEIndex) DeleteSelection();
	m_pIChartOCX->InvalidateControl();

	// ���� �׸��� ��쿡�� �������� ����
	if( m_nCommandType == 1) OnOLConfig();
	return TRUE;
}

BOOL CChartOrderLineAddInImp::OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_nCommandType) return FALSE;

	switch( m_nCommandType)
	{
		case 0:	// stop
				return FALSE;
		case 1:	// draw
				return TRUE;
		case 2: // modify
				if(m_pCurSelectedLine) if(m_pCurSelectedLine->IsInRegion(point))
				{
					m_dwMouseActionID = p_dwMouseActionID;
					return TRUE;
				}
				return SelectLine( point, p_dwMouseActionID);
	}

	return FALSE;
}

BOOL CChartOrderLineAddInImp::OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_nCommandType) return FALSE;

	//////////////////////////////////////////////////////////////////////////
	// popup menu ����
	CPoint clPoint(point);
	::ClientToScreen( m_pIChartOCX->GetOcxHwnd(), &clPoint);

	ML_SET_LANGUAGE_RES();
	CMenu menu;
	VERIFY( menu.LoadMenu( IDR_MENUOL));

	HMENU hPopupMenu = NULL;				// popup menu, Ŭ�� �� Ȱ��ȭ�Ǵ� �޴�
	switch(m_nCommandType)
	{
		case 1:	// draw mode
			hPopupMenu = GetSubMenu( menu.GetSafeHmenu(),0);
			break;
		case 2: // modify mode
			if( p_dwMouseActionID != m_dwMouseActionID) return FALSE;
			hPopupMenu = GetSubMenu(menu.GetSafeHmenu(),1);
			break;
		default:
			return FALSE;
	}
	
	TrackPopupMenu( hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, clPoint.x, clPoint.y, 0, m_pIChartOCX->GetOcxHwnd(), NULL);
	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}


BOOL CChartOrderLineAddInImp::OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( m_nCommandType != 2) return FALSE;

	CLineObj* pLine = NULL;
	POSITION pos = m_LineList.GetHeadPosition();
	while( pos)
	{
		pLine = m_LineList.GetNext(pos);
		if( !pLine->IsInRegion(point)) continue;

		ML_SET_LANGUAGE_RES();
		OnOLConfig();
		return TRUE;
	}

	return FALSE;
}

BOOL CChartOrderLineAddInImp::OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if(!m_nCommandType) return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
 			case ID_KSA_ORDERLINE_RELEASE:
			case ID_KSA_ORDERLINE_CONFIG:
			case ID_KSA_ORDERLINE_DELETE:
			case ID_KSA_ORDERLINE_DELETEFROMFILE:
			case ID_KSA_ORDERLINE_EXECUTE:			if( p_pICmdUI) p_pICmdUI->Enable( TRUE);
													break;
			default:	return FALSE;
		}
	}

	if( CN_COMMAND == nCode)
	{
		switch(nID)
		{
			case ID_KSA_ORDERLINE_CONFIG://ID_KSA_PERIODSUMMARY_CONFIG:
				OnOLConfig();
				break;
			case ID_KSA_ORDERLINE_DELETE:	//ȭ�鿡�� ����
				if( DeleteSelection()) m_pIChartOCX->InvalidateControl();
				break;
			case ID_KSA_ORDERLINE_DELETEFROMFILE:	// ����&ȭ�鿡�� ����
				OnDeleteLine();
				break;
			case ID_KSA_ORDERLINE_EXECUTE://ID_KSA_PERIODSUMMARY_EXECUTE://
				OnToolCommand( CToolOptionInfo::T_ORDER_LINE, 1);
				break;
			case ID_KSA_ORDERLINE_RELEASE://ID_KSA_COMMON_RELEASE:
				if( ChangeToolMode( 2)) m_pIChartOCX->InvalidateControl();
				break;
			default:
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CChartOrderLineAddInImp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( !m_nCommandType) return FALSE;
	if( nChar != VK_DELETE) return FALSE;
	if( !DeleteSelection()) return FALSE;

	m_pIChartOCX->InvalidateControl();
	return TRUE;
}

void CChartOrderLineAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	if( !GetChartBasicData()) return;

	CDC *pDC = CDC::FromHandle(p_hDC);
	CRgn rgn;
	rgn.CreateRectRgn( ( int)m_rctGraphRegion.left, ( int)m_rctGraphRegion.top, ( int)m_rctGraphRegion.right, ( int)m_rctGraphRegion.bottom);
	pDC->SelectClipRgn(&rgn);

	// 20081007 JS.Kim	����ڽð��� ����
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket(  _MTEXT( C0_DATE_TIME));
	CLineObj *pLine = NULL;
	POSITION pos = m_LineList.GetHeadPosition();
	while( pos)
	{
		pLine = m_LineList.GetNext( pos);

		// 20081007 JS.Kim	����ڽð��� ����<<
		if( pIPacket )
		{
			int nPckTimeDiff = pIPacket->GetBaseTimeDifference();
			if( nPckTimeDiff != pLine->m_nTimeDiff )
			{
				pLine->m_lSDate = (long)pIPacket->CalcTimeDiff(pLine->m_lSDate, nPckTimeDiff - pLine->m_nTimeDiff);
				pLine->m_lEDate = (long)pIPacket->CalcTimeDiff(pLine->m_lEDate, nPckTimeDiff - pLine->m_nTimeDiff);
				pLine->m_nTimeDiff = nPckTimeDiff;
			}
		}
		//>>
		GetIndexFromValue( pLine->m_lSDate, pLine->m_nSIndex);
		GetIndexFromValue( pLine->m_lEDate, pLine->m_nEIndex);

		if( ( pLine == m_pCurSelectedLine) && ( m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))) 
			continue;

		pLine->m_ptAIndexedStart.x = GetXPosition( pLine->m_nSIndex);
		pLine->m_ptAIndexedEnd.x = GetXPosition( pLine->m_nEIndex);

		pLine->m_ptAIndexedStart.y = m_pIChartOCX->ConvertDataToYPosition(pLine->m_dSData, m_dViewMax, m_dViewMin,m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_bLog, m_bReverse);
		pLine->m_ptAIndexedEnd.y = m_pIChartOCX->ConvertDataToYPosition(pLine->m_dEData, m_dViewMax, m_dViewMin,m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_bLog, m_bReverse);
		pLine->m_ptAStart = pLine->m_ptAIndexedStart;
		pLine->m_ptAEnd = pLine->m_ptAIndexedEnd;
		pLine->m_ptAExpand.x = m_rctBlockRegion.right;
		pLine->m_ptAExpand.y = GetYPositionOnLine((int&)pLine->m_ptAExpand.x, pLine->m_ptAIndexedStart, pLine->m_ptAIndexedEnd);
		pLine->Draw( pDC);
	}

	// 20081007 JS.Kim	����ڽð��� ����
	if( pIPacket )		pIPacket->Release();

	pDC->SelectClipRgn(NULL);
	rgn.DeleteObject();
}

BOOL CChartOrderLineAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	// Load Chart Data Info I
	CString strData = p_szData;
	strData += ":";
	int iStart = 0;
	m_strCode = TokenizeEx( strData, ":",iStart);		// �����ڵ�
	m_strCodeName = TokenizeEx( strData, ":",iStart);	// �����
	return TRUE;
}

RECT CChartOrderLineAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase)) return rct;
	if( !m_pCurSelectedLine) return rct;

	// 1. DC�� Handle�� ���Ѵ�.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	CRgn rgn;
	rgn.CreateRectRgn( ( int)m_rctGraphRegion.left, ( int)m_rctGraphRegion.top, ( int)m_rctGraphRegion.right, ( int)m_rctGraphRegion.bottom);
	pDC->SelectClipRgn( &rgn);

	// ������ �� ����
	m_pCurSelectedLine->m_ptAIndexedStart = m_pCurSelectedLine->m_ptAStart;
	m_pCurSelectedLine->m_ptAIndexedEnd = m_pCurSelectedLine->m_ptAEnd;

	// Ŭ�� ����Ʈ ��
	m_pCurSelectedLine->m_dSClicked = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAStart.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	m_pCurSelectedLine->m_dEClicked = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAEnd.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

	if(m_pCurSelectedLine->m_ptAStart.x < m_pCurSelectedLine->m_ptAEnd.x)
	{
		m_pCurSelectedLine->m_ptAIndexedStart.y = GetYPositionOnLine((int&)m_pCurSelectedLine->m_ptAIndexedStart.x, m_pCurSelectedLine->m_ptAStart, m_pCurSelectedLine->m_ptAEnd);
		m_pCurSelectedLine->m_ptAIndexedEnd.y = GetYPositionOnLine((int&)m_pCurSelectedLine->m_ptAIndexedEnd.x, m_pCurSelectedLine->m_ptAStart, m_pCurSelectedLine->m_ptAEnd);
		m_pCurSelectedLine->m_ptAExpand.x = m_rctBlockRegion.right;
		m_pCurSelectedLine->m_ptAExpand.y = GetYPositionOnLine((int&)m_pCurSelectedLine->m_ptAExpand.x, m_pCurSelectedLine->m_ptAIndexedStart, m_pCurSelectedLine->m_ptAIndexedEnd);
		m_pCurSelectedLine->m_dSData = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAIndexedStart.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
		m_pCurSelectedLine->m_dEData = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAIndexedEnd.y, m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	}

	rct = m_pCurSelectedLine->Draw(pDC);
	pDC->SelectClipRgn(NULL);
	rgn.DeleteObject();

	return rct;
}

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

void CChartOrderLineAddInImp::OnOLConfig()
{
	if( !m_pCurSelectedLine) return;

	ML_SET_LANGUAGE_RES();
	COrderLineConfigDlg dlg( m_pChartCWnd);
	dlg.m_OLObj = m_pCurSelectedLine;
	ILPCSTR szUserPath = m_pIChartOCX->GetUserFilePathString();
	dlg.m_strUserPath = szUserPath;
	dlg.m_pAllOrderLines = &m_slSavedOrderLines;	// ����� ��� �ֹ���
	if(dlg.DoModal() == IDOK)
	{
		m_pIChartOCX->InvalidateControl();
		return;
	}

	if( m_nCommandType == 1) 
		if( DeleteSelection())
			m_pIChartOCX->InvalidateControl();
}

void CChartOrderLineAddInImp::OnOLMan()
{
	ML_SET_LANGUAGE_RES();
	COrderLineManDlg dlg;
	ILPCSTR szUserPath = m_pIChartOCX->GetUserFilePathString();
	dlg.m_strUserPath = szUserPath;
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////
// �޸� & ���Ͽ��� �ֹ��� ����
void CChartOrderLineAddInImp::OnDeleteLine()
{
	//////////////////////////////////////////////////////////////////////////
	// ���� ����Ʈ���� ����
	if( !m_pCurSelectedLine) return;

	// �ش� �ֹ��� ã��
	POSITION prevpos = NULL, pos = m_slSavedOrderLines.GetHeadPosition();
	while( pos)
	{
		prevpos = pos;
		if( m_slSavedOrderLines.GetNext( pos).Find( m_pCurSelectedLine->m_strSerial) < 0) continue;

		m_slSavedOrderLines.RemoveAt(prevpos);

		// ���Ͽ� ��� ����
		CFile f;
		CString strFileName;
		ILPCSTR szUserPath = m_pIChartOCX->GetUserFilePathString();

		strFileName = szUserPath;
		strFileName += OL_FILE;

		if( !f.Open( strFileName, CFile::modeCreate | CFile::modeWrite))
			  return;

		CArchive ar( &f, CArchive::store);
		m_slSavedOrderLines.Serialize(ar);
		ar.Close();
		f.Close();
		break;
	}

	if( DeleteSelection()) m_pIChartOCX->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////////
// ���� �׸��� ����

int CChartOrderLineAddInImp::GetIndexFromPoint(CPoint& pt/*IN, OUT*/, int &x/*IN*/)
{
	int nStartIndex = 0, nEndIndex = 0;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return R_ERROR;
	// 1.3 ������ Mouse Point�� Data Index�� ���Ѵ�.
	x = long( double( pt.x - m_rctBlockRegion.left) / ( m_rctBlockRegion.Width() / double( nEndIndex - nStartIndex + 1)) + nStartIndex);

	return R_OK;
}

// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰� <<
//int CChartOrderLineAddInImp::GetValueFromIndex( int ix/*IN*/, long &lx/*OUT*/)
int CChartOrderLineAddInImp::GetValueFromIndex( int ix/*IN*/, __int64 &lx/*OUT*/)
{
	// 2.1 �ڷ����� Packet�� ���Ѵ�.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME ));
	if( !pIPacket) return R_ERROR;

	double dTime = 0.0;
	BOOL bResult = pIPacket->GetData( ix, dTime);
	pIPacket->Release();
	if( !bResult) return R_ERROR;

	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//lx = ( long)dTime;
	lx = ( __int64)dTime;
	// 2011.01.06 by SYS <<

	return R_OK;
}

long CChartOrderLineAddInImp::GetXPosition( int x)
{
	return long( ( x - m_nStartIndex + 0.5)
			* m_rctBlockRegion.Width()
			/ double( m_nEndIndex - m_nStartIndex + 1)
			+ m_rctBlockRegion.left);
}

// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰� <<
//int CChartOrderLineAddInImp::GetIndexFromValue( long lx/*IN*/, int &ix/*OUT*/)
int CChartOrderLineAddInImp::GetIndexFromValue( __int64 lx/*IN*/, int &ix/*OUT*/)
{
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return R_ERROR;

	int nIndex = pIPacket->GetIndexFromData( lx);
	pIPacket->Release();

	if( nIndex < 0) return R_ERROR;
	ix = nIndex;
	return R_OK;
}

/////////////////////////////////////////////
// pt1, pt2�� �����ϴ� ���� ���弱 ���� y pixel ���� ���Ѵ�.
int CChartOrderLineAddInImp::GetYPositionOnLine( int &x/*IN, OUT*/, CPoint pt1/*IN*/, CPoint pt2/*IN*/)
{
	int y = 0;
	double x1 = pt1.x;
	double x2 = pt2.x;
	double y1 = pt1.y;
	double y2 = pt2.y;

	if( x1 < x2) y = int( ( ( y1 - y2) * x + ( x1 * y2 - x2 * y1)) / double( x1 - x2));
	if( y < 0 && y1 != y2)
	{
		y = 0;
		x = int( ( x2 * y1 - x1 * y2) / double( y1 - y2));
	}

	return y;
}

//////////////////////////////////////////////////////////////////////////
// �־��� index �� ��ȿ�� ������ ���� �Ǵ�
BOOL CChartOrderLineAddInImp::IsValidIndex(int nIndex)
{
	IPacket *pIPacketForCur = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( !pIPacketForCur) return FALSE;

	double dCur = 0;
	BOOL bResult = pIPacketForCur->GetData( nIndex, dCur);	// ������ ����
	pIPacketForCur->Release();
	return bResult;
}

void CChartOrderLineAddInImp::RegPS(long pLine)
{
	if( m_pIAddInToolMgr != NULL )
	{
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_ADD_TOOL_TO_ORDERMANAGER,	
											  m_strAddInItemName,		
											  pLine );	
	}
}

void CChartOrderLineAddInImp::UnregPS(long pLine)
{
	if( m_pIAddInToolMgr != NULL )
	{
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_TOOL_FROM_ORDERMANAGER,	
											  m_strAddInItemName,		
											  pLine );	
	}
}

CString CChartOrderLineAddInImp::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	if( iStart < 0) return "";

	int iCur = iStart;
	iStart = strSrc.Find(pszTokens, iCur);
	if( iStart == -1) return "";

	iStart++;
	return strSrc.Mid(iCur, iStart - iCur - 1);
}

//////////////////////////////////////////////////////////////////////////
// ����� �ֹ��� �ε�
void CChartOrderLineAddInImp::LoadOrderLine()
{

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ Ŭ����
	DeleteAll();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �ʱ�ȭ
	if( !m_slSavedOrderLines.IsEmpty()) m_slSavedOrderLines.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	ILPCSTR szUserPath = m_pIChartOCX->GetUserFilePathString();
	CString strFileName = (CString)szUserPath + OL_FILE;
	//////////////////////////////////////////////////////////////////////////
	// ar �Ӽ� ����
	CFile f;
	if( !f.Open( strFileName, CFile::modeRead ) ) {
		  return;
	}
	CArchive ar( &f, CArchive::load);
	//////////////////////////////////////////////////////////////////////////
	m_slSavedOrderLines.Serialize(ar);
	ar.Close();
	f.Close();

	CString strLineInfo;
	POSITION pos = m_slSavedOrderLines.GetHeadPosition();
	int iStart2 = 0;
	while (pos)
	{
		strLineInfo = m_slSavedOrderLines.GetNext(pos);
		strLineInfo.Replace( ';', ',');

		TRACE( "%s\r\n", strLineInfo);

		CLineObj* pLine = new CLineObj( m_hOcxWnd);
		iStart2 = 0;
		pLine->m_strSerial = TokenizeEx(strLineInfo, ",", iStart2);
		pLine->m_strCode = TokenizeEx(strLineInfo, ",", iStart2);
		pLine->m_strName = TokenizeEx(strLineInfo, ",", iStart2);
		pLine->m_strCandle = TokenizeEx(strLineInfo, ",", iStart2);
		pLine->m_lSDate = atol(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_dSData = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_lEDate = atol(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_dEData = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_bUpSell = atoi(TokenizeEx(strLineInfo, ",", iStart2));		
		pLine->m_bUpBuy = atoi(TokenizeEx(strLineInfo, ",", iStart2));		
		pLine->m_bDownSell = atoi(TokenizeEx(strLineInfo, ",", iStart2));	
		pLine->m_bDownBuy = atoi(TokenizeEx(strLineInfo, ",", iStart2));	
		pLine->m_nLineThickness = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nLineStyle = atoi(TokenizeEx(strLineInfo, ",", iStart2));	
		pLine->m_clrLineColor = (COLORREF) atol(TokenizeEx(strLineInfo, ",", iStart2));
		// 20081007 JS.Kim	����ڽð��� ����
		pLine->m_nTimeDiff = atoi(TokenizeEx(strLineInfo, ",", iStart2));	

		// �ش� ���� �� ���غ��� �����͸� �߰�
		if( ( pLine->m_strCode == m_strCode) && ( ( pLine->m_strCandle) == m_strDateType))
		{
			GetIndexFromValue( pLine->m_lSDate, pLine->m_nSIndex);
			GetIndexFromValue( pLine->m_lEDate, pLine->m_nEIndex);

			if( ( pLine == m_pCurSelectedLine) && ( m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))) 
				continue;

			pLine->m_ptAIndexedStart.x = GetXPosition( pLine->m_nSIndex);
			pLine->m_ptAIndexedEnd.x = GetXPosition( pLine->m_nEIndex);

			//pLine->m_ptAIndexedStart.y = m_pIChartOCX->ConvertDataToYPosition(pLine->m_dSData, m_dViewMax, m_dViewMin,m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_bLog, m_bReverse);
			//pLine->m_ptAIndexedEnd.y = m_pIChartOCX->ConvertDataToYPosition(pLine->m_dEData, m_dViewMax, m_dViewMin,m_rctGraphDrwingRegion.top, m_rctGraphDrwingRegion.bottom, m_bLog, m_bReverse);
			pLine->m_ptAStart = pLine->m_ptAIndexedStart;
			pLine->m_ptAEnd = pLine->m_ptAIndexedEnd;
			//pLine->m_ptAExpand.x = m_rctBlockRegion.right;
			//pLine->m_ptAExpand.y = GetYPositionOnLine((int&)pLine->m_ptAExpand.x, pLine->m_ptAIndexedStart, pLine->m_ptAIndexedEnd);

			//>>

			m_LineList.AddTail( pLine);
			RegPS( ( long)pLine);
		}
		else
		{
			delete pLine;
			pLine = NULL;
		}
	}
	m_pIChartOCX->InvalidateControl();
}

// 20081007 JS.Kim	����ڽð��� ����
// Graph�� �ð��밡 ����Ǹ� �ֹ��� �� �ð��� �ٲ��ش�.
bool CChartOrderLineAddInImp::RecalcLineXPoint(const char* szName, int nTimeDiff)
{
//	CString strRQ = m_pIChartManager->GetCurrentRQ();
//	IPacket* pPacket= m_pIDefaultPacketManager->GetDatePacket(strRQ);
	IPacket *pPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pPacket )		return false;

	int	nBaseTimeDiff;
	if( strlen(szName) == 0 )	nBaseTimeDiff = pPacket->GetOrgBaseTimeDifference();		// �����ð����� �ǵ��� ���
	else						nBaseTimeDiff = nTimeDiff;

	double dXDate, oldXDate;
	int oldTimeDiff;

	CLineObj *pLine = NULL;
	POSITION pos = m_LineList.GetHeadPosition();
	while( pos)
	{
		if( !(pLine = m_LineList.GetNext( pos)) )	break;

		oldTimeDiff = pLine->m_nTimeDiff;
		if( oldTimeDiff != nBaseTimeDiff )
		{

			for( int i=0; i<2; i++ )
			{
				if( i == 0 )		oldXDate = pLine->m_lSDate;
				else				oldXDate = pLine->m_lEDate;

				dXDate = pPacket->CalcTimeDiff(oldXDate, nBaseTimeDiff - oldTimeDiff);

				if( i == 0 )		pLine->m_lSDate = (long)dXDate;
				else				pLine->m_lEDate = (long)dXDate;
			}
			pLine->m_nTimeDiff = nBaseTimeDiff;
		}
	}
	if( pPacket) pPacket->Release();

	return true;
}
