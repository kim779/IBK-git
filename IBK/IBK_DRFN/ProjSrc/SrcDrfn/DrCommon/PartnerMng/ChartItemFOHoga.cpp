// ChartItemFOHoga.cpp: implementation of the CChartItemFOHoga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFOHoga.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
						//���ذ�,	���Ѱ�,		���Ѱ�,		���簡,	
char* szFOHogaField[] = {"30319",	"30354",	"30355",	"31023",
					//	�ŵ�ȣ��1,	�ŵ�ȣ��2,	�ŵ�ȣ��3,	�ŵ�ȣ��1,	�ŵ�ȣ��2,	�ŵ�ȣ��3,
						"32051",	"32052",	"32053",	"32071",	"32072",	"32073",	NULL};
//���ذ�,	���Ѱ�,		���Ѱ�,		���簡,	
char* szOptionHogaField[] = {"40319",	"40354",	"40355",	"41023",
//	�ŵ�ȣ��1,	�ŵ�ȣ��2,	�ŵ�ȣ��3,	�ŵ�ȣ��1,	�ŵ�ȣ��2,	�ŵ�ȣ��3,
"42051",	"42052",	"42053",	"42071",	"42072",	"42073",	NULL};

CChartItemFOHoga::CChartItemFOHoga()
{
	m_bSet = FALSE;

	int nIndex = 0;
	while (szFOHogaField[nIndex] != NULL)
		nIndex++;

	m_nCnt = nIndex;
}

CChartItemFOHoga::~CChartItemFOHoga()
{

}

void CChartItemFOHoga::SetInData(KB_f0201_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_f0201_InRec1));
	m_bSet = TRUE;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemFOHoga::Convert()
{
	if(!m_bSet) return "";
	CString strCode;
	strCode = CString(m_InData.futcode, sizeof(m_InData.futcode));
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	m_nCodeType = pMasterDataManager->GetCodeTypeFromCode(strCode);
	
	switch(m_nCodeType)
	{
	case CDRCodeTypeInfo::CODE_KFUTURE://���ἱ��?? 
		return ConvertFuture();
		break;
	case CDRCodeTypeInfo::CODE_FUOPT_COMMODITY://��ǰ���� 
	//	return ConvertCommodity();	
		break;
	case	CDRCodeTypeInfo::CODE_KOPTION:
		return ConvertOption();//�ɼ��ϰ�� ù�ڸ��� 2,3
		break;
	case CDRCodeTypeInfo::CODE_FUTURE_SPREAD:
		return ConvertFuture();
		break;
	}

	return "";
}

CString CChartItemFOHoga::ConvertFuture()
{
	CString strData, strCode;
	strCode = CString(m_InData.futcode, sizeof(m_InData.futcode));
	
	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ ������
	strData += "30301"; // �����ڵ� Input �ɺ�
	strData += sDS;
	
	strData += strCode;
	strData += sCELL;
	
	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound
	for (int i=0; i<m_nCnt; i++)
	{
		strData += szFOHogaField[i];
		strData += sCELL;
	}

	return strData;
}

CString CChartItemFOHoga::ConvertOption()
{
	CString strData, strCode;
	strCode = CString(m_InData.futcode, sizeof(m_InData.futcode));
	
	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ ������
	strData += "40301"; // �����ڵ� Input �ɺ�
	strData += sDS;
	
	strData += strCode;
	strData += sCELL;
	
	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound
	for (int i=0; i<m_nCnt; i++)
	{
		strData += szOptionHogaField[i];
		strData += sCELL;
	}
	return strData;
}

char* CChartItemFOHoga::Convert2Struct(LPBYTE pData, int nLen, CString szTRCode, int &nDataLen)
{
	KB_f0201_OutRec1 stOut;
	CString strNode, strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);

	memset(&stOut, 0x20, sizeof(stOut));

	memcpy(stOut.futcode, m_InData.futcode, sizeof(stOut.futcode));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���ذ�
	strNode.Remove('.');
	memcpy(stOut.recprice, strNode, min(strNode.GetLength(), sizeof(stOut.recprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���Ѱ�
	strNode.Remove('.');
	memcpy(stOut.uplmtprice, strNode, min(strNode.GetLength(), sizeof(stOut.uplmtprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���Ѱ�
	strNode.Remove('.');
	memcpy(stOut.dnlmtprice, strNode, min(strNode.GetLength(), sizeof(stOut.dnlmtprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���簡
	strNode.Remove('.');
	memcpy(stOut.price, strNode, min(strNode.GetLength(), sizeof(stOut.price)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ�ȣ��1
	strNode.Remove('.');
	memcpy(stOut.offerho1, strNode, min(strNode.GetLength(), sizeof(stOut.offerho1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ�ȣ��2
	strNode.Remove('.');
	memcpy(stOut.offerho2, strNode, min(strNode.GetLength(), sizeof(stOut.offerho2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ�ȣ��3
	strNode.Remove('.');
	memcpy(stOut.offerho3, strNode, min(strNode.GetLength(), sizeof(stOut.offerho3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż�ȣ��1
	strNode.Remove('.');
	memcpy(stOut.bidho1, strNode, min(strNode.GetLength(), sizeof(stOut.bidho1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż�ȣ��2
	strNode.Remove('.');
	memcpy(stOut.bidho2, strNode, min(strNode.GetLength(), sizeof(stOut.bidho2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż�ȣ��3
	strNode.Remove('.');
	memcpy(stOut.bidho3, strNode, min(strNode.GetLength(), sizeof(stOut.bidho3)));

	int nSize = sizeof(KB_f0201_OutRec1);

	char* pDataBuf = NULL;
	pDataBuf = new char[nSize+1];
	memset(pDataBuf, 0x00, nSize+1);
	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}
