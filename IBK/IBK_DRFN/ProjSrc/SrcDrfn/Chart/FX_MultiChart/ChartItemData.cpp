// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "ChartItemData.h"
#include "ChartItem.h"
#include "./define/ChartDef.h"
#include "./define/ChartMsgDef.h"
#include "./include/arMarketIndicator.h"
#include "StdDialog.h"

//	�߰�... �ֶ󸮽� ȯ�濡�� ������ ������ �Ѵ�. �� �����̰� �����
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")		// htonl, ntohl

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

	m_pfx112_ChartData = NULL;

	m_lDataSize = 0;
	m_lCnt = 0;
	m_bInitalChart = FALSE;
	m_pChartItem = pChartItem;
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

// --> [Edit]  ������ 2008/07/30
	m_strOpen		= "";			//	�ð�
	m_strHigh		= "";			//	��
	m_strLow		= "";			//	����
	m_strPrice		= "";			//	���簡
	m_strSign		= "";			//	��ȣ
	m_strChange		= "";			//	���
	m_strChrate		= "";			//	�����
	m_strVolume		= "";			//	�ŷ���
	m_strPreprice	= "";			//	���ذ�
	m_strKey		= "";			//	����Ű
	m_strPosition	= "�� 1";
// <-- [Edit]  ������ 2008/07/30
}

// ���õ� ��Ʈ�� ���� ���� : �������� - ojtaso (20070215)
void CChartItemData::SetTRCode (LPCTSTR lpszTRCode)
{
	if(m_strTRCode.Compare(lpszTRCode))
		ChangePacketNameArray(lpszTRCode);

	m_strTRCode = lpszTRCode;
}

/*
	ok
*/
BOOL CChartItemData::ClearChartItemData()
{	
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog ("CChartItemData::ClearChartItemData::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	RemoveRealItem ();
	RemovePacketAndGraph ();
	RemoveData ();

#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog ("CChartItemData::ClearChartItemData::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	return TRUE;
}

/*
	����Ÿ ����.. ok
*/
BOOL CChartItemData::RemoveData()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveData::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	// ����Ÿ ����..
	if (m_pfx112_ChartData)
	{
		char *pData = (char*)m_pfx112_ChartData;
		delete [] pData;
		m_pfx112_ChartData = NULL;
	}

	m_lDataSize = 0;

#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveData::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	return TRUE;
}

