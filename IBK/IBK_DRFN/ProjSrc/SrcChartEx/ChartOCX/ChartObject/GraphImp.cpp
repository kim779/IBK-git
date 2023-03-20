// GraphImp.cpp: implementation of the CGraphImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphImp.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/PacketList.h"				// PacketData.dll
#include "../Include_Chart/Dll_Load/PacketBase.h"				// PacketData.dll
#include "../Include_Chart/Dll_Load/PacketListManager.h"		// for CPacketListManager
#include "../Include_Chart/Dll_Load/MainChartFormulate.h"		// ChartFormulate.dll
#include "../Include_Chart/Dll_Load/IndicatorList.h"			// IndicatorCondition.dll
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"			// IndicatorCondition.dll
#include "../Include_Chart/Dll_Load/GraphDrawer.h"				// GraphDrawer.dll
#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_Chart/Dll_Load/XScaleManager.h"			// for CXScaleManager
#include "../Include_Chart/ChartFormulateData.h"
#include "../Include_Chart/IChartCtrl.h"						// for IChartCtrl
#include "../Include_Chart/Indicator.h"							// for CIndicator
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/PacketPointerList.h"
#include "../Include_Chart/MessageDefine.h"						// for UDM_RECALC_DRI
#include "MainBlockImp.h"
#include "BlockImp.h"
#include "VertScaleGroup.h"
#include "CalculatorData.h"
#include "GraphSearchInfo.h"									// for CGraphSearchInfo_IndicatorObject
#include "SubGraph.h"											// for CSubGraph
#include "PacketRQ.h"											// for CPacketRQ

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// graph�� ��ü�� �����.
CGraphImp* CGraphImp::MakeGraph(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData)
{
	return new CGraphImp(pMainBlockBaseData, pVertScaleGroup, graphData, p_szSubGraphData);
}

// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
CGraphImp* CGraphImp::MakeGraph(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData)
{
	return new CGraphImp(pPacketRQ, pMainBlockBaseData, pVertScaleGroup, graphData, p_szSubGraphData);
}
 
// ��ü�� �����Ѵ�. Make���� new�� �����Ƿ�...
void CGraphImp::Delete(CGraphImp* pGraph)
{
	if( !pGraph) return;

	delete pGraph;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CGraphImp::CGraphImp(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroup(pVertScaleGroup),
	m_pIndiInfo(NULL),
	m_pSignalPacket(NULL),
	m_graphData(graphData),
	m_pPacketRQ(NULL)	// �⺻ RQ�� ������ �ִ� Graph : �������� - ojtaso (20061224)
{
	// (2007/2/28 - Seung-Won, Bae) Support Graph Hiding
	// (2007/3/5 - Seung-Won, Bae) Init before Initialize_MakeGraph() for ST Calculation.
	m_bHide = FALSE;

	// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
	m_pIndicator = NULL;

	m_subGraphList.SetMainBlockBaseData(pMainBlockBaseData);
	m_subGraphList.SetGraph(this);

	m_subGraphList.MakeSubGraphs( p_szSubGraphData, m_graphData.GetIndicatorName());

	Initialize_MakeGraph();
}

// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
CGraphImp::CGraphImp(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pVertScaleGroup(pVertScaleGroup),
	m_pIndiInfo(NULL),
	m_pSignalPacket(NULL),
	m_graphData(graphData),
	m_pPacketRQ((CPacketRQ*)pPacketRQ)	// �⺻ RQ�� ������ �ִ� Graph : �������� - ojtaso (20061224)
{
	// (2007/2/28 - Seung-Won, Bae) Support Graph Hiding
	// (2007/3/5 - Seung-Won, Bae) Init before Initialize_MakeGraph() for ST Calculation.
	m_bHide = FALSE;

	m_pPacketRQ->AddRQGraph(this);

	m_pIndicator = NULL;

	m_subGraphList.SetMainBlockBaseData(pMainBlockBaseData);
	m_subGraphList.SetGraph(this);

	// ��ƮDLL���� ���������� ������� : �������� - ojtaso (20070904)
	if(pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl()->GetMultiItemMode())
	{
		// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
		// ����� ǥ�� : �������� - ojtaso (20070625)
		LPCTSTR lpszItemName = m_pPacketRQ->GetItemName();
		m_subGraphList.MakeSubGraphs( p_szSubGraphData, m_graphData.GetIndicatorName(), lpszItemName);
	}
	else
		m_subGraphList.MakeSubGraphs( p_szSubGraphData, m_graphData.GetIndicatorName());

	Initialize_MakeGraph();
}

// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
void CGraphImp::ChangeSubGraphTitle(LPCTSTR lpszPreName, LPCTSTR lpszNewName)
{
	// ����� ǥ�� : �������� - ojtaso (20070625)
	if(!lpszPreName || !lpszNewName/* || m_graphData.GetIndicatorName2() != _MTEXT( C2_PRICE_CHART)*/)
		return;

	m_subGraphList.ChangeSubGraphTitle(lpszPreName, lpszNewName);
}

CGraphImp::~CGraphImp()
{
	//indicatorList���� info�� �����Ѵ�. (�ɼǿ� ����..)
	DeleteIndicatorInfo();

	// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
	if( m_pIndicator) 
	{
		delete m_pIndicator;
		m_pIndicator = NULL;
	}

 	if( m_pPacketRQ)
 	{
		// (2009/9/20 - Seung-Won, Bae) Unlink first to avoid the access it on next processing.
		CPacketRQ *pPacketRQ = m_pPacketRQ;
 		m_pPacketRQ = NULL;

 		pPacketRQ->RemoveRQGraph(this);
 		if( pPacketRQ->IsEmptyRQGraph() && m_pMainBlockBaseData)
		{
		//KHD	CString szRQ = pPacketRQ->GetRQ();
		//KHD	if(szRQ.IsEmpty()==FALSE)
				m_pMainBlockBaseData->GetPacketListManager()->DeleteChartItem( pPacketRQ->GetRQ());
		}
 	}
}


// public ==================================================================================
void CGraphImp::DeleteGraph()
{
	if(m_pVertScaleGroup == NULL)
		return;

	m_pVertScaleGroup->DeleteGraphWithNotify( this);
}

// bIsOnlyNoTRData = true : TR Data�� �ƴ� ��츸 ����.
void CGraphImp::DeletePacket(const bool bIsOnlyNoTRData)
{
	DeletePacket_SubGraph(bIsOnlyNoTRData);
	DeletePacket_Signal(bIsOnlyNoTRData);
}

// ��ǥ�� ����Ѵ�.
// ��ǥ����Dlg ���� <Ȯ��>�� ���� ���
bool CGraphImp::CalculateGraph(const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsCalMinMax, const bool bIsClearPacketData, const bool bIsRequestTR)
{
	// (2006/12/12 - Seung-Won, Bae) Skip Calculation for Initializing Speed.
	if( !m_pMainBlockBaseData) return false;
	if( !m_pMainBlockBaseData->IsEnableIndicatorCalculation()) return false;

	//Initialize_IndicatorInfo(false);
	if(!CalculateIndicator(eCalType, bIsClearPacketData, bIsRequestTR))
		return false;

	if(bIsCalMinMax)
		m_pVertScaleGroup->CalculateDisplayDataMinMax();

	return true;
}

// ----------------------------------------------------------------------------------------
// graph ������ �ʱ�ȭ�� ����
void CGraphImp::Initialize_MakeGraph()
{
	Initialize_IndicatorInfo( false);
	CalculateGraph( CCalculateBaseData::TRDATA_CAL, true, false, true);
}

/*
// packet* �� indicatorInfo*�� �����Ѵ�.
void CGraphImp::Initialize_PacketAndIndicatorInfo(const bool bIsCalculate)
{
	Initialize_IndicatorInfo(false);
	Initialize_AllPackets(bIsCalculate);
}
*/

// packet*�� �����Ѵ�.
void CGraphImp::Initialize_AllPackets(const bool bIsCalculate)
{
	Initialize_AllPackets();

	if(bIsCalculate)
		CalculateGraph(CCalculateBaseData::TRDATA_CAL);
}

// indicatorInfo*�� �����Ѵ�.
void CGraphImp::Initialize_IndicatorInfo(const bool bIsNULL)
{
	m_pIndiInfo = NULL;

	if(bIsNULL)
		return;

	CIndicatorList* pIndicatorList = GetIndicatorList();
	// (2006/12/6 - Seung-Won, Bae) Support Multi-Indicator Info.
	if(pIndicatorList != NULL)
	{
		m_pIndiInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_USER, m_graphData.GetGraphName2(), m_graphData.GetIndicatorName2(), EIIGT_WITH_DEFAULT_GET_AND_SET);
		TRACE( "Graph IndicatorInfo Binding : [%s](%s) ~ 0x%08X\r\n", m_graphData.GetIndicatorName2(), m_graphData.GetGraphName2(), m_pIndiInfo);
		
		// CGraphImp : �������� - ojtaso (20070308)
		if( m_pIndiInfo)
		{
			m_pIndiInfo->SetGraph(this);

			int nCount = m_subGraphList.GetSubGraphCount();
			for(int nIndex = 0; nIndex < nCount; nIndex++)
			{
				CSubGraphData *pIndiData = m_pIndiInfo->GetSubGraphData( nIndex);
				CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData( nIndex);
				if( !pIndiData || !pGraphData) continue;

				pIndiData->SetItemName( pGraphData->GetItemName());

				// (2007/12/17 - Seung-Won, Bae) Update Graph Type and Style of IndiCond with Graph Data of Design Time.
				pIndiData->SetType( pGraphData->GetType());
				pIndiData->SetStyle( pGraphData->GetStyle());
				pIndiData->SetDrawStyle( pGraphData->GetDrawStyle());
			}
		}
	}
}

// ----------------------------------------------------------------------------------------
// �ش� graph�� indicatorInfo ���� ����.
// -> gaphData(subGraph����)�� IndicatorInfo�� ����.
//	  (title �� ���ǰ��� �ش�Ǵ��� Ȯ���Ͽ� �����ϱ�)
//    (��ǥ���� ���õ� ������ ����� ��� ��ǥ��� ����)
// (2006/11/27 - Seung-Won, Bae) Do not use SetGraphName() & SetIndicatorName()
bool CGraphImp::ChangeGraphDataFromIndicatorInfo(const bool bIsCalculate)
{
	if( !m_pIndiInfo) return false;
	if( m_subGraphList.GetSubGraphCount() != m_pIndiInfo->GetSubGraphCount()) return false;

	// subGraph count�� ���� ��� -> subGraphData �� ����
	CList<CSubGraphData*, CSubGraphData*> sourceSubGraphDataList;
	if( !m_pIndiInfo->GetAllSubGraphDataList( sourceSubGraphDataList)) return false;
	if( !m_subGraphList.ChangeAllSubGraphData( sourceSubGraphDataList, true)) return false;

	// graphData ����
	m_graphData = m_pIndiInfo->GetGraphData();

	//��ǥ ��� -> subGraph ������ Ʋ���� ����� ����Ѵ�.
	if(bIsCalculate) Initialize_AllPackets(true);
	
	//{{ (2007/5/28 - LYH) For Weighted Volume Chart
	CheckWeightedVolume();
	//}}

	// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI
	CString strIndicatorName = m_pIndiInfo->GetIndicatorName();
	if( g_iMetaTable.IsSpecialGroupGraph( strIndicatorName) || strIndicatorName == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART))
	{
		if( strIndicatorName == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART))
		{
			// xScaleManager/�������� ���� - xScaleManager - ojtaso (20070404)
			CString strRQ = GetGraphRQ();
			CXScaleManager* pxScaleManager = GetMainBlock()->GetXScaleManager();
			if( pxScaleManager)
			{
				pxScaleManager->DelOEBTRQ( strRQ);
				pxScaleManager->AddOEBTRQ( strRQ, m_graphData.GetGraphName(), m_graphData.GetIndicatorName());
			}
		}
		::PostMessage( m_pMainBlockBaseData->GetMainAttributes().GetParent_Handle(), UDM_RECALC_DRI, NULL, NULL);
	}

	return true;
}

