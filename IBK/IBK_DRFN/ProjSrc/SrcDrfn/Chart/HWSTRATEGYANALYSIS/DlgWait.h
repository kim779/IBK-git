#pragma once
#include "afxwin.h"
#include "../../inc/ExStatic.h"

#define IDD_DLG_WAIT 3066

// CDlgWait ��ȭ �����Դϴ�.

class CDlgWait : public CDialog
{
	DECLARE_DYNAMIC(CDlgWait)

public:
	CDlgWait(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgWait();

	COLORREF m_crBk;
	CBrush	m_brBkColor;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CExStatic m_ExStaticTitle;
};
