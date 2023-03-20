// ChartItemDollar.cpp: implementation of the CChartItemDollar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemDollar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szDollarSymbol[] = { "51301", "51022", "51023", NULL};

CChartItemDollar::CChartItemDollar()
{
	m_bSet = FALSE;
	m_nCnt = 0;
	while (szDollarSymbol[m_nCnt] != NULL)		m_nCnt++;
}

CChartItemDollar::~CChartItemDollar()
{
}

void CChartItemDollar::SetInData(KB_d1018_InRec* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_d1018_InRec));
	m_bSet = TRUE;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemDollar::Convert()
{
	if (!m_bSet) return "";

	CString strCode, strData;
	int i;

	strCode	= CDataConverter::Char2String(m_InData.code, sizeof(m_InData.code));
	//////////////////////////////////////////////////////////////////////////
	//	Inbound Data
	//	�����ڵ� Input �ɺ�
	strData += "51301";	strData += sDS;	strData += strCode;	strData += sCELL;//sFS;

	//////////////////////////////////////////////////////////////////////////
	//	OutBound Data
	for (i=0; i<m_nCnt; i++)
	{
		strData += szDollarSymbol[i];
		strData += sCELL;//KHDsFS;
	}

	return strData;
}

char* CChartItemDollar::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_d1018_OutRec stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);

	CString strNode, strCode;
	int nBufSize = sizeof(KB_d1018_OutRec);

	memset(&stOut, 0x20, sizeof(stOut));

	strCode = CDataConverter::Parser(strSrc, sCELL); // �����ڵ�

	strNode = CDataConverter::Parser(strSrc, sCELL); // �����
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strNode = CDataConverter::Parser(strSrc, sCELL); // ���簡
	memcpy(stOut.price, strNode, min(sizeof(stOut.price), strNode.GetLength()));

	char* pDataBuf = new char[nBufSize+1];
	memcpy(pDataBuf, &stOut, sizeof(stOut));

	nDataLen = nBufSize;

	return pDataBuf;
}
