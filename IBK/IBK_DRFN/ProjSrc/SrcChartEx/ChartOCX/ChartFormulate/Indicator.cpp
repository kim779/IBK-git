// Indicator.cpp: implementation of the CIndicator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Indicator.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/ChartFormulateData.h"	// for CChartFormulateData
#include "PacketList.h"
#include "DataMath.h"
#include "GraphData.h"
#include "Conversion.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "GetIndicatorInfo.h"
#include "ChartFormlateBaseData.h"
#include "PacketPointerList.h"

#define RED 1
#define BLUE 2

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndicator::CIndicator( IMetaTable::EDDC_INDICATOR_ID p_eID)
{
	m_eID = p_eID;
}

CIndicator::~CIndicator()
{
}

void CIndicator::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);

		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pSubGraphList) return;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if( !pPacket) pPacket->ClearData();
	}
}

void CIndicator::SetData(const CPoint &point, const int nUpdateIndex, const double dData, CList<double, double>* pData)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pData) return;

	if(point.x == 0)
		pData->AddTail(dData);
	else
	{
		POSITION pos = FindPosition(nUpdateIndex, *pData);
	 	if(!pos)
			pData->AddTail(dData);
		else
			pData->SetAt(pos, dData);
	}
}

void CIndicator::SetData(const CPoint &point, const int nUpdateIndex, const CString& strData, CPacket* pPacket)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacket) return;

	if(point.x == 0)
		pPacket->AppendData(strData);
	else
		pPacket->UpdateData(strData, nUpdateIndex, false);
}


void CIndicator::SetData(const CPoint &point, const int nUpdateIndex, const double dData, CPacket* pPacket)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacket) return;

	if(point.x == 0)
		pPacket->AppendData(dData);
	else
		pPacket->UpdateData(dData, nUpdateIndex, false);
}

bool CIndicator::SeachCalForPacketName(const CString& calpacketname, const CString& packetname)
{
	return (calpacketname.Find(packetname) >= 0);
}

bool CIndicator::ClearPacketData(CPacket *pPacket)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacket) return FALSE;

	if( pPacket->GetCount() <= 0) return false;

	return (pPacket->ClearData() == 1 ? true: false);
}

CPacket* CIndicator::MakePacket_HighLowClose(CPacketList* pPacketList, const CCalculateBaseData::CALCULATETYPE eCalType, const CPoint& point)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if(!pPacketList) return NULL;

	CPacket* newpacket = pPacketList->GetBaseData( _MTEXT( C0_COMBINE_HIGH_LOW_CLOSE), false);
	if(!newpacket) return NULL;

	//������ ���Ǿ� �ִٸ� �ٽ� ����ϴ� ������ ������ �ʱ� ���ؼ�
	//�ǽð��϶��� ��� ����ϰ� �д�.
	if(eCalType != CCalculateBaseData::REALDATA_CAL && 
		newpacket->GetCount() != NULL && (pPacketList->GetMaxCount() == newpacket->GetCount()))
		return newpacket;

	CList<double, double>* pEndList = GetnumericDataList(pPacketList->GetBaseData( _MTEXT( C0_CLOSE)));
	CList<double, double>* pHighList = GetnumericDataList(pPacketList->GetBaseData( _MTEXT( C0_HIGH)));
	CList<double, double>* pLowList = GetnumericDataList(pPacketList->GetBaseData( _MTEXT( C0_LOW)));
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndList || !pHighList || !pLowList) return NULL;

	POSITION endpos = FindPosition(point.x, *pEndList);
	POSITION highpos = FindPosition(point.x, *pHighList);
	POSITION lowpos = FindPosition(point.x, *pLowList);

	if(endpos == NULL || highpos == NULL || lowpos == NULL)
		return NULL;

	for(int i = point.x ; i < pEndList->GetCount() ; i++)
	{
		double end = GetNext(pEndList, endpos);
		double high = GetNext(pHighList, highpos);
		double low = GetNext(pLowList, lowpos);

		// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( high < end) high = low = end;

		double data = (end + high + low)/3;
		if(eCalType == CCalculateBaseData::REALDATA_CAL)
			newpacket->UpdateData(data, i, false);
		else
			newpacket->AppendData(data);
	}
	return newpacket;
}

