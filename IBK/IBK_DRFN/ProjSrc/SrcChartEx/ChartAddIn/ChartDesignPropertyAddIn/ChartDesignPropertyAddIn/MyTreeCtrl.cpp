// MyTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "MyTreeCtrl.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()

#include "Conversion.h"
#include "ChartListPage.h"
#include "DataOfChartListPage.h"
#include "DlgBlockVerticalScale.h"
#include "DlgChartInvertSetting.h"

#include "ToolListData.h"			// for BLOCK

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl

// (2003.12.26, ��¿�) m_pChartRoot�� ���� �����ϹǷ� Pointer�� �ƴ϶� Object�� �����Ѵ�.
CMyTreeCtrl::CMyTreeCtrl() : m_clChartRoot(NULL, ROOT, "Chart List", NULL)
{
	m_pChartBlock = NULL;
	m_hTarget = NULL ;
	m_pSelectItem = NULL;

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	m_pIGraphInfoManager = NULL;

	// (2006/11/28 - Seung-Won, Bae) Initializing the Drag&Drop Flag.
	m_DoDrag = FALSE;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CMyTreeCtrl::~CMyTreeCtrl()
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->Release();
}


BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDR_TITLESCALECOLOR, OnTitlescalecolor)
	ON_COMMAND(IDR_DELETE, OnDelete)
	ON_COMMAND(IDR_HORIZONSCALE, OnHorizonscale)
	ON_COMMAND(IDR_VERT_SCALE, OnVertScale)
	ON_COMMAND(IDR_VERT_DELETE, OnVertDelete)
	ON_WM_CREATE()
	ON_COMMAND(IDR_CHARTINVERT, OnChartinvert)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DRAGCHARTDATA, OnDragChartData)
	ON_MESSAGE(WM_DROPCHARTDATA, OnDropChartData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl message handlers

void CMyTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	HTREEITEM hDragItem = pNMTreeView->itemNew.hItem;
	if( !hDragItem) return;

	m_pSelectItem = m_clChartRoot.Find(hDragItem);
	if( m_pSelectItem == NULL) return;

	if( m_pSelectItem->m_iType == GRAPH || m_pSelectItem->m_iType == BLOCK || m_pSelectItem->m_iType == VERT_SCALE) 
	{
		HGLOBAL hGlobal = NULL;
		ChartItem *pszGlobal = NULL;
		hGlobal = GlobalAlloc( GMEM_SHARE, sizeof(ChartItem));
		pszGlobal = (ChartItem *)((LPSTR)GlobalLock( hGlobal ));
		ASSERT( pszGlobal );
		memset(pszGlobal, 0, sizeof(ChartItem));
		pszGlobal->hItem = hDragItem;
		strcpy(pszGlobal->ChartName, (LPSTR)(LPCTSTR)m_pSelectItem->m_strName);
		m_dropTarget.SetName( pszGlobal->ChartName);
		GlobalUnlock( hGlobal );
		if(hGlobal)
		{
			m_DoDrag = TRUE;
			m_dragSource.CacheGlobalData( CF_TEXT, hGlobal );
			DROPEFFECT  de;
			de = m_dragSource.DoDragDrop( DROPEFFECT_MOVE);
			m_DoDrag = FALSE;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.26
//
//	�м�	:	Dragging ������ Graph�� ��Ͻ�ų ��ġ�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
LONG CMyTreeCtrl::OnDragChartData(UINT wParam, LPARAM lParam)
{
	// 1. Dragging ���� ��ġ Point�� ���Ѵ�.
	CPoint point;
	point.x = LOWORD((DWORD)lParam);
	point.y = HIWORD((DWORD)lParam);

	// 2. Terget Item�� ���Ѵ�.
	UINT uHitTest = TVHT_ONITEM;
	m_hTarget = HitTest( point, &uHitTest );
	if( !m_hTarget) return 0;

	// 3. Target Item�� Object�� ���Ѵ�.
	CChartLink* pDragItem = m_clChartRoot.Find( m_hTarget);
	if( !pDragItem) return 0L;

	// 4. Graph Type List�κ��ͳ� Tree�� Graph�� Dragging�Ǵ� ���,
	//		Graph�̹Ƿ� ������ Target���� �����Ѵ�.
	if( !m_pSelectItem || m_pSelectItem->m_iType == GRAPH) SelectDropTarget( m_hTarget);

	// 5. �׷��� ���� ���, �� Graph�� �ƴ� ��쿡�� ���� Type�̸鼭 ���� Group�� ���� ��쿡�� Target���� �����Ѵ�.
	else if( pDragItem->m_Parent == m_pSelectItem->m_Parent && pDragItem->m_iType == m_pSelectItem->m_iType)
		SelectDropTarget( m_hTarget);

	// 6. Terget�� �������� ���� ��쿡�� Target Handle�� Clear�Ѵ�.
	else m_hTarget = NULL;

	return 0L;
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.26
//
//	�м�	:	Drop���� Graph�� ����� ó���Ѵ�.
///////////////////////////////////////////////////////////////////////////////
LONG CMyTreeCtrl::OnDropChartData(UINT wParam, LPARAM lParam)
{
	// 0. Drop�� �����̴� ���д�, Tree���� �̵��Ǵ� Source Item�� Clear��Ų��.
	//		(m_pSelectItem�� Dragging ������ Tree�κ��� �̵��ϴ� Source�� Type�� Ȯ���ϱ� ���� ���̴�.)
	m_pSelectItem = NULL;

	// 0. ���� ChartItem Data�� Ȯ���Ѵ�.
	ChartItem *pData = (ChartItem *)lParam;
	if( !pData) return 0L;

	// 1. Drop ��� Node Item�� �˻��ϰ� ������ Target Item Handle�� Clear��Ų��.
	if( !m_hTarget) return 0L;
	CChartLink *pTarget = m_clChartRoot.Find( m_hTarget);
	m_hTarget = NULL;
	if(pTarget == NULL) return 0L;

	// 2. SubGraph Item�� �˻��Ͽ� ����Ѵ�. (List���� Dragging�� ���̸� ���� �����Ѵ�.)
	CChartLink *pSource;
	if( m_DoDrag == TRUE)
	{
		// 2.1 Tree���� �ű�� ��츦 ó���Ѵ�.
		//	(Graph�� �ƴ� ����� Target ������ �̹� OnDragChartData()���� �̷�����.)
		// 2.1.1 Tree���� Drag�� Item�� �˻��� ����.
		pSource = m_clChartRoot.Find( pData->hItem);
		if( !pSource) return 0L;
		// 2.1.2 ���ڸ��� Drop ��Ű�� ���̶�� �׳� ����Ѵ�.
		if( pTarget == pSource) return 0L;
		// 2.1.3 Source Item�� �и��� ����.
		switch( pSource->m_iType)
		{
			case GRAPH:
			{
				// 2.1.3 Graph�� �߶󳻴� ���, ������ Parent�� Vertical Scale�� Ȯ���Ͽ� ��� ��� �Բ� �����Ѵ�.
				CChartLink *pVertScale = pSource->m_Parent;
				pSource->Cut();
				DeleteGRAPH_OnlyTreeItem( pSource);
				// 2.1.3.1 ���� Target�� �ٷ� �� Vertical Scale�� �ƴ� ��쿡 (������ ���� Drop��Ű��?)
				if( !pVertScale->m_Child && pVertScale != pTarget)
				{
					pVertScale->Cut();
					DeleteVERT_SCALE_OnlyTreeItem( pVertScale);
					delete pVertScale;
				}
				break;
			}
			case VERT_SCALE:	pSource->Cut();
								DeleteVERT_SCALE_OnlyTreeItem( pSource);
								break;
			case BLOCK:	pSource->Cut();
						DeleteBLOCK_OnlyTreeItem( pSource);
						break;
		}
		// 2.1.4 Drop�� ó���Ѵ�.
		//	(Root�� �ű�� ���� �̹� �źεǸ�, VertScale�̳� Block�� �������� �����ϴ�.
		//	 ����, �� Type���� �����Ͽ� ó���� �ʿ䰡 ����.)
		// 2.1.4.0 Graph�� Root���� �̵���Ű�� ��츦 ó���Ѵ�.
		//	(m_pChartBlock�� ���ο� Vertical Scale�� ����� �߰��Ѵ�.)
		if( pSource->m_iType == GRAPH && pTarget->m_iType == ROOT)
		{
			pTarget = m_pChartBlock;
			CChartLink *pVertScale = new CChartLink(NULL, VERT_SCALE, "Vertical Scale", m_hOcxWnd);
			// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
			pVertScale->m_nScaleCompartType = GetFirstVScaleGridType( &m_clChartRoot);
			pTarget->AddChild(pVertScale);
			pVertScale->AddChild(pSource);
		}
		// 2.1.4.1 Graph�� Block���� �̵���Ű�� ��츦 ó���Ѵ�.
		//	(���ο� Vertical Scale�� ����� �߰��Ѵ�.)
		else if( pSource->m_iType == GRAPH && pTarget->m_iType == BLOCK)
		{
			CChartLink *pVertScale;
			pVertScale = new CChartLink(NULL, VERT_SCALE, "Vertical Scale", m_hOcxWnd);
			// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
			pVertScale->m_nScaleCompartType = GetFirstVScaleGridType( &m_clChartRoot);
			pTarget->AddChild(pVertScale);
			pVertScale->AddChild(pSource);
		}
		// 2.1.4.2 Graph�� Vertical Scale�� �̵���Ű�� ��츦 ó���Ѵ�.
		//	(�ٷ� Child�� ����Ѵ�.)
		else if(pSource->m_iType == GRAPH && pTarget->m_iType == VERT_SCALE)
			pTarget->AddChild(pSource);
		// 2.1.4.3 �׹��� ��츦 ó���Ѵ�.
		//	(Target�� Parent�� Child�� ����Ѵ�. �ǵڿ�)
		else pTarget->m_Parent->AddChild(pSource);
	}
	else
	{
		// 2.2 Graph Type List���� Graph�� �ű��߰��ϴ� ��츦 ó���Ѵ�.
		// 2.2.1 ���� Graph�� ChartLink Object�� �����Ѵ�.
		pSource = new CChartLink(NULL, GRAPH, pData->ChartName, m_hOcxWnd);
		// 2.2.2 Target Item�� �� Type�� Graph�� ���ó���Ѵ�.
		switch( pTarget->m_iType)
		{
			case ROOT:
			{
				pTarget = m_pChartBlock;
				CChartLink *pVertScale = new CChartLink(NULL, VERT_SCALE, "Vertical Scale", m_hOcxWnd);
				// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
				pVertScale->m_nScaleCompartType = GetFirstVScaleGridType( &m_clChartRoot);
				pTarget->AddChild(pVertScale);
				pVertScale->AddChild(pSource);
				break;
			}
			// 2.2.2.2 Block�� ��� Vertical Scale�� �߰�����, Vertical Scale�� ����� �����Ѵ�.
			case BLOCK:
			{
				CChartLink *pVertScale = new CChartLink(NULL, VERT_SCALE, "Vertical Scale", m_hOcxWnd);
				// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
				pVertScale->m_nScaleCompartType = GetFirstVScaleGridType( &m_clChartRoot);
				pTarget->AddChild(pVertScale);
				pTarget = pVertScale;
				// (����) NO BREAK
			}
			case VERT_SCALE:
			{
				if( pTarget->m_Child == NULL)
				{
					pTarget->m_Child = pSource;
					pSource->m_Parent = pTarget;
				}
				else
				{
					pSource->m_Parent = pTarget;
					pSource->m_Next = pTarget->m_Child;
					pTarget->m_Child->m_Prev = pSource;
					pTarget->m_Child = pSource;
				}
				break;
			}
			case GRAPH:	pTarget->InsertAfterTarget(pSource);
						break;
			default:	delete pSource;
						return 0;
		}
	}

	// 3. ��ϵ� Item�� Tree�� �ݿ��Ѵ�.
	DeleteAllItems();
	InitTreeView();

	return 0L;
}

void CMyTreeCtrl::InitDropTarget()
{
	m_dropTarget.SetName("ChartInfo");
	m_dropTarget.Register(this);
	m_dropTarget.SetParent(this);
}

/////////////////////////////////////////////////////////////////////


HTREEITEM CMyTreeCtrl::AddItem(HTREEITEM hItem, LPSTR name)
{
	if(hItem == NULL) return (HTREEITEM)0;
	HTREEITEM nhItem = 0;

	TV_INSERTSTRUCT	tvstruct;
	tvstruct.hParent = hItem;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = GetimageofIndicatorTypeIndex( ( char *)_LTOM5( name)) ;
	tvstruct.item.iSelectedImage = tvstruct.item.iImage;
	tvstruct.item.pszText = name;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	nhItem = InsertItem(&tvstruct);
	EnsureVisible( nhItem );

	return nhItem;
}

HTREEITEM CMyTreeCtrl::AddBlock(HTREEITEM hItem, LPSTR name)
{
	if(hItem == NULL) return (HTREEITEM)0;
	HTREEITEM nhItem = 0;

	TV_INSERTSTRUCT	tvstruct;
	tvstruct.hParent = hItem;
	tvstruct.hInsertAfter = TVI_LAST ;
	tvstruct.item.iImage = ROOT ;
	tvstruct.item.iSelectedImage = tvstruct.item.iImage;
	tvstruct.item.pszText = name;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	nhItem = InsertItem(&tvstruct);
	EnsureVisible( nhItem );

	((CChartListPage* )GetParent())->SetToolTipInfo(nhItem, "Block");

	return nhItem;
}


int CMyTreeCtrl::GetimageofIndicatorTypeIndex( LPSTR strGraphType)
{
	for(int i = 0 ; i < GRAPHTYPE_ITEM ; i++) {
		if(strcmp( strGraphType, _MTEXTN( pGraphTypeItem[ i])) == 0)
			return i + 1 ;
	}
	return 0 ;
}

void CMyTreeCtrl::InitTreeView()
{
	if(m_ImageList == NULL)
	{
		ML_SET_DEFAULT_RES();
		m_ImageList.Create(IDB_LIST, 16, 1, RGB(172, 172, 172));
	}
	SetImageList(&m_ImageList, TVSIL_NORMAL);

	// Tree Chart Init Root
	TV_INSERTSTRUCT tvinsert;

	tvinsert.hParent = (HTREEITEM)0 ;
	tvinsert.hInsertAfter = TVI_LAST ;
	tvinsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT ;
	tvinsert.item.state = 0 ;
	tvinsert.item.stateMask = 0 ;
	tvinsert.item.cchTextMax = 60 ;
	tvinsert.item.iImage= ROOT;
	tvinsert.item.iSelectedImage = ROOT;
	tvinsert.item.cChildren = 0 ;
	tvinsert.item.pszText = _T("Chart List") ;
	
	HTREEITEM hRoot = InsertItem(&tvinsert);
	m_clChartRoot.m_hItem = hRoot ;
	((CChartListPage* )GetParent())->SetToolTipInfo(hRoot, "Root");

	// Tree Chart Init List
	CChartLink* pBlock = m_clChartRoot.m_Child ;
	while(pBlock) {//Block
		pBlock->m_hItem = AddBlock(m_clChartRoot.m_hItem, (LPSTR)(LPCTSTR)pBlock->m_strName) ;
		CChartLink* pVertScale = pBlock->m_Child;
		while(pVertScale) { //vert Scale
			pVertScale->m_hItem = AddVertScale(pBlock->m_hItem, (LPSTR)(LPCTSTR)pVertScale->m_strName) ;
			CChartLink* pGraph = pVertScale->m_Child;
			while(pGraph) {//Graph
				pGraph->m_hItem = AddItem(pVertScale->m_hItem, (LPSTR)(LPCTSTR)pGraph->m_strName) ;
				//nami//tooltip
				if(pGraph == NULL) return;
				CString str;
				str.Format("[IndicatorName]: %s,  [Title1]: %s", pGraph->m_strIndicatorName, GetTitle(pGraph->m_strSubGraphDatas));
				((CChartListPage* )GetParent())->SetToolTipInfo(pGraph->m_hItem, str);
				//nami End
				pGraph = pGraph->m_Next; //next Graph
			}
			pVertScale = pVertScale->m_Next ; //next VertScale
		}
		pBlock = pBlock->m_Next ; //next Block
	}
}

/////////////////////////////////////////////////////////////////////
//	Menu

void CMyTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnRButtonDown(nFlags, point);
	UINT uHitTest = TVHT_ONITEM ;
	HTREEITEM hItem = HitTest( point, &uHitTest );
	if(hItem == NULL) return;

	SelectItem(hItem) ;
	CChartLink *pItem	= m_clChartRoot.Find(hItem);

	BOOL bResult = FALSE ;
	CMenu Context, *Popup = NULL ;

	ML_SET_LANGUAGE_RES();
	if(pItem != NULL && pItem->m_iType == ROOT)
		bResult	= Context.LoadMenu(IDR_BLOCKMENU);
	else if(pItem != NULL && pItem->m_iType == BLOCK)
		bResult	= Context.LoadMenu(IDR_BLOCKMENU);
	else if(pItem != NULL && pItem->m_iType == VERT_SCALE)
		bResult	= Context.LoadMenu(IDR_VERT_SCALE_MENU);
	else if(pItem != NULL && pItem->m_iType == GRAPH)
		bResult	= Context.LoadMenu(IDR_ITEMMENU);
	
	if(bResult)
	{
		Popup = Context.GetSubMenu(0);		
		ClientToScreen(&point);
		BOOL brack = Popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
					point.x, point.y, this, NULL);
		Context.DestroyMenu();
	}
}

void CMyTreeCtrl::OnDelete() 
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL) return;
	CChartLink *pItem = m_clChartRoot.Find(hItem);
	if(pItem == NULL) return;
	if(pItem->m_iType == ROOT) return;


	CChartLink* pParent = pItem->m_Parent;
	if(pItem->m_Prev == NULL && pItem->m_Next == NULL) 
		OnDeleteVertScale(pParent);
	else {
		pItem->Cut();
		DeleteItem(pItem->m_hItem);
		delete pItem;
	}
}

