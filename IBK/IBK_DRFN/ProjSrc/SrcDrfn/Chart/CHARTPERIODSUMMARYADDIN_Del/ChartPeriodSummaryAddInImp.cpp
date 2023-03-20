// ChartPeriodSummaryAddInImp.cpp: implementation of the CChartPeriodSummaryAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "resource.h"
#include "PSConfigDlg.h"
#include "ChartPeriodSummaryAddInImp.h"
//#include "./Include_AddIn/Include_AddIn.h"			// for Ixxxxxx.h
#include "./Include_Chart/MessageDefine.h"			// for UM_CURSOR_EXCHANGE

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define IDC_HAND MAKEINTRESOURCE(32649)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartPeriodSummaryAddInImp::CChartPeriodSummaryAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
,	m_nMouseClickStatus(0)	// No click
,	m_bCentered(TRUE)
,	m_nCommandType(0)
,	m_bAttachData(FALSE)
,	m_bMouseCaptured(FALSE)
,	m_pCurSelectedLine(NULL)
,	m_bDrag(FALSE)
,	m_bLoadData(FALSE)
{

}

CChartPeriodSummaryAddInImp::~CChartPeriodSummaryAddInImp()
{
	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		delete pLine;
		pLine = NULL;
	}
	m_LineList.RemoveAll();
//	delete m_pBmp;
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartPeriodSummaryAddInImp::m_strAddInItemName = "PERIOD_SUMMARY";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartPeriodSummaryAddInImp)
	ONADDINEVENT( OnCaptureChanged)
	ONADDINEVENT(OnDrawAfterGraph)	
	ONADDINEVENT(OnDrawBeforeGraph)
	ONADDINEVENT(OnPacketData)
	ONADDINEVENT(OnMouseMove)
	ONADDINEVENT(OnLButtonDown)
	ONADDINEVENT(OnLButtonUp)
	ONADDINEVENT(OnRButtonDown)
	ONADDINEVENT(OnRButtonUp)
	ONADDINEVENT(OnCmdMsg)
	ONADDINEVENT(OnKeyDown)
	ONADDINEVENT(OnResetChart)
	ONADDINEVENT(OnLButtonDblClk)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT(OnAddInToolCommand_EAICommandType_long_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
// �������� Real�ݿ� : �������� - ojtaso (20070131)
	ONADDINEVENT( OnUpdatePacketDataMultiItem)
	ONADDINEVENT( OnPacketDataMultiItem)	
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////
BOOL CChartPeriodSummaryAddInImp::OnAddInToolCommand(const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if(p_eCommandID != EAI_OOA_ON_TOOL_FUNCTION) return FALSE;

	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_lParam1;
	
	if( eToolOption == CToolOptionInfo::T_DELETE_ALL )
	{
		POSITION pos = m_LineList.GetHeadPosition();
		while (pos)
		{
			CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
			delete pLine;
			pLine = NULL;
		}
		m_LineList.RemoveAll();
	}
	else if(eToolOption == CToolOptionInfo::T_TOOL_PERIOD_SUMMARY)
	{
		if(p_lParam2 != 3)
			m_nCommandType = p_lParam2;
		switch(m_nCommandType)
		{
		case 1:			// draw mode
			{
				m_nCommandType = p_lParam2;
				int		nR, nC ;
				if( !GetChartBasicData(m_nStartIndex, m_nEndIndex, nR, nC, m_rctBlockRegion, m_rctGraphRegion))
					return FALSE;

				m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, TRUE );	
			}
			break;
		case 2:			// modify mode
			break;
		}
		return TRUE;
	}
	else if(eToolOption == CToolOptionInfo::T_TOOL_SELECTION)
	{
//		if(p_lParam2 == CToolOptionInfo::T_TOOL_PERIOD_SUMMARY) return FALSE;
		if(p_lParam2 == CToolOptionInfo::T_TOOL_PERIOD_SUMMARY || p_lParam2 == CToolOptionInfo::T_DELETE_INORDER) return FALSE;
		m_nCommandType = 2;
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );
		if(m_pCurSelectedLine)
		{
			m_pCurSelectedLine->m_bSelected = FALSE;
			m_pCurSelectedLine = NULL;
			m_pIChartOCX->InvalidateControl();
		}
		return FALSE;
	}
	else if(eToolOption == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		if( (m_pIChartOCX != NULL) && (m_nCommandType == 1))
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, TRUE );	
	}
	
	else if(eToolOption == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{
		m_nCommandType = 2;
	}

	return FALSE;
}

BOOL CChartPeriodSummaryAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if(p_eCommandID == EAI_TOOLORDERMGR_DELETE_THIS_TOOL)
	{
		if(strcmp(p_szParam, "PERIOD_SUMMARY")) return FALSE;
		CPeriodObj * pLine = (CPeriodObj *)p_lParam;
		if(!pLine) return FALSE;
		POSITION pos = m_LineList.Find(pLine);
		m_LineList.RemoveAt(pos);
		delete pLine;
		pLine = NULL;
		m_pIChartOCX->InvalidateControl();
	}

	return FALSE;
}


void CChartPeriodSummaryAddInImp::OnResetChart()
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ Ŭ����
	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		delete pLine;
		pLine = NULL;
	}
	m_LineList.RemoveAll();

	m_nCommandType = 0;

	if( m_pIChartOCX != NULL )
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );	

	//////////////////////////////////////////////////////////////////////////
}
void CChartPeriodSummaryAddInImp::OnPacketData(const char *p_szPacketNames, BOOL p_bWithReset)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	//////////////////////////////////////////////////////////////////////////
	// ������Ʈ ������ Ŭ����
	POSITION pos = m_LineList.GetHeadPosition();
	while (pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		delete pLine;
		pLine = NULL;
	}
	m_LineList.RemoveAll();
}

BOOL CChartPeriodSummaryAddInImp::OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
		case ID_KSA_PERIODSUMMARY_CONFIG:
		case ID_KSA_PERIODSUMMARY_DELETE:
		case ID_KSA_COMMON_RELEASE:
		case ID_KSA_PERIODSUMMARY_EXECUTE:
			if( p_pICmdUI)
			{
				p_pICmdUI->Enable(TRUE);
			}
			break;
		default:
			return FALSE;
		}
	}

	if( CN_COMMAND == nCode)
	{
		switch(nID)
		{
		case ID_KSA_PERIODSUMMARY_CONFIG:
			OnPSConfig(m_pCurSelectedLine);
			break;
		case ID_KSA_PERIODSUMMARY_DELETE:
			OnDeleteLine();
			break;
		case ID_KSA_COMMON_RELEASE:
			OnReleaseDraw();
			break;
		case ID_KSA_PERIODSUMMARY_EXECUTE:
			OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, 1);
			break;
		default:
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CChartPeriodSummaryAddInImp::OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	POINT ptTemp = point;
//	point.x -= m_ptOffSet.x;
//	point.y -= m_ptOffSet.y;

	CPoint pt = point;

	if(!m_pCurSelectedLine) return FALSE;
	if( p_dwMouseActionID == m_dwMouseActionID)
	{
		m_bDrag = TRUE;

		int ix, is, ie;
		if(m_rctGraphRegion.PtInRect(ptTemp))
		{
			m_pCurSelectedLine->SetPos(pt, m_pCurSelectedLine->m_CurClickedPosition);
			if(		(GetIndexFromPoint(pt, ix, ix) == R_OK) 
				&&	(GetIndexFromPoint(m_pCurSelectedLine->m_ptAStart, is, is) == R_OK)
				&&	(GetIndexFromPoint(m_pCurSelectedLine->m_ptAEnd, ie, ie) == R_OK) 
			  )
			{
				if(IsValidIndex(ix) && IsValidIndex(is) && IsValidIndex(ie))
					m_ptCursor = point;
// 				else
// 				{
// 					//point = m_ptCursor;
// 					TRACE1("\nTEST:%d\n",point.x);
// 				}
			}

		}

		return TRUE;
	}
	else if(m_nCommandType != 1)
	{
	//	SetCursor(::LoadCursor(NULL, IDC_ARROW));

		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
			if( pLine->IsInRegion(point) )
			{
			//	m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_HAND));
				return TRUE;
			}
		}
		return FALSE;
	}
	if( m_ptInit == point)	return FALSE;


	return FALSE;
	
}

