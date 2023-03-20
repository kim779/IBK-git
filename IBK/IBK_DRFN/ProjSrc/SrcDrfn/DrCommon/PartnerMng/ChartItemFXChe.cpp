// ChartItemFXChe.cpp: implementation of the CChartItemFXChe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFXChe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//har* szRowFieldFX_S[]	= { "112302", "112034", "112025", "112129", "112130", "112131", NULL };
//char* szRowFieldFX_B[]	= { "112302", "112034", "112026", "112132", "112133", "112134", NULL };
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
CChartItemFXChe::CChartItemFXChe()
{
	m_bSet = FALSE;
	m_nReqFldCnt = 0;
	m_nFldCnt = 0;//m_nTickCnt = 0;	m_nSecCnt = 0;	m_nMinCnt = 0;	m_nDayCnt = 0;	m_nWeekCnt = 0;	m_nGridCnt=0;
//	while (szRowFieldFX_S[m_nFldCnt] != NULL)		m_nFldCnt++;
}

CChartItemFXChe::~CChartItemFXChe()
{
}

void CChartItemFXChe::SetInData(FX_CHE_RQ_INPUT* pData)
{
	try
	{
		//if(gDebugMng) gDebugMng->TraceTran('I', "SetInit", sizeof(FX_CHE_RQ_INPUT), (LPBYTE)pData);
		if(g_pDRDebuger) g_pDRDebuger->DRLog_RequestData("SetInit", (LPBYTE)pData, sizeof(FX_CHE_RQ_INPUT));
		memcpy(&m_InData, pData, sizeof(FX_CHE_RQ_INPUT));
		m_bSet = TRUE;
	}
	catch (CException* e)
	{
		char aBuff[MAX_PATH]={0,};
		e->GetErrorMessage(aBuff, MAX_PATH);
	}
}

