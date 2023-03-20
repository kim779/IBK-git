// SellWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100700.h"
#include "SellWnd.h"
#include "MapWnd.h"
#include "../../control/fx_misc/misctype.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSellWnd

CSellWnd::CSellWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
{
}

CSellWnd::~CSellWnd()
{
	//_SAFEDESTROY(m_pGroupStatic);
	//_SAFEDESTROY(m_pGroup);
	//_SAFEDESTROY(m_pGroupID);
	//_SAFEDESTROY(m_pAddAccount);
	//_SAFEDESTROY(m_pPassSet);
	//_SAFEDESTROY(m_pClear);
	//_SAFEDESTROY(m_pRemain);
	//_SAFEDESTROY(m_pOrder);
	//_SAFEDESTROY(m_pContract);
	//_SAFEDESTROY(m_pCodeCtrl);
	//_SAFEDESTROY(m_pCodeSearch);
	//_SAFEDESTROY(m_pSymbol);
	//_SAFEDESTROY(m_pTypeStatic);
	//_SAFEDESTROY(m_pType);
	//_SAFEDESTROY(m_pCondition);
	//_SAFEDESTROY(m_pQuantityStatic);
	//_SAFEDESTROY(m_pQuantity);
	//_SAFEDESTROY(m_pCostStatic);
	//_SAFEDESTROY(m_pCostType);
	//_SAFEDESTROY(m_pCost);
	//_SAFEDESTROY(m_pSellGrid); 
}


BEGIN_MESSAGE_MAP(CSellWnd, CWnd)
	//{{AFX_MSG_MAP(CSellWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADDACCOUNT, OnAddAccount)
	ON_BN_CLICKED(IDC_PASSSET, OnPassSet)
	ON_BN_CLICKED(IDC_CODESEARCH, OnCodeSearch)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_REMAIN, OnRemain)
	ON_BN_CLICKED(IDC_ORDER, OnOrder)
	ON_BN_CLICKED(IDC_CONTRACT, OnContract)
	ON_CBN_SELENDOK(IDC_CODECTRL, OnCodeSelEndOk)
	ON_CBN_SELENDOK(IDC_TYPE, OnTypeSelEndOk)
	ON_CBN_SELENDOK(IDC_CONDITION, OnConditionSelEndOk)
	ON_CBN_SELENDOK(IDC_COSTTYPE, OnCostTypeSelEndOk)
	ON_CBN_SELENDOK(IDC_GROUP, OnGroupSelEndOk)
	ON_EN_CHANGE(IDC_QUANTITY, OnQuantity)
	ON_EN_CHANGE(IDC_COST, OnCost)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSellWnd message handlers

int CSellWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pParent = (CMapWnd*)GetParent();

	CRect rcCtrl, rcClient;
	GetClientRect(&rcClient);
	rcCtrl.SetRect(SZ_TAB_HMARGIN, SZ_FORM_MARGIN, SZ_TAB_HMARGIN + SZ_CTRL_2STATIC, SZ_FORM_MARGIN + SZ_CTRL_HEIGHT);
	
	m_pGroupStatic =std::make_unique<CfxStatic>();
	m_pGroupStatic->Create(_T("�׷�"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_GROUPSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_7WIDTH;

	m_pGroup =std::make_unique<CComboBox>();
	m_pGroup->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_GROUP);
	SetWindowTheme(m_pGroup->GetSafeHwnd(), L"", L"");
	m_pGroupID =std::make_unique<CComboBox>();
	m_pGroupID->Create(WS_CHILD | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(0, 0, 0, 0), this, IDC_GROUP);
	SetWindowTheme(m_pGroupID->GetSafeHwnd(), L"", L"");
	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4BUTTON;

	m_pAddAccount =std::make_unique<CfxImgButton>();
	m_pAddAccount->Create("\n���µ��", rcCtrl, this, IDC_ADDACCOUNT, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN8_WIDTH;

	m_pPassSet =std::make_unique<CfxImgButton>();
	m_pPassSet->Create("\n���º�й�ȣ����", rcCtrl, this, IDC_PASSSET, TRUE, FALSE);

	rcCtrl.left = rcClient.right - SZ_TAB_HMARGIN - SZ_CTRL_3BUTTON - SZ_CTRL_4BUTTON * 2 - SZ_CTRL_2BUTTON - 6;
	rcCtrl.left = rcClient.right - (BTN4_WIDTH*2) - BTN3_WIDTH - BTN2_WIDTH - CTRL_SPACE_W;
	rcCtrl.right = rcCtrl.left + BTN3_WIDTH;

	m_pClear =std::make_unique<CfxImgButton>();
	m_pClear->Create("\nClear", rcCtrl, this, IDC_CLEAR, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN4_WIDTH;

	m_pRemain =std::make_unique<CfxImgButton>();
	m_pRemain->Create("\n�ܰ����", rcCtrl, this, IDC_REMAIN, FALSE, TRUE);
	
	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN4_WIDTH;

	m_pOrder =std::make_unique<CfxImgButton>();
	m_pOrder->Create("\n�ϰ��ֹ�", rcCtrl, this, IDC_ORDER, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN2_WIDTH;

	m_pContract =std::make_unique<CfxImgButton>();
	m_pContract->Create("\nü��", rcCtrl, this, IDC_CONTRACT, FALSE, TRUE);
	
	rcCtrl.left = SZ_TAB_HMARGIN;
	rcCtrl.right = SZ_COMBO_3WIDTH;
	rcCtrl.top = rcCtrl.bottom + CTRL_SPACE;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	m_pCodeCtrl =std::make_unique<CfxCodeCtrl>(this, CString(m_pParent->GetAxRoot() + "\\tab"));
	m_pCodeCtrl->Create(this, rcCtrl, IDC_CODECTRL);

	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN1_WIDTH;

	m_pCodeSearch =std::make_unique<CfxImgButton>();
	m_pCodeSearch->Create("\nȣ��", rcCtrl, this, IDC_CODESEARCH, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_SYMBOL;

	InitSymbol(rcCtrl);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE_W;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;
	
	m_pTypeStatic =std::make_unique<CfxStatic>();
	m_pTypeStatic->Create(_T("����"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_TYPESTATIC);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;					// ĸ�ǰ� ��Ʈ�Ѱ� ������ 5�� �Ѵ�. (fxStatic ���θ��� 2px)
	rcCtrl.right = rcCtrl.left + SZ_COMBO_6WIDTH;

	m_pType =std::make_unique<CComboBox>();
	m_pType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_TYPE);
	SetWindowTheme(m_pType->GetSafeHwnd(), L"", L"");

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_2WIDTH;
	
	m_pCondition =std::make_unique<CComboBox>();
	m_pCondition->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_CONDITION);
	SetWindowTheme(m_pCondition->GetSafeHwnd(), L"", L"");

	rcCtrl.left = rcCtrl.right + CTRL_SPACE_W;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pQuantityStatic =std::make_unique<CfxStatic>();
	m_pQuantityStatic->Create(_T("����"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_QUANTITYSTATIC);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4EDIT;

	m_pQuantity =std::make_unique<CfxEdit>();
	m_pQuantity->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_QUANTITY);
	m_pQuantity->SetPasswordChar(0);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE_W;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pCostStatic =std::make_unique<CfxStatic>();
	m_pCostStatic->Create(_T("�ܰ�"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_COSTSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_4WIDTH;
	
	m_pCostType =std::make_unique<CComboBox>();
	m_pCostType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_COSTTYPE);
	SetWindowTheme(m_pCostType->GetSafeHwnd(), L"", L"");
	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_6EDIT;

	m_pCost =std::make_unique<CfxEdit>();
	m_pCost->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_COST);
	m_pCost->SetPasswordChar(0);

	rcCtrl.left = 0;
	rcCtrl.right = rcClient.right - INNER_FRAME_SPACE;
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP + 1;
	rcCtrl.bottom = rcClient.bottom;

	m_pSellGrid =std::make_unique<CSellGrid>();
	m_pSellGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_SELLGRID);

	return 0;
}

void CSellWnd::Init()
{
	CFont* pFont = m_pParent->GetNormalFont();

	m_pGroupStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pGroupStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pGroupStatic->SetFont(pFont);

	m_pGroup->SetFont(pFont);

	CString strPath = "";
	strPath.Format("%s\\Image\\", m_pParent->GetAxRoot());

	HBITMAP hbmp	= m_pParent->GetAxBitmap(strPath + "4btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= m_pParent->GetAxBitmap(strPath + "4btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = m_pParent->GetAxBitmap(strPath + "4btn_en.bmp")->operator HBITMAP();

	m_pAddAccount->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pAddAccount->SetWindowText(_T("���µ��"));
	m_pAddAccount->SetFont(pFont);
	
	hbmp	= m_pParent->GetAxBitmap(strPath + "3btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "3btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "3btn_en.bmp")->operator HBITMAP();
	m_pClear->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pClear->SetWindowText(_T("Clear"));
	m_pClear->SetFont(pFont);

	hbmp	= m_pParent->GetAxBitmap(strPath + "4btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "4btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "4btn_en.bmp")->operator HBITMAP();
	m_pRemain->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pRemain->SetWindowText(_T("�ܰ����"));
	m_pRemain->SetFont(pFont);
	
	m_pOrder->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pOrder->SetWindowText(_T("�ϰ��ֹ�"));
	m_pOrder->SetFont(pFont);

	hbmp	= m_pParent->GetAxBitmap(strPath + "2btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "2btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "2btn_en.bmp")->operator HBITMAP();
	m_pContract->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pContract->SetWindowText(_T("ü��"));
	m_pContract->SetFont(pFont);

	m_pCodeCtrl->SetUnit(GU_CODE);
	m_pCodeCtrl->SetFont(pFont);
	m_pCodeCtrl->SetHistory(m_pParent->Variant(historyCC, _T("1301")));

	hbmp	= m_pParent->GetAxBitmap(strPath + "s�˻�.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "s�˻�_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "s�˻�_en.bmp")->operator HBITMAP();

	m_pCodeSearch->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pCodeSearch->SetFont(pFont);

	hbmp	= m_pParent->GetAxBitmap(strPath + "8btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "8btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "8btn_en.bmp")->operator HBITMAP();
	m_pPassSet->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pPassSet->SetWindowText(_T("���º�й�ȣ����"));
	m_pPassSet->SetFont(pFont);

	m_pTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pTypeStatic->SetFont(pFont);

	m_pType->AddString(_T("������"));
	m_pType->AddString(_T("���尡"));
	m_pType->AddString(_T("���Ǻ�������"));
	m_pType->AddString(_T("������������"));
	m_pType->AddString(_T("�ֿ켱������"));
	m_pType->AddString(_T("�尳���� �ð�������"));
	m_pType->AddString(_T("�ð�������"));
	m_pType->SetCurSel(0);
	m_pType->SetFont(pFont);

	m_pCondition->AddString(_T("����"));
	m_pCondition->AddString(_T("FOK"));
	m_pCondition->AddString(_T("IOC"));
	m_pCondition->SetCurSel(0);
	m_pCondition->SetFont(pFont);
	
	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pQuantityStatic->SetFont(pFont);
	
	m_pQuantity->SetFont(pFont);
	m_pQuantity->SetWindowText(_T("0"));

	m_pCostStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pCostStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pCostStatic->SetFont(pFont);

	m_pCostType->AddString(_T("������"));
	m_pCostType->AddString(_T("���簡"));
	m_pCostType->AddString(_T("�ŵ�ȣ��"));
	m_pCostType->AddString(_T("�ż�ȣ��"));
	m_pCostType->AddString(_T("���Ѱ�"));
	m_pCostType->AddString(_T("���Ѱ�"));

	m_pCostType->SetCurSel(1);
	m_pCostType->SetFont(pFont);


	m_pCost->ModifyAttribute(0, attrNUMERIC);
	m_pCost->SetFont(pFont);

	m_pSellGrid->SetFont(pFont);
}

void CSellWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	CRect rcClient, rcCtrl, rcTmp;
	GetClientRect(&rcClient);

	m_pCostType->GetWindowRect(&rcCtrl);
	ScreenToClient(rcCtrl);

	rcTmp = rcClient;
	rcTmp.bottom = rcCtrl.bottom +INNER_FRAME_SPACE;
	dc.FillSolidRect(&rcTmp, pMapWnd->GetIndexColor(CMD_BACK_INDEX));

	rcTmp = rcClient;
	rcTmp.top = rcCtrl.bottom +INNER_FRAME_SPACE;
	dc.FillSolidRect(&rcTmp, pMapWnd->GetIndexColor(CONTENTS_BACK_INDEX));
}

void CSellWnd::InitSymbol(CRect rcCtrl)
{
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*) = nullptr;

	text.Format("%s/dev/cx_symbol.dll", m_pParent->GetAxRoot());
	m_hiSymbol = AfxLoadLibrary(text);

	rcCtrl.OffsetRect(0, SZ_CTRL_HEIGHT + SZ_FORM_MARGIN);
	if (m_hiSymbol != NULL)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
		if (axCreate != NULL)
		{
			struct	_param	param;

			m_pWizard->ClientToScreen(&rcCtrl);
			ScreenToClient(&rcCtrl);

			CRect rc;
			m_pCodeSearch->GetWindowRect(&rc);
			ScreenToClient(&rc);
			rcCtrl.top = rc.top ;
			rcCtrl.bottom = rc.bottom ;


			param.key   = 0;
			param.name  = _T("17413");
			param.rect  = rcCtrl;
			param.fonts = "����ü";
			param.point = 9;
			param.style = 1;

			param.tRGB  = 69;
			param.pRGB  = 90;
			param.options = _T("/a89/b91/c92/d69");

			m_pSymbol = (*axCreate)(m_pWizard, &param);
			m_pSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

			m_pSymbol->SetParent(this);

			if (m_pSymbol == NULL)
			{
				AfxFreeLibrary(m_hiSymbol);
				m_hiSymbol = NULL;
			}
		}
	}
}

