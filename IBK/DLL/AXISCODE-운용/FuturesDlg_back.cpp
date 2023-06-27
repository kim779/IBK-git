// FuturesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FuturesDlg.h"
#include "AxisCode.h"

#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFuturesDlg dialog
#define BOND		1998
#define Y3_BOND		BOND+1
#define Y5_BOND		BOND+2
#define Y10_BOND	BOND+3
#define Y310_BOND	BOND+4
// #define SEC_BOND	BOND+4

#define MONEY		2998
#define USD			MONEY+1
#define YEN			MONEY+2
#define EURO		MONEY+3
#define YUAN		MONEY+4		//2015.10.05 KSJ ����ȭ �߰�

#define COMM		3998
#define GOLD		COMM+1
#define PIG			COMM+2
#define MINIGOLD    COMM+3

#define MINIK200	4998		//2015.06.30 KSJ �̴� �ڽ���200 �߰�

#define STANDARDNUM		11
#define ROWCOUNT		23
#define SF_ROWCOUNT		18	//�������� + �����ڻ�
#define MON_COUNT		7

void writelogs(CString strlog)
{
	return;
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("----------%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	
	CString strDay;
	strDay.Format(_T("%d%d"), oTime.GetMonth(), oTime.GetDay());
	
	
	CString file;
	file.Format("C:\\testlog");
	
	CFileFind ff; 
	if(!ff.FindFile(file))
	{
		::CreateDirectory(file, nullptr);
	}
	
	CString strFile;	 
	strFile.Format("C:\\testkog\\%s","log.ini"); 
	WritePrivateProfileString("log", strCurTime, strlog, strFile);
}

void CSortStringArray::Sort()
{
	BOOL bNotDone = TRUE;
	
	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < GetUpperBound();pos++)
			bNotDone |= CompareAndSwap(pos);
	}
}

BOOL CSortStringArray::CompareAndSwap(int pos)
{
	CString temp;
	const int posFirst = pos;
	const int posNext = pos + 1;
	
	if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)
	{
		temp = GetAt(posFirst);
		SetAt(posFirst, GetAt(posNext));
		SetAt(posNext, temp);
		return TRUE;
		
	}
	return FALSE;
}

CFuturesDlg::CFuturesDlg(int kind, CPoint pt, int type /*= 0*/, CWnd* pParent /*=nullptr*/)
	: CDialog(CFuturesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFuturesDlg)
	m_szEditCode = _T("");
	m_szEditName = _T("");
	m_kindList = 0;
	//}}AFX_DATA_INIT

	m_pCheck = nullptr;	
	m_pStock = nullptr;
	m_arrayFcode.RemoveAll();
	m_arrayOcode.RemoveAll();
	
	m_arrayPcode.RemoveAll();
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll();
	m_ListPcode.RemoveAll();

	m_Kind = kind;
	m_type = type;
	m_point = pt;
	m_nCountMonth = 0;
	m_nStep   = futureCODE;
	m_bKostar = FALSE;
	m_nMiniK200 = 0;

	//2015.09.14 KSJ ini ���ϸ�
	m_root = ((CAxisCodeApp*)AfxGetApp())->m_root;
	m_user = ((CAxisCodeApp*)AfxGetApp())->m_folder;
	m_file = m_root + "\\user\\" + m_user + "\\" + m_user + ".ini";
	m_bMini = false;
	m_iGubn = NORMAKKOSPI200;
}	


void CFuturesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFuturesDlg)
	DDX_Control(pDX, IDC_BUTTON_SEARCH_PREV, m_btnSearchPrev);
	DDX_Control(pDX, IDC_BUTTON_SEARCH_NEXT, m_btnSearchNext);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH92, m_btnSPMonth92);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH62, m_btnSPMonth62);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH32, m_btnSPMonth32);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH122, m_btnSPMonth122);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH12, m_btnSPMonth12);
	DDX_Control(pDX, IDC_BUTTON_MONTH92, m_btnMonth92);
	DDX_Control(pDX, IDC_BUTTON_MONTH62, m_btnMonth62);
	DDX_Control(pDX, IDC_BUTTON_MONTH32, m_btnMonth32);
	DDX_Control(pDX, IDC_BUTTON_MONTH122, m_btnMonth122);
	DDX_Control(pDX, IDC_BTN_MONEY, m_btnMoney);
	DDX_Control(pDX, IDC_BTN_GUMRI, m_btnGumri);
	DDX_Control(pDX, IDC_BTN_COMM, m_btnComm);
	DDX_Control(pDX, IDC_LIST_CODE_F, m_codeList);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH9, m_ctlButtonSPMonth9);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH6, m_ctlButtonSPMonth6);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH3, m_ctlButtonSPMonth3);
	DDX_Control(pDX, IDC_BUTTON_OPMONTH4, m_ctlButtonOPMonth4);
	DDX_Control(pDX, IDC_BUTTON_OPMONTH3, m_ctlButtonOPMonth3);
	DDX_Control(pDX, IDC_BUTTON_OPMONTH2, m_ctlButtonOPMonth2);
	DDX_Control(pDX, IDC_BUTTON_OPMONTH1, m_ctlButtonOPMonth1);
	DDX_Control(pDX, IDC_BUTTON_KOSTAR, m_ctlButtonKostar);
	DDX_Control(pDX, IDC_BUTTON_JOPTION, m_ctlButtonJOption);
	DDX_Control(pDX, IDC_PCODE_LIST, m_plistbox);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ctlEditName);
	DDX_Control(pDX, IDC_EDIT_CODE, m_ctlEditCode);
	DDX_Control(pDX, IDC_BUTTON_MONTH12, m_ctlButtonMonth12);
	DDX_Control(pDX, IDC_BUTTON_MONTH3, m_ctlButtonMonth3);
	DDX_Control(pDX, IDC_BUTTON_MONTH6, m_ctlButtonMonth6);
	DDX_Control(pDX, IDC_BUTTON_MONTH9, m_ctlButtonMonth9);
	DDX_Control(pDX, IDC_BUTTON_NAME, m_ctlButtonName);
	DDX_Control(pDX, IDC_BUTTON_KOSPI200, m_ctlButtonKospi);
	DDX_Control(pDX, IDC_BUTTON_NAME2, m_ctlButtonName2);
	DDX_Control(pDX, IDC_BTN_MINIK200, m_ctlButtonMiniK200);
	DDX_Control(pDX, IDC_BUTTON_MONTH123, m_ctlButtonUSD);
	DDX_Control(pDX, IDC_BUTTON_MONTH124, m_ctlButtonUSD1);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH123, m_ctlButtonSPUSD);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH124, m_ctlButtonSPUSD1);
	DDX_Control(pDX, IDC_BTN_WEEKOP, m_ctlButtonWEEKLYOP);
	DDX_Control(pDX, IDC_BTN_WEEK_IN_DLG, m_ctlButtonOPWeek);
	DDX_Text(pDX, IDC_EDIT_CODE, m_szEditCode);
	DDX_Text(pDX, IDC_EDIT_NAME, m_szEditName);
	DDX_Radio(pDX, IDC_PRICE_RDO, m_kindList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFuturesDlg, CDialog)
	//{{AFX_MSG_MAP(CFuturesDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_KOSPI200, OnButtonKospi200)
	ON_BN_CLICKED(IDC_BTN_MINIK200, OnButtonMiniK200)
	ON_BN_CLICKED(IDC_BUTTON_NAME2, OnButtonName2)
	ON_BN_CLICKED(IDC_BUTTON_NAME, OnButtonName)
	ON_BN_CLICKED(IDC_BUTTON_MONTH9, OnDoubleclickedButtonMonth9)
	ON_BN_CLICKED(IDC_BUTTON_MONTH6, OnDoubleclickedButtonMonth6)
	ON_BN_CLICKED(IDC_BUTTON_MONTH3, OnDoubleclickedButtonMonth3)
	ON_BN_CLICKED(IDC_BUTTON_MONTH12, OnDoubleclickedButtonMonth12)
	ON_BN_CLICKED(IDC_BUTTON_MONTH92, OnDoubleclickedButtonMonth92)
	ON_BN_CLICKED(IDC_BUTTON_MONTH62, OnDoubleclickedButtonMonth62)
	ON_BN_CLICKED(IDC_BUTTON_MONTH32, OnDoubleclickedButtonMonth32)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_PCODE_LIST, OnDblclkPcodeList)
	ON_LBN_SELCHANGE(IDC_PCODE_LIST, OnSelchangePcodeList)
	ON_BN_CLICKED(IDC_BUTTON_JOPTION, OnButtonJoption)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CODE_F, OnClickListCode)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE_F, OnDblclkListCode)
	ON_BN_CLICKED(IDC_BUTTON_OPMONTH1, OnButtonOpmonth1)
	ON_BN_CLICKED(IDC_BUTTON_OPMONTH2, OnButtonOpmonth2)
	ON_BN_CLICKED(IDC_BUTTON_OPMONTH3, OnButtonOpmonth3)
	ON_BN_CLICKED(IDC_BUTTON_OPMONTH4, OnButtonOpmonth4)
	ON_BN_CLICKED(IDC_PRICE_RDO, OnPOPTIONRdo)
	ON_BN_CLICKED(IDC_BUTTON_KOSTAR, OnButtonKostar)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH3, OnDoubleclickedButtonSpmonth3)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH6, OnDoubleclickedButtonSpmonth6)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH9, OnDoubleclickedButtonSpmonth9)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH12, OnDoubleclickedButtonSpmonth12)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH32, OnDoubleclickedButtonSpmonth32)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH62, OnDoubleclickedButtonSpmonth62)
	ON_BN_CLICKED(IDC_BTN_GUMRI, OnBtnGumri)
	ON_BN_CLICKED(IDC_BTN_MONEY, OnBtnMoney)
	ON_BN_CLICKED(IDC_BTN_COMM, OnBtnComm)
	ON_BN_CLICKED(IDC_BUTTON_MONTH32, OnButtonMonth32)
	ON_BN_CLICKED(IDC_BUTTON_MONTH62, OnButtonMonth62)
	ON_BN_CLICKED(IDC_BUTTON_MONTH92, OnButtonMonth92)
	ON_BN_CLICKED(IDC_BUTTON_MONTH122, OnButtonMonth122)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH12, OnButtonSpmonth12)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH32, OnButtonSpmonth32)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH62, OnButtonSpmonth62)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH92, OnButtonSpmonth92)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH122, OnButtonSpmonth122)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_PREV, OnButtonSearchPrev)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_NEXT, OnButtonSearchNext)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MONTH123, OnButtonMonth123)
	ON_BN_CLICKED(IDC_BUTTON_MONTH124, OnButtonMonth124)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH123, OnButtonSpmonth123)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH124, OnButtonSpmonth124)
	ON_BN_CLICKED(IDC_BTN_WEEKOP, OnBtnWeekop)
	ON_BN_CLICKED(IDC_CODE_RDO, OnPOPTIONRdo)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_WEEK_IN_DLG, OnBtnWeek)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFuturesDlg message handlers