CString CMyTreeCtrl::GetBlockandGraphAllData()
{
	CDataOfChartListPage data( m_hOcxWnd);
	return data.GetBlockandGraphAllData( &m_clChartRoot);
}

void CMyTreeCtrl::AddItem(CChartLink* pVertiScale,
						  const CString& strName,
						  const CString &p_strIndicatorName,
						  const CString &p_strGraphName,
						  const CString& strPacket,
						  const bool bLowHighUpDown,
						  const bool bLowNowUpDown,
						  const bool bHighNowUpDown,
						  const bool bPreNowContrast,
						  const int nCurData,
						  //sy 2006.04.17.
						  const bool bSignalUse,
						  const bool bSignalShow,
						  //sy end
						  const CString& strColorValue)
{
	CChartLink* pSource = new CChartLink(NULL, GRAPH, (LPSTR)(LPCTSTR)strName, m_hOcxWnd);
	pSource->m_strName = strName;
	pSource->m_strIndicatorName = p_strIndicatorName;
	pSource->m_strGraphName = p_strGraphName;
	pSource->m_strCheckedPacketItems = strPacket;

	pSource->m_bLowHighUpDown = bLowHighUpDown; //ȭ������� ���� ��ġ����Ÿ ǥ��
	pSource->m_bLowNowUpDown = bLowNowUpDown; //
	pSource->m_bHighNowUpDown = bHighNowUpDown; //
	pSource->m_bPreNowContrast = bPreNowContrast; //
	pSource->m_nCurData = nCurData; //

	//sy 2006.04.17.
	pSource->m_bSignalUse = bSignalUse;
	pSource->m_bSignalShow = bSignalShow;
	//sy end
	pSource->m_strSubGraphDatas = strColorValue;

	if(pVertiScale->m_Child == NULL) {
		pVertiScale->m_Child = pSource;
		pSource->m_Parent = pVertiScale;
	}
	else {
		pSource->m_Parent = pVertiScale;
		pVertiScale->m_Child->Add(pSource);
	}
}

