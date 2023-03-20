#if !defined(AFX_TARGETITEMDLG_H__B248DFA3_0A24_423B_8BA2_3BE8FBCA7732__INCLUDED_)
#define AFX_TARGETITEMDLG_H__B248DFA3_0A24_423B_8BA2_3BE8FBCA7732__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TargetItemDlg.h : header file
//

#include "../AxMCtrl/AxMCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTargetItemDlg dialog

class CConfigBar;

class CTargetItemDlg : public CDialog
{
// Construction
public:
	CTargetItemDlg(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTargetItemDlg)
	enum { IDD = IDD_TARGETITME };
	CAxMTreeCtrl	m_tree;
	CAxMListCtrl	m_listSelect;
	CAxMListCtrl	m_listSource;
	CAxMSpeedButton	m_btnUp;
	CAxMSpeedButton	m_btnDown;
	CAxMSpeedButton	m_btnRemoveAll;
	CAxMSpeedButton	m_btnRemove;
	CAxMSpeedButton	m_btnAddAll;
	CAxMSpeedButton	m_btnAdd;
	CAxMStaticCtrl	m_label3;
	CAxMStaticCtrl	m_label2;
	CAxMStaticCtrl	m_label1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTargetItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnAxItemClick(WPARAM, LPARAM);
	afx_msg LRESULT OnAxTreeItemClick(WPARAM, LPARAM);
	afx_msg LRESULT OnAxAddButtonClick(WPARAM, LPARAM);
	afx_msg LRESULT OnAxRemoveButtonClick(WPARAM, LPARAM);
	afx_msg LRESULT OnAxAddAllButtonClick(WPARAM, LPARAM);
	afx_msg LRESULT OnAxRemoveAllButtonClick(WPARAM, LPARAM);
	afx_msg LRESULT OnAxUpButtonClick(WPARAM, LPARAM);
	afx_msg LRESULT OnAxDownButtonClick(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private:
	CConfigBar* m_bar;
	CAxTargetItem* m_items;
	BOOL m_modified;
	KIND m_lastKind;
	CString m_selectKind;
	void InitControls();
	void InitKindTree();
	void InitListControls();
	void InitListControl(CListCtrl& list);

	void ResizeListColumn(CAxMListCtrl* list = NULL);
	
	void SetMessageHandler();

	void AddItem();
	void AddAllItem();
	void RemoveItem();
	void RemoveAllItem();
	void MoveUpItem();
	void MoveDownItem();

	int FindText(CAxMListCtrl& list, LPCTSTR text);

	void ExchangeItem(CAxMListCtrl& src, CAxMListCtrl& dest, BOOL selectOnly);
	void MoveListItem(CAxMListCtrl& list, int distance);
	void ProcessSelectedItem(CString items);
	void Modified();
public:
	void SetKind(LPCTSTR);;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARGETITEMDLG_H__B248DFA3_0A24_423B_8BA2_3BE8FBCA7732__INCLUDED_)
