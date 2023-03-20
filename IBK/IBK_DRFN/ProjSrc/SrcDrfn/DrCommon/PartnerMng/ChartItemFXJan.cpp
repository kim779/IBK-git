// ChartItemFXJan.cpp: implementation of the CChartItemFXJan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFXJan.h"

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
CChartItemFXJan::CChartItemFXJan()
{
	m_bSet = FALSE;
	m_nReqFldCnt = 0;
	m_nFldCnt = 0;//m_nTickCnt = 0;	m_nSecCnt = 0;	m_nMinCnt = 0;	m_nDayCnt = 0;	m_nWeekCnt = 0;	m_nGridCnt=0;
//	while (szRowFieldFX_S[m_nFldCnt] != NULL)		m_nFldCnt++;
}

CChartItemFXJan::~CChartItemFXJan()
{
}

void CChartItemFXJan::SetInData(FX_CHE_RQ_INPUT* pData)
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

void CChartItemFXJan::SetTR(CString strTR)
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

CString CChartItemFXJan::Convert()
{
	if(!m_bSet) return "";

	CString strData = "", strAccNo, strAccNoPW, strCode, strTemp, strMMGB;

	strAccNo = CString(m_InData.szAccNo, sizeof(m_InData.szAccNo));
	strAccNo.TrimLeft(); strAccNo.TrimRight();

	strAccNoPW = CString(m_InData.szAccNoPW, sizeof(m_InData.szAccNoPW));
	strAccNoPW.TrimLeft(); strAccNoPW.TrimRight();

	strCode = CString(m_InData.szItemCode, sizeof(m_InData.szItemCode));
	strCode.TrimLeft(); strCode.TrimRight();

	strMMGB = CString(m_InData.szMMGB, sizeof(m_InData.szMMGB));
	strMMGB.TrimLeft(); strMMGB.TrimRight();

	//  �����ڵ� Input �ɺ�
	if( m_strTR == QUERY_STRFOFOREIGN_JAN )		// �ؿܼ���
	{
		strData += "70301";	strData += sDS;	strData += strCode;		strData += sFS;
	}
	else										// FX
	{
		strData += "901301";	strData += sDS;	strData += strCode;		strData += sFS;
	}

	//	���¹�ȣ Input �ɺ�
	strData += "901310";	strData += sDS;	strData += strAccNo;	strData += sFS;
	//	���º�й�ȣ Input �ɺ�
	strData += "901311";	strData += sDS;	strData += strAccNoPW;	strData += sFS;


	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound
	// íƮ �ɺ�
	strData += _T('$');
	if( m_strTR == QUERY_STRFOFOREIGN_JAN )		// �ؿܼ���
		strData += "901831";
	else
		strData += "901841";
	strData += sDS;

	//////////////////////////////////////////////////////////////////////////
	// �׸��� ���
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));

	memcpy(grid_i.vrow, _T("99"), sizeof(grid_i.vrow));
	memcpy(grid_i.nrow, _T("9999"), sizeof(grid_i.nrow));// ��ȸ ������ ��

	grid_i.vflg[0] = _T('0');
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

	strData += "901942";	strData += sCOL;	m_nReqFldCnt ++;		// �ŵ�/�ż� ���� "-"/"+"
	strData += "901994";	strData += sCOL;	m_nReqFldCnt ++;		// �����ǹ�ȣ
	if( m_strTR == QUERY_STRFOFOREIGN_JAN )		// �ؿܼ���
	{
		strData += "901953";	strData += sCOL;	m_nReqFldCnt ++;		// ü������
	}
	else
	{
		strData += "901924";	strData += sCOL;	m_nReqFldCnt ++;		// ��������
		strData += "901923";	strData += sCOL;	m_nReqFldCnt ++;		// ���Խð�
	}
	strData += "901941";	strData += sCOL;	m_nReqFldCnt ++;		// û�갡�ɼ���
	strData += "901947";	strData += sCOL;	m_nReqFldCnt ++;		// ���Դܰ�
	if( m_strTR != QUERY_STRFOFOREIGN_JAN )		// FX
	{
		strData += "901957";	strData += sCOL;	m_nReqFldCnt ++;		// STOP����
		strData += "901956";	strData += sCOL;	m_nReqFldCnt ++;		// LIMIT����
		strData += "901960";	strData += sCOL;	m_nReqFldCnt ++;		// T-STOP
	}

	strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// �ܰ����(�̰������� ����)
	

	strData += sFS;

	return strData;
}

