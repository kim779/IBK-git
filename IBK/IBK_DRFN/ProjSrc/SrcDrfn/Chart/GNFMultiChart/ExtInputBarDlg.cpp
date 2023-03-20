// ExtInputBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExtInputBarDlg.h"
#include "StdDialog.h"
#include "./define/ChartMsgDef.h"
#include "../chart_common/BlockBaseEnum.h"
#include "../../inc/IAUGwanMng.h"
#include "../../../SrcSite/include/define.h"
#include "../../../SrcSite/include/axisfire.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtInputBarDlg dialog
//extern CWnd *g_pViewInMain;

const UINT RMSG_GETOPENEDOWNER = ::RegisterWindowMessage(_T("RMSG_GETOPENEDOWNER"));
const UINT RMSG_PRINTSCREEN = ::RegisterWindowMessage(_T("RMSG_PRINTSCREEN"));
//<< 20091214 by kmlee for account combo
const UINT RMSG_WRAP_GETINFO = ::RegisterWindowMessage(_T("RMSG_WRAP_GETINFO"));
//>>>

#define UWM_RECEIVE_ITRQUERY   ( WM_USER+826 )

CExtInputBarDlg::CExtInputBarDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CExtInputBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtInputBarDlg)	
	m_ctlTime = 0;
	//}}AFX_DATA_INIT
	m_lRows = 1;
	m_lCols = 1;
	m_pChartMng = NULL;
	m_pLeftBar = NULL;
//	m_bRevised = TRUE;
	m_nSave = SAVE_TRUE;
//	m_bTimeShareUseage = TRUE;
//	m_bUpperLowPriceMarkUseage = TRUE;
//	m_bShowLock = TRUE;

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	m_bNormalizedMinuteTimeRule = FALSE;
//	m_bUseNormalizedMinuteTimeRule = TRUE;

	m_nDragModeType = 0;

	m_nYUnitType = WON_TICK;

	m_strCfgFile = _T("");

	m_chType = DAY_DATA_CHART;

	m_nMarketType = CMasterTypeInfo::STOCK_CHART;

	m_bEndDate	= TRUE;
	m_strStartDate	= _T("");
	m_strEndDate	= _T("");

	//m_pCommonBaseInfo = NULL;
	
//@�������� m_pIL_W58H20 = NULL;
//@�������� m_pIL_W34H20 = NULL;
	m_pIL_W46H20 = NULL;
	m_pIL_W20H20 = NULL;
	m_pIL_W20H20_Tool = NULL;
//@�������� m_pIL_W58H19 = NULL;
	m_pIL_W58H20 = NULL;

	m_bChkRevised = FALSE;
	m_nDataType = DAY_DATA_CHART;
	m_bChkPeriod = FALSE;
	m_bChkSaveJipyoOneFile = FALSE;
	// �м��� ���� ���� - ojtaso (20080723)
	m_bChkSaveAnalTool = TRUE;

	// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
	m_bChkShareAnalTool	= FALSE;
	// 2011.01.12 by SYS <<

	m_bChkRegGwansimUp = FALSE;

	m_bChangeTool	= TRUE;
	m_nScrollState = SCROLL_STOP;

	m_bRealUpdate	= TRUE;
	m_nTimeIndex	= -1;
}


void CExtInputBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtInputBarDlg)
	DDX_Control(pDX, IDC_EDIT_PW, m_edAccPW);
	DDX_Control(pDX, IDC_CHK_ORDER2, m_chkOrderLine);
	DDX_Control(pDX, IDC_BTSCROLLPRE, m_btScrollPre);
	DDX_Control(pDX, IDC_BTSCROLLNEXT, m_btScrollNext);
	DDX_Control(pDX, IDC_BTSCROLLSTOP, m_btScrollStop);
	DDX_Control(pDX, IDC_BTN_GWANSIM, m_btnGwanSim);
//@��������	DDX_Control(pDX, IDC_BTN_CHANGETOOL, m_btnChangeTool);
//@��������	DDX_Control(pDX, IDC_BT_SUB1, m_btSub1);
	DDX_Control(pDX, IDC_BT_SUB2, m_btSub2);
//@��������	DDX_Control(pDX, IDC_BT_SUB3, m_btSub3);
//@��������	DDX_Control(pDX, IDC_BT_SUB4, m_btSub4);
//@��������	DDX_Control(pDX, IDC_BT_SUB5, m_btSub5);
//@��������	DDX_Control(pDX, IDC_BT_SUB6, m_btSub6);
//@��������	DDX_Control(pDX, IDC_BT_SUB7, m_btSub7);
//@��������	DDX_Control(pDX, IDC_BT_SUB8, m_btSub8);
//@��������	DDX_Control(pDX, IDC_BT_SUB9, m_btSub9);
//@��������	DDX_Control(pDX, IDC_BT_SUB10, m_btSub10);
//@��������	DDX_Control(pDX, IDC_BT_SUB11, m_btSub11);
//@��������	DDX_Control(pDX, IDC_BT_SUB12, m_btSub12);
//@��������	DDX_Control(pDX, IDC_BT_SUB13, m_btSub13);
//@��������	DDX_Control(pDX, IDC_BT_SUB14, m_btSub14);
	DDX_Control(pDX, IDC_BTETCMENU, m_btEtcMenu);
	DDX_Control(pDX, IDC_BTINITIAL, m_btInitial);
//@�������� DDX_Control(pDX, IDC_BTINITIAL_PERIOD, m_btInitial_Period);
	DDX_Control(pDX, IDC_BTN_PERIOD, m_btnPeriod);
	DDX_Control(pDX, IDC_CHKUPDATEINDEX, m_chkUpdateIndex);
	DDX_Control(pDX, IDC_CHK_ENDSAVE, m_chkEndSave);
	DDX_Control(pDX, IDC_BTN_BUYSCR, m_btnBuyScreen);
	DDX_Control(pDX, IDC_BTN_SELLSCR, m_btnSellScreen);
	DDX_Control(pDX, IDC_BTN_SDATE, m_btnStartDate);
	DDX_Control(pDX, IDC_BTN_EDATE, m_btnEndDate);
	DDX_Control(pDX, IDC_BTFIXPERIOD, m_btFixPeriod);
	DDX_Control(pDX, IDC_BTST, m_btST);
	DDX_Control(pDX, IDC_BTHIST, m_btHist);
	DDX_Control(pDX, IDC_BTCHARTGUIDELINE, m_btChartGuideLine);
	DDX_Control(pDX, IDC_BTANALYSISPATTERN, m_btAnalysisPattern);
	DDX_Control(pDX, IDC_BTEXPREV, m_btExPrev);
	DDX_Control(pDX, IDC_BTEXNEXT, m_btExNext);
	DDX_Control(pDX, IDC_BTSHOWMATRIX, m_btShowMatrix);
	DDX_Control(pDX, IDC_BTSETENVIR, m_btSetEnvir);
	DDX_Control(pDX, IDC_CHKSHOWSCALE, m_chkShowScale);
	DDX_Control(pDX, IDC_CHKJONGMOKLINK, m_chkJongmokLink);
	DDX_Control(pDX, IDC_BTPRINT, m_btPrint);
	DDX_Control(pDX, IDC_BTLOAD, m_btLoad);
	DDX_Control(pDX, IDC_BTSAVE, m_btSave);
	DDX_Control(pDX, IDC_CHKTYPELINK, m_chkTypeLink);
	DDX_Control(pDX, IDC_BTINITSPLIT, m_btInitSplit);
	DDX_Control(pDX, IDC_STBY, m_stBy);
	DDX_Control(pDX, IDC_STSETMATRIX, m_stSetMatrix);
	DDX_Control(pDX, IDC_BTINDEX, m_btIndex);
	DDX_Control(pDX, IDC_CHKFIXPERIOD, m_chkFixPeriod);
	DDX_Check(pDX, IDC_CHK_REVISED, m_bChkRevised);
	DDX_Control(pDX, IDC_BTN_CHANGEUNIT, m_btYUnit);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datePicker);
//@�����߰�
	DDX_Control(pDX, IDC_CMB_SCROLLSPEED, m_cmbScrollSpeed);
	DDX_Control(pDX, IDC_CHK_QUERYOPT, m_chkQueryOpt);
	DDX_Check(pDX, IDC_REAL_CHECK, m_chkRealUpdate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER, m_ctlTime);
	DDX_Control(pDX, IDC_BT_ORDER, m_btOrder);
//@�����߰�
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtInputBarDlg, CRscDlg)
	//{{AFX_MSG_MAP(CExtInputBarDlg)
	ON_BN_CLICKED(IDC_BTSCROLLPRE, OnBtScrollPre)
	ON_BN_CLICKED(IDC_BTSCROLLNEXT, OnBtScrollNext)
	ON_BN_CLICKED(IDC_BTSCROLLSTOP, OnBtScrollStop)
//@�����߰�
	ON_CBN_SELCHANGE(IDC_CMB_SCROLLSPEED, OnSelchangeCmbScrollSpeed)
//@�����߰�
	ON_BN_CLICKED(IDC_BTINITSPLIT, OnBtinitsplit)
	ON_BN_CLICKED(IDC_BTINDEX, OnBtindex)
	ON_BN_CLICKED(IDC_CHKJONGMOKLINK, OnChkjongmoklink)
	ON_BN_CLICKED(IDC_CHKTYPELINK, OnChktypelink)
	ON_BN_CLICKED(IDC_BTLOAD, OnBtload)
	ON_BN_CLICKED(IDC_BTSAVE, OnBtsave)
	ON_BN_CLICKED(IDC_BTINITIAL, OnBtinitial)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHKSHOWSCALE, OnChkshowscale)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTPRINT, OnBtprint)
	ON_BN_CLICKED(IDC_BTSETENVIR, OnBtsetenvir)
	ON_BN_CLICKED(IDC_BTSHOWMATRIX, OnBtshowmatrix)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTEXNEXT, OnBtexnext)
	ON_BN_CLICKED(IDC_BTEXPREV, OnBtexprev)
	ON_BN_CLICKED(IDC_BTANALYSISPATTERN, OnBtanalysispattern)
	ON_BN_CLICKED(IDC_BTCHARTGUIDELINE, OnBtchartguideline)
	ON_BN_CLICKED(IDC_BTHIST, OnBthist)
	ON_BN_CLICKED(IDC_BTST, OnBtst)
	ON_BN_CLICKED(IDC_BTN_SDATE, OnBtnSdate)
	ON_BN_CLICKED(IDC_BTN_EDATE, OnBtnEdate)
//	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
//	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER1, OnCloseupDatetimepicker1)
//	ON_CBN_SELCHANGE(IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BTN_SELLSCR, OnBtnSellscr)
	ON_BN_CLICKED(IDC_BTN_BUYSCR, OnBtnBuyscr)
	ON_BN_CLICKED(IDC_BTFIXPERIOD, OnBtfixperiod)
	ON_BN_CLICKED(IDC_CHK_ENDSAVE, OnChkEndsave)
	ON_BN_CLICKED(IDC_CHKUPDATEINDEX, OnChkupdateindex)
	ON_BN_CLICKED(IDC_BTN_ANALIZE_PERIOD, OnBtnAnalizePeriod)
	ON_BN_CLICKED(IDC_CHK_REVISED, OnChkRevised)
	ON_BN_CLICKED(IDC_BTN_PERIOD, OnBtnPeriod)
	ON_BN_CLICKED(IDC_BTETCMENU, OnBtetcmenu)
//@��������	ON_BN_CLICKED(IDC_BT_SUB1, OnBtSub1)
	ON_BN_CLICKED(IDC_BT_SUB2, OnBtSub2)
//@��������	ON_BN_CLICKED(IDC_BT_SUB3, OnBtSub3)
//@��������	ON_BN_CLICKED(IDC_BT_SUB4, OnBtSub4)
//@��������	ON_BN_CLICKED(IDC_BT_SUB5, OnBtSub5)
//@��������	ON_BN_CLICKED(IDC_BT_SUB6, OnBtSub6)
	ON_BN_CLICKED(IDC_BTN_CHANGEUNIT, OnBtnChangeunit)
	ON_BN_CLICKED(IDC_CHKFIXPERIOD, OnChkfixperiod)
//@�������� ON_BN_CLICKED(IDC_BTINITIAL_PERIOD, OnBtinitialPeriod)
//@��������	ON_BN_CLICKED(IDC_BTN_CHANGETOOL, OnBtnChangeTool)
//@��������	ON_BN_CLICKED(IDC_BT_SUB8, OnBtSub8)
//@��������	ON_BN_CLICKED(IDC_BT_SUB9, OnBtSub9)
	ON_BN_CLICKED(IDC_BTN_GWANSIM, OnBtnGwansim)
//@�����߰�
	ON_BN_CLICKED(IDC_CHK_QUERYOPT, OnChkQueryOpt)
//@�����߰�
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_REAL_CHECK, OnChkRealUpdate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIMEPICKER, OnTimepickerchange)
	ON_BN_CLICKED(IDC_BT_ORDER, OnBtOrder)
	ON_BN_CLICKED (IDC_CHK_ORDER2, OnCheckOrderLink)
	ON_CBN_SELENDOK(IDC_CTRL_ACCOUNT, OnSelendokAccCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtInputBarDlg message handlers

BOOL CExtInputBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();

	// ��Ų ����
	m_crBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);

	//001 2006.06.22{{
//@�������� m_pIL_W58H20 = &(((CStdDialog*)GetParent())->m_ImgList_W58H20);
//@�������� if(m_pIL_W58H20 == NULL) return TRUE;

	m_pIL_W46H20 = &(((CStdDialog*)GetParent())->m_ImgList_W46H20);
	if(m_pIL_W46H20 == NULL) return TRUE;

//@�������� m_pIL_W58H19 = &(((CStdDialog*)GetParent())->m_ImgList_W58H19);
//@�������� if(m_pIL_W58H19 == NULL) return TRUE;
	m_pIL_W58H20 = &(((CStdDialog*)GetParent())->m_ImgList_W58H20);
	if(m_pIL_W58H20 == NULL) return TRUE;

//@�������� m_pIL_W34H20 = &(((CStdDialog*)GetParent())->m_ImgList_W34H20);
//@�������� if(m_pIL_W34H20 == NULL) return TRUE;

//@�������� CImageList* pIL_W46H19_Gwan = &(((CStdDialog*)GetParent())->m_ImgList_W46H19_Gwan);
	CImageList* pIL_W46H20_Gwan = &(((CStdDialog*)GetParent())->m_ImgList_W46H20_Gwan);

	m_btEtcMenu.FP_vSetImage(m_pIL_W58H20, 0, 2, TRUE, "�������", FALSE);
	m_btEtcMenu.FP_vSetToolTipText("�������");

	m_btInitial.FP_vSetImage(m_pIL_W46H20, 32, 34, TRUE, "�ʱ�ȭ", FALSE);
	m_btInitial.FP_vSetToolTipText("��ü��Ʈ�ʱ�ȭ");

//@�������� m_btInitial_Period.FP_vSetImage(m_pIL_W34H20, 0, 2, TRUE, "�Ⱓ", FALSE);
//@�������� m_btInitial_Period.FP_vSetToolTipText("�Ⱓ");

	m_btnGwanSim.FP_vSetImage(pIL_W46H20_Gwan, 0, 2, FALSE, "", FALSE);
	m_btnGwanSim.FP_vSetToolTipText("���ɵ��");

	//m_btEtcMenu.InitButton(this, IDC_BTETCMENU);
	//m_btInitial.InitButton(this, IDC_BTINITIAL);
	//m_btInitial_Period.InitButton(this, IDC_BTINITIAL_PERIOD);
	//m_btEtcMenu.SetDataText("�������");
	//m_btInitial.SetDataText("��ü");
	//m_btInitial_Period.SetDataText("�Ⱓ");
	//001 2006.06.22 }}

	// TODO: Add extra initialization here
	//m_crBk = ((CStdDialog*)GetParent())->GetSkinColorManager()->GetColor(20);
