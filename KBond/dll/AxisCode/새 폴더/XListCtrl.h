#pragma once


// CXListCtrl
#include "XHeaderCtrl.h"

extern UINT NEAR WM_XLISTCTRL_COMBOBOX_CHANGE;
extern UINT NEAR WM_XLISTCTRL_CHECKBOX_CLICKING;
extern UINT NEAR WM_XLISTCTRL_EDITBOX_CHANGE;
extern UINT NEAR WM_XLISTCTRL_DATEBOX_CHANGE;

class CSortData
{
public:
	CSortData(CString str, DWORD dw)
	{
		m_strText =str;
		m_dw = dw;
	};
	~CSortData() {};

	 CString m_strText;
	DWORD m_dw;
};

typedef struct tagNMITEM
{
	HWND hwndFrom;
	int	iItem;
	int	iSubItem;
	CString sText;
} NMITEM;


// JCW_U 2008.04.12 START
struct XLISTCTRLDATA
{
	// ctor
	XLISTCTRLDATA()
	{
		bEnabled             = TRUE;
		crText               = ::GetSysColor(COLOR_WINDOWTEXT);
		crBackground         = RGB(255,255,255);//::GetSysColor(COLOR_WINDOW);
		bShowProgress        = FALSE;
		nProgressPercent     = 0;
		strProgressMessage   = _T("");
		bShowProgressMessage = TRUE;
		nCheckedState        = -1;
		bBold                = FALSE;
		nImage               = -1;
		bImageCenter		 = FALSE;

		crBar				 = RGB(0,200,0);

		nTextLeftmode		 = 0;

#ifndef NO_XLISTCTRL_TOOL_TIPS
		strToolTip           = _T("");
#endif
		//
		dwItemData           = NULL;

		// for Combo Box
		bCombo               = FALSE;

		// for Date Box
		bDateBox			 = FALSE;
		sDateFormat			 = _T("yyyy-MM-dd_HH:mm:ss");
		dwDateStyle			 = DTS_SHORTDATEFORMAT|WS_VISIBLE|WS_CHILD|WS_TABSTOP;

		// for Edit Box
		bEditBox			 = FALSE;
		nEditLimitText		 = -1;
		nEditMinNum			 = -1;
		nEditMaxNum			 = -1;
		dwEditStyle			 = ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE;
	}

	BOOL			bEnabled;				// TRUE = enabled, FALSE = disabled (gray text)
	BOOL			bBold;					// TRUE = display bold text
	int				nImage;					// index in image list, else -1
	BOOL			bImageCenter;

#ifndef NO_XLISTCTRL_TOOL_TIPS
	CString			strToolTip;				// tool tip text for cell
#endif

	// for combo
	BOOL			bCombo;					// TRUE = display combobox
	CStringArray	sa;						// string array for combo listbox
	int				nComboListHeight;		// combo listbox height (in rows)
	int				nInitialComboSel;		// initial combo listbox selection (0 = first)

	// for color
	COLORREF		crText;
	COLORREF		crBackground;	
	COLORREF		crBar;

	// for editbox
	BOOL			bEditBox;
	int				nEditLimitText;
	int				nEditMinNum;
	int				nEditMaxNum;
	DWORD			dwEditStyle;

	// for datebox
	BOOL			bDateBox;
	CString			sDateFormat;
	DWORD			dwDateStyle;
	CTime			date;

	// for progress
	BOOL			bShowProgress;				// true = show progress control
	int				nProgressPercent;			// 0 - 100
	CString			strProgressMessage;			// custom message for progress indicator -
												// MUST INCLUDE %d
	BOOL			bShowProgressMessage;		// TRUE = display % message, or custom message
												// if one is supplied
	// for checkbox
	int				nCheckedState;				// -1 = don't show, 0 = unchecked, 1 = checked

	//
	int				nTextLeftmode;

	//
	DWORD			dwItemData;					// pointer to app's data
};
// JCW_U 2008.04.12 END


class CXListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CXListCtrl)

public:
	CXListCtrl();
	virtual ~CXListCtrl();

