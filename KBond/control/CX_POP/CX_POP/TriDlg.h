#pragma once


// CTriDlg ��ȭ �����Դϴ�.

class CTriDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTriDlg)

public:
	CTriDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTriDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_TRI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
