#pragma once


// testdlg ��ȭ �����Դϴ�.

class testdlg : public CDialogEx
{
	DECLARE_DYNAMIC(testdlg)

public:
	testdlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~testdlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
