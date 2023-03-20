// SiseWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ib201600.h"
#include "SiseWnd.h"
#include "MainWnd.h"
#include "ViewWnd.h"
#include "Price.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSiseWnd

CSiseWnd::CSiseWnd(CViewWnd* parent, CWnd* mainWnd, int hogatype, int col, int row)
{
	m_pParent  = (CViewWnd*)parent;
	m_pMainWnd = mainWnd;
	m_pHogaWnd = nullptr;
	m_mapKey   = -1;

	m_code     = nullptr;
//	m_hiCodectrl = nullptr;
	m_hiSymbol	= nullptr;
	m_pwndSymbol = nullptr;
	m_SearchWordArray.RemoveAll();
	m_szCode= "";
	m_price = nullptr;
	
	m_iRowSiseWnd = -1;
	m_iColSiseWnd = -1;
	m_iHogaType = hogatype;			//����Ʈ 10 ȣ��

	m_bPercent = FALSE;	

	setIndexSiseWnd(col, row);
	EnableAutomation();
}

CSiseWnd::CSiseWnd(CViewWnd* parent, CWnd* mainWnd)
{
	m_pParent = (CViewWnd*)parent;
	m_pMainWnd = mainWnd;
	m_pHogaWnd = nullptr;
	m_mapKey   = -1;

	m_code     = nullptr;
//	m_hiCodectrl = nullptr;
	m_hiSymbol	= nullptr;
	m_pwndSymbol = nullptr;
	m_SearchWordArray.RemoveAll();
	m_szCode= "";
	
	m_iRowSiseWnd = -1;
	m_iColSiseWnd = -1;
	m_iHogaType = -1;			//����Ʈ 10 ȣ��
	EnableAutomation();
}

CSiseWnd::~CSiseWnd()
{
}

void CSiseWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSiseWnd, CWnd)
	//{{AFX_MSG_MAP(CSiseWnd)
	ON_WM_CREATE()
	ON_CBN_SELENDOK(IDC_CODECTRL, OnSelendokComboSearch)
	ON_BN_CLICKED(IDC_SEARCHJM, OnButtonJongmok)
	ON_BN_CLICKED(IDC_BTN_MAESU, OnButtonMaesu)
	ON_BN_CLICKED(IDC_BTN_MAEDO, OnButtonMaedo)
	ON_MESSAGE(IDC_RANGE_PRICE, OnRangePrice)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+21,OnPriceRange)
	ON_MESSAGE(WM_MANAGE, OnManage)
	ON_BN_CLICKED(IDC_CODECTRL, OnHistoryCode)

END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CSiseWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CSiseWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ISiseWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {98CBF053-C703-4615-ABA0-89C5943F7CBA}
static const IID IID_ISiseWnd =
{ 0x98cbf053, 0xc703, 0x4615, { 0xab, 0xa0, 0x89, 0xc5, 0x94, 0x3f, 0x7c, 0xba } };

BEGIN_INTERFACE_MAP(CSiseWnd, CWnd)
	INTERFACE_PART(CSiseWnd, IID_ISiseWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSiseWnd message handlers

int CSiseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	init();
	
	return 0;
}

void CSiseWnd::getIndexSiseWnd(int col, int row)
{
	row = m_iRowSiseWnd;
	col = m_iColSiseWnd;
}

void CSiseWnd::setIndexSiseWnd(int col, int row)
{
	m_iRowSiseWnd = row;
	m_iColSiseWnd = col;
}

void CSiseWnd::SendHogaWnd()
{
	CString strProc;
	
	strProc.Format("change_code\t%s", m_szCode);
	
	if (m_code->GetEditData() != "")
		m_pParent->GetParent()->Procedure(strProc, m_mapKey);
}

void CSiseWnd::SendChangeTab(int index)
{
	CString strProc;
	
	strProc.Format("change_tab\t%d", index);

	if (m_code->GetEditData() != "" && m_iHogaType == HOGATYPE5)
		m_pParent->GetParent()->Procedure(strProc, m_mapKey);
}

void CSiseWnd::SetCode(CString code)
{
	if(code.GetLength() == 6)
	{
		m_code->SetEditData(code);
		m_code->SetHistory(code);
		m_szCode = code;
		m_code->SetHistory(Variant(historyCC, _T("1301")));

		m_pParent->CallSnapShot();
	
		SetTimer(1000, 400, nullptr);
	}
	else
	{
		m_code->SetEditData("");
	}
}

void CSiseWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1000)
	{
		KillTimer(1000);
		sendSymbol(m_szCode);
		SendHogaWnd();
		m_price->SetCode(m_szCode);
	}
	CWnd::OnTimer(nIDEvent);
}

//2016.09.19 KSJ �����丮 �������� �� �߰�
void CSiseWnd::OnHistoryCode()
{
	m_code->SetHistory(Variant(historyCC, "1301"));
}

void CSiseWnd::init()
{
	CRect rc, clientRC;
	m_font = getAxFont(_T("����ü"), 9);
	m_strPal = GetPalette();
	CString	imgPath; 

	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");

	imgPath.Format("%s/%s/", m_root, IMAGEDIR);

	GetClientRect(&clientRC);

	rc.top = clientRC.top + gap5;
	rc.bottom = rc.top + comboH;
	rc.left = clientRC.left + gap5;
	rc.right = rc.left + comboH + 60;

	//����˻�
	m_code = std::make_unique<CfxCodeCtrl>(this, CString(m_root + "\\tab"));
	if (m_code->Create(this, rc, IDC_CODECTRL))
	{
		m_code->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		m_code->SetUnit(GU_CODE);
		m_code->SetFont(m_font);
	}
	m_code->MoveWindow(rc);
	m_code->ShowWindow(SW_SHOW);

	CString text = Variant(historyCC, _T("1301"));

	if (m_code)	m_code->SetHistory(text);

 	if (text.GetLength() > 6)
 	{
 		m_szCode = text.Left(6);
// 		if (m_code)	m_code->SetEditData(m_szCode);
 	}

	//���� �˻�
	rc.left = rc.right;// + gap1;
	rc.right = rc.left + 20;

	m_map_nm = getAxBitmap(imgPath + "�˻�.bmp")->operator HBITMAP();
	m_map_dn = getAxBitmap(imgPath + "�˻�_dn.bmp")->operator HBITMAP();
	m_map_hv = getAxBitmap(imgPath + "�˻�_en.bmp")->operator HBITMAP();
	
	m_btnJM = std::make_unique<CfxImgButton>();
	m_btnJM->Create("", rc, this, IDC_SEARCHJM);
	m_btnJM->SetFont(m_font, false);  
	m_btnJM->SetImgBitmap(m_map_nm, m_map_dn, m_map_hv);

	//����� �ɺ�
	rc.left = rc.right + gap3;
	//rc.right = rc.left + 130;
	rc.right = rc.left + 110;

// 
	InitSymbol(rc , m_iRowSiseWnd, m_iColSiseWnd);

	rc.left = rc.right + gap3;
	rc.right = rc.left + 20;

	m_price = std::make_unique<CPrice>(m_pMainWnd);
	m_price->m_sRoot = m_root;
	//m_price->m_pParent = this;
	m_price->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE, rc, this, IDC_RANGE_PRICE);
	m_price->createPrice(m_iColSiseWnd,m_iRowSiseWnd);

	rc.left = rc.right + gap5;
	rc.right = rc.left + 38;

	m_map2_nm = getAxBitmap(imgPath + "�̹���_�ż�.bmp")->operator HBITMAP();
	m_map2_dn = getAxBitmap(imgPath + "�̹���_�ż�_dn.bmp")->operator HBITMAP();
	m_map2_hv = getAxBitmap(imgPath + "�̹���_�ż�_en.bmp")->operator HBITMAP();

	m_btnMaesu = std::make_unique<CfxImgButton>();
	m_btnMaesu->Create("", rc, this, IDC_BTN_MAESU);
	m_btnMaesu->SetFont(m_font, false);  
	m_btnMaesu->SetImgBitmap(m_map2_nm, m_map2_dn, m_map2_hv);		

	rc.left = rc.right + gap2;
	rc.right = rc.left + 38;

	m_map2_nm = getAxBitmap(imgPath + "�̹���_�ŵ�.bmp")->operator HBITMAP();
	m_map2_dn = getAxBitmap(imgPath + "�̹���_�ŵ�_dn.bmp")->operator HBITMAP();
	m_map2_hv = getAxBitmap(imgPath + "�̹���_�ŵ�_en.bmp")->operator HBITMAP();

	m_btnMaedo = std::make_unique<CfxImgButton>();
	m_btnMaedo->Create("", rc, this, IDC_BTN_MAEDO);
	m_btnMaedo->SetFont(m_font, false);  
	m_btnMaedo->SetImgBitmap(m_map2_nm, m_map2_dn, m_map2_hv);


	//ȣ��â
	rc.top = clientRC.top + gapHoga;
	rc.bottom = rc.top + HOGA_HEIGHT;
	rc.left = clientRC.left + gap5;
	rc.right = rc.left + HOGA_WIDTH;

	//�巡�� �� ��� ���
	//CoInitialize(nullptr);
	//AfxOleInit(); 

	m_dropTarget.Register(this);
	m_dropTarget.GetSiseWnd(this);

	m_pHogaWnd = std::make_unique<CHogaWnd>(this, m_pMainWnd);
	m_pHogaWnd->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rc, this, IDC_HOGAWND);
	m_mapKey = m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(attachMAP, vtypeNRM), (LPARAM)m_pHogaWnd.get());

	showHoga(m_iHogaType);
}

