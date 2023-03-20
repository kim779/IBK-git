// SubGraphList.cpp: implementation of the CSubGraphList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SubGraphList.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "../Include_Chart/Conversion.h"
#include "VertScaleGroup.h"
#include "GraphImp.h"
#include "SubGraph.h"
#include "SelectedSubGraphDeque.h"
#include "GraphSearchInfo.h"				// for CGraphSearchInfo

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubGraphList::CSubGraphList():
	m_pGraph(NULL)
{
}

CSubGraphList::~CSubGraphList()
{
	DeleteAll();
}

//////////////////////////////////////////////////////////////////////
// Search Object
//////////////////////////////////////////////////////////////////////

CSubGraph *CSubGraphList::GetSubGraph( int p_nIndex)
{
	if( p_nIndex < 0 || m_SubGraphList.GetCount() <= p_nIndex) return NULL;

	POSITION posSubGraph = m_SubGraphList.FindIndex( p_nIndex);
	if( !posSubGraph) return NULL;

	return m_SubGraphList.GetAt( posSubGraph);
}


// (2007/2/22 - Seung-Won, Bae) Support Next Search.
BOOL CSubGraphList::FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nSubGraphIndex		= p_pGraphSearchInfo->m_nSubGraphIndex;
	POSITION posSubGraph	= ( p_pGraphSearchInfo->m_posSubGraph ? p_pGraphSearchInfo->m_posSubGraph : m_SubGraphList.FindIndex( p_pGraphSearchInfo->m_nSubGraphIndex));

	CSubGraph *pSubGraph = NULL;
	POSITION posCurrentSubGraph = NULL;
	while( posSubGraph)
	{
		posCurrentSubGraph = posSubGraph;
		pSubGraph = m_SubGraphList.GetNext( posSubGraph);
		if( !pSubGraph) continue;
		if( p_pGraphSearchInfo->FindGraph( pSubGraph))
		{
			p_pGraphSearchInfo->m_posSubGraph		= posCurrentSubGraph;
			p_pGraphSearchInfo->m_nSubGraphIndex	= nSubGraphIndex;
			p_pGraphSearchInfo->m_pSubGraph			= pSubGraph;
			return TRUE;
		}
		nSubGraphIndex++;
	}

	// (2007/4/11 - Seung-Won, Bae) Reset for Next Graph Search
	p_pGraphSearchInfo->m_posSubGraph		= NULL;
	p_pGraphSearchInfo->m_nSubGraphIndex	= 0;
	return FALSE;
}































// public ================================================================================
// subGraph�� ����&����
void CSubGraphList::MakeSubGraphs( char *p_szSubGraphsData, const char *p_szIndicatorName)
{
	DeleteAll();

	if( !p_szSubGraphsData) return;
	if( !*p_szSubGraphsData) return;

	char *szSubGraphData		= p_szSubGraphsData;
	char *szNextSubGraphData	= NULL;

	while( szSubGraphData)
	{
		if( !*szSubGraphData) break;

		szNextSubGraphData = strstr( szSubGraphData, "\r\n");
		if( szNextSubGraphData) *szNextSubGraphData = '\0';
		MakeSubGraph( szSubGraphData, p_szIndicatorName);
		if( szNextSubGraphData)
		{
			*szNextSubGraphData = '\r';
			szNextSubGraphData += 2;		// "\r\n"
		}
		szSubGraphData = szNextSubGraphData;
	}
}

// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
void CSubGraphList::MakeSubGraphs( char *p_szSubGraphsData, const char *p_szIndicatorName, LPCTSTR lpszItemName)
{
	DeleteAll();

	if( !p_szSubGraphsData) return;
	if( !*p_szSubGraphsData) return;

	char *szSubGraphData		= p_szSubGraphsData;
	char *szNextSubGraphData	= NULL;

	// ����� ǥ�� : �������� - ojtaso (20070625)
	if(szSubGraphData)	// SubGraph�� ���� ó������ ����� ǥ��
	{
		if( !*szSubGraphData) return;

		szNextSubGraphData = strstr( szSubGraphData, "\r\n");
		if( szNextSubGraphData) *szNextSubGraphData = '\0';
		MakeSubGraph( szSubGraphData, p_szIndicatorName, lpszItemName);
		if( szNextSubGraphData)
		{
			*szNextSubGraphData = '\r';
			szNextSubGraphData += 2;		// "\r\n"
		}
		szSubGraphData = szNextSubGraphData;
	}

	while( szSubGraphData)
	{
		if( !*szSubGraphData) break;

		szNextSubGraphData = strstr( szSubGraphData, "\r\n");
		if( szNextSubGraphData) *szNextSubGraphData = '\0';
		MakeSubGraph( szSubGraphData, p_szIndicatorName);
		if( szNextSubGraphData)
		{
			*szNextSubGraphData = '\r';
			szNextSubGraphData += 2;		// "\r\n"
		}
		szSubGraphData = szNextSubGraphData;
	}
}