void CChartItemFXChe::SetTR(CString strTR)
{
	m_strTR = strTR;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
//    char    szAccNo[14];				/* ���¹�ȣ */
//    char    szAccNoPW[8];				/* ��й�ȣ */
//    char    szItemCode[16];				/* �����ڵ� */
//    char    szChGB[1];					/* ü�ᱸ�� */
//    char    szMMGB[1];					/* �ŵ�/�ż� ���� */
//    char	  gubun[1];					/* ���ӱ��� */

CString CChartItemFXChe::Convert()
{
	if(!m_bSet) return "";

	CString strData = "", strAccNo, strAccNoPW, strCode, strTemp, strMMGB, strChGB;

	strAccNo = CString(m_InData.szAccNo, sizeof(m_InData.szAccNo));
	strAccNo.TrimLeft(); strAccNo.TrimRight();

	strAccNoPW = CString(m_InData.szAccNoPW, sizeof(m_InData.szAccNoPW));
	strAccNoPW.TrimLeft(); strAccNoPW.TrimRight();

	strCode = CString(m_InData.szItemCode, sizeof(m_InData.szItemCode));
	strCode.TrimLeft(); strCode.TrimRight();

	strMMGB = CString(m_InData.szMMGB, sizeof(m_InData.szMMGB));
	strMMGB.TrimLeft(); strMMGB.TrimRight();

	strChGB = CString(m_InData.szChGB, sizeof(m_InData.szChGB));
	strChGB.TrimLeft();	strChGB.TrimRight();

	//	���¹�ȣ Input �ɺ�
	strData += "901310";	strData += sDS;	strData += strAccNo;	strData += sFS;
	//	���º�й�ȣ Input �ɺ�
	strData += "901311";	strData += sDS;	strData += strAccNoPW;	strData += sFS;
	// �����ڵ� Input �ɺ�
	strData += "901301";	strData += sDS;	strData += strCode;		strData += sFS;
	//	ü�ᱸ�� Input �ɺ�	==>  '1':ü��, '2':��ü�� => ���� ��ȸ�ؾ��Ѵ�. �Ѳ����� ��ȸ�ϸ� ����ִ� ������ ����
	strData += "901312";	strData += sDS;	strData += strChGB;		strData += sFS;
	//	�ŵ��ż����� Input �ɺ� ==> �ȸԴµ�. ����� �൵ ��ȸ����� ����...
	//strData += "901313";	strData += sDS;	strData += strMMGB;		strData += sFS;
	// ���ӱ��� Input �ɺ�
	strData += "901319";	strData += sDS;	strData += "0";		strData += sFS;
//	strData += "901319";	strData += sFS;



	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound
	// íƮ �ɺ�
	strData += _T('$');
	if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// �ؿܼ���
		strData += "901832";
	else
		strData += "901840";
	strData += sDS;

	//////////////////////////////////////////////////////////////////////////
	// �׸��� ���
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));

	char szTemp[5];

	if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// �ؿܼ���
		memcpy(grid_i.vrow, _T("13"), sizeof(grid_i.vrow));
	else
		memcpy(grid_i.vrow, _T("19"), sizeof(grid_i.vrow));

	sprintf(szTemp, _T("%04d"), 2000);	// ��ȸ ������ ��
	memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));

	grid_i.vflg[0] = _T('1');
	grid_i.gdir[0] = _T('1');	//	top.
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
/*
	if( m_strTR != QUERY_STRFOFOREIGN_CHE2 )		// FX
	{
		strData += "901918";	strData += sCOL;	m_nReqFldCnt ++;		// �ŷ�����(11:����, 12:û��, 13:������)
	}
	strData += "901912";	strData += sCOL;	m_nReqFldCnt ++;		// �ŵ�/�ż� ���� "-"/"+"
	strData += "901988";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ���������
	strData += "901927";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ�����(1:���尡   2:������ 3:�������� 4:STOP-LIMIT 5:OCO V:�����ս�)
	strData += "901904";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ���ȣ
	strData += "901941";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ�����
	strData += "901947";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ��ð�
	strData += "901909";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ�����
	strData += "901910";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ�����

	strData += "901948";	strData += sCOL;	m_nReqFldCnt ++;		// ü���ȣ
	strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// ü������
	strData += "901923";	strData += sCOL;	m_nReqFldCnt ++;		// ü��ð�
	strData += "901931";	strData += sCOL;	m_nReqFldCnt ++;		// ����ü�����
	strData += "901916";	strData += sCOL;	m_nReqFldCnt ++;		// ü�ᰡ��(���)
	if( m_strTR != QUERY_STRFOFOREIGN_CHE2 )		// FX
	{
	   strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// �ܰ����(�̰������� ����)
	}
	strData += "901921";	strData += sCOL;	m_nReqFldCnt ++;		// ��ü�����
	if( m_strTR != QUERY_STRFOFOREIGN_CHE2 )		// FX
	{
		strData += "901318";	strData += sCOL;	m_nReqFldCnt ++;		// 
	}

	strData += "901945";	strData += sCOL;	m_nReqFldCnt ++;		// STOP����
	strData += "901950";	strData += sCOL;	m_nReqFldCnt ++;		// �����ǹ�ȣ
	strData += "901975";	strData += sCOL;	m_nReqFldCnt ++;		// �źλ���
*/
	if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// �ؿܼ���
	{
		strData += "901912";	strData += sCOL;	m_nReqFldCnt ++;		// �ŵ�/�ż� ���� "-"/"+"
		strData += "901916";	strData += sCOL;	m_nReqFldCnt ++;		// ü�ᰡ��(���)
		strData += "901936";	strData += sCOL;	m_nReqFldCnt ++;		// ü�����
		strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// ü������
		strData += "901923";	strData += sCOL;	m_nReqFldCnt ++;		// ü��ð�
	}
	else
	{
		strData += "901918";	strData += sCOL;	m_nReqFldCnt ++;		// �ŷ�����(11:����, 12:û��, 13:������)

		strData += "901912";	strData += sCOL;	m_nReqFldCnt ++;		// �ŵ�/�ż� ���� "-"/"+"
		strData += "901988";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ���������
		strData += "901927";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ�����(1:���尡   2:������ 3:�������� 4:STOP-LIMIT 5:OCO V:�����ս�)
		strData += "901904";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ���ȣ
		strData += "901941";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ�����
		strData += "901947";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ��ð�
		strData += "901909";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ�����
		strData += "901910";	strData += sCOL;	m_nReqFldCnt ++;		// �ֹ�����

		strData += "901948";	strData += sCOL;	m_nReqFldCnt ++;		// ü���ȣ
		strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// ü������
		strData += "901923";	strData += sCOL;	m_nReqFldCnt ++;		// ü��ð�
		strData += "901931";	strData += sCOL;	m_nReqFldCnt ++;		// ����ü�����
		strData += "901916";	strData += sCOL;	m_nReqFldCnt ++;		// ü�ᰡ��(���)
		strData += "901921";	strData += sCOL;	m_nReqFldCnt ++;		// ��ü�����

		strData += "901945";	strData += sCOL;	m_nReqFldCnt ++;		// STOP����
		strData += "901950";	strData += sCOL;	m_nReqFldCnt ++;		// �����ǹ�ȣ
		strData += "901975";	strData += sCOL;	m_nReqFldCnt ++;		// �źλ���
	}
	strData += sFS;

	return strData;
}