void CSellWnd::OnAddAccount()
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 2), (LPARAM)_T("IB100701 /p5 /S"));
	ClearSellGrid();
	m_pParent->QueryGroupList();
}

void CSellWnd::OnCodeSearch()
{
	CRect rcCtrl;
	m_pCodeSearch->GetClientRect(rcCtrl);
	m_pCodeSearch->ClientToScreen(&rcCtrl);
	CString strCodeData = (char*) m_pWizard->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(allCODE, NULL)), MAKELPARAM(rcCtrl.left, rcCtrl.bottom));
	
	if (!strCodeData.IsEmpty() && strCodeData.GetLength() > 6)
	{
		CString strCode = strCodeData.Left(6);
		if (m_pCodeCtrl)
			m_pCodeCtrl->SetEditData(strCode);

		QuerySymbol(strCode);
		OnRemain();
	}
}

void CSellWnd::QuerySymbol(CString strCode)
{
	CString strText;
	strText.Format("1301%c%s\t17413\t", 0x7f, strCode);
	m_pParent->SendTR(_T("POOPPOOP"), strText, KEY_SYMBOL, 0);

	m_pParent->SetCode(strCode);

	CString strProc;
	strProc.Format("change_code\t%s", strCode);
	m_pParent->Procedure(strProc);
}


void CSellWnd::OnCodeSelEndOk()
{
	CString strCode = m_pCodeCtrl->GetEditData();
	QuerySymbol(strCode);
	OnRemain();
}

