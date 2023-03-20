#if !defined(AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_)
#define AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatternMaker.h : header file
//

#include "./include_Addin/I423611/_IChartOCX.h"	// for IChartOCX

#include "PatternDBItems.h"								// Pattern DB Items
#include "WavePattern.h"								// for CWavePattern
#include "CandlePattern.h"								// for CCandlePattern


/////////////////////////////////////////////////////////////////////////////
// CPatternMaker Object

class CDlgAnalysis;
class CPatternMaker : public CObject
{
// Construction
public:
	CPatternMaker();
	virtual ~CPatternMaker();

// IChartOCX
protected:
	IChartManager	*m_pIChartManager;
	IChartOCX		*m_pIChartOCX;
	IPacketManager	*m_pIPacketManager;
public:
	void			SetIChartOCX( IChartManager *p_pIChartManager, IChartOCX *p_pIChartOCX, IPacketManager *p_pIPacketManager);
	void			SetGSharedData(CString strKey, CString strData);


public:
	CPoint			m_pointPrev;
	int				m_nSamePointCnt;
	BOOL			m_bSelWavePtn;

	CPatternEnvData	m_PatternEnvData;		// ���� ȯ�漳�� ������

	
public:	
	// Draw
	void		OnDraw( CDC *p_pDC);
	void		RefreshChart();

	
protected:
	CWavePattern	m_wpWavePattern;
	CArrayObj(CWavePatternData)			m_aryWavePatternData;		// Data Array of a Wave Pattern which was found.
	CArrayObj(CHighLowPtData)			m_aryHighLowPtData;			// �ֿ����, �ֿ����� ������
	CArrayObj(CMAData)					m_aryMAData;				// �̵���� ������
	CArrayObj(CRegistNSupportData)		m_aryRegistNSupportData;	// ����, ������ ������


public:
	// Pattern Data
	void		GetAllData_AboutPattern();

	void		GetFoundPatternCnt(int *p_nFindPatternCnt);										// Get Count of Pattern which was found

	int			GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion);	// X��ǥ�� ���Ѵ�
	BOOL		GetChartBasicData(	int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion);


	// Selected Pattern Data
	CWavePatternData		m_selWavePtternData;					// Selected Pattern Data				
	CSelPatternData4Draw	m_spdDrawingData;						// Selected Pattern Data for Drawing

	void		InitSelectedPatternData(BOOL bReal=FALSE);
	void		CheckSelectedPatternData_ForReal();


	// Find Wave Pattern at Click Down Position
	BOOL		OnLButtonDown( UINT nFlags, CPoint &point);
	BOOL		OnLButtonUp( UINT nFlags, CPoint &point);
	BOOL		FindPtn_OnLBtnDownPos( UINT nFlags, CPoint &point, int nSamePointCnt);


	// Analysis Dialog of a Pattern
	CDlgAnalysis	*m_pDlgAnalysis;	
	void		CreateDlgAnalysis( IChartOCX *p_pIChartOCX, int nType, BOOL bPatternShow = TRUE);		// Create Analysis Dialog


	// Destory Analysis Dialog
	void		DestroyAnalysisDialog();

	// Delete All Pen
	void		DeleteAllPen();															// Delete All Pen
	
	// Delete Data 
	void		DeleteAllData();														// Delete All Data Concern with Pattern
	void		DeleteHighLowPtData();													// �ֿ����, �ֿ����� ������ ����
	void		DeleteMAData();															// �̵���� ������ ����
	void		DeleteRegistNSupportData();												// ����, ������ ������ ����
	void		DeleteWavePatternData();												// Delete Wave Pattern Data


protected:
	CPen		m_pnHighLowLinePen;			// �ֿ���� ���� �������
	CPen		m_pnMaPen;					// �̵����
	CPen		m_pnHighLinePen;			// �ֿ���� �������
	CPen		m_pnLowLinePen;				// �ֿ����� �������

	CPen		m_pnPatternWave;			// Pattern Wave
	CPen		m_pnUpwardReverse;			// Pattern Upward Reverse
	CPen		m_pnDnwardReverse;			// Pattern Downward Reverse
	CPen		m_pnUpwardContinue;			// Pattern Upward Continue
	CPen		m_pnDnwardContinue;			// Pattern Downward Continue
	
	BOOL		m_bLog;						// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	BOOL		m_bReverse;					// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	
	double		m_dViewMin;					// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	double		m_dViewMax;					// View���� ��� Graph�� View�� Maximum Scale ���̴�.


// Candle Pattern
protected:
	CCandlePattern		m_cpCandlePattern;
	CCandlePatternData	m_selCandlePatternData;
	CArrayObj(CCandlePatternData)	m_aryCandlePatternData;		// Data Array of a Candle Pattern which was found
	CArrayObj(CCandlePatternPosInfo)	m_aryCandlePatternPosInfo;		// Data Array of a Candle Pattern which was found

public:
	void		DeleteCandlePatternData();
	void		GetAllData_AboutCandlePattern();

	void		InitSelectedCandlePatternData();
	void		SetSelectedCandlePatternData( double dTime, CString strPatternName);
	void		ResetScrollPosition( int nDataPos);

public:
	BOOL		OnMouseMove( UINT nFlags, CPoint &point, CDC* pDC);
	void		DeleteCandlePatternPosInfo();
	CToolTipCtrl m_toolTip;
	void		RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
	CPoint		SetToolTipText(CString s, CRect rect);
	CWnd*		m_hParentWnd;

	//2006.09.11 by LYH �߰�(draw �Լ� ����)
	//void CPatternMaker::DrawWaveLine( CDC *p_pDC, int i, CHighLowPtData	*pHighLowPtData, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion,  int& nXHigh, int& nYHigh, int& nXLow, int& nYLow);
	void DrawWaveLine( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion);
	void DrawMALine( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion);
	void DrawAutoTrendLine( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion);
	void DrawAutoPattern( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion);
	void DrawCandlePattern( CDC *p_pDC, int nStartIndex, int nEndIndex, double dCandleWidth, CRect rctGraphRegion);

	BOOL m_bDrawAutoTrendLine;
	BOOL m_bDrawAutoPattern;
	BOOL m_bDrawCandlePattern;

	CString	m_strRQ;		// ���ǰ� �ִ� RQ :�������� - ojtaso (20070307)
	void	SetRQ(LPCTSTR lpszRQ) {m_strRQ = lpszRQ; }
	LPCTSTR GetRQ() { return m_strRQ; };
// 2008.02.20 by LYH >>
protected:
	HWND m_hOcxWnd;
// 2008.02.20 by LYH <<
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_)