//sy 2005.09.25.
CPacket* CIndicator::MakePacket_HighLow(CPacketList* pPacketList, const CCalculateBaseData::CALCULATETYPE eCalType, const CPoint& point)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if(!pPacketList) return NULL;

	CPacket* newpacket = pPacketList->GetBaseData( _MTEXT( C0_COMBINE_HIGH_LOW), false);
	if(!newpacket) return NULL;

	//������ ���Ǿ� �ִٸ� �ٽ� ����ϴ� ������ ������ �ʱ� ���ؼ�
	//�ǽð��϶��� ��� ����ϰ� �д�.
	if(eCalType != CCalculateBaseData::REALDATA_CAL && 
		newpacket->GetCount() != NULL && (pPacketList->GetMaxCount() == newpacket->GetCount()))
		return newpacket;

	CList<double, double>* pHighList = GetnumericDataList(pPacketList->GetBaseData( _MTEXT( C0_HIGH)));
	CList<double, double>* pLowList = GetnumericDataList(pPacketList->GetBaseData( _MTEXT( C0_LOW)));
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if(!pHighList || !pLowList) return NULL;

	POSITION highpos = FindPosition(point.x, *pHighList);
	POSITION lowpos = FindPosition(point.x, *pLowList);
	if(highpos == NULL || lowpos == NULL)
		return NULL;

	for(int i = point.x ; i < pHighList->GetCount() ; i++)
	{
		double high = GetNext(pHighList, highpos);
		double low = GetNext(pLowList, lowpos);

		double data = (high + low)/2;
		if(eCalType == CCalculateBaseData::REALDATA_CAL)
			newpacket->UpdateData(data, i, false);
		else
			newpacket->AppendData(data);
	}
	return newpacket;
}

void CIndicator::MakeZeroMemory(const int nCount, double *doubleArray)
{
	for(int count = 0; count < nCount; count++) 
		doubleArray[count] = 0;
}

POSITION CIndicator::FindPosition(const int nIndex, const CList<double, double>& pSourceList) const
{
	int nTailIndex = pSourceList.GetCount() -1;
	if(nIndex < 0 || nIndex > nTailIndex)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = pSourceList.GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			pSourceList.GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = pSourceList.GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			pSourceList.GetNext(pos);
		}
	}

	return NULL;
}

POSITION CIndicator::FindPosition(const int nIndex, const CList<CString, CString>& pSourceList) const
{
	int nTailIndex = pSourceList.GetCount() -1;
	if(nIndex < 0 || nIndex > nTailIndex)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = pSourceList.GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			pSourceList.GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = pSourceList.GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			pSourceList.GetNext(pos);
		}
	}

	return NULL;
}
POSITION CIndicator::FindPosition(const int nIndex, const CList<int, int>& pSourceList) const
{
	int nTailIndex = pSourceList.GetCount() -1;
	if(nIndex < 0 || nIndex > nTailIndex)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = pSourceList.GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			pSourceList.GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = pSourceList.GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			pSourceList.GetNext(pos);
		}
	}

	return NULL;
}
//double���� list���� nIndex��°�� Data�� ����
double CIndicator::GetAt(const CList<double, double>* pSourceList, int nIndex)
{
	if(pSourceList == NULL || nIndex < 0) 
		return 0.0;

	POSITION pos = FindPosition(nIndex, *pSourceList);
	if(pos == NULL)
		return 0.0;

	return pSourceList->GetAt(pos);
}
//string���� list���� nIndex��°�� Data�� ����
CString CIndicator::GetAt(const CList<CString, CString>* pSourceList, int nIndex)
{
	if(pSourceList == NULL || nIndex < 0) 
		return "";

	POSITION pos = FindPosition(nIndex, *pSourceList);
	if(pos == NULL)
		return "";

	return pSourceList->GetAt(pos);
}
double CIndicator::GetKindMA(const CChartFormulateData& chartFormulateData, CList<double, double>* pConditionList)
{
	if(pConditionList == NULL || pConditionList->GetCount() <= 0)
		return -1;

	if(pConditionList->GetCount() < 5)
		return pConditionList->GetTail();
	else
	{
		//sy 2004.04.15. -> ���������� ������ 10���� �þ.
		if(chartFormulateData.GetGraphName() == _MTEXT( C2_PRICE_MA))
		{
			return pConditionList->GetTail();
			//return GetAt(pConditionList, 6);
		}
		else if(chartFormulateData.GetGraphName() == _MTEXT( C2_VOLUME_MA))
			return GetAt(pConditionList, 4);
	}
	return -1;
}

