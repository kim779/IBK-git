/******************************************************************
FileName              : STDlgConfirm.cpp
Company               : DRFN
Author                : Lee Jun Ok : alzioyes@gmail.com
Purpose               : 
Date Of Creation      : 2006-12-06 ���� 9:09:29
Modification History  : 
Date                  :
********************************************************************/

#include "stdafx.h"
#include "STDlgConfirm.h"
#include <mmsystem.h>

#include "../../inc/IDataUtilMng.h"
#include "../../inc/IMasterDataMng.h"
//#include "../chart_common/Grid/Tr_drds/D49890.h"
#include "../COMMONTR/SS_OrderDef.h"
#include "../COMMONTR/EU_s0001.h"	// �ֽ� ȣ��
#include "../COMMONTR/EU_f0201.h"	// �����ɼ� ȣ��
#include "../COMMONTR/EU_s3301.h"	// �ֽļ��� ȣ��
#include "../COMMONTR/EU_f3201.h"	// �ֽĿɼ� ȣ��
#include "../Include_ST/STOrderDef.h"

const UINT RMSG_DESTROY_CONFIRMWND = ::RegisterWindowMessage("RMSG_DESTROY_CONFIRMWND");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConfirmWnd

CConfirmWnd::CConfirmWnd()
{
	m_pITrCommManager = NULL;
	m_pITrComm = NULL;
	m_pSTDlgConfirmSendJumun = NULL;
}

CConfirmWnd::~CConfirmWnd()
{
}


BEGIN_MESSAGE_MAP(CConfirmWnd, CWnd)
	//{{AFX_MSG_MAP(CConfirmWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CConfirmWnd::OpenWindow()
{
	CString		szClassName;	
	
	szClassName =	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		0, 0, 0);
	BOOL bSuccess = CreateEx(0L,														//dwExStyle, 
					(LPCSTR)szClassName,
					"",
					WS_VISIBLE,	// | WS_CHILD, 
					0, 0, 0, 0, 
					HWND_DESKTOP, 
					NULL,
					NULL);
	if(bSuccess)
	{
		ShowWindow(SW_HIDE);

		// �Ÿű���, 00���尡, 03������.
		if(m_InData.m_nOrderPriceType==hogaIdx_market)
		{
			if(m_InData.m_nMarket==0)	memcpy(m_InData.m_jutype, "00", 2);
			else	m_InData.m_jutype[0] = '1';
		}
		else
		{
			if(m_InData.m_nMarket==0) memcpy(m_InData.m_jutype, "03", 2);
			else	m_InData.m_jutype[0] = '2';
		}

		InitTrComm();

		RequestHogaData();
	}

	return bSuccess;
}

BOOL CConfirmWnd::InitTrComm()
{
	//<<20100308_JS.Kim �ַθ�
	//m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager, 2);
	m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager);
	//>>
	m_pITrComm = (ITrComm2*)m_pITrCommManager->AdviseTrComm();
	m_pITrComm->ChangeCallback(&m_xMyITrCommSite);

	return TRUE;
}

void CConfirmWnd::RequestHogaData()
{
	if(m_InData.m_nMarket == 0)		// �ֽ�
	{
		CString strFidInfo("0                                                                               ");
		CString strTrInfo;
		strTrInfo.Format("%s20001", m_InData.m_szCode);
		CString strRecordInfo("1               000060220221001002003004005006008122126130134138124128132136140162166170174178164168172176180202204207209061062222070075010092;");
		CString strOutput;
		strOutput = strFidInfo + strTrInfo + strRecordInfo;

		stSend2Server st;
		st.m_nType = 0;
		st.m_pWnd = (CWnd*)this;
		st.m_pBuffer = (void*)(LPCTSTR)strOutput;
		st.m_lLength= strOutput.GetLength();
		st.m_strTRCode = QUERY_STOCK_HOGA;

		m_pITrComm->SetBaseData("SCREENCODE", "450100");
		m_pITrComm->SetTR(QUERY_STOCK_HOGA);
		m_pITrComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);
	}
	else if(m_InData.m_nMarket == 1)	// �����ɼ�
	{
		CString strFidInfo("0                                                                               ");
		CString strTrInfo;
		strTrInfo.Format("%s20001", m_InData.m_szCode);
		CString strRecordInfo("1               000014018019052077083089078084090;");
		CString strOutput;
		strOutput = strFidInfo + strTrInfo + strRecordInfo;

		stSend2Server st;
		st.m_nType = 0;
		st.m_pWnd = (CWnd*)this;
		st.m_pBuffer = (void*)(LPCTSTR)strOutput;
		st.m_lLength= strOutput.GetLength();
		st.m_strTRCode = QUERY_FUTOPT_HOGA;	

		m_pITrComm->SetBaseData("SCREENCODE", "450100");
		m_pITrComm->SetTR(QUERY_FUTOPT_HOGA);
		m_pITrComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);
	}

}
void CConfirmWnd::OnDestroy()
{
	if(m_pITrCommManager && m_pITrComm)
		m_pITrCommManager->UnAdviseTrComm(m_pITrComm);
	m_pITrComm = NULL;

	CWnd::OnDestroy();
}

