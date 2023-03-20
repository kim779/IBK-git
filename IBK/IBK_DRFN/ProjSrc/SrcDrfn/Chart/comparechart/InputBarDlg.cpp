// InputBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputBarDlg.h"
#include "StdDialog.h"
#include "./define/ChartMsgDef.h"
//#include "SetUnitDlg.h"
#include "CodeDataFromInputBar.h"
//#include "./include/LocalUtil.h"
#include "./define/MessageDefine.h"
#include "../../inc/IMasterDataMng.h"
#include "./ChartMng.h"

#include <math.h>
#include ".\inputbardlg.h"
//#include "../../../../Runtime/CodeMaster/MasterDefine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern IMainDllObj	* g_pMainDll;
const UINT RMSG_CREATEOPENSHAREDATA	= ::RegisterWindowMessage(_T("RMSG_CREATEOPENSHAREDATA"));



/////////////////////////////////////////////////////////////////////////////
// CInputBarDlg dialog
#define WM_POPUPLIST_SELITEM WM_USER + 77
#define WMU_GET_LAST_KEY			WM_USER + 2237	// AupAgent���� MainFrame�� ������ ��ȸ�� Key�� ��û�Ѵ�.


#define IDTIMER_SHARE_CODE_WAIT 10
#define IDTIMER_REQUESTTR		11
#define IDTIMER_USERSTART		12
#define IDTIMER_INITATER		13
#define IDTIMER_REQSTEP_WAIT    14

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
	//}}AFX_DATA_INIT
//	m_crBk = CLR_INPUTBAR_BKGRND;
	//m_pctrlCode = NULL;
//	m_dlgPopupListForHistory = NULL;
	//m_pctrlCode = NULL;
	m_pnLeftBarType = NULL;
	m_pChartMng = NULL;
	m_bSettingInitData = FALSE;
	m_chType = DAY_DATA_CHART;
	m_nUnitPos = -1;
	m_lBaseDesk = NULL;
//	m_bIsMiniBar = FALSE;
	m_nPosCbMarketOld = 0;
//	::ZeroMemory(m_lValueOfButton,sizeof(m_lValueOfButton));			// ��
//	::ZeroMemory(m_lValueOfMinButton,sizeof(m_lValueOfMinButton));		// ��
//	::ZeroMemory(m_lValueOfTickButton,sizeof(m_lValueOfTickButton));	// ƽ
//	::ZeroMemory(m_lValueOfCombo,sizeof(m_lValueOfCombo));
//
//	m_lValueOfHourButton[0] = 1;
//	m_lValueOfHourButton[1] = 2;
//	m_lValueOfHourButton[2] = 3;
//
//	for(int n = 0;n<8;n++)//ƽ �� �� �� �� �� 10�� 30�� 
//	{
//		m_lCntInView[n] = 120;
//	}

	m_nCntInViewHanHwa = 120;

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

	m_nCurrentMode = NEW_ITEM;

//@solomon	m_bCanRequestHogaTR = TRUE;

	m_pIL_W20H20 = NULL;
	m_pIL_W31H20 = NULL;
	m_pIL_W50H20 = NULL;
	m_pIL_W17H20 = NULL;

	m_bUseUserInputPeriod = FALSE;

	m_bChkPeriod = FALSE;
	m_nFixZoomValue = 120;
	m_nUpjongType = 100;
	m_nUnitValue = -1;

	m_bChkSaveJipyoOneFile = TRUE;
	m_bChkLoadSaveCode = FALSE;

	m_bSelectAction = FALSE;	// ��Ʈ ������ ����Ǿ� �����ڵ尡 ����Ǵ� ��� : �������� - ojtaso (20070323)

	m_bReceiveDataSetCode = FALSE;	// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)

//@��������
//	//{{2007.07.23 by LYH
//	m_strRecentCode = "";
//@��������
//@�����߰�
	m_nCurrentMode = CHANGE_ITEM;
	m_bIsShareCodeChanged = FALSE;
//	m_rectCmb = CRect(0,0,0,0);
//@�����߰�

	m_cChartShape = DIVIDED_COMPARE_CHART;
	m_pSelGroupData	= NULL;

	m_pSubItem = NULL;
}


void CInputBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBarDlg)	
	//DDX_Control(pDX, IDC_BTETCMENU, m_btEtcMenu);
//	DDX_Control(pDX, IDC_BTSAVECHART, m_btSaveChart);
	//DDX_Control(pDX, IDC_CMB_BONG, m_cmbBong);
	DDX_Control(pDX, IDC_BTN_RETRY, m_btnRetry);
	DDX_Control(pDX, IDC_CBMARKET, m_cbMarket);
	DDX_Control(pDX, IDC_LINETYPE, m_cbLineType);
	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbChartViewType);
	DDX_Control(pDX, IDC_CMB_CHARTTYPE, m_cmbChartType);
	DDX_Control(pDX, IDC_CHKDAY, m_chkDay);
	DDX_Control(pDX, IDC_CHKWEEK, m_chkWeek);
	DDX_Control(pDX, IDC_CHKMONTH, m_chkMonth);
	//DDX_Control(pDX, IDC_CHKHOUR, m_chkHour);
	DDX_Control(pDX, IDC_CHKMINUTE, m_chkMinute);
	DDX_Control(pDX, IDC_CHKSEC, m_chkSec);
	DDX_Control(pDX, IDC_CHKTICK, m_chkTick);
	DDX_Control(pDX, IDC_EDITPERIOD, m_editPeriod);
	DDX_Control(pDX, IDC_SPINPERIOD, m_spinPeriod);
	DDX_Control(pDX, IDC_EDITCNTOFDATALL, m_editCntOfDatAll);
	DDX_Control(pDX, IDC_STTEXT, m_stText);
	DDX_Check(pDX, IDC_CHK_REVISED, m_bChkRevised);
//	DDX_Control(pDX, IDC_CHKEXTBAR1, m_chkExtBar1);
//	DDX_Control(pDX, IDC_BTINC, m_btInc);
	DDX_Control(pDX, IDC_STATICDIV, m_stDiv);
	DDX_Control(pDX, IDC_SPINZOOMVALUE, m_spZoomValue);
	DDX_Control(pDX, IDC_BTREQUESTDATMORE, m_btRequestMore);
	DDX_Control(pDX, IDC_EDITZOOMVALUE, m_editZoomValue);
	DDX_Control(pDX, IDC_CHKLEFT, m_chkLeft);
	DDX_Control(pDX, IDC_CHK_DIV_CHART, m_chkShowDivChart);
	DDX_Control(pDX, IDC_CHK_OVRLPPD_CHART, m_chkShowOverLappedChart);
	DDX_Control(pDX, IDC_CHK_NUJUK_OVRLPPD_CHART, m_chkShowNujukOverLappedChart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBarDlg, CRscDlg)
	//{{AFX_MSG_MAP(CInputBarDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHKLEFT, OnBtleft)
	ON_BN_CLICKED(IDC_CHKMINUTE, OnChkminute)
	ON_BN_CLICKED(IDC_CHKDAY, OnChkday)
	ON_BN_CLICKED(IDC_CHKWEEK, OnChkweek)
	ON_BN_CLICKED(IDC_CHKMONTH, OnChkmonth)
	ON_BN_CLICKED(IDC_CHKSEC, OnChksec)
	ON_BN_CLICKED(IDC_CHKTICK, OnChktick)
	ON_BN_CLICKED(IDC_CHK_DIV_CHART, OnChkDivChart)
	ON_BN_CLICKED(IDC_CHK_OVRLPPD_CHART, OnChkOvrlppdChart)
	ON_BN_CLICKED(IDC_CHK_NUJUK_OVRLPPD_CHART, OnChkNujukOvrlppdChart)
	ON_BN_CLICKED(IDC_BTN_RETRY, OnBtnRetry)
	ON_BN_CLICKED(IDC_BTREQUESTDATMORE, OnBtrequestdatmore)
	ON_BN_CLICKED(IDC_CHK_QUERYOP, OnChkQueryOp)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTINC, OnBtinc)
//	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONUP()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPERIOD, OnDeltaposSpinPeriod)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINZOOMVALUE, OnDeltaposSpinzoomvalue)
//	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DATECNT, OnDeltaposSpinDatecnt)
	ON_BN_CLICKED(IDC_CHK_REVISED, OnChkRevised)
	ON_CBN_SELENDOK(IDC_CMB_CHARTTYPE, OnSelendokCmbCharttype)
	ON_CBN_SELCHANGE(IDC_CMB_TYPE, OnSelchangeCmbType)
	ON_EN_SETFOCUS(IDC_EDITPERIOD, OnSetfocusEditperiod)
	ON_EN_CHANGE(IDC_EDITPERIOD, OnChangeEditperiod)
//	ON_BN_CLICKED(IDC_CHKFIRST, OnChkfirst)
//	ON_BN_CLICKED(IDC_CHKSECOND, OnChksecond)
//	ON_BN_CLICKED(IDC_CHKTHIRD, OnChkthird)
//	ON_BN_CLICKED(IDC_CHKFOURTH, OnChkfourth)	
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_LINETYPE, OnSelchangeCblinetype)
	ON_CBN_SELCHANGE(IDC_CBMARKET, OnSelchangeCbmarket)
//	ON_BN_CLICKED(IDC_BTCOMPARECHART, OnBtcomparechart)
	ON_CBN_SELCHANGE(IDC_CTRL_CODEINPUT, OnSelchangeCodeInput)
//	ON_BN_CLICKED(IDC_BTSAVECHART, OnBtSaveChart)
//	ON_BN_CLICKED(IDC_CHKHOUR, OnChkhour)
//	ON_BN_CLICKED(IDC_BTETCMENU, OnBtetcmenu)
//	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POPUPLIST_SELITEM, OnSelBasedlgPopuplistctrl)
	ON_REGISTERED_MESSAGE( RMSG_FROMCODELIST,	OnFromCodeList)
	ON_EN_SETFOCUS(IDC_EDIT_FOCUS, OnEnSetfocusEditFocus)
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
		//if(pChild->IsKindOf(RUNTIME_CLASS(CDialog)) || pChild->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		//	ListupControlHandle(pChild);
		//else if(pChild->GetStyle() & WS_TABSTOP)
		//	m_arChildHandle.Add(pChild->GetSafeHwnd());
		{
			if(pChild->GetDlgCtrlID() == IDC_CBMARKET || pChild->GetDlgCtrlID() == IDC_CMB_TYPE)
			{
				pChild->ShowWindow(SW_HIDE);
			}
			else if(pChild->GetDlgCtrlID() != IDC_CHKLEFT)
			{
				pChild->GetWindowRect(rectComp);
				ScreenToClient(rectComp);
				pChild->SetWindowPos(NULL, rectComp.left - 118, rectComp.top, rectComp.Width(), 20, SWP_NOZORDER);
				//if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
				//	GetDlgItem(IDC_CHK_REVISED)->ShowWindow(SW_SHOW);
			}
		}

		pChild = pChild->GetNextWindow();
	}

	char szTemp[10] = {0,};
	memset(szTemp, 0x00, 10);
	CString strConfig = ((CStdDialog*)GetParent())->m_strConfig;
//@��������	
//	GetDlgItem(IDC_BTETCMENU)->ShowWindow(SW_SHOW);
//	::GetPrivateProfileString("InputBar","SaveJipyoOneFile","1",szTemp,sizeof(szTemp),strConfig);
//	if(szTemp[0]=='1')
//	{
//		m_bChkSaveJipyoOneFile = TRUE;
//	}
//	else
//	{
//		m_bChkSaveJipyoOneFile = FALSE;
//	}
//@��������	
}

BOOL CInputBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();	

	m_pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	m_cmbChartViewType.SetCurSel(2);
//	if(m_cbMarket)
//	{
//		if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
//			m_cbMarket.SetCurSel(m_nMarketType);
//		else
//			m_cbMarket.SetCurSel(0);
//	}
	m_cbMarket.SetCurSel(0);
	m_cbLineType.SetCurSel(0);

	Init();

	//SetTimer(IDTIMER_SHARE_CODE_WAIT, 2000, NULL);
#ifdef _DEBUG
	GetDlgItem(IDC_CBMARKET)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CMB_TYPE)->ShowWindow(SW_SHOW);
#endif

	SetShareInputData();

	//�������Ʈ�϶�
	// ����� �е����Ͱ� �ȵǹǷ� �Ʒ� ���� ���´�.
//-->
//	if(m_nScreenType==1)
//	{
//		GetRequestList();
//		m_pShareInputData->m_bCanDel = FALSE;
//		SendNextTR();
//	}
//<--
	//SetMenuStyle();
	if(m_nScreenType==CStdDialog::Screen_fluctuation)	//�������Ʈ�϶�
	{
		SetTimer(IDTIMER_INITATER, 10, NULL);
	}
	#ifdef _DEBUG		
		GetDlgItem(IDC_BTINC)->ShowWindow(SW_SHOW);
	#endif


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputBarDlg::Init()
{
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();

	m_strImagePath = ((CStdDialog*)GetParent())->m_szImageDir;
	m_strCfgFile = ((CStdDialog*)GetParent())->m_strConfig;

	CString strImageSelect;
	CString strImageUp;
	CString strImageDown;
	CString strImageOver;
	CString strImageDis;

//	m_brBkColor.CreateSolidBrush(m_crBk);

	m_pIL_W20H20 = &((pSTDialog)->m_ImgList_W20H20);
	if(m_pIL_W20H20 == NULL) return;

	m_pIL_W31H20 = &((pSTDialog)->m_ImgList_W31H20);
	if(m_pIL_W31H20 == NULL) return;
	
	m_chkLeft.FP_vSetImage( m_pIL_W20H20, 56, 58);
	m_chkLeft.FP_vSetToolTipText("��ȭ�鿭��");

	m_btRequestMore.FP_vSetImage( m_pIL_W20H20, 40, 42);
	m_btRequestMore.FP_vSetToolTipText("����Ÿ�߰��θ���");

	m_chkMinute.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);		
	m_chkMinute.FP_vSetToolTipText("��");

	m_chkDay.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE);
	m_chkDay.FP_vSetToolTipText("��");

	m_chkWeek.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);
	m_chkWeek.FP_vSetToolTipText("��");

	m_chkMonth.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);	
	m_chkMonth.FP_vSetToolTipText("��");

	m_chkSec.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);		
	m_chkSec.FP_vSetToolTipText("��");

	m_chkTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ƽ", TRUE, 83);	
	m_chkTick.FP_vSetToolTipText("ƽ");

	m_chkShowDivChart				.FP_vSetImage(m_pIL_W31H20, 0, 2, FALSE, "", TRUE);	
	m_chkShowDivChart				.FP_vSetToolTipText("�������� ���Һ� ��Ʈ");

	m_chkShowOverLappedChart		.FP_vSetImage(m_pIL_W31H20, 4, 6, FALSE, "", TRUE);	
	m_chkShowOverLappedChart		.FP_vSetToolTipText("�������� ��ø�� ��Ʈ");	

	m_chkShowNujukOverLappedChart	.FP_vSetImage(m_pIL_W31H20, 8, 10, FALSE, "", TRUE);	
	m_chkShowNujukOverLappedChart	.FP_vSetToolTipText("���� ����� ����Ʈ");	

	//����ȸ
	m_pIL_W50H20 = &((pSTDialog)->m_ImgList_W50H20);
	if(m_pIL_W50H20 == NULL) return;

	m_btnRetry	.FP_vSetImage(m_pIL_W50H20, 0, 2, TRUE, "����ȸ");
	m_btnRetry	.FP_vSetToolTipText("����ȸ");
	
	char szTemp[10] = {0,};
	UpdateData(FALSE);

	LoadValue();

//	UpdateBtNCbValue();
	
	CString sTemp;
	memset(szTemp, 0x00, 10);
	CString strConfig = (pSTDialog)->m_strConfig;

	szTemp[0] = MIN_DATA_CHART;
	m_editPeriod.SetWindowText("1");
	if(m_nScreenType == CStdDialog::Screen_ForeignChart)
	{
		m_chType = DAY_DATA_CHART;
		m_nDay = 1;
		m_chkMinute.EnableWindow(FALSE);
		m_chkDay.SetCheck(TRUE);
	}
	else
	{
		m_chType = MIN_DATA_CHART;
		m_nMin = 1;
		m_chkMinute.EnableWindow(TRUE);
		m_chkMinute.SetCheck(TRUE);
	}

	Enable_PeriodControl(TRUE);

	m_editZoomValue.SetLimitText(4);
	m_editCntOfDatAll.SetLimitText(4);

	//SetExt(0);
	sTemp.Format("%d", m_nFixZoomValue);
	m_editZoomValue.SetWindowText(sTemp);

	m_editCntOfDatAll.SetWindowText("400");

	// m_cChartShape :�������Ʈ�϶��� ���Ͽ��� ������ �о�� �Ѵ�.
	//m_cChartShape = DIVIDED_COMPARE_CHART;
	//if(m_nScreenType==0) //����Ʈ�̸�
	if(m_nScreenType!=CStdDialog::Screen_fluctuation)	//�������Ʈ�϶�
	{
		m_chkShowDivChart.SetCheck(TRUE);
		if(m_nScreenType == CStdDialog::Screen_ForeignChart)
			GetDlgItem(IDC_CHK_QUERYOP)->ShowWindow(SW_HIDE);
	}
	else	//�������Ʈ�̸�
	{
		if(pSTDialog->m_stDlgCompData.m_nTypeUnit>0)
		{
			sTemp.Format("%d", pSTDialog->m_stDlgCompData.m_nTypeUnit);
			m_editPeriod.SetWindowText(sTemp);
		}
		m_bOldShowAll = pSTDialog->m_stDlgCompData.m_bShowAll;

		GetDlgItem(IDC_CHK_QUERYOP)->ShowWindow(SW_SHOW);
		CheckDlgButton(IDC_CHK_QUERYOP, 1);	//������̸� ������ȸ��.
		m_pShareInputData->m_DataInputBar.m_bTodayRequest = TRUE;

		m_nCurrentMode = OVERLAPP_CHART;
		switch(m_pShareInputData->m_nBase)
		{
			case 0:
			default:
				m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTA;
				//@.080723������������. 1->5 �� ������.
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '5';				
				break;
			case 1:
				m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTB;
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '3';
				break;
			case 2:
				m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTC;
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '4';
				break;
		}

		m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
		m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;

		//m_pShareInputData->m_DataInputBar.m_cDataGubun = '1';
		//m_pChartMng->SendMessage(UMSG_RATEVALUE, PERCENT_TICK);

		//if(m_pChartMng)m_pChartMng->SendMessage(UMSG_RATEVALUE,m_pShareInputData->m_DataInputBar.m_nUnitType, TRUE);

		m_chkShowNujukOverLappedChart.SetCheck(TRUE);

		SetMode(OVERLAPP_CHART); // ��ø���

//		if(m_pChartMng)
//			m_pChartMng->SendMessage(UMSG_RATEVALUE, m_pShareInputData->m_DataInputBar.m_nUnitType, TRUE);
#ifdef _DEBUG		
		GetDlgItem(IDC_BTINC)->ShowWindow(SW_SHOW);
#endif
	}

	m_chkDay.Invalidate();
	m_chkWeek.Invalidate();
	m_chkMonth.Invalidate();
	m_chkMinute.Invalidate();
	m_chkSec.Invalidate();
	m_chkTick.Invalidate();
//	m_chkYear.Invalidate();
//@��������	m_btnForSendTR.Invalidate();	
	m_chkShowDivChart.Invalidate();
	m_chkShowOverLappedChart.Invalidate();
	m_chkShowNujukOverLappedChart.Invalidate();

//	UpdateBtNCbValue();	// �ֱ��ư Set
	m_bChkLoadSaveCode = FALSE;
	//SetCompareChartItemMode();
}

