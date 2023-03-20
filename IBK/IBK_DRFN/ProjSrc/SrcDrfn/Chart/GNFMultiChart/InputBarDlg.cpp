// InputBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputBarDlg.h"
#include "StdDialog.h"
#include "./define/ChartMsgDef.h"
//#include "SetUnitDlg.h"
#include "SetCycleDlg.h"
#include "CodeDataFromInputBar.h"
//#include "./include/LocalUtil.h"
#include "./define/MessageDefine.h"
#include "../../inc/IMasterDataMng.h"
#include "../../../SrcSite/include/axisfire.h"

#include <math.h>
#include ".\inputbardlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern IMainDllObj	* g_pMainDll;
const UINT RMSG_CREATEOPENSHAREDATA	= ::RegisterWindowMessage(_T("RMSG_CREATEOPENSHAREDATA"));
const UINT RMSG_WRAP_GETINFO = ::RegisterWindowMessage(_T("RMSG_WRAP_GETINFO"));


/////////////////////////////////////////////////////////////////////////////
// CInputBarDlg dialog
#define WM_POPUPLIST_SELITEM WM_USER + 77
#define WMU_GET_LAST_KEY			WM_USER + 2237	// AupAgent���� MainFrame�� ������ ��ȸ�� Key�� ��û�Ѵ�.

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
	m_bChkRevised = FALSE;
	m_bChkLog = FALSE;
	m_checkOrderLink = FALSE;
	//}}AFX_DATA_INIT
	//m_pctrlCode = NULL;
	m_dlgPopupListForHistory = NULL;
	//m_pctrlCode = NULL;
	m_pnLeftBarType = NULL;
	m_pChartMng = NULL;
	m_bSettingInitData = FALSE;
	m_chType = DAY_DATA_CHART;
	m_nUnitPos = -1;
	m_lBaseDesk = NULL;
	m_bIsMiniBar = FALSE;
	m_nPosCbMarketOld = 0;
	::ZeroMemory(m_lValueOfButton,sizeof(m_lValueOfButton));			// ��
	::ZeroMemory(m_lValueOfMinButton,sizeof(m_lValueOfMinButton));		// ��
	::ZeroMemory(m_lValueOfTickButton,sizeof(m_lValueOfTickButton));	// ƽ
	::ZeroMemory(m_lValueOfCombo,sizeof(m_lValueOfCombo));

	m_lValueOfHourButton[0] = 1;
	m_lValueOfHourButton[1] = 2;
	m_lValueOfHourButton[2] = 3;

	for(int n = 0;n<8;n++)//ƽ �� �� �� �� �� 10�� 30�� 
	{
		m_lCntInView[n] = 200;
	}

	m_nMarketType	= CMasterTypeInfo::STOCK_CHART;
//	m_pDBMgr		= NULL; //2005. 05. 12


	m_nCurSelCol = 0; 
	m_nCurSelRow = 0;


	m_nTick = 1;
	m_nDay	= 1;
	m_nMin	= 1;

	m_strCfgFile		= _T("");
	m_strCodeFromOutside = _T("");
	m_strCode			= _T("");
	m_strOldCode		= _T("");

	m_nFutOptType	= KOSPI_FUTOPT		; //2005. 07. 08
//	m_dwDataKind	= CODE_FUTOPT		; //2005. 07. 08

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
	m_nFixZoomValue = 200;
	m_nUpjongType = 100;
	m_nUnitValue = -1;

//	m_pCtrlCodeInput = NULL;
//	m_pCtrlCodeInputStock = NULL;
//	m_pCtrlCodeInputFuture = NULL;
//	m_pCtrlCodeInputUpjong = NULL;
//	m_pCtrlCodeInputELW = NULL;
//	m_pCtrlCodeInputForeign = NULL;
	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//	m_pCtrlCodeInputCommodity = NULL;

	m_bChkSaveJipyoOneFile = TRUE;
	m_bChkLoadSaveCode = TRUE;

	m_bSelectAction = FALSE;	// ��Ʈ ������ ����Ǿ� �����ڵ尡 ����Ǵ� ��� : �������� - ojtaso (20070323)
	m_bUseReplayChartMode = TRUE; 	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)

	m_bReceiveDataSetCode = FALSE;	// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)

//@��������
//	//{{2007.07.23 by LYH
//	m_strRecentCode = "";
//@��������
//@�����߰�
	m_nCurrentMode = CHANGE_ITEM;
	m_bIsShareCodeChanged = FALSE;
	m_rectCmb = CRect(0,0,0,0);
//@�����߰�

	m_hKSJongName = NULL;

	m_pCodeInput = NULL;

// --> [Edit]  ������ 2008/11/06
	m_bDeleteChart = FALSE;
// <-- [Edit]  ������ 2008/11/06
}


void CInputBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBarDlg)	
//@�������� DDX_Control(pDX, IDC_BTETCMENU, m_btEtcMenu);
//@��������		DDX_Control(pDX, IDC_BTCHANGEUPJONG, m_btChangeUpjong);
	//DDX_Control(pDX, IDC_BTETCMENU, m_btEtcMenu);
	DDX_Control(pDX, IDC_BTSAVECHART, m_btSaveChart);
	//DDX_Control(pDX, IDC_CMB_BONG, m_cmbBong);
	DDX_Control(pDX, IDC_BTPERIOD, m_btPeriod);
//@��������	DDX_Control(pDX, IDC_BTSETENVIR, m_btSetEnvir);
	DDX_Control(pDX, IDC_PRGBAR_REPLAY, m_prgBarReplay);
	DDX_Control(pDX, IDC_STATIC_REPLAYTIME, m_staticReplayTime);
	DDX_Control(pDX, IDC_CBMARKET, m_cbMarket);
	DDX_Control(pDX, IDC_CMB_DAY, m_cmbDayBefore);
	DDX_Control(pDX, IDC_CHKFIRST, m_chkFirst);
	DDX_Control(pDX, IDC_CHKSECOND, m_chkSecond);
	DDX_Control(pDX, IDC_CHKTHIRD, m_chkThird);
	DDX_Control(pDX, IDC_CHKFOURTH, m_chkFourth);
	DDX_Control(pDX, IDC_PRGBAR_REPLAY, m_prgBarReplay);
	DDX_Control(pDX, IDC_STATIC_REPLAYTIME, m_staticReplayTime);
	DDX_Control(pDX, IDC_BTN_STOP_REPLAY, m_btnStopReplay);
	DDX_Control(pDX, IDC_BTN_PLAY_PAUSE, m_btnHandleReplay);
	DDX_Control(pDX, IDC_CMB_HOUR, m_cmbReplayHour);
	DDX_Control(pDX, IDC_CMB_MIN, m_cmbReplayMin);
	DDX_Control(pDX, IDC_CMB_SPEED, m_cmbReplaySpeed);
	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbChartViewType);
//@��������	DDX_Control(pDX, IDC_BTN_FOREIGN_SENDTR, m_btnForSendTR);
//@�������� DDX_Control(pDX, IDC_CHECK_DOW, m_chkDow);
//@�������� DDX_Control(pDX, IDC_CHECK_TWD, m_chkTWD);
//@�������� DDX_Control(pDX, IDC_CHECK_SP500, m_chkSP500);
//@�������� DDX_Control(pDX, IDC_CHECK_NIKEI, m_chkNiKei);
//@�������� DDX_Control(pDX, IDC_CHECK_NASDAQ, m_chkNasDaq);
//@��������	DDX_Control(pDX, IDC_CHK_DATALIST, m_chkDataListWnd);
	DDX_Control(pDX, IDC_CMB_CHARTTYPE, m_cmbChartType);
	DDX_Control(pDX, IDC_STJONGNAME, m_stJongName);
	DDX_Control(pDX, IDC_CHKDAY, m_chkDay);
	DDX_Control(pDX, IDC_CHKWEEK, m_chkWeek);
	DDX_Control(pDX, IDC_CHKMONTH, m_chkMonth);
	//DDX_Control(pDX, IDC_CHKHOUR, m_chkHour);
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
	DDX_Control(pDX, IDC_BTPERIOD, m_btPeriod);
	DDX_Control(pDX, IDC_CMB_ORDER, m_cmbOrderType);
	DDX_Check(pDX, IDC_CHK_ORDER, m_checkOrderLink);
// --> [Edit]  ������ 2008/10/14
	DDX_Control(pDX, IDC_CHK_CHE, m_chkChe);
	DDX_Control(pDX, IDC_CHK_HOGA, m_chkHoga);
// <-- [Edit]  ������ 2008/10/14
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
	ON_BN_CLICKED(IDC_BTREQUESTDATMORE, OnBtrequestdatmore)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTINC, OnBtinc)
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_CHKEXTBAR1, OnChkextbar)
//@��������	ON_BN_CLICKED(IDC_CHK_DATALIST, OnChkDatalist)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINZOOMVALUE, OnDeltaposSpinzoomvalue)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DATECNT, OnDeltaposSpinDatecnt)
	ON_BN_CLICKED(IDC_CHK_REVISED, OnChkRevised)
	ON_CBN_SELENDOK(IDC_CMB_CHARTTYPE, OnSelendokCmbCharttype)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)	
//@��������	ON_BN_CLICKED(IDC_BTN_FOREIGN_SENDTR, OnBtnForeignSendtr)
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
	ON_CBN_SELCHANGE(IDC_CMB_MIN, OnSelchangeCmbMin)
	ON_CBN_SELCHANGE(IDC_CMB_HOUR, OnSelchangeCmbHour)
	ON_CBN_SELCHANGE(IDC_CMB_DAY, OnSelchangeCmbDay)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_CBMARKET, OnSelchangeCbmarket)
	ON_BN_CLICKED(IDC_BTPERIOD, OnBtperiod)
//@��������	ON_BN_CLICKED(IDC_BTSETENVIR, OnBtsetenvir)
	ON_BN_CLICKED(IDC_BTMULTICHART, OnBtmultichart)
	ON_CBN_SELCHANGE(IDC_CTRL_CODEINPUT, OnSelchangeCodeInput)
	ON_BN_CLICKED(IDC_BTSAVECHART, OnBtSaveChart)
//	ON_BN_CLICKED(IDC_CHKHOUR, OnChkhour)
//	ON_BN_CLICKED(IDC_BTETCMENU, OnBtetcmenu)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CMB_ORDER, OnSelchangeCmbOrder)
	ON_BN_CLICKED(IDC_CHK_ORDER, OnCheckOrderLink)
// --> [Edit]  ������ 2008/10/14
	ON_BN_CLICKED(IDC_CHK_CHE, OnChkCheClick)
	ON_BN_CLICKED(IDC_CHK_HOGA, OnChkHogaClick)
// <-- [Edit]  ������ 2008/10/14
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POPUPLIST_SELITEM, OnSelBasedlgPopuplistctrl)
	ON_REGISTERED_MESSAGE( RMSG_FROMCODELIST,	OnFromCodeList)
	ON_EN_SETFOCUS(IDC_EDIT_FOCUS, OnEnSetfocusEditFocus)
//@��������		ON_BN_CLICKED(IDC_BTCHANGEUPJONG, OnBnClickedBtchangeupjong)
//@�������� ON_BN_CLICKED(IDC_BTETCMENU, OnBtetcmenu)
	ON_CBN_SELENDOK(IDC_CTRL_CODEINPUT, OnCodeCtrlSelEndOk) 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBarDlg message handlers