void CSiseWnd::InitSymbol(CRect rect, int Row, int Col)
{
	// ���� symbol
	CString	text;
	CWnd* (APIENTRY * axCreate)(CWnd*, void*) = nullptr;
	
	CString indexInfo;
	int iInfo;
	indexInfo.Format("%d%d", Col, Row);
	iInfo = atoi(indexInfo);

	text.Format("%s\\dev\\CX_SYMBOL.DLL", m_root);
	m_hiSymbol = AfxLoadLibrary(text);
	if (m_hiSymbol != nullptr)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
		if (axCreate != nullptr)
		{
			symbolparam.key   = iInfo;
			symbolparam.name  = _T("17413");
			symbolparam.rect  = rect;
			symbolparam.fonts = "����ü";
			symbolparam.point = 9;
			symbolparam.style = 1;
			symbolparam.tRGB  = 130;
			symbolparam.pRGB  = 90;
			symbolparam.options = _T("/a89/b91/c92/d69/i99");
			
			m_pwndSymbol = (*axCreate)(m_pMainWnd, &symbolparam);
			if (m_pwndSymbol == nullptr)
			{
				AfxFreeLibrary(m_hiSymbol);
				m_hiSymbol = nullptr;
			}
			
			m_pwndSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);	
			//���ȴ� �ö���� �ɺ� ������� ���� �ذ�
			m_pwndSymbol->SetParent(this);			
		}
	}
}
/*
void CSiseWnd::InitCodeCtrl(CRect rect)
{
	// ���� symbol
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*);
	
	text.Format("%s\\dev\\cx_codectrl.dll", m_root);
	m_hiCodectrl = AfxLoadLibrary(text);
	if (m_hiCodectrl != nullptr)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiCodectrl, _T("axCreate"));
		if (axCreate != nullptr)
		{
			codeparam.key   = 0;
			codeparam.name  = _T("1301");
			codeparam.rect  = rect;
			codeparam.fonts = "����ü";
			codeparam.point = 9;
			codeparam.style = 1;
			codeparam.tRGB  = 130;
			codeparam.pRGB  = 90;
			codeparam.options = _T("/u�ֽ�/k00001003/l00001003/ptrue/sfalse/otrue/qfalse");
			
			m_code = (*axCreate)(m_pMainWnd, &codeparam);
			m_code->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			
			//���ȴ� �ö���� �ɺ� ������� ���� �ذ�
			m_code->SetParent(this);
			
			if (m_code == nullptr)
			{
				AfxFreeLibrary(m_hiCodectrl);
				m_hiCodectrl = nullptr;
			}
		}
	}
}

void CSiseWnd::InitCategory(CRect rect)
{
	// ���� symbol
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*);
	
	text.Format("%s\\dev\\CX_CATEGORY.dll", m_root);
	m_hiCategory = AfxLoadLibrary(text);
	if (m_hiCategory != nullptr)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiCategory, _T("axCreate"));
		if (axCreate != nullptr)
		{
			cateparam.key   = 0;
			cateparam.name  = _T("ctl_Category");
			cateparam.rect  = rect;
			cateparam.fonts = "����ü";
			cateparam.point = 9;
			cateparam.style = 1;
			cateparam.tRGB  = 69;
			cateparam.pRGB  = 68;
			cateparam.options = _T("/k89/i91/t92/r69");
			
			m_pwndCategory = (*axCreate)(m_pMainWnd, &cateparam);
			m_pwndCategory->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			
			//���ȴ� �ö���� �ɺ� ������� ���� �ذ�
			m_pwndCategory->SetParent(this);
			
			if (m_pwndCategory == nullptr)
			{
				AfxFreeLibrary(m_hiCategory);
				m_hiCategory = nullptr;
			}
		}
	}
}
*/