// ���� ���õ� ��ü�� �����Ѵ�.
void CSubGraphList::Delete(CSubGraph* pSubGraph)
{
	assert(pSubGraph != NULL);

	// subGraph �� �ϳ� ���� ������ ��� -- graph �� ����.
	if(!DeleteEmptyGraph())
	{
		DeleteSubGraph(pSubGraph);
		DeleteGraphInAllSubGraphHiding();
	}
}

// ����Ʈ�� ��� ���� ����
void CSubGraphList::DeleteAll()
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	int nCount = m_SubGraphList.GetCount();
	if(nCount <= 0) return;
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		pSubGraph->Delete();
		m_SubGraphList.RemoveHead(); 
	}
// 	while( !m_SubGraphList.IsEmpty())
// 	{
// 		// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
// 		//		In Destructor, can be access the List.
// 		CSubGraph* pSubGraph = m_SubGraphList.GetHead();
// 		int iitest = m_SubGraphList.GetCount();
// 		assert(pSubGraph != NULL);
// 		pSubGraph->Delete();
// 		m_SubGraphList.RemoveHead();
// 	}
	DeleteEmptyGraph();
}

void CSubGraphList::DeleteAllPackets(const bool bIsOnlyNoTRData)
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		pSubGraph->DeleteAllPackets(bIsOnlyNoTRData);
	}
}

void CSubGraphList::ClearAllPackets(const bool bIsOnlyNoTRData)
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		pSubGraph->ClearAllPackets(bIsOnlyNoTRData);
	}
}

// subGraphPacketList �ʱ�ȭ
void CSubGraphList::Initialize_AllPackets()
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		pSubGraph->GetSubGraphPacketList().RemoveAll();
	}
}

// ---------------------------------------------------------------------------------------
// �θ� �ּ�
void CSubGraphList::SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData)
{
	m_pMainBlockBaseData = pMainBlockBaseData;

	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		pSubGraph->SetMainBlockBaseData(m_pMainBlockBaseData);
	}
}

void CSubGraphList::SetGraph(CGraphImp* pGraph)
{
	m_pGraph = pGraph;
}

/*
// ��� subGraph�� data�� �ٲ۴�.
void CSubGraphList::SetSubGraphs(const CString& strData)
{
	// data�� this�� ���� index ��°�� subGraph�� ���븸�� data���� this�� �����Ѵ�.
	// ���� data ���� �� ���� ���� this�� ���� ����� add �Ѵ�.
	int nDeleteStartIndex = CopySubGraphs(strData);

	// data�� �������� ���� ��쿡�� this �� �ִ°� �����Ѵ�.
	// ex) data : subGraph �� - 4   this : subGraph �� - 6
	//     --> this�� �ִ� 4, 5��°�� subGraph ���� �Ѵ�.
	DeleteSubGraphs(nDeleteStartIndex);
}
*/

// ---------------------------------------------------------------------------------------
// subGraph�� ���̱� setting.
bool CSubGraphList::SetAllHiding( const bool bIsHiding)
{
	bool bResult = false;
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->SetHiding(bIsHiding))
			bResult = true;
	}

	return bResult;
}

bool CSubGraphList::SetHiding(const CString& targetTitle, const bool bIsHiding)
{
	bool ret = false;
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetTitle() == targetTitle)
		{
			pSubGraph->SetHiding(bIsHiding);
			ret = true;
		}
	}
	return ret;
}

bool CSubGraphList::SetHiding(const int nIndex, const bool bIsHiding)
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return false;

	pSubGraph->SetHiding(bIsHiding);
	return true;
}