// ���� ��ũ�ٿ��� �ʱ�ȭ ��ƾ�� �ҷ��� �� ���ϴ� �κ�.
void CInputBarDlg::Init_Controls()
{
	m_cbLineType.SetCurSel(0);

	m_nMin = 1;
	m_nDay = 1;
	//m_nTick = 1;
	//m_nSec = 1;
	if(m_nScreenType == CStdDialog::Screen_ForeignChart)
	{
		m_chkMinute	.SetCheck(FALSE);
		m_chkDay	.SetCheck(TRUE);
		m_chkMinute	.EnableWindow(FALSE);
	}
	else
	{
		m_chkMinute	.EnableWindow(TRUE);
		m_chkDay	.SetCheck(FALSE);
	}
	m_chkWeek	.SetCheck(FALSE);
	m_chkMonth	.SetCheck(FALSE);
	m_chkMinute	.Invalidate();
	m_chkDay	.Invalidate();
	m_chkWeek	.Invalidate();
	m_chkMonth	.Invalidate();

	m_chkShowDivChart				.SetCheck(TRUE);
	m_chkShowOverLappedChart		.SetCheck(0);
	m_chkShowNujukOverLappedChart	.SetCheck(0);
	m_chkShowDivChart				.Invalidate();
	m_chkShowOverLappedChart		.Invalidate();
	m_chkShowNujukOverLappedChart	.Invalidate();

	m_editPeriod.SetWindowText("1");
	m_editPeriod.SetWindowText("1");
	SetEditZoomValue("120");
	m_editCntOfDatAll.SetWindowText("400");

	_DoChkDivChart();
	
	m_pShareInputData->ReInit();

	if(m_nScreenType == CStdDialog::Screen_ForeignChart)
		m_pShareInputData->m_DataInputBar.m_chType = m_chType;
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

long CInputBarDlg::GetEndDate()
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
//			m_btInc.GetWindowText(strInc);
//			if((pStInputCodeData2->m_strCode.GetLength()==CODE_STOCK_LENGTH&&m_cbMarket.GetCurSel()==0)
//				||(pStInputCodeData2->m_strCode.GetLength()==UPJONG_LENGTH&&m_cbMarket.GetCurSel()==1)
//				||(pStInputCodeData2->m_strCode.GetLength()==FUTOPT_LENGTH&&m_cbMarket.GetCurSel()==2)
//				||strInc=="�߰�")
			{
				//2007.01.24 by LYH �ٸ���Ʈ Ÿ���� �ڵ� ���� ��Ʈ Ÿ�� ����
				int nRealMarketType;
				if(m_nMarketType != ((CStdDialog*)GetParent())->GetMarketTypeFromCode(pStInputCodeData2->m_strCode, nRealMarketType))
				{
					SetCodeDragAndDrop(pStInputCodeData2->m_strCode);
					SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)pStInputCodeData2->m_strCode);
					return 1L;
				}
				
				// CodeInput Ctrl�� SetDataText �Լ��� ȣ��� ���� ���
				if(GetShareCodeChanged())
				{
					SetShareCodeChanged(FALSE);
					break;
				}				
				
				// ���ǿ� �������� ����
				//if(GetCurrentCode() != pStInputCodeData2->m_strCode || pStInputCodeData2->m_bRequestWithoutHesitation)
				if(TRUE)
				{	
					m_strCodeFromOutside = pStInputCodeData2->m_strCode;
					//if(ChangeEditCode(FALSE,bDontPutInShared))
					//{	
						m_strOldCode = GetCurrentCode();
						SetCurrentCode(m_strCodeFromOutside);
						EnableWindow(FALSE);
						
						//{{2007.01.22 by LYH �ֱ� �ϵ����ͷ� 
						// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//@��������
//						if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART || m_nMarketType == COMMODITY_CHART)
						if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
//@��������
						{
							//if(GetType() == WEEK_DATA_CHART || GetType() == MONTH_DATA_CHART)
							//���� ����Ʈ �߰�
							if(GetType() == MONTH_DATA_CHART)
							{
								m_chType = DAY_DATA_CHART;
								//UpdateBtNCbValue();	// �ֱ��ư Set
								SetType(FALSE);				
							}
						}
						//}}

						CCodeDataFromInputBar CodeData;
						CodeData.m_strCode = GetCurrentCode();
						CodeData.m_lEndTime = GetEndDate();
						CodeData.m_chType = GetType();
						CodeData.m_lTypeNum = GetTypeNumber();
						CodeData.m_cChartShape = m_cChartShape;

						if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						{
							m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
//080520-001 CodeInput����							SetShareCodeChanged(TRUE);
//080520-001 CodeInput����							m_ctrlCodeInput.SetDataText(GetCurrentCode());
						}
						else
						{
//080520-001 CodeInput����							if(m_ctrlCodeInput.GetSafeHwnd())
//080520-001 CodeInput����							{
//080520-001 CodeInput����								m_ctrlCodeInput.SetDataText("");
//080520-001 CodeInput����								m_strOldCode = _T("");
//080520-001 CodeInput����								m_strCodeFromOutside = _T("");
//080520-001 CodeInput����								SetCurrentCode(_T(""));
//080520-001 CodeInput����								SetCurCodeNameToControl(_T(""));
//080520-001 CodeInput����							}
						}
						EnableWindow(TRUE);
					//}
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
			m_nCntInViewHanHwa = wParam;

			m_pShareInputData->m_DataInputBar.m_lDispCnt = wParam;
			m_nFixZoomValue = wParam;
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
			//strValue.Format("%d",_pStData->m_lTotalCnt);
			//m_editCntOfDatAll.SetWindowText(strValue);

			if(m_bChkPeriod)
			{
				//strValue.Format("%d",m_nFixZoomValue);
				//SetEditZoomValue(strValue);//m_editZoomValue.SetWindowText(strValue);
			}
			else
			{
				//strValue.Format("%d",_pStData->m_lZoomValue);
				//SetEditZoomValue(strValue);//m_editZoomValue.SetWindowText(strValue);
			}

			//m_nCurSelCol = _pStData->m_nSelCol;
			//m_nCurSelRow = _pStData->m_nSelRow;

			//m_chType = _pStData->m_chType;

			//08.22================================================================
			//���� �ְ� �� �α� �����ֱ� ����
			//m_bChkRevised	= _pStData->m_bRevised;
			//m_bChkLog		= _pStData->m_bLog;

			//UpdateData(FALSE);			
			//07.01.20 by LYH �߰�
			//if(oldChType != _pStData->m_chType)	UpdateBtNCbValue();
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

			if(m_chType==TICK_DATA_CHART||m_chType==MIN_DATA_CHART||m_chType==DAY_DATA_CHART)
			{
				long lNumber = 0;
				//BOOL bUpdateUnitPos = FALSE;
				// �߰� 2006.01.19 by LYH 
				BOOL bUpdateUnitPos = FALSE;

				if(m_chType == TICK_DATA_CHART)	//ƽ(0)
				{
					//lNumber = atol(_pStData->m_strTick);
					//m_nTick = lNumber;
//					for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//					{
//						if(m_lValueOfTickButton[nPos]==lNumber)
//						{
//							m_nUnitPos = nPos;
//							bUpdateUnitPos = TRUE;
//							break;
//						}
//					}
				}
				else if(m_chType == MIN_DATA_CHART)//��(1)
				{					
					//lNumber = atol(_pStData->m_strMin);
					//m_nMin	= lNumber;

					//if(300 == lNumber)
					//{
					//	m_nUnitPos = 0;
					//	bUpdateUnitPos = TRUE;
					//}
					//else
					//{
//						for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//						{
//							if(m_lValueOfMinButton[nPos]==lNumber)
//							{
//								m_nUnitPos = nPos;
//								bUpdateUnitPos = TRUE;
//								break;
//							}
//						}
					//}
				}
				else if(m_chType == DAY_DATA_CHART)//��(1)
				{					
					//lNumber = atol(_pStData->m_strDay);
					//m_nDay	= lNumber;
//					for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//					{
//						if(m_lValueOfButton[nPos]==lNumber)
//						{
//							m_nUnitPos = nPos;
//							bUpdateUnitPos = TRUE;
//							break;
//						}
//					}
//					CString sTemp;
//					sTemp.Format("InputBar : Recv__m_nDay %d", m_nDay);
//					OutputDebugString(sTemp);
				}

				if(!bUpdateUnitPos)
				{
					//m_nUnitPos = -1;
				}
				//m_nUnitPos = -1; 
				//SetType(FALSE);
				if(m_chType==TICK_DATA_CHART)	//ƽ(0)
				{				
					//m_nUnitPos = 0;
					//_pStData->m_strTick.TrimLeft('0');
					//m_editPeriod.SetWindowText(_pStData->m_strTick);
				}
				else if(m_chType==MIN_DATA_CHART)				//��(1)
				{				
					//m_nUnitPos = 1;
					//_pStData->m_strMin.TrimLeft('0');
					//CString strMin = _pStData->m_strMin;
					//if("300" == strMin)  // 30��
					//	strMin = "0";
					//m_editPeriod.SetWindowText(strMin);
				}
				else if(m_chType==DAY_DATA_CHART)
				{				
					//m_nUnitPos = 2;
					//_pStData->m_strDay.TrimLeft('0');
					//m_editPeriod.SetWindowText(_pStData->m_strDay);
				}
				//else
				//	m_editPeriod.SetWindowText("1");
			}
			else if (m_chType == HOUR_DATA_CHART)		// ��
			{
				//long lNumber = 0;
				//CString strTemp;
				//BOOL bUpdateUnitPos = FALSE;

				//lNumber = atol(_pStData->m_strHour);
				//				lNumber = lNumber / 60;
				//m_nHour = lNumber;

//				for(int nPos=0;nPos < 3;nPos++)
//				{
//					if(m_lValueOfHourButton[nPos]==lNumber)
//					{
//						m_nUnitPos = nPos;
//						bUpdateUnitPos = TRUE;
//						break;
//					}
//				}

				//if(!bUpdateUnitPos)
				//{
				//	m_nUnitPos = -1;
				//}
				//SetType(FALSE);
				//_pStData->m_strHour.TrimLeft('0');
				//strTemp.Format("%d",lNumber);
				//m_editPeriod.SetWindowText(strTemp);
			}
			else	////
			{
				//SetType(FALSE);				
				//m_editPeriod.SetWindowText("1");
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
//@solomon					if(m_bCanRequestHogaTR) //ȣ���� ��ȸ�ص� �Ǵ� ���¿����� ��ȸ�� �Ѵ�.
//@solomon						((CStdDialog*)GetParent())->Request_StockHogaData();
					//===============================================================

					return 1l;				
				}

				m_strOldCode = GetCurrentCode();
				SetCurrentCode(_pStData->m_strCode);
				SetCurCodeNameToControl(_pStData->m_strCode);
				
				// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)
				m_bReceiveDataSetCode = TRUE;
				//if(m_nMarketType != CMasterTypeInfo::FOREIGN_CHART)
				{
					if(wParam != 1) //TR�� ���� �����ͷ� ���� ���� ������ ���ϱ� ���� ���� �ڵ� ���� ���� �ʴ´�.
					{
						if (_pStData->m_strCode.GetLength() == UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
						{
							if(_pStData->m_strCode.GetAt(0) == '2')
								InitUpjongCodeCombo(100);
							else
								InitUpjongCodeCombo(101);
//080520-001 CodeInput����	m_ctrlCodeInput.SetDataText(_pStData->m_strCode.Mid(1));
						}
//080520-001 CodeInput����else
//080520-001 CodeInput����	m_ctrlCodeInput.SetDataText(_pStData->m_strCode);
					}
					else 
					{
//080520-001 CodeInput����
//						CString strCode = m_ctrlCodeInput.GetDataText();
//						strCode.TrimLeft(); strCode.TrimRight();
//						if(!strCode.GetLength())
//						{
//							if (_pStData->m_strCode.GetLength() == UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
//							{
//								SetShareCodeChanged(TRUE);
//								m_ctrlCodeInput.SetDataText(_pStData->m_strCode);
//							}
//							else
//							{
//								SetShareCodeChanged(TRUE);
//								m_ctrlCodeInput.SetDataText(_pStData->m_strCode);
//							}
//						}
//<--
					}
				}
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
//			if(m_bCanRequestHogaTR) //ȣ���� ��ȸ�ص� �Ǵ� ���¿����� ��ȸ�� �Ѵ�.
//				((CStdDialog*)GetParent())->Request_StockHogaData();
			//===============================================================
		}
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
//080520-001 CodeInput����					SetShareCodeChanged(TRUE);
//080520-001 CodeInput����					m_ctrlCodeInput.SetDataText(strRetryCode.Mid(1));
				}
				else
				{
//080520-001 CodeInput����					SetShareCodeChanged(TRUE);
//080520-001 CodeInput����					m_ctrlCodeInput.SetDataText(strRetryCode);
				}
			}
		}
		break;
	case UMSG_GETZOOMVALUE:
		{	
			//05.09.28
			//if(wParam<TICK_DATA_CHART || wParam>YEAR_DATA_CHART) return 0L;
//			if(wParam<TICK_DATA_CHART || wParam>SEC30_DATA_CHART) return 0L;
//			if(wParam<TICK_DATA_CHART || wParam>SEC_DATA_CHART) return 0L;//KHD 
//			if(m_bChkPeriod)
				return m_nFixZoomValue;
//			return m_lCntInView[wParam-'0'];			
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
// 	case UMSG_ENABLE_SENDHOGATR:
// 		{
// 			BOOL bCanRequest = (BOOL)wParam;
// 			SetStopRequestHogaTR(bCanRequest);
// 		}
// 		break;
	case UMSG_SETTEXT_CODE_INPUTBAR:
		{
			CString strCode;
			strCode.Format("%s", (LPTSTR)(LPCTSTR)lParam);			
			if (strCode.GetLength()==UPJONG_LENGTH && m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
			{
//080520-001 CodeInput����				SetShareCodeChanged(TRUE);
//080520-001 CodeInput����				m_ctrlCodeInput.SetDataText(strCode.Mid(1));
			}
			else
			{
//080520-001 CodeInput����				SetShareCodeChanged(TRUE);
//080520-001 CodeInput����				m_ctrlCodeInput.SetDataText(strCode);
			}
			//}
		}	
		break;
	case UMSG_END_REQUEST_CHARTDATA:
		{
			SetUseUserInputPeriod(FALSE);
		}
		break;
	case UMSG_EXTINPUT_CHANGEDATE:
		{
			if(wParam == 1)
			{
				m_bChkPeriod = lParam;
				CString strZoomValue;
				m_editZoomValue.GetWindowText(strZoomValue);
				//if(m_bChkPeriod)
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
//080520-001 CodeInput����			m_ctrlCodeInput.SetFocus();
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
	}
	
	if(message == RMSG_EVENT_CODEINPUT)
	{
		LPST_CTRLEVENT stEvent = (LPST_CTRLEVENT)lParam;
		if(strcmp("LengthFull", stEvent->lpszEventName) == 0 || strcmp("SelChanged", stEvent->lpszEventName) == 0)
		{
//080520-001 CodeInput����			SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_ctrlCodeInput.GetDataText());
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
//-->080529 �� ��ƾ �ʿ��Ѱ�?	
//#ifdef _DEBUG
//CString strTEST;
//strTEST.Format("[gm0604]SetType(%d) m_chType = %c, m_nUnitPos= %d...",bSendChartMng,m_chType,m_nUnitPos);
//::OutputDebugString(strTEST);
//#endif
//	EnableWindow(FALSE);
//
////	m_chkMinute	.SetCheck(0);				
////	m_chkDay	.SetCheck(0);
////	m_chkWeek	.SetCheck(0);
////	m_chkMonth	.SetCheck(0);
////	m_chkTick	.SetCheck(0);
////	m_chkSec	.SetCheck(0);
//
//	// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5) -- �� �κ��� ��ȭ���� �ű� �߰��� �׸�
//	CString sKey = _T("");
//	switch(m_chType)
//	{
//		case MIN_DATA_CHART:	
////			m_chkMinute.SetCheck(1);
//			m_editPeriod.EnableWindow(TRUE);
//			Enable_PeriodControl(TRUE);
//			break;
//		case DAY_DATA_CHART:							
////			m_chkDay.SetCheck(1);
//			m_editPeriod.EnableWindow(TRUE);
//			Enable_PeriodControl(TRUE);
//			break;	
//		case WEEK_DATA_CHART:
////			m_chkWeek.SetCheck(1);
//			m_editPeriod.EnableWindow(FALSE);
//			Enable_PeriodControl(FALSE);
//			break;
//		case MONTH_DATA_CHART:
////			m_chkMonth.SetCheck(1);
//			m_editPeriod.EnableWindow(FALSE);
//			Enable_PeriodControl(FALSE);
//			break;
//		case TICK_DATA_CHART:	
////			m_chkTick.SetCheck(1);
//			m_editPeriod.EnableWindow(TRUE);
//			Enable_PeriodControl(TRUE);
//			break;
//		case SEC_DATA_CHART:	
////			m_chkSec.SetCheck(1);
//			m_editPeriod.EnableWindow(TRUE);
//			Enable_PeriodControl(TRUE);
//			break;
//	}
//
////	m_chkDay.Invalidate();
////	m_chkWeek.Invalidate();
////	m_chkMonth.Invalidate();
////	m_chkMinute.Invalidate();
////	m_chkSec.Invalidate();
////	m_chkTick.Invalidate();
//
//	SetCodeInit();
//	if(bSendChartMng)
//	{
//		//if(m_nUnitPos>-1)
//		if(m_chType < WEEK_DATA_CHART || m_chType == SEC_DATA_CHART)
//		{
//			if(m_pChartMng->GetSafeHwnd())
//			{
//				CString strValue;				
//				m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
//				int nValue = atoi(strValue);
//				if(nValue <= 0)
//				{
//					strValue = _T("1");
//					m_editPeriod.SetWindowText(strValue);
//				}
//
//				m_pChartMng->SendMessage(UMSG_SETTYPE,
//										m_chType, // ƽ, ��, ��,....���� 
//										nValue);//����� ���� �Ⱓ
//			}
//		}
//		else if (HOUR_DATA_CHART == m_chType)		// ��
//		{
//			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			{
//				CString strValue;				
//				m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
//				int nValue = atoi(strValue);
//				if(nValue <= 0) 
//				{
//					strValue = _T("1");
//					m_editPeriod.SetWindowText(strValue);
//					nValue = 1;
//				}
//
//				m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, nValue);//����� ���� �Ⱓ
//			}
//		}
//		else // �� / �� / ��
//		{
//			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//				m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType);
//		}		
//	}
//
//	EnableWindow();
//<--
}

//��ȸ:��
void CInputBarDlg::OnChkminute() 
{
	SaveInputPeriod();
	GetDlgItem(IDC_CHK_QUERYOP)->ShowWindow(SW_SHOW);
	((CStdDialog*)GetParent())->EnableQueryOpt(TRUE);

	//�������� �� �����Ͱ� �ƴϾ����� ����ڰ� �Է��� �����Ѵ�.========
//	if(m_chType != MIN_DATA_CHART)	SetUseUserInputPeriod(FALSE);

	m_chType = MIN_DATA_CHART;
	//UpdateBtNCbValue();	// �ֱ��ư Set

	SetShareInputData();
	//SetType();

	SetChTypeBtn(m_chType);
	Test004(m_chType, m_nMin);
	//Test002();
	RequestStepAll();

	SetUseUserInputPeriod(FALSE);
}

//��ȸ:��
void CInputBarDlg::OnChkday() 
{
	SaveInputPeriod();
	GetDlgItem(IDC_CHK_QUERYOP)->ShowWindow(SW_HIDE);

	//�������� �� �����Ͱ� �ƴϾ��ٸ� ����� �Է°� ����========
	//05.09.12
	if(m_chType != DAY_DATA_CHART)
		SetUseUserInputPeriod(FALSE);
	//===========================================================

	m_chType = DAY_DATA_CHART;
//	m_nUnitPos = 2;	

	//UpdateBtNCbValue();	// �ֱ��ư Set

	//================================================================================
	// 2005. 09. 08
	// ����ڰ� �Է��� ������ �����ϵ��� �ش޶� ��ȭ �䱸
	//================================================================================	
	CString sTemp;
	if(!GetUseUserInputPeriod())
	{
//		if(m_pChartMng->GetSafeHwnd())
//			m_nDay = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 2);
		
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
	//SetType();
	SetShareInputData();
	SetChTypeBtn(m_chType);
	Test004(m_chType, m_nDay);
	//Test002();
	RequestStepAll();

	SetUseUserInputPeriod(FALSE);
}

//��ȸ:��
void CInputBarDlg::OnChkweek() 
{
	SaveInputPeriod();
	GetDlgItem(IDC_CHK_QUERYOP)->ShowWindow(SW_HIDE);

	// TODO: Add your control notification handler code here
	m_chType = WEEK_DATA_CHART;
	
	m_nUnitPos = -1;
	//Enable_PeriodControl(FALSE);
	m_editPeriod.SetWindowText("1");

	((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);
	//SetType();
	SetShareInputData();
	SetChTypeBtn(m_chType);
	Test004(m_chType, 1);
	//Test002();
	RequestStepAll();
}

//��ȸ:��
void CInputBarDlg::OnChkmonth() 
{
	SaveInputPeriod();
	GetDlgItem(IDC_CHK_QUERYOP)->ShowWindow(SW_HIDE);

	// TODO: Add your control notification handler code here
	m_chType = MONTH_DATA_CHART;

	m_nUnitPos = -1;
	//Enable_PeriodControl(FALSE);
	m_editPeriod.SetWindowText("1");

	((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);
	//SetType();
	SetShareInputData();
	SetChTypeBtn(m_chType);
	Test004(m_chType, 1);
	//Test002();
	RequestStepAll();
}

//��ȸ:ƽ
void CInputBarDlg::OnChktick() 
{
//-->080529ƽ�� ó���ϰ� ���� �����Ƿ� ���Ƴ���.
//	SaveInputPeriod();
//	GetDlgItem(IDC_CHK_QUERYOP)->ShowWindow(SW_SHOW);
//
//	//������ ���� ������ ���°� ƽ�� �ƴϸ����� �Է°� ����===
//	//05.09.12
//	if(m_chType != TICK_DATA_CHART)
//		SetUseUserInputPeriod(FALSE);
//	//==========================================================
//
//	/* ƽ(0)/��(1)/��(2)/��(3)/��(4)*/	
//	m_chType = TICK_DATA_CHART;	
//	SetShareInputData();
//	//m_nUnitPos = 0;
//
//	//UpdateBtNCbValue();	// �ֱ��ư Set
//
//	//================================================================================
//	// 2005. 09. 08
//	// ����ڰ� �Է��� ������ �����ϵ��� �ش޶� ��ȭ �䱸
//	//================================================================================	
//	CString sTemp;
//	if(!GetUseUserInputPeriod())
//	{
////		if(m_pChartMng->GetSafeHwnd())
////			m_nTick = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 0);	
//
//		
//		sTemp.Format("%d", m_nTick); 
//		m_editPeriod.SetWindowText(sTemp);
//	}
//	else
//	{
//		m_editPeriod.GetWindowText(sTemp);
//		m_nTick = atoi(sTemp);
//	}
//	//---------------------------------------------------	
//	//Enable_PeriodControl(TRUE);
//	//---------------------------------------------------
//
//	((CStdDialog*)GetParent())->EnableQueryOpt(TRUE);
//	SetType();
//
//	SetUseUserInputPeriod(FALSE);
//<--
}

//��ȸ:��
void CInputBarDlg::OnChksec() 
{
//-->080529�ʴ� ó���ϰ� ���� �����Ƿ� ���Ƴ���.
//	SaveInputPeriod();
//	GetDlgItem(IDC_CHK_QUERYOP)->ShowWindow(SW_SHOW);
//
//	//�������� �� �����Ͱ� �ƴϾ����� ����ڰ� �Է��� �����Ѵ�.========
//	//05.09.12
//	if(m_chType != SEC_DATA_CHART)
//		SetUseUserInputPeriod(FALSE);
//	//==========================================================
//
//	m_chType = SEC_DATA_CHART;
//	SetShareInputData();
//
//	//UpdateBtNCbValue();	// �ֱ��ư Set
//
//	//m_nUnitPos = 1;
//
//	CString sTemp;
//	if(!GetUseUserInputPeriod())
//	{
////		if(m_pChartMng->GetSafeHwnd())
////			m_nSec = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 1);	
//
//		sTemp.Format("%d", m_nMin);
//
//		if(m_nSec >= 300)
//			sTemp = "0";
//
//		m_editPeriod.SetWindowText(sTemp);
//	}
//	else
//	{
//		m_editPeriod.GetWindowText(sTemp);
//		m_nSec = atoi(sTemp);
//	}
//	//=======================================================================================
//
//	((CStdDialog*)GetParent())->EnableQueryOpt(TRUE);
//	SetType();
//
//	SetUseUserInputPeriod(FALSE);
//<--
}

//��ȸ:��
//void CInputBarDlg::OnChkyear() 
//{
//	// TODO: Add your control notification handler code here
//	m_chType = YEAR_DATA_CHART;
//	m_nUnitPos = -1;
//	//m_editPeriod.SetWindowText("1");
//	SetType();	
//}

//��ȸ:����

void CInputBarDlg::_DoChkDivChart() 
{
	//	m_cmbChartViewType.SetCurSel(2);
	//m_pChartMng->SendMessage(UMSG_RATEVALUE, WON_TICK);
	m_cChartShape = DIVIDED_COMPARE_CHART;
	m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
	m_pShareInputData->m_DataInputBar.m_nUnitType = WON_TYPE;
	m_pShareInputData->m_DataInputBar.m_cDataGubun = '0';
	
	m_pChartMng->SendMessage(UMSG_RATEVALUE, m_pShareInputData->m_DataInputBar.m_nUnitType, TRUE);
	SetMode(ADD_NEW_ITEM);	// �߰����
	SetCompareChartType();
}

void CInputBarDlg::OnChkDivChart() 
{
//	m_cmbChartViewType.SetCurSel(2);
	//m_pChartMng->SendMessage(UMSG_RATEVALUE, WON_TICK);
	m_cChartShape = DIVIDED_COMPARE_CHART;
	m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
	m_pShareInputData->m_DataInputBar.m_nUnitType = WON_TYPE;
	m_pShareInputData->m_DataInputBar.m_cDataGubun = '0';

	m_pChartMng->SendMessage(UMSG_RATEVALUE, m_pShareInputData->m_DataInputBar.m_nUnitType, TRUE);
	SetMode(ADD_NEW_ITEM);	// �߰����
	SetCompareChartType();
	RequestStep2All();
//	RequestTR();
}

//��ȸ:��ø
void CInputBarDlg::OnChkOvrlppdChart() 
{
//	m_cmbChartViewType.SetCurSel(3);
	//m_pChartMng->SendMessage(UMSG_RATEVALUE, WON_TICK);
	m_cChartShape = OVERLAPP_COMPARE_CHART;
	m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
	m_pShareInputData->m_DataInputBar.m_nUnitType = WON_TYPE;
	m_pShareInputData->m_DataInputBar.m_cDataGubun = '0';

	m_pChartMng->SendMessage(UMSG_RATEVALUE, m_pShareInputData->m_DataInputBar.m_nUnitType, TRUE);
	SetMode(OVERLAPP_CHART); // ��ø���
	SetCompareChartType();
	RequestStep2All();
//	RequestTR();
}

//��ȸ:�����
void CInputBarDlg::OnChkNujukOvrlppdChart() 
{
	if(m_nScreenType==CStdDialog::Screen_fluctuation)	//�������Ʈ�϶�
	{
		// m_nBase���� CStdDialog���� FormLoad()���� ���õȴ�.
		// �� �Ŀ��� ������ ����Ǿ��� �� ȣ��ȴ�.
		switch(m_pShareInputData->m_nBase)
		{
			case 0: OnChkNujukOvrlppdChartA(); break;
			default: OnChkNujukOvrlppdChartA(); break;
			case 1: OnChkNujukOvrlppdChartB(); break;
			case 2: OnChkNujukOvrlppdChartC(); break;
		}
	}
	else
	{
		//@.080730-001 ����Ʈ ������϶� 11
		m_pShareInputData->m_nBase = 11;	//0 -> 11
		m_pShareInputData->m_DataInputBar.m_cDataGubun = '1'; //@solomon �߰� (��������)20091019 by ����ȣ
		OnChkNujukOvrlppdChartA();
	}

}

void CInputBarDlg::OnChkNujukOvrlppdChartA()
{
//	m_cmbChartViewType.SetCurSel(3);
	m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTA;
	m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
	m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;
	if(m_nScreenType==CStdDialog::Screen_fluctuation)
		m_pShareInputData->m_DataInputBar.m_cDataGubun = '5';
	else
		m_pShareInputData->m_DataInputBar.m_cDataGubun = '1';
	//m_pChartMng->SendMessage(UMSG_RATEVALUE, PERCENT_TICK);

	m_pChartMng->SendMessage(UMSG_RATEVALUE,m_pShareInputData->m_DataInputBar.m_nUnitType, TRUE);
	SetMode(OVERLAPP_CHART); // ��ø���
	SetCompareChartType();
	RequestStep2All();
//	RequestTR();
}

//��ȸ:���(���Ͻð�)
void CInputBarDlg::OnChkNujukOvrlppdChartB() 
{
	m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTB;
	m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
	m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;
	m_pShareInputData->m_DataInputBar.m_cDataGubun = '3';
	//m_pChartMng->SendMessage(UMSG_RATEVALUE, PERCENT_TICK);

	m_pChartMng->SendMessage(UMSG_RATEVALUE,m_pShareInputData->m_DataInputBar.m_nUnitType, TRUE);
	SetMode(OVERLAPP_CHART); // ��ø���
	SetCompareChartType();
	RequestStep2All();
}

//��ȸ:���(���簡)
void CInputBarDlg::OnChkNujukOvrlppdChartC() 
{
	m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTC;
	m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
	m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;
	m_pShareInputData->m_DataInputBar.m_cDataGubun = '4';
	//m_pChartMng->SendMessage(UMSG_RATEVALUE, PERCENT_TICK);

	m_pChartMng->SendMessage(UMSG_RATEVALUE,m_pShareInputData->m_DataInputBar.m_nUnitType, TRUE);
	SetMode(OVERLAPP_CHART); // ��ø���
	SetCompareChartType();
	RequestStep2All();
}

//��ȸ:����ȸ
void CInputBarDlg::OnBtnRetry() 
{
	//Test002();
	//SetPeriodType();
	RequestStepAll();
}

void CInputBarDlg::SaveInputPeriod()
{
	CString sTemp;
	m_editPeriod.GetWindowText(sTemp);

	switch(m_chType)
	{
		case MIN_DATA_CHART:	
			m_nMin = atoi(sTemp);
			break;
		case DAY_DATA_CHART:							
			m_nDay = atoi(sTemp);
			break;	
		case WEEK_DATA_CHART:
		case MONTH_DATA_CHART:
			break;
		case TICK_DATA_CHART:	
			m_nTick = atoi(sTemp);
			break;
		case SEC_DATA_CHART:	
			m_nSec = atoi(sTemp);
			break;
	}
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
		m_pShareInputData->m_DataInputBar.m_lDispCnt = lZoomValue;
		m_nFixZoomValue = lZoomValue;

		lZoomValue = atol(strZoomValue);
		lTotValue  = atol(strTotValue);	
		
		if(lZoomValue > lTotValue) return;

		
		SetEditZoomValue(strZoomValue);//m_editZoomValue.SetWindowText(strZoomValue);
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_SETZOOMVALUE,atol(strZoomValue));
	}	
	*pResult = 0;
}

void CInputBarDlg::OnDeltaposSpinPeriod(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString strTime;
	long lTime = 0;
	m_editPeriod.GetWindowText(strTime);
	if(!strTime.IsEmpty())
	{
		SetUseUserInputPeriod(TRUE);

		lTime = atol(strTime);
		if(pNMUpDown->iDelta>0)
		{
			if(lTime<=0) return;
			lTime--;
		}
		else
		{
			lTime++;
		}
		
		if(lTime <= 1)
			lTime = 1;

		strTime.Format("%d",lTime);
		m_pShareInputData->m_DataInputBar.m_lTypeNum = lTime;
		m_editPeriod.SetWindowText(strTime);
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

//	long lDefaultValueOfButton[CNTOFMAXBUTTONS] = { 1, 2, 3, 4};		// ��
//	long lDefaultValueOfMinButton[CNTOFMAXBUTTONS] = {1, 3, 5, 10};		// ��
//	long lDefaultValueOfTickButton[CNTOFMAXBUTTONS] = { 1, 3, 5, 10};		// ƽ
//	long lValueOfCombo[CNTOFMAXCBITEMS] = {1, 2, 3, 4, 5, 10, 15, 20, 30, 60};
//	char szTemp[8];
	CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
//	int nPos=0;
//	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lValueOfButton[nPos] = ::GetPrivateProfileInt("Day Button",szTemp,lDefaultValueOfButton[nPos],strUserDir);
//	}
//	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lValueOfMinButton[nPos] = ::GetPrivateProfileInt("Min Button",szTemp,lDefaultValueOfMinButton[nPos],strUserDir);
//	}
//	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ƽ
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lValueOfTickButton[nPos] = ::GetPrivateProfileInt("Tick Button",szTemp,lDefaultValueOfTickButton[nPos],strUserDir);
//	}
//
//	for(nPos=0;nPos<CNTOFMAXCBITEMS;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lValueOfCombo[nPos] = ::GetPrivateProfileInt("Combo",szTemp,lValueOfCombo[nPos],strUserDir);
//	}
//	for(nPos=0;nPos<5;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lCntInView[nPos] = ::GetPrivateProfileInt("CntInView",szTemp,m_lCntInView[nPos],strUserDir);
//	}	

	// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
//	CString strData;
//	strData.GetBuffer(20);
//	::GetPrivateProfileString(_T("InputBar"), _T("SavedCode"), _T("001;NAS@IXIC"), (LPSTR)(LPCTSTR)strData, 20, strUserDir);

//	int nLineType = ::GetPrivateProfileInt(_T("InputBar"), _T("LineType"), 0, strUserDir);
//	m_cbLineType.SetCurSel(nLineType);

//	strData.ReleaseBuffer();
//	int nIndex = strData.Find(_T(";"));
//	m_strUpjongCode = strData.Left(nIndex);
//	strData.Delete(0, nIndex + 1);
//	m_strForeignCode = strData;
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
	char szTemp[10];
	char szTemp2[20];
	CString strTemp;
	int nPos=0;
//	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
//	{
//		sprintf(szTemp,"%d",nPos);
//		sprintf(szTemp2,"%d",m_lValueOfButton[nPos]);
//		::WritePrivateProfileString("Day Button",szTemp,szTemp2,strUserDir);
//	}	
//	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ��
//	{
//		sprintf(szTemp,"%d",nPos);
//		sprintf(szTemp2,"%d",m_lValueOfMinButton[nPos]);
//		::WritePrivateProfileString("Min Button",szTemp,szTemp2,strUserDir);
//	}	
//	for(nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)		// ƽ
//	{
//		sprintf(szTemp,"%d",nPos);
//		sprintf(szTemp2,"%d",m_lValueOfTickButton[nPos]);
//		::WritePrivateProfileString("Tick Button",szTemp,szTemp2,strUserDir);
//	}	
//	for(nPos=0;nPos<CNTOFMAXCBITEMS;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		sprintf(szTemp2,"%d",m_lValueOfCombo[nPos]);
//		::WritePrivateProfileString("Combo",szTemp,szTemp2,strUserDir);
//	}
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

	strTemp.Format("%d", m_cbLineType.GetCurSel());
	::WritePrivateProfileString("InputBar","LineType",strTemp,strUserDir);
}

void CInputBarDlg::SaveValue()
{	
	SaveIniValue();
}

void CInputBarDlg::OnBtrequestdatmore() 
{
	// TODO: Add your control notification handler code here
//-->@080523-002
//	if(m_pChartMng->GetSafeHwnd())	m_pChartMng->SendMessage(UMSG_REQUESTMORE);

	CString strCntOfDatAll;
	m_editCntOfDatAll.GetWindowText(strCntOfDatAll);
	int nCnt = atoi(strCntOfDatAll);
	nCnt += 100;
	//KHD : IBK �ִ�ġ ���� : ������ ������ �����ؼ� �������⶧���� ��Ʈ�� �ױ� ������ ���� 
	//���� ���������� ���� �����ָ� �� 
	int nMax = 1000;
	if(m_nMarketType ==CMasterTypeInfo::STOCK_CHART || m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
		nMax = 1000;
	else
		nMax = 1000;
	if(nCnt > nMax)
	{
		CString str;
		str.Format("������ %d�� ���� �� �����ϴ�. ", nMax);
		AfxMessageBox(str);
		return ;
	}
	//END
	strCntOfDatAll.Format("%d", nCnt);
	m_editCntOfDatAll.SetWindowText(strCntOfDatAll);

	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetCntForRequest(nCnt);

	if(atol(strCntOfDatAll)>0)
	{
		m_pShareInputData->m_DataInputBar.m_lRequestCnt = atol(strCntOfDatAll);
//������ �߰��θ��⸦ �Ҷ� �� �ѹ��� 
// 		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
// 			m_pChartMng->SendMessage(UMSG_SETCNTS,atol(strCntOfDatAll));
	}

	OnBtnRetry();
//<--
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

	m_chkDay.ShowWindow(nCmdShow1);
	m_chkWeek.ShowWindow(nCmdShow1);
	m_chkMonth.ShowWindow(nCmdShow1);
	m_chkMinute.ShowWindow(nCmdShow1);
//	m_chkSec.ShowWindow(nCmdShow1);
//	m_chkTick.ShowWindow(nCmdShow1);
	m_editPeriod.ShowWindow(nCmdShow1);

	m_editZoomValue.ShowWindow(nCmdShow2);
	m_editCntOfDatAll.ShowWindow(nCmdShow2);
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
		SHIFTCONTROL(m_chkMinute,nShiftExt61,rect);
//		SHIFTCONTROL(m_chkSec,nShiftExt61,rect);
//		SHIFTCONTROL(m_chkTick,nShiftExt61,rect);
		SHIFTCONTROL(m_editPeriod,nShiftExt61,rect);

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
		if(m_nMarketType==CMasterTypeInfo::FUTOPT_CHART)			
		{
			m_dlgPopupListForHistory = new CPopupList(&m_lstInfoFuture, 0, this);
		}
		else if(m_nMarketType==CMasterTypeInfo::UPJONG_CHART)
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
	SaveValue();
	
//	if(m_dlgPopupListForHistory) delete m_dlgPopupListForHistory;
	RemoveHistoryInfo();

//	m_brBkColor.DeleteObject();
	m_szTimerCodeList.RemoveAll();

	m_brBkColor.DeleteObject();

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
//		if( m_dlgPopupListForHistory->GetSafeHwnd() )
//		{
//			m_dlgPopupListForHistory->DestroyWindow();
//		}

//		CString sss;
//		m_editCode.GetWindowText(sss);
		if( str == m_strCode )
		{
			CCodeDataFromInputBar CodeData;
			CodeData.m_strCode = GetCurrentCode();
			CodeData.m_lEndTime = GetEndDate();
			CodeData.m_chType = GetType();
			CodeData.m_lTypeNum = GetTypeNumber();
			CodeData.m_cChartShape = m_cChartShape;

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
		CodeData.m_cChartShape = m_cChartShape;

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
//080520-001 CodeInput����		SetShareCodeChanged(TRUE);
//080520-001 CodeInput����		m_ctrlCodeInput.SetDataText(szCode.Mid(1));
	}
	else
	{
//080520-001 CodeInput����		SetShareCodeChanged(TRUE);
//080520-001 CodeInput����		m_ctrlCodeInput.SetDataText(szCode);
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
		case CMasterTypeInfo::UPJONG_CHART:
			{
				if(csCode.GetLength()==UPJONG_LENGTH)
				{
					CString strKey;
					strKey = csCode;

					if(m_pDataManager != NULL)
						//csName			= m_pDataManager->GetCodeInfo("UMASTER", 0, strKey);
						csName			= m_pDataManager->GetCodeName(strKey);
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
						//csName			= m_pDataManager->GetCodeInfo("FMASTER:PMASTER", 0, csCode);
						csName			= m_pDataManager->GetCodeName(csCode);
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
		case CMasterTypeInfo::STOCK_CHART: 
			{
				if(csCode.GetLength()==CODE_STOCK_LENGTH)
				{
					if(m_pDataManager != NULL)
					{
						csCode.TrimRight();
						//csName		= m_pDataManager->GetCodeInfo("JMASTER", 0, csCode);
						csName		= m_pDataManager->GetCodeName(csCode);
						if(csName.GetLength()<1)
							//csName		= m_pDataManager->GetCodeInfo("KMASTER", 0, csCode);
							csName		= m_pDataManager->GetCodeName(csCode);
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
//-->@�޴��׽�Ʈ
	CPoint ps;
	GetCursorPos(&ps);

	{
		CRect rect;
		GetDlgItem(IDC_BTINC)->GetWindowRect(&rect);
		ps.x = rect.left;
		ps.y = rect.bottom;
	}
	ShowScreen1Menu(ps);
}

#include "../Chart_Common/BCMenu.h"
#define UM_GET_SCREENLINKMENU (WM_USER + 235) 
HWND CInputBarDlg::FrameMenuInsert(CMenu* _pChartMenu, CMenu* pFrameMenu)
{
	BCMenu* pChartMenu = (BCMenu*)_pChartMenu;
//	return NULL;
	
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	if (pSTDialog == NULL)	return NULL;

	HWND hChildFrame = (HWND)::SendMessage(pSTDialog->m_hKSDllView, RMSG_KSDLLINFO, 1, 0);
	if (hChildFrame == NULL)
	{
		if(g_pDebugMng)
			g_pDebugMng->Trace("ChildFrame�ڵ��� ��������.");
		return NULL;
	}

	// LPARAM 3�̸� ���ɻ� �޴�
	long lMenu = ::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 );
	if(!lMenu) return NULL;

	BCMenu* pCommonMenu = (BCMenu*)lMenu;
	pFrameMenu = (CMenu*)lMenu;

	int nCommonMenuCount = pCommonMenu->GetMenuItemCount();
	CString strMenuString;
	UINT nCommandID ;
	UINT nFlagsChild;

	MENUITEMINFO info;
	info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
	info.fMask = MIIM_STATE;             // get the state of the menu item

	BOOL bLastSep = FALSE;
	for(int i=0; i<nCommonMenuCount; i++)
    {
		//GetMenuState���� ���� ��������.BCMenu������ �̻���.
		//nFlagsChild = pCommonMenu->GetMenuState(nCommandID, MF_BYPOSITION);	
		nCommandID = pCommonMenu->GetMenuItemID(i);
		pCommonMenu->GetMenuItemInfo(nCommandID, &info);
		pCommonMenu->GetMenuText (i, strMenuString, MF_BYPOSITION);

		//if(nFlagsChild & MF_SEPARATOR)
		if(strMenuString.GetLength()==0) 
		{
			pChartMenu->AppendMenu(MF_SEPARATOR, 0, "");
			bLastSep = TRUE;
		}
		else
		{
			BOOL bCheck = info.fState & MF_CHECKED;
			if( bCheck )
			{
				nFlagsChild = MF_STRING|MF_CHECKED;
			}
			else
			{
				nFlagsChild = MF_STRING|MF_UNCHECKED;
			}
			pChartMenu->AppendMenu(nFlagsChild, nCommandID, strMenuString);
			bLastSep = FALSE;
		}
    }
	if(nCommonMenuCount>0 && bLastSep==FALSE) pChartMenu->AppendMenu(MF_SEPARATOR, 0, "");

	pFrameMenu->DestroyMenu();
	delete pFrameMenu;

	return hChildFrame;


//	if ( lMenu > 0 )
//	{
//		CMenu* pCommonMenu = (CMenu*)lMenu;
//		ASSERT ( pCommonMenu );
//
//		int nCommonMenuCount = pCommonMenu->GetMenuItemCount();
//
//		if(nCommonMenuCount>0)
//		{
//			for(int i=0; i<nCommonMenuCount; i++)
//			{
//				//�������� ���� �ʿ�.
//				CString strMenuItem, strMenuString;
//				strMenuItem.Format("%d", i);
//				UINT nCommandID = pCommonMenu->GetMenuItemID(i);
//				pCommonMenu->GetMenuString (i, strMenuString, MF_BYPOSITION);
//		
//				g_pDebugMng->Trace("[%d][%s]", i, strMenuString);
//
//				if((strMenuString == "��������") || (strMenuString == "�� ���̱�")) continue; 
//				
//				if(pCommonMenu->GetSubMenu(i))	//����޴� ����
//				{
//					continue;
//				}
//				
//				//lResult = pCommonMenu->GetMenuState(nCommandID, MF_BYCOMMAND);
//				UINT nFlagsChild = pCommonMenu->GetMenuState(nCommandID, MF_BYCOMMAND);
//				//MENUITEMINFO mi;
//				//mi.cbSize = sizeof(MENUITEMINFO);
//				//mi.fMask = MIIM_STATE;
//				//mi.fState = lResult & ~MF_OWNERDRAW;
//				
//				if(nFlagsChild & MF_SEPARATOR)
//					pChartMenu->AppendMenu(MF_SEPARATOR, 0, "");
//				else
//				{
//					//UINT nFlagsChild = MF_STRING|MF_UNCHECKED;
//					pChartMenu->AppendMenu(nFlagsChild, nCommandID, strMenuString);
//				}
//			}
//			pChartMenu->AppendMenu(MF_SEPARATOR, 0, "");
//		}
//	}
//	else
//	{
//		g_pDebugMng->Trace("������ �޴��ڵ��� ��������.");
//	}
}

void CInputBarDlg::FrameMenuShow(CPoint &ps)
{
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	if (pSTDialog == NULL)	return;

	HWND hChildFrame = (HWND)::SendMessage(pSTDialog->m_hKSDllView, RMSG_KSDLLINFO, 1, 0);
	if (hChildFrame == NULL)
	{
		if(g_pDebugMng)
			g_pDebugMng->Trace("ChildFrame�ڵ��� ��������.");
		return;
	}
	// LPARAM 3�̸� ���ɻ� �޴�
	long lMenu = ::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 );
	if(!lMenu) return;

	BCMenu* pMainMenu = (BCMenu*)lMenu;
	UINT nID = 2000;
	pMainMenu->AppendMenu(MF_STRING	,nID++	,"��ġ��ȸ");	//2000

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = pMainMenu->TrackPopupMenu(nFlagsForMenu, ps.x, ps.y, this);
	pMainMenu->DestroyMenu();
	delete pMainMenu;

	if(!nRetValue) return;


	if( (nRetValue>=4310 && nRetValue<4320) ||
		(nRetValue>=33000 && nRetValue<34000))
	{
		::PostMessage(hChildFrame, WM_COMMAND, nRetValue, 0);
		return;
	}

	switch(nRetValue)
	{
		case 2000:	//��ġ��ȸ
			GetParent()->SendMessage(USMG_SHOW_DATAWND, 0, 0);
			break;
	}
}

void CInputBarDlg::ShowScreen1Menu(CPoint &ps ,int nScreenType)
{
	return; //@Solomon-091119:������ ������ ������ �������ؼ� ����.

	if(nScreenType==CStdDialog::Screen_Compchart || nScreenType==CStdDialog::Screen_ELWChart || nScreenType==CStdDialog::Screen_ForeignChart)
	{
		FrameMenuShow(ps);
		return;
	}

	BCMenu menu, childMenu1, childMenu2;
    menu.CreatePopupMenu();
	childMenu1.CreatePopupMenu();
	childMenu2.CreatePopupMenu();

	BCMenu* pMainMenu = &menu;
	CMenu* pFrameMenu = NULL;
	HWND hChildFrmae = FrameMenuInsert(&menu, pFrameMenu);	//ChildFrame�޴� ���� �ø���

	UINT nID = 1000;
	// �׷츮��Ʈ �߰�
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	CString szSelectGroup = pSTDialog->m_stDlgCompData.m_szSelectGroup;
	CString szTmp;
	UINT nFlagsChild;

	pMainMenu->AppendMenu(MF_POPUP, (UINT)childMenu1.GetSafeHmenu(), "�񱳱׷�");

	CDlgCompGroupData* pSelGroupData = NULL;
	for(POSITION pos=pSTDialog->m_stDlgCompData.m_GroupList.GetHeadPosition(); pos;)
	{
		nFlagsChild = MF_STRING|MF_UNCHECKED;

		CDlgCompGroupData* pGroupData = pSTDialog->m_stDlgCompData.m_GroupList.GetNext(pos);
		if(pGroupData->m_szGroupName.Compare(szSelectGroup)==0)
		{
			pSelGroupData = pGroupData;
			m_pSelGroupData = pGroupData;
			nFlagsChild = MF_STRING|MF_CHECKED;
		}
		childMenu1.AppendMenu(nFlagsChild,nID++, pGroupData->m_szGroupName);
	}

	nID = 1900;
	pMainMenu->AppendMenu(MF_POPUP, (UINT)childMenu2.GetSafeHmenu(), "����");
	BOOL bShowAll = pSTDialog->m_stDlgCompData.m_bShowAll;
	if(pSelGroupData)
	{
		nFlagsChild = MF_STRING|MF_UNCHECKED;
		if(pSTDialog->m_stDlgCompData.m_bShowAll)
			nFlagsChild = MF_STRING|MF_CHECKED;
		
		childMenu2.AppendMenu(nFlagsChild,nID++, "�񱳱׷�");
		childMenu2.AppendMenu(MF_SEPARATOR, 0, "");

		m_szTimerCodeList.RemoveAll();
		for(POSITION jPos=pSelGroupData->m_ItemList.GetHeadPosition(); jPos;)
		{
			CDlgCompSubItem* pItem = pSelGroupData->m_ItemList.GetNext(jPos);
			if(pItem->m_szCode==pSTDialog->m_stDlgCompData.m_szSelCode)
			{
				if(bShowAll)
					nFlagsChild = MF_STRING|MF_UNCHECKED;
				else
					nFlagsChild = MF_STRING|MF_CHECKED;
			}
			else
			{
				nFlagsChild = MF_STRING|MF_UNCHECKED;
			}

			childMenu2.AppendMenu(nFlagsChild,nID++, pItem->m_szCodeName);
			m_szTimerCodeList.AddTail(pItem->m_szCode);
		}
	}

	pMainMenu->AppendMenu(MF_SEPARATOR, 0, "");

	nID = 2000;
	pMainMenu->AppendMenu(MF_STRING	,nID++	,"��ġ��ȸ");	//2000

	if(pSTDialog->m_stDlgCompData.m_bTimerUse)
		pMainMenu->AppendMenu(MF_STRING|MF_CHECKED,nID++	,"��������");	//2001
	else
		pMainMenu->AppendMenu(MF_STRING	,nID++	,"��������");	//2001

	pMainMenu->AppendMenu(MF_SEPARATOR, 0, "");

	pMainMenu->AppendMenu(MF_STRING	,nID++	,"����Ʈ����");	//2002
	//@pMainMenu->AppendMenu(MF_STRING	,nID++	,"��Ʈȭ�鼳��");	//2003

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;

	int nRetValue = pMainMenu->TrackPopupMenu(nFlagsForMenu, ps.x, ps.y, this);
	childMenu1.DestroyMenu();
	childMenu2.DestroyMenu();
	menu.DestroyMenu();
	//���ο��� ���� ���� FrameMenuInsert(..)������ ó����.

	if(!nRetValue) return;

	if( (nRetValue>=4310 && nRetValue<4320) ||
		(nRetValue>=33000 && nRetValue<34000))
	{
		::PostMessage(hChildFrmae, WM_COMMAND, nRetValue, 0);
		return;
	}

	if(nRetValue>=1000 && nRetValue<1900)
	{
		int nIndex = nRetValue-1000;
		CDlgCompGroupData* pGroupData = pSTDialog->m_stDlgCompData.m_GroupList.GetAt(pSTDialog->m_stDlgCompData.m_GroupList.FindIndex(nIndex));
		pSTDialog->m_stDlgCompData.m_szSelectGroup = pGroupData->m_szGroupName;
		m_pSelGroupData = pGroupData;

		// ���⿡�� �񱳱׷� ����� �ڵ������Ѵ�.
		// �̶� �������� ������ üũ�Ѵ�.
		if(pSTDialog->m_stDlgCompData.m_bTimerUse)
		{
			pSTDialog->m_stDlgCompData.m_bShowAll = 0;
			pSTDialog->m_stDlgCompData.m_szSelCode = "";
			//pSTDialog->m_stDlgCompData.m_szSelCodeName = "";
			m_szCurTimerCode = "";
			// ���� �׷��� �ٽ� �����Ѵ�.
			fnReLoadstDlgCompData();
		}
		else
		{
			pSTDialog->m_stDlgCompData.m_bShowAll = 1;
			m_bOldShowAll = 1;

			// ���� �׷��� �ٽ� �����Ѵ�.
			fnReLoadstDlgCompData();

			m_cmbChartViewType.SetCurSel(3);
			OnBtnRetry();
		}

		pSTDialog->m_stDlgCompData.m_pFn->SaveShowGroupInfo((long)&pSTDialog->m_stDlgCompData);
		pSTDialog->m_stDlgCompData.m_pFn->SaveSelGroup((long)&pSTDialog->m_stDlgCompData);
		return;
	}
	if(nRetValue>=1900 && nRetValue<2000)
	{
		// �񱳱׷� ������ ���.
		if(nRetValue==1900)
		{
			pSTDialog->m_stDlgCompData.m_bShowAll = 1;
			m_bOldShowAll = 1;
			
			pSTDialog->m_stDlgCompData.m_pFn->SaveShowGroupInfo((long)&pSTDialog->m_stDlgCompData);

			pSTDialog->m_stDlgCompData.m_bTimerUse = 0;
			pSTDialog->m_stDlgCompData.m_pFn->SaveTimeUse((long)&pSTDialog->m_stDlgCompData);

			m_cmbChartViewType.SetCurSel(3);
			OnBtnRetry();
		}
		else	// �������� ������ ���.
		{
			int nIndex = nRetValue-1901;
			CDlgCompSubItem* pItem = pSelGroupData->m_ItemList.GetAt(pSelGroupData->m_ItemList.FindIndex(nIndex));

			pSTDialog->m_stDlgCompData.m_bShowAll = 0;
			pSTDialog->m_stDlgCompData.m_szSelCode = pItem->m_szCode;
			pSTDialog->m_stDlgCompData.m_szSelCodeName = pItem->m_szCodeName;

			pSTDialog->m_stDlgCompData.m_pFn->SaveShowGroupInfo((long)&pSTDialog->m_stDlgCompData);
			pSTDialog->m_stDlgCompData.m_szSelCode = pItem->m_szCode;
			m_szCurTimerCode = pItem->m_szCode;

			m_cmbChartViewType.SetCurSel(0);
			fnFluctTimerNextCodeSend(FALSE);
//			AfxMessageBox(pItem->m_szCodeName);
		}
		return;
	}

	switch(nRetValue)
	{
	case 2000:	//��ġ��ȸ
		GetParent()->SendMessage(USMG_SHOW_DATAWND, 0, 0);
		break;
	case 2001:	//��������
		// �������⸦ ����ϸ� pSTData->m_bShowAll ������ ��������(1)���� ���õǾ�� ��.
		// ����� ���߸� �׳� �� ����.
		pSTDialog->m_stDlgCompData.m_bTimerUse = !pSTDialog->m_stDlgCompData.m_bTimerUse;
		if(pSTDialog->m_stDlgCompData.m_bTimerUse)
		{
			m_cmbChartViewType.SetCurSel(0);
			pSTDialog->m_stDlgCompData.m_bShowAll = 0;
		}
		else
		{
			//m_cmbChartViewType.SetCurSel(2);
		}
		pSTDialog->m_stDlgCompData.m_pFn->SaveTimeUse((long)&pSTDialog->m_stDlgCompData);
		pSTDialog->m_stDlgCompData.m_pFn->SaveShowGroupInfo((long)&pSTDialog->m_stDlgCompData);

		fnFluctTimerStart(pSTDialog->m_stDlgCompData.m_bTimerUse);
		//

		break;
	case 2002:	//����Ʈ����
		//if(m_nScreenType!=1)
		if(m_nScreenType!=CStdDialog::Screen_fluctuation)	//�������Ʈ�϶�
		{
			AfxMessageBox("�������Ʈ(4602)������ �����մϴ�.");
			return;
		}
		// ����Ʈ ������ ����Ǹ�
		// CStdDialog::SetupChartUtil()���� ConfirmDlgSetupData(..)�� ȣ���Ѵ�.
		pSTDialog->SetupChartUtil();
		break;
	case 2003: //��Ʈȯ�漳��
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->ShowSelEnvironmentDlg(2);
		break;
	}
}



void CInputBarDlg::SetMode(int nMode)
{
	// �������� �߰� UI : �������� - ojtaso (20070522)
	m_nCurrentMode = nMode;	
	m_cmbChartViewType.SetCurSel(nMode);

	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_CHANGEMODE, 0, m_nCurrentMode);
}


//HBRUSH CInputBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//{
//	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
//	
//	// TODO: Change any attributes of the DC here
//	switch(nCtlColor) 
//	{
//	case CTLCOLOR_EDIT:
//	case CTLCOLOR_LISTBOX:
//		return hbr;
//		break;
//	default:
//		pDC->SetBkColor(m_crBk);
//		return m_brBkColor;
//	}
//	
//	// TODO: Return a different brush if the default is not desired
//	return hbr;
//}

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
//						for(int n = 0;n<8;n++)//ƽ �� �� �� �� �� 10�� 30�� 
//						{
//							m_lCntInView[n] = atol(strZoomValue);
//						}
						//}}
						//========================================================================

						if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						{
							m_pShareInputData->m_DataInputBar.m_lDispCnt = atol(strZoomValue);
							m_nFixZoomValue = atol(strZoomValue);

							m_pChartMng->SendMessage(UMSG_SETZOOMVALUE,atol(strZoomValue));

//							CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
//							char szTemp[8];
//							char szTemp2[8];
//							for(int nPos=0;nPos<5;nPos++)
//							{
//								sprintf(szTemp,"%d",nPos);
//								sprintf(szTemp2,"%d",m_lCntInView[nPos]);
//								::WritePrivateProfileString("CntInView",szTemp,szTemp2,strUserDir);
//							}
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
						m_pShareInputData->m_DataInputBar.m_lRequestCnt = atol(strCntOfDatAll);

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
					if(i>=1000 || i <= 0) 					
					{
						AfxMessageBox("1000������ ������ �ٽ� �����Ͽ� �ֽʽÿ�");
						return TRUE;
					}

					if(i > 0)
					{						
						m_pShareInputData->m_DataInputBar.m_lTypeNum = i;

						//SetType();
						Test004(m_chType, i);
                                                
                        //term�� �־� ��ȸ�Ѵ�
						SetTimer(IDTIMER_REQSTEP_WAIT, 200, NULL);
						//RequestStepAll();
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

//080520-001 CodeInput����	if(::IsChild(m_ctrlCodeInput.GetSafeHwnd(), pMsg->hwnd)
//080520-001 CodeInput����			)
//080520-001 CodeInput����		{
//080520-001 CodeInput����			return FALSE;
//080520-001 CodeInput����		}
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
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	if (pSTDialog == NULL)	return ;

	HWND hChildFrame = (HWND)::SendMessage(pSTDialog->m_hKSDllView, RMSG_KSDLLINFO, 1, 0);
	if (hChildFrame == NULL)
	{
		if(g_pDebugMng)
			g_pDebugMng->Trace("ChildFrame�ڵ��� ��������.");
		return ;
	}

	// LPARAM 3�̸� ���ɻ� �޴�
	long lMenu = ::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 );
	if(!lMenu) return;
//	return (CMenu*)lMenu;

	BCMenu* pMainMenu = (BCMenu*)lMenu;

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	POINT pt;
	pt.x = point.x; pt.y=point.y;
	ClientToScreen(&pt);
	int nRetValue = pMainMenu->TrackPopupMenu(nFlagsForMenu, pt.x, pt.y, this);

	if(!nRetValue) return;

	if( (nRetValue>=4310 && nRetValue<4320) ||
		(nRetValue>=33000 && nRetValue<34000))
	{
		::PostMessage(hChildFrame, WM_COMMAND, nRetValue, 0);
		return;
	}

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

//void CInputBarDlg::OnChkextbar() 
//{
//	// TODO: Add your control notification handler code here
//	BOOL bShowExtInputBarDlg = ((CStdDialog*)GetParent())->GetShowExtInputBarDlg();
//	((CStdDialog*)GetParent())->SetShowExtInputBarDlg(!bShowExtInputBarDlg);
//}

//void CInputBarDlg::SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg)
//{
//	if(IsMiniBar())
//		return;
//	m_chkExtBar1.SetCheck(bShowExtInputBarDlg);
//	if(bShowExtInputBarDlg)
//	{
//		//		m_chkExtBar1.FP_vSetImage( m_pIL_W20H20, 28, 29);
//		m_chkExtBar1.FP_vSetImage( m_pIL_W20H20, 44, 46);
//		m_chkExtBar1.FP_vSetToolTipText("����������");
//	}
//	else
//	{
//		//		m_chkExtBar1.FP_vSetImage( m_pIL_W20H20, 30, 31);
//		m_chkExtBar1.FP_vSetImage( m_pIL_W20H20, 48, 50);
//		m_chkExtBar1.FP_vSetToolTipText("�������Ȯ��");
//	}
//	m_chkExtBar1.Invalidate();
//}


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
	if(!((CStdDialog*)GetParent())->GetStatusEnable()) return FALSE;	

	//�Ʒ� Return�� �ؼ� TR��ȸ�� �ȵȰų� �� ��� �Ʒ� ��ƾ�� Ÿ���� �Ѵ�.
//	return; //2005. 07. 13 
	//===================================================================
	BOOL bRet = FALSE;

	if(nMarketType != m_nPosCbMarketOld)
	{
		((CChartMng*)m_pChartMng)->m_bNeedForceCode = TRUE;
//		if((nMarketType == CMasterTypeInfo::FUTOPT_CHART || m_nPosCbMarketOld == CMasterTypeInfo::FUTOPT_CHART))
//		{
//			CRect rectEditCode;
//			CRect rectBtnSearch;
//			CRect rectBtCodeHistory;
//			if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)// future option
//			{
//				m_chkWeek.EnableWindow(FALSE);
//				m_chkMonth.EnableWindow(FALSE);
//			}
//			else if(nMarketType == CMasterTypeInfo::UPJONG_CHART || nMarketType == CMasterTypeInfo::STOCK_CHART || nMarketType == CMasterTypeInfo::FOREIGN_CHART || nMarketType == ELW_CHART)	// Upjong, Jongmok
//			{
//				m_chkWeek.EnableWindow();
//				m_chkMonth.EnableWindow();
//			}
//		}
		if(m_nScreenType != CStdDialog::Screen_ForeignChart)
			m_chkMinute.EnableWindow();
		m_chkSec.EnableWindow();
		m_chkWeek.EnableWindow();
		m_chkMonth.EnableWindow();

		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//@��������
//		if(nMarketType == CMasterTypeInfo::FUTOPT_CHART || nMarketType == COMMODITY_CHART)// future option
		if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)// future option
//@��������
		{
			//���� ����Ʈ �߰�
			//m_chkWeek.EnableWindow(FALSE);
			//m_chkMonth.EnableWindow(FALSE);
		}

		CString strInc;
		//m_btInc.GetWindowText(strInc);
//		if(IsMiniBar())
//		{
//			strInc = "����";
//		}

		if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
		{
			if(bUpdateCbMarket)
			{
				m_cbMarket.SetCurSel(2);
				m_nMarketType = CMasterTypeInfo::FUTOPT_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}	
		}
		else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
		{
			if(bUpdateCbMarket)
			{
				m_cbMarket.SetCurSel(1);
				m_nMarketType = CMasterTypeInfo::UPJONG_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}
		}
		else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
		{
			if(bUpdateCbMarket)
			{
				m_cbMarket.SetCurSel(3);
				m_nMarketType = CMasterTypeInfo::STOCKFUTOPT_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}
		}
		else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		{
			if(bUpdateCbMarket)
			{
				m_cbMarket.SetCurSel(4);
				m_nMarketType = CMasterTypeInfo::FOREIGN_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
			}
		}
		else
		{
			if(bUpdateCbMarket)
			{
				m_cbMarket.SetCurSel(0);
				m_nMarketType = CMasterTypeInfo::STOCK_CHART;
				if(m_nCurSelRow == 0 && m_nCurSelCol == 0 || !((CChartMng*)m_pChartMng)->IsInLoop())
					((CStdDialog*)GetParent())->ChangeStockMode(m_nMarketType); 
				else
					((CChartMng*)m_pChartMng)->SetMarketType(m_nMarketType);
				bRet = TRUE;
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

//--> @080605-001 TrmCompChart�� �����ڵ��� Type�����ϴ� ��ƾ.
//	�񱳳� ��������� �ٲ� �ʿ䰡 �����Ƿ� ���´�.
//		CWnd* pWnd = ((CStdDialog*)GetParent())->GetParent();
//		if(pWnd)
//		{
//			//(BOOL) pWnd->SendMessage(WM_USER+101,nMarketType);
//			pWnd->SendMessage(WM_USER+101, nMarketType);
//		}
//<-- @080605-001
	}
	m_nPosCbMarketOld = nMarketType;
	return bRet;
}


//void CInputBarDlg::OnChkfirst()
//{
//	// TODO: Add your control notification handler code here
//	m_nUnitPos = 0;
////	if(m_chType!='0')
////	{
////		m_chType = '1';
////	}
//	CString strPeriod;
//	if (MIN_DATA_CHART == m_chType)		// �� : 1/2
//		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
//		//strPeriod = "0.5";
//	else if (TICK_DATA_CHART == m_chType)	// ƽ
//		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
//	else if (HOUR_DATA_CHART == m_chType)	// ��
//		strPeriod.Format("%d", m_lValueOfHourButton[m_nUnitPos]);
//	else		//(DAY_DATA_CHART == m_chType)  // ��
//		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);
//
//	m_editPeriod.SetWindowText(strPeriod);
//	SetType();
//}
//
//void CInputBarDlg::OnChksecond() 
//{
//	// TODO: Add your control notification handler code here
//	m_nUnitPos = 1;
////	if(m_chType!='0')
////	{
////		m_chType = '1';
////	}
//	CString strPeriod;
//	if (MIN_DATA_CHART == m_chType)	// ��
//		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
//	else if (TICK_DATA_CHART == m_chType)	// ƽ
//		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
//	else if (HOUR_DATA_CHART == m_chType)	// ��
//		strPeriod.Format("%d", m_lValueOfHourButton[m_nUnitPos]);
//	else		//(DAY_DATA_CHART == m_chType)  // ��
//		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);
//
//	m_editPeriod.SetWindowText(strPeriod);
//	SetType();
//}
//
//void CInputBarDlg::OnChkthird() 
//{
//	// TODO: Add your control notification handler code here
//	m_nUnitPos = 2;
////	if(m_chType!='0')
////	{
////		m_chType = '1';
////	}
//	CString strPeriod;
//	if (MIN_DATA_CHART == m_chType)	// ��
//		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
//	else if (TICK_DATA_CHART == m_chType)	// ƽ
//		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
//	else if (HOUR_DATA_CHART == m_chType)	// ��
//		strPeriod.Format("%d", m_lValueOfHourButton[m_nUnitPos]);
//	else		//(DAY_DATA_CHART == m_chType)  // ��
//		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);
//
//	m_editPeriod.SetWindowText(strPeriod);
//	SetType();
//}
//
//void CInputBarDlg::OnChkfourth() 
//{
//	// TODO: Add your control notification handler code here
//	m_nUnitPos = 3;
////	if(m_chType!='0')
////	{
////		m_chType = '1';
////	}
//	CString strPeriod;
//	if (MIN_DATA_CHART == m_chType)	// ��
//		strPeriod.Format("%d", m_lValueOfMinButton[m_nUnitPos]);
//	else if (TICK_DATA_CHART == m_chType)	// ƽ
//		strPeriod.Format("%d", m_lValueOfTickButton[m_nUnitPos]);
//	else		//(DAY_DATA_CHART == m_chType)  // ��
//		strPeriod.Format("%d", m_lValueOfButton[m_nUnitPos]);
//
//	m_editPeriod.SetWindowText(strPeriod);
//	SetType();
//}
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
//void CInputBarDlg::OnDeltaposSpinDatecnt(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
//	// TODO: Add your control notification handler code here
//	CString strPeriod;
//	long lPeriod = 0;
//	m_editPeriod.GetWindowText(strPeriod);
//	if(!strPeriod.IsEmpty())
//	{
//		lPeriod = atol(strPeriod);
//		if(pNMUpDown->iDelta>0)
//		{
//			if(lPeriod<=0) return;
//			lPeriod--;
//		}
//		else
//		{
//			lPeriod++;
//		}
//
//		if(lPeriod <= 1)
//			lPeriod = 1;
//
//		strPeriod.Format("%d",lPeriod);
//		m_pShareInputData->m_DataInputBar.m_lTypeNum = lPeriod;
//		m_editPeriod.SetWindowText(strPeriod);	
//	}
//	*pResult = 0;
//}

void CInputBarDlg::Enable_PeriodControl(BOOL bEnable)
{	
	m_editPeriod.SetReadOnly(!bEnable);
	GetDlgItem(IDC_SPINPERIOD)->EnableWindow(bEnable);	
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
//-->080520-001 CodeInput����
//	switch(m_nMarketType)
//	{
//		case CMasterTypeInfo::STOCK_CHART:
//				m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_STOCK);
//				//m_ctrlCodeInput.SetMarketKind(0);
//			break;
//		case CMasterTypeInfo::UPJONG_CHART:
//				m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_ITEM);
//				//m_ctrlCodeInput.SetMarketKind(5);
//			break;
//		case CMasterTypeInfo::FUTOPT_CHART:
//			m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_FO);
//				//m_ctrlCodeInput.SetMarketKind(3);
//			break;
//		default:
//			bFind = FALSE;
//	}
//
//	if(m_ctrlCodeInput.GetSafeHwnd() != NULL)
//	{
//		CRect rectComp, ctlRect;
//		GetDlgItem(IDC_CTRL_CODEINPUT)->GetWindowRect(ctlRect);
//		ScreenToClient(ctlRect);
//
//		m_ctrlCodeInput.GetWindowRect(rectComp);
//		ScreenToClient(rectComp);
//		if(m_rectCmb.left != ctlRect.left || m_rectCmb.top != ctlRect.top)
//		{
//			if( ((CStdDialog*)GetParent())->m_nChartType == MINI_MULTI_CHART ||
//				((CStdDialog*)GetParent())->m_nChartType == MINI_SINGLE_CHART )
//			{
//				m_ctrlCodeInput.SetWindowPos(NULL, ctlRect.left, ctlRect.top, rectComp.Width(), 20, SWP_NOZORDER);
//				m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
//			}
//			else
//			{
//				m_ctrlCodeInput.SetWindowPos(NULL, ctlRect.left, ctlRect.top, rectComp.Width(), 20, SWP_NOZORDER);
//				m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
//			}
//		}
//
//		//�̹��� �ӽ��ڵ�
//		//m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
//	}
//
//	
//	if(bRequestAP)
//	{
////		//�ֽ��� ������ ������  BaseInfo�ʿ��� WMU_GET_KEYCODE ȣ���Ѵ�.
//		if(	m_nMarketType == CMasterTypeInfo::STOCK_CHART || 	m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
//		{
//			if(!m_strCodeFromOutside.GetLength())
//			{
//				sCode = m_ctrlCodeInput.GetDataText();
//				sCode.TrimRight();
//				if(!sCode.GetLength())
//				{		
//					::SendMessage(m_ctrlCodeInput.GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
//				}
//			}
//		}
//		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
////@��������
////		else if( m_nMarketType == CMasterTypeInfo::FUTOPT_CHART || m_nMarketType == COMMODITY_CHART)
//		else if( m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
////@��������
//		{
//			::SendMessage(m_ctrlCodeInput.GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
//		}
//	}
//<--080520-001
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
//@��������
//			//{{����Ű ����
//			if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART && strCode.GetLength() == (UPJONG_LENGTH-1)	)
//			{
//				if(m_nUpjongType == 101)
//					strCode = "4" + strCode;
//				else
//					strCode = "2" + strCode;
//			}
//			//}}
//@��������

			// �������� �߰� ���� : �������� - ojtaso (20070122)
			if(((CChartMng*)m_pChartMng)->GetAddGraphMode() || m_strCode != strCode)
			{
				CString strTR;
				switch(m_nMarketType)
				{
					case CMasterTypeInfo::STOCK_CHART	: strTR = QUERY_STRJONGMOK	; break;
					case CMasterTypeInfo::UPJONG_CHART	: strTR = QUERY_STRUPJONG	; break;					
					case CMasterTypeInfo::FUTOPT_CHART	: strTR = QUERY_STRFUTOPT	; break;
					case CMasterTypeInfo::STOCKFUTOPT_CHART : strTR = QUERY_STRSTOCKFUTOPT; break;
					case CMasterTypeInfo::FOREIGN_CHART : strTR = QUERY_STRFOREIGN; break;
					default				: return;
				}

				long lResult = GetParent()->SendMessage(UMSG_CHECK_BE_COMPLETE_QUERYDATA, (WPARAM)(LPTSTR)(LPCTSTR)strTR);
				if(lResult == 0) return;


				//��ǥ �������⸦ �����. ===================================================================
				GetParent()->SendMessage(UMSG_STOP_JIPYOCYCLE, 0, 0);
				//===========================================================================================
//@��������
//				if(	((m_nMarketType == CMasterTypeInfo::STOCK_CHART	) && (strCode.GetLength() == CODE_STOCK_LENGTH	)) ||							
//						((m_nMarketType == CMasterTypeInfo::UPJONG_CHART	) && (strCode.GetLength() == UPJONG_LENGTH	)) ||
//						((m_nMarketType == ELW_CHART	) && (strCode.GetLength() == CODE_STOCK_LENGTH	)) ||
//						(m_nMarketType  == CMasterTypeInfo::FUTOPT_CHART	) ||
//						((m_nMarketType == CMasterTypeInfo::FOREIGN_CHART) || (m_nMarketType == COMMODITY_CHART))	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
				if(	((m_nMarketType == CMasterTypeInfo::STOCK_CHART	) && (strCode.GetLength() == CODE_STOCK_LENGTH	)) ||							
					((m_nMarketType == CMasterTypeInfo::UPJONG_CHART	) && (strCode.GetLength() == UPJONG_LENGTH	)) ||
					((m_nMarketType  == CMasterTypeInfo::FUTOPT_CHART ) && (strCode.GetLength() == FUTOPT_LENGTH	)) ||
					((m_nMarketType  == CMasterTypeInfo::STOCKFUTOPT_CHART ) && (strCode.GetLength() == FUTOPT_LENGTH )) ||
					((m_nMarketType  == CMasterTypeInfo::FOREIGN_CHART )) )
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

void CInputBarDlg::OnSelchangeCmbType() 
{		
	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	int nIdx = m_cmbChartViewType.GetCurSel();
	if(nIdx == CB_ERR) return;
	if(m_nCurrentMode == nIdx) return; //���� ���� Return
	
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
			if(m_nCurrentMode < CHANGE_ITEM || m_nCurrentMode > OVERLAPP_ITEM)
				m_nCurrentMode = CHANGE_ITEM;
		}
		else // nDefault == MODE_USER
		{
			ASSERT(nMode >= CHANGE_ITEM && nMode <= OVERLAPP_ITEM);

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

//void CInputBarDlg::ChangeToolBarForReplayChart(BOOL bReplay)
//{
//	//��Ʈ�� ���� �Ǵ� �Ϲ� ���·� �ʱ�ȭ �ϴ� �κ��� �ʿ�.
//	//#######################################################################
//	// ���� ��� Real ����..	
//	m_pChartMng->SendMessage(UMSG_DISCONNECT_ALLREAL); // ��� Real ����
//	
//	CRect ctlRect;
//	int nOffset = 0;
//	int nHeight = 0;
//	int nReplayOffset = 40;
//
//	if(bReplay) //�� ��ư �̵�
//	{
//		//0. ����� ������ ��Ʈ ���¸� �����Ѵ�.
//		m_stJongName.GetWindowRect(ctlRect);
//		ScreenToClient(ctlRect);
//		m_stJongName.MoveWindow(ctlRect.left, ctlRect.top, ctlRect.Width() - 40, ctlRect.Height());
//
//		CString sFile = ((CStdDialog*)GetParent())->m_strConfig;
//		((CChartMng*)m_pChartMng)->SaveChart(sFile);
//
//		m_chkDay.GetWindowRect(ctlRect);
//		ScreenToClient(ctlRect);
//		
//		nOffset = ctlRect.left - nReplayOffset; 
//		nHeight = ctlRect.Height();
//
//		m_chkTick.MoveWindow(nOffset, ctlRect.top, ctlRect.Width(), ctlRect.Height());
//
//		nOffset = ctlRect.right - nReplayOffset; 
//
//		m_chkMinute.MoveWindow(nOffset, ctlRect.top, ctlRect.Width(), ctlRect.Height());
//
//		nOffset = nOffset + ctlRect.Width(); 
//		m_editPeriod.GetWindowRect(ctlRect);
//		ScreenToClient(ctlRect);
//		m_editPeriod.MoveWindow(nOffset, ctlRect.top, ctlRect.Width(), ctlRect.Height());
//
//		nOffset = nOffset + ctlRect.Width(); 
//		m_btPeriod.GetWindowRect(ctlRect);
//		ScreenToClient(ctlRect);
//		m_btPeriod.MoveWindow(nOffset, ctlRect.top, ctlRect.Width(), ctlRect.Height());
//
//		nOffset = nOffset + ctlRect.Width(); 
//
//	}
//	else
//	{
//		m_stJongName.GetWindowRect(ctlRect);
//
//		ScreenToClient(ctlRect);
//		m_stJongName.MoveWindow(ctlRect.left, ctlRect.top, ctlRect.Width() + 40, ctlRect.Height());
//	}
//	
//
//	//�� ��ư Offset Get
//	
//	m_btPeriod.GetWindowRect(ctlRect);	
//	ScreenToClient(ctlRect);
//	nOffset = ctlRect.right; 
//	nHeight = ctlRect.Height();	
//
//	//��ư ���� ����.
//	m_bRunningReplay	= FALSE;
//	m_bPauseReplay		= FALSE;
//	m_bStop		= TRUE;
//
//
//	//1) ���� ƽ/��/��/�־� ���� ���� -----------------------------------------------
//	char chDataType = MIN_DATA_CHART;
//	if(m_ExChangeBeforeInfo.chOldType != NULL)  //���� Ÿ���� �ִٸ� ����ɷ� �����Ѵ�.
//		chDataType = m_ExChangeBeforeInfo.chOldType;
//			
//	m_ExChangeBeforeInfo.chOldType = m_chType;
//	m_chType = chDataType;
//
//	//SetType(FALSE);
//
//	//2) ���� ��ȸ ���� �� ���� ----------------------------------------------------
//	int nInterval = 1;
//	if(m_ExChangeBeforeInfo.nInterval)  //���� ��ȸ���� ���� �ִٸ� ����ɷ� �����Ѵ�.
//		nInterval = m_ExChangeBeforeInfo.nInterval;
//
//	CString sInterval;
//	m_editPeriod.GetWindowText(sInterval);
//	m_ExChangeBeforeInfo.nInterval = atoi(sInterval);
//	
//	sInterval.Format("%d", nInterval);
//	m_editPeriod.SetWindowText(sInterval);
//
//	//#######################################################################
//	if(bReplay)
//	{	
//		//�ϴ� ���� ���� / ���̱� ���� 2005.12.08
//		m_ExChangeBeforeInfo.bShowExtToolBar = ((CStdDialog*)GetParent())->GetShowExtInputBarDlg();
//	
//		//�ϴ� ���� ����.2005.12.08
//		((CStdDialog*)GetParent())->SetShowExtInputBarDlg(FALSE);
//		m_chkExtBar1.EnableWindow(FALSE);	
//		
//		//12.08
//		//���⿡�� ��ư�� Disable�Ǿ��� ��...���õ� ���� �����Ǿ� ���̵��� �ϱ� ���� �̹��� 
//		//����Ʈ�� ���Ѵ�.
//		m_chkTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ƽ", TRUE, 83);
//		m_chkMinute.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE, 83);		
//
//		//1. ���� ���� ����� ===================================================
//		UINT nHideItemId[] = {	
//			IDC_CHKDAY			,
//			IDC_CHKWEEK			,
//			IDC_CHKMONTH		,
//			IDC_CHKSEC			,
//			IDC_CHKFIRST        , 
//			IDC_CHKSECOND       , 
//			IDC_CHKTHIRD        , 
//			IDC_CHKFOURTH       , 
////			IDC_CMB_BONG		,
////			IDC_CHKYEAR			,			
////			IDC_CHK_REVISED		,
//			IDC_CMB_CHARTTYPE	,			
//			IDC_CHK_DATALIST,
//			IDC_BTSAVECHART,
//			IDC_BTSETENVIR,
//			IDC_STC_TOTALCNT
//		};
//
//
//		int nCnt = sizeof(nHideItemId) / sizeof(UINT);		
//		for(int i = 0; i < nCnt ; i++)
//			GetDlgItem(nHideItemId[i])->ShowWindow(SW_HIDE);			
//	
//
//		//���� �ð� ���� ���� 
//		m_ExChangeBeforeInfo.bTimeSync = ((CStdDialog*)GetParent())->GetTimeSyncFlag();		
//		
//		((CStdDialog*)GetParent())->SetTimeSyncFlag(TRUE);
//
//		//��Ʈ Ÿ��Ʋ DrawText 
//		((CChartMng*)m_pChartMng)->SendMessage(WM_PAINT);
//
//	//2. �� ��ư �ڿ� ���� ���� ��ư���� ���̹Ƿ� �� ��ư�� Rect�� ���ؼ� ��ġ�Ѵ�.
//		UINT nReplayItemId[] = 
//		{
//			IDC_BTN_PLAY_PAUSE	,
//			IDC_BTN_STOP_REPLAY	,
//			IDC_CMB_SPEED		,
//			IDC_CMB_DAY			,
//			IDC_STATIC_DAY		,
//			IDC_CMB_HOUR		,
//			IDC_STATIC_HOUR		,
//			IDC_CMB_MIN			,
//			IDC_STATIC_MIN		,
////			IDC_CHECK_LOG		,
//			IDC_EDITZOOMVALUE		, //2006. 1.9 �ּ�ó�� =====================
//			IDC_SPINZOOMVALUE		, //2006. 1.9 �ּ�ó�� =====================
//			IDC_STATICDIV			, //2006. 1.9 �ּ�ó�� =====================
//			IDC_EDITCNTOFDATALL	, //2006. 1.9 �ּ�ó�� =====================
//			IDC_BTREQUESTDATMORE	, //2006. 1.9 �ּ�ó�� =====================
//			IDC_STATIC_REPLAYTIME,	//2006. 1.9 �ű� �߰� =====================
//			IDC_PRGBAR_REPLAY		//2006. 1.9 �ű� �߰� =====================
//
//		};		
//
//		nCnt = sizeof(nReplayItemId) / sizeof(UINT);
//		for(i = 0; i < nCnt ; i++)
//		{
//			GetDlgItem(nReplayItemId[i])->GetWindowRect(ctlRect);
//			ScreenToClient(ctlRect);			
//			
//			int nWidth = ctlRect.Width();
//
//			ctlRect.top = 2;
//			ctlRect.bottom = ctlRect.top + nHeight; 
//			ctlRect.left = nOffset;
//			ctlRect.right = nOffset + nWidth;
//
//			switch(nReplayItemId[i])
//			{
//				case IDC_CMB_SPEED	:
//				case IDC_CMB_DAY	:
//				case IDC_CMB_HOUR	:
//				case IDC_CMB_MIN	:
//					ctlRect.bottom = 300;
//					break;
//				case IDC_STATIC_REPLAYTIME:
//					{
//						GetDlgItem(IDC_EDITZOOMVALUE)->GetWindowRect(ctlRect);
//						ScreenToClient(ctlRect);	
//						ctlRect.left += 5;
//						ctlRect.right = ctlRect.left + nWidth;
//					}
//					break;
//				case IDC_PRGBAR_REPLAY:
//					{
//						ctlRect.top += 2;
//						ctlRect.bottom -= 5;
//					}
//					break;
//			}
//
//			GetDlgItem(nReplayItemId[i])->MoveWindow(ctlRect);
//			switch(nReplayItemId[i])
//			{
//				case IDC_STATIC_REPLAYTIME	:
//				case IDC_PRGBAR_REPLAY		:
//					GetDlgItem(nReplayItemId[i])->ShowWindow(SW_HIDE);
//					break;
//				default:
//					GetDlgItem(nReplayItemId[i])->ShowWindow(SW_SHOW);
//					break;
//			}
//
//			nOffset = ctlRect.right;
//		}
//
//		//4. ȣ��â Delete
//		if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
//			GetParent()->SendMessage(UMSG_DESTROY_FLOATINGHOGA, 0);
//
//		//5. ���� ������ �ѱ��.
//		if( m_pReplayInfo ) 
//		{
//			delete m_pReplayInfo;
//			m_pReplayInfo = NULL;
//		}
//		
//		m_pReplayInfo = new REPLAY_INFO;
//		if(m_pReplayInfo)
//		{
//			//InputBar���� ä��� �ִ� �κи� ä��� �������� CStdDialog���� ä���.
//			m_pReplayInfo->nDataKind	= m_nMarketType;
//			m_pReplayInfo->cGubun		= MIN_DATA_CHART;
//			m_pReplayInfo->nUnit		= m_nMin;
//
//			//���
//			int nIdx = m_cmbReplaySpeed.GetCurSel();
//			if(nIdx == -1)
//			{
//				nIdx = 0;
//			}
//			m_pReplayInfo->nSpeed		= (int)pow(2.0, (double)nIdx);
//
//			//���۽ð� 
//			CString strSDate, sHour, sMin;
//
//			int nIdx2 = m_cmbReplayHour	.GetCurSel(); if(nIdx2 < 0) nIdx2 = 0;
//			int nIdx3 = m_cmbReplayMin	.GetCurSel(); if(nIdx3 < 0) nIdx3 = 0;
//
//			m_cmbReplayHour	.GetLBText(nIdx2, sHour);
//			m_cmbReplayMin	.GetLBText(nIdx3, sMin);
//			strSDate = "00000000"; //����� ���ϰ͸� �ϹǷ� 
//			strSDate += sHour;
//			strSDate += sMin;
//			m_pReplayInfo->strStartDate	= strSDate;	
//			
//			//n����
//			int nIdx4 = m_cmbDayBefore.GetCurSel(); if(nIdx4 < 0) nIdx4 = 0;
//			m_pReplayInfo->nDayBefore = nIdx4;
//
//			
//			//Type�� �̸� ����
//			OnChkminute();
//	
//			//if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			//	m_pChartMng->SendMessage(UMSG_SETTYPELINK,1); 
//
//			//((CStdDialog*)GetParent())->RqstTRForReplay();
//		}
//
//	
//	}
//	else
//	{	
//		if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
//			GetParent()->SendMessage(UMSG_DESTROY_FLOATINGHOGA, 1);
//
//		//�ϴ� ���� ������ ������ ���� .2005.12.08
//		if(m_ExChangeBeforeInfo.bShowExtToolBar)
//			((CStdDialog*)GetParent())->SetShowExtInputBarDlg(TRUE);
//
//		m_chkTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "ƽ", TRUE);
//		m_chkMinute.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "��", TRUE);
//	
//		m_chkExtBar1.EnableWindow(TRUE);
//
//		
//
//		EnableWindow(TRUE);
//
//	//2. ���� ���� ����� =================================================================
//		UINT nReplayItemId[] = 
//		{
//			IDC_BTN_PLAY_PAUSE	,
//			IDC_BTN_STOP_REPLAY	,
//			IDC_CMB_SPEED		,
//			IDC_CMB_DAY			,
//			IDC_STATIC_DAY		,
//			IDC_CMB_HOUR		,
//			IDC_STATIC_HOUR		,
//			IDC_CMB_MIN			,
//			IDC_STATIC_MIN		,
//			IDC_STATIC_REPLAYTIME,	//2006. 1.9 �ű� �߰� =====================
//			IDC_PRGBAR_REPLAY		//2006. 1.9 �ű� �߰� =====================
//		};
//
//		int nCnt = sizeof(nReplayItemId) / sizeof(UINT);		
//		for(int i = 0; i < nCnt ; i++)
//			GetDlgItem(nReplayItemId[i])->ShowWindow(SW_HIDE);
//
//	//3. ���� �⺻ ��ư�� �̵��� ���̱� ===================================================
//		UINT nShowItemId[] = {		
//			IDC_CHKDAY			,
////			IDC_CHKHOUR			,
//			IDC_CHKWEEK			,
//			IDC_CHKMONTH		,
//			IDC_CHKMINUTE		,
//			IDC_CHKSEC			,
//			IDC_CHKTICK			,
//			IDC_CHKFIRST        , 
//			IDC_CHKSECOND       , 
//			IDC_CHKTHIRD        , 
//			IDC_CHKFOURTH       , 
////			IDC_CHKYEAR			,			
////			IDC_CHK_REVISED		,
////			IDC_CHECK_LOG		,
////			IDC_CMB_CHARTTYPE	,			
//			IDC_EDITPERIOD		,
//			IDC_BTPERIOD		,
////			IDC_CMB_BONG		,
//			IDC_EDITZOOMVALUE	,
//			IDC_SPINZOOMVALUE	,
//			IDC_STATICDIV		,
//			IDC_STC_TOTALCNT	,
//			IDC_EDITCNTOFDATALL	,
//			IDC_BTREQUESTDATMORE,
//			IDC_BTSAVECHART
////			IDC_BTSETENVIR
//		};
//
//		BOOL bCompareChartItem = ((CStdDialog*)GetParent())->IsMultiItemChart();
//		nCnt = sizeof(nShowItemId) / sizeof(UINT);		
//		for(i = 0; i < nCnt ; i++)
//		{
//			if(!bCompareChartItem && nShowItemId[i] == IDC_STC_TOTALCNT)
//				continue;
//
//			GetDlgItem(nShowItemId[i])->GetWindowRect(ctlRect);
//			ScreenToClient(ctlRect);					
//				
//			int nWidth = ctlRect.Width();
//
//			if(i==0)
//				nOffset = ctlRect.left;
//			if(nShowItemId[i]==IDC_CHKFIRST || nShowItemId[i]==IDC_EDITPERIOD)
//				nOffset += 2;
//			ctlRect.left = 0;
//			ctlRect.left = nOffset;
//			ctlRect.right = nOffset + nWidth;		
//
//			GetDlgItem(nShowItemId[i])->MoveWindow(ctlRect);
//			GetDlgItem(nShowItemId[i])->ShowWindow(SW_SHOW);
//
//			nOffset = ctlRect.right;
//		}
//
//		// �ð� ���� ������
//		((CStdDialog*)GetParent())->SetTimeSyncFlag(m_ExChangeBeforeInfo.bTimeSync);
//
//		//����� ��Ʈ ������ �о�´�.
//		//0. ����� ������ ��Ʈ ���¸� �����Ѵ�.
//		((CChartMng*)m_pChartMng)->LoadChart(((CStdDialog*)GetParent())->m_strConfig);
//
//		
//	}
//}

void CInputBarDlg::SetCompareChartItemMode()
{
	if(!((CStdDialog*)GetParent())->IsMultiItemChart())
	{
		UINT nShowItemId[] = {		
				IDC_STATICDIV		,
				IDC_STC_TOTALCNT	,
				IDC_EDITCNTOFDATALL	,
				IDC_BTREQUESTDATMORE,
//				IDC_BTSAVECHART
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
			if(nShowItemId[i]==IDC_EDITPERIOD)
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

void CInputBarDlg::SetCurCodeNameToControl(CString strCode)
{
//	if(IsMiniBar())
//		return;

	if(strCode == _T(""))
	{
		m_strJonMokName = _T("");
		UpdateData(FALSE);
		return;
	}

	CString strCodeName;
	long lPos = -1;
	switch(m_nMarketType)
	{
		case CMasterTypeInfo::STOCK_CHART:
			strCode.TrimRight();
			//strCodeName		= m_pDataManager->GetCodeInfo("JMASTER", 0, strCode);
			strCodeName		= m_pDataManager->GetCodeName(strCode);
			//if(strCodeName.GetLength()<1)
			//	strCodeName		= m_pDataManager->GetCodeInfo("KMASTER", 0, strCode);
			//if(strCodeName.GetLength()<1)
			//	strCodeName		= m_pDataManager->GetCodeInfo("WMASTER", 0, strCode);

			lPos = strCodeName.Find('&');
			if(lPos>0)
			{
				strCodeName.Insert(lPos,'&');
			}
			break;
		case CMasterTypeInfo::FUTOPT_CHART:
			//strCodeName		= m_pDataManager->GetCodeInfo("FMASTER", 0, strCode);
			strCodeName		= m_pDataManager->GetCodeName(strCode);
			//if(strCodeName.GetLength()<1)
			//	strCodeName		= m_pDataManager->GetCodeInfo("PMASTER", 0, strCode);
			break;
		case CMasterTypeInfo::UPJONG_CHART:
			//strCodeName		= m_pDataManager->GetCodeInfo("UMASTER", 0, strCode);
			strCodeName		= m_pDataManager->GetCodeName(strCode);
			break;
	}

	m_strJonMokName = strCodeName;
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
	strCode.TrimRight();
	strCodeName = m_pDataManager->GetCodeName(strCode);
	return strCodeName;

/*
	switch(nMarketType)
	{
// 		case CMasterTypeInfo::STOCK_CHART:
// 			strCodeName		= m_pDataManager->GetCodeInfo("JMASTER", 0, strCode);			
// 			if(strCodeName.GetLength()<1)
// 				strCodeName		= m_pDataManager->GetCodeInfo("KMASTER", 0, strCode);
// 			if(strCodeName.GetLength()<1)	// ELW
// 				strCodeName		= m_pDataManager->GetCodeInfo("WMASTER", 0, strCode);			
// 			break;
// 		case CMasterTypeInfo::FUTOPT_CHART:
// 			strCodeName		= m_pDataManager->GetCodeInfo("FMASTER", 0, strCode);			
// 			if(strCodeName.GetLength()<1)
// 				strCodeName		= m_pDataManager->GetCodeInfo("PMASTER", 0, strCode);
// 			break;
// 		case CMasterTypeInfo::UPJONG_CHART:
// 			strCodeName		= m_pDataManager->GetCodeInfo("UMASTER", 0, strCode);			
// 			break;
		case CMasterTypeInfo::STOCK_CHART:
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
			strCodeName	= m_pDataManager->GetJongmokNameFromCode(strCode);
			break;
		case CMasterTypeInfo::FOREIGN_CHART:
			strCodeName	= m_pDataManager->GetCodeInfo("FRMASTER", 0, strCode);		
			{
				long lPos = strCodeName.Find('&');
				if(lPos>0)
					strCodeName.Insert(lPos,'&');
			}
				break;
		}

	return strCodeName;
*/
}

int CInputBarDlg::GetMarketTypeFromTRCode(LPCTSTR lpszTRCode)
{
	if(!::lstrcmp(lpszTRCode, QUERY_STRJONGMOK))
		return CMasterTypeInfo::STOCK_CHART;
	else if(!::lstrcmp(lpszTRCode, QUERY_STRFUTOPT))
		return CMasterTypeInfo::FUTOPT_CHART;
	else if(!::lstrcmp(lpszTRCode, QUERY_STRUPJONG))
		return CMasterTypeInfo::UPJONG_CHART;
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

void CInputBarDlg::SetEditZoomValue(CString strValue)
{
//	if(IsMiniBar())
//		return;

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
		strZoomValue = "120";
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetNew_OnePageDataCount(120);
	}
	
	m_editZoomValue.SetWindowText(strZoomValue);

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

void CInputBarDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == IDTIMER_SHARE_CODE_WAIT)
	{		
		if(m_pChartMng)
		{
			if( ((CChartMng*)m_pChartMng)->m_bNeedForceCode == FALSE )
			{

				KillTimer(IDTIMER_SHARE_CODE_WAIT);
				return;
			}

			CCodeDataFromInputBar CodeData;

			CString strCode;
			if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			{
//@��������
//				strCode = GetLastKey(1);
//				//strCode = m_pCtrlCodeInput->GetDataText();
//				if(strCode.GetLength()<1)
//				{
//					KillTimer(IDTIMER_SHARE_CODE_WAIT);
//					return;
//				}
//				//strCode = _T("003530");
//@��������
//@�����߰�
				strCode = GetLastHistoryCode(CMasterTypeInfo::STOCK_CHART);
//@�����߰�
			}
			else if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
			{
				// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
				if(m_strUpjongCode.IsEmpty())
					strCode = _T("001");
				else
					strCode = m_strUpjongCode;
			}
			else if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
			{
//@��������
//				strCode = GetLastKey(8);	//����
//				if(strCode.GetLength()<1)
//				{
//					strCode = GetLastKey(12);	//�ɼ�
//				}
//@��������
//@�����߰�
				strCode = GetLastHistoryCode(CMasterTypeInfo::FUTOPT_CHART);
//@�����߰�
				if(strCode.GetLength() < 1)
				{
					KillTimer(IDTIMER_SHARE_CODE_WAIT);
					return;
				}
			}
//@��������
//			else if(m_nMarketType == ELW_CHART)
//			{
////@��������
////				strCode = GetLastKey(3);
////@��������
////@�����߰�
//				strCode = GetLastHistoryCode(ELW_CHART);
////@�����߰�
//				//strCode = m_pCtrlCodeInput->GetDataText();
//				if(strCode.GetLength()<1)
//				{
//					KillTimer(IDTIMER_SHARE_CODE_WAIT);
//					return;
//				}
//			}
//			// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//			else if(m_nMarketType == COMMODITY_CHART)
//			{
//				strCode = GetLastKey(14);	
//				if(strCode.GetLength() < 1)
//				{
//					KillTimer(IDTIMER_SHARE_CODE_WAIT);
//					return;
//				}
//			}
//@��������
			else
			{
				KillTimer(IDTIMER_SHARE_CODE_WAIT);
				return;
			}

			CodeData.m_strCode = strCode;
			CodeData.m_lEndTime = GetEndDate();
			CodeData.m_chType = GetType();
			CodeData.m_lTypeNum = GetTypeNumber();
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			{					
				m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
				//STINPUTCODEDATA stInputCodeData;
				//stInputCodeData.m_strCode = strCode;
				//stInputCodeData.m_strTime = "";
				//m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
			}
#ifdef _DEBUG
			OutputDebugString("������� �� ���� ��� �ε� ��ȣ ����");
#endif
		}
	}
	else if(nIDEvent==IDTIMER_REQUESTTR)
	{
		KillTimer(nIDEvent);
		RequestTR();
	}
	else if(nIDEvent==IDTIMER_USERSTART)
	{
		KillTimer(nIDEvent);
		fnFluctTimerNextCodeSend();
	}
	else if(nIDEvent==IDTIMER_INITATER)
	{
		KillTimer(nIDEvent);
		fnInitAfter();
	}
    else if (nIDEvent == IDTIMER_REQSTEP_WAIT)
	{
		KillTimer(nIDEvent);
		RequestStepAll();
	}

	CRscDlg::OnTimer(nIDEvent);
}

//@��������
//void CInputBarDlg::OnCheckDow() 
//{
//	// TODO: Add your control notification handler code here
//	OnChkForeignMainCode(IDC_CHECK_DOW);
//}
//
//void CInputBarDlg::OnCheckNasdaq() 
//{
//	// TODO: Add your control notification handler code here
//	OnChkForeignMainCode(IDC_CHECK_NASDAQ);
//}
//
//void CInputBarDlg::OnCheckNikei() 
//{
//	// TODO: Add your control notification handler code here
//	OnChkForeignMainCode(IDC_CHECK_NIKEI);
//}
//
//void CInputBarDlg::OnCheckSp500() 
//{
//	// TODO: Add your control notification handler code here
//	OnChkForeignMainCode(IDC_CHECK_SP500);
//}
//
//void CInputBarDlg::OnCheckTwd() 
//{
//	// TODO: Add your control notification handler code here
//	OnChkForeignMainCode(IDC_CHECK_TWD);
//}
//@��������

void CInputBarDlg::OnSelchangeCblinetype() 
{
	int nSel = m_cbLineType.GetCurSel();
	m_pShareInputData->m_DataInputBar.m_nLineType = nSel;

	//ChartOCX���� ���¸� �˷��ش�.
	if(nSel==0)
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AddChartAllBlock("����Ʈ");
	else
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AddChartAllBlock("ĵ����Ʈ");
}

void CInputBarDlg::OnSelchangeCbmarket() 
{
	// TODO: Add your control notification handler code here
	if(!((CStdDialog*)GetParent())->GetStatusEnable()) return;
	int nSelMarketType = m_cbMarket.GetCurSel();

	// ���� �ڵ� ���� - ojtaso (20080115)
	if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
		m_strUpjongCode = GetCurrentCode();

	m_strCodeFromOutside = "";
	((CStdDialog*)GetParent())->ChangeStockMode(nSelMarketType); 

	((CChartMng*)m_pChartMng)->m_bNeedForceCode = TRUE;
	SetTimer(IDTIMER_SHARE_CODE_WAIT, 200, NULL);
}

//@��������
//{{2007.07.23 by LYH
//BOOL CInputBarDlg::SetRecentCode()
//{
//	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART || m_nMarketType == ELW_CHART)
//	{
//		m_strRecentCode = GetLastKey(1);
//	}
//	else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
//		m_strRecentCode = _T("COMP");
//	else if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
////@��������
////		m_strRecentCode = _T("20001");
//		m_strRecentCode = _T("001");
////@��������
//	else if( m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
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
//@��������
//	else if(m_nMarketType == ELW_CHART)
//		m_strCodeFromOutside = GetLastHistoryCode(ELW_CHART);
//@��������
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
//		if(m_nMarketType == CMasterTypeInfo::STOCK_CHART || m_nMarketType == ELW_CHART)
//		{
//			//::SendMessage(m_pCtrlCodeInput->GetSafeHwnd(), RMSG_FORMINITIALCOMPLETED, 0, 0);
//			//strCode = m_pCtrlCodeInput->GetDataText();
//			strCode = GetLastKey(1);
//		}
//		else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
//			strCode = _T("COMP");
//		else if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
//			strCode = _T("20001");
//		else if( m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
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
		CodeData.m_cChartShape = m_cChartShape;
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		{					
			m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
		}
#ifdef _DEBUG
		OutputDebugString("������� �� ���� ��� �ε� ��ȣ ����");
#endif
	}
}

//void CInputBarDlg::UpdateBtNCbValue()
//{
//	char szTemp[64];
//	if(DAY_DATA_CHART == m_chType)  // ��
//	{
//		sprintf(szTemp,"%d",m_lValueOfButton[0]);
//		m_chkFirst.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfButton[1]);
//		m_chkSecond.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfButton[2]);
//		m_chkThird.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfButton[3]);
//		m_chkFourth.SetBtnTitle(szTemp);
//	}
//	else if (HOUR_DATA_CHART == m_chType)	// ��
//	{
//		sprintf(szTemp,"%d",m_lValueOfHourButton[0]);
//		m_chkFirst.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfHourButton[1]);
//		m_chkSecond.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfHourButton[2]);
//		m_chkThird.SetBtnTitle(szTemp);
//	}
//	else if (MIN_DATA_CHART == m_chType)	// ��
//	{
//		sprintf(szTemp,"%d",m_lValueOfMinButton[0]);
//		m_chkFirst.SetBtnTitle(szTemp);
//		//m_chkFirst.SetBtnTitle("��", "��");
//		sprintf(szTemp,"%d",m_lValueOfMinButton[1]);
//		m_chkSecond.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfMinButton[2]);
//		m_chkThird.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfMinButton[3]);
//		m_chkFourth.SetBtnTitle(szTemp);
//	}
//	else if (TICK_DATA_CHART == m_chType)	// ƽ
//	{
//		sprintf(szTemp,"%d",m_lValueOfTickButton[0]);
//		m_chkFirst.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfTickButton[1]);
//		m_chkSecond.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfTickButton[2]);
//		m_chkThird.SetBtnTitle(szTemp);
//		sprintf(szTemp,"%d",m_lValueOfTickButton[3]);
//		m_chkFourth.SetBtnTitle(szTemp);
//	}
//}

void CInputBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}

void CInputBarDlg::OnBtsetenvir() 
{
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SHOWENVIRDLG);	
}

//void CInputBarDlg::OnBtcomparechart() 
//{
//	CString strDataPath = ((CStdDialog*)GetParent())->m_strRootPath + "\\data\\";
//	((CChartMng*)m_pChartMng)->m_strCodeForMulti = GetCurrentCode();
//	((CChartMng*)m_pChartMng)->LoadChart(strDataPath+"DayWeekMonth_ChartInfo.dat");
//}

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
//080520-001 CodeInput����	if(!m_bSelectAction)
//080520-001 CodeInput����		SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_ctrlCodeInput.GetDataText());
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
//void CInputBarDlg::OnBtSaveChart() 
//{
//	CRect rect;
//	CPoint pt;
//	GetDlgItem(IDC_BTSAVECHART)->GetWindowRect(rect);
//	pt.x = rect.left;
//	pt.y = rect.bottom;
//
//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//		m_pChartMng->SendMessage(UMSG_LOADSAVECHART,0,(LPARAM)&pt);	
//}

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
		return 400;
	else
		return retVal;
}

//BOOL CInputBarDlg::OnEraseBkgnd(CDC* pDC) 
//{
//	return TRUE;
//}
//

void CInputBarDlg::OnPaint() 
{
	CRscDlg::OnPaint();
}

//void CInputBarDlg::OnPaint() 
//{
//	CPaintDC dc(this); // device context for painting
//	CRect rcClient;
//	GetClientRect(&rcClient);
//
//	CDC memDC;
//	CBitmap bitmap;
//	memDC.CreateCompatibleDC(&dc);
//	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
//	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
//	memDC.FillSolidRect(rcClient, m_crBk);
//	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
//	memDC.SelectObject(pOldBitmap);
//	memDC.DeleteDC();
//	bitmap.DeleteObject();
//}

void CInputBarDlg::ResetUnitValue(BOOL bValueSet) 
{
	if(bValueSet)
	{
		if(m_nUnitPos >= 0)
		{
//			if (MIN_DATA_CHART == m_chType)		// 30��
//				m_nUnitValue = m_lValueOfMinButton[m_nUnitPos];
//			else if (TICK_DATA_CHART == m_chType)	// ƽ
//				m_nUnitValue = m_lValueOfTickButton[m_nUnitPos];
//			else if (HOUR_DATA_CHART == m_chType)	// ��
//				m_nUnitValue = m_lValueOfHourButton[m_nUnitPos];
//			else if (DAY_DATA_CHART == m_chType)
//				m_nUnitValue = m_lValueOfButton[m_nUnitPos];
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
//				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//				{
//					if(m_lValueOfMinButton[nPos] == m_nUnitValue)
//					{
//						m_nUnitPos = nPos;
//						bFind = TRUE;
//					}
//				}
			}
			else if (TICK_DATA_CHART == m_chType)	// ƽ
			{
//				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//				{
//					if(m_lValueOfTickButton[nPos] == m_nUnitValue)
//					{
//						m_nUnitPos = nPos;
//						bFind = TRUE;
//					}
//				}
			}
			else if (HOUR_DATA_CHART == m_chType)	// ��
			{
//				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//				{
//					if(m_lValueOfHourButton[nPos] == m_nUnitValue)
//					{
//						m_nUnitPos = nPos;
//						bFind = TRUE;
//					}
//				}
			}
			else if(DAY_DATA_CHART == m_chType)  // ��
			{
//				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//				{
//					if(m_lValueOfButton[nPos] == m_nUnitValue)
//					{
//						m_nUnitPos = nPos;
//						bFind = TRUE;
//					}
//				}
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
		case 100:
			return bRet;
//@��������
////		case 13:	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//		case 14:
//		case 15:
//		case 16:
////		case 17:
////		case 18:
////		case 19:
//		case 21:
//		case 61:
//		case 62:
//			nMarketType = COMMODITY_CHART;
//			break;
//@��������
		default:
			return bRet;
		}
	}
	else
	{
		int nRealMarketType;
		nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(strCode, nRealMarketType);
	}
	
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		bRet = ChangeMarket(nMarketType);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
//080520-001 CodeInput����			SetShareCodeChanged(TRUE);
//080520-001 CodeInput����			m_ctrlCodeInput.SetDataText(strCode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		bRet = ChangeMarket(nMarketType);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
//080520-001 CodeInput����			SetShareCodeChanged(TRUE);
//080520-001 CodeInput����			m_ctrlCodeInput.SetDataText(strCode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		bRet = ChangeMarket(nMarketType);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
//080520-001 CodeInput����			SetShareCodeChanged(TRUE);
//080520-001 CodeInput����			m_ctrlCodeInput.SetDataText(strCode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		bRet = ChangeMarket(nMarketType);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
//080520-001 CodeInput����			SetShareCodeChanged(TRUE);
//080520-001 CodeInput����			m_ctrlCodeInput.SetDataText(strCode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		bRet = ChangeMarket(nMarketType);
		if(bRet)
		{
			InitBaseInfo(NULL, FALSE);
//080520-001 CodeInput����			SetShareCodeChanged(TRUE);
//080520-001 CodeInput����			m_ctrlCodeInput.SetDataText(strCode);
		}
	}
	return FALSE;
}

//@��������
//// iKeyType�� �ش��ϴ� ����Ű�� �ֱٰ��� ��ȯ
//CString CInputBarDlg::GetLastKey(int iKeyType)
//{
//	CString strKey;
//	strKey.Empty();
//
//	if (iKeyType >= 0)
//	{
//		CWnd * pMainFrame = AfxGetMainWnd();
//
//		if (pMainFrame)
//		{
//			strKey = (LPCTSTR)pMainFrame->SendMessage(WMU_GET_LAST_KEY, (WPARAM)iKeyType, 0);
//		}
//	}
//
//	return strKey;
//}
//@��������
//@�����߰�
CString CInputBarDlg::GetLastHistoryCode(int nMarketType)
{
	CString strCode;
	if(m_pDataManager)
	{
		// �ֽ�
		if(nMarketType == CMasterTypeInfo::STOCK_CHART)
		{
			strCode = m_pDataManager->GetLastCode("01");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, CODE_STOCK_LENGTH);
			}
		}
		// ����
		else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
		{
			strCode = m_pDataManager->GetLastCode("02");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, UPJONG_LENGTH);
			}
		}
		// ����+�ɼ�+��������+�ֽĿɼ�
		else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
		{
			strCode = m_pDataManager->GetLastCode("03");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, FUTOPT_LENGTH);
			}
		}
//@��������
//		// ELW
//		else if(nMarketType == ELW_CHART)
//		{
//			strCode = m_pDataManager->GetLastCode("04");
//			if(strCode.GetLength() >= 11)
//			{
//				strCode = strCode.Mid(5, CODE_STOCK_LENGTH);
//			}
//		}
//@��������
	}

	return strCode;
}
//@�����߰�

void CInputBarDlg::OnEnSetfocusEditFocus()
{
//080520-001 CodeInput����	m_ctrlCodeInput.SetFocus();
}

//@��������	
//void CInputBarDlg::OnBnClickedBtchangeupjong()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
////@��������
////	CString strCode = "20001";
//	CString strCode = "001";
////@��������
//	if(m_nUpjongType != 100)
//	{
//		InitUpjongCodeCombo(100);
//	}
//	else
//	{
//		InitUpjongCodeCombo(101);
////@��������
////		strCode = "40001";
//		strCode = "4001";
////@��������
//	}
//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//	{					
//		STINPUTCODEDATA stInputCodeData;
//		stInputCodeData.m_strCode = strCode;
//		stInputCodeData.m_strTime = "";
//		m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
//	}
//}
//
//void CInputBarDlg::OnBtetcmenu() 
//{
//	// TODO: Add your control notification handler code here
//	// TODO: Add your control notification handler code here
//	CMenu menu;
//	CMenu *pMenuChild = NULL;
//
//	char* szEtcMenuName[] = {	"������ǥ", "�����ְ�", "��ǥ ��ü����"};
//	//	char* szEtcMenuName[] = {	"�����ְ�", 
//	//								"����� ��Ʈ����",
//	//								"�ð����� ����", 
//	//								"�����Ѱ� ǥ��", 
//	//								"����� ǥ��", 
//	//								"�Ǹ��� ǥ��"};
//
//	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//	menu.CreatePopupMenu();
//	UINT nFlags = NULL;
//
//	//menu.AppendMenu(MF_UNCHECKED,100,szEtcMenuName[0]);
//	pMenuChild = new CMenu;
//	pMenuChild->CreatePopupMenu();
//	menu.AppendMenu(MF_POPUP,(UINT)pMenuChild->GetSafeHmenu(),szEtcMenuName[0]);
//
//
//	CFavoriteThingsMng* pFavoriteMng = NULL;
//	BOOL bExistance;
//	int nFlagsChild;
//	int lPosAll = 0;
//	CStringArray strArrayIndicatorAll;	
//
//	pFavoriteMng = &((CStdDialog*)GetParent())->m_FavoriteMng;
//
//	stFvrData _stFvrData;
//	BOOL	bCreateRoot = FALSE;
//	if(pFavoriteMng)
//	{
//		pFavoriteMng->LoadItemInfo("JipyoFile");
//		int wSizeItem = pFavoriteMng->m_ArrayFvrItem.GetSize();
//		for(int wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
//		{
//			//_stData = pFavoriteMng.m_ArrayFvrItem.GetAt(wIndexItem);
//			//treeItem->InsertItem(_stData.m_chData1);
//			_stFvrData = pFavoriteMng->m_ArrayFvrItem.GetAt(wIndexItem);
//			//if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX&&_stFvrData.m_lSubData1==0)
//			if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX)
//			{
//				bCreateRoot = TRUE;
//				if(m_pChartMng->GetSafeHwnd())
//					bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)_stFvrData.m_chData1);
//				if(bExistance)
//				{
//					nFlagsChild = MF_CHECKED;
//				}
//				else
//				{
//					nFlagsChild = MF_UNCHECKED;
//				}
//				pMenuChild->AppendMenu(nFlagsChild,200+lPosAll,_stFvrData.m_chData1);
//				lPosAll++;
//				strArrayIndicatorAll.Add(_stFvrData.m_chData1);
//			}
//
//		}		
//	}
//
//	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
//	{
//		if(m_bChkRevised)
//		{
//			nFlags = MF_CHECKED;
//		}
//		else
//		{
//			nFlags = MF_UNCHECKED;
//		}
//		menu.AppendMenu(nFlags,103,szEtcMenuName[1]);
//	}
//
//	if(m_bChkSaveJipyoOneFile)
//	{
//		nFlags = MF_CHECKED;
//	}
//	else
//	{
//		nFlags = MF_UNCHECKED;
//	}
//	menu.AppendMenu(nFlags,106,szEtcMenuName[2]);
//
//	CRect rect;
//	m_btEtcMenu.GetWindowRect(rect);
//	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
//
//	pMenuChild = menu.GetSubMenu(0);
//	if(pMenuChild)
//	{
//		pMenuChild->DestroyMenu();
//		delete pMenuChild;
//	}
//	menu.DestroyMenu();	
//	if(nRetValue >= 200)
//	{
//		m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strArrayIndicatorAll.GetAt(nRetValue-200));
//		return;
//	}
//
//	switch(nRetValue)
//	{
//	case 103:
//		m_bChkRevised = !m_bChkRevised;
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_UPDATEREVISED,m_bChkRevised);
//		break;
//	case 106:
//		m_bChkSaveJipyoOneFile = !m_bChkSaveJipyoOneFile;
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_SAVEJIPYOONEFILE,m_bChkSaveJipyoOneFile);
//		break;
//	}	
//}
//@��������	

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
//	if(oldChType != pSelChart->m_cType)		UpdateBtNCbValue();

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
//			for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//			{
//				if(m_lValueOfTickButton[nPos]==lNumber)
//				{
//					m_nUnitPos = nPos;
//					bUpdateUnitPos = TRUE;
//				}
//			}
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
//				for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//				{
//					if(m_lValueOfMinButton[nPos]==lNumber)
//					{
//						m_nUnitPos = nPos;
//						bUpdateUnitPos = TRUE;
//					}
//				}
			}
		}
		else if(m_chType == DAY_DATA_CHART)//��(1)
		{					
			lNumber = atol(pSelChart->m_strTickMinDayNumber);
			m_nDay	= lNumber;
//			for(int nPos=0;nPos<CNTOFMAXBUTTONS;nPos++)
//			{
//				if(m_lValueOfButton[nPos]==lNumber)
//				{
//					m_nUnitPos = nPos;
//					bUpdateUnitPos = TRUE;
//				}
//			}
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
			if("300" == strCount)  // 30��
				strCount = "0";
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

//		for(int nPos=0;nPos < 3;nPos++)
//		{
//			if(m_lValueOfHourButton[nPos]==lNumber)
//			{
//				m_nUnitPos = nPos;
//				bUpdateUnitPos = TRUE;
//			}
//		}

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
	int nLength = strSelCode.GetLength();
	//CString strType = m_pDataManager->GetCodeInfo("", 2, strSelCode);
	//BYTE bType = m_pDataManager->GetMarketTypeFromCode(strSelCode);
	{
		if(nLength==FUTOPT_LENGTH)
		{
			BOOL bRet = ChangeMarket(CMasterTypeInfo::FUTOPT_CHART);
			if(bRet)
				InitBaseInfo(NULL, FALSE);
		}
		else if(nLength==UPJONG_LENGTH)
		{
//@��������
//			CString sTemp = strSelCode.Left(1);
//@��������
			BOOL bRet;
			//if(sTemp == 9)		// MS30
			//	bRet = ChangeMarket(3, TRUE, FALSE);
			//else
			bRet = ChangeMarket(CMasterTypeInfo::UPJONG_CHART);
			if(bRet)
				InitBaseInfo(NULL, FALSE);
		}
		else if(nLength==CODE_STOCK_LENGTH)
		{
			BOOL bRet = ChangeMarket(CMasterTypeInfo::STOCK_CHART);
			if(bRet)
				InitBaseInfo(NULL, FALSE);
		}	
	}

	m_strOldCode = GetCurrentCode();
	SetCurrentCode(strSelCode);	
	SetCurCodeNameToControl(GetCurrentCode());

	if(m_nMarketType != CMasterTypeInfo::FOREIGN_CHART)
	{
		STINPUTCODEDATA2 stInputData;
		stInputData.m_strCode = GetCurrentCode();
		stInputData.m_bDontPutInShared = FALSE;
		stInputData.m_bRequestWithoutHesitation = FALSE;
		SendMessage(UMSG_INPUTCODETOINPUTBAR, (WPARAM)&stInputData, 0);

		m_bSelectAction = TRUE;
		m_bSelectAction = FALSE;
	}
	else
	{
//-->080520-001 CodeInput����
//		CString strCode = m_ctrlCodeInput.GetDataText();
//		if(strCode.GetLength() == 0)
//		{
//			m_bSelectAction = TRUE;
//			SetShareCodeChanged(TRUE);
//			m_ctrlCodeInput.SetDataText(GetCurrentCode());
//			m_bSelectAction = FALSE;
//		}
//<--
	}

	m_bSettingInitData = FALSE;	
}

