#if !defined(AFX_INQUIRYDATAWINDOWDLG_H__F277BA79_4A57_4A50_8AEE_BCB343492CB6__INCLUDED_)
#define AFX_INQUIRYDATAWINDOWDLG_H__F277BA79_4A57_4A50_8AEE_BCB343492CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InquiryDataWindowDlg.h : header file
// �⺻���� ��ġ��ȸâ.

#include "NumericalInquiryDlg.h"
#include "Resource.h"
#include "InquiryDataListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CInquiryDataWindowDlg dialog

class CInquiryDataWindowDlg : public CNumericalInquiryDlg
{
	// (2006/2/15 - Seung-Won, Bae) for checking Runtime Class ~ Not used yet in 2006/2/15
	DECLARE_DYNAMIC( CInquiryDataWindowDlg)

// Construction
public:
	CInquiryDataWindowDlg( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL);   // standard constructor
	virtual ~CInquiryDataWindowDlg();

// Dialog Data
	//{{AFX_DATA(CInquiryDataWindowDlg)
	enum { IDD = IDD_INQUIRYDATAWINDOW };
	CInquiryDataListCtrl	m_listctrlInquiryData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInquiryDataWindowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInquiryDataWindowDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnGetdispinfoListInquirydata(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void SetInquiryData(CDC* pDC, CInquiryData *pInquiryData,CInquiryData *pInquiryPrevData,int nMaxDecimal = 0);
	virtual void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE, CDC* pDC = NULL);
	virtual void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	virtual void SetDlgBrush(COLORREF clrDlgBrush);
	virtual void OnMouseMove(CPoint point);	

private:
	int SetColumnWidth(int nColIndex, CDC* pDC);	// Į�� �ʺ� ����Ͽ� ����.
	CString ReplaceTitle(const CString& strTitle);	// Ÿ��Ʋ���� ��ģ��.

private:
	CInquiryItem *	m_listData;
	bool			m_bCalculateSize;
// (2006/7/2 - Seung-Won, Bae) Using Virtual Draw. 0:Title, 1:Data
protected:
	int				m_nListDataSize;
	CStringArray	m_saListData[ 2];

// (2006/6/27 - Seung-Won, Bae) Manage Interfaces
protected:
	IChartOCX *		m_pIChartOCX;
	IChartManager *	m_pIChartManager;

// (2006/6/24 - Seung-Won, Bae) Manage the Numerical Inquiry Data Queryer with HorizontalLine Option.
protected:
	CNumericalInquiryDataQueryer	m_niDataQueryer;
	HWND	m_hOcxWnd;
public:
	void	EnableHorizontalLineInNIDLG( BOOL p_bEnable)	{	m_niDataQueryer.EnableHorizontalLineInNIDLG( p_bEnable);	}

protected:
	bool m_bUseNumericalInquiryDlg;		// ��ġ��ȸâ ��뿩��.
	bool m_bInitDialogMove;				// ��ġ��ȸâ �̵� �ʱ�ȭ����.('����'�ϰ�� �� �ѹ��� ����ġ ã���ֱ� ����)
	bool m_bNumericalInquiryDlgPosLeft;	// ��ġ��ȸâ�� ��ġ(true: ����, false: ������)
	bool m_bCalcSizeInquiryDlg;			// ��ġ��ȸâ ������ ��꿩��.
	short m_nMoveNumericalInquiryDlg;	// ��ġ��ȸâ �̵�/����/���콺����ٴϱ� ����.
	COLORREF m_clrInquiryDlg;			// ��ġ��ȸâ ����.
public:
	void	ShowNumericalInquiryDlg( const BOOL p_bShow);
	void	ShowNumericalInquiryDlg( CDC *p_pDC, const CPoint &p_ptPoint);
	void	OnMouseMove(CDC* pDC, CPoint point);
	bool	DestroyNumericalInquiryDlg( void);
	bool	CreateNumericalInquiryDlg( void);
	void	SetNumericalInquiryDlgBrush(COLORREF clrDlg);			// ��ġâ ����.
	bool	IsUsingNumericalInquiryDlg();							// ��뿩��.
	void	MoveNumericalInquiryDlg(CDC* pDC, CRect rectRegion, CPoint point);			// �̵�.
	void	ResizeNumericalInquiryDlg(CDC* pDC, CRect* rectDlg);	// ����Ÿ���̿� �°� ��ġ��ȸâ ������ �����ϱ�.
	void	OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption);	// (2006/3/4 - Seung-Won, Bae) Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG
	void	Refresh( void);

// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
protected:
	HWND	m_hExportWnd;
public:
	void	SetExportMode( HWND p_hExportWnd)	{	m_hExportWnd = p_hExportWnd;	}
	BOOL	IsExportMode( void)					{	return ( BOOL)m_hExportWnd;	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INQUIRYDATAWINDOWDLG_H__F277BA79_4A57_4A50_8AEE_BCB343492CB6__INCLUDED_)
