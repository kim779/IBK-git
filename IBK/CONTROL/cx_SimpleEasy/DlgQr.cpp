// DlgQr.cpp : implementation file
//

#include "stdafx.h"
#include "cx_SimpleEasy.h"
#include "DlgQr.h"
#include "socket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgQr dialog

#define DF_HEIGHT	405
#define DF_WEIGHT	540

#define REMAIN_TIME	180

#define DF_BTN_CLOSE	9001
#define DF_BTN_INIT	9002
#define DF_BTN_OTHER	9003

DlgQr::DlgQr(CWnd* pParent /*=NULL*/)
	: CDialog(DlgQr::IDD, pParent)
{
	EnableAutomation();
	
	//{{AFX_DATA_INIT(DlgQr)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_sock = nullptr;
	m_pBrowser = nullptr;
	m_btnClose = nullptr;
	m_btnInit =  nullptr;
	m_btnOther = nullptr;
	m_pPubkey =  nullptr;
//	m_pBmpMain = nullptr;
	m_bChecking = false;

	m_iMheight = (int)(DF_HEIGHT / 9);
	m_iMwidth  = (int)(DF_WEIGHT / 9);

	m_iType = DF_TYPE_LOGIN;
	m_iRemainTime = REMAIN_TIME;
}

DlgQr::DlgQr(int type,CWnd* pParent /*=NULL*/)
	: CDialog(DlgQr::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(DlgQr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_sock = nullptr;
	m_pBrowser = nullptr;
	m_btnClose = nullptr;
	m_btnInit =  nullptr;
	m_btnOther = nullptr;
	m_pPubkey =  nullptr;
//	m_pBmpMain = nullptr;
	m_bChecking = false;
	m_iType = type;
	
	m_iMheight = (int)(DF_HEIGHT / 9);
	m_iMwidth  = (int)(DF_WEIGHT / 9);

	m_iRemainTime = REMAIN_TIME;
}

DlgQr::~DlgQr()
{
	bFont.DeleteObject();
	mFont.DeleteObject();
	sFont.DeleteObject();
	lbFont.DeleteObject();

	m_pBmpMain.Detach();
	m_pBmpMain.DeleteObject();
}

void DlgQr::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void DlgQr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgQr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgQr, CDialog)
	//{{AFX_MSG_MAP(DlgQr)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_BN_CLICKED(DF_BTN_CLOSE, OnExit)
	ON_BN_CLICKED(DF_BTN_INIT, OnQRInit)  
	ON_BN_CLICKED(DF_BTN_OTHER, OnQROther)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 12, OnSockManage)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(DlgQr, CDialog)
	//{{AFX_DISPATCH_MAP(DlgQr)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDlgQr to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D3BD38F3-216F-477C-8BC7-EB07412CB9B9}
static const IID IID_IDlgQr =
{ 0xd3bd38f3, 0x216f, 0x477c, { 0x8b, 0xc7, 0xeb, 0x7, 0x41, 0x2c, 0xb9, 0xb9 } };

BEGIN_INTERFACE_MAP(DlgQr, CDialog)
	INTERFACE_PART(DlgQr, IID_IDlgQr, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgQr message handlers

BOOL DlgQr::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_iType == DF_TYPE_LOGIN)  //로그인창에서 띄울경우 위치 보정
	{
		CRect	rec;

		m_pParent->GetWindowRect(rec);
	//	ClientToScreen(rec);
		::SetWindowPos(m_hWnd, HWND_TOPMOST, rec.left + 50, rec.top, 0, 0, SWP_SHOWWINDOW);
	}

	((CWnd*)GetDlgItem(IDOK))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDCANCEL))->ShowWindow(SW_HIDE);

	CRuntimeClass *pRClass = RUNTIME_CLASS(CAxBrowser);
	m_pBrowser = (CAxBrowser*)pRClass->CreateObject();
	ZeroMemory(m_pdata, szBUFF);

	if (m_sock)
		m_sock.reset();
	
	m_sock = std::make_unique<Csocket>(this);
	m_sIP = "172.16.202.106";  //BP서버에서 한다.
