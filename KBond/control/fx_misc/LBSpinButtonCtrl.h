// LBSpinButtonCtrl.h : header file

/////////////////////////////////////////////////////////////////////////////
//****************************************************************************

#if !defined(AFX_LBSPINBUTTONCTRL_H__16B8B6EF_C905_11D3_AA40_008048EAE3F5__INCLUDED_)
#define AFX_LBSPINBUTTONCTRL_H__16B8B6EF_C905_11D3_AA40_008048EAE3F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLBSpinButtonCtrl window

class CLBSpinButtonCtrl : public CSpinButtonCtrl
{
// Construction
public:
	CLBSpinButtonCtrl();
	
// Attributes
public:
	CWnd		*m_pParent;
	COLORREF	m_frColor;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLBSpinButtonCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor(COLORREF clr) { m_frColor = clr; Invalidate();}
	bool SetAutoDisable(bool bSetOn);
	void SetParent(CWnd *parent) { m_pParent = parent; }
	virtual ~CLBSpinButtonCtrl();

protected:	
	void DrawUpButton(CDC *pDC, CRect rect);
	void DrawDownButton(CDC *pDC, CRect rect);
	virtual void Init();
	void CleanUpHook() const;
	void DrawPressedRect(CDC &dc,const CRect& rctDown) const;
	void DisableRect(CDC &dc, const CRect &rectDisable) const;

protected:
	HWND m_hWndBuddy;

	bool m_bAutoDisable;
	bool m_bBuddyIsEdit;
	bool m_bDefaultDirection;
	bool m_bActiveSpinPressed;
	bool m_bVertical;

	CRect m_rctIsPressed;
	CRect m_rctClient;

	int m_nPrevPos;
	int m_nMinPos;
	int m_nMaxPos;

	UINT m_nSpinAlign;
	UINT m_nSpinState;
	enum m_nState {BothEnable=0,DisableRight,DisableLeft,BothDisable};
	enum m_nPosition{Outside=0,OnRightInside,OnLeftInside};

protected:
	//{{AFX_MSG(CLBSpinButtonCtrl)
	afx_msg BOOL OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	HPEN m_penLightShadow;
	HPEN m_penDarkShadow;
	HPEN m_penShadow;
	HPEN m_penLight;
	HPEN m_penButtonFace;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_LBSPINBUTTONCTRL_H__16B8B6EF_C905_11D3_AA40_008048EAE3F5__INCLUDED_)
