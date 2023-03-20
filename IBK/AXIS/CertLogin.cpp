// CertLogin.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "CertLogin.h"
#include "axmsg.hxx"
#include "setup.h"
#include "afxwin.h"
#include "SetupC.h"
#include "progressDlg.h"
#include "MyToolTip.h"
#include "../dll/sm/TransparentMgr.h"
#include "../dll/sm/EnBitmap.h"



#include "MainFrm.h"
#include "TestInfoDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CCertLogin dialog


#define	KEY_RETURN_TEST		// ����Ű�� �α���

#define	INPUTMAX	8
#define IDT_BANNER		1005
#define IDT_LEFTMOTION  1006
#define IDT_SECURE		1007

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_HAND MAKEINTRESOURCE(32649)

#define IDC_SHAPE1		1001

/////////////////////////////////////////////////////////////////////////////
// CCertLogin dialog

#define STR_CAPS	_T("Caps Lock �� ���� �ֽ��ϴ�")
#define STR_SLIDE	_T("��  ��\tCaps Lock �� ���� �ֽ��ϴ�\t")

#pragma warning (disable : 26409)
#pragma warning (disable : 26400)
#pragma warning (disable : 6273)
#pragma warning (disable : 6387)
#pragma warning (disable : 6011)

CCertLogin::CCertLogin(CWnd* pParent, CString userID /*=NULL*/)
: CDialog(CCertLogin::IDD, pParent)
{
	m_frame = pParent;
	m_user  = m_backUser = userID;
	m_pass = _T("");
	m_cPass = _T("");
	m_staff		= FALSE;
	m_bOnlyGuest = AfxGetApp()->GetProfileInt(WORKSTATION, "OnlyGuest", 0) ? TRUE : FALSE;

	m_mode		= SM_NO;

	m_progress = (CProgressDlg*)NULL;

	m_rate		= 0;
	m_totalCNT	= 0;
	m_curCNT	= 0;
	m_totalSIZE	= 0.0;
	m_curSIZE	= 0.0;
	m_auto		= false;
	m_skinShow	= false;
	m_framerun	= false;
	m_bWeb1		= false;
	m_bWeb2		= false;
	m_bitmap	= m_bitmapInput = m_bitmapEvent = (HBITMAP) nullptr;
	m_bCertLogin = FALSE;
	
	m_bmpInput = NULL;
	m_bmpEvent = NULL;

	CString	fontName;
	fontName.LoadString(ST_FONTNAME_DEFAULT);
	m_font.CreatePointFont(85, "����");
	m_bFont.CreateFont(	12,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD,	           // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				DEFAULT_CHARSET,           // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                   // lpszFacename
	m_pToolTip = NULL;
	
	for (int i = 0; i < MAX_SHAPE; i++)
		m_shapeButtons[i] = NULL;

	m_currEvent = -1;
	m_allResourceDownload = FALSE;
	m_forceLogin = 0;

	m_AkSdk = NULL;

	m_status = Axis::GetBitmap("NEW_LOGIN_STATUS",FALSE);

	if (!m_status || !m_status->m_hObject)
		m_status = Axis::GetBitmap("LOGIN_STATUS",FALSE);

	m_status_err = Axis::GetBitmap("NEW_LOGIN_STATUS_ERR",FALSE);

	if (!m_status_err || !m_status_err->m_hObject)
		m_status_err = Axis::GetBitmap("LOGIN_STATUS_ERR",FALSE);
	
	CString str = AfxGetApp()->GetProfileString(INFORMATION, "Port");
	m_staff = (atoi(str) == portEmployee);

	//::SetTitle("[�α���]IBK hot Trading");
	//::SetWindowText(m_hWnd, "[�α���]IBK hot Trading");
	//int ExtendedStyles;
	//ExtendedStyles = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	//SetWindowLong(m_hWnd, GWL_EXSTYLE, ExtendedStyles | WS_EX_APPWINDOW);
	//SetWindowPos(&wndTop,0,0,0,0,SW_SHOWNORMAL);
}

CCertLogin::~CCertLogin()
{
	if (m_progress)
	{
		m_progress->DestroyWindow();
		delete m_progress;
	}


	m_font.DeleteObject();
	m_bFont.DeleteObject();

	if (m_bmpInput)
	{
		m_bmpInput->DestroyWindow();
		delete m_bmpInput;
	}
	if (m_bmpEvent)
	{
		m_bmpEvent->DestroyWindow();
		delete m_bmpEvent;
	}
	
	if (m_bitmap)	
		DeleteObject(m_bitmap);
	if (m_bitmapInput)
		DeleteObject(m_bitmapInput);
	if (m_bitmapEvent)
		DeleteObject(m_bitmapEvent);
	//if (m_status)
	//	DeleteObject(m_status);

}

void CCertLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCertLogin)
	DDX_Control(pDX, IDC_DONLYSISE, m_ckSISEONLY);
	DDX_Control(pDX, IDC_DSAVEPASS, m_ckSAVEPASS);
	DDX_Control(pDX, IDC_DSAVEID, m_ckSAVEID);
	DDX_Control(pDX, IDC_DCSTART, m_ckCSTART);
	DDX_Control(pDX, IDC_DBITMAP, m_static);
	DDX_Text(pDX, IDC_DUSER, m_user);
	DDX_Text(pDX, IDC_DPASS, m_pass);
	DDX_Text(pDX, IDC_DCPASS, m_cPass);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCertLogin, CDialog)
	//{{AFX_MSG_MAP(CCertLogin)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DRUN, OnRun)
	ON_BN_CLICKED(IDC_DEXIT, OnExit)
	ON_BN_CLICKED(IDC_BANNER, OnBanner)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DSETUP, OnSetup)
	ON_BN_CLICKED(IDC_DSAVEID, OnSaveID)
	ON_BN_CLICKED(IDC_DCSTART, OnCertStart)
	ON_BN_CLICKED(IDC_CERT, OnCert)
	ON_BN_CLICKED(IDC_GEN, OnGen)
	ON_BN_CLICKED(IDC_DRUNX, OnRunX)
	ON_BN_CLICKED(IDC_DONLYSISE, OnOnlysise)
	ON_EN_SETFOCUS(IDC_DUSER, OnSetfocusDuser)
	ON_EN_KILLFOCUS(IDC_DUSER, OnKillfocusDuser)
	ON_EN_SETFOCUS(IDC_DPASS, OnSetfocusDpass)
	ON_EN_KILLFOCUS(IDC_DPASS, OnKillfocusDpass)
	ON_EN_SETFOCUS(IDC_DCPASS, OnSetfocusDcpass)
	ON_EN_KILLFOCUS(IDC_DCPASS, OnKillfocusDcpass)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DSAVEPASS, OnDsavepass)
	ON_EN_CHANGE(IDC_DPASS, OnChangeDpass)
	ON_EN_CHANGE(IDC_DUSER, OnChangeDuser)
	ON_EN_CHANGE(IDC_DCPASS, OnChangeDcpass)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_EVENT1, OnEvent1)
	ON_BN_CLICKED(IDC_EVENT2, OnEvent2)
	ON_BN_CLICKED(IDC_EVENT3, OnEvent3)
	ON_BN_CLICKED(IDC_MODE, OnMode)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_STOP, OnExit)  //vc2019 ?
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCertLogin message handlers

