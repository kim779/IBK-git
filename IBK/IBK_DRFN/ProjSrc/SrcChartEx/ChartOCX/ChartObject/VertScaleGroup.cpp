// VertScaleGroup.cpp: implementation of the CVertScaleGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VertScaleGroup.h"

#ifdef _TEST
#include <assert.h>
#include <math.h>											// for HUGE_VAL
#include "../Include_Chart/DLL_Load/IndicatorInfo.h"		// for CIndicatorInfo
#include "../Include_Chart/DLL_Load/PacketBase.h"			// for CPacket
#include "../Include_Chart/Dll_Load/PacketListManager.h"	// for CPacketListManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "../Include_Chart/Conversion.h"
#include "BlockImp.h"
#include "BlocksColumn.h"									// for CBlocksColumn
#include "GraphImp.h"
#include "ScaleAndGraphDrawer.h"
#include "CalculatorData.h"
#include "MainBlockImp.h"					// for CMainBlockImp
#include "GraphSearchInfo.h"							// for CGraphSearchInfo
#include "../Include_Chart/Dll_Load/XScaleManager.h"		// for CXScaleManager
#include "PacketRQ.h"									// for CPacketRQ


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// (2006/11/26 - Seung-Won, Bae) Support Empty VScaleGroup
CVertScaleGroup::CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList):
	m_pMainBlockBaseData( pMainBlockBaseData),
	m_pVertScaleGroupList( pVertScaleGroupList),
	m_displayDataMinMax( 0, 0),
	m_lpSelectedGraph(NULL)		// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;
}

CVertScaleGroup::CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroupList(pVertScaleGroupList),
	m_displayDataMinMax( 0, 0),
	m_lpSelectedGraph(NULL)		// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;

	MakeGraphs( p_szVertScaleGroup);
}

// RQ�� �˻� : �������� - ojtaso (20070118)
CVertScaleGroup::CVertScaleGroup(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroupList(pVertScaleGroupList),
	m_displayDataMinMax( 0, 0),
	m_lpSelectedGraph(NULL)		// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;

	MakeGraphs(pPacketRQ, p_szVertScaleGroup);
}


CVertScaleGroup::CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, const CVerticalScale& vertScale):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroupList(pVertScaleGroupList),
	m_vertScale(vertScale),
	m_displayDataMinMax(-20.0, 20.0),
	m_lpSelectedGraph(NULL)		// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;
}

CVertScaleGroup::~CVertScaleGroup()
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = TRUE;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph	= NULL;
	m_pBasePacket	= NULL;

	// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
	m_lpSelectedGraph = NULL;

	DeleteAll();
}

//////////////////////////////////////////////////////////////////////
// Delete
//////////////////////////////////////////////////////////////////////
BOOL CVertScaleGroup::DeleteAllIndicator( const char *p_szIndicatorName)
{
	if( !p_szIndicatorName)		return FALSE;
	if( !*p_szIndicatorName)	return FALSE;

	BOOL bResult = FALSE;
	CGraphImp *pGraph = NULL;
	POSITION posGraph = m_GraphList.GetHeadPosition();
	while( posGraph)
	{
		pGraph = m_GraphList.GetNext( posGraph);
		if( !pGraph) continue;
		if( pGraph->GetGraphData().GetIndicatorName() == p_szIndicatorName)
		{
			DeleteGraphWithNotify( pGraph, true);
		
			bResult = TRUE;
		}
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Search Object
//////////////////////////////////////////////////////////////////////

// (2007/2/22 - Seung-Won, Bae) Support Next Search.
BOOL CVertScaleGroup::FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nGraphIndex		= p_pGraphSearchInfo->m_nGraphIndex;
	POSITION posGraph	= ( p_pGraphSearchInfo->m_posGraph ? p_pGraphSearchInfo->m_posGraph : m_GraphList.FindIndex( p_pGraphSearchInfo->m_nGraphIndex));

	CGraphImp *pGraph = NULL;
	POSITION posCurrentGraph = NULL;
	while( posGraph)
	{
		posCurrentGraph = posGraph;
		pGraph = m_GraphList.GetNext( posGraph);
		if( !pGraph) continue;
		if( p_pGraphSearchInfo->FindGraph( pGraph))
		{
			p_pGraphSearchInfo->m_posGraph		= posGraph;
			p_pGraphSearchInfo->m_nGraphIndex	= nGraphIndex;
			p_pGraphSearchInfo->m_pGraphImp		= pGraph;
			return TRUE;
		}
		nGraphIndex++;
	}

	// (2007/4/11 - Seung-Won, Bae) Reset for Next Vertical Scale Group Search
	p_pGraphSearchInfo->m_posGraph		= NULL;
	p_pGraphSearchInfo->m_nGraphIndex	= 0;
	return FALSE;
}

// (2008/9/15 - Seung-Won, Bae) RQ Search
BOOL CVertScaleGroup::FindGraph( LPVOID pPacketRQ, CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nGraphIndex		= p_pGraphSearchInfo->m_nGraphIndex;
	POSITION posGraph	= ( p_pGraphSearchInfo->m_posGraph ? p_pGraphSearchInfo->m_posGraph : m_GraphList.FindIndex( p_pGraphSearchInfo->m_nGraphIndex));

	CGraphImp *pGraph = NULL;
	CPacketRQ* pRQ = (CPacketRQ*)pPacketRQ;
	POSITION posCurrentGraph = NULL;
	while( posGraph)
	{
		posCurrentGraph = posGraph;
		pGraph = m_GraphList.GetNext( posGraph);
		if( !pGraph) continue;
		if( p_pGraphSearchInfo->FindGraph( pGraph))
		{
			if(!::lstrcmp( pGraph->GetGraphRQ(), pRQ->GetRQ()))
			{
				p_pGraphSearchInfo->m_posGraph		= posGraph;
				p_pGraphSearchInfo->m_nGraphIndex	= nGraphIndex;
				p_pGraphSearchInfo->m_pGraphImp		= pGraph;
				return TRUE;
			}
		}
		nGraphIndex++;
	}

	// (2007/4/11 - Seung-Won, Bae) Reset for Next Vertical Scale Group Search
	p_pGraphSearchInfo->m_posGraph		= NULL;
	p_pGraphSearchInfo->m_nGraphIndex	= 0;
	return FALSE;
}

// Sub Graph
CSubGraph *CVertScaleGroup::GetSubGraph( const int nGraphIndex, const int nSubGraphIndex) const
{
	CGraphImp *pGraph = GetGraph( nGraphIndex);
	if( !pGraph) return NULL;

	return pGraph->GetSubGraphList().FindSubGraph( nSubGraphIndex);
}

// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
//		It can not be replaced with AddChartBlock(), because of Condition Reset.
void CVertScaleGroup::ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate)
{
	if( !p_szIndicatorName) return;
	if( !*p_szIndicatorName) return;

	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if( pGraph->GetGraphData().GetIndicatorName() != p_szIndicatorName)
			continue;

		pGraph->SetGraphHide( !p_bShow);

		if( p_bRecalculate && p_bShow) pGraph->CalculateGraph( CCalculateBaseData::TRDATA_CAL);
	}
}

// (2006/11/26 - Seung-Won, Bae) Support Empty VScaleGroup
CVertScaleGroup* CVertScaleGroup::MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList)
{
	return new CVertScaleGroup(pMainBlockBaseData, pVertScaleGroupList);
}

// CVertScaleGroup�� ����& ����
CVertScaleGroup* CVertScaleGroup::MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup)
{
	return new CVertScaleGroup(pMainBlockBaseData, pVertScaleGroupList, p_szVertScaleGroup);
}

// RQ�� �˻� : �������� - ojtaso (20070118)
CVertScaleGroup* CVertScaleGroup::MakeVertScaleGroup(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup)
{
	return new CVertScaleGroup(pPacketRQ, pMainBlockBaseData, pVertScaleGroupList, p_szVertScaleGroup);
}

CVertScaleGroup* CVertScaleGroup::MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, const CVerticalScale& vertScale)
{
	return new CVertScaleGroup(pMainBlockBaseData, pVertScaleGroupList, vertScale);
}

void CVertScaleGroup::Delete(CVertScaleGroup* pVertScaleGroup)
{
	if( !pVertScaleGroup->m_bOnDestructor) delete pVertScaleGroup;
}

/////////////////////////////////////////////////////////////////////////////////////////////


