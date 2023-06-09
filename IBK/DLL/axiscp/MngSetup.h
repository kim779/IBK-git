#if !defined(AFX_MNGSETUP_H__F9434804_9CC8_4862_B832_1303010D6244__INCLUDED_)
#define AFX_MNGSETUP_H__F9434804_9CC8_4862_B832_1303010D6244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MngSetup.h : header file
//
#include "SetupDialog.h"
#include "resource.h"

#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CMngSetup dialog

class AFX_EXT_CLASS CMngSetup : public CSetupDialog
{
// Construction
public:
	CMngSetup(CWnd* pParent = NULL);   // standard constructor
protected:
	int		m_pos;		// 0:�»��, 1:����, 2:���ϴ�, 3:���ϴ�
	int		m_popup;
	int		m_sound;
	CMapStringToString	m_mapMng;

	void		loadInfo();
	void		saveInfo();
	void		setControlValue(CString keys, CString value);

public:
	CFont m_font;
	void draw_groupBox(CPaintDC *pdc, int ids);
	virtual void		ApplySetup();
	virtual void		DefaultSetup();

// Dialog Data
	//{{AFX_DATA(CMngSetup)
	enum { IDD = IDD_MNGSETUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMngSetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMngSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitSignalControlMap();
	CMap<int, int&, UINT, UINT> m_signalMap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MNGSETUP_H__F9434804_9CC8_4862_B832_1303010D6244__INCLUDED_)
