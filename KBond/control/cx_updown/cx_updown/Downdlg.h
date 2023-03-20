#pragma once


// CDowndlg 대화 상자입니다.

class CDowndlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDowndlg)

public:
	CDowndlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDowndlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLGDOWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_Dnlist;


	BOOL addItem(CString items, int item, int subi);
};