BOOL CFuturesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetFocus();
	// TODO: Add extra initialization here
	SetDlgPos(m_point.x, m_point.y);
	
	CString szTitle, path;

	((CStatic*)GetDlgItem(IDC_STATIC_CAPTION))->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_EDIT_SEARCH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_NAME2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_SEARCH_NEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_SEARCH_PREV)->ShowWindow(SW_HIDE);

	//2015.09.14 KSJ �̴����� üũ
	//m_bMini = GetPrivateProfileInt("AxisCode", "mini", 0, m_file);
	m_iGubn = GetPrivateProfileInt("AxisCode", "mini", 0, m_file);
	
	if(m_Kind == sfCODE)  //122 �ֽļ���
	{ 
		const CRect rc(110, 20, 447, 267+50);

		HideFutureBtns(TRUE);
		
		m_pStock = std::make_unique<CStockCheckBox>();

		m_pStock->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_GROUP, rc, this, IDC_WND_CHECK);
		m_pStock->ShowWindow(SW_SHOW);
		m_pStock->SetWnd(this);
		
		m_pStock->SetCheckCursor(IDC_CURSOR_HAND);

		loadSfCode();

		m_ctlButtonKostar.EnableWindow(TRUE);
		m_ctlButtonKospi.EnableWindow(FALSE);
		m_ctlButtonMiniK200.EnableWindow(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
		m_ctlButtonJOption.EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_SEARCH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SEARCH_NEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SEARCH_PREV)->ShowWindow(SW_SHOW);

		OnButtonKostar();
	}
	else
	{
		CRect rc(110, 42 + (m_Kind!=optionCODE?24:0), 447, 267+50);

		m_pCheck = std::make_unique<CCheckBox>();

	//	if (m_point.x > 0 || m_point.y > 0)		// ���̾�α��� ��ġ �ʱ�ȭ..
	//		SetWindowPos(nullptr, m_point.x, m_point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

		((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BUTTON_NAME2))->ShowWindow(SW_HIDE);	//2015.06.30 KSJ �̴� �ڽ��� 200 �߰�
		((CButton*)GetDlgItem(IDC_BTN_WEEK_IN_DLG))->ShowWindow(SW_HIDE);

		m_pCheck->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_GROUP, rc, this, IDC_WND_CHECK);
		m_pCheck->ShowWindow(SW_SHOW);
		m_pCheck->SetWnd(this);
		
		if (m_Kind !=futureCODE/* && m_Kind != all2CODE*/)
			m_pCheck->SetCheckCursor(IDC_CURSOR_HAND);

		loadPjCode();
		SetButton();

		if (m_Kind == all2CODE)
		{
			m_ctlButtonKospi.SetBtnCursor(IDC_CURSOR_HAND);
			m_ctlButtonKospi.SetBold(FALSE);
		}

		if (m_Kind == optionCODE)  //102 �ɼ����� 
		{
			CRect rc;

			//2015.06.30 KSJ �̴� �ڽ���200 �߰�
			m_ctlButtonName2.GetWindowRect(&rc);
			ScreenToClient(&rc);
			rc.OffsetRect(-2, -80);
			m_ctlButtonName2.SetWindowPos(nullptr, rc.left, rc.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

			m_ctlButtonName.GetWindowRect(&rc);
			ScreenToClient(&rc);
			rc.OffsetRect(-2, -80);
			m_ctlButtonName.SetWindowPos(nullptr, rc.left, rc.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

			CRect tmpRec;
			m_ctlButtonOPWeek.GetWindowRect(&tmpRec);
			ScreenToClient(&tmpRec);
			tmpRec.OffsetRect(-2, -80);
			m_ctlButtonOPWeek.SetWindowPos(nullptr, tmpRec.left, tmpRec.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
			

			//2014.08.26 KSJ �ȳ����׹���
		//	rc.OffsetRect(2, 267+38);
			rc.OffsetRect(2, 267 + 25);

			((CStatic*)GetDlgItem(IDC_STATIC_CAPTION))->ShowWindow(SW_SHOW);
			((CStatic*)GetDlgItem(IDC_STATIC_CAPTION))->SetWindowPos(nullptr, rc.left, rc.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

			//2015.09.14 KSJ
		//	if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
		//	else		OnButtonKospi200();
			if(m_iGubn == NORMAKKOSPI200 )		OnButtonKospi200();
			else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
			else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
		}

		loadFjCode();   // ���� �ҷ����� ...
		SetFutureCodes();

		switch (m_Kind)		// ������ ����..
		{
		case customF2Code:
			szTitle = "����/�������� �ڵ� �ȳ�";
	//		if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
	//		else		OnButtonKospi200();
			if(m_iGubn == NORMAKKOSPI200)		OnButtonKospi200();
			else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
			else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
			m_btnGumri.EnableWindow( TRUE );
			m_btnMoney.EnableWindow( TRUE );
			m_btnComm.EnableWindow( TRUE );
			m_ctlButtonKostar.EnableWindow(FALSE);	//2016.06.28 KSJ 8830ȭ�鿡�� �ֽļ��� ��ư Disable
			break;
		case foptionCODE:
			szTitle = "����/�ɼ� �ڵ� �ȳ�";
			if (m_type == 1)
			{
				//OnButtonKostar();
				m_ctlButtonKospi.EnableWindow(FALSE);
				m_ctlButtonMiniK200.EnableWindow(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
			}
			else if (m_type == 2)
			{
				// Kospi + kostar + spread
				//OnButtonKostar();
		//		if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
		//		else		OnButtonKospi200();
				if(m_iGubn == NORMAKKOSPI200)		OnButtonKospi200();
				else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
				else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
				break;
			}
			else
			{
				if (m_type == 0)
					m_ctlButtonKostar.EnableWindow(FALSE);
			//	if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
			//	else		OnButtonKospi200();
				if(m_iGubn == NORMAKKOSPI200)		OnButtonKospi200();
				else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
				else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
			}
			DisableSpread();
			break;
		case futureCODE:
			szTitle = "���� �ڵ� �ȳ�";
		//	if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
		//	else		OnButtonKospi200();
			if(m_iGubn == NORMAKKOSPI200)		OnButtonKospi200();
			else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
			else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
			break;
		case optionCODE:
			szTitle = "�ɼ� �ڵ� �ȳ�";
			m_nStep = optionCODE;
			DisableFuture();
	//		if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
	//		else		OnButtonKospi200();
			if(m_iGubn == NORMAKKOSPI200 )		OnButtonKospi200();
			else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
			else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
			break;  
		case koptionCODE:
			szTitle = "�ֽĿɼ� �ڵ� �ȳ�";
			DisableFuture();
			m_ctlButtonJOption.EnableWindow(TRUE);
			ShowJOption(TRUE);
			OnPOPTIONRdo();
			m_plistbox.EnableWindow();
			m_plistbox.SetCurSel(0);
			OnDblclkPcodeList();
			break;
		case faoptionCODE: // ����
	//		if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
	//		else		OnButtonKospi200();
			if(m_iGubn == NORMAKKOSPI200)		OnButtonKospi200();
			else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
			else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
			szTitle = "�ڵ� �ȳ�";
			break;
		case all2CODE:
			szTitle = "����/�ɼ� �ڵ� �ȳ�";
	//		if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
	//		else		OnButtonKospi200();
			if(m_iGubn == NORMAKKOSPI200)		OnButtonKospi200();
			else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
			else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
			break;
		case spreadCODE:
			szTitle = "��������";
			if (m_type == 1)
			{
				//OnButtonKostar();
				m_ctlButtonKospi.EnableWindow(FALSE);
				m_ctlButtonMiniK200.EnableWindow(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
			}
			else
			{
				if (m_type == 0)
					m_ctlButtonKostar.EnableWindow(FALSE);
	//			if(m_bMini) OnButtonMiniK200();	//2015.09.14 KSJ �̴�
	//			else		OnButtonKospi200();
				if(m_iGubn == NORMAKKOSPI200)		OnButtonKospi200();
				else if(m_iGubn == MINIKOSPI200)	OnButtonMiniK200();
				else if(m_iGubn == WEEKLYOPTION)    OnBtnWeekop();
			}
			break;
		default:
			szTitle = _T("");
			break;
		}

		SetWindowText(szTitle);

		m_ctlButtonName.SetFlat(TRUE);
		m_ctlButtonName.m_bNoFocus = TRUE;
		m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COR_BACK);
		m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_IN, COR_BACK);

		//2015.06.30 KSJ �̴� �ڽ��� 200 �߰�
		//m_ctlButtonName2.SetFlat(TRUE);
		m_ctlButtonName2.m_bNoFocus = TRUE;
		m_ctlButtonName2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		m_ctlButtonName2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COR_BACK);
		m_ctlButtonName2.SetColor(CButtonST::BTNST_COLOR_BK_IN, COR_BACK);

		//weekly �ɼ� �߰�
		m_ctlButtonOPWeek.m_bNoFocus = TRUE;
		m_ctlButtonOPWeek.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		m_ctlButtonOPWeek.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COR_BACK);
		m_ctlButtonOPWeek.SetColor(CButtonST::BTNST_COLOR_BK_IN, COR_BACK);
	


		m_codeList.GetClientRect(&rc);

		m_codeList.SetFont(GetFont());
		m_codeList.SetBkColor(COR_WHITE);
		m_codeList.SetTextBkColor(COR_WHITE);
		m_codeList.SetTextColor(RGB(0,0,0));
		m_codeList.InsertColumn(1, "�����", LVCFMT_CENTER, rc.Width() - 100);
		m_codeList.InsertColumn(2, "�����ڵ�", LVCFMT_CENTER, 80);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFuturesDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int scWidth   = GetSystemMetrics(SM_CXFULLSCREEN);
	const int scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CAxisCodeApp* pApp = (CAxisCodeApp *)AfxGetApp();
	const int virScrX = pApp->GetVirtualScreenX();
	if (virScrX > 0)
		scWidth = virScrX;

	CRect	rc;
	GetClientRect(&rc);

	const int gabX = scWidth - (x + rc.Width());
	const int gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(nullptr, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

void CFuturesDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_arrayFcode.RemoveAll();
	m_arrayOcode.RemoveAll();
	m_arrayPcode.RemoveAll();
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll();
	m_ListPcode.RemoveAll();
}

BOOL CFuturesDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CRect rc;
	CString stmp;

	if(m_pCheck->m_bWeek)
		 rc.SetRect(0, 41, 321, 315);
	else
		rc.SetRect(0, 41, 321, 245);
	
	if(m_Kind != sfCODE)
	{
		if (m_pCheck->IsWindowEnabled())
		{
			if(zDelta > 0)
			{
				// �� ó��...
				if (m_pCheck->GetInit() < 40)
				{
					m_pCheck->SetInit(m_pCheck->GetInit() + 20); 
					m_pCheck->SetScroll(m_pCheck->GetScroll() - 20);
				}
				
				m_pCheck->InvalidateRect(rc);
				m_pCheck->SetScrollPos32(SB_VERT, m_pCheck->GetScroll());
				
			}
			else
			{
				// �Ʒ� ó��....
				stmp.Format("[%d] [%d]\n ", m_pCheck->GetCount(), -20*((m_pCheck->GetCount()-1)-15));
				if(m_pCheck->m_bWeek)
				{
					int iscale{};
					if(m_Kind == optionCODE)
						iscale = -20*((m_pCheck->GetCount()-1)-13);
					else
						iscale = -20*((m_pCheck->GetCount()-1)-14);

					if (m_pCheck->GetInit() > iscale)
					{
						m_pCheck->SetInit(m_pCheck->GetInit() - 20); 
						m_pCheck->SetScroll(m_pCheck->GetScroll() + 20);	
					}
				}
				else
				{
					if (m_pCheck->GetInit() > -20*((m_pCheck->GetCount()-1)-10))
					{
						m_pCheck->SetInit(m_pCheck->GetInit() - 20); 
						m_pCheck->SetScroll(m_pCheck->GetScroll() + 20);	
					}
				}
				
				m_pCheck->InvalidateRect(rc);
				m_pCheck->SetScrollPos32(SB_VERT, m_pCheck->GetScroll());
				
			}
		}
	}
	else
	{
		if (m_pStock->IsWindowEnabled())
		{
			if(zDelta > 0)
			{
				// �� ó��...
				if (m_pStock->GetInit() < 40)
				{
					m_pStock->SetInit(m_pStock->GetInit() + 20); 
					m_pStock->SetScroll(m_pStock->GetScroll() - 20);
				}
				
				//m_pStock->InvalidateRect(rc);
				m_pStock->Invalidate();
				m_pStock->SetScrollPos32(SB_VERT, m_pStock->GetScroll());
				
			}
			else
			{
				// �Ʒ� ó��....
				if (m_pStock->GetInit() > -20*((m_pStock->GetCount()-1)-10))
				{
					m_pStock->SetInit(m_pStock->GetInit() - 20); 
					m_pStock->SetScroll(m_pStock->GetScroll() + 20);
				}
				
				
				//m_pStock->InvalidateRect(rc);
				m_pStock->Invalidate();
				m_pStock->SetScrollPos32(SB_VERT, m_pStock->GetScroll());
				
			}
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CFuturesDlg::HideButton(BOOL flag)
{
	for (int ii = IDC_BUTTON_MONTH3; ii < IDC_BUTTON_MONTH12; ii++)
	{
		if (flag)
			((CButton*) GetDlgItem(ii))->ShowWindow(SW_HIDE);
		else
			((CButton*) GetDlgItem(ii))->ShowWindow(SW_SHOW);
	}
}


void CFuturesDlg::OnButtonKostar() 
{
	m_nStep = sfCODE;

	Invalidate();
	AdjustControls();

	if(m_Kind != sfCODE)
	{
		if(!m_pStock->GetSafeHwnd())
		{
			const CRect rc(110, 20, 447, 267+50);
			
			HideFutureBtns(TRUE);
			
			m_pStock = std::make_unique<CStockCheckBox>();
			
			m_pStock->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_GROUP, rc, this, IDC_WND_CHECK);
			m_pStock->ShowWindow(SW_SHOW);
			m_pStock->SetWnd(this);
			
			m_pStock->SetCheckCursor(IDC_CURSOR_HAND);
			
			loadSfCode();
			
			m_ctlButtonKostar.EnableWindow(TRUE);
			m_ctlButtonKospi.EnableWindow(TRUE);
			m_ctlButtonMiniK200.EnableWindow(TRUE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
			m_ctlButtonJOption.EnableWindow(FALSE);
		}
		else
		{
			HideFutureBtns(TRUE);

			m_pStock->ShowWindow(SW_SHOW);
		}
	}

	m_ctlButtonKospi.SetSelected(FALSE);
	m_ctlButtonMiniK200.SetSelected(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
	m_ctlButtonKostar.SetSelected(TRUE);
	m_btnMoney.SetSelected(FALSE);
	m_btnComm.SetSelected(FALSE);
	m_btnGumri.SetSelected(FALSE);
	
	SFCODEINFO SFInfo;
	SFCODEBASE SFBase;
	DATA data;
	CString szTemp;
	int row = 0;

	m_nCountMonth = 4;

	m_pStock->SetInit(40);
	m_pStock->SetScroll(0);

	m_pStock->RemoveAll();
	m_pStock->SetCount(m_sfbase.GetSize());
	
	int nOffSet = 0;
	if(m_sfbase.GetSize() == 25)	nOffSet = 6;	//2014.08.26 KSJ 25���ۿ� �������� ���������� �ٲ��� �ʾ�����.
	
	// CheckBox�� ������ �¹�����...
	for (int ii = 0; ii < m_sfview.GetSize() ; ii++)
	{
		SFBase = m_sfview.GetAt(ii);
		
		data.code = SFBase.basecodx;
		data.name = SFBase.basenam;

		m_pStock->AddData(row,0,data);
		const bool flag = m_mapsfinfo.Lookup(data.code,SFInfo);

		if(!flag)
		{
			continue;
		}

		// call... ������ 
		int n = 0;

		for (int jj = 1; jj < SF_ROWCOUNT - nOffSet; jj++)
		{
			if(jj < 10)
			{
				CString strMonth = m_ArrayMonth.GetAt(jj-1);
				if (SFInfo.info[n] != "OK")	
				{
					data.flag = FALSE;
					n++;
				}
				else					
				{	
					if(SFInfo.nam[n].Find(strMonth) > -1)
					{
						data.flag = TRUE;
						data.code = SFInfo.codx[n];
						data.name = SFInfo.nam[n];
						n++;
					}
					else
					{
						data.code = "";
						data.name = "";
						data.flag = FALSE;
						n++;
					}
				}
				m_pStock->AddData(row, jj, data);	// ������ ����..				
				data.code = "";
				data.name = "";
				data.flag = FALSE;
			}
			else
			{
				if (SFInfo.info[jj-1] != "OK")		
					data.flag = FALSE;
				else					
				{	
					data.flag = TRUE;
					data.code = SFInfo.codx[jj-1];
					data.name = SFInfo.nam[jj-1];
				}
				m_pStock->AddData(row, jj, data);	// ������ ����..
				
				data.code = "";
				data.name = "";
				data.flag = FALSE;
			}
		}
	
		data.name = "";
		data.code = "";
		data.flag = FALSE;
		data.atm = FALSE;
			
		++row;
	}
	m_pStock->ShowRow(0);	
}

CString CFuturesDlg::GetFName( CString strCode )
{
	if(strCode.Mid(1, 2) != "05")
	{
		//kospi200 ��ǲ 2015.07.02 KSJ
		loadFjCode();
	}
	else
	{
		//mini kospi200 ��ǲ 2015.07.02 KSJ
		loadMfCode();
	}

	for( int i = 0; i < m_arrayFcode.GetSize(); i++ )
	{
		FCODE fcode = m_arrayFcode.GetAt(i);
		if( fcode.code == strCode )
			return fcode.name;
	}

	return "";
		
}

void CFuturesDlg::OnButtonMonth3()		// ���� ���ý�..
{
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH3 ))->GetWindowText( strCode );
		
		GetCommCode( strCode );

		CString strName =GetFName(strCode);	
		
		//AfxMessageBox( strName );
		//AfxMessageBox( strCode );
	
		SetCode(strCode);
		SetName(strName);		
	}
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			FCODE fcode = m_arrayFcode.GetAt(0 + (m_bKostar?7:0));
			
			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

BOOL CFuturesDlg::IsComm( void )
{
	if( m_nStep == BOND || m_nStep == MONEY || m_nStep == COMM )
		return TRUE;
	else
		return FALSE;
}

void CFuturesDlg::OnButtonMonth6()		// ���� ���ý�..
{
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH6 ))->GetWindowText( strCode );

		GetCommCode( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			FCODE fcode = m_arrayFcode.GetAt(1 + (m_bKostar?7:0));

			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonMonth9() 
{
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH9 ))->GetWindowText( strCode );

		GetCommCode( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			FCODE fcode = m_arrayFcode.GetAt(2 + (m_bKostar?7:0));

			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonMonth12()		// ���� ���ý�..
{
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH12 ))->GetWindowText( strCode );

		GetCommCode( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			FCODE fcode = m_arrayFcode.GetAt(3 + (m_bKostar?7:0));
			
			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonSpmonth3() 
{
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH3 ))->GetWindowText( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			// 2014.08.26 KSJ 7���ۿ� �������� �ٸ��� �����ؾ���.
			FCODE fcode = m_arrayFcode.GetAt(m_arrayFcode.GetSize() == 7 ? 4 + (m_bKostar?7:0) - m_nMiniK200 : 7 + (m_bKostar?7:0) - m_nMiniK200); 

			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonSpmonth6() 
{
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH6 ))->GetWindowText( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			// 2014.08.26 KSJ 7���ۿ� �������� �ٸ��� �����ؾ���.
			FCODE fcode = m_arrayFcode.GetAt(m_arrayFcode.GetSize() == 7 ? 5 + (m_bKostar?7:0)  - m_nMiniK200 : 8 + (m_bKostar?7:0)  - m_nMiniK200); 

			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonSpmonth9() 
{
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH9 ))->GetWindowText( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			// 2014.08.26 KSJ 7���ۿ� �������� �ٸ��� �����ؾ���.
			FCODE fcode = m_arrayFcode.GetAt(m_arrayFcode.GetSize() == 7 ? 6 + (m_bKostar?7:0)  - m_nMiniK200 : 9 + (m_bKostar?7:0)  - m_nMiniK200); 

			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnDoubleclickedButtonMonth3() 
{
	OnButtonMonth3();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth6() 
{
	OnButtonMonth6();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth9() 
{
	OnButtonMonth9();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth12() 
{
	OnButtonMonth12();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth32() 
{
	OnButtonMonth32();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth62() 
{
	OnButtonMonth62();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth92() 
{
	OnButtonMonth92();
	//OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSpmonth3() 
{
	OnButtonSpmonth3();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSpmonth6() 
{
	OnButtonSpmonth6();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSpmonth9() 
{
	OnButtonSpmonth9();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSpmonth12() 
{
	OnButtonSpmonth12();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSpmonth32() 
{
	OnButtonSpmonth32();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSpmonth62() 
{
	OnButtonSpmonth62();
	//OnOK();
}

void CFuturesDlg::ButtonText(CString szName, CString szCode, BOOL bName2)
{
	if (m_Kind == optionCODE)
		szCode = _T("");
	else
		szCode = "(" + szCode + ")" + " �����ڻ�";
	
	if(bName2)
	{
		m_ctlButtonName2.ShowWindow(SW_SHOW);
		m_ctlButtonName2.SetWindowText(szName + szCode);
	}
	else
	{
		m_ctlButtonName.ShowWindow(SW_SHOW);
		m_ctlButtonName.SetWindowText(szName + szCode);
	}
}

void CFuturesDlg::OnButtonName()      //�ɼ�����ȭ�鿡�� �ڽ��� ��ư
{
	m_szEditCode = m_szBCode;
	m_szEditName = m_szBName;
	m_nStep = optionCODE;
	OnButtonKospi200();
	UpdateData(FALSE);
}

void CFuturesDlg::OnButtonName2()      //�ɼ�����ȭ�鿡�� �̴� ��ư
{
	m_szEditCode = m_szBCode;
	m_szEditName = m_szBName;
	m_nStep = optionCODE;
	OnButtonMiniK200();
	UpdateData(FALSE);
}
 
void CFuturesDlg::OnBtnWeek()	       //�ɼ�����ȭ�鿡�� ��Ŭ�� ��ư		
{
	// TODO: Add your control notification handler code here
	m_szEditCode = m_szBCode;
	m_szEditName = m_szBName;
	
	OnBtnWeekop();
	UpdateData(FALSE);
}

void CFuturesDlg::OnButtonClose() 
{
	OnOK();
}

BOOL CFuturesDlg::loadPjCode()			//���� �ڵ� ����...
{
	//return TRUE;
	if (m_arrayPcode.GetSize() > 0)
		return FALSE;
	
	int		codeN{};
	CFile		file;
	struct  pjcode  PJCode {};
	PCODE		pcode{};
	CString		path;
	CMapStringToPtr pcodemap;
	CQArray	<CString, CString>	qKindArr;
  	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\pjcode.dat";  //�ֽĿɼ� 

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{
		MessageBox("������ �������� �ʽ��ϴ�.");
		return FALSE;
	}
								// �������� ����...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct pjcode));

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&PJCode, sizeof(struct pjcode));
		
		pcode.kind = GetString(PJCode.tjgb, 2);
		pcode.name = GetString(PJCode.snam, 20);
		pcode.code = GetString(PJCode.codx, PCodeLen);
		pcode.hnam = GetString(PJCode.hnam, sizeof(PJCode.hnam)); pcode.hnam.TrimRight();
		pcode.month = GetString(PJCode.mont, 4);
		pcode.price = GetString(PJCode.hsga, 8);
		pcode.gcod  = GetString(PJCode.gcod, 12);
		pcode.mchk = PJCode.mchk;
		
		if (PJCode.atmx == 1)
			pcode.atm = TRUE;
		else
			pcode.atm = FALSE;

		pcodemap.SetAt(pcode.kind, (void*)m_arrayPcode.Add(pcode));
	}

	POSITION pos = pcodemap.GetStartPosition();
	CString sKind;
	int value{};

	if (m_plistbox.m_hWnd && IsWindow(m_plistbox.m_hWnd))
	{
		m_plistbox.ResetContent();
		while (pos)
		{
			pcodemap.GetNextAssoc(pos, sKind, (void*&)value);
			qKindArr.Add(sKind);
		}

		qKindArr.QuickSort();
		
		for (int ii = 0; ii < qKindArr.GetSize(); ii++)
		{
			sKind = qKindArr.GetAt(ii);
			if (pcodemap.Lookup(sKind, (void*&)value))
			{
				CString str = m_arrayPcode.GetAt(value).name;
				m_plistbox.SetItemData(m_plistbox.AddString(m_arrayPcode.GetAt(value).name), value);
			}
		}
	}

	file.Close();
	return TRUE;
}

BOOL CFuturesDlg::loadCFjCode()
{
	m_arrayFcode.RemoveAll();

	CString path;
  	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\cfcode.dat";  //��ǰ����

	CFile file;
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{
		MessageBox("������ �������� �ʽ��ϴ�.");
		return FALSE;
	}

	const int iCount = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct ccode));
	struct ccode ccode{};
	FCODE fcode{};

	for( int i = 0; i < iCount; i++ )
	{
		file.Read(&ccode, sizeof(struct ccode));

		if( CString(ccode.codx) == "99999999" )
			break; 

		fcode.code = CString(ccode.codx, FCodeLen);
		fcode.name = CString(ccode.hnam, 40);
		fcode.mchk = ccode.mchk;
		
		m_arrayFcode.Add(fcode);		
	}	

	return TRUE;
}

BOOL CFuturesDlg::loadSfCode()
{	
	CFile	file;
	struct  sfcode  SFCode{};
	SFCODE scode{};

	m_mapsfinfo.RemoveAll();
	m_sfbase.RemoveAll();
	m_mapsfcode.RemoveAll();

	CString path, sztmp = _T("");
  	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\sfcode.dat"; //�ֽļ���

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
 		MessageBox("������ �������� �ʽ��ϴ�.");
		return FALSE;
	}

	const int codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct sfcode));
	const int size = sizeof(struct sfcode);

	SFCODEINFO sfinfo;
	SFCODEBASE sfbase;
	CMapStringToString mapDate;
	CString preCodx;
	int nIndex = 0;

	CMapStringToString fms;
	CArray<CString,CString> arrDate;
	CSortStringArray sortArray;
	CMap<CString,LPCSTR,SFCODE,SFCODE> mapsfcode;

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&SFCode, sizeof(struct sfcode));
		
		scode.codx = CString(SFCode.codx, 8);
		scode.hnam = CString(SFCode.hnam, 50);
		scode.enam = CString(SFCode.enam, 50);
		scode.mchk = SFCode.mchk;
		scode.gcod = CString(SFCode.gcod, 12);
		scode.gnam = CString(SFCode.gnam, 32);
		scode.mgub = CString(SFCode.mgub, 1);

		scode.hnam.TrimLeft();
		scode.hnam.TrimRight();

		m_mapsfcode.SetAt(scode.codx,scode);

		scode.gcod.TrimRight();
		scode.gnam.TrimRight();

		sfbase.basecodx = scode.gcod;
		sfbase.basenam = scode.gnam;

		CString date;
		CString year = scode.codx.Mid(3,1);
		CString month = scode.codx.Mid(4,1);

		if(year == "N")
		{
			year = "18";
		}
		else if(year == "P")
		{
			year = "19";
		}
		else if(year == "Q")
		{
			year = "20";
		}
		else if(year == "R")
		{
			year = "21";
		}
		else if(year == "S")
		{
			year = "22";
		}
		else if(year == "T")
		{
			year = "23";
		}
		else if(year == "V")
		{
			year = "24";
		}
		else if(year == "W")
		{
			year = "25";
		}
		else if(year == "6")
		{
			year = "26";
		}
		else if(year == "7")
		{
			year = "27";
		}
		else if(year == "8")
		{
			year = "28";
		}
		else if(year == "9")
		{
			year = "29";
		}
		else if(year == "0")
		{
			year = "30";
		}
		else if(year == "1")
		{
			year = "31";
		}
		else if(year == "2")
		{
			year = "32";
		}
		else if(year == "3")
		{
			year = "33";
		}
		else if(year == "4")
		{
			year = "34";
		}
		else if(year == "5")
		{
			year = "35";
		}
		else if(year == "A")
		{
			year = "36";
		}

		if( month == "A" )
			month = "10";
		else if( month == "B" ) 
			month = "11";
		else if( month == "C" )
			month = "12";

		month.Format("%02d",atoi(month));

		date = year + month;

		CString tmp;

		if(!fms.Lookup(date,tmp))
		{	
			//2016.12.06 KSJ �ŷ������� �͵��� �����ڵ��� ���� �״�� �����ִ�. �׷��Ƿ� ��Ƽ�����ش�.
			if(scode.gnam.GetAt(0) == 'X')	continue;
			fms.SetAt(date,date);
			arrDate.Add(date);
		}

		if(ii == 0)
		{
			preCodx = scode.gcod;
			nIndex = 0;
			m_sfbase.Add(sfbase);
			sortArray.Add(scode.gnam);
			m_sfCode = scode;	//2016.03.11 KSJ ó���ڵ� ���� getCODEsfuture ���� ���
		}

// 		if(preCodx != scode.gcod)
// 		{
// 			for(int a=nIndex;a<SF_ROWCOUNT -1;a++)
// 			{
// 				sfinfo.codx[a] = "";
// 				sfinfo.nam[a] = "";
// 				sfinfo.info[a] = "NOK";
// 			}
// 
// 			m_mapsfinfo.SetAt(preCodx,sfinfo);
// 
// 			m_sfbase.Add(sfbase);
// 			sortArray.Add(scode.gnam);
// 			nIndex = 0;	
// 		}

		if(preCodx != scode.gcod)
		{
			nIndex = 0;	
			
			for(int a=nIndex;a<SF_ROWCOUNT -1;a++)
			{
				sfinfo.codx[a] = "";
				sfinfo.nam[a] = "";
				sfinfo.info[a] = "NOK";
			}
			
			//m_mapsfinfo.SetAt(preCodx,sfinfo);
			
			m_sfbase.Add(sfbase);
			sortArray.Add(scode.gnam);
		}

		if(scode.codx.GetAt(0) == '4' && nIndex < 9)
		{
			nIndex = 9;
		}

		sfinfo.codx[nIndex] = scode.codx;
		sfinfo.nam[nIndex] = scode.hnam;
		sfinfo.nam[nIndex].TrimLeft();
		sfinfo.info[nIndex] = "OK";
		nIndex++;

		preCodx = scode.gcod;

		m_mapsfinfo.SetAt(scode.gcod,sfinfo);
	}

	sortArray.Sort();

	for (int i = 0; i <= sortArray.GetUpperBound(); i++)
	{
		for(int ii = 0; ii < m_sfbase.GetSize(); ii++)
		{
			SFCODEBASE sfcode = m_sfbase.GetAt(ii);
			
			if(sortArray[i] == sfcode.basenam)
			{
				m_sfview.Add(sfcode);
			}
		}
	}

	for (int jj = 0; jj < arrDate.GetSize(); jj++)
	{
		CString str = arrDate.GetAt(jj);
		m_ArrayMonth.Add(str);
	}

	if (m_pStock->GetSafeHwnd())
		m_pStock->SetArray();

	file.Close();	
	
	return TRUE;

}

BOOL CFuturesDlg::loadFjCode()
{
	//if (m_arrayFcode.GetSize() > 0)
	//	return FALSE;

	m_arrayFcode.RemoveAll();
	
	int	codeN{};
	CFile	file;
	struct  fjcode  FJCode{};
	FCODE fcode{};
	CString path;
  	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\fjcode.dat";   //������������
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{
		MessageBox("������ �������� �ʽ��ϴ�.");
		return FALSE;
	}
								// �������� ����...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct fjcode));

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&FJCode, sizeof(struct fjcode));
		
		fcode.code = CString(FJCode.cod2, FCodeLen);
		fcode.name = CString(FJCode.hnam, FNameLen);
		fcode.mchk = FJCode.mchk;
		
		m_arrayFcode.Add(fcode);
	}

	file.Close();
	return TRUE;
}

BOOL CFuturesDlg::loadWCode()
{
//writelogs("AXISCODE loadWCode start");
	CString stmp, stmp1, stmp2;  //sWfname, sWlname
	m_arrayOcode.RemoveAll();
	int	codeN{}, len{};
	
	CFile	file;
	struct  ojcodh  OJCodh{};
	struct  ojcode  OJCode{};

	CString path;
	
//	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\opcode.dat";  //�ɼ��ڵ�
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\wocode.dat";  //�ɼ��ڵ�
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		MessageBox("������ �������� �ʽ��ϴ�.");
		return FALSE;
	}
	len = file.Read(&OJCodh, sizeof(struct ojcodh));

	for (int jj = 0; jj < STANDARDNUM; jj++)  
//	for (int jj = 0; jj < STANDARDNUM - 2; jj++)
	{
		CString str = CString(OJCodh.cjym[jj], 6).Right(4);
		stmp.Format("[%d] %s\n",jj,  str);
		m_ArrayMonth.Add(str);
	}

/*	 //��Ŭ�� �̸� 
	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();
	
	m_ArrayMonth.RemoveAll();
*/	

	codeN = gsl::narrow_cast<int>((file.GetLength() - len) / sizeof(struct ojcode));  //weekly 43   //���� 83
	
	
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&OJCode, sizeof(struct ojcode));

		stmp.Format("[%d] [%.5s] \r\n",ii, OJCode.price);
	//	OutputDebugString(stmp);
		for(int ii = 0 ; ii < 11 ; ii++)
		{
			stmp1.Format("----<%d>", ii);
		//	OutputDebugString(stmp1 + "\r\n");

			stmp1.Format("%s [%s] [%d]", OJCode.call[ii].cod2, OJCode.call[ii].hnam, OJCode.call[ii].yorn);
			stmp1.TrimRight();
		//	OutputDebugString(stmp1 + "\r\n");
	
			stmp2.Format("%s [%s] [%d]", OJCode.put[ii].cod2, OJCode.put[ii].hnam, OJCode.put[ii].yorn);
			stmp2.TrimRight();
		//	OutputDebugString(stmp2 + "\r\n");

		//	stmp2 = "-------------------------------------------------";
		//	OutputDebugString(stmp2 + "\r\n");
		//	if(stmp2.GetLength() == 0)
		//	stmp2 = "        ";

			/*if(ii == 9)   
			{
				stmp.Format("%s", OJCode.call[ii].hnam);
				stmp.TrimRight();
				if(stmp.GetLength() > 0)
					sWlname = stmp.Mid(4,4);
			}
			if(ii == 10)  
			{
				stmp.Format("%s", OJCode.call[ii].hnam);
				stmp.TrimRight();
				if(stmp.GetLength() > 0)
					sWfname = stmp.Mid(4,4);
			}*/
		}
		m_arrayOcode.Add(OJCode);
	}
	
	//m_ArrayMonth.Add(sWlname);
	//m_ArrayMonth.Add(sWfname);

	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();
	
	m_ArrayMonth.RemoveAll();
	
	file.Close();
//writelogs("AXISCODE loadWCode end");
	return TRUE;
}

BOOL CFuturesDlg::loadOjCode()
{									// �ɼ� �ڵ� ����...	
// 	if (m_arrayOcode.GetSize() > 0)
// 		return FALSE;
	
	//2015.07.02 KSJ ��ư���� �ű�� ������ �ѹ��� �����ش�.
	m_arrayOcode.RemoveAll();
	m_ArrayMonth.RemoveAll();

	int	codeN{}, len{};

	CFile	file;
	struct  ojcodh  OJCodh{};
	

	CString path;
  	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\opcode.dat";  //�ɼ��ڵ�

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		MessageBox("������ �������� �ʽ��ϴ�.");
		return FALSE;
	}
	len = file.Read(&OJCodh, sizeof(struct ojcodh));

	for (int jj = 0; jj < STANDARDNUM; jj++)
	{
		CString str = CString(OJCodh.cjym[jj], 6).Right(4);
		m_ArrayMonth.Add(str);
	}
	
	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();

	m_ArrayMonth.RemoveAll();

	codeN = gsl::narrow_cast<int>((file.GetLength() - len) / sizeof(struct ojcode));

	for (int ii = 0; ii < codeN; ii++)
	{
		struct  ojcode OJCode;
		file.Read(&OJCode, sizeof(struct ojcode));
		m_arrayOcode.Add(OJCode);
	}

	file.Close();
	return TRUE;
}

