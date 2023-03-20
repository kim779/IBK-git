// InputBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiChart.h"
#include "InputBarDlg.h"
#include "StdDialog.h"
#include "./define/ChartMsgDef.h"
//#include "SetUnitDlg.h"
#include "SetCycleDlg.h"
#include "CodeDataFromInputBar.h"
//#include "./include/LocalUtil.h"
#include "./define/MessageDefine.h"
#include "../../inc/IMasterDataMng.h"

#include <math.h>
#include ".\inputbardlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern IMainDllObj	* g_pMainDll;
//const UINT RMSG_CREATEOPENSHAREDATA	= ::RegisterWindowMessage(_T("RMSG_CREATEOPENSHAREDATA"));
const UINT RMSG_GET_CONTROLHANDLE = ::RegisterWindowMessage(_T("RMSG_GET_CONTROLHANDLE"));

//const UINT RMSG_TEST_TR = ::RegisterWindowMessage("RMSG_TEST_TR");

/////////////////////////////////////////////////////////////////////////////
// CInputBarDlg dialog
#define WM_POPUPLIST_SELITEM WM_USER + 77
#define WMU_GET_LAST_KEY			WM_USER + 2237	// AupAgent���� MainFrame�� ������ ��ȸ�� Key�� ��û�Ѵ�.

#define	USER_MAX_INPUT		(800)

#define ID_SHARE_CODE_WAIT 1754
typedef struct 
{
	char code[15];
	char name[40];
} STCODERESULT, *LPSTCODERESULT;

CInputBarDlg::CInputBarDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CInputBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBarDlg)
	m_bChkRevised = TRUE;
	m_bChkLog = FALSE;
	//}}AFX_DATA_INIT
	m_dlgPopupListForHistory = NULL;
	m_pnLeftBarType = NULL;
	m_pChartMng = NULL;
	m_bSettingInitData = FALSE;
	m_chType = DAY_DATA_CHART;
	m_nUnitPos = -1;
	m_lBaseDesk = NULL;
	m_bIsMiniBar = FALSE;
	::ZeroMemory(m_lValueOfButton,sizeof(m_lValueOfButton));			// ��
	::ZeroMemory(m_lValueOfMinButton,sizeof(m_lValueOfMinButton));		// ��
	::ZeroMemory(m_lValueOfSecButton,sizeof(m_lValueOfSecButton));		// ��
	::ZeroMemory(m_lValueOfTickButton,sizeof(m_lValueOfTickButton));	// ƽ
	::ZeroMemory(m_lValueOfCombo,sizeof(m_lValueOfCombo));

	m_lValueOfHourButton[0] = 1;
	m_lValueOfHourButton[1] = 2;
	m_lValueOfHourButton[2] = 3;

	for(int n = 0;n<8;n++)//ƽ �� �� �� �� �� �� �� 
	{
		m_lCntInView[n] = 120;
	}

	m_nCntInViewHanHwa = 120;

	m_nMarketType	= CMasterTypeInfo::NONE_CHART;
	m_nPosCbMarketOld = CMasterTypeInfo::NONE_CHART;

	m_nCurSelCol = 0; 
	m_nCurSelRow = 0;


	m_nTick = 1;
	m_nDay	= 1;
	m_nMin	= 1;
	m_nSec  = 1;

	m_strCodeFromOutside = _T("");
	m_strCode			= _T("");
	m_strOldCode		= _T("");

	m_nFutOptType	= KOSPI_FUTOPT		; //2005. 07. 08

	m_nCurrentMode = CHANGE_ITEM;

	m_pReplayInfo = NULL;

	m_bRunningReplay	= FALSE;
	m_bPauseReplay		= FALSE;
	m_bStop		= TRUE;

	m_bCanRequestHogaTR = TRUE;

	m_pIL_W20H20 = NULL;

	m_bUseUserInputPeriod = FALSE;
	m_pIL_W17H20 = NULL;

	m_bChkPeriod = FALSE;
	m_nFixZoomValue = 120;
	m_nUpjongType = 100;
	m_nUnitValue = -1;

	m_bChkSaveJipyoOneFile = FALSE;
	m_bChkLoadSaveCode = FALSE;

	m_bSelectAction = FALSE;	// ��Ʈ ������ ����Ǿ� �����ڵ尡 ����Ǵ� ��� : �������� - ojtaso (20070323)
	m_bUseReplayChartMode = TRUE; 	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)

	m_bReceiveDataSetCode = FALSE;	// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)

	m_nCurrentMode = CHANGE_ITEM;
	m_bIsShareCodeChanged = FALSE;
	m_rectCmb = CRect(0,0,0,0);

	m_pCodeInput = NULL;
	m_pFOHisBtn = NULL;

	m_bSetFixUnit = false;

	m_bInitChartData = TRUE;
	m_pIL_W46H20 = NULL;
	m_pwndSymbol = NULL;
}


void CInputBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBarDlg)	
	DDX_Control(pDX, IDC_BTSAVECHART, m_btSaveChart);
	DDX_Control(pDX, IDC_BTPERIOD, m_btPeriod);
	DDX_Control(pDX, IDC_PRGBAR_REPLAY, m_prgBarReplay);
	DDX_Control(pDX, IDC_STATIC_REPLAYTIME, m_staticReplayTime);
	DDX_Control(pDX, IDC_CBMARKET, m_cbMarket);
	DDX_Control(pDX, IDC_CMB_DAY, m_cmbDayBefore);
	DDX_Control(pDX, IDC_CHKFIRST, m_chkFirst);
	DDX_Control(pDX, IDC_CHKSECOND, m_chkSecond);
	DDX_Control(pDX, IDC_CHKTHIRD, m_chkThird);
	DDX_Control(pDX, IDC_CHKFOURTH, m_chkFourth);
//	DDX_Control(pDX, IDC_CHKFIFTH, m_chkFifth);
//	DDX_Control(pDX, IDC_CHKSIXTH, m_chkSixth);
	DDX_Control(pDX, IDC_BTN_STOP_REPLAY, m_btnStopReplay);
	DDX_Control(pDX, IDC_BTN_PLAY_PAUSE, m_btnHandleReplay);
	DDX_Control(pDX, IDC_CMB_HOUR, m_cmbReplayHour);
	DDX_Control(pDX, IDC_CMB_MIN, m_cmbReplayMin);
	DDX_Control(pDX, IDC_CMB_SPEED, m_cmbReplaySpeed);
	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbChartViewType);
	DDX_Control(pDX, IDC_BTN_FOREIGN_SENDTR, m_btnForSendTR);
	DDX_Control(pDX, IDC_CHK_DATALIST, m_chkDataListWnd);
	DDX_Control(pDX, IDC_CMB_CHARTTYPE, m_cmbChartType);
	DDX_Control(pDX, IDC_STJONGNAME, m_stJongName);
	DDX_Control(pDX, IDC_CHKDAY, m_chkDay);
	DDX_Control(pDX, IDC_CHKWEEK, m_chkWeek);
	DDX_Control(pDX, IDC_CHKMONTH, m_chkMonth);
	DDX_Control(pDX, IDC_CHKYEAR, m_chkYear);
	DDX_Control(pDX, IDC_CHKMINUTE, m_chkMinute);
	DDX_Control(pDX, IDC_CHKSEC, m_chkSec);
	DDX_Control(pDX, IDC_CHKTICK, m_chkTick);
	DDX_Control(pDX, IDC_EDITPERIOD, m_editPeriod);
	DDX_Control(pDX, IDC_EDITCNTOFDATALL, m_editCntOfDatAll);
	DDX_Control(pDX, IDC_STTEXT, m_stText);
	DDX_Check(pDX, IDC_CHK_REVISED, m_bChkRevised);
	DDX_Control(pDX, IDC_CHKEXTBAR1, m_chkExtBar1);
	DDX_Control(pDX, IDC_BTINC, m_btInc);
	DDX_Control(pDX, IDC_STATICDIV, m_stDiv);
	DDX_Control(pDX, IDC_SPINZOOMVALUE, m_spZoomValue);
	DDX_Control(pDX, IDC_BTREQUESTDATMORE, m_btRequestMore);
	DDX_Control(pDX, IDC_EDITZOOMVALUE, m_editZoomValue);
	DDX_Control(pDX, IDC_CHKLEFT, m_chkLeft);
	DDX_Control(pDX, IDC_STC_DATE, m_stcDate);
	DDX_Control(pDX, IDC_CHK_QUERYOPT, m_chkQueryOpt);
	DDX_Control(pDX, IDC_BTN_JUMUN, m_btJumun);
	DDX_Control(pDX, IDC_BTN_GWANSIM2, m_btnGwanSim); // 0023618: [7112] �ֽ�������Ʈ ȭ�� - �������� ��� ��ư �� ���� ��� �߰� ��û
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBarDlg, CRscDlg)
	//{{AFX_MSG_MAP(CInputBarDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHKLEFT, OnBtleft)
	ON_BN_CLICKED(IDC_CHKDAY, OnChkday)
	ON_BN_CLICKED(IDC_CHKWEEK, OnChkweek)
	ON_BN_CLICKED(IDC_CHKMONTH, OnChkmonth)
	ON_BN_CLICKED(IDC_CHKMINUTE, OnChkminute)
	ON_BN_CLICKED(IDC_CHKSEC, OnChksec)
	ON_BN_CLICKED(IDC_CHKTICK, OnChktick)
	ON_BN_CLICKED(IDC_CHKYEAR, OnChkyear)
	ON_BN_CLICKED(IDC_BTREQUESTDATMORE, OnBtrequestdatmore)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTINC, OnBtinc)
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_CHKEXTBAR1, OnChkextbar)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINZOOMVALUE, OnDeltaposSpinzoomvalue)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DATECNT, OnDeltaposSpinDatecnt)
	ON_BN_CLICKED(IDC_CHK_REVISED, OnChkRevised)
	ON_CBN_SELENDOK(IDC_CMB_CHARTTYPE, OnSelendokCmbCharttype)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)	
	ON_BN_CLICKED(IDC_BTN_FOREIGN_SENDTR, OnBtnForeignSendtr)
	ON_CBN_SELCHANGE(IDC_CMB_TYPE, OnSelchangeCmbType)
	ON_BN_CLICKED(IDC_BTN_PLAY_PAUSE, OnBtnPlayPause)
	ON_BN_CLICKED(IDC_BTN_STOP_REPLAY, OnBtnStopReplay)
	ON_CBN_SELCHANGE(IDC_CMB_SPEED, OnSelchangeCmbSpeed)
	ON_EN_SETFOCUS(IDC_EDITPERIOD, OnSetfocusEditperiod)
	ON_EN_CHANGE(IDC_EDITPERIOD, OnChangeEditperiod)
	ON_BN_CLICKED(IDC_CHKFIRST, OnChkfirst)
	ON_BN_CLICKED(IDC_CHKSECOND, OnChksecond)
	ON_BN_CLICKED(IDC_CHKTHIRD, OnChkthird)
	ON_BN_CLICKED(IDC_CHKFOURTH, OnChkfourth)
//	ON_BN_CLICKED(IDC_CHKFIFTH, OnChkfifth)
//	ON_BN_CLICKED(IDC_CHKSIXTH, OnChksixth)
	ON_CBN_SELCHANGE(IDC_CMB_MIN, OnSelchangeCmbMin)
	ON_CBN_SELCHANGE(IDC_CMB_HOUR, OnSelchangeCmbHour)
	ON_CBN_SELCHANGE(IDC_CMB_DAY, OnSelchangeCmbDay)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_CBMARKET, OnSelchangeCbmarket)
	ON_BN_CLICKED(IDC_BTPERIOD, OnBtperiod)
	ON_BN_CLICKED(IDC_BTMULTICHART, OnBtmultichart)
	ON_CBN_SELENDOK(IDC_CTRL_CODEINPUT, OnSelchangeCodeInput)
	ON_BN_CLICKED(IDC_BTSAVECHART, OnBtSaveChart)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_DOW, OnCheckDow)
	ON_BN_CLICKED(IDC_CHECK_NASDAQ, OnCheckNasdaq)
	ON_BN_CLICKED(IDC_CHECK_NIKEI, OnCheckNikei)
	ON_BN_CLICKED(IDC_CHECK_SP500, OnCheckSp500)
	ON_BN_CLICKED(IDC_CHECK_TWD, OnCheckTwd)
	ON_BN_CLICKED(IDC_CHK_QUERYOPT, OnChkQueryOpt)
	ON_BN_CLICKED(IDC_BTN_JUMUN, OnBtJumun)
	ON_BN_CLICKED(IDC_BTN_GWANSIM2, OnBtnGwansim) // 0023618: [7112] �ֽ�������Ʈ ȭ�� - �������� ��� ��ư �� ���� ��� �߰� ��û
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POPUPLIST_SELITEM, OnSelBasedlgPopuplistctrl)
	ON_REGISTERED_MESSAGE( RMSG_FROMCODELIST,	OnFromCodeList)
	ON_EN_SETFOCUS(IDC_EDIT_FOCUS, OnEnSetfocusEditFocus)
	ON_EN_CHANGE(IDC_FOHISTORYBTN, OnSelendokFOHistoryBtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBarDlg message handlers

void CInputBarDlg::SetAllPosition(int nType)
{
	if(nType == 0)
		return;
	CWnd* pChild = GetWindow(GW_CHILD);
	CRect rectComp;
	while(pChild)
	{
		if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		{
			if(pChild->GetDlgCtrlID() == IDC_CBMARKET || pChild->GetDlgCtrlID() == IDC_CMB_TYPE || pChild->GetDlgCtrlID() == IDC_CHKEXTBAR1
				|| pChild->GetDlgCtrlID() == IDC_CHKFIRST || pChild->GetDlgCtrlID() == IDC_CHKSECOND || pChild->GetDlgCtrlID() == IDC_CHKTHIRD || pChild->GetDlgCtrlID() == IDC_CHKFOURTH || pChild->GetDlgCtrlID() == IDC_CHKFIFTH || pChild->GetDlgCtrlID() == IDC_CHKSIXTH
				|| pChild->GetDlgCtrlID() == IDC_CHKTICK)
			{
				pChild->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			if(pChild->GetDlgCtrlID() == IDC_CBMARKET || pChild->GetDlgCtrlID() == IDC_CMB_TYPE || pChild->GetDlgCtrlID() == IDC_CHKEXTBAR1)
			{
				pChild->ShowWindow(SW_HIDE);
			}
			else if(pChild->GetDlgCtrlID() != IDC_CHKLEFT)
			{
				pChild->GetWindowRect(rectComp);
				ScreenToClient(rectComp);
				pChild->SetWindowPos(NULL, rectComp.left - 177, rectComp.top, rectComp.Width(), 20, SWP_NOZORDER);
			}
		}

		pChild = pChild->GetNextWindow();
	}

	char szTemp[10] = {0,};
	memset(szTemp, 0x00, 10);
	CString strConfig = m_pStdDialog->m_strConfig;//CString strConfig = theStdParent->m_strConfig;
}

BOOL CInputBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();	

	m_pStdDialog = (CStdDialog *)GetParent()->SendMessage(RMSG_GET_STDDLG_PTR);

	SetInitSymbol();
	CWnd* pCodeWnd = GetDlgItem(IDC_CTRL_CODEINPUT);
	if( !pCodeWnd->GetSafeHwnd() || !::IsWindow(pCodeWnd->GetSafeHwnd()) )
	{
		AfxMessageBox("�ý��۸��ҽ��� �����Ͽ� ȭ���� ��� �� �����ϴ�. �ٸ� ȭ���� �����ϰ� �����Ͽ� �ֽʽÿ�.");
		return FALSE;
	}
	CRect rcCode;
	GetDlgItem(IDC_CTRL_CODEINPUT)->GetWindowRect(rcCode);
	ScreenToClient(rcCode);
	GetDlgItem(IDC_CTRL_CODEINPUT)->DestroyWindow();

	if( InitMarketCombo() == -1 )
		return FALSE;

	rcCode.right = rcCode.left + 100;

	m_pCodeInput = new CCodeCtrl(GetParent()->GetParent()->GetParent()->GetParent());

	if( !m_pCodeInput )		
	{
		AfxMessageBox("�ý��۸��ҽ��� �����Ͽ� ȭ���� ��� �� �����ϴ�. �ٸ� ȭ���� �����ϰ� �����Ͽ� �ֽʽÿ�.");
		return FALSE;
	}

	m_pCodeInput->SetRootPath(theStdParent->m_strRootPath);
	m_pCodeInput->UseCodeDlg(TRUE);

	m_pCodeInput->CreateCodeCtrl(this, GetParent()->GetParent()->GetParent()->GetParent(),GetParent()->GetParent()->GetParent(), rcCode, IDC_CTRL_CODEINPUT);

	if( !m_pCodeInput->GetSafeHwnd() || !::IsWindow(m_pCodeInput->GetSafeHwnd()) )		
	{
		AfxMessageBox("�ý��۸��ҽ��� �����Ͽ� ȭ���� ��� �� �����ϴ�. �ٸ� ȭ���� �����ϰ� �����Ͽ� �ֽʽÿ�.");
		return FALSE;
	}
	m_pCodeInput->SetColor(RGB(0,0,0), RGB(255,255,255));
 

	_DoChangeCodeCtrlUnit(m_nMarketType);
	_DoSetPosCodeCodeName(rcCode);

	if(	theStdParent->IsChartScreen("OldFuture")==TRUE)
	{
		m_pCodeInput->ShowWindow(SW_HIDE);
		m_pFOHisBtn = new CFOHistoryBtnCtrl;

		m_pFOHisBtn->CreateFOHistoryBtn(this, GetParent()->GetParent()->GetParent()->GetParent(), rcCode, IDC_FOHISTORYBTN, KIND_ALL);
	}
	InitBaseInfo(NULL, FALSE);
	//��Ų���� ����
	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		m_pwndSymbol->ShowWindow(SW_SHOW);
		m_stJongName.ShowWindow(SW_HIDE);
	}
	else
	{
		m_pwndSymbol->ShowWindow(SW_HIDE);
		m_stJongName.ShowWindow(SW_SHOW);
	}
	m_crBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_stJongName.SetStaticColor(RGB(132,132,132), RGB(255,255,255) , RGB(0,0,0));
	
//KHD : ����� ����ü�� �����. 	
	CFont *font = m_stJongName.GetFont();
	LOGFONT lf;
	font->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	font->CreateFontIndirect(&lf);
	m_stJongName.SetFont(font);
	m_stJongName.EnableToolTips(TRUE);
//END 
	m_crBk = RGB(237,243,249);	//RGB(222,222,222);
	{
		CRect rect;
		m_stJongName.GetWindowRect(rect);
		ScreenToClient(rect);
		m_rectJongName = rect;
		m_pwndSymbol->MoveWindow(m_rectJongName);
	}


	if(m_cmbChartViewType.GetSafeHwnd() && ::IsWindow(m_cmbChartViewType.GetSafeHwnd()) )
	{
		// Combo Set 
		// �������� �߰� UI : �������� - ojtaso (20070522)
		m_cmbChartViewType.AddString("����");
		m_cmbChartViewType.AddString("����");
		m_cmbChartViewType.AddString("�߰�");
		m_cmbChartViewType.AddString("��ø");

		if(m_bUseReplayChartMode)
		{
			int nMarket;
			_DoCheckMarketTypeFromScreen(nMarket);

			if ((nMarket == CMasterTypeInfo::STOCK_CHART)	||
				(nMarket == CMasterTypeInfo::FUTOPT_CHART)	||
				(nMarket == CMasterTypeInfo::COMMODITY_CHART))
			{
				m_cmbChartViewType.AddString("����");
			}

		}

		m_cmbChartViewType.SetCurSel(0);
	}

	if(m_cbMarket.GetSafeHwnd() && ::IsWindow(m_cbMarket.GetSafeHwnd()))
	{
		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
		if ((m_nMarketType == CMasterTypeInfo::FUTOPT_CHART) ||
			(m_nMarketType == CMasterTypeInfo::COMMODITY_CHART))
		{
			m_cbMarket.SetCurSel(GetMarketIdxToCBIdx(m_nMarketType));
		}
		else
			m_cbMarket.SetCurSel(0);
	}

	Init();

	int nID = 5;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
		m_pDataManager = (IMasterDataManagerLast*)pGateMng->GetInterface(nID);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputBarDlg::Init()
{
	//���� ��Ʈ ���� ===================================
	m_ExChangeBeforeInfo.chOldType = NULL;
	m_ExChangeBeforeInfo.nInterval = 0 ;

	m_pReplayInfo		= NULL;
	m_bRunningReplay	= FALSE;
	m_bPauseReplay		= FALSE;
	m_bStop		= TRUE;

	m_cmbDayBefore	.SetCurSel(0);
	m_cmbReplayHour	.SetCurSel(0);
	m_cmbReplayMin	.SetCurSel(0);
	m_cmbReplaySpeed.SetCurSel(0);
	//==================================================

	m_strImagePath = theStdParent->m_szImageDir;

	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
	if (m_nMarketType == CMasterTypeInfo::FUTOPT_CHART || m_nMarketType == CMasterTypeInfo::COMMODITY_CHART)
	{
		m_chkYear.EnableWindow(FALSE);
		m_bChkLog = FALSE;		
		GetDlgItem(IDC_CHK_REVISED)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
	{
		m_bChkLog = FALSE;
		GetDlgItem(IDC_CHK_REVISED)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)	
	{
		GetDlgItem(IDC_CHK_REVISED)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		Replace_ForeignChartControl();

		m_bChkLog = FALSE;		

		GetDlgItem(IDC_CHK_REVISED)->EnableWindow(FALSE);		
		m_chkTick.EnableWindow(false);
		UpdateData(FALSE);
	}

	CString strImageSelect;
	CString strImageUp;
	CString strImageDown;
	CString strImageOver;
	CString strImageDis;

	m_brBkColor.CreateSolidBrush(m_crBk);


	m_pIL_W20H20 = &(theStdParent->m_ImgList_W20H20);
	m_pIL_W46H20 = &(theStdParent->m_ImgList_W46H20);
	if(m_pIL_W46H20 == NULL) 
	{ 
		AfxMessageBox("�̹��� ����");
		return;
	}
	if(m_pIL_W20H20 == NULL) return;
	
	m_chkLeft.FP_vSetImage( m_pIL_W20H20, 56, 58);
	m_chkLeft.FP_vSetToolTipText("��ȭ�鿭��");

	m_btRequestMore.FP_vSetImage( m_pIL_W20H20, 40, 42);
	m_btRequestMore.FP_vSetToolTipText("����Ÿ�߰��θ���");

	m_btSaveChart.FP_vSetImage( m_pIL_W20H20, 4, 6);
	m_btSaveChart.FP_vSetToolTipText("����/�ҷ�����");

	m_chkDay.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE);
	m_chkDay.FP_vSetToolTipText("��");

	m_chkWeek.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);
	m_chkWeek.FP_vSetToolTipText("��");

	m_chkMonth.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);	
	m_chkMonth.FP_vSetToolTipText("��");

	m_chkYear.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);	
	m_chkYear.FP_vSetToolTipText("��");

	m_chkMinute.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);		
	m_chkMinute.FP_vSetToolTipText("��");

	m_chkSec.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);		
	m_chkSec.FP_vSetToolTipText("��");

	m_chkTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ƽ", TRUE, 83);	
	m_chkTick.FP_vSetToolTipText("ƽ");

	m_chkFirst.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "1", TRUE, 83);
	m_chkSecond.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "5", TRUE, 83);
	m_chkThird.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "10", TRUE, 83);
	m_chkFourth.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "15", TRUE, 83);
//	m_chkFifth.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "20", TRUE, 83);
//	m_chkSixth.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "30", TRUE, 83);

//=========================================================================================
// ����
//=========================================================================================
	m_btnStopReplay.FP_vSetImage( m_pIL_W20H20, 64, 65, TRUE, "", FALSE, 67);
	m_btnHandleReplay.FP_vSetImage( m_pIL_W20H20, 68, 69, TRUE, "", FALSE, 71);
//=========================================================================================
	
	

	m_btJumun.FP_vSetImage(m_pIL_W46H20, 32, 34, TRUE, "�ֹ�", FALSE);
	m_btJumun.FP_vSetToolTipText("�ֹ�");
	if(theStdParent->m_nScreenID== 7112 ||theStdParent->m_nScreenID== 7114 )
		m_btJumun.ShowWindow(SW_SHOW);

	//--> 0023618: [7112] �ֽ�������Ʈ ȭ�� - �������� ��� ��ư �� ���� ��� �߰� ��û
	CImageList* pIL_W46H20_Gwan = &(((CStdDialog*)GetParent())->m_ImgList_W46H20_Gwan);
	m_btnGwanSim.FP_vSetImage(pIL_W46H20_Gwan, 0, 2, TRUE, "", FALSE);
	m_btnGwanSim.FP_vSetToolTipText("���ɵ��");
	if(theStdParent->m_nScreenID == 7112)
		m_btnGwanSim.ShowWindow(SW_SHOW);
	//<--

	m_pIL_W17H20 = &(theStdParent->m_ImgList_W17H20);
	if(m_pIL_W17H20 == NULL) return;
	m_btPeriod.FP_vSetImage( m_pIL_W17H20, 0, 2,TRUE, "", FALSE, 3);
	
	if(theStdParent->m_nChartType ==MULTI_CHART || m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		m_chkQueryOpt.ShowWindow(SW_HIDE);
	
//{{������Ʈ �ڽ���,�ڽ��� ���� ��ư �߰�
//@��������	m_btChangeUpjong.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "K", FALSE);
//}}

	char szTemp[10] = {0,};
//	UpdateData(FALSE);

//@��������
//	LoadInfoFutureList();	
//@��������
	LoadValue();

	UpdateBtNCbValue();
	
	//2005. 05. 09 by sy,nam--------------------------------------------------------
	CString sTemp;
	memset(szTemp, 0x00, 10);
	CString strConfig = m_pStdDialog->m_strConfig;
	
	::GetPrivateProfileString("0000", "Type", "2", szTemp, sizeof(szTemp), strConfig);
	if(szTemp[0] == 0x00)
		szTemp[0] = DAY_DATA_CHART;
	
	if(szTemp[0] == TICK_DATA_CHART)
	{
		::GetPrivateProfileString("0000", "TickValue", "1", szTemp, sizeof(szTemp), strConfig);
		sTemp.Format("%s", szTemp);
		sTemp.TrimLeft('0');
		m_editPeriod.SetWindowText(sTemp);	

		m_chType	= TICK_DATA_CHART;
//		m_nUnitPos	= 0;	
		m_nTick		= atoi(sTemp);
		m_chkTick	.SetCheck(TRUE);
	}
	else if(szTemp[0] == SEC_DATA_CHART)
	{
		::GetPrivateProfileString("0000", "SecValue", "1", szTemp, sizeof(szTemp), strConfig);
		sTemp.Format("%s", szTemp);
		sTemp.TrimLeft('0');
		m_editPeriod.SetWindowText(sTemp);
		
		m_chType = SEC_DATA_CHART;
//		m_nUnitPos = 1;	
		m_nSec = atoi(sTemp);
		m_chkSec.SetCheck(TRUE);
	}
	else if(szTemp[0] == MIN_DATA_CHART)
	{
		::GetPrivateProfileString("0000", "MinValue", "1", szTemp, sizeof(szTemp), strConfig);
		sTemp.Format("%s", szTemp);
		sTemp.TrimLeft('0');
		m_editPeriod.SetWindowText(sTemp);	
		
		m_chType = MIN_DATA_CHART;
//		m_nUnitPos = 1;	
		m_nMin = atoi(sTemp);
		m_chkMinute.SetCheck(TRUE);
	}
	else if(szTemp[0] == DAY_DATA_CHART)
	{
		::GetPrivateProfileString("0000", "DayValue", "1", szTemp, sizeof(szTemp), strConfig);
		sTemp.Format("%s", szTemp);
		sTemp.TrimLeft('0');
		m_editPeriod.SetWindowText(sTemp);	
		
		m_chType = DAY_DATA_CHART;
//		m_nUnitPos = 2;	
		m_nDay = atoi(sTemp);
		m_chkDay.SetCheck(TRUE);
	}
	//else if(szTemp[0] == HOUR_DATA_CHART)
	//{
	//	::GetPrivateProfileString("0000", "HourValue", "1", szTemp, sizeof(szTemp), strConfig);
	//	sTemp.Format("%s", szTemp);
	//	sTemp.TrimLeft('0');
	//	m_editPeriod.SetWindowText(sTemp);	

	//	m_chType = HOUR_DATA_CHART;
	//	//		m_nUnitPos = 1;	
	//	m_nHour = atoi(sTemp);
	//	m_chkHour.SetCheck(TRUE);
	//}
	else if(szTemp[0] == WEEK_DATA_CHART)
	{
		m_editPeriod.SetWindowText("1");			
		m_chType = WEEK_DATA_CHART;
		m_nUnitPos = -1;			
		m_chkWeek.SetCheck(TRUE);
	}
	else if(szTemp[0] == MONTH_DATA_CHART)
	{
		m_editPeriod.SetWindowText("1");			
		m_chType = MONTH_DATA_CHART;
		m_nUnitPos = -1;			
		m_chkWeek.SetCheck(TRUE);
	}
	else if(szTemp[0] == YEAR_DATA_CHART)
	{
		m_editPeriod.SetWindowText("1");			
		m_chType = YEAR_DATA_CHART;
		m_nUnitPos = -1;			
		m_chkYear.SetCheck(TRUE);
	}	
//	else if(szTemp[0] == SEC10_DATA_CHART)
//	{
//		m_editPeriod.SetWindowText("1");			
//		m_chType = SEC10_DATA_CHART;
//		m_nUnitPos = -1;			
//		//m_chkYear.SetCheck(TRUE);
//	}
//	else if(szTemp[0] == SEC30_DATA_CHART)
//	{
//		m_editPeriod.SetWindowText("1");			
//		m_chType = SEC30_DATA_CHART;
//		m_nUnitPos = -1;			
//		//m_chkYear.SetCheck(TRUE);
//	}

	Init_Size();
	m_chkDay.Invalidate();
	m_chkWeek.Invalidate();
	m_chkMonth.Invalidate();
	m_chkYear.Invalidate();
	m_chkMinute.Invalidate();
	m_chkSec.Invalidate();
	m_chkTick.Invalidate();
//@��������	m_btnForSendTR.Invalidate();	

	UpdateBtNCbValue();	// �ֱ��ư Set
	Enable_PeriodControl(TRUE);	
	
	SetExt(0);	

	if(theStdParent->m_nChartType==SINGLE_CHART)	//������Ʈ �� ���
	{
		m_bChkLoadSaveCode = FALSE;
	}
	else
	{
		if(::GetPrivateProfileString("InputBar","LoadSaveCode","0",szTemp,sizeof(szTemp),strConfig)>0)
		{
			
			//@100223-A002�ӽ�		szTemp[0] = '1';
			if(szTemp=="1")
			{
				m_bChkLoadSaveCode = TRUE;
			}
			else
			{
				m_bChkLoadSaveCode = FALSE;
			}
		}
		else
		{
			m_bChkLoadSaveCode = FALSE;
		}
	}

	m_bChkRevised = (BOOL)::GetPrivateProfileInt("InputBar","Revised", 1, strConfig);
	m_bChkLog = (BOOL)::GetPrivateProfileInt("InputBar","Log", 0 ,strConfig);

// 	// ���⼭ ����� ��� ó��.
// 	//{
// 		::GetPrivateProfileString("InputBar","SaveJipyoOneFile","1",szTemp,sizeof(szTemp),strConfig);
// 		if(szTemp[0]=='1')
// 			m_bChkSaveJipyoOneFile = TRUE;
// 		else
// 			m_bChkSaveJipyoOneFile = FALSE;
// 		m_bChkSaveJipyoOneFile = TRUE;
// 	//}

	SetMultiChartItemMode();

	//@solomon	100104SM024
	if (theStdParent->IsChartScreen("OldFuture") == TRUE)
		m_stcDate.ShowWindow(SW_HIDE);
}

