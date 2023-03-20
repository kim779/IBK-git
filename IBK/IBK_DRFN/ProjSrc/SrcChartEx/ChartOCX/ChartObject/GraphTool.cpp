// GraphTool.cpp: implementation of the CGraphTool class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphTool.h"

#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl
#include "BlockImp.h"
#include "VertScaleGroup.h"
#include "GraphImp.h"
#include "SubGraph.h"
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CGraphTool

CGraphTool::CGraphTool():
	m_bIsGraphMove(true),
	m_bIsGraphDelete(true),
	m_lBtDownPt(CPoint(0, 0)),
	m_graphMove(CBaseTool::GetParent())
{
}


// public =====================================================================
void CGraphTool::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	CBaseTool::SetIChartCtrl(p_pICharetCtrl);

	if(p_pICharetCtrl == NULL)
		m_graphMove.InitializeAllData(NULL);
	else
		m_graphMove.InitializeAllData(p_pICharetCtrl->GetChartWindowPointer());
}

void CGraphTool::SetGraphMove(const bool bIsGraphMove)
{
	m_bIsGraphMove = bIsGraphMove;
}

void CGraphTool::SetGraphDelete(const bool bIsGraphDelete)
{
	m_bIsGraphDelete = bIsGraphDelete;
}

// ----------------------------------------------------------------------------
// ��� data�� �ʱ�ȭ �Ѵ�.
void CGraphTool::InitializeAllData()
{
	CBaseTool::InitializeAllData();
	CBaseTool::ChangeCursor(CBaseToolEnumData::CTNONE);
	m_graphMove.InitializeAllData(CBaseTool::GetParent());
}

// ----------------------------------------------------------------------------
bool CGraphTool::OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	m_lBtDownPt = point;
	CBaseTool::SetProcessEnded(false);
	OnLButtonDown_Real(pDC, pMainBlock, nFlags, point);
	
	SetGraphMoving(IsSelectedSubGraph());
	return true;
}

bool CGraphTool::OnLButtonUp(CDC* /*pDC*/, CMainBlock* pMainBlock, const CPoint& point)
{
	bool result = OnLButtonUp();
	if(result)
		GraphMoving(pMainBlock, point);

	SetGraphMoving(false);
	return result;
}

void CGraphTool::OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	CBaseTool::ChangeCursor(GetCursorType(pMainBlock, point));
	OnMouseMove(pDC, pMainBlock);
}

bool CGraphTool::OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	if(!IsSelectedSubGraph())
		return false;

	if(menuType == CMenuType::MENU_DELETE_ALL)
	{
		CGraphImp* pGraph = GetGraphMoving();
		if(pGraph)
		{
			CString strIndicatorName = pGraph->GetGraphData().GetIndicatorName2();
			//KHD
			if( g_iMetaTable.IsSpecialGroupGraph(strIndicatorName) )
			{
				AfxMessageBox( "<Ư����Ʈ>�� ���� �� �� �����ϴ�!!", MB_ICONWARNING|MB_OK);
				return false;
			}
			//END
			// ������Ʈ ���� : �������� - ojtso (20070522)
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if( strIndicatorName == _MTEXT( C2_PRICE_CHART) && !::strncmp(pGraph->GetGraphRQ(), _T("DEFAULT"), 7))
			{
// 				ML_SET_LANGUAGE_RES();
// 				CString strFormat;
// 				strFormat.LoadString( IDS_CANNOT_DELETE_IT);
// 				CString strTitle;
// 				strTitle.LoadString( IDS_NOTATION);
// 				CString message;
// 				message.Format( strFormat, _MTOL2( strIndicatorName));
// 				if( !m_pParent) AfxMessageBox(message, MB_ICONWARNING | MB_OK);
// 				else MessageBox( m_pParent->m_hWnd, message, strTitle, MB_ICONWARNING | MB_OK);
				
				//NH:100528 ������Ʈ �������ϹǷ� ����.
				return false;
			}

			// vntsorl_20110608:[A00000600] �߰��� Graph�� RMenu ��ü������ �ȵǴ� ���� ����
			CString strGraphName = pGraph->GetGraphData().GetGraphName2();
			return pMainBlock->DeleteGraph(strGraphName);
		}
		return false;
	}

	if(menuType == CMenuType::MENU_DELETE){
		if(!m_bIsGraphMove)
			return false;
		//CSubGraph �Ҹ��ڿ��� SendMessage �ϱ⶧���� ����.
		//if(IsSendMessage_SubGraph())
		//	SendMessage_SubGraph();
	}

	return OnMenu_SubGraph(pMainBlock, menuType, wParam);
}

