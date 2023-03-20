// ViewMinMaxCalculator.cpp: implementation of the CViewMinMaxCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewMinMaxCalculator.h"

#include <assert.h>
#include "../Include_Chart/Dll_Load/PacketList.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/PacketBase.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/DataMath.h"
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/PacketPointerList.h"
#include "CalculatorData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CViewMinMaxCalculator

// public ===============================================================================
// view min, max �� ��������
CDisplayDataMinMax CViewMinMaxCalculator::GetViewHorzMinMax(const CHorzDisplayDataMinMaxInputData& inputData) const
{
	// "�����" �� ���
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	if( !strncmp( _MTEXT( C6_PERCENT), inputData.GetPacketName(), 6))
		return CDisplayDataMinMax(0.0, 100.0);
	// "MarketProfile" �� ���
	else if(inputData.GetPacketName() == _MTEXT( C3_MARKET_PROFILE))
		return GetViewHorzMinMax_MarketProfile(inputData);

	// �Ϲ��� scale�� ���Ѵ�.
	CDisplayDataMinMax displayDataMinMax;
	GetPacketMinMax(inputData.GetPacket(), inputData.GetDisplayAttributes(), false, displayDataMinMax, FALSE);
	return displayDataMinMax;
}

// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
bool CViewMinMaxCalculator::GetViewVertMinMax(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine)
{
	if(!IsViewVertMinMax(inputData.GetSubGraphPacketList()))
		return false;

	// "�����" �� ���
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	if( !strncmp( _MTEXT( C6_PERCENT), inputData.GetScaleData(), 6))
	{
		displayDataMinMax = CDisplayDataMinMax(0.0, 100.0);
		return true;
	}

	// "��ǥ Data" �� ���
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	if( !strncmp( _MTEXT( C6_INDICATOR_DATA), inputData.GetScaleData(), 9))
		return GetViewVertMinMax_Indicate(inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);

	// "packet" �� ���
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	return GetViewVertMinMax_Etc(inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);
}


// private ==============================================================================
// ���� scale�� min/max�� �������� ����
bool CViewMinMaxCalculator::IsViewVertMinMax(CSubGraphPacketList* pSubGraphPacketList) const
{
	return (pSubGraphPacketList != NULL && pSubGraphPacketList->GetCount() > 0);
}

// --------------------------------------------------------------------------------------
// "MarketProfile" �� ���
CDisplayDataMinMax CViewMinMaxCalculator::GetViewHorzMinMax_MarketProfile(const CHorzDisplayDataMinMaxInputData& inputData) const
{
/*
	if(inputData.GetPacket() == NULL)
		return CDisplayDataMinMax(0.0, 0.0);

	CMPPacket* pMPPacket = (CMPPacket*) inputData.GetPacket();
	CList<CMProfile*, CMProfile*>* pMProfileList = pMPPacket->GetMarketProfileList();
	if(pMProfileList == NULL || pMProfileList->GetCount() <= 0)
		return CDisplayDataMinMax(0.0, 0.0);
*/
	double dMin = 0.0;
	double dMax = 0.0;
/*
	CMProfile* pMProfile = pMProfileList->GetHead();
	if( pMProfile != NULL) dMin = pMProfile->GetDate();
	pMProfile = pMProfileList->GetTail();
	if( pMProfile != NULL) dMax = pMProfile->GetDate();
*/
	return 	CDisplayDataMinMax( dMin, dMax);
}

