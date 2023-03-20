#if !defined(AFX_CTRLWND_H__C8DF34A6_44CF_45D8_8E68_C1D94938E658__INCLUDED_)
#define AFX_CTRLWND_H__C8DF34A6_44CF_45D8_8E68_C1D94938E658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtrlWnd.h : header file
//


class CParam
{
public:
	int			key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int			point;			// font point
	int			style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

/////////////////////////////////////////////////////////////////////////////
// CCtrlWnd window

//class CFxInterestManager;

class CCtrlWnd : public CWnd
{
// Construction
public:
	CCtrlWnd(CWnd* pWizard, _param* pParam);

// Attributes
public:

// Operations
public:
	BOOL Initialize(BOOL bDLL);
	void SetParam(struct _param *pParam);
	CString OptionParser(CString strOption, CString strKey);
	CString	Parser(CString &strSource, CString strSep = _T("\t"));
	void SetButtonImages();
	CString Variant(int nComm, CString strData = _T(""));
	CBitmap* GetBitmap(CString strPath);
	COLORREF GetIndexColor(int nIndex);
	CFont* GetFont(CString strFontName, int nPoint, int style);
	CFont* GetFont();
	void ShowFileDialog(CRect rc);	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:	
	
	virtual ~CCtrlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCtrlWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CCtrlWnd)
	afx_msg void SetMode(short nMode);
	afx_msg BSTR GetPath();
	afx_msg void SetFileName(LPCTSTR strFileName);
	afx_msg void SetPath(LPCTSTR strPath);
	afx_msg void Enable(BOOL bEnable);
	afx_msg void SetFileFilter(LPCTSTR strFilter);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	CWnd* m_pWizard;
	CParam m_Param;

	CString m_strImgPath;
	CString m_strRoot;
	CString m_strName;
	CString m_strText;
	CString m_strSelectGroupKey;
	CString m_strFileFilter;

	COLORREF m_clrBtnText;

	CBitmap* m_pBitmap;
	CBitmap* m_pBitmap_dn;
	CBitmap* m_pBitmap_en;

	BOOL m_bCheck;
	BOOL m_bChecked;
	BOOL m_bCapture;
	BOOL m_bHOver;
	BOOL m_bTracking;	

	int	 m_nDialogMode;
	CString m_strPathName;
	CString m_strFileName;
	BOOL m_bEnable;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTRLWND_H__C8DF34A6_44CF_45D8_8E68_C1D94938E658__INCLUDED_)