// ��ǥ ������ - ojtaso (20071023)
bool CGraphTool::GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	if(!IsSelectedSubGraph())
		return false;

	if(menuType == CMenuType::MENU_DELETE){
		if(!m_bIsGraphMove)
			return false;
		//CSubGraph �Ҹ��ڿ��� SendMessage �ϱ⶧���� ����.
		//if(IsSendMessage_SubGraph())
		//	SendMessage_SubGraph();
	}

	return GetMenuCheck_SubGraph(pMainBlock, menuType, wParam);
}

// protected ==================================================================
// graph ��������.
CGraphImp* CGraphTool::GetGraph(CSubGraph* pSubGraph) const
{
	if(pSubGraph == NULL)
		return NULL;
	return pSubGraph->GetSubGraphList()->GetGraph();
}

// ----------------------------------------------------------------------------
// data index�� ocx�� �ֱ�
/*bool CGraphTool::OnMenu_DataIndex(const CSelectedSubGraphData& selectedSubGraphData)
{
	if(selectedSubGraphData.GetSubGraph() == NULL || CBaseTool::GetParent() == NULL)
		return false;

	LPCTSTR GraphName = selectedSubGraphData.GetGraph()->GetGraphData().GetGraphName();
	CBaseTool::GetParent()->SendMessage(UM_GRAPH_SELECTEDDATAINDEX, selectedSubGraphData.GetDataIndex(), (LPARAM)GraphName);
	return true;
}*/

bool CGraphTool::OnMenu_DataIndex(const CSelectedSubGraphData& selectedSubGraphData)
{
	if(selectedSubGraphData.GetSubGraph() == NULL || CBaseTool::GetIChartCtrl() == NULL)
		return false;

	CBaseTool::GetIChartCtrl()->SetGraphSelectedDataIndex(selectedSubGraphData.GetDataIndex(), 
		selectedSubGraphData.GetGraph()->GetGraphData().GetGraphName());
	return true;
}

// <��ǥ����> �� �°� ���� �����Ų��. - graph ���� �� ����.
bool CGraphTool::LoadIndicatorConfiguration(CMainBlock* pMainBlock, CSubGraph* pSubGraph)
{
	if( !pSubGraph) return false;
	CGraphImp *pGraph = pSubGraph->GetGraph();
	if( !pGraph) return false;
	pGraph->ChangeIndicatorInfoFromGraphData();
	CIndicatorInfo * pIndicatorInfo = pGraph->GetIndicatorInfo();
	// "��ǥ�� �ƴ� ���" : �ӽ÷� CIndcatorInfo �� ����� �ش�.
	CIndicatorInfo *pIndicatorInfo2 = NULL;
	if( !pIndicatorInfo)
	{
		pIndicatorInfo2 = new CIndicatorInfo( EIIT_GENERAL);
		pGraph->ChangeIndicatorInfoFromGraphData( *pIndicatorInfo2);
		pIndicatorInfo = pIndicatorInfo2;
	}

	bool bResult = CBaseTool::GetIChartCtrl()->ShowIndicatorConfigDlg( pIndicatorInfo) == IDOK;
	if( pIndicatorInfo2)
	{
		// (2007/1/12 - Seung-Won, Bae) Update with Temporary IndicatorInfo.
		pGraph->ChangeGraphDataFromIndicatorInfo( *pIndicatorInfo2, FALSE);
		delete pIndicatorInfo2;
	}
	return bResult;
}

// ���� ���õ� subGraph�� vert scale�� ���̰� �Ѵ�.
// -> ���� ���õ� graph�� vert scale�� �����ϰ� ��� vert scale�� <����>���� �Ѵ�.
// -> <����>�� �ƴ� vert scale�� grid type�� <����>���� �Ѵ�.
void CGraphTool::ChangeAllVertScaleGridType(CVertScaleGroup* pVertScaleGroup)
{
	if( pVertScaleGroup == NULL) return;

	CVertScaleGroupList *pVertScaleGroupList = pVertScaleGroup->GetVertScaleGroupList();
	if( !pVertScaleGroupList) return;
	pVertScaleGroupList->SetActivate( pVertScaleGroup);
}

// ----------------------------------------------------------------------------
// ���õ� Graph�� Type�� ocx�� �޽����� ������.
void CGraphTool::SendMessage_GraphType(CSubGraph* pSubGraph)
{
	if(pSubGraph == NULL || CBaseTool::GetIChartCtrl() == NULL)
		return;

	//CBaseTool::GetParent()->SendMessage(UM_GRAPH_THICKNESS, pSubGraph->GetSubGraphData().GetPenThickness().GetDrawingThickness1());
	//CBaseTool::GetParent()->SendMessage(UM_GRAPH_TYPE_STYLE, (int)pSubGraph->GetSubGraphData().GetType(), pSubGraph->GetSubGraphData().GetDrawStyle().GetDrawStyle());
	CBaseTool::GetIChartCtrl()->SetGraphThickness(pSubGraph->GetSubGraphData().GetPenThickness().GetDrawingThickness1());
	CBaseTool::GetIChartCtrl()->SetGraphTypeStyle(pSubGraph->GetSubGraphData().GetType(), pSubGraph->GetSubGraphData().GetDrawStyle().GetDrawStyle());
}