// ���������� Total Count�� ��Ƽ��Ʈ�� �˷��� : �������� - ojtaso (20070704)
void CInputBarDlg::SetMultiTotalCount(int nTotalCount)
{
	CString strCount;
	strCount.Format(_T("%d"), nTotalCount);

	SetDlgItemText(IDC_STC_TOTALCNT, strCount);
}

// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
void CInputBarDlg::EnableRequestMoreButton(BOOL bEnable)
{
	if(m_btRequestMore.GetSafeHwnd())
		m_btRequestMore.EnableWindow(bEnable);
}

void CInputBarDlg::SetCompareChartType()
{
//	SetCodeInit();
	
	// �������� ���Һ�(A)/�������� ��ø��(B)/��������� ���� ��ø��(C)/�Ⱓ�� ���ͷ� ��(D)/�ѱⰣ ������� ��(E)
	CString sKey = _T("");
	m_chkShowDivChart				.SetCheck(0);
	m_chkShowOverLappedChart		.SetCheck(0);
	m_chkShowNujukOverLappedChart	.SetCheck(0);

	switch(m_cChartShape)
	{			
		case DIVIDED_COMPARE_CHART:
			{		
				m_chkShowDivChart.SetCheck(1);
			}
			break;
		case OVERLAPP_COMPARE_CHART:
			{	
				m_chkShowOverLappedChart.SetCheck(1);
			}
			break;	
		case NUJUK_OVERLAPP_COMPARE_CHARTA:
		case NUJUK_OVERLAPP_COMPARE_CHARTB:
		case NUJUK_OVERLAPP_COMPARE_CHARTC:
			{
				m_chkShowNujukOverLappedChart.SetCheck(1);
			}
			break;
//		case SUIK_COMPARE_CHART:
//			{
//				m_chkShowSuikChart.SetCheck(1);
//			}
//			break;
//		case TOT_NUJUK_COMPARE_CHART:
//			{
//				m_chkShowTotNujukChart.SetCheck(1);
//			}
//			break;	
	}	
	m_chkShowDivChart				.Invalidate();
	m_chkShowOverLappedChart		.Invalidate();
	m_chkShowNujukOverLappedChart	.Invalidate();
//	m_chkShowTotNujukChart			.Invalidate();
//	m_chkShowLeftBar				.Invalidate();
//	m_chkShowSuikChart				.Invalidate();

//	//OCX�� ��Ʈ�� ���¸� �ٲٵ��� ��û�Ѵ�.
//	if(m_pChartMng->GetSafeHwnd())
//		m_pChartMng->SendMessage(UMSG_CHANGE_COMPARECHART_TYPE, (WPARAM)&m_cChartShape, 0);
}

