// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300900.h"
#include "ControlWnd.h"
#include "MarketConfirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		WM_CODE		9888

CControlWnd::CControlWnd(CWnd *pParent, CWnd* pMapWnd, AccountMap *pAccount, SiseData *pSiseData)
: CAxisExt(pParent), m_pAccount(pAccount), m_pSiseData(pSiseData)
{
	CString path;
	path.Format("%s\\%s\\", Variant(homeCC,""), "image");

	m_pMapWnd = pMapWnd;

	m_hBtn2[0] = GetAxBitmap(path + "2btn.bmp");
	m_hBtn2[1] = GetAxBitmap(path + "2btn_dn.bmp");
	m_hBtn2[2] = GetAxBitmap(path + "2btn_en.bmp");

	m_hBtn3[0] = GetAxBitmap(path + "3btn.bmp");
	m_hBtn3[1] = GetAxBitmap(path + "3btn_dn.bmp");
	m_hBtn3[2] = GetAxBitmap(path + "3btn_en.bmp");

	m_hBtnLink[0] = GetAxBitmap(path + "LINK.bmp");
	m_hBtnLink[1] = GetAxBitmap(path + "LINK_dn.bmp");
	m_hBtnLink[2] = GetAxBitmap(path + "LINK_en.bmp");

	m_pJcntGrid = m_pOptGrid = m_pJvolGrid = m_pChegGrid = NULL;
	m_pBtnCheg = m_pBtnChart = m_pBtnJango = m_pBtnFold = m_pBtnJvol = m_pBtnChegLink = NULL;
	m_pStJumun = m_pStJcnt = m_pStRsvd = m_pStMstr = m_pStJvol = NULL;
	m_pSpJcnt = m_pSpMulti = m_pSpRsvd = NULL;
	m_pEdJprc = NULL;
	m_pTab = NULL;

	m_dtOptionIdx = -1;

	nSizeMode = SIZE_GENERAL;

	m_profile.Format(CONFIG_INI, Variant(homeCC,""), Variant(nameCC,""));

	m_bChegUpdate = FALSE;

	m_bMarketConfirm = TRUE;

	m_hBrush1 = (HBRUSH)CreateSolidBrush(m_crBodyBk);

	LoadMaster();
}

CControlWnd::~CControlWnd()
{
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_AUTO, OnChkAuto)
	ON_BN_CLICKED(IDC_CHK_LOSS, OnChkLoss)
	ON_BN_CLICKED(IDC_BTN_SIZE, OnBtnSize)
	ON_BN_CLICKED(IDC_BTN_CHEG , OnBtnCheg )
	ON_BN_CLICKED(IDC_BTN_CHART, OnBtnChart)
	ON_BN_CLICKED(IDC_BTN_JANGO, OnBtnJango)
	ON_BN_CLICKED(IDC_CHK_MULTI, OnChkMulti)
	ON_BN_CLICKED(IDC_BTN_FOLD,  OnBtnFold)
	ON_BN_CLICKED(IDC_CHK_OCLICK, OnChkOneClick)
	ON_BN_CLICKED(IDC_BTN_JVOL, OnBtnJvol)
	ON_BN_CLICKED(IDC_BTN_CHEGLINK, OnChegLink)
	ON_BN_CLICKED(IDC_CHK_MARKET, OnChkMarket)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_FUT1, IDC_BTN_FUT4, OnFutBtnClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_OPT1, IDC_BTN_OPT4, OnPutBtnClick)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CTL_TAB1, OnTabChange)
	ON_MESSAGE(WM_APP_REFRESH, OnRefresh)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

void CControlWnd::OnDestroy() 
{
	if (m_pBtnChart) { m_pBtnChart->DestroyWindow(); delete m_pBtnChart; }
	if (m_pBtnCheg)  { m_pBtnCheg->DestroyWindow();  delete m_pBtnCheg;  }
	if (m_pBtnJango) { m_pBtnJango->DestroyWindow(); delete m_pBtnJango; }
	if (m_pStJumun)  { m_pStJumun->DestroyWindow();  delete m_pStJumun;  }
	if (m_pStJvol)   { m_pStJvol->DestroyWindow();   delete m_pStJvol;   }
	if (m_pEdJprc)   { m_pEdJprc->DestroyWindow();   delete m_pEdJprc;   }
	if (m_pJvolGrid) { m_pJvolGrid->DestroyWindow(); delete m_pJvolGrid; }
	if (m_pBtnJvol)  { m_pBtnJvol->DestroyWindow();  delete m_pBtnJvol;  }
	if (m_pBtnChegLink) { m_pBtnChegLink->DestroyWindow(); delete m_pBtnChegLink; }

	for(int n=0; n<4; ++n)
	{
		if (m_pBtnFuture[n]) { m_pBtnFuture[n]->DestroyWindow(); delete m_pBtnFuture[n]; }
		if (m_pBtnOption[n]) { m_pBtnOption[n]->DestroyWindow(); delete m_pBtnOption[n]; }
		if (m_pStFuture[n]) { m_pStFuture[n]->DestroyWindow(); delete m_pStFuture[n]; }
	}
	if (m_pStJcnt)  { m_pStJcnt->DestroyWindow();  delete m_pStJcnt;  }
	if (m_pStRsvd)  { m_pStRsvd->DestroyWindow();  delete m_pStRsvd;  }
	if (m_pStMstr)  { m_pStMstr->DestroyWindow();  delete m_pStMstr;  }
	if (m_pSpJcnt)  { m_pSpJcnt->DestroyWindow();  delete m_pSpJcnt;  }
	if (m_pSpMulti) { m_pSpMulti->DestroyWindow(); delete m_pSpMulti; }
	if (m_pSpRsvd)  { m_pSpRsvd->DestroyWindow();  delete m_pSpRsvd;  }
	
	if (m_pJcntGrid) { m_pJcntGrid->DestroyWindow(); delete m_pJcntGrid; }
	if (m_pOptGrid)  { m_pOptGrid->DestroyWindow();  delete m_pOptGrid;  }
	if (m_pChegGrid) { m_pChegGrid->DestroyWindow(); delete m_pChegGrid; }

	if (m_pTab)	     { m_pTab->DestroyWindow(); delete m_pTab; }

// 	if (m_pTooltip)
// 	{
// 		m_pTooltip->Activate(TRUE);
// 		delete m_pTooltip;
// 		m_pTooltip = NULL;
// 	}

	CWnd::OnDestroy();	
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if(nSizeMode == SIZE_GENERAL)
	{
		CalcSize();
	}
	else
	{
		CalcSizeJango();
	}
}

void CControlWnd::CalcSizeJango()
{
	int n;
	CRect rc;
	GetClientRect(rc);
	int width = rc.Width(), height = rc.Height();
	if (width<=0 || height<=0) return;

	int pos_x, pos_y, col, dx;
	int btn_w = 40;
	int btn_h = 18, tab_h = 24;

	// Line 1
	pos_x = 0;
	pos_y = 0;
	m_pBtnCheg->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x += btn_w + PADDING_VALUE/2;
	m_pBtnChart->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x += btn_w + PADDING_VALUE/2;
	m_pBtnJango->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x = width - 20;
	m_pBtnFold->MoveWindow(pos_x, pos_y, 20, btn_h, TRUE);
	
	// Line 2	-- �ֹ����� ��
// 	pos_x = 0;
// 	pos_y += btn_h + PADDING_VALUE;

	// Line 8.0 -- ��
	pos_x = 0;
	pos_y += tab_h;
	m_pTab->MoveWindow(pos_x, pos_y, width, tab_h, FALSE);
	m_pBtnChegLink->MoveWindow(width-41, pos_y, 20, 20, FALSE);

	m_pBtnSize->MoveWindow(width - 20, pos_y, 20, 20, TRUE);

	// Line 8	-- ���� - �ܰ�/��ü�� ��
// 	pos_x = 0;
// 	pos_y += 20;
// 	m_pStMstr->MoveWindow(pos_x, pos_y, width, btn_h, FALSE);

	// ü��׸���
	{
		int pos_x2 = 0;
		int pos_y2 = pos_y + tab_h + PADDING_VALUE/2;
		m_pChegGrid->MoveWindow(pos_x2, pos_y2, width, height-pos_y2);
	}

	// Line 9	-- �������� ��ư
	pos_x = 0;
	pos_y += tab_h + PADDING_VALUE/2;
	dx = (width-9)/4;
	for(n=0; n<4; ++n, pos_x+=(dx+4))
	{
		m_pBtnFuture[n]->MoveWindow(pos_x, pos_y, dx, btn_h);
		m_pStFuture[n]->MoveWindow(pos_x, pos_y+btn_h+PADDING_VALUE/3, dx, btn_h);
	}

	// Line 10	-- �ɼ����� ��ư
	pos_x = 0;
	pos_y += btn_h*2 + PADDING_VALUE;
	dx = (width-9)/4;
	for(n=0; n<4; ++n, pos_x+=(dx+4))
		m_pBtnOption[n]->MoveWindow(pos_x, pos_y, dx, btn_h);

	// Line 11	-- �ɼǱ׸���
	pos_x = 0;
	pos_y += btn_h + PADDING_VALUE/2;
	m_pOptGrid->MoveWindow(pos_x, pos_y, width, height-pos_y);
	col = m_pOptGrid->GetColumnCount();
	dx = (width-SCROLLBAR_WIDTH)/col;
	m_pOptGrid->SetColumnWidth(0, dx);
	m_pOptGrid->SetColumnWidth(1, width-SCROLLBAR_WIDTH-(dx*2) );
	m_pOptGrid->SetColumnWidth(2, dx);
	
	MakeBackground();
}

