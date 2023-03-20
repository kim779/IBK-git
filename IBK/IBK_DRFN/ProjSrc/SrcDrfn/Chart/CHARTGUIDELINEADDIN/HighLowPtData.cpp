// HighLowPtData.cpp : implementation file
//

#include "stdafx.h"
#include "HighLowPtData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCheChartData
CCheChartData::CCheChartData()
{
	m_strJmCode		= _T("")	;	//�����ڵ�
	m_strChetime	= _T("")	;	//ü��ð�		
	m_dChedanga		= VALUE_ZERO;	//ü��ܰ�	
	m_nMesuMedoGb	= VALUE_ZERO;	//�ŵ��ż�����
}

CCheChartData::~CCheChartData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CHighLowPtData
CHighLowPtData::CHighLowPtData()
{
	m_nPointGb		= VALUE_ZERO;

	m_nDataSequence = VALUE_ZERO;
	m_dTime			= VALUE_ZERO;		

	m_dOpen			= VALUE_ZERO;		
	m_dHigh			= VALUE_ZERO;		
	m_dLow			= VALUE_ZERO;			
	m_dClose		= VALUE_ZERO;		
}

CHighLowPtData::~CHighLowPtData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMAData
CMAData::CMAData()
{
	m_nDataSequence = VALUE_ZERO;
	
	m_dMMa			= VALUE_ZERO;
}

CMAData::~CMAData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRegistNSupportData
CRegistNSupportData::CRegistNSupportData()
{
	m_nPointGb		= VALUE_ZERO;				// ����, ���� ����

	m_nDataSeq1		= VALUE_ZERO;				// ù��° ������ ����
	m_dYPoint1		= VALUE_ZERO;				// ù��° ������ ��

	m_nDataSeq2		= VALUE_ZERO;				// �ι�° ������ ����
	m_dYPoint2		= VALUE_ZERO;				// �ι�° ������ ��
}

CRegistNSupportData::~CRegistNSupportData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRegistNSupportData
CPatternEnvData::CPatternEnvData()
{
	m_nMAPeriod				= 5;				// �̵���� �Ⱓ

	m_bDrawMMa				= FALSE;			// �����̵���� ����
	m_bDrawHighLowPtLine	= TRUE;				// ����, ���� �������

	m_bDrawHighPtHLine		= FALSE;			// ���� �������
	m_bDrawLowPtHLine		= FALSE;			// ���� �������

	m_bDrawHighRegistLine	= TRUE;				// ���� ���׼�
	m_bDrawLowSupportLine	= TRUE;				// ���� ������

	m_dHLPtAllowalbeError	= 0;				// �ֿ����, ���� ���� ����, ���� ���̰� n% �̳��� ��� �����ͷ� ������� ����
	m_dNextPtAllowableError	= 1;				// �ΰ��� �ֿ������� ����α��� ���������� ���̰� n%�̳��� ��� �����ͷ� ���

	m_bFilterSideWalkPeriod	= TRUE;				// Ⱦ������ ���͸� ����
}

CPatternEnvData::~CPatternEnvData()
{
}

