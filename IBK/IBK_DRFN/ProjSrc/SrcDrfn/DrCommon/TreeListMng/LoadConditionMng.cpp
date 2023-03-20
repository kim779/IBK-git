#include "stdafx.h"
#include "LoadConditionMng.h"

BOOL CCondHelper::SetTrSend_Result(POSITION pos, CItemOneClickData* pDataItem, LPCSTR szScreenNo)
{
	TR_RESULT_IN stResultIn;
	memset(&stResultIn, 0x00, SZ_TR_RESULT_IN);
	
	// stResultIn.nTrIndex = ntohl(n);
	m_TrPos[m_nTrIndex] = pos;
	//		stResultIn.nTrIndex = ntohl(m_nTrIndex);
	stResultIn.nTrIndex = m_nTrIndex;
	if(m_nTrIndex >= MAX_CCondHelper_Cnt)
		m_nTrIndex = 0;
	
	m_nCurTrIndex = m_nTrIndex;
	//		stResultIn.sFilterGubun = '0';
	
	//->>
	int nMaxSize = sizeof(KB_d1022_InRec1) + sizeof(KB_d1022_InRec2) + (sizeof(KB_d1022_InRec3)*MAX_IDXCNT);
	char* pSendData = new char[nMaxSize+1];
	int nPos = 0;
	
	KB_d1022_InRec1* pInRec1 = (KB_d1022_InRec1*)&pSendData[nPos];
	nPos += sizeof(KB_d1022_InRec1);
	
	CTranHelperd1022 TrHelper;
	TrHelper.TR_RESULT_IN_To_KB_d1022_InRec1(&stResultIn, pInRec1);
	//-<<
	
	User_Finder stUser_Finder;
	if(GetMakePacket_Condition(pDataItem, &stUser_Finder)==TRUE)
	{
		memcpy(&stResultIn.stCondition, &stUser_Finder, SZ_User_Finder);
		//->>
		// ���ο��� nPos ��ġ���� ó���Ѵ�.
		TrHelper.User_Finder_To_KB_d1022_InRec2(&stUser_Finder, &pSendData[0], nPos);
		pSendData[nPos] = 0;
		//-<<
	}
	else
	{
		m_TrPos[m_nTrIndex] = 0;
		//Error
		delete [] pSendData;
		pSendData=NULL;
		return FALSE;
	}
	
	//@��������		char* lpData = new char[SZ_TR_RESULT_IN];
	//@��������		memcpy(lpData, &stResultIn, SZ_TR_RESULT_IN);
	//@��������
	//@��������		ITrComm2* pTrComm = (ITrComm2*)pDataItem->m_pITrComm;
	//@��������		if(pTrComm)
	//@��������		{
	//@��������			pTrComm->SetTR(TRNO_CONDIONECLK_RESULT);
	//@��������			pTrComm->SetDestination('V');
	//@��������			pTrComm->ChangeCallback((ITrCommSite*)pDataItem->m_pITrcommSite);
	//@��������
	//@��������			//ȭ���ȣ�� �����Ѵ�.
	//@��������			pTrComm->SetBaseData("SCREENCODE", szScreenNo);
	//@��������			int nRet = pTrComm->Send2Server((LPCSTR)lpData, SZ_TR_RESULT_IN, FALSE);
	//@��������			pTrComm->SetDestination(' ');
	//@��������		}
	//@��������
	//@��������		if(lpData) delete[] lpData;
	
	int nRet = Modual_SendTr(TRNO_CONDIONECLK_RESULT, nPos, (BYTE*)pSendData, pDataItem, szScreenNo);
	
	if (pSendData)	delete[] pSendData;
	m_nTrIndex++;
	
	return TRUE;
}

