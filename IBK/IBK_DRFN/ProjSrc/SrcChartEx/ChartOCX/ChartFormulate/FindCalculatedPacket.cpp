// FindCalculatedPacket.cpp: implementation of the CFindCalculatedPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "chartformulate.h"
#include "FindCalculatedPacket.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "PacketList.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//PacketDll�� ��û�� Graph�� ���� ����Data�� ����Ǿ� �ִ��� Ȯ��.
//strSubGraphName�� data�� ������ IsSubGraphIntoPacketListWhenThereisSubGraphName()ȣ��.
//strSubGraphName�� data�� ������ IsSubGraphIntoPacketListWhenThereisnotSubGraphName()ȣ��.
bool CFindCalculatedPacket::IsSubGraphIntoPacketList(const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
	IsSubGraphIntoPacketListWhenThereisSubGraphName(chartFormulateData, CalculateData);
	if(chartFormulateData.GetCalculateType() != CCalculateBaseData::TRDATA_CAL || chartFormulateData.GetSubGraphNames().GetLength() > 0)
		return false;
	else
		return true;
}

//strSubGraphName�� �������� Packet�� �ִ��� Ȯ��.
void CFindCalculatedPacket::IsSubGraphIntoPacketListWhenThereisSubGraphName(const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
	int nIndex = CDataConversion::GetStringDataCount(chartFormulateData.GetSubGraphNames(), ",");
	if(nIndex == 0)
		return ;
	CString orgStr = CDataConversion::GetStringData(chartFormulateData.GetSubGraphNames(), ",");
	orgStr.TrimLeft();
	orgStr.TrimRight();
	IsSubGraphIntoPacketListbyNEWandINSERTandAPPEND(orgStr, chartFormulateData, CalculateData);
}



//New, Insert, Append�� ���� �ٽð���ؾ��ϴ��� �ƴ����� ���� ��찡 �ٸ�.
//packet�� Ÿ���� Append�� ��� packet(New�����϶� ����ߴ� Data)�� �ִ��� ����� �ٽ� �ؾ��Ѵ�.
void CFindCalculatedPacket::IsSubGraphIntoPacketListbyNEWandINSERTandAPPEND(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
//25
	if(strSubGraphName.CompareNoCase( _MTEXT( C3_BIG_NEWS)) == 0 || strSubGraphName.CompareNoCase( _MTEXT( C3_LOCK)) == 0) 
		WhenPacketIsStringData(strSubGraphName, chartFormulateData, CalculateData);
	else
		WhenPacketIsIntData(strSubGraphName, chartFormulateData, CalculateData);
}


//nStandardCount = maxcount
//nMycount = �� ����Ʈ ī��Ʈ
//���ེ�Ұ� ���ེ���� ��� �׳� count�� ���ϸ� ��� �ٽ� ����ϰ� �ȴ�.
//Append�� ���, ��) datacount�� 26�϶�..
//max = 26 , index = 25, nStandrdcount = 26
//nMycount = 1, ���� = 26
//�̷� ���� true����.
bool CFindCalculatedPacket::IsFinishCalulationWhenNew(const CString &strSubGraphName, 
													  const int nStandardCount, const int nMyCount, 
													  CList<double, double>& ConditionList)
{
	if(strSubGraphName.CompareNoCase( _MTEXT( C3_POST_SPAN)) == 0) {
		if(nStandardCount == (nMyCount + ConditionList.GetAt(ConditionList.FindIndex(1)) - 1))
			return true;
	}
	//������ ����Ÿ�� �����ؼ� 26���ϱ�..���������δ� 25���� ����Ÿ�� ��������.
	else if(strSubGraphName.CompareNoCase( _MTEXT( C3_PRE_SPAN_1)) == 0 || strSubGraphName.CompareNoCase( _MTEXT( C3_PRE_SPAN_2)) == 0 ) {
		if(nStandardCount == (nMyCount - ConditionList.GetAt(ConditionList.FindIndex(1)) - 1))
			return true;
	}
	
//	else if(strSubGraphName == _MTEXT( C2_OVERHANGING_SUPPLY))
// (2006.12.06) Sunghwan Ahn
	else if( g_iMetaTable.IsVolumeForSale(strSubGraphName))
// (2006.12.06) Sunghwan Ahn
	{
		if((nMyCount-2) == ConditionList.GetAt(ConditionList.FindIndex(0)))
			return true;
	}
	else {
		if(nStandardCount == nMyCount)
			return true;
	}
	return false;
}