BOOL CCertLogin::OnInitDialog() 
{
	AfxEnableControlContainer();
	
	CDialog::OnInitDialog();
	SetWindowText("IBK hot Trading");
	CEdit* edit = (CEdit*)GetDlgItem(IDC_DUSER);
	edit->SetPasswordChar('*');
	edit = (CEdit*)GetDlgItem(IDC_DPASS);
	edit->SetPasswordChar('*');
    edit = (CEdit*)GetDlgItem(IDC_DCPASS);
	edit->SetPasswordChar('*');

	//ResourceHelper()->LoadIcon();  //vc2019
	SetIcon(ResourceHelper()->GetIcon(), TRUE);

	if (!Axis::isCustomer)
		initStaff();

	CWinApp* app = AfxGetApp();	

	const int iLoginVerUP = 0;
	bLoginVerUp = false;
	
	((CButtonST*) GetDlgItem(IDC_DSAVEID))->SetButtonStyle(BS_CHECKBOX);

	if(Axis::isCustomer)
		((CEdit*)GetDlgItem(IDC_DPASS))->SetLimitText(10);
	else
		((CEdit*)GetDlgItem(IDC_DPASS))->SetLimitText(16);

	((CEdit*)GetDlgItem(IDC_DCPASS))->SetLimitText(30);
	
	m_stGuest.Create("GUEST", WS_CHILD, CRect(424, 128, 422 + 50, 128 + 13), this, IDC_STATIC);
	m_stGuest.SetFont(GetFont());
	//m_stGuest.ShowWindow(SW_HIDE);
	
	CString	imgN;

// 	if (Axis::isCustomer)
// 	{
// 		imgN.Format("%s\\%s\\axis.jpg", Axis::home, IMAGEDIR);
// 	}
// 	else
// 	{
// 		imgN.Format("%s\\%s\\saxis.jpg", Axis::home, IMAGEDIR);
// 	}

	BOOL bChkPosChange = FALSE;

//	m_bitmap = CEnBitmap::LoadImageFile(imgN, RGB(255, 255, 255));
	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 

	const int loginType = GetPrivateProfileInt("LOGINTYPE", "TYPE", 1, file);

	if(loginType == 1)
		m_bCertLogin = FALSE;
	else
		m_bCertLogin = TRUE;

	//CString s;
	//s.Format("PASSWORD 22 TYPE [%d] [%d]n",m_bCertLogin,loginType);
	//OutputDebugString(s);
	
	if (Axis::isCustomer && m_bitmap)
	{
		CRect rcOnlysise;
		CRect rcSaveid;
		((CButtonST *) GetDlgItem(IDC_DONLYSISE))->GetWindowRect(&rcOnlysise);
		((CButtonST *) GetDlgItem(IDC_DSAVEID))->GetWindowRect(&rcSaveid);
		ScreenToClient(&rcOnlysise);
		ScreenToClient(&rcSaveid);
		((CButtonST *) GetDlgItem(IDC_DSAVEID))->MoveWindow(rcOnlysise);
		((CButtonST *) GetDlgItem(IDC_DONLYSISE))->MoveWindow(rcSaveid);
		bChkPosChange = TRUE;
	}

	if (!m_bitmap && !bLoginVerUp)
	{
		if(Axis::isCustomer)
		{
			if(!m_bCertLogin)
				imgN.Format("%s\\%s\\new_login_bg_bagic_01.bmp", Axis::home, IMAGEDIR);
			else
				imgN.Format("%s\\%s\\new_login_bg_bagic_02.bmp", Axis::home, IMAGEDIR);
		}
		else
		{
			imgN.Format("%s\\%s\\new_staff_login_bg_bagic.bmp", Axis::home, IMAGEDIR);
		}

// 		CFileFind ff;
// 
// 		if(!ff.FindFile(imgN))
// 		{
// 			imgN.Format("%s\\%s\\login_bg_bagic.bmp", Axis::home, IMAGEDIR);
// 		}

		m_bitmap = CEnBitmap::LoadImageFile(imgN, RGB(255, 255, 255));
	
		m_bmpBg = Axis::GetBitmap(imgN);
	}

	m_btnBanner = CreateBmpButton(IDC_BANNER, "NEW_LOGIN_BANNER");

	if(!m_btnBanner || !m_btnBanner->GetSafeHwnd())
	{
		GetDlgItem(IDC_BANNER)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_BANNER)->ShowWindow(SW_SHOW);
	}
	
	m_btnRun = CreateBmpButton(IDC_DRUN, "NEW_LOGIN_BTN_CONNECT");

	if (!m_btnRun || !m_btnRun->GetSafeHwnd())
		m_btnRun = CreateBmpButton(IDC_DRUN, "LOGIN_BTN_CONNECT");

	m_btnSetup = CreateBmpButton(IDC_DSETUP, "NEW_LOGIN_BTN_SETUP");

	if (!m_btnSetup || !m_btnSetup->GetSafeHwnd())
		m_btnSetup = CreateBmpButton(IDC_DSETUP, "LOGIN_BTN_SETUP");

	m_btnExit = CreateBmpButton(IDC_DEXIT, "NEW_LOGIN_BTN_CANCEL");

	if (!m_btnExit || !m_btnExit->GetSafeHwnd())
		m_btnExit = CreateBmpButton(IDC_DEXIT, "LOGIN_BTN_CANCEL");

	m_btnCert = CreateBmpButton(IDC_CERT, "NEW_LOGIN_TAB_01_OFF");

	if (!m_btnCert || !m_btnCert->GetSafeHwnd())	
		m_btnCert = CreateBmpButton(IDC_CERT, "LOGIN_TAB_01");

	m_btnGen = CreateBmpButton(IDC_GEN, "NEW_LOGIN_TAB_02_ON");

	if (!m_btnGen || !m_btnGen->GetSafeHwnd())
		m_btnGen = CreateBmpButton(IDC_GEN, "LOGIN_TAB_02");

	m_btnCert->SetWindowPos(NULL, 300, 75, 320, 36, SWP_NOZORDER);
	m_btnGen->SetWindowPos(NULL, 459, 75, 320, 36, SWP_NOZORDER);

	if(Axis::isCustomer)
	{
		m_btnCert->ShowWindow(SW_SHOW);
		m_btnGen->ShowWindow(SW_SHOW);
	}
	
	//m_btnMode = CreateBmpButton(IDC_MODE, "NEW_LOGIN_BTN_MODE");
	
	if (m_bitmap)
	{
		m_skinShow = true;
		MakeShape();

		const POINT shapePosition[MAX_SHAPE] =
		{
			{ 649, 1 }, { 649, 67 }, { 649, 134 }, { 649, 201 }, {  649, 268 }, { 649, 335 }
		};

		for (int i = 0; i < MAX_SHAPE-1; i++)
		{
			m_shapeButtons[i] = CreateShapeButton(IDC_SHAPE1 + i, Format("NEW_LINK_%02d", i + 1),
				CRect(shapePosition[i].x, shapePosition[i].y+iLoginVerUP, 0, 0));
		}

		m_shapeButtons[MAX_SHAPE-1] = CreateShapeButton(IDC_SHAPE1 + MAX_SHAPE-1, Format("NEW_LINK_%02d", MAX_SHAPE),
				CRect(300, 374, 0, 0));
		m_shapeButtons[MAX_SHAPE-1]->ShowWindow(SW_HIDE);

		BITMAP bm;

		//get the info of the bitmap
		GetObject(m_bitmap, sizeof(bm), &bm);
		SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOZORDER|SWP_NOMOVE);

		m_static.ShowWindow(SW_HIDE);

		//m_static.SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE);
		
		const int msgHeight = 31;
		const int msgWidth = 320;
		const int msgX = 300;
		const int msgY = 332;
		
		const int msgSHeight = 12;
		const int msgSWidth = 280;
		const int msgSX = 310; 
		const int msgSY = 342;
		
		const int editHeight = 20;
		const int editWidth  = 120;
		//const int editX = 137;
		const int editX = 310;
		const int editYGap = 44;
		//int editY = 309;
		int editY = 145;
		
		const int checkWidth = 152;
		const int checkHeight = 15;
		const int checkX = 445;
		const int checkYGap = 45;
		int checkY = 141;
   
		GetDlgItem(IDC_BANNER)->SetWindowPos(NULL,0,0,270,404, SWP_NOZORDER);
		
		//m_msgBrush.CreateSolidBrush(0x00E4DDD7);
		m_msgBrush.CreateSolidBrush(0x00FFFFFF);
		m_msgBrush_Nor.CreateSolidBrush(0x00774100);
		m_msgBrush_Err.CreateSolidBrush(0x00ECE1D5);
		GetDlgItem(IDC_SMSG)->SetWindowPos(NULL, msgSX, msgSY, msgSWidth, msgSHeight, SWP_NOZORDER);
		GetDlgItem(IDC_SMSG_BACK)->SetWindowPos(NULL, msgX, msgY, msgWidth, msgHeight, SWP_NOZORDER);
		//GetDlgItem(IDC_SMSG)->SetFont(&m_bFont);
		//SetDlgItemText(IDC_SMSG, "�غ�...");
		//CRect rc = CRect(90,292+iLoginVerUP,240+90+iLoginVerUP,292+12);
		//InvalidateRect(rc); //���¹� ����� ����ȭ�ؼ� �Է��� ȭ�� ����
		SetGuide("�غ�...");
		
		GetDlgItem(IDC_DUSER)->SetWindowPos(NULL, editX, editY, editWidth, editHeight, SWP_NOZORDER);
		editY += editYGap;
		GetDlgItem(IDC_DPASS)->SetWindowPos(NULL, editX, editY, editWidth, editHeight, SWP_NOZORDER);
		editY += editYGap;
		GetDlgItem(IDC_DCPASS)->SetWindowPos(NULL, editX, editY, editWidth, editHeight, SWP_NOZORDER);
		
		GetDlgItem(IDC_DSAVEID)->SetWindowPos(NULL, checkX, checkY, checkWidth, checkHeight, SWP_NOZORDER);
		checkY += checkYGap;
		GetDlgItem(IDC_DONLYSISE)->SetWindowPos(NULL, checkX, checkY, checkWidth, checkHeight, SWP_NOZORDER);
		checkY += checkYGap-2;
		GetDlgItem(IDC_DSAVEPASS)->SetWindowPos(NULL, checkX, checkY+2, checkWidth, checkHeight, SWP_NOZORDER);

		GetDlgItem(IDC_DCSTART)->SetWindowPos(NULL, 297, 139, checkWidth, checkHeight, SWP_NOZORDER);
		
		GetDlgItem(IDC_DRUN)->SetWindowPos(m_bmpInput ? m_bmpInput: &m_static,   300, 276, 256, 39, 0);
		GetDlgItem(IDC_DSETUP)->SetWindowPos(m_bmpInput ? m_bmpInput: &m_static, 566, 276,  56, 39, 0);
		GetDlgItem(IDC_DEXIT)->SetWindowPos(m_bmpInput ? m_bmpInput: &m_static,  620, 10,  40, 20, 0);
		
		GetDlgItem(IDC_MODE)->SetWindowPos(m_bmpInput ? m_bmpInput: &m_static,  615, 115+iLoginVerUP,  15, 15, 0);
		
		if( !Axis::isCustomer )
		{
			GetDlgItem(IDC_DCPASS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DSAVEPASS)->ShowWindow(SW_HIDE);  
		}
	}
	else
	{
		invisibleButton();
	}
	
	
	//m_static.SetBitmap(m_bitmap);
	//SetEventBitmap(0);

	initLogin();
	m_progress = new CProgressDlg(NULL, IsNewProgressImage());

	m_progress->Create((IsNewProgressImage()?IDD_NEW_PROGRESS:IDD_PROGRESS), this);
	m_progress->ShowWindow(SW_HIDE);

	m_cust	   = app->GetProfileInt(WORKSTATION, "cust", 0)   ? TRUE : FALSE;
	m_saveID   = app->GetProfileInt(WORKSTATION, "SaveID", 0)   ? TRUE : FALSE;
	m_savePass = app->GetProfileInt(WORKSTATION, "SavePass", 0) ? TRUE : FALSE;
	m_onlysise = app->GetProfileInt(WORKSTATION, "OnlySise", 0) ? TRUE : FALSE;
	m_bOnlyGuest = app->GetProfileInt(WORKSTATION, "OnlyGuest", 0) ? TRUE : FALSE;
	m_bCertStart = app->GetProfileInt(WORKSTATION, "CStart", 0) ? TRUE : FALSE;

	//**IBK �ߺ��α� ��å�� ���ʿ��� �κ��̹Ƿ� ����ŷ.
	//**m_forceLogin = app->GetProfileInt(WORKSTATION, "ForceLogin", 0);
	
	if (!m_auto && !m_saveID)	
		m_user.Empty();

	m_by = app->GetProfileInt(INFORMATION, CONNECTBY, 0);
	switch (m_progK)
	{
	default:case 'H':	// HTS
		if (m_by != byINET && m_by != byINET2)
			m_by = byINET;
		break;
	}

	CenterWindow(CWnd::GetDesktopWindow());
	//CenterWindow(m_frame);
	GetDlgItem(IDC_DCPASS)->EnableWindow(Axis::isCustomer);
	if (m_saveID)	
		((CButtonST *) GetDlgItem(IDC_DSAVEID))->SetCheck(TRUE);
	else
	{
		GetDlgItem(IDC_DUSER)->SetWindowText("");
		m_user = m_backUser = "";
	}
	if (m_savePass)	((CButtonST *) GetDlgItem(IDC_DSAVEPASS))->SetCheck(TRUE);
	if (m_onlysise)
	{
		((CButtonST *) GetDlgItem(IDC_DONLYSISE))->SetCheck(TRUE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
	}

	if(((CMainFrame*)m_frame)->m_DInstallASTx == STSDKEX_ERROR_PRODUCT_NOT_INSTALLED)
		m_bCertStart= FALSE;

	if (m_bCertStart)
	{
		((CButtonST *) GetDlgItem(IDC_DCSTART))->SetCheck(TRUE);
	}

	m_ckSAVEID.SetWindowText("          ");
	m_ckSAVEPASS.SetWindowText("          ");
	m_ckSISEONLY.SetWindowText("          ");
	m_ckCSTART.SetWindowText("          ");
	m_ckSAVEID.SetFont(&m_font);
	m_ckSAVEPASS.SetFont(&m_font);
	m_ckSISEONLY.SetFont(&m_font);
	m_ckCSTART.SetFont(&m_font);

	/** guest ���ʿ�
	if (m_bOnlyGuest)	// ���忡 ��ġ�� ������.
	{
		// Guest + 000 ������ȣ 3�ڸ��� �޵���.
		GetDlgItem(IDC_DUSER)->SetWindowPos(NULL, 472, 124, 40, 18, SWP_NOZORDER);
		((CEdit*)GetDlgItem(IDC_DUSER))->SetLimitText(3);


		if (m_user.GetLength() > 5 && !m_user.Left(5).CompareNoCase("guest"))
		{
			m_user = m_user.Mid(5);
		}
		else if (m_user.GetLength() != 3)
			m_user.Empty();
		m_stGuest.ShowWindow(SW_SHOW);
	}
	**/

#ifdef _DEBUG
	m_cPass = "";
#endif // _DEBUG
	
	m_mode = AfxGetApp()->GetProfileInt(WORKSTATION, "RightMode", 1);

	//OnMode();		
	
	//���̵� �ʱ�ȭ
	if (m_user == "")
	{
		//MessageBox()
		CEdit* edit1 = (CEdit*) GetDlgItem(IDC_DUSER);
		edit1->SetPasswordChar(0);
		m_user = "����� ID";
		//((CEdit*) GetDlgItem(IDC_DUSER))->SetWindowText(m_user);
		
		//GetDlgItem(IDC_DUSER)->SetFocus();
	}
	
	//��й�ȣ �ʱ�ȭ
	if (m_pass == "")
	{
		//GetDlgItem(IDC_DPASS)->SetFocus();
		CEdit* edit2 = (CEdit*) GetDlgItem(IDC_DPASS);
		edit2->SetPasswordChar(0);
		
		m_pass = "���Ӻ�й�ȣ";
		((CEdit*) GetDlgItem(IDC_DPASS))->SetWindowText(m_pass);
	}
	if (m_user != "" && m_user != "����� ID")
	{
		//((CEdit*) GetDlgItem(IDC_DPASS))->SetSel(0,-1);
		((CEdit*) GetDlgItem(IDC_DPASS))->SetWindowText(m_pass);
		((CEdit*) GetDlgItem(IDC_DPASS))->SetFocus();		
	}
	else
	{
		//AfxMessageBox("2");
		m_user = "����� ID";
		((CEdit*) GetDlgItem(IDC_DUSER))->SetWindowText(m_user);
		((CEdit*) GetDlgItem(IDC_DUSER))->SetSel(0,-1);
		((CEdit*) GetDlgItem(IDC_DUSER))->SetFocus();	

	}	
	//������ �ʱ�ȭ
	if (m_cPass == "")
	{
		CEdit* edit3 = (CEdit*) GetDlgItem(IDC_DCPASS);
		edit3->SetPasswordChar(0);
		m_cPass = "������ ��й�ȣ";
	}	
	
	UpdateData(FALSE);

	//** �α��� ��ȭ������ ����ȭ ȿ���� ����� ��� remark ������ ��.
	/**
	CTransparentMgr tp;
	tp.SetTPWnd(GetSafeHwnd(), 10, TRUE);
	**/
	//SetTimer(IDT_LEFTMOTION, 100, NULL);
	
	if (m_auto)
	{
		if (!m_user.IsEmpty() && m_pass.IsEmpty())
			GotoDlgCtrl(GetDlgItem(IDC_DPASS));
		else	
			GotoDlgCtrl(GetDlgItem(IDC_DRUN));

		//dkkim 2015.10.10
		//�ڵ��α��ν� �������α׷� ������ �α��� ������ ���ؼ� ����
		PostMessage(WM_COMMAND, IDC_DRUN);

		return FALSE;
	}
	
	if (!m_user.IsEmpty() && m_pass.IsEmpty() && (m_user != "����� ID"))
	{		
		GotoDlgCtrl(GetDlgItem(IDC_DPASS));
		
		UpdateData(FALSE);
		return FALSE;
	}

	//SetTimer(3000,1000,NULL);

	ModifyStyleEx(0, WS_EX_APPWINDOW, SWP_FRAMECHANGED);

	//dateData(FALSE);
	return FALSE;
}

