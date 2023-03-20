// ExtInputBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExtInputBarDlg.h"
#include "StdDialog.h"
#include "./define/ChartMsgDef.h"
#include "../chart_common/BlockBaseEnum.h"
#include "../../inc/IAUGwanMng.h"

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
#define UWM_RECEIVE_ITRQUERY   ( WM_USER+826 )

CExtInputBarDlg::CExtInputBarDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CExtInputBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtInputBarDlg)	
	//}}AFX_DATA_INIT
//	m_crBk = CLR_EXTINPUTBAR_BKGRND;
	m_lRows = 1;
	m_lCols = 1;
	m_pChartMng = NULL;
	m_pLeftBar = NULL;
//	m_bRevised = TRUE;
	m_nSave = SAVE_TRUE;
	m_bNormalizedMinuteTimeRule = FALSE;
	m_bUseNormalizedMinuteTimeRule = TRUE;
	m_nDragModeType = 0;

	m_nYUnitType = WON_TICK;

	m_strCfgFile = _T("");

	m_chType = DAY_DATA_CHART;

	m_nMarketType = CMasterTypeInfo::STOCK_CHART;

	m_bEndDate	= TRUE;
	m_strStartDate	= _T("");
	m_strEndDate	= _T("");

	m_pIL_W46H20 = NULL;
	m_pIL_W20H20 = NULL;
	m_pIL_W20H20_Tool = NULL;
	m_pIL_W58H20 = NULL;

	m_bChkRevised = FALSE;
	m_nDataType = DAY_DATA_CHART;
	m_bChkPeriod = FALSE;
	m_bChkSaveJipyoOneFile = FALSE;
	m_bChkRegGwansimUp = FALSE;

	m_bChangeTool	= TRUE;
	m_nScrollState = SCROLL_STOP;
}


void CExtInputBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtInputBarDlg)
	DDX_Control(pDX, IDC_BT_SUB2, m_btSub2);
	DDX_Control(pDX, IDC_BTINITIAL, m_btInitial);
	DDX_Control(pDX, IDC_BTN_PERIOD, m_btnPeriod);
	DDX_Control(pDX, IDC_CHK_ENDSAVE, m_chkEndSave);
	DDX_Control(pDX, IDC_BTN_SDATE, m_btnStartDate);
	DDX_Control(pDX, IDC_BTN_EDATE, m_btnEndDate);
	DDX_Control(pDX, IDC_BTST, m_btST);
	DDX_Control(pDX, IDC_BTHIST, m_btHist);
//	DDX_Control(pDX, IDC_BTCHARTGUIDELINE, m_btChartGuideLine);
//	DDX_Control(pDX, IDC_BTANALYSISPATTERN, m_btAnalysisPattern);
	DDX_Control(pDX, IDC_BTEXPREV, m_btExPrev);
	DDX_Control(pDX, IDC_BTEXNEXT, m_btExNext);
	DDX_Control(pDX, IDC_BTSHOWMATRIX, m_btShowMatrix);
	DDX_Control(pDX, IDC_BTSETENVIR, m_btSetEnvir);
	DDX_Control(pDX, IDC_CHKSHOWSCALE, m_chkShowScale);
	DDX_Control(pDX, IDC_BTPRINT, m_btPrint);
	DDX_Control(pDX, IDC_BTLOAD, m_btLoad);
	DDX_Control(pDX, IDC_BTSAVE, m_btSave);
	DDX_Control(pDX, IDC_BTINITSPLIT, m_btInitSplit);
	DDX_Control(pDX, IDC_STBY, m_stBy);
	DDX_Control(pDX, IDC_STSETMATRIX, m_stSetMatrix);
	DDX_Control(pDX, IDC_BTINDEX, m_btIndex);
	DDX_Control(pDX, IDC_CHKFIXPERIOD, m_chkFixPeriod);
	DDX_Check(pDX, IDC_CHK_REVISED, m_bChkRevised);
	DDX_Control(pDX, IDC_BTN_CHANGEUNIT, m_btYUnit);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datePicker);
	DDX_Control(pDX, IDC_CHK_QUERYOPT, m_chkQueryOpt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtInputBarDlg, CRscDlg)
	//{{AFX_MSG_MAP(CExtInputBarDlg)
	ON_BN_CLICKED(IDC_BTINITSPLIT, OnBtinitsplit)
	ON_BN_CLICKED(IDC_BTINDEX, OnBtindex)
//	ON_BN_CLICKED(IDC_CHKJONGMOKLINK, OnChkjongmoklink)
	ON_BN_CLICKED(IDC_BTLOAD, OnBtload)
	ON_BN_CLICKED(IDC_BTSAVE, OnBtsave)
	ON_BN_CLICKED(IDC_BTINITIAL, OnBtinitial)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHKSHOWSCALE, OnChkshowscale)
