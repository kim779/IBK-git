// ChartItem.cpp: implementation of the CChartItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "ChartItem.h"
//#include "ChartDef.h"
#include "./define/ChartMsgDef.h"
#include "ChartMng.h"
//#include "../../COMMON_BASE/drds/DRDSDEF.h"
//#include "../CommonTR/DrdsS31.h"
#include "../COMMONTR/ChartRealConverter.h"
#include "./define/MessageDefine.h"
#include "../../inc/RealUpdate.h"
#include "../chart_common/IRealReceiver.h"

#include "StdDialog.h"

//#ifdef __MULTI_LANG__
	#include "../Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
//#endif

//=============================================================
//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../COMMON_BASE/baseInfo/BaseInfo.h" //2005.08.24 add by sy, nam
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27
//=============================================================
//#include "../CommonTR/DrdsS31.h"
//#include "../CommonTR/DrdsJS0.h"
//#include "../CommonTR/DrdsSC0.h"
//#include "../CommonTR/DrdsOC0.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString SetComma(CString strNumber, BOOL bShowSign=FALSE);

#define DEFAULT_CHARTDATA_COUNT_INT		400
#define DEFAULT_CHARTDATA_COUNT_CHAR	"400"

#define WMU_GET_LAST_KEY			WM_USER + 2237	// AupAgent���� MainFrame�� ������ ��ȸ�� Key�� ��û�Ѵ�.

#define	MAX_g_strColor	20
int	g_nColor=0;

LPCTSTR szSetAllForSignalnew[] = 
{
	"EN=1",
	"HI=0",
	"ST=",
	"GS=",
	"LS=",
	"XCM=��ȣ��",
	"XID=",
	"CFT=����ü;12;0;0;0;0;",
	"TKS=1",
	"UDF=1",
	"OCN=NULL"
};

LPCTSTR szSetAll[] = 
{
	"CM=Name,EuChart0",
	"AN=EuChart0",
	"ID=3,4,0",
	"RC=2,24,397,248",
	"WS=WS_CHILD|WS_VISIBLE",
	"WSE=",
//	"BT=0",
//	"BC=0",
//	"RC=1",
//	"ROWC=1",
//	"CC=1",
//	"BG=-1",
//	"BBT=1",

	"BDT=0",
//	"SLT=0",							// �Ǽ�(graph ���ý�)
	"SLT=2",							// ����(graph ���ý�)
	"WV=0",
//	"WV=0",							==> Big Bang Original Source���� ���
	"PDC=120",
	"BAC=10",			// �ִ� ����
	"GDD=0",
//	"LM=5",	// left ����
//	"RM=0",	// right ����
//	"TM=5",	// top ����
//	"BM=0",	// bottom ����
	"AGM=1",												// ���ؽ�Ʈ �޴� 
	"EFN=",
	"IFN=",
	"EU=",
	"HI=0",
	"IR=0",
	//"TO=����\r\n����;;;\r\n��ũ�ѹ�;;;\r\nȮ��,���;����;10��;\r\n��ü����;;;\r\n���ڼ�;��ġ��ȸâ ������;;\r\nDisplay�� ������;;;\r\n����;;;\r\n"
	"TO=����\r\n��ũ�ѹ�;;;\r\nȮ��,���;����;10��;\r\n��ü����;;;\r\nDisplay�� ������;;;\r\n"
	"�ܺ�\r\n"
	"RButton\r\n",

	"CL=BlockColumn;\r\n"
	"Block;\r\n"
	"�ڷ�����;4;2;/;:;0;0;0;\r\n"
	"VertScaleGroup;\r\n"
	"��ǥ Data;1;2;0;\r\n"
	"VertScaleGroupEnd;\r\n"
	"BlockEnd;\r\n"
	"BlockColumnEnd;\r\n",

	"PO=\r\n",//1;�ڷ�����;;6;HHMMSS;��;\r\n",
	"LSL=�����ڵ�5;",
	"RL=",
	"URD=1",
	"MBT=3",
	"USESKIN=0",
//	"CBKG=255,255,255",
//	"CBKB=188,198,223",
//	"CB=255,255,255",
//	"CBB=0,0,0",
//	"CSL=204,204,204",
//	"CST=0,0,0",
//	"CGB1=8,189,246",
//	"CGB2=128,182,0",
//	"CGB3=253,122,4",
//	"CGB4=150,19,147",
//	"CGB5=255,133,11",
//	"CGB6=128,0,255",
	"CAT=82,103,146",

	"BR=row;0;216;\r\n"
	"column;0;387;",

//	"CSL2=204,204,204",
//	"RDB=0",
	"RDB=-1",
	"UIDWCL=0",
	"CDB=0",
//	"CIW=255,255,255",
	"CFT=����ü;12;0;0;0;0;",
	"USK=0",
	"UCTT=1",
	"UDTT=0",
	"NRBMT=1",
	"BSWMB=0",
	"BCBS=1",
	"BBMD=1",
	"BST=0",	// vntsorl_20110608:[A00000421] ���콺�� �̿��� ���� ����ó��
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
//	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=0",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=4",//"NIO=6",//"NIO=7",
	"GSL=�ð�-�׸���4;�ð�-��Ʈ2;ȣ��-��Ʈ2;"
	"PGCNTTR=1"		// JSR 20060221 ADD : From Doori
};

LPCTSTR szSetAllNoToolbar[] = 
{
	"CM=Name,EuChart0",
	"AN=EuChart0",
	"ID=3,4,0",
	"RC=2,24,397,248",
	"WS=WS_CHILD|WS_VISIBLE",
	"WSE=",
	"BT=0",
	"BC=0",
//	"RC=1",
	"ROWC=1",
	"CC=1",
	"BG=-1",
	"BBT=1",

	"BDT=0",
//	"SLT=0",							// �Ǽ�(graph ���ý�)
	"SLT=2",							// ����(graph ���ý�)
	"WV=0",
//	"WV=0",							==> Big Bang Original Source���� ���
	"PDC=120",
	"BAC=10",			// �ִ� ����
	"GDD=0",
	"LM=5",	// left ����
	"RM=0",	// right ����
	"TM=5",	// top ����
	"BM=0",	// bottom ����
	"AGM=1",												// ���ؽ�Ʈ �޴� 
	"EFN=",
	"IFN=",
	"EU=",
	"HI=0",
	"IR=0",
	"TO=����\r\n\r\n��ũ�ѹ�;;;\r\nȮ��,���;����;10��;\r\n��ü����;;;\r\nDisplay�� ������;;;\r\n"
//	"TO=����\r\n����;;;\r\n��ũ�ѹ�;;;\r\nȮ��,���;����;10��;\r\nDisplay�� ������;;;\r\n����;;;\r\n"
//	"TO=����\n��ũ�ѹ�;;;\nGraph����;;;\n������;;;\n������;;;\n��ġ��ȸâ;�˾� ���;��ġDialog ���;\n���ڼ�;��ġ��ȸâ ������;;\n����;;;\n����;;;\nȮ��,���;����;10��;\n������;;;\r\n"
	"�ܺ�\r\n"
	"RButton\r\n",

	"CL=BlockColumn;\r\n"
	"Block;\r\n"
	"�ڷ�����;4;2;/;:;0;0;0;\r\n"
	"VertScaleGroup;\r\n"
	"��ǥ Data;1;2;0;\r\n"
//	"��ǥ Data;1;3;0;\r\n"

 
//=========================================================================================
//From_LTH (2003.7.1 ) 
// �Ʒ����� AddGraph�� ���� �����ϹǷ� �ּ�ó����
//	"Graph;\r\n"
//	"����;;�ǽð� ����;0;0;0;0;0;0;\r\n"
//	"1;0;0;Graph1;�ǽð�����;0;17,0,0;17,0,0;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
//	"GraphEnd;\r\n"
//=========================================================================================

/*	"Graph;\r\n",
	"����;;�ŵ�����;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;�ŵ�����;0;0,0,255;0,0,255;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	"����;;�ż�����;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;�ż�����;0;204,0,0;204,0,0;0,0,255;0,0,255;255,0,255;255,0,255;0;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	"����;;�ŷ�������;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;�ŷ�������;0;0,102,0;0,119,0;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"*/
	"VertScaleGroupEnd;\r\n"
	"BlockEnd;\r\n"
	"BlockColumnEnd;\r\n",

	"PO=\r\n",//1;�ڷ�����;;6;HHMMSS;��;\r\n",
//	"1;�ǽð� ����;;10;�� 0.01;��;\r\n"
//	"1;�ŵ�����;;10;�� 0.01;��;\r\n"
//	"1;�ż�����;;10;�� 0.01;��;\r\n"
//	"1;�ŷ�������;;10;�� 0.01;��;",
//	"PO=1;�ڷ�����;;6;HHMM  ;��;\r\n",		==> Big Bang Original Source���� ���
//	"1;�ǽð� ����;;10;�� 0.01;��;\r\n",

	"LSL=�����ڵ�5;",

//	"RL=KP2.time.�ڵ�-�ǽð�����;"
//	"KP2.time.�ڵ�-�ǽð�����;"
//	"KP2.medojisu.�ڵ�-�ǽð�����;"
//	"KP2.mesujisu.�ڵ�-�ǽð�����;"
//	"KP2.upjisu.�ڵ�-�ǽð�����;",
	"RL=",
//	"RL=SC0.time.�����ڵ�;"
//	"SC0.price.�����ڵ�;",

	"URD=1",
	"MBT=3",
//	"CBKG=208,209,213",
	"USESKIN=0",
	"CBKG=255,255,255",
	"CBKB=188,198,223",
//	"CBKB=0,0,0",
//	"CBKB=255,255,255",
//	"CB=236,236,236",
	"CB=255,255,255",
//	"CBB=231,233,230",
	"CBB=0,0,0",
//	"CSL=255,255,255",
//	"CSL=204,204,204",
	"CST=0,0,0",
	"CGB1=8,189,246",
	"CGB2=128,182,0",
	"CGB3=253,122,4",
	"CGB4=150,19,147",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=82,103,146",


	"BR=row;0;216;\r\n"
	"column;0;387;",

//	"CSL2=204,204,204",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
	"CIW=255,255,255",
	"CFT=����ü;12;0;0;0;0;",
	"USK=0",
	"UCTT=1",
	"UDTT=0",
	"NRBMT=1",
	"BSWMB=0",
	"BCBS=1",
	"BBMD=1",
	"BST=0",	// vntsorl_20110608:[A00000421] ���콺�� �̿��� ���� ����ó��
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=0",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=6",//"NIO=4",//"NIO=7",
	"GSL=�ð�-�׸���4;�ð�-��Ʈ2;"
//	"SLHW=�ð�_��Ʈ,1,0,0x00000000;�ð�_�׸���,2,0,0x00000000;", //05. 08. 29 ��ȭ���� ���� �������.
	//"PAGECOUNTTR=1" //05.11.28 �߰�
	"PGCNTTR=1"		// JSR 20060221 ADD : From Doori
/*
	"CBKG=255,255,255",
	"CBKB=0,0,0",
	"CB=255,255,255",
	"CBB=0,0,0",
	"CSL=128,128,128",
	"CST=0,0,0",
	"CGB1=0,136,0",
	"CGB2=255,55,255",
	"CGB3=255,51,51",
	"CGB4=0,109,219",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=0,0,0",

	"BR=row;0;216;\r\n"
	"column;0;387;",

	"CSL2=128,128,128",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
	"CIW=203,228,203",
	"CFT=����ü;12;0;0;0;0;",
	"USK=0",
	"UCTT=0",
	"UDTT=0",
	"NRBMT=0",
	"BSWMB=1",
	"BCBS=1",
	"BBMD=0",
	"BST=0",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=2048",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
*/
};

#define COLOR_UP		RGB(255,0,0)
#define COLOR_DOWN		RGB(65,65,255)
#define COLOR_STEADY	RGB(0,0,0)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartItem::CChartItem()
{
	m_chSign = '3';
	m_bOnAdvise = FALSE;
	m_bInitalChart = FALSE;
	m_bNext = FALSE;
	m_lCntForRequest = 0;
	m_chType = MIN_DATA_CHART;	// NULL; /*DAY_DATA_TYPE*/
	m_chTypeForRequest = MIN_DATA_CHART;// DAY_DATA_CHART;
//	SetChartRequestType(DAY_DATA_CHART);
//	m_strTickMinDayForRequest = "001";	// 2Byte Tick & Min & Day Number
	m_strTickMinDay = "001";
	m_nChartNumericValueType = 0;
	
	//2005. 05. 24 add by sy, nam ==================================
	m_strTick = _T("001");
	m_strMin = _T("001");
	m_strDay = _T("001");
	//==============================================================
	m_lPtrST = NULL;	// data pointer Of System trading is added by gm0604. 2005.7.21

	m_bRevisedPrice = FALSE;
	//m_bRateValue = FALSE;
	ResetData();
	//m_lColIndex = -1;
	//m_lRowIndex = -1;
	SetRowNCol(-1, -1);
	m_nSmallChartMode = 0;
	m_lNOnePageDataCount = 0;//05.09.09 0==>140
	m_bShowLock = TRUE;
	m_plDllWnd = NULL;
	m_bBuild = FALSE;
	m_bUseInnerControl = FALSE;

	m_nCntOverLap	= -1;    // ��ø�� ���� Cnt;
	m_bOverLapping	= FALSE; // ��ø ��� Flag
	m_bLogPrice		= FALSE; // �α� �� ���

	m_nChartNumericValueType = 0;

	m_pIL_Symbol = NULL;
	m_bSharedOn = FALSE;

	m_pStdDialog = NULL;

	m_bReceivedDataChart = FALSE;

	Init_ChartCfgInfo();
	
	m_strOpen = "";
	m_strHigh = "";
	m_strLow = "";

	m_bAddInLoadPattern = FALSE;
	m_bAddInLoadPeriod = FALSE;
	m_bAddInLoadSTIndex = FALSE;
//	m_bAddInLoadNews = FALSE;

	//2007.01.17 add by LYH  ���ٹ�ư ����ȭ
	m_bDrawAutoTrendLine = FALSE;
	m_bDrawAutoPattern = FALSE;
	m_bDrawCandlePattern = FALSE;
	m_bDrawNews = FALSE;

	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	ST_SELCHART* pSelChart = new ST_SELCHART;
	m_mapChartItem.SetAt(_T("DEFAULT"), pSelChart);

	// �������� �ҷ����� : �������� - ojtaso (20070514)
	m_bLoadRQ =	FALSE;
	m_posLoadingRQ = NULL;
	m_strKey.Empty();
	m_lpSelChart = pSelChart;

	m_strSelectedRQ = _T("DEFAULT");

	m_bLoadDefaultRQ = FALSE;

	m_chCompareChartType = DIVIDED_COMPARE_CHART;

	m_nCntCompare   = 0; //����Ʈ �߰��� Item ����.
}

CChartItem::~CChartItem()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::~CChartItem::Begin", _T("����Ʈ ����"), 13);
#endif
	//��ȣ ���� ================================================
	//2005. 08. 24  nam
//	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
//		CloseShareManager();
	//==========================================================

	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	POSITION pos = m_mapChartItem.GetStartPosition();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)pSelChart);
		delete pSelChart;
	}

	m_mapChartItem.RemoveAll();

	// ��� �������� ����
	pos = m_mapChartItemRealData.GetStartPosition();
	ST_CHARTITEM_REALDATA* lpChartItemRealData;
	while(pos)
	{
		m_mapChartItemRealData.GetNextAssoc(pos, strKey, (LPVOID&)lpChartItemRealData);
		if(lpChartItemRealData)
			delete lpChartItemRealData;
	}

	m_mapChartItemRealData.RemoveAll();
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::~CChartItem::End", _T("����Ʈ ����"), 13);
#endif
}

//BEGIN_EVENTSINK_MAP(CChartItem, CKBChart)
//    //{{AFX_EVENTSINK_MAP(CTOcx)
//	ON_EVENT(CChartItem, 1000, 1 /* OnLButtonDown */, OnOnLButtonDownTocxctrl1, VTS_I2 VTS_I2)
//	//}}AFX_EVENTSINK_MAP
//END_EVENTSINK_MAP()


BOOL CChartItem::Create(CWnd *pWnd,LPRECT lpRect,int nID,long lRow,long lCol, LPCTSTR lpImagePath, BOOL bUseInnerControl)
{
	m_pStdDialog = NULL;

	//================================================================================================================================
	BOOL lResult = CDRChartOcx::Create(NULL,WS_VISIBLE|WS_CHILD,*lpRect,pWnd,nID);
	//================================================================================================================================

	
	m_pStdDialog = ((CStdDialog*)GetParent()->GetParent());
#ifdef _DEBUG
	if(m_pStdDialog == NULL)
		AfxMessageBox("CChartItem::Create...pStdDialog is NULL");
#endif

//	m_pDrMLib_DBMgr = ((CChartMng*)pWnd)->GetpDBMgr();

	m_nID = nID;

	SetRowNCol(lRow, lCol);
	CString strImagePath = m_strImagePath = lpImagePath;
	CString strImageUp;
	CString strImageDown;
	m_bUseInnerControl = bUseInnerControl;

	if(bUseInnerControl)
	{
//		m_btMaxmize.Create("",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_CLIPSIBLINGS,CRect(0,0,0,0),pWnd,(m_nID+1));

		// CChartMng::OnCmdMsg��ƾ���� �޽����� ����.
		if( ((CStdDialog*)m_pStdDialog)->m_nScreenType==0)
		{
			m_btInit.Create("",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_CLIPSIBLINGS,CRect(0,0,0,0),pWnd,(m_nID+2));
			//BOOL bRet = m_signal.InitSignal((CDialog*)GetParent(),CRect(0,0,10,10),(m_nID+3), SIGNAL_PROP_NOSHARE);
			
			//Han Hwa  =====================================================================================	
	//		m_signal.Create("",WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_EX_CLIENTEDGE|WS_CLIPSIBLINGS, CRect(0,0,0,0), pWnd, (m_nID+3));

			// Load ImageList
	//		CString strSignalImageFile = strImagePath;
	//		strSignalImageFile.Replace("Chart", "");
	//		strSignalImageFile += "��ȣ���ȣ.bmp";	
	//		m_ILSymbol.Create(16,16,ILC_COLORDDB,16,16);		
	//
	//		CBitmap bitmap;
	//		bitmap.Attach((HBITMAP)LoadImage(NULL,strSignalImageFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	//		m_ILSymbol.Add(&bitmap, RGB(255,0,0));
	//		bitmap.DeleteObject();

			//m_pIL_Symbol = &(((CStdDialog*)m_pStdDialog)->m_ImgList_Signal);
			//m_signal.FP_vSetImage(m_pIL_Symbol, 19, 19);		
			// end=========================================================================================================	


	//		m_btMaxmize.SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			m_btInit.SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		
	//		m_btMaxmize.FP_vSetImage(&(((CStdDialog*)m_pStdDialog)->m_ImgList_W15H15), 0, 1);	
	//		m_btMaxmize.FP_vSetToolTipText("�ִ�ȭ/����");

			m_btInit.FP_vSetImage(&(((CStdDialog*)m_pStdDialog)->m_ImgList_W15H15), 8, 9);	
			m_btInit.FP_vSetToolTipText("����");
		}
	}
	else
	{
		//BOOL bRet = m_signal.InitSignal((CDialog*)GetParent(),CRect(0,0,10,10),(m_nID+3), SIGNAL_PROP_STOCKQTY);
		//m_signal.ShowWindow(SW_HIDE);//SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

//		m_btInit.Create("",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(0,0,0,0),pWnd,(m_nID+2));
//		m_btInit.FP_vSetImage(&(((CStdDialog*)m_pStdDialog)->m_ImgList_W15H15), 8, 9);	
//		m_btInit.FP_vSetToolTipText("�ʱ�ȭ");
	}

	//��Ʈ �����ִ� ����/�ֽ�/����/�ؿ� ����.
	if(m_pStdDialog)
		m_nMarketType = ((CStdDialog*)m_pStdDialog)->m_nMarketType;

//@��������
//	//-----------------------------------------------------------------------------
//	// Author		: Nam	Date :2005/8/24
//	// Reason		: ���� �޴��� ����
//	//-----------------------------------------------------------------------------
//	// start
//	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
//		InitShareManager();
//
//	//����
////	m_hCHild = ((CStdDialog*)m_pStdDialog)->GetParentFrame()->GetSafeHwnd();
////	m_hCtrl  = GetSafeHwnd();
//@��������

	return lResult;
	//================================================================================================================================
}