//@��������	m_crBk = RGB(237,243,249);	//RGB(222,222,222);
	m_brBkColor.CreateSolidBrush(m_crBk);

	m_strImageChartPath = ((CStdDialog*)GetParent())->m_szImageDir;
	//m_strCfgFile		= ((CStdDialog*)GetParent())->m_strUserDir + ENVINFOFILE;

	m_strCfgFile = ((CStdDialog*)GetParent())->m_strConfig;
	//	if(m_nMarketType == STOCK_CHART)	
	//	{	
	//		m_strCfgFile = ((CStdDialog*)GetParent())->m_strUserDir + STOCK_ENVINFOFILE;
	//		//m_btnSellScreen.ShowWindow(TRUE);
	//		//m_btnBuyScreen.ShowWindow(TRUE);
	//	}
	//	else if(m_nMarketType == FUTOPT_CHART)
	//		m_strCfgFile = ((CStdDialog*)GetParent())->m_strUserDir + FUTOPT_ENVINFOFILE;
	//	else if(m_nMarketType == UPJONG_CHART)
	//		m_strCfgFile = ((CStdDialog*)GetParent())->m_strUserDir + UPJONG_ENVINFOFILE;
	//	else if(m_nMarketType == FOREIGN_CHART)
	//		m_strCfgFile = ((CStdDialog*)GetParent())->m_strUserDir + FOREIGN_ENVINFOFILE;


	CString strImageSelect;
	CString strImageUp;
	CString strImageDown;
	CString strImageOver;
	CString strImageDis;


	m_pIL_W46H20 = &(((CStdDialog*)GetParent())->m_ImgList_W46H20);
	if(m_pIL_W46H20 == NULL) return TRUE;

	//��ǥ
	m_btIndex.FP_vSetImage( m_pIL_W46H20, 2, 3);	
	m_btIndex.FP_vSetToolTipText("��ǥ");

	//ȯ�漳�� 	
	m_btSetEnvir.FP_vSetImage(m_pIL_W46H20, 24, 25);
	m_btSetEnvir.FP_vSetToolTipText("ȯ�漳��");

	//���� ����ȭ
	m_chkJongmokLink.FP_vSetImage(m_pIL_W46H20, 4, 6, FALSE, "", TRUE, 7);
	m_chkJongmokLink.FP_vSetToolTipText("���񿬵�����");

	//�ð� ����ȭ 		
	m_chkTypeLink.FP_vSetImage(m_pIL_W46H20, 8, 10, FALSE, "", TRUE, 11);
	m_chkTypeLink.FP_vSetToolTipText("�ֱ⿬������");

	//=ȯ�� ����ȭ 
	//m_chkUpdateIndex.FP_vSetImage(m_pIL_W46H20, 12, 14, FALSE, "", TRUE);
	//m_chkUpdateIndex.FP_vSetImage(m_pIL_W46H20, 12, 14);
	m_chkUpdateIndex.FP_vSetImage(m_pIL_W46H20, 12, 14, FALSE, "", FALSE, 15);
	m_chkUpdateIndex.FP_vSetToolTipText("��Ʈ��ǥ����ȭ");	

	//�Ⱓ ����ȭ 	
	m_btFixPeriod.FP_vSetImage(m_pIL_W46H20, 16, 18, FALSE, "", FALSE, 19);
	m_btFixPeriod.FP_vSetToolTipText("�Ⱓ����");


	m_pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
	if(m_pIL_W20H20 == NULL) return TRUE;

	//Tool / ����ȭ��ư
//@��������	m_btnChangeTool.FP_vSetImage(m_pIL_W20H20, 0, 8, FALSE, "T", TRUE);		
//@��������	m_btnChangeTool.FP_vSetToolTipText("Tool");

	m_pIL_W20H20_Tool = &(((CStdDialog*)GetParent())->m_ImgList_W20H20_Tool);
	if(m_pIL_W20H20_Tool == NULL) return TRUE;
	//	m_btSub1.FP_vSetImage(m_pIL_W20H20_Tool, 0, 2, FALSE, "", TRUE);	
	//	m_btSub1.FP_vSetToolTipText("Line�߼���");
	m_btSub2.FP_vSetImage(m_pIL_W20H20_Tool, 0, 2, FALSE, "", TRUE);		
	m_btSub2.FP_vSetToolTipText("���ڼ�");
//@��������	m_btSub3.FP_vSetImage(m_pIL_W20H20_Tool, 4, 6, FALSE, "", TRUE);		
//@��������	m_btSub3.FP_vSetToolTipText("����");
	//	m_btSub4.FP_vSetImage(m_pIL_W20H20_Tool, 12, 14);		
	//	m_btSub4.FP_vSetToolTipText("��ü���찳");
	//	m_btSub5.FP_vSetImage(m_pIL_W20H20_Tool, 20, 22);		
	//	m_btSub5.FP_vSetToolTipText("Y�����");
	//m_btSub6.FP_vSetImage(m_pIL_W20H20_Tool, 24, 26, FALSE, "", TRUE);		
	//	m_btSub6.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub6.FP_vSetToolTipText("��ġ��ȸâ");
	//	m_btSub7.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub7.FP_vSetToolTipText("������");
//@��������	m_btSub8.FP_vSetImage( m_pIL_W20H20_Tool, 8, 10, FALSE, "", TRUE);	
//@��������	m_btSub8.FP_vSetToolTipText("������");
	//	m_btSub9.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub9.FP_vSetToolTipText("��ġ��ȸâ");
	//	m_btSub10.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub10.FP_vSetToolTipText("���찳");
	//	m_btSub11.FP_vSetImage( m_pIL_W20H20, 4, 6, FALSE, "", TRUE);	
	//	m_btSub11.FP_vSetToolTipText("����");
	//	m_btSub12.FP_vSetImage( m_pIL_W20H20, 12, 14, FALSE, "", TRUE);	
	//	m_btSub12.FP_vSetToolTipText("����");
	//	m_btSub13.FP_vSetImage( m_pIL_W20H20, 20, 22, FALSE, "", TRUE);	
	//	m_btSub13.FP_vSetToolTipText("�μ�");
	//	m_btSub14.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub14.FP_vSetToolTipText("����");

	//�Ⱓ����
	m_btnPeriod.FP_vSetImage( m_pIL_W20H20, 82, 83, TRUE, "P", FALSE);
	m_btnPeriod.FP_vSetToolTipText("�Ⱓ����");
	//	//���� ��ȯ 
	m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 12, 14, FALSE, "\\", TRUE, 15);	
	m_btYUnit.FP_vSetToolTipText("Y�� ������ȯ");
	m_nYUnitType = WON_TICK;

	//chart ���� 
	m_btShowMatrix.FP_vSetImage(m_pIL_W20H20, 52, 54);
	m_btShowMatrix.FP_vSetToolTipText("����");

	//1*1
	m_btInitSplit.FP_vSetImage(m_pIL_W20H20, 72, 74);
	m_btInitSplit.FP_vSetToolTipText("���Һ���");

	//��ũ������	�������� ǥ�÷� ����
	//m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106, TRUE, "", FALSE);
	m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106, FALSE, "", TRUE);
	m_btScrollPre.FP_vSetToolTipText("����");

	//��ũ�� ����
//	m_btScrollStop.FP_vSetImage(m_pIL_W20H20, 130, 132);
	m_btScrollStop.FP_vSetImage(m_pIL_W20H20, 64, 66);
	m_btScrollStop.FP_vSetToolTipText("��ũ������");

	//��ũ������	
	//m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70, TRUE, "", FALSE);
	m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70, FALSE, "", TRUE);
	m_btScrollNext.FP_vSetToolTipText("����");

	//����	
	m_btExPrev.FP_vSetImage(m_pIL_W20H20, 60, 62);
	m_btExPrev.FP_vSetToolTipText("����");

	//����	
	m_btExNext.FP_vSetImage(m_pIL_W20H20, 56, 58);	
	m_btExNext.FP_vSetToolTipText("����");

	//������ �����ֱ� 	
	m_chkShowScale.FP_vSetImage(m_pIL_W20H20, 24, 26, FALSE, "", TRUE);
	m_chkShowScale.FP_vSetToolTipText("�����Ϻ����ֱ�");

	//��Ʈ����
	m_btSave.FP_vSetImage(m_pIL_W20H20, 4, 6);
	m_btSave.FP_vSetToolTipText("��Ʈ����");

	//������Ʈ�ҷ�����
	m_btLoad.FP_vSetImage(m_pIL_W20H20, 12, 14);
	m_btLoad.FP_vSetToolTipText("������Ʈ�ҷ�����");

	//�μ�	
	m_btPrint.FP_vSetImage(m_pIL_W20H20, 20, 22);
	m_btPrint.FP_vSetToolTipText("�μ�");

	//������ ������
	m_btnStartDate	.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE, "��", TRUE);	
	m_btnEndDate	.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE, "��", TRUE);

	//�ŵ� �ż� ȭ��
	m_btnSellScreen.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE, "��");
	m_btnSellScreen.FP_vSetToolTipText("�ŵ�ȭ��");

	m_btnBuyScreen.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE, "��");
	m_btnBuyScreen.FP_vSetToolTipText("�ż�ȭ��");

	//��Ÿ Addin ���� ��ư 
	m_btAnalysisPattern.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"P")	;	//m_btAnalysisPattern.FP_vSetToolTipText();
	m_btChartGuideLine.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"G")		;	//m_btChartGuideLine.FP_vSetToolTipText();
	m_btHist.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"C")				;	//m_btHist.FP_vSetToolTipText();
	m_btST.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"S")					;	//m_btST.FP_vSetToolTipText();

	m_btOrder.FP_vSetImage(m_pIL_W58H20, 0, 2, TRUE, "�ֹ�ȭ��", FALSE);
	m_btOrder.FP_vSetToolTipText("�ֹ�ȭ��");

	SetExt(0);
	char szTemp[64];

	//<<20100308_JS.Kim �ַθ�
	CRect rect;
	GetDlgItem(IDC_CTRL_ACCOUNT)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDC_CTRL_ACCOUNT)->DestroyWindow();

	m_pAccountCtrl = new CAccountCtrl;
//	m_pAccountCtrl->SetRootPath(((CStdDialog*)GetParent())->m_strRootPath);
	//m_pAccountCtrl->CreateAccountCtrl(this, GetParent()->GetParent()->GetParent()->GetParent(), rect, IDC_CTRL_ACCOUNT, TRKEY_ACCCTRL1, TRKEY_ACCCTRL2, ACC_FOREIGN);
	m_pAccountCtrl->CreateAccountCtrl(this, GetParent()->GetParent()->GetParent()->GetParent(), rect, IDC_CTRL_ACCOUNT, ACC_REV_KEY1, ACC_REV_KEY2, ACC_FOREIGN);


	m_pAccountCtrl->Init();
	OnSelendokAccCtrl();	

