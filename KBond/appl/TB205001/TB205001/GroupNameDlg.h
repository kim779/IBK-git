#pragma once


// CGroupNameDlg ��ȭ �����Դϴ�.

class CGroupNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupNameDlg)

public:
	CGroupNameDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGroupNameDlg();

	void SetGroupName(LPCTSTR lpszGroupName);
	LPCTSTR GetGroupName();
	CString	m_strGroupName;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GROUPNAMEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