char* CChartItemFXJan::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	FX_CHE_STRUCT	*pOut;

	int nIndex = 0;

	char *pszBuff = NULL, *pszData=NULL;
	pszBuff = new char[nLen+1];
	memset(pszBuff, 0x00, nLen+1);
	memcpy(pszBuff, pData, nLen);
	
	pszData = pszBuff;

	CString strNode, strExeDate, strExeTime;
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

		// �ֹ���������
		memcpy(pOut[nRow].szKeyType, "CH01", 4);		// ü��

		// �ŷ�����(11:����, 12:û��, 13:������)
		if( m_strTR == QUERY_STRFOFOREIGN_JAN )		// �ؿܼ���
			memcpy(pOut[nRow].szTradeType, "91", 2);
		else
			memcpy(pOut[nRow].szTradeType, "11", 2);		// ����

		// �ŵ�/�ż� ���� "-"/"+"
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		if( strNode == "+�ż�" )			memcpy(pOut[nRow].szSide, "079", 3);
		else								memcpy(pOut[nRow].szSide, "081", 3);

		// �����ǹ�ȣ
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		memcpy(pOut[nRow].szPositionID, strNode, min(sizeof(pOut[nRow].szPositionID), strNode.GetLength()));
		memcpy(pOut[nRow].szFXCMPosID, strNode, min(sizeof(pOut[nRow].szFXCMPosID), strNode.GetLength()));//ü���ȣ
		memcpy(pOut[nRow].szCustItem2, strNode, min(sizeof(pOut[nRow].szCustItem2), strNode.GetLength()));//�ֹ���ȣ

		if( m_strTR == QUERY_STRFOFOREIGN_JAN )		// �ؿܼ���
		{
			// ü������
			strNode = arrNode.GetAt(nIndex++);
			strExeTime = strNode;
			strExeTime.TrimLeft();		strExeTime.TrimRight();
			memcpy(pOut[nRow].szExeTime, strExeTime, min(sizeof(pOut[nRow].szExeTime), strExeTime.GetLength()));
		}
		else
		{
			// ��������
			strNode = arrNode.GetAt(nIndex++);
			strExeDate = strNode;
			strExeDate.TrimLeft();		strExeDate.TrimRight();

			// ���Խð�
			strNode = arrNode.GetAt(nIndex++);
			strExeTime = strNode;
			strExeTime.TrimLeft();		strExeTime.TrimRight();
			strExeTime = strExeDate + strExeTime;
			memcpy(pOut[nRow].szExeTime, strExeTime, min(sizeof(pOut[nRow].szExeTime), strExeTime.GetLength()));
		}

		// û�갡�ɼ���
		strNode = arrNode.GetAt(nIndex++);
		//pOut[nRow].fAmount = atof(strNode);						//ü�����

		// ���Դܰ�
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		if( strNode.GetLength() > 0 )
			memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

		if( m_strTR != QUERY_STRFOFOREIGN_JAN )		// FX
		{
			// STOP����
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szStop, strNode, min(sizeof(pOut[nRow].szStop), strNode.GetLength()));

			// LIMIT����
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szLimit, strNode, min(sizeof(pOut[nRow].szLimit), strNode.GetLength()));

			// T-STOP����
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szTStop, strNode, min(sizeof(pOut[nRow].szTStop), strNode.GetLength()));
		}

		// �ܰ����(�̰������� ����)
		strNode = arrNode.GetAt(nIndex++);
		pOut[nRow].fAmount = atof(strNode);						//ü�����
	}

	delete []pszBuff;
	pszBuff = NULL;

	nDataLen = sizeof(FX_CHE_STRUCT) * nCnt;
	return (char *)pOut;
}