// private ====================================================================
bool CGraphTool::IsCursorType_NoneMode(const CPoint& point) const
{
	if(!m_bIsGraphMove || !m_graphMove.IsGraphMoving())
		return true;

	CRect offsetRegion(m_lBtDownPt, m_lBtDownPt + CPoint(1, 1));
	offsetRegion.InflateRect(3, 3);
	return ( offsetRegion.PtInRect(point) ? true : false);
}

// ----------------------------------------------------------------------------
// block ��������.
CBlockImp* CGraphTool::GetBlock(CGraphImp* pGraph) const
{
	if(pGraph == NULL)
		return NULL;
	return pGraph->GetVertScaleGroup()->GetVertScaleGroupList()->GetBlock();
}

// cursor type ��������.
CBaseToolEnumData::CURSORTYPE CGraphTool::GetCursorType(CMainBlock* pMainBlock, const CPoint& point) const
{
	if(pMainBlock == NULL || IsCursorType_NoneMode(point))
		return CBaseToolEnumData::CTNONE;

	// Graph �и�/���� - ojtaso (20070731)
	int nInsertCol = -1;
	int nInsertRow = -1;
	CRect graphRegion = pMainBlock->GetNoFrameGraphRegion(point, nInsertCol, nInsertRow);
	if(!graphRegion.IsRectEmpty())
	{
		if(nInsertCol >= 0 && nInsertRow >= 0)
			return CBaseToolEnumData::INSERT;
		else
			return CBaseToolEnumData::MOVE;
	}
	
	return CBaseToolEnumData::REMOVE;
}

// ----------------------------------------------------------------------------
void CGraphTool::SetGraphMoving(const bool bIsGraphMoving)
{
	if(!m_bIsGraphMove)
		return;

	m_graphMove.SetGraphMoving(bIsGraphMoving);

	if(!bIsGraphMoving)
		CBaseTool::ChangeCursor(CBaseToolEnumData::CTNONE);
}

// graph move -> �ϳ��� �̵��ȴ�!!
bool CGraphTool::GraphMoving(CMainBlock* pMainBlock, const CPoint& targetPoint)
{
	if(!m_bIsGraphMove){
		GraphMoving_Ended(false);
		return false;
	}

	CGraphImp* pGraph = GetGraphMoving();
	bool bResult = m_graphMove.GraphMoving(pMainBlock, pGraph, targetPoint);
	GraphMoving_Ended(bResult);
	return bResult;
}

// ��ǥ ������ - ojtaso (20071023)
bool CGraphTool::OnMenu_SubGraphInvert(CMainBlock* pMainBlock, CSubGraph* pSubGraph)
{
	CGraphImp* pGraph = pSubGraph->GetGraph();
	CVertScaleGroup* pGroup = pGraph->GetVertScaleGroup();

	int nCount = pGroup->GetGraphCount();
	
	if(nCount > 1)
	{
		CStringList p_slVertScaleGroupData;
		
		p_slVertScaleGroupData.AddTail( "VertScaleGroup;\r\n");
		
		p_slVertScaleGroupData.AddTail( CString());
		CString &strVertScaleData = p_slVertScaleGroupData.GetTail();
		pGroup->GetVertScale().GetScaleAllData( strVertScaleData);
		
//		pGraph->GetStringGraphData( p_slVertScaleGroupData);
		CGraphImp* pFindGraph = IsGeneralGraph(pGraph, pGroup);
//		if(pFindGraph)
//			pFindGraph->GetStringGraphData( p_slVertScaleGroupData);
		
		p_slVertScaleGroupData.AddTail( "VertScaleGroupEnd;\r\n");
		
		CString strVertScaleGroupData;
		POSITION pos = p_slVertScaleGroupData.GetHeadPosition();
		while(pos)
			strVertScaleGroupData += p_slVertScaleGroupData.GetNext(pos);
		
		// ���ο� �׷����� ������
		CVertScaleGroupList* pGroupList = pGroup->GetVertScaleGroupList();
		pGroupList->BuildVertScaleGroups((LPSTR)(LPCTSTR)strVertScaleGroupData, false);
		
		CVertScaleGroup* pNewGroup = pGroupList->GetVertScaleGroup(pGroupList->GetCount() - 1);
		
		bool bInvert = !pGroup->GetVertScaleInvertType();

		int nGraphIndex = 0;
		pGroup->GetIndexOfGraph(pGraph->GetGraphData().GetGraphName(), nGraphIndex);

		if(nGraphIndex == 0)
		{
			pGroup->RemoveAt(pGraph, true);
			pNewGroup->AddGraph(pGraph);
			if(pFindGraph)
			{
				pGroup->RemoveAt(pFindGraph, true);
				pNewGroup->AddGraph(pFindGraph);
			}
		}
		else
		{
			if(pFindGraph)
			{
				pGroup->RemoveAt(pFindGraph, true);
				pNewGroup->AddGraph(pFindGraph);
			}
			pGroup->RemoveAt(pGraph, true);
			pNewGroup->AddGraph(pGraph);
		}

		pNewGroup->SetVertScaleInvertType(bInvert);
		
		return true;
	}
	
	bool bInvert = !pGroup->GetVertScaleInvertType();
	pGroup->SetVertScaleInvertType(bInvert);
	
	return true;
}