BOOL CChartPeriodSummaryAddInImp::OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(!m_nCommandType) return FALSE;
	POINT ptTemp = point; 
//	point.x -= m_ptOffSet.x;
//	point.y -= m_ptOffSet.y;

	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart!
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty()) return FALSE;
	CRect rctRegion = m_pIChartManager->GetGraphRegionSumInColumn( ptTemp);
	if( rctRegion.IsRectEmpty()) return FALSE;

	m_ptInit = point;
	if(!m_rctBlockRegion.PtInRect(ptTemp)) return FALSE;
	CDC * pDC = CDC::FromHandle(p_hDC);

//	m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_HAND));
	

	m_dwMouseActionID = p_dwMouseActionID;

	
	BOOL bSpace=TRUE;

	
	POSITION pos = m_LineList.GetHeadPosition();
	while(pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		if( pLine->IsInRegion(point) && m_nCommandType == 2)
		{
			if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
			{
				if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
			}

			m_bNewDraw = FALSE;
			if(m_pCurSelectedLine)
				m_pCurSelectedLine->m_bSelected = FALSE;
			m_pCurSelectedLine = pLine;
			m_pCurSelectedLine->ButtonDownAt(point);
			POSITION pos2 = m_LineList.Find(m_pCurSelectedLine);
			m_LineList.RemoveAt(pos2);
			m_LineList.AddTail(m_pCurSelectedLine);
			m_pCurSelectedLine->m_bSelected = TRUE;
			m_ptCursor = point;
//			TRACE2("\n%d, %d\n", m_ptCursor.x, m_ptCursor.y);
			if( m_pIChartOCX != NULL )
			{

				m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION,	
													CToolOptionInfo::T_TOOL_PERIOD_SUMMARY );	
			}

			bSpace = FALSE;
			return TRUE;
			break;
		}
		else
		{
			bSpace = TRUE;
		}
	}

	if(bSpace && (m_nCommandType == 1))
	{
		if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
		{
			if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
		}
		m_bMouseCaptured = TRUE;
		m_ptCursor = point;
		m_bNewDraw = TRUE;
		CPeriodObj* pLine;
		pLine = new CPeriodObj;
 		pLine->m_Num = m_LineList.GetCount();
		GetDateType();
		pLine->m_nDateType = m_nDateType;
// 		pLine->Initilize();
 		int ix, iy;
 		// ������ �� ����
 		CPoint ptAt = point;
 		if(GetIndexFromPoint(ptAt, ix, iy) == R_ERROR) return FALSE;
 		IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "����");
 		IPacket *pIPacketForRef = m_pIPacketManager->GetPacket( "�ð�");
		if(!pIPacketForCur || !pIPacketForRef) return FALSE;

		double dCur, dRef;
		if( !pIPacketForCur->GetData( iy, dCur) || !pIPacketForRef->GetData( iy, dRef))
		{
			pIPacketForCur->Release();
			pIPacketForRef->Release();
			return FALSE;
		}
		double dClickedValue = m_pIChartOCX->ConvertYPositionToData(ptAt.y, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
		if(dClickedValue >= max(dRef, dCur)) pLine->SetUpDown(UP);
		else if(dClickedValue <= min(dRef, dCur)) pLine->SetUpDown(DOWN);
		else pLine->SetUpDown(UP);

		pIPacketForCur->Release();
		pIPacketForRef->Release();

 		pLine->ButtonDownAt(ptAt);
		pLine->m_CurClickedPosition = E_PT;
		
		if(m_pCurSelectedLine)	m_pCurSelectedLine->m_bSelected = FALSE;	// ������ ���õ� ���� ������ ���� ����

		m_pCurSelectedLine = pLine;
		m_pCurSelectedLine->m_bSelected = TRUE;
		m_LineList.AddTail(pLine);
		RegPS((long)pLine);
		return TRUE;
	}
	else if(bSpace && (m_nCommandType != 1))	// no action
 	{
		if(m_pCurSelectedLine)
			m_pCurSelectedLine->m_bSelected = FALSE;
		m_pCurSelectedLine = NULL;
		return FALSE;
 	}

	return FALSE;
}

BOOL CChartPeriodSummaryAddInImp::OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;

//	point.x -= m_ptOffSet.x;
//	point.y -= m_ptOffSet.y;

	CDC * pDC = CDC::FromHandle(p_hDC);
	if( !pDC) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase)) return FALSE;


	if(p_dwMouseActionID == m_dwMouseActionID)
	{
		m_bDrag = FALSE;
 		int ix = 0, iy = 0;
// 		double dDummy;
 		if(!m_pCurSelectedLine) return FALSE;
		if(m_pCurSelectedLine->m_nStartIndex == m_pCurSelectedLine->m_nEndIndex)
		{
		//	m_pCurSelectedLine->Erase(&m_dcMem);
			POSITION pos = m_LineList.Find(m_pCurSelectedLine);
			m_LineList.RemoveAt(pos);
			UnregPS((long)m_pCurSelectedLine);
			delete m_pCurSelectedLine;
			m_pCurSelectedLine = NULL;
	//		return TRUE;
		}
		m_pIChartOCX->InvalidateControl();
	//	m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_ARROW));
	}

	return TRUE;


}

BOOL CChartPeriodSummaryAddInImp::OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
//	point.x -= m_ptOffSet.x;
//	point.y -= m_ptOffSet.y;

	switch(m_nCommandType)
	{
	case 0:	// stop
		return FALSE;
	case 1:	// draw
		return TRUE;
		break;
	case 2: // modify
		POSITION tmpPos;
		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			tmpPos = pos;
			CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
			if( pLine->IsInRegion(point) )
			{
				m_PosForRemove = tmpPos;
				m_dwMouseActionID = p_dwMouseActionID;
				m_bNewDraw = FALSE;
				m_pCurSelectedLine = pLine;
				m_pCurSelectedLine->ButtonDownAt(point);			
				return TRUE;
			}
		}	
		return FALSE;
		break;
	}
	return FALSE;
}

BOOL CChartPeriodSummaryAddInImp::OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	CPoint clPoint(point);
//	point.x -= m_ptOffSet.x;
//	point.y -= m_ptOffSet.y;

	m_hDC = p_hDC;