HRESULT CConfirmWnd::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return S_OK;
}

HRESULT CConfirmWnd::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen)
{
	CWnd *pWndRequested = NULL;
	CString strTRCode = szTR;
	strTRCode.Remove(' ');

	//<<20100308_JS.Kim �ַθ�
	//IDataUtilManager* pUtilMng = (IDataUtilManager*)AfxGetPctrInterface(UUID_IDataUtilManager, 2);
	IDataUtilManager* pUtilMng = (IDataUtilManager*)AfxGetPctrInterface(UUID_IDataUtilManager);
	//>>
	if(pUtilMng == NULL)
		return 0l;

	if( (strTRCode == QUERY_STOCK_HOGA ) && aTRData != NULL)
	{
		ConvertStockHoga(aTRData, dwTRDateLen);
		CEU_s0001 eu_s0001;
		eu_s0001.Convert((EU_s0001_OutRec1*)(LPCSTR)m_strOut);

		// ���簡 ����
		m_strCurPrice = pUtilMng->GetComma(eu_s0001.m_szprice);
		
		// ���Ѱ�
		m_strUpLimitPrice = pUtilMng->GetComma(eu_s0001.m_szuplmtprice);

		// ���Ѱ�
		m_strDownLimitPrice = pUtilMng->GetComma(eu_s0001.m_szdnlmtprice);
		
		// �ŵ�3ȣ��
		m_strSell3Hoga = pUtilMng->GetComma(eu_s0001.m_szofferho3);

		// �ŵ�2ȣ��
		m_strSell2Hoga = pUtilMng->GetComma(eu_s0001.m_szofferho2);

		// �ŵ�1ȣ��
		m_strSell1Hoga = pUtilMng->GetComma(eu_s0001.m_szofferho1);

		// �ż�1ȣ��
		m_strBuy1Hoga = pUtilMng->GetComma(eu_s0001.m_szbidho1);

		// �ż�2ȣ��
		m_strBuy2Hoga = pUtilMng->GetComma(eu_s0001.m_szbidho2);

		// �ż�3ȣ��
		m_strBuy3Hoga = pUtilMng->GetComma(eu_s0001.m_szbidho3);

		SendJumun_Stock();
	}
	else if( (strTRCode == QUERY_FUTOPT_HOGA ) && aTRData != NULL)
	{
		ConvertFutOptHoga(aTRData, dwTRDateLen);
		CEU_f0201 eu_f0201;
		eu_f0201.Convert((EU_f0201_OutRec1*)(LPCSTR)m_strOut);

		// ���簡 ����
		m_strCurPrice.Format("%.2f", atof(eu_f0201.m_szprice)/100.);
		
		// ���Ѱ�
		m_strUpLimitPrice.Format("%.2f", atof(eu_f0201.m_szuplmtprice)/100.);

		// ���Ѱ�
		m_strDownLimitPrice.Format("%.2f", atof(eu_f0201.m_szdnlmtprice)/100.);
		
		// �ŵ�3ȣ��
		m_strSell3Hoga.Format("%.2f", atof(eu_f0201.m_szofferho3)/100.);

		// �ŵ�2ȣ��
		m_strSell2Hoga.Format("%.2f", atof(eu_f0201.m_szofferho2)/100.);

		// �ŵ�1ȣ��
		m_strSell1Hoga.Format("%.2f", atof(eu_f0201.m_szofferho1)/100.);

		// �ż�1ȣ��
		m_strBuy1Hoga.Format("%.2f", atof(eu_f0201.m_szbidho1)/100.);

		// �ż�2ȣ��
		m_strBuy2Hoga.Format("%.2f", atof(eu_f0201.m_szbidho2)/100.);

		// �ż�3ȣ��
		m_strBuy3Hoga.Format("%.2f", atof(eu_f0201.m_szbidho3)/100.);
		
		SendJumun_FO();
	}
	else if( (strTRCode == QUERY_STOCKFUT_HOGA ) && aTRData != NULL)
	{
		ConvertStockFutHoga(aTRData, dwTRDateLen);
		CEU_s3301 eu_s3301;
		eu_s3301.Convert((EU_s3301_OutRec1*)(LPCSTR)m_strOut);

		// ���簡 ����
		m_strCurPrice = pUtilMng->GetComma(eu_s3301.m_szprice);
		
		// ���Ѱ�
		m_strUpLimitPrice = pUtilMng->GetComma(eu_s3301.m_szuplmtprice);

		// ���Ѱ�
		m_strDownLimitPrice = pUtilMng->GetComma(eu_s3301.m_szdnlmtprice);
		
		// �ŵ�3ȣ��
		m_strSell3Hoga = pUtilMng->GetComma(eu_s3301.m_szofferho3);

		// �ŵ�2ȣ��
		m_strSell2Hoga = pUtilMng->GetComma(eu_s3301.m_szofferho2);

		// �ŵ�1ȣ��
		m_strSell1Hoga = pUtilMng->GetComma(eu_s3301.m_szofferho1);

		// �ż�1ȣ��
		m_strBuy1Hoga = pUtilMng->GetComma(eu_s3301.m_szbidho1);

		// �ż�2ȣ��
		m_strBuy2Hoga = pUtilMng->GetComma(eu_s3301.m_szbidho2);

		// �ż�3ȣ��
		m_strBuy3Hoga = pUtilMng->GetComma(eu_s3301.m_szbidho3);
		
		SendJumun_FO();
	}
	else if( (strTRCode == QUERY_STOCKOPT_HOGA ) && aTRData != NULL)
	{
		ConvertStockOptHoga(aTRData, dwTRDateLen);
		CEU_f3201 eu_f3201;
		eu_f3201.Convert((EU_f3201_OutRec1*)(LPCSTR)m_strOut);

		// ���簡 ����
		m_strCurPrice = pUtilMng->GetComma(eu_f3201.m_szprice);
		
		// ���Ѱ�
		m_strUpLimitPrice = pUtilMng->GetComma(eu_f3201.m_szhprice);

		// ���Ѱ�
		m_strDownLimitPrice = pUtilMng->GetComma(eu_f3201.m_szlprice);
		
		// �ŵ�3ȣ��
		m_strSell3Hoga = pUtilMng->GetComma(eu_f3201.m_szofferho3);

		// �ŵ�2ȣ��
		m_strSell2Hoga = pUtilMng->GetComma(eu_f3201.m_szofferho2);

		// �ŵ�1ȣ��
		m_strSell1Hoga = pUtilMng->GetComma(eu_f3201.m_szofferho1);

		// �ż�1ȣ��
		m_strBuy1Hoga = pUtilMng->GetComma(eu_f3201.m_szbidho1);

		// �ż�2ȣ��
		m_strBuy2Hoga = pUtilMng->GetComma(eu_f3201.m_szbidho2);

		// �ż�3ȣ��
		m_strBuy3Hoga = pUtilMng->GetComma(eu_f3201.m_szbidho3);
		
		SendJumun_FO();
	}

	return 0L;
}

