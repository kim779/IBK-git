#include "resource.h"

#pragma once
// setup.h : header file
//

//
//	config
//
class Cconfig
{
public:
	BOOL m_bApply{};
	int m_vol{};		// ü�ᷮ ����   (vol * 1,000)
	int m_amt{};		// ü��ݾ� ���� (amt * 10,000,000)
	BOOL m_and{};		// and / or

	BOOL m_price{};		// ���ݼ��� flag
	int m_sprc{};		// ���ݼ��� ���۰�
	int m_eprc{};		// ���ݼ��� ����

	BOOL m_ulimit{};	// ����
	BOOL m_up{};		// ���
	BOOL m_flat{};		// ����
	BOOL m_dlimit{};	// ����
	BOOL m_down{};		// �϶�

	Cconfig()
	{
		m_bApply = FALSE;
	}
};

//
//	field
//
#define F_CODE 0x0001 // �ڵ�
#define F_RATE 0x0002 // �����
#define F_VOL 0x0004  // �ŷ���
#define F_AMT 0x0008  // ����ü��ݾ�

/////////////////////////////////////////////////////////////////////////////
// Csetup dialog

class Csetup : public CDialog
{
	// Construction
public:
	Csetup(CWnd *pParent = nullptr, Cconfig *config = nullptr); // standard constructor

	// Dialog Data
	//{{AFX_DATA(Csetup)
	enum
	{
		IDD = IDD_TICKER_SETUP
	};
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	void GetConfig(Cconfig *config);
	void EnableControl(BOOL bEnable);

protected:
	Cconfig *m_config;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Csetup)
protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
							 //}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(Csetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPrice();
	afx_msg void OnCheckApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
