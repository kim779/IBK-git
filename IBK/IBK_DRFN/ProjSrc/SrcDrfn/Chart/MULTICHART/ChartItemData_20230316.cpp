// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartItemData.h"
#include "ChartItem.h"
#include "StdDialog.h"
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
	Init(pChartItem,strTRCode);
}

CChartItemData::~CChartItemData()
{
	ClearChartItemData();
}

void CChartItemData::Init(CChartItem *pChartItem, CString strTRCode)
{
	memset(m_chkey,'0',sizeof(m_chkey));

	m_pKB0602_ChartData = NULL;
	m_pKB0603_ChartData = NULL;
	m_pKB0604_ChartData = NULL;
	m_pKB1013_ChartData = NULL;
	m_pKB0606_ChartData = NULL;
	m_pKB1015_ChartData = NULL;
	m_pKB0515_ChartData = NULL;

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
}

// ���õ� ��Ʈ�� ���� ���� : �������� - ojtaso (20070215)
void CChartItemData::SetTRCode(LPCTSTR lpszTRCode)
{
	if(strlen(lpszTRCode)==0 || m_strTRCode.Compare(lpszTRCode)==0)
		return;
	
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
		char *pData = (char*)m_pKB0602_ChartData;
		delete [] pData;
		m_pKB0602_ChartData = NULL;
	}
	if(m_pKB0603_ChartData)
	{
		char *pData = (char*)m_pKB0603_ChartData;
		delete [] pData;
		m_pKB0603_ChartData = NULL;
	}	

	if(m_pKB0604_ChartData)
	{
		char *pData = (char*)m_pKB0604_ChartData;
		delete [] pData;
		m_pKB0604_ChartData = NULL;
	}

	if(m_pKB1013_ChartData)
	{
		char *pData = (char*)m_pKB1013_ChartData;
		delete [] pData;
		m_pKB1013_ChartData = NULL;
	}

	if(m_pKB0606_ChartData)
	{
		char *pData = (char*)m_pKB0606_ChartData;
		delete [] pData;
		m_pKB0606_ChartData = NULL;
	}

	if(m_pKB1015_ChartData)
	{
		char *pData = (char*)m_pKB1015_ChartData;
		delete [] pData;
		m_pKB1015_ChartData = NULL;
	}

	if (m_pKB0515_ChartData)
	{
		char *pData = (char*)m_pKB0515_ChartData;
		delete [] pData;
		m_pKB0515_ChartData = NULL;
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
	if(bInitKey)
		memset(m_chkey,'0',sizeof(m_chkey));
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItemData::GetQueryData(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	m_lDataSize = 0;
	m_lCnt = 0;
	CString strTRCode = lpSelChart ? lpSelChart->m_strChartGubun : m_strTRCode;

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartItemData::GetQueryData  strTRCode(%s)\n", __LINE__, strTRCode);
// 		OutputDebugString(szDebug);
// 	}

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
	else if(strTRCode == QUERY_STRFOJIPYO)
		return GetQueryData_KB_p0515(bNext, lpSelChart);

	return "";
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0602(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0602_InRec1 kb0602InRec1;

	// �ʱ�ȭ
	memset(&kb0602InRec1, 0x20, sizeof(kb0602InRec1));

	// �����ڵ�
	memset(kb0602InRec1.shcode, 0x20, sizeof(kb0602InRec1.shcode));
	if(lpSelChart)
	{
		memcpy(kb0602InRec1.shcode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
		//KHD: ����� ��Ʈ���� �ֽĶ��� �߻��ؾ� �ϱ⶧���� ����� ���� : 
		((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_pInputBarDlg->SendSymbol(lpSelChart->m_strItemCode);
//		m_pChartItem->SetOldChartCode(lpSelChart->m_strItemCode);
	}
	else
	{
		memcpy(kb0602InRec1.shcode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
		((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_pInputBarDlg->SendSymbol(m_strCodeForRequest);
//		m_pChartItem->SetOldChartCode(m_strCodeForRequest);
	}

	// �˻�������
	memcpy(kb0602InRec1.date, m_pChartItem->GetEndDateRequested(), sizeof(kb0602InRec1.date));

	// ��ȸ�Ǽ�
	char Temp[64];
	sprintf(Temp,"%04d",GetCntForQuery(bNext));
	memcpy(kb0602InRec1.num, Temp, sizeof(kb0602InRec1.num));

	// ��ȸ����
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if(lpSelChart)
		memcpy(kb0602InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
	else
		memcpy(kb0602InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());

	// ��ȸ����
	memcpy(kb0602InRec1.button,&chTypeRequested,sizeof(kb0602InRec1.button));

	// �����ְ�
	char chUseRevised = '0';
	if(m_pChartItem->GetUseRevisedValue())
	{
		chUseRevised = '1';
	}
	memcpy(kb0602InRec1.modGb,&chUseRevised,sizeof(kb0602InRec1.modGb));

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
	memcpy(kb0602InRec1.nflag,&chFlag,sizeof(kb0602InRec1.nflag));

	// �˻�Ű
	memset(kb0602InRec1.nkey,0x20,sizeof(kb0602InRec1.nkey));
	if(chFlag == '1')//���� ��ȸ�� ���
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();
		int i = sizeof(KB_p0602_InRec1);
		memcpy(kb0602InRec1.nkey, strKey, min(sizeof(kb0602InRec1.nkey), strKey.GetLength()));

	}
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		if(sKey.GetLength()>0)
			memcpy(kb0602InRec1.nkey,m_chkey,sizeof(kb0602InRec1.nkey));
		else
			memset(kb0602InRec1.nkey,0x20,sizeof(kb0602InRec1.nkey));
	}
	else //�ܼ� ��ȸ �� ���.
		memset(kb0602InRec1.nkey,0x20,sizeof(kb0602InRec1.nkey));
*/

	// ���ݱ���
	char chUseRateValue = '0';
	int nNumericValueType = lpSelChart ? lpSelChart->m_nChartNumericValueType : m_pChartItem->GetChart_NumericValueType();

	if(nNumericValueType == WON_TYPE)	
		chUseRateValue = '0';
	else if(nNumericValueType == PERCENT_TYPE)	
//		chUseRateValue = '1';		// ����Ʈ���� �������� 5�� �����
		chUseRateValue = '5';
	else if(nNumericValueType == DOLLAR_TYPE)
	{
		chUseRateValue = '2';
		//@Solomon	091123SM072	-->
		CString strRate = ((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_strDollar;
		memcpy(kb0602InRec1.usRate, (LPTSTR)(LPCTSTR)strRate, min(strRate.GetLength(), sizeof(kb0602InRec1.usRate)));
		//@Solomon	091123SM072	<--
	}
	memcpy(kb0602InRec1.nDataGubun, &chUseRateValue, sizeof(kb0602InRec1.nDataGubun));

	// ������
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())// TRUE�̸� ��� 
		chUseFackedCandle = '0'; // '0':����� '1':�Ǻ�           
	memcpy(kb0602InRec1.ImgRgb, &chUseFackedCandle, sizeof(kb0602InRec1.ImgRgb));

	// ���ⱸ�а�
	memcpy(kb0602InRec1.Irecordgb, m_pChartItem->GetReplayFlag(), sizeof(kb0602InRec1.Irecordgb));
	
	// �������n����
	memcpy(kb0602InRec1.nDayBefore, m_pChartItem->GetReplayDayBefore(),	sizeof(kb0602InRec1.nDayBefore));

	// ����ð�(hhmmss)
	CString strReplayTime = m_pChartItem->GetReplayStartTime();
	memcpy(kb0602InRec1.Itime, m_pChartItem->GetReplayStartTime(), strReplayTime.GetLength());
	
	// ������ȸ
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	|| 
		chTypeRequested == WEEK_DATA_CHART	|| 
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
		else if (strQueryOpt == "2")
			chNowData = '2';
	}
	memcpy(kb0602InRec1.nowData, &chNowData, sizeof(kb0602InRec1.nowData));

	CString strKB0602Info;
	char* szKB0602Info = strKB0602Info.GetBuffer(sizeof(kb0602InRec1));
	memcpy(szKB0602Info, (char*)&kb0602InRec1, sizeof(kb0602InRec1));
	strKB0602Info.ReleaseBuffer();
	CString strKB0602InRec1;
	strKB0602InRec1 = strKB0602Info;
	return strKB0602InRec1;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0603(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0603_InRec1 kb0603InRec1;

	// �ʱ�ȭ
	memset(&kb0603InRec1, 0x20, sizeof(kb0603InRec1));

	if(m_strCodeForRequest.GetLength() != UPJONG_LENGTH) 
	{
		ASSERT(0);
		return "";
	}

	// �屸�� / �����ڵ�
	if(lpSelChart)
	{
		kb0603InRec1.Sjanggb[0] = '1';		
		kb0603InRec1.Supcode[0] = lpSelChart->m_strItemCode[0];	
		kb0603InRec1.Supcode[1] = lpSelChart->m_strItemCode[1];	
		kb0603InRec1.Supcode[2] = lpSelChart->m_strItemCode[2];

//		m_pChartItem->SetOldChartCode(lpSelChart->m_strItemCode);
	}
	else
	{
		kb0603InRec1.Sjanggb[0] = '1';		
		kb0603InRec1.Supcode[0] = m_strCodeForRequest[0];	
		kb0603InRec1.Supcode[1] = m_strCodeForRequest[1];	
		kb0603InRec1.Supcode[2] = m_strCodeForRequest[2];

//		m_pChartItem->SetOldChartCode(m_strCodeForRequest);
	}
	// ������
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())// TRUE�̸� ��� 
		chUseFackedCandle = '0'; // '0':����� '1':�Ǻ�           
	memcpy(kb0603InRec1.ImgRgb, &chUseFackedCandle, sizeof(kb0603InRec1.ImgRgb));

	// �˻�������
	memcpy(kb0603InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(kb0603InRec1.Sdate));

	// �˻��ҵ����Ͱ���
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(kb0603InRec1.Smaxbong, Temp, sizeof(kb0603InRec1.Smaxbong));
	
	// N��(ƽ�н���)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if(lpSelChart)
		memcpy(kb0603InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
	else
		memcpy(kb0603InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());

	// ��ȸ����
	memcpy(kb0603InRec1.Sbtngb, &chTypeRequested, sizeof(kb0603InRec1.Sbtngb));

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
	memcpy(kb0603InRec1.Snextgb, &chFlag, sizeof(kb0603InRec1.Snextgb));

	// ��ġŰ
	memset(kb0603InRec1.Snextkey, 0x20, sizeof(kb0603InRec1.Snextkey));
	if(chFlag == '1')	// ���� ��ȸ�� ���
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(kb0603InRec1.Snextkey, strKey, min(sizeof(kb0603InRec1.Snextkey), strKey.GetLength()));
	}
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();

		if(sKey.GetLength()>0)
			memcpy(kb0603InRec1.Snextkey, m_chkey, sizeof(kb0603InRec1.Snextkey));
		else
			memset(kb0603InRec1.Snextkey, 0x20, sizeof(kb0603InRec1.Snextkey));
	}
	else				// �ܼ� ��ȸ�� ���.
		memset(kb0603InRec1.Snextkey, 0x20, sizeof(kb0603InRec1.Snextkey));
*/
	
	kb0603InRec1.SdataType[0] = '0';	//�߰�080521
	// ������ȸ
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	|| 
		chTypeRequested == WEEK_DATA_CHART	|| 
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
		else if (strQueryOpt == "2")
			chNowData = '2';
	}

	memcpy(kb0603InRec1.nowData, &chNowData, sizeof(kb0603InRec1.nowData));

	CString strKB0603Info;
	char* szKB0603Info = strKB0603Info.GetBuffer(sizeof(kb0603InRec1));
	memcpy(szKB0603Info, (char*)&kb0603InRec1, sizeof(kb0603InRec1));
	strKB0603Info.ReleaseBuffer();
	CString strKB0603InRec1;
	strKB0603InRec1 = strKB0603Info;
	return strKB0603InRec1;
}

CString CChartItemData::GetQueryData_KB_p0604(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0604_InRec1 kb0604InRec1;

	// �ʱ�ȭ
	memset(&kb0604InRec1, 0x20, sizeof(kb0604InRec1));

	// �����ڵ�
	memset(kb0604InRec1.Scode, 0x20, sizeof(kb0604InRec1.Scode));
	if(lpSelChart)
	{
		memcpy(kb0604InRec1.Scode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
//		m_pChartItem->SetOldChartCode(lpSelChart->m_strItemCode);
	}
	else
	{
		memcpy(kb0604InRec1.Scode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
//		m_pChartItem->SetOldChartCode(m_strCodeForRequest);
	}

	// �˻�������
	memcpy(kb0604InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(kb0604InRec1.Sdate));

	// �˻��ҵ����Ͱ���
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(kb0604InRec1.Smaxbong, Temp, sizeof(kb0604InRec1.Smaxbong));

	// N��(ƽ�н���)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if(lpSelChart)
		memcpy(kb0604InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
	else
		memcpy(kb0604InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());

	// ����(ƽ�����ֿ�)
	memcpy(kb0604InRec1.Sbtngb, &chTypeRequested, sizeof(kb0604InRec1.Sbtngb));

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
	memcpy(kb0604InRec1.Snextgb, &chFlag, sizeof(kb0604InRec1.Snextgb));

	// ��ġŰ
	memset(kb0604InRec1.Snextkey, 0x20, sizeof(kb0604InRec1.Snextkey));
	if(chFlag == '1')	// ���� ��ȸ�� ���
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(kb0604InRec1.Snextkey, strKey, min(sizeof(kb0604InRec1.Snextkey), strKey.GetLength()));
	}
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		if(sKey.GetLength()>0)
			memcpy(kb0604InRec1.Snextkey, m_chkey, sizeof(kb0604InRec1.Snextkey));
		else
			memset(kb0604InRec1.Snextkey, 0x20, sizeof(kb0604InRec1.Snextkey));
	}
	else				// �ܼ� ��ȸ�� ���
		memset(kb0604InRec1.Snextkey, 0x20, sizeof(kb0604InRec1.Snextkey));
*/

	kb0604InRec1.SdataType[0] = '0';	//�߰�080521

	// ������(����Ǻ�)
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())
	{
		chUseFackedCandle = '0'; // '0':����� '1':�Ǻ�           
	}
	memcpy(kb0604InRec1.Simgrgb, &chUseFackedCandle, sizeof(kb0604InRec1.Simgrgb));

	// ���ⱸ�а�
	memcpy(kb0604InRec1.Srecordgb, m_pChartItem->GetReplayFlag(), sizeof(kb0604InRec1.Srecordgb));

	// �������n����
	memcpy(kb0604InRec1.Sredate, m_pChartItem->GetReplayDayBefore(), sizeof(kb0604InRec1.Sredate));

	// ����ð�(hhmmss)
	memcpy(kb0604InRec1.Stime, m_pChartItem->GetReplayStartTime(), sizeof(kb0604InRec1.Stime));

	// ������ȸ
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
		else if (strQueryOpt == "2")
			chNowData = '2';
	}

	memcpy(kb0604InRec1.nowData, &chNowData, sizeof(kb0604InRec1.nowData));	// ������ȸ����

//<< 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
	kb0604InRec1.SgapRev[0] = '0';

	CString strCode;
	strCode.Format("%*.*s", sizeof(kb0604InRec1.Scode), sizeof(kb0604InRec1.Scode), kb0604InRec1.Scode);

	int nCodeType, nMarketType = ((CStdDialog*)m_pChartItem->m_pStdDialog)->GetMarketTypeFromCode(strCode, nCodeType);
	if (nMarketType == CMasterTypeInfo::COMMODITY_CHART)
	{
		if (m_pChartItem->m_bMonthGapRev)
			kb0604InRec1.SgapRev[0] = '1';
	}
//>> (Issue No : 0006572)

	CString strKB0604Info;
	char* szKB0604Info = strKB0604Info.GetBuffer(sizeof(kb0604InRec1));
	memcpy(szKB0604Info, (char*)&kb0604InRec1, sizeof(kb0604InRec1));
	strKB0604Info.ReleaseBuffer();
	CString strKB0604InRec1;
	strKB0604InRec1 = strKB0604Info;
	return strKB0604InRec1;
}

CString CChartItemData::GetQueryData_KB_d1013(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_d1013_InRec1 kb0605InRec1;

	// �ʱ�ȭ
	memset(&kb0605InRec1, 0x20, sizeof(kb0605InRec1));

	// �����ڵ�
	memset(kb0605InRec1.Scode, 0x20, sizeof(kb0605InRec1.Scode));
	if(lpSelChart)
	{
		memcpy(kb0605InRec1.Scode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
//		m_pChartItem->SetOldChartCode(lpSelChart->m_strItemCode);
	}
	else
	{
		memcpy(kb0605InRec1.Scode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
//		m_pChartItem->SetOldChartCode(m_strCodeForRequest);
	}

	// �˻�������
	memcpy(kb0605InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(kb0605InRec1.Sdate));

	// �˻��ҵ����Ͱ���
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(kb0605InRec1.Smaxbong, Temp, sizeof(kb0605InRec1.Smaxbong));

	// N��(ƽ�н���)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if(lpSelChart)
		memcpy(kb0605InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
	else
		memcpy(kb0605InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());

	// ����(ƽ�����ֿ� ��(01234 6))
	memcpy(kb0605InRec1.Sbtngb, &chTypeRequested, sizeof(kb0605InRec1.Sbtngb));

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
	memcpy(kb0605InRec1.Snextgb, &chFlag, sizeof(kb0605InRec1.Snextgb));

	// ��ġŰ
	memset(kb0605InRec1.Snextkey, 0x20, sizeof(kb0605InRec1.Snextkey));
	if(chFlag == '1')	// ���� ��ȸ�� ���
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(kb0605InRec1.Snextkey, strKey, min(sizeof(kb0605InRec1.Snextkey), strKey.GetLength()));
	}
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		//if(atoi(sKey) > 0)
		if(sKey.GetLength()>0)
			memcpy(kb0605InRec1.Snextkey, m_chkey, sizeof(kb0605InRec1.Snextkey));
		else
			memset(kb0605InRec1.Snextkey, 0x20, sizeof(kb0605InRec1.Snextkey));
	}
	else				// �ܼ� ��ȸ�� ���
		memset(kb0605InRec1.Snextkey, 0x20, sizeof(kb0605InRec1.Snextkey));
*/

	kb0605InRec1.SdataType[0] = '0';	//�߰�080521

	// ������(����Ǻ�)
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())
	{
		chUseFackedCandle = '0'; // '0':����� '1':�Ǻ�           
	}
	memcpy(kb0605InRec1.Simgrgb, &chUseFackedCandle, sizeof(kb0605InRec1.Simgrgb));

	// ���ⱸ�а�
	memcpy(kb0605InRec1.Srecordgb, m_pChartItem->GetReplayFlag(), sizeof(kb0605InRec1.Srecordgb));

	// �������n����
	memcpy(kb0605InRec1.Sredate, m_pChartItem->GetReplayDayBefore(), sizeof(kb0605InRec1.Sredate));

	// ����ð�(hhmmss)
	memcpy(kb0605InRec1.Stime, m_pChartItem->GetReplayStartTime(), sizeof(kb0605InRec1.Stime));

	// ������ȸ
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
		else if (strQueryOpt == "2")
			chNowData = '2';
	}

	memcpy(kb0605InRec1.nowData, &chNowData, sizeof(kb0605InRec1.nowData));	// ������ȸ����

//<< 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
	if (m_pChartItem->m_bMonthGapRev)
		kb0605InRec1.SgapRev[0] = '1';
	else
		kb0605InRec1.SgapRev[0] = '0';
//>> (Issue No : 0006572)

	CString strKB0605Info;
	char* szKB0605Info = strKB0605Info.GetBuffer(sizeof(kb0605InRec1));
	memcpy(szKB0605Info, (char*)&kb0605InRec1, sizeof(kb0605InRec1));
	strKB0605Info.ReleaseBuffer();
	CString strKB0605InRec1;
	strKB0605InRec1 = strKB0605Info;
	return strKB0605InRec1;
}

// �ؿ�����
// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0606(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0606_InRec1 kb0606InRec1;

	// �ʱ�ȭ
	memset(&kb0606InRec1, 0x20, sizeof(kb0606InRec1));

	// �����ڵ�
	memset(kb0606InRec1.Shcode, 0x20, sizeof(kb0606InRec1.Shcode));
	if (lpSelChart)
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
	// ��ȸ����
	if(chTypeRequested == MIN_DATA_CHART || chTypeRequested == SEC_DATA_CHART || chTypeRequested == TICK_DATA_CHART)
	{
//		20121211 �̻�� : ���ѷ����� ������ �ʵ��� ����.
//		CInputBarDlg::OnTimer() �� if (nIDEvent == ID_SHARE_CODE_WAIT) ���� ����.

//		((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_pInputBarDlg->SetChkType();// KHD : �ؿܴ� ��ƽ�� ���� ���Ѵ�. ��Ƽ��Ʈ���� ���庯��� ��Ÿ���� ���� ������ �Ϸ� ��ȯ.
		return "";
	}
	{
		if(lpSelChart)
			memcpy(kb0606InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
		else
			memcpy(kb0606InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	}


	memcpy(kb0606InRec1.button,&chTypeRequested,sizeof(kb0606InRec1.button));

	// ��ȸ���
	char chFlag;
	if (bNext)		chFlag = '1';
	else			chFlag = '0';

	memcpy(kb0606InRec1.nflag,&chFlag,sizeof(kb0606InRec1.nflag));

	// �˻�Ű
	memset(kb0606InRec1.nkey,0x20,sizeof(kb0606InRec1.nkey));
	if (chFlag == '1')//���� ��ȸ�� ���
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(kb0606InRec1.nkey, strKey, min(sizeof(kb0606InRec1.nkey), strKey.GetLength()));
	}

	// ������ȸ
	char chNowData = '0';
	if (chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART)
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if (strQueryOpt == "1")			chNowData = '1';
		else if (strQueryOpt == "2")	chNowData = '2';
	}
	memcpy(kb0606InRec1.nowData, &chNowData, sizeof(kb0606InRec1.nowData));

	if (m_pChartItem->GetScreenNo() == SCREENID_FOREIGN)
		memcpy(kb0606InRec1.timeGB, "0", sizeof(kb0606InRec1.timeGB));
	else
		memcpy(kb0606InRec1.timeGB, "1", sizeof(kb0606InRec1.timeGB));

	CString strKB0606Info;
	char* szKB0606Info = strKB0606Info.GetBuffer(sizeof(kb0606InRec1));
	memcpy(szKB0606Info, (char*)&kb0606InRec1, sizeof(kb0606InRec1));
	strKB0606Info.ReleaseBuffer();
	CString strKB0606InRec1;
	strKB0606InRec1 = strKB0606Info;
	return strKB0606InRec1;
}

CString CChartItemData::GetQueryData_KB_p0515(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0515_InRec InRec;
	memset(&InRec, 0x20, sizeof(InRec));

	CString strData;
	//	�����ڵ�
	if (lpSelChart)		strData = lpSelChart->m_strItemCode;
	else				strData = m_strCodeForRequest;
	memcpy(InRec.code, (LPCTSTR)strData, min(sizeof(InRec.code), strData.GetLength()));

	//	�˻�������
	strData = m_pChartItem->GetEndDateRequested();
	memcpy(InRec.date, (LPCTSTR)strData, min(sizeof(InRec.date), strData.GetLength()));

	//	��ȸ�Ǽ�
	strData.Format(_T("%04d"), GetCntForQuery(bNext));
	memcpy(InRec.maxbong, (LPCTSTR)strData, min(sizeof(InRec.maxbong), strData.GetLength()));

	// ��ȸ����
	if (lpSelChart)		strData = lpSelChart->m_strTickMinDayNumber;
	else				strData = m_pChartItem->GetTickMinDayNumberRequested();
	memcpy(InRec.unit, (LPCTSTR)strData, min(sizeof(InRec.unit), strData.GetLength()));

	// ��ȸ����
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if (chTypeRequested == SEC_DATA_CHART)
	{
		m_pChartItem->SetChartRequestType(DAY_DATA_CHART);
		chTypeRequested = DAY_DATA_CHART;
	}
	memcpy(InRec.btngb, &chTypeRequested, sizeof(InRec.btngb));

	// ������ȸ����
	char chFlag;
	if (bNext)		chFlag = '1';
	else			chFlag = '0';
	memcpy(InRec.nextgb, &chFlag, sizeof(InRec.nextgb));

	// ������ȸŰ
	if (chFlag == '1')
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(InRec.nextkey, strKey, min(sizeof(InRec.nextkey), strKey.GetLength()));
	}

	// ������ȸ
/*	char chNowData = '0';
	if (chTypeRequested != DAY_DATA_CHART	&&
		chTypeRequested != WEEK_DATA_CHART	&&
		chTypeRequested != MONTH_DATA_CHART &&
		chTypeRequested != YEAR_DATA_CHART)
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if (strQueryOpt == "1")			chNowData = '1';
		else if (strQueryOpt == "2")	chNowData = '2';
	}
	memcpy(InRec.nowdata, &chNowData, sizeof(InRec.nowData));
*/
	CString strTemp;
	char* szTemp = strTemp.GetBuffer(sizeof(InRec));
	memcpy(szTemp, (char*)&InRec, sizeof(InRec));
	strTemp.ReleaseBuffer();
	CString strInRec;
	strInRec = strTemp;

	return strInRec;
}

//������ǥ TR�� ������ ���� Input���� ��������.
CString CChartItemData::GetMarketQueryData_ChartItemData(BOOL bNext, UINT nRqstCnt)
{
	m_lDataSize = 0;
	m_lCnt = 0;
	if(m_strTRCode == QUERY_STRMARKET)
		return GetQueryDataMarket(bNext, nRqstCnt);//return GetQueryDataMarket(bNext);		

	return "";
}

BOOL CChartItemData::MakeChartData(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	BOOL bRet = FALSE;
	if(m_strTRCode == QUERY_STRJONGMOK)
		bRet = MakeChartData_KB_p0602(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRUPJONG)
		bRet = MakeChartData_KB_p0603(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRFUTOPT)
		bRet = MakeChartData_KB_p0604(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRSTOCKFUTOPT)
		bRet = MakeChartData_KB_d1013(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRFOREIGN)
		bRet = MakeChartData_KB_p0606(szData, nLength, lpSelChart);
	else if(m_strTRCode == QUERY_STRMARKET)
		bRet = MakeChartMarektData(szData, nLength);
	else if(m_strTRCode == QUERY_STRFOJIPYO)
		bRet = MakeChartData_KB_p0515(szData, nLength, lpSelChart);
/*
	if (g_pDebugMng)
	{
		CString strDebug;
		strDebug.Format(_T("MakeChartData() : start Date[%ld], end Date[%ld]\n"), m_lStartDate, m_lEndDate);
		g_pDebugMng->TraceTran('O', "CChartItemData::MakeChartData", strDebug.GetLength(), (LPBYTE)(LPCTSTR)strDebug);
	}
*/
	return bRet;
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
// 	sKey.Format("%*.*s", sizeof(pKB0602_OutRec1->nkey), sizeof(pKB0602_OutRec1->nkey), pKB0602_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
// 
// 	if( atoi(sKey)  == 0)	memset(m_chkey,0x00,sizeof(m_chkey));
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
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
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
		m_pKB0602_ChartData = (KB_p0602_CHART*)new char[nLength];
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

	m_pChartItem->EnableRequestMoreButton(TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

//@Solomon-->
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
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
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
		m_pKB0603_ChartData = (KB_p0603_CHART*)new char[nLength];
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

	m_pChartItem->EnableRequestMoreButton(TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

//@Solomon-->
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
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	{ //100217-alzioyes.
		char *pData = (char*)m_pKB0604_ChartData;
		delete [] pData;
		m_pKB0604_ChartData = NULL;
	}

	if(!m_pKB0604_ChartData)
	{
		m_pKB0604_ChartData = (KB_p0604_CHART*)new char[nLength];
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

	if(m_pChartItem->GetScreenNo() == nScreenID_6401 && m_pChartItem->GetPrevDateRecvFlag())
	{
		CString strDate = m_pChartItem->GetEndDate();
		for(long l = 0; l < lCnt; l++)
			::memcpy(m_pKB0604_ChartData->chart[l].date, strDate.Right(4), 4);
	}
	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_d1013(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_d1013_OutRec1_EX* pKB0605_OutRec1 = (KB_d1013_OutRec1_EX*)szData;
	if(pKB0605_OutRec1 == NULL) return FALSE;

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0605_OutRec1->nkey, sizeof(pKB0605_OutRec1->nkey));

//	m_pChartItem->EnableRequestMoreButton(TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

//@Solomon-->
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB0605_OutRec1->nkey);
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
	sKey.Format("%*.*s", sizeof(pKB0605_OutRec1->nkey), sizeof(pKB0605_OutRec1->nkey), pKB0605_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0605_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
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
		m_pKB1013_ChartData = (KB_d1013_CHART*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0605_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp, pKB0605_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pKB0605_OutRec1->fcnt[0] - &pKB0605_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB0605_OutRec1->fcnt[0] - &pKB0605_OutRec1->name[0]);
	memcpy(m_pKB1013_ChartData, &pKB0605_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB1013_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB1013_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);
	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_p0606(void* szData, long nLength, ST_SELCHART* lpSelChart) //�ؿ�����
{
	KB_p0606_OutRec1_EX* pKB0606_OutRec1 = (KB_p0606_OutRec1_EX*)szData;
	if(pKB0606_OutRec1 == NULL) return FALSE;

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0606_OutRec1->nkey, sizeof(pKB0606_OutRec1->nkey));

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0606_OutRec1->nkey), sizeof(pKB0606_OutRec1->nkey), pKB0606_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0606_OutRec1->nkey));
	if (sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
		m_pChartItem->EnableRequestMoreButton(FALSE);

	if (lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if (!m_pKB0606_ChartData)
		m_pKB0606_ChartData = (KB_p0606_CHART*)new char[nLength];

	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0606_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_pInputBarDlg->m_strJonMokName = m_strName;//IBK �����ڵ忡�� ������� ���� �ʾƼ� AP�� �� 
	MEMCPYWITHNULL(szTemp, pKB0606_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if (nLength < (&pKB0606_OutRec1->fcnt[0] - &pKB0606_OutRec1->name[0]))
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

BOOL CChartItemData::MakeChartData_KB_p0515(void* szData, long nLength, ST_SELCHART* lpSelChart) //�ؿ�����
{
	if ((nLength <= 0) || (szData == NULL))
		return FALSE;

	KB_p0515_OutRec* pOutRec = (KB_p0515_OutRec*)szData;

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pOutRec->nkey, sizeof(pOutRec->nkey));

	CString sKey;
	sKey.Format("%*.*s", sizeof(pOutRec->nkey), sizeof(pOutRec->nkey), pOutRec->nkey);
	sKey.TrimLeft(); sKey.TrimRight();

	ZeroMemory(m_chkey, sizeof(m_chkey));
	if (sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
		m_pChartItem->EnableRequestMoreButton(FALSE);

	if (lpSelChart)
		lpSelChart->m_strKey = m_chkey;
	
	if (!m_pKB0515_ChartData)
		m_pKB0515_ChartData = (KB_p0515_CHART*)new char[nLength];

	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pOutRec->name);
	m_strName = szTemp;
	m_strName.TrimRight();

	MEMCPYWITHNULL(szTemp, pOutRec->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;

	long lPosForChart = 0;
	if (nLength < (&pOutRec->fcnt[0] - &pOutRec->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pOutRec->fcnt[0] - &pOutRec->name[0]);
	memcpy(m_pKB0515_ChartData, &pOutRec->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB0515_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB0515_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}

CString CChartItemData::GetTrCode()
{
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
	MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, lpszRQ);

	if(lpSelChart->m_strChartGubun == QUERY_STRJONGMOK)
	{
		CString strPackType = "";
		if (lpSelChart->m_nChartNumericValueType == PERCENT_TYPE)
			strPackType = g_strPacketTypeNumber[7];
		else if(lpSelChart->m_nChartNumericValueType == WON_TYPE)
			strPackType = g_strPacketTypeNumber[0];
		else if(lpSelChart->m_nChartNumericValueType == DOLLAR_TYPE)
			strPackType = g_strPacketTypeNumber[6];

		if (strPackType.GetLength() > 0)
		{
			MakePacketOption(g_strPacketName[1],10,strPackType,FALSE, lpszRQ);
			MakePacketOption(g_strPacketName[2],10,strPackType,FALSE, lpszRQ);
			MakePacketOption(g_strPacketName[3],10,strPackType,FALSE, lpszRQ);
			MakePacketOption(g_strPacketName[4],10,strPackType,TRUE, lpszRQ);
			MakePacketOption(g_strPacketName[5],10,strPackType,TRUE, lpszRQ);
			MakePacketOption(g_strPacketName[6],18,strPackType,FALSE, lpszRQ);
		}
		MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, lpszRQ);			//������
		MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE, lpszRQ);		//������ Sign
		MakePacketOption("�ð�GuideLine",1,g_strPacketTypeEtc[1],FALSE, lpszRQ);
		MakePacketOption("��GuideLine",1,g_strPacketTypeEtc[1],FALSE, lpszRQ);
		MakePacketOption("����GuideLine",1,g_strPacketTypeEtc[1],FALSE, lpszRQ);
	}
	else
	{
		MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, lpszRQ);
		MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, lpszRQ);
		MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, lpszRQ);
		MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, lpszRQ);
		MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, lpszRQ);

		if (m_strTRCode != QUERY_STRFOREIGN)	// QUERY_STRUPJONG, QUERY_STRFUTOPT, QUERY_STRSTOCKFUTOPT, QUERY_STRFOJIPYO
		{
			MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE, lpszRQ);
			MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, lpszRQ);			//������
			if (m_strTRCode == QUERY_STRFUTOPT || m_strTRCode == QUERY_STRSTOCKFUTOPT)
				MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE, lpszRQ);	//KHD : �̰��� ����
		}
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
		BOOL bPrice = FALSE, bPriceMA = FALSE, bVolume = FALSE, bVolumeMA = FALSE;

		m_strArrayPacketName.RemoveAll();
		BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
		if(m_pChartItem->GetPtrST())// it depends on System Trading Spec. gm0604 2005/10/04
		{	
			ASSERT(IsPrimaryChartItemData);
			if(m_strTRCode == QUERY_STRJONGMOK)
			{
				MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
				MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE);		//������ Sign
			}
			else if(m_strTRCode == QUERY_STRFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
			{
				MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
				MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE);
				MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE);		//KHD : �̰������� 
			}

			AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
			
			bPriceMA = bVolume = bVolumeMA = TRUE;
		}
		else
		{
			if(IsPrimaryChartItemData)
			{			
				if(m_strTRCode == QUERY_STRJONGMOK)
				{
					MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
					
					int nNumber = -1;
					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)		nNumber = 7;
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)		nNumber = 0;
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)	nNumber = 6;

					if (nNumber != -1)
					{
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[nNumber],TRUE);
					}

					MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE);
					MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE);		//������ Sign

					MakePacketOption("�ð�GuideLine",1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption("��GuideLine",1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption("����GuideLine",1,g_strPacketTypeEtc[1],FALSE);

					if(!IsNoVolumeGraph)
					{
						bPriceMA = bVolume = bVolumeMA = TRUE;
						AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
					}
					else
						bPrice = bPriceMA = TRUE;
				}
				else
				{
					if( m_strTRCode == QUERY_STRSTOCKFUTOPT )
					{
						MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
						MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					}
					else
					{
						MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE);
						MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					}

					if (m_strTRCode != QUERY_STRFOREIGN)
					{
						MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);
					}

					if (m_strTRCode == QUERY_STRFUTOPT || m_strTRCode == QUERY_STRSTOCKFUTOPT)
						MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE);

					AddGraphPriceChart();	// JSR 20060123 Add : For Making a Price Chart

					if (m_strTRCode != QUERY_STRFOJIPYO)
					{
						if (m_strTRCode != QUERY_STRFUTOPT || m_pChartItem->GetScreenNo() != nScreenID_6401)
							bPriceMA = TRUE;

						bVolume = bVolumeMA = TRUE;
					}
				}
			}
			else
			{			
				if(m_strTRCode == QUERY_STRUPJONG)
				{
					MakePacketOption(m_strName + g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
					MakePacketOption(m_strName + g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName + g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName + g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName,						10,g_strPacketTypeNumber[4],TRUE);
					MakePacketOption(m_strName + g_strPacketName[5],10,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName + g_strPacketName[6],18,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName + g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);

					CStringArray arrayPacket;
					arrayPacket.Add(m_strName);
					m_strArrayGraphTitle.Add(m_strName);
					CString strGraphOption = MakeGraphOption(g_strChartType[1],m_strName,arrayPacket,m_strName);
					m_pChartItem->AddGraph(1,0,0,strGraphOption,TRUE,TRUE,TRUE);
				}
				else if(m_strTRCode == QUERY_STRMARKET)
				{
					m_strArrayPacketName.Add(m_strCodeForRequest); //��Ŷ �� �߰� 
					return TRUE;
				}
			}
		}
		m_bInitalChart = TRUE;

		if (bPrice)
		{
			m_strArrayGraphTitle.Add(g_strIndexName[0]);
			m_pChartItem->AddGraph(0,0,0,"������Ʈ",FALSE,TRUE,TRUE);
		}

		if (bPriceMA)
		{
			m_strArrayGraphTitle.Add("���� �̵����");
			m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
		}

		if (bVolume)
		{
			m_strArrayGraphTitle.Add(g_strIndexName[1]);
			m_pChartItem->AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);
		}

		if (bVolumeMA)
		{
			m_strArrayGraphTitle.Add("�ŷ��� �̵����");
			m_pChartItem->AddGraph(1,0,0,"�ŷ��� �̵����",FALSE,FALSE,FALSE);
		}

		if (bPrice)
			m_pChartItem->HideScale(FALSE);

		if (m_pChartItem->GetPtrST())
			m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
		else if (IsPrimaryChartItemData)
		{
			if (!m_bResetChartCfgWith)
			{
				m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
				m_bResetChartCfgWith = TRUE;
			}
		}