void CMyTreeCtrl::InitBlock(const CString& strData, const int nRow, const int nColumn)
{
	CString allData = strData;
	for(int i = 0; i< nColumn; i++)
	{
		CString strColumnData = CDataConversion::GetStringData(allData, "BlockColumn;\r\n", "BlockColumnEnd;\r\n", true);
		if(strColumnData.IsEmpty()){
			strColumnData = allData;
			allData.Empty();
		}

		for(int j =0; j< nRow;j++)
		{
			CString BlockName;
			BlockName.Format("%s(%d,%d)", "Block", (nRow-j), (nColumn-i));
			m_pChartBlock = new CChartLink( &m_clChartRoot, BLOCK, ( LPSTR)( LPCTSTR)BlockName, m_hOcxWnd);	
			m_pChartBlock->m_nColumnIndex = (nColumn-i);
			m_clChartRoot.InsertChild( m_pChartBlock);
		}
	}
}

void CMyTreeCtrl::OnHorizonscale() 
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL) return;
	CChartLink *pItem = m_clChartRoot.Find(hItem);
	if(pItem == NULL) return;

	CDlgBlockHorizontalScale dlgBlockHorizonScale( pItem, m_hOcxWnd);
	if(m_clChartRoot.m_hItem == hItem) 
		SetDlgBlockHorizontalScale(m_pChartBlock, dlgBlockHorizonScale);
	else  
		SetDlgBlockHorizontalScale(pItem, dlgBlockHorizonScale);

	ML_SET_LANGUAGE_RES();
	//Root���� ������ ������ ��� ���� ����.
	if(m_clChartRoot.m_hItem == hItem) {
		if(dlgBlockHorizonScale.DoModal() == IDOK) {
			SetDlgValueToCChartLinkClass(dlgBlockHorizonScale, pItem);
			SetRootValueToAllBlocks(pItem);
		}
	}
	else { //�ٸ� ������ �������� �ʴ´�.
		if(dlgBlockHorizonScale.DoModal() == IDOK)
			SetDlgValueToCChartLinkClass(dlgBlockHorizonScale, pItem);
	}
}