void CControlWnd::CalcSize()
{
	int n;
	CRect rc;
	GetClientRect(rc);
	int width = rc.Width(), height = rc.Height();
	if (width<=0 || height<=0) return;

	int pos_x, pos_y, row, col, dx;
	int btn_w = 40;
	int btn_h = 18, tab_h = 24;

	// Line 1
	pos_x = 0;
	pos_y = 0;
	m_pBtnCheg->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x += btn_w + PADDING_VALUE/2;
	m_pBtnChart->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x += btn_w + PADDING_VALUE/2;
	m_pBtnJango->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x = width - 20;
	m_pBtnFold->MoveWindow(pos_x, pos_y, 20, btn_h, TRUE);
	
	// Line 2	-- �ֹ����� ��
	pos_x = 0;
	pos_y += btn_h + PADDING_VALUE;
	m_pStJumun->MoveWindow(pos_x, pos_y, width, btn_h, FALSE);

	// Line 2<->3 -- �ֹ����ɼ��� ( ���¸���ϰ�쿡�� )
	if (!m_pBtnFold->IsWindowEnabled())
	{
		pos_x = 0;
		pos_y += btn_h + PADDING_VALUE;

		m_pStJvol->MoveWindow(pos_x, pos_y, 45, btn_h, FALSE); pos_x += 45;
		m_pEdJprc->MoveWindow(pos_x, pos_y, 50, btn_h, FALSE); pos_x += 55;
		m_chkAutoJvol.MoveWindow(pos_x, pos_y, 80, btn_h, FALSE);

		pos_x = 0;
		pos_y += btn_h + PADDING_VALUE/2;
		dx = width/3;
		m_pJvolGrid->MoveWindow(pos_x, pos_y, width, GRID_CELL_HEIGHT*3, FALSE);
		for(int n=0; n<3; ++n)
			m_pJvolGrid->SetColumnWidth(n, dx);

		CRect rc;
		m_pJvolGrid->GetCellRect(0, 0, &rc);
		m_pBtnJvol->MoveWindow(pos_x, pos_y, rc.Width()+2, rc.Height()+2);

		pos_y += GRID_CELL_HEIGHT*3 + PADDING_VALUE;
	}
	else
	{
		pos_y += btn_h + PADDING_VALUE;
	}

	// Line 3	-- �ֹ����� ���� �׸���
	pos_x = 0;
	//pos_y += GRID_CELL_HEIGHT*3 + PADDING_VALUE; ������ �Ѵ�~
	row = m_pJcntGrid->GetRowCount();
	col = m_pJcntGrid->GetColumnCount();
	m_pJcntGrid->SetNoScrollBar(TRUE);

	m_pJcntGrid->MoveWindow(0, pos_y, width, btn_h*4 );
	for(n=0; n<col; ++n)
		m_pJcntGrid->SetColumnWidth(n, width/col);
	for(n=0; n<row; ++n)
		m_pJcntGrid->SetRowHeight(n, btn_h);

	// Line 4	-- �ֹ����� SpinButton
	pos_y += btn_h*4 + PADDING_VALUE/2;
	m_pStJcnt->MoveWindow( 0, pos_y,       75, btn_h, FALSE);
	m_pSpJcnt->MoveWindow(75, pos_y, width-75, btn_h, FALSE);

	// Line 5	-- �����ֹ�
	pos_y += btn_h + PADDING_VALUE/2;
	m_pStRsvd->MoveWindow( 0, pos_y,       43, btn_h, FALSE);
	m_pSpRsvd->MoveWindow(43, pos_y, 50, btn_h, FALSE);
	m_chkMarket.MoveWindow(96,pos_y, 85, btn_h, FALSE);

	// Line 6	-- �ֹ�����
	pos_x = 5;
	pos_y += btn_h + PADDING_VALUE/2;
	dx = (width-pos_x)/3;
	m_rdNone.MoveWindow(pos_x     , pos_y, dx, btn_h, FALSE); pos_x += 5;
	m_rdIOC.MoveWindow( pos_x+dx  , pos_y, dx, btn_h, FALSE); pos_x += 2;
	m_rdFOK.MoveWindow( pos_x+dx*2, pos_y, dx, btn_h, FALSE);

	// Line 7	-- �����ֹ�
	pos_x = 5;
	pos_y += btn_h + PADDING_VALUE/2;
	m_chkMulti.MoveWindow(pos_x, pos_y, 70, btn_h, FALSE);
	m_pSpMulti->MoveWindow(pos_x+70, pos_y, width-(pos_x+70), btn_h);

	// Line 7-1 -- ��Ŭ���ֹ�
	pos_x = 5;
	pos_y += btn_h + PADDING_VALUE/2;
	m_chkOneClick.MoveWindow(pos_x, pos_y, 90, btn_h, FALSE);

// 	pos_x = 5;
// 	pos_y += btn_h +PADDING_VALUE/2;
// 	m_chkAuto.MoveWindow( pos_x, pos_y,       70, btn_h, FALSE);
// 	m_pSpAuto->MoveWindow(75, pos_y, width-75, btn_h, FALSE);
// 
// 	pos_x = 5;
// 	pos_y += btn_h +PADDING_VALUE/2;
// 	m_chkLoss.MoveWindow( pos_x, pos_y,       70, btn_h, FALSE);
// 	m_pSpLoss->MoveWindow(75, pos_y, width-75, btn_h, FALSE);

	// Line 8.0 -- ��
	pos_x = 0;
	pos_y += tab_h;
	m_pTab->MoveWindow(pos_x, pos_y, width, tab_h, FALSE);
	m_pBtnChegLink->MoveWindow(width-41, pos_y, 20, 20, FALSE);

	m_pBtnSize->MoveWindow(width - 20, pos_y, 20, 20, TRUE);

	// Line 8	-- ���� - �ܰ�/��ü�� ��
// 	pos_x = 0;
// 	pos_y += 20;
// 	m_pStMstr->MoveWindow(pos_x, pos_y, width, btn_h, FALSE);

	// ü��׸���
	{
		int pos_x2 = 0;
		int pos_y2 = pos_y + tab_h + PADDING_VALUE/2;
		m_pChegGrid->MoveWindow(pos_x2, pos_y2, width, height-pos_y2);
	}

	// Line 9	-- �������� ��ư
	pos_x = 0;
	pos_y += tab_h + PADDING_VALUE/2;
	dx = (width-9)/4;
	for(n=0; n<4; ++n, pos_x+=(dx+4))
	{
		m_pBtnFuture[n]->MoveWindow(pos_x, pos_y, dx, btn_h);
		m_pStFuture[n]->MoveWindow(pos_x, pos_y+btn_h+PADDING_VALUE/3, dx, btn_h);
	}

	// Line 10	-- �ɼ����� ��ư
	pos_x = 0;
	pos_y += btn_h*2 + PADDING_VALUE;
	dx = (width-9)/4;
	for(n=0; n<4; ++n, pos_x+=(dx+4))
		m_pBtnOption[n]->MoveWindow(pos_x, pos_y, dx, btn_h);

	// Line 11	-- �ɼǱ׸���
	pos_x = 0;
	pos_y += btn_h + PADDING_VALUE/2;
	m_pOptGrid->MoveWindow(pos_x, pos_y, width, height-pos_y);
	col = m_pOptGrid->GetColumnCount();
	dx = (width-SCROLLBAR_WIDTH)/col;
	m_pOptGrid->SetColumnWidth(0, dx);
	m_pOptGrid->SetColumnWidth(1, width-SCROLLBAR_WIDTH-(dx*2) );
	m_pOptGrid->SetColumnWidth(2, dx);
	
	MakeBackground();
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pBtnCheg  = CreateImgButton("ü��", IDC_BTN_CHEG , m_hBtn2);
	m_pBtnChart = CreateImgButton("��Ʈ", IDC_BTN_CHART, m_hBtn2);
	m_pBtnJango = CreateImgButton("�ܰ�", IDC_BTN_JANGO, m_hBtn2);
	m_pBtnFold   = CreateImgButton("<<" , IDC_BTN_FOLD,  m_hBtn2);
	m_pBtnChegLink = CreateImgButton("" , IDC_BTN_CHEGLINK,  m_hBtnLink);

	CRect rc(0, 0, 0, 0);
	m_pStJumun = new CfxStatic(m_pFont);
	m_pStJumun->Create("�ֹ� ����", WS_CHILD|WS_VISIBLE|WS_BORDER, rc, this);
	m_pStJumun->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStJumun->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);

	m_pStJvol = new CfxStatic(m_pFont);
	m_pStJvol->Create("�ֹ���", WS_VISIBLE|WS_CHILD|SS_LEFT, rc, this);
	m_pStJvol->SetBkColor(m_crBodyBk );
	m_pStJvol->SetAlignment(alLEFT);
	
	m_pEdJprc = new CfxEdit(m_pFont);
	m_pEdJprc->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_RIGHT|ES_READONLY, rc, this, -1);
	m_pEdJprc->SetPasswordChar(0x0);
	m_pEdJprc->SetLimitText(8);

	m_chkAutoJvol.Create("�ڵ���ȸ", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_AUTOJVOL);
	m_chkAutoJvol.SetFont(m_pFont);
	m_chkAutoJvol.SetCheck(FALSE);

	m_pJvolGrid = new CGridCtrl();
	m_pJvolGrid->SetNoScrollBar(TRUE);
	m_pJvolGrid->Create(rc, this, IDC_GRID_JVOL, WS_CHILD|WS_BORDER|WS_TABSTOP|WS_VISIBLE|WS_CLIPSIBLINGS);
	SetJvolGrid();

	m_pBtnJvol = CreateImgButton("��ȸ", IDC_BTN_JVOL, m_hBtn3);
	m_pBtnJvol->BringWindowToTop();

	m_pJcntGrid = new CGridCtrl();
	m_pJcntGrid->SetNoScrollBar(TRUE);
	m_pJcntGrid->Create(rc, this, IDC_GRID_JCNT);
	m_pJcntGrid->EnableSelection(FALSE);
	m_pJcntGrid->SetFrameFocusCell(FALSE);
	m_pJcntGrid->EnableTitleTips(FALSE);
	m_pJcntGrid->SetFont(m_pFont);
	CGridCellBase *pc = m_pJcntGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)
	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
		m_pJcntGrid->SetRowCount(4);  
		m_pJcntGrid->SetColumnCount(4);
	}
	m_pJcntGrid->ShowScrollBar(SB_BOTH, FALSE);

	m_pStJcnt = new CfxStatic(m_pFont);
	m_pStJcnt->Create(" �ֹ����� ", WS_VISIBLE|WS_CHILD, rc, this);
	m_pStJcnt->SetBkColor( m_crBodyBk );

	m_pSpJcnt = new CfxSpin(m_pFont);
	m_pSpJcnt->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
	m_pSpJcnt->SetRangeEx(0, 5000);

	m_rdNone.Create("����", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON, rc, this, IDC_RD_NONE);
	m_rdIOC.Create("IOC", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON, rc, this, IDC_RD_IOC);
	m_rdFOK.Create("FOK", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON, rc, this, IDC_RD_FOK);
	m_rdNone.SetFont(m_pFont); m_rdIOC.SetFont(m_pFont); m_rdFOK.SetFont(m_pFont);
	m_rdNone.SetCheck(TRUE);

	m_chkMulti.Create("���߸Ÿ�", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_MULTI);
	m_chkMulti.SetFont(m_pFont);
	m_chkMulti.SetCheck(FALSE);
	
	m_chkOneClick.Create("��Ŭ�� �ֹ�", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_OCLICK);
	m_chkOneClick.SetFont(m_pFont);
	m_chkOneClick.SetCheck(FALSE);

	m_pSpMulti = new CfxSpin(m_pFont);
	m_pSpMulti->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
	m_pSpMulti->EnableWindow(FALSE);
	m_pSpMulti->SetText("2");
	m_pSpMulti->SetRangeEx(2, 10);

	m_pStRsvd = new CfxStatic(m_pFont);
	m_pStRsvd->Create("����ƽ", WS_VISIBLE|WS_CHILD, rc, this);
	m_pStRsvd->SetBkColor( m_crBodyBk );

	m_chkMarket.Create("���尡", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_MARKET);
	m_chkMarket.SetFont(m_pFont);
	m_chkMarket.SetCheck(FALSE);
	
	m_pSpRsvd = new CfxSpin(m_pFont);
	m_pSpRsvd->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
	m_pSpRsvd->SetText("1");
	m_pSpRsvd->SetRangeEx(-100, 100);