#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX�� AddPacket/Graph::End"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
	}
	return TRUE;
}

// ���ο� ��Ŷ,��Ʈ �߰� : �������� - ojtaso (20061224)
BOOL CChartItemData::AddPacketAndGraph2(BOOL bAddGraph /*= TRUE*/, BOOL bOverlapp /*= FALSE*/)
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

	int nBlockCount = 0;
	if(bAddGraph == TRUE && bOverlapp == FALSE && !((CStdDialog*)(m_pChartItem->m_pStdDialog))->IsAvailableBlockCount(TRUE, nBlockCount))
		bAddGraph = FALSE;

//	if(!m_bInitalChart)
	{
#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX�� AddPacket/Graph::Begin"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
		m_strArrayPacketName.RemoveAll();
		BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
		if(m_pChartItem->GetPtrST())// it depends on System Trading Spec. gm0604 2005/10/04
		{	
			ASSERT(IsPrimaryChartItemData);
			if(m_strTRCode == QUERY_STRJONGMOK)
			{
				MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
				MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE);		//������ Sign
			}
			else if(m_strTRCode == QUERY_STRFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
			{
				MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
				MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE);
				MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE);		//KHD : �̰������� 
			}

			AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart

			m_strArrayGraphTitle.Add("���� �̵����");
			m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);


			m_strArrayGraphTitle.Add(g_strIndexName[1]);
			m_pChartItem->AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);

			m_strArrayGraphTitle.Add("�ŷ��� �̵����");
			m_pChartItem->AddGraph(1,0,0,"�ŷ��� �̵����",FALSE,FALSE,FALSE);
		}
		else
		{	
			if(IsPrimaryChartItemData)
			{
				if(m_strTRCode == QUERY_STRJONGMOK)
				{
					MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);

					int nNumber = -1;
					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)		nNumber = 7;
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)		nNumber = 0;
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)	nNumber = 6;

					if (nNumber != -1)
					{
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[nNumber],TRUE);
					}

					MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE);
					MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE);		//������ Sign

					MakePacketOption("�ð�GuideLine",1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption("��GuideLine",1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption("����GuideLine",1,g_strPacketTypeEtc[1],FALSE);
				}
				else
				{
					if( m_strTRCode == QUERY_STRSTOCKFUTOPT )
					{
						MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
						MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					}
					else
					{
						MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE);
						MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					}

					if (m_strTRCode != QUERY_STRFOREIGN)
					{
						MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);
					}

					if (m_strTRCode == QUERY_STRFUTOPT || m_strTRCode == QUERY_STRSTOCKFUTOPT)
						MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE);
				}

				// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
				int nRow, nCol;
				if(bOverlapp == FALSE)
					AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);
				else
					AddGraphPriceChart_Overlapp(nRow, nCol);

				if(!m_bResetChartCfgWith)
				{
					m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					m_bResetChartCfgWith = TRUE;
				}
			}
			else
			{
				if(m_strTRCode == QUERY_STRUPJONG)
				{
					MakePacketOption(m_strName + g_strPacketName[0],  8, g_strPacketTypeDate[0], TRUE);
					MakePacketOption(m_strName + g_strPacketName[1], 10, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName + g_strPacketName[2], 10, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName + g_strPacketName[3], 10, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName,						 10, g_strPacketTypeNumber[4], TRUE);
					MakePacketOption(m_strName + g_strPacketName[5], 10, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName + g_strPacketName[6], 18, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName + g_strPacketName[7],  1, g_strPacketTypeEtc[1], FALSE);

					CStringArray arrayPacket;
					arrayPacket.Add(m_strName);
					m_strArrayGraphTitle.Add(m_strName);
					CString strGraphOption = MakeGraphOption(g_strChartType[1],m_strName,arrayPacket,m_strName);
					m_pChartItem->AddGraph(1,0,0,strGraphOption,TRUE,TRUE,TRUE);
				}
				else if(m_strTRCode == QUERY_STRMARKET)
				{
					m_strArrayPacketName.Add(m_strCodeForRequest); //��Ŷ �� �߰� 
					return TRUE;
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


	//MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
	//MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
	//MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
	//MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
	//MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
	//MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);

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
		if(m_pChartItem->GetScreenNo() == nScreenID_6401)
			m_pChartItem->AddGraph(0,0,0,"��������Ʈ",FALSE,FALSE,FALSE);
		else
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

		m_strArrayGraphTitle.Add("���� �̵����");
		m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);
	}

}

// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
void CChartItemData::AddGraphPriceChart_Overlapp(int& nRow, int& nCol)
{
	int nVertScaleGroup = m_pChartItem->GetVertScaleGroupCount(0, 0);
	if(nVertScaleGroup < 0)
		return;

	m_strArrayGraphTitle.Add(g_strIndexName[0]);

	nRow = nCol = 0;

	if(m_pChartItem->GetScreenNo() == nScreenID_6401)
	{
		m_pChartItem->AddGraph(nRow,nCol,0,"��������Ʈ",FALSE,FALSE,TRUE);
	}
	else
	{
		m_pChartItem->AddGraph(nRow,nCol,nVertScaleGroup,"������Ʈ",FALSE,TRUE,TRUE);
		
		m_strArrayGraphTitle.Add("���� �̵����");
		m_pChartItem->AddGraph(nRow,nCol,nVertScaleGroup,"���� �̵����",FALSE,FALSE,FALSE);
	}
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

BOOL CChartItemData::MakeChartMarektData(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	CString strPacketCnt;

//@��������
	KB_d1015_OutRec1* pOutRec = (KB_d1015_OutRec1*)szData;
//@��������
	strPacketCnt.Format("%1.1s", pOutRec->inNumGb);	

	int nMarketJipyCnt = atoi(strPacketCnt);
	//AfxMessageBox(strPacketCnt);
	
	if(nMarketJipyCnt == 2) //��ǥ���� 2��
	{
		KB_d1015_OutRec_1* pOutRec_1 = (KB_d1015_OutRec_1*)szData;

		if(nLength < (&pOutRec_1->d1015_CHART.chtCnt[0] - &pOutRec_1->winid[0]))
			return FALSE;

		CString sOriCode, sRecvCode;
		sRecvCode.Format("%8.8s", pOutRec_1->shcode); sRecvCode.TrimLeft(); sRecvCode.TrimRight();
		sOriCode = m_pChartItem->GetSelectedItemCode();
		CString strJipyo = pOutRec->dataNm;
		if (strJipyo.Find("ADR") == -1 && strJipyo.Find("ADL") == -1 )	// �������� �����϶� �����ڵ尡 �ڽ����϶� 001 �δ� 00������ 1�� �־ �̷� �ڵ带 �ۼ���.
		{
			if(sOriCode.CompareNoCase(sRecvCode) != 0)
			{		
				CString strTR;
				strTR = QUERY_STRMARKET;

				m_pChartItem->RemoveChartItem_OneData(m_strCodeForRequest, strTR);
				return FALSE;
			}
		}
		memset(m_chkey,0x00,sizeof(m_chkey));
		memcpy(m_chkey, pOutRec_1->nxtKey, sizeof(pOutRec_1->nxtKey));

		if(lpSelChart)
			lpSelChart->m_strKey = m_chkey;

		if(!m_pKB1015_ChartData)
		{
			//m_pKB1015_ChartData = (char*)(new KB_p0607_CHART_1);
			m_pKB1015_ChartData = new char[nLength];
			//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_1));
			memset(m_pKB1015_ChartData, 0x00, nLength);
		}
		else
		{
			//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_1));
			char* pData = m_pKB1015_ChartData;
			//delete m_pKB1015_ChartData;
			delete [] pData;
			m_pKB1015_ChartData = NULL;
		}
		m_strCode = m_strCodeForRequest;	
		m_strName = m_strCode;
		char szTemp[64];
		long lSize = 0;

		MEMCPYWITHNULL(szTemp, pOutRec_1->d1015_CHART.chtCnt);
		lSize = atol(szTemp);

		if(lSize <= 0) 
		{
			if( m_pKB1015_ChartData )
			{
				//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_1));
				char* pData = m_pKB1015_ChartData;
				//delete m_pKB1015_ChartData;
				delete [] pData;
				m_pKB1015_ChartData = NULL;
			}
//			AfxMessageBox("Err 2");
			return FALSE;
		}


		MEMCPYWITHNULL(szTemp, pOutRec_1->d1015_CHART.chart[0].date);
		m_lStartDate = atol(szTemp);
		m_lCntTemp = lSize;

		long lPosForChart = 0;
		lPosForChart = nLength - (&pOutRec_1->d1015_CHART.chtCnt[0] - &pOutRec_1->winid[0]);		
		memcpy(m_pKB1015_ChartData, &pOutRec_1->d1015_CHART.chtCnt[0], lPosForChart);
		
		m_lDataSize = lPosForChart;

		MEMCPYWITHNULL(szTemp, pOutRec_1->d1015_CHART.chart[lSize-1].date);
		m_lEndDate = atol(szTemp);	

		
//			CString strData;			
//			strData.Format("Query__%.*s", 100, m_pSt10450Data);
//			OutputDebugString(strData);
			
//			AfxMessageBox(strData);

		long lSizeOfChtPacket = SZ_TR_MARKET_1_OS;
	
	
/*		char szTempBig[512];
		int lPos = 0;
		int lPos2 = 0;
		for(lPos = 0;lPos<lSize;lPos++)
		{
			memset(szTempBig,0x00,sizeof(szTempBig));
			memcpy(szTempBig,&stOutput.os_buf[lPos],lSizeOfChtPacket);
			for(lPos2 = 8;lPos2<lSizeOfChtPacket;lPos2++) //lPos2 = 8 �� ������ ��¥ �κ��� �ǳʶٱ� ���� 
			{
				if(szTempBig[lPos2]>' ')
				{
					MEMCPYWITHNULL(szTemp,stOutput.os_buf[lPos].date);
					m_lStartDate = atol(szTemp);
					m_lCntTemp = lSize-lPos;
					return TRUE;
				}
			}		
		}
*/
		return TRUE;
	}
	else if(nMarketJipyCnt == 4)//��ǥ���� 
	{
		KB_d1015_OutRec_3* pOutRec_3 = (KB_d1015_OutRec_3*)szData;

		if(nLength < (&pOutRec_3->d1015_CHART.chtCnt[0] - &pOutRec_3->winid[0]))
			return FALSE;

		memset(m_chkey, 0x00, sizeof(m_chkey));
		memcpy(m_chkey, pOutRec_3->nxtKey, sizeof(pOutRec_3->nxtKey));

		if(lpSelChart)
			lpSelChart->m_strKey = m_chkey;

		if(!m_pKB1015_ChartData)
		{
			//m_pKB1015_ChartData = (char*)(new KB_p0607_CHART_3);
			//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_3));
			m_pKB1015_ChartData = new char[nLength];
			memset(m_pKB1015_ChartData, 0x00, nLength);
		}
		else
		{
			//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_3));
			//delete m_pKB1015_ChartData;
			char* pData = m_pKB1015_ChartData;
			//delete m_pKB1015_ChartData;
			delete [] pData;
			m_pKB1015_ChartData = NULL;
		}

		m_strCode = m_strCodeForRequest;	
		m_strName = m_strCode;
		char szTemp[64];
		long lSize = 0;

		MEMCPYWITHNULL(szTemp, pOutRec_3->d1015_CHART.chtCnt);
		lSize = atol(szTemp);

		if(lSize <= 0) 
		{
			if( m_pKB1015_ChartData )
			{
				//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_3));
				//delete m_pKB1015_ChartData;
				char* pData = m_pKB1015_ChartData;
				//delete m_pKB1015_ChartData;
				delete [] pData;
				m_pKB1015_ChartData = NULL;
			}
//			AfxMessageBox("Err 4");
			return FALSE;
		}


		MEMCPYWITHNULL(szTemp, pOutRec_3->d1015_CHART.chart[0].date);
		m_lStartDate = atol(szTemp);
		m_lCntTemp = lSize;

		long lPosForChart = 0;
		lPosForChart = nLength - (&pOutRec_3->d1015_CHART.chtCnt[0] - &pOutRec_3->nxtKey[0]);
		memcpy(m_pKB1015_ChartData, &pOutRec_3->d1015_CHART.chtCnt[0], lPosForChart);

		m_lDataSize = lPosForChart;

		MEMCPYWITHNULL(szTemp, pOutRec_3->d1015_CHART.chart[lSize-1].date);
		m_lEndDate = atol(szTemp);	

		
		long lSizeOfChtPacket = SZ_TR_MARKET_3_OS;
	
	
	/*	char szTempBig[512];
		int lPos = 0;
		int lPos2 = 0;
		for(lPos = 0;lPos<lSize;lPos++)
		{
			memset(szTempBig,0x00,sizeof(szTempBig));
			memcpy(szTempBig,&stOutput.os_buf[lPos],lSizeOfChtPacket);
			for(lPos2 = 8;lPos2<lSizeOfChtPacket;lPos2++) //lPos2 = 8 �� ������ ��¥ �κ��� �ǳʶٱ� ���� 
			{
				if(szTempBig[lPos2]>' ')
				{
					MEMCPYWITHNULL(szTemp,stOutput.os_buf[lPos].date);
					m_lStartDate = atol(szTemp);
					m_lCntTemp = lSize-lPos;
					return TRUE;
				}
			}		
		}
	*/
		return TRUE;
	}
	
//	AfxMessageBox("Err 5");
	return FALSE;
}

CString CChartItemData::GetQueryDataMarket(BOOL bNext, int nRqstCnt)
{
	KB_d1015_InRec1 InRec1;

	char Temp[64];
	char chFlag;

	// �ʱ�ȭ
	memset(&InRec1, 0x20, sizeof(InRec1));
	
	// ��������̵�
	CString sWndID;
	sWndID.Format("%d", m_pChartItem->GetDlgCtrlID());
	memcpy(InRec1.winid, sWndID, sWndID.GetLength());

	// ����Ÿ��ȣ
	memcpy(InRec1.dataNm, (LPTSTR)(LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
	
	// ������(0:��/1:�Ǻ�)
	InRec1.img[0] = '1';//���(0) �Ǻ�(1);
	if(m_pChartItem->m_CfgInfo.bRealCandle == false) //����̶��..
		InRec1.img[0] = '0';

	CString strPrimCode = m_pChartItem->GetSelectedItemCode();
	if(strPrimCode.GetLength() == UPJONG_LENGTH)// ����
	{
		// Ÿ��(0:����/1:����)
		InRec1.type [0]= '1';

		// �����ڵ�
		memcpy(InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(InRec1.shcode));
		InRec1.shcode[3] = ' ';	
		InRec1.shcode[4] = ' ';	
		InRec1.shcode[5] = ' ';	
		InRec1.shcode[6] = ' ';	
		InRec1.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == CODE_STOCK_LENGTH || strPrimCode.GetLength() == (CODE_STOCK_LENGTH-1))							// ����
	{
		// Ÿ��(0:����/1:����)
		InRec1.type[0] = '0';

		// �����ڵ�
		memcpy(InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(InRec1.shcode));
		InRec1.shcode[6] = ' ';
		InRec1.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == FUTOPT_LENGTH)// || strPrimCode.GetLength() == COMMODITY_LENGTH)							// ����
	{
		// Ÿ��(0:����/1:����)
		InRec1.type[0] = '0';

		// �����ڵ�
		memcpy(InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(InRec1.shcode));
	}

	// �˻�����(�ֱ���)
	memcpy(InRec1.edate,(LPCTSTR)m_pChartItem->GetEndDateRequested(),sizeof(InRec1.edate));
	if(nRqstCnt >= 10000)
	{
		AfxMessageBox("������ǥ �ִ� ��ȸ ������ 10000�� �����Դϴ�");
		nRqstCnt = 9999;
	}

	// �˻��Ǽ�
	m_lCntRequired = nRqstCnt; //��ȸ�� ������ ���� ���� ������ ���ϱ� ���� �����صд�.
	sprintf(Temp,"%04d",nRqstCnt);
	
	if(atoi(Temp) <= 0) return _T("");
	memcpy(InRec1.num,Temp,sizeof(InRec1.num));

	// ����(0:��ȸ/1:����)
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
	memcpy(InRec1.nxtGb,&chFlag,sizeof(InRec1.nxtGb));

	// ��ȸŰ(����/����)
	CString sKey;
	sKey.Format("%*.*s", sizeof(m_chkey), sizeof(m_chkey), m_chkey);
	sKey.TrimLeft(); sKey.TrimRight();

	if(sKey.GetLength()>0)
		memcpy(InRec1.nxtKey, m_chkey, sizeof(InRec1.nxtKey));
	else
		memset(InRec1.nxtKey, 0x20, sizeof(InRec1.nxtKey));

	//>>0000550: 0503_����������Ʈ�� ������ǥ�� �̰������� ��ǥ�� ���ֱ� �̿��� ��� �ֱ⿡���� ��ȸ�ǵ��� ������û
	// ��ȸ����
//	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	char chTypeRequested = m_pChartItem->GetTypeRequested();
//	if(lpSelChart)
//		memcpy(InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
//	else
		memcpy(InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	
	// ��ȸ����
	memcpy(InRec1.button,&chTypeRequested,sizeof(InRec1.button));
	//<<0000550

	CString strTrInfo;
	char* pszTrInfo = strTrInfo.GetBuffer(sizeof(InRec1));
	memcpy(pszTrInfo, (char*)&InRec1, sizeof(InRec1));
	strTrInfo.ReleaseBuffer();
	CString strOutput;
	strOutput = strTrInfo;
	return strOutput;
}

long CChartItemData::GetMaximumRequestCnt()
{
	if( (m_strTRCode == QUERY_STRJONGMOK) ||
		(m_strTRCode == QUERY_STRUPJONG ) ||
		(m_strTRCode == QUERY_STRMARKET ) || 
		(m_strTRCode == QUERY_STRFUTOPT ) ||
		(m_strTRCode == QUERY_STRSTOCKFUTOPT ) ||
		(m_strTRCode == QUERY_STRFOREIGN) ||
		(m_strTRCode == QUERY_STRFOJIPYO)
	)
	{
		return (long)DEFAULTCNTATONCE;
	}
	
	return 0;
		
}

long CChartItemData::GetCntForQuery(BOOL bNext)
{
	//-->@Solomon:100311-Next��ȸ�̴��� ���� ��û�� ������ū ��û�ϵ��� ����.
	bNext = FALSE;
	//<--

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
//	if(m_lCntRequired<lCntReceived)
//	{
//		if (m_strCodeForRequest.Find("�������ż�") < 0 && m_strCodeForRequest.Find("�ݿɼǼ��ż�") < 0 && m_strCodeForRequest.Find("ǲ�ɼǼ��ż�") < 0 && m_strCodeForRequest.Find("������ż�����") < 0)
//		{
//	#ifdef _DEBUG
//			CString strMsg;
//			strMsg.Format("������ �����ͼ��� ���� �ʽ��ϴ�...[TR:%s][Code:%s]\
//						  �䱸�� �����ͼ�:%d, ���� �����ͼ�:%d",\
//						  m_strTRCode,m_strCode,m_lCntRequired,lCntReceived);
//			AfxMessageBox(strMsg);
//	#endif
//			return FALSE;
//		}
//	}

	if (!m_pChartItem->IsPrimaryChartItemData(this))	// �� �����Ͱ� �ƴ� ���
	{
		if (m_lCntRequiredByPrim >= lCntReceived)
			m_lCntRequiredByPrim -= lCntReceived;

////gm0604 2004.8.2
		if (!m_lCntRequiredByPrim)
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

	if (!lCntReceived)
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

CString CChartItemData::MakePacketOption(LPCTSTR lpPacketName, long lPacketSize, LPCTSTR lpPacketType, BOOL bUseInReal, CString strRQ)
{
	CString strPacket, strTemp;

	if (bUseInReal)		strTemp = "��;\r\n";
	else				strTemp = "��;\r\n";

	//2005. 05. 18 add by sy, nam =============================================================
	//��ø ����ΰ�� ��Ŷ ������ �ٸ��� �ϱ� ���� �ٸ� �Լ��� ȣ���Ѵ�.	
	if (m_bIsFilled && m_bOverLapping)
		strPacket.Format(_T("1;ov%d%s;;ov%d%d;ov%d%s;ov%d%s"), m_nCntOverLap, lpPacketName, m_nCntOverLap, lPacketSize,
																m_nCntOverLap, lpPacketType, m_nCntOverLap, strTemp);
	//=========================================================================================
	else
		strPacket.Format(_T("1;%s;;%d;%s;%s"), lpPacketName, lPacketSize, lpPacketType, strTemp);

	if (strRQ.GetLength() <= 0)
		m_pChartItem->AddPacket(strPacket);
	else
		m_pChartItem->AddPacketRQ(strRQ, strPacket);

	m_strArrayPacketName.Add(lpPacketName);

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
		return (long)m_pKB1015_ChartData;
	else if(m_strTRCode == QUERY_STRFOJIPYO)
		return (long)m_pKB0515_ChartData;

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
				strRealItem = g_strPacketName[5];
				strRealItem += '.';
				strRealItem += "S31.volume.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);		
				m_strArrayRealItemRegistered.Add(strRealItem);

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
				strRealItem = g_strPacketName[5];
				strRealItem += '.';
				strRealItem += "JS0.volume.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);		
				m_strArrayRealItemRegistered.Add(strRealItem);
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
				strRealItem = g_strPacketName[5];
				strRealItem += '.';
				strRealItem += "SC0.volume.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);		
				m_strArrayRealItemRegistered.Add(strRealItem);
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
				strRealItem = g_strPacketName[5];
				strRealItem += '.';
				strRealItem += "OC0.volume.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);		
				m_strArrayRealItemRegistered.Add(strRealItem);
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
	}
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
BOOL CChartItemData::SetLoadedPacketData(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart)
{
//#ifdef __MULTI_LANG__
//	return m_pChartItem->SetPacketDataRQ(lpszRQ, GetPacketName(),GetData(),GetDataSize(),FALSE);
//#else
 	BOOL bSetPacketDataRQ = m_pChartItem->SetPacketDataRQ(lpszRQ, GetPacketName(),GetData(),GetDataSize(),FALSE);
 	RemoveData();
 	return bSetPacketDataRQ;
//#endif
}

BOOL CChartItemData::SetPacketData(BOOL bReset)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartItemData::SetPacketData:Begin\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}
//	if(g_pDebugMng) 
//		g_pDebugMng->Trace("%s TR[%s]", "CChartItemData::SetPacketData(..)", "IN");

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
	//20090922 SJ_KIM ViewCount <*
	m_pChartItem->SetViewCountFlag(TRUE);
	bRetValue =  m_pChartItem->SetPacketData(GetPacketName(),GetData(),GetDataSize(),bReset);
	m_pChartItem->SetViewCountFlag(FALSE);
	//20090922 SJ_KIM ViewCount *>

ExitLoop:
	RemoveData();

//	if(g_pDebugMng) 
//		g_pDebugMng->Trace("%s TR[%s]", "CChartItemData::SetPacketData(..)", "OUT-OK");

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartItemData::SetPacketData:End\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}
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

	//if(g_pDebugMng) g_pDebugMng->TraceTran('O', "ItemData", GetDataSize(), (LPBYTE)GetData());
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

// ������ �ٲ�� PacketName�� �ٲ۴� : �������� - ojtaso (20070711)
void CChartItemData::ChangePacketNameArray(LPCTSTR lpszTRCode)
{
	m_strArrayPacketName.RemoveAll();

	m_strArrayPacketName.Add(g_strPacketName[0]);
	m_strArrayPacketName.Add(g_strPacketName[1]);
	m_strArrayPacketName.Add(g_strPacketName[2]);
	m_strArrayPacketName.Add(g_strPacketName[3]);
	m_strArrayPacketName.Add(g_strPacketName[4]);
	m_strArrayPacketName.Add(g_strPacketName[5]);
	if(::lstrcmp(lpszTRCode, QUERY_STRFOREIGN))
	{
		m_strArrayPacketName.Add(g_strPacketName[6]);
		m_strArrayPacketName.Add(g_strPacketName[7]);
	}//KHD 
	if( ::lstrcmp(lpszTRCode, QUERY_STRFUTOPT))
		m_strArrayPacketName.Add(g_strPacketName[8]);
}

void CChartItemData::OnSelectChartItem(ST_SELCHART* lpSelChart)
{
	SetTRCode(lpSelChart->m_strChartGubun);
	m_strCodeForRequest = lpSelChart->m_strItemCode;
	memcpy(m_chkey, (LPCTSTR)lpSelChart->m_strKey, lpSelChart->m_strKey.GetLength());
}

CString CChartItemData::GetEndDate()
{
	if (m_lEndDate > 0)
	{
		CString strDate;
		strDate.Format(_T("%ld"), m_lEndDate);

		return strDate;
	}

	return _T("");
}
