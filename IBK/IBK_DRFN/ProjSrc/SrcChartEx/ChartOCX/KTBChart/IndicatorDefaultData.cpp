// IndicatorDefaultData.cpp: implementation of the CIndicatorDefaultData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorDefaultData.h"

#include "../Include_Chart/DLL_Load/GraphTypeData.h"		// for CGraphTypeAndStyleData
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()

#include "Conversion.h"
#include "GraphData.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "KTBChartCtl.h"		// for CKTBChartCtrl

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndicatorDefaultData::CIndicatorDefaultData( const CKTBChartCtrl *p_pChartOCX, const CScaleBaseData::SCALEGRIDTYPE eBaseGridType, 
	const CScaleBaseData::VERTSCALEPOSITION eVertScalePosition) :
		m_eBaseGridType(eBaseGridType), 
		m_eVertScalePosition(eVertScalePosition)
{
	// (2004.10.06, ��¿�) ������Ʈ�� Graph Drawing Data�� �����޾� DefaultType�� ���� ����, ������� �Ѵ�.
	m_pChartOCX = p_pChartOCX;
}

void CIndicatorDefaultData::SetVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	m_eVertScalePosition = ePosition;
}

// ----------------------------------------------------------------------------
// ��ǥ�� ��ü�� ����� ���� string.

// BlockColumn;\r\n
// Block;\r\n
// ����� Data;��ġ;gridŸ��;����Ÿ��;�ð�Ÿ��;textŸ��;log;invert;2��block;\r\n
// VertScaleGroup;\r\n
// ����� Data;��ġ;gridŸ��;unitŸ��;\r\n
// Graph;\r\n
// index;������Ʈ;�ð�,��,����,����;signal��뿩��:signal����;
//		�������ְ������;���������簡�����;�ְ����簡�����;���Ϻ�;���簡;\r\n
// subGraphtype;subGraphStyle;subGraphDrawStyle;subGraphName;title;����;��1;��2;��3;��4;��5;��6;
//		���ä��;�϶�ä��;������1;������2;������3;����;\r\n
// .... subGraph ������ŭ..
// GraphEnd;\r\n
// VertScaleGroupEnd;\r\n
// BlockEnd;\r\n
// BlockColumnEnd;\r\n
// -> Block ����� : ��ǥ�� �ش�Ǵ� ���
BOOL CIndicatorDefaultData::GetBlockDataForMaking( CString &p_strBlockData, CIndicatorList* pIndicatorList, const CString& strIndicatorName, 
		const CScaleBaseData::SCALEGRIDTYPE eHorzGridType, const CScaleBaseData::SCALEGRIDTYPE eVertGridType, CString &p_strGraphName)
{
	p_strBlockData.Empty();
	p_strGraphName.Empty();
	if( strIndicatorName.IsEmpty()) return FALSE;

	// vertScaleGroup �κ�.
	m_eBaseGridType = eVertGridType;
	CString strVertScaleGroupData;
	GetVertScaleGroupDataForMaking( strVertScaleGroupData, pIndicatorList, strIndicatorName, p_strGraphName);
	if( strVertScaleGroupData.IsEmpty() || p_strGraphName.IsEmpty()) return FALSE;

	// ���� scale �κ�.
	m_eBaseGridType = eHorzGridType;

	CString strHorizontalScaleData = GetHorizontalScaleData( strIndicatorName, TRUE);

	int nBlockDataLen = 0;
	nBlockDataLen += 22;									// "BlockColumn;\r\nBlock;\r\n"
	nBlockDataLen += strHorizontalScaleData.GetLength();
	nBlockDataLen += strVertScaleGroupData.GetLength();
	nBlockDataLen += 28;									// "BlockEnd;\r\nBlockColumnEnd;\r\n"

	char *szBlockData = p_strBlockData.GetBufferSetLength( nBlockDataLen);
	
	strcpy( szBlockData, "BlockColumn;\r\nBlock;\r\n");
	szBlockData += 22;

	strcpy( szBlockData, strHorizontalScaleData);
	szBlockData += strHorizontalScaleData.GetLength();

	strcpy( szBlockData, strVertScaleGroupData);
	szBlockData += strVertScaleGroupData.GetLength();
	
	strcpy( szBlockData, "BlockEnd;\r\nBlockColumnEnd;\r\n");
	szBlockData += 28;

	return TRUE;
}