void CChartItem::ResetData()
{
	SetDrdsAdvise(FALSE);
	m_bInitalChart = FALSE;
//{{2007.05.02 by LYH ����ð� - ���� �����Ϸ� ����
	// 1. Master���� �ڵ鸵�� �Ŵ��� ���� ���������� �����ʿ�
//	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
//	CString strDate = pDataManager->GetBizDate();
	CTime t = CTime::GetCurrentTime();
	CString strDate = t.Format("%Y%m%d");
	SetEndDateRequested(strDate);
	SetEndDate(strDate);
//}}

	m_bNext = FALSE;
	m_lCntForRequest = DEFAULTCNTATONCE;
	m_lAllCnt = 0;
	m_lPtrST = NULL;// gm0604 added 2005.9.6
	m_strKorName.Empty();

	m_chCompareChartType = DIVIDED_COMPARE_CHART;
	m_nCntCompare   = 0; //����Ʈ �߰��� Item ����.

	// �����ְ� Ŭ���� ��ü�� ������Ű�� ���� �߰� by LYH 2007.01.11
	m_nTotCountForRevised = 0;
	g_nColor = 0;
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ�� �ʱ�ȭ�Ѵ�.
//  ����    :   sy, nam
//  ��������:   2005.05.10
//  ��������:   BaseDesk�� ��ȭ���� �� ���Ƿ� ���� ���� HTS���� ��θ� �ѱ��.
//////////////////////////////////////////////////////////////////////////////

void CChartItem::SetInit(long p_lDllWnd, long lKey, LPCTSTR strHtsPath, BOOL bNoToolBar /*=FALSE*/)
{
	m_plDllWnd = p_lDllWnd;
	m_pBaseDesk2 = (CXIBaseDesk*)lKey;
	CDRChartOcx::SetBaseDesk((long)&m_xIBaseDesk);
	CDRChartOcx::SetHtsPath(strHtsPath);
	CDRChartOcx::SetContainerDllInfo((long)p_lDllWnd,GetDlgCtrlID());
	// �������� TR���� ��Ŷ�� ó�� Real ó�� ���� - ojtaso (20080804)
	if(((CStdDialog*)m_pStdDialog)->m_nScreenType == CStdDialog::Screen_fluctuation)
		CDRChartOcx::UseDummyPacketData(TRUE);

	SetOutsideCommonInterface(((CChartMng*)GetParent())->m_lMltchrtDlgParentClsPointer); 
	SetAllProperties(bNoToolBar);	


// 	long nOption = CKBChart::GetNNumericalInquiryOption();
// 	if(nOption & TOOLTIPVIEW)
// 	{
// 		nOption &= ~TOOLTIPVIEW;
// 		CKBChart::SetNNumericalInquiryOption(nOption);
// 	}

	// ���������� ������� : �������� - ojtaso (20070904)
	SetMultiItemMode(((CStdDialog*)m_pStdDialog)->IsMultiItemChart());

#ifdef _DEBUG
	//LoadAddIn("ChartPatternAddin.dll");
	//LoadAddIn("ChartPeriodSummaryAddIn.dll:PERIOD_SUMMARY");
	LoadAddIn("ChartGuideLineAddin.dll:GUIDE_LINE");
#else	
	//�ʿ�� �ε���
	//LoadAddIn("ChartPatternAddin.dll");
	//LoadAddIn("ChartPeriodSummaryAddIn.dll:PERIOD_SUMMARY");
	LoadAddIn("ChartGuideLineAddin.dll:GUIDE_LINE");
#endif
	//���� :KHD
	m_bAddInLoadSTIndex = TRUE;
	LoadAddIn("ChartSTIndexAddIn.dll");
	InvokeAddIn("ChartSTIndexAddIn.dll", 3, (long)this);

	//{{ 2007.06.26 by LYH �ֹ���, Excel Import �߰�
	LoadAddIn("ChartPeriodSummaryAddIn.dll:ORDER_LINE");
	LoadAddIn("ChartDataListViewerAddIn.dll:EXCEL_IMPORT");
	//}}
	//{{ 2007.06.26 by LYH Undo,Redo �߰�
	LoadAddIn("ChartPeriodSummaryAddIn.dll:ORDER_LINE");
	LoadAddIn("ChartCommonAddIn.dll:UNDO_REDO_MANAGER;");
	//}}

//�ϴ� ������ ��ȸ
	LoadAddInWithOverExclusive( "ChartCommonAddIn.dll:MCHART_DVIEW;");
	InvokeAddIn( "ChartCommonAddIn.dll:MCHART_DVIEW", 0, ( long)((CChartMng*)GetParent())->m_lMltchrtDlgParentClsPointer);

	//�ڵ���ũ��
	LoadAddIn("ChartCommonAddIn.dll:AUTO_SCROLL");
	LoadAddIn("ChartCommonAddIn.dll:PRICE_YSCALE_SETUP");
	LoadAddInWithOverExclusive("ChartAnalysisToolAddIn.dll:NIDLG_OVER");

	// Set Pointer & StdDialog Handle
	m_listAddIn.AddTail("ChartGuideLineAddin.dll:GUIDE_LINE");
	SendMessageToInvoke(INVOKEMSG_INIT,NULL,0);
}


BOOL CChartItem::SetAllProperties(BOOL bNoToolbar /*=FALSE*/)
{
	CStringList strlistSetAll;
	if(bNoToolbar)
	{
		for(int i=0;i<sizeof(szSetAllNoToolbar)/sizeof(LPCTSTR);i++)
			strlistSetAll.AddTail(szSetAllNoToolbar[i]);	
	}
	else
	{
		for(int i=0;i<sizeof(szSetAll)/sizeof(LPCTSTR);i++)
			strlistSetAll.AddTail(szSetAll[i]);	
	}

	//===========================================================================
	// 2005. 09. 06 added by sy, nam
	// ���� �븮�� ��û�� 
	// ���� ���� �����ؼ�.
	//===========================================================================
	//"MAPNAME" : �ʹ�ȣ"BONGMG" : 4 (�ֽ�,����,����,�ɼ�)"BONGMG" : 0 (����Ʈ)

/*	CString strMapName, strBongGMG;
	
	switch(m_nMarketType)
	{
	case CMasterTypeInfo::STOCK_CHART	:	
		strMapName = "MAPNAME=M157000";		break;
	case CMasterTypeInfo::FUTOPT_CHART:
		strMapName = "MAPNAME=M280000";		break;
	case CMasterTypeInfo::UPJONG_CHART:
		strMapName = "MAPNAME=M158000";		break;
	}

	strlistSetAll.AddTail(strMapName);
	
	strBongGMG = "BONGMG=0";
	strlistSetAll.AddTail(strBongGMG);*/
	//===========================================================================

	CDRChartOcx::SetAllProperties2((long)&strlistSetAll);
	strlistSetAll.RemoveAll();
	//for(int i=0;i<sizeof(szSetAllForSignalnew)/sizeof(LPCTSTR);i++)
	//	strlistSetAll.AddTail(szSetAllForSignalnew[i]);	
	//Han Hwa        m_signal.SetAllProperties2((long)&strlistSetAll);

	return TRUE;
}

void CChartItem::ResetOCX(BOOL bNotCodeInputInit/*= TRUE*/)//���� �ڵ� �ʱ�ȭ�� Default, TRUE�̸� �ʱ�ȭ ���Ѵ�.
{	
	Init_ChartCfgInfo();
	ResetData();
	DestoryItemData();
	GetParent()->Invalidate();	
	CDRChartOcx::ShowInnerToolBar(FALSE);
	//CKBChart::EmptyChart(TRUE, FALSE);	
	CString strChartList =	"BlockColumn;\r\n"
		"Block;\r\n"
		"�ڷ�����;4;2;/;:;0;0;0;\r\n"
		"VertScaleGroup;\r\n"
		"��ǥ Data;1;2;0;\r\n"
		"VertScaleGroupEnd;\r\n"
		"BlockEnd;\r\n"
		"BlockColumnEnd;";
	CDRChartOcx::EmptyChart2(strChartList, TRUE, FALSE);
	m_bBuild = FALSE;

	if(bNotCodeInputInit == FALSE)
	{
		 //�����ʱ�ȭ ���� �ʴ´�. ��Ʈ�� �ε���� �� �ѹ��� �Ҹ���.
		((CChartMng*)GetParent())->SendInitChartData(FALSE);
	}
	else
		((CChartMng*)GetParent())->SendInitChartData();
	
	
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	�ִ�ȭ ��ư, �ݱ��ư�� ��ġ�� ����ش�
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetButton(const LPRECT lpRect)
{
	//long lTrCode = atol(GetDataTRCode());
	CString strTRCode = GetDataTRCode();
	CRect rect = lpRect;
	CRect rectMaxmize(0,0,0,0);
	CRect rectInit(0,0,0,0);
	CRect rectSignal(0,0,0,0);
	CRect rectCheck(0,0,0,0);
	
	if(!rect.IsRectNull())
	{	
		rectMaxmize.left  = rect.right-WIDTHOFTITLEBUTTON-POSXOFMAXMIZE;
		rectMaxmize.top   = rect.top + POSYOFMAXMIZE;
		rectMaxmize.right = rect.right-POSXOFMAXMIZE;
		rectMaxmize.bottom = rect.top + POSYOFMAXMIZE + HEIGHTOFTITLEBUTTON;

	    rectInit.left =  rect.right-WIDTHOFTITLEBUTTON-POSXOFINIT;
		rectInit.top =	rect.top + POSYOFINIT;
		rectInit.right = rect.right-POSXOFINIT;
		rectInit.bottom = rect.top + POSYOFINIT + HEIGHTOFTITLEBUTTON;
		
		if(strTRCode==QUERY_STRJONGMOK)
		{
			rectSignal.left = rect.left;
//			rectSignal.top = rect.top + rect.Height()/2 - SIZEOFSIGNAL/2;
			rectSignal.top = rect.top + HEIGHTOFCAPTION/2 - SIZEOFSIGNAL/2;// changed by gm0604 2005.10.10
			rectSignal.right = rectSignal.left;// + SIZEOFSIGNAL;
			rectSignal.bottom = rectSignal.top + SIZEOFSIGNAL;

			rectCheck.left = rectSignal.right + 1;
//			rectCheck.top = rect.top + rect.Height()/2 - SIZEOFCHECK/2;
			rectCheck.top = rect.top + HEIGHTOFCAPTION/2 - SIZEOFCHECK/2;// changed by gm0604 2005.10.10
			rectCheck.right = rectCheck.left + SIZEOFCHECK;
			rectCheck.bottom = rectSignal.top + SIZEOFCHECK;
		}
		else
		{
			rectSignal.SetRect(0,0,0,0);
			rectCheck.SetRect(0,0,0,0);
		}
	}
	if(m_btInit.GetSafeHwnd())
	{
		m_btInit.MoveWindow(rectInit);
		m_btInit.Invalidate();	
	}
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ�� Title�� Draw�Ѵ�.
//////////////////////////////////////////////////////////////////////////////
/*
void CChartItem::DrawTitle(CRect rectIn,CDC* pDC,CFont *pFt, CFont *pFtNormal, COLORREF crBackIn )
{
	// �������� �ǽð� : �������� - ojtaso (20070523)
	ST_CHARTITEM_REALDATA* pChartItemRealData;
	if(!m_mapChartItemRealData.Lookup(m_strSelectedItemCode, (LPVOID&)pChartItemRealData))
		return;

	// �������� �ǽð� : �������� - ojtaso (20070523)
	m_strKorName = pChartItemRealData->m_strItemName;

	CRect rect = rectIn;
	//	rect.left += SIZEOFSIGNAL + SIZEOFCHECK + 1;
	//	rect.right -= (WIDTHOFTITLEBUTTON+POSXOFMAXMIZE);

	//Han Hwa
	//�ֽ� ��Ʈ�� ��ȣ ���� 
	//if( m_signal.GetSafeHwnd() && (m_nMarketType == CMasterTypeInfo::STOCK_CHART) && m_bUseInnerControl) 
	//{ 
	//	rect.left += SIZEOFSIGNAL;		
	//}

	if(m_btInit.GetSafeHwnd())
	{
		rect.right -= WIDTHOFTITLEBUTTON;
	}
	if(m_btMaxmize.GetSafeHwnd())
	{
		rect.right -= POSXOFMAXMIZE;
	}	

	COLORREF crText = NULL;
	COLORREF crTextSise = NULL;
	COLORREF crBackSise = NULL;
	///////////////////////////////////////////////////////////////////////////////	
	CFont *pFtDefault = (CFont *)pDC->SelectObject(pFt);

	CRect rectTitlePos;
	CRect rectTitleType1;	
	CRect rectTitleType2;
	CRect rectTitleJongmok;

	CRect rectTitleName;
	CRect rectTitlePrice;
	CRect rectTitleChange;
	CRect rectTitleRate;
	CRect rectTitleVolume;
	CRect rectTitleOpenHighLow;

	CString strTitlePos;

	CString strTitleName;
	CString strTitlePrice;
	CString strTitleChange;
	CString strTitleRate;
	CString strTitleVolume;
	CString strTitleOpenHighLow;

	CString strTitleType;

	CSize szTitlePos;

	if(m_bUseInnerControl)
	{
		long lCols = ((CChartMng*)GetParent())->GetCols();
		long lCntOfChart = ((CChartMng*)GetParent())->GetCntOfChart();
		strTitlePos.Format("[%d / %d]", m_lColIndex + 1 + m_lRowIndex*lCols,lCntOfChart);
		szTitlePos = pDC->GetOutputTextExtent(strTitlePos);
		rectTitlePos.SetRect(rect.right-szTitlePos.cx,rect.top+4,rect.right,rect.bottom);
	}
	else
	{
		szTitlePos.cx = 0;
		szTitlePos.cy = 0;
	}
	BOOL bShortTitleMode = FALSE;
	if(!m_strKorName.IsEmpty())
	{
		//long lTrCode = atol(GetDataTRCode());
		CString strTRCode = GetDataTRCode();
		CString strSign = "";
		switch(pChartItemRealData->m_chSign)
		{
		case DOWN:			// Down
			strSign = "��";			
			if(pChartItemRealData->m_strChange.GetLength()>0 && pChartItemRealData->m_strChange[0] == '-')
				pChartItemRealData->m_strChange = pChartItemRealData->m_strChange.Mid(1);
			break;
		case DOWN_LIMIT:	// DownLimit
			strSign = "��";
			if(pChartItemRealData->m_strChange.GetLength()>0 && pChartItemRealData->m_strChange[0] == '-')
				pChartItemRealData->m_strChange = pChartItemRealData->m_strChange.Mid(1);
			break;	
		case UP:
			strSign = "��";			
			break;
		case UP_LIMIT:
			strSign = "��";			
			break;
		}
		if(strTRCode==QUERY_STRUPJONG||strTRCode==QUERY_STRFUTOPT)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
		{			
			strTitleName.Format(" %s ",m_strKorName);
			strTitlePrice.Format(" %s ",::SetComma(pChartItemRealData->m_strPrice));
			strTitleChange.Format("%s %s (%s%%) ",strSign, pChartItemRealData->m_strChange, pChartItemRealData->m_strChrate);
			//strTitleRate.Format(" %s%% ",m_strChrate);
			strTitleVolume.Format(" %s ",::SetComma(pChartItemRealData->m_strVolume));

			if(pChartItemRealData->m_strOpen.GetLength()>0)
				strTitleOpenHighLow.Format("��:%s ��:%s ��:%s", ::SetComma(pChartItemRealData->m_strOpen), ::SetComma(pChartItemRealData->m_strHigh), ::SetComma(pChartItemRealData->m_strLow));
		}
		else
		{
			strTitleName.Format(" %s ",m_strKorName);
			
			//{{ 2007.06.26 by LYH ����ü���� ��� ���簡 �տ� (��) ����
			if(m_chSign == '9')	//����ü��
				strTitlePrice.Format("(��) %s ",::SetComma(pChartItemRealData->m_strPrice));
			else
			    strTitlePrice.Format(" %s ",::SetComma(pChartItemRealData->m_strPrice));
			//}}

			strTitleChange.Format("%s %s (%s%%) ",strSign, ::SetComma(pChartItemRealData->m_strChange,TRUE), pChartItemRealData->m_strChrate);
			//strTitleRate.Format(" %s%% ",m_strChrate);
			strTitleVolume.Format(" %s ",::SetComma(pChartItemRealData->m_strVolume));
			if(pChartItemRealData->m_strOpen.GetLength()>0)
				strTitleOpenHighLow.Format("��:%s ��:%s ��:%s", ::SetComma(pChartItemRealData->m_strOpen), ::SetComma(pChartItemRealData->m_strHigh), ::SetComma(pChartItemRealData->m_strLow));
		}

		//		OutputDebugString("DrawTitle �����	: "  + strTitleName);
		//		OutputDebugString("DrawTitle ����	: "  +strTitlePrice);
		//		OutputDebugString("DrawTitle �����	: "  +strTitleChange);
		//		OutputDebugString("DrawTitle ����� : "  +strTitleRate);
		//		OutputDebugString("DrawTitle �ŷ��� : "  +strTitleVolume);




		CSize szTitleName = pDC->GetOutputTextExtent(strTitleName);
		CSize szTitlePrice = pDC->GetOutputTextExtent(strTitlePrice);
		CSize szTitleChange = pDC->GetOutputTextExtent(strTitleChange);
		CSize szTitleRate = pDC->GetOutputTextExtent(strTitleRate);
		CSize szTitleVolume = pDC->GetOutputTextExtent(strTitleVolume);
		CSize szTitleOpenHighLow = pDC->GetOutputTextExtent(strTitleOpenHighLow);

		CRect rectShift;
		CRect rectUnion;
		CRect rectUnionTitle;
		BOOL  bSetEmpty = FALSE;

		rectUnionTitle.SetRectEmpty();
		rectShift = rect;
		rectShift.top += 4;
		rectShift.OffsetRect(1,0);

		rectTitleName.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleName.cx,rectShift.top + szTitleName.cy);
		rectUnionTitle.UnionRect(rectUnionTitle,rectTitleName);

		rectShift.left = rectTitleName.right + 2;
		rectTitlePrice.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitlePrice.cx,rectShift.top + szTitlePrice.cy);
		rectUnion.UnionRect(rect,rectTitlePrice);
		if(rectUnion!=rect)
		{
			rectTitlePrice.SetRectEmpty();
			bSetEmpty = TRUE;
		}
		else
		{
			rectUnionTitle.UnionRect(rectUnionTitle,rectTitlePrice);
		}

		rectShift.left = rectTitlePrice.right + 2;
		rectTitleChange.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleChange.cx,rectShift.top + szTitleChange.cy);
		rectUnion.UnionRect(rect,rectTitleChange);
		if(rectUnion!=rect||bSetEmpty)
		{
			rectTitleChange.SetRectEmpty();
			bSetEmpty = TRUE;
		}
		else
		{
			rectUnionTitle.UnionRect(rectUnionTitle,rectTitleChange);
		}

		rectShift.left = rectTitleChange.right + 2;
		//rectTitleRate.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleRate.cx,rectShift.top + szTitleRate.cy);
		//rectUnion.UnionRect(rect,rectTitleRate);
		//if(rectUnion!=rect||bSetEmpty)
		//{
		//	rectTitleRate.SetRectEmpty();
		//	bSetEmpty = TRUE;
		//}
		//else
		//{
		//	rectUnionTitle.UnionRect(rectUnionTitle,rectTitleRate);
		//}
		//rectShift.left = rectTitleRate.right + 2;
		rectTitleVolume.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleVolume.cx,rectShift.top + szTitleVolume.cy);
		rectUnion.UnionRect(rect,rectTitleVolume);
		if(rectUnion!=rect||bSetEmpty)
		{
			rectTitleVolume.SetRectEmpty();
			bSetEmpty = TRUE;
		}
		else
		{
			rectUnionTitle.UnionRect(rectUnionTitle,rectTitleVolume);
		}

		rectTitleJongmok = rectUnionTitle;
		rectTitleJongmok.InflateRect(2,3);
		if(rectTitleJongmok!=m_rectTitleJongmok)
		{
			m_rectTitleJongmokPast = m_rectTitleJongmok;
			m_rectTitleJongmok = rectTitleJongmok;		
		}

		rectShift.left = rectTitleVolume.right + 5;
		rectTitleOpenHighLow.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleOpenHighLow.cx,rectShift.top + szTitleOpenHighLow.cy);
		rectUnion.UnionRect(rect,rectTitleOpenHighLow);
		if(rectUnion!=rect||bSetEmpty)
		{
			rectTitleOpenHighLow.SetRectEmpty();
			bSetEmpty = TRUE;
		}

	}
	char chType = NULL;

	if(m_chType == NULL)
		if(m_chTypeForRequest == NULL)		
			chType = DAY_DATA_CHART;
		else
			chType = m_chTypeForRequest;
	else
		chType = m_chType;


	//	chType = m_chTypeForRequest;	
	//	if(chType == NULL) chType = DAY_DATA_CHART;

	if(chType!=NULL)
	{

		//		CString sTemp;
		//		sTemp.Format("DrawTitle_ChartType:%c_Tick:%s_Min:%s_Day:%s",
		//						chType,
		//						(LPTSTR)(LPCTSTR)m_strTick,
		//						(LPTSTR)(LPCTSTR)m_strMin,
		//						(LPTSTR)(LPCTSTR)m_strDay						
		//						);
		//		OutputDebugString(sTemp);

		// ƽ(0)/��(1)/��(2)/��(3)/��(4)
		switch(chType)
		{
		case TICK_DATA_CHART:
			//strTitleType.Format("[%sƽ]",(LPTSTR)(LPCTSTR)m_strTick);
			strTitleType.Format("[%dƽ]",atoi((LPCTSTR)m_strTick));
			break;
		case MIN_DATA_CHART:
			//strTitleType.Format("[%s��]",(LPTSTR)(LPCTSTR)m_strMin);
			if("300" == m_strMin) // 30��
				strTitleType.Format("[30��]");
			else
				strTitleType.Format("[%d��]",atoi((LPCTSTR)m_strMin));
			break;
		case DAY_DATA_CHART:
			//strTitleType.Format("[%s��]",(LPTSTR)(LPCTSTR)m_strDay);
			strTitleType.Format("[%d��]",atoi((LPCTSTR)m_strDay));
			break;
		case HOUR_DATA_CHART:
			strTitleType.Format("[%d��]", atoi((LPCTSTR)m_strHour));
			break;
		case WEEK_DATA_CHART:
			strTitleType.Format("[��]");
			break;
		case MONTH_DATA_CHART:
			strTitleType.Format("[��]");
			break;
		case YEAR_DATA_CHART:
			strTitleType.Format("[��]");
			break;
		case SEC_DATA_CHART:
			strTitleType.Format("[��]");
			break;
//		case SEC10_DATA_CHART:
//			strTitleType.Format("[10��]");
//			break;
//		case SEC30_DATA_CHART:
//			strTitleType.Format("[30��]");
//			break;

		default:
			strTitleType = _T("[]");
			break;
		}

		if(strTitleType.GetLength())
		{
			CSize szTitleType = pDC->GetOutputTextExtent(strTitleType);
			rectTitleType1.SetRect(rect.right-szTitleType.cx-szTitlePos.cx,rect.top+4,rect.right-szTitlePos.cx,rect.bottom);
			rectTitleType2.SetRect(rect.right-szTitleType.cx,rect.top+4,rect.right,rect.bottom);
		}

		if(rectTitleType1.left < rectTitleOpenHighLow.right)
		{
			rectTitleOpenHighLow.SetRectEmpty();
		}

	}	

	///////////////////////////////////////////////////////////////////////////////
	if(!m_strKorName.IsEmpty())
	{
		CRect rectInflate;
		///////////////////////////////////////////////////////////////////////////////
		//	JSR-20051128 ADD Begin.
		// �������� ����map�� �̿� : �������� - ojtaso (20070612)
		switch(pChartItemRealData->m_chSign)
		{
		case UP_LIMIT:	// UpLimit
			crText = RGB(255,255,255);
			crTextSise = RGB(255,255,255);
			crBackSise = COLOR_UP;
			break;
		case UP:	// Up
			crText = COLOR_STEADY;
			crTextSise = COLOR_UP;
			break;
		case DOWN:	// Down
			crText = COLOR_STEADY;
			crTextSise = COLOR_DOWN;
			break;
		case DOWN_LIMIT:	// DownLimit
			crText = RGB(255, 255, 255);
			crTextSise = RGB(255,255,255);
			crBackSise = COLOR_DOWN;
			break;
		default://'3':	// Steady
			crText = COLOR_STEADY;
			crTextSise = COLOR_STEADY;
			break;
		}
		pDC->SetTextColor(crText);
		//	JSR-20051128 ADD End.
		///////////////////////////////////////////////////////////////////////////////		
		if(!crBackSise)
		{
			pDC->SetBkMode(TRANSPARENT);							// JSR-20051128 ADD
			rectInflate = rectTitleName;
			rectInflate.InflateRect(1,3);
			//			pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);		JSR-20051128 Modify
		}
		else
		{
			rectInflate = rectTitleName;
			rectInflate.InflateRect(1,3);							// JSR-20051128 Modify
			pDC->FillSolidRect(rectInflate,crBackSise);
		}
		pDC->TextOut(rectTitleName.left,rectTitleName.top,strTitleName);

		//		if(!crBackSise)
		//		{
		//////////// DC �ʱⰪ �ٽ� ���� JSR-20051128 ADD Begin. //////////////////////
		COLORREF crText = NULL;
		COLORREF crTextSise = NULL;
		COLORREF crBackSise = NULL;
		pDC->SetBkMode(TRANSPARENT);

		// �������� ����map�� �̿� : �������� - ojtaso (20070612)
		switch(pChartItemRealData->m_chSign)
		{
		case UP_LIMIT:	// UpLimit
		case UP:	// Up
			crText = COLOR_UP;
			crTextSise = COLOR_UP;
			break;
		case DOWN:	// Down
		case DOWN_LIMIT:	// DownLimit
			crText = COLOR_DOWN;
			crTextSise = COLOR_DOWN;
			break;
		//{{ 2007.06.26 by LYH ����ü�� ����
		case '9':	// ����ü��
			{
				int nChange = atoi(m_strChange);
				if( nChange > 0 )
				{
					crText = COLOR_UP;
					crTextSise = COLOR_UP;
				}
				else if(nChange < 0)
				{
					crText = COLOR_DOWN;
					crTextSise = COLOR_DOWN;
				}
				else
				{
					crText = COLOR_STEADY;
					crTextSise = COLOR_STEADY;
				}
			}
			break;
		//}}
		default://'3':	// Steady
			crText = COLOR_STEADY;
			crTextSise = COLOR_STEADY;
			break;
		}
		pDC->SetTextColor(crText);
		// JSR-20051128 ADD End.
		///////////////////////////////////////////////////////////////////////////////
		if(!rectTitlePrice.IsRectEmpty())
		{
			rectInflate = rectTitlePrice;
			rectInflate.InflateRect(1,3);
			//				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);				JSR-20051128 Modify
			pDC->TextOut(rectTitlePrice.left,rectTitlePrice.top,strTitlePrice);		
		}
		if(!rectTitleChange.IsRectEmpty())
		{
			rectInflate = rectTitleChange;
			rectInflate.InflateRect(1,3);
			//				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);				JSR-20051128 Modify
			pDC->TextOut(rectTitleChange.left,rectTitleChange.top,strTitleChange);
		}
		//			if(!rectTitleRate.IsRectEmpty())
		//			{
		//				rectInflate = rectTitleRate;
		//				rectInflate.InflateRect(1,3);
		//				pDC->TextOut(rectTitleRate.left,rectTitleRate.top,strTitleRate);
		//			}
		pDC->SetTextColor(COLOR_STEADY);
		if(!rectTitleVolume.IsRectEmpty())
		{
			rectInflate = rectTitleVolume;
			rectInflate.InflateRect(1,3);
			//				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);				JSR-20051128 Modify
			pDC->TextOut(rectTitleVolume.left,rectTitleVolume.top,strTitleVolume);
		}

		if(!rectTitleOpenHighLow.IsRectEmpty())
		{
			rectInflate = rectTitleOpenHighLow;
			rectInflate.InflateRect(1,3);
			//				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);				JSR-20051128 Modify
			pDC->TextOut(rectTitleOpenHighLow.left,rectTitleOpenHighLow.top,strTitleOpenHighLow);
		}
		//		}

		///////////////////////////////////////////////////////////////////////////////
		if(m_lPtrST)
		{
			pDC->TextOut(rectTitleName.left,rectIn.top+HEIGHTOFCAPTION+4,m_strName);		
		}		
	}

	CRect rectInter11;
	CRect rectInter12;
	CRect rectInter2;
	rectInter11.IntersectRect(m_rectTitleJongmok,rectTitleType1);
	rectInter12.IntersectRect(m_rectTitleJongmok,rectTitleType2);
	rectInter2.IntersectRect(m_rectTitleJongmok,rectTitlePos);

	//	pDC->SetBkMode(TRANSPARENT);					JSR-20051202 STOP
	//	pDC->SetTextColor(crText);						JSR-20051202 STOP

	pFtDefault = (CFont *)pDC->SelectObject(pFtNormal);
	if(rectInter11.IsRectEmpty())
	{
		pDC->TextOut(rectTitleType1.left,rectTitleType1.top,strTitleType);
		if(rectTitleType1!=m_rectTitleType)
		{
			m_rectTitleTypePast = m_rectTitleType;
			m_rectTitleType = rectTitleType1;
		}
		if(rectInter2.IsRectEmpty())
		{
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,strTitlePos);			
			if(rectTitlePos!=m_rectTitlePos)
			{
				m_rectTitlePosPast = m_rectTitlePos;
				m_rectTitlePos = rectTitlePos;
			}

		}	
	}
	else if(rectInter12.IsRectEmpty())
	{
		pDC->TextOut(rectTitleType2.left,rectTitleType2.top,strTitleType);
		if(rectTitleType2!=m_rectTitleType)
		{
			m_rectTitleTypePast = m_rectTitleType;
			m_rectTitleType = rectTitleType2;
		}		
	}
	else
	{
		if(rectInter2.IsRectEmpty())
		{
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,strTitlePos);			
			if(rectTitlePos!=m_rectTitlePos)
			{
				m_rectTitlePosPast = m_rectTitlePos;
				m_rectTitlePos = rectTitlePos;
			}
		}	
	}
	pDC->SelectObject(pFtDefault);
}
*/

//void CChartItem::DrawTitle(CRect rectIn,CDC* pDC,CFont *pFt, COLORREF crBackIn )
void CChartItem::DrawTitle(CRect rectIn,CDC* pDC,CFont *pFt, CFont *pFtNormal, COLORREF crBackIn )
{
	m_chCompareChartType = m_pShareInputData->m_DataInputBar.m_cChartShape;
	CRect rect = rectIn;

	COLORREF crText = NULL;
	COLORREF crTextSise = NULL;
	COLORREF crBackSise = NULL;

	crText = RGB(219,47,43);
	crTextSise = RGB(255,255,255);
	crBackSise = RGB(219,47,43);
	rect.left += 1;
	

	if(m_btInit.GetSafeHwnd())
	{
		rect.right -= WIDTHOFTITLEBUTTON;
	}
	
	CRect rectTitlePos;
	CRect rectTitleType1;	
	CRect rectTitleType2;
	CRect rectTitleJongmok;

	CRect rectTitleName;
	CRect rectTitlePrice;
	CRect rectTitleChange;
	CRect rectTitleRate;
	CRect rectTitleVolume;

	CString strTitlePos;

	CString strTitleName;
	CString strTitlePrice;
	CString strTitleChange;
	CString strTitleRate;
	CString strTitleVolume;

	CString strTitleType;

	BOOL bShortTitleMode = FALSE;

	switch(m_chCompareChartType)
	{
		case DIVIDED_COMPARE_CHART:
			 strTitleName = "���� ����Ʈ";
			 break;
		case OVERLAPP_COMPARE_CHART:			
			strTitleName = "��ø ����Ʈ";
			break;
		case NUJUK_OVERLAPP_COMPARE_CHARTA:	
		case NUJUK_OVERLAPP_COMPARE_CHARTB:
		case NUJUK_OVERLAPP_COMPARE_CHARTC:
			strTitleName = "��������� ��ø ����Ʈ";
			break;
		case SUIK_COMPARE_CHART:				
			strTitleName = "�Ⱓ�� ���ͷ� ����Ʈ";
			break;
		case TOT_NUJUK_COMPARE_CHART:			
			strTitleName = "�ѱⰣ ��������� ����Ʈ";
			break;
	}	

	CSize szTitleName = pDC->GetOutputTextExtent	(strTitleName);

	
	CRect rectShift;
	CRect rectUnion;
	CRect rectUnionTitle;
	BOOL  bSetEmpty = FALSE;
	
	rectUnionTitle.SetRectEmpty();
	rectShift = rect;
	rectShift.top += 4;
	rectShift.OffsetRect(1,0);

	rectTitleName.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleName.cx,rectShift.top + szTitleName.cy);
	rectUnionTitle.UnionRect(rectUnionTitle,rectTitleName);


	char chType = NULL;
	if(m_chType == NULL)
		if(m_chTypeForRequest == NULL)		
			chType = DAY_DATA_CHART;
		else
			chType = m_chTypeForRequest;
		else
			chType = m_chType;


	if(chType == NULL) 
	{
		AfxMessageBox("chType is NULL");
		return;
	}

	if(chType!=NULL)
	{	CString sTemp;
		// ƽ(0)/��(1)/��(2)/��(3)/��(4)
		switch(chType)
		{
		case MIN_DATA_CHART:
			
			sTemp.Format("MinDay 9: %s", m_strMin);
			OutputDebugString(sTemp);
			strTitleType.Format("[%s��]",(LPTSTR)(LPCTSTR)m_strMin);
			break;
		case DAY_DATA_CHART:			
			sTemp.Format("MinDay 8: %s", m_strDay);
			OutputDebugString(sTemp);
			strTitleType.Format("[%s��]",(LPTSTR)(LPCTSTR)m_strDay);
			break;
		case WEEK_DATA_CHART:
			strTitleType.Format("[��]");
			break;
		case MONTH_DATA_CHART:
			strTitleType.Format("[��]");
			break;
		case YEAR_DATA_CHART:
			strTitleType.Format("[��]");
			break;
		case SEC_DATA_CHART:
			strTitleType.Format("[��]");
			break;
		case HOUR_DATA_CHART:
			strTitleType.Format("[��]");
			break;
		}

		if(strTitleType.GetLength())
		{
			//pFtDefault = (CFont *)pDC->SelectObject(pFtNormal);
			pDC->SelectObject(pFtNormal);
			CSize szTitleType = pDC->GetOutputTextExtent(strTitleType);			
			rectTitleType2.SetRect(rect.right-szTitleType.cx-2 ,rect.top+4, rect.right, rect.bottom);
		}
	}

	CFont *pFtDefault = (CFont *)pDC->SelectObject(pFt);
	pDC->TextOut(rectTitleName.left,rectTitleName.top,strTitleName);

	pDC->SelectObject(pFtNormal);
	pDC->TextOut(rectTitleType2.left,rectTitleType2.top,strTitleType);

	pDC->SelectObject(pFtDefault);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ��ǥ�� �߰��Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::AddChartBlock(LPCTSTR lpChartName)
{
	if(m_bInitalChart)
	{
		CString strPnF = "P&&F";
		if(strPnF==lpChartName)
		{
			lpChartName = "P&F";
		}

//		CString sTemp;
//		sTemp.Format("AddChartBlock Called : %s", lpChartName);
//		OutputDebugString(sTemp);

		return CDRChartOcx::AddChartBlock( lpChartName);
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	�ش���Ʈ��ǥ�� �ִ��� Ȯ���Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::HasGraph(LPCTSTR p_szGraphName)
{
	// Default RQ �˻� - ojtaso (20080416)
	if(m_strSelectedRQ != _T("DEFAULT"))
		return TRUE;
	
	CString strPnF = "P&&F";
	if(strPnF==p_szGraphName)
	{
		p_szGraphName = "P&F";
	}
	if(GetOnRunningOneChart()) // if Chart Item In View is P&F, etc..., 
	{
		return CDRChartOcx::HasGraphInSpecialBack(p_szGraphName);
	}
	return CDRChartOcx::HasGraph( p_szGraphName);
}

BOOL CChartItem::DestroyWindow()
{

	//���� �������� Real�� ���� �ϹǷ�  ======================================
	//������ �ڵ带 ���� �ڵ�� ������ �ΰ�..	
	//2005. 09. 06
	SetOldChartCode(GetDataCode()); 
	((CChartMng*)GetParent())->OnChartMngCodeUnAdvise(this, IsAddMode());

	//@.080728-001 ��ȸ������ ��������.
	((CChartMng*)GetParent())->OnChartMngCodeScdClear(this, IsAddMode());
	//========================================================================


	// Call InvokeAddIn Func. for ChartPatternAddin ------------------------------->
	// (��Ʈ���Ϻм� dll���� Modal Dialog�� �����Ǿ� �ִ°�� ���� �����Ű�� ����)
	// wParam - 0:Create, 1:Destroy
	if(m_bAddInLoadPattern)
		InvokeAddInPattern("ChartPatternAddin.dll",NULL,0);
	// <----------------------------------------------------------------------------

#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::ClearData::Begin", _T("����Ʈ ����"), 13);
#endif
	ClearData();
#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::ClearData::End", _T("����Ʈ ����"), 13);
	WriteToStringLog("DestroyWindow::SetDrdsAdvise::Begin", _T("����Ʈ ����"), 13);
#endif
	SetDrdsAdvise(FALSE);
#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::SetDrdsAdvise::End", _T("����Ʈ ����"), 13);
#endif
	DestoryItemData();	

	m_btInit.DestroyWindow();

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestroyWindow::End", _T("����Ʈ ����"), 13);
#endif
	//Han Hwa        m_signal.OffDrdsAdvise();
	return CDRChartOcx::DestroyWindow();
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��ȸ�� Tick/Min/Day ��ġ.������ Set�Ѵ�.
//  
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetTickMinDayNumber(UINT nNumber)
{
	if(MIN_DATA_CHART == m_chTypeForRequest && 0 == nNumber)
		nNumber = 300;

	if(nNumber>=1000 || nNumber <= 0) 					
	{
		AfxMessageBox("1000������ ������ �ٽ� �����Ͽ� �ֽʽÿ�");
		return;
	}
	
	ClearData();
	m_strTickMinDayForRequest.Format("%03d",nNumber);
	
	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART:
			SetTickNumber(m_strTickMinDayForRequest);
			break;
		case MIN_DATA_CHART:
			SetMinNumber(m_strTickMinDayForRequest);
			break;
		case DAY_DATA_CHART:
			SetDayNumber(m_strTickMinDayForRequest);
			break;		
		case HOUR_DATA_CHART:
			SetHourNumber(m_strTickMinDayForRequest);
	}	
}



///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	�䱸�Ǿ����� �������� ������ ��¥�� Set�Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetEndDateRequested(LPCTSTR lpDate)
{	
	m_strDateForRequest = lpDate;
	if(m_strDateForRequest.GetLength()!=8)
	{
		CTime t = CTime::GetCurrentTime();
		m_strDateForRequest = t.Format("%Y%m%d");	
	}

	ClearData(); //05. 08. 29
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	�ǽð� ������ �ݿ��Ѵ�.
//	lDataKind = MarketType
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetRealPriceData(void *pData, long lDataKind)
{
//	if(m_pShareInputData->m_DataInputBar.m_cChartShape >= 'C')
	if(m_pShareInputData->m_DataInputBar.m_nUnitType == PERCENT_TYPE)
	{
		SetRealPriceDataFluct(pData, lDataKind);
		return;
	}

	BOOL bRet = FALSE;

	CString strSign, strTime;
	CString strItemCode;
	CRealData_Common* pRealData_Common = (CRealData_Common*)pData;

	// �ǽð� ���Ź��� �����ڵ� : �������� - ojtaso (20070108)
	m_strRealItemCode = pRealData_Common->m_strCode;

	// �������� ����map�� �̿� : �������� - ojtaso (20070612)
	POSITION pos = m_mapChartItem.GetStartPosition();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	BOOL bCurBase=FALSE;
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)pSelChart);

		strItemCode = pSelChart->m_strItemCode;

		if(lDataKind == FUTURE_REAL || lDataKind == STOCK_REAL)
			((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strItemCode);

		if(!::lstrcmp(strItemCode, m_strRealItemCode))
		{
			ST_CHARTITEM_REALDATA* pChartItemRealData;
			BOOL bRealItem = m_mapChartItemRealData.Lookup(m_strRealItemCode, (LPVOID&)pChartItemRealData);

			m_chSign = pRealData_Common->m_strSign.GetAt(0);
			if(bRealItem)
				pChartItemRealData->m_chSign = m_chSign;

			// ���簡 ������̸�
			CString strData;
			if(m_pShareInputData->m_nBase==2)
			{
				bCurBase = TRUE;
				// ���簡 ����� - ojtaso (20080602)
				if(bRealItem)
				{
					long lData1 = atol(pChartItemRealData->m_strPrice);
					long lData2 = atol(pRealData_Common->m_strPrice);
					long lData = abs(lData1 - lData2);
					double dData = double(lData) / double(lData2) * 100.;
					
//					if(pSelChart->m_strChartGubun == QUERY_STRJONGMOK)
					{
						if(lData1 < lData2)
							strData.Format(_T("-%.2f"), dData);
						else
							strData.Format(_T("%.2f"), dData);
					}
				}
			}			

			m_strPrice.Format("%ld", atol(pRealData_Common->m_strPrice));
			{
				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
				{
					double dData = atof(m_strPrice);
					m_strPrice.Format("%.2f", dData);
				}
				m_strPrice.TrimLeft();

				if(bRealItem)
					pChartItemRealData->m_strPrice = m_strPrice;
			}

			m_strOpen.Format("%ld", atol(pRealData_Common->m_strOpen));

			{
				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
				{
					double dData = atof(m_strOpen)/100.;
					m_strOpen.Format("%.2f", dData);
				}
				m_strOpen.TrimLeft();

				if(bRealItem)
					pChartItemRealData->m_strOpen = m_strOpen;
			}

			m_strHigh.Format("%ld", atol(pRealData_Common->m_strHigh));

			{
				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
				{
					double dData = atof(m_strHigh)/100.;
					m_strHigh.Format("%.2f", dData);
				}

				m_strHigh.TrimLeft();

				if(bRealItem)
					pChartItemRealData->m_strHigh = m_strHigh;
			}

			m_strLow.Format("%ld", atol(pRealData_Common->m_strLow));

			{
				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
				{
					double dData = atof(m_strLow)/100.;
					m_strLow.Format("%.2f", dData);
				}
				m_strLow.TrimLeft();

				if(bRealItem)
					pChartItemRealData->m_strLow = m_strLow;
			}

//@Solomon: --> �������.
//After
			m_strChange = pRealData_Common->m_strChange;
			if(bRealItem)
				pChartItemRealData->m_strChange = m_strChange;

//Before
// 			m_strChange.Format("%ld", atol(pRealData_Common->m_strChange));
// 			{
// 				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
// 				{
// 					double dData = atof(m_strChange)/100.;
// 					m_strChange.Format("%.2f", dData);
// 				}
// 
// 				if(m_strChange=="")
// 				{
// 					m_strChange = "0";
// 				}
// 				else if(m_strChange[0]=='-')
// 				{
// 					if(m_strChange[0]=='.')
// 					{
// 						m_strChange = '0' + m_strChange;
// 					}
// 				}
// 				else if(m_strChange[0]=='.')
// 				{
// 					m_strChange = '0' + m_strChange;
// 				}
// 
// 				if(bRealItem)
// 					pChartItemRealData->m_strChange = m_strChange;
// 			}
//@Solomon:<--

			m_strChrate.Format("%ld", atol(pRealData_Common->m_strChrate));
			{
				double dRate = atof(m_strChrate)/100.;
				m_strChrate.Format("%.2f", dRate);
				if(m_strChrate[0]=='-')
				{
					if(m_strChrate[0]=='.')
					{
						m_strChrate = '0' + m_strChrate;
					}
				}
				else if(m_strChrate[0]=='.')
				{
					m_strChrate = '0' + m_strChrate;
				}
				if(bRealItem)
					pChartItemRealData->m_strChrate = m_strChrate;
			}

			if(bRealItem)
			{
				strTime = pRealData_Common->m_strTime;
				{
					IRealReceiver *pIRealReceiver = ( IRealReceiver *)GetAddInCustomInterface( "ChartCommonAddIn.dll:DIRECT_REAL");

					BOOL bAppend = FALSE;
					strTime = strTime.Mid(0, 6);

					pIRealReceiver->SetRealDataRQ(strKey, "_DateTime_",		atof(strTime), bAppend);
					
					if(pSelChart->m_nChartNumericValueType == PERCENT_TYPE)
					{
						if(bCurBase && bRealItem) // ���簡 ����� ��Ʈ�϶�
							pIRealReceiver->SetRealDataRQ(strKey, "_Close_",			0., bAppend);
						else
							pIRealReceiver->SetRealDataRQ(strKey, "_Close_",			atof(pChartItemRealData->m_strChrate), bAppend);				
					}
					else
						pIRealReceiver->SetRealDataRQ(strKey, "_Close_",			atof(pChartItemRealData->m_strPrice), bAppend);

					BOOL ret2 = pIRealReceiver->SetRealPacketEndRQ(strKey);

					// ���簡 ����� - ojtaso (20080602)
					if(bCurBase && bRealItem) // ���簡 ����� ��Ʈ�϶�
						pIRealReceiver->OffsetPacketData(strKey, atof(strData));
					
					pIRealReceiver->Release();
				}
			}
		}
	}
	//end
	
//	//{{ 2006.10.09 by LYH ���� Repaint ���� �ذ�
//	if(!::lstrcmp(m_strRealItemCode, m_strSelectedItemCode))
//	{
//		CRect rectTemp;
//		if(rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
//			GetParent()->InvalidateRect(rectTemp);
//	}
//	//}}
}


void CChartItem::SetRealPriceDataFluct(void *pData, long lDataKind)
{
	BOOL bRet = FALSE;

	CString strSign, strTime;
	CRealData_Common* pRealData_Common = (CRealData_Common*)pData;

	// �ǽð� ���Ź��� �����ڵ� : �������� - ojtaso (20070108)
	m_strRealItemCode = pRealData_Common->m_strCode;

	// �������� ����map�� �̿� : �������� - ojtaso (20070612)
	POSITION pos = m_mapChartItem.GetStartPosition();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	BOOL bCurBase=FALSE;
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)pSelChart);
		if(!::lstrcmp(pSelChart->m_strItemCode, m_strRealItemCode))
		{
			ST_CHARTITEM_REALDATA* pChartItemRealData;
			BOOL bRealItem = m_mapChartItemRealData.Lookup(m_strRealItemCode, (LPVOID&)pChartItemRealData);

			m_chSign = pRealData_Common->m_strSign.GetAt(0);
			if(bRealItem)
				pChartItemRealData->m_chSign = m_chSign;

			// ���簡 ������̸�
			CString strData;
			if(m_pShareInputData->m_nBase==0)	// ������������
			{
				m_strChrate = pRealData_Common->m_strChrate;
				if(bRealItem)
				{
//@Solomon--> ���� �϶��϶� �ǽð�ǥ�� ��������.
//After
					if(pRealData_Common->m_strSign=="5")
					{
						pChartItemRealData->m_strChrate = "-";
						pChartItemRealData->m_strChrate += m_strChrate;
					}
					else
					{
						pChartItemRealData->m_strChrate = m_strChrate;
					}
				}

//Before
// 				m_strChrate.Format("%ld", atol(pRealData_Common->m_strChrate));
// 				{
// 					//double dRate = atof(m_strChrate)/100.;
// 					double dRate = atof(m_strChrate);
// 					m_strChrate.Format("%.2f", dRate);
// 					if(m_strChrate[0]=='-')
// 					{
// 						if(m_strChrate[0]=='.')
// 						{
// 							m_strChrate = '0' + m_strChrate;
// 						}
// 					}
// 					else if(m_strChrate[0]=='.')
// 					{
// 						m_strChrate = '0' + m_strChrate;
// 					}
// 
// 					if(bRealItem)
// 						pChartItemRealData->m_strChrate = m_strChrate;
// 					}
// 				}
//@Solomon<--
			}
			else if(m_pShareInputData->m_nBase==1)	// ���Ͻð�����
			{
				//BOOL bMinus = FALSE;
				m_strChrate.Format("%ld", atol(pRealData_Common->m_strChrate));
				//if(m_strChrate[0]=='-')		m_bMinus = TRUE;

				long lData1 = atol(pRealData_Common->m_strPrice);
				long lData2 = atol(pRealData_Common->m_strOpen);
				long lData = abs(lData1 - lData2);
				long lDataCmp = lData1-lData2;
				double dData = (double(lData) / double(lData2)) * 100.;
				//double dData = double(lData) / double(lData2);

				CString szValue;
				if(lDataCmp>0)			szValue.Format(_T("%f"), dData*100);
				else					szValue.Format(_T("-%f"), dData*100);
				szValue.TrimLeft();

				pRealData_Common->m_strChrate = szValue;
				//m_strChrate.Format("%ld", atol(pRealData_Common->m_strChrate));
				{
					if(bRealItem)
						pChartItemRealData->m_strChrate = szValue;
				}
			}
			else if(m_pShareInputData->m_nBase==2)	//���簡����
			{
				m_strPrice.Format("%ld", atol(pRealData_Common->m_strPrice));
				bCurBase = TRUE;
				// ���簡 ����� - ojtaso (20080602)
				if(bRealItem)
				{
					if(pSelChart->m_strChartGubun==QUERY_STRJONGMOK)
					{
						long lData1 = atol(pChartItemRealData->m_strPrice);
						long lData2 = atol(pRealData_Common->m_strPrice);
						long lData = abs(lData1 - lData2);
						double dData = double(lData) / double(lData2) * 100.;
						if(lData1 < lData2)
							strData.Format(_T("-%.2f"), dData);
						else
							strData.Format(_T("%.2f"), dData);
					}
					else
					{
						double lData1 = atof(pChartItemRealData->m_strPrice) * 100;
						double lData2 = atof(pRealData_Common->m_strPrice);
						double lData = fabs(lData1 - lData2);

						double dData = double(lData) / double(lData2) * 100.;
						if(lData1 < lData2)
							strData.Format(_T("-%.2f"), dData);
						else
							strData.Format(_T("%.2f"), dData);
					}
				}

				m_strPrice.Format("%ld", atol(pRealData_Common->m_strPrice));
				{
					if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
					{
						//double dData;
						//if( ((CStdDialog*)m_pStdDialog)->m_nScreenType==1)
						//	dData = atof(m_strPrice)/100.;
						double dData = atof(m_strPrice)/100.;
						//else
						//	dData = atof(m_strPrice);
						//double dData = atof(m_strPrice);
						m_strPrice.Format("%.2f", dData);
					}
					m_strPrice.TrimLeft();

					if(bRealItem)
						pChartItemRealData->m_strPrice = m_strPrice;
				}
			}
			//else	// ����Ʈ���� �����: ��ȸ���ۺ� ��������.
			//@.080730-001 ����Ʈ ������϶� 11
			if(m_pShareInputData->m_nBase==11)
			{
				CString rValue;
				((CStdDialog*)m_pStdDialog)->GetSCDData(pRealData_Common->m_strCode, rValue);
				//rValue = "28450"; 000660. 2008.06.18�Ϻ� �������ذ�.

				//m_strChrate.Format("%ld", atol(pRealData_Common->m_strPrice)/atol(rValue));
				//m_strChrate.Format("%ld", atol(pRealData_Common->m_strChrate));

				long lData1 = atol(pRealData_Common->m_strPrice);
				long lData2 = atol(rValue);
				long lData = abs(lData1 - lData2);
				long lDataCmp = lData1-lData2;
				double dData = (double(lData) / double(lData2)) * 100.;
				//double dData = (double(lData1) / double(lData2)) * 100.;
				//double dData = (double(lData1) / double(lData2));
				//double dData = double(lData) / double(lData2);
				
				CString szValue;
				if(lDataCmp>0)			szValue.Format(_T("%f"), dData*100);
				else					szValue.Format(_T("-%f"), dData*100);

				szValue.TrimLeft();
				
				pRealData_Common->m_strChrate = szValue;
				{
					if(bRealItem)
						pChartItemRealData->m_strChrate = szValue;
				}
			}

//			m_strPrice.Format("%ld", atol(pRealData_Common->m_strPrice));
//			{
//				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
//				{
//					//double dData;
//					//if( ((CStdDialog*)m_pStdDialog)->m_nScreenType==1)
//					//	dData = atof(m_strPrice)/100.;
//					//double dData = atof(m_strPrice)/100.;
//					//else
//					//	dData = atof(m_strPrice);
//					double dData = atof(m_strPrice);
//					m_strPrice.Format("%.2f", dData);
//				}
//				m_strPrice.TrimLeft();
//
//				if(bRealItem)
//					pChartItemRealData->m_strPrice = m_strPrice;
//			}

//			m_strOpen.Format("%ld", atol(pRealData_Common->m_strOpen));
//
//			{
//				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
//				{
//					double dData = atof(m_strOpen)/100.;
//					m_strOpen.Format("%.2f", dData);
//				}
//				m_strOpen.TrimLeft();
//
//				if(bRealItem)
//					pChartItemRealData->m_strOpen = m_strOpen;
//			}
//
//			m_strHigh.Format("%ld", atol(pRealData_Common->m_strHigh));
//
//			{
//				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
//				{
//					double dData = atof(m_strHigh)/100.;
//					m_strHigh.Format("%.2f", dData);
//				}
//
//				m_strHigh.TrimLeft();
//
//				if(bRealItem)
//					pChartItemRealData->m_strHigh = m_strHigh;
//			}
//
//			m_strLow.Format("%ld", atol(pRealData_Common->m_strLow));
//
//			{
//				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
//				{
//					double dData = atof(m_strLow)/100.;
//					m_strLow.Format("%.2f", dData);
//				}
//				m_strLow.TrimLeft();
//
//				if(bRealItem)
//					pChartItemRealData->m_strLow = m_strLow;
//			}
//
//			m_strChange.Format("%ld", atol(pRealData_Common->m_strChange));
//			{
//				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
//				{
//					double dData = atof(m_strChange)/100.;
//					m_strChange.Format("%.2f", dData);
//				}
//
//				if(m_strChange=="")
//				{
//					m_strChange = "0";
//				}
//				else if(m_strChange[0]=='-')
//				{
//					if(m_strChange[0]=='.')
//					{
//						m_strChange = '0' + m_strChange;
//					}
//				}
//				else if(m_strChange[0]=='.')
//				{
//					m_strChange = '0' + m_strChange;
//				}
//
//				if(bRealItem)
//					pChartItemRealData->m_strChange = m_strChange;
//			}

//			m_strChrate.Format("%ld", atol(pRealData_Common->m_strChrate));
//			{
//				double dRate = atof(m_strChrate)/100.;
//				m_strChrate.Format("%.2f", dRate);
//				if(m_strChrate[0]=='-')
//				{
//					if(m_strChrate[0]=='.')
//					{
//						m_strChrate = '0' + m_strChrate;
//					}
//				}
//				else if(m_strChrate[0]=='.')
//				{
//					m_strChrate = '0' + m_strChrate;
//				}
//				if(bRealItem)
//					pChartItemRealData->m_strChrate = m_strChrate;
//			}

			if(bRealItem)
			{
				strTime = pRealData_Common->m_strTime;
				{
					IRealReceiver *pIRealReceiver = ( IRealReceiver *)GetAddInCustomInterface( "ChartCommonAddIn.dll:DIRECT_REAL");

					strTime = strTime.Mid(0, 6);
					BOOL bAppend = FALSE;

//#ifdef __MULTI_LANG__
					pIRealReceiver->SetRealDataRQ(strKey, "_DateTime_",		atof(strTime), bAppend);
					if(bCurBase && bRealItem) // ���簡 ����� ��Ʈ�϶�
						pIRealReceiver->SetRealDataRQ(strKey, "_Close_",			0., bAppend);
					else
						pIRealReceiver->SetRealDataRQ(strKey, "_Close_",			atof(pChartItemRealData->m_strChrate), bAppend);

//#else
// 						pIRealReceiver->SetRealDataRQ(strKey, "�ڷ�����",		atof(strTime), bAppend);
// 
// 	//					if((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && (pSelChart->m_nChartNumericValueType == PERCENT_TYPE))
// 	//					{
// 							//pIRealReceiver->SetRealDataRQ(strKey, "����",			atof(pChartItemRealData->m_strChrate));
// 							// ���簡 ����� - ojtaso (20080602)
// 							if(bCurBase && bRealItem) // ���簡 ����� ��Ʈ�϶�
// 								pIRealReceiver->SetRealDataRQ(strKey, "����",			0., bAppend);
// 							else
// 								pIRealReceiver->SetRealDataRQ(strKey, "����",			atof(pChartItemRealData->m_strChrate), bAppend);
// 	//					}
// 	//					else
// 	//						pIRealReceiver->SetRealDataRQ(strKey, "����",			atof(pChartItemRealData->m_strPrice));
// 	#endif
					BOOL ret2 = pIRealReceiver->SetRealPacketEndRQ(strKey);

					// ���簡 ����� - ojtaso (20080602)
					if(bCurBase && bRealItem) // ���簡 ����� ��Ʈ�϶�
						pIRealReceiver->OffsetPacketData(strKey, atof(strData));

					pIRealReceiver->Release();
				}
			}
		}
	}
	//end

//	//{{ 2006.10.09 by LYH ���� Repaint ���� �ذ�
//	if(!::lstrcmp(m_strRealItemCode, m_strSelectedItemCode))
//	{
//		CRect rectTemp;
//		if(rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
//			GetParent()->InvalidateRect(rectTemp);
//	}
//	//}}
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	�ǽð� ���� ó�����θ� ó���Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::SetDrdsAdvise(long lOnAdvise)
{
	long bAdviceForChart = FALSE;
	switch(lOnAdvise)
	{
	case 1:
		m_bOnAdvise = TRUE;
		bAdviceForChart = TRUE;
		break;
	case 2:
		m_bOnAdvise = TRUE;
		bAdviceForChart = FALSE;
		break;
	case 0:
		m_bOnAdvise = FALSE;
		bAdviceForChart = FALSE;
		break;
	}

	if(!::IsWindow(this->GetSafeHwnd()))
	{
		return FALSE;
	}
	if(GetParent()->GetSafeHwnd())
	{
		// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
		BOOL bAddMode = IsAddMode();
		if(m_bOnAdvise)
		{
			//Han Hwa        m_signal.OnDrdsAdvise(GetDataCode());
			((CChartMng*)GetParent())->OnChartMngCodeAdvise(this, bAddMode);
		}
		// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
		else if(!m_bOnAdvise && bAddMode)
		{
			return TRUE;
		}
		else
		{
			//Han Hwa        m_signal.OnDrdsAdvise("");
			//Han Hwa        m_signal.OffDrdsAdvise();			
			((CChartMng*)GetParent())->OnChartMngCodeUnAdvise(this, bAddMode);
		}	
	}
	long lEndDate = atol(GetEndDate());
	CTime t = CTime::GetCurrentTime();
	long lToday = atol(t.Format("%Y%m%d"));
	if(lEndDate<lToday)
	{
		bAdviceForChart = FALSE;
	}	
	return CDRChartOcx::SetDrdsAdvise(bAdviceForChart);
}

// �������� �߰� ������� : �������� - ojtaso (20070119)
BOOL CChartItem::IsAddMode()
{
	// ��Ʈ �߰�����̸� ���� ������ ���� ���� �ʴ´�
	// �� ���� ������ Ÿ���� �ٲܶ��� ������ �����Ѵ�
	if(((CChartMng*)GetParent())->m_bChangeTypeMode)
		return FALSE;
	else if(((CChartMng*)GetParent())->GetAddGraphMode())
		return TRUE;
	else
		return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��ȸ�� ���� �ڵ带 �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
//void CChartItem::SetCodeForQuery(LPCTSTR lpCode, UINT nQueryType, BOOL bIsPrimaryData) 
void CChartItem::SetCodeForQuery(LPCTSTR lpCode, CString strQueryType, BOOL bIsPrimaryData) 
{	
#ifdef _DEBUG	
	if(!strQueryType.GetLength())
	{
		AfxMessageBox("CChartItem::SetCodeForQuery TR ��ȣ ����");
		return;
	}
#endif
// �������� ���� : �������� - ojtaso (20070503)
//	CString strOldCode = GetDataCode();
	
//	if( (strQueryType != QUERY_STRMARKET_10450) && (strQueryType != QUERY_STRMARKET_10451) && (strQueryType != QUERY_STRMARKET_10452))
	if(strQueryType != QUERY_STRMARKET)
//		SetOldChartCode(strOldCode);
		SetOldChartCode(m_strSelectedItemCode);

	if(IsAvailibleCode(lpCode))
		GetChartItemDataNew(strQueryType,lpCode,TRUE,bIsPrimaryData);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	�ش� ��Ʈ �����Ͱ� �����ϴ��� üũ�Ѵ�.
//////////////////////////////////////////////////////////////////////////////
//BOOL CChartItem::HasChartItemData(LPCTSTR lpCode, UINT nQueryType, BOOL bFindWithPrimaryData, BOOL bAutoRemoveUselessData/*=TRUE*/) 
BOOL CChartItem::HasChartItemData(LPCTSTR lpCode, CString strQueryType, BOOL bFindWithPrimaryData, BOOL bAutoRemoveUselessData/*=TRUE*/) 
{
	//CChartItemData *pChartItemData = GetChartItemDataNew(nQueryType,lpCode,FALSE);	2005.05.06 comment by sy.nam
	CChartItemData *pChartItemData = GetChartItemDataNew(strQueryType,lpCode,FALSE);	
	if(pChartItemData)
	{
		BOOL bRetValue = HasGraph(pChartItemData->GetChartItemDataTitle());	
		if(bRetValue)
		{
			return TRUE;
		}
		if(bAutoRemoveUselessData)
		{
			//RemoveChartItemData(lpCode,nQueryType); 2005.05.06 comment by sy.nam 
			RemoveChartItemData(lpCode,strQueryType);
		}
	}

#ifdef _DEBUG
		CString sTemp;
		sTemp.Format("HasChartItemData::m_arrayChartItemData ���� %d", m_arrayChartItemData.GetSize());
		OutputDebugString(sTemp);
#endif

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��ȸ�� ���� TR-Code�� �޴´� 
//////////////////////////////////////////////////////////////////////////////
CString CChartItem::GetTrCodeForQuery()
{
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(pChartItemData)
	{
		return pChartItemData->GetTrCode();
	}
	return "";
}



char CChartItem::GetType() 
{ 
	if(!m_arrayChartItemData.GetSize())
	{
		return m_chTypeForRequest;
	}

	return m_chType;// ƽ(0)/��(1)/��(2)/��(3)/��(4)
}
 

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��ȸ�� ���� Input Spec�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
// �������� �߰� : �������� - ojtaso (20070118)
CString CChartItem::GetQueryData(BOOL bChangeType/* = FALSE*/)
{
	if(	m_strDateForRequest.IsEmpty()	// �˻�������
		||m_lCntForRequest==0			// �˻��� ������ ����
		||m_chTypeForRequest==NULL		// ƽ(0)/��(1)/��(2)/��(3)/��(4)
		) 
	{
		return "";
	}
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(pChartItemData)
	{
		// ó�� ��Ʈ�� �����ڵ带 ����, �߰��Ǵ� ��Ʈ�� RQ���� �׷��� ���� : �������� - ojtaso (20061224)
		CChartMng* pMng = (CChartMng*)GetParent();
		LPCTSTR lpszItemCode = pChartItemData->GetCodeForRqstFromChartItemData();

		// ó�� Block�� ��Ʈ�߰��ϴ� ���(CHANGE_ITEM)
		// ���� Block�� �߰��ϴ� ��Ʈ�߰��� ���(NEW_ITEM)
		if((!m_bBuild || pMng->GetAddBlockMode()) && !bChangeType)
		{
			ST_CHARTITEM_REALDATA* pChartItemRealData;
			if(m_mapChartItemRealData.Lookup(lpszItemCode, (LPVOID&)pChartItemRealData))
				pChartItemRealData->m_nRefCount++;
			else
			{
				pChartItemRealData = new ST_CHARTITEM_REALDATA;

				pChartItemRealData->m_nRefCount = 1;

				CString strItemCode = lpszItemCode;
				((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strItemCode);

				m_mapChartItemRealData.SetAt(strItemCode, (LPVOID)pChartItemRealData);
			}

			AddChartItem(pChartItemData, lpszItemCode, ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(lpszItemCode, FALSE), m_chTypeForRequest, GetTickMinDayNumber(), GetCntForRequest(), GetChart_NumericValueType(), TRUE);//m_bBuild);
		}
		else if(m_bBuild)	// �ܼ� ��Ʈ ����(CHANGE_ITEM)
		{
			if(pMng->IsAbleToAdd())
			{
//				CString strPreCode = pChartItemData->GetCode();
				if(::lstrcmp(m_strSelectedItemCode, lpszItemCode))
				{
					ST_CHARTITEM_REALDATA* pChartItemRealData;

					if(m_mapChartItemRealData.Lookup(m_strSelectedItemCode, (LPVOID&)pChartItemRealData))
					{
						if(pChartItemRealData->m_nRefCount > 1)
							pChartItemRealData->m_nRefCount--;
						else
						{
							delete pChartItemRealData;
							m_mapChartItemRealData.RemoveKey(m_strSelectedItemCode);
						}
					}

					if(m_mapChartItemRealData.Lookup(lpszItemCode, (LPVOID&)pChartItemRealData))
						pChartItemRealData->m_nRefCount++;
					else
					{
						pChartItemRealData = new ST_CHARTITEM_REALDATA;

						pChartItemRealData->m_nRefCount = 1;
						CString strItemCode = lpszItemCode;
						((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strItemCode);
						m_mapChartItemRealData.SetAt(strItemCode, (LPVOID)pChartItemRealData);
					}
				}
			}

			if(m_strSelectedItemCode.IsEmpty())
				m_strSelectedItemCode = lpszItemCode;

			AddChartItem(pChartItemData, lpszItemCode, ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(lpszItemCode, FALSE), m_chTypeForRequest, GetTickMinDayNumber(), GetCntForRequest(), GetChart_NumericValueType(), FALSE);
		}

		return pChartItemData->GetQueryData(m_bNext, m_lpSelChart);
	}
	return "";
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
CString CChartItem::GetLoadedQueryData()
{
	if(	m_strDateForRequest.IsEmpty()	// �˻�������
		||m_lCntForRequest==0			// �˻��� ������ ����
		||m_chTypeForRequest==NULL		// ƽ(0)/��(1)/��(2)/��(3)/��(4)
		) 
	{
		return "";
	}

	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(pChartItemData)
	{
		if(m_bLoadDefaultRQ && !m_posLoadingRQ)
		{
			m_bLoadDefaultRQ = FALSE;
			m_strKey = _T("DEFAULT");

			m_mapChartItem.Lookup(m_strKey, (LPVOID&)m_lpSelChart);
		}
		else
		{
			if(!m_posLoadingRQ)
				m_posLoadingRQ = m_mapChartItem.GetStartPosition();

			m_mapChartItem.GetNextAssoc(m_posLoadingRQ, m_strKey, (LPVOID&)m_lpSelChart);

			if(m_posLoadingRQ && !m_strKey.Compare(_T("DEFAULT")))
			{
				m_bLoadDefaultRQ = TRUE;
				m_mapChartItem.GetNextAssoc(m_posLoadingRQ, m_strKey, (LPVOID&)m_lpSelChart);
			}
		}

		ST_CHARTITEM_REALDATA* pChartItemRealData;
		if(m_mapChartItemRealData.Lookup(m_lpSelChart->m_strItemCode, (LPVOID&)pChartItemRealData))
			pChartItemRealData->m_nRefCount++;
		else
		{
			pChartItemRealData = new ST_CHARTITEM_REALDATA;

			pChartItemRealData->m_nRefCount = 1;
			m_mapChartItemRealData.SetAt(m_lpSelChart->m_strItemCode, (LPVOID)pChartItemRealData);

			//{{2007.05.30 by LYH �ֽ�, ����, �ɼ� ������ �� TR��ȣ,�����ȣ ��߳��� ���� ����
			pChartItemData->SetTRCode(m_lpSelChart->m_strChartGubun);
			pChartItemData->SetCodeForQuery(m_lpSelChart->m_strItemCode);
			//}}
		}

		if(!m_posLoadingRQ && !m_strKey.Compare(_T("DEFAULT")))
		{
			m_bLoadRQ = FALSE;

			CDRChartOcx::SetCurrentRQ(m_strKey);
			CDRChartOcx::AddChartItem(m_lpSelChart->m_strItemCode, m_lpSelChart->m_strItemName, FALSE);
			return pChartItemData->GetQueryData(m_bNext, m_lpSelChart);
		}

		if(CDRChartOcx::AddChartItemRQ(m_strKey, m_lpSelChart->m_strItemCode, m_lpSelChart->m_strItemName))
			return pChartItemData->GetQueryData(m_bNext, m_lpSelChart);
	}  

	return "";
}


// �������� ���� : �������� - ojtaso (20080215)
void CChartItem::OnSelectChartItem(ST_SELCHART* pSelChart, BOOL bInitKey/* = TRUE*/)
{
	CChartItemData *pChartItemData = GetPrimaryChartItemData();
	if(pChartItemData)
		pChartItemData->OnSelectChartItem(pSelChart);

	m_strSelectedItemCode = pSelChart->m_strItemCode;
	if(m_strOldCode.IsEmpty())
		m_strOldCode = m_strSelectedItemCode;

	m_strKorName = pSelChart->m_strItemName;

	m_chTypeForRequest = m_chType = pSelChart->m_cType;
//	m_strTickMinDayForRequest = pSelChart->m_strTickMinDayNumber;
//
//	switch(m_chTypeForRequest)
//	{
//	case TICK_DATA_CHART:
//		SetTickNumber(m_strTickMinDayForRequest);
//		break;
//	case MIN_DATA_CHART:
//		SetMinNumber(m_strTickMinDayForRequest);
//		break;
//	case DAY_DATA_CHART:
//		SetDayNumber(m_strTickMinDayForRequest);
//		break;		
//	case HOUR_DATA_CHART:
//		SetHourNumber(m_strTickMinDayForRequest);
//	}	

	//	SetCntForRequest(pSelChart->m_lCount);
//	SetChartNumericValue(pSelChart->m_nChartNumericValueType);

	CRect rectTemp;
	if(rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
		GetParent()->InvalidateRect(rectTemp);

	m_lpSelChart = pSelChart;
}

// �������� ���� : �������� - ojtaso (20080215)
void CChartItem::OnDeleteChartItem(LPCTSTR lpszRQ, ST_SELCHART*& pSelChart)
{
	ST_CHARTITEM_REALDATA* pChartItemRealData;
	int nCount = m_mapChartItemRealData.GetCount();
	if(nCount<=0) return;
	if(m_mapChartItemRealData.Lookup(pSelChart->m_strItemCode, (LPVOID&)pChartItemRealData))
	{
		if(pChartItemRealData->m_nRefCount > 1)
			pChartItemRealData->m_nRefCount--;
		else
		{
			delete pChartItemRealData;
			m_mapChartItemRealData.RemoveKey(pSelChart->m_strItemCode);
		}
	}

	if(!m_strOldCode.CompareNoCase(pSelChart->m_strItemCode))
		m_strOldCode = m_strSelectedItemCode;

	m_mapChartItem.RemoveKey(lpszRQ);
	delete pSelChart;

	m_lpSelChart = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��ȸ�� ���� Ÿ���� �����Ѵ�.
//			 chType: ƽ(0)/��(1)/��(2)/��(3)/��(4)
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetType(char chType) 
{ 
//#ifdef _DEBUG
//	CString strTEST;
//	strTEST.Format("[gm0604]void CChartItem::SetType(%c)",chType);
//	::OutputDebugString(strTEST);
//#endif

	switch(chType)
	{
	case TICK_DATA_CHART:
	case MIN_DATA_CHART :
//	case SEC10_DATA_CHART: //05.09.28
//	case SEC30_DATA_CHART: //05.09.28
		{
			// remove market, upjong
			CChartItemData *pChartItemData = NULL;
			int nSize = m_arrayChartItemData.GetSize();
			for(int nPos=nSize-1;nPos>0;nPos--)
			{
				pChartItemData = m_arrayChartItemData.GetAt(nPos);
				//long lTrCode = atol(pChartItemData->GetTrCode());
				CString strTRCode	= pChartItemData->GetTrCode();
				CString strCode		= pChartItemData->GetCode();
				if(	(strTRCode	==	QUERY_STRMARKET)		//���� ��ǥ ����Ʈ ��ȸ
//					||(strTRCode==	QUERY_STRMARKET_NEWS)	//���� ��ǥ ���� ����Ʈ ��ȸ
					||(strTRCode==	QUERY_STRUPJONG)	)
				{
					CChartItemData *pItemData = NULL;
					if(HasChartItemData(strCode,strTRCode,FALSE))
					{
						//pItemData = new CChartItemData(this,pChartItemData->GetLTrCode());
						//��ȭ������ CString Type TRCode�� ����. 05.05. 06
						pItemData = new CChartItemData(this,pChartItemData->GetTrCode());
						pItemData->SetCodeForQuery(pChartItemData->GetCode());	
						m_arrayChartItemDataBackup.Add(pItemData);
						m_arrayChartItemData.RemoveAt(nPos);
						delete pChartItemData;
					}
				}
			}
		}
		break;
	case DAY_DATA_CHART:
		{
			// add all
			CChartItemData *pChartItemDataBackup = NULL;
			int nSize = m_arrayChartItemDataBackup.GetSize();
			for(int nPos=nSize-1;nPos>=0;nPos--)
			{
				pChartItemDataBackup = m_arrayChartItemDataBackup.GetAt(nPos);
				m_arrayChartItemData.Add(pChartItemDataBackup);
				m_arrayChartItemDataBackup.RemoveAt(nPos);
			}
		}
		break;
	case WEEK_DATA_CHART:
	case MONTH_DATA_CHART:
	case YEAR_DATA_CHART:
		{
			// remove market
			CChartItemData *pChartItemData = NULL;
			int nSize = m_arrayChartItemData.GetSize();
			for(int nPos=nSize-1;nPos>0;nPos--)
			{
				pChartItemData = m_arrayChartItemData.GetAt(nPos);
				//long lTrCode = atol(pChartItemData->GetTrCode());
				CString strTRCode = pChartItemData->GetTrCode();
				CString strCode = pChartItemData->GetCode();
//				if( (strTRCode==QUERY_STRMARKET) ||	(strTRCode==QUERY_STRMARKET_NEWS))
				if (strTRCode==QUERY_STRMARKET)
				{
					CChartItemData *pItemData = NULL;
					if(HasChartItemData(strCode,strTRCode,FALSE))
					{
						//pItemData = new CChartItemData(this,pChartItemData->GetLTrCode());
						//��ȭ������ CString Type TRCode�� ����. 05.05. 06
						pItemData = new CChartItemData(this,pChartItemData->GetTrCode());
						pItemData->SetCodeForQuery(pChartItemData->GetCode());	
						m_arrayChartItemDataBackup.Add(pItemData);
						m_arrayChartItemData.RemoveAt(nPos);
						delete pChartItemData;
					}

				}
			}
			// add upjong
			CChartItemData *pChartItemDataBackup = NULL;
			nSize = m_arrayChartItemDataBackup.GetSize();
			for(int nPos=nSize-1;nPos>=0;nPos--)
			{
				pChartItemDataBackup = m_arrayChartItemDataBackup.GetAt(nPos);
				//long lTrCode = atol(pChartItemDataBackup->GetTrCode());
				CString strTRCode = pChartItemDataBackup->GetTrCode();
				if(strTRCode==QUERY_STRUPJONG)
				{				
					m_arrayChartItemData.Add(pChartItemDataBackup);
					m_arrayChartItemDataBackup.RemoveAt(nPos);
				}
			}
		}
		break;
	}

	ClearData(); 
	//m_chTypeForRequest = chType;
	SetChartRequestType(chType);

	//2005. 05. 27 ===================================================
	//���� ���� ���α׷� ������ ���� �ӽ÷� �־��.
	//m_chType = chType;
	//================================================================

//	CString sTemp;
//	sTemp.Format("ChartItem : Recv__m_chTypeForRequest %c", m_chTypeForRequest);
//	OutputDebugString(sTemp);
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
long CChartItem::ReceiveLoadingChartData(void* szData, long nLength)
{
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(!pChartItemData)
		return 0L;

	BOOL bRetValue = pChartItemData->MakeChartData(szData,nLength, m_lpSelChart);

	pChartItemData->AddLoadedPacket(m_strKey, m_lpSelChart);
	pChartItemData->SetLoadedPacketData(m_strKey, m_lpSelChart);

	ST_CHARTITEM_REALDATA* pChartItemRealData;
	if(m_mapChartItemRealData.Lookup(m_lpSelChart->m_strItemCode, (LPVOID&)pChartItemRealData))
	{
		char szTemp[64];
		if(m_lpSelChart->m_strChartGubun == QUERY_STRJONGMOK)
		{
//@��������
//			S33100_O* pstOutput = (S33100_O*)szData;
//@��������
//@�����߰�
			KB_p0602_OutRec1* pKB0602OutRec1 = (KB_p0602_OutRec1*)szData;
//@�����߰�

			pChartItemRealData->m_chSign = pKB0602OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->price);
			pChartItemRealData->m_strPrice = szTemp;
			pChartItemRealData->m_strPrice.Remove(' ');
			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
//			MEMCPYWITHNULL(szTemp,pstOutput->open);
//			pChartItemRealData->m_strOpen = szTemp;
//			pChartItemRealData->m_strOpen.Remove(' ');
//
//			MEMCPYWITHNULL(szTemp,pstOutput->high);
//			pChartItemRealData->m_strHigh = szTemp;
//			pChartItemRealData->m_strHigh.Remove(' ');
//
//			MEMCPYWITHNULL(szTemp,pstOutput->low);
//			pChartItemRealData->m_strLow = szTemp;
//			pChartItemRealData->m_strLow.Remove(' ');
			//}}

			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->change);
			pChartItemRealData->m_strChange = szTemp;
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->chrate);
			pChartItemRealData->m_strChrate = szTemp;
			pChartItemRealData->m_strChrate.Remove(' ');
//			MEMCPYWITHNULL(szTemp,pstOutput->volume);
//			pChartItemRealData->m_strVolume = szTemp;	
//			pChartItemRealData->m_strVolume.TrimLeft();
//			if(pChartItemRealData->m_strVolume!="0")
//			{
//				pChartItemRealData->m_strVolume.TrimLeft('0');
//			}
		}
		else if(m_lpSelChart->m_strChartGubun == QUERY_STRUPJONG)//if(lTrCode==QUERY_UPJONG)
		{
//@��������	S35100_O* pstOutput = (S35100_O*)szData;	
			KB_p0603_OutRec1* pKB0603OutRec1 = (KB_p0603_OutRec1*)szData;

			pChartItemRealData->m_chSign = pKB0603OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->price);
			pChartItemRealData->m_strPrice = szTemp;
			pChartItemRealData->m_strPrice.Remove(' ');
//@��������
//			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
//			MEMCPYWITHNULL(szTemp,pstOutput->open);
//			pChartItemRealData->m_strOpen = szTemp;
//			pChartItemRealData->m_strOpen.Remove(' ');
//
//			MEMCPYWITHNULL(szTemp,pstOutput->high);
//			pChartItemRealData->m_strHigh = szTemp;
//			pChartItemRealData->m_strHigh.Remove(' ');
//
//			MEMCPYWITHNULL(szTemp,pstOutput->low);
//			pChartItemRealData->m_strLow = szTemp;
//			pChartItemRealData->m_strLow.Remove(' ');
//			//}}
//@��������

			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->change);
			pChartItemRealData->m_strChange = szTemp;
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->chrate);
			pChartItemRealData->m_strChrate = szTemp;
			pChartItemRealData->m_strChrate.Remove(' ');
//@��������
//			MEMCPYWITHNULL(szTemp,pstOutput->volume);
//			pChartItemRealData->m_strVolume = szTemp;	
//			pChartItemRealData->m_strVolume.TrimLeft();
//			if(pChartItemRealData->m_strVolume!="0")
//			{
//				pChartItemRealData->m_strVolume.TrimLeft('0');
//			}		
//@��������
		}
		else if(m_lpSelChart->m_strChartGubun == QUERY_STRFUTOPT)//if(lTrCode==QUERY_FUTURE)
		{
//@��������	S36100_O* pstOutput = (S36100_O*)szData;
			KB_p0604_OutRec1* pKB0604OutRec1 = (KB_p0604_OutRec1*)szData;

			pChartItemRealData->m_chSign = pKB0604OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->price);
			pChartItemRealData->m_strPrice = szTemp;
			pChartItemRealData->m_strPrice.Remove(' ');
//@��������
//			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
//			MEMCPYWITHNULL(szTemp,pstOutput->open);
//			pChartItemRealData->m_strOpen = szTemp;
//			pChartItemRealData->m_strOpen.Remove(' ');
//
//			MEMCPYWITHNULL(szTemp,pstOutput->high);
//			pChartItemRealData->m_strHigh = szTemp;
//			pChartItemRealData->m_strHigh.Remove(' ');
//
//			MEMCPYWITHNULL(szTemp,pstOutput->low);
//			pChartItemRealData->m_strLow = szTemp;
//			pChartItemRealData->m_strLow.Remove(' ');
//			//}}
//@��������

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->change);
			pChartItemRealData->m_strChange = szTemp;
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->chrate);
			pChartItemRealData->m_strChrate = szTemp;
			pChartItemRealData->m_strChrate.Remove(' ');
//@��������
//			MEMCPYWITHNULL(szTemp,pstOutput->volume);
//			pChartItemRealData->m_strVolume = szTemp;	
//			pChartItemRealData->m_strVolume.TrimLeft();
//			if(pChartItemRealData->m_strVolume!="0")
//			{
//				pChartItemRealData->m_strVolume.TrimLeft('0');
//			}		
//@��������
		}
		else if(m_lpSelChart->m_strChartGubun == QUERY_STRSTOCKFUTOPT)
		{
			char szTemp[64];
			KB_d1013_OutRec1* pKBd1013OutRec1 = (KB_d1013_OutRec1*)szData;

			pChartItemRealData->m_chSign = pKBd1013OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKBd1013OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp,pKBd1013OutRec1->price);
			pChartItemRealData->m_strPrice = szTemp;
			pChartItemRealData->m_strPrice.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKBd1013OutRec1->change);
			pChartItemRealData->m_strChange = szTemp;
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKBd1013OutRec1->chrate);
			pChartItemRealData->m_strChrate = szTemp;
			pChartItemRealData->m_strChrate.Remove(' ');
		}
		else if(m_lpSelChart->m_strChartGubun == QUERY_STRFOREIGN)//if(lTrCode==QUERY_FUTURE)
		{
			char szTemp[64];
			KB_p0606_OutRec1* pKB0606OutRec1 = (KB_p0606_OutRec1*)szData;

			pChartItemRealData->m_chSign = pKB0606OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->price);
			pChartItemRealData->m_strPrice = szTemp;
			pChartItemRealData->m_strPrice.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->change);
			pChartItemRealData->m_strChange = szTemp;
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->chrate);
			pChartItemRealData->m_strChrate = szTemp;
			pChartItemRealData->m_strChrate.Remove(' ');
		}