// public ================================================================================
// ������ Graph�� �����ϰ� ���ϴ� ������ Graph �����.
// "����� Data;��ġ;gridŸ��;unitŸ��;\r\n
//		Graph;\r\n
//		GraphData...\r\n
//		SubGraphData....\r\n
// GraphEnd;\r\n" �����̿�����.
void CVertScaleGroup::MakeGraphs( char *p_szVertScaleGroup)
{
	DeleteAll();

	char *szGraph = strstr( p_szVertScaleGroup, "\r\n");
	if( !szGraph) return;
	*szGraph = '\0';

	m_vertScale.SetScaleData( p_szVertScaleGroup, m_pMainBlockBaseData->GetParent_Handle());
	*szGraph = '\r';

	char cGraphEnd = 0;
	char *szGraphEnd = NULL;
	szGraph = strstr( szGraph, "Graph;\r\n");
	while( szGraph)
	{
		szGraphEnd = strstr( szGraph, "GraphEnd;\r\n");
		if( szGraphEnd)
		{
			szGraphEnd += 11;									// "GraphEnd;\r\n"
			cGraphEnd = *szGraphEnd;
			*szGraphEnd = '\0';
		}

		MakeGraph( szGraph);
		if( szGraphEnd)
		{
			*szGraphEnd = cGraphEnd;
			szGraph = strstr( szGraphEnd, "Graph;\r\n");
		}
		else szGraph = NULL;
	}
}

// RQ�� �˻� : �������� - ojtaso (20070118)
void CVertScaleGroup::MakeGraphs(LPVOID pPacketRQ,  char *p_szVertScaleGroup)
{
	DeleteAll();

	char *szGraph = strstr( p_szVertScaleGroup, "\r\n");
	if( !szGraph) return;
	*szGraph = '\0';

	m_vertScale.SetScaleData( p_szVertScaleGroup, m_pMainBlockBaseData->GetParent_Handle());
	*szGraph = '\r';

	char cGraphEnd = 0;
	char *szGraphEnd = NULL;
	szGraph = strstr( szGraph, "Graph;\r\n");
	while( szGraph)
	{
		szGraphEnd = strstr( szGraph, "GraphEnd;\r\n");
		if( szGraphEnd)
		{
			szGraphEnd += 11;									// "GraphEnd;\r\n"
			cGraphEnd = *szGraphEnd;
			*szGraphEnd = '\0';
		}

		MakeGraph(pPacketRQ, szGraph);
		if( szGraphEnd)
		{
			*szGraphEnd = cGraphEnd;
			szGraph = strstr( szGraphEnd, "Graph;\r\n");
		}
		else szGraph = NULL;
	}
}

// graph ����
bool CVertScaleGroup::MakeGraph( char *p_szGraphData, const bool bIsAddTail)
{
	if( !p_szGraphData) return false;
	if( !*p_szGraphData) return false;

	CGraphImp* pGraph = MakeNewGraph( p_szGraphData);
	if(pGraph == NULL) return false;

	AddGraph(pGraph, bIsAddTail);

	// (2006/11/17 - Seung-Won, Bae) Notify to OCX
	if( m_pMainBlockBaseData)
	{
		IChartCtrl *pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
		if( pChartCtrl)
		{
			CGraphData &graphData = pGraph->GetGraphData();
			pChartCtrl->OnGraphAdded( pGraph->GetGraphRQ(), graphData.GetGraphName(), graphData.GetIndicatorName());
		}
	}

	// (2007/1/19 - Seung-Won, Bae) Activate Vertical Scale Group.
	if( m_pVertScaleGroupList) m_pVertScaleGroupList->SetActivate( this);

	return true;
}

// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
bool CVertScaleGroup::MakeGraph(LPVOID pPacketRQ, char *p_szGraphData, const bool bIsAddTail)
{
	if( !p_szGraphData) return false;
	if( !*p_szGraphData) return false;

	CGraphImp* pGraph = MakeNewGraph(pPacketRQ, p_szGraphData);
	if(pGraph == NULL) return false;

	AddGraph(pGraph, bIsAddTail);

	// (2006/11/17 - Seung-Won, Bae) Notify to OCX
	if( m_pMainBlockBaseData)
	{
		IChartCtrl *pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
		if( pChartCtrl)
		{
			CGraphData &graphData = pGraph->GetGraphData();
			pChartCtrl->OnGraphAdded( pGraph->GetGraphRQ(), graphData.GetGraphName(), graphData.GetIndicatorName());
		}
	}

	return true;
}

bool CVertScaleGroup::DeleteGraphWithNotify(CGraphImp* pGraph, const bool bIsOnlyNoTRData)
{
	if( !pGraph) return false;

	// (2006/11/17 - Seung-Won, Bae) Notify to OCX
	// (2007/1/2 - Seung-Won, Bae) Vertical Scale Group can be deleted in DeleteGraph()
	//		So. Do Everything before DeleteGraph().
	if( m_pMainBlockBaseData)
	{
		IChartCtrl *pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
		if( pChartCtrl)
		{
			CGraphData &graphData = pGraph->GetGraphData();
			// �������� ������ ���� ���� : �������� - ojtaso (20070125)
			pChartCtrl->OnGraphDeleted(pGraph->GetGraphRQ(), graphData.GetGraphName(), graphData.GetIndicatorName());
		}
	}

	DeleteGraph( pGraph, bIsOnlyNoTRData);
	return true;
}

// ���� ���õ� ��ü�� �����Ѵ�.
bool CVertScaleGroup::DeleteGraph(CGraphImp* pGraph, const bool bIsOnlyNoTRData)
{
	assert(pGraph != NULL);
	CString strGraphName(pGraph->GetGraphData().GetGraphName()),
			strIndicatorName(pGraph->GetGraphData().GetIndicatorName());	
	
	if(strGraphName.GetLength()>0 && strGraphName.GetAt(0)=='#')
	{
		if(strIndicatorName.Compare("_Price MA_") !=0 && 
			strIndicatorName.Compare("_Volume MA_") !=0)
			pGraph->DeletePacket( bIsOnlyNoTRData);
	}
	else
	{
		pGraph->DeletePacket( bIsOnlyNoTRData);
	}

	// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
	if(m_lpSelectedGraph == pGraph)
		m_lpSelectedGraph = NULL;

	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	CGraphImp::Delete(pGraph);

	POSITION pos = m_GraphList.Find(pGraph);
	if(pos == NULL) return false;
	m_GraphList.RemoveAt(pos);

	// (2006/12/10 - Seung-Won, Bae) Support Self Destruction.
	//		Graph has Deleted in VScaleGroup's Destructor.
	//		Return Value must be false for No-More Action in DeleteGraphWithNotify().
	//		OCX Notify was processed in another DeleteGraphWithNotify() of VScaleGroup's Destructor.
	DeleteEmptyGroup();

	return true;
}

// ����Ʈ�� ��� ���� ����
// graph ������ ���ڸ��� ���� �����.
// ���� �Ŀ��� remove ��Ų��.
void CVertScaleGroup::DeleteAll()
{
	// (2006/12/10 - Seung-Won, Bae) Support Notify for ST.
	while( !m_GraphList.IsEmpty())
	{
		CGraphImp *pGraph = m_GraphList.GetTail();
		DeleteGraphWithNotify( pGraph);
	}
}

// ----------------------------------------------------------------------------------------
// list�� �߰��Ѵ�.
void CVertScaleGroup::AddGraph(CGraphImp* pGraph, const bool bIsAddTail)
{
	assert(pGraph != NULL);
	pGraph->SetVertScaleGroup(this);

	if(bIsAddTail)
		m_GraphList.AddTail(pGraph);
	else
		m_GraphList.AddHead(pGraph);
	
	CalculateDisplayDataMinMax();
}

bool CVertScaleGroup::RemoveAt(CGraphImp* pGraph, const bool bIsDeleteBlock)
{
	if(pGraph == NULL)
		return false;

	POSITION pos = m_GraphList.Find(pGraph);
	if(pos == NULL)
		return false;

	m_GraphList.RemoveAt(pos);

	if(m_GraphList.GetCount() == 0 && m_pVertScaleGroupList != NULL)
		m_pVertScaleGroupList->DeleteVertScaleGroup(this, bIsDeleteBlock);
	
	// (2009/3/9 - Seung-Won, Bae) Clear Selected Graph
	if( m_lpSelectedGraph == pGraph) m_lpSelectedGraph = NULL;

	return true;
}

// ----------------------------------------------------------------------------------------
bool CVertScaleGroup::MoveAllGraphs(CVertScaleGroupList& groupList)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	if(pos == NULL)
		return false;

	bool bResult = false;
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		if(groupList.MoveGraph(pGraph, -1, false))
			bResult = true;
	}

	return bResult;
}

bool CVertScaleGroup::MoveAllGraphs(CVertScaleGroup* pSourceGroup)
{
	if(pSourceGroup == NULL || pSourceGroup->GetGraphCount() <= 0)
		return false;

	while(pSourceGroup->GetGraphCount() > 0)
	{
		CGraphImp* pGraph = pSourceGroup->GetGraph( 0);
		assert(pGraph != NULL);
		pSourceGroup->RemoveAt(pGraph, true);
		AddGraph(pGraph);
	}

	return true;
}


// graph type ����.
bool CVertScaleGroup::ChangeGraphType(const CString& targetGraphName, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetGraphData().GetGraphName() != targetGraphName)
			continue;

		pGraph->SetPacketNames(sourcePacketNames);
		pGraph->GetSubGraphList().SetType(0, sourceType, sourceStyle, sourceDrawStyle);
		return true;
	}

	return false;
}