// ��ǥ ������ - ojtaso (20071023)
CGraphImp* CGraphTool::IsGeneralGraph(CGraphImp* pGraph, CVertScaleGroup* pGroup)
{
	CString strName = pGraph->GetIndicatorInfo()->GetGraphName();
	CGraphImp* pFindGraph = NULL;

	if(strName == _MTEXT( C2_PRICE_CHART))
		pFindGraph = pGroup->FindGraph( _MTEXT( C2_PRICE_MA));
	else if(strName == _MTEXT( C2_PRICE_MA))
		pFindGraph = pGroup->FindGraph( _MTEXT( C2_PRICE_CHART));
	else if(strName == _MTEXT( C2_VOLUME_CHART))
		pFindGraph = pGroup->FindGraph( _MTEXT( C2_VOLUME_MA));
	else if(strName == _MTEXT( C2_VOLUME_MA))
		pFindGraph = pGroup->FindGraph( _MTEXT( C2_VOLUME_CHART));

	return pFindGraph;
}

// ��ǥ ������ - ojtaso (20071023)
bool CGraphTool::GetMenuCheck_SubGraphInvert(CMainBlock* pMainBlock, CSubGraph* pSubGraph)
{
	CGraphImp* pGraph = pSubGraph->GetGraph();
	CVertScaleGroup* pGroup = pGraph->GetVertScaleGroup();

	return pGroup->GetVertScaleInvertType();
}

// 20081007 JS.Kim	����ڽð��� ����
bool CGraphTool::OnMenu_SubGraphSelectTimeDiff(CMainBlock* pMainBlock, CSubGraph* pSubGraph, CTimeDiffInfo* pTimeInfo)
{
	CGraphImp* pGraph = pSubGraph->GetGraph();

	if( pTimeInfo == NULL )	// �����ð���� return
		pGraph->SetPacketTimeDiff("", 0);
	else
		pGraph->SetPacketTimeDiff(pTimeInfo->m_strName, pTimeInfo->m_nDiffTime);

	return true;
}

