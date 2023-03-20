// ChartItemMarket.cpp: implementation of the CChartItemMarket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemMarket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef __MULTI_LANG__
	char *szSendTypeTitle[] = {"_OrganPureBuyAccumulation_", "_OrganPureBuy_", "_SuspenseContract_", "_TotalAmount_", "_ForeignHolding_",
					"_ForeignHoldingRate_", "_ForeignPureBuy_", "_VolumeAmount_", "_KspPureBuy_Person_", "_KdqPureBuy_Person_;",
					"_K200PureBuy_Person_", "_FutPureBuy_Person_", "_COptPureBuy_Person_", "_POptPureBuy_Person_",
					"_OrganFinancePureBuy_", "_OrganInsurePureBuy_", "_OrganInvestmentPureBuy_", "_OrganBankPureBuy_", "_OrganEtcFinancePureBuy_",
					"_OrganPensionFundPureBuy_", "_OrganEtcCorpPureBuy_", "_OrganPrivateEquityPureBuy_", "_OrganCountryPureBuy_", "_OrganFinancePureBuyAccum_",
					"_OrganInsurePureBuyAccum_", "_OrganInvestmentPureBuyAccum_", "_OrganBankPureBuyAccum_", "_OrganEtcFinancePureBuyAccum_", 
					"_OrganPensionFundPureBuyAccum_", "_OrganEtcCorpPureBuyAccum_", "_OrganPrivateEquityPureBuyAccum_", "_OrganCountryPureBuyAccum_", "_ADL_","_ADR_",NULL};
#else
	char *szSendTypeTitle[] = {"������ż�����", "������ż���", "�̰�������", "�ð��Ѿ�", "�ܱ��κ�����",
					"�ܱ��κ�����", "�ܱ��μ��ż�", "�ŷ����", "�ŷ��Ҽ��ż�_����", "�ڽ��ڼ��ż�_����",
					"K200���ż�_����", "�������ż�_����", "�ݿɼǼ��ż�_����", "ǲ�ɼǼ��ż�_����",
					"�������ڼ��ż�", "������ż�", "���ż��ż�", "������ż�", "��Ÿ�������ż�", "����ݼ��ż�",
					"��Ÿ���μ��ż�", "����ݵ���ż�", "�������ż�", "�������ڴ������ż�",
					"���贩�����ż�", "���Ŵ������ż�", "���ഩ�����ż�", "��Ÿ�����������ż�", "����ݴ������ż�",
					"��Ÿ���δ������ż�", "����ݵ崩�����ż�", "�����������ż�","ADL","����ֺ���", NULL};
#endif

//char *szSendTypeFID[] = {"", "������ż���", "�̰�������", "�ð��Ѿ�", "�ܱ��κ�����",
//					"�ܱ��κ�����", "�ܱ��μ��ż�", "�ŷ����", "�ŷ��Ҽ��ż�_����", "�ڽ��ڼ��ż�_����",
//					"K200���ż�_����", "�������ż�_����", "�ݿɼǼ��ż�_����", "ǲ�ɼǼ��ż�_����", NULL};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartItemMarket::CChartItemMarket()
{
	m_bSet = FALSE;
}

CChartItemMarket::~CChartItemMarket()
{

}

void CChartItemMarket::SetInData(KB_d1015_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_d1015_InRec1));

	m_bSet = TRUE;
}

