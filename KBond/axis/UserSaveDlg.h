#pragma once


// CUserSaveDlg ��ȭ �����Դϴ�.

class CUserSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserSaveDlg)

public:
	CUserSaveDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserSaveDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_USERSAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