////@��������
////	else if(m_lpSelChart->m_strChartGubun == QUERY_STRELW)
////		{
////			S01007_O* pstOutput = (S01007_O*)szData;
////
////			pChartItemRealData->m_chSign = pstOutput->sign[0];
////			MEMCPYWITHNULL(szTemp,pstOutput->name);
////			pChartItemRealData->m_strItemName = szTemp;
////			pChartItemRealData->m_strItemName.TrimLeft(' ');
////			pChartItemRealData->m_strItemName.TrimRight(' ');
////			MEMCPYWITHNULL(szTemp,pstOutput->price);
////			pChartItemRealData->m_strPrice = szTemp;
////			pChartItemRealData->m_strPrice.Remove(' ');
////			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
////			MEMCPYWITHNULL(szTemp,pstOutput->open);
////			pChartItemRealData->m_strOpen = szTemp;
////			pChartItemRealData->m_strOpen.Remove(' ');
////
////			MEMCPYWITHNULL(szTemp,pstOutput->high);
////			pChartItemRealData->m_strHigh = szTemp;
////			pChartItemRealData->m_strHigh.Remove(' ');
////
////			MEMCPYWITHNULL(szTemp,pstOutput->low);
////			pChartItemRealData->m_strLow = szTemp;
////			pChartItemRealData->m_strLow.Remove(' ');
////			//}}
////
////			MEMCPYWITHNULL(szTemp,pstOutput->change);
////			pChartItemRealData->m_strChange = szTemp;
////			pChartItemRealData->m_strChange.Remove(' ');
////
////			MEMCPYWITHNULL(szTemp,pstOutput->chrate);
////			pChartItemRealData->m_strChrate = szTemp;
////			pChartItemRealData->m_strChrate.Remove(' ');
////			MEMCPYWITHNULL(szTemp,pstOutput->volume);
////			pChartItemRealData->m_strVolume = szTemp;	
////			pChartItemRealData->m_strVolume.TrimLeft();
////			if(pChartItemRealData->m_strVolume!="0")
////			{
////				pChartItemRealData->m_strVolume.TrimLeft('0');
////			}		
////		}
////@��������
	}

	CChartMng* lpChartMng = (CChartMng*)GetParent();

