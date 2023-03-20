#pragma once
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

#include "resource.h"
#include "DlgQr.h"

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();
	virtual ~CMainWnd();

public:
	CWnd*	m_pParent;

protected:
	DlgQr	m_DlgQr;
	struct	_param	m_param;

	CString m_root;
	std::unique_ptr<class Csocket> m_sock;

	CString m_strClientNum;	//고객번호
	CString m_strPinNumber;	//고객인 입력한 pin번호

	CString m_macaddr;  
	CString m_sIP;
	CString	m_sPort;
	int	m_iType;	//0:QRCODE(로그인) 1:QRCODE(주문) 2:OTP(수동) 3:OTP(자동)

// Attributes
public:
	BOOL	SendTR(CString strName, char* pData, int nData, BYTE key);
	void	SearchOTP_MP(CString sUserNum);
	void	CertOTP_MP(CString sOTPNum, CString strCOTPNum);
	void	CertOTP_BP(CString sCustNum, CString spinNum);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	LPCTSTR Variant( int cmd, LPCTSTR data );
	void SetParam(_param *pParam);
	CString GetMacAddr();
	void	dispatch(char* pBytes, int nBytes);

protected:
	// Generated message map functions
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LONG OnSockManage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	CString m_errCode;
	afx_msg void OnERRcodeChanged();
	CString m_otpSuccYN;
	afx_msg void OnOTPsuccYNChanged();
	CString m_otpMaxErrCnt;
	afx_msg void OnOTPMaxErrCntChanged();
	CString m_otpErrCnt;
	afx_msg void OnOTPErrCntChanged();
	short m_otpState;
	afx_msg void OnOTPStateChanged();
	CString m_otpSeqNo;
	afx_msg void OnOTPSeqNoChanged();
	CString m_errMsg;
	afx_msg void OnERRMsgChanged();
	afx_msg void SetIPandPORT(LPCTSTR ip, LPCTSTR port);
	afx_msg void ShowQRcode();
	afx_msg void testmethod(LPCTSTR sdata);
	afx_msg short CertOTP(LPCTSTR sCustNum, LPCTSTR spinNum);
	afx_msg BSTR GetCustNumber();
	afx_msg short SearchOTP(LPCTSTR custnum);
	afx_msg void CertOTPMP(LPCTSTR sCustNum, LPCTSTR sPinNum);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};