void CInputBarDlg::Init_Size()
{
	m_chkFirst.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkSecond.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkThird.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkFourth.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
//	m_chkFifth.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
//	m_chkSixth.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkDay.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkWeek.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkMonth.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkYear.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkMinute.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkSec.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_chkTick.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);

	m_btRequestMore.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
	m_btSaveChart.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);
//	m_btJumun.SetWindowPos(NULL, 0, 0, 20, 20, SWP_NOMOVE|SWP_NOZORDER);

}

void CInputBarDlg::OnBtleft() 
{
	// TODO: Add your control notification handler code here
	UINT nLeftBarType = theStdParent->GetLeftBarType();
	if(nLeftBarType==0||nLeftBarType==2)
	{
		SetLeftBarType(1);
		theStdParent->SetLeftBarType(1);		
	}
	else if(nLeftBarType==1)
	{
		SetLeftBarType(0);
		theStdParent->SetLeftBarType(0);
	}
}

void CInputBarDlg::OnBtright() 
{
	// TODO: Add your control notification handler code here
	UINT nLeftBarType = theStdParent->GetLeftBarType();
	if(nLeftBarType==0||nLeftBarType==1)
	{
		SetLeftBarType(2);
		theStdParent->SetLeftBarType(2);
	}
	else if(nLeftBarType==2)
	{
		SetLeftBarType(0);
		theStdParent->SetLeftBarType(0);
	}	
}

void CInputBarDlg::SetLeftBarType(UINT nLeftBarType)
{
	//@091113
	if(!m_chkLeft.GetSafeHwnd() || !IsWindow(m_chkLeft.GetSafeHwnd()))
		return;

	switch(nLeftBarType) {
	case 0:
	case 2:
		m_chkLeft.FP_vSetToolTipText("��ȭ�鿭��");
		m_chkLeft.SetCheck(1);
		//m_chkLeft.FP_vSetImage( m_pIL_W20H20, 35, 36);//normal, down, disable, focus	
		m_chkLeft.FP_vSetImage( m_pIL_W20H20, 56, 58);//normal, down, disable, focus	
		break;
	case 1:
		m_chkLeft.FP_vSetToolTipText("��ȭ��ݱ�");
		m_chkLeft.SetCheck(0);
		//m_chkLeft.FP_vSetImage( m_pIL_W20H20, 37, 38);//normal, down, disable, focus	
		m_chkLeft.FP_vSetImage( m_pIL_W20H20, 60, 62);//normal, down, disable, focus	
		break;
//	case 2:
//		m_chkLeft.FP_vSetToolTipText("��ȭ�鿭��");
//		m_chkLeft.SetCheck(1);
//		m_chkLeft.FP_vSetImage( m_pIL_W20H20, 35, 36);//normal, down, disable, focus	
//		break;
	default:
		break;
	}
}

int CInputBarDlg::GetEndDate()
{
	long lEndDate = 0;
	if((m_chType	== DAY_DATA_CHART	||
		m_chType	== WEEK_DATA_CHART	||
		m_chType	== MONTH_DATA_CHART	||
		m_chType	== YEAR_DATA_CHART	||
		m_chType	== NULL))
	{
		// �ϴ� Toolbar���� ���´�.
		SYSTEMTIME tm;
		if(m_pChartMng->GetSafeHwnd())		
		{
			return m_pChartMng->SendMessage(UMSG_GET_TIME_FROM_DATETIMEPICKER, 0, (LPARAM)&tm);
		}
	}	
	return lEndDate;

// ���� �ҽ� 
//	long lEndDate = 0;
//	if(m_chkFixPeriod.GetCheck()&&(m_chType=='2'||m_chType=='3'||m_chType=='4'||m_chType==NULL))
//	{
//		SYSTEMTIME tm;
//		m_datePicker.GetTime(&tm);
//		lEndDate = tm.wYear*10000 + tm.wMonth*100 + tm.wDay;
//	}	
//	return lEndDate;
}
LRESULT CInputBarDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_INPUTCODETOINPUTBAR:
		{
			STINPUTCODEDATA2 *pStInputCodeData2 = (STINPUTCODEDATA2 *)wParam;

#ifdef _WRITELOG			// Log - ojtaso (20080107)
			theStdParent->WriteToStringLog(_T("����� �����ڵ� Input��Ʈ�ѿ� ����"), pStInputCodeData2->m_strCode, pStInputCodeData2->m_strCode.GetLength());
#endif			
			BOOL bDontPutInShared = pStInputCodeData2->m_bDontPutInShared;
			CString strInc;

			{
				// CodeInput Ctrl�� SetDataText �Լ��� ȣ��� ���� ���
/*	����(20080922) CodeInput Ctrl���� ���� �����ڵ� �޼����� ���� �ʾ� �ּ�ó�� ��
				if(GetShareCodeChanged())
				{
					SetShareCodeChanged(FALSE);
					break;
				}				
*/
				
				// ���ǿ� �������� ����
				if(GetCurrentCode() != pStInputCodeData2->m_strCode || pStInputCodeData2->m_bRequestWithoutHesitation)
				{	
					m_strCodeFromOutside = pStInputCodeData2->m_strCode;

					m_strOldCode = GetCurrentCode();
					SetCurrentCode(m_strCodeFromOutside);
					EnableWindow(FALSE);
					
					if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
					{
						if(GetType() == SEC_DATA_CHART)
						{
							m_chType = DAY_DATA_CHART;
							//@solomon	100104SM027
							m_editPeriod.SetWindowText(_T("1"));
							UpdateBtNCbValue();	// �ֱ��ư Set
							SetType(FALSE);				
						}
					}
					else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
					{
						if(/*GetType() == MIN_DATA_CHART || */GetType() == SEC_DATA_CHART)
						{
							m_chType = DAY_DATA_CHART;
							//@solomon	100104SM027
							m_editPeriod.SetWindowText(_T("1"));
							UpdateBtNCbValue();	// �ֱ��ư Set
//							((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetChartRequestType(DAY_DATA_CHART);
							SetType(FALSE);
						}
					}

					CCodeDataFromInputBar CodeData;
					CodeData.m_strCode = GetCurrentCode();
					CodeData.m_lEndTime = GetEndDate();
					CodeData.m_chType = GetType();
					CodeData.m_lTypeNum = GetTypeNumber();				
					
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					{
						m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
//						CString strCodeInputCode = m_ctrlCodeInput.GetDataText();
//						if(GetCurrentCode().GetLength() && strCodeInputCode != GetCurrentCode() && !((CChartMng*)m_pChartMng)->IsInLoop())
//						{
//							SetShareCodeChanged(TRUE);
//							m_ctrlCodeInput.SetDataText(GetCurrentCode());
//						}
					}
					else
					{
						if(m_pCodeInput->GetSafeHwnd())
						{
							m_pCodeInput->SetCode("");
							m_strOldCode = _T("");
							m_strCodeFromOutside = _T("");
							SetCurrentCode(_T(""));
							SetCurCodeNameToControl(_T(""));
						}
					}
					EnableWindow(TRUE);
				}
			}			
		}
		break;
	case UMSG_SETZOOMVALUE:
		{
			CString strZoomValue;
			strZoomValue.Format("%d",wParam);
			SetEditZoomValue(strZoomValue, (BOOL)lParam);
//@solomon	100104SM015
//			m_lCntInView[m_chType-'0'] = wParam; //05.09.13
		}
		break;
	case UMSG_SETINITIALCHARTDATA:
		{
			//05. 09. 08
			SetUseUserInputPeriod(FALSE);

			m_bSettingInitData = TRUE;
			stChartInitData* _pStData = (stChartInitData*)lParam;

			if(!_pStData->m_chType) return 0L;

			//07.01.20 by LYH �߰�
			char oldChType = m_chType;
			//}}
			
			CString strValue;			
//@100303-A026-->
			//strValue.Format("%d",_pStData->m_lTotalCnt);
			//m_editCntOfDatAll.SetWindowText(strValue);
 			{
				long lCount=0;
				if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
				{
					char cType;
					CString strNumber, strDate, strTime;
					
					((CChartMng*)m_pChartMng)->GetCurStatusInfo(cType, strNumber, strDate, strTime, lCount);
				}

				if(_pStData->m_lTotalCnt < lCount)
				{
					strValue.Format("%d",_pStData->m_lTotalCnt);					
				}
				else
				{
					strValue.Format("%d", lCount);
				}
				m_editCntOfDatAll.SetWindowText(strValue);

//  				CString rString;
//  				m_editCntOfDatAll.GetWindowText(rString);
//  				if(atoi(rString)==0 || atoi(rString)>_pStData->m_lTotalCnt)
//  					m_editCntOfDatAll.SetWindowText(strValue);
 			}
//@100303-A026<--

			if(m_stcDate.GetSafeHwnd())
			{
				CTime t = CTime::GetCurrentTime();
				CString strDate = t.Format("%Y/%m/%d");
				m_stcDate.SetWindowText(strDate);
			}
			
			if(m_bChkPeriod)
			{
				strValue.Format("%d",m_nFixZoomValue);
				SetEditZoomValue(strValue);//m_editZoomValue.SetWindowText(strValue);
			}
			else
			{
				strValue.Format("%d",_pStData->m_lZoomValue);
				SetEditZoomValue(strValue);//m_editZoomValue.SetWindowText(strValue);
			}

			m_nCurSelCol = _pStData->m_nSelCol;
			m_nCurSelRow = _pStData->m_nSelRow;

			m_chType = _pStData->m_chType;

			//08.22================================================================
			//���� �ְ� �� �α� �����ֱ� ����
			m_bChkRevised	= _pStData->m_bRevised;
			m_bChkLog		= _pStData->m_bLog;

			UpdateData(FALSE);			
			//07.01.20 by LYH �߰�
			if(oldChType != _pStData->m_chType)
				UpdateBtNCbValue();
			//

			//=====================================================================

			//08.22================================================================
			//��Ʈ ������ �����Ѵ�.
			/*CString strLineType;
			for(int i = 0 ; i < m_cmbChartType.GetCount(); i++)
			{
				m_cmbChartType.GetLBText(i, strLineType);
				if( strLineType.CompareNoCase(_pStData->m_strChartLineType) == 0)
				{
					m_cmbChartType.SetCurSel(i);
					break;
				}
			}*/
			//=====================================================================			

			if(m_chType==TICK_DATA_CHART||m_chType==SEC_DATA_CHART||m_chType==MIN_DATA_CHART||m_chType==DAY_DATA_CHART)
			{
				long lNumber = 0;
				//BOOL bUpdateUnitPos = FALSE;
				// �߰� 2006.01.19 by LYH 
				BOOL bUpdateUnitPos = FALSE;

				if(m_chType == TICK_DATA_CHART)	//ƽ(0)
				{
					lNumber = atol(_pStData->m_strTick);
					m_nTick = lNumber;
					for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
					{
						if(m_lValueOfTickButton[nPos]==lNumber)
						{
							m_nUnitPos = nPos;
							bUpdateUnitPos = TRUE;
							break;
						}
					}
				}
				else if(m_chType == SEC_DATA_CHART)//��
				{					
					lNumber = atol(_pStData->m_strSec);
					m_nSec	= lNumber;

					if(300 == lNumber)
					{
						m_nUnitPos = 0;
						bUpdateUnitPos = TRUE;
					}
					else
					{
						for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
						{
							if(m_lValueOfSecButton[nPos]==lNumber)
							{
								m_nUnitPos = nPos;
								bUpdateUnitPos = TRUE;
								break;
							}
						}
					}
				}
				else if(m_chType == MIN_DATA_CHART)//��(1)
				{					
					lNumber = atol(_pStData->m_strMin);
					m_nMin	= lNumber;

					if(300 == lNumber)
					{
						m_nUnitPos = 0;
						bUpdateUnitPos = TRUE;
					}
					else
					{
						for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
						{
							if(m_lValueOfMinButton[nPos]==lNumber)
							{
								m_nUnitPos = nPos;
								bUpdateUnitPos = TRUE;
								break;
							}
						}
					}
				}
				else if(m_chType == DAY_DATA_CHART)//��(1)
				{					
					lNumber = atol(_pStData->m_strDay);
					m_nDay	= lNumber;
					for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
					{
						if(m_lValueOfButton[nPos]==lNumber)
						{
							m_nUnitPos = nPos;
							bUpdateUnitPos = TRUE;
							break;
						}
					}
//					CString sTemp;
//					sTemp.Format("InputBar : Recv__m_nDay %d", m_nDay);
//					OutputDebugString(sTemp);
				}

				if(!bUpdateUnitPos)
				{
					m_nUnitPos = -1;
				}
				//m_nUnitPos = -1; 
				SetType(FALSE);
				if(m_chType==TICK_DATA_CHART)	//ƽ(0)
				{				
					//m_nUnitPos = 0;
					_pStData->m_strTick.TrimLeft('0');
					if(m_editPeriod.GetSafeHwnd())
						m_editPeriod.SetWindowText(_pStData->m_strTick);
				}
				else if(m_chType==SEC_DATA_CHART)				//��
				{				
					//m_nUnitPos = 1;
					_pStData->m_strSec.TrimLeft('0');
					if(m_editPeriod.GetSafeHwnd())
						m_editPeriod.SetWindowText(_pStData->m_strSec);
				}
				else if(m_chType==MIN_DATA_CHART)				//��(1)
				{				
					//m_nUnitPos = 1;
					_pStData->m_strMin.TrimLeft('0');
					if(m_editPeriod.GetSafeHwnd())
						m_editPeriod.SetWindowText(_pStData->m_strMin);
				}
				else if(m_chType==DAY_DATA_CHART)
				{				
					//m_nUnitPos = 2;
					_pStData->m_strDay.TrimLeft('0');
					if(m_editPeriod.GetSafeHwnd())
						m_editPeriod.SetWindowText(_pStData->m_strDay);
				}
				else
				{
					if(m_editPeriod.GetSafeHwnd())
						m_editPeriod.SetWindowText("1");
				}
			}
			else if (m_chType == HOUR_DATA_CHART)		// ��
			{
				long lNumber = 0;
				CString strTemp;
				BOOL bUpdateUnitPos = FALSE;

				lNumber = atol(_pStData->m_strHour);
				//				lNumber = lNumber / 60;
				m_nHour = lNumber;

				for(int nPos=0;nPos < 3;nPos++)
				{
					if(m_lValueOfHourButton[nPos]==lNumber)
					{ 
						m_nUnitPos = nPos;
						bUpdateUnitPos = TRUE;
						break;
					}
				}

				if(!bUpdateUnitPos)
				{
					m_nUnitPos = -1;
				}
				SetType(FALSE);
				_pStData->m_strHour.TrimLeft('0');
				strTemp.Format("%d",lNumber);
				m_editPeriod.SetWindowText(strTemp);
			}
			else	////
			{
				SetType(FALSE);				
				m_editPeriod.SetWindowText("1");
			}			
			int nLength = _pStData->m_strCode.GetLength();
#ifdef _DEBUG
			OutputDebugString("InputBar�� SetMarket Comment");
#endif

			if(_pStData->m_strCode.GetLength())
			{
				if(_pStData->m_strCode == "NOCHANGE")
				{					
					m_strOldCode = GetCurrentCode();
					SetCurrentCode( m_strCodeFromOutside );	
					SetCurCodeNameToControl(GetCurrentCode());

					m_bSettingInitData = FALSE;				

					//�θ𿡼� ȣ�� ��ȸ�� ������.			
					//===============================================================
					if(m_bCanRequestHogaTR) //ȣ���� ��ȸ�ص� �Ǵ� ���¿����� ��ȸ�� �Ѵ�.
						theStdParent->Request_StockHogaData();
					//===============================================================

					return 1l;				
				}
				else
				{
					CWnd* pTrmChartView = theStdParent->GetParent();
					int nMarketType = theStdParent->GetMarketTypeFromItemCode(_pStData->m_strCode);
// KB ������ ���� - ojtaso (20080901)
//					pTrmChartView->SendMessage(WM_USER+101, nMarketType, (LPARAM)(LPCSTR)_pStData->m_strCode);
//@��������			CString strType = m_pDataManager->GetCodeInfo("", 2, _pStData->m_strCode);
					ChangeMarket(nMarketType, FALSE, FALSE);
				}

				m_strOldCode = GetCurrentCode();
				SetCurrentCode(_pStData->m_strCode);
				SetCurCodeNameToControl(_pStData->m_strCode);
				
				// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)
				m_bReceiveDataSetCode = TRUE;

				CString strCodeInputCode = m_pCodeInput->GetCode();
				
				strCodeInputCode.TrimLeft(); strCodeInputCode.TrimRight();
				if(_pStData->m_strCode.GetLength() && strCodeInputCode != _pStData->m_strCode)
				{
					BOOL bSendShare = (BOOL)wParam;
					if(!bSendShare)
					{
						theStdParent->DoShareCode(_pStData->m_strCode);
						if(g_pPtUtilMng) g_pPtUtilMng->AddToHistory(theStdParent->m_hPlatform, _pStData->m_strCode);
					}

					if(((CChartMng*)m_pChartMng)->GetSendShareFlag())
					{
						SetShareCodeChanged(FALSE);
					}
					else
					{
						SetShareCodeChanged(TRUE);						
					}
					m_pCodeInput->SetCode(_pStData->m_strCode); //@solomon
					DoSelchangeCodeInput();
				}

				m_bReceiveDataSetCode = FALSE;
			}
			else
			{
				// chart ocx�� ���� �Ǿ����� �� ��Ʈ���� ������ �ڵ尡 �޸����̱� ���ؼ�..				
				SetCurCodeNameToControl(_T(""));

				//{{ó�� �㶧 ���ڵ� ���� ����
				if(m_pChartMng)
				{
					if(((CChartMng*)m_pChartMng)->m_strStatic_ShareCode.GetLength()<1)
					{
						//m_pCtrlCodeInput->SetDataText(_T(""));
					}
				}
				//}}
				//��ȸ�� �� �ȵǸ� �Ʒ� 2������ �����Ѵ�.================================
				m_strOldCode = GetCurrentCode(); //2005. 08. 26
				SetCurrentCode(_T("")); //2005. 08. 26
				//=======================================================================
			}
			
			m_bSettingInitData = FALSE;	
			

			//�θ𿡼� ȣ�� ��ȸ�� ������.			
			//===============================================================
			if(m_bCanRequestHogaTR) //ȣ���� ��ȸ�ص� �Ǵ� ���¿����� ��ȸ�� �Ѵ�.
				theStdParent->Request_StockHogaData();
			//===============================================================
		}
		SetShareCodeChanged(FALSE);
		break;
/*	case UMSG_SETVIEWENDTIME:
		{
			//if(!m_chkFixPeriod.GetCheck()&&(m_chType=='2'||m_chType=='3'||m_chType=='4'))
			if((m_chType=='2'||m_chType=='3'||m_chType=='4'))
			{
				long lEndDate = wParam;
				SYSTEMTIME tm;
				::ZeroMemory(&tm,sizeof(tm));
				if(m_chType=='2'||m_chType=='3')
				{
					tm.wYear = lEndDate/10000;
					tm.wMonth = lEndDate%10000/100;
					tm.wDay = lEndDate%100;
				}
				else if(m_chType=='4')
				{
					tm.wYear = lEndDate/10000;
					tm.wMonth = lEndDate%10000/100;
					tm.wDay = 1;
				}
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//				m_datePicker.SetTime(&tm);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
			}
		}
		break;
	case UMSG_GETENDDATEFROMINPUTBAR:
		{
			SYSTEMTIME tm;					// ƽ(0)/��(1)/��(2)/��(3)/��(4)
			//if(m_chkFixPeriod.GetCheck()&&(m_chType=='2'||m_chType=='3'||m_chType=='4'))
			if((m_chType=='2'||m_chType=='3'||m_chType=='4'))
			{
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//				m_datePicker.GetTime(&tm);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
				return (long)(tm.wYear*10000+tm.wMonth*100+tm.wDay);
			}
			return 0L;
		}
		break;*/
	case UMSG_GETTYPEFROMINPUTBAR:
		{
			return (long)GetType();
		}
		break;
	case UMSG_GETTYPENUMFROMINPUTBAR:
		{
			return GetTypeNumber();
		}
		break;
	case UMSG_INPUTINSHAREDCODELIST:
		{
			CString strCode = (LPCTSTR)wParam;
			CString strName = GetCodeNameAndGubun(strCode);
			if(strCode.GetLength()==6)
			{
				AddHistoryInfoListJongmok(strCode,strName);
			}
			else if(strCode.GetLength()==4)
			{
				AddHistoryInfoListUpjong(strCode,strName);
			}			
		}
		break;
	case UMSG_CHART_INIT_FROMOCX:	// Quote
		{	
			//=====================================================================
			// ��Ʈ�� �ε�(���Ͽ��� �б�) �ǰ� �ִ� �� �� �ʱ�ȭ �޼����� 
			// ���� �����ϰ� Return �Ѵ�.
			//=====================================================================
			if(m_pChartMng)
				if( ((CChartMng*)m_pChartMng)->m_bLastEnvUpdated == FALSE )
					return 0;
			//=====================================================================

			//�����߰� ��忡�� ������ �űԷ� �߰����� �ʰ� 
			//�ʱ�ȭ�� �ǵ��� �ϱ� ���ؼ�..05.11.24
			((CChartMng*)m_pChartMng)->SetBInitFromExtToolBar(TRUE);

			if(wParam==1)
			{
				for(int n = 0;n<8;n++)//ƽ �� �� �� �� �� 10�� 30�� 
				{
					m_lCntInView[n] = 120;
				}
				OnChkday();	
			}
			else if(wParam==2) //20090910 SJ_KIM �̴�TICK��Ʈ �ʱ�ȭ�� ����ȸ �����ʵ��� ����
			{
//@Solomon:091123SM070--> �ʱ�ȭ�� Tick���� �����Ѵ�.
//			
//				CString sType;
//				sType.Format("%c", GetType());
//				m_lCntInView[atoi(sType)] = 120;
				m_lCntInView[0] = 120;
				m_lCntInView[1] = 120;

//				if(sType.CompareNoCase("0") == 0)
//					OnChktick();
//				if(sType.CompareNoCase("1") == 0)
//					OnChkminute();

					//OnChktick();
					OnChkminute();
//@Solomon<--
			}
			else
			{
				CString sType;
				sType.Format("%c", GetType());
				m_lCntInView[atoi(sType)] = 120;
				
				
				CString strRetryCode;
				strRetryCode = GetCurrentCode();			
				SetCurrentCode(_T(""));

				if (strRetryCode.GetLength()==UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
				{
					CString strCodeInputCode = m_pCodeInput->GetCode();//m_ctrlCodeInput.GetDataText();
					if(strRetryCode.GetLength() && strCodeInputCode != strRetryCode)
					{
						SetShareCodeChanged(TRUE);
						//m_ctrlCodeInput.SetDataText(strRetryCode);
						m_pCodeInput->SetCode(strRetryCode);
						DoSelchangeCodeInput();
					}
				}
				else
				{
					CString strCodeInputCode = m_pCodeInput->GetCode();
					if(strRetryCode.GetLength() && strCodeInputCode != strRetryCode)
					{
						SetShareCodeChanged(TRUE);
//						m_ctrlCodeInput.SetDataText(strRetryCode);
						m_pCodeInput->SetCode(strRetryCode);
						DoSelchangeCodeInput();
					}
				}
			}
		}
		break;
	case UMSG_GETZOOMVALUE:
		{	
			//05.09.28
			//if(wParam<TICK_DATA_CHART || wParam>YEAR_DATA_CHART) return 0L;
//			if(wParam<TICK_DATA_CHART || wParam>SEC30_DATA_CHART) return 0L;
			if(wParam<TICK_DATA_CHART || wParam>SEC_DATA_CHART) return 0L;
			if(m_bChkPeriod)
				return m_nFixZoomValue;
			return m_lCntInView[wParam-'0'];			
		}
		break;
	case UMSG_SETTYPE:
		{
			if(wParam<0||wParam>7) return 0L;
			m_chType = wParam;
		}
		break;
	case UMSG_GETREVISED:
		{
			UpdateData(TRUE);
			return m_bChkRevised;
		}
		break;
	case UMSG_UPDATEREVISED:
		{
			m_bChkRevised = wParam;	
			UpdateData(FALSE);
			if(m_pChartMng->GetSafeHwnd())
				m_pChartMng->SendMessage(UMSG_UPDATEREVISED,m_bChkRevised,1);
		}
		break;
	case UMSG_GETLOG:
		{
			UpdateData(TRUE);
			return m_bChkLog;
		}
		break;
	case UMSG_SETLOG:
		{
			m_bChkLog = (long)wParam > 0 ? TRUE : FALSE;
			UpdateData(FALSE);
		}
		break;
	case UMSG_USE_LOG:
		{
			m_bChkLog = wParam;
			UpdateData(FALSE);
			if(m_pChartMng->GetSafeHwnd())
				m_pChartMng->SendMessage(UMSG_USE_LOG,m_bChkLog,1);
		}
		break;
	//05.05.30 add by sy, nam =========================================
	//��Ʈ ����Ʈ �ǿ��� ���� ����Ǹ� Input Bar�� Combo�� ���õ� ���� ��ȭ ��Ų��. 
	case UMSG_CHANGE_CHARTVALUE:
		{	
			CString sText;
			int nCnt = m_cmbChartType.GetCount();
			for(int i = 0; i < nCnt; i++)
			{
				m_cmbChartType.GetLBText(i, sText);
				if(sText.CompareNoCase((LPCTSTR)wParam) == 0)
				{
					m_cmbChartType.SetCurSel(i);
					break;
				}
			}						
		}
		break;
	//-----------------------------------------------------------------------------
	// Author		: Sang Yun, Nam	Date :2005/6/01
	// Return void : 
	// Param  WPARAM wp : ��ȭ �ڵ���ǲ�� ���ҽ� ID
	// Param  LPARAM lp : �����ڵ� 
	// Comments			: WMU_SET_KEYCODE �޽����� �����ڵ尡 ����Ǿ����� 
	//					  �ڵ���ǲ���� ���� �޴´�.
	//					"include\UserMsg.h" ���Ͽ� ���ǵǾ� �ִ�.
	//-----------------------------------------------------------------------------
//	case WMU_SET_KEYCODE:
//		SetKeyCodeChange(wParam, lParam);
//		break;
	//=================================================================
	//-----------------------------------------------------------------------------
	// Author		: Sang Yun, Nam	Date :2005/8/16
	// Return void	: 	
	// Comments		: ���� ������ �ްڴٰ� �˸���
	//				
	//				
	//-----------------------------------------------------------------------------
//	case WMU_GET_SHARE_STATE:
//		return 1L;
//		break;
	//=================================================================	
//	case WMU_CHANGE_TAB_COLOR:
//		{
//			ChangeButtonColor();			
//		}
//		break;
	case USMG_NOTIFY_REVISED: //�����ְ� ��뿩�θ� ��Ʈ�ѿ� �ݿ��ϵ��� �Ѵ�.
		{			
			SetCheckRevised(wParam);
		}
		break;
	case UMSG_ENABLE_SENDHOGATR:
		{
			BOOL bCanRequest = (BOOL)wParam;
			SetStopRequestHogaTR(bCanRequest);
		}
		break;
	case UMSG_SETTEXT_CODE_INPUTBAR:
		{
			CString strCode;
			strCode.Format("%s", (LPTSTR)(LPCTSTR)lParam);			
			if (strCode.GetLength()==UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
			{
				CString strCodeInputCode = m_pCodeInput->GetCode();//m_ctrlCodeInput.GetDataText();
				if(strCode.GetLength() && strCodeInputCode != strCode)
				{
					SetShareCodeChanged(TRUE);
//					m_ctrlCodeInput.SetDataText(strCode.Mid(1));

//					m_ctrlCodeInput.SetDataText(strCode);
					m_pCodeInput->SetCode(strCode); //@solomon
					DoSelchangeCodeInput();
				}
			}
			else
			{
				if(wParam==9999)
				{
					m_strCode="";
					//m_pCodeInput->SetCode(""); //@solomon
				}
				else
				{
					CString strCodeInputCode = m_pCodeInput->GetCode(); //m_ctrlCodeInput.GetDataText();
					if(strCode.GetLength() && strCodeInputCode != strCode)
					{
						SetShareCodeChanged(TRUE);
						//	m_ctrlCodeInput.SetDataText(strCode);
						m_pCodeInput->SetCode(strCode); //@solomon
						DoSelchangeCodeInput();
					}
				}
			}
			//
		}	
		break;
	case UMSG_END_REQUEST_CHARTDATA:
		{
			SetUseUserInputPeriod(FALSE);

			if(m_chType == MIN_DATA_CHART || m_chType == SEC_DATA_CHART || m_chType == TICK_DATA_CHART)
				theStdParent->EnableQueryOpt(TRUE);
			else
				theStdParent->EnableQueryOpt(FALSE);

			if(m_chType == TICK_DATA_CHART || 
				m_chType == SEC_DATA_CHART || 
				m_chType == WEEK_DATA_CHART || 
				m_chType == MONTH_DATA_CHART||
				m_chType == YEAR_DATA_CHART)
				theStdParent->EnableAccount(FALSE);
			else if(m_chType == DAY_DATA_CHART && m_nDay > 1)
				theStdParent->EnableAccount(FALSE);
			else
				theStdParent->EnableAccount(TRUE);
		}
		break;
	case UMSG_ENABLE_PLAYBTN:
		{
			m_btnHandleReplay.EnableWindow(TRUE);
		}
		break;
	case UMSG_EXTINPUT_CHANGEDATE:
		{
			if(wParam == 1)
			{
				m_bChkPeriod = lParam;
				CString strZoomValue;
				m_editZoomValue.GetWindowText(strZoomValue);
				if(m_bChkPeriod)
					m_nFixZoomValue = atoi(strZoomValue);
			}
			else
			{
				CString strEndDateFromInputBar;
				strEndDateFromInputBar.Format("%d",lParam);
				
				((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetEndDateRequested(strEndDateFromInputBar);
				((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetEndDate(strEndDateFromInputBar);

				SetType();
			}
		}
		break;
	case UMSG_EDIT_FOCUS:
		{
			if(m_nMarketType != CMasterTypeInfo::FOREIGN_CHART)
				m_pCodeInput->SetFocus(); //m_ctrlCodeInput.SetFocus();

		}
		break;
	case UMSG_GETSAVEJIPYOONEFILE:
		{
			return (long)(m_bChkSaveJipyoOneFile);
		}
		break;
	case UMSG_GETLOADSAVECODE:
		{
/*			if (theStdParent->m_nChartType == SINGLE_CHART &&
				theStdParent->m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			{
				return TRUE;
			}
			else
*/				return (long)(m_bChkLoadSaveCode);
		}
		break;
	case UMSG_SETLOADSAVECODE:
		{
			m_bChkLoadSaveCode = !m_bChkLoadSaveCode;
		}
		break;
	}

	//@solomon ����
// 	{
// 		if(message == RMSG_EVENT_CODEINPUT)
// 		{
// 			LPST_CTRLEVENT stEvent = (LPST_CTRLEVENT)lParam;
// 			if(strcmp("LengthFull", stEvent->lpszEventName) == 0 || strcmp("SelChanged", stEvent->lpszEventName) == 0)
// 			{
// 				SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_ctrlCodeInput.GetDataText());
// 			}
// 		}
// 	}

	if(message==RMSG_SETINITIALCHARTDATA)
		m_bInitChartData = (BOOL)lParam;

	return CRscDlg::WindowProc(message, wParam, lParam);
}



void CInputBarDlg::ChangeButtonColor() 
{
//	CClrButton m_btnFontSet;
//	m_baseInfo.GetButtonColor(m_btnFontSet.m_arrColor);
//	m_btnFontSet.ChangeSetColor();
}

void CInputBarDlg::SetType(BOOL bSendChartMng/*=TRUE*/)
{
	theStdParent->SendMessage(UMSG_SYNC_TOOLBAR, 0, 50000);//KHD : �ֱ⺯��� ���� �ʱ�ȭ 

	// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
	CString sKey = _T("");

	m_chkDay.SetCheck(0);
	m_chkWeek.SetCheck(0);
	m_chkMonth.SetCheck(0);
	m_chkYear.SetCheck(0);
	m_chkMinute.SetCheck(0);
	m_chkSec.SetCheck(0);
	m_chkTick.SetCheck(0);
	m_chkFirst.SetCheck(0);
	m_chkSecond.SetCheck(0);
	m_chkThird.SetCheck(0);
	m_chkFourth.SetCheck(0);
//	m_chkFifth.SetCheck(0);
//	m_chkSixth.SetCheck(0);

	switch(m_chType)
	{
	case WEEK_DATA_CHART:
		m_chkWeek.SetCheck(1);
		m_editPeriod.EnableWindow(FALSE);
		Enable_PeriodControl(FALSE);
		break;
	case MONTH_DATA_CHART:
		m_chkMonth.SetCheck(1);
		m_editPeriod.EnableWindow(FALSE);
		Enable_PeriodControl(FALSE);
		break;
	case YEAR_DATA_CHART:
		m_chkYear.SetCheck(1);
		m_editPeriod.EnableWindow(FALSE);
		Enable_PeriodControl(FALSE);
		break;
	default:
		Enable_PeriodControl(TRUE);

		switch(m_chType)
		{
			case TICK_DATA_CHART:	
				m_chkTick.SetCheck(1);
				m_editPeriod.EnableWindow(TRUE);
				break;
			case SEC_DATA_CHART:	
				m_chkSec.SetCheck(1);
				m_editPeriod.EnableWindow(TRUE);
				break;
			case MIN_DATA_CHART:	
				m_chkMinute.SetCheck(1);
				if( m_nMarketType == CMasterTypeInfo::FOREIGN_CHART )
				{
					m_editPeriod.SetWindowText("1");
					m_editPeriod.EnableWindow(FALSE);
					Enable_PeriodControl(FALSE);
				}
				else
					m_editPeriod.EnableWindow(TRUE);
				break;
			case DAY_DATA_CHART:
				m_chkDay.SetCheck(1);	
				m_editPeriod.EnableWindow(TRUE);
				break;
		}

		switch(m_nUnitPos)
		{
		case 0:
			m_chkFirst.SetCheck(1);
			break;
		case 1:
			m_chkSecond.SetCheck(1);
			break;
		case 2:
			m_chkThird.SetCheck(1);
			break;
		case 3:
			m_chkFourth.SetCheck(1);
			break;
/*		case 4:
			m_chkFifth.SetCheck(1);
			break;
		case 5:
			m_chkSixth.SetCheck(1);
			break;
*/		default:
			m_nUnitPos = 0;
			break;
		}
		break;
	}

	//@Solomon������ �Ϻ��� 1���� ����.
	if (m_chType==DAY_DATA_CHART) Enable_PeriodControl(FALSE);

	m_chkDay.Invalidate();
	m_chkWeek.Invalidate();
	m_chkMonth.Invalidate();
	m_chkMinute.Invalidate();
	m_chkSec.Invalidate();
	m_chkTick.Invalidate();
	m_chkYear.Invalidate();
	m_chkFirst.Invalidate();
	m_chkSecond.Invalidate();
	m_chkThird.Invalidate();
	m_chkFourth.Invalidate();
//	m_chkFifth.Invalidate();
//	m_chkSixth.Invalidate();

	//>>JS.Kim_20100930. solomon. �ֱⰡ �ٲ�� �ش� �ֱ��� ��ũ�Ѱ����� ����
	CString strValue;
	if(m_bChkPeriod)
		strValue.Format("%d",m_nFixZoomValue);
	else
		strValue.Format("%d",m_lCntInView[m_chType-'0']);

	//0001399: ��Ʈ�� �׸��� ���������� �׸� �Ŀ� �ϴ� ������Ʈ���� ������ŭ ���������� �ٽ� �׸��°� �����ϴ�.
	SetEditZoomValue(strValue, FALSE, FALSE);//m_editZoomValue.SetWindowText(strValue);
	//<<

	if(bSendChartMng)
	{
		if(m_chType < WEEK_DATA_CHART || m_chType == SEC_DATA_CHART)
		{
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			{
				CString strValue;				
				m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
				int nValue = atoi(strValue);
				if(nValue <= 0) 
				{
					strValue = _T("1");
					m_editPeriod.SetWindowText(strValue);
				}

				//--> 0016313: [7111] N�� �Է��Ͽ� ��ȸ�� 30�� �̻��� ��ȸ���� �ʴ� ����
				if (!(m_chType == SEC_DATA_CHART && nValue > 30))
				{
					//--> 0016490: [7111] ������Ʈ���� �ʺ��ֱ� ������ 60�� ����� �����ǵ��� ����
					if (m_chType == SEC_DATA_CHART && nValue > 0)
					{
						int nSecValue = 60%nValue;
						if (nSecValue == 0)
							m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, nValue);//����� ���� �Ⱓ
						else
						{
							AfxMessageBox("��ȸ������ �ʺ��� �ƴմϴ�.");
							m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, 30);
						}
					}
					else
						m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, nValue);//����� ���� �Ⱓ
					//<--
				}
				else
				{
					AfxMessageBox("�ʺ� �����ʹ� 30�ʺ������� ��ȸ�� �����մϴ�.");
					m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, 30);
				}
				//<--
			}
		}
		else if (HOUR_DATA_CHART == m_chType)		// ��
		{
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			{
				CString strValue;				
				m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
				int nValue = atoi(strValue);
				if(nValue <= 0) 
				{
					strValue = _T("1");
					m_editPeriod.SetWindowText(strValue);
					nValue = 1;
				}

				m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, nValue);//����� ���� �Ⱓ
			}
		}
		else // �� / �� / ��
		{
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
				m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType);
		}		
	}
	EnableWindow();
}