BOOL CGraphImp::ChangeGraphDataFromIndicatorInfo( CIndicatorInfo& indicatorInfo, const bool bIsCalculate)
{
	if( m_subGraphList.GetSubGraphCount() != indicatorInfo.GetSubGraphCount()) return false;

	// subGraph count�� ���� ��� -> subGraphData �� ����
	CList<CSubGraphData*, CSubGraphData*> sourceSubGraphDataList;
	if( !indicatorInfo.GetAllSubGraphDataList( sourceSubGraphDataList)) return false;
	if( !m_subGraphList.ChangeAllSubGraphData( sourceSubGraphDataList, true)) return false;

	// graphData ����
	m_graphData = indicatorInfo.GetGraphData();

	//��ǥ ��� -> subGraph ������ Ʋ���� ����� ����Ѵ�.
	if(bIsCalculate) Initialize_AllPackets(true);
	return true;
}

// -> IndicatorInfo�� gaphData(subGraph����)�� ����.
bool CGraphImp::ChangeIndicatorInfoFromGraphData()
{
	if(m_pIndiInfo == NULL)
		Initialize_IndicatorInfo(false);

	if(m_pIndiInfo == NULL)
		return false;

	return ChangeIndicatorInfoFromGraphData(*m_pIndiInfo);
}

bool CGraphImp::ChangeIndicatorInfoFromGraphData(CIndicatorInfo& indicatorInfo)
{
	if(!ChangeIndicatorInfoFromAllSubGraphData(indicatorInfo))
		return false;

	indicatorInfo.GetGraphData() = m_graphData;

	//{{ (2007/5/28 - LYH) For Weighted Volume Chart
	CheckWeightedVolume();
	//}}
	return true;
}

/*
// -> graphData �� indicatorInfo�� ��� ����.
bool CGraphImp::ChangeIndicatorInfoAndGraphData(const CIndicatorInfo* pSourceIndicatorInfo, const bool bIsCalculate)
{
	if(pSourceIndicatorInfo == NULL)
		return false;

	//indicatorInfo �ٲٱ�
	bool bResult = ChangeIndicatorInfo(pSourceIndicatorInfo);

	//��ǥ�� �ٽ� ��� ���� �𸣹Ƿ� subGraph �κ��� ���� �����Ѵ�.
	if(!ChangeAllSubGraphDataFromIndicatorInfo(pSourceIndicatorInfo))
		return bResult;

	ChangeGraphData(const_cast<CIndicatorInfo*>(pSourceIndicatorInfo)->GetGraphData(), false);
	if(bIsCalculate)
		CalculateIndicator(CCalculateBaseData::TRDATA_CAL, true);
	
	return true;
}
*/

// -> �ش� sub graph�� indicatorInfo ���� ����.
bool CGraphImp::ChangeAllSubGraphTitleFromIndicator()
{
	if(m_pIndiInfo == NULL)
		return false;

	bool bResult = false;
	int nCount = m_subGraphList.GetSubGraphCount();
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		if(ChangeSubGraphTitleFromIndicator(nIndex))
			bResult = true;
	}
	return bResult;
}

bool CGraphImp::ChangeSubGraphTitleFromIndicator(const int nSubGraphIndex)
{
	if(m_pIndiInfo == NULL)
		return false;

	return m_subGraphList.SetTitle(nSubGraphIndex, m_pIndiInfo->GetSubGraphTitle(nSubGraphIndex));
}

// -> �ش� graph�� Map Default IndicatorInfo�� ����
bool CGraphImp::ChangeMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList)
{
	if(pIndicatorList == NULL) return false;

	// (2006/12/6 - Seung-Won, Bae) Support Multiple-Indicator.
	CIndicatorInfo* pMapDefaultInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_MAP_DEFAULT, m_graphData.GetGraphName2(), m_graphData.GetIndicatorName2(), EIIGT_WITH_DEFAULT_GET_AND_SET);
	if( pMapDefaultInfo == NULL) return false;

	// CGraphImp : �������� - ojtaso (20070308)