// subGraph data ����.
bool CVertScaleGroup::ChangeSubGraphData(const CString& targetGraphName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetGraphData().GetGraphName() != targetGraphName)
			continue;

		if(pGraph->GetSubGraphList().ChangeSubGraphData(targetSubGraphName, sourceSubGraphData))
			return true;
	}

	return false;
}

// subGraph ���� ���� ����.
bool CVertScaleGroup::ChangeSubGraphPenThickness(const CString& targetGraphName, const int nSubGraphIndex, const CPenThickness& penThickness)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetGraphData().GetGraphName() != targetGraphName)
			continue;

		// ��� subGraph�� �ٲ۴�.
		if(nSubGraphIndex == -1)
			pGraph->GetSubGraphList().SetAllSubGraphPenThickness(penThickness);
		else
			pGraph->GetSubGraphList().SetPenThickness(nSubGraphIndex, penThickness);
		
		return true;
	}

	return false;
}

// subGraph title ����.
bool CVertScaleGroup::ChangeSubGraphTitle(const CString& orgSubGraphTitle, const CString& changeSubGraphTitle)
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().SetTitle(orgSubGraphTitle, changeSubGraphTitle))
			return true;
	}
	return false;
}

// subGraph �� ����.
bool CVertScaleGroup::ChangeAllHeadSubGraphColor(const CList<CGraphColor, CGraphColor&>& graphColorList, POSITION& colorPos) const
{
	if(colorPos == NULL)
		colorPos = graphColorList.GetHeadPosition();

	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		const CGraphColor& graphColor = graphColorList.GetNext(colorPos);
		if(pGraph->GetSubGraphList().SetSubGraphColor(0, graphColor))
			bResult = true;
	}

	return bResult;
}

// ----------------------------------------------------------------------------------------
// �̹� �����Ǿ� �ִ� graph�� data�� setting.
void CVertScaleGroup::ModifyGraphDataInVertScaleGroup( char *p_szVertScaleGroup, const bool doesMaintainOldScale)
{
	if( !p_szVertScaleGroup) return;
	if( !*p_szVertScaleGroup) return;

	// ���ο� scale ����.
	if( !doesMaintainOldScale)
	{
		char *szGraphData = strstr( p_szVertScaleGroup, "\r\n");
		if( szGraphData)
		{
			*szGraphData = '\0';
			m_vertScale.SetScaleData( p_szVertScaleGroup, m_pMainBlockBaseData->GetParent_Handle());
			*szGraphData = '\r';
		}
	}

	char cNextGraphData = 0;
	char *szGraphData = strstr( p_szVertScaleGroup, "Graph;\r\n");
	char *szNextGraphData = NULL;
	POSITION posGraph = m_GraphList.GetHeadPosition();
	while( szGraphData)
	{
		szNextGraphData = strstr( szGraphData, "GraphEnd;\r\n");
		if( szNextGraphData)
		{
			szNextGraphData += 11;		// "GraphEnd;\r\n"
			cNextGraphData = *szNextGraphData;
			*szNextGraphData = '\0';
		}

		CGraphImp *pGraph = NULL;
		if( posGraph) pGraph = m_GraphList.GetNext( posGraph);
		if( pGraph) ModifyGraph( pGraph, szGraphData);
		else
		{
			posGraph = NULL;
			MakeGraph( szGraphData);
		}

		if( szNextGraphData)
		{
			*szNextGraphData = cNextGraphData;
			szGraphData = strstr( szNextGraphData, "Graph;\r\n");
		}
		else szGraphData = NULL;
	}
}


// -> IndicatorInfo�� gaphData(subGraph����)�� ����.
bool CVertScaleGroup::ChangeAllIndicatorInfoFromGraphData()
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->ChangeIndicatorInfoFromGraphData())
			bResult = true;
	}
	return bResult;
}

BOOL CVertScaleGroup::ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate)
{
	BOOL bResult = FALSE;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->ChangeGraphDataFromIndicatorInfo( p_bWithCalculate ? true : false))
			bResult = TRUE;
	}
	return bResult;
}

// -> �ش� graph�� Map Default IndicatorInfo�� ����
bool CVertScaleGroup::ChangeAllMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->ChangeMapDefaultIndicatorInfo( pIndicatorList))
			bResult = true;
	}
	return bResult;
}

// ----------------------------------------------------------------------------------------
// ���� class �ּ�
void CVertScaleGroup::SetVertScaleGroupList(CVertScaleGroupList* pVertScaleGroupList)
{
	m_pVertScaleGroupList = pVertScaleGroupList;
}

void CVertScaleGroup::SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData)
{
	m_pMainBlockBaseData = pMainBlockBaseData;

	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pGraph->SetMainBlockBaseData(m_pMainBlockBaseData);
	}
}

// ���� scale �� �ٲٱ�.
void CVertScaleGroup::SetVerticalScale(const CVerticalScale& vertScale)
{
	m_vertScale = vertScale;
}

// subGraph�� ���̱� setting.
void CVertScaleGroup::SetAllGraphsHiding(const bool bIsHiding)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().SetAllHiding(bIsHiding))
			bResult = true;
	}

	//������ Min/Max �� �ٽ� ���Ѵ�.
	if(bResult)
		CalculateDisplayDataMinMax();
}

bool CVertScaleGroup::SetSubGraphHiding(const CString& subGraphTitle, const bool bIsHiding)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().SetHiding(subGraphTitle, bIsHiding))
		{
			bResult = true;

			// (2007/3/2 - Seung-Won, Bae) Update Indicator Info.
			pGraph->ChangeIndicatorInfoFromGraphData();
		}
	}

	//������ Min/Max �� �ٽ� ���Ѵ�.
	if(bResult)
		CalculateDisplayDataMinMax();
	return bResult;
}

bool CVertScaleGroup::SetSubGraphHiding(const CString& strGraphName, const int nSubGraphIndex, const bool bIsHiding)
{
	CGraphImp* pGraph = FindGraph(strGraphName);
	if(pGraph == NULL)
		return false;

	if(!pGraph->GetSubGraphList().SetHiding(nSubGraphIndex, bIsHiding))
		return false;

	//����/���Ӱ�� Min/Max �� �ٽ� ���Ѵ�.
	CalculateDisplayDataMinMax();
	return true;
}

// ----------------------------------------------------------------------------------------
// graph�� indicatorinfo�� �ʱ�ȭ(NULL)
void CVertScaleGroup::Initialize_IndicatorInfo(const bool bIsNULL)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pGraph->Initialize_IndicatorInfo(bIsNULL);
	}
}

// graph(subGraph��)�� packet���� �ʱ�ȭ(NULL)
void CVertScaleGroup::Initialize_AllPackets(const bool bIsCalculate)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pGraph->Initialize_AllPackets(bIsCalculate);
	}
}

// strPacketName �� ���Ե� ��� subGraph�� packet���� �ʱ�ȭ
bool CVertScaleGroup::Initialize_Packet(const CString& strPacketName, const bool bIsCalculate)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->IsExistencePacketName(strPacketName)){
			pGraph->Initialize_AllPackets(bIsCalculate);
			bResult = true;
		}
	}
	return bResult;
}

// ��ǥ�� ����Ѵ�.
void CVertScaleGroup::CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pGraph->CalculateGraph(eCalType, false, false, true);
	}

	CalculateDisplayDataMinMax();
}

bool CVertScaleGroup::CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph)
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!pGraph->IsExistencePacketName(packetNameList))
			continue;

		pGraph->CalculateGraph(eCalType, true, false, false);

		bResult = true;
		if(bIsHeadGraph)
			return true;
	}

	return bResult;
}