int CChartItemMarket::GetSendDataType()
{
	CString strTemp;
	strTemp = CString(m_InData.dataNm, sizeof(m_InData.dataNm));

#ifdef __MULTI_LANG__
	if (strTemp.Find(_T("_OrganPureBuyAccumulation_")) >= 0)	return TRKEY_STRMARKET_0;
	else if (strTemp.Find(_T("_OrganPureBuy_")) >= 0)			return TRKEY_STRMARKET_1;
	else if (strTemp.Find(_T("_SuspenseContract_")) >= 0)		return TRKEY_STRMARKET_2;
	else if (strTemp.Find(_T("_TotalAmount_")) >= 0)			return TRKEY_STRMARKET_3;
	else if (strTemp.Find(_T("_Foreign Holding_")) >= 0)			return TRKEY_STRMARKET_4;
	else if (strTemp.Find(_T("_Foreign Holding Rate_")) >= 0)		return TRKEY_STRMARKET_5;
	else if (strTemp.Find(_T("_Foreign Pure Buy_")) >= 0)			return TRKEY_STRMARKET_6;
	else if (strTemp.Find(_T("_ForeignPureBuyAccumulation_")) >= 0)			return TRKEY_STRMARKET_7;
	else if (strTemp.Find(_T("_Person PureBuy Accumulation_")) >= 0)		return TRKEY_STRMARKET_8;
	else if (strTemp.Find(_T("_Person PureBuy_")) >= 0)		return TRKEY_STRMARKET_9;
	//	�����ں� : IBK���� �Ⱦ� 
//	else if (strTemp.Find(_T("_KspPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_8;
//	else if (strTemp.Find(_T("_KdqPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_9;
//	else if (strTemp.Find(_T("_K200PureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_10;
//	else if (strTemp.Find(_T("_FutPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_11;
//	else if (strTemp.Find(_T("_COptPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_12;
//	else if (strTemp.Find(_T("_POptPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_13;
	// �󼼱����
	else if (strTemp.Find(_T("_OrganFinancePureBuy_")) >= 0)			return TRKEY_STRMARKET_10;
	else if (strTemp.Find(_T("_OrganInsurePureBuy_")) >= 0)				return TRKEY_STRMARKET_11;
	else if (strTemp.Find(_T("_OrganInvestmentPureBuy_")) >= 0)			return TRKEY_STRMARKET_12;
	else if (strTemp.Find(_T("_OrganBankPureBuy_")) >= 0)				return TRKEY_STRMARKET_13;
	else if (strTemp.Find(_T("_OrganEtcFinancePureBuy_")) >= 0)			return TRKEY_STRMARKET_14;
	else if (strTemp.Find(_T("_OrganPensionFundPureBuy_")) >= 0)		return TRKEY_STRMARKET_15;
	else if (strTemp.Find(_T("_OrganEtcCorpPureBuy_")) >= 0)			return TRKEY_STRMARKET_16;
	else if (strTemp.Find(_T("_OrganPrivateEquityPureBuy_")) >= 0)		return TRKEY_STRMARKET_17;
	else if (strTemp.Find(_T("_OrganCountryPureBuy_")) >= 0)			return TRKEY_STRMARKET_18;
	else if (strTemp.Find(_T("_OrganFinancePureBuyAccum_")) >= 0)		return TRKEY_STRMARKET_19;
	else if (strTemp.Find(_T("_OrganInsurePureBuyAccum_")) >= 0)		return TRKEY_STRMARKET_20;
	else if (strTemp.Find(_T("_OrganInvestmentPureBuyAccum_")) >= 0)	return TRKEY_STRMARKET_21;
	else if (strTemp.Find(_T("_OrganBankPureBuyAccum_")) >= 0)			return TRKEY_STRMARKET_22;
	else if (strTemp.Find(_T("_OrganEtcFinancePureBuyAccum_")) >= 0)	return TRKEY_STRMARKET_23;
	else if (strTemp.Find(_T("_OrganPensionFundPureBuyAccum_")) >= 0)	return TRKEY_STRMARKET_24;
	else if (strTemp.Find(_T("_OrganEtcCorpPureBuyAccum_")) >= 0)		return TRKEY_STRMARKET_25;
	else if (strTemp.Find(_T("_OrganPrivateEquityPureBuyAccum_")) >= 0)	return TRKEY_STRMARKET_26;
	else if (strTemp.Find(_T("_OrganCountryPureBuyAccum_")) >= 0)		return TRKEY_STRMARKET_27;
	else if (strTemp.Find(_T("_ADL_")) >= 0)		return TRKEY_STRUPMARKET_0;
	else if (strTemp.Find(_T("_ADR_")) >= 0)		return TRKEY_STRUPMARKET_1;
	else if (strTemp.Find(_T("_CreditBalanceRate_")) >= 0)				return TRKEY_STRMARKET_28;
	else if (strTemp.Find(_T("_CreditGivingRate_")) >= 0)				return TRKEY_STRMARKET_29;
	else if (strTemp.Find(_T("_CreditBalanceQty_")) >= 0)				return TRKEY_STRMARKET_30;
	else if (strTemp.Find(_T("_CreditBalanceVariation_")) >= 0)			return TRKEY_STRMARKET_31;
	else if (strTemp.Find(_T("_Foreign+OrganPureBuy_")) >= 0)				return TRKEY_STRMARKET_32;
	else if (strTemp.Find(_T("_Foreign+OrganPureBuyAccumulation_")) >= 0)	return TRKEY_STRMARKET_33;
	
#else
	if (strTemp.Find(_T("������ż�����")) >= 0)		return TRKEY_STRMARKET_0;
	else if (strTemp.Find(_T("������ż���")) >= 0)		return TRKEY_STRMARKET_1;
	else if (strTemp.Find(_T("�̰�������")) >= 0)		return TRKEY_STRMARKET_2;
	else if (strTemp.Find(_T("�ð��Ѿ�")) >= 0)			return TRKEY_STRMARKET_3;
	else if (strTemp.Find(_T("�ܱ��κ�����")) >= 0)		return TRKEY_STRMARKET_4;
	else if (strTemp.Find(_T("�ܱ��κ�����")) >= 0)		return TRKEY_STRMARKET_5;
	else if (strTemp.Find(_T("�ܱ��μ��ż�")) >= 0)		return TRKEY_STRMARKET_6;
	else if (strTemp.Find(_T("_ForeignPureBuyAccumulation_")) >= 0)			return TRKEY_STRMARKET_7;
	else if (strTemp.Find(_T("_Person PureBuy Accumulation_")) >= 0)		return TRKEY_STRMARKET_8;
	else if (strTemp.Find(_T("_Person PureBuy_")) >= 0)		return TRKEY_STRMARKET_9;
	//	�����ں� IBK���� �Ⱦ� 
//	else if (strTemp.Find(_T("�ŷ���")) >= 0)			return TRKEY_STRMARKET_8;
//	else if (strTemp.Find(_T("�ڽ���")) >= 0)			return TRKEY_STRMARKET_9;
//	else if (strTemp.Find(_T("K200")) >= 0)				return TRKEY_STRMARKET_10;
//	else if (strTemp.Find(_T("����")) >= 0)				return TRKEY_STRMARKET_11;
//	else if (strTemp.Find(_T("�ݿɼ�")) >= 0)			return TRKEY_STRMARKET_12;
//	else if (strTemp.Find(_T("ǲ�ɼ�")) >= 0)			return TRKEY_STRMARKET_13;
	// �󼼱����
	else if (strTemp.Find(_T("�������ڼ��ż�")) >= 0)		return TRKEY_STRMARKET_10;
	else if (strTemp.Find(_T("������ż�")) >= 0)			return TRKEY_STRMARKET_11;
	else if (strTemp.Find(_T("���ż��ż�")) >= 0)			return TRKEY_STRMARKET_12;
	else if (strTemp.Find(_T("������ż�")) >= 0)			return TRKEY_STRMARKET_13;
	else if (strTemp.Find(_T("��Ÿ�������ż�")) >= 0)		return TRKEY_STRMARKET_14;
	else if (strTemp.Find(_T("����ݼ��ż�")) >= 0)			return TRKEY_STRMARKET_15;
	else if (strTemp.Find(_T("��Ÿ���μ��ż�")) >= 0)		return TRKEY_STRMARKET_16;
	else if (strTemp.Find(_T("����ݵ���ż�")) >= 0)		return TRKEY_STRMARKET_17;
	else if (strTemp.Find(_T("�������ż�")) >= 0)			return TRKEY_STRMARKET_18;
	else if (strTemp.Find(_T("�������ڴ������ż�")) >= 0)	return TRKEY_STRMARKET_19;
	else if (strTemp.Find(_T("���贩�����ż�")) >= 0)		return TRKEY_STRMARKET_20;
	else if (strTemp.Find(_T("���Ŵ������ż�")) >= 0)		return TRKEY_STRMARKET_21;
	else if (strTemp.Find(_T("���ഩ�����ż�")) >= 0)		return TRKEY_STRMARKET_22;
	else if (strTemp.Find(_T("��Ÿ�����������ż�")) >= 0)	return TRKEY_STRMARKET_23;
	else if (strTemp.Find(_T("����ݴ������ż�")) >= 0)		return TRKEY_STRMARKET_24;
	else if (strTemp.Find(_T("��Ÿ���δ������ż�")) >= 0)	return TRKEY_STRMARKET_25;
	else if (strTemp.Find(_T("����ݵ崩�����ż�")) >= 0)	return TRKEY_STRMARKET_26;
	else if (strTemp.Find(_T("�����������ż�")) >= 0)		return TRKEY_STRMARKET_27;
	else if (strTemp.Find(_T("ADL")) >= 0)						return TRKEY_STRUPMARKET_0;
	else if (strTemp.Find(_T("����ֺ���")) >= 0)				return TRKEY_STRUPMARKET_1;
	else if (strTemp.Find(_T("�ſ��ܰ���")) >= 0)				return TRKEY_STRMARKET_28;
	else if (strTemp.Find(_T("�ſ������")) >= 0)				return TRKEY_STRMARKET_29;
	else if (strTemp.Find(_T("�ſ��ܰ����")) >= 0)				return TRKEY_STRMARKET_30;
	else if (strTemp.Find(_T("�ſ��ܰ�����")) >= 0)				return TRKEY_STRMARKET_31;
	else if (strTemp.Find(_T("�ܱ���+������ż�")) >= 0)		return TRKEY_STRMARKET_32;
	else if (strTemp.Find(_T("�ܱ���+������ż�����")) >= 0)	return TRKEY_STRMARKET_33;
#endif
	return -1;
}

