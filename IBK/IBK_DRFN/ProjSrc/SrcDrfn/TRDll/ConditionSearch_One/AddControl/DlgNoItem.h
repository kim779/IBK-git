#pragma once


// CDlgNoItem ��ȭ �����Դϴ�.
#include "../resource.h"
class CDlgNoItem : public CDialog
{
	DECLARE_DYNAMIC(CDlgNoItem)

public:
	CDlgNoItem(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgNoItem();
	
	CFont	m_font;
	CStatic m_staticText;
	CRect	m_reParent;
	

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
