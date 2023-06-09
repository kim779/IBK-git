#pragma once


// CCtrlWnd
#include "afxtempl.h"
enum { ctl_NO = 0, ctl_ZOOMOUT, ctl_ZOOMIN, ctl_PRE, ctl_STOP, ctl_NEXT, ctl_EXCEL };

class _ctlINFO
{
public:
	bool	pushed{};
	CRect	rect;
	CString text;
	int	imgdown{};
	int	imgup{};
};


class CCtrlWnd : public CWnd
{
	DECLARE_DYNAMIC(CCtrlWnd)

public:
	CCtrlWnd(CWnd* pView, CWnd* pParent);

	enum { autoPre = -1, autoStop, autoNext };

	virtual ~CCtrlWnd();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT	OnCtrl(WPARAM wParam, LPARAM lParam);

public:
	void	InitCtrl(CString sDay);

private:
	void	AddControlClass(int key, CRect rc, CString text);
	void	DeleteAllBtn();
	int	SearchPointInControl(CPoint pt);

	void	InitScroll(int pos, int end, int day, int total);
	void	RedrawCtrl(bool bResize);
	void	UpdateScroll(bool bGetData = true);
	void	GetScrInfo();
	void	ParsingScrInfo(CString tmpS);
	void	DrawBtn(CDC* pDC, int selectID, _ctlINFO* pInfoCTL);

	void	setAutoScroll(int autoID);
	void	AutoShiftAction();

public:
	int	m_totalDay;
	int	m_dispPos;
	int	m_dispEnd;
	int	m_dispDay;

private:
	CWnd* m_pView;
	CWnd* m_pParent;
	std::unique_ptr<class	CExScrollBar>	m_pScroll;
	std::unique_ptr<class	CExSlider>	m_pSlider;
	std::map<int, std::unique_ptr<_ctlINFO>> _mapCont;

	int	m_ctrlSAVE;
	CString	m_strTemp;

	COLORREF	m_clrBack;
	COLORREF	m_clrText;
	COLORREF	m_clrForm;

	bool		m_bCtrlEnable;
	bool		m_bCapture;
	bool		m_bLDown;
	int		m_nSlider;

	CTestctrlApp* m_pApp;
	CImageList* m_pCtrlImage;

	int			m_TimerID;
	int			m_AutoScroll;
};