/*
	UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);

	//nOpenedOwner  = 0 : �޴�/���ٿ��� ����
	//nOpenedOwner  = 1 : ���콺 ������ ��ư���� ���� (�����ڵ� ����)
	//nOpenedOwner  = 2 : ��ũ��Ʈ,dll���� ȭ�� ��û (OpenMapData ����)
	if(nOpenedOwner==0)
	{
		{
			::GetPrivateProfileString("ExtInputBar","Saved","1",szTemp,sizeof(szTemp),m_strCfgFile);
			if(szTemp[0]=='1')
			{
				m_nSave = SAVE_TRUE;
				m_chkEndSave.SetCheck(1);
			}
			else
			{
				m_nSave = SAVE_FALSE;
				m_chkEndSave.SetCheck(0);
			}
		}
	}
	else
	{
		m_nSave = SAVE_DISABLE;
	}
*/
	::GetPrivateProfileString("ExtInputBar","DragModeType","0",szTemp,sizeof(szTemp),m_strCfgFile);
	m_nDragModeType = atol(szTemp);

	//{{JS.Kim_20100903 Solomon ��ǥ��ü�������� �ϸ� �߼����� ������ �ȵǴ� ������ �־� �ϴ� �޴����� ����
	/*
	::GetPrivateProfileString("ExtInputBar","SaveJipyoOneFile","1",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
	{
		m_bChkSaveJipyoOneFile = TRUE;
	}
	else
	{
		m_bChkSaveJipyoOneFile = FALSE;
	}
	*/
	//}}

	::GetPrivateProfileString("ExtInputBar","RegGwansimUp","0",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
	{
		m_bChkRegGwansimUp = TRUE;
	}
	else
	{
		m_bChkRegGwansimUp = FALSE;
	}

	// �м��� ���� ���� - ojtaso (20080723)
	::GetPrivateProfileString("ExtInputBar","SaveAnalTool","1",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
	{
		m_bChkSaveAnalTool = TRUE;
	}
	else
	{
		m_bChkSaveAnalTool = FALSE;
	}

	// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
	// �м��� ���� ���
	::GetPrivateProfileString("ExtInputBar","ShareAnalTool","0",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
		m_bChkShareAnalTool = TRUE;
	else
		m_bChkShareAnalTool = FALSE;
	// ����, �м������� ����� üũ���°� �ƴϸ�..  �м������� ��ɵ� üũ���� �Ѵ�.
	if (!m_bChkSaveAnalTool)
		m_bChkShareAnalTool = FALSE;
	// 2011.01.12 by SYS <<

//@��������
//	m_datePicker.InitCodeInput(this, IDC_DATETIMEPICKER1, 5000);
//@��������
	CRect rectComp;
	GetDlgItem(IDC_DATETIMEPICKER1)->GetWindowRect(rectComp);
	ScreenToClient(rectComp);
	m_datePicker.SetWindowPos(NULL, rectComp.left, rectComp.top, rectComp.Width(), 19, SWP_NOZORDER);


	//>>JS.Kim_20101216
	m_cmbScrollSpeed.AddString("X  1");
	m_cmbScrollSpeed.AddString("X  2");
	m_cmbScrollSpeed.AddString("X  4");
	m_cmbScrollSpeed.AddString("X  8");
	//<<

//@�����߰�
	m_cmbScrollSpeed.SetCurSel(0);
//@�����߰�

// --> [Edit]  ������ 2008/10/31
	m_ctlTime = CTime::GetCurrentTime();
	m_chkRealUpdate = FALSE;
	UpdateData(FALSE);



////////////////////////////////////////////////////////////////////////////////////////////
// --> [Edit]  ������ 2008/11/09	( �ӽ÷� ���� )
//	GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
//	m_datePicker.EnableWindow(FALSE);
//	GetDlgItem(IDC_REAL_CHECK)->EnableWindow(FALSE);
// <-- [Edit]  ������ 2008/11/09
////////////////////////////////////////////////////////////////////////////////////////////

//	OnChkRealUpdate();
// <-- [Edit]  ������ 2008/10/31

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CExtInputBarDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case UMSG_UPDATEMATRIX:
		{
			if(wParam*lParam>20)
			{
				MessageBox("��Ʈ�� ���� 20�����Ϸ� �Ͽ��ֽʽÿ�.",DLLTITLENAME);
			}
			else if(wParam!=m_lCols||lParam!=m_lRows)
			{
				m_lCols = wParam;
				m_lRows = lParam;	
				if(m_pChartMng->GetSafeHwnd())
					m_pChartMng->SendMessage(UMSG_SETMATRIX,m_lRows, m_lCols);			
			}
		}
		break;
	case UMSG_SETMATRIX:
		{
			m_lRows = wParam;
			m_lCols = lParam;
			CString strBy;
			strBy.Format("%2d X%2d",m_lCols,m_lRows);
			//2007.01.13 by LYH 1*1
			CString strOrgBy;
			m_stBy.GetWindowText(strOrgBy);
			if(strBy != strOrgBy)
			{
				if(m_lRows==1 && m_lCols==1 )
				{
					m_chkTypeLink.EnableWindow(FALSE);
					m_chkJongmokLink.EnableWindow(FALSE);
					m_btFixPeriod.EnableWindow(FALSE);
					m_chkUpdateIndex.EnableWindow(FALSE);
				}
				else
				{
					m_chkTypeLink.EnableWindow(TRUE);
					m_chkJongmokLink.EnableWindow(TRUE);
					m_btFixPeriod.EnableWindow(TRUE);
					m_chkUpdateIndex.EnableWindow(TRUE);
				}
			}
			m_stBy.SetWindowText(strBy);
		}
		break;
	case UMSG_CLEARLINK:
		{
			ClearLink();
		}
		break;
	case UMSG_SETJONGMOKLINK:
		{
			m_chkJongmokLink.SetCheck(wParam);
		}
		break;
	case UMSG_GETSAVED:
		{
			if(m_nSave==SAVE_TRUE)
			{
				return TRUE;
			}
			return FALSE;
		}
		break;
	case UMSG_GETJONGMOKLINK:
		{
			return (long)(m_chkJongmokLink.GetCheck());
		}
		break;
	case UMSG_GETSAVEJIPYOONEFILE:
		{
			if(wParam == 100)
				return (long)(m_bChkSaveAnalTool);
			else
				return (long)(m_bChkSaveJipyoOneFile);
		}
		break;

		// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
	case UMSG_GET_ALALTOOL_SHAREFILE:
		{
			return (long)(m_bChkShareAnalTool);
		}
		break;
		// 2011.01.12 by SYS <<

	case UMSG_GETENVSINK:
		{
			return (long)0;
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
// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	case UMSG_SETNORMALIZEDMINUTETIMERULE:
//		{
//			m_bNormalizedMinuteTimeRule = wParam;
//		}
//		break;

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	case UMSG_GETNORMALIZEDMINUTETIMERULE:
//		{
//			return m_bNormalizedMinuteTimeRule;
//		}
//		break;

	case UMSG_SETHIGHLIGHTINFO:
		{
			CString strComment = (LPCTSTR)wParam;
			m_stSetMatrix.SetWindowText(strComment);
		}
		break;
	case UMSG_GETDRAGMODETYPE:
		{
			return m_nDragModeType;
		}
		break;
	case UMSG_SETVIEWENDTIME:
		{
			if((m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART||m_chType==MONTH_DATA_CHART||m_chType==YEAR_DATA_CHART))
			{
				long lEndDate = wParam;
			/*	2005.09.06	
				//�޷� ��Ʈ�ѿ� ���ᳯ¥�� �ٲ��ִ� ��ƾ�̳��� 
				//��Ʈ OCX�� ������ ���� ��û�� ������� 
				//�亯�� ���� �����ϸ� �ٲ۴�. */

				SYSTEMTIME tm;
				::ZeroMemory(&tm,sizeof(tm));
				if(m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART)
				{
					tm.wYear	= (WORD)lEndDate/10000;
					tm.wMonth	= (WORD)lEndDate%10000/100;
					tm.wDay		= (WORD)lEndDate%100;
				}
				else if(m_chType==MONTH_DATA_CHART)
				{
					tm.wYear	= (WORD)lEndDate/10000;
					tm.wMonth	= (WORD)lEndDate%10000/100;
					tm.wDay = 1;
				}

				/*
				if(m_bEndDate) // �����Ϸ� ���õǾ� ���� ��쿡�� �޷� ��Ʈ�ѿ� ���� ���Ѵ�.
				{
					m_datePicker.SetTime(&tm);
				}
				*/
				//2005. 07. 28 added by sy, nam chart�� ���� ���� ���� ��� �ݿ��Ѵ�.
				m_strEndDate.Format("%d", lEndDate);				
			}
		}
		break;
	case UMSG_GET_TIME_FROM_DATETIMEPICKER:
		{
			/* ====================================
			//2005. 07. 28
			SYSTEMTIME* pTm;
			pTm = (SYSTEMTIME*)lParam;

			//���� ������ ���� �ñ⺯������ �����;� �Ѵ�. 05.06. 03
			m_datePicker.GetTime(pTm);
			*/
			return atoi(m_strEndDate);
		}
		break;
	case UMSG_GETENDDATEFROMINPUTBAR:
		{
			//SYSTEMTIME tm;		
			if((m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART||m_chType==MONTH_DATA_CHART||m_chType==YEAR_DATA_CHART))
			{
				//m_datePicker.GetTime(&tm);
				//return (long)(tm.wYear*10000+tm.wMonth*100+tm.wDay);
				return atoi(m_strEndDate);
			}
			return 0L;
		}
		break;
	case UMSG_SETINITIALCHARTDATA:
		{		
			m_bEndDate	= TRUE;
			m_strStartDate	= _T("");
			m_strEndDate	= _T("");

			stChartInitData* _pStData = (stChartInitData*)lParam;	
			m_nYUnitType = _pStData->m_nChartNumericValue;
			if(_pStData->m_nChartNumericValue == WON_TYPE)
			{				
				m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 12, 13, FALSE, "\\", TRUE, 15);
			}
			else if(_pStData->m_nChartNumericValue == PERCENT_TYPE)
			{
				m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 20, 21, FALSE, "%", TRUE, 23);
			}
			else if(_pStData->m_nChartNumericValue == DOLLAR_TYPE)
			{
				m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 16, 17, FALSE, "$", TRUE, 19);
			}
			m_btYUnit.Invalidate();
			
			//2005. 08. 25 ================================================================
			//ƽ/��/��/��/��/�� ���� ���� 05.08.31
			m_chType = _pStData->m_chType;

			//�����ڵ� 05. 09. 05 
			//m_strCode = _pStData->m_strCode;

			// chart Type�� ���� Control �޷� Control Ȱ��ȭ ����..
// --> [Edit]  ������ 2008/12/02	( �ֱ⿡ ���� ����Ʈ�� Ȱ��ȭ )
			GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
// <-- [Edit]  ������ 2008/12/02
			BOOL bEnable = FALSE;
			if((_pStData->m_chType==DAY_DATA_CHART||_pStData->m_chType==WEEK_DATA_CHART||_pStData->m_chType==MONTH_DATA_CHART||_pStData->m_chType==YEAR_DATA_CHART||_pStData->m_chType==MIN_DATA_CHART))
			{
				bEnable = TRUE;
				GetDlgItem(IDC_REAL_CHECK)->EnableWindow(bEnable);
			}
			else
				GetDlgItem(IDC_REAL_CHECK)->EnableWindow(FALSE);


			//m_datePicker.EnableWindow(bEnable);
//@��������
//			if(m_datePicker.GetEnabled() == !bEnable)
//				m_datePicker.SetEnabled(bEnable);
//@��������
//@�����߰�

			//	���ϰ�� �ð���Ʈ���� Ȱ��ȭ �����ش�.
			if ( !m_bRealUpdate )
			{
				if ( _pStData->m_chType==MIN_DATA_CHART )
					GetDlgItem(IDC_TIMEPICKER)->EnableWindow(TRUE);
				else
					GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);

				if(m_datePicker.IsWindowEnabled() == !bEnable)
					m_datePicker.EnableWindow(bEnable);
			}
			else
			{
				m_datePicker.EnableWindow(FALSE);
				GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
			}

////////////////////////////////////////////////////////////////////////////////////////////
// --> [Edit]  ������ 2008/11/09	( �ӽ÷� ���� )
//			m_datePicker.EnableWindow(FALSE);
//			GetDlgItem(IDC_REAL_CHECK)->EnableWindow(FALSE);
//			GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
// <-- [Edit]  ������ 2008/11/09
////////////////////////////////////////////////////////////////////////////////////////////

//@�����߰�
			m_btnStartDate.EnableWindow(bEnable);
			m_btnEndDate.EnableWindow(bEnable);

// --> [Edit]  ������ 2008/11/07	( ��¥ ���� ����.. )
//			// ��¥ ���� ����
//			if(bEnable)
//			{		
//				m_strStartDate	.Format("%d", _pStData->m_lStartDate);
//				m_strEndDate	.Format("%d", _pStData->m_lEndDate);
//			
//				if(		(m_strStartDate	.GetLength() != 8) ||
//						(m_strEndDate	.GetLength() != 8) ||	
//						(atoi(m_strStartDate)<= 19700101 ) ||
//						(atoi(m_strEndDate	)<= 19700101 ) 
//				)
//				{
//					CTime currentTime = CTime::GetCurrentTime(); 
////@�������� : m_datePicker �ε��ȵǰ� �������Ƿ� �ڵ�üũ ����.
//					if(m_datePicker.GetSafeHwnd()) m_datePicker.SetTime(&currentTime);
//					m_strStartDate = m_strEndDate =currentTime.Format("%Y%m%d");
//				}
//
//				m_bEndDate = _pStData->m_bStartDateState ? FALSE : TRUE;
//				if( m_bEndDate )				
//				{
//					OnBtnEdate();
//				}
//				else 
//				{
//					OnBtnSdate();
//				}
//			}
// <-- [Edit]  ������ 2008/11/07

			//01.11 LYH================================================================
			//���� �ְ� �� �α� �����ֱ� ����
			m_bChkRevised	= _pStData->m_bRevised;

			// (2008/9/23 - Seung-Won, Bae) Disable Sync Button for Empty Chart.
			if( _pStData->m_lEndDate == 0 && _pStData->m_lStartDate == 0)
			{
				m_chkTypeLink.EnableWindow(FALSE);
				m_chkJongmokLink.EnableWindow(FALSE);
				m_btFixPeriod.EnableWindow(FALSE);
				m_chkUpdateIndex.EnableWindow(FALSE);
			}
			else
			{
// --> [Edit]  ������ 2008/10/22
				if(m_lRows==1 && m_lCols==1 )
				{
					m_chkTypeLink.EnableWindow(FALSE);
					m_chkJongmokLink.EnableWindow(FALSE);
					m_btFixPeriod.EnableWindow(FALSE);
					m_chkUpdateIndex.EnableWindow(FALSE);
				}
				else
				{
					m_chkTypeLink.EnableWindow(TRUE);
					m_chkJongmokLink.EnableWindow(TRUE);
					m_btFixPeriod.EnableWindow(TRUE);
					// (2008/9/24 - Seung-Won, Bae) Disable Index Sync for Multi-Item Chart.
					m_chkUpdateIndex.EnableWindow( ( ( CChartMng *)m_pChartMng)->GetpChartItemSelected()->GetChartItemCount() == 1);
				}
// <-- [Edit]  ������ 2008/10/22
			}

			m_edAccPW.GetWindowText(m_strPassWord);
			UpdateData(FALSE);			
			//=====================================================================
			//=============================================================================

		}
		break;
	case UMSG_GET_TICKMINDAY_VALUE:
		{
			ChangeStatusDateControl(lParam);
		}
		break;
	case UMSG_CHART_INIT_FROMOCX: //��Ʈ �ʱ�ȭ ó��.
		{
			if(wParam==1)
				ClearLink();
			CTime currentTime = CTime::GetCurrentTime(); 	
			m_datePicker.SetTime(&currentTime);	
			
			
			m_bEndDate = TRUE;

			//������ ������
			m_btnStartDate	.FP_vSetImage(m_pIL_W20H20, 80, 80, TRUE, "��", TRUE);	
			m_btnEndDate	.FP_vSetImage(m_pIL_W20H20, 80, 80, TRUE, "��", TRUE);
			m_btnStartDate	.Invalidate();
			m_btnEndDate	.Invalidate();

			m_strStartDate = m_strEndDate =currentTime.Format("%Y%m%d");
		
			if(m_pChartMng && m_pChartMng->GetSafeHwnd())
				m_pChartMng->SendMessage( UMS_SETCHART_DATEINFO, (WPARAM)m_bEndDate, (LPARAM)atoi(m_strEndDate));

			((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetEndDateRequested(m_strEndDate);
		}
		break;
	case UMSG_SYNC_TOOLBAR:
		{
			if(wParam == CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL)
			{
				m_btSub2.SetCheck(lParam);
			}
			else if(wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT)
			{
				if(!lParam)
				{
					//m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 126, 128);
					m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106);
					m_btScrollPre.Invalidate();
				}
				else
				{
					//m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 128, 126);
					m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 106, 104);
					m_btScrollPre.Invalidate();
				}
			}
			else if(wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT)
			{
				if(!lParam)
				{
					//m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 134, 136);
					m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70);
					m_btScrollNext.Invalidate();
				}
				else
				{
					//m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 136, 134);
					m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 70, 68);
					m_btScrollNext.Invalidate();
				}
			}
		}
		break;
	case UWM_RECEIVE_ITRQUERY:
		{
			if(wParam == 2)
			{
				IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
				IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
				long lGroup = (long)lParam;
				CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
				CStringArray strCodes;
				strCodes.Add(strCode);
				// 4. ���� �Ŵ����� ����Լ� ����� ���� ���� �����ʿ�
//				pMng->AddGwansimtem_New(lGroup, strCodes, (long)m_hWnd);
			}
		}
		break;
// --> [Edit]  ������ 2008/10/15
	case UMSG_CHE_HOGA_VALUE:
		{
			
		}
		break;
	case WM_ENTERIDLE:
		{
			int n = 0;
		}
		break;
// <-- [Edit]  ������ 2008/10/15
	}

	return CRscDlg::WindowProc(message, wParam, lParam);
}

void CExtInputBarDlg::ChangeButtonColor() 
{
//	CDrMLib_CBaseInfo2* pBaseInfo = NULL;
//	pBaseInfo = ((CStdDialog*)GetParent())->GetpMainBaseInfo();
//	if(pBaseInfo)
//	{
//		//	CClrButton m_btnFontSet;
//		//	m_baseInfo.GetButtonColor(m_btnFontSet.m_arrColor);
//		//	m_btnFontSet.ChangeSetColor();
//	}
}

void CExtInputBarDlg::OnBtinitsplit() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd())
		m_pChartMng->SendMessage(UMSG_SETONECHART);
	ClearLink();
}


//==============================================================================
// ��� ������ ��Ʈ List�� ����� �ߺ��Ǿ� �����Ѵ�.
// 2005. 05. 26 by sy,nam
//==============================================================================
//DEL void CExtInputBarDlg::OnBtchart() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL     CMenu menu;
//DEL 	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//DEL     menu.CreatePopupMenu();
//DEL 
//DEL 	CStringArray strArrayIndicator;
//DEL 	if(m_pChartMng->GetSafeHwnd())
//DEL 		m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)"Ư����ǥ",(LPARAM)&strArrayIndicator);
//DEL 
//DEL 	CString strIndicator;
//DEL 	BOOL bExistance = FALSE;
//DEL 	long lSize = strArrayIndicator.GetSize();
//DEL 	UINT nFlags = NULL;
//DEL 	for(long lPos=0;lPos<lSize;lPos++)
//DEL 	{
//DEL 		strIndicator = strArrayIndicator.GetAt(lPos);
//DEL 		if(m_pChartMng->GetSafeHwnd())
//DEL 			bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
//DEL 		if(bExistance)
//DEL 		{
//DEL 			nFlags = MF_CHECKED;
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			nFlags = MF_UNCHECKED;
//DEL 		}
//DEL 		   
//DEL 		menu.AppendMenu(nFlags,100+lPos,strIndicator);
//DEL 	}
//DEL 	
//DEL 	CRect rect;
//DEL 	GetDlgItem(IDC_BTCHART)->GetWindowRect(rect);
//DEL 	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
//DEL 	if(nRetValue)
//DEL 	{
//DEL 		if(m_pChartMng->GetSafeHwnd())
//DEL 			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strArrayIndicator.GetAt(nRetValue-100));
//DEL 	}	
//DEL     menu.DestroyMenu();
//DEL }

void CExtInputBarDlg::OnBtindex() 
{
	// TODO: Add your control notification handler code here
//	char* szIndexGroupName[] = { "�⺻��ǥ",
//								 "�̵����",
//								 "�ְ���ǥ",
//								 "��Ÿ��ǥ",
//								 "÷����ǥ",
//								 "�߼���ǥ",
//								 "�ŷ�����ǥ",
//								 "��������ǥ",
//								 "ź�¼���ǥ"};

//-------------------------------------------------------------------
// Han Hwa�� �°� ��ǥ �޴��� �����Ѵ�.
//-------------------------------------------------------------------
	char* szIndexGroupName[] = {					
								 //"��ü��ǥ",
								 "������ǥ",
								 "�߼���ǥ",
								 "��������ǥ",
								 "�ŷ�����ǥ",
								 "2���Ļ���ǥ",
								 "��Ÿ��ǥ",
								 "������ǥ"
								 };
//-------------------------------------------------------------------
	long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);
	//=============================================================
//#ifdef _DEBUG
	// 05.10.24
	// �ֽĸ� ���� ��ǥ �߰� 
	//=============================================================
	if(m_nMarketType != CMasterTypeInfo::STOCK_CHART)
		lAllCnt -= 1;
	//=============================================================
//#endif

    CMenu menu;
    menu.CreatePopupMenu();

	CStringArray strArrayIndicator;	
	CStringArray strArrayIndicatorAll;	
	CStringArray strArrayMarket; //���� ��ǥ����Ʈ ���� ����.
	long lPosAll = 0;
	long lSize = 0;
	CString strIndicator;
	CMenu *pMenuChild = NULL;
	UINT nFlagsChild = NULL;
	BOOL bExistance = FALSE;
	strArrayIndicatorAll.RemoveAll();	
	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		pMenuChild = new CMenu;
		pMenuChild->CreatePopupMenu();
		menu.AppendMenu(MF_POPUP,(UINT)pMenuChild->GetSafeHmenu(),szIndexGroupName[lIndex]);

		if(m_pChartMng->GetSafeHwnd())
			m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)szIndexGroupName[lIndex],(LPARAM)&strArrayIndicator);
		lSize = strArrayIndicator.GetSize();
		nFlagsChild = NULL;
		bExistance = FALSE;

		for(long lPos=0;lPos<lSize;lPos++)
		{
			strIndicator = strArrayIndicator.GetAt(lPos);
			if(m_pChartMng->GetSafeHwnd())
				bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
			if(bExistance)
			{
				nFlagsChild = MF_CHECKED;
			}
			else
			{
				nFlagsChild = MF_UNCHECKED;
			}
			   
			pMenuChild->AppendMenu(nFlagsChild,100+lPosAll,strIndicator);
			lPosAll++;
		}
		strArrayIndicatorAll.Append(strArrayIndicator);

		//==========================================================
		//������ǥ �׸��� ������ ���� ���� �ѹ� �� �Ѵ�. 05.11. 01
		//�޴����� ������ ������ǥ��� ���� �˱����� 
		//==========================================================
		if(lIndex == lAllCnt - 1)
		{
			strArrayMarket.Append(strArrayIndicator);
		}
	}

//	CString sArrMarket = strArrayIndicatorAll.GetAt(lAllCnt); //������ǥ �׸��� �߰��Ǿ� �ִ� CStringArray�� �����´�.


	int nCntAddedByFavorite = 0;

	CFavoriteThingsMng* pFavoriteMng = NULL;

	pFavoriteMng = &((CStdDialog*)GetParent())->m_FavoriteMng;
	if(pFavoriteMng)
	{
		pFavoriteMng->LoadGrpInfo();
		DWORD wSizeGrp = pFavoriteMng->m_strArrayGrp.GetSize();
		DWORD wSizeItem = 0;
		HTREEITEM  hRootItem = NULL;
		HTREEITEM  hItem = NULL;
		CString strGrpName;
		CString strItemName;
		DWORD wIndexGrp = 0;
		DWORD wIndexItem = 0;
		stFvrData _stFvrData;
		BOOL	bCreateRoot = FALSE;
		for(wIndexGrp=0;wIndexGrp<wSizeGrp;wIndexGrp++)
		{
			bCreateRoot = FALSE;
			strGrpName  = pFavoriteMng->m_strArrayGrp.GetAt(wIndexGrp);
			pFavoriteMng->LoadItemInfo(strGrpName);
			wSizeItem = pFavoriteMng->m_ArrayFvrItem.GetSize();
			for(wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
			{
				_stFvrData = pFavoriteMng->m_ArrayFvrItem.GetAt(wIndexItem);
				if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX&&_stFvrData.m_lSubData1==0)
				{
					if(!nCntAddedByFavorite)
					{
						menu.AppendMenu(MF_SEPARATOR);
						nCntAddedByFavorite++;
					}
					bCreateRoot = TRUE;
					if(m_pChartMng->GetSafeHwnd())
						bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)_stFvrData.m_chData1);
					if(bExistance)
					{
						nFlagsChild = MF_CHECKED;
					}
					else
					{
						nFlagsChild = MF_UNCHECKED;
					}
					menu.AppendMenu(nFlagsChild,100+lPosAll,_stFvrData.m_chData1);
					nCntAddedByFavorite++;
					lPosAll++;
					strArrayIndicatorAll.Add(_stFvrData.m_chData1);
				}
			}		
		}
	}
	
	CRect rect;
	GetDlgItem(IDC_BTINDEX)->GetWindowRect(rect);
	UINT nFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(nFlags, rect.left, rect.bottom, this);
	if(nRetValue)
	{	
		CString sJipyo = strArrayIndicatorAll.GetAt(nRetValue-100);

		//==============================================================================
		BOOL	bMarketJipyo = FALSE;
		for(int k = 0; k < strArrayMarket.GetSize(); k++)
		{
			if( sJipyo.CompareNoCase(strArrayMarket.GetAt(k)) == 0 )
			{
				bMarketJipyo = TRUE;
				break;
			}
		}
		//==============================================================================

		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)sJipyo, bMarketJipyo ? FROM_EXTINPUTBAR_MARKET : 0) ;
	}
	
	for(long lIndexOfMenu=0;lIndexOfMenu<(lAllCnt+nCntAddedByFavorite);lIndexOfMenu++)
	{
		pMenuChild = menu.GetSubMenu(lIndexOfMenu);
		if(pMenuChild)
		{
			pMenuChild->DestroyMenu();
			delete pMenuChild;
		}
	}
	menu.DestroyMenu();
}