void CInputBarDlg::OnChkyear() 
{
	// TODO: Add your control notification handler code here
	m_chType = YEAR_DATA_CHART;
	m_nUnitPos = -1;
	//Enable_PeriodControl(FALSE);
	//m_editPeriod.SetWindowText("1");

	theStdParent->EnableQueryOpt(FALSE);
	theStdParent->EnableAccount(FALSE);
	SetType();
}

void CInputBarDlg::OnChkmonth() 
{
	// TODO: Add your control notification handler code here
	m_chType = MONTH_DATA_CHART;
	m_nUnitPos = -1;
	//Enable_PeriodControl(FALSE);
	//m_editPeriod.SetWindowText("1");

	theStdParent->EnableQueryOpt(FALSE);
	theStdParent->EnableAccount(FALSE);
	SetType();
}

void CInputBarDlg::OnChkweek() 
{
	// TODO: Add your control notification handler code here
	m_chType = WEEK_DATA_CHART;
	m_nUnitPos = -1;
	//Enable_PeriodControl(FALSE);
	//m_editPeriod.SetWindowText("1");

	theStdParent->EnableQueryOpt(FALSE);
	theStdParent->EnableAccount(FALSE);
	SetType();
}

void CInputBarDlg::OnChktick() 
{	

	//������ ���� ������ ���°� ƽ�� �ƴϸ����� �Է°� ����===
	//05.09.12
	if(m_chType != TICK_DATA_CHART)
		SetUseUserInputPeriod(FALSE);
	//==========================================================

	/* ƽ(0)/��(1)/��(2)/��(3)/��(4)*/	
	m_chType = TICK_DATA_CHART;	
	//m_nUnitPos = 0;

	UpdateBtNCbValue();	// �ֱ��ư Set

	//================================================================================
	// 2005. 09. 08
	// ����ڰ� �Է��� ������ �����ϵ��� �ش޶� ��ȭ �䱸
	//================================================================================	
	CString sTemp;
	if(!GetUseUserInputPeriod())
	{
		if(m_pChartMng->GetSafeHwnd())
			m_nTick = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 0);	

		
		sTemp.Format("%d", m_nTick); 
		m_editPeriod.SetWindowText(sTemp);
	}
	else
	{
		m_editPeriod.GetWindowText(sTemp);
		m_nTick = atoi(sTemp);
	}
	//---------------------------------------------------	
	//Enable_PeriodControl(TRUE);
	//---------------------------------------------------

	if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		theStdParent->EnableQueryOpt(FALSE);
	else
		theStdParent->EnableQueryOpt(TRUE);
	theStdParent->EnableAccount(FALSE);
	SetType();

	SetUseUserInputPeriod(FALSE);
}

void CInputBarDlg::OnChksec() 
{
	//�������� �� �����Ͱ� �ƴϾ����� ����ڰ� �Է��� �����Ѵ�.========
	//05.09.12
	if(m_chType != SEC_DATA_CHART)
		SetUseUserInputPeriod(FALSE);
	//==========================================================

	m_chType = SEC_DATA_CHART;

	UpdateBtNCbValue();	// �ֱ��ư Set

	//m_nUnitPos = 1;

	//================================================================================
	// 2005. 09. 08
	// ����ڰ� �Է��� ������ �����ϵ��� �ش޶� ��ȭ �䱸
	//================================================================================	
	CString sTemp;
	if(!GetUseUserInputPeriod())
	{
		if(m_pChartMng->GetSafeHwnd())
			m_nSec = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 6);	

		sTemp.Format("%d", m_nSec);

		if(m_nSec >= 300)
			sTemp = "0";

		m_editPeriod.SetWindowText(sTemp);
	}
	else
	{
		m_editPeriod.GetWindowText(sTemp);
		m_nSec = atoi(sTemp);
	}
	//=======================================================================================

	if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		theStdParent->EnableQueryOpt(FALSE);
	else
		theStdParent->EnableQueryOpt(TRUE);

	theStdParent->EnableAccount(FALSE);

	SetType();

	SetUseUserInputPeriod(FALSE);
}

void CInputBarDlg::OnChkminute() 
{
	//�������� �� �����Ͱ� �ƴϾ����� ����ڰ� �Է��� �����Ѵ�.========
	//05.09.12
	if(m_chType != MIN_DATA_CHART)
		SetUseUserInputPeriod(FALSE);
	//==========================================================

	m_chType = MIN_DATA_CHART;

	UpdateBtNCbValue();	// �ֱ��ư Set

	//m_nUnitPos = 1;

	//================================================================================
	// 2005. 09. 08
	// ����ڰ� �Է��� ������ �����ϵ��� �ش޶� ��ȭ �䱸
	//================================================================================	
	CString sTemp;
	if(!GetUseUserInputPeriod())
	{
		if(m_pChartMng->GetSafeHwnd())
			m_nMin = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 1);	

		sTemp.Format("%d", m_nMin);

		if(m_nMin >= 300)
			sTemp = "0";

		m_editPeriod.SetWindowText(sTemp);
	}
	else
	{
		m_editPeriod.GetWindowText(sTemp);
		m_nMin = atoi(sTemp);
	}
	//=======================================================================================

	if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		theStdParent->EnableQueryOpt(FALSE);
	else
		theStdParent->EnableQueryOpt(TRUE);
	theStdParent->EnableAccount(TRUE);
	SetType();

	SetUseUserInputPeriod(FALSE);
}

void CInputBarDlg::SetChkType(char chType)
{
	OnChkday();
}
void CInputBarDlg::OnChkday() 
{		

	//�������� �� �����Ͱ� �ƴϾ��ٸ� ����� �Է°� ����========
	//05.09.12
	if(m_chType != DAY_DATA_CHART)
		SetUseUserInputPeriod(FALSE);
	//===========================================================

	
	m_chType = DAY_DATA_CHART;
//	m_nUnitPos = 2;	

	UpdateBtNCbValue();	// �ֱ��ư Set

	//================================================================================
	// 2005. 09. 08
	// ����ڰ� �Է��� ������ �����ϵ��� �ش޶� ��ȭ �䱸
	//================================================================================	
	CString sTemp;
	if(!GetUseUserInputPeriod())
	{
		if(m_pChartMng->GetSafeHwnd())
			m_nDay = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 2);
		
		
		sTemp.Format("%d", m_nDay);
		m_editPeriod.SetWindowText(sTemp);	
	}
	else
	{
		m_editPeriod.GetWindowText(sTemp);
		m_nDay = atoi(sTemp);
	}
	//================================================================================

	theStdParent->EnableQueryOpt(FALSE);
	theStdParent->EnableAccount(TRUE);
	SetType();

	SetUseUserInputPeriod(FALSE);

}

void CInputBarDlg::OnDeltaposSpinzoomvalue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	CString strZoomValue, strTotValue;
	long lZoomValue = 0;
	long lTotValue = 0;
	m_editZoomValue.GetWindowText(strZoomValue);
	// ���������� Total Count�� ��� : �������� - ojtaso (20070704)
//	m_editCntOfDatAll.GetWindowText(strTotValue);
	GetDlgItemText(IDC_STC_TOTALCNT, strTotValue);

	if(!strZoomValue.IsEmpty())
	{
		lZoomValue = atol(strZoomValue);
		lTotValue  = atol(strTotValue);		

		if(pNMUpDown->iDelta>0)
		{
			if(lZoomValue<=0) return;
			lZoomValue--;
		}
		else
		{
			lZoomValue++;
		}
		strZoomValue.Format("%d",lZoomValue);	

		lZoomValue = atol(strZoomValue);
		lTotValue  = atol(strTotValue);	
		
		if(lZoomValue > lTotValue) return;

		
		SetEditZoomValue(strZoomValue);//m_editZoomValue.SetWindowText(strZoomValue);
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_SETZOOMVALUE,atol(strZoomValue));
	}	
	*pResult = 0;
}

void CInputBarDlg::LoadIniValue()
{
//	CString strUserDir = theStdParent->m_strConfig;
//
//	char szTemp[8];
//	for(int nPos=0;nPos<8;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lCntInView[nPos] = ::GetPrivateProfileInt("CntInView",szTemp,m_lCntInView[nPos],strUserDir);
//	}

	long lDefaultValueOfButton[CNTOFMAXBUTTONS] = { 1, 2, 3, 4, 5, 6};		// ��
	long lDefaultValueOfMinButton[CNTOFMAXBUTTONS] = {1, 3, 5, 10, 20, 30};		// ��
	long lDefaultValueOfSecButton[CNTOFMAXBUTTONS] = { 1, 3, 5, 10, 20, 30};		// ��
	long lDefaultValueOfTickButton[CNTOFMAXBUTTONS] = { 1, 3, 5, 10, 20, 30};		// ƽ
	long lValueOfCombo[CNTOFMAXCBITEMS] = {1, 2, 3, 4, 5, 10, 15, 20, 30, 60, 120};
	char szTemp[8];
	CString strUserDir = m_pStdDialog->m_strConfig;
	int nPos = 0;
	for( nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfButton[nPos] = ::GetPrivateProfileInt("Day Button",szTemp,lDefaultValueOfButton[nPos],strUserDir);

		VerifySettingData(m_lValueOfButton[nPos]);
	}
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfMinButton[nPos] = ::GetPrivateProfileInt("Min Button",szTemp,lDefaultValueOfMinButton[nPos],strUserDir);

		VerifySettingData(m_lValueOfMinButton[nPos]);
	}
 	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
 	{
 		sprintf(szTemp,"%d",nPos);
 		m_lValueOfSecButton[nPos] = ::GetPrivateProfileInt("Sec Button",szTemp,lDefaultValueOfSecButton[nPos],strUserDir);
		if (m_lValueOfSecButton[nPos] > 30 || m_lValueOfSecButton[nPos] <= 0)
			m_lValueOfSecButton[nPos] = lDefaultValueOfSecButton[nPos];
 
		VerifySettingData(m_lValueOfSecButton[nPos]);
 	}
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ƽ
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfTickButton[nPos] = ::GetPrivateProfileInt("Tick Button",szTemp,lDefaultValueOfTickButton[nPos],strUserDir);

		VerifySettingData(m_lValueOfTickButton[nPos]);
	}

	for(nPos=0;nPos<CNTOFMAXCBITEMS;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfCombo[nPos] = lValueOfCombo[nPos];
//		m_lValueOfCombo[nPos] = ::GetPrivateProfileInt("Combo",szTemp,lValueOfCombo[nPos],strUserDir);
	}
	for(nPos=0;nPos<8;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		m_lCntInView[nPos] = ::GetPrivateProfileInt("CntInView",szTemp,m_lCntInView[nPos],strUserDir);
	}	

//	// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
//	CString strData;
//	strData.GetBuffer(20);
//	::GetPrivateProfileString(_T("InputBar"), _T("SavedCode"), _T("001;DJI@DJC"), (LPSTR)(LPCTSTR)strData, 20, strUserDir);
//	strData.ReleaseBuffer();
//	int nIndex = strData.Find(_T(";"));
//	m_strUpjongCode = strData.Left(nIndex); if(m_strUpjongCode[0]=='.')m_strUpjongCode="001";
//	strData.Delete(0, nIndex + 1);
//	m_strForeignCode = strData; if(m_strForeignCode[0]=='.') m_strForeignCode="DJI@DJC";
}

void CInputBarDlg::VerifySettingData(LONG& lData)
{
	if(lData <= 0)
		lData = 1;
	else if(lData > 360)//KHD : 360���� ����
		lData = 360;
}

void CInputBarDlg::LoadValue()
{	
	LoadIniValue();
}

long CInputBarDlg::GetBitFromCodeInfoUpjong()
{
	long nBit = 0;
	int n = 0;
	CString strCodeChoice = "00000110000011010000000000000";
	                        
	for(int i = 0 ; i < strCodeChoice.GetLength(); i++)
	{
		if(strCodeChoice[i] == '1') 
			n = 1;
		else 
			n = 0;
		
		nBit |= ( n << (strCodeChoice.GetLength() - i-1));
	}
	return nBit;
}

long CInputBarDlg::GetBitFromCodeInfoJongmok()
{
	long nBit = 0;
	int n = 0;
	CString strCodeChoice = "11000000000000000000000000000";	
	                        
	for(int i = 0 ; i < strCodeChoice.GetLength(); i++)
	{
		if(strCodeChoice[i] == '1') 
			n = 1;
		else 
			n = 0;
		
		nBit |= ( n << (strCodeChoice.GetLength() - i-1));
	}
	return nBit;
}

void CInputBarDlg::SaveIniValue()
{
	CString strConfig = m_pStdDialog->m_strConfig;
	char szTemp[10];
	char szTemp2[20];
	CString strTemp;
	int nPos = 0;
	for( nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfButton[nPos]);
		::WritePrivateProfileString("Day Button",szTemp,szTemp2,strConfig);
	}	
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfMinButton[nPos]);
		::WritePrivateProfileString("Min Button",szTemp,szTemp2,strConfig);
	}	
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfSecButton[nPos]);
		::WritePrivateProfileString("Sec Button",szTemp,szTemp2,strConfig);
	}	
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ƽ
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfTickButton[nPos]);
		::WritePrivateProfileString("Tick Button",szTemp,szTemp2,strConfig);
	}	
/*
	for(nPos=0;nPos<CNTOFMAXCBITEMS;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfCombo[nPos]);
		::WritePrivateProfileString("Combo",szTemp,szTemp2,strConfig);
	}
*/
	for(nPos=0;nPos<8;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lCntInView[nPos]);
		::WritePrivateProfileString("CntInView",szTemp,szTemp2,strConfig);
	}

	strTemp.Format("%d", m_bChkSaveJipyoOneFile);
	::WritePrivateProfileString("InputBar","SaveJipyoOneFile",strTemp,strConfig);	

	strTemp.Format("%d", m_bChkLoadSaveCode);
	::WritePrivateProfileString("InputBar","LoadSaveCode",strTemp,strConfig);	

	// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
//	strTemp.Format("%s;%s", m_strUpjongCode, m_strForeignCode);
//	::WritePrivateProfileString("InputBar","SavedCode",strTemp,strConfig);	

//	CString strConfig = theStdParent->m_strConfig;
//	char szTemp[8];
//	char szTemp2[8];
//	CString strTemp;
////	for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
////	{
////		sprintf(szTemp,"%d",nPos);
////		sprintf(szTemp2,"%d",m_lValueOfButton[nPos]);
////		::WritePrivateProfileString("Button",szTemp,szTemp2,strUserDir);
////	}	
////	for(nPos=0;nPos<CNTOFMAXCBITEMS;nPos++)
////	{
////		sprintf(szTemp,"%d",nPos);
////		sprintf(szTemp2,"%d",m_lValueOfCombo[nPos]);
////		::WritePrivateProfileString("Combo",szTemp,szTemp2,strUserDir);
////	}
//	for(int nPos=0;nPos<8;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		sprintf(szTemp2,"%d",m_lCntInView[nPos]);
//		::WritePrivateProfileString("CntInView",szTemp,szTemp2,strUserDir);
//	}
//
////	sprintf(szTemp2,"%d",m_nCntInViewOutside);
////	::WritePrivateProfileString("CntInView","CntInView",szTemp2,strUserDir);
//
//
////	strTemp.Format("%d", m_chkExt1.GetCheck());
////	::WritePrivateProfileString("InputBar","Ext1",strTemp,strUserDir);
////	strTemp.Format("%d", m_chkExt2.GetCheck());
////	::WritePrivateProfileString("InputBar","Ext2",strTemp,strUserDir);
////	strTemp.Format("%d", m_chkExt6.GetCheck());
////	::WritePrivateProfileString("InputBar","Ext6",strTemp,strUserDir);
//
//	//---------------------------------------------------------------------
//	// Tick, Min, Day�� ������ �����Ѵ�.
//	// 2005. 05. 09 by sy, nam
//	//---------------------------------------------------------------------
//	m_editPeriod.GetWindowText(strTemp); strTemp.Remove(' ');
//
//	//���� ���°� �������� �ľ��Ͽ� �����Ѵ�. 05.05.20. add by sy, nam
//	//SaveChartType_IniFile(m_chType);
//	//---------------------------------------------------------------------

}

void CInputBarDlg::SaveValue()
{	
	SaveIniValue();
}

void CInputBarDlg::OnBtrequestdatmore() 
{
	// TODO: Add your control notification handler code here
//	return ;//KHD : ����Ű ���� 
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_REQUESTMORE);
}

//void CInputBarDlg::OnChkext1() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(1);
//}

//void CInputBarDlg::OnChkext2() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(2);
//}


void CInputBarDlg::SetExt(int nAct)
{
	EnableWindow(FALSE);
//	BOOL bExt1 = m_chkExt1.GetCheck();
//	BOOL bExt2 = m_chkExt2.GetCheck();
//	BOOL bExt6 = m_chkExt6.GetCheck();

	int nCmdShow1 = SW_SHOW;
	int nCmdShow2 = SW_SHOW;
	int nCmdShow6 = SW_SHOW;
//	if(bExt1)
//	{
//		m_chkExt1.SetToolTipText("��ư�׷쿭��");
//		nCmdShow1 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt1.SetToolTipText("��ư�׷�ݱ�");
//	}
//	if(bExt2)
//	{
//		m_chkExt2.SetToolTipText("��ư�׷쿭��");
//		nCmdShow2 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt2.SetToolTipText("��ư�׷�ݱ�");		
//	}
//	if(bExt6)
//	{
//		m_chkExt6.SetToolTipText("��ư�׷쿭��");
//		nCmdShow6 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt6.SetToolTipText("��ư�׷�ݱ�");		
//	}


//	m_stText.ShowWindow(SW_HIDE);
// 	m_chkShowScale.ShowWindow(SW_HIDE);

	//m_cbMarket.ShowWindow(nCmdShow6);
	//m_editCode.ShowWindow(nCmdShow6);

	//------------------ ���� ��Ʈ �ΰ�� --------------------
//	if(m_nMarketType==FUTOPT_CHART)
//	{
//		m_btnSearch.ShowWindow(SW_HIDE);
//	}
//	else
//	{
//		m_btnSearch.ShowWindow(nCmdShow6);
//	}	
//	m_btCodeHistory.ShowWindow(nCmdShow6);
	//m_stJongName.ShowWindow(nCmdShow6);
// ---------�ϴ� ���ٷ� �̵� ��Ų��. 05.04.20 -------------------------------------	
//	m_chkRateValue.ShowWindow(nCmdShow1);
// ---------�ϴ� ���ٷ� �̵� ��Ų��. 05.04.20 -------------------------------------
	m_chkDay.ShowWindow(nCmdShow1);
	m_chkWeek.ShowWindow(nCmdShow1);
	m_chkMonth.ShowWindow(nCmdShow1);
	m_chkYear.ShowWindow(nCmdShow1);
	m_chkMinute.ShowWindow(nCmdShow1);
	m_chkSec.ShowWindow(nCmdShow1);
	m_chkTick.ShowWindow(nCmdShow1);
	m_chkFirst.ShowWindow(nCmdShow1);
	m_chkSecond.ShowWindow(nCmdShow1);
	m_chkThird.ShowWindow(nCmdShow1);
	m_chkFourth.ShowWindow(nCmdShow1);	
//	m_chkFifth.ShowWindow(nCmdShow1);	
//	m_chkSixth.ShowWindow(nCmdShow1);	
	m_editPeriod.ShowWindow(nCmdShow1);
//	m_btPeriod.ShowWindow(nCmdShow1);

	m_editZoomValue.ShowWindow(nCmdShow2);
	m_editCntOfDatAll.ShowWindow(nCmdShow2);
	m_editCntOfDatAll.SetLimitText(3);
	m_editZoomValue.SetLimitText(4);
//	m_chkFixPeriod.ShowWindow(nCmdShow2);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//	m_btFixPeriod.ShowWindow(nCmdShow2);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//	m_btResetPeriod.ShowWindow(nCmdShow2);
	m_btRequestMore.ShowWindow(nCmdShow2);
	m_spZoomValue.ShowWindow(nCmdShow2);
	m_stDiv.ShowWindow(nCmdShow2);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//	m_datePicker.ShowWindow(nCmdShow2);
// �ϴ� ���ٷ� �̵� ---------------------------------------------

	CRect rect;
	int nShiftExt61 = 201;//154;
	int nShiftExt12 = 254;
	int nShiftExt23 = 290;
	if(nAct==6)		
	{
//		if(bExt6)
//		{
//			nShiftExt61 *= -1;
//		}
//		SHIFTCONTROL(m_chkExt1,nShiftExt61,rect);
// ---------�ϴ� ���ٷ� �̵� ��Ų��. 05.04.20 -------------------------------------
//		SHIFTCONTROL(m_chkRateValue,nShiftExt61,rect);
// ---------�ϴ� ���ٷ� �̵� ��Ų��. 05.04.20 -------------------------------------
		SHIFTCONTROL(m_chkDay,nShiftExt61,rect);
		SHIFTCONTROL(m_chkWeek,nShiftExt61,rect);
		SHIFTCONTROL(m_chkMonth,nShiftExt61,rect);
		SHIFTCONTROL(m_chkYear,nShiftExt61,rect);
		SHIFTCONTROL(m_chkMinute,nShiftExt61,rect);
	//KHD	SHIFTCONTROL(m_chkSec,nShiftExt61,rect);
		SHIFTCONTROL(m_chkTick,nShiftExt61,rect);
		SHIFTCONTROL(m_chkFirst,nShiftExt61,rect);
		SHIFTCONTROL(m_chkSecond,nShiftExt61,rect);
		SHIFTCONTROL(m_chkThird,nShiftExt61,rect);
		SHIFTCONTROL(m_chkFourth,nShiftExt61,rect);
//		SHIFTCONTROL(m_chkFifth,nShiftExt61,rect);
//		SHIFTCONTROL(m_chkSixth,nShiftExt61,rect);
		SHIFTCONTROL(m_editPeriod,nShiftExt61,rect);
//		SHIFTCONTROL(m_btPeriod,nShiftExt61,rect);

//		SHIFTCONTROL(m_chkExt2,nShiftExt61,rect);
		SHIFTCONTROL(m_editZoomValue,nShiftExt61,rect);
		SHIFTCONTROL(m_editCntOfDatAll,nShiftExt61,rect);
//		SHIFTCONTROL(m_chkFixPeriod,nShiftExt61,rect);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//		SHIFTCONTROL(m_btFixPeriod,nShiftExt61,rect);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//		SHIFTCONTROL(m_btResetPeriod,nShiftExt61,rect);
		SHIFTCONTROL(m_btRequestMore,nShiftExt61,rect);
		SHIFTCONTROL(m_spZoomValue,nShiftExt61,rect);
		SHIFTCONTROL(m_stDiv,nShiftExt61,rect);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//		SHIFTCONTROL(m_datePicker,nShiftExt61,rect);
// �ϴ� ���ٷ� �̵� ---------------------------------------------

//		SHIFTCONTROL(m_chkExtBar2,nShiftExt61,rect);
//		SHIFTCONTROL(m_chkRight,nShiftExt61,rect);
	}
	if(nAct==1)		
	{
//		if(bExt1)
//		{
//			nShiftExt12 *= -1;
//		}
//		SHIFTCONTROL(m_chkExt2,nShiftExt12,rect);
		SHIFTCONTROL(m_editZoomValue,nShiftExt12,rect);
		SHIFTCONTROL(m_editCntOfDatAll,nShiftExt12,rect);
//		SHIFTCONTROL(m_chkFixPeriod,nShiftExt12,rect);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//		SHIFTCONTROL(m_btFixPeriod,nShiftExt12,rect);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//		SHIFTCONTROL(m_btResetPeriod,nShiftExt12,rect);
		SHIFTCONTROL(m_btRequestMore,nShiftExt12,rect);
		SHIFTCONTROL(m_spZoomValue,nShiftExt12,rect);
		SHIFTCONTROL(m_stDiv,nShiftExt12,rect);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//		SHIFTCONTROL(m_datePicker,nShiftExt12,rect);
// �ϴ� ���ٷ� �̵� ---------------------------------------------

//		SHIFTCONTROL(m_chkExtBar2,nShiftExt12,rect);
//		SHIFTCONTROL(m_chkRight,nShiftExt12,rect);
	}
	if(nAct==2)		
	{
//		if(bExt2)
//		{
//			nShiftExt23 *= -1;
//		}

//		SHIFTCONTROL(m_chkExtBar2,nShiftExt23,rect);
//		SHIFTCONTROL(m_chkRight,nShiftExt23,rect);
	}
	EnableWindow();
}
/*
void CInputBarDlg::OnBtnjcodehistory() 
{
	// TODO: Add your control notification handler code here
	if( m_dlgPopupListForHistory != NULL )
	{
		m_dlgPopupListForHistory->DestroyWindow();
		delete m_dlgPopupListForHistory;
		m_dlgPopupListForHistory = NULL;
	}
		
	if(m_dlgPopupListForHistory == NULL)
	{
		if(m_nMarketType==FUTOPT_CHART)			
		{
			m_dlgPopupListForHistory = new CPopupList(&m_lstInfoFuture, 0, this);
		}
		else if(m_nMarketType==UPJONG_CHART)
		{
			m_dlgPopupListForHistory = new CPopupList(&m_lstHistoryInfoUpjong, 0, this);
		}
		else
		{
			m_dlgPopupListForHistory = new CPopupList(&m_lstHistoryInfo, 0, this);
		}
		m_dlgPopupListForHistory->Create(IDD_POPUPLIST, this);		
	}

	CRect rc;
	GetDlgItem(IDC_EDITCODE)->GetWindowRect(&rc);
	m_dlgPopupListForHistory->SetWindowPos(NULL, rc.left, rc.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER); //, MoveWindow(rc.left, rc.bottom, crc.Width(), crc.Height());
	m_dlgPopupListForHistory->ShowWindow(SW_SHOW);		
	
}*/

