#pragma once
// DlgQr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgQr dialog

#include "ShapeButton.h"
#include "AxBrowser.h"
#include "SimpleAuth.h"

class DlgQr : public CDialog
{
// Construction
public:
	DlgQr(CWnd* pParent = nullptr); 
	DlgQr(int type, CWnd* pParent = nullptr);
	virtual ~DlgQr();

public:
	CWnd*	m_pParent;
	CString m_strpath;
	int	m_iType;		//0:QRCODE(로그인) 1:QRCODE(주문) 2:OTP(수동) 3:OTP(자동)
	CString	m_sIP;
	CString	m_sPort;
	char    m_pdata[2048]{};

protected:
	std::unique_ptr<class Csocket> m_sock;
	CAxBrowser* m_pBrowser;

	CString m_strTrdNum;		//연계거래번호
	CString m_strEasyTrdNum;	//연계간편인증번호
	CString m_strAutTrdNum;		//인증거래번호

	CString m_macaddr;

	CRect	m_rcQR;
	CRect	m_rcEnum;
	
	int	m_iMheight;
	int	m_iMwidth;
	CFont	bFont;
	CFont	mFont;
	CFont	sFont;
	CFont	lbFont;

	CBitmap	m_pBmpMain;
	std::unique_ptr<CShapeButton>	m_btnClose;
	std::unique_ptr<CShapeButton>	m_btnInit;
	std::unique_ptr<CShapeButton>	m_btnOther;

	int	m_iRemainTime;
	CString m_strRemainTime;

	bool	m_bChecking;

	PUCHAR	m_pPubkey;
//	PUCHAR* m_pPubkey;

public:
	void	LoadBitmapFromFile(const char* bmpName);
	void	initFont();
	void	initbutton();
//	int	getPubkey(PUCHAR* pdata);
	int	getPubkey(PUCHAR pdata);
	
	CString	GetMacAddr();
	
	void	SendRegTranByRelay();	//서명원문등록
	void	SendtagINQUIRYREG();	//서명등록여부조회 
	void	SendSrhAuthTrdnum();	//인증거래번호조회(서명등록조회확인후)
	void	SendLoginconfirm();	//인증서로그인서명메시지검증
	
	void	ShowQRcode(CString strEnum);
	void	dispatch(char* pBytes, int nBytes);

// Dialog Data
	//{{AFX_DATA(DlgQr)
	enum { IDD = IDD_DLG_QRCODE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgQr)
public:
	virtual void OnFinalRelease();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DlgQr)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnExit();
	afx_msg	void OnQRInit();
	afx_msg void OnQROther();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnSockManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(DlgQr)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};