//void CExtInputBarDlg::OnChkext1() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(1);
//}

//void CExtInputBarDlg::OnChkext2() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(2);
//}
//
//void CExtInputBarDlg::OnChkext3() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(3);	
//}
//
//void CExtInputBarDlg::OnChkext4() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(4);	
//}
//
//void CExtInputBarDlg::OnChkext5() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(5);
//}

void CExtInputBarDlg::SetExt(int nAct)
{
	EnableWindow(FALSE);
//	BOOL bExt1 = m_chkExt1.GetCheck();
//	BOOL bExt2 = m_chkExt2.GetCheck();
//	BOOL bExt3 = m_chkExt3.GetCheck();
//	BOOL bExt4 = m_chkExt4.GetCheck();
//	BOOL bExt5 = m_chkExt5.GetCheck();

	int nCmdShow1 = SW_SHOW;
	int nCmdShow2 = SW_SHOW;
	int nCmdShow3 = SW_SHOW;
	int nCmdShow4 = SW_SHOW;
	int nCmdShow5 = SW_SHOW;
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
//	if(bExt3)
//	{
//		m_chkExt3.SetToolTipText("��ư�׷쿭��");
//		nCmdShow3 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt3.SetToolTipText("��ư�׷�ݱ�");
//	}
//	if(bExt4)
//	{
//		m_chkExt4.SetToolTipText("��ư�׷쿭��");
//		nCmdShow4 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt4.SetToolTipText("��ư�׷�ݱ�");
//	}
//	if(bExt5)
//	{
//		m_chkExt5.SetToolTipText("��ư�׷쿭��");
//		nCmdShow5 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt5.SetToolTipText("��ư�׷�ݱ�");
//	}
	m_btSave.ShowWindow(nCmdShow1);
	m_btLoad.ShowWindow(nCmdShow1);
	m_btPrint.ShowWindow(nCmdShow1);

//	m_btChart.ShowWindow(nCmdShow2);
	m_btIndex.ShowWindow(nCmdShow2);
//	m_btAddIndex.ShowWindow(nCmdShow2);
//	m_btSetEnvir.ShowWindow(nCmdShow2);
//	m_btAnalysisType.ShowWindow(nCmdShow2);
//	m_btEtcMenu.ShowWindow(nCmdShow2);

	//S01 2006.09.07 {{
//@��������		m_btnChangeTool.ShowWindow(nCmdShow3);
	//	m_stBy.ShowWindow(nCmdShow3);			// 1 X 1
	//	m_btShowMatrix.ShowWindow(nCmdShow3);	// ����
	//	m_btInitSplit.ShowWindow(nCmdShow3);	// ���Һ���
	m_stSetMatrix.ShowWindow(nCmdShow3);		
	m_btExPrev.ShowWindow(nCmdShow3);
	m_btExNext.ShowWindow(nCmdShow3);
	//	m_chkJongmokLink.ShowWindow(nCmdShow4);		// ���񿬵�
	//	m_chkTypeLink.ShowWindow(nCmdShow4);		// �ֱ⿬��
	//	m_chkUpdateIndex.ShowWindow(nCmdShow4);		// ��ǥ����

	m_btInitial.ShowWindow(nCmdShow5);
	m_chkShowScale.ShowWindow(nCmdShow5);
	m_chkFixPeriod.ShowWindow(nCmdShow5);		// �Ⱓ����
	//S01 2006.09.07 }}

// for Pattern Test - SeoJeong - 
#ifdef _DEBUG
//	m_btAnalysisPattern.ShowWindow(nCmdShow5);
//	m_btChartGuideLine.ShowWindow(nCmdShow5);
//	GetDlgItem(IDC_BTN_ANALIZE_PERIOD)->ShowWindow(nCmdShow5);
#else
//	m_btAnalysisPattern.ShowWindow(SW_HIDE);
//	m_btAnalysisPattern.ShowWindow(nCmdShow5);
//	m_btChartGuideLine.ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_BTN_ODRTRACE)->ShowWindow(SW_HIDE);
#endif

	
	m_btHist.ShowWindow(nCmdShow5);
	m_btST.ShowWindow(nCmdShow5);


	CRect rect;
	int nShiftExt12 = 63;//63;
	int nShiftExt23 = 230;//210;//188;//142;//116;
	int nShiftExt34 = 208;//159;//117;//133;
	int nShiftExt45 = 140;//64;
	if(nAct==1)		
	{
//		if(bExt1)
//		{
//			nShiftExt12 *= -1;
//		}
//		SHIFTCONTROL(m_chkExt2,nShiftExt12,rect);
//		SHIFTCONTROL(m_btChart,nShiftExt12,rect);
		SHIFTCONTROL(m_btIndex,nShiftExt12,rect);
		SHIFTCONTROL(m_btAddIndex,nShiftExt12,rect);
//		SHIFTCONTROL(m_btSetEnvir,nShiftExt12,rect);
//		SHIFTCONTROL(m_btAnalysisType,nShiftExt12,rect);
//		SHIFTCONTROL(m_btEtcMenu,nShiftExt12,rect);

//		SHIFTCONTROL(m_chkExt3,nShiftExt12,rect);
		SHIFTCONTROL(m_stSetMatrix,nShiftExt12,rect);
		SHIFTCONTROL(m_stBy,nShiftExt12,rect);
		SHIFTCONTROL(m_btShowMatrix,nShiftExt12,rect);
		SHIFTCONTROL(m_btExPrev,nShiftExt12,rect);
		SHIFTCONTROL(m_btExNext,nShiftExt12,rect);
		SHIFTCONTROL(m_btInitSplit,nShiftExt12,rect);

//		SHIFTCONTROL(m_chkExt4,nShiftExt12,rect);
		SHIFTCONTROL(m_chkJongmokLink,nShiftExt12,rect);
		SHIFTCONTROL(m_chkTypeLink,nShiftExt12,rect);
		SHIFTCONTROL(m_chkUpdateIndex,nShiftExt12,rect);
		
//		SHIFTCONTROL(m_chkExt5,nShiftExt12,rect);
//001 2006.06.22		SHIFTCONTROL(m_btInitial,nShiftExt12,rect);
		SHIFTCONTROL(m_chkShowScale,nShiftExt12,rect);
		SHIFTCONTROL(m_btAnalysisPattern,nShiftExt12,rect);
		SHIFTCONTROL(m_btChartGuideLine,nShiftExt12,rect);
		SHIFTCONTROL(m_btHist,nShiftExt12,rect);
		SHIFTCONTROL(m_btST,nShiftExt12,rect);
	}
	else if(nAct==2)
	{
//		if(bExt2)
//		{
//			nShiftExt23 *= -1;
//		}
//
//		SHIFTCONTROL(m_chkExt3,nShiftExt23,rect);
		SHIFTCONTROL(m_stSetMatrix,nShiftExt23,rect);
		SHIFTCONTROL(m_stBy,nShiftExt23,rect);
		SHIFTCONTROL(m_btShowMatrix,nShiftExt23,rect);
		SHIFTCONTROL(m_btExPrev,nShiftExt23,rect);
		SHIFTCONTROL(m_btExNext,nShiftExt23,rect);
		SHIFTCONTROL(m_btInitSplit,nShiftExt23,rect);

//		SHIFTCONTROL(m_chkExt4,nShiftExt23,rect);
		SHIFTCONTROL(m_chkJongmokLink,nShiftExt23,rect);
		SHIFTCONTROL(m_chkTypeLink,nShiftExt23,rect);
		SHIFTCONTROL(m_chkUpdateIndex,nShiftExt23,rect);
		
//		SHIFTCONTROL(m_chkExt5,nShiftExt23,rect);

//001 2006.06.22		SHIFTCONTROL(m_btInitial,nShiftExt23,rect);
		SHIFTCONTROL(m_chkShowScale,nShiftExt23,rect);
		SHIFTCONTROL(m_btAnalysisPattern,nShiftExt23,rect);
		SHIFTCONTROL(m_btChartGuideLine,nShiftExt23,rect);
		SHIFTCONTROL(m_btHist,nShiftExt23,rect);
		SHIFTCONTROL(m_btST,nShiftExt23,rect);

	}
	else if(nAct==3)
	{
//		if(bExt3)
//		{
//			nShiftExt34 *= -1;
//		}
//
//		SHIFTCONTROL(m_chkExt4,nShiftExt34,rect);
		SHIFTCONTROL(m_chkJongmokLink,nShiftExt34,rect);
		SHIFTCONTROL(m_chkTypeLink,nShiftExt34,rect);
		SHIFTCONTROL(m_chkUpdateIndex,nShiftExt34,rect);
		
//		SHIFTCONTROL(m_chkExt5,nShiftExt34,rect);
//001 2006.06.22		SHIFTCONTROL(m_btInitial,nShiftExt34,rect);
		SHIFTCONTROL(m_chkShowScale,nShiftExt34,rect);
		SHIFTCONTROL(m_btAnalysisPattern,nShiftExt34,rect);
		SHIFTCONTROL(m_btChartGuideLine,nShiftExt34,rect);
		SHIFTCONTROL(m_btHist,nShiftExt34,rect);
		SHIFTCONTROL(m_btST,nShiftExt34,rect);
	}
	else if(nAct==4)
	{
//		if(bExt4)
//		{
//			nShiftExt45 *= -1;
//		}
//
//		SHIFTCONTROL(m_chkExt5,nShiftExt45,rect);
//001 2006.06.22		SHIFTCONTROL(m_btInitial,nShiftExt45,rect);
		SHIFTCONTROL(m_chkShowScale,nShiftExt45,rect);
		SHIFTCONTROL(m_btAnalysisPattern,nShiftExt45,rect);
		SHIFTCONTROL(m_btChartGuideLine,nShiftExt45,rect);
		SHIFTCONTROL(m_btHist,nShiftExt45,rect);
		SHIFTCONTROL(m_btST,nShiftExt45,rect);

	}
	Invalidate();
	EnableWindow();
}

void CExtInputBarDlg::OnChkjongmoklink() 
{
	// TODO: Add your control notification handler code here
	if(m_chkJongmokLink.GetCheck())
	{
/*		m_chkJongmokLink.SetToolTipText("���񿬵�����");
		m_chkTypeLink.SetCheck(0);
		m_chkTypeLink.SetToolTipText("�ð���������");
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_SETTYPELINK,(WPARAM)m_chkTypeLink.GetCheck());
*/
	}
	else
	{
//		m_chkJongmokLink.SetToolTipText("���񿬵�����");
	}	
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SETJONGMOKLINK,(WPARAM)m_chkJongmokLink.GetCheck());
}

void CExtInputBarDlg::OnChktypelink() 
{
	// TODO: Add your control notification handler code here
/*	if(m_chkTypeLink.GetCheck())
	{
		m_chkTypeLink.SetToolTipText("�ð���������");
		m_chkJongmokLink.SetCheck(0);
		m_chkJongmokLink.SetToolTipText("���񿬵�����");
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_SETTYPELINK,(WPARAM)m_chkJongmokLink.GetCheck());
	}
	else
	{
		m_chkTypeLink.SetToolTipText("�ð���������");
	}
*/	
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SETTYPELINK,(WPARAM)m_chkTypeLink.GetCheck());	
}

void CExtInputBarDlg::OnBtload() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	CPoint pt;
	GetDlgItem(IDC_BTLOAD)->GetWindowRect(rect);
	pt.x = rect.left;
	pt.y = rect.bottom;

	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_LOADCHART,0,(LPARAM)&pt);	
}

void CExtInputBarDlg::OnBtsave() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	CPoint pt;
	GetDlgItem(IDC_BTSAVE)->GetWindowRect(rect);
	pt.x = rect.left;
	pt.y = rect.bottom;
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SAVECHART,0,(LPARAM)&pt);	
}


void CExtInputBarDlg::OnBtinitial() 
{
	m_bEndDate	= TRUE;
	m_strStartDate	= _T("");
	m_strEndDate	= _T("");
	
	// TODO: Add your control notification handler code here
	CString strMessage = "��ü ��Ʈ�� ȯ���� �ʱ�ȭ �˴ϴ�. ��� �Ͻðڽ��ϱ�?";
	if(MessageBox(strMessage, "��Ʈ ����", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_RESETCHART);

		//StdDialog���� ��Ʈ �ʱ�ȭ �߰�
		GetParent()->SendMessage(UMSG_RESETCHART);
	}	
}

//void CExtInputBarDlg::OnBtaddindex() 
//{
//	// TODO: Add your control notification handler code here
//	m_pChartMng->SendMessage(UMSG_SHOWINDICATORCONFIGDLG);
//}

void CExtInputBarDlg::OnDestroy() 
{	
	// TODO: Add your message handler code here
	if(m_MatrixSelecter.GetSafeHwnd() != NULL)
		m_MatrixSelecter.DestroyWindow();
	m_brBkColor.DeleteObject();
	
	if(((CStdDialog*)GetParent())->m_strConfig == m_strCfgFile)
	{
		CString strTemp;
/*
		UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);
		//nOpenedOwner  = 0 : �޴�/���ٿ��� ����
		//nOpenedOwner  = 1 : ���콺 ������ ��ư���� ���� (�����ڵ� ����)
		//nOpenedOwner  = 2 : ��ũ��Ʈ,dll���� ȭ�� ��û (OpenMapData ����)
		if(nOpenedOwner==0&&m_nSave)
		{
			if(m_nSave==SAVE_TRUE)
			{
				strTemp = "1";
			}
			else if(m_nSave==SAVE_FALSE)
			{
				strTemp = "0";
			}
			::WritePrivateProfileString("ExtInputBar","Saved",strTemp,m_strCfgFile);
		}
*/
		strTemp.Format("%d", m_nDragModeType);
		::WritePrivateProfileString("ExtInputBar","DragModeType",strTemp,m_strCfgFile);

		strTemp.Format("%d", m_bChkSaveJipyoOneFile);
		::WritePrivateProfileString("ExtInputBar","SaveJipyoOneFile",strTemp,m_strCfgFile);	

		strTemp.Format("%d", m_bChkRegGwansimUp);
		::WritePrivateProfileString("ExtInputBar","RegGwansimUp",strTemp,m_strCfgFile);	

		// �м��� ���� ���� - ojtaso (20080723)
		strTemp.Format("%d", m_bChkSaveAnalTool);
		::WritePrivateProfileString("ExtInputBar","SaveAnalTool",strTemp,m_strCfgFile);	

		// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
		strTemp.Format("%d", m_bChkShareAnalTool);
		::WritePrivateProfileString("ExtInputBar","ShareAnalTool",strTemp,m_strCfgFile);	
		// 2011.01.12 by SYS 
	}

	CRscDlg::OnDestroy();	
}

void CExtInputBarDlg::OnChkshowscale() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SHOWSCALE,m_chkShowScale.GetCheck());
}

/*
void CExtInputBarDlg::OnBtupdateindex() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_UPDATEINDEX,m_chkShowScale.GetCheck());
}*/

HBRUSH CExtInputBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CExtInputBarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam) 
		{
		case VK_RETURN :
			{
// --> [Edit]  ������ 2008/11/02		( �ð���Ʈ�ѿ��� ����Ű �Է½� ��ȸ�� ���ش�. )
				if ( pMsg->hwnd == GetDlgItem(IDC_TIMEPICKER)->GetSafeHwnd() )
				{
//					GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
					((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AllChartItemCodeRequest();
				}
// <-- [Edit]  ������ 2008/11/02
				else if(m_edAccPW.GetSafeHwnd()==pMsg->hwnd)
				{
					m_edAccPW.GetWindowText(m_strPassWord);
					SendAccountToPlatform(m_strAccountNo, m_strPassWord);

					BOOL bOrderLine = m_chkOrderLine.GetCheck();
					if( bOrderLine )
					{
						((CStdDialog*)GetParent())->SetOrderLineInfo(bOrderLine);
						if(m_pChartMng->GetSafeHwnd())
							m_pChartMng->SendMessage(UMSG_SETORDERLINK,(WPARAM)bOrderLine);	
					}
				}

				return FALSE;
			}
			break;
		case VK_ESCAPE :
			{
				return FALSE;			
			}
			break;
		}		
		break;
	}
	if(pMsg->wParam == VK_HOME)
		return FALSE;
	
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);	
	
}

/*
void CExtInputBarDlg::OnChkrevised() 
{
	// TODO: Add your control notification handler code here
	m_pChartMng->SendMessage(UMSG_UPDATEREVISED,m_chkRevised.GetCheck());
}
*/