//double���� list���� nIndex��°�� Data�� ����
double CIndicator::GetNext(const CList<double, double>* pSourceList, POSITION& pos)
{
	if(pSourceList == NULL || pos == NULL) 
		return 0.0;

	return pSourceList->GetNext(pos);
}
//CString���� list���� nIndex��°�� Data�� ����
CString CIndicator::GetNext(const CList<CString, CString>* pSourceList, POSITION& pos)
{
	if(pSourceList == NULL || pos == NULL) 
		return "";

	return pSourceList->GetNext(pos);
}

//double���� list���� nIndex��°�� Data�� ����
double CIndicator::GetAt(const CList<double, double>* pSourceList, POSITION& pos)
{
	if(pSourceList == NULL || pos == NULL) 
		return 0.0;

	return pSourceList->GetAt(pos);
}
//CString���� list���� nIndex��°�� Data�� ����
CString CIndicator::GetAt(const CList<CString, CString>* pSourceList, POSITION& pos)
{
	if(pSourceList == NULL || pos == NULL) 
		return "";

	return pSourceList->GetAt(pos);
}

//��,ƽ�ϰ�� ������ ����Ÿ �������� ���� date����Ʈ�� �ε����� �����Ѵ�. 
int CIndicator::GetPreEndPrice(CList<double, double>* pDateList, 
							   CScaleBaseData::HORZSCALEDRAWERTYPE nType,
							   int startindex)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pDateList) return NULL;

	int ReturnIndex = 0;
	CString StartDate;
	POSITION Datepos = FindPosition(startindex, *pDateList);
	POSITION pos = FindPosition(startindex, *pDateList);
	if(Datepos == NULL || pos == NULL || nType == CScaleBaseData::HORZ_TICK)
		return 0;
	double date = pDateList->GetAt(Datepos);
	double startdate = pDateList->GetAt(pos);

	CString Date  = CDataConversion::DoubleToString(date, 0);
	CString first = CDataConversion::DoubleToString(startdate, 0);
	if(Date.GetLength() < 6 || first.GetLength() < 6)
		return 0;
	StartDate = Date.Mid(0, Date.GetLength() - 4);
	first = first.Mid(0, Date.GetLength() - 4);
	date = atof(StartDate);
	
	while(Datepos)
	{
		double temp = pDateList->GetPrev(Datepos);
		CString temp1 = CDataConversion::DoubleToString(temp, 0);
		if(temp1.GetLength() < 6) return 0;
		temp1 = temp1.Mid(0, temp1.GetLength() - 4);
		double comparedate = atof(temp1);
		if(comparedate != date)
		{
			return startindex - ReturnIndex;
		}
		ReturnIndex++;
	}
	return 0;
}

//nIndex��°�� Condition�� ����.
double CIndicator::GetCondition(const int& nIndex, CList<double, double>* pConditionList) const
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if(!pConditionList)
		return 0;

	int i=0;
	POSITION pos = pConditionList->GetHeadPosition();
	while(pos) {
		double data = pConditionList->GetNext(pos);
		if(i == nIndex) 
			return data;
		i++;
	}
	return 0;
}

bool CIndicator::IsValidPacket(CPacket* pPacket)
{
	if(pPacket == NULL)
		return false;

	return IsValidPacket(pPacket->GetnumericDataList());
}

bool CIndicator::IsValidPacket(const CList<double, double>* pSourceList)
{
	return (pSourceList != NULL && (pSourceList->GetCount() > 0 ? true : false));
}