//@��������
//	// ���õ� RQ�� BuildChart���� Real�����
//	if(m_strKey.Compare(m_strSelectedRQ))
//		lpChartMng->AddRealCodeList(m_lpSelChart->m_strItemCode);
//@��������

	lpChartMng->RequestChartData();

	return 0L;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ��������ȸ������� �޴´�.
//////////////////////////////////////////////////////////////////////////////
long CChartItem::ReceiveInitalChartData(void* szData, long nLength)
{
	CString strJMCode;
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(!pChartItemData)
	{
#ifdef _DEBUG
		CString sTemp;
		sTemp.Format("%d", pChartItemData);
		AfxMessageBox("CChartItem::ReceiveInitalChartData \n pChartItemData pointer is NULL\n" + sTemp);
		
#endif
		return 0;
	}

	// �������� �ҷ����� : �������� - ojtaso (20070514)
	CChartMng* lpChartMng = (CChartMng*)GetParent();
//@��������
//	if(m_lpSelChart)
//	{
//		// ���õ� RQ�� BuildChart���� Real�����
//		if(m_strKey.Compare(m_strSelectedRQ))
//			lpChartMng->AddRealCodeList(m_lpSelChart->m_strItemCode);
//	}
//@��������

	//ChangePriceGraphDrawingData(!m_bRateValue,!m_bRateValue,!m_bRateValue,FALSE,FALSE,1);
/*	if(m_nChartNumericValueType == WON_TYPE)
		ChangePriceGraphDrawingData(TRUE,TRUE,TRUE,FALSE,FALSE,1);	
	else
		ChangePriceGraphDrawingData(FALSE,FALSE,FALSE,FALSE,FALSE,1);
*/	
	//2005. 09. 13 pm 06:26 �����븮 ��û���� �� ���� ChangePriceGraphDrawingData(TRUE,TRUE,TRUE,TRUE,FALSE,1);	

	BOOL bRetValue = pChartItemData->MakeChartData(szData,nLength, m_lpSelChart);
	if(!bRetValue)
	{
		return 0;
	}
	m_strOpen = "";
	m_strHigh = "";
	m_strLow = "";

	CString strTrCode = _T(""); //ȣ�� ��û ������ ������ ����. 2005. 07. 15
	// �������� ���� : �������� - ojtaso (20070328)
	CString strCode = m_lpSelChart ? m_lpSelChart->m_strItemCode : pChartItemData->GetCode();

	if(IsPrimaryChartItemData(pChartItemData)&&!m_bNext)	// Update Title Info..
	{	
		m_bReceivedDataChart = TRUE;
	
		//long lTrCode = pChartItemData->GetLTrCode();
		//if(lTrCode==QUERY_JONGMOK)
		//��ȯ������ CString Type TRCode�� ����. 05.05. 06
		strTrCode = pChartItemData->GetTrCode();
		if(strTrCode == QUERY_STRJONGMOK)
		{			
			char szTemp[64];
//			S33100_O* pstOutput = (S33100_O*)szData;
			KB_p0602_OutRec1_EX* pKB0602OutRec1 = (KB_p0602_OutRec1_EX*)szData;
		

 			m_chType = m_chTypeForRequest;			
#ifdef _DEBUG
//			OutputDebugString("################### 1");
//			OutputDebugString((char*)m_chType);
//			OutputDebugString("###################");
#endif
			SetEndDate(m_strDateForRequest); //05.09.06 m_strEndDate = m_strDateForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case MIN_DATA_CHART:
					SetMinNumber(m_strTickMinDay);
					break;
				case DAY_DATA_CHART:
					SetDayNumber(m_strTickMinDay);
					break;
				case HOUR_DATA_CHART:
					SetHourNumber(m_strTickMinDay);
				default:
					m_strTickMinDay = "001";
					break;
			}

			m_chSign = pKB0602OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->price);
			m_strPrice.Format("%ld", atol(szTemp));
			m_strPrice.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->change);
			m_strChange.Format("%ld", atol(szTemp));
			m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->chrate);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			m_strChrate.Remove(' ');