void CInputBarDlg::SetAllPosition(int nType)
{
	if (nType == 0)
		return;

	CWnd* pChild = GetWindow(GW_CHILD);
	CRect rectComp;

	GetDlgItem(IDC_CHKEXTBAR1)->GetWindowRect(rectComp);
	int nWidth = rectComp.Width();
	GetDlgItem(IDC_CBMARKET)->GetWindowRect(rectComp);
	nWidth += rectComp.Width();
	
	while (pChild)
	{
		if (m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		{
			if (pChild->GetDlgCtrlID() == IDC_CBMARKET ||
				pChild->GetDlgCtrlID() == IDC_CMB_TYPE ||
				pChild->GetDlgCtrlID() == IDC_CHKEXTBAR1 ||
				pChild->GetDlgCtrlID() == IDC_CHKFIRST ||
				pChild->GetDlgCtrlID() == IDC_CHKSECOND ||
				pChild->GetDlgCtrlID() == IDC_CHKTHIRD ||
				pChild->GetDlgCtrlID() == IDC_CHKFOURTH)
			{
				pChild->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			if (pChild->GetDlgCtrlID() == IDC_CBMARKET ||
				pChild->GetDlgCtrlID() == IDC_CMB_TYPE ||
				pChild->GetDlgCtrlID() == IDC_CHKEXTBAR1)
			{
				pChild->ShowWindow(SW_HIDE);
			}
			else if (pChild->GetDlgCtrlID() != IDC_CHKLEFT)
			{
				pChild->GetWindowRect(rectComp);
				ScreenToClient(rectComp);
				pChild->SetWindowPos(NULL, rectComp.left - nWidth, rectComp.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
			}
		}

		pChild = pChild->GetNextWindow();
	}
}

BOOL CInputBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();	

	CRect rect;
	GetDlgItem(IDC_CTRL_CODEINPUT)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDC_CTRL_CODEINPUT)->DestroyWindow();

	m_pCodeInput = new CCodeCtrl;
	m_pCodeInput->SetRootPath(((CStdDialog*)GetParent())->m_strRootPath);
	m_pCodeInput->UseCodeDlg(TRUE);

	m_pCodeInput->CreateCodeCtrl(this, GetParent()->GetParent()->GetParent()->GetParent(), rect, IDC_CTRL_CODEINPUT);
	m_pCodeInput->SetColor(RGB(0,0,0), RGB(255,255,255));

	m_pCodeInput->SetUnit("�ؿܼ���");
	m_pCodeInput->SetFixUnit("�ؿܼ���");	//0000062: [8041] �ؿܼ��� ������Ʈ�� ����ȳ�â�� �ؿܼ����� �ƴ� �ֽ�����ȳ�â���� ������ ����

	// TODO: Add extra initialization here	
	//InitBaseInfo();
	//��Ų���� ����
	m_crBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);

	m_hKSJongName = (HWND)GetParent()->GetParent()->SendMessage(RMSG_WRAP_GETINFO, 10, (LPARAM)"jcodename");
	if (m_hKSJongName && IsWindow(m_hKSJongName))
	{
		m_stJongName.ShowWindow(SW_HIDE);

		m_stJongName.GetWindowRect(rect);
		ScreenToClient(rect);
		m_stJongName.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
		CWnd::FromHandle(m_hKSJongName)->SetParent(this);
		CWnd::FromHandle(m_hKSJongName)->MoveWindow(rect);
		m_rectJongName = rect;
	}

	if(m_cmbChartViewType)
	{
		// Combo Set 
		// �������� �߰� UI : �������� - ojtaso (20070522)
		m_cmbChartViewType.AddString("���񺯰�");
		m_cmbChartViewType.AddString("��Ʈ�߰�");
		m_cmbChartViewType.AddString("�����߰�");

		//20080913 �̹��� >>
		//�ڽ��� ������ �������� �۾��ϱ� ������ ������ �ִ� ����� ������ø�� ����.
		//������ø����� ������ ���� ���װ� ���� �߻���.
		//m_cmbChartViewType.AddString("������ø");

		if(m_bUseReplayChartMode)
		{
			//��� ������ �� �߰�
			// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
			if( ( m_nMarketType == CMasterTypeInfo::STOCK_CHART ) || ( m_nMarketType == CMasterTypeInfo::FUTOPT_CHART ))
			{
			}
		}

		m_cmbChartViewType.SetCurSel(0);
	}
	if(m_cbMarket)
	{
		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
		if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
			m_cbMarket.SetCurSel(m_nMarketType);
		else
			m_cbMarket.SetCurSel(0);
	}

	Init();

	int nID = 5;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
		m_pDataManager = (IMasterDataManagerLast*)pGateMng->GetInterface(nID);

	m_cmbOrderType.SetCurSel(1);
	return TRUE;
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

	m_strImagePath = ((CStdDialog*)GetParent())->m_szImageDir;

	m_strCfgFile = ((CStdDialog*)GetParent())->m_strConfig;
	if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
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

	m_pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
	if (m_pIL_W20H20 == NULL)
		return;

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

	m_btnStopReplay.FP_vSetImage( m_pIL_W20H20, 64, 65, TRUE, "", FALSE, 67);
	m_btnHandleReplay.FP_vSetImage( m_pIL_W20H20, 68, 69, TRUE, "", FALSE, 71);

// --> [Edit]  ������ 2008/10/14	( ü��/ȣ�� ��ư �߰� )
	m_chkChe.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ü", TRUE, 83);	
	m_chkChe.FP_vSetToolTipText("ü");

	m_chkHoga.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ȣ", TRUE, 83);	
	m_chkHoga.FP_vSetToolTipText("ȣ");
// <-- [Edit]  ������ 2008/10/14

	m_pIL_W17H20 = &(((CStdDialog*)GetParent())->m_ImgList_W17H20);
	if(m_pIL_W17H20 == NULL) return;
	m_btPeriod.FP_vSetImage( m_pIL_W17H20, 0, 2,TRUE, "", FALSE, 3);


	char szTemp[10] = {0,};
	UpdateData(FALSE);

	LoadValue();

	UpdateBtNCbValue();
	
	//2005. 05. 09 by sy,nam--------------------------------------------------------
	CString sTemp;
	memset(szTemp, 0x00, 10);
	CString strConfig = ((CStdDialog*)GetParent())->m_strConfig;
// --> [Edit]  ������ 2008/10/15		( ü��/ȣ�� ��ư�� ���� )
	m_chkChe.SetCheck(TRUE);
	m_chkHoga.SetCheck(FALSE);
// <-- [Edit]  ������ 2008/10/15
	
	::GetPrivateProfileString("0000", "Type", "3", szTemp, sizeof(szTemp), strConfig);
	if(szTemp[0] == 0x00)
		szTemp[0] = DAY_DATA_CHART;
	
	if(szTemp[0] == TICK_DATA_CHART)
	{
		::GetPrivateProfileString("0000", "TickValue", "1", szTemp, sizeof(szTemp), strConfig);
		sTemp.Format("%s", szTemp);
		sTemp.TrimLeft('0');
		m_editPeriod.SetWindowText(sTemp);	

		m_chType	= TICK_DATA_CHART;
		m_nTick		= atoi(sTemp);
		m_chkTick	.SetCheck(TRUE);
	}
	else if(szTemp[0] == SEC_DATA_CHART)
	{
		::GetPrivateProfileString("0000", "SecValue", "5", szTemp, sizeof(szTemp), strConfig);
		sTemp.Format("%s", szTemp);
		sTemp.TrimLeft('0');
		m_editPeriod.SetWindowText(sTemp);	
		
		m_chType = SEC_DATA_CHART;
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
		m_nDay = atoi(sTemp);
		m_chkDay.SetCheck(TRUE);
	}
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
		m_chkMonth.SetCheck(TRUE);
	}

	m_chkDay.Invalidate();
	m_chkWeek.Invalidate();
	m_chkMonth.Invalidate();
	m_chkMinute.Invalidate();
	m_chkSec.Invalidate();
	m_chkTick.Invalidate();

	UpdateBtNCbValue();	// �ֱ��ư Set
	Enable_PeriodControl(TRUE);	
	
	SetExt(0);	

	if (((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)	//������Ʈ �� ���
		m_bChkLoadSaveCode = TRUE;
	else
	{
		::GetPrivateProfileString("InputBar","LoadSaveCode","1",szTemp,sizeof(szTemp),strConfig);
		if(szTemp[0]=='1')
			m_bChkLoadSaveCode = TRUE;
		else
			m_bChkLoadSaveCode = FALSE;
	}

	SetMultiChartItemMode();
}

void CInputBarDlg::OnBtleft() 
{
	// TODO: Add your control notification handler code here
	UINT nLeftBarType = ((CStdDialog*)GetParent())->GetLeftBarType();
	if(nLeftBarType==0||nLeftBarType==2)
	{
		SetLeftBarType(1);
		((CStdDialog*)GetParent())->SetLeftBarType(1);		
	}
	else if(nLeftBarType==1)
	{
		SetLeftBarType(0);
		((CStdDialog*)GetParent())->SetLeftBarType(0);
	}
}

void CInputBarDlg::OnBtright() 
{
	// TODO: Add your control notification handler code here
	UINT nLeftBarType = ((CStdDialog*)GetParent())->GetLeftBarType();
	if(nLeftBarType==0||nLeftBarType==1)
	{
		SetLeftBarType(2);
		((CStdDialog*)GetParent())->SetLeftBarType(2);
	}
	else if(nLeftBarType==2)
	{
		SetLeftBarType(0);
		((CStdDialog*)GetParent())->SetLeftBarType(0);
	}	
}

void CInputBarDlg::SetLeftBarType(UINT nLeftBarType)
{
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
			((CStdDialog*)GetParent())->WriteToStringLog(_T("����� �����ڵ� Input��Ʈ�ѿ� ����"), pStInputCodeData2->m_strCode, pStInputCodeData2->m_strCode.GetLength());
#endif			
			BOOL bDontPutInShared = pStInputCodeData2->m_bDontPutInShared;
			CString strInc;

			{
				// CodeInput Ctrl�� SetDataText �Լ��� ȣ��� ���� ���
				if(GetShareCodeChanged())
				{
					SetShareCodeChanged(FALSE);
					break;
				}				

				// ���ǿ� �������� ����
				// (2008/10/6 - Seung-Won, Bae) Set same code in blank chart.
				CChartItem *pChartItem = ( ( CChartMng *)m_pChartMng)->GetpChartItemSelected();
				BOOL bSetNewItemCode = pStInputCodeData2->m_bRequestWithoutHesitation;

// --> [Edit]  ������ 2008/11/02	( ���� �߰� ���� => ����+��ǥ�� Row ���� ����(2008/11/06) )
				int nId = GetCurrentMode( MODE_DEFAULT, CHANGE_ITEM );
				int nChartCnt =	pChartItem->GetRowBlockCount();			//	pChartItem->GetChartItemCount();
				if ( nId == NEW_ITEM && nChartCnt >= MAX_ADD_CODE && !m_bDeleteChart )
				{
					m_bDeleteChart = TRUE;
					pChartItem->RemoveBlockWithIndex(MAX_ADD_CODE-1, 0);
					m_bDeleteChart = FALSE;

//					if ( (int)lParam != 1 )		//	�޽����� �������� �ʴ´�... ���⿡ �����߰��� ����� ���� ������ ������... )
//					{
//						CString strMsg = "";
//						strMsg.Format("���� �߰��� �ִ� %d�� �Դϴ�.", MAX_ADD_CODE);
////						AfxMessageBox(strMsg);
//						MessageBox(strMsg);
//					}
//					break;
				}
// <-- [Edit]  ������ 2008/11/02

				if( !bSetNewItemCode)
				{
					if( pChartItem)
							bSetNewItemCode = ( GetCurrentCode() != pStInputCodeData2->m_strCode || pChartItem->IsEmpty());
					else	bSetNewItemCode = GetCurrentCode() != pStInputCodeData2->m_strCode;
				}
				if( bSetNewItemCode)
				{	
					m_strCodeFromOutside = pStInputCodeData2->m_strCode;

					m_strOldCode = GetCurrentCode();
					SetCurrentCode(m_strCodeFromOutside);
					EnableWindow(FALSE);
					
					if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
					{
						if(GetType() == MONTH_DATA_CHART)
						{
							m_chType = DAY_DATA_CHART;
							UpdateBtNCbValue();	// �ֱ��ư Set
							SetType(FALSE);				
						}
					}
					else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
					{
						if(GetType() == HOUR_DATA_CHART || GetType() == MIN_DATA_CHART)
						{
							m_chType = DAY_DATA_CHART;
							UpdateBtNCbValue();	// �ֱ��ư Set
							((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetChartRequestType(DAY_DATA_CHART);
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
						// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
						CString strCodeInputCode = GetTypedItemCode();
						if(GetCurrentCode().GetLength() && strCodeInputCode != GetCurrentCode())
						{
//							SetShareCodeChanged(TRUE);
							// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
							if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(GetCurrentCode()));
						}
					}
					else
					{
/*@sini
						if(m_ctrlCodeInput.GetSafeHwnd())
						{
							m_ctrlCodeInput.SetDataText("");
							m_strOldCode = _T("");
							m_strCodeFromOutside = _T("");
							SetCurrentCode(_T(""));
							SetCurCodeNameToControl(_T(""));
						}
*/					}
					EnableWindow(TRUE);
				}
			}			
		}
		break;
	case UMSG_SETZOOMVALUE:
		{
			CString strZoomValue;
			strZoomValue.Format("%d",wParam);
			SetEditZoomValue(strZoomValue);
			//m_lCntInView[m_chType-'0'] = wParam; //05.09.13
		}
		break;
	case UMSG_SETINITIALCHARTDATA:
		{
			//05. 09. 08
			SetUseUserInputPeriod(FALSE);

			m_bSettingInitData = TRUE;
			stChartInitData* _pStData = (stChartInitData*)lParam;

			if(!_pStData->m_chType) return 0L;

// --> [Edit]  �̵��� 2008/09/11
// 
			m_cmbOrderType.SetCurSel(atoi(_pStData->m_strOrderType));
// <-- [Edit]  �̵��� 2008/09/11

			//07.01.20 by LYH �߰�
			char oldChType = m_chType;
			//}}
			
			CString strValue;			
			strValue.Format("%d",_pStData->m_lTotalCnt);
			//{{JS.Kim_20100906 ��ü ������ �߰��θ��� ������ Update�Ǵ� ���� ����
			//m_editCntOfDatAll.SetWindowText(strValue);
			SetDlgItemText(IDC_STC_TOTALCNT, strValue);
			//}}

// --> [Edit]  ������ 2008/11/02	( ��ü ������ ī��Ʈ�� �����Ѵ�. )
//			SetMultiTotalCount(_pStData->m_lTotalCnt);
// <-- [Edit]  ������ 2008/11/02

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

// --> [Edit]  ������ 2008/10/15
			if ( _pStData->m_nCheHoga == 0 )
			{
				m_chkChe.SetCheck(TRUE);
				m_chkHoga.SetCheck(FALSE);
			}
			else
			{
				m_chkChe.SetCheck(FALSE);
				m_chkHoga.SetCheck(TRUE);
			}
// <-- [Edit]  ������ 2008/10/15
			//08.22================================================================
			//���� �ְ� �� �α� �����ֱ� ����
			m_bChkRevised	= _pStData->m_bRevised;
			m_bChkLog		= _pStData->m_bLog;

			UpdateData(FALSE);			
			//07.01.20 by LYH �߰�
			if(oldChType != _pStData->m_chType)
				UpdateBtNCbValue();
			//}}

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

					if(SEC30_VALUE == lNumber)
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

					if(SEC30_VALUE == lNumber)
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

				if ( m_chType==MIN_DATA_CHART && _pStData->m_strMin == SEC30_STRING )
					m_nUnitPos = -1;

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
					m_editPeriod.SetWindowText(_pStData->m_strTick);
				}
				else if(m_chType==SEC_DATA_CHART)				//��
				{				
					//m_nUnitPos = 1;
					_pStData->m_strSec.TrimLeft('0');
					m_editPeriod.SetWindowText(_pStData->m_strSec);
				}
				else if(m_chType==MIN_DATA_CHART)				//��(1)
				{				
					//m_nUnitPos = 1;
					_pStData->m_strMin.TrimLeft('0');

// --> [Edit]  ������ 2008/11/07		( 30�� ��Ʈ ǥ���ϱ����� ����ó�� )
					if ( _pStData->m_strMin == SEC30_STRING )
						m_editPeriod.SetWindowText(SEC30_STRING_FORMAT);
					else
						m_editPeriod.SetWindowText(_pStData->m_strMin);
// <-- [Edit]  ������ 2008/11/07
				}
				else if(m_chType==DAY_DATA_CHART)
				{				
					//m_nUnitPos = 2;
					_pStData->m_strDay.TrimLeft('0');
					m_editPeriod.SetWindowText(_pStData->m_strDay);
				}
				else
					m_editPeriod.SetWindowText("1");
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
						((CStdDialog*)GetParent())->Request_StockHogaData();
					//===============================================================

					return 1l;				
				}
				else
				{
					CWnd* pTrmChartView = ((CStdDialog*)GetParent())->GetParent();
//@��������			CString strType = m_pDataManager->GetCodeInfo("", 2, _pStData->m_strCode);
					int nType;
					int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(_pStData->m_strCode, nType);
					ChangeMarket(nMarketType, FALSE, FALSE);
					pTrmChartView->SendMessage(WM_USER+101, ((CStdDialog*)GetParent())->GetMarketTypeFromCode(_pStData->m_strCode, nType), (LPARAM)(LPCSTR)_pStData->m_strCode);
//@��������						
//					//if(atoi(strType) == 84)// || strType == "W" || strType == "D" || strType == "G" || strType == "H" || strType == "C" || (strType == "Q" && atoi(_pStData->m_strCode) < 1))
//					if(strType == "77" || strType == "78" || strType == "79" || strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")	//�ؿ�
//					{
//						BOOL bRet = ChangeMarket(FOREIGN_CHART);
//						if(bRet)
//							InitBaseInfo(NULL, FALSE);
//					}
////@��������
////					else if(atoi(strType) == 68) //ELW
////					{
////						BOOL bRet = ChangeMarket(ELW_CHART);
////						if(bRet)
////							InitBaseInfo(NULL, FALSE);
////					}
////					// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
////					else if(strType == "13" || strType == "14" || strType == "15" || strType == "16" || strType == "17" || strType == "18" || strType == "19" || strType == "22" || strType == "61" || strType == "62")
////					{
////						BOOL bRet = ChangeMarket(COMMODITY_CHART);
////						if(bRet)
////							InitBaseInfo(NULL, FALSE);
////					}
////@��������
//					else// if(m_nMarketType != FOREIGN_CHART)
//					{
//						if(nLength==FUTOPT_LENGTH)
//						{
////@��������
////							BOOL bRet = ChangeMarket(FUTOPT_CHART);
////							if(bRet)
////								InitBaseInfo(NULL, FALSE);
////@��������
//							pTrmChartView->SendMessage(WM_USER+101, FUTOPT_CHART);
//						}
//						else if(nLength==UPJONG_LENGTH)
//						{
////@��������
////							CString sTemp = _pStData->m_strCode.Left(1);
////							BOOL bRet = ChangeMarket(UPJONG_CHART);
////							if(bRet)
////								InitBaseInfo(NULL, FALSE);
////@��������
//							pTrmChartView->SendMessage(WM_USER+101, UPJONG_CHART);
//						}
//						else if(nLength==CODE_STOCK_LENGTH)
//						{
////@��������
////							BOOL bRet = ChangeMarket(STOCK_CHART);
////							if(bRet)
////								InitBaseInfo(NULL, FALSE);
////@��������						
//							pTrmChartView->SendMessage(WM_USER+101, STOCK_CHART);
//						}	
//					}
//@��������						
				}

				m_strOldCode = GetCurrentCode();
				SetCurrentCode(_pStData->m_strCode);
				SetCurCodeNameToControl(_pStData->m_strCode);
				
				// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)
				m_bReceiveDataSetCode = TRUE;
				if(m_nMarketType != CMasterTypeInfo::FOREIGN_CHART)
				{
					if(wParam != 1) //TR�� ���� �����ͷ� ���� ���� ������ ���ϱ� ���� ���� �ڵ� ���� ���� �ʴ´�.
					{
						if (_pStData->m_strCode.GetLength() == UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
						{
//@��������
//							if(_pStData->m_strCode.GetAt(0) == '2')
//								InitUpjongCodeCombo(100);
//							else
//								InitUpjongCodeCombo(101);
//							m_ctrlCodeInput.SetDataText(_pStData->m_strCode.Mid(1));
//@��������
/*@sini
							CString strCodeInputCode = m_ctrlCodeInput.GetDataText();
							if(_pStData->m_strCode.GetLength() && strCodeInputCode != _pStData->m_strCode)
							{
								SetShareCodeChanged(TRUE);
								// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
								m_ctrlCodeInput.SetDataText( CStdDialog::GetRealItemCode( _pStData->m_strCode));
								SetShareCodeChanged(FALSE);
							}
*/
						}
						else
//							m_ctrlCodeInput.SetDataText(_pStData->m_strCode);
						{
							// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
							CString strCodeInputCode = GetTypedItemCode();
							if(_pStData->m_strCode.GetLength() && strCodeInputCode != _pStData->m_strCode)
							{
								SetShareCodeChanged(TRUE);
								// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
								if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(_pStData->m_strCode));

								SetShareCodeChanged(FALSE);
							}
						}
					}
					else 
					{
						// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
						CString strCode = GetTypedItemCode();
						strCode.TrimLeft(); strCode.TrimRight();
						if(!strCode.GetLength())
						{
							if (_pStData->m_strCode.GetLength() == UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
							{
//@��������
//								if(_pStData->m_strCode.GetAt(0) == '2')
//									InitUpjongCodeCombo(100);
//								else
//									InitUpjongCodeCombo(101);
//								m_ctrlCodeInput.SetDataText(_pStData->m_strCode.Mid(1));
//@��������
								// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
								CString strCodeInputCode = GetTypedItemCode();
								if(_pStData->m_strCode.GetLength() && strCodeInputCode != _pStData->m_strCode)
								{
									SetShareCodeChanged(TRUE);
									// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
									if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(_pStData->m_strCode));
									SetShareCodeChanged(FALSE);
								}
							}
							else
							{
								// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
								CString strCodeInputCode = GetTypedItemCode();
								if(_pStData->m_strCode.GetLength() && strCodeInputCode != _pStData->m_strCode)
								{
									SetShareCodeChanged(TRUE);
									// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
									if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(_pStData->m_strCode));
									SetShareCodeChanged(FALSE);
								}
							}
						}
					}
				}
				else
				{
					if(wParam != 1) //TR�� ���� �����ͷ� ���� ���� ������ ���ϱ� ���� ���� �ڵ� ���� ���� �ʴ´�.
					{
						// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
						CString strCodeInputCode = GetTypedItemCode();
						if(_pStData->m_strCode.GetLength() && strCodeInputCode != _pStData->m_strCode)
						{
							SetShareCodeChanged(TRUE);
							// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
							if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(_pStData->m_strCode));
							SetShareCodeChanged(FALSE);
						}
					}
					else 
					{
						// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
						CString strCode = GetTypedItemCode();
						strCode.TrimLeft(); strCode.TrimRight();
						if(!strCode.GetLength() && _pStData->m_strCode.GetLength())
						{
							SetShareCodeChanged(TRUE);
							// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
							if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(_pStData->m_strCode));
							SetShareCodeChanged(FALSE);
						}
					}
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
//				m_strOldCode = GetCurrentCode(); //2005. 08. 26
//				SetCurrentCode(_T("")); //2005. 08. 26
				//=======================================================================
			}
			
			m_bSettingInitData = FALSE;	
			

			//�θ𿡼� ȣ�� ��ȸ�� ������.			
			//===============================================================
			if(m_bCanRequestHogaTR) //ȣ���� ��ȸ�ص� �Ǵ� ���¿����� ��ȸ�� �Ѵ�.
				((CStdDialog*)GetParent())->Request_StockHogaData();
			//===============================================================

			if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
			{
			}
		}
		break;
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
					m_lCntInView[n] = 200;
				}