//	m_sIP = "172.16.202.150";  //MP서버에서 한다.
	m_sPort = "15201";

	if (m_sock->Open(m_sIP, atoi(m_sPort)))
	{
		SetTimer(TM_CONNECT, 2000, NULL);
	}
	else
	{
		m_sock.reset();
		CDialog::OnCancel();
	}
	
	m_rcQR = CRect(50, m_iMheight*3, 180, m_iMheight*3 + 130);
	m_rcEnum = m_rcQR;
	m_rcEnum.OffsetRect(DF_WEIGHT/2 + m_iMwidth/2, -(m_iMheight/2));
	if (m_pBrowser->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, m_rcQR, this, 9753))
	{
		CRect	wrec;

		GetWindowRect(wrec);
		MoveWindow(CRect(wrec.left, wrec.top, wrec.left + DF_WEIGHT, wrec.top + DF_HEIGHT), false);
		m_pBrowser->ShowWindow(SW_HIDE);
		initFont();
		initbutton();
		LoadBitmapFromFile("EVENT09.BMP");

		SetTimer(TM_QR, 1000, NULL);
		m_iRemainTime = REMAIN_TIME;
		m_strRemainTime = "3:00";
		
		if (!m_pPubkey)
			m_pPubkey = new UCHAR[1024];
			//m_pPubkey = new PUCHAR[1024];
	}	

	Invalidate();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgQr::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (m_pPubkey)
	{
		delete m_pPubkey;
		m_pPubkey = nullptr;
	}
	if (m_sock)
		m_sock.reset();
	if (m_pBrowser)
	{
		delete m_pBrowser;
		m_pBrowser = nullptr;
	}

	CDialog::OnClose();
}

void DlgQr::initbutton()
{
	CRect	rc;
	CString imagepath;

	imagepath.Format("%s\\image\\%s", m_strpath, "NEW_LOGIN_BTN_CANCEL.bmp");
	rc.SetRect(DF_WEIGHT - 26, 5, DF_WEIGHT - 5, 16);
	m_btnClose = std::make_unique<CShapeButton>();
	m_btnClose->Create("", WS_CHILD | WS_VISIBLE, rc, this, DF_BTN_CLOSE);
	m_btnClose->SetImgBitmap(imagepath, 2);

	imagepath.Format("%s\\image\\%s", m_strpath, "NEW_QR_INIT.bmp");
	rc.SetRect(DF_WEIGHT / 3, m_iMheight * 7 + (m_iMheight / 4), DF_WEIGHT / 3 + (m_iMwidth * 3), m_iMheight * 8 + (m_iMheight / 4));
	m_btnInit = std::make_unique<CShapeButton>();
	m_btnInit->Create("", WS_CHILD | WS_CHILD, rc, this, DF_BTN_INIT);
	m_btnInit->SetImgBitmap(imagepath, 2);

	imagepath.Format("%s\\image\\%s", m_strpath, "NEW_QR_OTHER.bmp");
//	rc.SetRect(m_iMwidth * 6 + (m_iMwidth/3), m_iMheight * 8, m_iMwidth * 9 + (m_iMwidth/3) - 105, m_iMheight * 9));
	rc.SetRect(m_iMwidth * 5 + (m_iMwidth / 2) + 25, m_iMheight * 8 + 5, m_iMwidth * 5 + (m_iMwidth / 2) + 100, m_iMheight * 9 + 5);
	m_btnOther = std::make_unique<CShapeButton>();
	m_btnOther->Create("", WS_CHILD | WS_CHILD, rc, this, DF_BTN_OTHER);
	m_btnOther->SetImgBitmap(imagepath, 2);
//	m_btnOther->ShowWindow(SW_HIDE);
}

void DlgQr::LoadBitmapFromFile(const char* bmpName)
{
	HBITMAP hBitmap;
	CString fileName;
	CString path(bmpName);

	path.MakeUpper();
	fileName.Format("%s\\image\\%s", m_strpath, path);
	
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
		m_pBmpMain.Attach(hBitmap);
}