//			CWnd *pWnd = lpChartMng->GetpLeftBarDlg();
//			if(pWnd)
//			{	
//				BOOL bChecked = FALSE;
//				pWnd->SendMessage(UMSG_GETJCODECHECKEDFROMLEFTBAR,(WPARAM)&bChecked,(LPARAM)(LPCTSTR)strCode);
//				if(m_Check.GetSafeHwnd())
//					m_Check.SetCheck(bChecked);
//			}		
		}
		else if(strTrCode == QUERY_STRUPJONG)//if(lTrCode==QUERY_UPJONG)
		{
			char szTemp[64];
//			S35100_O* pstOutput = (S35100_O*)szData;	
			KB_p0603_OutRec1_EX* pKB0603OutRec1 = (KB_p0603_OutRec1_EX*)szData;
#ifdef _DEBUG
//			AfxMessageBox((char*)pstOutput);
#endif		
			m_chType = m_chTypeForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			SetEndDate(m_strDateForRequest); //05.09.06 m_strDate = m_strDateForRequest;

			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case MIN_DATA_CHART:
					SetMinNumber(m_strTickMinDay);
					break;
				case DAY_DATA_CHART:
					SetDayNumber(m_strTickMinDay);
					break;
				case HOUR_DATA_CHART:
					SetHourNumber(m_strTickMinDay);
					break;
				default:
					m_strTickMinDay = "001";
					break;
			}

			m_chSign = pKB0603OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			m_strPrice.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->change);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			m_strChrate.Remove(' ');
		}
		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
		else if(strTrCode == QUERY_STRFUTOPT/* || strTrCode == QUERY_STRCOMMODITY*/)//if(lTrCode==QUERY_FUTURE)
		{
			char szTemp[64];
//			S36100_O* pstOutput = (S36100_O*)szData;
			KB_p0604_OutRec1_EX* pKB0604OutRec1 = (KB_p0604_OutRec1_EX*)szData;
			m_chType = m_chTypeForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			SetEndDate(m_strDateForRequest); //05.09.06 m_strDate = m_strDateForRequest;

			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case MIN_DATA_CHART:
					SetMinNumber(m_strTickMinDay);
					break;
				case DAY_DATA_CHART:
					SetDayNumber(m_strTickMinDay);
					break;
				case HOUR_DATA_CHART:
					SetHourNumber(m_strTickMinDay);
				default:
					m_strTickMinDay = "001";
					break;
			}

			m_chSign = pKB0604OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			m_strPrice.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->change);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			m_strChrate.Remove(' ');

		}
		else if(strTrCode == QUERY_STRSTOCKFUTOPT/* || strTrCode == QUERY_STRCOMMODITY*/)//if(lTrCode==QUERY_FUTURE)
		{
			char szTemp[64];
			KB_d1013_OutRec1_EX* pKB1013OutRec1 = (KB_d1013_OutRec1_EX*)szData;
			m_chType = m_chTypeForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			SetEndDate(m_strDateForRequest); //05.09.06 m_strDate = m_strDateForRequest;

			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case MIN_DATA_CHART:
					SetMinNumber(m_strTickMinDay);
					break;
				case DAY_DATA_CHART:
					SetDayNumber(m_strTickMinDay);
					break;
				case HOUR_DATA_CHART:
					SetHourNumber(m_strTickMinDay);
				default:
					m_strTickMinDay = "001";
					break;
			}

			m_chSign = pKB1013OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			m_strPrice.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->change);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			m_strChrate.Remove(' ');
		}
		else if(strTrCode == QUERY_STRFOREIGN)//if(lTrCode==QUERY_FUTURE)
		{			
			char szTemp[64];
			KB_p0606_OutRec1_EX* pKB0606OutRec1 = (KB_p0606_OutRec1_EX*)szData;
			m_chType = m_chTypeForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			SetEndDate(m_strDateForRequest); //05.09.06 m_strDate = m_strDateForRequest;

			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case MIN_DATA_CHART:
					SetMinNumber(m_strTickMinDay);
					break;
				case DAY_DATA_CHART:
					SetDayNumber(m_strTickMinDay);
					break;
				case HOUR_DATA_CHART:
					SetHourNumber(m_strTickMinDay);
				default:
					m_strTickMinDay = "001";
					break;
			}

			m_chSign = pKB0606OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			m_strPrice.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->change);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			m_strChrate.Remove(' ');
		}
		else
			strTrCode = _T("");

		
		ST_CHARTITEM_REALDATA* pChartItemRealData;
		if(m_mapChartItemRealData.Lookup(strCode, (LPVOID&)pChartItemRealData))
		{
			pChartItemRealData->m_strItemName = m_strKorName;
			pChartItemRealData->m_chSign = m_chSign;
			pChartItemRealData->m_strPrice = m_strPrice;
			pChartItemRealData->m_strChange = m_strChange;
			pChartItemRealData->m_strChrate = m_strChrate;
			pChartItemRealData->m_strVolume = m_strVolume;
			pChartItemRealData->m_strOpen = m_strOpen;
			pChartItemRealData->m_strHigh = m_strHigh;
			pChartItemRealData->m_strLow = m_strLow;
		}

		GetParent()->InvalidateRect(lpChartMng->GetRectOfChart(m_lRowIndex,m_lColIndex,1));
		SendMessageToInvoke(INVOKEMSG_CHANGECODE, (LONG)(LPCTSTR)GetDataCode(),GetDataCode().GetLength());
	}
	else
	{
		strTrCode = pChartItemData->GetTrCode();
		if(strTrCode == QUERY_STRJONGMOK)
		{
			S33100_O* pstOutput = (S33100_O*)szData;
			CString strNextKey;
			strNextKey.Format("%*.*s", sizeof(pstOutput->nkey), sizeof(pstOutput->nkey), pstOutput->nkey);
			strNextKey.TrimLeft();
			if(strNextKey.GetLength() <1)
			{
				CString szMessage = "   �̹� ��� �����͸� ��ȸ�Ͽ����ϴ�.   ";
				lpChartMng->SetEndData(szMessage);
			}
		}
	}

	if(bRetValue)
	{
		if(!BuildChart()) return 0;
		if(FindChartItemDataForQuery())
		{
			return 2; // �ش�ChartItem���� �߰����� Request�� �ʿ��Ҷ�..
		}
		else
		{
			//2005. 07. 15 == �ֽ� ���񿡼��� Hoga�� ��û�Ѵ�.		
			if(strTrCode == QUERY_STRJONGMOK)
			{
				// �������� ���� : �������� - ojtaso (20070328)
//				CString strCode = pChartItemData->GetCode();
//KHD : ȣ���� ����Ʈ���� ��û�ؾ� �ϳ�?
				//GetParent()->GetParent()->SendMessage(UMSG_SENDTR_HOGA_DATA, (WPARAM)&strCode, 0);
			}

			// �������� �ҷ����� : �������� - ojtaso (20080521)
			if(m_mapChartItem.GetCount() > 1)
				CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);

			return 1; // �ش�ChartItem���� �߰����� Request �Ϸ�...
		}
	}

	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	���� �����ͷ� ��Ʈ�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::BuildChart()
{
	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108)
	BOOL bAdd = m_bBuild;

	m_bBuild = TRUE;
	long lSize = 0;
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	BOOL bIsPrimaryChartItemData = IsPrimaryChartItemData(pChartItemData);
	
	// 2005. 05. 18 add by sy, nam ===========================================
	// ��ø�� ��� ��� ������ ��ø�Ǵ��� �� �� ���� �����Ѵ�.
	if(m_bOverLapping == FALSE)
		m_nCntOverLap = 0;

	pChartItemData->m_nCntOverLap = m_nCntOverLap;
	// �������� - ojtaso (20070108)
	CChartMng* pChartMng = (CChartMng*)GetParent();
	//========================================================================
	if(m_nSmallChartMode==1)
	{
		pChartItemData->AddPacketAndGraph(TRUE);
	}
	else if(bAdd && !pChartMng->m_bChangeTypeMode) 	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	{
		if(((CChartMng*)GetParent())->GetMode() != OVERLAPP_ITEM)
			pChartItemData->AddPacketAndGraph2(pChartMng->GetAddGraphMode());
		else 
		{
//-->@.080526-001
//			pChartItemData->AddPacketAndGraph2(pChartMng->GetAddGraphMode(), TRUE);
			if(((CChartMng*)GetParent())->m_pShareInputData->m_DataInputBar.m_cChartShape=='B')
				pChartItemData->AddPacketAndGraph2(pChartMng->GetAddGraphMode(), TRUE, TRUE);
			else
				pChartItemData->AddPacketAndGraph2(pChartMng->GetAddGraphMode(), TRUE, FALSE);
//<--
		}
	}
	else
	{
		pChartItemData->AddPacketAndGraph();
		m_strOldCode = pChartItemData->GetCodeForRqstFromChartItemData();
	}

	// ���õ� ����� ���� : �������� - ojtaso (20070108)
	if(!m_lpSelChart)
		m_strSelectedItemCode = pChartItemData->GetCodeForRqstFromChartItemData();

	m_bInitalChart = TRUE;
	if(bIsPrimaryChartItemData)
	{
		SetDrdsAdvise(0);		
	}

	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	pChartMng->m_bChangeTypeMode = FALSE;

	// 2005. 08. 17 ���� ó�� ȯ���� �ٲ�鼭 
	// �������� �߰� ������� : �������� - ojtaso (20070119)
	if(!IsAddMode())
		pChartItemData->RemoveRealItem();

	ShowInnerToolBar(TRUE);

	if( !HasGraph("������Ʈ"))
	{
		AddGraph(0,0,0,"������Ʈ"		,FALSE	,FALSE	,FALSE	); 
	}

	BOOL bNextOld = m_bNext;	
	BOOL bRetValueOfSetPacketData = pChartItemData->SetPacketData(!m_bNext);

	if(m_nChartNumericValueType == WON_TYPE)
	{
		SetBShowPriceRateInquiry(TRUE);
	}
	else
	{
		SetBShowPriceRateInquiry(FALSE);
	}
	
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	if(!bRetValueOfSetPacketData) 
	{
		if(m_lpSelChart)
			m_lpSelChart = NULL;

		return FALSE;
	}

	if(bIsPrimaryChartItemData)
	{
		lSize = pChartItemData->GetCnt();
		if(lSize)
		{
			if(bNextOld)
			{
				m_lAllCnt += lSize;
			}
			else
			{
				m_lAllCnt = lSize;
			}
		}
	}

	CString strDateToday;
	CTime t = CTime::GetCurrentTime();
	strDateToday = t.Format("%Y%m%d");

	if(strDateToday==m_strEndDate)
	{
		if(m_lPtrST)
		{
			pChartItemData->RemoveRealItem();
			SetDrdsAdvise(2);
		}
		else
		{			
			pChartItemData->AddRealItem();
			
			if(bIsPrimaryChartItemData)
			{
				SetDrdsAdvise(1);
			}
		}
	}
	else
	{
		if(bIsPrimaryChartItemData)
		{
			SetDrdsAdvise(2);
		}
	}

	CString strSharedTime = ((CChartMng*)GetParent())->GetSharedTime();
	if(!strSharedTime.IsEmpty())
	{
		SetTimeMark(strSharedTime,TRUE);
	}
	Invalidate();	
	if(m_lPtrST)
	{
		InvokeAddIn("ChartStrategyAddin.dll",1,m_lPtrST);
	}

	// �������� �ҷ����� : �������� - ojtaso (20070514)
	if(m_lpSelChart)
	{
		CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);

		if(GetSelChart(m_strSelectedRQ, m_lpSelChart))
		{
			CChartItemData *pItemData = m_arrayChartItemData.GetAt(0);
			int nSize = m_arrayChartItemData.GetSize();

			BOOL bFiil = pItemData->IsFilled();

			OnSelectChartItem(m_lpSelChart, FALSE);

			pItemData->m_bIsFilled = bFiil;
		}
		
//-->@080611-001 ������϶��� ó��
		int nType = 0;
		fnChartFluct(0);
//<--

		m_lpSelChart = NULL;
	}


	return TRUE;
}

BOOL CChartItem::ResetChart()
{
	BOOL bRetValue = CDRChartOcx::ResetChart();	

	// ��� �������� ����
	POSITION pos = m_mapChartItemRealData.GetStartPosition();
	ST_CHARTITEM_REALDATA* lpChartItemRealData;
	CString strKey;
	while(pos)
	{
		m_mapChartItemRealData.GetNextAssoc(pos, strKey, (LPVOID&)lpChartItemRealData);
		if(lpChartItemRealData)
			delete lpChartItemRealData;
	}

	m_mapChartItemRealData.RemoveAll();
	m_strSelectedItemCode.Empty();

	return bRetValue;
}

void CChartItem::ResetRQ()
{
	m_mapChartItemRealData.RemoveAll();
	m_strSelectedItemCode.Empty();
}

//////////////////////////////////////////////////////////////
///////////////////// func For Save & Load ChartInfo


BOOL CChartItem::LoadChartInfo(LPCTSTR lpTitle, BOOL bSetChartTick /* TRUE */, BOOL bNotCodeInputInit /* TRUE */)
{	
	CString strAppName;
	CString strTemp;
	CString strTemp2;
	char szTemp[128];
	char szTemp2[128];


	//��Ʈ �ε�� ���� ����� ��Ʈ�� �ϳ��� ���� 
	//��츦 üũ�ϱ� ���� 
	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);
	::GetPrivateProfileString(strAppName,"CntOfGraph","",szTemp,sizeof(szTemp),lpTitle);
	long lCntOfGraph = atol(szTemp);

	if(lCntOfGraph == 0)  //����� ��Ʈ ������ ���� ���� ��� 
		bNotCodeInputInit = FALSE; //������ �������� �����ڵ带 �״�� CodeInput�� �α����� FALSE�� ���Ѵ�.

	if(bNotCodeInputInit == FALSE)
		ResetOCX(FALSE);//�����ڵ� �κ��� ������ �ʴ´�.
	else
	{
		ResetChart();
		ResetOCX();		//�����ڵ� �κ��� �����.
	}

	//�����ְ� ����Ʈ TRUE�� ���� 2006.03.16 by LYH
	m_CfgInfo.bRevisedValue = TRUE;

	if(lCntOfGraph == 0) return FALSE;

	
	
//	::GetPrivateProfileString(strAppName,"CntOfGraph","",szTemp,sizeof(szTemp),lpTitle);
//	long lCntOfGraph = atol(szTemp);
	CChartItemData *pData = NULL;

	int nMarketType = -1;
	for(long lPos=0;lPos<lCntOfGraph;lPos++)
	{
		strTemp.Format("Code%02d",lPos);
		strTemp2.Format("TRCode%02d",lPos);
		::GetPrivateProfileString(strAppName,strTemp,"",szTemp,sizeof(szTemp),lpTitle);
		::GetPrivateProfileString(strAppName,strTemp2,"",szTemp2,sizeof(szTemp2),lpTitle);
	
		
		//GetChartItemDataNew(atol(szTemp2),szTemp);
		//==========================================================================
		// chart code share
		// 05.09.14
		if(lPos == 0)
		{
			//{{2007.01.25 by LYH �������� �ҷ������ ���� �ڵ� �ݿ�
			CString strPreCode = ((CChartMng*)GetParent())->m_strCodeForLoadChart;
			if(strPreCode.GetLength()>0 && ((CChartMng*)GetParent())->GetpInputBarDlg())
			{
				BOOL bLoadSaveCode = ((CChartMng*)GetParent())->GetpInputBarDlg()->SendMessage(UMSG_GETLOADSAVECODE);
				if(bLoadSaveCode)
				{
					CString strSaveCode, strCodeName, strLastCode;
					strSaveCode.Format("%s", szTemp);
					strSaveCode.TrimLeft();
					int nRealMarketType;
					nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(strSaveCode, nRealMarketType);
					IMasterDataManagerLast* pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
					if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
					{
						//strCodeName	= pDataManager->GetCodeInfo("FMASTER", 0, strSaveCode);
						strCodeName	= pDataManager->GetCodeName(strSaveCode);	
						//if(strCodeName.GetLength()<1)
						//	strCodeName		= pDataManager->GetCodeInfo("PMASTER", 0, strSaveCode);

						if(strCodeName.GetLength()<=0 || strCodeName == strSaveCode)//����� ������ ����
						{
//@��������
//							strLastCode = GetLastKey(8);	//����
//							if(strLastCode.GetLength()<1)
//							{
//								strLastCode = GetLastKey(12);	//�ɼ�
//							}
//							if(strLastCode.GetLength()>0)
//								strPreCode = strLastCode; 
//@��������
//@�����߰�
							strLastCode = GetLastHistoryCode(CMasterTypeInfo::FUTOPT_CHART);
							if(strLastCode.GetLength()>0)
								strPreCode = strLastCode; 
//@�����߰�
						}
						else
						{
							strPreCode = "";
						}
					}
//@��������
//					else if(nMarketType == ELW_CHART)
//					{
////@��������
////						strCodeName		= pDataManager->GetCodeInfo("WMASTER", 0, strSaveCode);
////						if(strCodeName.GetLength()<=0 || strCodeName == strSaveCode)//����� ������ ����
////						{
////							strLastCode = GetLastKey(3);
////							if(strLastCode.GetLength()>0)
////								strPreCode = strLastCode; 
////						}
////						else
////						{
////							strPreCode = "";
////						}
////@��������
////@�����߰�
//						strCodeName		= pDataManager->GetCodeInfo("WMASTER", 0, strSaveCode);
//						if(strCodeName.GetLength()<=0 || strCodeName == strSaveCode)//����� ������ ����
//						{
//							strLastCode = GetLastHistoryCode(CMasterTypeInfo::FUTOPT_CHART);
//							if(strLastCode.GetLength()>0)
//								strPreCode = strLastCode; 
//						}
//						else
//						{
//							strPreCode = "";
//						}
////@�����߰�
//					}
//@��������
//@��������
//					// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//					else if(nMarketType == COMMODITY_CHART)
//					{
//						strCodeName		= pDataManager->GetCodeInfo("", 1, strSaveCode);
//						if(strCodeName.GetLength()<=0 || strCodeName == strSaveCode)//����� ������ ����
//						{
//							strLastCode = GetLastKey(14);
//							if(strLastCode.GetLength()>0)
//								strPreCode = strLastCode; 
//						}
//						else
//						{
//							strPreCode = "";
//						}
//					}
//					else
//						strPreCode = "";
//@��������
				}
			}
			if(strPreCode.GetLength())
			{
				memset(szTemp, 0x00, sizeof(szTemp));
				memcpy(szTemp, (LPTSTR)(LPCTSTR)strPreCode, strPreCode.GetLength());

				int nRealMarketType;
				nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(strPreCode, nRealMarketType);
				switch(nMarketType) {
				case CMasterTypeInfo::STOCK_CHART:
					memset(szTemp2, 0x00, sizeof(szTemp2));
					memcpy(szTemp2, (LPTSTR)(LPCTSTR)QUERY_STRJONGMOK, sizeof(szTemp2));
					break;
				case CMasterTypeInfo::UPJONG_CHART:
					memset(szTemp2, 0x00, sizeof(szTemp2));
					memcpy(szTemp2, (LPTSTR)(LPCTSTR)QUERY_STRUPJONG, sizeof(szTemp2));
					break;
				case CMasterTypeInfo::FUTOPT_CHART:
					memset(szTemp2, 0x00, sizeof(szTemp2));
					memcpy(szTemp2, (LPTSTR)(LPCTSTR)QUERY_STRFUTOPT, sizeof(szTemp2));
					break;
				case CMasterTypeInfo::STOCKFUTOPT_CHART:
					memset(szTemp2, 0x00, sizeof(szTemp2));
					memcpy(szTemp2, (LPTSTR)(LPCTSTR)QUERY_STRSTOCKFUTOPT, sizeof(szTemp2));
					break;
				}
			}
			else
				//}}
			{
				CString strQuery;
				strQuery.Format("%s", szTemp2);
				//if(strQuery != QUERY_STRUPJONG)// && strQuery != QUERY_STRFUTOPT)
				CString strShareCode = ((CChartMng*)GetParent())->m_strStatic_ShareCode;
				if((strQuery == QUERY_STRJONGMOK && strShareCode.GetLength() == CODE_STOCK_LENGTH) || 
					(strQuery == QUERY_STRFUTOPT && (strShareCode.GetLength() == FUTOPT_LENGTH)))// ||
//					(strQuery == QUERY_STRCOMMODITY && (strShareCode.GetLength() == COMMODITY_LENGTH)))	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
				{
					if(((CChartMng*)GetParent())->m_nStatic_RecvCntShareCode > 0)
					{
						if(strShareCode.GetLength() && IsAvailibleCode(strShareCode))
						{
							memset(szTemp, 0x00, sizeof(szTemp));
							memcpy(szTemp, (LPTSTR)(LPCTSTR)strShareCode, strShareCode.GetLength());
						}
					}
				}
				//{{ 2007.08.23 by LYH Multi��Ʈ���� �����ɼ� �Ǵ� ELW ����� ����� ���� �ε�� ������ �ڵ�� �ε�
				else
				{
					CString strSaveCode, strCodeName, strLastCode;
					strSaveCode.Format("%s", szTemp);
					strSaveCode.TrimLeft();
					int nRealMarketType;
					nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(strSaveCode, nRealMarketType);
					IMasterDataManagerLast* pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
					if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
					{
						//strCodeName		= pDataManager->GetCodeInfo("FMASTER", 0, strSaveCode);	
						strCodeName		= pDataManager->GetCodeName(strSaveCode);	
						//if(strCodeName.GetLength()<1)
						//	strCodeName		= pDataManager->GetCodeInfo("PMASTER", 0, strSaveCode);

						if(strCodeName.GetLength()<=0 || strCodeName == strSaveCode)//����� ������ ����
						{
							strLastCode = GetLastHistoryCode(CMasterTypeInfo::FUTOPT_CHART);
							if(strLastCode.GetLength()>0)
							{
								memset(szTemp, 0x00, sizeof(szTemp));
								memcpy(szTemp, (LPTSTR)(LPCTSTR)strLastCode, strLastCode.GetLength());
							}
							else
								memset(szTemp, 0x00, sizeof(szTemp));
						}
					}
				}
			}
		}

		CString strCode;
		strCode.Format("%s", szTemp);
		strCode.TrimLeft();
		if(strCode.GetLength()<1)
		{
			strCode = ((CChartMng*)GetParent())->m_strCodeForMulti;
			if(strCode.GetLength())
			{
				memset(szTemp, 0x00, sizeof(szTemp));
				memcpy(szTemp, (LPTSTR)(LPCTSTR)strCode, strCode.GetLength());
			}
		}
		//==========================================================================
			
		GetChartItemDataNew(szTemp2/*TRCode*/,szTemp/*����Code*/);
		if( CString(szTemp).GetLength() == 0) 
		{
			ResetOCX();
			return FALSE;
		}
		
		
		
//		if(lPos==0)// ���񸮽�Ʈ���߰��ϱ� ����....
//		{
//			((CChartMng*)GetParent())->GetpInputBarDlg()->SendMessage(UMSG_INPUTINSHAREDCODELIST,(WPARAM)szTemp);
//		}
	}


	if( ((CStdDialog*)GetParent()->GetParent())->m_nChartType==MINI_MULTI_CHART ||
		((CStdDialog*)GetParent()->GetParent())->m_nChartType==MINI_SINGLE_CHART )
	{
		::GetPrivateProfileString(strAppName,"AllCnt","90",szTemp,sizeof(szTemp),lpTitle);
		//==============================================================================
		// ��ȭ ����ȣ ����� ��û
		// ����� ��ü ���� �����ϰ�	
		// ������ 400�� ��ȸ�� ������.
		// 2005.10.14
		//=============================================================================	
		m_lCntForRequest = atol(szTemp);
		if( (m_lCntForRequest<=0) || (m_lCntForRequest>CHTMXREQUEST) )	
			m_lCntForRequest = 90;
		//=============================================================================		
	}
	else
	{
		::GetPrivateProfileString(strAppName,"AllCnt", DEFAULT_CHARTDATA_COUNT_CHAR,szTemp,sizeof(szTemp),lpTitle);
		//==============================================================================
		// ��ȭ ����ȣ ����� ��û
		// ����� ��ü ���� �����ϰ�	
		// ������ 400�� ��ȸ�� ������.
		// 2005.10.14
		//=============================================================================	
		m_lCntForRequest = atol(szTemp);
		if( (m_lCntForRequest<=0) || (m_lCntForRequest>CHTMXREQUEST) )	
			m_lCntForRequest = DEFAULTCNTATONCE;
		//=============================================================================		
	}

	m_nSaveAllCount = m_lCntForRequest;

	::GetPrivateProfileString(strAppName,"CntInView","120",szTemp,sizeof(szTemp),lpTitle);
	long lTemp = atol(szTemp);
	if(lTemp<=0||lTemp>m_lCntForRequest)
	{
		SetNew_OnePageDataCount(120);
	}
	else
	{
		SetNew_OnePageDataCount(lTemp);
	}



	//=====================================================================================
	// ��Ʈ�� ƽ���� �� �ش� Value�� �о m_strTickMinDayForRequest ����� �����Ѵ�.
	//=====================================================================================
	//::GetPrivateProfileString(strAppName,"TickMinNum","01",szTemp,sizeof(szTemp),lpTitle);		
	// 2005. 09. 12 edited by nam	- ƽ�п� ��� �����Ѵ�.
	//=================================================================================
	
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString(strAppName,"TickValue",	"001",	szTemp,sizeof(szTemp),lpTitle);	
	SetTickNumber(CString(szTemp));

	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString(strAppName,"MinValue",	"001",	szTemp,sizeof(szTemp),lpTitle);
	SetMinNumber(CString(szTemp));
	
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString(strAppName,"DayValue",	"001",	szTemp,sizeof(szTemp),lpTitle);
	SetDayNumber(CString (szTemp));

	if(bSetChartTick)
	{
		::GetPrivateProfileString(strAppName,"Type","2",szTemp,sizeof(szTemp),lpTitle);
		//=====================================================================================
		//2005. 05. 23 �� ��Ʈ�� ������ �����Ѵ�. by sy, nam
		//if(szTemp[0]<'0' || szTemp[0]>'4') 
		//=====================================================================================
		//if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>YEAR_DATA_CHART) 
//		if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>SEC30_DATA_CHART) 
		if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>SEC_DATA_CHART) 
		{
			szTemp[0] = DAY_DATA_CHART;
		}
		//m_chTypeForRequest = szTemp[0];
		//{{2007.01.25 by LYH 
		BOOL bSetType = TRUE;
		if(nMarketType == CMasterTypeInfo::FUTOPT_CHART && (szTemp[0] == WEEK_DATA_CHART || szTemp[0] == WEEK_DATA_CHART))
			bSetType = FALSE;
		if((szTemp[0] == HOUR_DATA_CHART || szTemp[0] == MIN_DATA_CHART))
			bSetType = FALSE;