void CSiseWnd::OnButtonJongmok()
{
	CPoint pt(152, 50);
	ClientToScreen(&pt);
	CString nameS = GetAxCode(allCODE,MAKELPARAM(pt.x, pt.y));
	CString temp = Parser(nameS, "\t");

	if (m_code && nameS != "")
	{
		m_szCode = temp;
		m_code->SetEditData(m_szCode);
		OnSelendokComboSearch();
		
	}

	m_code->SetEditData(m_szCode);
	m_code->SetFocus();
}

void CSiseWnd::OnButtonMaesu()
{
	CString str;
	str.Format("IB101200 /t/s/p5/d1301\t%s\n", m_szCode);	

	m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)str);
}

void CSiseWnd::OnButtonMaedo()
{
	CString str;
	str.Format("IB101100 /t/S/p5/d1301\t%s\n", m_szCode);	

	m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)str);
}

void CSiseWnd::showHoga(int hogatype)
{
	m_iHogaType = hogatype;
	loadHoga(m_iHogaType, m_pHogaWnd.get());
}


void CSiseWnd::loadHoga(int hogatype, CHogaWnd* hogaWnd)
{
	CString mapname;

	mapname = (hogatype == HOGATYPE10) ? currMAP : currMAP2;
	if (m_mapKey != WK_NONE)
	{
		if (m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_mapKey), (LPARAM)mapname.GetString()))
		{
//			hogaWnd->Invalidate();
		}
	}
}

void CSiseWnd::closeMap()
{
	if (m_mapKey != -1 && m_mapKey != WK_NONE)
	{
		m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_mapKey), 0);
		m_mapKey = -1;
	}
}


COLORREF CSiseWnd::GetColor(int color)
{
	if (color & 0x02000000)
		return color;
	
	return SendAxisMessage( MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}


CString	CSiseWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}


CString	CSiseWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	CString strPath = Variant(homeCC, "");
	
	file.Format("%s\\%s\\%s", strPath, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CSiseWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC	backdc;
	backdc.CreateCompatibleDC(&dc); 
	CBitmap bitmap,  *pOldBitmap;
	CRect rectClient;
	GetClientRect(rectClient);
	
	bitmap.CreateCompatibleBitmap(&dc, rectClient.right, rectClient.bottom);
	pOldBitmap = (CBitmap*)backdc.SelectObject(&bitmap);
	backdc.SetBkMode(OPAQUE);
	
	backdc.FillSolidRect( rectClient, GetColor(181)); 
	CFont* pOldFont = backdc.SelectObject(m_font);	
	
	Draw(&backdc, rectClient);	
	dc.BitBlt( rectClient.left , rectClient.top , rectClient.right , rectClient.bottom , &backdc, 0,0, SRCCOPY );
	bitmap.DeleteObject(); 
	backdc.SelectObject(pOldBitmap);
	backdc.SelectObject(pOldFont); 
	DeleteObject(pOldFont);
	DeleteObject(pOldBitmap);
	ReleaseDC(&backdc);	
	// Do not call CWnd::OnPaint() for painting messages
}

void CSiseWnd::Draw(CDC *pDC, CRect rcClient)
{
	CRect rect;

	//background
	pDC->FillSolidRect(&rcClient,GetColor(64));//64
	
	rect.top = rcClient.top + gap2;
	rect.bottom = rcClient.bottom - gap2;
	rect.left = rcClient.left + gap2;
	rect.right = rcClient.right - gap2;

 	pDC->FillSolidRect(&rect,GetColor(181));
	DrawRoundBitmap(pDC, rect, ROUND_CONTENTS);
}


void CSiseWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}


void CSiseWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != nullptr)
	{
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

BOOL CSiseWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	switch (LOWORD(wParam))
// 	{
// 	}
	return CWnd::OnCommand(wParam, lParam);
}