void DlgQr::initFont()
{
	CString fontName = "고딕";
	
	lbFont.CreateFont(35, 0, 0, 0, FW_BOLD, FALSE,
		false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, fontName);
	
	bFont.CreateFont(14, 0, 0, 0, FW_BOLD, FALSE,
		false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, fontName);
	
	mFont.CreateFont(25, 0, 0, 0, FW_BOLD, FALSE,
		false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, fontName);
	
	const	int	fontS = 90;
	sFont.CreatePointFont(fontS, fontName);
}

void DlgQr::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rec;

	GetClientRect(&rec);
	dc.FillSolidRect(rec, RGB(255,255,255));  //바탕
	
	CFont *pOldFont = (CFont*)dc.SelectObject(&bFont);
	dc.DrawText("\r\n\r\nIBK FARM앱을 통해 간편인증 진행해 주십시요.", CRect(0, 0, DF_WEIGHT, m_iMheight), DT_CENTER);

	/*	
	dc.DrawText("\r\n\r\nIBK FARM앱을 통해 QR코드스캔 또는 인증번호를 입력해 주세요.", CRect(0, 0, DF_WEIGHT, m_iMheight), DT_CENTER);
	dc.DrawText("\r\n    QR스캔", CRect(0, m_iMheight * 2, m_iMwidth * 3, m_iMheight * 3), DT_CENTER);
	dc.DrawText("\r\n    인증번호", CRect(m_iMwidth * 6, m_iMheight * 2, m_iMwidth * 9, m_iMheight * 3), DT_LEFT);
	dc.DrawText("\r\nOR", CRect(m_iMwidth * 3, m_iMheight * 4, m_iMwidth * 6, m_iMheight * 5), DT_CENTER);	
	
	if (m_strEasyTrdNum.GetLength() >= 6)
	{
		dc.SelectObject(&lbFont);
		dc.DrawText("\r\n" + m_strEasyTrdNum.Left(3) + "\r\n" + m_strEasyTrdNum.Mid(3, m_strEasyTrdNum.GetLength() - 3), m_rcEnum, DT_CENTER);	
	}
	
	dc.SelectObject(&sFont);
	dc.DrawText("\r\n(IBK FARM -> 인증센터 -> PC인증)", CRect(0, m_iMheight , DF_WEIGHT, m_iMheight * 2), DT_CENTER);
	*/
	dc.DrawText("\r\n\r\n인증유효시간", CRect(m_iMwidth * 3 , m_iMheight * 6 - (m_iMheight/2), m_iMwidth * 6, m_iMheight * 7 - (m_iMheight/3)), DT_CENTER);
	
	
	dc.SelectObject(mFont);
	dc.DrawText("\r\n" + m_strRemainTime, CRect(m_iMwidth * 3, m_iMheight * 6 , m_iMwidth * 6, m_iMheight * 7 + (m_iMheight/4)), DT_CENTER);	
	//	dc.DrawText("다른인증방법선택 >", CRect(m_iMwidth * 6, m_iMheight * 8, m_iMwidth * 9, m_iMheight * 9), DT_CENTER);
	
	// Do not call CDialog::OnPaint() for painting messages
	
	dc.SelectObject(pOldFont);

	CDC mdc;
	BITMAP bm;

	mdc.CreateCompatibleDC(&dc);
	m_pBmpMain.GetBitmap(&bm);
	CBitmap* oldBmp = mdc.SelectObject(&m_pBmpMain);
	dc.StretchBlt(rec.Width() / 6 , m_iMheight, rec.Width() - (rec.Width()/6 * 2) , m_iMheight * 6 - (m_iMheight/2), &mdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	CPen	cPen(PS_SOLID, 2, RGB(0,0,0));
	CPen*	sPen = dc.SelectObject(&cPen);
	
	dc.MoveTo(rec.left, rec.top);
	dc.LineTo(rec.left, rec.bottom);

	dc.MoveTo(rec.left, rec.bottom);
	dc.LineTo(rec.right, rec.bottom);

	dc.MoveTo(rec.right, rec.bottom);
	dc.LineTo(rec.right, rec.top);

	dc.MoveTo(rec.right, rec.top);
	dc.LineTo(rec.left, rec.top);

	dc.SelectObject(sPen);

	// Do not call CDialog::OnPaint() for painting messages
}