// --> [Edit]  ������ 2008/11/02		( �ʱ�ȭ �Ҷ� ���� �߰�)
				this->SendMessage(UMSG_CHE_HOGA_CHANGE, 0);
				m_chkChe.SetCheck(TRUE);
				m_chkHoga.SetCheck(FALSE);

				m_chkDay.EnableWindow(1);
		//		m_chkHour.EnableWindow(1);
				m_chkWeek.EnableWindow(1);
				m_chkMonth.EnableWindow(1);
				m_chkMinute.EnableWindow(1);
				m_chkSec.EnableWindow(1);
				m_chkTick.EnableWindow(1);
				m_chkFirst.EnableWindow(1);
				m_chkSecond.EnableWindow(1);
				m_chkThird.EnableWindow(1);
				m_chkFourth.EnableWindow(1);

				m_cmbChartViewType.EnableWindow(TRUE);
				m_chOldType = ' ';
// <-- [Edit]  ������ 2008/11/02

				OnChkday();
			}
			else
			{
				CString sType;
				sType.Format("%c", GetType());
// --> [Edit]  ������ 2008/12/29	( ��ī��Ʈ�� �ʱ�ȭ�� 400 -> 200���� ���� )
				m_lCntInView[atoi(sType)] = 200;
// <-- [Edit]  ������ 2008/12/29
				
				
				CString strRetryCode;
				strRetryCode = GetCurrentCode();			
				SetCurrentCode(_T(""));

				if (strRetryCode.GetLength()==UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
				{
					// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
					CString strCodeInputCode = GetTypedItemCode();
					if(strRetryCode.GetLength() && strCodeInputCode != strRetryCode)
					{
						SetShareCodeChanged(TRUE);
						// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
						if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strRetryCode));
						SetShareCodeChanged(FALSE);
					}
				}
				else
				{
					// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
					CString strCodeInputCode = GetTypedItemCode();
					if(strRetryCode.GetLength() && strCodeInputCode != strRetryCode)
					{
						SetShareCodeChanged(TRUE);
						// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
						if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strRetryCode));
						SetShareCodeChanged(FALSE);
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
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				CString strCodeInputCode = GetTypedItemCode();
				if(strCode.GetLength() && strCodeInputCode != strCode)
				{
					SetShareCodeChanged(TRUE);
//					m_ctrlCodeInput.SetDataText(strCode.Mid(1));
					// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
					if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strCode));
					SetShareCodeChanged(FALSE);
				}
			}
			else
			{
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				CString strCodeInputCode = GetTypedItemCode();
				if(strCode.GetLength() && strCodeInputCode != strCode)
				{
					SetShareCodeChanged(TRUE);
					// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
					if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strCode));
					SetShareCodeChanged(FALSE);
				}
			}
			//}
		}	
		break;
	case UMSG_END_REQUEST_CHARTDATA:
		{
			SetUseUserInputPeriod(FALSE);
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
			if (m_pCodeInput && (m_nMarketType != CMasterTypeInfo::FOREIGN_CHART))
				m_pCodeInput->SetFocus();
		}
		break;
	case UMSG_GETSAVEJIPYOONEFILE:
		{
			return (long)(m_bChkSaveJipyoOneFile);
		}
		break;
	case UMSG_GETLOADSAVECODE:
		{
			return (long)(m_bChkLoadSaveCode);
		}
		break;
	case UMSG_SETLOADSAVECODE:
		{
			m_bChkLoadSaveCode = !m_bChkLoadSaveCode;
		}
		break;
	case UMSG_CHE_HOGA_CHANGE:
		{
			((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetCheHogaChange((short)wParam);
		}
		break;
	}

	
	{
		if(message == RMSG_EVENT_CODEINPUT)
		{
			LPST_CTRLEVENT stEvent = (LPST_CTRLEVENT)lParam;
			if(strcmp("LengthFull", stEvent->lpszEventName) == 0 || strcmp("SelChanged", stEvent->lpszEventName) == 0)
			{
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				SetKeyCodeChange(NID_CODEINPUT, ( LPARAM)( LPCTSTR)GetTypedItemCode());
			}
		}
	}
		
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
#ifdef _DEBUG
CString strTEST;
strTEST.Format("[gm0604]SetType(%d) m_chType = %c, m_nUnitPos= %d...",bSendChartMng,m_chType,m_nUnitPos);
::OutputDebugString(strTEST);
#endif
	EnableWindow(FALSE);
	// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5) -- �� �κ��� ��ȭ���� �ű� �߰��� �׸�
	CString sKey = _T("");
	switch(m_chType)
	{
		case TICK_DATA_CHART:	
		case SEC_DATA_CHART:
		case MIN_DATA_CHART:
		case DAY_DATA_CHART:
		case HOUR_DATA_CHART:
			//m_chkDay.SetCheck(0);
			m_chkWeek.SetCheck(0);
			m_chkMonth.SetCheck(0);
//			m_chkYear.SetCheck(0);
//			m_chkSec10.SetCheck(0);
//			m_chkSec30.SetCheck(0);			
			Enable_PeriodControl(TRUE);
			switch(m_chType)
			{
				case TICK_DATA_CHART:	
					m_chkTick.SetCheck(1);
					m_chkSec.SetCheck(0);
					m_chkMinute.SetCheck(0);				
					m_chkDay.SetCheck(0);
//					m_chkHour.SetCheck(0);
					//m_chkFourth.ShowWindow(TRUE);
					//{{JS.Kim_20100902 Solomon ��/ƽ��Ʈ �ֱ� ����â
					//m_editPeriod.EnableWindow(FALSE);
					m_editPeriod.EnableWindow(TRUE);
					//}}
					//m_cmbBong.EnableWindow(FALSE);
					
					//05.10.14 ���� ��� �϶� TR������ �� ���� ������ ���� �۵� ���´�.
					//if(m_nDataAddMode == REPLAY_ITEM) 
					//	m_btnHandleReplay.EnableWindow(FALSE);
					Enable_PeriodControl(TRUE);
					break;
				case SEC_DATA_CHART:	
					m_chkTick.SetCheck(0);
					m_chkSec.SetCheck(1);
					m_chkMinute.SetCheck(0);				
					m_chkDay.SetCheck(0);
//					m_chkHour.SetCheck(0);
					//m_chkFourth.ShowWindow(TRUE);
					m_editPeriod.EnableWindow(FALSE);
					//m_cmbBong.EnableWindow(FALSE);
					
					//05.10.14 ���� ��� �϶� TR������ �� ���� ������ ���� �۵� ���´�.
					//if(m_nDataAddMode == REPLAY_ITEM) 
					//	m_btnHandleReplay.EnableWindow(FALSE);
					Enable_PeriodControl(FALSE);
					break;
				case MIN_DATA_CHART:	
					m_chkTick.SetCheck(0);
					m_chkSec.SetCheck(0);
					m_chkMinute.SetCheck(1);				
					m_chkDay.SetCheck(0);	
//					m_chkHour.SetCheck(0);
					//m_chkFourth.ShowWindow(TRUE);
					m_editPeriod.EnableWindow(TRUE);
					//m_cmbBong.EnableWindow(TRUE);

					////////<<20100406_JS.Kim ���� ���۾�. N�к�,Nƽ �� ����
					Enable_PeriodControl(TRUE);
					//>>

					//05.10.14 ���� ��� �϶� TR������ �� ���� ������ ���� �۵� ���´�.
					//if(m_nDataAddMode == REPLAY_ITEM) 
					//	m_btnHandleReplay.EnableWindow(FALSE);

					break;
				case DAY_DATA_CHART:
					m_chkTick.SetCheck(0);
					m_chkSec.SetCheck(0);
					m_chkMinute.SetCheck(0);									
					m_chkDay.SetCheck(1);	
//					m_chkHour.SetCheck(0);
					//m_chkFourth.ShowWindow(TRUE);
					m_editPeriod.EnableWindow(FALSE);
					//m_cmbBong.EnableWindow(FALSE);
					Enable_PeriodControl(FALSE);
					break;
				//case HOUR_DATA_CHART:
				//	m_chkTick.SetCheck(0);
				//	m_chkMinute.SetCheck(0);									
				//	m_chkDay.SetCheck(0);		
				//	m_chkHour.SetCheck(1);

				//	m_chkFourth.ShowWindow(FALSE);
				//	m_editPeriod.EnableWindow(FALSE);
				//	m_cmbBong.EnableWindow(FALSE);
				//	break;
			}
			switch(m_nUnitPos)
			{
			case 1:
				m_chkFirst.SetCheck(0);
				m_chkSecond.SetCheck(1);
				m_chkThird.SetCheck(0);
				m_chkFourth.SetCheck(0);
				break;
			case 2:
				m_chkFirst.SetCheck(0);
				m_chkSecond.SetCheck(0);
				m_chkThird.SetCheck(1);
				m_chkFourth.SetCheck(0);
				break;
			case 3:
				m_chkFirst.SetCheck(0);
				m_chkSecond.SetCheck(0);
				m_chkThird.SetCheck(0);
				m_chkFourth.SetCheck(1);
				break;
			case 0:
				m_chkFirst.SetCheck(1);
				m_chkSecond.SetCheck(0);
				m_chkThird.SetCheck(0);
				m_chkFourth.SetCheck(0);
				break;
			default:
				m_chkFirst.SetCheck(0);
				m_chkSecond.SetCheck(0);
				m_chkThird.SetCheck(0);
				m_chkFourth.SetCheck(0);
				m_nUnitPos = 0;

				//bSendChartMng = FALSE;
				break;			
			}
			break;	
		case WEEK_DATA_CHART:
			m_chkDay.SetCheck(0);
//			m_chkHour.SetCheck(0);
			m_chkWeek.SetCheck(1);
			m_chkMonth.SetCheck(0);
			m_chkMinute.SetCheck(0);
			m_chkSec.SetCheck(0);
			m_chkTick.SetCheck(0);
			m_chkFirst.SetCheck(0);
			m_chkSecond.SetCheck(0);
			m_chkThird.SetCheck(0);
			m_chkFourth.SetCheck(0);
			//m_chkFourth.ShowWindow(TRUE);
			m_editPeriod.EnableWindow(FALSE);
//			m_chkYear.SetCheck(0);
//			m_chkSec10.SetCheck(0); //05.09.28
//			m_chkSec30.SetCheck(0); //05.09.28
			Enable_PeriodControl(FALSE);
			//m_cmbBong.EnableWindow(FALSE);
			break;
		case MONTH_DATA_CHART:
			m_chkDay.SetCheck(0);
//			m_chkHour.SetCheck(0);
			m_chkWeek.SetCheck(0);
			m_chkMonth.SetCheck(1);
			m_chkMinute.SetCheck(0);
			m_chkSec.SetCheck(0);
			m_chkTick.SetCheck(0);
			m_chkFirst.SetCheck(0);
			m_chkSecond.SetCheck(0);
			m_chkThird.SetCheck(0);
			m_chkFourth.SetCheck(0);
			//m_chkFourth.ShowWindow(TRUE);
			m_editPeriod.EnableWindow(FALSE);
			//m_cmbBong.EnableWindow(FALSE);
			//			m_chkYear.SetCheck(0);
			//			m_chkSec10.SetCheck(0); //05.09.28
			//			m_chkSec30.SetCheck(0); //05.09.28
			Enable_PeriodControl(FALSE);
			break;
	}

	m_chkDay.Invalidate();
//	m_chkHour.Invalidate();
	m_chkWeek.Invalidate();
	m_chkMonth.Invalidate();
	m_chkMinute.Invalidate();
	m_chkSec.Invalidate();
	m_chkTick.Invalidate();
//	m_chkYear.Invalidate();
//	m_chkSec10.Invalidate(); //05.09.28
//	m_chkSec30.Invalidate(); //05.09.28
	m_chkFirst.Invalidate();
	m_chkSecond.Invalidate();
	m_chkThird.Invalidate();
	m_chkFourth.Invalidate();
	
	if(bSendChartMng)
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

			m_pChartMng->SendMessage(UMSG_SETTYPE,
									m_chType, // ƽ, ��, ��,....���� 
									nValue);//����� ���� �Ⱓ
		}
	}
	EnableWindow();
}