// --------------------------------------------------------------------------------------
// "��ǥ Data" �� ���
// -> ��ǥ������ Min/Max�� ���Ѵ�.
// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
bool CViewMinMaxCalculator::GetViewVertMinMax_Indicate(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine)
{
	// �������� packet�� min/max �� ��� - "Zig Zag", "��������Ʈ"
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	if( g_iMetaTable.IsViewVertMinMax_EndPacket(inputData.GetIndicatorName()))
		return GetViewVertMinMax_EndPacket(inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);
	// "���ð�" �� ���
	else if( g_iMetaTable.IsScaleGroup_InverseLine(inputData.GetIndicatorName()))
		return GetViewVertMinMax_HeadPacket(inputData, displayDataMinMax, p_bWithFullRange);
	// (2004.02.06, ��¿�) ���� ���� P&F�� ��� ������ Ȯ���Ѵ�.
	//		1. Ư�� �Ϲ� P&F Group�� �ش�ǹǷ� �Ϲ� P&F���� �켱 Ȯ��, ó���Ѵ�.
	//		2. ���� ���� P&F Chart�� Packet�� 'FUPF_ML�ð�', 'FUPF_ML����', 'FUPF_�����ڰ�'�� �����Ǿ� �ִ�.
	//		3. �̵� ��� Packet�� ����(Price)�� Y Scale�� �ش������,
	//			'FUPF_�����ڰ�'�� ���ο� ���ڷ� ����Ǵ� ���ݸ��� ��ô� ������,
	//			���ڰ� ������� �ʴ� ��� 0�� ���� Minimum Scale�� ������ �� �� �ִ�.
	//		4. MinMax�� '0'�� �������� �ʵ��� �����Ѵ�.
	else if(inputData.GetIndicatorName() == _MTEXT( C2_FUTURE_OPTION_P_AND_F))
		return GetViewVertMinMax_FuPAndF( inputData, displayDataMinMax) ? true : false;
	// "P&F" �� ���
	else if( g_iMetaTable.IsDrawGroup_PAndF(inputData.GetIndicatorName()))
		return GetViewVertMinMax_PAndF(inputData, displayDataMinMax, p_bWithFullRange);
	// "���Ź�" �� ���
// (2006.12.06) Seunghwan Ahn
	else if( g_iMetaTable.IsVolumeForSale(inputData.GetIndicatorName()))
// (2006.12.06) Seunghwan Ahn
//	else if(inputData.GetIndicatorName() == _MTEXT( C2_OVERHANGING_SUPPLY))
		return GetViewVertMinMax_Sale(inputData, displayDataMinMax, p_bWithFullRange);
	// "MarketProfile" �� ���
	else if(inputData.GetIndicatorName() == _MTEXT( C3_MARKET_PROFILE))
		return GetViewVertMinMax_MarketProfile(inputData, displayDataMinMax);
	// ( + ) percent ������ ��� - "�ɸ���ǥ", "RSI", "Stochastics Fast", "Stochastics Slow"
	else if( g_iMetaTable.IsScaleGroup_PlusPercent(inputData.GetIndicatorName()))
	{
		displayDataMinMax = CDisplayDataMinMax(0.0, 100.0);
		return true;
	}
	// (-) percent ������ ��� - "William's %R"
	else if( g_iMetaTable.IsScaleGroup_MinusPercent(inputData.GetIndicatorName()))
	{
		displayDataMinMax = CDisplayDataMinMax(-100.0, 0.0);
		return true;
	}
	// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�
	else if(inputData.GetIndicatorName() == _MTEXT( C2_DISPERSION))
		return GetViewVertMinMax_DisperSion(inputData, displayDataMinMax, p_bWithFullRange);
	// (2007/1/15 - Seung-Won, Bae) Line Chart of Price Chart
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	if( ( inputData.GetSubGraphType() == CGraphBaseData::Bong_Type
			&& inputData.GetSubGraphStyle().GetGraphBongStyle() == CGraphBaseData::LineBong)
		|| ( inputData.GetSubGraphType() == CGraphBaseData::Line_Type
			&& inputData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::ComparisonLine))
		return GetViewVertMinMax_EndPacket( inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);

	return GetViewVertMinMax_Indicate_General_Gap(inputData, displayDataMinMax, p_bWithFullRange);
}