void CSiseWnd::OnSelendokComboSearch() 
{
	CString strProc, strParam;
	
	CString preCode = m_szCode;

	m_szCode = m_code->GetEditData();
// 	m_szCode.TrimLeft();
// 	m_szCode.TrimRight();
//  	AfxMessageBox(m_szCode);
//  	CString codeName = Variant(nameDLL,m_szCode);
// 	
// 	AfxMessageBox(codeName);
// 
// 	if(codeName == "" || codeName == nullptr)
// 	{
// 		m_code->SetEditData(preCode);
// 	
// 		return;
// 	}
	
	if(m_szCode.GetLength() >= 6)
	{	
			m_szCode = m_szCode.Left(6);
			m_code->SetEditData(m_szCode);
			m_code->SetHistory(Variant(historyCC, _T("1301")));
			
			//���� ����ø��� �������� ����
			//m_pParent->saveCodeInfo(m_pParent->m_nBtnIndex);
			m_code->SetEditData(m_szCode);
			m_pParent->CallSnapShot();
	}

	m_price->SetCode(m_szCode);
	sendSymbol(m_szCode);
	
	strProc.Format("change_code\t%s", m_szCode);
	m_pParent->GetParent()->Procedure(strProc, m_mapKey);

	m_code->SetFocus();
}

CString	CSiseWnd::GetCode()
{
	return m_szCode;
}

CString CSiseWnd::Parser(CString &srcstr, CString substr)
{
	CString temp;
	
	if (srcstr.Find(substr) == -1)
	{
		temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	
	return "";
}

void CSiseWnd::RequestJMInfo()
{
	if(m_code)
	{
		m_szCode = m_code->GetEditData();
		m_szCode.TrimLeft();
		m_szCode.TrimRight();
		m_szCode.Remove(0);
	}
	
}

void CSiseWnd::sendSymbol(CString code)
{
	CString sDat, indexInfo;
	int	iInfo{};
	char	key{};
	_trkey* trkey = (struct _trkey*)&key;
	//	trkey->group =m_iIndexSiseWnd;
	trkey->kind = type_symbol;
	
	//�࿭�� �������·� ��� ������
	indexInfo.Format("%d%d", m_iColSiseWnd, m_iRowSiseWnd);
	iInfo = atoi(indexInfo);

	sDat.Format("1301%c%s\t1021\t17413\t", 0x7f, m_szCode);
	sendOOP(iInfo, (char *)sDat.operator LPCTSTR(), sDat.GetLength());
}

void CSiseWnd::sendHoga(CString code)
{
	CString sDat, indexInfo;
	int iInfo{};
	sDat.Format("1301%c%s\t1021\t17412\t", 0x7f, m_szCode);

	char	key{};
	_trkey* trkey = (struct _trkey*)&key;

	//�࿭�� �������·� ��� ������
	indexInfo.Format("%d%d", m_iColSiseWnd, m_iRowSiseWnd);
	iInfo = atoi(indexInfo);

	trkey->group = iInfo;
	trkey->kind = type_hoga;
}

void CSiseWnd::sendOOP(int key, char* datb, int datl)
{
	struct	_userTH* uTH{};	
	auto wb = std::make_unique<char[]>(L_userTH+datl);				
	uTH = (struct _userTH *)wb.get();

	CopyMemory(uTH->trc, "POOPPOOP", sizeof(uTH->trc));
	uTH->key  = key;
	uTH->stat = US_PASS;

	CopyMemory(&wb[L_userTH], datb, datl);
	m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)wb.get());

	if (m_code)
	{
		CString data;
		data = m_code->GetEditData();
		data.TrimLeft(); 
		data.TrimRight();
		data.Remove(0);

		if (!data.IsEmpty() && data.GetLength() == 6 && Variant(whichCC, data))
		{
			data.Insert(0, "1301\t");
			Variant(codeCC, data);
			Variant(triggerCC, data);
		}
		else	
			m_code->SetEditData(_T(""));
	}
}