//�̰� ������� �ʰ� ����.
//���ེ�Ұ� ���ེ���� ��� �׳� count�� ���ϸ� ��� �ٽ� ����ϰ� �ȴ�.
bool CFindCalculatedPacket::IsFinishCalulationWhenInsertAppend(const CString &strSubGraphName, const int nStandardCount, const int nMyCount, CList<double, double>& ConditionList)
{
	if(strSubGraphName.CompareNoCase( _MTEXT( C3_POST_SPAN)) == 0) {
		if(nStandardCount == (nMyCount + ConditionList.GetAt(ConditionList.FindIndex(1))))
			return true;
	}
	else if(strSubGraphName.CompareNoCase( _MTEXT( C3_PRE_SPAN_1)) == 0 || strSubGraphName.CompareNoCase( _MTEXT( C3_PRE_SPAN_2)) == 0 ) {
		if(nStandardCount == (nMyCount - ConditionList.GetAt(ConditionList.FindIndex(1))))
			return true;
	}
	else {
		if(nStandardCount == nMyCount)
			return true;
	}
	return false;
}

void CFindCalculatedPacket::WhenPacketIsStringData(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	CCalculateBaseData::CALCULATETYPE timetype = chartFormulateData.GetCalculateType();

	CList<CString, CString>* pList = pPacketList->GetstrDataList(pPacketList->GetBaseData(strSubGraphName));

	if(timetype == CCalculateBaseData::TRDATA_CAL) {
//		if(pPacketList->GetBaseData(strSubGraphName) == NULL || pList == NULL || pList->GetCount() <= 0) 
//			CalculateData.SetPacketNameForCal(chartFormulateData.GetSubGraphNames());
//		else if(!IsFinishCalulationWhenNew(strSubGraphName, pPacketList->GetMaxCount(), pList->GetCount(), ConditionList))
//			CalculateData.SetPacketNamesForCal(chartFormulateData.GetSubGraphNames());	

	}
	//real�� ������ �ٽ� ����Ѵ�.
//	else if(timetype == CCalculateBaseData::REALDATA_CAL) 
//		CalculateData.SetPacketNameForCal(chartFormulateData.GetSubGraphNames());

}

void CFindCalculatedPacket::WhenPacketIsIntData(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	CCalculateBaseData::CALCULATETYPE timetype = chartFormulateData.GetCalculateType();

	CPacket* pPacket = pPacketList->GetBaseData(strSubGraphName);
	if(pPacket == NULL)
		return;

	CList<double, double>* pList = pPacket->GetnumericDataList();
	if(timetype == CCalculateBaseData::TRDATA_CAL) {
//		if(pPacketList->GetBaseData(strSubGraphName) == NULL) 
//			CalculateData.SetPacketNameForCal(chartFormulateData.GetSubGraphNames());
//		else if(!IsFinishCalulationWhenNew(strSubGraphName, pPacketList->GetMaxCount(), pList->GetCount(), ConditionList))
//			CalculateData.SetPacketNamesForCal(ChartFormulateData.GetSubGraphNames());	
	}
	//real�� ������ �ٽ� ����Ѵ�.
//	else if(timetype == CCalculateBaseData::REALDATA_CAL) 
//		CalculateData.SetPacketNameForCal(chartFormulateData.GetSubGraphNames());

}
