#if !defined(AFX_FXSPINEX_H__ABD4C216_142A_4D93_B7D3_B4527763F614__INCLUDED_)
#define AFX_FXSPINEX_H__ABD4C216_142A_4D93_B7D3_B4527763F614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif


class CSpinEditEx;
class CSpinCtrlEx;

class AFX_EXT_CLASS CfxSpinEx : public CStatic
{
public:
	CfxSpinEx(CFont* pFont = NULL);
	virtual ~CfxSpinEx();

	//{{AFX_VIRTUAL(CfxSpinEx)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CfxSpinEx)
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnEditChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	BOOL	Create(DWORD attr, DWORD spType, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void	Init(DWORD attr, DWORD spType, CWnd* pParentWnd);
	void	SetAttribute(DWORD attr);
	void	SetSpinType(DWORD remove, DWORD add);
	void	SetColors(COLORREF crBack, COLORREF crFont, bool redraw = true);

	void	SetText(CString text);	
	CString	GetText();
	void	SetRangeEx(double lower, double upper);
	void	GetRangeEx(double &lower, double &upper);
	void	SetDelta(double delta, bool redraw = true);
	double	GetDelta();
	void	SetPos(double nPos);
	double	GetPos();
	void	SetDecimalPlaces(int digit = 0, bool redraw = true);
	void	SetDirection(BOOL nFlag = TRUE);
	void	SetLimitText(int len);
private:
	void	createControls();

public:
	CSpinEditEx*	m_pEdit;
private:
	CWnd	*m_parent;
	DWORD	m_spType;
	DWORD	m_attr;

	CFont*		m_pFont;
	CSpinCtrlEx*	m_pSpin;
};


class CSpinEditEx : public CEdit
{
public:
	CSpinEditEx(CWnd* pWnd, DWORD attr);
	virtual ~CSpinEditEx();
	//{{AFX_VIRTUAL(CSpinEditEx)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSpinEditEx)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	SetUseColor(bool bColor);
	void	SetColors(COLORREF clrBACK, COLORREF clrFORE, bool redraw = true);
	void	SetRange(double lower, double upper);
	void	SetText(CString text, bool redraw = true);
	CString	GetText();
	void	SetAtrribute(DWORD attr, bool redraw = true);
	void	SetDecimalPlaces(int digit);

private:
	void	formatComma(CString& text);
	void	commaText();
private:
	CWnd		*m_pWnd;
	DWORD		m_attr;
	COLORREF	m_clrFORE;
	COLORREF	m_clrBACK;
	CBrush		m_brBACK;
	bool		m_bColor;
	double		m_upper;
	double		m_lower;
	int		m_digit;
	bool		m_bFocus;
};

class CSpinCtrlEx : public CSpinButtonCtrl
{
public:
	CSpinCtrlEx(CWnd* pWnd, DWORD spType, DWORD attr);
	virtual ~CSpinCtrlEx();
	//{{AFX_VIRTUAL(CSpinCtrlEx)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSpinCtrlEx)
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	SetPos(double nPos);
	double	GetPos();
	void	SetSpinType(DWORD spType, bool redraw = true);
	void	SetAttribute(DWORD attr, bool redraw = true);
	void	SetDirection(int direction);
	void	SetDecimalPlaces(int digit);
	double	GetDelta();
	void	SetDelta(double delta);
	
private:
	void	formatComma(CString& text);
	void	checkDelta(BOOL nFlag);
private:
	CWnd	*m_pWnd;
	DWORD	m_spType;
	DWORD	m_attr;

	int	m_direction;
	int	m_digit;
	double	m_delta;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXSPINEX_H__ABD4C216_142A_4D93_B7D3_B4527763F614__INCLUDED_)
