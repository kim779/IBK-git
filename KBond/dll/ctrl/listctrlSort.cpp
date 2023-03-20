#include "stdafx.h"
#include "listctrlSort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CListCtrlSort::CListCtrlSort(CListCtrl *pList, const int nCol)
{
	m_pList = pList;
	ASSERT(m_pList);
	
	int totItems = m_pList->GetItemCount();

	for (int ii = 0; ii < totItems; ii++)
	{
		DWORD dw = m_pList->GetItemData(ii);
		CString txt = m_pList->GetItemText(ii, nCol);
		m_pList->SetItemData(ii, (DWORD) new CSortItem(dw, txt));
	}
}

CListCtrlSort::~CListCtrlSort()
{
	ASSERT(m_pList);
	int totItems = m_pList->GetItemCount();
	for (int ii = 0; ii < totItems; ii++)
	{
		CSortItem *pItem = (CSortItem*)(m_pList->GetItemData(ii));
		ASSERT(pItem);
		m_pList->SetItemData(ii, pItem->m_dw);
		delete pItem;
	}
}

void CListCtrlSort::Sort(BOOL bAsc, SortDataType dtType)
{
	long lParamSort = bAsc ? dtType : -dtType;
	m_pList->SortItems(Compare, lParamSort);
}

int CALLBACK CListCtrlSort::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortItem *pItem1 = (CSortItem*)lParam1;
	CSortItem *pItem2 = (CSortItem*)lParam2;
	ASSERT(pItem1 && pItem2);

	int nOrder = (lParamSort < 0) ? -1 : 1;
	SortDataType dtType = (SortDataType) (lParamSort * nOrder); 

	switch (dtType)
	{
	case SortDataType::dtINT:
		return (atol(pItem1->m_txt) - atol(pItem2->m_txt)) * nOrder;
	case SortDataType::dtSTRING:
		return pItem1->m_txt.Compare(pItem2->m_txt) * nOrder;
	default:
		ASSERT("Error: attempt to sort a column without type.");
		return 0;
	}
}