bool CViewMinMaxCalculator::GetViewVertMinMax_HeadPacket(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CPacket* pPacket = inputData.GetSubGraphPacketList()->GetHeadPacket();
	return GetPacketMinMax(pPacket, inputData.GetDisplayAttributes(), false, displayDataMinMax, p_bWithFullRange);
}

// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
bool CViewMinMaxCalculator::GetViewVertMinMax_EndPacket(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine)
{
	CPacket* pPacket = inputData.GetSubGraphPacketList()->GetTailPacket();
	if( !GetPacketMinMax(pPacket, inputData.GetDisplayAttributes(), false, displayDataMinMax, p_bWithFullRange)) return false;
	// (2007/1/14 - Seung-Won, Bae) Include Base Line Value
	if( pPacket->IsUseStandardValue())
	{
		double dMin = displayDataMinMax.GetDisplayMin();
		double dMax = displayDataMinMax.GetDisplayMax();
		// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
		if(bDrawBaseLine)
		{
			double dBase = pPacket->GetStandardValue();
			double dGap = (dMax - dMin) * (20. / 100.);
			if( dBase < dMin && dBase > dMin - dGap) dMin = dBase;
			if( dMax < dBase && dMax + dGap > dBase) dMax = dBase;
		}
		displayDataMinMax.SetDisplayData( dMin, dMax);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.02.06
//
//	����	:	���� ���� P&F Chart
//
//	����	:	1. ���� ���� P&F Chart�� Packet�� 'FUPF_ML�ð�', 'FUPF_ML����', 'FUPF_�����ڰ�'�� �����Ǿ� �ִ�.
//				2. �̵� ��� Packet�� ����(Price)�� Y Scale�� �ش������,
//					'FUPF_�����ڰ�'�� ���ο� ���ڷ� ����Ǵ� ���ݸ��� ��ô� ������,
//					���ڰ� ������� �ʴ� ��� 0�� ���� Minimum Scale�� ������ �� �� �ִ�.
//				3. MinMax�� '0'�� �������� �ʵ��� �����Ѵ�.
//
//	����	:	1. DoesViewMinMaxWithoutZero()�� �̿��Ͽ� GetViewVertMinMax_Indicate_General()����
//					ó���Ϸ��� �Ͽ�����, DrawStyle�� 0�� MP Chart�� ���е��� �ʾ� ���� Routine���� �����Ѵ�.
//
//	����	:	(2004.02.22, ��¿�) ��/�Ʒ��� 1ȣ��(0.05)���� �߰��Ͽ� ������ ���̵��� �Ѵ�.
//				(2004.02.24, ��¿�) �ǽð� Tick P&F�� Scale�� �޺��� �����ϱ� ���Ͽ�,
//					ù �� Packet�� ù Data�� ���� Min/Max ���� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
BOOL CViewMinMaxCalculator::GetViewVertMinMax_FuPAndF( const CVertDisplayDataMinMaxInputData &inputData, CDisplayDataMinMax &displayDataMinMax)
{
	// 1. Sub Graph Packet List�� Display Attribute�� ���Ѵ�.
	CSubGraphPacketList *pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if( !pSubGraphPacketList) return false;
	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();

	// 2. �� Packet�� ��� Ȯ���Ͽ� Scale�� Ȯ���Ѵ�.
	// (2004.02.24, ��¿�) �ǽð� Tick P&F�� Scale�� �޺��� �����ϱ� ���Ͽ�,
	//		ù �� Packet�� ù Data�� ���� Min/Max ���� �����Ѵ�.
	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	if( !pos) FALSE;
	CPacket* pPacket = pSubGraphPacketList->GetNext( pos);
	int nMinX100 = int( pPacket->GetData( 0) * 100);
	if( !pos) FALSE;
	pPacket = pSubGraphPacketList->GetNext( pos);
	int nMaxX100 = int( pPacket->GetData( 0) * 100);

	// (2004.02.22, ��¿�) ��/�Ʒ��� 1ȣ��(0.05)���� �߰��Ͽ� ������ ���̵��� �Ѵ�.
	displayDataMinMax.SetDisplayData( ( nMinX100 - 5) / 100., ( nMaxX100 + 5) / 100.);

	return TRUE;
}

bool CViewMinMaxCalculator::GetViewVertMinMax_PAndF(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CSubGraphPacketList* pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return false;

	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();
	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = pSubGraphPacketList->GetNext(pos);
		assert(pPacket != NULL);
		if(pPacket->GetName().Find( _MTEXT( C0_OPEN)) < 0)
			GetViewVertMinMax_Packet(pPacket, displayAttributes, false, displayDataMinMax, p_bWithFullRange);
	}

	return displayDataMinMax.IsInitialized() ? true : false;
}

bool CViewMinMaxCalculator::GetViewVertMinMax_Sale(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	//2004.05.12. -> "���Ź�" �� ÷������ ���� ���� ���.
	//				 ��, vertScaleGroup�� �������϶� ���ȴ�.
	// "���Ź�" -> packet : <���Ź�>, <���Ź�_���ݴ�>
	// -> Min/Max : <���Ź�_���ݴ�> �θ� ���Ѵ�.
	// (2006/12/11 - Seung-Won, Bae) "���Ź�_���ݴ�" is second packet.
	CSubGraphPacketList *pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if( !pSubGraphPacketList) return false;
	const CList < CPacket *, CPacket *> &packetList = pSubGraphPacketList->GetSubGraphPacketList();
	if( packetList.GetCount() < 2) return false;
	POSITION psnPacket = packetList.FindIndex( 1);
	if( !psnPacket) return false;
	CPacket *pPacket = packetList.GetAt( psnPacket);
	if( !pPacket) return false;

	// ���۰� ���� Packet Data ������ �Ѵ�.
	// -> ���Ź� ���簡/�ִ밪 index�� ��Ÿ���� ���� ���� �������� data ������ ���Ѵ�.
	CDisplayAttributes displayAttributes(0, pPacket->GetCount() -1);
	return GetViewVertMinMax_Packet(pPacket, displayAttributes, false, displayDataMinMax, p_bWithFullRange);
}

bool CViewMinMaxCalculator::GetViewVertMinMax_MarketProfile(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax)
{
	CMPPacket* pMPPacket = (CMPPacket*) inputData.GetSubGraphPacketList()->GetHeadPacket();
	if(pMPPacket == NULL)
		return false;

	// (2004.01.20, ��¿�) �� ���ݱ����� ��ǥ�� ���ִ� ��ü ���ݹ��ֺ��� ���� �������� ����.
	//		��ü ���ݹ��ָ� ���ϴ� Interface�� �����Ѵ�.
	//		����, Double�� �ƴ� IntX100�� �̿��Ѵ�.
	int nMinX100, nMaxX100;
	if( !pMPPacket->GetPriceScaleX100( nMinX100, nMaxX100)) return false;

	displayDataMinMax.SetDisplayData( ( nMinX100 - 15) / 100., ( nMaxX100 + 15) / 100.);
	return true;
}

// Min/Max �� gap�� �ش�.
bool CViewMinMaxCalculator::GetViewVertMinMax_Indicate_General_Gap(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	if(!GetViewVertMinMax_Indicate_General(inputData, displayDataMinMax, p_bWithFullRange))
		return false;

	// Min/Max �� gap �� �ش�.
	if(!DoesDisplayDataMinMaxInGap(inputData))
		return true;

	double dGap = displayDataMinMax.GetCount() * 0.05;

	displayDataMinMax.Inflate(dGap, dGap);

	return true;
}

bool CViewMinMaxCalculator::GetViewVertMinMax_Indicate_General(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CSubGraphPacketList* pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return false;

	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();
	bool bWithoutZero = DoesViewMinMaxWithoutZero(inputData.GetSubGraphData());
	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = pSubGraphPacketList->GetNext(pos);
		assert(pPacket != NULL);
		GetViewVertMinMax_Packet(pPacket, displayAttributes, bWithoutZero, displayDataMinMax, p_bWithFullRange);
	}
	if( displayDataMinMax.IsInitialized()) ChangeViewVertMinMax_Indicator(inputData, displayDataMinMax);

	return displayDataMinMax.IsInitialized() ? true : false;
}