// 	m_pTooltip = new CToolTipCtrl;
// 	m_pTooltip->Create(this);
// 	m_pTooltip->Activate(TRUE);
// 	m_pTooltip->AddTool(GetDlgItem(IDC_CHK_MARKET),"�簢�� ǥ�ô� ���尡 �ֹ��� ���մϴ�.");

// 	m_chkAuto.Create("�ڵ�û��", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_AUTO);
// 	m_chkAuto.SetFont(m_pFont);
// 	m_chkAuto.SetCheck(FALSE);
// 	
// 	m_pSpAuto = new CfxSpin(m_pFont);
// 	m_pSpAuto->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
// 	m_pSpAuto->SetText("1");
// 	m_pSpAuto->SetRangeEx(-100, 100);
// 	m_pSpAuto->EnableWindow(FALSE);
// 
// 	m_chkLoss.Create("�ڵ�����", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_LOSS);
// 	m_chkLoss.SetFont(m_pFont);
// 	m_chkLoss.SetCheck(FALSE);
// 	
// 	m_pSpLoss = new CfxSpin(m_pFont);
// 	m_pSpLoss->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
// 	m_pSpLoss->SetText("1");
// 	m_pSpLoss->SetRangeEx(-100, 100);
// 	m_pSpLoss->EnableWindow(FALSE);

	CString home = Variant(homeCC, "");

	CString szn, szu, szd;
	
	szn = home + "\\image\\" + "â��������_Ȯ��.BMP";
	szu = home + "\\image\\" + "â��������_Ȯ��_en.BMP";
	szd = home + "\\image\\" + "â��������_Ȯ��_dn.BMP";
	
	m_hBtnSize[0] = GetAxBitmap(szn);
	m_hBtnSize[1] = GetAxBitmap(szu);
	m_hBtnSize[2] = GetAxBitmap(szd);

 	m_pBtnSize = CreateImgButton("", IDC_BTN_SIZE , m_hBtnSize);

	m_pTab = new CfxTab();
	m_pTab->Create(CRect(0,0,0,24), this, IDC_CTL_TAB1);
	//m_pTab->InsertItem(0, "�ܰ�(��)");
	m_pTab->InsertItem(0, "ü��");
	m_pTab->LoadTabImage(Variant(homeCC, ""), false);
	m_pTab->SetStyle(0x00000001, false);
	m_pTab->SetBorderColor(GetIndexColor(TAB_BORDER_INDEX));
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CONTENTS_BACK_INDEX), 
			GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CONTENTS_BACK_INDEX), GetIndexColor(CONTENTS_BACK_INDEX));
	m_pTab->SetFont(m_pFont);

	m_pChegGrid = new CGridCtrl();
	m_pChegGrid->Create(rc, this, IDC_GRID_CHEG);
	m_pChegGrid->ShowWindow(SW_HIDE);
	m_pChegGrid->SetFont(m_pFont);
	SetChegGrid();

	m_pStMstr = new CfxStatic(m_pFont);
	m_pStMstr->Create("���� - �ܰ�(��ü��)", WS_CHILD|WS_VISIBLE|WS_BORDER, rc, this);
	m_pStMstr->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStMstr->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);

	for(size_t n=0; n<4; ++n)
	{
		m_pBtnFuture[n] = CreateImgButton("", IDC_BTN_FUT1+n , m_hBtn2);
		if (n<m_fjcode.size())
		{
			CString str(m_fjcode[n].hnam, sizeof(m_fjcode[n].hnam));
			str.TrimRight();
			m_pBtnFuture[n]->SetWindowText(str.Right(4));
		}

		m_pStFuture[n] = new CfxStatic(m_pFont);
		m_pStFuture[n]->Create("123", WS_VISIBLE|WS_CHILD, rc, this);
		m_pStFuture[n]->Init(ssBORDER|ssTEXT, 0, "", "", 
			GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_BTNTEXT), alRIGHT);
		m_pStFuture[n]->SetBkColor(m_crWhite);
		m_pStFuture[n]->SetBorderColor( m_crLine, false);

		m_pBtnOption[n] = CreateImgButton("", IDC_BTN_OPT1+n , m_hBtn2);
		{
			CString str(m_ophead.pjym[n], sizeof(m_ophead.pjym[n]));
			str.TrimRight();
			m_pBtnOption[n]->SetWindowText(str.Right(2) + "��");
		}
	}

	m_pOptGrid = new CGridCtrl();
	m_pOptGrid->Create(rc, this, IDC_GRID_OPT);
	m_pOptGrid->SetFont(m_pFont);
	SetOptionGrid();
	//SetOptionData(0);
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_OPT1, 0), (LPARAM)m_pBtnOption[0]->GetSafeHwnd());

	LoadConfig();

	return 0;
}