void DlgQr::OnQROther()
{
	if (IDOK == MessageBox("PC인증이 실행중입니다 \r\nPC인증을 취소하시겠습니까?", "확인", MB_OKCANCEL | MB_ICONQUESTION))
	{
		EndDialog(IDCANCEL);
//		OnExit();
	}
}

void DlgQr::OnQRInit()
{
	//	if (IDOK == MessageBox("초기화 하시겠습니까?", "확인", MB_OKCANCEL | MB_ICONQUESTION))
	{

		ZeroMemory(m_pdata, szBUFF);

		KillTimer(TM_QR);
		SetTimer(TM_QR, 1000, NULL);
		m_iRemainTime = REMAIN_TIME;
		SendRegTranByRelay();
	}
}

void DlgQr::OnExit() 
{
	EndDialog(IDCANCEL);
	//PostMessage(WM_CLOSE, 0,0);
}

void DlgQr::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TM_QR:
		m_iRemainTime -= 1;
		m_strRemainTime.Format("%d:%02d", m_iRemainTime/60,m_iRemainTime%60);
		InvalidateRect(CRect(m_iMwidth * 3, m_iMheight * 6, m_iMwidth * 6, m_iMheight * 7 + (m_iMheight/4)));
		if (m_iRemainTime <= 0)
		{
			KillTimer(TM_QR);
			if (IDOK != MessageBox("인증유효시간이 초과하였습니다 \r\n재시도 하시겠습니까?", "확인", MB_OKCANCEL | MB_ICONQUESTION))
			{
				OnExit();
			}
			else
			{
				SetTimer(TM_QR, 1000, NULL);
				m_iRemainTime = REMAIN_TIME;	
			}
		}	
		break;

	case TM_CONNECT:
		KillTimer(nIDEvent);
		if (m_sock)
			m_sock.reset();
		break;

	case TM_RECEIVE:
		KillTimer(nIDEvent);
		if (m_sock)
			m_sock.reset();
		break;
	case TM_CHECKREG:
		SendtagINQUIRYREG();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

//int DlgQr::getPubkey(PUCHAR* pdata)
int DlgQr::getPubkey(PUCHAR pdata)
{
	ATPASS_CONTEXT ctx = { 0x00, };

	ATPASS_InitContext(&ctx);   
	ATPASS_Initialize(&ctx, TRUE);
	
	const	int	rsakeyBlobSize = ATPASS_GetPublicKeyBlobSize(&ctx);
	if (rsakeyBlobSize < 0)
		return 0;

	ULONG	pubKeySize = 0;
	PUCHAR	pubKey = (PUCHAR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, rsakeyBlobSize);
	if (pubKey == nullptr)
		return 0;

	const	HRESULT hr = ATPASS_GetPublicAsn1Primitive(&ctx, pubKey, rsakeyBlobSize, &pubKeySize);
	
	if (!SUCCEEDED(hr))
	{
		AfxMessageBox("[ATPASS] ATPASS_GetPublicAsn1Primitive fail");
		return 0;
	}

	CString strSend, stmp;

	for (int ii = 0 ; ii < (int)pubKeySize; ii++)
	{
		stmp.Format("%02X", pubKey[ii]);
		strSend += stmp;
	}
	
	CopyMemory(pdata, strSend.GetString(), strSend.GetLength());
	HeapFree(GetProcessHeap(), 0, pubKey);
	
	ATPASS_CleanUp(&ctx);
	return pubKeySize;
}

