#if !defined(AFX_DLLDLG_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_)
#define AFX_DLLDLG_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CTabDlg)
	afx_msg void OnOk();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg	LRESULT OnSetDllSize(WPARAM wParam, LPARAM lParam); //DLL���������� ȭ������� �����û�� �� �ݵ�.
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLDLG_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_)