void CExtInputBarDlg::OnBtprint() 
{	
	::SendMessage(((CStdDialog*)GetParent())->m_hMainFrame,WM_COMMAND, (WPARAM)ID_FILE_PRINT, 0);
}

void CExtInputBarDlg::OnBtsetenvir() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SHOWENVIRDLG);	
}

void CExtInputBarDlg::OnBtshowmatrix() 
{
	// TODO: Add your control notification handler code here
// --> [Edit]  ������ 2008/10/15
	if(m_pChartMng->GetSafeHwnd())
	{
		short nType = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetCheHogaChange();
		if ( nType != 0 )
		{
			MessageBox("ȣ�� ��Ʈ�ϰ�� 1 X 1 �� �����մϴ�.");
//			AfxMessageBox("ȣ�� ��Ʈ�ϰ�� 1 X 1 �� �����մϴ�.");
			return;
		}
	}
// <-- [Edit]  ������ 2008/10/15

	if(m_MatrixSelecter.GetSafeHwnd() == NULL)
	{
		m_MatrixSelecter.Create(CMatrixSelecterDlg::IDD,GetParent());
		m_MatrixSelecter.SetpFrameDlg(this);
		m_MatrixSelecter.SetBesideWnd(&m_btShowMatrix);
		m_MatrixSelecter.SetMatrix(4,3);
	}
	m_MatrixSelecter.ShowMatrix();
}

void CExtInputBarDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_MatrixSelecter.GetSafeHwnd() != NULL)
		m_MatrixSelecter.ShowWindow(SW_HIDE);	
	CRscDlg::OnLButtonDown(nFlags, point);
}

void CExtInputBarDlg::OnBtexnext() 
{
	// TODO: Add your control notification handler code here
	((CStdDialog*)GetParent())->SetJongListNext();	
}

void CExtInputBarDlg::OnBtexprev() 
{
	// TODO: Add your control notification handler code here
	((CStdDialog*)GetParent())->SetJongListPrev();	
}

void CExtInputBarDlg::OnBtanalysispattern() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN);
}

void CExtInputBarDlg::OnBtchartguideline() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_CHARTGUIDELINE);	
}
/*
void CExtInputBarDlg::OnBtanalysisType() 
{
	// TODO: Add your control notification handler code here
    CMenu menu;
	char* szEtcMenuName[] = {	"Ȯ��/���",
								"����Ư¡�м�",
								"���ڼ�ǥ��"							
								};
	long lAllCnt = sizeof(szEtcMenuName)/sizeof(char*);
	
	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
    menu.CreatePopupMenu();
	UINT nFlags = NULL;
	
	for(int nPos = 0;nPos < lAllCnt;nPos++)
	{
		if(m_nDragModeType== nPos)
		{
			nFlags = MF_CHECKED;
		}
		else
		{
			nFlags = MF_UNCHECKED;
		}
		menu.AppendMenu(nFlags,100 + nPos,szEtcMenuName[nPos]);
	}

	CRect rect;
	m_btAnalysisType.GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
    menu.DestroyMenu();	
	if(!nRetValue) return;
	m_nDragModeType = nRetValue%100;
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SETDRAGMODETYPE,m_nDragModeType);	
}*/

void CExtInputBarDlg::OnBthist() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SETORDEREDINFO);	
	
}

void CExtInputBarDlg::OnBtst() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_LOADSYSTEMTRADING);
}

//DEL void CExtInputBarDlg::OnChkratevalue() 
//DEL {
//DEL 	BOOL bCheckRateValue = m_chkRateValue.GetCheck();
//DEL 	if(bCheckRateValue)
//DEL 	{
//DEL 		m_chkRateValue.SetWindowText("%");
//DEL 	}
//DEL 	else 
//DEL 	{
//DEL 		m_chkRateValue.SetWindowText("\\");
//DEL 	}	
//DEL 	m_pChartMng->SendMessage(UMSG_RATEVALUE,bCheckRateValue);
//DEL }

void CExtInputBarDlg::OnBtnChangeunit() 
{
	if( m_nYUnitType == WON_TICK)
	{
		m_nYUnitType = PERCENT_TICK;
		m_btYUnit.SetWindowText("%");
	}
	else if(m_nYUnitType == DOLLAR_TICK)
	{
		m_nYUnitType = WON_TICK;
		m_btYUnit.SetWindowText("\\");
	}
	else if(m_nYUnitType == PERCENT_TICK)
	{
		m_nYUnitType = DOLLAR_TICK;
		m_btYUnit.SetWindowText("$");
	}

	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_RATEVALUE, m_nYUnitType);	
}

void CExtInputBarDlg::OnBtnSdate() 
{	
// --> [Edit]  ������ 2008/11/06		//	���� ���ڸ� �������� �ʴ´�.
	return;
// <-- [Edit]  ������ 2008/11/06

	m_bEndDate = FALSE;

//	CString strImageUp		= m_strImageChartPath + "\\btn_up.bmp";
//	CString strImageDown	= m_strImageChartPath + "\\btn_down.bmp";		
//	m_btnStartDate	.SetSkin(strImageDown,strImageDown,"","","","",0,0,0);
//	m_btnEndDate	.SetSkin(strImageUp,strImageUp,"","","","",0,0,0);

	//������ ������
	m_btnStartDate	.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "��", TRUE);	
	m_btnEndDate	.FP_vSetImage(m_pIL_W20H20, 80, 80, TRUE, "��", TRUE);

	m_btnStartDate	.Invalidate();
	m_btnEndDate	.Invalidate();


	//================================================================================
	// ������ �������� �����´�. 05.09.06
	//================================================================================
	double dStartDate = 0, dEndDate = 0;
	
	((CStdDialog*)GetParent())->GetStartEndDateAtStdDialog(dStartDate, dEndDate);

	if(dStartDate)
		m_strStartDate.Format("%d", (long)dStartDate);
	if(dEndDate)
		m_strEndDate.Format("%d", (long)dEndDate);
	//================================================================================

	

	if(!m_strStartDate.GetLength())
	{
		CTime currentTime = CTime::GetCurrentTime(); 	
		//m_datePicker.SetTime(&currentTime);	
//@��������
//		CString strCurTime = currentTime.Format("%Y%m%d");
//		m_datePicker.SetDataText(strCurTime);
//@��������
//@�����߰�
		m_datePicker.SetTime(&currentTime);
//@�����߰�
		return;
	}
	
	m_strStartDate.TrimRight();

	if(m_chType == TICK_DATA_CHART) return;
	if(m_chType == YEAR_DATA_CHART) return;
	
	int nYear	= atoi(m_strStartDate.Left(4)	);	
	int nMonth	= atoi(m_strStartDate.Mid(4,2)	);
	int nDay	= atoi(m_strStartDate.Right(2)	);	


	CTime myTime(nYear,nMonth, nDay, 0, 0, 0);      		
	//m_datePicker.SetTime(&myTime); 
//@��������
//	m_datePicker.SetDataText(m_strStartDate);
//@��������
//@�����߰�
	m_datePicker.SetTime(&myTime);
//@�����߰�

	if(m_pChartMng && m_pChartMng->GetSafeHwnd())
		m_pChartMng->SendMessage( UMS_SETCHART_DATEINFO, (WPARAM)m_bEndDate, (LPARAM)atoi(m_strStartDate));
}

void CExtInputBarDlg::OnBtnEdate() 
{	

// --> [Edit]  ������ 2008/11/06		//	������ ���ڸ� �������� �ʴ´�.
	return;
// <-- [Edit]  ������ 2008/11/06

	m_bEndDate = TRUE;

	//������ ������
	m_btnStartDate	.FP_vSetImage(m_pIL_W20H20, 80, 80, TRUE, "��", TRUE);	
	m_btnEndDate	.FP_vSetImage(m_pIL_W20H20, 82, 82, TRUE, "��", TRUE);
	m_btnStartDate	.Invalidate();
	m_btnEndDate	.Invalidate();


	//================================================================================
	// ������ �������� �����´�. 05.09.06
	//================================================================================
	double dStartDate = 0, dEndDate = 0;
	
	((CStdDialog*)GetParent())->GetStartEndDateAtStdDialog(dStartDate, dEndDate);

	if(dStartDate)
		m_strStartDate.Format("%d", (long)dStartDate);
	if(dEndDate)
		m_strEndDate.Format("%d", (long)dEndDate);

	if(		(m_strStartDate	.GetLength() != 8) ||
			(m_strEndDate	.GetLength() != 8) ||	
			(atoi(m_strStartDate)<= 19700101 ) ||
			(atoi(m_strEndDate	)<= 19700101 ) 
	)
	{
		CTime currentTime = CTime::GetCurrentTime(); 
		if(m_datePicker.GetSafeHwnd()) m_datePicker.SetTime(&currentTime);
		m_strStartDate = m_strEndDate =currentTime.Format("%Y%m%d");
	}
	
	//================================================================================

	

	 /* �޷���Ʈ���� ��¥�� �ý��� ��¥�� �����ϱ� */ 
	if(!m_strEndDate.GetLength())
	{
		CTime currentTime = CTime::GetCurrentTime(); 	
		//m_datePicker.SetTime(&currentTime);	
		CString strCurTime = currentTime.Format("%Y%m%d");
//@��������
//		m_datePicker.SetDataText(strCurTime);
//@��������
//@�����߰�
		m_datePicker.SetTime(&currentTime);
//@�����߰�
		return;
	}

	m_strEndDate.TrimRight();	

	if(m_chType == TICK_DATA_CHART) return;
	if(m_chType == YEAR_DATA_CHART) return;


	int nYear	= atoi(m_strEndDate.Left(4)		);
	int nMonth	= atoi(m_strEndDate.Mid(4,2)	);
	int nDay	= atoi(m_strEndDate.Right(2)	);

	CTime myTime(nYear,nMonth, nDay, 0, 0, 0);      		
	//m_datePicker.SetTime(&myTime); 
//@��������
//	m_datePicker.SetDataText(m_strEndDate);
//@��������
//@�����߰�
	m_datePicker.SetTime(&myTime);
//@�����߰�

	if(m_pChartMng && m_pChartMng->GetSafeHwnd())
		m_pChartMng->SendMessage( UMS_SETCHART_DATEINFO, (WPARAM)m_bEndDate, (LPARAM)atoi(m_strEndDate));
}

//void CExtInputBarDlg::SetCheckRevised(BOOL bUseRevised)
//{
////	m_bRevised = bUseRevised;
//}

void CExtInputBarDlg::SetCheckRevised(BOOL bRevised)
{
	m_bChkRevised = bRevised;
	UpdateData(FALSE);
}

void CExtInputBarDlg::OnChkRevised() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pChartMng)
		m_pChartMng->SendMessage(UMSG_UPDATEREVISED, m_bChkRevised);
}


void CExtInputBarDlg::SetMarketType(int nMarketType)
{
	m_nMarketType = nMarketType;
	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		m_btYUnit.EnableWindow(TRUE);
	}
	else
	{
		m_btYUnit.EnableWindow(FALSE);
		m_nYUnitType = WON_TICK;
		m_btYUnit.SetWindowText("\\");
	}
} 

void CExtInputBarDlg::ChangeStatusDateControl(int nDataType)
{
	m_nDataType = nDataType;
	BOOL bEnable = TRUE;
	char a = nDataType + '0';
	//char cType[1];
	//itoa(nDataType, cType, 10);

	if(
		( a == TICK_DATA_CHART) ||
		( a == SEC_DATA_CHART) ||
		( a == MIN_DATA_CHART )
		)

	m_btnStartDate	.EnableWindow(bEnable);
	m_btnEndDate	.EnableWindow(bEnable);
	//m_datePicker	.EnableWindow(bEnable);		
//@��������
//	if(m_datePicker.GetEnabled() == !bEnable)
//		m_datePicker.SetEnabled(bEnable);		
//@��������
//@�����߰�
	if ( !m_bRealUpdate )
	{
		if(m_datePicker.IsWindowEnabled() == !bEnable)
			m_datePicker.EnableWindow(bEnable);		
	}

////////////////////////////////////////////////////////////////////////////////////////////
// --> [Edit]  ������ 2008/11/09	( �ӽ÷� ���� )
//	m_datePicker.EnableWindow(FALSE);
//	GetDlgItem(IDC_REAL_CHECK)->EnableWindow(FALSE);
//	GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
// <-- [Edit]  ������ 2008/11/09
////////////////////////////////////////////////////////////////////////////////////////////

//@�����߰�

	m_btnStartDate.SetWindowText("��");	
	m_btnEndDate.SetWindowText("��");
}

//void CExtInputBarDlg::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
//{	
//	LPNMDATETIMECHANGE lpChange = (LPNMDATETIMECHANGE)pNMHDR;	
//	if(m_bEndDate)
//	{
//		m_strEndDate.Format("%04d%02d%02d", lpChange->st.wYear, lpChange->st.wMonth, lpChange->st.wDay);
//		
//	}
//	else
//	{
//		m_strStartDate.Format("%04d%02d%02d", lpChange->st.wYear, lpChange->st.wMonth, lpChange->st.wDay);
//	}	
//	
//	*pResult = 0;
//}

void CExtInputBarDlg::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{	
//@��������
//	CString strDate = m_datePicker.GetDataText();
//@��������
//@�����߰�
	CString strDate;
	CTime timeTime;
	DWORD dwResult = m_datePicker.GetTime(timeTime);
	if (dwResult == GDT_VALID)
		strDate = timeTime.Format(_T("%Y%m%d"));
//@�����߰�
	if(m_bEndDate)
	{
		if (m_strEndDate != strDate)
		{
			m_strEndDate = strDate;
		}
		else
		{
			return;
		}
	}
	else
	{
		m_strStartDate = strDate;
	}	

// --> [Edit]  ������ 2008/11/02		( ���⼭ ��ȸ�� �ع����� ��ȸ �Ϸ��� ������ ���ڷ� �ٽ� �����Ǳ⶧���� ��¥ ����ÿ��� TR�� ������ �ʴ´�. )
//	if(m_bEndDate) // ������ ����̸� TR�� ������.
//	{
//		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AllChartItemCodeRequest();
//	}
// <-- [Edit]  ������ 2008/11/02

	*pResult = 0;

}

void CExtInputBarDlg::OnCloseupDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//2007.02.28 by LYH �޷¼��ý� ������ ��ȸ
	//if(	atoi(m_strStartDate) > atoi(m_strEndDate)) return;
	//}}

	if(m_bEndDate) // ������ ����̸� TR�� ������.
	{
		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
	}
	else
	{
		if(m_pChartMng)
		{
			m_pChartMng->SendMessage(UMSG_MOVE_CHART_VIEWRANGE,
								atoi(m_strStartDate),
								atoi(m_strEndDate));
		}
	}
	*pResult = 0;
}

//#ifdef __INFO
//	#define SELL_SCR_NO		"5000"
//	#define BUY_SCR_NO		"5001"
//#else
	#define SELL_SCR_NO		"8325"
	#define BUY_SCR_NO		"8326"
//#endif

void CExtInputBarDlg::OnBtnSellscr() 
{	
	
	//OpenMap(SELL_SCR_NO);
	//���� ���� �߻�
}

void CExtInputBarDlg::OnBtnBuyscr() 
{	
	//OpenMap(BUY_SCR_NO);
	//���� ���� �߻�
}


//-----------------------------------------------------------------------------
// Reason		: ȭ�� ����
//		- CMapCoreView::OpenMap(LPCTSTR szMapName, LPCTSTR szArg0, LPCTSTR szArg1) 
//			 �Լ��� ����� ��.
//-----------------------------------------------------------------------------
// start
long CExtInputBarDlg::OpenMap(LPCTSTR szMapName)
{	
	//m_pCommonBaseInfo = ((CStdDialog*)GetParent())->GetpMainBaseInfo();
	//if(m_pCommonBaseInfo && m_strCode.GetLength() == CODE_STOCK_LENGTH)
	//{		
	//	m_pCommonBaseInfo->SetSharedData2(0,0,_T("�����ڵ�"),(BYTE*)(LPTSTR)(LPCTSTR)m_strCode,CODE_STOCK_LENGTH,0);	
	//}


//	CString m_strTempMapName = szMapName;
//	if( m_strTempMapName.GetLength() >= 6)
//		m_strTempMapName = m_strTempMapName.Mid(2, 4);
//	else if( m_strTempMapName.GetLength() >= 4)
//		m_strTempMapName = m_strTempMapName.Left(4);
//	else
//		m_strTempMapName.Format("%04d", atoi((LPSTR)szMapName));
//
//	CString	strName0 = "_Open_arg0_" + m_strTempMapName;
//	CWnd* pMainWnd = AfxGetMainWnd();
//
//	pMainWnd->SendMessage(WMU_SETVAR, (WPARAM)(LPSTR)(LPCTSTR)strName0, (LPARAM)(LPSTR)"");
//
//	CString	strName1 = "_Open_arg1_" + m_strTempMapName;
//	pMainWnd->PostMessage(WMU_SETVAR, (WPARAM)(LPSTR)(LPCTSTR)strName1, (LPARAM)(LPSTR)"");
//
//	pMainWnd->PostMessage(WMU_OPEN_MAP, -1, (LPARAM)(LPCSTR)m_strTempMapName);
	return 0;
}
void CExtInputBarDlg::OnBtfixperiod() 
{
	//CString strDate;
	//CTime tmItem;
	//m_datePicker.GetTime(tmItem);
	//strDate = tmItem.Format("%Y%m%d");	

	//InputBar �� ���� ���� ZoomValue���� �����´�.
	

//	CString strZoomValue;
//	long lZoomValue = 0;
//	m_editZoomValue.GetWindowText(strZoomValue);
//	lZoomValue = atol(strZoomValue);

//	m_bChkPeriod = m_btFixPeriod.GetCheck();
//	GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 1, m_bChkPeriod);
//	if(m_bChkPeriod)
//	{
		int lEndDate = atoi(m_strEndDate);

		SYSTEMTIME tm;
		::ZeroMemory(&tm,sizeof(tm));
		if(m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART)
		{
			tm.wYear = lEndDate/10000;
			tm.wMonth = lEndDate%10000/100;
			tm.wDay = lEndDate%100;
		}
		else if(m_chType==MONTH_DATA_CHART)
		{
			tm.wYear = lEndDate/10000;
			tm.wMonth = lEndDate%10000/100;
			tm.wDay = 1;
		}

		if(m_bEndDate) // �����Ϸ� ���õǾ� ���� ��쿡�� �޷� ��Ʈ�ѿ� ���� ���Ѵ�.
		{
			//m_datePicker.SetTime(&tm);
//@��������
//			m_datePicker.SetDataText(m_strEndDate);
//@��������
//@�����߰�
			m_datePicker.SetTime(&tm);
//@�����߰�
		}

		m_pChartMng->SendMessage(UMSG_FIXDATE,0, (LPARAM)(LPCTSTR)m_strEndDate);
//	}
}