// vert scale Min/Max �� ����Ѵ�.
// �Լ� ȣ�� ���� => �������� ȣ�� �Ǵ� ���� CVertScaleGroup
//					CGraphImp ���Ͽ��� ȣ�� �Ǵ� ���� CGraphImp
// ��� ���� =>
// 1. ��ǥ ���(TR, RealData) - CVertScaleGroup::CalculateAllGraphs()
//		- CVertScaleGroupList::CalculateGraph()
// 2. Scroll - CMainBlockImp::SetDataRegionInBlock()
// 3. ��ǥ���� dlg ���� �κ� - CGraphImp::CalculateGraph()
// 4. Graph �߰�, ����, ����, �̵� - CVertScaleGroup::AddTail()
// 5. ���� Scale Data ����
void CVertScaleGroup::CalculateDisplayDataMinMax()
{
	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	m_pBaseGraph = FindBaseGraphForDrawingVertScaleData();
	if( m_pBaseGraph)	m_pBasePacket = m_pBaseGraph->GetSubGraphList().GetHeadPacket();
	else				m_pBasePacket = NULL;

	// (2006/11/11 - Seung-Won, Bae) Use Y Scale Min/Max Setting
	if( m_vertScale.GetVertScaleMinMaxType() == CScaleBaseData::VERT_MINMAX_OF_USER)
	{
		// (2006/11/12 - Seung-Won, Bae) Check Min/Max reverse setting.
		if( m_vertScale.GetVertScaleMax() < m_vertScale.GetVertScaleMin())
		{
			double dMin = m_vertScale.GetVertScaleMin();
			m_vertScale.SetVertScaleMin( m_vertScale.GetVertScaleMax());
			m_vertScale.SetVertScaleMax( dMin);
		}

		m_displayDataMinMax.SetDisplayData( m_vertScale.GetVertScaleMin(), m_vertScale.GetVertScaleMax());
	}
	else
	{
		// (2006/10/15 - Seung-Won, Bae) Hide Empty Scale Text (Make Default Scale Range to 0:0)
		//	m_displayDataMinMax.SetDisplayData(-20.0, 20.0);
		//20100515 �̹��� �ּҰ��� �߸� ���ͼ� �������� ���� ���ԵǴ� ���� ����>>
		//m_displayDataMinMax.SetDisplayData( -HUGE_VAL, -HUGE_VAL);
		m_displayDataMinMax.SetDisplayData( HUGE_VAL, -HUGE_VAL);
		//20100515 �̹��� <<

		// <������Ʈ> �� �ְ� <������Ʈ�� min/max> �� ��츸..
		if(!GetDisplayDataMinMaxInOnlyPrice(m_displayDataMinMax))
			m_displayDataMinMax = GetDisplayDataMinMaxInAll();
	}

	// (2006/11/19 - Seung-Won, Bae) Deny Empty VScale
	double dMax = m_displayDataMinMax.GetDisplayMax();
	double dMin = m_displayDataMinMax.GetDisplayMin();
	if( dMax == dMin && m_displayDataMinMax.IsInitialized() && m_pBasePacket)
	{
		CString strType = m_pBasePacket->GetType();
		strType.Replace( "x", "");
		strType.Replace( "X", "");
		strType.Replace( '%', ' ');
		strType.Replace( '$', ' ');
		// (2010/4/17 - Seung-Won, Bae) Check with Graphic Character 'X'
		strType.Replace( "��", "");
		double dGap = atof( strType);
		dMin -= dGap;
		dMax += dGap;
		m_displayDataMinMax.SetDisplayData( dMin, dMax);
	}

	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	if( m_vertScale.GetVertScaleMinMaxType() != CScaleBaseData::VERT_MINMAX_OF_USER && dMax != dMin && m_displayDataMinMax.IsInitialized())
	{
		int nUpSpace = 0, nDownSpace = 0;
		m_vertScale.GetSpace( nUpSpace, nDownSpace);
		if( 0 != nUpSpace || 0 != nDownSpace)
		{
			double dHeight = dMax - dMin;

			// (2007/7/18 - Seung-Won, Bae) Do not Support Down Space for Vertical BarChart (0-Base BarChart)
			CGraphSearchInfo_VisibleTypeStyleDrawStyle gsi;
			gsi.m_eGraphType = CGraphBaseData::Bar_Type;
			gsi.m_nGraphStyle = CGraphBaseData::VerticalBar;
			gsi.m_nDrawStyle = CGraphBaseData::Bar_Draw;
			if( FindGraph( &gsi)) nDownSpace = 0;

			double nHeight = 100 - nUpSpace - nDownSpace;
			dMax += ( dHeight * nUpSpace) / nHeight;
			dMin -= ( dHeight * nDownSpace) / nHeight;

			m_displayDataMinMax.SetDisplayData( dMin, dMax);
		}
	}

	// (2006/11/12 - Seung-Won, Bae) Update Min/Max Scale Setting Value
	m_vertScale.SetVertScaleMin( dMin);
	m_vertScale.SetVertScaleMax( dMax);
}

// ----------------------------------------------------------------------------------------
// ���� class �ּ�
CMainChartFormulate* CVertScaleGroup::GetMainChartFormulate() const
{
	if( !m_pMainBlockBaseData) return NULL;
	return m_pMainBlockBaseData->GetMainAttributes().GetMainChartFormulate();
}

CBlockImp* CVertScaleGroup::GetBlock() const
{
	if(m_pVertScaleGroupList == NULL)
		return NULL;
	return m_pVertScaleGroupList->GetBlock();
}

CVertScaleGroupList* CVertScaleGroup::GetVertScaleGroupList() const
{
	return m_pVertScaleGroupList;
}

// ���� scale
CVerticalScale& CVertScaleGroup::GetVertScale()
{
	return m_vertScale;
}

// ���� scale
const CVerticalScale& CVertScaleGroup::GetVertScale() const
{
	return m_vertScale;
}

// ��� graph�� ��� data
void CVertScaleGroup::GetVertScaleGroupAllData( CStringList &p_slVertScaleGroupData) const
{
	//>> JS.Kim_20100929  solomon.. PAIR RQ�� �������� �ʵ��� ����..
	/*
	p_slVertScaleGroupData.AddTail( "VertScaleGroup;\r\n");

	p_slVertScaleGroupData.AddTail( CString());
	CString &strVertScaleData = p_slVertScaleGroupData.GetTail();
	m_vertScale.GetScaleAllData( strVertScaleData);


	POSITION pos = m_GraphList.GetHeadPosition();
	while( pos)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert( pGraph != NULL);
		pGraph->GetStringGraphData( p_slVertScaleGroupData);
	}
	
	p_slVertScaleGroupData.AddTail( "VertScaleGroupEnd;\r\n");
	*/
	CStringList slVertScaleGroupData;
	POSITION pos = m_GraphList.GetHeadPosition();
	while( pos)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert( pGraph != NULL);
		pGraph->GetStringGraphData( slVertScaleGroupData);
	}
	if( slVertScaleGroupData.IsEmpty() )		return;

	p_slVertScaleGroupData.AddTail( "VertScaleGroup;\r\n");

	p_slVertScaleGroupData.AddTail( CString());
	CString &strVertScaleData = p_slVertScaleGroupData.GetTail();
	m_vertScale.GetScaleAllData( strVertScaleData);

	p_slVertScaleGroupData.AddTail(&slVertScaleGroupData);
	
	p_slVertScaleGroupData.AddTail( "VertScaleGroupEnd;\r\n");
	//<<
}

bool CVertScaleGroup::GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		CString strIndicatorName = pGraph->GetGraphData().GetIndicatorName();
		if( strIndicatorName.IsEmpty())
			continue;

		strIndicatorNameList.AddTail( strIndicatorName);
		bResult = true;
	}
	return bResult;
}

bool CVertScaleGroup::GetAllGraphName(CList<CString, CString>& strGraphNameList) const
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		CString strGraphName = pGraph->GetGraphData().GetGraphName();
		if( strGraphName.IsEmpty())
			continue;

		strGraphNameList.AddTail( strGraphName);
		bResult = true;
	}
	return bResult;
}

// ----------------------------------------------------------------------------------------
// graph ����
int CVertScaleGroup::GetGraphCount() const
{
	return m_GraphList.GetCount();
}

// ��� graph �Լ���
CString CVertScaleGroup::GetGraphsGraphName() const
{
	CString strData;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		CString strGraphName = pGraph->GetGraphData().GetGraphName();
		if(!strGraphName.IsEmpty())
			strData += (strGraphName + ';');
	}
	return strData;
}

// graph ��
// ��� block�� �ִ� ��� graph�� head subGraph color ��������.
bool CVertScaleGroup::GetAllHeadSubGraphColor(CList<CGraphColor, CGraphColor&>& graphColorList) const
{
	bool bResult = false;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		CGraphColor* pGraphColor = pGraph->GetSubGraphList().GetGraphColor(0);
		if(pGraphColor == NULL)
			continue;

		graphColorList.AddTail(*pGraphColor);
		bResult = true;
	}

	return bResult;
}

COLORREF CVertScaleGroup::GetGraphColor1(const int nGraphIndex, const int nSubGraphIndex) const
{
	CGraphImp* pGraph = GetGraph(nGraphIndex);
	if(pGraph == NULL)
		return RGB(255, 255, 255);

	return pGraph->GetSubGraphList().GetGraphColor1(nSubGraphIndex);
}

// subGraph title region
// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
CRect CVertScaleGroup::GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition, int& nPreLineCount) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
		CRect region = pGraph->GetSubGraphList().GetSubGraphTitleRegion(pDC, pSubGraph, graphRegion, preTitleEndPosition, nPreLineCount);
		if(!region.IsRectEmpty())
			return region;
	}

	return CRect(0, 0, 0, 0);
}

