#pragma once


// CDlgNoItem ��ȭ �����Դϴ�.
#define COND_NORESUT 0
#define COND_INVALID 1

#include "resource.h"
class CDlgNoItem : public CDialog
{
	DECLARE_DYNAMIC(CDlgNoItem)

public:
	CDlgNoItem(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgNoItem();
	
	CFont	m_font;
	CStatic m_staticText;
	CRect	m_reParent;

	int		m_nType;  //COND_NORESUT:��ȸ��� 0��   COND_INVALID:���ǿ���
	

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NO_ITEM };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