void CCertLogin::initStaff()
{
	// empty
}

void CCertLogin::OnRun() 
{

	//OutputDebugString("CERT START ONRUN.......................................\n");
	HideToolTip();
	
	int iLoginVerUP = 0;
	const int tmp_mod = AfxGetApp()->GetProfileInt(WORKSTATION, "VerMode", 1);

	
	CString sip = ((CMainFrame*)m_frame)->m_ipAddr;
	if (tmp_mod >= 4)
	{
		//m_frame->PostMessage(WM_CLOSE);
		//��ġ������ �� ��Ȳ
		//ShellExecute(NULL, _T("open"), "IBKS_setup.exe", NULL, NULL, SW_SHOWNORMAL);
	//}
	//else if (tmp_mod == 5)
	//{
		//��ġ�� ���ҽ��� ���� ��Ȳ 
		bLoginVerUp = false;
		iLoginVerUP = 0;
	}
	else
	{	
		if (true)
		{
//			OutputDebugString("START UPDATE.EXE.......................................\n");
			RunUpdate();
			m_frame->PostMessage(WM_CLOSE);
			EndDialog(IDCANCEL);
		}
		else
		{		
			bLoginVerUp = true;
			iLoginVerUP = -2000;
			AfxGetApp()->WriteProfileInt(WORKSTATION, "VerMode", 3);
		}
	}


	SetCurrentDirectory(Axis::home + "\\exe");
	UpdateData(TRUE);
	//������Ʈ ������Ʈ ����
// 	CString updatepath;
// 	updatepath.Format("%s\\%s\\update.ini",Axis::home,TABDIR);
// 
// 	int nUseUpdate = GetPrivateProfileInt("UPDATE","RSCDNENABLE",0,updatepath);
// 
// 	if(nUseUpdate)
// 	{
// 		SetGuide("������Ʈ ������Ʈ�� ���ҽ� �ٿ����Դϴ�.��� �� �ٽ� ������ּ���.");
// 		((CEdit*) GetDlgItem(IDC_DUSER))->SetSel(0,-1);
// 		GetDlgItem(IDC_DUSER)->SetFocus();
// 		
// 		return;
// 	}

	// ������ ���� ����
	if(!m_bCertLogin)
	{
		if (!m_btnRun->IsEnable())	return;

		Axis::userID = m_user;
		if (IsNumber(m_user) && (m_user != "##ibk9") && (m_user != "##opuser") && (m_user != "##ibk8"))
		{
			//���� ���̵���
			if (Axis::isCustomer)
			{
				SetGuide("��Ȯ���� ���� ���̵� �Դϴ�.\n���̵� Ȥ�� ������ Ȯ���Ͻñ� �ٶ��ϴ�.");
				((CEdit*) GetDlgItem(IDC_DUSER))->SetSel(0,-1);
				GetDlgItem(IDC_DUSER)->SetFocus();
				
				return;
			}
			
		}
		else
		{
			//�� ���̵���
			if (!Axis::isCustomer && (m_user != "##ibk9") && (m_user != "##opuser") && (m_user != "##ibk8"))
			{
				SetGuide("��Ȯ���� ���� ���̵� �Դϴ�.\n���̵� Ȥ�� ������ Ȯ���Ͻñ� �ٶ��ϴ�.");
				((CEdit*) GetDlgItem(IDC_DUSER))->SetSel(0,-1);
				GetDlgItem(IDC_DUSER)->SetFocus();
				return;
			}
			//MessageBox("�������Ծ��̵�");
		}
		

		if (m_user.IsEmpty() || m_user == "����� ID")
		{
			SetGuide("�����ID�� �Է��Ͽ� �ֽʽÿ�.");
			
			GetDlgItem(IDC_DUSER)->SetFocus();
			return;
		}
		if (!Axis::devMode && m_pass.GetLength() < 4 || m_pass == "���Ӻ�й�ȣ")
		{
			SetGuide("���Ӻ�й�ȣ�� �Է��Ͽ� �ֽʽÿ�.");
			GetDlgItem(IDC_DPASS)->SetFocus();
			return;
		}

		if (Axis::isCustomer && m_cPass.IsEmpty() && !m_ckSISEONLY.GetCheck() || (Axis::isCustomer && !m_ckSISEONLY.GetCheck() && m_cPass  == "������ ��й�ȣ"))
		{
			SetGuide("���� ��й�ȣ�� �Է��Ͽ� �ֽʽÿ�.");
			GetDlgItem(IDC_DCPASS)->SetFocus();
			return;
		}

		if (((CButtonST*) GetDlgItem(IDC_DONLYSISE))->GetCheck() == BST_CHECKED)
		{
	#ifdef NDEBUG
			CString	msg;
		//	msg = "��ȸ,��ȸ������ ������ �ü���ȸ, ������ȸ �� �����ϰ� \n�ֹ� �� ��ü���� ó���� ������ �Ұ��ϵ��� �α��� �ϴ� ����Դϴ�";
			msg = "�ü���ȸ �������� �α����Ͻðڽ��ϱ�? \n�ü���ȸ �������� �α��ν� �ֹ� �� ��ü �� �������� �ʿ��� �ŷ��� �̿��Ͻ� �� �����ϴ�.";
			Axis::MessageBox(this, msg, MB_ICONINFORMATION);
	#endif
		}

		Save();
	}

	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 

	if (m_allResourceDownload)
	{
		DeleteResourceInfoFile();
	}
	SetStatus(SM_RSM);
	m_frame->PostMessage(WM_AXIS, MAKEWPARAM(axSTART, 1));
	m_btnRun->SetEnable(false);
//	OutputDebugString("END ONRUN.......................................\n");
}

void CCertLogin::OnRunX() 
{
//	OutputDebugString("START ONRUNX.................................\n");
	OnRun();	
}

void CCertLogin::OnExit() 
{
	if (!GetDlgItem(IDC_DEXIT)->IsWindowEnabled() || !m_btnExit->IsEnable())
		return;

	m_frame->PostMessage(WM_CLOSE);
	EndDialog(IDCANCEL);
}

void CCertLogin::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

CString CCertLogin::GetUserID()
{
	if (IsWindow(m_hWnd))
		UpdateData(TRUE);

	return (m_bOnlyGuest? "guest" + m_user : m_user);
}

CString CCertLogin::GetPassword()
{
	if (IsWindow(m_hWnd))
		UpdateData(TRUE);
	//CString s;
	//s.Format("GETPASSWORD [%s]\n",m_pass);
	//OutputDebugString(s);

// 	if(m_AkSdk != NULL)
// 	{
// 		TCHAR buff[1024] = { 0x00 };
// 		DWORD dwLen = 1024;
// 		m_AkSdk->GetText(GetDlgItem(IDC_DPASS)->GetSafeHwnd(), buff, &dwLen);
// 		buff[dwLen] = 0x00;
// 		
// 		m_pass = buff;
// 	}

	if(m_bCertLogin)
		m_pass = "CERTLOGIN";

	return m_pass;
}

CString CCertLogin::GetCPass()
{
	if (IsWindow(m_hWnd))  
		UpdateData(TRUE);

// 	if(m_AkSdk != NULL)
// 	{
// 		TCHAR buff[1024] = { 0x00 };
// 		DWORD dwLen = 1024;
// 		m_AkSdk->GetText(GetDlgItem(IDC_DPASS)->GetSafeHwnd(), buff, &dwLen);
// 		buff[dwLen] = 0x00;
// 		
// 		m_cPass = buff;
// 	}

	return m_cPass;
}

void CCertLogin::SetUserID(CString user)
{
	m_user = m_backUser = user;
	/** guest ���ʿ�
	if (m_bOnlyGuest)
	{
		if (m_user.GetLength() > 5 && !m_user.Left(5).CompareNoCase("guest"))
		{
			m_user = m_user.Mid(5);
		}
		else if (m_user.GetLength() != 3)
			m_user.Empty();
	}
	**/
	
	if (IsWindow(m_hWnd))
	{	
		
		UpdateData(FALSE);

	}	
	
}

void CCertLogin::SetPassword(CString pass)
{
	CString s;
	s.Format("PASSWORD [%s]\n",pass);
	OutputDebugString(s);
	if(pass.Find("CERTLOGIN") > -1)
	{
		m_bCertLogin = TRUE;
		m_auto = true;
		return;
	}
	s.Format("PASSWORD [%s]\n",pass);
	OutputDebugString(s);
	m_pass = pass;
	m_auto = true;
	if (IsWindow(m_hWnd))
		UpdateData(FALSE);
}

void CCertLogin::SetCPass(CString pass)
{
	m_cPass = pass;
	if (IsWindow(m_hWnd))
		UpdateData(FALSE);

	m_bReboot = TRUE;
}

void CCertLogin::Save()
{
	CWinApp* app = AfxGetApp();
	if (((CButtonST *) GetDlgItem(IDC_DSAVEID))->GetCheck() == BST_CHECKED)
		m_saveID = TRUE;
	else	m_saveID = FALSE;
	if (((CButtonST *) GetDlgItem(IDC_DONLYSISE))->GetCheck() == BST_CHECKED)
		m_onlysise = TRUE;
	else	m_onlysise = FALSE;
	if (((CButtonST *) GetDlgItem(IDC_DSAVEPASS))->GetCheck() == BST_CHECKED)
		m_savePass = TRUE;
	else	m_savePass = FALSE;

	if (((CButtonST *) GetDlgItem(IDC_DCSTART))->GetCheck() == BST_CHECKED)
		m_bCertStart = TRUE;
	else
		m_bCertStart = FALSE;
	
	app->WriteProfileInt(WORKSTATION, "SaveID", m_saveID ? 1 : 0);
	app->WriteProfileInt(WORKSTATION, "SavePass", m_savePass ? 1 : 0);
	app->WriteProfileInt(WORKSTATION, "OnlySise", m_onlysise ? 1 : 0);
	app->WriteProfileInt(WORKSTATION, "CStart", m_bCertStart ? 1 : 0);
}