BOOL CFuturesDlg::loadMfCode()
{
	m_arrayFcode.RemoveAll();

	int	codeN{};
	
	CFile	file;
	struct  fjcode  FJCode{};
	FCODE fcode{};
	CString path;
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\mfcode.dat";  //�̴ϼ���

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{
		SYSTEMTIME stime;
		GetLocalTime(&stime);
		
		CString strCurTime;
		strCurTime.Format(_T("%04d%02d%02d"), stime.wYear, stime.wMonth, stime.wDay);
		
		if(strCurTime.Compare("20150720") > 0)
			MessageBox("������ �������� �ʽ��ϴ�.");

		return FALSE;
	}

	// �������� ����...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct fjcode));
	
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&FJCode, sizeof(struct fjcode));
		
		fcode.code = CString(FJCode.cod2, FCodeLen);
		fcode.name = CString(FJCode.hnam, FNameLen);
		fcode.mchk = FJCode.mchk;
		
		m_arrayFcode.Add(fcode);
	}
	
	file.Close();
	return TRUE;
}

BOOL CFuturesDlg::loadMoCode()
{									// �ɼ� �ڵ� ����...	
// 	if (m_arrayOcode.GetSize() > 0 && m)
// 		return FALSE;

	CString stmp, stmp1, stmp2;
	//2015.07.02 KSJ ��ư���� �ű�� ������ �ѹ��� �����ش�.
	m_arrayOcode.RemoveAll();
	m_ArrayMonth.RemoveAll();
	
	int	codeN{}, len{};
	
	CFile	file;
	struct  ojcodh  OJCodh{};
	struct  ojcode  OJCode{};
	
	CString path;
	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\mocode.dat";  //���� ��Ÿ
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		SYSTEMTIME stime;
		GetLocalTime(&stime);
		
		CString strCurTime;
		strCurTime.Format(_T("%04d%02d%02d"), stime.wYear, stime.wMonth, stime.wDay);
		
// 		MessageBox("��¥[" + strCurTime + "]");

		if(strCurTime.Compare("20150720") > 0)
			MessageBox("������ �������� �ʽ��ϴ�.[" + strCurTime + "]");

		return FALSE;
	}
	len = file.Read(&OJCodh, sizeof(struct ojcodh));
	
	for (int jj = 0; jj < STANDARDNUM; jj++)
	{
		CString str = CString(OJCodh.cjym[jj], 6).Right(4);
		m_ArrayMonth.Add(str);
	}
	
	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();
	
	m_ArrayMonth.RemoveAll();
	
	codeN = gsl::narrow_cast<int>((file.GetLength() - len) / sizeof(struct ojcode));
	
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&OJCode, sizeof(struct ojcode));
		stmp.Format("[%d] [%.5s] \n",ii, OJCode.price);
		OutputDebugString(stmp);
		for(int ii = 0 ; ii < 11 ; ii++)
		{
			stmp1.Format("%s [%c]", OJCode.call[ii].cod2, OJCode.call[ii].yorn);
			stmp1.TrimRight();
			//	if(stmp1.GetLength() == 0)
			//		stmp1 = "        ";
			
			stmp2.Format("%s [%c]", OJCode.put[ii].cod2, OJCode.put[ii].yorn);
			stmp2.TrimRight();
			//	if(stmp2.GetLength() == 0)
			//	stmp2 = "        ";
			
			stmp.Format("%s   %s \n", stmp1,  stmp2);
			OutputDebugString(stmp);
		}
		m_arrayOcode.Add(OJCode);
	}
	
	file.Close();
	return TRUE;
}