//	if(p_dwMouseActionID == m_dwMouseActionID)
//	{

		//////////////////////////////////////////////////////////////////////////
		// popup menu ����

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENUPS));

		switch(m_nCommandType)
		{
		case 1:	// draw mode
			::ClientToScreen(m_pIChartOCX->GetOcxHwnd(),&clPoint);
			m_hPopupMenu = GetSubMenu(menu.GetSafeHmenu(),0);
			break;
		case 2: // modify mode
			{
				POSITION tmpPos;
				POSITION pos = m_LineList.GetHeadPosition();
				while(pos)
				{
					tmpPos = pos;
					CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
					if( pLine->IsInRegion(clPoint) )
					{
						m_PosForRemove = tmpPos;
						m_dwMouseActionID = p_dwMouseActionID;
						m_bNewDraw = FALSE;
						m_pCurSelectedLine = pLine;
						m_pCurSelectedLine->ButtonDownAt(clPoint);			
						break;
					}
				}	

				if(p_dwMouseActionID == m_dwMouseActionID)
				{
					::ClientToScreen(m_pIChartOCX->GetOcxHwnd(),&clPoint);
					m_hPopupMenu = GetSubMenu(menu.GetSafeHmenu(),1);
				}
				else
					return FALSE;
				
			}
			break;
		default:
			return FALSE;
		}
		
		TrackPopupMenu(m_hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, clPoint.x, clPoint.y,	0, m_pIChartOCX->GetOcxHwnd(), NULL);
//		DestroyMenu(m_hPopupMenu);
		//////////////////////////////////////////////////////////////////////////
//		TRACE1("\nthis:%ld",this);
		return TRUE;
//	}
//	return FALSE;
}


BOOL CChartPeriodSummaryAddInImp::OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	//point.x -= m_ptOffSet.x;
	//point.y -= m_ptOffSet.y;

	POSITION pos = m_LineList.GetHeadPosition();
	while(pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		if( pLine->IsInRegion(point) && m_nCommandType == 2)
		{
			OnPSConfig(pLine);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CChartPeriodSummaryAddInImp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	
	if(nChar == 46)
	{
		if(!m_pCurSelectedLine) return FALSE;
//		m_pCurSelectedLine->Erase(&m_dcMem);
		POSITION pos = m_LineList.Find(m_pCurSelectedLine);
		m_LineList.RemoveAt(pos);
		UnregPS((long)m_pCurSelectedLine);
		delete m_pCurSelectedLine;
		m_pCurSelectedLine = NULL;
		m_pIChartOCX->InvalidateControl();
		return TRUE;
	}

	return FALSE;
}

void CChartPeriodSummaryAddInImp::OnDrawBeforeGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
}

void CChartPeriodSummaryAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	try
	{
		CDC * pDC = CDC::FromHandle(p_hDC);
		int nR, nC;
		CRect rcPrev = m_rctBlockRegion;
		if( !GetChartBasicData(m_nStartIndex, m_nEndIndex, nR, nC, m_rctBlockRegion, m_rctGraphRegion))
			throw this;

		CRgn rgn;
		rgn.CreateRectRgn((int)m_rctGraphRegion.left, (int)m_rctGraphRegion.top, (int)m_rctGraphRegion.right, (int)m_rctGraphRegion.bottom);
		pDC->SelectClipRgn(&rgn);

		POSITION pos = m_LineList.GetHeadPosition();
		while(pos)
		{
			CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
			if(pLine->m_bSaved)
			{
				int iDummy=0;
				pLine->m_bSaved = FALSE;
//2008.08.27 by LYH >> �������� ��ȸ �� �ð� �����̸� �׸��� ����
//				GetIndexFromValue(pLine->m_lSDate, 0, pLine->m_nStartIndex, iDummy);
//				GetIndexFromValue(pLine->m_lEDate, 0, pLine->m_nEndIndex, iDummy);
				int nRet = GetIndexFromValue(pLine->m_lSDate, 0, pLine->m_nStartIndex, iDummy);
				pLine->m_nEndIndex = -1;
				GetIndexFromValue(pLine->m_lEDate, 0, pLine->m_nEndIndex, iDummy);
				if(nRet == R_ERROR && pLine->m_nStartIndex == 0)
				{
					pLine->m_nStartIndex = 0;
					pLine->m_nEndIndex = 0;
				}
				if(pLine->m_nEndIndex<0)
					pLine->m_nEndIndex = m_nEndIndex;
//2008.08.27 by LYH <<
				RegPS((long)pLine);

			}
	
			if((pLine == m_pCurSelectedLine) && (m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))) 
			{
				continue;
			}
 			GetPointFromIndex(pLine->m_nStartIndex,0,pLine->m_ptAStart);
 			GetPointFromIndex(pLine->m_nEndIndex,0,pLine->m_ptAEnd);
 			pLine->m_ptAStart.y = m_pIChartOCX->ConvertDataToYPosition(pLine->m_dSClicked, m_dViewMax, m_dViewMin, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_bLog, m_bReverse);
 			pLine->m_ptAEnd.y = m_pIChartOCX->ConvertDataToYPosition(pLine->m_dEClicked, m_dViewMax, m_dViewMin, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_bLog, m_bReverse);
	
			if(GetPriceInfo(&pLine->m_Price, pLine->m_nStartIndex, pLine->m_nEndIndex) == R_ERROR) goto Drawing;
			if(GetCandleInfo(&pLine->m_Candle, pLine->m_nStartIndex, pLine->m_nEndIndex) == R_ERROR) goto Drawing;
			if(GetAmountInfo(&pLine->m_Amount, pLine->m_nStartIndex, pLine->m_nEndIndex) == R_ERROR) goto Drawing;
			
//			TRACE2("\n\nsidx : %d, eidx : %d\n\n",pLine->m_nStartIndex,pLine->m_nEndIndex);
Drawing:
			pLine->Draw(pDC);
		}

		//pDC->SelectClipRgn(&rgn, NULLREGION);
		pDC->SelectClipRgn(NULL);
		rgn.DeleteObject();

	}
	catch (CChartPeriodSummaryAddInImp* )
		{
		
	}
}

BOOL CChartPeriodSummaryAddInImp::InvokeAddIn(int p_nCommandType, long p_lData)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	
	m_nCommandType = p_nCommandType;
	switch(p_nCommandType)
	{
	case 0:			// stop mode
		break;
	case 1:			// draw mode
		{
			int		nR, nC ;
			if( !GetChartBasicData(m_nStartIndex, m_nEndIndex, nR, nC, m_rctBlockRegion, m_rctGraphRegion))
				return FALSE;
		}
		break;
	case 2:			// modify mode
		break;
	}
	return TRUE;
}

BOOL CChartPeriodSummaryAddInImp::GetChartBasicData(int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion)
{
	// �� Data View�� Start/End Index�� ���Ѵ�.
	if( !m_pIChartOCX->GetViewStartEndIndex( p_nStartIndex, p_nEndIndex)) return FALSE;
	int nV, nG;
	
	// ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
	if( !m_pIChartManager->GetIndexOfGraph( "������Ʈ", p_nR, p_nC, nV, nG)) return FALSE;
	
	// Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	IBlock *pIBlock = m_pIChartManager->GetBlock(p_nR, p_nC);
	if( pIBlock)
	{
		pIBlock->GetLogAndReverseScaleFlag( m_bLog, m_bReverse);
		pIBlock->GetVScaleGroupMinMax(nV, m_dViewMin, m_dViewMax);
	   	pIBlock->GetGraphRegions(p_rctGraphRegion, p_rctBlockRegion );
		p_rctGraphRegion = pIBlock->GetGraphDrwingRegion();
 		//m_ptOffSet.x = p_rctGraphRegion.left;
		//m_ptOffSet.y = p_rctGraphRegion.top;

		pIBlock->Release();
		pIBlock = NULL;

	}

	return TRUE;
}