CString CCertLogin::GetSignInfo()
{
	const	int	signL = 10;
	char	signB[signL +1];

	FillMemory(signB, signL, ' ');
	signB[signL] = '\0';

	UpdateData(TRUE);

/** ����� ��������
//   [0] : '0' ���� '2' ����  
//   [1] : Save CA passwd (���� ��й�ȣ �ڵ��Է� ����)
//   [2] : '0' ���� '1' �ü�����    
**/
	CString forceLoginKind(Format("%d", m_forceLogin));
	signB[0] = forceLoginKind[0];
	
	if(m_bCertLogin)
	{
		signB[1] = '1';
		signB[2] = '0';
	}
	else
	{
		signB[1] = m_savePass ? '1': '0';
		signB[2] = m_onlysise ? '1': '0';
	}
	signB[3] = Axis::isCustomer ? '1': '0';

	//CString tmp;
	//if (Axis::isCustomer)
	//	tmp = "��";
	//else
	//	tmp = "����";

	//CString slog;
	//slog.Format("\r\n -------------[axis][GetSignInfo] [%s]  [%s] \r\n ", tmp, signB);
	//OutputDebugString(slog);

	return signB;
}

void CCertLogin::SetStatus(int mode)
{
	m_mode = mode;
	if (!IsWindow(m_hWnd))
		return;
	switch (mode)
	{
	case SM_EXIT:
		GetDlgItem(IDC_DUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);
		if (m_onlysise || !Axis::isCustomer)
		{
			GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
		}
		m_btnRun->SetEnable(true);
		m_btnExit->SetEnable(true);
		m_btnSetup->SetEnable(true);
		break;
	case SM_RSM:
		GetDlgItem(IDC_DUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_DPASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);

		m_btnRun->SetEnable(false);
		m_btnExit->SetEnable(true);
		m_btnSetup->SetEnable(false);
		break;
	case SM_WSH:
		GetDlgItem(IDC_DUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);
		if (m_onlysise || !Axis::isCustomer)
		{
			GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
		}
		m_btnRun->SetEnable(false);
		m_btnExit->SetEnable(true);
		m_btnSetup->SetEnable(true);
		break;
	case SM_NO:
	default:
		GetDlgItem(IDC_DUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);
		if (m_onlysise || !Axis::isCustomer)
		{
			GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
		}
		m_btnRun->SetEnable(true);
		m_btnExit->SetEnable(true);
		m_btnSetup->SetEnable(true);
		break;
	}
}

void CCertLogin::SetChoice(BOOL choice)
{
	if (IsWindow(m_hWnd))
	{
		GetDlgItem(IDC_DUSER)->EnableWindow(choice);
		GetDlgItem(IDC_DPASS)->EnableWindow(choice);
		GetDlgItem(IDC_DCPASS)->EnableWindow(choice);
#ifdef	KEY_RETURN_TEST
		if (Axis::isCustomer && choice && m_onlysise)	// �� : �ü������� ���� ������ ��й�ȣ disable
		{
			GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
		}
#endif
		if (m_onlysise || !Axis::isCustomer)
		{
			GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
			//MessageBox("m_onlysise");
		}

		m_btnRun->SetEnable(choice  ? true : false);
		m_btnExit->SetEnable(choice ? true : false);
	}
}

void CCertLogin::SetGuide(CString msg)
{
	if (IsWindow(m_hWnd))
	{
		if(msg.Find("�ý��� �۾�") >= 0)   
		{
			CTestInfoDlg dlg;
			CString file,stemp;
			char	buf[512];
			file.Format("%s\\tab\\axis.ini", Axis::home);
			DWORD dw = GetPrivateProfileString("SYSTEMPOPUP", "url", "", buf, sizeof(buf), file);
			stemp.Format("%s", buf);
			if(stemp.GetLength() > 0)
				dlg.m_strurl = stemp;
		
			dw = GetPrivateProfileString("SYSTEMPOPUP", "image", "", buf, sizeof(buf), file);
			stemp.Format("%s", buf);
			if(stemp.GetLength() > 0)
				dlg.m_strbitmap = stemp;

			dw = GetPrivateProfileString("SYSTEMPOPUP", "mode", "", buf, sizeof(buf), file);
			stemp.Format("%s", buf);
			if(stemp.GetLength() > 0)
				dlg.m_bRscMode = false;

			CString date;	
			CTime time;
			time = CTime::GetCurrentTime();	
			date.Format("%04d%02d%02d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(), time.GetMinute());
			
			char readB[1024];
			int readL;
			
			readL = GetPrivateProfileString("SYSTEMPOPUP","FROM",date,readB,sizeof(readB),file);
			CString strFrom(readB,readL);
			readL = GetPrivateProfileString("SYSTEMPOPUP","TO",date,readB,sizeof(readB),file);
			CString strTo(readB,readL);
			
			if(strFrom.GetLength() > 0)
			{
				if(atof(date) >= atof(strFrom) && atof(date) <= atof(strTo))
				{
					if(dlg.DoModal() == IDOK)
					{
						m_frame->PostMessage(WM_CLOSE);
						EndDialog(IDCANCEL);
					}
				}
			}
			else
			{
				if(dlg.DoModal() == IDOK)
				{
					m_frame->PostMessage(WM_CLOSE);
					EndDialog(IDCANCEL);
				}
			}
		}

		SetDlgItemText(IDC_SMSG, msg);
// 		CRect rc = CRect(90,292,240+90,+292+12);
// 		InvalidateRect(rc); //���¹� ����� ����ȭ�ؼ� �Է��� ȭ�� ����

		if(msg.Mid(0,4) == "3302")
		{
			CString	str;

			if(Axis::isCustomer)
			{
				str.Format("[���� HTS ���� ������ �ȳ�]"\
					"\n\n"\
					"�������� ����HTS ���ӽ� ID ��� �� ���� ������."\
					"\n\n"\
					"�� ������� : ���� ���� �� IP�ּ� �����"\
					"\n\n"\
					"�� ������� : ����������ID��û��(���ö��̾��� ���� ��)"\
					"\n\n"\
					"�� �׷���� > ���ڰ��� > ���繮���ۼ� > ����������"\
					"\n\n"\
					"�� ����� ���ö��̾��� ���� �Ϸ�� ó����");
			}
			else
			{
				str.Format("[������ HTS ���� �ȳ�]"\
					"\n\n"\
					"������ HTS�� IP ��� �� ���� ������."\
					"\n\n"\
					"�� IP ��� ���� : ���������� (6915-5257)");
			}

			Axis::MessageBox(this, str, MB_OK | MB_ICONINFORMATION);
		}
		if(msg.Mid(0,4) == "4259")
		{
			
		}
	}
}

void CCertLogin::SetGuide(CString msg, CString detail)
{
	if (IsWindow(m_hWnd))
	{
		msg += '\t';
		msg += detail;
		SetGuide(msg);
	}
}

void CCertLogin::ClearPassword()
{
	if (IsWindow(m_hWnd))
	{
		m_pass = _T("");
		GetDlgItem(IDC_DPASS)->SetWindowText(m_pass);
		GetDlgItem(IDC_DPASS)->SetFocus();
	}
}

void CCertLogin::SetTotal(CString total)
{
	if (m_progress && IsWindow(m_progress->GetSafeHwnd()))
		m_progress->SetTotal(total);
}

void CCertLogin::SetProgress(bool onoff)
{
	if (!IsWindow(m_hWnd))	return;

	if (onoff)
	{
		if (!m_progress->IsWindowVisible())
		{
			/** ���� Progress ǥ�� ���ϹǷ� ����ŷ.
			if (m_bNew)
			{
				CRect rc;
				GetWindowRect(&rc);
				m_progress->SetWindowPos(NULL, rc.left, rc.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
			}
			m_progress->ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
			**/
		}
	}
	else
	{
		if (m_progress->IsWindowVisible())
		{
			//**m_progress->ShowWindow(SW_HIDE);
			//**ShowWindow(SW_SHOW);
			SetForegroundWindow();
		}
	}
}

void CCertLogin::SetProgress(CString detail, int rate, BOOL error)
{
	if (!IsWindow(m_hWnd))	return;
	
	if (m_progress->IsWindowVisible())
		m_progress->SetProgress(detail, rate, error);
	else if (error)
	{
		SetGuide(detail);
		SetStatus(SM_NO);
	}
	else
	{
		SetDownloadFile(detail);
	}
}

void CCertLogin::OnCert()
{
	//AfxMessageBox("OK!");
	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 

	WritePrivateProfileString("LOGINTYPE", "TYPE", "0", file);

	m_bCertLogin = TRUE;

	CString imgN;

	imgN.Format("%s\\%s\\new_login_bg_bagic_02.bmp", Axis::home, IMAGEDIR);

	m_bmpBg = Axis::GetBitmap(imgN);

	m_btnCert->SetImgBitmap("NEW_LOGIN_TAB_01_ON",2);
	
	m_btnGen->SetImgBitmap("NEW_LOGIN_TAB_02_OFF",2);
	
	if (!m_btnGen || !m_btnGen->GetSafeHwnd())
		m_btnGen = CreateBmpButton(IDC_DRUN, "LOGIN_TAB_02");

	Invalidate();
	
// 	m_bitmap = CEnBitmap::LoadImageFile(imgN, RGB(255, 255, 255));
// 
// 	m_static.SetBitmap(m_bitmap);
// 
// 	BITMAP bm;
// 	
// 	//get the info of the bitmap
// 	GetObject(m_bitmap, sizeof(bm), &bm);
// 	SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOZORDER|SWP_NOMOVE);
// 
// 	::SetWindowPos(m_static.m_hWnd,HWND_BOTTOM,0, 0, bm.bmWidth, bm.bmHeight, SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);

	GetDlgItem(IDC_DCSTART)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_DUSER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DPASS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DCPASS)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_DSAVEID)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DONLYSISE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DSAVEPASS)->ShowWindow(SW_HIDE);

	SetGuide("");

	//m_pass = "CERTLOGIN";

	//Invalidate();  

	if(!m_auto)
	{
		//AfxMessageBox("AUTO");
		CString s;
		s.Format("CERT AUTO FALSE [%d]\n",m_bCertStart);
		OutputDebugString(s);
		
		if(m_bCertStart)
		{
			/*OnRun();*/
			SetTimer(2000,1000,NULL);
		}
	}
}

void CCertLogin::OnGen()
{
	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 

	WritePrivateProfileString("LOGINTYPE", "TYPE", "1", file);

	CString imgN;

	m_pass = "";
	
	imgN.Format("%s\\%s\\new_login_bg_bagic_01.bmp", Axis::home, IMAGEDIR);

	m_bCertLogin = FALSE;

	m_btnCert->SetImgBitmap("NEW_LOGIN_TAB_01_OFF",2);
	
	m_btnGen->SetImgBitmap("NEW_LOGIN_TAB_02_ON",2);
	
	m_bmpBg = Axis::GetBitmap(imgN);

	GetDlgItem(IDC_DUSER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DPASS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DCPASS)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_DSAVEID)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DONLYSISE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DSAVEPASS)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_DCSTART)->ShowWindow(SW_HIDE);
	
	m_ckSAVEID.DrawTransparent();
	m_ckSAVEPASS.DrawTransparent();
	m_ckSISEONLY.DrawTransparent();
	m_ckCERTLOGIN.DrawTransparent();

	SetGuide("");

	Invalidate();
}

