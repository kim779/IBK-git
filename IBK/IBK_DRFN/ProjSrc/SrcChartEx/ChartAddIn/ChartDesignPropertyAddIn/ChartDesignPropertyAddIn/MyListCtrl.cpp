// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyListCtrl.h"
//#include "BlockBaseData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.26
//
//	�м�	:	Graph�� ����� ó���Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CMyListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW *pNMListView = ( NM_LISTVIEW *)pNMHDR;
	// TODO: Add your control notification handler code here
	
	// 0. ���� Result ���� �ʱ�ȭ�Ѵ�.
	*pResult = 0;
	if( !pNMListView) return;

	// 1. ChartItem Data�� ���� Memory�� Ȯ���Ѵ�.
	HGLOBAL hGlobal = GlobalAlloc( GMEM_SHARE | GMEM_ZEROINIT, sizeof( ChartItem));
	if( !hGlobal) return;
	ChartItem *pszGlobal = ( ChartItem *)GlobalLock( hGlobal);
	if( !pszGlobal)
	{
		GlobalFree( hGlobal);
		return;
	}

	// 2. ChartItem Data�� �����Ѵ�. (hItem�� Tree������ �̵��� �̿��Ѵ�.)
	GetItemText( pNMListView->iItem, 0, pszGlobal->ChartName, 64);
	GlobalUnlock( hGlobal);

	// 3. Chart Drop Object���� ��ϵǴ� Graph Type���� �����Ѵ�. (��?)
	m_dropTarget.SetName( pszGlobal->ChartName);

	// 4. DragSource�� Text Type���� Data�� ��Ͻ�Ų��. (Text�� �ƴϸ鼭...)
	m_dragSource.CacheGlobalData( CF_TEXT, hGlobal);

	// 5. DragDrop�� ó���Ѵ�.
	//	(Copy Option�̸� Data�� �����ϴ� ���� ��� ����? �� ���� NULL�� �� CF_TEXT�ε�?)
	DROPEFFECT de;
	de = m_dragSource.DoDragDrop( DROPEFFECT_COPY);
}

void CMyListCtrl::InitDropTarget()
{
	m_dropTarget.SetName("ChartInfo");
	m_dropTarget.Register(this);
	m_dropTarget.SetParent(this);
}
