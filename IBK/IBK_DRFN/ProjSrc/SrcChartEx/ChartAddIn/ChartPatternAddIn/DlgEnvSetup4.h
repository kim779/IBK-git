#if !defined(AFX_DLGENVSETUP4_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_)
#define AFX_DLGENVSETUP4_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnvSetup4.h : header file
//

#include "resource.h"

#include "PatternDBItems.h"			// Pattern DB Items


/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup4 dialog

class CDlgEnvSetup4 : public CDialog
{
// Construction
public:
	CDlgEnvSetup4(CWnd* pParent = NULL);   // standard constructor


protected:
	CBrush			m_brBack;				// Dialog BG Color
	CBrush			m_brWhite;				// Brush Whiet
	
	CPatternEnvData	*m_pPatternEnvData;		// ���� ȯ�漳�� ������


public:
	void			SetPatternEnvData(CPatternEnvData *pPatternEnvData);
	void			SetPatternEnvDataToControl();	// ���� ȯ�漳���� ��Ʈ�ѿ� ����
	virtual void OnOK();


public:

// Dialog Data
	//{{AFX_DATA(CDlgEnvSetup4)
	enum { IDD = IDD_DLG_TABENVSETUP4 };
	CTreeCtrl	m_treeItem;
	CButton	m_chkDisplayName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnvSetup4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEnvSetup4)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnButtonAllcheck();
	afx_msg void OnButtonAlluncheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENVSETUP4_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_)
