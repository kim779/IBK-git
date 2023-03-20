#pragma once
// MGrpWnd.h : header file
//

struct _MChartCode{
	int		ctype;	// ���� 1, �� 2, ǲ 3
	int		atmgap;	// atm���� ����
	int		Num;	// ����
	int		mmgb;	// �Ÿű��� (-1. �ŵ�, +1. �ż�)


	double		ySum;	// ���������� ������� y�� �հ�
};

/////////////////////////////////////////////////////////////////////////////
// CMGrpWnd window

class CMGrpWnd : public CWnd
{
// Construction
public:
	CMGrpWnd(CWnd* pView, CWnd* parent);

// Attributes
public:
	CIB410100App*	m_pApp{};
	CWnd*			m_pParent{};
	CWnd*			m_pView{};
	COLORREF		m_bkColor{};

	std::unique_ptr<CBitmap>	m_bmpChart{};
	int	m_nStgCode{};
	//CArray <struct _MChartCode, struct _MChartCode>		m_chartCode;

	bool		m_bRedraw{};

// Operations
public:
	void	drawChart(CDC *dc, CRect rcClient);
	void	Redraw();
	int	GetPerentX(CRect rcDraw, double perX);
	int	GetPerentY(CRect rcDraw, double perY);
	void	DrawZero(CDC *dc, CRect rcDraw, int zeroY, int x, bool upText = true, CString text = "ATM");


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMGrpWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMGrpWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMGrpWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