void CInputBarDlg::SetPeriodType(BOOL bSendChartMng)
{	
return;
//	EnableWindow(FALSE);
//	// ��(1)/��(2)/��(3)/��(4)
//	CString sKey = _T("");
//	switch(m_chType)
//	{			
//		case MIN_DATA_CHART:
//		case DAY_DATA_CHART:
//			{				
//				m_chkWeek.SetCheck(0);
//				m_chkMonth.SetCheck(0);	
//				Enable_PeriodControl(TRUE);
//				switch(m_chType)
//				{				
//					case MIN_DATA_CHART:							
//						m_chkMin.SetCheck(1);				
//						m_chkDay.SetCheck(0);					
//						break;
//					case DAY_DATA_CHART:						
//						m_chkMin.SetCheck(0);									
//						m_chkDay.SetCheck(1);					
//						break;
//				}	
//			}
//			break;	
//		case WEEK_DATA_CHART:
//			{
//				m_chkMin.SetCheck(0);
//				m_chkDay.SetCheck(0);
//				m_chkWeek.SetCheck(1);
//				m_chkMonth.SetCheck(0);							
//				Enable_PeriodControl(FALSE);	
//			}
//			break;
//		case MONTH_DATA_CHART:
//			{
//				m_chkMin.SetCheck(0);			
//				m_chkDay.SetCheck(0);
//				m_chkWeek.SetCheck(0);
//				m_chkMonth.SetCheck(1);				
//				Enable_PeriodControl(FALSE);	
//			}
//			break;
//	}	
//
//	ChangeStatusDateControl();
//
//	m_chkMin.Invalidate();
//	m_chkDay.Invalidate();
//	m_chkWeek.Invalidate();
//	m_chkMonth.Invalidate();	
//
//	if(m_chType  < WEEK_DATA_CHART)
//	{
//		if(m_pChartMng->GetSafeHwnd())
//		{
//			UpdateData(TRUE);
//
//			CString strValue;				
//			m_edtTime.GetWindowText(strValue); strValue.Replace(" ", "");			
//			int nValue = atoi(strValue);
//			if(nValue <= 0) 
//			{
//				strValue = _T("1");
//				m_edtTime.SetWindowText(strValue);
//			}			
//			
//			m_strPeriod = strValue;
//
//			//if(bSendChartMng == FALSE)
//				if(m_pChartMng->GetSafeHwnd()) 
//					m_pChartMng->SendMessage(	��_NO_REQUEST	,
//												m_chType	, // ƽ, ��, ��,....���� 
//												nValue			);//����� ���� �Ⱓ
//			
//			//m_pChartMng->SendMessage(bSendChartMng ? UMSG_SETTYPE : UMSG_SETTYPE_NO_REQUEST	,
//			//						m_chType	, // ƽ, ��, ��,....���� 
//			//						nValue			);//����� ���� �Ⱓ
//		}
//	}
//	else // �� / �� / 
//	{
//		
//		//if(bSendChartMng == FALSE)
//			if(m_pChartMng->GetSafeHwnd()) 
//				m_pChartMng->SendMessage(UMSG_SETTYPE_NO_REQUEST,m_chType, 1);
//				
//		//if(m_pChartMng->GetSafeHwnd()) 
//		//	m_pChartMng->SendMessage(bSendChartMng ? UMSG_SETTYPE : UMSG_SETTYPE_NO_REQUEST,m_chType, 1);
//	}			
//
//	if(bSendChartMng)
//	{
//		GetParent()->SendMessage(UMSG_RE_REQUEST_TRDATA, 0, 0);
//	}
//
//	EnableWindow();
}