extern long KSLink_GetEnvInfo(int nType);
#define		UM_SM_SIGNALORDER_MSG		(WM_USER+551)
int CConfirmWnd::SendJumun_Stock()
{
//<<20100308_JS.Kim �ַθ�
	/*
	HWND hSignalWnd;
	hSignalWnd = (HWND)KSLink_GetEnvInfo(2);
	if(hSignalWnd == NULL)
		return 0;
	
// ����û�� or �ż�û�� or �ŵ�û��
//	if( (m_InData.m_nOrderType & FORCEDEXIT) || (m_InData.m_nOrderType & VALEXITLONG) || (m_InData.m_nOrderType & VALEXITSHORT) )
//	{
//		if(m_InData.m_bSound_Sell_Use) sndPlaySound (m_InData.m_szSound_Sell_File, SND_ASYNC);
//	}
//	�ż� or �ŵ�
//	else if((m_InData.m_nOrderType & VALENTRYBUY) || (m_InData.m_nOrderType & VALENTRYSELL) )
//	{
//		if(m_InData.m_bSound_Buy_Use) sndPlaySound (m_InData.m_szSound_Buy_File, SND_ASYNC);
//	}
// ����û�� or �ż�û�� or �ŵ�
	if( (m_InData.m_nOrderType & FORCEDEXIT) || (m_InData.m_nOrderType & VALEXITLONG) || (m_InData.m_nOrderType & VALENTRYSELL) )
	{
		if(m_InData.m_bSound_Sell_Use) sndPlaySound (m_InData.m_szSound_Sell_File, SND_ASYNC);
	}
// �ż� or �ŵ�û��
	else if((m_InData.m_nOrderType & VALENTRYBUY) || (m_InData.m_nOrderType & VALEXITSHORT) )
	{
		if(m_InData.m_bSound_Buy_Use) sndPlaySound (m_InData.m_szSound_Buy_File, SND_ASYNC);
	}

	// �ֹ�ó��
	if(m_InData.m_nAutoOrder == 1) // ���ڵ�
	{
		SIGNALORDERSINFO signalOrdersInfo;
		memset(&signalOrdersInfo, 0, sizeof(SIGNALORDERSINFO));
		signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd();																// ȣ���� ��Ʈ�� �ڵ�
		memcpy(signalOrdersInfo.szIndexNo, "16", sizeof(signalOrdersInfo.szIndexNo)-1);								// DLL Index��ȣ : 16(�ý���Ʈ���̵�)
		memcpy(signalOrdersInfo.szWindName, "��Ʈ", sizeof(signalOrdersInfo.szWindName)-1);							// ��ȣ�̸� : "��Ʈ"
		memcpy(signalOrdersInfo.szSignalgb, "��", sizeof(signalOrdersInfo.szSignalgb)-1);							// ��ȣ���� : "��"
		memcpy(signalOrdersInfo.szPopgb, "2", sizeof(signalOrdersInfo.szPopgb)-1);									// �˾�Type : (2)�ֹ�â
		memcpy(signalOrdersInfo.szActionGb, "1", sizeof(signalOrdersInfo.szActionGb)-1);							// ��ư ó�� ���а� : (1)�ֹ�
		memcpy(signalOrdersInfo.szAcctno, (LPSTR)(LPCSTR)m_InData.m_szGeja, sizeof(signalOrdersInfo.szAcctno)-1);				// ���¹�ȣ
		memcpy(signalOrdersInfo.szCode, (LPSTR)(LPCSTR)m_InData.m_szCode, sizeof(signalOrdersInfo.szCode)-1);						// �����ڵ�
		memcpy(signalOrdersInfo.szLoangb, "0", sizeof(signalOrdersInfo.szLoangb)-1);								// �ſ뱸�� : (0)����
		memcpy(signalOrdersInfo.szJanGogb, "0", sizeof(signalOrdersInfo.szJanGogb)-1);								// �ܰ��� : (0)��ü

		CString	szPrice;
		if(m_InData.m_nOrderPriceType == 0)		// ���簡
			szPrice = m_strCurPrice;
		else if(m_InData.m_nOrderPriceType == 1)		// �ż�1ȣ��
			szPrice = m_strBuy1Hoga;
		else if(m_InData.m_nOrderPriceType == 2)		// �ż�2ȣ��
			szPrice = m_strBuy2Hoga;
		else if(m_InData.m_nOrderPriceType == 3)		// �ż�3ȣ��
			szPrice = m_strBuy3Hoga;
		else if(m_InData.m_nOrderPriceType == 4)		// �ŵ�1ȣ��
			szPrice = m_strSell1Hoga;
		else if(m_InData.m_nOrderPriceType == 5)		// �ŵ�2ȣ��
			szPrice = m_strSell2Hoga;
		else if(m_InData.m_nOrderPriceType == 6)		// �ŵ�3ȣ��
			szPrice = m_strSell3Hoga;
		else if(m_InData.m_nOrderPriceType == 7)		// ���Ѱ�
			szPrice = m_strUpLimitPrice;
		else if(m_InData.m_nOrderPriceType == 8)		// ���Ѱ�
			szPrice = m_strDownLimitPrice;

		CString strMsg("�ý���Ʈ���̵�");
//		if(m_InData.m_nOrderType & VALENTRYSELL  || m_InData.m_nOrderType & VALENTRYBUY)
		if(m_InData.m_nOrderType & VALEXITSHORT  || m_InData.m_nOrderType & VALENTRYBUY)
		{
			if(m_InData.m_nOrderType & VALENTRYBUY)
			{
				strMsg += " �ż�����";
			}
			if(m_InData.m_nOrderType & VALEXITSHORT)
			{
				if(strMsg != "�ý���Ʈ���̵�")
					strMsg += " /";

				strMsg += " �ŵ�û��";
			}
			
			memcpy(signalOrdersInfo.szMemegb, "2", sizeof(signalOrdersInfo.szMemegb)-1);							// �Ÿű��� : (2)�ż�
			if(m_InData.m_nOrderMarketEntry == 1)		// ���尡
			{
				memcpy(signalOrdersInfo.szOrdgb, "1", sizeof(signalOrdersInfo.szOrdgb)-1);							// �ֹ�����(���밡, ���尡,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// �ֹ��ܰ�����(0:������...(�ε���))
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// �ֹ��ܰ�
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "0", sizeof(signalOrdersInfo.szOrdgb)-1);							// �ֹ�����(���밡, ���尡,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// �ֹ��ܰ�����(0:������...(�ε���))
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);						// �ֹ��ܰ�
			}
		}
//		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALEXITSHORT)
		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALENTRYSELL || m_InData.m_nOrderType & FORCEDEXIT)
		{
			if(m_InData.m_nOrderType & FORCEDEXIT)
			{
				strMsg += " ����û��";
			}
			if(m_InData.m_nOrderType & VALENTRYSELL)
			{
				if(strMsg != "�ý���Ʈ���̵�")
					strMsg += " /";

				strMsg += " �ŵ�����";
			}
			if(m_InData.m_nOrderType & VALEXITLONG)
			{
				if(strMsg != "�ý���Ʈ���̵�")
					strMsg += " /";

				strMsg += " �ż�û��";
			}

			memcpy(signalOrdersInfo.szMemegb, "1", sizeof(signalOrdersInfo.szMemegb)-1);							// �Ÿű��� : (1)�ŵ�
			if(m_InData.m_nOrderMarketEntry == 1)		// ���尡
			{
				memcpy(signalOrdersInfo.szOrdgb, "1", sizeof(signalOrdersInfo.szOrdgb)-1);							// �ֹ�����(���밡, ���尡,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// �ֹ��ܰ�����(0:������...(�ε���))
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// �ֹ��ܰ�
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "0", sizeof(signalOrdersInfo.szOrdgb)-1);							// �ֹ�����(���밡, ���尡,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// �ֹ��ܰ�����(0:������...(�ε���))
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);						// �ֹ��ܰ�
			}
		}

		memcpy(signalOrdersInfo.szOrdqtygb, "0", sizeof(signalOrdersInfo.szOrdqtygb)-1);							// �ֹ���������(0:����, 1:�ݾ�, 2:�ŵ����ɼ���%)
		CString strQty;
		strQty.Format("%d", m_InData.m_nQty);
		memcpy(signalOrdersInfo.szOrdqty, (LPSTR)(LPCSTR)strQty, sizeof(signalOrdersInfo.szOrdqty)-1);				// �ֹ�����
		memcpy(signalOrdersInfo.szbContinue, "1", sizeof(signalOrdersInfo.szbContinue)-1);							// ���� ���࿩��
		memcpy(signalOrdersInfo.szMsg, strMsg, sizeof(signalOrdersInfo.szMsg)-1);									// ����MSG

		::SendMessage(hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
	}

	if(m_pSTDlgConfirmSendJumun) 
	{
		CWnd* pParentWnd = m_pSTDlgConfirmSendJumun->GetParent();
		pParentWnd->PostMessage(RMSG_DESTROY_CONFIRMWND, (WPARAM)this);
	}
	*/
//>>
	return 1;
}

