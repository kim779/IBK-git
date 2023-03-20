// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartItemData.h"
#include "ChartItem.h"
#include "./define/ChartDef.h"
#include "./define/ChartMsgDef.h"
#include "./include/arMarketIndicator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CChartItemData::CChartItemData(CChartItem *pChartItem, long lTrCode)
CChartItemData::CChartItemData(CChartItem *pChartItem, CString strTRCode)
{
	m_pShareInputData = NULL;
	Init(pChartItem,strTRCode);
}

CChartItemData::~CChartItemData()
{
	ClearChartItemData();
}

void CChartItemData::Init(CChartItem *pChartItem, CString strTRCode)
{
	memset(m_chkey,'0',sizeof(m_chkey));

//@��������	m_pSt33100Data = NULL;
//@��������	m_pSt35100Data = NULL;
//@��������	m_pSt36100Data = NULL;
	m_pKB0602_ChartData = NULL;
	m_pKB0603_ChartData = NULL;
	m_pKB0604_ChartData = NULL;
	m_pKB1013_ChartData = NULL;
	m_pKB0606_ChartData = NULL;

	m_pSt10450Data = NULL;
	m_pSt10451Data = NULL;
	m_pSt10452Data = NULL;
	m_pSt03040Data = NULL;
	m_pSt01007Data = NULL;

	m_lDataSize = 0;
	m_lCnt = 0;
	m_bInitalChart = FALSE;
	m_pChartItem = pChartItem;
	//m_lTrCode = lTrCode;	
	m_strTRCode = strTRCode;
	m_bIsFilled = FALSE;
	m_lStartDate = NULL; 
	m_lEndDate = NULL;
	m_lCntRequiredByPrim = 0;
	m_lCntTemp = 0;

	m_nCntOverLap = 0;
	m_bOverLapping = FALSE;

	m_bNextMarketData = FALSE;
	// �������� �ҷ����� : �������� - ojtaso (20070529)
	m_bResetChartCfgWith = FALSE;

	m_chCompareChartType = DIVIDED_COMPARE_CHART;
}

// ���õ� ��Ʈ�� ���� ���� : �������� - ojtaso (20070215)
void CChartItemData::SetTRCode(LPCTSTR lpszTRCode)
{
	if(m_strTRCode.Compare(lpszTRCode))
		ChangePacketNameArray(lpszTRCode);

	m_strTRCode = lpszTRCode;
}


BOOL CChartItemData::ClearChartItemData()
{	
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::ClearChartItemData::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	RemoveRealItem();
	RemovePacketAndGraph();
	RemoveData();
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::ClearChartItemData::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	return TRUE;
}

BOOL CChartItemData::RemoveData()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveData::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	if(m_pKB0602_ChartData)
	{
		delete  m_pKB0602_ChartData;
		m_pKB0602_ChartData = NULL;
	}
	if(m_pKB0603_ChartData)
	{
		delete  m_pKB0603_ChartData;
		m_pKB0603_ChartData = NULL;
	}	

	if(m_pKB0604_ChartData)
	{
		delete  m_pKB0604_ChartData;
		m_pKB0604_ChartData = NULL;
	}

	if(m_pKB1013_ChartData)
	{
		delete  m_pKB1013_ChartData;
		m_pKB1013_ChartData = NULL;
	}

	if(m_pKB0606_ChartData)
	{
		delete  m_pKB0606_ChartData;
		m_pKB0606_ChartData = NULL;
	}

	if(m_pSt10450Data)
	{
		delete  m_pSt10450Data;
		m_pSt10450Data = NULL;
	}

	if(m_pSt10451Data)
	{
		delete  m_pSt10451Data;
		m_pSt10451Data = NULL;
	}

	if(m_pSt03040Data)
	{
		delete m_pSt03040Data;
		m_pSt03040Data = NULL;
	}
	if(m_pSt01007Data)
	{
		delete m_pSt01007Data;
		m_pSt01007Data = NULL;
	}

	m_lDataSize = 0;

#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveData::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	return TRUE;
}

