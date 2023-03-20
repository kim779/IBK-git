// ChartItemFOForeignHoga.cpp: implementation of the CChartItemFOForeignHoga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFOForeignHoga.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ���ذ�, ���簡
// �ŵ�ȣ��5, �ŵ��ܷ�5, �ŵ�ȣ��4, �ŵ��ܷ�4, �ŵ�ȣ��3, �ŵ��ܷ�3, �ŵ�ȣ��2, �ŵ��ܷ�2, �ŵ�ȣ��1, �ŵ��ܷ�1
// �ż�ȣ��1, �ż��ܷ�1, �ż�ȣ��2, �ż��ܷ�2, �ż�ȣ��3, �ż��ܷ�3, �ż�ȣ��4, �ż��ܷ�4, �ż�ȣ��5, �ż��ܷ�5
// �ŵ� �� �ܷ�, �ż� �� �ܷ�, ��, ����, �����, �����ŷ���
char* szFOFoHogaField[] = {"70319","71023",
						"72051","72041","72052","72042","72053","72043","72054","72044","72055", "72045",
						"72071","72061","72072","72062","72073","72063","72074","72064","72075", "72065",
						"72101","72106","71030","71031","71033","71027",
						NULL};

CChartItemFOForeignHoga::CChartItemFOForeignHoga()
{
	m_bSet = FALSE;
	
	int nIndex = 0;
	while (szFOFoHogaField[nIndex] != NULL)
		nIndex++;

	m_nCnt = nIndex;
}

CChartItemFOForeignHoga::~CChartItemFOForeignHoga()
{

}

void CChartItemFOForeignHoga::SetInData(HCQ01112_IN* pData)
{
	memcpy(&m_InData, pData, sizeof(HCQ01112_IN));
	m_bSet = TRUE;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemFOForeignHoga::Convert()
{
	if(!m_bSet) return "";
	
	int i;
	CString strData, strCode;

	strCode = CString(m_InData.jmcode, sizeof(m_InData.jmcode));

	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ ������
	strData += "70301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData += sFS;

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound
	for (i=0; i<m_nCnt; i++)
	{
		strData += szFOFoHogaField[i];
		strData += sFS;
	}

	return strData;
}

char* CChartItemFOForeignHoga::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	HCQ01112_OUT stOut;
	CString strNode, strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);

	memset(&stOut, 0x20, sizeof(stOut));
	memcpy(stOut.jmcode, m_InData.jmcode, sizeof(stOut.jmcode));

	strNode = CDataConverter::Parser(strSrc, sFS);	// ���ذ�
	memcpy(stOut.jsprice, strNode, min(strNode.GetLength(), sizeof(stOut.jsprice)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// ���簡
	memcpy(stOut.price, strNode, min(strNode.GetLength(), sizeof(stOut.price)));

	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ�ȣ��5
	memcpy(stOut.mdhoga5, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga5)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ��ܷ�5
	memcpy(stOut.mdhorem5, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem5)));

	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ�ȣ��4
	memcpy(stOut.mdhoga4, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga4)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ��ܷ�4
	memcpy(stOut.mdhorem4, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem4)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ�ȣ��3
	memcpy(stOut.mdhoga3, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga3)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ��ܷ�3
	memcpy(stOut.mdhorem3, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem3)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ�ȣ��2
	memcpy(stOut.mdhoga2, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga2)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ��ܷ�2
	memcpy(stOut.mdhorem2, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem2)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ�ȣ��1
	memcpy(stOut.mdhoga1, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga1)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ŵ��ܷ�1
	memcpy(stOut.mdhorem1, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem1)));

	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż�ȣ��1
	memcpy(stOut.mshoga1, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga1)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż��ܷ�1
	memcpy(stOut.mshorem1, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem1)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż�ȣ��2
	memcpy(stOut.mshoga2, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga2)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż��ܷ�2
	memcpy(stOut.mshorem2, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem2)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż�ȣ��3
	memcpy(stOut.mshoga3, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga3)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż��ܷ�3
	memcpy(stOut.mshorem3, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem3)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż�ȣ��4
	memcpy(stOut.mshoga4, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga4)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż��ܷ�4
	memcpy(stOut.mshorem4, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem4)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż�ȣ��5
	memcpy(stOut.mshoga5, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga5)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// �ż��ܷ�5
	memcpy(stOut.mshorem5, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem5)));

	// �ŵ� �� �ܷ�
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.totmdhorem, strNode, min(strNode.GetLength(), sizeof(stOut.totmdhorem)));
	// �ż� �� �ܷ�
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.totmshorem, strNode, min(strNode.GetLength(), sizeof(stOut.totmshorem)));
	// ��
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.high, strNode, min(strNode.GetLength(), sizeof(stOut.high)));
	// ����
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.low, strNode, min(strNode.GetLength(), sizeof(stOut.low)));
	// �����ȣ
//	strNode = CDataConverter::Parser(strSrc, sFS);
//	memcpy(stOut.sign, strNode, min(strNode.GetLength(), sizeof(stOut.sign)));
	// �����
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.change, strNode, min(strNode.GetLength(), sizeof(stOut.change)));
	// �����ŷ���
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.volume, strNode, min(strNode.GetLength(), sizeof(stOut.volume)));

	int nSize = sizeof(HCQ01112_OUT);
	
	char* pDataBuf = NULL;
	pDataBuf = new char[nSize+1];
	memset(pDataBuf, 0x00, nSize+1);
	memcpy(pDataBuf, &stOut, sizeof(stOut));
	
	nDataLen = sizeof(HCQ01112_OUT);

	return pDataBuf;
}