CString CChartItemMarket::GetSendDataType(int nType)
{
	CString strTemp;
	strTemp.Format(_T("%s"), szSendTypeTitle[nType-TRKEY_STRMARKET_0]);

	return strTemp;
}


// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemMarket::Convert()
{
	if (!m_bSet) return "";

	CString strTemp, strGrid;
	int nCount;

	strTemp = CString(m_InData.num, sizeof(m_InData.num));
	nCount = atoi(strTemp);

	//////////////////////////////////////////////////////////////////////////
	// �׸��� ���
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));

	char szTemp[5];
	grid_i.gdir[0] = _T('1');	//	top.
	if (m_InData.nxtGb[0] == '1')	grid_i.ikey[0] = _T('2');	//	Page Down
	else							grid_i.ikey[0] = _T('0');	//	�Ϲ�(Next��ȸ).

	sprintf(szTemp, _T("%04d"), nCount);	// ��ȸ ������ ��
	memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));
	memcpy(grid_i.page, m_InData.nxtKey, 4);
	memcpy(grid_i.save, m_InData.nxtKey+4, sizeof(grid_i.save));

	grid_i.sdir[0] = _T('1');
	grid_i.vflg[0] = _T('1');
	memcpy(grid_i.vrow, _T("999"), sizeof(grid_i.vrow));

	char szGrid_i[128];
	memset(szGrid_i, 0x00, sizeof(szGrid_i));
	memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));

	strGrid.Format(_T("%s"), szGrid_i);
	//////////////////////////////////////////////////////////////////////////

	int nDataType = GetSendDataType();

	if (nDataType < 0)
		return _T("");

	return fnStructType1(strGrid, nDataType);
/*
	if (nDataType < TRKEY_STRMARKET_10)//KHD : ������ǥ 
		return fnStructType1(strGrid, nDataType);
	else
		return fnStructType2(strGrid, nDataType);
*/
}