//sy 2004.3.12.
// -> �Լ��� ������(���� : AddBlock). �̸����� ���� �Լ��� ��� ������ ��� �߻���.
void CMyTreeCtrl::ChangeBlockData(const int nBlock, const CString& szScaleData, const int nHorizonPos, const int nScaleCompart, const CString& szDate, const CString& time, const int nHorizonStyle, const int nLog, const int nOverTurn)
{
	if(m_pChartBlock == NULL)
		return;

	int i =0;
	CChartLink* pLink = m_clChartRoot.m_Child ;
	while(pLink) {
		if(i == nBlock)	{
			m_pChartBlock = pLink;
			break;
		}
		i++;
		pLink = pLink->m_Next ;
	}

	m_pChartBlock->m_strScaleStyle = szScaleData;
	m_pChartBlock->m_nHorizonpos = nHorizonPos;
	m_pChartBlock->m_nScaleCompartType = nScaleCompart;
	m_pChartBlock->m_strDateScaleCompart = szDate;
	m_pChartBlock->m_strTimeScaleCompart = time;
	m_pChartBlock->m_nHorizonStyle = nHorizonStyle;
	m_pChartBlock->m_nLog = nLog;
}


void CMyTreeCtrl::SetDlgBlockHorizontalScale(CChartLink *pItem, CDlgBlockHorizontalScale& dlgBlockHorizonScale)
{
	if((pItem->m_strScaleStyle).Compare( _LTEXT0( C0_VOLUME)) == 0) //���� ����Ǿ� �ִ� String�� �ٲٱ� ���ؼ�.
		pItem->m_strScaleStyle = _LTEXT6( C0_INVERSETIMELINE_VOLUME);

	dlgBlockHorizonScale.m_strScaleData = pItem->m_strScaleStyle;
	dlgBlockHorizonScale.m_nHorizonScalePos = pItem->m_nHorizonpos;
	dlgBlockHorizonScale.m_strDateCompartType = pItem->m_strDateScaleCompart;
	dlgBlockHorizonScale.m_StrTypeTimeCompartType = pItem->m_strTimeScaleCompart;
	dlgBlockHorizonScale.m_nStyleHorizonScale = pItem->m_nHorizonStyle;
	dlgBlockHorizonScale.m_nScaleCompartType = pItem->m_nScaleCompartType;
	dlgBlockHorizonScale.SetAllItemInfoOfPacketPage(m_strAllItemInfoOfPacketPage);
}