CfxImgButton* CControlWnd::CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp)
{
	CfxImgButton *pBtn = new CfxImgButton(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn;
	}
	else return NULL;
}

void CControlWnd::OnBtnCheg()
{
	CString opt;
	opt.Format(MAP_CHEG);
	SetView(opt);
}

void CControlWnd::OnBtnChart()
{
	CString opt;
	opt.Format(MAP_CHART);
	SetView(opt);

	//OutputDebugString("START TIMER +++++++++++++++++++++++++++++++++++++++++++\n");
	m_strCode.Format("%s",(char*)m_pMapWnd->SendMessage(WM_CODE, (WPARAM)0, 0));
// 	OutputDebugString("START TIMER +++++++++++++++++++++++++++++++++++++++++++\n");
	SetTimer(333,200,NULL);
}

void CControlWnd::OnBtnJango()
{
	CString opt;
	opt.Format(MAP_JANGO);
	SetView(opt);
}

void CControlWnd::MakeBackground( int width, int height )
{
	if (width<0 || height<0)
	{
		CRect rc;
		GetClientRect(rc);
		width = rc.Width();
		height = rc.Height();
	}

	CDC *pdc = GetDC();
	{
		CDC mdc; 
		CBitmap *oldBmp;

		m_hBk.DeleteObject();
		m_hBk.CreateCompatibleBitmap(pdc, width, height);
		
		mdc.CreateCompatibleDC(pdc);
		oldBmp = mdc.SelectObject(&m_hBk);
		
		mdc.FillSolidRect(0, 0, width, height, m_crBodyBk);
		
		mdc.SelectObject(oldBmp);
		mdc.DeleteDC();
	}
	ReleaseDC(pdc);
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
}

void CControlWnd::Draw(CDC *pdc)
{
	CDC mdc;
	CRect rc;
	
	GetClientRect(rc);
	mdc.CreateCompatibleDC(pdc);	
	CBitmap *oldBmp = mdc.SelectObject(&m_hBk);
	pdc->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	
	mdc.DeleteDC();	
}

BOOL CControlWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	static CString oldVal;
	static CCellID oldId(-1, -1);
	if (wParam==IDC_GRID_JCNT)
	{
		*pResult = 1;
        GV_DISPINFO *pi = (GV_DISPINFO*)lParam;
		if (pi->hdr.code == GVN_BEGINLABELEDIT)
		{
			// ���� �����ϸ� ���� ���� ����
			oldVal = m_pJcntGrid->GetItemText(pi->item.row, pi->item.col);
		}
        else if (pi->hdr.code == GVN_ENDLABELEDIT || pi->hdr.code == GVN_SELCHANGED)
		{
			CCellID cId(pi->item.row, pi->item.col);
			CString key, val = m_pJcntGrid->GetItemText(cId.row, cId.col);
			val.TrimLeft(); val.TrimRight();

			if (pi->hdr.code==GVN_ENDLABELEDIT)
			{
				// �����̸� ���������� �ǵ�����.
				if (val.GetLength()>0)
				{
					key.Format("MA_%d_%d", cId.row, cId.col);
					WritePrivateProfileString("JQTY_GRID", (LPCSTR)key, (LPCSTR)val, (LPCSTR)m_profile);
				} 
				else 
				{
					val = oldVal;
					m_pJcntGrid->SetItemText(cId.row, cId.col, oldVal);
				}
			}
			
			// �ֹ����� ����
			m_pSpJcnt->SetText(val);

			for(int row=0; row<m_pJcntGrid->GetRowCount(); ++row)
			{
				for(int col=0; col<m_pJcntGrid->GetColumnCount(); ++col)
				{
					if (pi->item.row==row && pi->item.col==col)
					{
						m_pJcntGrid->SetItemBkColour(row, col, ::GetSysColor(COLOR_HIGHLIGHT));
						m_pJcntGrid->SetItemFgColour(row, col, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
					}
					else
					{
						m_pJcntGrid->SetItemBkColour(row, col, m_crWhite);
						m_pJcntGrid->SetItemFgColour(row, col, m_crBlack);
					}
				}
			}
			m_pJcntGrid->Invalidate(FALSE);
		}
	}
	else if (wParam==IDC_GRID_JVOL)
	{
		NM_GRIDVIEW *view = (NM_GRIDVIEW *)lParam;
		if (view->hdr.code==NM_CLICK || view->hdr.code==NM_DBLCLK)
		{
			if (view->iRow>0 && view->iColumn>0)
			{
				CString val = m_pJvolGrid->GetItemText(view->iRow, view->iColumn);
				val.Replace(",", "");
				val.TrimRight();
				if (!val.IsEmpty())
					SetJcnt( atoi((LPCSTR)val) );
			}
		}
	}
	else if (wParam==IDC_GRID_OPT)
	{
		*pResult = 1;
		NM_GRIDVIEW *view = (NM_GRIDVIEW *)lParam;
		if (view->hdr.code == NM_DBLCLK || view->hdr.code == NM_CLICK)
		{
			if (view->iRow>0)
			{
				OptionRows &rows = m_dtOption[m_dtOptionIdx];
				if (view->iColumn==0)       // Call
					GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)rows[ view->iRow-1 ].call_code);
				else if (view->iColumn==2)	// Put
					GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)rows[ view->iRow-1 ].put_code);
			}
		}
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

HBRUSH CControlWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	int nID = pWnd->GetDlgCtrlID();
	if (nID==IDC_RD_NONE || nID==IDC_RD_IOC || nID==IDC_RD_FOK || nID==IDC_CHK_MULTI || nID==IDC_CHK_OCLICK || nID==IDC_CHK_AUTOJVOL || nID==IDC_CHK_AUTO || nID==IDC_CHK_LOSS || nID == IDC_CHK_MARKET)
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(m_crBodyBk);
		return m_hBrush1;
	}

	return hbr;
}

void CControlWnd::OnChkMulti()
{
	BOOL bCheck = m_chkMulti.GetCheck();
	m_chkMulti.SetCheck(FALSE);
	if (bCheck)
	{
		CDialog dlg(IDD_MULTI_DLG, this);
		if (dlg.DoModal()==IDOK)
		{
			m_chkMulti.SetCheck(TRUE);
			m_pSpMulti->EnableWindow(TRUE);
		}
	}
	else
	{
		m_pSpMulti->EnableWindow(FALSE);
	}
}