void CSiseWnd::parsingSymbol(char* datb)
{
	int	value{};
	CString	text;
	
	text  = CString(datb);


	if(text.GetAt(0) == 'A' || text.GetAt(0) == 'Q')  //20200430 ETN �߰�
	{
		value = text.Find('\t');
		if (value != -1)
		{
			WPARAM	wParam{};
			CString tempCode;
			tempCode  = text.Left(value++);

			if( tempCode.GetLength() ==7 )
			{
				text   = text.Mid(value);
			
				struct	_extTHx exdata {};
				exdata.size = text.GetLength();
				exdata.key  = 20;
				exdata.data = text.GetBuffer();

				wParam = MAKEWPARAM(MAKEWORD(DLL_OUBx, 20), (text.GetLength()));

				if (m_pwndSymbol)
					m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)&exdata);
			}
			else
			{
	//			text   = text.Mid(value);

				struct	_extTHx exdata {};
				exdata.size = text.GetLength();
				exdata.key  = 20;
				exdata.data = text.GetBuffer();

				wParam = MAKEWPARAM(MAKEWORD(DLL_OUBx, 20), text.GetLength());
				if (m_pwndSymbol)
					m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)&exdata);
			}
		}
	}
	else
	{
		m_price->SetValues(text);
	}


	m_code->SetEditData(m_szCode);
	m_code->SetFocus();
}


void CSiseWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	m_dropTarget.Revoke();		
	closeMap();
}


void CSiseWnd::parsingHoga(char* datb)
{
	int	value{};
	CString	text;
	
	text  = CString(datb);
	value = text.Find('\t');
	if (value != -1)
	{
		WPARAM	wParam{};
		CString tempCode;
		tempCode  = text.Left(value++);
		if( tempCode.GetLength() ==7 )
		{
			text   = text.Mid(value);
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), (text.GetLength() - 13));
			m_pwndHoga->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		}
	}
}

CString CSiseWnd::GetAxCode(int cmd,LPARAM lParam)
{
	if( !m_pMainWnd )
		return "";
	
	char* dta = (char*)m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(cmd, nullptr)), lParam);
	if ((long)dta > 1)	return dta;
	else			return "";
}

LONG CSiseWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	const LONG	ret = 0;
	
	switch (LOWORD(wParam))
	{
	case WM_COMBO_ENTERED :
		
		break;
	}
	return ret;
}


CFont* CSiseWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;
	
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)SendAxisMessage( getFONT, (long)&fontR);
}

CBitmap* CSiseWnd::getAxBitmap(CString path)
{
	return (CBitmap*)m_pMainWnd->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}


LRESULT CSiseWnd::SendAxisMessage(WPARAM wParam, LPARAM lPAram)
{	
	return m_pMainWnd->SendMessage(WM_USER, wParam, lPAram);
}


void CSiseWnd::InitHoga(CRect rect)
{
/*
	// ���� Hoga
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*);
	
	text.Format("%s\\dev\\CX_Depth.dll", m_root);
	m_hiHoga = AfxLoadLibrary(text);
	if (m_hiHoga != nullptr)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiHoga, _T("axCreate"));
		if (axCreate != nullptr)
		{
			hogaparam.key   = 0;
			hogaparam.name  = _T("17412");
			hogaparam.rect  = rect;
			hogaparam.fonts = "����ü";
			hogaparam.point = 9;
			hogaparam.style = 1;
			hogaparam.tRGB  = 232;
			hogaparam.pRGB  = 193;
			hogaparam.options = _T("/u�ֽ�/d10ȣ��/pNone/bTrue/sText/lTrue/cTrue/hboth/oTrue/gTrue/xFalse/a�ֹ��˾�â/wFalse/fIB201600/r/yFalse/vFalse");
			
			m_pwndHoga = (*axCreate)(m_pMainWnd, &hogaparam);
			m_pwndHoga->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			
			//���ȴ� �ö���� �ɺ� ������� ���� �ذ�
			m_pwndHoga->SetParent(this);
			
			if (m_pwndHoga == nullptr)
			{
				AfxFreeLibrary(m_hiHoga);
				m_hiHoga = nullptr;
			}
		}
	}
*/
}