CList<double, double>* CIndicator::GetnumericDataList(CPacket *pPacket)
{
	if(pPacket == NULL)
		return NULL;

	return pPacket->GetnumericDataList();
}

//sy 2005.09.21.
CPacket* CIndicator::GetPacket(CPacketList* pPacketList, const CString& strPacketNames, 
		const CCalculateBaseData::CALCULATETYPE eCalType, const CPoint& point)
{
	if(pPacketList == NULL)
		return NULL;

	CString strRealPacketNames = strPacketNames;
	//��+����+���� �� ��������..
	bool bHigh = SeachCalForPacketName(strRealPacketNames, _MTEXT( C0_HIGH));
	bool bLow = SeachCalForPacketName(strRealPacketNames, _MTEXT( C0_LOW));
	bool bClose = SeachCalForPacketName(strRealPacketNames, _MTEXT( C0_CLOSE));
	if(bHigh && bLow)
	{
		if(bClose)
			return MakePacket_HighLowClose(pPacketList, eCalType, point);
		return MakePacket_HighLow(pPacketList, eCalType, point);
	}
	
	//�⺻ packet�� ��������..
	if(strRealPacketNames.Right(1) != ',')
		return pPacketList->GetBaseData(strRealPacketNames, true);
	return pPacketList->GetBaseData(CDataConversion::GetStringData(strRealPacketNames, ","), true);
}
//sy end

CPacket* CIndicator::GetPacket(const CPoint& point, 
							   const CChartFormulateData& chartFormulateData)
{
	return GetPacket(chartFormulateData.GetPacketList(), chartFormulateData.GetPacketNames(), chartFormulateData.GetCalculateType(), point);
}

//�̹� ����� �Ǿ� ����.
//object���� build�� �ٽ� ������.������ �ִ� packet�� �ּҸ� ��� �Ҿ�����ϱ�..
//�ٽ� ������ ��. �׸���, ����� �ٽ� �� �ʿ� ����.
bool CIndicator::IsIndicatorCalculated(const CCalculateBaseData::CALCULATETYPE calculatetype,
									   CPacket* pPacket)
{
	return (calculatetype == CCalculateBaseData::TRDATA_CAL && pPacket->GetCount() > 0);
}

//dSourceCondition�� dSourceList�� �����Ͱ� 0�� �ƴ� ���� �������̴�.
//�����̵���� ���Ҷ� �ǹ̾��� ���� �����ϸ� �ȵȴ�.
void CIndicator::GetSignalData(const CPoint& point,
							   const int nDataCount,
							   const double dSourceCondition,
							   const double dCondition,
							   const CList<double, double>& dSourceList,
							   CPacket* pSignalPacket)
{
	if(pSignalPacket == NULL) return ;
	CList<double, double>* pSignalList = pSignalPacket->GetnumericDataList();
	double dCalData = 0.0;
	int nStart = point.x;
	if(point.x == 0) {	    //new �϶�
		if(dSourceCondition == 0)
			nStart = 0;
		else
			nStart = (int)dSourceCondition - 1;

		//sourcelist�� ���ǰ��� ���� datacount���� Ŭ���� ����� �ȵȴ�.
		if(nStart > nDataCount - 1) {
			for(int i = 0 ; i < nDataCount ; i++)
				SetData(point, i, 0.00, pSignalPacket);
			return;
		}

		//sourcelist�� 0���� �ƴ� ���� ������ ����ϱ� ���ؼ�..
		 for(int i = 0 ; i < nStart ; i++)
			SetData(point, i, 0.00, pSignalPacket);
		
		dCalData = GetAt(&dSourceList, nStart);
	}
	else 
	{
		if(pSignalList == NULL || pSignalList->GetCount() <= 0)
			return;

		dCalData = pSignalPacket->GetData(point.x - 1);
	}

	if ( dCondition <= 1 )  {
          for(int i = nStart; i < nDataCount; i++)
			SetData(point, i, GetAt(&dSourceList, i), pSignalPacket);
    }
	else {
		double D = 2.00 / (dCondition + 1.00); 
		for(int count = nStart ; count < nDataCount; count++) {
			double source;
			//source�� �Ǵ� data�� ÷���� ������ ���Ȱ��
			if(nDataCount == dSourceList.GetCount())
				source = GetAt(&dSourceList, count);
			//source�� �Ǵ� data�� �ǽð��κи� ���Ȱ��
			//data�� 1���� ���Ǿ� �����ϱ�..
			//���� point�� 300�̸�
			//�׳� count�� 300�̰� 300��°�� �����ϰ� �Ǹ�
			//���� ����Ÿ�̹Ƿ� �װų� �߸��� ����Ÿ�� ����ȴ�.
			else 
				source = GetAt(&dSourceList, count - point.x);

			dCalData = (source * D) + ((1.00 - D) * dCalData);
			if((count + 1) >= (dSourceCondition + dCondition))
				SetData(point, count, dCalData, pSignalPacket);
			else
				SetData(point, count, 0.0, pSignalPacket);
		}
	}
}