// subGraph data setting.
bool CSubGraphList::SetData(const int nIndex, const CString& changeTitle, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CPenThickness& penThickness)
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return false;

	pSubGraph->GetSubGraphData().SetTitle(changeTitle);
	pSubGraph->GetSubGraphData().SetType(sourceType);
	pSubGraph->GetSubGraphData().SetStyle(sourceStyle);
	pSubGraph->GetSubGraphData().SetDrawStyle(sourceDrawStyle);
	pSubGraph->GetSubGraphData().SetPenThickness(penThickness);
	return true;
}

// subGraph name setting.
bool CSubGraphList::SetSubGraphName(const int nIndex, const CString& strSubGraphName)
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return false;

	if(pSubGraph->GetSubGraphData().GetName() == strSubGraphName)
		return false;

	pSubGraph->GetSubGraphData().SetName(strSubGraphName);
	return true;
}

// subGraph title setting.
bool CSubGraphList::SetTitle(const CString& orgTitle, const CString& changeTitle)
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetTitle() == orgTitle)
		{
			pSubGraph->GetSubGraphData().SetTitle(changeTitle);
			return true;
		}
	}
	return false;
}

bool CSubGraphList::SetTitle(const int nIndex, const CString& changeTitle)
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return false;

	pSubGraph->GetSubGraphData().SetTitle(changeTitle);
	return true;
}

// subGraph type ����.
bool CSubGraphList::SetType(const int nIndex, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle)
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return false;

	pSubGraph->GetSubGraphData().SetType(sourceType);
	pSubGraph->GetSubGraphData().SetStyle(sourceStyle);
	pSubGraph->GetSubGraphData().SetDrawStyle(sourceDrawStyle);
	return true;
}

// subGraph ���� ���� ����.
bool CSubGraphList::SetAllSubGraphPenThickness(const CPenThickness& penThickness)
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		pSubGraph->GetSubGraphData().SetPenThickness(penThickness);
	}

	return true;
}

bool CSubGraphList::SetPenThickness(const int nIndex, const CPenThickness& penThickness)
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return false;

	pSubGraph->GetSubGraphData().SetPenThickness(penThickness);
	return true;
}

bool CSubGraphList::SetSubGraphColor(const int nIndex, const CGraphColor& graphColor)
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return false;

	pSubGraph->GetSubGraphData().SetColor(graphColor);
	return true;
}

// ---------------------------------------------------------------------------------------
// ��� subGraphData ����.
bool CSubGraphList::ChangeAllSubGraphData(const CList<CSubGraphData*, CSubGraphData*>& sourceSubGraphDataList, const bool bIsAllChange)
{
	if(sourceSubGraphDataList.GetCount() <= 0)
		return false;

	POSITION subGraph_Pos = m_SubGraphList.GetHeadPosition();
	POSITION source_Pos = sourceSubGraphDataList.GetHeadPosition();
	while(subGraph_Pos != NULL && source_Pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(subGraph_Pos);
		CSubGraphData* pSourceSubGraphData = sourceSubGraphDataList.GetNext(source_Pos);
		assert(pSubGraph != NULL && pSourceSubGraphData != NULL);

		pSubGraph->SetSubGraphData(*pSourceSubGraphData, bIsAllChange);
	}
	return true;
}

bool CSubGraphList::ChangeSubGraphData(const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData)
{
	CSubGraph* pTargetSubGraph = FindSubGraph(targetSubGraphName);
	if(pTargetSubGraph == NULL)
		return false;

	pTargetSubGraph->GetSubGraphData() = sourceSubGraphData;
	return true;
}

// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
void CSubGraphList::ChangeSubGraphTitle(LPCTSTR lpszPreName, LPCTSTR lpszNewName)
{
	// ����� ǥ�� : �������� - ojtaso (20070625)
	CSubGraph* pTargetSubGraph = FindSubGraphTitle(lpszPreName);
	if(pTargetSubGraph)
		pTargetSubGraph->GetSubGraphData().SetItemName(lpszNewName);
}

// ---------------------------------------------------------------------------------------
// subGraphs ��ΰ� "����"���� üũ
bool CSubGraphList::AreHidingAllSubGraphs() const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(!pSubGraph->GetHiding()) return false;
	}

	return true;
}

// subGraph title name�� ��� empty���� üũ
bool CSubGraphList::IsViewableSubGraphTitleEmpty() const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if( pSubGraph->GetHiding()) continue;
		if(!pSubGraph->GetSubGraphData().GetTitle().IsEmpty())
			return false;
	}

	return true;
}