void CInputBarDlg::OnChkmonth() 
{
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkMonth.SetCheck(!m_chkMonth.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	( ( CStdDialog *)GetParent())->SelectMainRQ();

	// TODO: Add your control notification handler code here
	m_chType = MONTH_DATA_CHART;
	m_nUnitPos = -1;
	//Enable_PeriodControl(FALSE);
	//m_editPeriod.SetWindowText("1");

	((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);
	SetType();
}

void CInputBarDlg::OnChkweek() 
{
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkWeek.SetCheck(!m_chkWeek.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	( ( CStdDialog *)GetParent())->SelectMainRQ();

	// TODO: Add your control notification handler code here
	m_chType = WEEK_DATA_CHART;
	m_nUnitPos = -1;
	//Enable_PeriodControl(FALSE);
	//m_editPeriod.SetWindowText("1");

	((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);
	SetType();
}

void CInputBarDlg::OnChktick() 
{	
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkTick.SetCheck(!m_chkTick.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

// --> [Edit]  ������ 2008/11/28	( ƽ��Ʈ�ϰ�� ���ӿ����� �����Ҽ� ������ �Ѵ�. )
	int nFind = m_strCode.Find('.');
	if ( nFind != -1 )
	{
		MessageBox("������ ������ ƽ�� ��� ���ӿ��� �ڵ�� ����� �� �����ϴ�.");
		UpdateData(TRUE);
		m_chkTick.SetCheck(!m_chkTick.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/28

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
		((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);
	else
		((CStdDialog*)GetParent())->EnableQueryOpt(TRUE);
	SetType();

	SetUseUserInputPeriod(FALSE);
}

void CInputBarDlg::OnChksec() 
{
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkSec.SetCheck(!m_chkSec.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	( ( CStdDialog *)GetParent())->SelectMainRQ();

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
		((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);
	else
		((CStdDialog*)GetParent())->EnableQueryOpt(TRUE);
	SetType();

	SetUseUserInputPeriod(FALSE);
}

void CInputBarDlg::OnChkminute() 
{
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkMinute.SetCheck(!m_chkMinute.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	( ( CStdDialog *)GetParent())->SelectMainRQ();

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

		if(m_nMin >= SEC30_VALUE)
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
		((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);
	else
		((CStdDialog*)GetParent())->EnableQueryOpt(TRUE);
	SetType();

	SetUseUserInputPeriod(FALSE);
}

void CInputBarDlg::OnChkday() 
{
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkDay.SetCheck(!m_chkDay.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	( ( CStdDialog *)GetParent())->SelectMainRQ();

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

	((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);
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
//	CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
//
//	char szTemp[8];
//	for(int nPos=0;nPos<8;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lCntInView[nPos] = ::GetPrivateProfileInt("CntInView",szTemp,m_lCntInView[nPos],strUserDir);
//	}

	long lDefaultValueOfButton[CNTOFMAXBUTTONS] = { 1, 2, 3, 4};		// ��
	long lDefaultValueOfMinButton[CNTOFMAXBUTTONS] = {1, 15, 30, 60};		// ��
	long lDefaultValueOfSecButton[CNTOFMAXBUTTONS] = { 5, 10, 30, 50};		// ��
	long lDefaultValueOfTickButton[CNTOFMAXBUTTONS] = { 1, 3, 5, 10};		// ƽ
	long lValueOfCombo[7] = { 1, 3, 5, 10, 15, 30, 60};
//	long lValueOfCombo[CNTOFMAXCBITEMS] = { 1, 3, 5, 10};
	char szTemp[8];
	CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
	for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfButton[nPos] = ::GetPrivateProfileInt("Day Button",szTemp,lDefaultValueOfButton[nPos],strUserDir);
	}
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfMinButton[nPos] = ::GetPrivateProfileInt("Min Button",szTemp,lDefaultValueOfMinButton[nPos],strUserDir);
	}
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfSecButton[nPos] = ::GetPrivateProfileInt("Sec Button",szTemp,lDefaultValueOfSecButton[nPos],strUserDir);
	}
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ƽ
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfTickButton[nPos] = ::GetPrivateProfileInt("Tick Button",szTemp,lDefaultValueOfTickButton[nPos],strUserDir);
	}

	for(nPos=0;nPos< 7 ;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		m_lValueOfCombo[nPos] = ::GetPrivateProfileInt("Combo",szTemp,lValueOfCombo[nPos],strUserDir);
	}
	for(nPos=0;nPos<5;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		m_lCntInView[nPos] = ::GetPrivateProfileInt("CntInView",szTemp,m_lCntInView[nPos],strUserDir);
	}	

	// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
	CString strData;
	strData.GetBuffer(20);
//@��������
//	::GetPrivateProfileString(_T("InputBar"), _T("SavedCode"), _T("20001;NAS@IXIC"), (LPSTR)(LPCTSTR)strData, 20, strUserDir);
	::GetPrivateProfileString(_T("InputBar"), _T("SavedCode"), _T("001;.DJI"), (LPSTR)(LPCTSTR)strData, 20, strUserDir);
//@��������
	strData.ReleaseBuffer();
	int nIndex = strData.Find(_T(";"));
	m_strUpjongCode = strData.Left(nIndex);
	strData.Delete(0, nIndex + 1);
	m_strForeignCode = strData;
}

void CInputBarDlg::LoadValue()
{	
	LoadIniValue();
/*	CString strHistory = m_pctrlCode->GetHistoryList(20, GetBitFromCodeInfoJongmok());
	CString strAHistory;
	CString strName;
	CString strCode;
	long lFullLength = 0;
	long lALength = 0;
	strHistory.TrimRight(';');
	while(1)
	{
		int nPos=0;
		nPos = strHistory.ReverseFind(';');
		lFullLength = strHistory.GetLength();
		lALength = lFullLength-nPos;
		if(nPos==-1) break;
		strAHistory = strHistory.Right(lALength-1);
		strHistory.Delete(lFullLength-lALength,lALength);
		nPos = strAHistory.Find('=');
		strCode = strAHistory.Left(nPos);
		strAHistory.Delete(0,nPos+1);
		strName = strAHistory;
		AddHistoryInfoListJongmok(strCode, strName,FALSE);
	}
	
	CString strHistoryUpjong = m_pctrlCode->GetHistoryList(20, GetBitFromCodeInfoUpjong());
	CString strAHistoryUpjong;
	lFullLength = 0;
	lALength = 0;
	strHistoryUpjong.TrimRight(';');
	int nPos=0;
	while(1)
	{
		nPos = strHistoryUpjong.ReverseFind(';');
		lFullLength = strHistoryUpjong.GetLength();
		lALength = lFullLength-nPos;
		if(nPos==-1) break;
		strAHistoryUpjong = strHistoryUpjong.Right(lALength-1);
		strHistoryUpjong.Delete(lFullLength-lALength,lALength);
		nPos = strAHistoryUpjong.Find('=');
		strCode = strAHistoryUpjong.Left(nPos);
		strAHistoryUpjong.Delete(0,nPos+1);
		strName = strAHistoryUpjong;
		AddHistoryInfoListUpjong(strCode, strName,FALSE);
	}
*/
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
	CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
	char szTemp[8];
	char szTemp2[8];
	CString strTemp;
	for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfButton[nPos]);
		::WritePrivateProfileString("Day Button",szTemp,szTemp2,strUserDir);
	}	
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfMinButton[nPos]);
		::WritePrivateProfileString("Min Button",szTemp,szTemp2,strUserDir);
	}	
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfSecButton[nPos]);
		::WritePrivateProfileString("Sec Button",szTemp,szTemp2,strUserDir);
	}	
	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ƽ
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfTickButton[nPos]);
		::WritePrivateProfileString("Tick Button",szTemp,szTemp2,strUserDir);
	}	
	for(nPos=0;nPos<7;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lValueOfCombo[nPos]);
		::WritePrivateProfileString("Combo",szTemp,szTemp2,strUserDir);
	}
	for(nPos=0;nPos<5;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		sprintf(szTemp2,"%d",m_lCntInView[nPos]);
		::WritePrivateProfileString("CntInView",szTemp,szTemp2,strUserDir);
	}

	strTemp.Format("%d", m_bChkSaveJipyoOneFile);
	::WritePrivateProfileString("InputBar","SaveJipyoOneFile",strTemp,strUserDir);	

	strTemp.Format("%d", m_bChkLoadSaveCode);
	::WritePrivateProfileString("InputBar","LoadSaveCode",strTemp,strUserDir);	

	// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
	strTemp.Format("%s;%s", m_strUpjongCode, m_strForeignCode);
	::WritePrivateProfileString("InputBar","SavedCode",strTemp,strUserDir);	

// --> [Edit]  ������ 2008/11/02
	strTemp.Format("%c",m_chType);
	::WritePrivateProfileString("0000","Type",strTemp,strUserDir);

	if ( GetCheHogaType() == 0 )		//	ü�� ��ư�̸� ü���� �������Ͽ��� �������ش�... 
	{
		CString strName = strUserDir;
		CString strScreenNo = ((CStdDialog*)GetParent())->m_strScreenNo;
#ifdef	_LASTINFO_SAVE
		strTemp = "";
		strName.Replace(strScreenNo, "Che_" + strScreenNo);
		strName.Replace(".dat", ".mct");
//		strTemp = strName;
//		strName.Format("%s%s.mct", strTemp, ((CStdDialog*)GetParent())->m_strSaveKey);
#else
		strName.Replace(strScreenNo, "Che_" + strScreenNo);
		strName.Replace(".dat", ".mct");
#endif
		::WritePrivateProfileString("0000","Type",strTemp,strName);
	}
// <-- [Edit]  ������ 2008/11/02

//	CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
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
// --> [Edit]  ������ 2008/12/27
	CString strTotValue;
	GetDlgItemText(IDC_EDITCNTOFDATALL, strTotValue);

	int nCnt(0);
	if ( !strTotValue.IsEmpty() )
		nCnt = atoi(strTotValue);

// <-- [Edit]  ������ 2008/12/27
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_REQUESTMORE, nCnt);
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
	m_chkMinute.ShowWindow(nCmdShow1);
//	m_chkSec.ShowWindow(nCmdShow1);
	m_chkTick.ShowWindow(nCmdShow1);
	m_chkFirst.ShowWindow(nCmdShow1);
	m_chkSecond.ShowWindow(nCmdShow1);
	m_chkThird.ShowWindow(nCmdShow1);
	m_chkFourth.ShowWindow(nCmdShow1);	
//	m_editPeriod.ShowWindow(nCmdShow1);
//	m_btPeriod.ShowWindow(nCmdShow1);

//	m_editZoomValue.ShowWindow(nCmdShow2);
//	m_editCntOfDatAll.ShowWindow(nCmdShow2);
//	m_chkFixPeriod.ShowWindow(nCmdShow2);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//	m_btFixPeriod.ShowWindow(nCmdShow2);
// �ϴ� ���ٷ� �̵� ---------------------------------------------
//	m_btResetPeriod.ShowWindow(nCmdShow2);
//	m_btRequestMore.ShowWindow(nCmdShow2);
//	m_spZoomValue.ShowWindow(nCmdShow2);
//	m_stDiv.ShowWindow(nCmdShow2);
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
		SHIFTCONTROL(m_chkMinute,nShiftExt61,rect);
		SHIFTCONTROL(m_chkSec,nShiftExt61,rect);
		SHIFTCONTROL(m_chkTick,nShiftExt61,rect);
		SHIFTCONTROL(m_chkFirst,nShiftExt61,rect);
		SHIFTCONTROL(m_chkSecond,nShiftExt61,rect);
		SHIFTCONTROL(m_chkThird,nShiftExt61,rect);
		SHIFTCONTROL(m_chkFourth,nShiftExt61,rect);
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
			::WritePrivateProfileString("Frame","ShowExtInputBarDlg","1",m_strCfgFile);
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
	if(!((CStdDialog*)GetParent())->GetStatusEnable()) return 1L;
	LPSTCODERESULT pcodeRet = (LPSTCODERESULT)lParam ;
	CString szCode, szName;
	szCode.Format("%-15.15s", pcodeRet->code);		
	szCode.TrimLeft();
	szCode.TrimRight();
	szName.TrimLeft();
	szName.TrimRight();
	
	if (szCode.GetLength()==UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		CString strCodeInputCode = GetTypedItemCode();
		if(szCode.GetLength() && strCodeInputCode != szCode)
		{
			SetShareCodeChanged(TRUE);
//			m_ctrlCodeInput.SetDataText(szCode.Mid(1));
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(szCode));
			SetShareCodeChanged(FALSE);
		}
	}
	else
	{
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		CString strCodeInputCode = GetTypedItemCode();
		if(szCode.GetLength() && strCodeInputCode != szCode)
		{
			SetShareCodeChanged(TRUE);
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(szCode));
			SetShareCodeChanged(FALSE);
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
			//m_stJongName.SetWindowText(name);
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
			//m_stJongName.SetWindowText(name);
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
			//m_stJongName.SetWindowText(name);
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
				}
			}
			break;
		case CMasterTypeInfo::FOREIGN_CHART:
			{
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
			m_nCurrentMode = nMode;	
			((CStdDialog*)GetParent())->UnLoad_ReplayChart_Lib();
			ChangeToolBarForReplayChart(FALSE);
		}
		else if( (m_nCurrentMode != REPLAY_ITEM) && (nMode == REPLAY_ITEM) ) // ���� X -> ���� O
		{
			//���� Lib �ε� 
			if(!((CStdDialog*)GetParent())->Load_ReplayChart_Lib())
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
			if(m_cmbChartViewType.GetSafeHwnd()==pMsg->hwnd)
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
						//m_lCntInView[m_chType-'0'] = atol(strZoomValue); //05.09.13
//						for(int n = 0;n<8;n++)//ƽ �� �� �� �� �� 10�� 30�� 
//						{
//							m_lCntInView[n] = atol(strZoomValue);
//						}
						m_lCntInView[m_chType-'0'] = atol(strZoomValue); //05.09.13
						//}}
						//========================================================================

						if(m_pChartMng->GetSafeHwnd()) // 20 05. 04. 22 by Nam
						{
							m_pChartMng->SendMessage(UMSG_SETZOOMVALUE,atol(strZoomValue));

							CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
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
					if(atol(strCntOfDatAll)>0)
					{
						if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
							m_pChartMng->SendMessage(UMSG_SETCNTS,atol(strCntOfDatAll));
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
					//{{ JS.Kim_20100903 Solomon �ؿܼ����� �ִ� 360���� ���񽺵ȴ�.(���� ����)
					//if(i>=1000 || i <= 0) 					
					//{
					//	AfxMessageBox("1000������ ������ �ٽ� �����Ͽ� �ֽʽÿ�");
					//	return TRUE;
					//
					if(i > 360 || i <= 0) 					
					{
						AfxMessageBox("360 ������ ������ �ٽ� �����Ͽ� �ֽʽÿ�");
						return TRUE;
					}
					//}}


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

/*@sini
	if (::IsChild(m_ctrlCodeInput.GetSafeHwnd(), pMsg->hwnd))
	{
		return FALSE;
	}
*/
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
	BOOL bShowExtInputBarDlg = ((CStdDialog*)GetParent())->GetShowExtInputBarDlg();
	((CStdDialog*)GetParent())->SetShowExtInputBarDlg(!bShowExtInputBarDlg);
}

void CInputBarDlg::SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg)
{
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

	//	30�� �޴��� �߰�
	CString	strTemp = "30��";
	sprintf(szTemp,"%s", strTemp);
	menu.AppendMenu(MF_STRING,(UINT)1000,szTemp);
	
	for(int nIndex=0;nIndex<7;nIndex++)
	{
/*
// --> [Edit]  ������ 2008/10/22
		if(m_chType==DAY_DATA_CHART)		//	��
		{
			if(m_lValueOfButton[nIndex])
			{
				sprintf(szTemp,"%d",m_lValueOfButton[nIndex]);
				menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,szTemp);
			}
		}
		else if(m_chType==TICK_DATA_CHART)	//	ƽ
		{
			if(m_lValueOfTickButton[nIndex])
			{
				sprintf(szTemp,"%d",m_lValueOfTickButton[nIndex]);
				menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,szTemp);
			}
		}
		else if(m_chType==SEC_DATA_CHART)	//	��
		{
			if(m_lValueOfSecButton[nIndex])
			{
				sprintf(szTemp,"%d",m_lValueOfSecButton[nIndex]);
				menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,szTemp);
			}
		}
		else if(m_chType==MIN_DATA_CHART)	//	��
		{
			if(m_lValueOfMinButton[nIndex])
			{
				sprintf(szTemp,"%d",m_lValueOfMinButton[nIndex]);
				menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,szTemp);
			}
		}
		else
		{
			if(m_lValueOfCombo[nIndex])
			{
				sprintf(szTemp,"%d",m_lValueOfCombo[nIndex]);
				menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,szTemp);
			}
		}
*/
		if(m_lValueOfCombo[nIndex])
		{
			sprintf(szTemp,"%d",m_lValueOfCombo[nIndex]);
			menu.AppendMenu(MF_STRING,(UINT)1000+1+nIndex,szTemp);
		}

// <-- [Edit]  ������ 2008/10/22
	}
	menu.AppendMenu(MF_STRING,(UINT)1000+1+7,"����");

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	CRect rect;
	GetDlgItem(IDC_BTPERIOD)->GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, rect.left, rect.bottom, this);
	if(!nRetValue) return;
	menu.DestroyMenu();

	if((nRetValue-1000) == (7+1))
	{
//		CSetUnitDlg dlg;
		CSetCycleDlg dlg(GetParent());
		dlg.m_pInputBarDlg = this;
		dlg.DoModal();
	}
	else
	{
		m_nUnitPos = -1;
		if(m_chType==WEEK_DATA_CHART || m_chType==MONTH_DATA_CHART)
		{
			m_chType = MIN_DATA_CHART;
		}

		if ( (nRetValue-1000) == 0 )
		{
			strTemp = SEC30_STRING_FORMAT;
			sprintf(szTemp,"%s", strTemp);
			SetType(FALSE);
			m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType, SEC30_VALUE);			
			m_editPeriod.SetWindowText(szTemp);	
		}
		else
		{
			sprintf(szTemp,"%d",m_lValueOfCombo[nRetValue-1000-1]);
			SetType(FALSE);
			m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType,atol(szTemp));			
			m_editPeriod.SetWindowText(szTemp);	
		}

//		SetType(FALSE);
//		m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType,atol(szTemp));			
//		m_editPeriod.SetWindowText(szTemp);	
		UpdateData(FALSE);
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
	CString strPeriod;
	long lPeriod = 0;
	m_editPeriod.GetWindowText(strPeriod);

// --> [Edit]  ������ 2008/11/17
	if ( strPeriod == SEC30_STRING_FORMAT )
		lPeriod = SEC30_VALUE;
	else
		lPeriod = atol(strPeriod);
// <-- [Edit]  ������ 2008/11/17

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
	if(!((CStdDialog*)GetParent())->GetStatusEnable()) return FALSE;	

	//�Ʒ� Return�� �ؼ� TR��ȸ�� �ȵȰų� �� ��� �Ʒ� ��ƾ�� Ÿ���� �Ѵ�.
//	return; //2005. 07. 13 
	//===================================================================
	BOOL bRet = FALSE;

	if(nMarketType != m_nPosCbMarketOld)
	{
		((CChartMng*)m_pChartMng)->m_bNeedForceCode = TRUE;
//		if((nMarketType == FUTOPT_CHART || m_nPosCbMarketOld == FUTOPT_CHART))
//		{
//			CRect rectEditCode;
//			CRect rectBtnSearch;
//			CRect rectBtCodeHistory;
//			if(nMarketType == FUTOPT_CHART)// future option
//			{
//				m_chkWeek.EnableWindow(FALSE);
//				m_chkMonth.EnableWindow(FALSE);
//			}
//			else if(nMarketType == UPJONG_CHART || nMarketType == STOCK_CHART || nMarketType == FOREIGN_CHART || nMarketType == ELW_CHART)	// Upjong, Jongmok
//			{
//				m_chkWeek.EnableWindow();
//				m_chkMonth.EnableWindow();
//			}
//		}

		if(!IsMiniBar())
		{
			m_chkMinute.EnableWindow();
			m_chkSec.EnableWindow();
			m_chkWeek.EnableWindow();
			m_chkMonth.EnableWindow();
		}

		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
		if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)// future option
		{
			//���� ����Ʈ �߰�
			//m_chkWeek.EnableWindow(FALSE);
			//m_chkMonth.EnableWindow(FALSE);
		}
		if(nMarketType == CMasterTypeInfo::UPJONG_CHART && !IsMiniBar())// foreign option
		{
			m_chkSec.EnableWindow(FALSE);
		}
		if(nMarketType == CMasterTypeInfo::FOREIGN_CHART && !IsMiniBar())// foreign option
		{
			m_chkMinute.EnableWindow(FALSE);
			m_chkSec.EnableWindow(FALSE);
		}

		CString strInc;
		//m_btInc.GetWindowText(strInc);
		if(IsMiniBar())
		{
			strInc = "����";
		}

		if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
		{
			m_cbMarket.SetCurSel(1);
			if(bUpdateCbMarket)
			{
				m_nMarketType = CMasterTypeInfo::UPJONG_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}
			else
			{
				((CChartMng*)m_pChartMng)->SetMarketType(nMarketType);
//@��������				if(!IsMiniBar())
//@��������					m_stJongName.SetText("");
			}
		}
		else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
		{
			m_cbMarket.SetCurSel(2);
			if(bUpdateCbMarket)
			{
				m_nMarketType = CMasterTypeInfo::FUTOPT_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}	
			else
			{
				((CChartMng*)m_pChartMng)->SetMarketType(nMarketType);
//@��������				if(!IsMiniBar())
//@��������					m_stJongName.SetText("");
			}
		}
		else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
		{
			m_cbMarket.SetCurSel(3);
			if(bUpdateCbMarket)
			{
				m_nMarketType = CMasterTypeInfo::STOCKFUTOPT_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}	
			else
			{
				((CChartMng*)m_pChartMng)->SetMarketType(nMarketType);
//@��������				if(!IsMiniBar())
//@��������					m_stJongName.SetText("");
			}
		}
		else if( nMarketType ==CMasterTypeInfo::FOREIGN_CHART)
		{		
			m_cbMarket.SetCurSel(CMasterTypeInfo::FOREIGN_CHART);
			if(bUpdateCbMarket)
			{		
				m_nMarketType = CMasterTypeInfo::FOREIGN_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}
			else
			{
				((CChartMng*)m_pChartMng)->SetMarketType(nMarketType);
//@��������				if(!IsMiniBar())
//@��������					m_stJongName.SetText("");
			}
		}
//@��������
//		else if(nMarketType == ELW_CHART)
//		{
//			if(bUpdateCbMarket)
//			{
//				m_cbMarket.SetCurSel(ELW_CHART);
//				m_nMarketType = ELW_CHART;
//				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
//					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
//				else
//					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
//				bRet = TRUE;
//			}
//			else
//			{
//				if(!IsMiniBar())
//					m_stJongName.SetText("");
//			}
//		}
//		else if(nMarketType == COMMODITY_CHART)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//		{
//			if(bUpdateCbMarket)
//			{
//				m_cbMarket.SetCurSel(COMMODITY_CHART);
//				m_nMarketType = COMMODITY_CHART;
//				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
//					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
//				else
//					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
//				bRet = TRUE;
//			}	
//			else
//			{
//				if(!IsMiniBar())
//					m_stJongName.SetText("");
//			}
//		}
//@��������
		else
		{
			m_cbMarket.SetCurSel(0);
			if(bUpdateCbMarket)
			{
				m_nMarketType = CMasterTypeInfo::STOCK_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}
			else
			{
				((CChartMng*)m_pChartMng)->SetMarketType(nMarketType);
//@��������				if(!IsMiniBar())
//@��������					m_stJongName.SetText("");
			}
			POSITION pos = m_lstHistoryInfo.GetTailPosition();
			if(pos)
			{
				CHistory* pHistory = m_lstHistoryInfo.GetAt(pos);
				if(bRequestToServer&&strInc=="����")
				{
					STINPUTCODEDATA2 stInputCodeData2;
					stInputCodeData2.m_strCode = pHistory->_Code;
					stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
					stInputCodeData2.m_bDontPutInShared = FALSE;
					this->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);			
				}
			}
		}