void CMyTreeCtrl::SetRootValueToAllBlocks(CChartLink *pItem)
{
	CChartLink* pLink = m_clChartRoot.m_Child ;
	while(pLink)
	{
		pLink->m_strScaleStyle = pItem->m_strScaleStyle;
		pLink->m_nHorizonpos = pItem->m_nHorizonpos;
		pLink->m_strDateScaleCompart = pItem->m_strDateScaleCompart;
		pLink->m_strTimeScaleCompart = pItem->m_strTimeScaleCompart;
		pLink->m_nHorizonStyle = pItem->m_nHorizonStyle;
		pLink->m_nScaleCompartType = pItem->m_nScaleCompartType;
		pLink = pLink->m_Next ;
	}
}

void CMyTreeCtrl::SetDlgValueToCChartLinkClass(CDlgBlockHorizontalScale& dlgBlockHorizonScale, CChartLink *pItem)
{
	pItem->m_strScaleStyle = dlgBlockHorizonScale.m_strScaleData;
	pItem->m_nHorizonpos = dlgBlockHorizonScale.m_nHorizonScalePos;
	pItem->m_strDateScaleCompart = dlgBlockHorizonScale.m_strDateCompartType;
	pItem->m_strTimeScaleCompart = dlgBlockHorizonScale.m_StrTypeTimeCompartType;
	pItem->m_nHorizonStyle = dlgBlockHorizonScale.m_nStyleHorizonScale;
	pItem->m_nScaleCompartType = dlgBlockHorizonScale.m_nScaleCompartType;
}