// subGraph���� �����ϴ��� ����
bool CSubGraphList::IsExistenceSubGrape(const CString& subGraphName) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetName() == subGraphName)
			return true;
	}
	return false;
}

// �ش� graph type�� �����ϴ��� üũ
bool CSubGraphList::IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetType() == eGraphType)
			return true;
	}
	return false;
}

bool CSubGraphList::IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType, const int nSubGraphIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nSubGraphIndex);
	if(pSubGraph == NULL)
		return false;

	return (pSubGraph->GetSubGraphData().GetType() == eGraphType);
}

// �ش� graph style�� �����ϴ��� üũ
bool CSubGraphList::IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetType() != eGraphType)
			continue;

		if(pSubGraph->GetSubGraphData().GetStyle().GetGraphStyle() == nGraphStyle)
			return true;
	}
	return false;
}

bool CSubGraphList::IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nSubGraphIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nSubGraphIndex);
	if(pSubGraph == NULL)
		return false;

	if(pSubGraph->GetSubGraphData().GetType() != eGraphType)
		return false;

	return (pSubGraph->GetSubGraphData().GetStyle().GetGraphStyle() == nGraphStyle);
}

// �ش� graph draw style�� �����ϴ��� üũ
bool CSubGraphList::IsViewableGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nGraphDrawStyle) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetType() != eGraphType)
			continue;

		if(pSubGraph->GetSubGraphData().GetStyle().GetGraphStyle() == nGraphStyle)
		{
			if(pSubGraph->GetSubGraphData().GetDrawStyle().GetDrawStyle() == nGraphDrawStyle)
				return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
// ���� packet�� �����ϴ��� Ȯ���Ѵ�.
bool CSubGraphList::IsExistenceSamePacketInGraph(CGraphImp* pTargetGraph) const
{
	if(pTargetGraph == NULL)
		return false;

	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->IsExistenceSamePacketInGraph(pTargetGraph))
			return true;
	}
	return false;
}

bool CSubGraphList::IsExistenceSamePacket(CPacket* pPacket) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphPacketList().IsExistenceSamePacket(pPacket))
			return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
// �θ� �ּ�
CGraphImp* CSubGraphList::GetGraph() const
{
	return m_pGraph;
}

// subGraph ����
int CSubGraphList::GetSubGraphCount() const
{
	return m_SubGraphList.GetCount();
}

// ��� subGraphData
bool CSubGraphList::GetAllSubGraphDataList(CList<CSubGraphData*, CSubGraphData*>& subGraphDataList) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		subGraphDataList.AddTail(pSubGraph->GetSubGraphData_PointerType());
	}

	return (subGraphDataList.GetCount() > 0);
}

CSubGraphData* CSubGraphList::GetSubGraphData(const CString& strSubGraphName) const
{
	CSubGraph* pSubGraph = FindSubGraph(strSubGraphName);
	if(pSubGraph == NULL)
		return NULL;

	return &pSubGraph->GetSubGraphData();
}

CSubGraphData* CSubGraphList::GetSubGraphData(const int nSubGraphIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nSubGraphIndex);
	if(pSubGraph == NULL)
		return NULL;

	return &pSubGraph->GetSubGraphData();
}

// ȭ�鿡 ���̴� subGraph�� ����
int CSubGraphList::GetViewableSubGraphCount() const
{
	int count = 0;
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if( !pSubGraph->GetHiding()) count++;
	}

	return count;
}

// subGraphs name.
CString CSubGraphList::GetSubGraphNames() const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	CString subGraphNames;
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		CString subGraphName = pSubGraph->GetSubGraphData().GetName();
		if(!subGraphName.IsEmpty())
			subGraphNames += subGraphName + ",";
	}

	return subGraphNames;
}

CString CSubGraphList::GetSubGraphName(const int nIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return "";

	return pSubGraph->GetSubGraphData().GetName();
}

CString CSubGraphList::GetViewableSubGraphNames() const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	CString subGraphNames;
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if( !pSubGraph->GetHiding()) // "����"�ƴ� ��츸.
			subGraphNames += pSubGraph->GetSubGraphData().GetName() + ",";
	}
	return subGraphNames;
}

// subGraphs title.
bool CSubGraphList::GetAllSubGraphTitle(CList<CString, CString>& titleList) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL){
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		titleList.AddTail(pSubGraph->GetSubGraphData().GetTitle());
	}

	return (titleList.GetCount() > 0);
}