// 20081007 JS.Kim	����ڽð��� ����
bool CGraphTool::GetMenuCheck_SubGraphSelectTimeDiff(CMainBlock* pMainBlock, CSubGraph* pSubGraph, CTimeDiffInfo* pTimeInfo)
{
	if( pTimeInfo == NULL )		return false;

	CGraphImp* pGraph = pSubGraph->GetGraph();
	CString strTimeName = pGraph->GetPacketTimeDiffName();
	if( !strTimeName.IsEmpty() && strTimeName == pTimeInfo->m_strName )
		return true;
	if( pTimeInfo->m_strName.IsEmpty() )	// �����ð����� �����ð��� ���Ѵ�
	{
		pTimeInfo->m_nDiffTime = pGraph->GetPacketOrgBaseTimeDiff();
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
// class CSubGraphTitleTool

CSubGraphTitleTool::CSubGraphTitleTool():
	CGraphTool(),
	m_selectedSubGraphData(NULL, -1),
	m_selectedSubGraphTitleRegion(0, 0, 0, 0),
	m_oldSelectedSubGraphTitleRegion(0, 0, 0, 0)
{
}

// public =====================================================================
// ��� data�� �ʱ�ȭ �Ѵ�.
void CSubGraphTitleTool::InitializeAllData()
{
	CGraphTool::InitializeAllData();
	m_selectedSubGraphData = CSelectedSubGraphData(NULL, -1);
	InitializeTitleRegion();
}

CBlockBaseData::TOOLTYPE CSubGraphTitleTool::GetToolType() const
{
	return CBlockBaseData::TOOL_GRAPHTITLE;
}

// ----------------------------------------------------------------------------
bool CSubGraphTitleTool::OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point)
{
	if(!FindSubGraphTitle(pDC, pMainBlock, point, m_selectedSubGraphData.GetSubGraph()) || !IsLoadIndicatorConfiguration(point))
		return false;

	return LoadIndicatorConfiguration(pMainBlock);
}

CMenuType::MENUTYPE CSubGraphTitleTool::OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point)
{
	// 0. Get Current and New Graph
	CSubGraph *pCurrentGraph = m_selectedSubGraphData.GetSubGraph();
	CSubGraph *pNewGraph = NULL;
	FindSubGraphTitle( pDC, pMainBlock, point, pNewGraph);

	// 1. Check New Graph
	// (2006/11/1 - Seung-Won, Bae) Auto Release for Design Mode.
	if( pNewGraph || pMainBlock->GetDesignMode())
	{
		// (2006/11/1 - Seung-Won, Bae) Auto Select Grpah Title for RButton Menu.
		OnMouseMove( pDC, pMainBlock);

		OnLButtonDown( pDC, pMainBlock, 0, point);
		// 2008.10.28  JS.Kim   CGraphTool::LButtonDown �� �ҷ��� ��ư�� Ŭ���� �������� �˰� ������ �Ͽ� CGraphTool::OnLButtonUp�� Call ��
		CGraphTool::OnLButtonUp(pDC, pMainBlock, point);
		OnLButtonUp();

		// 20080925 JS.Kim	������ ��ư Ŭ���Ͽ� Ÿ��Ʋ���� ������ ���� ���ʹ�ư�� �����ϰ� ó�� <<
		if( pNewGraph)
		{
			CGraphImp* pGraph = pNewGraph->GetGraph();
			if( pGraph)
			{
				LPCTSTR lpszRQ = pGraph->GetGraphRQ();
				m_pIChartCtrl->SelectedGraph(lpszRQ);
			}
		}
		//>>
		
		Invalidate( false);

		// (2006/11/1 - Seung-Won, Bae) Auto Release for Design Mode.
		if( pMainBlock->GetDesignMode())
		{
			pCurrentGraph = m_selectedSubGraphData.GetSubGraph();
			FindSubGraphTitle( pDC, pMainBlock, point, pNewGraph);
		}
	}

	// 2. Check No Graph
	if( !pCurrentGraph && !pNewGraph) return CMenuType::MENU_NONE;

	// 3. Show Graph Menu
	CBaseTool::SetProcessEnded(false);
	return m_selectedSubGraphData.GetMenuType();
}

// ----------------------------------------------------------------------------
// �ش� subgraph�� ã�´�.
bool CSubGraphTitleTool::Find(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point)
{
	if(GetSubGraph() != NULL || m_selectedSubGraphTitleRegion.PtInRect(point))
		return true;

	CSubGraph* pSubGraph = NULL;
	return FindSubGraphTitle(pDC, pMainBlock, point, pSubGraph);
}

// ----------------------------------------------------------------------------
// <��ǥ����> �� �°� ���� �����Ų��. - graph ���� �� ����.
bool CSubGraphTitleTool::LoadIndicatorConfiguration(CMainBlock* pMainBlock)
{
	if(!CGraphTool::LoadIndicatorConfiguration(pMainBlock, GetSubGraph()))
		return false;

	InitializeAllData();
	return true;
}


// protected ==================================================================
void CSubGraphTitleTool::OnLButtonDown_Real(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	if(!FindSubGraphTitle(pDC, pMainBlock, point, m_selectedSubGraphData.GetSubGraph()))
		return;

	DrawSelectedSubGraphGrip(pDC);
}

bool CSubGraphTitleTool::OnLButtonUp()
{
	CGraphTool::SendMessage_GraphType(GetSubGraph());
	if(GetSubGraph() != NULL)
		return true;

	CBaseTool::SetProcessEnded(true);
	return false;
}

void CSubGraphTitleTool::OnMouseMove(CDC* pDC, CMainBlock* /*pMainBlock*/)
{
	if(m_oldSelectedSubGraphTitleRegion == m_selectedSubGraphTitleRegion)
		return;

	CBaseTool::InvalidateRect(m_oldSelectedSubGraphTitleRegion);
	DrawSelectedSubGraphTitle(pDC);

	m_oldSelectedSubGraphTitleRegion = m_selectedSubGraphTitleRegion;
}

bool CSubGraphTitleTool::OnMenu_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	switch(menuType){
	case CMenuType::MENU_DELETE:	// subGraph ����
		return OnMenu_SubGraphHide( wParam ? true : false);
	case CMenuType::MENU_SET:		// ��ǥ���� Dlg
		return OnMenu_SubGraphSet(pMainBlock);
	case CMenuType::MENU_INVERTSCALE:	// ��ǥ ������ - ojtaso (20071023)
		return OnMenu_SubGraphInvert(pMainBlock, GetSubGraph());
	case CMenuType::MENU_DATAINDEX: // data index�� ���ϱ�
		return CGraphTool::OnMenu_DataIndex(m_selectedSubGraphData);
	case CMenuType::MENU_TIME_DIFF:		// 20081007 JS.Kim	����ڽð��� ����
		return OnMenu_SubGraphSelectTimeDiff(pMainBlock, GetSubGraph(), (CTimeDiffInfo *)wParam);
	default:
		return m_selectedSubGraphData.OnMenu(menuType, wParam);
	}

	return false;
}