CString CChartItemMarket::fnStructType1(CString strGrid, int nType1)
{
	if(!m_bSet) 
		return "";
	CString szDS, szFS, szCOL;
	szDS = sDS;	szFS = sCELL;	szCOL = sCOL;
	CString strData, strCode, strTemp, strDate, strNow, strUnit, strTime, strImgRgb;
	CString stNextKey, stNextData;
	int  nCount, nUnit, nType, nFlag, nMarketType;
	double dUSDollar=1;
	
	strCode	= CDataConverter::Char2String(m_InData.shcode, sizeof(m_InData.shcode));
	strDate	= CDataConverter::Char2String(m_InData.edate, sizeof(m_InData.edate));
//	strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
//	strTime	= CDataConverter::Char2String(m_InData.Itime, sizeof(m_InData.Itime));
	nCount	= CDataConverter::Char2Int(m_InData.num, sizeof(m_InData.num));
	nUnit	= CDataConverter::Char2Int(m_InData.unit, sizeof(m_InData.unit));
	nType	= CDataConverter::Char2Int(m_InData.button, sizeof(m_InData.button));	// ƽ(0)/��(1)/��(2)/��(3)/��(4)
//	strImgRgb = CDataConverter::Char2String(m_InData.ImgRgb, sizeof(m_InData.ImgRgb));
	nFlag     = CDataConverter::Char2Int(m_InData.nxtGb, sizeof(m_InData.nxtGb));	// 0:
	nMarketType  = CDataConverter::Char2Int(m_InData.type, sizeof(m_InData.type));	// ���屸��

	strUnit.Format(_T("%d"), nUnit);

	//////////////////////////////////////////////////////////////////////////
	//	Inbound Data
	
	//	�����ڵ� Input �ɺ�
	if (nMarketType == 0)
	{
		_dataH	Datah;
		FillMemory(&Datah, sz_DATAH, 0x20);
		strData += "1301";	strData += sDS;	strData += strCode;	strData += sCELL;//KHD sFS;
		CString stRowData;
		
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?5500");
		
		//NEXTKEY : KHD
		if (nFlag == 0)
		{
			stNextData = "99999999999999";
			memcpy(m_InData.nxtKey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
		}
		
		stNextKey = "5324";
		stNextData.Format("%8.8s",m_InData.nxtKey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		
		//////////////////////////////////////////////////////////////////////////
		//	OutBound Data
		strData += "1301";
		strData += sCELL;//KHD sFS;
		
		strData += stNextKey;
		strData += sCELL;
		
		// Row Data ����
		strData += stRowData;
		strData += sDS;
		
		CString temp;
		//��ȸ������ �� 
		int GrpHLen = 0;
		temp.Format("%06d", nCount);
		CopyMemory(Datah.count, temp, temp.GetLength());
		Datah.dkey = 0 + 0x30;
		
		CopyMemory(Datah.pday, strDate, strDate.GetLength());
		
		temp.Format("%0*d", sizeof(Datah.lgap), nUnit);// N�к� 
		CopyMemory(Datah.lgap, temp, temp.GetLength() );
		
		char szData_i[sz_DATAH];
		memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
		CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
		szData_i[sz_DATAH-1] = 0x00; // KHD : �ǳ��� 0x00���ڸ� ���� �ִ´�.
		strData += szData_i;
	}
	else if (nMarketType == 1)
	{
		_dataH	Datah;
		FillMemory(&Datah, sz_DATAH, 0x20);
		strData += "21301";	strData += sDS;	strData += strCode;	strData += sCELL;//KHD sFS;
		CString stRowData;
		
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?25500");
		
		//NEXTKEY : KHD
		if (nFlag == 0)
		{
			stNextData = "99999999999999";
			memcpy(m_InData.nxtKey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
		}
		
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.nxtKey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		
		//////////////////////////////////////////////////////////////////////////
		//	OutBound Data
		strData += "21301";
		strData += sCELL;//KHD sFS;
		
		strData += stNextKey;
		strData += sCELL;
		
		// Row Data ����
		strData += stRowData;
		strData += sDS;
		
		CString temp;
		//��ȸ������ �� 
		int GrpHLen = 0;
		temp.Format("%06d", nCount);
		CopyMemory(Datah.count, temp, temp.GetLength());
		Datah.dkey = 0 + 0x30;
		
		CopyMemory(Datah.pday, strDate, strDate.GetLength());
		
		temp.Format("%0*d", sizeof(Datah.lgap), nUnit);// N�к� 
		CopyMemory(Datah.lgap, temp, temp.GetLength() );
		
		char szData_i[sz_DATAH];
		memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
		CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
		szData_i[sz_DATAH-1] = 0x00; // KHD : �ǳ��� 0x00���ڸ� ���� �ִ´�.
		strData += szData_i;
	}

	switch (nType1)
	{
	case TRKEY_STRMARKET_0:		//	����������ż� : KHD
		strData += "5302";		strData += sCOL;
		strData += "5512";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_1:		//	������ż� : KHD
		strData += "5302";		strData += sCOL;
		strData += "5412";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_2:		//	�̰������� : KHD : ������ �����Ǿ��� 
		strData += "5302";		strData += sCOL;
		strData += "35201";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_3:		//	�ð��Ѿ� : KHD : �Ⱦ� 
		strData += "5302";		strData += sCOL;
		strData += "5363";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_4:		//	�ܱ��κ����� : KHD
		strData += "5302";		strData += sCOL;
		strData += "5363";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_5:		//	�ܱ��κ�����
		strData += "5302";		strData += sCOL;
		strData += "5373";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_6:		//	�ܱ��μ��ż�
		strData += "5302";		strData += sCOL;
		strData += "5411";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_7:		//	�ܱ��δ������ż�
		strData += "5302";		strData += sCOL;
		strData += "5511";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_8:		//	���δ������ż�
		strData += "5302";		strData += sCOL;
		strData += "5508";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_9:		//	���μ��ż�
		strData += "5302";		strData += sCOL;
		strData += "5408";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_10:	//	�������ڼ��ż�
		strData += "5302";		strData += sCOL;
		strData += "5401";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_11:	//	������ż�
		strData += "5302";		strData += sCOL;
		strData += "5402";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_12:	//	���ż��ż�
		strData += "5302";		strData += sCOL;
		strData += "5403";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_13:	//	������ż�
		strData += "5302";		strData += sCOL;
		strData += "5404";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_14:	//	��Ÿ�������ż�
		strData += "5302";		strData += sCOL;
		strData += "5405";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_15:	//	����ݼ��ż�
		strData += "5302";		strData += sCOL;
		strData += "5406";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_16:	//	��Ÿ���μ��ż�
		strData += "5302";		strData += sCOL;
		strData += "5407";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_17:	//	����ݵ���ż�
		strData += "5302";		strData += sCOL;
		strData += "5409";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_18:	//	�������ż�
		strData += "5302";		strData += sCOL;
		strData += "5413";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_19:	//	�������ڴ������ż�
		strData += "5302";		strData += sCOL;
		strData += "5501";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_20:	//	���贩�����ż�
		strData += "5302";		strData += sCOL;
		strData += "5502";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_21:	//	���Ŵ������ż�
		strData += "5302";		strData += sCOL;
		strData += "5503";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_22:	//	���ഩ�����ż� 
		strData += "5302";		strData += sCOL;
		strData += "5504";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_23:	//	��Ÿ�����������ż�  
		strData += "5302";		strData += sCOL;
		strData += "5505";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_24:	//	����ݴ������ż�   
		strData += "5302";		strData += sCOL;
		strData += "5506";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_25:	//	��Ÿ���δ������ż�
		strData += "5302";		strData += sCOL;
		strData += "5507";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_26:	//	����ݵ崩�����ż�
		strData += "5302";		strData += sCOL;
		strData += "5509";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_27:	//	�����������ż�
		strData += "5302";		strData += sCOL;
		strData += "5513";		strData += sCOL;
		break;
	case TRKEY_STRUPMARKET_0:	//	ADL
		strData += "25302";		strData += sCOL;
		strData += "25602";		strData += sCOL;
		break;
	case TRKEY_STRUPMARKET_1:	//	ADR
		strData += "25302";		strData += sCOL;
		strData += "25603";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_28:	//	�ſ��ܰ���
		strData += "5302";		strData += sCOL;
		strData += "5601";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_29:	//	�ſ������
		strData += "5302";		strData += sCOL;
		strData += "5602";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_30:	//	�ſ��ܰ����
		strData += "5302";		strData += sCOL;
		strData += "5603";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_31:	//	�ſ��ܰ�����
		strData += "5302";		strData += sCOL;
		strData += "5604";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_32:	//	�ܱ���+������ż�
		strData += "5302";		strData += sCOL;
		strData += "5414";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_33:	//	�ܱ���+������ż�����
		strData += "5302";		strData += sCOL;
		strData += "5514";		strData += sCOL;
		break;
	}
	
	strData += szFS;
	return strData;
	

/*	CString strData, strCode, strTemp, strDate;
	CString szDS, szFS, szCOL;
	szDS = sDS;	szFS = sCELL;	szCOL = sCOL;

	strCode = CString(m_InData.shcode, sizeof(m_InData.shcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strDate = CString(m_InData.edate, sizeof(m_InData.edate));
	strDate.TrimLeft(); strDate.TrimRight();

	int nCount	= CDataConverter::Char2Int(m_InData.num, sizeof(m_InData.num));
	CString stCount;
	stCount.Format("%06d", nCount);
	switch (nType)
	{
	case TRKEY_STRMARKET_0:		//	������ż�����
		//	Input �ɺ�	---------->>
		strData += "1301" + szDS + strCode + szFS;		// �����ڵ� 
		strData += "101306" + szDS + strDate + szFS;	// ��ȸ����
		//	output �ɺ�	---------->>
		strData += "1301" + szFS;						// �����ڵ� 
		strData += "$101810" + szDS;					// �׸��� �ɺ�.
		strData += strGrid;								// �׸��� ���.
		// Row Data ����
		strData += "101302" + szCOL;					//	����
		strData += "101390" + szCOL;					//	�������ż�
		break;
	case TRKEY_STRMARKET_1:		//	������ż���
		strData += "1301"	+ szDS + strCode + szFS;	// �����ڵ� 
		strData += "101306" + szDS + strDate + szFS;	// ��ȸ����
		strData += "1301" + szFS;						// �����ڵ� 
		strData += "$101810" + szDS;					// �׸��� �ɺ�.
		strData += strGrid;								// �׸��� ���.
		strData += "101302" + szCOL;					//	����
		strData += "101391" + szCOL;					//	���ż�
		break;
	case TRKEY_STRMARKET_2:		//	�̰�������
		{
			CStringArray dataArr;
			dataArr.Add(strCode);
			dataArr.Add(strDate);
			dataArr.Add(strGrid);
				
			MakeDataOfSuspensSeContract(strData, &dataArr);
		}
// 		strData += "30301"	+ szDS + strCode + szFS;	// �����ڵ� 
// 		strData += "103306" + szDS + strDate + szFS;	// ��ȸ����
// 		strData += "103308" + szDS + "1" + szFS;		// ���ֿ� ����.
// 		strData += "30301" + szFS;						// �����ڵ� 
// 		strData += "$103610" + szDS;					// �׸��� �ɺ�.
// 		strData += strGrid;								// �׸��� ���.
// 		strData += "103302" + szCOL;					//	����
// 		strData += "103201" + szCOL;					//	�̰��� ����
		break;
	case TRKEY_STRMARKET_3:		//	�ð��Ѿ�
		strData += "1301"	+ szDS + strCode + szFS;	// �����ڵ� 
		strData += "101306" + szDS + strDate + szFS;	// ��ȸ����
		strData += "1301" + szFS;						// �����ڵ� 
		strData += "?5500" + szDS;					// �׸��� �ɺ�.
		strData += strGrid;								// �׸��� ���.
		strData += "101302" + szCOL;					//	����
		strData += "101305" + szCOL;					//	�ð��Ѿ�
		break;
	case TRKEY_STRMARKET_4:		//	�ܱ��κ����� : KHD
		strData += "1301"	+ szDS + strCode + szFS;	// �����ڵ� 
		strData += "?5500" + szDS + stCount ;		// �׸��� �ɺ�.
		strData += strDate + szFS;	// ��ȸ����
		strData += "1301" + szFS;						// �����ڵ� 
		
	//	strData += strGrid;								// �׸��� ���.
		strData += "5302" + szCOL;					//	����
		strData += "5363" + szCOL;					//	������
		break;
	case TRKEY_STRMARKET_5:		//	�ܱ��κ�����
		strData += "5306"	+ szDS + strCode + szFS;	// �����ڵ� 
		strData += "101306" + szDS + strDate + szFS;	// ��ȸ����
		strData += "1301" + szFS;						// �����ڵ� 
		strData += "$101710" + szDS;					// �׸��� �ɺ�.
		strData += strGrid;								// �׸��� ���.
		strData += "101302" + szCOL;					//	����
		strData += "101367" + szCOL;					//	��������
		break;
	case TRKEY_STRMARKET_6:		//	�ܱ��μ��ż�
		strData += "1301"	+ szDS + strCode + szFS;	// �����ڵ� 
		strData += "101306" + szDS + strDate + szFS;	// ��ȸ����
		strData += "1301" + szFS;						// �����ڵ� 
		strData += "$101710" + szDS;					// �׸��� �ɺ�.
		strData += strGrid;								// �׸��� ���.
		strData += "101302" + szCOL;					//	����
		strData += "101364" + szCOL;					//	���ż�
		break;
	case TRKEY_STRMARKET_7:		//	�ŷ����
		strData += "1301"	+ szDS + strCode + szFS;	// �����ڵ� 
		strData += "101306" + szDS + strDate + szFS;	// ��ȸ����
		strData += "1301" + szFS;						// �����ڵ� 
		strData += "$101610" + szDS;					// �׸��� �ɺ�.
		strData += strGrid;								// �׸��� ���.
		strData += "101302" + szCOL;					//	����
		strData += "101028" + szCOL;					//	�ŷ����
		break;
	}
*/
//	strData += szFS;
//	return strData;
}

CString CChartItemMarket::fnStructType2(CString strGrid, int nType)
{
	CString strData, strCode, strMarket, strDate;
	CString szDS, szFS, szCOL;
	szDS = sDS;	szFS = sCELL;	szCOL = sCOL;

	strCode = CString(m_InData.shcode, sizeof(m_InData.shcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strDate = CString(m_InData.edate, sizeof(m_InData.edate));
	strDate.TrimLeft(); strDate.TrimRight();

	switch (nType)
	{
	case TRKEY_STRMARKET_8:		strMarket = "001";	break;	//	�����ں��ŷ���
	case TRKEY_STRMARKET_9:		strMarket = "201";	break;	//	�����ں��ڽ���
	case TRKEY_STRMARKET_10:	strMarket = "101";	break;	//	�����ں�K200
	case TRKEY_STRMARKET_11:	strMarket = "9101";	break;	//	�����ں�����
	case TRKEY_STRMARKET_12:	strMarket = "9201";	break;	//	�����ں��ݿɼ�
	case TRKEY_STRMARKET_13:	strMarket = "9301";	break;	//	�����ں�ǲ�ɼ�
	}

	//	Input �ɺ�	---------->>
	strData += "105301"	+ szDS + strMarket + szFS;		// �����ڵ�
	strData += "105306" + szDS + strDate + szFS;		// ��ȸ����
	strData += "105309" + szDS + "5" + szFS;			// �ŵ��� ����(5)/���(6)
	//	output �ɺ�	---------->>
	strData += "105301" + szFS;							// �����ڵ� 
	strData += "$105610" + szDS;						// �׸��� �ɺ�.
	strData += strGrid;									// �׸��� ���.
	// Row Data ����
	strData += "105302" + szCOL;						//	����
	strData += "105318" + szCOL;						//	���� ���ż�
	strData += "105319" + szCOL;						//	�ܱ��� ���ż�
	strData += "105310" + szCOL;						//	����� ���ż�

	strData += szFS;

	return strData;
}

char* CChartItemMarket::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{	
	char *pszBuff = NULL;
	int nIndex = 0, nSize=0;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode;
	CChartBozoManager bojoMng;
	CChartBozoInput bojoInput;
	CStringArray arrDate, arrSel1, arrSel2, arrSel3;

	CString strNm1, strNm2, strNm3, strNm4;
	CString strDataNm, strData;
	strDataNm.Format(_T("%.*s"), sizeof(m_InData.dataNm), m_InData.dataNm);
	int nFind = strDataNm.Find(_T(";"));
	if (nFind)
	{
		strNm1 = strDataNm.Left(nFind);
		strDataNm = strDataNm.Mid(nFind+1);
	}
	nFind = strDataNm.Find(_T(";"));
	if (nFind)
	{
		strNm2 = strDataNm.Left(nFind);
		strDataNm = strDataNm.Mid(nFind+1);
	}
	nFind = strDataNm.Find(_T(";"));
	if (nFind)
	{
		strNm3 = strDataNm.Left(nFind);
		strDataNm = strDataNm.Mid(nFind+1);
	}
	nFind = strDataNm.Find(_T(";"));
	if (nFind)
		strNm4 = strDataNm.Left(nFind);
//	strSrc = strSrc.Mid(136);
	if (nKey <= TRKEY_STRMARKET_END)
	{
		strCode = CDataConverter::Parser(strSrc, sCELL);	// �����ڵ�

		//KHD : NextKey
		CString strNextKey = CDataConverter::Parser(strSrc, sCELL);

		// �׸��� ���
		GRAPH_IO gridOut;
		memcpy(&gridOut, (LPSTR)(LPCTSTR)strSrc, sizeof(gridOut));
		strSrc = strSrc.Mid(sizeof(gridOut));
		strSrc.TrimLeft(); strSrc.TrimRight();

		while (strSrc.IsEmpty() == FALSE)
		{
			strNode = CDataConverter::Parser(strSrc, sCELL);	// ����
			arrDate.Add(strNode);

			if (strSrc.IsEmpty() || strNode.GetLength() < 3)	break;
			strNode = CDataConverter::Parser(strSrc, sROW);		// �������ż�
			arrSel1.Add(strNode);

			nIndex++;
		}

		nDataLen = sizeof(KB_d1015_OutRec1) + nIndex * sizeof(KB_d1015_OutRec2_1);
		pszBuff = new char[nDataLen+1];
		memset(pszBuff, 0x20, nDataLen);
		pszBuff[nDataLen] = 0x00;

		KB_d1015_OutRec_1 *pstOut = NULL;
		pstOut = (KB_d1015_OutRec_1 *)pszBuff;

		memcpy(pstOut->winid, m_InData.winid, sizeof(m_InData.winid));
		memcpy(pstOut->dataNm, m_InData.dataNm, sizeof(m_InData.dataNm));

		if (strNextKey !="99999999999999")// 9�� 14�ڸ��� ����Ű�� ���°�.  
			memcpy(pstOut->nxtKey, (LPCTSTR)strNextKey, min(sizeof(pstOut->nxtKey),strNextKey.GetLength()));
		else
			memset(pstOut->nxtKey, 0x20, sizeof(pstOut->nxtKey));

		pstOut->inNumGb[0] = '2';

		memcpy(pstOut->shcode, strCode, min(sizeof(pstOut->shcode), strCode.GetLength()));
		bojoInput.m_sShcode = strCode;

		strNode.Format(_T("%05d"), nIndex);
		memcpy(pstOut->d1015_CHART.chtCnt, strNode, 5);
		int ii= nIndex-1;
		for (int i=0; i<nIndex; i++)
//		for(int i = nIndex-1; i>=0 ; i--)
		{
			KB_d1015_OutRec2_1* pRow = (KB_d1015_OutRec2_1*)&pstOut->d1015_CHART.chart[ii];

			strNode = arrDate.GetAt(i);
			memcpy(pRow->date, strNode, min(sizeof(pRow->date), strNode.GetLength()));

			strNode = arrSel1.GetAt(i);
			if (strNm1.Find(_T("_Foreign Holding Rate___DateTime_")) >= 0 ||
				strNm1.Find(_T("_CreditBalanceRate___DateTime_")) >= 0 ||
				strNm1.Find(_T("_CreditGivingRate___DateTime_")) >= 0)
			{
				double fNode = atoi(strNode)/100.00;
				strNode.Format("%f",fNode);
			}
// 			else
// 			{
// 				strNode+="000";
// 			}
			memcpy(pRow->data1, strNode, min(sizeof(pRow->data1), strNode.GetLength()));
			ii--;
//			OutputDebugString("KHD : ������ = " + strNode );
		}

		bojoInput.m_sDataType = _T("0");		//	����. ���� �ʿ�� ����.
		bojoInput.m_sMarket = _T("1");			//	����. ���� �ʿ�� ����.

		char szKey[5];
//		memset(szKey, 0x00, sizeof(szKey));
//		memcpy(szKey, gridOut.page, sizeof(gridOut.page));
		if (m_InData.nxtGb[0] != _T('0'))
		{
			bojoInput.m_sNxtFlg = _T("1");
			bojoInput.sNxtFlg[0] = _T('1');
		}
		else
		{
			bojoInput.m_sNxtFlg = _T("0");
			bojoInput.sNxtFlg[0] = _T('0');
		}

		bojoInput.m_sTerm = _T("2");				//	��,��,��
		bojoInput.m_sTick = _T("1");

		CString strBozo;
		memset(szKey, 0x00, sizeof(szKey));
		sprintf(szKey, _T("%03d"), 133); // 133�� ��ǥ

		nSize = bojoMng.GetChartBozoMsg(szKey, &bojoInput, strNode);
		if (strNm1.Find(_T("_Foreign Holding Rate___DateTime_")) >= 0 ||
			strNm1.Find(_T("_CreditBalanceRate___DateTime_")) >= 0 ||
			strNm1.Find(_T("_CreditGivingRate___DateTime_")) >= 0)
			strData.Format(_T("%.*s:8:YYYYMMDD|%.*s:15:x0.01"), strNm1.GetLength(), strNm1, strNm2.GetLength(), strNm2);
//			strData.Format(_T("�ڷ�����:8:YYYYMMDD|%.*s:15:x0.01"), strNm2.GetLength(), strNm2);
		else
			strData.Format(_T("%.*s:8:YYYYMMDD|%.*s:15:x1"), strNm1.GetLength(), strNm1, strNm2.GetLength(), strNm2);
//			strData.Format(_T("�ڷ�����:8:YYYYMMDD|%.*s:15:x1"), strNm2.GetLength(), strNm2);

		CChartBozoItem *pItem = NULL;
		List_CChartBozoItem arr;
//		if (m_InData.nxtGb[0] == _T('0'))
			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("NEWPACKET"), strData);
//		else
//			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("RESETPACKET"), strData);

		arr.AddTail(pItem);

		nSize = bojoMng.ChangeChartBozoMsg(strNode, &arr);

		memcpy(pstOut->d1015_CHART.chtBjM, (LPSTR)(LPCTSTR)strNode, nSize);		//	�����޽���
		memcpy(pstOut->d1015_CHART.chtBjM, "0396", 4);

		delete pItem;
		pItem = NULL;
		arr.RemoveAll();
	}
	else
	{
		strCode = CDataConverter::Parser(strSrc, sFS);	// �����ڵ�

		//KHD : NextKey
		CString strNextKey = CDataConverter::Parser(strSrc, sCELL);

		// �׸��� ���
		GRAPH_IO gridOut;
		memcpy(&gridOut, (LPSTR)(LPCTSTR)strSrc, sizeof(gridOut));
		strSrc = strSrc.Mid(sizeof(gridOut));

		while (strSrc.IsEmpty() == FALSE)
		{
			strNode = CDataConverter::Parser(strSrc, sCELL);	// ����
			arrDate.Add(strNode);

			if (strSrc.IsEmpty())	break;
			strNode = CDataConverter::Parser(strSrc, sCELL);		// �������ż�
			arrSel1.Add(strNode);

			if (strSrc.IsEmpty())	break;
			strNode = CDataConverter::Parser(strSrc, sCELL);		// �������ż�
			arrSel2.Add(strNode);

			if (strSrc.IsEmpty())	break;
			strNode = CDataConverter::Parser(strSrc, sROW);		// �������ż�
			arrSel3.Add(strNode);

			nIndex++;
		}

		nDataLen = sizeof(KB_d1015_OutRec1) + nIndex * sizeof(KB_d1015_OutRec2_3);
		pszBuff = new char[nDataLen+1];
		memset(pszBuff, 0x20, nDataLen);
		pszBuff[nDataLen] = 0x00;

		KB_d1015_OutRec_3 *pstOut = NULL;
		pstOut = (KB_d1015_OutRec_3 *)pszBuff;

		memcpy(pstOut->winid, m_InData.winid, sizeof(m_InData.winid));
		memcpy(pstOut->dataNm, m_InData.dataNm, sizeof(m_InData.dataNm));

		if (strNextKey !="99999999999999")// 9�� 14�ڸ��� ����Ű�� ���°�.  
			memcpy(pstOut->nxtKey, (LPCTSTR)strNextKey, min(sizeof(pstOut->nxtKey),strNextKey.GetLength()));
		else
			memset(pstOut->nxtKey, 0x20, sizeof(pstOut->nxtKey));

		pstOut->inNumGb[0] = '4';

		memcpy(pstOut->shcode, strCode, min(sizeof(pstOut->shcode), strCode.GetLength()));
		bojoInput.m_sShcode = strCode;

		strNode.Format(_T("%05d"), nIndex);
		memcpy(pstOut->d1015_CHART.chtCnt, strNode, 5);

		for (int i=0; i<nIndex; i++)
		{
			KB_d1015_OutRec2_3* pRow = (KB_d1015_OutRec2_3*)&pstOut->d1015_CHART.chart[i];

			strNode = arrDate.GetAt(i);
			memcpy(pRow->date, strNode, min(sizeof(pRow->date), strNode.GetLength()));

			strNode = arrSel1.GetAt(i);
			memcpy(pRow->data1, strNode, min(sizeof(pRow->data1), strNode.GetLength()));

			strNode = arrSel2.GetAt(i);
			memcpy(pRow->data2, strNode, min(sizeof(pRow->data2), strNode.GetLength()));

			strNode = arrSel3.GetAt(i);
			memcpy(pRow->data3, strNode, min(sizeof(pRow->data3), strNode.GetLength()));
		}

		bojoInput.m_sDataType = _T("0");		//	����. ���� �ʿ�� ����.
		bojoInput.m_sMarket = _T("1");			//	����. ���� �ʿ�� ����.

		char szKey[5];
	//	memset(szKey, 0x00, sizeof(szKey));
	//	memcpy(szKey, gridOut.page, sizeof(gridOut.page));
		if (m_InData.nxtGb[0] != _T('0'))
		{
			bojoInput.m_sNxtFlg = _T("1");
			bojoInput.sNxtFlg[0] = _T('1');
		}
		else
		{
			bojoInput.m_sNxtFlg = _T("0");
			bojoInput.sNxtFlg[0] = _T('0');
		}

		bojoInput.m_sTerm = _T("2");				//	��,��,��
		bojoInput.m_sTick = _T("1");

		CString strBozo;
		memset(szKey, 0x00, sizeof(szKey));
		sprintf(szKey, _T("%03d"), 133); // 133�� ��ǥ

		nSize = bojoMng.GetChartBozoMsg(szKey, &bojoInput, strNode);
		strData.Format(_T("%.*s:8:YYYYMMDD|%.*s:15:x1|%.*s:15:x1|%.*s:15:x1"),
					strNm1.GetLength(), strNm1, strNm2.GetLength(), strNm2,
					strNm3.GetLength(), strNm3, strNm4.GetLength(), strNm4);
//		strData.Format(_T("�ڷ�����:8:YYYYMMDD|%.*s:15:x1|%.*s:15:x1|%.*s:15:x1"),
//				strNm2.GetLength(), strNm2, strNm3.GetLength(), strNm3, strNm4.GetLength(), strNm4);

		CChartBozoItem *pItem = NULL;
		List_CChartBozoItem arr;
//		if (m_InData.nxtGb[0] == _T('0'))
			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("NEWPACKET"), strData);
//		else
//			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("RESETPACKET"), strData);

		arr.AddTail(pItem);

		nSize = bojoMng.ChangeChartBozoMsg(strNode, &arr);

		memcpy(pstOut->d1015_CHART.chtBjM, (LPSTR)(LPCTSTR)strNode, nSize);		//	�����޽���
		memcpy(pstOut->d1015_CHART.chtBjM, "0396", 4);

		delete pItem;
		pItem = NULL;
		arr.RemoveAll();
	}

	return pszBuff;
}

//nDataType 1(��), 
void CChartItemMarket::MakeDataOfSuspensSeContract(CString &strData, CStringArray* pDataArr) 
{
	CString strCode = pDataArr->GetAt(0);
	CString strDate = pDataArr->GetAt(1);
	CString strGrid = pDataArr->GetAt(2);

	CString szDS, szFS, szCOL;
	szDS = sDS;	szFS = sFS;	szCOL = sCOL;

	char cDataType = m_InData.button[0];
	char aUnit[3+1] = {0,};
	memcpy(aUnit, m_InData.unit, 3);

	// cDataType : ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(6)
	switch(cDataType)
	{
		case '2':	//��
		default:
			strData += "30301"	+ szDS + strCode + szFS;	// �����ڵ� 
			strData += "103306" + szDS + strDate + szFS;	// ��ȸ����
			strData += "103308" + szDS + "1" + szFS;		// ���ֿ� ����.
			strData += "$103610" + szDS;					// �׸��� �ɺ�.
			strData += strGrid;								// �׸��� ���.
			strData += "103302" + szCOL;					//	����
			strData += "103201" + szCOL;					//	�̰��� ����
			break;

/*		case '0':		//ƽ
			strData += "30301" + szDS + strCode + szFS;		// �����ڵ�
			strData += "103304" + szDS + strDate + szFS;	// ���ؽð�
			strData += "103306" + szDS + strDate + szFS;	// ��ȸ����
			strData += "103307" + szDS + "1" + szFS;		// ���ϱ��� 1���� 0������
			//strData += "103308" + szDS + "1" + szFS;		// ��.
			strData += "103308" + szDS + aUnit + szFS;		// ��.
			strData += "30301" + szFS;						// �����ڵ�
			strData += "$103410" + szDS;					// �׸��� �ɺ�.
			strData += strGrid;								// �׸��� ���.
			strData += "103302" + szCOL;					// ����
			strData += "103034" + szCOL;					// �ð�
			strData += "103201" + szCOL;					// �̰��� ����
			break;

		case '1':		//��
			strData += "30301" + szDS + strCode + szFS;		// �����ڵ�
			strData += "103304" + szDS + strDate + szFS;	// ���ؽð�
			strData += "103306" + szDS + strDate + szFS;	// ��ȸ����
			strData += "103307" + szDS + "1" + szFS;		// ���ϱ��� 1���� 0�����Ϻ���, 2�����ϸ�
			//strData += "103308" + szDS + "1" + szFS;		// ��.
			strData += "103308" + szDS + aUnit + szFS;		// ��.
			strData += "30301" + szFS;						// �����ڵ�
			strData += "$103510" + szDS;					// �׸��� �ɺ�.
			strData += strGrid;								// �׸��� ���.
			strData += "103302" + szCOL;					// ����
			strData += "103034" + szCOL;					// �ð�
			strData += "103201" + szCOL;					// �̰��� ����
			break;

		case '6':		//��
			strData += "30301" + szDS + strCode + szFS;		// �����ڵ�
			strData += "103304" + szDS + "1" + szFS;		// ��û���ڵ�
			strData += "103307" + szDS + "1" + szFS;		// ���ϱ��� �Ⱦ�
			//strData += "103308" + szDS + "1" + szFS;		// ��.
			strData += "103308" + szDS + aUnit + szFS;		// ��.
			strData += "30301" + szFS;						// �����ڵ�
			strData += "$103511" + szDS;					// �׸��� �ɺ�.
			strData += strGrid;								// �׸��� ���.
			strData += "103302" + szCOL;					// ����
			strData += "103034" + szCOL;					// �ð�
			strData += "103201" + szCOL;					// �̰��� ����
			break;
*/
	}
}