void CChartItemData::SetCodeForQuery (LPCTSTR lpCode, BOOL bInitKey/* = TRUE*/)
{
	ASSERT(lpCode);
	m_strCodeForRequest = lpCode;
	if(bInitKey)
		memset(m_chkey,'0',sizeof(m_chkey));
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
stSend2Server* CChartItemData::GetQueryData (BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	m_lDataSize = 0;
	m_lCnt = 0;
	CString strTRCode = lpSelChart ? lpSelChart->m_strChartGubun : m_strTRCode;

	if(strTRCode == QUERY_STRFX)
		return GetQueryData_EU_x56000 (bNext, lpSelChart);

	return NULL;
}

// --> [Edit]  ������ 2008/07/22		( �ð����� )
#define		UD_DATAKIND_TICK	"0"
#define		UD_DATAKIND_MIN		"1"
#define		UD_DATAKIND_DAY		"2"
#define		UD_DATAKIND_WEEK	"3"
#define		UD_DATAKIND_MONTH	"4"
#define		UD_DATAKIND_HOUR	"5"

#include "../../inc/IKSLibMng.h"

/*
	20090902 by kmlee íƮ����Ÿ ��ȸ
*/
stSend2Server* CChartItemData::GetQueryData_EU_x56000 (BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	int nID = 50;

	IGateManager* pGateMng;
	//<<20100308_JS.Kim �ַθ�
	//AfxGetIGateManagerFX(pGateMng);
	AfxGetIGateManager(pGateMng);
	//>>
	if(pGateMng == NULL)
		return NULL;

	//<<20100308_JS.Kim �ַθ�
	/*
	IKSLibManager* pKSLibManager = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
	
	CString	m_strIBNo = pKSLibManager->Lib_KSLink_GetLoginInfo("IB_NO");	// ȸ���� IB_NO(3)
	CString m_strTradeType = "00";											// �ŷ�����(00 : 10��, 01:1��)

	CString m_strDealerGrpID= pKSLibManager->Lib_KSLink_GetLoginInfo("GROUP_CODE");	// �����׷� �ڵ�
	*/
	//>>

	//<<20100308_JS.Kim �ַθ�
	CString strKeyValue = "";
	CString strJMCode = "";
	CString strDate = "";
	CString strTime = "";
	CString strFirstGu = "";
	CString strTimeGu = "";
	CString strSelectMin = "";
	CString strDataCnt = "";
	CString strQtyCd = "1";

	strKeyValue		= "";				/* key ����+�ð�(18)	: key ����+�ð� */
	strJMCode		= "";				/* �����ڵ�(10)			: �����ڵ� */
	strDate			= "99999999";		/* ����(8)				: ���� */
	strTime			= "9999999999";		/* �ð�(10)				: �ð� */
	strFirstGu		= "0";				/* ����(1)				: 0:ó��1:���� */
	strTimeGu		= UD_DATAKIND_DAY;	/* �����ͱ���(1)		: 1:Tick,2:��,3:��,4:��,5:�� */
	strSelectMin	= "1";				/* N�б���(2)			: 0:30sec,1:1M,2:2M,... */
	strDataCnt		= "400";				/* �����ͰǼ�(5)		: �����ͰǼ� */


	SM_fx112_InRec1* input = new SM_fx112_InRec1;
	if (input == NULL)
		return NULL;
	
	memset (input, 0x20, sizeof (SM_fx112_InRec1));									// �ʱ�ȭ

	strKeyValue = lpSelChart ? lpSelChart->m_strKey : m_pChartItem->m_strNextKey;
	if ( bNext )
		memcpy(input->keyvalue,	(LPCTSTR)strKeyValue,	strKeyValue.GetLength());	/* key ����+�ð�	: key ����+�ð� */

	//	�����ڵ�
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	if(lpSelChart)	strJMCode = CStdDialog::GetRealItemCode( lpSelChart->m_strItemCode);
	else			strJMCode = CStdDialog::GetRealItemCode( m_strCodeForRequest);
	memcpy(input->jmcode,	(LPCTSTR)strJMCode,		strJMCode.GetLength());	/* �����ڵ�			: �����ڵ� */

	// ��ȸ���
	if(bNext)
		strFirstGu = "1";
	else
		strFirstGu = "0";
	memcpy(input->gubun,		(LPCTSTR)strFirstGu,	strFirstGu.GetLength());	/* ����				: 0:ó��1:���� */

	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	strTimeGu.Format("%c", chTypeRequested);
	memcpy(input->dgubun,	(LPCTSTR)strTimeGu,		strTimeGu.GetLength());	/* �����ͱ���		: 1:Tick,2:��,3:��,:��,5:�� */

//aaaaaaaaaaa	m_pChartItem->SetTypeExInputBarDlg(chTypeRequested);

	if ( chTypeRequested == TICK_DATA_CHART )		//	ƽ��Ʈ�ϰ�� ���ڸ� �ʱ�ȭ�ؼ� �ش�... �������ڰ� ����� ƽ��Ʈ ��ȸ�� �ȵǱ⶧��.
	{
		memcpy(input->date,		(LPCTSTR)strDate,		strDate.GetLength());		/* ����				: ���� */
		memcpy(input->chetime,	(LPCTSTR)strTime,		strTime.GetLength());		/* �ð�				: �ð� */
	}
	else
	{
		memcpy(input->date,		(LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(input->date) + sizeof(input->chetime));		/* ����				: ���� */
	}

	//	�� ����
	strSelectMin = lpSelChart ? lpSelChart->m_strTickMinDayNumber : m_pChartItem->GetTickMinDayNumberRequested();
	int nSelMin = atoi(strSelectMin);
//	if(lpSelChart)
//		m_pChartItem->SetTickMinDayNumber(nSelMin);

	//{{JS.Kim_20100903 Solomon 99�� �̻� ����Ÿ ó��
	//if (nSelMin < 100)
	//	strSelectMin.Format(_T("%d"), nSelMin);
	//else
	//	strSelectMin = "1";
	strSelectMin.Format(_T("%d"), nSelMin);
	//}}

	memcpy(input->nsel,		(LPCTSTR)strSelectMin,	strSelectMin.GetLength());	/* N�б���			: 0:30sec,1:1M,2:2M,... */

	// ��ȸ�Ǽ�
	int nQueryCnt = GetCntForQuery(bNext);
	if ( nQueryCnt == 0 )	nQueryCnt = DEFAULTCNTATONCE;
	strDataCnt.Format("%04d", nQueryCnt);
	memcpy(input->dcnt,		(LPCTSTR)strDataCnt,	strDataCnt.GetLength());	/* �����ͰǼ�		: �����ͰǼ� */

	if ( m_pChartItem->GetBNormalizedMinuteTimeRule() )			//	TRUE : ����� ����, FALSE : �Ǻ���
		strQtyCd = "0";
	else
		strQtyCd = "1";
	
	//<<20100317_JS.Kim �ַθ�.  PairTick�� ó���ϱ� ���� �ŵ�/�ż� ������ ���� ���õ� ���θ����� ó������ �ʰ� �����ڵ�� �����ϴ� ���� �߰��Ѵ�.
	//if (m_strOrderType == "0")	// �ŵ�
	//	input->mgubun[0] = '0';
	//else						// �ż�
	//	input->mgubun[0] = '1';
	if( chTypeRequested == '0' && nSelMin == 1 )
	{
		/*
		if(lpSelChart)	strJMCode = lpSelChart->m_strItemCode;
		else			strJMCode = m_strCodeForRequest;

		if( strJMCode.Left(3) == "|S_" )		input->mgubun[0] = '0';		//// �ŵ�
		else									input->mgubun[0] = '1';		//// �ż�
		*/
		input->mgubun[0] = '2';		//// ��ü
	}
	else
	{
		if (m_strOrderType == "0")	// �ŵ�
			input->mgubun[0] = '0';
		else						// �ż�
			input->mgubun[0] = '1';
	}
	if(lpSelChart)	strJMCode = CStdDialog::GetRealItemCode( lpSelChart->m_strItemCode);
	else			strJMCode = CStdDialog::GetRealItemCode( m_strCodeForRequest);

	//>>

	//	�� ����ü�� �޸� ������ ����� ���������� �Ѵ�.
	stSend2Server* st = new stSend2Server;
	st->m_pBuffer = (void*)input;
	st->m_lLength= sizeof(SM_fx112_InRec1);
	return st;
	//>>
}

//������ǥ TR�� ������ ���� Input���� ��������.
CString CChartItemData::GetMarketQueryData_ChartItemData(BOOL bNext, UINT nRqstCnt)
{
	m_lDataSize = 0;
	m_lCnt = 0;

	if (m_strTRCode == QUERY_STRMARKET)
		return GetQueryDataMarket(bNext, nRqstCnt);//return GetQueryDataMarket(bNext);		

	return "";
}

BOOL CChartItemData::MakeChartData (void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	BOOL r = FALSE;

	if (m_strTRCode == QUERY_STRFX)
	{
		MaketOutputData(szData, nLength, lpSelChart);
		return MakeChartData_EU_p0602 ((void *)((char *)szData+sizeof(SM_fx112_OutRec1)), nLength-sizeof(SM_fx112_OutRec1), lpSelChart);		
	}
	return FALSE;
}

BOOL CChartItemData::MaketOutputData(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	SM_fx112_OutRec1* pOutRec1 = (SM_fx112_OutRec1 *)szData;
	if(pOutRec1 == NULL) return FALSE;

	m_strName.Format("%*.*s", sizeof(pOutRec1->name), sizeof(pOutRec1->name), pOutRec1->name);
	m_strName.TrimLeft();		m_strName.TrimRight();

	CString strType;
	strType.Format("%*.*s", sizeof(pOutRec1->piplowest), sizeof(pOutRec1->piplowest), pOutRec1->piplowest);
	int nType = atoi(strType);
	if( nType == 0 )
		m_strPosition = "�� 1";
	else
		m_strPosition.Format("�� %.*f", (int)log10(nType), (double)1/(double)nType); 

	// ����Ű
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// ������ǥ������ chkeyOld ����
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));

	CString sKey;
	sKey.Format("%*.*s", sizeof(pOutRec1->nkey), sizeof(pOutRec1->nkey), pOutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");

	ZeroMemory(m_chkey, sizeof(m_chkey));
	if( lpSelChart )	lpSelChart->m_strKey = "";
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
		if(lpSelChart)
			lpSelChart->m_strKey = sKey;
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}

	return TRUE;
}