CString CSubGraphList::GetSubGraphTitle(const int nIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return "";

	return pSubGraph->GetSubGraphData().GetTitle();
}

CString CSubGraphList::GetViewableSubGraphTitles() const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	CString subGraphTitles;
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if( !pSubGraph->GetHiding()) // "����"�ƴ� ��츸.
			subGraphTitles += pSubGraph->GetSubGraphData().GetTitle();
	}
	return subGraphTitles;
}

// subGraph title�� �ش�Ǵ����� üũ
int CSubGraphList::GetSameTitleSubgraphIndex(const CString& subGraphTitle) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	int nIndex = -1;
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		nIndex++;
		if(pSubGraph->GetSubGraphData().GetTitle() == subGraphTitle)
			return nIndex;
	}
	return -1;
}

// subGraphs���� data
void CSubGraphList::GetSubGraphsData( CStringList &p_slSubGraphData) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = (CSubGraph*) m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);

		p_slSubGraphData.AddTail( CString());
		CString &strSubGraphData = p_slSubGraphData.GetTail();
		pSubGraph->GetSubGraphStringData( strSubGraphData);
	}
}

// head subGraph type, style
bool CSubGraphList::GetSubGraphType(const int nIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return false;

	eGraphType = pSubGraph->GetSubGraphData().GetType();
	graphStyle = pSubGraph->GetSubGraphData().GetStyle();
	graphDrawStyle = pSubGraph->GetSubGraphData().GetDrawStyle();
	return true;
}

CGraphBaseData::GRAPHTYPE CSubGraphList::GetSubGraphType(const int nIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return CGraphBaseData::GTNONE;

	return pSubGraph->GetSubGraphData().GetType();
}

CGraphStyle CSubGraphList::GetSubGraphStyle(const int nIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return CGraphStyle(-1);

	return pSubGraph->GetSubGraphData().GetStyle();
}

CGraphDrawStyle CSubGraphList::GetSubGraphDrawStyle(const int nIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nIndex);
	if(pSubGraph == NULL)
		return CGraphDrawStyle(-1);

	return pSubGraph->GetSubGraphData().GetDrawStyle();
}

// graph ��
bool CSubGraphList::GetSubGraphsColor(CList<CGraphColor, CGraphColor&>& graphColorList) const
{
	graphColorList.RemoveAll();

	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		graphColorList.AddTail(pSubGraph->GetSubGraphData().GetColor());
	}

	return (graphColorList.GetCount() <= 0);
}

CGraphColor* CSubGraphList::GetGraphColor(const int nSubGraphIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nSubGraphIndex);
	if(pSubGraph == NULL)
		return NULL;

	return &pSubGraph->GetSubGraphData().GetColor();
}

COLORREF CSubGraphList::GetGraphColor1(const int nSubGraphIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nSubGraphIndex);
	if(pSubGraph == NULL)
		return RGB(255, 255, 255);
	
	return pSubGraph->GetSubGraphData().GetColor().GetColor1();
}

// subGraph title region
CRect CSubGraphList::GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* ptargetSubGraph = m_SubGraphList.GetNext(pos);
		assert(ptargetSubGraph != NULL);
		CRect region = ptargetSubGraph->GetSubGraphTitleRegion(pDC, graphRegion, preTitleEndPosition);
		if(ptargetSubGraph == pSubGraph)
			return region;
	}

	return CRect(0, 0, 0, 0);
}

// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
CRect CSubGraphList::GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition, int& nPreLineCount) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* ptargetSubGraph = m_SubGraphList.GetNext(pos);
		assert(ptargetSubGraph != NULL);
		CRect region = ptargetSubGraph->GetSubGraphTitleRegion(pDC, graphRegion, preTitleEndPosition, nPreLineCount);
		if(ptargetSubGraph == pSubGraph)
			return region;
	}

	return CRect(0, 0, 0, 0);
}

// ---------------------------------------------------------------------------------------
// view data setting
void CSubGraphList::GetSubGraphViewDataList(const CString& GraphName, const CString& packetName, CSubGraphViewDataList& subGraphViewDataList) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		pSubGraph->GetSubGraphViewDataList(GraphName, packetName, subGraphViewDataList);
	}
}

// ---------------------------------------------------------------------------------------
// packet�� ���õ� ����
bool CSubGraphList::GetAllSubGraphCalculateData(CGraphCalculateData& graphCalculateData) const
{
	graphCalculateData.RemoveAll();

	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		graphCalculateData.AddTail(&pSubGraph->GetSubGraphPacketList());
	}
	return (graphCalculateData.GetCount() > 0);
}