// ----------------------------------------------------------------------------------------------
// ��Ÿ ������ ���..
// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
bool CViewMinMaxCalculator::GetViewVertMinMax_Etc(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine)
{
	// "�ݵ�" �� ���
	if(inputData.GetSubGraphType() == CGraphBaseData::Fund_Type)
		return GetViewVertMinMax_Fund(inputData, displayDataMinMax, p_bWithFullRange);

	// GraphName �� �ִ� ���
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	if(!inputData.GetIndicatorName().IsEmpty())
		return GetViewVertMinMax_Indicate(inputData, displayDataMinMax, p_bWithFullRange, bDrawBaseLine);

	// "packet"�� �� ��� - subGraph �� �ִ� packet �� ���
	if(GetViewVertMinMax_InSubGraphPacketList(inputData, displayDataMinMax, p_bWithFullRange))
		return true;

	// "packet"�� �� ��� - subGraph �� �ִ� packet �� �ƴ� ���
	return GetViewVertMinMax_OutsideSubGraphPacketList(inputData, displayDataMinMax, p_bWithFullRange);
}

// -> "�ݵ�" �� ���
bool CViewMinMaxCalculator::GetViewVertMinMax_Fund(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	switch(inputData.GetSubGraphStyle().GetGraphFundStyle()){
	case CGraphBaseData::ComponentRatio_Line:	// ������(����)
	case CGraphBaseData::ComponentRatio_Bar:	// ������(����)
		displayDataMinMax = CDisplayDataMinMax(0.0, 100.0);
		return true;
	case CGraphBaseData::ComponentRatio_Pie:	// ������(������)
	case CGraphBaseData::ReturnRiskGrade:		// ����&������
		displayDataMinMax = CDisplayDataMinMax(0.0, 0.0);
		return true;
	case CGraphBaseData::StyleBox:				// ��Ÿ�Ϲڽ�
		displayDataMinMax = CDisplayDataMinMax(0.0, 15.0);
		return true;
	}

	// "�ݵ�&����л�" �� ��Ÿ
	return GetPacketMinMax(GetPacket(inputData.GetSubGraphPacketList(), inputData.GetScaleData()), 
		inputData.GetDisplayAttributes(), false, displayDataMinMax, p_bWithFullRange);
}