//	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTPRINT, OnBtprint)
	ON_BN_CLICKED(IDC_BTSETENVIR, OnBtsetenvir)
	ON_BN_CLICKED(IDC_BTSHOWMATRIX, OnBtshowmatrix)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTEXNEXT, OnBtexnext)
	ON_BN_CLICKED(IDC_BTEXPREV, OnBtexprev)
//	ON_BN_CLICKED(IDC_BTANALYSISPATTERN, OnBtanalysispattern)
//	ON_BN_CLICKED(IDC_BTCHARTGUIDELINE, OnBtchartguideline)
	ON_BN_CLICKED(IDC_BTHIST, OnBthist)
	ON_BN_CLICKED(IDC_BTST, OnBtst)
	ON_BN_CLICKED(IDC_BTN_SDATE, OnBtnSdate)
	ON_BN_CLICKED(IDC_BTN_EDATE, OnBtnEdate)
	ON_CBN_SELCHANGE(IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
//	ON_BN_CLICKED(IDC_BTFIXPERIOD, OnBtfixperiod)
	ON_BN_CLICKED(IDC_CHK_ENDSAVE, OnChkEndsave)
	ON_BN_CLICKED(IDC_BTN_ANALIZE_PERIOD, OnBtnAnalizePeriod)
	ON_BN_CLICKED(IDC_CHK_REVISED, OnChkRevised)
	ON_BN_CLICKED(IDC_BTN_PERIOD, OnBtnPeriod)
	ON_BN_CLICKED(IDC_BT_SUB2, OnBtSub2)
	ON_BN_CLICKED(IDC_BTN_CHANGEUNIT, OnBtnChangeunit)
	ON_BN_CLICKED(IDC_CHKFIXPERIOD, OnChkfixperiod)
	ON_BN_CLICKED(IDC_CHK_QUERYOPT, OnChkQueryOpt)
//	ON_WM_ERASEBKGND()
//	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtInputBarDlg message handlers

BOOL CExtInputBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();

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
//	CImageList* pIL_W46H20_Gwan = &(((CStdDialog*)GetParent())->m_ImgList_W46H20_Gwan);

	m_btEtcMenu.FP_vSetImage(m_pIL_W58H20, 0, 2, TRUE, "�������", FALSE);
	m_btEtcMenu.FP_vSetToolTipText("�������");

	m_btInitial.FP_vSetImage(m_pIL_W46H20, 32, 34, TRUE, "�ʱ�ȭ", FALSE);
	m_btInitial.FP_vSetToolTipText("��ü��Ʈ�ʱ�ȭ");

//	m_brBkColor.CreateSolidBrush(m_crBk);

	m_strImageChartPath = ((CStdDialog*)GetParent())->m_szImageDir;

	m_strCfgFile = ((CStdDialog*)GetParent())->m_strConfig;

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

	m_pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
	if(m_pIL_W20H20 == NULL) return TRUE;

	m_pIL_W20H20_Tool = &(((CStdDialog*)GetParent())->m_ImgList_W20H20_Tool);
	if(m_pIL_W20H20_Tool == NULL) return TRUE;
	m_btSub2.FP_vSetImage(m_pIL_W20H20_Tool, 0, 2, FALSE, "", TRUE);		
	m_btSub2.FP_vSetToolTipText("���ڼ�");

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

	//��Ÿ Addin ���� ��ư 
//	m_btAnalysisPattern.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"P")	;	//m_btAnalysisPattern.FP_vSetToolTipText();
//	m_btChartGuideLine.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"G")		;	//m_btChartGuideLine.FP_vSetToolTipText();
	m_btHist.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"C")				;	//m_btHist.FP_vSetToolTipText();
	m_btST.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"S")					;	//m_btST.FP_vSetToolTipText();


	SetExt(0);
	char szTemp[64];
	UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);

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
	if(m_bUseNormalizedMinuteTimeRule)
	{
		::GetPrivateProfileString("ExtInputBar","NormalizedMinuteTimeRule","0",szTemp,sizeof(szTemp),m_strCfgFile);
		if(szTemp[0]=='1')
		{
			m_bNormalizedMinuteTimeRule = TRUE;
		}
		else
		{
			m_bNormalizedMinuteTimeRule = FALSE;
		}
	}

	::GetPrivateProfileString("ExtInputBar","DragModeType","0",szTemp,sizeof(szTemp),m_strCfgFile);
	m_nDragModeType = atol(szTemp);

	::GetPrivateProfileString("ExtInputBar","SaveJipyoOneFile","0",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
	{
		m_bChkSaveJipyoOneFile = TRUE;
	}
	else
	{
		m_bChkSaveJipyoOneFile = FALSE;
	}

	::GetPrivateProfileString("ExtInputBar","RegGwansimUp","0",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
	{
		m_bChkRegGwansimUp = TRUE;
	}
	else
	{
		m_bChkRegGwansimUp = FALSE;
	}

//@��������
//	m_datePicker.InitCodeInput(this, IDC_DATETIMEPICKER1, 5000);
//@��������
	CRect rectComp;
	GetDlgItem(IDC_DATETIMEPICKER1)->GetWindowRect(rectComp);
	ScreenToClient(rectComp);
	m_datePicker.SetWindowPos(NULL, rectComp.left, rectComp.top, rectComp.Width(), 19, SWP_NOZORDER);

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
			m_stBy.SetWindowText(strBy);
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
	case UMSG_GETSAVEJIPYOONEFILE:
		{
			return (long)(m_bChkSaveJipyoOneFile);
		}
		break;
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
	case UMSG_SETNORMALIZEDMINUTETIMERULE:
		{
			m_bNormalizedMinuteTimeRule = wParam;
		}
		break;
	case UMSG_GETNORMALIZEDMINUTETIMERULE:
		{
			return m_bNormalizedMinuteTimeRule;
		}
		break;
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
			return atol(m_strEndDate);
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
			m_chType = m_pShareInputData->m_DataInputBar.m_chType;

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
//			m_chType = _pStData->m_chType;

			//�����ڵ� 05. 09. 05 
			//@080523-001m_strCode = _pStData->m_strCode;

			// chart Type�� ���� Control �޷� Control Ȱ��ȭ ����..
			BOOL bEnable = FALSE;

//@080523-001	if((_pStData->m_chType==DAY_DATA_CHART||_pStData->m_chType==WEEK_DATA_CHART||_pStData->m_chType==MONTH_DATA_CHART||_pStData->m_chType==YEAR_DATA_CHART))
			if((m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART||m_chType==MONTH_DATA_CHART||m_chType==YEAR_DATA_CHART))
			{
				bEnable = TRUE;
			}
			//m_datePicker.EnableWindow(bEnable);
//@��������
//			if(m_datePicker.GetEnabled() == !bEnable)
//				m_datePicker.SetEnabled(bEnable);
//@��������
//@�����߰�
			if(m_datePicker.IsWindowEnabled() == !bEnable)
				m_datePicker.EnableWindow(bEnable);
//@�����߰�
			m_btnStartDate.EnableWindow(bEnable);
			m_btnEndDate.EnableWindow(bEnable);

			// ��¥ ���� ����
			if(bEnable)
			{		
				m_strStartDate	.Format("%d", _pStData->m_lStartDate);
				m_strEndDate	.Format("%d", _pStData->m_lEndDate);
			
				if(		(m_strStartDate	.GetLength() != 8) ||
						(m_strEndDate	.GetLength() != 8) ||	
						(atoi(m_strStartDate)<= 19700101 ) ||
						(atoi(m_strEndDate	)<= 19700101 ) 
				)
				{
					CTime currentTime = CTime::GetCurrentTime(); 
//@�������� : m_datePicker �ε��ȵǰ� �������Ƿ� �ڵ�üũ ����.
					if(m_datePicker.GetSafeHwnd()) m_datePicker.SetTime(&currentTime);
					m_strStartDate = m_strEndDate =currentTime.Format("%Y%m%d");
				}

				m_bEndDate = _pStData->m_bStartDateState ? FALSE : TRUE;
				if( m_bEndDate )				
				{
					OnBtnEdate();
				}
				else 
				{
					OnBtnSdate();
				}
			}

			//01.11 LYH================================================================
			//���� �ְ� �� �α� �����ֱ� ����
			m_bChkRevised	= _pStData->m_bRevised;
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
					m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106);
					m_btScrollPre.Invalidate();
				}
				else
				{
					m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 106, 104);
					m_btScrollPre.Invalidate();
				}
			}
			else if(wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT)
			{
				if(!lParam)
				{
					m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70);
					m_btScrollNext.Invalidate();
				}
				else
				{
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
	case UMSG_SETTYPE:
		{
			char chType = wParam;
			m_chType = chType;
		}
		break;
	default:
		break;
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
}