// ----------------------------------------------------------------------------------------
// ���� scale�� ���� ���Ѵ�.
// -> ���� block�ȿ� �ִ� graph���� ���� scale�� ������ �ִ���̷� �Ѵ�.
// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
void CVertScaleGroup::GetLeftVertScaleRegionWidth(CDC* pDC, int &p_nLeftWidth, BOOL p_bIsDualScale) const
{
	// ���� scale�� width �������� ����
	// scale ���� ������ scale �� �׸��� �ʴ´�.
	if( m_pMainBlockBaseData == NULL || m_vertScale.GetScaleData().IsEmpty()) return;

	//scale position = ����, ������ ��쿡��.
	if( m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_LEFT
		&& m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_BOTH) return;

	CVertScaleMaxWidthCalculatorData calData(false, m_pMainBlockBaseData->GetDisplayAttributes(), m_vertScale, p_bIsDualScale);
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		int nTemp = pGraph->GetLeftVertScaleRegionWidth(pDC, &calData);
		if( nTemp > p_nLeftWidth) p_nLeftWidth = nTemp;
	}
}
void CVertScaleGroup::GetRightVertScaleRegionWidth( CDC* pDC, int &p_nRightWidth, BOOL p_bIsDualScale) const
{
	// scale ���� ������ scale �� �׸��� �ʴ´�.
	if( m_pMainBlockBaseData == NULL || m_vertScale.GetScaleData().IsEmpty()) return;

	//scale position = ������, ������ ��쿡��.
	if( m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_RIGHT 
		&& m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_BOTH) return;

	CVertScaleMaxWidthCalculatorData calData(true, m_pMainBlockBaseData->GetDisplayAttributes(), m_vertScale, p_bIsDualScale);
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		int nTemp = pGraph->GetRightVertScaleRegionWidth(pDC, &calData);
		if(nTemp > p_nRightWidth) p_nRightWidth = nTemp;
	}
}

// �ش� RQ�� �۾� : �������� - ojtaso (20070420)
void CVertScaleGroup::GetSubGraphViewDataList(LPCTSTR lpszRQ, CSubGraphViewDataList& subGraphViewDataList) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);

		if( lpszRQ) if( *lpszRQ) if( ::lstrcmp( pGraph->GetGraphRQ(), lpszRQ))
			continue;
		pGraph->GetSubGraphViewDataList(subGraphViewDataList);
	}
}

// ----------------------------------------------------------------------------------------
// ���� ���̴� ȭ�鿡���� min, max�� ���Ѵ�. ==> y���� ��
CDisplayDataMinMax CVertScaleGroup::GetDisplayDataMinMax() const
{
	return m_displayDataMinMax;
}

// ----------------------------------------------------------------------------------------
// graph title name�� ��� empty���� ���ϱ�
bool CVertScaleGroup::IsViewableGraphTitleEmpty() const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!pGraph->GetSubGraphList().IsViewableSubGraphTitleEmpty())
			return false;
	}
	return true;
}

// �̹� �����ϴ� graph���� üũ
bool CVertScaleGroup::IsExistenceGraph( const CString &strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if( pGraph->GetGraphData().GetGraphName() == strGraphName) return true;
	}
	return false;
}

// RQ�� ������ �ִ� Graph üũ : �������� - ojtaso (20061224)
bool CVertScaleGroup::IsExistenceGraph(LPCTSTR lpszRQ, const CString &strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!::lstrcmp(lpszRQ, pGraph->GetGraphRQ()) && 
			pGraph->GetGraphData().GetGraphName() == strGraphName) return true;
	}
	return false;
}

// CPacketRQ*�� Graph üũ : �������� - ojtaso (20070109)
bool CVertScaleGroup::IsExistenceGraph(LPVOID pPacketRQ, const CString &strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->ComparePacketRQ(pPacketRQ) && 
			pGraph->GetGraphData().GetGraphName() == strGraphName) return true;
	}
	return false;
}

// �̹� �����ϴ� graph���� üũ
bool CVertScaleGroup::IsExistenceIndicator( const CString &strIndicatorName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if( pGraph->GetGraphData().GetIndicatorName() == strIndicatorName) return true;
	}
	return false;
}

// RQ�� ������ �ִ� Graph üũ : �������� - ojtaso (20061224)
bool CVertScaleGroup::IsExistenceIndicator(LPCTSTR lpszRQ, const CString &strIndicatorName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!::lstrcmp(lpszRQ, pGraph->GetGraphRQ()) && 
			pGraph->GetGraphData().GetIndicatorName() == strIndicatorName) return true;
	}
	return false;
}

// �ش� graph Type�� �����ϴ��� üũ
bool CVertScaleGroup::IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().IsViewableGraphType(eGraphType))
			return true;
	}
	return false;
}

// �ش� graph style�� �����ϴ��� üũ
bool CVertScaleGroup::IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().IsViewableGraphStyle(eGraphType, nGraphStyle))
			return true;
	}
	return false;
}

// �ش� graph draw style�� �����ϴ��� üũ
bool CVertScaleGroup::IsViewableGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nDrawStyle) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().IsViewableGraphDrawStyle(eGraphType, nGraphStyle, nDrawStyle))
			return true;
	}
	return false;
}

// ��� graph �� hide ���� üũ
bool CVertScaleGroup::DoesAreHidingAllGraphs() const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!pGraph->GetSubGraphList().AreHidingAllSubGraphs())
			return false;
	}
	return true;
}

// ----------------------------------------------------------------------------------------
// exceptionGraph �� �����ϰ� ���� indicatorInfo�� ����ϴ��� ����
bool CVertScaleGroup::DoesUseEqualIndicatorInfo(const CIndicatorInfo* pIndiInfo, const CGraphImp* pExceptionGraph) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph == pExceptionGraph)
			continue;
		if(pGraph->GetIndicatorInfo() == pIndiInfo)
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------------------
// ���� scale �׸���.
void CVertScaleGroup::DrawVertScale(CDC* pDC, CDrawingVertScaleData* pDrawingVertScaleData, const CDrawingRegion& drawingRegion)
{
	if(!IsVertScaleDrawing() || pDrawingVertScaleData == NULL) return;

	// (2008/12/19 - Seung-Won, Bae) Calc it in VScale Draw for Including BaseLine.
	//real data ó�������� Min/Max�� ����Ѵ�.
	CalculateDisplayDataMinMax();

	// (2006/11/19 - Seung-Won, Bae) Deny to draw of Not-Initialized Scale.
	if( !m_displayDataMinMax.IsInitialized()) return;

	SetDrawingVertScaleData(pDrawingVertScaleData);
	CBlockScaleDrawer().DrawVertScale(pDC, pDrawingVertScaleData, drawingRegion.GetVertScaleRegion(), m_vertScale, m_pMainBlockBaseData->GetParent_Handle(), m_pMainBlockBaseData->GetMainAttributes().GetChartMode());
	InitializeDrawingVertScaleData(pDrawingVertScaleData);
}

// graph �׸���.
// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
void CVertScaleGroup::DrawGraphs(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& preGraphTitleEndPosition, int& nEndLimit, int& nGraphTitleEndLine)
{
	if( pDrawingGraphData == NULL) return;

	int nPreGraphTitleEndLine = 1;

	// ���õ� ��Ʈ�� ������ ������Ʈ�� �ٽ� �׸� - ojtaso (20080104)
	if(!m_lpSelectedGraph)
		m_lpSelectedGraph = FindGraph( _MTEXT( C2_PRICE_CHART));

	int nThread_Flag = 0;

	// (2009/2/20 - Seung-Won, Bae) check no graph region
	CMainBlockImp *pMainBlock = m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
	CXScaleManager *pXScaleManager = pMainBlock->GetXScaleManager();
	BOOL bNoRegion = ( pXScaleManager->GetScreenWidth( GetVertScaleGroupList()->GetBlock()->GetBlocksColumn()->GetColumnIndex()) <= 0);

	//NH-alzioyes.100616 ���Ź��� ������θ� �׷�������.
	bool bExtraDraw = false;
	if( !bNoRegion) 
	{
		CString szIndiName;
		if(nThread_Flag != THREAD_DRAWING)   //2009.01.06 �輺�� - �׷��� �׸��ÿ��� ����. �ι� �׸��� ���� ����.
		{
			if(m_lpSelectedGraph && m_lpSelectedGraph->GetIndicatorInfo())
			{
				szIndiName = m_lpSelectedGraph->GetIndicatorInfo()->GetIndicatorName();
				if(szIndiName.Compare(_MTEXT( C2_OVERHANGING_SUPPLY))==0)
				{
					pDrawingGraphData->SetDisplayDataVertMinMax(m_displayDataMinMax);
					m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
					bExtraDraw = true;
				}
			}
		}
	}

	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		pDrawingGraphData->SetDisplayDataVertMinMax(m_displayDataMinMax);
		// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
		
		nThread_Flag = pMainBlock->GetXScaleManager()->GetThreadFlag();
		
		// (2009/1/7 - Seung-Won, Bae) Draw all graph title on drawing-thread.
		if( nThread_Flag == THREAD_DRAWING)
			pGraph->DrawGraphTitle( pDC, pDrawingGraphData, preGraphTitleEndPosition, nEndLimit, nGraphTitleEndLine);

		// (2009/1/7 - Seung-Won, Bae) Do not call the selected graph on drawing. it will be done at last.
		if( !bNoRegion) if( pGraph != m_lpSelectedGraph || nThread_Flag != THREAD_DRAWING)
		{
			// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
			if( THREAD_CALCULATION != nThread_Flag || 1 != pXScaleManager->GetCalculationMode())
				pGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
			else
			{
				pMainBlock->m_nThreadCount++;
				pGraph->m_pDC = pDC;
				pGraph->m_pDrawingGraphData = pDrawingGraphData;
				//20100506 �̹��� ���������ǽ� ������ ����ߴ� >>
				//AfxBeginThread( DrawGraphAndSignal, pGraph);
				DrawGraphAndSignal( pGraph );

			}
		}

//>> Ÿ��Ʋ ������ ǥ�ý� ������ ������ ������ �ʵ��� ó�� - vntsorl (20090106)
		if(nEndLimit <= preGraphTitleEndPosition + MARGIN * 2)
//<< Ÿ��Ʋ ������ ǥ�ý� ������ ������ ������ �ʵ��� ó�� - vntsorl (20090106)
		{
			preGraphTitleEndPosition = VSGL_TITLE_START_POSITION;
			nGraphTitleEndLine++;
		}

		if(nGraphTitleEndLine > nPreGraphTitleEndLine)
			m_pVertScaleGroupList->RecalculateTitleRegion(TRUE);

		nPreGraphTitleEndLine = nGraphTitleEndLine;
	}	

	// ���õ� �׷��� �ٽ� �׸� (20071007)
	//NH-alzioyes.100616 ���Ź��� ������θ� �׷�������.
	if( !bNoRegion) 
	{
		if(bExtraDraw)
		{
			CString szIndiName;
			if(nThread_Flag == THREAD_DRAWING)
			{
				if(m_lpSelectedGraph && m_lpSelectedGraph->GetIndicatorInfo())
				{
					szIndiName = m_lpSelectedGraph->GetIndicatorInfo()->GetIndicatorName();
					if(szIndiName.Compare(_MTEXT( C2_OVERHANGING_SUPPLY))!=0)
						m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
				}
			}
		}
		else
		{
			if(nThread_Flag == THREAD_DRAWING)   //2009.01.06 �輺�� - �׷��� �׸��ÿ��� ����. �ι� �׸��� ���� ����.
				if(m_lpSelectedGraph)
				m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
		}
	}
