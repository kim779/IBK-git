#if !defined(AFX_TOOLANDRBUTTONTABCTRL_H__868CD342_C32E_491C_8841_E7119CE66D1D__INCLUDED_)
#define AFX_TOOLANDRBUTTONTABCTRL_H__868CD342_C32E_491C_8841_E7119CE66D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolandRButtonTabCtrl.h : header file
//

#include "ToolandRButtonItemMgr.h"
// ADD: ����(04/03/17) ��ġ��ȸ����
#include "NumericalInquiryOptionDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonTabCtrl window

class CToolandRButtonDlgwithGrid_ListCtrl;
class CToolandRButtonTabCtrl : public CTabCtrl
{
// Construction
public:
	CToolandRButtonTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolandRButtonTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolandRButtonTabCtrl();

	//�θ�κ��� ����Ǿ� �ִ� Data�� �޴´�.
	void SetSavedDataFromParent(const CString& strSavedData);
	// ADD: ����(04/03/17) ��ġ��ȸ����(�������̵�)
	void SetSavedDataFromParent(const int nOption);
	//��������� ����Ǿ� �ִ� data�� �ش� Dlg�� �����Ų��.
	void SetSavedDataToDlgwithListCtrl();
	//�����Ǿ� �ִ� ����Ʈ��Ʈ���� ��� ������ ��´�.
	CString GetListItemInfo();
	// ADD: ����(04/03/17) ��ġ��ȸ���� �ɼǰ��� ��´�.
	int GetNumericalInquiryOption();
	//DragandDrop�� ���ؼ� �ʱ�ȭ�Ѵ�.
	void InitDropTarget();
	//Tab���� ��ġ�� ����Ʈ�� create�Ѵ�.
	void InitTab();
	// ADD: ����(04/03/17) ��ġ��ȸ����. ���������� ������ ���� ���ڿ��� �����´�.
	CString GetMiniToolBarItem(void);

	void SetTabFocus();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolandRButtonTabCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//�����Ǿ� �ִ� ���尪�� �θ�κ��� �޾Ƽ� �� class�� ����Ǿ� �ִ�.
	CString m_strSavedDataFromParent;
	//��� Item�������� ������ �ְ�, �� ������ ���� �� �ִ� Ŭ�����̴�.
	CToolandRButtonItemMgr m_ItemInfoManager;

	//�������� Tab �߿��� Active �Ǿ� �ִ� tab�� ��ȣ�̴�.
	int m_tabCurrent;
	//��� tab�� ������ �ִ��� �˷��ش�.
	int m_nNumberOfPages;
	//tab�� ��ġ�� Dlg�� �����ؼ� �����Ѵ�.
	CToolandRButtonDlgwithGrid_ListCtrl *m_tabPages[3];

	// ADD: ����(04/03/17)��ġ��ȸ���ÿɼ��߰�
	CNumericalInquiryOptionDlg m_DlgNumericalInquiryOption;

	//������ Dlg�� Tab���� ��ġ ��Ű�� ���ؼ� ��ġ�� ����Ѵ�.
	void SetRectangle();

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLANDRBUTTONTABCTRL_H__868CD342_C32E_491C_8841_E7119CE66D1D__INCLUDED_)
