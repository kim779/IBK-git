// ChartItemFX.cpp: implementation of the CChartItemFX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szRowFieldFX_S[]	= { "112302", "112034", "112025", "112129", "112130", "112131", NULL };
char* szRowFieldFX_B[]	= { "112302", "112034", "112026", "112132", "112133", "112134", NULL };
//char* szMinRowFieldFX_B[]	= { "112302", "112034", "112025", "112026", "112129", "112130", "112131", "112132", "112133", "112134", NULL };
//char* szDayRowFieldFX[]	= { "112302", "112034", "112025", "112026", "112129", "112130", "112131", "112132", "112133", "112134", NULL };
//char* szWeekRowFieldFX[]	= { "112302", "112034", "112025", "112026", "112129", "112130", "112131", "112132", "112133", "112134", NULL };
/* ������
//					      �����ڵ�,   �̸�, �ŵ�ȣ��, �ż�ȣ��, �ŵ���������, �ż���������, �ŵ����ϴ��, �ż����ϴ��,
char* szFXGridSymbol[] = { "80301", "80022", "81025", "81026",  "80373",      "80374",      "81136",      "81137",
//							�ŵ��ð�, �ŵ���, �ŵ�����
							"81129",  "81130", "81131",
//							�ż��ð�, �ż���, �ż�����
							"81132",  "81133", "81134",
							NULL};
*/
CChartItemFX::CChartItemFX()
{
	m_bSet = FALSE;
	m_nReqFldCnt = 0;
	m_nFldCnt = 0;//m_nTickCnt = 0;	m_nSecCnt = 0;	m_nMinCnt = 0;	m_nDayCnt = 0;	m_nWeekCnt = 0;	m_nGridCnt=0;
	while (szRowFieldFX_S[m_nFldCnt] != NULL)		m_nFldCnt++;
}

CChartItemFX::~CChartItemFX()
{
}