// -> "packet"�� �� ��� - subGraph �� �ִ� packet �� ���
bool CViewMinMaxCalculator::GetViewVertMinMax_InSubGraphPacketList(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CSubGraphPacketList* pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return false;

	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();
	bool bWithoutZero = DoesViewMinMaxWithoutZero(inputData.GetSubGraphData());

	CString strPacketNames = inputData.GetScaleData();
	if(strPacketNames.Right(1) != ',')
		strPacketNames += ',';

	while(!strPacketNames.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ',');
		CPacket* pPacket = GetPacket(pSubGraphPacketList, strPacketName);
		if(pPacket == NULL)
		{
			displayDataMinMax.SetDisplayData(1.0, 1.0);
			return false;
		}
		
		GetViewVertMinMax_Packet(pPacket, displayAttributes, bWithoutZero, displayDataMinMax, p_bWithFullRange);
	}
	if( displayDataMinMax.IsInitialized()) ChangeViewVertMinMax_GraphTypeOrStyle(inputData.GetSubGraphData(), displayDataMinMax);

	return displayDataMinMax.IsInitialized() ? true : false;
}

// -> "packet"�� �� ��� - subGraph �� �ִ� packet �� �ƴ� ���
bool CViewMinMaxCalculator::GetViewVertMinMax_OutsideSubGraphPacketList(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	CPacketList* pPacketList = GetPacketList(inputData.GetSubGraphPacketList());
	if(pPacketList == NULL)
		return false;

	CDisplayAttributes displayAttributes = inputData.GetDisplayAttributes();
	bool bWithoutZero = DoesViewMinMaxWithoutZero(inputData.GetSubGraphData());

	CString strPacketNames = inputData.GetScaleData();
	if(strPacketNames.Right(1) != ',')
		strPacketNames += ',';

	while(!strPacketNames.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ',');
		CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
		if(pPacket == NULL)
			continue;
		
		GetViewVertMinMax_Packet(pPacket, displayAttributes, bWithoutZero, displayDataMinMax, p_bWithFullRange);
	}
	
	return displayDataMinMax.IsInitialized() ? true : false;
}