//BOOL CChartPeriodSummaryAddInImp::GetValueFromPoint(CPoint& ptAt/*IN, OUT*/, long &x/*OUT*/, double &y/*OUT*/)
//{
//	int ix, iy;
//	if(GetIndexFromPoint(ptAt, ix, iy) == -1) return FALSE;
//	if(GetValueFromIndex(ix, iy, x, y) == -1) return FALSE;
//	return TRUE;
//}

int CChartPeriodSummaryAddInImp::GetIndexFromPoint(CPoint& pt/*IN, OUT*/, int &x/*IN*/, int &y/*IN*/)
{
//	POINT ptTemp = pt;
//	ptTemp.x += m_ptOffSet.x;
//	ptTemp.y += m_ptOffSet.y;
//
//	int nStartIndex, nEndIndex;
//	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return R_ERROR;
//	// 1.3 ������ Mouse Point�� Data Index�� ���Ѵ�.
//	x = y = nStartIndex + ( ( pt.x - m_rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / m_rctBlockRegion.Width();
//
//	if(m_bAttachData)
//	{
//		if(m_bCentered)	// �߽ɼ��� ����
//			ptTemp.x = (x*m_rctBlockRegion.Width() -m_nStartIndex*m_rctBlockRegion.Width() + m_rctBlockRegion.Width()/2.)/
//						( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left;
//		else
//			ptTemp.x = (x*m_rctBlockRegion.Width() - m_nStartIndex*m_rctBlockRegion.Width())/
//						( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left;
//	}
//
//	POINT ptAt;
//	ptAt.x = pt.x;
//	ptAt.y = pt.y;

//	POINT ptTemp = pt;
	//ptTemp.x += m_ptOffSet.x;
	//ptTemp.y += m_ptOffSet.y;

	CString strRQ = "DEFAULT";
	int nStartIndex, nEndIndex;
	if( !m_pIChartManager->GetStartEndDrawIdx_Cur(strRQ, nStartIndex, nEndIndex)) return R_ERROR;
	// 1.3 ������ Mouse Point�� Data Index�� ���Ѵ�.
	//x = y = nStartIndex + ( ( pt.x - m_rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / m_rctBlockRegion.Width();
	x = y = m_pIChartManager->FindViewDataIndex_Pt(strRQ, pt.x);

//	if(m_bAttachData)
//	{
//		if(m_bCentered)	// �߽ɼ��� ����
//			ptTemp.x = (x*m_rctBlockRegion.Width() -m_nStartIndex*m_rctBlockRegion.Width() + m_rctBlockRegion.Width()/2.)/
//						( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left;
//		else
//			ptTemp.x = (x*m_rctBlockRegion.Width() - m_nStartIndex*m_rctBlockRegion.Width())/
//						( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left;
//	}

//	POINT ptAt;
//	ptAt.x = pt.x;
//	ptAt.y = pt.y;


	return R_OK;
}

int CChartPeriodSummaryAddInImp::GetValueFromIndex(int ix, int iy, long &lx, double &dy)
{
	// 2.1 �ڷ����� Packet�� ���Ѵ�.
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "�ڷ�����");
	if( !pIPacket) return R_ERROR;

	try
	{
		double dTime;
		ILPCSTR is = pIPacket->GetType();
		CString s = is;
		if(s == "YYYYMMDD")
		{
			m_nDateType = 0;
		}
		else if(s == "HHMMSS")
		{
			m_nDateType = 1;
		}

		if( !pIPacket->GetData( ix, dTime)) throw this;
		lx = ( long)dTime;
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();

	return R_OK;
}

int CChartPeriodSummaryAddInImp::GetPointFromIndex(int x, int y, CPoint &ptIndex)
{
//	if(m_bCentered)	// �߽ɼ��� ���� 
//		ptIndex.x = (x*(m_rctBlockRegion.Width()+1) -m_nStartIndex*( m_rctBlockRegion.Width()+1) + (m_rctBlockRegion.Width())/2.)/
//					( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left+2;
//	else
//		ptIndex.x = (x*m_rctBlockRegion.Width() - m_nStartIndex*m_rctBlockRegion.Width())/
//					( m_nEndIndex - m_nStartIndex + 1) + m_rctBlockRegion.left-1;

	ptIndex.x = m_pIChartManager->GetXPosition("DEFAULT", x, y);
	return R_OK;
}

int CChartPeriodSummaryAddInImp::GetIndexFromValue(long lx, double dy, int &ix, int &iy)
{
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "�ڷ�����");

	try
	{
		double dTime;
		for(iy=0;iy< m_nEndIndex;iy++)
		{
			if( !pIPacket->GetData( iy, dTime)) throw this;
			if(lx == ( long)dTime)	// ������...
			{
				ix = iy;
				pIPacket->Release();
				iy++;
				return R_OK;
			}
			else if(lx < ( long)dTime)
			{
				//iy++;
				pIPacket->Release();
				return R_ERROR;
			}
		}
		
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();
	return R_ERROR; 
}

//////////////////////////////////////////////////////////////////////////
// ���� ���, ���
int CChartPeriodSummaryAddInImp::GetPriceInfo(CPeriodObj::CPrice *pPriceInfo, int nSIndex, int nEIndex)
{
	// ���� ���
	IPacket *pIPacketForMax = m_pIPacketManager->GetPacket( "��");
	IPacket *pIPacketForMin = m_pIPacketManager->GetPacket( "����");
	IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "����");
	if( !pIPacketForMax || !pIPacketForMin || !pIPacketForCur) return R_ERROR;

	pPriceInfo->avg = 0;
	
	try
	{
		double dMax=0, dMin=0, dCur=0;

		// �ʱⰪ ����
		if( !pIPacketForCur->GetData( nSIndex, pPriceInfo->sPrice)) throw this;	// ������ ����
		if( !pIPacketForCur->GetData( nEIndex, pPriceInfo->ePrice)) throw this;	// ������ ����
		if( !pIPacketForMax->GetData( nSIndex, pPriceInfo->max)) throw this;	// ��
		if( !pIPacketForMin->GetData( nSIndex, pPriceInfo->min)) throw this;	// ����

		if(nSIndex>nEIndex)
		{
			int nTemp = nEIndex;
			nEIndex = nSIndex;
			nSIndex = nTemp;
		}
		for(int i=nSIndex; i<=nEIndex;i++)
		{
			// �ִ밡 ���
			if( !pIPacketForMax->GetData( i, dMax) || !pIPacketForMin->GetData( i, dMin) || !pIPacketForCur->GetData( i, dCur))
			{
				pIPacketForMax->Release();
				pIPacketForMin->Release();
				pIPacketForCur->Release();
				return R_ERROR;
			}
			pPriceInfo->max = max(pPriceInfo->max, dMax);
			// �ּҰ� ���
			pPriceInfo->min = min(pPriceInfo->min, dMin);
			// ��հ� ���
			pPriceInfo->avg += dCur/(nEIndex-nSIndex+1);
		}		
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacketForMax->Release();
		pIPacketForMin->Release();
		pIPacketForCur->Release();
		return R_ERROR;
	}

	pIPacketForMax->Release();
	pIPacketForMin->Release();
	pIPacketForCur->Release();
	return R_OK;

}