/*
	�������� ���� ��Ʈ �����͸� ��Ʈ�� �Ѱ��ֱ� ���� �����͸� �����ϴ� �κ�
	EU_p0602_OutRec2 ������ ���缭 ��ȯ ok
*/
BOOL CChartItemData::MakeChartData_EU_p0602(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	m_strOpen	= "";			//	�ð�
	m_strHigh	= "";			//	��
	m_strLow	= "";			//	����

	if(szData == NULL) return FALSE;

	//	������ ī��Ʈ ���ϱ�
	CString strDataCnt = "", strTemp = "";
	long nDataCnt = 0;

	strDataCnt = CString((char*)szData, sizeof (m_pfx112_ChartData->fcnt));
	nDataCnt = atol(strDataCnt);

	m_pChartItem->EnableRequestMoreButton (TRUE);	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)

	CString sKey = "";
	sKey = lpSelChart ? lpSelChart->m_strKey : m_pChartItem->m_strNextKey;
	sKey.TrimLeft();	sKey.TrimRight();

	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, (LPCTSTR)sKey, sKey.GetLength());

	if (sKey.IsEmpty() || atol(sKey) == 0)
		m_pChartItem->EnableRequestMoreButton(FALSE);// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
/*
	if(lpSelChart)
		// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
		lpSelChart->m_strKey.Format("%14.14s", m_chkey);
*/
	if (m_pfx112_ChartData)
	{
		char *pData = (char*)m_pfx112_ChartData;
		delete [] pData;
		m_pfx112_ChartData = NULL;
	}

	long nDataSize = sizeof(m_pfx112_ChartData->fcnt) + sizeof(m_pfx112_ChartData->bojomsg) + (nDataCnt * sizeof(SM_fx112_OutRec2));
	m_pfx112_ChartData = (SM_fx112_CHART *) new char [nDataSize * 2];

	m_strCode = m_strCodeForRequest;

	// �����
	char szTemp [64];
	long lCnt = 0;


	// ��Ʈī��Ʈ
	lCnt = m_lCntTemp = nDataCnt;

	long lPosForChart = 0, nBojoMsgLen = 0;

	// ��Ʈ������
	strDataCnt = CString ((char*)szData + sizeof (m_pfx112_ChartData->fcnt), BOJO_MSG_LENGTH_SIZE);
	nBojoMsgLen = atoi(strDataCnt) + BOJO_MSG_LENGTH_SIZE;

	m_lDataSize = lPosForChart = nLength;
	memcpy (m_pfx112_ChartData, (char*)szData , lPosForChart);