LRESULT CSiseWnd::OnRangePrice(WPARAM wParam, LPARAM lParam)
{
	m_price->BtnClick();
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COleEditDropTarget

CDropTarget::CDropTarget() {}

CDropTarget::~CDropTarget() {}  

//
// OnDragEnter is called by OLE dll's when drag cursor enters
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT CDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{   
	// Check if the control key was pressed          
	if (!pDataObject->IsDataAvailable(CF_TEXT))
	{
		return DROPEFFECT_NONE;
	}
	
	// if the control key is held down, return a drop effect COPY 
	if((dwKeyState&MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_COPY;
	// Otherwise return a drop effect of MOVE
	else
		return DROPEFFECT_MOVE;   
} 

//
// OnDragLeave is called by OLE dll's when drag cursor leaves
// a window that is REGISTERed with the OLE dll's
//
void CDropTarget::OnDragLeave(CWnd* pWnd)
{
//	return;
	// Call base class implementation
	COleDropTarget::OnDragLeave(pWnd);
}

// 
// OnDragOver is called by OLE dll's when cursor is dragged over 
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT CDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

	if((dwKeyState&MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_NONE;  
	else
		return DROPEFFECT_COPY;  // move source
}
	
// 
// OnDrop is called by OLE dll's when item is dropped in a window
// that is REGISTERed with the OLE dll's
//
BOOL CDropTarget::OnDrop(CWnd* pWnd, COleDataObject * pDataObject, DROPEFFECT dropEffect, CPoint point )
{         
	HGLOBAL  hGlobal{};
	LPCSTR   szBuffer{};
	
	// Get text data from COleDataObject
	 if(dropEffect == DROPEFFECT_NONE)
		return FALSE;	
	
	if(pDataObject->GetGlobalData(CF_TEXT))
	{
		// Get pointer to data
		hGlobal=pDataObject->GetGlobalData(CF_TEXT);

		szBuffer = (LPCSTR)GlobalLock(hGlobal);    
		ASSERT(szBuffer!=nullptr);

		GlobalUnlock(hGlobal);	

	}

	CString strText = szBuffer;
	CString str1301, strCode;

	strCode.Empty();str1301.Empty();
	str1301 = Parser(strText,"\t");



	if(str1301.Find("1301") != -1)
	{
 		strCode = Parser(strText, "\t");
	}
	else
	{
			for(int ii=0 ; ii<10 ; ii++)
			{
				str1301 = Parser(strText, "\t");
				str1301.TrimRight();
				
				if(strText.GetLength() <=0)
					break;

				if(str1301.Find("1301") != -1)
				{
					strCode = Parser(strText, "\t");
					break;
				}
				
			}
	}

	strCode.TrimRight();
	CString sendCode;
	
	
	if (!strCode.IsEmpty() && str1301.Find("1301") >= 0)
	{
//		sendCode.Format("1301\t%s", strCode);
		m_pSiseWnd->SetCode(strCode);
		//���� 1301�� �����ڵ� �ְ� �Լ� ����
 		return TRUE;
	}

	return FALSE;
	
}

CString CDropTarget::Parser(CString &srcstr, CString substr)
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

void CDropTarget::GetSiseWnd(CSiseWnd* cWnd)
{
	m_pSiseWnd = cWnd;
}
/////////////////////////////////////////////////////////////////////////////
// 


void CSiseWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDown(nFlags, point);
}

LONG CSiseWnd::OnPriceRange( WPARAM wParam, LPARAM lParam )
{
	//AfxMessageBox((char*)lParam);
	CString str = (char*)lParam;

	if(m_bPercent)
	{
		return 0;
	}
	else
	{
		CString strPath;
		strPath.Format("%s\\%s\\%s\\IB201600.ini", m_root, USRDIR, m_name);
		WritePrivateProfileString((LPCTSTR)m_code.get(), "DRAG", str, (LPCTSTR)strPath);
	}

	CString strProc;
	strProc.Format("changerange\t%s",str);
	//AfxMessageBox(strProc);
	m_pParent->GetParent()->Procedure(strProc, m_mapKey);

	return 0;
}

void CSiseWnd::SetRangePercent( int nHigh,int nLow )
{
	CString strProc;
	strProc.Format("changepercent\t%d,%d",nHigh,nLow);
	//AfxMessageBox(strProc);
	m_pParent->GetParent()->Procedure(strProc, m_mapKey);

	if(nHigh == 0 && nLow == 0)
		m_bPercent = FALSE;
	else
		m_bPercent = TRUE;	
}

void CSiseWnd::SetEnablePrice( bool flag )
{
	m_price->EnableWindow(flag);
}

void CSiseWnd::LoadPriceRange()
{
	if(m_bPercent)
	{
		return;
	}
	else
	{
		CString strPath;
		strPath.Format("%s\\%s\\%s\\IB201600.ini", m_root, USRDIR, m_name);
		
		char buff[64];
		CString str;
		
		GetPrivateProfileString("RANGE", "PERCENTF", "", buff, sizeof(buff), (LPCTSTR)strPath);

		str = buff;

		CString strProc;
		strProc.Format("changerange\t%s",str);
		//AfxMessageBox(strProc);
		m_pParent->GetParent()->Procedure(strProc, m_mapKey);
	}
}