//dSourceCondition�� dSourceList�� �����Ͱ� 0�� �ƴ� ���� �������̴�.
//�����̵���� ���Ҷ� �ǹ̾��� ���� �����ϸ� �ȵȴ�.
void CIndicator::GetSignalData(const CPoint& point,
							   const int nDataCount,
							   const double dSourceCondition,
							   const double dCondition,
							   CPacket* pSourceData,
							   CPacket* pSignalData)
{
	if(pSignalData == NULL) return ;
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSourceData) return;

	double dCalData = 0.0;
	int nStart = point.x;
	if(point.x == 0) {	    //new �϶�
		if(dSourceCondition == 0)
			nStart = 0;
		else
			nStart = (int)dSourceCondition - 1;

		//sourcelist�� ���ǰ��� ���� datacount���� Ŭ���� ����� �ȵȴ�.
		if(nStart > nDataCount - 1) 
		{
		 for(int i = 0 ; i < nDataCount ; i++)
			pSignalData->AppendData(0.00);
		 return;
		}

		//sourcelist�� 0���� �ƴ� ���� ������ ����ϱ� ���ؼ�..
		for(int i = 0 ; i < nStart ; i++)
			pSignalData->AppendData(0.00);
		
		dCalData = pSourceData->GetData(nStart);
	}
	else {
		if(pSignalData->GetnumericDataList() == NULL || pSignalData->GetCount() <= 0)
			return;

		dCalData = pSignalData->GetData(point.x - 1);
	}

	if ( dCondition <= 1 )  
	{
          for(int i = nStart; i < nDataCount; i++)
			 SetData(point, i, pSourceData->GetData(i), pSignalData);
    }
	else {
		double D = 2.00 / (dCondition + 1.00); 
		for(int count = nStart ; count < nDataCount; count++) {
			double source;
			//source�� �Ǵ� data�� ÷���� ������ ���Ȱ��
			if(nDataCount == pSourceData->GetCount())
				source = pSourceData->GetData(count);
			//source�� �Ǵ� data�� �ǽð��κи� ���Ȱ��
			//data�� 1���� ���Ǿ� �����ϱ�..
			//���� point�� 300�̸�
			//�׳� count�� 300�̰� 300��°�� �����ϰ� �Ǹ�
			//���� ����Ÿ�̹Ƿ� �װų� �߸��� ����Ÿ�� ����ȴ�.
			else 
				source = pSourceData->GetData(count - point.x);

			dCalData = (source * D) + ((1.00 - D) * dCalData);
			if((count + 1) >= (dSourceCondition + dCondition))
				SetData(point, count, dCalData, pSignalData);
			else
				SetData(point, count, 0.0, pSignalData);
		}
	}
}