//////////////////////////////////////////////////////////////////////////
// �� ���� ���, ���
int CChartPeriodSummaryAddInImp::GetCandleInfo(CPeriodObj::CCandle *pCandleInfo, int nSIndex, int nEIndex)
{
	if(nSIndex>nEIndex)
	{
		int nTemp = nEIndex;
		nEIndex = nSIndex;
		nSIndex = nTemp;
	}
	// ���� ���
	IPacket *pIPacketForRef = m_pIPacketManager->GetPacket( "�ð�");
	IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "����");
	if( !pIPacketForRef || !pIPacketForCur) return R_ERROR;

	try
	{
		double dRef=0, dCur=0;
		pCandleInfo->minus = 0;
		pCandleInfo->plus = 0;
		pCandleInfo->count = abs(nEIndex - nSIndex) + 1;
		for(int i=nSIndex; i<=nEIndex;i++)
		{
			// ���� ���
			if( !pIPacketForRef->GetData( i, dRef) || !pIPacketForCur->GetData( i, dCur))
			{
				pIPacketForRef->Release();
				pIPacketForCur->Release();
				return R_ERROR;
			}
			if(dCur < dRef)	// ����
				pCandleInfo->minus++;
			else
				pCandleInfo->plus++;				
		}

		pCandleInfo->CalcRatio();
		
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacketForRef->Release();
		pIPacketForCur->Release();
		return R_ERROR;
	}


	pIPacketForRef->Release();
	pIPacketForCur->Release();
	return R_OK;
}

//////////////////////////////////////////////////////////////////////////
// �ŷ��� ���, ���
int CChartPeriodSummaryAddInImp::GetAmountInfo(CPeriodObj::CAmount *pAmountInfo, int nSIndex, int nEIndex)
{
	if(nSIndex>nEIndex)
	{
		int nTemp = nEIndex;
		nEIndex = nSIndex;
		nSIndex = nTemp;
	}
	// ���� ���
	IPacket *pIPacketForAmount = m_pIPacketManager->GetPacket( "�⺻�ŷ���");
	if( !pIPacketForAmount)
	{
		pIPacketForAmount = m_pIPacketManager->GetPacket( "�����ŷ���");
		if(!pIPacketForAmount) return R_ERROR;
	}

	pAmountInfo->avg = 0;

	try
	{
		double dAmount=0;
		if( !pIPacketForAmount->GetData( nSIndex, pAmountInfo->max) || !pIPacketForAmount->GetData( nSIndex, pAmountInfo->min))
		{
			pIPacketForAmount->Release();
			return R_ERROR;
		}

		for(int i=nSIndex; i<=nEIndex;i++)
		{
			// min max ���
			if( !pIPacketForAmount->GetData( i, dAmount))
			{
				pIPacketForAmount->Release();
				return R_ERROR;
			}
			pAmountInfo->max = max(pAmountInfo->max, dAmount);
			pAmountInfo->min = min(pAmountInfo->min, dAmount);
			pAmountInfo->avg += dAmount/(nEIndex-nSIndex+1);
		}
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacketForAmount->Release();
		return R_ERROR;
	}

	pIPacketForAmount->Release();
	return R_OK;
}

void CChartPeriodSummaryAddInImp::OnPSConfig(CPeriodObj* pLine)
{
	CPSConfigDlg dlg;

	m_pCurSelectedLine = pLine;

	//������ ǥ�� �÷���
	dlg.m_SPrice = m_pCurSelectedLine->m_Price.sPrice;
	dlg.m_EPrice = m_pCurSelectedLine->m_Price.ePrice;
	dlg.m_bSEPrice = m_pCurSelectedLine->m_bSEPrice;
	dlg.m_bEEPrice = m_pCurSelectedLine->m_bEEPrice;
	dlg.m_bHPrice = m_pCurSelectedLine->m_bHPrice;
	dlg.m_bLPrice = m_pCurSelectedLine->m_bLPrice;
	dlg.m_bMPrice = m_pCurSelectedLine->m_bMPrice;
	dlg.m_bHAmount = m_pCurSelectedLine->m_bHAmount;
	dlg.m_bLAmount = m_pCurSelectedLine->m_bLAmount;
	dlg.m_bMAmount = m_pCurSelectedLine->m_bMAmount;
	dlg.m_bPCandle = m_pCurSelectedLine->m_bPCNum;
	dlg.m_bMCandle = m_pCurSelectedLine->m_bMCNum;
	dlg.m_bPMRatio = m_pCurSelectedLine->m_bPMRatio;

	// ����
	dlg.m_lSDate = m_pCurSelectedLine->m_lSDate;
	dlg.m_lEDate = m_pCurSelectedLine->m_lEDate;

	// Ŭ�� ����Ʈ ��
	dlg.m_SPrice = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAStart.y, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	dlg.m_EPrice = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAEnd.y, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

	// �� ��
	dlg.m_clrLineColor = m_pCurSelectedLine->m_clrLineColor;
	// �� ����
	dlg.m_nLineThickness = m_pCurSelectedLine->m_nLineThickness-1;
	dlg.m_nLineStyle = m_pCurSelectedLine->m_nLineStyle;
	dlg.m_nDateType = m_nDateType;

	dlg.m_nTextAlignment = m_pCurSelectedLine->m_nTextAlignment;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bAllChange)
		{
			POSITION pos = m_LineList.GetHeadPosition();
			while (pos)
			{
				CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
				pLine->m_nRowCnt		= dlg.m_nCnt + 1;
				pLine->m_bSEPrice		= dlg.m_bSEPrice;
				pLine->m_bEEPrice		= dlg.m_bEEPrice;
				pLine->m_bHPrice		= dlg.m_bHPrice;
				pLine->m_bLPrice		= dlg.m_bLPrice;
				pLine->m_bMPrice		= dlg.m_bMPrice;
				pLine->m_bHAmount		= dlg.m_bHAmount;
				pLine->m_bLAmount		= dlg.m_bLAmount;
				pLine->m_bMAmount		= dlg.m_bMAmount;
				pLine->m_bPCNum			= dlg.m_bPCandle;
				pLine->m_bMCNum			= dlg.m_bMCandle;
				pLine->m_bPMRatio		= dlg.m_bPMRatio;
				pLine->m_nLineThickness	= dlg.m_nLineThickness+1;
				pLine->m_nLineStyle		= dlg.m_nLineStyle;
				pLine->m_lSDate			= dlg.m_lSDate;
				pLine->m_lEDate			= dlg.m_lEDate;

				pLine->m_clrLineColor	= dlg.m_clrLineColor;
				pLine->m_nTextAlignment	= dlg.m_nTextAlignment;
			}
		}
		else
		{
			m_pCurSelectedLine->m_nRowCnt			= dlg.m_nCnt + 1;
			m_pCurSelectedLine->m_bSEPrice			= dlg.m_bSEPrice;
			m_pCurSelectedLine->m_bEEPrice			= dlg.m_bEEPrice;
			m_pCurSelectedLine->m_bHPrice			= dlg.m_bHPrice;
			m_pCurSelectedLine->m_bLPrice			= dlg.m_bLPrice;
			m_pCurSelectedLine->m_bMPrice			= dlg.m_bMPrice;
			m_pCurSelectedLine->m_bHAmount			= dlg.m_bHAmount;
			m_pCurSelectedLine->m_bLAmount			= dlg.m_bLAmount;
			m_pCurSelectedLine->m_bMAmount			= dlg.m_bMAmount;
			m_pCurSelectedLine->m_bPCNum			= dlg.m_bPCandle;
			m_pCurSelectedLine->m_bMCNum			= dlg.m_bMCandle;
			m_pCurSelectedLine->m_bPMRatio			= dlg.m_bPMRatio;
			m_pCurSelectedLine->m_nLineThickness	= dlg.m_nLineThickness+1;
			m_pCurSelectedLine->m_nLineStyle		= dlg.m_nLineStyle;
			m_pCurSelectedLine->m_lSDate			= dlg.m_lSDate;
			m_pCurSelectedLine->m_lEDate			= dlg.m_lEDate;
			m_pCurSelectedLine->m_clrLineColor		= dlg.m_clrLineColor;
			m_pCurSelectedLine->m_nTextAlignment	= dlg.m_nTextAlignment;
		}

		// ��ġ ���� (X)
		int iDummy=0;
		GetIndexFromValue( dlg.m_lSDate, 0, m_pCurSelectedLine->m_nStartIndex, iDummy);
		GetPointFromIndex(m_pCurSelectedLine->m_nStartIndex, 0,m_pCurSelectedLine->m_ptAStart);
		GetIndexFromValue( dlg.m_lEDate, 0, m_pCurSelectedLine->m_nEndIndex, iDummy);
		GetPointFromIndex(m_pCurSelectedLine->m_nEndIndex, 0,m_pCurSelectedLine->m_ptAEnd);
		// ��ġ ����(Y)
		m_pCurSelectedLine->m_ptAStart.y = m_pIChartOCX->ConvertDataToYPosition(dlg.m_SPrice, m_dViewMax, m_dViewMin, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_bLog, m_bReverse);
		m_pCurSelectedLine->m_ptAEnd.y = m_pIChartOCX->ConvertDataToYPosition(dlg.m_EPrice, m_dViewMax, m_dViewMin, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_bLog, m_bReverse);
		m_pIChartOCX->InvalidateControl();
	}
}

