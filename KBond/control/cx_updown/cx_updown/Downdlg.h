#pragma once


// CDowndlg ��ȭ �����Դϴ�.

class CDowndlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDowndlg)

public:
	CDowndlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDowndlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLGDOWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_Dnlist;


	BOOL addItem(CString items, int item, int subi);
};