void CChartItemData::SetCodeForQuery(LPCTSTR lpCode, BOOL bInitKey/* = TRUE*/)
{
	ASSERT(lpCode);
	m_strCodeForRequest = lpCode;
	//if(m_strCodeForRequest.GetLength() == CODE_STOCK_LENGTH2)
	//	m_strCodeForRequest.Format("%s ", lpCode);
	if(bInitKey)
		memset(m_chkey,'0',sizeof(m_chkey));
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItemData::GetQueryData(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	m_lDataSize = 0;
	m_lCnt = 0;
	CString strTRCode = lpSelChart ? lpSelChart->m_strChartGubun : m_strTRCode;

	if(strTRCode == QUERY_STRJONGMOK)		
		return GetQueryData_KB_p0602(bNext, lpSelChart);
	else if(strTRCode == QUERY_STRUPJONG)
		return GetQueryData_KB_p0603(bNext, lpSelChart);	
	else if(strTRCode == QUERY_STRFUTOPT)
		return GetQueryData_KB_p0604(bNext, lpSelChart);
	else if(strTRCode == QUERY_STRSTOCKFUTOPT)
		return GetQueryData_KB_d1013(bNext, lpSelChart);
	else if(strTRCode == QUERY_STRFOREIGN)
		return GetQueryData_KB_p0606(bNext, lpSelChart);
//	else if(strTRCode == QUERY_STRELW)
//		return GetQueryDataELW(bNext, lpSelChart);
//@��������

	return "";
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0602(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0602_InRec1 KB0602InRec1;

	// �ʱ�ȭ
	memset(&KB0602InRec1, 0x20, sizeof(KB0602InRec1));

	// �����ڵ�
	memset(KB0602InRec1.shcode, 0x20, sizeof(KB0602InRec1.shcode));

	memset(KB0602InRec1.shcode, 0x20, sizeof(KB0602InRec1.shcode));

	if(lpSelChart)
		memcpy(KB0602InRec1.shcode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(KB0602InRec1.shcode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());

	// �˻�������
	memcpy(KB0602InRec1.date, m_pChartItem->GetEndDateRequested(), sizeof(KB0602InRec1.date));

	// ��ȸ�Ǽ�
	char Temp[64];
	sprintf(Temp,"%04d",GetCntForQuery(bNext));
	memcpy(KB0602InRec1.num, Temp, sizeof(KB0602InRec1.num));

	// ��ȸ����
//080520-002��ȸ���� ����	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
//	char chTypeRequested = m_pChartItem->GetTypeRequested();
//	eu0602InRec1.button[0] = chTypeRequested;
	KB0602InRec1.button[0] = m_pShareInputData->m_DataInputBar.m_chType;
//@	memcpy(eu0602InRec1.unit, aTypeNum, 3);
//	if(chTypeRequested == SEC10_DATA_CHART)
//		memcpy(eu0602InRec1.unit,"10s", 3);
//	else if (chTypeRequested == SEC30_DATA_CHART)
//		memcpy(eu0602InRec1.unit,"30s", 3);
//	else if (chTypeRequested == HOUR_DATA_CHART)
//	{
//		int nTemp;	
//		CString strTemp;
//		strTemp = lpSelChart ? lpSelChart->m_strTickMinDayNumber : m_pChartItem->GetTickMinDayNumberRequested();
//		nTemp = atoi(strTemp) * 60;
//		strTemp.Format("%03d",nTemp);
//		memcpy(eu0602InRec1.unit, strTemp, strTemp.GetLength());
//	}
//	else
//	{
//@		if(lpSelChart)
//@			memcpy(eu0602InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
//@		else
//@			memcpy(eu0602InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());

		sprintf(Temp, "%03ld", m_pChartItem->m_pShareInputData->m_DataInputBar.m_lTypeNum);
		memcpy(KB0602InRec1.unit, Temp, 3);
//	}

	// ��ȸ����
	//memcpy(eu0602InRec1.button,&chTypeRequested,sizeof(eu0602InRec1.button));
	//eu0602InRec1.button[0] = chTypeRequested;

	// �����ְ�
	char chUseRevised = '0';
	if(m_pChartItem->GetUseRevisedValue())
	{
		chUseRevised = '1';
	}
	memcpy(KB0602InRec1.modGb,&chUseRevised,sizeof(KB0602InRec1.modGb));

	// ��ȸ���
	char chFlag;
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(KB0602InRec1.nflag,&chFlag,sizeof(KB0602InRec1.nflag));

	// �˻�Ű
	if(chFlag == '1')//���� ��ȸ�� ���
	{
		CString sKey;
		sKey.Format("%*.*s", sizeof(m_chkey), sizeof(m_chkey), m_chkey);
		sKey.TrimLeft(); sKey.TrimRight();
		if(sKey.GetLength()>0)
			memcpy(KB0602InRec1.nkey,m_chkey,sizeof(KB0602InRec1.nkey));
		else
			memset(KB0602InRec1.nkey,0x20,sizeof(KB0602InRec1.nkey));
	}
	else //�ܼ� ��ȸ �� ���.
		memset(KB0602InRec1.nkey,0x20,sizeof(KB0602InRec1.nkey));

	// ���ݱ���
	//int nNumericValueType = lpSelChart ? lpSelChart->m_nChartNumericValueType : m_pChartItem->GetChart_NumericValueType();
	char chUseRateValue = m_pChartItem->m_pShareInputData->m_DataInputBar.m_cDataGubun;//'0' + nNumericValueType;
//	if(nNumericValueType == WON_TYPE)			chUseRateValue = '0';
//	else if(nNumericValueType == PERCENT_TYPE)	chUseRateValue = '1';
//	else if(nNumericValueType == DOLLAR_TYPE)	chUseRateValue = '2';

	KB0602InRec1.nDataGubun[0] = chUseRateValue;
	//memcpy(eu0602InRec1.nDataGubun, &chUseRateValue, sizeof(eu0602InRec1.nDataGubun));

	// ������
	char chUseFackedCandle = '1';
//	if(m_pChartItem->GetBNormalizedMinuteTimeRule())// TRUE�̸� ��� 
//		chUseFackedCandle = '0'; // '0':����� '1':�Ǻ�           
	memcpy(KB0602InRec1.ImgRgb, &chUseFackedCandle, sizeof(KB0602InRec1.ImgRgb));

	// ���ⱸ�а�
	//memcpy(eu0602InRec1.Irecordgb, m_pChartItem->GetReplayFlag(), sizeof(eu0602InRec1.Irecordgb));
	KB0602InRec1.Irecordgb[0] = '0';
	
	// �������n����
//	memcpy(eu0602InRec1.nDayBefore, m_pChartItem->GetReplayDayBefore(),	sizeof(eu0602InRec1.nDayBefore));
	memcpy(KB0602InRec1.nDayBefore, "00", 2);

	// ����ð�(hhmmss)
//	CString strReplayTime = m_pChartItem->GetReplayStartTime();
//	memcpy(eu0602InRec1.Itime, m_pChartItem->GetReplayStartTime(), strReplayTime.GetLength());
	memcpy(KB0602InRec1.Itime, "000000", 6);
	
	// ������ȸ
	char chNowData = '0' + m_pChartItem->m_pShareInputData->m_DataInputBar.m_bTodayRequest;
//-->
//	if( chTypeRequested == DAY_DATA_CHART	|| 
//		chTypeRequested == WEEK_DATA_CHART	|| 
//		chTypeRequested == MONTH_DATA_CHART )
//		chNowData = '0';
//	else
//	{
//		//CString strQueryOpt = m_pChartItem->GetQueryOpt();
//		CString strQueryOpt;
//		strQueryOpt.Format("%d", m_pChartItem->m_pShareInputData->m_DataInputBar.m_bTodayRequest);
//		if(strQueryOpt == "1")
//			chNowData = '1';
//	}
//<--
	//memcpy(eu0602InRec1.nowData, &chNowData, sizeof(eu0602InRec1.nowData));
	KB0602InRec1.nowData[0] = chNowData;

	CString strKB0602Info;
	char* szKB0602InRec1 = strKB0602Info.GetBuffer(sizeof(KB0602InRec1));
	memcpy(szKB0602InRec1, (char*)&KB0602InRec1, sizeof(KB0602InRec1));
	strKB0602Info.ReleaseBuffer();
	CString strKB0602InRec1;
	strKB0602InRec1 = strKB0602Info;
	return strKB0602InRec1;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0603(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0603_InRec1 KB0603InRec1;

	// �ʱ�ȭ
	memset(&KB0603InRec1, 0x00, sizeof(KB0603InRec1));

	if(m_strCodeForRequest.GetLength() != UPJONG_LENGTH) 
	{
		ASSERT(0);
		return "";
	}

	// �屸�� / �����ڵ�
	if(lpSelChart)
	{
		KB0603InRec1.Sjanggb[0] = '1';		
		KB0603InRec1.Supcode[0] = lpSelChart->m_strItemCode[0];	
		KB0603InRec1.Supcode[1] = lpSelChart->m_strItemCode[1];	
		KB0603InRec1.Supcode[2] = lpSelChart->m_strItemCode[2];
	}
	else
	{
		KB0603InRec1.Sjanggb[0] = '1';		
		KB0603InRec1.Supcode[0] = m_strCodeForRequest[0];	
		KB0603InRec1.Supcode[1] = m_strCodeForRequest[1];	
		KB0603InRec1.Supcode[2] = m_strCodeForRequest[2];
	}

	// �˻�������
	memcpy(KB0603InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(KB0603InRec1.Sdate));

	// �˻��ҵ����Ͱ���
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(KB0603InRec1.Smaxbong, Temp, sizeof(KB0603InRec1.Smaxbong));
	
	//char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	// ��ȸ����
	char chTypeRequested = m_pChartItem->m_pShareInputData->m_DataInputBar.m_chType;
	KB0603InRec1.Sbtngb[0] = chTypeRequested;
//	if(chTypeRequested == SEC10_DATA_CHART)
//		memcpy(KB0603InRec1.Sunit, "10s", 3);
//	else if (chTypeRequested == SEC30_DATA_CHART)
//		memcpy(KB0603InRec1.Sunit, "30s", 3);
//	else
//	{
//		if(lpSelChart)
//			memcpy(KB0603InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
//		else
//			memcpy(KB0603InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());
//	}
	// N��(ƽ�н���)
	sprintf(Temp, "%03ld", m_pChartItem->m_pShareInputData->m_DataInputBar.m_lTypeNum);
	memcpy(KB0603InRec1.Sunit, Temp, 3);


	// ����/��ȸ
	char chFlag;
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(KB0603InRec1.Snextgb, &chFlag, sizeof(KB0603InRec1.Snextgb));

	// ��ġŰ
	if(chFlag == '1')	// ���� ��ȸ�� ���
	{
		CString sKey;
		sKey.Format("%*.*s", sizeof(m_chkey), sizeof(m_chkey), m_chkey);
		sKey.TrimLeft(); sKey.TrimRight();

		if(sKey.GetLength()>0)
			memcpy(KB0603InRec1.Snextkey, m_chkey, sizeof(KB0603InRec1.Snextkey));
		else
			memset(KB0603InRec1.Snextkey, 0x20, sizeof(KB0603InRec1.Snextkey));
	}
	else				// �ܼ� ��ȸ�� ���.
		memset(KB0603InRec1.Snextkey, 0x20, sizeof(KB0603InRec1.Snextkey));

	//int nNumericValueType = m_pChartItem->m_pShareInputData->m_DataInputBar.m_cDataGubun;//lpSelChart ? lpSelChart->m_nChartNumericValueType : m_pChartItem->GetChart_NumericValueType();
	KB0603InRec1.SdataType[0] = m_pChartItem->m_pShareInputData->m_DataInputBar.m_cDataGubun;
	//KB0603InRec1.SdataType[0] = '0';	//�߰�080521

	// ������ȸ
//	char chNowData = '0';
//	if( chTypeRequested == DAY_DATA_CHART	|| 
//		chTypeRequested == WEEK_DATA_CHART	|| 
//		chTypeRequested == MONTH_DATA_CHART )
//		chNowData = '0';
//	else
//		chNowData = '1';
//	memcpy(KB0603InRec1.nowData, &chNowData, sizeof(KB0603InRec1.nowData));
	// ������ȸ
	char chNowData = '0' + m_pChartItem->m_pShareInputData->m_DataInputBar.m_bTodayRequest;
	KB0603InRec1.nowData[0] = chNowData;

	CString strKB0603Info;
	char* szKB0603Info = strKB0603Info.GetBuffer(sizeof(KB0603InRec1));
	memcpy(szKB0603Info, (char*)&KB0603InRec1, sizeof(KB0603InRec1));
	strKB0603Info.ReleaseBuffer();
	CString strKB0603InRec1;
	strKB0603InRec1 = strKB0603Info;
	return strKB0603InRec1;
}

CString CChartItemData::GetQueryData_KB_p0604(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0604_InRec1 KB0604InRec1;

	// �ʱ�ȭ
	memset(&KB0604InRec1, 0x20, sizeof(KB0604InRec1));

	// �����ڵ�
	memset(KB0604InRec1.Scode, 0x20, sizeof(KB0604InRec1.Scode));
	if(lpSelChart)
		memcpy(KB0604InRec1.Scode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(KB0604InRec1.Scode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());

	// �˻�������
	memcpy(KB0604InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(KB0604InRec1.Sdate));

	// �˻��ҵ����Ͱ���
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(KB0604InRec1.Smaxbong, Temp, sizeof(KB0604InRec1.Smaxbong));

	// ����(ƽ�����ֿ�)
	//memcpy(KB0604InRec1.Sbtngb, &chTypeRequested, sizeof(KB0604InRec1.Sbtngb));
	KB0604InRec1.Sbtngb[0] = m_pShareInputData->m_DataInputBar.m_chType;

	// ��?��.
	sprintf(Temp, "%03ld", m_pShareInputData->m_DataInputBar.m_lTypeNum);
	memcpy(KB0604InRec1.Sunit, Temp, 3);

	// ��ȸ����(������ȸ)
	char chFlag;
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(KB0604InRec1.Snextgb, &chFlag, sizeof(KB0604InRec1.Snextgb));

	// ��ġŰ
	if(chFlag == '1')	// ���� ��ȸ�� ���
	{
// 		CString sKey;
// 		sKey.Format("%*.*s", sizeof(m_chkey), sizeof(m_chkey), m_chkey);
// 		sKey.TrimLeft(); sKey.TrimRight();
// 		//if(atoi(sKey) > 0)
// 		if(sKey.GetLength()>0)
// 			memcpy(KB0604InRec1.Snextkey, m_chkey, sizeof(KB0604InRec1.Snextkey));
// 		else
// 			memset(KB0604InRec1.Snextkey, 0x20, sizeof(KB0604InRec1.Snextkey));
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();
		
		memcpy(KB0604InRec1.Snextkey, strKey, min(sizeof(KB0604InRec1.Snextkey), strKey.GetLength()));
	}
//	else				// �ܼ� ��ȸ�� ���
//		memset(KB0604InRec1.Snextkey, 0x20, sizeof(KB0604InRec1.Snextkey));

	//��Ʈ���� (0:����, 1:����,2:�޷�)
	//int nNumericValueType = lpSelChart ? lpSelChart->m_nChartNumericValueType : m_pChartItem->GetChart_NumericValueType();
	//KB0604InRec1.SdataType[0] = '0' + nNumericValueType;
	KB0604InRec1.SdataType[0] = m_pShareInputData->m_DataInputBar.m_cDataGubun;

	// ������(����Ǻ�)
	char chUseFackedCandle = '1';
//	if(m_pChartItem->GetBNormalizedMinuteTimeRule())
//	{
		chUseFackedCandle = '0'; // '0':����� '1':�Ǻ�           
//	}
	memcpy(KB0604InRec1.Simgrgb, &chUseFackedCandle, sizeof(KB0604InRec1.Simgrgb));

	// ���ⱸ�а�
//	memcpy(KB0604InRec1.Srecordgb, m_pChartItem->GetReplayFlag(), sizeof(KB0604InRec1.Srecordgb));
	KB0604InRec1.Srecordgb[0] = '0';

	// �������n����
//	memcpy(KB0604InRec1.Sredate, m_pChartItem->GetReplayDayBefore(), sizeof(KB0604InRec1.Sredate));
	memcpy(KB0604InRec1.Sredate, "00", 2);

	// ����ð�(hhmmss)
//	memcpy(KB0604InRec1.Stime, m_pChartItem->GetReplayStartTime(), sizeof(KB0604InRec1.Stime));
	memcpy(KB0604InRec1.Stime, "000000", 6);

	// ������ȸ
//	char chNowData = '0';
//	if( chTypeRequested == DAY_DATA_CHART	||
//		chTypeRequested == WEEK_DATA_CHART	||
//		chTypeRequested == MONTH_DATA_CHART )
//		chNowData = '0';
//	else
//		chNowData = '1';

	char chNowData = '0' + m_pChartItem->m_pShareInputData->m_DataInputBar.m_bTodayRequest;
	memcpy(KB0604InRec1.nowData, &chNowData, sizeof(KB0604InRec1.nowData));	// ������ȸ����


	CString strKB0604Info;
	char* szKB0604Info = strKB0604Info.GetBuffer(sizeof(KB0604InRec1));
	memcpy(szKB0604Info, (char*)&KB0604InRec1, sizeof(KB0604InRec1));
	strKB0604Info.ReleaseBuffer();
	CString strKB0604InRec1;
	strKB0604InRec1 = strKB0604Info;
	return strKB0604InRec1;
}

CString CChartItemData::GetQueryData_KB_d1013(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_d1013_InRec1 KB1013InRec1;

	// �ʱ�ȭ
	memset(&KB1013InRec1, 0x20, sizeof(KB1013InRec1));

	// �����ڵ�
	memset(KB1013InRec1.Scode, 0x20, sizeof(KB1013InRec1.Scode));
	if(lpSelChart)
		memcpy(KB1013InRec1.Scode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(KB1013InRec1.Scode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());

	// �˻�������
	memcpy(KB1013InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(KB1013InRec1.Sdate));

	// �˻��ҵ����Ͱ���
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(KB1013InRec1.Smaxbong, Temp, sizeof(KB1013InRec1.Smaxbong));

	// N��(ƽ�н���)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
//	if(chTypeRequested == SEC10_DATA_CHART)
//		memcpy(KB1013InRec1.Sunit,"10s", 3);
//	else if (chTypeRequested == SEC30_DATA_CHART)
//		memcpy(KB1013InRec1.Sunit,"30s", 3);
//	else
	{
		if(lpSelChart)
			memcpy(KB1013InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
		else
			memcpy(KB1013InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	}

	// ����(ƽ�����ֿ�)
	memcpy(KB1013InRec1.Sbtngb, &chTypeRequested, sizeof(KB1013InRec1.Sbtngb));

	// ��ȸ����(������ȸ)
	char chFlag;
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(KB1013InRec1.Snextgb, &chFlag, sizeof(KB1013InRec1.Snextgb));

	// ��ġŰ
	if(chFlag == '1')	// ���� ��ȸ�� ���
	{
		CString sKey;
		sKey.Format("%*.*s", sizeof(m_chkey), sizeof(m_chkey), m_chkey);
		sKey.TrimLeft(); sKey.TrimRight();
		//if(atoi(sKey) > 0)
		if(sKey.GetLength()>0)
			memcpy(KB1013InRec1.Snextkey, m_chkey, sizeof(KB1013InRec1.Snextkey));
		else
			memset(KB1013InRec1.Snextkey, 0x20, sizeof(KB1013InRec1.Snextkey));
	}
	else				// �ܼ� ��ȸ�� ���
		memset(KB1013InRec1.Snextkey, 0x20, sizeof(KB1013InRec1.Snextkey));

	int nNumericValueType = lpSelChart ? lpSelChart->m_nChartNumericValueType : m_pChartItem->GetChart_NumericValueType();
	KB1013InRec1.SdataType[0] = '0' + nNumericValueType;
	//KB1013InRec1.SdataType[0] = '0';	//�߰�080521

	// ������(����Ǻ�)
	char chUseFackedCandle = '1';
//	if(m_pChartItem->GetBNormalizedMinuteTimeRule())
//	{
		chUseFackedCandle = '0'; // '0':����� '1':�Ǻ�           
//	}
	memcpy(KB1013InRec1.Simgrgb, &chUseFackedCandle, sizeof(KB1013InRec1.Simgrgb));

	// ���ⱸ�а�
	//memcpy(KB1013InRec1.Srecordgb, m_pChartItem->GetReplayFlag(), sizeof(KB1013InRec1.Srecordgb));
	KB1013InRec1.Srecordgb[0] = '0';

	// �������n����
	//memcpy(KB1013InRec1.Sredate, m_pChartItem->GetReplayDayBefore(), sizeof(KB1013InRec1.Sredate));

	// ����ð�(hhmmss)
	//memcpy(KB1013InRec1.Stime, m_pChartItem->GetReplayStartTime(), sizeof(KB1013InRec1.Stime));

	// ������ȸ
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART )
		chNowData = '0';
	else
		chNowData = '1';

	memcpy(KB1013InRec1.nowData, &chNowData, sizeof(KB1013InRec1.nowData));	// ������ȸ����

	CString strKB1013Info;
	char* szKB1013Info = strKB1013Info.GetBuffer(sizeof(KB1013InRec1));
	memcpy(szKB1013Info, (char*)&KB1013InRec1, sizeof(KB1013InRec1));
	strKB1013Info.ReleaseBuffer();
	CString strKB1013InRec1;
	strKB1013InRec1 = strKB1013Info;
	return strKB1013InRec1;
}

//������ǥ TR�� ������ ���� Input���� ��������.
CString CChartItemData::GetMarketQueryData_ChartItemData(BOOL bNext, UINT nRqstCnt)
{
	m_lDataSize = 0;
	m_lCnt = 0;
//	if(		(m_strTRCode == QUERY_STRMARKET_10450)	||
//			(m_strTRCode == QUERY_STRMARKET_10451)	||
//			(m_strTRCode == QUERY_STRMARKET_10452)
//		)
	if(m_strTRCode == QUERY_STRMARKET)
		return GetQueryDataMarket(bNext, nRqstCnt);//return GetQueryDataMarket(bNext);		

	return "";
}

BOOL CChartItemData::MakeChartData(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	BOOL r = FALSE;
	if(m_strTRCode == QUERY_STRJONGMOK)
		return MakeChartData_KB_p0602(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRUPJONG)
		return MakeChartData_KB_p0603(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRFUTOPT)
		return MakeChartData_KB_p0604(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRSTOCKFUTOPT)
		return MakeChartData_KB_d1013(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRFOREIGN)
		return MakeChartData_KB_p0606(szData, nLength, lpSelChart);

	return FALSE;
}

BOOL CChartItemData::MakeChartData_KB_p0602(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_p0602_OutRec1_EX* pKB0602_OutRec1 = (KB_p0602_OutRec1_EX*)szData;
	if(pKB0602_OutRec1 == NULL) return FALSE;

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0602_OutRec1->nkey, sizeof(pKB0602_OutRec1->nkey));

//@Solomon-->
// 	m_pChartItem->EnableRequestMoreButton(TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
// 
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB0602_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0602_OutRec1->nkey), sizeof(pKB0602_OutRec1->nkey), pKB0602_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0602_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)m_chkey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}

//@Solomon<--
	
	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pKB0602_ChartData)
	{
		//m_pKB0602_ChartData = new EU_p0602_CHART_COMP;
		m_pKB0602_ChartData = (KB_p0602_CHART_COMP*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	// �����
	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0602_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();

	// ��Ʈī��Ʈ
	MEMCPYWITHNULL(szTemp, pKB0602_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;

	long lPosForChart = 0;
	if(nLength < (&pKB0602_OutRec1->fcnt[0] - &pKB0602_OutRec1->name[0]))
		return FALSE;

	// ��Ʈ������
	lPosForChart = nLength - (&pKB0602_OutRec1->fcnt[0] - &pKB0602_OutRec1->name[0]);
	memcpy(m_pKB0602_ChartData, &pKB0602_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB0602_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB0602_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_p0603(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_p0603_OutRec1_EX* pKB0603_OutRec1 = (KB_p0603_OutRec1_EX*)szData;
	if(pKB0603_OutRec1 == NULL) return FALSE;

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0603_OutRec1->nkey, sizeof(pKB0603_OutRec1->nkey));

//@Solomon-->
// 	m_pChartItem->EnableRequestMoreButton(TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
// 
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB0603_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0603_OutRec1->nkey), sizeof(pKB0603_OutRec1->nkey), pKB0603_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0603_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)m_chkey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pKB0603_ChartData)
	{
		//m_pKB0603_ChartData = new EU_p0603_CHART_COMP;
		m_pKB0603_ChartData = (KB_p0603_CHART_COMP*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lSize = 0;
	MEMCPYWITHNULL(szTemp, pKB0603_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	if(m_strCode[0]=='1')
	{
		m_strName = "KOSPI " + m_strName;
	}
	else if(m_strCode[0]=='2')
	{
		m_strName = "KOSDAQ " + m_strName;
	}

	MEMCPYWITHNULL(szTemp, pKB0603_OutRec1->fcnt);
	lSize = atol(szTemp);
	m_lCntTemp = lSize;

	long lPosForChart = 0;
	if(nLength < (&pKB0603_OutRec1->fcnt[0] - &pKB0603_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB0603_OutRec1->fcnt[0] - &pKB0603_OutRec1->name[0]);
	memcpy(m_pKB0603_ChartData, &pKB0603_OutRec1->fcnt[0],lPosForChart);
	if(!m_pChartItem->GetNext()&&!m_pChartItem->IsPrimaryChartItemData(this))
	{
		m_pKB0603_ChartData->bojomsg[7] = 'P';
	}

	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp,m_pKB0603_ChartData->chart[lSize-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp,m_pKB0603_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);
	
	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_p0604(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_p0604_OutRec1_EX* pKB0604_OutRec1 = (KB_p0604_OutRec1_EX*)szData;
	if(pKB0604_OutRec1 == NULL) return FALSE;

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0604_OutRec1->nkey, sizeof(pKB0604_OutRec1->nkey));

//@Solomon-->
// 	m_pChartItem->EnableRequestMoreButton(TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
// 
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB0604_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0604_OutRec1->nkey), sizeof(pKB0604_OutRec1->nkey), pKB0604_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0604_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)m_chkey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pKB0604_ChartData)
	{
		//m_pKB0604_ChartData = new EU_p0604_CHART_COMP;
		m_pKB0604_ChartData = (KB_p0604_CHART_COMP*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0604_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp, pKB0604_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pKB0604_OutRec1->fcnt[0] - &pKB0604_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB0604_OutRec1->fcnt[0] - &pKB0604_OutRec1->name[0]);
	memcpy(m_pKB0604_ChartData, &pKB0604_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB0604_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB0604_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);
	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_d1013(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_d1013_OutRec1_EX* pKB1013_OutRec1 = (KB_d1013_OutRec1_EX*)szData;
	if(pKB1013_OutRec1 == NULL) return FALSE;

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB1013_OutRec1->nkey, sizeof(pKB1013_OutRec1->nkey));

//@Solomon-->
// 	m_pChartItem->EnableRequestMoreButton(TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
// 
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB1013_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB1013_OutRec1->nkey), sizeof(pKB1013_OutRec1->nkey), pKB1013_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB1013_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)m_chkey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pKB1013_ChartData)
	{
		//m_pKB1013_ChartData = new EU_p0605_CHART_COMP;
		m_pKB1013_ChartData = (KB_d1013_CHART_COMP*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB1013_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp, pKB1013_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pKB1013_OutRec1->fcnt[0] - &pKB1013_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB1013_OutRec1->fcnt[0] - &pKB1013_OutRec1->name[0]);
	memcpy(m_pKB1013_ChartData, &pKB1013_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB1013_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB1013_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);
	return TRUE;
}

CString CChartItemData::GetTrCode()
{
//	CString strTrCode;
//	strTrCode.Format("%d",m_lTrCode);
//	return strTrCode;
	return m_strTRCode;
}


//---------------------------------------------
// ��ȭ������ �� ����.
// 05. 05. 06 
// comment by sy, nam
//---------------------------------------------
long CChartItemData::GetLTrCode()
{
	return 0;//m_lTrCode;
}


BOOL CChartItemData::RemovePacketAndGraph()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemovePacketAndGraph::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	CString strTitle;
	long lSize = m_strArrayGraphTitle.GetSize();
	long lPos = 0;
	for( lPos=0;lPos<lSize;lPos++)
	{
		strTitle = m_strArrayGraphTitle.GetAt(lPos);
		m_pChartItem->RemoveChart(strTitle);
	}
	m_strArrayGraphTitle.RemoveAll();
	lSize = m_strArrayPacketName.GetSize();
	for(lPos=0;lPos<lSize;lPos++)
	{
		strTitle = m_strArrayPacketName.GetAt(lPos);
		m_pChartItem->RemovePacket(strTitle);
	}
	m_strArrayPacketName.RemoveAll();
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemovePacketAndGraph::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	return TRUE;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
BOOL CChartItemData::AddLoadedPacket(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart)
{
	m_strArrayPacketName.RemoveAll();
	if(lpSelChart->m_strChartGubun == QUERY_STRJONGMOK)
	{
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[0]);

		if(lpSelChart->m_nChartNumericValueType == PERCENT_TYPE)
		{
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[1]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[2]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[3]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[4]);
		}
		else if(lpSelChart->m_nChartNumericValueType == WON_TYPE)
		{
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[1]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[2]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[3]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[4]);
		}
		else if(lpSelChart->m_nChartNumericValueType == DOLLAR_TYPE)
		{
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[1]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[2]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[3]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[4]);
		}

//		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//		m_strArrayPacketName.Add(g_strPacketName[5]);
//		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//		m_strArrayPacketName.Add(g_strPacketName[7]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("�ð�GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("�ð�GuideLine");				
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("��GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("��GuideLine");				
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("����GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("����GuideLine");				

		// �������� �ҷ����� : �������� - ojtaso (20070529)
//		m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)						
	}
	else if(lpSelChart->m_strChartGubun == QUERY_STRUPJONG)
	{
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[0]);

		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[1]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[2]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[3]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[4]);

//		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//		m_strArrayPacketName.Add(g_strPacketName[5]);

		// �������� �ҷ����� : �������� - ojtaso (20070529)
//		m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
	}				
	else if(m_strTRCode == QUERY_STRFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
	{
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[0]);

		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[1]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[2]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[3]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[4]);

