// ViewMinMaxCalculator.h: interface for the CViewMinMaxCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWMINMAXCALCULATOR_H__8023B2AA_3388_46FC_8996_1E3B938E8676__INCLUDED_)
#define AFX_VIEWMINMAXCALCULATOR_H__8023B2AA_3388_46FC_8996_1E3B938E8676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseData.h"

class CHorzDisplayDataMinMaxInputData;
class CVertDisplayDataMinMaxInputData;
class CDisplayDataMinMax;
class CDisplayAttributes;
class CSubGraphData;
class CSubGraphPacketList;
class CPacketList;
class CPacket;

class CViewMinMaxCalculator  
{
public:
	// view min, max �� ��������
	CDisplayDataMinMax GetViewHorzMinMax(const CHorzDisplayDataMinMaxInputData& inputData) const;
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	bool GetViewVertMinMax(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine);

private:
	// ���� scale�� min/max�� �������� ����
	bool IsViewVertMinMax(CSubGraphPacketList* pSubGraphPacketList) const;

	// ���� scale
	// "MarketProfile" �� ���
	CDisplayDataMinMax GetViewHorzMinMax_MarketProfile(const CHorzDisplayDataMinMaxInputData& inputData) const;

	// ���� scale
	// -> "��ǥ Data" �� ���
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	bool GetViewVertMinMax_Indicate(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine);
	bool GetViewVertMinMax_Indicate_General_Gap(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_Indicate_General(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_HeadPacket(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	bool GetViewVertMinMax_EndPacket(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine);
	// (2004.02.06, ��¿�) ���� ���� P&F�� ��� ������ Ȯ���Ѵ�.
	BOOL	GetViewVertMinMax_FuPAndF( const CVertDisplayDataMinMaxInputData &inputData, CDisplayDataMinMax &displayDataMinMax);
	bool GetViewVertMinMax_PAndF(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_Sale(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_MarketProfile(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax);
	// -> ��Ÿ ������ ���..
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	bool GetViewVertMinMax_Etc(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine);
	bool GetViewVertMinMax_Fund(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_InSubGraphPacketList(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_OutsideSubGraphPacketList(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	// graph�� ���� Min/Max ����
	bool ChangeViewVertMinMax_Indicator(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax);
	// -> graph type / graph style�� ���� ���
	bool ChangeViewVertMinMax_GraphTypeOrStyle(CSubGraphData* pSubGraphData, CDisplayDataMinMax& displayDataMinMax);
	bool ChangeViewVertMinMax_BarType(CSubGraphData* pSubGraphData, CDisplayDataMinMax& displayDataMinMax);

	bool GetViewVertMinMax_Packet(CPacket* pPacket, const CDisplayAttributes& displayAttributes, 
		const bool bWithoutZero, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetPacketMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, const bool bWithoutZero, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const;
// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�
	bool GetViewVertMinMax_DisperSion(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�

	// �ش� packet ��������
	CPacketList* GetPacketList(CSubGraphPacketList* pSubGraphPacketList) const;
	CPacket* GetPacket(CSubGraphPacketList* pSubGraphPacketList, const CString& strPacketName) const;
	bool GetDataStartAndEndInBlock(CPacket* pPacket, const CDisplayAttributes& displayAttributes, int& nStart, int& nEnd) const;
	// ȭ�鿡 ���̴� data ���� index
	int GetDataStartInBlock(CPacket* pPacket, const int nDataStartInBlock) const;
	// Min/Max�� "0" ���� ����
	bool DoesViewMinMaxWithoutZero(CSubGraphData* pSubGraphData) const;
	// Min/Max�� gap�� ������ ����
	bool DoesDisplayDataMinMaxInGap(const CVertDisplayDataMinMaxInputData& inputData) const;
	bool DoesDisplayDataMinMaxInGap_Type(const CSubGraphData* pSubGraphData) const;
};

#endif // !defined(AFX_VIEWMINMAXCALCULATOR_H__8023B2AA_3388_46FC_8996_1E3B938E8676__INCLUDED_)