// ��ǥ ������ - ojtaso (20071023)
bool CSubGraphTitleTool::GetMenuCheck_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	switch(menuType)
	{
	case CMenuType::MENU_INVERTSCALE:	// ��ǥ ������ - ojtaso (20071023)
		return GetMenuCheck_SubGraphInvert(pMainBlock, GetSubGraph());

	case CMenuType::MENU_TIME_DIFF:		// 20081007 JS.Kim	����ڽð��� ����
		return GetMenuCheck_SubGraphSelectTimeDiff(pMainBlock, GetSubGraph(), (CTimeDiffInfo *)wParam);

	default:
		return false;
	}

	return false;
}

void CSubGraphTitleTool::OnDraw(CDC* pDC, CMainBlock* /*pMainBlock*/)
{
	if(DrawSelectedSubGraphGrip(pDC))
		return;

	DrawSelectedSubGraphTitle(pDC);
}

// ----------------------------------------------------------------------------
bool CSubGraphTitleTool::IsSelectedSubGraph() const
{
	return (GetSubGraph() != NULL);
}

// <���ེ��>�� ���� ��츸 ocx�� �޽��� ������.
bool CSubGraphTitleTool::IsSendMessage_SubGraph() const
{
	return m_selectedSubGraphData.AreExistenceSubGraph( _MTEXT( C3_PRESPAN));
}

CGraphImp* CSubGraphTitleTool::GetGraphMoving()
{
	return CGraphTool::GetGraph(GetSubGraph());
}

void CSubGraphTitleTool::GraphMoving_Ended(const bool bGraphMovingResult)
{
	CBaseTool::SetProcessEnded(bGraphMovingResult);
	if(bGraphMovingResult)
		return;

	CBaseTool::InvalidateRect(m_oldSelectedSubGraphTitleRegion);
	ChangeAllVertScaleGridType();
}


// private ====================================================================
// �ش� subgraph�� ã�´�.
bool CSubGraphTitleTool::FindSubGraphTitle(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, CSubGraph*& pSubGraph)
{
	pSubGraph = NULL;
	if(pMainBlock == NULL)
		return false;

	CRect subGraphtitleRegion(0, 0, 0, 0);
	pSubGraph = pMainBlock->FindSubGraphTitle(pDC, point, subGraphtitleRegion);
	if(pSubGraph == NULL){
		InitializeTitleRegion();
		return false;
	}

	m_selectedSubGraphTitleRegion = GetSelectedSubGraphTitleRegion(subGraphtitleRegion);
	return true;
}

// ���� ���õ� subGraph�� vert scale�� ���̰� �Ѵ�.
void CSubGraphTitleTool::ChangeAllVertScaleGridType()
{
	CSubGraph* pSubGraph = GetSubGraph();
	if(pSubGraph == NULL)
		return;

	CGraphTool::ChangeAllVertScaleGridType(pSubGraph->GetGraph()->GetVertScaleGroup());
}

// ----------------------------------------------------------------------------
// subgraph title�� ���õ� ���
void CSubGraphTitleTool::DrawSelectedSubGraphTitle(CDC* pDC)
{
	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
	int oldDrawMode = pDC->GetROP2();
	pDC->SetROP2(R2_NOT);

	pDC->Rectangle(m_selectedSubGraphTitleRegion);

	pDC->SetROP2(oldDrawMode);
	pDC->SelectObject(pOldBrush);
}

bool CSubGraphTitleTool::DrawSelectedSubGraphGrip(CDC* pDC)
{
	if(GetSubGraph() == NULL)
		return false;

	return GetSubGraph()->DrawGrip(pDC, -1);
}

// ----------------------------------------------------------------------------
bool CSubGraphTitleTool::IsLoadIndicatorConfiguration(const CPoint& point) const
{
	if(GetSubGraph() == NULL || !m_selectedSubGraphTitleRegion.PtInRect(point))
		return false;

	return (GetSubGraph()->GetSubGraphData().GetType() != CGraphBaseData::DataView_Type);
}

CSubGraph* CSubGraphTitleTool::GetSubGraph() const
{
	return m_selectedSubGraphData.GetSubGraph();
}

// subgraph title�� ���� ����
CRect CSubGraphTitleTool::GetSelectedSubGraphTitleRegion(const CRect& subGraphTitleRegion) const
{
	return CRect(subGraphTitleRegion.left -2, subGraphTitleRegion.top -2, 
		subGraphTitleRegion.right +2, subGraphTitleRegion.bottom +1);
}

