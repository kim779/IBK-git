#include "stdafx.h"
#include "fxBasketTree.h"
#include "resource.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { imgROOT = 0, imgGROUP, imgWAIT, imgJUMUN, imgEND};

CfxBasketTree::CfxBasketTree()
{
	m_pParent = NULL;
	m_pSendWnd = NULL;
	m_ImageList = NULL;
	
	m_bskType = bskNEW | bskCHS;

	m_arPtr.RemoveAll();
	m_arETF.RemoveAll();

	m_font.DeleteObject();
	ZeroMemory(&m_lf, sizeof(LOGFONT));
	m_lf.lfHeight         = 10 * 10;
	m_lf.lfWidth          = 0;
	m_lf.lfEscapement     = 0;
	m_lf.lfOrientation    = 0;
	m_lf.lfWeight         = FW_NORMAL;
	m_lf.lfItalic         = false;
	m_lf.lfUnderline      = false;
	m_lf.lfStrikeOut      = false;
	m_lf.lfCharSet        = DEFAULT_CHARSET;
	m_lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	m_lf.lfQuality        = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	strcpy(m_lf.lfFaceName, "굴림체");
	m_font.CreatePointFontIndirect(&m_lf);
}

CfxBasketTree::~CfxBasketTree()
{
	if (m_ImageList)
	{
		m_ImageList->DeleteImageList();
		delete m_ImageList;
		m_ImageList = NULL;
	}
	m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CfxBasketTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CfxBasketTree)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CfxBasketTree::Init(CWnd* pParent, CWnd* pSendWnd, WORD bskType)
{
	m_pParent = pParent;
	m_pSendWnd = pSendWnd;
	m_bskType = bskType;
	ModifyStyle(0, TVS_HASLINES|TVS_HASBUTTONS|TVS_SHOWSELALWAYS);

	if (!m_ImageList)
	{
		HINSTANCE hinst = AfxGetResourceHandle();
		AfxSetResourceHandle(::GetModuleHandle("fx_misc.dll"));		
		m_ImageList = new CImageList();
		m_ImageList->Create(IDB_BSKIMG, 16, 1, RGB(255, 0, 255)); 
		SetImageList(m_ImageList, TVSIL_NORMAL); 
		AfxSetResourceHandle(hinst);	
	}
}

void CfxBasketTree::BasketSearch(CString userID)
{
	if (!m_pSendWnd)
		return;

	removeAll();

	struct	bmid	bmid;
	int	midL = sizeof(struct bmid);
	int	datL = 0;
	char*	datB = new char[L_userTH+midL];
	struct	_userTH* uTH;
	uTH = (struct _userTH *) datB;
	datL += L_userTH;

	strcpy(uTH->trc, PIHOBLST);
	uTH->key = BSKTRKEY;
	uTH->stat = 0;

	memset(&bmid, ' ', midL);
	memcpy(&bmid.usid, userID, userID.GetLength());
	CopyMemory(&datB[datL], &bmid, midL);
	datL += midL;
	m_pSendWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, midL), (LPARAM) datB);
	delete [] datB;
}

int CfxBasketTree::GetBasketStatus(HTREEITEM item)
{
	if (!item)	return 0;
	int img, selectedImg;
	GetItemImage(item, img, selectedImg);
	switch (img)
	{
	case imgWAIT:
		return bskWAIT;	break;
	case imgJUMUN:
		return bskJUMUN; break;
	case imgEND:
		return bskEND;	break;
	}
	return 0;
}

void CfxBasketTree::SetBasketStatus(HTREEITEM item, int status)
{
	if (!item)	return;
	_tInfo *tInfo = GetCodeInfo(item);
	if (!tInfo || tInfo->depth == 0 || tInfo->depth == 1)
		return;
	switch (status)
	{
	case bskWAIT:
		SetItemImage(item, imgWAIT, imgWAIT);	break;
	case bskJUMUN:
		SetItemImage(item, imgJUMUN, imgJUMUN);	break;
	case bskEND:
		SetItemImage(item, imgEND, imgEND);	break;
	}
}

CString CfxBasketTree::GetETFInfo()
{
	CString tmpS = _T("");
	CString str = _T("");
	struct _etfcode etfcode;
	for (int ii = 0; ii < m_arETF.GetSize(); ii++)
	{
		etfcode = m_arETF.GetAt(ii);
		tmpS.Format("%s\t%s\n", etfcode.code, etfcode.name);
		str += tmpS;
	}
	return str;
}