void CExtInputBarDlg::OnBtindex() 
{
}

void CExtInputBarDlg::SetExt(int nAct)
{
	EnableWindow(FALSE);

	int nCmdShow1 = SW_SHOW;
	int nCmdShow2 = SW_SHOW;
	int nCmdShow3 = SW_SHOW;
	int nCmdShow4 = SW_SHOW;
	int nCmdShow5 = SW_SHOW;

	m_btSave.ShowWindow(nCmdShow1);
	m_btLoad.ShowWindow(nCmdShow1);
	m_btPrint.ShowWindow(nCmdShow1);

	m_btIndex.ShowWindow(nCmdShow2);

	m_stSetMatrix.ShowWindow(nCmdShow3);		
	m_btExPrev.ShowWindow(nCmdShow3);
	m_btExNext.ShowWindow(nCmdShow3);
	m_btInitial.ShowWindow(nCmdShow5);
	m_chkShowScale.ShowWindow(nCmdShow5);
	m_chkFixPeriod.ShowWindow(nCmdShow5);		// �Ⱓ����
	//S01 2006.09.07 }}

// for Pattern Test - SeoJeong - 
#ifdef _DEBUG
//	m_btAnalysisPattern.ShowWindow(nCmdShow5);
//	m_btChartGuideLine.ShowWindow(nCmdShow5);
	GetDlgItem(IDC_BTN_ANALIZE_PERIOD)->ShowWindow(nCmdShow5);
#else
//	m_btChartGuideLine.ShowWindow(SW_HIDE);
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

		
		SHIFTCONTROL(m_chkShowScale,nShiftExt12,rect);
		SHIFTCONTROL(m_btHist,nShiftExt12,rect);
		SHIFTCONTROL(m_btST,nShiftExt12,rect);
	}
	else if(nAct==2)
	{
		SHIFTCONTROL(m_stSetMatrix,nShiftExt23,rect);
		SHIFTCONTROL(m_stBy,nShiftExt23,rect);
		SHIFTCONTROL(m_btShowMatrix,nShiftExt23,rect);
		SHIFTCONTROL(m_btExPrev,nShiftExt23,rect);
		SHIFTCONTROL(m_btExNext,nShiftExt23,rect);
		SHIFTCONTROL(m_btInitSplit,nShiftExt23,rect);

		SHIFTCONTROL(m_chkShowScale,nShiftExt23,rect);
		SHIFTCONTROL(m_btHist,nShiftExt23,rect);
		SHIFTCONTROL(m_btST,nShiftExt23,rect);

	}
	else if(nAct==3)
	{
		SHIFTCONTROL(m_chkShowScale,nShiftExt34,rect);
		SHIFTCONTROL(m_btHist,nShiftExt34,rect);
		SHIFTCONTROL(m_btST,nShiftExt34,rect);
	}
	else if(nAct==4)
	{
		SHIFTCONTROL(m_chkShowScale,nShiftExt45,rect);
		SHIFTCONTROL(m_btHist,nShiftExt45,rect);
		SHIFTCONTROL(m_btST,nShiftExt45,rect);

	}
	Invalidate();
	EnableWindow();
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
	m_brBkColor.DeleteObject();
	
	if(((CStdDialog*)GetParent())->m_strConfig == m_strCfgFile)
	{
		CString strTemp;
		UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);
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