// nKey		0 :
//BOOL CInputBarDlg::GetInputCode(int nKey, long &dwIn, CString& szIn)
//{
//	switch(nKey) {
//	case 0:	//��/��/��/��
//		szIn = m_chType;
//		break;
//	case 1:	// ��к�
//		m_editPeriod.GetWindowText(szIn);
//		dwIn = atol(szIn);
//		break;
//	case 2: // ��ȸī��Ʈ
//		m_editCntOfDatAll.GetWindowText(szIn);
//		dwIn = atol(szIn);
//		if(dwIn==0) dwIn = 400;
//		if(dwIn<15) dwIn = 15;
//		break;
//	case 3: // ������ȸ����
//		dwIn = IsDlgButtonChecked(IDC_CHK_QUERYOP);
//		break;
//	case 4: //��ȸ����
//		break;
//	case 5: // EndData
//		dwIn = GetEndDate();
//		break;
//	default:
//		// �����ְ���뿩�δ� m_pChartItem->GetUseRevisedValue()���� ��´�.
//		return FALSE;
//	}
//
//	return TRUE;
//}

//-->@080522-001 ��/������.
void CInputBarDlg::ConfirmLineType()
{
	OnSelchangeCblinetype();
}
//<--

//-->@080522-003 TestCode
void CInputBarDlg::Test001()
{
	CCodeDataFromInputBar CodeData;
	CodeData.m_strCode = GetCurrentCode();
	CodeData.m_lEndTime = GetEndDate();
	CodeData.m_chType = GetType();
	CodeData.m_lTypeNum = GetTypeNumber();
	CodeData.m_cChartShape = m_cChartShape;

	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
	{
		m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
	}
}
//<--

