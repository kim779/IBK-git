#if !defined(AFX_TOOLANDRBUTTONGRID_LISTCTRL_H__6E1C5731_9579_4E68_A798_F9433CCCC117__INCLUDED_)
#define AFX_TOOLANDRBUTTONGRID_LISTCTRL_H__6E1C5731_9579_4E68_A798_F9433CCCC117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolandRButtonGrid_ListCtrl.h : header file
//
#include "SuperGridCtrl.h"
#include "ToolandRButtonBase_ListCtrl.h"
#include "NumericalInquiryOptionDlg.h"	// ADD: ����(04/03/17) ��ġ��ȸ����
/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonGrid_ListCtrl window

class CToolandRButtonItemMgr;
class CToolandRButtonGrid_ListCtrl : public CSuperGridCtrl
{
// Construction
public:
	CToolandRButtonGrid_ListCtrl();

// Attributes
public:
	//HOWTO:
	void InitializeGrid(void);
	CImageList *CreateDragImageEx(int nItem);
	BOOL m_bDrag;
	// Overrides
	void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);	
	void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	CItemInfo* CopyData(CItemInfo* lpSrc);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB(void);
	BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	BOOL OnCollapsing(CTreeItem *pItem);
	BOOL OnItemCollapsed(CTreeItem *pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);
	BOOL OnVkReturn(void);
	BOOL OnItemLButtonDown(LVHITTESTINFO& ht);

	//list�� �����Ǿ� �ִ� ���� ��´�.
	CString GetListItemInfo();
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolandRButtonGrid_ListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolandRButtonGrid_ListCtrl();

	//������ ����Ǿ� �ִ� Item�� ������ �ٽ� listctrl�� �����ĭ��.
	void InsertSavedItemInfo(const CString &strItemName, const CString &strLimit1, const CString &strLimit2);
	//��� Item�� ������ ������ ��Ʈ���ϴ� ��ü�� �����͸� Setting�Ѵ�.
	void SetListItemInfoMgrObject(CToolandRButtonItemMgr *pItemInfoMgr, LPCTSTR pcszTabName, CNumericalInquiryOptionDlg* pNumericalInquiryOptionDlg);	// MODIFY: ����(04/03/17) ��ġ��ȸ���� �ɼ�.
	//draganddrop�� �����ϱ� ���ؼ� ȣ���Ѵ�.
	void InitDropTarget();

protected:
	CImageList m_image;//Must provide an imagelist
	//positions and creates/initalize a combobox control
	CComboBox* ShowList(int nItem, int nCol, CStringList *lstItems);
	//helper function called from ShowList...calcs the lists max horz extent
	int CalcHorzExtent(CWnd* pWnd, CStringList *pList);

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolandRButtonGrid_ListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	LONG OnDragChartData(UINT wParam, LPARAM lParam);
	LONG OnDropChartData(UINT wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void SetComboListWhen_Extension_Recuction_Item(const int nItem, const int nColumn, CStringList *lstItems);
	//ListCtrl�� �� ���κ�.
	CTreeItem* m_pRootItem;
	//��� Item�� ������ ������ ��Ʈ���ϴ� ��ü�� �������̴�.
	CToolandRButtonItemMgr* m_pItemInfoManager;

	//listctrl�� �� ���Ѹ��� Setting�Ѵ�.
	void InsertRoot();
	//Į������� Insert�Ѵ�.
	void InsertColumnHeader();
	//CItemInfo�� ������ �����Ѵ�.
	void SetRootItem(const CString& szItemName, CItemInfo *lpItemInfo);

	//DragandDrop�� ���� ��������̴�.
	COleListCtrlDropTarget m_dropTarget;

	// ADD: ����(04/03/17) ��ġ��ȸ����. �������� ���������� ��ġ��ȸ���������� �ɼǰ� ������ ���ؼ�
	CString m_strTabName;
	CNumericalInquiryOptionDlg* m_pNumericalInquiryOptionDlg;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLANDRBUTTONGRID_LISTCTRL_H__6E1C5731_9579_4E68_A798_F9433CCCC117__INCLUDED_)