void CChartPeriodSummaryAddInImp::OnDeleteLine()
{
		CDC * pDC = CDC::FromHandle(m_hDC);
//		m_pCurSelectedLine->Erase(pDC);
		UnregPS((long)m_pCurSelectedLine);
		delete m_pCurSelectedLine;
		m_LineList.RemoveAt(m_PosForRemove);
		m_pIChartOCX->InvalidateControl();
}

void CChartPeriodSummaryAddInImp::OnReleaseDraw()
{
	m_nCommandType = 2;
	if( m_pIChartOCX != NULL )
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );	
}

//void CChartPeriodSummaryAddInImp::MakeInfo()
//{
//
//}

int CChartPeriodSummaryAddInImp::GetDateType()
{
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "�ڷ�����");
	if( !pIPacket) return FALSE;

	try
	{
//		double dTime;
		ILPCSTR is = pIPacket->GetType();
		CString s = is;
		if(s == "YYYYMMDD")
		{
			m_nDateType = 0;
		}
		else if(s == "MMDDHHMM")
		{
			m_nDateType = 1;
		}
		else if(s == "HHMMSS")
		{
			m_nDateType = 2;
		}
		else if(s == "DDHHMMSS")
		{
			m_nDateType = 3;
		}
		else if(s == "YYYYMM")
		{
			m_nDateType = 4;
		}

	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();

	return R_OK;
}

RECT CChartPeriodSummaryAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	// 1. DC�� Handle�� ���Ѵ�.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	if( m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
		if(!m_pCurSelectedLine) return rct;
		CRgn rgn;
		rgn.CreateRectRgn((int)m_rctGraphRegion.left, (int)m_rctGraphRegion.top, (int)m_rctGraphRegion.right, (int)m_rctGraphRegion.bottom);
		pDC->SelectClipRgn(&rgn);

		m_pCurSelectedLine->SetPos(m_ptCursor, m_pCurSelectedLine->m_CurClickedPosition);

		//////////////////////////////////////////////////////////////////////////
		// �������� �������� ������ ��
		if(m_pCurSelectedLine->m_ptAStart.x > m_pCurSelectedLine->m_ptAEnd.x)	// ��ġ ����
		{
			CPoint ptTemp;
			if(m_pCurSelectedLine->m_CurClickedPosition == E_PT)
			{
				m_pCurSelectedLine->m_CurClickedPosition = S_PT;
			}
			else if(m_pCurSelectedLine->m_CurClickedPosition == S_PT)
			{
				m_pCurSelectedLine->m_CurClickedPosition = E_PT;
			}
			ptTemp = m_pCurSelectedLine->m_ptAStart;
			m_pCurSelectedLine->m_ptAStart = m_pCurSelectedLine->m_ptAEnd;
			m_pCurSelectedLine->m_ptAEnd = ptTemp;

		}
	
		int ix, iy;
		double dDummy = 0;
		m_pCurSelectedLine->m_nDateType = m_nDateType;
		// ������ �� ����
		if(GetIndexFromPoint(m_pCurSelectedLine->m_ptAStart, ix, iy) == R_ERROR) goto Drawing;
		if(IsValidIndex(ix))
			m_pCurSelectedLine->m_nStartIndex = ix;
 		//if(GetValueFromIndex(ix, iy, m_pCurSelectedLine->m_lSDate, dDummy) == R_ERROR) goto Drawing;
		if(GetData_X(m_pCurSelectedLine->m_ptAStart, m_pCurSelectedLine->m_lSDate) == R_ERROR) goto Drawing;
		// ���� �� ����
		if(GetIndexFromPoint(m_pCurSelectedLine->m_ptAEnd, ix, iy) == R_ERROR) goto Drawing;
		if(IsValidIndex(ix))
			m_pCurSelectedLine->m_nEndIndex = ix;
 		//if(GetValueFromIndex(ix, iy, m_pCurSelectedLine->m_lEDate, dDummy) == R_ERROR) goto Drawing;
		//m_pCurSelectedLine->m_lEDate = GetData_X(m_pCurSelectedLine->m_ptAEnd);
		if(GetData_X(m_pCurSelectedLine->m_ptAEnd, m_pCurSelectedLine->m_lEDate) == R_ERROR) goto Drawing;
		if(m_bNewDraw == FALSE)
		{
			// �߰��� ��ġ
			if(GetIndexFromPoint(m_pCurSelectedLine->m_ptAt, ix, iy) == R_ERROR) goto Drawing;
			if(!IsValidIndex(ix)) goto Drawing;
		}
		// ����, ��, �ŷ��� ����
		if(GetPriceInfo(&m_pCurSelectedLine->m_Price, m_pCurSelectedLine->m_nStartIndex, m_pCurSelectedLine->m_nEndIndex) == R_ERROR) return rct;
		if(GetCandleInfo(&m_pCurSelectedLine->m_Candle, m_pCurSelectedLine->m_nStartIndex, m_pCurSelectedLine->m_nEndIndex) == R_ERROR) return rct;
		if(GetAmountInfo(&m_pCurSelectedLine->m_Amount, m_pCurSelectedLine->m_nStartIndex, m_pCurSelectedLine->m_nEndIndex) == R_ERROR) return rct;

		// Ŭ�� ����Ʈ ��
		m_pCurSelectedLine->m_dSClicked = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAStart.y, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
		m_pCurSelectedLine->m_dEClicked = m_pIChartOCX->ConvertYPositionToData(m_pCurSelectedLine->m_ptAEnd.y, m_rctGraphRegion.top, m_rctGraphRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

Drawing:
		m_pCurSelectedLine->Draw(pDC);
		//pDC->SelectClipRgn(&rgn, NULLREGION);
		pDC->SelectClipRgn(NULL);
		rgn.DeleteObject();
	}
	