//@��������
//		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//		if(nMarketType == COMMODITY_CHART && (szTemp[0] == HOUR_DATA_CHART || szTemp[0] == MIN_DATA_CHART))
//			bSetType = FALSE;
//		//}}
//@��������

		if(bSetType)
			SetChartRequestType(szTemp[0]);
		//=====================================================================================
	}

	//080520-002 ��ȸ���� ����
	m_chTypeForRequest = szTemp[0];
	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART: m_strTickMinDayForRequest = GetTickNumber(); break;
		case MIN_DATA_CHART : m_strTickMinDayForRequest = GetMinNumber() ; break;			
		case DAY_DATA_CHART : m_strTickMinDayForRequest = GetDayNumber() ; break;
		case HOUR_DATA_CHART: m_strTickMinDayForRequest = GetHourNumber(); break;
		default				: m_strTickMinDayForRequest = "001"			 ; break;  
	}	

	if(m_strTickMinDayForRequest.IsEmpty())
	{
		m_strTickMinDayForRequest = "001";
	}
	//=====================================================================================

	//05.06.22 by sy, nam
	// ��, %, �޷��� 3���� ������ �ʿ��Ѵ�. 
	//::GetPrivateProfileString(strAppName,"UseRateValue","0",szTemp,sizeof(szTemp),lpTitle);
	::GetPrivateProfileString(strAppName,"ChartUnitValue","0",szTemp,sizeof(szTemp),lpTitle);	
	//SetUseRateValue(atol(szTemp));	
	SetChartNumericValue(atoi(szTemp));

	//=================================================================================
	// 2005. 08. 27 add by nam
	// ������ ��Ʈ�� ������ ���� 
	//=================================================================================
	// bRealCandle			// �Ǻ� or �Ǻ� + ���
	// bShowHLBar			// �����ѹ� ǥ��
	// bShowRight			// �ϸ��� / ���� ǥ��
	// bRecvTimeShare		// �ð����� ����
	// bAutoSave			// ��Ʈ ����� �ڵ����� ����
	// bShowLog				// ��Ʈ �α�ǥ�� ����
	// bRevisedValue		// �����ְ� ǥ�� ����
	// bStartDate			// ��Ʈ�� ���õǰ� �ִ� ��¥�� ��� ���ٿ��� ��ȸ �������� �����ߴ��� ����.	


	//�Ǻ� ���
	::GetPrivateProfileString(strAppName,	"bRealCandle"		,"1",szTemp,sizeof(szTemp),lpTitle);	
	//SetNew_BNormalizedMinuteTimeRule( atoi(szTemp) > 0 ? FALSE : TRUE) ; //atoi(szTemp)�� 0���� ũ�ٴ� ���� �Ǻ��� �ǹ��ϹǷ� 
																		 //SetNew_BNormalizedMinuteTimeRule	�Լ��� ���ڴ� FALSE�� �ѱ��.

	m_CfgInfo.bRealCandle =  atoi(szTemp) > 0 ? TRUE : FALSE; 
	SetBNormalizedMinuteTimeRule( atoi(szTemp) > 0 ? FALSE : TRUE);//atoi(szTemp)�� 1 �̸� �Ǻ�,  0�̸� �Ǻ�
	

	//�����ѹ�
	::GetPrivateProfileString(strAppName,	"bShowHLBar"		,"1",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bShowHLBar = atoi(szTemp) ? true : false;
	EnableBoundMark(m_CfgInfo.bShowHLBar);	

	//05. 08. 31 ======== �� ������ OCX�� �����Ѵ�. ===============================================
	//�ϸ��� 
	//::GetPrivateProfileString(strAppName,	"bShowRight"		,"1",szTemp,sizeof(szTemp),lpTitle);	
	//m_CfgInfo.bShowRight = atoi(szTemp) ? true : false;
	//ShowLock(m_CfgInfo.bShowRight);
	//============================================================================================

	//�ð�����
	::GetPrivateProfileString(strAppName,	"bRecvTimeShare"	,"1",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bRecvTimeShare = atoi(szTemp) ? true : false;	
	EnableReceivingTimeShare(m_CfgInfo.bRecvTimeShare);

	//�ڵ�����
	::GetPrivateProfileString(strAppName,	"bAutoSave"			,"0",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bAutoSave = atoi(szTemp) ? true : false;

	//�α� ���̱� ����.
	::GetPrivateProfileString(strAppName,	"bShowLog"			,"0",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bShowLog = atoi(szTemp) ? true : false;
	GetParent()->SendMessage( UMSG_SETLOG, m_CfgInfo.bShowLog, 0);

	//�����ְ�.
	::GetPrivateProfileString(strAppName,	"bRevisedValue"		,"0",szTemp,sizeof(szTemp),lpTitle);
	SetUseRevisedValue(atoi(szTemp) ? true : false);
	//m_CfgInfo.bRevisedValue = atoi(szTemp) ? true : false;
	GetParent()->SendMessage(USMG_NOTIFY_REVISED, atoi(szTemp));
	//=================================================================================
	
	// �������� �ε� : �������� - ojtaso (20070118)
	LoadRQInfo(strAppName, lpTitle);

	return TRUE;
}

BOOL CChartItem::IsAvailibleCode(LPCTSTR lpszCodeName)
{
//@��������
//	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
////@��������
////	if(GetMarketType(lpszCodeName) == CMasterTypeInfo::FUTOPT_CHART || GetMarketType(lpszCodeName) == COMMODITY_CHART)
//	if(GetMarketType(lpszCodeName) == CMasterTypeInfo::FUTOPT_CHART)
////@��������
//	{
//		IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
//
//		CString strFindCodeName = pDataManager->GetCodeInfo("", 2, lpszCodeName);		
//
//		return atoi(strFindCodeName);
//	}
//	else
//@��������
		return TRUE;
}

// �������� �ε� : �������� - ojtaso (20070118)
void CChartItem::LoadRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle)
{
	UINT nSize = ::GetPrivateProfileInt(lpszAppName, _T("RQSIZE"), 0, lpszTitle);

	CString strRQData;
	LPSTR lpszRQDAta = strRQData.GetBufferSetLength(nSize + 1);

	::GetPrivateProfileString(lpszAppName, _T("RQ"), _T("0"), lpszRQDAta, nSize, lpszTitle);

	strRQData.ReleaseBuffer();

	CString strTemp;
	int nIndex = 0;
	if(!AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';')))
		return;

	int nCount = atoi(strTemp);

	if(!nCount)
		return;

	AfxExtractSubString(m_strSelectedRQ, strRQData, nIndex++, _T(';'));

	m_bLoadRQ = TRUE;

	CChartMng* lpChartMng = ((CChartMng*)GetParent());
	CString strShareCode = lpChartMng->m_strStatic_ShareCode;

	BOOL bChangeCodeByShare = TRUE;
	BOOL bDefualtRQ = FALSE;

	CString strRQ;
	for(int i = 0; i < nCount; i++)
	{
		AfxExtractSubString(strRQ, strRQData, nIndex++, _T(';'));

		ST_SELCHART* lpSelChart = NULL;

		bDefualtRQ = !strRQ.Compare(_T("DEFAULT"));

		if(!bDefualtRQ)
			lpSelChart = new ST_SELCHART;
		else
			m_mapChartItem.Lookup(strRQ, (LPVOID&)lpSelChart);

		AfxExtractSubString(lpSelChart->m_strChartGubun, strRQData, nIndex++, _T(';'));

		if(bDefualtRQ)
		{
			if(((lpSelChart->m_strChartGubun == QUERY_STRJONGMOK && strShareCode.GetLength() == CODE_STOCK_LENGTH) || 
				(lpSelChart->m_strChartGubun == QUERY_STRFUTOPT && (strShareCode.GetLength() == FUTOPT_LENGTH))) &&
				lpChartMng->m_nStatic_RecvCntShareCode > 0 && !strShareCode.IsEmpty() && bChangeCodeByShare
				)
			{
				lpSelChart->m_strItemCode = strShareCode;
				lpSelChart->m_strItemName = ((CStdDialog*)m_pStdDialog)->GetJongMokNameByCode(strShareCode);
				nIndex += 2;
			}
			else
			{
				AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
				AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));
			}
		}
		else
		{
			AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
			AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));

			if(bChangeCodeByShare)
			{
				bChangeCodeByShare = lpSelChart->m_strItemCode.CompareNoCase(strShareCode);
				if(!bChangeCodeByShare)
					m_strSelectedRQ = strRQ;
			}
		}

		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		//lpSelChart->m_cType = strTemp.GetAt(0);
		lpSelChart->m_cType = '2';
		AfxExtractSubString(lpSelChart->m_strTickMinDayNumber, strRQData, nIndex++, _T(';'));
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_lCount = atol(strTemp);
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_nChartNumericValueType = atoi(strTemp);
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_strKey = strTemp;

		m_mapChartItem.SetAt(strRQ, lpSelChart);

		if(!m_strSelectedRQ.Compare(strRQ))
		{
			m_strSelectedItemCode = lpSelChart->m_strItemCode;
			m_lpSelChart = lpSelChart;
		}
	}
}

BOOL CChartItem::LoadChartSetting(LPCTSTR lpTitle)
{
	//05.09.26 ===========================================================
	if( (m_lColIndex < 0) || (m_lColIndex > 20) ) return FALSE;
	if( (m_lRowIndex < 0) || (m_lRowIndex > 20) ) return FALSE;
	//====================================================================

	int nSize = m_arrayChartItemData.GetSize();	
	if(!nSize) return FALSE;
	CString strAppName;

	

	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);
	CString strLoadFileName = lpTitle;
	int nPos = strLoadFileName.Find('.');
	strLoadFileName.Delete(nPos,strLoadFileName.GetLength()-nPos);
	while(1)
	{
		nPos = strLoadFileName.Find('\\');
		if(nPos!=-1)
		{
			strLoadFileName.Delete(0,nPos+1);
		}
		else
		{
			break;
		}	
	}
	strLoadFileName += '_';
	strLoadFileName += strAppName;

	SaveandOpenFromFile(strLoadFileName,"", TRUE, FALSE);


	//====================================================================================
	// 05.09.23
	// ��ȭ���� ������ ť�� ���� �׿� ��Ʈ Data��ȸ�� ������ 
	// ������ �� �޴� ��찡 ���� �Ͼ��. ��.��;;
	// �̷� ��� ���� �� ��Ʈ�� �������� ������ ��ȸ�� 
	// ��Ʈ�� ����� �� �׷����� ������ �߻��ϴ� ��
	// �� ������ �ذ��ϱ� ���� ��Ʈ ���� �ִ��� üũ�ؼ� ���ٸ� 
	// �ʿ��� �⺻ ��Ʈ ���� �ִ´�.
	//====================================================================================
	if( !HasGraph("������Ʈ"))
	{
		AddGraph(0,0,0,"������Ʈ"		,FALSE	,FALSE	,FALSE	); 
//		AddGraph(0,0,0,"���� �̵����"	,FALSE	,FALSE	,FALSE	);
//		AddGraph(1,0,0,"�ŷ�����Ʈ"		,TRUE	,TRUE	,TRUE	);
		//AddGraph(1,0,0,"�ŷ��� �̵����",FALSE	,FALSE	,FALSE	);
	}
	//====================================================================================


	if( GetAllCnt() < GetNOnePageDataCount() )
		SetNew_OnePageDataCount(GetAllCnt());

	
//	ChangePriceGraphDrawingData(!m_bRateValue,!m_bRateValue,!m_bRateValue,FALSE,FALSE,1);
/*	if(m_nChartNumericValueType == WON_TYPE)
		ChangePriceGraphDrawingData(TRUE,TRUE,TRUE,FALSE,FALSE,1);	
	else
		ChangePriceGraphDrawingData(FALSE,FALSE,FALSE,FALSE,FALSE,1);
*/	
	
	//2005. 09. 13 pm 06:26 �����븮 ��û���� �� ���� ChangePriceGraphDrawingData(TRUE,TRUE,TRUE,TRUE,FALSE,1);	
	

//	AddChartBlock("��");
	return TRUE;
}

BOOL CChartItem::SaveChartInfo(LPCTSTR lpTitle, BOOL bOnlyJongmok /*=FALSE*/)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartInfo::Begin", _T("����Ʈ ����"), 13);
#endif
	if(!m_bUseInnerControl&&!m_bBuild) return FALSE;

	long lSize = m_arrayChartItemData.GetSize();
	CString strAppName;
	CString strTemp;	
	CString strTemp2;	
	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);

	long lCntOfGraph = m_arrayChartItemData.GetSize();	
	
	CChartItemData *pData = NULL;
	BOOL bHasChartItemData = FALSE;
	long lCntChartItemData = 0;

	if(lCntOfGraph == 0)
	{
		::WritePrivateProfileString(strAppName, "Code00","",lpTitle);
		::WritePrivateProfileString(strAppName, "TRCode00","",lpTitle);	
	}
	else
	{	
		for(long lPos=0;lPos<lCntOfGraph;lPos++)
		{
			bHasChartItemData = TRUE;
			pData = m_arrayChartItemData.GetAt(lPos);		
			if(lPos!=0)
			{
				//bHasChartItemData = HasChartItemData(pData->GetCode(),pData->GetLTrCode(), FALSE, FALSE);
				//��ȭ������ CString Type TRCode�� ����. 05.05. 06
				CString strCode		= pData->GetCode();
				CString strTRCode	= pData->GetTrCode();


	//#ifdef _DEBUG
	//			if( (strTRCode == QUERY_STRMARKET) || (strTRCode == QUERY_STRMARKET_NEWS) )
	//				bHasChartItemData = TRUE;
	//			else
	//				bHasChartItemData = HasChartItemData(pData->GetCode(),pData->GetTrCode(), FALSE, FALSE);
	//#else
				bHasChartItemData = HasChartItemData(pData->GetCode(),pData->GetTrCode(), FALSE, FALSE);
	//#endif
			}
			if(bHasChartItemData)
			{
				strTemp.Format("Code%02d",lCntChartItemData);
				strTemp2.Format("TRCode%02d",lCntChartItemData);
				//if(bOnlyJongmok&&lPos==0&&pData->GetLTrCode()!=QUERY_JONGMOK)
				//��ȭ������ CString Type TRCode�� ����. 05.05. 06
				/*
				if(bOnlyJongmok&&lPos==0&&pData->GetTrCode()!=QUERY_STRJONGMOK)
				{
					::WritePrivateProfileString(strAppName,strTemp,"000000",lpTitle);
					::WritePrivateProfileString(strAppName,strTemp2,QUERY_STRJONGMOK,lpTitle);
				}
				else
				{
					::WritePrivateProfileString(strAppName,strTemp,pData->GetCode(),lpTitle);
					::WritePrivateProfileString(strAppName,strTemp2,pData->GetTrCode(),lpTitle);
				}
				*/
				if(bOnlyJongmok && lPos == 0)
				{
					::WritePrivateProfileString(strAppName,strTemp,"",lpTitle);
					::WritePrivateProfileString(strAppName,strTemp2,pData->GetTrCode(),lpTitle);
				}
				else
				{	
					::WritePrivateProfileString(strAppName,strTemp,pData->GetCode(),lpTitle);
					::WritePrivateProfileString(strAppName,strTemp2,pData->GetTrCode(),lpTitle);
				}
				
				lCntChartItemData++;
			}
		}
	}



	strTemp.Format("%d",lCntChartItemData);
	::WritePrivateProfileString(strAppName,"CntOfGraph",strTemp,lpTitle);
	
	//strTemp.Format("%d",m_lAllCnt);
	//::WritePrivateProfileString(strAppName,"AllCnt",strTemp,lpTitle);
	strTemp.Format("%d",m_nSaveAllCount);
	if(m_nSaveAllCount <10)
		strTemp.Format("%d",DEFAULTCNTATONCE);
	::WritePrivateProfileString(strAppName,"AllCnt",strTemp,lpTitle);

	strTemp.Format("%d",GetNOnePageDataCount());	
	if(atoi(strTemp) <1)
		strTemp.Format("%d",120);
	::WritePrivateProfileString(strAppName,"CntInView",strTemp,lpTitle);

	strTemp.Format("%c",m_chType);
	::WritePrivateProfileString(strAppName,"Type",strTemp,lpTitle);
	
	//=================================================================================
	// 2005. 05. 23 added  by nam	- ƽ�п� ���� ������ �����Ѵ�.
	// 2005. 09. 12 edited by nam	- ƽ�п� ��� �����Ѵ�.
	//=================================================================================
	::WritePrivateProfileString(strAppName,"TickValue",	m_strTick,	lpTitle);		
	::WritePrivateProfileString(strAppName,"MinValue" ,	m_strMin ,	lpTitle);
	::WritePrivateProfileString(strAppName,"DayValue" ,	m_strDay ,	lpTitle);
//	switch(m_chType)
//	{	
//		case TICK_DATA_CHART:				
//			::WritePrivateProfileString(strAppName,"TickValue",m_strTickMinDay,lpTitle);
//			break;
//		case MIN_DATA_CHART:
//			::WritePrivateProfileString(strAppName,"MinValue",m_strTickMinDay,lpTitle);
//			break;
//		case DAY_DATA_CHART:
//			::WritePrivateProfileString(strAppName,"DayValue",m_strTickMinDay,lpTitle);
//			break;
//
//	}	
	//::WritePrivateProfileString(strAppName,"TickMinNum",m_strTickMin,lpTitle);
	//=================================================================================
	
	
	//05.06.22 by sy, nam
	// ��, %, �޷��� 3���� ������ �ʿ��Ѵ�. 
	//strTemp.Format("%d",GetUseRateValue());
	//::WritePrivateProfileString(strAppName,"UseRateValue",strTemp,lpTitle);
	strTemp.Format("%d",GetChart_NumericValueType());
	::WritePrivateProfileString(strAppName,"ChartUnitValue",strTemp,lpTitle);	
	
	//=================================================================================
	// 2005. 08. 27 add by nam
	// ������ ��Ʈ�� ������ ���� 
	//=================================================================================
	// bRealCandle			// �Ǻ� or �Ǻ� + ���
	// bShowHLBar			// �����ѹ� ǥ��
	// bShowRight			// �ϸ��� / ���� ǥ��
	// bRecvTimeShare		// �ð����� ����
	// bAutoSave			// ��Ʈ ����� �ڵ����� ����
	// bShowLog				// ��Ʈ �α�ǥ�� ����
	// bRevisedValue		// �����ְ� ǥ�� ����
	// bStartDate			// ��Ʈ�� ���õǰ� �ִ� ��¥�� ��� ���ٿ��� ��ȸ �������� �����ߴ��� ����.

	strTemp.Format("%d", m_CfgInfo.bRealCandle		);	
	::WritePrivateProfileString(strAppName,	"bRealCandle"		,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bShowHLBar		);
	::WritePrivateProfileString(strAppName,	"bShowHLBar"		,strTemp,lpTitle);

	//05. 08. 31 ======== �� ������ OCX�� �����Ѵ�. ===============================================
	//strTemp.Format("%d", m_CfgInfo.bShowRight		);
	//::WritePrivateProfileString(strAppName,	"bShowRight"		,strTemp,lpTitle);
	//============================================================================================

	strTemp.Format("%d", m_CfgInfo.bRecvTimeShare	);
	::WritePrivateProfileString(strAppName,	"bRecvTimeShare"	,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bAutoSave		);
	::WritePrivateProfileString(strAppName,	"bAutoSave"			,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bShowLog			);
	::WritePrivateProfileString(strAppName,	"bShowLog"			,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bRevisedValue	);
	::WritePrivateProfileString(strAppName,	"bRevisedValue"		,strTemp,lpTitle);
	//=================================================================================

	if(HasGraph("������Ʈ")) //05.10.26 =============== ���� ��Ʈ�� ���� ���� �����Ѵ�.
	{
		SaveChartSetting(lpTitle);
	}

	// �������� ���� : �������� - ojtaso (20070118)
	SaveRQInfo(strAppName, lpTitle);

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartInfo::End", _T("����Ʈ ����"), 13);
#endif
	return TRUE;
}

// �������� ���� : �������� - ojtaso (20070118)
void CChartItem::SaveRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveRQInfo::Begin", _T("����Ʈ ����"), 13);
#endif
	POSITION pos = m_mapChartItem.GetStartPosition();
	CString strKey;
	ST_SELCHART* lpSelChart = NULL;

	CString strValue, strTemp;
	strValue.Format(_T("%d;%s"), m_mapChartItem.GetCount(), m_strSelectedRQ);

	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)lpSelChart);
		strTemp.Format(_T(";%s;%s;%s;%s;%c;%s;%ld;%d;%s"), strKey,
			lpSelChart->m_strChartGubun,
			lpSelChart->m_strItemCode,
			lpSelChart->m_strItemName,
			lpSelChart->m_cType,
			lpSelChart->m_strTickMinDayNumber,
			lpSelChart->m_lCount,
			lpSelChart->m_nChartNumericValueType,
			lpSelChart->m_strKey);

		strValue += strTemp;
	}

	strTemp.Format(_T("%d"), strValue.GetLength() + m_strSelectedRQ.GetLength() + 1);

	::WritePrivateProfileString(lpszAppName,	"RQSIZE"	,strTemp,lpszTitle);
	::WritePrivateProfileString(lpszAppName,	"RQ"		,strValue,lpszTitle);
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveRQInfo::End", _T("����Ʈ ����"), 13);
#endif
}

BOOL CChartItem::SaveChartSetting(LPCTSTR lpTitle)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartSetting::Begin", _T("����Ʈ ����"), 13);
#endif
	CString strAppName;
	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);
	CString strSaveFileName = lpTitle;
	int nPos = strSaveFileName.Find('.');
	strSaveFileName.Delete(nPos,strSaveFileName.GetLength()-nPos);
	while(1)
	{
		nPos = strSaveFileName.Find('\\');
		if(nPos!=-1)
		{
			strSaveFileName.Delete(0,nPos+1);
		}
		else
		{
			break;
		}	
	}
	strSaveFileName += '_';
	strSaveFileName += strAppName;
	SaveandOpenFromFile("", strSaveFileName, FALSE, TRUE);
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartSetting::End", _T("����Ʈ ����"), 13);
#endif
	return TRUE;
}
//////////////////////////////////////////////////////////////
///////////////////// func For Managing arrayChartItem

//BOOL CChartItem::RemoveChartItemData(LPCTSTR lpCode, UINT nQueryType) 
BOOL CChartItem::RemoveChartItemData(LPCTSTR lpCode, CString strQueryType) 
{
	CChartItemData *pChartItemData = NULL;
	//long lIndex = GetPosChartItemData(nQueryType,lpCode);
	long lIndex = GetPosChartItemData(strQueryType,lpCode);
	if(lIndex==-1) return FALSE;
	pChartItemData = m_arrayChartItemData.GetAt(lIndex);
	delete pChartItemData;
	pChartItemData = NULL;
	m_arrayChartItemData.RemoveAt(lIndex);
	return TRUE;
}


BOOL CChartItem::DestoryItemData()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestoryItemData::Begin", _T("����Ʈ ����"), 13);
#endif

	m_bReceivedDataChart = FALSE; //05.09.22

	CChartItemData *pChartItemData = NULL;
#ifdef _WRITELOG
	WriteToStringLog("m_arrayChartItemData::RemoveAll::Begin", _T("����Ʈ ����"), 13);
#endif
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pChartItemData = m_arrayChartItemData.GetAt(nPos);
		delete pChartItemData;
	}
	m_arrayChartItemData.RemoveAll();
#ifdef _WRITELOG
	WriteToStringLog("m_arrayChartItemData::RemoveAll::End", _T("����Ʈ ����"), 13);
	WriteToStringLog("m_arrayChartItemDataBackup::RemoveAll::Begin", _T("����Ʈ ����"), 13);
#endif
	nSize = m_arrayChartItemDataBackup.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pChartItemData = m_arrayChartItemDataBackup.GetAt(nPos);
		delete pChartItemData;
	}
	m_arrayChartItemDataBackup.RemoveAll();
#ifdef _WRITELOG
	WriteToStringLog("m_arrayChartItemDataBackup::RemoveAll::End", _T("����Ʈ ����"), 13);
	WriteToStringLog("m_arrayMarketTRData::RemoveAll::Begin", _T("����Ʈ ����"), 13);
#endif

	return TRUE;
}

CChartItemData *CChartItem::FindChartItemDataForQuery()
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	
//	CString sTemp;
//	sTemp.Format("m_arrayChartItemData's size is %d\n", nSize);
//	OutputDebugString(sTemp);

	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		if(!pItemData->IsFilled())
		{
			return pItemData;
		}
	}
	return NULL;
}

BOOL CChartItem::ClearDataExceptPrimaryData()
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=nSize-1;nPos>0;nPos--)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		m_arrayChartItemData.RemoveAt(nPos);
		delete pItemData;
	}
	return TRUE;
}

BOOL CChartItem::ClearData()
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();

	// gm0604 added 2005.10.10
	if(m_lPtrST&&nSize>0)
	{
		m_lPtrST = NULL;// gm0604 added 2005.9.6
		CChartItemData *pItemDataNew = NULL;
		pItemData = m_arrayChartItemData.GetAt(0);
		CString strTrCode = pItemData->GetTrCode();
		CString strCode = pItemData->GetCode();
		pItemData->Init(this,strTrCode);
		pItemData->SetCodeForQuery(strCode);	

		CStringArray arrayGraph;
		InvokeAddIn("ChartStrategyAddin.dll",8,(long)&arrayGraph);
		int nSize = arrayGraph.GetSize();
		for(int i=0;i<nSize;i++)
		{
			RemoveChart(arrayGraph.GetAt(i));
		}
		//((CChartMng*)GetParent())->MoveCharts(m_lRowIndex,m_lColIndex);
		((CChartMng*)GetParent())->MoveCharts();
	}
	m_lPtrST = NULL;// gm0604 added 2005.9.6


	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		pItemData->ClearData();
	}	
	if(GetDataTRCode()==QUERY_STRJONGMOK)
	{
		((CChartMng*)GetParent())->UnhighlightJCode(this);
	}		
	return TRUE;
}

//CChartItemData *CChartItem::GetChartItemDataNew(long lTrCode,LPCTSTR lpCodeForQuery, BOOL bCreateIfNoExist /*= TRUE*/,BOOL bIsPrimaryData /*= FALSE */)
CChartItemData* CChartItem::GetChartItemDataNew(CString strTRCode,LPCTSTR lpCodeForQuery, BOOL bCreateIfNoExist /*= TRUE*/,BOOL bIsPrimaryData /*= FALSE */)
{
	CString strCodeForQuery;
	CChartItemData *pItemData = NULL;
	if(lpCodeForQuery!=NULL)
	{
		strCodeForQuery = lpCodeForQuery;
	}	
	if(strCodeForQuery.IsEmpty()) return NULL;

	//@.080730-001
	m_strNewRequestCode = strCodeForQuery;

	//2005.05.06.
	//CString strTrCode;
	//strTrCode.Format("%d",lTrCode);

	int nSize = m_arrayChartItemData.GetSize();	
	if(bIsPrimaryData&&nSize)
	{
		// Clear the same ItemData Without PrimaryData..
		for(int nPos=1;nPos<nSize;nPos++)
		{
			pItemData = m_arrayChartItemData.GetAt(nPos);
			if(pItemData->GetTrCode()==strTRCode
				&&pItemData->GetCode()==strCodeForQuery)
			{
				delete pItemData;
				pItemData = NULL;
				m_arrayChartItemData.RemoveAt(nPos);
				nSize--;
			}
		}

		pItemData = m_arrayChartItemData.GetAt(0);
		//if(pItemData->GetLTrCode()==lTrCode)	// if Primary Data 's TrCode is the same as New Data's , ...
		//��ȭ������ CString Type TRCode�� ����. 05.05. 06
		if(pItemData->GetTrCode()==strTRCode)	// if Primary Data 's TrCode is the same as New Data's , ...
		{
			ClearData();
			//@.080730-001
			pItemData->SetCodeForQuery(strCodeForQuery);

			//if(m_signal.GetSafeHwnd())
			//{
			//	m_signal.OffDrdsAdvise();
			//	m_signal.OnDrdsAdvise(strCodeForQuery);
			//}
		}
		else
		{
			//{{2007.05.30 by LYH ���� ���� ���� ���� �߰� �ϵ��� ����
			ClearData();
			pItemData->SetTRCode(strTRCode);
			//@.080730-001
			pItemData->SetCodeForQuery(strCodeForQuery);
			//}}
		}		
		return pItemData;
	}	

	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		if(strCodeForQuery.IsEmpty())
		{
			if(pItemData->GetTrCode()==strTRCode)
			{
				if(nPos==0&&!bIsPrimaryData)
				{
					return NULL;
				}
				return pItemData;
			}
		}
		else
		{
			if(pItemData->GetTrCode()==strTRCode
				&&pItemData->GetCode()==strCodeForQuery)
			{
				if(nPos==0&&!bIsPrimaryData)
				{
					return NULL;
				}
				return pItemData;
			}
		}
	}


	if(bCreateIfNoExist)
	{	
		pItemData = new CChartItemData(this,strTRCode);
		pItemData->m_pShareInputData = m_pShareInputData;
		pItemData->SetCodeForQuery(strCodeForQuery);	
		m_arrayChartItemData.Add(pItemData);
		//if(m_signal.GetSafeHwnd())
		//{
		//	m_signal.OffDrdsAdvise();
		//	m_signal.OnDrdsAdvise(strCodeForQuery);
		//}
		
#ifdef _DEBUG
		CString sTemp;
		sTemp.Format("GetChartItemDataNew::m_arrayChartItemData ���� %d", m_arrayChartItemData.GetSize());
		OutputDebugString(sTemp);
#endif 
		return pItemData;
	}
	return NULL;	
}

//long CChartItem::GetPosChartItemData(long lTrCode,LPCTSTR lpCodeForQuery)
long CChartItem::GetPosChartItemData(CString strTrCode,LPCTSTR lpCodeForQuery)
{
	CChartItemData *pItemData = NULL;
	//CString strTrCode;
	//strTrCode.Format("%d",lTrCode);
	int nSize = m_arrayChartItemData.GetSize();	
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		if(pItemData->GetTrCode()==strTrCode
			&&pItemData->GetCode()==lpCodeForQuery)
		{
			return nPos;
		}
	}
	return -1;
}

CChartItemData *CChartItem::GetPrimaryChartItemData()
{
	CChartItemData *pItemDataInArray = NULL;
	if(!m_arrayChartItemData.GetSize()) return NULL;
	return m_arrayChartItemData.GetAt(0);
}