void CMyTreeCtrl::DeleteGRAPH_OnlyTreeItem(CChartLink *pLink)
{
	DeleteItem(pLink->m_hItem);
	pLink->m_hItem = (HTREEITEM)0;		
}

void CMyTreeCtrl::DeleteBLOCK_OnlyTreeItem(CChartLink *pLink)
{
	CChartLink* pVerScale = pLink->m_Child;
	while(pVerScale)
	{
		CChartLink* pGraph = pVerScale->m_Child;
		while(pGraph)
		{
			if(pGraph->m_hItem != (HTREEITEM)0)
			{
				DeleteItem(pGraph->m_hItem);
				pGraph->m_hItem = (HTREEITEM)0;				
			}
			pGraph = pGraph->m_Next;
		}
		if(pVerScale->m_hItem != (HTREEITEM)0)
		{
			DeleteItem(pVerScale->m_hItem);
			pVerScale->m_hItem = (HTREEITEM)0;				
		}
		pVerScale = pVerScale->m_Next;
	}
	DeleteItem(pLink->m_hItem);
	pLink->m_hItem = (HTREEITEM)0;
}


void CMyTreeCtrl::DeleteVERT_SCALE_OnlyTreeItem(CChartLink *pLink)
{
	CChartLink* pGraph = pLink->m_Child;
	while(pGraph)
	{
		if(pGraph->m_hItem != (HTREEITEM)0)
		{
			DeleteItem(pGraph->m_hItem);
			pGraph->m_hItem = (HTREEITEM)0;				
		}
		pGraph = pGraph->m_Next;
	}
	DeleteItem(pLink->m_hItem);
	pLink->m_hItem = (HTREEITEM)0;
}

CString CMyTreeCtrl::GetTitle(const CString &strTitle)
{
	CString strData = strTitle;
	CDataConversion::GetStringData(strData, ";");
	CDataConversion::GetStringData(strData, ";");
	return CDataConversion::GetStringData(strData, ";");
}

