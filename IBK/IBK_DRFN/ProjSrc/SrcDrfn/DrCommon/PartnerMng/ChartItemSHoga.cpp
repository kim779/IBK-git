// ChartItemSHoga.cpp: implementation of the CChartItemSHoga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemSHoga.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
					//	�����,	���簡,	��񱸺�,���,	�����,	�ŷ���,	���ϰŷ������,	�켱�ŵ�ȣ��
char* szHogaField[] = { "1022", "2023", "", "2024", "2033", "2027", "", "2025",
		//	�켱�ż�ȣ��,�ܱ��κ�������,�ܱ��κ�������,ȣ�����Žð�,
			"2026",		"2339",			"2333",			"3040",
		//	�ŵ�ȣ��1,�ŵ�ȣ������1,�ŵ�����������1,	�ŵ�ȣ��2,�ŵ�ȣ������2,�ŵ�����������2,
			"3051",		"3041",		"3081",				"3052",		"3042",		"3082",
		//	�ŵ�ȣ��3,�ŵ�ȣ������3,�ŵ�����������3,	�ŵ�ȣ��4,�ŵ�ȣ������4,�ŵ�����������4,
			"3053",		"3043",		"3083",				"3054",		"3044",		"3084",
		//	�ŵ�ȣ��5,�ŵ�ȣ������5,�ŵ�����������5,	�ŵ�ȣ��6,�ŵ�ȣ������6,�ŵ�����������6,
			"3055",		"3045",		"3085",				"3056",		"3046",		"3086",
		//	�ŵ�ȣ��7,�ŵ�ȣ������7,�ŵ�����������7,	�ŵ�ȣ��8,�ŵ�ȣ������8,�ŵ�����������8,
			"3057",		"3047",		"3087",				"3058",		"3048",		"3088",
		//	�ŵ�ȣ��9,�ŵ�ȣ������9,�ŵ�����������9,	�ŵ�ȣ��10,�ŵ�ȣ������10,�ŵ�����������10,
			"3059",		"3049",		"3089",				"3060",		"3050",		"3090",
		//	�ż�ȣ��1,�ż�ȣ������1,�ż�����������1,	�ż�ȣ��2,�ż�ȣ������2,�ż�����������2,
			"3071",		"3061",		"3091",				"3072",		"3062",		"3092",
		//	�ż�ȣ��3,�ż�ȣ������3,�ż�����������3,	�ż�ȣ��4,�ż�ȣ������4,�ż�����������4,
			"3073",		"3063",		"3093",				"3074",		"3064",		"3094",
		//	�ż�ȣ��5,�ż�ȣ������5,�ż�����������5,	�ż�ȣ��6,�ż�ȣ������6,�ż�����������6,
			"3075",		"3065",		"3095",				"3076",		"3066",		"3096",
		//	�ż�ȣ��7,�ż�ȣ������7,�ż�����������7,	�ż�ȣ��8,�ż�ȣ������8,�ż�����������8,
			"3077",		"3067",		"3097",				"3078",		"3068",		"3098",
		//	�ż�ȣ��9,�ż�ȣ������9,�ż�����������9,	�ż�ȣ��10,�ż�ȣ������10,�ż�����������10,
			"3079",		"3069",		"3099",				"3080",		"3070",		"3100",
		//	�ŵ�ȣ���Ѽ���,�ŵ�ȣ���Ѽ����������,�ż�ȣ���Ѽ���,�ż�ȣ���Ѽ����������,����ü�ᰡ��,����ü�����
			"3101",		"3104",		"3106",				"3109",		"3111",		"3112",
		//	����ü�ᰡ����������񱸺�,����ü�ᰡ�����������,��������,�ð��ܸŵ��ܷ�,�ð��ܸż��ܷ�,�屸��
			"",		"3115",		"3116",				"3102",		"3107",		"1371",
		//	��������, ���ذ�, ���Ѱ�, ���Ѱ�, �ð�, ��, ����, �ŷ����,�Ǻ�,���ؼ�,1������,2������
			"2320",	"1336",	"2302",	"2303",	"2029",	"2030",	"2031",	"",	"2423",	"",	"2422",	"2421",
		//	1������, 2������, D����, D����, �׸鰡, ��밡, �ð��Ѿ�, �ں���, �����ֽļ�,PER,PBR,250���ְ�
			"2424",	"2425",	"2426",	"2427",	"1337",	"1338",	"2305",	"1333",	"1334",	"5355",	"5352",	"5311",
		//	250���ְ������, 250���ְ�����, 250��������, 250�������������, 250������������
			"5341",	"5321",	"5312",	"5342",	"5322",
		//	�ŷ����, �ŷ���ȸ����, ���ϰŷ���
			"2028",	"2350",	"2321", NULL };