BOOL CChartItem::IsPrimaryChartItemData(CChartItemData *pChartItemData)
{
	CChartItemData *pItemDataInArray = NULL;
	if(!m_arrayChartItemData.GetSize()) return FALSE;
	pItemDataInArray = m_arrayChartItemData.GetAt(0);
	if(pItemDataInArray==pChartItemData)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CChartItem::SaveandOpenFromFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
{
	//==============================================================================================
	// ������ �����ϱ� ���� �ش� ��Ʈ�� ����ڰ� �Է��� �������� ������� �����Ѵ�.
	// ��ũ�� �� ���·� �������� �ʴ´�.
	// 2005. 11. 17
	//==============================================================================================		
	//���ϸ��� temp.tmp�� ���� 
	//�Ⱓ����ȭ
	//ȯ�� ����ȭ
	//�Ҷ� ���� �ӽ� ���ϸ��̹Ƿ� 
	//����ڰ� ���Ƿ� ������ ������ �縦�� �ʱ� ���� 
	//����ڰ� �⺻���� �Է��� ������ ������ �ؼ� ������ �������� �ʴ´�.
	CString sSaveFile;
	sSaveFile.Format("%s", strSaveFileName); sSaveFile.TrimLeft(); sSaveFile.TrimRight();

	if( (_T("temp.tmp") != sSaveFile) && sSaveFile.GetLength() )
	{
		if(::IsWindow(GetParent()->GetSafeHwnd()))
		{
			int nCnt = GetParent()->SendMessage(UMSG_GETZOOMVALUE, GetType());
			if(nCnt > 0)
				CDRChartOcx::SetNOnePageDataCount(nCnt);
		}		
	}
	//==============================================================================================

	//2007.01.29 by LYH
	CString sOpenFile;
	sOpenFile.Format("%s", strOpenFileName); sOpenFile.TrimLeft(); sOpenFile.TrimRight();
	if(sOpenFile.Find(".dat") >0)
	{
		CFileFind ff;
		CString sTempPath;
		sTempPath = ((CChartMng*)GetParent())->GetUserpath() + "\\" + sOpenFile;
		BOOL bResult = ff.FindFile(sTempPath);
		if(!bResult)
		{
			ResetChart();
		}
	}
	//}}

	BOOL bRetValue = CDRChartOcx::SaveandOpenFromFile( strOpenFileName, strSaveFileName, bOpenOption, bSaveOption);	
	//==============================================================
	// 2005. 08. 23 added by sy, nam
	// ��Ʈ�� �ε��� ���  LogScale���θ� ���δ�.
	if(bOpenOption) 
	{
		m_CfgInfo.bShowLog = GetLogScale(_T("")) ? true : false;
		m_bLogPrice = m_CfgInfo.bShowLog;
		GetParent()->SendMessage( UMSG_SETLOG, m_CfgInfo.bShowLog, 0);


		if( GetAllCnt() < GetNOnePageDataCount() ) //05.11.24 �������� ���� ���̴� ���...
			SetNew_OnePageDataCount(GetAllCnt());
	}
	//==============================================================

	CDRChartOcx::SetStrUserIndicatorListFileName("");
	CDRChartOcx::SetStrUserEnvironmentFileName("");

	

	/* ���� ���� ���� ��Ʈ���� OCX����... 05.11.24
	if(bOpenOption)
	{
		m_bShowLock = m_CfgInfo.bShowRight;
		ShowLock(m_bShowLock);
	}
	*/
	return bRetValue;
}

void CChartItem::SetNext(BOOL bNext)
{ 
	if(!m_arrayChartItemData.GetSize()) return;
	m_bNext = bNext;
}

void CChartItem::SetCntForRequest(UINT nCntForRequest) 
{ 
	ClearData();
	if(nCntForRequest>CHTMXREQUEST)
	{
		nCntForRequest = CHTMXREQUEST;
	}
	else if(nCntForRequest <= 0) //05.10.14
		nCntForRequest = DEFAULTCNTATONCE;	 //05.10.14

	m_lCntForRequest = nCntForRequest;
	m_lAllCnt = 0;
}

CString CChartItem::GetDataCode(int nIndex/*=0*/)
{ 
	if(m_arrayChartItemData.GetSize()<=nIndex) return "";
	return m_arrayChartItemData.GetAt(nIndex)->GetCode();	
}

CString CChartItem::GetDataTRCode(int nIndex/*=0*/)
{ 
	if(m_arrayChartItemData.GetSize()<=nIndex) return "";
	return m_arrayChartItemData.GetAt(nIndex)->GetTrCode();	
}

long CChartItem::GetCntForRequest()
{ 
	if(m_nTotCountForRevised > 0)
	{
		int nRet = m_nTotCountForRevised;
		m_nTotCountForRevised = 0;
		return nRet;
	}
	return m_lCntForRequest;
}


CString CChartItem::GetEndDateRequested() 
{ 
	return m_strDateForRequest;
}

CString CChartItem::GetEndDate() 
{ 
	return m_strEndDate;
}


//void CChartItem::SetUseFackedCandle(BOOL bUse)
//{
//	ClearData();
//	m_bShowingFackedValue = bUse;
//}



void CChartItem::SetUseLogValue(BOOL bUse)
{	
	m_bLogPrice = bUse;
	m_CfgInfo.bShowLog = bUse ? true : false;
}
/*
void CChartItem::SetUseRateValue(BOOL bUse)
{	
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		pItemData->ClearData();
		pItemData->SetUseRateValue(bUse);
	}	
	m_bRateValue = bUse;
}*/

//2005. 06. 22 add by sy, nam 
//�� ��Ʈ�� %, $, W�� ���а��� ���� �Ѵ�. 
void CChartItem::SetChartNumericValue(int nNumericValue)
{	
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		pItemData->ClearData();
		pItemData->SetChartNumericValue(nNumericValue);
		
	}	
	//m_nChartNumericValueType =nNumericValue; 05.12.01
	SetChart_NumericValueType(nNumericValue);
}

void CChartItem::SetBNormalizedMinuteTimeRule(BOOL bUse)
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		pItemData->ClearData();		
	}	

	CDRChartOcx::SetBNormalizedMinuteTimeRule(bUse);
}

void CChartItem::HideScale(BOOL bHide)
{
	if(bHide)	
	{
		ChangeHorzScalePosition(CDRChartOcx::HORZ_HIDE, -1, -1);
		ChangeVertScalePosition(CDRChartOcx::VERT_NONE, -1, -1,-1);					
		//ShowInnerToolBar(FALSE);
	}
	else
	{
		ChangeHorzScalePosition(CDRChartOcx::HORZ_BOTTOM, GetRowBlockCount()-1, 0);
		ChangeVertScalePosition(CDRChartOcx::VERT_RIGHT, -1, -1,-1);					
		//ShowInnerToolBar(TRUE);
	}
}

void CChartItem::HideInnerToolBar(BOOL bHide)
{
	if(bHide)	
	{
		ShowInnerToolBar(FALSE);
	}
	else
	{
		ShowInnerToolBar(TRUE);
	}
}

void CChartItem::InvalidateType()
{
	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART	:
		case MIN_DATA_CHART		:
		case HOUR_DATA_CHART	:
		case WEEK_DATA_CHART	:
		case MONTH_DATA_CHART	:
		case YEAR_DATA_CHART	:
		case SEC_DATA_CHART:
//		case SEC10_DATA_CHART: //05.09.28
//		case SEC30_DATA_CHART: //05.09.28
		{
			// remove market
			CChartItemData *pChartItemData = NULL;
			int nSize = m_arrayChartItemData.GetSize();
			for(int nPos=nSize-1;nPos>0;nPos--)
			{
				pChartItemData = m_arrayChartItemData.GetAt(nPos);
				//long lTrCode = atol(pChartItemData->GetTrCode());
				CString strTRCode = pChartItemData->GetTrCode();
				CString strCode = pChartItemData->GetCode();
//				if( (strTRCode == QUERY_STRMARKET_10450) ||
//					(strTRCode == QUERY_STRMARKET_10451) ||
//					(strTRCode == QUERY_STRMARKET_10452)
//				)
				if(strTRCode == QUERY_STRMARKET)
				{
					m_arrayChartItemData.RemoveAt(nPos);
					delete pChartItemData;
				}
			}
		}
		break;
	}
}


BOOL CChartItem::PtInRectJongmokTitle(POINT point) const
{
	return m_rectTitleJongmok.PtInRect(point);
}


//const UINT RMSG_FORMSTATECHANGE = ::RegisterWindowMessage(_T("RMSG_FORMSTATECHANGE"));
void CChartItem::ResetChartCfgWith(short p_nIndex)
{
	long lCnt = GetNOnePageDataCount();
	SetNew_OnePageDataCount(120);

	//��ü���� ����
	SetBWholeView(0);

	//���߿� ����
//	SendMessage(RMSG_FORMSTATECHANGE, 500, 0);
//
	CDRChartOcx::ResetChartCfgWith(p_nIndex);
	SetNew_OnePageDataCount(lCnt);
}

long CChartItem::GetTempOnePageDataCount() 
{
	long lTemp = m_lNOnePageDataCount;
	m_lNOnePageDataCount = 0;
	return lTemp; 
}


CString CChartItem::GetQueryDataForGuideLine(const CUIntArray* pAryAvgForChartGuideLine)
{

	if(GetPrimaryChartItemData())
	{
		CString strQuoteCode = GetPrimaryChartItemData()->GetCode();
		if(strQuoteCode.GetLength()==CODE_STOCK_LENGTH)
		{
			CString strData;
			D10418_I st10418i;		
			memset(&st10418i,0x20,sizeof(st10418i));
			st10418i.chNull = NULL;
			strData.Format("%d",GetSafeHwnd());
			memcpy(st10418i.szBuffer,(LPCTSTR)strData,strData.GetLength());
			memcpy(st10418i.shcode,(LPCTSTR)strQuoteCode,sizeof(st10418i.shcode));
			st10418i.shcode[6] = ' ';
			strData.Format("%3d",pAryAvgForChartGuideLine->GetAt(0));
			memcpy(st10418i.nAvg1,(LPCTSTR)strData,sizeof(st10418i.nAvg1));
			strData.Format("%3d",pAryAvgForChartGuideLine->GetAt(1));
			memcpy(st10418i.nAvg2,(LPCTSTR)strData,sizeof(st10418i.nAvg2));
			strData.Format("%3d",pAryAvgForChartGuideLine->GetAt(2));
			memcpy(st10418i.nAvg3,(LPCTSTR)strData,sizeof(st10418i.nAvg3));		
			strData.Format("%3d",pAryAvgForChartGuideLine->GetAt(3));
			memcpy(st10418i.nAvg4,(LPCTSTR)strData,sizeof(st10418i.nAvg4));

			if(
			(atoi(st10418i.nAvg1) == 0) &&
			(atoi(st10418i.nAvg2) == 0) &&
			(atoi(st10418i.nAvg3) == 0) &&
			(atoi(st10418i.nAvg4) == 0) 			
			)
			return _T("");

			return (char*)&st10418i;
		}
	}
	return "";
}

void CChartItem::ShowLock(BOOL bShow)
{

	//ȯ�� ����ȭ �� �ݺ��ؼ� ������ ���ؼ�..
	//����Ѵ�.
/*	if(m_nMarketType != CMasterTypeInfo::STOCK_CHART)
	{
		m_bShowLock = FALSE;
		m_CfgInfo.bShowRight = false;
		RemoveChart("��");
		return;
	}

	if( m_nMarketType == CMasterTypeInfo::STOCK_CHART)   //�ֽ� ��Ʈ�̸鼭 	
	{
		BOOL bRemove = FALSE;
		if(m_chType == NULL) //TR�� �޴� Data��  Ÿ���� ������� ��� 
		{
			if(m_chTypeForRequest != DAY_DATA_CHART)  //��û�� Data�� Ÿ���� ���� �ƴ� ��� 
				bRemove = TRUE;
		}
		else
		{
			if(m_chType != DAY_DATA_CHART) //TR�� �޴� Data��  Ÿ���� ���� �ƴ� �ܿ�
				bRemove = TRUE;
		}

		if(bRemove)
		{
			m_bShowLock = FALSE;
			m_CfgInfo.bShowRight = false;
			RemoveChart("��");
			return;	
		}
	}

	m_bShowLock = bShow;
	m_CfgInfo.bShowRight = (bShow == TRUE) ? true : false;

	if(bShow)
	{
		if(!HasGraph("��"))
		{			
			AddChartBlock("��");
		}
	}
	else
	{		
		RemoveChart("��");
	}*/
}

BOOL CChartItem::SendMessageToInvoke(short nCommandType, long lData, long lSize)
{
	POSITION pos = m_listAddIn.GetHeadPosition();
	CString strAddIn;
	stMSGToInvoke stMSG;
	while(pos)
	{
		strAddIn = m_listAddIn.GetNext(pos);
		if(nCommandType==INVOKEMSG_INIT)
		{
			LoadAddIn(strAddIn);			
		}
		stMSG.m_hChartMng	= (HWND)m_plDllWnd;
		stMSG.m_pChartItem	= this;
		stMSG.m_pBuffer		= (void*)lData;
		stMSG.m_dwLength	= lSize;
		InvokeAddIn(strAddIn,nCommandType,(long)&stMSG);
	}
	return TRUE;
}

void CChartItem::Init_OverLapped()
{
	m_nCntOverLap	= 0;		// ��ø�� ���� Cnt;
	m_bOverLapping	= FALSE;	// ��ø ��� Flag
}

void CChartItem::Add_OverLappedCnt()
{
	m_nCntOverLap++;		// ��ø�� ���� Cnt;
	m_bOverLapping	= TRUE;	// ��ø ��� Flag
}

char CChartItem::GetTypeRequested()
{
	//20080526�� ���������� ó���ϴ� ���������� ó���Ѵ�.
	return m_pShareInputData->m_DataInputBar.m_chType;
	
	return m_chTypeForRequest;
}

void CChartItem::LoadTickMinDayInfo(CString stCfgFile)
{
	CString strAppName;
	CString strTemp;
	CString strTemp2;
	char szTemp[128] = {0,};

	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);	

	::GetPrivateProfileString(strAppName,"Type","2",szTemp,sizeof(szTemp),stCfgFile);
	//=====================================================================================
	//2005. 05. 23 �� ��Ʈ�� ������ �����Ѵ�. by sy, nam
	//if(szTemp[0]<'0' || szTemp[0]>'4') 
	//=====================================================================================
	//if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>YEAR_DATA_CHART) 
//	if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>SEC30_DATA_CHART)
	if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>SEC_DATA_CHART)
	{
		szTemp[0] = DAY_DATA_CHART;
	}
	//m_chTypeForRequest = ;
	SetChartRequestType(szTemp[0]);
	//=====================================================================================


	//=====================================================================================
	// ��Ʈ�� ƽ���� �� �ش� Value�� �о m_strTickMinDayForRequest ����� �����Ѵ�.
	//=====================================================================================
	//::GetPrivateProfileString(strAppName,"TickMinNum","01",szTemp,sizeof(szTemp),lpTitle);	
	memset(szTemp, 0x00, sizeof(szTemp));
	//switch(m_chTypeForRequest)
	//{
	//	case TICK_DATA_CHART:			
			::GetPrivateProfileString(strAppName,"TickValue",	"001",	szTemp,sizeof(szTemp),stCfgFile);
			SetTickNumber(CString(szTemp));
	//		break;
	//	case MIN_DATA_CHART:
			::GetPrivateProfileString(strAppName,"MinValue",	"001",	szTemp,sizeof(szTemp),stCfgFile);
			SetMinNumber(CString(szTemp));
	//		break;
	//	case DAY_DATA_CHART:
			::GetPrivateProfileString(strAppName,"DayValue",	"001",	szTemp,sizeof(szTemp),stCfgFile);
			SetDayNumber(CString(szTemp));
			::GetPrivateProfileString(strAppName,"HourValue",	"001",	szTemp,sizeof(szTemp),stCfgFile);
			SetHourNumber(CString(szTemp));
	//		break;
	//}	

	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART:	m_strTickMinDayForRequest = GetTickNumber();	break;
		case MIN_DATA_CHART	:	m_strTickMinDayForRequest = GetMinNumber ();	break;
		case DAY_DATA_CHART	:	m_strTickMinDayForRequest = GetDayNumber ();	break;
		case HOUR_DATA_CHART:	m_strTickMinDayForRequest = GetHourNumber();	break;
		default				:	m_strTickMinDayForRequest = "001"		   ;	break;
	}	


	m_strTickMinDayForRequest = szTemp;
	if(m_strTickMinDayForRequest.IsEmpty())
	{
		m_strTickMinDayForRequest = "001";
	}
}

CString CChartItem::GetTickMinDayNumber(int nChartType)
{
	//===========================================================================
	// Ư�� ��Ʈ ������ ���� ����ڰ� �Ѱ� �ش� ���� ���� ��� ������ �Ҷ� 
	// nChartType�� -1�̻��� ���� �ѱ��.
	// 05. 05. 25 add by sy, nam 
	//===========================================================================
	if(nChartType > -1)
	{
		if(nChartType == 0)
			return GetTickNumber();
		else if(nChartType == 1)
			return GetMinNumber();
		else if(nChartType == 2)
			return GetDayNumber();
		else if (nChartType == 7)
			return GetHourNumber();
		else
			return "001";
	}
	//===========================================================================


	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART:						
			return GetTickNumber();
			break;
		case MIN_DATA_CHART:			
			return GetMinNumber();
			break;
		case DAY_DATA_CHART:			
			return GetDayNumber();
			break;
		case HOUR_DATA_CHART:
			return GetHourNumber();
		default:
			return "001";
			break;
	}
	
	return "001";
}

CString CChartItem::GetTickNumber() 
{
	return m_strTick;
}

CString CChartItem::GetMinNumber()	
{
	return m_strMin;
}

CString CChartItem::GetDayNumber()
{
	return m_strDay;
}

CString CChartItem::GetHourNumber()
{
	return m_strHour;
}

void	CChartItem::SetTickNumber	(CString strValue)
{
	if( atoi(strValue) <= 0) 
	{
		strValue = "001";
	}
	m_strTickMinDay = m_strTick	= strValue;
}

void	CChartItem::SetMinNumber	(CString strValue)
{
	if( atoi(strValue) <= 0)
	{
		strValue = "001";
	}
	m_strTickMinDay = m_strMin	= strValue;
}

void	CChartItem::SetDayNumber	(CString strValue)
{
	if( atoi(strValue) <= 0)
		strValue = "001";
	m_strTickMinDay = m_strDay	= strValue;
}

void	CChartItem::SetHourNumber	(CString strValue)
{
	if( atoi(strValue) <= 0)
		strValue = "001";
	m_strTickMinDay = m_strHour	= strValue;
}

void CChartItem::Init_ChartCfgInfo()
{
	m_CfgInfo.bAutoSave			= true	;
	m_CfgInfo.bRealCandle		= true	;	   // ����Ʈ�� ������� ǥ�ð��Ǿ� �ǽð��� �ȵ��͵� ��Ʈ�� �׷���������������.
	//SetNew_BNormalizedMinuteTimeRule(FALSE); //�Ǻ��� �⺻�̴�.
	m_CfgInfo.bRecvTimeShare	= false	;
	m_CfgInfo.bShowHLBar		= true	;	
	m_CfgInfo.bShowRight		= true	;
	m_CfgInfo.bShowLog			= false ; //05. 08. 25
	m_CfgInfo.bStartDate		= false	; //05. 08. 25
}

void CChartItem::SetChartConfigInfo(CHART_CONFIG_INFO &ChartCfgInfo)
{
	m_CfgInfo.bAutoSave			= ChartCfgInfo.bAutoSave		;
	//=========================================================================	
	m_CfgInfo.bRealCandle		= ChartCfgInfo.bRealCandle		; 
	//05.11.25
	//if(ChartCfgInfo.bRealCandle == true)  //�Ǻ��̸� 
	//	SetNew_BNormalizedMinuteTimeRule(FALSE); 
	//else
	//	SetNew_BNormalizedMinuteTimeRule(TRUE); 
	//=========================================================================

	m_CfgInfo.bRecvTimeShare	= ChartCfgInfo.bRecvTimeShare	;
	m_CfgInfo.bShowHLBar		= ChartCfgInfo.bShowHLBar		;
	m_CfgInfo.bShowRight		= ChartCfgInfo.bShowRight		;	
	m_CfgInfo.bShowLog			= ChartCfgInfo.bShowLog			; //05. 08. 25
	m_CfgInfo.bStartDate		= ChartCfgInfo.bStartDate		; //05. 08. 25
//	m_CfgInfo.bShowCurPrice		= ChartCfgInfo.bShowCurPrice	; //05. 08. 27
}

void CChartItem::InitShareManager()
{
//	m_ShareManager.SetParent(this);
//	m_ShareManager.InitBaseInfo(m_pDrMLib_DBMgr);
//	m_ShareManager.SetHandleInfoToShareBase();
//
//	// ��ȣ�� ������ ����Ѵ�.
//	// 
//	m_ShareManager.AdviseSharedInfo();
//
//	//BaseInfo�� �� set
//	m_pBaseInfo = (CDrMLib_CBaseInfo2*)m_ShareManager.GetBaseInfoPtr();
}

//void CChartItem::CloseShareManager()
//{
//	// ��ȣ�� �������� �����Ѵ�.
//	m_ShareManager.UnAdviseSharedInfo();
//	m_ShareManager.DestroyBaseInfo();
//}


int	CChartItem::GetSignalImageIndex_FromSignalDegree(int nType, int nSignalDegree)
{
	// Image\\��ȣ���ȣ_16.bmp �̹����� Index 
	int nRet_ImageIndex = 0;
	if(nType == e_SAS)
	{
		switch(nSignalDegree) 
		{
		case 3:
			nRet_ImageIndex = 4;
			break;
		case 2:
			nRet_ImageIndex = 5;
			break;
		case 1:
			nRet_ImageIndex = 6;
			break;
		case 0:
			nRet_ImageIndex = 7;
			break;
		case -1:
			nRet_ImageIndex = 8;
			break;
		case -2:
			nRet_ImageIndex = 9;
			break;
		case -3:
			nRet_ImageIndex = 10;
			break;
		default:
			nRet_ImageIndex = 7;
			break;
		}
		
		return nRet_ImageIndex;
	}
	else if(nType == e_NKS)
	{
		if( nSignalDegree ==0 )
			nRet_ImageIndex = 2;
		else if( nSignalDegree > 0 )
			nRet_ImageIndex = 1;
		else if( nSignalDegree < 0 )
			nRet_ImageIndex = 3;


		return nRet_ImageIndex;
	
	}
	else if(nType == e_UAS)
	{
		switch(nSignalDegree) 
		{
		case 3:
			nRet_ImageIndex = 11;
			break;
		case 2:
			nRet_ImageIndex = 12;
			break;
		case 1:
			nRet_ImageIndex = 13;
			break;
		case 0:
			nRet_ImageIndex = 14;
			break;
		case -1:
			nRet_ImageIndex = 15;
			break;
		case -2:
			nRet_ImageIndex = 16;
			break;
		case -3:
			nRet_ImageIndex = 17;
			break;
		default:
			nRet_ImageIndex = 14;
			break;
		}
	
		return nRet_ImageIndex;
	}

	return nRet_ImageIndex;
}

void CChartItem::SetSignalData(CString sShareName, BYTE* pData, int nLen)
{
//	stChartSign stSig;
//	memcpy(&stSig,pData,sizeof(stChartSign));
//	
//	if(memcmp(stSig.RepresentCode,GetDataCode(),CODE_LEN))
//	{
////		CString sTemp;
////		sTemp.Format("Signal...%6.6s" , stSig.RepresentCode);
////		OutputDebugString(sTemp);
////		sTemp = "SignalChart..." + GetDataCode();
////		OutputDebugString(sTemp);
//		return;
//	}
//
//	int nType = 0;
//			if(	sShareName == SHARE_SIG_SAS_ALARM) 	nType = e_SAS;
//	else	if(	sShareName == SHARE_SIG_NKS_ALARM) 	nType = e_NKS; 
//	else	if( sShareName == SHARE_SIG_UAS_ALARM) 	nType = e_UAS;
//	else	return;
//	
//	int nSignal = GetSignalImageIndex_FromSignalDegree(nType, stSig.nSignal);
//
//	m_signal.FP_vSetImage(m_pIL_Symbol, nSignal, nSignal);		
}
/*
void CChartItem::SetShowCurrentPriceToOutside(const bool bShow)
{
	m_CfgInfo.bShowCurPrice = bShow;	
	ChangePriceGraphDrawingData(bShow, bShow, bShow, bShow, false, 1);	
}*/

void CChartItem::SetAutoSaveToOutside(const bool bAutoSave)
{
	m_CfgInfo.bAutoSave = bAutoSave;
}

BOOL CChartItem::GetUseRevisedValue()
{
	m_bRevisedPrice = m_CfgInfo.bRevisedValue;
	return m_bRevisedPrice;
}


void CChartItem::SetUseRevisedValue(BOOL bUse)
{
	//{{ �����ְ� Ŭ���� ��ü�� ������Ű�� ���� �߰� by LYH 2007.01.11
	if(m_bRevisedPrice != bUse)
	{
		if(m_lAllCnt>m_lCntForRequest)
		{
			if(m_lAllCnt>CHTMXREQUEST)
			{
				m_nTotCountForRevised = CHTMXREQUEST;
			}
			else
			{
				m_nTotCountForRevised = m_lAllCnt;
			}				
		}	
	}
	//}}
	ClearData();
	m_bRevisedPrice = bUse;
	m_CfgInfo.bRevisedValue = bUse ? true : false;
}

//DEL void CChartItem::SetTypeRequested(char cType)
//DEL {
//DEL 	m_chTypeForRequest = cType;
//DEL }

void CChartItem::SetEndDate(LPCTSTR endDate)  
{
	m_strEndDate = endDate;
	if(m_strEndDate.GetLength()!=8)
	{
		CTime t = CTime::GetCurrentTime();
		m_strEndDate = t.Format("%Y%m%d");	
	}
}


void CChartItem::SetOldChartCode(CString strCode)
{
	m_strOldCode = strCode;
}


void CChartItem::OpenSignalMapScr()
{	
//	CDrMLib_CBaseInfo2* pMainBaseInfo = ((CStdDialog*)GetParent()->GetParent())->GetpMainBaseInfo();
//	if(pMainBaseInfo)
//	{		
//		CString strCode = GetDataCode();
//		if(strCode.GetLength() == CODE_STOCK_LENGTH)
//		{			
//			pMainBaseInfo->SetSharedData2(	0,
//											0,
//											_T("�����ڵ�"),
//											(BYTE*)(LPTSTR)(LPCTSTR)strCode,
//											CODE_STOCK_LENGTH,
//											m_pDrMLib_DBMgr->GetCodeType(strCode));
//
//
//
//
//
//
//											
//			AfxGetMainWnd()->PostMessage(WMU_OPEN_MAP,-6,(LPARAM)"6100");
//		}
//	}

}

BOOL CChartItem::AddInEvent_FromChart(LPCSTR _szParam)
{
	//EUGENE ȣ������ ��� ���� by LYH 2006.12.20
	//CString strParam;
	//char *szParam = strParam.GetBufferSetLength( strlen( _szParam));
	//strcpy( szParam, _szParam);

	//CString strData;
	//strData.Format("%s", szParam);
	//if(strData.GetLength() < 1)
	//	return FALSE;

	//int nIndex = strData.Find("strData ");
	//int nStart = strData.Find("\"", nIndex);
	//int nEnd = strData.Find("\"", nStart + 1);

	//
	//CString strPrice;
	//strPrice = strData.Mid(nStart+1, nEnd-nStart-1);
	//((CStdDialog*)m_pStdDialog)->SendHogaNCodeToSpeedJumun(GetDataCode(), strPrice);

//	char *szAddInName = szParam;
//	
//	char *szEventID = strchr( szAddInName, ',');
//	if( !szEventID) return FALSE;
//	*szEventID = '\0';
//	szEventID++;
//
//	char *szPrice = strchr( szEventID, ',');
//	if( !szPrice) return FALSE;
//	*szPrice = '\0';
//	szPrice++;
//
//	char *szEtc = strchr( szPrice, ',');
//	if( !szEtc) return FALSE;
//	*szEtc = '\0';
//
//	
//	CString strMsg;	strMsg.Format("%s,%s,%s", szAddInName, szEventID, szPrice);
//	
//	CString sData;
////	sData.Format("%s?%s", GetDataCode(), szPrice);
//	sData.Format("%s", szPrice);
//
//	((CStdDialog*)m_pStdDialog)->SendHogaNCodeToSpeedJumun(GetDataCode(), sData);
//	//AfxMessageBox(strMsg);
	return TRUE;
}

