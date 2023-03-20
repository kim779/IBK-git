#pragma once

#include "listHeader.h"
#include "listctrlSort.h"

class ListData
{
public:
	ListData(CString sData);
	~ListData();
public:
	CString	m_sData;
};

class CExListCtrl : public CListCtrl
{
public:
	CExListCtrl();
	virtual ~CExListCtrl();
	//{{AFX_VIRTUAL(CExListCtrl)
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void PreSubclassWindow();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	InitHeaderData(CString* strHead,
				int* width, 
				int cnt, 
				int align = LVCFMT_CENTER);	// LVCFMT_LEFT,
								// LVCFMT_CENTER
								// LVCFMT_RIGHT
	void	HeaderFix(bool bFix = true, int col = -1);	// column resize prevent
	void	AddItem(CString* strData, int cnt);		

	void	ListSort();
	void	ListSort(int col);
	void	SetScroll(int index = 0);
	void	SetScroll(int index, bool bSel);

	void	SetFullRowSelect();
	void	SetGridLines();
	void	SetEditLabels();
	void	SetSingleSelect();
	void	SetMultiSelect();
	void	DisableEditColumn(int col);
	void	SetDisableSort(bool bSort = false);

	CEdit*	EditSubItem(int item, int col);
	
	void	GetListState(int& col, BOOL& asc);
	void	SetItemStateEx(int row, bool bSel);

	void	SetItemDataEx(int row, CString str);
	void	GetItemDataEx(int row, CString& str);

	bool	GetEditState();
	void	SetEditState(bool bIsEdit);
	void	DeleteEdit();
	void	FreePtr();

	void	SetInterest(bool bInter = true);

private:
	int	hitTestEx(const	CPoint& Point, int* pColumn);
	

public:
	CExListHeader	m_Header;
	
	int		m_FixCol;
	bool		m_bFix;

	int		m_nSortItem;
	BOOL		m_bSortAsc;
	bool		m_bSort;

	int		m_disEditCol;
	
	CArray <std::shared_ptr<ListData>, std::shared_ptr<ListData>>	m_Ptr;
	bool		m_bIsEdit;
	std::unique_ptr<CEdit>	m_pEdit;

	bool		m_bInter;
	int		m_curCol;

	bool		m_bEditEnable;
};