//		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//		m_strArrayPacketName.Add(g_strPacketName[5]);

		// �������� �ҷ����� : �������� - ojtaso (20070529)
//		m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)

	}
	else if(lpSelChart->m_strChartGubun == QUERY_STRSTOCKFUTOPT)
	{
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[0]);

		if(lpSelChart->m_nChartNumericValueType == PERCENT_TYPE)
		{
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[1]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[2]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[3]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[4]);
		}
		else if(lpSelChart->m_nChartNumericValueType == WON_TYPE)
		{
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[1]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[2]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[3]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[4]);
		}
		else if(lpSelChart->m_nChartNumericValueType == DOLLAR_TYPE)
		{
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[1]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[2]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[3]);
			m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
			m_strArrayPacketName.Add(g_strPacketName[4]);
		}

		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("�ð�GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("�ð�GuideLine");				
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("��GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("��GuideLine");				
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("����GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("����GuideLine");				
	}
	else if(m_strTRCode == QUERY_STRFOREIGN)
	{
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[0]);

		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[1]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[2]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[3]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[4]);

//		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//		m_strArrayPacketName.Add(g_strPacketName[5]);

		// �������� �ҷ����� : �������� - ojtaso (20070529)
//		m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)

	}

	return TRUE;
}

BOOL CChartItemData::AddPacketAndGraph(BOOL IsNoVolumeGraph /*= FALSE*/)
{	
	if(!m_bInitalChart)
	{
#ifdef _WRITELOG		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX�� AddPacket/Graph::Begin"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
		m_strArrayPacketName.RemoveAll();
		BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);

		{	
			if(IsPrimaryChartItemData)
			{			
				if(m_strTRCode == QUERY_STRJONGMOK)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);
					
					//if(m_pChartItem->GetUseRateValue())
					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
#ifdef _DEBUG
//						AfxMessageBox("CChartItemData::AddPacketAndGraph�� \n Dollar �� �Ѿ�� �κ�  ó�� �ض�...");
#endif
					}

//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[5]);
					// >>>>>>>>>>>>>>>>>>> g_strPacketName[6]  "������" <<<<<<<<<<<<<<<<<<<<<<<<<<						
//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[7]);
					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[6],12,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					//m_strArrayPacketName.Add(g_strPacketName[6]);

/*
					m_pChartItem->AddPacket(MakePacketOption("�ð�GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("�ð�GuideLine");				
					m_pChartItem->AddPacket(MakePacketOption("��GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("��GuideLine");				
					m_pChartItem->AddPacket(MakePacketOption("����GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("����GuideLine");				
*/

					if(!IsNoVolumeGraph)
					{
						AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart

//-->����Ʈ������ ���������� ������� �ʴ´�.
//@							m_strArrayGraphTitle.Add("���� �̵����");
//@							m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
//<--
//							m_strArrayGraphTitle.Add(g_strIndexName[1]);
//							m_pChartItem->AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);
					}
					else
					{
						m_strArrayGraphTitle.Add(g_strIndexName[0]);
						m_pChartItem->AddGraph(0,0,0,"������Ʈ",FALSE,TRUE,TRUE);

//							m_strArrayGraphTitle.Add("���� �̵����");
//							m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);

						m_pChartItem->HideScale(FALSE);
					}					

					// �������� �ҷ����� : �������� - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}						
				}				
				else if(m_strTRCode == QUERY_STRUPJONG)
				{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[0]);

						//@.080723-001 ����������϶�ǥ�ô��� "�� 0.01%"����. 4 -> 7�κ�����.
						if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
						{
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[1]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[2]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[3]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[4]);
						}
						else
						{
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[1]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[2]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[3]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[4]);
						}
						
