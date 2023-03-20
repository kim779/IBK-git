#pragma once


// CPopInfo ��ȭ �����Դϴ�.
#include "MTable.h"

class CPopInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CPopInfo)

public:
	CPopInfo(CString home, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPopInfo();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_POP };
	int	m_width;
	int	m_height;
	int	m_pos;

	CRect	m_baseRc;
	CString	m_home;
	CString m_data;
	CWnd *   m_pwnd;
	CMTable*	m_table;

	void	Init();
	void	ChangePalette();
	void	ChangeData();
	void	ShowSlide(CRect);
	void	HideSlide();
	void	SetData(CString dat, int kind, int pos = 0);
	void	StopSlide();	
	void	Slide();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