bool CSubGraphList::GetSubGraphPacketList(CList<CPacket*, CPacket*>& packetList) const
{
	bool bResult = false;
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphPacketList(packetList))
			bResult = true;
	}

	return bResult;
}

CSubGraphPacketList* CSubGraphList::GetSubGraphPacketList(const int nSubGraphIndex) const
{
	CSubGraph* pSubGraph = FindSubGraph(nSubGraphIndex);
	if(pSubGraph == NULL)
		return NULL;

	return &pSubGraph->GetSubGraphPacketList();
}

CPacket* CSubGraphList::GetHeadPacket() const
{
	CSubGraph* pSubGraph = FindSubGraph(0);
	if(pSubGraph == NULL)
		return NULL;

	return pSubGraph->GetSubGraphPacketList().GetHeadPacket();
}

bool CSubGraphList::GetHeadPacketAndDataType(CPacket*& pPacket, double& dDataType) const
{
	CSubGraph* pSubGraph = FindSubGraph(0);
	if(pSubGraph == NULL)
		return NULL;

	return pSubGraph->GetHeadPacketAndDataType(pPacket, dDataType);
}

// ---------------------------------------------------------------------------------------
// ���� scale width ���ϱ�.
int CSubGraphList::GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const
{
	int nWidth = 0;
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		int nData = pSubGraph->GetVertScaleRegionWidth(pDC, pCalData);
		if(nData > nWidth)
			nWidth = nData;
	}
	return nWidth;
}

// ���� ���̴� ȭ�鿡���� min, max�� ���Ѵ�. ==> y���� ��
bool CSubGraphList::GetDisplayDataMinMax(CVertDisplayDataMinMaxInputData* pInputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const
{
	bool bHead = true;
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		CDisplayDataMinMax temp;
		if(!pSubGraph->GetDisplayDataMinMax(pInputData, temp, p_bWithFullRange))
			continue;

		if(bHead)
		{
			displayDataMinMax = temp;
			bHead = false;
		}
		else
			displayDataMinMax.Inflate(temp);
	}
	return !bHead;
}

// ---------------------------------------------------------------------------------------
// graph �׸���
// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
bool CSubGraphList::DrawSubGraphTitles(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine)
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	// subGraph �׸���.
	bool ret = false;

	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	int nSubGraphIndex = -1;
	BOOL bDrawConditionEachTitle = g_iMetaTable.IsTitleAndCondition(m_pGraph->GetGraphData().GetIndicatorName2());

	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);

		// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
		if(bDrawConditionEachTitle)
			nSubGraphIndex++;

		// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
		// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
		if( pSubGraph->DrawSubGraphTitle( pDC, pDrawingGraphData, graphTitlePosition, nGraphTitleEndLine, nSubGraphIndex))
		{
			// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
			ClearDrawingGraphData(pDrawingGraphData, bDrawConditionEachTitle);
			ret = true;
		}
//>> Ÿ��Ʋ ������ ǥ�ý� ������ ������ ������ �ʵ��� ó�� - vntsorl (20090106)
		else
		{
			if(graphTitleLine < graphTitlePosition + MARGIN * 2)
			{
				graphTitlePosition = VSGL_TITLE_START_POSITION;
				nGraphTitleEndLine++;

				if(pSubGraph->DrawSubGraphTitle(pDC, pDrawingGraphData, graphTitlePosition, nGraphTitleEndLine, nSubGraphIndex))
				{
					// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
					ClearDrawingGraphData(pDrawingGraphData, bDrawConditionEachTitle);
					ret = true;
				}
				continue;
			}
		}
		
		if(graphTitleLine < graphTitlePosition + MARGIN * 2)
		{
			graphTitlePosition = VSGL_TITLE_START_POSITION;
			nGraphTitleEndLine++;
		}
//<< Ÿ��Ʋ ������ ǥ�ý� ������ ������ ������ �ʵ��� ó�� - vntsorl (20090106)
	}
	return ret;
}

// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
bool CSubGraphList::DrawSubGraphs(CDC* pDC, CDrawingGraphData* pDrawingGraphData)
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	// subGraph �׸���.
	bool ret = false;

	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
		if(pSubGraph->DrawSubGraph(pDC, pDrawingGraphData))
		{
			// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
			ClearDrawingGraphData(pDrawingGraphData, TRUE);
			ret = true;
		}
	}

	return ret;
}