//	pMapDefaultInfo->SetGraph(this);

	return ChangeIndicatorInfoFromGraphData( *pMapDefaultInfo);
}

// ----------------------------------------------------------------------------------------
void CGraphImp::SetVertScaleGroup(CVertScaleGroup* pVertScaleGroup)
{
	m_pVertScaleGroup = pVertScaleGroup;
}

/*
// graph data setting.
// �̹� �����Ǿ� �ִ� graph�� data�� setting.
bool CGraphImp::SetGraph(const CGraphData& graphData, const CString& strSubGraphData)
{
	//testing ->functionIndex �� copy??
	if(strSubGraphData.IsEmpty()) // || graphData.GetFunctionIndex() < 0
		return false;

	// subGraph�� ���� setting �ؾ� �Ѵ�.
	// ChangeGraphData() ���� ��ǥ�� �ٽ� ����ϴ� ��찡 �����Ƿ�..
	m_subGraphList.SetSubGraphs(strSubGraphData);

	// graph data �κ� setting.
	ChangeGraphData(graphData, true);
	return true;
}
*/

/*
// �Լ���� packetname�� �����Ѵ�.
bool CGraphImp::SetGraphNameAndPacketName(const CString& strGraphName, const CString& strPacketName)
{
	bool bResult = false;
	// �Լ��� ����
	if(SetGraphName(strGraphName, false))
		bResult = true;
	// packet�� ����
	if(SetPacketNames(strPacketName, true, false))
		bResult = true;

	// �Լ���, packet ���� �ٲ� ���� ����Ѵ�.
	if(bResult)
		Initialize_PacketAndIndicatorInfo(true);

	return bResult;
}
*/

/*
// �Լ���� subGraphName �����Ѵ�.
// bIsSubGraphCheck == true �϶��� subGraphName ���Ѵ�.
// �Լ��� �� -> subGraph �� -> ��ǥ ���
// �Լ���, subGraph ���� -> ��ǥ ���
// subGraphC const ����Ǹ� Info�� ���� graph �ٽ� ����.
bool CGraphImp::SetGraphNameAndSubGraphNames(const CString& strGraphName, const bool bIsSubGraphCheck, const bool bIsCalculate)
{
	// �Լ��� ���� -> ��ǥ����� subGraphName check�Ŀ� �Ѵ�.
	// �Լ����� ������� ���� ��� : subGraph�� üũ���� ���� ��츸 return ��.
	if(!SetGraphName(strGraphName, false) && !bIsSubGraphCheck)
		return false;

	// subGraphName ���� -> subGraph�� ���� �����.
	if(bIsSubGraphCheck)
	{
		ChangeAllSubGraphData(true, false);
	}

	// ��ǥ ��� -> �Լ����̳� subGraph �� ���� ���� ����� ���� ���߿� �Ѵ�.
	//				����� �ߺ��� ���ϱ� ����.
	Initialize_PacketAndIndicatorInfo(bIsCalculate);
	return true;
}
*/

/*
bool CGraphImp::SetGraphName(const CString& strGraphName, const bool bIsCalculate)
{
	if(m_graphData.GetGraphName() == strGraphName)
		return false;

	m_graphData.SetGraphName(strGraphName);
	Initialize_PacketAndIndicatorInfo(bIsCalculate);
	return true;
}
*/

bool CGraphImp::SetPacketNames(const CString& strPacketName, const bool bIsChangeIndicatorInfo, const bool bIsCalculate)
{
	if(IsEqualPacketNames(strPacketName))
		return false;

	m_graphData.SetPacketNames(strPacketName);
	if(bIsChangeIndicatorInfo)
		ChangePacketNameInIndicatorInfo(strPacketName);

	Initialize_AllPackets(bIsCalculate);
	return true;
}

bool CGraphImp::SetGraphDataDrawingData(const CGraphDataDrawingData& sourceGraphDrawingData)
{
	// (2007/2/22 - Seung-Won, Bae) Do not support the Up/Down Info, But only Candle Graph
	CGraphDataDrawingData graphDrawingData( sourceGraphDrawingData);
	CSubGraph *pSubGraph = m_subGraphList.GetSubGraph( 0);
	if( pSubGraph) if( pSubGraph->GetSubGraphData().GetType() != CGraphBaseData::Bong_Type)
		if( graphDrawingData.GetCalcRiseFallRate_CurData() < 3)
			graphDrawingData.SetCalcRiseFallRate_CurData( graphDrawingData.GetCalcRiseFallRate_CurData() + 2);
	
	m_graphData.SetGraphDataDrawingData( graphDrawingData);
	if(m_pIndiInfo != NULL)
		m_pIndiInfo->GetGraphData().SetGraphDataDrawingData( graphDrawingData);

	return true;
}

// GraphDataDraw ���� �߰� - ojtaso (20080609)
bool CGraphImp::ShowGraphDataDrawingData(const bool bShow, const int nShowType)
{
	CGraphDataDrawingData graphDrawingData = m_graphData.GetGraphDataDrawingData();
	
	graphDrawingData.SetDoesCalcRiseFallRate_HighCur(bShow);
	graphDrawingData.SetDoesCalcRiseFallRate_LowCur(bShow);
	graphDrawingData.SetDoesCalcRiseFallRate_LowHigh(bShow);

	graphDrawingData.SetCalcRiseFallRate_CurData(nShowType);

	m_graphData.SetGraphDataDrawingData( graphDrawingData);
	if(m_pIndiInfo != NULL)
		m_pIndiInfo->GetGraphData().SetGraphDataDrawingData( graphDrawingData);

	return true;
}

// ----------------------------------------------------------------------------------------
// ���� class �ּ�
void CGraphImp::SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData)
{
	m_pMainBlockBaseData = pMainBlockBaseData;

	m_subGraphList.SetMainBlockBaseData(m_pMainBlockBaseData);
}

CMainBlockImp* CGraphImp::GetMainBlock() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	return m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
}

CBlockImp* CGraphImp::GetBlock() const
{
	if(m_pVertScaleGroup == NULL)
		return NULL;
	return m_pVertScaleGroup->GetBlock();
}

CVertScaleGroupList* CGraphImp::GetVertScaleGroupList() const
{
	if(m_pVertScaleGroup == NULL)
		return NULL;
	return m_pVertScaleGroup->GetVertScaleGroupList();
}

CVertScaleGroup* CGraphImp::GetVertScaleGroup() const
{
	return m_pVertScaleGroup;
}

// graph data
CGraphData& CGraphImp::GetGraphData()
{
	return m_graphData;
}

const CGraphData& CGraphImp::GetGraphData() const
{
	return m_graphData;
}

// graph�� ������ �ִ� subGraph���� �ִ� list.
CSubGraphList& CGraphImp::GetSubGraphList()
{
	return m_subGraphList;
}

const CSubGraphList& CGraphImp::GetSubGraphList() const
{
	return m_subGraphList;
}

CIndicatorInfo* CGraphImp::GetIndicatorInfo()
{
	return m_pIndiInfo;
}

// ----------------------------------------------------------------------------------------
// graph data ��������
void CGraphImp::GetStringGraphData( CStringList &p_slGraphData) const
{
	//>> JS.Kim_20100929  solomon.. PAIR RQ�� �������� �ʵ��� ����..
	if(m_pPacketRQ)
	{
		CString strRQ = m_pPacketRQ->GetRQ();
		if( strRQ.GetLength() > 5 && strRQ.Right(5) == "_PAIR" )
		{
			return;
		}
	}
	//<<
	p_slGraphData.AddTail( "Graph;\r\n");

	p_slGraphData.AddTail( CString());
	CString &strGraphData = p_slGraphData.GetTail();

	// �������� ���� : �������� - ojtaso (20070507)
	if(m_pPacketRQ)
		m_graphData.GetData( strGraphData, m_pPacketRQ->GetRQ());
	else
		m_graphData.GetData( strGraphData);

	m_subGraphList.GetSubGraphsData( p_slGraphData);


	p_slGraphData.AddTail( "GraphEnd;\r\n");
}

