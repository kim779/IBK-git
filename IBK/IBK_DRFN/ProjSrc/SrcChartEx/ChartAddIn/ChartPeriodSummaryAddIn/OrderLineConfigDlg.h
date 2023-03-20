#if !defined(AFX_ORDERLINECONFIGDLG_H__D4ABFF3C_2BB3_4E65_9411_CCB83CFB9806__INCLUDED_)
#define AFX_ORDERLINECONFIGDLG_H__D4ABFF3C_2BB3_4E65_9411_CCB83CFB9806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrderLineConfigDlg.h : header file
//
#include "control/BitmapItemComboBox.h"
#include "Control/ColorButton.h"

static TCHAR* szType[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5"),
	_T("6"),	_T("7"),	_T("8"),	_T("9"),	_T("10"),	_T("11"),
	_T("12"),	_T("13"),	_T("14"),	_T("15"),	_T("16"),	_T("17"),
	_T("18"),	_T("19"),	_T("20"),	_T("21"),	_T("22"),	_T("23"),
	_T("24")
};

/////////////////////////////////////////////////////////////////////////////
// COrderLineConfigDlg dialog

class CLineObj;
class COrderLineConfigDlg : public CDialog
{
// Construction
public:
	COrderLineConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrderLineConfigDlg)
	enum { IDD = IDD_DLG_OL_CONFIG };
	CEdit	m_eCode;
	CButton	m_chkSell2;
	CButton	m_chkSell;
	CButton	m_chkBuy2;
	CButton	m_chkBuy;
	CColorButton	m_ctlColorPick;
	CBitmapItemComboBox	m_ctlLineThickness;
	CBitmapItemComboBox	m_ctlLineStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderLineConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(COrderLineConfigDlg)
	afx_msg void OnBtnSave();
	afx_msg void OnBtnColorPick();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSemiAutoBuy();
	afx_msg void OnSemiAutoBuy2();
	afx_msg void OnSemiautoSell();
	afx_msg void OnSemiautoSell2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	HWND			m_hOcxWnd;
public:
	CStringList *	m_pAllOrderLines;
	CLineObj *		m_OLObj;
	CString			m_strUserPath;
protected:
	void			AddLineType( CComboBox* pCombo, int Type);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERLINECONFIGDLG_H__D4ABFF3C_2BB3_4E65_9411_CCB83CFB9806__INCLUDED_)
