#pragma once


// CDowndlg ��ȭ �����Դϴ�.

class CDowndlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDowndlg)

public:
	CDowndlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDowndlg();

	class CControlwnd*  m_pParent;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLGDOWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Dnlist;
	afx_msg void OnBnClickedBtnDown();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_btndown;
};