void CFuturesDlg::Standard(CString szCode)		// �ݿɼ� ǲ�ɼ�..
{
	PCODE pcode;

	loadPjCode();

	for (int ii = 0; ii < m_arrayPcode.GetSize(); ii++)
	{
		pcode = m_arrayPcode.GetAt(ii);
	
		if (!pcode.kind.Compare(szCode))
		{
			if (pcode.code.Left(1).Compare("2") == 0)	// ��
				m_arraycall.Add(pcode);
			if (pcode.code.Left(1).Compare("3") == 0)	// ǲ
				m_arrayput.Add(pcode);
		}
	}

	// ���Ŀ�...
	SearchPrice();
	SearchMonth();
}

void CFuturesDlg::SearchPrice()
{
	PCode pcode, temp;
	POSITION pos{};

	for (int ii= 0; ii < m_arraycall.GetSize(); ii++)
	{
		pcode = m_arraycall.GetAt(ii);

		if (m_ListPcode.IsEmpty())
			m_ListPcode.AddHead(pcode);
		
		if (!FindPrice(pcode.price))
			AddList(pcode);			
			
	}

	pos = m_ListPcode.GetHeadPosition();
	
	for (int ii = 0; ii < m_ListPcode.GetCount(); ii++)
	{
		pcode = m_ListPcode.GetAt(pos);
		m_ArrayPrice.Add(pcode.price);
		m_ListPcode.GetNext(pos);
	}
	
	m_ListPcode.RemoveAll();

}

BOOL CFuturesDlg::FindPrice(CString szCompare)
{
	if(m_ListPcode.IsEmpty())
		return FALSE;

	PCode pcode;
	POSITION pos = m_ListPcode.GetHeadPosition();

	for (int ii = 0; ii < m_ListPcode.GetCount(); ii++)
	{
		pcode = m_ListPcode.GetAt(pos);
		if (pcode.price == szCompare)
			return TRUE;
		m_ListPcode.GetNext(pos);
	}

	return FALSE;
}

void CFuturesDlg::AddList(const PCODE &pcode)
{
	POSITION pos = m_ListPcode.GetHeadPosition();
	PCODE temp;

	for (int ii = 0; ii < m_ListPcode.GetCount(); ii++)
	{
		temp = m_ListPcode.GetAt(pos);
		
		if (pcode.price.Compare(temp.price) > 0)
		{
			m_ListPcode.AddHead(pcode);
			return;
		}
		
		m_ListPcode.GetNext(pos);
	}
}

void CFuturesDlg::SearchMonth()
{
	PCode pcode{}, temp{};
	CString szTemp;
	
	POSITION pos{};

	for (int ii= 0; ii < m_arraycall.GetSize(); ii++)
	{
		pcode = m_arraycall.GetAt(ii);

		if (m_ListMonth.IsEmpty())
			m_ListMonth.AddHead(pcode.month);
		
		if (!FindMonth(pcode.month))
			m_ListMonth.AddHead(pcode.month);	
	}

	pos = m_ListMonth.GetHeadPosition();
	
	for (int ii = 0; ii < m_ListMonth.GetCount(); ii++)
	{
		szTemp = m_ListMonth.GetAt(pos);
		m_ArrayMonth.Add(szTemp);
		m_ListMonth.GetNext(pos);
		if (ii < 4)
			GetDlgItem(IDC_BUTTON_OPMONTH4 - ii)->SetWindowText(szTemp.Right(2) + "��");
	}
	
	m_nCountMonth = m_ArrayMonth.GetSize();

	m_ListMonth.RemoveAll();

}

BOOL CFuturesDlg::FindMonth(CString szMon)
{
	if(m_ListMonth.IsEmpty())
		return FALSE;

	CString sztemp;
	POSITION pos = m_ListMonth.GetHeadPosition();

	for (int ii = 0; ii < m_ListMonth.GetCount(); ii++)
	{
		sztemp = m_ListMonth.GetAt(pos);
		if (sztemp == szMon)
			return TRUE;
		m_ListMonth.GetNext(pos);
	}

	return FALSE;
}

DATA CFuturesDlg::FindCall(CString szMonth, CString szPrice)
{
	DATA data;
	PCODE code;
	
	for (int ii = 0; ii < m_arraycall.GetSize(); ii++)
	{
		code = m_arraycall.GetAt(ii);
		if (code.month == szMonth && code.price == szPrice)
		{
			data.flag = TRUE;
			data.code = code.code;
//			code.name.TrimRight();
//	             	data.name.Format("%s %s %s",code.name, code.hnam, code.month);
			data.name = code.hnam;
			data.atm = code.atm;

			return data;
		}
		else
		{
			data.flag = FALSE;
			data.code = "";
			data.name = "";
		}

	}

	return data;

}

DATA CFuturesDlg::FindPut(CString szMonth, CString szPrice)
{
	DATA data;
	PCODE code;
	
	for (int ii = 0; ii < m_arrayput.GetSize(); ii++)
	{
		code = m_arrayput.GetAt(ii);
		if (code.month == szMonth && code.price == szPrice)
		{
			data.flag = TRUE;
			data.code = code.code;
//			code.name.TrimRight();
//			data.name.Format("%s %s %s",code.name, code.hnam, code.month);
			data.name = code.hnam;
		
			return data;
		}
		else
		{
			data.flag = FALSE;
			data.code = "";
			data.name = "";
		}
	}

	return data;
}

void CFuturesDlg::FindEnd()
{
	m_pCheck->SetArray();
	m_ArrayMonth.RemoveAll();
	m_ArrayPrice.RemoveAll();
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll(); 
}