// 	if( !bNoRegion) 
// 		if(nThread_Flag == THREAD_DRAWING)   //2009.01.06 �輺�� - �׷��� �׸��ÿ��� ����. �ι� �׸��� ���� ����.
// 			if(m_lpSelectedGraph)
// 				m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);

// 	//NH-alzioyes.100616 ���Ź��� ������θ� �׷�������.
// 	if( !bNoRegion) 
// 	{
// 		CString szIndiName;
// 		if(nThread_Flag == THREAD_DRAWING)
// 		{
// 			if(m_lpSelectedGraph && m_lpSelectedGraph->GetIndicatorInfo())
// 			{
// 				szIndiName = m_lpSelectedGraph->GetIndicatorInfo()->GetIndicatorName();
// 				if(szIndiName.Compare(_MTEXT( C2_OVERHANGING_SUPPLY))!=0)
// 					m_lpSelectedGraph->DrawGraphAndSignal( pDC, pDrawingGraphData);
// 			}
// 		}
// 	}
}

// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
UINT CVertScaleGroup::DrawGraphAndSignal( LPVOID pParam)
{
	CGraphImp *pGraph = ( CGraphImp *)pParam;
	pGraph->DrawGraphAndSignal( pGraph->m_pDC, pGraph->m_pDrawingGraphData);
	pGraph->GetMainBlock()->m_nThreadCount--;
	return 0;
}

// ---------------------------------------------------------------------------------------
// graph ã��
CGraphImp* CVertScaleGroup::FindGraph(const CString& strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetGraphData().GetGraphName() == strGraphName)
			return pGraph;
	}
	return NULL;
}

// RQ�� ������ �ִ� Graph �˻� : �������� - ojtaso (20061224)
CGraphImp* CVertScaleGroup::FindGraph(LPCTSTR lpszRQ, const CString& strGraphName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(!::lstrcmp(lpszRQ, pGraph->GetGraphRQ()) && 
			pGraph->GetGraphData().GetGraphName() == strGraphName)
			return pGraph;
	}
	return NULL;
}
	 
CGraphImp* CVertScaleGroup::GetGraph(const int nIndex) const
{
	POSITION pos = m_GraphList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;

	return m_GraphList.GetAt(pos);
}

CGraphImp* CVertScaleGroup::FindGraph(const CString& strTitleName, int& nSubGraphIndex) const
{
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		int nIndex = pGraph->GetSubGraphList().GetSameTitleSubgraphIndex(strTitleName);
		if(nIndex < 0)
			continue;

		nSubGraphIndex = nIndex;
		return pGraph;
	}

	return NULL;
}

CGraphImp* CVertScaleGroup::FindIndicator( const CString &strIndicatorName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert( pGraph != NULL);
		if(pGraph->GetGraphData().GetIndicatorName() == strIndicatorName)
			return pGraph;
	}
	return NULL;
}

// RQ�� ������ �ִ� Graph �˻� : �������� - ojtaso (20061224)
CGraphImp* CVertScaleGroup::FindIndicator(LPCTSTR lpszRQ, const CString &strIndicatorName) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert( pGraph != NULL);
		if(!::lstrcmp(lpszRQ, pGraph->GetGraphRQ()) && 
			pGraph->GetGraphData().GetIndicatorName() == strIndicatorName)
			return pGraph;
	}
	return NULL;
}
	 
// subGraph Title�� �̿��Ͽ� subGraphã��.
// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
CSubGraph* CVertScaleGroup::FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, CRect& titleRegion, int& preTitleEndPosition, int& nPreLineCount) const
{
	POSITION pos = m_GraphList.GetHeadPosition();

	while(pos != NULL){
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
		CSubGraph* pSubGraph = pGraph->GetSubGraphList().FindSubGraphTitle(pDC, allTitleRegion, point, titleRegion, preTitleEndPosition, nPreLineCount);
		if(pSubGraph != NULL)
 		{
			// �׷���/Ÿ��Ʋ ���� �� RQ�˷��� : �������� - ojtaso (20070106)
			if(GetKeyState(VK_LBUTTON) < 0)
			{
				IChartCtrl* pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();

				LPCTSTR lpszRQ = pGraph->GetGraphRQ();
				pChartCtrl->SelectedGraph(lpszRQ);
				
				// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
				const_cast<CGraphImp*>(m_lpSelectedGraph) = pGraph;
			}
   			return pSubGraph;
   		}
	}

	return NULL;
}

// subGraph ã��.
bool CVertScaleGroup::FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const
{
	// (2007/2/1 - Seung-Won, Bae) 
	if( !m_displayDataMinMax.IsInitialized()) return false;

	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL){
		CGraphImp* pGraph = (CGraphImp*) m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if(pGraph->GetSubGraphList().FindSubGraph(point, selectedSubGraphDeque))
		{
 			// �׷���/Ÿ��Ʋ ���� �� RQ�˷��� : �������� - ojtaso (20070106)
 			if(GetKeyState(VK_LBUTTON) < 0)
 			{
 				IChartCtrl* pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
 				pChartCtrl->SelectedGraph(pGraph->GetGraphRQ());

				// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
				const_cast<CGraphImp*>(m_lpSelectedGraph) = pGraph;
 			}
 			return true;
		}
	}
	return false;
}

