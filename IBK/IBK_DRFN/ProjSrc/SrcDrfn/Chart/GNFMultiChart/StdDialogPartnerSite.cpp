// StdDialog.cpp : implementation file
//
#include "stdafx.h"
#include "MultiChart.h"
#include "StdDialog.h"
#include "TabCodeSearch.h"
#include "../../inc/RealUpdate.h"

#include "../../../SrcSite/include/axisfire.h"
// ��Ʈ �ֹ� ���� - onlyojt (20091204)
#include "../CommonTR/SM_CheJanStruct.h"

void CStdDialog::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);

	if (m_hWinixWnd == NULL)
	{
		IGateManager* pGateMng;
		AfxGetIGateManager(pGateMng);
		if (pGateMng == NULL)
			return;

		IPartnerSvrManager* pMng = (IPartnerSvrManager*)pGateMng->GetInterface(UUID_IPartnerSvrManager);
		if (pMng)
			pMng->GetParentHandle(0, m_hWinixWnd);
	}
}

int CStdDialog::GetMarketTypeFromCode(CString strItemCode, int& nCodeType)
{
	int nRetType = CMasterTypeInfo::NONE_CHART;
	nCodeType = -1;
	if (strItemCode.IsEmpty())
		return -1;

	CString szChkCode;
	szChkCode = strItemCode.Right(2);
	if(szChkCode.Compare(".1")==0)
	{
		nCodeType = gfutureType;
		nRetType = CMasterTypeInfo::STOCK_CHART;
	}
	else
	{
		if (m_hWinixWnd)
		{
			nCodeType = (int)::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strItemCode);
			if (nCodeType == gfutureType)
				nRetType = CMasterTypeInfo::STOCK_CHART;
		}
	}

	return nRetType;
}

HRESULT CStdDialog::CPartnerSite::RecieveFidData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveTRData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	CString strTR(szTR);
//	if (dwKey == TRKEY_WORKDATE)		strTR = TRNO_WORKDATE;
//	else if (dwKey == TRKEY_STOCK_HOGA)	strTR = QUERY_STOCK_HOGA;
	
	pThis->ReceiveData(dwKey, strTR, szMsgCode, szMessage, aTRData, dwTRDateLen);

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealData(long dwKey, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	pThis->m_ChartMng.SetRealJPriceData((LPVOID)dwData, STOCK_SISE_PID);
	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveKeyData(int nKey, LPCSTR _szData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	switch(nKey)
	{
		case IPartnerSite::IPS_RK_shareCode:
			//100531-alzioyes: �����϶��� CHANGE_ITEM ��常 ó���Ѵ�.
			if(pThis->m_pInputBarDlg->GetCurrentMode() != CHANGE_ITEM)
				return S_OK;

			//��ȸ���̸� return;
			if (pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;
			else			//WPARAM �����ڵ�, LPARAM MarketType	
			{
				CString szData(_szData), szCode,szSymbol;
				int nPos = szData.Find('\t');
				if(nPos<0) return FALSE;

				szSymbol = szData.Left(nPos);
				szCode = szData.Mid(nPos+1);
				szCode.TrimRight();

				int nCodeType;
				pThis->GetMarketTypeFromCode(szCode, nCodeType);
				if (nCodeType == gfutureType)
				{
//					pThis->DoNotifyChangeCode(FALSE);
					pThis->ChangeCode(szCode);
//					pThis->DoNotifyChangeCode(TRUE);
				}
			}
			break;
		case IPartnerSite::IPS_RK_codeDlg: //����Dialog���� ������ Code
			if (pThis->m_pInputBarDlg->IsWindowEnabled())
			{
				CString szCode(_szData);
				int nPos = szCode.Find('\t');
				if (nPos > 0)
					szCode = szCode.Left(nPos);

				int nCodeType;
				pThis->GetMarketTypeFromCode(szCode, nCodeType);
				if (nCodeType == gfutureType)
				{
					pThis->ChangeCode(szCode);
					if (g_pPtUtilMng)
						g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
				}
			}
			break;

		case IPartnerSite::IPS_RK_codeHidDlg:
			break;
		case IPartnerSite::IPS_RK_gwansim:
			break;
		case IPartnerSite::IPS_RK_SaveKeyEnd:	//��������� �� ������ ���� ����ó��.
			break;
	}

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealDataEx(long dwKey, LPCTSTR pszCode, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::ReceiveOnUser(WPARAM wParam, LPARAM lParam)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

//<<20100412_JS.Kim FX. ü�ᵥ��Ÿ �ǽð� ����
HRESULT CStdDialog::CPartnerSite::ReceiveNoticeData(LPCTSTR pszCode, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	pThis->ReceiveNoticeData(pszCode, dwData);

	return S_OK;
}

void CStdDialog::ReceiveNoticeData(LPCTSTR pszCode, LONG dwData)
{
	FX_CHE_STRUCT* pOrderInfomation = (FX_CHE_STRUCT*)dwData;
	if(!pOrderInfomation)
		return;
			
	CString strCodeInputCode = m_pInputBarDlg->m_pCodeInput->GetCode();

	if(strCodeInputCode.IsEmpty())
		return;

	CString strAccount = "", strAccName = "", strAccPW = "";
	if( m_pExtInputBarDlg )	m_pExtInputBarDlg->GetAccountInfo(strAccount, strAccName, strAccPW);
			
	CString strItemCode (pOrderInfomation->SHCODE,	sizeof(pOrderInfomation->SHCODE));
	CString strAccNo (pOrderInfomation->szAccNo,	sizeof(pOrderInfomation->szAccNo));

	strItemCode.TrimRight ();
	strCodeInputCode.TrimRight ();
	strAccount.TrimRight ();
	strAccNo.TrimRight ();
			
	if (strCodeInputCode == strItemCode && strAccount == strAccNo)
	{
		CString strKeyType(pOrderInfomation->szKeyType, sizeof(pOrderInfomation->szKeyType));

		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		if(pChartItem)
		{
			if(strKeyType == "CH01" || strKeyType == "CLSL")		// ü��
			{
				pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 3, (long)pOrderInfomation);
			}
		}
	}
}
//>>