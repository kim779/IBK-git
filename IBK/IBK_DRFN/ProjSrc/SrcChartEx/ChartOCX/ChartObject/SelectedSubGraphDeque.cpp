// SelectedSubGraphDeque.cpp: implementation of the CSelectedSubGraphDeque class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectedSubGraphDeque.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "GraphImp.h"
#include "SubGraph.h"
#include "resource.h"											// for IDS_CANNOT_DELETE_PRICE_CHART
#include "MainBlockImp.h"										// for CMainBlockImp

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
//class CSelectedSubGraphData

CSelectedSubGraphData& CSelectedSubGraphData::operator=(const CSelectedSubGraphData& selectedSubGraphData)
{
	if(this == &selectedSubGraphData)
		return *this;

	m_pSubGraph = selectedSubGraphData.GetSubGraph();
	m_nDataIndex = selectedSubGraphData.GetDataIndex();
	return *this;
}

CSelectedSubGraphData::CSelectedSubGraphData(CSubGraph* pSubGraph, const int nDataIndex):
	m_pSubGraph(pSubGraph),
	m_nDataIndex(nDataIndex)
{
}

CSelectedSubGraphData::CSelectedSubGraphData(const CSelectedSubGraphData& selectedSubGraphData):
	m_pSubGraph(selectedSubGraphData.GetSubGraph()),
	m_nDataIndex(selectedSubGraphData.GetDataIndex())
{
}


// public =====================================================================
// list�� data ��������
CSubGraph*& CSelectedSubGraphData::GetSubGraph()
{
	return m_pSubGraph;
}

CSubGraph* CSelectedSubGraphData::GetSubGraph() const
{
	return m_pSubGraph;
}

CGraphImp* CSelectedSubGraphData::GetGraph() const
{
	if(m_pSubGraph == NULL)
		return NULL;

	return m_pSubGraph->GetGraph();
}

CVertScaleGroup* CSelectedSubGraphData::GetVertScaleGroup() const
{
	CGraphImp* pGraph = GetGraph();
	if(pGraph == NULL)
		return NULL;

	return pGraph->GetVertScaleGroup();
}

int CSelectedSubGraphData::GetDataIndex() const
{
	return m_nDataIndex;
}
// ----------------------------------------------------------------------------
CMenuType::MENUTYPE CSelectedSubGraphData::GetMenuType() const
{
	if(m_pSubGraph == NULL)
		return CMenuType::MENU_NONE;

	// <DataView_Type>�� graph�� �ִ� ���
	if(m_pSubGraph->GetSubGraphData().GetType() == CGraphBaseData::DataView_Type)
		return CMenuType::MENU_DATAVIEWTYPE_GRAPH;

	// ���õ� ��� subGraph �� <����>�� ��쿡�� <��������> �� �Ѵ�!!
	if(AreAllSubGraphHighlighted())
		return CMenuType::MENU_GRAPH_NO_HIGHLIGHT;

	return CMenuType::MENU_GRAPH_HIGHLIGHT;
}

// ----------------------------------------------------------------------------
bool CSelectedSubGraphData::OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	switch(menuType){
	case CMenuType::MENU_COLOR:		// graph color
		return SetSubGraphColor((COLORREF) wParam);
	case CMenuType::MENU_THICKNESS:	// graph thickness
		return SetSubGraphThickness((int) wParam);
	case CMenuType::MENU_HIGHLIGHT:	// graph ����
		//return SetSubGraphHighlight((bool) wParam);
		return SetSubGraphHighlight(); // ��������� �ݴ�
	case CMenuType::MENU_DRAWSTYLE:	// graph draw style
		return SetSubGraphDrawStyle((CGraphDrawStyle) wParam);
	}

	return false;
}

// ----------------------------------------------------------------------------
// graph color -> ù��°�� �ٲ۴�.
bool CSelectedSubGraphData::SetSubGraphColor(const COLORREF& color)
{
	if(m_pSubGraph == NULL)
		return false;

	m_pSubGraph->GetSubGraphData().GetColor().SetColor1(color);
	return true;
}