//		strTemp.Format("%d", m_bTimeShareUseage);
//		::WritePrivateProfileString("ExtInputBar","TimeShareUseage",strTemp,m_strCfgFile);	
//
////		strTemp.Format("%d", m_bUpperLowPriceMarkUseage);
////		::WritePrivateProfileString("ExtInputBar","UpperLowPriceMarkUseage",strTemp,m_strCfgFile);	
//
		strTemp.Format("%d", m_bNormalizedMinuteTimeRule);
		::WritePrivateProfileString("ExtInputBar","NormalizedMinuteTimeRule",strTemp,m_strCfgFile);	
//
//		strTemp.Format("%d", m_bShowLock);
//		::WritePrivateProfileString("ExtInputBar","ShowLock",strTemp,m_strCfgFile);
		
		strTemp.Format("%d", m_nDragModeType);
		::WritePrivateProfileString("ExtInputBar","DragModeType",strTemp,m_strCfgFile);

		strTemp.Format("%d", m_bChkSaveJipyoOneFile);
		::WritePrivateProfileString("ExtInputBar","SaveJipyoOneFile",strTemp,m_strCfgFile);	

		strTemp.Format("%d", m_bChkRegGwansimUp);
		::WritePrivateProfileString("ExtInputBar","RegGwansimUp",strTemp,m_strCfgFile);	
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

//HBRUSH CExtInputBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
//		break;
//	}
//	
//	// TODO: Return a different brush if the default is not desired
//	return hbr;
//}