void CMyTreeCtrl::SetColorOfLine(const int nLineIndex, const CString &strItemValue)
{
	m_LineColor.SetLineColor(nLineIndex, strItemValue);
}

void CMyTreeCtrl::SetAllItemInfoOfPacketPage(const CString &strAllItemInfoOfPacketPage)
{
	m_strAllItemInfoOfPacketPage = strAllItemInfoOfPacketPage;
}

HTREEITEM CMyTreeCtrl::AddVertScale(HTREEITEM hItem, LPSTR name)
{
	if(hItem == NULL) return (HTREEITEM)0;
	HTREEITEM nhItem = 0;

	TV_INSERTSTRUCT	tvstruct;
	tvstruct.hParent = hItem;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = ROOT;
	tvstruct.item.iSelectedImage = tvstruct.item.iImage;
	tvstruct.item.pszText = name;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	nhItem = InsertItem(&tvstruct);
	EnsureVisible( nhItem );

	((CChartListPage* )GetParent())->SetToolTipInfo(nhItem, "Vertical Scale");

	return nhItem;
}

void CMyTreeCtrl::OnVertScale() 
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL) return;
	CChartLink *pItem = m_clChartRoot.Find(hItem);
	if(pItem == NULL) return;

	ML_SET_LANGUAGE_RES();
	CDlgBlockVerticalScale dlgBlockVerticalScale(pItem, m_hOcxWnd);
	dlgBlockVerticalScale.SetAllItemInfoOfPacketPage(m_strAllItemInfoOfPacketPage);

	if(dlgBlockVerticalScale.DoModal() == IDOK)
	{
		pItem->m_nScalePosition = dlgBlockVerticalScale.m_nScalePosition;
		pItem->m_strScaleStyle = dlgBlockVerticalScale.m_strScaleData; 
		pItem->m_nScaleCompartType = dlgBlockVerticalScale.m_nScaleCompartType;
		pItem->m_nScaleExpressionType = dlgBlockVerticalScale.m_nScaleExpressionType;

		// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
		pItem->m_bMinMaxLock	= dlgBlockVerticalScale.m_bMinMaxLock;
		pItem->m_eMinMaxType	= ( CScaleBaseData::VERTMINMAXTYPE)dlgBlockVerticalScale.m_nMinMaxType;
		pItem->m_dMax			= dlgBlockVerticalScale.m_dMax;
		pItem->m_dMin			= dlgBlockVerticalScale.m_dMin;

		// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
		pItem->m_nUpSpace	= dlgBlockVerticalScale.m_nUpSpace;
		pItem->m_nDownSpace	= dlgBlockVerticalScale.m_nDownSpace;

		//2007.01.22 Scale����� ����,ȣ��������
		pItem->m_dHogaNumber	= dlgBlockVerticalScale.m_dHogaNumber;
		pItem->m_dUserNumber	= dlgBlockVerticalScale.m_dUserNumber;
		pItem->m_nUserHogaType	= dlgBlockVerticalScale.m_nUserHogaType;
	
		// ��ǥ ������ - ojtaso (20071023)
		pItem->m_bIsInvert		= dlgBlockVerticalScale.m_bIsInvert;
	}
}

CChartLink *CMyTreeCtrl::AddVerticalScaleItem(	const CString &	p_strScaleData,
												const int		p_nScalePos,
												const int		p_nScaleCompart,
												const int		p_nScaleExpressionType,
												const BOOL		p_bMinMaxLock,			// Lock		// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
												const int		p_nMinMaxType,			// Type
												const double	p_dMin,					// Min
												const double	p_dMax,					// Max
												const int		p_nUpSpace,				// UpSpace		// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
												const int		p_nDownSpace,			// DownSpace
												const double	p_dHogaNumber,			//2007.01.22 Scale����� ����,ȣ��������
												const double	p_dUserNumber,
												const int		p_nUserHogaType,
												const BOOL		p_bIsInvert				// ��ǥ ������ - ojtaso (20071023)
											 )
{
	if(m_pChartBlock == NULL)
		return NULL;

	CChartLink* pSource = new CChartLink(NULL, VERT_SCALE, "Vertical Scale", m_hOcxWnd);
	
	pSource->m_strScaleStyle = _MTOLS2( p_strScaleData, ',', 6, 0);
	pSource->m_nScalePosition = p_nScalePos;
	pSource->m_nScaleCompartType = p_nScaleCompart;
	pSource->m_nScaleExpressionType = p_nScaleExpressionType;

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	pSource->m_bMinMaxLock	= p_bMinMaxLock;
	pSource->m_eMinMaxType	= ( CScaleBaseData::VERTMINMAXTYPE)p_nMinMaxType;
	pSource->m_dMin			= p_dMin;
	pSource->m_dMax			= p_dMax;

	// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
	pSource->m_nUpSpace			= p_nUpSpace;
	pSource->m_nUpSpace			= p_nDownSpace;

	//2007.01.22 Scale����� ����,ȣ��������
	pSource->m_dHogaNumber		= p_dHogaNumber;
	pSource->m_dUserNumber		= p_dUserNumber;
	pSource->m_nUserHogaType	= p_nUserHogaType;
	
	// ��ǥ ������ - ojtaso (20071023)
	pSource->m_bIsInvert		= p_bIsInvert;

	if(m_pChartBlock->m_Child == NULL) {
		m_pChartBlock->m_Child = pSource;
		pSource->m_Parent = m_pChartBlock;
	}
	else {
		pSource->m_Parent = m_pChartBlock;
		m_pChartBlock->m_Child->Add(pSource);
	}
	return pSource;
}