bool CIndicator::GetSignalPacket(CPacketList* pPacketList, CString& strFuncName, CPacket*& pSignalPacket)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	CString strSignalPacketName = strFuncName + SIGNAL_PACKET;
	pSignalPacket = pPacketList->GetBaseData(strSignalPacketName, false);

	// (2004.06.07, ��¿�) Signal Packet�� GetSignalPacket()���� Clear�Ǹ�,
	//		��ǥ�� �̹� ���� ����� IsIndicatorCalculated()���� �ٷ� Skip�� Signal�� Clear�Ǵ� ������ �߻��Ѵ�.
	//		����, �̰ܱ� �ƴ� Signal�� ���� ����� �� Clear��Ű���� �Ѵ�.
	//ClearPacketData(pSignalPacket);

	return (pSignalPacket != NULL);
}

CList<CString, CString>* CIndicator::GetstrDataList(CPacket *pPacket)
{
	if(pPacket == NULL)
		return NULL;

	return pPacket->GetstrDataList();
}

bool CIndicator::SetGraphPacketList(CPacketList* pPacketList,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo*& pIndicatorInfo,
									CPacket*& pPacket)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	pPacket = NULL;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pPacket, 
			                             pSubGraphList);
		else
		{
			if(pPacket == NULL)
				pPacket = pIndiPacket;
		}
	}
	return (pPacket != NULL);
}

bool CIndicator::SetGraphPacketList(CPacketList* pPacketList,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo*& pIndicatorInfo,
									CPacket*& pPacket, CPacket*& pPacket1)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	pPacket = pPacket1 = NULL;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pPacket,
										 pPacket1, pGraphCalculateData);
		else
		{
			if(pPacket == NULL)
				pPacket = pIndiPacket;
			else if(pPacket != NULL && pPacket1 == NULL)	
				pPacket1 = pIndiPacket;
		}
	}
	return (pPacket != NULL || pPacket1 != NULL);
}

/*
bool CIndicator::SetGraphPacketList(CPacketList* pPacketList,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo*& pIndicatorInfo,
									CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	pPacket = pPacket1 = pPacket2 = NULL;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pPacket, 
										 pPacket1, pPacket2, pGraphCalculateData);
		else
		{
			if(pPacket == NULL)
				pPacket = pIndiPacket;
			else if(pPacket != NULL && pPacket1 == NULL)	
				pPacket1 = pIndiPacket;
			else if(pPacket != NULL && pPacket1 != NULL && pPacket2 == NULL)
				pPacket2 = pIndiPacket;
		}
	}
	return (pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL);
}

bool CIndicator::SetGraphPacketList(CPacketList* pPacketList,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo*& pIndicatorInfo,
									CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
									CPacket*& pPacket3)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	pPacket = pPacket1 = pPacket2 = pPacket3 = NULL;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pPacket, 
										 pPacket1, pPacket2, pPacket3, pGraphCalculateData);
		else
		{
			if(pPacket == NULL)
				pPacket = pIndiPacket;
			else if(pPacket != NULL && pPacket1 == NULL)	
				pPacket1 = pIndiPacket;
			else if(pPacket != NULL && pPacket1 != NULL && pPacket2 == NULL)
				pPacket2 = pIndiPacket;
			else if(pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 == NULL)
				pPacket3 = pIndiPacket;
		}
	}
	return (pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 != NULL);
}

bool CIndicator::SetGraphPacketList(CPacketList* pPacketList,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo*& pIndicatorInfo,
									CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
									CPacket*& pPacket3, CPacket*& pPacket4)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	pPacket = pPacket1 = pPacket2 = pPacket3 = pPacket4 = NULL;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pPacket, pPacket1, 
			                             pPacket2, pPacket3, pPacket4, pGraphCalculateData);
		else
		{
			if(pPacket == NULL)
				pPacket = pIndiPacket;
			else if(pPacket != NULL && pPacket1 == NULL)	
				pPacket1 = pIndiPacket;
			else if(pPacket != NULL && pPacket1 != NULL && pPacket2 == NULL)
				pPacket2 = pIndiPacket;
			else if(pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 == NULL)
				pPacket3 = pIndiPacket;
			else if(pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 != NULL && pPacket4 == NULL)
				pPacket4 = pIndiPacket;
		}
	}
	return (pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 != NULL && pPacket4 != NULL);
}
*/

