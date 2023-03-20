#pragma once


// CControlwnd
class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};
class CControlwnd : public CWnd
{
	DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	virtual ~CControlwnd();

	CWnd*		m_pParent;
	CWnd*		m_wnd;
	CParam		m_Param;

	HBITMAP		m_hBitmap, m_hBitmap_dn, m_hBitmap_hv;

	bool		m_capture, m_bChecked, m_bCheck, m_bDisable;
	BOOL		m_bHover, m_bTracking;

	int	m_kind;
	int	m_value;
	int	m_type;
	CString	m_domino;
	CString m_sRoot;
	CString m_sImagePath;

	CBitmap* getBitmap(CString path);
	CString Variant(int comm, CString data);
	CString	Parser(CString &srcstr, CString substr);
	CString OptionParser(CString sOption, CString sKey);
	COLORREF GetIndexColor(int index);

	void SetParam(struct _param *pParam);
	void SetButtonImages();
	void process_Xtr(CString dat);

	void sendTR(CString trC, CString dat);
	void sendTR(CString trC, char* pBytes, int nBytes);
protected:
	afx_msg	LONG OnUSER(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnMSG(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnCHILD(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	virtual void OnFinalRelease();
};