void DlgQr::dispatch(char* pBytes, int nBytes)
{
	CString ecode;
	struct	_axisH* axisH = nullptr;

	pBytes += L_fmH;
	nBytes -= L_fmH;

	axisH = (struct _axisH *)pBytes;
	pBytes += L_axisH;
	nBytes -= L_axisH;
	nBytes = min(nBytes, atoi(CString(axisH->datL, sizeof(axisH->datL))));

	switch (axisH->trxK)
	{
	case DF_ST_TAGREG:   //서명원문등록
		{
			st_OUT_tagREG_PRE* pout = (st_OUT_tagREG_PRE*)&pBytes[80];
			ecode = CString(pout->ERR_CD);
			ecode.TrimRight();
			if (ecode == "000000")
			{
				m_strTrdNum = CString(pout->REL_TRAN_NO);
				m_strTrdNum.TrimRight();
				m_strEasyTrdNum= CString(pout->REL_TRAN_EASY_NO);
				m_strEasyTrdNum.TrimRight();
				ShowQRcode(m_strEasyTrdNum);  
		
				SendtagINQUIRYREG();  
				Invalidate();
			}	
		}
		break;

	case DF_ST_CHKREG:
		{
			tagINQUIRY_REG_TRAN_BY_RELAY_RES* pout = (tagINQUIRY_REG_TRAN_BY_RELAY_RES*)&pBytes[80];
			ecode = CString(pout->ERR_CD);
			ecode.TrimRight();

			if (ecode == "000000")
			{
				if (pout->REG_TRAN_YN[0] == 'Y')	//test
				{	
					KillTimer(TM_CHECKREG);
					SendSrhAuthTrdnum();   //인증거래번호조회
				}
			}
		}
		break;

	case DF_ST_SEARCHTRDNUM:  //인증거래번호조회 
		{
			INQUIRY_AUTH_TRAN_NO_BY_RELAY_RES* pout = (INQUIRY_AUTH_TRAN_NO_BY_RELAY_RES*)&pBytes[80];
			ecode = CString(pout->ERR_CD);
			ecode.TrimRight();

		//	SendLoginconfirm();  //test
			if (ecode == "000000")
			{
				m_strAutTrdNum = CString(pout->AUTH_TRAN_NO);
				m_strAutTrdNum.TrimRight();
				SendLoginconfirm();
			}
		}
		break;

	case DF_ST_VEFYLOGIN:  //로그인거래검증 
		{
			VERIFY_LOGIN_TRAN_BY_RELAY_RES* pout = (VERIFY_LOGIN_TRAN_BY_RELAY_RES*)&pBytes[80];
			ecode = CString(pout->ERR_CD);
			ecode.TrimRight();

			if (ecode == "000000" && pout->VRFC_SUCC_YN[0]  == 'Y')
			{
				OnOK();
			}
			else
			{
		//		OnOK(); //test
				ZeroMemory(m_pdata, szBUFF);  //test
			}
		}
		break;

	case DF_ST_VEFYTRADE:  //금융거래검증 
		{
			VERIFY_TRAN_BY_RELAY_RES* pout = (VERIFY_TRAN_BY_RELAY_RES*)&pBytes[80];
			ecode = CString(pout->ERR_CD);
			ecode.TrimRight();	

			if (ecode == "000000" && pout->VRFC_SUCC_YN[0] == 'Y')
			{
				OnOK();
			}
			else
			{
				ZeroMemory(m_pdata, szBUFF);  //test
			}
		}
		break;
	}
}

void DlgQr::ShowQRcode(CString strEnum) 
{
	//QRCODE 파일에 등록시 내려받은 거래번호입력
	CString spath;
	CFile	fileB;

	spath.Format("%s\\%s\\%s", m_strpath, "TAB", "QRCODE_TEST.HTML");
	if (!fileB.Open(spath, CFile::modeRead | CFile::shareDenyNone))
		return;
				
	const	int	len = (int)fileB.GetLength();
	std::unique_ptr<char[]> pdata = std::make_unique<char[]>(len+ 1);

	if (fileB.Read(pdata.get(), len) != len)
	{
		fileB.Close();
		pdata.reset();
		return;
	}
	fileB.Close();

	CString strdata;
	strdata = CString(pdata.get(), len);
	strdata.Replace("replacenum", strEnum);
	pdata.reset();
	
	spath.Format("%s\\%s\\%s", m_strpath, "TAB", "QRCODE.html");
	if (!fileB.Open(spath, CFile::modeWrite|CFile::modeCreate | CFile::shareDenyNone))
		return;

	fileB.Write(strdata, strdata.GetLength());
	fileB.Close();
	
	m_pBrowser->Navigate2("file:///" + spath);
	m_pBrowser->ShowScrollBar(SB_BOTH, FALSE);
}

