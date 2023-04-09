#pragma once


// CDlg_Cloude 대화 상자
#include "cloude/InterfaceDLL.h"   //클라우드
class CDlg_Cloude : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Cloude)

public:
	CDlg_Cloude(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_Cloude();
	void SetAppContext(APP_CONTEXT* appContext, SD_API_CONTEXT_NEW* Context);

	APP_CONTEXT m_appContext;
	SD_API_CONTEXT_NEW m_Context;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIG_CLOUDE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnCon();
	afx_msg void OnBnClickedBtnKeypad();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnCertIssue();
	afx_msg void OnBnClickedBtnCertnew();
	afx_msg void OnBnClickedBtnPasscha();
	afx_msg void OnBnClickedBtnCertdel();
	afx_msg void OnBnClickedBtnCertup();
	afx_msg void OnBnClickedBtnCertdown();
	afx_msg void OnBnClickedBtnConntchk();
	afx_msg void OnBnClickedBtnAutochk();
	afx_msg void OnBnClickedBtnWithdraw();
	afx_msg void OnBnClickedBtnConcloude();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

	int selectoption;
	int keypaduse;
	afx_msg void OnBnClickedBtnCertOrder();
	afx_msg void OnBnClickedBtnCertTransfer();
};