// ----------------------------------------------------------------------------------------------
// graph�� ���� Min/Max ����
bool CViewMinMaxCalculator::ChangeViewVertMinMax_Indicator(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax)
{
	return ChangeViewVertMinMax_GraphTypeOrStyle(inputData.GetSubGraphData(), displayDataMinMax);
}

// graph type / graph style �� Min/Max ����
bool CViewMinMaxCalculator::ChangeViewVertMinMax_GraphTypeOrStyle(CSubGraphData* pSubGraphData, CDisplayDataMinMax& displayDataMinMax)
{
	if(pSubGraphData == NULL)
		return false;

	// <����>�� <��������������> �� ���
	return ChangeViewVertMinMax_BarType(pSubGraphData, displayDataMinMax);
}

// -> <����>�� ���
bool CViewMinMaxCalculator::ChangeViewVertMinMax_BarType(CSubGraphData* pSubGraphData, CDisplayDataMinMax& displayDataMinMax)
{
	if(pSubGraphData->GetType() != CGraphBaseData::Bar_Type)
		return false;

	switch( pSubGraphData->GetStyle().GetGraphBarStyle())
	{
		// <��������������> �� ��� - ���밪�� ���� ū���� Max, Min = -Max
		case CGraphBaseData::FixedRateCombBar:
		// �����������Ͻ� �߰� - ojtaso (20080603)
		case CGraphBaseData::FixedRateUpDownBar:
		{
			double dMin = fabs(displayDataMinMax.GetDisplayMin());
			double dMax = fabs(displayDataMinMax.GetDisplayMax());
			if(dMax > dMin)
				displayDataMinMax.SetDisplayData(-dMax, dMax);
			else
				displayDataMinMax.SetDisplayData(-dMin, dMin);
			break;
		}

		// "�ŷ�����Ʈ" �� ��쿣 �׻� min = 0
		case CGraphBaseData::VerticalBar:
		{
			// (2007/1/5 - Seung-Won, Bae) Support Default Max of Bar Chart. (for avoiding the Center 0.0 of VScale.)
			double dMax = displayDataMinMax.GetDisplayMax();
			if( dMax == 0.0)	displayDataMinMax.SetDisplayData( 0.0, 1.0);
			else				displayDataMinMax.SetDisplayData( 0.0, dMax);
			break;
		}

		default:	return false;
	}
	return true;
}

// ----------------------------------------------------------------------------------------------
// packet���� Min/Max ���� ���Ѵ�.
bool CViewMinMaxCalculator::GetViewVertMinMax_Packet(CPacket* pPacket, const CDisplayAttributes& displayAttributes, 
		const bool bWithoutZero, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	if(pPacket == NULL)
		return false;

	// packetName = <xxx�ڷ�����> �� ��쿡�� min/max�� ������ �ʴ´�!!
	if(pPacket->GetName().Find( _MTEXT( C0_DATE_TIME)) >= 0)
		return false;
	
	CDisplayDataMinMax temp;
	if(!GetPacketMinMax(pPacket, displayAttributes, bWithoutZero, temp, p_bWithFullRange))
		return false;

	// Min/Max ���ϴ� packet�� ó���� �ƴ� ��쿡�� ������ Min/Max�� ���Ѵ�.
	// (2006/11/19 - Seung-Won, Bae) Support Initializing State
	if( displayDataMinMax.IsInitialized())
	{
		double dMin = displayDataMinMax.GetDisplayMin();
		double dMax = displayDataMinMax.GetDisplayMax();
		if(temp.GetDisplayMin() < dMin) dMin = temp.GetDisplayMin();
		if(temp.GetDisplayMax() > dMax) dMax = temp.GetDisplayMax();
		displayDataMinMax.SetDisplayData( dMin, dMax);
	}
	else displayDataMinMax = temp;

	return true;
}