const UINT RMSG_SENDMESSAGE = ::RegisterWindowMessage(_T("RMSG_SENDMESSAGE"));
int CCondHelper::Modual_SendTr(CString szTrNo, int nTrLen, BYTE *TrData, CItemOneClickData* pDataItem, LPCSTR szScreenNo)
{
	ITrComm2* pTrComm = (ITrComm2*)pDataItem->m_pITrComm;
	if(!pTrComm) return 0;
	
	IDataUtilManager2	*pDataUtilMng = (IDataUtilManager2*)AfxGetPctrInterface(UUID_IDataUtilManager2);
	
	//ȭ���ȣ�� �����Ѵ�.
	CString strScreenCode=szScreenNo;
	
//	CString strMsg;
	char *szSaveBuf = new char[nTrLen+200+1];
	int nPos = 0;
	
	if ((CString)TRNO_CONDIONECLK_RESULT == szTrNo)
	{
//		m_b0622Ing = TRUE;
//		strMsg = "���� ��� ��ȸ ��û";
		InMakeFid_D1022(szTrNo, nTrLen, TrData, szSaveBuf, nPos);
		

//@solomon TrComm���� �Լ� �������� ������ �ȵǼ� ���� by ����ȣ  20091008
 		pTrComm->ChangeCallback((ITrCommSite*)pDataItem->m_pITrcommSite);
 		pTrComm->SetBaseData("SCREENCODE", strScreenCode);
 		pTrComm->SetTR(szTrNo);
 		//@Solomon-->
//		pTrComm->Send2Server(szSaveBuf, nPos, TRKEY_CONDITION);

		{
			CString strTRData = "", strTemp;
			strTRData.Format("%s\t%08X\t%d\t%d\t"
								,szTrNo
								, pDataItem->m_pITrcommSite
								, TRKEY_CONDITION
								, nPos);

			int nBuffSize = strTRData.GetLength() + nPos + 4;
			char *pszBuff = new char[nBuffSize+1];
			memcpy(pszBuff, (LPTSTR)(LPCTSTR)strTRData, strTRData.GetLength());
			memcpy(pszBuff+strTRData.GetLength(), szSaveBuf, nPos);

			int nTab = strTRData.GetLength() + nPos;
			memcpy(pszBuff+nTab, "\t", sizeof("\t"));

			if (m_hWrapper)
				::SendMessage(m_hWrapper, RMSG_SENDMESSAGE, MAKEWPARAM(1, nBuffSize), (LPARAM)pszBuff);

			delete []pszBuff;
			pszBuff = NULL;
		}

		//@Solomon<--
	}
	//SetSendMainMsg(strMsg);
	
	if(szSaveBuf) delete [] szSaveBuf;
	return 1;
}

void CCondHelper::InMakeFid_D1022(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
{
	IDataUtilManager2	*pDataUtilMng = (IDataUtilManager2*)AfxGetPctrInterface(UUID_IDataUtilManager2);
	
	//1. ������Ŷ, DRFN ����Ű�� ���ʿ��ֳ�???, �׷� space(80)
//	memset(&szSaveBuf[nPos], 0x20, 80);
//	nPos += 80;
	
	//2. TR�� ���ڿ��� �����.
	// �̹� TR�����͸� �����ؼ� �����Դ�.
	//int nSendTrDataSize = InMakeTr_D1022(pTrData, (BYTE*)&inAll);
	int nSendTrDataSize = nTrLen;
	memcpy(&szSaveBuf[nPos], pTrData, nSendTrDataSize);
	nPos += nSendTrDataSize;
	
	//3. ���ڵ� ��ȣ 1���� ���ʷ� 
	CString szOutRecord;
	
	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();
	
	pDataUtilMng->GetFillString(szOutRecord, 0, 8, 3);	// 0 ~ 8
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();
	
	memcpy(&szSaveBuf[nPos], "|", 1);	//������.
	nPos++;
	
	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();
	
	pDataUtilMng->GetFillString(szOutRecord, 0, 9, 3);	// 0 ~ 9
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();
	
	memcpy(&szSaveBuf[nPos], ";", 1);	//������.
	nPos++;
}

int CCondHelper::D1022OutRec1_GetDataCount(KB_d1022_OutRec1* pOutRec)
{
	CTranHelperd1022 TrHelper;
	return TrHelper.D1022OutRec1_GetDataCount(pOutRec);
}

void CCondHelper::D1022OutRec1_SetData(KB_d1022_OutRec1* pOutRec, TR_RESULT_HEADER* pTrResult)
{
	CTranHelperd1022 TrHelper;
	TrHelper.D1022OutRec1_SetData(pOutRec, pTrResult);
}

void CCondHelper::D1022OutRec2_SetData(KB_d1022_OutRec2* pOutRec2, char* pDataBuf, int nCnt)
{
	CTranHelperd1022 TrHelper;
	TrHelper.D1022OutRec2_SetData(pOutRec2, pDataBuf, nCnt);
}