//DWORD CChartItem::GetCodeType_By_DBMgr(CString strCode)
//{
//	return m_pDrMLib_DBMgr->GetCodeType(strCode);
//}

void CChartItem::SetRowNCol(long lRowIndex, long lColIndex)
{ 
	if(lRowIndex < -1)
		OutputDebugString("aa");

	m_lColIndex = lColIndex;
	m_lRowIndex = lRowIndex;
}

BOOL CChartItem::SetPacketData(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset)
{
	if(m_lPtrST)
	{
		CPtrArray ptrArray;
		ptrArray.Add((void*)m_lPtrST);
		CString strType;
		char chType = GetType();
		if(chType==DAY_DATA_CHART)
		{
			strType = "��";
		}
		else if(chType==MIN_DATA_CHART)
		{
			strType = "��";
		}
		else if(chType==TICK_DATA_CHART)
		{
			strType = "ƽ";
		}
		else if(chType==HOUR_DATA_CHART)
		{
			strType = "��";
		}
		else
		{
			ASSERT(0);
		}			
		ptrArray.Add((void*)(LPCTSTR)strType);
		BOOL bRetValue = InvokeAddIn("ChartStrategyAddin.dll",6,(long)&ptrArray);
		if(bRetValue)
		{
			szData = (long)ptrArray.GetAt(0);
			nLength = (long)ptrArray.GetAt(1);
		}

		bRetValue = CDRChartOcx::SetPacketData(szPacketName,szData,nLength,bReset);
		bRetValue = InvokeAddIn("ChartStrategyAddin.dll",7,szData);
		return bRetValue;
	}
	else
	{
		// �������� �߰� UI : �������� - ojtaso (20070522)
		if(((CChartMng*)GetParent())->GetAddModeFromTab())
			CDRChartOcx::SetResetTotalCountWhileDraw(FALSE);

		return CDRChartOcx::SetPacketData(szPacketName,szData,nLength,bReset);
	}
	return TRUE;
	
}


void CChartItem::SetNew_OnePageDataCount(int nPCnt)
{
	//SetNOnePageDataCount(nPCnt); <== ����ÿ� �Ӽ������θ� ������ ī��Ʈ�� �����ϱ� ����Ѵ�.
	CDRChartOcx::SetOnePageDataCount(nPCnt); //<== ���� ������ ī��Ʈ�� �����Ѵ�.
#ifdef _DEBUG
	CString sTemp;
	sTemp.Format("SetNew_OnePageDataCount : %d", nPCnt);
	OutputDebugString(sTemp);
#endif
}

const char* CChartItem::GetNextKey(BOOL bMarket)
{
	if( m_arrayChartItemData.GetSize() )
	{
		CChartItemData* pItemData = NULL;
		pItemData = m_arrayChartItemData.GetAt(0); //ù��°�� ��Ʈ �⺻ ������
		if(bMarket)
			return pItemData->GetNextKeyOld();
		else
			return pItemData->GetNextKey();
	}
	return NULL;	
}

void CChartItem::SetNew_BNormalizedMinuteTimeRule(BOOL bFakeCandle)
{
	m_CfgInfo.bRealCandle = bFakeCandle ? false : true; //bFakeCandle�� TRUE�� ���,  FALSE�� �Ǻ�
														//�� m_CfgInfo.bRealCandle�� �Ǻ��̸� TRUE�̹Ƿ�	
														//bFakeCandle�� �ݴ��� ���� ���Ѵ�.
	CDRChartOcx::SetBNormalizedMinuteTimeRule(bFakeCandle);
}

void CChartItem::InitChartItem_FromExtToolBar()
{
	m_bShowLock = TRUE;
}

void CChartItem::SetChart_NumericValueType(int nValue)
{
	m_nChartNumericValueType = nValue;

	//���ؼ� Addin���� chart type�� �˸���.
	InvokeAddIn("ChartGuideLine.dll", 104, nValue);	
}

void CChartItem::PrintChart()
{
	//SendMessage(WM_COMMAND, 32816, 0);
	CDRChartOcx::PrintChatImage();
}

void CChartItem::SetExcelData()
{
	//SendMessage(WM_COMMAND, 32826, 0);
	CDRChartOcx::ExportToExcel();
}

LPCSTR CChartItem::CXIBaseDesk::GetBasePath()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	m_szPath = (LPCSTR)AfxGetPctrData(10);		// nID=10, ROOT ���丮 ���ϱ�. ������(����_PC�����͸Ŵ���.xls)
	return m_szPath;
}

HWND  CChartItem::CXIBaseDesk::GetDeskHwnd()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	
	return pThis->m_pBaseDesk2->GetDeskHwnd();
}

LPDISPATCH CChartItem::CXIBaseDesk::GetUnknown()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetUnknown();
}

LPUNKNOWN CChartItem::CXIBaseDesk::GetCtrlUnknown(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlUnknown(szName);
}

HWND CChartItem::CXIBaseDesk::GetCtrlHwnd(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlHwnd(szName);
}

LPVOID CChartItem::CXIBaseDesk::GetSharedInterface()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	//if(pThis->m_pBaseDesk2)
	//	return pThis->m_pBaseDesk2->GetSharedInterface();
	//else
		return NULL;
}

BOOL CChartItem::CXIBaseDesk::InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

//	return pThis->InvokeEvent(_szEventName, _szParam);
//AfxMessageBox(_szEventName);
//	return pThis->m_pBaseDesk2->InvokeEvent(_hWnd, _szEventName, _szParam);
	if( _szEventName == CString( "AddInEvent"))
	{
		pThis->AddInEvent_FromChart(_szParam);
		return TRUE;
	}
	return false;
}

BOOL CChartItem::CXIBaseDesk::InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

//	return pThis->InvokeEvent(_szEventName, _szParam);
	return pThis->m_pBaseDesk2->InvokeEvent2(_szCtrlName, _szEventName, _szParam);
}
	
BOOL CChartItem::CXIBaseDesk::SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) 
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->SendKey(_hWnd, nChar, _bIsShift, _bIsCtrl);
}

LPVOID CChartItem::CXIBaseDesk::GetLSharedData()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetLSharedData();
}



///////////////////////////////////////////////////////////////////////////////
// ������            : �ѵ���(Dong-Hyuk, Han)
// Email             : aviman@dooriic.co.kr
// ���� ��¥         : 2002/2/1
// �Լ� �̸�         : CChartItem::CXIBaseDesk::CallCommand
// ���� Ÿ��         : void
// �Ķ����
//		LPCSTR _szCmd  : 
// �Լ� ����         : 
///////////////////////////////////////////////////////////////////////////////
void CChartItem::CXIBaseDesk::CallCommand(LPCSTR _szCmd)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	pThis->m_pBaseDesk2->CallCommand(_szCmd);
}

// * ����� �÷� ������ �����Ѵ�.
OLE_COLOR CChartItem::CXIBaseDesk::GetUserColor(short nIndex, LPCTSTR szName, short nFlag)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetUserColor(nIndex, szName, nFlag);
}

// * ���� �ƹ��͵� Ű���� ���� ���ǰ� �Ǿ����� �����Ƿ� �θ���.
// * �䱸 �Ǵ� �ʿ䰡 ������ ���⿡�� �����Ѵ�.
long CChartItem::CXIBaseDesk::GetDeskInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetDeskInfo(szKey);
}

long CChartItem::CXIBaseDesk::GetUserInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
		
	return pThis->m_pBaseDesk2->GetUserInfo(szKey);
}

long CChartItem::CXIBaseDesk::GetFrameInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
		
	return pThis->m_pBaseDesk2->GetFrameInfo(szKey);
}

long CChartItem::CXIBaseDesk::GetMainInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
		
//	return pThis->m_pBaseDesk2->GetMainInfo(szKey);
//	if(pThis->m_pBaseDesk2)
//	return pThis->m_pBaseDesk2->GetMainInfo(szKey);
	return NULL;
}




//* ���� ���� (2002.8.22 001) ����͸� �� �߰�
BOOL CChartItem::CXIBaseDesk::FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->FP_vAdviseDebug(sName, pMonitoringCommand);
}


void CChartItem::CXIBaseDesk::FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	pThis->m_pBaseDesk2->FP_vUnadviseDebug(sName, pMonitoringCommand);
}

LPCSTR CChartItem::CXIBaseDesk::FP_sGetMyVBName(HWND hWnd)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->FP_sGetMyVBName(hWnd);
}

void CChartItem::CXIBaseDesk::FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	pThis->m_pBaseDesk2->FP_vSendDebug(csSection, csKey, csValue);
}
//* ���� �� (2002.8.22 001) 


// CControlPos�� m_awndControls �ɹ������� �����͸� �����Ѵ�.
// �� ������ �������� �� ������Ʈ�� ������ ��� �ִ�.
long CChartItem::CXIBaseDesk::FP_nGetCtrlPos()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->FP_nGetCtrlPos();
}


// ������Ʈ�� �������� ��Ų��.
void CChartItem::CXIBaseDesk::FP_vResizeCtrl()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	
	pThis->m_pBaseDesk2->FP_vResizeCtrl();
}

// {{ OCX���� DESK�� �޼��� ���� �� �ֵ��� ��û(����) 2003.05.19
void CChartItem::CXIBaseDesk::SetMapMessage(long lType, LPCSTR lpData)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	pThis->m_pBaseDesk2->SetMapMessage(lType, lpData);
}
// }}



int CChartItem::CXIBaseDesk::FP_nGetDataInt(int nCommand)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

		return 1;
//	return pThis->m_pBaseDesk2->FP_nGetDataInt(nCommand);
}

void CChartItem::SetSaveAllCount(int nAllCnt)
{
	m_nSaveAllCount = nAllCnt;
}



BOOL CChartItem::InvokeAddInPattern(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if(!m_bAddInLoadPattern)
	{
		LoadAddIn(p_szAddInDllName);
		m_bAddInLoadPattern = TRUE;
	}
	if(p_lData == 1)
		m_bDrawAutoTrendLine = TRUE;
	else if(p_lData == 2)
		m_bDrawAutoPattern = TRUE;
	else if(p_lData == 3)
		m_bDrawCandlePattern = TRUE;
	else if(p_lData == 4)
		m_bDrawAutoTrendLine = FALSE;
	else if(p_lData == 5)
		m_bDrawAutoPattern = FALSE;
	else if(p_lData == 6)
		m_bDrawCandlePattern = FALSE;
	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

BOOL CChartItem::InvokeAddInPeriod(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if(!m_bAddInLoadPeriod)
	{
		LoadAddIn(p_szAddInDllName);
		m_bAddInLoadPeriod = TRUE;
	}
	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

BOOL CChartItem::InvokeAddInSTIndex(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if(!m_bAddInLoadSTIndex)
	{
		LoadAddIn(p_szAddInDllName);
		InvokeAddIn(p_szAddInDllName, 3, (long)this);
		m_bAddInLoadSTIndex = TRUE;
	}

	if(p_nCommandType == 200)//�ʱ�ȭ
	{
		m_strArrayNameBySystemTradingTab.RemoveAll();
	}

	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

//BOOL CChartItem::InvokeAddInNews(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
//{
//	if(!m_bAddInLoadNews)
//	{
//		LoadAddIn(p_szAddInDllName);
//		m_bAddInLoadNews = TRUE;
//	}
//	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
//}

int CChartItem::GetMarketType(CString strCode)
{
	IMasterDataManagerLast* pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	//CString strType = pDataManager->GetCodeInfo("", 2, strCode);
	BYTE bType = pDataManager->GetMarketTypeFromCode(strCode);
	BOOL bRet = FALSE;
	int nLength = strCode.GetLength();

	// �����ɼ�
	// if(strType == "4" || strType == "5")
	if(nLength == FUTOPT_LENGTH)
	{
		return CMasterTypeInfo::FUTOPT_CHART;
	}
	// ���� (KOSPI 200 ����, KOSPI 100 ����, �峻����, KRX)
	// else if(strType == "B")
	// ���� (�ڽ��� ����)
	// else if(strType == "C")
	else if(nLength == UPJONG_LENGTH)
	{
		return CMasterTypeInfo::UPJONG_CHART;
	}

	// �ֽ� (��������, �ڽ��� ����)
	// if(strType == "1" || strType == "2")
	if(bType==CMasterTypeInfo::STOCK_CHART) return CMasterTypeInfo::STOCK_CHART;
	//if(strType == "1" || strType == "3")	//KOSPI, KOSDAQ
	//{
	//	return CMasterTypeInfo::STOCK_CHART;
	//}
//@��������
//	// �ֽĿ���Ʈ ELW
//	// else if(strType == "N")
//	else if(strType == "68")	//ELW
//	{
//		return ELW_CHART;
//	}
//@��������
	return CMasterTypeInfo::STOCK_CHART;
}

//@��������
//// iKeyType�� �ش��ϴ� ����Ű�� �ֱٰ��� ��ȯ
//CString CChartItem::GetLastKey(int iKeyType)
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
CString CChartItem::GetLastHistoryCode(int nMarketType)
{
	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
	CString strCode;
	if(pDataManager)
	{
		// �ֽ�
		if(nMarketType == CMasterTypeInfo::STOCK_CHART)
		{
			strCode = pDataManager->GetLastCode("01");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, CODE_STOCK_LENGTH);
			}
		}
		// ����
		else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
		{
			strCode = pDataManager->GetLastCode("02");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, UPJONG_LENGTH);
			}
		}
		// ����+�ɼ�+��������+�ֽĿɼ�
		else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
		{
			strCode = pDataManager->GetLastCode("03");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, FUTOPT_LENGTH);
			}
		}
	}

	return strCode;
}

CString CChartItem::GetQueryOpt()
{
	int nQueryOpt = ((CChartMng*)GetParent())->GetpExtInputBarDlg()->SendMessage(UMSG_GETQUERYOPT);
	CString strQueryOpt;
	strQueryOpt.Format("%d", nQueryOpt);
	return strQueryOpt;
}

//@�����߰�
/////////////////////////////////////////////////////////////////////////////

// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
void CChartItem::AddChartItem(CChartItemData* pChartItemData, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, char cType, LPCTSTR lpszTickMinDayNumber, long lCount, long lChartNumericValueType, BOOL bBuiltChart)
{
	//@Solomon100226 �ڽ������հ� �ڽ��������� "����"���� �����ϰ� �ͼ� �����ϱ� ����.

	CString szItemName = lpszItemName;
	if(strcmp(lpszItemCode, "001")==0)
		szItemName = "�ڽ�������";
	else if(strcmp(lpszItemCode, "201")==0)
		szItemName = "�ڽ�������";

	CString strChartGubun = pChartItemData->GetTrCode();
	m_strSelectedRQ = CDRChartOcx::AddChartItem(lpszItemCode, szItemName, bBuiltChart);

	ST_SELCHART* pSelChart = NULL;
	if(bBuiltChart)
	{
		pSelChart = new ST_SELCHART;
	}
	else 
	{
		if(!m_mapChartItem.Lookup(m_strSelectedRQ, (LPVOID&)pSelChart))
		{
			if(m_strSelectedRQ != _T("DEFAULT"))
				return;
		}
		pSelChart = new ST_SELCHART;
	}

	pSelChart->m_strChartGubun = strChartGubun;
	pSelChart->m_strItemCode = lpszItemCode;
	pSelChart->m_strItemName = szItemName;
	pSelChart->m_cType = cType;
	pSelChart->m_strTickMinDayNumber = lpszTickMinDayNumber;
	pSelChart->m_lCount = lCount;
	pSelChart->m_nChartNumericValueType = lChartNumericValueType;
	pSelChart->m_strKey = pChartItemData->GetNextKey();

	m_mapChartItem.SetAt(m_strSelectedRQ, pSelChart);

	m_lpSelChart = pSelChart;

}

// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
BOOL CChartItem::GetSelChart(LPCTSTR lpszRQ, ST_SELCHART*& pSelChart)
{
	BOOL bResult = FALSE;

	if(!lpszRQ)
	{
		if(m_strSelectedRQ.IsEmpty())
			m_strSelectedRQ = _T("DEFAULT");

		bResult = m_mapChartItem.Lookup(m_strSelectedRQ, (LPVOID&)pSelChart);
	}
	else
	{
		bResult = m_mapChartItem.Lookup(lpszRQ, (LPVOID&)pSelChart);
		if(bResult)
			m_strSelectedRQ = lpszRQ;
	}

	return bResult;
}

int CChartItem::GetRQCount()
{
	return m_mapChartItem.GetCount();
}

// �����ڵ�� RQ�� ã�� ���� : �������� - ojtaso (20070705)
BOOL CChartItem::FindSelectChartByItemCode(LPCTSTR lpszItemCode, BOOL bSelect/* = TRUE*/)
{
	CString strRQ;
	ST_SELCHART* lpSelChart;

	POSITION pos = m_mapChartItem.GetStartPosition();
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strRQ, (LPVOID&)lpSelChart);

		if(!lpSelChart->m_strItemCode.CompareNoCase(lpszItemCode))
		{
			if(bSelect)
			{
				m_strSelectedRQ = strRQ;
				CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);

				if(GetSelChart(m_strSelectedRQ, lpSelChart))
				{
					OnSelectChartItem(lpSelChart);
					((CStdDialog*)m_pStdDialog)->OnSelectChartItem(lpSelChart);
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
void CChartItem::EnableRequestMoreButton(BOOL bEnable)
{
	((CStdDialog*)m_pStdDialog)->EnableRequestMoreButton(bEnable);
}

#ifdef _WRITELOG
// Log - ojtaso (20080107)
void CChartItem::WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
{
	((CStdDialog*)m_pStdDialog)->WriteToStringLog(p_szSection, p_szData, p_nDataLen);
}
#endif


void CChartItem::SetCompareChartType(char cType)
{
	if(m_chCompareChartType  != cType)
	{
		switch(cType)
		{			
			case DIVIDED_COMPARE_CHART:
				{		
					PartitionAllGraph();
//					if(m_chCompareChartType == NUJUK_OVERLAPP_COMPARE_CHART)
//					{
//						CString strList = GetStrChartList();
//						strList.Replace(JIPYO_STRTITLE_C, JIPYO_STRTITLE_AB);
//						SetStrChartList(strList);
//						//}}
//					}
				}
				break;
			case OVERLAPP_COMPARE_CHART:
				{	
					JoinAllGraph(FALSE);
//					if(m_chCompareChartType == NUJUK_OVERLAPP_COMPARE_CHART)
//					{
//						//{{2006.12.14 by LYH ������� ��û�� ���� ����
//						//ChangeAllFunctionName(JIPYO_STRTITLE_C, JIPYO_STRTITLE_AB, true);
//						CString strList = GetStrChartList();
//						strList.Replace(JIPYO_STRTITLE_C, JIPYO_STRTITLE_AB);
//						SetStrChartList(strList);
//						//}}
//					}
				}
				break;	
			case NUJUK_OVERLAPP_COMPARE_CHARTA:
			case NUJUK_OVERLAPP_COMPARE_CHARTB:
			case NUJUK_OVERLAPP_COMPARE_CHARTC:
				{
					JoinAllGraph(TRUE);
//					if( (m_chCompareChartType == DIVIDED_COMPARE_CHART) ||
//						(m_chCompareChartType == OVERLAPP_COMPARE_CHART) 
//					)
//					{
//						//{{2006.12.14 by LYH ������� ��û�� ���� ����
//						//ChangeAllFunctionName(JIPYO_STRTITLE_AB, JIPYO_STRTITLE_C, true);
//						CString strList = GetStrChartList();
//						strList.Replace(JIPYO_STRTITLE_AB, JIPYO_STRTITLE_C);
//						SetStrChartList(strList);
//						//}}
//					}
				}
				break;
			case SUIK_COMPARE_CHART:
				{
					//m_chkShowSuikChart.SetCheck(1);
				}
				break;
			case TOT_NUJUK_COMPARE_CHART:
				{
					//m_chkShowTotNujukChart.SetCheck(1);
				}
				break;	
		}
		

		m_chCompareChartType = cType;
	}

	long lCntOfGraph = m_arrayChartItemData.GetSize();
	CChartItemData *pData = NULL;	
	for(long lPos=0;lPos<lCntOfGraph;lPos++)
	{		
		pData = NULL;
		pData = m_arrayChartItemData.GetAt(lPos);		
		if(pData)
		{
			pData->SetCompareChartType(cType);
		}
	}	
}

BOOL CChartItem::RemoveGraphTitle(int nRow, int nCol, CString strCode, CString strJMName, BOOL bIsOnlyNoTRData)
{
//	if( m_nCntCompare > 0)
//		m_nCntCompare--;
//	
//	//=========================================================================================
//	// 2005. 08. 09
//	// �ϳ��� �����Ҷ� Real ������ �Ѵ�.
//	// ���� �ϳ���.CChartItemData �ϳ�
//	//=========================================================================================
//	CChartItemData *pChartItemData = FindChartItemDataForUnAdvise_By_Code(strCode);
//	if(pChartItemData)
//	{
//		pChartItemData->RemoveRealItem();
//	}
//	//=========================================================================================
//
//	CString strName;
//	for(int i = 0 ;  i < m_arrayAddCode.GetSize(); i++)
//	{
//		strName = m_arrayAddCode.GetAt(i);
//		if(strName.CompareNoCase(strJMName) == 0)
//		{
//			m_arrayAddCode.RemoveAt(i);
//			break;
//		}
//	}
//
//	//��Ŷ ���� 
//	CKBChart::RemovePacket(strJMName);

	return CDRChartOcx::RemoveGraph_Title(nRow, nCol, strJMName, bIsOnlyNoTRData, TRUE);
}

void CChartItem::SetChartRequestType(char chType)
{
	m_chTypeForRequest = chType;
}

void CChartItem::SaveRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle, int cType)
{
	POSITION pos = m_mapChartItem.GetStartPosition();
	CString strKey;
	ST_SELCHART* lpSelChart = NULL;

	CString strValue, strTemp;
	strValue.Format(_T("%d;%s"), m_mapChartItem.GetCount(), m_strSelectedRQ);

	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)lpSelChart);
		strTemp.Format(_T(";%s;%s;%s;%s;%c;%s;%ld;%d;%s"), strKey,
			lpSelChart->m_strChartGubun,
			lpSelChart->m_strItemCode,
			lpSelChart->m_strItemName,
			cType,
			lpSelChart->m_strTickMinDayNumber,
			lpSelChart->m_lCount,
			lpSelChart->m_nChartNumericValueType,
			lpSelChart->m_strKey);

		strValue += strTemp;
	}

	strTemp.Format(_T("%d"), strValue.GetLength() + m_strSelectedRQ.GetLength() + 1);

	::WritePrivateProfileString(lpszAppName,	"RQSIZE"	,strTemp,lpszTitle);
	::WritePrivateProfileString(lpszAppName,	"RQ"		,strValue,lpszTitle);
}

LPCTSTR g_strColor[] =
{
		"255,   3, 255"		,
		"  0, 113,  46"		,
		"  0,   0, 160"		,	//"3,234,255"		,
		"255, 128,   0"		,
		"255,   0,   0"		,
		"128,  64,  64"		,
		"102, 122, 170"		,	//"162, 174, 203"		,
		"255, 128, 128"		,	//"255,228,128"	,
		"124, 122, 125"		,
		"149, 136,  43"		,	//"199, 183,  61"		,
		" 25, 228, 229"		,
		"238, 187,  88"		,
		"246, 142,  86"		,
		"  0, 174, 239"		,
		"  0, 114, 188"		,
		"245, 152, 157"		,
		"146,  39, 143"		,
		"  6, 253,   0"		,
		"126, 126, 126"		,
		"191,  96,  62"		
};

//-->@080611-001 ������϶��� ó��
void CChartItem::fnChartFluct(int nType)
{
	CStdDialog* pSTDialog = (CStdDialog*)GetParent()->GetParent();

	if(pSTDialog->m_nScreenType!=CStdDialog::Screen_fluctuation)
	{
		LPCTSTR lpszRQ = (LPCSTR)m_strSelectedRQ;

		CString szTmp = g_strColor[g_nColor++];
		g_nColor = g_nColor % MAX_g_strColor;
		CString szColor;
		//-->@.080811-001 ������� ����.
		//		szColor = szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp;
		szColor = szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp;
		szColor += ";0;1;";
		szColor += szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp;
		szColor += ";0;1";
		//<--

//#ifdef __MULTI_LANG__
		ChangeGraphPropertyAtOnce(lpszRQ, "_Price_", "", -1, -1, -1, szColor, -1, -1, -1);
//#else
//		ChangeGraphPropertyAtOnce(lpszRQ, "����", "", -1, -1, -1, szColor, -1, -1, -1);
//#endif

		int nShowType = 1;
		//ShowGraphDataDrawingData(lpszRQ, "������Ʈ", 0, nShowType);
		ChangeGraphDrawingData("������Ʈ", FALSE, FALSE, FALSE, FALSE, 1);
		return;
	}

	if(pSTDialog)
	{
		//ShowGraphDataDrawingData
		LPCTSTR lpszRQ = (LPCSTR)m_strSelectedRQ;
		if(pSTDialog->m_pInputBarDlg->m_pSubItem)
		{
			//CDefineData::INDICATEDATA_PRICECHART
			//CDefineData::INDICATETITLE_PRICE
			//CDefineData::INDICATETITLE_PRICE - ����
			//CDefineData::INDICATEDATA_PRICECHART - ������Ʈ
			CString szTmp;
			szTmp.Format("%03d,%03d,%03d",
				GetRValue(pSTDialog->m_pInputBarDlg->m_pSubItem->m_clr),
				GetGValue(pSTDialog->m_pInputBarDlg->m_pSubItem->m_clr),
				GetBValue(pSTDialog->m_pInputBarDlg->m_pSubItem->m_clr));
			
//#ifdef __MULTI_LANG__
			LPCTSTR lpszTargetTitle="_Price_";
//#else
//			LPCTSTR lpszTargetTitle="����";
//#endif
//			LPCTSTR lpszTargetTitle="����";
			LPCTSTR lpszNewTitle="";
			short nGraphType = -1;
			short nGraphStyle = -1;
			short nDrawStyle = pSTDialog->m_pInputBarDlg->m_pSubItem->m_nDrawStyle;

			CString szColor;
			szColor = szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp + ";" + szTmp;
			//LPCTSTR szColor = "255,255,0;255,255,0;255,255,0";
			short nThickness1=pSTDialog->m_pInputBarDlg->m_pSubItem->m_nThickness+1;//index�̹Ƿ� +1
			short nThickness2=-1;
			short nThickness3=-1;
			ChangeGraphPropertyAtOnce(lpszRQ, lpszTargetTitle, lpszNewTitle, nGraphType, nGraphStyle, nDrawStyle, szColor, nThickness1, nThickness2, nThickness3);
		}
		
		int nShowType = 0;
		if(pSTDialog->m_stDlgCompData.m_bCurDisp) nShowType = 3;
		//ShowGraphDataDrawingData(lpszRQ, "������Ʈ", 0, nShowType);
		ChangeGraphDrawingData("������Ʈ", FALSE, FALSE, FALSE, FALSE, 1);

//-->@.080616-001 : ��ü���� ����.
		SetBWholeView(TRUE);
//<--
		//pSTDialog->m_stDlgCompData.m_bCurDisp
	}
}
//<--