// ----------------------------------------------------------------------------------------
// graph title region
CRect CGraphImp::GetGraphTitleRegion(CDC* pDC, const CRect& graphRegion, int& preGraphTitleEndPosition) const
{
	if(m_subGraphList.AreHidingAllSubGraphs())
		return CRect(0, 0, 0, 0);

	CString subGraphTitle = m_subGraphList.GetViewableSubGraphTitles();
	if(subGraphTitle.IsEmpty())
		return CRect(0, 0, 0, 0);

	CSize textSize = pDC->GetTextExtent(subGraphTitle);

	CRect titleRegion;
	titleRegion.left = graphRegion.left + preGraphTitleEndPosition + MARGIN;
	titleRegion.top = graphRegion.top + MARGIN;
	int totalMargin = (m_subGraphList.GetViewableSubGraphCount()-1)*MARGIN*3;
	titleRegion.right = titleRegion.left + textSize.cx + totalMargin;
	titleRegion.bottom = titleRegion.top + textSize.cy;

	preGraphTitleEndPosition = titleRegion.right - graphRegion.left + MARGIN*2;

	return titleRegion;
}

// ----------------------------------------------------------------------------------------
// view data setting
void CGraphImp::GetSubGraphViewDataList(CSubGraphViewDataList& subGraphViewDataList) const
{
	// (2010/6/24 - Seung-Won, Bae) Skip overhanging
	if( m_graphData.GetIndicatorName() == _MTEXT( C2_OVERHANGING_SUPPLY)) return;
	
	m_subGraphList.GetSubGraphViewDataList(m_graphData.GetGraphName2(), m_graphData.GetPacketNames(), subGraphViewDataList);
}

// ----------------------------------------------------------------------------------------
// ���� scale�� ���� ���Ѵ�.
int CGraphImp::GetLeftVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const
{
	// graph ����, <Data ǥ����> �� ��쿣 ������ �ʴ´�.
	if(m_subGraphList.AreHidingAllSubGraphs() || m_subGraphList.IsViewableGraphType(CGraphBaseData::DataView_Type, 0))
		return 0;

	return GetVertScaleRegionWidth(pDC, pCalData);
}

int CGraphImp::GetRightVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const
{
	// graph ����, <Data ǥ����> �� ��쿣 ������ �ʴ´�.
	if(m_subGraphList.AreHidingAllSubGraphs() || m_subGraphList.IsViewableGraphType(CGraphBaseData::DataView_Type, 0))
		return 0;

	return GetVertScaleRegionWidth(pDC, pCalData);
}

// ----------------------------------------------------------------------------------------
// ���� ���̴� ȭ�鿡���� min, max�� ���Ѵ�. ==> y���� ��
bool CGraphImp::GetDisplayDataMinMax(const CString& strScaleData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const
{
	if(m_pMainBlockBaseData == NULL || !IsDisplayDataMinMax())
		return false;

	CVertDisplayDataMinMaxInputData inputData( m_graphData.GetIndicatorName(), m_graphData.GetGraphName2(), strScaleData, m_pMainBlockBaseData->GetDisplayAttributes());

	return m_subGraphList.GetDisplayDataMinMax(&inputData, displayDataMinMax, p_bWithFullRange);
}

// packet���� ���翩��
bool CGraphImp::IsExistencePacketName(const CList<CString, CString>& packetNameList) const
{
	if(packetNameList.GetCount() <= 0)
		return false;

	POSITION pos = packetNameList.GetHeadPosition();
	while(pos != NULL)
	{
		if(IsExistencePacketName(packetNameList.GetNext(pos)))
			return true;
	}
	return false;
}

bool CGraphImp::IsExistencePacketName(const CString& strPacketName) const
{
	if(strPacketName.IsEmpty())
		return false;

	// Find �� �� ��� �߸��� ����� return �� �� �ִ�.
	// ex) strPacketName == "�ð�", graphPacketNames = "PF_�ð�" �ΰ�� -> return : true
	// �̷� ���� �߸��� ����. �̰� ���� ���� "," �� ���� ���Ѵ�.

	CString strGraphPacketNames = m_graphData.GetPacketNames();
	if(strGraphPacketNames.IsEmpty() || strGraphPacketNames.Find(strPacketName) < 0)
		return false;

	// graphPacket �� �ϳ��� ���
	if(strGraphPacketNames.Find(",") < 0){
		return (strGraphPacketNames == strPacketName);
	}
	
	// graphPacket �� �ΰ��̻��� ���
	if(strGraphPacketNames.Right(1) != ",")
		strGraphPacketNames += ",";

	while(!strGraphPacketNames.IsEmpty()){
		CString strTemp = CDataConversion::GetStringData(strGraphPacketNames, ",");
		if(strTemp == strPacketName)
			return true;
	}

	return false;
}

// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
void CGraphImp::DrawGraphTitle( CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine)
{
	if(pDrawingGraphData == NULL || m_subGraphList.AreHidingAllSubGraphs())
		return;

	CDataConversion::SelectClipRegion( pDC, pDrawingGraphData->GetGraphRegions().GetNoFrameRegion());
	pDrawingGraphData->SetGraphName( m_graphData.GetGraphName2());
	pDrawingGraphData->SetPacketNames( m_graphData.GetPacketNames());
	pDrawingGraphData->SetGraphDataDrawingData(m_graphData.GetGraphDataDrawingData());

	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	bool bOldConditionTitleDraw = false, bOldDataTitleDraw = false;
	ChangeTitleDraw( pDrawingGraphData, bOldConditionTitleDraw, bOldDataTitleDraw);
	pDrawingGraphData->SetIndicatorInfo(m_pIndiInfo);
	pDrawingGraphData->SetPriceChartColor(GetPriceChartColor());
	
	// PacketList ���� : �������� - ojtaso (20070214)
	if( m_pPacketRQ) pDrawingGraphData->SetPacketList(m_pPacketRQ->GetPacketList());

	m_subGraphList.DrawSubGraphTitles( pDC, pDrawingGraphData, graphTitlePosition, graphTitleLine, nGraphTitleEndLine);

	pDrawingGraphData->SetTitleDraw( bOldConditionTitleDraw, bOldDataTitleDraw);

	CDataConversion::SelectClipRegion(pDC);
}

// �׷��� �׸� (20071007)
void CGraphImp::DrawGraphAndSignal( CDC* pDC, CDrawingGraphData* pDrawingGraphData)
{
	if(pDrawingGraphData == NULL || m_subGraphList.AreHidingAllSubGraphs())
		return;

	CDataConversion::SelectClipRegion(pDC, pDrawingGraphData->GetGraphRegions().GetNoFrameRegion());
	pDrawingGraphData->SetGraphName( m_graphData.GetGraphName2());
	pDrawingGraphData->SetPacketNames(m_graphData.GetPacketNames());
	pDrawingGraphData->SetGraphDataDrawingData(m_graphData.GetGraphDataDrawingData());

	pDrawingGraphData->SetIndicatorInfo(m_pIndiInfo);
	pDrawingGraphData->SetPriceChartColor(GetPriceChartColor());

	// PacketList ���� : �������� - ojtaso (20070214)
	if(m_pPacketRQ) pDrawingGraphData->SetPacketList(m_pPacketRQ->GetPacketList());

	m_subGraphList.DrawSubGraphs(pDC, pDrawingGraphData);

	// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
	DrawVerticalBox(pDC, pDrawingGraphData);

	pDrawingGraphData->SetSubGraphPacketList(m_subGraphList.GetSubGraphPacketList());

	if( m_pSignalPacket && !m_graphData.GetIndicatorName2().IsEmpty() && m_graphData.GetSignalData().IsDraw())
	{
		// xScaleManager/�������� ���� - xScaleManager - ojtaso (20070404)
		CString strRQ = GetGraphRQ();
		CXScaleManager* pxScaleManager = GetMainBlock()->GetXScaleManager();

		CGraphDrawer().DrawSignal( pDC, *pDrawingGraphData, m_pSignalPacket->GetnumericDataList(), m_graphData.GetSignalData(), strRQ, pxScaleManager);
	}

	CDataConversion::SelectClipRegion(pDC);
}

// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
void CGraphImp::DrawVerticalBox(CDC* pDC, CDrawingGraphData* pDrawingGraphData)
{
	CXScaleManager* pxScaleManager = GetMainBlock()->GetXScaleManager();

	COLORREF clrBox;
	BOOL bFill = -1;
	CString strName = GetBlock()->GetBlockData().GetHorzScale().GetScaleData();
	CRect rect = pDrawingGraphData->GetGraphRegions().GetNoFrameRegion();

	pxScaleManager->GetVerticalBoxInfo(GetGraphRQ(), strName, rect, clrBox, bFill);
	if(bFill != -1)
	{
		rect.NormalizeRect();
		rect.top -= 3;
		rect.bottom += 4;
		pDC->Draw3dRect(rect, clrBox, clrBox);
	}
}
// private ================================================================================
// Info�� data�� SubGraph�� �����.
/*
bool CGraphImp::MakeAllSubGraphFromIndicatorInfo(const bool bIsCalculate)
{
	if(m_pIndiInfo == NULL)
		return false;

	CString strAllSubGraphData = m_pIndiInfo->GetAllSubGraphDataForMaking();
	if(strAllSubGraphData.IsEmpty())
		return false;

	m_subGraphList.MakeSubGraphs(strAllSubGraphData);
	if(bIsCalculate)
		CalculateGraph(CCalculateBaseData::TRDATA_CAL, false, true, false);

	return true;
}
*/

// IndicatorInfo �����ϱ�
bool CGraphImp::DeleteIndicatorInfo()
{
	// ��ǥ�� Graph�� NULL�� ���� : Ư����Ʈ - ojtaso (20070619)
	if(m_pIndiInfo)
		m_pIndiInfo->SetGraph(NULL);

	if(!IsDeleteIndicatorInfo())
		return false;

	TRACE( "Graph Deleted : [%s](%s) ~ 0x%08X\r\n", m_graphData.GetIndicatorName(), m_graphData.GetGraphName(), long( this));
	const char *szGraphName = m_pIndiInfo->GetGraphName();
	bool bResult = ( GetIndicatorList()->RemoveGraphIndicatorInfo( szGraphName) ? true : false);
	m_pIndiInfo = NULL;

	return bResult;
}

// Packet ���� �ϱ�
void CGraphImp::DeletePacket_SubGraph(const bool bIsOnlyNoTRData)
{
	m_subGraphList.DeleteAllPackets(bIsOnlyNoTRData);
}

void CGraphImp::DeletePacket_Signal(const bool bIsOnlyNoTRData)
{
	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL)
		return;

	pPacketList->DeleteOrClearPacket(m_pSignalPacket, true, bIsOnlyNoTRData);
	m_pSignalPacket = NULL;
}