void CMyTreeCtrl::OnVertDelete() 
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL) return;
	CChartLink *pItem = m_clChartRoot.Find(hItem);
	if(pItem == NULL) return;
	if(pItem->m_iType == ROOT) return;

	//child���� ��� delete
	OnDeleteVertScale(pItem);
}

void CMyTreeCtrl::OnDeleteVertScale(CChartLink *pVertScale)
{
	CChartLink* pGraph = pVertScale->m_Child;
	while(pGraph)
	{
		if(pGraph->m_hItem != (HTREEITEM)0)
		{
			pGraph->Cut();
			DeleteItem(pGraph->m_hItem);
			pGraph->m_hItem = (HTREEITEM)0;	
		}
		CChartLink* pOld = pGraph;
		pGraph = pGraph->m_Next;
		delete pOld;
	}
	pVertScale->Cut();
	DeleteItem(pVertScale->m_hItem);
	pVertScale->m_hItem = (HTREEITEM)0;
	delete pVertScale;
}

void CMyTreeCtrl::SetScaleCompartTypeToAllBlocks(const int nScaleCompartType)
{
	m_clChartRoot.m_nScaleCompartType = nScaleCompartType;
	CChartLink* pBlock = m_clChartRoot.m_Child;
	while(pBlock)
	{
		pBlock->m_nScaleCompartType = nScaleCompartType;
		CChartLink* pVertical = pBlock->m_Child;
		while(pVertical) {
			pVertical->m_nScaleCompartType = nScaleCompartType;
			pVertical = pVertical->m_Next;
		}
		pBlock = pBlock->m_Next;
	}
}

int CMyTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	DeleteAllItems();
	InitTreeView();	
	
	return 0;
}

//sy 2003.6.23. -> chart ������ ����
void CMyTreeCtrl::OnChartinvert() 
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL) return;

	CChartLink *pItem = m_clChartRoot.Find(hItem);
	if(pItem == NULL) return;

	ML_SET_LANGUAGE_RES();
	CDlgChartInvertSetting dlg( pItem);
	dlg.DoModal();
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CMyTreeCtrl::SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager)
{
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->Release();
	m_pIGraphInfoManager = p_pIGraphInfoManager;
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->AddRef();
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CMyTreeCtrl::OnTitlescalecolor() 
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL) return;
	CChartLink *pItem = m_clChartRoot.Find(hItem);
	if(pItem == NULL) return;
	if(pItem->m_iType == ROOT) return;

	ML_SET_LANGUAGE_RES();
	CDlgGraphSetting dlgGraph(pItem, &m_LineColor, m_pIGraphInfoManager, m_hOcxWnd, this);
    dlgGraph.SetAllItemInfoOfPacketPage(m_strAllItemInfoOfPacketPage);
	dlgGraph.DoModal();

	// (2006/10/25 - Seung-Won, Bae) Set Item Title with First SubGraph Type.
	int nGraphType = atoi( pItem->m_strSubGraphDatas.Left( 1));
	CString strTypeData, strStyleData;
	if( !m_pIGraphInfoManager->GetGraphTypeAndStyle( nGraphType, 0, strTypeData, strStyleData)) return;
	SetItemText( hItem, _MTOL5( strTypeData));
}


// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
CScaleBaseData::SCALEGRIDTYPE CMyTreeCtrl::GetFirstVScaleGridType( CChartLink *p_pLink)
{
	if( !p_pLink) return CScaleBaseData::LONGDOTTEDLINE;

	if( p_pLink->m_iType == VERT_SCALE) return ( CScaleBaseData::SCALEGRIDTYPE)p_pLink->m_nScaleCompartType;
	
	return GetFirstVScaleGridType( p_pLink->m_Child);
}

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
void CMyTreeCtrl::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	m_clChartRoot.SetOcxHwnd( m_hOcxWnd);
}