LONG DlgQr::OnSockManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case sm_CLOSE:
		if (m_sock)
			m_sock.reset();
		break;	

	case sm_CONNECT:
		KillTimer(TM_CONNECT);
		SendRegTranByRelay();  //서명원문등록
		break;

	case sm_RECEIVE:
		KillTimer(TM_CONNECT);
		KillTimer(TM_RECEIVE);
		dispatch((char *)lParam, (int)HIWORD(wParam));
		break;
	}
	
	return 0;	
}

CString DlgQr::GetMacAddr()
{
	IP_ADAPTER_INFO AdapterInfo[16]{};
	DWORD	dwBufLen = sizeof(AdapterInfo);
	const	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);

	m_macaddr.Empty();
	if (dwStatus == ERROR_SUCCESS)
	{
		m_macaddr.Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
			AdapterInfo[0].Address[0],
			AdapterInfo[0].Address[1],
			AdapterInfo[0].Address[2],
			AdapterInfo[0].Address[3],
			AdapterInfo[0].Address[4],
			AdapterInfo[0].Address[5]);
	}
	return m_macaddr;
}

void DlgQr::SendRegTranByRelay()   //서명메시지등록 
{
	m_strTrdNum.Empty();
	m_strEasyTrdNum.Empty();

	if (GetMacAddr().IsEmpty())
	{
		TRACE("[QRCODE] SendRegTranByRelay mac 추출 실패 \n");
	}
//	TRACE("[QRCODE] SendRegTranByRelay mac=[%s]\n", m_macaddr);

	CString	tmps;
	const	int	datl = L_fmH + L_axisH + SZ_COMHEADER + SZ_TAGREGPRE;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl);
	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];
	struct st_comheader* pst_header = (struct st_comheader*)&datb[L_fmH + L_axisH];
	struct st_tagREG_PRE* pst_midatca = (struct st_tagREG_PRE*)&datb[L_fmH + L_axisH + SZ_COMHEADER];

	// fmH
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	tmps.Format("%05d", L_axisH + SZ_COMHEADER + SZ_TAGREGPRE);
	CopyMemory(fmH->datL, tmps, sizeof(fmH->datL));

	// axisH
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
	axisH->trxK = DF_ST_TAGREG;
	CopyMemory(axisH->trxC, "piborely", sizeof(axisH->trxC));
	tmps.Format("%05d", SZ_COMHEADER + SZ_TAGREGPRE);
	CopyMemory(axisH->datL, tmps, sizeof(axisH->datL));

	// st_comheader
	FillMemory(pst_header, SZ_COMHEADER, ' ');
	CopyMemory(pst_header->msgid, "0801", 4);
	CopyMemory(pst_header->uvalue, m_macaddr, m_macaddr.GetLength());

	// st_tagREG_PRE
	FillMemory(pst_midatca, SZ_TAGREGPRE, ' ');
	CopyMemory(pst_midatca->rqst_CHNL_DCD, "HTS", 3);			//채널구분(임의로)
	CopyMemory(pst_midatca->tran_LGN_YN, "Y", 1);				//로그인 여부(주문 , 로그인)

	ZeroMemory(m_pPubkey, 1024);
	const	int	ipkeysize = getPubkey(m_pPubkey);			//공개키
	CopyMemory(pst_midatca->cprs_pubkey, m_pPubkey, ipkeysize * 2);		//공개키
	CopyMemory(pst_midatca->tran_elsg_ontt_con, "IBKs", 4);			//서명원문(임의로)

	if (m_sock->Write(datb.get(), datl))
		SetTimer(TM_RECEIVE, 2000, NULL);

	datb.reset();
}	

