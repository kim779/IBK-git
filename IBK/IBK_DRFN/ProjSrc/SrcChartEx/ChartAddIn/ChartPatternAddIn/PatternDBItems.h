#if !defined(AFX_PATTERNDBITEMS_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_)
#define AFX_PATTERNDBITEMS_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatternDBItems.h : header file
//


// Define Macro for CArray ===============================================>>
#include <afxtempl.h>			// for CArray
#define CArrayObj(CObject)		CArray <CObject*, CObject*>
#define CArrayObjPt(CObject)	CArray <CObject*, CObject*> *
// <<=======================================================================



// **************************************************************************
// CHighLowPtData Object				(�ֿ����, ���� ������)
// **************************************************************************
class CHighLowPtData : public CObject
{
// Construction
public:
	CHighLowPtData();
	virtual ~CHighLowPtData();

public:
	int				m_nPointType;				// ���� ���� ����

	int				m_nDataSeq;					// ������ ����
	double			m_dTime;					// �ð�
	
	double			m_dOpen;					// �ð�
	double			m_dHigh;					// ��
	double			m_dLow;						// ����
	double			m_dClose;					// ����
};


// **************************************************************************
// CMAData Object						(�̵���� ������)
// **************************************************************************
class CMAData : public CObject
{
// Construction
public:
	CMAData();
	virtual ~CMAData();

public:
	int				m_nDataSeq;					// ������ ����

	double			m_dMMa;						// �����̵����
};


// **************************************************************************
// CRegistNSupportData Object				(������, ���׼� ������)
// **************************************************************************
class CRegistNSupportData : public CObject
{
// Construction
public:
	CRegistNSupportData();
	virtual ~CRegistNSupportData();

public:
	int				m_nRSLineType;				// ���׼�, ������ ����

	int				m_nDataSeq1;				// ù��° ������ ����
	double			m_dYPoint1;					// ù��° ������ ��

	int				m_nDataSeq2;				// �ι�° ������ ����
	double			m_dYPoint2;					// �ι�° ������ ��
};


// **************************************************************************
// CWavePatternData Object					(Wave Pattern Data)
// **************************************************************************
class CWavePatternData : public CObject
{
// Construction
public:
	CWavePatternData();
	virtual ~CWavePatternData();

public:
	int				m_nPatternType;				// Pattern Type
	BOOL			m_bIsSelect;				// Pattern Select

	int				m_nStartSeq;				// Start Sequence of a Pattern
	int				m_nEndSeq;					// End Sequence of a Pattern

	double			m_dDrawSeq1_1;				// First Draw Data
	double			m_dDrawYPt1_1;
	double			m_dDrawSeq1_2;				
	double			m_dDrawYPt1_2;

	double			m_dDrawSeq2_1;				// Second Draw Data
	double			m_dDrawYPt2_1;
	double			m_dDrawSeq2_2;
	double			m_dDrawYPt2_2;

	double			m_dNeckSeq1;				// First Neck Data
	double			m_dNeckYPt1;
	double			m_dNeckSeq2;				// Second Neck Data
	double			m_dNeckYPt2;
	
	double			m_dNeckBaseYPt1;
	double			m_dNeckBaseYPt2;
};

// **************************************************************************
// CLineDrawData Object					(Environment Setup of Line)
// **************************************************************************
class CLineDrawData : public CObject
{
// Construction
public:
	CLineDrawData();
	virtual ~CLineDrawData();

public:
	int m_nWeight;
	COLORREF m_color;
};


// **************************************************************************
// CPatternEnvData Object					(Environment Setup of Pattern)
// **************************************************************************
class CPatternEnvData : public CObject
{
// Construction
public:
	CPatternEnvData();
	virtual ~CPatternEnvData();
	BOOL IsDisplayCandle(int nCandleType);

public:
	BOOL			m_bNotDrawAll;					// ��� ���� �׸��� ����

	int				m_nDrawMethod;					// �ڵ��߼��� �۵���(0: �����̵���չ�, 1:����ĵ��)
	int				m_nMAPeriod;					// �̵���� �Ⱓ
	int				m_nCandleCnt;					// ���� ĵ�� ����
	int				m_nCandleAfterCnt;				// �� ĵ�� ����

	BOOL			m_bDrawAutoPatternLine;			// �ڵ��߼�����
	BOOL			m_bDrawHighLowPtLine;			// �ĵ���
	BOOL			m_bDrawMMa;						// �����̵���� ����
	
	BOOL			m_bDrawHighPtHLine;				// ���� �������
	BOOL			m_bDrawLowPtHLine;				// ���� �������

	BOOL			m_bDrawHighRegistLine;			// ���� ���׼�
	BOOL			m_bDrawLowSupportLine;			// ���� ������

	double			m_dHLPtAllowalbeError;			// �ֿ����, ���� ���� ����, ���� ���̰� n% �̳��� ��� �����ͷ� ������� ����
	double			m_dNextPtAllowableError;		// �� �ֿ����� ������ ������ ���� �ֿ������� ���̰� n% �̳��� ��� �����ͷ� ���

	BOOL			m_bFilterSideWalkPeriod;		// Ⱦ������ ���͸� ����

	CLineDrawData	m_lineDataHighLowPt;			// �ĵ���
	CLineDrawData	m_lineDataHighPtH;				// ���� �������
	CLineDrawData	m_lineDataLowPtH;				// ���� �������
	CLineDrawData	m_lineDataHighRegist;			// ���� ���׼�
	CLineDrawData	m_lineDataLowSupport;			// ���� ������

	BOOL			m_bDrawPatternName;				// ĵ������ �̸�ǥ��
	CArray<int, int>	m_arrNotDisplayCandleItems;		//ĵ������ �� ���÷��� ���� �ʴ� ���� ����Ʈ
};

// **************************************************************************
// CSelPatternData4Draw Object				(Selected Pattern Data for Drawing)
// **************************************************************************
class CSelPatternData4Draw : public CObject
{
// Construction
public:
	CSelPatternData4Draw();
	virtual ~CSelPatternData4Draw();

public:
	BOOL			m_bDraw;
	int				m_nPatternType;

	int				m_nStartSeq;
	int				m_nEndSeq;

	int				m_nX1;
	int				m_nY1;
	int				m_nX2;
	int				m_nY2;
	
	int				m_nX3;
	int				m_nY3;
	int				m_nX4;
	int				m_nY4;

	int				m_nNeckXPos1;
	int				m_nNeckYPos1;
	int				m_nNeckXPos2;
	int				m_nNeckYPos2;

	int				m_nNeckXLength;
	int				m_nNeckYLength1;
	int				m_nNeckYLength2;
};


// **************************************************************************
// CCandlePatternData Object				(Candle Pattern Data)
// **************************************************************************
class CCandlePatternData : public CObject
{
// Construction
public:
	CCandlePatternData();
	virtual ~CCandlePatternData();

public:
	double			m_dTime;
	int				m_nCandlePtnType;
	
	int				m_nStartPos;
	int				m_nEndPos;

	double			m_dClose[6];		// Close Price for Accumulation rise and fall rate
	double			m_dHigh;			// 2006.07.21 Add by LYH for paint on chart candle.
	double			m_dLow;			// 2006.07.21 Add by LYH for paint on chart candle.
};

class CCandlePatternPosInfo : public CObject
{
	// Construction
public:
	CCandlePatternPosInfo();
	virtual ~CCandlePatternPosInfo();

public:
	CRect			m_rect;
	int				m_nCandlePtnType;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERNDBITEMS_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_)