// graph thickness -> ��м��� ���⸦ �ٲ۴�.
bool CSelectedSubGraphData::SetSubGraphThickness(const int nThickness)
{
	if(m_pSubGraph == NULL)
		return false;

	m_pSubGraph->GetSubGraphData().GetPenThickness().SetThickness1(nThickness);
	m_pSubGraph->GetSubGraphData().GetPenThickness().SetThickness2(nThickness);
	m_pSubGraph->GetSubGraphData().GetPenThickness().SetThickness3(nThickness);
	return true;
}

// ����
bool CSelectedSubGraphData::SetSubGraphHighlight() 
{
	if(m_pSubGraph == NULL)
		return false;

	bool bIsHightlight = m_pSubGraph->GetSubGraphData().GetPenThickness().IsHightlight();
	m_pSubGraph->GetSubGraphData().GetPenThickness().SetHightlight(!bIsHightlight);
	return true;
}

bool CSelectedSubGraphData::SetSubGraphHighlight(const bool bIsHightlight) 
{
	if(m_pSubGraph == NULL)
		return false;

	m_pSubGraph->GetSubGraphData().GetPenThickness().SetHightlight(bIsHightlight);
	return true;
}

// subGraph draw style	
bool CSelectedSubGraphData::SetSubGraphDrawStyle(const CGraphDrawStyle& drawStyle)
{
	if(m_pSubGraph == NULL)
		return false;

	// "������Ʈ" �� ��쿡�� style == drawStyle.
	CGraphImp* pGraph = GetGraph();
	if( pGraph != NULL)
		if( pGraph->GetGraphData().GetIndicatorName() == _MTEXT( C2_PRICE_CHART)
			// (2009/6/5 - Seung-Won, Bae) for Only Candle Type.
			&& m_pSubGraph->GetSubGraphData().GetType() == CGraphBaseData::Bong_Type)
			m_pSubGraph->GetSubGraphData().SetStyle( CGraphStyle(drawStyle.GetDrawStyle()));

	m_pSubGraph->GetSubGraphData().SetDrawStyle(drawStyle);
	return true;
}

// ----------------------------------------------------------------------------
bool CSelectedSubGraphData::AreAllSubGraphHighlighted() const
{
	if(m_pSubGraph == NULL)
		return false;

	return (m_pSubGraph->GetSubGraphData().GetPenThickness().IsHightlight());
}

bool CSelectedSubGraphData::AreExistenceSubGraph(const CString& strSubGraphName) const
{
	if(m_pSubGraph == NULL || strSubGraphName.IsEmpty())
		return false;

	return (m_pSubGraph->GetSubGraphData().GetName() == strSubGraphName);
}

bool CSelectedSubGraphData::DeleteOrHideSubGraph()
{
	if(!IsDeleteOrHideSubGraph())
		return false;

	CSubGraphList* pSubGraphList = m_pSubGraph->GetSubGraphList();
	assert(pSubGraphList != NULL);

	// ��� subGraph �� "����"�� ��� graph�� �����Ѵ�.
	if(IsDeleteGraph())
		DeleteGraph();
	else
		m_pSubGraph->SetHiding(true);

	m_pSubGraph = NULL;
	return true;
}

bool CSelectedSubGraphData::DrawGrip(CDC* pDC)
{
	if(m_pSubGraph == NULL)
		return false;

	return m_pSubGraph->DrawGrip(pDC, m_nDataIndex);
}


