#include "stdafx.h"
#include "./ChartRealConverter_GNF.h"
#include "../CommonTR/EUREAL_KXH3.h"	//�ֽ�
#include "../CommonTR/EUREAL_IXIJ.h"	//����
#include "../CommonTR/EUREAL_FFC0.h"	//����
#include "../CommonTR/EUREAL_OOC0.h"	//�ɼ�
#include "../CommonTR/EUREAL_HWFJ.h"	//�ؿ�

//#include "../CommonTR/REAL_GNF_STOCK_SISE.h"
//#include "../CommonTR/REAL_GNF_STOCK_HOGA.h"
#include "../CommonTR/REALSTOCK_HOGA_GNF.h"
#include "../CommonTR/REALSTOCK_SISE_GNF.h"

// --> [Edit]  ������ 2008/09/10		( ����� ������... ��.�� �׳� ���Լ� �Ⱥθ��� DRDS���� �ٷ� ó����...  )
//#include "../CommonTR/FXREAL_a.h"		// FX �ü� ����

// --------------------------------------------------------------------------------------
// FX�ü�
BOOL CChartRealConverter::Convert_a(LPCSTR pRealStruct, CRealData_Common& realData_Common, short nTime)
{
//	stFXREAL_a* pInput = (stFXREAL_a*)pRealStruct;
//	char sTmp[20+1];
//	CString strTemp = "";
//
//	memcpy(sTmp, pInput->shcode, sizeof(pInput->shcode));	/*  [�����ڵ�]				*/
//	sTmp[sizeof(pInput->shcode)]=0;
//	realData_Common.m_strCode = sTmp;
//	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
//	
////	if ( nTime == 0 )	//	����
////	{
////		memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [ü��ð�]				*/
////		sTmp[sizeof(pInput->chetime)]=0;
////		realData_Common.m_strTime = sTmp;
////		realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
////	}
////	else				//	�ð�
//	{
//		memcpy(sTmp, pInput->real_time, sizeof(pInput->real_time));	/*  [ü��ð�]				*/
//		sTmp[sizeof(pInput->real_time)]=0;
//		realData_Common.m_strTime = sTmp;
//		realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
//	}
//	
//	memcpy(sTmp, pInput->bid_sign, sizeof(pInput->bid_sign));		/*  [1:����]				*/
//	sTmp[sizeof(pInput->bid_sign)]=0;
//	realData_Common.m_strSign = sTmp;
//	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();
//
//	memcpy(sTmp, pInput->bidclose_price, sizeof(pInput->bidclose_price));		/*  [���簡,     ��]		*/
//	sTmp[sizeof(pInput->bidclose_price)]=0;
//	realData_Common.m_strPrice = sTmp;
//	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();
//
//	memcpy(sTmp, pInput->bid_change_price, sizeof(pInput->bid_change_price));	/*  [���ϴ��,   ��]		*/
//	sTmp[sizeof(pInput->bid_change_price)]=0;
//	realData_Common.m_strChange = sTmp;
//	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();
//
////	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [�����]				*/
////	sTmp[sizeof(pInput->drate)]=0;
//	realData_Common.m_strChrate = "";
//	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();
//
//	strTemp.Format("%s", pInput->bid_volume);
////	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [�����ŷ���, ��]		*/
////	sTmp[sizeof(pInput->volume)]=0;
////	realData_Common.m_strVolume = sTmp;
//	realData_Common.m_strVolume = strTemp;
//	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();
//
//	memcpy(sTmp, pInput->bidopen_price, sizeof(pInput->bidopen_price));		/*  [�ð�,       ��]		*/
//	sTmp[sizeof(pInput->bidopen_price)]=0;
//	realData_Common.m_strOpen = sTmp;
//	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();
//
//	memcpy(sTmp, pInput->bidhigh_price, sizeof(pInput->bidhigh_price));		/*  [��,       ��]		*/
//	sTmp[sizeof(pInput->bidhigh_price)]=0;
//	realData_Common.m_strHigh = sTmp;
//	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();
//
//	memcpy(sTmp, pInput->bidlow_price, sizeof(pInput->bidlow_price));			/*  [����,       ��]		*/
//	sTmp[sizeof(pInput->bidlow_price)]=0;
//	realData_Common.m_strLow = sTmp;
//	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	return TRUE;
}
// <-- [Edit]  ������ 2008/09/10

