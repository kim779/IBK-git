#pragma once


// CUpdlg ��ȭ �����Դϴ�.

class CUpdlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdlg)

public:
	CUpdlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUpdlg();

	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLGUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_uplist;
	afx_msg void OnBnClickedBtnFile();
};