void CSubGraphTitleTool::InitializeTitleRegion()
{
	m_selectedSubGraphTitleRegion.SetRectEmpty();
	m_oldSelectedSubGraphTitleRegion.SetRectEmpty();
}

// ----------------------------------------------------------------------------
// subGraph ����
bool CSubGraphTitleTool::OnMenu_SubGraphHide(const bool bIsAction)
{
	if(!bIsAction)
		return false;

	if(!m_selectedSubGraphData.DeleteOrHideSubGraph())
		return false;

	CBaseTool::SetProcessEnded(true);
	return true;
}

// ��ǥ����
bool CSubGraphTitleTool::OnMenu_SubGraphSet(CMainBlock* pMainBlock) 
{
	CBaseTool::SetProcessEnded(true);

	return CGraphTool::LoadIndicatorConfiguration(pMainBlock, m_selectedSubGraphData.GetSubGraph());
}


///////////////////////////////////////////////////////////////////////////////
// class CSubGraphTool


// public =====================================================================
// ��� data�� �ʱ�ȭ �Ѵ�.
void CSubGraphTool::InitializeAllData()
{
	CGraphTool::InitializeAllData();
	m_selectedSubGraphDeque.RemoveAll();
}

CBlockBaseData::TOOLTYPE CSubGraphTool::GetToolType() const
{
	return CBlockBaseData::TOOL_SUBGRAPH;
}

// ----------------------------------------------------------------------------
bool CSubGraphTool::OnLButtonDblClk(CDC* /*pDC*/, CMainBlock* pMainBlock, const CPoint& point)
{
	FindSubGraph(pMainBlock, point, m_selectedSubGraphDeque);

	bool bResult = SelectedGraphDataIndex();
	if(!bResult)
		bResult = ReversedSubGraphHighlight(pMainBlock, point);

	InitializeAllData();
	return bResult;
}

CMenuType::MENUTYPE CSubGraphTool::OnRButtonUp(CDC* /*pDC*/, CMainBlock* /*pMainBlock*/, const CPoint& /*point*/)
{
	if(!IsSelectedSubGraph())
		return CMenuType::MENU_NONE;

	CBaseTool::SetProcessEnded(false);
	return m_selectedSubGraphDeque.GetMenuType();
}

void CSubGraphTool::OnDraw(CDC* pDC, CMainBlock* pMainBlock)
{
	m_selectedSubGraphDeque.DrawGrip(pDC);
}

// ----------------------------------------------------------------------------
// �ش� graph�� ã�´�.
bool CSubGraphTool::Find(CDC* /*pDC*/, CMainBlock* pMainBlock, const CPoint& point)
{
	CSelectedSubGraphDeque selectedSubGraphDeque;
	bool result = FindSubGraph(pMainBlock, point, selectedSubGraphDeque);
	//SetSelectedCursor(result);
	return result;
}


// protected ==================================================================
void CSubGraphTool::OnLButtonDown_Real(CDC* /*pDC*/, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	FindSubGraph(pMainBlock, nFlags, point);
}

bool CSubGraphTool::OnLButtonUp()
{
	CGraphTool::SendMessage_GraphType(m_selectedSubGraphDeque.GetSubGraphTail());
	if(IsSelectedSubGraph())
		return true;

	CBaseTool::SetProcessEnded(true);
	return false;
}

void CSubGraphTool::OnMouseMove(CDC* pDC, CMainBlock* pMainBlock)
{
}

bool CSubGraphTool::OnMenu_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	if(m_selectedSubGraphDeque.GetCount() <= 0)
		return false;

	switch(menuType){
	case CMenuType::MENU_SET:		// ��ǥ���� Dlg
		return OnMenu_SubGraphSet(pMainBlock);
	case CMenuType::MENU_INVERTSCALE:	// ��ǥ ������ - ojtaso (20071023)
		return OnMenu_SubGraphInvert(pMainBlock, m_selectedSubGraphDeque.GetSubGraphTail());
	case CMenuType::MENU_DATAINDEX: // data index�� ���ϱ�
		return CGraphTool::OnMenu_DataIndex(m_selectedSubGraphDeque.GetTail());
	case CMenuType::MENU_TIME_DIFF:		// 20081007 JS.Kim	����ڽð��� ����
		return OnMenu_SubGraphSelectTimeDiff(pMainBlock, m_selectedSubGraphDeque.GetSubGraphTail(), (CTimeDiffInfo *)wParam);
	default:
		return OnMenu_Etc(menuType, wParam);
	}

	return false;
}