void CChartItemFX::SetInData(SM_fx112_InRec1* pData)
{
	try
	{
		//if(gDebugMng) gDebugMng->TraceTran('I', "SetInit", sizeof(SM_fx112_InRec1), (LPBYTE)pData);
		if(g_pDRDebuger) g_pDRDebuger->DRLog_RequestData("SetInit", (LPBYTE)pData, sizeof(SM_fx112_InRec1));
		memcpy(&m_InData, pData, sizeof(SM_fx112_InRec1));
		m_bSet = TRUE;
	}
	catch (CException* e)
	{
		char aBuff[MAX_PATH]={0,};
		e->GetErrorMessage(aBuff, MAX_PATH);
	}
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemFX::Convert()
{
	if(!m_bSet) return "";

	CString strData, strCode, strTemp, strImgRgb;
	int i, nCount;

	strTemp = CString(m_InData.dcnt, sizeof(m_InData.dcnt));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.jmcode, sizeof(m_InData.jmcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.nsel, sizeof(m_InData.nsel));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.dgubun, sizeof(m_InData.dgubun)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	int nType = atoi(strTemp);

	strTemp = CString(m_InData.mgubun, sizeof(m_InData.mgubun)); // 0:�ŵ�,1:�ż�
	int nMType = atoi(strTemp);

	strImgRgb = _T("1");//CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));


	//////////////////////////////////////////////////////////////////////////
	//	Inbound Data

	//	�����ڵ� Input �ɺ�
	strData += "80301";	strData += sDS;	strData += strCode;	strData += sFS;

	switch (nType)
	{
	case 0:		// ƽ
		// �������� Input �ɺ�
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += "112306";	strData += sDS;	strData += strTemp;	strData += sFS;

	case 1:		// ��
		/*
		// �������� Input �ɺ�
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += "112306";	strData += sDS;	strData += strTemp;	strData += sFS;

		// ���ϸ���ȸ Input �ɺ�
		strTemp = _T("0");
		strData += "112307";	strData += sDS;	strData += strTemp;	strData += sFS;
		*/

		// GAP Input �ɺ�
		strTemp.Format("%d", nUnit);
		strData += "112308";	strData += sDS;	strData += strTemp;	strData += sFS;
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
		// �������� Input �ɺ�, //   <<!>> ����¥�� ����
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += "112306";	strData += sDS;	strData += strTemp;	strData += sFS;

		// ��(1)��(2)��(3)���� Input �ɺ�
		strTemp = (nType==2) ? "1":(nType==3 ? "2":"3");
		strData += "112308";	strData += sDS;	strData += strTemp;	strData += sFS;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	//	OutBound Data

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� 
/*
	for (i=0; i<m_nGridCnt; i++)
	{
		strData += szFXGridSymbol[i];
		strData += sFS;
	}
*/
	strData += "80301";	strData += sFS;	strData += "80312";	strData += sFS;	// �����ڵ�,	�Ҽ����ڸ� ( 5�ڸ�:100000, 3�ڸ�:1000)
	strData += "80022";	strData += sFS;										// �̸�

	strData += "80325";	strData += sFS;								// ����۽ð�(KST) : HHMM
	strData += "80326";	strData += sFS;								// ������ð�(KST) : HHMM

	if( nMType == 0 || nMType == 2 )		// �ŵ�
	{
		strData += "81025";	strData += sFS;		// �ŵ�ȣ��
		//strData += "80373";	strData += sFS;		// �ŵ���������
		strData += "81136";	strData += sFS;		// �ŵ����ϴ��
		strData += "81129";	strData += sFS;		// �ŵ��ð�
		strData += "81130";	strData += sFS;		// �ŵ���
		strData += "81131";	strData += sFS;		// �ŵ�����

		strData += "80378";	strData += sFS;		// �ŵ����Ͻð�
		strData += "80379";	strData += sFS;		// �ŵ����ϰ�
		strData += "80380";	strData += sFS;		// �ŵ���������
		strData += "80381";	strData += sFS;		// �ŵ���������
	}
	if( nMType == 1 || nMType == 2 )		// �ż�
	{
		strData += "81026";	strData += sFS;		// �ż�ȣ��
		//strData += "80374";	strData += sFS;		// �ż���������
		strData += "81137";	strData += sFS;		// �ż����ϴ��
		strData += "81132";	strData += sFS;		// �ż��ð�
		strData += "81133";	strData += sFS;		// �ż���
		strData += "81134";	strData += sFS;		// �ż�����

		strData += "80382";	strData += sFS;		// �ż����Ͻð�
		strData += "80383";	strData += sFS;		// �ż����ϰ�
		strData += "80384";	strData += sFS;		// �ż���������
		strData += "80385";	strData += sFS;		// �ż���������
	}


	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound
	// íƮ �ɺ�
	strData += _T('$');
	if(nType == 0) strData += "112410";
	else if(nType == 1) strData += "112510";
	else strData += "112610";
	strData += sDS;

	//////////////////////////////////////////////////////////////////////////
	// �׸��� ���
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));

	char szTemp[5];

//	memcpy(grid_i.vrow, _T("11"), sizeof(grid_i.vrow));
	memcpy(grid_i.vrow, _T("20"), sizeof(grid_i.vrow));

	sprintf(szTemp, _T("%04d"), nCount);	// ��ȸ ������ ��
	memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));

//	grid_i.vflg[0] = _T('1');
	grid_i.vflg[0] = _T('0');
	grid_i.gdir[0] = _T('1');	//	top.