void CIndicator::SetPacketTypes(CPacket* pPacket, const CString& strUnit, const CString& strType, const int nStartPeriod)
{
	if(pPacket == NULL)
		return;

	pPacket->SetBaseData(strUnit, strType, nStartPeriod);
}

bool CIndicator::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					                   CPacket*& pPacket, CSubGraphPacketList* pSubGraphList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphList) return FALSE;

	if(pIndicatorInfo == NULL)
		return false;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	CString strSubGraphPacketName = pIndicatorInfo->GetSubGraphName( 0);
	if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketName = pIndicatorInfo->GetGraphName() + "_" + pIndicatorInfo->GetSubGraphName( 0);
	pPacket = pPacketList->GetBaseData( strSubGraphPacketName, false);
	pSubGraphList->AddTail2( pPacket);
	return (pPacket != NULL);
}

bool CIndicator::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					                   CPacket*& pPacket, CPacket*& pPacket1, 
					                   CGraphCalculateData* pGraphCalculateData)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(pIndicatorInfo == NULL)
		return false;

	CList<CString, CString> subGraphNameList;
	if(!pIndicatorInfo->GetSubGraphNameList(subGraphNameList))
		return false;

	POSITION pos_sugGraphName = subGraphNameList.GetHeadPosition();

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	CString strSubGraphPacketNameHeader;
	if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

	POSITION pos_graphCalData = pGraphCalculateData->GetHeadPosition();
	while(pos_sugGraphName && pos_graphCalData)
	{
		CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetNext(pos_graphCalData);
		if(pSubGraphPacketList == NULL)
			return false;

		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CPacket* pIndiPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + subGraphNameList.GetNext( pos_sugGraphName), false);
		if(pIndiPacket == NULL)
			return false;

		if(pPacket == NULL)
			pPacket = pIndiPacket;
		else if(pPacket != NULL && pPacket1 == NULL)
			pPacket1 = pIndiPacket;
		else
			break;
		
		pSubGraphPacketList->AddTail2( pIndiPacket);
	}
	return (pPacket != NULL || pPacket1 != NULL);
}