// Packet Clear �ϱ�
void CGraphImp::ClearPacket(const bool bIsOnlyNoTRData)
{
	ClearPacket_Signal(bIsOnlyNoTRData);
	m_subGraphList.ClearAllPackets(bIsOnlyNoTRData);
}

void CGraphImp::ClearPacket_Signal(const bool bIsOnlyNoTRData)
{
	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL)
		return;

	pPacketList->DeleteOrClearPacket(m_pSignalPacket, false, bIsOnlyNoTRData);
	m_pSignalPacket = NULL;
}

// ---------------------------------------------------------------------------------------
void CGraphImp::Initialize_AllPackets()
{
	m_pSignalPacket = NULL;
	m_subGraphList.Initialize_AllPackets();

//	CMainChartFormulate* pMainChartFormulate = GetMainChartFormulate();
//	if(pMainChartFormulate != NULL)
//		pMainChartFormulate->Initialize_AllPackets();
}

// ---------------------------------------------------------------------------------------
bool CGraphImp::CalculateIndicator(const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsClearPacketData, const bool bIsRequestTR)
{
	if(bIsClearPacketData)
	{
		ClearPacket();
	}

	bool bResult = CalculateIndicator_Indicator(eCalType);
	if(bIsRequestTR)
		CalculateIndicator_TR(eCalType);

	// (2009/5/26 - Seung-Won, Bae) Clear SubGraph Drawing Cahe.
	if( eCalType != CCalculateBaseData::REALDATA_CAL)
	{
		m_subGraphList.ClearCalcDataForDraw();
		m_graphData.GetSignalData().ClearCalcDataForDraw();
	}

	// (2010/4/18 - Seung-Won, Bae) for Proce Steady Color of Volume Chart.
	CPacketList* pPacketList = m_pMainBlockBaseData->GetPacketList();
	if( CCalculateBaseData::TRDATA_CAL == eCalType && m_graphData.GetIndicatorName() == _MTEXT( C2_PRICE_CHART) && pPacketList)
		m_pMainBlockBaseData->GetXScaleManager()->SetNeedCalcAreaFlag( 2);	// for price chart color retrieving

	return bResult;
}

bool CGraphImp::CalculateIndicator_TR(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	if(eCalType != CCalculateBaseData::TRDATA_CAL)
		return false;

	return SendMessage_TRData();
}

bool CGraphImp::CalculateIndicator_Indicator(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	CMainChartFormulate* pMainChartFormulate = GetMainChartFormulate();
	if(pMainChartFormulate == NULL)
		return false;

	CGraphCalculateData graphCalculateData( m_pSignalPacket, m_pIndicator);
	if(!m_subGraphList.GetAllSubGraphCalculateData(graphCalculateData))
		return false;

	CChartFormulateData chartFormulateData;
	if(!GetChartFormulateData(eCalType, chartFormulateData))
		return false;

	bool bResult = pMainChartFormulate->CalculateIndicator(chartFormulateData, &graphCalculateData, m_pIndiInfo);
	// (2006/11/19 - Seung-Won, Bae) Call ST Indiator Process.
	if( graphCalculateData.GetIndicator())
		if( g_iMetaTable.IsSTIndexIndicator( graphCalculateData.GetIndicator()->GetID()))
			CalculateIndicator_OCX(eCalType);

	return bResult;
}

// OCX�� ��ǥ ����϶�� �˷��ش� -> TR ����� ��츸..
void CGraphImp::CalculateIndicator_OCX(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	if(m_pMainBlockBaseData == NULL || eCalType != CCalculateBaseData::TRDATA_CAL)
		return;

	CList<CPacket*, CPacket*> packetList;
	if(!m_subGraphList.GetSubGraphPacketList(packetList))
		return;

	// signal(��ȣ)�� ����ϴ� ��ǥ�� ��� signal packet�� ÷��.
	if(m_pSignalPacket != NULL)
		packetList.AddTail(m_pSignalPacket);

	// (2007/2/27 - Seung-Won, Bae) Skip if All SubGraph Hide.
	//	Do not skip calculating for ST Notify.
	//	if( m_subGraphList.AreHidingAllSubGraphs()) return;

	m_pMainBlockBaseData->SendMessageToParent_CalculateIndicatorInTRData(m_pIndiInfo, packetList);
}

// TR ��û
// -> "������ǥ"
bool CGraphImp::SendMessage_TRData()
{
	if(!IsSendMessage_TRData())
		return false;

	int nRequestTRCount = 0;
	bool bIsNew = false;
	if(!GetRequestTRCountAndType(nRequestTRCount, bIsNew))
		return false;

	m_pMainBlockBaseData->SendMessageToParent_TRData( m_graphData.GetGraphName2(), m_graphData.GetPacketNames(), nRequestTRCount, bIsNew);
	return true;
}