void CFuturesDlg::CountData(CString code)		// ������ ó��..
{
	Standard(code);

	DATA data;
	const __POSITION* pos = m_ListMonth.GetHeadPosition();
	CString szMonth, szPrice;
	
	m_pCheck->RemoveAll();
	m_pCheck->SetCount(m_ArrayPrice.GetSize());
	
	// �� ... call ��...
	for (int ii = 0; ii < STANDARDNUM; ii++)
	{
		szMonth = "";
		for (int jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // ��...
		{
			if (m_ArrayMonth.GetSize() > ii)
				szMonth = m_ArrayMonth.GetAt(ii);
			szPrice = m_ArrayPrice.GetAt(jj);
			
			data = FindCall(szMonth, szPrice);
			m_pCheck->AddData(jj, ii, data);
			
			data.code = "";
			data.flag = FALSE;
			szPrice.TrimLeft();
			//				data.name = szPrice;
			data.name = CommaModify(szPrice);
			m_pCheck->AddData(jj, STANDARDNUM, data);
			
			if (data.atm == 1)		// atm..
				m_nAtm = jj;
		}
	}
	
	// �� ... put ��...
	for (int kk = 0; kk < STANDARDNUM; kk++)
	{
		szMonth = "";
		for (int jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // ��...
		{
			if (m_ArrayMonth.GetSize() > kk)
				szMonth = m_ArrayMonth.GetAt(kk);
			szPrice = m_ArrayPrice.GetAt(jj);
			data = FindPut(szMonth, szPrice);
			m_pCheck->AddData(jj, STANDARDNUM + kk + 1, data);	
		}
	}
	
	m_pCheck->Invalidate();
	FindEnd();
}

CString CFuturesDlg::CommaModify(CString string)
{
	CString stripData = string;
	stripData.TrimLeft(); stripData.TrimRight();

	if (stripData.GetLength() <= 3)
		return stripData;

	char dChar = 0x00;
	switch (stripData[0])
	{
	case '+':
		dChar = '+'; stripData.Delete(0);
		break;
	case '-':
		dChar = '-'; stripData.Delete(0);
		break;
	}

	stripData.MakeReverse();
	string = _T("");
	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			string += ',';

		string += stripData[ii];
	}

	string.MakeReverse();
	if (dChar != 0x00) string.Insert(0, dChar);
	return string;
}

void CFuturesDlg::DisableButton(long lExcept)
{
	((CButton*)GetDlgItem(IDC_BUTTON_JOPTION))->EnableWindow(FALSE);
	m_plistbox.EnableWindow(FALSE);
}

void CFuturesDlg::SetButton()
{
	SetButton(&m_ctlButtonMonth12);
	SetButton(&m_btnMonth122);

	SetButton(&m_ctlButtonMonth3);
	SetButton(&m_btnMonth32);

	SetButton(&m_ctlButtonMonth6);
	SetButton(&m_btnMonth62);

	SetButton(&m_ctlButtonMonth9);
	SetButton(&m_btnMonth92);

	SetButton(&m_ctlButtonSPMonth3);
	SetButton(&m_btnSPMonth32);

	SetButton(&m_ctlButtonSPMonth6);
	SetButton(&m_btnSPMonth62);

	SetButton(&m_ctlButtonSPMonth9);
	SetButton(&m_btnSPMonth92);

	SetButton(&m_btnSPMonth12);
	SetButton(&m_btnSPMonth122);

	SetButton(&m_ctlButtonJOption);

	SetButton(&m_ctlButtonKostar);
	SetButton(&m_ctlButtonKospi);
	SetButton(&m_ctlButtonMiniK200);	//2015.06.30 KSJ �̴� �ڽ���200 �߰�

	SetButton(&m_btnGumri);
	SetButton(&m_btnMoney);
	SetButton(&m_btnComm);

	SetButton(&m_ctlButtonOPMonth1);
	SetButton(&m_ctlButtonOPMonth2);
	SetButton(&m_ctlButtonOPMonth3);
	SetButton(&m_ctlButtonOPMonth4);

	SetButton(&m_ctlButtonUSD);
	SetButton(&m_ctlButtonUSD1);
	SetButton(&m_ctlButtonSPUSD);
	SetButton(&m_ctlButtonSPUSD1);

	SetButton(&m_ctlButtonWEEKLYOP);
}

void CFuturesDlg::SetButton(CButtonST *pButton)
{
	const short	shBtnColor = 245;
	pButton->SetFlat(FALSE);
	pButton->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
	pButton->SetBtnCursor(IDC_CURSOR_HAND);
	pButton->SetBold(FALSE);
}

void CFuturesDlg::InitBtn()
{ 
	m_ctlButtonKospi.SetBold(FALSE);
	m_ctlButtonMiniK200.SetBold(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
	m_ctlButtonKostar.SetBold(FALSE);
	m_ctlButtonJOption.SetBold(FALSE);
}

void CFuturesDlg::MonthInit()
{
	m_ctlButtonMonth12.SetBold(FALSE);
	m_ctlButtonMonth9.SetBold(FALSE);
	m_ctlButtonMonth6.SetBold(FALSE);
	m_ctlButtonMonth3.SetBold(FALSE);
}

void CFuturesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	return;

	CRect rc;//(305, 17, 90, 35);
	m_ctlButtonSPMonth9.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.OffsetRect(rc.Width() + 6, 0);
	rc.DeflateRect(0, 2, 0, 3);
	CString szTitle;

	m_Tool.SetDC(&dc);
	m_Tool.FrameRect(rc, RGB(255,255,255), RGB(0,0,0), TRUE);

	switch (m_nStep)
	{
	case foptionCODE:
		szTitle = "���� �ɼ�";
		break;
	case futureCODE:
		if (m_bKostar)
			szTitle = "KOSTAR";
		else
			szTitle = "KOSPI200";
		break;
	case optionCODE:
		szTitle = "�� ��";
		break;  
	case koptionCODE:
		szTitle = "�ֽĿɼ�";
		break;
	case faoptionCODE:
		szTitle = "���� �ɼ�";
		break;
	case spreadCODE:
		szTitle = "��������";
		break;
	}

	m_Tool.WriteText(szTitle, rc);
	
	CPen	pen(PS_SOLID, 1, RGB(128,128,128)), *oldpen;

	oldpen = dc.SelectObject(&pen);

	dc.MoveTo(17, 68);
	dc.LineTo(88, 68);
	/*dc.MoveTo(17, 146);
	dc.LineTo(88, 146);
	dc.MoveTo(17, 248);
	dc.LineTo(88, 248);*/
////////
	
	dc.SelectObject(oldpen);
	pen.DeleteObject();
	
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CFuturesDlg::FindCode(int kind, CString szCode)		// GetName()���� ���. 
{
	FCODE	fcode;
	ojcode	OJcode{};
	PCODE	pcode;

	szCode.TrimRight();

	switch(kind)
	{
	case whichTYPE:
		{
			m_jongmuk = 0;

			if(szCode.Mid(1, 2) == "09")
			{
				loadFjCode();
				loadWCode();
			}
			else if(szCode.Mid(1, 2) != "05")
			{
				//kospi200 ��ǲ 2015.07.02 KSJ
				loadFjCode();
				loadOjCode();
			}
			else
			{
				//mini kospi200 ��ǲ 2015.07.02 KSj
				loadMfCode();
				loadMoCode();
			}

			if (m_arrayFcode.GetSize() > 0)
			{
				for(int ii = 0; ii < m_arrayFcode.GetSize(); ii++)
				{
					fcode = m_arrayFcode[ii];
					if (szCode.CompareNoCase(fcode.code) == 0) // fcode.code == szCode)
					{
						fcode.name.TrimRight();
						SetName(fcode.name);
						m_arrayFcode.RemoveAll();

						m_jongmuk = futureType;
						return TRUE;
					}
				}
			}

			if (m_arrayOcode.GetSize() > 0)
			{
				for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
				{
					OJcode = m_arrayOcode.GetAt(ii);
					
					for (int jj = 0; jj < STANDARDNUM ; jj++)
					{
						if (szCode.CompareNoCase(OJcode.call[jj].cod2) == 0)  // == szCode)
						{
							SetName(OJcode.call[jj].hnam);
							m_arrayOcode.RemoveAll();
							m_jongmuk = callType;
							return TRUE;
						}

						if (szCode.CollateNoCase(OJcode.put[jj].cod2) == 0) // == szCode)
						{
							SetName(OJcode.put[jj].hnam);
							m_arrayOcode.RemoveAll();
							m_jongmuk = putType;
							return TRUE;
						}
					}
				}
			}

			if (m_arrayPcode.GetSize() > 0)
			{

				for (int ii = 0; ii < m_arrayPcode.GetSize(); ii++)
				{
					pcode = m_arrayPcode.GetAt(ii);

					if (szCode.CompareNoCase(pcode.code) == 0) // == szCode)
					{
						pcode.name.TrimRight();
						CString temp =  pcode.hnam;
						SetName(temp);
						m_arrayPcode.RemoveAll();
						m_jongmuk = koptionType;
						return TRUE;
					}
				}
			}
		}
		break;
	case futureNAME:
		{
			if(szCode.Mid(1, 2) != "05")
			{
				//kospi200 ��ǲ 2015.07.02 KSJ
				loadFjCode();
			}
			else
			{
				//mini kospi200 ��ǲ 2015.07.02 KSJ
				loadMfCode();
			}

			if (m_arrayFcode.GetSize() < 0)
				return FALSE;


			for(int ii = 0; ii < m_arrayFcode.GetSize(); ii++)
			{
				fcode = m_arrayFcode[ii];
				if (szCode.CompareNoCase(fcode.code) == 0) // fcode.code == szCode)
				{
					fcode.name.TrimRight();
					SetName(fcode.name);
					m_arrayFcode.RemoveAll();
					return TRUE;
				}
			}
		}
		break;
	case sfName:
		{
			SFCODE sfcode;

			if(!m_mapsfcode.Lookup(szCode,sfcode))
			{
				return FALSE;
			}

			sfcode.hnam.TrimRight();

			SetName(sfcode.hnam);

			return TRUE;
		}
		break;
/*
	case WoptionNAME:
		{
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (szCode.CompareNoCase(OJcode.call[jj].cod2) == 0)  // == szCode)
					{
						SetName(OJcode.call[jj].hnam);
						m_arrayOcode.RemoveAll();
						return TRUE;
					}
					
					if (szCode.CollateNoCase(OJcode.put[jj].cod2) == 0) // == szCode)
					{
						SetName(OJcode.put[jj].hnam);
						m_arrayOcode.RemoveAll();
						return TRUE;
					}
				}
			}
		}
		break;
		*/
	case optionNAME:
		{
			if(szCode.Mid(1, 2) == "09")
			{
				loadWCode();
			}
			else if(szCode.Mid(1, 2) != "05")
			{
				//kospi200 ��ǲ 2015.07.02 KSJ
				loadOjCode();
			}
			else
			{
				//mini kospi200 ��ǲ 2015.07.02 KSJ
				loadMoCode();
			}
			
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (szCode.CompareNoCase(OJcode.call[jj].cod2) == 0)  // == szCode)
					{
						SetName(OJcode.call[jj].hnam);
						m_arrayOcode.RemoveAll();
						return TRUE;
					}
					
					if (szCode.CollateNoCase(OJcode.put[jj].cod2) == 0) // == szCode)
					{
						SetName(OJcode.put[jj].hnam);
						m_arrayOcode.RemoveAll();
						return TRUE;
					}
				}
			}
		}
		break;
	case koptionNAME:
		{
			if (m_arrayPcode.GetSize() <=  0)
				return FALSE;

			for (int ii = 0; ii < m_arrayPcode.GetSize(); ii++)
			{
				pcode = m_arrayPcode.GetAt(ii);

				if (szCode.CompareNoCase(pcode.code) == 0) // == szCode)
				{
					pcode.name.TrimRight();
//					CString temp =  pcode.name+ " " + pcode.hnam + " " + pcode.month;
					CString temp =  pcode.hnam;
					SetName(temp);
					m_arrayPcode.RemoveAll();
					return TRUE;
				}
			}
		}
		break;
	case getCODEfuture:
		{
			if(szCode.Mid(1, 2) != "05")
			{
				//kospi200 ��ǲ 2015.07.02 KSJ
				loadFjCode();
			}
			else
			{
				//mini kospi200 ��ǲ 2015.07.02 KSJ
				loadMfCode();
			}
			
			if (m_arrayFcode.GetSize() < 0)
				return FALSE;
			
			fcode = m_arrayFcode[0];
			
			//	SetName(fcode.name);
			SetCode(fcode.code);
			
			m_arrayFcode.RemoveAll();
			return TRUE;
		}
		break;
	case getCODEcall:
		{
			if(szCode.Mid(1, 2) == "09")
			{
				loadWCode();
			}
			else if(szCode.Mid(1, 2) != "05")
			{
				//kospi200 ��ǲ 2015.07.02 KSJ
				loadOjCode();
			}
			else
			{
				//mini kospi200 ��ǲ 2015.07.02 KSJ
				loadMoCode();
			}
			
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				if (OJcode.atmg == 1)
				{
					//	SetName(OJcode.call[3].hnam);
					SetCode(OJcode.call[10].cod2);	//2014.08.11 KSJ �ֱٿ����� 10��°��
					m_arrayOcode.RemoveAll();
					return TRUE;
				}
				
			}
		}
		break;
	case getCODEput:
		{
			if(szCode.Mid(1, 2) == "09")
			{
				loadWCode();
			}
			else if(szCode.Mid(1, 2) != "05")
			{
				//kospi200 ��ǲ	2015.07.02 KSJ
				loadOjCode();
			}
			else
			{
				//mini kospi200 ��ǲ 2015.07.02 KSJ
				loadMoCode();
			}
			
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				if (OJcode.atmg == 1)
				{	
					//	SetName(OJcode.put[0].hnam);
					SetCode(OJcode.put[0].cod2);
					m_arrayOcode.RemoveAll();
					return TRUE;
				}
			}
		}
		break; 
		//2015.07.10 KSJ Mini Kospi �߰�
	case getCODEminifuture:
		{
			//mini kospi200 ��ǲ 2015.07.02 KSJ
			loadMfCode();
			
			if (m_arrayFcode.GetSize() < 0)
				return FALSE;
			
			fcode = m_arrayFcode[0];
			
			//	SetName(fcode.name);
			SetCode(fcode.code);
			
			m_arrayFcode.RemoveAll();
			return TRUE;
		}
		break;
	case getCODEminicall:
		{
			//mini kospi200 ��ǲ 2015.07.02 KSJ
			loadMoCode();
			
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				if (OJcode.atmg == 1)
				{
					//	SetName(OJcode.call[3].hnam);
					SetCode(OJcode.call[10].cod2);	//2014.08.11 KSJ �ֱٿ����� 10��°��
					m_arrayOcode.RemoveAll();
					return TRUE;
				}
				
			}
		}
		break;
	case getCODEminiput:
		{
			//mini kospi200 ��ǲ 2015.07.02 KSJ
			loadMoCode();
		
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				if (OJcode.atmg == 1)
				{	
					//	SetName(OJcode.put[0].hnam);
					SetCode(OJcode.put[0].cod2);
					m_arrayOcode.RemoveAll();
					return TRUE;
				}
			}
		}
		break; 

		//2016.03.11 KSJ �ֽļ��� ù��° �ڵ� ��������
	case getCODEsfuture:
		{
			if(m_sfCode.codx.IsEmpty())	loadSfCode();

			SetCode(m_sfCode.codx);
		
			return TRUE;
		}
		break;

		//2016.03.18 KSJ �ֽļ��� ���ϱ��� ��������
	case getMarketsfuture:
		{
			SFCODE sfcode;
			
			if(!m_mapsfcode.Lookup(szCode,sfcode))
			{
				return FALSE;
			}

			SetMgubn(sfcode.mgub);
			
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

void CFuturesDlg::DisableFuture()
{
	for (int ii = IDC_BUTTON_MONTH3 ; ii <= IDC_BUTTON_SPMONTH9 ; ii++)
		((CButton*)GetDlgItem(ii))->ShowWindow(SW_HIDE);

	m_ctlButtonKospi.EnableWindow(FALSE);
	m_ctlButtonMiniK200.EnableWindow(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
	m_plistbox.EnableWindow(FALSE);
	m_ctlButtonKostar.EnableWindow(FALSE);
	m_ctlButtonWEEKLYOP.EnableWindow(FALSE); 
}

bool CFuturesDlg::GetCodeOrderable(CString code, int* type)
{
	bool	rValue = false;

	loadFjCode();
	loadPjCode();
	loadOjCode();

	if (code.IsEmpty())
		return false;

	if(code.Mid(1, 2) != "05")
	{
		//kospi200 ��ǲ	2015.07.02 KSJ
		loadOjCode();
		loadFjCode();
	}
	else
	{
		//kospi200 ��ǲ	2015.07.02 KSJ
		loadMoCode();
		loadMfCode();
	}


	CString comp;
	CString kind = code.Left(2);
	switch (atoi(kind))
	{
	case 10: // ����
	case 40: // ������������
		{
			FCODE	fcode;
			for (int ii = 0 ; ii < m_arrayFcode.GetSize() ; ii++)
			{
				fcode = m_arrayFcode.GetAt(ii);
				if (fcode.code.Compare(code) == 0)
				{
					*type = fcode.mchk;
					rValue = true;
					break;
				}
			}
		}
		break;
	case 20: // �ݿɼ�
		{
			ojcode	ojcode{};
			for (int ii = 0 ; ii < m_arrayOcode.GetSize() ; ii++)
			{
				ojcode = m_arrayOcode.GetAt(ii);
				for (int jj = 0 ; jj < STANDARDNUM ; jj++)
				{
					if (ojcode.call[jj].yorn == '1')
					{
						comp = CString(ojcode.call[jj].cod2, OCodeLen);
						if (comp.Compare(code) == 0)
						{
							*type = ojcode.call[jj].mchk;
							rValue = true;
							break;
						}
					}
				}

				if (rValue)
					break;
			}
		}
		break;
	case 30: // ǲ�ɼ�
		{ 
			ojcode	ojcode{};
			for (int ii = 0 ; ii < m_arrayOcode.GetSize() ; ii++)
			{
				ojcode = m_arrayOcode.GetAt(ii);
				for (int jj = 0 ; jj < STANDARDNUM ; jj++)
				{
					if (ojcode.put[jj].yorn == '1')
					{
						comp = CString(ojcode.put[jj].cod2, OCodeLen);
						if (comp.Compare(code) == 0)
						{
							*type = ojcode.put[jj].mchk;
							rValue = true;
							break;
						}
					}
				}

				if (rValue)
					break;
			}
		}
		break;
	case 21: // �ֽĿɼ� ��
	case 31: // �ֽĿɼ� ǲ
		{
			PCODE	pcode;
			for (int ii = 0 ; ii < m_arrayPcode.GetSize() ; ii++)
			{
				pcode = m_arrayPcode.GetAt(ii);
				if (pcode.code.Compare(code) == 0)
				{
					*type = pcode.mchk;
					rValue = true;
					break;
				}
			}
		}
		break;
	}

	return rValue;
}

void CFuturesDlg::OnDblclkPcodeList() 
{
	CString strText;
	m_plistbox.GetText( m_plistbox.GetCurSel(), strText );
	if( m_nStep == BOND )
	{		
		if( strText == "3�ⱹä" )		
			m_nStep2 = Y3_BOND;
		else if( strText == "5�ⱹä" )
			m_nStep2 = Y5_BOND;
		else if( strText == "10�ⱹä" )
			m_nStep2 = Y10_BOND;
		else if( strText == "3��~10�� SP" )
			m_nStep2 = Y310_BOND;
// 		else if( strText == "�������" )
// 			m_nStep2 = SEC_BOND;

		SetFutureCodes();

		return;
	}
	else if( m_nStep == MONEY )
	{
		((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH122))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_BUTTON_MONTH122))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH123))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH124))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BUTTON_MONTH123))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BUTTON_MONTH124))->ShowWindow(SW_HIDE);

		if( strText == "�̱��޷�" )	
		{
			m_nStep2 = USD;
			//2016.06.21 KSJ �̱��޷��϶� 
			((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH122))->ShowWindow(SW_HIDE);
			((CButton*)GetDlgItem(IDC_BUTTON_MONTH122))->ShowWindow(SW_HIDE);
			((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH123))->ShowWindow(SW_SHOW);
			((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH124))->ShowWindow(SW_SHOW);
			((CButton*)GetDlgItem(IDC_BUTTON_MONTH123))->ShowWindow(SW_SHOW);
			((CButton*)GetDlgItem(IDC_BUTTON_MONTH124))->ShowWindow(SW_SHOW);

			m_arrA.RemoveAll();
			m_arrB.RemoveAll();
		}
		else if( strText == "��" )
			m_nStep2 = YEN;
		else if( strText == "����" )
			m_nStep2 = EURO;
		else if( strText == "����" )	//2015.10.05 KSJ ����ȭ �߰�
			m_nStep2 = YUAN;

		SetFutureCodes();

		return;
	}
	else if( m_nStep == COMM )
	{
		if( strText == "��" )		
			m_nStep2 = GOLD;
		else if( strText == "����" )
			m_nStep2 = PIG;
		else if( strText == "�̴ϱ�" )
			m_nStep2 = MINIGOLD;

		SetFutureCodes();

		return;
	}

	const int nItem = m_plistbox.GetCurSel();
	if (nItem == LB_ERR)
		return;
	const int nIdx = m_plistbox.GetItemData(nItem);

	if (m_arrayPcode.GetSize() <= nIdx)
		return;

	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);

	CString sCode = m_arrayPcode.GetAt(nIdx).gcod;
	if (sCode.GetLength() >= 9)
		sCode = sCode.Mid(3, 6);

	CountData(m_arrayPcode.GetAt(nIdx).kind);
	OnButtonOpmonth1();
	ButtonText(m_arrayPcode.GetAt(nIdx).name, sCode);

	m_pCheck->ShowRow(m_nAtm);
}