BOOL CExtInputBarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam) 
		{
		case VK_RETURN :
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
}

void CExtInputBarDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
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

//void CExtInputBarDlg::OnBtanalysispattern() 
//{
//	// TODO: Add your control notification handler code here
//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//		m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN);
//}

//void CExtInputBarDlg::OnBtchartguideline() 
//{
//	// TODO: Add your control notification handler code here
//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//		m_pChartMng->SendMessage(UMSG_CHARTGUIDELINE);	
//}
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
	if( ( m_nMarketType == CMasterTypeInfo::STOCK_CHART	)//	||
		//( m_nMarketType == UPJONG_CHART )	
		)
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
	if(m_datePicker.IsWindowEnabled() == !bEnable)
		m_datePicker.EnableWindow(bEnable);		
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

void CExtInputBarDlg::OnDatetimechangeDatetimepicker1() 
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
		m_strEndDate = strDate;
	}
	else
	{
		m_strStartDate = strDate;
	}	

	if(m_bEndDate) // ������ ����̸� TR�� ������.
	{
		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
	}

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
//void CExtInputBarDlg::OnBtfixperiod() 
//{
//	//CString strDate;
//	//CTime tmItem;
//	//m_datePicker.GetTime(tmItem);
//	//strDate = tmItem.Format("%Y%m%d");	
//
//	//InputBar �� ���� ���� ZoomValue���� �����´�.
//	
//
////	CString strZoomValue;
////	long lZoomValue = 0;
////	m_editZoomValue.GetWindowText(strZoomValue);
////	lZoomValue = atol(strZoomValue);
//
////	m_bChkPeriod = m_btFixPeriod.GetCheck();
////	GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 1, m_bChkPeriod);
////	if(m_bChkPeriod)
////	{
//		int lEndDate = atoi(m_strEndDate);
//
//		SYSTEMTIME tm;
//		::ZeroMemory(&tm,sizeof(tm));
//		if(m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART)
//		{
//			tm.wYear = lEndDate/10000;
//			tm.wMonth = lEndDate%10000/100;
//			tm.wDay = lEndDate%100;
//		}
//		else if(m_chType==MONTH_DATA_CHART)
//		{
//			tm.wYear = lEndDate/10000;
//			tm.wMonth = lEndDate%10000/100;
//			tm.wDay = 1;
//		}
//
//		if(m_bEndDate) // �����Ϸ� ���õǾ� ���� ��쿡�� �޷� ��Ʈ�ѿ� ���� ���Ѵ�.
//		{
//			//m_datePicker.SetTime(&tm);
////@��������
////			m_datePicker.SetDataText(m_strEndDate);
////@��������
////@�����߰�
//			m_datePicker.SetTime(&tm);
////@�����߰�
//		}
//
//		m_pChartMng->SendMessage(UMSG_FIXDATE,0, (LPARAM)(LPCTSTR)m_strEndDate);
////	}
//}

void CExtInputBarDlg::OnChkEndsave() 
{
	// TODO: Add your control notification handler code here
	if( m_chkEndSave.GetCheck() )
		m_nSave = SAVE_TRUE	;
	else
		m_nSave = SAVE_FALSE;
}

//BOOL CExtInputBarDlg::GetTimeSyncFlag()
//{
//	return m_chkTypeLink.GetCheck();
//}

//void CExtInputBarDlg::SetTimeSyncFlag(BOOL bCheck)
//{
//	m_bReplayMode = bCheck;
//	
//	m_chkTypeLink.SetCheck(bCheck);	
//	m_chkTypeLink.Invalidate();
//}

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
	//����Ʈ/ĵƲ��Ʈ ���� �׽�Ʈ
	//((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AddChartBlock("����Ʈ");
	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AddChartBlock("ĵ����Ʈ");
}

void CExtInputBarDlg::OnBtSub2() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = m_btSub2.GetCheck();
	if(bCheck)
	{
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, TRUE);
	}
	else
	{
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, FALSE);
	}
}

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

//BOOL CExtInputBarDlg::OnEraseBkgnd(CDC* pDC) 
//{
//	return TRUE;
//}

//void CExtInputBarDlg::OnPaint() 
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
//
//	memDC.FillSolidRect(rcClient, m_crBk);
//	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
//	memDC.SelectObject(pOldBitmap);
//	memDC.DeleteDC();
//	bitmap.DeleteObject();
//}

//void CExtInputBarDlg::ClearLink()
//{
//	m_chkJongmokLink.SetCheck(0);
//	OnChkjongmoklink();
//	m_chkTypeLink.SetCheck(0);
//	OnChktypelink();
//}

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