// --------------------------------------------------------------------------------------
// �ֽ�
BOOL CChartRealConverter::Convert_KXS3(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_KXH3* pInput = (stEUREAL_KXH3*)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kshcode, sizeof(pInput->kshcode));	/*  [�����ڵ�]				*/
	sTmp[sizeof(pInput->kshcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [ü��ð�]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:����]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [���簡,     ��]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [���ϴ��,   ��]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [�����]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [�����ŷ���, ��]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [�ð�,       ��]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [��,       ��]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [����,       ��]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	return TRUE;
}

// --------------------------------------------------------------------------------------
// ����
BOOL CChartRealConverter::Convert_IXIJ(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_IXIJ* pInput = (stEUREAL_IXIJ*)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kupcode, sizeof(pInput->kupcode));		/*  [�����ڵ�]			*/
	sTmp[sizeof(pInput->kupcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->time, sizeof(pInput->time));			/*  [�ð�]				*/
	sTmp[sizeof(pInput->time)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));			/*  [2:��� 3:���� 5:�϶�]	*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->jisu, sizeof(pInput->jisu));			/*  [����]				*/
	sTmp[sizeof(pInput->jisu)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));		/*  [���Ϻ�]			*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));			/*  [�����]			*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));		/*  [�ŷ���]			*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->openjisu, sizeof(pInput->openjisu));	/*  [�ð�����]			*/
	sTmp[sizeof(pInput->openjisu)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->highjisu, sizeof(pInput->highjisu));	/*  [������]			*/
	sTmp[sizeof(pInput->highjisu)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->lowjisu, sizeof(pInput->lowjisu));		/*  [��������]			*/
	sTmp[sizeof(pInput->lowjisu)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	return TRUE;
}

// --------------------------------------------------------------------------------------
// ����
BOOL CChartRealConverter::Convert_FFC0(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_FFC0* pInput = (stEUREAL_FFC0*)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kfutcode, sizeof(pInput->kfutcode));	/*  [���������ڵ�]				*/
	sTmp[sizeof(pInput->kfutcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [ü��ð�]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:����]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [���簡,     ��]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [���ϴ��,   ��]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [�����]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [�����ŷ���, ��]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [�ð�,       ��]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [��,       ��]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [����,       ��]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	return TRUE;
}

// --------------------------------------------------------------------------------------
// �ɼ�
BOOL CChartRealConverter::Convert_OOC0(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_OOC0* pInput = (stEUREAL_OOC0*)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->koptcode, sizeof(pInput->koptcode));	/*  [�����ڵ�]				*/
	sTmp[sizeof(pInput->koptcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [ü��ð�]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:����]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [���簡,     ��]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [���ϴ��,   ��]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [�����]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [�����ŷ���, ��]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [�ð�,       ��]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [��,       ��]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [����,       ��]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	return TRUE;
}

// �ؿ�
BOOL CChartRealConverter::Convert_HWFJ(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_HWFJ* pInput = (stEUREAL_HWFJ*)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->symbol, sizeof(pInput->symbol));	/*  [�ɺ��ڵ�]				*/
	sTmp[sizeof(pInput->symbol)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->time, sizeof(pInput->time));	/*  [�ð�(�����ð�)]				*/
	sTmp[sizeof(pInput->time)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [���ϴ�񱸺�]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [���簡]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [���ϴ��]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [�����]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [�ŷ���]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [�ð�]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [��]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [����]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	return TRUE;
}