///////////////////////////////////////////////////
	return rct;
}

BOOL CChartPeriodSummaryAddInImp::GetSpanTime( CPoint ptPoint )
{
	// 1. ���� ȭ�鿡 ���̴� Data�� Start/End Index�� ���Ѵ�.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
		return FALSE;

	// 2. Mouse Point�� Data Index�� ���Ѵ�.
	int nDataOnPage = m_nEndIndex - m_nStartIndex + 1;
	double dBarWidth = double( m_rctGraphRegion.Width() ) / double( nDataOnPage );
	int nDataIndex = m_nStartIndex + int( double( ptPoint.x - m_rctBlockRegion.left ) / dBarWidth );

	// 3. ���� Mouse Point�� �̿��Ͽ� �� Span�� Index�� ���Ѵ�.
	m_nPrevSpanIndex = nDataIndex - m_nPrevSpan;
	m_nAfterSpanIndex = nDataIndex + m_nAfterSpan;

	// 4. �� Span�� ���� Point�� ����Ѵ�.
	int nBarLeftEdge, nBarRightEdge, nX;
	int nDataOffset = m_nPrevSpanIndex - m_nStartIndex;
	nBarLeftEdge = m_rctGraphRegion.left + int( dBarWidth * double( nDataOffset) + 0.5) + 1;
	nBarRightEdge = m_rctGraphRegion.left + int( dBarWidth * double( nDataOffset + 1) + 0.5) - 1;
	nX = ( nBarLeftEdge + nBarRightEdge) / 2;

	m_ptPrevSpan.x = nX;
	m_ptPrevSpan.y = ptPoint.y;

	nDataOffset = m_nAfterSpanIndex - m_nStartIndex;
	nBarLeftEdge = m_rctGraphRegion.left + int( dBarWidth * double( nDataOffset) + 0.5) + 1;
	nBarRightEdge = m_rctGraphRegion.left + int( dBarWidth * double( nDataOffset + 1) + 0.5) - 1;
	nX = ( nBarLeftEdge + nBarRightEdge) / 2;

	m_ptAfterSpan.x = nX;
	m_ptAfterSpan.y = ptPoint.y;

	// 5. Block���� XScale �����ڸ� ���´�.
	int nR, nC;
	if( !m_pIChartManager->GetBlockIndex( ptPoint, nR, nC))
		return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock)
		return FALSE;

	pIBlock->Release();
	return TRUE;
}

void CChartPeriodSummaryAddInImp::OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB)
{
	m_bMouseCaptured = FALSE;
}

BOOL CChartPeriodSummaryAddInImp::GetStringfromDataIdx( int nDataIdx, CString &strSpan )
{
	// 1. �ڷ����� Packet�� ���Ѵ�.
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "�ڷ�����");
	if( !pIPacket)
	{
		strSpan = "00/00/00";
		return FALSE;
	}

	// 2. Data�� ���Ѵ�.
	double dMarkTime;
	if( !pIPacket->GetData( nDataIdx, dMarkTime ))
		pIPacket->GetDataPastandFuture( nDataIdx, dMarkTime );
	
	long lMarkTime = (long)dMarkTime;
	CString szMarkTime;
	szMarkTime.Format("%d", lMarkTime);

	// 3. Packet Type�� ���Ѵ�.
	ILPCSTR szPacketType = pIPacket->GetType();
	CString strPacketType( szPacketType);
	
	int nPacketTypeLength = strPacketType.GetLength();

	// 4. PacketType�� ���̿� Data�� ���̰� �ٸ��� '0'���� ä���ش�.
	for( int i = nPacketTypeLength - szMarkTime.GetLength() ; i ; i-- )
		szMarkTime = "0" + szMarkTime;

	char strChar = strPacketType.GetAt(0);
	strSpan = szMarkTime.GetAt(0);

	// 5. XScale�� ���Ǵ� ������ �� Type�� �����Ѵ�.
	CString strCompartment = m_strDateCompartment;
	BOOL bUseOneCharDateCompartment = ( m_strDateCompartment.GetLength() > 1 ) ? FALSE : TRUE;
	BOOL bUseOneCharTimeCompartment = ( m_strTimeCompartment.GetLength() > 1 ) ? FALSE : TRUE;

	
	// "��"�� "��"�� PacketType�� ���� 'M'�� ����ϱ� ������ �������ش�.
	BOOL bIsMonth = FALSE;				
	CString strPacketChar = "";
	
	// 6. PacketType�� ���̸�ŭ ���ڿ��� parsing�Ѵ�.
	for( int nIdx = 1 ; nPacketTypeLength > nIdx ; nIdx++ )
	{
		strPacketChar = strPacketType.GetAt(nIdx);

		// 6.1 PacketType������ Data�� Type�� �ٲ�� �ش�Ǵ� �����ڸ� ���ڿ��� �߰����ش�.
		if( strChar != strPacketChar )
		{
			// 6.1.1 Date�� �����ڰ� "�� �� ��"�� ����� ��� 
			if( !bUseOneCharDateCompartment )
			{
				if( strChar == 'Y' && strPacketChar == 'M' )
				{
					strCompartment = "��";
					bIsMonth = TRUE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'D' )		strCompartment = "��";
				else if( strChar == 'D' && strPacketChar == 'H' )		strCompartment = "��";
			}

			// 6.1.2 Time�� �����ڰ� "�� �� ��"�� ����� ��� 
			if( !bUseOneCharTimeCompartment )
			{
				if( strChar == 'H' && strPacketChar == 'M' )
				{
					strCompartment = "��";
					bIsMonth = FALSE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'S' )		strCompartment = "��";
				else if( strChar == 'S' && strPacketChar == 'N' )		strCompartment = "��";
			}


			// 6.1.3 ���� �����ڰ� �Ϲ� �����ϰ�� ��¥���� �ð����� ����� �� ���� ���ڸ� �������ش�.
			if( strChar == 'H' && strPacketChar == 'M' && bUseOneCharTimeCompartment )
				strCompartment = m_strTimeCompartment;

			strSpan = strSpan + strCompartment + szMarkTime.GetAt(nIdx);

			strChar = strPacketType.GetAt(nIdx);
		}

		// 6.2 PacketType�� ������� ������ ���ڸ� 
		else
		{
			// 6.2.1 PacketType�� ������ ������ Span���ڸ� �����ش�.
			strSpan = strSpan + szMarkTime.GetAt(nIdx);
			
			// 6.2.2 szMarkTime�� ��� parsing�ϰ� ���� ���������� �����ڸ� �ٿ��� �� ��츦 Ȯ���ϰ� �ٿ��ش�.
			if( nPacketTypeLength == ( nIdx + 1 ) )
			{
				if( strChar == 'Y' && !bUseOneCharDateCompartment )
					strSpan = strSpan + "��";
				
				else if( strChar == 'M' && !bUseOneCharDateCompartment && bIsMonth )
					strSpan = strSpan + "��";

				else if( strChar == 'D' && !bUseOneCharDateCompartment )
					strSpan = strSpan + "��";
				
				else if( strChar == 'H' && !bUseOneCharTimeCompartment )
					strSpan = strSpan + "��";
				
				else if( strChar == 'M' && !bUseOneCharTimeCompartment && !bIsMonth)
					strSpan = strSpan + "��";
				
				else if( strChar == 'S' && !bUseOneCharTimeCompartment )
					strSpan = strSpan + "��";
			}
		}
	}

	pIPacket->Release();
	return TRUE;
}



