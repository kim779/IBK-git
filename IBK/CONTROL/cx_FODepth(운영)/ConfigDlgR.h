#if !defined(AFX_CONFIGDLGR_H__0E813731_EF25_42D9_BABC_EE2670A483C0__INCLUDED_)
#define AFX_CONFIGDLGR_H__0E813731_EF25_42D9_BABC_EE2670A483C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlgR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDlgR dialog

class CConfigDlgR : public CDialog
{
// Construction
public:
	CConfigDlgR(struct _config *pConfig, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlgR)
	enum { IDD = IDD_CONFIGR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlgR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlgR)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	struct _config	*m_pConfig;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLGR_H__0E813731_EF25_42D9_BABC_EE2670A483C0__INCLUDED_)
