// ReplayChartWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Conversion.h"
#include "ReplayChartWnd.h"
#include "DlgRecvTRProgress.h"

#include "../COMMONTR/KB_d1016.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//const UINT RMSG_SENDMESSAGE = ::RegisterWindowMessage(_T("RMSG_SENDMESSAGE"));
const UINT RMSG_RECEIVEREPLAYDATA = ::RegisterWindowMessage(_T("RMSG_RECEIVEREPLAYDATA"));
const UINT RMSG_GET_TRCOMM = ::RegisterWindowMessage(_T("RMSG_GET_TRCOMM"));

/////////////////////////////////////////////////////////////////////////////
// CReplayChartWnd

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
// Modifier		: 
// Comments		: Construction
//-----------------------------------------------------------------------------
CReplayChartWnd::CReplayChartWnd()
{
	m_nReplaySpeed = 1;				// ���� ��� 
	m_nReplayDayCount = 0;

	m_unTimerHandle = 100;			// Timer�� Handle
	m_unTRWaitTimerHandle = 101;	

	m_bReceiveTR = FALSE;
	m_hParent = NULL;
	m_pDlgRecvTRProgress = NULL;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
// Modifier		: 
// Comments		: Destruction
//-----------------------------------------------------------------------------
CReplayChartWnd::~CReplayChartWnd()
{
}

BEGIN_MESSAGE_MAP(CReplayChartWnd, CWnd)
	//{{AFX_MSG_MAP(CReplayChartWnd)	
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_RECEIVEREPLAYDATA, OnRmsgReceiveReplayData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplayChartWnd message handlers
int CReplayChartWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CWnd::OnCreate(lpCreateStruct);

	return 0;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
// Return void	: 
// Param  void *pReplayInfo : 
// Comments		: ���� TR �߻� dll�� �ʱ�ȭ
//-----------------------------------------------------------------------------
void CReplayChartWnd::Init( void *pReplayInfo )
{
	// 1. ���ڷ� ���� pointer�� ���� dll�� Parent Wnd���� ���� ������ �����Ѵ�.
	REPLAY_INFO *pRecvReplayInfo = ( REPLAY_INFO * )pReplayInfo;
	m_hMainFrame = pRecvReplayInfo->hRecvMainFrame;
	m_hParent = pRecvReplayInfo->hCallHwnd;

	// 3. MultiChart�� ���� ���� ���⿡ �ʿ��� ������ �����Ѵ�.
	GetReplayInfo(pRecvReplayInfo);

	// 4. ���ڷ� ���� ���� List���� ������ ��û�� TR�߿� �ִ��� Ȯ���Ѵ�.
	if (pRecvReplayInfo->arrStrCode.GetSize() <= 0)
		return;

	memset(m_rcvTrData.m_szKey, 0x20, sizeof(m_rcvTrData.m_szKey));
	m_rcvTrData.m_szStockCode = pRecvReplayInfo->arrStrCode.GetAt(0);
	m_rcvTrData.m_nNDayBefore = pRecvReplayInfo->nDayBefore;
	m_rcvTrData.m_nTickIdx = 0;
	m_rcvTrData.m_arrChart.RemoveAll();
//	m_rcvTrData.m_pTRData = NULL;
	m_rcvTrData.m_strBojoMsg = "";
	m_rcvTrData.m_szDateInfo = "000000000900";

	// 5. TR�� server�� �����Ѵ�.
	CString szStime;
	szStime.Format("%02d%02d%2s", m_StartTime.m_nHH, m_StartTime.m_nMM, "00");

	m_nRcvDate = m_rcvTrData.m_nNDayBefore;

	CalcTotalTime();

	// ProgressBar ����
	if (m_nTotalTime > 0)
	{
		m_pDlgRecvTRProgress = NULL;
		m_pDlgRecvTRProgress = new CDlgRecvTRProgress;
		m_pDlgRecvTRProgress->Create( this );
		m_pDlgRecvTRProgress->ShowWindow(SW_SHOW);
		m_pDlgRecvTRProgress->SetTRTotalCount(m_nTotalTime);
	}
	
	SendTRList2Server(szStime);
}

//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 26
// Return  void : 
// Param  REPLAY_INFO *pRecvReplayInfo   : 
// Explain		: MultiChart�� ���� ���� ���⿡ �ʿ��� ������ �����Ѵ�.
//-----------------------------------------------------------------------------
void CReplayChartWnd::GetReplayInfo(REPLAY_INFO *pRecvReplayInfo)
{
	if (pRecvReplayInfo == NULL)
		return;

	// 1. ���� �����ڸ� �����Ѵ�.
	switch (pRecvReplayInfo->nDataKind)
	{
		case 0 :		// �ֽ�
			m_szMarketID = "s";
			break;
		case 2 :		// ����
			m_szMarketID = "f";
			break;
		default :		// ���� �Է��� �ƴ� ���
			AfxMessageBox("���� �����ڰ� ���������� �Էµ��� �ʾҽ��ϴ�.");
			break;
	}

	// 2. ���� ���� �ð��� �����Ѵ�.
	CString szStartTime = pRecvReplayInfo->strStartDate;
	m_CurTime.m_nDD = pRecvReplayInfo->nDayBefore;
	m_CurTime.m_nHH = atoi(szStartTime.Mid(8, 2));
	m_CurTime.m_nMM = atoi(szStartTime.Mid(10, 2));
	m_CurTime.m_nSS = 0;

	m_sendTime = m_StartTime = m_CurTime;

	// 3. ������ PC�ð�(���⸦ �Ҷ��� PC�ð�)�� �����Ѵ�.
	SYSTEMTIME stRePresentTime;
	GetLocalTime(&stRePresentTime);

	// 3. ������ ����� �����Ѵ�.
	m_nReplaySpeed = pRecvReplayInfo->nSpeed;

	// 4. ���⸦ �� ��¥���� �����Ѵ�.
	m_nReplayDayCount = pRecvReplayInfo->nDayBefore;

	// 5. Timer�� ���� ������ ������ �����Ѵ�.
	m_fTimeInterval = (float)1000 / (float)m_nReplaySpeed;
}

//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 26
// Return  void : 
// Param   void : 
// Explain		: m_lstCurTRList�� ��ϵ� ������� TR���� Server�� ��û�Ѵ�.
//-----------------------------------------------------------------------------
void CReplayChartWnd::SendTRList2Server(CString strStime)
{
	SetTimer(m_unTRWaitTimerHandle, 30000, NULL);
	KB_d1016_InRec1 InRec1;

	// �ʱ�ȭ
	memset(&InRec1, 0x20, sizeof(InRec1));

	// �����ڵ�
	CString szCommonCode;
	szCommonCode.Format( "%-16s", m_rcvTrData.m_szStockCode);
	memcpy( InRec1.shcode, szCommonCode, 16);

	// �������n����
	CString strData;
	strData.Format(_T("%02d"), m_rcvTrData.m_nNDayBefore);
	memcpy(InRec1.remindDate, (LPCSTR)(LPCTSTR)strData, 2);

	// ����ð�
	memcpy(InRec1.remindTime, strStime, 6);			// HHMMSS
	memcpy(InRec1.nkey, m_rcvTrData.m_szKey, sizeof(InRec1.nkey));

	CString strInfo;
	char* szInfo = strInfo.GetBuffer(sizeof(InRec1));
	memcpy(szInfo, (char*)&InRec1, sizeof(InRec1));
	strInfo.ReleaseBuffer();

	if (m_hParent == NULL)
		return;

	ITrComm2* pTrComm = NULL;
	CString strPtMng;
	::SendMessage(m_hParent, RMSG_GET_TRCOMM, (WPARAM)&pTrComm, (LPARAM)&strPtMng);

	pTrComm->SetTR(QUERY_STRREPLAY);
	pTrComm->SetBaseData("PtManager", strPtMng);
	pTrComm->SetBaseData("SCREENCODE", "");

	pTrComm->Send2Server((LPVOID)(LPTSTR)(LPCTSTR)strInfo, strInfo.GetLength(), TRKEY_STRREPLAY);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 1 / 9
// Return int	: 
// Param  void	: 
// Comments		: 
//-----------------------------------------------------------------------------
int CReplayChartWnd::GetTotalTime( void )
{
	int nSizeOf_TICK_DATA = sizeof(KB_d1016_OutRec2);

	int nTotalSec = 0;
	int nStartTotalSec = 0;
	int nEndTotalSec = 0;
	
	// �����Ϻ��� �� ���� �ð��� ã�´�.
	for (int i=0; i<=m_StartTime.m_nDD; i++)
	{
		CString szMarketStartTime;
		CString szMarketEndTime = "153000";
		CString szTodayEndTime = "090000";
		
		// �� ���۽ð��� ���´�.
		szMarketStartTime = CDataConversion::GetMidToTrimDataIndex(m_rcvTrData.m_strBojoMsg, "MARKETTIME=", "@", 0, true);
		szMarketStartTime.TrimRight();
		
		// �� ���� �ð��� ������ �� ���� �ð��� üũ���ش�.
		int nIdx = 0;
		if ((nIdx = szMarketStartTime.Find('|')) > 0)
		{
			szMarketEndTime = szMarketStartTime.Mid(nIdx + 1);
			szMarketStartTime = szMarketStartTime.Mid(0, nIdx);
		}

		if (i < m_StartTime.m_nDD)
			szMarketStartTime = _T("090000");
		else if (i == m_StartTime.m_nDD)
		{
			szMarketStartTime.Format("%0.2d%0.2d%0.2d", m_StartTime.m_nHH, m_StartTime.m_nMM, m_StartTime.m_nSS);
			if (m_rcvTrData.m_nNDayBefore == 1)
				szMarketEndTime = _T("153000");
		}

		// �ð������� int������ ��ȯ
		int nStartTimeInfo = atoi( szMarketStartTime );
		int nEndTimeInfo = atoi( szMarketEndTime );

		int nMarketStartSS = nStartTimeInfo % 100;		// �� ���۽ð� : ��
		nStartTimeInfo = nStartTimeInfo / 100;
		int nMarketStartMM = nStartTimeInfo % 100;		// �� ���۽ð� : ��
		int nMarketStartHH = nStartTimeInfo / 100;		// �� ���۽ð� : ��

		int nMarketEndSS = nEndTimeInfo % 100;			// �� ����ð� : ��
		nEndTimeInfo = nEndTimeInfo / 100;
		int nMarketEndMM = nEndTimeInfo % 100;			// �� ����ð� : ��
		int nMarketEndHH = nEndTimeInfo / 100;			// �� ����ð� : ��

		m_MarketStartTime[i].m_nDD = i;
		m_MarketStartTime[i].m_nSS = nMarketStartSS;		// �� ���۽ð� : ��
		m_MarketStartTime[i].m_nMM = nMarketStartMM;		// �� ���۽ð� : ��
		m_MarketStartTime[i].m_nHH = nMarketStartHH;		// �� ���۽ð� : ��

		m_MarketEndTime[i].m_nDD = i;
		m_MarketEndTime[i].m_nSS = nMarketEndSS;			// �� ����ð� : ��
		m_MarketEndTime[i].m_nMM = nMarketEndMM;			// �� ����ð� : ��
		m_MarketEndTime[i].m_nHH = nMarketEndHH;			// �� ����ð� : ��

		nStartTotalSec = nMarketStartSS + ( nMarketStartMM * 60 ) + ( nMarketStartHH * 3600 );
		nEndTotalSec = nMarketEndSS + ( nMarketEndMM * 60 ) + ( nMarketEndHH * 3600 ); 

		nTotalSec = nTotalSec + ( nEndTotalSec - nStartTotalSec );
	}

	return nTotalSec;
}

LONG CReplayChartWnd::OnRmsgReceiveReplayData(WPARAM wParam, LPARAM lParam)
{
	return ReceiveData((LPVOID)wParam, (long)lParam);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
// Return HRESULT : 
// Param  LPVOID aTRData : 
// Param  long dwTRDateLen : 
// Comments		: TR ��ȸ ����� �޴´�.
//-----------------------------------------------------------------------------
HRESULT CReplayChartWnd::ReceiveData(LPVOID aTRData, long dwTRDateLen)
{
	if (dwTRDateLen < 0)
		return 0L;

	if (aTRData == NULL)
		return 0L;

	m_bReceiveTR = TRUE;

	KB_d1016_OutRec1* pOutRec1 = (KB_d1016_OutRec1*)aTRData;
	if (pOutRec1 == NULL)
		return FALSE;

	char szPage[5];
	memset(szPage, 0x00, 5);
	memcpy(szPage, pOutRec1->nkey, 4);

	memset(m_rcvTrData.m_szKey, 0x20, sizeof(m_rcvTrData.m_szKey));
	if (atoi(szPage) > 0)
		memcpy(m_rcvTrData.m_szKey, pOutRec1->nkey, sizeof(pOutRec1->nkey));
	CString str; 
	str.Format("%s",m_rcvTrData.m_szKey);
//	OutputDebugString("KHD : " + str);
	CString strCount;
	strCount.Format("%*.*s", sizeof(pOutRec1->fcnt), sizeof(pOutRec1->fcnt), pOutRec1->fcnt);
	int nTotalTickCount = atoi(strCount);
	char* pBojoMsg = NULL;
	pBojoMsg = new  char[atoi(pOutRec1->bojo) + 1];
	int nBojoLen = atoi(pOutRec1->bojo);
	memcpy( pBojoMsg, pOutRec1->bojo, nBojoLen);

	// ���� �޼������� �����ڵ带 ���´�.
	CString szStockCode = CDataConversion::GetMidToTrimDataIndex( pBojoMsg, "SC=", "@", 0, true );
	szStockCode.TrimRight();

	// �� ���۽ð��� ���´�.
	CString szMarketStartTime = CDataConversion::GetMidToTrimDataIndex( pBojoMsg, "MARKETTIME=", "@", 0, true );
	szMarketStartTime.TrimRight();

	//�ڽ��� ���� by LYH 2006.11.06
	CString szMarketEndTime = "310000";
	int nIdx = 0;
	if( ( nIdx = szMarketStartTime.Find('|') ) > 0 )
	{
		szMarketEndTime = szMarketStartTime.Mid( nIdx + 1 );
		szMarketStartTime = szMarketStartTime.Mid( 0, nIdx);
	}

	// TRData���� n���� Data�� ���´�.
	char pNDayBefore[] = "00";
	memcpy( (char*)pNDayBefore, (char*)pOutRec1->remindDate, 2 );
	CString szNDayBefore;
	szNDayBefore.Format( "%s", pNDayBefore );
	szNDayBefore.TrimLeft();
	int nDayBefore = atoi(szNDayBefore);

	if (m_rcvTrData.m_szStockCode != szStockCode)
		return 0L;

	// �ش�Ǵ� ��¥�� data�� ����Ѵ�.
//	if (m_rcvTrData.m_nNDayBefore != nDayBefore)
//		return 0L;

	m_rcvTrData.m_nTickIdx = 0;
	m_rcvTrData.m_nTotalTickCount = nTotalTickCount;

	// �� ���۽ð��� �����޼����� ���� �ð����� �ٲپ��ش�.
	if (szMarketStartTime.IsEmpty())
		m_rcvTrData.m_szDateInfo = "0000000090000";
	else
	{
		if (szMarketStartTime.GetLength() == 5)		// �� ���۽ð��� 90000���� ���ð��
			m_rcvTrData.m_szDateInfo = "00000000" + szMarketStartTime;
		else
			m_rcvTrData.m_szDateInfo = "0000000" + szMarketStartTime;
	}

	m_rcvTrData.m_szMarketEndTime = szMarketEndTime;

	char *pChartList = NULL;
	pChartList = (char*)((long)(pOutRec1) + sizeof(KB_d1016_OutRec1));
	int nRowCnt = (dwTRDateLen - sizeof(KB_d1016_OutRec1)) / sizeof(KB_d1016_OutRec2);

	char szStartTime[8], szEndTime[8];
	memset(szStartTime, 0x00, sizeof(szStartTime));
	memset(szEndTime, 0x00, sizeof(szEndTime));
	KB_d1016_OutRec2 outRec2;
	int i = 0;
	for ( i=0; i<nRowCnt; i++)
	{
		memset(&outRec2, 0x00, sizeof(outRec2));
		memcpy(&outRec2, pChartList+sizeof(KB_d1016_OutRec2)*i, sizeof(KB_d1016_OutRec2));

		m_rcvTrData.m_arrChart.Add(outRec2);
		if (i==0)
			memcpy(szStartTime, outRec2.time, sizeof(outRec2.time));
	}

	if (i>0)
	{
		memcpy(szEndTime, outRec2.time, sizeof(outRec2.time));
		if (atoi(szStartTime) > atoi(szEndTime))
			m_nRcvDate = 0;

		int nRate = CalcCurTime(atoi(szEndTime));

		if (m_pDlgRecvTRProgress != NULL)
		{
			m_pDlgRecvTRProgress->SetFocus();
			m_pDlgRecvTRProgress->AddCount(nRate);
		}
	}

	memcpy((char*)pNDayBefore, (char*)pOutRec1->remindDate, 2);

	szNDayBefore.Format("%s", pNDayBefore);
	szNDayBefore.TrimLeft();
	m_rcvTrData.m_nNDayBefore = atoi(szNDayBefore);

	m_rcvTrData.m_strBojoMsg.Format("%*.*s", nBojoLen, nBojoLen, pBojoMsg);
	m_rcvTrData.m_strBojoMsg = m_rcvTrData.m_strBojoMsg.Mid(4);	// �����޼��� count ����

	if (pBojoMsg)
	{
		delete [] pBojoMsg;
		pBojoMsg = NULL;
	}

	KillTimer(m_unTRWaitTimerHandle);

	if (atoi(szPage) > 0)
	{
		CString szStime;
		szStime.Format("%02d%02d%2s", m_StartTime.m_nHH, m_StartTime.m_nMM, "00");
		
		SendTRList2Server(szStime);
		return 0L;
	}

	// MultiChart�� ��ü �ð�( �ʱ��� )�� �����ش�.
	int nTotalCount = GetTotalTime();
	GetParent()->SendMessage( RMSG_REPLAY_TOTAL_SEC,  nTotalCount);

	// ���� Progress Dialog�� ������ �Ҹ��Ų��.
	if (m_pDlgRecvTRProgress != NULL)
	{
		m_pDlgRecvTRProgress->ShowWindow(SW_HIDE);
		m_pDlgRecvTRProgress->DestroyWindow();
		m_pDlgRecvTRProgress = NULL;
	}

	m_fTimeInterval = ( 1000 / m_nReplaySpeed ) * 0.001f;
	SetTimer(m_unTimerHandle, 1000 / m_nReplaySpeed, NULL);

	return 0L;
}

//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 9 / 26
// Return  BOOL : 
// Param  int nMode : 0 : Stop, 1 : Play, 2 : Pause
// Explain		: 
//-----------------------------------------------------------------------------
BOOL CReplayChartWnd::Run(int nMode) //0:Stop, 1:Play, 2:Pause
{
	switch (nMode)
	{
		case STOP:
			KillTimer(m_unTimerHandle);
			m_CurTime.m_nDD = m_StartTime.m_nDD;
			m_CurTime.m_nHH = m_StartTime.m_nHH;
			m_CurTime.m_nMM = m_StartTime.m_nMM;
			m_CurTime.m_nSS = m_StartTime.m_nSS;

			m_rcvTrData.m_nTickIdx = 0;
			break;
		// Multi Chart���� Play ��ư�� ������ �����Ѵ�.
		case PLAY:
			m_fTimeInterval = ( 1000 / m_nReplaySpeed ) * 0.001f;
			SetTimer( m_unTimerHandle, 1000 / m_nReplaySpeed, NULL);
			break;
		case PAUSE:
			KillTimer(m_unTimerHandle);
			break;
	}

	return TRUE;
}


void CReplayChartWnd::ChangeSpeed(int nSpeed)
{
	m_nReplaySpeed = nSpeed ;
	UINT nTimer = 1000 / nSpeed;
	SetTimer(m_unTimerHandle, nTimer, NULL);
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 18
// Return  void : 
// Param  UINT nIDEvent   : 
// Explain		: 
//-----------------------------------------------------------------------------
void CReplayChartWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_unTimerHandle)
		SendTickData2MultiChart();
	else if (nIDEvent == m_unTRWaitTimerHandle)
	{
		// Server���� TR �����͸� �������� ���ϸ� MultiChart�� ErrorMessage�� ����
		if (m_bReceiveTR == FALSE)
			GetParent()->SendMessage(RMSG_REPLAY_FAIL,  NULL, NULL );

		KillTimer(m_unTRWaitTimerHandle);
		AfxMessageBox( "TR�� ���������� ���� ���߽��ϴ�." );
		if (m_pDlgRecvTRProgress != NULL)
		{
			m_pDlgRecvTRProgress->ShowWindow(SW_HIDE);
			m_pDlgRecvTRProgress->DestroyWindow();
			m_pDlgRecvTRProgress = NULL;
		}
	}

	CWnd::OnTimer(nIDEvent);
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 27
// Return  void : 
// Param  void  : 
// Explain		: MultiChart�� ���� TickData�� ������.
//-----------------------------------------------------------------------------
void CReplayChartWnd::SendTickData2MultiChart()
{
	int nSizeOf_TICK_DATA = sizeof(KB_d1016_OutRec2);
	char* pData = NULL;
	CString sTemp;

//	BOOL bChangeDay = FALSE;
	m_bSendCurTime = FALSE;			// ���� Ȯ���ϰ� ����

	CString szMarketEndTime = "";
	CString szCurTime = "";
	szMarketEndTime.Format("%0.2d%0.2d%0.2d",
		m_MarketEndTime[m_nReplayDayCount].m_nHH, m_MarketEndTime[m_nReplayDayCount].m_nMM, m_MarketEndTime[m_nReplayDayCount].m_nSS );
	szCurTime.Format("%0.2d%0.2d%0.2d",	m_CurTime.m_nHH, m_CurTime.m_nMM, m_CurTime.m_nSS );
/*
	// 2. ���� �ð��� �� ����ð��� �Ǿ����� Ȯ���Ѵ�.
	if (szCurTime >= szMarketEndTime)
	{
		// a. ���� �ð��� Ȯ���ϰ� MultiChat�� �����Ѵ�.
		if (szCurTime > szMarketEndTime && m_CurTime.m_nDD == 0)
		{
			KillTimer( m_unTimerHandle );
			AfxMessageBox("���� �ð��� ���� ���� �ð��� Ȯ���ϼ���.");
			GetParent()->SendMessage(RMSG_REPLAY_END);
			return;
		}
		else
			SendCurTime();

		// b. ���� �ð��� ��¥�� 0�� �̸� ���� ����
		if (m_CurTime.m_nDD == 0)
		{
			KillTimer( m_unTimerHandle );
			GetParent()->SendMessage(RMSG_REPLAY_END);
			return;
		}

		// c. ���� �ð��� �� ���� �ð����� �����Ѵ�.
		m_nReplayDayCount--;
		
		m_StartTime.m_nDD = m_CurTime.m_nDD = m_nReplayDayCount;
		m_StartTime.m_nHH = m_CurTime.m_nHH = m_MarketStartTime[m_nReplayDayCount].m_nHH;
		m_StartTime.m_nMM = m_CurTime.m_nMM = m_MarketStartTime[m_nReplayDayCount].m_nMM;
		m_StartTime.m_nSS = m_CurTime.m_nSS = m_MarketStartTime[m_nReplayDayCount].m_nSS;
		
		bChangeDay = TRUE;
	}
*/
	// 3. ���� ���� �ð��� 9�ð� �ƴ� ��� ������ TickCount�� ��ȸ�ð��� ������´�.
	if ((m_StartTime.m_nDD == m_CurTime.m_nDD) && ((m_StartTime.m_nHH == m_CurTime.m_nHH) &&
		(m_StartTime.m_nMM == m_CurTime.m_nMM) && (m_StartTime.m_nSS == m_CurTime.m_nSS)))
	{
		MoveTickIdx();
	}

	// 1�ʰ� �����Ǿ��ٴ� Message "DDHHMMSS" ����
//	if (bChangeDay != TRUE)
		SendCurTime();

	HWND hParent = GetParent()->GetSafeHwnd();

	if (m_rcvTrData.m_nNDayBefore == m_nReplayDayCount)
	{
		if (m_rcvTrData.m_arrChart.GetSize() <= 0)
		{
			KillTimer( m_unTimerHandle );
			GetParent()->SendMessage(RMSG_REPLAY_END);
			return;
//			IncreaseTime();
//			return;
		}
	}

	KB_d1016_OutRec2 curTickData, beforeData;
	int nSize = m_rcvTrData.m_arrChart.GetSize();//�Ųٷ� �о��ش�. IBK 
	for (int i=0; i<MAX_COUNT; i++)
	{
		if (m_rcvTrData.m_nTickIdx >= nSize ||m_rcvTrData.m_nTickIdx < 0 )
		{
			KillTimer( m_unTimerHandle );
			GetParent()->SendMessage(RMSG_REPLAY_END);
			break;
		}

		curTickData = m_rcvTrData.m_arrChart.GetAt(m_rcvTrData.m_nTickIdx);

		szCurTime.Format("%02d%02d%02d", m_CurTime.m_nHH, m_CurTime.m_nMM, m_CurTime.m_nSS);

		CString szTime = (LPCSTR)curTickData.time;
		szTime = szTime.Left(6);
		if (atoi(szCurTime) < atoi(szTime))
			szCurTime = szTime;

		if (m_rcvTrData.m_nTickIdx > 0)
		{
			beforeData = m_rcvTrData.m_arrChart.GetAt(m_rcvTrData.m_nTickIdx-1);
			if (strncmp(beforeData.date, curTickData.date, 8))
			{
				m_nReplayDayCount--;
				
				m_CurTime.m_nDD = m_nReplayDayCount;
				m_CurTime.m_nHH = m_MarketStartTime[m_nReplayDayCount].m_nHH;
				m_CurTime.m_nMM = m_MarketStartTime[m_nReplayDayCount].m_nMM;
				m_CurTime.m_nSS = m_MarketStartTime[m_nReplayDayCount].m_nSS;

				CString strBojo;
				//strBojo.Format("    DATE=%8.8s@SC=%s@", curTickData.date, m_rcvTrData.m_szStockCode);
				strBojo.Format("DATATIME=%8.8s000000@SC=%s@", curTickData.date, m_rcvTrData.m_szStockCode);

				// MultiChart�� ��¥�� ��������Ƿ� �����޼����� �����Ѵ�.
				GetParent()->SendMessage(RMSG_REPLAY_CHANGE_DATE, (WPARAM)(LPCTSTR)m_rcvTrData.m_szStockCode, (LPARAM)(LPCTSTR)strBojo);

				SendCurTime();
			}
		}

		if (szCurTime == szTime)
		{
			// ���� �ð��� �� ���۽ð��̰ų� ���� �����ϰ� ���� ���� �ð��� ������ �ѹ� ����
			if (((m_StartTime.m_nDD == 0) && 
				(m_CurTime.m_nHH == m_MarketStartTime[0].m_nHH) &&
				(m_CurTime.m_nMM == m_MarketStartTime[0].m_nMM) &&
				(m_CurTime.m_nSS == m_MarketStartTime[0].m_nSS)) ||
				((m_StartTime.m_nDD == m_CurTime.m_nDD) &&
				(m_StartTime.m_nHH == m_CurTime.m_nHH) &&
				(m_StartTime.m_nMM == m_CurTime.m_nMM) &&
				(m_StartTime.m_nSS == m_CurTime.m_nSS)))
//				|| bChangeDay == TRUE)
			{
				CString strBojo;
				//strBojo.Format("    DATE=%8.8s@SC=%s@", curTickData.date, m_rcvTrData.m_szStockCode);
				strBojo.Format("DATATIME=%8.8s000000@SC=%s@", curTickData.date, m_rcvTrData.m_szStockCode);
				// MultiChart�� ��¥�� ��������Ƿ� �����޼����� �����Ѵ�.
				GetParent()->SendMessage(RMSG_REPLAY_CHANGE_DATE, (WPARAM)(LPCTSTR)m_rcvTrData.m_szStockCode, (LPARAM)(LPCTSTR)strBojo);
				
				SendCurTime();
			}

			GetParent()->SendMessage(RMSG_REPLAY_REAL, (WPARAM)(LPCTSTR)m_rcvTrData.m_szStockCode, (LPARAM)curTickData.time);
			m_rcvTrData.m_nTickIdx--;
			break;
		}
		else
			m_rcvTrData.m_nTickIdx--;
	}

	IncreaseTime();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 2 / 7
// Return void : 
// Param  void : 
// Comments		: �ð��� ������Ų��.
//-----------------------------------------------------------------------------
void CReplayChartWnd::IncreaseTime(void)
{
	if ((m_CurTime.m_nSS += 1) >= 60)
	{
		m_CurTime.m_nSS = 0;
		if ((m_CurTime.m_nMM += 1) >= 60)
		{
			m_CurTime.m_nMM = 0;
			m_CurTime.m_nHH += 1;
		}
	}
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 2 / 7
// Return void : 
// Param  void : 
// Comments		: Multi Chart�� ���� �ð��� �����Ѵ�.
//-----------------------------------------------------------------------------
void CReplayChartWnd::SendCurTime(void)
{
	if (m_bSendCurTime == FALSE)
	{
		CString szCurTime;
		szCurTime.Format("%d���� %02d:%02d:%02d", m_CurTime.m_nDD, m_CurTime.m_nHH, m_CurTime.m_nMM, m_CurTime.m_nSS);
		GetParent()->SendMessage(RMSG_REPLAY_INC_SEC, (WPARAM)(LPTSTR)(LPCTSTR)szCurTime);
		m_bSendCurTime = TRUE;
	}
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 2 / 7
// Return void : 
// Param  void : 
// Comments		: ���� �����Ͽ� �°� TickIndex�� �̵���Ų��.
//-----------------------------------------------------------------------------
void CReplayChartWnd::MoveTickIdx(void)
{
	// 1. ���� ���� �ð��� ���ڿ��� �����.
	CString szStartTime;
	szStartTime.Format("%02d%02d%02d",m_StartTime.m_nHH, m_StartTime.m_nMM, m_StartTime.m_nSS);

	// b. ���ϴ� �������� Data�� ã���� TickData�� Index�� ã�´�.
	if (m_rcvTrData.m_nNDayBefore != m_nReplayDayCount)
		return;

	// TickData���� ���� ���� �ð��� �ش�Ǵ� TickData�� �۾Ƽ� Index�� �������ش�.
	KB_d1016_OutRec2 curTickData;
	int nSize = m_rcvTrData.m_arrChart.GetSize()-1;
	for (int i=nSize; i>=0; i--)
	{
		curTickData = m_rcvTrData.m_arrChart.GetAt(i);

		CString szDate = (LPCSTR)curTickData.time;
		szDate = szDate.Left(6);

		if (szStartTime <= szDate)
		{
			m_rcvTrData.m_nTickIdx = i;
			break;
		}
	}
}

void CReplayChartWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	KillTimer(m_unTimerHandle);
}

int CReplayChartWnd::CalcTotalTime()
{
	CTime time;
	time = CTime::GetCurrentTime();

	int nCurTime = time.GetHour()*10000 + time.GetMinute()*100 + time.GetSecond();
	int nStartTime = m_sendTime.m_nHH*10000 + m_sendTime.m_nMM*100 + m_sendTime.m_nSS;
	if (nCurTime > 153000)
		nCurTime = 153000;

	if (nStartTime < 90000)
		nStartTime = 90000;

	if (m_sendTime.m_nDD > 0)
	{
		if (nStartTime < 153000)
			m_nTotalTime = 153000 - nStartTime;
		if (nCurTime > 90000)
			m_nTotalTime += nCurTime - 90000;
	}
	else
	{
		if (nCurTime > nStartTime)
			m_nTotalTime = nCurTime - nStartTime;
	}

	return m_nTotalTime;
}

int CReplayChartWnd::CalcCurTime(int nEndTime) 
{
	int nCurTime=0, nStartTime=0;

	nStartTime = m_sendTime.m_nHH*10000 + m_sendTime.m_nMM*100 + m_sendTime.m_nSS;
	if (m_nRcvDate > 0)
	{
		if (nEndTime > nStartTime)
			nCurTime = nEndTime - nStartTime;
	}
	else
	{
		if (m_sendTime.m_nDD > 0)
		{
			if (nStartTime < 153000)
				nCurTime = 153000 - nStartTime;

			if (nEndTime > 90000)
				nCurTime += nEndTime - 90000;
		}
		else
		{
			if (nEndTime > nStartTime)
				nCurTime = nEndTime - nStartTime;
		}
	}

	return nCurTime;
}