void CControlWnd::LoadMaster()
{
	CFile file;
	CString path;
	path.Format("%s\\tab\\", Variant(homeCC, ""));

	// ���������ڵ�
	if (file.Open(path+"fjcode.dat", CFile::modeRead))
	{
		m_fjcode.resize( (size_t)(file.GetLength()/sizeof(struct fjcode)) );
		file.Read(&m_fjcode[0], m_fjcode.size() * sizeof(struct fjcode) );
		file.Close();
	}
	
	if (file.Open(path+"opcode.dat", CFile::modeRead))
	{
		m_opcode.resize( (size_t)((file.GetLength()-sizeof(struct ojcodh)) / sizeof(struct ojcode) ));
		file.Read(&m_ophead, sizeof(struct ojcodh));
		file.Read(&m_opcode[0], m_opcode.size() * sizeof(struct ojcode));
		file.Close();

		m_dtOption.clear();
		vector<struct ojcode>::iterator st, ed;
		st = m_opcode.begin();
		ed = m_opcode.end();
		for(int n=0;st!=ed; ++st)
		{
			for(int i=0; i<4; ++i)
			{
				if (st->put[i].yorn=='1')
				{
					OptionRow row;
					row.atmg = (st->atmg==1);
					sprintf(row.call_code, "%.8s", st->call[3-i].cod2);
					sprintf(row.put_code, "%.8s", st->put[i].cod2);
					sprintf(row.hsga, "%.3s.%.2s", &st->price[0], &st->price[3]);
				
					m_dtOption[i].push_back(row);
				}
			}
		}
		m_OptionMap.clear();
		for(int n=0; n<4; ++n)
		{
			reverse(m_dtOption[n].begin(), m_dtOption[n].end());
			for(size_t j=0; j<m_dtOption[n].size(); ++j)
			{
				m_OptionMap[ m_dtOption[n][j].call_code ] = OptionIdxRow(n, j+1);
				m_OptionMap[ m_dtOption[n][j].put_code  ] = OptionIdxRow(n, j+1);
				//m_OptionMap[string(st->call[3-i].cod2, 8)] = OptionIdxRow(i, m_dtOption[i].size()-1);
			}
		}
	}
}

void CControlWnd::SetChegGrid()
{
	CGridCellBase *pc;
	m_pChegGrid->SetEditable(FALSE);
	m_pChegGrid->EnableSelection(FALSE);
	m_pChegGrid->SetFrameFocusCell(FALSE);
	m_pChegGrid->SetColumnResize(FALSE);
	m_pChegGrid->SetRowResize(FALSE);
	m_pChegGrid->EnableTitleTips(FALSE);
	m_pChegGrid->SetDefCellHeight(16);

	m_pChegGrid->SetRowCount(1);
	m_pChegGrid->SetColumnCount(3);

	pc = m_pChegGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
	}

	pc = m_pChegGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pChegGrid->SetBkColor(m_crWhite);
	m_pChegGrid->SetRowCount(m_dtOption[0].size()+1);
	m_pChegGrid->SetColumnCount(3);
	m_pChegGrid->SetFixedRowCount(1);
	
	char *head[] = { "�ð�", "ü�ᰡ", "����" };
	int  width[] = { 60, 50, 40 };
	for(int n=0; n<3; ++n)
	{
		m_pChegGrid->SetItemText(0, n, head[n]);
		m_pChegGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		m_pChegGrid->SetColumnWidth(n, width[n]);
	}
}


void CControlWnd::SetOptionGrid()
{
	CGridCellBase *pc;
	m_pOptGrid->SetEditable(FALSE);
	m_pOptGrid->EnableSelection(FALSE);
	m_pOptGrid->SetFrameFocusCell(FALSE);
	m_pOptGrid->SetColumnResize(FALSE);
	m_pOptGrid->SetRowResize(FALSE);
	m_pOptGrid->EnableTitleTips(FALSE);
	
	pc = m_pOptGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
	}
	pc = m_pOptGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pOptGrid->SetBkColor(m_crWhite);
	m_pOptGrid->SetRowCount(m_dtOption[0].size()+1);
	m_pOptGrid->SetColumnCount(3);
	m_pOptGrid->SetFixedRowCount(1);

	char *head[] = { "Call", "��簡", "Put" };
	for(int n=0; n<3; ++n)
	{
		m_pOptGrid->SetItemText(0, n, head[n]);
		m_pOptGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
}

void CControlWnd::SetOptionData( int idx )
{
	OptionRows &rows = m_dtOption[idx];
	if (rows.empty()) return;

	for(int n=0; n<4; ++n)
	{
		if (n==idx)
			m_pBtnOption[n]->SetImgBitmap(m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP());
		else
			m_pBtnOption[n]->SetImgBitmap(m_hBtn2[0]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[2]->operator HBITMAP());
		m_pBtnOption[n]->Invalidate(FALSE);
	}

	m_dtOptionIdx = idx;
	m_pOptGrid->SetRowCount(rows.size()+1);
	int atmRow = -1;
	OptionRows::iterator st, ed;
	st = rows.begin();
	ed = rows.end();
	for(size_t i=0; i<rows.size(); ++i, ++st)
	{
		m_pOptGrid->SetItemText(i+1, 0, "");
		m_pOptGrid->SetItemText(i+1, 1, st->hsga);
		m_pOptGrid->SetItemText(i+1, 2, "");

		m_pOptGrid->SetItemFormat(i+1, 0, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		m_pOptGrid->SetItemFormat(i+1, 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		m_pOptGrid->SetItemFormat(i+1, 2, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

		m_pOptGrid->SetItemBkColour(i+1, 1, (st->atmg) ? m_crLine : m_crBodyBk);
		if (st->atmg)
			atmRow = i+1;
	}

	if (atmRow>0)
	{
#ifdef _DEF_IB300900_
		m_pOptGrid->SetScrollPos(SB_VERT, m_pOptGrid->GetDefCellHeight()*(atmRow-4), FALSE);
#else
		m_pOptGrid->SetScrollPos(SB_VERT, m_pOptGrid->GetDefCellHeight()*(atmRow-6), FALSE);
#endif
		m_pOptGrid->PostMessage(WM_VSCROLL, 0, 0);
	}

	m_pOptGrid->Refresh();

	DisplayAllJango();
}

void CControlWnd::OnFutBtnClick( UINT nID )
{
	int idx = nID - IDC_BTN_FUT1;
	CString code(m_fjcode[idx].cod2, sizeof(m_fjcode[idx].cod2));
	GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)code);
}

void CControlWnd::OnPutBtnClick( UINT nID )
{
	int idx = nID-IDC_BTN_OPT1;
	SetOptionData(idx);
}

void CControlWnd::LoadConfig()
{
	int def_val[][4] = {{1, 2, 3, 5},{10, 20, 30, 50},{100, 200, 300, 500}, {1000, 2000, 3000, 5000}};
	
	int ival;
	CString key, sval;
	for(int row=0; row<4; ++row)
	{
		for(int col=0; col<4; ++col)
		{
			key.Format("MA_%d_%d", row, col);
			ival = GetPrivateProfileInt("JQTY_GRID", (LPCSTR)key, def_val[row][col], (LPCSTR)m_profile);
			sval.Format("%d", ival);
			m_pJcntGrid->SetItemText(row, col, (LPCSTR)sval );
		}
	}

	//int iSel = GetPrivateProfileInt("TAB1", "SELECT", 0, (LPCSTR)m_profile);
	m_pTab->SetCurSel(0);

	NMHDR hdr;
	hdr.idFrom = IDC_CTL_TAB1;
	OnTabChange(&hdr, NULL);
}

int CControlWnd::GetJqty()
{
	return Str2Int(m_pSpJcnt->GetText());
}

int CControlWnd::GetCdgb()
{
	if      (m_rdNone.GetCheck()) return  0;
	else if (m_rdIOC.GetCheck())  return  1;
	else if (m_rdFOK.GetCheck())  return  2;
	else                          return -1;
}

LRESULT CControlWnd::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case WP_JANGO: 
		DisplayJango((Jango*)lParam);
		break;
	case WP_MICHEG: 
		DisplayMicheg((LPCSTR)lParam);
		break;
	case WP_CTRL2_FOLD_DISABLE: 
		m_pBtnFold->ShowWindow( ((BOOL)lParam) ? SW_HIDE : SW_SHOW );
		m_pBtnFold->EnableWindow( !(BOOL)lParam );

		if(nSizeMode == SIZE_GENERAL)
		{
			CalcSize();
		}
		else
		{
			CalcSizeJango();
		}
		break;
	case WP_FOLDSTAT:
		m_pBtnFold->SetWindowText( ((BOOL)lParam) ? ">>" : "<<" );
		break;
	}

	return 0;
}