void CExtInputBarDlg::OnChkEndsave() 
{
	// TODO: Add your control notification handler code here
	if( m_chkEndSave.GetCheck() )
		m_nSave = SAVE_TRUE	;
	else
		m_nSave = SAVE_FALSE;
}

void CExtInputBarDlg::OnChkupdateindex() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_UPDATEINDEX,0);
//	if( m_chkUpdateIndex.GetCheck() )
//	{
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_UPDATEINDEX,m_chkShowScale.GetCheck());
//	}	
}

BOOL CExtInputBarDlg::GetTimeSyncFlag()
{
	return m_chkTypeLink.GetCheck();
}

void CExtInputBarDlg::SetTimeSyncFlag(BOOL bCheck)
{
	m_bReplayMode = bCheck;
	
	m_chkTypeLink.SetCheck(bCheck);	
	m_chkTypeLink.Invalidate();
}

void CExtInputBarDlg::EnableWindowExtBar(BOOL bEnable)
{
}

//void CExtInputBarDlg::OnBtnOdrtrace() 
//{		
//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//		m_pChartMng->SendMessage(UMSG_CHARTODRTRACE);
//}

void CExtInputBarDlg::OnBtnAnalizePeriod() 
{	
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_APPLY_ANALYZE_PERIOD_ADDIN);	
}

void CExtInputBarDlg::OnBtnPeriod() 
{
	// TODO: Add your control notification handler code here
	
}

void CExtInputBarDlg::OnBtetcmenu() 
{
	// TODO: Add your control notification handler code here
    CMenu menu;
	CMenu *pMenuChild = NULL;

	//char* szEtcMenuName[] = {	"������ǥ", "����", "�ҷ�����", "�����ְ�", "����� ��������", "����� ǥ��", "��ǥ ��ü����"};
	//{{����, �ҷ����� ���� by LYH 2007.01.05
	//Eugene�� ������ �����

	//20080914 �̹��� >>
	//�츮���� fxíƮ���� �����ְ� ������ ��� ���ŵǰ� ����Ʈ ����� �߰��ȴ�.	
	//char* szEtcMenuName[] = {	"������ǥ", "�����ְ�", "��ǥ ��ü����"};

	// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
	//char* szEtcMenuName[] = {"������ǥ", "��ǥ ��ü����", "�м��� ����", "��Ʈ�μ�", "��Ʈ�׸�����", "�̰�������ǥ��"};	
	char* szEtcMenuName[] = {"������ǥ", "��ǥ ��ü����", "�м��� ����", "�м��� ����", "��Ʈ�μ�", "��Ʈ�׸�����", "�̰�������ǥ��"};	
	// 2011.01.12 by SYS <<


	//20080914 �̹��� <<

 //	char* szEtcMenuName[] = {	"�����ְ�", 
 //								"����� ��Ʈ����",
 //								"�ð����� ����", 
 //								"�����Ѱ� ǥ��", 
 //								"����� ǥ��", 
 //								"�Ǹ��� ǥ��"};
 	

 	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
     menu.CreatePopupMenu();
 	UINT nFlags = NULL;
 
 	//menu.AppendMenu(MF_UNCHECKED,100,szEtcMenuName[0]);
	pMenuChild = new CMenu;
	pMenuChild->CreatePopupMenu();
	menu.AppendMenu(MF_POPUP,(UINT)pMenuChild->GetSafeHmenu(),szEtcMenuName[0]);


	CFavoriteThingsMng* pFavoriteMng = NULL;
	BOOL bExistance;
	int nFlagsChild;
	int lPosAll = 0;
	CStringArray strArrayIndicatorAll;	

	pFavoriteMng = &((CStdDialog*)GetParent())->m_FavoriteMng;

	stFvrData _stFvrData;
	BOOL	bCreateRoot = FALSE;
	if(pFavoriteMng)
	{
		pFavoriteMng->LoadItemInfo("JipyoFile");
		int wSizeItem = pFavoriteMng->m_ArrayFvrItem.GetSize();
		for(int wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
		{
			//_stData = pFavoriteMng.m_ArrayFvrItem.GetAt(wIndexItem);
			//treeItem->InsertItem(_stData.m_chData1);
			_stFvrData = pFavoriteMng->m_ArrayFvrItem.GetAt(wIndexItem);
				//if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX&&_stFvrData.m_lSubData1==0)
				if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX)
				{
					bCreateRoot = TRUE;
					if(m_pChartMng->GetSafeHwnd())
						bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)_stFvrData.m_chData1);
					if(bExistance)
					{
						nFlagsChild = MF_CHECKED;
					}
					else
					{
						nFlagsChild = MF_UNCHECKED;
					}
					pMenuChild->AppendMenu(nFlagsChild,200+lPosAll,_stFvrData.m_chData1);
					lPosAll++;
					strArrayIndicatorAll.Add(_stFvrData.m_chData1);
				}

		}		

//		pFavoriteMng->LoadGrpInfo();
//		DWORD wSizeGrp = pFavoriteMng->m_strArrayGrp.GetSize();
//		DWORD wSizeItem = 0;
//		HTREEITEM  hRootItem = NULL;
//		HTREEITEM  hItem = NULL;
//		CString strGrpName;
//		CString strItemName;
//		DWORD wIndexGrp = 0;
//		DWORD wIndexItem = 0;
//		stFvrData _stFvrData;
//		BOOL	bCreateRoot = FALSE;
//		//for(wIndexGrp=0;wIndexGrp<wSizeGrp;wIndexGrp++)
//		//{
//			//bCreateRoot = FALSE;
//			//strGrpName  = pFavoriteMng->m_strArrayGrp.GetAt(wIndexGrp);
//			pFavoriteMng->LoadItemInfo("JipyoFile");
//			wSizeItem = pFavoriteMng->m_ArrayFvrItem.GetSize();
//			for(wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
//			{
//				_stFvrData = pFavoriteMng->m_ArrayFvrItem.GetAt(wIndexItem);
//				if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX&&_stFvrData.m_lSubData1==0)
//				{
//					if(!nCntAddedByFavorite)
//					{
//						menu.AppendMenu(MF_SEPARATOR);
//						nCntAddedByFavorite++;
//					}
//					bCreateRoot = TRUE;
//					if(m_pChartMng->GetSafeHwnd())
//						bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)_stFvrData.m_chData1);
//					if(bExistance)
//					{
//						nFlagsChild = MF_CHECKED;
//					}
//					else
//					{
//						nFlagsChild = MF_UNCHECKED;
//					}
//					pMenuChild->AppendMenu(nFlagsChild,200+lPosAll,_stFvrData.m_chData1);
//					nCntAddedByFavorite++;
//					lPosAll++;
//					strArrayIndicatorAll.Add(_stFvrData.m_chData1);
//				}
//			}		
		//}
	}

	//{{����, �ҷ����� ���� by LYH 2007.01.05
	//menu.AppendMenu(MF_UNCHECKED,101,szEtcMenuName[1]);
	//menu.AppendMenu(MF_UNCHECKED,102,szEtcMenuName[2]);
	//}}


	//20080914 �̹��� >>
	//�츮���� fxíƮ���� �����ְ� ������ ��� ����
 	//if(m_bChkRevised)
 	//{
 	//	nFlags = MF_CHECKED;
 	//}
 	//else
 	//{
 	//	nFlags = MF_UNCHECKED;
 	//}
	//menu.AppendMenu(nFlags,101,szEtcMenuName[1]);
	//20080914 �̹��� <<
 

// 	if(m_nSave)
//	{
// 		if(m_nSave == 1)
//		{
// 			nFlags = MF_CHECKED;
// 		}
// 		else if(m_nSave == 2)
// 		{
// 			nFlags = MF_UNCHECKED;
// 		}		
// 	}
// 	else
// 	{
// 		nFlags = MF_GRAYED;
// 	}
// 	menu.AppendMenu(nFlags,102,szEtcMenuName[2]);		
 
 	//if(m_bUseNormalizedMinuteTimeRule)
 	//{
 	//	if(m_bNormalizedMinuteTimeRule)
 	//	{
 	//		nFlags = MF_CHECKED;
 	//	}
 	//	else
 	//	{
 	//		nFlags = MF_UNCHECKED;
 	//	}
 	//	menu.AppendMenu(nFlags,105,szEtcMenuName[5]);
 	//}


	//{{JS.Kim_20100903 Solomon ��ǥ��ü�������� �ϸ� �߼����� ������ �ȵǴ� ������ �־� �ϴ� �޴����� ����
	/*
	if(m_bChkSaveJipyoOneFile)
	{
 		nFlags = MF_CHECKED;
 	}
 	else
 	{
 		nFlags = MF_UNCHECKED;
 	}

// --> [Edit]  ������ 2008/11/08		( 2���� �̻��ϰ�� ��ǥ ��ü���� �޴��� Disable�� �ش޶�� ��û )
	int nChartCnt = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetChartItemCount();
	if ( nChartCnt > 1 || GetTimeSyncFlag() )
		nFlags = MF_DISABLED | MF_GRAYED;
// <-- [Edit]  ������ 2008/11/08

	//20080914 �̹��� >>
	//�츮���� fxíƮ���� �����ְ� ������ ��� �����ϰ� �޴��� �ϳ��� ���� �ø���.
 	//menu.AppendMenu(nFlags,102,szEtcMenuName[2]);
	menu.AppendMenu(nFlags,102,szEtcMenuName[1]);
	//20080914 �̹��� <<
	*/

	// �м��� ���� ���� - ojtaso (20080723)
	if(m_bChkSaveAnalTool)
	{
 		nFlags = MF_CHECKED;
 	}
 	else
 	{
 		nFlags = MF_UNCHECKED;
 	}
 	menu.AppendMenu(nFlags,103,szEtcMenuName[2]);


	// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
	//20080914 �̹��� >>
	//nFlags = MF_UNCHECKED;
	//menu.AppendMenu(nFlags,106,szEtcMenuName[3]);//íƮ�μ�
	//menu.AppendMenu(nFlags,107,szEtcMenuName[4]);//íƮ�׸�����
	//20080914 �̹��� <<

	// �м��� ����
	if (m_bChkSaveAnalTool)
	{
		if(m_bChkShareAnalTool)
			nFlags = MF_CHECKED;
		else
			nFlags = MF_UNCHECKED;
	}
	else
	{
		// �м��� ���� ����� üũ�� �ȵǾ� ������ disable �Ѵ�.
		nFlags = MF_UNCHECKED | MF_DISABLED;
	}
	// 2010.01.18�� ���� ��ɰ� �����ϰ� �ϱ� ���� �м��� ���� ��� ����.
	menu.AppendMenu(nFlags,104,szEtcMenuName[3]);

	//20080914 �̹��� >>
	nFlags = MF_UNCHECKED;
	menu.AppendMenu(nFlags,106,szEtcMenuName[4]);//íƮ�μ�
	menu.AppendMenu(nFlags,107,szEtcMenuName[5]);//íƮ�׸�����
	//20080914 �̹��� <<
	// 2011.01.12 by SYS <<



	/* solomon
	CStdDialog* pStdDialog = (CStdDialog*)GetParent();
	if (pStdDialog->m_bMigyulcheDraw)
	{
 		nFlags = MF_CHECKED;
 	}
 	else
 	{
 		nFlags = MF_UNCHECKED;
 	}
 	menu.AppendMenu(nFlags,108,szEtcMenuName[5]);
	*/

// 	if(m_bShowLock)
// 	{
// 		nFlags = MF_CHECKED;
// 	}
// 	else
// 	{
// 		nFlags = MF_UNCHECKED;
// 	}
// 	menu.AppendMenu(nFlags,105,szEtcMenuName[5]);
 
 	CRect rect;
 	m_btEtcMenu.GetWindowRect(rect);
 	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);

	pMenuChild = menu.GetSubMenu(0);
	if(pMenuChild)
	{
		pMenuChild->DestroyMenu();
		delete pMenuChild;
	}
    menu.DestroyMenu();	
	if(nRetValue >= 200)
	{
		m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strArrayIndicatorAll.GetAt(nRetValue-200));
		return;
	}

 	switch(nRetValue)
 	{
//	case 101:
//		OnBtsave();
//		break;
//	case 102:
//		OnBtload();
//		break;
 	case 101:
		m_bChkRevised = !m_bChkRevised;
 		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 			m_pChartMng->SendMessage(UMSG_UPDATEREVISED,m_bChkRevised);
 		break;
// 	case 104:
// 		if(m_nSave==1)
// 		{
// 			m_nSave = 2;
// 		}
// 		else if(m_nSave==2)
// 		{
// 			m_nSave = 1;
// 		}
// 		break;
 	//case 105:
 	//	m_bNormalizedMinuteTimeRule = !m_bNormalizedMinuteTimeRule;
 	//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 	//		m_pChartMng->SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,m_bNormalizedMinuteTimeRule);
 	//	break;
	case 102:
 		m_bChkSaveJipyoOneFile = !m_bChkSaveJipyoOneFile;
 		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 			m_pChartMng->SendMessage(UMSG_SAVEJIPYOONEFILE,m_bChkSaveJipyoOneFile);
 		break;

	// �м��� ���� ���� - ojtaso (20080723)
	case 103:
		// 2011.01.31 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
		{
// 			BOOL bShareChange = FALSE;
// 
// 			// �������°� : �߼������� �� �߼������� ����� üũ�� �����̸�,
// 			// �߼��������� �����ǹǷ�, 
// 			// ����, �߼������� ����� ���� �׷��� �߼��� ������ �����, 
// 			// ���� ���õ� �߱��� �߼��������� �׸���.
// 			if (m_bChkSaveAnalTool && m_bChkShareAnalTool)
// 			{
// 				bShareChange = TRUE;
// 
// 				// ���� �׷��� �߼������� ����..
// 				ChangeAnalToolFile(TRUE);
// 
// 				// ���� �߼��� ��� �����...
// 				((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DELETE_ALL, TRUE);
// 			}			
// 			
			//
 			m_bChkSaveAnalTool = !m_bChkSaveAnalTool;
 			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 				m_pChartMng->SendMessage(UMSG_SAVEJIPYOONEFILE,m_bChkSaveAnalTool, 100);

			if (!m_bChkSaveAnalTool)
			{
				// �м��� ���� ����� üũ ������ => �м��� ���� ��ɵ� �ڵ� üũ �����Ѵ�.
				m_bChkShareAnalTool = FALSE;
				if(m_pChartMng->GetSafeHwnd()) 
					m_pChartMng->SendMessage(UMSG_SET_ALALTOOL_SHAREFILE,m_bChkShareAnalTool, 0);
			}

// 			if (bShareChange)
// 			{
// 				// ���� �ֱ��� �߼������Ϸ� �߼����� �ٽ� �׸���.
// 				ChangeAnalToolFile(FALSE);
// 			}
		}
 		break;

	case 104:
		{
			// ���� �׷��� �߼������� ����..
// 			ChangeAnalToolFile(TRUE);
// 
// 			// ���� �߼��� ��� �����...
// 			((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DELETE_ALL, TRUE);
// 			
			m_bChkShareAnalTool = !m_bChkShareAnalTool;
			if(m_pChartMng->GetSafeHwnd()) 
				m_pChartMng->SendMessage(UMSG_SET_ALALTOOL_SHAREFILE,m_bChkShareAnalTool, 0);
			
			if (m_bChkShareAnalTool)
			{
				// �м��� ���� ����� üũ ���¸� => �м��� ���� ��ɵ� �ڵ� üũ�Ѵ�.
				m_bChkSaveAnalTool = TRUE;
				if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					m_pChartMng->SendMessage(UMSG_SAVEJIPYOONEFILE,m_bChkSaveAnalTool, 100);
			}

			// �߼������� ������ �߼����� �׸���.
//			ChangeAnalToolFile(FALSE);
		}
		break;
	// 2011.01.31 by SYS 

	//20080914 �̹��� íƮ�μ�, íƮ�׸�����>>
	case 106:
		{
			if(m_pChartMng && IsWindow(m_pChartMng->GetSafeHwnd()))
				m_pChartMng->SendMessage(UMSG_CHART_CONTAINER_PRINT);
		}
		break;
	case 107:
		{
			if(m_pChartMng && IsWindow(m_pChartMng->GetSafeHwnd()))
				m_pChartMng->SendMessage(UMSG_CHART_IMG_SAVE);
		}
		break;
	/* solomon
	//20080914 �̹��� <<
	case 108:
		{
			((CStdDialog*)GetParent())->m_bMigyulcheDraw = !((CStdDialog*)GetParent())->m_bMigyulcheDraw;
			if(m_pChartMng && IsWindow(m_pChartMng->GetSafeHwnd()))
				m_pChartMng->SendMessage(UMSG_CHART_MIGYULCHE, ((CStdDialog*)GetParent())->m_bMigyulcheDraw);
		}
		break;
	*/
  	}
}