// ��ǥ ������ - ojtaso (20071023)
bool CSubGraphTool::GetMenuCheck_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	switch(menuType){
	case CMenuType::MENU_INVERTSCALE:	// ��ǥ ������ - ojtaso (20071023)
		return GetMenuCheck_SubGraphInvert(pMainBlock, m_selectedSubGraphDeque.GetSubGraphTail());
	
	case CMenuType::MENU_TIME_DIFF:		// 20081007 JS.Kim	����ڽð��� ����
		return GetMenuCheck_SubGraphSelectTimeDiff(pMainBlock, m_selectedSubGraphDeque.GetSubGraphTail(), (CTimeDiffInfo *)wParam);

	default:
		return false;
	}

	return false;
}

// ----------------------------------------------------------------------------
bool CSubGraphTool::IsSelectedSubGraph() const
{
	return (m_selectedSubGraphDeque.GetCount() > 0);
}

// <���ེ��>�� ���� ��츸 ocx�� �޽��� ������.
bool CSubGraphTool::IsSendMessage_SubGraph() const
{
	return m_selectedSubGraphDeque.AreExistenceSubGraph( _MTEXT( C3_PRESPAN));
}

CGraphImp* CSubGraphTool::GetGraphMoving()
{
	if(m_selectedSubGraphDeque.GetCount() <= 0)
		return NULL;

	return m_selectedSubGraphDeque.GetTail().GetGraph();
}

void CSubGraphTool::GraphMoving_Ended(const bool bGraphMovingResult)
{
	CBaseTool::SetProcessEnded(bGraphMovingResult);
	if(bGraphMovingResult)
		m_selectedSubGraphDeque.RemoveAll();
	else
		ChangeAllVertScaleGridType();
}


// private ====================================================================
// �ش� graph�� ã�´�.
void CSubGraphTool::FindSubGraph(CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	// no shift key
	if((nFlags & MK_SHIFT) == 0)
		m_selectedSubGraphDeque.RemoveAll();

	FindSubGraph(pMainBlock, point, m_selectedSubGraphDeque);
}

bool CSubGraphTool::FindSubGraph(CMainBlock* pMainBlock, const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque)
{
	InitializeAllData();
	if(pMainBlock == NULL)
		return false;

	return pMainBlock->FindSubGraph(point, selectedSubGraphDeque);
}

// ----------------------------------------------------------------------------
/*void CSubGraphTool::SetSelectedCursor(const bool bIsSelectedCursor)
{
	if(bIsSelectedCursor)
		CBaseTool::ChangeCursor(CBaseToolEnumData::SELECT);
	else
		CBaseTool::ChangeCursor(CBaseToolEnumData::CTNONE);
}*/

// ���� ���õ� subGraph�� vert scale�� ���̰� �Ѵ�.
void CSubGraphTool::ChangeAllVertScaleGridType()
{
	if(!IsSelectedSubGraph())
		return;

	CGraphTool::ChangeAllVertScaleGridType(m_selectedSubGraphDeque.GetVertScaleGroupTail());
}

// ----------------------------------------------------------------------------
// subGraph ����
bool CSubGraphTool::ReversedSubGraphHighlight(CMainBlock* pMainBlock, const CPoint& point)
{
	if(pMainBlock == NULL)
		return false;

	CSubGraph* pSubGraph = m_selectedSubGraphDeque.GetSubGraphTail();
	if(pSubGraph == NULL)
		return false;

	// (2008/10/7 - Seung-Won, Bae) Show config for double click.
	//	pSubGraph->GetSubGraphData().GetPenThickness().ReversedHightlight();
	//	return true;
	return CGraphTool::LoadIndicatorConfiguration( pMainBlock, pSubGraph);
}

// data index�� ocx�� �ֱ�
bool CSubGraphTool::SelectedGraphDataIndex()
{
	if(m_selectedSubGraphDeque.GetCount() != 1)
		return false;

	CSubGraph* pSubGraph = m_selectedSubGraphDeque.GetSubGraphTail();
	if(pSubGraph == NULL || pSubGraph->GetSubGraphData().GetType() != CGraphBaseData::DataView_Type)
		return false;

	return CGraphTool::OnMenu_DataIndex(m_selectedSubGraphDeque.GetTail());
}

// ----------------------------------------------------------------------------
// ��ǥ����
bool CSubGraphTool::OnMenu_SubGraphSet(CMainBlock* pMainBlock) 
{
	CBaseTool::SetProcessEnded(true);

	bool bResult = CGraphTool::LoadIndicatorConfiguration(pMainBlock, m_selectedSubGraphDeque.GetSubGraphTail());
	m_selectedSubGraphDeque.RemoveAll();
	return bResult;
}

// ��Ÿ menu
bool CSubGraphTool::OnMenu_Etc(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	return m_selectedSubGraphDeque.OnMenu(menuType, wParam);
}