*/
char* szHogaField[] = {
	//	���ذ�,	����ü�ᰡ��,	����ü�����,	�����,	���簡,	�����ȣ,�����,	�����,	�����ŷ���,	�����ŷ����,
		"1336",	"3111",			"3112",			"1022", "2023", "",		"2024", "2033",	"2027",		"2028",
	//	�ŵ�ȣ��1,�ŵ�����1,�ŵ�ȣ��2,�ŵ�����2,�ŵ�ȣ��3,�ŵ�����3,�ŵ�ȣ��4,�ŵ�����4,�ŵ�ȣ��5,�ŵ�����5,
		"3051",		"3041",	"3052",		"3042",	"3053",		"3043",	"3054",		"3044","3055",		"3045",
	//	�ż�ȣ��1,�ż�����1,�ż�ȣ��2,�ż�����2,�ż�ȣ��3,�ż�����3,�ż�ȣ��4,�ż�����4,�ż�ȣ��5,�ż�����5,
		"3071",		"3061",	"3072",		"3062","3073",		"3063",	"3074",		"3064",	"3075",		"3065",
	//	�ŵ��Ѽ���,�ż��Ѽ���,�ð��ܸŵ��ܷ�,�ð��ܸż��ܷ�,���Ѱ�, ���Ѱ�,	��������
		"3101",		"3106",		"3102",		"3107",			"2302",	"2303",	"3116",
	//	��,	����,	���ϰŷ���,����ü�ᰡ����������񱸺�
		"2030",	"2031",	"2321",		"", NULL};

CChartItemSHoga::CChartItemSHoga()
{
	m_bSet = FALSE;

	int nIndex = 0;
	while (szHogaField[nIndex] != NULL)
		nIndex++;

	m_nCnt = nIndex;
}

CChartItemSHoga::~CChartItemSHoga()
{

}

