// IndicatorSingleton.cpp: implementation of the CIndicatorFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorFactory.h"

#include <Math.h>											// for floor()
#include <Assert.h>											// for assert()
#include "../Include_Chart/DLL_Load/IndicatorInfo.h"		// for CIndicatorInfo
#include "../Include_Chart/DLL_Load/PacketList.h"			// for CPacketList
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/ChartFormulateData.h"			// for CChartFormulateData
#include "../Include_Chart/PacketPointerList.h"				// for CGraphCalculateData
#include "../Include_Chart/Conversion.h"					// for CDataConversion
#include "../Include_Chart/DataMath.h"						// for CMath
#include "GetIndicatorInfo.h"								// for CGetIndicatorInfo
#include "resource.h"										// for IDS_EX_RIGHTS
// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
#include "../Include_Chart/GraphData.h"					// for CSubGraphData
#include "Signal.h"											// for CSignal

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CIndicator* CIndicatorSingleton::Create( IMetaTable::EDDC_INDICATOR_ID p_eID, HWND p_hOcxHwnd)
{
	switch( p_eID)
	{
		case IMetaTable::EDDC_OCX_ACCOUNT_ITEMSIZE:	return new CAccount;
		case IMetaTable::EDDC_OCX_BIG_NEWS:			return new CBigNews;
		case IMetaTable::EDDC_OCX_BOXCHARTCHART:		return new CBoxChart;
		case IMetaTable::EDDC_OCX_FOURIER_TRANSFORM:	return new CFourierTransform;
		case IMetaTable::EDDC_OCX_FUOPPANDFCHART:		return new CFuOpPF;
		case IMetaTable::EDDC_OCX_GLANCEBALANCECHART:	return new CGlanceBalance;
		case IMetaTable::EDDC_OCX_VOLUMEFORSALE:		return new CHorizontalBar;
		case IMetaTable::EDDC_OCX_INVERSLINECHART:		return new CInverseLine;
		case IMetaTable::EDDC_OCX_KAGICHART:			return new CKagiChart;
		case IMetaTable::EDDC_OCX_LOCKCHART:			return new CLock( p_hOcxHwnd);
		case IMetaTable::EDDC_OCX_MARKETPROFILECHART:	return new CMarketProfileChart;
		case IMetaTable::EDDC_OCX_PANDFCHART:			return new CPF;
		case IMetaTable::EDDC_OCX_RAINBOWCHART:		return new CRainbow;
		case IMetaTable::EDDC_OCX_COMPARATIVECHART:	return new CRelative_Strength;
		case IMetaTable::EDDC_OCX_RENKOCHART:			return new CRenKoChart;
		case IMetaTable::EDDC_OCX_SWINGCHART:			return new CSwingChart;
		case IMetaTable::EDDC_OCX_THREELINEBRACKCHART:	return new CThreeLineBreak;
		case IMetaTable::EDDC_OCX_ZIGZAGCHART:			return new CZigzag;
		case IMetaTable::EDDC_OCX_CLCCHART:				return new CCLC;
		case IMetaTable::EDDC_OCX_VOLUMESELLBUYCHART:	return new CSellBuyHorizontalBar;
// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
		// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
		case IMetaTable::EDDC_OCX_PRICEMACHART:			return new CMA( IMetaTable::EDDC_OCX_PRICEMACHART);
		case IMetaTable::EDDC_OCX_VOLUMEMACHART:		return new CMA( IMetaTable::EDDC_OCX_VOLUMEMACHART);

		default:										return new CNoCalculate( p_eID);
	}
	return NULL;
}

// (2006/9/6 - Seung-Won, Bae) Check ST Index Indicator
CIndicator *CIndicatorSingleton::CreateSTIndexCalculator( IMetaTable::EDDC_INDICATOR_ID p_eID)
{
	return new CSystemTradingCalculate( p_eID);
}


//*****************************************************************************************************
// SystemTrading���� ����� �ϴ� ��ǥ
// ����(05/08/03) ���� �ð��� ���⶧���� ���� ���ڵ���! ���߿� ��! �ݵ��! ��ĥ��!!!!!!!
//******************************************************************************************************
void CSystemTradingCalculate::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	//bool bUse = signalData.IsUse();
	// signal ����� ��츸 packet ����
	// subGraph�� �ش� packet �����ϱ�
	SetGraphPacketList(chartFormulateData, pGraphCalculateData, pIndicatorInfo, calculateData);
	// graph�� signal �����ϱ�
	SetSingalPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

// ----------------------------------------------------------------------------
//sy 2006.04.13.
// subGraph�� �ش� packet �����ϱ�
void CSystemTradingCalculate::SetGraphPacketList(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo, CCalculateData& calculateData)
{
	//sy 2005.10.11 -> real ����Ҷ��� packet�� �ٽ� ������ �ʿ䰡 ����!!
	if(pIndicatorInfo == NULL || chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL)
		return;

	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	CString strIndicatorName = pIndicatorInfo->GetIndicatorName();

// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
// 	//sy 2005.10.11 Parabolic�� ��Ŷ�� �����ʿ��ϴ� parabolic����Ÿ�� ��, ������Ŷ.(Ư���Ѱ����)
// 	switch( g_iMetaTable.GetIndicatorID( strIndicatorName))
// 	{
// // (2007/1/25 - Seung-Won, Bae) Use High/Low Packet directly on Draw Graph.
// //		case IMetaTable::EDDC_ST_PARABOLICCHART:	SetPacketList_Parabolic( pPacketList, pGraphCalculateData, pIndicatorInfo);
// //													break;
// 		case IMetaTable::EDDC_ST_PRICEMACHART:
// 		case IMetaTable::EDDC_ST_VOLUMEMACHART:	SetMAPacketList( pPacketList, pGraphCalculateData, pIndicatorInfo, calculateData);
// 												break;
// 		default:	SetGraphPacketList( pPacketList, pGraphCalculateData, pIndicatorInfo);
// 					break;
// 	}
	SetGraphPacketList( pPacketList, pGraphCalculateData, pIndicatorInfo);
}

// graph�� signal �����ϱ�
void CSystemTradingCalculate::SetSingalPacket(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	if(pGraphCalculateData == NULL || pIndicatorInfo == NULL)
		return;

	// signal ����� ��츸 packet ����
	if(!pIndicatorInfo->IsSignalUse())
		return;

	if(pGraphCalculateData->GetSignalPacket() != NULL)
		return;

	CPacket* pSignalPacket = NULL;
	// (2006/12/11 - Seung-Won, Bae) Support multi-same Indicator.
	if( !CIndicator::GetSignalPacket( chartFormulateData.GetPacketList(), chartFormulateData.GetGraphName(), pSignalPacket))
		return;

	pGraphCalculateData->SetSignalPacket( pSignalPacket);
}
//sy end

// ----------------------------------------------------------------------------
// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
// void CSystemTradingCalculate::SetMAPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo, CCalculateData& calculateData)
// {
// 	if(!pPacketList || !pGraphCalculateData || !pIndicatorInfo)
// 		return;
// 
// 	int nSubGraphIndex = CGetIndicatorInfo::GetSubGraphIndex(pIndicatorInfo, calculateData.GetPacketNamesForSave());
// 	if(nSubGraphIndex < 0)
// 		return;
// 
// 	CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetAt(nSubGraphIndex);
// 	if(pSubGraphList == NULL)
// 		return;
// 
// 	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
// 	CString strSubGraphPacketNameHeader;
// 	if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";
// 
// 	CPacket* pPacket = pSubGraphList->GetHeadPacket();
// 	if(pPacket == NULL) 
// 	{
// 		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
// 		CPacket* pMAPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + calculateData.GetPacketNamesForSave(), false);
// 		if(pMAPacket == NULL)
// 			return;
// 		pSubGraphList->AddTail2( pMAPacket);
// 	}
// 
// }

// (2007/1/25 - Seung-Won, Bae) Use High/Low Packet directly on Draw Graph.
/*
//sy 2005.10.11
void CSystemTradingCalculate::SetPacketList_Parabolic( CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	if(pPacketList == NULL || pGraphCalculateData == NULL || pIndicatorInfo == NULL)
		return;

	CPacket* pHighPricePacket = pPacketList->GetBaseData( _MTEXT( C0_HIGH));
	CPacket* pLowPricePacket = pPacketList->GetBaseData( _MTEXT( C0_LOW));
	if(pHighPricePacket == NULL || pLowPricePacket == NULL) return;

	CPacket* pParabolicPacket = NULL;
	CParabolic_SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo, pParabolicPacket, pHighPricePacket, pLowPricePacket);
}
//sy end
bool CSystemTradingCalculate::CParabolic_SetGraphPacketList(CPacketList* pPacketList,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo*& pIndicatorInfo,
									CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	pPacket = NULL;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) return CParabolic_InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pPacket, pPacket1, pPacket2, pSubGraphList);
		else
		{
			if(pPacket == NULL)
				pPacket = pIndiPacket;
		}
	}
	return (pPacket != NULL);
}
bool CSystemTradingCalculate::CParabolic_InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					                   CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2,
									   CSubGraphPacketList* pSubGraphList)
{
	if(pIndicatorInfo == NULL)
		return false;

	CString strSubGraphName = pIndicatorInfo->GetSubGraphName(0);
	if(strSubGraphName.IsEmpty())
		return false;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	CString strSubGraphPacketNameHeader;
	if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";
	pPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + strSubGraphName, false);

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( pPacket);
	pSubGraphList->AddTail2( pPacket1);
	pSubGraphList->AddTail2( pPacket2);
	return (pPacket != NULL);
}
*/

void CSystemTradingCalculate::SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo)
{
	if(!pPacketList || !pGraphCalculateData || !pIndicatorInfo)
		return;

	CList<CString, CString> subGraphNameList;
	if(!pIndicatorInfo->GetSubGraphNameList(subGraphNameList))
		return;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	CString strSubGraphPacketNameHeader;
	if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

	POSITION pos_subGraphName = subGraphNameList.GetHeadPosition();
	POSITION pos_graphCalData = pGraphCalculateData->GetHeadPosition();
	while(pos_subGraphName && pos_graphCalData)
	{
		CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetNext(pos_graphCalData);
		if(pSubGraphPacketList == NULL)
			return;

		CPacket* pPacket = pSubGraphPacketList->GetHeadPacket();
		if(pPacket == NULL)
		{
			CPacket* pIndiPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + subGraphNameList.GetNext(pos_subGraphName), false);
			if(pIndiPacket == NULL)
				return;
			pSubGraphPacketList->AddTail2(pIndiPacket);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ�����
///////////////////////////////////////////////////////////////////////////////////////////////////////
CAccount::CAccount() : 
	m_pDateList(NULL),
	m_pHighPriceList(NULL),
	m_pLowPriceList(NULL),
	m_pAccountList(NULL),
	m_pAccountDateList(NULL),
	m_pDatePacket(NULL),
	m_pAccountDatePacket(NULL),
	CIndicator( IMetaTable::EDDC_OCX_ACCOUNT_ITEMSIZE)
{
}

void CAccount::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDateList = m_pDatePacket->GetnumericDataList();
		m_pHighPriceList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pLowPriceList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pAccountList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pAccountDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pAccountDateList = GetnumericDataList(m_pAccountDatePacket);
	}
	if(!IsValidPacket(m_pDateList) || !IsValidPacket(m_pHighPriceList) || !IsValidPacket(m_pLowPriceList) ||
	   !IsValidPacket(m_pAccountList) || !IsValidPacket(m_pAccountDateList))
		return;

	CPacket* pBuy = NULL;
	CPacket* pSell = NULL;
	if(!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo, pBuy, pSell))
		return;
	
	if(IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pBuy))
		return;

	//�ڷ�����, �ŷ������� date Ÿ�Ծ��, �ΰ��� ���ڸ� ���� �� �ִ� date Ÿ�� ���
	CString strgraphdate, strsellbuydate, strcomparedate;
	GetPacketDateType(m_pDatePacket, m_pAccountDatePacket, strgraphdate, strsellbuydate, strcomparedate);	

	CList<double, double> drealgraphdate, drealsellbuydate;
	GetRealDate(strgraphdate, strcomparedate, *m_pDateList, drealgraphdate); //�ڷ�����
	GetRealDate(strsellbuydate, strcomparedate, *m_pAccountDateList, drealsellbuydate); //���� �ڷ�����
	//�ŵ�, �ż�
	GetSellandBuyList(calculateData.GetPoint(), drealgraphdate, drealsellbuydate,
				      m_pAccountList, pBuy, pSell);

	SetPacketTypes(pBuy, TYPE_TWODECIMAL, TYPE_INTERGER, 0);
	SetPacketTypes(pSell, TYPE_TWODECIMAL, TYPE_INTERGER, 0); 
}


void CAccount::GetPacketDateType(CPacket* pDate, CPacket* pAccountDate,
								 CString& strgraphdate,
								 CString& strsellbuydate,
								 CString& strcomparedate)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pDate || !pAccountDate) return;

	strgraphdate = pDate->GetType(); 
	strsellbuydate = pAccountDate->GetType();

	strcomparedate = GetCompareDateType(strgraphdate, strsellbuydate);
}

//���� �����Դ� data ���� ���ؾ��ϴ� data���� ����..
//��) 20020314, 14102109 �϶�, ���� ���� 2�ڸ��� ���� 2�ڸ��� ������ data�� ��´�. �񱳸� ���ؼ�...
void CAccount::GetRealDate(const CString& stroriginaldatetype, const CString& strdatetype,
						   const CList<double, double>& dDateList,
						   CList<double, double>& dRealDateList)
{
	TCHAR first = strdatetype.GetAt(0);
	int nfind = stroriginaldatetype.Find(first, 0);

	//string���� �ٲ㼭 �ش� �ε������� �ε������� �����ͼ� �ٽ� double������ �ٲ㼭 �ִ´�.
	POSITION pos = dDateList.GetHeadPosition();
	while(pos) {
		CString temp = CDataConversion::DoubleToString(dDateList.GetNext(pos), stroriginaldatetype.GetLength());
		dRealDateList.AddTail(atoi(temp.Mid(nfind, strdatetype.GetLength())));
	}
}

//dSellBuyDateList = HHMMSSNN, �ٸ��� �ɼ��� �ֵ�..
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//��) graph = YYYYMMDD, sellbuy = DDHHMMSS �϶��� dd�θ� �� ����
CString CAccount::GetCompareDateType(const CString& strgraphdate, const CString& strsellbuydate)
{
	if(strsellbuydate.CompareNoCase("YYYYMMDD") == 0)
		return WhenYYYYMMDD(strgraphdate);
	else if(strsellbuydate.CompareNoCase("YYMMDD") == 0)
		return WhenYYMMDD(strgraphdate);
	else if(strsellbuydate.CompareNoCase("YYYYMM") == 0)
		return WhenYYYYMM(strgraphdate);
	else if(strsellbuydate.CompareNoCase("YYMM") == 0)
		return WhenYYMM(strgraphdate);
	else if(strsellbuydate.CompareNoCase("MMDD") == 0)
		return WhenMMDD(strgraphdate);
	else if(strsellbuydate.CompareNoCase("DDHH") == 0)
		return WhenDDHH(strgraphdate);
	else if(strsellbuydate.CompareNoCase("DDHHMM") == 0)
		return WhenDDHHMM(strgraphdate);
	else if(strsellbuydate.CompareNoCase("DDHHMMSS") == 0)
		return WhenDDHHMMSS(strgraphdate);
	else if(strsellbuydate.CompareNoCase("HHMMSS") == 0)
		return WhenHHMMSS(strgraphdate);
	else if(strsellbuydate.CompareNoCase("HHMMSSNN") == 0)
		return WhenHHMMSSNN(strgraphdate);
	return "";
}

//dSellBuyDateList = YYYYMMDD
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. yyyy
//2. YYMM - �а� �򰥸��� �����ϱ�..
//3. MMDD�� - �а� �򰥸��� �����ϱ�..
//4. DD��
//1,2,3 => YYYYMMDD
//2,3 => YYMMDD
//1,2=> YYYYMM
//2=> YYMM
//3 => MMDD
//4=> DD
CString CAccount::WhenYYYYMMDD(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYYY") != -1 && strGraphDateType.Find("YYMM") != -1 && strGraphDateType.Find("MMDD") != -1)
		return "YYYYMMDD";
	else if(strGraphDateType.Find("YYMM") != -1 && strGraphDateType.Find("MMDD") != -1)
		return "YYMMDD";
	else if(strGraphDateType.Find("YYYY") != -1 && strGraphDateType.Find("YYMM") != -1)
		return "YYYYMM";
	else if(strGraphDateType.Find("YYMM") != -1)
		return "YYMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MMDD";
	else if(strGraphDateType.Find("DD") != -1)
		return "DD";
	return "";
}
//dSellBuyDateList = YYMMDD
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. YYMM
//2. MMDD
//3. DD
//1,2 => YYMMDD
//1 => YYMM
//2 => MMDD
//3=> DD
CString CAccount::WhenYYMMDD(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYMM") != -1 && strGraphDateType.Find("MMDD") != -1)
		return "YYMMDD";
	else if(strGraphDateType.Find("YYMM") != -1)
		return "YYMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MMDD";
	else if(strGraphDateType.Find("DD") != -1)
		return "DD";
	return "";
}
//dSellBuyDateList = YYYYMM
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. YYYY
//2. YYMM
//3. MMDD - �а� �򰥸��� �����ϱ�..���ؼ� DD�� �����ϱ�. MM�� �ش�
//1,2 => YYYYMM
//2 => YYMM
//3 => MM
CString CAccount::WhenYYYYMM(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYYY") != -1 && strGraphDateType.Find("YYMM") != -1)
		return "YYYYMM";
	else if(strGraphDateType.Find("YYMM") != -1)
		return "YYMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MM";
	return "";
}
//dSellBuyDateList = YYMM
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. YYMM
//2. MMDD - �а� �򰥸��� �����ϱ�..���ؼ� DD�� �����ϱ�. MM�� �ش�
//1 => YYMM
//2 => MM
CString CAccount::WhenYYMM(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYMM") != -1)
		return "YYMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MM";
	return "";
}
//dSellBuyDateList = MMDD
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. YYMM - �а� �򰥸��� �����ϱ�..���ؼ� YY�� �����ϱ�. MM�� �ش�
//2. MMDD - �а� �򰥸��� �����ϱ�..
//3. DD
//1 => MM
//2 => MMDD
//3 => DD
CString CAccount::WhenMMDD(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("YYMM") != -1)
		return "MM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "MMDD";
	else if(strGraphDateType.Find("DD") != -1)
		return "DD";
	return "";
}
//dSellBuyDateList = DDHH
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. MMDD - �а� �򰥸��� �����ϱ�..���ؼ� MM�� �����ϱ�. DD�� �ش�
//2. DDHH
//3. HH
//1 => DD
//2 => DDHH
//3 => HH
CString CAccount::WhenDDHH(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("MMDD") != -1)
		return "DD";
	else if(strGraphDateType.Find("DDHH") != -1)
		return "DDHH";
	else if(strGraphDateType.Find("HH") != -1)
		return "HH";
	return "";
}
//dSellBuyDateList = DDHHMM
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. MMDD - �а� �򰥸��� �����ϱ�..���ؼ� MM�� �����ϱ�. DD�� �ش�
//2. DDHH
//3. HHMM - �а� �򰥸��� �����ϱ�..
//1 => DD
//2 => DDHH
//3 => HHMM
//2,3 => DDHHMM
CString CAccount::WhenDDHHMM(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("DDHH") != -1 && strGraphDateType.Find("HHMM") != -1)
		return "DDHHMM";
	else if(strGraphDateType.Find("MMDD") != -1)
		return "DD";
	else if(strGraphDateType.Find("DDHH") != -1)
		return "DDHH";
	else if(strGraphDateType.Find("HHMM") != -1)
		return "HHMM";
	return "";
}

//dSellBuyDateList = DDHHMMSS
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. MMDD - �а� �򰥸��� �����ϱ�..���ؼ� MM�� �����ϱ�. DD�� �ش�
//2. DDHH
//3. HHMM - �а� �򰥸��� �����ϱ�..
//3. SS
//1 => DD
//2 => DDHH
//3 => HHMM
//2,3 => DDHHMM
//2,3,4 => DDHHMMSS
CString CAccount::WhenDDHHMMSS(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("DDHH") != -1 && strGraphDateType.Find("HHMM") != -1 && strGraphDateType.Find("SS") != -1)
		return "DDHHMMSS";
	else if(strGraphDateType.Find("DDHH") != -1 && strGraphDateType.Find("HHMM") != -1)
		return "DDHHMM";
	if(strGraphDateType.Find("MMDD") != -1)
		return "DD";
	else if(strGraphDateType.Find("DDHH") != -1)
		return "DDHH";
	else if(strGraphDateType.Find("HHMM") != -1)
		return "HHMM";
	return "";
}

//dSellBuyDateList = HHMMSS
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. HH
//2. HHMM 
//3. HHMMSS
//1 => HH
//2 => HHMM
//3 => HHMMSS
CString CAccount::WhenHHMMSS(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("HH") != -1)
		return "HH";
	else if(strGraphDateType.Find("HHMM") != -1)
		return "HHMM";
	else if(strGraphDateType.Find("HHMMSS") != -1)
		return "HHMMSS";
	return "";
}
//dSellBuyDateList = HHMMSSNN
//dGraphDateList = YYYYMMDD, YYMMDD, YYYYMM, YYMM, MMDD, DDHH, DDHHMM, DDHHMMSS, HHMMSS, HHMMSSNN
//graph��
//1. HH
//2. HHMM 
//3. HHMMSS
//4. HHMMSSNN
//1 => HH
//2 => HHMM
//3 => HHMMSS
//4 => HHMMSSNN
CString CAccount::WhenHHMMSSNN(const CString& strGraphDateType)
{
	if(strGraphDateType.Find("HH") != -1)
		return "HH";
	else if(strGraphDateType.Find("HHMM") != -1)
		return "HHMM";
	else if(strGraphDateType.Find("HHMMSS") != -1)
		return "HHMMSS";
	else if(strGraphDateType.Find("HHMMSSNN") != -1)
		return "HHMMSSNN";
	return "";
}

//
void CAccount::GetSellandBuyList(const CPoint& point,
								 const CList<double, double>& dRealGraphDateList,
								 const CList<double, double>& dRealSellBuyDateList,
								 CList<double, double>* pSellBuyList,
								 CPacket* pBuy, CPacket* pSell)
{
//�ǽð�.....
	double nsell = 0; //data
	double nbuy = 0;
	int ngraph, nsellbuy;
	int date[2];  

	double* nSell = new double[dRealGraphDateList.GetCount()];
	double* nBuy = new double[dRealGraphDateList.GetCount()];
	for(int count = 0; count< dRealGraphDateList.GetCount(); count++) {
		nSell[count] = 0;
		nBuy[count] = 0;
	}

	//�ǽð�..
	//�� ������ �ŷ������� ���Ѵ�.
	if(point.x != 0) {
		ngraph = point.y;
		nsellbuy = (dRealSellBuyDateList.GetCount() - 1);
	}
	else { //New, Insert
		ngraph = (dRealGraphDateList.GetCount() - 1);
		nsellbuy = 0;
	}

	date[0] = (int)(dRealGraphDateList.GetAt(FindPosition(dRealGraphDateList.GetCount() - 1, dRealGraphDateList)));
	POSITION pos = FindPosition(ngraph, dRealGraphDateList);
	if(pos == NULL)
	{
		delete[] nSell;
		delete[] nBuy;
		return ;
	}
	for(ngraph; ngraph >= point.x ; ngraph--) {
		if(nsellbuy == dRealSellBuyDateList.GetCount())
			break;

		date[1] = date[0];
//		date[0] = (int)(dRealGraphDateList.GetAt(dRealGraphDateList.FindIndex(ngraph)));
		date[0] = (int)(dRealGraphDateList.GetPrev(pos));
		POSITION pos1 = FindPosition(nsellbuy, dRealSellBuyDateList); 
		if(pos1 == NULL)
		{
			delete[] nSell;
			delete[] nBuy;
			return ;
		}
		for(nsellbuy; nsellbuy < dRealSellBuyDateList.GetCount() ; nsellbuy++) {
//			int sellbuydate = (int)(dRealSellBuyDateList.GetAt(dRealSellBuyDateList.FindIndex(nsellbuy)));
			int sellbuydate = (int)(dRealSellBuyDateList.GetNext(pos1));

			//nsellbuy�� �ٲ�°� �ұ�Ģ
			if(date[0] == sellbuydate) {

				// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
				if( !pSellBuyList){
					delete[] nSell;
					delete[] nBuy;
					return;
				}

				double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
				if(value == 1)
					nbuy += 1;
				else if(value == 2)
					nsell += 1;
			}
			else if(date[1] > sellbuydate && date[0] < sellbuydate) {
				CString strPredate = CDataConversion::IntToString(date[1]);
				CString strCurrentdate = CDataConversion::IntToString(date[0]);
				CString strsellbuydate = CDataConversion::IntToString(sellbuydate);

				int nCount = (strPredate.GetLength() / 2);
				if(nCount > 3)
					nCount = 3;
				bool blast = false;
				for(int nIndex = 1 ; nIndex < nCount; nIndex++) {
					strCurrentdate.Delete(strCurrentdate.GetLength() - 2, 2);
					strPredate.Delete(strPredate.GetLength() - 2, 2);
					strsellbuydate.Delete(strsellbuydate.GetLength() - 2, 2);

					if((nIndex +1) == nCount)
						blast = true;

					if(CompareDate(blast, strPredate, strCurrentdate, strsellbuydate,
								  ngraph, nsellbuy, nsell, nbuy, nSell, nBuy, pSellBuyList))
						break;
				}
			}
			else  //�����°�� data�� �ٽ� ���ؾ� �ϴϱ�..
				break;
		}
		nSell[ngraph] = nsell;
		nBuy[ngraph] = nbuy;
		nsell = 0;
		nbuy = 0;
//		TRACE("index=> %d, date=>%d, sell => %d, buy => %d\r\n", ngraph, date[0], nSell[ngraph], nBuy[ngraph]);
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSell || !pBuy){
		delete[] nSell;
		delete[] nBuy;
		return;
	}

	for(int i = point.x ; i < dRealGraphDateList.GetCount() ; i++) {
		pSell->AppendData(nSell[i]);
		pBuy->AppendData(nBuy[i]);
	}
	delete[] nSell;
	delete[] nBuy;
}

//1, 0,
//sellbuydate = 13090023
bool CAccount::CompareDate(const bool bLast,
						   const CString strPreDate,
						   const CString strCurrentDate,
						   const CString strSellBuyDate,
						   const int ngraph, const int nsellbuy, double& nsell, double& nbuy,
						   double* nSell, double* nBuy, CList<double, double>* pSellBuyList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSellBuyList) return FALSE;

	//13090024, 13090015
	if(strCurrentDate.CompareNoCase(strSellBuyDate) == 0 && strPreDate.CompareNoCase(strSellBuyDate) == 0) {
		double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
		if(value == 1)
			nbuy += 1;
		else if(value == 2)
			nsell += 1;
		return true;
	}
	//130902 , 130800 //2��° ��������.
	else if(strCurrentDate.CompareNoCase(strSellBuyDate) != 0 && strPreDate.CompareNoCase(strSellBuyDate) == 0) {
		double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
		if(value ==1)
			nSell[ngraph + 1] = (nSell[ngraph+1] + 1);
		else if(value == 2)
			nBuy[ngraph + 1] = (nBuy[ngraph+1] + 1);
		return true;
	}
	//13090200, 13090000
	else if(strCurrentDate.CompareNoCase(strSellBuyDate) == 0 && strPreDate.CompareNoCase(strSellBuyDate) != 0) {
		double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
		if(value == 1)
			nbuy += 1;
		else if(value == 2)
			nsell += 1;
		return true;
	}
	//131002 , 130800 �ϴٰ� ������.
	else if(strPreDate.CompareNoCase(strSellBuyDate) != 0 && strPreDate.CompareNoCase(strSellBuyDate) != 0) {
		if(bLast == true) {
			double value = pSellBuyList->GetAt(FindPosition(nsellbuy, *pSellBuyList));
			if(value == 1)
				nbuy += 1;
			else if(value == 2)
				nsell += 1;
			return true;
		}
		return false;
	}
	return false;
}

bool CAccount::IsCurrentDateCloserthanPreDate(const int nPredate, const int nCurrentdate, const int nSellbuyDate)
{
	return abs(nCurrentdate - nSellbuyDate) < abs(nPredate - nSellbuyDate) ? true : false;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//BigNews
//�� �ٽ� ���.
///////////////////////////////////////////////////////////////////////////////////////////////////////
CBigNews::CBigNews() : 
	m_pDateList(NULL),
	m_pBignewsDataList(NULL),
	m_pNewsDateList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_BIG_NEWS)
{
}
void CBigNews::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pDateList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pBignewsDataList = pPacketList->GetstrDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pNewsDateList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
	}
//	if(!IsValidPacket(m_pDateList) || !IsValidPacket(m_pNewsDateList) || m_pBignewsDataList == NULL || m_pBignewsDataList->GetCount() <= 0)
//		return;

	CPacket* pBigNews = NULL;
	if(!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo, pBigNews))
		return;
	
	if(IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pBigNews))
		return;

	//sy 2004.04.13.
	// ������ ��쿡�� �̸� type�� �־� ��� �Ѵ�.
	// -> �⺻���� "X 1" �� setting �ϹǷ� add���� �ʴ´�.
	SetPacketTypes(pBigNews, _MTEXT( C6_CHARACTER), _MTEXT( C6_CHARACTER), 0);
	GetBigNews(calculateData.GetPoint(), m_pDateList, m_pBignewsDataList, m_pNewsDateList, pBigNews);
}

void CBigNews::GetBigNews(const CPoint& point, 
						  CList<double, double>* pGraphDateList,
						  CList<CString, CString>* pBigNewsList,
						  CList<double, double>* pBigNewsDateList,
						  CPacket* pBigNews)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pBigNewsDateList || !pGraphDateList || !pBigNews) return;

	if(pBigNewsDateList->GetCount() <= 0)
		return;

	POSITION posNewsDate = FindPosition(0, *pBigNewsDateList);
	POSITION posDate = FindPosition(0, *pGraphDateList);

	if(!IsValidBigNews(pGraphDateList, pBigNewsDateList, pBigNews))
		return;

	int nCount = pGraphDateList->GetCount();
	int ngraph = 0;
	int nNewsDate = 0;
	for(int nbignews = 0; nbignews < pBigNewsDateList->GetCount() ; nbignews++)
	{
		if(posNewsDate == NULL || posDate == NULL)
			return ;
		//�ڷ����ڿ� �򴺽��� �Ͼ ��¥�� �Ϻ� ���� ���(�ڷ����ڰ� �� �����ΰ��)
		//���� �򴺽��� ���� ǥ���Ҽ� ����.
		if(ngraph == nCount - 1)
			break;

		nNewsDate = (int)pBigNewsDateList->GetNext(posNewsDate);
		for(ngraph; ngraph < nCount ; ngraph++)
		{
			POSITION pos = posDate;
			int nGraphDate = (int)pGraphDateList->GetNext(posDate);
			if(nGraphDate < nNewsDate)
				pBigNews->AppendData("");
			else if(nGraphDate == nNewsDate)
			{
				pBigNews->AppendData(pBigNewsList->GetAt(FindPosition(nbignews, *pBigNewsList)));
				ngraph++;
				break;
			}
			else if(nGraphDate > nNewsDate)
			{
				posDate = pos;
				break;
			}
		}
	}

	//�ڷ����ڿ� �򴺽��� �Ͼ ��¥�� �Ϻ� ���� ���(�ڷ����ڰ� ������ �򴺽� ���Ŀ� �����ϴ°��)
	//������ �򴺽��� ���ϰ� �� for���� �����.
	if(ngraph != nCount - 1)
	{
		for(ngraph ; ngraph < nCount ; ngraph++)
			pBigNews->AppendData("");
	}

}

bool CBigNews::IsValidBigNews(CList<double, double>* pGraphDateList,
							  CList<double, double>* pBigNewsDateList,
							  CPacket* pBigNews)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphDateList || !pBigNewsDateList || !pBigNews) return FALSE;

	int nCount = pGraphDateList->GetCount();
	//�ڷ����ڿ� �򴺽��� �Ͼ ��¥�� ���� ���� ���(�ڷ����ڰ� �� �����ΰ��)
	if(pGraphDateList->GetTail() < pBigNewsDateList->GetHead())
	{
		for(int nGraph = 0 ; nGraph < nCount ; nGraph++)
			pBigNews->AppendData("");
		return false;
	}

	//�ڷ����ڿ� �򴺽��� �Ͼ ��¥�� ���� ���� ���(�ڷ����ڰ� ������ �򴺽� ���Ŀ� �����ϴ°��)
	if(pGraphDateList->GetHead() > pBigNewsDateList->GetTail())
	{
		for(int nGraph = 0 ; nGraph < nCount ; nGraph++)
			pBigNews->AppendData("");
		return false;
	}
	return true;
}


//******************************************************************************************************************
//	CBoxChart
//	  
// (2004.10.20, ��¿�) Logic �м� �� ���� ����
// (2004.10.20, ��¿�) Box Chart Packet�� �ð����� 4���� �����Ѵ�.
// [ Box Chart�� Output Packet�� ���Ͽ� ]
//		1. Output Packet 4���� �����Ǹ� �Ʒ��� Packet �� ����� ���� �����ȴ�.
//		2. Output Packet�� Data�� ���� Packet (��/��/��/��)�� �� Data ���� ������,
//				Box ������ ���� �����ϰ� �����ǵ��� �����Ѵ�.
//******************************************************************************************************************
CBoxChart::CBoxChart() : 
	m_pBoxStartPacket(NULL),	// Box Chart������ �ð� Packet
	m_pBoxHighPacket(NULL),		// Box Chart������ �� Packet
	m_pBoxLowPacket(NULL),		// Box Chart������ ���� Packet
	m_pBoxEndPacket(NULL),		// Box Chart������ ���� Packet
	m_pStartPriceList(NULL),	// ������Ʈ�� ��/��/��/�� 4 Packet
	m_pHighPriceList(NULL),
	m_pLowPriceList(NULL),
	m_pEndPriceList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_BOXCHARTCHART)
{
}

void CBoxChart::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 0. Packet List�� Ȯ���Ѵ�.
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList || !pIndicatorInfo) return;

	// 1. ���� List�� Ȯ���Ѵ�. (���ǰ��� Box�� ���� ���� �ϳ� ���̴�.)
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();

	// 2. �ʿ��� �⺻ Packet�� Ȯ���Ѵ�.
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		CPacket *pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pStartPriceList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pHighPriceList  = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pLowPriceList   = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pEndPriceList   = pPacket->GetnumericDataList();
	}
	if(!IsValidPacket(m_pStartPriceList) || !IsValidPacket(m_pHighPriceList) || 
		!IsValidPacket(m_pLowPriceList) || !IsValidPacket(m_pEndPriceList))
		return;

	// 3. �����ؾ��� Packet�� Ȯ���Ѵ�. (�� 6�����̳�?)
	if(!SetGraphPacketList(chartFormulateData.GetCalculateType(), pPacketList, pGraphCalculateData, 
		                   pIndicatorInfo))
		return;

	// 4. Box Chart Data�� �����Ѵ�.
	CPoint point = calculateData.GetPoint();
	CScaleBaseData::HORZSCALEDRAWERTYPE nType = pPacketList->GetDateUnitType();
	// 4.1 ���� Box��Ʈ�� �����ϴ� ���� ���Ѵ�.
	// 4.1.1 ���� ��/��/�� Chart�� Box Chart �����Ѵ�.
	//		Real�� �͵� Boxing ��� ���� ���� �ʴ´�. ������ �Ϻ��� Real�� ���ŵȴ�.
	//		����, ���� ������ Box�� ������ �������ǵ��� ������ Data�������� Box�� �����Ͽ�,
	//			ù ������ Box�� �̿����� ���� �� �ֵ��� �Ѵ�.
	if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY)
		GetBackwardBoxChart(point, GetCondition(0, pConditionList), m_pHighPriceList, m_pLowPriceList, m_pStartPriceList, m_pEndPriceList);
	// 4.1.2 ��/ƽ Chart�� Box Chart Data�� �����Ѵ�. (Real ���Ž� Boxing ��� ���� �þ �� �ִ�.)
	//		����, Ư�� �߰��� ���� ��ȸ������ ���� Box���� ��ġ�� ������ �ʵ��� (��/��/�� ������� �ϸ� ���ϰ� �ȴ�.)
	//			ù������ Box�� �����ϵ��� �����Ͽ�, �������� Box�� �̿����� ���� �� �ֵ��� �Ѵ�.
	//		�׷��� �Ͽ� Real�� �����ϸ鼭 Box�� ���������� ���� �ֵ��� �Ѵ�.
	else GetForwardBoxChart(point, GetCondition(0, pConditionList), m_pHighPriceList, m_pLowPriceList, m_pStartPriceList, m_pEndPriceList);

	// 5. ����� Packet�� �����Ѵ�.
	CString strPacketType = GetPacketType(pPacketList);
	SetPacketTypes( m_pBoxStartPacket,	strPacketType, strPacketType, 0);
	SetPacketTypes( m_pBoxHighPacket,	strPacketType, strPacketType, 0);
	SetPacketTypes( m_pBoxLowPacket,	strPacketType, strPacketType, 0);
	SetPacketTypes( m_pBoxEndPacket,	strPacketType, strPacketType, 0);
}

void CBoxChart::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData( m_pBoxStartPacket);
	ClearPacketData( m_pBoxHighPacket);
	ClearPacketData( m_pBoxLowPacket);
	ClearPacketData( m_pBoxEndPacket);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

// (2004.10.20, ��¿�, �м�, ���� ����) ��/��/�� Chart�� Box Chart �����Ѵ�.
//		Real�� �͵� Boxing ��� ���� ���� �ʴ´�. ������ �Ϻ��� Real�� ���ŵȴ�.
//		����, ���� ������ Box�� ������ �������ǵ��� ������ Data�������� Box�� �����Ͽ�,
//			ù ������ Box�� �̿����� ���� �� �ֵ��� �Ѵ�.
void CBoxChart::GetBackwardBoxChart(const CPoint& point, const double dCondition,
									CList<double, double>* pTopPriceList,
									CList<double, double>* pBottomPriceList,
									CList<double, double>* pStartPriceList,
									CList<double, double>* pEndPriceList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pBoxStartPacket || !m_pBoxHighPacket || !m_pBoxLowPacket || !m_pBoxEndPacket) return;

	// 0. ��/�� Data Packet�� ���ϰ�, �߰� ���� ������ Clear�Ѵ�.
	CList<double, double>* pStartList	= m_pBoxStartPacket->GetnumericDataList();
	CList<double, double>* pTopList		= m_pBoxHighPacket->GetnumericDataList();
	CList<double, double>* pBottomList	= m_pBoxLowPacket->GetnumericDataList();
	CList<double, double>* pEndList		= m_pBoxEndPacket->GetnumericDataList();

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pTopPriceList || !pBottomPriceList || !pStartPriceList || !pEndPriceList
		|| !pTopList || !pBottomList || !pStartList || !pEndList) return;

	// 1. ������ ������ Box Chart Data�� Clear��Ų��.
	//		Clear ������ ��찡, Data�� �ϳ��� ���ų� TR �Ǵ� ùReal(NoTR Real)�� ����̹Ƿ�,
	//		���������� Clear��Ű�� �ʴ� ��찡 ���ٰ� ���� �����ϴ�.
	// (2004.10.20, ��¿�) ������ Box�� ������ ���� ��Ȳ���� Real�� ���´ٰ�,
	//		�� ù Box (������ �����ϴ� ���忡��)�� �籸���ϴ� Logic�� �ƴϹǷ�,
	//		�׻� Clear��Ű���� �Ͽ��� �Ѵ�.
	pTopList->RemoveAll();
	pBottomList->RemoveAll();
	pStartList->RemoveAll();
	pEndList->RemoveAll();

	// 2. Box Chart�� �ð� List�� �����Ѵ�.
	// 2.0 ���� ���� ������ Data���� ���� ������ �������� �ʴ´�.
	int nTopPriceCount = pTopPriceList->GetCount();
	if( nTopPriceCount != pBottomPriceList->GetCount()
		&& nTopPriceCount != pStartPriceList->GetCount()
		&& nTopPriceCount != pEndPriceList->GetCount()) return;
	// 2.1 ������Ʈ ���� ������ ������ ���� �����ϸ鼭 �����Ѵ�.
	int			nDataCount = 0;
	int			nBoxWidth = (int)dCondition;
	double		dTop, dBottom, dMaxTop, dMaxBottom, dStart, dEnd;
	POSITION	posTop		= pTopPriceList->GetTailPosition();
	POSITION	posBottom	= pBottomPriceList->GetTailPosition();
	POSITION	posStart	= pStartPriceList->GetTailPosition();
	POSITION	posEnd		= pEndPriceList->GetTailPosition();
	while( posTop && posBottom && posStart && posEnd)
	{
		// 2.1.1 �̹��� ������ų Box Chart�� ���� ������ �����Ѵ�.
		// 2.1.1.1 ù Box Data�� Ȯ���Ѵ�.
		dMaxTop		= pTopPriceList->GetPrev(		posTop);
		dMaxBottom	= pBottomPriceList->GetPrev(	posBottom);
		dStart		= pStartPriceList->GetPrev(		posStart);
		dEnd		= pEndPriceList->GetPrev(		posEnd);

		// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( dMaxTop < dEnd || dMaxTop < dStart || dEnd < dMaxBottom || dStart < dMaxBottom)
			dMaxTop = dStart = dMaxBottom = dEnd;

		nDataCount++;
		// 2.1.1.2 ���� Box Data�� Ȯ���Ѵ�.
		while( posTop && posBottom && posStart && posEnd && nDataCount < nBoxWidth)
		{
			dTop	= pTopPriceList->GetPrev(		posTop);
			dBottom	= pBottomPriceList->GetPrev(	posBottom);
			dStart	= pStartPriceList->GetPrev(		posStart);
					  pEndPriceList->GetPrev(		posEnd);		// ������ �̹� ���� ����. �ݿ����� �ʴ´�.
			if( dMaxTop < dTop)			dMaxTop = dTop;
			if( dBottom < dMaxBottom)	dMaxBottom = dBottom;
			nDataCount++;
		}

		// 2.1.2 Ȯ�ε� ���� Data ��ŭ Box Data�� ������Ų��.
		for( ; 0 < nDataCount; nDataCount--)
		{
			pTopList->AddHead(		dMaxTop);
			pBottomList->AddHead(	dMaxBottom);
			pStartList->AddHead(	dStart);
			pEndList->AddHead(		dEnd);
		}
	}
}

// (2004.10.20, ��¿�, �м�, ���� ����) ��/ƽ Chart�� Box Chart Data�� �����Ѵ�.
//		Real ���Ž� Boxing ��� ���� �þ �� �ִ�.
//		����, Ư�� �߰��� ���� ��ȸ������ ���� Box���� ��ġ�� ������ �ʵ��� (��/��/�� ������� �ϸ� ���ϰ� �ȴ�.)
//			ù������ Box�� �����ϵ��� �����Ͽ�, �������� Box�� �̿����� ���� �� �ֵ��� �Ѵ�.
//		�׷��� �Ͽ� Real�� �����ϸ鼭 Box�� ���������� ���� �ֵ��� �Ѵ�.
void CBoxChart::GetForwardBoxChart(const CPoint& point, const double dCondition,
									CList<double, double>* pTopPriceList,
									CList<double, double>* pBottomPriceList,
									CList<double, double>* pStartPriceList,
									CList<double, double>* pEndPriceList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pBoxStartPacket || !m_pBoxHighPacket || !m_pBoxLowPacket || !m_pBoxEndPacket) return;

	// 0. ��/�� Data Packet�� ���ϰ�, �߰� ���� ������ Clear�Ѵ�.
	CList<double, double>* pStartList	= m_pBoxStartPacket->GetnumericDataList();
	CList<double, double>* pTopList		= m_pBoxHighPacket->GetnumericDataList();
	CList<double, double>* pBottomList	= m_pBoxLowPacket->GetnumericDataList();
	CList<double, double>* pEndList		= m_pBoxEndPacket->GetnumericDataList();

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pTopPriceList || !pBottomPriceList || !pStartPriceList || !pEndPriceList
		|| !pTopList || !pBottomList || !pStartList || !pEndList) return;

	// 1. ������ ������ Box Chart Data�� Clear��Ų��.
	//		Clear ������ ��찡, ÷���� �ٽ� ����ϴ� ��� �� TR�̹Ƿ� ���� Clear�Ǵ� ����̴�.
	//		��, �׻� Clear�ǰ� �ȴ�.
	// (2004.10.20, ��¿�) Box�� ������ ���� ��Ȳ���� Real�� ���´ٰ�,
	//		�� ������ Box (�����ϴ� ���忡��)�� �籸���ϴ� Logic�� �ƴϹǷ�,
	//		�׻� Clear��Ű���� �Ͽ��� �Ѵ�.
	pTopList->RemoveAll();
	pBottomList->RemoveAll();
	pStartList->RemoveAll();
	pEndList->RemoveAll();

	// 2. Box Chart�� �ð� List�� �����Ѵ�.
	// 2.0 ���� ���� ������ Data���� ���� ������ �������� �ʴ´�.
	int nTopPriceCount = pTopPriceList->GetCount();
	if( nTopPriceCount != pBottomPriceList->GetCount()
		&& nTopPriceCount != pStartPriceList->GetCount()
		&& nTopPriceCount != pEndPriceList->GetCount()) return;
	// 2.1 ������Ʈ ���� ������ ÷���� ���� �����ϸ鼭 �����Ѵ�.
	int			nDataCount = 0;
	int			nBoxWidth = (int)dCondition;
	double		dTop, dBottom, dMaxTop, dMaxBottom, dStart, dEnd;
	POSITION	posTop		= pTopPriceList->GetHeadPosition();
	POSITION	posBottom	= pBottomPriceList->GetHeadPosition();
	POSITION	posStart	= pStartPriceList->GetHeadPosition();
	POSITION	posEnd		= pEndPriceList->GetHeadPosition();
	while( posTop && posBottom && posStart && posEnd)
	{
		// 2.1.1 �̹��� ������ų Box Chart�� ���� ������ �����Ѵ�.
		// 2.1.1.1 ù Box Data�� Ȯ���Ѵ�.
		dMaxTop		= pTopPriceList->GetNext(		posTop);
		dMaxBottom	= pBottomPriceList->GetNext(	posBottom);
		dStart		= pStartPriceList->GetNext(		posStart);
		dEnd		= pEndPriceList->GetNext(		posEnd);
		nDataCount++;
		// 2.1.1.2 ���� Box Data�� Ȯ���Ѵ�.
		while( posTop && posBottom && posStart && posEnd && nDataCount < nBoxWidth)
		{
			dTop	= pTopPriceList->GetNext(		posTop);
			dBottom	= pBottomPriceList->GetNext(	posBottom);
					  pStartPriceList->GetNext(		posStart);		// �ð��� �̹� ���� ����. �ݿ����� �ʴ´�.
			dEnd	= pEndPriceList->GetNext(		posEnd);
			if( dMaxTop < dTop)			dMaxTop = dTop;
			if( dBottom < dMaxBottom)	dMaxBottom = dBottom;
			nDataCount++;
		}

		// 2.1.2 Ȯ�ε� ���� Data ��ŭ Box Data�� ������Ų��.
		for( ; 0 < nDataCount; nDataCount--)
		{
			pTopList->AddTail(		dMaxTop);
			pBottomList->AddTail(	dMaxBottom);
			pStartList->AddTail(	dStart);
			pEndList->AddTail(		dEnd);
		}
	}
}

bool CBoxChart::ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList,
							CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL)
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		m_pBoxStartPacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BOX_CHART) + _MTEXT( C0_OPEN), false);
		m_pBoxHighPacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BOX_CHART) + _MTEXT( C0_HIGH), false);
		m_pBoxLowPacket		= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BOX_CHART) + _MTEXT( C0_LOW), false);
		m_pBoxEndPacket		= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BOX_CHART) + _MTEXT( C0_CLOSE), false);
		
		ClearPacketData( m_pBoxStartPacket);
		ClearPacketData( m_pBoxHighPacket);
		ClearPacketData( m_pBoxLowPacket);
		ClearPacketData( m_pBoxEndPacket);
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphPacketList) return FALSE;

	pSubGraphPacketList->AddTail2( m_pBoxStartPacket);
	pSubGraphPacketList->AddTail2( m_pBoxHighPacket);
	pSubGraphPacketList->AddTail2( m_pBoxLowPacket);
	pSubGraphPacketList->AddTail2( m_pBoxEndPacket);

	return (m_pBoxStartPacket != NULL && m_pBoxHighPacket != NULL && m_pBoxLowPacket != NULL
		&& m_pBoxEndPacket != NULL);
}
bool CBoxChart::SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
								   CPacketList* pPacketList,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
		{
			return (ClearPacket(calculatetype, pPacketList, pSubGraphList, pIndicatorInfo));
		}
		if(calculatetype == CCalculateBaseData::TRDATA_CAL)
		{
			ClearPacketData( m_pBoxStartPacket);
			ClearPacketData( m_pBoxHighPacket);
			ClearPacketData( m_pBoxLowPacket);
			ClearPacketData( m_pBoxEndPacket);
		}
	}
	return ( m_pBoxStartPacket != NULL && m_pBoxHighPacket != NULL && m_pBoxLowPacket != NULL
		&& m_pBoxEndPacket != NULL);
}

//******************************************************************************************************************
//	CFourierTransform
//	  
//******************************************************************************************************************
CFourierTransform::CFourierTransform() :
	CIndicator( IMetaTable::EDDC_OCX_FOURIER_TRANSFORM)
{
}
void CFourierTransform::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.02.05
//
//	�м�	:	���� ���� P&F Chart (�ɼ� �ƴ�. �������� ȣ�� ���� �������� Hard Codeing �Ǿ� ����.)
//				1. �⺻����
//					1. ���� ���� P&F�� t�к� ���� Ȥ�� Tick�� Price�� �������� nȣ�� ������ ��ȯ���̴�.
//						(�Ｑ ��ȯ���� ����)
//					2. ����� 'X', �϶��� 'O'ǥ�� �Ͽ� MarkLine�� �����Ѵ�.
//					3. ����� ������ ���� ��/�� ������ nȣ�� ���� �����Ǿ�߸� ���� ��ȯ�� �����ϰ�,
//						��ȯ ���� ���������� Marking�� ó���ϰ� �ȴ�.
//					4. Mark�� �׻� ȣ������ (0.05)���� ǥ���Ѵ�.
//					5. ���ڰ� ����Ǵ� ���, �ش� Mark���� ������ �����Ѵ�. (������ û���� �ݺ��ϸ� ��ȯ�Ѵ�.)
//				2. ���� ����
//					1. TR�� ���� ��ȸ�ÿ��� Server���� �̹� P&F Chart�� �׸��� ���� Data���� �����Ͽ� �����ش�.
//						(�� Packet�� �ϳ��� ��� MarkLine�̴�.)
//							�ڷ�����	: �ش� MarkLine�� ������ ��(�߼��� Ȯ���Ų) ������ Data�� �ð��̴�.
//											(�߼��� Ȯ���Ű�� �ʰ�, �񱳸��� Data�� �ð��� �ݿ����� �ʴ´�.
//											 �����ڰ��� �Ǵ� ������ �Ǳ� �����̴�.)
//										  P&F Chart���� ��µ��� ������, ���� Real�� ���Žÿ� ���� ������ ������ �ȴ�.
//							�ð�		: MarkLine�� ���� �����̴�.
//							����		: MarkLine�� �� �����̴�.
//										  MarkLine�� ���� ���ݿ� ���� ������η� ����� ��� 'X', �϶��� ��� 'O'ǥ���Ѵ�.
//							�����ڰ�	: �ϳ��� MarkLine������ ���ڰ� ����Ǵ� ��쿡 ������ ������ Mark�� �����̴�.
//											�����ڰ��� ���� ���� 0���� �����ȴ�.
//											(����, �����ڰ��� �׳��� ù ���� ���� Data�� �ƴϴ�.
//												�����ڰ��� �׳��� ù Ȯ�� �� ������ ���� �����Ǵ� ù Mark�����̴�.
//												���� Ư������ ���� �߼��� Ȯ���ϰų� �������� �ʴ� ���,
//												�����ڰ��� �������� ���� �� �ִ�.)
//											(�ݴ��, �ϳ��� �߼��� ���ڰ� �ι� �ٲ�鼭 �߼��� �����Ǹ�,
//													�����ڰ��� �� ���� ���̾�� �Ѵ�.
//												�׷��� 1�� �߼��� �� ���� Packet���� ���������� �� �ִ� ������ �������� �ʾ�,
//													������ �����ڰ��� �������Ѵ�.
//												Ư�� �̷� ���� ����ȣ�������� ������� ��������� ũ�� ����� ����Ÿ���� ��������,
//													�ϴ� ����ȣ�������� Maximum�� �۰��Ͽ� �ذ��ϴ� �������� �����Ѵ�. - ����ö ���� ���� ó�� 2004.02.26)
//					2. Server���� �����ִ� Packet���� Ư���� ��츦 ó���ϱ� ���� ���� Packet�� �����´�.
//						1. ��1 ���� Packet (�������� Packet)	: ��� Packet Data�� �ռ� �׻� ������ ù��° �⺻ Packet�̴�.
//								�ڷ�����	: ����(��ȸ����)�� ����� �ð��̴�.
//												������ �������� �ƴ� ���, (Real�� ������?) 0���� ���´�.
//												�� ���� ��ȸ����, �������, �к�����, ���� Data���� ��ȸ�ϴ� ��쿡,
//													ù ���� �����ϱ� ���� �ð����� 1�к��� ��� 1�������� Data '����'���� �����Ͽ� ���� �����ȴ�.
//								�ð�		: �ڷ���ȸ �������� '��������'
//												Server���� ������ (��/��)�߼��� ���� ���, ȭ�鿡 ����� ���ذ��̴�.
//												���߼� ������ �ʱ� ��/������ �̿�ȴ�.
//								����		: �̿����
//								�����ڰ�	: �̿����
//						2. �� 2���� Packet (���� Packet)	: ��� Packet Data�� �ռ� �׻� ������ �ι�° �⺻ Packet�̴�.
//												������ ������ Real ���ſ� ���Ͽ� �ϼ��Ǵ� �̿ϼ� ������ �����Ͽ� ������ Packet���� �޴´�.
//												�� �Ʒ��� ���߼��� �߼� Packet�� ������� �ʰ� ������ �����Ͽ� ���߼� Packet���� �����ϰ� �ȴ�.
//												Tick�� ��쿡�� ������ Tick�� �������� �޴´�.
//								�ڷ�����	: ������ ���۽ð��̴�.
//								�ð�		: �̿����
//								����		: ������ �����̴�.
//								�����ڰ�	: ���� Packet Type (0:���߼� or NoPacket, 1:���߼�)
//						2. ���߼� Packet Data	: Server�� ��ȸ�� Data�� ������,
//											  Server���� ������ �߼��� �ϳ��� ���� ��� ��������, �߼� ������ Packet�̴�.
//								[���߼�]	: ���� �߼��� ���� �߼��� ������ �߼�ġ�� �������� ������ Ȯ���ϰԵǴµ�,
//													ù �߼��� ���� �߼��� ���� ����� ���/�϶��� �߼��� ���ÿ� �����ϸ鼭,
//													�� ���� ���� ȣ���� �Ѿ�� ������ �߼��� �����ϰ� �ȴ�.
//													�̶�, ���߼��� �ð��� +/- 0.05���Ͽ� ���� �ð� ������ �������� ��������Ѵ�.
//												�߼� �������� ��/������ ���Žÿ��� ���������� ���ŵ� ���� ������ ��Ÿ������ �Ѵ�.
//												Ư�� ���߼��� Drawing Packet���� ��ϵ��� �ʴ´�. ���� ���߼��� �ݿ��Ǿ� ���δ�.
//												�װ��� ���߼��� ���߼��� ���� �߼� ���� ����� Ʋ���� ������ ���� ������ �ʿ伺�� ����,
//													���� ���߼��� ��½ÿ��� ���� ���߼� ����� ���չ���� ���� ���߼��� Ʋ���⿡,
//													���߼��� ��¿� Packet���� ������� �ʴ´�.
//												���߼��� �ϴ���, �� ��/�� ���� ���������� �̻��� �Ǵ� ��찡 �ִ�.
//													����, ���߼����� �� 2���� Packet�� �����ڰ��� Ȯ���Ͽ� �̿��Ѵ�.
//								�ڷ�����	: �߼� ������ Ȯ���Ų ������ Data�� �ð��̴�.
//								�ð�		: �߼� ���� �ð���, ���߼��� �����ϴ� ���� �߻��� ������ Ȥ�� �ְ��̴�.
//								����		: �߼� ���� ������, ���߼��� �����ϴ� ���� �߻��� ������ Ȥ�� �ְ��̴�.
//								�����ڰ�	: �߼� �����߿� �߻��� �����ڰ��̴�.
//						3. �߼� Packet Data	: Server�� ������ �߼� Packet�̴�.
//								�ڷ�����	: �߼��� Ȯ���Ų ������ Data�� �ð��̴�.
//								�ð�		: �߼��� ���� �����̴�. ���� �߼��� �������� 0.05�� ũ�ų� (����� ���) �۴� (�϶��� ���)
//								����		: �߼��� �� �����̴�. �ð��� ������ ����߼����� �϶��߼����� �ĺ��Ѵ�.
//								�����ڰ�	: �߼� �����߿� �߻��� �����ڰ��̴�.
//					3. ��¿� ���� Packet
//						1. Scale Packet		: Drawing ó���� Y Scale�� ���߼��� ���� �޺��ϴ� ��츦 �����ϱ� ���Ͽ�,
//												Drawing�� ���� Min/Max Data�� �����Ͽ�, �������� ��Ҹ��� �����ϵ��� �����ϴ� Packet�̴�.
//											  Drawing ó������ �ϰ������� ó���� �����ϵ��� �׻� �� ó�� �߰��Ѵ�.
//								�ڷ�����	: �ڷᱸ�� ����
//								�ð�		: ��ü ��� Data�� �ּҰ�
//								����		: ��ü ��� Data�� �ִ밪
//								�����ڰ�	: �̿� ����
//						2. ���߼� Packet Data	: Server���� �����ִ� Packet�� �ƴ�, Real�� �к� ó���� Tick�� ���� �����Ǵ� �߼� Packet�̴�.
//								[���߼�]	: ���߼��� �к��� ���� ������ Ȯ������ �ʴ� ��Ȳ������ �ӽ� �߼��� ��Ÿ���� Packet����,
//													Server�� �ƴ�, ��ǥ ����� �� 2���� Packet�� �������� �����Ǹ�,
//													Tick�� ���� �߼� �����̳� Ȯ���� ��Ÿ���� Packet���� �׻� ��ϵȴ�.
//											  ���߼��� ��ǥ ��� Routine�� �ϰ����� ���Ͽ�, Real �Ӹ� �ƴ϶� TR�� ��쿡�� �߰��Ѵ�.
//											  ���� ���߼��� ��½� �� �߼����� Ȯ���Ͽ� �������� �Ѿ�� ��쿡�� ��������,
//													�׸���, �߼�Ȯ���� ��쿡�� Ȯ������ ���ó���Ѵ�.
//											  �̶�, ���� �߼��� Ȯ���Ͽ��� ���� Packet���� �ξ�, ������� ����� ���߼��� �����ϴ�.
//													Drawing�ÿ� �����Ͽ� ó���Ѵ�.
//											  Ư�� ���߼��� ���߼��� �����Ͽ� �����ش�. �̰�� ������ ���� ���߼� ������ Ȯ���� ��,
//													���� ���߼��� ���߼� ���� ������ �ݿ��Ͽ�, ���ο� ���߼��� ��������Ѵ�.
//								�ڷ�����	: �ڷᱸ�� ����
//								�ð�		: Tick�� ���� �ӽ÷� ������ �߼��� �ð��̴�.
//								����		: Tick�� ���� �ӽ÷� ������ �߼��� �����̴�.
//								�����ڰ�	: ���߼��� �����ϴ� ������ �����߼��� Ȯ��/������ �߻���Ű�鼭,
//												��¥�� �ٸ� ���, Ȯ��/�����Ǵ� ù Mark�� �����̴�. (���߼��� �ð��̴�.)
//												(����, �߼��� ������ �ƴϴ�.)
//					4. Real ó��
//						1. ���߼� ����	:
//								1. Server���� �߼� Packet Data�� ����, ���� Packet��, Ȥ�� ���߼� Packet�� �Բ�
//										�������� ����̴�.
//								2. ���߼��� ��ǥó���� Packet���� �������� �ʰ�, ���� ��Ȳ �����θ� �����Ѵ�.
//								3. �� �к��� �������� ���Ͽ� ���߼��� Ȯ���Ͽ� ������. (�߼��� Ȯ���ɶ�����)
//								4. ���� Packet�� �ð�(��ȸ������ ���� ����)�� �������� ������ ���߼� Packet���� ����Ѵ�.
//								5. �߼��� ������ ���, ���ο� �߼� Packet���� ��Ͻ�Ų��.
//						2. �߼� ����	:
//								1. Server���� �߼� Packet Data�� �����ְų�, Real�� ���� �߼��� Ȯ���� ����̴�.
//								2. �� Real Data�� �����Ͽ� �߼��� Ȯ�� �Ǵ� ������ ���߼� Packet�� ����Ѵ�.
//						3. Scale Packet ����	:
//								1. ���߼�/���߼�/���߼��� ������ ��� �߼� Data���� Drawing�� ���Ͽ�
//										�߼� Data���� Min/Max�� Packet���� �����Ѵ�.
//								2. ù ��ǥ Packet�� ��/�� ���� ��� Update�Ѵ�.
//						4. ���߼� Packet ����	:
//								1. �к� �߼������� Real������ ������ �̿ϼ� �к��� �߼��� �����ϴ� Packet����,
//										��ǥ Packet�� �������� �׻� ��Ͻ�Ų��.
///////////////////////////////////////////////////////////////////////////////
// (2004.02.05, ��¿�, �м�) ���� ���� P&F Chart ��ǥ Contructor
CFuOpPF::CFuOpPF() : CIndicator( IMetaTable::EDDC_OCX_FUOPPANDFCHART)
{
	// (2004.02.05, ��¿�, �м�) Input Packet�� �����Ѵ�.
	m_pIMLDatePacket			= NULL;	// �ڷ����� Packet
	m_pIMLStartPricePacket		= NULL;	// MarkLine �ð� Packet
	m_pIMLEndPricePacket		= NULL;	// MarkLine ���� Packet (Real ä�ᰡ Packet)
	m_pIMLNewDatePricePacket	= NULL;	// �����ڰ� Packet

	// (2004.02.05, ��¿�, �м�) Output Packet�� �����Ѵ�.
	//		(��½ÿ��� Date Packet�� �ʿ�� ���� �ʴ´�.
	//		 �� Mark���� �ð��� ������� �ʰ�, MarkLine ������ �ð��� �ǹ̰� ����.)
	m_pOMLStartPricePacket		= NULL;	// MarkLine �ð� Packet
	m_pOMLEndPricePacket		= NULL;	// MarkLine ���� Packet
	m_pOMLNewDatePricePacket	= NULL;	// �����ڰ� Packet

	// (2004.02.24, ��¿�) P&F ��ǥ�� �����ϱ� ���� ���ǰ��� ���Ͽ� �����Ѵ�.
	m_nStickTimeWidth				= 0;	// Real ���� �ð� (�д���, 0�� Tick)
	m_nNewTrendPriceX100			= 0;	// �߼����� ȣ������

	// (2004.02.24, ��¿�) Real�� ó���ϱ� ���� ���� ���¸� �����Ѵ�.
	m_nLastStickTime				= 0;	// ������ ���۽ð� (�̰��� �������� Real�� ������ �����Ѵ�.)

	// (2004.02.24, ��¿�) ���߼� ���� ������ �����Ѵ�.
	m_nMiniTrendTime				= -3;	// ���߼��� ������ �ð��̴�. (�⺻ ������ ���·� �ʱ�ȭ�Ѵ�.)
	m_nMiniTrendStartX100			= 0;	// ���߼��� �ð��̴�.
	m_nMiniTrendEndX100				= 0;	// ���߼��� �����̴�.
	m_nMiniTrendNewDatePriceX100	= 0;	// ���߼��� �����ڰ��̴�.
	m_nFirstBasePriceX100			= 0;	// ù ���߼��� Real�� �����ÿ� ������ ��ȸ ���� ������ �����Ѵ�.

	// (2004.03.01, ��¿�) ������ ���߼��� �ð��� �����Ѵ�. (���߼��� �����ڰ� ����� �����Ѵ�.
	m_nFixedTrendTime				= 0;

	// (2004.03.05, ��¿�) ���� �ð��� �����ϸ鼭 Real�� �����ϴ� ���� ���� ������ �����̴�.
	//		����, ����� �ð��� ������ �����Ͽ�, ù Real ���Ž� �����ڰ��� �����ϵ��� �Ѵ�.
	m_nMarketStartTime				= 0;
}

// (2004.02.05, ��¿�, �м�) ���� P&F Chart�� ��ǥ�� �����ϴ� �⺻ Routine�̴�.
void CFuOpPF::GetData( const CChartFormulateData &chartFormulateData,
		CGraphCalculateData *pGraphCalculateData, CIndicatorInfo * &pIndicatorInfo,
		CCalculateData &calculateData)
{
	// 1. ���� TR ���Ž��� I/O Packet�� �ʱ�ȭ�Ѵ�.
	//	(�⺻ ��ǥ Data�� �����Ѵ�.)
	CPacketList *pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		// 1.1 Input Packet�� Ȯ���Ͽ� �����Ѵ�. (�ڷ�����, MarkLine �ð�, MarkLine ����, �����ڰ� Pakcet���� �����ȴ�.)
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pIMLDatePacket			= pPacketList->GetBaseData( CDataConversion::GetStringData( strPacketNames, ","));
		m_pIMLStartPricePacket		= pPacketList->GetBaseData( CDataConversion::GetStringData( strPacketNames, ","));
		m_pIMLEndPricePacket		= pPacketList->GetBaseData( CDataConversion::GetStringData( strPacketNames, ","));
		m_pIMLNewDatePricePacket	= pPacketList->GetBaseData( CDataConversion::GetStringData( strPacketNames, ","));
		if( !m_pIMLDatePacket || !m_pIMLStartPricePacket || !m_pIMLEndPricePacket || !m_pIMLNewDatePricePacket) return;
		// 1.2 Output Packet�� Ȯ���Ͽ� �ʱ�ȭ/�����Ѵ�. (CGraphCalculateData���� ��Ͻ��� �ش�.)
		// 1.2.1 Packet List�κ��� Packet�� �˻��Ѵ�.
		m_pOMLStartPricePacket		= pPacketList->GetBaseData( CString( "FUPF_ML") + _MTEXT( C0_OPEN),		FALSE);
		m_pOMLEndPricePacket		= pPacketList->GetBaseData( CString( "FUPF_ML") + _MTEXT( C0_OPEN),		FALSE);
		m_pOMLNewDatePricePacket	= pPacketList->GetBaseData( CString( "FUPF") + "_" + _MTEXT( C0_NEW_DATE_PRICE),	FALSE);
		// 1.2.2 �˻� ���н� ó���� �����Ѵ�.
		if( !m_pOMLStartPricePacket || !m_pOMLEndPricePacket || !m_pOMLNewDatePricePacket) return;
		// 1.2.3 �˻��� Packet�� �ʱ�ȭ�Ѵ�.
		ClearPacketData( m_pOMLStartPricePacket		);
		ClearPacketData( m_pOMLEndPricePacket		);
		ClearPacketData( m_pOMLNewDatePricePacket	);
		// 1.2.4 CGraphCalculateData�� Output Packet�� ��Ͻ�Ų��.
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pGraphCalculateData) return;
		if( pGraphCalculateData->GetCount() < 1) return;
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pSubGraphList) return;
		if( pSubGraphList->GetCount() < 1)	// (ó�� �ѹ��� ��Ͻ����ش�.)
		{
			pSubGraphList->AddTail2( m_pOMLStartPricePacket		);		// ��� ������ �����Ѵ�.
			pSubGraphList->AddTail2( m_pOMLEndPricePacket		);
			pSubGraphList->AddTail2( m_pOMLNewDatePricePacket	);
		}

		// (2004.02.24, ��¿�) �ǽð� Tick P&F Chart�� ����ȣ�� ������ �к� ���սð� ������ ���Ͽ� �����Ѵ�.
		//		(���� Real�̳� TR ó���� �����Ѵ�.)
		CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
		if( !pConditionList) return;
		m_nNewTrendPriceX100 = int( GetCondition( 0, pConditionList) * 100);
		m_nStickTimeWidth = int( GetCondition( 1, pConditionList));

		// 1.3 TR Data�� ������� ��ǥ Data�� �����Ѵ�.
		GetTRFuOpPF();
	}
	// 2. Real�� ��ǥ Data ������ ó���Ѵ�.
	else if( chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL)
		GetRealFuOpPF( calculateData.GetPoint());

	// 3. ���� Real�� ó���� ���Ͽ� Real ���� Packet Data�� Clear�Ѵ�.
	if( m_pIMLDatePacket->GetCount() > 0 && m_pIMLEndPricePacket->GetCount() > 0)
	{
		m_pIMLDatePacket->ClearData(		TRUE);
		m_pIMLEndPricePacket->ClearData(	TRUE);
	}

	// 4. ������ Output Packet�� ��Ͻ�Ų��.
	CString strPriceType = m_pIMLEndPricePacket->GetType( FALSE);
	SetPacketTypes( m_pOMLStartPricePacket,		TYPE_TWODECIMAL, strPriceType, 0);
	SetPacketTypes( m_pOMLEndPricePacket,		TYPE_TWODECIMAL, strPriceType, 0);
	SetPacketTypes( m_pOMLNewDatePricePacket,	TYPE_TWODECIMAL, strPriceType, 0);
}

// (2004.02.05, ��¿�, �м�) ���� P&F Chart�� TR�� ó���ϴ� Routine�� �����Ѵ�.
// (2004.02.24, ��¿�) Server���� �����ִ� ���� Packet, ���߼� Packet, �߼� Packet�� ó���ϰ�,
//		���߼� Packet�� �߰��Ͽ� �������Ѵ�.
void CFuOpPF::GetTRFuOpPF( void)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pIMLDatePacket || !m_pIMLStartPricePacket || !m_pIMLEndPricePacket || !m_pIMLNewDatePricePacket) return;

	// 1. Input Packet�� DataList���� ���Ѵ�.
	CList< double, double> *pMLDateList			= m_pIMLDatePacket->GetnumericDataList();
	CList< double, double> *pMLStartPriceList	= m_pIMLStartPricePacket->GetnumericDataList();
	CList< double, double> *pMLEndPriceList		= m_pIMLEndPricePacket->GetnumericDataList();
	CList< double, double> *pMLNewDatePriceList	= m_pIMLNewDatePricePacket->GetnumericDataList();
	if( !pMLDateList || !pMLStartPriceList || !pMLEndPriceList || !pMLNewDatePriceList) return;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pOMLStartPricePacket || !m_pOMLEndPricePacket || !m_pOMLNewDatePricePacket) return;

	// (2004.02.24, ��¿�) Scale Packet�� ������ �߰��Ͽ�, Drawing�ÿ� ������� �Ѵ�.
	// (2004.03.05, ��¿�) Min ���� 0�� ���� �⺻ ��2 Packet�� ��/������ 0�� �������� ����
	//		�̴� Scale�� ���Ƚ�Ű�� �ʴ´�.
	double dI, dMin = DBL_MAX, dMax = DBL_MIN;
	POSITION posMLStartPrice	= pMLStartPriceList->GetHeadPosition();
	while( posMLStartPrice)
	{
		dI = pMLStartPriceList->GetNext( posMLStartPrice);
		if( 0 < dI)
		{
			if( dI < dMin) dMin = dI;
			else if( dMax < dI) dMax = dI;
		}
	}
	POSITION posMLEndPrice	= pMLEndPriceList->GetHeadPosition();
	while( posMLEndPrice)
	{
		dI = pMLStartPriceList->GetNext( posMLEndPrice);
		if( 0 < dI)
		{
			if( dI < dMin) dMin = dI;
			else if( dMax < dI) dMax = dI;
		}
	}

	// (2004.04.09, ��¿�) �� ���۽�, ���������� ǥ�ð� ����ġ�� Ŀ���� �ʵ���,
	//		�⺻ ���� Ȯ���Ѵ�.
	const double dWidth = 0.5;
	if( dMax - dMin < dWidth)
	{
		int nPaddingX100 = ( ( ( int( dWidth * 100) - int( dMax - dMin) * 100) / 2) / 5) * 5;
		dMin = ( int( dMin * 100) - nPaddingX100) / 100.;
		dMax = ( int( dMax * 100) + nPaddingX100) / 100.;
	}

	m_pOMLStartPricePacket->AppendData(		dMin);
	m_pOMLEndPricePacket->AppendData(		dMax);
	m_pOMLNewDatePricePacket->AppendData(	0);

	// (2004.02.26, ��¿�) ���߼� ���� ������ �ʱ�ȭ�Ѵ�.
	//		Ư��, Server�� Packet�� ������ �ִ� ��츦 �����Ͽ�
	//			Server Packet�� Ȯ���ϱ⿡ �ռ�, ���߼� ���� ������ �ʱ�ȭ�Ѵ�.
	//		Server Packet�� �����ÿ��� ���߼��� �̹� �ִ� ��ó�� �����Ͽ�,
	//			Real�� ���� ���߼� ������ ������� �ʵ��� �Ѵ�.
	m_nMiniTrendTime				= -3;
	m_nMiniTrendStartX100			= 0;
	m_nMiniTrendEndX100				= 0;
	m_nMiniTrendNewDatePriceX100	= 0;

	// 2. MarkLine �ð�/����, �����ڰ� Packet�� Output Packet���� ����ó���Ѵ�.
	POSITION	posMLDate		= pMLDateList->GetHeadPosition();
				posMLStartPrice	= pMLStartPriceList->GetHeadPosition();
				posMLEndPrice	= pMLEndPriceList->GetHeadPosition();
	POSITION	posNewDatePrice	= pMLNewDatePriceList->GetHeadPosition();
	
	// (2004.02.24, ��¿�) Server���� �����ִ� ���� Packet, ���߼� Packet, �߼� Packet�� ó���ϰ�,
	//		���߼� Packet�� �߰��Ͽ� �������Ѵ�.
	// 2.1 ���� ��1 ���� Packet�� �����Ͽ� Real�� ���� ���߼� ���� ������ �ʱ�ȭ�Ѵ�. (�⺻ Packet�� �׻� ���ŵȴ�.)
	//		(��1 ���� Packet�� ���� ��쿡�� Real�̳� ���߼� Packet �������� �ʴ´�.)
	if( !posMLDate || !posMLStartPrice || !posMLEndPrice || !posNewDatePrice) return;
	// 2.1.1 ���� ����� �ð��� �����Ѵ�.
	//	(Real�� ���� ���߼� ������ ������ �� ���� �ð��� �̿�ȴ�.
	//	 Ư�� ���� �ϳ��� ���� ��쿡�� �̸�, ���� ������ �� ���� ���۽ð����� Update�ȴ�.)
	// (2004.03.05, ��¿�) ���� �ð��� �����ϸ鼭 Real�� �����ϴ� ���� ���� ������ �����̴�.
	//		����, ����� �ð��� ������ �����Ͽ�, ù Real ���Ž� �����ڰ��� �����ϵ��� �Ѵ�.
	m_nMarketStartTime = int( pMLDateList->GetNext( posMLDate));
	// 2.1.2 ��ȸ�� ���� ������ �����Ѵ�.
	//	(���߼��� ���� ����� ���߼� ��/������ �̿�ȴ�.
	//	 ���� ���� Real ���Žÿ��� ù ���߼� ������ �ð��� �����ȴ�.)
	m_nFirstBasePriceX100	= int( pMLStartPriceList->GetNext( posMLStartPrice) * 100);
	// 2.1.3 �׿� ���� �����Ѵ�.
	pMLEndPriceList->GetNext(		posMLEndPrice	);
	pMLNewDatePriceList->GetNext(	posNewDatePrice	);
	// 2.1.4 ��2 ���� Packet�� ���� ��쵵 ���� ������ Real�̳� ���߼� Packet�� �������� �ʴ´�.
	if( !posMLDate || !posMLStartPrice || !posMLEndPrice || !posNewDatePrice) return;
	
	// 2.2 ��2 ���� Packet�� �����Ͽ� ������ ������ �����Ѵ�. (Real ���Ž� �̿��Ѵ�.)
	// 2.2.0 ���� �� 2���� Packet���� �̿�ġ �ʴ� ������ Skip�Ѵ�.
	pMLStartPriceList->GetNext(		posMLStartPrice	);
	// (2004.03.01, ��¿�) ��2 �⺻ Packet�� �����ڰ��� Ȯ���Ͽ� ���� Packet�� ��/�� �߼����� Ȯ���Ѵ�.
	//		(0�̸� ���߼� or No Packet, 1�̸� ���߼�)
	int nPacketType = ( int)pMLNewDatePriceList->GetNext(	posNewDatePrice);
	// 2.2.1 ������ ���۽ð��� Ȯ���Ѵ�.
	//	(�� ���� 0�̸� Server���� ��ȸ�� ���� �ϳ��� ���� ����̴�.
	//	 Real ������ ���� ���� ������, �������ð��� �Ǵ��ϱ� ���� ���ؽð����� �̿�ȴ�.)
	m_nLastStickTime = int( pMLDateList->GetNext( posMLDate));
	// 2.2.2 ������ ������ Ȯ���Ѵ�.
	//	(Real�� ���� ������ ������ ������ �����Ͽ� ���߼� Packet�� �����ϴµ� �̿��Ѵ�.)
	int nLastStickPriceX100 = int( pMLEndPriceList->GetNext( posMLEndPrice) * 100);
	// 2.2.3 ��ȸ�� ������ ���۽ð��� ����, Real ó���� ���� ���۽ð��̳� ���� ������ �����Ѵ�.
	//	(���� ���� ��쳪, ������ ������ ��� ����۽ð��� �������� ó���ϱ� ���Ͽ� Update���� �ʴ´�. - �к��� ���
	//		���� �к��� �ִ��ص� �װ��� ������ ������ ���̾�߸� �Ѵ�.
	//	 Ư�� TR�� ������ ���� ������ Real�� ������ ��ó�� ���߼��� ������ ���߼��� �����ϴ�,
	//		���߼� ������ �̿�������,
	//	 Real�� ó���� ���� ���� �����ð��� TR���� �����ϴ� �����ð��� ������ ���� ������ �����Ѵ�.
	//		����, Real�� ���� �ð��� ����۽ð��� �ƴ� ���ο� �ð����� Update���� �ʴ´ٰ� �Ͽ�,
	//		���� ������ Clear��Ű�� ������ ������ �ʵ��� �����Ѵ�.)
	//   �߼� Packet�� �ϳ��� (���߼�/���߼� �Ѵ�) ���� ����, ��ȸ���������� �̿��� ���߼� Packet�� �߰��Ѵ�.
	if( !posMLDate || !posMLStartPrice || !posMLEndPrice || !posNewDatePrice)
	{
		int nStartPriceX100	= m_nFirstBasePriceX100;
		int nEndPriceX100	= m_nFirstBasePriceX100;

		// (2004.02.26, ��¿�) ���߼��� ������ ���� �������� ���� ��츦 ó���Ѵ�.
		//		�̶� ������ Data�� ���� ��� -2, �ִ� ��� -1�� �����Ѵ�.
		if( m_nLastStickTime)
		{
			m_nMiniTrendTime = -1;

			// 2.2.3.1 ���� ������ ���߼��� ������ �����Ѵ�.
			nEndPriceX100	= nLastStickPriceX100;

			// 2.2.3.2 ���߼��� �ð��� ���߼��� �߼����⿡ ���� �����Ѵ�.
			if( m_nFirstBasePriceX100 < nLastStickPriceX100)		nStartPriceX100 = m_nFirstBasePriceX100 + 5;
			else if( nLastStickPriceX100 < m_nFirstBasePriceX100)	nStartPriceX100 = m_nFirstBasePriceX100 - 5;
			else													nStartPriceX100	= m_nFirstBasePriceX100;
		}
		else m_nMiniTrendTime = -2;

		m_pOMLStartPricePacket->AppendData(		nStartPriceX100 / 100.);
		m_pOMLEndPricePacket->AppendData(		nEndPriceX100 / 100.);
		m_pOMLNewDatePricePacket->AppendData(	0);

		return;
	}
	
	// 2.3 ���߼� Packet�� ������ Ȯ���Ͽ� ó���Ѵ�.
	//	(�켱 �����ϰ�, ���߼��� ��/���� ���� ���� ȣ�� ������ ��ġ�� ù �߼��� ��Ͻ�Ų��.
	//	 �װ� Server���� ���߼��� ������ ���� �ƴϱ� �����̴�.)
	// 2.3.1 ���߼� �������� ���Ѵ�.
	double dMLDate			= pMLDateList->GetNext(			posMLDate		);
	double dMLStartPrice	= pMLStartPriceList->GetNext(	posMLStartPrice	);
	double dMLEndPrice		= pMLEndPriceList->GetNext(		posMLEndPrice	);
	double dNewDatePrice	= pMLNewDatePriceList->GetNext(	posNewDatePrice	);
	// 2.3.2 ���߼��� Ȯ�εǴ� ��츦 ó���Ѵ�.
	//	(������ ���߼��� Ȯ�εǴ� ���, ���߼� ���� ������ �ʱ�ȭ�ϰ�,
	//	 ���� Data�� �����Ͽ� ���߼��� �����Ѵ�.)
	// (2004.03.01, ��¿�) Server�� ���� �������� ���߼� Packet�� ���߼��� �ĺ����� �����Ƿ�,
	//		������ Flag (��2 �⺻ Packet�� �����ڰ�)�� Ȯ���Ͽ� ó���Ѵ�.
	if( nPacketType == 1)
	{
		// 2.3.2.1 ���� ���߼��� ������ ���ð��� �޴´�.
		//	(���� Real ó������ ���߼��� Ȯ���ɶ� ��ϵ� �ð��̴�.)
		m_nMiniTrendTime = int( dMLDate);
		// 2.3.2.2 ���߼��� ������ Ȯ���Ѵ�.
		//	(���� Real ó������ �߼� Ȯ����, �ð��� �̿�ȴ�.)
		m_nMiniTrendStartX100 = int( dMLStartPrice * 100);
		// 2.3.2.3 ���߼��� ���� Ȯ���Ѵ�.
		//	(���� Real ó������ �߼� Ȯ����, ������ �̿�ȴ�.)
		m_nMiniTrendEndX100 = int( dMLEndPrice * 100);
		// 2.3.2.4 ���߼��� �����ڰ��� Ȯ���Ѵ�.
		//  (���� Real ó������ �߼� Ȯ������, �����ڰ��� �̿�ȴ�.)
		m_nMiniTrendNewDatePriceX100 = int( dNewDatePrice * 100);
		// 2.3.2.5 ������ ���߼��� ���� Data�� �����Ͽ� ���߼��� ��Ͻ�Ų��.
		// 2.3.2.5.1 ������ ������ �ִ� ���, ���߼��� �����Ͽ� ���߼��� ����Ѵ�.
		//	(���߼��� Drawing Packet���� ��ϵ��� �ʴ´�. ���߼��� �ݿ��Ǿ� ��ϵȴ�.)
		if( m_nLastStickTime)
		{
			// 2.3.2.5.1.2 ������ ���߼��� Ȯ���Ű�� ��츦 ó���Ѵ�. (Ȯ�� ���߼��̸� �������� �����Ѵ�.)
			//	- Ȯ�� ���߼� : ���߼��� �� ��½�Ű�ų�, �϶� �߼��� �� �϶���Ű�� ����̴�.
			//					Ȯ���� ���, ���� �߼��� �ִ°��� Ȯ���Ѵ�. �Ʒ��� IF������ �̹� Ȯ���ϰ� ������ Ȯ���� �� �ִ�.
			//					�󸶳� Ȯ���Ű�� ���� �߿����� �ʴ�. (1ȣ��(0.05)�� Ȯ���ص� �����ȴ�.)
			//	(����, ���߼� ���� ������ �������� �ʴ´�.
			//	 �ֳ��ϸ� ���� Real������ ���߼� ���������� �������� �̿��Ͽ��� �ϴϱ�.)
			if( 0 < ( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) * ( nLastStickPriceX100 - m_nMiniTrendEndX100))
			{
				// 2.3.2.5.1.2.1 ���߼��� �ݿ��Ͽ� ����ϴ� ���߼���, ������ ���� �����ڰ��� Update�Ѵ�.
				//	(�̹� ���߼����� ���߼����� �����ڰ����� ��ϵǾ� �����Ƿ� �������� ��츸 �����ڰ��� �ݿ��ϰ�,
				//		�׷��� ������ �������� �����Ѵ�.
				//	 Ư�� �װ��� ���߼� Ȯ�� ���⿡ ����, ��/������ +/- 0.05�̴�.)
				if( m_nMiniTrendTime / 10000 != m_nLastStickTime / 10000)
				{
					// 2.3.2.5.1.2.2 �϶��� ���, �������� 0.05���� �����Ѵ�.
					if( m_nMiniTrendEndX100 < m_nMiniTrendStartX100)		dNewDatePrice = ( m_nMiniTrendEndX100 - 5) / 100.;
					// 2.3.2.5.1.2.3 ����� ���, �������� 0.05���� �����Ѵ�.
					else if( m_nMiniTrendStartX100 < m_nMiniTrendEndX100)	dNewDatePrice = ( m_nMiniTrendEndX100 + 5) / 100.;
				}
			
				// 2.3.2.5.1.2.4 ���߼��� ���������� ������ ����Ѵ�.
				dMLEndPrice = nLastStickPriceX100 / 100.;
			}
			// 2.3.2.5.1.3 ������ ���߼��� ������ Ȯ���Ű�� ��츦 ó���Ѵ�.
			//	(�߼� ������ Ȯ������ �ʰ�, ���߼��� ������ Ȯ���Ű�� ��츸 Ȯ���Ѵ�.
			//	 ���߼��� ���, ���߼��� �ƴ� ����� ���� �߼��� ���ο� ���߼��� ����ϰԵȴ�.)
			//  - ��Ȯ�� ���߼� : ���߼��� �����ϸ鼭 �ð����� �� �����ϴ� ����̴�.
			//					  �̰�� ���߼��� �⺻ ������ �ð��� �̿��ϰ�, ������ ������ ������ �̿��ϴ�
			//						���ο� ���߼��� ���·� ���߼��� ����Ͽ��� �Ѵ�.
			//					  (���� �������̹Ƿ� ���߼� ���� ������ �������� �ʴ´�.)
			//	(�߼� Ȯ���� ���߼��� ������ ������ ���� Ȯ���Ѵ�.)
			if( 0 < ( m_nMiniTrendStartX100 - m_nMiniTrendEndX100) * ( nLastStickPriceX100 - m_nMiniTrendStartX100))
			{
				// 2.3.2.5.1.3.1 ���߼��� �ݿ��ϴ� ����ϴ� ���߼���, ������ ���� �����ڰ��� Update�Ѵ�.
				//	(�̹� ���߼����� ���߼����� �����ڰ����� ��ϵǾ� �����Ƿ� �������� ��츸 �����ڰ��� �ݿ��ϰ�,
				//		�׷��� ������ �������� �����Ѵ�.
				//	 Ư�� �װ��� ���߼� Ȯ�� ���⿡ ����, ��/������ +/- 0.05�̴�.)
				if( m_nMiniTrendTime / 10000 != m_nLastStickTime / 10000)
				{
					// 2.3.2.5.1.3.2 �϶��� ���, �ð����� 0.05���� �����Ѵ�. (���߼� Ȯ�忡 ����)
					if( m_nMiniTrendEndX100 < m_nMiniTrendStartX100)		dNewDatePrice = ( m_nMiniTrendStartX100 + 5) / 100.;
					// 2.3.2.5.1.3.3 ����� ���, �ð����� 0.05���� �����Ѵ�. (���߼� Ȯ�忡 ����)
					else if( m_nMiniTrendStartX100 < m_nMiniTrendEndX100)	dNewDatePrice = ( m_nMiniTrendStartX100 - 5) / 100.;
				}

				// 2.3.2.5.1.3.4 ���߼��� �ð��� ���߼� �⺻ ������ �ݿ��Ѵ�.
				dMLStartPrice = dMLEndPrice;
				// 2.3.2.5.1.3.5 ���߼��� ������ ������ ������ �ݿ��Ѵ�.
				dMLEndPrice = nLastStickPriceX100 / 100.;
			}

			// 2.3.2.5.1.4 ������ ���߼��� ���� ���� ȣ���������� ū ���,
			//		���߼��� ���� Data�� �����Ͽ� ������ ���߼��� ���߼� �������� �����Ͽ� �����ش�.
			//		(���߼��� ���߼��� �����ɶ�, �ð��� +/- 0.05 ó���Ͽ��� �Ѵ�.
			//		 �ð������� �������� �����Ǵ� ���߼��̱� �����̴�.)
			if( m_nNewTrendPriceX100 <= abs( int( dMLEndPrice * 100) - int( dMLStartPrice * 100)))
			{
				if( dMLStartPrice < dMLEndPrice)	dMLStartPrice = ( int( dMLStartPrice * 100) + 5) / 100.;
				else								dMLStartPrice = ( int( dMLStartPrice * 100) - 5) / 100.;
			}

			// 2.3.2.5.1.5 ������ ���߼��� ������ ���� �ʴ� ��쿡�� ���߼��� �״�� �ݿ��Ѵ�.
			//	(���߼��� ���� �ð��� �������� �ʴ� ��� ��θ� �����Ѵ�. �߼� ������ ���Եȴ�.)
			// 2.3.2.5.1.6 ������ ���� ���߼� ���� dMLStartPrice, dMLEndPrice, dNewDatePrice�� �ݿ��Ǿ����Ƿ�
			//	�Ʒ��� ���߼� ��� Routine���� ó������ �Ѵ�.
		}
		// 2.3.2.5.2 ������ ���� ���߼��� �߰����� ���� ���, ���߼��� �״�� ���߼��� �ݿ��Ѵ�.
		m_pOMLStartPricePacket->AppendData(		dMLStartPrice);
		m_pOMLEndPricePacket->AppendData(		dMLEndPrice	);
		m_pOMLNewDatePricePacket->AppendData(	dNewDatePrice);

		// 2.3.2.6 TR ó���� �����Ѵ�.
		return;
	}
	// 2.3.3 ���߼��� Ȯ�εǴ� ���, �߼��� ��Ͻ�Ų��.
	m_pOMLStartPricePacket->AppendData(		dMLStartPrice	);
	m_pOMLEndPricePacket->AppendData(		dMLEndPrice		);
	m_pOMLNewDatePricePacket->AppendData(	dNewDatePrice	);

	// 2.4 �� �߰��� ���߼��� �ִ��� Ȯ���Ͽ� ��Ͻ�Ų��.
	//	(�� ���������� ���߼��� �̹� �ϳ� ��ϵ� ��Ȳ�̴�.)
	while( posMLDate && posMLStartPrice && posMLEndPrice && posNewDatePrice)
	{
		dMLDate			= pMLDateList->GetNext(			posMLDate		);
		dMLStartPrice	= pMLStartPriceList->GetNext(	posMLStartPrice	);
		dMLEndPrice		= pMLEndPriceList->GetNext(		posMLEndPrice	);
		dNewDatePrice	= pMLNewDatePriceList->GetNext(	posNewDatePrice	);

		m_pOMLStartPricePacket->AppendData(		dMLStartPrice	);
		m_pOMLEndPricePacket->AppendData(		dMLEndPrice		);
		m_pOMLNewDatePricePacket->AppendData(	dNewDatePrice	);
	}

	// (2004.03.01, ��¿�) ������ ���߼��� �ð��� �����Ѵ�. (���߼��� �����ڰ� ����� �����Ѵ�.)
	//		Ư��, �̶��� ���߼� �����ð��� 0���� �����Ͽ� ���߼� ���������� �����Ѵ�.
	m_nMiniTrendTime	= 0;
	m_nFixedTrendTime	= int( dMLDate);

	// 2.5 ���������� ���߼��� �߰��Ѵ�.
	// 2.5.1 ������ ������ �ִ� ���, Ȯ��/���� ���߼��� Ȯ���Ͽ� �߰���Ų��.
	if( m_nLastStickTime)
	{
		//	(���߼� ������ ���߼��� ���������� ��ϵ� �߼��� ������ ��������,
		//	 ���ο� �߼��� ������� Ȯ���ϸ� �߰��Ѵ�.)
		// 2.5.1.0 ���� ���߼��� ���� ������ ���ο� ���߼��� �����ڰ��� �̸� ���Ͽ� ���´�.
		int nMLStartPriceX100	= int( dMLStartPrice	* 100);
		int nMLEndPriceX100		= int( dMLEndPrice		* 100);
		// 2.5.1.1 ���� Ȯ�� �Ǵ� ���� ���߼��� ��츦 ó���Ѵ�.
		//	- Ȯ�� ���߼� : ���� ��� �߼��� �� ��½�Ű�ų�, �϶� �߼��� �� �϶���Ű�� ����̴�.
		//					Ȯ���� ��� ���� �߼��� �ִ°��� Ȯ���Ѵ�. �Ʒ��� IF������ �̹� Ȯ���ϰ� ������ Ȯ���� �� �ִ�.
		//	- ���� ���߼� : ���� ��� �߼��� ���� �϶��ϰų�, �϶� �߼��� ���� ����ϴ� ����̴�.
		//					������ ��쿡�� �߼� ���� ���⸸�� �ƴ϶�, ���߼� ���� ũ�Ⱑ ���� ���� ȣ���� �ش��ϴ����� Ȯ���Ѵ�.
			// (Ȯ�� ���߼��̰ų�)
		if( 0 < ( nMLEndPriceX100 - nMLStartPriceX100) * ( nLastStickPriceX100 - nMLEndPriceX100)
			// (���� ���߼��� ���)
			|| ( ( nMLEndPriceX100 - nMLStartPriceX100) * ( nLastStickPriceX100 - nMLEndPriceX100) < 0
				&& m_nNewTrendPriceX100 <= abs( nLastStickPriceX100 - nMLEndPriceX100)))
		{
			// (2004.02.26, ��¿�) �����ڰ��� Ȯ��/�����Ǵ� ���߼��� �ð��̴�.
			// 2.5.1.1.1 ���� Ȯ��/�����Ǵ� ���߼��� �ð��� ���Ѵ�.
			int nVirtualStartPriceX100 = nMLEndPriceX100 + ( nMLEndPriceX100 < nLastStickPriceX100 ? 5 : -5);
			// 2.5.1.1.2 �������� ��츦 Ȯ���Ͽ� �����ڰ��� Update�Ѵ�. (����. �������� �����Ͽ� Clear��Ű�� �ʴ´�.)
			if( m_nFixedTrendTime / 10000 != m_nLastStickTime / 10000) dNewDatePrice =  nVirtualStartPriceX100 / 100.;
			// (2004.02.27, ��¿�) ���߼� ������ ���߼��� ���� �����ڰ��� �߻����� �ʴ� ��쿡�� 0���� Clear��Ų��.
			else dNewDatePrice = 0;
			// 2.5.1.1.3 ���߼��� ����Ѵ�.
			m_pOMLStartPricePacket->AppendData(		nVirtualStartPriceX100 / 100.);
			m_pOMLEndPricePacket->AppendData(		nLastStickPriceX100 / 100.);
			m_pOMLNewDatePricePacket->AppendData(	dNewDatePrice);
			return;
		}
		// 2.5.1.2 Ȯ��/������ ���߼��� �������� ���� ���, ���� ���߼��� ������ �����Ѵ�.
		// (2004.02.25, ��¿�) ���߼��� Ȯ��/������ �߼��� �������� �ʴ� ���,
		//		�����ڰ����� �������� �ʴ´�.
	}
	// 2.5.2 ������ ���� ���߼��� �߰����� ���� ���, �������� ���߼��� �߰��Ѵ�.
	//	(���߼��� Drawing�ÿ� �����ϰ� ������� �߰��Ͽ��� �Ѵ�. ������ ������ �ѹ� �� �׸����� �Ѵ�.)
	m_pOMLStartPricePacket->AppendData(		dMLEndPrice	);
	m_pOMLEndPricePacket->AppendData(		dMLEndPrice	);
	m_pOMLNewDatePricePacket->AppendData(	0			);
}

//PF ������
// (2004.02.26, ��¿�, �м�) Real�� ó���� TR���� ���� Data�� ó���� ����.
//		�� ������ ó����, ���� ������ �ٸ� ���ο� ������ ������ ���,
//		���� ������ ��/���߼��� �߰��Ѵ�.
void CFuOpPF::GetRealFuOpPF( const CPoint &point)
{
	// 0. ���� Real�� �����ϴ� ��Ȳ���� Ȯ���Ѵ�.
	//	(������� �ð��� ���ٴ� ���� ������ �ƴϸ鼭, ���� ������ �������� �ƴ� ����̴�.)
	if( !m_nMarketStartTime) return;
	CList< double, double> *pOMLStartPriceList		= m_pOMLStartPricePacket->GetnumericDataList();
	CList< double, double> *pOMLEndPriceList		= m_pOMLEndPricePacket->GetnumericDataList();
	CList< double, double> *pOMLNewDatePriceList	= m_pOMLNewDatePricePacket->GetnumericDataList();
	if( !pOMLStartPriceList || !pOMLEndPriceList || !pOMLNewDatePriceList) return;

	// 1. ���� ���ο� Real�� �ð��� ������ ���Ѵ�.
	//		(�ð��� ���� Packet�� ���� Data���� �̹� TR ó���� Clear�Ǿ���.
	//		 ���� ��ϵǾ� �ִ� ù Data���� ó���Ѵ�. => Real�� ���� ������?)
	// 1.1 Real�� ���ŵǾ����� Ȯ���Ѵ�.
	if( !m_pIMLDatePacket->GetCount() || !m_pIMLEndPricePacket->GetCount()) return;
	// 1.2 Real �ð��� ���Ѵ�. (TR�� �����ϴ� �ð��� �����ϰ� �ϱ� ���Ͽ� �ʴ����� �����Ѵ�.)
	// (2004.03.05, ��¿�) ���� �ð��� �����ϸ鼭 Real�� �����ϴ� ���� ���� ������ �����̴�.
	//		����, ����� �ð��� ������ �����Ͽ�, ù Real ���Ž� �����ڰ��� �����ϵ��� �Ѵ�.
	//		Tick Time�� ����� �ð��� �̿��� ���ڸ� �߰��Ѵ�.
	// (2006/5/25 - Seung-Won, Bae) PacketData.dll process Date Setting already.
	//	int	nTickTime		= ( m_nMarketStartTime / 10000) * 10000 + int( m_pIMLDatePacket->GetData( 0)) / 100;
	int	nTickTime		= int( m_pIMLDatePacket->GetData( 0)) / 100;
	// 1.3 Real�� ������ ���Ѵ�.
	int nTickPriceX100	= int( m_pIMLEndPricePacket->GetData( 0) * 100);
	// 1.4 ���ŵ� Real�� ������ Ȯ���Ѵ�.
	if( !nTickTime || !nTickPriceX100) return;

	// 2. ������ �ð����� Real�� �ð��� �� �̸� ���(�� ������ Test Real)����
	//		���������� ó���� �� �ֵ��� �����ð��� Real�ð� �������� �����Ѵ�.
	// (2004.03.05, ��¿�) ���� �ð��� �����ϸ鼭 Real�� �����ϴ� ���� ���� ������ �����̴�.
	//		����, ����� �ð��� ������ �����Ͽ�, ù Real ���Ž� �����ڰ��� �����ϵ��� �Ѵ�.
	//		������ �ð� ����, ������ Real�� ���ŵǴ� �������� Test Real�� ó����,
	//		������ ������ ������ �����Ѵ�.
	// (2004.03.09, ��¿�) Tick�� �ð��� ���� ���ڰ� �߰��ǹǷ� �̸� �̿��Ͽ� ���Ѵ�.
	if( nTickTime < m_nLastStickTime) m_nLastStickTime = ( ( m_nMarketStartTime / 10000 - 1) * 10000) + m_nLastStickTime % 10000;

	// 2.5 ���� ���߼��� Index�� ���Ѵ�.
	//		���߼��� ���ٸ� TR�� ����� �������� ���� ����̴�.
	POSITION posVirtualTrendStart	= pOMLStartPriceList->GetTailPosition();
	POSITION posVirtualTrendEnd		= pOMLEndPriceList->GetTailPosition();
	POSITION posVirtualTrendNewDate = pOMLNewDatePriceList->GetTailPosition();
	if( !posVirtualTrendStart || !posVirtualTrendEnd || !posVirtualTrendNewDate) return;

	// (2004.03.05, ��¿�) Real ó���߿����� Scale�� Ȯ��ǵ���,
	//		Scale Packet�� Update�Ѵ�.
	int nScalePriceX100	= int( pOMLStartPriceList->GetHead() * 100);
	if( nTickPriceX100	< nScalePriceX100)	pOMLStartPriceList->SetAt(	pOMLStartPriceList->GetHeadPosition(),	nTickPriceX100 / 100.);
	nScalePriceX100		= int( pOMLEndPriceList->GetHead() * 100);
	if( nScalePriceX100	< nTickPriceX100)	pOMLEndPriceList->SetAt(	pOMLEndPriceList->GetHeadPosition(),	nTickPriceX100 / 100.);

	// 3. ���� ������ �̹� Real�� ���� Ȯ���Ͽ�, ���� ������ ��/���߼��� �������� �����Ѵ�.
	//		Ư�� Tick P&F�� ��쿡�� ������ ���� ����(Tick)�� ��/���߼��� �����Ų��.
	//		����, �����ð��� ������ ���۽ð����� �� ���̰� �к� ������ ū ���� �Ǵ��Ѵ�.
	// (2004.03.05, ��¿�) �ð��� ��� �񱳽ÿ��� ��ü�� ������ ȯ���ϴ� ������� �Ͽ��� �Ѵ�.
	// (2004.03.05, ��¿�) ���� �ð��� �����ϸ鼭 Real�� �����ϴ� ���� ���� ������ �����̴�.
	//		����, ����� �ð��� ������ �����Ͽ�, ù Real ���Ž� �����ڰ��� �����ϵ��� �Ѵ�.
	//		���� �����ð��� 0�� ���(��������)�� �����Ѵ�.
	// (2004.03.05, ��¿�) ������ ������ ���� ��쵵 ó���ǵ��� �����Ѵ�.
	if( m_nLastStickTime
		&& ( !m_nStickTimeWidth
			|| m_nLastStickTime / 10000 < nTickTime / 10000
			|| ( ( m_nLastStickTime % 10000) / 100) * 60 + m_nLastStickTime % 100 + m_nStickTimeWidth
				<= ( ( nTickTime % 10000) / 100) * 60 + nTickTime % 100))
	{
		// 3.1 ���� ���߼��� �������� ���� ���¿���, ù Data�� �޴� ��츦 ó���Ѵ�.
		//		(������ ������ ���� ���߼��� �������� ���� �����̴�.
		//		 �̰��, ���� �ð��� ����۽ð��̶� Update�� ���߼� ���� Data�� ����.)
		if( m_nMiniTrendTime == -2);
		// 3.2 ���߼��� ������ ó���Ѵ�.
		else if( m_nMiniTrendTime != 0)
		{
			// 3.2.1 ���� ���߼��� �������� �ʰ�, ���ο� ���߼� ������ �����ϴ� ��츦 ó���Ѵ�.
			// (2004.03.03, ��¿�) ���ο� ���߼��� ��Ͻ��� ���߼��� ���뿡����,
			//		���߼��� �̹� ���߼��� ���·� �����Ͽ� ��ϵ� ���� �����Ͽ� �����Ѵ�.
			//		(���߼��� �������� �����Ѵ�. �ð��� �̹� ��¿����� �����Ǿ� �ִ�.)
			if( m_nMiniTrendTime == -1)
			{
				// 3.2.1.1 ���ο� ���߼� ������ ���� ������ �����Ѵ�.
				m_nMiniTrendTime				= m_nLastStickTime;
				// 3.2.1.2 ������ �����Ǵ� ���߼��� �ð���, ��ȸ���� �����̴�.
				//	(��ȸ���� ������ �������� �ϸ�, ���߼��� ������ �����ϵ��� �����Ѵ�.)
				m_nMiniTrendStartX100			= m_nFirstBasePriceX100;
				// 3.2.1.3 ������ �����Ǵ� ���߼��� ������, ���߼��� �����̴�.
				m_nMiniTrendEndX100				= int( pOMLEndPriceList->GetAt( posVirtualTrendEnd) * 100);
				// 3.2.1.4 ������ �����Ǵ� ���߼��� �����ڰ��� ����.
				m_nMiniTrendNewDatePriceX100	= 0;
			}
			// 3.2.2 ���߼� ������ ��츦 ó���Ѵ�.
			//		���߼��� ������ ���, ������ ������ �����ǹǷ�,
			//		 ������ ������ �����Ͽ� ����� ���߼��� ���ο� ���߼��� �����ȴ�.
			// (2004.03.03, ��¿�) ���߼��� ������ ���߼����� ������ ���� ���� ���̴�.
			//		���߼��� �̹� ���� ���߼����� Tick�� ���յ� ���·� ������ ���̹Ƿ�,
			//		������ �����Ͽ� ������ �����Ѵ�. �׷��� ������ ���߼� �Ǵ��� �����ϴ�.
			//		(������ �Ǵܾ��� �׳� ���߼��� ���߼��� �ٷ� ���߼��� �����ϴ� Logic�� �����ϱ� ���ؼ���,
			//		 ������ Flag ������ �ʿ��ϹǷ�, �׷��� ó�������ʰ�, ���⼭ ���߼��� �ٽ� �����Ѵ�.)
			// (2004.03.04, ��¿�) �߼��� �ð��� �߼��� Ȯ���Ų ��쿡�� Update�Ѵ�.
			else if( 0 < m_nMiniTrendTime)
			{
				// 3.2.2.2 ������ �����Ǵ� ���߼��� �����ڰ���, ���߼��� �����ڰ��̴�.
				m_nMiniTrendNewDatePriceX100	= int( pOMLNewDatePriceList->GetAt(	posVirtualTrendNewDate) * 100);
				// 3.2.2.3 ������ �����Ǵ� ���߼��� �ð��� ������ �����Ѵ�.
				//		(�Ѵ� �������� �ʴ´�. ���� ���߼��� ���߼��� ���, ���߼����� ���� ������ �̹� ���ܵǾ� �ִ�.)
				int nVirtualTrendEnd			= int( pOMLEndPriceList->GetAt(		posVirtualTrendEnd)		* 100);
				// 3.2.2.3.1 ������ �����ϴ� ��츦 ó���Ѵ�. (���߼��� �����ϴ� ���)
				if( 0 < ( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) * ( nVirtualTrendEnd - m_nMiniTrendEndX100)
					&& abs( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) < abs( nVirtualTrendEnd - m_nMiniTrendStartX100))
				{
					// 3.2.2.3.1.1 ���ο� ���߼� ������ ���� ������ �����Ѵ�.
					m_nMiniTrendTime	= m_nLastStickTime;
					// 3.2.2.3.1.2 �߼��� �������� Ȯ���Ѵ�.
					m_nMiniTrendEndX100	= nVirtualTrendEnd;
				}
				// 3.2.2.3.2 �ð��� �����ϴ� ��츦 ó���Ѵ�. (���߼��� �ݴ� Ȯ���ϴ� ���)
				else if( ( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) * ( nVirtualTrendEnd - m_nMiniTrendEndX100) < 0
					&& abs( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) < abs( nVirtualTrendEnd - m_nMiniTrendEndX100))
				{
					// 3.2.2.3.2.1 ���ο� ���߼� ������ ���� ������ �����Ѵ�.
					m_nMiniTrendTime		= m_nLastStickTime;
					// 3.2.2.3.2.2 �߼��� ������ ��ȯ�Ѵ�.
					m_nMiniTrendStartX100	= m_nMiniTrendEndX100;
					m_nMiniTrendEndX100		= nVirtualTrendEnd;
				}
			}
			// 3.2.3 ���߼� ���� ���°� ������ ��츦 ó���Ѵ�.
			else return;

			// 3.2.4 ������ ���߼� ������ ���߼��� �����Ǵ� ��츦 ó���Ѵ�.
			if( m_nNewTrendPriceX100 <= abs( m_nMiniTrendEndX100 - m_nMiniTrendStartX100))
			{
				// (2004.03.01, ��¿�) ������ ���߼��� �ð��� �����Ѵ�. (���߼��� �����ڰ� ����� �����Ѵ�.)
				m_nFixedTrendTime = m_nMiniTrendTime;
				// 3.2.4 ���� �������� ���߼��� ���߼��� �߰��Ѵ�. (���߼����� �Ǿտ� �߰��Ѵ�.)
				// 3.2.4.1 ���߼� ���� �ð��� ó���Ѵ�. (���߼��� �ִ� ������ �����Ѵ�.)
				m_nMiniTrendTime = 0;
				// 3.2.4.2 ���߼� �ð��� ó���Ѵ�.
				// (2004.03.01, ��¿�) ���߼��� ���߼��� �����Ǵ� ���������� �ð��� ���� �߼��� ������ �򰡵ǹǷ�,
				//		���ο� �߼��� ���⿡ ���� +/- 0.05�� ó���Ͽ��� �Ѵ�.
				// (2004.03.09, ��¿�) ù ���߼�����ص� ù Packet�� Scale Packet�̴�.
				//		���� ���߼� �������� �߰����Ѿ� �Ѵ�.
				pOMLStartPriceList->InsertBefore(	posVirtualTrendStart,	( m_nMiniTrendStartX100 + ( m_nMiniTrendStartX100 < m_nMiniTrendEndX100 ? 5 : -5))  / 100.);
				// 3.2.4.3 ���߼� ������ ó���Ѵ�.
				pOMLEndPriceList->InsertBefore(		posVirtualTrendEnd,		m_nMiniTrendEndX100 / 100.);
				// 3.2.4.4 ���߼� �����ڰ��� ó���Ѵ�.
				pOMLNewDatePriceList->InsertBefore(	posVirtualTrendNewDate,	m_nMiniTrendNewDatePriceX100	/ 100.);
			}
		}
		// 3.3 ���߼� ������ ��츦 ó���Ѵ�.
		//		���߼��� ������ ���, ���߼��� Ȯ��/�����ϴ��� Ȯ���Ͽ� ó���Ѵ�.
		else if( !m_nMiniTrendTime)
		{
			// 3.3.1 ���� ���߼��� ��/��/�����ڰ��� Ȯ���Ѵ�.
			//		(�̶� ���߼��� Position�� ���� �� �ִ� ������� ó���Ѵ�.)
			POSITION posLastFixedStart		= posVirtualTrendStart;
			POSITION posLastFixedEnd		= posVirtualTrendEnd;		
			POSITION posLastFixedNewDate	= posVirtualTrendNewDate;
			int nVirtualStartPriceX100		= int( pOMLStartPriceList->GetPrev(		posLastFixedStart	) * 100);
			int nVirtualEndPriceX100		= int( pOMLEndPriceList->GetPrev(		posLastFixedEnd		) * 100);
			int nVirtualNewDatePriceX100	= int( pOMLNewDatePriceList->GetPrev(	posLastFixedNewDate	) * 100);
			// 3.3.2 ���� ���߼��� Ȯ���Ѵ�.
			int nLastFixedStartPriceX100		= int( pOMLStartPriceList->GetAt(	posLastFixedStart)		* 100);
			int nLastFixedEndPriceX100			= int( pOMLEndPriceList->GetAt(		posLastFixedEnd)		* 100);
			// 3.3.3 ���� ���߼��� Ȯ���ϴ� ��츦 ó���Ѵ�.
			// (2004.03.01, ��¿�) �������� ������ ���� ���߼��� �������� �̹� ���߼��� ���������� ���� �����Ѵ�.
			// (2004.03.04, ��¿�) �߼��� Ȯ���� ���߼��� �ð� -> ���� -> ���߼� ������ �̾����� ��Ȳ�� ���Ѵ�.
			if( 0 < ( nLastFixedEndPriceX100 - nLastFixedStartPriceX100) * ( nVirtualEndPriceX100 - nLastFixedEndPriceX100))
			{
				// 3.3.3.1 ���߼��� ������ �����Ѵ�.
				pOMLEndPriceList->SetAt( posLastFixedEnd, nVirtualEndPriceX100 / 100.);
				// 3.3.3.2 ���߼��� �����ڰ��� 0�� �ƴϸ� ���߼��� Update�Ѵ�.
				//		�̰�� ���� ���߼��� �����ڰ��� �־��ٸ�, �װ��� ���� �߼��ȿ� ���ڰ� �ι�° �ٲ�� ������,
				//		�ֱ��� �͸��� �����Ѵ�. ~ �� Server Packet������ 1�� �߼����� �� �����ڰ��� ������� (����ö ���� ����, 2004.02.26)
				if( nVirtualNewDatePriceX100) pOMLNewDatePriceList->SetAt( posLastFixedNewDate, nVirtualNewDatePriceX100 / 100.);
			}
			// 3.3.4 ���� ���߼��� �����ϴ� ��츦 �����Ѵ�.
			//		���� ���߼��� �����ϴ� ���߼��� ����� �׳� ���߼��� ���߼��� �����ϴ� ������ ó���ȴ�.
			//		�� ���ο� ���� ���߼��� ����Ͽ� ���߼��� �����Ѵ�.
			// (2004.03.01, ��¿�) �������� ������ ���� ���߼��� �������� �̹� ���߼��� ���������� ���� �����Ѵ�.
			else if( ( nLastFixedEndPriceX100 - nLastFixedStartPriceX100) * ( nVirtualEndPriceX100 - nLastFixedEndPriceX100) < 0
				&& m_nNewTrendPriceX100 <= abs( nLastFixedEndPriceX100 - nVirtualEndPriceX100))
			{
				posVirtualTrendStart	= pOMLStartPriceList->AddTail(		nVirtualEndPriceX100 / 100.);
				posVirtualTrendEnd		= pOMLEndPriceList->AddTail(		nVirtualEndPriceX100 / 100.);
				posVirtualTrendNewDate	= pOMLNewDatePriceList->AddTail(	0.);
			}
			// 3.3.4 ���� ���߼��� ���ԵǴ� ��츦 �����Ѵ�.
			//	(Ư���� �۾��� �������� �ʴ´�. ���߼��� �״�� �д�.)
			else;
			// 3.3.5 (2004.03.01, ��¿�) ������ ���߼��� �ð��� �����Ѵ�. (���߼��� �����ڰ� ����� �����Ѵ�.)
			m_nFixedTrendTime = m_nLastStickTime;
		}
	
		// 3.4 ������ ������ �����Ͽ����Ƿ�, ���ο� �����ð��� �����Ͽ� ��������Ѵ�.
		//		�Ʒ� ������ ��Ȯ�� ���ذ� �ʿ��ϴ�.
		//	(����, �����ð��� �׻� �� ���սð����� ���� �������ٴ� ������ ����. ex:14�к�)
		//		�Ʒ��� ������ ������ �ǹ��ϴ��� ��Ȯ�� ������ ��~!
		//		(����۽ð� nStartT, nBarW �к� �ð���, ������ �к� ���۽ð��� nLastBarT, �̶�� �Ҷ�,
		//		 nStartT = nVirtualBarC * nBarW + nStartTMod	(nVirtualBarC�� 0�ú��� ����� �ð������� �����̴�.)
		//		 nLastBarT = ( nVirtualBarC + nRealBarC) * nBarW + nStartTMod ��� ǥ���� �� �ִ�. (nRealBarC�� ������� �ϼ��� �����̴�. �ϳ��� �̿ϼ�)
		//		����, �� Tick �ð��� nTickT�� �����ϴ� ������ �к��� �����Ѵٸ�,
		//		 nTickT = ( nVirtualBarC + nRealBarC) * nBarW + nStartTMod + nBarTMod �̴�. (nBarTMod�� nTickT�� �����ϴ� �к����� ������ �ð��̴�.)
		//		�׷��Ƿ�, nTickT = ( nVirtualBarC + nRealBarC) * nBarW + nStartTMod + nBarTMod 
		//				nTickT = ( nVirtualBarC * nBarW + nStartTMod) + ( nRealBarC * nBarW + nBarTMod)
		//				nBarTMod = ( nTickT - ( nVirtualBarC * nBarW + nStartTMod)) % nBarW
		//						 = ( nTickT + nA * nBarW - ( nVirtualBarC * nBarW + nStartTMod)) % nBarW
		// (2004.03.05, ��¿�) ���� �ð��� �����ϸ鼭 Real�� �����ϴ� ���� ���� ������ �����̴�.
		//		����, ����� �ð��� ������ �����Ͽ�, ù Real ���Ž� �����ڰ��� �����ϵ��� �Ѵ�.
		//		�̰�� ������ ������ �ƴϸ� ����ۺ��� ������Ų��.
		if( m_nLastStickTime < m_nMarketStartTime) m_nLastStickTime = m_nMarketStartTime;
		else if( m_nStickTimeWidth)
		{
			// 3.4.1 ���� Tick�ð��� ����ȭ��Ų��. (��ü �м��� �����Ѵ�.)
			int nNTickTime = nTickTime % 10000;
			nNTickTime = ( nNTickTime / 100) * 60 + nNTickTime % 100;
			// 3.4.2 ������ ���ð��� ����ȭ��Ų��. (��ü �м��� �����Ѵ�.)
			int nNLastStickTime = m_nLastStickTime % 10000;
			nNLastStickTime = ( nNLastStickTime / 100) * 60 + nNLastStickTime % 100;
			// 3.4.3 Tick�� �����ϴ� ���� ���۽ð��� ���Ѵ�.
			int nBarTMod = ( nNTickTime - nNLastStickTime) % m_nStickTimeWidth;
			nNTickTime -= nBarTMod;
			// 3.4.4 ���ο� Tick�� �����ϴ� ���� ���۽ð��� �����Ѵ�.
			m_nLastStickTime = ( ( nTickTime / 10000) * 10000)
				+ ( ( nNTickTime / 60) * 100) + ( nNTickTime % 60); // ���ڴ� ������ ���¿��� �úи� ������ 60���� ǥ��� �����Ѵ�.
		}
		else m_nLastStickTime = nTickTime;
	}
	// (2004.03.05, ��¿�) ���� �ð��� �����ϸ鼭 Real�� �����ϴ� ���� ���� ������ �����̴�.
	//		����, ����� �ð��� ������ �����Ͽ�, ù Real ���Ž� �����ڰ��� �����ϵ��� �Ѵ�.
	//		���� �����ð��� 0��(��������) ���, ���� ������ �ð��� ����۽ð����� �����Ѵ�.
	if( !m_nLastStickTime) m_nLastStickTime = m_nMarketStartTime;

	// 4. ���ŵ� Real Data�� �̿��� ���߼��� ������Ų��.
	//	(����, ���߼� ���������� ���߼��� �������� �ʴ´�. Tick�� �ٷ� ���� Tick�� ���� RollBack�� �� �ֱ� �����̴�.)
	// 4.1 ���� ���߼��� �������� ���� ���¿���, ù Data�� �޴� ��츦 ó���Ѵ�.
	//		(���� ������ �������� ù ���߼��� �����Ѵ�.
	//		 �̰�� �̹� ���߼��� ������ ���(m_nMiniTrendTime == -1)�� �Բ� ó���Ѵ�.
	//		 ������ ������ȸ������ �������� ó���ϱ� �����̴�.)
	if( m_nMiniTrendTime == -2 || m_nMiniTrendTime == -1)
	{
		// 4.1.1 ���� ���߼� ������ �����Ѵ�.
		pOMLEndPriceList->SetAt(		posVirtualTrendEnd,		nTickPriceX100	/ 100.	);
		// 4.1.2 ��ȸ ���� ������ �������� ���߼� �ð��� �����Ѵ�.
		// (2004.03.03, ��¿�) �ʱ� ���߼� ���� ���߼��� ��½� �ð��� ���� ���� +/-0.05�̴�.
		if( m_nFirstBasePriceX100 < nTickPriceX100)			nTickPriceX100 = m_nFirstBasePriceX100 + 5;
		else if( nTickPriceX100 < m_nFirstBasePriceX100)	nTickPriceX100 = m_nFirstBasePriceX100 - 5;
		pOMLStartPriceList->SetAt(		posVirtualTrendStart,	nTickPriceX100	/ 100.	);
		// 4.1.3 ���߼��� ���� ��Ȳ������ ù ���߼����� �����ڰ��� �������� �ʴ´�.
		pOMLNewDatePriceList->SetAt(	posVirtualTrendNewDate,	0						);
		// (2004.03.03, ��¿�) ������ ���� ���߼��� �����Ǿ����Ƿ�, m_nMiniTrendTime�� -1�� ��ȯ�Ѵ�.
		m_nMiniTrendTime = -1;
	}
	// 4.2 ���߼��� ���� ������ ���� ��츦 ó���Ѵ�.
	//		(���߼��� �����Ͽ� ���߼��� �����Ѵ�.)
	else if( 0 < m_nMiniTrendTime)
	{
		// 4.2.0 ���߼��� Tick�� �ݿ��Ͽ� ���߼��� �����ϱ� ����, �⺻������ ���߼��� ���� ���Ͽ� ���´�.
		double dStartPrice		= m_nMiniTrendStartX100			/ 100.;
		double dEndPrice		= m_nMiniTrendEndX100			/ 100.;
		double dNewDatePrice	= m_nMiniTrendNewDatePriceX100	/ 100.;

		// 4.2.1 Tick�� ���߼��� Ȯ���Ű�� ��츦 ó���Ѵ�. (Ȯ�� ���߼��̸� �������� �����Ѵ�.)
		//	- Ȯ�� ���߼� : ���߼��� �� ��½�Ű�ų�, �϶� �߼��� �� �϶���Ű�� ����̴�.
		//					Ȯ���� ���, ���� �߼��� �ִ°��� Ȯ���Ѵ�. �Ʒ��� IF������ �̹� Ȯ���ϰ� ������ Ȯ���� �� �ִ�.
		//					�󸶳� Ȯ���Ű�� ���� �߿����� �ʴ�. (1ȣ��(0.05)�� Ȯ���ص� �����ȴ�.)
		//	(����, ���߼� ���� ������ �������� �ʴ´�.
		//	 �ֳ��ϸ� ���� Real������ ���߼� ���������� �������� �̿��Ͽ��� �ϴϱ�.)
		if( 0 < ( m_nMiniTrendEndX100 - m_nMiniTrendStartX100) * ( nTickPriceX100 - m_nMiniTrendEndX100))
		{
			// 4.2.1.1 ���߼��� �ݿ��Ͽ� ����ϴ� ���߼���, Tick�� ���� �����ڰ��� Update�Ѵ�.
			//	(�̹� ���߼����� ���߼����� �����ڰ����� ��ϵǾ� �����Ƿ� �������� ��츸 �����ڰ��� �ݿ��ϰ�,
			//		�׷��� ������ �������� �����Ѵ�.
			//	 Ư�� �װ��� ���߼� Ȯ�� ���⿡ ����, ��/������ +/- 0.05�̴�.)
			if( m_nMiniTrendTime / 10000 != m_nLastStickTime / 10000)
			{
				// 4.2.1.1.1 �϶��� ���, �������� 0.05���� �����Ѵ�.
				if( m_nMiniTrendEndX100 < m_nMiniTrendStartX100)		dNewDatePrice = ( m_nMiniTrendEndX100 - 5) / 100.;
				// 4.2.1.1.2 ����� ���, �������� 0.05���� �����Ѵ�.
				else if( m_nMiniTrendStartX100 < m_nMiniTrendEndX100)	dNewDatePrice = ( m_nMiniTrendEndX100 + 5) / 100.;
			}
			// 4.2.1.2 ���߼��� ������ Tick ������ �����Ų��.
			dEndPrice = nTickPriceX100 / 100.;
		}
		// 4.2.2 Tick�� ���߼��� ������ Ȯ���Ű�� ��츦 ó���Ѵ�.
		//	(�߼� ������ Ȯ������ �ʰ�, ���߼��� ������ Ȯ���Ű�� ��츸 Ȯ���Ѵ�.
		//	 ���߼��� ���, ���߼��� �ƴ� ����� ���� �߼��� ���ο� ���߼��� ����ϰԵȴ�.)
		//  - ��Ȯ�� ���߼� : ���߼��� �����ϸ鼭 �ð����� �� �����ϴ� ����̴�.
		//					  �̰�� ���߼��� �⺻ ������ �ð��� �̿��ϰ�, Tick�� ������ ������ �̿��ϴ�
		//						���ο� ���߼��� ���·� ���߼��� ����Ͽ��� �Ѵ�.
		//					  (���� �������̹Ƿ� ���߼� ���� ������ �������� �ʴ´�.)
		//	(�߼� Ȯ���� ���߼��� ������ ������ ���� Ȯ���Ѵ�.)
		if( 0 < ( m_nMiniTrendStartX100 - m_nMiniTrendEndX100) * ( nTickPriceX100 - m_nMiniTrendStartX100))
		{
			// 4.2.2.1 ���߼��� �ݿ��ϸ� ����ϴ� ���߼���, Tick�� ���� �����ڰ��� Update�Ѵ�.
			//	(�̹� ���߼����� ���߼����� �����ڰ����� ��ϵǾ� �����Ƿ� �������� ��츸 �����ڰ��� �ݿ��ϰ�,
			//		�׷��� ������ �������� �����Ѵ�.
			//	 Ư�� �װ��� ���߼� Ȯ�� ���⿡ ����, ��/������ +/- 0.05�̴�.)
			if( m_nMiniTrendTime / 10000 != m_nLastStickTime / 10000)
			{
				// 4.2.2.1.1 �϶��� ���, �ð����� 0.05���� �����Ѵ�. (���߼� Ȯ�忡 ����)
				if( m_nMiniTrendEndX100 < m_nMiniTrendStartX100)		dNewDatePrice = ( m_nMiniTrendStartX100 + 5) / 100.;
				// 4.2.2.1.2 ����� ���, �ð����� 0.05���� �����Ѵ�. (���߼� Ȯ�忡 ����)
				else if( m_nMiniTrendStartX100 < m_nMiniTrendEndX100)	dNewDatePrice = ( m_nMiniTrendStartX100 - 5) / 100.;
			}

			// 4.2.2.2 ���߼��� �ð��� ���߼� �⺻ ������ �����Ѵ�.
			dStartPrice = dEndPrice;
			// 4.2.2.3 ���߼��� ������ Tick�� �������� �ݿ��Ѵ�.
			dEndPrice = nTickPriceX100 / 100.;
		}

		// 4.2.3 ������ ���߼��� ���� ���� ȣ���������� ū ���,
		//		���߼��� ���� Data�� �����Ͽ� ������ ���߼��� ���߼� �������� �����Ͽ� �����ش�.
		//		(���߼��� ���߼��� �����ɶ�, �ð��� +/- 0.05 ó���Ͽ��� �Ѵ�.
		//		 �ð������� �������� �����Ǵ� ���߼��̱� �����̴�.)
		if( m_nNewTrendPriceX100 <= abs( int( dEndPrice * 100) - int( dStartPrice * 100)))
		{
			if( dStartPrice < dEndPrice)	dStartPrice = ( int( dStartPrice * 100) + 5) / 100.;
			else							dStartPrice = ( int( dStartPrice * 100) - 5) / 100.;
		}

		// 4.2.4 Tick�� ���߼��� ������ ���� �ʴ� ��쿡�� ���߼��� �״�� �ݿ��Ѵ�.
		//	(���߼��� ���� �ð��� �������� �ʴ� ��� ��θ� �����Ѵ�. �߼� ������ ���Եȴ�.)
		pOMLStartPriceList->SetAt(			posVirtualTrendStart,	dStartPrice		);
		pOMLEndPriceList->SetAt(			posVirtualTrendEnd,		dEndPrice		);
		pOMLNewDatePriceList->SetAt(		posVirtualTrendNewDate,	dNewDatePrice	);
	}
	// 3.3 ���߼� ������ ��츦 ó���Ѵ�.
	//		���߼��� ������ ���, Tick�� �߼��� Ȯ��/�����ϴ��� Ȯ���Ͽ� ���߼��� �����Ѵ�.
	//		(���߼� ������ ���߼��� ���������� ��ϵ� �߼��� ������ ��������,
	//		 ���ο� �߼��� ������� Ȯ���ϸ� �߰��Ѵ�.)
	else
	{
		// 3.3.1 ���� ���߼��� ������ Ȯ���Ѵ�.
		// 3.3.1.1 ������ ���߼��� Position�� ���Ѵ�.
		POSITION posLastFixedStart		= posVirtualTrendStart;
		POSITION posLastFixedEnd		= posVirtualTrendEnd;		
		POSITION posLastFixedNewDate	= posVirtualTrendNewDate;
		pOMLStartPriceList->GetPrev(	posLastFixedStart	);
		pOMLEndPriceList->GetPrev(		posLastFixedEnd		);
		pOMLNewDatePriceList->GetPrev(	posLastFixedNewDate	);
		// (2004.04.19, ��¿�) UI�󿡼� ���� ������ �����Ͽ� �����, Output Packet�� Clear�Ǵ� ��츦 �����Ͽ�,
		//		NULL Position�� �����Ѵ�.
		if( !posLastFixedStart || !posLastFixedEnd || !posLastFixedNewDate) return;
		// 3.3.1.2 ���� ���߼��� Ȯ���Ѵ�.
		int nLastFixedStartPriceX100		= int( pOMLStartPriceList->GetAt(	posLastFixedStart)		* 100);
		int nLastFixedEndPriceX100			= int( pOMLEndPriceList->GetAt(		posLastFixedEnd)		* 100);
		// 3.3.1.3 ���� Ȯ�� �Ǵ� ���� ���߼��� ��츦 ó���Ѵ�.
		//	- Ȯ�� ���߼� : ���� ��� �߼��� �� ��½�Ű�ų�, �϶� �߼��� �� �϶���Ű�� ����̴�.
		//					Ȯ���� ��� ���� �߼��� �ִ°��� Ȯ���Ѵ�. �Ʒ��� IF������ �̹� Ȯ���ϰ� ������ Ȯ���� �� �ִ�.
		//	- ���� ���߼� : ���� ��� �߼��� ���� �϶��ϰų�, �϶� �߼��� ���� ����ϴ� ����̴�.
		//					������ ��쿡�� �߼� ���� ���⸸�� �ƴ϶�, ���߼� ���� ũ�Ⱑ ���� ���� ȣ���� �ش��ϴ����� Ȯ���Ѵ�.
			// (Ȯ�� ���߼��̰ų�)
		if( 0 < ( nLastFixedEndPriceX100 - nLastFixedStartPriceX100) * ( nTickPriceX100 - nLastFixedEndPriceX100)
			// (���� ���߼��� ���)
			|| ( ( nLastFixedEndPriceX100 - nLastFixedStartPriceX100) * ( nTickPriceX100 - nLastFixedEndPriceX100) < 0
				&& m_nNewTrendPriceX100 <= abs( nTickPriceX100 - nLastFixedEndPriceX100)))
		{
			// (2004.02.26, ��¿�) �����ڰ��� Ȯ��/�����Ǵ� ���߼��� �ð��̴�.
			// 3.3.1.3.1 ���� Ȯ��/�����Ǵ� ���߼��� �ð��� ���Ѵ�.
			int nVirtualStartPriceX100 = nLastFixedEndPriceX100 + ( nLastFixedEndPriceX100 < nTickPriceX100 ? 5 : -5);
			// 3.3.1.3.2 �������� ��츦 Ȯ���Ͽ� �����ڰ��� Update�Ѵ�. (����. �������� �����Ͽ� Clear��Ű�� �ʴ´�.)
			// (2004.02.27, ��¿�) ���߼� ������ ���߼��� ���� �����ڰ��� �߻����� �ʴ� ��쿡�� 0���� Clear��Ų��.
			double dNewDatePrice = 0;
			if( m_nFixedTrendTime / 10000 != m_nLastStickTime / 10000) dNewDatePrice =  nVirtualStartPriceX100 / 100.;
			// 3.3.1.3.3 ���߼��� �����Ѵ�.
			pOMLStartPriceList->SetAt(			posVirtualTrendStart,	nVirtualStartPriceX100	/ 100.	);
			pOMLEndPriceList->SetAt(			posVirtualTrendEnd,		nTickPriceX100			/ 100.	);
			pOMLNewDatePriceList->SetAt(		posVirtualTrendNewDate,	dNewDatePrice					);
			return;
		}
		// 3.3.1.4 Ȯ��/������ ���߼��� �������� ���� ���, ���� ���߼��� ������ �����Ѵ�.
		//		���߼��� Ȯ��/������ �߼��� �������� �ʴ� ���, �����ڰ����� �������� �ʴ´�.
		//		��, Tick�� ���� ���߼��� �߰����� ���� ���, �������� ���߼��� �����Ѵ�.
		//			���߼��� Drawing�ÿ� �����ϰ� ������� �߰��Ͽ��� �Ѵ�. ������ ������ �ѹ� �� �׸����� �Ѵ�.
		pOMLStartPriceList->SetAt(			posVirtualTrendStart,	nLastFixedEndPriceX100	/ 100.	);
		pOMLEndPriceList->SetAt(			posVirtualTrendEnd,		nLastFixedEndPriceX100	/ 100.	);
		pOMLNewDatePriceList->SetAt(		posVirtualTrendNewDate,	0								);
	}
}



//////////////////////////////////////////////////////////////////////////////////////////
// ��ǥ name : �ϸ����ǥ
// �ֱ� : 9, 26 (dcondition : 2����)
// ��ȯ��, ���ؼ�, ���ེ��, ���ེ��1, 2, ������
// 
//////////////////////////////////////////////////////////////////////////////////////////
CGlanceBalance::CGlanceBalance() : 
	m_pLeadSpan1(NULL), 
	m_pLeadSpan2(NULL),
	m_pHighPriceList(NULL),
	m_pLowPriceList(NULL),
	m_pEndPriceList(NULL),
	m_pHighPacket(NULL),
	CIndicator( IMetaTable::EDDC_OCX_GLANCEBALANCECHART)
{
}

void CGlanceBalance::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pHighPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pHighPriceList = GetnumericDataList(m_pHighPacket);
		m_pLowPriceList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
		m_pEndPriceList = GetnumericDataList(pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ",")));
	}
	if(!IsValidPacket(m_pHighPriceList) || !IsValidPacket(m_pLowPriceList) || !IsValidPacket(m_pEndPriceList))
		return;

	CPacket* pTuringPoint = NULL;
	CPacket* pStandardLine = NULL;
	CPacket* pLagSpan = NULL;
	if(!SetGraphPacketList(chartFormulateData.GetCalculateType(), pPacketList, pGraphCalculateData, pIndicatorInfo, 
						  pTuringPoint, pStandardLine, pLagSpan))
		return;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pIndicatorInfo) return;
	
	//sy 2005.11.03. -> ���ǰ� ��ġ ����
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dStandardCond = GetCondition(0, pConditionList);
	double dTuringCond = GetCondition(1, pConditionList);
	// �ϸ����ǥ ����, ���� ���� ���� - ojtaso (20071226)
	double dLeadSpanCond1 = GetCondition(2, pConditionList);
	double dLeadSpanCond2 = GetCondition(3, pConditionList);
	double dSpanCond = GetCondition(4, pConditionList);
	//sy end

	// �ϸ����ǥ ����, ���� ���� ���� - ojtaso (20071226)
	BOOL bEachSpan = !( dLeadSpanCond2 == 0 && dSpanCond == 0);
	if( !bEachSpan)
	{
		dSpanCond = dLeadSpanCond1;

		dLeadSpanCond1 = dStandardCond;
		dLeadSpanCond2 = dSpanCond;
		dSpanCond = dStandardCond;

		POSITION pos = pConditionList->GetHeadPosition();	// array Index[0]
		pConditionList->GetNext(pos);						// array Index[1]
		pConditionList->GetNext(pos);						// array Index[2]
		pConditionList->SetAt(pos, dLeadSpanCond1);			// dLeadSpanCond1 array Index[2]
		pConditionList->AddTail(dLeadSpanCond2);			// array Index[3]
		pConditionList->AddTail(dSpanCond);					// array Index[4]
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pTuringPoint || !pStandardLine || !pLagSpan || !m_pLeadSpan1 || !m_pLeadSpan2) return;

	CString strType;
	if(m_pHighPacket == NULL)
		strType = TYPE_INTERGER;
	else
		strType = m_pHighPacket->GetType(false);
	
	// (2009/9/19 - Seung-Won, Bae) for FX/WorldOn Index Digit.
	CString strValueType( TYPE_TWODECIMAL);
	if( CChartInfo::HTS != chartFormulateData.GetChartMode())
		strValueType = strType;

	SetPacketTypes( pTuringPoint,	strValueType, strType, (int)dTuringCond - 1);
	SetPacketTypes( pStandardLine,	strValueType, strType, (int)dStandardCond - 1);
	// �ϸ����ǥ ����, ���� ���� ���� - ojtaso (20071226)
	SetPacketTypes( pLagSpan,		strValueType, strType, int( dStandardCond - dSpanCond));
	SetPacketTypes( m_pLeadSpan1,	strValueType, strType, (int)(dStandardCond + dLeadSpanCond1) - 1);
	SetPacketTypes( m_pLeadSpan2,	strValueType, strType, (int)(dLeadSpanCond1 + dLeadSpanCond2) - 1);

	CPoint point = calculateData.GetPoint();
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pTuringPoint))
		GetTurningPointLine(point, dTuringCond, m_pHighPriceList, m_pLowPriceList, *pTuringPoint->GetnumericDataList());
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pStandardLine))
		GetStandardLine(point, dStandardCond, m_pHighPriceList, m_pLowPriceList, *pStandardLine->GetnumericDataList());
	// �ϸ����ǥ ����, ���� ���� ���� - ojtaso (20071226)
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pLagSpan))
		GetLagSpan(point, dSpanCond, m_pEndPriceList, *pLagSpan->GetnumericDataList());
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), m_pLeadSpan1))
	{
		GetLeadSpan1(point, m_pHighPriceList->GetCount(), dLeadSpanCond1,
						pTuringPoint->GetnumericDataList(), pStandardLine->GetnumericDataList(), *m_pLeadSpan1->GetnumericDataList());
		GetLeadSpan2(point, dLeadSpanCond1, dLeadSpanCond2, m_pHighPriceList, m_pLowPriceList, *m_pLeadSpan2->GetnumericDataList());
	}

	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), pLagSpan))
		GetLagSpan(point, dStandardCond, m_pEndPriceList, *pLagSpan->GetnumericDataList());	
	if(!IsIndicatorCalculated(chartFormulateData.GetCalculateType(), m_pLeadSpan1)){
		GetLeadSpan1(point, m_pHighPriceList->GetCount(), dStandardCond, 
				pTuringPoint->GetnumericDataList(), pStandardLine->GetnumericDataList(), 
				*m_pLeadSpan1->GetnumericDataList());
		GetLeadSpan2(point, dStandardCond, dSpanCond, m_pHighPriceList, m_pLowPriceList, *m_pLeadSpan2->GetnumericDataList());
	}
}

bool CGlanceBalance::SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
										CPacketList* pPacketList,
										CGraphCalculateData* pGraphCalculateData,
										CIndicatorInfo*& pIndicatorInfo,
										CPacket*& pTuringPoint, CPacket*& pStandardLine,
										CPacket*& pLagSpan)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	pTuringPoint = pStandardLine = pLagSpan = NULL;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL) 
			return InCaseNoPacketPointer(calculatetype, pPacketList, pIndicatorInfo, pGraphCalculateData, pTuringPoint, pStandardLine, pLagSpan);
		else
		{
/*			if(pTuringPoint == NULL)
				pTuringPoint = pPacket;
			else if(pTuringPoint != NULL && pStandardLine == NULL)	
				pStandardLine = pPacket;
			else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan == NULL)	
				pLagSpan = pPacket;
			else break;
*/
			// �ϸ����ǥ ����, ���� ���� ���� - ojtaso (20071226)
			if(pStandardLine == NULL)
				pStandardLine = pPacket;
			else if(pStandardLine != NULL && pTuringPoint == NULL)
				pTuringPoint = pPacket;
			else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan == NULL)
				pLagSpan = pPacket;
			else break;
		}
	}
	return (pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL && m_pLeadSpan1 != NULL && m_pLeadSpan2 != NULL);
}

bool CGlanceBalance::InCaseNoPacketPointer(const CCalculateBaseData::CALCULATETYPE calculatetype,
										   CPacketList* pPacketList,
										   CIndicatorInfo*& pIndicatorInfo,
										   CGraphCalculateData* pGraphCalculateData,
										   CPacket*& pTuringPoint, CPacket*& pStandardLine,
										   CPacket*& pLagSpan)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;

	CList<CString, CString> subGraphNameList;
	if(!pIndicatorInfo->GetSubGraphNameList(subGraphNameList))
		return false;

	POSITION pos_subGraphName = subGraphNameList.GetHeadPosition();

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	CString strSubGraphPacketNameHeader;
	if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

	POSITION pos_graphCalData = pGraphCalculateData->GetHeadPosition();
	while(pos_subGraphName && pos_graphCalData)
	{
		CSubGraphPacketList* pSubGraphPacketList = pGraphCalculateData->GetNext(pos_graphCalData);
		if(pSubGraphPacketList == NULL)
			return false;

		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CPacket* pPacket = pPacketList->GetBaseData( strSubGraphPacketNameHeader + subGraphNameList.GetNext(pos_subGraphName), false);
/*		if(pTuringPoint == NULL)
			pTuringPoint = pPacket;
		else if(pTuringPoint != NULL && pStandardLine == NULL)	
			pStandardLine = pPacket;
		else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan == NULL)	
			pLagSpan = pPacket;
		else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL)//���ེ��
			return ( GetSubGraphPacket( calculatetype, pPacketList, pSubGraphPacketList, pIndicatorInfo) &&
					pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL);
*/
		// �ϸ����ǥ ����, ���� ���� ���� - ojtaso (20071226)
		if( pStandardLine == NULL)
			pStandardLine = pPacket;
		else if(pStandardLine != NULL && pTuringPoint == NULL)
			pTuringPoint = pPacket;
		else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan == NULL)
			pLagSpan = pPacket;
		else if(pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL)//���ེ��
			return ( GetSubGraphPacket( calculatetype, pPacketList, pSubGraphPacketList, pIndicatorInfo) &&
					pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL);

		// (2007/6/18 - Seung-Won, Bae) Clear Packet for Recalculation.
		ClearPacketData( pPacket);

		pSubGraphPacketList->AddTail2( pPacket);
	}
	return (pTuringPoint != NULL && pStandardLine != NULL && pLagSpan != NULL && m_pLeadSpan1 != NULL && m_pLeadSpan2 != NULL);

}

bool CGlanceBalance::GetSubGraphPacket( const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList, CSubGraphPacketList* pSubGraphList,
									   CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	bool bClearResult = false;
	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		m_pLeadSpan1 = pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C3_PRE_SPAN_1), false);
		m_pLeadSpan2 = pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C3_PRE_SPAN_2), false);

		if(ClearPacketData(m_pLeadSpan1))
			bClearResult = true;
		if(ClearPacketData(m_pLeadSpan2))
			bClearResult = true;
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( m_pLeadSpan1);
	pSubGraphList->AddTail2( m_pLeadSpan2);

	return (bClearResult || (m_pLeadSpan1 != NULL && m_pLeadSpan2 != NULL));
}

void CGlanceBalance::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
										CGraphCalculateData* pGraphCalculateData,
										CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pLeadSpan1);
	ClearPacketData(m_pLeadSpan2);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

void CGlanceBalance::GetTurningPointLine(const CPoint& point, double dCondition,
										 CList<double, double>* pTopPriceList,
										 CList<double, double>* pBottomPriceList,
										 CList<double, double>& dTurningPointLineList)
{ //��ȯ�� ���
	GetMA(point, dCondition, pTopPriceList, pBottomPriceList, dTurningPointLineList);
}

void CGlanceBalance::GetStandardLine(const CPoint& point, double dCondition,
									 CList<double, double>* pTopPriceList,
									 CList<double, double>* pBottomPriceList,
									 CList<double, double>& dStandardLineList)
{	//	���ؼ� ���
	GetMA(point, dCondition, pTopPriceList, pBottomPriceList, dStandardLineList);
}

void CGlanceBalance::GetMA(const CPoint& point, double dCondition,
						   CList<double, double>* pTopPriceList,
						   CList<double, double>* pBottomPriceList,
						   CList<double, double>& dResultList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pTopPriceList || !m_pEndPriceList) return;

	// dCondition�� �ְ��� �ְ��� �������� �߰����ġ�� �̵���ռ�ó�� �׸���.
	// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
	//		if Error, Set All with Close for Base or Market Close Price.
	for(int count = point.x ; count < pTopPriceList->GetCount() ; count++)
	{
		if(count >= (dCondition - 1))
		{
			int start = (int)floor(count - dCondition +1);

			POSITION posTop = FindPosition(start, *pTopPriceList);
			POSITION posBottom = FindPosition(start, *pBottomPriceList);
			POSITION posClose = FindPosition(start, *m_pEndPriceList);
			if(posTop == NULL || posBottom == NULL || posClose == NULL) return;

			double dTop = GetAt(pTopPriceList, posTop);
			double dBottom = GetAt(pBottomPriceList, posBottom);
			double dClose = GetAt( m_pEndPriceList, posClose);

			// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( 0 == dTop || 0 == dBottom) dTop = dBottom = dClose;

			for(int j = start ; j <= count ; j++) 
			{
				double dTopPrice = GetNext(pTopPriceList, posTop);
				double dBottomPrice = GetNext(pBottomPriceList, posBottom);
				double dClosePrice = GetNext( m_pEndPriceList, posClose);

				// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
				//		if Error, Set All with Close for Base or Market Close Price.
				if( 0 == dTopPrice || 0 == dBottomPrice) dTopPrice = dBottomPrice = dClosePrice;

				dTop = dTop > dTopPrice ? dTop : dTopPrice;
				dBottom = dBottom < dBottomPrice ? dBottom : dBottomPrice;
			}
			SetData(point, count, (dTop + dBottom) / 2.0, &dResultList);
		}
		else
			SetData(point, count, 0.00, &dResultList);
	}
}

//end�� 5���� �����Ͱ� ���ེ���� ù���� �����Ͱ� �ȴ�.
void CGlanceBalance::GetLagSpan(const CPoint& point, double dCondition, CList<double, double>* pEndPriceList, CList<double, double>& dLagSpanList)
{	//	���ེ�� ���
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPriceList) return;

	if(pEndPriceList->GetCount() < dCondition)
		return;

	int startpoint = point.x;
	//����� �� �������� �����ؾ� �Ѵ�. ����. 100��° data�� real�� ����Ǿ��ٸ�..
	//���ེ���� ���� 74���� Data�� �������̰� �� Data�� ����Ǿ�� �Ѵ�.
	if(startpoint != 0) //append, realappend
		startpoint -= ((int)dCondition - 1);

	int nDataCount = pEndPriceList->GetCount();
	POSITION pos = FindPosition((int)(dCondition) + startpoint - 1, *pEndPriceList);
	for(int count = (int)(dCondition) + startpoint - 1; count < nDataCount; count++)
	{
		if(pos == NULL)
			return ;
		double dData = GetNext(pEndPriceList, pos);
		//����������count - (dCondition - 1) == lagspan������count �� update mode
		//lagspan�� ���������ͺ��� dCondition-1���� �����ϱ�...
		SetData(point, count - int(dCondition - 1), dData, &dLagSpanList);
	}
}

/////
//(���ؼ� + ��ȯ��) /2 �� 26�� �����Ų��.
//	���ེ��1 ���
///////
//�� ������ ����Ÿ �����ؼ� 26��
//���� 26���� ����Ÿ�� ������ 26���̶��..51���� ����Ÿ�� ����Ʈ�� ����Ǿ�� �Ѵ�.
//ù��° ����Ÿ�� 26��°�� ���� �Ѵ�. 
//���ݱ����� ��� index�� �ƴ� ���� count�̴�.
void CGlanceBalance::GetLeadSpan1(const CPoint& point,
								  const int nDataCount,
								  double dCondition,
								  CList<double, double>* pTurningPointLine,
								  CList<double, double>* pStandardLine,
								  CList<double, double>& dLeadSpanList)
{
	if(nDataCount < dCondition)
			return;

	POSITION posTurning = FindPosition(point.x, *pTurningPointLine);
	POSITION posStandard = FindPosition(point.x, *pStandardLine);

	//conditiondl 26�̸� �� ����Ʈ 25���� �����Ͱ� 0�̴�
	if(point.x == 0) //new, insert
	{ 
		for(int count = 0 ; count < dCondition - 1 ; count++)	
			dLeadSpanList.AddTail(0.00);
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pTurningPointLine) return;

	for(int count = point.x ; count < pTurningPointLine->GetCount() ; count++)
	{
		if(posTurning == NULL || posStandard == NULL)
			return ;
		double dData = (GetNext(pTurningPointLine, posTurning) + GetNext(pStandardLine, posStandard)) / 2.0;
		//�⺻�����ͺ��� count + dCondition - 1��ŭ �����Ͱ� �� ����.
		SetData(point, count + int(dCondition - 1), dData, &dLeadSpanList);
	}
}
/////
//(���� 52�ϰ��� �� + ����) /2 �� 26�� �����Ų��.
///////
//�� ������ ����Ÿ �����ؼ� 26��
//���� 26���� ����Ÿ�� ������ 26���̶��..51���� ����Ÿ�� ����Ʈ�� ����Ǿ�� �Ѵ�.
//ù��° ����Ÿ�� 26��°�� ���� �Ѵ�. 
//���ݱ����� ��� index�� �ƴ� ���� count�̴�.
// �ϸ����ǥ ����, ���� ���� ���� - ojtaso (20071226)
void CGlanceBalance::GetLeadSpan2(const CPoint& point, const double& dSpanCond1, const double& dSpanCond2, 
								  CList<double, double>* pTopPriceList,
								  CList<double, double>* pBottomPriceList,
								  CList<double, double>& dLeadSpanList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pTopPriceList || !pBottomPriceList || !m_pEndPriceList) return;

	if(pTopPriceList->GetCount() < dSpanCond2)
			return;

	if(point.x == 0) // insert, new�� ���. dSpanCond1�������� 0���� ä��.
	{ 
		for(int i = 0 ; i < dSpanCond1 - 1 ; i++)
			dLeadSpanList.AddTail(0.00);
	}

	// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
	//		if Error, Set All with Close for Base or Market Close Price.
	for(int i = point.x ; i < pTopPriceList->GetCount() ; i++)
	{
		if(i >= (dSpanCond2 - 1)) 
		{
			double high = GetAt(pTopPriceList, i);
			double low = GetAt(pBottomPriceList, i);
			double close = GetAt( m_pEndPriceList, i);

			// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( 0 == high || 0 == low) high = low = close;

			int start = (int)floor(i - dSpanCond2 +1);

			POSITION posTop = FindPosition(start, *pTopPriceList);
			POSITION posBottom = FindPosition(start, *pBottomPriceList);
			POSITION posClose = FindPosition(start, *m_pEndPriceList);
			for(int j = start ; j <= i ; j++)
			{
				if(posTop == NULL || posBottom == NULL || posClose == NULL) return;
				double dTop = GetNext(pTopPriceList, posTop);
				double dBottom = GetNext(pBottomPriceList, posBottom);
				double dClose = GetNext( m_pEndPriceList, posClose);

				// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
				//		if Error, Set All with Close for Base or Market Close Price.
				if( 0 == dTop || 0 == dBottom) dTop = dBottom = dClose;

				high = high > dTop ? high : dTop;
				low = low < dBottom ? low : dBottom;
			}
			SetData(point, i + int(dSpanCond1 - 1), (high + low) / 2.0, &dLeadSpanList);
		}
		else
			SetData(point, i + int(dSpanCond1 - 1), 0.00, &dLeadSpanList);
	}
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.02.03
//
//	�м�	:	��� �Ź� Chart
//					1. ������ �ŷ��� Packet�� Input�����Ͽ�, ���Ź� ���ݴ� Packet��
//						���Ź� �ŷ������� Packet�� �����Ѵ�.
//					2. ���Ź� ���ݴ� Packet�� �ŷ������� Packet�� ������ ������ ���ݴ� ������ ��ϵǾ� �ִ�.
//					3. ���Ź� ���ݴ� Packet�� ��ϵǴ� ���� �� ���ݴ� ������ ���Ѱ��̴�.
//					4. ���Ź� �ŷ������� Packet�� ���� ����ġ���� 2�� �� �ʱ�ȭ�Ѵ�.
//						�װ��� ���� Drawing �������� ������ Data�� ���� ������ ���ݴ� Index��
//						���� �������� ���ݴ뺰 �ŷ��������� �ִ��� ���ݴ��� Index�� �����Ѵ�.
//						Drawing�ÿ� ���� ���ݴ�� �ִ� ���ݴ��� High Light�� �����ȴ�.
//
//	����	:	(2004.02.05, ��¿�) Real ó���ÿ��� ���ŵ� Real�� ���� ���Ź� ��ǥ�� �����۾�����
//					�����Ǿ� �־�����, ������ Real�� ���͵� �װ��� �������� ��쿡�� �����ؾ� �ϹǷ� Logic�� ���� �ʴ´�.
//					�Ƹ� ������ �׻� ��ü ���⸦ �������� �Ǿ� �־��� �Ͱ���.
//					���� Real ó���� TRó�� �׻� ���� ������ �߽����� �����ϴ� ������ �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
CHorizontalBar::CHorizontalBar() : CIndicator( IMetaTable::EDDC_OCX_VOLUMEFORSALE)
{
	// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ�� ���⿡ �ʿ��� Packet�� �����Ѵ�.
	m_pHorZonPacket			= NULL;
	m_pHorZonPricePacket	= NULL;
	m_pEndPriceList			= NULL;
	m_pVolumeList			= NULL;

	// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ�� ���� ������ �⺻���� �����Ѵ�.
	m_StartPoint			= 0;
	m_EndPoint				= 0;
	m_Max					= 0.0;
	m_Min					= 0.0;
}

// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ���� �����Ѵ�. (������ ���� �⺻ ���� ����)
void CHorizontalBar::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 1. Packet List�� ���Ͽ� �����Ѵ�.
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	
	// 2. ���� TR�� ó���ø� ó���Ѵ�. (�ʱ�ȭ�Ѵ�.)
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		// 2.1 Input Packet�� ���Ѵ�.
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		CPacket *pPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));// "����"
		if( !pPacket) return;
		m_pEndPriceList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData( _MTEXT( C0_VOLUME));
		// (2006/1/12 - Seung-Won, Bae) Check NULL Pointer
		if( !pPacket) return;
		m_pVolumeList = pPacket->GetnumericDataList();
		// 2.2 Output Packet Pointer�� Clear�Ѵ�.
		//		(Object�� ������ ��� �ϴ���?)
		m_pHorZonPacket = NULL;
		m_pHorZonPricePacket = NULL;
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
		CString csHoga;
		(pPacketList->GetHelpMessageMap())->Lookup("HBPRICE", csHoga);
		m_dHoga = atof(csHoga);
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	}

	// 3. Input Packet�� ��ȿ���� Ȯ���Ѵ�.
	if( !IsValidPacket( m_pEndPriceList) || !IsValidPacket( m_pVolumeList)) return;

	// 4. ���Ź� Output Packet�� Ȯ���Ѵ�.
	// 4.1 �̹� Ȯ���Ǿ� �ִ� ���� �����Ѵ�. (TR�� ��쿡�� NULL�� �ʱ�ȭ�Ǿ� �ִ�.)
	if( !m_pHorZonPacket || !m_pHorZonPricePacket)
	{
		// 4.2 CGraphCalculateData���� Output Packet�� Ȯ���Ѵ�.
		// 4.2.1 ��ǥ Output Packet ������ �ִ� ��츸 ó���Ѵ�.
		//		SubGraphList�� ������ 2�� (���Ź��� �׻� ������Ʈ�� �����Ѵ�.)
		//		ù��° ����Ʈ���� ���Ź� �ŷ�������, ���Ź����ݴ� ��Ŷ �ּ�
		//		�ι�° ����Ʈ���� ��,��,��,�� ��Ŷ �ּ�
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pGraphCalculateData) return;
		if( pGraphCalculateData->GetCount() > 0)
		{
			// 4.2.2 CGraphCalculateData���� Output Packet�� Sub Graph List�� Ȯ���Ѵ�.
			CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
			if( !pSubGraphList) return;

			// 4.2.3 Output Packet�� Ȯ���Ѵ�.
			m_pHorZonPacket			= pSubGraphList->GetPacket( 0);
			m_pHorZonPricePacket	= pSubGraphList->GetPacket( 1);
		}

		// 4.3 CGraphCalculateData���� Output Packet�� Ȯ���� ������ ��츦 ó���Ѵ�.
		//		Packet List���� ���� ���Ѵ�.
		if( !m_pHorZonPacket || !m_pHorZonPricePacket)
		{
			// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
			CString strSubGraphPacketNameHeader;
			if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

			// 4.3.1 Packet List���� ���� �̸��� ���Ź� Output Packet�� ���Ѵ�.
			m_pHorZonPacket			= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C3_OVERHANGING_SUPPLY),							FALSE);
			m_pHorZonPricePacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C3_OVERHANGING_SUPPLY) + _MTEXT( C6_PRICE_BAND),	FALSE);

			// 4.3.2 Packet�� Ȯ���� ������ ��츦 ó���Ѵ�.
			if( !m_pHorZonPacket || !m_pHorZonPricePacket) return;

			// 4.3.3 ������ Packet�� Data�� �ʱ�ȭ�Ѵ�.
			// (2004.02.05, ��¿�) Real ó���ÿ��� ���ŵ� Real�� ���� ���Ź� ��ǥ�� �����۾�����
			//		�����Ǿ� �־�����, ������ Real�� ���͵� �װ��� �������� ��쿡�� �����ؾ� �ϹǷ� Logic�� ���� �ʴ´�.
			//		�Ƹ� ������ �׻� ��ü ���⸦ �������� �Ǿ� �־��� �Ͱ���.
			//		���� Real ó���� TRó�� �׻� ���� ������ �߽����� �����ϴ� ������ �����Ѵ�.
			//		���� if �����ۿ��� �׻� Packet�� Clear��Ű�� �����Ѵ�.

			// 4.3.4 ������ Packet�� ���Ź� I/O Packet ������ ��Ͻ�Ų��.
			CSubGraphPacketList *pSubGraphList = NULL;
			POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
			if( posSubGraph)
			{
				pSubGraphList = pGraphCalculateData->GetNext( posSubGraph);
				if( pSubGraphList)
				{
					pSubGraphList->AddTail2( m_pHorZonPacket);
					pSubGraphList->AddTail2( m_pHorZonPricePacket);
				}
			}
		}
	}

	// (2004.06.25, ��¿�) ������ �ϳ��� ���� ��쿡���� ���Ź��� ������� �ʴ´�.
	if( m_pEndPriceList->GetCount() <= 0) return;

	// 6. ���Ź� ���� ������ ������ ���Ͽ� �����Ѵ�.
	//		���� GetHorizontalBar()���� �̿��Ѵ�.
	m_StartPoint = chartFormulateData.GetDisplayDataStartIndex();
	m_EndPoint = chartFormulateData.GetDisplayDataEndIndex();
	
	// (2006/8/22 - Seung-Won, Bae) Real Calculate is in the case of Last Data in View.
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL
		&& m_EndPoint < m_pEndPriceList->GetCount() - 1)
			return;

	// 5. Scroll�� ���� ���Ź� ������ ���Ͽ� Ouput Packet�� Clear��Ų��.
	// (2004.02.05, ��¿�) Real ó���ÿ��� ���ŵ� Real�� ���� ���Ź� ��ǥ�� �����۾�����
	//		�����Ǿ� �־�����, ������ Real�� ���͵� �װ��� �������� ��쿡�� �����ؾ� �ϹǷ� Logic�� ���� �ʴ´�.
	//		�Ƹ� ������ �׻� ��ü ���⸦ �������� �Ǿ� �־��� �Ͱ���.
	//		���� Real ó���� TRó�� �׻� ���� ������ �߽����� �����ϴ� ������ �����Ѵ�.
	//		���� �׻� Packet�� Clear��Ű�� �����Ѵ�.
	ClearPacketData( m_pHorZonPacket);
	ClearPacketData( m_pHorZonPricePacket);

	// (2004.06.25, ��¿�) ���� Index�� �������� ���� �ƴϸ� ���Ź��� ������� �ʴ´�.
	if( m_EndPoint < 0 || m_StartPoint < 0) return;
	CPacket *pEndPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));//"����"
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPacket) return;
	pEndPacket->GetMinMax( m_StartPoint, m_EndPoint, m_Min, m_Max);
	// (2004.06.25, ��¿�) ���� ������ ���� 0�̸�, ��������ġ ���Ϸ� (����/�ɼ� ����, 0.001) ���� ������.
	if( m_Min == m_Max)
	{
		m_Min -= 0.001;
		m_Max += 0.001;
	}
	
	// 7. ���Ź� ��ǥ�� �����Ѵ�.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pIndicatorInfo) return;
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
//	GetHorizontalBar( chartFormulateData, calculateData.GetPoint(),
//		GetCondition( 0, pIndicatorInfo->GetCommonCalConditionList()), m_pEndPriceList, m_pVolumeList);
	GetHorizontalBar( chartFormulateData, calculateData.GetPoint(),
		GetCondition( 0, pIndicatorInfo->GetCommonCalConditionList()), m_pEndPriceList, m_pVolumeList,
		GetCondition( 1, pIndicatorInfo->GetCommonCalConditionList()), m_dHoga);
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�

	// 8. ���Ź� Output Packet�� ����Ѵ�.
	SetPacketTypes(m_pHorZonPacket,		pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
	SetPacketTypes(m_pHorZonPricePacket,	pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
}

// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ�� �����Ѵ�. (���ݴ� ���� ���� �� �ŷ��� ���� ó��)
void CHorizontalBar::GetHorizontalBar(const CChartFormulateData& chartFormulateData, 
				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
				CList<double, double>* pVolumeList,double dConditionType, double dHoga)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPriceList || !pVolumeList) return;

	// 1. ������ ü�ᷮ Packet�� Data�� Ȯ���Ѵ�.
	if( pEndPriceList->GetCount() < 1 || pVolumeList->GetCount() < 1) return;

	// 2. ���� ������ �� Index�� Ȯ���Ͽ�, �� Data�� ����� ��츦 �����Ѵ�.
	//		(�̰��� Chart�� ���� ������ �ִ� ���� �Ǵܵȴ�.)
	if( m_EndPoint > pEndPriceList->GetCount() - 1) m_EndPoint = pEndPriceList->GetCount() - 1;

	// 3. Real ���Ž��� ��츦 �켱 ó���Ͽ� �����Ѵ�.
	// (2004.02.05, ��¿�) Real ó���ÿ��� ���ŵ� Real�� ���� ���Ź� ��ǥ�� �����۾�����
	//		�����Ǿ� �־�����, ������ Real�� ���͵� �װ��� �������� ��쿡�� �����ؾ� �ϹǷ� Logic�� ���� �ʴ´�.
	//		�Ƹ� ������ �׻� ��ü ���⸦ �������� �Ǿ� �־��� �Ͱ���.
	//		���� Real ó���� TRó�� �׻� ���� ������ �߽����� �����ϴ� ������ �����Ѵ�.

	// 4. TR�̳� Scroll�� ���� ������ ��츦 ó���Ѵ�.
	// 4.1 ������ ������ ���Ź� Data���� �ִ� ��� Clear ��Ų��.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pHorZonPacket || !m_pHorZonPricePacket) return;
	if( m_pHorZonPacket->GetCount() > 0)
	{
		m_pHorZonPacket->ClearData();
		m_pHorZonPricePacket->ClearData();
	}

	// 4.2 ���� ���Ź� �ŷ������� Packet�� �������� Clear��Ų��.
	//		(���� ����ġ���� 2�� �� �ʱ�ȭ�Ѵ�. �װ��� ���� Drawing �������� ������ Data�� 
	//		 �װ��� ���� Drawing �������� ������ Data�� ���� ������ ���ݴ� Index��
	//		 ���� �������� ���ݴ뺰 �ŷ��������� �ִ��� ���ݴ��� Index�� �����Ѵ�.
	//		 Drawing�ÿ� ���� ���ݴ�� �ִ� ���ݴ��� High Light�� �����ȴ�.)
	CList<double, double>* pHorizontalBarList = m_pHorZonPacket->GetnumericDataList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pHorizontalBarList) return;

	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	double nInterval;
	if( dCondition < 1) dCondition = 1;
	if(dConditionType && 0 < dHoga)
	{
		nInterval = dHoga * dCondition;
		dCondition = int( ( ( m_Max - m_Min) / nInterval) * ( 1 + DBL_EPSILON)) + 1;
	}
	else
	{
		dHoga = 0;
		nInterval = ( m_Max - m_Min) / dCondition;
	}
	int i = 0;
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	for( i = 0; i < dCondition + 2; i++) pHorizontalBarList->AddTail( 0.0);

	// 4.3 ������ dCondition ������ �����Ͽ� ���Ź� ���ݴ� Packet ���� �����Ѵ�.
	CList<double, double>* pPriceZonList = m_pHorZonPricePacket->GetnumericDataList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPriceZonList) return;
// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�	double nInterval = ( m_Max - m_Min) / dCondition;
// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�	for( i = 0; i < dCondition; i++) pPriceZonList->AddHead( m_Min + nInterval * i);
	double	dHogaPrice = 0.0;
	for( i = 0; i <= dCondition; i++)
	{
		if( i == 0) dHogaPrice = m_Min;
		else if( i == dCondition && dHoga <= 0) dHogaPrice = m_Max;
		else dHogaPrice = m_Min + nInterval  * i;

		pPriceZonList->AddHead( dHogaPrice);
	}
// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	// 4.5 ���Ź� ���ⱸ���� �ŷ����� �����Ѵ�.
	//	(�̶� ���� ������ ���ݴ� Index�� ���� ��� �����Ѵ�.)
	// 4.5.0 ���Ź� ���� ������ �� ������ �ŷ����� Position�� ���Ͽ� �����Ѵ�.
	//		���� ���� �������� Ȯ���ϴµ� �̿��Ѵ�.
	POSITION posEndPrice	= FindPosition( m_EndPoint, *pEndPriceList);
	POSITION posEndVolume	= FindPosition( m_EndPoint, *pVolumeList);
	if( posEndPrice == NULL || posEndVolume == NULL) return;
	// 4.5.1 ���Ź� ���ⱸ���� ù ������ �ŷ����� Position�� ���Ѵ�.
	POSITION posOldPrice	= NULL, posPrice	= FindPosition( m_StartPoint, *pEndPriceList);
	POSITION posOldVolume	= NULL, posVolume	= FindPosition( m_StartPoint, *pVolumeList);
	if( posPrice == NULL || posVolume == NULL) return;
	// 4.5.2 ���� ������ �� ������ ��������, �ش� ���Ź� ���ݴ��� �ŷ����� ����ó���Ѵ�.
	//		(���� �������� ó���ϵ��� �Ѵ�.)
	int nIndex;
	while( posPrice && posVolume && posEndPrice	!= posOldPrice && posEndVolume != posOldVolume)
	{
		// 4.5.2.0 ���� Position�� Backup�Ѵ�.
		posOldPrice		= posPrice;
		posOldVolume	= posOldVolume;

		// 4.5.2.1 ���ݴ� Index�� �����Ѵ�.
		// (2008/4/23 - Seung-Won, Bae) Repaire of the loss in double calculation.
		double dValue = ( pEndPriceList->GetNext( posPrice) - m_Min) / nInterval;
		dValue *= ( 1 + DBL_EPSILON * 1000);
		nIndex = int( dCondition - int( dValue)) - 1;

		// 4.5.2.2 Zero Base�� ��ȯ�Ѵ�.
		//		�̶� ������ ���ⱸ������ �ְ��� ��쿡��, nIndex�� -1�� �����Ǵµ�,
		//		�̴� ù ���ݴ� ������ �ְ� ������ ��ǥ���� ���� ���Ѱ��� ������ �Ӹ� �ƴ϶�
		//			���� ��ü������ �ְ��� �����Ѵ�.
		if( nIndex < 0) nIndex = 0;

		// 4.5.2.3 ������ Index�� ���ݴ뿡 �ŷ����� �����Ѵ�.
		// (2007/3/15 - Seung-Won, Bae) Check NULL Position
		POSITION posVolumeBar = FindPosition( nIndex, *pHorizontalBarList);
		if( posVolumeBar) pHorizontalBarList->SetAt( posVolumeBar, GetAt( pHorizontalBarList, posVolumeBar)
			+ pVolumeList->GetNext(posVolume));

		// 4.5.2.4 ���������� ����� ���� ������ ���ݴ� Index�� ����Ͽ� �����Ѵ�.
		posVolumeBar = FindPosition( ( int)dCondition, *pHorizontalBarList);
		if( posVolumeBar) pHorizontalBarList->SetAt( posVolumeBar, nIndex);
	}

	// 4.6 ���Ź� ���ⱸ���� ���ݴ뺰 �ŷ��� ������ �ְ�ġ�� ���ݴ��� Index�� ���� �����Ѵ�.
	POSITION pos = pHorizontalBarList->GetHeadPosition();
	if(pos == NULL) return;
	double dVolume, dMaxVolume = 0;
	for(i = 0; i < dCondition; i++)
	{
		dVolume = pHorizontalBarList->GetNext(pos);
		if( dVolume >= dMaxVolume)
		{
			dMaxVolume = dVolume;
			pHorizontalBarList->SetAt( FindPosition( ( int)dCondition + 1, *pHorizontalBarList), i);
		}
	}
}

				
//******************************************************************************************************************
//���ð� � 
//:����(30�� �ְ� �̵����ġ)�� Ⱦ��(30�� �ŷ��� �̵����ġ)�� ������ ����
//******************************************************************************************************************

CInverseLine::CInverseLine() : 
	m_pPriceMA(NULL), 
	m_pVolumeMA(NULL),
	m_pEndPricePacket(NULL),
	m_pVolumePacket(NULL),
	m_pEndPriceList(NULL),
	m_pVolumeList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_INVERSLINECHART)
{
}

void CInverseLine::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pEndPricePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pEndPriceList = GetnumericDataList(m_pEndPricePacket);
		m_pVolumePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pVolumeList = GetnumericDataList(m_pVolumePacket);
	}
	if(!IsValidPacket(m_pEndPriceList) || !IsValidPacket(m_pVolumeList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pIndicatorInfo) return;

	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dCondition0 = GetCondition(0, pConditionList);

	CPoint point = calculateData.GetPoint();

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pEndPricePacket || !m_pVolumePacket) return;

	if(m_pEndPricePacket == NULL)
		SetPacketTypes(m_pPriceMA, TYPE_TWODECIMAL, TYPE_INTERGER, (int)dCondition0 - 1);
	else
	{
		// (2008/10/17 - Seung-Won, Bae) Use packet type of price.
		CString strPacketType = m_pEndPricePacket->GetType(false);
		strPacketType.Replace( "��", "");
		strPacketType.TrimLeft( "xX ");
		if( strPacketType.IsEmpty())				strPacketType = TYPE_TWODECIMAL;
		else if( strPacketType.GetAt( 0) == '1')	strPacketType = TYPE_TWODECIMAL;
		SetPacketTypes(m_pPriceMA, strPacketType, m_pEndPricePacket->GetType(false), (int)dCondition0 - 1);
	}

	SetPacketTypes(m_pVolumeMA, TYPE_TWODECIMAL, TYPE_INTERGER, (int)dCondition0 - 1);

	GetSimpleMA(point, dCondition0, m_pEndPriceList, m_pPriceMA);
	GetSimpleMA(point, dCondition0, m_pVolumeList, m_pVolumeMA);
}

//Ư����
bool CInverseLine::SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);

	}
	return (m_pPriceMA != NULL && m_pVolumeMA != NULL);
}

//Ư����
bool CInverseLine::InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;

	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;

	m_pPriceMA	= pPacketList->GetBaseData( _MTEXT( C0_INVERSETIMELINE_PRICE),	false);
	m_pVolumeMA	= pPacketList->GetBaseData( _MTEXT( C0_INVERSETIMELINE_VOLUME),	false);

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( m_pPriceMA);
	pSubGraphList->AddTail2( m_pVolumeMA);

	// (2007/4/30 - Seung-Won, Bae) Add Time Data
	CPacket *pDatePacket = pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( pDatePacket) pSubGraphList->AddTail2( pDatePacket);

	return (m_pPriceMA != NULL || m_pVolumeMA != NULL);
}

bool CInverseLine::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		m_pPriceMA = pPacketList->GetBaseData( _MTEXT( C0_INVERSETIMELINE_PRICE), false);
		m_pVolumeMA = pPacketList->GetBaseData( _MTEXT( C0_INVERSETIMELINE_VOLUME), false);

		ClearPacketData(m_pPriceMA);
		ClearPacketData(m_pVolumeMA);
	}
	return (m_pPriceMA != NULL  && m_pVolumeMA != NULL );
}

void CInverseLine::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pPriceMA);
	ClearPacketData(m_pVolumeMA);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}



//////////////////////////////////////////////////////////////////////////
// Swing Chart
// ����(05/06/14)
//
//////////////////////////////////////////////////////////////////////////
CSwingChart::CSwingChart() : CIndicator( IMetaTable::EDDC_OCX_SWINGCHART)
{
	m_pEndPricePacket = NULL;
	m_pDatePacket = NULL;
	m_pSwingPrice = NULL;
	m_pSwingDate = NULL;
}


void CSwingChart::GetData(const CChartFormulateData& chartFormulateData, 
						  CGraphCalculateData* pGraphCalculateData, 
						  CIndicatorInfo*& pIndicatorInfo, 
						  CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(!pPacketList || !pIndicatorInfo)
		return;
	
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();	// ������
	CList<double, double>* pEndPriceList = NULL;	// ����
	CList<double, double>* pDateList = NULL;		// �ڷ�����

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		
		//�ڷ����ڿ� ������Ŷ�������� (IndiCond.cfg�� ����)
		m_pEndPricePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		
	}
	if(!m_pEndPricePacket || !m_pDatePacket)
		return;

	pEndPriceList = this->GetnumericDataList(m_pEndPricePacket);
	pDateList = GetnumericDataList(m_pDatePacket);

	// ����Ÿ���� ��ȿ������ �˻��Ѵ�.
	if(!IsValidPacket(pEndPriceList) || !IsValidPacket(pDateList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	if(!pIndicatorInfo)
		return;

	// ĭ��ȯ�� ���ϱ�
	double dInverseUnit = GetCondition(0, pConditionList);

	CString strPriceType = m_pEndPricePacket->GetType(false);
	CString strDateType = m_pDatePacket->GetType(false);
	SetPacketTypes(m_pSwingPrice, strPriceType, strPriceType, 0);
	SetPacketTypes(m_pSwingDate, strDateType, strDateType, 0);


	// ��ĭ�� ���ϱ� = �ְ� ������ �������� 2%.
	double dMin = 0.00;
	double dMax = 0.00;
	m_pEndPricePacket->GetMinMax(0, m_pEndPricePacket->GetCount() - 1, dMin, dMax);
	double dUnitValue = (dMax - dMin) / 50.0;

	GetSwingChart(calculateData.GetPoint(), dUnitValue, dInverseUnit, pEndPriceList, pDateList, m_pSwingPrice, m_pSwingDate);	
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetSwingChart
 - Created at  :  2005-06-16   10:11
 - Author      :  ������
 - Parameters  :  point         - 
 -                dUnitValue    - ��ĭ��
 -                dInverseUnit  - ĭ��ȯ��
 -                pEndPriceList - ���� ����Ÿ
 -                pDateList     - �ڷ����� ����Ÿ
 -				  pSwingPrice   - [out] Swing Data
 -				  pSwingDate    - [out] Swing ��Ʈ�� �ڷ�����.
 - Description :  SwingChart�� ����Ÿ�� ����س���. (P&F ������ ����)
 -----------------------------------------------------------------------------------------*/
void CSwingChart::GetSwingChart(const CPoint& point, 
								double dUnitValue,
								double dInverseUnit,
								CList<double, double>* pEndPriceList,
								CList<double, double>* pDateList,
								CPacket* pSwingPrice,
								CPacket* pSwingDate)
{
	if(pEndPriceList == NULL || pDateList == NULL)
		return;
	
	double dBaseData = 0.00; // SwingChart�� ����Ҷ� �ʿ��� BaseData.
	double dBaseDate = 0.00;	// BaseData�� �ڷ�����.
	UINT nIndex = 0;		// SwingChart ��Ŷ�� Setting�� ���� �ε���.
	short nSignal = 0;	// ������ Swing Data �߼��� �����Ѵ�. -1: �϶�, 0: ����(ó�����۽�), 1: ���
	double dInverseUnitValue = dUnitValue * dInverseUnit;	// �ּҹ����� = ��ĭ�� * ĭ��ȯ��

	POSITION posEndPrice = pEndPriceList->GetHeadPosition();
	POSITION posDate = pDateList->GetHeadPosition();

	// BaseData�� �����Ѵ�.(ù��° ����)
	dBaseData = pEndPriceList->GetNext(posEndPrice);
	dBaseDate = pDateList->GetNext(posDate);
	SetData(point, nIndex, dBaseData, pSwingPrice);
	SetData(point, nIndex, dBaseDate, pSwingDate);
	nIndex++;

	while(posEndPrice)
	{
		double dEndPrice = pEndPriceList->GetNext(posEndPrice);
		double dDate = pDateList->GetNext(posDate);

		// 1. ó�� �߼� ������.
		if(0 == nSignal)	
		{
			// 1.0. Data�� �ռ� while�� �տ��� Insert�� �ߴ�.
			//     �߼��� �����ϸ� �ȴ�.
			
			// 1.1. ���������� ���ذ�+�ּҹ����� �̻��϶�.
			if(dBaseData+dInverseUnitValue <= dEndPrice)
			{
				// 1.1.1. ���ο� BaseData����
				dBaseData = dEndPrice;
				dBaseDate = dDate;

				// 1.1.2. ����߼�
				nSignal = 1;
			}
			// 1.2. ���������� ���ذ�+�ּҹ����� �����϶�.
			else if(dBaseData-dInverseUnitValue >= dEndPrice)
			{
				// 1.2.1. ���ο� BaseData����
				dBaseData = dEndPrice;
				dBaseDate = dDate;

				// 1.2.2. �϶��߼�
				nSignal = -1;
			}
		}
		// 2. ����߼��϶�...
		else if(1 == nSignal)
		{
			// 2.1. ���ذ�-�ּҹ����� ���Ϸ� ������ ��������
			if(dBaseData-dInverseUnitValue >= dEndPrice)
			{	
				// 2.1.1. �߼�����
				nSignal = -1;
				// 2.1.2. Insert
				SetData(point, nIndex, dBaseData, pSwingPrice);
				SetData(point, nIndex, dBaseDate, pSwingDate);
				nIndex++;
				// 2.1.3. ���ο� BaseData����
				dBaseData = dEndPrice;
				dBaseDate = dDate;
			}
			// 2.2. ���ذ�+��ĭ���̻� ����̸� ���ο� BaseData����.
			else if(dBaseData+dUnitValue < dEndPrice)
			{
				dBaseData = dEndPrice;
				dBaseDate = dDate;
			}
			// 2.3. ���ذ�+��ĭ���̸� ����̰ų� ���ذ�-�ּҹ����� �̸��� �϶�
			// 2.3.1 �ƹ��͵� ���� �ʴ´�. ^^;
		}
		// 3. �϶��߼��϶�...
		else if(-1 == nSignal)
		{
			// 3.1. ���ذ�+�ּҹ����� �̻����� ������ �ö󰡸�
			if(dBaseData+dInverseUnitValue <= dEndPrice)
			{
				// 3.1.1. �߼�����
				nSignal = 1;
				// 3.1.2. Insert
				SetData(point, nIndex, dBaseData, pSwingPrice);
				SetData(point, nIndex, dBaseDate, pSwingDate);
				nIndex++;
				// 3.1.3. ���ο� BaseData����
				dBaseData = dEndPrice;
				dBaseDate = dDate;
			}
			// 3.2. ���ذ�-��ĭ�����Ϸ�  �϶��̸� ���ο� BaseData����.
			else if(dBaseData-dUnitValue > dEndPrice)
			{
				dBaseData = dEndPrice;
				dBaseDate = dDate;
			}
			// 3.3. ���ذ�+��ĭ���̸� ����̰ų� ���ذ�-�ּҹ������̸��� �϶�
			// 3.3.1 �ƹ��͵� ���� �ʴ´�.
		}
	}

	// ���� BaseData�� Insert�Ѵ�.
	SetData(point, nIndex, dBaseData, pSwingPrice);
	SetData(point, nIndex, dBaseDate, pSwingDate);

	return;
}

bool CSwingChart::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculateType, CPacketList* pPacketList)
{
	if(!pPacketList)
		return false;

//	if(calculateType == CCalculateBaseData::TRDATA_CAL)
//	{
		m_pSwingPrice = pPacketList->GetBaseData( _MTEXT( C0_SWING_CHART_PRICE), false);
		m_pSwingDate = pPacketList->GetBaseData( _MTEXT( C0_SWING_CHART_DATE_TIME), false);

		ClearPacketData(m_pSwingPrice);
		ClearPacketData(m_pSwingDate);
//	}
	return (m_pSwingPrice != NULL && m_pSwingDate != NULL);
}

bool CSwingChart::SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo)
{
	if(!pGraphCalculateData)
		return false;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	return (m_pSwingPrice != NULL && m_pSwingDate != NULL);
}

bool CSwingChart::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo, CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;
	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;
	if(!pSubGraphList)
		return false;

	m_pSwingPrice	= pPacketList->GetBaseData( _MTEXT( C0_SWING_CHART_PRICE),	false);
	m_pSwingDate	= pPacketList->GetBaseData( _MTEXT( C0_SWING_CHART_DATE_TIME),		false);

	pSubGraphList->AddTail2( m_pSwingPrice);
	pSubGraphList->AddTail2( m_pSwingDate);
	return (m_pSwingPrice != NULL || m_pSwingDate != NULL);
}

void CSwingChart::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pSwingPrice);
	ClearPacketData(m_pSwingDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

//////////////////////////////////////////////////////////////////////////
// Kagi ��Ʈ
// ����(05/06/20)
//////////////////////////////////////////////////////////////////////////
CKagiChart::CKagiChart() : CIndicator( IMetaTable::EDDC_OCX_KAGICHART)
{
	m_pEndPricePacket = NULL;
	m_pDatePacket = NULL;
	m_pKagiPrice = NULL;
	m_pKagiDate = NULL;
}

void CKagiChart::GetData(const CChartFormulateData& chartFormulateData, 
					CGraphCalculateData* pGraphCalculateData, 
					CIndicatorInfo*& pIndicatorInfo, 
					CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(!pPacketList || !pIndicatorInfo)
		return;
	
	CList<double, double>* pEndPriceList = NULL;	// ����
	CList<double, double>* pDateList = NULL;		// �ڷ�����

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		
		//�ڷ����ڿ� ������Ŷ�������� (IndiCond.cfg�� ����)
		m_pEndPricePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
	}

	if(!m_pEndPricePacket || !m_pDatePacket)
		return;

	pEndPriceList = this->GetnumericDataList(m_pEndPricePacket);
	pDateList = GetnumericDataList(m_pDatePacket);

	// ����Ÿ���� ��ȿ������ �˻��Ѵ�.
	if(!IsValidPacket(pEndPriceList) || !IsValidPacket(pDateList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	if(!pIndicatorInfo)
		return;

	CString strPriceType = m_pEndPricePacket->GetType(false);
	CString strDateType = m_pDatePacket->GetType(false);
	SetPacketTypes(m_pKagiPrice, strPriceType, strPriceType, 0);
	SetPacketTypes(m_pKagiDate, strDateType, strDateType, 0);


	// ��ĭ�� ���ϱ� = �ְ� ������ �������� 3%.
	double dMin = 0.00;
	double dMax = 0.00;
	m_pEndPricePacket->GetMinMax(0, m_pEndPricePacket->GetCount() - 1, dMin, dMax);
	double dUnitValue = (dMax - dMin) * 0.03;


	// ���ǰ� ����� ���� ���� - ojtaso (20070913)
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dInverseUnit = GetCondition(0, pConditionList);

	// ���ǰ� ����� ���� ���� - ojtaso (20070913)
	GetKagiChart(calculateData.GetPoint(), dUnitValue, dInverseUnit, pEndPriceList, pDateList, m_pKagiPrice, /*m_pCenterLine,*/ m_pKagiDate);
}

// ���ǰ� ����� ���� ���� - ojtaso (20070913)
void CKagiChart::GetKagiChart(const CPoint& point, 
							double dUnitValue, 
							double dInverseUnit,
							CList<double, double>* pEndPriceList, 
							CList<double, double>* pDateList,
							CPacket* pKagiPrice,
							CPacket* pKagiDate)
{
	if(pEndPriceList == NULL || pDateList == NULL)
		return;

	// ���ǰ� ����� ���� ���� - ojtaso (20070913)
	m_swingChart.GetSwingChart(point, dUnitValue, dInverseUnit, pEndPriceList, pDateList, pKagiPrice, pKagiDate);
}


bool CKagiChart::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculateType, CPacketList* pPacketList)
{
	if(!pPacketList)
		return false;

//	if(calculateType == CCalculateBaseData::TRDATA_CAL)
//	{
		m_pKagiPrice = pPacketList->GetBaseData( _MTEXT( C0_KAGI_PRICE), false);
		m_pKagiDate = pPacketList->GetBaseData( _MTEXT( C0_KAGI_DATE_TIME), false);

		ClearPacketData(m_pKagiPrice);
		ClearPacketData(m_pKagiDate);
//	}
	return (m_pKagiPrice != NULL && m_pKagiDate != NULL);
}


bool CKagiChart::SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo)
{
	if(!pGraphCalculateData)
		return false;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	return (m_pKagiPrice != NULL && m_pKagiDate != NULL);
}


bool CKagiChart::InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo, CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;
	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;
	if(!pSubGraphList)
		return false;

	m_pKagiPrice	= pPacketList->GetBaseData( _MTEXT( C0_KAGI_PRICE),	false);
	m_pKagiDate		= pPacketList->GetBaseData( _MTEXT( C0_KAGI_DATE_TIME),	false);

	pSubGraphList->AddTail2( m_pKagiPrice);
	pSubGraphList->AddTail2( m_pKagiDate);
	return (m_pKagiPrice != NULL && m_pKagiDate != NULL);
}


void CKagiChart::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pKagiPrice);
	ClearPacketData(m_pKagiDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//Lock
///////////////////////////////////////////////////////////////////////////////////////////////////////
CLock::CLock( HWND p_hOcxWnd) : 
	m_pLockYesOrNotList(NULL),
	m_pLockRatioTextList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_LOCKCHART)
{
	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
}
void CLock::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		CPacket *pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pLockYesOrNotList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( pPacket) m_pLockRatioTextList = pPacket->GetnumericDataList();
	}
	if( !IsValidPacket(m_pLockYesOrNotList)) return;

	CPacket* pLock = NULL;
	if(!SetGraphPacketList(pPacketList, pGraphCalculateData, 
		chartFormulateData.GetPacketNames(), pIndicatorInfo, pLock))
		return;
	
	if(IsIndicatorCalculated( chartFormulateData.GetCalculateType(), pLock))
		return;

	//sy 2004.04.13.
	// "����"�� ��쿡�� �̸� type�� �־� ��� �Ѵ�.
	// -> �⺻���� "X 1" �� setting �ϹǷ� add���� �ʴ´�.
	SetPacketTypes(pLock, _MTEXT( C6_CHARACTER), _MTEXT( C6_CHARACTER), 0);
	GetLock(calculateData.GetPoint(), m_pLockYesOrNotList, m_pLockRatioTextList, pLock);
}

//sy 2004.04.13.
bool CLock::SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							const CString& strPacketNames, 
							CIndicatorInfo*& pIndicatorInfo,
							CPacket*& pLock)
{
	if(pPacketList == NULL || pGraphCalculateData == NULL)
		return false;

	// ù��° packet : Data Packet (����� Packet)
	if(!CIndicator::SetGraphPacketList(pPacketList, 
		pGraphCalculateData, pIndicatorInfo, pLock))
		return false;

	// �ι�° packet : �׸� ��ġ�� �˷��ִ� packet (TR�� ���� Packet)
	CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
	if(pSubGraphList == NULL)
		return false;

	if(pSubGraphList->GetCount() >= 2)
		return true;

	CString strPacketName = strPacketNames;
	CDataConversion::GetStringData(strPacketName, ",");
	strPacketName.TrimRight(",");
	if(strPacketName.IsEmpty())
		return false;

	CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return false;

	pSubGraphList->AddTail2( pPacket);
	return true;
}

void CLock::GetLock(const CPoint& point, CList<double, double>* pLockList, CList<double, double>* pLockRatioTextList, CPacket* pLock)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pLockList) return;

	if( pLockList->GetCount() <= 0) return;

	POSITION pos = FindPosition(point.x, *pLockList);
	POSITION RatioPos = NULL;
	if( pLockRatioTextList) RatioPos = FindPosition(point.x, *pLockRatioTextList);
	CString strRatio,strLock;
	for(int count = point.x ; count < pLockList->GetCount() ; count++) 
	{
		if(pos == NULL) return;
		strLock = GetData((int)pLockList->GetNext(pos));

		if( pLockRatioTextList &&  RatioPos != NULL) strRatio = GetRatioTextData( pLockRatioTextList->GetNext(RatioPos));
		SetData( point, count, strLock, pLock);
	}
}

CString CLock::GetRatioTextData(const double nRatioData) const
{
	CString strText;
//	if(nRatioData<=0.0)
//		return "";
//	else
	
		strText.Format("(%.2f%%)",nRatioData);
		return strText;
//	}
}

CString CLock::GetData(const int nData) const
{
	if(nData==0) return "";

	ML_SET_LANGUAGE_RES();

	// data �� 3�ڸ��� ��� "1":�ŷ���, "2":�ڽ���
	CString strLock;
	switch(nData)
	{
	// �ŷ���
	case 1:
	case 110:
	case 210:	strLock.LoadString( IDS_EX_RIGHTS);
				break;
	case 2:
	case 120:
	case 220:	strLock.LoadString( IDS_EX_DIVIDEND);
				break;
	case 3:
	case 130:
	case 230:	strLock.LoadString( IDS_EX_RIGHTS_AND_DIVIDEND);
				break;
	case 6:		strLock.LoadString( IDS_EX_DOUBLE_DIVIDEND);
				break;
	case 7:		strLock.LoadString( IDS_EX_RIGHTS_AND_DOUBLE_DIVIDEND);
				break;
	case 140:
	case 240:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN);
				break;
	case 141:
	case 241:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_EX_RIGHTS);
				break;
	case 142:
	case 242:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_EX_DIVIDEND);
				break;
	case 143:
	case 243:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_EX_RIGHTS_AND_DIVIDEND);
				break;
	case 146:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_INTERIM_DIVIDEND);
				break;
	case 147:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_EX_RIGHTS_AND_INTERIM_DIVIDEND);
				break;
	case 150:
	case 250:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION);
				break;
	case 151:		
	case 251:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_EX_RIGHTS);
				break;
	case 152:
	case 252:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_EX_DIVIDEND);
				break;
	case 153:
	case 253:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_EX_RIGHTS_AND_DIVIDEND);
				break;
	case 156:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_INTERIM_DIVIDEND);
				break;
	case 157:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_EX_RIGHTS_AND_INTERIM_DIVIDEND);
				break;
	case 160:	strLock.LoadString( IDS_INTERIM_DIVIDEND);
				break;
	case 170:	strLock.LoadString( IDS_EX_RIGHTS_AND_INTERIM_DIVIDEND);
				break;

	// �ڽ���
	case 246:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_REDUCTION_OF_CAPITAL);
				break;
	case 247:	strLock.LoadString( IDS_FACE_VALUE_PARTITIOIN_STOCKS_ANNEXATION);
				break;
	case 256:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_REDUCTION_OF_CAPITAL);
				break;
	case 257:	strLock.LoadString( IDS_FACE_VALUE_ANNEXATION_STOCKS_ANNEXATION);
				break;
	case 260:	strLock.LoadString( IDS_REDUCTION_OF_CAPITAL);
				break;
	case 270:	strLock.LoadString( IDS_STOCKS_ANNEXATION);
				break;
	case 280:	strLock.LoadString( IDS_ENTERPRISE_PARTITION);
				break;
	case 290:	strLock.LoadString( IDS_MUTUAL_FUND);
				break;
	}

	return strLock;
}
//sy end


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.22
//
//	�м�	:	Market Profile (�ð��뺰 ���� ��Ʈ)
//					1. Server������ �����Ǵ� �������� �ð����� �������� ����� �ŷ��� Data�� �����Ѵ�.
//						1. �Ͻ�		: MMDDHHMM
//						2. ����		: ����� ��ǥ �����̴�.
//						3. CODE		: �ð��� �帧�� �����ִ� Alphabet Code
//						4. Index	: ���� ������ ��Ÿ�� Index�̴�.
//					2. Client������ ���� Data�� ����ϱ� ���� ������ Data Structure�� ������Ų��.
//						1. CMPPacket		: MP Chart�� ����� ���� Data Structure Object Class�̴�.
//						2. CMProfile		: CMPPacket�� �����ϴ� �Ϻ� Chart�� Data Structure Object Class��
//												MP Unit Chart Data Structure Object Class�̴�.
//						3. CMPItem			: CMProfile�� �����ϴ� ���ݴ뺰 Graph�� Data Strucre Object Class�̴�.
//						4. CBracket			: CMPItem�� �����ϴ� ���ݴ�/�ð��뺰�� Data Object�̴�.
//												�ٷ� Server���� ������ 1�� Packet Data�� ��Ÿ����.
//					3. m_CurrentIndex, m_CurrentTime, m_CurrentChar, m_CurrentPriceX100
//						1. Server Data�� CMPPacket Data Object�� ��Ͻ�Ű�� ����, ���� ��� ���¸� ��Ÿ����.
//						2. TR�� ��� GetTotalMarketProfile()���� ù Data�� �ʱ�ȭ�ǰ�, (ù Data ������ -1��?)
//								GetTotalMarketProfile()�� ȣ���ϱ� ���� IsValidPacket()���� �� �ڷ� Data Packet�� �ʱ�ȭ�� Data�� �ִ��� �����ȴ�.
//								GetDayMarketProfile()������ ������ ���ڸ� �������� GetTotalMarketProfile()�� ȣ��ǹǷ� �翬 �ʱ�ȭ�� Data�� �ְ� �ȴ�.
//						3. Real�� ��� TR�� ���� �ʱ�ȭ�� ���еǸ�, ���� �Ϻ� ���ο� CMProfile�� �и��ؾ� �ϸ�...
//								Real�� �ð��� DDHHMMSS�� ��Ÿ����.
//
//	����	:	1. Local ������ Member ������ Double�� �����Ͽ� �̿����� �ʴ´�.
//				2. double�� �� ��ü�� ��Ȯ���� �����Ƿ� (�ٻ�ġ�� ���´ٰ� �����ؾ� �Ѵ�.)
//					�׻� Int�� Long ������ ��ȯ�Ͽ� �̿��Ѵ�.
//					(MSDN, Knowledge Base�� Miscellaneous, [Q125056] INFO: Precision and Accuracy in Floating-Point Calculations ����)
//
//	����	:	(2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
//					�̸� ���Ͽ� CMProfile Chart�� Member�� Last Price�� ����Ͽ� ��½� ��������Ѵ�.
//					Server������ �Ϻ�/���� ���о��� TR Packet ���� �߰� Packet Data�� Alphabet�� '*'�� Data�� �޾�,
//						�� Packet Data�� Price�� CMProfile Chart�� Last Price�� �����Ѵ�.
//						�� ���� Packet�� ������ ��ȸ�� �ϴ��� �������� 1���� �߰��ȴ�.
//					�Ϻ��� ��쿡�� ������ Bracket�� �����̹Ƿ� �̸� �̿��Ͽ� �����Ѵ�.
//				(2004.02.10, ��¿�) �Ͻ� Packet�� '��'�� �޾� X Scale�� ������ �Ѵ�.
//				(2005/8/31 - Seung-Won, Bae) '*' is last price indicator. Because Last Braket can be not last price.
//					and first some Aphabet times have no Tick Data. then The First Braket's Alphabet Character must not be 'A'.
//						so. I need the time of Market Open for correcting this.
//					then. I need '*' Packet every time even if there is no tick in Market Open Time.
//						and that packet('*')'s Time must be Market Open Time for the Time of Last 'A'
//				(2005/9/7 - Seung-Won, Bae) In Hanwha, The Real Time 09:05 means 09:04:00.01 ~ 09:05:00.00
//						m_CurrentTime	- 3. Get from TR	: It is the Last Bracket's Time for Last Group Alphabet.
//										  2. Set to TR		: Set to m_LastAMinute with Last Character.
//																CMProfile::SetDate()
//																Reference for Creating New Propfile Chart on Real.
//										- 1. Get from Real	: It is the Last Real Time in Minute, not Current.
//										  2. Set to Real	: Set To Date of Real Time (It is from TR).
//										=> It is used for Only m_LastAMinute in TR.
//										=> It is used in Real for Only Remember Date.
//										=> Then, Is m_LastAMinute the Start Time of A or the End Time of A ?
//						m_LastAMinute	- Get from TR	: It is from Last Time (m_CurrentTime) & Alphabet of TR.
//										- Set to Real	: It is used to make a Alphabet of Real.
//										=> m_LastAMinute is used only for the logic to get Alphabet.
//										=> So. It is good to be the Start Time of Alphabet for the Logic.
//											Because. In the case of No data in TR. Server will send the Constant Market Open Time.
//										=> So. In Hanwha, It must be the Start Time (Minute) + 1 in Minute. ( The Start Time of B is 09:06 in Minute.)
//						Bracket's Time	- It is used to check if New Bracket in same alphabet.
//					So. The Time Field in TR must be the Start Time of Alphabet.
//						And m_CurrentTime is Last Bracket's Start Time. (before Market Open. It's 0900)
//						And It is not matter for Bracket's Time to be the Start Time or +1.
//						So in Hanwha. m_LastAMinute must have +1 for Upper Round.
///////////////////////////////////////////////////////////////////////////////
// (2004.01.16, ��¿�, �м�) MP Chart ��ǥ�� �ʱ�ȭ ó���Ѵ�.
CMarketProfileChart::CMarketProfileChart() : CIndicator( IMetaTable::EDDC_OCX_MARKETPROFILECHART)
{
	m_pEndPriceList = NULL;
	m_pVolumeList = NULL;
	m_pDateList = NULL;

	// (2004.01.19, ��¿�) ���������� ����� Data�� ���� �����Ѵ�.
	//		������ Data �߰��� �����ϱ� �����̴�.
	m_CurrentIndex			= -1;	// ���������� ��ϵ� CBracket�� �ִ� CMPItem�� Index ��ȣ�̴�.
	m_CurrentTime			= -1;	// ���������� ��ϵ� CBracket�� Data Time�̴�.
	m_CurrentPriceX100		= -1;	// ���ڸ����� ��ϵ� CBracket�� �ִ� CMPItem�� �����̴�.
	m_CurrentBracketIndex	= -1;	// ���������� ��ϵ� CBracket�� Index���̴�.
	m_CurrentChar			= ' ';	// ���������� ��ϵ� CBracket�� Character Item�̴�.

	m_strDateType = ' ';
	m_pPacketList = NULL;
	m_pAlpabetList = NULL;
	m_pIndexList = NULL;

	// (2004.01.19, ��¿�) CMPItem���� CBracket�� �ű� �߰��� �ĺ��ϱ� ���Ͽ� ���� �ð��� ������ �����Ѵ�.
	m_nTimeUnit		= 0;
	m_nPriceUnitX100	= 0;

	// (2004.01.26, ��¿�) ������ 'A'�� �ð��� �����Ͽ�, Real�� ó������Ѵ�.
	m_LastAMinute	= -1;

	// (2004.01.26, ��¿�) Real ó���ø� ���� �Ϻ�/���� ���ε� �����Ѵ�.
	m_nTotalMPChart	= 0;	// 1:����, 0:�Ϻ�
}
///////

// (2004.01.16, ��¿�, �м�) MP Chart ��ǥ Data�� �����Ѵ�.
void CMarketProfileChart::GetData( const CChartFormulateData &chartFormulateData,
									CGraphCalculateData *pGraphCalculateData,
									CIndicatorInfo *&pIndicatorInfo,
									CCalculateData &calculateData)
{
	// 1. Packet List�� ���Ѵ�.
	m_pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pPacketList) return;

	// 2. ��ǥ ��� Type�� ���Ѵ�. (���� Sub Routine���� ������ ���̴�.)
	m_CalType = chartFormulateData.GetCalculateType();

	// 3. TR ������ �ʱ� ������ ó���Ѵ�. (TR�̸� �⺻ Data Packet���� Pointer�� �ʱ�ȭ�Ѵ�.)
	if(m_CalType == CCalculateBaseData::TRDATA_CAL)
	{
		// 3.2 ��꿡 �ʿ��� Packet Data���� ���Ѵ�.
		CString strPacketNames = chartFormulateData.GetPacketNames();
		// 3.2.1 ��¥ Packet�� ���Ѵ�.
		CPacket *pPacket = m_pPacketList->GetBaseData( CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pDateList = GetnumericDataList( pPacket);
		// 3.2.1.1 ��¥ Packet�� Type�� ���Ѵ�.
		m_strDateType = pPacket->GetType();
		// 3.2.2 ���� Packet�� ���Ѵ�.
		pPacket = m_pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pEndPriceList = GetnumericDataList( pPacket);
		// 3.2.3 ALPHABET Packet�� ���Ѵ�.
		pPacket = m_pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pAlpabetList = GetstrDataList( pPacket);
		// 3.2.4 Index Packet�� ���Ѵ�.
		pPacket = m_pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pIndexList = GetnumericDataList( pPacket);
		// 3.2.5 �ŷ��� Packet�� ���Ѵ�.
		pPacket = m_pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pVolumeList = GetnumericDataList( pPacket);
	}

	// 4. ������ Data Packet���� ��ȿ������ Ȯ���Ѵ�.
	//		(2004.02.18, ��¿�) Real�� ��� Index List���� Data�� �ϳ��� ���� ��찡 �����Ƿ�
	//			Ȯ������ �ʴ´�.
	if( !IsValidPacket( m_pEndPriceList) || !IsValidPacket( m_pVolumeList) || !IsValidPacket( m_pDateList))
		return;

	// 5. MarketProfile Packet Object Pointer�� ���ϰ�, ���� �ʿ伺�� Ȯ���Ѵ�.
	CPacket *pMarketPacket = NULL;
	if( !SetGraphPacketList( m_pPacketList, pGraphCalculateData, pIndicatorInfo, pMarketPacket)) return;
	if( IsIndicatorCalculated( m_CalType, pMarketPacket)) return;

	// 6. MarketProfile�� ó���� �ʿ��� Condition Data�� Ȯ���Ѵ�.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pIndicatorInfo) return;
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	if(pConditionList == NULL || pConditionList->GetCount() < 3) return ;
	// 6.1 ���� �� CMPItem���� CBracket�� �ű� �߰��� �ĺ��ϱ� ���Ͽ� ���� �ð��� �����Ѵ�.
	// (2005.02.26, ��¿�) Int�� ȯ��� ��ȿ������ ���� X.XXXX99999�� ������ ��쿡 ����Ͽ�
	//		Epsilon�� ���ѵ� Int�� ȯ���Ѵ�.
	m_nPriceUnitX100	= int( ( GetCondition(0, pConditionList) * ( 1 + DBL_EPSILON)) * 100); //���ݴ���
	m_nTimeUnit			= ( int)GetCondition(1, pConditionList);		 //�ð�����
	// 6.2 Real ó���ø� ���� �Ϻ�/���� ���ε� �����Ѵ�.
	m_nTotalMPChart		= ( int)GetCondition(2, pConditionList);		 // 1:����, 0:�Ϻ�

	// 7. MarketProfile Packet�� ���ϴ� ��츦 �����Ͽ� ó���Ѵ�.
	CPoint point								= calculateData.GetPoint();
	CMPPacket* pPacket							= (CMPPacket* )pMarketPacket;
	CScaleBaseData::HORZSCALEDRAWERTYPE nType	= m_pPacketList->GetDateUnitType();
	// 7.0 Draw�ÿ� ������ �����ϵ��� MP Chart Packet�� ������ ������ �����Ѵ�.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacket) return;
	pPacket->SetPriceUnitX100( m_nPriceUnitX100);
	// 7.1 TR�� ��츦 ó���Ѵ�.
	if( m_CalType == CCalculateBaseData::TRDATA_CAL)
	{
		// 7.1.1 ������ ��츦 ó���Ѵ�. (�ι�° ������ 1)
		if( GetCondition( 2, pConditionList))	// 1:����, 0:�Ϻ�
			GetTotalMarketProfile(point, pConditionList, pPacket, 0, m_pEndPriceList->GetCount(), nType);

		// 7.1.2 �Ϻ��� ��츦 ó���Ѵ�. (�ι�° ������ 0)
		else GetDayMarketProfile(point, pConditionList, pPacket, nType);

		// (2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
		//		���� ������ �˷��ִ� ���� Packet�� �����Ͽ� GetTotalMarketProfile()���� ���� ���¸� Backup �����Ѵ�.
		//	m_CurrentChar	= *( const char *)m_pAlpabetList->GetTail();
		// 7.1.3.1 (2004.01.26, ��¿�) ������ 'A'�� �ð��� �����Ͽ�, Real�� ó������Ѵ�.
		// 7.1.3.1.1 ���� ���� �ð��� ���Ѵ�. (�ú� 4�ڸ��� ���Ѵ�.)
		m_LastAMinute	= m_CurrentTime % 10000;
		m_LastAMinute	= m_LastAMinute / 100 * 60 + m_LastAMinute % 100;	// �ð� ������ �����Ѵ�.
		// 7.1.3.1.2 ���� �ð��� ���۽ð��� ���Ѵ�.
		m_LastAMinute	-= m_LastAMinute % m_nTimeUnit;
		// 7.1.3.1.3 ������ 'A'�� ���۽ð��� ���Ѵ�.
		m_LastAMinute	-= ( m_CurrentChar - 'A') * m_nTimeUnit;
		// (2005/9/7 - Seung-Won, Bae) Update Last A Minute for Ceiling Mode
		//		in floor,	A is 0900, 0901, 0902, 0903, 0904
		//		in Ceiling, A is 0901, 0902, 0903, 0904, 0905
		// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
		if( CPacketType::FLOOR != m_pPacketList->GetRealType()) m_LastAMinute++;
	}
	// 7.2 Real�� ��츦 ó���Ѵ�. (Real ó������ ���� Real �غ� ������ GetRealMarketProfile()������ ó���Ѵ�.)
	else GetRealMarketProfile( point, pConditionList, pPacket, nType);

	// 8. ������ Market Profile ��ǥ Packet�� �⺻ ������ ���ó���Ѵ�.
	CPacket* pClosePacket = m_pPacketList->GetBaseData( CString( "MP_") + _MTEXT( C0_CLOSE));
	assert(pClosePacket != NULL);
	SetPacketTypes( pPacket, pClosePacket->GetType(true), pClosePacket->GetType(false), 0);
}

// (2004.01.19, ��¿�, �м�) 1�� MarketProfile ���� Chart�� �����Ѵ�.
//		�Ϻ� Chart�� ��� �ϼ���ŭ�� MarketProfile Chart�� ������� ȣ��ȴ�.
//		(������ CMPPacket�� ����ϴ� ������� CMPPacket�� Member Routine�� �ش��ϴ� ����̳�,
//		Data Packet�� �� Handling Routine�� Indicator Member Routine�� �̿��ϱ� ���Ͽ� ���⼭ ó���Ѵ�.)
void CMarketProfileChart::GetTotalMarketProfile(const CPoint& point, CList<double, double>* pConditionList,
  CMPPacket* pMarketPacket, int Start, int End, CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	// 1. MP Unit Chart Object�� �����ϰ�, ���ڸ� �����Ѵ�.
	// 1.1 ���� MP Unit Chart�� ��ϵ� Data�� MP Unit Chart���� Index�� �ʱ�ȭ�Ѵ�.
	// (2004.01.19, ��¿�) ���������� ����� Data�� ���� �����Ѵ�.
	//		������ Data �߰��� �����ϱ� �����̴�.
	m_CurrentIndex		= 0;														// ���������� ��ϵ� CBracket�� �ִ� CMPItem�� Index ��ȣ�̴�.
	m_CurrentTime		= ( int)GetAt( m_pDateList, ( int)Start);							// ���������� ��ϵ� CBracket�� Data Time�̴�.
	m_CurrentChar		= *( const char *)GetAt( m_pAlpabetList, ( int)Start);		// ���������� ��ϵ� CBracket�� Character Item�̴�.
	// (2005.02.26, ��¿�) Int�� ȯ��� ��ȿ������ ���� X.XXXX99999�� ������ ��쿡 ����Ͽ�
	//		Epsilon�� ���ѵ� Int�� ȯ���Ѵ�.
	m_CurrentPriceX100	= int( ( GetAt( m_pEndPriceList, (int)Start) * ( 1 + DBL_EPSILON)) * 100);	// ���ڸ����� ��ϵ� CBracket�� �ִ� CMPItem�� �����̴�.
	// (2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
	//		Server�κ��� �޴� ���� ���� Packet�� ÷���� �ִ� ��쿡�� ó���� ���� �ʴ´�.
	if( m_CurrentChar == '*')
	{
		m_CurrentChar = 'A';	// (2005/8/31 - Seung-Won, Bae) for Market Open Time.
		return;
	}
	// 1.2 MP Chart Object�� �����Ѵ�.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pMarketPacket) return;
	CMProfile* pMarketProfile = pMarketPacket->AddTail();
	if( !pMarketProfile) return;
	// 1.2 ù Time�� ���ڸ� CMPPacket(���� MP Chart Object)�� ��Ͻ�Ų��.
	pMarketProfile->SetDate( m_CurrentTime);

	// 2. ���� ù Server Packet Data�� �ش��ϴ� CBracket Object�� ���� ����Ͽ�,
	//		���� Packet Data ����� �������� ��´�.
	// 2.1 ���� CMPItem Object�� ����/����Ѵ�.
	CMPItem* pMPItem = pMarketProfile->AddTail();
	if( !pMPItem) return;
	// 2.2 ������ CMPItem Object�� ��ǥ ���ݰ� �ŷ����� �����Ѵ�.
	//		(�̶� ������ Backup�Ͽ� ���� Packet Data ��Ͻ� ������� �Ѵ�.)
	pMPItem->SetPrice( m_CurrentPriceX100);
	pMPItem->AddVolume( ( DWORD)GetAt( m_pVolumeList, Start));
	// (2004.01.27, ��¿�) Real ���Žø� ���Ͽ� ���� Index�� �����Ѵ�.
	m_CurrentBracketIndex = ( int)GetAt( m_pIndexList, Start);
	// 2.3 ������ CMPItem Object�� Server Packet Data�� �´� CBracket Object�� ����/����Ѵ�.
	pMPItem->AddBracket( CString( m_CurrentChar), m_CurrentBracketIndex, m_CurrentTime);

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pIndexList || !m_pVolumeList || !m_pAlpabetList || !m_pEndPriceList || !m_pDateList) return;

	// 3. Server�� Packet Data�� Iterating�ϸ鼭 �� Data�� ���ó���Ѵ�.
	int			nPriceX100, nIndex, nDate;
	double		dVolume;		// �ŷ����� ��ġ�� ũ�Ƿ� Double�� �����Ѵ�.
	CString		strAlphabet;
	POSITION	posIndex	= FindPosition( ( int)Start + 1, *m_pIndexList);
	POSITION	posVolume	= FindPosition( ( int)Start + 1, *m_pVolumeList);
	POSITION	posAlphabet	= FindPosition( ( int)Start + 1, *m_pAlpabetList);
	POSITION	posEndPrice	= FindPosition( ( int)Start + 1, *m_pEndPriceList);
	POSITION	posDate		= FindPosition( ( int)Start + 1, *m_pDateList);
	for( int count = Start + 1; count <= End; count++)
	{
		// 3.1 ���� ������ Server Packet�� Data�� ���ڶ� ��츦 ó���Ѵ�.
		if( !posEndPrice || !posVolume || !posAlphabet || !posIndex) return;

		// 3.2 Server�� Packet Data�� ���Ѵ�.
		// (2005.02.26, ��¿�) Int�� ȯ��� ��ȿ������ ���� X.XXXX99999�� ������ ��쿡 ����Ͽ�
		//		Epsilon�� ���ѵ� Int�� ȯ���Ѵ�.
		nPriceX100	= int( ( GetNext( m_pEndPriceList, posEndPrice) * ( 1 + DBL_EPSILON)) * 100);
		dVolume		= GetNext( m_pVolumeList,	posVolume);
		strAlphabet	= GetNext( m_pAlpabetList,	posAlphabet);
		nIndex		= ( int)GetNext( m_pIndexList,	posIndex);
		nDate		= ( int)GetNext( m_pDateList,		posDate);

		// (2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
		//		���� ������ �˷��ִ� ���� Packet�� ��쿡�� MP Chart�� ������ �����ϰ� ó���� �����Ѵ�.
		// 3.3.1 '*'�� ��츦 Ȯ���Ѵ�.
		if( *( const char *)strAlphabet == '*')
		{
			// (2005/8/31 - Seung-Won, Bae) Set Current Time with Last Bracket Time.
			m_CurrentTime		= nDate;

			pMarketProfile->SetLastPriceX100( nPriceX100);
			return;
		}
		// 3.3.2 '*'�� ��츦 �����Ͽ� TRó���� ������ ���� GetData()�� �ƴ� �̰����� �Ź� Backup �޾� ���´�.
		m_CurrentChar	= *( const char *)strAlphabet;

		// 3.4 ���� ���ݰ� ���Ͽ� Data�� ��, �Ǵ� �Ʒ��� Ȯ���Ͽ� ��Ͻ�Ų��.
		if(			nPriceX100 > m_CurrentPriceX100) InsertUp(		pMarketProfile, nPriceX100, dVolume, strAlphabet, nIndex, nDate);
		else if(	nPriceX100 < m_CurrentPriceX100) InsertDown(	pMarketProfile, nPriceX100, dVolume, strAlphabet, nIndex, nDate);

		// 3.5 ���� ���ݰ� ���� ���, ������ ����� CMPItem Object�� Data�� �߰��Ѵ�.
		else
		{
			CMPItem *pMPItem = pMarketProfile->GetMPItem( m_CurrentIndex);
			if( !pMPItem) return;
			pMPItem->AddVolume( ( unsigned long)dVolume);

			// 3.6 ���� ���ݰ� ���Ƶ� �翬 Server���� �ٸ� Bracket���� �������� �״� ������ �߰��Ѵ�.
			pMPItem->AddBracket( strAlphabet, nIndex, nDate);

			// (2004.01.27, ��¿�) Real ���Žø� ���Ͽ� ���� Index�� �����Ѵ�. (Insert Up/Down������ ó���Ѵ�.)
			m_CurrentBracketIndex = nIndex;
		}
	}

	// (2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
	//		�Ϻ��� ��ȸ�ϸ鼭 ������ MP Chart�� �ƴ� ����� ��������, (�������� '*'�� Ȯ��������)
	//		������ ����� Bracket�� �������� MP Chart�� ������ ������ �����Ѵ�.
	pMarketProfile->SetLastPriceX100( nPriceX100);
}

// (2004.01.19, ��¿�, �м�) ������ ��ϵ� CMPItem�� Index�� m_CurrentIndex�� �������� Ȯ���ϸ鼭,
//		Data�� ��Ͻ�Ų��.
void CMarketProfileChart::InsertUp( CMProfile* pMarketProfile, int nPriceX100, double dVolume, CString& strAlpa, int nIndex, int nDate)
{
	// 1. (2004.01.27, ��¿�) Real ���Žø� ���Ͽ� ���� Index�� �����Ѵ�.
	m_CurrentBracketIndex = nIndex;

	// 2. CMPItem�� List�� ���� Iterating�ϸ鼭 �߰��� Data ���ݿ� �´� CMPItem�� ã�� ��Ͻ�Ų��.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pMarketProfile) return;
	CList<CMPItem*, CMPItem*>* pMPItemList = pMarketProfile->GetMPItemList();
	if( !pMPItemList) return;
	POSITION posMPItem = pMPItemList->FindIndex( m_CurrentIndex + 1);
	while( posMPItem)
	{
		// 2.1 ���� ���� ���� CMPItem Object�� ���Ѵ�.
		CMPItem *pMPItem = pMPItemList->GetNext( posMPItem);

		// 2.2 CMPItem�� ��ϵ� Position�� �ְ�, Object Pointer�� NULL�̸� ������ �ٷ� Return�Ѵ�.
		if( !pMPItem) return;

		// 2.2.5 ������ ���ݰ� CMPItem�� Index�� �����Ͽ� Ȯ��ÿ� �����Ѵ�.
		//	(������ ��ϴ���� CMPItem�� ������ �����޾ƾ� �ϳ�, Ȯ���� ��� ������ �� �ֵ��� �̸� �޾� ���´�.)
		m_CurrentPriceX100 = pMPItem->GetPriceX100();
		m_CurrentIndex++;

		// 2.3 ����� Data�� ������ ���� CMPItem Object�� ã�� ���, �ش� Object�� Data�� ��Ͻ�Ű�� Return�Ѵ�.
		if( m_CurrentPriceX100 == nPriceX100)
		{
			// 2.3.1 �ŷ����� �����Ų��.
			pMPItem->AddVolume( ( unsigned long)dVolume);

			// 2.3.2 CBracket Object�� �߰���ų �ʿ䰡 �ִ��� Ȯ���Ͽ� �߰��Ѵ�.
			//	( CBracket Object�� �߰����� �ʴ� ����, ���������� ��ϵ� CBracket Object�� 
			//	  �̹� Data�� �ð������� �ϳ��� ������ ���� ����̴�. (Real)
			//	  ����, �켱������ �ð��� ������ ���� �������� ū ���� Ȯ���ϰ�,
			//			�׷��� ���� ��� (�ð� ������ ���� �������� �۴���) Aphabet�� ����� ��쵵 ��� ó���Ѵ�.)
			//	( *. ����, CMPItem�� �ִٰ� �ؼ� �׻� CBracket Object�� �������� ���� �� �ִ�. JUMP ����)
			// (2004.01.27, ��¿�) TR ���Žÿ��� Bracket ������ Server���� å������ ó���ϹǷ� ������ ����Ѵ�.
			CList< CBracket *, CBracket *> *pBracketList = pMPItem->GetBracketList();
			// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
			if( !pBracketList) return;
			if( pBracketList->IsEmpty() || m_CalType == CCalculateBaseData::TRDATA_CAL) pMPItem->AddBracket( strAlpa, nIndex, nDate);
			else
			{
				CBracket *pBracket = pBracketList->GetTail();
				// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
				if( !pBracket) return;
				if( nDate - pBracket->GetDate() > m_nTimeUnit	// ���� �ʸ� �����ϹǷ� ���ؽõ� �ð������� ������ ȯ���Ѵ�.
					|| *( const char *)pBracket->GetItem() != *( const char *)strAlpa)
						pMPItem->AddBracket( strAlpa, nIndex, nDate);
				// (2004.01.27, ��¿�) Real ���Žø� ���Ͽ� ���� Index�� �����Ѵ�.
				//		(InsertUp ���۽� �ٷ� Backup�Ͽ�����, Real�� ���� ��Ͻÿ� ���ο� Bracket�� �������� ������ �ٽ� ���Ѵ�.)
				else m_CurrentBracketIndex--;
			}

			// 2.3.4 Data�� �߰��� �������Ƿ� Return�Ѵ�.
			return;
		}
	}

	// 3. �̹� ��ϵ� CMPItem�� List�� �˸��� ���ݴ밡 ���ٸ�, ���� Ȯ���ϰ� ��Ͻ�Ų��.
	// 3.1 ���������� ��ϵǾ� �ִ� ������ �������� �ʿ��� CMPItem Object�� Ȯ���Ѵ�.
	// 3.1.1 ���� ����ؾ� �ϴ� CMPItem Object�� ���� Ȯ���Ѵ�.
	// (2005/8/12 - Seung-Won, Bae) Check Devide by Zero
	if( !m_nPriceUnitX100) return;
	int nCount = ( nPriceX100 - m_CurrentPriceX100) / m_nPriceUnitX100;
	// 3.1.2 CMPItem Object�� ������ŭ �����Ѵ�.
	CMPItem *pMPItem = NULL;
	for( int i = 0; i < nCount; i++)
	{
		m_CurrentPriceX100 += m_nPriceUnitX100;
		m_CurrentIndex++;

		pMPItem = pMarketProfile->AddTail();
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pMPItem) return;
		pMPItem->SetPrice( m_CurrentPriceX100);
	}
	// 3.2 ���������� ����� CMPItem Object�� CBracket Object�� �����ϸ� Data�� ��Ͻ�Ų��.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pMPItem) return;
	pMPItem->AddVolume( ( unsigned long)dVolume);
	pMPItem->AddBracket( strAlpa, nIndex, nDate);
}

// (2004.01.19, ��¿�, �м�) ������ ��ϵ� CMPItem�� Index�� m_CurrentIndex�� �Ʒ������� Ȯ���ϸ鼭,
//		Data�� ��Ͻ�Ų��.
void CMarketProfileChart::InsertDown( CMProfile* pMarketProfile, int nPriceX100, double dVolume, CString& strAlpa, int nIndex, int nDate)
{
	// 1. (2004.01.27, ��¿�) Real ���Žø� ���Ͽ� ���� Index�� �����Ѵ�.
	m_CurrentBracketIndex = nIndex;

	// 2. CMPItem�� List�� ���� Iterating�ϸ鼭 �߰��� Data ���ݿ� �´� CMPItem�� ã�� ��Ͻ�Ų��.
	//	(���� ���� m_CurrentIndex���� �̿��Ͽ� ã�� ����� �ִ��� Ȯ���Ѵ�.)
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pMarketProfile) return;
	CList<CMPItem*, CMPItem*>* pMPItemList = pMarketProfile->GetMPItemList();
	if( !pMPItemList) return;
	POSITION posMPItem = NULL;
	if( 0 < m_CurrentIndex) posMPItem = pMPItemList->FindIndex( m_CurrentIndex - 1);
	while( posMPItem && 0 < m_CurrentIndex)
	{
		// 2.1 ���� ���� �Ʒ��� CMPItem Object�� ���Ѵ�.
		CMPItem* pMPItem = pMPItemList->GetPrev(posMPItem);

		// 2.2 CMPItem�� ��ϵ� Position�� �ְ�, Object Pointer�� NULL�̸� ������ �ٷ� Return�Ѵ�.
		if( !pMPItem) return;

		// 2.2.5 ������ ���ݰ� CMPItem�� Index�� �����Ͽ� Ȯ��ÿ� �����Ѵ�.
		//	(������ ��ϴ���� CMPItem�� ������ �����޾ƾ� �ϳ�, Ȯ���� ��� ������ �� �ֵ��� �̸� �޾� ���´�.)
		m_CurrentPriceX100 = pMPItem->GetPriceX100();
		m_CurrentIndex--;

		// 2.3 ����� Data�� ������ ���� CMPItem Object�� ã�� ���, �ش� Object�� Data�� ��Ͻ�Ű�� Return�Ѵ�.
		if( m_CurrentPriceX100 == nPriceX100)
		{
			// 2.3.1 �ŷ����� �����Ų��.
			pMPItem->AddVolume( ( unsigned long)dVolume);

			// 2.3.2 CBracket Object�� �߰���ų �ʿ䰡 �ִ��� Ȯ���Ͽ� �߰��Ѵ�.
			//	( CBracket Object�� �߰����� �ʴ� ����, ���������� ��ϵ� CBracket Object�� 
			//	  �̹� Data�� �ð������� �ϳ��� ������ ���� ����̴�. (Real)
			//	  ����, �켱������ �ð��� ������ ���� �������� ū ���� Ȯ���ϰ�,
			//			�׷��� ���� ��� (�ð� ������ ���� �������� �۴���) Aphabet�� ����� ��쵵 ��� ó���Ѵ�.)
			//	( *. ����, CMPItem�� �ִٰ� �ؼ� �׻� CBracket Object�� �������� ���� �� �ִ�. JUMP ����)
			// (2004.01.27, ��¿�) TR ���Žÿ��� Bracket ������ Server���� å������ ó���ϹǷ� ������ ����Ѵ�.
			CList< CBracket *, CBracket *> *pBracketList = pMPItem->GetBracketList();
			// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
			if( !pBracketList) return;
			if( pBracketList->IsEmpty() || m_CalType == CCalculateBaseData::TRDATA_CAL) pMPItem->AddBracket( strAlpa, nIndex, nDate);
			else
			{
				CBracket *pBracket = pBracketList->GetTail();
				// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
				if( !pBracket) return;
				if( nDate - pBracket->GetDate() > m_nTimeUnit	// ���� �ʸ� �����ϹǷ� ���ؽõ� �ð������� ������ ȯ���Ѵ�.
					|| *( const char *)pBracket->GetItem() != *( const char *)strAlpa)
						pMPItem->AddBracket( strAlpa, nIndex, nDate);
				// (2004.01.27, ��¿�) Real ���Žø� ���Ͽ� ���� Index�� �����Ѵ�.
				//		(InsertDown ���۽� �ٷ� Backup�Ͽ�����, Real�� ���� ��Ͻÿ� ���ο� Bracket�� �������� ������ �ٽ� ���Ѵ�.)
				else m_CurrentBracketIndex--;
			}

			// 2.3.4 Data�� �߰��� �������Ƿ� Return�Ѵ�.
			return;
		}
	}

	// 3. �̹� ��ϵ� CMPItem�� List�� �˸��� ���ݴ밡 ���ٸ�, ���� Ȯ���ϰ� ��Ͻ�Ų��.
	// 3.1 ���������� ��ϵǾ� �ִ� ������ �������� �ʿ��� CMPItem Object�� Ȯ���Ѵ�.
	// 3.1.1 ���� ����ؾ� �ϴ� CMPItem Object�� ���� Ȯ���Ѵ�.
	int nCount = ( m_CurrentPriceX100 - nPriceX100) / m_nPriceUnitX100;
	// 3.1.2 CMPItem Object�� ������ŭ �����Ѵ�.
	CMPItem *pMPItem = NULL;
	for( int i = 0; i < nCount; i++)
	{
		m_CurrentPriceX100 -= m_nPriceUnitX100;
		m_CurrentIndex = 0;

		pMPItem = pMarketProfile->AddHead();
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pMPItem) return;
		pMPItem->SetPrice( m_CurrentPriceX100);
	}
	// 3.2 ���������� ����� CMPItem Object�� CBracket Object�� �����ϸ� Data�� ��Ͻ�Ų��.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pMPItem) return;
	pMPItem->AddVolume( ( unsigned long)dVolume);
	pMPItem->AddBracket( strAlpa, nIndex, nDate);
}

// (2004.01.19, ��¿�, �м�) �Ϻ� MP Chart�� �����ϴ� Routine�� �����Ѵ�.
void CMarketProfileChart::GetDayMarketProfile(const CPoint& point, CList<double, double>* pConditionList,
			CMPPacket* pMarketPacket, CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	// 1. CMProfile Object�� ������ ������ ���ذ��� �ʱ�ȭ�Ѵ�.
	//	(dLastIndex�� nNextDate�� Index���̰�, dStartIndex�� nPrevDate�� Index�̴�.
	//	 ��, ó�� ������ ���Ͽ� nPrevDate�� -1�� �ʱ�ȭ�Ͽ���.)
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pDateList) return;
	int		nPrevDate = -1;
	int		nNextDate = GetMDay( ( int)m_pDateList->GetHead());
	int 	nStartIndex = 0;
	// (2004.01.26, ��¿�) nLastIndex�� Loop������ �ٷ� �����ϹǷ� �̸� 1 ���ҽ�Ų��. (���� ZeroBase)
	int		nLastIndex = -1;

	// 2. ������ �˻��Ͽ� CMProfile Object�� �����Ѵ�.
	POSITION posDate = m_pDateList->GetHeadPosition();
	while( posDate)
	{
		// 2.1 ���� ���ڿ� �ٸ� ��� ���ο� ������ Data ���� Ȯ���Ѵ�.
		//	(nPrevDate�� ó���� -1�� ������ ������ ���� �ִ�.)
		nPrevDate = nNextDate;
		while( posDate && nPrevDate == nNextDate)
		{
			nNextDate = GetMDay( ( int)GetNext( m_pDateList, posDate));
			nLastIndex++;
		}

		// 2.2 GetTotalMarketProfile()�� ������ Data�� CMProfile Object�� �����Ѵ�.
		//	(���� dLastIndex Index�� nNextDate���� �̹� ���� �ۿ� �ش��Ѵ�.)
		// (2004.02.05, ��¿�) ������ ������ ���, ���ڰ� �ٲ�� ������ ������ �ʰ� Data�� ���� ������ ������.
		//		����, �װ�� nLastIndex�� ���� ���� ����Ų��. �̸� �ĺ��Ͽ� ������ �����ϵ��� �Ѵ�.
		GetTotalMarketProfile( point, pConditionList, pMarketPacket, nStartIndex, nLastIndex - ( posDate ? 1 : 0), nType);

		// 2.3 ���� ������ ���Ͽ� Start���� �����Ѵ�.
		//	(nPrevDate�� 2.1�� Loop���� ó���ȴ�.)
		nStartIndex = nLastIndex;
	}
}

// (2004.01.19, ��¿�, �м�) �Ϻ� ����� ���Ͽ� ���� ����Ÿ(�� DDHHMMSS ������ DATE���� DD���� �̾� ����..
int CMarketProfileChart::GetMDay( int nDate)
{
	// 1. ���� Date Data Format�� Ȯ���Ѵ�.
	int nDataTypeLen = m_strDateType.GetLength();
	if( nDataTypeLen < 2) return 0;

	// 2. Date Data Format���� "DD"�� �ڸ����� Ȯ���Ѵ�.
	int nDigit = m_strDateType.Find( "DD");
	if( nDigit < 0) return 0;

	// 3. "DD"������ �ڸ����� Ȯ���Ѵ�.
	nDigit = nDataTypeLen - nDigit - 2;

	// 4. "DD" ������ �ڸ�����ŭ 10^n���� ���� "DD"���ĸ� �����Ų��.
	nDate = int( nDate / pow( 10, nDigit));

	// 5. 100�� �������� "DD" �ڸ��� ���� ���Ѵ�.
	return nDate % 100;
}

// (2004.01.19, ��¿�, �м�) Real�� ���ŵǴ� ����� MP Chart�� �����Ѵ�.
void CMarketProfileChart::GetRealMarketProfile(const CPoint& point,CList<double, double>* pConditionList, 
			CMPPacket* pMarketPacket, CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	// 1. ���� �ŷ��� Data���� ���� Data���� ���� �ʴ� ��츦 �����Ѵ�.
	//	(���� Packet ���� Real�� ���ŵ��� �ʴ� ��쿡 ���� Code�̴�.)
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pDateList || !m_pVolumeList) return;
	if( m_pVolumeList->GetCount() != m_pDateList->GetCount()) return;
	
	// 2. �� CMProfile List�� ������ ���ϴ� ��츦 ó���Ѵ�.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pMarketPacket) return;
	CList<CMProfile*, CMProfile*>* pMProfileList = pMarketPacket->GetMarketProfileList();
	if( !pMProfileList) return;

	// 3. Real ����� CMProfile Chart Object�� �˻��Ѵ�.
	//	(���� ������ CMProfile Object�� ��ϵ��� ���� ��쵵 ó���Ѵ�. �ٷ� ���� ù Real�� ����̴�.)
	BOOL bAddNewMProfile = FALSE;
	CMProfile *pMarketProfile = NULL;
	// 3.1 ���ο� CMProfile Object�� �ʿ��� ���� �Ǵ��Ѵ�.
	// 3.1.0 Real�� �ð�(Time)�� �̸� Ȯ���Ѵ�. (�߰� ������ �Ǹ�, ���ڴ� ����.)
	// (2004.01.27, ��¿�) ���� Packet�� 'MP_�ð�'���� '�ڷ�����'�� �����Ͽ� DDHHMMSS�� Data�� ����,
	//		���� TR Packet�� DDHHMM���� ó���ȴ�. ���� �ʸ� ���ش�.
	// (2004.02.10, ��¿�) �Ͻ� Packet�� '��'�� �޾� X Scale�� ������ �Ѵ�.
	//		�� TR Packet�� MMDDHHMM���� ���ŵȴ�.
	//		����, Real�� ���ŵ� �Ͻÿ��� �ð��� �����Ͽ�, ������ ���ŵ� TR Data�� ������ �߰��Ѵ�.
	// (2005/8/31 - Seung-Won, Bae) Real�� HHMMSS�� ���ŵǾ �ڷ����� Packet���� �ڵ����� MMDDHHMM���� �����Ѵ�.
	//		���� HHMM�� �����Ͽ� �̿��Ѵ�.
	// (2006/5/25 - Seung-Won, Bae) PacketData.dll process Date Setting already.
	//	int nDate = int( m_pDateList->GetTail()) % 10000 + ( ( m_CurrentTime / 10000) * 10000);
	int nDate = ( int)m_pDateList->GetTail();
	// 3.1.1 CMProfile Chart�� �ϳ��� ���ų�
	if( pMProfileList->IsEmpty()) bAddNewMProfile = TRUE;
	else
	{
		// 3.1.2 �̸� ������ CMProfile Chart�� �˻��Ͽ� ���Ŀ� ������ �������� �ʴ� ��쿡�� ����Ѵ�.
		pMarketProfile = pMProfileList->GetTail();
		// 3.1.3 ���� Mode�� �ƴϸ鼭, ������ ��ϵ� CMProfile Chart�� ������ �ð��� �̹� Real �ð����� �ʴ� ��� (�ð� ����)
		//		(�̶� ������ CMProfile�� ������ Bracket�� �ð��� ���� �ʰ�, m_CurrentTime�� ������ Bracket�� �ð��� ���� �Ϳ� ��������.)
		//		(Real ó�� ������ ���� �ð��� ��� �ִ� Packet�� ��ȸ�Ǳ⵵ �Ѵ�. �Ƹ� ���� �ð� Packet�� �ݿ����� ������..)
		bAddNewMProfile = ( !m_nTotalMPChart && ( nDate < m_CurrentTime));
		// (2004.02.10, ��¿�) �Ͻ� Packet�� '��'�� �޾� X Scale�� ������ �Ѵ�.
		//		Real�� ��� �ð��� �����Ǵ� ���� ����. �� TR�� ������ �ð����� �̸� Real�� �� �� ����.
		//		���� �ִٸ� ������ Open�� Map�� Real�� �����ϴ� ����̴�.
		//		�̷��� ��츦 �����Ͽ� ���ڸ� �����ϴ� Code�� �߰���Ų��.
		//		Ư�� ��ü������ ��쿡�� ������ ���ڸ� �ٷ� �����߸� Alphabet�� ���������� ��Ÿ����.
		if( m_nTotalMPChart && ( nDate < m_CurrentTime)) nDate += 10000;	// ���ڸ� �ϳ� �÷��ش�.
	}
	// 3.2 ������ �����Ѵ�. (�������� �ʴ� ���, ������ CMProfile Chart�� �˻��Ǿ� �ִ�.)
	if( bAddNewMProfile)
	{
		// (2004.02.10, ��¿�) �Ͻ� Packet�� '��'�� �޾� X Scale�� ������ �Ѵ�.
		//		Real�� ��� �ð��� �����Ǵ� ���� ����. �� TR�� ������ �ð����� �̸� Real�� �� �� ����.
		//		���� �ִٸ� ������ Open�� Map�� Real�� �����ϴ� ����̴�.
		//		�̷��� ��츦 �����Ͽ� ���ڸ� �����ϴ� Code�� �߰���Ų��.
		SYSTEMTIME stDate;
		GetLocalTime( &stDate);
		nDate = nDate % 10000 + stDate.wMonth * 1000000 + stDate.wDay * 10000;

		pMarketProfile = pMarketPacket->AddTail();
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pMarketProfile) return;
		pMarketProfile->SetDate( nDate);
	}

	// 4. ���� Real�� ���ŵ� Packet�� Data�� Ȯ���Ѵ�.
	// 4.1 �ð�(Time) Ȯ���Ѵ�. (�߰� ������ �Ǹ�, ���ڴ� ����.)
	//		(���� CMProfile �˻��� �����ϱ� ���Ͽ� �̸� Ȯ���Ͽ���.)
	// 4.2 ���ŵ� ������ ���� ������ ���� ����Ѵ�. (���� ���������� ���� ������ �����Ѵ�.)
	// (2005.02.26, ��¿�) Int�� ȯ��� ��ȿ������ ���� X.XXXX99999�� ������ ��쿡 ����Ͽ�
	//		Epsilon�� ���ѵ� Int�� ȯ���Ѵ�.
	int nPriceX100 = ( int( ( m_pEndPriceList->GetTail() * ( 1 + DBL_EPSILON)) * 100) / m_nPriceUnitX100) * m_nPriceUnitX100;
	// 4.3 ����� �ŷ��� Data�� ��ȸ�Ѵ�. (Real������ �ŵ�/�ż��� ���� ��ȣ�� �����Ǿ� �ִ�.)
	double dVolume = fabs( m_pVolumeList->GetTail());
	// 4.4 Alphabet�� ����Ͽ� �����Ѵ�.
	// 4.4.1 ���ο� CMProfile Chat�� ��Ȳ������ Alphabet ������ ���Ͽ� 'A' ���ؽø� �����Ѵ�.
	// (2005/8/31 - Seung-Won, Bae) Server���� �ּ��� 1���� Packet Set('*')�� �����ָ鼭
	//		ü���� ���� ��� ����۽ð��� �����־� �ݵ�� m_LastAMinute�� �ʱ�ȭ�ǵ��� �Ͽ� �߰� �ʱ�ȭ Logic�� �����Ѵ�.
	// 4.4.2 Real�� Alphabet�� �����Ѵ�.
	// 4.4.2.1 ���� Real �ð��� �м��� �����Ѵ�. (���ؽÿ� ���ϱ� �����̴�.)
	int nRealMinute = nDate % 10000;
	nRealMinute	= nRealMinute / 100 * 60 + nRealMinute % 100;	// �ð� ������ �����Ѵ�.
	// 4.4.2.2 ���ؽ÷κ��� �帥 �ð� �����Ѵ�.
	nRealMinute -= m_LastAMinute;
	// 4.4.2.3 �м��� �´� Aphabet�� �����Ѵ�.
	CString strAlpha;
	char *szAlpha = strAlpha.GetBufferSetLength( 2);
	szAlpha[ 0] = 'A' + int( nRealMinute / m_nTimeUnit) % 26;

	// 5. TR�� ���ŵ��� ���� ��Ȳ������ Real �߰��� ����, ���� ���� �������� �ʱ�ȭ�Ͽ� ������ ���� ȯ���� �����Ѵ�.
	//		(�����δ� ���ο� CMProfile�� �����Ǵ� ��츶�� �ʱ�ȭ�ϰ� ������ ��������Ѵ�.)
	if( pMarketProfile->IsEmpty())
	{
		// 5.1 �⺻ �������� �ʱ�ȭ�Ѵ�.
		m_CurrentIndex			= 0;			// ���������� ��ϵ� CBracket�� �ִ� CMPItem�� Index ��ȣ�̴�.
		m_CurrentChar			= 'A';			// ���������� ��ϵ� CBracket�� Character Item�̴�.
		m_CurrentPriceX100		= nPriceX100;	// ���ڸ����� ��ϵ� CBracket�� �ִ� CMPItem�� �����̴�.
		m_CurrentTime			= nDate;		// ���������� ��ϵ� CBracket�� ��Ͻð��̴�.
		m_CurrentBracketIndex	= 0;			// ���������� ��ϵ� CBracket�� Index�̴�.

		// 5.2 CMProfile Data Object�� ù Data ����� ��츦 ó���Ѵ�.
		CMPItem *pMPItem = pMarketProfile->AddHead();
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pMPItem) return;
		pMPItem->SetPrice( nPriceX100);
	}

	// 6. ������ ����� Data�� �������� Insert ó���Ѵ�.
	if( nPriceX100 > m_CurrentPriceX100)		InsertUp(	pMarketProfile, nPriceX100, dVolume, strAlpha, m_CurrentBracketIndex + 1, nDate); 
	else if( nPriceX100 < m_CurrentPriceX100)	InsertDown(	pMarketProfile, nPriceX100, dVolume, strAlpha, m_CurrentBracketIndex + 1, nDate);
	// 6.1 ���� ���ݰ� ���� ���, ������ ����� CMPItem Object�� Data�� �߰��Ѵ�.
	else
	{
		CMPItem *pMPItem = pMarketProfile->GetMPItem( m_CurrentIndex);
		if( !pMPItem) return;
		pMPItem->AddVolume( ( unsigned long)dVolume);

		//	( *. ����, CMPItem�� �ִٰ� �ؼ� �׻� CBracket Object�� �������� ���� �� �ִ�. JUMP ����)
		CList< CBracket *, CBracket *> *pBracketList = pMPItem->GetBracketList();
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pBracketList) return;
		if( pBracketList->IsEmpty()) pMPItem->AddBracket( strAlpha, ++m_CurrentBracketIndex, nDate);		// ���� Index�� 1������Ų��.
		else
		{
			CBracket *pBracket = pBracketList->GetTail();
			// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
			if( !pBracket) return;
			if( nDate - pBracket->GetDate() > m_nTimeUnit		// ���� �ʸ� �����ϹǷ� ���ؽõ� �ð������� ������ ȯ���Ѵ�.
				|| *( const char *)pBracket->GetItem() != szAlpha[ 0])
					pMPItem->AddBracket( strAlpha, ++m_CurrentBracketIndex, nDate);	// ���� Index�� 1������Ų��.
		}
	}

	// 7. ���� Real ó���ÿ� ������ �� �ֵ��� ���� Data�� �����Ѵ�.
	// m_CurrentIndex�� InsertUp�� Down���� �����ȴ�.
	m_CurrentPriceX100	= nPriceX100;	// ���ڸ����� ��ϵ� CBracket�� �ִ� CMPItem�� �����̴�.
	m_CurrentTime		= nDate;		// ���������� ��ϵ� CBracket�� Data Time�̴�.
	m_CurrentChar		= szAlpha[ 0];	// ���������� ��ϵ� CBracket�� Character Item�̴�.
	// m_CurrentBracketIndex�� InsertUp�� Down���� �����ȴ�.

	// (2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
	//		Real�� ���, Real ó���� MP Chart�� ���� �����μ� ����Ѵ�.
	pMarketProfile->SetLastPriceX100( nPriceX100);
}


//******************************************************************************************************************
//	CPF
//	  
//******************************************************************************************************************

CPF::CPF() : 
	m_pTLBTop(NULL), 
	m_pTLBBottom(NULL),
	m_pTLBDate(NULL),
	m_pEndPricePacket(NULL),
	m_pDatePacket(NULL),
	m_pEndPriceList(NULL),
	m_pDatePriceList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_PANDFCHART)
{
}

void CPF::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList || !pIndicatorInfo) return;
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();

//	(2006.08.14, �Ƚ�ȯ) ��ĭ�� ���� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	CList<double, double>* pDrawConditionList = pIndicatorInfo->GetDrawConditionList();
	BOOL	bReal = FALSE;
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL)
		bReal = TRUE;
//	(2006.08.14, �Ƚ�ȯ) ��ĭ�� ���� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pEndPricePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pEndPriceList = GetnumericDataList(m_pEndPricePacket);
		m_pDatePacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDatePriceList = GetnumericDataList(m_pDatePacket);
	}
	if(!IsValidPacket(m_pEndPriceList) || !IsValidPacket(m_pDatePriceList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	double nMin = 0;
	double nMax = 0;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pEndPricePacket || !m_pDatePacket) return;

	CString strPriceType = m_pEndPricePacket->GetType(false);
	CString strDateType = m_pDatePacket->GetType(false);
	SetPacketTypes(m_pTLBTop, strPriceType, strPriceType, 0);
	SetPacketTypes(m_pTLBBottom, strPriceType, strPriceType, 0);
	SetPacketTypes(m_pTLBDate, strDateType, strDateType, 0);

	m_pEndPricePacket->GetMinMax(0, m_pEndPricePacket->GetCount() - 1, nMin, nMax);
	GetPF(calculateData.GetPoint(), nMin, nMax, pConditionList, m_pEndPriceList, m_pDatePriceList, 
				m_pTLBTop, m_pTLBBottom, m_pTLBDate,pDrawConditionList,bReal, 
				pPacketList->GetDataInsertType() == CPacketDataInfo::NEW, pPacketList);
}

//Ư����
bool CPF::SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	return (m_pTLBTop != NULL && m_pTLBBottom != NULL && m_pTLBDate != NULL);
}

//Ư����
bool CPF::InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;

	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;

	m_pTLBTop		= pPacketList->GetBaseData( _MTEXT( C0_PF_HIGH),		false);//"PF_��"
	m_pTLBBottom	= pPacketList->GetBaseData( _MTEXT( C0_PF_LOW),			false);//"PF_����"
	m_pTLBDate		= pPacketList->GetBaseData( _MTEXT( C0_PF_DATE_TIME),	false);//"PF_�ڷ�����"

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( m_pTLBTop);
	pSubGraphList->AddTail2( m_pTLBBottom);
	pSubGraphList->AddTail2( m_pTLBDate);
	return (m_pTLBTop != NULL || m_pTLBBottom != NULL || m_pTLBDate != NULL);
}

//���߿� �ǽð� ó�� Ȯ��
//�ǽð� ���ö��� ÷���� �ٽ� ����ؾ� �Ǵϱ�...data�� ���� �����ش�.
bool CPF::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL)
	{
		m_pTLBTop = pPacketList->GetBaseData(		_MTEXT( C0_PF_HIGH),		false);//"PF_��"
		m_pTLBBottom = pPacketList->GetBaseData(	_MTEXT( C0_PF_LOW),			false);//"PF_����"
		m_pTLBDate = pPacketList->GetBaseData(		_MTEXT( C0_PF_DATE_TIME),	false);//"PF_�ڷ�����"

		ClearPacketData(m_pTLBTop);
		ClearPacketData(m_pTLBBottom);
		ClearPacketData(m_pTLBDate);
	}
	else if(calculatetype == CCalculateBaseData::REALDATA_CAL)
	{
		ClearPacketData(m_pTLBTop);
		ClearPacketData(m_pTLBBottom);
		ClearPacketData(m_pTLBDate);
	}
	return (m_pTLBTop != NULL  && m_pTLBBottom != NULL  && m_pTLBDate != NULL );
}

void CPF::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pTLBTop);
	ClearPacketData(m_pTLBBottom);
	ClearPacketData(m_pTLBDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}


void CPF::GetPF(const CPoint &point, const double nMin, const double nMax, CList<double, double>* pConditionList,
				CList<double, double>* pClosePriceList, CList<double, double>* pDateList,
				CPacket* pTOPPacket, CPacket* pBOTTOMPacket, CPacket* pDATEPacket,
				CList<double, double>* pDrawConditionList,BOOL bReal,BOOL bDataType, CPacketList* pPacketList)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return ;

//	2006.08.14 ��ĭ�� ���� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	double dUnitSize;
	POSITION pos = pDrawConditionList->GetHeadPosition();
	if( !pos) return;
	double nIndex = pDrawConditionList->GetAt(pos);
	// (2007/6/19 - Seung-Won, Bae) Support Only Draw Condition of P&F for User Setting.
	if( nIndex) //  || bReal ||!bDataType
		dUnitSize = GetCondition(1, pConditionList);
	else
	{
		// (2008/11/2 - Seung-Won, Bae) Use HBPRICE.
		dUnitSize = (nMax - nMin) / 50;
		CString strTickValue;
		if( pPacketList->GetHelpMessageItem( "HBPRICE", strTickValue))
		{
			double dTickValue = atof( strTickValue);
			if( dUnitSize < dTickValue) dUnitSize = dTickValue;
		}

		pConditionList->SetAt(pConditionList->FindIndex(1),dUnitSize);
	}
//	if(dUnitSize <= 0)
//		dUnitSize = (nMax - nMin) / 50;
//	2006.08.14 ��ĭ�� ���� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 

	if(dUnitSize == 0.00)
		return;

	double dBaseData = GetAt(pClosePriceList, 0);

	int startpoint;
	double SecondData;

	bool UPCheck = true;
//	2006.08.14 ��ĭ�� ���� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
//	int nRet = 3;
	int nRet = (int)GetCondition(0, pConditionList);
//	2006.08.14 ��ĭ�� ���� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	double conversionValue = 0;

	GetSecondDataandStartPoint(pClosePriceList, dUnitSize, startpoint, SecondData);

	if(SecondData >= dBaseData + dUnitSize)
		UPCheck = true;
	else if(SecondData <= dBaseData - dUnitSize)
		UPCheck = false;

	pTOPPacket->AppendData(dBaseData);

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pClosePriceList) return;

	const int nCnt = pClosePriceList->GetCount();

	POSITION posClose = FindPosition(0, *pClosePriceList);
	POSITION posdate = FindPosition(0, *pDateList);

	for(int count = 0 ; count< nCnt; count++) 
	{
		double dClose = GetNext(pClosePriceList, posClose);
		double dDate = GetNext(pDateList, posdate);

		if(dClose >= dBaseData + dUnitSize && UPCheck == TRUE) //��� �� ���
			WhenUpduringUp(dClose, dDate, dUnitSize, dBaseData, pTOPPacket, pBOTTOMPacket, pDATEPacket);
		else if(dClose >= dBaseData + dUnitSize && UPCheck == FALSE)  //�϶��� ���
			WhenUpduringDown(nRet, dClose, dDate, dUnitSize, UPCheck, dBaseData, conversionValue, pTOPPacket, pBOTTOMPacket, pDATEPacket);
		else if(dClose <= dBaseData - dUnitSize && UPCheck == FALSE)  //�϶��� �϶�
			WhenDownduringDown(dClose, dDate, dUnitSize, dBaseData, pTOPPacket, pBOTTOMPacket, pDATEPacket);
		else if(dClose <= dBaseData - dUnitSize && UPCheck == TRUE)  //����� �϶�
			WhenDownduringUp(nRet, dClose, dDate, dUnitSize, UPCheck, dBaseData, conversionValue, pTOPPacket, pBOTTOMPacket, pDATEPacket);
		else
			conversionValue = 0;
	}
}
double CPF::GetUnitValue(const double nMin, const double nMax, CList<double, double>* pConditionList)
{
	if(pConditionList == NULL || pConditionList->GetCount() < 2)
		return (nMax - nMin) / 50.0;

	// ù��° ���ǰ� -> value
	// �ι�° ���ǰ� -> ��� ���(%, ��ġ)
	double dCondition0 = GetCondition(0, pConditionList);
	double dCondition1 = GetCondition(1, pConditionList);
	if(dCondition1 == 1)	//�ۼ�Ʈ
		return ((nMax - nMin) * (dCondition0 / 100));
	else if(dCondition1 == 2)	//�ܼ� ��ġ
		return dCondition0;
	
	return -1;
}
void CPF::GetSecondDataandStartPoint(CList<double, double>* pClosePriceList,
									 const double dUnitSize,
									 int& startpoint,
									 double& seconddata)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pClosePriceList) return;

	startpoint = 1;

	POSITION pos = FindPosition(0, *pClosePriceList);
	double first = GetNext(pClosePriceList, pos);

	for(int i = 1 ; i < pClosePriceList->GetCount() ; i++) {
		double second = GetNext(pClosePriceList, pos);
		if(first == second || !((second > first + dUnitSize) || (second < first - dUnitSize)) ) 
			startpoint++;
		else 
		{
			seconddata = second;
			break;
		}
	}
}


void CPF::WhenUpduringUp(const double& dClose, const double& dDate, const double dUnitSize,
						 double& dBaseData, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						 CPacket* pDATEPacket)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return;

	// 20080916 ������ pTOPList�� ����Ÿ�� ���� ������ ��� ó������ �ʴ´� => �����������ϴ� ���� ó��
	if(pTOPList->GetCount() == 0 )
		return;

	int nUnitSize = (int)(dUnitSize * 10000);
	// 20080916 ������	0���� �����ų� �������� ���ϸ� ������ �����Ѵ�...
	//if(nUnitSize == 0)
	//	return ;
	if( nUnitSize == 0 )		nUnitSize = 1;

	if(pBOTTOMPacket->GetCount() > 0 )  {//��� ������϶��� ������ ��� ����ǰ�  ������ ������ ����.
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->UpdateData(dClose, pBOTTOMList->GetCount() - 1, false);
		else {
			if(data < (nUnitSize / 2 * 0.0001)) 
				pBOTTOMPacket->UpdateData(dClose - data, pBOTTOMList->GetCount() - 1, false);
			else 
				pBOTTOMPacket->UpdateData(dClose + (dUnitSize - data), pBOTTOMList->GetCount() - 1, false);
		}
		pDATEPacket->UpdateData(dDate, pBOTTOMList->GetCount() - 1, false);
	}
	else {
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize)* 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->AppendData(dClose);
		else {
			if(data < (nUnitSize / 2 * 0.0001))
				pBOTTOMPacket->AppendData(dClose - data);
			else
				pBOTTOMPacket->AppendData(dClose + (dUnitSize - data));
		}
		pDATEPacket->AppendData(dDate);
	}
	dBaseData = dClose;
}

void CPF::WhenUpduringDown(const int& nRet, const double& dClose, const double& dDate,
						   const double dUnitSize, bool& UPCheck, double& dBaseData,
						   double& conversionValue, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						   CPacket* pDATEPacket)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return ;
	// 20080916 ������ pTOPList�� ����Ÿ�� ���� ������ ��� ó������ �ʴ´� => �����������ϴ� ���� ó��
	if(pTOPList->GetCount() == 0 )
		return;

	int nUnitSize = (int)(dUnitSize * 10000);
	// 20080916 ������	0���� �����ų� �������� ���ϸ� ������ �����Ѵ�...
	if( nUnitSize == 0 )		nUnitSize = 1;

	conversionValue = (dClose - dBaseData) / dUnitSize;
	// 20080916 ������ pBOTTOMList�� ����Ÿ�� ���� ������ ��� ó������ �ʴ´� => �����������ϴ� ���� ó��
//	if(conversionValue >= nRet) //3ĭ �̻� ����̳� �϶��� �Ǿ�����.
	if(conversionValue >= nRet && pBOTTOMPacket->GetCount() > 0) //3ĭ �̻� ����̳� �϶��� �Ǿ�����.
	{ 
		pTOPPacket->AppendData(pBOTTOMList->GetTail() + dUnitSize);

		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->AppendData(dClose);
		else {
			if(data < (nUnitSize / 2 * 0.0001))
				pBOTTOMPacket->AppendData(dClose - data);
			else 
				pBOTTOMPacket->AppendData(dClose + (dUnitSize - data));	
		}
		pDATEPacket->AppendData(dDate);
		UPCheck = TRUE;
		dBaseData = dClose;
	}
	else
		conversionValue = 0;

}

//���� unitsize�� 5,840 �̰�. top�� 224260 close�� 218000�̶��
//bottom���� 206740�� ���� �Ѵ�. 
//��ĭ���� ������ ��ŭ�� ����Ÿ�� ���� �Ѵ�.
//�̷��� ������� 3���� O�� �׸��� �ִ�.
//������ŭ ����Ÿ�� �ð� �پ�� �Ѵ�.
void CPF::WhenDownduringDown(const double& dClose, const double& dDate, const double dUnitSize, 
							 double& dBaseData, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
							 CPacket* pDATEPacket)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return ;

	// 20080916 ������ pTOPList�� ����Ÿ�� ���� ������ ��� ó������ �ʴ´� => �����������ϴ� ���� ó��
	if(pTOPList->GetCount() == 0 )
		return;

	int nUnitSize = (int)(dUnitSize * 10000);
	// 20080916 ������	0���� �����ų� �������� ���ϸ� ������ �����Ѵ�...
	if( nUnitSize == 0 )		nUnitSize = 1;
	if( nUnitSize <= 0) return;
	if(pBOTTOMList->GetCount() > 0 ) { //��� ������϶��� ������ ��� ����ǰ�  ������ ������ ����.
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->UpdateData(dClose, pBOTTOMList->GetCount() - 1, false);
		else {
			if(data < (nUnitSize / 2 * 0.0001)) 
				pBOTTOMPacket->UpdateData(dClose + data, pBOTTOMList->GetCount() - 1, false);
			else 
				pBOTTOMPacket->UpdateData(dClose - (dUnitSize - data), pBOTTOMList->GetCount() - 1, false);
		}
		pDATEPacket->UpdateData(dDate, pDATEList->GetCount() - 1, false);
	}
	else {
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->AppendData(dClose);
		else {
			if(data < (nUnitSize / 2 * 0.0001))
				pBOTTOMPacket->AppendData(dClose + data);
			else
				pBOTTOMPacket->AppendData(dClose - (dUnitSize - data));
		}
		pDATEPacket->AppendData(dDate);
	}

	dBaseData = dClose;

}


void CPF::WhenDownduringUp(const int& nRet, const double& dClose, const double& dDate,
						   const double dUnitSize, bool& UPCheck, double& dBaseData,
						   double& conversionValue, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						   CPacket* pDATEPacket)
{
	if(pTOPPacket == NULL || pBOTTOMPacket == NULL || pDATEPacket == NULL)
		return ;
	CList<double, double>* pTOPList = pTOPPacket->GetnumericDataList();
	CList<double, double>* pBOTTOMList = pBOTTOMPacket->GetnumericDataList();
	CList<double, double>* pDATEList = pDATEPacket->GetnumericDataList();
	if(pTOPList == NULL || pBOTTOMList == NULL || pDATEPacket == NULL)
		return;

	// 20080916 ������ pTOPList�� ����Ÿ�� ���� ������ ��� ó������ �ʴ´� => �����������ϴ� ���� ó��
	if(pTOPList->GetCount() == 0 )
		return;

	// (2009/9/8 - Seung-Won, Bae) Check Empty Data.
	if( pBOTTOMList->IsEmpty()) return;

	int nUnitSize = (int)(dUnitSize * 10000);
	// 20080916 ������	0���� �����ų� �������� ���ϸ� ������ �����Ѵ�...
	if( nUnitSize == 0 )		nUnitSize = 1;
	conversionValue = (dBaseData - dClose) / dUnitSize;
	// 20080916 ������ pBOTTOMList�� ����Ÿ�� ���� ������ ��� ó������ �ʴ´� => �����������ϴ� ���� ó��
//	if(conversionValue >= nRet) //3ĭ �̻� ����̳� �϶��� �Ǿ�����.
	if(conversionValue >= nRet && pBOTTOMPacket->GetCount() > 0) //3ĭ �̻� ����̳� �϶��� �Ǿ�����.
	{
		pTOPPacket->AppendData(pBOTTOMList->GetTail() - dUnitSize);
		double data = abs(CMath::Round((dClose - pTOPList->GetTail()) * 10000) % nUnitSize) * 0.0001;
		if(data == 0.0)
			pBOTTOMPacket->AppendData(dClose);
		else {
			if(data < (nUnitSize / 2 * 0.0001))
				pBOTTOMPacket->AppendData(dClose + data);
			else
				pBOTTOMPacket->AppendData(dClose - (dUnitSize - data));
		}
		pDATEPacket->AppendData(dDate);
		UPCheck = FALSE;
		dBaseData = dClose;
	}
	else
		conversionValue = 0;

}


//******************************************************************************************************************
//	Rainbow 
//	  
//******************************************************************************************************************
CRainbow::CRainbow() :
	m_pEndPriceList(NULL),
	m_pEndPricePacket(NULL),
	CIndicator( IMetaTable::EDDC_OCX_RAINBOWCHART)
{
}
CRainbow::~CRainbow()
{
	// (2006/12/12 - Seung-Won, Bae) Release List Chain.
	m_RainbowList.RemoveAll();
}
void CRainbow::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		m_pEndPricePacket = GetPacket(calculateData.GetPoint(), chartFormulateData);
		m_pEndPriceList = GetnumericDataList(m_pEndPricePacket);
	}
	if(!IsValidPacket(m_pEndPriceList))
		return;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pIndicatorInfo) return;

	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dFirstCondition  = GetCondition(0, pConditionList);
	double dIntervalofMV  = GetCondition(1, pConditionList);
	double dPacketCount = GetCondition(2, pConditionList);

	if( !GetSubGraphPacket(chartFormulateData.GetCalculateType(), dPacketCount, pPacketList, pIndicatorInfo)) return;
	if( !SetGraphPacketList(chartFormulateData, pGraphCalculateData)) return;
	
	if(IsIndicatorCalculated(chartFormulateData.GetCalculateType()))
		return;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pEndPricePacket) return;

	GetRainbow(calculateData.GetPoint(), dFirstCondition, dIntervalofMV, dPacketCount, 
			   m_pEndPricePacket->GetType(false), m_pEndPriceList, pPacketList, chartFormulateData.GetChartMode());
}

//�̹� ����� �Ǿ� ����.
//object���� build�� �ٽ� ������.������ �ִ� packet�� �ּҸ� ��� �Ҿ�����ϱ�..
//�ٽ� ������ ��. �׸���, ����� �ٽ� �� �ʿ� ����.
bool CRainbow::IsIndicatorCalculated(const CCalculateBaseData::CALCULATETYPE calculatetype)
{
	if(calculatetype == CCalculateBaseData::TRDATA_CAL)
	{
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( m_RainbowList.IsEmpty()) return FALSE;
		CPacket *pPacket = m_RainbowList.GetHead();
		if( !pPacket) return FALSE;

		if( pPacket->GetCount() > 0)
			return true;
	}
	return false;
}

bool CRainbow::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, const double dPacketCount, CPacketList* pPacketList,
								 CIndicatorInfo* pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		//20���� �׹��� �ִٰ� 5���� �پ����� �����ϱ� ����Ʈ�� �ִ� ��� ��� ����
		m_RainbowList.RemoveAll();
		for(int count = 0 ; count < dPacketCount ; count++) 
		{
			CString strSubgraphName;
			strSubgraphName.Format("%s%s%d", strSubGraphPacketNameHeader, _MTEXT( C2_RAINBOW_CHART), count + 1);//"�׹���Ʈ"
			
			CPacket* pRainbow = pPacketList->GetBaseData( strSubgraphName, false);
			ClearPacketData(pRainbow);
			m_RainbowList.AddTail(pRainbow);
		}
	}
	return (m_RainbowList.GetCount() > 0);
}

//���� subgraphname�� �Ⱦ���. 
bool CRainbow::SetGraphPacketList(const CChartFormulateData& chartFormulateData,
								  CGraphCalculateData* pGraphCalculateData)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL) 
		{
			POSITION pos = m_RainbowList.GetHeadPosition();
			while(pos)
				pSubGraphList->AddTail2( m_RainbowList.GetNext(pos));
		}
		return (pSubGraphList->GetCount() > 0);
	}
	return false;
}

void CRainbow::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									    CGraphCalculateData* pGraphCalculateData,
										CIndicatorInfo* pIndicatorInfo)
{
	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

void CRainbow::GetRainbow(const CPoint& point,
						  const double dFirstCondition, 
						  const double dIntervalofMV, 
						  const double dPacketCount, 
						  const CString& strType,
						  CList<double, double>* pEndPriceList, 
						  CPacketList* pPacketList,
						  CChartInfo::CHART_MODE p_eChartMode)
{
	POSITION pos = m_RainbowList.GetHeadPosition();
	for(int count = 0 ; count < dPacketCount ; count++) 
	{
		if(pos == NULL)
			return ;
		double dCondition = dFirstCondition + (count * dIntervalofMV);

		CPacket* pRainbowPacket = m_RainbowList.GetNext(pos);

		// (2009/9/19 - Seung-Won, Bae) for FX/WorldOn Index Digit.
		CString strValueType( TYPE_TWODECIMAL);
		if( CChartInfo::HTS != p_eChartMode)
			strValueType = strType;

		SetPacketTypes( pRainbowPacket, strValueType, strType, (int)dCondition - 1);
		GetSimpleMA(point, dCondition, pEndPriceList, pRainbowPacket, TRUE);
	}
}


//******************************************************************************************************************
//	CRelative_Strength
//	��) 1999.10.10 ~ 2000.01.01
// 10.10 �� ����Ÿ�� 90�̿���.
// �� ����Ÿ�� 100���� ���� ��� ����Ÿ�� 0.9�� ������.  
//////
// �������� / (ù��° data / dCondition).
//******************************************************************************************************************

CRelative_Strength::CRelative_Strength() : 
	m_pRS(NULL),
	m_pEndPriceList(NULL),
	m_pEndPricePacket(NULL),
	CIndicator( IMetaTable::EDDC_OCX_COMPARATIVECHART)
{
}

void CRelative_Strength::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;

	CString strPacketNames = CDataConversion::GetStringData(chartFormulateData.GetPacketNames(), ",");
	if(strPacketNames.IsEmpty())
		strPacketNames = chartFormulateData.GetPacketNames();

	// (2006/12/11 - Seung-Won, Bae) With Multiple-Same Indciator.
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		m_pEndPricePacket = pPacketList->GetBaseData(strPacketNames);
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !m_pEndPricePacket) return;
		m_pEndPriceList = m_pEndPricePacket->GetnumericDataList();

		CString strSubGraphName = CDataConversion::GetStringData(chartFormulateData.GetSubGraphNames(), ",");
		if( !GetSubGraphPacket(chartFormulateData.GetCalculateType(), strPacketNames + strSubGraphName, pPacketList, pIndicatorInfo)) return;
		if( !SetGraphPacketList(strSubGraphName, pPacketList, pGraphCalculateData, pIndicatorInfo))return;
	}
	if( !IsValidPacket(m_pEndPriceList)) return;

	if(IsIndicatorCalculated(chartFormulateData.GetCalculateType(), m_pRS))
		return;

	//sy test
	double dCond = 0.0;
	if(!GetCondition(pIndicatorInfo, dCond))
		return;

	SetPacketTypes(dCond);
	GetRSC( calculateData.GetPoint(), dCond, m_pEndPricePacket, m_pRS);

	//double dMin = 0, dMax = 0;
	//m_pEndPricePacket->GetMinMax(0, m_pEndPricePacket->GetCount() - 1, dMin, dMax);
	//GetRSC(calculateData.GetPoint(), dCond, nMin, nMax, m_pEndPriceList, m_pRS);	
}

void CRelative_Strength::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
											CGraphCalculateData* pGraphCalculateData,
											CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pRS);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

bool CRelative_Strength::SetGraphPacketList(const CString& strSubGraphName,
											CPacketList* pPacketList,
											CGraphCalculateData* pGraphCalculateData,
											CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	if( !posSubGraph) return false;

	CSubGraphPacketList *pSubGraphList = pGraphCalculateData->GetNext( posSubGraph);
	if( pSubGraphList == NULL) return false;

	CPacket *pPacket = pSubGraphList->GetHeadPacket();
	if( !pPacket && m_pRS) pSubGraphList->AddTail2( m_pRS);
	return true;
}

bool CRelative_Strength::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, const CString& strSubName, CPacketList* pPacketList,
										   CIndicatorInfo* pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		m_pRS = pPacketList->GetBaseData( strSubGraphPacketNameHeader + strSubName, false);
		ClearPacketData( m_pRS);
	}
// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
//	else if(calculatetype == CCalculateBaseData::REALDATA_CAL)
//	{
//		m_pRS = pPacketList->GetBaseData(strSubName, true);
//	}
	return ( m_pRS != NULL);
}

void CRelative_Strength::GetRSC(const CPoint &point, const double dCondition,
								const double nMin, const double nMax,
								CList<double, double>* pClosePriceList,
								CPacket* pRSC)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pClosePriceList || !pRSC) return;

	int nCnt = pClosePriceList->GetCount();
	if(nMin == 0 && nMax == 0)
	{ //data�� ��� 0�϶� ��� data�� 100%��.
		for(int count = point.x;  count < nCnt; count++) 
		{
			//sy 2006.06.01.
			//pRSC->AppendData(100);
			pRSC->AppendData(dCondition);
		}
		return;
	}

	int startpoint = point.x;
	if(point.x == 0) 
	{
		startpoint += 1;
		//sy 2006.06.01.
		//pRSC->AppendData(100);
		pRSC->AppendData(dCondition);
	}
	//nami 7.30
	double dFirstData = GetAt(pClosePriceList, 0);

	POSITION posStart = FindPosition(startpoint, *pClosePriceList);
	for(int count = startpoint;  count < nCnt; count++) {
		if(posStart == NULL) return ;
		if(dFirstData == 0)
			dFirstData = 1;

		SetData(point, count, (GetNext(pClosePriceList, posStart) / dFirstData ) * dCondition, pRSC);
	}
}

//sy test
bool CRelative_Strength::GetCondition(const CIndicatorInfo* pIndicatorInfo, double& dCond) const
{
	if(pIndicatorInfo == NULL)
		return false;

	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	if(pConditionList == NULL)
		return false;

	dCond = pConditionList->GetHead();
	return true;
}

void CRelative_Strength::GetRSC(const CPoint& point, const double& dCond, const CPacket* pClosePrice, CPacket* pRSC)
{
	if(pClosePrice == NULL || pRSC == NULL)
		return;

	CList<double, double>* pClosePriceDataList = NULL;
	int nCount = 0;
	double dMin = 0.0, dMax = 0.0, dFirstData;
	if(!GetEnvironmentDataForCalRSC(pClosePrice, pClosePriceDataList, nCount, dMin, dMax, dFirstData))
		return;

	if(InitializeRSC(pRSC, point.x, nCount, dCond, dMin, dMax))
		return;

	// ���񱳰��
	POSITION pos = CIndicator::FindPosition(point.x, *pClosePriceDataList);
	for(int i = point.x; i < nCount; i++)
	{
		if(pos == NULL)
			break;

		double dRSCData = GetRSCData(dFirstData, pClosePriceDataList->GetNext(pos), dCond);
		CIndicator::SetData(point, i, dRSCData, pRSC);
	}
}

double CRelative_Strength::GetRSCData(const double& dFirstData, const double& dPriceData, const double& dCond) const
{
	double dRSCData = (dPriceData - dFirstData)/dFirstData;
	// dCond : ���� ���� ��
	dRSCData = dCond + (dRSCData * 100);
	return dRSCData;
}

bool CRelative_Strength::GetEnvironmentDataForCalRSC(const CPacket* pClosePrice, 
		CList<double, double>*& pClosePriceDataList, int& nCount, double& dMin, double& dMax, double& dFirstData) const
{
	if(pClosePrice == NULL)
		return false;

	pClosePriceDataList = const_cast<CPacket*>(pClosePrice)->GetnumericDataList();
	if(pClosePriceDataList == NULL)
		return false;

	nCount = pClosePriceDataList->GetCount();
	if(nCount <= 0)
		return false;

	const_cast<CPacket*>(pClosePrice)->GetMinMax(0, (nCount - 1), dMin, dMax);
	dFirstData = pClosePriceDataList->GetHead();
	if(dFirstData == 0.0)
		dFirstData = 1.0;

	return true;
}

void CRelative_Strength::SetPacketTypes(const double& dCond)
{
	if( dCond == 100.0)
	{
		CIndicator::SetPacketTypes( m_pRS, TYPE_TWODECIMAL, TYPE_INTERGER_PERCENT, 0);
	}
	else
	{
		CIndicator::SetPacketTypes( m_pRS, TYPE_TWODECIMAL, TYPE_TWODECIMAL, 0);
	}
}

bool CRelative_Strength::InitializeRSC(CPacket* pRSC, const int nStartIndex, const int nCount, const double& dCond, const double& dMin, const double& dMax)
{
	if(pRSC == NULL || dMin != 0.0 || dMax != 0.0)
		return false;

	bool bResult = false;
	//data�� ��� 0�϶� : ��� data�� ���ǰ����� �ִ´� (0 or 100%)
	for(int i = nStartIndex; i < nCount; i++)
	{
		pRSC->AppendData(dCond);
		bResult = true;
	}
	
	return bResult;
}
//sy end

/******************************************************************************************************************
* Chart Name : CRenKoChart 
* Date       : 2005.06.23. 	
* Made By    : Yoo. Je Sun
* ��ǥ����   : �������� �ʰ��ϴ� ���ݺ�ȭ(�ּ���ȯ����, ����ũ��)�� �߻��� ����(Bricks)�� �׸�
*              ������ ũ��� ������
*******************************************************************************************************************/
CRenKoChart::CRenKoChart() : 
	m_pClosePrice(NULL), 
	m_pDate(NULL), 
	m_pRenKoHighPrice(NULL),
	m_pRenKoLowPrice(NULL),
	m_pRenKoDate(NULL),
	CIndicator( IMetaTable::EDDC_OCX_RENKOCHART)
{
}

void CRenKoChart::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(!pPacketList || !pIndicatorInfo)
		return;
	
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		//TR ����϶��� ���� ��ĭ���� �ʱ�ȭ �Ѵ�.
		// ���ǰ� ����� ���� ���� - ojtaso (20070913)
		//	(2006.08.14, �Ƚ�ȯ) ��ĭ�� ���� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
		CList<double, double>* pDrawConditionList = pIndicatorInfo->GetDrawConditionList();
		if( pDrawConditionList)
		{
			//	2006.08.14 ��ĭ�� ���� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
			POSITION pos = pDrawConditionList->GetHeadPosition();
			if( !pos) return;
			double nIndex = pDrawConditionList->GetAt(pos);
			// (2007/6/19 - Seung-Won, Bae) Support Only Draw Condition of P&F for User Setting.
			if( 0 == nIndex) pIndicatorInfo->SetCommonCalConditionData(0, 0);
		}
		//�ڷ����ڿ� ������Ŷ�������� (IndiCond.cfg�� ����)
		CString strPacketNames = chartFormulateData.GetPacketNames();
		m_pClosePrice = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		m_pDate = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
	}

	// ����Ÿ���� ��ȿ������ �˻��Ѵ�.
	if(!IsValidPacket(m_pClosePrice) || !IsValidPacket(m_pDate))
		return;

	CList<double, double>* pClosePriceList = CIndicator::GetnumericDataList(m_pClosePrice); // ���� ����Ʈ
	CList<double, double>* pDateList = CIndicator::GetnumericDataList(m_pDate); // �ڷ����� ����Ʈ
	// ����Ÿ���� ��ȿ������ �˻��Ѵ�.
	if(!IsValidPacket(pClosePriceList) || !IsValidPacket(pDateList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

	double dBlockCondition = GetBrickSize( pIndicatorInfo, m_pClosePrice, pPacketList, chartFormulateData.GetChartMode());
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		//TR ����϶��� info�� ��ĭ���� �����Ѵ�.
		pIndicatorInfo->SetCommonCalConditionData(0, dBlockCondition);
		
		// packet type ����.
		CString strPriceType = m_pClosePrice->GetType(false);
		CString strDateType = m_pDate->GetType(false);
		SetPacketTypes(m_pRenKoHighPrice, strPriceType, strPriceType, 0);
		SetPacketTypes(m_pRenKoLowPrice, strPriceType, strPriceType, 0);
		SetPacketTypes(m_pRenKoDate, strDateType, strDateType, 0);
	}

	GetRenKoChart(calculateData.GetPoint(), dBlockCondition, pClosePriceList, pDateList);
}

bool CRenKoChart::SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo)
{
	// NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if(!pGraphCalculateData)
		return false;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	
	// ��� ������ Clear
	ClearPacketData(m_pRenKoHighPrice);
	ClearPacketData(m_pRenKoLowPrice);
	ClearPacketData(m_pRenKoDate);

	return (m_pRenKoHighPrice != NULL && m_pRenKoLowPrice != NULL && m_pRenKoDate != NULL);
}

bool CRenKoChart::InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;
	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;
	if(!pSubGraphList)
		return false;

	m_pRenKoHighPrice	= pPacketList->GetBaseData( _MTEXT( C0_RENKO_CHART_OPEN),		false);
	m_pRenKoLowPrice	= pPacketList->GetBaseData( _MTEXT( C0_RENKO_CHART_CLOSE),		false);
	m_pRenKoDate		= pPacketList->GetBaseData( _MTEXT( C0_RENKO_CHART_DATETIME),	false);

	pSubGraphList->AddTail2( m_pRenKoHighPrice);
	pSubGraphList->AddTail2( m_pRenKoLowPrice);
	pSubGraphList->AddTail2( m_pRenKoDate);
	return (m_pRenKoHighPrice != NULL || m_pRenKoLowPrice != NULL || m_pRenKoDate != NULL);
}
					
//���߿� �ǽð� ó�� Ȯ��
//�ǽð� ���ö��� ÷���� �ٽ� ����ؾ� �Ǵϱ�...data�� ���� �����ش�.
bool CRenKoChart::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList)
{
	if(!pPacketList)
		return false;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL)
	{
		m_pRenKoHighPrice = pPacketList->GetBaseData(	_MTEXT( C0_RENKO_CHART_OPEN),		false);
		m_pRenKoLowPrice = pPacketList->GetBaseData(	_MTEXT( C0_RENKO_CHART_CLOSE),		false);
		m_pRenKoDate = pPacketList->GetBaseData(		_MTEXT( C0_RENKO_CHART_DATETIME),	false);

		ClearPacketData(m_pRenKoHighPrice);
		ClearPacketData(m_pRenKoLowPrice);
		ClearPacketData(m_pRenKoDate);
	}
	else if(calculatetype == CCalculateBaseData::REALDATA_CAL)
	{
		ClearPacketData(m_pRenKoLowPrice);
		ClearPacketData(m_pRenKoDate);
	}
	return (m_pRenKoHighPrice != NULL && m_pRenKoLowPrice != NULL && m_pRenKoDate != NULL);
}

void CRenKoChart::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
										 CGraphCalculateData* pGraphCalculateData,
										 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pRenKoHighPrice);
	ClearPacketData(m_pRenKoLowPrice);
	ClearPacketData(m_pRenKoDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

void CRenKoChart::GetRenKoChart(const CPoint &point, double dBrickCondition,
								CList<double, double>* pClosePriceList,
								CList<double, double>* pDateList)
{
	// ���� ��Ŷ�� ���°�� �����Ѵ�
	if( !pClosePriceList )
		return;

	UINT nIndex = 0;	// RenKoPacket�� ���Ǵ� Index.
	POSITION posDate		= pDateList->GetHeadPosition();
	POSITION posClosePrice	= pClosePriceList->GetHeadPosition();
	
	int		i=0, nListCount=0;				// nListCount-���� ����Ʈ ����
	double	dBrickType = 0;					// BrickType (0: Down, 1: Up)
	double	dClosePrice = 0;				// ���� ������
	double	dHighPrice = 0, dLowPrice = 0;	// RenKoPacket�� High/Low Price
	double	dDate = 0;						// ���� �ڷ�����
	double	dBrickHigh = 0, dBrickLow = 0;	// ������ High/Low Price

	nListCount = pClosePriceList->GetCount();	// ���� ������ ����
	
	if(nListCount <= 1)	// ���� �����Ͱ� 1������ ��� ��� �ʿ����
		return;

	dClosePrice = pClosePriceList->GetNext(posClosePrice);	// ù ����
	dDate = pDateList->GetNext(posDate);				  // ù ���� ������
	dHighPrice = dLowPrice = 0;				// ������ ����
	dBrickHigh = dBrickLow = dClosePrice;

	CList<double, double>* pBrickTypeList = NULL;
	pBrickTypeList = m_pRenKoHighPrice->GetTempList();

	for(i=0; i < nListCount -1; i++)
	{
		while( dClosePrice >= dBrickHigh + dBrickCondition )
		{
			/* Up Type Brick ���� */
			dBrickType = 1;
			dHighPrice = dBrickHigh + dBrickCondition;
			dLowPrice = dBrickHigh;

			// A HighPrice and a BrickType add to m_pRenKoHighPrice
			SetData(point, nIndex, dHighPrice, m_pRenKoHighPrice);
			m_pRenKoHighPrice->SetTempListData(nIndex, dBrickType, true);

			// LowPrice add to m_pRenKoLowPrice
			SetData(point, nIndex, dLowPrice, m_pRenKoLowPrice);

			// dDate add to m_pRenKoDate
			SetData(point, nIndex, dDate, m_pRenKoDate);
			nIndex++;
			
			dBrickHigh = dHighPrice; /* ReSetting BrickData */
			dBrickLow = dLowPrice;
		}
		while( dClosePrice <= dBrickLow - dBrickCondition )
		{
			/* Down Type Brick ���� */
			dBrickType = 0;
			dHighPrice = dBrickLow;
			dLowPrice = dBrickLow - dBrickCondition;

			// HighPrice add to m_pRenKoHighPrice 
			SetData(point, nIndex, dHighPrice, m_pRenKoHighPrice);
			m_pRenKoHighPrice->SetTempListData(nIndex, dBrickType, true);

			// LowPrice add to m_pRenKoLowPrice
			SetData(point, nIndex, dLowPrice, m_pRenKoLowPrice);

			// dDate add to m_pRenKoDate
			SetData(point, nIndex, dDate, m_pRenKoDate);
			nIndex++;
			// ReSetting BrickData
			dBrickHigh = dHighPrice;
			dBrickLow = dLowPrice;
		}
		dClosePrice = pClosePriceList->GetNext(posClosePrice);	// Next ����
		dDate = pDateList->GetNext(posDate);				  // Next �ڷ�����
	}
}

// ������ ����� ���񺰷� �������� �Լ�
// -> ���� ������ 0�� ��� ������ 50���� ����. (���� �ʿ�)
//	  �⺻ ������ ũ��
//	  + �ֽ� : ȣ������ * 10
//	  + ����/����/�ɼ� : 5-Point
double CRenKoChart::GetBrickSize( CIndicatorInfo* pIndicatorInfo, CPacket* pClosePacket, CPacketList* pPacketList, CChartInfo::CHART_MODE p_eChartMode) const
{
	// ���Ǽ���
	double dBrickSize = GetBrickSize(pIndicatorInfo);
	if(dBrickSize > 0)
		return dBrickSize;

	// (2008/10/27 - Seung-Won, Bae) Do not use asked price in world-on.
	if( CChartInfo::WORLD_ON != p_eChartMode)
	{
		// ȣ������
		dBrickSize = GetBrickSize(pClosePacket);
		if(dBrickSize > 0)
			return dBrickSize;
	}

	// �ְ� ������ �������� 2%.
	dBrickSize = GetBrickSize_Price(pClosePacket, pPacketList, p_eChartMode);

	// (2008/10/27 - Seung-Won, Bae) Do not use asked price in world-on.
	if( CChartInfo::WORLD_ON == p_eChartMode) return dBrickSize;

	return (dBrickSize > 0 ? dBrickSize: 1);
}

double CRenKoChart::GetBrickSize(CIndicatorInfo* pIndicatorInfo) const
{
	// ���Ǽ���
	if(pIndicatorInfo == NULL)
		return 0;

	return GetCondition(0, pIndicatorInfo->GetCommonCalConditionList());
}

double CRenKoChart::GetBrickSize(CPacket* pClosePacket) const
{
	// ȣ������
	if(pClosePacket == NULL)
		return 0;

	switch(pClosePacket->GetCommodityBondName()) 
	{
	case CPacketType::NONAME:
	case CPacketType::STOCK:	//�ֽ�
		return GetBrickSize_Stock(pClosePacket);
	case CPacketType::INDUSTRY:	//����
	case CPacketType::FUTURES:	//����
	case CPacketType::OPTION:	//�ɼ�
		return 5;
	}

	return 0;
}

double CRenKoChart::GetBrickSize_Stock(CPacket* pClosePacket) const
{
	// ȣ������ -> �ֽ�
	if(pClosePacket == NULL)
		return 0;

	CList<double, double>* pCloseDataList = pClosePacket->GetnumericDataList();
	// ���Ǽ����� ���� ��� -> "������ ����"�� ���� ����
	if(pCloseDataList == NULL || pCloseDataList->GetTailPosition() == NULL)
		return 0;

	double dBrickSize = 0;
	// ������ ���� ȣ�� �ݿ��� ���� ����
	double dClosePrice = pCloseDataList->GetTail();
	if(dClosePrice >= 500000)		// 50���� �̻�
		dBrickSize = 1000;
	else if(dClosePrice >= 100000)	// 10���� �̻�
		dBrickSize = 500;
	else if(dClosePrice >= 50000)	// 5���� �̻�
		dBrickSize = 100;
	else if(dClosePrice >= 10000)	// 1���� �̻�
		dBrickSize = 50;
	else if(dClosePrice >= 5000)	// 5õ�� �̻�
		dBrickSize = 10;
	else							// 5õ�� �̸�
		dBrickSize = 5;
	
	return (dBrickSize * 10);
}

double CRenKoChart::GetBrickSize_Price(CPacket* pClosePacket, CPacketList* pPacketList, CChartInfo::CHART_MODE p_eChartMode) const
{
	if( !pClosePacket || !pPacketList) return 0;

	// �ְ� ������ �������� 2%.
	double dMin = 0.00, dMax = 0.00;
	pClosePacket->GetMinMax(0, pClosePacket->GetCount() - 1, dMin, dMax);
	double dBrickSize = (dMax - dMin) * 0.02;

	// (2008/11/2 - Seung-Won, Bae) Use HBPRICE.
	CString strTickValue;
	if( pPacketList->GetHelpMessageItem( "HBPRICE", strTickValue))
	{
		double dTickValue = atof( strTickValue);
		if( dTickValue < dBrickSize) dTickValue = dBrickSize;
		return dTickValue;
	}

	// (2008/10/27 - Seung-Won, Bae) Do not use asked price in world-on.
	if( CChartInfo::WORLD_ON != p_eChartMode)
	{
		if(dBrickSize > 100000) // 10���� ����
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 5);
		else if(dBrickSize > 10000) // 1���� ����
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 4);
		else if(dBrickSize > 1000) // 1õ�� ����
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 3);
		else if(dBrickSize > 100) // 100�� ����
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 2);
		else if(dBrickSize > 10) // 10�� ����
			dBrickSize = CMath::Round(CMath::Round(dBrickSize), 1);
		else if(dBrickSize > 5) // 5�� ����
			dBrickSize = 5;
		return (dBrickSize > 1 ? dBrickSize : 1);
	}
	return dBrickSize;
}


///******************************************************************************************************************
//	CThreeLineBreak
//	  
///******************************************************************************************************************
CThreeLineBreak::CThreeLineBreak() : 
	m_pTLBTop(NULL), 
	m_pTLBBottom(NULL), 
	m_pTLBDate(NULL), 
	m_pEndPriceList(NULL),
	m_pDateList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_THREELINEBRACKCHART)
{
}

void CThreeLineBreak::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;

//	(2006.09.07,�Ƚ�ȯ) �Ｑ��ȯ�� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
//	(2006.09.07,�Ƚ�ȯ) �Ｑ��ȯ�� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		CString strPacketNames = chartFormulateData.GetPacketNames();
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		CPacket *pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pEndPriceList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));
		if( !pPacket) return;
		m_pDateList = pPacket->GetnumericDataList();
	}
	if(!IsValidPacket(m_pEndPriceList) || !IsValidPacket(m_pDateList))
		return;

	if(!GetSubGraphPacket(chartFormulateData.GetCalculateType(), pPacketList) ||
		!SetGraphPacketList(pPacketList, pGraphCalculateData, pIndicatorInfo))
		return;

//	(2006.09.07,�Ƚ�ȯ) �Ｑ��ȯ�� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
//	GetThreeLineBreak(calculateData.GetPoint(), m_pEndPriceList, m_pDateList, 
//					  m_pTLBTop, m_pTLBBottom, m_pTLBDate);	
	GetThreeLineBreak(calculateData.GetPoint(), m_pEndPriceList, m_pDateList, 
					  m_pTLBTop, m_pTLBBottom, m_pTLBDate,pConditionList);	
//	(2006.09.07,�Ƚ�ȯ) �Ｑ��ȯ�� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	CPacket *pPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));//"����"
	if( !pPacket) return;
	CString strEndPacketType = pPacket->GetType(false);
	pPacket = pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return;
	CString strDatePacketType = pPacket->GetType(false);

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pTLBTop || !m_pTLBBottom || !m_pTLBDate) return;

	SetPacketTypes(m_pTLBTop, strEndPacketType, strEndPacketType, 0);
	SetPacketTypes(m_pTLBBottom, strEndPacketType, strEndPacketType, 0);
	SetPacketTypes(m_pTLBDate, strDatePacketType, strDatePacketType, 0);
}
	
//Ư����
bool CThreeLineBreak::SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pIndiPacket = pSubGraphList->GetHeadPacket();
		if(pIndiPacket == NULL) 
			return InCaseNoPacketPointer(pPacketList, pIndicatorInfo, pSubGraphList);
	}
	return (m_pTLBTop != NULL && m_pTLBBottom != NULL && m_pTLBDate != NULL);
}

//Ư����
bool CThreeLineBreak::InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList)
{
	if(pPacketList == NULL || pIndicatorInfo == NULL)
		return false;

	if(pIndicatorInfo->GetSubGraphCount() <= 0)
		return false;

	m_pTLBTop		= pPacketList->GetBaseData( _MTEXT( C0_THREE_LINE_BREAK_OPEN),		false);
	m_pTLBBottom	= pPacketList->GetBaseData( _MTEXT( C0_THREE_LINE_BREAK_CLOSE),		false);
	m_pTLBDate		= pPacketList->GetBaseData( _MTEXT( C0_THREE_LINE_BREAK_DATE_TIME),	false);

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphList) return FALSE;

	pSubGraphList->AddTail2( m_pTLBTop);
	pSubGraphList->AddTail2( m_pTLBBottom);
	pSubGraphList->AddTail2( m_pTLBDate);
	return (m_pTLBTop != NULL || m_pTLBBottom != NULL || m_pTLBDate != NULL);
}
					
//���߿� �ǽð� ó�� Ȯ��
//�ǽð� ���ö��� ÷���� �ٽ� ����ؾ� �Ǵϱ�...data�� ���� �����ش�.
bool CThreeLineBreak::GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		m_pTLBTop = pPacketList->GetBaseData(		_MTEXT( C0_THREE_LINE_BREAK_OPEN),		false);
		m_pTLBBottom = pPacketList->GetBaseData(	_MTEXT( C0_THREE_LINE_BREAK_CLOSE),		false);
		m_pTLBDate = pPacketList->GetBaseData(		_MTEXT( C0_THREE_LINE_BREAK_DATE_TIME),	false);

		ClearPacketData(m_pTLBTop);
		ClearPacketData(m_pTLBBottom);
		ClearPacketData(m_pTLBDate);
	}
	else if(calculatetype == CCalculateBaseData::REALDATA_CAL)
	{
		ClearPacketData(m_pTLBTop);
		ClearPacketData(m_pTLBBottom);
		ClearPacketData(m_pTLBDate);
	}
	return (m_pTLBTop != NULL && m_pTLBBottom != NULL  && m_pTLBDate != NULL );
}

void CThreeLineBreak::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
										 CGraphCalculateData* pGraphCalculateData,
										 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pTLBTop);
	ClearPacketData(m_pTLBBottom);
	ClearPacketData(m_pTLBDate);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

// (2004.04.06, ��¿�, �м�) �Ｑ ��ȯ Data Packet�� �ǹ̸� ������ �� �ֵ��� �������� �����Ѵ�.
//		(TopPacket�� �Ｑ��ȯ�� �ð� Packet, BottomPacket�� ���� Packet)
// (2004.04.07, ��¿�, �м�) �Ｑ��ȯ��
//		1. �߼��� ������ ���, ���� �ؼ� ���� ������Ű��,
//			�߼��� ������ ���, ���� 3�� ���� �Ѿ�� ��쿡�� ���������� �����Ѵ�.
//		2. ���� �׻� ���� 4���� �Ｑ�� ������ �����Ͽ� ���� ��ǥ���� Ȯ���Ѵ�.
//		3. Ư�� ������ �߻��ϴ� ���, �������� ���� 4�� �Ｑ�� ������ ���� ���� �߼������� ��� �����Ͽ�,
//			�� �������� ��ǥġ�� �Ѵ�.
//		4. ���� �������� 4�� �Ｑ�� ���� �ʱ�ȭ��, ������ ������ �����ϱ⿡ �ռ�,
//			���� ������ '����' ������ �ξ�, �̹� ������ �ð��� �߰��Ѵ�.
//			�̰��� ����, 2ȸ �߼� Ȯ�� ����� ���� ���� ��ǥġ�� �Ǳ� �����̴�.
void CThreeLineBreak::GetThreeLineBreak(const CPoint &point,
										CList<double, double>* pClosePriceList,
										CList<double, double>* pDateList,
										CPacket* pStartPacket,
										CPacket* pEndPacket,
										CPacket* pDATEPacket,
										CList<double, double>* pConditionList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pClosePriceList) return;

//	(2006.09.07,�Ƚ�ȯ) �Ｑ��ȯ�� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	CList<double, double> m_DataList;
	if( pConditionList->IsEmpty()) return;
	int nCount = (int)pConditionList->GetHead();
	double dData;
	if(m_DataList.GetCount() > 0)
		m_DataList.RemoveAll();
	for(int i =0; i < nCount+1; i++)
	{
		dData = GetAt(pClosePriceList, 0);
		m_DataList.AddHead(dData);
	}
	
	POSITION posDate		= pDateList->GetHeadPosition();
	POSITION posClosePrice	= pClosePriceList->GetHeadPosition();
	while( posDate && posClosePrice)
	{
		double dClose	= pClosePriceList->GetNext( posClosePrice);
		double dDate	= pDateList->GetNext( posDate);

		if( GetAt(&m_DataList,0) == dClose) continue;

		if( ( dClose - GetAt(&m_DataList,0)) * ( GetAt(&m_DataList,0) - GetAt(&m_DataList,1)) >= 0) ;

		else if( 0 < ( GetAt(&m_DataList,nCount) - dClose) * ( GetAt(&m_DataList,0) - GetAt(&m_DataList,nCount)))
		{
			
			for(int j = nCount; j>=2; j--)
			{
				m_DataList.SetAt(FindPosition(j,m_DataList),GetAt(&m_DataList,0));
			}
			m_DataList.SetAt(FindPosition(0,m_DataList),GetAt(&m_DataList,1));			
			m_DataList.SetAt(FindPosition(1,m_DataList),GetAt(&m_DataList,2));
		}
		else continue;

		pStartPacket->AppendData( GetAt(&m_DataList,0));
		pEndPacket->AppendData(	dClose);
		pDATEPacket->AppendData(dDate);

		// 6. ���� �Ｑ�� ������ ������Ų��.
		for(int k = nCount; k>=1; k--)
		{
			m_DataList.SetAt(FindPosition(k,m_DataList),GetAt(&m_DataList,k-1));
		}
		m_DataList.SetAt(FindPosition(0,m_DataList),dClose);
		
	}
//	(2006.09.07,�Ƚ�ȯ) �Ｑ��ȯ�� ĭ ��ȯ���� �����Ҽ� �ֵ��� �� 
	
	// 1. ù �Ｑ��ȯ�� ���۽� 4�� ���� ������ ù �������� �ʱ�ȭ�Ѵ�.
/*	double data[ 4];
	data[ 0] = data[ 1] = data[ 2] = data[ 3] = GetAt(pClosePriceList, 0);

	// (2004.12.15, ��¿�) ó�� �ӵ��� ������ �ϱ� ���Ͽ� Index ��Ŀ��� List Link ����� ������.
	POSITION posDate		= pDateList->GetHeadPosition();
	POSITION posClosePrice	= pClosePriceList->GetHeadPosition();
	while( posDate && posClosePrice)
	{
		double dClose	= pClosePriceList->GetNext( posClosePrice);
		double dDate	= pDateList->GetNext( posDate);

		// 2. ���� �߼��� ������ ���� �ʴ� ��츦 �����Ѵ�.
		if( data[ 0] == dClose) continue;

		// 3. �߼��� Ȯ���̳� ������ �ƴ� ��츦 �����Ѵ�.
		// 3.1 �߼��� Ȯ���� ��츦 Ȯ���Ѵ�.
		//		(ó�� �߼� �����ÿ��� 'data[ 0] - data[ 1]'�� 0�̰�, ������ �߼��� Ȯ������ �����Ѵ�.)
		if( ( dClose - data[ 0]) * ( data[ 0] - data[ 1]) >= 0);
		// 3.2 �߼��� ������ ��츦 Ȯ���Ѵ�.
		//		(�߼��� ������ ���� 4��° �Ｑ�� ������ �����ϴ� ��쿩�� �Ѵ�. ���� �Ｑ ���� !
		//		 �� ���� 4��° �Ｑ�� ������, ������ �Ｑ�� ������ �̹� ���� ���̿� ������ �Ѵ�.)
		else if( 0 < ( data[ 3] - dClose) * ( data[ 0] - data[ 3]))
		{
			// 3.3 �����ÿ��� �������� ���� �Ｑ�� ������ ���ο� �߼��� �������� ������ �Ѵ�.
			//		�������� ���� �Ｑ���� ������ �Ｑ�� ������ �ʱ�ȭ�ϰ�,
			//		������ �Ｑ�� �����δ� �� '����' ������ �������Ѵ�.
			//		(���� ���� ��ǥ�� �ٷ��� �Ｑ�� �����̸�,
			//			���� �߼��� �������� �ʰ� ��� 2ȸ Ȯ���, �� '����' ������ ���� ���� ��ǥ�� �Ǳ� �����̴�.)
			data[ 3] = data[ 2] = data[ 0];
			data[ 0] = data[ 1];
			data[ 1] = data[ 2];
		}
		else continue;

		// 5. �Ｑ���� ��Ͻ�Ų��.
		pStartPacket->AppendData(	data[0]);
		pEndPacket->AppendData(		dClose);
		pDATEPacket->AppendData(	dDate);

		// 6. ���� �Ｑ�� 4���� ������Ų��.
		data[3] = data[2];
		data[2] = data[1];
		data[1] = data[0];
		data[0] = dClose;
	}
*/
}


//******************************************************************************************************************
// ��ǥ name : Zig Zag
// -���ϰ� ������ ���Ͽ� ������ ��ġ���� X��ŭ ������ ��(��)�ų� X%��ŭ ��(��)���� 
//  ������� ����Ʈ�� �� �� �ִ�.
// -ù �����Ϳ� �� �����ʹ� �ӽ÷� ������� ����Ʈ�� �ȴ�. 
// -��� ����Ʈ ������ �϶�����Ʈ�� ������ ��� ��� ����Ʈ�� ������� ����Ʈ�� �ǰ� 
//  �϶� ����Ʈ�� ��ٸ���.
// -����� ������ ���Դٰ� �϶��� ������ ��� �϶� �ٷ� �տ� �ִ� ��¸� ����� ��� 
//  �����Ѵ�.�ݴ�� �϶��� ������ ���Դٰ� ����� ������ ��� ��� �ٷ� �տ� �ִ� 
//  �϶��� ����� ��� ����
// -����� �ݺ��Ǵٰ� �϶��� ������ ���ϰ� �� �����Ϳ� �̸����� ��� ���������� 
//  �������� �ִ� ����� ������� ����Ʈ�� ���ߴٰ� �ڿ� �����Ͱ� �� �߰��� ��� �ٷ� 
//  �ڿ� ����� �� ��Ÿ������ ����� �־�� �Ѵ�.
//  �ݴ��� ��쵵 ��������
// ex)     0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  17  18  19  20  21
//       ���� 0  �� 0  0  �� 0  0  0  ��  0  ��   0  ��  ��   0   0   ��  0  ��   0  ��
// ���� ���� ������ �Ǿ� ���� ��� ����, 9, 14, 17, 19, ���� ������� ����Ʈ�� �ȴ�.
// �ֱ� ���ڷ� ǥ���ϱⰡ �����ϳ�... 
//******************************************************************************************************************
CZigzag::CZigzag() : 
	m_MoveAverge(0.0), 
	m_Startpoint(0.0), 
	m_lastDirection(NONE),
	m_PrevHighValue(0.0),
	m_PrevLowValue(0.0),
	m_ChangeValue(0.0),
	m_Min(0.0),
	m_Max(0.0),
	m_pZigDataPacket(NULL), 
	m_pZigIndexPacket(NULL),
	m_pEndPacket(NULL),
	m_pEndPriceList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_ZIGZAGCHART)
{
}

void CZigzag::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList || !pIndicatorInfo) return;
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		m_pEndPacket = GetPacket(calculateData.GetPoint(), chartFormulateData);
		m_pEndPriceList = GetnumericDataList(m_pEndPacket);
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pEndPacket) return;
	m_pEndPacket->GetMinMax(0, m_pEndPacket->GetCount(), m_Min, m_Max);

	if(!IsValidPacket(m_pEndPriceList))
		return;

	if(SetGraphPacketList(chartFormulateData.GetCalculateType(), pPacketList, pGraphCalculateData, 
		                   pIndicatorInfo))
		return;

	CPoint point = calculateData.GetPoint();

	CScaleBaseData::HORZSCALEDRAWERTYPE nType = pPacketList->GetDateUnitType();

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pZigDataPacket) return;

	CList<double, double>* pZigDataList = m_pZigDataPacket->GetnumericDataList();
	CList<double, double>* pZigIndexList = m_pZigIndexPacket->GetnumericDataList();
	///���ǰ��� 2���� �޾Ƽ� ù��° ���� ���ϴ� ��ġ�̰� �ι�° ���� �������� %���� ����
	GetZigzag(point, GetCondition(0, pConditionList), GetCondition(1, pConditionList),
			  m_pEndPriceList, pZigDataList, pZigIndexList, false, nType);

	CString strType = m_pEndPacket->GetType(false);
	SetPacketTypes(m_pZigDataPacket, TYPE_TWODECIMAL, strType, 0);
	SetPacketTypes(m_pZigIndexPacket, TYPE_TWODECIMAL, strType, 0);
}

void CZigzag::GetZigzag(const CPoint& point, double dCondition1, double dCondition2,
				   CList<double, double>* pEndPriceList, CList<double, double>* pZigDataList,
				   CList<double, double>* pZigIndexList, bool CLCflage,
				   CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	DIRECTION nDirection = NONE, nPreDirection = NONE;
	double HighLimit=0.0, LowLimit=0.0, ChangeValue=0.0, PrevValue=0.0, CurValue=0.0, CompareValue =0.0;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPriceList || !pZigDataList || !pZigIndexList) return;

	int nCount = pEndPriceList->GetCount();
	int nIndex=0, nSignal=0;
	BOOL HighFlage = true, LowFlage = true;
	int nPoint = point.x;

	if(point.x)
	{
		if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY )
			return ;
		else if(nType == CScaleBaseData::HORZ_TIME_ONE)
		{
			if(point.x != pEndPriceList->GetCount()-1)
			{
				nPoint = 0;
				PrevValue = GetAt(pEndPriceList, nPoint);
				pZigDataList->RemoveAll();
				pZigIndexList->RemoveAll();
			}
			else
				return ;
		}
		else if(nType == CScaleBaseData::HORZ_TICK)
		{
			PrevValue = GetRealDataIndex(pEndPriceList, pZigDataList, pZigIndexList, nPoint);
			if(PrevValue < 0)
				return ;
			if(pZigIndexList->IsEmpty())
			{
				nPreDirection = m_lastDirection = NONE;
			}
			else
			{
				nPreDirection = m_lastDirection;
			}
			nIndex = nPoint;
		}
	}
	else
	{
		double Max = 0.00, Min = pEndPriceList->GetHead();
		PrevValue = GetAt(pEndPriceList, nPoint);
		
		if(dCondition2==2)	//�ܼ� ��ġ
			m_ChangeValue = dCondition1;
		//else if(dCondition2==1)	///�ۼ�Ʈ
		else	///�ۼ�Ʈ
		{
			if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY )
				m_ChangeValue = PrevValue * (dCondition1 / 100);
			else
				m_ChangeValue = (m_Max - m_Min)*(dCondition1/100);
		}
	}

	double dPrevValue = 0;
	HighLimit = PrevValue + m_ChangeValue;
	LowLimit = PrevValue - m_ChangeValue;
	int i = 0;
	for( i = nPoint; i < nCount; i++)
	{
		CurValue = GetAt(pEndPriceList, i);

		// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( 0 == CurValue)	CurValue = dPrevValue;
		else				dPrevValue = CurValue;

		pZigIndexList->AddTail(0.00);

		if(CurValue > HighLimit){  //n%���..
			nDirection = HIGH;
			if(nDirection != nPreDirection){
				m_lastDirection = nDirection;
				pZigIndexList->SetAt(FindPosition(nIndex, *pZigIndexList), nIndex);
			}
			nIndex = i, nPreDirection = nDirection;
			LowLimit = MakeChangLowValue(CurValue, dCondition2, dCondition1, nType);
			HighLimit = CurValue;
		}
		else if(CurValue < LowLimit){
			nDirection = LOW;
			if(nDirection != nPreDirection){
				m_lastDirection = nDirection;
				pZigIndexList->SetAt(FindPosition(nIndex, *pZigIndexList), nIndex);
			}
			nIndex = i, nPreDirection = nDirection;
			HighLimit = MakeChangHighValue(CurValue, dCondition2, dCondition1, nType);
			LowLimit = CurValue;
			//nIndex�� n%���, �϶��� ���� ��ġ�� �����ϰ� �ִ�
		}
	}
	pZigIndexList->SetAt(FindPosition(i-1, *pZigIndexList), i-1);       

	int nPrevIndex=0, nCurIndex=0;
	double step=0,  Tostep = 0, TPrevValue=0;
	POSITION pointpos;
	if(point.x){
		if(nPoint)
		{
			pointpos = FindPosition(nPoint-1, *pEndPriceList); 
			PrevValue = GetNext(pEndPriceList, pointpos);
		}
		else
		{
			pointpos = FindPosition(nPoint, *pEndPriceList); 
			PrevValue = GetNext(pEndPriceList, pointpos);
		}

		nPrevIndex = nPoint-1;
		pZigIndexList->SetAt(pZigIndexList->GetHeadPosition(), m_Startpoint);
		m_PrevHighValue = PrevValue;
		m_PrevLowValue  = PrevValue;
	}
	else{
		pointpos = FindPosition(nPoint, *pEndPriceList); 
		PrevValue = GetNext(pEndPriceList, pointpos);
		pZigDataList->AddTail(PrevValue);
		m_PrevHighValue = PrevValue;
		m_PrevLowValue  = PrevValue;
		//ù����Ÿ�¸� high,low ������ ��´�..
	}
	int IndexCount = 0, StepCount = 0;
	double TotalStep = 0.00;
	const int indexCnt = pZigIndexList->GetCount();
	if(!CLCflage)
	{
		for(int n=nPrevIndex+1; n<indexCnt; n++)
		{
			nCurIndex = (int)(pZigIndexList->GetAt(FindPosition(n, *pZigIndexList)));
			if(nCurIndex && (abs(nCurIndex) != 9999))
			{
				CurValue = GetAt(pEndPriceList, nCurIndex);

				// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
				//		if Error, Set All with Close for Base or Market Close Price.
				if( 0 == CurValue && 0 < nCurIndex)	CurValue = GetAt(pEndPriceList, nCurIndex - 1);;

				step = (CurValue-PrevValue)/(nCurIndex-nPrevIndex);
				CompareValue = PrevValue; // ������ ��,�� ���� �˱� ���� �÷��� ��Ȱ
				//���μ��� ���ؼ� ����ϴ� ����.. ������׿����� ���� �ʿ� ���µ� ���̿�..
				TotalStep = TotalStep + fabs(step);
				StepCount++;
				///////////////////////////////////////////////////////////////////////////
				for(int l=nPrevIndex+1; l<=nCurIndex; l++)
				{
					pZigDataList->AddTail(PrevValue+=step);
					if(PrevValue > m_PrevHighValue && HighFlage)
					{
						if(!l)
							pZigIndexList->SetAt(FindPosition(l, *pZigIndexList), 9999);
						else
							pZigIndexList->SetAt(FindPosition(l-1, *pZigIndexList), 9999);
						HighFlage = false; LowFlage = true;
					}
					if(PrevValue < m_PrevLowValue && LowFlage)
					{
						if(!l)
							pZigIndexList->SetAt(FindPosition(l, *pZigIndexList), -9999);
						else
							pZigIndexList->SetAt(FindPosition(l-1, *pZigIndexList), -9999);
						LowFlage = false; HighFlage = true;
					}
				}
				if(StepCount == 1)
					HighFlage = true; LowFlage = true;
				nPrevIndex = nCurIndex;
				
				PrevValue  = CurValue;
				//������ ��,������ �������� ���ؼ� indexlist���� �׽� �� index�� ���� ������ �־�� �Ѵ�..
				if(CurValue > CompareValue)
					m_PrevHighValue = CurValue;  //���� ������ �ٲ۴�..
				else if(CurValue < CompareValue)
					m_PrevLowValue  = CurValue;  //���� ������ �ٲ۴�..
			}
		}
		if (StepCount > 0 && TotalStep > 0)
		{
			m_MoveAverge = TotalStep / StepCount;
			//ù����Ÿ�� ���� �����ϰ� �װ��� ���� ���۰��� �־� �ش�..
			POSITION pos = pEndPriceList->GetHeadPosition();
			if(pEndPriceList->GetNext(pos) < pEndPriceList->GetNext(pos))
			{
				m_Startpoint = pZigIndexList->GetHead();
				pZigIndexList->SetAt(FindPosition(0, *pZigIndexList), -9999);
			}
			else
			{
				m_Startpoint = pZigIndexList->GetHead();
				pZigIndexList->SetAt(FindPosition(0, *pZigIndexList), 9999);
			}
		}
	}
}
double CZigzag::GetRealDataIndex(CList<double, double>* pEndPriceList,CList<double, double>* pZigDataList, 
					             CList<double, double>* pZigIndexList, int& nPoint)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPriceList || !pZigDataList || !pZigIndexList) return 0;

	double PrevValue = 0.0;
	if(nPoint){
		if(pZigDataList->GetCount()==0 || pZigIndexList->GetCount() == 0)
			return -1;

		///�ڿ������� �����Ͽ� �ι�° ������� ����Ʈ���� �����. �� ������ ���� ó�� �߰ߵǴ� ������� ����Ʈ��
		///������ ������� ����Ʈ�� �ƴҼ��� �ֱ� �����̴�.
		pZigDataList->RemoveTail();
		pZigIndexList->RemoveTail();
		//������ ���� ù �ε��� �����Ͱ� ���ö� ���� ����Ÿ�� �����...
		while(!pZigIndexList->GetTail() || fabs(pZigIndexList->GetTail()) == 9999){
			pZigDataList->RemoveTail();
			pZigIndexList->RemoveTail();
			if(pZigIndexList->IsEmpty())
				break;
		}
		if(pZigIndexList->GetCount()){
			pZigDataList->RemoveTail();
			pZigIndexList->RemoveTail();
		}
		////������ ���� �ι�° �ε��� �����Ͱ� ���ö� ���� ����Ÿ�� �����...
		if(pZigIndexList->GetCount())
			while(!pZigIndexList->GetTail() || fabs(pZigIndexList->GetTail()) == 9999){
				pZigDataList->RemoveTail();
				pZigIndexList->RemoveTail();
				if(pZigIndexList->IsEmpty())
					break;
			}
		if(pZigIndexList->IsEmpty()){
			nPoint=0;	///���� ������ ������� ����Ʈ�� �ε���0�̰ų� ������� ����Ʈ�� ���� ���
			POSITION endpos = FindPosition(nPoint, *pEndPriceList);	
			PrevValue = GetNext(pEndPriceList, endpos);//�����̶� �ε���ī���Ͱ� 0�϶�..
		}
		else{
			const int nEndIndex = pZigIndexList->GetCount()-1;
			POSITION indexpos = FindPosition(nEndIndex, *pZigIndexList);
			nPoint = (int)(GetNext(pZigIndexList, indexpos));
			POSITION endpos = FindPosition(nPoint, *pEndPriceList);
			PrevValue = GetNext(pEndPriceList, endpos);
			nPoint++;	//���� ����Ʈ������ ���Ǿ� �����Ƿ� ���� ����Ʈ���� ����ؾ� �Ѵ�.
		}
	}
	return PrevValue;
}


bool CZigzag::ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList,
						  CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
		CString strSubGraphPacketNameHeader;
		if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

		m_pZigDataPacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + "Zig_Data",	false);//"Zig_Data"
		m_pZigIndexPacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + "Zig_Index",	false);//"Zig_Index"

		ClearPacketData(m_pZigDataPacket);
		ClearPacketData(m_pZigIndexPacket);
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphPacketList) return FALSE;

	pSubGraphPacketList->AddTail2( m_pZigIndexPacket);
	pSubGraphPacketList->AddTail2( m_pZigDataPacket);

	return (m_pZigDataPacket != NULL && m_pZigIndexPacket != NULL);
}

void CZigzag::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pZigDataPacket);
	ClearPacketData(m_pZigIndexPacket);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}

double CZigzag::MakeChangHighValue(double PrevValue, double dCondition2, double dCondition1,
								   CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	if(dCondition2==2)	//�ܼ� ��ġ
		m_ChangeValue = dCondition1;
	else
	{///�ۼ�Ʈ
		if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY )
			m_ChangeValue = PrevValue * (dCondition1 / 100);
		else
			m_ChangeValue = (m_Max - m_Min) * dCondition1 / 100; 
	}
	double HighLimit		= PrevValue + m_ChangeValue;
	return HighLimit;
}

double CZigzag::MakeChangLowValue(double PrevValue, double dCondition2, double dCondition1,
								  CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	if(dCondition2==2)	//�ܼ� ��ġ
		m_ChangeValue = dCondition1;
	else	///�ۼ�Ʈ
	{
		if(nType == CScaleBaseData::HORZ_DAILY || nType == CScaleBaseData::HORZ_WEEKLEY || nType == CScaleBaseData::HORZ_MONTHLY )
			m_ChangeValue = PrevValue * (dCondition1 / 100);
		else
			m_ChangeValue = (m_Max - m_Min) * dCondition1 / 100; 
	}
	double LowLimit		= PrevValue - m_ChangeValue;
	return LowLimit;
}

bool CZigzag::SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
								   CPacketList* pPacketList,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
		{
			return ( !ClearPacket(calculatetype, pPacketList, pSubGraphList, pIndicatorInfo));
		}
	}
	return (m_pZigDataPacket == NULL && m_pZigIndexPacket == NULL);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//������ ��Ʈ
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
CCLC::CCLC() : 
	m_step(0.0),
	m_nType(CScaleBaseData::HORZ_NOTYPE),
	m_lastDirection(NONE),
	m_PrevHighValue(0.0),
	m_PrevLowValue(0.0),
	m_pCLCDataPacket(NULL),
	m_pCLCIndexPacket(NULL),
	m_pEndPacket(NULL),
	m_pEndPriceList(NULL),
	CIndicator( IMetaTable::EDDC_OCX_CLCCHART)
{
}

void CCLC::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList || !pIndicatorInfo) return;
	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();

	if(chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		m_pEndPacket = GetPacket(calculateData.GetPoint(), chartFormulateData);
		m_pEndPriceList = GetnumericDataList(m_pEndPacket);
	}
	if(!IsValidPacket(m_pEndPriceList))
		return ;

	if(SetGraphPacketList(chartFormulateData.GetCalculateType(), pPacketList, pGraphCalculateData, 
		                   pIndicatorInfo))
		return ;

	CPoint point = calculateData.GetPoint();
	if(point.x == m_pEndPriceList->GetCount()-1)
		return ;

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !m_pCLCDataPacket) return;

	else if(point.x == 0 && m_pCLCDataPacket->GetCount() > 0)
		return ;
	else if(point.x > 0)
	{
		ClearPacketData(m_pCLCDataPacket);
		ClearPacketData(m_pCLCIndexPacket);
		point.x = 0;
	}
	
	///���ǰ��� 2���� �޾Ƽ� ù��° ���� ���ϴ� ��ġ�̰� �ι�° ���� �������� %���� ����
	GetCLC(point, GetCondition(0, pConditionList), GetCondition(1, pConditionList),
		   m_pEndPriceList, m_pCLCDataPacket, m_pCLCIndexPacket);

	CString strType = TYPE_INTERGER;
	if(m_pEndPacket != NULL)
		strType = m_pEndPacket->GetType(false);

	SetPacketTypes(m_pCLCDataPacket, TYPE_TWODECIMAL, strType, 0);
	SetPacketTypes(m_pCLCIndexPacket, TYPE_TWODECIMAL, strType, 0);
}

void CCLC::GetCLC(const CPoint& point, double dCondition1, double dCondition2,
			  	  CList<double, double>* pEndPriceList, CPacket* pCLCDataPacket,
				  CPacket* pCLCIndexPacket)
{
	CZigzag zig;
	DIRECTION nDirection = NONE, nPreDirection = NONE;
	double startdata =0.00;
	double Tostep = 0,   TPrevValue=0, CompareValue = 0.00;
	int nIndex=0, ToValue = 0, nPoint = 0;
	int nPrevIndex=0, nCurIndex=0;
	int StepIndex = 0, StepCount = 0, IndexCount = 0, count = 0; 

	BOOL HighFlage = true, LowFlage = true;
	
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pCLCDataPacket || !pCLCIndexPacket) return;

	CList<double, double>* pCLCDataList = pCLCDataPacket->GetnumericDataList();
	CList<double, double>* pCLCIndexList = pCLCIndexPacket->GetnumericDataList();
	CList<double, double> dZizagDataList;
	CList<double, double> dZizagIndexList;
	
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPriceList) return;

	int nCount = pEndPriceList->GetCount();
	startdata = pEndPriceList->GetHead();

	CPacketList* pPacketList = pCLCIndexPacket->GetPacketList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return;

	CScaleBaseData::HORZSCALEDRAWERTYPE nType = pPacketList->GetDateUnitType();

	zig.GetZigzag(point, dCondition1, dCondition2, pEndPriceList, &dZizagDataList,
	              &dZizagIndexList, true, nType);

	CalculateRatio(point, dZizagIndexList, pEndPriceList);
	
	int Color =0;
	if(GetAt(pEndPriceList, 0) > GetAt(pEndPriceList, 1))
	{
		nDirection = LOW;
		Color = 1;
	}

	bool flage = SetCLCIndexList(nPrevIndex, pEndPriceList, dZizagIndexList, pCLCIndexList, pCLCDataList);

	if(pCLCIndexList->GetCount() < dZizagIndexList.GetCount())
	{
		int nCount = dZizagIndexList.GetCount() - pCLCIndexList->GetCount();
		double prevalue = pCLCDataList->GetTail(); 
		for(int i = 0; i < nCount; i++)
		{
			pCLCIndexList->AddTail(0.0);
			if(flage)
				pCLCDataList->AddTail(prevalue+=m_step);
			else
				pCLCDataList->AddTail(prevalue-=m_step);
		}
	}
	nPrevIndex = 0;
	double PrevValue = GetAt(pCLCDataList, nPoint+1);
	m_PrevHighValue = PrevValue;
	m_PrevLowValue  = PrevValue;

	if(GetAt(pCLCDataList, 0) > GetAt(pCLCDataList, 1))
		Color = 1;
	for(int n = nPrevIndex; n < pCLCIndexList->GetCount(); n++){
		nCurIndex = (int)(pCLCIndexList->GetAt(FindPosition(n, *pCLCIndexList)));
		if(nCurIndex){
			CompareValue = PrevValue; // ������ ��,�� ���� �˱� ���� �÷��� ��Ȱ
			double CurValue = pCLCDataList->GetAt(FindPosition(nCurIndex, *pCLCDataList));
			if(!nPrevIndex)
			{
				if(PrevValue >= CurValue)
					HighFlage = false;
				else
					LowFlage = false;
			}
			if(int p = Color%2)
				m_PrevHighValue = PrevValue;
			else
				m_PrevLowValue = PrevValue;
			
			for(int l = nPrevIndex+1; l<=nCurIndex; l++)
			{
				PrevValue = GetAt(pCLCDataList , l);
				if(PrevValue >= m_PrevHighValue && HighFlage)
				{
					if(nPrevIndex)
						pCLCIndexList->SetAt(FindPosition(l, *pCLCIndexList), 9999);
					else
						pCLCIndexList->SetAt(FindPosition(l-1, *pCLCIndexList), 9999);
					HighFlage = false; LowFlage = true;
					break;
				}
				if(PrevValue <= m_PrevLowValue && LowFlage)
				{
					if(nPrevIndex)
						pCLCIndexList->SetAt(FindPosition(l, *pCLCIndexList),-9999);
					else
						pCLCIndexList->SetAt(FindPosition(l-1, *pCLCIndexList), -9999);
					LowFlage = false; HighFlage = true;
					break;
				} 
			}
			nPrevIndex = nCurIndex;
			PrevValue = CurValue;
			Color++;
		}
	}
	return ;
}
bool CCLC::SetCLCIndexList(double nPrevIndex, CList<double, double>* pEndPriceList, 
							 CList<double, double>& dZigzagIndexList, 
							 CList<double, double>* pCLCIndexList, CList<double, double>* pCLCDataList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPriceList || !pCLCDataList || !pCLCIndexList) return FALSE;

	double remain = 0.0;
	double CompareValue = 0.0;
	DIRECTION nDirection = NONE, nPreDirection = NONE;
	bool flage = true;
	int count = 0;
	double PrevValue = GetAt(pEndPriceList, 0);
	for(int n = (int)nPrevIndex; n < dZigzagIndexList.GetCount(); n++){
		int nCurIndex = (int)(dZigzagIndexList.GetAt(FindPosition(n, dZigzagIndexList)));
		if(nCurIndex ){
			CompareValue = PrevValue; // ������ ��,�� ���� �˱� ���� �÷��� ��Ȱ
			double CurValue = pEndPriceList->GetAt(FindPosition(nCurIndex, *pEndPriceList));
			
			if(CurValue < PrevValue)
			{
				if(nDirection == LOW)
					dZigzagIndexList.SetAt(FindPosition((int)nPrevIndex, dZigzagIndexList), 0);
				flage = false;
				nDirection = LOW;
			}
			else if(CurValue > PrevValue)
			{
				if(nDirection == HIGH)
					dZigzagIndexList.SetAt(FindPosition((int)nPrevIndex, dZigzagIndexList), 0);
				nDirection = HIGH;
			}
			if(flage)
			{
				CurValue = CurValue - remain;
				while(PrevValue <= CurValue)
				{
					pCLCDataList->AddTail(PrevValue+=m_step);
					pCLCIndexList->AddTail(0.0);
					count++;
					if(pCLCDataList->GetCount() >= dZigzagIndexList.GetCount())
						return flage;
				}
				pCLCIndexList->SetAt(FindPosition(count-1, *pCLCIndexList), count-1);
			}
			else
			{
				CurValue = CurValue + remain;
				while(PrevValue >= CurValue)
				{
					pCLCDataList->AddTail(PrevValue-=m_step);
					pCLCIndexList->AddTail(0.0);
					count++;
					if(pCLCDataList->GetCount() >= dZigzagIndexList.GetCount())
						return flage;
				}
				pCLCIndexList->SetAt(FindPosition(count-1, *pCLCIndexList), count-1);
			}
			flage = true;
			nPrevIndex = nCurIndex;
			remain = fabs(CurValue - PrevValue);
		}
	}
	return flage;
}
double CCLC::MakeChangHighValue(double PrevValue, double dCondition2, double dCondition1,
								CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	double ChangeValue;
	if(dCondition2==2)	///�ܼ� ��ġ


		 ChangeValue = dCondition1;
	else	///�ۼ�Ʈ
		 ChangeValue = PrevValue * dCondition1 / 100;
	double HighLimit		= PrevValue + ChangeValue;
	return HighLimit;
}

double CCLC::MakeChangLowValue(double PrevValue, double dCondition2, double dCondition1,
							   CScaleBaseData::HORZSCALEDRAWERTYPE nType)
{
	double ChangeValue;
	if(dCondition2==2)	///�ܼ� ��ġ
		 ChangeValue = dCondition1;
	else	///�ۼ�Ʈ
		 ChangeValue = PrevValue * dCondition1 / 100;


	double LowLimit		= PrevValue - ChangeValue;
	return LowLimit;
}
void CCLC::CalculateRatio(const CPoint& point, CList<double, double>& dZigzagList, 
						  CList<double, double>* pEndPriceList)
{
	int preindex = 0, count = 0, nPoint = 0, nCurIndex = 0;
	double PrevValue = 0.0, ratio = 0.0, curdata = 0.0, predata = 0.0;
	
	PrevValue = GetAt(pEndPriceList, nPoint);
	const int indexCnt = dZigzagList.GetCount();
	POSITION pos = dZigzagList.GetHeadPosition();

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPriceList) return;

	POSITION pos1 = pEndPriceList->GetHeadPosition();

	for(int z = point.x; z < indexCnt; z++){
		curdata = pEndPriceList->GetNext(pos1);
		if(z == 0)
		{
			predata = curdata;
			preindex = z;
		}
		nCurIndex = (int)(dZigzagList.GetNext(pos));
		if(nCurIndex)
		{
			ratio += fabs(predata - curdata);
			predata = curdata;
			preindex = z;
			count++;
		}
	}
	m_step = ratio/(pEndPriceList->GetCount()-1);
	m_PrevHighValue = PrevValue;
	m_PrevLowValue  = PrevValue;
}
bool CCLC::SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
								   CPacketList* pPacketList,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo*& pIndicatorInfo)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pGraphCalculateData) return FALSE;

	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return false;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL)
		{
			return (!ClearPacket(calculatetype, pPacketList, pSubGraphList));
		}
	}
	return (m_pCLCDataPacket == NULL && m_pCLCIndexPacket == NULL);

}

bool CCLC::ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, 
						  CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList)
{
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pPacketList) return FALSE;

	if(calculatetype == CCalculateBaseData::TRDATA_CAL) 
	{
		m_pCLCDataPacket = pPacketList->GetBaseData("CLC_Data", false);
		m_pCLCIndexPacket = pPacketList->GetBaseData("CLC_Index", false);

		ClearPacketData(m_pCLCDataPacket);
		ClearPacketData(m_pCLCIndexPacket);
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pSubGraphPacketList) return FALSE;

	pSubGraphPacketList->AddTail2(m_pCLCIndexPacket);
	pSubGraphPacketList->AddTail2(m_pCLCDataPacket);

	return (m_pCLCDataPacket != NULL && m_pCLCIndexPacket != NULL);
}

void CCLC::DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo)
{
	ClearPacketData(m_pCLCDataPacket);
	ClearPacketData(m_pCLCIndexPacket);

	CIndicator::DeleteDataInPacket(chartFormulateData, pGraphCalculateData, pIndicatorInfo);
}


CSellBuyHorizontalBar::CSellBuyHorizontalBar() : CIndicator( IMetaTable::EDDC_OCX_VOLUMESELLBUYCHART)
{
	// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ�� ���⿡ �ʿ��� Packet�� �����Ѵ�.
	m_pHorZonPacket			= NULL;
	m_pHorZonPricePacket	= NULL;
	m_pEndPriceList			= NULL;
	m_pVolumeList			= NULL;

	// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ�� ���� ������ �⺻���� �����Ѵ�.
	m_StartPoint			= 0;
	m_EndPoint				= 0;
	m_Max					= 0.0;
	m_Min					= 0.0;
}

// (2004.02.03, ��¿�, �м�) ���Ź� ��ǥ���� �����Ѵ�. (������ ���� �⺻ ���� ����)
void CSellBuyHorizontalBar::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 1. Packet List�� ���Ͽ� �����Ѵ�.
	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	
	// 2. ���� TR�� ó���ø� ó���Ѵ�. (�ʱ�ȭ�Ѵ�.)
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::TRDATA_CAL)
	{
		// (2007/3/27 - Seung-Won, Bae) Use Map Setting of Packet
		CString strPacketNames = chartFormulateData.GetPacketNames();
		CPacket *pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));	// ����
		if( !pPacket) return;
		m_pEndPriceList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));			// �⺻�ŷ���
		if( !pPacket) return;
		m_pVolumeList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));			// �ż��ŷ���
		if( !pPacket) return;
		m_pBuyVolumeList = pPacket->GetnumericDataList();
		pPacket = pPacketList->GetBaseData(CDataConversion::GetStringData(strPacketNames, ","));			// �ŵ��ŷ���
		if( !pPacket) return;
		m_pSellVolumeList = pPacket->GetnumericDataList();

		// 2.2 Output Packet Pointer�� Clear�Ѵ�.
		//		(Object�� ������ ��� �ϴ���?)
		m_pHorZonPacket = NULL;
		m_pHorZonPricePacket = NULL;
		m_pHorZonBuyPacket = NULL;
		m_pHorZonSellPacket = NULL;
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
		CString csHoga;
		(pPacketList->GetHelpMessageMap())->Lookup("HBPRICE", csHoga);
		m_dHoga = atof(csHoga);
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	}

	// 3. Input Packet�� ��ȿ���� Ȯ���Ѵ�.
	if( !IsValidPacket( m_pEndPriceList) || !IsValidPacket( m_pVolumeList) ||
		!IsValidPacket( m_pSellVolumeList) || !IsValidPacket( m_pBuyVolumeList)) return;

	// 4. ���Ź� Output Packet�� Ȯ���Ѵ�.
	// 4.1 �̹� Ȯ���Ǿ� �ִ� ���� �����Ѵ�. (TR�� ��쿡�� NULL�� �ʱ�ȭ�Ǿ� �ִ�.)
	if( !m_pHorZonPacket || !m_pHorZonPricePacket || !m_pHorZonBuyPacket || !m_pHorZonSellPacket)
	{
		// 4.2 CGraphCalculateData���� Output Packet�� Ȯ���Ѵ�.
		// 4.2.1 ��ǥ Output Packet ������ �ִ� ��츸 ó���Ѵ�.
		//		SubGraphList�� ������ 2�� (���Ź��� �׻� ������Ʈ�� �����Ѵ�.)
		//		ù��° ����Ʈ���� ���Ź� �ŷ�������, ���Ź����ݴ� ��Ŷ �ּ�
		//		�ι�° ����Ʈ���� ��,��,��,�� ��Ŷ �ּ�
		// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
		if( !pGraphCalculateData) return;
		if( pGraphCalculateData->GetCount() > 0)
		{
			// 4.2.2 CGraphCalculateData���� Output Packet�� Sub Graph List�� Ȯ���Ѵ�.
			CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetHead();
			if( !pSubGraphList) return;

			// 4.2.3 Output Packet�� Ȯ���Ѵ�.
			m_pHorZonPacket			= pSubGraphList->GetPacket( 0);
			m_pHorZonPricePacket	= pSubGraphList->GetPacket( 1);
			m_pHorZonBuyPacket		= pSubGraphList->GetPacket( 2);
			m_pHorZonSellPacket		= pSubGraphList->GetPacket( 3);
		}

		// 4.3 CGraphCalculateData���� Output Packet�� Ȯ���� ������ ��츦 ó���Ѵ�.
		//		Packet List���� ���� ���Ѵ�.
		if( !m_pHorZonPacket || !m_pHorZonPricePacket)
		{
			// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
			CString strSubGraphPacketNameHeader;
			if( pIndicatorInfo) if( pIndicatorInfo->GetGraphName() != pIndicatorInfo->GetIndicatorName()) strSubGraphPacketNameHeader = pIndicatorInfo->GetGraphName() + "_";

			// 4.3.1 Packet List���� ���� �̸��� ���Ź� Output Packet�� ���Ѵ�.
			m_pHorZonPacket			= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY),				FALSE);
			m_pHorZonPricePacket	= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C0_BUY_SELL_OVERHANGING_SUPPLY_PRICE_BAND),	FALSE);
			m_pHorZonBuyPacket		= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C0_BUY_OVERHANGING_SUPPLY),					FALSE);
			m_pHorZonSellPacket		= pPacketList->GetBaseData( strSubGraphPacketNameHeader + _MTEXT( C0_SELL_OVERHANGING_SUPPLY),					FALSE);

			// 4.3.2 Packet�� Ȯ���� ������ ��츦 ó���Ѵ�.
			if( !m_pHorZonPacket || !m_pHorZonPricePacket) return;

			// 4.3.3 ������ Packet�� Data�� �ʱ�ȭ�Ѵ�.
			// (2004.02.05, ��¿�) Real ó���ÿ��� ���ŵ� Real�� ���� ���Ź� ��ǥ�� �����۾�����
			//		�����Ǿ� �־�����, ������ Real�� ���͵� �װ��� �������� ��쿡�� �����ؾ� �ϹǷ� Logic�� ���� �ʴ´�.
			//		�Ƹ� ������ �׻� ��ü ���⸦ �������� �Ǿ� �־��� �Ͱ���.
			//		���� Real ó���� TRó�� �׻� ���� ������ �߽����� �����ϴ� ������ �����Ѵ�.
			//		���� if �����ۿ��� �׻� Packet�� Clear��Ű�� �����Ѵ�.

			// 4.3.4 ������ Packet�� ���Ź� I/O Packet ������ ��Ͻ�Ų��.
			CSubGraphPacketList *pSubGraphList = NULL;
			POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
			if( posSubGraph)
			{
				pSubGraphList = pGraphCalculateData->GetNext( posSubGraph);
				if( pSubGraphList)
				{
					pSubGraphList->AddTail2( m_pHorZonPacket);
					pSubGraphList->AddTail2( m_pHorZonPricePacket);
					pSubGraphList->AddTail2( m_pHorZonBuyPacket);
					pSubGraphList->AddTail2( m_pHorZonSellPacket);
				}
			}
		}
	}

	// (2004.06.25, ��¿�) ������ �ϳ��� ���� ��쿡���� ���Ź��� ������� �ʴ´�.
	if( m_pEndPriceList->GetCount() <= 0) return;

	// 6. ���Ź� ���� ������ ������ ���Ͽ� �����Ѵ�.
	//		���� GetHorizontalBar()���� �̿��Ѵ�.
	m_StartPoint = chartFormulateData.GetDisplayDataStartIndex();
	m_EndPoint = chartFormulateData.GetDisplayDataEndIndex();
	
	// (2006/8/22 - Seung-Won, Bae) Real Calculate is in the case of Last Data in View.
	if( chartFormulateData.GetCalculateType() == CCalculateBaseData::REALDATA_CAL
		&& m_EndPoint < m_pEndPriceList->GetCount() - 1)
			return;

	// 5. Scroll�� ���� ���Ź� ������ ���Ͽ� Ouput Packet�� Clear��Ų��.
	// (2004.02.05, ��¿�) Real ó���ÿ��� ���ŵ� Real�� ���� ���Ź� ��ǥ�� �����۾�����
	//		�����Ǿ� �־�����, ������ Real�� ���͵� �װ��� �������� ��쿡�� �����ؾ� �ϹǷ� Logic�� ���� �ʴ´�.
	//		�Ƹ� ������ �׻� ��ü ���⸦ �������� �Ǿ� �־��� �Ͱ���.
	//		���� Real ó���� TRó�� �׻� ���� ������ �߽����� �����ϴ� ������ �����Ѵ�.
	//		���� �׻� Packet�� Clear��Ű�� �����Ѵ�.
	ClearPacketData( m_pHorZonPacket);
	ClearPacketData( m_pHorZonPricePacket);
	ClearPacketData( m_pHorZonBuyPacket);
	ClearPacketData( m_pHorZonSellPacket);

	// (2004.06.25, ��¿�) ���� Index�� �������� ���� �ƴϸ� ���Ź��� ������� �ʴ´�.
	if( m_EndPoint < 0 || m_StartPoint < 0) return;
	CPacket *pEndPacket = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));//"����"
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPacket) return;
	pEndPacket->GetMinMax( m_StartPoint, m_EndPoint, m_Min, m_Max);
	// (2004.06.25, ��¿�) ���� ������ ���� 0�̸�, ��������ġ ���Ϸ� (����/�ɼ� ����, 0.001) ���� ������.
	if( m_Min == m_Max)
	{
		m_Min -= 0.001;
		m_Max += 0.001;
	}
	
	// 7. ���Ź� ��ǥ�� �����Ѵ�.
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pIndicatorInfo) return;
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	GetSellBuyHorizontalBar( chartFormulateData, calculateData.GetPoint(),
		GetCondition( 0, pIndicatorInfo->GetCommonCalConditionList()), m_pEndPriceList, m_pVolumeList,
		m_pSellVolumeList,m_pBuyVolumeList,GetCondition( 1, pIndicatorInfo->GetCommonCalConditionList()),
		m_dHoga);
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�

	// 8. ���Ź� Output Packet�� ����Ѵ�.
	SetPacketTypes( m_pHorZonPacket,		pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
	SetPacketTypes( m_pHorZonPricePacket,	pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
	SetPacketTypes( m_pHorZonBuyPacket,		pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
	SetPacketTypes( m_pHorZonSellPacket,	pEndPacket->GetType(true), pEndPacket->GetType(false), 0);
}

// ���Ź� ��ǥ�� �����Ѵ�. (���ݴ� ���� ���� �� �ŷ��� ���� ó��)
void CSellBuyHorizontalBar::GetSellBuyHorizontalBar(const CChartFormulateData& chartFormulateData, 
				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
				CList<double, double>* pVolumeList,CList<double, double>* pSellVolumeList,
				CList<double, double>* pBuyVolumeList,double dConditionType,double dHoga)
{
	// NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pEndPriceList || !pVolumeList || !pBuyVolumeList || !pSellVolumeList) return;

	// 1. ������ ü�ᷮ Packet�� Data�� Ȯ���Ѵ�.
	if( pEndPriceList->GetCount() < 1 || pVolumeList->GetCount() < 1 || 
		pSellVolumeList->GetCount() < 1|| pBuyVolumeList->GetCount() < 1)	return;

	// 2. ���� ������ �� Index�� Ȯ���Ͽ�, �� Data�� ����� ��츦 �����Ѵ�.
	//		(�̰��� Chart�� ���� ������ �ִ� ���� �Ǵܵȴ�.)
	if( m_EndPoint > pEndPriceList->GetCount() - 1) m_EndPoint = pEndPriceList->GetCount() - 1;

	// 3. Real ���Ž��� ��츦 �켱 ó���Ͽ� �����Ѵ�.
	// Real ó���ÿ��� ���ŵ� Real�� ���� ���Ź� ��ǥ�� �����۾�����
	//		�����Ǿ� �־�����, ������ Real�� ���͵� �װ��� �������� ��쿡�� �����ؾ� �ϹǷ� Logic�� ���� �ʴ´�.
	//		�Ƹ� ������ �׻� ��ü ���⸦ �������� �Ǿ� �־��� �Ͱ���.
	//		���� Real ó���� TRó�� �׻� ���� ������ �߽����� �����ϴ� ������ �����Ѵ�.

	// 4. TR�̳� Scroll�� ���� ������ ��츦 ó���Ѵ�.
	// 4.1 ������ ������ ���Ź� Data���� �ִ� ��� Clear ��Ų��.

	if( !m_pHorZonPacket || !m_pHorZonPricePacket) return;
	if( m_pHorZonPacket->GetCount() > 0)
	{
		m_pHorZonPacket->ClearData();
		m_pHorZonPricePacket->ClearData();
		m_pHorZonSellPacket->ClearData();
		m_pHorZonBuyPacket->ClearData();
	}

	// 4.2 ���� ���Ź� �ŷ������� Packet�� �������� Clear��Ų��.
	//		(���� ����ġ���� 2�� �� �ʱ�ȭ�Ѵ�. �װ��� ���� Drawing �������� ������ Data�� 
	//		 �װ��� ���� Drawing �������� ������ Data�� ���� ������ ���ݴ� Index��
	//		 ���� �������� ���ݴ뺰 �ŷ��������� �ִ��� ���ݴ��� Index�� �����Ѵ�.
	//		 Drawing�ÿ� ���� ���ݴ�� �ִ� ���ݴ��� High Light�� �����ȴ�.)
	CList<double, double>* pHorizontalBarList = m_pHorZonPacket->GetnumericDataList();
	CList<double, double>* pSellHorizontalBarList = m_pHorZonSellPacket->GetnumericDataList();
	CList<double, double>* pBuyHorizontalBarList = m_pHorZonBuyPacket->GetnumericDataList();

	if( !pHorizontalBarList || !pSellHorizontalBarList || !pBuyHorizontalBarList) return;

	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	double nInterval;
	if( dCondition < 1) dCondition = 1;
	if(dConditionType && 0 < dHoga)
	{
		nInterval = dHoga * dCondition;
		dCondition = int( ( ( m_Max - m_Min) / nInterval) * ( 1 + DBL_EPSILON)) + 1;
	}
	else
	{
		dHoga = 0;
		nInterval = ( m_Max - m_Min) / dCondition;
	}
	// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	int i = 0;
	for( i = 0; i < dCondition + 2; i++) 
	{
		pHorizontalBarList->AddTail( 0.0);
		pSellHorizontalBarList->AddTail( 0.0);
		pBuyHorizontalBarList->AddTail( 0.0);
	}
	// 4.3 ������ dCondition ������ �����Ͽ� ���Ź� ���ݴ� Packet ���� �����Ѵ�.
	CList<double, double>* pPriceZonList = m_pHorZonPricePacket->GetnumericDataList();

	if( !pPriceZonList) return;
	double	dHogaPrice = 0.0;
	for( i = 0; i <= dCondition; i++)
	{
		if( i == 0) dHogaPrice = m_Min;
		else if( i == dCondition && dHoga <= 0) dHogaPrice = m_Max;
		else dHogaPrice = m_Min + nInterval  * i;
		
		pPriceZonList->AddHead( dHogaPrice);
	}
// (2006.10.23 �Ƚ�ȯ) ���Ź� ȣ���� ��� �߰�
	// 4.5 ���Ź� ���ⱸ���� �ŷ����� �����Ѵ�.
	//	(�̶� ���� ������ ���ݴ� Index�� ���� ��� �����Ѵ�.)
	// 4.5.0 ���Ź� ���� ������ �� ������ �ŷ����� Position�� ���Ͽ� �����Ѵ�.
	//		���� ���� �������� Ȯ���ϴµ� �̿��Ѵ�.
	POSITION posEndPrice	= FindPosition( m_EndPoint, *pEndPriceList);
	POSITION posEndVolume	= FindPosition( m_EndPoint, *pVolumeList);
	POSITION posEndSellVolume	= FindPosition( m_EndPoint, *pSellVolumeList);
	POSITION posEndBuyVolume	= FindPosition( m_EndPoint, *pBuyVolumeList);
	if( posEndPrice == NULL || posEndVolume == NULL|| posEndSellVolume == NULL|| posEndBuyVolume == NULL) return;
	// 4.5.1 ���Ź� ���ⱸ���� ù ������ �ŷ����� Position�� ���Ѵ�.
	POSITION posOldPrice	= NULL, posPrice	= FindPosition( m_StartPoint, *pEndPriceList);
	POSITION posOldVolume	= NULL, posVolume	= FindPosition( m_StartPoint, *pVolumeList);
	POSITION posOldSellVolume	= NULL, posSellVolume	= FindPosition( m_StartPoint, *pSellVolumeList);
	POSITION posOldBuyVolume	= NULL, posBuyVolume	= FindPosition( m_StartPoint, *pBuyVolumeList);
	if( posPrice == NULL || posVolume == NULL) return;
	// 4.5.2 ���� ������ �� ������ ��������, �ش� ���Ź� ���ݴ��� �ŷ����� ����ó���Ѵ�.
	//		(���� �������� ó���ϵ��� �Ѵ�.)
	int nIndex;
	while( posPrice && posVolume && posSellVolume && posBuyVolume&& posEndPrice	!= posOldPrice && 
		posEndVolume != posOldVolume && posEndSellVolume != posOldSellVolume && posEndBuyVolume != posOldBuyVolume)
	{
		// 4.5.2.0 ���� Position�� Backup�Ѵ�.
		posOldPrice		= posPrice;
		posOldVolume	= posOldVolume;

		// 4.5.2.1 ���ݴ� Index�� �����Ѵ�.
		// (2008/4/23 - Seung-Won, Bae) Repaire of the loss in double calculation.
		double dValue = ( pEndPriceList->GetNext( posPrice) - m_Min) / nInterval;
		dValue *= ( 1 + DBL_EPSILON * 10000);
		nIndex = int( dCondition - int( dValue)) - 1;

		// 4.5.2.2 Zero Base�� ��ȯ�Ѵ�.
		//		�̶� ������ ���ⱸ������ �ְ��� ��쿡��, nIndex�� -1�� �����Ǵµ�,
		//		�̴� ù ���ݴ� ������ �ְ� ������ ��ǥ���� ���� ���Ѱ��� ������ �Ӹ� �ƴ϶�
		//			���� ��ü������ �ְ��� �����Ѵ�.
		if( nIndex < 0) nIndex = 0;

		// 4.5.2.3 ������ Index�� ���ݴ뿡 �ŷ����� �����Ѵ�.
		POSITION posVolumeBar = FindPosition( nIndex, *pHorizontalBarList);
		pHorizontalBarList->SetAt( posVolumeBar, GetAt( pHorizontalBarList, posVolumeBar)
			+ pVolumeList->GetNext(posVolume));
	
		POSITION posSellVolumeBar = FindPosition( nIndex, *pSellHorizontalBarList);
		pSellHorizontalBarList->SetAt( posSellVolumeBar, GetAt( pSellHorizontalBarList, posSellVolumeBar)
			+ pSellVolumeList->GetNext(posSellVolume));
	
		POSITION posBuyVolumeBar = FindPosition( nIndex, *pBuyHorizontalBarList);
		pBuyHorizontalBarList->SetAt( posBuyVolumeBar, GetAt( pBuyHorizontalBarList, posBuyVolumeBar)
			+ pBuyVolumeList->GetNext(posBuyVolume));

		// 4.5.2.4 ���������� ����� ���� ������ ���ݴ� Index�� ����Ͽ� �����Ѵ�.
		pHorizontalBarList->SetAt( FindPosition( ( int)dCondition, *pHorizontalBarList), nIndex);
		pSellHorizontalBarList->SetAt( FindPosition( ( int)dCondition, *pSellHorizontalBarList), nIndex);
		pBuyHorizontalBarList->SetAt( FindPosition( ( int)dCondition, *pBuyHorizontalBarList), nIndex);
	}

	// 4.6 ���Ź� ���ⱸ���� ���ݴ뺰 �ŷ��� ������ �ְ�ġ�� ���ݴ��� Index�� ���� �����Ѵ�.
	POSITION pos = pHorizontalBarList->GetHeadPosition();
	POSITION posBuy = pBuyHorizontalBarList->GetHeadPosition();
	POSITION posSell = pSellHorizontalBarList->GetHeadPosition();
	if(pos == NULL) return;
	double dVolume, dMaxVolume = 0;
	double dSellVolume, dMaxSellVolume = 0;
	double dBuyVolume, dMaxBuyVolume = 0;
	for(i = 0; i < dCondition; i++)
	{
		dVolume = pHorizontalBarList->GetNext(pos);
		dSellVolume = pBuyHorizontalBarList->GetNext(posBuy);
		dBuyVolume = pBuyHorizontalBarList->GetNext(posSell);
		if( dVolume >= dMaxVolume)
		{
			dMaxVolume = dVolume;
			pHorizontalBarList->SetAt( FindPosition( ( int)dCondition + 1, *pHorizontalBarList), i);
		}
		if( dSellVolume >= dMaxSellVolume)
		{
			dMaxSellVolume = dSellVolume;
			pHorizontalBarList->SetAt( FindPosition( ( int)dCondition + 1, *pSellHorizontalBarList), i);
		}
		if( dBuyVolume >= dMaxBuyVolume)
		{
			dMaxBuyVolume = dBuyVolume;
			pHorizontalBarList->SetAt( FindPosition( ( int)dCondition + 1, *pBuyHorizontalBarList), i);
		}
	}
}

//*****************************************************************************************************
//����� �ʿ���� ��ǥ
//������Ʈ, �ŷ�����Ʈ ��
//******************************************************************************************************
void CNoCalculate::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// �̴�������Ʈ�� ���ؼ� ���� �Լ�
	CalculateBalancePacket(chartFormulateData, calculateData);
	// subGraph�� �ش� packet �����ϱ�
	SetGraphPacketList(chartFormulateData, pGraphCalculateData);
}

//�̴�����
void CNoCalculate::CalculateBalancePacket(const CChartFormulateData& chartFormulateData, CCalculateData& calculateData)
{
	CString strOrgPacketName = chartFormulateData.GetPacketNames();
	if(strOrgPacketName.Find("##") < 0)
		return;

	CString strPacketName = CDataConversion::GetStringData(strOrgPacketName, ",");
	if(strPacketName.IsEmpty() && !strOrgPacketName.IsEmpty())
		strPacketName = strOrgPacketName;

	if(strPacketName.Find( _MTEXT( C0_PROGRAM_TRADE)) != -1 || strPacketName.Find( _MTEXT( C0_PER_INVESTOR)) != -1)
	{
		CalExCeptionPacket(chartFormulateData, calculateData);
		return;
	}

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	CPacketList *pPacketList = chartFormulateData.GetPacketList();
	if( !pPacketList) return;

	CPacket* pPacket1 = pPacketList->GetBaseData(strPacketName);
	if(pPacket1 == NULL) return;
	CList<double, double>* pList1 = pPacket1->GetnumericDataList();
	
	CString strPacketName1 = CDataConversion::GetStringData(strPacketName, "_");
	CPacket* pPacket = pPacketList->GetBaseData(strPacketName1, false);
	CString temp = strPacketName1 + "_" + _MTEXT( C6_PREVIOUS_DAY_COMPARISON);
	CPacket* pk = pPacketList->GetBaseData(temp, true);

	if(pPacket == NULL || pk == NULL)
		return ;
	CPoint point = calculateData.GetPoint();
	CList<double, double>* pList = pPacket->GetnumericDataList();
	if(pList == NULL || pList->GetCount() <= 0)
		return ;
	POSITION pos = NULL;
	int start = 0;

	CString tempStr = strPacketName;
	CDataConversion::GetStringData(tempStr, "##");
	int nMarity = atoi(tempStr);
	CPacket* pPreRatioPacket = NULL;
	BOOL bStart = FALSE;
	CString strName = strPacketName1 + "_" + _MTEXT( C6_CHANGE_RATE);
	pPreRatioPacket = pPacketList->GetBaseData(strName, true);
	if(!pPreRatioPacket) return;

	// (2004.03.10, ��¿�, �ӽ� ����) ����Ʈ�� �� Block���� �� ���� ���������� �߰��� ���,
	//		Real�� �幮 Packet�� �������ִ� Logic�� ���� �ٸ� Packet�� ��ǥ���⿡ ������ �ִ� ������ �����ȴ�.
	//		���� �ӽ� �������� Real ���Ž� �� Packet�� �ٽ� ����ϴ� ������ �����Ѵ�.
	point.x = 0;

	if(point.x == 0)
	{
		pPacket1->ClearData(true);
		pPreRatioPacket->ClearData(TRUE);
		start = 0;
		//pPacket1->AppendData(0.0);
		pos = pList->FindIndex(start);
		bStart = TRUE;
	}
	else
		pos = pList->GetTailPosition();


	if(pos == NULL)
		return ;

	if(nMarity < 1)
		nMarity = 1;

	if(pList->GetCount() < 1)
		return ;
	double dStartPrice = 0.0;
	BOOL bRecalc = TRUE;
	
	dStartPrice = pList->GetHead();
	//�������� : Yesterday, ����ð� : CurPrice , R : ������ �׸� ��� 	
	if(strPacketName.Find("YesterPriceR##")!=-1)
		dStartPrice = pPacket->GetStandardValue(); 
	else if(strPacketName.Find("YesterPrice##")!=-1)
	{
		dStartPrice = pPacket->GetStandardValue();
		bRecalc = FALSE;
	}
	else if(strPacketName.Find("CurPrice##")!=-1)
		bRecalc = FALSE;


	start = point.x;
	double dDiv = dStartPrice / 100;
	double dData;
	for(int i = start; i < pList->GetCount() ; i++)
	{
		if(pos == NULL)
			return ;

		double realPrice = pList->GetNext(pos);
		dData = CMath::Round((realPrice - dStartPrice) / dDiv, 3);
		if(bRecalc)
			SetData(point, i, dData*nMarity, pPacket1); 
		else
			SetData(point, i, realPrice*nMarity, pPacket1);
		
		int pCnt=0,rCnt = 0;
		if(pk) pCnt = pk->GetCount();
		if(pPreRatioPacket) rCnt = pPreRatioPacket->GetCount();

		if(!bStart && pk && pPreRatioPacket && (pCnt > rCnt+1))
		{
			double tData = pPreRatioPacket->GetData(rCnt-1);
			for(int i=0;i<pCnt-rCnt-1;i++)
				pPreRatioPacket->AppendData(dData);
			rCnt = pPreRatioPacket->GetCount();
		}

		if(bStart || point.x > 0 && pk->GetCount() != pPreRatioPacket->GetCount())
			pPreRatioPacket->AppendData(dData);
	}
}

//�̴�����
void CNoCalculate::CalExCeptionPacket(const CChartFormulateData& chartFormulateData, CCalculateData& calculateData)
{
	CString strPacketName = chartFormulateData.GetPacketNames();
	strPacketName = CDataConversion::GetStringData(strPacketName, ",");

	CString tempStr = strPacketName;
	CDataConversion::GetStringData(tempStr, "##");
	int nMarity = atoi(tempStr);
	if(nMarity<1) nMarity = 1;
	CPoint point = calculateData.GetPoint();

	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	CPacketList *pPacketList = chartFormulateData.GetPacketList();
	if( !pPacketList) return;

	CPacket* pPacket1 = pPacketList->GetBaseData(strPacketName);
	if(pPacket1 == NULL)
		return ;
	CList<double, double>* pList1 = pPacket1->GetnumericDataList();
	if(point.x == 0)
		pPacket1->ClearData(true);
	CString strPacketName1 = CDataConversion::GetStringData(strPacketName, "_");
	CPacket* pPacket = pPacketList->GetBaseData(strPacketName1, false);
	
	if(pPacket == NULL)
		return ;

	CList<double, double>* pList = pPacket->GetnumericDataList();
	// (2004.02.07, ��¿�) NULL Pointer Exception�� Ȯ���Ͽ�, �����Ѵ�.
	if( !pList) return;

	POSITION pos = FindPosition(point.x, *pList);
	if(pos == NULL || pList->GetCount() <= 0)
		return ;
	double dmin, dmax;
	double dDiv;
	pPacket->GetMinMax(0, pPacket->GetCount(), dmin, dmax);
	if(fabs(dmin) >= fabs(dmax))
		dDiv = 1 / fabs(dmin);
	else
		dDiv = 1 / fabs(dmax);
	for(int i = point.x; i < pList->GetCount(); i++)
	{
		if(pos == NULL)
			return ;
		double dData = CMath::Round((pList->GetNext(pos) * dDiv), 3);
		SetData(point.x, i, dData*nMarity, pPacket1);
	}
}

//sy 2005.10.25.
void CNoCalculate::SetGraphPacketList(const CChartFormulateData& chartFormulateData,
							  CGraphCalculateData* pGraphCalculateData)
{
	if(!pGraphCalculateData || pGraphCalculateData->GetCount() <= 0)
		return;

	int nSubGraphCount = pGraphCalculateData->GetCount();
	int nSubGraphIndex = 0;
	POSITION posSubGraph = pGraphCalculateData->GetHeadPosition();
	while(posSubGraph)
	{
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetNext(posSubGraph);
		if(pSubGraphList == NULL)
			return;

		CPacket* pPacket = pSubGraphList->GetHeadPacket();
		if(pPacket == NULL) 
			InCaseNoPacketPointer(chartFormulateData, nSubGraphCount, nSubGraphIndex, pSubGraphList);

		nSubGraphIndex++;
	}
}

bool CNoCalculate::InCaseNoPacketPointer(const CChartFormulateData& chartFormulateData, 
		const int nSubGraphCount, const int nSubGraphIndex, CSubGraphPacketList* pSubGraphList)
{
	if(pSubGraphList == NULL || nSubGraphIndex < 0)
		return false;

	CPacketList* pPacketList = chartFormulateData.GetPacketList();
	if(pPacketList == NULL)
		return FALSE;

	CString strPacketNames = GetPacketNames(chartFormulateData.GetPacketNames(), nSubGraphCount, nSubGraphIndex);
	if(strPacketNames.IsEmpty())
		return false;

	int nCount = CDataConversion::GetStringDataCount(strPacketNames, ",");
	for(int i = 0; i < nCount; i++)
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ",");
		if(strPacketName.IsEmpty())
			continue;

		CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
		if(pPacket == NULL)
		{
			pSubGraphList->RemoveAll();
			break;
		}
		pSubGraphList->AddTail2( pPacket);
	}

	return (pSubGraphList->GetCount() > 0 ? true: false);
}

CString CNoCalculate::GetPacketNames(const CString& strOrgPacketNames, const int nSubGraphCount, const int nSubGraphIndex) const
{
	if(strOrgPacketNames.IsEmpty())
		return "";

	CString strPacketNames = strOrgPacketNames;
	if(strPacketNames.Right(1) != ',')
		strPacketNames += ',';

	// subGraph ������ 1���� ���
	if(nSubGraphCount <= 1 || nSubGraphIndex < 0)
		return strPacketNames;

	// subGraph ������ 2�� �̻��� ���
	for(int i = 0; i < nSubGraphCount; i++)
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ",", false);
		if(i == nSubGraphIndex)
			return strPacketName;
	}

	return "";
}
//sy end

// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
CMA::CMA( IMetaTable::EDDC_INDICATOR_ID p_eID): CIndicator( p_eID)
{
	m_pEndList	= NULL;
	m_pHighList	= NULL;
	m_pLowList	= NULL;
	m_saSubGraphName.SetSize( _MA_COUNT);

	m_pPacketList = NULL;

	// (2010/4/19 - Seung-Won, Bae) for Signal Sub Graph Index.
	m_nSignalMA1 = -1;
	m_nSignalMA2 = -1;

	// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
	m_nFirstVisibleSubGraph = 0;
}

CMA::~CMA()
{
	for( int i = 0; i < _MA_COUNT; i++)
	{
		m_dlDoubleList1[ i].RemoveAll();
		m_dlDoubleList2[ i].RemoveAll();
	}
}

void CMA::GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 0. Check NULL.
	m_pPacketList = chartFormulateData.GetPacketList();
	if( !pIndicatorInfo || !pGraphCalculateData || !m_pPacketList) return;

	int nCount = pIndicatorInfo->GetSubGraphCount();
	if( nCount <= 0) return;
	m_eCalcType = chartFormulateData.GetCalculateType();

	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
	{
		CPacket *pSignalPacket = NULL;
		// (2010/5/30 - Seung-Won, Bae) Do not use Indicator Name for Multiple Same Indicator.
		CString strIndicatorName( chartFormulateData.GetGraphName());
		if( !GetSignalPacket( m_pPacketList, strIndicatorName, pSignalPacket))
			return;
		pGraphCalculateData->SetSignalPacket( pSignalPacket);
	}

	// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
		m_nFirstVisibleSubGraph = _MA_COUNT;

	CString strSubGraphNames;
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType) strSubGraphNames = chartFormulateData.GetSubGraphNames();
	for( m_nSubGraphIndex = 0 ; m_nSubGraphIndex < nCount && m_nSubGraphIndex < _MA_COUNT; m_nSubGraphIndex++)
	{
		if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
			m_saSubGraphName[ m_nSubGraphIndex] = CDataConversion::GetStringData(strSubGraphNames, ",");
		if( m_saSubGraphName[ m_nSubGraphIndex].IsEmpty()) return;

		GetMA( chartFormulateData, pGraphCalculateData, pIndicatorInfo, calculateData);
	}

	// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
		if( _MA_COUNT == m_nFirstVisibleSubGraph)
			m_nFirstVisibleSubGraph = 0;

	// (2010/4/19 - Seung-Won, Bae) for Signal Sub Graph Index.
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
	{
		m_nSignalMA1 = -1;
		m_nSignalMA2 = -1;
		if( pIndicatorInfo->GetSignalData().IsShow())
		{
			CList< double, double> *pConditionList = pIndicatorInfo->GetCommonCalConditionList();
			if( pConditionList) if( 2 == pConditionList->GetCount())
			{
				m_nSignalMA1 = pConditionList->GetHead();
				m_nSignalMA2 = pConditionList->GetTail();
			}
			if( m_nSignalMA1 == m_nSignalMA2)
			{
				m_nSignalMA1 = -1;
				m_nSignalMA2 = -1;
			}
			if( nCount <= m_nSignalMA1) m_nSignalMA1 = -1;
			if( nCount <= m_nSignalMA2) m_nSignalMA2 = -1;
			if( 0 <= m_nSignalMA1) if( !m_bShow[ m_nSignalMA1]) m_nSignalMA1 = -1;
			if( 0 <= m_nSignalMA2) if( !m_bShow[ m_nSignalMA2]) m_nSignalMA2 = -1;
		}
		pGraphCalculateData->GetSignalPacket()->ClearData();
	}

	if( 0 <= m_nSignalMA1 && 0 <= m_nSignalMA2)
	{
		CSignal signal;
		signal.GetSignalTypesbyUsingGraphDatas( calculateData.GetPoint(),
												m_ppaMAPacket[ m_nSignalMA1]->GetnumericDataList(),
												m_ppaMAPacket[ m_nSignalMA2]->GetnumericDataList(),
												pGraphCalculateData->GetSignalPacket());
	}
}

inline void CMA::GetMA(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData)
{
	// 2. Get the Environment
	CPoint point = calculateData.GetPoint();
	if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
	{
		// 2.1 Bind Packet to Graph.
		m_ppaMAPacket[ m_nSubGraphIndex] = NULL;
		CSubGraphPacketList* pSubGraphList = pGraphCalculateData->GetAt( m_nSubGraphIndex);
		if( !pSubGraphList) return;
		m_ppaMAPacket[ m_nSubGraphIndex] = pSubGraphList->GetHeadPacket();
		if( !m_ppaMAPacket[ m_nSubGraphIndex])
		{
			// (2010/5/30 - Seung-Won, Bae) Use Graph Name for Multiple Same Indicator.
			// 130612 �̻�� : ���� ȭ��ǥ �ȳ�Ÿ���� ���� ����(ISSUE NO - 6817).
			// 130814 �̻�� : ��Ʈ�� �ü��� �̻��ϰ� ������ ����(ISSUE NO - 7435). �� �̽������� ���� ����, 6817 ���󺹱�.
			m_ppaMAPacket[ m_nSubGraphIndex] = m_pPacketList->GetBaseData( chartFormulateData.GetGraphName() + "_" + m_saSubGraphName[ m_nSubGraphIndex], false);
//			m_ppaMAPacket[ m_nSubGraphIndex] = m_pPacketList->GetBaseData( m_saSubGraphName[ m_nSubGraphIndex], false);

			m_ppaMAPacket[ m_nSubGraphIndex]->ClearData();
			pSubGraphList->AddTail2( m_ppaMAPacket[ m_nSubGraphIndex]);
		}

		// 2.2 Get Input Data List
		CPacket *pSourcePacket = NULL;
		CString strPacketNames = pIndicatorInfo->GetPacketNames( m_nSubGraphIndex);
		if( strPacketNames.IsEmpty() || IMetaTable::EDDC_OCX_PRICEMACHART != m_eID)
			strPacketNames = pIndicatorInfo->GetCommonPacketNames();
		m_naInputPacketType[ m_nSubGraphIndex] = 1;
		// (2010/12/2 - Seung-Won, Bae) for Pseudo Packet.
		CString strPseudoName, strHighPacket( _MTEXT( C0_HIGH)), strLowPacket( _MTEXT( C0_LOW)), strClosePacket( _MTEXT( C0_CLOSE));
		CString strHLCPacket( _MTEXT( C0_COMBINE_HIGH_LOW_CLOSE)), strHLPacket( _MTEXT( C0_COMBINE_HIGH_LOW));
// 		if( 0 <= strPacketNames.Find( _MTEXT( C0_PSEUDO_CLOSE))) strPseudoName = _MTEXT( C0_PSEUDO_CLOSE);
// 		if( strPseudoName.IsEmpty())
// 			if( 0 <= strPacketNames.Find( _MTEXT( C0_PSEUDO_HIGH))) strPseudoName = _MTEXT( C0_PSEUDO_HIGH);
// 		if( strPseudoName.IsEmpty())
// 			if( 0 <= strPacketNames.Find( _MTEXT( C0_PSEUDO_LOW))) strPseudoName = _MTEXT( C0_PSEUDO_LOW);
// 		if( strPseudoName.IsEmpty())
// 			if( 0 <= strPacketNames.Find( _MTEXT( C0_PSEUDO_OPEN))) strPseudoName = _MTEXT( C0_PSEUDO_OPEN);
// 		if( !strPseudoName.IsEmpty())
// 		{
// 			int nLeft = strPacketNames.Find( strPseudoName);
// 			int nRight = nLeft + strPseudoName.GetLength();
// 			CString strPseudoLeft = strPacketNames.Left( nLeft);
// 			CString strPseudoRight = strPacketNames.Mid( nRight);
// 			nLeft = strPseudoLeft.ReverseFind( '_');
// 			nRight = strPseudoRight.Find( '_');
// 			if( 0 <= nLeft && 0 <= nRight)
// 			{
// 				strPseudoLeft = strPseudoLeft.Mid( nLeft);
// 				strPseudoRight = strPseudoRight.Left( nRight + 1);
// 				
// 				strHighPacket	= strPseudoLeft + _MTEXT( C0_PSEUDO_HIGH) + strPseudoRight;
// 				strLowPacket	= strPseudoLeft + _MTEXT( C0_PSEUDO_LOW) + strPseudoRight;
// 				strClosePacket	= strPseudoLeft + _MTEXT( C0_PSEUDO_CLOSE) + strPseudoRight;
// 				strHLCPacket	= strPseudoLeft + _MTEXT( C0_PSEUDO_HIGH) + "/" + _MTEXT( C0_PSEUDO_LOW) + "/" + _MTEXT( C0_PSEUDO_CLOSE) + strPseudoRight;
// 				strHLPacket		= strPseudoLeft + _MTEXT( C0_PSEUDO_HIGH) + "/" + _MTEXT( C0_PSEUDO_LOW) + strPseudoRight;
// 			}
// 		}
		if( 0 <= strPacketNames.Find( strHighPacket) && 0 <= strPacketNames.Find( strLowPacket))
		{
			if( 0 <= strPacketNames.Find( strClosePacket))
			{
				m_naInputPacketType[ m_nSubGraphIndex] = 3;
				pSourcePacket = m_pPacketList->GetBaseData( strHLCPacket, false);
			}
			else
			{
				m_naInputPacketType[ m_nSubGraphIndex] = 2;
				pSourcePacket = m_pPacketList->GetBaseData( strHLPacket, false);
			}

			m_pHighList = GetnumericDataList( m_pPacketList->GetBaseData( strHighPacket));
			m_pLowList = GetnumericDataList( m_pPacketList->GetBaseData( strLowPacket));
			m_pEndList = GetnumericDataList( m_pPacketList->GetBaseData( strClosePacket));
		}
		else
		{
			if( 0 <= strPacketNames.Find( ',')) strPacketNames = CDataConversion::GetStringData(strPacketNames, ",");
			pSourcePacket = m_pPacketList->GetBaseData( strPacketNames, true);
		}
		m_pdlaSourceList[ m_nSubGraphIndex] = CIndicator::GetnumericDataList( pSourcePacket);

		// 2.3 Get Calculate Condition.
		m_daMAPeriod[ m_nSubGraphIndex] = 99999;
		m_naMAType[ m_nSubGraphIndex] = 0;
		CList< double, double> *pConditionList = pIndicatorInfo->GetCalConditionList( m_nSubGraphIndex);
		if( pConditionList) if( 2 <= pConditionList->GetCount())
		{
			m_daMAPeriod[ m_nSubGraphIndex] = pConditionList->GetHead();
			m_naMAType[ m_nSubGraphIndex] = pConditionList->GetTail();
		}

		// Get StartIndex
		int nStartIndex = m_daMAPeriod[ m_nSubGraphIndex] - 1;
		switch( m_naMAType[ m_nSubGraphIndex])
		{
			case 2:	// EMA
					nStartIndex = 0;
					break;
					// AMA
			case 7:	nStartIndex = m_daMAPeriod[ m_nSubGraphIndex];
					break;
		}

		// 2.4 Set Packet Type
		// (2009/9/19 - Seung-Won, Bae) for FX/WorldOn Index Digit.
		CString strDataType = TYPE_TWODECIMAL, strScaleType = TYPE_TWODECIMAL;
		if( pSourcePacket)
		{
			strScaleType = pSourcePacket->GetType( false);
			nStartIndex += pSourcePacket->GetStartPeriod();
		}
		if( CChartInfo::HTS != chartFormulateData.GetChartMode()) strDataType = strScaleType;
		SetPacketTypes( m_ppaMAPacket[ m_nSubGraphIndex], strDataType, strScaleType, nStartIndex);

		m_bShow[ m_nSubGraphIndex] = !pIndicatorInfo->GetSubGraphHiding( m_nSubGraphIndex);
		if( !m_bShow[ m_nSubGraphIndex]) m_ppaMAPacket[ m_nSubGraphIndex]->ClearData();
		// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
		else if( m_nSubGraphIndex < m_nFirstVisibleSubGraph) m_nFirstVisibleSubGraph = m_nSubGraphIndex;

		if( IsIndicatorCalculated( chartFormulateData.GetCalculateType(), m_ppaMAPacket[ m_nSubGraphIndex])) return;
	}
	else if( CCalculateBaseData::REALDATA_CAL != m_eCalcType) return;
	if( !m_pdlaSourceList[ m_nSubGraphIndex]) return;
	if( !m_ppaMAPacket[ m_nSubGraphIndex]) return;
	if( !m_bShow[ m_nSubGraphIndex]) return;
	if( m_daMAPeriod[ m_nSubGraphIndex] < 1) return;

	// for Combinated Input Packet.
	if( 1 < m_naInputPacketType[ m_nSubGraphIndex] && m_pEndList && m_pHighList && m_pLowList)
	{
		int nStartIndex = point.x;
		int nSourceLastIndex = m_pdlaSourceList[ m_nSubGraphIndex]->GetCount() - 1;
		if( CCalculateBaseData::TRDATA_CAL == m_eCalcType)
		{
			if( nSourceLastIndex + 1 != m_pPacketList->GetMaxCount())	// It can be bigger or smaller.
			{
				nStartIndex = 0;
				nSourceLastIndex = -1;
				m_pdlaSourceList[ m_nSubGraphIndex]->RemoveAll();
			}
			else nStartIndex = nSourceLastIndex + 1;
		}
		// (2010/4/15 - Seung-Won, Bae) Caution! It can be multiple calculated on Real Time.
		else if( nStartIndex <= nSourceLastIndex) for( ; nStartIndex <= nSourceLastIndex; nSourceLastIndex--)
				m_pdlaSourceList[ m_nSubGraphIndex]->RemoveTail();
		if( nSourceLastIndex < nStartIndex - 1) nStartIndex = nSourceLastIndex + 1;

		double end = 0.0, high = 0.0, low = 0.0;
		POSITION endpos		= CIndicator::FindPosition( nStartIndex, *m_pEndList);
		POSITION highpos	= CIndicator::FindPosition( nStartIndex, *m_pHighList);
		POSITION lowpos		= CIndicator::FindPosition( nStartIndex, *m_pLowList);
		while( endpos && highpos && lowpos)
		{
			end		= m_pEndList->GetNext( endpos);
			high	= m_pHighList->GetNext( highpos);
			low		= m_pLowList->GetNext( lowpos);

			// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
			//		if Error, Set All with Close for Base or Market Close Price.
			if( high < end) high = low = end;

			if( 3 == m_naInputPacketType[ m_nSubGraphIndex])
					m_pdlaSourceList[ m_nSubGraphIndex]->AddTail( ( end + high + low) / 3);
			else	m_pdlaSourceList[ m_nSubGraphIndex]->AddTail( ( high + low) / 2);
		}
	}
	int nSourceCount = m_pdlaSourceList[ m_nSubGraphIndex]->GetCount();
	if( nSourceCount <= 0 || nSourceCount <= point.x) return;
	if (m_saSubGraphName[ m_nSubGraphIndex].CompareNoCase(calculateData.GetPacketNamesForSave()) != 0)
		return;

	switch( m_naMAType[ m_nSubGraphIndex])
	{
		case 0:
		case 1:	GetSimpleMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 2:	GetExponentialMA(	point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 3:	GetWeightedMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 4:	GetGeometricMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 5:	GetHarmonicMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 6:	GetTriangularMA(	point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
		case 7:	GetAdaptiveMA(		point.x, m_daMAPeriod[ m_nSubGraphIndex], m_pdlaSourceList[ m_nSubGraphIndex], m_ppaMAPacket[ m_nSubGraphIndex]);
				break;
	}
}

inline void	CMA::SetMAValue( const int &p_nStartIndex, const int &nUpdateIndex, const double &dMA, CPacket *pMAPacket)
{
	if( 0 == p_nStartIndex)
	{
		if( nUpdateIndex >= pMAPacket->GetCount())
			pMAPacket->AppendData( dMA);
		else
			pMAPacket->UpdateData( dMA, nUpdateIndex, false);
	}
	else
		pMAPacket->UpdateData( dMA, nUpdateIndex, false);
}

void CMA::GetSimpleMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	double dSource = 0.0;
	double dSum = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA = pMAPacket->GetData( nIndex - 1);
		dSum = dMA * dPeriod;
	}

	// nIndex : [0 ~ dPeriod - 1]
	if( nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dSum += dSource;
		dMA = dSum / dPeriod;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	double dPreSource = 0.0;
	for( ; psnSource && psnPreSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dPreSource = pdlSourceList->GetNext( psnPreSource);
		dSum += dSource - dPreSource;
		dMA = dSum / dPeriod;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}
}

void CMA::GetExponentialMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	double dSource = 0.0;
	double dSmoothConst = 2 / ( dPeriod + 1);
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA = pMAPacket->GetData( nIndex - 1);
	}

	// nIndex : [0 ~ 0]
	if( 0 == nIndex)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dMA = dSource;
		SetMAValue( p_nStartIndex, nIndex++, dMA, pMAPacket);
	}

	// nIndex : [1 ~]
	for( ; psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dMA = dMA * ( 1 - dSmoothConst) + dSource * dSmoothConst;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}
}

void CMA::GetWeightedMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	CList< double, double> &dlSum = m_dlDoubleList1[ m_nSubGraphIndex];
	double dSource = 0.0;
	double dSum = 0.0;
	double dWeightedSum = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA = pMAPacket->GetData( nIndex - 1);
		dWeightedSum = dMA / 2 * ( dPeriod * ( dPeriod + 1));

		// dSum
		POSITION psnSum = CIndicator::FindPosition( nIndex - 1, dlSum);
		dSum = dlSum.GetAt( psnSum);
		while( nIndex < dlSum.GetCount())
			dlSum.RemoveTail();
	}
	else
	{
		dlSum.RemoveAll();
	}

	// nIndex : [0 ~ dPeriod - 1]
	if( nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dWeightedSum = dWeightedSum + dSource * ( nIndex + 1);
		dMA = dWeightedSum * 2 / ( dPeriod * ( dPeriod + 1));
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dSum = dSum + dSource;
		dlSum.AddTail( dSum);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	double dPreSource = 0.0;
	for( ; psnSource && psnPreSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dPreSource = pdlSourceList->GetNext( psnPreSource);
		dWeightedSum = dWeightedSum - dSum + dSource * dPeriod;
		dMA = dWeightedSum * 2 / ( dPeriod * ( dPeriod + 1));
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dSum = dSum + dSource - dPreSource;
		dlSum.AddTail( dSum);
	}
}

void CMA::GetGeometricMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	double dSource = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
			dMA = pMAPacket->GetData( nIndex - 1);
	else	dMA = 1.0;

	// nIndex : [0 ~ dPeriod - 1]
	if( nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dMA *= pow( dSource, 1/dPeriod);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	double dPreSource = 0.0;
	for( ; psnSource && psnPreSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dPreSource = pdlSourceList->GetNext( psnPreSource);
		dMA *= pow( dSource / dPreSource, 1/dPeriod);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}
}

void CMA::GetHarmonicMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	double dSource = 0.0;
	double dInverseSum = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA = pMAPacket->GetData( nIndex - 1);
		dInverseSum = 1 / dMA * dPeriod;
	}

	// nIndex : [0 ~ dPeriod - 1]
	if( nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dInverseSum += 1 / dSource;
		dMA = 1 / ( dInverseSum / dPeriod);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	double dPreSource = 0.0;
	for( ; psnSource && psnPreSource; nIndex++)
	{
		dSource = pdlSourceList->GetNext( psnSource);
		dPreSource = pdlSourceList->GetNext( psnPreSource);
		dInverseSum += 1 / dSource - 1 / dPreSource;
		dMA = 1 / ( dInverseSum / dPeriod);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);
	}
}

void CMA::GetTriangularMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	const double dPeriod1 = int( dPeriod / 2 + 0.5);
	const double dPeriod2 = dPeriod1 + ( dPeriod1 * 2 == dPeriod ? 1 : 0);

	int nIndex = p_nStartIndex;
	POSITION psnSource = CIndicator::FindPosition( nIndex, *pdlSourceList);
	CList< double, double> &dlMA1 = m_dlDoubleList1[ m_nSubGraphIndex];
	double dSource = 0.0;
	double dSum1 = 0.0, dSum2 = 0.0;
	double dMA1 = 0.0, dMA2 = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		dMA2 = pMAPacket->GetData( nIndex - 1);
		dSum2 = dMA2 * dPeriod2;

		// dMA1
		POSITION psnMA1 = CIndicator::FindPosition( nIndex - 1, dlMA1);
		dMA1 = dlMA1.GetAt( psnMA1);
		while( nIndex < dlMA1.GetCount())
			dlMA1.RemoveTail();
		dSum1 = dMA1 * dPeriod1;
	}
	else
	{
		dlMA1.RemoveAll();
	}

	// nIndex : [0 ~ dPeriod1 - 2]
	if( nIndex < dPeriod1 - 1) for( ; nIndex < dPeriod1 - 1 && psnSource; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);

		dSum1 += dSource;
		dMA1 = dSum1 / dPeriod1;
		dSum2 = 0;
		dMA2 = 0;
		SetMAValue( p_nStartIndex, nIndex, dMA2, pMAPacket);

		dlMA1.AddTail( dMA1);
	}

	// nIndex : [dPeriod1 - 1]
	if( dPeriod1 - 1 == nIndex) for( ; dPeriod1 - 1 == nIndex && psnSource; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);

		dSum1 += dSource;
		dMA1 = dSum1 / dPeriod1;
		dSum2 += dMA1;
		dMA2 = dSum2 / dPeriod2;
		SetMAValue( p_nStartIndex, nIndex, dMA2, pMAPacket);

		dlMA1.AddTail( dMA1);
	}

	// nIndex : [dPeriod1 ~ dPeriod - 1]
	POSITION psnPreSource = CIndicator::FindPosition( nIndex - dPeriod1, *pdlSourceList);
	double dPreSource = 0.0;
	if( dPeriod1 <= nIndex && nIndex < dPeriod) for( ; nIndex < dPeriod && psnSource && psnPreSource; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);
		dPreSource	= pdlSourceList->GetNext( psnPreSource);

		dSum1 += dSource - dPreSource;
		dMA1 = dSum1 / dPeriod1;
		dSum2 += dMA1;
		dMA2 = dSum2 / dPeriod2;
		SetMAValue( p_nStartIndex, nIndex, dMA2, pMAPacket);

		dlMA1.AddTail( dMA1);
	}

	// nIndex : [dPeriod ~]
	POSITION psnPreMA1 = CIndicator::FindPosition( nIndex - dPeriod2, dlMA1);
	POSITION psnLastMA1 = NULL;
	double dPreMA1 = 0.0;
	for( ; psnSource && psnPreSource && psnPreMA1; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);
		dPreSource	= pdlSourceList->GetNext( psnPreSource);
		dPreMA1		= dlMA1.GetNext( psnPreMA1);

		dSum1 += dSource - dPreSource;
		dMA1 = dSum1 / dPeriod1;
		dSum2 += dMA1 - dPreMA1;
		dMA2 = dSum2 / dPeriod2;
		SetMAValue( p_nStartIndex, nIndex, dMA2, pMAPacket);

		psnLastMA1 = dlMA1.AddTail( dMA1);
		if( 1 == dPeriod2) psnPreMA1 = psnLastMA1;
	}
}

void CMA::GetAdaptiveMA( const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket)
{
	if( !pdlSourceList) return;

	int nIndex = p_nStartIndex;
	POSITION psnSource = NULL;
	CList< double, double> &dlDiff		= m_dlDoubleList1[ m_nSubGraphIndex];
	CList< double, double> &dlDiffSum	= m_dlDoubleList2[ m_nSubGraphIndex];
	double dSource = 0.0;
	double dLastSource = 0.0;
	double dDiffSum = 0.0;
	double dMA = 0.0;

	// Get Pre State
	if( 0 < nIndex)
	{
		psnSource	= CIndicator::FindPosition( nIndex - 1, *pdlSourceList);
		dLastSource	= pdlSourceList->GetNext( psnSource);

		while( nIndex < dlDiff.GetCount())
			dlDiff.RemoveTail();

		POSITION psnDiffSum = CIndicator::FindPosition( nIndex - 1, dlDiffSum);
		dDiffSum = dlDiffSum.GetAt( psnDiffSum);
		while( nIndex < dlDiffSum.GetCount())
			dlDiffSum.RemoveTail();

		dMA = pMAPacket->GetData( nIndex - 1);
	}
	// nIndex : [0]
	else
	{
		dlDiff.RemoveAll();
		dlDiffSum.RemoveAll();

		psnSource	= pdlSourceList->GetHeadPosition();
		dSource		= pdlSourceList->GetNext( psnSource);
		dMA = dSource;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dLastSource = dSource;
		dlDiff.AddTail( 0.0);
		dlDiffSum.AddTail( 0.0);

		nIndex++;
	}

	// nIndex : [1 ~ dPeriod]
	double dDiff = 0.0;
	if( nIndex <= dPeriod) for( ; nIndex <= dPeriod && psnSource; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);

		dDiff = fabs( dSource - dLastSource);
		dDiffSum += dDiff;
		dMA = dSource;
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dLastSource = dSource;
		dlDiff.AddTail( dDiff);
		dlDiffSum.AddTail( dDiffSum);
	}

	// nIndex : [dPeriod + 1~]
	POSITION psnPreSource	= CIndicator::FindPosition( nIndex - dPeriod, *pdlSourceList);
	POSITION psnDiff		= CIndicator::FindPosition( nIndex - dPeriod, dlDiff);
	POSITION psnLastDiff	= NULL;
	double dPreSource = 0.0, dSmoothFactor = 0.0, dEffRatio = 0.0, dPreDiff = 0.0;
	for( ; psnSource && psnPreSource && psnDiff; nIndex++)
	{
		dSource		= pdlSourceList->GetNext( psnSource);
		dPreSource	= pdlSourceList->GetNext( psnPreSource);
		dPreDiff	= dlDiff.GetNext( psnDiff);

		dDiff = fabs( dSource - dLastSource);
		dDiffSum += dDiff - dPreDiff;
		dEffRatio = fabs( dSource - dPreSource) / ( 0 == dDiffSum ? 1 : dDiffSum);
		dSmoothFactor = dEffRatio * ( ( 2. / 3.) - ( 2. / 31.)) + 2. / 31.;
		dSmoothFactor *= dSmoothFactor;
		dMA += dSmoothFactor * ( dSource - dMA);
		SetMAValue( p_nStartIndex, nIndex, dMA, pMAPacket);

		dLastSource = dSource;
		psnLastDiff = dlDiff.AddTail( dDiff);
		dlDiffSum.AddTail( dDiffSum);

		if( 1 == dPeriod) psnDiff = psnLastDiff;
	}
}