void CInputBarDlg::OnDestroy() 
{
	// TODO: Add your message handler code here

	CString strCfgFile = m_pStdDialog->m_strConfig;
	//=================================================================================
	//2005. 12. 08
	//������ ���¿��� �����..
	//Ȯ�� ���ٰ� ������ �� ���̰� �����Ǿ� �ִ� ��� 
	//���̴� ������ INI ���Ͽ� �����ϵ��� �ϱ� ���� �߰��Ѵ�.
	//=================================================================================
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(m_bUseReplayChartMode && m_nCurrentMode == REPLAY_ITEM)
	{		
		if(m_ExChangeBeforeInfo.bShowExtToolBar)
		{
			::WritePrivateProfileString("Frame","ShowExtInputBarDlg","1",strCfgFile);
		}
	}
	//=================================================================================

	SaveValue();
	
	if(m_dlgPopupListForHistory) delete m_dlgPopupListForHistory;
	RemoveHistoryInfo();

	m_brBkColor.DeleteObject();

//@��������
//	if(m_pCtrlCodeInputStock)
//	{
//		delete m_pCtrlCodeInputStock;
//		m_pCtrlCodeInputStock = NULL;
//	}
//	if(m_pCtrlCodeInputUpjong)
//	{
//		delete m_pCtrlCodeInputUpjong;
//		m_pCtrlCodeInputUpjong = NULL;
//	}
//	if(m_pCtrlCodeInputFuture)
//	{
//		delete m_pCtrlCodeInputFuture;
//		m_pCtrlCodeInputFuture = NULL;
//	}
//	if(m_pCtrlCodeInputELW)
//	{
//		delete m_pCtrlCodeInputELW;
//		m_pCtrlCodeInputELW = NULL;
//	}
//	if(m_pCtrlCodeInputForeign)
//	{
//		delete m_pCtrlCodeInputForeign;
//		m_pCtrlCodeInputForeign = NULL;
//	}
//	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//	if(m_pCtrlCodeInputCommodity)
//	{
//		delete m_pCtrlCodeInputCommodity;
//		m_pCtrlCodeInputCommodity = NULL;
//	}
//@��������

	if( m_pReplayInfo ) 
	{
		delete m_pReplayInfo;
		m_pReplayInfo = NULL;
	}

	if(m_pCodeInput)
	{
		delete m_pCodeInput;
		m_pCodeInput = NULL;
	}
	if(m_pFOHisBtn)
	{
		delete m_pFOHisBtn ;
		m_pFOHisBtn = NULL;
	}

//@Solomon 	if(m_pCodeNameCtrl)
//@Solomon 	{
//@Solomon 		delete m_pCodeNameCtrl;
//@Solomon 		m_pCodeNameCtrl = NULL;
//@Solomon 	}

	CRscDlg::OnDestroy();	
}

void CInputBarDlg::RemoveHistoryInfo()
{
	if(m_lstHistoryInfo.GetCount() > 0)	
	{	
		POSITION pos = m_lstHistoryInfo.GetTailPosition();	
		CHistory* pHistory;	
		while(pos)		
		{	
			pHistory = (CHistory*)m_lstHistoryInfo.GetPrev(pos);	
			delete pHistory;	
		}	
		m_lstHistoryInfo.RemoveAll();
	}
	if(m_lstHistoryInfoUpjong.GetCount() > 0)	
	{	
		POSITION pos = m_lstHistoryInfoUpjong.GetTailPosition();	
		CHistory* pHistory;	
		while(pos)		
		{	
			pHistory = (CHistory*)m_lstHistoryInfoUpjong.GetPrev(pos);	
			delete pHistory;	
		}	
		m_lstHistoryInfoUpjong.RemoveAll();
	}
	if(m_lstInfoFuture.GetCount() > 0)	
	{	
		POSITION pos = m_lstInfoFuture.GetTailPosition();	
		CHistory* pHistory;	
		while(pos)		
		{	
			pHistory = (CHistory*)m_lstInfoFuture.GetPrev(pos);	
			delete pHistory;	
		}	
		m_lstInfoFuture.RemoveAll();
	}
}


LRESULT CInputBarDlg::OnSelBasedlgPopuplistctrl(WPARAM wParam, LPARAM lParam) 
{
	EnableWindow(FALSE);

	int type = (int)wParam;
	CString str;
	str.Format("%s", (char*)lParam);
	
	if(type == 0)								// �����ڵ�
	{	
		if( m_dlgPopupListForHistory->GetSafeHwnd() )
		{
			m_dlgPopupListForHistory->DestroyWindow();
		}

//		CString sss;
//		m_editCode.GetWindowText(sss);
		if( str == m_strCode )
		{
			CCodeDataFromInputBar CodeData;
			CodeData.m_strCode = GetCurrentCode();
			CodeData.m_lEndTime = GetEndDate();
			CodeData.m_chType = GetType();
			CodeData.m_lTypeNum = GetTypeNumber();
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
				m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
			EnableWindow(TRUE);
			return 0L;
		}
		
		ChangeEditCode(TRUE);
		CCodeDataFromInputBar CodeData;

		CodeData.m_strCode = str;
		CodeData.m_lEndTime = GetEndDate();
		CodeData.m_chType = GetType();
		CodeData.m_lTypeNum = GetTypeNumber();
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
	}

	EnableWindow(TRUE);
	return 1L;
}



LRESULT CInputBarDlg::OnFromCodeList(WPARAM wParam, LPARAM lParam)
{
	if(!theStdParent->GetStatusEnable()) return 1L;
	LPSTCODERESULT pcodeRet = (LPSTCODERESULT)lParam ;
	CString szCode, szName;
	szCode.Format("%-15.15s", pcodeRet->code);		
	szCode.TrimLeft();
	szCode.TrimRight();
	szName.TrimLeft();
	szName.TrimRight();
	
	if (szCode.GetLength()==UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		CString strCodeInputCode = m_pCodeInput->GetCode(); //m_ctrlCodeInput.GetDataText();
		if(szCode.GetLength() && strCodeInputCode != szCode)
		{
			SetShareCodeChanged(TRUE);
//			m_ctrlCodeInput.SetDataText(szCode.Mid(1));

			//m_ctrlCodeInput.SetDataText(szCode);
			m_pCodeInput->SetCode(szCode); //@solomon
			DoSelchangeCodeInput();
		}
	}
	else
	{
		CString strCodeInputCode = m_pCodeInput->GetCode();
		if(szCode.GetLength() && strCodeInputCode != szCode)
		{
			SetShareCodeChanged(TRUE);
			//m_ctrlCodeInput.SetDataText(szCode);
			m_pCodeInput->SetCode(szCode); //@solomon
			DoSelchangeCodeInput();
		}
	}

	m_strCodeFromOutside = szCode;
	if(ChangeEditCode(FALSE))
	{
		EnableWindow(FALSE);
		
		m_strOldCode = GetCurrentCode();
		SetCurrentCode(szCode);

		CCodeDataFromInputBar CodeData;
		CodeData.m_strCode = szCode;
		CodeData.m_lEndTime = GetEndDate();
		CodeData.m_chType = GetType();
		CodeData.m_lTypeNum = GetTypeNumber();
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
		EnableWindow(TRUE);
	}
	
	return 1L;
}

BOOL CInputBarDlg::ChangeEditCode(BOOL bFocus, BOOL bDontPutInShared /*=FALSE*/)
{
	return TRUE; //2005. 07. 15

	BOOL bUpdate = FALSE;
	
	if(m_strCodeFromOutside.GetLength() == UPJONG_LENGTH )
	{
		m_strCodeFromOutside.MakeUpper();			
		CString name;
		name = GetCodeNameAndGubun(m_strCodeFromOutside);
		if(!name.IsEmpty())
		{
			m_stJongName.SetWindowText(name);	//@Solomon-091020
			AddHistoryInfoListUpjong(m_strCodeFromOutside, name);
			bUpdate = TRUE;
			GetParent()->SendMessage(UMSG_SENDSHAREDCODE,(WPARAM)(LPCTSTR)m_strCodeFromOutside);
		}
	}
	else if( (m_strCodeFromOutside.GetLength() == FUTOPT_LENGTH	) ||
			 (m_strCodeFromOutside.GetLength() == KQOPT_LENGTH12	) ||
			 (m_strCodeFromOutside.GetLength() == KQFUT_LENGTH	) ||
			 (m_strCodeFromOutside.GetLength() == KQOPT_LENGTH	)
			 )
	{
		CString name;
		name = GetCodeNameAndGubun(m_strCodeFromOutside);
		if(!name.IsEmpty())
		{
			//m_stJongName.SetWindowText(name);	//@Solomon-091020
			m_stJongName.SetText(name);
			bUpdate = TRUE;
			GetParent()->SendMessage(UMSG_SENDSHAREDCODE,(WPARAM)(LPCTSTR)m_strCodeFromOutside);
		}
	}
	else if(m_strCodeFromOutside.GetLength() >= CODE_STOCK_LENGTH)
	{	
		m_strCodeFromOutside.MakeUpper();			
		CString name;
		name = GetCodeNameAndGubun(m_strCodeFromOutside);
		if(!name.IsEmpty())
		{
			//m_stJongName.SetWindowText(name);	//@Solomon-091020
			m_stJongName.SetText(name);
			AddHistoryInfoListJongmok(m_strCodeFromOutside, name);		
			if(!bDontPutInShared)
			{
				GetParent()->SendMessage(UMSG_SENDSHAREDCODE,(WPARAM)(LPCTSTR)m_strCodeFromOutside);
			}
			bUpdate = TRUE;
		}	
	}
	else
	{	
#ifdef _DEBUG
		AfxMessageBox("�ؿ� ���� ����?");
#endif 
	}
	
	return bUpdate;
} 
 
// JSJ_ITCastle_Modify_040503	COMMENT: ���� ����üũ �Ϻ� ����
CString CInputBarDlg::GetCodeNameAndGubun(CString csCode)
{
	CString str		=	_T("");	
	CString csName	=	_T("");

	switch(m_nMarketType)
	{
		case CMasterTypeInfo::STOCK_CHART: 
			{
				if(csCode.GetLength()==CODE_STOCK_LENGTH)
				{
					if(m_pDataManager != NULL)
					{
						csCode.TrimRight();
						csName		= m_pDataManager->GetCodeInfo("JMASTER", 0, csCode);			
						if(csName.GetLength()<1)
							csName		= m_pDataManager->GetCodeInfo("KMASTER", 0, csCode);
					}
					if(!csName.IsEmpty())
					{
						long lPos = csName.Find('&');
						if(lPos>0)
						{
							csName.Insert(lPos,'&');
						}
						return csName;
					}
					return csName;
//					if(m_pDBMgr->GetCodeName(CODE_JONGMOK, csCode, csName))
//					{
//						if(!csName.IsEmpty())
//						{
//							long lPos = csName.Find('&');
//							if(lPos>0)
//							{
//								csName.Insert(lPos,'&');
//							}
//							return csName;
//						}
//					}
//					if(m_pDBMgr->GetCodeName(CODE_KJONGMOK, csCode, csName))
//					{
//						if(!csName.IsEmpty())
//						{
//							long lPos = csName.Find('&');
//							if(lPos>0)
//							{
//								csName.Insert(lPos,'&');
//							}
//							return csName;
//						}
//					}
				}	
			}
			break;
		case CMasterTypeInfo::UPJONG_CHART:
			{
				if(csCode.GetLength()==UPJONG_LENGTH)
				{
					CString strKey;
					strKey = csCode;

					if(m_pDataManager != NULL)
						csName			= m_pDataManager->GetCodeInfo("UMASTER", 0, strKey);			
					return csName;

//					char type = strKey.GetAt(0);
//					
//			
//					// �屸�� '1'-KOSPI, '2'-KOSDAQ, '3'-KOSPI200
//					if( type == '0')
//						m_pDBMgr->GetCodeName(CODE_UPJONG,		strKey, csName);
//					else if( type == '1')
//						m_pDBMgr->GetCodeName(CODE_K200UPJONG,	strKey, csName);
//					else if( type == '2' || type == '3')
//						m_pDBMgr->GetCodeName(CODE_KUPJONG,		strKey, csName);
//			
//					return csName;
				}
			}
			break;
		case CMasterTypeInfo::FUTOPT_CHART:		
			{
				if( (csCode.GetLength() == FUTOPT_LENGTH) ||
					(csCode.GetLength() == KQOPT_LENGTH12) ||
					(csCode.GetLength() == KQFUT_LENGTH	) ||
					(csCode.GetLength() == KQOPT_LENGTH )
				)
				{
					if(m_pDataManager != NULL)
						csName			= m_pDataManager->GetCodeInfo("FMASTER:PMASTER", 0, csCode);			
					return csName;
					// ����
//					m_pDBMgr->GetCodeName(CODE_FUTURE, csCode, csName);
//					if(csName.GetLength())
//						return csName;
//					
//					// �ɼ�
//					m_pDBMgr->GetCodeName(CODE_OPTION, csCode, csName);
//					if(csName.GetLength())
//						return csName;		
//			
//					// KOSDAQ50 ����  -�Ϲ� �����ڵ�
//					m_pDBMgr->GetCodeName(CODE_QFUTURE	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//			
//					// KOSDAQ50 ����  -���� �����ڵ�
//					m_pDBMgr->GetCodeName(CODE_QFUTURES	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//			
//					// KOSPI ���� SPREAD �����ڵ�
//					m_pDBMgr->GetCodeName(CODE_FSPREAD	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//			
//					// KOSDAQ �ɼ� - cain72
//					m_pDBMgr->GetCodeName(CODE_QOPTION	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//			
//					// KOSDAQ �ɼ� - ��������	//�ð�����, ������
//					m_pDBMgr->GetCodeName(CODE_QOPTIONS	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//					
//					// �ֽĿɼ�		//shc 20020110
//					m_pDBMgr->GetCodeName(CODE_JOPTION	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//					
//					m_pDBMgr->GetCodeName(CODE_FSTAR	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//
//					m_pDBMgr->GetCodeName(CODE_FSSPREAD	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
				}
			}
			break;
		case CMasterTypeInfo::STOCKFUTOPT_CHART:
			{
				if( (csCode.GetLength() == FUTOPT_LENGTH) ||
					(csCode.GetLength() == KQOPT_LENGTH12) ||
					(csCode.GetLength() == KQFUT_LENGTH	) ||
					(csCode.GetLength() == KQOPT_LENGTH )
				)
				{
					if(m_pDataManager != NULL)
						csName			= m_pDataManager->GetCodeInfo("FMASTER:PMASTER", 0, csCode);			
					return csName;
					// ����
//					m_pDBMgr->GetCodeName(CODE_FUTURE, csCode, csName);
//					if(csName.GetLength())
//						return csName;
//					
//					// �ɼ�
//					m_pDBMgr->GetCodeName(CODE_OPTION, csCode, csName);
//					if(csName.GetLength())
//						return csName;		
//			
//					// KOSDAQ50 ����  -�Ϲ� �����ڵ�
//					m_pDBMgr->GetCodeName(CODE_QFUTURE	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//			
//					// KOSDAQ50 ����  -���� �����ڵ�
//					m_pDBMgr->GetCodeName(CODE_QFUTURES	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//			
//					// KOSPI ���� SPREAD �����ڵ�
//					m_pDBMgr->GetCodeName(CODE_FSPREAD	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//			
//					// KOSDAQ �ɼ� - cain72
//					m_pDBMgr->GetCodeName(CODE_QOPTION	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//			
//					// KOSDAQ �ɼ� - ��������	//�ð�����, ������
//					m_pDBMgr->GetCodeName(CODE_QOPTIONS	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//					
//					// �ֽĿɼ�		//shc 20020110
//					m_pDBMgr->GetCodeName(CODE_JOPTION	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//					
//					m_pDBMgr->GetCodeName(CODE_FSTAR	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
//
//					m_pDBMgr->GetCodeName(CODE_FSSPREAD	, csCode, csName);
//					if(csName.GetLength())
//						return csName;	
				}
			}
			break;
		case CMasterTypeInfo::FOREIGN_CHART:				
			{						
//				m_pDBMgr->GetCodeName(CODE_HIGHYIELD, csCode, csName);
//				if(csName.GetLength())
//					return csName;					
//				
//				m_pDBMgr->GetCodeName(CODE_INBOND, csCode, csName);
//				if(csName.GetLength())
//					return csName;
//				
//				m_pDBMgr->GetCodeName(CODE_BOND	, csCode, csName);
//				if(csName.GetLength())
//					return csName;
//				
//				m_pDBMgr->GetCodeName(CODE_SMALLBOND	, csCode, csName);
//				if(csName.GetLength())
//					return csName;	
//				
//				m_pDBMgr->GetCodeName(CODE_WRSTOCK	, csCode, csName);
//				if(csName.GetLength())
//					return csName;				
//				
//				m_pDBMgr->GetCodeName(CODE_EXCHANGE	, csCode, csName);
//				if(csName.GetLength())
//					return csName;				
//				
//				m_pDBMgr->GetCodeName(CODE_EXCH_AUP	, csCode, csName);
//				if(csName.GetLength())
//					return csName;						
//				
//				m_pDBMgr->GetCodeName(CODE_EXCH_AJONG	, csCode, csName);
//				if(csName.GetLength())
//					return csName;	
//					
//				m_pDBMgr->GetCodeName(CODE_EXCH_DR	, csCode, csName);
//				if(csName.GetLength())
//					return csName;	
					
			}
			break;
		case CMasterTypeInfo::FUOPT_JIPYO_CHART:
			{
				//@Solomon���⿡ �����Ұ�.
			}
			break;
		default:
	#ifdef _DEBUG
				AfxMessageBox("GetCodeNameAndGubun == ��Ʈ ���а��� ����");
	#endif
			break;
	}

	return "";
}


BOOL CInputBarDlg::AddHistoryInfoListUpjong(CString szKey, CString szName, BOOL bShareCodeList /*=TRUE*/)
{
	CString rValue;
	POSITION pos = m_lstHistoryInfoUpjong.GetHeadPosition();
	CHistory* pHistory;
	CHistory* pInsert = new CHistory(0, szKey, szName);
	while(pos)
	{
		pHistory = (CHistory*)m_lstHistoryInfoUpjong.GetAt(pos);
		if(!pHistory->_Code.Compare(szKey))
		{
			delete pHistory;
			m_lstHistoryInfoUpjong.RemoveAt(pos);
			m_lstHistoryInfoUpjong.AddTail(pInsert);
			return FALSE;
		}
		m_lstHistoryInfoUpjong.GetNext(pos);
	}
	m_lstHistoryInfoUpjong.AddTail(pInsert);
	if(m_lstHistoryInfoUpjong.GetCount()>20)
	{
		CHistory* pHistory = m_lstHistoryInfoUpjong.GetHead();
		if(pHistory)
		{
			delete pHistory;
		}
		m_lstHistoryInfoUpjong.RemoveHead();			
	}
	if(bShareCodeList)
	{
		BOOL bResult = FALSE;
		//���� �����丮�� �ִ� �κ�...���� �����Ǹ� ��Ȳ�� ���� �ڵ� �ʿ� -- 2005.05.12
		//BOOL bResult = m_pctrlCode->InsertHistory(20, GetBitFromCodeInfoUpjong(), szKey, szName);
		return bResult;
	}	
	return TRUE;
}

BOOL CInputBarDlg::AddHistoryInfoListJongmok(CString szKey, CString szName, BOOL bShareCodeList /*=TRUE*/)
{
	CString rValue;

	if(szKey=="000000") return FALSE;
	long lPos = szName.Find("&&");
	if(lPos>=0)
	{
		szName.Delete(lPos);
	}	
	POSITION pos = m_lstHistoryInfo.GetHeadPosition();
	CHistory* pHistory;
	CHistory* pInsert = new CHistory(0, szKey, szName);
	while(pos)
	{
		pHistory = (CHistory*)m_lstHistoryInfo.GetAt(pos);
		if(!pHistory->_Code.Compare(szKey))
		{
			delete pHistory;
			m_lstHistoryInfo.RemoveAt(pos);
			m_lstHistoryInfo.AddTail(pInsert);
			return FALSE;
		}
		m_lstHistoryInfo.GetNext(pos);
	}
	m_lstHistoryInfo.AddTail(pInsert);
	if(m_lstHistoryInfo.GetCount()>20)
	{
		CHistory* pHistory = m_lstHistoryInfo.GetHead();
		if(pHistory)
		{
			delete pHistory;
		}
		m_lstHistoryInfo.RemoveHead();
			
	}
	if(bShareCodeList)
	{
		BOOL bResult = FALSE;
		//���� �����丮�� �ִ� �κ�...���� �����Ǹ� ��Ȳ�� ���� �ڵ� �ʿ� -- 2005.05.12
		//BOOL bResult = m_pctrlCode->InsertHistory(20, GetBitFromCodeInfoJongmok(), szKey, szName);
		return bResult;
	}
	
	return TRUE;
}

BOOL CInputBarDlg::LoadInfoFutureList()
{
//	CStringArray arFutOptCode, arFutOptName;
//	m_pDBMgr->GetMasterData(CODE_FUTURE, arFutOptCode, arFutOptName);
//	CHistory* pHistory = NULL;
//	for(int i = arFutOptCode.GetSize() - 1; i > -1 ; i--)
//	{		
//		pHistory = new CHistory(0, arFutOptCode.GetAt(i), arFutOptName.GetAt(i));
//		m_lstInfoFuture.AddTail(pHistory);
//	}




/*	CList<CFMaster, CFMaster>*	m_pListF = NULL;
	m_pListF = (CList<CFMaster, CFMaster>*)m_pctrlCode->GetCodePointer("FMASTER");
	if(m_pListF)
	{
		CHistory* pHistory = NULL;
		CFMaster fMaster;
		POSITION pos = m_pListF->GetTailPosition();
		while(pos)
		{
			fMaster = m_pListF->GetPrev(pos);
			if(fMaster.m_szCode.Find('S')==-1)
			{
				pHistory = new CHistory(0, fMaster.m_szCode, fMaster.m_szName);
				m_lstInfoFuture.AddTail(pHistory);
			}
		}
	}
*/
	return TRUE;
}

// �ϴ� ���ٷ� �̵� ---------------------------------------------
//void CInputBarDlg::OnBtfixperiod() 
//{
//	// TODO: Add your control notification handler code here
//	CString strDate;
//	CTime tmItem;
//	m_datePicker.GetTime(tmItem);
//	strDate = tmItem.Format("%Y%m%d");
//	CString strZoomValue;
//	long lZoomValue = 0;
//	m_editZoomValue.GetWindowText(strZoomValue);
//	lZoomValue = atol(strZoomValue);
//	m_pChartMng->SendMessage(UMSG_FIXDATE,(WPARAM)lZoomValue, (LPARAM)(LPCTSTR)strDate);
//}
// �ϴ� ���ٷ� �̵� ---------------------------------------------

void CInputBarDlg::OnBtinc() 
{
	// TODO: Add your control notification handler code here
	CString strInc;
	m_btInc.GetWindowText(strInc);
	if(strInc=="����")
	{
		SetMode(ADD_NEW_CHART); // ���濡�� �߰��� ����
	}
	else if(strInc=="�߰�")
	{
		SetMode(OVERLAPP_CHART);  //�߰����� ��ø���� ����
	}	
	else if(strInc=="��ø") //2005.05.11 ��Ʈ ��ø �κ� �߰�
	{
		SetMode(CHANGE_CHART);	//��ø���� �������� ����
	}
}

void CInputBarDlg::SetMode(int nMode)
{
	// �������� �߰� UI : �������� - ojtaso (20070522)
	if(m_bUseReplayChartMode)
	{
		if( (m_nCurrentMode == REPLAY_ITEM) && (nMode != REPLAY_ITEM) ) // ���� O-> ���� X
		{
			if(!theStdParent->UnLoad_ReplayChart_Lib())
				return;

			ChangeToolBarForReplayChart(FALSE);
			m_nCurrentMode = nMode;	
		}
		else if( (m_nCurrentMode != REPLAY_ITEM) && (nMode == REPLAY_ITEM) ) // ���� X -> ���� O
		{
			//���� Lib �ε� 
			if(!theStdParent->Load_ReplayChart_Lib())
			{			
				m_cmbChartViewType.SetCurSel(m_nCurrentMode);
				AfxMessageBox("���� ���� ��ȯ�ϴµ� �����Ͽ����ϴ�");
				return;
			}

			m_nCurrentMode = nMode;	
			ChangeToolBarForReplayChart(TRUE);
		}
		else
			m_nCurrentMode = nMode;	
	}
	else
		m_nCurrentMode = nMode;	

	if(m_cmbChartViewType.GetSafeHwnd())
		m_cmbChartViewType.SetCurSel(nMode);
		


	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_CHANGEMODE, 0, m_nCurrentMode);

	// �����
	// 1. ��� ��Ʈ Real ���� 
	// 2. ȣ��â ���߱�
	// 3. ��Ʈ �ʱ�ȭ 

/*	if(nMode == OVERLAPP_CHART)
	{
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_CHANGEMODE,0,nMode);
		m_btInc.SetWindowText("��ø");
		m_btInc.SetToolTipText("������ø");
	}
	else if(nMode == CHANGE_CHART)
	{ 
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_CHANGEMODE,0,nMode);
			
		m_btInc.SetWindowText("����");
		m_btInc.SetToolTipText("���񺯰�");
	}
	else if(nMode == ADD_NEW_CHART)//2005.05.11 ��Ʈ ��ø �κ� �߰�
	{
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_CHANGEMODE,0,nMode);

		m_btInc.SetWindowText("�߰�");
		m_btInc.SetToolTipText("�����߰�");
	}
*/
}

//void CInputBarDlg::OnChkfixperiod() 
//{
//	// TODO: Add your control notification handler code here
////	if(m_chkFixPeriod.GetCheck())
////	{
////		OnBtfixperiod();
////	}
//}

HBRUSH CInputBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		if(pWnd->GetSafeHwnd()==m_editPeriod.GetSafeHwnd())
		{
			return hbr;
		}
		else
		{
			pDC->SetBkColor(m_crBk);
			return m_brBkColor;
		}		
		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


BOOL CInputBarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
	case WM_MOUSEWHEEL :
		{
			if( m_cmbChartViewType.GetSafeHwnd()==pMsg->hwnd ||
				m_cbMarket.GetSafeHwnd()==pMsg->hwnd )
			{
				return TRUE;
			}
			break;
		}
	case WM_KEYDOWN:
		switch(pMsg->wParam) 
		{
		case VK_RETURN:
			{
				HWND hParent = ::GetParent(pMsg->hwnd);
				if(m_editZoomValue.GetSafeHwnd()==pMsg->hwnd)
				{
					CString strZoomValue, strTotValue;
					m_editZoomValue.GetWindowText(strZoomValue);

					// ���������� Total Count�� ��� : �������� - ojtaso (20070704)
					GetDlgItemText(IDC_STC_TOTALCNT, strTotValue);

					if(strZoomValue.GetLength() && atoi(strTotValue) > 0)
					{
						if( atoi(strZoomValue) > atoi(strTotValue) )
							strZoomValue = strTotValue;
					}	

					if(atol(strZoomValue)>0)
					{
						//����ڰ� ���� �Է��� ��쿡�� ��Ʈ�� ���̴� ������ ���� �Ѵ�. ==========
						//2007.02.14 by LYH Enter�� ��� Ÿ�� �ݿ����� ����
						m_lCntInView[m_chType-'0'] = atol(strZoomValue); //05.09.13
/*
						for(int n = 0;n<8;n++)//ƽ �� �� �� �� �� 10�� 30�� 
						{
							m_lCntInView[n] = atol(strZoomValue);
						}
*/
						//}}
						//========================================================================

						if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						{
							m_pChartMng->SendMessage(UMSG_SETZOOMVALUE,atol(strZoomValue));

							CString strUserDir = m_pStdDialog->m_strConfig;
							char szTemp[8];
							char szTemp2[8];
							for(int nPos=0;nPos<5;nPos++)
							{
								sprintf(szTemp,"%d",nPos);
								sprintf(szTemp2,"%d",m_lCntInView[nPos]);
								::WritePrivateProfileString("CntInView",szTemp,szTemp2,strUserDir);
							}
						}

					}
					return TRUE;
				}
				else if(m_editCntOfDatAll.GetSafeHwnd()==pMsg->hwnd)
				{
					CString strCntOfDatAll;
					m_editCntOfDatAll.GetWindowText(strCntOfDatAll);
					long dwUserVal = atol(strCntOfDatAll);
					if(dwUserVal>0)
					{
						if(dwUserVal>USER_MAX_INPUT)
						{
							m_editCntOfDatAll.SetWindowText("800");
							dwUserVal = USER_MAX_INPUT;
						}
						if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
							m_pChartMng->SendMessage(UMSG_SETCNTS, dwUserVal);
					}
					return TRUE;
				}
				else if(m_editPeriod.GetSafeHwnd()==pMsg->hwnd)
				{
					//========================================================================================
					//2005.05.09 Tick, Min, Day�� ������ ����Ǿ� ����ڰ� ���͸� ġ�� ��Ʈ ������ ����ȸ �Ѵ�.			
					CString strPeriod;
					m_editPeriod.GetWindowText(strPeriod);
					int i = atoi(strPeriod);
					if(i > 360) 					
					{
						AfxMessageBox("�ִ밡���� ���� 360�Դϴ�.");
						m_editPeriod.SetWindowText("360");
					}
					else if(i <= 0)
					{
						AfxMessageBox("�ּҰ����� ���� 1�Դϴ�.");
						m_editPeriod.SetWindowText("1");
						i = 1;
					}

					if(i > 0)
					{						
						SetType();
					}
					return TRUE;
					//==========================================================================================
				}
			}
			return FALSE;
			break;
		case VK_ESCAPE :
			{
				return FALSE;			
			}
			break;	
		}		
		break;
	}

	if(::IsChild(m_pCodeInput->GetSafeHwnd(), pMsg->hwnd))
	{
		return FALSE;
	}

	if(pMsg->wParam == VK_HOME)
		return FALSE;

	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);	
}



void CInputBarDlg::OnBtresetperiod() 
{
	// TODO: Add your control notification handler code here
	CTime t = CTime::GetCurrentTime();
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//	m_datePicker.SetTime(&t);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//	m_chkFixPeriod.SetCheck(0);
//	OnBtfixperiod();
}

void CInputBarDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*	CString strCode;
	m_editCode.GetWindowText(strCode);
	STSHAREDDATA stInfo;
	stInfo.m_szKey = "�ڵ�-����2";
	stInfo.m_szValue = strCode;
	stInfo.m_szValue.TrimRight(';');
//	((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_CREATEOPENSHAREDATA,0, (LPARAM)&stInfo);		
*/	
	CRscDlg::OnRButtonUp(nFlags, point);
}

void CInputBarDlg::OnChkextbar() 
{
	// TODO: Add your control notification handler code here
	BOOL bShowExtInputBarDlg = theStdParent->GetShowExtInputBarDlg();
	theStdParent->SetShowExtInputBarDlg(!bShowExtInputBarDlg);
}

void CInputBarDlg::SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg)
{
	if( !m_chkExtBar1.GetSafeHwnd() || !::IsWindow(m_chkExtBar1.GetSafeHwnd()) )
		return;
	if(IsMiniBar())
		return;
	m_chkExtBar1.SetCheck(bShowExtInputBarDlg);
	if(bShowExtInputBarDlg)
	{
		//		m_chkExtBar1.FP_vSetImage( m_pIL_W20H20, 28, 29);
		m_chkExtBar1.FP_vSetImage( m_pIL_W20H20, 44, 46);
		m_chkExtBar1.FP_vSetToolTipText("����������");
	}
	else
	{
		//		m_chkExtBar1.FP_vSetImage( m_pIL_W20H20, 30, 31);
		m_chkExtBar1.FP_vSetImage( m_pIL_W20H20, 48, 50);
		m_chkExtBar1.FP_vSetToolTipText("�������Ȯ��");
	}
	m_chkExtBar1.Invalidate();
}


void CInputBarDlg::OnBtperiod() 
{
	// TODO: Add your control notification handler code here
    CMenu menu;
    menu.CreatePopupMenu();

	char szTemp[64];
	long lPosAll = 0;
	long lSize = 0;
	
	for(int nIndex=0;nIndex<CNTOFMAXCBITEMS;nIndex++)
	{
		if(m_lValueOfCombo[nIndex])
		{
			sprintf(szTemp,"%d",m_lValueOfCombo[nIndex]);
			if (SEC_DATA_CHART == m_chType && m_lValueOfCombo[nIndex] > 30)
				continue;

			menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,szTemp);
		}
	}
	if( theStdParent->m_nChartType < MINI_MULTI_CHART )
		menu.AppendMenu(MF_STRING,(UINT)1000+CNTOFMAXCBITEMS,"����");

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	CRect rect;
	GetDlgItem(IDC_BTPERIOD)->GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, rect.left, rect.bottom, this);
	if(!nRetValue) return;
	menu.DestroyMenu();

	if((nRetValue-1000)==CNTOFMAXCBITEMS)
	{
//		CSetUnitDlg dlg;
		CSetCycleDlg dlg;
		dlg.m_pInputBarDlg = this;
		dlg.DoModal();
	}
	else
	{
		m_nUnitPos = -1;
		if(m_chType==WEEK_DATA_CHART || m_chType==MONTH_DATA_CHART || m_chType==YEAR_DATA_CHART)
		{
			m_chType = MIN_DATA_CHART;
		}
		sprintf(szTemp,"%d",m_lValueOfCombo[nRetValue-1000]);
		SetType(FALSE);
		m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType,atol(szTemp));			
	}
}