// private ====================================================================
// "������Ʈ" �� "graph type = DataView_Type" && �� ��쿡�� ���� ���� �ʴ´�.!!
// <Ư����Ʈ> �� �������� �ʴ´�.
// ��, "System Signal" �� Ÿ�Կ� ������� ���� �����ϴ�.
bool CSelectedSubGraphData::IsDeleteOrHideSubGraph() const
{
	CGraphImp* pGraph = GetGraph();
	if(pGraph == NULL)
		return false;

	if(pGraph->GetGraphData().GetGraphName() == "System Signal")
		return true;

	//if(pGraph->GetSubGraphList().IsViewableGraphType(CGraphBaseData::DataView_Type))
	//	return false;

	_MHWND( pGraph->GetMainBlock()->GetParent_Handle());
	ML_SET_LANGUAGE_RES();
	CString strMsg;

	CString strGraphName = pGraph->GetGraphData().GetGraphName();
	CString strIndicatorName = pGraph->GetGraphData().GetIndicatorName();
	// ������Ʈ ���� : �������� - ojtso (20070522)
	if( strGraphName == _MTEXT( C2_PRICE_CHART) && !::lstrcmp(pGraph->GetGraphRQ(), _T("DEFAULT")))
	{
		strMsg.LoadString( IDS_CANNOT_DELETE_PRICE_CHART);
		AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
		return false;
	}
	//Ư����Ʈ�϶� ���ҽ��� ����� �������� ���ϴ� ��쿡 #���� ���鼭 �����Ǵ� ��� ����.
	//{{ JS.Kim_20100902 Solomon Pair �� ��� ��ǥ�߰��� 2���� �߰��Ǹ鼭 GraphName�� #���� ���۵Ǵµ�, �� ��ǥ�� ���� �ȵǴ� ���� ����
	//else if( g_iMetaTable.IsSpecialGroupGraph(strGraphName) || strGraphName.GetAt(0)=='#')
	//{
	//	strMsg.LoadString( IDS_CANNOT_DELETE_SPECIAL_CHART);
	//	AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
	//	return false;
	//}
	else if( g_iMetaTable.IsSpecialGroupGraph(strGraphName) )
	{
		strMsg.LoadString( IDS_CANNOT_DELETE_SPECIAL_CHART);
		AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
		return false;
	}
	else if( strGraphName.GetAt(0)=='#' )
	{
		if( pGraph->GetMainBlock()->GetMainBlockBaseData()->GetMainAttributes().GetChartMode() == CChartInfo::FX )
		{
			CString strRQ = pGraph->GetGraphRQ();
			if( strIndicatorName == _MTEXT( C2_PRICE_CHART) && ( strRQ == "DEFAULT" || strRQ.Right( 5) == "_PAIR" ) )
			{
				strMsg.LoadString( IDS_CANNOT_DELETE_PRICE_CHART);
				AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
				return false;
			}
			return true;
		}
		//>>[A00000422] ������ �����ǵ���. ������Ʈ�϶��� �޽��� ����.
		else 
		{
			if( strIndicatorName == _MTEXT( C2_PRICE_MA) )
				return true;
		}

// vntsorl_20110608:[A00000423] �����߰��� �׷����� RMenu������ �ȵǴ� ���� ����
// 		if( strIndicatorName == _MTEXT( C2_PRICE_CHART) )
// 			strMsg.LoadString( IDS_CANNOT_DELETE_PRICE_CHART);
// 		else
// 			strMsg.LoadString( IDS_CANNOT_DELETE_SPECIAL_CHART);
// 		//<<[A00000422]
// 
// 		AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
// 		return false;

// vntsorl_20110608:[A00000423] �����߰��� �׷����� RMenu������ �ȵǴ� ���� ����
		if( g_iMetaTable.IsSpecialGroupGraph(strIndicatorName))
		{
			strMsg.LoadString( IDS_CANNOT_DELETE_SPECIAL_CHART);
			AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
			return false;
		}
	}
	//}}
	return true;
}

// ��� "����"�� ��� m_pSubGraph = "����" ���� �ϰ� Graph�� �����Ѵ�.
bool CSelectedSubGraphData::IsDeleteGraph() const
{
	if(m_pSubGraph == NULL)
		return false;

	CSubGraphList* pSubGraphList = m_pSubGraph->GetSubGraphList();
	if(pSubGraphList == NULL)
		return false;

	m_pSubGraph->GetSubGraphData().SetHiding(true);
	bool bResult = pSubGraphList->AreHidingAllSubGraphs();
	m_pSubGraph->GetSubGraphData().SetHiding(false);
	return bResult;
}

// ----------------------------------------------------------------------------
void CSelectedSubGraphData::DeleteGraph()
{
	if(m_pSubGraph == NULL)
		return;

	CGraphImp* pGraph = m_pSubGraph->GetGraph();
	if(pGraph == NULL)
		return;

	pGraph->DeleteGraph();
}


///////////////////////////////////////////////////////////////////////////////
//class CSelectedSubGraphDeque

