#if !defined(AFX_TABSTOCKDLG_H__05303F26_9F99_49F4_953A_0DD7520A7380__INCLUDED_)
#define AFX_TABSTOCKDLG_H__05303F26_9F99_49F4_953A_0DD7520A7380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabStockDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabStockDlg dialog
#include "TabDlg.h"
#include "./control/VirtualListCtrl.h"
#include "../../inc/ExOCXDLL.h"
#include "../../inc/ITreeListMng.h"
#include "../../inc/IAUGwanMng.h"
#include "../chart_common/ILButton.h"
#include "./Resource.h"
#include "../../../srcsite/include/CodeCtrl.h"

//@Solomon����-->
typedef CList<CItemLongData*, CItemLongData*> List_CItemLongData;
//@Solomon����<--


class CTabStockDlg : public CTabDlg
{
// Construction
public:
	CTabStockDlg(CWnd* pParent = NULL);   // standard constructor
	enum {ALL, KOSPI, KOSDAQ};	
	int m_nOption;
//	void* m_pDBMgr;

// Dialog Data
	//{{AFX_DATA(CTabStockDlg)
	enum { IDD = IDD_TAB_STOCK };
	CComboBox	m_cmbSort;
	CVirtualListCtrl	m_listStockCode;
	CExTreeCtrl	m_treeSearch;
	CILButton m_ctrlBtnAdd;
	//}}AFX_DATA
//	CExCodeInput		m_ctrlCodeInput;
	CCodeCtrl*		m_pCodeInput;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabStockDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabStockDlg)
//	afx_msg void OnRadioAll();
//	afx_msg void OnRadioKospi();
//	afx_msg void OnRadioKosdaq();
	virtual BOOL OnInitDialog();
//	afx_msg void OnChangeEditStock();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCodeInput();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
public:
	HTREEITEM m_hSelTreeItem;
	ITreeListManager2*	m_pTreeListMng;
	void fnMakeTree(CTreeCtrl* pTree, LPCSTR szFile);

	class CTLMCallback : public ITLMCallback
	{
	public:
		STDMETHOD_(void, ReceiveData)(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData) ;
	} m_xTLMCallback;
	friend class CTLMCallback;	
	long m_dwTreeKey; 
	void	InitTreeSearch();

	CString m_strCodeNName;
	//2007.04.30 by LYH ��������� �ڵ��޺� ���� ��ġ ���� ���� �߰�
	CRect m_rectCmb;

	// ��ȸ ����Ʈ�� ���õ� ������ Clear�Ѵ�.
	void fnRemovelistStockCode();
	
	// ���ɱ׷� �� ������ ������ ���� ó��
//	IAUGwanManager*	m_pIGwanMonitorManager;
//	long m_dwIGwanMonitorManager;
//	static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);
	void AdviseGwansim();
	void UnAdviseGwansim();
	//void ReloadGwansim();

//@Solomon����-->
public:
	virtual void SetCodeFromOutSide(LPCSTR szCode);

	afx_msg long OnWinixNotifyLoad(WPARAM wParam, LPARAM lParam); //@Solomon ���н��� ����
	afx_msg long OnWinixNotifySave(WPARAM wParam, LPARAM lParam); //@Solomon ���н��� ����
	afx_msg long OnWinixNotifyGroup(WPARAM wParam, LPARAM lParam); //@Solomon ���н��� ����
	afx_msg long OnWinixNotifyGroupList(WPARAM wParam, LPARAM lParam); //@Solomon ���н��� ����
	afx_msg long OnWinixNotifySaveCode(WPARAM wParam, LPARAM lParam); //@Solomon ���н��� ����
	
	List_CItemLongData m_ItemLongDataList;
	//void DoDeleteTreeItem(CTreeCtrl* pTreeCtrl, CString& szRemain);
	void DoClearItemLongDataList();
	IWinixGwanManager*	m_pGwanMng;	

	void DoDeleteTreeItem(CTreeCtrl* pTreeCtrl, HTREEITEM hRoot);
	HTREEITEM FindTreeItem(LPCSTR szFindStr);
//@Solomon����<--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSTOCKDLG_H__05303F26_9F99_49F4_953A_0DD7520A7380__INCLUDED_)