void CInputBarDlg::OnBtinitial2() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_RESETCHART);
}

void CInputBarDlg::OnChkshowscale2() 
{
	// TODO: Add your control notification handler code here
//	m_pChartMng->SendMessage(UMSG_SHOWSCALE,m_chkShowScale.GetCheck());
}

// ---------�ϴ� ���ٷ� �̵� ��Ų��. 05.04.20 -------------------------------------
//void CInputBarDlg::OnChkratevalue() 
//{
//	// TODO: Add your control notification handler code here
//	BOOL bCheckRateValue = m_chkRateValue.GetCheck();
//	if(bCheckRateValue)
//	{
//		m_chkRateValue.SetWindowText("%");
//	}
//	else 
//	{
//		m_chkRateValue.SetWindowText("\\");
//	}	
//	m_pChartMng->SendMessage(UMSG_RATEVALUE,bCheckRateValue);
//}
// ---------�ϴ� ���ٷ� �̵� ��Ų��. 05.04.20 -------------------------------------

void CInputBarDlg::HideLeftBarBtn(int nCmdShow)
{
	m_chkLeft.ShowWindow(SW_HIDE);
//	m_chkRight.ShowWindow(SW_HIDE);	
}

//=========================================================
//void CInputBarDlg::OnChkext6() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(6);
//}
//=========================================================

long CInputBarDlg::GetTypeNumber() 
{ 
	if(!m_editPeriod.GetSafeHwnd())
		return 0;

	CString strPeriod;
	long lPeriod = 0;
	m_editPeriod.GetWindowText(strPeriod);
	strPeriod.TrimRight();
	if(strPeriod.GetLength()==0)
		lPeriod = 0;
	else
		lPeriod = atol(strPeriod);

	if(lPeriod>0)
	{
		return lPeriod;
	}
//	else
//	{
//		if(m_nUnitPos!=-1)
//		{
//			return m_lValueOfButton[m_nUnitPos];
//		}
//	}		
	return 1;		
}

BOOL CInputBarDlg::ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket/* = TRUE*/ , BOOL bRequestToServer/* = FALSE*/)
{
	if (!theStdParent->GetStatusEnable())
		return FALSE;

	BOOL bRet = FALSE;

	if (nMarketType == m_nPosCbMarketOld)
	{
		return TRUE;
	}
	((CChartMng*)m_pChartMng)->m_bNeedForceCode = TRUE;
	if (!IsMiniBar())
	{
		m_chkMinute.EnableWindow();
		m_chkSec.EnableWindow();
		m_chkWeek.EnableWindow();
		m_chkMonth.EnableWindow();
		m_chkYear.EnableWindow();
		m_chkTick.EnableWindow();
	}

	if (nMarketType == CMasterTypeInfo::STOCK_CHART && !IsMiniBar())// foreign option
	{
		m_pwndSymbol->ShowWindow(SW_SHOW);
		m_stJongName.ShowWindow(SW_HIDE);
	}
	else
	{
		m_pwndSymbol->ShowWindow(SW_HIDE);
		m_stJongName.ShowWindow(SW_SHOW);
	
	}

	if (nMarketType == CMasterTypeInfo::UPJONG_CHART)
		m_chkSec.EnableWindow(FALSE);

	if (nMarketType != CMasterTypeInfo::STOCK_CHART && nMarketType != CMasterTypeInfo::UPJONG_CHART)
		m_chkYear.EnableWindow(FALSE);

	if (nMarketType == CMasterTypeInfo::FOREIGN_CHART && !IsMiniBar())// foreign option
	{
		//KHD : ��.��. ƽ�� ����. 
		m_chkMinute.EnableWindow(FALSE);
		m_chkSec.EnableWindow(FALSE);
		m_chkTick.EnableWindow(FALSE);
	}
	if (nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
	{
		m_chkSec.EnableWindow(FALSE);
		m_chkTick.EnableWindow(FALSE);
	}

	if (theStdParent->IsChartScreen("OldFuture") == TRUE)
	{
		m_chkSec.EnableWindow(FALSE);
		m_chkTick.EnableWindow(FALSE);
	}

	int nCurSel = GetMarketIdxToCBIdx(nMarketType);
	m_cbMarket.SetCurSel(nCurSel);
	m_nMarketType = nMarketType;

	if (m_nMarketType != CMasterTypeInfo::UPJONG_CHART && m_nMarketType != CMasterTypeInfo::STOCK_CHART &&
		m_chType == YEAR_DATA_CHART)
	{
		m_chType = DAY_DATA_CHART;
		SetType(FALSE);
	}
	else if (m_nMarketType == CMasterTypeInfo::UPJONG_CHART && m_chType == SEC_DATA_CHART)
	{
		m_chType = MIN_DATA_CHART;
		SetType(FALSE);
	}

	if (bUpdateCbMarket)
	{
		if (m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
			theStdParent->ChangeStockMode(m_nMarketType); 
		else
			((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);

		bRet = TRUE;
	}
	else
	{
		((CChartMng*)m_pChartMng)->SetMarketType(nMarketType);
		if (!IsMiniBar())	m_stJongName.SetText("");	//@Solomon-091020
	}

	if (nCurSel == 0)
	{
		POSITION pos = m_lstHistoryInfo.GetTailPosition();
		if (pos)
		{
			CHistory* pHistory = m_lstHistoryInfo.GetAt(pos);
			if (bRequestToServer && IsMiniBar())
			{
				STINPUTCODEDATA2 stInputCodeData2;
				stInputCodeData2.m_strCode = pHistory->_Code;
				stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
				stInputCodeData2.m_bDontPutInShared = FALSE;
				this->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);			
			}
		}
	}

	if (!bUpdateCbMarket)
		theStdParent->ChangeControlMarketType(nMarketType);

	m_nPosCbMarketOld = nMarketType;

	_DoChangeCodeCtrlUnit(nMarketType);
	
	return bRet;
}

//void CInputBarDlg::OnChkyear() 
//{
//	// TODO: Add your control notification handler code here
//	m_chType = YEAR_DATA_CHART;
//	m_nUnitPos = -1;
//	//m_editPeriod.SetWindowText("1");
//	SetType();	
//}

void CInputBarDlg::OnChkfirst()
{
	// TODO: Add your control notification handler code here
	m_nUnitPos = 0;
//	if(m_chType!='0')
//	{
//		m_chType = '1';
//	}
	CString strPeriod;
	if (MIN_DATA_CHART == m_chType)		// �� : 1/2
		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
		//strPeriod = "0.5";
	else if (SEC_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfSecButton[m_nUnitPos]);
	else if (TICK_DATA_CHART == m_chType)	// ƽ
		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
	else if (HOUR_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfHourButton[m_nUnitPos]);
	else		//(DAY_DATA_CHART == m_chType)  // ��
		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);

	m_editPeriod.SetWindowText(strPeriod);
	SetType();
}

void CInputBarDlg::OnChksecond() 
{
	// TODO: Add your control notification handler code here
	m_nUnitPos = 1;
//	if(m_chType!='0')
//	{
//		m_chType = '1';
//	}
	CString strPeriod;
	if (MIN_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
	else if (SEC_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfSecButton[m_nUnitPos]);
	else if (TICK_DATA_CHART == m_chType)	// ƽ
		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
	else if (HOUR_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfHourButton[m_nUnitPos]);
	else		//(DAY_DATA_CHART == m_chType)  // ��
		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);

	m_editPeriod.SetWindowText(strPeriod);
	SetType();
}

void CInputBarDlg::OnChkthird() 
{
	// TODO: Add your control notification handler code here
	m_nUnitPos = 2;
//	if(m_chType!='0')
//	{
//		m_chType = '1';
//	}
	CString strPeriod;
	if (MIN_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
	else if (SEC_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfSecButton[m_nUnitPos]);
	else if (TICK_DATA_CHART == m_chType)	// ƽ
		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
	else if (HOUR_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfHourButton[m_nUnitPos]);
	else		//(DAY_DATA_CHART == m_chType)  // ��
		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);

	m_editPeriod.SetWindowText(strPeriod);
	SetType();
}

void CInputBarDlg::OnChkfourth() 
{
	// TODO: Add your control notification handler code here
	m_nUnitPos = 3;
//	if(m_chType!='0')
//	{
//		m_chType = '1';
//	}
	CString strPeriod;
	if (MIN_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
	else if (SEC_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfSecButton[m_nUnitPos]);
	else if (TICK_DATA_CHART == m_chType)	// ƽ
		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
	else		//(DAY_DATA_CHART == m_chType)  // ��
		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);

	m_editPeriod.SetWindowText(strPeriod);
	SetType();
}
/*
void CInputBarDlg::OnChkfifth() 
{
	m_nUnitPos = 4;
	CString strPeriod;
	if (MIN_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
	else if (SEC_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfSecButton[m_nUnitPos]);
	else if (TICK_DATA_CHART == m_chType)	// ƽ
		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
	else		//(DAY_DATA_CHART == m_chType)  // ��
		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);

	m_editPeriod.SetWindowText(strPeriod);
	SetType();
}

void CInputBarDlg::OnChksixth() 
{
	m_nUnitPos = 5;

	CString strPeriod;
	if (MIN_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
	else if (SEC_DATA_CHART == m_chType)	// ��
		strPeriod.Format("%d", m_lValueOfSecButton[m_nUnitPos]);
	else if (TICK_DATA_CHART == m_chType)	// ƽ
		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
	else		//(DAY_DATA_CHART == m_chType)  // ��
		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);

	m_editPeriod.SetWindowText(strPeriod);
	SetType();
}
*/

//void CInputBarDlg::OnChksec10() 
//{
//	// TODO: Add your control notification handler code here
//	m_chType = SEC10_DATA_CHART;
//	m_nUnitPos = -1;	
//	SetType();	
//}
//
//void CInputBarDlg::OnChksec30() 
//{
//	// TODO: Add your control notification handler code here
//	m_chType = SEC30_DATA_CHART;
//	m_nUnitPos = -1;	
//	SetType();	
//}



//@��������	
//void CInputBarDlg::OnChkDatalist() 
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);	
//	BOOL bCheck = m_chkDataListWnd.GetCheck();
//	::SendMessage(GetParent()->m_hWnd, USMG_SHOW_DATAWND, 0, (LPARAM)bCheck);
//}
//@��������	


//-----------------------------------------------------------
// 2005. 05. 09 by Nam
//-----------------------------------------------------------
//�� ,  ƽ ��Ʈ �ϰ�� ���� ���� ��Ʈ�� ��û�Ѵ�.
//-----------------------------------------------------------
void CInputBarDlg::OnDeltaposSpinDatecnt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strPeriod;
	long lPeriod = 0;
	m_editPeriod.GetWindowText(strPeriod);
	if(!strPeriod.IsEmpty())
	{
		lPeriod = atol(strPeriod);
		if(pNMUpDown->iDelta>0)
		{
			if(lPeriod<=0) return;
			lPeriod--;
		}
		else
		{
			lPeriod++;
		}

		if(lPeriod <= 1)
			lPeriod = 1;

		strPeriod.Format("%d",lPeriod);
		m_editPeriod.SetWindowText(strPeriod);	
	}
	*pResult = 0;
}

void CInputBarDlg::Enable_PeriodControl(BOOL bEnable)
{	
	m_editPeriod.SetReadOnly(!bEnable);
	m_chkFirst.EnableWindow(bEnable);
	m_chkSecond.EnableWindow(bEnable);
	m_chkThird.EnableWindow(bEnable);
	m_chkFourth.EnableWindow(bEnable);
//	m_chkFifth.EnableWindow(bEnable);
//	m_chkSixth.EnableWindow(bEnable);
	m_btPeriod.EnableWindow(bEnable);
//	GetDlgItem(IDC_SPIN_DATECNT)->EnableWindow(bEnable);	
}

void CInputBarDlg::SetCheckRevised(BOOL bRevised)
{
	m_bChkRevised = bRevised;
	UpdateData(FALSE);
}

void CInputBarDlg::OnChkRevised() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pChartMng)
		m_pChartMng->SendMessage(UMSG_UPDATEREVISED, m_bChkRevised);
}


//@��������
//void CInputBarDlg::SetMarKetToolType(int nMarketType)
//{
//	if(!theStdParent->GetStatusEnable()) return;	
//	ChangeMarket(nMarketType, FALSE, TRUE);
//}
//@��������

void CInputBarDlg::SaveChartType_IniFile(int nType)
{	
	CString strConfig = m_pStdDialog->m_strConfig;
	CString strValue;
	m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
	int nValue = atoi(strValue);
	if(nValue <= 0) strValue = _T("1");

	CString strChartCell;
	strChartCell.Format("%02d%02d", m_nCurSelRow, m_nCurSelCol);
	
	switch(m_chType)
	{
		case TICK_DATA_CHART:				
			::WritePrivateProfileString(strChartCell,"TickValue",strValue,strConfig);
			break;
		case MIN_DATA_CHART:
			::WritePrivateProfileString(strChartCell,"MinValue",strValue,strConfig);
			break;
		case DAY_DATA_CHART:
			::WritePrivateProfileString(strChartCell,"DayValue",strValue,strConfig);
			break;
		case HOUR_DATA_CHART:
			::WritePrivateProfileString(strChartCell,"HourValue",strValue,strConfig);
			break;
	}	
}





//void CInputBarDlg::OnCheckLog() 
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
//	if(m_pChartMng)
//		m_pChartMng->SendMessage(UMSG_USE_LOG, m_bChkLog);
//}

//DEL void CInputBarDlg::OnSelendokCmbCharttype() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	
//DEL 	int nCurSel = m_cmbChartType.GetCurSel();
//DEL 	CString sValue;
//DEL 	m_cmbChartType.GetLBText(nCurSel,sValue);
//DEL 	if(m_pChartMng->GetSafeHwnd()) // 2005. 05. 30 by Nam
//DEL 		m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)sValue);
//DEL }

void CInputBarDlg::OnSelendokCmbCharttype() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_cmbChartType.GetCurSel();
	CString sValue;
	m_cmbChartType.GetLBText(nCurSel,sValue);
	if(m_pChartMng->GetSafeHwnd()) // 2005. 05. 30 by Nam
		m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)sValue, FROM_INPUTBAR);
}


	

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/24
// Return void	: 
// Comments		: ��ȭ �ڵ� ��ǲ�� �����Ѵ�.
//				-. ���� ������ ���� �ڵ���ǲ�� �����ȵɼ��� �ִ�.
//				1. ��ȭ �ڵ���ǲ ���� �� ���� ��ǲ ����
//=> �ڽ��� �ڵ��޺� ����
//-----------------------------------------------------------------------------
void CInputBarDlg::InitBaseInfo(CBaseDesk2* pBaseDesk, BOOL bRequestAP /*=TRUE*/)
{
	CString sCode;
	char szBuf[20] = {0,};
	int nKind = 0;
//	BOOL bFind = TRUE;
	_DoChangeCodeCtrlUnit(m_nMarketType);
// 	switch(m_nMarketType)
// 	{
// 		case STOCK_CHART:
// 			m_pCodeInput->SetUnit("�ֽ�");
// 			//m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_STOCK);
// 			break;
// 		case UPJONG_CHART:
// 			m_pCodeInput->SetUnit("����");
// 			//m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_ITEM);
// 			break;
// 		case FUTOPT_CHART:
// //			m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_FO);
// //			break;
// 		case STOCKFUTOPT_CHART:
// 			m_pCodeInput->SetUnit("�����ɼ�");
// 			//m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_FO);
// 			break;
// 		case FOREIGN_CHART:
// 			m_pCodeInput->SetUnit("�ؿ�");
// 			//m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_STOCK);
// 			break;
// 		default:
// 			bFind = FALSE;
// 	}

//@Solomon-091029 --> �ʿ���� ����.
		if(m_pCodeInput && m_pCodeInput->GetSafeHwnd() != NULL)
		{
			CRect rectComp, ctlRect;
			GetDlgItem(IDC_CTRL_CODEINPUT)->GetWindowRect(ctlRect);
			ScreenToClient(ctlRect);

			//m_ctrlCodeInput.GetWindowRect(rectComp);
			m_pCodeInput->GetWindowRect(rectComp);
			ScreenToClient(rectComp);
			if(m_rectCmb.left != ctlRect.left || m_rectCmb.top != ctlRect.top)
			{
				if( theStdParent->m_nChartType >= MINI_MULTI_CHART )
				{
					//@Now m_pCodeInput->SetWindowPos(NULL, ctlRect.left, ctlRect.top, rectComp.Width(), 20, SWP_NOZORDER);
					m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
				}
				else
				{
					//@Now m_pCodeInput->SetWindowPos(NULL, ctlRect.left, ctlRect.top, rectComp.Width(), 20, SWP_NOZORDER);
					m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
				}
			}

			if( theStdParent->m_nChartType < MINI_MULTI_CHART)
			{
				GetDlgItem(IDC_STJONGNAME)->GetWindowRect(ctlRect);
				ScreenToClient(ctlRect);

				m_stJongName.GetWindowRect(rectComp);
				ScreenToClient(rectComp);

			}

			//�̹��� �ӽ��ڵ�
			//m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
		}

		
// 			if(bRequestAP)
// 			{
// 		//		//�ֽ��� ������ ������  BaseInfo�ʿ��� WMU_GET_KEYCODE ȣ���Ѵ�.
// 				if(	m_nMarketType == STOCK_CHART || 	m_nMarketType == UPJONG_CHART)
// 				{
// 					if(!m_strCodeFromOutside.GetLength())
// 					{
// 						sCode = m_pCodeInput->GetCode(); //m_ctrlCodeInput.GetDataText();
// 						sCode.TrimRight();
// 						if(!sCode.GetLength())
// 						{	
// 		//@solomon �޽��� �����°� �ʿ��Ѱ�? <<!>> ����ȣ 20091001
// 							//::SendMessage(m_ctrlCodeInput.GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
// 							::SendMessage(m_pCodeInput->GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
// 						}
// 					}
// 				}
// 
// 				// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
// 		//@��������
// 		//		else if( m_nMarketType == FUTOPT_CHART || m_nMarketType == COMMODITY_CHART)
// 				else if( m_nMarketType == FUTOPT_CHART)
// 		//@��������
// 				{
// 		//@solomon �޽��� �����°� �ʿ��Ѱ�? <<!>> ����ȣ 20091001
// 					//::SendMessage(m_ctrlCodeInput.GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
// 					::SendMessage(m_pCodeInput->GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
// 				}
// 			}
//@Solomon-091029 <-- �ʿ���� ����.
}

void CInputBarDlg::SetKeyCodeChange(WPARAM wp, LPARAM lp)
{	
	// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)
	if (m_bReceiveDataSetCode)
		return;
	
	CString strCode;
	strCode = (LPSTR)lp;

	if ((wp == NID_CODEINPUT) || (wp == NID_FUTOPTINPUT) ||
		(wp == NID_UPJONGINPUT) || (wp == NID_FOREIGNINPUT))
	{
		// strData -> ����� ���� �ڵ�
		if (strCode.GetLength())
		{
			// �������� �߰� ���� : �������� - ojtaso (20070122)
			if (((CChartMng*)m_pChartMng)->GetAddGraphMode() || m_strCode != strCode)
			{
				CString strTR;
				switch(m_nMarketType)
				{
					case CMasterTypeInfo::STOCK_CHART:			strTR = QUERY_STRJONGMOK;	break;
					case CMasterTypeInfo::UPJONG_CHART:			strTR = QUERY_STRUPJONG;	break;					
					case CMasterTypeInfo::FOREIGN_CHART:		strTR = QUERY_STRFOREIGN;	break;
					case CMasterTypeInfo::FUTOPT_CHART:			strTR = QUERY_STRFUTOPT;	break;
					case CMasterTypeInfo::STOCKFUTOPT_CHART:	strTR = QUERY_STRSTOCKFUTOPT;break;
						// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
					case CMasterTypeInfo::COMMODITY_CHART:		strTR = QUERY_STRFUTOPT;	break;
					case CMasterTypeInfo::FUOPT_JIPYO_CHART:	strTR = QUERY_STRFOJIPYO;	break;
					default				: return;
				}

				long lResult = GetParent()->SendMessage(UMSG_CHECK_BE_COMPLETE_QUERYDATA, (WPARAM)(LPTSTR)(LPCTSTR)strTR);
				if (lResult == 0)
					return;

				//��ǥ �������⸦ �����. ===================================================================
				GetParent()->SendMessage(UMSG_STOP_JIPYOCYCLE, 0, 0);
				//===========================================================================================
				if (((m_nMarketType == CMasterTypeInfo::STOCK_CHART) && (strCode.GetLength() == CODE_STOCK_LENGTH)) ||							
					((m_nMarketType == CMasterTypeInfo::UPJONG_CHART) && (strCode.GetLength() == UPJONG_LENGTH)) ||
					(strCode.GetLength() == CODE_STOCK_LENGTH) ||
					(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART) || (m_nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART) ||
					(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART) || (m_nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART) ||
					(m_nMarketType == CMasterTypeInfo::COMMODITY_CHART))
				{
					m_strCodeFromOutside = strCode;
					EnableWindow(FALSE);												
					
					m_strOldCode = GetCurrentCode();
					SetCurrentCode(m_strCodeFromOutside);

					CCodeDataFromInputBar CodeData;
					CodeData.m_strCode = m_strCodeFromOutside;//GetCurrentCode();
					CodeData.m_lEndTime = GetEndDate();
					CodeData.m_chType = GetType();
					CodeData.m_lTypeNum = GetTypeNumber();
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					{
						CString strKorName = GetJongMokNameByCode(GetCurrentCode());
//						m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData, (LPARAM)(LPTSTR)(LPCTSTR)strKorName);
						m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
					}
					EnableWindow(TRUE);		
				}
			}
		}
		// ��Ƽ��Ʈ�� ���ÿ��� : �������� - ojtaso (20070525)
		((CChartMng*)m_pChartMng)->SetSelChangeChart(FALSE);
	}	
}

void CInputBarDlg::SetFocusCodeInput()
{
}

void CInputBarDlg::OnButton1() 
{
	::SendMessage(GetParent()->GetSafeHwnd(), WM_DRDS_DATA, 0, 0);	
}

//@��������
void CInputBarDlg::OnChkForeignMainCode(UINT nID)
{
	// TODO: Add your control notification handler code here
/*
	CString strCode;
	switch ( nID )
    {
		case IDC_CHECK_NASDAQ	:   
			{
				strCode = _T("COMP");
				m_chkNasDaq	.SetCheck(TRUE);
				m_chkDow	.SetCheck(FALSE);
				m_chkSP500	.SetCheck(FALSE);
				m_chkNiKei	.SetCheck(FALSE);
				m_chkTWD	.SetCheck(FALSE);
			}
			break;
		case IDC_CHECK_DOW		:
			{
				strCode = _T("DJI@DJC");
				m_chkNasDaq	.SetCheck(FALSE);
				m_chkDow	.SetCheck(TRUE);
				m_chkSP500	.SetCheck(FALSE);
				m_chkNiKei	.SetCheck(FALSE);
				m_chkTWD	.SetCheck(FALSE);
			}
			break;
		case IDC_CHECK_SP500	: 
			{
				strCode = _T("SPX");
				m_chkNasDaq	.SetCheck(FALSE);
				m_chkDow	.SetCheck(FALSE);
				m_chkSP500	.SetCheck(TRUE);
				m_chkNiKei	.SetCheck(FALSE);
				m_chkTWD	.SetCheck(FALSE);
			}
			break;
		case IDC_CHECK_NIKEI	:  
			{
				strCode = _T("N225");
				m_chkNasDaq	.SetCheck(FALSE);
				m_chkDow	.SetCheck(FALSE);
				m_chkSP500	.SetCheck(FALSE);
				m_chkNiKei	.SetCheck(TRUE);
				m_chkTWD	.SetCheck(FALSE);
			}
			break;
		case IDC_CHECK_TWD		:
			{
				strCode = _T("TWI");
				m_chkNasDaq	.SetCheck(FALSE);
				m_chkDow	.SetCheck(FALSE);
				m_chkSP500	.SetCheck(FALSE);
				m_chkNiKei	.SetCheck(FALSE);
				m_chkTWD	.SetCheck(TRUE);
			}
			break;
	}

	m_chkNasDaq	.Invalidate();
	m_chkDow	.Invalidate();
	m_chkSP500	.Invalidate();
	m_chkNiKei	.Invalidate();
	m_chkTWD	.Invalidate();

//	m_pCtrlCodeInput->SetDataText(strCode);

	m_strCodeFromOutside = strCode;
//@��������
//	//2005. 08. 27 =============================================================
	OnBtnForeignSendtr();
*/
//	//==========================================================================
//@��������
}
//@��������

void CInputBarDlg::Replace_ForeignChartControl()
{
	//���� ������ �׸��� right ���� �̵���ų Ÿ ��Ʈ���� offset���� �����Ѵ�.		
	CRect ctlRect;
	GetDlgItem(IDC_CHKLEFT)->GetWindowRect(ctlRect);
	ScreenToClient(ctlRect);
	int nOffset = ctlRect.right;

	//ƽ��ư �����
	m_chkTick.MoveWindow(CRect(0,0,0,0));

	UINT nID[] = {
		IDC_CTRL_CODEINPUT	, 
		//IDC_BTN_FOREIGN_SENDTR,	
		IDC_STJONGNAME      ,
		IDC_CHKDAY          , 
//		IDC_CHKHOUR         , 
		IDC_CHKWEEK         , 
		IDC_CHKMONTH        , 
		IDC_CHKYEAR         , 
		IDC_CHKMINUTE       , 
//		IDC_CHKSEC			,
//		IDC_CHKFIRST        , 
//		IDC_CHKSECOND       , 
//		IDC_CHKTHIRD        , 
//		IDC_CHKFOURTH       , 
		IDC_EDITPERIOD      , 
		IDC_BTPERIOD		,
//		IDC_CHK_REVISED     , 
//		IDC_CMB_CHARTTYPE   , 
		IDC_EDITZOOMVALUE   , 
		IDC_SPINZOOMVALUE   , 
		IDC_STATICDIV       , 
		IDC_EDITCNTOFDATALL , 
		IDC_BTREQUESTDATMORE, 
		IDC_BTSETENVIR		,
		IDC_CHK_DATALIST	, 
	};

	int nCnt = sizeof(nID) / sizeof(UINT);

	int nWidth = 0;
	for (int i = 0; i < nCnt ; i++)
	{
		ctlRect.left	= 0;
		ctlRect.top		= 0;
		ctlRect.right	= 0;
		ctlRect.bottom	= 0;

		if (nID[i] == IDC_CHK_REVISED)
			continue;
		
		if (nID[i] == IDC_STJONGNAME)
		{
			CRect rc;
			m_pCodeInput->GetWindowRect(rc);
			ScreenToClient(rc);
			m_stJongName.SetWindowPos(NULL, rc.right+1 , rc.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

			GetDlgItem(nID[i])->GetWindowRect(ctlRect);		
			ScreenToClient(ctlRect);

			m_rectJongName = ctlRect;

			nOffset = ctlRect.right+2;

			continue;
		}		

		GetDlgItem(nID[i])->GetWindowRect(ctlRect);		
		
		ScreenToClient(ctlRect);
		nWidth = ctlRect.Width();

		if (nID[i] == IDC_BTN_FOREIGN_SENDTR)
			ctlRect.top = 0;

		ctlRect.left	= nOffset;
		ctlRect.right	= ctlRect.left + nWidth;			
		
		
		GetDlgItem(nID[i])->MoveWindow(ctlRect);
		nOffset = ctlRect.right;
	}
}

void CInputBarDlg::Replace_FutOptControl()
{
	UINT nMainItemId[] = {		   
		IDC_BTN_FUTOPT_CODEDLG,
		IDC_BTN_FUTOPT_FLAG		
	};
	
	
//�����ɼǿ��� �ڽ���/�ֽ� �ɼ��� ���ÿ� �����ִ� ���´� ����ϹǷ�
//�������� ���� BaseInfoũ���� ���� �ٿ� �����ɼ�/�ֽĿɼ�/�ڽ��� �ɼ���
//��� ���·� ���õǵ��� �ϴ� ��ư�� �׿� ������ �����ڵ�â�� ����ǵ��� 
//��ư�� 2�� �߰��Ѵ�.
//�� 2���� ��ư�߰��� ���� Baseinfo�ܰ� ���ξȿ��� �߰���Ű��
//BaseInfo �ܰ��� ���̴� 2��ư�� ����ŭ �������� ���� �پ���.

	// BaseInfo �ܰ� ���� �������� ���� ? ��ư�� ���� Pos�� ��´�.
	CRect baseRect, ctlRect;
	GetDlgItem(IDC_CTRL_CODEINPUT)->GetWindowRect(baseRect);
	ScreenToClient(baseRect);
	int nOffset = baseRect.right ; 

	int nCnt = sizeof(nMainItemId) / sizeof(UINT);
	for(int i = 0; i < nCnt ; i++)
	{
		GetDlgItem(nMainItemId[i])->GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);
		int nWidth = ctlRect.Width();		

		ctlRect.top		= 0;
		ctlRect.right	= nOffset;
		ctlRect.left	= nOffset -=  nWidth;	
		ctlRect.bottom  = baseRect.bottom;

		GetDlgItem(nMainItemId[i])->MoveWindow(ctlRect);
		GetDlgItem(nMainItemId[i])->ShowWindow(SW_SHOW);
	}

	baseRect.right = nOffset;
	GetDlgItem(IDC_CTRL_CODEINPUT)->MoveWindow(baseRect);

}

//@��������	
void CInputBarDlg::OnBtnForeignSendtr() 
{
	EnableWindow(FALSE);												
	
	m_strOldCode = GetCurrentCode();
	SetCurCodeNameToControl(m_strCodeFromOutside);

	

	CCodeDataFromInputBar CodeData;
	CodeData.m_strCode = m_strCodeFromOutside;
	CodeData.m_lEndTime = GetEndDate();
	CodeData.m_chType = GetType();
	CodeData.m_lTypeNum = GetTypeNumber();
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
	{
		SetCurCodeNameToControl(m_strCodeFromOutside);							
		m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
	}
	else
	{
		SetShareCodeChanged(TRUE);
		m_pCodeInput->SetCode(_T(""));
		m_strOldCode = GetCurrentCode();
		m_strCodeFromOutside = _T("");
		SetCurCodeNameToControl(_T(""));
	}
	
	EnableWindow(TRUE);			
}
//@��������	