//sy 2004.11.02.
// -> VertScaleGroup ����� : ��ǥ�� �ش�Ǵ� ���
BOOL CIndicatorDefaultData::GetVertScaleGroupDataForMaking( CString &p_strVertScaleGroupData, CIndicatorList* pIndicatorList, const CString &strIndicatorName, CString &p_strGraphName) const
{
	p_strGraphName.Empty();
	p_strVertScaleGroupData.Empty();
	if( !pIndicatorList) return FALSE;
	CIndicatorInfo *pIndicatorInfo = pIndicatorList->CreateGraphIndicatorInfo( strIndicatorName);
	if( !pIndicatorInfo) return FALSE;
	GetVertScaleGroupDataForMaking( p_strVertScaleGroupData, pIndicatorInfo);
	p_strGraphName = pIndicatorInfo->GetGraphName();
	return TRUE;
}

// -> VertScaleGroup ����� : ��ǥ�� �ش�Ǵ� ���
void CIndicatorDefaultData::GetVertScaleGroupDataForMaking( CString &p_strVertScaleGroupData, CIndicatorInfo* pIndicatorInfo) const
{
	p_strVertScaleGroupData.Empty();
	if( !pIndicatorInfo) return;

	// (2004.10.06, ��¿�) ������Ʈ�� Graph Drawing Data�� �����޾� DefaultType�� ���� ����, ������� �Ѵ�.
	CString strGraphData;
	pIndicatorInfo->GetGraphDataForMaking( strGraphData);
	if( strGraphData.IsEmpty()) return;

	CString strVertScaleData;
	GetVerticalScaleData( strVertScaleData, pIndicatorInfo->GetIndicatorName());

	int nVertScaleGroupDataLen = 0;
	nVertScaleGroupDataLen += 17;		// "VertScaleGroup;\r\n"
	nVertScaleGroupDataLen += strVertScaleData.GetLength();
	nVertScaleGroupDataLen += strGraphData.GetLength();
	nVertScaleGroupDataLen += 20;		// "VertScaleGroupEnd;\r\n"

	char *szVertScaleGroupData = p_strVertScaleGroupData.GetBufferSetLength( nVertScaleGroupDataLen);
	
	strcpy( szVertScaleGroupData, "VertScaleGroup;\r\n");
	szVertScaleGroupData += 17;
	
	strcpy( szVertScaleGroupData, strVertScaleData);
	szVertScaleGroupData += strVertScaleData.GetLength();
	
	strcpy( szVertScaleGroupData, strGraphData);
	szVertScaleGroupData += strGraphData.GetLength();
	
	strcpy( szVertScaleGroupData, "VertScaleGroupEnd;\r\n");
	szVertScaleGroupData += 20;
}
//sy end

// ----------------------------------------------------------------------------
// ���� scaleData �����
// ����� Data;��ġ;gridŸ��;����Ÿ��;�ð�Ÿ��;textŸ��;log;invert;2��block;\r\n
CString CIndicatorDefaultData::GetHorizontalScaleData(const CString& strGraphName, const BOOL bIsEndBlock) const
{
	CString strData;
	// (2004.03.11, ��¿�) ��� �ڷ����ڸ� '/'�� �����Ѵ�.
	strData.Format("%s;%d;%d;/;:;����;\r\n",
		GetHorizontalPacketName(strGraphName), // ����� Data -> ��ǥ Data
		GetHorizontalPosition(strGraphName, bIsEndBlock),   // ��ġ
		m_eBaseGridType);	// grid type

	return strData;
}

