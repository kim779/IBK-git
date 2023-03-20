#pragma once
// ContractGrid.h : header file
//
#include <afxtempl.h>
enum { clr_UpIndex = 0, clr_NormalIndex, clr_DnIndex};

#define MAX_EQUALIZER		(10)

class CMatch {
public:
	CString price;
	CString contract;
	int	cp;	// price color index(0,1,2)
	int	cc;	// contract color index(0,1,2)

	CMatch(const char* p, const char* q, int cp, int cc): price(p), contract(q), cp(cp), cc(cc) {}
};

/////////////////////////////////////////////////////////////////////////////
// CContractGrid window

class CContractGrid : public CWnd
{
// Construction
public:
	CContractGrid(CWnd *pWizard, CWnd *pParent);

// Attributes
public:
	std::unique_ptr<class CContractScroll>	m_pScroll;

// Operations
public:
	void	Resize();
	void	SetDisplay(int display)	{ m_iDispCnt = display; }
	void	ChangeDisplay(int display);
	void	SetFontx(CFont *pFont);
	//**void	InsertData(CString sPrice, CString sVolume, int nRow);
	void	AutoScroll();
	void	Init(int DisplayCnt)	{ m_iDispCnt = DisplayCnt;	}
	void	Clear();
	void	PaletteGrid(COLORREF upColor, COLORREF downColor, COLORREF normalColor, 
		COLORREF lineColor, COLORREF gridColor1st, COLORREF gridColor2st);
	void	UpdateScroll(int pos, int end, int day, int total);
	void	DrawData(CDC *pDC);
	void	DrawEqualizer(CDC *pDC, const CRect& rc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContractGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CContractGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CContractGrid)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg long OnVScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	class CCX_DepthApp	*m_pApp;
	CWnd	*m_pWizard, *m_pParent;

	COLORREF m_pColor[3];	// ���,����,�϶�
	COLORREF m_colorUp = 0, m_colorDown = 0, m_colorNormal = 0;
	COLORREF m_colorLine = 0, m_colorGrid1st = 0, m_colorGrid2st = 0;
	CFont	*m_pFont;
	
	CRect	m_rcData;
	int	m_SCRpos, m_SCRday, m_SCRend, m_SCRtotal;	// ������ ������ġ, �׷����°���, ����ġ(m_SCRpos+m_SCRday), ��ü����
	int	m_iDispCnt;	// �������� ������ ����(5 or 10)
	int	m_iHalfLine;	// �߾� ���μ�

	// Equalizer ------------------------------------------------
	int	m_eq_type = 0;
	int	m_eq_cnt = 0;
	BOOL	m_eq_alarm[2]{};		// 0:�ŵ� , 1:�ż�
	int	m_eq_alarm_lev[2]{};		// 0:�ŵ� , 1:�ż�
	CString m_eq_alarm_wav[2]{};		// 0:�ŵ� , 1:�ż�
	CBitmap	eq_bmp[3]; // 0:�ŵ�, 1:�ż�, 2:����
	double	m_eq_lev[MAX_EQUALIZER]{};		// ü�ᷮ ���� ����
	int	m_eq_vol[MAX_EQUALIZER]{};		// ���Ұ��� ���� ����
	COLORREF m_eq_mdcol[MAX_EQUALIZER]{};	// �ŵ� ����
	COLORREF m_eq_mscol[MAX_EQUALIZER]{};	// �ż� ����
	COLORREF m_eq_nmcol[MAX_EQUALIZER]{};	// �Ϲ� ����
	void LoadEqualizer();
	void MakeEqualizerBmp();
	// ----------------------------------------------------------
private:
	BOOL m_showTitle;
	void SetShowTitle(BOOL showTitle)
	{
		if (m_showTitle != showTitle)
		{
			m_showTitle = showTitle;
			Invalidate();
		}
	}

	int		m_iHalfInfo;
//	CTypedPtrList<CPtrList, CMatch*> m_match;
	CList <std::shared_ptr<CMatch>, std::shared_ptr<CMatch>> m_match;
	int GetCount() const 	{ return m_match.GetCount();	}

public:
	void SetHalfLineInfo(int iHalf) { m_iHalfInfo = iHalf; }
	void InsertData(const char* price, const char* contract, int cp, int cc);
	void AddData(const char* price, const char* contract, int cp, int cc);
	void AutoUpdate();
};