//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[5]);
						//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[6],12,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						//m_strArrayPacketName.Add(g_strPacketName[6]);

						AddGraphPriceChart();	// JSR 20060123 Add : For Making a Price Chart

//-->����Ʈ������ ���������� ������� �ʴ´�.
//@						m_strArrayGraphTitle.Add("���� �̵����");
//@						m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
//<--

//@						m_strArrayGraphTitle.Add(g_strIndexName[1]);
//@						m_pChartItem->AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);

						// �������� �ҷ����� : �������� - ojtaso (20070529)
						//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						if(!m_bResetChartCfgWith)
						{
							m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
							m_bResetChartCfgWith = TRUE;
						}
				}				
				else if(m_strTRCode == QUERY_STRFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
				{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[0]);

						//@.080723-001 ����������϶�ǥ�ô��� "�� 0.01%"����. 4 -> 7�κ�����.
						if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
						{
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[1]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[2]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[3]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[4]);
						}
						else
						{
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[1]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[2]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[3]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[4]);
						}
						
//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[5]);
						//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[6],12,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						//m_strArrayPacketName.Add(g_strPacketName[6]);

						AddGraphPriceChart();	// JSR 20060123 Add : For Making a Price Chart
						
//-->����Ʈ������ ���������� ������� �ʴ´�.
//@						m_strArrayGraphTitle.Add("���� �̵����");
//@						m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
//<--
//@						m_strArrayGraphTitle.Add(g_strIndexName[1]);
//@						m_pChartItem->AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);

						// �������� �ҷ����� : �������� - ojtaso (20070529)
						//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						if(!m_bResetChartCfgWith)
						{
							m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
							m_bResetChartCfgWith = TRUE;
						}

				}
				else if(m_strTRCode == QUERY_STRSTOCKFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);
					
					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}

					AddGraphPriceChart();	// JSR 20060123 Add : For Making a Price Chart
						
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}
				}
				else if(m_strTRCode == QUERY_STRFOREIGN)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);

					//@.080723-001 ����������϶�ǥ�ô��� "�� 0.01%"����. 4 -> 7�κ�����.
						if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[5]);
					AddGraphPriceChart();	// JSR 20060123 Add : For Making a Price Chart

//-->����Ʈ������ ���������� ������� �ʴ´�.
//@					m_strArrayGraphTitle.Add("���� �̵����");
//@					m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
//<--
//@					m_strArrayGraphTitle.Add(g_strIndexName[1]);
//@					m_pChartItem->AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);


					// �������� �ҷ����� : �������� - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}

				}
			}
			else
			{			
				if(m_strTRCode == QUERY_STRUPJONG)
				{
						CString strPacketName;
						CString strPacketName1;
						strPacketName = m_strName + g_strPacketName[0];
						m_pChartItem->AddPacket(MakePacketOption(strPacketName,8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(strPacketName);

						strPacketName = m_strName + g_strPacketName[1];
						m_pChartItem->AddPacket(MakePacketOption(strPacketName,10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(strPacketName);
						strPacketName = m_strName + g_strPacketName[2];
						m_pChartItem->AddPacket(MakePacketOption(strPacketName,10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(strPacketName);
						strPacketName = m_strName + g_strPacketName[3];
						m_pChartItem->AddPacket(MakePacketOption(strPacketName,10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(strPacketName);
						strPacketName1 = m_strName;
						m_pChartItem->AddPacket(MakePacketOption(strPacketName1,10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(strPacketName1);
						
//						strPacketName = m_strName + g_strPacketName[5];
//						m_pChartItem->AddPacket(MakePacketOption(strPacketName,10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(strPacketName);
						//strPacketName = m_strName + g_strPacketName[6];
						//m_pChartItem->AddPacket(MakePacketOption(strPacketName,12,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(strPacketName);
						CStringArray arrayPacket;
						arrayPacket.Add(strPacketName1);
						m_strArrayGraphTitle.Add(strPacketName1);
						CString strGraphOption = MakeGraphOption(g_strChartType[1],strPacketName1,arrayPacket,strPacketName1);
						m_pChartItem->AddGraph(1,0,0,strGraphOption,TRUE,TRUE,TRUE);
				}
				else if(m_strTRCode == QUERY_STRMARKET)
				{
					m_strArrayPacketName.Add(m_strCodeForRequest); //��Ŷ �� �߰� 
					return TRUE;
				}
			}
		}

//-->@080609-001 �����ϻ���� ��������
		if(m_pChartItem) m_pChartItem->ChangeHorzScalePosition(CDRChartOcx::HORZ_BOTTOM, -1, -1);
//<--
		m_bInitalChart = TRUE;
	
#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX�� AddPacket/Graph::End"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
	}
	return TRUE;
}

// ���ο� ��Ŷ,��Ʈ �߰� : �������� - ojtaso (20061224)
BOOL CChartItemData::AddPacketAndGraph2(BOOL bAddGraph /*= TRUE*/, BOOL bOverlapp /*= FALSE*/, BOOL bSacle)
{
	if(!bAddGraph)
	{
		CString strTitle;
		long lSize = m_strArrayPacketName.GetSize();
		for(long lPos=0;lPos<lSize;lPos++)
		{
			strTitle = m_strArrayPacketName.GetAt(lPos);
			m_pChartItem->RemovePacket(strTitle);
		}
		m_strArrayPacketName.RemoveAll();
	}

//	if(!m_bInitalChart)
	{
#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX�� AddPacket/Graph::Begin"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
		m_strArrayPacketName.RemoveAll();
		BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);

		{	
			if(IsPrimaryChartItemData)
			{			
//-->@080531-001 �ֽ��϶��� ó����ƾ�� ����, �������� �����ϰ� ������.
				if( (m_strTRCode == QUERY_STRJONGMOK) ||
					(m_strTRCode == QUERY_STRUPJONG) ||
					(m_strTRCode == QUERY_STRFUTOPT) ||
					(m_strTRCode == QUERY_STRSTOCKFUTOPT) ||
					(m_strTRCode == QUERY_STRFOREIGN)
					)
//<--@080531-001
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);

					//if(m_pChartItem->GetUseRateValue())
					//if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
					if(m_pChartItem->m_pShareInputData->m_DataInputBar.m_nUnitType == PERCENT_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					//else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)
					else //if(m_pChartItem->m_pShareInputData->m_DataInputBar.m_nUnitType == WON_TYPE)
					{
						LPCTSTR strPacketTypeNumber = g_strPacketTypeNumber[0];
						if((m_strTRCode == QUERY_STRUPJONG) ||
							(m_strTRCode == QUERY_STRFUTOPT)||(m_strTRCode == QUERY_STRFOREIGN) )
								strPacketTypeNumber = g_strPacketTypeNumber[4];

						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,strPacketTypeNumber,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,strPacketTypeNumber,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,strPacketTypeNumber,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,strPacketTypeNumber,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
//					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)
//					{
//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[1]);
//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[2]);
//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[3]);
//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[4]);
//#ifdef _DEBUG
////						AfxMessageBox("CChartItemData::AddPacketAndGraph�� \n Dollar �� �Ѿ�� �κ�  ó�� �ض�...");
//#endif
//					}

//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[5]);
					// >>>>>>>>>>>>>>>>>>> g_strPacketName[6]  "������" <<<<<<<<<<<<<<<<<<<<<<<<<<						
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[7]);
					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[6],12,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					//m_strArrayPacketName.Add(g_strPacketName[6]);

					//�ֽĿ� �ϵ����Ͱ� �ƴϸ� �� ������ �߰����� �ʴ´�.=========================================================================================
					//if(m_pChartItem->GetType() == DAY_DATA_CHART)
					//{
					// >>>>>>>>>>>>>>>>>>> g_strPacketName[7]  "������" <<<<<<<<<<<<<<<<<<<<<<<<<<						
					//	m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					//	m_strArrayPacketName.Add(g_strPacketName[7]);						
					//}
					//============================================================================================================================================

					m_pChartItem->AddPacket(MakePacketOption("�ð�GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("�ð�GuideLine");				
					m_pChartItem->AddPacket(MakePacketOption("��GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("��GuideLine");				
					m_pChartItem->AddPacket(MakePacketOption("����GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("����GuideLine");				

					// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
					int nRow, nCol;
					if(bOverlapp == FALSE)
						AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);
					else
						AddGraphPriceChart_Overlapp(nRow, nCol, bSacle);

//					if(!IsNoVolumeGraph)
//					{
//						AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
						// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
//						m_strArrayGraphTitle.Add("���� �̵����");
//						m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);
//
//						m_strArrayGraphTitle.Add(g_strIndexName[1]);
//						m_pChartItem->AddGraph(nRow+1,nCol,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);
//
//						//m_strArrayGraphTitle.Add("�ŷ��� �̵����");
//						//m_pChartItem->AddGraph(1,0,0,"�ŷ��� �̵����",FALSE,FALSE,FALSE);										
//					}
//					else
//					{
//						m_strArrayGraphTitle.Add(g_strIndexName[0]);
//						m_pChartItem->AddGraph(0,0,0,"������Ʈ",FALSE,TRUE,TRUE);
//
//						m_strArrayGraphTitle.Add("���� �̵����");
//						m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
//
//						m_pChartItem->HideScale(FALSE);
//					}					

					// �������� �ҷ����� : �������� - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}

				}				
				else if(m_strTRCode == QUERY_STRUPJONG)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);

					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[1]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[2]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[3]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[4]);

//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[5]);
					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[6],12,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					//m_strArrayPacketName.Add(g_strPacketName[6]);

					// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
					int nRow, nCol;
					if(bOverlapp == FALSE)
						AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);
					else
						AddGraphPriceChart_Overlapp(nRow, nCol, bSacle);

//					m_strArrayGraphTitle.Add("���� �̵����");
//					m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);

					//m_strArrayGraphTitle.Add("�ŷ��� �̵����");
					//m_pChartItem->AddGraph(1,0,0,"�ŷ��� �̵����",FALSE,FALSE,FALSE);

					// �������� �ҷ����� : �������� - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}
				}				
				else if(m_strTRCode == QUERY_STRFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);

					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[1]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[2]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[3]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[4]);

//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[5]);
					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[6],12,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					//m_strArrayPacketName.Add(g_strPacketName[6]);

					// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
					int nRow, nCol;
					AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);

//					m_strArrayGraphTitle.Add("���� �̵����");
//					m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);

					//m_strArrayGraphTitle.Add("�ŷ��� �̵����");
					//m_pChartItem->AddGraph(1,0,0,"�ŷ��� �̵����",FALSE,FALSE,FALSE);

					// �������� �ҷ����� : �������� - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}

				}
				else if(m_strTRCode == QUERY_STRSTOCKFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);

					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[1]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[2]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[3]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[4]);

//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[5]);
					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[6],12,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					//m_strArrayPacketName.Add(g_strPacketName[6]);

					// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
					int nRow, nCol;
					AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);

//					m_strArrayGraphTitle.Add("���� �̵����");
//					m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);

					//m_strArrayGraphTitle.Add("�ŷ��� �̵����");
					//m_pChartItem->AddGraph(1,0,0,"�ŷ��� �̵����",FALSE,FALSE,FALSE);

					// �������� �ҷ����� : �������� - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}
				}
//@��������
				else if(m_strTRCode == QUERY_STRFOREIGN)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);

					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[1]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[2]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[3]);
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[4]);

//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[5]);

					// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
					int nRow, nCol;
					AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);

//					m_strArrayGraphTitle.Add("���� �̵����");
//					m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);

					//m_strArrayGraphTitle.Add("�ŷ��� �̵����");
					//m_pChartItem->AddGraph(1,0,0,"�ŷ��� �̵����",FALSE,FALSE,FALSE);

					// �������� �ҷ����� : �������� - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}
				}