/*
bool CIndicator::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					                   CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
					                   CGraphCalculateData* pGraphCalculateData)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData || !pPacketList) return FALSE;

	if(pIndicatorInfo == NULL)
		return false;

	CList<CString, CString> subGraphNameList;
	if(!pIndicatorInfo->GetSubGraphNameList(subGraphNameList))
		return false;

	POSITION pos_subGraphName = subGraphNameList.GetHeadPosition();
	POSITION pos_graphCalData = pGraphCalculateData->GetHeadPosition();
	while(pos_subGraphName && pos_graphCalData)
	{
		CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetNext(pos_graphCalData);
		if(pSubGraphPacketList == NULL)
			return false;

		CPacket* pIndiPacket = pPacketList->GetBaseData(subGraphNameList.GetNext(pos_subGraphName), false);
		if(pIndiPacket == NULL)
			return false;

		if(pPacket == NULL)
			pPacket = pIndiPacket;
		else if(pPacket != NULL && pPacket1 == NULL)
			pPacket1 = pIndiPacket;
		else if(pPacket != NULL && pPacket1 != NULL && pPacket2 == NULL)
			pPacket2 = pIndiPacket; 
		else
			break;
		
		pSubGraphPacketList->AddTail(pIndiPacket);
	}
	return (pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL);
}

bool CIndicator::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					                   CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
					                   CPacket*& pPacket3, CGraphCalculateData* pGraphCalculateData)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData || !pPacketList) return FALSE;

	if(pIndicatorInfo == NULL)
		return false;

	CList<CString, CString> subGraphNameList;
	if(!pIndicatorInfo->GetSubGraphNameList(subGraphNameList))
		return false;

	POSITION pos_subGraphName = subGraphNameList.GetHeadPosition();
	POSITION pos_graphCalData = pGraphCalculateData->GetHeadPosition();
	while(pos_subGraphName && pos_graphCalData)
	{
		CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetNext(pos_graphCalData);
		if(pSubGraphPacketList == NULL)
			return false;

		CPacket* pIndiPacket = pPacketList->GetBaseData(subGraphNameList.GetNext(pos_subGraphName), false);
		if(pIndiPacket == NULL)
			return false;

		if(pPacket == NULL)
			pPacket = pIndiPacket;
		else if(pPacket != NULL && pPacket1 == NULL)
			pPacket1 = pIndiPacket;
		else if(pPacket != NULL && pPacket1 != NULL && pPacket2 == NULL)
			pPacket2 = pIndiPacket; 
		else if(pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 == NULL)
			pPacket3 = pIndiPacket;
		else
			break;
		
		pSubGraphPacketList->AddTail(pIndiPacket);
	}
	return (pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 != NULL);
}

bool CIndicator::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					                   CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
					                   CPacket*& pPacket3,  CPacket*& pPacket4, 
									   CGraphCalculateData* pGraphCalculateData)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData || !pPacketList) return FALSE;

	if(pIndicatorInfo == NULL)
		return false;

	CList<CString, CString> subGraphNameList;
	if(!pIndicatorInfo->GetSubGraphNameList(subGraphNameList))
		return false;

	POSITION pos_subGraphName = subGraphNameList.GetHeadPosition();
	POSITION pos_graphCalData = pGraphCalculateData->GetHeadPosition();
	while(pos_subGraphName && pos_graphCalData)
	{
		CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetNext(pos_graphCalData);
		if(pSubGraphPacketList == NULL)
			return false;

		CPacket* pIndiPacket = pPacketList->GetBaseData(subGraphNameList.GetNext(pos_subGraphName), false);
		if(pIndiPacket == NULL)
			return false;

		if(pPacket == NULL)
			pPacket = pIndiPacket;
		else if(pPacket != NULL && pPacket1 == NULL)
			pPacket1 = pIndiPacket;
		else if(pPacket != NULL && pPacket1 != NULL && pPacket2 == NULL)
			pPacket2 = pIndiPacket; 
		else if(pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 == NULL)
			pPacket3 = pIndiPacket;
		else if(pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 != NULL && pPacket4 == NULL)
			pPacket4 = pIndiPacket;
		else
			break;
		
		pSubGraphPacketList->AddTail(pIndiPacket);
	}
	return (pPacket != NULL && pPacket1 != NULL && pPacket2 != NULL && pPacket3 != NULL && pPacket3 != NULL);
}
*/

//sy 2004.2.13.
CString CIndicator::GetPacketType(CPacketList* pPacketList) const
{
	if(pPacketList == NULL)
		return TYPE_INTERGER; // "��1"

	CPacket* pEndPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));
	if(pEndPacket == NULL)
		return TYPE_INTERGER; // "��1"

	return pEndPacket->GetType(false); // ������ type
}
//sy end

//-> �ܼ�������
void CIndicator::GetSimpleMA(const CPoint& point, const double& dCond, CList<double, double>* pSourceList, CPacket* pMAPacket, BOOL p_bContinueZero)
{
	if(pMAPacket == NULL || pSourceList == NULL || dCond <= 0)
		return;

	double dData = 0, dPrevData = 0;
	double totval=0 ;
	const int nCnt = pSourceList->GetCount();
	for(int count = point.x ; count < nCnt ; count++)
	{
		if((count+1) >= dCond)
		{
			int start = (int)floor(count + 1 - dCond);       //���糯¥���� day��ŭ�� ���� ��¥..

			POSITION pos = FindPosition(start, *pSourceList);
			for(int i = start ; i < (dCond+start) ; i++) 
			{
				if(pos == NULL) return ;
				dData = GetNext(pSourceList, pos);	// totval=�ñ׸� ����		

				// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
				//		if Error, Set All with Close for Base or Market Close Price.
				if( 0 == dData && p_bContinueZero)	dData		= dPrevData;
				else								dPrevData	= dData;
				
				totval = totval + dData;
			}
			
			SetData(point, count, totval/dCond, pMAPacket);
			int cout = pMAPacket->GetCount();
			totval=0;
		}
		else		//n�ϸ�ŭ�� �̵������ �� �� ������..
			SetData(point, count, totval/dCond, pMAPacket);
	}
}