void CFuturesDlg::OnSelchangePcodeList() 
{
	OnDblclkPcodeList();
}

CString CFuturesDlg::GetString(char *pChar, int nMinLen)
{
	return CString(pChar, min(nMinLen, (int)strlen(pChar)));
}

void CFuturesDlg::OnButtonJoption() 
{
	m_nStep = koptionCODE;
	AdjustControls();
	Invalidate();
	m_ctlButtonJOption.EnableWindow(TRUE);
	ShowJOption(TRUE);
	OnPOPTIONRdo();
	m_plistbox.EnableWindow();
	m_plistbox.SetCurSel(0);
	OnDblclkPcodeList();
}

BOOL CFuturesDlg::GetCommCode( CString& strCode )
{
	CString tmp;
	if (m_codemap.Lookup(strCode, tmp))
	{
		strCode = tmp;
		return TRUE;
	}

	const int idx = strCode.Find( ' ', 0 );

	CString strYear = strCode.Mid( idx+1, strCode.GetLength()-idx-1-2 );	

	if( strYear == "10" )
		strYear = "A";
	else if( strYear == "11" )
		strYear = "B";
	else if( strYear == "12" )
		strYear = "C";

	strYear = "E" + strYear + "000";

	if( m_nStep == BOND )
	{
		if( m_nStep2 == Y3_BOND )	
			strCode = "165" + strYear;
		else if( m_nStep2 == Y5_BOND )
			strCode = "166" + strYear;
		else if( m_nStep2 == Y10_BOND )		
			strCode = "167" + strYear;
// 		else if( m_nStep2 == SEC_BOND )	
// 			strCode = "162" + strYear;
		else 
			return FALSE;
	}
	else if( m_nStep == MONEY )
	{
		if( m_nStep2 == USD )		
			strCode = "175" + strYear;
		else if( m_nStep2 == YEN )
			strCode = "176" + strYear;
		else if( m_nStep2 == EURO )
			strCode = "177" + strYear;
		else if( m_nStep2 == YUAN )		//2015.10.05 KSJ ����ȭ �߰�
			strCode = "178" + strYear;
		else 
			return FALSE;
	}
	else if( m_nStep == COMM )
	{
		if( m_nStep2 == PIG )		
			strCode = "185" + strYear;
		else if( m_nStep2 == GOLD )	
			strCode = "186" + strYear;
		else if( m_nStep2 == MINIGOLD )
			strCode = "187" + strYear;
		else 
			return FALSE;
	}
	else 
		return FALSE;
	
	return TRUE;
}

void CFuturesDlg::SetFutureCodes()
{
	m_codemap.RemoveAll();
	int btnCnt = 0;

	int ia = 0; // ���� ī��Ʈ
	int ib = 0; // ��������ī��Ʈ 

	// ����� 
	for( int ii = 0 ; ii < 8; ii++ )
	{
		((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ii))->SetWindowText("");
		((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ii))->SetWindowText("");
	}

	for (int jj = 0; jj < m_arrayFcode.GetSize(); jj++)
	{
		FCODE fcode = m_arrayFcode.GetAt(jj);
		CString temp = CString(fcode.name), code;
		temp.TrimRight();
		code = m_arrayFcode[jj].code;
		code.TrimLeft(); code.TrimRight();
		/*if (m_bKostar) // Kostar ��� ���� �����Ƿ� �ּ� ó�� 2010.02.24
			if (code.GetLength() <= 3 || code[2] != '3')   //�ּ�
				continue;*/

		CString month = code.Mid(4,1);
		CString text;

		if( month == "A" )
			month = "10";
		else if( month == "B" ) 
			month = "11";
		else if( month == "C" )
			month = "12";

		month += "��";

		if( m_nStep == futureCODE )
		{
// 			if (btnCnt > 7) // ���� ������ ��� ���� 
// 				break;
			if (!code.IsEmpty() && (code[0] == '4' || code[0] == 'D')) // �������� �� ��   //�Ļ���ǰ �ڵ尳��
			{
				((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
				ib++;
			}
			else
			{
				if (m_bKostar)
					temp = "��Ÿ " + temp.Right(4);
				else
					temp = "���� " + temp.Right(4);
				((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(temp);
				m_codemap.SetAt(temp, code);
				ia++;
			}
		}
		else if( m_nStep == BOND ) // �ݸ�����
		{
			CString str3 = code.Left(3);
			CString str2 = code.Mid(1,2);			

			if( m_nStep2 == Y3_BOND )
			{
				//if( str3 == "161" )
				if(str3 == "165")
				{	
					text = "3�ⱹä ";
					text += month;					

					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "465" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}
			else if( m_nStep2 == Y5_BOND )
			{
				//if( str3 == "163" )
				if(str3 == "166")
				{
					text = "5�ⱹä ";
					text += month;

					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "466" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}
			else if( m_nStep2 == Y10_BOND )
			{
				//if( str3 == "164" )
				if(str3 == "167")
				{
					text = "10�ⱹä ";
					text += month;

					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "467" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}
			else if(m_nStep2 == Y310_BOND)   
			{
				if( str3 == "468" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}
/*
			else if( m_nStep2 == SEC_BOND )
			{
				if( str3 == "162" )
				{
					text = "��� ";
					text += month;

					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "462" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}			
*/
		}
		else if( m_nStep == MONEY ) // ��ȭ����
		{
			CString str3 = code.Left(3);
			
			if( m_nStep2 == USD )
			{
				if( str3 == "175" )
				{
					text = "�޷� ";
					text += temp.Right(4);

					if(ia == 8)
					{
						((CButton*)GetDlgItem(IDC_BUTTON_MONTH123))->SetWindowText(text);
						m_arrA.Add(text);
					}
					else if(ia > 8)
					{
						m_arrA.Add(text);
					}
					else
					{
						((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					}

					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "475" )
				{
					if(ib == 8)
					{
						((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH123))->SetWindowText(code);
						m_arrB.Add(code);
					}
					else if(ib > 8)
					{
						m_arrB.Add(code);
					}
					else
					{
						((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					}

					ib++;
				}
			}
			else if( m_nStep2 == YEN )
			{
				if( str3 == "176" )
				{
					text = "�� ";
					text += temp.Right(4);
					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "476" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}
			else if( m_nStep2 == EURO )
			{
				if( str3 == "177" )
				{
					text = "���� ";
					text += temp.Right(4);
					
					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "477" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}			
			else if( m_nStep2 == YUAN )		//2015.10.05 KSJ ����ȭ �߰�
			{
				if( str3 == "178" )
				{
					text = "���� ";
					text += temp.Right(4);
					
					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "478" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}			
		}
		else if( m_nStep == COMM ) // ��ǰ����
		{
			CString str3 = code.Left(3);
		
			if( m_nStep2 == GOLD )
			{
				if( str3 == "185" )
				{
					text = "�� ";
					text += month;

					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "485" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}
			else if( m_nStep2 == PIG )
			{
				if( str3 == "186" )
				{
					text = "���� ";
					text += month;
					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "486" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}
			else if( m_nStep2 == MINIGOLD )
			{
				if ( str3 == "187" )
				{
					text = "�̴ϱ� ";
					text += month;
					((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + ia))->SetWindowText(text);
					m_codemap.SetAt(text, code);
					ia++;
				}
				else if( str3 == "487" )
				{
					((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH3 + ib))->SetWindowText(code);
					ib++;
				}
			}
		}
		
		btnCnt++;
	}
	if (m_Kind == spreadCODE)
	{
		GetDlgItem(IDC_BUTTON_MONTH3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MONTH6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MONTH9)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MONTH12)->EnableWindow(FALSE);
	}
}

void CFuturesDlg::AdjustControls()
{
	CRect checkRC(110, 42 + (m_nStep!=optionCODE?80:10), 447, 267+50);
	switch (m_nStep)
	{
	case futureCODE:
		HideFutureBtns(FALSE);
		ShowJOption(FALSE);
		m_plistbox.EnableWindow(FALSE);
		m_pCheck->SetWindowPos(nullptr, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
		if (m_bKostar || m_Kind == spreadCODE)	// kostar
			m_pCheck->EnableWindow(FALSE);
		else
			//m_pCheck->EnableWindow(FALSE);	//2014.08.11 KSJ ���������� ���õǰ� �ϸ� �ȵ�.
		break;
	case koptionCODE:
		HideFutureBtns();
		ShowJOption(TRUE);
		m_pCheck->EnableWindow();
		m_pCheck->SetWindowPos(nullptr, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
		break;
	case optionCODE:
		checkRC.bottom -= 20;
		HideFutureBtns();
		m_pCheck->EnableWindow();
		m_pCheck->SetWindowPos(nullptr, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
		((CStatic*)GetDlgItem(IDC_STATIC_CAPTION))->ShowWindow(SW_SHOW);
		break;
	case all2CODE:
		ShowJOption(TRUE);
		if (m_Kind == all2CODE && m_type)
		{
			if (m_type & 1)	// Spread disable
				DisableSpread();
			if (m_type & 2)	// koption disable
			{
				ShowJOption(FALSE);
			}
		}
		break;
	case WeeklyOption:
		{
			HideFutureBtns();
		//	CRect checkRC;
			
			if(m_Kind == optionCODE)  //102 �����ڷ� ����� (�ɼ�����)
				checkRC.SetRect(110, 52, 447, 337);
			else
				checkRC.SetRect(110, 22, 447, 337);

			m_pCheck->SetWindowPos(nullptr, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
			m_pCheck->ShowWindow(SW_SHOW);
			((CStatic*)GetDlgItem(IDC_STATIC_CAPTION))->ShowWindow(SW_HIDE);
		}
		break;
	case BOND:  //test
	case MONEY:
		{
			HideFutureBtns(FALSE);
		
			m_plistbox.EnableWindow(FALSE);
			m_pCheck->SetWindowPos(nullptr, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
		}
		break;
	}
}

//2014.09.01 KSJ �������� ��ư �߰�
void CFuturesDlg::DisableSpread()
{
	GetDlgItem(IDC_BUTTON_SPMONTH3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SPMONTH6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SPMONTH9)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SPMONTH12)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SPMONTH32)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SPMONTH62)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SPMONTH92)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SPMONTH122)->EnableWindow(FALSE);
}

void CFuturesDlg::OnClickListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem{};
	CString	code, name;
	POSITION pos = m_codeList.GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			nItem = m_codeList.GetNextSelectedItem(pos);

			code = m_codeList.GetItemText(nItem, 1);
			name = m_codeList.GetItemText(nItem, 0);

			SetCode(code);
			SetName(name);
			
			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}

void CFuturesDlg::OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem{};
	CString	code, name;
	POSITION pos = m_codeList.GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			nItem = m_codeList.GetNextSelectedItem(pos);

			code = m_codeList.GetItemText(nItem, 1);
			name = m_codeList.GetItemText(nItem, 0);

			SetCode(code);
			SetName(name);
			
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
	OnOK();
}

void CFuturesDlg::AddListCtlCode(int nItem, CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();
	int rItem{};
	rItem = m_codeList.InsertItem(LVIF_TEXT|LVIF_PARAM, nItem, szName
		, 0, 0, 0, (LPARAM)0);

	m_codeList.SetItemText(rItem, 1, szCode);
}

void CFuturesDlg::SetCommListBox( BOOL bEnable )
{
	m_plistbox.ResetContent();
	m_plistbox.EnableWindow( bEnable );

	if( !bEnable )
		return;
	
	if( m_nStep == BOND ) // �ݸ����� 
	{
		m_plistbox.AddString( "3�ⱹä" );
		m_plistbox.AddString( "5�ⱹä" );
		m_plistbox.AddString( "10�ⱹä" );
		m_plistbox.AddString( "3��~10�� SP" );  
// 		m_plistbox.AddString( "�������" );
	}
	else if( m_nStep == MONEY ) // ��ȭ���� 
	{
		m_plistbox.AddString( "�̱��޷�" );
		m_plistbox.AddString( "��" );
		m_plistbox.AddString( "����" );		
		m_plistbox.AddString( "����" );	 //2015.10.05 KSJ ����ȭ �߰�	
	}
	else if( m_nStep == COMM ) // ��ǰ���� 
	{
		m_plistbox.AddString( "��" );
		m_plistbox.AddString( "����" );	
		m_plistbox.AddString( "�̴ϱ�" );
	}

}

void CFuturesDlg::SetCodeList(int nMonth)
{
	const int nItem = m_plistbox.GetCurSel();
	if (nItem == LB_ERR)
		return;

	const int nIdx = m_plistbox.GetItemData(nItem);

	if (m_arrayPcode.GetSize() <= nIdx)
		return;
	Standard(m_arrayPcode.GetAt(nIdx).kind);

	if (m_ArrayMonth.GetSize() <= nMonth)
		return;
	
	m_codeList.DeleteAllItems();
	DATA data;
	CString szMonth, szPrice;
	szMonth = m_ArrayMonth.GetAt(nMonth);

	for (int jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // ��...
	{
		szPrice = m_ArrayPrice.GetAt(jj);
		
		data.code = "";
		data.flag = FALSE;
		data = FindCall(szMonth, szPrice);
		if (!data.code.IsEmpty())
			AddListCtlCode(0, data.code, data.name);

		data.code = "";
		data.flag = FALSE;
		data = FindPut(szMonth, szPrice);
		if (!data.code.IsEmpty())
			AddListCtlCode(0, data.code, data.name);
	}
	FindEnd();
}

void CFuturesDlg::OnButtonOpmonth1() 
{
	SetCodeList(3);
}

void CFuturesDlg::OnButtonOpmonth2() 
{
	SetCodeList(2);
}

void CFuturesDlg::OnButtonOpmonth3() 
{
	SetCodeList(1);
}

void CFuturesDlg::OnButtonOpmonth4() 
{
	SetCodeList(0);
}

void CFuturesDlg::ShowJOption(BOOL bShow)
{
	const UINT uShow = bShow?SW_SHOW:SW_HIDE;
	m_ctlButtonOPMonth1.ShowWindow(uShow);
	m_ctlButtonOPMonth2.ShowWindow(uShow);
	m_ctlButtonOPMonth3.ShowWindow(uShow);
	m_ctlButtonOPMonth4.ShowWindow(uShow);
	GetDlgItem(IDC_PRICE_RDO)->ShowWindow(uShow);
	GetDlgItem(IDC_CODE_RDO)->ShowWindow(uShow);

	if (bShow)
	{
		
	}
}

void CFuturesDlg::OnPOPTIONRdo() 
{
	UpdateData();
	if (m_kindList == 0)
	{
		m_pCheck->ShowWindow(SW_SHOW);
		m_codeList.ShowWindow(SW_HIDE);
		m_ctlButtonOPMonth1.EnableWindow(FALSE);
		m_ctlButtonOPMonth2.EnableWindow(FALSE);
		m_ctlButtonOPMonth3.EnableWindow(FALSE);
		m_ctlButtonOPMonth4.EnableWindow(FALSE);
	}
	else
	{
		m_pCheck->ShowWindow(SW_HIDE);
		m_codeList.ShowWindow(SW_SHOW);
		m_ctlButtonOPMonth1.EnableWindow();
		m_ctlButtonOPMonth2.EnableWindow();
		m_ctlButtonOPMonth3.EnableWindow();
		m_ctlButtonOPMonth4.EnableWindow();
		OnButtonOpmonth1();
	}	
}

void CFuturesDlg::HideFutureBtns(BOOL bHide /*= TRUE*/)
{
	GetDlgItem(IDC_BUTTON_MONTH3)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH6)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH9)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH12)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH32)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH62)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH92)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH122)->ShowWindow(bHide?SW_HIDE:SW_SHOW);

	GetDlgItem(IDC_BUTTON_SPMONTH3)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH6)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH9)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH12)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH32)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH62)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH92)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH122)->ShowWindow(bHide?SW_HIDE:SW_SHOW);

	GetDlgItem(IDC_BUTTON_NAME)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_NAME2)->ShowWindow(bHide?SW_HIDE:SW_SHOW);	//2015.06.30 KSJ �̴� �ڽ��� 200 �߰�

	GetDlgItem(IDC_STATIC_JCODE)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_EDIT_CODE)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_STATIC_NAME)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_EDIT_NAME)->ShowWindow(bHide?SW_HIDE:SW_SHOW);

	//2014.08.27 KSJ �ֽļ��� �˻�â�� �ٸ��Ͱ� �ݴ�� �ൿ�ϱ�.
	GetDlgItem(IDC_EDIT_SEARCH)->ShowWindow(!bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_STATIC_NAME2)->ShowWindow(!bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SEARCH_NEXT)->ShowWindow(!bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SEARCH_PREV)->ShowWindow(!bHide?SW_HIDE:SW_SHOW);

	if(m_Kind != sfCODE)
	{
		GetDlgItem(IDC_EDIT_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SEARCH_NEXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SEARCH_PREV)->ShowWindow(SW_HIDE);
	}

	if(m_pCheck->GetSafeHwnd())
		m_pCheck->ShowWindow(bHide?SW_HIDE:SW_SHOW);

	//2016.06.21 KSJ �̱��޷� ��ư �ʱ�ȭ
	((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH123))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH124))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_MONTH123))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_MONTH124))->ShowWindow(SW_HIDE);
}

