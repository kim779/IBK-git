// GetIndicatorInfo.cpp: implementation of the CGetIndicatorInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetIndicatorInfo.h"

#include "ChartFormulateData.h"
#include "ChartFormlateBaseData.h"
#include "Conversion.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
//strOriginal�� �Ǹ����� �κп� strCompare�� �ִ��� üũ�ϰ� ������ strOriginal�� strCompare�� Add.	
void CGetIndicatorInfo::AddCompareStringInOriginalString(CString &strOriginal, const CString &strCompare)
{
	if(strOriginal.Right(1) != strCompare) 
		strOriginal += strCompare;
}

//�Ӽ�Dll���� Condition����Ʈ ���	
void CGetIndicatorInfo::GetConditionList(CIndicatorInfo* pIndicatorInfo, CList<double, double>& ConditionList)
{
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	if(pConditionList == NULL)
		return;

	POSITION pos = pConditionList->GetHeadPosition();
	while(pos) {
		double data = pConditionList->GetNext(pos);
		ConditionList.AddTail(data);
	}
}

//strPacketName�� List Ÿ������ �ٲٱ�
void CGetIndicatorInfo::ChangeTypeFromCStringToCList(const CString& strPacketName, CList<CString, CString>& PacketNameList)
{
	CString packetname = strPacketName;
	AddCompareStringInOriginalString(packetname, ",");
	while(packetname.GetLength()) {
		CString orgStr = CDataConversion::GetStringData(packetname, ",");
		orgStr.TrimLeft();
		orgStr.TrimRight();
		PacketNameList.AddTail(orgStr);
	}
}


//�Ӽ�Dll���� PacketName����Ʈ ���
void CGetIndicatorInfo::GetPacketNameList(CIndicatorInfo* pIndicatorInfo, CList<CString, CString>& PacketNameList)
{
	CString strPacketNames = pIndicatorInfo->GetCommonPacketNames();
	if(strPacketNames.IsEmpty())
		return;

	while(!strPacketNames.IsEmpty()) {
		CString strData = CDataConversion::GetStringData(strPacketNames, ",");
		if(strData.IsEmpty() && !strPacketNames.IsEmpty()){
			strData = strPacketNames;
			strPacketNames.Empty();
		}

		PacketNameList.AddTail(strData);
	}

}

int CGetIndicatorInfo::GetSubGraphIndex(CIndicatorInfo* pIndicatorinfo, const CString& subgraphname)
{
	if(pIndicatorinfo == NULL)
		return -1;

	return pIndicatorinfo->GetSubGraphIndex(subgraphname);
}

void CGetIndicatorInfo::GetSubGraphNameList(CIndicatorInfo* pIndicatorInfo, CList<CString, CString>& subGraphNameList)
{
	if(pIndicatorInfo == NULL)
		return;

	pIndicatorInfo->GetSubGraphNameList(subGraphNameList);
}

/*
void CGetIndicatorInfo::GetSubGraphNameList(const CString& strGraphName, CIndicatorList* pIndicatorList, CList<CString, CString>& subGraphNameList)
{
	//subGraphName �� cfg ������ �������� ���ɼ� ������ default ���� �����´�.
 	CIndicatorInfo* pIndicatorInfo = pIndicatorList->GetIndicatorInfo( EILT_SITE_DEFAULT, strGraphName);
	GetSubGraphNameList(pIndicatorInfo, subGraphNameList);
}
*/