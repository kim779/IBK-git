// StdDialog.cpp : implementation file
//
#include "stdafx.h"
#include "MultiChart.h"
#include "StdDialog.h"
#include "TabCodeSearch.h"

void CStdDialog::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);
}

void CStdDialog::_DoCheckMetrixScreen()
{
	//if(m_nScreenID<0367 && m_nScreenID>0371) return;
	if(m_nScreenID<367 && m_nScreenID>371) return;

	CString szCode;
	if( m_ChartMng.GetpChartItemSelected() != NULL)
		szCode = m_ChartMng.GetpChartItemSelected()->GetSelectedItemCode();

	if(!g_pPtUtilMng) return;

	m_nMatrix;
	CString szDomino;
	int nMajor = (int)g_pPtUtilMng->DoWinixVariant(m_hPlatform, majorCC, "");
	szDomino.Format("OnSaveHistory\t%d\t%d\t%s", nMajor, m_nMatrix, szCode);

	g_pPtUtilMng->DoWinixVariant(m_hPlatform, triggerCC, szDomino);

}

HRESULT CStdDialog::CPartnerSite::RecieveFidData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveTRData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::CPartnerSite::RecieveTRData: Entry\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	CString strTR(szTR);
	if (dwKey == TRKEY_WORKDATE)		strTR = TRNO_WORKDATE;
	else if (dwKey == TRKEY_STOCK_HOGA)	strTR = QUERY_STOCK_HOGA;
	else if (dwKey == TRKEY_MSTCODE)	strTR = TRNO_MSTCODE;

	pThis->ReceiveData(dwKey, strTR, szMsgCode, szMessage, aTRData, dwTRDateLen);

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealData(long dwKey, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	pThis->m_ChartMng.SetRealJPriceData((LPVOID)dwData);
	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveKeyData(int nKey, LPCSTR _szData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	switch(nKey)
	{
		case IPartnerSite::IPS_RK_shareCode:
			//��Ʈ����ȭ���� ����(���̳�)ó���� �����ʴ´�.
			if(pThis->m_bMetrixScreen) return S_OK;

			//100531-alzioyes: �����϶��� CHANGE_ITEM ��常 ó���Ѵ�.
			if(pThis->m_pInputBarDlg->GetCurrentMode() != CHANGE_ITEM)
				return S_OK;

			//��ȸ���̸� return;
			if(pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;

			//WPARAM �����ڵ�, LPARAM MarketType	
			{
				CString szData(_szData), szCode,szSymbol;
				int nPos = szData.Find('\t');
				if(nPos<0) return FALSE;
				
				szSymbol = szData.Left(nPos);
				szCode = szData.Mid(nPos+1);
				szCode.TrimRight();

				if(g_pPtUtilMng)
					if(g_pPtUtilMng->IsCodeSymbol(szSymbol)==FALSE)
						return S_OK;

				//WPARAM wp = (WPARAM)(LPCSTR)szCode;
				//LPARAM lp = pThis->m_nChartType;
				//pThis->SendMessage(UMSG_SENDSHAREDCODE, wp, lp);
				int nCodeType;
				int nMarketType = pThis->GetMarketTypeFromCode(szCode, nCodeType);
				if (pThis->m_nChartType>SINGLE_CHART && 
					(nMarketType==CMasterTypeInfo::STOCKFUTOPT_CHART || nMarketType==CMasterTypeInfo::FUOPT_JIPYO_CHART))
					return S_OK;

//{ //@alzioyes-A001
//	CString szDebug;
//	szDebug.Format("[%s]%08X]symbol=%s, code=%s\n", pThis->GetScreenNo(), this, szSymbol, szCode);
//	OutputDebugString(szDebug);
//}
				//@Solomon	�̴�������Ʈ���� ��/��,������ �����Ǵ� ������ ����.
//				if (pThis->m_nChartType == MINI_SINGLE2_CHART && nCodeType != CMasterTypeInfo::STOCK_CHART)
				//0006516: [7117] �̴���Ʈ - �ڽ��� ������ ��������� �ȵǴ� ���� ����.
				if (pThis->m_nChartType == MINI_SINGLE2_CHART && nMarketType != CMasterTypeInfo::STOCK_CHART)
					return S_OK;

				if( pThis->IsOpenLoaded() && pThis->m_nMarketType == nMarketType )
				{
					//@Solomon	���������� ��� ���� ���� ����.	-->
					if (pThis->m_pInputBarDlg && pThis->m_pInputBarDlg->m_pFOHisBtn)
					{
						nCodeType = pThis->m_pInputBarDlg->m_pFOHisBtn->GetCodeType(szCode);
						switch (nCodeType)
						{
							case futureType:	// ����
//KHD							case spreadType:	// ��������
							case callType:		// �ݿɼ�
							case putType:		// ǲ�ɼ�
							case sfutureType:	// �ֽļ���
							case koptionType:	// �ֽĿɼ�
								break;
							default:
								return S_OK;
						}
						//@Solomon	���������� ��� ���� ���� ����.	<--
						pThis->DoNotifyChangeCode(FALSE);
						pThis->ChangeCode(szCode);
						pThis->DoNotifyChangeCode(TRUE);
					}
					else
					{
						pThis->ChangeCode(szCode);

						//@alzioyes-A001 
						pThis->DoShareCode(szCode);						
						if(g_pPtUtilMng)
							g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
					}
				}
			}
			break;
		case IPartnerSite::IPS_RK_codeDlg: //����Dialog���� ������ Code
			//��ȸ���̸� return;
			if(pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;

			{
				//CString szSymbol;
				//g_pPtUtilMng->GetSymboCode(_szData, szSymbol);

				CString szCode(_szData);
				int nPos = szCode.Find('\t');
				if(nPos>0) szCode = szCode.Mid(nPos);

				szCode.TrimLeft();szCode.TrimRight();
				//������ѱ�� ó�����Ѵ�.
				if(szCode.GetLength()==0) return S_OK;

				//WPARAM wp = (WPARAM)(LPCSTR)szCode;
				//LPARAM lp = pThis->m_nChartType;
				//pThis->SendMessage(UMSG_SENDSHAREDCODE, wp, lp);
				int nCodeType;
				int nMarketType = pThis->GetMarketTypeFromCode(szCode, nCodeType);

				if (pThis->m_nChartType == SINGLE_CHART &&
					pThis->m_nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART &&
					nMarketType == CMasterTypeInfo::FUTOPT_CHART)
				{
					pThis->ChangeStockMode(CMasterTypeInfo::FUTOPT_CHART);
				}

				if(pThis->m_nMarketType == nMarketType)
				{
					//@Solomon	���������� ��� ���� ���� ����.	-->
					if (pThis->m_pInputBarDlg && pThis->m_pInputBarDlg->m_pFOHisBtn)
					{
						nCodeType = pThis->m_pInputBarDlg->m_pFOHisBtn->GetCodeType(szCode);
						switch (nCodeType)
						{
						case futureType:	// ����
//KHD						case spreadType:	// ��������
						case callType:		// �ݿɼ�
						case putType:		// ǲ�ɼ�
						case sfutureType:	// �ֽļ���
						case koptionType:	// �ֽĿɼ�
							return S_OK;
						}
					}
					//@Solomon	���������� ��� ���� ���� ����.	<--
					if(pThis->m_pInputBarDlg->GetCurrentCode() != szCode)
						pThis->ChangeCode(szCode);
					if(g_pPtUtilMng) g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
				}
			}
			break;
		case IPartnerSite::IPS_RK_codeHidDlg:
			//��ȸ���̸� return;
			if(pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;

			{
				CString szCode(_szData);

//				int nMarketType = CMasterTypeInfo::FUTOPT_CHART;
				int nCodeType;
				int nMarketType = pThis->GetMarketTypeFromCode(szCode, nCodeType);
				if(pThis->m_nMarketType == nMarketType)
//				if (nMarketType == CMasterTypeInfo::FUTOPT_CHART || nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
				{
					pThis->ChangeCode(szCode);
					if(g_pPtUtilMng) g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
				}
			}
			break;
		case IPartnerSite::IPS_RK_gwansim:
			{
				//���ɱ׷��� ����Ǿ��ٴ� �޽����� �޾Ƽ� ���ɱ׷� ������ ��û��.
				if(pThis->m_pGwanMng)
					pThis->m_pGwanMng->RequestGwansimGroup();
			}		
			break;

		case IPartnerSite::IPS_RK_SaveKeyEnd:
			{
				CString szSaveKey1(_szData);
				CString szOldSaveKey;
				if(szSaveKey1.Find("USERSCREEN")==0)				
				{
					szOldSaveKey = pThis->GetSaveKey();
					pThis->SetSaveKey(_szData);

					pThis->SaveShowInfo();
					pThis->m_ChartMng.SaveChart(pThis->m_strConfig);
					if(pThis->m_pInputBarDlg)
 						pThis->m_pInputBarDlg->SaveValue();
					if(pThis->m_pExtInputBarDlg)
						pThis->m_pExtInputBarDlg->SaveExtInputBar();

					pThis->SetSaveKey(szOldSaveKey);
				}
				else
				{
					pThis->SetSaveKey(_szData);
				}
			}
			break;
	}

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealDataEx(long dwKey, LPCTSTR pszCode, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)
		
	pThis->m_ChartMng.ReceiveRealHogaData(dwKey, pszCode, dwData);

	return S_OK;
}

#define		DLL_Check_AdviseCode		0x80
HRESULT CStdDialog::CPartnerSite::ReceiveOnUser(WPARAM wParam, LPARAM lParam)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)
		
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch (bt)
	{
	case DLL_DOMINO:		// ȭ�� �����鼭 ������ ���� ���� ��
		{
			pThis->m_mapDominoCode.RemoveAll();
			
			CString szData((char*)lParam), strTemp, strTemp2, strKey, strValue;
			CMapStringToString strMap;
			strTemp = szData;
			strTemp += _T("\n");

			int nFind = strTemp.Find(_T("\n"));
			while (nFind>=0)
			{
				strTemp2 = strTemp.Left(nFind);
				strTemp = strTemp.Mid(nFind+1);
				nFind = strTemp2.Find(_T("\t"));
				if (nFind >= 0)
				{
					strKey = strTemp2.Left(nFind);
					strValue = strTemp2.Mid(nFind+1);
					strMap.SetAt(strKey, strValue);
					
					if (strKey[0]>='1' && strKey[0]<='9')
					{
						if (strKey.Find(_T("301")) > 0)
						{
							pThis->m_mapDominoCode.SetAt(strKey, strValue);
							RecieveKeyData(0, strTemp2);
						}
					}
					else if(strKey.CompareNoCase("gicode") == 0)
					{
						pThis->m_mapDominoCode.SetAt(strKey, strValue);
						RecieveKeyData(0, strTemp2);
					}
					else if (strKey.CompareNoCase(_T("OnLoadHistory")) == 0)
					{
						pThis->m_bMetrixScreen = TRUE;
						//pThis->m_nMatrix = atoi(strValue);
						nFind = strValue.Find(_T("\t"));
						CString sLHIndex, sLHValue;

						if(nFind<0) sLHIndex = strValue;
						else sLHIndex = strValue.Left(nFind);
						pThis->m_nMatrix = atoi(sLHIndex);

						sLHValue = strValue.Mid(nFind+1);
						sLHValue.TrimLeft(); sLHValue.TrimRight();
						if(sLHValue.GetLength()>0)
						{
							CString szInitKey;
							szInitKey.Format("OnLoadHistory_Init_%d", pThis->m_nMatrix);
							pThis->m_mapDominoCode.SetAt(szInitKey, sLHValue);
						}
					}
				}

				nFind = strTemp.Find(_T("\n"));
			}
		}		
		break;
		//Code Cehck.
		//���� Code������ �ʿ��� ��� ���⼭ �����Ѵ�.
		case DLL_Check_AdviseCode:
			break;
	}

	return S_OK;
}

LRESULT CStdDialog::OnRmsgDRUser(WPARAM wp, LPARAM lp)
{
	BYTE bt = LOBYTE(LOWORD(wp));
	BYTE ht = HIBYTE(LOWORD(wp));
	
	switch(bt)
	{
	case drbt_REGWINDOW:
		{
			CString szTmp = (LPCSTR)lp;
			int nPos = szTmp.Find('\t');
			if(nPos<0) return 0L;
			
			CString szName = szTmp.Left(nPos);
			szTmp = szTmp.Mid(nPos+1);
			
			if(szName.CompareNoCase("TabCodeSearch")==0)
				sscanf((LPCSTR)szTmp, "%08X", &m_hTabCodeSearch);
		}
		return 0L;

	case drbt_GETINPUTBARMARKET:
		return m_pInputBarDlg->GetComboMarket();
	default:
		break;
	}
	
	return 0L;
}