void DlgQr::SendtagINQUIRYREG() //서명등록여부조회 //주기적으로 계속조회한다
{
	CString	tmps;
	const	int	datl = L_fmH + L_axisH + SZ_COMHEADER + SZ_INQUIRY_REG_TRAN_BY_RELAY_REQ;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl);
	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];
	struct st_comheader* pst_header = (struct st_comheader*)&datb[L_fmH + L_axisH];
	INQUIRY_REG_TRAN_BY_RELAY_REQ* pst_midatca = (INQUIRY_REG_TRAN_BY_RELAY_REQ*)&datb[L_fmH + L_axisH + SZ_COMHEADER];

	// fmH
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	tmps.Format("%05d", L_axisH + SZ_COMHEADER + SZ_INQUIRY_REG_TRAN_BY_RELAY_REQ);
	CopyMemory(fmH->datL, tmps, sizeof(fmH->datL));

	// axisH
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
	axisH->trxK = DF_ST_CHKREG;
	CopyMemory(axisH->trxC, "piborely", sizeof(axisH->trxC));
	tmps.Format("%05d", SZ_COMHEADER + SZ_INQUIRY_REG_TRAN_BY_RELAY_REQ);
	CopyMemory(axisH->datL, tmps, sizeof(axisH->datL));

	// st_comheader
	FillMemory(pst_header, SZ_COMHEADER, ' ');
	CopyMemory(pst_header->msgid, "0805", 4);
	if (GetMacAddr().IsEmpty())
	{
	}
	CopyMemory(pst_header->uvalue, m_macaddr, m_macaddr.GetLength());

	// INQUIRY_REG_TRAN_BY_RELAY_REQ
	FillMemory(pst_midatca, SZ_INQUIRY_REG_TRAN_BY_RELAY_REQ, ' ');
	CopyMemory(pst_midatca->RQST_CHNL_DCD, "HTS", 3);						//m_strTrdNum m_strEasyTrdNum
	CopyMemory(pst_midatca->REL_TRAN_NO, m_strTrdNum, m_strTrdNum.GetLength());			//연계거래번호
	CopyMemory(pst_midatca->REL_TRAN_EASY_NO, m_strEasyTrdNum, m_strEasyTrdNum.GetLength());	//연계간편인증번호
	
	const	BOOL	rc = m_sock->Write(datb.get(), datl);
	if (!m_bChecking && rc)
	{
		m_bChecking = true;
		KillTimer(TM_CHECKREG);
		SetTimer(TM_CHECKREG, TM_CHECKTIME, NULL);
	}

	datb.reset();
}

void DlgQr::SendSrhAuthTrdnum()  //직전 인증 성공한 인증거래번호를 조회
{
	CString	tmps;
	const	int	datl = L_fmH + L_axisH + SZ_COMHEADER + SZ_INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl);
	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];
	struct st_comheader* pst_header = (struct st_comheader*)&datb[L_fmH + L_axisH];
	INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ* pst_midatca = (INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ*)&datb[L_fmH + L_axisH + SZ_COMHEADER];

	// fmH
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	tmps.Format("%05d", L_axisH + SZ_COMHEADER + SZ_INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ);
	CopyMemory(fmH->datL, tmps, sizeof(fmH->datL));

	// axisH
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
	axisH->trxK = DF_ST_SEARCHTRDNUM;
	CopyMemory(axisH->trxC, "piborely", sizeof(axisH->trxC));
	tmps.Format("%05d", SZ_COMHEADER + SZ_INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ);
	CopyMemory(axisH->datL, tmps, sizeof(axisH->datL));

	// st_comheader
	FillMemory(pst_header, SZ_COMHEADER, ' ');
	CopyMemory(pst_header->msgid, "0806", 4);
	if (GetMacAddr().IsEmpty())
	{
	}
	CopyMemory(pst_header->uvalue, m_macaddr, m_macaddr.GetLength());

	// INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ
	FillMemory(pst_midatca, SZ_INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ, ' ');
	CopyMemory(pst_midatca->RQST_CHNL_DCD, "HTS", 3);
	CopyMemory(pst_midatca->REL_TRAN_NO, m_strTrdNum, m_strTrdNum.GetLength());
	CopyMemory(pst_midatca->REL_TRAN_EASY_NO, m_strEasyTrdNum, m_strEasyTrdNum.GetLength());  //연계간편인증번호
	
	m_sock->Write(datb.get(), datl);
	datb.reset();
}

void DlgQr::SendLoginconfirm()  //로그인 거래 검증
{
	CString	tmps;
	const	int	datl = L_fmH + L_axisH + SZ_COMHEADER + SZ_VERIFY_LOGIN_TRAN_BY_RELAY_REQ;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl);
	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];
	struct st_comheader* pst_header = (struct st_comheader*)&datb[L_fmH + L_axisH];
	VERIFY_LOGIN_TRAN_BY_RELAY_REQ* pst_midatca = (VERIFY_LOGIN_TRAN_BY_RELAY_REQ*)&datb[L_fmH + L_axisH + SZ_COMHEADER];

	// st_comheader
	FillMemory(pst_header, SZ_COMHEADER, ' ');
	CopyMemory(pst_header->msgid, "0807", 4);
	if (GetMacAddr().IsEmpty())
	{
	}
	CopyMemory(pst_header->uvalue, m_macaddr, m_macaddr.GetLength());
	
	// VERIFY_LOGIN_TRAN_BY_RELAY_REQ
	FillMemory(pst_midatca, SZ_VERIFY_LOGIN_TRAN_BY_RELAY_REQ, ' ');
	CopyMemory(pst_midatca->RQST_CHNL_DCD, "HTS", 3);
	CopyMemory(pst_midatca->REL_TRAN_NO, m_strTrdNum, m_strTrdNum.GetLength());
	CopyMemory(pst_midatca->REL_TRAN_EASY_NO, m_strEasyTrdNum, m_strEasyTrdNum.GetLength()); 
	CopyMemory(pst_midatca->AUTH_TRAN_NO, m_strAutTrdNum, m_strAutTrdNum.GetLength());
	
	ZeroMemory(m_pdata, szBUFF);
	CopyMemory(m_pdata, pst_midatca, SZ_VERIFY_LOGIN_TRAN_BY_RELAY_REQ);

	// fmH
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC  = fmC_SSM;
	fmH->ssM  = ssM_WS;
	fmH->stat = stat_WS;
	tmps.Format("%05d", L_axisH + SZ_COMHEADER + SZ_VERIFY_LOGIN_TRAN_BY_RELAY_REQ);
	CopyMemory(fmH->datL, tmps, sizeof(fmH->datL));
	
	// axisH
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
	axisH->trxK = DF_ST_VEFYLOGIN;
	CopyMemory(axisH->trxC, "piborely", sizeof(axisH->trxC));
	tmps.Format("%05d", SZ_COMHEADER + SZ_VERIFY_LOGIN_TRAN_BY_RELAY_REQ);
	CopyMemory(axisH->datL, tmps, sizeof(axisH->datL));
	
	m_sock->Write(datb.get(), datl);
	datb.reset();
}

void DlgQr::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (m_pPubkey)
	{
		delete m_pPubkey;
		m_pPubkey = NULL;
	}
	if (m_sock)
		m_sock.reset();
	
	if (m_pBrowser)
	{
		delete m_pBrowser;
		m_pBrowser = nullptr;
	}
	CDialog::OnCancel();
}

void DlgQr::OnOK() 
{
	// TODO: Add extra validation here
	if (m_pPubkey)
	{
		delete m_pPubkey;
		m_pPubkey = NULL;
	}
	if (m_sock)
		m_sock.reset();
	
	if (m_pBrowser)
	{
		delete m_pBrowser;
		m_pBrowser = nullptr;
	}

	CDialog::OnOK();
}

void DlgQr::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