//	grid_i.sdir[0] = _T('1');
	grid_i.sdir[0] = _T('0');

	if (m_InData.gubun[0] == '1')	grid_i.ikey[0] = _T('2');	//	Page Down
	else							grid_i.ikey[0] = _T('0');	//	�Ϲ�(Next��ȸ).

	memcpy(grid_i.page, m_InData.keyvalue, 4);
	memcpy(grid_i.save, m_InData.keyvalue+4, sizeof(grid_i.save));

	
	char szGrid_i[128];
	memset(szGrid_i, 0x00, sizeof(szGrid_i));
	memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));

	strData += szGrid_i;
	//////////////////////////////////////////////////////////////////////////
	// Row Data ����
	m_nReqFldCnt = 0;
	switch (nType)
	{
	case 0:		// ƽ
	case 1:		// ��
	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
		if( nMType == 0 || nMType == 2 )		// �ŵ�
		{
			for (i = 0; i < m_nFldCnt; i++)
			{
				strData += szRowFieldFX_S[i];
				strData += sCOL;
				m_nReqFldCnt ++;
			}
		}
		if( nMType == 1 || nMType == 2 )		// �ż�
		{
			for (i = (nMType==2) ? 2:0; i < m_nFldCnt; i++)
			{
				strData += szRowFieldFX_B[i];
				strData += sCOL;
				m_nReqFldCnt ++;
			}
		}
		break;
	}
	
	strData += sFS;

	return strData;
}