void CInputBarDlg::OnSelchangeCmbType() 
{		
	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	int nIdx = m_cmbChartViewType.GetCurSel();
	if (nIdx == CB_ERR) return;
	if(m_nCurrentMode == nIdx) return; //���� ���� Return
	
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	BOOL bSaveJipyoPerCodePeriod = ::GetCommonDataInfo(GetSafeHwnd(), COMDATA_SAVE_JIPYO_PER_CODE_PERIOD, 0);
/*	if( bSaveJipyoPerCodePeriod && ( nIdx == NEW_ITEM || nIdx == OVERLAPP_ITEM))
	{
		AfxMessageBox("�����߰�/��ø�� [�����ֱ⺰ ��ǥ����]����� �������� �ʽ��ϴ�.");
		
		m_cmbChartViewType.SetCurSel(m_nCurrentMode);
		return;
	}
*/
	// �������� �߰� UI : �������� - ojtaso (20070522)
	if(m_bUseReplayChartMode)
	{
		if(nIdx == REPLAY_ITEM) // ������ ���ý�
		{
			//=================================================================================
			// 2005.12.08
			// ���Ⱑ ������ �������� �����Ѵ�.
			// ����� ��� ���ҵ� ��Ʈ�� �� �����϶��� �����ϴ�.
			//=================================================================================
			if( !theStdParent->CheckDataTypeForReplay() && m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			{
				AfxMessageBox("������Ʈ�� ������ ��Ʈ�϶��� ������ �����մϴ�");
				m_cmbChartViewType.SetCurSel(m_nCurrentMode);
				return;
			}
			
			int nReason = 0;
			if(nReason = ((CChartMng*)m_pChartMng)->IsAvailableForReplay())
			{
				if(nReason == 1)
					AfxMessageBox("�������� �ֽİ� ���� ���� �����մϴ�.");
				else if(nReason == 2)
					AfxMessageBox("������Ʈ�� �� ����(1X1)������ �����մϴ�");

				m_cmbChartViewType.SetCurSel(m_nCurrentMode);
				return;
			}

			//=================================================================================
//@��������
//			if(m_nMarketType == UPJONG_CHART || m_nMarketType == ELW_CHART)
			if(m_nMarketType != CMasterTypeInfo::STOCK_CHART && m_nMarketType != CMasterTypeInfo::FUTOPT_CHART)
//@��������
			{
				AfxMessageBox("�������� �ֽİ� ���� ���� �����մϴ�.");
				m_cmbChartViewType.SetCurSel(m_nCurrentMode);
				return;
			}

			_DoReplyResourceCheck();
		}
	}

	SetMode(nIdx); // ��Ʈ ��� ����
	
}

//@�����߰�
// ������ ��� ��� �� �ٸ� ���� ����ó��
// MODE_DEFAULT : ������ ���(����/�߰�/��ø/����) ���
// MODE_USER : ���� ���� ����
int CInputBarDlg::GetCurrentMode(int nDefault/* = MODE_DEFAULT*/, int nMode/* = CHANGE_ITEM*/)
{
	if(theStdParent->IsMultiItemChart())
	{
		if(nDefault == MODE_DEFAULT)
		{
			// ������ ���(����/�߰�/��ø/����) ���
			m_nCurrentMode = m_cmbChartViewType.GetCurSel();
			ASSERT(m_nCurrentMode >= CHANGE_ITEM && m_nCurrentMode <= REPLAY_ITEM);

			if(m_nCurrentMode < CHANGE_ITEM || m_nCurrentMode > REPLAY_ITEM)
				m_nCurrentMode = CHANGE_ITEM;
		}
		else // nDefault == MODE_USER
		{
			ASSERT(nMode >= CHANGE_ITEM && nMode <= REPLAY_ITEM);

			// ���� ���� ����
			m_nCurrentMode = nMode;
		}

		return m_nCurrentMode;
	}

	return CHANGE_ITEM;
}

BOOL CInputBarDlg::GetChartViewType(CStringList& listViewType)
{
	listViewType.RemoveAll();
	int nCount = m_cmbChartViewType.GetCount();

	CString strLBText;
	for(int nIndx = 0; nIndx < nCount; ++nIndx)
	{
		m_cmbChartViewType.GetLBText(nIndx, strLBText);
		listViewType.AddTail(strLBText);
	}

	if(nCount != listViewType.GetCount())
	{
		listViewType.RemoveAll();
		return FALSE;
	}

	return TRUE;
}
//@�����߰�

void CInputBarDlg::ChangeToolBarForReplayChart(BOOL bReplay)
{
	//��Ʈ�� ���� �Ǵ� �Ϲ� ���·� �ʱ�ȭ �ϴ� �κ��� �ʿ�.
	//#######################################################################
	// ���� ��� Real ����..	
	m_pChartMng->SendMessage(UMSG_DISCONNECT_ALLREAL); // ��� Real ����
	
	CRect ctlRect;
	int nOffset = 0;
	int nHeight = 0;
	int i = 0;
	if(bReplay) //�� ��ư �̵�
	{
		//0. ����� ������ ��Ʈ ���¸� �����Ѵ�.
		m_stJongName.GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);
		m_stJongName.MoveWindow(ctlRect.left, ctlRect.top, ctlRect.Width(), ctlRect.Height());
		m_rectJongName = ctlRect;

		CString sFile = m_pStdDialog->m_strConfig;
		((CChartMng*)m_pChartMng)->SaveChart(sFile);

		m_chkDay.GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);
		
		nOffset = ctlRect.left; 
		nHeight = ctlRect.Height();

		m_chkTick.MoveWindow(nOffset, ctlRect.top, ctlRect.Width(), ctlRect.Height());

		nOffset = ctlRect.right; 

		m_chkMinute.MoveWindow(nOffset, ctlRect.top, ctlRect.Width(), ctlRect.Height());

		nOffset = nOffset + ctlRect.Width(); 
		m_editPeriod.GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);
		m_editPeriod.MoveWindow(nOffset, ctlRect.top, ctlRect.Width(), ctlRect.Height());

		nOffset = nOffset + ctlRect.Width(); 
		m_btPeriod.GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);
		m_btPeriod.MoveWindow(nOffset, ctlRect.top, ctlRect.Width(), ctlRect.Height());

		nOffset = nOffset + ctlRect.Width(); 

	}
	else
	{
		m_stJongName.GetWindowRect(ctlRect);
		
		ScreenToClient(ctlRect);
		m_stJongName.MoveWindow(ctlRect.left, ctlRect.top, ctlRect.Width(), ctlRect.Height());
		m_rectJongName = ctlRect;
	}
	

	//�� ��ư Offset Get
	
	m_btPeriod.GetWindowRect(ctlRect);	
	ScreenToClient(ctlRect);
	nOffset = ctlRect.right; 
	nHeight = ctlRect.Height();	

	//��ư ���� ����.
	m_bRunningReplay	= FALSE;
	m_bPauseReplay		= FALSE;
	m_bStop		= TRUE;


	//1) ���� ƽ/��/��/�־� ���� ���� -----------------------------------------------
	char chDataType = MIN_DATA_CHART;
	if(m_ExChangeBeforeInfo.chOldType != NULL)  //���� Ÿ���� �ִٸ� ����ɷ� �����Ѵ�.
		chDataType = m_ExChangeBeforeInfo.chOldType;
			
	m_ExChangeBeforeInfo.chOldType = m_chType;
	m_chType = chDataType;

	//SetType(FALSE);

	//2) ���� ��ȸ ���� �� ���� ----------------------------------------------------
	int nInterval = 1;
	if(m_ExChangeBeforeInfo.nInterval)  //���� ��ȸ���� ���� �ִٸ� ����ɷ� �����Ѵ�.
		nInterval = m_ExChangeBeforeInfo.nInterval;

	CString sInterval;
	m_editPeriod.GetWindowText(sInterval);
	m_ExChangeBeforeInfo.nInterval = atoi(sInterval);
	
	sInterval.Format("%d", nInterval);
	m_editPeriod.SetWindowText(sInterval);
	//#######################################################################
	if(bReplay)
	{	
		//�ϴ� ���� ���� / ���̱� ���� 2005.12.08
		m_ExChangeBeforeInfo.bShowExtToolBar = theStdParent->GetShowExtInputBarDlg();
	
		//�ϴ� ���� ����.2005.12.08
		theStdParent->SetShowExtInputBarDlg(FALSE);
		m_chkExtBar1.EnableWindow(FALSE);	
		
		//12.08
		//���⿡�� ��ư�� Disable�Ǿ��� ��...���õ� ���� �����Ǿ� ���̵��� �ϱ� ���� �̹��� 
		//����Ʈ�� ���Ѵ�.
		m_chkTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ƽ", TRUE, 83);
		m_chkMinute.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);		

		//1. ���� ���� ����� ===================================================
		UINT nHideItemId[] = {	
			IDC_CHKDAY			,
			IDC_CHKWEEK			,
			IDC_CHKMONTH		,
			IDC_CHKYEAR         , 
//			IDC_CHKSEC			,
			IDC_CHKFIRST        , 
			IDC_CHKSECOND       , 
			IDC_CHKTHIRD        , 
			IDC_CHKFOURTH       , 
//			IDC_CHKFIFTH        , 
//			IDC_CHKSIXTH        , 
//			IDC_CMB_BONG		,						
//			IDC_CHK_REVISED		,
			IDC_CMB_CHARTTYPE	,			
			IDC_CHK_DATALIST,
			IDC_BTSAVECHART,
			IDC_BTSETENVIR,
			IDC_STC_TOTALCNT
		};


		int nCnt = sizeof(nHideItemId) / sizeof(UINT);		
		for(int i = 0; i < nCnt ; i++)
			GetDlgItem(nHideItemId[i])->ShowWindow(SW_HIDE);			
	

		//���� �ð� ���� ���� 
		m_ExChangeBeforeInfo.bTimeSync = theStdParent->GetTimeSyncFlag();		
		
		theStdParent->SetTimeSyncFlag(TRUE);

		//��Ʈ Ÿ��Ʋ DrawText 
		((CChartMng*)m_pChartMng)->SendMessage(WM_PAINT);

	//2. �� ��ư �ڿ� ���� ���� ��ư���� ���̹Ƿ� �� ��ư�� Rect�� ���ؼ� ��ġ�Ѵ�.
		UINT nReplayItemId[] = 
		{
			IDC_BTN_PLAY_PAUSE	,
			IDC_BTN_STOP_REPLAY	,
			IDC_CMB_SPEED		,
			IDC_CMB_DAY			,
			IDC_STATIC_DAY		,
			IDC_CMB_HOUR		,
			IDC_STATIC_HOUR		,
			IDC_CMB_MIN			,
			IDC_STATIC_MIN		,
//			IDC_CHECK_LOG		,
			IDC_EDITZOOMVALUE		, //2006. 1.9 �ּ�ó�� =====================
			IDC_SPINZOOMVALUE		, //2006. 1.9 �ּ�ó�� =====================
			IDC_STATICDIV			, //2006. 1.9 �ּ�ó�� =====================
			IDC_EDITCNTOFDATALL	, //2006. 1.9 �ּ�ó�� =====================
			IDC_BTREQUESTDATMORE	, //2006. 1.9 �ּ�ó�� =====================
			IDC_STATIC_REPLAYTIME,	//2006. 1.9 �ű� �߰� =====================
			IDC_PRGBAR_REPLAY		//2006. 1.9 �ű� �߰� =====================

		};		

		nCnt = sizeof(nReplayItemId) / sizeof(UINT);
		for(i = 0; i < nCnt ; i++)
		{
			GetDlgItem(nReplayItemId[i])->GetWindowRect(ctlRect);
			ScreenToClient(ctlRect);			
			
			int nWidth = ctlRect.Width();

			ctlRect.top = 3;
			ctlRect.bottom = ctlRect.top + nHeight; 
			ctlRect.left = nOffset;
			ctlRect.right = nOffset + nWidth;

			switch(nReplayItemId[i])
			{
				case IDC_CMB_SPEED	:
				case IDC_CMB_DAY	:
				case IDC_CMB_HOUR	:
				case IDC_CMB_MIN	:
					ctlRect.bottom = 300;
					break;
				case IDC_STATIC_REPLAYTIME:
					{
						GetDlgItem(IDC_EDITZOOMVALUE)->GetWindowRect(ctlRect);
						ScreenToClient(ctlRect);	
						ctlRect.left += 5;
						ctlRect.right = ctlRect.left + nWidth;
					}
					break;
				case IDC_PRGBAR_REPLAY:
					{
						ctlRect.top += 2;
						ctlRect.bottom -= 5;
					}
					break;
			}

			GetDlgItem(nReplayItemId[i])->MoveWindow(ctlRect);
			switch(nReplayItemId[i])
			{
				case IDC_STATIC_REPLAYTIME	:
				case IDC_PRGBAR_REPLAY		:
					GetDlgItem(nReplayItemId[i])->ShowWindow(SW_HIDE);
					break;
				default:
					GetDlgItem(nReplayItemId[i])->ShowWindow(SW_SHOW);
					break;
			}

			nOffset = ctlRect.right;
		}

		//4. ȣ��â Delete
		if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			GetParent()->SendMessage(UMSG_DESTROY_FLOATINGHOGA, 0);

		//5. ���� ������ �ѱ��.
		if( m_pReplayInfo ) 
		{
			delete m_pReplayInfo;
			m_pReplayInfo = NULL;
		}
		
		m_pReplayInfo = new REPLAY_INFO;
		if(m_pReplayInfo)
		{
			//InputBar���� ä��� �ִ� �κи� ä��� �������� CStdDialog���� ä���.
			m_pReplayInfo->nDataKind	= m_nMarketType;
			m_pReplayInfo->cGubun		= MIN_DATA_CHART;
			m_pReplayInfo->nUnit		= m_nMin;

			//���
			int nIdx = m_cmbReplaySpeed.GetCurSel();
			if(nIdx == -1)
			{
				nIdx = 0;
			}
			m_pReplayInfo->nSpeed		= (int)pow(2.0, (double)nIdx);

			//���۽ð� 
			CString strSDate, sHour, sMin;

			int nIdx2 = m_cmbReplayHour	.GetCurSel(); if(nIdx2 < 0) nIdx2 = 0;
			int nIdx3 = m_cmbReplayMin	.GetCurSel(); if(nIdx3 < 0) nIdx3 = 0;

			m_cmbReplayHour	.GetLBText(nIdx2, sHour);
			m_cmbReplayMin	.GetLBText(nIdx3, sMin);
			strSDate = "00000000"; //����� ���ϰ͸� �ϹǷ� 
			strSDate += sHour;
			strSDate += sMin;
			m_pReplayInfo->strStartDate	= strSDate;	
			
			//n����
			int nIdx4 = m_cmbDayBefore.GetCurSel(); if(nIdx4 < 0) nIdx4 = 0;
			m_pReplayInfo->nDayBefore = nIdx4;

			
			//Type�� �̸� ����
			OnChkminute();
	
			//if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			//	m_pChartMng->SendMessage(UMSG_SETTYPELINK,1); 

			//theStdParent->RqstTRForReplay();
		}

	
	}
	else
	{	
		if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			GetParent()->SendMessage(UMSG_DESTROY_FLOATINGHOGA, 1);

		//�ϴ� ���� ������ ������ ���� .2005.12.08
		if(m_ExChangeBeforeInfo.bShowExtToolBar)
			theStdParent->SetShowExtInputBarDlg(TRUE);

		m_chkTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ƽ", TRUE);
		m_chkMinute.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE);
	
		m_chkExtBar1.EnableWindow(TRUE);

		

		EnableWindow(TRUE);

	//1. ���� Stop & Clear
		BOOL bResult = FALSE;
		bResult = theStdParent->RunReplay((int)STOP); 		


		if( m_pReplayInfo ) 
		{
			delete m_pReplayInfo;
			m_pReplayInfo = NULL;
		}

		
	//2. ���� ���� ����� =================================================================
		UINT nReplayItemId[] = 
		{
			IDC_BTN_PLAY_PAUSE	,
			IDC_BTN_STOP_REPLAY	,
			IDC_CMB_SPEED		,
			IDC_CMB_DAY			,
			IDC_STATIC_DAY		,
			IDC_CMB_HOUR		,
			IDC_STATIC_HOUR		,
			IDC_CMB_MIN			,
			IDC_STATIC_MIN		,
			IDC_STATIC_REPLAYTIME,	//2006. 1.9 �ű� �߰� =====================
			IDC_PRGBAR_REPLAY		//2006. 1.9 �ű� �߰� =====================
		};

		int nCnt = sizeof(nReplayItemId) / sizeof(UINT);		
		for(int i = 0; i < nCnt ; i++)
			GetDlgItem(nReplayItemId[i])->ShowWindow(SW_HIDE);

	//3. ���� �⺻ ��ư�� �̵��� ���̱� ===================================================
		UINT nShowItemId[] = {	
			IDC_CHKDAY			,
//			IDC_CHKHOUR			,
			IDC_CHKWEEK			,
			IDC_CHKMONTH		,
			IDC_CHKYEAR         , 
			IDC_CHKMINUTE		,
			IDC_CHKSEC			,
			IDC_CHKTICK			,
			IDC_CHKFIRST        , 
			IDC_CHKSECOND       , 
			IDC_CHKTHIRD        , 
			IDC_CHKFOURTH       , 
//			IDC_CHKFIFTH        , 
//			IDC_CHKSIXTH        , 	
//			IDC_CHK_REVISED		,
//			IDC_CHECK_LOG		,
//			IDC_CMB_CHARTTYPE	,			
			IDC_EDITPERIOD		,
			IDC_BTPERIOD		,
//			IDC_CMB_BONG		,
			IDC_EDITZOOMVALUE	,
			IDC_SPINZOOMVALUE	,
			IDC_STATICDIV		,
			IDC_STC_TOTALCNT	,
			IDC_EDITCNTOFDATALL	,
			IDC_BTREQUESTDATMORE,
			IDC_BTSAVECHART
//			IDC_BTSETENVIR
		};

		BOOL bMultiChartItem = theStdParent->IsMultiItemChart();
		nCnt = sizeof(nShowItemId) / sizeof(UINT);		
		for(i = 0; i < nCnt ; i++)
		{
			if(!bMultiChartItem && nShowItemId[i] == IDC_STC_TOTALCNT)
				continue;

			GetDlgItem(nShowItemId[i])->GetWindowRect(ctlRect);
			ScreenToClient(ctlRect);					
				
			int nWidth = ctlRect.Width();

			if(i==0)
				nOffset = ctlRect.left;
			if(nShowItemId[i]==IDC_CHKFIRST || nShowItemId[i]==IDC_EDITPERIOD)
				nOffset += 2;
			ctlRect.left = 0;
			ctlRect.left = nOffset;
			ctlRect.right = nOffset + nWidth;		

			GetDlgItem(nShowItemId[i])->MoveWindow(ctlRect);
			GetDlgItem(nShowItemId[i])->ShowWindow(SW_SHOW);

			nOffset = ctlRect.right;
		}
		
		// �ð� ���� ������
		theStdParent->SetTimeSyncFlag(m_ExChangeBeforeInfo.bTimeSync);

		//����� ��Ʈ ������ �о�´�.
		//0. ����� ������ ��Ʈ ���¸� �����Ѵ�.
		((CChartMng*)m_pChartMng)->LoadChart(m_pStdDialog->m_strConfig);

		
	}
}

void CInputBarDlg::SetMultiChartItemMode()
{
	if(!theStdParent->IsMultiItemChart())
	{
		UINT nShowItemId[] = {		
				IDC_STATICDIV		,
				IDC_STC_TOTALCNT	,
				IDC_EDITCNTOFDATALL	,
				IDC_BTREQUESTDATMORE,
				IDC_BTSAVECHART,
				IDC_STC_DATE
		};

		int nCnt = sizeof(nShowItemId) / sizeof(UINT);		
		CRect ctlRect;
		int nOffset = 0;
		for(int i = 0; i < nCnt ; i++)
		{
//@100303-A002--> ��Ƽ������Ʈ�ܿ����� UI�� �����ϱ� ���� ó��.
//			if(nShowItemId[i] == IDC_STC_TOTALCNT)
//			{
//				GetDlgItem(nShowItemId[i])->ShowWindow(SW_HIDE);
//				continue;
//			}
//@100303-A002<--

			GetDlgItem(nShowItemId[i])->GetWindowRect(ctlRect);
			ScreenToClient(ctlRect);					

			int nWidth = ctlRect.Width();

			if(i==0)
				nOffset = ctlRect.left;
			if(nShowItemId[i]==IDC_CHKFIRST || nShowItemId[i]==IDC_EDITPERIOD)
				nOffset += 2;
			if(nShowItemId[i]==IDC_STC_DATE)
				nOffset += 4;
			ctlRect.left = 0;
			ctlRect.left = nOffset;
			ctlRect.right = nOffset + nWidth;		

			GetDlgItem(nShowItemId[i])->MoveWindow(ctlRect);
			GetDlgItem(nShowItemId[i])->ShowWindow(SW_SHOW);

			nOffset = ctlRect.right;
		}
	}
}

void CInputBarDlg::OnBtnPlayPause() 
{
	BOOL bResult = FALSE;

	if(m_bRunningReplay) //���� ���¿��� �Ͻ����� ���·� ����ȴ�.
	{
		m_bRunningReplay = FALSE;
		m_bPauseReplay	 = TRUE;
		m_bStop = FALSE;

		bResult = theStdParent->RunReplay(PAUSE); 
		if(!bResult) return;

		m_btnStopReplay.EnableWindow(TRUE);
		m_btnHandleReplay.SetWindowText("��");

		ChangeToolBarStatusForReplay(PAUSE);
		
	}
	else	//�Ͻ����� �Ǵ� ���� ���� -> ������·�.	
	{		
		m_bRunningReplay = TRUE;
		m_bPauseReplay	 = FALSE;

		if(m_bStop)
		{
			if(m_pReplayInfo)
			{
				//���
				int nIdx = m_cmbReplaySpeed.GetCurSel();
				if(nIdx == -1)
				{
					nIdx = 0;
				}
				m_pReplayInfo->nSpeed		= (int)pow(2.0, (double)nIdx);

				//���۽ð� 
				CString strSDate, sHour, sMin;

				int nIdx2 = m_cmbReplayHour	.GetCurSel(); if(nIdx2 < 0) nIdx2 = 0;
				int nIdx3 = m_cmbReplayMin	.GetCurSel(); if(nIdx3 < 0) nIdx3 = 0;

				m_cmbReplayHour	.GetLBText(nIdx2, sHour);
				m_cmbReplayMin	.GetLBText(nIdx3, sMin);
				strSDate = "00000000"; //����� ���ϰ͸� �ϹǷ� 
				strSDate += sHour;
				strSDate += sMin;
				m_pReplayInfo->strStartDate	= strSDate;	
				
				//n����
				int nIdx4 = m_cmbDayBefore.GetCurSel(); if(nIdx4 < 0) nIdx4 = 0;
				m_pReplayInfo->nDayBefore = nIdx4;

				ChangeToolBarStatusForReplay(PLAY);

				bResult = theStdParent->RunReplay(PLAY, m_pReplayInfo);
				if(!bResult)
				{

				}
			}
		}
		else
		{
			bResult = theStdParent->RunReplay(PLAY);
			OnSelchangeCmbSpeed();
			m_btnHandleReplay.FP_vSetImage( m_pIL_W20H20, 69, 68);
		}
			
		m_bStop = FALSE;
		
		if(!bResult) 
		{
			theStdParent->RunReplay( STOP );
	
			m_bRunningReplay	= FALSE;	
			m_bPauseReplay		= FALSE;
			m_bStop				= TRUE;
			

			ChangeToolBarStatusForReplay(STOP);
			return;
		}
		

		m_btnHandleReplay.SetWindowText("||");
		m_btnStopReplay.EnableWindow(TRUE);
	}	

}

void CInputBarDlg::OnBtnStopReplay() 
{	
//	if(m_bPauseReplay) //�Ͻ����� ���¿��� Play ���·� ����ȴ�.
//		theStdParent->RunReplay(PLAY); 
//	else //Play ���¿���  �Ͻ����� ���·� ����ȴ�.
//		theStdParent->RunReplay(PAUSE); 

	theStdParent->RunReplay( STOP );
	
	m_bRunningReplay	= FALSE;	
	m_bPauseReplay		= FALSE;
	m_bStop		= TRUE;

	SetType();

	ChangeToolBarStatusForReplay(STOP);

	Init_nPosOfPrgBar();
}

void CInputBarDlg::OnSelchangeCmbSpeed() 
{	
	//���
	int nIdx = m_cmbReplaySpeed.GetCurSel();
	if(nIdx == -1)
	{
		nIdx = 0;
	}
	int nSpeed		= (int)pow(2.0, (double)nIdx);
	
	//�������̰ų� �Ͻ����� �����϶��� ������.
	//���� ���´� ������ �ʴ´�.
	if(m_bRunningReplay)
		theStdParent->ChangeSpeed(nSpeed);
}


long CInputBarDlg::GetReplayStartTime()
{
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(!m_bUseReplayChartMode)
		return 0;

	if(m_nCurrentMode != REPLAY_ITEM ) return 0l;

	CString strTime, strMin, sReturnTime;
	m_cmbReplayHour.GetLBText(m_cmbReplayHour.GetCurSel()	, strTime	);
	m_cmbReplayMin.GetLBText(m_cmbReplayMin.GetCurSel()		, strMin	);

	return atoi(strTime + strMin);
}

long CInputBarDlg::GetReplayDayBefore()
{
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(!m_bUseReplayChartMode)
		return 0;

	if(m_nCurrentMode != REPLAY_ITEM ) return 0l;

	CString strDay;
	if( m_cmbDayBefore.GetCurSel() == CB_ERR )
	{
		return 0;
	}

	m_cmbDayBefore.GetLBText(m_cmbDayBefore.GetCurSel(), strDay); 
	

	return atoi(strDay);
}

long CInputBarDlg::GetReplaySpeed()
{
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(!m_bUseReplayChartMode)
		return 0;

	if(m_nCurrentMode != REPLAY_ITEM ) return 0;

	CString strSpeed;
	if( m_cmbReplaySpeed.GetCurSel() == CB_ERR )
	{
		return 0;
	}

	m_cmbReplaySpeed.GetLBText(m_cmbReplaySpeed.GetCurSel(), strSpeed); 
	

	return atoi(strSpeed);
}


void CInputBarDlg::SetCurCodeNameToControl(CString strCode)
{
	if (strCode.GetLength()==0)
		return;

	CString strCodeName = m_pDataManager->GetCodeName(strCode);

	if (IsMiniBar())
	{
		int nRealMarketType;
		int nMarketType = theStdParent->GetMarketTypeFromCode(strCode, nRealMarketType);

		SetCodeName(nMarketType, strCode, strCodeName);

		return;
	}

	if (strCodeName != _T(""))
		m_strJonMokName = strCodeName;

	if (theStdParent->IsChartScreen("OldFuture") == TRUE)
	{
		if (m_pFOHisBtn)
			m_strJonMokName = m_pFOHisBtn->GetCodeName(strCode);
	}

	m_stJongName.SetText(m_strJonMokName);

	UpdateData(FALSE);
}

CString CInputBarDlg::GetJongMokNameByCode(CString strCode, LPCTSTR lpszTRCode/* = NULL*/)
{
	if(strCode == _T(""))
	{
		return _T("");	
	}

	int nMarketType = lpszTRCode ? GetMarketTypeFromTRCode(lpszTRCode) : m_nMarketType;
	if(m_pDataManager == NULL)
		return "";

	CString strCodeName;
	switch(nMarketType)
	{
		case CMasterTypeInfo::STOCK_CHART:
			strCode.TrimRight();
			strCodeName	= m_pDataManager->GetCodeInfo("JMASTER", 0, strCode);			
			if(strCodeName.GetLength()<1)
				strCodeName	= m_pDataManager->GetCodeInfo("KMASTER", 0, strCode);
			if(strCodeName.GetLength()<1)	// ELW
				strCodeName	= m_pDataManager->GetCodeInfo("WMASTER", 0, strCode);			
			break;
		case CMasterTypeInfo::UPJONG_CHART:
			strCodeName	= m_pDataManager->GetCodeInfo("UMASTER", 0, strCode);			
			break;
		case CMasterTypeInfo::FUTOPT_CHART:
			strCodeName		= m_pDataManager->GetCodeInfo("FMASTER", 0, strCode);			
			if(strCodeName.GetLength()<1)
				strCodeName		= m_pDataManager->GetCodeInfo("PMASTER", 0, strCode);
			break;
		case CMasterTypeInfo::STOCKFUTOPT_CHART:
			strCodeName	= m_pDataManager->GetCodeName(strCode);
			break;
		case CMasterTypeInfo::FOREIGN_CHART:
			strCodeName	= m_pDataManager->GetCodeInfo("FRMASTER", 0, strCode);		
			{
				long lPos = strCodeName.Find('&');
				if(lPos>0)
					strCodeName.Insert(lPos,'&');
			}
			break;
		case CMasterTypeInfo::FUOPT_JIPYO_CHART:
			strCodeName	= m_pDataManager->GetCodeInfo("FOMASTER", 0, strCode);
			break;
		case CMasterTypeInfo::COMMODITY_CHART:		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
			strCodeName	= m_pDataManager->GetCodeInfo("COMMOCITYMASTER", 0, strCode);
			break;
//@��������
//		case ELW_CHART:
//			strCodeName		= m_pDataManager->GetCodeInfo("WMASTER", 0, strCode);			
//			break;
//@��������
	}

	return strCodeName;
}

int CInputBarDlg::GetMarketTypeFromTRCode(LPCTSTR lpszTRCode)
{
	if(!::lstrcmp(lpszTRCode, QUERY_STRJONGMOK))
		return CMasterTypeInfo::STOCK_CHART;
	else if(!::lstrcmp(lpszTRCode, QUERY_STRUPJONG))
		return CMasterTypeInfo::UPJONG_CHART;
	else if(!::lstrcmp(lpszTRCode, QUERY_STRFUTOPT))
		return CMasterTypeInfo::FUTOPT_CHART;
	else if(!::lstrcmp(lpszTRCode, QUERY_STRSTOCKFUTOPT))
		return CMasterTypeInfo::STOCKFUTOPT_CHART;
//@��������
//	else if(!::lstrcmp(lpszTRCode, QUERY_STRELW))
//		return ELW_CHART;
//@��������
	else if(!::lstrcmp(lpszTRCode, QUERY_STRFOREIGN))
		return CMasterTypeInfo::FOREIGN_CHART;
	else if(!::lstrcmp(lpszTRCode, QUERY_STRFOJIPYO))
		return CMasterTypeInfo::FUOPT_JIPYO_CHART;
	else 
		return CMasterTypeInfo::STOCK_CHART;
}

void CInputBarDlg::SetCurrentCode(CString strCode)
{
	m_strCode = strCode;
}

CString CInputBarDlg::GetCurrentCode()
{
	return m_strCode;
}

void CInputBarDlg::SetEditZoomValue(CString strValue, BOOL bSetInput, BOOL bSetToOCX)
{
	if(IsMiniBar())
		return;

	{	//@Sol-100104SM005
		m_editZoomValue.GetWindowText(m_szOldZoomValue);
		if(strValue.Compare(m_szOldZoomValue)==0) return;
	}

	m_editZoomValue.SetWindowText(strValue);

	//>>JS.Kim_20100918.  ��ȸ����Ÿ�� ���� ��� ��ũ�Ѱ����� �������� �� ������ �������� ������ ��ȸ�ϴ� ��쵵 ��ũ�Ѱ����� �������� ���� ����
	/*
	long nViewCount;
	GetViewCount(nViewCount);
	if(nViewCount>atol(strValue))
	{
		bSetInput = false;
		if(bSetInput==false && m_bInitChartData==TRUE)
			bSetInput = TRUE;
	}
	*/
	//<<
	if(!m_bInitChartData) bSetInput = FALSE;

	UpdateData(TRUE);

	CString strZoomValue, strTotValue;
	// ���������� Total Count�� ��� : �������� - ojtaso (20070704)
//	m_editCntOfDatAll.GetWindowText(strTotValue); strTotValue.TrimLeft(); strTotValue.TrimRight();
	GetDlgItemText(IDC_STC_TOTALCNT, strTotValue);

	if(strTotValue.GetLength() && atoi(strTotValue) > 0)
	{
		if( atoi(strValue) > atoi(strTotValue) )
			strValue = strTotValue;
	}	

	strZoomValue = strValue;
	if(atoi(strZoomValue) < 0)
		strZoomValue = "120";

	//100628-alzioyes: SetViewCount(atol(strZoomValue));
//KHD	if(bSetToOCX)
//		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetNew_OnePageDataCount(atoi(strZoomValue));// KHD : �����̵� ���ý� ����Ʈ�� �ȵǾ� �����Ƿ� ..

	//alzioyes:���Ź� �ٽñ׸����� ó��.
	//((CChartMng*)m_pChartMng)->GetpChartItemSelected()->ReCalculateAllGraphs();

	//100628-alzioyes: 
	if(bSetInput) 
		SetViewCount(atol(strZoomValue));
	
	m_editZoomValue.SetWindowText(strZoomValue);
	m_szOldZoomValue = strZoomValue;

	UpdateData(FALSE);
}

