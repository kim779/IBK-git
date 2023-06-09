// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB315400.h"
#include "MapWnd.h"
#include "../../h/axisvar.h"
#include "../../control/fx_misc/misctype.h"
#include "../../control/fx_GridEx/UGCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		1
#define GAP_BLOCK		5
#define GAP_PANEL		4

#define clContents 181
#define clGuide	26
#define	clBack	64
#define clBox	65
#define	clText	69
#define	clBackIn	66
#define	clBack		64
#define	clcontentsBack		181

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
	, m_panel(NULL)
	, m_pAccTitle(NULL)
	, m_pGroupTitle(NULL)
	, m_pSelTitle(NULL)
	, m_pAccGrid(NULL)
	, m_pGroupGrid(NULL)
	, m_pSelGrid(NULL)
	, m_pLtoR(NULL)
	, m_pNormalFont(NULL)
	, m_pBoldFont(NULL)
	, m_strRoot(_T(""))
	, m_pIDStatic(NULL)
	, m_pID(NULL)
	, m_pNameStatic(NULL)
	, m_pName(NULL)
	, m_pAddModify(NULL)
	, m_pGroupDelete(NULL)
//	, m_pGroupUp(NULL)
//	, m_pGroupDown(NULL)
	, m_pAccStatic(NULL)
	, m_pAccount(NULL)
	, m_pSelModify(NULL)
	, m_pSelDelete(NULL)
//	, m_pSelUp(NULL)
//	, m_pSelDown(NULL)
	, m_pDefault(NULL)
	, m_pOk(NULL)
	, m_pCancel(NULL)
	, m_pApply(NULL)
	, m_pLinePen(NULL)
	
{
}

CMapWnd::~CMapWnd()
{

	_SAFEDESTROY(m_panel);
	_SAFEDESTROY(m_pAccTitle);
	_SAFEDESTROY(m_pGroupTitle);
	_SAFEDESTROY(m_pSelTitle);
	_SAFEDESTROY(m_pAccGrid);
	_SAFEDESTROY(m_pGroupGrid);
	_SAFEDESTROY(m_pSelGrid);
	_SAFEDESTROY(m_pLtoR);
	_SAFEDESTROY(m_pIDStatic);
	_SAFEDESTROY(m_pID);
	_SAFEDESTROY(m_pNameStatic);
	_SAFEDESTROY(m_pName);
	_SAFEDESTROY(m_pAddModify);
	_SAFEDESTROY(m_pGroupDelete);
//	_SAFEDESTROY(m_pGroupUp);
//	_SAFEDESTROY(m_pGroupDown);
	_SAFEDESTROY(m_pAccStatic);
	_SAFEDESTROY(m_pAccount);
	_SAFEDESTROY(m_pSelModify);
	_SAFEDESTROY(m_pSelDelete);
//	_SAFEDESTROY(m_pSelUp);
//	_SAFEDESTROY(m_pSelDown);
	_SAFEDESTROY(m_pDefault);
	_SAFEDESTROY(m_pOk);
	_SAFEDESTROY(m_pCancel);
	_SAFEDESTROY(m_pApply);
	
	
	m_pLinePen->DeleteObject();
	delete m_pLinePen;
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LTOR, OnLtoR)
	ON_BN_CLICKED(IDC_ADDMODIFY, OnAddModify)
	ON_BN_CLICKED(IDC_GROUPDELETE, OnGroupDelete)
	ON_BN_CLICKED(IDC_SELMODIFY, OnSelModify)
	ON_BN_CLICKED(IDC_SELDELETE, OnSelDelete)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Variant(titleCC, "[3154] 상품선물옵션복수계좌설정");
	m_strRoot = Variant(homeCC);
	m_pNormalFont = GetAxFont("굴림체", 9, false, 0);
	m_pBoldFont = GetAxFont("굴림체", 9, false, 1);
	m_strPal = GetPalette();
	SetBitmap();
	int nLineTick[4];
	nLineTick[0] = GAP_SIDE;
	nLineTick[1] = GAP_TOP;
	nLineTick[2] = GAP_SIDE;
	nLineTick[3] = GAP_BLOCK;
	CRect rc;
	GetClientRect(rc);
	const int itop = 0;
	rc.bottom -= 17;
	m_panel = new CfxStatic;
	m_panel->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, rc, this, IDC_STATIC);
	m_panel->SetCornerRound(m_hBmpRoundPanel, 5, &nLineTick[0]);
	m_panel->SetBkColor(GetIndexColor(clBackIn));
	m_panel->SetBorderColor(GetIndexColor(clBack));
	
	
	CRect rcClient;
	CRect rcCtrl;
	GetClientRect(&rcClient);

	int nCenterLeft = 0;

	rcCtrl.SetRect(SZ_FORM_MARGIN, SZ_FORM_MARGIN, SZ_FORM_MARGIN + SZ_CTRL_ACCWIDTH, SZ_FORM_MARGIN + SZ_CTRL_HEIGHT);

	m_pAccTitle = new CfxStatic;
	m_pAccTitle->Create(_T("계좌리스트"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_ACCTITLE);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcClient.Height() - SZ_FORM_MARGIN - SZ_CTRL_HEIGHT - SZ_CTRL_VGAP - 22;

	m_pAccGrid = new CGridEx(this);
	m_pAccGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_ACCGRID);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;
	rcCtrl.top = 220;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	m_pLtoR = new CfxImgButton;
	m_pLtoR->Create("계좌추가\n계좌추가", rcCtrl, this, IDC_LTOR);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_RIGHTWIDTH;
	rcCtrl.top = SZ_FORM_MARGIN;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	nCenterLeft = rcCtrl.left;

	m_pGroupTitle = new CfxStatic;
	m_pGroupTitle->Create(_T("그룹리스트"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_GROUPTITLE);

	//asdf

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_GROUPHEIGHT;
	rcCtrl.left = nCenterLeft;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_RIGHTWIDTH;

	m_pGroupGrid = new CGridEx(this);
	m_pGroupGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_GROUPGRID);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3STATIC;

	m_pIDStatic = new CfxStatic;
	m_pIDStatic->Create(_T("그룹ID"), WS_CHILD | WS_VISIBLE | SS_LEFT | /*WS_CLIPCHILDREN |*/ WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_IDSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2EDIT;
	
	m_pID = new CfxEdit;
	m_pID->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, rcCtrl, this, IDC_ID);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pNameStatic = new CfxStatic;
	m_pNameStatic->Create(_T("설명"), WS_CHILD | WS_VISIBLE | SS_LEFT | /*WS_CLIPCHILDREN |*/ WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_NAMESTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5EDIT;

	m_pName = new CfxEdit;
	m_pName->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, rcCtrl, this, IDC_NAME);

	rcCtrl.left = rcClient.right - SZ_FORM_MARGIN - SZ_CTRL_5BUTTON - SZ_CTRL_3BUTTON - 2;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5BUTTON;

	m_pAddModify = new CfxImgButton;
	m_pAddModify->Create(_T("\n추가/수정"), rcCtrl, this, IDC_ADDMODIFY);

	rcCtrl.left = rcCtrl.right + 2;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3BUTTON;

	m_pGroupDelete = new CfxImgButton;
	m_pGroupDelete->Create(_T("\n삭제Χ"), rcCtrl, this, IDC_GROUPDELETE);
