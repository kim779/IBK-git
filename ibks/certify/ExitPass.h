#if !defined(AFX_EXITPASS_H__AB597C2E_37CA_4044_8235_0559B6CA4E97__INCLUDED_)
#define AFX_EXITPASS_H__AB597C2E_37CA_4044_8235_0559B6CA4E97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExitPass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExitPass dialog

class CExitPass : public CDialog
{
// Construction
public:
	CExitPass(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExitPass)
	enum { IDD = IDD_PASSOVER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExitPass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExitPass)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXITPASS_H__AB597C2E_37CA_4044_8235_0559B6CA4E97__INCLUDED_)
