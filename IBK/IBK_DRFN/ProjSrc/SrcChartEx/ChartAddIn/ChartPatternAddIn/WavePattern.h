#if !defined(AFX_WAVEPATTERN_H__0E8F5C27_A5C8_46D8_A109_1906BC06A986__INCLUDED_)
#define AFX_WAVEPATTERN_H__0E8F5C27_A5C8_46D8_A109_1906BC06A986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WavePattern.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CWavePattern

class CWavePattern : public CObject
{
// Construction
public:
	CWavePattern();
	virtual ~CWavePattern();



// ***************************************************************************************
//	Find Wave Pattern
// ***************************************************************************************
public:
	// Regist & Support Line
	BOOL	Get_RegistNSupportLine(		CObject *pObjHighLowPtData, CObject *pObjRegistNSupportData, double dNextPtAllowableError);

	// Header & Shoulder
	BOOL	Find_HeaderNShoulders(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Reversal Header & Shoulder
	BOOL	Find_R_HeaderNShoulders(	CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	
	// Double Top(�ֺ���)
	BOOL	Find_DoubleTop(				CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Reversal Double Top(���ֺ���)
	BOOL	Find_R_DoubleTop(			CObject *pObjHighLowPtData, CObject *pObjWavePatternData);

	// Bull Isosceles Triangles(�����̵�ﰢ��)
	BOOL	Find_Bull_IsoTriangles(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Bear Isosceles Triangles(�༼�̵�ﰢ��)
	BOOL	Find_Bear_IsoTriangles(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);

	// Bull Triangles(��»ﰢ��)
	BOOL	Find_Bull_Triangles(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Bear Triangles(�϶��ﰢ��)
	BOOL	Find_Bear_Triangles(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);

	// Bullish Wide Triangles(���Ȯ��ﰢ��)
	BOOL	Find_Bullish_WideTriangles( CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Bearish Wide Triangles(�϶�Ȯ��ﰢ��)
	BOOL	Find_Bearish_WideTriangles( CObject *pObjHighLowPtData, CObject *pObjWavePatternData);

	// Bull Wedge(���������)
	BOOL	Find_Bull_Wedge(			CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Bear Wedge(���������)
	BOOL	Find_Bear_Wedge(			CObject *pObjHighLowPtData, CObject *pObjWavePatternData);

	// Bull Square Flag(��»簢�����)
	BOOL	Find_Bull_SquareFlag(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Bear Square Flag(�϶��簢�����)
	BOOL	Find_Bear_SquareFlag(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);

	// Bull Triangle Flag(��»ﰢ�����)
	BOOL	Find_Bull_TriangleFlag(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Bear Triangle Flag(�϶��ﰢ�����)
	BOOL	Find_Bear_TriangleFlag(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);

	// Bull Rectangle(������簢��)
	BOOL	Find_Bull_Rectangle(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// Bear Rectangle(�϶����簢��)
	BOOL	Find_Bear_Rectangle(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);

	// V Top
	BOOL	Find_V_Top(					CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	// V Bottom
	BOOL	Find_V_Bottom(				CObject *pObjHighLowPtData, CObject *pObjWavePatternData);


	// Add New Pattern Data
	void	AddNewWavePatternData(	CObject* pObjWavePatternData,
									int nPatternType,		int nStartSeq,			int nEndSeq,		
									double dDrawSeq1_1,		double dDrawYPt1_1,		double dDrawSeq1_2,		double dDrawYPt1_2,	
									double dDrawSeq2_1,		double dDrawYPt2_1,		double dDrawSeq2_2,		double dDrawYPt2_2,
									double dNeckSeq1,		double dNeckYPt1,		double dNeckSeq2,		double dNeckYPt2,
									double dNeckBaseYPt1,	double dNeckBaseYPt2	);
	
public:
	// Find All Wave Pattern
	void	Find_All_WavePattern(		CObject *pObjHighLowPtData, CObject *pObjWavePatternData);
	

	CString			GetPatternName(		int nPatternType);
	int				GetPatternTrend(	int nPatternType);
	CString			GetPatternTrendName(int nPatternType);


public:
	double		GetGradient(double dXPos1, double dYPos1, double dXPos2, double dYPos2);		// ������ ������ ���⸦ ���Ѵ�
	double		GetModifyGradient(double dXPos1, double dYPos1, double dXPos2, double dYPos2);	// ������ ������ ������ ���⸦ ���Ѵ�

// 2008.02.20 by LYH >>
protected:
	HWND m_hOcxWnd;
// 2008.02.20 by LYH <<
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEPATTERN_H__0E8F5C27_A5C8_46D8_A109_1906BC06A986__INCLUDED_)