// ���� scaleData �����
// ����� Data;��ġ;gridŸ��;unitŸ��;\r\n
void CIndicatorDefaultData::GetVerticalScaleData( CString &p_strVerticalScaleData, const CString& strGraphName) const
{
	//sy 2004.05.12. -> graph �� ���� ���� ������ ��ġ ����.
	// (2004.09.16, ��¿�, �м�) Main Block�� ���� Scale ������ ���󰡵��� �Ѵ�.
	CScaleBaseData::VERTSCALEPOSITION eVertScalePosition = m_eVertScalePosition;

	// (2006/4/16 - Seung-Won, Bae) Special Chart has scale in Both Side.
	if( g_iMetaTable.IsSpecialGroupGraph( strGraphName)) eVertScalePosition = CScaleBaseData::VERT_BOTH;

	int nIndiDataLen = strlen( _MTEXT( C6_INDICATOR_DATA));

	char szVertScalePosition[ 20];
	itoa( eVertScalePosition, szVertScalePosition, 10);
	int nVertScalePositionLen = strlen( szVertScalePosition);

	char szBaseGridType[ 20];
	itoa( m_eBaseGridType, szBaseGridType, 10);
	int nBaseGridTypeLen = strlen( szBaseGridType);

	int nVerticalScaleDataLen = 0;
	nVerticalScaleDataLen += nIndiDataLen;
	nVerticalScaleDataLen++;						// ;
	nVerticalScaleDataLen += nVertScalePositionLen;	
	nVerticalScaleDataLen++;						// ;
	nVerticalScaleDataLen += nBaseGridTypeLen;	
	nVerticalScaleDataLen++;						// ;
	nVerticalScaleDataLen += 4;						// 0;\r\n

	char *szVirticalScaleData = p_strVerticalScaleData.GetBufferSetLength( nVerticalScaleDataLen);

	strcpy( szVirticalScaleData, _MTEXT( C6_INDICATOR_DATA));
	szVirticalScaleData += nIndiDataLen;
	*szVirticalScaleData++ = ';';

	strcpy( szVirticalScaleData, szVertScalePosition);
	szVirticalScaleData += nVertScalePositionLen;
	*szVirticalScaleData++ = ';';

	strcpy( szVirticalScaleData, szBaseGridType);
	szVirticalScaleData += nBaseGridTypeLen;
	*szVirticalScaleData++ = ';';

	strcpy( szVirticalScaleData, "0;\r\n");
}

// ----------------------------------------------------------------------------
// ���� scale ����� Data.
CString CIndicatorDefaultData::GetHorizontalPacketName(const CString& strGraphName) const
{
	if(strGraphName == _MTEXT( C2_THREE_LINE_BREAK))	// "�Ｑ��ȯ��"
		return _MTEXT( C0_THREE_LINE_BREAK_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_INVERSE_TIME_LINE))		// "���ð�"
		return _MTEXT( C0_INVERSETIMELINE_VOLUME);
	//2007.06.01 by LYH P&F X������ ���迡�� �������� ���� return ""; -> return _MTEXT( C0_PF_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_P_AND_F))	// "P&F"
		return _MTEXT( C0_PF_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_SWING_CHART))	// "SwingChart"
		return _MTEXT( C0_SWING_CHART_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_KAGI_CHART))		// "KagiChart"
		return _MTEXT( C0_KAGI_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_RENKO_CHART))	// "RenKoChart"
		return _MTEXT( C0_RENKO_CHART_DATETIME);
// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�
	else if(strGraphName == _MTEXT( C2_DISPERSION))	// "�л�Chart"
		return _MTEXT( C0_VOLUME);
// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�

	return _MTEXT( C0_DATE_TIME);
}

// ���� scale �� ��ġ.
CScaleBaseData::HORZSCALEPOSITION CIndicatorDefaultData::GetHorizontalPosition(const CString& strGraphName, const BOOL bIsEndBlock) const
{
	// Block �ϳ� / �� -> �ϴ�(���� �� text ����)
	if(bIsEndBlock)
		return CScaleBaseData::HORZ_BOTTOM;

	// Block �ΰ� �̻� �� ó��/�߰� -> ����(������ ����)
	return CScaleBaseData::HORZ_HIDE;
}

CIndicatorDefaultData::BLOCKMAKINGTYPE CIndicatorDefaultData::GetBlockMakingType(const int nCount, const int nIndex) const
{
	if(nCount <= 0)
		return NONE;
	else if(nCount == 1) // block 1���� ���
		return ONE_BLOCK;

	if(nIndex == 0) // block 2�� �̻� �� ó���� ���
		return START_TWO_BLOCK;
	else if(nIndex == nCount - 1) // block 2�� �̻� �� ���� ���
		return END_TWO_BLOCK;

	return MIDDEL_THREE_BLOCK; // block 3�� �̻� �� ����� ���
}