//------------------------------------------------------------------------------
//		�����ֹ�
//------------------------------------------------------------------------------
int CConfirmWnd::SendJumun_FO()
{
//<<20100308_JS.Kim �ַθ�
	/*
	HWND hSignalWnd;
	hSignalWnd = (HWND)KSLink_GetEnvInfo(2);
	if(hSignalWnd == NULL)
		return 0;
	
// ����û�� or �ż�û�� or �ŵ�û��
//	if( (m_InData.m_nOrderType & FORCEDEXIT) || (m_InData.m_nOrderType & VALEXITLONG) || (m_InData.m_nOrderType & VALEXITSHORT) )
//	{
//		if(m_InData.m_bSound_Sell_Use) sndPlaySound (m_InData.m_szSound_Sell_File, SND_ASYNC);
//	}
//	�ż� or �ŵ�
//	else if((m_InData.m_nOrderType & VALENTRYBUY) || (m_InData.m_nOrderType & VALENTRYSELL) )
//	{
//		if(m_InData.m_bSound_Buy_Use) sndPlaySound (m_InData.m_szSound_Buy_File, SND_ASYNC);
//	}
// ����û�� or �ż�û�� or �ŵ�
	if( (m_InData.m_nOrderType & FORCEDEXIT) || (m_InData.m_nOrderType & VALEXITLONG) || (m_InData.m_nOrderType & VALENTRYSELL) )
	{
		if(m_InData.m_bSound_Sell_Use) sndPlaySound (m_InData.m_szSound_Sell_File, SND_ASYNC);
	}
// �ż� or �ŵ�û��
	else if((m_InData.m_nOrderType & VALENTRYBUY) || (m_InData.m_nOrderType & VALEXITSHORT) )
	{
		if(m_InData.m_bSound_Buy_Use) sndPlaySound (m_InData.m_szSound_Buy_File, SND_ASYNC);
	}

	// �ֹ�ó��
	if(m_InData.m_nAutoOrder == 1) // ���ڵ�
	{
		SIGNALORDERSINFO signalOrdersInfo;
		memset(&signalOrdersInfo, 0, sizeof(SIGNALORDERSINFO));
		signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd();																// ȣ���� ��Ʈ�� �ڵ�
		memcpy(signalOrdersInfo.szIndexNo, "16", sizeof(signalOrdersInfo.szIndexNo)-1);								// DLL Index��ȣ : 16(�ý���Ʈ���̵�)
		memcpy(signalOrdersInfo.szWindName, "��Ʈ", sizeof(signalOrdersInfo.szWindName)-1);							// ��ȣ�̸� : "��Ʈ"
		memcpy(signalOrdersInfo.szSignalgb, "��", sizeof(signalOrdersInfo.szSignalgb)-1);							// ��ȣ���� : "��"
		memcpy(signalOrdersInfo.szPopgb, "2", sizeof(signalOrdersInfo.szPopgb)-1);									// �˾�Type : (2)�ֹ�â
		memcpy(signalOrdersInfo.szActionGb, "1", sizeof(signalOrdersInfo.szActionGb)-1);							// ��ư ó�� ���а� : (1)�ֹ�
		memcpy(signalOrdersInfo.szAcctno, (LPSTR)(LPCSTR)m_InData.m_szGeja, sizeof(signalOrdersInfo.szAcctno)-1);				// ���¹�ȣ
		memcpy(signalOrdersInfo.szCode, (LPSTR)(LPCSTR)m_InData.m_szCode, sizeof(signalOrdersInfo.szCode)-1);						// �����ڵ�
		memcpy(signalOrdersInfo.szLoangb, "0", sizeof(signalOrdersInfo.szLoangb)-1);								// �ſ뱸�� : (0)����
		memcpy(signalOrdersInfo.szJanGogb, "0", sizeof(signalOrdersInfo.szJanGogb)-1);								// �ܰ��� : (0)��ü

		CString	szPrice;
		if(m_InData.m_nOrderPriceType == 0)		// ���簡
			szPrice = m_strCurPrice;
		else if(m_InData.m_nOrderPriceType == 1)		// �ż�1ȣ��
			szPrice = m_strBuy1Hoga;
		else if(m_InData.m_nOrderPriceType == 2)		// �ż�2ȣ��
			szPrice = m_strBuy2Hoga;
		else if(m_InData.m_nOrderPriceType == 3)		// �ż�3ȣ��
			szPrice = m_strBuy3Hoga;
		else if(m_InData.m_nOrderPriceType == 4)		// �ŵ�1ȣ��
			szPrice = m_strSell1Hoga;
		else if(m_InData.m_nOrderPriceType == 5)		// �ŵ�2ȣ��
			szPrice = m_strSell2Hoga;
		else if(m_InData.m_nOrderPriceType == 6)		// �ŵ�3ȣ��
			szPrice = m_strSell3Hoga;
		else if(m_InData.m_nOrderPriceType == 7)		// ���Ѱ�
			szPrice = m_strUpLimitPrice;
		else if(m_InData.m_nOrderPriceType == 8)		// ���Ѱ�
			szPrice = m_strDownLimitPrice;

		CString strMsg("�ý���Ʈ���̵�");
//		if(m_InData.m_nOrderType & VALENTRYSELL  || m_InData.m_nOrderType & VALENTRYBUY)
		if(m_InData.m_nOrderType & VALEXITSHORT  || m_InData.m_nOrderType & VALENTRYBUY)
		{
			if(m_InData.m_nOrderType & VALENTRYBUY)
			{
				strMsg += " �ż�����";
			}
			if(m_InData.m_nOrderType & VALEXITSHORT)
			{
				if(strMsg != "�ý���Ʈ���̵�")
					strMsg += " /";

				strMsg += " �ŵ�û��";
			}

			memcpy(signalOrdersInfo.szMemegb, "2", sizeof(signalOrdersInfo.szMemegb)-1);							// �Ÿű��� : (2)�ż�
			if(m_InData.m_nOrderMarketEntry == 1)		// ���尡
			{
				memcpy(signalOrdersInfo.szOrdgb, "1", sizeof(signalOrdersInfo.szOrdgb)-1);							// �ֹ�����(���밡, ���尡,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// �ֹ��ܰ�����(0:������...(�ε���))
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// �ֹ��ܰ�
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "0", sizeof(signalOrdersInfo.szOrdgb)-1);							// �ֹ�����(���밡, ���尡,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// �ֹ��ܰ�����(0:������...(�ε���))
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);						// �ֹ��ܰ�
			}
		}
//		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALEXITSHORT)
		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALENTRYSELL || m_InData.m_nOrderType & FORCEDEXIT)
		{
			if(m_InData.m_nOrderType & FORCEDEXIT)
			{
				strMsg += " ����û��";
			}
			if(m_InData.m_nOrderType & VALENTRYSELL)
			{
				if(strMsg != "�ý���Ʈ���̵�")
					strMsg += " /";

				strMsg += " �ŵ�����";
			}
			if(m_InData.m_nOrderType & VALEXITLONG)
			{
				if(strMsg != "�ý���Ʈ���̵�")
					strMsg += " /";

				strMsg += " �ż�û��";
			}

			memcpy(signalOrdersInfo.szMemegb, "1", sizeof(signalOrdersInfo.szMemegb)-1);							// �Ÿű��� : (1)�ŵ�
			if(m_InData.m_nOrderMarketEntry == 1)		// ���尡
			{
				memcpy(signalOrdersInfo.szOrdgb, "1", sizeof(signalOrdersInfo.szOrdgb)-1);							// �ֹ�����(���밡, ���尡,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// �ֹ��ܰ�����(0:������...(�ε���))
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// �ֹ��ܰ�
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "0", sizeof(signalOrdersInfo.szOrdgb)-1);							// �ֹ�����(���밡, ���尡,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// �ֹ��ܰ�����(0:������...(�ε���))
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);						// �ֹ��ܰ�
			}
		}

		memcpy(signalOrdersInfo.szOrdqtygb, "0", sizeof(signalOrdersInfo.szOrdqtygb)-1);							// �ֹ���������(0:����, 1:�ݾ�, 2:�ŵ����ɼ���%)
		CString strQty;
		strQty.Format("%d", m_InData.m_nQty);
		memcpy(signalOrdersInfo.szOrdqty, (LPSTR)(LPCSTR)strQty, sizeof(signalOrdersInfo.szOrdqty)-1);				// �ֹ�����
		memcpy(signalOrdersInfo.szbContinue, "1", sizeof(signalOrdersInfo.szbContinue)-1);							// ���� ���࿩��
		memcpy(signalOrdersInfo.szMsg, strMsg, sizeof(signalOrdersInfo.szMsg)-1);									// ����MSG

		::SendMessage(hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
	}

	if(m_pSTDlgConfirmSendJumun) 
	{
		CWnd* pParentWnd = m_pSTDlgConfirmSendJumun->GetParent();
		pParentWnd->PostMessage(RMSG_DESTROY_CONFIRMWND, (WPARAM)this);
	}
	*/
//>>
	return 1;
}