void CInputBarDlg::Test002()
{
//	m_pChartMng->GetParent()->SendMessage(UMSG_RE_REQUEST_TRDATA, 0, 0);
//	return;
//-->
	CChartMng* pChtMng = (CChartMng*)m_pChartMng;

	pChtMng->SaveChart(((CStdDialog*)GetParent())->m_strConfig);
	CString strAppName = "0000"; // ����Ʈ�� row,col �Ѱ��Ƿ�.
	pChtMng->GetpChartItemSelected()->SaveRQInfo(strAppName, ((CStdDialog*)pChtMng->GetParent())->m_strConfig, m_chType);

	CString szType(m_chType);
	::WritePrivateProfileString(strAppName,"Type", szType, ((CStdDialog*)pChtMng->GetParent())->m_strConfig);
	pChtMng->LoadChart(((CStdDialog*)GetParent())->m_strConfig);
//<--
}

void CInputBarDlg::SetChTypeBtn(char chType)
{
	m_chkMinute	.SetCheck(0);
	m_chkDay	.SetCheck(0);
	m_chkWeek	.SetCheck(0);
	m_chkMonth	.SetCheck(0);
	m_chkTick	.SetCheck(0);
	m_chkSec	.SetCheck(0);

	// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5) -- �� �κ��� ��ȭ���� �ű� �߰��� �׸�
	switch(chType)
	{
		case MIN_DATA_CHART:	m_chkMinute.SetCheck(1);	break;
		case DAY_DATA_CHART:	m_chkDay.SetCheck(1);		break;	
		case WEEK_DATA_CHART:	m_chkWeek.SetCheck(1);		break;
		case MONTH_DATA_CHART:	m_chkMonth.SetCheck(1);		break;
		case TICK_DATA_CHART:	m_chkTick.SetCheck(1);		break;
		case SEC_DATA_CHART:	m_chkSec.SetCheck(1);		break;
	}

	if (chType == MIN_DATA_CHART)
	{
		m_editPeriod.EnableWindow(TRUE);
		m_spinPeriod.EnableWindow(TRUE);
	}
	else
	{
		m_editPeriod.EnableWindow(FALSE);
		m_spinPeriod.EnableWindow(FALSE);
	}

	m_chkDay.Invalidate();
	m_chkWeek.Invalidate();
	m_chkMonth.Invalidate();
	m_chkMinute.Invalidate();
	m_chkSec.Invalidate();
	m_chkTick.Invalidate();
}