//@�����߰�
		//// {{		�����ڵ��� Type����
		CWnd* pWnd = ((CStdDialog*)GetParent())->GetParent();
		if(pWnd)
		{
			//(BOOL) pWnd->SendMessage(WM_USER+101,nMarketType);
			pWnd->SendMessage(WM_USER+101, nMarketType);
		}
		//// }}
//@�����߰�
	}
	m_nPosCbMarketOld = nMarketType;
	return bRet;
}

void CInputBarDlg::OnChkyear() 
{
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
//	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
//	{
//		UpdateData(TRUE);
//		m_chkYear.SetCheck(!m_chkYear.GetCheck());
//		UpdateData(FALSE);
//		return;
//	}
// <-- [Edit]  ������ 2008/11/07

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	( ( CStdDialog *)GetParent())->SelectMainRQ();

	// TODO: Add your control notification handler code here
	m_chType = YEAR_DATA_CHART;
	m_nUnitPos = -1;
	//m_editPeriod.SetWindowText("1");
	SetType();	
}

void CInputBarDlg::OnChkfirst()
{
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkFirst.SetCheck(!m_chkFirst.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

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
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkSecond.SetCheck(!m_chkSecond.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

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
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkThird.SetCheck(!m_chkThird.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

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
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
		UpdateData(TRUE);
		m_chkFourth.SetCheck(!m_chkFourth.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

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
//	if(!((CStdDialog*)GetParent())->GetStatusEnable()) return;	
//	ChangeMarket(nMarketType, FALSE, TRUE);
//}
//@��������

void CInputBarDlg::SaveChartType_IniFile(int nType)
{	
	CString strConfig = ((CStdDialog*)GetParent())->m_strConfig;
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
	BOOL bFind = TRUE;
/*@sini
	switch(m_nMarketType)
	{
		case STOCK_CHART:
				m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_STOCK);
				//m_ctrlCodeInput.SetMarketKind(0);
			break;
		case UPJONG_CHART:
				m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_ITEM);
				//m_ctrlCodeInput.SetMarketKind(5);
			break;
		case FUTOPT_CHART:
			m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_FO);
				//m_ctrlCodeInput.SetMarketKind(3);
			break;
		case STOCKFUTOPT_CHART:
				m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, 10);
			break;
		case FOREIGN_CHART:
				m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, 10);
			break;
		default:
			bFind = FALSE;
	}
*/
	if (m_pCodeInput && m_pCodeInput->GetSafeHwnd() != NULL)
	{
		CRect rectComp, ctlRect;
		GetDlgItem(IDC_CTRL_CODEINPUT)->GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);

		m_pCodeInput->GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		if(m_rectCmb.left != ctlRect.left || m_rectCmb.top != ctlRect.top)
		{
			if( ((CStdDialog*)GetParent())->m_nChartType == MINI_MULTI_CHART )
				m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
			else if( ((CStdDialog*)GetParent())->m_nChartType == MINI_SINGLE_CHART )
				m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
			else
				m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width()-40, ctlRect.top + 20);
		}

		if( ((CStdDialog*)GetParent())->m_nChartType != MINI_MULTI_CHART &&
			((CStdDialog*)GetParent())->m_nChartType != MINI_SINGLE_CHART )
		{
			GetDlgItem(IDC_STJONGNAME)->GetWindowRect(ctlRect);
			ScreenToClient(ctlRect);

			m_stJongName.GetWindowRect(rectComp);
			ScreenToClient(rectComp);
			if(m_rectJongName.left != ctlRect.left || m_rectJongName.top != ctlRect.top)
			{
				if(m_hKSJongName && IsWindow(m_hKSJongName))
				{
					m_stJongName.MoveWindow(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
					CWnd::FromHandle(m_hKSJongName)->SetWindowPos(NULL, ctlRect.left, ctlRect.top, rectComp.Width(), 20, SWP_NOZORDER);
					m_rectJongName = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
				}
			}
		}

		//�̹��� �ӽ��ڵ�
		//m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
	}

	
	if(bRequestAP)
	{
//		//�ֽ��� ������ ������  BaseInfo�ʿ��� WMU_GET_KEYCODE ȣ���Ѵ�.
		if (m_nMarketType == CMasterTypeInfo::STOCK_CHART || m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
		{
			if(!m_strCodeFromOutside.GetLength())
			{
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				sCode = GetTypedItemCode();
				sCode.TrimRight();
				if (!sCode.GetLength() && m_pCodeInput)
					::SendMessage(m_pCodeInput->GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
			}
		}
		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//@��������
//		else if( m_nMarketType == FUTOPT_CHART || m_nMarketType == COMMODITY_CHART)
		else if( m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
//@��������
		{
			if (m_pCodeInput)
				::SendMessage(m_pCodeInput->GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
		}
	}
}

void CInputBarDlg::SetKeyCodeChange(WPARAM wp, LPARAM lp)
{	
	// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)
	if(m_bReceiveDataSetCode)
		return;
	
	CString strCode;
	strCode = (LPSTR)lp;	

	if(
		(wp == NID_CODEINPUT	) ||
		(wp == NID_FUTOPTINPUT	) ||
		(wp == NID_UPJONGINPUT	) ||
		(wp == NID_FOREIGNINPUT	)
	)
	{
		// strData -> ����� ���� �ڵ�
		if(strCode.GetLength())
		{
			// �������� �߰� ���� : �������� - ojtaso (20070122)
			if(((CChartMng*)m_pChartMng)->GetAddGraphMode() || m_strCode != strCode)
			{
				CString strTR;
				switch(m_nMarketType)
				{
					case CMasterTypeInfo::STOCK_CHART	: strTR = QUERY_STRJONGMOK	; break;
					case CMasterTypeInfo::UPJONG_CHART	: strTR = QUERY_STRUPJONG	; break;					
					case CMasterTypeInfo::FOREIGN_CHART	: strTR = QUERY_STRFOREIGN	; break;
					case CMasterTypeInfo::FUTOPT_CHART	: strTR = QUERY_STRFUTOPT	; break;
					case CMasterTypeInfo::STOCKFUTOPT_CHART	: strTR = QUERY_STRSTOCKFUTOPT	; break;
					default				: return;
				}

				long lResult = GetParent()->SendMessage(UMSG_CHECK_BE_COMPLETE_QUERYDATA, (WPARAM)(LPTSTR)(LPCTSTR)strTR);
				if(lResult == 0) return;


				//��ǥ �������⸦ �����. ===================================================================
				GetParent()->SendMessage(UMSG_STOP_JIPYOCYCLE, 0, 0);
				//===========================================================================================
				if (((m_nMarketType == CMasterTypeInfo::STOCK_CHART	) && (strCode.GetLength() == CODE_STOCK_LENGTH	)) ||							
					((m_nMarketType == CMasterTypeInfo::UPJONG_CHART	) && (strCode.GetLength() == UPJONG_LENGTH	)) ||
					((strCode.GetLength() == CODE_STOCK_LENGTH	)) ||
					(m_nMarketType  == CMasterTypeInfo::FUTOPT_CHART	) || (m_nMarketType  == CMasterTypeInfo::STOCKFUTOPT_CHART) ||
					((m_nMarketType == CMasterTypeInfo::FOREIGN_CHART))	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
					)
				{
					m_strCodeFromOutside = strCode;
					EnableWindow(FALSE);												
					
					m_strOldCode = GetCurrentCode();
					SetCurrentCode(m_strCodeFromOutside);

					CCodeDataFromInputBar CodeData;
					CodeData.m_strCode = GetCurrentCode();
					CodeData.m_lEndTime = GetEndDate();
					CodeData.m_chType = GetType();
					CodeData.m_lTypeNum = GetTypeNumber();
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					{	
						CString strKorName = GetJongMokNameByCode(GetCurrentCode());
						m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData, (LPARAM)(LPTSTR)(LPCTSTR)strKorName);
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
//void CInputBarDlg::OnChkForeignMainCode(UINT nID)
//{
//	// TODO: Add your control notification handler code here
//	CString strCode;
//	switch ( nID )
//    {
//		case IDC_CHECK_NASDAQ	:   
//			{
//				strCode = _T("COMP");
//				m_chkNasDaq	.SetCheck(TRUE);
//				m_chkDow	.SetCheck(FALSE);
//				m_chkSP500	.SetCheck(FALSE);
//				m_chkNiKei	.SetCheck(FALSE);
//				m_chkTWD	.SetCheck(FALSE);
//			}
//			break;
//		case IDC_CHECK_DOW		:
//			{
//				strCode = _T(".DJI");
//				m_chkNasDaq	.SetCheck(FALSE);
//				m_chkDow	.SetCheck(TRUE);
//				m_chkSP500	.SetCheck(FALSE);
//				m_chkNiKei	.SetCheck(FALSE);
//				m_chkTWD	.SetCheck(FALSE);
//			}
//			break;
//		case IDC_CHECK_SP500	: 
//			{
//				strCode = _T("SPX");
//				m_chkNasDaq	.SetCheck(FALSE);
//				m_chkDow	.SetCheck(FALSE);
//				m_chkSP500	.SetCheck(TRUE);
//				m_chkNiKei	.SetCheck(FALSE);
//				m_chkTWD	.SetCheck(FALSE);
//			}
//			break;
//		case IDC_CHECK_NIKEI	:  
//			{
//				strCode = _T("N225");
//				m_chkNasDaq	.SetCheck(FALSE);
//				m_chkDow	.SetCheck(FALSE);
//				m_chkSP500	.SetCheck(FALSE);
//				m_chkNiKei	.SetCheck(TRUE);
//				m_chkTWD	.SetCheck(FALSE);
//			}
//			break;
//		case IDC_CHECK_TWD		:
//			{
//				strCode = _T("TWI");
//				m_chkNasDaq	.SetCheck(FALSE);
//				m_chkDow	.SetCheck(FALSE);
//				m_chkSP500	.SetCheck(FALSE);
//				m_chkNiKei	.SetCheck(FALSE);
//				m_chkTWD	.SetCheck(TRUE);
//			}
//			break;
//	}
//
//	m_chkNasDaq	.Invalidate();
//	m_chkDow	.Invalidate();
//	m_chkSP500	.Invalidate();
//	m_chkNiKei	.Invalidate();
//	m_chkTWD	.Invalidate();
//
////	m_pCtrlCodeInput->SetDataText(strCode);
//
//	m_strCodeFromOutside = strCode;
////@��������
////	//2005. 08. 27 =============================================================
////	OnBtnForeignSendtr();
////	//==========================================================================
////@��������
//}
//@��������

void CInputBarDlg::Replace_ForeignChartControl()
{
	UINT nMainItemId[] = {
		IDC_CHECK_NASDAQ,   
		IDC_CHECK_DOW	,    
		IDC_CHECK_SP500 , 
		IDC_CHECK_NIKEI ,                  
		IDC_CHECK_TWD	
	};
	

	int nCnt = sizeof(nMainItemId) / sizeof(UINT);
	int nHeight = 0;
	CRect ctlRect;

	GetDlgItem(IDC_CBMARKET)->GetWindowRect(ctlRect);
	ScreenToClient(ctlRect);
	m_cmbChartViewType.MoveWindow(ctlRect);

	for(int i = 0; i < nCnt ; i++)
	{
		GetDlgItem(nMainItemId[i])->GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);
		
		nHeight = ctlRect.Height();
		ctlRect.top = 2;
		ctlRect.bottom = ctlRect.top + nHeight; 

		GetDlgItem(nMainItemId[i])->MoveWindow(ctlRect);
		GetDlgItem(nMainItemId[i])->ShowWindow(SW_SHOW);
	}
	

	//���� ������ �׸��� right ���� �̵���ų Ÿ ��Ʈ���� offset���� �����Ѵ�.		
	//GetDlgItem(nMainItemId[4])->GetWindowRect(ctlRect);
	//ScreenToClient(ctlRect);
	int nOffset = ctlRect.right;

//	GetDlgItem(IDC_CBMARKET)->GetWindowRect(ctlRect);
//	ScreenToClient(ctlRect);

//	ctlRect.left = nOffset; 
//	ctlRect.right = nOffset + 80; 
//	m_cbMarket.MoveWindow(ctlRect);

	//ƽ��ư �����
	m_chkTick.MoveWindow(CRect(0,0,0,0));
	
//	nOffset = ctlRect.right;

//	GetDlgItem(IDC_CHKSEC10)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_CHKSEC30)->ShowWindow(SW_HIDE);
	
//	UINT nID[] = {
//		IDC_CTRL_CODEINPUT	, 
//		//IDC_BTN_FOREIGN_SENDTR,	
//		IDC_STJONGNAME      , 
//		IDC_EDITPERIOD      , 
//		IDC_SPIN_DATECNT    , 
//		IDC_CHKTICK         ,	
//		IDC_CHKMINUTE       , 
//		IDC_CHKDAY          , 
//		IDC_CHKWEEK         , 
//		IDC_CHKMONTH        , 
////		IDC_CHKYEAR         , 
//		IDC_CHK_REVISED     , 
////		IDC_CHECK_LOG       , 
//		IDC_CMB_CHARTTYPE   , 
//		IDC_EDITZOOMVALUE   , 
//		IDC_SPINZOOMVALUE   , 
//		IDC_STATICDIV       , 
//		IDC_EDITCNTOFDATALL , 
//		IDC_BTREQUESTDATMORE, 
//		IDC_CHK_DATALIST	, 
//	};

	UINT nID[] = {
		IDC_CTRL_CODEINPUT	, 
		//IDC_BTN_FOREIGN_SENDTR,	
		IDC_STJONGNAME      , 
		IDC_CHKDAY          , 
//		IDC_CHKHOUR         , 
		IDC_CHKMINUTE       , 
//		IDC_CHKFIRST        , 
//		IDC_CHKSECOND       , 
//		IDC_CHKTHIRD        , 
//		IDC_CHKFOURTH       , 
		IDC_EDITPERIOD      , 
		IDC_BTPERIOD		,
		IDC_CHKWEEK         , 
		IDC_CHKMONTH        , 
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

	nCnt = sizeof(nID) / sizeof(UINT);

	
	int nWidth = 0;
	for(i = 0; i < nCnt ; i++)
	{
		ctlRect.left	= 0;
		ctlRect.top		= 0;
		ctlRect.right	= 0;
		ctlRect.bottom	= 0;

		if(	(nID[i] == IDC_CHK_REVISED	) //||
//			(nID[i] == IDC_CHECK_LOG	) 
		)
		{	
			continue;
		}
		
		if(nID[i] == IDC_STJONGNAME)
		{
			GetDlgItem(nID[i])->GetWindowRect(ctlRect);		
			ScreenToClient(ctlRect);
			nWidth = ctlRect.Width();
			ctlRect.left	= nOffset;
			ctlRect.right	= ctlRect.left + nWidth;			
			GetDlgItem(nID[i])->MoveWindow(ctlRect);

			if(m_hKSJongName && IsWindow(m_hKSJongName))
			{
				CWnd::FromHandle(m_hKSJongName)->MoveWindow(ctlRect.left, ctlRect.top, ctlRect.Width(), ctlRect.Height());
				m_rectJongName = ctlRect;
			}
			nOffset = ctlRect.right;

			continue;
		}

		GetDlgItem(nID[i])->GetWindowRect(ctlRect);		
		if(nID[i] == IDC_CTRL_CODEINPUT)
			ctlRect.right -= 50;
		
		ScreenToClient(ctlRect);
		nWidth = ctlRect.Width();

		if(nID[i] == IDC_BTN_FOREIGN_SENDTR)
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
//void CInputBarDlg::OnBtnForeignSendtr() 
//{
//	EnableWindow(FALSE);												
//	
//	m_strOldCode = GetCurrentCode();
//	SetCurCodeNameToControl(m_strCodeFromOutside);
//
//	
//
//	CCodeDataFromInputBar CodeData;
//	CodeData.m_strCode = m_strCodeFromOutside;
//	CodeData.m_lEndTime = GetEndDate();
//	CodeData.m_chType = GetType();
//	CodeData.m_lTypeNum = GetTypeNumber();
//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//	{
//		SetCurCodeNameToControl(m_strCodeFromOutside);							
//		m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
//	}
//	else
//	{
//		SetShareCodeChanged(TRUE);
//		m_ctrlCodeInput.SetDataText(_T(""));
//		m_strOldCode = GetCurrentCode();
//		m_strCodeFromOutside = _T("");
//		SetCurCodeNameToControl(_T(""));
//	}
//	
//	EnableWindow(TRUE);			
//}
//@��������	

void CInputBarDlg::OnSelchangeCmbType() 
{		
	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	int nIdx = m_cmbChartViewType.GetCurSel();
	if(nIdx == CB_ERR) return;
	if(m_nCurrentMode == nIdx) return; //���� ���� Return

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
			if( !((CStdDialog*)GetParent())->CheckDataTypeForReplay() && m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			{
				AfxMessageBox("������Ʈ�� ������ ��Ʈ�϶��� ������ �����մϴ�");
				m_cmbChartViewType.SetCurSel(m_nCurrentMode);
				return;
			}

			//=================================================================================
			if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
			{
				AfxMessageBox("�������� �ֽİ� ���� ���� �����մϴ�.");
				m_cmbChartViewType.SetCurSel(m_nCurrentMode);
				return;
			}
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
	if(((CStdDialog*)GetParent())->IsMultiItemChart())
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
//	int nReplayOffset = 40;

	if(bReplay) //�� ��ư �̵�
	{
		//0. ����� ������ ��Ʈ ���¸� �����Ѵ�.
		m_stJongName.GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);
		m_stJongName.MoveWindow(ctlRect.left, ctlRect.top, ctlRect.Width(), ctlRect.Height());
		if(m_hKSJongName && IsWindow(m_hKSJongName))
		{
			CWnd::FromHandle(m_hKSJongName)->MoveWindow(ctlRect.left, ctlRect.top, ctlRect.Width(), ctlRect.Height());
			m_rectJongName = ctlRect;
		}

		CString sFile = ((CStdDialog*)GetParent())->m_strConfig;
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
		if(m_hKSJongName && IsWindow(m_hKSJongName))
		{
			CWnd::FromHandle(m_hKSJongName)->MoveWindow(ctlRect.left, ctlRect.top, ctlRect.Width(), ctlRect.Height());
			m_rectJongName = ctlRect;
		}
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
		m_ExChangeBeforeInfo.bShowExtToolBar = ((CStdDialog*)GetParent())->GetShowExtInputBarDlg();
	
		//�ϴ� ���� ����.2005.12.08
		((CStdDialog*)GetParent())->SetShowExtInputBarDlg(FALSE);
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
			IDC_CHKSEC			,
			IDC_CHKFIRST        , 
			IDC_CHKSECOND       , 
			IDC_CHKTHIRD        , 
			IDC_CHKFOURTH       , 
//			IDC_CMB_BONG		,
//			IDC_CHKYEAR			,			
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
		m_ExChangeBeforeInfo.bTimeSync = ((CStdDialog*)GetParent())->GetTimeSyncFlag();		
		
		((CStdDialog*)GetParent())->SetTimeSyncFlag(TRUE);

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

			ctlRect.top = 2;
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

			//((CStdDialog*)GetParent())->RqstTRForReplay();
		}

	
	}
	else
	{	
		if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			GetParent()->SendMessage(UMSG_DESTROY_FLOATINGHOGA, 1);

		//�ϴ� ���� ������ ������ ���� .2005.12.08
		if(m_ExChangeBeforeInfo.bShowExtToolBar)
			((CStdDialog*)GetParent())->SetShowExtInputBarDlg(TRUE);

		m_chkTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ƽ", TRUE);
		m_chkMinute.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE);
	
		m_chkExtBar1.EnableWindow(TRUE);

		

		EnableWindow(TRUE);

	//1. ���� Stop & Clear
		BOOL bResult = FALSE;
		bResult = ((CStdDialog*)GetParent())->RunReplay((int)STOP); 		


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
			IDC_CHKMINUTE		,
			IDC_CHKSEC			,
			IDC_CHKTICK			,
			IDC_CHKFIRST        , 
			IDC_CHKSECOND       , 
			IDC_CHKTHIRD        , 
			IDC_CHKFOURTH       , 
//			IDC_CHKYEAR			,			
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

		BOOL bMultiChartItem = ((CStdDialog*)GetParent())->IsMultiItemChart();
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
		((CStdDialog*)GetParent())->SetTimeSyncFlag(m_ExChangeBeforeInfo.bTimeSync);

		//����� ��Ʈ ������ �о�´�.
		//0. ����� ������ ��Ʈ ���¸� �����Ѵ�.
		((CChartMng*)m_pChartMng)->LoadChart(((CStdDialog*)GetParent())->m_strConfig);

		
	}
}

void CInputBarDlg::SetMultiChartItemMode()
{
	if(!((CStdDialog*)GetParent())->IsMultiItemChart())
	{
		UINT nShowItemId[] = {		
				IDC_STATICDIV		,
				IDC_STC_TOTALCNT	,
				IDC_EDITCNTOFDATALL	,
				IDC_BTREQUESTDATMORE,
				IDC_BTSAVECHART
		};

		int nCnt = sizeof(nShowItemId) / sizeof(UINT);		
		CRect ctlRect;
		int nOffset = 0;
		for(int i = 0; i < nCnt ; i++)
		{
			if(nShowItemId[i] == IDC_STC_TOTALCNT)
			{
				GetDlgItem(nShowItemId[i])->ShowWindow(SW_HIDE);
				continue;
			}

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

		bResult = ((CStdDialog*)GetParent())->RunReplay(PAUSE); 
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

				bResult = ((CStdDialog*)GetParent())->RunReplay(PLAY, m_pReplayInfo);
				if(!bResult)
				{

				}
			}
		}
		else
		{
			bResult = ((CStdDialog*)GetParent())->RunReplay(PLAY);
			OnSelchangeCmbSpeed();
			m_btnHandleReplay.FP_vSetImage( m_pIL_W20H20, 69, 68);
		}
			
		m_bStop = FALSE;
		
		if(!bResult) 
		{
			((CStdDialog*)GetParent())->RunReplay( STOP );
	
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
//		((CStdDialog*)GetParent())->RunReplay(PLAY); 
//	else //Play ���¿���  �Ͻ����� ���·� ����ȴ�.
//		((CStdDialog*)GetParent())->RunReplay(PAUSE); 

	((CStdDialog*)GetParent())->RunReplay( STOP );
	
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
		((CStdDialog*)GetParent())->ChangeSpeed(nSpeed);
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
	if(IsMiniBar())
		return;

	if(strCode == _T(""))
	{
		m_strJonMokName = _T("");
//@��������		m_stJongName.SetText(_T(""));
		UpdateData(FALSE);
		return;
	}
/*
// --> [Edit]  ������ 2008/07/22		( ����� ���ϴ� �κ��ε� ���⼭�� ������� ���°� ���Ƽ� ����... �׳� �����ڵ带 ����� ���� �ִ´�. )
	CString strCodeName;
	long lPos = -1;
	int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(strCode);
	switch(nMarketType)
	{
		case STOCK_CHART:
			strCode.TrimRight();
			strCodeName		= m_pDataManager->GetCodeInfo("JMASTER", 0, strCode);			
			if(strCodeName.GetLength()<1)
				strCodeName		= m_pDataManager->GetCodeInfo("KMASTER", 0, strCode);
			if(strCodeName.GetLength()<1)
				strCodeName		= m_pDataManager->GetCodeInfo("WMASTER", 0, strCode);

			lPos = strCodeName.Find('&');
			if(lPos>0)
			{
				strCodeName.Insert(lPos,'&');
			}
			break;
		case UPJONG_CHART:
			strCodeName		= m_pDataManager->GetCodeInfo("UMASTER", 0, strCode);			
			break;
		case FUTOPT_CHART:
			strCodeName		= m_pDataManager->GetCodeInfo("FMASTER", 0, strCode);			
			if(strCodeName.GetLength()<1)
				strCodeName		= m_pDataManager->GetCodeInfo("PMASTER", 0, strCode);
			break;
		case STOCKFUTOPT_CHART:
			strCodeName		= m_pDataManager->GetJongmokNameFromCode(strCode);
//			strCodeName		= strCodeName.Mid(0, strCodeName.ReverseFind('('));
			break;
		case FOREIGN_CHART:
			strCodeName		= m_pDataManager->GetJongmokNameFromCode(strCode);
//			strCodeName		= m_pDataManager->GetCodeInfo("FRMASTER", 0, strCode);		
			lPos = strCodeName.Find('&');
			if(lPos>0)
			{
				strCodeName.Insert(lPos,'&');
			}
			break;
//@��������
//		case ELW_CHART:
//			strCodeName		= m_pDataManager->GetCodeInfo("WMASTER", 0, strCode);			
//			break;
//		case COMMODITY_CHART:		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//			strCodeName		= m_pDataManager->GetCodeInfo("", 1, strCode);			
//			break;
//@��������
	}
	m_strJonMokName = strCodeName;
// <-- [Edit]  ������ 2008/07/22
*/
	m_strJonMokName = strCode;
//@��������	m_stJongName.SetText(strCodeName);
	UpdateData(FALSE);
}


CString CInputBarDlg::GetJongMokNameByCode(CString strCode, LPCTSTR lpszTRCode/* = NULL*/)
{
// --> [Edit]  ������ 2008/07/22		( �̺κп��� �� ������� ���... �ʿ���°� ������ ���ƺ���.. ���� )
/*
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
		case STOCK_CHART:
			strCode.TrimRight();
			strCodeName		= m_pDataManager->GetCodeInfo("JMASTER", 0, strCode);			
			if(strCodeName.GetLength()<1)
				strCodeName		= m_pDataManager->GetCodeInfo("KMASTER", 0, strCode);
			if(strCodeName.GetLength()<1)	// ELW
				strCodeName		= m_pDataManager->GetCodeInfo("WMASTER", 0, strCode);			
			break;
		case UPJONG_CHART:
			strCodeName		= m_pDataManager->GetCodeInfo("UMASTER", 0, strCode);			
			break;
		case FUTOPT_CHART:
			strCodeName		= m_pDataManager->GetCodeInfo("FMASTER", 0, strCode);			
			if(strCodeName.GetLength()<1)
				strCodeName		= m_pDataManager->GetCodeInfo("PMASTER", 0, strCode);
			break;
		case STOCKFUTOPT_CHART:
			strCodeName		= m_pDataManager->GetJongmokNameFromCode(strCode);
			strCodeName		= strCodeName.Mid(0, strCodeName.ReverseFind('('));
			break;
		case FOREIGN_CHART:
			strCodeName		= m_pDataManager->GetCodeInfo("FRMASTER", 0, strCode);		
			{
				long lPos = strCodeName.Find('&');
				if(lPos>0)
					strCodeName.Insert(lPos,'&');
			}
			break;
//@��������
//		case ELW_CHART:
//			strCodeName		= m_pDataManager->GetCodeInfo("WMASTER", 0, strCode);			
//			break;
//		case COMMODITY_CHART:		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//			strCodeName		= m_pDataManager->GetCodeInfo("", 0, strCode);			
//			break;
//@��������
	}
	return strCodeName;
*/
// <-- [Edit]  ������ 2008/07/22
	return strCode;
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
	else 
		return CMasterTypeInfo::STOCK_CHART;
}

void CInputBarDlg::SetCurrentCode(CString strCode)
{
	m_strCode = strCode;

	//<<solomon.  ���н��� �ٲ� �����ڵ� ������.
	m_strCode.TrimRight();
	if( !m_strCode.IsEmpty() )
	{
		int nMajor = (int)::SendMessage(((CStdDialog*)GetParent())->m_hPlatform, WM_USER, MAKEWPARAM(variantDLL, majorCC), (LPARAM)(LPCTSTR)"");

		CString strTemp;
		strTemp.Format("%s\t%d\t%s", "SetCode9065", nMajor, CStdDialog::GetRealItemCode(m_strCode));
		::SendMessage(((CStdDialog*)GetParent())->m_hPlatform, WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)strTemp.operator LPCTSTR());
	}
	//>>
}

CString CInputBarDlg::GetCurrentCode()
{
	return m_strCode;
}

// 	void CInputBarDlg::SetEditZoomValue(CString strValue)
// 	{
// 		if(IsMiniBar())
// 			return;
// 
// 		UpdateData(TRUE);
// 
// 		CString strZoomValue, strTotValue;
// 		// ���������� Total Count�� ��� : �������� - ojtaso (20070704)
// 	//	m_editCntOfDatAll.GetWindowText(strTotValue); strTotValue.TrimLeft(); strTotValue.TrimRight();
// 		GetDlgItemText(IDC_STC_TOTALCNT, strTotValue);
// 	//	if(strTotValue.GetLength() && atoi(strTotValue) > 0)
// 	//	{
// 	//		if( atoi(strValue) > atoi(strTotValue) )
// 	//			strValue = strTotValue;
// 	//	}	
// 		
// 		strZoomValue = strValue;
// 		if(atoi(strZoomValue) < 0)
// 		{
// 			strZoomValue = "200";
// 			((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetNew_OnePageDataCount(200);
// 		}
// 		
// 		m_editZoomValue.SetWindowText(strZoomValue);
// 
// 		UpdateData(FALSE);
// 	}

void CInputBarDlg::SetEditZoomValue(CString strValue)
{
	if(IsMiniBar())
		return;
	
	{	//@Sol-100104SM005
		if(strValue.Compare(m_szOldZoomValue)==0) return;
	}
	
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
	{
		strZoomValue = "200";
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetNew_OnePageDataCount(200);
	}
	
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
		if (m_pCodeInput)	m_pCodeInput->EnableWindow(FALSE);
		m_cmbDayBefore.EnableWindow(FALSE);
		m_cmbChartViewType.EnableWindow(FALSE);
//@��������		m_btnForSendTR.EnableWindow(FALSE);
//@��������		m_chkDow.EnableWindow(FALSE);
//@��������		m_chkTWD.EnableWindow(FALSE);
//@��������		m_chkSP500.EnableWindow(FALSE);
//@��������		m_chkNiKei.EnableWindow(FALSE);
//@��������		m_chkNasDaq.EnableWindow(FALSE);
//@��������		m_chkDataListWnd.EnableWindow(FALSE);
		m_cmbChartType.EnableWindow(FALSE);
		m_chkWeek.EnableWindow(FALSE);
		m_chkMonth.EnableWindow(FALSE);
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

		m_staticReplayTime.ShowWindow(SW_SHOW);	//2006. 1.9 �ű� �߰� =====================
		m_prgBarReplay.ShowWindow(SW_SHOW);		//2006. 1.9 �ű� �߰� =====================	

		GetDlgItem(IDC_EDITZOOMVALUE	)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================	
		GetDlgItem(IDC_SPINZOOMVALUE	)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================
//		GetDlgItem(IDC_STATICDIV		)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================
		GetDlgItem(IDC_EDITCNTOFDATALL	)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================
		GetDlgItem(IDC_BTREQUESTDATMORE	)->ShowWindow(SW_HIDE); //2006. 1.11 �ű� �߰� =====================

		m_btnHandleReplay.FP_vSetImage( m_pIL_W20H20, 69, 68);
	}
	else if(nReplayMode == PAUSE)
	{		
		m_cmbReplaySpeed.EnableWindow(TRUE);

		m_btnStopReplay.EnableWindow(TRUE);
		m_btnHandleReplay.EnableWindow(TRUE);
		if (m_pCodeInput)	m_pCodeInput->EnableWindow(FALSE);
		m_cmbReplayHour.EnableWindow(FALSE);
		m_cmbReplayMin.EnableWindow(FALSE);
		m_cmbDayBefore.EnableWindow(FALSE);
		m_cmbChartViewType.EnableWindow(FALSE);
//@��������		m_btnForSendTR.EnableWindow(FALSE);
//@��������		m_chkDow.EnableWindow(FALSE);
//@��������		m_chkTWD.EnableWindow(FALSE);
//@��������		m_chkSP500.EnableWindow(FALSE);
//@��������		m_chkNiKei.EnableWindow(FALSE);
//@��������		m_chkNasDaq.EnableWindow(FALSE);
//@��������		m_chkDataListWnd.EnableWindow(FALSE);
		m_cmbChartType.EnableWindow(FALSE);
		m_chkWeek.EnableWindow(FALSE);
		m_chkMonth.EnableWindow(FALSE);
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
		if (m_pCodeInput)	m_pCodeInput->EnableWindow(TRUE);
		m_cmbDayBefore.EnableWindow(TRUE);
		m_cmbReplayHour.EnableWindow(TRUE);
		m_cmbReplayMin.EnableWindow(TRUE);
		m_cmbReplaySpeed.EnableWindow(TRUE);
		m_cmbChartViewType.EnableWindow(TRUE);
//@��������		m_btnForSendTR.EnableWindow(TRUE);
//@��������		m_chkDow.EnableWindow(TRUE);
//@��������		m_chkTWD.EnableWindow(TRUE);
//@��������		m_chkSP500.EnableWindow(TRUE);
//@��������		m_chkNiKei.EnableWindow(TRUE);
//@��������		m_chkNasDaq.EnableWindow(TRUE);
//@��������		m_chkDataListWnd.EnableWindow(TRUE);
		m_cmbChartType.EnableWindow(TRUE);
		m_chkWeek.EnableWindow(TRUE);
		m_chkMonth.EnableWindow(TRUE);
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

//		GetDlgItem(IDC_EDITZOOMVALUE	)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================	
//		GetDlgItem(IDC_SPINZOOMVALUE	)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================
//		GetDlgItem(IDC_STATICDIV		)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================
//		GetDlgItem(IDC_EDITCNTOFDATALL	)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================
//		GetDlgItem(IDC_BTREQUESTDATMORE	)->ShowWindow(SW_SHOW); //2006. 1.11 �ű� �߰� =====================

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

			//((CStdDialog*)GetParent())->RqstTRForReplay();
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
		if(m_pChartMng)
		{
			if( ((CChartMng*)m_pChartMng)->m_bNeedForceCode == FALSE )
			{

				KillTimer(ID_SHARE_CODE_WAIT);
#ifdef _DEBUG	
				OutputDebugString("Load �ȳ��� �־ Timer Kill");
#endif
				return;

			}

			CCodeDataFromInputBar CodeData;

			CString strCode;
//@SolomonGNF -->
// 			if(m_nMarketType == STOCK_CHART)
// 			{
// 				strCode = GetLastHistoryCode(STOCK_CHART);
// 			}
// 			else if(m_nMarketType == FOREIGN_CHART)
// 			{
// 				// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
// 				if(m_strForeignCode.IsEmpty())
// 					strCode = _T(".DJI");	// "NAS@IXIC"
// 				else
// 					strCode = m_strForeignCode;
// 			}
// 			else if(m_nMarketType == UPJONG_CHART)
// 			{
// 				// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
// 				if(m_strUpjongCode.IsEmpty())
// 					strCode = _T("001");
// 				else
// 					strCode = m_strUpjongCode;
// 			}
// 			else if(m_nMarketType == FUTOPT_CHART)
// 			{
// 				CStringArray arrCode, arrName;
// 				m_pDataManager->GetRecentCodeNameArray(FUTOPT_CHART, arrCode, arrName);
// 				if(arrCode.GetSize() > 0)
// 					strCode = arrCode.GetAt(0);
// 				else
// 				{
// 					// CODE_FUTURE		"0301"		// ����
// 					m_pDataManager->GetMasterData("0301", arrCode, arrName);
// 					if(arrCode.GetSize() > 0)
// 						strCode = arrCode.GetAt(0);
// 				}
// 
// 				if(strCode.GetLength() < 1)
// 				{
// 					KillTimer(ID_SHARE_CODE_WAIT);
// 					return;
// 				}
// 			}
// 			else if(m_nMarketType == STOCKFUTOPT_CHART)
// 			{
// 				CStringArray arrCode, arrName;
// 				m_pDataManager->GetRecentCodeNameArray(STOCKFUTOPT_CHART, arrCode, arrName);
// 				if(arrCode.GetSize() > 0)
// 					strCode = arrCode.GetAt(0);
// 				else
// 				{
// 					// CODE_J_FUTURE		"0316"		// �ֽļ���
// 					m_pDataManager->GetMasterData("0316", arrCode, arrName);
// 					if(arrCode.GetSize() > 0)
// 						strCode = arrCode.GetAt(0);
// 				}
// 
// 				if(strCode.GetLength() < 1)
// 				{
// 					KillTimer(ID_SHARE_CODE_WAIT);
// 					return;
// 				}
// 			}
// 			else
			{
				KillTimer(ID_SHARE_CODE_WAIT);
				return;
			}
//@SolomonGNF <--

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
	if(!((CStdDialog*)GetParent())->GetStatusEnable()) return;
	//{{2007.03.15 by LYH
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	if(m_bUseReplayChartMode && m_nCurrentMode == REPLAY_ITEM)
	{
		m_cbMarket.SetCurSel(m_nMarketType);
		AfxMessageBox("���� ��忡���� ������ �Ұ��մϴ�.");
		return;
	}
	//}}
	int nSelMarketType = m_cbMarket.GetCurSel();

	// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
	CString strCode = GetCurrentCode();
	int nType;
	int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(strCode, nType);
	if(CMasterTypeInfo::FOREIGN_CHART == nMarketType)
		m_strForeignCode = strCode;
	else if(CMasterTypeInfo::UPJONG_CHART == nMarketType)
		m_strUpjongCode = strCode;

	m_strCodeFromOutside = "";
//@��������
	((CStdDialog*)GetParent())->ChangeStockMode(nSelMarketType); 
//@��������
//	if(m_pCtrlCodeInput == m_pCtrlCodeInputUpjong)
//	{
//		m_btChangeUpjong.ShowWindow(SW_SHOW);
//	}
//	else
//	{
//		m_btChangeUpjong.ShowWindow(SW_HIDE);
//	}

	((CChartMng*)m_pChartMng)->m_bNeedForceCode = TRUE;
	SetTimer(ID_SHARE_CODE_WAIT, 200, NULL);
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
	m_strCodeFromOutside = ((CStdDialog*)GetParent())->GetLinkInfoData();
	if(m_strCodeFromOutside.GetLength())
	{
		((CChartMng*)m_pChartMng)->m_bNeedForceCode = TRUE;
		return;
	}

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
	else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_strCodeFromOutside = GetLastHistoryCode(CMasterTypeInfo::FOREIGN_CHART);
		if(m_strCodeFromOutside.IsEmpty())
			m_strCodeFromOutside = _T(".DJI");
	}
}
//@�����߰�

void CInputBarDlg::SetCodeInit()
{
	if(m_pChartMng)
	{
		if( ((CChartMng*)m_pChartMng)->m_bNeedForceCode == FALSE )
		{
#ifdef _DEBUG	
			OutputDebugString("Load �ȳ��� �־ Timer Kill");
#endif
			return;
		}

		CCodeDataFromInputBar CodeData;

		//{{2007.07.23 by LYH SetRecentCode�� ��ü
//		CString strCode;
//		if(m_nMarketType == STOCK_CHART || m_nMarketType == ELW_CHART)
//		{
//			//::SendMessage(m_pCtrlCodeInput->GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
//			//strCode = m_pCtrlCodeInput->GetDataText();
//			strCode = GetLastKey(1);
//		}
//		else if(m_nMarketType == FOREIGN_CHART)
//			strCode = _T("COMP");
//		else if(m_nMarketType == UPJONG_CHART)
//			strCode = _T("20001");
//		else if( m_nMarketType == FUTOPT_CHART)
//		{
//			//::SendMessage(m_pCtrlCodeInput->GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
//			//strCode = m_pCtrlCodeInput->GetDataText();
//			strCode = GetLastKey(8);
//			if(strCode.GetLength()<1)
//				strCode = GetLastKey(12);
//
////			strCode = m_pCtrlCodeInput->GetDataText();
////			CList<CFMaster, CFMaster>*	pListF = NULL;
////			if(m_pDataManager != NULL)
////				pListF = (CList<CFMaster, CFMaster>*)m_pDataManager->GetCodePointer("FMASTER");
////			if(pListF)
////			{
////				CFMaster fMaster = pListF->GetHead();
////				strCode.Format("%s", fMaster.m_szCode);
////			}
//		}
//		else
//			return;

		CodeData.m_strCode = m_strCodeFromOutside;
		CodeData.m_lEndTime = GetEndDate();
		CodeData.m_chType = GetType();
		CodeData.m_lTypeNum = GetTypeNumber();
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		{					
			m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
		}
#ifdef _DEBUG
		OutputDebugString("������� �� ���� ��� �ε� ��ȣ ����");
#endif
	}
}

void CInputBarDlg::UpdateBtNCbValue()
{
	if(IsMiniBar())
		return;
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
//	if(m_bUseReplayChartMode)
//		return;

//{{2007.07.05 by LYH �������� ������ ����
//	if(m_nDataAddMode == REPLAY_ITEM)
//		return;
//}}
	char szTemp[64];
	if(DAY_DATA_CHART == m_chType)  // ��
	{
		sprintf(szTemp,"%d",m_lValueOfButton[0]);
		m_chkFirst.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfButton[1]);
		m_chkSecond.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfButton[2]);
		m_chkThird.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfButton[3]);
		m_chkFourth.SetBtnTitle(szTemp);
	}
	else if (HOUR_DATA_CHART == m_chType)	// ��
	{
		sprintf(szTemp,"%d",m_lValueOfHourButton[0]);
		m_chkFirst.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfHourButton[1]);
		m_chkSecond.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfHourButton[2]);
		m_chkThird.SetBtnTitle(szTemp);
	}
	else if (MIN_DATA_CHART == m_chType)	// ��
	{
		sprintf(szTemp,"%d",m_lValueOfMinButton[0]);
		m_chkFirst.SetBtnTitle(szTemp);
		//m_chkFirst.SetBtnTitle("��", "��");
		sprintf(szTemp,"%d",m_lValueOfMinButton[1]);
		m_chkSecond.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfMinButton[2]);
		m_chkThird.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfMinButton[3]);
		m_chkFourth.SetBtnTitle(szTemp);
	}
	else if (SEC_DATA_CHART == m_chType)	// ��
	{
		sprintf(szTemp,"%d",m_lValueOfSecButton[0]);
		m_chkFirst.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfSecButton[1]);
		m_chkSecond.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfSecButton[2]);
		m_chkThird.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfSecButton[3]);
		m_chkFourth.SetBtnTitle(szTemp);
	}
	else if (TICK_DATA_CHART == m_chType)	// ƽ
	{
		sprintf(szTemp,"%d",m_lValueOfTickButton[0]);
		m_chkFirst.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfTickButton[1]);
		m_chkSecond.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfTickButton[2]);
		m_chkThird.SetBtnTitle(szTemp);
		sprintf(szTemp,"%d",m_lValueOfTickButton[3]);
		m_chkFourth.SetBtnTitle(szTemp);
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
	CString strDataPath = ((CStdDialog*)GetParent())->m_strRootPath + "\\data\\";
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
		//case EN_CHANGE :
		//	m_stcc8.SetWindowText("CodeCombo EN_CHANGE");
		//	break;
		//case CBN_DROPDOWN :
		//	m_stc.SetWindowText("CodeCombo CBN_DROPDOWN" );
		//	break;
		//case CBN_CLOSEUP : 
		//	m_stc2.SetWindowText("CodeCombo CBN_CLOSEUP" );
		//	break;
		case CBN_SELCHANGE :
			//AfxMessageBox("CodeCombo CBN_SELCHANGE" );
			//SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_pCtrlCodeInput->GetDataText());
			break;
		case CBN_EDITCHANGE :
			//AfxMessageBox("CodeCombo CBN_EDITCHANGE" );
			break;
		//case CBN_SELENDCANCEL :
		//	m_stc5.SetWindowText("CodeCombo CBN_SELENDCANCEL" );
		//	break;
		case CBN_SELENDOK :
			//AfxMessageBox("CodeCombo CBN_SELENDOK" );
		//	SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_pCtrlCodeInput->GetDataText());
			break;
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}

void CInputBarDlg::OnSelchangeCodeInput()
{
	if(!m_bSelectAction)
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		SetKeyCodeChange(NID_CODEINPUT, ( LPARAM)( LPCTSTR)GetTypedItemCode());
//	else
//		m_bSelectAction = FALSE;
}

//008 {{
void CInputBarDlg::InitUpjongCodeCombo(UINT nUpjongType)
{
//@��������
//	if(m_pCtrlCodeInput->GetMarketKind() != nUpjongType)
//	{
//		//m_pCtrlCodeInput->InitCodeInput(this, IDC_CTRL_CODEINPUT, 10);
//		if(m_pCtrlCodeInputUpjong)
//		{
//			m_pCtrlCodeInputUpjong->SetMarketKind(nUpjongType);
//			m_nUpjongType = nUpjongType;
//		}
//		if(nUpjongType == 100)
//		{
//			if(!m_btChangeUpjong.IsWindowVisible())
//				m_btChangeUpjong.ShowWindow(SW_SHOW);
//
//			m_btChangeUpjong.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "K", FALSE);
//			m_btChangeUpjong.Invalidate();
//		}
//		else
//		{
//			m_btChangeUpjong.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "Q", FALSE);
//			m_btChangeUpjong.Invalidate();
//		}
//	}	
//@��������
}
//008 }}

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

//@��������	
//void CInputBarDlg::OnChkhour() 
//{
//	//�������� �� �����Ͱ� �ƴϾ����� ����ڰ� �Է��� �����Ѵ�.========
//	//05.09.12
//	if(m_chType != HOUR_DATA_CHART)
//		SetUseUserInputPeriod(FALSE);
//	//==========================================================
//
//	m_chType = HOUR_DATA_CHART;
//
//	UpdateBtNCbValue();	// �ֱ��ư Set
//
//	//m_nUnitPos = 1;
//
//	//================================================================================
//	// 2005. 09. 08
//	// ����ڰ� �Է��� ������ �����ϵ��� �ش޶� ��ȭ �䱸
//	//================================================================================	
//	CString sTemp;
//	if(!GetUseUserInputPeriod())
//	{
//		if(m_pChartMng->GetSafeHwnd())
//			m_nHour = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 7);	
//
//		sTemp.Format("%d", m_nHour);
//		m_editPeriod.SetWindowText(sTemp);
//	}
//	else
//	{
//		m_editPeriod.GetWindowText(sTemp);
//		m_nHour= atoi(sTemp);
//	}
//	//=======================================================================================
//
//	SetType();
//
//	SetUseUserInputPeriod(FALSE);	
//}
//@��������	

void CInputBarDlg::SetViewCount(long nViewCount) 
{
	m_lCntInView[m_chType-'0'] = nViewCount;
}

int CInputBarDlg::GetAllCount()
{
	CString strCount;
	m_editCntOfDatAll.GetWindowText(strCount);
	int retVal = atoi(strCount);
	if(retVal<=0)
		return 200;
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

	rcClient.bottom = rcClient.top + 2;
	dc.FillSolidRect(rcClient, RGB(255,255,255));
}

void CInputBarDlg::ResetUnitValue(BOOL bValueSet) 
{
	if(bValueSet)
	{
		if(m_nUnitPos >= 0)
		{
			if (MIN_DATA_CHART == m_chType)		// 30��
				m_nUnitValue = m_lValueOfMinButton[m_nUnitPos];
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
	BOOL bRet = FALSE;
	int nMarketType = CMasterTypeInfo::STOCK_CHART;

	if(nGetMarketType >= 0)
	{
		switch(nGetMarketType) {
		case 1:
			nMarketType = CMasterTypeInfo::STOCK_CHART;
			break;
		case 8:
		case 12:
			nMarketType = CMasterTypeInfo::FUTOPT_CHART;
			break;
		case 79:
		case 80:
		case 81:
		case 82:
		case 83:
		case 84:
		case 85:
		case 86:
			nMarketType = CMasterTypeInfo::FOREIGN_CHART;
			break;
		case 100:
			return bRet;
		default:
			return bRet;
		}
	}
	else
	{
		int nType;
		nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(strCode, nType);
	}
	
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		bRet = ChangeMarket(CMasterTypeInfo::STOCK_CHART);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			CString strCodeInputCode = GetTypedItemCode();
			if(strCode.GetLength() && strCodeInputCode != strCode)
			{
				SetShareCodeChanged(TRUE);
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strCode));
				SetShareCodeChanged(FALSE);
			}
		}
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		bRet = ChangeMarket(CMasterTypeInfo::UPJONG_CHART);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			CString strCodeInputCode = GetTypedItemCode();
			if(strCode.GetLength() && strCodeInputCode != strCode)
			{
				SetShareCodeChanged(TRUE);
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strCode));
				SetShareCodeChanged(FALSE);
			}
		}
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		bRet = ChangeMarket(CMasterTypeInfo::FUTOPT_CHART);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			CString strCodeInputCode = GetTypedItemCode();
			if(strCode.GetLength() && strCodeInputCode != strCode)
			{
				SetShareCodeChanged(TRUE);
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strCode));
				SetShareCodeChanged(FALSE);
			}
		}
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		bRet = ChangeMarket(CMasterTypeInfo::STOCKFUTOPT_CHART);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			CString strCodeInputCode = GetTypedItemCode();
			if(strCode.GetLength() && strCodeInputCode != strCode)
			{
				SetShareCodeChanged(TRUE);
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strCode));
				SetShareCodeChanged(FALSE);
			}
		}
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		bRet = ChangeMarket(CMasterTypeInfo::FOREIGN_CHART);
		if(bRet)
			InitBaseInfo(NULL, FALSE);
	}

	return FALSE;
}

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
			else
				strCode = m_pDataManager->GetLastCode("03");

			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, FUTOPT_LENGTH);
			}
		}
		// ����+�ɼ�+��������+�ֽĿɼ�
		else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
		{
			CStringArray arrCode, arrName;
			m_pDataManager->GetRecentCodeNameArray(3, arrCode, arrName);
			if(arrCode.GetSize() > 0)
				strCode = arrCode.GetAt(0);
			else
				strCode = "110C6000";//m_pDataManager->GetLastCode("03");
			
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, FUTOPT_LENGTH);
			}
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
	}

	return strCode;
}
//@�����߰�

void CInputBarDlg::OnEnSetfocusEditFocus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_pCodeInput)	m_pCodeInput->SetFocus();
}

// �������񿡼� ���õ� ��Ʈ ���� ǥ�� : �������� - ojtaso (20070223)
void CInputBarDlg::InitChartMulitItem(ST_SELCHART* pSelChart)
{
	//05. 09. 08
	SetUseUserInputPeriod(FALSE);

	m_bSettingInitData = TRUE;

	char oldChType = m_chType;
	m_chType = pSelChart->m_cType;

//	CString strZoomValue;
//	strZoomValue.Format("%d",pSelChart->m_lCount);
//	SetEditZoomValue(strZoomValue);

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

			if(SEC30_VALUE == lNumber)
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

// 	CString strSelCode = pSelChart->m_strItemCode;
// 	if(ChangeMarket(((CStdDialog*)GetParent())->GetMarketTypeFromCode(strSelCode)))
// 		InitBaseInfo(NULL, FALSE);
	CString strSelCode = pSelChart->m_strItemCode;
	int nType;
	int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(strSelCode, nType);
	CWnd* pTrmChartView = ((CStdDialog*)GetParent())->GetParent();
	pTrmChartView->SendMessage(WM_USER+101, nMarketType, (LPARAM)(LPCSTR)strSelCode);
	ChangeMarket(nMarketType, FALSE, FALSE);

//@��������
//	CString strType = m_pDataManager->GetCodeInfo("", 2, strSelCode);
//	if(strType == "77" || strType == "78" || strType == "79" || strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")	//�ؿ�
//	{
//		BOOL bRet = ChangeMarket(FOREIGN_CHART);
//		if(bRet)
//			InitBaseInfo(NULL, FALSE);
//	}
////@��������
////	else if(strType == "68") //ELW
////	{
////		BOOL bRet = ChangeMarket(ELW_CHART);
////		if(bRet)
////			InitBaseInfo(NULL, FALSE);
////	}
////@��������
//	else/* if(m_nMarketType != FOREIGN_CHART)*/
//	{
//		if(nLength==FUTOPT_LENGTH)
//		{
//			BOOL bRet = ChangeMarket(FUTOPT_CHART);
//			if(bRet)
//				InitBaseInfo(NULL, FALSE);
//		}
//		else if(nLength==UPJONG_LENGTH)
//		{
////@��������
////			CString sTemp = strSelCode.Left(1);
////@��������
//			BOOL bRet;
//			//if(sTemp == 9)		// MS30
//			//	bRet = ChangeMarket(3, TRUE, FALSE);
//			//else
//			bRet = ChangeMarket(UPJONG_CHART);
//			if(bRet)
//				InitBaseInfo(NULL, FALSE);
//		}
//		else if(nLength==CODE_STOCK_LENGTH)
//		{
//			BOOL bRet = ChangeMarket(STOCK_CHART);
//			if(bRet)
//				InitBaseInfo(NULL, FALSE);
//		}	
//	}
//@��������

	m_strOldCode = GetCurrentCode();
	SetCurrentCode(strSelCode);	
	SetCurCodeNameToControl(strSelCode);

	if(m_nMarketType != CMasterTypeInfo::FOREIGN_CHART)
	{
		STINPUTCODEDATA2 stInputData;
		stInputData.m_strCode = GetCurrentCode();
		stInputData.m_bDontPutInShared = FALSE;
		stInputData.m_bRequestWithoutHesitation = FALSE;
// --> [Edit]  ������ 2008/11/06	( ����+��ǥ ���� ���ѽ� �Ʒ� �޽������� ó���ϴµ� ���⼭ �������� �����Ǳ⶧���� �÷��׸� ������ ���´�... ��.�� )
		m_bDeleteChart = TRUE;
		SendMessage(UMSG_INPUTCODETOINPUTBAR, (WPARAM)&stInputData, 1);
		m_bDeleteChart = FALSE;
// <-- [Edit]  ������ 2008/11/06

		m_bSelectAction = TRUE;
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		CString strCodeInputCode = GetTypedItemCode();
		if(GetCurrentCode().GetLength() && strCodeInputCode != GetCurrentCode())
		{
			SetShareCodeChanged(TRUE);
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(GetCurrentCode()));
			SetShareCodeChanged(FALSE);

			if( stInputData.m_strCode.GetAt( 0) == '|')
			{
				if( stInputData.m_strCode.GetAt( 1) == 'S')	m_cmbOrderType.SetCurSel( 0);
				else										m_cmbOrderType.SetCurSel( 1);
			}
		}
//@��������
//		if (GetCurrentCode().GetLength() == UPJONG_LENGTH && m_cbMarket.GetCurSel() == 1)
//		{
//			if(strSelCode.GetAt(0) == '2')
//				InitUpjongCodeCombo(100); 
//			else
//				InitUpjongCodeCombo(101);
//			m_ctrlCodeInput.SetDataText(strSelCode.Mid(1));
//			m_ctrlCodeInput.SetDataText(GetCurrentCode());
//		}
//		else
//		m_ctrlCodeInput.SetDataText(GetCurrentCode());
//@��������

		SetShareCodeChanged(FALSE);
		m_bSelectAction = FALSE;
	}
	else
	{
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		CString strCode = GetTypedItemCode();
		strCode.TrimLeft(); strCode.TrimRight();
		if(strCode.GetLength() == 0)
		{
			m_bSelectAction = TRUE;
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			CString strCodeInputCode = GetTypedItemCode();
			if(GetCurrentCode().GetLength() && strCodeInputCode != GetCurrentCode())
			{
				SetShareCodeChanged(TRUE);
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				if (m_pCodeInput)	m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(GetCurrentCode()));
				SetShareCodeChanged(FALSE);
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
	UpdateData(FALSE);
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
	if(m_bUseReplayChartMode && m_nCurrentMode == REPLAY_ITEM)
	{		
		if(m_ExChangeBeforeInfo.bShowExtToolBar)
		{
			::WritePrivateProfileString("Frame","ShowExtInputBarDlg","1",m_strCfgFile);
		}
	}
	//=================================================================================

	SaveValue();
}


// --> [Edit]  �̵��� 2008/09/11
// 
CString CInputBarDlg::GetOrderType()
{
	CString strReturn;
	strReturn.Format("%d", m_cmbOrderType.GetCurSel());
	return strReturn;
}


void CInputBarDlg::OnSelchangeCmbOrder()
{
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	CString strItemCode = GetTypedItemCode();
	if( !strItemCode.IsEmpty())
	{
		CChartItem* pSelectedChartItem = ( ( CChartMng *)m_pChartMng)->GetpChartItemSelected();
		if( pSelectedChartItem)
			pSelectedChartItem->SetCodeForQuery( strItemCode, QUERY_STRJONGMOK, TRUE);
	}

	SetType();
}

void CInputBarDlg::OnCheckOrderLink()
{
	UpdateData();

	//20080913 �̹��� >>
	//if(m_pChartMng->GetSafeHwnd())
	//	m_pChartMng->SendMessage(UMSG_SETORDERLINK,(WPARAM)m_checkOrderLink);	
	if(m_pChartMng->GetSafeHwnd())
	{
		if(m_checkOrderLink == TRUE)
			AfxMessageBox("�ֹ��������� íƮ�������� ���콺 ����� ���ѵ˴ϴ�.");

		m_pChartMng->SendMessage(UMSG_SETORDERLINK,(WPARAM)m_checkOrderLink);	
	}
	//20080913 �̹��� <<
}
// <-- [Edit]  �̵��� 2008/09/11

// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
CString CInputBarDlg::GetTypedItemCode( void)
{
	return GetTypedItemCode(m_pCodeInput->GetCode());
}
CString CInputBarDlg::GetTypedItemCode( const char *p_szItemCode, int p_nRequestPairTickState)
{
	CString strItemCode( p_szItemCode);
	if( strItemCode.IsEmpty()) return strItemCode;
	if( strItemCode.GetAt( 0) == '|') return strItemCode;
	switch( p_nRequestPairTickState)
	{
		case E_PRTS_ROTATE_START:
					strItemCode = "|B_" + strItemCode;
					break;
		case E_PRTS_ROTATE_PAIR:
					strItemCode = "|S_" + strItemCode;
					break;
		default:
					if( 0 == m_cmbOrderType.GetCurSel())	strItemCode = "|S_" + strItemCode;
					else									strItemCode = "|B_" + strItemCode;
					break;
	}
	return strItemCode;
}

// --> [Edit]  ������ 2008/10/14
void CInputBarDlg::OnChkCheClick() 
{
// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	UpdateData(TRUE);
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
//		UpdateData(TRUE);
		m_chkChe.SetCheck(!m_chkChe.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

	short nType = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetCheHogaChange();
	if ( nType != 0 )
	{
		BOOL	bFind = ((CChartMng*)m_pChartMng)->CheHoga_Save(0);
//		((CChartMng*)m_pChartMng)->CheHoga_Save(0);

		if ( !bFind )
		{
			if(m_chOldType == TICK_DATA_CHART)
			{
				m_chType	= TICK_DATA_CHART;
				m_chkTick.SetCheck(TRUE);
				OnChktick();
			}
			else if(m_chOldType == SEC_DATA_CHART)
			{
				m_chType = SEC_DATA_CHART;
				m_chkSec.SetCheck(TRUE);
				OnChksec();
			}
			else if(m_chOldType == MIN_DATA_CHART)
			{
				m_chType = MIN_DATA_CHART;
				m_chkMinute.SetCheck(TRUE);
				OnChkminute();
			}
			else if(m_chOldType == DAY_DATA_CHART)
			{
				m_chType = DAY_DATA_CHART;
				m_chkDay.SetCheck(TRUE);
				OnChkday();
			}
			else if(m_chOldType == WEEK_DATA_CHART)
			{
				m_chType = WEEK_DATA_CHART;
				m_chkWeek.SetCheck(TRUE);
				OnChkweek();
			}
			else if(m_chOldType == MONTH_DATA_CHART)
			{
				m_chType = MONTH_DATA_CHART;
				m_chkMonth.SetCheck(TRUE);
				OnChkmonth();
			}
		}

		this->SendMessage(UMSG_CHE_HOGA_CHANGE, 0);
	}
	m_chkChe.SetCheck(TRUE);
	m_chkHoga.SetCheck(FALSE);


	m_chkDay.EnableWindow(1);
//	m_chkHour.EnableWindow(1);
	m_chkWeek.EnableWindow(1);
	m_chkMonth.EnableWindow(1);
	m_chkMinute.EnableWindow(1);
	m_chkSec.EnableWindow(1);
	m_chkTick.EnableWindow(1);

	if(m_chOldType == MIN_DATA_CHART)
	{
		m_chkFirst.EnableWindow(1);
		m_chkSecond.EnableWindow(1);
		m_chkThird.EnableWindow(1);
		m_chkFourth.EnableWindow(1);
	}

	m_cmbChartViewType.EnableWindow(TRUE);
	m_btSaveChart.EnableWindow(TRUE);

	m_chOldType = ' ';


}

void CInputBarDlg::OnChkHogaClick() 
{
// --> [Edit]  ������ 2008/11/20	( �ӽ÷� ������Ƽ�� �˾ƿ������� �߰��� �Լ� )
/*
	CStringList	strlistSetAll;
	CString strTemp = "";
//	LPCTSTR	pProperty = NULL;
	CChartItem *pChartItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
	pChartItem->GetAllProperties2((long)&strlistSetAll);

#ifdef _WRITELOG_SISE
	((CStdDialog*)GetParent())->WriteToStringLog("\n SetAllProperties::Start \n", "", 0);
#endif
	for(POSITION pos = strlistSetAll.GetHeadPosition(); pos != NULL;)
	{
		strTemp = "";
		strTemp = strlistSetAll.GetNext(pos);
#ifdef _WRITELOG_SISE
	((CStdDialog*)GetParent())->WriteToStringLog("", strTemp, strTemp.GetLength());
#endif
	}

#ifdef _WRITELOG_SISE
	((CStdDialog*)GetParent())->WriteToStringLog("\n SetAllProperties::End \n", "", 0);
#endif
//	strTemp = pProperty;

	m_chkChe.SetCheck(TRUE);
	m_chkHoga.SetCheck(FALSE);

	return;
*/
// <-- [Edit]  ������ 2008/11/20

// --> [Edit]  ������ 2008/11/07	( ��ȸ�߿��� ��ư�� Ŭ���ص� ��ȸ���� �ʵ��� �Ѵ�. )
	UpdateData(TRUE);
	if ( ((CChartMng *)m_pChartMng)->IsInLoop() || !((CStdDialog*)GetParent())->GetStatusEnable() )
	{
//		UpdateData(TRUE);
		m_chkHoga.SetCheck(!m_chkHoga.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/07

// --> [Edit]  ������ 2008/11/28	( ƽ��Ʈ�ϰ�� ���ӿ����� �����Ҽ� ������ �Ѵ�. )
	int nFind = m_strCode.Find('.');
	if ( nFind != -1 )
	{
		MessageBox("������ ������ ȣ��ƽ�� ��� ���ӿ��� �ڵ�� ����� �� �����ϴ�.");
		UpdateData(TRUE);
		m_chkHoga.SetCheck(!m_chkHoga.GetCheck());
		UpdateData(FALSE);
		return;
	}
// <-- [Edit]  ������ 2008/11/28

	short nType = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetCheHogaChange();
	if ( nType == 1 )
	{
		m_chkChe.SetCheck(FALSE);
		m_chkHoga.SetCheck(TRUE);

		m_chkDay.EnableWindow(0);
//		m_chkHour.EnableWindow(0);
		m_chkWeek.EnableWindow(0);
		m_chkMonth.EnableWindow(0);
		m_chkMinute.EnableWindow(0);
		m_chkSec.EnableWindow(0);
		m_chkTick.EnableWindow(1);
		m_chkFirst.EnableWindow(0);
		m_chkSecond.EnableWindow(0);
		m_chkThird.EnableWindow(0);
		m_chkFourth.EnableWindow(0);

		//	ȣ�� ��ư�ϰ�� ���񺯰� �޺��� ������ ���´�. 
		m_cmbChartViewType.SetCurSel(CHANGE_ITEM);
		m_cmbChartViewType.EnableWindow(FALSE);
		m_btSaveChart.EnableWindow(FALSE);

		return;
	}

	long nCol = ((CChartMng*)m_pChartMng)->GetCols();
	long nRow = ((CChartMng*)m_pChartMng)->GetRows();
	
	if ( nCol == 1 && nRow == 1 )
	{

		CChartItem *pChartItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
		int nChartCnt = pChartItem->GetChartItemCount();
		if ( nChartCnt > 1 )
		{
			AfxMessageBox("�� ������ ��쿡�� ȣ�����⸦ �Ҽ� �ֽ��ϴ�.");
			m_chkChe.SetCheck(TRUE);
			m_chkHoga.SetCheck(FALSE);

			m_chkDay.EnableWindow(1);
	//		m_chkHour.EnableWindow(1);
			m_chkWeek.EnableWindow(1);
			m_chkMonth.EnableWindow(1);
			m_chkMinute.EnableWindow(1);
			m_chkSec.EnableWindow(1);
			m_chkTick.EnableWindow(1);
			m_chkFirst.EnableWindow(1);
			m_chkSecond.EnableWindow(1);
			m_chkThird.EnableWindow(1);
			m_chkFourth.EnableWindow(1);

			m_cmbChartViewType.EnableWindow(TRUE);
			m_btSaveChart.EnableWindow(TRUE);
		}
		else
		{
			BOOL	bFind = ((CChartMng*)m_pChartMng)->CheHoga_Save(1);

// --> [Edit]  ������ 2008/12/01
//			//	��Ʈ�� ChartItemData�� ������쿡 ������ �缱���ϰ� �Ѵ�... ���⼭ ��ȸ�� �Ǹ� �̻��ϰ� ����... ��.��
//			if ( !((CChartMng*)m_pChartMng)->GetpChartItemSelected()->FindChartItemDataForQuery() ) 
//			{
//				MessageBox("������ �ٽ� �������ֽñ� �ٶ��ϴ�.");
//				UpdateData(TRUE);
//				m_chkHoga.SetCheck(!m_chkHoga.GetCheck());
//				UpdateData(FALSE);
//				return;
//			}
// <-- [Edit]  ������ 2008/12/01

			m_chkChe.SetCheck(FALSE);
			m_chkHoga.SetCheck(TRUE);

			m_chkDay.EnableWindow(0);
	//		m_chkHour.EnableWindow(0);
			m_chkWeek.EnableWindow(0);
			m_chkMonth.EnableWindow(0);
			m_chkMinute.EnableWindow(0);
			m_chkSec.EnableWindow(0);
			m_chkTick.EnableWindow(1);
			m_chkFirst.EnableWindow(0);
			m_chkSecond.EnableWindow(0);
			m_chkThird.EnableWindow(0);
			m_chkFourth.EnableWindow(0);

			m_chOldType = m_chType;
			
	//		//	ü��� �ٲ������ ��Ʈ ���� ����
	//		((CStdDialog*)GetParent())->SaveChart();

	//		((CChartMng*)m_pChartMng)->CheHoga_Save(1);

			if ( !bFind )
				OnChktick();

			this->SendMessage(UMSG_CHE_HOGA_CHANGE, 1);

			//	ȣ�� ��ư�ϰ�� ���񺯰� �޺��� ������ ���´�. 
			m_cmbChartViewType.SetCurSel(CHANGE_ITEM);
			m_cmbChartViewType.EnableWindow(FALSE);
			m_btSaveChart.EnableWindow(FALSE);
		}
		
	}
	else
	{
// --> [Edit]  ������ 2008/11/04
//		AfxMessageBox("1 X 1 �� ��쿡�� ȣ�����⸦ �Ҽ� �ֽ��ϴ�.");
		MessageBox("1 X 1 �� ��쿡�� ȣ�����⸦ �Ҽ� �ֽ��ϴ�.");
// <-- [Edit]  ������ 2008/11/04
		m_chkChe.SetCheck(TRUE);
		m_chkHoga.SetCheck(FALSE);

		m_chkDay.EnableWindow(1);
//		m_chkHour.EnableWindow(1);
		m_chkWeek.EnableWindow(1);
		m_chkMonth.EnableWindow(1);
		m_chkMinute.EnableWindow(1);
		m_chkSec.EnableWindow(1);
		m_chkTick.EnableWindow(1);
		m_chkFirst.EnableWindow(1);
		m_chkSecond.EnableWindow(1);
		m_chkThird.EnableWindow(1);
		m_chkFourth.EnableWindow(1);

		m_cmbChartViewType.EnableWindow(TRUE);
		m_btSaveChart.EnableWindow(TRUE);
	}
}
// <-- [Edit]  ������ 2008/10/14


int CInputBarDlg::GetCheHogaType() 
{
	UpdateData(TRUE);
	if ( m_chkChe.GetCheck() )
		return 0;
	else if ( m_chkHoga.GetCheck() )
		return 1;
	return 0;
}
// <-- [Edit]  ������ 2008/10/14

//>> �Ⱓ �ʱ�ȭ�� �ֱ� �ʱ�ȭ ó�� - vntsorl(20090217)
void CInputBarDlg::ResetZoomValue(long lZoomValue)
{
	for(int n = 0;n<8;n++)//ƽ �� �� �� �� �� 10�� 30�� 
	{
		m_lCntInView[n] = lZoomValue;
	}
}
//<< �Ⱓ �ʱ�ȭ�� �ֱ� �ʱ�ȭ ó�� - vntsorl(20090217)

void CInputBarDlg::OnCodeCtrlSelEndOk() 
{
	// TODO: Add your control notification handler code here
	//>>20100316_JS.Kim		��Ʈ ������ ����Ǿ� �����ڵ尡 ����� ���� ó�� ���Ѵ�.
	if(m_bSelectAction)		return;
	//<<
	//>>20100525	N * N �� �����ߴٰ� �ε��ϴ� ��� �߰��� ���� ù��° ��Ʈ�� �ڵ�� TR ��û�� �ѹ� �� �ϴ� ��� �߻��Ͽ� ����
	if( m_pChartMng && ( ( CChartMng *)m_pChartMng)->m_bIsInLoop )		return;
	//<<

	if( m_pCodeInput && ::IsWindow(m_pCodeInput->m_hWnd) )
	{
		CString strCode = m_pCodeInput->GetCode();
		((CStdDialog*)GetParent())->ChangeCode(strCode);
	}
}
