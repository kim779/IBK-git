// DeskHelper.cpp : implementation file
//

#include "stdafx.h"
#include "./PcTrMng.h"
#include "TrComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../../Chart/commontr/TRDefine.h"

#define		TID_WAITCHK				103
#define		CHECKSTATE(xxCallback, xxState)	if(xxCallback)	xxCallback->StatusChange(xxState);

#define AS_ONLY_TR			0x00		// FID ����ΰ��

/////////////////////////////////////////////////////////////////////////////
// CTRComm
BEGIN_MESSAGE_MAP(CTRComm, CWnd)
	//{{AFX_MSG_MAP(CTRComm)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTRComm::OnDestroy() 
{
	StopTRTimer();
	CWnd::OnDestroy();
}

int CTRComm::_DoPreCheck(LPBYTE szData, long nLen, int nKey, BOOL &bChk)
{
	if(!m_pPartnerSite) return 0;

	CString szInputKey;
	if (nKey > TRKEY_TR_END)
	{
		switch(nKey)
		{
			case TRKEY_STRDOLLAR: szInputKey=QUERY_STRDOLLAR; break;
			default: return 0;
		}
	}
	else
	{
		szInputKey = m_szTR;
	}

	bChk = FALSE;
	if(strcmp(szInputKey, QUERY_STRDOLLAR )==0)
		return _DoPreCheck_Dollar(szData, nLen, szInputKey, bChk);
	else if(strcmp(szInputKey, TRNO_WORKDATE )==0)
		return _DoPreCheck_WorkDate(szData, nLen, szInputKey, bChk);
	else if(strcmp(szInputKey, TRNO_MSTCODE)==0)
		return _DoPreCheck_MstCode(szData, nLen, szInputKey, bChk);

	return 0;
}

int CTRComm::_DoPreCheck_WorkDate(LPBYTE szData, long nLen, LPCSTR szInfo, BOOL &bChk)
{
	CString rValue;
	theApp.m_MainExinfoMng.GetUserStringData(gszDataKey_WorkDate, rValue);
	if (rValue.GetLength()>0)
	{
		bChk = TRUE;
		
		LPBYTE pDataBuf = (LPBYTE)(LPCSTR)rValue;
		long nDataLen = (long)rValue.GetLength();

		m_pPartnerSite->RecieveTRData(0,
				szInfo,
				"",		//szMsgCode,
				"",		//szMsg,
				pDataBuf, //pBodyData,
				nDataLen); //nBodyLen);
		return 1;
	}
	return 0;
}

int CTRComm::_DoPreCheck_Dollar(LPBYTE szData, long nLen, LPCSTR szInfo, BOOL &bChk)
{
	CString rValue;
	theApp.m_MainExinfoMng.GetUserStringData(gszDataKey_USDollar, rValue);
	if (rValue.GetLength()>0)
	{
		bChk = TRUE;
		
		LPBYTE pDataBuf = (LPBYTE)(LPCSTR)rValue;
		long nDataLen = (long)rValue.GetLength();
		
		m_pPartnerSite->RecieveTRData(0,
				szInfo,
				"",		//szMsgCode,
				"",		//szMsg,
				pDataBuf, //pBodyData,
				nDataLen); //nBodyLen);
		return 1;
	}
	return 0;
}