void CConfirmWnd::ConvertStockHoga(LPVOID aTRData, long dwTRDateLen)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	m_strOut = "";

	CString strOut;
	// ����Ű
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	// ���ذ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
	// ����ü�ᰡ��
	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
	// ����ü�����
	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
	// �����
	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
	// ���簡
	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
	// �����ȣ
	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
	// �����
	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
	// �����
	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
	// �����ŷ���
	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
	// �����ŷ����
	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
	// �ŵ�ȣ��1
	strOut += pStdrFid_Rec1->vvRpValueList[1][11];
	// �ŵ�ȣ��2
	strOut += pStdrFid_Rec1->vvRpValueList[1][12];
	// �ŵ�ȣ��3
	strOut += pStdrFid_Rec1->vvRpValueList[1][13];
	// �ŵ�ȣ��4
	strOut += pStdrFid_Rec1->vvRpValueList[1][14];
	// �ŵ�ȣ��5
	strOut += pStdrFid_Rec1->vvRpValueList[1][15];
	// �ŵ��ܷ�1
	strOut += pStdrFid_Rec1->vvRpValueList[1][16];
	// �ŵ��ܷ�2
	strOut += pStdrFid_Rec1->vvRpValueList[1][17];
	// �ŵ��ܷ�3
	strOut += pStdrFid_Rec1->vvRpValueList[1][18];
	// �ŵ��ܷ�4
	strOut += pStdrFid_Rec1->vvRpValueList[1][19];
	// �ŵ��ܷ�5
	strOut += pStdrFid_Rec1->vvRpValueList[1][20];
	// �ż�ȣ��1
	strOut += pStdrFid_Rec1->vvRpValueList[1][21];
	// �ż�ȣ��2
	strOut += pStdrFid_Rec1->vvRpValueList[1][22];
	// �ż�ȣ��3
	strOut += pStdrFid_Rec1->vvRpValueList[1][23];
	// �ż�ȣ��4
	strOut += pStdrFid_Rec1->vvRpValueList[1][24];
	// �ż�ȣ��5
	strOut += pStdrFid_Rec1->vvRpValueList[1][25];
	// �ż��ܷ�1
	strOut += pStdrFid_Rec1->vvRpValueList[1][26];
	// �ż��ܷ�2
	strOut += pStdrFid_Rec1->vvRpValueList[1][27];
	// �ż��ܷ�3
	strOut += pStdrFid_Rec1->vvRpValueList[1][28];
	// �ż��ܷ�4
	strOut += pStdrFid_Rec1->vvRpValueList[1][29];
	// �ż��ܷ�5
	strOut += pStdrFid_Rec1->vvRpValueList[1][30];
	// �Ѹŵ��ܷ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][31];
	// �Ѹż��ܷ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][32];
	// �ð��ܸŵ��ܷ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][33];
	// �ð��� �ż��ܷ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][34];
	// ���Ѱ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][35];
	// ���Ѱ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][36];
	// ��������
	strOut += pStdrFid_Rec1->vvRpValueList[1][37];
	// ��
	strOut += pStdrFid_Rec1->vvRpValueList[1][38];
	// ����
	strOut += pStdrFid_Rec1->vvRpValueList[1][39];
	// ���ϰŷ���
	strOut += pStdrFid_Rec1->vvRpValueList[1][40];
	// ����kospi����kosdaq �ֽļ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][41];
	//strData = "          ";
	m_strOut = strOut;
}