void CInputBarDlg::OnSetfocusEditperiod() 
{	
	SetUseUserInputPeriod(TRUE);
}

void CInputBarDlg::OnChangeEditperiod() 
{
	SetUseUserInputPeriod(TRUE);
}


void CInputBarDlg::ChangeToolBarStatusForReplay(int nReplayMode)
{
	if(nReplayMode == PLAY)
	{		
		m_btnStopReplay.EnableWindow(TRUE);
		m_btnHandleReplay.EnableWindow(TRUE);
		m_cmbReplaySpeed.EnableWindow(TRUE);
		m_cmbReplayHour.EnableWindow(FALSE);
		m_cmbReplayMin.EnableWindow(FALSE);
		m_pCodeInput->EnableWindow(FALSE);
		m_cmbDayBefore.EnableWindow(FALSE);
		m_cmbChartViewType.EnableWindow(FALSE);
		m_btnForSendTR.EnableWindow(FALSE);
//		m_chkDow.EnableWindow(FALSE);
//		m_chkTWD.EnableWindow(FALSE);
//		m_chkSP500.EnableWindow(FALSE);
//		m_chkNiKei.EnableWindow(FALSE);
//		m_chkNasDaq.EnableWindow(FALSE);
		m_chkDataListWnd.EnableWindow(FALSE);
		m_cmbChartType.EnableWindow(FALSE);
		m_chkWeek.EnableWindow(FALSE);
		m_chkMonth.EnableWindow(FALSE);
		m_chkYear.EnableWindow(FALSE);
		m_chkDay.EnableWindow(FALSE);
		m_chkTick.EnableWindow(FALSE);
		m_chkSec.EnableWindow(FALSE);
		m_chkMinute.EnableWindow(FALSE);
		m_editPeriod.EnableWindow(FALSE);
		m_editCntOfDatAll.EnableWindow(FALSE);
		m_stText.EnableWindow(FALSE);
		m_chkExtBar1.EnableWindow(FALSE);
		m_spZoomValue.EnableWindow(FALSE);
		m_btRequestMore.EnableWindow(FALSE);
		m_chkLeft.EnableWindow(FALSE);
		m_btInc.EnableWindow(FALSE);
		m_stDiv.EnableWindow(FALSE);
		m_editZoomValue.EnableWindow(FALSE);
		m_chkDay.EnableWindow(FALSE);

		m_staticReplayTime.ShowWindow(SW_SHOW);	//2006. 1.9 �ű� �߰� =====================
		m_prgBarReplay.ShowWindow(SW_SHOW);		//2006. 1.9 �ű� �߰� =====================	

		GetDlgItem(IDC_EDITZOOMVALUE	)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================	
		GetDlgItem(IDC_SPINZOOMVALUE	)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================
		GetDlgItem(IDC_STATICDIV		)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================
		GetDlgItem(IDC_EDITCNTOFDATALL	)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================
		GetDlgItem(IDC_BTREQUESTDATMORE	)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================

		m_btnHandleReplay.FP_vSetImage( m_pIL_W20H20, 69, 68);
	}
	else if(nReplayMode == PAUSE)
	{		
		m_cmbReplaySpeed.EnableWindow(TRUE);

		m_btnStopReplay.EnableWindow(TRUE);
		m_btnHandleReplay.EnableWindow(TRUE);
		m_pCodeInput->EnableWindow(FALSE);
		m_cmbReplayHour.EnableWindow(FALSE);
		m_cmbReplayMin.EnableWindow(FALSE);
		m_cmbDayBefore.EnableWindow(FALSE);
		m_cmbChartViewType.EnableWindow(FALSE);
		m_btnForSendTR.EnableWindow(FALSE);
//		m_chkDow.EnableWindow(FALSE);
//		m_chkTWD.EnableWindow(FALSE);
//		m_chkSP500.EnableWindow(FALSE);
//		m_chkNiKei.EnableWindow(FALSE);
//		m_chkNasDaq.EnableWindow(FALSE);
		m_chkDataListWnd.EnableWindow(FALSE);
		m_cmbChartType.EnableWindow(FALSE);
		m_chkWeek.EnableWindow(FALSE);
		m_chkMonth.EnableWindow(FALSE);
		m_chkYear.EnableWindow(FALSE);
		m_chkDay.EnableWindow(FALSE);
		m_chkTick.EnableWindow(FALSE);
		m_chkSec.EnableWindow(FALSE);
		m_chkMinute.EnableWindow(FALSE);
		m_editPeriod.EnableWindow(FALSE);
		m_editCntOfDatAll.EnableWindow(FALSE);
		m_stText.EnableWindow(FALSE);
		m_chkExtBar1.EnableWindow(FALSE);
		m_spZoomValue.EnableWindow(FALSE);
		m_btRequestMore.EnableWindow(FALSE);
		m_chkLeft.EnableWindow(FALSE);
		m_btInc.EnableWindow(FALSE);
		m_stDiv.EnableWindow(FALSE);
		m_editZoomValue.EnableWindow(FALSE);

		m_btnHandleReplay.FP_vSetImage( m_pIL_W20H20, 68, 69);
		m_btnHandleReplay.Invalidate();
	}
	else if(nReplayMode == STOP)
	{		
		m_btnStopReplay.EnableWindow(FALSE);
		m_btnHandleReplay.EnableWindow(TRUE);
		m_pCodeInput->EnableWindow(TRUE);
		m_cmbDayBefore.EnableWindow(TRUE);
		m_cmbReplayHour.EnableWindow(TRUE);
		m_cmbReplayMin.EnableWindow(TRUE);
		m_cmbReplaySpeed.EnableWindow(TRUE);
		m_cmbChartViewType.EnableWindow(TRUE);
		m_btnForSendTR.EnableWindow(TRUE);
//		m_chkDow.EnableWindow(TRUE);
//		m_chkTWD.EnableWindow(TRUE);
//		m_chkSP500.EnableWindow(TRUE);
//		m_chkNiKei.EnableWindow(TRUE);
//		m_chkNasDaq.EnableWindow(TRUE);
		m_chkDataListWnd.EnableWindow(TRUE);
		m_cmbChartType.EnableWindow(TRUE);
		m_chkWeek.EnableWindow(TRUE);
		m_chkMonth.EnableWindow(TRUE);
		m_chkYear.EnableWindow(TRUE);
		m_chkDay.EnableWindow(TRUE);
		m_chkDay.EnableWindow(TRUE);
		m_chkTick.EnableWindow(TRUE);
		m_chkSec.EnableWindow(TRUE);
		m_chkMinute.EnableWindow(TRUE);
		m_editPeriod.EnableWindow(TRUE);
		m_editCntOfDatAll.EnableWindow(TRUE);
		m_stText.EnableWindow(TRUE);
		m_chkExtBar1.EnableWindow(TRUE);
		m_spZoomValue.EnableWindow(TRUE);
		m_btRequestMore.EnableWindow(TRUE);
		m_chkLeft.EnableWindow(TRUE);
		m_btInc.EnableWindow(TRUE);
		m_stDiv.EnableWindow(TRUE);
		m_editZoomValue.EnableWindow(TRUE);
		m_btnHandleReplay.FP_vSetImage( m_pIL_W20H20, 68, 69);
		m_btnHandleReplay.Invalidate();
		
		m_staticReplayTime.ShowWindow(SW_HIDE);	//2006. 1.9 �ű� �߰� =====================
		m_prgBarReplay.ShowWindow(SW_HIDE);		//2006. 1.9 �ű� �߰� =====================

		GetDlgItem(IDC_EDITZOOMVALUE	)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================	
		GetDlgItem(IDC_SPINZOOMVALUE	)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================
		GetDlgItem(IDC_STATICDIV		)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================
		GetDlgItem(IDC_EDITCNTOFDATALL	)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================
		GetDlgItem(IDC_BTREQUESTDATMORE	)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================

	}
}

void CInputBarDlg::OnSelchangeCmbMin() 
{
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(!m_bUseReplayChartMode)
		return;

	if(m_nCurrentMode != REPLAY_ITEM ) return;

	CString strMin;
	m_cmbReplayMin.GetLBText(m_cmbReplayMin.GetCurSel(), strMin);	

	if(m_pReplayInfo)
	{
		CString strOldSDate;		
		strOldSDate = m_pReplayInfo->strStartDate;
		if(strOldSDate.Right(2) != strMin)
		{
			strOldSDate = strOldSDate.Left(strOldSDate.GetLength() - 2) + strMin;
			m_pReplayInfo->strStartDate = strOldSDate;

			//TR�� �ٽ� ��û�ϵ��� �� �Ѵ�.
			//m_btnHandleReplay.EnableWindow(FALSE);

			//theStdParent->RqstTRForReplay();
			SetType();
		}		
	}

}

void CInputBarDlg::OnSelchangeCmbHour() 
{
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(!m_bUseReplayChartMode)
		return;

	if(m_nCurrentMode != REPLAY_ITEM ) return;

	CString strHour;
	m_cmbReplayHour.GetLBText(m_cmbReplayHour.GetCurSel(), strHour);	

	if(m_pReplayInfo)
	{
		CString strOldSDate;		
		strOldSDate = m_pReplayInfo->strStartDate;
		if(strOldSDate.Mid(6, 2) != strHour)
		{
			strOldSDate = strOldSDate.Left(6) + strHour +  strOldSDate.Right(2);
			m_pReplayInfo->strStartDate = strOldSDate;

			//TR�� �ٽ� ��û�ϵ��� �� �Ѵ�.
			//m_btnHandleReplay.EnableWindow(FALSE);
			
			SetType();
		}		
	}
	
}

void CInputBarDlg::OnSelchangeCmbDay() 
{
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(!m_bUseReplayChartMode)
		return;

	// TODO: Add your control notification handler code here
	if(m_nCurrentMode != REPLAY_ITEM) return;

	CString strDay;
	m_cmbDayBefore.GetLBText(m_cmbDayBefore.GetCurSel(), strDay);	

	if(m_pReplayInfo)
	{	
		if(m_pReplayInfo->nDayBefore != atoi(strDay))
		{
			m_pReplayInfo->nDayBefore = atoi(strDay);
			//TR�� �ٽ� ��û�ϵ��� �� �Ѵ�.
			//m_btnHandleReplay.EnableWindow(FALSE);
			
			SetType();
		}		
	}
}

void CInputBarDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == ID_SHARE_CODE_WAIT)
	{
		KillTimer(ID_SHARE_CODE_WAIT);

		if(m_pChartMng)
		{
			CCodeDataFromInputBar CodeData;

			CString strCode;
			int nSetp=0, nSelMarketType = m_nMarketType;
			CStringArray arrCode, arrName;
			IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
			pMasterDataManager->GetRecentCodeNameArray(nSelMarketType, arrCode, arrName);
			if(arrCode.GetSize()>0)
			{
				strCode = arrCode.GetAt(0);
			}
			else
			{
				theStdParent->GetFirstCode(strCode, nSelMarketType, nSetp);
				if(strCode == "")
				{
					strCode = m_pCodeInput->GetCode();
				}
			}

			if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
			{
				if(GetType() == SEC_DATA_CHART)
				{
					m_chType = DAY_DATA_CHART;
					m_editPeriod.SetWindowText(_T("1"));
					UpdateBtNCbValue();	// �ֱ��ư Set
					SetType(FALSE);				
				}
			}
			else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
			{
				if (GetType() == MIN_DATA_CHART || GetType() == SEC_DATA_CHART || GetType() == TICK_DATA_CHART)
				{
					m_chType = DAY_DATA_CHART;
					m_editPeriod.SetWindowText(_T("1"));
					UpdateBtNCbValue();	// �ֱ��ư Set
					SetType(FALSE);
				}
			}

			if(strCode.IsEmpty())
			{
				KillTimer(ID_SHARE_CODE_WAIT);
				m_pCodeInput->SetCode(""); //@solomon

				return;
			}
		
			CodeData.m_strCode = strCode;
			CodeData.m_lEndTime = GetEndDate();
			CodeData.m_chType = GetType();
			CodeData.m_lTypeNum = GetTypeNumber();
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			{					
				m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
			}
		}

	}
	CRscDlg::OnTimer(nIDEvent);
}

void CInputBarDlg::ReplayFail()
{
	OnBtnStopReplay();
}

void CInputBarDlg::ReplayEnd()
{
	OnBtnStopReplay();
}

void CInputBarDlg::SetReplayTotalSec(int nTotalSec)
{
	Init_nPosOfPrgBar();
	m_nTotalSec = nTotalSec;

	m_prgBarReplay.SetShowText(TRUE);

	m_prgBarReplay.SetRange(0, nTotalSec);
}

void CInputBarDlg::SetReplayEachSec(CString sTime)
{
	Add_nPosOfPrgBar();
	
	m_staticReplayTime.SetWindowText(sTime);
	

	m_prgBarReplay.StepIt();

	int nCurPos = Get_nPosOfPrgBar();	
	m_prgBarReplay.SetPos(nCurPos);
	

	CString sCurPos;	

	int nCurPos2 = int (((double)nCurPos / (double)m_nTotalSec) * 100.00);

	sCurPos.Format("%d%%", nCurPos2);	
	m_prgBarReplay.SetWindowText(sCurPos);
}

void CInputBarDlg::OnSelchangeCbmarket() 
{
	// TODO: Add your control notification handler code here
	if(!theStdParent->GetStatusEnable()) return;
	//{{2007.03.15 by LYH
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(m_bUseReplayChartMode && m_nCurrentMode == REPLAY_ITEM)
	{
		m_cbMarket.SetCurSel(GetMarketIdxToCBIdx(m_nMarketType));
		AfxMessageBox("���� ��忡���� ������ �Ұ��մϴ�.");
		return;
	}
	//}}
	int nSelCombo = m_cbMarket.GetCurSel();
	int nSelMarketType = m_cbMarket.GetItemData(nSelCombo);

	// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
	//CString strCode = GetCurrentCode();
//	int nRealMarketType;

// 	CString strCode;
// 	int nSetp=0;
// 
// 	CStringArray arrCode, arrName;
// 	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
// 	pMasterDataManager->GetRecentCodeNameArray(nSelMarketType, arrCode, arrName);
// 	if(arrCode.GetSize()>0)
// 	{
// 		strCode = arrCode.GetAt(0);
// 	}
// 	else
// 	{
// 		theStdParent->GetFirstCode(strCode, nSelMarketType, nSetp);
// 	}


// 		//theStdParent->GetFirstCode(strCode, nSelMarketType, nSetp);
// 	// 	int nMarketType = theStdParent->GetMarketTypeFromCode(strCode, nRealMarketType);
// 		int nMarketType = nSelMarketType;
//  		if(CMasterTypeInfo::FOREIGN_CHART == nMarketType)
//  			m_strForeignCode = strCode;
//  		else if(CMasterTypeInfo::UPJONG_CHART == nMarketType)
//  			m_strUpjongCode = strCode;

	//m_nMarketType = CMasterTypeInfo::FUOPT_JIPYO_CHART;

	m_strCodeFromOutside = "";

	m_nMarketType = nSelMarketType;

//@��������
	theStdParent->ChangeStockMode(nSelMarketType); 
//@��������
//	if(m_pCtrlCodeInput == m_pCtrlCodeInputUpjong)
//	{
//		m_btChangeUpjong.ShowWindow(SW_SHOW);
//	}
//	else
//	{
//		m_btChangeUpjong.ShowWindow(SW_HIDE);
//	}

	{	//100611-alzioyes: �������� �ڵ����� �������� ó��.
		CString szText;
		m_cbMarket.GetLBText(m_cbMarket.GetCurSel(), szText);
		if(szText.CompareNoCase("�ֽ�")==0) szText="���ɱ׷�";
		else if(szText.CompareNoCase("����")==0) szText="����/�ɼ�";
		else if(szText.CompareNoCase("�ּ�")==0) szText="����/�ɼ�";
		else if(szText.CompareNoCase("��ǰ")==0) szText="��ǰ����";
		

		theStdParent->SetSelFolder(szText);
	}

	((CChartMng*)m_pChartMng)->m_bNeedForceCode = TRUE;
	SetTimer(ID_SHARE_CODE_WAIT, 10, NULL);
//	ChangeMarket(nPosCbMarket, FALSE, TRUE);
}

//@��������
//{{2007.07.23 by LYH
//BOOL CInputBarDlg::SetRecentCode()
//{
//	if(m_nMarketType == STOCK_CHART || m_nMarketType == ELW_CHART)
//	{
//		m_strRecentCode = GetLastKey(1);
//	}
//	else if(m_nMarketType == FOREIGN_CHART)
//		m_strRecentCode = _T("COMP");
//	else if(m_nMarketType == UPJONG_CHART)
////@��������
////		m_strRecentCode = _T("20001");
//		m_strRecentCode = _T("001");
////@��������
//	else if( m_nMarketType == FUTOPT_CHART)
//	{
//		m_strRecentCode = GetLastKey(8);
//		if(m_strRecentCode.GetLength()<1)
//			m_strRecentCode = GetLastKey(12);
//	}
//	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//	else if( m_nMarketType == COMMODITY_CHART)
//		m_strRecentCode = GetLastKey(14);
//	else
//		return FALSE;
//	return TRUE;
//}
//@��������
//@�����߰�
void CInputBarDlg::SetLastHistoryCode()
{
// 	m_strCodeFromOutside = theStdParent->GetLinkInfoData();
// 	if(m_strCodeFromOutside.GetLength())
// 	{
// 		((CChartMng*)m_pChartMng)->m_bNeedForceCode = TRUE;
// 		if(theStdParent->m_nChartType == SINGLE_CHART)
// 		{
// 			int nRealMarketType;
// 			int nMarketType = theStdParent->GetMarketTypeFromCode(m_strCodeFromOutside, nRealMarketType);
// 			if(nMarketType != m_nMarketType)
// 				ChangeMarket(nMarketType, FALSE, TRUE);
// 		}
// 		return;
// 	}

	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
		m_strCodeFromOutside = GetLastHistoryCode(CMasterTypeInfo::STOCK_CHART);
	else if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_strCodeFromOutside = GetLastHistoryCode(CMasterTypeInfo::UPJONG_CHART);
		if(m_strCodeFromOutside.IsEmpty())
			m_strCodeFromOutside = _T("001");
	}
	else if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
		m_strCodeFromOutside = GetLastHistoryCode(CMasterTypeInfo::FUTOPT_CHART);
	else if(m_nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
		m_strCodeFromOutside = GetLastHistoryCode(CMasterTypeInfo::STOCKFUTOPT_CHART);	// 110C6000
//@��������
//	else if(m_nMarketType == ELW_CHART)
//		m_strCodeFromOutside = GetLastHistoryCode(ELW_CHART);
//@��������
	else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_strCodeFromOutside = GetLastHistoryCode(CMasterTypeInfo::FOREIGN_CHART);
		if(m_strCodeFromOutside.IsEmpty())
			m_strCodeFromOutside = _T("DJI@DJC");
	}
	else if(m_nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
	{
//		m_strCodeFromOutside = GetLastHistoryCode(CMasterTypeInfo::FOREIGN_CHART);
//		if(m_strCodeFromOutside.IsEmpty())
//			m_strCodeFromOutside = _T("DJI@DJC");
	}
}
//@�����߰�

void CInputBarDlg::SetCodeInit()
{
	if (m_pChartMng)
	{
		if (((CChartMng*)m_pChartMng)->m_bNeedForceCode == FALSE)
		{
#ifdef _DEBUG
			OutputDebugString("Load �ȳ��� �־ Timer Kill");
#endif
			return;
		}

		CCodeDataFromInputBar CodeData;
		CodeData.m_strCode = m_strCodeFromOutside;
		CodeData.m_lEndTime = GetEndDate();
		CodeData.m_chType = GetType();
		CodeData.m_lTypeNum = GetTypeNumber();
	//	if (m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
	//		m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);


#ifdef _DEBUG
		OutputDebugString("������� �� ���� ��� �ε� ��ȣ ����");
#endif
	}
}

void CInputBarDlg::UpdateBtNCbValue()
{
	if(IsMiniBar())
		return;

	LONG * pValue = NULL;

	char szTemp[64];
	if (DAY_DATA_CHART == m_chType)  // ��
		pValue = m_lValueOfButton;
	else if (HOUR_DATA_CHART == m_chType)	// ��
		pValue = m_lValueOfHourButton;
	else if (MIN_DATA_CHART == m_chType)	// ��
		pValue = m_lValueOfMinButton;
	else if (SEC_DATA_CHART == m_chType)	// ��
		pValue = m_lValueOfSecButton;
	else if (TICK_DATA_CHART == m_chType)	// ƽ
		pValue = m_lValueOfTickButton;

	if (pValue)
	{
		sprintf(szTemp,"%d",pValue[0]);
		m_chkFirst.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",pValue[1]);
		m_chkSecond.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",pValue[2]);
		m_chkThird.SetBtnTitle(szTemp);
		if (HOUR_DATA_CHART != m_chType)
		{
			sprintf(szTemp,"%d",pValue[3]);
			m_chkFourth.SetBtnTitle(szTemp);
//			sprintf(szTemp,"%d",pValue[4]);
//			m_chkFifth.SetBtnTitle(szTemp);
//			sprintf(szTemp,"%d",pValue[5]);
//			m_chkSixth.SetBtnTitle(szTemp);
		}
	}

	if (DAY_DATA_CHART == m_chType)  // ��
	{
		//@�ַθ󿡼��� �Ϻ����� N���� �ȵ�. ������ 1��.
		sprintf(szTemp,"%d", 1);
		m_chkFirst.SetBtnTitle(szTemp);
	}
}

void CInputBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}

void CInputBarDlg::OnBtsetenvir() 
{
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SHOWENVIRDLG);	
}

void CInputBarDlg::OnBtmultichart() 
{
	CString strDataPath = theStdParent->m_strRootPath + "\\Icss\\";	//@Solomon
	((CChartMng*)m_pChartMng)->m_strCodeForMulti = GetCurrentCode();
	((CChartMng*)m_pChartMng)->LoadChart(strDataPath+"DayWeekMonth_ChartInfo.dat");
}

BOOL CInputBarDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{

	int n = LOWORD(wParam);
	if(LOWORD(wParam) == IDC_CTRL_CODEINPUT)
	{
		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE :
			break;
		case CBN_EDITCHANGE :
			break;
		case CBN_SELENDOK :
			break;
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}

void CInputBarDlg::OnSelchangeCodeInput()
{
	//@Solomon:091113SM037 �����߰��� ����߰��Ǵ� ���� �߻��� �� �־ ����.
	CString szCode = m_pCodeInput->GetCode();
	if(theStdParent->m_nScreenID==nScreenID_6401)
	{
		theStdParent->ChangeCode(szCode);
		return;
	}

	HWND  hFocus = NULL;
	{
		CWnd* pWnd = GetFocus();
		if(pWnd)
		{
			pWnd = pWnd->GetParent();
			if(pWnd)
			{
				pWnd = pWnd->GetParent();
				if(pWnd)
				{
					hFocus = pWnd->GetSafeHwnd();
				}
			}
		}
	}
	HWND  hInput = m_pCodeInput->GetSafeHwnd();

	if(m_strCode==szCode)
	{
		//return; //������ skip;
	}

	if(hFocus==hInput)
	{
		theStdParent->DoShareCode(m_pCodeInput->GetCode());
	}

	DoSelchangeCodeInput();
}

void CInputBarDlg::DoSelchangeCodeInput()
{
	if(!m_bSelectAction)
	{
		CString szCode = m_pCodeInput->GetCode();

		SendSymbol(szCode);
		//KHD : Symbol: ù��ȸ 
		SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)szCode);
		//code��Ʈ�ѿ��� ó��. -> 100209 �����ϴ� ������ ����.
		//if(g_pPtUtilMng) g_pPtUtilMng->AddToHistory(theStdParent->m_hPlatform, szCode);
	}
//	else
//		m_bSelectAction = FALSE;
}

void CInputBarDlg::InitUpjongCodeCombo(UINT nUpjongType)
{
}

// ��Ʈ ����/�ҷ�����
void CInputBarDlg::OnBtSaveChart() 
{
	CRect rect;
	CPoint pt;
	GetDlgItem(IDC_BTSAVECHART)->GetWindowRect(rect);
	pt.x = rect.left;
	pt.y = rect.bottom;

	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_LOADSAVECHART,0,(LPARAM)&pt);	
}

void CInputBarDlg::SetViewCount(long nViewCount) 
{
	m_lCntInView[m_chType-'0'] = nViewCount;
}

void CInputBarDlg::GetViewCount(long &nViewCount) 
{
	nViewCount = m_lCntInView[m_chType-'0'];
}

int CInputBarDlg::GetAllCount()
{
	CString strCount;
	m_editCntOfDatAll.GetWindowText(strCount);
	int retVal = atoi(strCount);
	if(retVal<=0)
		return 400;
	else
		return retVal;
}

BOOL CInputBarDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CInputBarDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(rcClient, m_crBk);
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CInputBarDlg::ResetUnitValue(BOOL bValueSet) 
{
	if(bValueSet)
	{
		if(m_nUnitPos >= 0)
		{
			if (MIN_DATA_CHART == m_chType)		// 30��
				m_nUnitValue = m_lValueOfMinButton[m_nUnitPos];
			else if (SEC_DATA_CHART == m_chType)	// ��
				m_nUnitValue = m_lValueOfSecButton[m_nUnitPos];
			else if (TICK_DATA_CHART == m_chType)	// ƽ
				m_nUnitValue = m_lValueOfTickButton[m_nUnitPos];
			else if (HOUR_DATA_CHART == m_chType)	// ��
				m_nUnitValue = m_lValueOfHourButton[m_nUnitPos];
			else if (DAY_DATA_CHART == m_chType)
				m_nUnitValue = m_lValueOfButton[m_nUnitPos];
		}
	}
	else
	{
		BOOL bFind = FALSE;
		if(m_nUnitValue>0)
		{
			m_nUnitPos = -1;

			if (MIN_DATA_CHART == m_chType)		// ��
			{
				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
				{
					if(m_lValueOfMinButton[nPos] == m_nUnitValue)
					{
						m_nUnitPos = nPos;
						bFind = TRUE;
					}
				}
			}
			else if (SEC_DATA_CHART == m_chType)	// ��
			{
				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
				{
					if(m_lValueOfSecButton[nPos] == m_nUnitValue)
					{
						m_nUnitPos = nPos;
						bFind = TRUE;
					}
				}
			}
			else if (TICK_DATA_CHART == m_chType)	// ƽ
			{
				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
				{
					if(m_lValueOfTickButton[nPos] == m_nUnitValue)
					{
						m_nUnitPos = nPos;
						bFind = TRUE;
					}
				}
			}
			else if (HOUR_DATA_CHART == m_chType)	// ��
			{
				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
				{
					if(m_lValueOfHourButton[nPos] == m_nUnitValue)
					{
						m_nUnitPos = nPos;
						bFind = TRUE;
					}
				}
			}
			else if(DAY_DATA_CHART == m_chType)  // ��
			{
				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
				{
					if(m_lValueOfButton[nPos] == m_nUnitValue)
					{
						m_nUnitPos = nPos;
						bFind = TRUE;
					}
				}
			}
			SetType(FALSE);
		}
		if(!bFind)
			m_nUnitPos = -1;
		m_nUnitValue = -1;
	}
}

BOOL CInputBarDlg::SetCodeDragAndDrop(CString strCode, int nGetMarketType/*=-1*/)
{
	return FALSE;
}

//@�����߰�
CString CInputBarDlg::GetLastHistoryCode(int nMarketType)
{
	CString strCode;
	if(m_pDataManager)
	{
		// �ֽ�
		if(nMarketType == CMasterTypeInfo::STOCK_CHART)
		{
			CStringArray arrCode, arrName;
			m_pDataManager->GetRecentCodeNameArray(0, arrCode, arrName);
			if(arrCode.GetSize() > 0)
				strCode = arrCode.GetAt(0);
			else
				strCode = m_pDataManager->GetLastCode("01");

			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, CODE_STOCK_LENGTH);
			}
		}
		// ����
		else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
		{
			CStringArray arrCode, arrName;
			m_pDataManager->GetRecentCodeNameArray(1, arrCode, arrName);
			if(arrCode.GetSize() > 0)
				strCode = arrCode.GetAt(0);
			else
				strCode = m_pDataManager->GetLastCode("02");

			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, UPJONG_LENGTH);
			}
		}
		// ����+�ɼ�+��������+�ֽĿɼ�
		else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
		{
			CStringArray arrCode, arrName;
			m_pDataManager->GetRecentCodeNameArray(2, arrCode, arrName);
			if(arrCode.GetSize() > 0)
				strCode = arrCode.GetAt(0);
		}
		// ����+�ɼ�+��������+�ֽĿɼ�
		else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
		{
			CStringArray arrCode, arrName;
			m_pDataManager->GetRecentCodeNameArray(3, arrCode, arrName);
			if(arrCode.GetSize() > 0)
				strCode = arrCode.GetAt(0);
		}
		// �ؿ�
		else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		{
			strCode = m_pDataManager->GetLastCode("05");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, CODE_STOCK_LENGTH);
			}
		}
		else if(nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
		{
			//@Solomon���⿡ ��ƾ�� ������.
		}
	}

	return strCode;
}
//@�����߰�

void CInputBarDlg::OnEnSetfocusEditFocus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pCodeInput->SetFocus();
}