// ---------------------------------------------------------------------------------------
// subGraph Title�� �̿��Ͽ� subGraphã��.
CSubGraph* CSubGraphList::FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, CRect& titleRegion, int& preTitleEndPosition) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();

	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		CRect region = pSubGraph->GetSubGraphTitleRegion(pDC, allTitleRegion, preTitleEndPosition);
		if(region.PtInRect(point))
		{
			titleRegion = region;
			return pSubGraph;
		}
	}

	return NULL;
}

// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
CSubGraph* CSubGraphList::FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, CRect& titleRegion, int& preTitleEndPosition, int& nPreLineCount) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();

	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
		CRect region = pSubGraph->GetSubGraphTitleRegion(pDC, allTitleRegion, preTitleEndPosition, nPreLineCount);
		if(region.PtInRect(point))
		{
			titleRegion = region;
			return pSubGraph;
		}
	}

	return NULL;
}

// Ÿ��Ʋ�� ã�� : �������� - ojtaso (20070322)
// ����� ǥ�� : �������� - ojtaso (20070625)
CSubGraph* CSubGraphList::FindSubGraphTitle(const CString& strSubGraphTitle) const
{
	if(strSubGraphTitle.IsEmpty())
		return NULL;

	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetItemName() == strSubGraphTitle)
			return pSubGraph;
	}
	return NULL;
}

// subGraph ã��.
CSubGraph* CSubGraphList::FindSubGraph(const int nIndex) const
{
	POSITION pos = m_SubGraphList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;

	return m_SubGraphList.GetAt(pos);
}

CSubGraph* CSubGraphList::FindSubGraph(const CString& strSubGraphName) const
{
	if(strSubGraphName.IsEmpty())
		return NULL;

	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(pSubGraph->GetSubGraphData().GetName() == strSubGraphName)
			return pSubGraph;
	}
	return NULL;
}

bool CSubGraphList::FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		if(IsSelectedSubGraphInDeque(pSubGraph, selectedSubGraphDeque))
			continue;

		int nDataIndex = pSubGraph->PointInSubGraph(point);
		if(nDataIndex >= 0)
		{
			selectedSubGraphDeque.AddTail(CSelectedSubGraphData(pSubGraph, nDataIndex));
			return true;
		}
	}
	return false;
}

/*
bool CSubGraphList::FindSubGraph(const CPoint& point, CString& strSubGraphData, int& nSubGraphIndex, int& nDataIndex) const
{
	int nIndex = 0;
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);

		int nPacketDataIndex = pSubGraph->PointInSubGraph(point);
		if(nPacketDataIndex >= 0)
		{
			pSubGraph->GetSubGraphStringData( strSubGraphData);
			nSubGraphIndex = nIndex;
			nDataIndex = nPacketDataIndex;
			return true;
		}

		nIndex++;
	}
	return false;
}
*/

// private ===============================================================================
bool CSubGraphList::MakeSubGraph( const char *p_szSubGraphData, const char *p_szIndicatorName)
{
	if( !p_szSubGraphData) return false;
	if( !*p_szSubGraphData) return false;

	CSubGraphData subGraphData( p_szSubGraphData, m_pMainBlockBaseData->GetParent_Handle(), p_szIndicatorName);
	if(subGraphData.GetType() == CGraphBaseData::GTNONE)
		return false;

	Add(CSubGraph::MakeSubGraph(m_pMainBlockBaseData, this, subGraphData));
	return true;
}

// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
bool CSubGraphList::MakeSubGraph( const char *p_szSubGraphData, const char *p_szIndicatorName, LPCTSTR lpszItemName)
{
	if( !p_szSubGraphData) return false;
	if( !*p_szSubGraphData) return false;

	CSubGraphData subGraphData( p_szSubGraphData, m_pMainBlockBaseData->GetParent_Handle(), p_szIndicatorName);
	if(subGraphData.GetType() == CGraphBaseData::GTNONE)
		return false;

	// ����� ǥ�� : �������� - ojtaso (20070625)
	subGraphData.SetItemName(lpszItemName);
	Add(CSubGraph::MakeSubGraph(m_pMainBlockBaseData, this, subGraphData));
	return true;
}