//				else if(m_strTRCode == QUERY_STRELW)
//				{
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[0]);
//
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[1]);
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[2]);
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[3]);
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[4]);
//
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[5]);
//					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					//m_strArrayPacketName.Add(g_strPacketName[7]);
//					// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
//					int nRow, nCol;
//					AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);
//
//					m_strArrayGraphTitle.Add("���� �̵����");
//					m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);
//
//					//m_strArrayGraphTitle.Add("�ŷ��� �̵����");
//					//m_pChartItem->AddGraph(1,0,0,"�ŷ��� �̵����",FALSE,FALSE,FALSE);
//
//					// �������� �ҷ����� : �������� - ojtaso (20070529)
//					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
//					if(!m_bResetChartCfgWith)
//					{
//						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
//						m_bResetChartCfgWith = TRUE;
//					}
//
//				}
//@��������
			}
			else
			{			
				if(m_strTRCode == QUERY_STRUPJONG)
				{
					CString strPacketName;
					CString strPacketName1;
					strPacketName = m_strName + g_strPacketName[0];
					m_pChartItem->AddPacket(MakePacketOption(strPacketName,8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(strPacketName);

					strPacketName = m_strName + g_strPacketName[1];
					m_pChartItem->AddPacket(MakePacketOption(strPacketName,10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(strPacketName);
					strPacketName = m_strName + g_strPacketName[2];
					m_pChartItem->AddPacket(MakePacketOption(strPacketName,10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(strPacketName);
					strPacketName = m_strName + g_strPacketName[3];
					m_pChartItem->AddPacket(MakePacketOption(strPacketName,10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(strPacketName);
					strPacketName1 = m_strName;
					m_pChartItem->AddPacket(MakePacketOption(strPacketName1,10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(strPacketName1);

//					strPacketName = m_strName + g_strPacketName[5];
//					m_pChartItem->AddPacket(MakePacketOption(strPacketName,10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(strPacketName);
					//strPacketName = m_strName + g_strPacketName[6];
					//m_pChartItem->AddPacket(MakePacketOption(strPacketName,12,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(strPacketName);
					CStringArray arrayPacket;
					arrayPacket.Add(strPacketName1);
					m_strArrayGraphTitle.Add(strPacketName1);
					CString strGraphOption = MakeGraphOption(g_strChartType[1],strPacketName1,arrayPacket,strPacketName1);
					m_pChartItem->AddGraph(1,0,0,strGraphOption,TRUE,TRUE,TRUE);
				}
			}
		}
		m_bInitalChart = TRUE;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	m_pChartItem->WriteToStringLog(_T("OCX�� AddPacket/Graph::End"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
	return TRUE;
}

//KHD : �߰� ChartItemŬ�������� AddPacketAndGraphAnal ȣ��ȴ�. 
BOOL CChartItemData::AddPacketAndGraphAnal()
{
	//m_strArrayPacketName.RemoveAll();
	//m_strArrayGraphTitle.RemoveAll();
	//m_pChartItem->ResetOCX();


	//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
	//m_strArrayPacketName.Add(g_strPacketName[0]);
	//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
	//m_strArrayPacketName.Add(g_strPacketName[1]);
	//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
	//m_strArrayPacketName.Add(g_strPacketName[2]);
	//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
	//m_strArrayPacketName.Add(g_strPacketName[3]);
	//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
	//m_strArrayPacketName.Add(g_strPacketName[4]);
	//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
	//m_strArrayPacketName.Add(g_strPacketName[5]);

	//CStringArray arrayPacket;
	//arrayPacket.Add(g_strPacketName[1]);	// �ð�
	//arrayPacket.Add(g_strPacketName[2]);	// ��
	//arrayPacket.Add(g_strPacketName[3]);	// ����
	//arrayPacket.Add(g_strPacketName[4]);	// ����
	//m_strArrayGraphTitle.Add(g_strIndexName[0]);
	//CString strGraphOption = MakeGraphOption(g_strChartType[0],g_strIndexName[0],arrayPacket,"����");
	//m_pChartItem->AddGraph(0,0,0,strGraphOption,FALSE,FALSE,FALSE);
	////AddGraph(0,0,0,"������Ʈ",FALSE,FALSE,FALSE);

	//arrayPacket.RemoveAll();
	//arrayPacket.Add(g_strPacketName[5]);	// �ŷ���
	//m_strArrayGraphTitle.Add(g_strIndexName[1]);
	//strGraphOption = MakeGraphOption(g_strChartType[2],g_strIndexName[1],arrayPacket,"�ŷ���",0
	//	,RGB(218,46,46),RGB(218,46,46),RGB(46,46,218),RGB(46,46,218),RGB(218,46,46),RGB(218,46,46)
	//	,1);				
	//m_pChartItem->AddGraph(1,0,0,strGraphOption,TRUE,TRUE,TRUE);
		//AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Function Name		: AddGraphPriceChart()
// Function description : ������Ʈ�� ���� �߰��Ѵ�
// Author				: ������ (Yoo Je Sun)
// Made Date			: 2006. 01. 22
///////////////////////////////////////////////////////////////////////////////
void CChartItemData::AddGraphPriceChart(void)
{
	int RowCount = (int)m_pChartItem->GetRowBlockCount();
	int ColumnCount = (int)m_pChartItem->GetColumnBlockCount();

	m_strArrayGraphTitle.Add(g_strIndexName[0]);
	if( RowCount <= 0 || ColumnCount <= 0 )
	{
		m_pChartItem->AddGraph(0,0,0,"������Ʈ",TRUE,TRUE,TRUE);
	}
	else
	{
		m_pChartItem->AddGraph(0,0,0,"������Ʈ",FALSE,FALSE,FALSE);
	}
}
///////////////////////////////////////////////////////////////////////////////

// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
void CChartItemData::AddGraphPriceChart2NewItem(int& nRow, int& nCol, BOOL bAddGraph)
{
	m_strArrayGraphTitle.Add(g_strIndexName[0]);

	if(bAddGraph)
	{
		int RowCount = (int)m_pChartItem->GetRowBlockCount();
		int ColumnCount = (int)m_pChartItem->GetColumnBlockCount();

		if( RowCount <= 0 || ColumnCount <= 0 )
		{
			m_pChartItem->AddGraph(RowCount,0,0,"������Ʈ",TRUE,TRUE,TRUE);
		}
		else
		{
			m_pChartItem->AddGraph(RowCount,0,0,"������Ʈ",TRUE,TRUE,TRUE);
		}

		nRow = RowCount;
		nCol = 0;
	}
}

// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
void CChartItemData::AddGraphPriceChart_Overlapp(int& nRow, int& nCol, BOOL bSacle)
{
	m_strArrayGraphTitle.Add(g_strIndexName[0]);

	nRow = nCol = 0;
	//m_pChartItem->AddGraph(nRow,nCol,0,"������Ʈ",FALSE,TRUE,TRUE);
	m_pChartItem->AddGraph(nRow,nCol,0,"������Ʈ",FALSE,bSacle,TRUE);
}

CString* CChartItemData::GetPstrPacketName(long lPosByOutput, long lPosByInput,CString *pstrPacketName,CString *pstrPacketName1)
{
	CString *pStr = NULL;
	long lSize = sizeof(g_lpszInputMarketIndicator)/sizeof(LPCTSTR) - 1;
	if( lPosByInput < lSize  && pstrPacketName1->IsEmpty()
		&&m_strName==g_lpszInputMarketIndicator[lPosByInput]
		&&g_lpszInputMarketIndicator[lPosByInput]==g_lpszOutputMarketIndicator[lPosByOutput]
		)
	{
		pStr = pstrPacketName1;
		*pStr = g_lpszOutputMarketIndicator[lPosByOutput];
	}
	else if( lPosByInput < lSize  
			&& m_strName==g_lpszInputMarketIndicator[lPosByInput]
			&& g_lpszInputMarketIndicator[lPosByInput]!=g_lpszOutputMarketIndicator[lPosByOutput]
		)
	{
		pStr = pstrPacketName1;
		*pStr = g_lpszOutputMarketIndicator[lPosByOutput];
	}
	else
	{
		CString strNum;
		strNum.Format("tmp%d",lPosByOutput);
		pStr = pstrPacketName;
		*pStr = m_strName + strNum; 
	}			
	return pStr;
}

CString CChartItemData::GetPacketName()
{
	CString strPacketName;
	CString strPacketNameTemp;
	long lSize = m_strArrayPacketName.GetSize();
	for(long lPos=0;lPos<lSize;lPos++)
	{
		strPacketNameTemp = m_strArrayPacketName.GetAt(lPos);
		if(strPacketNameTemp.Find("GuideLine")==-1)
		{
			strPacketName += strPacketNameTemp;
			strPacketName += ";";
		}
	}
	return strPacketName;
}

CString CChartItemData::GetQueryDataMarket(BOOL bNext, int nRqstCnt)
{
//@��������
//	TR_MARKET_I stOutput;
//@��������
//@�����߰�
	p0607InBlock stOutput;
//@�����߰�

	char Temp[64];
	char chFlag;
	memset(&stOutput,0x20,sizeof(stOutput)); //stOutput.chNull = NULL;
	

	CString sWndID;
	sWndID.Format("%d", m_pChartItem->GetDlgCtrlID());
	memcpy(stOutput.wndID, sWndID, sWndID.GetLength());

	memcpy( stOutput.pkName, (LPTSTR)(LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
	
/*	long lSize = m_strArrayPacketName.GetSize();
	BOOL bFind = FALSE;
	CString strPacketNameTemp;
	for(long lPos=0;lPos<lSize;lPos++)
	{
		strPacketNameTemp = m_strArrayPacketName.GetAt(lPos);
		if(strPacketNameTemp.CompareNoCase(m_strCodeForRequest) == 0)
		{
			bFind = TRUE;
		}
	}

	if(!bFind) 
		m_strArrayPacketName.Add(m_strCodeForRequest);
*/	
	

	stOutput.imgr_Gb[0] = '1';//���(0) �Ǻ�(1);
	if(m_pChartItem->m_CfgInfo.bRealCandle == false) //����̶��..
		stOutput.imgr_Gb[0] = '0';

	//CString strPrimCode = m_pChartItem->GetPrimaryChartItemData()->GetCode();
	CString strPrimCode = m_pChartItem->GetSelectedItemCode();
	if(strPrimCode.GetLength() == UPJONG_LENGTH)// ����
	{
		stOutput.type [0]= '1';
		memcpy(stOutput.shcode,(LPCTSTR)strPrimCode,sizeof(stOutput.shcode));
		stOutput.shcode[4] = ' ';	
		stOutput.shcode[5] = ' ';	
		stOutput.shcode[6] = ' ';	
		stOutput.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == CODE_STOCK_LENGTH || strPrimCode.GetLength() == (CODE_STOCK_LENGTH-1))							// ����
	{
		stOutput.type[0] = '0';
		memcpy(stOutput.shcode,(LPCTSTR)strPrimCode,sizeof(stOutput.shcode));
		stOutput.shcode[6] = ' ';
		stOutput.shcode[7] = ' ';
	}
	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
	else if(strPrimCode.GetLength() == FUTOPT_LENGTH || strPrimCode.GetLength() == COMMODITY_LENGTH)							// ����
	{
		stOutput.type[0] = '0';
		memcpy(stOutput.shcode,(LPCTSTR)strPrimCode,sizeof(stOutput.shcode));
	}
	memcpy(stOutput.edate,(LPCTSTR)m_pChartItem->GetEndDateRequested(),sizeof(stOutput.edate));
	if(nRqstCnt >= 10000)
	{
		AfxMessageBox("������ǥ �ִ� ��ȸ ������ 10000�� �����Դϴ�");
		nRqstCnt = 9999;
	}

	//===========================================
	//��ȸ ���� 
	m_lCntRequired = nRqstCnt; //��ȸ�� ������ ���� ���� ������ ���ϱ� ���� �����صд�.
	sprintf(Temp,"%04d",nRqstCnt);
	
	if(atoi(Temp) <= 0) return _T("");
	memcpy(stOutput.num,Temp,sizeof(stOutput.num));

	//���� ����
	if(bNext)
	{		
		chFlag = '1';
		memcpy(m_chkey, m_pChartItem->GetNextKey(TRUE), sizeof(m_chkey)); //�⺻ ��Ʈ �������� ����Ű�� �����´�.
	}
	else
	{
		chFlag = '0';
		memset(m_chkey, 0x00, sizeof(m_chkey));
	}
	memcpy(stOutput.nxtGb,&chFlag,sizeof(stOutput.nxtGb));
	memcpy(stOutput.nxtKey,m_chkey,sizeof(stOutput.nxtKey));

//@�����߰�
	CString strFidInfo("0                                                                               ");
	CString strTrInfo;
	char* pszTrInfo = strTrInfo.GetBuffer(sizeof(stOutput));
	memcpy(pszTrInfo, (char*)&stOutput, sizeof(stOutput));
	strTrInfo.ReleaseBuffer();
	CString strRecordInfo("999999999999991               000001002003004005006007|2               000001002003;");
	CString strOutput;
	strOutput = strFidInfo + strTrInfo + strRecordInfo;
	return strOutput;
//@�����߰�

	return (char*)&stOutput;
}


// �������� �ҷ����� : �������� - ojtaso (20070514)
BOOL CChartItemData::MakeChartData_KB_p0606(void* szData, long nLength, ST_SELCHART* lpSelChart) //�ؿ�����
{
	KB_p0606_OutRec1_EX* pKB0606_OutRec1 = (KB_p0606_OutRec1_EX*)szData;
	if(pKB0606_OutRec1 == NULL) return FALSE;

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0606_OutRec1->nkey, sizeof(pKB0606_OutRec1->nkey));

//@Solomon-->
// 	m_pChartItem->EnableRequestMoreButton(TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
// 
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB0606_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0606_OutRec1->nkey), sizeof(pKB0606_OutRec1->nkey), pKB0606_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0606_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)m_chkey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pKB0606_ChartData)
	{
		m_pKB0606_ChartData = (KB_p0606_CHART_COMP*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0606_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp, pKB0606_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pKB0606_OutRec1->fcnt[0] - &pKB0606_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB0606_OutRec1->fcnt[0] - &pKB0606_OutRec1->name[0]);
	memcpy(m_pKB0606_ChartData, &pKB0606_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB0606_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB0606_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}

long CChartItemData::GetMaximumRequestCnt()
{
	if( (m_strTRCode == QUERY_STRJONGMOK) ||
		(m_strTRCode == QUERY_STRUPJONG ) ||
//		(m_strTRCode == QUERY_STRELW ) || 
//		(m_strTRCode == QUERY_STRMARKET_10451 ) || 
//		(m_strTRCode == QUERY_STRMARKET_10452 ) || 
		(m_strTRCode == QUERY_STRMARKET ) || 
		(m_strTRCode == QUERY_STRFUTOPT ) ||
		(m_strTRCode == QUERY_STRSTOCKFUTOPT ) ||
//		(m_strTRCode == QUERY_STRCOMMODITY ) ||	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
		(m_strTRCode == QUERY_STRFOREIGN) 
	)
	{
		return (long)DEFAULTCNTATONCE;
	}
	
	return 0;
		
}

long CChartItemData::GetCntForQuery(BOOL bNext)
{
	//080526�� ���������� ó���� �������� �������� �Ѵ�.
	m_lCntRequired = m_pChartItem->m_pShareInputData->m_DataInputBar.m_lRequestCnt;
	return m_pChartItem->m_pShareInputData->m_DataInputBar.m_lRequestCnt;

	if(m_pChartItem->IsPrimaryChartItemData(this))	// �� �������� ���
	{
// gm0604 2004/08/20 Due to problem that Cnt become zero
		if(!bNext)
		{
			m_lCntRequired = m_pChartItem->GetCntForRequest();
			if(m_lCntRequired<5)
			{
				m_lCntRequired = 5;
			}
		}
		else
		{
			m_lCntRequired = GetMaximumRequestCnt();
		}		
	}
	else		// ������������ ���..
	{
		if(!bNext)	// ó���� ��� �䱸�Ǿ��� �� �����͸� �����Ѵ�.
		{
			m_lCntRequiredByPrim = m_pChartItem->GetAllCnt();
		}
		// �ִ��û�ɼ��ִ¼������� ��ü�����ͼ��� ������� ��ü�����ͼ��� �����ͼ��� ��û�Ѵ�.
/*		if(GetMaximumRequestCnt()>m_lCntRequiredByPrim) 
		{
			m_lCntRequired = m_lCntRequiredByPrim;
		}
		else
		{
			m_lCntRequired = GetMaximumRequestCnt();
		}		
*/
		m_lCntRequired = m_lCntRequiredByPrim;
	}

	return m_lCntRequired;	
}

BOOL CChartItemData::ProcRequestCntAfterQuery(long lCntReceived)
{
	if(m_lCntRequired<lCntReceived)
	{
		if (m_strCodeForRequest.Find("�������ż�") < 0 && m_strCodeForRequest.Find("�ݿɼǼ��ż�") < 0 && m_strCodeForRequest.Find("ǲ�ɼǼ��ż�") < 0 && m_strCodeForRequest.Find("������ż�����") < 0)
		{
	#ifdef _DEBUG
			CString strMsg;
			strMsg.Format("������ �����ͼ��� ���� �ʽ��ϴ�...[TR:%s][Code:%s]\
						  �䱸�� �����ͼ�:%d, ���� �����ͼ�:%d",\
						  m_strTRCode,m_strCode,m_lCntRequired,lCntReceived);
			AfxMessageBox(strMsg);
	#endif
			return FALSE;
		}
	}
	if(m_pChartItem->IsPrimaryChartItemData(this))	// �� �������� ���
	{
	}
	else
	{
		m_lCntRequiredByPrim -= lCntReceived;
////gm0604 2004.8.2
		if(!m_lCntRequiredByPrim)
		{	
			m_pChartItem->SetNext(FALSE);
			
			m_bIsFilled = TRUE;
#ifdef _DEBUG
			OutputDebugString("CChartItemData::ProcRequest .... m_bIsFillded = TRUE");
#endif
			return TRUE;
		}
////
	}
	if(!lCntReceived)
	{
		m_pChartItem->SetNext(FALSE);

		//===
		SetBNextMarketData(FALSE);
		return TRUE;
	}	
	else
	{
		m_lCnt = lCntReceived;
	}
	return TRUE;
}

// �ؿ�����
// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0606(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0606_InRec1 kb0606InRec1;

	// �ʱ�ȭ
	memset(&kb0606InRec1, 0x00, sizeof(kb0606InRec1));

	// �����ڵ�
	memset(kb0606InRec1.Shcode, 0x20, sizeof(kb0606InRec1.Shcode));
	if(lpSelChart)
		memcpy(kb0606InRec1.Shcode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(kb0606InRec1.Shcode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());

	// �˻�������
	memcpy(kb0606InRec1.date, m_pChartItem->GetEndDateRequested(), sizeof(kb0606InRec1.date));

	// ��ȸ�Ǽ�
	char Temp[64];
	sprintf(Temp,"%04d",GetCntForQuery(bNext));
	memcpy(kb0606InRec1.num, Temp, sizeof(kb0606InRec1.num));

	// ��ȸ����
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	{
		if(lpSelChart)
			memcpy(kb0606InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
		else
			memcpy(kb0606InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	}

	// ��ȸ����
	if(chTypeRequested == MIN_DATA_CHART || chTypeRequested == SEC_DATA_CHART)
	{
		m_pChartItem->SetChartRequestType(DAY_DATA_CHART);
		chTypeRequested = DAY_DATA_CHART;
	}
	memcpy(kb0606InRec1.button,&chTypeRequested,sizeof(kb0606InRec1.button));

	// ��ȸ���
	char chFlag;
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(kb0606InRec1.nflag,&chFlag,sizeof(kb0606InRec1.nflag));

	// �˻�Ű
	memset(kb0606InRec1.nkey,0x20,sizeof(kb0606InRec1.nkey));
	if(chFlag == '1')//���� ��ȸ�� ���
		memcpy(kb0606InRec1.nkey,m_chkey,::lstrlen(m_chkey));
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		if(sKey.GetLength()>0)
			memcpy(kb0606InRec1.nkey,m_chkey,sizeof(kb0606InRec1.nkey));
		else
			memset(kb0606InRec1.nkey,0x20,sizeof(kb0606InRec1.nkey));
	}
	else //�ܼ� ��ȸ �� ���.
		memset(kb0606InRec1.nkey,0x20,sizeof(kb0606InRec1.nkey));
*/
	kb0606InRec1.dataType[0] = m_pChartItem->m_pShareInputData->m_DataInputBar.m_cDataGubun;

	// ������ȸ
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	|| 
		chTypeRequested == WEEK_DATA_CHART	|| 
		chTypeRequested == MONTH_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
	}
	memcpy(kb0606InRec1.nowData, &chNowData, sizeof(kb0606InRec1.nowData));

	CString strKB0606Info;
	char* szKB0606Info = strKB0606Info.GetBuffer(sizeof(kb0606InRec1));
	memcpy(szKB0606Info, (char*)&kb0606InRec1, sizeof(kb0606InRec1));
	strKB0606Info.ReleaseBuffer();
	CString strKB0606InRec1;
	strKB0606InRec1 = strKB0606Info;
	return strKB0606InRec1;
}


CString CChartItemData::MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled, BOOL bOverLapping)
{
	//2005. 05. 18 add by sy, nam =============================================================
	//��ø ����ΰ�� ��Ŷ ������ �ٸ��� �ϱ� ���� �ٸ� �Լ��� ȣ���Ѵ�.	
	if(bChartFilled && bOverLapping)
		return OverLapping_MakePacketOption(lpPacketName,lPacketSize,lpPacketType,bUseInReal, nCntOverLap, bChartFilled, bOverLapping);
	//=========================================================================================

	char szTemp[64];
	CString strPacket;

	strPacket = "1;";
	strPacket += lpPacketName; // ����� Packet��
	strPacket += ";;";
	sprintf(szTemp,"%d",lPacketSize);
	strPacket += szTemp;
	strPacket += ";";
	strPacket += lpPacketType;
	strPacket += ";";
	if(bUseInReal)
	{
		strPacket += "��;\r\n";
	}
	else
	{
		strPacket += "��;\r\n";
	}
//	OutputDebugString(strPacket);
	return strPacket;
}



CString CChartItemData::OverLapping_MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled, BOOL bOverLapping )
{
	char szTemp[64];
	CString strPacket;
	CString strOverLapCnt;
	strOverLapCnt.Format("ov%d", nCntOverLap);

	strPacket += strOverLapCnt;
	strPacket = "1;";
	
	strPacket += strOverLapCnt;
	strPacket += lpPacketName; // ����� Packet��
	strPacket += ";;";
	sprintf(szTemp,"%d",lPacketSize);

	strPacket += strOverLapCnt;
	strPacket += szTemp;
	strPacket += ";";

	strPacket += strOverLapCnt;
	strPacket += lpPacketType;
	strPacket += ";";
	if(bUseInReal)
	{
		strPacket += strOverLapCnt;
		strPacket += "��;\r\n";
	}
	else
	{
		strPacket += strOverLapCnt;
		strPacket += "��;\r\n";
	}
	return strPacket;
}

CString CChartItemData::MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle /*=0*/,
									COLORREF clrGrp1 /*= RGB(218,46,46)*/, COLORREF clrGrp2 /*= RGB(218,46,46)*/,
									COLORREF clrGrp3 /*= RGB(46,46,218)*/, COLORREF clrGrp4 /*= RGB(46,46,218)*/,
									COLORREF clrGrp5 /*= RGB(218,46,46)*/, COLORREF clrGrp6 /*= RGB(218,46,46)*/,
									BOOL bUpFilled /*= 0*/, BOOL bDownFilled /*= 1*/,
									long lUpLineWidth /*= 1*/, long lDownLineWidth /*= 1*/, BOOL bHideGrp /*= 0*/)
{
	CString strChartType = lpChartType;
	CString tempStr;
	CString strGraphOption("Graph;\r\n");
	strGraphOption += lpChartType;
	strGraphOption += ";";
	strGraphOption +=lpChartName;
	strGraphOption += ";";
	LONG lSize = rArrayPacketList.GetSize();
	for(long lIndex=0;lIndex<lSize;lIndex++)
	{
		if(lIndex)	strGraphOption += ",";
		strGraphOption += rArrayPacketList.GetAt(lIndex);		
	}
	if(strChartType==g_strChartType[0])
	{
		strGraphOption += ";0;0;0;0;1;0";
	}
	strGraphOption += ";\r\n";

	// "�׷���Ÿ��;�׷�����Ÿ��;�׸��½�Ÿ��;�̸�;title;����;��1;��2;��3;��4;��5;��6;
	//  ���ä��;�϶�ä��;������1;������2;������3;����;\r\n"
	
	if(strChartType==g_strChartType[0])
	{
		strGraphOption += "0;";
	}
	else if(strChartType==g_strChartType[1])
	{
		strGraphOption += "1;";
	}
	else if(strChartType==g_strChartType[2])
	{
		strGraphOption += "2;";
	}
	else
	{
		ASSERT(0); // Error !! 
	}
	tempStr.Format("%d;",lChartStyle);
	strGraphOption += tempStr;
	strGraphOption += "0;"; 
	strGraphOption += lpChartName;
	strGraphOption += ";";
	strGraphOption += lpChartTitle;
	tempStr.Format(";0;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d;%d;%d;%d;%d;\r\n",
		GetRValue(clrGrp1),GetGValue(clrGrp1), GetBValue(clrGrp1),
		GetRValue(clrGrp2),GetGValue(clrGrp2), GetBValue(clrGrp2),
		GetRValue(clrGrp3),GetGValue(clrGrp3), GetBValue(clrGrp3),
		GetRValue(clrGrp4),GetGValue(clrGrp4), GetBValue(clrGrp4),
		GetRValue(clrGrp5),GetGValue(clrGrp5), GetBValue(clrGrp5),
		GetRValue(clrGrp6),GetGValue(clrGrp6), GetBValue(clrGrp6),
		bUpFilled,bDownFilled,
		lUpLineWidth,lDownLineWidth,bHideGrp);
	strGraphOption += tempStr;
	
	strGraphOption += "GraphEnd;\r\n";	
	return strGraphOption;
}

long CChartItemData::GetData()
{
	if(m_strTRCode == QUERY_STRJONGMOK)
		return (long)m_pKB0602_ChartData; 
	else if(m_strTRCode == QUERY_STRUPJONG)
		return (long)m_pKB0603_ChartData;		
	else if(m_strTRCode == QUERY_STRFUTOPT)
		return (long)m_pKB0604_ChartData;
	else if(m_strTRCode == QUERY_STRSTOCKFUTOPT)
		return (long)m_pKB1013_ChartData;
	else if(m_strTRCode == QUERY_STRFOREIGN)
		return (long)m_pKB0606_ChartData;
	else if(m_strTRCode == QUERY_STRMARKET)
		return (long)m_pSt10450Data;			
//@��������
//	else if(m_strTRCode == QUERY_STRELW)
//		return (long)m_pSt01007Data;
//@��������
	return 0L;
}

void CChartItemData::RemoveRealItem()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveRealItem::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	CString strRealItemRegistered;
	int nSize = m_strArrayRealItemRegistered.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		strRealItemRegistered = m_strArrayRealItemRegistered.GetAt(nPos);
		m_pChartItem->RemoveRealItem(strRealItemRegistered);
	}
	m_strArrayRealItemRegistered.RemoveAll();
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveRealItem::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
}

void CChartItemData::SetChartNumericValue(int nValue)
{
	if(m_strTRCode==QUERY_STRJONGMOK)
	{
		if(nValue == PERCENT_TYPE)
		{
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[7]);
		}
		else if(nValue == WON_TYPE)
		{
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[0]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[0]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[0]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[0]);
		}
		else if(nValue == DOLLAR_TYPE)
		{
#ifdef _DEBUG
//				OutputDebugString("==============================================================");
//				AfxMessageBox(" CChartItemData::SetChartNumericValue �޷� Ÿ�� ���� �ʿ�	 ");
//				OutputDebugString("==============================================================");
#endif			
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[6]);
		}
	}	
}

void CChartItemData::AddRealItem()
{
	BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
	
	if(m_strTRCode == QUERY_STRJONGMOK)
	{
			CString strRealItem;	
			if(IsPrimaryChartItemData)
			{
				// �ڷ�����
				strRealItem = g_strPacketName[0];
				strRealItem += '.';
				strRealItem += "S31.time.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);
				m_strArrayRealItemRegistered.Add(strRealItem);

				// �ŷ���
//				strRealItem = g_strPacketName[5];
//				strRealItem += '.';
//				strRealItem += "S31.volume.";
//				strRealItem += m_strCode;
//				strRealItem += ';';
//				m_pChartItem->AddRealItem(strRealItem);		
//				m_strArrayRealItemRegistered.Add(strRealItem);

				//if(m_pChartItem->GetUseRateValue())
				if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
				{
					// ����
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "S31.chgrate.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
				else
				{
					// ����
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "S31.price.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);

/*
					if(m_pChartItem->GetType() == DAY_DATA_CHART)
					{	
						// �ð�
						strRealItem = "�ð�GuideLine";
						strRealItem += '.';
						strRealItem += "58.23.";
						strRealItem += m_strCode;
						strRealItem += ';';
						m_pChartItem->AddRealItem(strRealItem);
						m_strArrayRealItemRegistered.Add(strRealItem);
						// �� 
						strRealItem = "��GuideLine";
						strRealItem += '.';
						strRealItem += "58.7.";
						strRealItem += m_strCode;
						strRealItem += ';';
						m_pChartItem->AddRealItem(strRealItem);
						m_strArrayRealItemRegistered.Add(strRealItem);
						// ����
						strRealItem = "����GuideLine";
						strRealItem += '.';
						strRealItem += "58.8.";
						strRealItem += m_strCode;
						strRealItem += ';';
						m_pChartItem->AddRealItem(strRealItem);
						m_strArrayRealItemRegistered.Add(strRealItem);
					}*/


				}
			}
	}		
	else if(m_strTRCode == QUERY_STRUPJONG)
	{
			CString strRealItem;
			if(IsPrimaryChartItemData)
			{
				// �ڷ�����
				strRealItem = g_strPacketName[0];
				strRealItem += '.';
				strRealItem += "JS0.time.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);
				m_strArrayRealItemRegistered.Add(strRealItem);
				// �ŷ���
//				strRealItem = g_strPacketName[5];
//				strRealItem += '.';
//				strRealItem += "JS0.volume.";
//				strRealItem += m_strCode;
//				strRealItem += ';';
//				m_pChartItem->AddRealItem(strRealItem);		
//				m_strArrayRealItemRegistered.Add(strRealItem);
				if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
				{
					// ����
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "JS0.chgrate.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
				else
				{
					// ����
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "JS0.jisu.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
			}
			else
			{
				//if(m_pChartItem->GetUseRateValue())
				if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
				{
					// ����
					strRealItem = GetChartItemDataTitle();
					strRealItem += '.';
					strRealItem += "JS0.chgrate.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
				else
				{
					// ����
					strRealItem = GetChartItemDataTitle();
					strRealItem += '.';
					strRealItem += "JS0.jisu.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}

			}
	}		
	else if(m_strTRCode == QUERY_STRFUTOPT)
	{
			CString strRealItem;
			if(IsPrimaryChartItemData)
			{
				CString strCode = GetCode();
				if(strCode[0] == '1' || strCode[0] == '4')	// Future
				{
					// �ڷ�����
					strRealItem = g_strPacketName[0];
					strRealItem += '.';
					strRealItem += "SC0.time.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
					// �ŷ���
//					strRealItem = g_strPacketName[5];
//					strRealItem += '.';
//					strRealItem += "SC0.volume.";
//					strRealItem += m_strCode;
//					strRealItem += ';';
//					m_pChartItem->AddRealItem(strRealItem);		
//					m_strArrayRealItemRegistered.Add(strRealItem);
					// ����
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "SC0.price.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
				else
				{
					// �ڷ�����
					strRealItem = g_strPacketName[0];
					strRealItem += '.';
					strRealItem += "OC0.time.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
					// �ŷ���
//					strRealItem = g_strPacketName[5];
//					strRealItem += '.';
//					strRealItem += "OC0.volume.";
//					strRealItem += m_strCode;
//					strRealItem += ';';
//					m_pChartItem->AddRealItem(strRealItem);		
//					m_strArrayRealItemRegistered.Add(strRealItem);
					// ����
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "OC0.price.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
			}
//			CString strRealItem;
//			if(IsPrimaryChartItemData)
//			{
//				//=============================================================
//				// ����, �ɼ���  �����Ѵ�.
//				CString sPacketName = _T("");
//				m_strCode.Remove(' ');
//
//				DWORD dw = m_pChartItem->GetCodeType_By_DBMgr(m_strCode);					
//				switch(dw)
//				{ 	
//					case CODE_QFUTURE	:
//					case CODE_FSPREAD	:
//					case CODE_QFUTURES	:
//					case CODE_FUTURE	: //���� 				
//						sPacketName = "65";
//						break;
//					case CODE_OPTION	: //�ɼ�	
//					case CODE_QOPTION	:						
//					case CODE_QOPTIONS	: //�ڽ��� �ɼ� 
//					case CODE_JOPTION	: //�ֽ� �ɼ�
//						sPacketName = "67";									
//						break;
//					case CODE_FSTAR		:
//					case CODE_FSSPREAD	:
//						sPacketName = "90";
//						break;
//					default:
//						return;
//						break;
//				}
//				//=============================================================
//				// �ڷ�����
//				strRealItem = g_strPacketName[0];
//				strRealItem += '.';
//				strRealItem += sPacketName + ".2.";
//				strRealItem += m_strCode;
//				strRealItem += ';';
//				m_pChartItem->AddRealItem(strRealItem);
//				m_strArrayRealItemRegistered.Add(strRealItem);
//				// �ŷ���
//				strRealItem = g_strPacketName[5];
//				strRealItem += '.';
//				strRealItem += sPacketName + ".10.";
//				strRealItem += m_strCode;
//				strRealItem += ';';
//				m_pChartItem->AddRealItem(strRealItem);		
//				m_strArrayRealItemRegistered.Add(strRealItem);
//				// ����
//				strRealItem = g_strPacketName[4];
//				strRealItem += '.';
//				strRealItem += sPacketName + ".3.";
//				strRealItem += m_strCode;
//				strRealItem += ';';
//				m_pChartItem->AddRealItem(strRealItem);
//				m_strArrayRealItemRegistered.Add(strRealItem);
//			}
	}
//	else if(m_strTRCode == QUERY_STRCOMMODITY)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//	{
//		CString strRealItem;
//		if(IsPrimaryChartItemData)
//		{
//			CString strCode = GetCode();
//			// �ڷ�����
//			strRealItem = g_strPacketName[0];
//			strRealItem += '.';
//			strRealItem += "OC0.time.";
//			strRealItem += m_strCode;
//			strRealItem += ';';
//			m_pChartItem->AddRealItem(strRealItem);
//			m_strArrayRealItemRegistered.Add(strRealItem);
//			// �ŷ���
//			strRealItem = g_strPacketName[5];
//			strRealItem += '.';
//			strRealItem += "OC0.volume.";
//			strRealItem += m_strCode;
//			strRealItem += ';';
//			m_pChartItem->AddRealItem(strRealItem);		
//			m_strArrayRealItemRegistered.Add(strRealItem);
//			// ����
//			strRealItem = g_strPacketName[4];
//			strRealItem += '.';
//			strRealItem += "OC0.price.";
//			strRealItem += m_strCode;
//			strRealItem += ';';
//			m_pChartItem->AddRealItem(strRealItem);
//			m_strArrayRealItemRegistered.Add(strRealItem);
//		}
//	}
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
BOOL CChartItemData::SetLoadedPacketData(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart)
{
	return m_pChartItem->SetPacketDataRQ(lpszRQ, GetPacketName(),GetData(),GetDataSize(),FALSE);
}

BOOL CChartItemData::SetPacketData(BOOL bReset)
{
	BOOL bRetValue = FALSE;
	if(!m_pChartItem->IsPrimaryChartItemData(this)&&m_lCntRequiredByPrim)
	{		
		if(m_lCntRequired>m_lCntTemp)
		{
			m_bIsFilled = TRUE;
			m_pChartItem->SetNext(FALSE);
			bRetValue = m_pChartItem->SetPacketData(GetPacketName(),GetData(),GetDataSize(),bReset);
			long lSize = GetCntChartItemDataTitle();
			if(m_lStartDate)
			{
				for(long lPos=0;lPos<lSize;lPos++)
				{
					m_pChartItem->SetStartPeriodOfPacket(GetChartItemDataTitle(lPos),m_lStartDate,TRUE);
				}			
			}
			goto ExitLoop;
		}
		else
		{
			m_bIsFilled = FALSE;
			m_pChartItem->SetNext(TRUE);
		}
	}
	else
	{
		m_bIsFilled = TRUE;
		m_pChartItem->SetNext(FALSE);
	}

	if(!ProcRequestCntAfterQuery(m_lCntTemp))
	{
		bRetValue = FALSE;
		goto ExitLoop;
	}


	bRetValue =  m_pChartItem->SetPacketData(GetPacketName(),GetData(),GetDataSize(),bReset);	
	
ExitLoop:
	RemoveData();
	return bRetValue;
}

BOOL CChartItemData::SetPacketDataInAdditionPacket(BOOL bReset)
{
	BOOL bRetValue = FALSE;
	if(!m_pChartItem->IsPrimaryChartItemData(this)&&m_lCntRequiredByPrim)
	{		
		if(m_lCntRequired>m_lCntTemp)
		{
			m_bIsFilled = TRUE;
#ifdef _DEBUG
			OutputDebugString("CChartItemData::SetPacketDataInAdditionPacket .... m_bIsFillded = TRUE");
#endif 
			m_pChartItem->SetNext(FALSE);
			bRetValue = m_pChartItem->SetPacketDataInAdditionPacket(GetPacketName(),GetData(),GetDataSize(),bReset);
			long lSize = GetCntChartItemDataTitle();
			if(m_lStartDate)
			{
				for(long lPos=0;lPos<lSize;lPos++)
				{
					m_pChartItem->SetStartPeriodOfPacket(GetChartItemDataTitle(lPos),m_lStartDate,TRUE);
				}			
			}
			goto ExitLoop;
		}
		else
		{
			m_bIsFilled = FALSE;
#ifdef _DEBUG
			OutputDebugString("CChartItemData::SetPacketData .... m_bIsFillded = FALSE#2");
#endif 

			m_pChartItem->SetNext(TRUE);
		}
	}
	else
	{
		m_bIsFilled = TRUE;
#ifdef _DEBUG
			OutputDebugString("CChartItemData::SetPacketData .... m_bIsFillded = TRUE#3");
#endif 

		m_pChartItem->SetNext(FALSE);
	}

//	m_bIsFilled = TRUE;	

	if(!ProcRequestCntAfterQuery(m_lCntTemp))
	{
		bRetValue = FALSE;
		goto ExitLoop;
	}


	//bRetValue =  m_pChartItem->SetPacketDataInAdditionPacket(GetPacketName(),GetData(),GetDataSize(),bReset);	
	bRetValue =  m_pChartItem->SetPacketDataInAdditionPacket(GetPacketName(),GetData(),GetDataSize(),!m_bNextMarketData);
	
ExitLoop:
	RemoveData();
	
	SetBNextMarketData(FALSE);

	return bRetValue;
}





BOOL CChartItemData::IsFilled()
{
	return m_bIsFilled;
}

long CChartItemData::GetCntChartItemDataTitle()
{
	return m_strArrayGraphTitle.GetSize();
}

CString CChartItemData::GetChartItemDataTitle(long nIndex /*= 0*/)
{
	if(m_strArrayGraphTitle.GetSize()>nIndex)
	{
		return m_strArrayGraphTitle.GetAt(nIndex);
	}
	return "";
}

void CChartItemData::ClearData() 
{ 
	m_bIsFilled = FALSE;
	
#ifdef _DEBUG
			OutputDebugString("CChartItemData::ClearData .... m_bIsFillded = FALSE");
#endif 

}

// ELW INPUT ====================================================================
//    char    Scode    [15];       /* �����ڵ�                             */
//    char    Sdate    [ 8];       /* �˻� ������                          */
//    char    Smaxbong [ 4];       /* �˻��� ������ ����                   */
//    char    Sunit    [ 3];       /* Tick, Sec(10s, 30s), Min, Day Number */
//    char    Sbtngb   [ 1];       /* ƽ(0)/��(1)/��(2)/��(3)/��(4)        */
//    char    Snextgb  [ 1];       /* ����(1) / ��ȸ(0) ������             */
//    char    Snextkey [14];       /* ���� / ���� Fetch Key                */
//================================================================================
// �������� �ҷ����� : �������� - ojtaso (20070514)
//CString CChartItemData::GetQueryDataELW(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
//{
////@��������
////	S01007_I stOutput;
////@��������
////@�����߰�
//	p0605InBlock stOutput;
////@�����߰�
//
//	char Temp[64];
//	char chFlag;
//
//	memset(&stOutput,0x00,sizeof(stOutput)); 	stOutput.chNull = NULL;
//	memset(stOutput.Scode, 0x20, sizeof(stOutput.Scode));
//	if(lpSelChart)
//		memcpy(stOutput.Scode,(LPCTSTR)lpSelChart->m_strItemCode,lpSelChart->m_strItemCode.GetLength());
//	else
//		memcpy(stOutput.Scode,(LPCTSTR)m_strCodeForRequest,m_strCodeForRequest.GetLength());
//	//	memcpy(stOutput.shcode,(LPCTSTR)m_strCodeForRequest,sizeof(stOutput.shcode));
//	//	stOutput.shcode[7] = ' ';	
//	memcpy(stOutput.Sdate, m_pChartItem->GetEndDateRequested(), sizeof(stOutput.Sdate));
//
///*
//	if(lpSelChart)
//		sprintf(Temp,"%04d",lpSelChart->m_lCount);
//	else
//*/
//		sprintf(Temp,"%04d",GetCntForQuery(bNext));
//	if(bNext)
//	{		
//		chFlag = '1';
//	}
//	else
//	{
//		chFlag = '0';
//	}	
//	char chUseRevised = '0';
//	if(m_pChartItem->GetUseRevisedValue())
//	{
//		chUseRevised = '1';
//	}
//	//05.06.22 add by sy, nam =====================================================
//	//	if(m_pChartItem->GetUseRateValue())
//	//	{
//	//		chUseRateValue = '1';
//	//	}	
//	char chUseRateValue = '0';
//	int nNumericValueType = lpSelChart ? lpSelChart->m_nChartNumericValueType : m_pChartItem->GetChart_NumericValueType();
//	if(nNumericValueType == WON_TYPE)	
//		chUseRateValue = '0';
//	else if(nNumericValueType == PERCENT_TYPE)	
//		chUseRateValue = '1';
//	else if(nNumericValueType == DOLLAR_TYPE)	
//		chUseRateValue = '2';
//	//===============================================================================
//
//	char chUseFackedCandle = '1';
//	if(m_pChartItem->GetBNormalizedMinuteTimeRule())// TRUE�̸� ��� 
//		chUseFackedCandle = '0'; // '0':����� '1':�Ǻ�           
//
//
//
//	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
//
//
//
//	memcpy(stOutput.Smaxbong, Temp, sizeof(stOutput.Smaxbong));
//
//	//==================================================================================================================================
//	// ƽ/��/��... ���� ���а��� ������ ���� ������.. tick/min�̶�� ��Ҹ� ���ְ�
//	// unit�� ƽ/��/�� �� ������ �Է��Ѵ�.
//	// 2005. 05. 23 by sy, nam
//	//==================================================================================================================================
//	if(chTypeRequested == SEC10_DATA_CHART)
//		memcpy(stOutput.Sunit, "10s", 3);
//	else if (chTypeRequested == SEC30_DATA_CHART)
//		memcpy(stOutput.Sunit, "30s", 3);
//	else
//	{
//		if(lpSelChart)
//			memcpy(stOutput.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
//		else
//			memcpy(stOutput.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());
//	}
//
//	//==================================================================================================================================
//	memcpy(stOutput.Sbtngb, &chTypeRequested, sizeof(stOutput.Sbtngb));
//	//	memcpy(stOutput.modGb,&chUseRevised,sizeof(stOutput.modGb));
//	memcpy(stOutput.Snextgb, &chFlag, sizeof(stOutput.Snextgb));
//
//	// 2005. 09. 21 by sy, nam ==============================================================
//	// memset 0x00���� �ϴ� '\0'�� ���� �� �����Ͱ� �߷��� 
//	// �Ǻ� ��� �� ������ ����� �� �Ǵ� ��찡 �����.
//	if(chFlag == '1')//���� ��ȸ�� ���
//	{
//		CString sKey;
//		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
//		//if(atoi(sKey) > 0)
//		if(sKey.GetLength()>0)
//			memcpy(stOutput.Snextkey, m_chkey, sizeof(stOutput.Snextkey));
//		else
//			memset(stOutput.Snextkey, 0x20, sizeof(stOutput.Snextkey));
//	}
//	else //�ܼ� ��ȸ �� ���.
//		memset(stOutput.Snextkey, 0x20, sizeof(stOutput.Snextkey));
//
//	//memcpy(stOutput.nkey,m_chkey,sizeof(stOutput.nkey)); <== original
//	//=======================================================================================
//
//
//
//	//	memcpy(stOutput.nDataGubun,&chUseRateValue,sizeof(stOutput.nDataGubun));
//	//	memcpy(stOutput.ImgRgb,&chUseFackedCandle,sizeof(stOutput.ImgRgb));
//	//==================================================================================================================================
//	// ���� ���� ������ �ִ´�.
//	// 2005. 08. 11 by sy, nam
//	//==================================================================================================================================
//	//	CString strReplayFlag = m_pChartItem->GetReplayFlag();
//	//	CString strBeforeDay  = m_pChartItem->GetReplayDayBefore();
//	//	CString strReplayTime = m_pChartItem->GetReplayStartTime();
//	//
//	//	memset(stOutput.Itime	 ,  0x00			,   sizeof(stOutput.Itime)		);
//	//
//	//	memcpy(stOutput.Irecordgb, 	strReplayFlag	, 	sizeof(stOutput.Irecordgb)	);/* ���� ���а�(1)		*///<========2005.08.11
//	//
//	//	memcpy(stOutput.nDayBefore, strBeforeDay	,	sizeof(stOutput.nDayBefore)	);/* ���� ���� n����	*/
//	//
//	//	memcpy(stOutput.Itime	 , 	strReplayTime	,	strReplayTime.GetLength()	);/* ����ð� (hhmmss)	*///<========2005.08.11
//
////@�����߰�
//	CString strFidInfo("0                                                                               ");
//	CString strTrInfo;
//	char* pszTrInfo = strTrInfo.GetBuffer(sizeof(stOutput));
//	memcpy(pszTrInfo, (char*)&stOutput, sizeof(stOutput));
//	strTrInfo.ReleaseBuffer();
//	CString strRecordInfo("1               000002003004005009010012013|2               000001002003004005007;");
//	CString strOutput;
//	strOutput = strFidInfo + strTrInfo + strRecordInfo;
//	return strOutput;
////@�����߰�
//
//	return (char*)&stOutput;
//}

//OUTPUT ELW
BOOL CChartItemData::MakeChartDataELW(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	S01007_O stOutput;
	memset(&stOutput, 0x20, sizeof(S01007_O));
	if(nLength>sizeof(stOutput))
	{			
		ASSERT(0);
		return FALSE;
	}
	S01007_O *pRecvData = NULL;
	pRecvData = (S01007_O *)szData;
	if(pRecvData == NULL) return FALSE;

	//������ǥ������ chkeyOld ����
	memset(m_chkeyOld,0x00,sizeof(m_chkeyOld));
	memcpy(m_chkeyOld,m_chkey,sizeof(pRecvData->nkey));

	memset(m_chkey,0x00,sizeof(m_chkey));
	memcpy(m_chkey,pRecvData->nkey,sizeof(pRecvData->nkey));

	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
	m_pChartItem->EnableRequestMoreButton(TRUE);

	//���� 05.09.09 ================================================
//@Solomon
// 	CString sKey;
// 	sKey.Format("%21.21s", pRecvData->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 	//===============================================================
// 	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);

	CString sKey;
	sKey.Format("%*.*s", sizeof(pRecvData->nkey), sizeof(pRecvData->nkey), pRecvData->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pRecvData->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)m_chkey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pSt01007Data)
	{
		m_pSt01007Data = new S01007_CHART;
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp,pRecvData->name);
	m_strName = szTemp;
	m_strName.TrimRight();

	MEMCPYWITHNULL(szTemp,pRecvData->tmp);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;

	long lPosForChart = 0;
	if(nLength < (&pRecvData->tmp[0] - &pRecvData->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pRecvData->tmp[0] - &pRecvData->name[0]);
	memcpy(m_pSt01007Data,&pRecvData->tmp[0],lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp,pRecvData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp,pRecvData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}

// ������ �ٲ�� PacketName�� �ٲ۴� : �������� - ojtaso (20070711)
void CChartItemData::ChangePacketNameArray(LPCTSTR lpszTRCode)
{
	m_strArrayPacketName.RemoveAll();

	m_strArrayPacketName.Add(g_strPacketName[0]);
	m_strArrayPacketName.Add(g_strPacketName[1]);
	m_strArrayPacketName.Add(g_strPacketName[2]);
	m_strArrayPacketName.Add(g_strPacketName[3]);
	m_strArrayPacketName.Add(g_strPacketName[4]);
/*
	if(::lstrcmp(lpszTRCode, QUERY_STRFOREIGN))
	{
		m_strArrayPacketName.Add(g_strPacketName[5]);
		m_strArrayPacketName.Add(g_strPacketName[6]);
		m_strArrayPacketName.Add(g_strPacketName[7]);
	}
*/
}

void CChartItemData::OnSelectChartItem(ST_SELCHART* lpSelChart)
{
	SetTRCode(lpSelChart->m_strChartGubun);
	m_strCodeForRequest = lpSelChart->m_strItemCode;
//	memcpy(m_chkey, (LPCTSTR)lpSelChart->m_strKey, lpSelChart->m_strKey.GetLength());
}