// private ===============================================================================
// CGraphImp�� �����.
CGraphImp* CVertScaleGroup::MakeNewGraph( char *p_szGraphData)
{
	if( !p_szGraphData) return NULL;
	if( !*p_szGraphData) return NULL;

	char *szGraphData = strstr( p_szGraphData, "Graph;\r\n");
	if( !szGraphData) return NULL;
	szGraphData += 8;	// "Graph;\r\n"

	char *szGraphDataEnd = strstr( szGraphData, "GraphEnd;\r\n");
	if( szGraphDataEnd) *szGraphDataEnd = '\0';

	if( !*szGraphData)
	{
		if( szGraphDataEnd) *szGraphDataEnd = 'G';
		return NULL;
	}


	char *szSubGraphData = strstr( szGraphData, "\r\n");
	if( szSubGraphData) *szSubGraphData = '\0';

	// �������� ���� : �������� - ojtaso (20070507)
	char *szRQ = strstr( szGraphData, "RQ:");  

	CGraphData graphData( szGraphData, m_pMainBlockBaseData->GetParent_Handle());
	if( szSubGraphData)
	{
		*szSubGraphData = '\r';
		szSubGraphData += 2;		// "\r\n"
	}

	// (2006/12/10 - Seung-Won, Bae) Change Graph Name for Multiple Supporting of Same Indicator.
	//		Caution. It must be processed before creation of Graph.
	//				Because the Creation of Graph makes the Indicator Info and bind it with the Graph.
	if( m_pMainBlockBaseData)
	{
		CMainBlockImp *pMainBlock = m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
		CString strGraphName = graphData.GetGraphName();
		CIndicatorList *pIndicatorList = m_pMainBlockBaseData->GetMainAttributes().GetIndicatorList();
		if( pIndicatorList)
		{
			// (2007/1/12 - Seung-Won, Bae) Make Unique Graph Name
			if( strGraphName.IsEmpty())
			{
				CIndicatorInfo *pIndicatorInfo = NULL;
				do
				{
					strGraphName.Format( "#%04X", rand());
					pIndicatorInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
				} while( pIndicatorInfo);
				( ( CGraphDataBase *)&graphData)->SetGraphName( strGraphName);
			}
			else if( IsExistenceGraph( strGraphName)						// Check Current VScale Group (It may be not added in Block, because in creation.)
						|| pMainBlock->IsExistenceGraph( strGraphName))
			{
				CIndicatorInfo *pIndicatorInfo = pIndicatorList->CreateGraphIndicatorInfo( graphData.GetIndicatorName());
				if( pIndicatorInfo) ( ( CGraphDataBase *)&graphData)->SetGraphName( pIndicatorInfo->GetGraphName());
			}
		}
	}

	// �������� ���� : �������� - ojtaso (20070507)
	CGraphImp *pGraph = NULL;
	if(szRQ)
	{
		szRQ += 3;
		char *szRQEnd = strstr( szRQ, "\r\n");
		if(szRQEnd)
			*szRQEnd = '\0';

		LPVOID pPacketRQ = m_pMainBlockBaseData->GetMainAttributes().GetPacketListManager()->GetPacketRQ(szRQ);
		if(pPacketRQ)
		{
			pGraph = CGraphImp::MakeGraph(pPacketRQ, m_pMainBlockBaseData, this, graphData, szSubGraphData);
			m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->NotifyRQStatusToAddin( CCommonInterfaceEnum::PACKETLISTMANAGER_NEW_RQ, szRQ);
		}
		else
			pGraph = CGraphImp::MakeGraph(m_pMainBlockBaseData, this, graphData, szSubGraphData);
	}
	else
	{
		// Packet Name �ٽ� ���� : �������� - ojtaso (20070531)
		CPacketListManager* lpPacketListManager = m_pMainBlockBaseData->GetMainAttributes().GetPacketListManager();

		LPVOID pPacketRQ = lpPacketListManager->GetPacketRQ( m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->GetCurrentRQ());
		if( !pPacketRQ)
		{
			if( szGraphDataEnd) *szGraphDataEnd = 'G';
			return NULL;
		}
		pGraph = CGraphImp::MakeGraph(pPacketRQ, m_pMainBlockBaseData, this, graphData, szSubGraphData);
	}

	if( szGraphDataEnd) *szGraphDataEnd = 'G';

	return pGraph;
}

// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
CGraphImp* CVertScaleGroup::MakeNewGraph(LPVOID pPacketRQ, char *p_szGraphData)
{
	if( !p_szGraphData) return NULL;
	if( !*p_szGraphData) return NULL;

	char *szGraphData = strstr( p_szGraphData, "Graph;\r\n");
	if( !szGraphData) return NULL;
	szGraphData += 8;	// "Graph;\r\n"

	char *szGraphDataEnd = strstr( szGraphData, "GraphEnd;\r\n");
	if( szGraphDataEnd) *szGraphDataEnd = '\0';

	if( !*szGraphData)
	{
		if( szGraphDataEnd) *szGraphDataEnd = 'G';
		return NULL;
	}

	char *szSubGraphData = strstr( szGraphData, "\r\n");
	if( szSubGraphData) *szSubGraphData = '\0';

	// �������� ���� : �������� - ojtaso (20070507)
	char *szRQ = strstr( szGraphData, "RQ:");  

	CGraphData graphData( szGraphData, m_pMainBlockBaseData->GetParent_Handle());
	if( szSubGraphData)
	{
		*szSubGraphData = '\r';
		szSubGraphData += 2;		// "\r\n"
	}


	// (2006/12/10 - Seung-Won, Bae) Change Graph Name for Multiple Supporting of Same Indicator.
	//		Caution. It must be processed before creation of Graph.
	//				Because the Creation of Graph makes the Indicator Info and bind it with the Graph.
	if( m_pMainBlockBaseData)
	{
		CMainBlockImp *pMainBlock = m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
		CString strGrpahName = graphData.GetGraphName();
		if( IsExistenceGraph(pPacketRQ, strGrpahName)					// Check Current VScale Group (It may be not added in Block, because in creation.)
			|| pMainBlock->IsExistenceGraph( strGrpahName))
		{
			CIndicatorList *pIndicatorList = m_pMainBlockBaseData->GetMainAttributes().GetIndicatorList();
			if( pIndicatorList)
			{
				CIndicatorInfo *pIndicatorInfo = pIndicatorList->CreateGraphIndicatorInfo( graphData.GetIndicatorName());
				if( pIndicatorInfo) ( ( CGraphDataBase *)&graphData)->SetGraphName( pIndicatorInfo->GetGraphName());
			}
		}
	}

	// �������� �ҷ����� : �������� - ojtaso (20070507)
	CGraphImp *pGraph = NULL;
	if(szRQ)
	{
		szRQ += 3;
		char *szRQEnd = strstr( szRQ, "\r\n");
		if(szRQEnd)
		{
			*szRQEnd = '\0';
			
			pGraph = CGraphImp::MakeGraph(m_pMainBlockBaseData, this, graphData, szSubGraphData);
			pGraph->SetGraphRQ(szRQ);

			*szRQEnd = '\r';
		}
	}
	else
		pGraph = CGraphImp::MakeGraph(pPacketRQ, m_pMainBlockBaseData, this, graphData, szSubGraphData);

//	CGraphImp *pGraph = CGraphImp::MakeGraph(pPacketRQ, m_pMainBlockBaseData, this, graphData, szSubGraphData);
	if( szGraphDataEnd) *szGraphDataEnd = 'G';

	return pGraph;
}

// ������ �����ϴ� graph�� ������ ��� �ڽ��� ����.
bool CVertScaleGroup::DeleteEmptyGroup()
{
	if( !m_GraphList.IsEmpty() || m_pVertScaleGroupList == NULL || m_bOnDestructor) return false;
	return m_pVertScaleGroupList->DeleteVertScaleGroup( this, true);
}

// graph�� ���Ͽ� packet�� �����Ѵ�.
void CVertScaleGroup::DeleteGraph(CGraphImp* pDeleteGraph, CGraphImp* pSourceGraph)
{
	bool bSamePacket = false;
	if(pSourceGraph != NULL)
		bSamePacket = pSourceGraph->GetSubGraphList().IsExistenceSamePacketInGraph(pDeleteGraph);

	if(bSamePacket)
		DeleteGraphNoDeletePacket(pDeleteGraph);
	else
		DeleteGraphWithNotify(pDeleteGraph);
}

// ���� ���õ� ��ü�� �����Ѵ�.
void CVertScaleGroup::DeleteGraphNoDeletePacket(CGraphImp* pGraph)
{
	assert(pGraph != NULL);
	POSITION pos = m_GraphList.Find(pGraph);

	// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
	if(m_lpSelectedGraph == pGraph)
		m_lpSelectedGraph = NULL;

	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	CGraphImp::Delete(pGraph);
	if(pos != NULL)
		m_GraphList.RemoveAt(pos);
}

// �� graph�� �ٲ۴�.
void CVertScaleGroup::ChangeGraph(CGraphImp* pSourceGraph, CGraphImp* pTargetGraph)
{
	assert(pSourceGraph != NULL && pTargetGraph != NULL);

	POSITION pos = m_GraphList.Find(pTargetGraph);
	m_GraphList.InsertBefore(pos, pSourceGraph);
	//sy test
	// �ΰ��� graph�� packet�� ���� ���.
	//DeleteGraph(pTargetGraph);
	DeleteGraph(pTargetGraph, pSourceGraph);
}


// ---------------------------------------------------------------------------------------
// �̹� �����Ǿ� �ִ� graph�� data�� setting.
void CVertScaleGroup::ModifyGraph( CGraphImp* pGraph, char *p_szGraphData)
{
	CGraphImp* pNewGraph = MakeNewGraph( p_szGraphData);
	// �ٲ� ������ ���� ��쿣 graph�� list���� �����Ѵ�.
	if( !pNewGraph)
	{
		DeleteGraphWithNotify( pGraph);
		return;
	}

	// ���� ���õ� pGraph�� ���ο� graph�� �ٲ۴�.
	// -> pGraph�� ���� �ȴ�.
	ChangeGraph(pNewGraph, pGraph);
}

// ----------------------------------------------------------------------------------------
// ���� VertScaleGroup ���� ������ �ƴ� ��� Graph�� min/max ���ϱ�
CDisplayDataMinMax CVertScaleGroup::GetDisplayDataMinMaxInAll() const
{
	//sy test
	CDisplayDataMinMax displayDataMinMax, gapData;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);

		// (2007/4/23 - Seung-Won, Bae) Check Hide Graph.
		if( pGraph->IsGraphHide()) continue;

		GetDisplayDataMinMax(pGraph, displayDataMinMax);
	}

	return displayDataMinMax;
}