void CCertLogin::OnClose() 
{
	if (m_framerun)
		EndDialog(IDOK);
	else	
		EndDialog(IDCANCEL);

	CDialog::OnClose();
}

void CCertLogin::OnSetup() 
{
	if (!m_btnSetup->IsEnable())
		return;
	const bool iscust = Axis::isCustomer;
	
	CSetup dlg(this);
	

	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_allResourceDownload)
		{
			m_allResourceDownload = TRUE;	
		}
		
		m_staff = (atoi(dlg.m_Port) == portEmployee);
		m_staff = !iscust;
	
		if (((CButtonST*) GetDlgItem(IDC_DONLYSISE))->GetCheck() == BST_CHECKED)
		{
			GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
		}
		else	
		{
			if (Axis::isCustomer) GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);
			else GetDlgItem(IDC_DCPASS)->EnableWindow(false);	//������ ��й�ȣ Ȱ�� ��Ȱ�� �߰�
		}
		ApplySecurity();
		GetDlgItem(IDC_DUSER)->SetFocus();
	}
}

void CCertLogin::OnSaveID() 
{
	UpdateData(TRUE);

	/** guest ���ʿ�
	if (m_user.GetLength() > 5 && !m_user.Left(5).CompareNoCase("guest"))
	{
		m_user = m_user.Mid(5);
	}
	else if (m_user.GetLength() != 3)
		m_user.Empty();
	**/
	/*
	if (m_ckSAVEID.GetCheck()) MessageBox("123: true");
	else MessageBox("123: false");
	*/
	//((CButtonST *) GetDlgItem(IDC_DSAVEID))->SetCheck(m_ckSAVEID.GetCheck()? 1: 0);
	GetDlgItem(IDC_DUSER)->SetWindowText(m_user);
	Save();
	//GetDlgItem(IDC_DUSER)->SetFocus();
}

void CCertLogin::invisibleButton()
{
	GetDlgItem(IDC_DSTATIC1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DSTATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DSTATIC3)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_DRUN)->ModifyStyle(BS_OWNERDRAW, 0);
	GetDlgItem(IDC_DEXIT)->ModifyStyle(BS_OWNERDRAW, 0);
	GetDlgItem(IDC_DSETUP)->ModifyStyle(BS_OWNERDRAW, 0);
}

void CCertLogin::InfoRSC()
{
	char		mapN[32]{}, ver[32]{}, vkey[32]{}, size[32]{}, path[32]{}, buf[256]{};
	FILE* fp{}, * wfp{};
	HANDLE		hFind{};
	CString		writefile, readfile, searchfile;
	WIN32_FIND_DATA FindFileData{};

	writefile.Format("%s\\tab\\infoRSC", Axis::home);
	readfile.Format("%s\\tab\\infoRSC_", Axis::home);
	CopyFile(writefile, readfile, TRUE);

	fopen_s(&fp, readfile, "r");
	if (fp == NULL)	return;

	fopen_s(&wfp, writefile, "w+");
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		sscanf_s(buf, "%s %s %s %s %s", mapN, 32, ver, 32, vkey, 32, size, 32, path, 32);
		searchfile.Format("%s\\%s\\%s", Axis::home, path, mapN);
		hFind = FindFirstFile(searchfile, &FindFileData);
		
		if (hFind != INVALID_HANDLE_VALUE)
			fprintf(wfp, buf);
	}

	fclose(wfp);
	fclose(fp);
	DeleteFile(readfile);
}

CWnd* CCertLogin::GetWnd(UINT nRes)
{
	return GetDlgItem(nRes);
}

void CCertLogin::OnOnlysise() 
{

	if (((CButtonST*) GetDlgItem(IDC_DONLYSISE))->GetCheck() == BST_CHECKED)
	{
/*
#ifdef NDEBUG
		CString	msg;
		msg = "�ü���ȸ�� �����ϰ� �ֹ� �� ������ȸ / ��ü����\n�Ұ��ϵ��� �α��� �ϴ� ����Դϴ�.";
		Axis::MessageBox(this, msg, MB_ICONINFORMATION);
#endif
*/
		GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
	}
	else	
	{
		if (Axis::isCustomer) GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);
		else GetDlgItem(IDC_DCPASS)->EnableWindow(false);	//������ ��й�ȣ Ȱ�� ��Ȱ�� �߰�
	}
	GotoDlgCtrl(GetDlgItem(IDC_DPASS));
	Save();
	GetDlgItem(IDC_DPASS)->SetFocus();

}

void CCertLogin::initLogin()
{
	m_ckCSTART.SetBitmaps(IDB_LOGIN_CHECK_ON, Axis::maskColor, IDB_LOGIN_CHECK_OFF, Axis::maskColor);
	m_ckCSTART.DrawTransparent();

	m_ckSAVEID.SetBitmaps(IDB_LOGIN_CHECK_ON, Axis::maskColor, IDB_LOGIN_CHECK_OFF, Axis::maskColor);
	m_ckSAVEID.DrawTransparent();

	m_ckSAVEPASS.SetBitmaps(IDB_LOGIN_CHECK_ON, Axis::maskColor, IDB_LOGIN_CHECK_OFF, Axis::maskColor);
	m_ckSAVEPASS.DrawTransparent();

	m_ckSISEONLY.SetBitmaps(IDB_LOGIN_CHECK_ON, Axis::maskColor, IDB_LOGIN_CHECK_OFF, Axis::maskColor);
	m_ckSISEONLY.DrawTransparent();

	//m_ckCERTLOGIN.SetBitmaps(IDB_LOGIN_CHECK_ON, Axis::maskColor, IDB_LOGIN_CHECK_OFF, Axis::maskColor);
	//m_ckCERTLOGIN.DrawTransparent();

	for (int ii = 0; ii <= 5; ii++)
	{
		m_ckSAVEID.SetColor(ii, RGB(255, 255, 255));
		m_ckSAVEPASS.SetColor(ii, RGB(255, 255, 255));
		m_ckSISEONLY.SetColor(ii, RGB(255, 255, 255));
		//m_ckCERTLOGIN.SetColor(ii, RGB(255, 255, 255));
		m_ckCSTART.SetColor(ii, RGB(255, 255, 255));
	}

	SetGuide("       ");

	//�������� ��ȸ,�ü����� üũ�ڽ� ����
	if(m_staff)
	{
		m_onlysise = false;
		((CButtonST *) GetDlgItem(IDC_DONLYSISE))->SetCheck(FALSE);
		m_ckSISEONLY.ShowWindow(SW_HIDE);  //�������� ���
		((CButtonST*) GetDlgItem(IDC_DONLYSISE))->ShowWindow(SW_HIDE);
	}
}

void CCertLogin::MakeShape()
{
	HRGN hWndRgn = DIBToRgn(m_bitmap, Axis::maskColor, FALSE);
	if(hWndRgn)
	{
		SetWindowRgn(hWndRgn,true);
		//UpdateWindow();
		//SomeDelay(m_hWnd);
		::DeleteObject(hWndRgn);
	}
}

BOOL CCertLogin::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CString		file, keys;
	const UINT		cmd = LOWORD(wParam);
	const UINT		event = HIWORD(wParam);

	if (event != BN_CLICKED)
		return CDialog::OnCommand(wParam, lParam);

	if (cmd >= IDC_SHAPE1 && cmd < IDC_SHAPE1 + MAX_SHAPE)
	{
		OnShapeButton(cmd - IDC_SHAPE1);
		return TRUE;
	}
	switch (cmd)
	{
	case IDC_BN_CA:		keys = "ca";		break;
	case IDC_BN_REMOTE:	RunHelpCom();		break;
	case IDC_BN_BREAK:	keys = "break";		break;
	case IDC_BN_BRANCH:	keys = "branch";	break;
	case IDC_BN_VIRUS:	keys = "virus";		break;
	}

	if (!keys.IsEmpty())
	{
		char	buf[512];
		file.Format("%s\\tab\\axis.ini", Axis::home);
		const DWORD dw = GetPrivateProfileString("WebLinkDT", keys, "", buf, sizeof(buf), file);
		if (dw > 0)	ShellExecute(NULL, _T("open"), buf, NULL,NULL, SW_SHOWNORMAL);
	}

	if (cmd == IDC_DNEWUSER)
	{
		m_user = "guest";
		UpdateData(FALSE);
		OnRun();
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CCertLogin::RunHelpCom()
{
	CString	aps, cmds, exes;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	//cmds.Format("%s\\exe\\HelpCom.exe type:2/aip:210.107.66.228/aport:6090/eip:210.107.66.228/eport:6080/t1:ShowInfoDlg", Axis::home);
	//cmds.Format("HCUser.exe 210.107.66.228 6080 ShowInfoDlg 123-1234 none 0000");
	cmds.Format("AHCUser.exe 210.107.66.228 6080 ShowInfoDlg 123-1234 none 0000 0 none");
	aps.Format("%s\\exe\\AHCUser.exe", Axis::home);

	const BOOL bRc = CreateProcess(
			aps,				// application name
			(char*)(const char *)cmds,	// command line
			NULL,				// process attribute
			NULL,				// thread attribute
			FALSE,				// is inherit handle
			0,					// creation flags
			NULL,				// environment
			NULL,				// current directory
			&si,				// STARTUPINFO
			&pi);				// PROCESS_INFORMATION
	PostMessage(WM_CLOSE);
}

void CCertLogin::OnSetfocusDuser() 
{
	if (m_auto)	return;
	CEdit* edit = (CEdit*) GetDlgItem(IDC_DUSER);
	edit->GetWindowText(m_user);
	m_user.TrimLeft(); m_user.TrimRight();

	edit->Invalidate();
}

void CCertLogin::OnKillfocusDuser() 
{
	CEdit* edit = (CEdit*) GetDlgItem(IDC_DUSER);
	edit->GetWindowText(m_user);
	m_user.TrimLeft(); m_user.TrimRight();
	//MessageBox(m_user);
	//MessageBox(m_user);
	if (m_user == "" || m_user == "����� ID")
	{
		m_user = "����� ID";
		
		edit->SetPasswordChar(0);
		edit->SetWindowText(m_user);
	}
	else
	{
		edit->SetPasswordChar('*');
	}
	//edit->SetPasswordChar('*');
	edit->Invalidate();
}


void CCertLogin::OnSetfocusDpass() 
{
	if (m_pass == "���Ӻ�й�ȣ")
	{
		CEdit* edit = (CEdit*) GetDlgItem(IDC_DPASS);
		edit->SetPasswordChar(0);
		((CEdit*) GetDlgItem(IDC_DPASS))->SetSel(0,-1);
	}
	else
	{
		CEdit* edit = (CEdit*) GetDlgItem(IDC_DPASS);
		edit->SetPasswordChar('*');
		GetDlgItem(IDC_DPASS)->SetWindowText(m_pass);
		((CEdit*) GetDlgItem(IDC_DPASS))->SetSel(0,-1);
	}
	
	
	const short	val = GetKeyState(VK_CAPITAL);
	if (val)
		ShowToolTip();
	else
		HideToolTip();
}

void CCertLogin::OnKillfocusDpass() 
{
	CEdit* edit = (CEdit*) GetDlgItem(IDC_DPASS);
	edit->GetWindowText(m_pass);
	if (m_pass == "" || m_pass == "���Ӻ�й�ȣ")
	{
		m_pass = "���Ӻ�й�ȣ";
		
		edit->SetPasswordChar(0);
		GetDlgItem(IDC_DPASS)->SetWindowText(m_pass);
	}
	else
	{	
		edit->SetPasswordChar('*');
	}

}

void CCertLogin::OnSetfocusDcpass() 
{
	/*
	if (m_cPass == "������ ��й�ȣ")
	{
		m_cPass = "";	
		CEdit* edit = (CEdit*) GetDlgItem(IDC_DCPASS);
		edit->SetSel(0,-1);
		edit->SetPasswordChar('*');
		GetDlgItem(IDC_DCPASS)->SetWindowText(m_cPass);
		
	}
	*/
	if (m_cPass == "������ ��й�ȣ")
	{
		//m_pass = "";
		CEdit* edit = (CEdit*) GetDlgItem(IDC_DCPASS);
		edit->SetPasswordChar(0);
		edit->SetSel(0,-1);
		
		
	}
	else
	{
		//m_pass = "";
		CEdit* edit = (CEdit*) GetDlgItem(IDC_DCPASS);
		edit->SetPasswordChar('*');
		GetDlgItem(IDC_DCPASS)->SetWindowText(m_cPass);
		edit->SetSel(0,-1);	
	}
	
	const short	val = GetKeyState(VK_CAPITAL);
	if (val)
		ShowToolTip();
	else
		HideToolTip();
	
}

void CCertLogin::OnKillfocusDcpass() 
{
	CEdit* edit = (CEdit*) GetDlgItem(IDC_DCPASS);
	edit->GetWindowText(m_cPass);
	if (m_cPass == "" || m_cPass == "������ ��й�ȣ")
	{
		m_cPass = "������ ��й�ȣ";	
		
		edit->SetPasswordChar(0);
		GetDlgItem(IDC_DCPASS)->SetWindowText(m_cPass);
	}
	else
	{
		edit->SetPasswordChar('*');
	}	

}

BOOL CCertLogin::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_CAPITAL)
		{
			const short	val = GetKeyState(VK_CAPITAL);
			if (val & 0x01)
				ShowToolTip();
			else
				HideToolTip();
		}
		if (pMsg->wParam == VK_PROCESSKEY)
			return TRUE;