public:
	void	EnableFocusRect(BOOL bFocusRect=TRUE){m_bFocusRect=bFocusRect;}
	void	EnableResize(BOOL bResize=TRUE){m_bResize=bResize;}

	void	SetNoItemMsg(CString strNoItemMsg){m_strNoItemMsg=strNoItemMsg;}
	void	SetStatusColumn(int nSubItem){m_nStatusColumn=nSubItem;}

	void	SetBgColor(COLORREF crBg){m_crWindow=crBg;}
	void	SetBgColorProgress(COLORREF crBg){m_crBgProgress=crBg;}
	void	SetUnderLine(BOOL bUnderLine=TRUE){m_bUnderLine=bUnderLine;}
	void	SetColorUnderLine(COLORREF crUnderLine){m_crUnderLine=crUnderLine;}

	BOOL	GetProgressColor(int nItem, int nSubItem, COLORREF &cf);
	void	GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd);

	int		CountCheckedItems(int nSubItem);
	BOOL	DeleteAllItems();
	BOOL	DeleteItem(int nItem);
	void	DeleteProgress(int nItem, int nSubItem);
	BOOL	GetBold(int nItem, int nSubItem);
	int		GetCheckbox(int nItem, int nSubItem);
	int		GetColumns();
	CString	GetComboText(int iItem, int iSubItem);
	int		GetCurSel();
	BOOL	GetEnabled(int nItem);
	DWORD	GetExtendedStyleX() { return m_dwExtendedStyleX; }
	int		GetHeaderCheckedState(int nSubItem);
	DWORD	GetItemData(int nItem);
	BOOL	GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& rect);

	int		InsertItem(int nItem, LPCTSTR lpszItem);
	int		InsertItem(int nItem,
					   LPCTSTR lpszItem,
					   COLORREF crText,
					   COLORREF crBackground);
	int		InsertItem(const LVITEM* pItem);
	BOOL	SetBold(int nItem, int nSubItem, BOOL bBold);

	BOOL	SetComboBox(int nItem, 
						int nSubItem, 
						BOOL bEnableCombo, 
						CStringArray *psa=NULL);
	BOOL	SetEditBox(int nItem,
					   int nSubItem,
					   BOOL bEditBox=TRUE,
					   int nEditMinNum=-1,
					   int nEditMaxNum=-1,
					   int nEditLimitText=-1,
					   DWORD dwEditStyle=ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE);
	BOOL	SetDateBox(int nItem, 
					   int nSubItem, 
					   BOOL bDateBox=TRUE,
					   CString sDateFormat=_T("yyyy-MM-dd_HH:mm:ss"),
					   DWORD dwDateStyle=WS_VISIBLE|WS_CHILD|WS_TABSTOP|DTS_SHORTDATEFORMAT);
	void	UpdateDate(int nItem, int nSubItem, CTime time, COLORREF crText, COLORREF crBackground);

	BOOL	SetCheckbox(int nItem, int nSubItem, int nCheckedState);
	BOOL	SetCurSel(int nItem);
	BOOL	SetEnabled(int nItem, BOOL bEnable);
	DWORD	SetExtendedStyleX(DWORD dwNewStyle) 
	{
		DWORD dwOldStyle = m_dwExtendedStyleX;
		m_dwExtendedStyleX = dwNewStyle;
		return dwOldStyle;
	}

	BOOL	SetHeaderCheckedState(int nSubItem, int nCheckedState);
	int		SetItem(const LVITEM* pItem);
	BOOL	SetItemData(int nItem, DWORD dwData);
	
	BOOL	SetItemImage(int nItem, int nSubItem, int nImage, BOOL bImageCenter=FALSE);
	int		GetItemImage(int nItem, int nSubItem);

	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText); 
	BOOL	SetItemText(int nItem, 
						int nSubItem, 
						LPCTSTR lpszText,
						COLORREF crText, 
						COLORREF crBackground);
	BOOL	SetItemTextColor(int nItem, 
							 int nSubItem, 
							 COLORREF crText, 
							 COLORREF crBackground);
	BOOL	SetProgress(int nItem, 
						int nSubItem, 
						BOOL bShowProgressText = TRUE, 
						LPCTSTR lpszProgressText = NULL);
	// JCW_U 2008.04.12 START
	void	UpdateProgress(int nItem, int nSubItem, int nPercent, COLORREF crText, COLORREF crBar, CString ProgressText=_T(""));
	// JCW_U 2008.04.12 END
	void	UpdateProgressLeftText(int nItem, int nSubItem, int nPercent, COLORREF crBar,
								   CString ProgressText, int nTextLeftmode);
	void	UpdateSubItem(int nItem, int nSubItem);

	// JCW_+ 2008.04.12 START
	virtual void Sort(int nSubItem, BOOL bSort); // bSort = TRUE:��������, FALSE:��������

	void	SetRowHeight(int nRowHeight);
	void	SetTextFont(CFont *pTextFont) { m_pTextFont = pTextFont; RedrawWindow(); }

	