void CConfirmWnd::ConvertFutOptHoga(LPVOID aTRData, long dwTRDateLen)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	m_strOut = "";

	CString strOut;
	// �����ڵ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	// ���ذ���
	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
	// ���Ѱ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
	// ���Ѱ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
	// ���簡 
	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
	// �ŵ�ȣ��1
	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
	// �ŵ�ȣ��2
	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
	// �ŵ�ȣ��3
	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
	// �ż�ȣ��1
	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
	// �ż�ȣ��2
	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
	// �ż�ȣ��3
	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
	m_strOut = strOut;
}

void CConfirmWnd::ConvertStockFutHoga(LPVOID aTRData, long dwTRDateLen)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	m_strOut = "";

	CString strOut;
	// �����ڵ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	// ���ذ���
	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
	// ���Ѱ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
	// ���Ѱ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
	// ���簡 
	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
	// �ŵ�ȣ��1
	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
	// �ŵ�ȣ��2
	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
	// �ŵ�ȣ��3
	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
	// �ż�ȣ��1
	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
	// �ż�ȣ��2
	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
	// �ż�ȣ��3
	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
	m_strOut = strOut;
}

void CConfirmWnd::ConvertStockOptHoga(LPVOID aTRData, long dwTRDateLen)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	m_strOut = "";

	CString strOut;
	// �����ڵ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	// ���ذ���
	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
	// ���Ѱ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
	// ���Ѱ�
	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
	// ���簡 
	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
	// �ŵ�ȣ��1
	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
	// �ŵ�ȣ��2
	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
	// �ŵ�ȣ��3
	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
	// �ż�ȣ��1
	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
	// �ż�ȣ��2
	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
	// �ż�ȣ��3
	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
	m_strOut = strOut;
}
