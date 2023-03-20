// PatternDBItems.cpp : implementation file
//

#include "stdafx.h"
#include "PatternDBItems.h"
#include "PatternDefine.h"
#include "Include/ColorTable.h"							// for Color


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// **************************************************************************
// CHighLowPtData
// **************************************************************************
CHighLowPtData::CHighLowPtData()
{
	m_nPointType	= ZERO;

	m_nDataSeq		= ZERO;
	m_dTime			= ZERO;		

	m_dOpen			= ZERO;		
	m_dHigh			= ZERO;		
	m_dLow			= ZERO;			
	m_dClose		= ZERO;		
}
CHighLowPtData::~CHighLowPtData()
{
}


// **************************************************************************
// CMAData
// **************************************************************************
CMAData::CMAData()
{
	m_nDataSeq		= ZERO;
	
	m_dMMa			= ZERO;
}
CMAData::~CMAData()
{
}


// **************************************************************************
// CRegistNSupportData
// **************************************************************************
CRegistNSupportData::CRegistNSupportData()
{
	m_nRSLineType	= ZERO;				// ���׼�, ������ ����

	m_nDataSeq1		= ZERO;				// ù��° ������ ����
	m_dYPoint1		= ZERO;				// ù��° ������ ��

	m_nDataSeq2		= ZERO;				// �ι�° ������ ����
	m_dYPoint2		= ZERO;				// �ι�° ������ ��
}
CRegistNSupportData::~CRegistNSupportData()
{
}


// **************************************************************************
// CWavePatternData
// **************************************************************************
CWavePatternData::CWavePatternData()
{
	m_nPatternType	= ZERO;				// Pattern Type
	m_bIsSelect		= FALSE;			// Pattern Select

	m_nStartSeq		= ZERO;				// Start Sequence of a Pattern
	m_nEndSeq		= ZERO;				// End Sequence of a Pattern

	m_dDrawSeq1_1	= ZERO;				// First Draw Data
	m_dDrawYPt1_1	= ZERO;
	m_dDrawSeq1_2	= ZERO;				
	m_dDrawYPt1_2	= ZERO;				

	m_dDrawSeq2_1	= ZERO;				// Second Draw Data
	m_dDrawYPt2_1	= ZERO;
	m_dDrawSeq2_2	= ZERO;				
	m_dDrawYPt2_2	= ZERO;				

	m_dNeckSeq1		= ZERO;				// First Neck Data
	m_dNeckYPt1		= ZERO;
	m_dNeckSeq2		= ZERO;				// Second Neck Data
	m_dNeckYPt2		= ZERO;

	m_dNeckBaseYPt1	= ZERO;
	m_dNeckBaseYPt2	= ZERO;
		
}
CWavePatternData::~CWavePatternData()
{
}

// **************************************************************************
// CLineDrawData
// **************************************************************************
CLineDrawData::CLineDrawData()
{
	m_nWeight = 1;
	m_color = RGB(0,0,0);
}

CLineDrawData::~CLineDrawData()
{
}

// **************************************************************************
// CPatternEnvData
// **************************************************************************
CPatternEnvData::CPatternEnvData()
{
	m_bNotDrawAll			= FALSE;			// ��� ���� �׸��� ����

	m_nDrawMethod			= 0;				// �ڵ��߼��� �۵���(0: �����̵���չ�, 1:����ĵ��)
	m_nMAPeriod				= 5;				// �̵���� �Ⱓ
	m_nCandleCnt			= 3;				// ���� ĵ�� ����
	m_nCandleAfterCnt		= 3;				// �� ĵ�� ����

	m_bDrawAutoPatternLine	= TRUE;				// �ڵ��߼�����
	m_bDrawHighLowPtLine	= TRUE;				// �ĵ���
	m_bDrawMMa				= FALSE;			// �����̵���� ����

	m_bDrawHighPtHLine		= FALSE;			// ���� �������
	m_bDrawLowPtHLine		= FALSE;			// ���� �������

	m_bDrawHighRegistLine	= TRUE;			// ���� ���׼�
	m_bDrawLowSupportLine	= TRUE;			// ���� ������

	m_dHLPtAllowalbeError	= 0;				// �ֿ����, ���� ���� ����, ���� ���̰� n% �̳��� ��� �����ͷ� ������� ����
	m_dNextPtAllowableError	= 1;				// �� �ֿ����� ������ ������ ���� �ֿ������� ���̰� n% �̳��� ��� �����ͷ� ���

	m_bFilterSideWalkPeriod	= TRUE;				// Ⱦ������ ���͸� ����

	m_lineDataHighLowPt.m_color = COLOR_HIGHLOW_PTLINE;
	m_lineDataHighPtH.m_color = COLOR_HIGH_HORZLINE;			// ���� �������
	m_lineDataLowPtH.m_color = COLOR_LOW_HORZLINE;				// ���� �������
	m_lineDataHighRegist.m_color = RGB(100,100,100);			// ���� ���׼�
	m_lineDataLowSupport.m_color = RGB(100,100,100);			// ���� ������

	m_bDrawPatternName = FALSE;
}
CPatternEnvData::~CPatternEnvData()
{
}

BOOL CPatternEnvData::IsDisplayCandle(int nCandleType)
{
	int nIndex;
	for(int i=0; i<m_arrNotDisplayCandleItems.GetSize(); i++)
	{
		nIndex = m_arrNotDisplayCandleItems.GetAt(i);
		if(nCandleType == nIndex)
			return FALSE;
	}
	return TRUE;
}
// **************************************************************************
// CSelPatternData4Draw Object
// **************************************************************************
CSelPatternData4Draw::CSelPatternData4Draw()
{
	m_bDraw			= FALSE;
	m_nPatternType	= ZERO;				// Pattern Type

	m_nStartSeq		= ZERO;
	m_nEndSeq		= ZERO;

	m_nX1			= ZERO;
	m_nY1			= ZERO;
	m_nX2			= ZERO;
	m_nY2			= ZERO;

	m_nX3			= ZERO;
	m_nY3			= ZERO;
	m_nX4			= ZERO;
	m_nY4			= ZERO;

	m_nNeckXPos1	= ZERO;
	m_nNeckYPos1	= ZERO;
	m_nNeckXPos2	= ZERO;
	m_nNeckYPos2	= ZERO;

	m_nNeckXLength	= ZERO;
	m_nNeckYLength1	= ZERO;
	m_nNeckYLength2	= ZERO;
}
CSelPatternData4Draw::~CSelPatternData4Draw()
{
}


// **************************************************************************
// CCandlePatternData Object
// **************************************************************************
CCandlePatternData::CCandlePatternData()
{
	m_dTime				= ZERO;			// Time
	m_nCandlePtnType	= ZERO;			// Candle Pattern Type
	
	m_nStartPos			= ZERO;
	m_nEndPos			= ZERO;

	memset( (void*)&m_dClose, ZERO, sizeof(m_dClose));	// Close Price for Accumulation rise and fall rate
}
CCandlePatternData::~CCandlePatternData()
{
}


// **************************************************************************
// CCandlePatternData Object
// **************************************************************************
CCandlePatternPosInfo::CCandlePatternPosInfo()
{
}
CCandlePatternPosInfo::~CCandlePatternPosInfo()
{
}