//@��������	
//void CExtInputBarDlg::OnBtSub1() 
//{
//	// TODO: Add your control notification handler code here
//	BOOL bCheck = m_btSub1.GetCheck();
//	if(bCheck)
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//		m_btSub2.SetCheck(FALSE);
//		m_btSub3.SetCheck(FALSE);
//		m_btSub8.SetCheck(FALSE);
//		m_btSub9.SetCheck(FALSE);
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_LINE, TRUE);
//	}
//	else
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	}
//}
//@��������	

void CExtInputBarDlg::OnBtSub2() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = m_btSub2.GetCheck();
	if(bCheck)
	{
		//((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
		//m_btSub1.SetCheck(FALSE);
		//m_btSub3.SetCheck(FALSE);
		//m_btSub8.SetCheck(FALSE);
		//m_btSub9.SetCheck(FALSE);
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, TRUE);
	}
	else
	{
		//((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, FALSE);
	}
}

//@��������	
//void CExtInputBarDlg::OnBtSub3() 
//{
//	// TODO: Add your control notification handler code here
//	BOOL bCheck = m_btSub3.GetCheck();
//	if(bCheck)
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//		m_btSub1.SetCheck(FALSE);
//		m_btSub2.SetCheck(FALSE);
//		m_btSub8.SetCheck(FALSE);
//		m_btSub9.SetCheck(FALSE);
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_HORZ, TRUE);
//	}
//	else
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	}
//}
//
//void CExtInputBarDlg::OnBtSub4() 
//{
//	// TODO: Add your control notification handler code here
//	m_btSub1.SetCheck(FALSE);
//	m_btSub2.SetCheck(FALSE);
//	m_btSub3.SetCheck(FALSE);
//
//	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DELETE_ALL, TRUE);
//}
//
//void CExtInputBarDlg::OnBtSub5() 
//{
//	// TODO: Add your control notification handler code here
//	// TODO: Add your control notification handler code here
//	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	m_btSub1.SetCheck(FALSE);
//	m_btSub2.SetCheck(FALSE);
//	m_btSub3.SetCheck(FALSE);
//	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CHART_INVERT, TRUE);
//}
//
//void CExtInputBarDlg::OnBtSub6() 
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);	
//	BOOL bCheck = m_btSub6.GetCheck();
//	::SendMessage(GetParent()->m_hWnd, USMG_SHOW_DATAWND, 0, (LPARAM)bCheck);
//}
//@��������	


void CExtInputBarDlg::OnChkfixperiod() 
{
	// TODO: Add your control notification handler code here
	if(m_chkFixPeriod.GetCheck())
	{
		//================================================================================
		// ������ �������� �����´�. 05.09.06
		//================================================================================
		double dStartDate = 0, dEndDate = 0;
		
		((CStdDialog*)GetParent())->GetStartEndDateAtStdDialog(dStartDate, dEndDate);

		if(dStartDate)
			m_strStartDate.Format("%d", (long)dStartDate);
		if(dEndDate)
			m_strEndDate.Format("%d", (long)dEndDate);
		//================================================================================OnBtnEdate();
//		CString strDate;
//		CTime tmItem;
//		m_datePicker.GetTime(tmItem);
//		strDate = tmItem.Format("%Y%m%d");
		CString strZoomValue;
		//long lZoomValue = 0;
		int lZoomValue = m_pChartMng->SendMessage(UMSG_GETZOOMVALUE, m_nDataType);
	//	m_editZoomValue.GetWindowText(strZoomValue);
		//lZoomValue = atol(strZoomValue);
		m_pChartMng->SendMessage(UMSG_FIXDATE,(WPARAM)lZoomValue, (LPARAM)(LPCTSTR)m_strEndDate);
	}
}

//@��������
//void CExtInputBarDlg::OnBtinitialPeriod() 
//{
//	// TODO: Add your control notification handler code here
//	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DISPLAY_RESTORE, TRUE);
//	m_pChartMng->SendMessage(UMSG_SETZOOMVALUE,120);
//}
//@��������

//001 2006.06.22 {{ EUGENE��Ʈ���� �̺�Ʈ�� �޴´�
BOOL CExtInputBarDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDC_BTETCMENU)			///> Button
	{
		switch (HIWORD(wParam))
		{
		case BN_CLICKED :
			TRACE("ExtInputBarDlg : BUTTON BN_CLICKED") ;
//			OnBtetcmenu();
			break;
		case BN_SETFOCUS :
			TRACE("ExtInputBarDlg : BUTTON BN_SETFOCUS") ;
			OnBtetcmenu();
			break;
		case BN_KILLFOCUS :
			TRACE("ExtInputBarDlg : BUTTON BN_KILLFOCUS") ;
			break;
		}
	} 
	
	return CDialog::OnCommand(wParam, lParam);
}
//001 2006.06.22 }}

//@��������	
//S01 2006.09.07 {{ Tool ����
//void CExtInputBarDlg::OnBtnChangeTool() 
//{
//	BOOL bInterlockShow;
//	BOOL bToolShow;
//	if(m_bChangeTool)	// Tool Button �� ������
//	{
//		bInterlockShow = TRUE;
//		m_btSub1.ShowWindow(bInterlockShow);
//		m_btSub2.ShowWindow(bInterlockShow);
//		m_btSub3.ShowWindow(bInterlockShow);
//		m_btSub4.ShowWindow(bInterlockShow);
//		m_btSub5.ShowWindow(bInterlockShow);
//		m_btSub6.ShowWindow(bInterlockShow);
//		m_btSub7.ShowWindow(bInterlockShow);
//		m_btSub8.ShowWindow(bInterlockShow);
//		m_btSub9.ShowWindow(bInterlockShow);
//		m_btSub10.ShowWindow(bInterlockShow);
//		m_btSub11.ShowWindow(bInterlockShow);
//		m_btSub12.ShowWindow(bInterlockShow);
//		m_btSub13.ShowWindow(bInterlockShow);
//		m_btSub14.ShowWindow(bInterlockShow);
//		//		m_btYUnit.ShowWindow(bInterlockShow);
//		bToolShow = FALSE;
//		m_stBy.ShowWindow(bToolShow);				// 1 X 1
//		m_btShowMatrix.ShowWindow(bToolShow);		// ����
//		m_btInitSplit.ShowWindow(bToolShow);		// ���Һ���
//		m_chkJongmokLink.ShowWindow(bToolShow);		// ���񿬵�
//		m_chkTypeLink.ShowWindow(bToolShow);		// �ֱ⿬��
//		m_btFixPeriod.ShowWindow(bToolShow);		// �Ⱓ����
//		m_chkUpdateIndex.ShowWindow(bToolShow);		// ��ǥ����	
//		m_bChangeTool = FALSE;
//	}
//	else	// ���� ��ư�� ������
//	{
//		bInterlockShow = FALSE;
//		m_btSub1.ShowWindow(bInterlockShow);
//		m_btSub2.ShowWindow(bInterlockShow);
//		m_btSub3.ShowWindow(bInterlockShow);
//		m_btSub4.ShowWindow(bInterlockShow);
//		m_btSub5.ShowWindow(bInterlockShow);
//		m_btSub6.ShowWindow(bInterlockShow);
//		m_btSub7.ShowWindow(bInterlockShow);
//		m_btSub8.ShowWindow(bInterlockShow);
//		m_btSub9.ShowWindow(bInterlockShow);
//		m_btSub10.ShowWindow(bInterlockShow);
//		m_btSub11.ShowWindow(bInterlockShow);
//		m_btSub12.ShowWindow(bInterlockShow);
//		m_btSub13.ShowWindow(bInterlockShow);
//		m_btSub14.ShowWindow(bInterlockShow);
//		//		m_btYUnit.ShowWindow(bInterlockShow);
//		bToolShow = TRUE;
//		m_stBy.ShowWindow(bToolShow);				// 1 X 1
//		m_btShowMatrix.ShowWindow(bToolShow);		// ����
//		m_btInitSplit.ShowWindow(bToolShow);		// ���Һ���
//		m_chkJongmokLink.ShowWindow(bToolShow);		// ���񿬵�
//		m_chkTypeLink.ShowWindow(bToolShow);		// �ֱ⿬��
//		m_btFixPeriod.ShowWindow(bToolShow);		// �Ⱓ����
//		m_chkUpdateIndex.ShowWindow(bToolShow);		// ��ǥ����	
//		m_bChangeTool = TRUE;
//	}
//}
//S01 2006.09.07 }}
//
//void CExtInputBarDlg::OnBtSub8() 
//{
//	BOOL bCheck = m_btSub8.GetCheck();
//	if(bCheck)
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//		m_btSub1.SetCheck(FALSE);
//		m_btSub2.SetCheck(FALSE);
//		m_btSub3.SetCheck(FALSE);
//		m_btSub9.SetCheck(FALSE);
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_VERT, TRUE);
//	}
//	else
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	}	
//}
//
//void CExtInputBarDlg::OnBtSub9() 
//{
//	BOOL bCheck = m_btSub8.GetCheck();
//	if(bCheck)
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//		m_btSub1.SetCheck(FALSE);
//		m_btSub2.SetCheck(FALSE);
//		m_btSub3.SetCheck(FALSE);
//		m_btSub8.SetCheck(FALSE);
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL_WITH_NIDLG, TRUE);
//	}
//	else
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	}
//
//}
//@��������	

/*
void CExtInputBarDlg::OnBtnGwansim() 
{
	// TODO: Add your control notification handler code here
	CMenu menu;

	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	menu.CreatePopupMenu();

	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(!pGateMng)
		return;

	IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
	if(!pMng) return;

	/////////////////////////
	CUIntArray arrIndex;
	CStringArray arrName;
	// 5. ���� �Ŵ����� ����Լ� ����� ���� ���� �����ʿ�
//	pMng->GetGwansimGroupList(arrIndex, arrName);

	int nSize = arrName.GetSize();
	char szNo[3] = {0, };

	LPCSTR szGroupName;
	UINT nGroupIndex;
	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	{
		szGroupName = (LPCSTR)arrName.GetAt(nIndx);	//���ɱ׷��̸�.
		nGroupIndex = (UINT)arrIndex.GetAt(nIndx);	//���ɱ׷��ε���.
		menu.AppendMenu(MF_STRING,200+nGroupIndex,szGroupName);
	}

	CRect rect;
	m_btnGwanSim.GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
	menu.DestroyMenu();	

	if(nRetValue >= 200)
	{
		IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
		if(!pGateMng)
			return;

		IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
		if(!pMng) return;

		/////////////////////////
		CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
		CStringArray strCodes;
		strCodes.Add(strCode);
		UINT nGroup = nRetValue-200;
		// 4. ���� �Ŵ����� ����Լ� ����� ���� ���� �����ʿ�
//		pMng->AddGwansimtem_New(nGroup, strCodes, (long)m_hWnd);
		return;
	}
}
*/
void CExtInputBarDlg::OnBtnGwansim() 
{
//@SolomonGNF
/*		// TODO: Add your control notification handler code here
		CMenu menu, pMenuChild;

 		UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
		menu.CreatePopupMenu();

		menu.AppendMenu(MF_STRING,101,"�������� ������");
		menu.AppendMenu(MF_SEPARATOR, (UINT)110, "");
		if(m_bChkRegGwansimUp)
		{
			menu.AppendMenu(MF_CHECKED,102,"  ������ �߰�");
			menu.AppendMenu(MF_UNCHECKED,103,"  �ǾƷ��� �߰�");
		}
		else
		{
			menu.AppendMenu(MF_UNCHECKED,102,"  ������ �߰�");
			menu.AppendMenu(MF_CHECKED,103,"  �ǾƷ��� �߰�");
		}

		menu.AppendMenu(MF_SEPARATOR, (UINT)110, "");
		IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
		if(!pGateMng)
			return;

		CStringArray arrKey, arrName;
		IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
		if(!pMng) return;

		//������ �׷��̸��� �����Դµ�, ������ Key�� �����´�.
		pMng->GetGwansimGroupName(arrKey, arrName);

		CString strGroup;
		if(pMng)
		{
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
	//		// ��������.
	//		//[[
	//		pMng->ShowGwansimDlg();
	//		//]]
	//
			// ������ ����. 
			//[[
			//2007.05.08 by LYH ���� ���â ��� �� ���� �ڵ� ���� �� �� �ֵ��� �����ڵ� �ѱ�(�����ڵ� �������� '|'�����ڷ� ��)
			//pMng->ShowGwansimDlg();
			CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
	//		if(strCode && strCode.GetLength() > 0)
	//			pMng->ShowGwansimDlg(strCode+"|");
	//		else
	//			pMng->ShowGwansimDlg();
			//����� �� �ִ� �ڵ����� üũ
			if (!strCode.IsEmpty() )
			{
				//BYTE cType = dll_GetJongMarket(strCode);
				//CString strType = dll_GetChangeMarketCode(cType, false);  // �ð��� �ŷ� TRUE
	//@��������	
	//			IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager);
	//			CString strType = pDataManager->GetCodeInfo(NULL, 2, strCode);
	//			BYTE ucMarketType = pDataManager->GetMarketTypeFromCode(strCode);
	//@��������	
	//@������ ���õ� �ڵ� �ѱ��� �ʴ´ٰ� ��.
	//			if( strType == "J" || strType == "Q" || strType == "F" || strType == "S" || strType == "O" || strType == "Z"
	//				|| strType == "T" || strType == "L"  || strType == "B")
	//			{			
	//				strCode += "|" ;				
	//			}
	//			else
				{
					strCode = "";
				}
				pMng->ShowGwansimDlg(this, strCode);
			}
			else
			{
				pMng->ShowGwansimDlg(this);
			}
			//]]

			//}}
		}
		else if(nRetValue == 102)
			m_bChkRegGwansimUp = TRUE;
		else if(nRetValue == 103)
			m_bChkRegGwansimUp = FALSE;
		else if(nRetValue >= 200)
		{
			CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
			CStringArray arrCodes;
			arrCodes.Add(strCode+"!");
			//arrCodes.Add(strCode);
			CString strKey, strGroup;

			strGroup = arrName.GetAt(nRetValue-200);
			strKey = pMng->GetGwanKeyFromName(strGroup);
			if(strKey.GetLength() == 0)
				return;

			if(m_bChkRegGwansimUp)
				pMng->ADDGwasimItem(strKey, arrCodes, TRUE);
			else
				pMng->ADDGwasimItem(strKey, arrCodes, FALSE);
		}
*/
}

void CExtInputBarDlg::OnBtScrollPre() 
{
	m_nScrollState = SCROLL_LEFT;

	// TODO: Add your control notification handler code here
	m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 106, 104);
	m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70);
	m_btScrollNext.Invalidate();
	m_btScrollPre.Invalidate();

	//���
	int nIdx = m_cmbScrollSpeed.GetCurSel();
	if(nIdx == -1)
	{
		nIdx = 0;
	}
	int nSpeed = (int)pow(2.0, (double)nIdx);

	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT, nSpeed);
}

void CExtInputBarDlg::OnBtScrollStop() 
{
	m_nScrollState = SCROLL_STOP;

	// TODO: Add your control notification handler code here
	m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106);
	m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70);
	m_btScrollNext.Invalidate();
	m_btScrollPre.Invalidate();
	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_STOP , TRUE);
}

void CExtInputBarDlg::OnBtScrollNext() 
{
	m_nScrollState = SCROLL_RIGHT;

	// TODO: Add your control notification handler code here
	m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 70, 68);
	m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106);
	m_btScrollNext.Invalidate();
	m_btScrollPre.Invalidate();

		//���
	int nIdx = m_cmbScrollSpeed.GetCurSel();
	if(nIdx == -1)
	{
		nIdx = 0;
	}
	int nSpeed = (int)pow(2.0, (double)nIdx);

	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT , nSpeed);
}

//@�����߰�
void CExtInputBarDlg::OnSelchangeCmbScrollSpeed() 
{
	//���
	int nIdx = m_cmbScrollSpeed.GetCurSel();
	if(nIdx == -1)
	{
		nIdx = 0;
	}
	int nSpeed = (int)pow(2.0, (double)nIdx);

	if(m_nScrollState == SCROLL_LEFT)
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT, nSpeed);
	else if(m_nScrollState == SCROLL_RIGHT)
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT, nSpeed);
}
//@�����߰�

BOOL CExtInputBarDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CExtInputBarDlg::OnPaint() 
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

void CExtInputBarDlg::ClearLink()
{
	m_chkJongmokLink.SetCheck(0);
	OnChkjongmoklink();
	m_chkTypeLink.SetCheck(0);
	OnChktypelink();
}

// �������񿡼� ���õ� ��Ʈ ���� ǥ�� : �������� - ojtaso (20070223)
void CExtInputBarDlg::InitChartMulitItem(ST_SELCHART* pSelChart)
{
	m_nYUnitType = pSelChart->m_nChartNumericValueType;
	switch(m_nYUnitType)
	{
	case WON_TYPE:
		m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 12, 13, FALSE, "\\", TRUE, 15);
		break;
	case PERCENT_TYPE:
		m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 20, 21, FALSE, "%", TRUE, 23);
		break;
	case DOLLAR_TYPE:
		m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 16, 17, FALSE, "$", TRUE, 19);
		break;
	default:
		m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 12, 13, FALSE, "\\", TRUE, 15);
	}