// bWithoutZero = true : Min/Max �� "0" �� �������� �ʴ´�.
bool CViewMinMaxCalculator::GetPacketMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, 
		const bool bWithoutZero, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const
{	
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return false;

	int nStart = -1, nEnd = -1;
	
	// (2006/11/11 - Seung-Won, Bae) Support Full Range Min/Max Vert Scale.
	if( !p_bWithFullRange)
	{
		if(!GetDataStartAndEndInBlock(pPacket, displayAttributes, nStart, nEnd))
			return false;
	}
	else
	{
		nStart	= 0;
		nEnd	= INT_MAX;
	}

	bool bResult = false;
	//20100515 �̹��� >>
	//double dMin = -HUGE_VAL, dMax = -HUGE_VAL;
	double dMin = HUGE_VAL, dMax = -HUGE_VAL;
	//20100515 �̹��� >>

	// (2007/2/7 - Seung-Won, Bae) Calculate Min/Max without Zero
	if( pPacket->GetMinMax( nStart, nEnd, dMin, dMax, bWithoutZero) < 0) return false;

	displayDataMinMax = CDisplayDataMinMax(dMin, dMax);
	return true;
}

// ----------------------------------------------------------------------------------------------
// �ش� packet ��������
CPacketList* CViewMinMaxCalculator::GetPacketList(CSubGraphPacketList* pSubGraphPacketList) const
{
	if(pSubGraphPacketList == NULL)
		return NULL;

	CPacket* pPacket = pSubGraphPacketList->GetHeadPacket();
	if(pPacket == NULL)
		return NULL;

	return pPacket->GetPacketList();
}