#ifdef	KEY_RETURN_TEST
		if (pMsg->wParam == VK_RETURN)
		{
			if (GetDlgItem(IDC_DUSER)->m_hWnd == pMsg->hwnd)
			{  
				GotoDlgCtrl(GetDlgItem(IDC_DPASS));
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);
				return TRUE;	
			}
			else if (GetDlgItem(IDC_DPASS)->m_hWnd == pMsg->hwnd)
			{
				if (Axis::isCustomer && GetDlgItem(IDC_DCPASS)->IsWindowEnabled() && !m_onlysise)
				{
					GotoDlgCtrl(GetDlgItem(IDC_DCPASS));
					::TranslateMessage(pMsg);
					::DispatchMessage(pMsg);
					return TRUE;	
				}

			}
		}
#endif
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (GetDlgItem(IDC_DUSER)->m_hWnd == pMsg->hwnd)
		{
			((CEdit*) GetDlgItem(IDC_DUSER))->SetSel(0,-1);
			((CEdit*) GetDlgItem(IDC_DUSER))->SetFocus();
		}
		else if (GetDlgItem(IDC_DPASS)->m_hWnd == pMsg->hwnd)
		{
			((CEdit*) GetDlgItem(IDC_DPASS))->SetSel(0,-1);
			((CEdit*) GetDlgItem(IDC_DPASS))->SetFocus();
		}
		else if (GetDlgItem(IDC_DCPASS)->m_hWnd == pMsg->hwnd)
		{
			((CEdit*) GetDlgItem(IDC_DCPASS))->SetSel(0,-1);
			((CEdit*) GetDlgItem(IDC_DCPASS))->SetFocus();
		}
	}	
		
	return CDialog::PreTranslateMessage(pMsg);
}

void CCertLogin::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case 1000:
		HideToolTip();
		break;
	case IDT_BANNER:
		RotateBanner();
		break;
	case IDT_LEFTMOTION:
		//MessageBox("IDT_LEFTMOTION");
		change_login(1);
		break;
	case 2000:
		{
			KillTimer(2000);
			OnRun();
		}
		break;
	case 3000:
		{
			KillTimer(3000);
			if(m_bCertLogin)
				OnCert();
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CCertLogin::OnDestroy() 
{
	CDialog::OnDestroy();
	ReleaseBmpButton();
	ReleaseShapeButton();

	KillTimer(1000);

#ifndef DF_USE_CPLUS17
	if (m_pToolTip)
	{
		delete m_pToolTip;
		m_pToolTip = NULL;
	}
#endif
}

void CCertLogin::ShowToolTip()
{
	if (m_pToolTip == NULL)
	{
#ifdef DF_USE_CPLUS17
		m_pToolTip = std::make_unique<CMyToolTip>();
#else
		m_pToolTip = new CMyToolTip;
#endif
		m_pToolTip->Create(this->m_hWnd, STR_CAPS, AfxGetResourceHandle()
			,TTS_NOPREFIX | TTS_BALLOON 
			, _T("��  ��")
			);
	}
	
	if (m_pToolTip)
	{
		CRect	rect;
		CPoint	pt;

		m_btnRun->GetWindowRect(rect);
		pt.x = rect.right;
		pt.y = rect.bottom;
		m_pToolTip->ShowToolTip(true, pt);	
		SetTimer(1000, 1000 * 3, NULL);
	}
}

void CCertLogin::HideToolTip()
{
	KillTimer(1000);
	if (m_pToolTip)
		m_pToolTip->ShowToolTip(false);
}

void CCertLogin::OnDsavepass() 
{
	if (m_ckSAVEPASS.GetCheck() == BST_CHECKED)
	{
		CString	str;
		str.Format("���ڼ��� ��й�ȣ ������ �����ϼ̽��ϴ�."\
			   "\n\n"\
			   "���ڼ��� ��й�ȣ �������� ������ �ֹ����Ǹ� �帮��"\
			   "\n\n"\
			   "���� �ż��� �ŸŸ� �ϽǼ� �ֵ��� �����ϱ� ���� ���������,"\
			   "\n\n"\
			   "�ڸ��� ���� ��� ��Ÿ�ο� ���Ͽ� �ֹ��� ���ɢ�"\
			   "\n\n"\
			   "�� �� �����Ƿ� ������ ���ǰ� �䱸�˴ϴ�.");
		Axis::MessageBox(this, str, MB_OK | MB_ICONINFORMATION);
	}
	Save();
	if (GetDlgItem(IDC_DCPASS)->SetFocus() == NULL)
		GetDlgItem(IDC_DPASS)->SetFocus();
}

void CCertLogin::OnChangeDpass() 
{
	// ��й�ȣ 8�ڸ��ΰ�� ������й�ȣ�� focus������
	//UpdateData(true);
	CString	sPWD;
	GetDlgItem(IDC_DPASS)->GetWindowText(sPWD);
	
	if (sPWD != "���Ӻ�й�ȣ")
	{
		CEdit* edit = (CEdit*) GetDlgItem(IDC_DPASS);
		edit->SetPasswordChar('*');
		edit->Invalidate();
	}

	if (sPWD.GetLength() == 8 && sPWD != "���Ӻ�й�ȣ")
	{
		/*
		if (GetDlgItem(IDC_DCPASS)->SetFocus()==NULL)
			GetDlgItem(IDC_DPASS)->SetFocus();
		*/	
		//NextDlgCtrl();
		/*CEdit	*pPwdEdit = (CEdit*)GetDlgItem(IDC_DCPASS);
		pPwdEdit->SetFocus();
		pPwdEdit->SetSel(0, -1);*/
	}
}

void CCertLogin::OnChangeDcpass() 

{
	// ��й�ȣ 8�ڸ��ΰ�� ������й�ȣ�� focus������
	//UpdateData(true);
	CString	sPWD;
	GetDlgItem(IDC_DCPASS)->GetWindowText(sPWD);
	
	if (sPWD != "������ ��й�ȣ")
	{
		CEdit* edit = (CEdit*) GetDlgItem(IDC_DCPASS);
		edit->SetPasswordChar('*');
		edit->Invalidate();
	}
	
}


BOOL CCertLogin::IsNewProgressImage()
{
	CString	imgN;
	imgN.Format("%s\\%s\\DPROGRESS.BMP", Axis::home, IMAGEDIR);
	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, imgN,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		BITMAP bm;
		GetObject(hBitmap, sizeof(BITMAP), &bm);
		::DeleteObject(hBitmap);
		if (!(bm.bmWidth == 279 && bm.bmHeight == 525))	// �� �̹���
			return TRUE;		
	}
	return FALSE;
}

void CCertLogin::OnChangeDuser()
{
	UpdateData(true);

	if (m_user == "")
	{
		CEdit* edit1 = (CEdit*) GetDlgItem(IDC_DUSER);
		edit1->SetPasswordChar(0);
		//edit1->SetWindowText(m_user);
	}
	const int ipos = m_user.Find("����� ID");
	bool del_default = false;
	if ((ipos != -1) && (m_user.GetLength()>9))
	{
		m_user = m_user.Left(ipos);
		
		//edit1->SetSel(m_user.GetLength()+1, 2);
		del_default = true;
	}
	UpdateData(false);
	if (del_default)
	{
		CEdit* edit1 = (CEdit*) GetDlgItem(IDC_DUSER);
		edit1->SetSel(m_user.GetLength(), m_user.GetLength());
	}
	/*
	UpdateData(true);
	
	int ipos = m_user.Find("����� ID");
	
	if (ipos != -1)
	{
		m_user.Delete(ipos,13);
		GetDlgItem(IDC_DUSER)->SetWindowText(m_user);
		((CEdit *)GetDlgItem(IDC_DUSER))->SetSel(m_user.GetLength(),m_user.GetLength());
	}
	else
	{
		/*
		UpdateData(false);
		m_user = DeleteStr("��",m_user);-*955-
		
		GetDlgItem(IDC_DUSER)->SetWindowText(m_user);
		*/
		//((CEdit *)GetDlgItem(IDC_DUSER))->SetSel(m_user.GetLength(),m_user.GetLength());
		
		/*
		m_user = DeleteStr("��",m_user);
		m_user = DeleteStr("��",m_user);
		m_user = DeleteStr(" ",m_user);
		m_user = DeleteStr("��",m_user);
		m_user = DeleteStr("��",m_user);
		m_user = DeleteStr("��",m_user);
		*/
	
	//}


	
	/** guest ���ʿ�
	// guest�϶� 3�ڸ� �Է��� password�� focus������
	
	if (m_bOnlyGuest)
	{
		CString	sUSER;
		GetDlgItem(IDC_DUSER)->GetWindowText(sUSER);
		
		if (sUSER.GetLength() >= 3)
		{
			NextDlgCtrl();
		}
	}
	**/

}

HBRUSH CCertLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	const int ids = ::GetWindowLong(pWnd->m_hWnd, GWL_ID);
	if ( ids == IDC_SMSG)
	{
		/*	
		CDC dc; 
		dc.CreateCompatibleDC(pDC);
		dc.SelectObject(m_status);
		pDC->BitBlt(0,0,300,20,&dc,0,0,SRCCOPY);
		
		//pDC->BitBlt(100,150,300,20,&dc,0,0,SRCCOPY);
		
		dc.DeleteDC();
		*/
		
		CRect rc;
		pWnd->GetClientRect(rc);
		pDC->Rectangle(rc);//0,0,300,20);
		/*
		CDC* mDC = GetDC();
		mDC->SetBkMode(OPAQUE);
		mDC->SetBkColor(RGB(4,62,130));
		
		mDC->Rectangle(230,270,440,290);
	    mDC->DeleteDC();
		*/
		GetDlgItem(IDC_SMSG_BACK)->RedrawWindow();
		pDC->SetBkMode(TRANSPARENT);
		CString vdata(_T(""));
		pWnd->GetWindowText(vdata);
		if (vdata.Find("���Ӻ�й�ȣ��")>-1)
		{
			GetDlgItem(IDC_SMSG_BACK)->ShowWindow(SW_SHOW);
			/*
			CDC dc; 
			dc.CreateCompatibleDC(pDC);
			dc.SelectObject(m_status_err);
			pDC->BitBlt(0,0,300,20,&dc,0,0,SRCCOPY);
			dc.DeleteDC();
			*/
			pDC->SetTextColor(RGB(255,0,0));
			//return m_msgBrush_Err;
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}else
		{
			GetDlgItem(IDC_SMSG_BACK)->ShowWindow(SW_HIDE);
			CDC dc; 
			dc.CreateCompatibleDC(pDC);
			dc.SelectObject(m_status);
			pDC->BitBlt(0,0,318,30,&dc,0,0,SRCCOPY);
			
			//pDC->BitBlt(100,150,300,20,&dc,0,0,SRCCOPY);
			
			dc.DeleteDC();
			pDC->SetTextColor(RGB(0,0,0));
			//pDC->SetTextColor(RGB(255,255,255));
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
		
		//(HBRUSH)GetStockObject(NULL_BRUSH);
	}
	else if ( ids == IDC_SMSG_BACK)
	{
		/*
		CRect rc;
		pWnd->GetClientRect(rc);
		pDC->Rectangle(rc);
		//pDC->Rectangle(0,0,300,25);
		*/
		CDC dc; 
		dc.CreateCompatibleDC(pDC);
		dc.SelectObject(m_status_err);
		pDC->BitBlt(0,0,318,30,&dc,0,0,SRCCOPY);
		
		//pDC->BitBlt(100,150,300,20,&dc,0,0,SRCCOPY);
		
		dc.DeleteDC();
		return (HBRUSH)GetStockObject(NULL_BRUSH);
		
	}
	else if (ids == IDC_DCPASS || ids == IDC_DPASS || ids == IDC_DUSER)
	{
		
		CString tmp;
		GetDlgItem(ids)->GetWindowText(tmp);
		if ((tmp == "����� ID") || (tmp == "���Ӻ�й�ȣ") || (tmp == "������ ��й�ȣ"))
		{	
			pDC->SetTextColor(RGB(128,128,128));
			
		}	
		else
			pDC->SetTextColor(RGB(0,0,0));
		return m_msgBrush;
	}


		
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

CBmpButton* CCertLogin::CreateBmpButton(UINT buttonID, const char* bmpFileName, BOOL check)
{
	CBmpButton* btn = new CBmpButton;

	btn->SetImgBitmap(bmpFileName, 2);
	btn->SubclassWindow(GetDlgItem(buttonID)->m_hWnd);
	if (check)
	{
		btn->ModifyStyle(0, BS_AUTORADIOBUTTON);
		btn->ModifyStyle(0, BS_PUSHLIKE);
	}

	btn->SetWindowPos(m_bmpInput, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	m_bmpButtons.Add(btn);
	return btn;
}

void CCertLogin::ReleaseBmpButton()
{
	for (int i = 0; i < m_bmpButtons.GetSize(); i++)
	{
		delete m_bmpButtons[i];
	}
}

CShapeButton* CCertLogin::CreateShapeButton(UINT buttonID, const char* bmpFileName, CRect rc)
{
	CShapeButton* btn = new CShapeButton;

	btn->Create("", WS_CHILD | WS_VISIBLE, rc, this, buttonID);
	btn->SetImgBitmap(bmpFileName, 2);

	return btn;
}

void CCertLogin::ReleaseShapeButton()
{
	for (int i = 0; i < MAX_SHAPE; i++)
	{
		if (m_shapeButtons[i])
		{
			m_shapeButtons[i]->DestroyWindow();
			delete m_shapeButtons[i];
			m_shapeButtons[i] = NULL;
		}
	}
}

void CCertLogin::OnEvent1()
{
	//((CButton *) GetDlgItem(IDC_DSAVEID))->SetCheck(TRUE);
	SetEventBitmap(1);
	KillTimer(IDT_BANNER);
}

void CCertLogin::OnEvent2()
{
	
	SetEventBitmap(2);
	KillTimer(IDT_BANNER);
}

void CCertLogin::OnEvent3()
{
	
	SetEventBitmap(3);
	KillTimer(IDT_BANNER);
}


void CCertLogin::SetEventBitmap(int index)
{
	m_bmpEvent = new CStatic;

	const CRect rc(0, 0, 270, 404);
	
	m_bmpEvent->Create("", m_static.GetStyle(), rc, this, IDC_STATIC);
	m_bmpEvent->SetWindowPos(&m_static, 0, 0, 270, 404, SWP_NOSIZE );

	if (m_bitmapEvent)
	{
		DeleteObject(m_bitmapEvent);
		m_bitmapEvent = NULL;
	}

	CString fileName;
	fileName.Format("%s\\%s\\NEW_LOGIN_BANNER.bmp", Axis::home, IMAGEDIR);

	m_bitmapEvent = CEnBitmap::LoadImageFile(fileName, RGB(255, 255, 255));

	if (m_bitmapEvent)
	{
		m_bmpEvent->SetBitmap(m_bitmapEvent);
		m_bmpEvent->ShowWindow(SW_SHOW);
	}
	else
	{
		m_bmpEvent->ShowWindow(SW_HIDE);
	}
}

void CCertLogin::SetDownloadFile(CString fileName)
{
	const int find = fileName.Find('\t');
	if (find)
		fileName = fileName.Left(find);

	
	SetDlgItemText(IDC_SMSG, CString("�ٿ�ε���.. ") + fileName);
	//CRect rc = CRect(90,292,240+90,+292+12);
	//InvalidateRect(rc); //���¹� ����� ����ȭ�ؼ� �Է��� ȭ�� ����
}

void CCertLogin::RotateBanner()
{
	
	if (m_currEvent == -1)
		return;

	m_currEvent++;
	if (m_currEvent >= 2)
		m_currEvent = 0;
	SetEventBitmap(m_currEvent + 1);

	CString s;
	s.Format("Rotate Banner [%d]\n",m_currEvent);
	OutputDebugString(s);
/*
	for (int i = IDC_EVENT1; i < IDC_EVENT1 + 3; i++)
		GetDlgItem(i)->SendMessage(BM_SETCHECK);
	
	CWnd* btn = GetDlgItem(IDC_EVENT1 + m_currEvent);
	if (btn)
		btn->SendMessage(BM_SETCHECK, TRUE);
*/
}

void CCertLogin::OnShapeButton(int index)
{
	/*
	CString slog;
	slog.Format("%d", index);
	//�������� 
	if(index == 5)
	{
		OnEasyCert();
		return;
	}
	//@@�������� 
	*/

	const char* const urls[MAX_SHAPE] =
	{
		"https://www.ibks.com/quick_start.jsp?fromUrl=joinHTS",	// ó���̼���?
		"https://www.ibks.com/member/idsearch/associate.jsp",		// ID�н�
		"https://www.ibks.com/member/pwcancel/pwretry.jsp",	//��й�ȣ�н�
		"www.ibks.com/customer/certificate/cert_main.jsp",	// ��������
		"http://113366.com/ibks",								// �¶��ε����
		"http://www.ibks.com/customer/stipulation/info02_attendedview.do?seq=36&qs_flag=N",					// ����ֹ�  www.ibks.com/wts/index_view.do
		"http://www.ibks.com/customer/dealing/ars.jsp"			// ����������
	};

	if (index >= 0 && index < MAX_SHAPE)
	{
		if (strlen(urls[index]) > 0)
			ShellExecute(NULL, _T("open"), urls[index], NULL,NULL, SW_SHOWNORMAL);
		else
			Axis::MessageBox(this, "�غ���");
	}
}

void CCertLogin::DeleteResourceInfoFile()
{
	TRY 
	{
		CFile::Remove(Format("%s\\%s\\infoAXIS", Axis::home, TABDIR));
	}
	CATCH (CFileException, e)
	{
	}
	END_CATCH;

	TRY
	{
		CFile::Remove(Format("%s\\%s\\infoRSC", Axis::home, TABDIR));
	}
	CATCH (CFileException, e)
	{
	}
	END_CATCH;
}

void CCertLogin::ApplySecurity()
{
	GetParent()->SendMessage(WM_SECURETOOLS);
}

void CCertLogin::change_login(int mode)
{
	BITMAP bm;
	GetObject(m_bitmap, sizeof(bm), &bm);
	
	CDC Main_DC, *pDC = GetDC();
	if (pDC)
	{
		Main_DC.CreateCompatibleDC(pDC);
		ReleaseDC(pDC);
	}
	
	SelectObject(Main_DC, m_bitmap);

	//event_pos_x = 100;
	
	if (event_pos_x < 0)
	{
		event_pos_x = 20;
	}
	else if (event_pos_x < 250)
	{
		if (event_pos_x < 40) event_pos_x = event_pos_x+7;
		else if (event_pos_x < 100) event_pos_x = event_pos_x+10;
		else if (event_pos_x < 200) event_pos_x = event_pos_x+25;
		else if (event_pos_x < 220) event_pos_x = event_pos_x+45;
		else if (event_pos_x < 240) event_pos_x = event_pos_x+7;
		else if (event_pos_x < 250) event_pos_x = event_pos_x+1;
		
		//Invalidate();
	}
	else
	{
		//event_pos_x = 270;
		KillTimer(IDT_LEFTMOTION);
		Invalidate(true);
	}
	
	//Main_DC.BitBlt(event_pos_x,0,bm.bmWidth,bm.bmHeight,&Mask_DC,0,0,SRCCOPY);
	//TransparentBlt(Main_DC,0,0,bm.bmWidth,bm.bmHeight,Sing_DC,0,0,bm.bmWidth,bm.bmHeight,Axis::maskColor);
	Main_DC.DeleteDC();	
	
	
	MakeShape();
	
	if (event_pos_x != 250)
	{
		//Invalidate();
	}
	

}

void CCertLogin::SomeDelay(HWND hWnd)
{
    int count = 0;
	MSG message{};
    while(++count <= 1000)
    {
        if(::PeekMessage(&message, hWnd, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&message);
            ::DispatchMessage(&message);
        }
        if(count % 100 == 0)
            Sleep(1);
    }
}

void CCertLogin::OnMode()
{
	return;
	BITMAP bm;
	GetObject(m_bitmap, sizeof(bm), &bm);
	
	CDC Main_DC, *pDC = GetDC();
	if (pDC)
	{
		Main_DC.CreateCompatibleDC(pDC);
		ReleaseDC(pDC);
	}
	
	//event_pos_x = 100;
	if (m_mode == 0)
	{
		event_pos_x = 0;
		m_mode = 1;
	}
	else
	{
		event_pos_x = 259;
		m_mode = 0;
	}
	
//	Main_DC.BitBlt(event_pos_x,0,bm.bmWidth,bm.bmHeight,&Mask_DC,0,0,SRCCOPY);
//	TransparentBlt(Main_DC,0,0,bm.bmWidth,bm.bmHeight,Sing_DC,0,0,bm.bmWidth,bm.bmHeight,Axis::maskColor);
	
	if (bLoginVerUp)
	{
		
		m_bitmap = ((CBitmap*) Axis::GetBitmap(IDB_VERUP))->operator HBITMAP();
		//ASSERT(m_bitmap);
		
		
		//SelectObject(Sing_DC, (HBITMAP)m_bitmaps->operator HBITMAP());
		//Main_DC.BitBlt(0,0,bm.bmWidth,bm.bmHeight,&Sing_DC,0,0,SRCCOPY);
		

		//BITMAP bm;
		//m_bitmap->GetBitmap(&bm);
		//SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE | SWP_NOZORDER);
		
	}
	SelectObject(Main_DC, m_bitmap);
//	Sing_DC.DeleteDC();
//	Mask_DC.DeleteDC();
	Main_DC.DeleteDC();	
	
	
	MakeShape();
	/*
	CWinApp* app = AfxGetApp();
	app->WriteProfileInt(WORKSTATION, "RightMode", (m_mode == 0)? 1: 0 );
	if (m_mode == 0)
	{
		GetDlgItem(IDC_MODE)->SetWindowPos(m_bmpInput ? m_bmpInput: &m_static,  615, 115,  15, 15, 0);
		m_btnMode->SetImgBitmap("LOGIN_BTN_MODE", 2);
		
	}
	else
	{
		GetDlgItem(IDC_MODE)->SetWindowPos(m_bmpInput ? m_bmpInput: &m_static,  370, 115,  15, 15, 0);
		m_btnMode->SetImgBitmap("LOGIN_BTN_MODE2", 2);
		
	}
	m_btnMode->ShowWindow(SW_HIDE);
	*/
}

void CCertLogin::OnLoginUpdateMode()
{
	CString	fontName;
	fontName.LoadString(ST_FONTNAME_DEFAULT2);
	/*
	m_fontBold.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE,
			false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, fontName);

	m_font.CreatePointFont(90, fontName);
	*/
	CBitmap* m_bitmap = Axis::GetBitmap(IDB_LOGOUT_BG);
	ASSERT(m_bitmap);

	BITMAP bm;
	m_bitmap->GetBitmap(&bm);
	SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE | SWP_NOZORDER);

	HRGN hWndRgn = DIBToRgn((HBITMAP)m_bitmap->m_hObject, Axis::maskColor, FALSE);
	if(hWndRgn)
	{
		SetWindowRgn(hWndRgn,TRUE);
		::DeleteObject(hWndRgn);
	}

	//SetEventBitmap( 1 );
}

bool CCertLogin::IsNumber(CString str)
{
	if (str.GetAt(0) < 0 || str.GetAt(0) > 127)
		return false;

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


CString CCertLogin::DeleteStr(CString p1,CString p2)
{
	
	const int iposs = p2.Find(p1);
	
	if (iposs != -1)
	{
		p2.Delete(iposs,2);
		
	}
	
	return p2;
}



BOOL CCertLogin::RunUpdate()
{
	
	CString	aps, cmds, exes;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	const CAxisApp* app = (class CAxisApp *) AfxGetApp();
	/*
	AfxMessageBox(app->m_regkey);
	AfxMessageBox(app->m_conIP);
	AfxMessageBox(app->m_forceIP);
	AfxMessageBox(Axis::home);
	*/
	cmds = "update";
	
	
	
	if (!app->m_forceIP.IsEmpty())
		cmds += _T(" /a "+app->m_forceIP);
	else
	{
		
		if ((app->m_conIP == "172.16.205.20")||(app->m_conIP == "172.16.202.106")||(app->m_conIP == "172.16.202.171")||(app->m_conIP == "211.255.204.19")||(app->m_conIP == "211.255.204.134"))
			cmds += _T(" /a "+app->m_conIP);
		
	}

	CString reg; reg.Format(" /k %s", app->m_regkey);
	cmds += reg;
	
	
	if (!Axis::user.IsEmpty())
	{
		CString user, pass, cpass;

		cmds += _T(" /i");

		GetDlgItem(IDC_DUSER)->GetWindowText(user);
		GetDlgItem(IDC_DPASS)->GetWindowText(pass);
		GetDlgItem(IDC_DCPASS)->GetWindowText(cpass);
  
		cmds += " ";
		cmds += user;
		cmds += " ";
		cmds += pass;
		cmds += "#";
		cmds += cpass;
	}
 
	
	//AfxMessageBox(cmds);
	//return false;
	
	aps.Format("%s\\%s\\update.exe", Axis::home, RUNDIR);
	const BOOL bRc = CreateProcess(
			aps,				// application name
			(char*)(const char*)cmds,// command line
			NULL,				// process attribute
			NULL,				// thread attribute
			FALSE,				// is inherit handle
			0,				// creation flags
			NULL,				// environment
			NULL,				// current directory
			&si,				// STARTUPINFO
			&pi);				// PROCESS_INFORMATION

	return bRc;
	
	return false;
}

void CCertLogin::RunCopyCfg()
{
	CString	aps, cmds, exes;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	const CAxisApp* app = (class CAxisApp *) AfxGetApp();
	cmds = "update";
	
	
	
	CString reg; reg.Format(" %s", app->m_regkey);
	cmds += reg;
	
	
	
	aps.Format("%s\\%s\\conf_copy.exe", Axis::home, RUNDIR);
	const BOOL bRc = CreateProcess(
			aps,				// application name
			(char*)(const char*)cmds,// command line
			NULL,				// process attribute
			NULL,				// thread attribute
			FALSE,				// is inherit handle
			0,				// creation flags
			NULL,				// environment
			NULL,				// current directory
			&si,				// STARTUPINFO
			&pi);				// PROCESS_INFORMATION	
}

int CCertLogin::ShowMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return MessageBox(lpszText, lpszCaption, nType);
}

void CCertLogin::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CDialog::OnShowWindow(bShow, nStatus);

	//2015.10.10 dkkim
	//�������α׷� ���� ��� ����
	if (bShow)
	{
		m_frame->PostMessage(WM_SECURE,0,(LPARAM)m_auto);
	}
	CString s;
	s.Format("PASSWORD 22 [%d]\n",m_bCertLogin);
	OutputDebugString(s);
	if(m_bCertLogin)
		OnCert();
}

//2015.10.10 dkkim
//�������α׷� ���� ��� ����
void CCertLogin::SetEditFocus()
{
	if (m_user != "" && m_user != "����� ID")
	{
		if(GetFocus() != GetDlgItem(IDC_DPASS))
			((CEdit*) GetDlgItem(IDC_DPASS))->SetFocus();
	}
	else
	{
		((CEdit*) GetDlgItem(IDC_DUSER))->SetSel(0,-1);
		((CEdit*) GetDlgItem(IDC_DUSER))->SetFocus();	
	}	
}

bool CCertLogin::GetCertLogin()
{
	return m_bCertLogin;
}

void CCertLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(m_bmpBg)
	{
		CDC mdc;
		mdc.CreateCompatibleDC(&dc);

		BITMAP bm;
		m_bmpBg->GetBitmap(&bm);
		
		CBitmap* oldBmp = mdc.SelectObject(m_bmpBg);
		
		dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);
		
		mdc.SelectObject(oldBmp);
		mdc.DeleteDC();
	}
}

HWND CCertLogin::GetPassHwnd()
{
	return GetDlgItem(IDC_DPASS)->GetSafeHwnd();
}

HWND CCertLogin::GetCPassHwnd()
{
	return GetDlgItem(IDC_DCPASS)->GetSafeHwnd();
}

void CCertLogin::SetAK( IAstxMkd8* AkSdk )
{
	m_AkSdk = AkSdk;
}

void CCertLogin::OnCertStart()
{
	if (((CButtonST *) GetDlgItem(IDC_DCSTART))->GetCheck() == BST_CHECKED)
		m_bCertStart = true;
	else
		m_bCertStart = false;

	Save();
}

void CCertLogin::OnBanner()
{
	CString	Path;
	Path.Format("%s\\%s\\BANNER.INI", Axis::home, "tab");
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("LOGIN_BANNER","url","http://www.ibks.com",readB,sizeof(readB),Path);
	
	CString url(readB,readL);
	
	ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
}

void CCertLogin::OnLButtonUp( UINT nFlags, CPoint point )
{
	CRect rc;
	
	((CStatic*)GetDlgItem(IDC_STATIC))->GetWindowRect(rc);
	
	ScreenToClient(rc);
	
	if(rc.PtInRect(point))
	{
		CString	Path;
		Path.Format("%s\\%s\\BANNER.INI", Axis::home, "tab");
		
		char readB[1024];
		int readL;
		
		readL = GetPrivateProfileString("LOGIN_BANNER","url","http://www.ibks.com",readB,sizeof(readB),Path);
		
		CString url(readB,readL);

		ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
	}
	CDialog::OnLButtonUp(nFlags, point);
}

//��������
typedef CWnd* (__stdcall*pQrShow)			(CWnd*,CHAR*,CHAR*,int, CString, CString);
pQrShow			pQrfunc		= NULL;
void CCertLogin::OnEasyCert()
{
	((CMainFrame*)m_frame)->m_bSimpleAuth = true;
	m_frame->PostMessage(WM_AXIS, MAKEWPARAM(axSTART, 1));
}


void CCertLogin::ShowQRDlg()
{
	CString spath;
//	spath =  Axis::home + "\\dev\\cx_SimpleAuth.dll";
	spath =  Axis::home + "\\exe\\cx_SimpleAuth.dll";
	HMODULE hModule = LoadLibrary(spath);	 
	
CRect rec;
GetWindowRect(rec);
m_slog.Format("[QRCODE] [CCertLogin] ShowQRDlg ---------- [%d][%d][%d][%d]", rec.left, rec.top, rec.right, rec.bottom);
OutputDebugString(m_slog);

	if(hModule)
	{
		pQrfunc	= (pQrShow)	::GetProcAddress(hModule, "ShowQrCode");
	//	char* pdata = new char[1];
		std::unique_ptr<char[]>pdata = std::make_unique<char[]>(1);
		memset(pdata.get(), 0x00, 1);
		char  qrdata[2048] = {0,};
		if(pQrfunc)
		{
//			spath.Replace("\\dev\\cx_SimpleAuth.dll", "");
			spath.Replace("\\exe\\cx_SimpleAuth.dll", "");
			CString sResult;
			sResult.Format("%.1s", (char*)pQrfunc(this, (LPSTR)(LPCTSTR)spath, qrdata, 1, ((CMainFrame*)m_frame)->m_ip, ((CMainFrame*)m_frame)->m_port));
			
m_slog.Format("[QRCODE] [CCertLogin] ShowQRDlg result=[%s]", qrdata);
OutputDebugString(m_slog);
			
			if(sResult == "1")  //qrlogin �����̸�  
				((CMainFrame*)m_frame)->signOnSimpleAuth(qrdata);
		}
		FreeLibrary(hModule);
	}
}

#pragma warning(default : 26409)
#pragma warning(default : 26400)
#pragma warning (default : 6273)
#pragma warning (default : 6387)
#pragma warning (default : 6011)

//CFileFind ff;
//CString Version_path;
//Version_path = Axis::home + "\\exe\\VERSION.exe";
//if (ff.FindFile(Version_path) && (tmp_mod < 5))
//{
//	CString	aps, cmds, exes;
//	STARTUPINFO		si;
//	PROCESS_INFORMATION	pi;

//	ZeroMemory(&si, sizeof(STARTUPINFO));
//	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

//	si.cb = sizeof(STARTUPINFO);
//	si.dwFlags = STARTF_USESHOWWINDOW;
//	si.wShowWindow = SW_SHOW;

//	const CAxisApp* app = (class CAxisApp*)AfxGetApp();
//	char	buffer[1024];
//	GetClassName(m_hWnd, buffer, sizeof(buffer));
//	cmds.Format(" /c %d /d \"%s\" /k \"%s\" ", AfxGetMainWnd()->m_hWnd, Axis::home, app->m_regkey);
//	aps.Format("%s\\%s\\VERSION.exe", Axis::home, RUNDIR);

//	const BOOL bRc = CreateProcess(
//		aps,				// application name
//		(char*)(const char*)cmds,// command line
//		NULL,				// process attribute
//		NULL,				// thread attribute
//		FALSE,				// is inherit handle
//		0,					// creation flags
//		NULL,				// environment
//		NULL,				// current directory
//		&si,				// STARTUPINFO
//		&pi);				// PROCESS_INFORMATION


//	m_frame->PostMessage(WM_CLOSE);
//	EndDialog(IDCANCEL);
//}