char* CChartItemFXChe::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	FX_CHE_STRUCT	*pOut;

	int nIndex = 0;

	char *pszBuff = NULL, *pszData=NULL;
	pszBuff = new char[nLen+1];
	memset(pszBuff, 0x00, nLen+1);
	memcpy(pszBuff, pData, nLen);
	
	pszData = pszBuff;

	CString strNode, strOpenDate, strOpenTime, strExeDate, strExeTime;
	double	fOrder = 0.;


	// �׸��� ���
	GRAPH_IO gridOut;

	nIndex = sizeof(gridOut);
	memcpy(&gridOut, pszData, nIndex);
//	memcpy(stOut1.nkey, gridOut.page, sizeof(gridOut.page));

	pszData += nIndex;

	int nSize, nCnt;
	nSize = m_nReqFldCnt;
	CStringArray arrNode;
	int nNodeIndex = 1;
	while ((pszData - pszBuff) < nLen-1)
	{
		pszData += CDataConverter::Parser2(pszData, (nNodeIndex == nSize)?sROW:sCELL, strNode) + 1;
		arrNode.Add(strNode);
		
		if (nNodeIndex == nSize)		nNodeIndex = 1;
		else							nNodeIndex++;
	}

	// ��ü ������ �� ���
	nCnt = arrNode.GetSize() / nSize;
	pOut = new FX_CHE_STRUCT[nCnt+1];

	nIndex = 0;
	for(int nRow = 0; nRow < nCnt; nRow++)
	{
		memset((char *)&pOut[nRow], 0x20, sizeof(FX_CHE_STRUCT));
/*
		if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// �ؿܼ���
		{
			memcpy(pOut[nRow].szTradeType, "91", 2);
		}
		else
		{
			// �ŷ�����(11:����, 12:û��, 13:������)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			memcpy(pOut[nRow].szTradeType, strNode, min(sizeof(pOut[nRow].szTradeType), strNode.GetLength()));
		}


		// �ŵ�/�ż� ���� "-"/"+"
		strNode = arrNode.GetAt(nIndex++);
		if( strNode == "+�ż�" )			memcpy(pOut[nRow].szSide, "079", 3);
		else								memcpy(pOut[nRow].szSide, "081", 3);

		// �ֹ���������
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		//if( strNode == "ü��" )				memcpy(pOut[nRow].szKeyType, "CH01", 4);
		//else								memcpy(pOut[nRow].szKeyType, "OR01", 4);
		//memcpy(pOut[nRow].szKeyType, "OR01", 4);//��ü�Ḹ ��ȸ�� ���
		if( m_InData.szChGB[0] == '1' )		memcpy(pOut[nRow].szKeyType, "CH01", 4);
		else								memcpy(pOut[nRow].szKeyType, "OR01", 4);

		// �ֹ�����(1:���尡   2:������ 3:�������� 4:STOP-LIMIT 5:OCO V:�����ս�)
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		memcpy(pOut[nRow].szOrdType, strNode, min(sizeof(pOut[nRow].szOrdType), strNode.GetLength()));

		// �ֹ���ȣ
		strNode = arrNode.GetAt(nIndex++);
		memcpy(pOut[nRow].szCustItem2, strNode, min(sizeof(pOut[nRow].szCustItem2), strNode.GetLength()));

		// �ֹ�����
		strNode = arrNode.GetAt(nIndex++);
		strOpenDate = strNode;
		strOpenDate.TrimLeft();		strOpenDate.TrimRight();

		// �ֹ��ð�
		strNode = arrNode.GetAt(nIndex++);
		strOpenTime = strNode;
		strOpenTime.TrimLeft();		strOpenTime.TrimRight();
		memcpy(pOut[nRow].szOpenTime, strOpenTime, min(sizeof(pOut[nRow].szOpenTime), strOpenTime.GetLength()));

		// �ֹ�����
		strNode = arrNode.GetAt(nIndex++);
		fOrder = atof(strNode);

		// �ֹ�����
		strNode = arrNode.GetAt(nIndex++);
		memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

		// ü���ȣ. ü���� �Ǿ��µ� ü���ȣ�� ���� ���� �ֹ���ȣ�� ü���ȣ�� �����Ѵ�.
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		if( (strNode.IsEmpty() || strNode == "0") &&  memcmp(pOut[nRow].szKeyType, "CH01", 4) == 0 )
			memcpy(pOut[nRow].szFXCMPosID, pOut[nRow].szCustItem2, min(sizeof(pOut[nRow].szFXCMPosID), sizeof(pOut[nRow].szCustItem2)));
		else
			memcpy(pOut[nRow].szFXCMPosID, strNode, min(sizeof(pOut[nRow].szFXCMPosID), strNode.GetLength()));

		// ü������
		strNode = arrNode.GetAt(nIndex++);
		strExeDate = strNode;
		strExeDate.TrimLeft();		strExeDate.TrimRight();

		// ü��ð�
		strNode = arrNode.GetAt(nIndex++);
		strExeTime = strNode;
		strExeTime.TrimLeft();		strExeTime.TrimRight();
		strExeTime.Remove(':');
		strExeTime = strExeDate + strExeTime;
		memcpy(pOut[nRow].szExeTime, strExeTime, min(sizeof(pOut[nRow].szExeTime), strExeTime.GetLength()));

		// ����ü�����
		strNode = arrNode.GetAt(nIndex++);
		pOut[nRow].fAmount = atof(strNode);						//ü�����

		// ü�ᰡ��(���)
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		if( strNode.GetLength() > 0 && atof(strNode) != 0.0)
			memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

		
		// ��ü�����
		strNode = arrNode.GetAt(nIndex++);
		pOut[nRow].fOrderAmt = atof(strNode);		

		if( m_strTR != QUERY_STRFOFOREIGN_CHE2 )		// FX
		{
			// ����
			strNode = arrNode.GetAt(nIndex++);
		}

		// STOP����
		strNode = arrNode.GetAt(nIndex++);
		memcpy(pOut[nRow].szStop, strNode, min(sizeof(pOut[nRow].szStop), strNode.GetLength()));

		// �����ǹ�ȣ
		strNode = arrNode.GetAt(nIndex++);
		memcpy(pOut[nRow].szPositionID, strNode, min(sizeof(pOut[nRow].szPositionID), strNode.GetLength()));

		// �źλ���
		strNode = arrNode.GetAt(nIndex++);
*/
		if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// �ؿܼ���
		{
			memcpy(pOut[nRow].szTradeType, "91", 2);
			memcpy(pOut[nRow].szKeyType, "CH01", 4);

			// �ŵ�/�ż� ���� "-"/"+"
			strNode = arrNode.GetAt(nIndex++);
			if( strNode == "+�ż�" )			memcpy(pOut[nRow].szSide, "079", 3);
			else								memcpy(pOut[nRow].szSide, "081", 3);

			// ü�ᰡ��(���)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			if( strNode.GetLength() > 0 && atof(strNode) != 0.0)
				memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

			// ü�����
			strNode = arrNode.GetAt(nIndex++);
			pOut[nRow].fAmount = atof(strNode);						//ü�����

			// ü������
			strNode = arrNode.GetAt(nIndex++);
			strExeDate = strNode;
			strExeDate.TrimLeft();		strExeDate.TrimRight();

			// ü��ð�
			strNode = arrNode.GetAt(nIndex++);
			strExeTime = strNode;
			strExeTime.TrimLeft();		strExeTime.TrimRight();
			strExeTime.Remove(':');
			strExeTime = strExeDate + strExeTime;
			memcpy(pOut[nRow].szExeTime, strExeTime, min(sizeof(pOut[nRow].szExeTime), strExeTime.GetLength()));
		}
		else		// FX
		{
			// �ŷ�����(11:����, 12:û��, 13:������)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			memcpy(pOut[nRow].szTradeType, strNode, min(sizeof(pOut[nRow].szTradeType), strNode.GetLength()));


			// �ŵ�/�ż� ���� "-"/"+"
			strNode = arrNode.GetAt(nIndex++);
			if( strNode == "+�ż�" )			memcpy(pOut[nRow].szSide, "079", 3);
			else								memcpy(pOut[nRow].szSide, "081", 3);

			// �ֹ���������
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			//if( strNode == "ü��" )				memcpy(pOut[nRow].szKeyType, "CH01", 4);
			//else								memcpy(pOut[nRow].szKeyType, "OR01", 4);
			//memcpy(pOut[nRow].szKeyType, "OR01", 4);//��ü�Ḹ ��ȸ�� ���
			if( m_InData.szChGB[0] == '1' )		memcpy(pOut[nRow].szKeyType, "CH01", 4);
			else								memcpy(pOut[nRow].szKeyType, "OR01", 4);

			// �ֹ�����(1:���尡   2:������ 3:�������� 4:STOP-LIMIT 5:OCO V:�����ս�)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			memcpy(pOut[nRow].szOrdType, strNode, min(sizeof(pOut[nRow].szOrdType), strNode.GetLength()));

			// �ֹ���ȣ
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szCustItem2, strNode, min(sizeof(pOut[nRow].szCustItem2), strNode.GetLength()));

			// �ֹ�����
			strNode = arrNode.GetAt(nIndex++);
			strOpenDate = strNode;
			strOpenDate.TrimLeft();		strOpenDate.TrimRight();

			// �ֹ��ð�
			strNode = arrNode.GetAt(nIndex++);
			strOpenTime = strNode;
			strOpenTime.TrimLeft();		strOpenTime.TrimRight();
			memcpy(pOut[nRow].szOpenTime, strOpenTime, min(sizeof(pOut[nRow].szOpenTime), strOpenTime.GetLength()));

			// �ֹ�����
			strNode = arrNode.GetAt(nIndex++);
			fOrder = atof(strNode);

			// �ֹ�����
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

			// ü���ȣ. ü���� �Ǿ��µ� ü���ȣ�� ���� ���� �ֹ���ȣ�� ü���ȣ�� �����Ѵ�.
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			if( (strNode.IsEmpty() || strNode == "0") &&  memcmp(pOut[nRow].szKeyType, "CH01", 4) == 0 )
				memcpy(pOut[nRow].szFXCMPosID, pOut[nRow].szCustItem2, min(sizeof(pOut[nRow].szFXCMPosID), sizeof(pOut[nRow].szCustItem2)));
			else
				memcpy(pOut[nRow].szFXCMPosID, strNode, min(sizeof(pOut[nRow].szFXCMPosID), strNode.GetLength()));

			// ü������
			strNode = arrNode.GetAt(nIndex++);
			strExeDate = strNode;
			strExeDate.TrimLeft();		strExeDate.TrimRight();

			// ü��ð�
			strNode = arrNode.GetAt(nIndex++);
			strExeTime = strNode;
			strExeTime.TrimLeft();		strExeTime.TrimRight();
			strExeTime.Remove(':');
			strExeTime = strExeDate + strExeTime;
			memcpy(pOut[nRow].szExeTime, strExeTime, min(sizeof(pOut[nRow].szExeTime), strExeTime.GetLength()));

			// ����ü�����
			strNode = arrNode.GetAt(nIndex++);
			pOut[nRow].fAmount = atof(strNode);						//ü�����

			// ü�ᰡ��(���)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			if( strNode.GetLength() > 0 && atof(strNode) != 0.0)
				memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));
			
			// ��ü�����
			strNode = arrNode.GetAt(nIndex++);
			pOut[nRow].fOrderAmt = atof(strNode);		

			// STOP����
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szStop, strNode, min(sizeof(pOut[nRow].szStop), strNode.GetLength()));

			// �����ǹ�ȣ
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szPositionID, strNode, min(sizeof(pOut[nRow].szPositionID), strNode.GetLength()));

			// �źλ���
			strNode = arrNode.GetAt(nIndex++);
		}

	}

	delete []pszBuff;
	pszBuff = NULL;

	nDataLen = sizeof(FX_CHE_STRUCT) * nCnt;
	return (char *)pOut;
}