BOOL CChartRealConverter::Convert_FX_STOCK_SISE(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	STREAL_GNF_STOCK_SISE* pInput = (STREAL_GNF_STOCK_SISE*)pRealStruct;
	CREAL_GNF_STOCK_SISE cRealData;
	cRealData.Convert(pInput);
	CString strData;

	//�����ڵ�		
	realData_Common.m_strCode = cRealData.m_szjmcode;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();

	//���簡
	strData = cRealData.m_szprice;
	strData.Replace(".", "");
	realData_Common.m_strPrice.Format("%ld", atol(strData));

	//��ȣ
	strData = cRealData.m_szsign;
	if ( strData == "" )
		strData = " ";
	realData_Common.m_strSign = strData;

	//���
	strData = cRealData.m_szchange;
	strData.Replace(".", "");
	realData_Common.m_strChange.Format("%ld", atol(strData));

	//�����
	strData = cRealData.m_szdrate;
	strData.Replace(".", "");
	realData_Common.m_strChrate.Format("%ld", atol(strData));

	//ü�ᷮ
	strData = cRealData.m_szcvolume;
	realData_Common.m_strcVolume = strData;
	realData_Common.m_strcVolume.TrimLeft(); realData_Common.m_strcVolume.TrimRight();

	//�����ŷ���
	strData = cRealData.m_szvolume;
	realData_Common.m_strVolume = strData;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();
	
	//�ð�
	strData = cRealData.m_szopen;
	strData.Replace(".", "");
	realData_Common.m_strOpen.Format("%ld", atol(strData));

	//��
	strData = cRealData.m_szhigh;
	strData.Replace(".", "");
	realData_Common.m_strHigh.Format("%ld", atol(strData));

	//����
	strData = cRealData.m_szlow;
	strData.Replace(".", "");
	realData_Common.m_strLow.Format("%ld", atol(strData));

	//�ż�ȣ��
	strData = cRealData.m_szbidho1;
	strData.Replace(".", "");
	realData_Common.m_strBid.Format("%ld", atol(strData));

	//�ŵ�ȣ��
	strData = cRealData.m_szofferho1;
	strData.Replace(".", "");
	realData_Common.m_strOffer.Format("%ld", atol(strData));	
	

	CString strTempDate, strTempTime;
	strTempDate = cRealData.m_szbday;
	strTempTime = cRealData.m_szchetime;
	
//  	if(strTempDate == "" || strTempTime == "")
//  		realData_Common.m_strTime = "00000000";
//  	else
//  		realData_Common.m_strTime = strTempDate.Right(2) + strTempTime;
	realData_Common.m_strTime = strTempTime;

	return TRUE;
}
BOOL CChartRealConverter::Convert_FX_STOCK_HOGA(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	STREAL_GNF_STOCK_HOGA* pInput = (STREAL_GNF_STOCK_HOGA*)pRealStruct;
	CREAL_GNF_STOCK_HOGA cRealData;
	cRealData.Convert(pInput);
	CString strData;

	//�����ڵ�		
	realData_Common.m_strCode = cRealData.m_szjmcode;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();

	//�ð�
	
	CTime clTime = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format("%d%02d%02d", clTime.GetYear(), clTime.GetMonth(), clTime.GetDay());
	realData_Common.m_strTime = cRealData.m_szchetime;
//	realData_Common.m_strTime = cRealData.m_szchetime;

	//�ŵ�ȣ��
	strData = cRealData.m_szofferho1;
	strData.Replace(".", "");
	realData_Common.m_strBid.Format("%ld", atol(strData));

	//	�ŵ��ܷ�
	realData_Common.m_strChange = cRealData.m_szofferrem;

	//	�ż�ȣ��
	strData = cRealData.m_szbidho1;
	strData.Replace(".", "");
	realData_Common.m_strOffer.Format("%ld", atol(strData));

	//	�ż��ܷ�
	realData_Common.m_strChrate = cRealData.m_szbidrem;


	return TRUE;
}
BOOL CChartRealConverter::Convert_FX_STOCK_30SEC(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	return TRUE;
}


BOOL CChartRealConverter::Convert_ORDERINFO(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	return TRUE;
}