void CfxBasketTree::SetTreeInfo(char *buf)
{
	removeAll();
	if (strlen(buf) == 0)
		return;
	
	CWaitCursor	wait;
	
	struct	bgroup	*bgroup = NULL;
	struct	blist	*blist = NULL;
	int	cnt = 0;
	int	idx = 0;
	_tInfo *tInfo = NULL;
	CString	tmpS;
	HTREEITEM rItem, sItem, gItem;
	bool bUse = false;
	CString	gicd, sgicd;
	CString bseq, bskn;
	int stat = 0;

	struct	_etfcode etfcode;
	bool bETF = false;
	if (m_arETF.GetSize() == 0)
		bETF = true;
	rItem = sItem = gItem = NULL;
	while (buf)
	{
		idx++;
		if (idx > 3) 	break;
		
		bgroup = (struct bgroup *)buf;
		buf += sz_BGROUP;
		cnt = atoi(CString(bgroup->acnt, sizeof(bgroup->acnt)));
		
		bUse = true;
		sgicd = _T("");
		switch (idx)
		{
		case 1:
			if (!(m_bskType & bskNEW))
				bUse = false;
			break;
		case 2:
			if (!(m_bskType & bskCHS))
				bUse = false;
			break;
		case 3:
			if (!(m_bskType & bskETF))
				bUse = false;
			break;
		}
		
		if (bUse)
		{
			tInfo = new _tInfo();
			tInfo->depth = 0;
			tInfo->gubn[0] = bgroup->bskg[0];
			tInfo->code = "";
			tmpS = CString(bgroup->fnam, sizeof(bgroup->fnam));
			tmpS.TrimRight();
			rItem = InsertItem(tmpS, imgROOT, imgROOT);
			SetItemData(rItem, (DWORD)tInfo);
			m_arPtr.Add(tInfo);

		}
		
		gItem = NULL;
		for (int ii = 0; ii < cnt; ii++)
		{
			blist = (struct blist *)buf;
			buf += sz_BLIST;

			if (idx == 3 && bETF)	// basket정보 저장
			{
				etfcode.code = CString(blist->bseq, sizeof(blist->bseq));
				etfcode.code.TrimRight();
				etfcode.name = CString(blist->bskn, sizeof(blist->bskn));
				etfcode.name.TrimRight();
				m_arETF.Add(etfcode);
			}

			if (!bUse)
				continue;
			
			if (m_bskType & bskETF)
			{
				tInfo = new _tInfo();
				tInfo->depth = 1;
				tInfo->gubn[0] = bsks_ETF;
				tmpS = CString(blist->bseq, sizeof(blist->bseq));
				tmpS.TrimRight();
				tInfo->code = tmpS;
				tmpS = CString(blist->bskn, sizeof(blist->bskn));
				tmpS.TrimRight();
				sItem = InsertItem(tmpS, imgWAIT, imgWAIT, rItem);
				SetItemData(sItem, (DWORD)tInfo);
				m_arPtr.Add(tInfo);
			}
			else
			{
				gicd = CString(blist->gicd, sizeof(blist->gicd));
				if (gicd != sgicd)	
				{
					sgicd = gicd;	// 기관정보
					tInfo = new _tInfo();
					tInfo->depth = 1;
					tInfo->gubn[0] = bsks_SEARCH;
					
					tmpS = CString(blist->gicd, sizeof(blist->gicd));
					tmpS.TrimRight();
					tInfo->gicd = tmpS;

					tmpS = CString(blist->ginm, sizeof(blist->ginm));
					tmpS.TrimRight();
					gItem = InsertItem(tmpS, imgGROUP, imgGROUP, rItem);
					SetItemData(gItem, (DWORD)tInfo);
					m_arPtr.Add(tInfo);
				}
				tInfo = new _tInfo();
				tInfo->depth = 2;
				tInfo->gubn[0] = bsks_SEARCH;
				
				bseq = CString(blist->bseq, sizeof(blist->bseq));
				bseq.TrimRight();
				tInfo->code = bseq.Right(4);

				tInfo->gicd = gicd;

				bskn = CString(blist->bskn, sizeof(blist->bskn));
				bskn.TrimRight();

				tmpS.Format("%s %s", bseq.Right(4), bskn);
				switch (blist->jsta[0])
				{
				case '1':	// 대기
					stat = imgWAIT;	break;
				case '2':	// 주문
					stat = imgJUMUN;	break;
				case '3':	// 완료
					stat = imgEND;	break;
				}
				sItem = InsertItem(tmpS, stat, stat, gItem);
				SetItemData(sItem, (DWORD)tInfo);
				m_arPtr.Add(tInfo);
			}
			if (gItem)	Expand(gItem, TVE_EXPAND);	
		}
		if (rItem)	Expand(rItem, TVE_EXPAND);
	}
	if (GetRootItem())	SelectItem(GetRootItem());
	AfxGetApp()->RestoreWaitCursor();
}


_tInfo* CfxBasketTree::GetCodeInfo(HTREEITEM item)
{
	if (item == NULL)
		return NULL;
	int depth = 0;
	_tInfo *tInfo = (_tInfo *)GetItemData(item);
	return tInfo;
}

int CfxBasketTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFont(&m_font);
	return 0;
}

void CfxBasketTree::removeAll()
{
	DeleteAllItems();
	_tInfo* tInfo = NULL;
	for (int ii = 0; ii < m_arPtr.GetSize(); ii++)
	{
		tInfo = (_tInfo *)m_arPtr.GetAt(ii);
		delete tInfo;
		tInfo = NULL;
	}
 	m_arPtr.RemoveAll();
}

void CfxBasketTree::OnDestroy() 
{
	removeAll();
	m_arETF.RemoveAll();
	CTreeCtrl::OnDestroy();
}

void CfxBasketTree::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}


HTREEITEM CfxBasketTree::InsertItemEx(CString text, HTREEITEM rItem, _tInfo* tInfo)
{
	HTREEITEM item;
	switch (tInfo->depth)
	{
	case 1:
		item = InsertItem(text, imgGROUP, imgGROUP, rItem);
		break;
	case 2:
		item = InsertItem(text, imgWAIT, imgWAIT, rItem);
		break;
	default:
		return NULL;
	}
	SetItemData(item, (DWORD)tInfo);
	m_arPtr.Add(tInfo);	
	return item;
}