void CSellWnd::ParsingSymbol(WPARAM wParam, LPARAM lParam)
{
	if (m_pSymbol)
	{
		m_pSymbol->SendMessage(WM_USER, wParam, lParam);
		
		if (HIWORD(wParam) >= L_jinfo)
		{
			struct _jinfo jinfo;
			CopyMemory(&jinfo, (void*)lParam, L_jinfo);
			CString m_strListedQuantity;
			m_strListedQuantity = CString(jinfo.sjjs, sizeof(jinfo.sjjs));
			if(m_pSellGrid)
			{
				
				m_strListedQuantity.TrimLeft(); m_strListedQuantity.TrimRight();
				m_strListedQuantity.Format("%d",atoi(m_strListedQuantity));
				if (m_strListedQuantity=="") m_strListedQuantity = "0"; 
				m_pSellGrid->fListedQty = m_strListedQuantity;
			}
		}
	}
}

void CSellWnd::OnClear()
{
	ClearSellGrid();
}

void CSellWnd::ClearSellGrid()
{
	CUGCell cell;
	m_pSellGrid->SetNumberRows(DEF_SELLGRIDROW);

	for (int i = 0; i < MAX_SELLGRIDCOL; i++)
	{
		for (int j = 0; j < DEF_SELLGRIDROW; j++)
		{
			m_pSellGrid->GetCell(i, j, &cell);
			cell.ClearProperty(UGCELL_TEXT_SET);
			m_pSellGrid->SetCell(i, j, &cell);
		}
	}

	m_pSellGrid->GotoRow(0);
	m_pSellGrid->RedrawAll();
}