// list�� �߰��Ѵ�.
void CSubGraphList::Add(CSubGraph* pSubGraph)
{
	assert(pSubGraph != NULL);
	m_SubGraphList.AddTail(pSubGraph);
}

// subGraph�� list�� �ϳ��� ���� ��� graph ����.
bool CSubGraphList::DeleteEmptyGraph()
{
	if(m_SubGraphList.GetCount() != 1 || m_pGraph == NULL)
		return false;
	
	return DeleteGraph();
}


// ��� subGraph�� ���� �̸� graph�� �����Ѵ�.
bool CSubGraphList::DeleteGraphInAllSubGraphHiding()
{
	if(m_pGraph == NULL)
		return false;

	if(!AreHidingAllSubGraphs())
		return false;

	return DeleteGraph();
}

bool CSubGraphList::DeleteGraph()
{
	if(m_pGraph == NULL)
		return false;

	CVertScaleGroup* pVertScaleGroup = m_pGraph->GetVertScaleGroup();
	if(pVertScaleGroup == NULL)
		return false;
	
	pVertScaleGroup->DeleteGraphWithNotify(m_pGraph);
	m_pGraph = NULL;
	return true;
}

void CSubGraphList::DeleteSubGraphs(const int nStartIndex)
{
	if(nStartIndex < 0 || nStartIndex >= m_SubGraphList.GetCount())
		return;

	for(int nIndex = nStartIndex; nIndex < m_SubGraphList.GetCount(); nIndex++)
	{
		Delete(FindSubGraph(nIndex));		
	}
}

void CSubGraphList::DeleteSubGraph(CSubGraph* pSubGraph)
{
	assert(pSubGraph != NULL);
	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	pSubGraph->Delete();

	POSITION pos = m_SubGraphList.Find(pSubGraph);
	if(pos == NULL) return;
	m_SubGraphList.RemoveAt(pos);
}

// ---------------------------------------------------------------------------------------
/*
int CSubGraphList::CopySubGraphs(const CString& strData)
{
	int nCount = CDataConversion::GetStringDataCount(strData, "\r\n");
	if(nCount <= 0)
		return -1;

	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CString subGraphData = CDataConversion::GetMidToTrimDataIndex(strData, "\r\n", nIndex);
		if(!CopySubGraph(FindSubGraph(nIndex), subGraphData))
			break;
	}

	return nIndex;
}
*/

/*
bool CSubGraphList::CopySubGraph(CSubGraph* pSubGraph, const CString& strSubGraphData)
{
	// data�� this�� ���� index ��°�� subGraph�� ���븸�� data���� this�� �����Ѵ�.
	// ���� data ���� �� ���� ���� this�� ���� ����� add �Ѵ�.
	if(pSubGraph == NULL)
		return MakeSubGraph(strSubGraphData);
	
	return SetSubGraphData(pSubGraph, strSubGraphData);
}
*/

// CDrawingGraphData clear
// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
void CSubGraphList::ClearDrawingGraphData(CDrawingGraphData* pDrawingGraphData, BOOL bDrawConditionEachTitle)
{
	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	if(!bDrawConditionEachTitle)
	{
		if(pDrawingGraphData->DoesConditionTitleDraw())
			pDrawingGraphData->SetConditionTitleDraw(false);
	}

	pDrawingGraphData->SetGraphDataDrawingData(CGraphDataDrawingData());
}

// ---------------------------------------------------------------------------------------
// list�� subGraph�� �̹� �ִ����� ã�´�. true = �̹� ������.
bool CSubGraphList::IsSelectedSubGraphInDeque(CSubGraph* pSubGraph, CSelectedSubGraphDeque& selectedSubGraphDeque) const
{
	for(int nIndex = 0; nIndex < selectedSubGraphDeque.GetCount(); nIndex++)
	{
		CSelectedSubGraphData selectedSubGraphData = selectedSubGraphDeque.GetAt(nIndex);
		if(pSubGraph == selectedSubGraphData.GetSubGraph())
			return true;
	}
	return false;
}

// (2009/5/26 - Seung-Won, Bae) Clear SubGraph Drawing Cahe.
void CSubGraphList::ClearCalcDataForDraw( void)
{
	POSITION pos = m_SubGraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CSubGraph* pSubGraph = m_SubGraphList.GetNext(pos);
		assert(pSubGraph != NULL);
		pSubGraph->GetSubGraphData().ClearCalcDataForDraw();
		pSubGraph->ResetMaxVerticalScaleWidth();
	}
}