// ---------------------------------------------------------------------------------------
// "BoxChart", "���Ź�"�϶��� ��� ����
// (����: GraphType �� subGraph ���� üũ�Ѵ�.)
// 2004.05.12. -> "���Ź�"�� Ư�������� ÷�������� ����(������Ʈ�� scaleGroup ���)
bool CGraphImp::IsDisplayDataMinMax() const
{
	CString strIndicatorName = m_graphData.GetIndicatorName2();
	return (strIndicatorName != _MTEXT( C2_OVERHANGING_SUPPLY));
}

bool CGraphImp::IsEqualPacketNames(const CString& strPacketNames) const
{
	if(m_graphData.GetPacketNames() == strPacketNames)
		return true;

	// "," �� " "�� �����ϰ� ���Ѵ�.
	return (GetNoSpaceCommaString(m_graphData.GetPacketNames()) == GetNoSpaceCommaString(strPacketNames));
}

bool CGraphImp::IsDeleteIndicatorInfo() const
{
	if(m_pIndiInfo == NULL || m_pMainBlockBaseData == NULL)
		return false;

	CIndicatorList* pIndicatorList = GetIndicatorList();
	if(pIndicatorList == NULL)
		return false;

	// indicatorInfo �ڵ� ������ ��츸 �����Ѵ�!
	if(!m_pMainBlockBaseData->DoesAutoDeleteIndicatorInfo())
		return false;

	// (2007/2/16 - Seung-Won, Bae) Check for Previous Version of Multiple and Same Name Indicator.
	// ���� �ٸ� graph���� ���� IndicatorInfo �� ���� �ִ��� üũ!!	
	CMainBlockImp* pMainBlock = GetMainBlock();
	if(pMainBlock == NULL) return false;
	CGraphSearchInfo_IndicatorObject gsi;
	gsi.m_pIndicatorInfo	= m_pIndiInfo;
	gsi.m_pExceptGraph		= this;
	return !pMainBlock->FindGraph( &gsi);
}

bool CGraphImp::IsSendMessage_TRData() const
{
	if(m_pIndiInfo == NULL || m_pMainBlockBaseData == NULL)
		return false;

	// TR data type : <�ϰ�> �� ��츸 ó��
	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL || pPacketList->GetDateUnitType() != CScaleBaseData::HORZ_DAILY)
		return false;

	// group �� üũ
	// -> ���߿� �����ʿ��� ��ǥ�� TR �κ��� �з��ѵ� ó��.
//	IChartCtrl *pChartCtrl = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl();
//	if( !pChartCtrl) return false;
//	return pChartCtrl->IsIndicatorGroup_TR( m_pIndiInfo->GetGroupName(), m_pIndiInfo->GetIndicatorName());
	return g_iMetaTable.IsMarketOrServerIndex( m_pIndiInfo->GetIndicatorName());
}

bool CGraphImp::IsSendMessage_NewTRData(const CPacket* pPacket) const
{
	return (pPacket == NULL || !pPacket->IsAcceptFromServer());
}

bool CGraphImp::IsSendMessage_InsertTRData(const CPacket* pPacket) const
{
	//�⺻ TR �� �ȵ��� ���� Insert TR �� �ƴ�!
	if(pPacket == NULL || !pPacket->IsAcceptFromServer())
		return false;

	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL)
		return false;

	CPacketDataInfo::DATAINSERTTYPE eDataInsertType = pPacketList->GetDataInsertType();
	return (eDataInsertType == CPacketDataInfo::INSERT);
}

//���� data count �� ó�� TR data count ���� ���� ���(��Ź��, �ݸ� ���)
//��ü TR data count�� ���� ���� TR�� ��û���� �ʾƵ� �ȴ�.
bool CGraphImp::IsSendMessage_InsertTRData(const CPacketList* pPacketList, const int nDataCount) const
{
	if(pPacketList == NULL)
		return false;

	int nTotalTRIndex = pPacketList->GetTotalTRIndex();
	// ��ȸ�� ������ ���� ��� - TR ��û ����
	if(nTotalTRIndex <= 0)
		return false;
	// ��ȸ�� ������ 1���� ��� - dataCount �� �ִ� ���� ��û ����
	else if(nTotalTRIndex == 1)
		return (nDataCount <= 0);
	// ��ȸ�� ������ 2�� �̻��� ��� - ��ü data count�� ��
	else
		return (nDataCount < pPacketList->GetTotalTRCount());

	return false;
}

// ---------------------------------------------------------------------------------------
CPacketList* CGraphImp::GetPacketList() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	// �������� �ҷ����� : �������� - ojtaso (20070514)
	if(m_pPacketRQ)
		return m_pPacketRQ->GetPacketList();
	else
		return m_pMainBlockBaseData->GetPacketList();
}

CIndicatorList* CGraphImp::GetIndicatorList() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	return m_pMainBlockBaseData->GetIndicatorList();
}

CMainChartFormulate* CGraphImp::GetMainChartFormulate() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	return m_pMainBlockBaseData->GetMainAttributes().GetMainChartFormulate();
}

bool CGraphImp::GetChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType, CChartFormulateData& chartFormulateData) const
{
	CPacketList* pPacketList = GetPacketList();
	CIndicatorList* pIndicatorList = GetIndicatorList();
	if(pPacketList == NULL || pIndicatorList == NULL)
		return false;

	chartFormulateData.SetCalculateType(eCalType);
	chartFormulateData.SetGraphName(		m_graphData.GetGraphName2());
	chartFormulateData.SetIndicatorName(	m_graphData.GetIndicatorName2());	// (2006/12/5 - Seung-Won, Bae) Support Indicator Name
	chartFormulateData.SetPacketNames(m_graphData.GetPacketNames());
	chartFormulateData.SetSubGraphNames(m_subGraphList.GetSubGraphNames());
	chartFormulateData.SetViewableSubGraphNames(m_subGraphList.GetViewableSubGraphNames());
	chartFormulateData.SetDisplayDataStartIndex(m_pMainBlockBaseData->GetDisplayAttributes().GetDataStartInBlock());
	chartFormulateData.SetDisplayDataEndIndex(m_pMainBlockBaseData->GetDisplayAttributes().GetDataEndInBlock());
	chartFormulateData.SetPacketList(pPacketList);
	chartFormulateData.SetIndicatorList(pIndicatorList);
	chartFormulateData.SetOcxHwnd( m_pMainBlockBaseData->GetMainAttributes().GetParent_Handle());
	// (2009/9/8 - Seung-Won, Bae) Chart Running Mode for the brick size of Renko on HTS.
	chartFormulateData.SetChartMode( m_pMainBlockBaseData->GetMainAttributes().GetChartMode());
	return true;
}

// graph scale type �����´�.
CScaleBaseData::VERTSCALEPOSITION CGraphImp::GetVertScalePosition(const CString& strVertScalePosition) const
{
	int vertScalePosition = atoi(strVertScalePosition);
	if(vertScalePosition < 0 || vertScalePosition > 3)
		return CScaleBaseData::VERT_NONE;
	return (CScaleBaseData::VERTSCALEPOSITION) vertScalePosition;
}

// ---------------------------------------------------------------------------------------
// ���� scale width ���ϱ�.
int CGraphImp::GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const
{
	assert(pCalData != NULL);

	// min/max�� ���ϴ� ��ǥ�� width�� ���Ѵ�.
	if(!IsDisplayDataMinMax())
		return 0;

	pCalData->SetCalcRiseFallRate_CurData(m_graphData.GetGraphDataDrawingData().GetCalcRiseFallRate_CurData());
	pCalData->SetGraphName( m_graphData.GetGraphName2());
	pCalData->SetIndicatorInfo( m_pIndiInfo);
	return m_subGraphList.GetVertScaleRegionWidth( pDC, pCalData);
}