void CFuturesDlg::SetAll2Code()
{
	if (m_Kind == all2CODE && m_type)
	{
		if (m_type & 1)	// Spread disable
			DisableSpread();
		if (m_type & 2)	// koption disable
		{
			m_ctlButtonJOption.EnableWindow(FALSE);
			m_plistbox.EnableWindow(FALSE);
		}
		if (m_type & 4)	// kostar
		{
			m_ctlButtonKostar.EnableWindow(FALSE);
		}
	}
}

void CFuturesDlg::OnBtnGumri() 
{
	// TODO: Add your control notification handler code here	
	m_nStep = BOND;
	m_nStep2 = Y3_BOND;

	m_bKostar = FALSE;
	Invalidate();
	AdjustControls();

	if(m_Kind != sfCODE && m_pStock->GetSafeHwnd())
	{
		HideFutureBtns(FALSE);

		m_pStock->ShowWindow(SW_HIDE);
	}
	else{
		//2016.06.21 KSJ �̱��޷� ��ư �ʱ�ȭ
		((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH122))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_BUTTON_MONTH122))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH123))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH124))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BUTTON_MONTH123))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BUTTON_MONTH124))->ShowWindow(SW_HIDE);
	}

	SetCommListBox(TRUE);
	m_plistbox.SetCurSel(0);
	
	loadCFjCode();

	SetFutureCodes();

	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_NAME2))->ShowWindow(SW_HIDE);	//2015.06.30 KSJ �̴� �ڽ��� 200 �߰�

	SetButton();

	m_ctlButtonKospi.SetSelected(FALSE);
	m_ctlButtonMiniK200.SetSelected(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
	m_ctlButtonKostar.SetSelected(FALSE);
	m_btnMoney.SetSelected(FALSE);
	m_btnComm.SetSelected(FALSE);
	m_btnGumri.SetSelected(TRUE);

	m_pCheck->EnableWindow( FALSE );
}

void CFuturesDlg::OnBtnMoney() 
{
	// TODO: Add your control notification handler code here
	m_nStep = MONEY;
	m_nStep2 = USD;

	m_bKostar = FALSE;
	Invalidate();
	AdjustControls();

	if(m_Kind != sfCODE && m_pStock->GetSafeHwnd())
	{
		HideFutureBtns(FALSE);

		m_pStock->ShowWindow(SW_HIDE);
	}

	SetCommListBox(TRUE);
	m_plistbox.SetCurSel(0);
	
	loadCFjCode();

	//2016.06.21 KSJ �̱��޷��϶� 
	((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH122))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_MONTH122))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH123))->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH124))->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_BUTTON_MONTH123))->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_BUTTON_MONTH124))->ShowWindow(SW_SHOW);
	
	m_arrA.RemoveAll();
	m_arrB.RemoveAll();
	//2016.06.21 KSJ

	SetFutureCodes();

	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_NAME2))->ShowWindow(SW_HIDE);	//2015.06.30 KSJ �̴� �ڽ��� 200 �߰�

	SetButton();

	m_ctlButtonKospi.SetSelected(FALSE);
	m_ctlButtonMiniK200.SetSelected(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
	m_ctlButtonKostar.SetSelected(FALSE);
	m_btnMoney.SetSelected(TRUE);
	m_btnComm.SetSelected(FALSE);
	m_btnGumri.SetSelected(FALSE);

	m_pCheck->EnableWindow( FALSE );
}

void CFuturesDlg::OnBtnComm() 
{
	// TODO: Add your control notification handler code here
	m_nStep = COMM;
	m_nStep2 = GOLD;

	m_bKostar = FALSE;
	Invalidate();
	AdjustControls();

	if(m_Kind != sfCODE && m_pStock->GetSafeHwnd())
	{
		HideFutureBtns(FALSE);

		m_pStock->ShowWindow(SW_HIDE);
	}

	SetCommListBox(TRUE);
	m_plistbox.SetCurSel(0);
	
	loadCFjCode();

	SetFutureCodes();

	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_NAME2))->ShowWindow(SW_HIDE);	//2015.06.30 KSJ �̴� �ڽ��� 200 �߰�

	SetButton();

	m_ctlButtonKospi.SetSelected(FALSE);
	m_ctlButtonMiniK200.SetSelected(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
	m_ctlButtonKostar.SetSelected(FALSE);
	m_btnMoney.SetSelected(FALSE);
	m_btnComm.SetSelected(TRUE);
	m_btnGumri.SetSelected(FALSE);

	m_pCheck->EnableWindow( FALSE );
}