void CInputBarDlg::Test004(char chType, int nTypeUnit)
{
	if(m_pChartMng && m_pChartMng->GetSafeHwnd())
	{
		((CChartMng*)m_pChartMng)->GetpExtInputBarDlg()->SendMessage(UMSG_SETTYPE, (WPARAM)chType, 0);
		m_pChartMng->SendMessage(UMSG_SETTYPE_NO_REQUEST, chType, nTypeUnit);
	}

//	// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5) -- �� �κ��� ��ȭ���� �ű� �߰��� �׸�
//	CString sKey = _T("");
//	switch(m_chType)
//	{
//		case MIN_DATA_CHART:	
//			m_editPeriod.EnableWindow(TRUE);
//			Enable_PeriodControl(TRUE);
//			break;
//		case DAY_DATA_CHART:							
//			m_editPeriod.EnableWindow(TRUE);
//			Enable_PeriodControl(TRUE);
//			break;	
//		case WEEK_DATA_CHART:
//			m_editPeriod.EnableWindow(FALSE);
//			Enable_PeriodControl(FALSE);
//			break;
//		case MONTH_DATA_CHART:
//			m_editPeriod.EnableWindow(FALSE);
//			Enable_PeriodControl(FALSE);
//			break;
//		case TICK_DATA_CHART:	
//			m_editPeriod.EnableWindow(TRUE);
//			Enable_PeriodControl(TRUE);
//			break;
//		case SEC_DATA_CHART:	
//			m_editPeriod.EnableWindow(TRUE);
//			Enable_PeriodControl(TRUE);
//			break;
//	}
//
//	if(m_chType < WEEK_DATA_CHART || m_chType == SEC_DATA_CHART)
//	{
//		if(m_pChartMng->GetSafeHwnd())
//		{
//			CString strValue;				
//			m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
//			int nValue = atoi(strValue);
//			if(nValue <= 0)
//			{
//				strValue = _T("1");
//				m_editPeriod.SetWindowText(strValue);
//			}
//
//			m_pChartMng->SendMessage(UMSG_SETTYPE_NO_REQUEST,
//									m_chType, // ƽ, ��, ��,....���� 
//									nValue);//����� ���� �Ⱓ
//		}
//	}
//	else if (HOUR_DATA_CHART == m_chType)		// ��
//	{
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//		{
//			CString strValue;				
//			m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
//			int nValue = atoi(strValue);
//			if(nValue <= 0) 
//			{
//				strValue = _T("1");
//				m_editPeriod.SetWindowText(strValue);
//				nValue = 1;
//			}
//
//			m_pChartMng->SendMessage(UMSG_SETTYPE_NO_REQUEST, m_chType, nValue);//����� ���� �Ⱓ
//		}
//	}
//	else // �� / �� / ��
//	{
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_SETTYPE_NO_REQUEST,m_chType);
//	}
}