// <������Ʈ>�� color ���ϱ�
// <�ŷ��� ��Ʈ>, <�ŷ����>�� ��츸 �ʿ��ϴ�.
CGraphColor* CGraphImp::GetPriceChartColor() const
{
	if(m_pMainBlockBaseData == NULL)
		return NULL;

	if( !g_iMetaTable.IsDrawGroup_VolumeType( m_graphData.GetIndicatorName2())) return NULL;

	CMainBlockImp* pMainBlock = GetMainBlock();
	if(pMainBlock == NULL)
		return NULL;

	return pMainBlock->GetGraphColor( _MTEXT( C2_PRICE_CHART), 0);
}

// �糡�� " "�� ","�� ���� string.
CString CGraphImp::GetNoSpaceCommaString(const CString& strData) const
{
	CString strNewData = strData;
	strNewData.TrimLeft(" ");
	strNewData.TrimRight(" ");
	strNewData.TrimRight(",");

	return strNewData;
}

// ---------------------------------------------------------------------------------------
// TR ���� count �� type
bool CGraphImp::GetRequestTRCountAndType(int& nRequestTRCount, bool& bIsNew) const
{
	CPacket* pPacket = m_subGraphList.GetHeadPacket();
	if(IsSendMessage_NewTRData(pPacket))
	{
		return GetRequestTRCountAndType_New(nRequestTRCount, bIsNew);
	}
	
	if(IsSendMessage_InsertTRData(pPacket))
	{
		return GetRequestTRCountAndType_Insert(pPacket, nRequestTRCount, bIsNew);
	}

	return false;
}

bool CGraphImp::GetRequestTRCountAndType_New(int& nRequestTRCount, bool& bIsNew) const
{
	CPacketList* pPacketList = GetPacketList();
	if(pPacketList == NULL)
		return false;

	bIsNew = true;
	nRequestTRCount = pPacketList->GetTotalTRCount();
	return true;
}

bool CGraphImp::GetRequestTRCountAndType_Insert(const CPacket* pPacket, int& nRequestTRCount, bool& bIsNew) const
{
	if(pPacket == NULL)
		return false;

	bIsNew = false; //(nDataCount > 0 ? false: true);
	nRequestTRCount = 0;

	//count ���ϱ�
	CPacketList* pPacketList = GetPacketList();
	int nDataCount = pPacket->GetCount();
	if(!IsSendMessage_InsertTRData(pPacketList, nDataCount))
		return false;

	nRequestTRCount = pPacketList->GetRequestTRCount(nDataCount);
	return (nRequestTRCount > 0);
}

/*
// ---------------------------------------------------------------------------------------
void CGraphImp::ChangeGraphData(const CGraphData& sourceGraphData, const bool bIsAutoCalCheck)
{
	// �Լ���, packet ���� �ٲ������ Ȯ���Ͽ� �ٲ� ��� ��ǥ��� ���� üũ.
	bool bIsCalculate = false;
	if(bIsAutoCalCheck)
		bIsCalculate = !IsEqualDataForCalculate(sourceGraphData);

	// �Լ����� �ٲ� ���� CIndicatorInfo* �� ��������� ��
	SetGraphNameAndSubGraphNames(sourceGraphData.GetGraphName(), false, false);

	// graphData ����
	m_graphData = sourceGraphData;

	// ��ǥ ���
	if(bIsCalculate)
		CalculateGraph(CCalculateBaseData::TRDATA_CAL);
}
*/

/*
// subGraphData�� ��� �ٲٱ�.
// -> bIsCheckSubGraphCount = true : Info�� �ִ� subGraph ���� ��.
//							  false : Info�� �ִ� �������� ���� Make.
bool CGraphImp::ChangeAllSubGraphData(const bool bIsCheckSubGraphCount, const bool bIsAllChangeSubGraphData)
{
	if(m_pIndiInfo == NULL)
		return false;

	if(bIsCheckSubGraphCount)
	{
		// subGraph count�� ���� ��� -> subGraphData �� ����
		if(m_subGraphList.GetSubGraphCount() == m_pIndiInfo->GetSubGraphCount())
			return ChangeAllSubGraphDataFromIndicatorInfo(m_pIndiInfo, bIsAllChangeSubGraphData);
	}

	return MakeAllSubGraphFromIndicatorInfo(false);
}
*/

// IndicatorInfo�� �ִ� subGraphData�� �ٲٱ�.
bool CGraphImp::ChangeAllSubGraphDataFromIndicatorInfo(const CIndicatorInfo* pIndicatorInfo, const bool bIsAllChangeSubGraphData)
{
	if(pIndicatorInfo == NULL)
		return false;

	CList<CSubGraphData*, CSubGraphData*> sourceSubGraphDataList;
	if(!pIndicatorInfo->GetAllSubGraphDataList(sourceSubGraphDataList))
		return false;

	return m_subGraphList.ChangeAllSubGraphData(sourceSubGraphDataList, bIsAllChangeSubGraphData);
}

// subGraph�� �ִ� subGraphData�� IndicatorInfo�� ����.
bool CGraphImp::ChangeIndicatorInfoFromAllSubGraphData(CIndicatorInfo& indicatorInfo)
{
	CList<CSubGraphData*, CSubGraphData*> sourceSubGraphDataList;
	if(!m_subGraphList.GetAllSubGraphDataList(sourceSubGraphDataList))
		return false;

	return indicatorInfo.SetAllSubGraphDataList(&sourceSubGraphDataList);
}

void CGraphImp::ChangeTitleDraw(CDrawingGraphData* pDrawingGraphData, bool& bOldConditionTitleDraw, bool& bOldDataTitleDraw)
{
	bOldConditionTitleDraw = pDrawingGraphData->DoesConditionTitleDraw();
	bOldDataTitleDraw = pDrawingGraphData->DoesDataTitleDraw();

	if(bOldConditionTitleDraw)
		pDrawingGraphData->SetConditionTitleDraw( g_iMetaTable.IsConditionTitle( m_graphData.GetIndicatorName2()));

	// data �Ⱦ���.
	// "������Ʈ", "�ŷ�����Ʈ", "�׹���Ʈ", "Box Chart", "��������Ʈ", "Zig Zag", "���Ź�", 
	// GRAPHTYPE == DataView_Type(����Ÿǥ����) �� ���
	if(bOldDataTitleDraw)
	{
		if( !g_iMetaTable.IsDataTitle( m_graphData.GetIndicatorName2())
			|| m_subGraphList.IsViewableGraphType( CGraphBaseData::DataView_Type, 0))
				pDrawingGraphData->SetDataTitleDraw(false);
	}
}

// IndicatorInfo�� packetName ����.
bool CGraphImp::ChangePacketNameInIndicatorInfo(const CString& strPacketName)
{
	if(m_pIndiInfo == NULL)
		return false;

	// common �κ� ����
	bool bResult = m_pIndiInfo->SetCommonPacketNames(strPacketName);
	// subGraph �κ� ��� ����
	if(m_pIndiInfo->SetAllPacketNames_SubGraphPart(strPacketName, true))
		bResult = true;

	return bResult;
}

bool CGraphImp::ChangeIndicatorInfo(const CIndicatorInfo* pSourceIndicatorInfo)
{
	if(m_pIndiInfo == NULL || pSourceIndicatorInfo == NULL || pSourceIndicatorInfo == m_pIndiInfo)
		return false;

	return m_pIndiInfo->GetCopyDataFrom(pSourceIndicatorInfo);
}

// CPacketRQ*�� Graph üũ : �������� - ojtaso (20070109)
BOOL CGraphImp::ComparePacketRQ(LPVOID pPacketRQ)
{
	if(!m_pPacketRQ)
		return FALSE;

	return !::lstrcmp(m_pPacketRQ->GetRQ(), ((CPacketRQ*)pPacketRQ)->GetRQ());
}

// CPacketRQ*�� RQ : �������� - ojtaso (20070109)
LPCTSTR CGraphImp::GetGraphRQ() const
{
	if(!m_pPacketRQ)
		return _T("DEFAULT");
	
	return m_pPacketRQ->GetRQ();
}