/*
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5BUTTON;

	m_pGroupUp = new CfxImgButton;
	m_pGroupUp->Create(_T("\n위로↑"), rcCtrl, this, IDC_GROUPUP);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5BUTTON;

	m_pGroupDown = new CfxImgButton;
	m_pGroupDown->Create(_T("\n아래로↓"), rcCtrl, this, IDC_GROUPDOWN);
*/
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP * 3;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	rcCtrl.left = nCenterLeft;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_RIGHTWIDTH;

	m_pSelTitle = new CfxStatic;
	m_pSelTitle->Create(_T("선택그룹 계좌리스트"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_SELTITLE);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_SELHEIGHT;
	rcCtrl.left = nCenterLeft;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_RIGHTWIDTH;

	m_pSelGrid = new CGridEx(this);
	m_pSelGrid->CreateGrid(WS_CHILD | WS_VISIBLE , rcCtrl, this, IDC_SELGRID);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	rcCtrl.left = nCenterLeft;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pAccStatic = new CfxStatic;
	m_pAccStatic->Create(_T("계좌"), WS_CHILD | WS_VISIBLE | SS_LEFT | /*WS_CLIPCHILDREN |*/ WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_ACCSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_8STATIC;

	m_pAccount = new CfxStatic;
	m_pAccount->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | /*WS_CLIPCHILDREN |*/ WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_ACCOUNT);
		
	rcCtrl.left = rcClient.right - SZ_FORM_MARGIN - SZ_CTRL_3BUTTON - SZ_CTRL_2BUTTON  - 2;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2BUTTON;

	m_pSelModify = new CfxImgButton;
	m_pSelModify->Create(_T("\n수정"), rcCtrl, this, IDC_SELMODIFY);

	rcCtrl.left = rcCtrl.right + 2;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3BUTTON;

	m_pSelDelete = new CfxImgButton;
	m_pSelDelete->Create(_T("\n삭제Χ"), rcCtrl, this, IDC_SELDELETE);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	rcCtrl.left = rcCtrl.right - SZ_CTRL_2BUTTON;

	m_pOk = new CfxImgButton;
	m_pOk->Create(_T("\n확인"), rcCtrl, this, IDOK);

/*
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5BUTTON;

	m_pSelUp = new CfxImgButton;
	m_pSelUp->Create(_T("\n위로↑"), rcCtrl, this, IDC_SELUP);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5BUTTON;

	m_pSelDown = new CfxImgButton;
	m_pSelDown->Create(_T("\n아래로↓"), rcCtrl, this, IDC_SELDOWN);
*/
	m_panel->SetWindowPos(&wndBottom, 0,0,0,0, SWP_NOSIZE);
	Invalidate();
	return 0;
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, GetIndexColor(CLR_FORM_BACK));
}