// �������񿡼� ���õ� ��Ʈ ���� ǥ�� : �������� - ojtaso (20070223)
void CInputBarDlg::InitChartMulitItem(ST_SELCHART* pSelChart)
{
	//05. 09. 08
	SetUseUserInputPeriod(FALSE);

	m_bSettingInitData = TRUE;

	char oldChType = m_chType;
	m_chType = pSelChart->m_cType;

	CString strValue;			
	strValue.Format("%d",pSelChart->m_lCount);
	m_editCntOfDatAll.SetWindowText(strValue);

	UpdateData(FALSE);			

	//07.01.20 by LYH �߰�
	if(oldChType != pSelChart->m_cType)
		UpdateBtNCbValue();

	if(m_chType==TICK_DATA_CHART||m_chType==MIN_DATA_CHART||m_chType==DAY_DATA_CHART)
	{
		long lNumber = 0;
		//BOOL bUpdateUnitPos = FALSE;
		// �߰� 2006.01.19 by LYH 
		BOOL bUpdateUnitPos = FALSE;

		if(m_chType == TICK_DATA_CHART)	//ƽ(0)
		{
			lNumber = atol(pSelChart->m_strTickMinDayNumber);
			m_nTick = lNumber;
			for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
			{
				if(m_lValueOfTickButton[nPos]==lNumber)
				{
					m_nUnitPos = nPos;
					bUpdateUnitPos = TRUE;
				}
			}
		}
		else if(m_chType == MIN_DATA_CHART)//��(1)
		{					
			lNumber = atol(pSelChart->m_strTickMinDayNumber);
			m_nMin	= lNumber;

			if(300 == lNumber)
			{
				m_nUnitPos = 0;
				bUpdateUnitPos = TRUE;
			}
			else
			{
				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
				{
					if(m_lValueOfMinButton[nPos]==lNumber)
					{
						m_nUnitPos = nPos;
						bUpdateUnitPos = TRUE;
					}
				}
			}
		}
		else if(m_chType == DAY_DATA_CHART)//��(1)
		{					
			lNumber = atol(pSelChart->m_strTickMinDayNumber);
			m_nDay	= lNumber;
			for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
			{
				if(m_lValueOfButton[nPos]==lNumber)
				{
					m_nUnitPos = nPos;
					bUpdateUnitPos = TRUE;
				}
			}
		}

		if(!bUpdateUnitPos)
		{
			m_nUnitPos = -1;
		}
		//m_nUnitPos = -1; 
		SetType(FALSE);

		CString strCount;
		strCount.Format(_T("%d"), lNumber);

		if(m_chType==TICK_DATA_CHART)	//ƽ(0)
		{				
			//m_nUnitPos = 0;
			m_editPeriod.SetWindowText(strCount);
		}
		else if(m_chType==MIN_DATA_CHART)				//��(1)
		{				
			//m_nUnitPos = 1;
			m_editPeriod.SetWindowText(strCount);
		}
		else if(m_chType==DAY_DATA_CHART)
		{				
			//m_nUnitPos = 2;
			m_editPeriod.SetWindowText(strCount);
		}
		else
			m_editPeriod.SetWindowText("1");
	}
	else if (m_chType == HOUR_DATA_CHART)		// ��
	{
		long lNumber = 0;
		BOOL bUpdateUnitPos = FALSE;

		lNumber = atol(pSelChart->m_strTickMinDayNumber);
		//				lNumber = lNumber / 60;
		m_nHour = lNumber;

		for(int nPos=0;nPos < 3;nPos++)
		{
			if(m_lValueOfHourButton[nPos]==lNumber)
			{
				m_nUnitPos = nPos;
				bUpdateUnitPos = TRUE;
			}
		}

		if(!bUpdateUnitPos)
		{
			m_nUnitPos = -1;
		}
		SetType(FALSE);

		CString strCount;
		strCount.Format(_T("%d"), lNumber);

		m_editPeriod.SetWindowText(strCount);
	}
	else	////
	{
		SetType(FALSE);				
		m_editPeriod.SetWindowText("1");
	}			

	CString strSelCode = pSelChart->m_strItemCode;
	int nRealMarketType;
	if(ChangeMarket(theStdParent->GetMarketTypeFromCode(strSelCode, nRealMarketType)))
		InitBaseInfo(NULL, FALSE);

	m_strOldCode = GetCurrentCode();
	SetCurrentCode(strSelCode);	
	SetCurCodeNameToControl(strSelCode);

	if(m_nMarketType != CMasterTypeInfo::FOREIGN_CHART)
	{
		STINPUTCODEDATA2 stInputData;
		stInputData.m_strCode = GetCurrentCode();
		stInputData.m_bDontPutInShared = FALSE;
		stInputData.m_bRequestWithoutHesitation = FALSE;
		SendMessage(UMSG_INPUTCODETOINPUTBAR, (WPARAM)&stInputData, 0);

		m_bSelectAction = TRUE;
		CString strCodeInputCode = m_pCodeInput->GetCode();
		
		if(GetCurrentCode().GetLength() && strCodeInputCode != GetCurrentCode())
		{
			SetShareCodeChanged(TRUE);
			//m_ctrlCodeInput.SetDataText(GetCurrentCode());
			m_pCodeInput->SetCode(GetCurrentCode()); //@solomon
			DoSelchangeCodeInput();
		}

		SetShareCodeChanged(FALSE);
		m_bSelectAction = FALSE;
	}
	else
	{
		CString strCode = m_pCodeInput->GetCode();
		strCode.TrimLeft(); strCode.TrimRight();
		if(strCode.GetLength() == 0)
		{
			m_bSelectAction = TRUE;
			CString strCodeInputCode = m_pCodeInput->GetCode();
			if(GetCurrentCode().GetLength() && strCodeInputCode != GetCurrentCode())
			{
				SetShareCodeChanged(TRUE);
				//m_ctrlCodeInput.SetDataText(GetCurrentCode());
				m_pCodeInput->SetCode(GetCurrentCode()); //@solomon
				DoSelchangeCodeInput();
			}
			m_bSelectAction = FALSE;
		}
	}

	m_bSettingInitData = FALSE;	
}

// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
BOOL CInputBarDlg::GetReplayChartMode()
{
	return m_bUseReplayChartMode;
}

// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
void CInputBarDlg::SetReplayChartMode(BOOL bUseReplayChartMode)
{
	m_bUseReplayChartMode = bUseReplayChartMode;
}

// ���������� Total Count�� ��Ƽ��Ʈ�� �˷��� : �������� - ojtaso (20070704)
void CInputBarDlg::SetMultiTotalCount(int nTotalCount)
{
	CString strCount;
	strCount.Format(_T("%d"), nTotalCount);

	SetDlgItemText(IDC_STC_TOTALCNT, strCount);

	CString strZoomValue;
	m_editZoomValue.GetWindowText(strZoomValue);

	//>>[A00000529]alzioyes:20110524 �����̰� Ư����Ʈ�� ������ �״� ���� �־ ����.
	// 	//>>JS.Kim_20100918.  ȭ�� �����ϸ鼭 Total Count�� 0���� ���� ��찡 �־� ScrollView Count�� �̻�����
	// 	//if(atoi(strZoomValue)>nTotalCount)
	// 	if( nTotalCount > 0 && atoi(strZoomValue) > nTotalCount )
	// 	//<<
	// 	{
	// 		strZoomValue.Format("%d", nTotalCount);
	// 		//m_editZoomValue.SetWindowText(strZoomValue);
	// 		//>>JS.Kim_20100918.  ��ȸ����Ÿ�� ���� ��� ��ũ�Ѱ����� �������� �� ������ �������� ������ ��ȸ�ϴ� ��쵵 ��ũ�Ѱ����� �������� ���� ����
	// 		//SetEditZoomValue(strZoomValue, TRUE);
	// 		SetEditZoomValue(strZoomValue, FALSE);
	// 		//<<
	// 	}
	if(atoi(strZoomValue)>nTotalCount)
	{
		strZoomValue.Format("%d", nTotalCount);
		m_editZoomValue.SetWindowText(strZoomValue);
	}
	//<<[A00000529]alzioyes:20110524
}

// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
void CInputBarDlg::EnableRequestMoreButton(BOOL bEnable)
{
	if(m_btRequestMore.GetSafeHwnd())
		m_btRequestMore.EnableWindow(bEnable);
}

void CInputBarDlg::ChangeSkinColor()
{
	m_crBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.DeleteObject();
	m_brBkColor.CreateSolidBrush(m_crBk);
	Invalidate();
}

void CInputBarDlg::SaveInputBar()
{
	//=================================================================================
	//2005. 12. 08
	//������ ���¿��� �����..
	//Ȯ�� ���ٰ� ������ �� ���̰� �����Ǿ� �ִ� ��� 
	//���̴� ������ INI ���Ͽ� �����ϵ��� �ϱ� ���� �߰��Ѵ�.
	//=================================================================================
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	CString strConfig = m_pStdDialog->m_strConfig;
	if(m_bUseReplayChartMode && m_nCurrentMode == REPLAY_ITEM)
	{		
		if(m_ExChangeBeforeInfo.bShowExtToolBar)
		{
			::WritePrivateProfileString("Frame","ShowExtInputBarDlg","1",strConfig);
		}
	}
	//=================================================================================

	SaveValue();
}

void CInputBarDlg::RequestReplayMode()
{
		// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	if(m_nCurrentMode == REPLAY_ITEM) return; //���� ���� Return
	
	// �������� �߰� UI : �������� - ojtaso (20070522)
	if(m_bUseReplayChartMode)
	{
		//=================================================================================
		// 2005.12.08
		// ���Ⱑ ������ �������� �����Ѵ�.
		// ����� ��� ���ҵ� ��Ʈ�� �� �����϶��� �����ϴ�.
		//=================================================================================
		if( !theStdParent->CheckDataTypeForReplay() && m_nMarketType == CMasterTypeInfo::STOCK_CHART)
		{
			AfxMessageBox("������Ʈ�� ������ ��Ʈ�϶��� ������ �����մϴ�");
			m_cmbChartViewType.SetCurSel(m_nCurrentMode);
			return;
		}
		
		int nReason = 0;
		if(nReason = ((CChartMng*)m_pChartMng)->IsAvailableForReplay())
		{
			if(nReason == 1)
				AfxMessageBox("�������� �ֽİ� ���� ���� �����մϴ�.");
			else if(nReason == 2)
				AfxMessageBox("������Ʈ�� �� ���� �����մϴ�");

			m_cmbChartViewType.SetCurSel(m_nCurrentMode);
			return;
		}

		if(m_nMarketType != CMasterTypeInfo::STOCK_CHART && m_nMarketType != CMasterTypeInfo::FUTOPT_CHART)
		{
			AfxMessageBox("�������� �ֽİ� ���� ���� �����մϴ�.");
			m_cmbChartViewType.SetCurSel(m_nCurrentMode);
			return;
		}
	}

	SetMode(REPLAY_ITEM); // ��Ʈ ��� ����
}

/*
LRESULT CInputBarDlg::OnTestTR(WPARAM wParam, LPARAM lParam)
{
	m_strCodeFromOutside = "005930";
	m_chType = '2';
	m_editPeriod.SetWindowText("1");

	SetCodeInit();

	return 0L;
}
*/

void CInputBarDlg::OnCheckDow() 
{
	// TODO: Add your control notification handler code here
	OnChkForeignMainCode(IDC_CHECK_DOW);
}

void CInputBarDlg::OnCheckNasdaq() 
{
	// TODO: Add your control notification handler code here
	OnChkForeignMainCode(IDC_CHECK_NASDAQ);
}

void CInputBarDlg::OnCheckNikei() 
{
	// TODO: Add your control notification handler code here
	OnChkForeignMainCode(IDC_CHECK_NIKEI);
}

void CInputBarDlg::OnCheckSp500() 
{
	// TODO: Add your control notification handler code here
	OnChkForeignMainCode(IDC_CHECK_SP500);
}

void CInputBarDlg::OnCheckTwd() 
{
	// TODO: Add your control notification handler code here
	OnChkForeignMainCode(IDC_CHECK_TWD);
}

void CInputBarDlg::SetShareCodeChanged(BOOL bShareCodeChanged)
{
	m_bIsShareCodeChanged = bShareCodeChanged;
}

void CInputBarDlg::_DoSetPosCodeCodeName(CRect& rcCode)
{
	int nX=1, nY=rcCode.top;
	CRect tRc;
	INT nMinSX = 90;

	switch(theStdParent->m_nChartType)
	{
		case MULTI_CHART:
			m_stJongName.SetWindowPos(NULL, 0, 0, 110, 20, SWP_NOMOVE|SWP_NOZORDER );
			m_pwndSymbol->SetWindowPos(NULL, 0, 0, 110, 20, SWP_NOMOVE|SWP_NOZORDER );	
			break;
		case SINGLE_CHART:
			{
		
			}
			break;
		case MINI_MULTI_CHART:
			//nX = m_rectCmb.right + 1;			
			m_cbMarket.GetWindowRect(&tRc);
			nX = tRc.Width() + 1;
			m_pCodeInput->SetWindowPos(NULL, nX, rcCode.top, nMinSX, 20, SWP_NOZORDER);
			//nX += (nMinSX+2);
			//m_stJongName.SetWindowPos(NULL, nX, nY, 60, 20, SWP_NOZORDER );
			break;
		case MINI_SINGLE_CHART:
			m_pCodeInput->SetWindowPos(NULL, 1, rcCode.top, nMinSX, 20, SWP_NOZORDER);
			//nX += (nMinSX+2);
			//m_stJongName.SetWindowPos(NULL, nX, nY, 60, 20, SWP_NOZORDER );
			break;
		case MINI_SINGLE2_CHART:
			m_pCodeInput->SetWindowPos(NULL, 1, rcCode.top, nMinSX, 20, SWP_NOZORDER);
			//nX += (nMinSX+2);
			//m_stJongName.SetWindowPos(NULL, nX, nY, 60, 20, SWP_NOZORDER );
			break;
		case MINI_DWM_CHART:
			m_pCodeInput->SetWindowPos(NULL, 1, rcCode.top, nMinSX, 20, SWP_NOZORDER);
			//nX += (nMinSX+2);
			//m_stJongName.SetWindowPos(NULL, nX, nY, 60, 20, SWP_NOZORDER );
			break;
	}

	if(theStdParent->m_nChartType == SINGLE_CHART)
	{
		UINT nID[] = {IDC_STJONGNAME,
			
			IDC_CHKDAY			,
			IDC_CHKWEEK			,
			IDC_CHKMONTH		,
			IDC_CHKYEAR         , 
			IDC_CHKMINUTE		,
			IDC_CHKSEC          , 
			IDC_CHKTICK			,

			IDC_CHKFIRST        , 
			IDC_CHKSECOND       , 
			IDC_CHKTHIRD        , 
			IDC_CHKFOURTH       , 
//			IDC_CHKFIFTH        , 
//			IDC_CHKSIXTH        , 	
			IDC_EDITPERIOD		,
			IDC_BTPERIOD		,

			IDC_EDITZOOMVALUE	,
			IDC_SPINZOOMVALUE	,
			IDC_STATICDIV		,
			IDC_STC_TOTALCNT	,
			IDC_EDITCNTOFDATALL	,
			IDC_BTREQUESTDATMORE,
			IDC_BTSAVECHART,
			IDC_CHK_QUERYOPT
		};
		
		int nCnt = sizeof(nID) / sizeof(UINT);
		CRect rectComp, ctlRect;
		
		int nWidth = 0;int nOffset;
		for(int i = 0; i < nCnt ; i++)
		{
			ctlRect.left	= 0;
			ctlRect.top		= 0;
			ctlRect.right	= 0;
			ctlRect.bottom	= 0;
			
			if(nID[i] == IDC_STJONGNAME)
			{
				CRect rect;
				CWnd* pChkCtrl = GetDlgItem(IDC_CHKLEFT);
				if(pChkCtrl)
				{
					pChkCtrl->GetWindowRect(rect);
					ScreenToClient(rect);
					m_pCodeInput->SetWindowPos(NULL, rect.right+1, rect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
				}
				
				m_stJongName.GetWindowRect(rect);
				ScreenToClient(rect);
				m_stJongName.MoveWindow(rect.left+58 , rect.top, rect.Width()+25, rect.Height());
				m_pwndSymbol->MoveWindow(rect.left+58 , rect.top, rect.Width()+25, rect.Height());

				
				GetDlgItem(nID[i])->GetWindowRect(ctlRect);		
				ScreenToClient(ctlRect);
				m_rectJongName = ctlRect;				
				nOffset = ctlRect.right+2;
				
				continue;
			}		
	
			
			GetDlgItem(nID[i])->GetWindowRect(ctlRect);		
			ScreenToClient(ctlRect);
			nWidth = ctlRect.Width();
			

			ctlRect.left	= nOffset;
			ctlRect.right	= ctlRect.left + nWidth;			
			
			
			GetDlgItem(nID[i])->MoveWindow(ctlRect);
			nOffset = ctlRect.right;
		}
	}
}

BOOL CInputBarDlg::_DoPreCheckCodeCtrlUnit()
{
	if(m_bSetFixUnit) return TRUE;

	CString szScreenNo = theStdParent->GetScreenNo();

	int nScreenNo = atoi(szScreenNo);
	switch(nScreenNo)
	{
		case 7112:
		case 7125:
			m_pCodeInput->SetFixUnit("�ֽ�");
			m_nMarketType = CMasterTypeInfo::STOCK_CHART;
			break;
		case 7113:
		case 712:
			m_pCodeInput->SetFixUnit("����");
			m_nMarketType = CMasterTypeInfo::UPJONG_CHART;
			break;
		case 7114:
		case 508:
		case 7121:
			m_pCodeInput->SetFixUnit("�����ɼ�");
			m_nMarketType = CMasterTypeInfo::FUTOPT_CHART;
			break;
		case 7138:
			m_pCodeInput->SetFixUnit("�ֽļ���");
			m_nMarketType = CMasterTypeInfo::STOCKFUTOPT_CHART;
			break;
		case 7116:
			m_pCodeInput->SetFixUnit("�ؿ�");
			m_nMarketType = CMasterTypeInfo::FOREIGN_CHART;
			break;
		case 7115:
		//case 373: -> �̰� ��.
			m_pCodeInput->SetFixUnit("��ǰ����");
			m_nMarketType = CMasterTypeInfo::COMMODITY_CHART;
			break;
		default:
			return FALSE;
	}

	m_bSetFixUnit = true;
	return TRUE;
}

//����Ÿ�Ժ��� �ڵ嵵���� Ÿ�� ����.
void CInputBarDlg::_DoChangeCodeCtrlUnit(int nMarketType, LPCSTR szCode)
{
	if(_DoPreCheckCodeCtrlUnit()==TRUE)
		return;

	CString strCode = CString(szCode);
	if(strCode.IsEmpty() == TRUE && nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
	}
	if (strCode.GetLength() > 0)
	{
		IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
		if (pMasterDataManager)
		{
			int nCodeType = pMasterDataManager->GetCodeTypeFromCode(strCode);
			m_pCodeInput->SetUnit(nCodeType);
			return ;
		}
	}

	switch(nMarketType)
	{
	case CMasterTypeInfo::STOCK_CHART:			m_pCodeInput->SetUnit("�ֽ�");		break;
	case CMasterTypeInfo::UPJONG_CHART:			m_pCodeInput->SetUnit("����");		break;
	case CMasterTypeInfo::FUTOPT_CHART:			m_pCodeInput->SetUnit("�����ɼ�");	break;
	case CMasterTypeInfo::STOCKFUTOPT_CHART:	m_pCodeInput->SetUnit("�ֽļ���");	break;
	case CMasterTypeInfo::FOREIGN_CHART:		m_pCodeInput->SetUnit("�ؿ�");		break;
	case CMasterTypeInfo::FUOPT_JIPYO_CHART:	m_pCodeInput->SetUnit("FO��ǥ");	break;
	case CMasterTypeInfo::COMMODITY_CHART:		m_pCodeInput->SetUnit("��ǰ����");	break;
	default:	break;
//	default:									m_pCodeInput->SetUnit("�ֽ�");		break;
	}
}

void CInputBarDlg::OnSelendokFOHistoryBtn() 
{
	CString strCode = m_pFOHisBtn->GetCode();
	if(strCode.GetLength()>0)
	{
		theStdParent->m_xPartnerSite.RecieveKeyData(IPartnerSite::IPS_RK_codeHidDlg, strCode);
	}	
}

void CInputBarDlg::_DoCheckMarketTypeFromScreen(int &nMarketType)
{
	CString szCFG;
	szCFG.Format("%s\\Icss\\PctrMng.cfg", theStdParent->m_strRootPath);

	CString szScreenNo = theStdParent->GetScreenNo();
	nMarketType = ::GetPrivateProfileInt("Market", (LPCSTR)szScreenNo, 0, szCFG);
}

int CInputBarDlg::GetMarketStrToIndex(CString strMarket)
{
	if (strMarket.CompareNoCase(_T("�ֽ�")) == 0)
		return CMasterTypeInfo::STOCK_CHART;
	else if (strMarket.CompareNoCase(_T("����")) == 0)
		return CMasterTypeInfo::UPJONG_CHART;
	else if (strMarket.CompareNoCase(_T("����")) == 0)
		return CMasterTypeInfo::FUTOPT_CHART;
	else if (strMarket.CompareNoCase(_T("�ּ�")) == 0)
		return CMasterTypeInfo::STOCKFUTOPT_CHART;
//	20130515 �̻�� : �ؿ����� ����.(issue No - 0006748)
//	else if (strMarket.CompareNoCase(_T("�ؿ�")) == 0)
//		return CMasterTypeInfo::FOREIGN_CHART;
	else if (strMarket.CompareNoCase(_T("FO����")) == 0)
		return CMasterTypeInfo::FUOPT_JIPYO_CHART;
	else if (strMarket.CompareNoCase(_T("��ǰ")) == 0)
		return CMasterTypeInfo::COMMODITY_CHART;
	else if (strMarket.CompareNoCase(_T("������ǥ")) == 0)
		return CMasterTypeInfo::MARKET_CHART;

	return CMasterTypeInfo::NONE_CHART;
}

int CInputBarDlg::GetMarketIdxToCBIdx(int nIndex)
{
	int nCnt = m_cbMarket.GetCount();
	for (int i=0; i<nCnt; i++)
	{
		if (nIndex == m_cbMarket.GetItemData(i))
			return i;
	}

	return 0;
}

int CInputBarDlg::InitMarketCombo()
{
	if( !m_cbMarket.GetSafeHwnd() || !::IsWindow(m_cbMarket.GetSafeHwnd()) )
	{
		AfxMessageBox("�ý��۸��ҽ��� �����Ͽ� ȭ���� ��� �� �����ϴ�. �ٸ� ȭ���� �����ϰ� �����Ͽ� �ֽʽÿ�.");
		return -1;
	}

	IMainInfoManager0002* pMng = (IMainInfoManager0002*)AfxGetPctrInterface(UUID_IMainInfoManager);
	if (!pMng)
		return 0;
	
	int nMode = pMng->GetHtsMode();

	if (theStdParent->m_nChartType == MINI_MULTI_CHART)
	{
		char *pszTitle[] = {_T("�ֽ�"), _T("����"), _T("�����ɼ�"), NULL};
		int nIndex = 0;
		while (pszTitle[nIndex] != NULL)
		{
			m_cbMarket.AddString(pszTitle[nIndex]);
			m_cbMarket.SetItemData(nIndex, GetMarketStrToIndex(pszTitle[nIndex]));
			nIndex++;
		}
	}
	else
	{
		if (nMode)
		{
//	20130515 �̻�� : �ؿ����� ����.(issue No - 0006748)
//			char *pszTitle[] = {_T("�ֽ�"), _T("����"), _T("����"), _T("�ּ�"),_T("�ؿ�"), _T("FO����"), _T("��ǰ"), NULL};
			char *pszTitle[] = {_T("�ֽ�"), _T("����"), _T("����"), _T("�ּ�"), _T("FO����"), _T("��ǰ"), NULL};
			int nIndex = 0;
			while (pszTitle[nIndex] != NULL)
			{
				m_cbMarket.AddString(pszTitle[nIndex]);
				m_cbMarket.SetItemData(nIndex, GetMarketStrToIndex(pszTitle[nIndex]));
				nIndex++;
			}
		}
		else
		{
//	20130515 �̻�� : �ؿ����� ����.(issue No - 0006748)
//			char *pszTitle[] = {_T("�ֽ�"), _T("����"), _T("����"), _T("�ּ�"), _T("�ؿ�"), _T("��ǰ"), NULL};
			char *pszTitle[] = {_T("�ֽ�"), _T("����"), _T("����"), _T("�ּ�"), _T("��ǰ"), NULL};
			int nIndex = 0;
			while (pszTitle[nIndex] != NULL)
			{
				m_cbMarket.AddString(pszTitle[nIndex]);
				m_cbMarket.SetItemData(nIndex, GetMarketStrToIndex(pszTitle[nIndex]));
				nIndex++;
			}
		}
	}

	return 1;
}

BOOL CInputBarDlg::GetOldFututureCodeName(LPCSTR szCode, CString& szCodeName)
{
	if(m_pFOHisBtn)
	{
		szCodeName = m_pFOHisBtn->GetCodeName(szCode);
		return TRUE;
	}
	return FALSE;
}

void CInputBarDlg::_DoReplyResourceCheck()
{
	// ��7���� ���ҽ��� ���þȵǴ� ��� �־� �߰�.
	{
		//IDC_CMB_SPEED
		if(m_cmbReplaySpeed.GetCount()==0)
		{
			m_cmbReplaySpeed.AddString("X  1");
			m_cmbReplaySpeed.AddString("X  2");
			m_cmbReplaySpeed.AddString("X  4");
			m_cmbReplaySpeed.AddString("X  8");
			m_cmbReplaySpeed.AddString("X 16");
			m_cmbReplaySpeed.AddString("X 32");
			m_cmbReplaySpeed.AddString("X 64");

			m_cmbReplaySpeed.SetCurSel(0);
		}
		
		//	IDC_CMB_DAY
		if(m_cmbDayBefore.GetCount()==0)
		{
			m_cmbDayBefore.AddString("0");
			m_cmbDayBefore.AddString("1");

			m_cmbDayBefore.SetCurSel(0);
		}
		
		//	IDC_CMB_HOUR
		if(m_cmbReplayHour.GetCount()==0)
		{
			m_cmbReplayHour.AddString("09");
			m_cmbReplayHour.AddString("10");
			m_cmbReplayHour.AddString("11");
			m_cmbReplayHour.AddString("12");
			m_cmbReplayHour.AddString("13");
			m_cmbReplayHour.AddString("14");

			m_cmbReplayHour.SetCurSel(0);
		}
		
		//	IDC_CMB_MIN
		if(m_cmbReplayMin.GetCount()==0)
		{
			m_cmbReplayMin.AddString("00");
			m_cmbReplayMin.AddString("10");
			m_cmbReplayMin.AddString("20");
			m_cmbReplayMin.AddString("30");
			m_cmbReplayMin.AddString("40");
			m_cmbReplayMin.AddString("50");

			m_cmbReplayMin.SetCurSel(0);
		}
	}
}


int CInputBarDlg::GetComboMarket()
{
	int nSelMarketType = m_cbMarket.GetItemData(m_cbMarket.GetCurSel());
	return nSelMarketType;
}
//KHD : �ֽ�. ���� ��Ʈ������ ���� ������ ��ȸ �����ϰ� ���� 
void CInputBarDlg::OnChkQueryOpt() 
{
	UpdateData(TRUE);
	CExtInputBarDlg *pExInput = NULL;
	pExInput = (CExtInputBarDlg*)((CChartMng*)m_pChartMng)->GetpExtInputBarDlg();
	pExInput->SetQueryOpt(m_chkQueryOpt.GetCheck());
	if(m_pChartMng)
		m_pChartMng->SendMessage(UMSG_REQUEST_FORQUERYOPT);
}

void CInputBarDlg::EnableQueryOpt(BOOL bEnable)
{
	m_chkQueryOpt.EnableWindow(bEnable);
}
void CInputBarDlg::SetQueryOpt(int nCheck)
{
	m_chkQueryOpt.SetCheck(nCheck);
}

void CInputBarDlg::OnBtJumun() 
{
	//	m_pChartMng->SendMessage(UMSG_SHOWENVIRDLG);
	IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
	IPartnerUtilManager* pPtUtilMng = NULL;

	if (pPtSvrMng)
	{
		pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);

		CString strScrArg,strDomino;

		strDomino.Format(_T("%s||%s|%s||%s"), m_pCodeInput->GetCode(), "0","0", "03");	//�ֹ����� �߰�
		if(theStdParent->m_nScreenID== 7112)
		{
			strScrArg.Format(_T("IB101200 /p5 /t/d catch_domino_data\t%s\n") , strDomino);

		//	strScrArg.Format("%s /t/s/p5/d1301\t%s", "IB101200", m_pCodeInput->GetCode());
			if (pPtUtilMng)
				pPtUtilMng->OpenScreen(GetParent()->GetParent()->GetParent()->GetParent()->GetSafeHwnd(), strScrArg);

		}
		else if(theStdParent->m_nScreenID== 7114 )
		{
			strScrArg.Format(_T("IB301200 /p5 /t/d catch_domino_data\t%s\n") , strDomino);

			if (pPtUtilMng)
				pPtUtilMng->OpenScreen(GetParent()->GetParent()->GetParent()->GetParent()->GetSafeHwnd(), strScrArg);

		}
	}
}
//�ɺ����� 

void CInputBarDlg::SetInitSymbol()
{
	// ���� symbol
	CString	strPath;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*);
	
	strPath.Format("%s\\dev\\cx_symbol.dll", theStdParent->m_strRootPath);
	m_hiSymbol = AfxLoadLibrary(strPath);
	if (m_hiSymbol != NULL)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
		if (axCreate != NULL)
		{
			struct	_param	param;
			
			int nkey = TRKEY_SYMBOL;
			param.key   = nkey;
			param.name  = _T("17413");
			param.rect  = CRect(0, 0, 130, 20);
			param.fonts = "����ü";
			param.point = 9;
			param.style = 1;
			param.tRGB  = 63;
			param.pRGB  = 181;
			param.options = _T("/a89/b91/c92/d69/i99");
			
			m_pwndSymbol = (*axCreate)(GetParent()->GetParent()->GetParent()->GetParent(), &param);
			m_pwndSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			
			//���ȴ� �ö���� �ɺ� ������� ���� �ذ�
			m_pwndSymbol->SetParent(this);
			
			if (m_pwndSymbol == NULL)
			{
				AfxFreeLibrary(m_hiSymbol);
				m_hiSymbol = NULL;
			}
		}
	}
}

void CInputBarDlg::ParsingSymbol(char* datb)
{
	int	value;
	CString	strCode;
	
	strCode  = CString(datb);
	value = strCode.Find('\t');
	if (value != -1)
	{
		WPARAM	wParam;
		CString tempCode;
		tempCode  = strCode.Left(value++);
		
		if( tempCode.GetLength() ==7 )
		{
			strCode   = strCode.Mid(value);
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), (strCode.GetLength()));
			m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)strCode.operator LPCTSTR());
		}
		else
		{
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), strCode.GetLength());
			m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)strCode.operator LPCTSTR());
		}
	}
}

void CInputBarDlg::SendSymbol(CString strCode)
{
	if(IsMiniBar()== TRUE) return; 
	CString strData;
	strData.Format("1301%c%s\t1021\t17413\t", 0x7f, strCode);
	
	char*	wb;
	struct	_userTH* userth;
	
	wb = new char[L_userTH + strData.GetLength()];
	FillMemory(wb, L_userTH + strData.GetLength(), ' ');
	userth = (struct _userTH *)wb;
	CopyMemory(userth->trc, "POOPPOOP", sizeof(userth->trc));
	userth->key  = TRKEY_SYMBOL;
	userth->stat = US_OOP ;
	CopyMemory(&wb[L_userTH], (char *)strData.operator LPCTSTR(), strData.GetLength());
	
	GetParent()->GetParent()->GetParent()->GetParent()->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM)wb);
	
	delete[] wb;
}
//END 

//--> 0023618: [7112] �ֽ�������Ʈ ȭ�� - �������� ��� ��ư �� ���� ��� �߰� ��û
void CInputBarDlg::OnBtnGwansim() 
{
	//100311 ���ɵ�ϰ����� ���� ���ɸ���Ʈ ���̰� ����.
	BOOL bIsCan = TRUE;
	CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
	
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	int nCodeType = pMasterDataManager->GetCodeTypeFromCode(strCode);
	switch(nCodeType)
	{
	case CDRCodeTypeInfo::CODE_US_ITEM:
	case CDRCodeTypeInfo::CODE_FUOPT_JIPYO:
	case CDRCodeTypeInfo::CODE_FUOPT_COMMODITY:
		bIsCan = FALSE;
		break;
	}
	
    CMenu menu, pMenuChild;
	
	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
    menu.CreatePopupMenu();
	
	menu.AppendMenu(MF_STRING,101,"���ɵ��");
	menu.AppendMenu(MF_SEPARATOR, (UINT)110, "");
	
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(!pGateMng)
		return;
	
	CStringArray arrKey, arrName;
	//������ �׷��̸��� �����Դµ�, ������ Key�� �����´�.
	
	CStdDialog* pStdParent = (CStdDialog*)GetParent();
	//<<20100413_JS.Kim ����ó��
	if( !pStdParent || !pStdParent->m_pGwanMng )
		return;
	//>>
	
	pStdParent->m_pGwanMng->GetGwansimGroupName(arrKey, arrName);	
	if(bIsCan)
	{
		CString strGroup;
		for(int wIndexItem=0;wIndexItem<arrName.GetSize();wIndexItem++)
		{
			strGroup = arrName.GetAt(wIndexItem);
			menu.AppendMenu(MF_STRING,200+wIndexItem,strGroup);
		}
	}
	
	CRect rect;
	m_btnGwanSim.GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
    menu.DestroyMenu();	
	
	if(nRetValue == 101)
	{
		//����� �� �ִ� �ڵ����� üũ
		if (!strCode.IsEmpty() )
		{
			strCode = "";
			pStdParent->m_pGwanMng->ShowGwansimDlg(this, strCode);
		}
		else
		{
			pStdParent->m_pGwanMng->ShowGwansimDlg(this, strCode);
		}
	}
	if(nRetValue >= 200)
	{
		CString strKey;
		strKey = arrKey.GetAt(nRetValue-200);		
		if(strKey.GetLength() == 0)
			return;	
		pStdParent->m_pGwanMng->ADDGwasimItem(strKey, strCode);
	}
}
//<--