void CControlWnd::DisplayJango( Jango *pj )
{
	CString text, tmp;
	AccountMap::iterator st, ed;
	
	int wcnt = 0, jqty = 0;	// ��ü�� �Ǽ�, �ܰ����
	
	// �ܰ���� ���
	st = m_pAccount->begin();
	ed = m_pAccount->end();
	for(; st!=ed; ++st)
	{
		CodeJangoMap::iterator pos = st->second.jango.find(pj->code);
		if (pos != st->second.jango.end())
		{
			jqty += pos->second.jqty;
		}
	}
	
	// ��ü��Ǽ� ���
	st = m_pAccount->begin();
	ed = m_pAccount->end();
	for(; st!=ed; ++st)
	{
		CodeMichegMap::iterator pos = st->second.micheg.find(pj->code);
		if (pos != st->second.micheg.end())
		{
			PriceMichegMap::iterator st2 = pos->second.begin();
			PriceMichegMap::iterator ed2 = pos->second.end();
			for(; st2!=ed2; ++st2)
				wcnt += st2->second.d_cnt + st2->second.s_cnt;
		}
	}
	
	if (jqty<=0 && wcnt<=0) text = "";
	else if (jqty>0 && wcnt<=0) text.Format("%d", jqty);
	else if (jqty>0 && wcnt>0)	text.Format("%d(%d)", jqty, wcnt);
	else if (jqty<=0 && wcnt>0) text.Format("(%d)", wcnt);
	else return;		// -_-;
	
	//TRACE("\nControl Jango -> %d %d\n", jqty, wcnt);

	if (pj->code[0]=='1' || pj->code[0] == 'A')  //�Ļ���ǰ �ڵ尳��
	{
		int cnt = (int)m_fjcode.size();
		for(int n=0; n<cnt; ++n)
		{
			if (memcmp(pj->code, m_fjcode[n].cod2, 8)==0)
			{
				if (m_pStFuture[n]->GetText()!=text)
					m_pStFuture[n]->SetText(text);
				break;
			}
		}
	}
	else
	{
		OptionIdxMap::iterator pos = m_OptionMap.find(pj->code) ;
		if (pos != m_OptionMap.end())
		{
			if (m_dtOptionIdx==pos->second.first)
			{
				CCellID id;
				id.row = pos->second.second;
				
				if      (pj->code[0]=='2' || pj->code[0] == 'B') id.col = 0;  //�Ļ���ǰ �ڵ尳��
				else if (pj->code[0]=='3' || pj->code[0] == 'C') id.col = 2;
				else return;
				
				if (m_pOptGrid->GetItemText(id.row, id.col)!=text)
				{
					m_pOptGrid->SetItemText(id.row, id.col, text);
					m_pOptGrid->RedrawCell(id);
				}
			}
		}
		}
}

void CControlWnd::DisplayAllJango()
{
	Jango jngo;
	set<string> codes;

	AccountMap::iterator st, ed;
	st = m_pAccount->begin();
	ed = m_pAccount->end();
	for(; st!=ed; ++st)
	{
		CodeJangoMap::iterator st2, ed2;
		st2 = st->second.jango.begin();
		ed2 = st->second.jango.end();
		for(; st2!=ed2; ++st2)
			codes.insert(st2->first);
	}

	set<string>::iterator st3 = codes.begin();
	set<string>::iterator ed3 = codes.end();
	for(; st3!=ed3; ++st3)
	{
		sprintf(jngo.code, st3->c_str());
		DisplayJango(&jngo);
	}
}

void CControlWnd::DisplayMicheg( LPCSTR code )
{
	Jango jngo;
	if (code && strlen(code)==8)
	{
		sprintf(jngo.code, code);
		DisplayJango(&jngo);
	}
	else
	{
		set<string> tmp;

		AccountMap::iterator st, ed;
		st = m_pAccount->begin();
		ed = m_pAccount->end();
		for(; st!=ed; ++st)
		{
			CodeMichegMap::iterator cst, ced;
			cst = st->second.micheg.begin();
			ced = st->second.micheg.end();
			for(; cst!=ced; ++cst)
			{
				//MessageBox(cst->first.c_str());
				tmp.insert(cst->first);
			}
		}

		set<string>::iterator tst, ted;
		tst = tmp.begin();
		ted = tmp.end();
		for(; tst!=ted; ++tst)
		{
			sprintf(jngo.code, tst->c_str());
			DisplayJango(&jngo);
		}
	}
}

int CControlWnd::GetMulti()
{
	if (m_chkMulti.GetCheck() && m_pSpMulti->IsWindowEnabled())
		return atoi((LPCSTR)m_pSpMulti->GetText());
	else
		return 0;
}

void CControlWnd::Clear()
{
	int n;
	for(n=0; n<4; ++n)
		m_pStFuture[n]->SetText("");
	for(n=1; n<m_pOptGrid->GetRowCount(); ++n)
	{
		m_pOptGrid->SetItemText(n, 0, ""); m_pOptGrid->RedrawCell(n, 0);
		m_pOptGrid->SetItemText(n, 2, ""); m_pOptGrid->RedrawCell(n, 2);
	}
}

int CControlWnd::GetRsvd()
{
	return atoi((LPCSTR)m_pSpRsvd->GetText());
}

void CControlWnd::ChangeTheme()
{
	AxInit();
	m_pStJumun->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStJumun->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);
	m_pStMstr->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStMstr->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);
	SetOptionGrid();
	SetJvolGrid();

	for(int n=0; n<m_pChegGrid->GetColumnCount(); ++n)
	{
		m_pChegGrid->SetItemBkColour(0, n, GetIndexColor(COLOR_GRIDHEAD));
		m_pChegGrid->SetItemFgColour(0, n, GetIndexColor(COLOR_GRIDHEADTXT));
	}

	m_pTab->LoadTabImage(Variant(homeCC, ""), false);

	Invalidate(TRUE);
}

void CControlWnd::OnBtnFold()
{
	CString txt;
	m_pBtnFold->GetWindowText(txt);
	if (txt=="<<")
	{
		m_pBtnFold->SetWindowText(">>");
		GetParent()->SendMessage(WM_APP_SIG, WP_CTRL2_FOLD, (LPARAM)TRUE);
	}
	else if (txt==">>")
	{
		m_pBtnFold->SetWindowText("<<");
		GetParent()->SendMessage(WM_APP_SIG, WP_CTRL2_FOLD, (LPARAM)FALSE);
	}

}

void CControlWnd::OnChkOneClick()
{
	GetParent()->SendMessage(WM_APP_SIG, WP_ONECLICK_MODE, (LPARAM)m_chkOneClick.GetCheck());
}

