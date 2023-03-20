#pragma once

#include "BtnST.h"
#include "NMButton.h"
// CDlgModaless ��ȭ �����Դϴ�.

class CDlgModaless : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModaless)

public:
	CDlgModaless(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgModaless();

	CFont* m_pfont;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MODALESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CButtonST m_ckProgram;
	CButtonST m_ckSDI;

	CBrush	*m_pBrush;
	CString m_home;
	CStringArray m_arr;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnOk();
	NMButton m_btnok;
	NMButton m_btncancel;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void GetResultArr(CStringArray& arr);
};