char* CChartItemFX::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	SM_fx112_OutRec1 stOut1;
	SM_fx112_CHART stOut;
	SM_fx112_OutRec1_Sub *pOut1_Sub;
	CChartBozoInput bojoInput_S, bojoInput_B, *pBojoInput = NULL;
	int nIndex = 0;

	char *pszBuff = NULL, *pszData=NULL;
	pszBuff = new char[nLen+1];
	memset(pszBuff, 0x00, nLen+1);
	memcpy(pszBuff, pData, nLen);
	
	pszData = pszBuff;

	CString strNode, strName, strLock, strDecimalGB, strLastDate;
	CString strPrice, strVolume, strSDate, strEDate, strMType, strMDay;
	CString strBaseTime;

	double dOpen, dHigh, dLow, dPreOpen, dPreHigh, dPreLow, dMaxHigh, dMinLow;
	double dPrePrice, dChRate=0.0;
	
	dPrePrice = dOpen = dHigh = dLow = dPreOpen = dPreHigh = dPreLow = dMaxHigh = dMinLow = 0;

	memset(&stOut, 0x20, sizeof(stOut));
	memset(&stOut1, 0x20, sizeof(stOut1));

	int nPrice = 0;

	pszData += CDataConverter::Parser2(pszData, sFS, m_bojoInput.m_sShcode) + 1;	// �����ڵ�
	bojoInput_S.m_sShcode = bojoInput_B.m_sShcode = m_bojoInput.m_sShcode;
	pszData += CDataConverter::Parser2(pszData, sFS, strDecimalGB) + 1;				// �Ҽ����ڸ���
	pszData += CDataConverter::Parser2(pszData, sFS, strName) + 1;					// �����
	memcpy(stOut1.name, strName, min(strName.GetLength(), sizeof(stOut1.name)));


	pszData += CDataConverter::Parser2(pszData, sFS, strSDate) + 1;	// ����۽ð�(KST)
	pszData += CDataConverter::Parser2(pszData, sFS, strEDate) + 1;	// ������ð�(KST)
	strBaseTime = strSDate + "00";
	strSDate = strSDate + "00|" + strEDate + "00";

	double dValue=0;	//, dRate;
	int nRow, nPcnt;
	//char cType = CDataConverter::GetDecimalRate(strDecimalGB, nPcnt, dRate);
	//stOut1.codetype[0] = cType;

	nPcnt = 0;
	if( strDecimalGB.GetLength() > 1 )
		nPcnt = atoi(strDecimalGB);
	memcpy(stOut1.piplowest, strDecimalGB, min(sizeof(stOut1.piplowest), strDecimalGB.GetLength()));
	memcpy(stOut1.mgubun, m_InData.mgubun, min(sizeof(stOut1.mgubun), sizeof(m_InData.mgubun)));

	CString strTemp = CString(m_InData.mgubun, sizeof(m_InData.mgubun)); // 0:�ŵ�,1:�ż�
	int nMType = atoi(strTemp);
	int nTmpType = nMType == 2 ? 0 : nMType;

	while( nTmpType <= 1 )
	{
		if( nTmpType == 0 )		// �ŵ�
		{
			pOut1_Sub = &stOut1.fx112OutRec1_S;
			pBojoInput = &bojoInput_S;
		}
		else if( nTmpType == 1 )		// �ż�
		{
			pOut1_Sub = &stOut1.fx112OutRec1_B;
			pBojoInput = &bojoInput_B;
		}

		// ���簡
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		memcpy(pOut1_Sub->price, strNode, min(sizeof(pOut1_Sub->price), strNode.GetLength()));
		pBojoInput->m_sPrice = strNode.Mid(1);

		/*// �������� aaaaaaaaaaaaaaaaaaaaaaaa
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		memcpy(pOut1_Sub->preprice, strNode, min(sizeof(pOut1_Sub->preprice), strNode.GetLength()));
		pBojoInput->m_sPreprice = strNode.Mid(1);
		*/

		// ��������
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		strNode = strNode.Mid(1);	strNode.Remove(_T('.'));
		memcpy(pOut1_Sub->change, strNode, min(sizeof(pOut1_Sub->change), strNode.GetLength()));

		// �ð�
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		memcpy(pOut1_Sub->open, strNode, min(sizeof(pOut1_Sub->open), strNode.GetLength()));
		pBojoInput->m_sOpen = strNode.Mid(1);

		// ��
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		memcpy(pOut1_Sub->high, strNode, min(sizeof(pOut1_Sub->high), strNode.GetLength()));
		pBojoInput->m_sHigh = strNode.Mid(1);

		// ����
		pszData += CDataConverter::Parser2(pszData, sFS, pBojoInput->m_sLow) + 1;
		memcpy(pOut1_Sub->low, strNode, min(sizeof(pOut1_Sub->low), strNode.GetLength()));
		pBojoInput->m_sLow = strNode.Mid(1);

		// ���Ͻð�
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		memcpy(pOut1_Sub->preopen, strNode, min(sizeof(pOut1_Sub->preopen), strNode.GetLength()));
		pBojoInput->m_sPreOpen= strNode;//strNode.Mid(1);

		// ���ϰ�
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		memcpy(pOut1_Sub->prehigh, strNode, min(sizeof(pOut1_Sub->prehigh), strNode.GetLength()));
		pBojoInput->m_sPreHigh = strNode;//strNode.Mid(1);

		// ��������
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		memcpy(pOut1_Sub->prelow, strNode, min(sizeof(pOut1_Sub->prelow), strNode.GetLength()));
		pBojoInput->m_sPreLow = strNode;//strNode.Mid(1);

		// ��������
		pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
		memcpy(pOut1_Sub->preprice, strNode, min(sizeof(pOut1_Sub->preprice), strNode.GetLength()));
		pBojoInput->m_sPreprice = strNode;//strNode.Mid(1);

		if( nMType != 2 )		break;
		else					nTmpType ++;
	}

	int nSize, nCnt, nType;
	strNode = CString(m_InData.dgubun, sizeof(m_InData.dgubun)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	nType = atoi(strNode);
	nSize = m_nReqFldCnt;
//	if (nType == 0)			nSize = m_nTickCnt;
//	else if(nType == 1)		nSize = m_nMinCnt;
//	else if(nType == 6)		nSize = m_nSecCnt;
//	else					nSize = m_nDayCnt;

	// �׸��� ���
	GRAPH_IO gridOut;

	nIndex = sizeof(gridOut);
	memcpy(&gridOut, pszData, nIndex);
//	if ((gridOut.xpos[0] & 0x01) || (gridOut.xpos[0] & 0x02))
//		memcpy(stOut1.nkey, gridOut.page, sizeof(gridOut.page));

	pszData += nIndex;

	CStringArray arrNode;
	int nNodeIndex = 1;
	while ((pszData - pszBuff) < nLen-2)
	{
		pszData += CDataConverter::Parser2(pszData, (nNodeIndex == nSize)?sROW:sCELL, strNode) + 1;
		arrNode.Add(strNode);
		
		if (nNodeIndex == nSize)
			nNodeIndex = 1;
		else							nNodeIndex++;
	}

	// ��ü ������ �� ���
	nCnt = arrNode.GetSize()/ nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// ��Ʈ �� ������
	int nBongSize = sizeof(SM_fx112_OutRec2);
	int	nPosBong = sizeof(SM_fx112_OutRec1) + sizeof(SM_fx112_CHART) - 1;	// SM_fx112_OutRec1+�����޼���
	int nBufSize = nPosBong + nBongSize * nCnt;

	int nPosBong_Sell = nPosBong;
	if( nMType == 2 )		// �ŵ� �ż� ��� ��ȸ�� ���
	{
		nBufSize *= 2;
		// �ŵ�/�ż� ���ÿ� ��ȸ�� ��� �ŵ� ����Ÿ�� �ֱ� ���� ���� ��ġ ���
		nPosBong_Sell = nPosBong + (nCnt*nBongSize) + sizeof(SM_fx112_CHART) - 1;// SM_fx112_OutRec1+�ż������޼���+�ż�������Ÿ+�ŵ������޼���
	}

	nDataLen = nBufSize;

	CString strDate = "00000000", strTime = "000000";
	char* pDataBuf = new char[nBufSize+1];
	SM_fx112_OutRec2 stFirstBongB, stFirstBongS, stBong;

	nIndex = 0;
	for(nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);

		// time : ƽ,��,���� ��
		strTime = arrNode.GetAt(nIndex++); 
		
		if( strTime.GetLength() == 0 )
			strTime = "000000";
		
		// 2011.01.13 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)

// 		if(nType == 0) // DDHHMMSS
// 			strNode = strDate.Right(2) + strTime;
// 		else if(nType == 1) //MMDDHHMM
// 			strNode = strDate.Right(4) + strTime.Left(4);
// 		else if(nType == 4) //YYYYMM
// 			strNode = strDate.Left(6);
// 		else if(nType == 6) //DDHHMMSS
// 			strNode = strDate.Right(2) + strTime.Left(6);
// 		else 
// 			strNode = strDate;

		if(nType == 0)		// ƽ : DDHHMMSS
		{
			strNode = strDate + strTime.Left(6);
		}
		else if(nType == 1) // �� : MMDDHHMM ==> YYYYMMDDHHMMSS �� ����
		{
			strNode = strDate + strTime.Left(6);
		}
		else if(nType == 4) // �� : YYYYMM
		{
			strNode = strDate.Left(6);
			strNode += _T("        ");
		}
		else if(nType == 6) // �� : DDHHMMSS
		{
			strNode = strDate + strTime.Left(6);
		}
		else 
		{
			strNode = strDate;
			strNode += _T("      ");
		}

		// 14Byte : YYYYMMDDHHMMSS 
		//strNode.Format("%8s%6s", strDate, strTime);
		// 2011.01.13 by SYS 

/*
		if ((nType == 0 && nRow == 0) || (nType != 0 && nRow == nCnt-1))
		{
			memcpy(stOut1.endDate, strDate, min(strDate.GetLength(), sizeof(stOut1.endDate)));
			strLastDate = strDate+strTime.Left(6);
		}
		else if ((nType != 0 && nRow == 0) || (nType == 0 && nRow == nCnt-1))
			memcpy(stOut1.startDate, strDate, min(strDate.GetLength(), sizeof(stOut1.startDate)));
*/
		if( nRow == nCnt-1 )		// ù����Ÿ
		{
			memcpy(stOut1.startDate, strDate, min(strDate.GetLength(), sizeof(stOut1.startDate)));
		}
		else if(nRow == 0)
		{
			memcpy(stOut1.endDate, strDate, min(strDate.GetLength(), sizeof(stOut1.endDate)));
			strLastDate = strDate+strTime.Left(6);
		}

		memcpy(stBong.date, strNode, min(sizeof(stBong.date), strNode.GetLength()));
		
		nTmpType = nMType == 2 ? 0 : nMType;

		while( nTmpType <= 1 )
		{
			// ����/ü�ᰡ
			strNode = arrNode.GetAt(nIndex++);
			memcpy(stBong.close, strNode, min(sizeof(stBong.close), strNode.GetLength()));
			// �ð�
			strNode = arrNode.GetAt(nIndex++);
			memcpy(stBong.open, strNode, min(sizeof(stBong.open), strNode.GetLength()));
			// ��
			strNode = arrNode.GetAt(nIndex++);
			memcpy(stBong.high, strNode, min(sizeof(stBong.high), strNode.GetLength()));
			// ����
			strNode = arrNode.GetAt(nIndex++);
			memcpy(stBong.low, strNode, min(sizeof(stBong.low), strNode.GetLength()));

			if( nMType == 2 && nTmpType == 0 )	// �ŵ��ż� ������ȸ�� �ŵ��� ���. 2��° ��Ʈ ���ۿ� �ִ´�.
			{
				//memcpy(&pDataBuf[nPosBong + (nCnt+(nCnt-nRow-1))*nBongSize], &stBong, nBongSize);
				memcpy(&pDataBuf[nPosBong_Sell + (nCnt-nRow-1)*nBongSize], &stBong, nBongSize);
			}
			else
			{
				memcpy(&pDataBuf[nPosBong + (nCnt-nRow-1)*nBongSize], &stBong, nBongSize);
			}

			// ù �����͸� ������ ���´�.
//			if ((nType == 0 && nRow == 0) || (nType != 0 && nRow == nCnt-1))
			if ( nRow == nCnt-1 )
			{
				if( nTmpType == 0 )		// �ŵ�
					memcpy(&stFirstBongS, &stBong, sizeof(stBong));
				else
					memcpy(&stFirstBongB, &stBong, sizeof(stBong));
			}

			if( nMType != 2 )		break;
			else					nTmpType ++;
		}
			
	}

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	if (nCnt > 0 && nType == 3 && m_InData.gubun[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = CString(stBong.date, sizeof(stBong.date));
	else
		m_bojoInput.m_sLastDataTime = _T("");

	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ���� �޽��� �����
	CChartBozoManager bojoMng;

	nTmpType = nMType == 2 ? 0 : nMType;

	while( nTmpType <= 1 )
	{
		if( nTmpType == 0 )		// �ŵ�
		{
			pOut1_Sub = &stOut1.fx112OutRec1_S;
			pBojoInput = &bojoInput_S;
		}
		else if( nTmpType == 1 )		// �ż�
		{
			pOut1_Sub = &stOut1.fx112OutRec1_B;
			pBojoInput = &bojoInput_B;
		}

		//	m_bojoInput.m_sPreVolume = strPreVolume;	// ���ϰŷ���
		//	m_bojoInput.m_sStartTime = "090000";	// �ŷ����۽ð�(HHMMSS) <<!>>
		// �ŵ�����Ÿ �����޼���
		m_bojoInput.m_sFirstClose = CString(stFirstBongS.close, sizeof(stFirstBongS.close)) ;	// ��ȸ�� ù���� ����.
		
		m_bojoInput.m_sDataType = _T("0");		//	����. ���� �ʿ�� ����.
		m_bojoInput.m_sMarket = _T("1");			//	����. ���� �ʿ�� ����.
		m_bojoInput.m_sNxtFlg = m_InData.gubun[0];
		
		if(nType == 0)
		{
			m_bojoInput.m_sTerm = "0";	//	ƽ
			m_bojoInput.m_sTick = CString(m_InData.nsel, sizeof(m_InData.nsel));
		}
		else if(nType == 1)
		{
			m_bojoInput.m_sTerm = "1";	//	��
			m_bojoInput.m_sTick = CString(m_InData.nsel, sizeof(m_InData.nsel));
			//{{JS.Kim_20100903 Solomon 60�к� �̻��� ��� �ð����� ���� �����Ͽ��� ��
			int nTick = atoi(m_bojoInput.m_sTick);
			if( nTick >= 60 )
				m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
			//}}
		}
		else if(nType >= 2)
		{
			m_bojoInput.m_sTerm.Format("%d", nType);	//	��,��,��
			m_bojoInput.m_sTick = _T("1");
		}

		// ���簡
		m_bojoInput.m_sPrice = pBojoInput->m_sPrice;
		// �ð�
		m_bojoInput.m_sOpen = pBojoInput->m_sOpen;
		// ��
		m_bojoInput.m_sHigh = pBojoInput->m_sHigh;
		// ����
		m_bojoInput.m_sLow = pBojoInput->m_sLow;
		// ��������
		m_bojoInput.m_sPreprice = pBojoInput->m_sPreprice;
		// ���Ͻð�
		m_bojoInput.m_sPreOpen = pBojoInput->m_sPreOpen;
		// ���ϰ�
		m_bojoInput.m_sPreHigh = pBojoInput->m_sPreHigh;
		// ��������
		m_bojoInput.m_sPreLow = pBojoInput->m_sPreLow;


		CString strBozo;
		char szKey[4];
		memset(szKey, 0x00, sizeof(szKey));
		sprintf(szKey, _T("%03d"), 112); // 112�� fx

		nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode, nPcnt);

		CString strTimeIdx;//, strRate;
		CString strBozoTail;
		//strRate.Format(_T("%.*f"), nPcnt, dRate);
		strName = m_bojoInput.m_sShcode + ":" + strName;

		List_CChartBozoItem arr;
		CChartBozoItem *pItem;

		//100618-alzioyes : �״� �������.
		strLastDate.TrimLeft();	strLastDate.TrimRight();

		//{{JS.Kim_20100903 Solomon	�������� ����ð� ���� �����Ƿ� ������ �������� ����
		//pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("DATATIME"), strLastDate);	arr.AddTail(pItem);
		//}}
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("CODEANDNAME"), strName);	arr.AddTail(pItem);
		//pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("MARKETDAY"), strMDay);		arr.AddTail(pItem);

		// FX�϶� MARKETTIME�� BASETIME�� 6�ñ������� �ϱ�� ����.(with ��:�۴�������)
		// ���� PacketData.dll���� defaultó���ϵ��� �Ѵ�.
		// pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("MARKETTIME"), strSDate);	arr.AddTail(pItem);
		// pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("BASETIME"), strBaseTime);	arr.AddTail(pItem);

		nSize = bojoMng.ChangeChartBozoMsg(strNode, &arr);

		memset(stOut.bojomsg, ' ', sizeof(stOut.bojomsg));		//	�����޽���
		memcpy(stOut.bojomsg, (LPSTR)(LPCTSTR)strNode, nSize);		//	�����޽���
		memcpy(stOut.bojomsg, "0496", 4);

		POSITION pos = arr.GetHeadPosition();
		while (pos)
		{
			pItem = arr.GetNext(pos);
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		arr.RemoveAll();

		if( nMType == 2 && nTmpType == 0 )	// �ŵ��ż� ������ȸ�� �ŵ��� ���. 2��° ��Ʈ ���ۿ� �ִ´�.
		{
			memcpy(&pDataBuf[nPosBong + nCnt*nBongSize], &stOut, sizeof(stOut)-1);
		}
		else
		{
			memcpy(&pDataBuf[sizeof(stOut1)], &stOut, sizeof(stOut)-1);
		}

		if( nMType != 2 )		break;
		else					nTmpType ++;
	}

	memcpy(pDataBuf, &stOut1, sizeof(stOut1));


	delete []pszBuff;
	pszBuff = NULL;

	return pDataBuf;
}