// RQ ���� : �������� - ojtaso (200705015)
void CGraphImp::SetGraphRQ(LPCTSTR lpszRQ)
{
	if(!m_pPacketRQ)
	{
		m_pPacketRQ = (CPacketRQ*)m_pMainBlockBaseData->GetMainAttributes().GetPacketListManager()->GetPacketRQ(lpszRQ);
		if(m_pPacketRQ)
		{
			m_pPacketRQ->AddRQGraph(this);
	//		m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->SelectedGraph(lpszRQ);
	
	//		m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl()->CalculateAllGraphs(CCalculateBaseData::CALCULATETYPE::TRDATA_CAL);
	//		CalculateGraph(CCalculateBaseData::CALCULATETYPE::TRDATA_CAL, true, false, true);
		}
	}
}

//{{ (2007/5/28 - LYH) For Weighted Volume Chart
void CGraphImp::CheckWeightedVolume()
{
	if( 0 < m_subGraphList.GetSubGraphCount())
	{
		CSubGraph *pSubGraph = m_subGraphList.GetSubGraph( 0);
		if( pSubGraph)
		{
			CSubGraphData &subGraphData = pSubGraph->GetSubGraphData();
			if( subGraphData.GetType() == CGraphBaseData::Bong_Type)
				m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl()->SetWeightedVolume( 
					subGraphData.GetStyle().GetGraphBongStyle());
		}
	}
}
//}}

// CPacketRQ�� �˷��� : �������� - ojtaso (20070531)
CPacketRQ* CGraphImp::GetPacketRQ()
{
	return m_pPacketRQ;
}

// �ѹ��� �׷��� �Ӽ� ���� ������ �޽�� �߰� - ojtaso (20080602)
void CGraphImp::ChangeGraphPropertyAtOnce( short nSubGraphIndex, short nGraphType, short nGraphStyle, short nDrawStyle, short nThickness1, short nThickness2, short nThickness3) 
{
	if(!m_pIndiInfo)
		return;

	CSubGraphData *pIndiData = m_pIndiInfo->GetSubGraphData( nSubGraphIndex);
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData( nSubGraphIndex);

	if(pIndiData)
	{
		if(nGraphType >= 0)
		{
			pIndiData->SetType((CGraphBaseData::GRAPHTYPE)nGraphType);
			pGraphData->SetType((CGraphBaseData::GRAPHTYPE)nGraphType);
		}

		if(nGraphStyle >= 0)
		{
			pIndiData->SetStyle(nGraphStyle);
			pGraphData->SetStyle(nGraphStyle);
		}
	
		if(nDrawStyle >= 0)
		{
			pIndiData->SetDrawStyle(nDrawStyle);
			pGraphData->SetDrawStyle(nDrawStyle);
		}

		CPenThickness PenThickness;
		PenThickness = pIndiData->GetPenThickness();

		if(nThickness1 >= 0)
			PenThickness.SetThickness1(nThickness1);
		if(nThickness2 >= 0)
			PenThickness.SetThickness2(nThickness2);
		if(nThickness3 >= 0)
			PenThickness.SetThickness3(nThickness3);

		pIndiData->SetPenThickness(PenThickness);
		pGraphData->SetPenThickness(PenThickness);
	}
}

// �ѹ��� �׷��� �Ӽ� ���� �� �޽�� �߰� - (@Solomon-20100120)
bool CGraphImp::GetGraphPropertyAtOnce( short nSubGraphIndex, short &nGraphType, short &nGraphStyle, short &nDrawStyle, short &nThickness1, short &nThickness2, short &nThickness3) 
{
	if(!m_pIndiInfo)
		return false;
	
	CSubGraphData *pIndiData = m_pIndiInfo->GetSubGraphData( nSubGraphIndex);
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData( nSubGraphIndex);
	
	if(pIndiData)
	{
		nGraphType = pIndiData->GetType();
// 		if(nGraphType >= 0)
// 		{
// 			pIndiData->SetType((CGraphBaseData::GRAPHTYPE)nGraphType);
// 			pGraphData->SetType((CGraphBaseData::GRAPHTYPE)nGraphType);
// 		}

		nGraphStyle = pIndiData->GetStyle().GetGraphStyle();
// 		if(nGraphStyle >= 0)
// 		{
// 			pIndiData->SetStyle(nGraphStyle);
// 			pGraphData->SetStyle(nGraphStyle);
// 		}

		nDrawStyle = pIndiData->GetDrawStyle().GetDrawStyle();
// 		if(nDrawStyle >= 0)
// 		{
// 			pIndiData->SetDrawStyle(nDrawStyle);
// 			pGraphData->SetDrawStyle(nDrawStyle);
// 		}
		
		CPenThickness PenThickness;
		PenThickness = pIndiData->GetPenThickness();
		
// 		if(nThickness1 >= 0)
// 			PenThickness.SetThickness1(nThickness1);
// 		if(nThickness2 >= 0)
// 			PenThickness.SetThickness2(nThickness2);
// 		if(nThickness3 >= 0)
// 			PenThickness.SetThickness3(nThickness3);
// 		
// 		pIndiData->SetPenThickness(PenThickness);
// 		pGraphData->SetPenThickness(PenThickness);
		nThickness1 = PenThickness.GetThickness1();
		nThickness2 = PenThickness.GetThickness2();
		nThickness3 = PenThickness.GetThickness3();

		return true;
	}

	return false;
}

// 20081007 JS.Kim	����ڽð��� ����
LPCTSTR CGraphImp::GetPacketTimeDiffName()
{
	CPacketList* ppacketList = GetPacketList();
	return ppacketList->GetBaseTimeName();
}

int CGraphImp::GetPacketOrgBaseTimeDiff()
{
	CPacketList* ppacketList = GetPacketList();
	return ppacketList->GetOrgBaseTimeDifference();
}

void CGraphImp::SetPacketTimeDiff(CString strTimeName, int nTimeDiff)
{
	CPacketList* pPacketList = GetPacketList();

	if( strTimeName.IsEmpty() )
		pPacketList->SetOrgBaseTime();		// ���� �ð����...
	else
		pPacketList->SetBaseTime(strTimeName, nTimeDiff);

	if( pPacketList && CScaleBaseData::HORZ_TICK == pPacketList->GetDateUnitType() )
	{
		CString strRQ = m_pPacketRQ->GetRQ();
		if( strRQ.Right( 5) == "_PAIR")	strRQ = strRQ.Left( strRQ.GetLength() - 5);
		else							strRQ = strRQ + "_PAIR";

		CPacketList* pPairPacketList = m_pMainBlockBaseData->GetPacketListManager()->GetPacketList(strRQ);
		if( pPairPacketList )
		{
			if( strTimeName.IsEmpty() )		pPairPacketList->SetOrgBaseTime();		// ���� �ð����...
			else							pPairPacketList->SetBaseTime(strTimeName, nTimeDiff);
		}
	}

	m_pMainBlockBaseData->GetXScaleManager()->Calc_DRI();
	CalculateGraph( CCalculateBaseData::TRDATA_CAL);
}

// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
long CGraphImp::AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	// ������Ʈ SubGraph�� �Ѱ�
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData(0);
	if(!pGraphData) return -1;

	return pGraphData->AddHorizontalLine(lpszTitle, clrLine, nLine, dPrice);
}

// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
void CGraphImp::DeleteHorizontalLine(long nIndex)
{
	// ������Ʈ SubGraph�� �Ѱ�
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData(0);
	if(!pGraphData) return;

	pGraphData->DeleteHorizontalLine(nIndex);
}

// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
void CGraphImp::DeleteAllHorizontalLine()
{
	// ������Ʈ SubGraph�� �Ѱ�
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData(0);
	if(!pGraphData) return;

	pGraphData->DeleteAllHorizontalLine();
}

// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
void CGraphImp::UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	// ������Ʈ SubGraph�� �Ѱ�
	CSubGraphData *pGraphData = m_subGraphList.GetSubGraphData(0);
	if(!pGraphData) return;

	pGraphData->UpdateHorizontalLine(nIndex, lpszTitle, clrLine, nLine, dPrice);
}
