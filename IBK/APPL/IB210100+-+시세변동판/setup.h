#pragma once
// setup.h : header file
//

//
//	config
//
#define CNT_TREE 15
#define CNT_CHECK 10
class	Cconfig
{
public:
	int	m_vol;			// ü�ᷮ ����   (vol * 1,000)
	int	m_amt;			// ü��ݾ� ���� (amt * 10,000,000)
	bool	m_and;			// and / or

	bool	m_price;		// ���ݼ��� flag
	int	m_sprc;			// ���ݼ��� ���۰�
	int	m_eprc;			// ���ݼ��� ����

	bool	m_ulimit;		// ����
	bool	m_up;			// ���
	bool	m_flat;			// ����
	bool	m_dlimit;		// ����
	bool	m_down;			// �϶�

	bool	m_color;		// ����ǥ��
	WORD	m_field;		// �ʵ�ǥ��

	CString	m_tree[CNT_TREE];			/* 0: ��ü                        */
										/* 1: �ŷ���,      2: �ڽ���      */
										/* 3: KP200,       4: KRX100      */
										/* 5: ��������,    6: ETF         */
										/* 7: ELW,         8: ELW�����ڻ� */
};

//
//	field
//
#define	F_CODE		0x0001		// �ڵ�
#define	F_RATE		0x0002		// �����
#define	F_VOL		0x0004		// �ŷ���
#define	F_AMT		0x0008		// ����ü��ݾ�


/////////////////////////////////////////////////////////////////////////////
// Csetup dialog

class Csetup : public CDialog
{
// Construction
public:
	Csetup(CWnd* pParent = NULL, Cconfig* config = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Csetup)
	enum { IDD = IDD_SETUP };
	CButton	m_chkKrx100;
	CButton	m_chkKospi200;
	CButton	m_chkKospi;
	CButton	m_chkKosdaq;
	CButton	m_chkFreeboard;
	CButton	m_chkEtf;
	CButton	m_chkElwbase;
	CButton	m_chkElw;
	CButton	m_chkAll;
	//}}AFX_DATA

public:
	void	GetConfig(Cconfig* config);
	void	SetProceedCheck(int index);
	void	Check_AllCheck();
protected:
	Cconfig* m_config;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Csetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Csetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPrice();
	afx_msg void OnAll();
	afx_msg void OnElw() { UpdateData(); };
	afx_msg void OnElwbase() { UpdateData(); };
	afx_msg void OnEtf() { UpdateData(); };
	afx_msg void OnFreeboard() { UpdateData(); };
	afx_msg void OnKosdaq() { UpdateData(); };
	afx_msg void OnKospi() { UpdateData(); };
	afx_msg void OnKospi200() { UpdateData(); };
	afx_msg void OnKrx100() { UpdateData(); };
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