void CFuturesDlg::OnButtonKospi200() 
{
	if (m_Kind != optionCODE)
		m_nStep = futureCODE;
	m_bKostar = FALSE;
	m_nMiniK200 = 0;
	m_pCheck->m_bWeek = false;  

	if(m_Kind == foptionCODE)  //103 �Ļ� �Ϲ�   
		m_ctlButtonWEEKLYOP.EnableWindow(true);

	Invalidate();
	AdjustControls();	

	if(m_Kind != sfCODE && m_pStock->GetSafeHwnd())
	{
		HideFutureBtns(FALSE);

		m_pStock->ShowWindow(SW_HIDE);
	}

	SetCommListBox(FALSE);
	
	
	DATA data;
	CString szTemp;
	int col = 0, row = 0;
	
	loadOjCode();	// �ɼ� �ҷ�����...
	loadFjCode();   // ���� �ҷ����� ...

	SetFutureCodes();

//	m_nCountMonth = 4;
	m_nCountMonth = STANDARDNUM;

	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);
	
	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_NAME2))->ShowWindow(SW_HIDE);	//2015.06.30 KSJ �̴� �ڽ��� 200 �߰�

	SetButton();

	if (m_Kind != faoptionCODE)   //115	 future + option + kospi option
	{
		m_ctlButtonKospi.m_bNoFocus = FALSE;
		m_ctlButtonKospi.SetSelected(TRUE);
		m_ctlButtonKospi.SetBold(TRUE);

		m_ctlButtonMiniK200.m_bNoFocus = TRUE;	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
		m_ctlButtonMiniK200.SetSelected(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
		m_ctlButtonMiniK200.SetBold(FALSE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
		m_ctlButtonKostar.SetSelected(FALSE);

		m_ctlButtonWEEKLYOP.m_bNoFocus = TRUE;	
		m_ctlButtonWEEKLYOP.SetSelected(FALSE);	
		m_ctlButtonWEEKLYOP.SetBold(FALSE);	
		m_ctlButtonWEEKLYOP.SetSelected(FALSE);

		m_btnMoney.SetSelected(FALSE);
		m_btnComm.SetSelected(FALSE);
		m_btnGumri.SetSelected(FALSE);

		SetTimer(1000, 10, nullptr);	//2015.09.14 KSJ ó����ﶧ�� ��Ŀ���� ���� �ʾƼ�..
	}

	if (m_Kind == optionCODE)   //102
	{
		ButtonText("KOSPI200", _T(""));
		m_ctlButtonName.SetSelected(TRUE);
		m_ctlButtonName.SetBold(TRUE);

		ButtonText("MINI KOSPI200 ", _T(""), TRUE);
		m_ctlButtonName2.SetSelected(FALSE);
		m_ctlButtonName2.SetBold(FALSE);
		m_pCheck->ShowWindow(SW_SHOW);	//2013.07.03 KSJ ��Ʈ���� ������ �ʾ� ���̵��� ó��
		
		m_ctlButtonOPWeek.SetWindowText("��Ŭ�� �ɼ�");
		m_ctlButtonOPWeek.SetSelected(FALSE);
		m_ctlButtonOPWeek.SetBold(FALSE);

		m_ctlButtonOPWeek.ShowWindow(SW_SHOW);
		
	}

	if (m_Kind != faoptionCODE && m_Kind != all2CODE)
		DisableButton();

	if (m_Kind == futureCODE)
	{
		m_pCheck->ShowWindow(SW_HIDE);	//2013.07.03 KSJ ��Ʈ���� ������ �ʾ� ���̵��� ó��
	}

	SetAll2Code();

	m_pCheck->RemoveAll();
	m_pCheck->SetCount(m_arrayOcode.GetSize());

	// CheckBox�� ������ �¹�����...
	for (int ii = m_arrayOcode.GetSize() - 1; ii >= 0 ; ii--)
	{
		const ojcode OJCode  = m_arrayOcode.GetAt(ii);
		
		if (OJCode.atmg == 1)	// ATM ���ϱ� ...
			m_nAtm = abs(ii - (m_arrayOcode.GetSize() - 1));

		int jj = 0;
		// call... ������ 
		for (jj = 0; jj < STANDARDNUM; jj++)
		{
			if (OJCode.call[jj].yorn == '0')
			{
				data.atm = FALSE;
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{	
				data.flag = TRUE;
				data.code = OJCode.call[jj].cod2;
				szTemp = OJCode.call[jj].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;
				
				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// ������ ����..
		}
		
		szTemp = "";
		data.name = CString(OJCode.price, OPriceLen);

		float	val = (float)atol(data.name);
			val /= 100;
			data.name.Format("%3.1f", val);

		m_pCheck->AddData(row, jj, data);
		data.name = "";
		
		//put... ������ 
		for (jj = STANDARDNUM + 1; jj < ROWCOUNT; jj++)
		{
			col = jj - (STANDARDNUM + 1);
			if (OJCode.put[col].yorn == '0')		
			{	
				data.atm = FALSE;
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{
				data.flag = TRUE;
				data.code = OJCode.put[col].cod2;
				szTemp = OJCode.put[col].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;	

				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// ������ ����..

		}			
		++row;
	}
	m_pCheck->ShowRow(m_nAtm);	
	m_pCheck->EnableWindow(TRUE);

	//2015.09.14 KSJ �̴����� ����
	m_bMini = FALSE;
	WritePrivateProfileString("AxisCode", "mini", "0", m_file);
}

void CFuturesDlg::OnBtnWeekop() 
{
	// TODO: Add your control notification handler code here
	m_nStep = WeeklyOption;
  //m_pCheck �� ������ �ֱ� 
	AdjustControls();
	SetCommListBox(FALSE);
	
	
	DATA data;
	CString szTemp;
	int col = 0, row = 0;
	
	loadWCode();	// ��Ŭ�� �ɼ� �ҷ�����...

	m_nCountMonth = STANDARDNUM;

	m_pCheck->m_bWeek = true;
	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);
	
	SetButton();

	if (m_Kind == optionCODE)
	{
		ButtonText("KOSPI200", _T(""));
		m_ctlButtonName.SetSelected(FALSE);
		m_ctlButtonName.SetBold(FALSE);

		ButtonText("MINI KOSPI200 ", _T(""), TRUE);
		m_ctlButtonName2.SetSelected(FALSE);
		m_ctlButtonName2.SetBold(FALSE);

		m_ctlButtonOPWeek.ShowWindow(SW_SHOW);
		m_ctlButtonOPWeek.SetWindowText("��Ŭ�� �ɼ�");
		m_ctlButtonOPWeek.SetSelected(TRUE);
		m_ctlButtonOPWeek.SetBold(TRUE);
		m_ctlButtonOPWeek.Invalidate(FALSE);

		m_pCheck->ShowWindow(SW_SHOW);	//2013.07.03 KSJ ��Ʈ���� ������ �ʾ� ���̵��� ó��
		
		m_ctlButtonOPWeek.ShowWindow(SW_SHOW);
		
	}

	m_ctlButtonKospi.m_bNoFocus = TRUE;
	m_ctlButtonKospi.SetSelected(FALSE);
	m_ctlButtonKospi.SetBold(FALSE);

	m_ctlButtonMiniK200.m_bNoFocus = TRUE;
	m_ctlButtonMiniK200.SetSelected(FALSE);
	m_ctlButtonMiniK200.SetBold(FALSE);

	m_ctlButtonWEEKLYOP.m_bNoFocus =FALSE;
	m_ctlButtonWEEKLYOP.SetFocus();
	m_ctlButtonWEEKLYOP.SetSelected(TRUE);
	m_ctlButtonWEEKLYOP.SetBold(TRUE);
	SetTimer(3000, 10, nullptr);

	if (m_Kind != faoptionCODE && m_Kind != all2CODE)
		DisableButton();

	SetAll2Code();

	m_pCheck->RemoveAll();
	m_pCheck->SetCount(m_arrayOcode.GetSize());

	// CheckBox�� ������ �¹�����...
	for (int ii = m_arrayOcode.GetSize() - 1; ii >= 0 ; ii--)
	{
		const ojcode OJCode = m_arrayOcode.GetAt(ii);
		
		if (OJCode.atmg == 1)	// ATM ���ϱ� ...
			m_nAtm = abs(ii - (m_arrayOcode.GetSize() - 1));

		// call... ������ 
		int icheck = 0 ;
		int jj = 0;
		for (jj = 0; jj < STANDARDNUM; jj++)
		{
			
			//if (OJCode.call[jj].yorn == '0')
			if (OJCode.call[jj].yorn == '0' || OJCode.call[jj].yorn == 0)
			{
				data.atm = FALSE;
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{	
				icheck++;
				data.flag = TRUE;
				data.code = OJCode.call[jj].cod2;
				szTemp = OJCode.call[jj].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;
				
				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// ������ ����..
		}
		
		szTemp = "";
		data.name = CString(OJCode.price, OPriceLen);

		float	val = (float)atol(data.name);
			val /= 100;
			data.name.Format("%3.1f", val);

		m_pCheck->AddData(row, jj, data);
		data.name = "";

		//put... ������ 
		for (jj = STANDARDNUM + 1; jj < ROWCOUNT; jj++)
		{
			col = jj - (STANDARDNUM + 1);
			if (OJCode.put[col].yorn == '0')		
			{	
				data.atm = FALSE;
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{
				data.flag = TRUE;
				data.code = OJCode.put[col].cod2;
				szTemp = OJCode.put[col].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;	

				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// ������ ����..

		}			
		++row;
	}
	m_pCheck->ShowRow(m_nAtm);	
	m_pCheck->EnableWindow(TRUE);

	//2015.09.14 KSJ �̴����� ����
	m_bMini = FALSE;
	WritePrivateProfileString("AxisCode", "mini", "2", m_file);
	

}

//2015.06.29 KSJ
void CFuturesDlg::OnButtonMiniK200() 
{
	if (m_Kind != optionCODE)
		m_nStep = futureCODE;
	m_bKostar = FALSE;
	m_nMiniK200 = 1;
	m_pCheck->m_bWeek = false;  
	Invalidate();
	AdjustControls();	
//	m_ctlButtonWEEKLYOP.EnableWindow(false);
	if(m_Kind != sfCODE && m_pStock->GetSafeHwnd())
	{
		HideFutureBtns(FALSE);
		m_pStock->ShowWindow(SW_HIDE);
	}

	SetCommListBox(FALSE);
	
	
	DATA data;
	CString szTemp;
	int col = 0, row = 0;
	
	loadMoCode();	// �ɼ� �ҷ�����...
	loadMfCode();   // ���� �ҷ����� ...

	SetFutureCodes();

//	m_nCountMonth = 4;
	m_nCountMonth = STANDARDNUM;

	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);
	
	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_NAME2))->ShowWindow(SW_HIDE);	//2015.06.30 KSJ �̴� �ڽ��� 200 �߰�

	SetButton();

	if (m_Kind != faoptionCODE)
	{
	//	m_ctlButtonKospi.m_bNoFocus = TRUE;
		m_ctlButtonKospi.m_bNoFocus = false;
		m_ctlButtonKospi.SetSelected(FALSE);
		m_ctlButtonKospi.SetBold(FALSE);

		::SetFocus(m_ctlButtonMiniK200);
		m_ctlButtonMiniK200.SetFocus();
		m_ctlButtonMiniK200.m_bNoFocus = FALSE;
		m_ctlButtonMiniK200.SetSelected(TRUE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
		m_ctlButtonMiniK200.SetBold(TRUE);	//2015.06.29 KSJ �̴� �ڽ���200 �߰�
		m_ctlButtonKostar.SetSelected(FALSE);

		m_ctlButtonWEEKLYOP.m_bNoFocus = TRUE;	
		m_ctlButtonWEEKLYOP.SetSelected(FALSE);	
		m_ctlButtonWEEKLYOP.SetBold(FALSE);	
		m_ctlButtonWEEKLYOP.SetSelected(FALSE);

		m_btnMoney.SetSelected(FALSE);
		m_btnComm.SetSelected(FALSE);
		m_btnGumri.SetSelected(FALSE);

		SetTimer(2000, 10, nullptr);	//2015.09.14 KSJ ó����ﶧ�� ��Ŀ���� ���� �ʾƼ�..
	}

	if (m_Kind == optionCODE)
	{
		ButtonText("KOSPI200", _T(""));
		m_ctlButtonName.SetSelected(FALSE);
		m_ctlButtonName.SetBold(FALSE);
		ButtonText("MINI KOSPI200", _T(""), TRUE);
		m_ctlButtonName2.SetSelected(TRUE);
		m_ctlButtonName2.SetBold(TRUE);

		m_ctlButtonOPWeek.SetWindowText("��Ŭ�� �ɼ�");
		m_ctlButtonOPWeek.SetSelected(FALSE);
		m_ctlButtonOPWeek.SetBold(FALSE);
		m_ctlButtonOPWeek.ShowWindow(SW_SHOW);

		m_pCheck->ShowWindow(SW_SHOW);	//2013.07.03 KSJ ��Ʈ���� ������ �ʾ� ���̵��� ó��
	}

	if (m_Kind != faoptionCODE && m_Kind != all2CODE)
		DisableButton();

	if (m_Kind == futureCODE)
	{
		m_pCheck->ShowWindow(SW_HIDE);	//2013.07.03 KSJ ��Ʈ���� ������ �ʾ� ���̵��� ó��
	}

	SetAll2Code();

	m_pCheck->RemoveAll();
	m_pCheck->SetCount(m_arrayOcode.GetSize());

	// CheckBox�� ������ �¹�����...
	for (int ii = m_arrayOcode.GetSize() - 1; ii >= 0 ; ii--)
	{
		const ojcode OJCode = m_arrayOcode.GetAt(ii);
		
		if (OJCode.atmg == 1)	// ATM ���ϱ� ...
			m_nAtm = abs(ii - (m_arrayOcode.GetSize() - 1));
		
		int jj = 0;
		// call... ������ 
		for (jj = 0; jj < STANDARDNUM; jj++)
		{
			if (OJCode.call[jj].yorn == '0')
			{
				data.atm = FALSE;
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{	
				data.flag = TRUE;
				data.code = OJCode.call[jj].cod2;
				szTemp = OJCode.call[jj].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;
				
				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// ������ ����..
		}
		
		szTemp = "";
		data.name = CString(OJCode.price, OPriceLen);

		float	val = (float)atol(data.name);
			val /= 100;
			data.name.Format("%3.1f", val);

		m_pCheck->AddData(row, jj, data);
		data.name = "";
		
		//put... ������ 
		for (jj = STANDARDNUM + 1; jj < ROWCOUNT; jj++)
		{
			col = jj - (STANDARDNUM + 1);
			if (OJCode.put[col].yorn == '0')		
			{	
				data.atm = FALSE;
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{
				data.flag = TRUE;
				data.code = OJCode.put[col].cod2;
				szTemp = OJCode.put[col].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;	

				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// ������ ����..

		}			
		++row;
	}
	m_pCheck->ShowRow(m_nAtm);	
	m_pCheck->EnableWindow(TRUE);

	//2015.09.14 KSJ �̴����� ����
	m_bMini = TRUE;
	WritePrivateProfileString("AxisCode", "mini", "1", m_file);
}
//2015.06.29 KSJ End

void CFuturesDlg::OnButtonMonth32() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH32 ))->GetWindowText( strCode );

		GetCommCode( strCode );		

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}	
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			if(m_arrayFcode.GetSize() == 7)	return; // 2014.08.26 KSJ 7���ۿ� �������� ���� Ÿ��ȵ�.
			
			FCODE fcode = m_arrayFcode.GetAt(4 + (m_bKostar?7:0));
			
			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonMonth62() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH62 ))->GetWindowText( strCode );

		GetCommCode( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}	
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			if(m_arrayFcode.GetSize() == 7)	return; // 2014.08.26 KSJ 7���ۿ� �������� ���� Ÿ��ȵ�.

			FCODE fcode = m_arrayFcode.GetAt(5 + (m_bKostar?7:0));
			
			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonMonth92() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH92 ))->GetWindowText( strCode );

		GetCommCode( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);

		OnOK();
	}	
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			if(m_nMiniK200 == 1)	return;			//2015.07.01 KSJ MINI Kospi200�϶�
			if(m_arrayFcode.GetSize() == 7)	return; // 2014.08.26 KSJ 7���ۿ� �������� ���� Ÿ��ȵ�.

			FCODE fcode = m_arrayFcode.GetAt(6 + (m_bKostar?7:0) - m_nMiniK200);
			
			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);

			OnOK();
		}
	}
}

void CFuturesDlg::OnButtonMonth122() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH122 ))->GetWindowText( strCode );

		GetCommCode( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
		OnOK();
	}	
}

void CFuturesDlg::OnButtonSpmonth12() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH12 ))->GetWindowText( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}	
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			if(m_arrayFcode.GetSize() == 7)	return; // 2014.08.26 KSJ 7���ۿ� �������� ���� Ÿ��ȵ�.

			FCODE fcode = m_arrayFcode.GetAt(10 + (m_bKostar?7:0) - m_nMiniK200);
			
			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonSpmonth32() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH32 ))->GetWindowText( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
	}	
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			if(m_arrayFcode.GetSize() == 7)	return; // 2014.08.26 KSJ 7���ۿ� �������� ���� Ÿ��ȵ�.

			FCODE fcode = m_arrayFcode.GetAt(11 + (m_bKostar?7:0) - m_nMiniK200);
			
			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);
		}
	}
}

void CFuturesDlg::OnButtonSpmonth62() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH62 ))->GetWindowText( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);

		OnOK();
	}	
	else
	{
		if(m_arrayFcode.GetSize() != 0)	//2015.07.14 KSJ size üũ
		{
			if(m_nMiniK200 == 1)	return;			//2015.07.01 KSJ MINI Kospi200�϶�
			if(m_arrayFcode.GetSize() == 7)	return; // 2014.08.26 KSJ 7���ۿ� �������� ���� Ÿ��ȵ�.

			FCODE fcode = m_arrayFcode.GetAt(12 + (m_bKostar?7:0) - m_nMiniK200);
			
			MonthInit();
			SetCode(fcode.code);
			SetName(fcode.name);
			UpdateData(FALSE);

			OnOK();
		}
	}

}

void CFuturesDlg::OnButtonSpmonth92() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH92 ))->GetWindowText( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
		OnOK();
	}	
	
}

void CFuturesDlg::OnButtonSpmonth122() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH122 ))->GetWindowText( strCode );

		CString strName =GetFName(strCode);

		SetCode(strCode);
		SetName(strName);
		OnOK();
	}	
}

void CFuturesDlg::OnChangeEditSearch() 
{
	CString strName;

	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strName);

	m_pStock->SearchStock(strName, 0);
}

HBRUSH CFuturesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	DeleteObject(m_hBrush);
	
	const int ctlId = pWnd->GetDlgCtrlID();

	if (ctlId == IDC_BUTTON_SEARCH_NEXT || ctlId == IDC_BUTTON_SEARCH_PREV)
	{
		pDC->SetBkColor(RGB(225,225,225));
		m_hBrush = CreateSolidBrush(RGB(255,255,255));
		return m_hBrush;
	}
	return hbr;
}

//2015.09.09 KSJ �ڷ�
void CFuturesDlg::OnButtonSearchPrev() 
{
	// TODO: Add your control notification handler code here
	CString strName;
	
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strName);
	
	m_pStock->SearchStock(strName, -1);
}

//2015.09.09 KSJ ������
void CFuturesDlg::OnButtonSearchNext() 
{
	// TODO: Add your control notification handler code here
	CString strName;
	
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strName);
	
	m_pStock->SearchStock(strName, 1);
}

void CFuturesDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(nIDEvent == 1000)
	{
		m_ctlButtonKospi.SetFocus();
		KillTimer(1000);
	}
	else if(nIDEvent == 2000)
	{
		m_ctlButtonMiniK200.SetFocus();
		KillTimer(2000);
	}
	else if(nIDEvent == 3000)
	{
		m_ctlButtonWEEKLYOP.SetFocus();
		KillTimer(3000);
	}

	CDialog::OnTimer(nIDEvent);
}

void CFuturesDlg::OnButtonMonth123() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_MONTH123 ))->GetWindowText( strCode );
		
		GetCommCode( strCode );		
		CString strName =GetFName(strCode);
		
		SetCode(strCode);
		SetName(strName);
		OnOK();
	}	
}

void CFuturesDlg::OnButtonMonth124() 
{
	// TODO: Add your control notification handler code here
	CMenu cMenu;
	cMenu.CreatePopupMenu();

	CString strCode, name;
	
	((CButton*)GetDlgItem(IDC_BUTTON_MONTH123))->GetWindowText(strCode);

	for(int ii = 0; ii < m_arrA.GetSize(); ii++)
	{
		name = m_arrA.GetAt(ii);

		if (!strCode.CompareNoCase(name))
			cMenu.AppendMenu(MFT_STRING|MF_CHECKED, ii+1, name);
		else
			cMenu.AppendMenu(MFT_STRING, ii+1, name);
	}

	CPoint	point;
	GetCursorPos(&point);
	int ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, point.x, point.y, this, nullptr);
	ret--;

	if (ret >= 0)
	{
		strCode = m_arrA.GetAt(ret);
			
		((CButton*)GetDlgItem(IDC_BUTTON_MONTH123))->SetWindowText(strCode);
	}
}

void CFuturesDlg::OnButtonSpmonth123() 
{
	// TODO: Add your control notification handler code here
	if( IsComm() )
	{
		CString strCode;
		((CButton*)GetDlgItem( IDC_BUTTON_SPMONTH123 ))->GetWindowText( strCode );
		
		CString strName =GetFName(strCode);
		
		SetCode(strCode);
		SetName(strName);
		OnOK();
	}	
}

void CFuturesDlg::OnButtonSpmonth124() 
{
	// TODO: Add your control notification handler code here
	CMenu cMenu;
	cMenu.CreatePopupMenu();
	
	CString strCode, name;
	
	((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH123))->GetWindowText(strCode);
	
	for(int ii = 0; ii < m_arrB.GetSize(); ii++)
	{
		name = m_arrB.GetAt(ii);
		
		if (!strCode.CompareNoCase(name))
			cMenu.AppendMenu(MFT_STRING|MF_CHECKED, ii+1, name);
		else
			cMenu.AppendMenu(MFT_STRING, ii+1, name);
	}
	
	CPoint	point;
	GetCursorPos(&point);
	int ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, point.x, point.y, this, nullptr);
	ret--;

	if (ret >= 0)
	{
		strCode = m_arrB.GetAt(ret);
		
		((CButton*)GetDlgItem(IDC_BUTTON_SPMONTH123))->SetWindowText(strCode);
	}
}
