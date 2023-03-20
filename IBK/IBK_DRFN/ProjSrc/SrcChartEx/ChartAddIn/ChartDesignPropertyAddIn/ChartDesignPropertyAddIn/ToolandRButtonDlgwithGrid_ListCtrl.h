#if !defined(AFX_TOOLANDRBUTTONDLGWITHGRID_LISTCTRL_H__B68FDC9D_E1B8_4D87_80C6_2489577C6998__INCLUDED_)
#define AFX_TOOLANDRBUTTONDLGWITHGRID_LISTCTRL_H__B68FDC9D_E1B8_4D87_80C6_2489577C6998__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolandRButtonDlgwithGrid_ListCtrl.h : header file
//
#include "ToolandRButtonGrid_ListCtrl.h"
#include "NumericalInquiryOptionDlg.h"	// ADD: ����(04/03/17) ��ġ��ȸ����

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonDlgwithGrid_ListCtrl dialog

class CToolandRButtonDlgwithGrid_ListCtrl : public CDialog
{
// Construction
public:
	CToolandRButtonDlgwithGrid_ListCtrl(const int nID, CWnd* pParent = NULL);   // standard constructor

	//������ �����Ǿ� �ִ� ���� �޾Ƽ� list�� �����Ų��.
	void SetListItemInfoFromParent(const CString &strItemInfo);
	//��� Item�� ������ ������ �ְ�, ListCtrl���� ���� ���� �� �ֵ��� ��Ʈ���ϴ� class�̴�.
	void SetListItemInfoMgrObject(CToolandRButtonItemMgr *pItemInfoMgr, LPCTSTR pcszTabName, CNumericalInquiryOptionDlg* pNumericalInquiryOptionDlg); // MODIFY: ����(04/03/17) ��ġ��ȸ���� �ɼ�
	//ListCtrl�� �����Ǿ� �ִ� Item�� ������ ��´�.
	CString GetListItemInfo();
	//Dragand Drop �ϱ� ���ؼ� ȣ���Ѵ�.
	void InitDropTarget();

// Dialog Data
	//{{AFX_DATA(CToolandRButtonDlgwithGrid_ListCtrl)
	enum { IDD = IDD_DLG_TOOL_RBUTTON };
	CStatic	m_Frame;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolandRButtonDlgwithGrid_ListCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolandRButtonDlgwithGrid_ListCtrl)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UINT m_nID;
	//Grid List Ctrl.
	CToolandRButtonGrid_ListCtrl m_GridList;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLANDRBUTTONDLGWITHGRID_LISTCTRL_H__B68FDC9D_E1B8_4D87_80C6_2489577C6998__INCLUDED_)