#ifndef NO_XLISTCTRL_TOOL_TIPS
	void DeleteAllToolTips();
	BOOL SetItemToolTipText(int nItem, int nSubItem, LPCTSTR lpszToolTipText);
	CString GetItemToolTipText(int nItem, int nSubItem);

#endif

public:
	CXHeaderCtrl	m_HeaderCtrl;

protected:
	CImageList		m_cImageList;		// Image list for the header control

	// JCW_+ 2008.04.12 START
	BOOL			m_bUnderLine;		// ������� �׸����� ���� ����
	BOOL			m_bResize;			// ���콺 �巡�׷� ��� �÷��� �������� ���� ����
	BOOL			m_bFocusRect;		// Focus Rect ǥ�� ����
	CString			m_strNoItemMsg;		// �������� ������ ǥ�� �� �ؽ�Ʈ ����
	int				m_nStatusColumn;	// ���� �÷��� Rect(��׶��� �÷�����)�� �缳���� �÷� ����

	COLORREF		m_crBgProgress;		// ���α׷����� ����	
	COLORREF		m_crUnderLine;		// ��������� ��

	BOOL			m_bHeaderTracking;	// ���� ����� Ʈ��ŷ������ �Ǵ��ϴ� �÷���
	BOOL			m_bHeaderDraging;	// ���� ����� �巡�������� �Ǵ��ϴ� �÷���

	BOOL			m_bTracking;
	
	CFont		*	m_pTextFont;
	// JCW_+ 2008.04.12 END

protected:
	void DrawCheckbox(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
	// JCW_+ 2008.04.12 START
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	void ShowComboBox(int nItem, int nSubItem);
#endif
	void ShowEditBox(int nItem, int nSubItem);
	void ShowDateBox(int nItem, int nSubItem);
	// JCW_+ 2008.04.12 END

	int DrawImage(int nItem, 
				  int nSubItem, 
				  CDC* pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect rect,
				  XLISTCTRLDATA *pXLCD);

	void DrawProgress(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
	void DrawText(int nItem, 
				  int nSubItem, 
				  CDC *pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect& rect, 
				  XLISTCTRLDATA *pCLD);
	void SubclassHeaderControl();

	BOOL			m_bHeaderIsSubclassed;
	DWORD			m_dwExtendedStyleX;

	COLORREF		m_cr3DFace;
	COLORREF		m_cr3DHighLight;
	COLORREF		m_cr3DShadow;
	COLORREF		m_crBtnFace;
	COLORREF		m_crBtnShadow;
	COLORREF		m_crBtnText;
	COLORREF		m_crGrayText;
	COLORREF		m_crHighLight;
	COLORREF		m_crHighLightText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;

	// JCW_+ 2008.04.12 START
	COLORREF		m_crTracking;
	// JCW_+ 2008.04.12 END

	
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	int				m_nComboItem;
	int				m_nComboSubItem;
#endif

	int				m_nEditItem;
	int				m_nEditSubItem;

	// JCW_+ 2008.04.12 START
	int				m_nDateItem;
	int				m_nDateSubItem;
	
	int				m_nTrackingItem;
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
};