void CChartItemSHoga::SetInData(LPk01230In1 pData)
{
	memcpy(&m_InData, pData, sizeof(k01230In1));
	m_bSet = TRUE;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemSHoga::Convert()
{
	if(!m_bSet) return "";

	int i;
	CString strData, strCode;

	strCode = CString(m_InData.shcode, sizeof(m_InData.shcode));

	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ ������
	strData += "1301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData += sCELL;

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound
	for (i=0; i<m_nCnt; i++)
	{
		strData += szHogaField[i];
		strData += sCELL;
	}

	return strData;
}

char* CChartItemSHoga::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	k01230Out1 stOut;
	CString strNode, strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);

	memset(&stOut, 0x20, sizeof(stOut));
	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���ذ�
	strNode.Remove(_T('-'));
	memcpy(stOut.recprice, strNode, min(strNode.GetLength(), sizeof(stOut.recprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ����ü�ᰡ��
	strNode.Remove(_T('-'));
	memcpy(stOut.yeprice, strNode, min(strNode.GetLength(), sizeof(stOut.yeprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ����ü�����
	strNode.Remove(_T('-'));
	memcpy(stOut.yevolume, strNode, min(strNode.GetLength(), sizeof(stOut.yevolume)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �����
	memcpy(stOut.hname, strNode, min(strNode.GetLength(), sizeof(stOut.hname)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���簡
	strNode.Remove(_T('-'));
	memcpy(stOut.price, strNode, min(strNode.GetLength(), sizeof(stOut.price)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �����ȣ
	memcpy(stOut.sign, strNode, min(strNode.GetLength(), sizeof(stOut.sign)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �����
	memcpy(stOut.change, strNode, min(strNode.GetLength(), sizeof(stOut.change)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �����
	memcpy(stOut.rate, strNode, min(strNode.GetLength(), sizeof(stOut.rate)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �����ŷ���
	strNode.Remove(_T('-'));
	memcpy(stOut.volume, strNode, min(strNode.GetLength(), sizeof(stOut.volume)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �����ŷ����
	strNode.Remove(_T('-'));
	memcpy(stOut.trade_cost, strNode, min(strNode.GetLength(), sizeof(stOut.trade_cost)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ�ȣ��1
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho1, strNode, min(strNode.GetLength(), sizeof(stOut.offerho1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ��ܷ�1
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem1, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ�ȣ��2
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho2, strNode, min(strNode.GetLength(), sizeof(stOut.offerho2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ��ܷ�2
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem2, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ�ȣ��3
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho3, strNode, min(strNode.GetLength(), sizeof(stOut.offerho3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ��ܷ�3
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem3, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ�ȣ��4
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho4, strNode, min(strNode.GetLength(), sizeof(stOut.offerho4)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ��ܷ�4
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem4, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem4)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ�ȣ��5
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho5, strNode, min(strNode.GetLength(), sizeof(stOut.offerho5)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ŵ��ܷ�5
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem5, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem5)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż�ȣ��1
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho1, strNode, min(strNode.GetLength(), sizeof(stOut.bidho1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż��ܷ�1
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem1, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż�ȣ��2
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho2, strNode, min(strNode.GetLength(), sizeof(stOut.bidho2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż��ܷ�2
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem2, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż�ȣ��3
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho3, strNode, min(strNode.GetLength(), sizeof(stOut.bidho3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż��ܷ�3
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem3, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż�ȣ��4
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho4, strNode, min(strNode.GetLength(), sizeof(stOut.bidho4)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż��ܷ�4
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem4, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem4)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż�ȣ��5
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho5, strNode, min(strNode.GetLength(), sizeof(stOut.bidho5)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ż��ܷ�5
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem5, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem5)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �Ѹŵ��ܷ�
	strNode.Remove(_T('-'));
	memcpy(stOut.totofferrem, strNode, min(strNode.GetLength(), sizeof(stOut.totofferrem)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �Ѹż��ܷ�
	strNode.Remove(_T('-'));
	memcpy(stOut.totbidrem, strNode, min(strNode.GetLength(), sizeof(stOut.totbidrem)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ð��ܸŵ��ܷ�
	strNode.Remove(_T('-'));
	memcpy(stOut.tmofferrem, strNode, min(strNode.GetLength(), sizeof(stOut.tmofferrem)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// �ð��� �ż��ܷ�
	strNode.Remove(_T('-'));
	memcpy(stOut.tmbidrem, strNode, min(strNode.GetLength(), sizeof(stOut.tmbidrem)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���Ѱ�
	strNode.Remove(_T('-'));
	memcpy(stOut.uplmtprice, strNode, min(strNode.GetLength(), sizeof(stOut.uplmtprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���Ѱ�
	strNode.Remove(_T('-'));
	memcpy(stOut.dnlmtprice, strNode, min(strNode.GetLength(), sizeof(stOut.dnlmtprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ��������
	memcpy(stOut.yerate, strNode, min(strNode.GetLength(), sizeof(stOut.yerate)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ��
	strNode.Remove(_T('-'));
	memcpy(stOut.high, strNode, min(strNode.GetLength(), sizeof(stOut.high)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ����
	strNode.Remove(_T('-'));
	memcpy(stOut.low, strNode, min(strNode.GetLength(), sizeof(stOut.low)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ���ϰŷ���
	strNode.Remove(_T('-'));
	memcpy(stOut.volrate, strNode, min(strNode.GetLength(), sizeof(stOut.volrate)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// ����ü�ᰡ����������񱸺�
	memcpy(stOut.jnilysign, strNode, min(strNode.GetLength(), sizeof(stOut.jnilysign)));

	int nSize = sizeof(k01230Out1);

	char* pDataBuf = NULL;
	pDataBuf = new char[nSize+1];
	memset(pDataBuf, 0x00, nSize+1);
	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}