void CSellWnd::OnRemain()
{
	ClearSellGrid();

	CString strGroupID(_T(""));
	CString strCode(_T(""));
	CString strDept(_T(""));
	CString strUser(_T(""));
	CString strSendData(_T(""));

	if (m_pGroup->GetCurSel() == -1)
		return;
	else
		strGroupID.Format("%03d", m_pGroup->GetItemData(m_pGroup->GetCurSel()));

	m_pGroupID->SetCurSel(m_pGroup->GetCurSel());
	m_pGroupID->GetWindowText(strGroupID);
	_TRIMDATA(strGroupID);
	

	BOOL bCustomer = FALSE;
	struct _ledgerH ledger;
	struct i_frem mid {};

	strDept = m_pParent->Variant(deptCC);
	_TRIMDATA(strDept);

	strUser = m_pParent->Variant(userCC, "");
	_TRIMDATA(strUser);

//	strCode = m_pCodeCtrl->GetEditData();

	if (m_pParent->IsCustomer())
		bCustomer = TRUE;

	FillMemory(&ledger, L_ledgerH, ' ');

	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(ledger.svcd, _T("SONAQ113"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(ledger.brno, (LPCTSTR)strDept, strDept.GetLength());
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	strSendData = CString((char*)&ledger, L_ledgerH);

	FillMemory(&mid, L_i_frem, ' ');

	CopyMemory(mid.In, _T("00001"), sizeof(mid.In));

	if (bCustomer)
		mid.zUserTp[0] = _T('2');
	else
		mid.zUserTp[0] = _T('1');

	CopyMemory(mid.zUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(mid.zFundNm, (LPCTSTR)strGroupID, strGroupID.GetLength());
	CopyMemory(mid.zFundKindCode, _T("2 "), sizeof(mid.zFundKindCode));
//	CopyMemory(mid.zIsuNo, _T("KR7000220004"), sizeof(mid.zIsuNo));
	
	strSendData += CString((char*)&mid, L_i_frem);
	
	m_pParent->SendTR("PIBOPBXQ", strSendData, KEY_REMAIN, 0);
}

void CSellWnd::OnOrder()
{
	
	m_arrSendData.RemoveAll();
	m_pSellGrid->GetSendData(&m_arrSendData);
	m_nCurOrder = 0;

	SendOrder();
}

void CSellWnd::OnContract()
{
	CRect  rcMenu;

	std::unique_ptr<CMenu> pMenu = std::make_unique<CMenu>();
	pMenu->CreatePopupMenu();
	pMenu->AppendMenu(MF_STRING, 0, "ü����ȸ");
	pMenu->AppendMenu(MF_STRING, 1, "��ü����ȸ");

	m_pContract->GetWindowRect(&rcMenu);

	const int nSelectGroup = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
								rcMenu.left, rcMenu.bottom, this);
	pMenu = nullptr;

	CString strData(_T(""));

	if (nSelectGroup == 1)
		strData = _T("IB110100 /S/t");
	else if (nSelectGroup == 0)
		strData = _T("IB110200 /S/t");

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

CComboBox* CSellWnd::GetGroupCombo()
{
	return m_pGroup.get();
}

CComboBox* CSellWnd::GetGroupIDCombo()
{
	return m_pGroupID.get();
}

void CSellWnd::SetCode(CString strCode)
{
	if (m_pCodeCtrl == NULL)
		return;

	m_pCodeCtrl->SetEditData(strCode);
	QuerySymbol(strCode);
	OnRemain();
}

CUGCtrl* CSellWnd::GetGridWnd()
{
	return (CUGCtrl*)m_pSellGrid.get();
}

void CSellWnd::ParsingRemain(CString strData, int nLen, CString strMsg)
{
	int nRec = atoi((LPCTSTR)strData.Mid(L_i_frem, 5));
	strData.Delete(0, L_i_frem + 5);

	CString strAccount(_T(""));
	CString strAccName(_T(""));
	CString strCode(_T(""));
	CString strCodeName(_T(""));
	CString strOrderAbleQty(_T(""));
	CString strRemainQty(_T(""));
	CString strUnErcQty(_T(""));
	CString strTemp(_T(""));
	CString strVal(_T(""));
	CString strPass(_T(""));

	CString strQCode = m_pCodeCtrl->GetEditData();
	_TRIMDATA(strQCode);

	int nCurRow = 0;

	for (int i = 0; i < nRec; i++)
	{
		struct o_frec rec;
		CopyMemory(&rec, (LPCTSTR)strData.Left(L_o_frec), L_o_frec);

		strTemp  = CString(rec.zAcntNo, sizeof(rec.zAcntNo));
		_TRIMDATA(strTemp);
		
		if (!strTemp.IsEmpty())
		{
			strAccount = strTemp;
			strAccount.Insert(3, _T("-"));
			strAccount.Insert(6, _T("-"));
		}

		strTemp  = CString(rec.zAcntNm, sizeof(rec.zAcntNm));
		_TRIMDATA(strTemp);
		
		if (!strTemp.IsEmpty())
			strAccName = strTemp;
		
		strCode = CString(rec.zIsuNo, sizeof(rec.zIsuNo));
		_TRIMDATA(strCode);
		if (strCode.GetLength() == 7)
		{
			strCode.Delete(0);

			if (strCode == strQCode)
			{
				strRemainQty = CString(rec.lOrdAbleQty, sizeof(rec.lOrdAbleQty));
				strRemainQty.TrimLeft(_T("+0"));

				if (!strRemainQty.IsEmpty())
				{
					strCodeName = CString(rec.zIsuNm, sizeof(rec.zIsuNm));
					strOrderAbleQty = CString(rec.lOrdAbleQty, sizeof(rec.lOrdAbleQty));
					strOrderAbleQty.TrimLeft(_T("+0"));
					strUnErcQty = CString(rec.lUnercQty, sizeof(rec.lUnercQty));
					
					m_pSellGrid->QuickSetText(1, nCurRow, (LPCTSTR)strAccount);
					m_pSellGrid->QuickSetText(2, nCurRow, (LPCTSTR)strAccName);
					m_pSellGrid->QuickSetText(3, nCurRow, (LPCTSTR)m_pParent->GetAccountRate(strAccount));
					m_pSellGrid->QuickSetText(4, nCurRow, (LPCTSTR)strOrderAbleQty);
					m_pSellGrid->QuickSetText(5, nCurRow, (LPCTSTR)strRemainQty); 			

					m_pCost->GetWindowText(strVal);
					m_pSellGrid->QuickSetText(6, nCurRow, (LPCTSTR)strVal);

					m_pType->GetWindowText(strVal);
					m_pSellGrid->QuickSetText(7, nCurRow, (LPCTSTR)strVal);

					m_pCondition->GetWindowText(strVal);
					m_pSellGrid->QuickSetText(8, nCurRow, (LPCTSTR)strVal);

					m_pSellGrid->RedrawRow(nCurRow++);
				}
			}
		}

		strData.Delete(0, L_o_frec);
	}
}

void CSellWnd::OnTypeSelEndOk()
{
	CString strVal;
	m_pType->GetWindowText(strVal);

	m_pSellGrid->UpdateCheckedRowData(7, strVal);
}

void CSellWnd::OnConditionSelEndOk()
{
	CString strVal;
	m_pCondition->GetWindowText(strVal);

	m_pSellGrid->UpdateCheckedRowData(8, strVal);
}

void CSellWnd::OnQuantity()
{
	CString strVal;
	m_pQuantity->GetWindowText(strVal);
	m_pSellGrid->UpdateCheckedRowData(5, strVal);
}

void CSellWnd::OnCostTypeSelEndOk()
{
	CString strVal(_T(""));

	const int nIndex = m_pCostType->GetCurSel();

	switch (nIndex)
	{
	case 0:
		strVal = m_pParent->GetSiseData(_T("000"));
		break;
	case 1:
		strVal = m_pParent->GetSiseData(_T("023"));
		break;
	case 2:
		strVal = m_pParent->GetSiseData(_T("025"));
		break;
	case 3:
		strVal = m_pParent->GetSiseData(_T("026"));
		break;
	case 4:
		strVal = m_pParent->GetSiseData(_T("302"));
		break;
	case 5:
		strVal = m_pParent->GetSiseData(_T("303"));
		break;
	}

	if (nIndex == 0)
		m_pCost->SetReadOnly(FALSE);
	else
		m_pCost->SetReadOnly(TRUE);

	m_pParent->FormatThousand(strVal);
	m_pCost->SetWindowText(strVal);
}

void CSellWnd::OnCost()
{
	CString strVal;
	m_pCost->GetWindowText(strVal);

	m_pSellGrid->UpdateCheckedRowData(6, strVal);

	if (m_pCostType->GetCurSel() == 0)
		m_pParent->SetCustomPrice(strVal);
}

void CSellWnd::ChangeSiseData()
{
	OnCostTypeSelEndOk();
}

void CSellWnd::SendOrder(BOOL bStart /* = TRUE */)
{
	//�������� �ֹ� �ȳ������� ���� ó�� [ Start ]
	if (!(long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
	{
		if (!(long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCCx), 0L))
		{
			MessageBox("���������� �ֹ��� �Ұ��մϴ�.","IBK��������");
			return;
		}	
	}
	//�������� �ֹ� �ȳ������� ���� ó�� [ End ]
	if (m_nCurOrder >= m_arrSendData.GetSize())
		return;

	CString strGroupID(_T(""));
	CString strCode(_T(""));
	CString strDept(_T(""));
	CString strUser(_T(""));
	CString strSendData(_T(""));

	strCode = m_pCodeCtrl->GetEditData();
	_TRIMDATA(strCode);

	if (strCode.IsEmpty())
		return;

	strCode = _T("A") + strCode;

	if (m_pGroup->GetCurSel() == -1)
		return;
	else
		strGroupID.Format("%03d", m_pGroup->GetItemData(m_pGroup->GetCurSel()));
	m_pGroupID->SetCurSel(m_pGroup->GetCurSel());
	m_pGroupID->GetWindowText(strGroupID);
	_TRIMDATA(strGroupID);

	BOOL bCustomer = FALSE;
	struct _ledgerH ledger;
	struct godr_mid mid {};

	strDept = m_pParent->Variant(deptCC);
	_TRIMDATA(strDept);

	strUser = m_pParent->Variant(userCC, "");
	_TRIMDATA(strUser);

	if (m_pParent->IsCustomer())
		bCustomer = TRUE;

	FillMemory(&ledger, L_ledgerH, ' ');

	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(ledger.svcd, _T("SONAT060"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(ledger.brno, (LPCTSTR)strDept, strDept.GetLength());
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '2';

	FillMemory(&mid, L_godr_mid, ' ');

	CopyMemory(mid.In, _T("00001"), sizeof(mid.In));
	
	if (bCustomer)
	{
		mid.zUserTp[0] = _T('2');
		CopyMemory(mid.zCommdaCode, _T("01"), sizeof(mid.zCommdaCode));
	}
	else
	{
		mid.zUserTp[0] = _T('1');
		CopyMemory(mid.zCommdaCode, _T("00"), sizeof(mid.zCommdaCode));
	}

	CopyMemory(mid.zUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(mid.zFundNm, (LPCTSTR)strGroupID, strGroupID.GetLength());
	mid.zIvstfndPtnTp[0] = _T('2');
	CopyMemory(mid.lSpotOrdCnt, _T("+00001"), sizeof(mid.lSpotOrdCnt));
	CopyMemory(mid.zIsuNo, (LPCTSTR)strCode, strCode.GetLength());
	mid.zBnsTp[0] = _T('1');

	CString strOrderData(_T(""));
	CString strVal(_T(""));
	float fRate = 0;
	int nQty = 0;
	int nPrice = 0;

	strOrderData = m_arrSendData.GetAt(m_nCurOrder++);
	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);
	strVal.Remove(_T('-'));
	CopyMemory(mid.zAcntNo, (LPCTSTR)strVal, strVal.GetLength());

	CString strPass = m_pParent->Variant(passCC, strVal);

	if (strPass.IsEmpty() || strPass.GetLength() != 4)
	{
		MessageBox(_T("���º�й�ȣ ������ ����Ͻʽÿ�"));
		return;
	}

	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);
	strVal.Remove(',');
	fRate = (float)atof((LPCTSTR)strVal);


	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);
	strVal.Remove(',');
	nQty = atoi(strVal);
	
	strVal.Format("+%015d", (int)(nQty /* * fRate*/));

	CopyMemory(mid.lOrdQty, (LPCTSTR)strVal, strVal.GetLength());

	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);
	strVal.Remove(_T(','));
	nPrice = atoi((LPCTSTR)strVal);

	strVal.Format("+%09d.00", nPrice);
	CopyMemory(mid.dOrdPrc, (LPCTSTR)strVal, strVal.GetLength());

	strVal = m_pParent->Parser(strOrderData);
	BOOL bCondition = TRUE;

	if (strVal == _T("������"))
	{
		CopyMemory(mid.zOrdprcPtnCode, _T("00"), sizeof(mid.zOrdprcPtnCode));
		bCondition = TRUE;
	}
	else if (strVal == _T("���尡"))
	{
		CopyMemory(mid.zOrdprcPtnCode, _T("03"), sizeof(mid.zOrdprcPtnCode));
		bCondition = TRUE;
		if (nPrice!=0)
		{
			CString msg; msg.Format("%s �ֹ��� �ŵ��ܰ��� 0���� �ڵ� ���� �˴ϴ�.", strVal);
			MessageBox(msg,"IBK��������",MB_OK);
			m_pSellGrid->QuickSetText(6, GetCheckOrder(m_nCurOrder), "0");
			CopyMemory(mid.dOrdPrc, _T("+000000000.00"), sizeof(mid.dOrdPrc));
		}
	}
	else if (strVal == _T("���Ǻ�������"))
	{
		CopyMemory(mid.zOrdprcPtnCode, _T("05"), sizeof(mid.zOrdprcPtnCode));
		bCondition = FALSE;
	}
	else if (strVal == _T("������������"))
	{
		CopyMemory(mid.zOrdprcPtnCode, _T("06"), sizeof(mid.zOrdprcPtnCode));
		bCondition = TRUE;
		if (nPrice!=0)
		{
			CString msg; msg.Format("%s �ֹ��� �ŵ��ܰ��� 0���� �ڵ� ���� �˴ϴ�.", strVal);
			MessageBox(msg,"IBK��������",MB_OK);
			m_pSellGrid->QuickSetText(6, GetCheckOrder(m_nCurOrder), "0");
			CopyMemory(mid.dOrdPrc, _T("+000000000.00"), sizeof(mid.dOrdPrc));
		}
	}
	else if (strVal == _T("�ֿ켱������"))
	{
		CopyMemory(mid.zOrdprcPtnCode, _T("07"), sizeof(mid.zOrdprcPtnCode));
		bCondition = FALSE;
		if (nPrice!=0)
		{
			CString msg; msg.Format("%s �ֹ��� �ŵ��ܰ��� 0���� �ڵ� ���� �˴ϴ�.", strVal);
			MessageBox(msg,"IBK��������",MB_OK);
			m_pSellGrid->QuickSetText(6, GetCheckOrder(m_nCurOrder), "0");
			CopyMemory(mid.dOrdPrc, _T("+000000000.00"), sizeof(mid.dOrdPrc));
		}
	}
	else if (strVal == _T("�尳���� �ð�������"))
	{
		CopyMemory(mid.zOrdprcPtnCode, _T("61"), sizeof(mid.zOrdprcPtnCode));
		bCondition = FALSE;
		if (nPrice!=0)
		{
			CString msg; msg.Format("%s �ֹ��� �ŵ��ܰ��� 0���� �ڵ� ���� �˴ϴ�.", strVal);
			MessageBox(msg,"IBK��������",MB_OK);
			m_pSellGrid->QuickSetText(6, GetCheckOrder(m_nCurOrder), "0");
			CopyMemory(mid.dOrdPrc, _T("+000000000.00"), sizeof(mid.dOrdPrc));
		}
	}
	else if (strVal == _T("�ð�������"))
	{
		CopyMemory(mid.zOrdprcPtnCode, _T("81"), sizeof(mid.zOrdprcPtnCode));
		bCondition = FALSE;
		if (nPrice!=0)
		{
			CString msg; msg.Format("%s �ֹ��� �ŵ��ܰ��� 0���� �ڵ� ���� �˴ϴ�.", strVal);
			MessageBox(msg,"IBK��������",MB_OK);
			m_pSellGrid->QuickSetText(6, GetCheckOrder(m_nCurOrder), "0");
			CopyMemory(mid.dOrdPrc, _T("+000000000.00"), sizeof(mid.dOrdPrc));
		}
	}

	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);

	if (strVal == _T("����") || !bCondition)
		mid.zOrdCndiTp[0] = _T('0');
	else if (strVal == _T("IOC"))
		mid.zOrdCndiTp[0] = _T('1');
	else if (strVal == _T("FOK"))
		mid.zOrdCndiTp[0] = _T('2');

	if (bStart)
		mid.zGrpPtnTp[0] = _T('S');

	strSendData = CString((char*)&ledger, L_ledgerH) + CString((char*)&mid, L_godr_mid);
	m_pParent->SendTR("PIBOPBXO", strSendData, KEY_SELLORDER, 0);
}

void CSellWnd::ParsingOrder(CString strData, int nLen, CString strMsg)
{
	CString strOrdNo(_T(""));
	strData.Delete(0, L_godr_mid);

	const int nRows = m_pSellGrid->GetNumberRows();
	int nCount = 0;
	BOOL bFind = FALSE;

	for (int i = 0; i < nRows; i++)
	{
		if (m_pSellGrid->IsChecked(i))
		{
			if (nCount == m_nCurOrder - 1)
			{
				nCount = i;
				bFind = TRUE;
				break;
			}
			else
				nCount++;
		}
	}

	if (bFind)
	{
		if (strMsg.Left(3) == _T("ERR"))
		{
			strMsg.Delete(0, 4);
			m_pSellGrid->QuickSetText(10, nCount, (LPCTSTR)strMsg);
		}
		else
		{
			m_pSellGrid->QuickSetText(10, nCount, (LPCTSTR)strMsg);
			
			strOrdNo = strData.Mid(15, 10);
			_TRIMDATA(strOrdNo);
			strOrdNo.TrimLeft(_T("+0"));

			m_pSellGrid->QuickSetText(9, nCount, (LPCTSTR)strOrdNo);
		}

		m_pSellGrid->RedrawRow(nCount);
	}

	if (m_nCurOrder < m_arrSendData.GetSize())
		SendOrder(FALSE);
	else
	{
		m_nCurOrder = 0;
		m_arrSendData.RemoveAll();
	}
}

void CSellWnd::OnGroupSelEndOk()
{
	GroupListComplete();
}

void CSellWnd::GroupListComplete()
{
	CString strGroupID(_T(""));
	strGroupID.Format("%03d", m_pGroupID->GetItemData(m_pGroup->GetCurSel()));
	m_pGroupID->SetCurSel(m_pGroup->GetCurSel());
	//m_pGroupID->GetWindowText(strGroupID);
	m_pParent->QueryGroupAccount(strGroupID);

	OnRemain();
}

void CSellWnd::OnPassSet()
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 2), (LPARAM)_T("IB0000A1 /p5 /S"));
}

