// MainChartFormulateImplementation.cpp: implementation of the CMainChartFormulateImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainChartFormulateImplementation.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "ChartFormulateData.h"
#include "PacketList.h"
#include "Conversion.h"
#include "Indicator.h"
#include "IndicatorList.h"
#include "IndicatorFactory.h"
#include "GetIndicatorInfo.h"
#include "FindCalculatedPacket.h"
#include "CalculateData.h"
#include "PacketPointerList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainChartFormulateImplementation::CMainChartFormulateImplementation()
{
}

CMainChartFormulateImplementation::~CMainChartFormulateImplementation()
{
}

//main���� ȣ��.
bool CMainChartFormulateImplementation::CalculateIndicator(const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo*& pIndicatorInfo)
{
	CString strGraphName = chartFormulateData.GetGraphName();
	CString strIndicatorName = chartFormulateData.GetIndicatorName();

	if( graphCalculateData == NULL || !IsThisDaegiMemul( strIndicatorName, chartFormulateData.GetCalculateType()))
		return false;
	
	bool bGetInfo = false;
	if(pIndicatorInfo == NULL)
	{
		bGetInfo = true;
		pIndicatorInfo = chartFormulateData.GetIndicatorList()->GetGraphIndicatorInfo( EILT_USER, strGraphName, strIndicatorName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	}

	// (2006/9/6 - Seung-Won, Bae) Indicator for Graph
	CIndicator *&pIndicator = graphCalculateData->GetIndicator();
	if( pIndicator) if( pIndicator->GetID() != g_iMetaTable.GetIndicatorID( strIndicatorName))
	{
		delete pIndicator;
		pIndicator = NULL;
	}
	if( !pIndicator)
	{
		IMetaTable::EDDC_INDICATOR_ID eID = ( IMetaTable::EDDC_INDICATOR_ID)g_iMetaTable.GetIndicatorID( strIndicatorName);
		if( g_iMetaTable.IsSTIndexIndicator( eID)) 
			pIndicator = m_indiSingleton.CreateSTIndexCalculator( eID);
		else 
			pIndicator = m_indiSingleton.Create( eID, chartFormulateData.GetOcxHwnd());
	}
	if( !pIndicator) return bGetInfo;	//����� �ʿ���� ��ǥ�� ���.

	return Calculate(pIndicator, chartFormulateData, graphCalculateData, pIndicatorInfo);
}

bool CMainChartFormulateImplementation::Calculate(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	if(pIndicator == NULL)
		return false;

	m_CalculateData.SetPoint(GetDataPoint(chartFormulateData, graphCalculateData));
	if(m_CalculateData.GetPoint().x < 0)
		return false;
	
	if(IsCalculateSubGraph( chartFormulateData.GetIndicatorName()))
		return Calculate_SubGraph(pIndicator, chartFormulateData, graphCalculateData, pIndicatorInfo);

	return Calculate_Graph(pIndicator, chartFormulateData, graphCalculateData, pIndicatorInfo);
}

// graph �� ��� (��� subgraph�� ����Ѵ�.)
bool CMainChartFormulateImplementation::Calculate_Graph(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	assert(pIndicator != NULL);

	pIndicator->GetData(chartFormulateData, graphCalculateData, pIndicatorInfo, m_CalculateData);
	if(graphCalculateData->GetCount() < 0)
		return false;
	return (graphCalculateData->GetHead()->GetHeadPacket() != NULL);
}

// subgraph �� ��� (subgraph�� �ϳ��� ����Ѵ�.)
bool CMainChartFormulateImplementation::Calculate_SubGraph(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	assert(pIndicator != NULL);

	int nCount = CDataConversion::GetStringDataCount(chartFormulateData.GetSubGraphNames(), ",");
	if(nCount <= 0)
		return false;

	CString strSubGraphNames = chartFormulateData.GetSubGraphNames();
	for(int nIndex = 0 ; nIndex < nCount; nIndex++){
		CString strSubGraphName = CDataConversion::GetStringData(strSubGraphNames, ",");
		if(strSubGraphName.IsEmpty())
			return false;
		m_CalculateData.SetPacketNamesForSave(strSubGraphName);
		pIndicator->GetData(chartFormulateData, graphCalculateData, pIndicatorInfo, m_CalculateData);
	}
	return true;
}

CPoint CMainChartFormulateImplementation::GetDataPoint(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(pPacketList == NULL)
		return (-1, -1);

	CPoint point = pPacketList->GetUpdateRange();
	CCalculateBaseData::CALCULATETYPE timetype = chartFormulateData.GetCalculateType();
	if(timetype == CCalculateBaseData::REALDATA_CAL) 
		GetDataPointWhenREALAPPEND(pPacketList, pGraphCalculateData, point);
	else if(timetype == CCalculateBaseData::TRDATA_CAL && point.x != 0
		|| timetype == CCalculateBaseData::SCROLL_CAL)
	{
		point.x = 0;
		point.y = pPacketList->GetMaxCount();
	}

	//InsertType�� new�ε� �������� üũ�Ǿ��ִ� Graph�� ������ �������ȿ� ���� Data�� ����ϰ� �����ϱ� ���ؼ��� 
	//�߰����� ����ϰ� �ϰ� �; ������ ���� ������ Insert���� Append ���� �˼��� ���� ������ 
	//ó������ ����Ҽ� �ۿ� ����.
	//÷���� �ٽ� ����ؾ� �Ѵ�. ����� �����Ҷ� packet�� ������ �ִ� Insert Ÿ���� New�̱� ������¡..
	//�ŸŸ���Ʈ, �ý��� Ʈ���̴��� ���� ����.
/*	if(point.x == -1 || point.y == -1 || bSaveFlag == false) {
//		CDataList* pDataList = ptrDataList.GetAt(ptrDataList.FindIndex(0));
		CDataList* pDataList = ptrDataList.GetHead();
		point.x = 0;
		point.y = pDataList->GetCount();
	}*/
	return point;
}

void CMainChartFormulateImplementation::GetDataPointWhenREALAPPEND(CPacketList* pPacketList,
																   CGraphCalculateData* pGraphCalculateData,
																   CPoint& point)
{
	if(pGraphCalculateData->GetCount() < 0)
		return ;
// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
	CSubGraphPacketList* pSubGraphList = NULL;

	// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
	CIndicator *pIndicator = pGraphCalculateData->GetIndicator();
	if( pIndicator) switch( pIndicator->GetID())
	{
		case IMetaTable::EDDC_OCX_PRICEMACHART:
		case IMetaTable::EDDC_OCX_VOLUMEMACHART:
			pSubGraphList = pGraphCalculateData->GetAt( ( ( CMA *)pIndicator)->GetFirstVisibleSubGraph());
			break;
	}

	if( !pSubGraphList) pSubGraphList = pGraphCalculateData->GetHead();

	CPacket* pPacket = NULL;
	if( pSubGraphList) pPacket = pSubGraphList->GetHeadPacket();
	if(pPacket == NULL || pPacket->GetCount() <= 0)
	{
		point.x = 0;
		point.y = pPacketList->GetMaxCount();
	}
	//�׸��� �׷����� ���� �ʾ����� �ǽð����� �޾Ҵ� Data�� ����� ���� �ʴ´�.
	//�ٽ� ȭ���� ������� ������ �ʾҴ� ������ ����϶�� �Ѿ���� �ʴ´�.
	//���⼭ ������ �ʾҴ� ������ ����ϱ� ���� ����ؾ� �� ������ �ٽ� ��´�.
	else if(pPacket != NULL) {
		int nCalculatedDataCount = pPacket->GetCount();
		if(nCalculatedDataCount != point.x)
			point.x = (nCalculatedDataCount - 1);
	}
}

/*
void CMainChartFormulateImplementation::Initialize_Packet(const CString& strGraphName)
{
//	CIndicator* pIndicator = m_indiSingleton.Find(strGraphName);
//	if(pIndicator == NULL)
//		return;

//	pIndicator->Initialize_Packet();
}

// ���ľȿ� �ִ� packet ���� �����Ѵ�.


void CMainChartFormulateImplementation::DeleteDataInPacket(CPacketList* pPacketList, const CString& strGraphName)
{
//	if(pPacketList == NULL || strGraphName.IsEmpty())
//		return;

//	CIndicator* pIndicator = m_indiSingleton.Find(strGraphName);
//	if(pIndicator != NULL)
//		pIndicator->DeleteDataInPacket(pPacketList);
}
*/

/*
void CMainChartFormulateImplementation::DeletePacket(const CString& strGraphName, const CString& strSubGraphName,
			CPacketList* pPacketList, CIndicatorList* pIndicatorList)
{
	if(pPacketList == NULL || pIndicatorList == NULL) 
		return ;

	CList<double, double> ConditionList;
	CList<CString, CString> SubGraphNameList;
	CList<CString, CString> PacketNameList;
	CString strPacketName = "";
	CString strNewSubGraphName = strSubGraphName;

	CGetIndicatorInfo::GetIndicatorInfoFromIndicatorConditionDLL(strPacketName, strGraphName, pIndicatorList, ConditionList, PacketNameList, SubGraphNameList); 
	//�������� ��ǥ�ǰ�� PacketName�� �����.
//	CMakePacketName::MakePacketNameWhenExceptionalIndicator(strGraphName, strPacketName, ConditionList, strNewSubGraphName, SubGraphNameList); 

	if(strNewSubGraphName.Right(1) != ",")
		strNewSubGraphName += ",";

	int nCount = CDataConversion::GetStringDataCount(strNewSubGraphName, ",");
	for(int i = 0 ; i < nCount ; i++) {
		pPacketList->ClearIndicatorData(CDataConversion::GetStringData(strNewSubGraphName, ","));
	}
}
*/

// scroll �̵��� ����� ������ ����
// "���Ź�"�ø� ����Ѵ�.
bool CMainChartFormulateImplementation::IsThisDaegiMemul(const CString &strGraphName, const CCalculateBaseData::CALCULATETYPE calculatetype)
{
	if(calculatetype != CCalculateBaseData::SCROLL_CAL)
		return true;

	// (2006.12.06) Sunghwan Ahn
	//	return (strGraphName.Find( _MTEXT( C2_OVERHANGING_SUPPLY)) >= 0);
	return g_iMetaTable.IsVolumeForSale( strGraphName) ? true : false;
}

// subGraph ���� ��������� ����
// "���� �̵����", "�ŷ��� �̵����", "�̰ݵ�"
bool CMainChartFormulateImplementation::IsCalculateSubGraph(const CString& strIndicatorName) const
{
	if( strIndicatorName.IsEmpty()) return false;

	return (strIndicatorName == _MTEXT( C2_PRICE_MA) || strIndicatorName == _MTEXT( C2_VOLUME_MA));
}
