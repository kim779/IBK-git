#pragma once

#include "control/HelpHtmlCtrl.h"
#include "CandlePattern.h"	
#include "DlgHtml.h"

// CDlgHtml ��ȭ �����Դϴ�.

class CDlgHtml : public CDialog
{
	DECLARE_DYNAMIC(CDlgHtml)

public:
	CDlgHtml(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgHtml();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_HTML };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CHelpHtmlCtrl* m_pHtmlView;
	void	ShowHtml();	


	CString m_strURL;
	void SetURL(LPCTSTR lpszURL){m_strURL = lpszURL;};
};