// --> [Edit]  ������ 2008/12/04		( ���ϰ�� �ð���Ʈ���� Ȱ��ȭ ��Ų��. )
	if ( !m_bRealUpdate )
	{
//		if ( pSelChart )
//		{
////			SetExInputBarDlgTimeIndex(pSelChart->m_nOrgTimeIndex);
//			m_nTimeIndex = pSelChart->m_nOrgTimeIndex;
//		}

		if( m_chType == DAY_DATA_CHART || m_chType == WEEK_DATA_CHART || m_chType == MONTH_DATA_CHART || m_chType == MIN_DATA_CHART)
		{
			m_datePicker.EnableWindow(TRUE);
			if ( m_chType == MIN_DATA_CHART )
				GetDlgItem(IDC_TIMEPICKER)->EnableWindow(TRUE);
			else
				GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
		}
		else
			m_datePicker.EnableWindow(FALSE);
	}
	else
	{
		m_nTimeIndex = -1;
		m_datePicker.EnableWindow(FALSE);
		GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
	}
// <-- [Edit]  ������ 2008/12/04

	m_btYUnit.Invalidate();
}

void CExtInputBarDlg::OnChkQueryOpt() 
{
	UpdateData(TRUE);
	
	if(m_pChartMng)
		m_pChartMng->SendMessage(UMSG_REQUEST_FORQUERYOPT);
}

int CExtInputBarDlg::GetQueryOpt()
{
	return m_chkQueryOpt.GetCheck();
}

void CExtInputBarDlg::EnableQueryOpt(BOOL bEnable)
{
	m_chkQueryOpt.EnableWindow(bEnable);
}

void CExtInputBarDlg::ChangeSkinColor()
{
	m_crBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.DeleteObject();
	m_brBkColor.CreateSolidBrush(m_crBk);
	Invalidate();
}

void CExtInputBarDlg::SaveExtInputBar()
{
	if(((CStdDialog*)GetParent())->m_strConfig == m_strCfgFile)
	{
		CString strTemp;
/*		UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);
		if(nOpenedOwner==0&&m_nSave)
		{
			if(m_nSave==SAVE_TRUE)
			{
				strTemp = "1";
			}
			else if(m_nSave==SAVE_FALSE)
			{
				strTemp = "0";
			}
			::WritePrivateProfileString("ExtInputBar","Saved",strTemp,m_strCfgFile);
		}
*/
		strTemp.Format("%d", m_nDragModeType);
		::WritePrivateProfileString("ExtInputBar","DragModeType",strTemp,m_strCfgFile);

		strTemp.Format("%d", m_bChkSaveJipyoOneFile);
		::WritePrivateProfileString("ExtInputBar","SaveJipyoOneFile",strTemp,m_strCfgFile);	

		strTemp.Format("%d", m_bChkRegGwansimUp);
		::WritePrivateProfileString("ExtInputBar","RegGwansimUp",strTemp,m_strCfgFile);	

		// �м��� ���� ���� - ojtaso (20080723)
		strTemp.Format("%d", m_bChkSaveAnalTool);
		::WritePrivateProfileString("ExtInputBar","SaveAnalTool",strTemp,m_strCfgFile);	

		// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
		strTemp.Format("%d", m_bChkShareAnalTool);
		::WritePrivateProfileString("ExtInputBar","ShareAnalTool",strTemp,m_strCfgFile);	
		// 2011.01.12 by SYS 
	}
}

void CExtInputBarDlg::OnChkRealUpdate() 
{
	UpdateData(TRUE);
	
	if ( m_chkRealUpdate )
	{
//		//	������ 2���� �̻��ϰܿ� ���ŵ����� ��ȸ�� ���´�. 
//		if (((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetChartItemCount() > 1)
//		{
//			m_chkRealUpdate = FALSE;
//			UpdateData(FALSE);
//
//			AfxMessageBox("��Ʈ���� 2���� �̻��� ��� ���ŵ����� ��ȸ�� �ϽǼ� �����ϴ�.");
//
//			return;
//		}

		// ����üũ �˸��� �������� ����
		CString strTemp = "";
		char szTemp[64];
		memset(szTemp, 0x00, sizeof(szTemp));
		::GetPrivateProfileString("ExtInputBar","RealCheck","0",szTemp,sizeof(szTemp),m_strCfgFile);
		strTemp = szTemp;

		if ( strTemp == "0" )
		{
			int nResult = AfxMessageBox("�ð��� �����Ͻø� ��ȸ�� �ǽð� �����͸� �����Ǽ� �����ϴ�. \n\r�˸��� �ٽ� ���� �����÷��� Ȯ���� �����ּ���.", MB_OKCANCEL );
			if ( nResult == IDOK )
				::WritePrivateProfileString( "ExtInputBar", "RealCheck", "1" , m_strCfgFile);
		}

		UpdateTimeCtrl();

// --> [Edit]  ������ 2008/12/08	( ���õ� ������ �ð��븦 �����Ѵ�. )
		ST_SELCHART* pSelChart = NULL;
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetSelChart( NULL, pSelChart);
		if ( pSelChart )
		{
//			SetExInputBarDlgTimeIndex(pSelChart->m_nOrgTimeIndex);
			if ( pSelChart->m_nTimeIndex <= 0 )
				m_nTimeIndex = pSelChart->m_nOrgTimeIndex;
			else
				m_nTimeIndex = pSelChart->m_nTimeIndex;
		}
// <-- [Edit]  ������ 2008/12/08

		if( m_chType == DAY_DATA_CHART || m_chType == WEEK_DATA_CHART || m_chType == MONTH_DATA_CHART || m_chType == MIN_DATA_CHART)
		{
			m_datePicker.EnableWindow(TRUE);

			if( m_chType == MIN_DATA_CHART )
				GetDlgItem(IDC_TIMEPICKER)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
			m_datePicker.EnableWindow(FALSE);
		}

		m_bRealUpdate = FALSE;
	}
	else
	{
		GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
		m_datePicker.EnableWindow(FALSE);
		m_bRealUpdate = TRUE;

// --> [Edit]  ������ 2008/12/03
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AllChartItemCodeRequest();

		//	üũ ������ ����ȸ�� ��û�Ѵ�.
//		CTime currentTime = CTime::GetCurrentTime(); 	
//		m_datePicker.SetTime(&currentTime);	
//		m_strEndDate = currentTime.Format("%Y%m%d");
//		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
// <-- [Edit]  ������ 2008/12/03
	}

//	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetSelChart( NULL, pSelChart);
//	if ( pSelChart )
//	{
//		pSelChart->m_nRealCheck = m_bRealUpdate
//	}

}

void CExtInputBarDlg::UpdateTimeCtrl() 
{
	CTime currentTime = CTime::GetCurrentTime(); 	
	CString strTime = "";
	strTime = currentTime.Format("%H%M%S");

	ST_SELCHART* pSelChart = NULL;
	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetSelChart( NULL, pSelChart);

	if ( pSelChart )
	{
		int nGab(0), nSeoulGab(0);
		nSeoulGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(22, 1));

		if ( pSelChart->m_nTimeIndex <= 0 )
			nGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(pSelChart->m_nOrgTimeIndex, 1));
		else
			nGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(pSelChart->m_nTimeIndex, 1));

		CString strValue = "";
		strTime = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetCalcTimeGab(strValue, strTime, (nGab - nSeoulGab));
		strTime.Replace(":", "");

		int nHour	= atoi(strTime.Left(2)	);	
		int nMin	= atoi(strTime.Mid(2,2)	);
		int nSec	= atoi(strTime.Right(2)	);	

		m_ctlTime = CTime(currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), nHour, nMin, nSec);
		m_datePicker.SetTime(&currentTime);	
	}
	else
		m_ctlTime = currentTime;

	UpdateData(FALSE);

/*
	CString strTime = ((CChartMng*)m_pChartMng)->GetSelectTime();
	strTime.Remove(':');
	if ( strTime >= 6 )
	{
		CTime myTime;
		m_datePicker.GetTime(myTime);

		int nHour	= atoi(strTime.Left(2)	);	
		int nMin	= atoi(strTime.Mid(2,2)	);
		int nSec	= atoi(strTime.Right(2)	);	

		m_ctlTime = CTime(myTime.GetYear(), myTime.GetMonth(), myTime.GetDay(), nHour, nMin, nSec);
		UpdateData(FALSE);
	}
*/
}

CString CExtInputBarDlg::GetCtlTime() 
{
	UpdateData(TRUE);
	
	CString strResult = "";
	CTime myTime;
	m_datePicker.GetTime(myTime);
	
	if ( m_chkRealUpdate )
	{
		if( m_chType == DAY_DATA_CHART || m_chType == WEEK_DATA_CHART || m_chType == MONTH_DATA_CHART )
		{
			strResult = myTime.Format("%Y%m%d");
			strResult += "9999999999";
		}
		else if ( m_chType == MIN_DATA_CHART )
		{
			strResult = myTime.Format("%Y%m%d");
			strResult += m_ctlTime.Format("%H%M00");
		}
		else
		{
			//	���� üũ�� Ǯ��������� ���� ���ڸ� ��ȸ�Ѵ�.
			myTime = CTime::GetCurrentTime();
			strResult = myTime.Format("%Y%m%d");
			strResult += "9999999999";
		}
	}
	else
	{
		//	���� üũ�� Ǯ��������� ���� ���ڸ� ��ȸ�Ѵ�.
		myTime = CTime::GetCurrentTime();
		strResult = myTime.Format("%Y%m%d");
		strResult += "9999999999";
	}

	
	return strResult;
}

void CExtInputBarDlg::OnTimepickerchange(NMHDR* pNMHDR, LRESULT* pResult) 
{	
//	CString strDate;
//	CTime timeTime;
//	DWORD dwResult = m_datePicker.GetTime(timeTime);
//	if (dwResult == GDT_VALID)
//		strDate = timeTime.Format(_T("%Y%m%d"));
////@�����߰�
//	if(m_bEndDate)
//	{
//		if (m_strEndDate != strDate)
//		{
//			m_strEndDate = strDate;
//		}
//		else
//		{
//			return;
//		}
//	}
//	else
//	{
//		m_strStartDate = strDate;
//	}	
//
//// --> [Edit]  ������ 2008/11/02		( ���⼭ ��ȸ�� �ع����� ��ȸ �Ϸ��� ������ ���ڷ� �ٽ� �����Ǳ⶧���� ��¥ ����ÿ��� TR�� ������ �ʴ´�. )
////	if(m_bEndDate) // ������ ����̸� TR�� ������.
////	{
//		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
////	}
//// <-- [Edit]  ������ 2008/11/02

	*pResult = 0;

}

void CExtInputBarDlg::SetExInputBarDlgTimeIndex(int nIndex)
{
	UpdateData(TRUE);

	if ( !m_bRealUpdate )
	{
		int nGab(0), nGab2(0);
		nGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(nIndex, 1));

		if ( m_nTimeIndex > 0 )
			nGab2 = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(m_nTimeIndex, 1));
		else
		{
			ST_SELCHART* pSelChart = NULL;
			((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetSelChart( NULL, pSelChart);
			if ( pSelChart )
				m_nTimeIndex = pSelChart->m_nOrgTimeIndex;

			if ( pSelChart->m_nOrgTimeIndex == -1 )
				return;

			nGab2 = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(m_nTimeIndex, 1));
		}


		CTimeSpan cts = CTimeSpan(0, ( nGab - nGab2 ), 0, 0);     // 1��
	
		CString strDate1 = "", strDate2 = "";
		CTime myTime;
		m_datePicker.GetTime(myTime);

		strDate1 = m_ctlTime.Format("%Y%m%d");
		m_ctlTime = m_ctlTime + cts;
		strDate2 = m_ctlTime.Format("%Y%m%d");

		int nGabDate(0);
		if ( atol(strDate1) > atol(strDate2) )
			nGabDate = -1;
		else if ( atol(strDate1) < atol(strDate2) )
			nGabDate = 1;

		cts = CTimeSpan(nGabDate, 0, 0, 0);     // 1��


		myTime = myTime + cts;
		m_datePicker.SetTime(&myTime);	

		m_nTimeIndex = nIndex; 

		UpdateData(FALSE);

	}
}

void CExtInputBarDlg::OnBtOrder() 
{
	CString strCode = ((CChartMng*)m_pChartMng)->GetCurrentCode();
	CChartItem* pChartItem = NULL;
	pChartItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();

	ST_CHARTITEM_REALDATA* pChartItemRealData;
	if(!pChartItem->m_mapChartItemRealData.Lookup(strCode, (LPVOID&)pChartItemRealData))
		return;

	int nLog(1);
	int nLogDispSize(0);
	int nPrecision(0);
	int nTickSize(1);
	int nValidPoint(0);
	int nMask(0);

	CString strPrice = "";
	strCode = CStdDialog::GetRealItemCode( ( LPCTSTR)strCode , TRUE );

	pChartItem->GetCodeConvInfo( strCode, nLog, nLogDispSize, nPrecision, nValidPoint, nMask, nTickSize);
	if ( nLog == 1 )
		strPrice = pChartItem->Gl_Format32String(atol(pChartItemRealData->m_strPrice), nLog, nLogDispSize, nPrecision, nMask);
	else if ( nLog == 8 || nLog == 32 )
		strPrice = pChartItem->Gl_Format32String(atol(pChartItemRealData->m_strPrice) * nTickSize, nLog, nLogDispSize, nPrecision, nMask);

	strPrice.Remove(',');	
	strPrice.Remove('.');
	strPrice.Remove('\'');

	int nOrderType = 0;
	char *pData = new char[100];
	memset(pData, 0x00, 100);
	sprintf(pData, "%s,%s,%d", strCode, strPrice, nOrderType);
	((CStdDialog*)GetParent())->SendMessage(UMSG_CREATESCREEN_LINK, 3, (LPARAM)pData);
	delete []pData;
}

void CExtInputBarDlg::SetOrderButtonEnable(BOOL bEnable)
{
	m_btOrder.EnableWindow(bEnable);
}

//<<< 20091111 by kmlee
void CExtInputBarDlg::OnCheckOrderLink ()
{
//<<20100308_JS.Kim �ַθ� aaaaaaaaaaaaaaaaaaaaaaaa
	// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
	BOOL bOrderLine = m_chkOrderLine.GetCheck();

	if(m_pChartMng->GetSafeHwnd())
	{
		if(bOrderLine == TRUE)
		{
			//AfxMessageBox("�ֹ���� ���� ��Ʈ�������� ���콺 ����� ���ѵ˴ϴ�.");
			// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
			((CStdDialog*)GetParent())->SetOrderLineInfo(bOrderLine);
		}

		m_pChartMng->SendMessage(UMSG_SETORDERLINK,(WPARAM)bOrderLine);	
	}
//>>
}
//>>>

// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
BOOL CExtInputBarDlg::GetChkOrder ()
{
	return m_chkOrderLine.GetCheck();
}


void CExtInputBarDlg::OnSelendokAccCtrl() 
{	
	m_strAccountNo = m_pAccountCtrl->GetAccNo();
	m_strAccountName = m_pAccountCtrl->GetAccName();
	m_strPassWord = m_pAccountCtrl->GetPswd();

	m_edAccPW.SetWindowText(m_strPassWord);

	SendAccountToPlatform(m_strAccountNo, m_strPassWord);

/*
	CString strAccount = m_strAccountNo;
	strAccount.Insert(5, "-");
	strAccount.Insert(3, "-");	
	m_pEditPass->SetText(m_strPassWord);
	
	SendCheckPassword();
	m_pEditPass->SetFocus();
	
	m_pStatic[STATIC_NAME]->SetText(m_strAccountName);
*/
}

void CExtInputBarDlg::GetAccountInfo(CString& strAccNo, CString& strAccName, CString& strAccPW) 
{
	strAccNo = m_pAccountCtrl->GetAccNo();
	strAccName = m_pAccountCtrl->GetAccName();
	m_edAccPW.GetWindowText(strAccPW);
}

void CExtInputBarDlg::SendAccountToPlatform(CString strAccountNo, CString strPassWord) 
{
	int nMajor = (int)::SendMessage(((CStdDialog*)GetParent())->m_hPlatform, WM_USER, MAKEWPARAM(variantDLL, majorCC), (LPARAM)(LPCTSTR)"");

	CString strCode = ((CChartMng*)m_pChartMng)->GetCurrentCode();
	strCode = CStdDialog::GetRealItemCode(strCode);

	CString strTemp;
	strTemp.Format("%s\t%d\t%s\t%s\t%s", "SetAccount9065", nMajor, strAccountNo, strPassWord, strCode);
	::SendMessage(((CStdDialog*)GetParent())->m_hPlatform, WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)strTemp.operator LPCTSTR());
}

void CExtInputBarDlg::InitAccountCtrl(WPARAM wParam, LPARAM lParam)
{
	if(m_pAccountCtrl)
	{
		if( wParam == 0 && lParam == 0 )
			m_pAccountCtrl->Init();
		else
			m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
	}
}

// 2011.01.31 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
void CExtInputBarDlg::ChangeAnalToolFile(BOOL bSave)
{
	if(m_pChartMng->GetSafeHwnd())
	{
		if (bSave)
			m_pChartMng->SendMessage(UMSG_ALALTOOL_FILE_CHANGE, 0, 1);
		else
			m_pChartMng->SendMessage(UMSG_ALALTOOL_FILE_CHANGE, 0, 0);
	}
}
// 2011.01.31 by SYS 