void CChartPeriodSummaryAddInImp::RegPS(long pLine)
{
	if( m_pIAddInToolMgr != NULL )
	{
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_ADD_TOOL_TO_ORDERMANAGER,	
											  m_strAddInItemName,		
											  pLine );	
	}
}

void CChartPeriodSummaryAddInImp::UnregPS(long pLine)
{
	if( m_pIAddInToolMgr != NULL )
	{
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_TOOL_FROM_ORDERMANAGER,	
											  m_strAddInItemName,		
											  pLine );	
	}
}

void CChartPeriodSummaryAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	if(p_pIAddInEnvMap->GetCount() <= 0)
		return;

	char *chToolLoading;
	p_pIAddInEnvMap->Lookup(m_strAddInItemName,chToolLoading);
	m_strToolLoading.Format("%s",chToolLoading);
	if(m_strToolLoading.IsEmpty() == TRUE)
	{
//		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );
//		m_nCommandType = 3;
		return;
	}
	// ���ӱ׸��� ����
	CString strLineInfo, strPreLineInfo(_T(""));
	int iStart = 0;

	while(iStart >= 0)
	{
		strLineInfo = TokenizeEx(m_strToolLoading, ";", iStart);

		if(iStart == -1 || strPreLineInfo == strLineInfo)	// No data
			break;

		strPreLineInfo = strLineInfo;

		strLineInfo += ",";
		CPeriodObj* pLine;
		pLine = new CPeriodObj;
//		pLine->Initilize();
		int iStart2 = 0;
		pLine->m_bSaved = TRUE;
		pLine->m_nDateType = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nUpDown = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_lSDate = atol(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_lEDate = atol(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_Price.sPrice = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_Price.ePrice = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nLineThickness = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nLineStyle = atoi(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_bSEPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));			// ������ ����
		pLine->m_bEEPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));			// ������ ����
		pLine->m_bHPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));				// ��
		pLine->m_bLPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));				// ����
		pLine->m_bMPrice = atoi(TokenizeEx(strLineInfo, ",", iStart2));				// ��հ�
		pLine->m_bHAmount = atoi(TokenizeEx(strLineInfo, ",", iStart2));			//�ִ� �ŷ���
		pLine->m_bLAmount = atoi(TokenizeEx(strLineInfo, ",", iStart2));			// �ּ� �ŷ���
		pLine->m_bMAmount = atoi(TokenizeEx(strLineInfo, ",", iStart2));			// ��� �ŷ���
		pLine->m_bPCNum = atoi(TokenizeEx(strLineInfo, ",", iStart2));				//��� ��
		pLine->m_bMCNum = atoi(TokenizeEx(strLineInfo, ",", iStart2));				// ���� ��
		pLine->m_bPMRatio = atoi(TokenizeEx(strLineInfo, ",", iStart2));			//������ ����
		pLine->m_dSClicked = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_dEClicked = atof(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_clrLineColor = atol(TokenizeEx(strLineInfo, ",", iStart2));
		pLine->m_nRowCnt = atoi(TokenizeEx(strLineInfo, ",", iStart2)); 
		pLine->m_nTextAlignment = atoi(TokenizeEx(strLineInfo, ",", iStart2)); 

		m_LineList.AddTail(pLine);
	}

	//m_pIChartOCX->InvalidateControl();
//>> [QC No]090728HMC016
//	if(m_nCommandType == 0)
	if(m_nCommandType == 0 && m_pCurSelectedLine)
		m_nCommandType = 2;
//<< [QC No]090728HMC016
//	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_TOOL_PERIOD_SUMMARY, FALSE );
}

void CChartPeriodSummaryAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	CString strSaveToolList = "";;// = m_pAnalysisTool->GetSavedElementsData();
	CString strTemp;

	POSITION pos = m_LineList.GetHeadPosition();
	while(pos)
	{
		CPeriodObj* pLine = (CPeriodObj*)m_LineList.GetNext(pos);
		strTemp.Format("%d,%d,%ld,%ld,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%ld,%d,%d;"
						, pLine->m_nDateType
						, pLine->m_nUpDown
						, pLine->m_lSDate
						, pLine->m_lEDate
						, pLine->m_Price.sPrice
						, pLine->m_Price.ePrice
						, pLine->m_nLineThickness
						, pLine->m_nLineStyle
						, pLine->m_bSEPrice			// ������ ����
						, pLine->m_bEEPrice			// ������ ����
						, pLine->m_bHPrice				// ��
						, pLine->m_bLPrice				// ����
						, pLine->m_bMPrice				// ��հ�
						, pLine->m_bHAmount			//�ִ� �ŷ���
						, pLine->m_bLAmount			// �ּ� �ŷ���
						, pLine->m_bMAmount			// ��� �ŷ���
						, pLine->m_bPCNum				//��� ��
						, pLine->m_bMCNum				// ���� ��
						, pLine->m_bPMRatio			//������ ����
						, pLine->m_dSClicked
						, pLine->m_dEClicked
						, pLine->m_clrLineColor
						, pLine->m_nRowCnt
						, pLine->m_nTextAlignment
						);
	strSaveToolList += strTemp;

	}
	p_pIAddInEnvMap->SetAt(m_strAddInItemName,strSaveToolList);
}

CString CChartPeriodSummaryAddInImp::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	CString strRes = _T("");
	int iCur = iStart;
	iStart = strSrc.Find(pszTokens, iCur);
	if(iStart != -1)
	{
		strRes = strSrc.Mid(iCur, iStart-iCur);
		iStart++;
		return strRes;
	}
	return strRes;
}

BOOL CChartPeriodSummaryAddInImp::IsValidIndex(int nIndex)
{
	IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "����");
	try
	{
		double dCur=0;
		if( !pIPacketForCur->GetData( nIndex, dCur)) throw this;	// ������ ����
	}
	catch( CChartPeriodSummaryAddInImp *)
	{
		pIPacketForCur->Release();
		return FALSE;
	}
	pIPacketForCur->Release();
	return TRUE;
}

// protected ==================================================================
// XScale : ���� Interface�� ��� - vntsorl(20070521)
BOOL CChartPeriodSummaryAddInImp::GetData_X(const CPoint& point, long& lx)
{
	int nBlockColumn, nCenter_x;
	int nRow, nCol;
	m_pIChartManager->GetBlockIndex(point, nRow, nCol);
	if(nRow<0 || nCol<0)
		return R_ERROR;
	else
		lx = (int)(m_pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x));
	return R_OK;
}

void CChartPeriodSummaryAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
}

void CChartPeriodSummaryAddInImp::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	if(p_bTrUpdate) return;
}