CSelectedSubGraphDeque& CSelectedSubGraphDeque::operator=(const CSelectedSubGraphDeque& selectedSubGraphDeque)
{
	if(this == &selectedSubGraphDeque)
		return *this;

	for(int nIndex = 0; nIndex < selectedSubGraphDeque.GetCount(); nIndex++){
		m_selectedSubGraphDeque.AddTail(selectedSubGraphDeque.GetAt(nIndex));
	}

	return *this;
}

// public =====================================================================
// list�� data �߰�
void CSelectedSubGraphDeque::AddTail(const CSelectedSubGraphData& selectedSubGraphData)
{
	if(selectedSubGraphData.GetSubGraph() == NULL)
		return;

	m_selectedSubGraphDeque.AddTail(selectedSubGraphData);
}

void CSelectedSubGraphDeque::RemoveAll()
{
	m_selectedSubGraphDeque.RemoveAll();
}

// ----------------------------------------------------------------------------
// list�� �ִ� data ����
int CSelectedSubGraphDeque::GetCount() const
{
	return m_selectedSubGraphDeque.GetCount();
}

// list�� data ��������
CSelectedSubGraphData CSelectedSubGraphDeque::GetAt(const int nIndex) const
{
	if(nIndex < 0 || nIndex >= GetCount())
		return CSelectedSubGraphData(NULL, -1);

	return m_selectedSubGraphDeque.GetAt(nIndex);
}

CSelectedSubGraphData CSelectedSubGraphDeque::GetTail() const
{
	if(GetCount() <= 0)
		return CSelectedSubGraphData(NULL, -1);
	return m_selectedSubGraphDeque.GetTail();
}

CSubGraph* CSelectedSubGraphDeque::GetSubGraphTail() const
{
	if(GetCount() <= 0)
		return NULL;
	return m_selectedSubGraphDeque.GetTail().GetSubGraph();
}

CVertScaleGroup* CSelectedSubGraphDeque::GetVertScaleGroupTail() const
{
	if(GetCount() <= 0)
		return NULL;
	return m_selectedSubGraphDeque.GetTail().GetVertScaleGroup();
}

// ----------------------------------------------------------------------------
CMenuType::MENUTYPE CSelectedSubGraphDeque::GetMenuType() const
{
	// <DataView_Type>�� graph�� �ִ� ���
	if(AreSubGraphType(CGraphBaseData::DataView_Type))
		return CMenuType::MENU_DATAVIEWTYPE_GRAPH;

	// ���õ� ��� subGraph �� <����>�� ��쿡�� <��������> �� �Ѵ�!!
	if(AreAllSubGraphHighlighted())
		return CMenuType::MENU_GRAPH_NO_HIGHLIGHT;

	return CMenuType::MENU_GRAPH_HIGHLIGHT;
}

// ----------------------------------------------------------------------------
bool CSelectedSubGraphDeque::OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	switch(menuType){
	case CMenuType::MENU_DELETE:	// subGraph ����
		return OnMenu_SubGraphHide( wParam ? true : false);
	case CMenuType::MENU_COLOR:		// graph color
		return OnMenu_SubGraphColor((COLORREF) wParam);
	case CMenuType::MENU_THICKNESS:	// graph thickness
		return OnMenu_SubGraphThickness((int) wParam);
	case CMenuType::MENU_HIGHLIGHT:	// graph ����
		return OnMenu_SubGraphHighlight( wParam ? true : false);
	case CMenuType::MENU_DRAWSTYLE:	// graph draw style
		return OnMenu_SubGraphDrawStyle((CGraphDrawStyle) wParam);
	}

	return false;
}

// ----------------------------------------------------------------------------
// ��� subGraph �� ������ ��� = true
bool CSelectedSubGraphDeque::AreAllSubGraphHighlighted() const
{
	if(GetCount() <= 0)
		return false;

	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		CSelectedSubGraphData data = m_selectedSubGraphDeque.GetAt(nIndex);
		assert(data.GetSubGraph() != NULL);
		if(!data.AreAllSubGraphHighlighted())
			return false;
	}
	return true;
}

// �ش� name�� subGraph �� �ִ� �� ���� = true
bool CSelectedSubGraphDeque::AreExistenceSubGraph(const CString& strSubGraphName) const
{
	if(GetCount() <= 0)
		return false;

	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		CSelectedSubGraphData data = m_selectedSubGraphDeque.GetAt(nIndex);
		assert(data.GetSubGraph() != NULL);
		if(data.AreExistenceSubGraph(strSubGraphName))
			return true;
	}
	return false;
}