void CControlWnd::SetJvolGrid()
{
	m_pJvolGrid->SetEditable(FALSE);
	m_pJvolGrid->EnableSelection(FALSE);
	m_pJvolGrid->SetFrameFocusCell(FALSE);
	m_pJvolGrid->SetFont(m_pFont);
	m_pJvolGrid->SetColumnResize(FALSE);
	m_pJvolGrid->SetRowResize(FALSE);
	m_pJvolGrid->ShowScrollBar(SB_BOTH, FALSE);
	m_pJvolGrid->SetDefCellHeight(GRID_CELL_HEIGHT);

	m_pJvolGrid->SetRowCount(3);
	m_pJvolGrid->SetColumnCount(3);
	m_pJvolGrid->SetFixedRowCount(1);
	m_pJvolGrid->SetFixedColumnCount(1);

	COLORREF row_cols[3] = { GetIndexColor(COLOR_GRIDHEAD), m_crMadoBk, m_crMasuBk};
	for(int row=0; row<3; ++row)
	{
		for(int col=0; col<m_pJvolGrid->GetColumnCount(); ++col)
		{
			m_pJvolGrid->SetItemBkColour(row, col, row_cols[row]);
			if (row==0)
				m_pJvolGrid->SetItemFgColour(row, col, GetIndexColor(COLOR_GRIDHEADTXT));
			else
				m_pJvolGrid->SetItemFgColour(row, col, m_crNormal);

			if (row!=0 && col!=0)
				m_pJvolGrid->SetItemFormat(row, col, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			else
				m_pJvolGrid->SetItemFormat(row, col, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}

	m_pJvolGrid->SetItemText(0, 1, "�� ��");
	m_pJvolGrid->SetItemText(0, 2, "û ��");
	m_pJvolGrid->SetItemText(1, 0, "�� ��");
	m_pJvolGrid->SetItemText(2, 0, "�� ��");
}

void CControlWnd::SetJvolPrice( int jprc )
{
	CString sdat;
	sdat.Format("%d.%02d", jprc/100, jprc%100);
	if (m_pEdJprc)
		m_pEdJprc->SetText(sdat);

	if (m_chkAutoJvol.GetCheck())
		req_SONBQ101();
}

void CControlWnd::SetJcnt( int jcnt, BOOL bPower )
{
	//2017.01.24 KSJ m_pJcntGrid���� �������� �� �׸��� ���õ� �͵� �ʱ�ȭ
	if (!bPower &&jcnt<=0) return;

	char buff[16];
	sprintf(buff, "%d", jcnt);
	m_pSpJcnt->SetText(buff);

	for(int row=0; row<m_pJcntGrid->GetRowCount(); ++row)
	{
		for(int col=0; col<m_pJcntGrid->GetColumnCount(); ++col)
		{
			m_pJcntGrid->SetItemBkColour(row, col, m_crWhite);
			m_pJcntGrid->SetItemFgColour(row, col, m_crBlack);
		}
	}

	//2017.02.10 KSJ �׸��忡�� ���õ� �� �ʱ�ȭ
	m_pJcntGrid->SetFocusCell(-1, -1);

	m_pJcntGrid->Invalidate(FALSE);
}

void CControlWnd::req_SONBQ101()
{
	if (m_pAccount->empty()) return;

	for(int row=1; row<m_pJvolGrid->GetRowCount(); ++row)
	{
		for(int col=1; col<m_pJvolGrid->GetColumnCount(); ++col)
		{
			m_pJvolGrid->SetItemText(row, col, "");
			m_pJvolGrid->RedrawCell(row, col);
		}
	}

	vector<char> data( L_ledgerH + sizeof(SONBQ101_mid), ' ' );
	
	CString OdrPrc = m_pEdJprc->GetText();
	OdrPrc.Replace(".", "");	
	int dOdrPrc = atoi(OdrPrc);
	OdrPrc.Format("+%09d.%02d", dOdrPrc/100, dOdrPrc%100);
	
	struct _ledgerH *ledger = (struct _ledgerH*)&data[0];
	SONBQ101_mid *mid = (SONBQ101_mid*)&data[L_ledgerH];
	
	GetLedger(ledger);
	CopyMemory(ledger->svcd, "SONBQ101", 8);
	CopyMemory(ledger->rcnt, "0000", 4);
	CopyMemory(ledger->mkty, "2", 1);
	CopyMemory(ledger->odrf, "2", 1);
	
	CopyMemory(mid->In, "00001", 5);
	CopyMemory(mid->zQryTp, "1", 1);
	CopyMemory(mid->lOrdAmt, "+000000000000000", 16);
	CopyMemory(mid->dRatVal, "+000000000.00000000", 19);
	CopyMemory(mid->zFnoIsuNo, m_pSiseData->code, m_pSiseData->code.GetLength());
	CopyMemory(mid->dOrdPrc, (LPCSTR)OdrPrc, 13);
	CopyMemory(mid->zFnoOrdprcPtnCode, "00", 2);
	
	AccountMap::iterator st = m_pAccount->begin();
	AccountMap::iterator ed = m_pAccount->end();
	for(;st!=ed;++st)
	{
		string acno = st->first;
		string pswd = (LPCSTR)st->second.pswd;
		FillMemory(mid->zAcntNo, sizeof(mid->zAcntNo), ' ');
		CopyMemory(mid->zAcntNo, acno.c_str(), acno.size());
		FillMemory(mid->zPwd, sizeof(mid->zPwd), ' ');
		CopyMemory(mid->zPwd, pswd.c_str(), pswd.size());
	
		// �ŵ�
		CopyMemory(mid->zBnsTp, "1", 1);
		SendTR("piboPBxQ", US_ENC, &data[0], data.size(), TK_SONBQ101);

		// �ż�
		CopyMemory(mid->zBnsTp, "2", 1);
		SendTR("piboPBxQ", US_ENC, &data[0], data.size(), TK_SONBQ101);
	}
}

LRESULT CControlWnd::OnUser( WPARAM wParam, LPARAM lParam )
{
	int msg = LOBYTE(LOWORD(wParam));
	int key = HIBYTE(LOWORD(wParam));
	int len = HIWORD(wParam);
	LPCSTR dat = (LPCSTR)lParam;
	switch(msg)
	{
	case DLL_OUB:
		switch(key)
		{
		case TK_SONBQ101:
			res_SONBQ101(dat, len);
			break;
		}
	}
	return 0;	
}

void CControlWnd::res_SONBQ101( LPCSTR data, int dlen )
{
	struct _ledgerH *ledger = (struct _ledgerH*)data;
	SONBQ101_mod *mod = (SONBQ101_mod*)(data+L_ledgerH);

	int tmp, row;

	if      (mod->mid.zBnsTp[0]=='1') row = 1;
	else if (mod->mid.zBnsTp[0]=='2') row = 2;
	else return;

	CString OrdAbleQty(mod->lOrdAbleQty, sizeof(mod->lOrdAbleQty));
	CString NewAbleQty(mod->lNewOrdAbleQty, sizeof(mod->lNewOrdAbleQty));
	CString LqdtAbleQty(mod->lLqdtOrdAbleQty, sizeof(mod->lLqdtOrdAbleQty));
	
	tmp = atoi(OrdAbleQty); OrdAbleQty = Int2CommaStr(tmp);
	tmp = atoi(NewAbleQty); NewAbleQty = Int2CommaStr(tmp);
	tmp = atoi(LqdtAbleQty); LqdtAbleQty = Int2CommaStr(tmp);

	//MessageBox(OrdAbleQty + "|" + NewAbleQty + "|" + LqdtAbleQty);
	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	GetParent()->SendMessage(WM_USER, DLL_GUIDE, (LPARAM)(LPCSTR)emsg);

	m_pJvolGrid->SetItemText(row, 1, NewAbleQty);
	m_pJvolGrid->SetItemText(row, 2, LqdtAbleQty);
	m_pJvolGrid->RedrawRow(row);
}

void CControlWnd::OnBtnJvol()
{
	req_SONBQ101();
}

void CControlWnd::OnTabChange( NMHDR *pHeader, LRESULT *pResult )
{
	if (pHeader->idFrom==IDC_CTL_TAB1 && m_pTab)
	{
		int nCmd1 = SW_SHOW, nCmd2 = SW_SHOW;
		if (m_pTab->GetCurSel()==1)
		{
			nCmd1 = SW_SHOW;
			nCmd2 = SW_HIDE;
			m_bChegUpdate = FALSE;
		}
		else if (m_pTab->GetCurSel()==0) 
		{
			nCmd1 = SW_HIDE;
			nCmd2 = SW_SHOW;
			m_bChegUpdate = TRUE;
		}
		else 
		{
			return;
		}

		{
			CString strVal;
			strVal.Format("%d", m_pTab->GetCurSel());
			WritePrivateProfileString("TAB1", "SELECT", (LPCSTR)strVal, (LPCSTR)m_profile);
		}

		for(int n=0; n<4; ++n)
		{
			if (m_pStFuture[n])  m_pStFuture[n]->ShowWindow(nCmd1);
			if (m_pBtnFuture[n]) m_pBtnFuture[n]->ShowWindow(nCmd1);
			if (m_pBtnOption[n]) m_pBtnOption[n]->ShowWindow(nCmd1);
			if (m_pOptGrid) m_pOptGrid->ShowWindow(nCmd1);
		}
		m_pChegGrid->ShowWindow(nCmd2);
		m_pBtnChegLink->ShowWindow(nCmd2);
	}
}

void CControlWnd::SetRealData( RealData *rp )
{
	RealData::iterator p = rp->find(34);
	if (p!=rp->end())
	{
		CString time;
		time.Format("%.2s:%.2s:%.2s", p->second.first, p->second.first+2, p->second.first+4);
		CString curr = rp->find(23)->second.ToCStr();
		CString cvol = rp->find(32)->second.ToCStr();
		
		if (m_pChegGrid)
		{
			m_pChegGrid->InsertRow(NULL, 1);
			
			m_pChegGrid->SetItemFormat(1, 0, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			m_pChegGrid->SetItemText(1, 0, time);

			curr.TrimLeft();
			curr.TrimRight();
			
			switch(curr.GetAt(0))
			{
			case '+': m_pChegGrid->SetItemFgColour(1, 1, m_crPlus);
					  m_pChegGrid->SetItemText(1, 1, curr.Mid(1)); break;
			case '-': m_pChegGrid->SetItemFgColour(1, 1, m_crMinus);
					  m_pChegGrid->SetItemText(1, 1, curr.Mid(1)); break;
			default : m_pChegGrid->SetItemFgColour(1, 1, m_crBlack); 
					  m_pChegGrid->SetItemText(1, 1, curr); break;
			}
			
			//m_pChegGrid->SetItemText(1, 1, curr.Mid(1));

			switch(cvol.GetAt(0))
			{

			case '+':
				m_pChegGrid->SetItemFgColour(1, 2, m_crPlus);
				m_pChegGrid->SetItemBkColour(1, 2, m_crWhite);
				break;
			case '-':
				m_pChegGrid->SetItemFgColour(1, 2, m_crMinus);
				m_pChegGrid->SetItemBkColour(1, 2, m_crWhite);
				break;
			case '1': 
				m_pChegGrid->SetItemFgColour(1, 2, m_crWhite);
				m_pChegGrid->SetItemBkColour(1, 2, m_crPlus);
				break;
			case '4':
				m_pChegGrid->SetItemFgColour(1, 2, m_crWhite);
				m_pChegGrid->SetItemBkColour(1, 2, m_crMinus);
				break;
			}
			m_pChegGrid->SetItemText(1, 2, cvol.Mid(1));
			if (m_bChegUpdate)
				m_pChegGrid->Refresh();

			if (m_pChegGrid->GetRowCount()>200)
				m_pChegGrid->SetRowCount(200);
		}
	}
}

void CControlWnd::SetRealData( DWORD* data )
{
	if (data[34])
	{
		CString time = (char*)data[34];

		if(time.GetLength() > 5)
		{
			time.Insert(2,':');
			time.Insert(5,':');
		}

		CString curr = (char*)data[23];
		CString cvol = (char*)data[32];
		
		if (m_pChegGrid)
		{
			m_pChegGrid->InsertRow(NULL, 1);
			
			m_pChegGrid->SetItemFormat(1, 0, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			m_pChegGrid->SetItemText(1, 0, time);
			
			switch(curr.GetAt(0))
			{
			case '+': m_pChegGrid->SetItemFgColour(1, 1, m_crPlus); break;
			case '-': m_pChegGrid->SetItemFgColour(1, 1, m_crMinus); break;
			default : m_pChegGrid->SetItemFgColour(1, 1, m_crBlack); break;
			}
			m_pChegGrid->SetItemText(1, 1, curr.Mid(1));
			
			switch(cvol.GetAt(0))
			{
				
			case '+':
				m_pChegGrid->SetItemFgColour(1, 2, m_crPlus);
				m_pChegGrid->SetItemBkColour(1, 2, m_crWhite);
				break;
			case '-':
				m_pChegGrid->SetItemFgColour(1, 2, m_crMinus);
				m_pChegGrid->SetItemBkColour(1, 2, m_crWhite);
				break;
			case '1': 
				m_pChegGrid->SetItemFgColour(1, 2, m_crWhite);
				m_pChegGrid->SetItemBkColour(1, 2, m_crPlus);
				break;
			case '4':
				m_pChegGrid->SetItemFgColour(1, 2, m_crWhite);
				m_pChegGrid->SetItemBkColour(1, 2, m_crMinus);
				break;
			}
			m_pChegGrid->SetItemText(1, 2, cvol.Mid(1));
			if (m_bChegUpdate)
				m_pChegGrid->Refresh();
			
			if (m_pChegGrid->GetRowCount()>200)
				m_pChegGrid->SetRowCount(200);
		}
	}
}

void CControlWnd::ClearChegGrid()
{
	SetChegGrid();
}

void CControlWnd::OnChegLink()
{
	CString opt = MAP_SISECHEG;
	SetView(opt);
}

void CControlWnd::OnChkAuto()
{
// 	if(m_chkAuto.GetCheck() == TRUE)
// 	{
// 		m_pSpAuto->EnableWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_pSpAuto->EnableWindow(FALSE);
// 	}
}

void CControlWnd::OnChkLoss()
{
// 	if(m_chkLoss.GetCheck() == TRUE)
// 	{
// 		m_pSpLoss->EnableWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_pSpLoss->EnableWindow(FALSE);
// 	}
}

void CControlWnd::OnBtnSize()
{
	if(nSizeMode == SIZE_GENERAL)
	{
		nSizeMode = SIZE_JANGO;
		
		m_pStJumun->ShowWindow(SW_HIDE);
		m_pStJvol->ShowWindow(SW_HIDE);
		m_pEdJprc->ShowWindow(SW_HIDE);
		m_chkAutoJvol.ShowWindow(SW_HIDE);
		m_pJvolGrid->ShowWindow(SW_HIDE);
		m_pBtnJvol->ShowWindow(SW_HIDE);
		m_pJcntGrid->ShowWindow(SW_HIDE);
		m_pStJcnt->ShowWindow(SW_HIDE);
		m_pSpJcnt->ShowWindow(SW_HIDE);
		m_pStRsvd->ShowWindow(SW_HIDE);
		m_pSpRsvd->ShowWindow(SW_HIDE);
		m_rdNone.ShowWindow(SW_HIDE);
		m_rdIOC.ShowWindow(SW_HIDE);
		m_rdFOK.ShowWindow(SW_HIDE);
		m_chkMulti.ShowWindow(SW_HIDE);
		m_pSpMulti->ShowWindow(SW_HIDE);
		m_chkOneClick.ShowWindow(SW_HIDE);
		m_chkMarket.ShowWindow(SW_HIDE);

		CalcSizeJango();
	}
	else
	{
		nSizeMode = SIZE_GENERAL;

		m_pStJumun->ShowWindow(SW_SHOW);
		m_pStJvol->ShowWindow(SW_SHOW);
		m_pEdJprc->ShowWindow(SW_SHOW);
		m_chkAutoJvol.ShowWindow(SW_SHOW);
		m_pJvolGrid->ShowWindow(SW_SHOW);
		m_pBtnJvol->ShowWindow(SW_HIDE);
		m_pJcntGrid->ShowWindow(SW_SHOW);
		m_pStJcnt->ShowWindow(SW_SHOW);
		m_pSpJcnt->ShowWindow(SW_SHOW);
		m_pStRsvd->ShowWindow(SW_SHOW);
		m_pSpRsvd->ShowWindow(SW_SHOW);
		m_rdNone.ShowWindow(SW_SHOW);
		m_rdIOC.ShowWindow(SW_SHOW);
		m_rdFOK.ShowWindow(SW_SHOW);
		m_chkMulti.ShowWindow(SW_SHOW);
		m_pSpMulti->ShowWindow(SW_SHOW);
		m_chkOneClick.ShowWindow(SW_SHOW);
		m_chkMarket.ShowWindow(SW_SHOW);

		CalcSize();
	}

	Invalidate();
}

void CControlWnd::OnChkMarket()
{
	if(m_chkMarket.GetCheck())
	{
		m_pSpRsvd->EnableWindow(FALSE);

		if(m_bMarketConfirm)
		{
			CMarketConfirm dlg(this);

			dlg.DoModal();
		}
	}
	else
	{
		m_pSpRsvd->EnableWindow(TRUE);
	}
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
// 	CRect rc;
// 	m_chkMarket.GetClientRect(rc);
// 
// 	if(rc.PtInRect(point))
// 	{
// 		m_pTooltip->AddTool(this, "�簢�� ǥ�ô� ���尡 �ֹ��� ���մϴ�.", rc, IDC_CHK_MARKET);
// 		m_pTooltip->Activate(TRUE);
// 	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CControlWnd::PreTranslateMessage(MSG* pMsg) 
{	
// 	OutputDebugString("PreTranslateMessage======================================>GO\n");
// 	if(pMsg->message == WM_MOUSEMOVE && pMsg->hwnd == m_chkMarket.m_hWnd)
// 	{
// 		OutputDebugString("RELAY EVENT-======================================>GO\n");
// 		m_pTooltip->RelayEvent(pMsg);
// 	}
	return CWnd::PreTranslateMessage(pMsg);
}

LRESULT CControlWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
// 	CString s;
// 	s.Format("WINDOW PROC : [%d]\n",message);
// 	OutputDebugString(s);
// 	MSG msg;
// 	msg.hwnd = m_hWnd;
// 	msg.message = message;
// 	msg.wParam = wParam;
// 	msg.lParam = lParam;
// 	if (message == WM_SETCURSOR)
// 	{
// 		OutputDebugString("RELAY EVENT-======================================>GO\n");
// 
// 		m_pTooltip->RelayEvent(&msg);
// 	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 333)
	{
//		OutputDebugString("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
		KillTimer(333);

		CString param;
		CString tmp;
		tmp.Format("GETCODE : [%s]\n",m_strCode);
//		OutputDebugString(tmp);

		param.Format("ed_fcod\t%s", m_strCode); Variant(codeCC, param); Variant(triggerCC, param);
		
		//2017.01.24 KSJ 3010���� Ʈ���� �����ǹǷ� �ּ�ó��
		//param.Format("100301\t%s", m_strCode); Variant(triggerCC, param);Variant(codeCC, param);
	}
	
	CWnd::OnTimer(nIDEvent);
}