/*
	strTemp = CString ((char*)szData, nBojoMsgLen);
	int nStart = strTemp.Find("@TDC=")+ 5;
	strTemp = strTemp.Mid (nStart);
	nStart = strTemp.Find (":");
	m_strPosition.Format ("�� %s", strTemp.Left(nStart));
*/
	//	�Ҽ��� ���ϱ�... 
	int nStart = m_strPosition.GetLength () - m_strPosition.Find (".") - 1;

	SM_fx112_OutRec2		OutRec2;
	//SM_fx112_OutRec2_Sub	*pOutRec2_Sub;

	CString strItemCode;

	if(lpSelChart)	strItemCode = CStdDialog::GetRealItemCode( lpSelChart->m_strItemCode);
	else			strItemCode = CStdDialog::GetRealItemCode( m_strCodeForRequest);

	BOOL bAskItem = FALSE;
	if (!strItemCode.IsEmpty ()) 
		if (strItemCode.GetAt (0) == '|') 
			if (strItemCode.GetAt (1) == 'S') 
				bAskItem = TRUE;

	//if( bAskItem )			// �ż�
	//	pOutRec2_Sub = &OutRec2.fx112OutRec2_B;
	//else
	//	pOutRec2_Sub = &OutRec2.fx112OutRec2_S;

	memcpy (&OutRec2, ((char*)szData) + (nBojoMsgLen + sizeof(m_pfx112_ChartData->fcnt)) + ( sizeof(SM_fx112_OutRec2) * (lCnt - 1 ) ), sizeof(SM_fx112_OutRec2));

	memset (szTemp, 0x00 , sizeof(szTemp));
	memcpy (szTemp, OutRec2.date, sizeof(OutRec2.date));
	m_lEndDate = atol (szTemp);

	//	�ð�
	memset (szTemp, 0x00 , sizeof (szTemp));
	memcpy (szTemp, OutRec2.open, sizeof (OutRec2.open));
	m_strOpen = szTemp;

	//	��
	memset (szTemp, 0x00 , sizeof (szTemp));
	memcpy (szTemp, OutRec2.high, sizeof (OutRec2.high));
	m_strHigh = szTemp;

	//	����
	memset (szTemp, 0x00 , sizeof (szTemp));
	memcpy (szTemp, OutRec2.low, sizeof (OutRec2.low));
	m_strLow = szTemp;

	//	���簡
	memset (szTemp, 0x00 , sizeof (szTemp));
	memcpy (szTemp, OutRec2.close, sizeof (OutRec2.close));
	m_strPrice = szTemp;


	//	ù ���� ���ϱ�
	memcpy (&OutRec2, ((char*)szData + (nBojoMsgLen + sizeof(m_pfx112_ChartData->fcnt))), sizeof(SM_fx112_OutRec2));
	memset (szTemp, 0x00 , sizeof(szTemp));
	memcpy (szTemp, OutRec2.date, sizeof(OutRec2.date));
	m_lStartDate = atol (szTemp);

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
	return 0;
}