// ----------------------------------------------------------------------------
bool CSelectedSubGraphDeque::DrawGrip(CDC* pDC)
{
	if(GetCount() <= 0)
		return false;

	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		m_selectedSubGraphDeque.GetAt(nIndex).DrawGrip(pDC);
	}

	return true;
}


// private ====================================================================
// subGraph ����
bool CSelectedSubGraphDeque::DeleteOrHideSubGraph(const int nIndex)
{
	CSelectedSubGraphData data = GetAt(nIndex);
	if(data.GetSubGraph() == NULL)
		return false;

	bool bResult = data.DeleteOrHideSubGraph();
	if(bResult && m_selectedSubGraphDeque.GetCount() > 0)
		m_selectedSubGraphDeque.RemoveAt(nIndex);
	return bResult;
}

// ----------------------------------------------------------------------------
// subGraph ����
bool CSelectedSubGraphDeque::OnMenu_SubGraphHide(const bool bIsAction) 
{
	if(!bIsAction || GetCount() <= 0)
		return false;

	bool bResult = false;
	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		if(DeleteOrHideSubGraph(nIndex))
			bResult = true;
	}

	return bResult;
}

// graph color -> ù��°�� �ٲ۴�.
bool CSelectedSubGraphDeque::OnMenu_SubGraphColor(const COLORREF& color)
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		m_selectedSubGraphDeque.GetAt(nIndex).SetSubGraphColor(color);
	}
	return true;
}

// graph thickness -> ù��°�� �ٲ۴�.
bool CSelectedSubGraphDeque::OnMenu_SubGraphThickness(const int nThickness)
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		CSelectedSubGraphData selectedData = m_selectedSubGraphDeque.GetAt(nIndex);
		selectedData.SetSubGraphHighlight(false);
		selectedData.SetSubGraphThickness(nThickness);
	}
	return true;
}

// ����
bool CSelectedSubGraphDeque::OnMenu_SubGraphHighlight(const bool bIsHightlight) 
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		m_selectedSubGraphDeque.GetAt(nIndex).SetSubGraphHighlight(bIsHightlight);
	}
	return true;
}

// subGraph draw style	
bool CSelectedSubGraphDeque::OnMenu_SubGraphDrawStyle(const CGraphDrawStyle& drawStyle)
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		m_selectedSubGraphDeque.GetAt(nIndex).SetSubGraphDrawStyle(drawStyle);
	}
	return true;
}

// ----------------------------------------------------------------------------
// subGraph �� GraphType�� �ִ� ��� = true
bool CSelectedSubGraphDeque::AreSubGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++){
		CSubGraph* pSubGraph = m_selectedSubGraphDeque.GetAt(nIndex).GetSubGraph();
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetType() == eGraphType)
			return true;
	}
	
	return false;
}


///////////////////////////////////////////////////////////////////////////////
//class CMovingGraphDeque

CMovingGraphDeque& CMovingGraphDeque::operator=(const CMovingGraphDeque& movingGraphDeque)
{
	if(this == &movingGraphDeque)
		return *this;

	for(int nIndex = 0; nIndex < movingGraphDeque.GetCount(); nIndex++){
		m_movingGraphDeque.AddTail(movingGraphDeque.GetAt(nIndex));
	}
	return *this;
}

// public =====================================================================
void CMovingGraphDeque::AddTail(CGraphImp* pGraph)
{
	if(pGraph != NULL)
		m_movingGraphDeque.AddTail(pGraph);
}

void CMovingGraphDeque::RemoveAll()
{
	m_movingGraphDeque.RemoveAll();
}

int CMovingGraphDeque::GetCount() const
{
	return m_movingGraphDeque.GetCount();
}

CGraphImp* CMovingGraphDeque::GetAt(const int nIndex) const
{
	if(nIndex < 0 || nIndex >= GetCount())
		return NULL;
	return m_movingGraphDeque.GetAt(nIndex);
}

CGraphImp* CMovingGraphDeque::GetAt(const int nIndex)
{
	if(nIndex < 0 || nIndex >= GetCount())
		return NULL;
	return m_movingGraphDeque.GetAt(nIndex);
}