CPacket* CViewMinMaxCalculator::GetPacket(CSubGraphPacketList* pSubGraphPacketList, const CString& strPacketName) const
{
	if(pSubGraphPacketList == NULL || strPacketName.IsEmpty())
		return NULL;

	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = pSubGraphPacketList->GetNext(pos);
		assert(pPacket != NULL);
		if(pPacket->GetName() == strPacketName)
			return pPacket;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------------------
bool CViewMinMaxCalculator::GetDataStartAndEndInBlock(CPacket* pPacket, const CDisplayAttributes& displayAttributes, int& nStart, int& nEnd) const
{
	nStart = GetDataStartInBlock(pPacket, displayAttributes.GetDataStartInBlock());
	nEnd = displayAttributes.GetDataEndInBlock();

	return (nStart >= 0 && nEnd >= 0 && nStart <= nEnd);
}

// ȭ�鿡 ���̴� data ���� index
int CViewMinMaxCalculator::GetDataStartInBlock(CPacket* pPacket, const int nDataStartInBlock) const
{
	if(pPacket == NULL || nDataStartInBlock > pPacket->GetStartPeriod())
		return nDataStartInBlock;

	return pPacket->GetStartPeriod();
}

// Min/Max�� "0" ���� ����
// true : "0" ���� ����. -> <����>�� <NonZero��ǥ����> �� ��츸 �ش��.
bool CViewMinMaxCalculator::DoesViewMinMaxWithoutZero(CSubGraphData* pSubGraphData) const
{
	if(pSubGraphData == NULL)
		return false;

	// (2006/12/20 - Seung-Won, Bae) Support with Not-Continued Line in 0
	switch( pSubGraphData->GetType())
	{
		case CGraphBaseData::Bar_Type:
			if (pSubGraphData->GetDrawStyle().GetGraphBarDrawStyle() == CGraphBaseData::NonZero_DotBar_Draw)
				return true;
			else break;

		case CGraphBaseData::Line_Type:
			if (pSubGraphData->GetStyle().GetGraphLineStyle() == CGraphBaseData::NonZeroLine)
				return true;
			else break;

		// (2007/2/6 - Seung-Won, Bae) Check Candle Stick Chart for avoiding Zero Open/High/Low in Base or Market Close Price.
		//		Caution! It is Koscom Only. Use EXCEPTIONVALUE Help Message in Packet.
		case CGraphBaseData::Bong_Type:
			// (2009/1/17 - Seung-Won, Bae) Do not check 'KOSCOM' name. (on sometimes, CWinApp can not be found.)
			return ( pSubGraphData->GetStyle().GetGraphBongStyle() != CGraphBaseData::LineBong);
	}

	return false;
}

// Min/Max�� gap�� ������ ����
bool CViewMinMaxCalculator::DoesDisplayDataMinMaxInGap(const CVertDisplayDataMinMaxInputData& inputData) const
{
	// <�̴�������Ʈ> �� ���
	if(inputData.GetIndicatorName() == _MTEXT( C2_MINI_INDEX_CHART))
		return true;

	if(!inputData.GetIndicatorName().IsEmpty())
		return false;

	// subGraph type �� ����.
	return DoesDisplayDataMinMaxInGap_Type(inputData.GetSubGraphData());
}

// Min/Max�� gap�� �δ� graph������ ����.
// <����>�� <���Ͻ�>, <���ϵ����>, <��������������> �� ���
// -> ��, drawingStyle�� <Text��>�� ���� gap�� ���� �ʴ´�.
bool CViewMinMaxCalculator::DoesDisplayDataMinMaxInGap_Type(const CSubGraphData* pSubGraphData) const
{
	if(pSubGraphData == NULL)
		return false;
	
	if(pSubGraphData->GetType() != CGraphBaseData::Bar_Type)
		return false;

	// drawingStyle�� <Text��>�� ���� Text�� ǥ���Ǳ� ������ gap�� ���ʿ䰡 ����!
	if(pSubGraphData->GetDrawStyle().GetGraphBarDrawStyle() == CGraphBaseData::TextBar_Draw)
		return false;

	CGraphBaseData::BARSTYLE eStyle = pSubGraphData->GetStyle().GetGraphBarStyle();
	switch(eStyle){
		case CGraphBaseData::UpDownBar: // ���Ͻ�
		case CGraphBaseData::UpDownHighLowBar: // ���ϵ����
		case CGraphBaseData::FixedRateCombBar: // ��������������
		// �����������Ͻ� �߰� - ojtaso (20080603)
		case CGraphBaseData::FixedRateUpDownBar: // �����������Ͻ�
			return true;
	}

	return false;
}

// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�
bool CViewMinMaxCalculator::GetViewVertMinMax_DisperSion(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	// 1. Sub Graph Packet List�� ���Ѵ�.
	CSubGraphPacketList *pSubGraphPacketList = inputData.GetSubGraphPacketList();
	if( !pSubGraphPacketList) return false;

	// 2. ���� Packet�� Ȯ���Ѵ�.
	POSITION pos = pSubGraphPacketList->GetHeadPosition();
	if( !pos) FALSE;
	CPacket* pPacket = pSubGraphPacketList->GetNext( pos);	//�ð�Packet
	if( !pos) FALSE;
	pPacket = pSubGraphPacketList->GetNext( pos);			//����Packet

	return GetPacketMinMax(pPacket, inputData.GetDisplayAttributes(), false, displayDataMinMax, p_bWithFullRange);
}
// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�
