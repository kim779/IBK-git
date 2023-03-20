#if !defined(AFX_DLGENVSETUP3_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_)
#define AFX_DLGENVSETUP3_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnvSetup3.h : header file
//

#include "resource.h"

#include "PatternDBItems.h"			// Pattern DB Items

// 2008.02.20 by LYH >> 7.0 -> 6.0
//#include "../chart_common/ColorButton.h"
//#include "../chart_common/BitmapItemComboBox.h"
#include "control/ColorButton.h"
#include "control/BitmapItemComboBox.h"
// 2008.02.20 by LYH <<

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup3 dialog

class CDlgEnvSetup3 : public CDialog
{
// Construction
public:
	CDlgEnvSetup3(CWnd* pParent = NULL);   // standard constructor


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
	//{{AFX_DATA(CDlgEnvSetup3)
	enum { IDD = IDD_DLG_TABENVSETUP3 };
	CButton m_chkAutoPattern;
	CBitmapItemComboBox	m_cbWaveThick;
	CColorButton	m_btnWaveClr;
	CBitmapItemComboBox	m_cbLowThick;
	CColorButton	m_btnLowClr;
	CBitmapItemComboBox	m_cbHighThick;
	CColorButton	m_btnHighClr;
	CButton m_chkHighLowPtLine;
	CButton	m_chkMMa;
	CButton	m_chkHighPtHLine;
	CButton m_chkLowPtHLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnvSetup3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEnvSetup3)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBtnWaveClr();
	afx_msg void OnBtnHighClr();
	afx_msg void OnBtnLowClr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENVSETUP3_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_)