/*
*/
BOOL CChartItemData::RemovePacketAndGraph()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog ("CChartItemData::RemovePacketAndGraph::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	CString strTitle;
	long lSize = m_strArrayGraphTitle.GetSize ();

	for (long lPos = 0; lPos < lSize; lPos++)
	{
		strTitle = m_strArrayGraphTitle.GetAt (lPos);
		m_pChartItem->RemoveChart (strTitle);
	}
	m_strArrayGraphTitle.RemoveAll ();

	lSize = m_strArrayPacketName.GetSize ();
	for (lPos = 0; lPos < lSize; lPos++)
	{
		strTitle = m_strArrayPacketName.GetAt (lPos);
		m_pChartItem->RemovePacket (strTitle);
	}
	m_strArrayPacketName.RemoveAll ();

#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog ("CChartItemData::RemovePacketAndGraph::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	return TRUE;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
BOOL CChartItemData::AddLoadedPacket(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart)
{
	m_strArrayPacketName.RemoveAll();
	if(lpSelChart && lpSelChart->m_strChartGubun == QUERY_STRFX)
	{
		// 2011.01.13 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
//		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		// 2011.01.13 by SYS <<

		m_strArrayPacketName.Add(g_strPacketName[0]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[1]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[2]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[3]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[4]);

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
		if(m_pChartItem->GetPtrST())// it depends on System Trading Spec. gm0604 2005/10/04
		{	
			ASSERT(IsPrimaryChartItemData);
			if(m_strTRCode == QUERY_STRFX)
			{
// --> [Edit]  ������ 2008/09/08
				// 2011.01.13 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
				//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				// 2011.01.13 by SYS <<

				m_strArrayPacketName.Add(g_strPacketName[0]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[1]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[2]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[3]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[4]);
//				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//				m_strArrayPacketName.Add(g_strPacketName[5]);		

			}
			else if(m_strTRCode == QUERY_STRFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
			{
				// 2011.01.13 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
				//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				// 2011.01.13 by SYS 
				m_strArrayPacketName.Add(g_strPacketName[0]);

				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[1]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[2]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[3]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[4]);
				
//				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//				m_strArrayPacketName.Add(g_strPacketName[5]);
			}

			AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
			
			m_strArrayGraphTitle.Add("���� �̵����");
			m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);


//			m_strArrayGraphTitle.Add(g_strIndexName[1]);
//			m_pChartItem->AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);

			m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)

		}
		else
		{	
			if(IsPrimaryChartItemData)
			{			
				if(m_strTRCode == QUERY_STRFX)
				{
						// 2011.01.13 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
						//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						// 2011.01.13 by SYS 

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
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[1]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[2]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
							m_strArrayPacketName.Add(g_strPacketName[3]);
							m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
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
//						// >>>>>>>>>>>>>>>>>>> g_strPacketName[6]  "������" <<<<<<<<<<<<<<<<<<<<<<<<<<						
//						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//						m_strArrayPacketName.Add(g_strPacketName[7]);
						
						m_pChartItem->AddPacket(MakePacketOption("�ð�GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add("�ð�GuideLine");				
						m_pChartItem->AddPacket(MakePacketOption("��GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add("��GuideLine");				
						m_pChartItem->AddPacket(MakePacketOption("����GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add("����GuideLine");				

						if(!IsNoVolumeGraph)
						{
							AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
							m_strArrayGraphTitle.Add("���� �̵����");
							m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
						}
						else
						{
							m_strArrayGraphTitle.Add(g_strIndexName[0]);
							m_pChartItem->AddGraph(0,0,0,"������Ʈ",FALSE,TRUE,TRUE);

							m_strArrayGraphTitle.Add("���� �̵����");
							m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);

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
			}
		}
		m_bInitalChart = TRUE;
	
#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX�� AddPacket/Graph::End"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
	}
	return TRUE;
}

// ���ο� ��Ŷ,��Ʈ �߰� : �������� - ojtaso (20061224)
BOOL CChartItemData::AddPacketAndGraph2 (BOOL bAddGraph /*= TRUE*/, BOOL bOverlapp /*= FALSE*/)
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
		m_pChartItem->WriteToStringLog (_T("OCX�� AddPacket/Graph::Begin"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
		m_strArrayPacketName.RemoveAll ();
		BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData (this);
		if (m_pChartItem->GetPtrST ())	// it depends on System Trading Spec. gm0604 2005/10/04
		{	
			ASSERT(IsPrimaryChartItemData);
			if(m_strTRCode == QUERY_STRFX)
			{
				// 2011.01.13 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
				//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				// 2011.01.13 by SYS 

				m_strArrayPacketName.Add(g_strPacketName[0]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[1]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[2]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[3]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[4]);
//				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//				m_strArrayPacketName.Add(g_strPacketName[5]);		
			}

			AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart

			m_strArrayGraphTitle.Add("���� �̵����");
			m_pChartItem->AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
			m_strArrayGraphTitle.Add(g_strIndexName[1]);
			m_pChartItem->AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);
		}
		else
		{	
			if(IsPrimaryChartItemData)
			{			
				if(m_strTRCode == QUERY_STRFX)
				{
					// 2011.01.13 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					// 2011.01.13 by SYS 

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
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
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

//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[5]);
//
//					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
//					m_strArrayPacketName.Add(g_strPacketName[7]);


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
						AddGraphPriceChart_Overlapp(nRow, nCol);

					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}

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
// (2008/9/14 - Seung-Won, Bae) for Pair Tick
void CChartItemData::AddGraphPriceChart2NewItem(int& nRow, int& nCol, BOOL bAddGraph)
{
	m_strArrayGraphTitle.Add(g_strIndexName[0]);

	int nMainRow = -1;
	CString strPairRQ = m_pChartItem->GetSelectedRQ();
	CString strMainRQ( strPairRQ);
	if( strPairRQ.Right( 5) != "_PAIR")	strPairRQ = strPairRQ + "_PAIR";
	else								strMainRQ = strMainRQ.Left( strMainRQ.GetLength() - 5);
	int nRequestPairTickState = ( ( CStdDialog *)m_pChartItem->m_pStdDialog)->GetRequestPairTickState();
	switch( nRequestPairTickState)
	{
		case E_RPTS_PAIR:
		case E_RPTS_LOOP_PAIR:
		case E_PRTS_ROTATE_PAIR:
			bAddGraph = !m_pChartItem->HasIndicatorWithRQ( "������Ʈ", strPairRQ);
			if( bAddGraph) nMainRow = m_pChartItem->GetRowOfGraphWithRQ( "������Ʈ", strMainRQ);
			break;
	}
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
			switch( nRequestPairTickState)
			{
				case E_RPTS_PAIR:
				case E_RPTS_LOOP_PAIR:
				case E_PRTS_ROTATE_PAIR:
					RowCount = nMainRow;
					m_pChartItem->AddGraph(RowCount,0,0,"������Ʈ",FALSE,FALSE,TRUE);
					break;
				default:
					m_pChartItem->AddGraph(RowCount,0,0,"������Ʈ",TRUE,TRUE,TRUE);
					break;
			}
		}

		nRow = RowCount;
		nCol = 0;

		//<<20100329_JS.Kim �ؿ��� �Űܿ�
		if( nRequestPairTickState != E_RPTS_PAIR
			&& nRequestPairTickState != E_RPTS_LOOP_PAIR
			&& nRequestPairTickState != E_PRTS_ROTATE_PAIR)
		{
			m_strArrayGraphTitle.Add("���� �̵����");
			m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);
		}
		//>>

	}

	//<<20100329_JS.Kim ���� if(bAddGraph) ������ �ű�
	/*
	if( nRequestPairTickState != E_RPTS_PAIR
		&& nRequestPairTickState != E_RPTS_LOOP_PAIR
		&& nRequestPairTickState != E_PRTS_ROTATE_PAIR)
	{
		m_strArrayGraphTitle.Add("���� �̵����");
		m_pChartItem->AddGraph(nRow,nCol,0,"���� �̵����",FALSE,FALSE,FALSE);
	}
	*/
	//>>
}

// ���ο� ��Ʈ �߰� : �������� - ojtaso (20061224)
void CChartItemData::AddGraphPriceChart_Overlapp(int& nRow, int& nCol)
{
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
	EU_p0607_InRec1 eu0607InRec1;

	char Temp[64];
	char chFlag;

	// �ʱ�ȭ
	memset(&eu0607InRec1, 0x20, sizeof(eu0607InRec1));
	
	// ��������̵�
	CString sWndID;
	sWndID.Format("%d", m_pChartItem->GetDlgCtrlID());
	memcpy(eu0607InRec1.winid, sWndID, sWndID.GetLength());

	// ����Ÿ��ȣ
	memcpy(eu0607InRec1.dataNm, (LPTSTR)(LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
	
	// ������(0:��/1:�Ǻ�)
	eu0607InRec1.img[0] = '1';//���(0) �Ǻ�(1);
	if(m_pChartItem->m_CfgInfo.bRealCandle == false) //����̶��..
		eu0607InRec1.img[0] = '0';

	CString strPrimCode = m_pChartItem->GetSelectedItemCode();
	if(strPrimCode.GetLength() == UPJONG_LENGTH)// ����
	{
		// Ÿ��(0:����/1:����)
		eu0607InRec1.type [0]= '1';

		// �����ڵ�
		memcpy(eu0607InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(eu0607InRec1.shcode));
		eu0607InRec1.shcode[4] = ' ';	
		eu0607InRec1.shcode[5] = ' ';	
		eu0607InRec1.shcode[6] = ' ';	
		eu0607InRec1.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == CODE_STOCK_LENGTH || strPrimCode.GetLength() == (CODE_STOCK_LENGTH-1))							// ����
	{
		// Ÿ��(0:����/1:����)
		eu0607InRec1.type[0] = '0';

		// �����ڵ�
		memcpy(eu0607InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(eu0607InRec1.shcode));
		eu0607InRec1.shcode[6] = ' ';
		eu0607InRec1.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == FUTOPT_LENGTH)// || strPrimCode.GetLength() == COMMODITY_LENGTH)							// ����
	{
		// Ÿ��(0:����/1:����)
		eu0607InRec1.type[0] = '0';

		// �����ڵ�
		memcpy(eu0607InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(eu0607InRec1.shcode));
	}

	// �˻�����(�ֱ���)
	memcpy(eu0607InRec1.edate,(LPCTSTR)m_pChartItem->GetEndDateRequested(),sizeof(eu0607InRec1.edate));
	if(nRqstCnt >= 10000)
	{
		AfxMessageBox("������ǥ �ִ� ��ȸ ������ 10000�� �����Դϴ�");
		nRqstCnt = 9999;
	}

	// �˻��Ǽ�
	m_lCntRequired = nRqstCnt; //��ȸ�� ������ ���� ���� ������ ���ϱ� ���� �����صд�.
	sprintf(Temp,"%04d",nRqstCnt);
	
	if(atoi(Temp) <= 0) return _T("");
	memcpy(eu0607InRec1.num,Temp,sizeof(eu0607InRec1.num));

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
	memcpy(eu0607InRec1.nxtGb,&chFlag,sizeof(eu0607InRec1.nxtGb));

	// ��ȸŰ(����/����)
	memcpy(eu0607InRec1.nxtKey,m_chkey,sizeof(eu0607InRec1.nxtKey));

//@�����߰�
	CString strFidInfo("0                                                                               ");
	CString strTrInfo;
	char* pszTrInfo = strTrInfo.GetBuffer(sizeof(eu0607InRec1));
	memcpy(pszTrInfo, (char*)&eu0607InRec1, sizeof(eu0607InRec1));
	strTrInfo.ReleaseBuffer();
	CString strRecordInfo("999999999999991               000001002003004005006007|2               000001002003;");
	CString strOutput;
	strOutput = strFidInfo + strTrInfo + strRecordInfo;
	return strOutput;
//@�����߰�
}

long CChartItemData::GetMaximumRequestCnt()
{
	if( (m_strTRCode == QUERY_STRFX) ||
		(m_strTRCode == QUERY_STRUPJONG ) ||
		(m_strTRCode == QUERY_STRMARKET ) || 
		(m_strTRCode == QUERY_STRFUTOPT ) ||
		(m_strTRCode == QUERY_STRSTOCKFUTOPT ) ||
		(m_strTRCode == QUERY_STRFOREIGN) 
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
		m_lCntRequired = m_lCntRequiredByPrim;
	}

	return m_lCntRequired;	
}

BOOL CChartItemData::ProcRequestCntAfterQuery(long lCntReceived)
{
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

long CChartItemData::GetData ()
{
	if(m_strTRCode == QUERY_STRFX)
		return (long)m_pfx112_ChartData; 

	return 0L;
}

/*
	ok
*/
void CChartItemData::RemoveRealItem()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveRealItem::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	CString strRealItemRegistered;
	int nSize = m_strArrayRealItemRegistered.GetSize ();

	for (int nPos = 0; nPos < nSize; nPos++)
	{
		strRealItemRegistered = m_strArrayRealItemRegistered.GetAt (nPos);
		m_pChartItem->RemoveRealItem (strRealItemRegistered);
	}
	m_strArrayRealItemRegistered.RemoveAll ();

#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveRealItem::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
}

void CChartItemData::SetChartNumericValue(int nValue)
{
	//<<20100329_JS.Kim FX�� ���� ���� Type�� �ٸ��Ƿ� ���⼭ �ϰ������ϸ� �ȵȴ�.
	/*
	if(m_strTRCode==QUERY_STRFX)
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
			m_pChartItem->ChangePacketType(g_strPacketName[1],m_strPosition);
			m_pChartItem->ChangePacketType(g_strPacketName[2],m_strPosition);
			m_pChartItem->ChangePacketType(g_strPacketName[3],m_strPosition);
			m_pChartItem->ChangePacketType(g_strPacketName[4],m_strPosition);
		}
		else if(nValue == DOLLAR_TYPE)
		{
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[6]);
		}
	}	
	*/
	//>>
}

void CChartItemData::AddRealItem()
{
	BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
	
	if(m_strTRCode == QUERY_STRFX)
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

//				// �ŷ���
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
				}
			}
	}		
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
//	m_strArrayPacketName.Add(g_strPacketName[5]);
	
//	if(!::lstrcmp(lpszTRCode, QUERY_STRFX))
//		m_strArrayPacketName.Add(g_strPacketName[7]);
}

void CChartItemData::OnSelectChartItem(ST_SELCHART* lpSelChart)
{
	SetTRCode(lpSelChart->m_strChartGubun);
	m_strCodeForRequest = lpSelChart->m_strItemCode;
//	memcpy(m_chkey, (LPCTSTR)lpSelChart->m_strKey, lpSelChart->m_strKey.GetLength());
}

void CChartItemData::GetOHLValue(CString &strOpen, CString &strHigh, CString &strLow)
{
	strOpen = m_strOpen;			//	�ð�
	strHigh = m_strHigh;			//	��
	strLow = m_strLow;			//	����
}

CString CChartItemData::GetReciveValue(short nType)
{
	CString strTemp = "";
	switch ( nType )
	{
	case 0:
		return m_strName;
	case 1:
		return m_strPrice;
	case 2:
		return m_strOpen;
	case 3:
		return m_strHigh;
	case 4:
		return m_strLow;
	case 5:
		return m_strSign;
	case 6:
		return m_strChange;
	case 7:
		return m_strChrate;
	case 8:
		return m_strVolume;
	case 9:
		return m_strPreprice;
	case 10:
		return m_strKey;
	case 11:
		{
			strTemp.Format("%ld", m_lStartDate);
			return strTemp;
		}
		break;
	case 12:
		{
			strTemp.Format("%ld", m_lEndDate);
			return strTemp;
		}
		break;
	case 13:
		{
			strTemp.Format("%ld", m_lDataSize);
			return strTemp;
		}
		break;
	}
	return "";
}


CString CChartItemData::GetPipLowest()
{
	CString strResult = m_strPosition;
	strResult.Replace("�� ", "");
	return strResult;
}