void CInputBarDlg::OnChkQueryOp()
{
	m_pShareInputData->m_DataInputBar.m_bTodayRequest = IsDlgButtonChecked(IDC_CHK_QUERYOP);
	OnBtnRetry();
}

void CInputBarDlg::SetShareInputData()
{
	CString szTmp;

	m_pShareInputData->m_DataInputBar.m_chType = GetType();//m_chType;

	//m_editPeriod.GetWindowText(szTmp);
	//m_pShareInputData->m_DataInputBar.m_lTypeNum = atol(szTmp);
	m_pShareInputData->m_DataInputBar.m_lTypeNum = GetTypeNumber();

	m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
	switch(m_cChartShape)
	{
		//m_nUnitType
		case DIVIDED_COMPARE_CHART:
		case OVERLAPP_COMPARE_CHART:
			m_pShareInputData->m_DataInputBar.m_cDataGubun = '0';
			m_pShareInputData->m_DataInputBar.m_nUnitType = WON_TYPE;
			m_pShareInputData->m_nBase = 0;
			break;
		case NUJUK_OVERLAPP_COMPARE_CHARTA:
			if(m_nScreenType==CStdDialog::Screen_fluctuation)
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '5';
			else
			{
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '1';
				m_pShareInputData->m_nBase = 11;
			}
			m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;
			break;
		case NUJUK_OVERLAPP_COMPARE_CHARTB:
			m_pShareInputData->m_DataInputBar.m_cDataGubun = '3';
			m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;
			break;
		case NUJUK_OVERLAPP_COMPARE_CHARTC:
			m_pShareInputData->m_DataInputBar.m_cDataGubun = '4';
			m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;
			break;
		default:
			m_pShareInputData->m_nBase = 0;
			break;
	}

	m_editZoomValue.GetWindowText(szTmp);
	m_pShareInputData->m_DataInputBar.m_lDispCnt = atol(szTmp);
	m_nFixZoomValue = atol(szTmp);
	
	m_editCntOfDatAll.GetWindowText(szTmp);
	m_pShareInputData->m_DataInputBar.m_lRequestCnt = atol(szTmp);
	
	if(m_pShareInputData->m_DataInputBar.m_lRequestCnt<5)
		m_pShareInputData->m_DataInputBar.m_lRequestCnt=400;

	// ������ȸ ������ ����ȸ �ǵ��� ó���ʿ���.
	// ���϶��� ���õȴ�. �ƴϸ� 0;
	m_pShareInputData->m_DataInputBar.m_bTodayRequest = IsDlgButtonChecked(IDC_CHK_QUERYOP);
	if(m_chType!=MIN_DATA_CHART)
		m_pShareInputData->m_DataInputBar.m_bTodayRequest = 0;
//	if(GetDlgItem(IDC_CHK_QUERYOP)->IsWindowVisible()==FALSE)
//		m_pShareInputData->m_DataInputBar.m_bTodayRequest = 0;

	m_pShareInputData->m_DataInputBar.m_lEndTime = GetEndDate();

	//@.080730-001 ����Ʈ ������϶� 11
	//if(m_nScreenType!=CStdDialog::Screen_fluctuation)	//�������Ʈ�ƴϸ�
	//	m_pShareInputData->m_nBase = 0;	//������������ �����
}


void CInputBarDlg::SetMenuStyle()
{
}

void CInputBarDlg::RequestTR()
{
//	switch(m_chType)
//	{
//		case MIN_DATA_CHART:
//			PostMessage(IDC_CHKMINUTE);break;
//		case DAY_DATA_CHART:
//			PostMessage(IDC_CHKDAY);break;
//		case WEEK_DATA_CHART:
//			PostMessage(IDC_CHKWEEK);break;
//		case MONTH_DATA_CHART:
//			PostMessage(IDC_CHKMONTH);break;
//	}
	int nCnt = m_szRCodeList.GetCount();
	if(nCnt>0)
	{
		SendTR(m_szRCodeList.RemoveHead());
	}
	else
	{
		m_pShareInputData->m_bCanDel = TRUE;
	}
	//m_bFirstSend = FALSE;
}

// ��ȸ�� ���񸮽�Ʈ�� ���Ѵ�.
void CInputBarDlg::GetRequestList()
{
	m_szRCodeList.RemoveAll();

	if(m_nScreenType==CStdDialog::Screen_fluctuation)	//�������Ʈ�϶�
	{
		//int nCnt = m_szTimerCodeList.GetCount();

		for(POSITION pos=m_szTimerCodeList.GetHeadPosition(); pos;)
		{
			m_szRCodeList.AddTail(m_szTimerCodeList.GetNext(pos));
		}
	}
	else //����Ʈ�϶�
	{
		CStdDialog* pStdDialog = (CStdDialog*)GetParent();
		int nCnt = pStdDialog->GetRequestList(&m_szRCodeList, &m_szRCodeNameList);
	}
	
}

void CInputBarDlg::CurrentTrError()
{
	// ��ȸ�����ÿ� ���ʹٿ��� ����Ʈ����.
	((CStdDialog*)GetParent())->DeleteToLeftBar(m_szCurTimerCode);
}

void CInputBarDlg::SendNextTR()
{
	if(m_nScreenType==CStdDialog::Screen_fluctuation)	//�������Ʈ�϶�
	{
		CStdDialog* pSTDialog = (CStdDialog*)GetParent();

		if(pSTDialog->m_stDlgCompData.m_bShowAll==1)
		{
			SetTimer(IDTIMER_REQUESTTR, 1, NULL);
		}
		else
		{
			//������������ Ȯ��.
			if(pSTDialog->m_stDlgCompData.m_bTimerUse)
			{
				int nTime = pSTDialog->m_stDlgCompData.m_nTimer * 1000;
				SetTimer(IDTIMER_USERSTART, nTime, NULL);
			}
		}
	}
	else
	{
		SetTimer(IDTIMER_REQUESTTR, 1, NULL);
	}	
		
	//RequestTR();
}

// �����ڵ�� ��ȸ�Ѵ�.
void CInputBarDlg::SendTR(LPCSTR szCode)
{
//	m_bFirstSend = FALSE;
//	if(m_bFirstSend)
//	{
//		CCodeDataFromInputBar CodeData;
//		CodeData.m_strCode	= szCode;
//		CodeData.m_lEndTime = GetEndDate();
//		CodeData.m_chType	= m_pShareInputData->m_DataInputBar.m_chType;
//		CodeData.m_lTypeNum = m_pShareInputData->m_DataInputBar.m_lTypeNum;
//		CodeData.m_cChartShape = m_pShareInputData->m_DataInputBar.m_cChartShape;
//		CodeData.m_nUnitType = m_pShareInputData->m_DataInputBar.m_nUnitType;
//
//		if(m_pChartMng->GetSafeHwnd())
//		{
//			m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
//		}
//		return;
//	}
//	else
	{
		fnSetUserSetting(szCode);
		((CStdDialog*)GetParent())->SendToChartMngCode(szCode);
		return;
	}
}

void CInputBarDlg::RequestStepAll()
{
//--> ��ȸ��ƾ
	GetRequestList();
	//if(m_szRCodeList.GetCount()>0) m_bFirstSend = TRUE;
	m_pShareInputData->m_bCanDel = FALSE;
	//m_pChartMng->SendMessage(UMSG_RESETOCX);
	//((CChartMng*)m_pChartMng)->GetpChartItemSelected()->ResetChart();
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	if(pSTDialog) pSTDialog->AllCodeUnAdvise();

	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->ResetOCX(TRUE);
	//RequestTR();
	SendNextTR();
//<--
}

void CInputBarDlg::RequestStep2All()
{
	GetRequestList();
	//if(m_szRCodeList.GetCount()>0) m_bFirstSend = TRUE;
	m_pShareInputData->m_bCanDel = FALSE;
	//m_pChartMng->SendMessage(UMSG_RESETOCX);
	//((CChartMng*)m_pChartMng)->GetpChartItemSelected()->ResetChart();
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	if(pSTDialog) pSTDialog->AllCodeUnAdvise();
	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->ResetOCX(TRUE);

	//OCX�� ��Ʈ�� ���¸� �ٲٵ��� ��û�Ѵ�.
//	if(m_pChartMng->GetSafeHwnd())
//		m_pChartMng->SendMessage(UMSG_CHANGE_COMPARECHART_TYPE, (WPARAM)&m_cChartShape, 0);
	//RequestTR();
	SendNextTR();
}

// ���������â���� ���� ������ ���� ��� ȣ���
void CInputBarDlg::ConfirmDlgSetupData()
{
	CString szTmp;
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();

	m_pShareInputData->m_nBase = pSTDialog->m_stDlgCompData.m_nBase;
	{
		m_nCurrentMode = OVERLAPP_CHART;
		switch(m_pShareInputData->m_nBase)
		{
			case 0:
			default:
				m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTA;
				//@.080723������������. 1->5 �� ������.
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '5';
				break;
			case 1:
				m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTB;
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '3';
				break;
			case 2:
				m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTC;
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '4';
				break;
		}

		m_pShareInputData->m_DataInputBar.m_cChartShape = m_cChartShape;
		m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;
	}

	//pSTDialog->m_stDlgCompData.m_nTimer;
	//pSTDialog->m_stDlgCompData.m_bCurDisp;

	szTmp.Format("%d", pSTDialog->m_stDlgCompData.m_nTypeUnit);
	m_editPeriod.SetWindowText(szTmp);
	m_pShareInputData->m_DataInputBar.m_lTypeNum = pSTDialog->m_stDlgCompData.m_nTypeUnit;

	// �Ʒ��κ��� �׳� Confirm�������� ... ��� ��.
	m_pShareInputData->m_DataInputBar.m_chType = MIN_DATA_CHART;
	m_chType = MIN_DATA_CHART;
	SetShareInputData();

	// ���� ��ȸ�� ������ ���� ���� Clear, ���� ���� �״��.
	if(pSTDialog->m_stDlgCompData.m_szSelCode.GetLength()>0 &&
		IsCodeExist(pSTDialog->m_stDlgCompData.m_szSelCode)==FALSE)
		pSTDialog->m_stDlgCompData.m_szSelCode.Empty(); //Clear

//-->080708-001ȭ ���������� �ɼ��� ����� �����߰� ���·� �Ǵ� ��찡 �־ �߰���.
// �ֱ�ȭ ��Ű�� �ٽ� ��ȸ�Ѵ�.
	m_bOldShowAll = pSTDialog->m_stDlgCompData.m_bShowAll;
	if(pSTDialog) pSTDialog->AllCodeUnAdvise();
	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->ResetOCX(TRUE);
//<--080708-001ȭ 

	fnInitAfter();
}

//�������Ʈ�� �� OninitDialog���� Ÿ�̸� ȣ�⿡ ���ؼ� �Ҹ���.
void CInputBarDlg::fnInitAfter()
{
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();

	// 01. ������ �׷���� ������ ���Ѵ�.
	CString szSelectGroup = pSTDialog->m_stDlgCompData.m_szSelectGroup;

	fnReLoadstDlgCompData();
//	// m_pSelGroupData	
//	for(POSITION pos=pSTDialog->m_stDlgCompData.m_GroupList.GetHeadPosition(); pos;)
//	{
//		CDlgCompGroupData* pGroupData = pSTDialog->m_stDlgCompData.m_GroupList.GetNext(pos);
//		if(pGroupData->m_szGroupName.Compare(szSelectGroup)==0)
//		{
//			m_pSelGroupData = pGroupData;
//			break;
//		}
//	}
//	if(m_pSelGroupData==NULL) return;	// �ƹ��׷쵵 ������ ó���� �� ����.
//
//	CDlgCompGroupData* pSelGroupData = m_pSelGroupData;
//	m_szTimerCodeList.RemoveAll();
//
//	for(POSITION jPos=pSelGroupData->m_ItemList.GetHeadPosition(); jPos;)
//	{
//		CDlgCompSubItem* pItem = pSelGroupData->m_ItemList.GetNext(jPos);
//		m_szTimerCodeList.AddTail(pItem->m_szCode);
//	}

	if(m_pSelGroupData==NULL) return;	// �ƹ��׷쵵 ������ ó���� �� ����.

	// 02. ���� ������ ���ʷ� �������� ó���� �Ѵ�.
	// ���������̸� �������� ���� �����ϰ�.
	// �������Ⱑ �ƴϸ� ��ü��ȸ ó���� �Ѵ�.
	if(pSTDialog->m_stDlgCompData.m_bTimerUse)
	{
		pSTDialog->m_stDlgCompData.m_bShowAll = 0;
		m_bOldShowAll = 0;
		if(m_szTimerCodeList.GetCount()>0)
		{
			m_cmbChartViewType.SetCurSel(0);	// ���񺯰���� ��ȯ.
			m_szCurTimerCode = m_szTimerCodeList.GetHead();	
			pSTDialog->m_stDlgCompData.m_szSelCode = m_szCurTimerCode;
			fnFluctTimerNextCodeSend(FALSE);
		}
	}
	else
	{
		if(pSTDialog->m_stDlgCompData.m_bShowAll==1)	//��ü����.
		{
			//��ȸ�ڵ� ����Ʈ�� �����.
			//GetRequestList();

			m_cmbChartViewType.SetCurSel(3);
			OnBtnRetry();
		}
		else	//��������
		{
			if(pSTDialog->m_stDlgCompData.m_szSelCode.IsEmpty())
				pSTDialog->m_stDlgCompData.m_szSelCode = GetFirstGroupSelCode();

			m_szCurTimerCode = pSTDialog->m_stDlgCompData.m_szSelCode;
			if(m_szCurTimerCode.GetLength()>0)
				SendTR(m_szCurTimerCode);
		}

	}
}

// �������� ����/����
void CInputBarDlg::fnFluctTimerStart(BOOL bStart)
{
	if(bStart)
	{
		fnFluctTimerNextCodeSend(FALSE);
	}
	//pSTDialog->m_stDlgCompData.m_bTimerUse	
}

// �������⿡�� �����ڵ� ����������ȸ.
void CInputBarDlg::fnFluctTimerNextCode(CString &szCode, CString &szCodeName)
{
	CString szCurCode = m_szCurTimerCode;
	if(m_szTimerCodeList.GetCount()==0)
	{
		szCode.Empty();
		return;
	}

	if(szCurCode.IsEmpty())
	{
		szCode = m_szTimerCodeList.GetHead();
		return;
	}

	CString szTmp;
	for(POSITION pos=m_szTimerCodeList.GetHeadPosition(); pos;)
	{
		szTmp = m_szTimerCodeList.GetNext(pos);
		if(szCurCode.Compare(szTmp)==0)
		{
			if(pos==NULL)
			{
				szCode = m_szTimerCodeList.GetHead();
				return;
			}
			else
			{
				szCode = m_szTimerCodeList.GetNext(pos);
				return;
			}
		}
	}

	szCode = m_szTimerCodeList.GetHead();
}

// �������⿡�� ������ȸ�ϴ� ���� �ڵ带 ��ȸ�Ѵ�.
// FALSE�̸� ���� m_szCurTimerCode �ڵ�� ��ȸ�� �����Ѵ�.
void CInputBarDlg::fnFluctTimerNextCodeSend(BOOL bNextCheck)
{
	CString szCode, szCodeName;
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	
	if(!bNextCheck)
	{
		BOOL bRQSet = FALSE;
		if(((CChartMng*)m_pChartMng)->GetRQCount()>1) bRQSet = TRUE;
		if(bRQSet || m_bOldShowAll!=pSTDialog->m_stDlgCompData.m_bShowAll)
		{
			m_bOldShowAll = pSTDialog->m_stDlgCompData.m_bShowAll;
			if(pSTDialog) pSTDialog->AllCodeUnAdvise();
			((CChartMng*)m_pChartMng)->GetpChartItemSelected()->ResetOCX(TRUE);
		}

		if(m_szCurTimerCode.GetLength()==0)
			fnFluctTimerNextCode(szCode, szCodeName);
		else
			szCode = m_szCurTimerCode;

		if(szCode.GetLength()>0)
		{
			m_szCurTimerCode = szCode;
			pSTDialog->m_stDlgCompData.m_szSelCode = m_szCurTimerCode;

			fnSetUserSetting(m_szCurTimerCode);
			SendTR(m_szCurTimerCode);
		}
		return;
	}

	//CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	//pSTDialog->m_stDlgCompData.m_szSelCode

	fnFluctTimerNextCode(szCode, szCodeName);
	if(szCode.GetLength()>0)
	{
		// ����Ʈ�� �ϳ��϶��� ��� ���� �ʵ��� �����.
		if(bNextCheck && m_szCurTimerCode.Compare(szCode)==0)
		{
			// Ÿ�̸Ӵ� ��� �۵��ϰ� �Ѵ�.
			// ����Ʈ�� �ϳ��϶� ���ߴ� ������ ���� �� ��������.
			int nTime = pSTDialog->m_stDlgCompData.m_nTimer * 1000;
			SetTimer(IDTIMER_USERSTART, nTime, NULL);
			return;
		}

		SendTR(szCode);
		m_szCurTimerCode = szCode;
		pSTDialog->m_stDlgCompData.m_szSelCode = m_szCurTimerCode;
	}
}

void CInputBarDlg::fnReLoadstDlgCompData()
{
	CStdDialog* pSTDialog = (CStdDialog*)GetParent();
	CString szSelectGroup = pSTDialog->m_stDlgCompData.m_szSelectGroup;
	CString szTmp;

	CDlgCompGroupData* pSelGroupData = NULL;
	for(POSITION pos=pSTDialog->m_stDlgCompData.m_GroupList.GetHeadPosition(); pos;)
	{
		CDlgCompGroupData* pGroupData = pSTDialog->m_stDlgCompData.m_GroupList.GetNext(pos);
		if(pGroupData->m_szGroupName.Compare(szSelectGroup)==0)
		{
			pSelGroupData = pGroupData;
			m_pSelGroupData = pGroupData;
			break;
		}
	}
	if(pSelGroupData)
	{
		m_szTimerCodeList.RemoveAll();
		for(POSITION jPos=pSelGroupData->m_ItemList.GetHeadPosition(); jPos;)
		{
			CDlgCompSubItem* pItem = pSelGroupData->m_ItemList.GetNext(jPos);
			m_szTimerCodeList.AddTail(pItem->m_szCode);
		}
	}
}

void CInputBarDlg::fnSetUserSetting(LPCSTR szCode)
{
	if(m_nScreenType==CStdDialog::Screen_fluctuation)	//�������Ʈ�϶�
	{
		for(POSITION pos=m_pSelGroupData->m_ItemList.GetHeadPosition(); pos; )
		{
			CDlgCompSubItem* pSubItem = m_pSelGroupData->m_ItemList.GetNext(pos);
			if(pSubItem && pSubItem->m_szCode.CompareNoCase(szCode)==0)
			{
				m_pSubItem = pSubItem;
				return;
			}
		}
	}
}

LPCSTR CInputBarDlg::GetFirstGroupSelCode()
{
	for(POSITION pos=m_pSelGroupData->m_ItemList.GetHeadPosition(); pos; )
	{
		CDlgCompSubItem* pSubItem = m_pSelGroupData->m_ItemList.GetNext(pos);
		if(pSubItem)
		{
			return (LPCSTR)pSubItem->m_szCode;
		}
	}
	return "";
}

// ������ ���� ���õ� �׷쿡 �����ִ��� Ȯ���Ѵ�.
BOOL CInputBarDlg::IsCodeExist(LPCSTR szCode)
{
	if(!m_pSelGroupData) return FALSE;

	if(m_pSelGroupData->m_ItemList.GetCount()==0) return FALSE;

	for(POSITION pos=m_pSelGroupData->m_ItemList.GetHeadPosition(); pos; )
	{
		CDlgCompSubItem* pSubItem = m_pSelGroupData->m_ItemList.GetNext(pos);
		if(pSubItem)
		{
			if(pSubItem->m_szCode.CompareNoCase(szCode)==0)
				return TRUE;
		}
	}
	return FALSE;
}

void CInputBarDlg::ChangeSkinColor(COLORREF clrBackground)
{
	m_crBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	//m_crBk = clrBackground;
	m_brBkColor.DeleteObject();
	m_brBkColor.CreateSolidBrush(m_crBk);
	Invalidate();
}