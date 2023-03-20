#pragma once


// CUpdlg 대화 상자입니다.

class CUpdlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdlg)

public:
	CUpdlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUpdlg();

	BOOL	addItem(CString items, int item, int subi);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLGUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
