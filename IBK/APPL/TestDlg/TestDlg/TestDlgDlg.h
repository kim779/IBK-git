
// TestDlgDlg.h: 헤더 파일
//

#pragma once


// CTestDlgDlg 대화 상자
class CTestDlgDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestDlgDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	std::unique_ptr<char[]> m_hcB{};

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrnBit();
	afx_msg void OnBnClickedBrnBit2();
	afx_msg void OnBnClickedBtnmfc140();
	afx_msg void OnEnChangeEditData();
	afx_msg void OnBnClickedBtnFormat();
	afx_msg void OnBnClickedBtnMap();
	afx_msg void OnBnClickedBtnRace();
	afx_msg void OnBnClickedBtnConsumer();
	afx_msg void OnBnClickedBtnConsumer2();
	afx_msg void OnBnClickedBtnFuture();
	afx_msg void OnBnClickedBtnFuture2();
	afx_msg void OnBnClickedBtnAsync();
	afx_msg void OnBnClickedBtnRlef();
	afx_msg void OnBnClickedBtnPair();
	afx_msg void OnBnClickedBtnTuple();
	afx_msg void OnBnClickedBtnLamda();
	afx_msg void OnBnClickedBtnLamdaFind();
	afx_msg void OnBnClickedBtnOs();
	afx_msg void OnBnClickedBtnOsversion();
	afx_msg void OnBnClickedBtnHashkey();
};