int CTRComm::_DoPreCheck_MstCode(LPBYTE szData, long nLen, LPCSTR szInfo, BOOL &bChk)
{

	CString szDataFile;
//	szDataFile.Format("%s\\icss\\%s\\%s", theApp.m_MainInfoMng.GetDataDir(), gszCacheDir, "MstCode.dat");
	szDataFile.Format("%s\\FindData\\%s\\%s", theApp.m_MainInfoMng.GetDataDir(), gszCacheDir, "MstCode.dat");

	CFileFind find;
	if(find.FindFile(szDataFile)==FALSE)
		return 0;

	CFile file;
	if(file.Open(szDataFile, CFile::modeRead|CFile::typeBinary)==TRUE)
	{
		long nDataLen = file.GetLength();
		if(nDataLen>0)
		{
			CString rValue;
			rValue.GetBufferSetLength(nDataLen);
			file.Read((char*)(LPCSTR)rValue, nDataLen);
			file.Close();			
			rValue.ReleaseBuffer();
			
			bChk = TRUE;
			LPBYTE pDataBuf = (LPBYTE)(LPCSTR)rValue;
			
			m_pPartnerSite->RecieveTRData(0,
					szInfo,
					"",		//szMsgCode,
					"",		//szMsg,
					pDataBuf, //pBodyData,
					nDataLen); //nBodyLen);


			return 1;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// * �Ķ���� �߰�(2002/05/09, ������)
// * bWait : ���� tr send�ÿ� ��ٸ��� �ʰ� �ٷ� ����
#include "../../inc/IAUPartnerMng.h"		//@090922 For IPartnerSite
BOOL CTRComm::Send2Server(void* szData, int nLen, int nKey)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CTRComm::Send2Server: Entry\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	BOOL bChk = FALSE;
	int nRet = _DoPreCheck((LPBYTE)szData, nLen, nKey, bChk);
	if(nRet!=0 && bChk>0) return nRet;

	//�����͸� ������ ��ƾ�� IPartnerManager�� ���� �Ѱ��ش�.
	Send2ServerWithKey(szData, nLen, nKey);

	return TRUE;
}

// ���� ������ �Ѱ��ش�.
// TR�ڵ��(5)
// �޽����ڵ�(6)
// �޽���(82)
// �����϶� Ŀ����ġ(2)
// �����޽��� (�����޽�������)
// TR����Ÿ
// TR����Ÿ����
BOOL CTRComm::NotifyParent(
				LPCSTR szTRName,
				LPCSTR szMsgCode,
				LPCSTR szMessage,
				LPVOID	aTRData,
				long	dwTRDateLen )
{
//	m_sect.Lock();
	HRESULT hr = 0;
	if(m_pCallback)
		hr = m_pCallback->ReceiveData((long)m_hWnd,
										szTRName,
										szMsgCode,
										szMessage,
										aTRData,
										dwTRDateLen);
//	m_sect.Unlock();

	if(SUCCEEDED(hr)) return TRUE;
	else return FALSE;
}


void CTRComm::OnTimer(UINT nIDEvent) 
{
	StopTRTimer();

	CHECKSTATE(m_pCallback, ICBS_TIMEOUT);
	CWnd::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
// ������            : ���ؿ�(JunOk, Lee)
// Email             : alzio@dooriic.co.kr
// ���� ��¥         : 2001/8/17
// �Լ� �̸�         : CTRComm::StartTRTimer
// ���� Ÿ��         : void
// �Ķ���� ����     : 
// �Լ� ����         : TR Ÿ�̸� ����
///////////////////////////////////////////////////////////////////////////////
void CTRComm::StartTRTimer()
{
	if(m_nTRTimer>0) KillTimer(m_nTRTimer);
	m_nTRTimer = SetTimer(TID_WAITCHK, m_nTRTimeout, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// ������            : ���ؿ�(JunOk, Lee)
// Email             : alzio@dooriic.co.kr
// ���� ��¥         : 2001/8/17
// �Լ� �̸�         : CTRComm::StopTRTimer
// ���� Ÿ��         : void
// �Ķ���� ����     : 
// �Լ� ����         : TR Ÿ�̸� ��
///////////////////////////////////////////////////////////////////////////////
void CTRComm::StopTRTimer()
{
	if(m_nTRTimer>0) KillTimer(m_nTRTimer);
	m_nTRTimer = -1;
}

BOOL CTRComm::Create(CWnd* pParentWnd)
{
	if(!pParentWnd) 
		pParentWnd = theApp.PctrGetMainWnd();  //AfxGetMainWnd();
	else
		m_pParent = pParentWnd;

	// Get the class name and create the window
	CString szClassName = AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS, 
		LoadCursor(NULL, IDC_ARROW));
	
	if (!CWnd::CreateEx(WS_EX_TOPMOST, szClassName, _T(""), 
		WS_POPUP, 
		0, 0, 0, 0, // size & position updated when needed
		NULL, 0, NULL))
	{
		return FALSE;
	}

	return TRUE;
}

int CTRComm::WaitResult()
{

	DWORD dwStartTime = GetCurrentTime();	
	DWORD dwTimeOut = 1000*30;	// TimeOut : 30��
	MSG		msg;
	while( (GetCurrentTime() - dwStartTime) < dwTimeOut )
	{
		if(m_nWaitResult>0) {
			return m_nWaitResult;
		}

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE )) 
		{
			if (!m_hWnd || msg.message == WM_QUIT || msg.message == WM_CLOSE) 
			{
    			PostQuitMessage(msg.wParam) ;
				m_nWaitResult = WRET_QUIT;
				return m_nWaitResult;
       		}

   			TranslateMessage( &msg );
   			DispatchMessage( &msg );
	    }
	}
	m_nWaitResult = WRET_TIMEOUT;			// �ð��ʰ� 
	return m_nWaitResult;
}

void CTRComm::SetTR(LPCSTR szTR, BOOL bCert /*= FALSE*/)
{
	m_szTR = szTR;
	m_bCert = bCert;

	char aTR[3]={0}; memcpy(aTR, szTR, 2);
	if(strncmp(aTR, "TR", 2)==0)
	{
		m_szTR = m_szTR.Mid(2);
		m_nTRType = RQ_TR;
	}
	else
	{
		m_nTRType = RQ_FID;
	}
}

void CTRComm::SetBaseData(LPCSTR szKey, LPCSTR szValue)
{
	if(strcmp(szKey, "PtManager")==0)
	{
		sscanf(szValue, "%08X", &m_pPtManager);
	}
	m_DataTable.SetAt(szKey, szValue);
}

void CTRComm::GetBaseData(LPCSTR szKey, CString &rValue)
{
	rValue.Empty();
	m_DataTable.Lookup(szKey, rValue);
}

void CTRComm::SetBaseDataLong(LPCSTR szKey, LONG dwValue)
{
	if(strcmp(szKey, "IPartnerSite")==0)
	{
		m_pPartnerSite = (IPartnerSite*)dwValue;
	}
}
//*********************************************************************
// ��    ��    �� : OnGetData
// ��          �� : ����Ÿ ���� ó��
// ��          �� : WPARAM wParam, LPARAM lParam
// ��          �� : long 
// ��    ��    �� : (��)��˿����� ���ؿ�
// ��  ��  ��  �� : 
//*********************************************************************
// #define QUERY_STRDOLLAR			"d1018"
// #define QUERY_STRMARKET			"d1015"

//@Solomon090819extern void KSLink_SetSpeedTestEnd(LARGE_INTEGER end);
//int gDDDCnt=0;
int gOffSize = 10;

long CTRComm::Send_MessageData(LPCSTR szTRCode, LPCSTR szMsgCode, LPCSTR szMsg)
{
	m_pCallback->ReceiveMessage((long)m_hWnd,
								szTRCode,
								szMsgCode,
								szMsg);
	return 0;
}

long CTRComm::Send_PacketData(BYTE* pPackData, int nPacketSize, LPCSTR szTRCode, LPCSTR szMsgCode, LPCSTR szMsg)
{
	return 0L;
}


long CTRComm::OnGetMsg(WPARAM wParam, LPARAM lParam )
{
	return 0L;
}

int gUniqueID = 1;
int CTRComm::GetPacketUniqueID(BOOL bInCrease)
{
	if(bInCrease)
	{
		gUniqueID++;
		if(gUniqueID==9) gUniqueID++;
		if(gUniqueID>50) gUniqueID=1;
	}
	return gUniqueID;
}

// �ַθ�� ����Լ�(������ Ű���� �ѱ��, ���� �� Ű������ ����)
BOOL CTRComm::Send2ServerWithKey(void* szData, int nLen, int nKey)
{
// 	CString szDebug;
// 	szDebug.Format("[DRCS]CTRComm::Send2ServerWithKey");
// 	OutputDebugString(szDebug);


	ASSERT(m_pPtManager != NULL);
	IPartnerManager* pPtManager = (IPartnerManager*)m_pPtManager;

	if (pPtManager == NULL)
		return 0;

	LPBYTE pRBuff = new BYTE[nLen+1];
	memcpy(pRBuff, szData, nLen);
	pRBuff[nLen] = 0;

	BOOL bRet;
	if (nKey > TRKEY_TR_END)
		bRet = pPtManager->SendFidData(m_szTR, (LPBYTE)szData, nLen, nKey);
	else
		bRet = pPtManager->SendTrData(m_szTR, (LPBYTE)szData, nLen, nKey);

	delete [] pRBuff;

	return bRet;
}