bool CSellWnd::IsNumber(CString str)
{
    BOOL bRet = TRUE;
    const int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = FALSE;
            break;
        }
    }
    return bRet;
}

int CSellWnd::GetCheckOrder(int index)
{
	int cnt = 0;
	const int nRows = m_pSellGrid->GetNumberRows();
	for (int i = 0; i < nRows; i++)
	{
		if (m_pSellGrid->IsChecked(i))
		{
			cnt++;
			if (cnt == index)
			{
				return i;
				break;
			}
			
		}
	}
	return 0;
}


void CSellWnd::ChangeSize(double* lRate, int ipoint, LPCTSTR lszname)
{
	CFont* pFont = m_pParent->GetNormalFont();
	CRect wrc, rc;
	GetClientRect(&rc);

	int iw=0, ih=0, itop=0, ileft=0;
	itop = CTRL_SPACE;//max(3 + (ipoint - 10), 3);

	m_pGroupStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pGroupStatic->SetFont(pFont);
	m_pGroupStatic->MoveWindow(wrc.left, itop, wrc.left + iw, ih );
	ileft = wrc.left + iw + CTRL_SPACE;
	
	m_pGroup->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pGroup->SetFont(pFont);
	m_pGroup->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + 1;

	m_pGroupID->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pGroupID->SetFont(pFont);
	m_pGroupID->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + CTRL_SPACE;
	
	m_pGroupID->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pGroupID->SetFont(pFont);
	m_pGroupID->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + CTRL_SPACE;

	m_pAddAccount->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pAddAccount->SetFont(pFont);
	m_pAddAccount->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + BUTTON_SPACE;
	
	m_pPassSet->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pPassSet->SetFont(pFont);
	m_pPassSet->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + CTRL_SPACE;
	

	int ibtnw[4]{};
	m_pClear->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	ibtnw[0] = iw;

	m_pRemain->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	ibtnw[1] = iw;

	m_pOrder->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	ibtnw[2] = iw;

	m_pContract->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	ibtnw[3] = iw;

	ileft = rc.right - (ibtnw[0] + ibtnw[1] + ibtnw[2] + ibtnw[3]) - (BUTTON_SPACE*4) - CTRL_SPACE;
	m_pClear->SetFont(pFont);
	m_pClear->MoveWindow(ileft, itop, ibtnw[0], ih );
	ileft +=  ibtnw[0] + BUTTON_SPACE;

	m_pRemain->SetFont(pFont);
	m_pRemain->MoveWindow(ileft, itop, ibtnw[1], ih );
	ileft +=  ibtnw[1] + BUTTON_SPACE;

	m_pOrder->SetFont(pFont);
	m_pOrder->MoveWindow(ileft, itop, ibtnw[2], ih );
	ileft +=  ibtnw[2] + BUTTON_SPACE;

	m_pContract->SetFont(pFont);
	m_pContract->MoveWindow(ileft, itop, ibtnw[3], ih );
//	ileft +=  ibtnw[3] + igap;

	itop += (ih + CTRL_SPACE);
	m_pCodeCtrl->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);

	ileft = wrc.left ;
	m_pCodeCtrl->SetFont(pFont);
	m_pCodeCtrl->MoveWindow(ileft, itop, iw, ih );
	ileft += iw;

	m_pCodeSearch->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pCodeSearch->SetFont(pFont);
	m_pCodeSearch->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+1);

	m_pSymbol->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pSymbol->MoveWindow(ileft, itop, iw, ih );
	m_pSymbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, ipoint), (LPARAM)lszname);
	ileft += (iw+13);

	m_pTypeStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pTypeStatic->SetFont(pFont);
	m_pTypeStatic->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);

	m_pType->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pType->SetFont(pFont);
	m_pType->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);

	m_pCondition->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pCondition->SetFont(pFont);
	m_pCondition->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+13);


	m_pQuantityStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pQuantityStatic->SetFont(pFont);
	m_pQuantityStatic->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);


	m_pQuantity->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pQuantity->SetFont(pFont);
	m_pQuantity->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+13);


	m_pCostStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pCostStatic->SetFont(pFont);
	m_pCostStatic->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);

	m_pCostType->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pCostType->SetFont(pFont);
	m_pCostType->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);

	m_pCost->GetWindowRect(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);

	const int iright = rc.Width() - ileft - CTRL_SPACE; 
	m_pCost->SetFont(pFont);
	m_pCost->MoveWindow(ileft, itop, iright, ih );
	ileft += (iw+CTRL_SPACE);


	itop += (ih + CTRL_SPACE);
	ih = rc.Height()  - itop - 2;
	m_pSellGrid->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left ;
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );

	
	m_pSellGrid->MoveWindow(ileft, itop, iw, ih);
	m_pSellGrid->ChangeFont(pFont, lRate);
	
}


void CSellWnd::SetPallete()
{
	m_pGroupStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pGroupStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pCostStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pCostStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pSellGrid->SetPallete(m_pParent);
	Invalidate();
}

void CSellWnd::SetCost( CString strCost )
{
	m_pCostType->SetCurSel(0);
	m_pParent->FormatThousand(strCost);
	m_pCost->SetWindowText(strCost);
}
