#pragma once


// CUpdlg ��ȭ �����Դϴ�.

class CUpdlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdlg)

public:
	CUpdlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUpdlg();

	BOOL	addItem(CString items, int item, int subi);
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLGUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_uplist;

	void GetListData(CStringArray& parr);
	afx_msg void OnBnClickedBtnFile();
	afx_msg void OnBnClickedBtnUp();
//	afx_msg void OnItemdblclickListUp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListUp(NMHDR *pNMHDR, LRESULT *pResult);
};