// <������Ʈ> ���� min/max ���ϱ�
bool CVertScaleGroup::GetDisplayDataMinMaxInOnlyPrice(CDisplayDataMinMax& displayDataMinMax) const
{
	CGraphImp* pGraph = FindGraph( _MTEXT( C2_PRICE_CHART));
	if(pGraph == NULL) return false;

	bool bIsOnlyPriceVertScale = DoesOnlyPriceVertScale();
	if(!bIsOnlyPriceVertScale) return false;

	GetDisplayDataMinMax(pGraph, displayDataMinMax);
	return true;
}

// �ش� graph�� min, max
void CVertScaleGroup::GetDisplayDataMinMax(CGraphImp* pGraph, CDisplayDataMinMax& displayDataMinMax) const
{
	CDisplayDataMinMax temp;
	// (2006/11/11 - Seung-Won, Bae) Use Y Scale Min/Max Setting
	if(!pGraph->GetDisplayDataMinMax(m_vertScale.GetScaleData(), temp, m_vertScale.GetVertScaleMinMaxType() == CScaleBaseData::VERT_MINMAX_OF_FULL))
		return;

	// (2006/11/19 - Seung-Won, Bae) Support Initizling State.
	if( displayDataMinMax.IsInitialized())
	{
		double dMin = displayDataMinMax.GetDisplayMin();
		double dMax = displayDataMinMax.GetDisplayMax();
		if(temp.GetDisplayMin() < dMin) dMin = temp.GetDisplayMin();
		if(temp.GetDisplayMax() > dMax) dMax = temp.GetDisplayMax();
		displayDataMinMax.SetDisplayData( dMin, dMax);
	}
	else displayDataMinMax = temp;
}

// ----------------------------------------------------------------------------------------
// draw �ϱ� ���� �� ����
void CVertScaleGroup::SetDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData)
{
	if(pDrawingVertScaleData == NULL || m_GraphList.GetCount() <= 0)
		return;

	// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
	if( !m_pBaseGraph && !m_pBasePacket) return;

	pDrawingVertScaleData->SetDisplayDataMinMax(	m_displayDataMinMax);
	pDrawingVertScaleData->SetIndicatorName(		m_pBaseGraph->GetGraphData().GetIndicatorName());
	pDrawingVertScaleData->SetSubGraphNames(		m_pBaseGraph->GetSubGraphList().GetViewableSubGraphNames());
	pDrawingVertScaleData->SetPacketNames(			m_pBaseGraph->GetGraphData().GetPacketNames());
	pDrawingVertScaleData->SetIndicatorInfo(		m_pBaseGraph->GetIndicatorInfo());
	pDrawingVertScaleData->SetPacket(				m_pBasePacket);

	if( pDrawingVertScaleData->GetScaleDrawerType() == CScaleBaseData::VERT_NOTYPE){
		// <����>�� <��������������> �� ��� -> ScaleDrawerType : <+���Ͻ�>
		if( m_pBaseGraph->GetSubGraphList().IsViewableGraphStyle(CGraphBaseData::Bar_Type, CGraphBaseData::FixedRateCombBar, 0))
			pDrawingVertScaleData->SetScaleDrawerType(CScaleBaseData::VERT_TOPBUTTOM_PLUS);
	}
}

void CVertScaleGroup::InitializeDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData)
{
	// VERT_LOG �� �ƴ� ��츸 �ʱ�ȭ�Ѵ�.
	if(pDrawingVertScaleData->GetScaleDrawerType() == CScaleBaseData::VERT_LOG)
		return;

	pDrawingVertScaleData->SetScaleDrawerType(CScaleBaseData::VERT_NOTYPE);
}

// ----------------------------------------------------------------------------------------
// scale�� �׸����� ����
bool CVertScaleGroup::IsVertScaleDrawing() const
{
	if( m_vertScale.GetScaleData().IsEmpty() || m_GraphList.GetCount() <= 0)
		return false;

	return (m_vertScale.GetVertScalePosition() != CScaleBaseData::VERT_NONE && m_vertScale.GetActiveFlag());
}

// <������Ʈ> ���� scale ���� ����
bool CVertScaleGroup::DoesOnlyPriceVertScale() const
{
	if(m_pMainBlockBaseData == NULL)
		return false;

	return m_pMainBlockBaseData->GetScaleAttributes().DoesOnlyPriceVertScale();
}

// ----------------------------------------------------------------------------
// vertScale�� �׸��� ���� �ʿ��� data(CDrawingVertScaleData)�� 
// ���ϱ� ���� ������ �Ǵ� graph ã��.
CGraphImp* CVertScaleGroup::FindBaseGraphForDrawingVertScaleData() const
{
	if( m_GraphList.IsEmpty()) return NULL;

	// �⺻������ head�� �ִ� graph.
	// (2007/3/11 - Seung-Won, Bae) Retrieve the Visible Graph first.
	CGraphImp* pBaseGraph = NULL;
	CGraphSearchInfo_VisibleSubGraph gsi;
	if( FindGraph( &gsi)) pBaseGraph = gsi.m_pGraphImp;
	else pBaseGraph = m_GraphList.GetHead();
	if( pBaseGraph == NULL) return NULL;
	
	CString strGraphName = pBaseGraph->GetGraphData().GetIndicatorName();
	// "������Ʈ" group. -> "������Ʈ"
	// (��, "������Ʈ"�� ������ vertScaleGroup �̾�� �Ѵ�.!)
	if(strGraphName != _MTEXT( C2_PRICE_CHART) && 
		g_iMetaTable.IsScaleGroup_Price(strGraphName))
	{
		CGraphImp* pGraph = FindGraph( _MTEXT( C2_PRICE_CHART));
		if(pGraph != NULL)
			return pGraph;
	}
	// "�ŷ�����Ʈ" group. -> "�ŷ��� ��Ʈ"
	else if(strGraphName != _MTEXT( C2_VOLUME_CHART) && 
		g_iMetaTable.IsScaleGroup_Volume(strGraphName))
	{
		CGraphImp* pGraph = FindGraph( _MTEXT( C2_VOLUME_CHART));
		if(pGraph != NULL)
			return pGraph;
	}

	return pBaseGraph;
}

//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Get Data Info of Graph
//////////////////////////////////////////////////////////////////////

// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
BOOL CVertScaleGroup::GetIndexOfGraph( const char *p_szGraphName, int &p_nG)
{
	p_nG = -1;
	if( !p_szGraphName) return FALSE;
	if( !*p_szGraphName) return FALSE;

	p_nG = 0;
	POSITION pos = m_GraphList.GetHeadPosition();
	while(pos != NULL)
	{
		CGraphImp* pGraph = m_GraphList.GetNext(pos);
		assert(pGraph != NULL);
		if( pGraph->GetGraphData().GetGraphName() == p_szGraphName) return TRUE;
		p_nG++;
	}

	p_nG = -1;
	return FALSE;
}

// Change Input Packet Names and Calculate again.
BOOL CVertScaleGroup::ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames)
{
	if( !p_szIndicatorName || !p_szPacketNames) return FALSE;
	if( !*p_szIndicatorName || !*p_szPacketNames) return FALSE;

	BOOL bResult = FALSE;
	CGraphImp *pGraph = NULL;
	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION pos = m_GraphList.GetHeadPosition();
	while( pos)
	{
		pGraph = m_GraphList.GetNext( pos);
		if( !pGraph) continue;
		pIndicatorInfo = pGraph->GetIndicatorInfo();
		if( !pIndicatorInfo) continue;
		if( pIndicatorInfo->GetIndicatorName() != p_szIndicatorName) continue;

		if( p_nSubGraphIndex < 0)
		{
			pIndicatorInfo->SetCommonPacketNames( p_szPacketNames);
			pIndicatorInfo->SetAllPacketNames_SubGraphPart( p_szPacketNames, false);
		}
		else if( !pIndicatorInfo->SetPacketNames( p_nSubGraphIndex, p_szPacketNames)) continue;

		pGraph->ChangeGraphDataFromIndicatorInfo( true);
		bResult = TRUE;
	}
	return bResult;
}

// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
BOOL CVertScaleGroup::IsScaleTextDrawing()
{
	return IsVertScaleDrawing();
}

// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
void CVertScaleGroup::SetSelectedGraph(CGraphImp*& lpSelectedGraph)
{
	m_lpSelectedGraph = lpSelectedGraph;
}

// ��ǥ ������ - ojtaso (20071023)
bool CVertScaleGroup::GetVertScaleInvertType() const
{
	return m_vertScale.GetVertScaleInvertType();
}

// ��ǥ ������ - ojtaso (20071023)
void CVertScaleGroup::SetVertScaleInvertType(const bool bIsVertScaleInvertType)
{
	m_vertScale.SetVertScaleInvertType(bIsVertScaleInvertType);
}

#endif