#if !defined(AFX_DLLDLG_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_)
#define AFX_DLLDLG_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HT_NOWHERE			0
#define HT_CLOSE			1
#define CX_BTNCLOSE			16
#define CY_BTNCLOSE			16

// CDllDlg ��ȭ �����Դϴ�.
#include "DlgSTExOption.h"
class CDllDlg : public CDialog
{
	DECLARE_DYNAMIC(CDllDlg)

public:
	CDllDlg(LPCSTR szDllName, LPCSTR szTitle, void* pSTInterface, HINSTANCE hOldRsc, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDllDlg();

	CString m_szDllName, m_szTitle;
	void*   m_pSTInterface;
	HINSTANCE m_hOldRsc;
	HINSTANCE m_hTradeInst;
	CListCtrl*	m_pCtlList;
	int m_nOptimizerDiv;//1001:��ǥ����ȭ,1002:��������ȭ,1003:�ð�����ȭ
	BOOL	OpenAUP();
	CWnd*	LoadTradeAup(CString strFile, CWnd *pParent);

	CWnd*	m_pWndTrade;

// ��ȭ ���� �������Դϴ�.
	//{{AFX_DATA(CDllDlg)
	enum { IDD = IDD_DLLDLG };
	//}}AFX_DATA

	CString			m_strImageDir;
	CBitmap			m_bmpCaption;

	CImageList		m_ImgList_BtnClose;
	BOOL			m_bLButtonDown;
	int				m_nHitTest;
	CRect			m_rcBtnClose;

	void	CalcCaptionBarRect(CRect& rcCaptionBar);
	void	DrawBkgrnd(CDC& dc, CRect& rcCaptionBar);
	void	DrawTitle(CDC& dc, CRect& rcCaptionBar);
	void	DrawBtnClose(CDC& dc, CRect& rcCaptionBar);
	int		HitTestClient(CPoint point);
	int		HitTestWindow(CPoint point);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CTabDlg)
	afx_msg void OnOk();
	afx_msg void OnDestroy();
	afx_msg void OnNcPaint();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
	afx_msg	LRESULT OnSetDllSize(WPARAM wParam, LPARAM lParam); //DLL���������� ȭ������� �����û�� �� �ݵ�.
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLDLG_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_)