void CMapWnd::Init()
{
	m_pAccTitle->SetStyle(m_pAccTitle->GetStyle() | ssBORDER);
	m_pAccTitle->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pAccTitle->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pAccTitle->SetBorderColor(GetIndexColor(CLR_PANEL_BORD));//CLR_GUIDE_LINE));
	m_pAccTitle->SetFont(m_pNormalFont);
	m_pAccTitle->SetAlignment(alCENTER);

	m_pGroupTitle->SetStyle(m_pAccTitle->GetStyle() | ssBORDER);
	m_pGroupTitle->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pGroupTitle->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pGroupTitle->SetBorderColor(GetIndexColor(CLR_PANEL_BORD));
	m_pGroupTitle->SetFont(m_pNormalFont);
	m_pGroupTitle->SetAlignment(alCENTER);

	m_pSelTitle->SetStyle(m_pAccTitle->GetStyle() | ssBORDER);
	m_pSelTitle->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pSelTitle->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pSelTitle->SetBorderColor(GetIndexColor(CLR_PANEL_BORD));
	m_pSelTitle->SetFont(m_pNormalFont);
	m_pSelTitle->SetAlignment(alCENTER);

	/*
	m_pIDStatic->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pIDStatic->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pIDStatic->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pIDStatic->SetFont(m_pNormalFont);
	m_pIDStatic->SetAlignment(alCENTER);
	/*
	m_pNameStatic->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pNameStatic->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pNameStatic->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pNameStatic->SetFont(m_pNormalFont);
	m_pNameStatic->SetAlignment(alCENTER);
	
	m_pAccStatic->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pAccStatic->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pAccStatic->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pAccStatic->SetFont(m_pNormalFont);
	m_pAccStatic->SetAlignment(alCENTER);

	m_pAccount->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pAccount->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pAccount->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pAccount->SetFont(m_pNormalFont);
	m_pAccount->SetAlignment(alCENTER);
	*/
	CString strPath = "";
	strPath.Format("%s\\Image\\", m_strRoot);

	HBITMAP hbmp	= GetAxBitmap(strPath + "4BTN.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= GetAxBitmap(strPath + "4BTN_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = GetAxBitmap(strPath + "4BTN_en.bmp")->operator HBITMAP();

	m_pLtoR->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pLtoR->SetFont(m_pNormalFont);

	
	m_pIDStatic->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pIDStatic->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pIDStatic->SetFont(m_pNormalFont);

	m_pID->SetFont(m_pNormalFont);
	m_pID->ModifyAttribute(0, attrNUMERIC);
	m_pID->LimitText(3);

	m_pNameStatic->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pNameStatic->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pNameStatic->SetFont(m_pNormalFont);

	m_pName->SetFont(m_pNormalFont);

	m_pAccStatic->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pAccStatic->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pAccStatic->SetFont(m_pNormalFont);

	m_pAccount->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pAccount->SetBorderColor(GetIndexColor(CLR_PANEL_BORD));//CLR_GRID_LINE));
	m_pAccount->SetStyle(m_pAccount->GetStyle() | ssBORDER);
	m_pAccount->SetFont(m_pNormalFont);

	hbmp	= GetAxBitmap(strPath + "5btn.bmp")->operator HBITMAP();
	hbmp_dn	= GetAxBitmap(strPath + "5btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = GetAxBitmap(strPath + "5btn_en.bmp")->operator HBITMAP();

	m_pAddModify->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pAddModify->SetWindowText(_T("추가/수정"));
	m_pAddModify->SetFont(m_pNormalFont);

	hbmp	= GetAxBitmap(strPath + "3btn.bmp")->operator HBITMAP();
	hbmp_dn	= GetAxBitmap(strPath + "3btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = GetAxBitmap(strPath + "3btn_en.bmp")->operator HBITMAP();

	m_pGroupDelete->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pGroupDelete->SetWindowText(_T("삭제Χ"));
	m_pGroupDelete->SetFont(m_pNormalFont);

	m_pSelDelete->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pSelDelete->SetWindowText(_T("삭제Χ"));
	m_pSelDelete->SetFont(m_pNormalFont);

//	m_pGroupUp->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pGroupUp->SetWindowText(_T("위로↑"));
//	m_pGroupUp->SetFont(m_pNormalFont);

//	m_pGroupDown->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pGroupDown->SetWindowText(_T("아래로↓"));
//	m_pGroupDown->SetFont(m_pNormalFont);

//	m_pSelUp->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pSelUp->SetWindowText(_T("위로↑"));
//	m_pSelUp->SetFont(m_pNormalFont);

//	m_pSelDown->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pSelDown->SetWindowText(_T("아래로↓"));
//	m_pSelDown->SetFont(m_pNormalFont);

	
	hbmp	= GetAxBitmap(strPath + "2btn.bmp")->operator HBITMAP();
	hbmp_dn	= GetAxBitmap(strPath + "2btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = GetAxBitmap(strPath + "2btn_en.bmp")->operator HBITMAP();

	m_pSelModify->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pSelModify->SetWindowText(_T("수정"));
	m_pSelModify->SetFont(m_pNormalFont);

	m_pOk->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pOk->SetWindowText(_T("확인"));
	m_pOk->SetFont(m_pNormalFont);

	CUGCell cell;
	m_pLinePen = new CPen(PS_SOLID, 1, GetIndexColor(CLR_GRID_LINE));
	//m_pAccGrid->SetGridScroll(GVSC_VERT);
	//m_pAccGrid->SetGridFocusLine(FALSE, TRUE);
	
	m_pAccGrid->SetNumberCols(2);
	m_pAccGrid->SetNumberRows(0);

	m_pAccGrid->SetColWidth(0, SZ_COL_ACCOUNT);
	m_pAccGrid->SetColWidth(1, SZ_COL_ACCNAME);
	
	m_pAccGrid->GetCell(0, -1, &cell);
	cell.SetBackColor(GetIndexColor(CLR_GRID_HBACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_HTEXT));
	cell.SetText(_T("계좌번호"));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pAccGrid->SetCell(0, -1, &cell);

	m_pAccGrid->GetCell(1, -1, &cell);
	cell.SetBackColor(GetIndexColor(CLR_GRID_HBACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_HTEXT));
	cell.SetText(_T("계좌명"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pAccGrid->SetCell(1, -1, &cell);

	m_pAccGrid->GetColDefault(0, &cell);
	cell.SetHBackColor(GetIndexColor(CLR_GRID_SBACK));
	cell.SetHTextColor(GetIndexColor(CLR_GRID_STEXT));
	cell.SetBackColor(GetIndexColor(CLR_GRID_BACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_TEXT));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pAccGrid->SetColDefault(0, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pAccGrid->SetColDefault(1, &cell);

	

	m_pSelGrid->SetNumberCols(5);
	m_pSelGrid->SetNumberRows(0);

	m_pSelGrid->SetColWidth(0, SZ_COL_ACCOUNT - 20);
	m_pSelGrid->SetColWidth(1, SZ_COL_ACCNAME - 10);
	m_pSelGrid->SetColWidth(2, 0);
	m_pSelGrid->SetColWidth(3, 45);
	m_pSelGrid->SetColWidth(4, 45);

	m_pSelGrid->GetCell(0, -1, &cell);
	cell.SetBackColor(GetIndexColor(CLR_GRID_HBACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_HTEXT));
	cell.SetText(_T("계좌번호"));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pSelGrid->SetCell(0, -1, &cell);

	cell.SetText(_T("계좌명"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetCell(1, -1, &cell);
	
	cell.SetText(_T("종목"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetCell(2, -1, &cell);
	
	cell.SetText(_T("배수"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetCell(3, -1, &cell);

	cell.SetText(_T("한도"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetCell(4, -1, &cell);

	m_pSelGrid->GetColDefault(0, &cell);
	cell.SetHBackColor(GetIndexColor(CLR_GRID_SBACK));
	cell.SetHTextColor(GetIndexColor(CLR_GRID_STEXT));
	cell.SetBackColor(GetIndexColor(CLR_GRID_BACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_TEXT));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pSelGrid->SetColDefault(0, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetColDefault(1, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetColDefault(2, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetColDefault(3, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetColDefault(4, &cell);

	m_pGroupGrid->SetNumberCols(2);
	m_pGroupGrid->SetNumberRows(0);

	m_pGroupGrid->SetColWidth(0, SZ_COL_GROUPID);
	m_pGroupGrid->SetColWidth(1, SZ_COL_GROUPNAME);

	m_pGroupGrid->GetCell(0, -1, &cell);
	cell.SetBackColor(GetIndexColor(CLR_GRID_HBACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_HTEXT));
	cell.SetText(_T("그룹ID"));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pGroupGrid->SetCell(0, -1, &cell);

	cell.SetText(_T("그룹설명"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pGroupGrid->SetCell(1, -1, &cell);

	m_pGroupGrid->GetColDefault(0, &cell);
	cell.SetHBackColor(GetIndexColor(CLR_GRID_SBACK));
	cell.SetHTextColor(GetIndexColor(CLR_GRID_STEXT));
	cell.SetBackColor(GetIndexColor(CLR_GRID_BACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_TEXT));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pGroupGrid->SetColDefault(0, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pGroupGrid->SetColDefault(1, &cell);
}

COLORREF CMapWnd::GetIndexColor(UINT nIndex)
{
	if (!m_pWizard)
		return 0;

	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	const UINT nID = pWnd->GetDlgCtrlID();

	return hbr;
}

CString CMapWnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strRet(_T(""));

	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pData > 1)
		strRet = pData;

	return strRet;
}

CFont* CMapWnd::GetAxFont(CString strName, int nPt, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name = (LPSTR)(LPCTSTR)strName;
	fontR.point = nPt;
	fontR.italic = bItalic;
	fontR.bold = nBold;

	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMapWnd::SetAccountList()
{
	CString strData = Variant(accountCC);
	int nCurRow = 0;
	BOOL bUseAlias = FALSE;

	CString strUserPath;
	strUserPath.Format("%s\\user\\%s\\%s.ini", m_strRoot, Variant(nameCC), Variant(nameCC));

	if (GetPrivateProfileInt(_T("Account"), _T("UseAlias"), 0, (LPCTSTR)strUserPath) == 1)
		bUseAlias = TRUE;

	while (!strData.IsEmpty())
	{
		CString strAccInfo(_T(""));
		CString strAccount;
		CString strAccNameInfo;
		CString strAccName;
		CString strAccNick;
		
		strAccInfo = Parser(strData, _T("\n"));

		strAccount = Parser(strAccInfo, _T("\t"));

		CString strMid = strAccount.Mid(3, 2);

		if (strMid != _T("20") && strMid != _T("21"))
			continue;
/*
		if (strAccount.Mid(3, 2) == _T("20") || strAccount.Mid(3, 2) == _T("21") ||
			strAccount.Mid(3, 2) == _T("53") || strAccount.Mid(3, 2) == _T("00"))
			continue;
*/
		strAccount.Insert(3, _T("-"));
		strAccount.Insert(6, _T("-"));
		Parser(strAccInfo, _T("\t"));

		strAccNameInfo = Parser(strAccInfo, _T("\t"));

		strAccName = Parser(strAccNameInfo, _T("|"));
		strAccNick = Parser(strAccNameInfo, _T("|"));
		Parser(strAccNameInfo, _T("|"));

		if (m_pAccGrid->GetNumberRows() == nCurRow)
			m_pAccGrid->InsertRow(nCurRow);

		m_pAccGrid->QuickSetText(0, nCurRow, strAccount);
		
		if (bUseAlias)
		{
			if (strAccNick.IsEmpty())
				strAccNick = strAccName;

			m_pAccGrid->QuickSetText(1, nCurRow, strAccNick);
		}
		else
			m_pAccGrid->QuickSetText(1, nCurRow, strAccName);

		m_pAccGrid->QuickSetText(1, nCurRow, bUseAlias ? strAccNick : strAccName);

		nCurRow++;
	}
}

CString CMapWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CMapWnd::OnLtoR()
{
	const int nGroupRow = m_pGroupGrid->GetCurrentRow();
	
	if (nGroupRow < 0)
		return;

	CString strGroupID = m_pGroupGrid->QuickGetText(0, nGroupRow);

	_TRIMDATA(strGroupID);

	if (strGroupID.IsEmpty())
		return;

	const int nCurRow = m_pAccGrid->GetCurrentRow();

	CString strAccount = m_pAccGrid->QuickGetText(0, nCurRow);
	_TRIMDATA(strAccount);
	strAccount.Remove(_T('-'));

	if (strAccount.IsEmpty())
		return;
	
	QueryGroupAMD("2", strAccount, "", "", "100.00");

/*
	m_pSelGrid->InsertRow(m_pSelGrid->GetNumberRows());
	m_pSelGrid->QuickSetText(0, nTargetRow, strAccount);
	m_pSelGrid->QuickSetText(1, nTargetRow, strAccName);
	m_pSelGrid->QuickSetText(2, nTargetRow, _T("1.00"));
	m_pSelGrid->RedrawRow(nTargetRow);
	m_pSelGrid->GotoRow(nTargetRow);

	QueryGroupAMD(2);
*/
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INIT:
		QueryGroupList();
		SetAccountList();
		break;

	case DLL_OUB:
		{
			const int key = HIBYTE(LOWORD(wParam));
			const int len = HIWORD(wParam);
			struct _ledgerH ledger;
			CString strLedger(_T(""));
			CString strErrCode(_T(""));
			CString strErrText(_T(""));
			CString strMsg(_T(""));
			int nNextKey = 0;

			CopyMemory(&ledger, (void*)lParam, L_ledgerH);
			strLedger = CString((char*)&ledger, L_ledgerH);
			strErrCode = CString((char*)&ledger.emsg, 4);
			strErrText = CString((char*)&ledger.emsg, 98);
			nNextKey = ledger.next[0];
			
			lParam += L_ledgerH;
			strMsg = "ERR\t" + strErrText;
			if (strErrCode.GetAt(0) != _T('0'))	// 오류
			{
				strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));
				return 0;
			}
			
			switch (key)
			{
			case KEY_GROUPLIST:
				ParsingGroupList((char*)lParam, HIWORD(wParam), strMsg);
				break;

			case KEY_GROUPACCOUNT:
				//AfxMessageBox((char*)lParam);
				ParsingGroupAccount((char*)lParam, HIWORD(wParam), strMsg);
				break;
			}
		}
		break;


	case DLL_TRIGGER:
		ParsingTrigger((char*)lParam);
		break;
	case DLL_SETPAL:
		//m_clrBackIn = getColor(clBackIn);
		//m_clrBack = getColor(clBack);
		//m_clrContentsBackIn = getColor(clcontentsBack);
		m_strPal = GetPalette();
		SetBitmap();
		if (m_panel)
		{
			int nLineTick[4];
			nLineTick[0] = GAP_SIDE;	nLineTick[1] = GAP_TOP;
			nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BLOCK;
			m_panel->SetCornerRound(m_hBmpRoundPanel, 5, &nLineTick[0]);
			m_panel->SetBkColor(GetIndexColor(clBackIn));
			m_panel->SetBorderColor(GetIndexColor(clBack));
			m_panel->SetWindowPos(&wndBottom, 0,0,0,0, SWP_NOSIZE);
		}

		Invalidate();
		
		break;
	}

	return 0;
}

BOOL CMapWnd::SendTR(CString strTR, CString strData, int nKey, int nStat)
{

	const int iLength = strData.GetAllocLength();
	std::unique_ptr<char[]> pcDataBuffer = std::make_unique<char[]>(L_userTH + iLength);
	ZeroMemory(pcDataBuffer.get(), sizeof(pcDataBuffer));
	struct	_userTH* puserTH;
	puserTH = (struct _userTH*)pcDataBuffer.get();

	CopyMemory(puserTH->trc, strTR.GetString(), sizeof(puserTH->trc));
	puserTH->key = nKey;
	puserTH->stat = nStat;

	CopyMemory(&pcDataBuffer.get()[L_userTH], strData.GetString(), iLength);

	const LRESULT lResult = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, iLength), (LPARAM)pcDataBuffer.get());

	pcDataBuffer = nullptr;
	return lResult;




	//CString strTRData = "";
	//
	//struct _userTH user;
	//strncpy(&user.trc[0], strTR, 8);
	//user.key  = nKey;
	//user.stat = nStat;
	//
	//strTRData = CString((char*)&user, sizeof(_userTH));
	//strTRData += strData;
	//
	//return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), 
	//	(LPARAM)(const char*)strTRData); 
}

void CMapWnd::QueryGroupList()
{
	
	ClearGroupGrid();
	ClearSelGrid();
	
	CString strUser(_T(""));

	strUser = Variant(userCC);
	_TRIMDATA(strUser);

	CString strPass = Variant(passCCx);//(char*)AfxGetMainWnd()->SendMessage(WM_USER, 0x01, 0L);
	_TRIMDATA(strPass);

	struct fofqry_mid mid;
	FillMemory(&mid, L_fofqry_mid, ' ');

	CopyMemory(&mid.In, _T("00001"), sizeof(mid.In));
	
	if ((long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L) == 0)
		mid.zUserTp[0] = _T('1');
	else
		mid.zUserTp[0] = _T('2');

	CopyMemory(mid.zFundOrdUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(mid.zEmpPwd, (LPCTSTR)strPass, strPass.GetLength());
	
	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.svcd, _T("SONBQ743"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	CString strSendData = CString((char*)&ledger, L_ledgerH);


	strSendData += CString((char*)&mid, L_fofqry_mid);
	SendTR("PIBOPBXQ", strSendData, KEY_GROUPLIST, 0);
}

void CMapWnd::ParsingGroupList(CString strData, int nLen, CString strMsg)
{
	int nRec = atoi((LPCTSTR)strData.Mid(L_gqry_mid, 5));

	strData.Delete(0, L_fofqry_mid + 5);

	int nCount = 0;

	for (int i = 0; i < nRec; i++)
	{
		struct fofqry_rec rec;

		CopyMemory(&rec, (LPCTSTR)strData.Left(L_fofqry_rec), L_fofqry_rec);
		CString strGroupID(rec.zFundId, sizeof(rec.zFundId));
		CString strGroupName(rec.zFundDescr40, sizeof(rec.zFundDescr40));

		if (rec.zFundPtnTp[0] == _T('3') && (strGroupID.Mid(0,1) == "F"))
		{
			_TRIMDATA(strGroupID);
			_TRIMDATA(strGroupName);
			strGroupID.Delete(0,1);
			//AfxMessageBox(strGroupID);
			//AfxMessageBox(strGroupID.Mid(0,1));
			//if (strGroupID.Mid(0,1) != "F") continue;
			//AfxMessageBox(strGroupID);
		

			m_pGroupGrid->InsertRow(i - nCount);
			m_pGroupGrid->QuickSetText(0, i - nCount, (LPCTSTR)strGroupID);
			m_pGroupGrid->QuickSetText(1, i - nCount, (LPCTSTR)strGroupName);

			m_pGroupGrid->RedrawRow(i - nCount);
		}
		else
			nCount++;

		strData.Delete(0, L_fofqry_rec);
	}

	if (nRec > 0)
		OnRowChange(-1, 0, m_pGroupGrid);
}

void CMapWnd::QueryGroupAMD(CString strMode, CString strAccount, CString strCode, CString strRate, CString strRate2)
{

	CString strUser(_T(""));
	CString strPass(_T(""));
	CString strGroupID(_T(""));
	CString strGroupName(_T(""));
	CString strPassword(_T(""));

	strUser = Variant(userCC);
	_TRIMDATA(strUser);

	strPass = Variant(passCCx);//(char*)AfxGetMainWnd()->SendMessage(WM_USER, 0x01, 0L);
	_TRIMDATA(strPass);

	const int nRow = m_pGroupGrid->GetCurrentRow();

	strGroupID = m_pGroupGrid->QuickGetText(0, nRow);
	
	
	strGroupName = m_pGroupGrid->QuickGetText(1, nRow);

	if (strAccount.IsEmpty())
		return;

	strAccount.Remove(_T('-'));

	strPassword = Variant(passCC, strAccount);
	// MessageBox(LPCTSTR(strPassword), "", MB_OK);
	/*
	if (strPassword.IsEmpty() || strPassword.GetLength() != 4)
	{
		MessageBox("계좌비밀번호 저장후 사용하십시오");
		return;
	}
	*/
	if (strAccount == "계좌번호")
		strAccount = " ";
	if (strCode == "종목")
		strCode = " ";
	if (strRate == "배수")
		strRate = "";
		

	if (strRate.IsEmpty())
		strRate = "1.00";
	if (strRate2.IsEmpty())
		strRate2 = "1.00";
	
	strGroupID = "F"+strGroupID;
	pre_id = strGroupID;
	CString strData(_T(""));
	strData.Format("IB315377 /p5 /S /t /d zTrxPtnTp\t%s\nzFundOrdUserId\t%s\nzEmpPwd\t%s\nzFundId\t%s\nzFundDescr40\t%s\nAN20\t%s\ncod2\t%s\nrtga_o\t%s\nEDIT6\t%s\n",
					strMode, strUser, strPass, strGroupID, strGroupName, strAccount, strCode, strRate, strRate2);
	
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 2), (LPARAM)(LPCTSTR)strData);
	/*
	if (strMode == "6")
		QueryGroupList();
	*/
	//QueryGroupAccount();
//	strData = "TT900999 /p5 /S /t";
//	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 2), (LPARAM)(LPCTSTR)strData);
}

void CMapWnd::QueryGroupAccount()
{
	ClearSelGrid();

	CString strUser(_T(""));
	CString strPass(_T(""));
	CString strGroupID(_T(""));
	CString strGroupName(_T(""));

	strUser = Variant(userCC);
	_TRIMDATA(strUser);

	strPass = Variant(passCCx);
	_TRIMDATA(strPass);
	
	const int nRow = m_pGroupGrid->GetCurrentRow();
	
	
	if (nRow < 0)
		return;
	strGroupID = m_pGroupGrid->QuickGetText(0, nRow);
	//if (pre_id != "") strGroupID = pre_id;
	_TRIMDATA(strGroupID);
	strGroupID = "F" + strGroupID;
	
	if (strGroupID.IsEmpty())
		return;

	struct fofaqry_mid mid;

	FillMemory(&mid, L_fofaqry_mid, ' ');
	CopyMemory(&mid.In, _T("00001"), sizeof(mid.In));
	
	if ((long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L) == 0)
		mid.zUserTp[0] = _T('1');
	else
		mid.zUserTp[0] = _T('2');

	CopyMemory(&mid.zFundOrdUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&mid.zEmpPwd, (LPCTSTR)strPass, strPass.GetLength());
	CopyMemory(&mid.zFundId, (LPCTSTR)strGroupID, strGroupID.GetLength());
	mid.zFundPtnTp[0] = _T('3');

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.svcd, _T("SONBQ744"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '1';

	CString strSendData = CString((char*)&ledger, L_ledgerH);


	strSendData += CString((char*)&mid, L_fofaqry_mid);
	
	SendTR("PIBOPBXQ", strSendData, KEY_GROUPACCOUNT, 0);
}

void CMapWnd::ParsingGroupAccount(CString strData, int nLen, CString strMsg)
{
	int nRec = atoi((LPCTSTR)strData.Mid(L_fofaqry_mid, 5));
	strData.Delete(0, L_fofaqry_mid + 5);

	for (int i = 0; i < nRec; i++)
	{
		struct fofaqry_rec rec;

		CopyMemory(&rec, (LPCTSTR)strData.Left(L_fofaqry_rec), L_fofaqry_rec);

		CString strAccount(rec.zAcntNo, sizeof(rec.zAcntNo));
		CString strAccName(rec.zAcntNm, sizeof(rec.zAcntNm));
		CString strCode(rec.zFnoIsuNo, sizeof(rec.zFnoIsuNo));
		CString strRate(rec.dRatVal, sizeof(rec.dRatVal));
		CString strWght(rec.dWghtval, sizeof(rec.dWghtval));
		
		_TRIMDATA(strAccount);
		_TRIMDATA(strAccName);
		_TRIMDATA(strCode);
		_TRIMDATA(strRate);
		_TRIMDATA(strWght);

		strAccount.Insert(3, _T('-'));
		strAccount.Insert(6, _T('-'));

		const double dRate = atof((LPCTSTR)strRate);
		strRate.Format("%.2f", dRate);

		const double dWght = atof((LPCTSTR)strWght);
		strWght.Format("%.2f", dWght);

		m_pSelGrid->InsertRow(i);
		m_pSelGrid->QuickSetText(0, i, (LPCTSTR)strAccount);
		m_pSelGrid->QuickSetText(1, i, (LPCTSTR)strAccName);
		m_pSelGrid->QuickSetText(2, i, (LPCTSTR)strCode);
		m_pSelGrid->QuickSetText(3, i, (LPCTSTR)strRate);
		m_pSelGrid->QuickSetText(4, i, (LPCTSTR)strWght);
		
		m_pSelGrid->RedrawRow(i);

		strData.Delete(0, L_fofaqry_rec);
	}

	if (nRec > 0)
		OnRowChange(-1, 0, m_pSelGrid);
}

/* 내부적으로 GroupId별로 등록계좌수를 리턴한다. */
UINT CMapWnd::GetGroupAccounts(CString GroupId)
{
	GroupVectorIter st = m_grpVector.begin();
	const GroupVectorIter ed = m_grpVector.end();
	while(st!=ed)
	{
		if (st->GroupId==GroupId) return st->Accounts;
		++st;
	}
	return 0;
}

/* 내부적으로 GroupId별로 등록계좌수를 세팅한다.*/
void CMapWnd::SetGroupAccounts(CString GroupId, UINT cnt)
{
	GroupVectorIter st = m_grpVector.begin();
	const GroupVectorIter ed = m_grpVector.end();
	while(st!=ed)
	{
		if (st->GroupId==GroupId) {
			st->Accounts = cnt;
			return;
		}
		++st;
	}

	GROUP_INFO gi;
	gi.GroupId = GroupId;
	gi.Accounts = cnt;
	m_grpVector.push_back(gi);
}

void CMapWnd::OnAddModify()
{
	CString strID(_T(""));
	CString strName(_T(""));

	m_pID->GetWindowText(strID);
	m_pName->GetWindowText(strName);
	//strID = "F"+strID;
	
	if (strID.IsEmpty() || strName.IsEmpty())
		return;

	if (FindGroup(strID) == -1)
	{
		/* 그룹 추가시 해당 그룹의 계좌수를 0으로 세팅 */

		SetGroupAccounts( strID, 0 );

		const int nCurRow = m_pGroupGrid->GetNumberRows();
		m_pGroupGrid->InsertRow(nCurRow);
		m_pGroupGrid->GotoRow(nCurRow);

		m_pGroupGrid->QuickSetText(0, nCurRow, (LPCTSTR)strID);
		m_pGroupGrid->QuickSetText(1, nCurRow, (LPCTSTR)strName);

		m_pGroupGrid->RedrawRow(nCurRow);

		ClearSelGrid();
	}
	else
	{	
		const int nRow = m_pSelGrid->GetNumberRows();
		if (nRow > 0)
		{
			MessageBox("그룹에 계좌가 존재하는 경우\n그룹명의 수정 불가합니다.", "IBK투자증권");
			return;
		}
		if (m_pSelGrid->GetNumberRows() > 0)
		{
			const int nCurRow = m_pSelGrid->GetCurrentRow();
			CString strAccount = m_pSelGrid->QuickGetText(0, nCurRow);
			CString strCode = m_pSelGrid->QuickGetText(2, nCurRow);
			CString strRate = m_pSelGrid->QuickGetText(3, nCurRow);
			CString strRate2 = m_pSelGrid->QuickGetText(4, nCurRow);
			_TRIMDATA(strAccount);
			_TRIMDATA(strCode);
			_TRIMDATA(strRate);

			QueryGroupAMD("7", strAccount, strCode, strRate, strRate2);	// 수정
		}
		else
		{
			const int nFindRow = FindGroup(strID);
			m_pGroupGrid->QuickSetText(1, nFindRow, (LPCTSTR)strName);
			m_pGroupGrid->RedrawCell(1, nFindRow);
		}
	}
}

void CMapWnd::OnRowChange(int oldRow, int newRow, CWnd* pWnd)
{
	if (pWnd == m_pGroupGrid)
	{
		CString strGroupID = m_pGroupGrid->QuickGetText(0, newRow);
		CString strGroupName = m_pGroupGrid->QuickGetText(1, newRow);
		if (!strGroupID.IsEmpty())
			m_pID->SetWindowText(strGroupID);

		if (!strGroupName.IsEmpty())
			m_pName->SetWindowText(strGroupName);
		
		QueryGroupAccount();

	}
	else if (pWnd == m_pSelGrid)
	{
		CString strAccount = m_pSelGrid->QuickGetText(0, newRow);
		m_pAccount->SetText(strAccount);
	}
//	else if (pWnd == m_pAccGrid)
//		;
}

int CMapWnd::FindGroup(CString strID)
{
	const int nCount = m_pGroupGrid->GetNumberRows();

	for (int i = 0; i < nCount; i++)
	{
		if (strID == m_pGroupGrid->QuickGetText(0, i))
			return i;
	}

	return -1;
}

void CMapWnd::OnGroupDelete()
{
	const int nCurRow = m_pSelGrid->GetCurrentRow();
	CString strAccount = m_pSelGrid->QuickGetText(0, nCurRow);
	CString strCode = m_pSelGrid->QuickGetText(2, nCurRow);
	CString strRate = m_pSelGrid->QuickGetText(3, nCurRow);
	_TRIMDATA(strAccount);
	_TRIMDATA(strCode);
	_TRIMDATA(strRate);

	QueryGroupAMD("6", strAccount, strCode, strRate);
}

void CMapWnd::ClearSelGrid()
{
	while (m_pSelGrid->GetNumberRows() != 0)
		m_pSelGrid->DeleteRow(0);
}



void CMapWnd::OnSelModify()
{
	const int nGroupRow = m_pGroupGrid->GetCurrentRow();
	
	if (nGroupRow < 0)
		return;

	CString strGroupID = m_pGroupGrid->QuickGetText(0, nGroupRow);

	_TRIMDATA(strGroupID);
	strGroupID = "F"+strGroupID;

	if (strGroupID.IsEmpty())
		return;

	const int nCurRow = m_pSelGrid->GetCurrentRow();
	CString strAccount = m_pSelGrid->QuickGetText(0, nCurRow);
	CString strCode = m_pSelGrid->QuickGetText(2, nCurRow);
	CString strRate = m_pSelGrid->QuickGetText(3, nCurRow);
	CString strRate2 = m_pSelGrid->QuickGetText(4, nCurRow);
	_TRIMDATA(strAccount);
	_TRIMDATA(strCode);
	_TRIMDATA(strRate);

	QueryGroupAMD("3", strAccount, strCode, strRate, strRate2);
}

void CMapWnd::OnSelDelete()
{
	if (m_pSelGrid->GetCurrentRow() < 0)
		return;

	const int nCurRow = m_pSelGrid->GetCurrentRow();
	CString strAccount = m_pSelGrid->QuickGetText(0, nCurRow);
	CString strCode = m_pSelGrid->QuickGetText(2, nCurRow);
	CString strRate = m_pSelGrid->QuickGetText(3, nCurRow);
	_TRIMDATA(strAccount);
	_TRIMDATA(strCode);
	_TRIMDATA(strRate);

	QueryGroupAMD("4", strAccount, strCode, strRate);
}

void CMapWnd::OnDestroy() 
{
	/* [확인]버튼을 누르지 않고 타이틀바의 X버튼을 바로 클릭할때 [확인]버튼을 누른것과 같은 효과를 내게 한다. 09/02/09 by lkm */
	OnOk();

	CString strGroup(_T("OnGroupAcc\t"));
	
	const int nRow = m_pGroupGrid->GetNumberRows();

	for (int i = 0; i < nRow; i++)
	{
		strGroup += m_pGroupGrid->QuickGetText(0, i);
		strGroup += _T("\t");
		strGroup += m_pGroupGrid->QuickGetText(1, i);
		strGroup += _T("\n");
	}

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)strGroup);

	CWnd::OnDestroy();
}


void CMapWnd::OnOk()
{
	/* 신규 추가한 그룹중 계좌를 등록하지 않은 그룹들의 삭제를 안내한다. 09/02/09 by lkm */
	std::vector<CString> rmGroups;
	GroupVectorIter st = m_grpVector.begin();
	const GroupVectorIter ed = m_grpVector.end();
	for(;st!=ed;++st)
	{
		if ( (FindGroup(st->GroupId)>=0) && (GetGroupAccounts(st->GroupId)<1) )
		{
			rmGroups.push_back(st->GroupId);
		}
	}
	
	if (!rmGroups.empty())
	{
		CString msg = "계좌를 등록하지 않은 그룹은 삭제됩니다.\n\n";
		for(size_t n=0;n<rmGroups.size();++n)
			msg += "[" + rmGroups[n] + "], ";
		msg = msg.Left( msg.GetLength()-2 );
		msg += "\n\n진행하시겠습니까?";

		const int ret = MessageBox(msg, "", MB_YESNO);
		if (ret==IDNO) return;
		
		/* 추가된 그룹중 계좌를 추가하지 않은 그룹의 경우 그리드에서 삭제함 -> OnDestory에서 그리드의 내용을 읽어감.*/
		for(size_t n=0;n<rmGroups.size();++n)
		{
			m_pGroupGrid->DeleteRow(FindGroup(rmGroups[n]));
		}
	}


	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(closeDLL, 0), 0L);
}



void CMapWnd::ParsingTrigger(CString strTrigger)
{
	
	if (strTrigger.IsEmpty())
		return;

	const int nPos = -1;
	
	CString strVal = Parser(strTrigger);

	if (strVal != "AddAccount")
		return;
	
	CString strMode = Parser(strTrigger);
	

	strVal = Parser(strTrigger);
	CString strGuide = Parser(strTrigger);
//	SetGuide(strVal);

	if (strVal != "0")
		return;

	if (strMode == "2")
	{
		
		CString groupId = m_pGroupGrid->QuickGetText(0, m_pGroupGrid->GetCurrentRow());
		
		/* 계좌 추가시 그룹에 계좌수를 +1한다. */
		SetGroupAccounts(groupId, GetGroupAccounts(groupId)+1);

		CString strAccount = Parser(strTrigger);
		CString strAccName = Parser(strTrigger);
		CString strRate = Parser(strTrigger);
		CString strFOCode = Parser(strTrigger);
		
		Parser(strTrigger);
		Parser(strTrigger);
		//Parser(strTrigger);
		
		CString strRate2 = Parser(strTrigger);
		//AfxMessageBox(strRate2);

		_TRIMDATA(strAccount);
		_TRIMDATA(strAccName);
		_TRIMDATA(strRate);
		_TRIMDATA(strFOCode);
		_TRIMDATA(strRate2);

		strAccount.Insert(3, _T('-'));
		strAccount.Insert(6, _T('-'));

		const double dRate = atof((LPCTSTR)strRate);
		strRate.Format("%.2f", dRate);
		const double dRate2 = atof((LPCTSTR)strRate2);
		strRate2.Format("%.2f", dRate2);
		const int nRows = m_pSelGrid->GetNumberRows();

		m_pSelGrid->InsertRow(nRows);
		m_pSelGrid->QuickSetText(0, nRows, (LPCTSTR)strAccount);
		m_pSelGrid->QuickSetText(1, nRows, (LPCTSTR)strAccName);
		m_pSelGrid->QuickSetText(2, nRows, (LPCTSTR)strFOCode);
		m_pSelGrid->QuickSetText(3, nRows, (LPCTSTR)strRate);
		m_pSelGrid->QuickSetText(4, nRows, (LPCTSTR)strRate2);
		
		m_pSelGrid->RedrawRow(nRows);
	}
	else
		QueryGroupList();
}

void CMapWnd::ClearGroupGrid()
{
	while (m_pGroupGrid->GetNumberRows() != 0)
		m_pGroupGrid->DeleteRow(0);
}

void CMapWnd::SetBitmap()
{
	CStringArray strImageArr;
	CStringArray strImageArr2;
	CString sImageDir;
	//panel
	strImageArr.Add("axpanel" + m_strPal + "1_lt.bmp");
	strImageArr.Add("axpanel" + m_strPal + "1_rt.bmp");
	strImageArr.Add("axpanel" + m_strPal + "1_lb.bmp");
	strImageArr.Add("axpanel" + m_strPal + "1_rb.bmp");

	//contents
	strImageArr2.Add("axpanel" + m_strPal + "0_lt.bmp");
	strImageArr2.Add("axpanel" + m_strPal + "0_rt.bmp");
	strImageArr2.Add("axpanel" + m_strPal + "0_lb.bmp");
	strImageArr2.Add("axpanel" + m_strPal + "0_rb.bmp");

	for(int i = 0; i < 4; i++)
	{	
		m_hBmpRoundContents[i] = getBitmap(strImageArr.GetAt(i));
		m_hBmpRoundPanel[i] = getBitmap(strImageArr2.GetAt(i));
	}
}

HBITMAP CMapWnd::getBitmap(CString bmps)
{
	CString	path;

	path.Format("%s\\%s\\%s", m_strRoot, IMAGEDIR, bmps);
	return ((CBitmap *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}

CString CMapWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_strRoot, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CMapWnd::OnDClicked(int col, int row, RECT *rect, POINT *point, CWnd* pWnd)
{
	if (pWnd == m_pAccGrid)
	{
		OnLtoR();
	}else if (pWnd == m_pSelGrid)
	{
		if (m_pSelGrid->GetNumberRows()>0)
		OnSelModify();
	}
	//m_pMapWnd->OnDClicked(col, row, rect, point);
}