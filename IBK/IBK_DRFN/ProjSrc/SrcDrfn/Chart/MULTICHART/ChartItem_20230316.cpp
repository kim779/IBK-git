// ChartItem.cpp: implementation of the CChartItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartItem.h"
//#include "ChartDef.h"
#include "./define/ChartMsgDef.h"
#include "ChartMng.h"
#include "../COMMONTR/ChartRealConverter.h"
#include "./define/MessageDefine.h"
#include "../../inc/RealUpdate.h"
#include "../chart_common/IRealReceiver.h"

#include "StdDialog.h"

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

IMPLEMENT_DYNAMIC(CChartItem, CDRChartOcx)

extern CString SetComma(CString strNumber, BOOL bShowSign=FALSE);

#define DEFAULT_CHARTDATA_COUNT_INT		400
#define DEFAULT_CHARTDATA_COUNT_CHAR	"400"

//{{JS.Kim_20100920. ����Ÿ �Ǽ� �ʰ��Ͽ� �ڸ��� ������ �ּ��� TR ��ȸ�� �Ǽ� ��ŭ�� �������ֵ��� ����
#define _REMOVE_PACKET_COUNT	500		//caution. OCX�� PacketListImplemetation.cpp �� define�Ǿ� �ִ� ���� �׻� ������ ������ ��!!!
#define _HOLDING_PACKET_COUNT	3000	//caution. �Ʒ��� SetAll ���� NHPS ���� ��ġ���Ѿ� ��..
//}}
#define MAX_RQ 10

#define WMU_GET_LAST_KEY			WM_USER + 2237	// AupAgent���� MainFrame�� ������ ��ȸ�� Key�� ��û�Ѵ�.
LPCTSTR szSetAll[] = 
{
	"CM=Name,DRChart0",
	"AN=DRChart0",
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
	"PDC=70",
	"BAC=10",			// �ִ� ����
	"GDD=0",
//	"LM=5",	// left ����
//	"RM=0",	// right ����
//	"TM=5",	// top ����
//	"BM=0",	// bottom ����
	"AGM=1",												// ���ؽ�Ʈ �޴� 
	"EFN=",
	"IFN=",
	"SY=",
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
	"RDB=0",
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
	"BST=1",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
//	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=3000",	//"NHPS=0",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=4",//"NIO=6",//"NIO=7",
	"GSL=�ð�-�׸���4;�ð�-��Ʈ2;ȣ��-��Ʈ2;"
	"PGCNTTR=1"		// JSR 20060221 ADD : From Doori
};

LPCTSTR szSetAllNoToolbar[] = 
{
	"CM=Name,SyChart0",
	"AN=SyChart0",
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
	"PDC=70",
	"BAC=10",			// �ִ� ����
	"GDD=0",
	"LM=5",	// left ����
	"RM=0",	// right ����
	"TM=5",	// top ����
	"BM=0",	// bottom ����
	"AGM=1",												// ���ؽ�Ʈ �޴� 
	"EFN=",
	"IFN=",
	"SY=",
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
	"BST=1",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=3000",	//"NHPS=0",
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
#define COLOR_STEADY	RGB(57,57,57)//CLR_FONT_DEFAULT
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UINT THREAD_SEND2SERVER(LPVOID lpVoid)
{
	CChartMng* lpChartMng = (CChartMng*)lpVoid;
	if (lpChartMng)
		lpChartMng->RequestChartData();

	return 0;
}


CChartItem::CChartItem()
{
	m_chSign = '3';
	m_bOnAdvise = FALSE;
	m_bInitalChart = FALSE;
	m_bNext = FALSE;
	m_lCntForRequest = 0;
	m_chType = NULL; /*DAY_DATA_TYPE*/
	//m_chTypeForRequest = DAY_DATA_CHART;
	SetChartRequestType(DAY_DATA_CHART);
	m_strTickMinDayForRequest = "001";	// 2Byte Tick & Min & Day Number
	m_strTickMinDay = "001";
	m_nChartNumericValueType = 0;
	
	//2005. 05. 24 add by sy, nam ==================================
	m_strTick = _T("001");
	m_strSec = _T("005");
	m_strMin = _T("001");
	m_strDay = _T("001");
	//==============================================================
	m_lPtrST = NULL;	// data pointer Of System trading is added by gm0604. 2005.7.21

	m_bRevisedPrice = FALSE;
	//m_bRateValue = FALSE;
	m_pStdDialog = NULL;
	ResetData();
	//m_lColIndex = -1;
	//m_lRowIndex = -1;
	SetRowNCol(-1, -1);
	m_nSmallChartMode = 0;
	m_bLoadChartInfo = FALSE;
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

	m_bReceivedDataChart = FALSE;

	Init_ChartCfgInfo();
	

	m_bReplaychartMode = FALSE;

	m_pDCForReplay = NULL;

	m_strOpen = "";
	m_strHigh = "";
	m_strLow = "";

	m_bAddInLoadPattern = FALSE;
	m_bAddInLoadPeriod = FALSE;
	m_bAddInLoadSTIndex = FALSE;
	m_bAddInLoadNews = FALSE;

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
	m_bNowInitialState = FALSE;

	m_pIRealReceiver = NULL;

	m_bSetViewCountFlag = FALSE; //20090922 SJ_KIM ViewCount

	m_nQueryOpt = 0;

	m_bReqCntIsMore = TRUE;

	m_bAfterReceiveData = FALSE;

	m_bMonthGapRev = FALSE;
}

CChartItem::~CChartItem()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::~CChartItem::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	//��ȣ ���� ================================================
	//2005. 08. 24  nam
//	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
//		CloseShareManager();
	//==========================================================

	if(m_pDCForReplay)
	{
		ReleaseDC(m_pDCForReplay);
		m_pDCForReplay = NULL;
	}

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
	WriteToStringLog("CChartItem::~CChartItem::End", _T("��Ƽ��Ʈ ����"), 13);
#endif

	if(m_pIRealReceiver)
	{
		m_pIRealReceiver->Release();
		m_pIRealReceiver = NULL;
	}
}

//BEGIN_EVENTSINK_MAP(CChartItem, CDRChartOcx)
//    //{{AFX_EVENTSINK_MAP(CTOcx)
//	ON_EVENT(CChartItem, 1000, 1 /* OnLButtonDown */, OnOnLButtonDownTocxctrl1, VTS_I2 VTS_I2)
//	//}}AFX_EVENTSINK_MAP
//END_EVENTSINK_MAP()

#include "Atlconv.h"

long CLSIDFromProgID_GZone( const char *p_szProgID, LPCLSID p_pClsID)
{
	USES_CONVERSION;
	LPCOLESTR szOleProgID = T2COLE( p_szProgID);
	HRESULT hResult = ::CLSIDFromProgID( szOleProgID, p_pClsID);
	if( hResult == S_OK) return 0;
	else if( hResult == CO_E_CLASSSTRING) return -1;
	else if( hResult == REGDB_E_WRITEREGDB) return -2;
	return -3;
}

BOOL CChartItem::Create(CWnd *pWnd,LPRECT lpRect,int nID,long lRow,long lCol, LPCTSTR lpImagePath, BOOL bUseInnerControl)
{
	m_pStdDialog = NULL;

	//================================================================================================================================
	BOOL lResult = CDRChartOcx::Create(NULL,WS_VISIBLE|WS_CHILD,*lpRect,pWnd,nID);
	//================================================================================================================================

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartItem::Create: lResult(%d)\n", __LINE__, lResult);
// 		OutputDebugString(szDebug);
// 	}	

	if(!lResult)
		return FALSE;
	
	m_pStdDialog = ((CStdDialog*)GetParent()->GetParent());

	if(((CStdDialog*)m_pStdDialog)->m_nChartType >= MINI_MULTI_CHART)
		m_lCntForRequest = MINICNTATONCE;
	else
		m_lCntForRequest = DEFAULTCNTATONCE;

#ifdef _DEBUG
	if(m_pStdDialog == NULL)
		AfxMessageBox("CChartItem::Create...pStdDialog is NULL");
#endif

	m_nID = nID;

	SetRowNCol(lRow, lCol);
	CString strImagePath = m_strImagePath = lpImagePath;
	CString strImageUp;
	CString strImageDown;
	m_bUseInnerControl = bUseInnerControl;

	if(bUseInnerControl)
	{
		m_btMaxmize.Create("",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_CLIPSIBLINGS,CRect(0,0,0,0),pWnd,(m_nID+1));
		m_btInit.Create("",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_CLIPSIBLINGS,CRect(0,0,0,0),pWnd,(m_nID+2));
		m_btMaxmize.SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		m_btInit.SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

		m_btMaxmize.FP_vSetImage(&(((CStdDialog*)m_pStdDialog)->m_ImgList_W15H15), 0, 1);	
		m_btMaxmize.FP_vSetToolTipText("�ִ�ȭ/����");

		if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6301 || ((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6401)
		{
			m_btMaxmize.ShowWindow(SW_HIDE);
			m_btInit.ShowWindow(SW_HIDE);
		}
		else
		{
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
//	CTime t = CTime::GetCurrentTime();
//	CString strDate = t.Format("%Y%m%d");
	if(m_pStdDialog)
	{
		CString strDate = ((CStdDialog*)m_pStdDialog)->GetBizDate();
		SetEndDateRequested(strDate);
		SetEndDate(strDate);
	}
//}}

	m_bNext = FALSE;

	m_lAllCnt = 0;
	m_lPtrST = NULL;// gm0604 added 2005.9.6
	m_strKorName.Empty();
//@��������
//	if(m_Check.GetSafeHwnd())
//	{
//		m_Check.SetCheck(0);
//	}
//@��������
	//==========================================================
	for(int i = 0; i < m_arrayMarketTRData.GetSize();i++)
	{
		CChartItemData* pChartItemData = NULL;
		pChartItemData = m_arrayMarketTRData.GetAt(i);
		if(pChartItemData)
			pChartItemData->SetBNextMarketData(FALSE);
	}
	//==========================================================

	// �����ְ� Ŭ���� ��ü�� ������Ű�� ���� �߰� by LYH 2007.01.11
	m_nTotCountForRevised = 0;
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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

	SetOutsideCommonInterface(((CChartMng*)GetParent())->m_lMltchrtDlgParentClsPointer); 
	SetAllProperties(bNoToolBar);	

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
	m_listAddIn.AddTail("ChartGuideLineAddin.dll:LOWHIGH_MARKER");
	SendMessageToInvoke(INVOKEMSG_INIT,NULL,0);
	
	//>> ������ - vntsorl(20091008)
	LoadAddIn("ChartCommonAddIn.dll:GAP_REVISION");
	//<< ������ - vntsorl(20091008)
}


BOOL CChartItem::SetAllProperties(BOOL bNoToolbar /*=FALSE*/)
{
	CStringList strlistSetAll;
	if(bNoToolbar)
	{
//@Solomon-100226 alzioyes.
// szSetAllNoToolbar������ global�����̹Ƿ� ���� �ٲٸ� �� ���� ���� ȣ�鿡�� ������ ����.
// ���� ����. ocx���� ���� Ű�϶��� ���������� �ϹǷ� �Ʒ�ó�� ������.
//>> [QC No]090806HMC092
// 		if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6401)
// 		{
// 			CString strBSG("BSG=1");
// 			for(int i=0;i<sizeof(szSetAllNoToolbar)/sizeof(LPCTSTR);i++)
// 			{
// 				if(strBSG == szSetAllNoToolbar[i])
// 					szSetAllNoToolbar[i] = "BSG=0";
// 			}
// 		}
//<< [QC No]090806HMC092
		for(int i=0;i<sizeof(szSetAllNoToolbar)/sizeof(LPCTSTR);i++)
			strlistSetAll.AddTail(szSetAllNoToolbar[i]);
	}
	else
	{
//>> [QC No]090806HMC092
// 		if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6401)
// 		{
// 			CString strBSG("BSG=1");
// 			for(int i=0;i<sizeof(szSetAll)/sizeof(LPCTSTR);i++)
// 			{
// 				if(strBSG == szSetAll[i])
// 					szSetAll[i] = "BSG=0";
// 			}
// 		}
//<< [QC No]090806HMC092
		for(int i=0;i<sizeof(szSetAll)/sizeof(LPCTSTR);i++)
			strlistSetAll.AddTail(szSetAll[i]);	
	}
	if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6401)
		strlistSetAll.AddTail("BSG=0");

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
	case CMasterTypeInfo::FOREIGN_CHART:
		strMapName = "MAPNAME=M650000";		break;
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
//>> [QC No]090731HMC065
	ResetChart();		// Ư����Ʈ �ݱ� �� ������ȸ�� �� ��Ÿ���� �������
//<< [QC No]090731HMC065
	Init_ChartCfgInfo();
	ResetData();
	DestoryItemData();
	GetParent()->Invalidate();	
	CDRChartOcx::ShowInnerToolBar(FALSE);
	//CDRChartOcx::EmptyChart(TRUE, FALSE);	
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
	{
		((CChartMng*)GetParent())->SendInitChartData();
	}

	//@�����߰�
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	//20110502 �̹��� >>
// 	ST_CHARTITEM_REALDATA* pChartItemRealData;
// 	if(m_mapChartItemRealData.Lookup(m_strSelectedItemCode, (LPVOID&)pChartItemRealData))
// 	{
// 		if(pChartItemRealData->m_nRefCount > 1)
// 			pChartItemRealData->m_nRefCount--;
// 		else
// 		{
// 			delete pChartItemRealData;
// 			m_mapChartItemRealData.RemoveKey(m_strSelectedItemCode);
// 		}
// 	}
	{		
		//��� RQ���� ����
		POSITION pos = m_mapChartItem.GetStartPosition();
		ST_SELCHART* pSelChart = NULL;
		CString strKey;
		while(pos)
		{
			m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)pSelChart);
			
			if(strKey != "DEFAULT" && !CDRChartOcx::FindRQ(strKey))
			{			
				delete pSelChart;
				m_mapChartItem.RemoveKey(strKey);
			}
			
		}

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
	}
	//20110502 �̹��� <<
	//@�����߰�
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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
//@��������	CRect rectCheck(0,0,0,0);
	
	if(!rect.IsRectNull())
	{	
		rectMaxmize.left  = rect.right-WIDTHOFTITLEBUTTON-POSXOFMAXMIZE;
		rectMaxmize.top   = rect.top + POSYOFMAXMIZE + 1;
		rectMaxmize.right = rect.right-POSXOFMAXMIZE;
		rectMaxmize.bottom = rect.top + POSYOFMAXMIZE + HEIGHTOFTITLEBUTTON + 1;

	    rectInit.left =  rect.right-WIDTHOFTITLEBUTTON-POSXOFINIT;
		rectInit.top =	rect.top + POSYOFINIT + 1;
		rectInit.right = rect.right-POSXOFINIT;
		rectInit.bottom = rect.top + POSYOFINIT + HEIGHTOFTITLEBUTTON + 1;
		
//@��������
//		if(strTRCode==QUERY_STRJONGMOK)
//		{
//			rectSignal.left = rect.left;
////			rectSignal.top = rect.top + rect.Height()/2 - SIZEOFSIGNAL/2;
//			rectSignal.top = rect.top + HEIGHTOFCAPTION/2 - SIZEOFSIGNAL/2;// changed by gm0604 2005.10.10
//			rectSignal.right = rectSignal.left;// + SIZEOFSIGNAL;
//			rectSignal.bottom = rectSignal.top + SIZEOFSIGNAL;
//
//			rectCheck.left = rectSignal.right + 1;
////			rectCheck.top = rect.top + rect.Height()/2 - SIZEOFCHECK/2;
//			rectCheck.top = rect.top + HEIGHTOFCAPTION/2 - SIZEOFCHECK/2;// changed by gm0604 2005.10.10
//			rectCheck.right = rectCheck.left + SIZEOFCHECK;
//			rectCheck.bottom = rectSignal.top + SIZEOFCHECK;
//		}
//		else
//		{
//			rectSignal.SetRect(0,0,0,0);
//			rectCheck.SetRect(0,0,0,0);
//		}
//@��������
	}
	if(m_btMaxmize.GetSafeHwnd())
	{
		m_btMaxmize.MoveWindow(rectMaxmize);
//		m_btMaxmize.Invalidate();
	}
	if(m_btInit.GetSafeHwnd())
	{
		m_btInit.MoveWindow(rectInit);
//		m_btInit.Invalidate();	
	}
	//if(m_signal.GetSafeHwnd() && m_bUseInnerControl)
	//{
	//	m_signal.MoveWindow(rectSignal);
	//}
//@��������
//	if(m_Check.GetSafeHwnd())
//	{
//		m_Check.MoveWindow(rectCheck);
////		m_Check.Invalidate();
//	}
//@��������
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ�� Title�� Draw�Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void CChartItem::DrawTitle(CRect rectIn,CDC* pDC,CFont *pFt, CFont *pFtNormal, COLORREF crBackIn )
{
	// �������� �ǽð� : �������� - ojtaso (20070523)
	ST_CHARTITEM_REALDATA* pChartItemRealData;
	CString strItemCode = m_strSelectedItemCode;
	((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strItemCode);
	if (!m_mapChartItemRealData.Lookup(strItemCode, (LPVOID&)pChartItemRealData))
		return;
	
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

	CString strTitleName;
	CString strTitlePrice;
	CString strTitleChange;
	CString strTitleRate;
	CString strTitleVolume;
	CString strTitleOpenHighLow;
	CString strTitleType;

	CSize szTitlePos;
	
	// �������� �ǽð� : �������� - ojtaso (20070523)
	if(m_strSelectedItemCode.Compare(DEF_CONTINUE_FUT_CODE)==0)
		m_strKorName = "���ἱ��";
	else
		m_strKorName = pChartItemRealData->m_strItemName;

	CRect rect = rectIn;

	if (m_btInit.GetSafeHwnd() && m_btInit.IsWindowVisible())
		rect.right -= WIDTHOFTITLEBUTTON;

	if (m_btMaxmize.GetSafeHwnd() && m_btMaxmize.IsWindowVisible())
		rect.right -= POSXOFMAXMIZE;

	COLORREF crText = NULL, crTextSise = NULL, crBackSise = NULL;
	CFont *pFtDefault = (CFont *)pDC->SelectObject(pFt);

	LPSTR lpszTitlePos = new char[10];
	::memset(lpszTitlePos, 0x00, 10);

	if (m_bUseInnerControl)
	{
		int nID = ((CStdDialog*)m_pStdDialog)->m_nScreenID;
		if (!(nID == nScreenID_6301 || nID == nScreenID_6401))
		{
			long lCols = ((CChartMng*)GetParent())->GetCols();
			long lCntOfChart = ((CChartMng*)GetParent())->GetCntOfChart();
			::sprintf(lpszTitlePos, "[%d / %d]", m_lColIndex + 1 + m_lRowIndex*lCols,lCntOfChart);
			szTitlePos = pDC->GetOutputTextExtent(lpszTitlePos);
			rectTitlePos.SetRect(rect.right-szTitlePos.cx,rect.top+4,rect.right,rect.bottom);
		}
	}
	else
	{
		szTitlePos.cx = 0;
		szTitlePos.cy = 0;
	}

	//@100304-A052 �ð��� �ؽ�Ʈ�� ��������ó��.
	double dwBasePrice = atof((LPCSTR)pChartItemRealData->m_strPrice);
	COLORREF crSHL[3];
	BOOL bShortTitleMode = FALSE;
	if (!m_strKorName.IsEmpty())
	{
		//long lTrCode = atol(GetDataTRCode());
		CString strTRCode = GetDataTRCode();
		CString strSign = "";

		switch (pChartItemRealData->m_chSign)
		{
		case DOWN:			// Down
			strSign = "��";			
			if (pChartItemRealData->m_strChange.GetLength()>0 && pChartItemRealData->m_strChange[0] == '-')
				pChartItemRealData->m_strChange = pChartItemRealData->m_strChange.Mid(1);
			dwBasePrice += atof((LPCSTR)pChartItemRealData->m_strChange);
			break;
		case DOWN_LIMIT:	// DownLimit
			strSign = "��";
			if (pChartItemRealData->m_strChange.GetLength()>0 && pChartItemRealData->m_strChange[0] == '-')
				pChartItemRealData->m_strChange = pChartItemRealData->m_strChange.Mid(1);
			dwBasePrice += atof((LPCSTR)pChartItemRealData->m_strChange);
			break;
		case UP:
			strSign = "��";
			dwBasePrice -= atof((LPCSTR)pChartItemRealData->m_strChange);
			break;
		case UP_LIMIT:
			strSign = "��";
			dwBasePrice -= atof((LPCSTR)pChartItemRealData->m_strChange);
			break;
		}

		//@100304-A052
		{
			int nPoint = pChartItemRealData->m_strPrice.GetLength() - pChartItemRealData->m_strPrice.Find('.') -1 ;
 			_DoGetColor(crSHL[0], pChartItemRealData->m_bIsRealJang, dwBasePrice, atof(pChartItemRealData->m_strOpen), nPoint);
 			_DoGetColor(crSHL[1], pChartItemRealData->m_bIsRealJang, dwBasePrice, atof(pChartItemRealData->m_strHigh), nPoint);
 			_DoGetColor(crSHL[2], pChartItemRealData->m_bIsRealJang, dwBasePrice, atof(pChartItemRealData->m_strLow), nPoint);
		}

		if (strTRCode==QUERY_STRUPJONG||strTRCode==QUERY_STRFUTOPT/* || strTRCode==QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
		{
			strTitleName.Format(" %s ",m_strKorName);
			strTitlePrice.Format(" %s ",::SetComma(pChartItemRealData->m_strPrice));
			if (strTRCode!=QUERY_STRFOJIPYO)
				strTitleChange.Format("%s %s (%s%%) ",strSign, pChartItemRealData->m_strChange, pChartItemRealData->m_strChrate);

			strTitleVolume.Format(" %s ",::SetComma(pChartItemRealData->m_strVolume));

			if (pChartItemRealData->m_strOpen.GetLength()>0)
				strTitleOpenHighLow.Format("��:%s ��:%s ��:%s", ::SetComma(pChartItemRealData->m_strOpen), ::SetComma(pChartItemRealData->m_strHigh), ::SetComma(pChartItemRealData->m_strLow));
		}
		else
		{
			strTitleName.Format(" %s ",m_strKorName);

			//{{ 2007.06.26 by LYH ����ü���� ��� ���簡 �տ� (��) ����
			if (pChartItemRealData->m_bIsRealJang)	//����ü��
			{				
				strTitlePrice.Format(" %s ",::SetComma(pChartItemRealData->m_strPrice));
			}
			else
			{
				strTitlePrice.Format("(��) %s ",::SetComma(pChartItemRealData->m_strPrice));
			}
			//}}

			if (strTRCode!=QUERY_STRFOJIPYO)
				strTitleChange.Format("%s %s (%s%%) ",strSign, ::SetComma(pChartItemRealData->m_strChange,TRUE), pChartItemRealData->m_strChrate);

			strTitleVolume.Format(" %s ",::SetComma(pChartItemRealData->m_strVolume));
			if (pChartItemRealData->m_bIsRealJang && (pChartItemRealData->m_strOpen.GetLength()>0))
				strTitleOpenHighLow.Format("��:%s ��:%s ��:%s", ::SetComma(pChartItemRealData->m_strOpen), ::SetComma(pChartItemRealData->m_strHigh), ::SetComma(pChartItemRealData->m_strLow));
		}

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

	if(chType!=NULL)
	{
		// ƽ(0)/��(1)/��(2)/��(3)/��(4)
		switch(chType)
		{
		case TICK_DATA_CHART:
			//strTitleType.Format("[%sƽ]",(LPTSTR)(LPCTSTR)m_strTick);
			strTitleType.Format("[%dƽ]",atoi((LPCTSTR)m_strTick));
			break;
		case SEC_DATA_CHART:
			strTitleType.Format("[%d��]",atoi((LPCTSTR)m_strSec));
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
//		case SEC_DATA_CHART:
//			strTitleType.Format("[��]");
//			break;
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
			pDC->SetBkMode(TRANSPARENT);							/* JSR-20051128 ADD */
			rectInflate = rectTitleName;
			rectInflate.InflateRect(1,3);
			/*			pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);		JSR-20051128 Modify	*/
		}
		else
		{
			rectInflate = rectTitleName;
			rectInflate.InflateRect(1,3);							/* JSR-20051128 Modify */
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
			/*				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);				JSR-20051128 Modify	*/
			pDC->TextOut(rectTitlePrice.left,rectTitlePrice.top,strTitlePrice);		
		}
		if(!rectTitleChange.IsRectEmpty())
		{
			rectInflate = rectTitleChange;
			rectInflate.InflateRect(1,3);
			/*				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);				JSR-20051128 Modify */
			pDC->TextOut(rectTitleChange.left,rectTitleChange.top,strTitleChange);
		}
		pDC->SetTextColor(COLOR_STEADY);
		if(!rectTitleVolume.IsRectEmpty())
		{
			rectInflate = rectTitleVolume;
			rectInflate.InflateRect(1,3);
			/*				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);				JSR-20051128 Modify */
			pDC->TextOut(rectTitleVolume.left,rectTitleVolume.top,strTitleVolume);
		}

		pDC->SetTextColor(crText);
		pFtDefault = (CFont *)pDC->SelectObject(pFtNormal);
		if(!rectTitleOpenHighLow.IsRectEmpty())
		{
			rectInflate = rectTitleOpenHighLow;
			rectInflate.InflateRect(1,3);
			/*				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);				JSR-20051128 Modify */

			//@100304-A052 Ÿ��Ʋ ���ذ��������� ����ǥ��.
			//pDC->TextOut(rectTitleOpenHighLow.left,rectTitleOpenHighLow.top,strTitleOpenHighLow);
			if(pChartItemRealData->m_bIsRealJang)
			{
				pDC->SetTextColor(crSHL[0]);
				strTitleOpenHighLow.Format("��:%s ", ::SetComma(pChartItemRealData->m_strOpen));
				pDC->TextOut(rectTitleOpenHighLow.left,rectTitleOpenHighLow.top,strTitleOpenHighLow);

				CSize tsz = pDC->GetOutputTextExtent(strTitleOpenHighLow);
				rectTitleOpenHighLow.left += tsz.cx;
				pDC->SetTextColor(crSHL[1]);
				strTitleOpenHighLow.Format("��:%s ", ::SetComma(pChartItemRealData->m_strHigh));
				pDC->TextOut(rectTitleOpenHighLow.left,rectTitleOpenHighLow.top,strTitleOpenHighLow);

				tsz = pDC->GetOutputTextExtent(strTitleOpenHighLow);
				rectTitleOpenHighLow.left += tsz.cx;
				pDC->SetTextColor(crSHL[2]);
				strTitleOpenHighLow.Format("��:%s", ::SetComma(pChartItemRealData->m_strLow));
				pDC->TextOut(rectTitleOpenHighLow.left,rectTitleOpenHighLow.top,strTitleOpenHighLow);
			}
			else
			{				
				pDC->TextOut(rectTitleOpenHighLow.left,rectTitleOpenHighLow.top,strTitleOpenHighLow);
			}

		}
		//		}
		if(m_lPtrST)
		{
			pDC->TextOut(rectTitleName.left,rectIn.top+HEIGHTOFCAPTION+4,m_strName);		
		}		
	}

		pDC->SetTextColor(COLOR_STEADY);
	CRect rectInter11;
	CRect rectInter12;
	CRect rectInter2;
	rectInter11.IntersectRect(m_rectTitleJongmok,rectTitleType1);
	rectInter12.IntersectRect(m_rectTitleJongmok,rectTitleType2);
	rectInter2.IntersectRect(m_rectTitleJongmok,rectTitlePos);

	/*	pDC->SetBkMode(TRANSPARENT);					JSR-20051202 STOP */
	/*	pDC->SetTextColor(crText);						JSR-20051202 STOP */

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
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,lpszTitlePos);			
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
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,lpszTitlePos);			
			if(rectTitlePos!=m_rectTitlePos)
			{
				m_rectTitlePosPast = m_rectTitlePos;
				m_rectTitlePos = rectTitlePos;
			}
		}	
	}
	pDC->SelectObject(pFtDefault);
	
	delete [] lpszTitlePos;

//	if( ((CStdDialog*)m_pStdDialog)->m_nChartType != MINI_MULTI_CHART &&
//		((CStdDialog*)m_pStdDialog)->m_nChartType != MINI_SINGLE_CHART )
	{
//@��������		m_Check.Invalidate();
		m_btMaxmize.Invalidate();
		m_btInit.Invalidate();
	}
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestroyWindow::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	CChartItem* pRecv = NULL;
	pRecv = ((CChartMng*)GetParent())->GetpChartCfgWnd(m_chType);
	if(pRecv->GetSafeHwnd() == GetSafeHwnd())
		((CChartMng*)GetParent())->SetpChartCfgWnd(m_chType, NULL);
	



	//���� �������� Real�� ���� �ϹǷ�  ======================================
	//������ �ڵ带 ���� �ڵ�� ������ �ΰ�..	
	//2005. 09. 06
	SetOldChartCode(GetDataCode()); 
//	((CChartMng*)GetParent())->OnChartMngCodeUnAdvise(this, IsAddMode());
		((CChartMng*)GetParent())->ChartItemUnAdvise(this, IsAddMode());
	//========================================================================


	// Call InvokeAddIn Func. for ChartPatternAddin ------------------------------->
	// (��Ʈ���Ϻм� dll���� Modal Dialog�� �����Ǿ� �ִ°�� ���� �����Ű�� ����)
	// wParam - 0:Create, 1:Destroy
	if(m_bAddInLoadPattern)
		InvokeAddInPattern("ChartPatternAddin.dll",NULL,0);
	// <----------------------------------------------------------------------------

#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::ClearData::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	ClearData();
#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::ClearData::End", _T("��Ƽ��Ʈ ����"), 13);
	WriteToStringLog("DestroyWindow::SetDrdsAdvise::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
//	SetDrdsAdvise(FALSE);
#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::SetDrdsAdvise::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	DestoryItemData();	
	m_btMaxmize.DestroyWindow();
	m_btInit.DestroyWindow();
//@��������
//	if(m_Check.GetSafeHwnd())
//	{
//		m_Check.DestroyWindow();
//	}
//@��������

//	if(m_signal.GetSafeHwnd())
//	{
//		m_signal.DestroyWindow();
//	}

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestroyWindow::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	//Han Hwa        m_signal.OffDrdsAdvise();
	return CDRChartOcx::DestroyWindow();
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��ȸ�� Tick/Min/Day ��ġ.������ Set�Ѵ�.
//  
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetTickMinDayNumber(UINT nNumber)
{
	if(MIN_DATA_CHART == m_chTypeForRequest && 0 == nNumber)
		nNumber = 120;

	if(nNumber>360 || nNumber <= 0) 					
	{
		AfxMessageBox("360�̸��� ������ �ٽ� �����Ͽ� �ֽʽÿ�");
		return;
	}
	
	ClearData();
	m_strTickMinDayForRequest.Format("%03d",nNumber);
	
	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART:
			SetTickNumber(m_strTickMinDayForRequest);
			break;
		case SEC_DATA_CHART:
			SetSecNumber(m_strTickMinDayForRequest);
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
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	�ǽð� ������ �ݿ��Ѵ�.
//	lDataKind = MarketType
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetRealPriceData(void *pData, long lDataKind)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SetRealPriceData::Begin", _T("��Ƽ��Ʈ �ü�"), 13);
#endif

	//�ؿ����� ���󶧹��� �߰�
	if (GetDataTRCode() == QUERY_STRFOREIGN)
		lDataKind = UPJONG_REAL;

	BOOL bRet = FALSE;

	CString strSign, strTime;
	CRealData_Common* pRealData_Common = (CRealData_Common*)pData;

	// �ǽð� ���Ź��� �����ڵ� : �������� - ojtaso (20070108)
	m_strRealItemCode = pRealData_Common->m_strCode;

	// �������� ����map�� �̿� : �������� - ojtaso (20070612)
	POSITION pos = m_mapChartItem.GetStartPosition();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	CString strPriceForChart,
			strPriceOpen(pRealData_Common->m_strOpen),
			strPriceHigh(pRealData_Common->m_strHigh),
			strPriceLow(pRealData_Common->m_strLow);
	CString strValue;
	CString strItemCode;
	CString strRealItemCode(m_strRealItemCode);

	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)pSelChart);
		
//>> [QC No]090811HMC104
		strItemCode = pSelChart->m_strItemCode;
// 		if(lDataKind == FUTURE_REAL)
// 		{
// 			CString strYungyulCode;
// 			strRealItemCode = strYungyulCode = strItemCode;
// 			if(((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strYungyulCode))
// 				strItemCode = strYungyulCode;
// 		}
		if(lDataKind == FUTURE_REAL || lDataKind == STOCK_REAL)
		{
			((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strItemCode);
			strRealItemCode = strItemCode;
		}

		if(::lstrcmp(strItemCode, m_strRealItemCode) !=0) continue;

//		if(!::lstrcmp(pSelChart->m_strItemCode, m_strRealItemCode))
		if(!::lstrcmp(strItemCode, m_strRealItemCode))
		{
			ST_CHARTITEM_REALDATA* pChartItemRealData = NULL;
//			BOOL bRealItem = m_mapChartItemRealData.Lookup(m_strRealItemCode, (LPVOID&)pChartItemRealData);
			BOOL bRealItem = m_mapChartItemRealData.Lookup(strRealItemCode, (LPVOID&)pChartItemRealData);
			if (!bRealItem)
				return;

			if( pChartItemRealData->m_szTime.GetLength()>0 &&
				pChartItemRealData->m_szTime.Compare(pRealData_Common->m_strTime)>0)
			{
				//@�ǽð� �������� �������� üũ.
				
// 				if (g_pDebugMng)
// 				{
// 					CString strDebug;
// 					strDebug.Format("*** TimeCheck Code[%s] [CurTime=%s] vs [NewTime=%s]\n", strItemCode, pChartItemRealData->m_szTime, pRealData_Common->m_strTime);
// 					g_pDebugMng->TraceTran('I', "ChartItem", strDebug.GetLength(), (LPBYTE)(LPCSTR)(LPCTSTR)(strDebug));
// 				}	
				return;
			}
			pChartItemRealData->m_szTime = pRealData_Common->m_strTime;

//			{
//				if(pChartItemRealData->m_strVolume.Compare(pRealData_Common->m_strVolume)>0)
//				{
//					CString szDebug;
//					szDebug.Format("[CurVol=%s] vs [NewVol=%s]\n",
//						pChartItemRealData->m_strVolume,
//						pRealData_Common->m_strVolume);
//					OutputDebugString(szDebug);
//				}
//			}
//<< [QC No]090811HMC104

			m_chSign = pRealData_Common->m_strSign.GetAt(0);
			if(bRealItem)
			{
				pChartItemRealData->m_bIsRealJang = pRealData_Common->m_bIsRealJang;
				pChartItemRealData->m_chSign = m_chSign;
			}

			//20110124_alzioyes:FO��Ʈ(FUOPT_JIPYO_CHART) ����ó����ƾ ����.
			if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN
				 || pSelChart->m_strChartGubun == QUERY_STRFOJIPYO )
			{
				m_strPrice = pRealData_Common->m_strPrice;
				strPriceForChart = m_strPrice;
				strPriceForChart.Remove('.');
				strPriceOpen.Remove('.');
				strPriceHigh.Remove('.');
				strPriceLow.Remove('.');				
			}
			else
			{
				m_strPrice.Format("%ld", atol(pRealData_Common->m_strPrice));
				strPriceForChart = m_strPrice;
			}
			m_strPrice.TrimLeft();

			if(bRealItem)
				pChartItemRealData->m_strPrice = m_strPrice;

			//20110124_alzioyes:FO��Ʈ(FUOPT_JIPYO_CHART) ����ó����ƾ ����.
			if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN
				|| pSelChart->m_strChartGubun == QUERY_STRFOJIPYO)
				m_strOpen = pRealData_Common->m_strOpen;
			else
				m_strOpen.Format("%ld", atol(pRealData_Common->m_strOpen));
			m_strOpen.TrimLeft();

			if(bRealItem)
				pChartItemRealData->m_strOpen = m_strOpen;

			//20110124_alzioyes:FO��Ʈ(FUOPT_JIPYO_CHART) ����ó����ƾ ����.
			if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN
				|| pSelChart->m_strChartGubun == QUERY_STRFOJIPYO)
				m_strHigh = pRealData_Common->m_strHigh;
			else
				m_strHigh.Format("%ld", atol(pRealData_Common->m_strHigh));

			m_strHigh.TrimLeft();

			if(bRealItem)
				pChartItemRealData->m_strHigh = m_strHigh;

			//20110124_alzioyes:FO��Ʈ(FUOPT_JIPYO_CHART) ����ó����ƾ ����.
			if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT || pSelChart->m_strChartGubun == QUERY_STRUPJONG || pSelChart->m_strChartGubun == QUERY_STRFOREIGN
				|| pSelChart->m_strChartGubun == QUERY_STRFOJIPYO)
				m_strLow = pRealData_Common->m_strLow;
			else
				m_strLow.Format("%ld", atol(pRealData_Common->m_strLow));

			m_strLow.TrimLeft();

			if(bRealItem)
				pChartItemRealData->m_strLow = m_strLow;

//@Solomon: -->
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

			//-->@Solomon
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
			//KHD	pChartItemRealData->m_strChrate.Remove('-');
			}

			//m_strChrate = pRealData_Common->m_strChrate; 
			//if(bRealItem) pChartItemRealData->m_strChrate = m_strChrate;
			//<--@Solomon


// 			m_strVolume.Format("%ld", atol(pRealData_Common->m_strVolume));
// 			{
// 				m_strVolume.TrimLeft();
// 
// 				if(bRealItem)
// 					pChartItemRealData->m_strVolume = m_strVolume;
// 			}
			m_strVolume = pRealData_Common->m_strVolume;
			if(bRealItem)
				pChartItemRealData->m_strVolume = m_strVolume;

//@��������	
//			//��Ʈ ���� �߰� by LYH 2006.08.13
//			bRet = cReal->GetDataString(DI_TIME, strTime);// �ڷ�����
//			//if(bRet)
//			//{
//			//	if(strTime.GetLength() < 8)
//			//		strTime.Format("%08d", atoi(strTime));
//			//	SetRealData("�ڷ�����",		atof(strTime)		, 0);
//			//	SetRealData("����",			atof(m_strPrice)	, 0);
//			//	SetRealData("�⺻�ŷ���",	atof(m_strVolume)	, 0);
//			//}
//
//			if(bRet)
//@��������	
//@�����߰�	
			strTime = pRealData_Common->m_strTime;

//@solomon	�����ð�(Ȯ�εǸ� �ٽ� ����..)
//			if (GetScreenNo() == SCREENID_FOREIGN)
//				strTime = pRealData_Common->m_strTime2;

//@�����߰�	
			if(bRealItem && pRealData_Common->m_bIsRealJang)
			{
				if(!m_pIRealReceiver)
				{
					m_pIRealReceiver = ( IRealReceiver *)GetAddInCustomInterface( "ChartCommonAddIn.dll:DIRECT_REAL");
					if (!m_pIRealReceiver)
						return;
				}
//@��������	
//				if(strTime.GetLength() < 8)
//					strTime.Format("%08d", atoi(strTime));
//@��������	

				strTime = strTime.Mid(0, 6);
				// ���� ���ؼ� �ǽð� ���� ǥ�� - ojtaso (20090210)
				BOOL bAppend = FALSE;
				//m_pIRealReceiver->SetRealDataRQ(strKey, "�ڷ�����",		atof(strTime), bAppend);
				
				// ���� ���ؼ� �ǽð� ���� ǥ�� - ojtaso (20090210)
				BOOL bAppendPacket = FALSE;

//#ifdef __MULTI_LANG__		//@100308 ������ �ٱ���.
				m_pIRealReceiver->SetRealDataRQ(strKey, "_DateTime_",		atof(strTime), bAppend);

				if((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && (pSelChart->m_nChartNumericValueType == PERCENT_TYPE))
				{
					m_pIRealReceiver->SetRealDataRQ(strKey, "_Close_", atof(pChartItemRealData->m_strChrate)*100., bAppendPacket);

				}
				else if((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && (pSelChart->m_nChartNumericValueType == DOLLAR_TYPE))
				{
					double dStdDollar = atof(((CStdDialog*)m_pStdDialog)->m_strDollar);
					if(dStdDollar == 0.0)
						strPriceForChart = "0";	
					else 
						strPriceForChart.Format("%10.2f", (atof(strPriceForChart)/dStdDollar)*100.0);				
			
					m_pIRealReceiver->SetRealDataRQ(strKey, "_Close_",			atof(strPriceForChart), bAppendPacket);
				}
				else
					m_pIRealReceiver->SetRealDataRQ(strKey, "_Close_",			atof(strPriceForChart), bAppendPacket);
						
				m_pIRealReceiver->SetRealDataRQ(strKey, "_Volume_",	atof(pChartItemRealData->m_strVolume), bAppendPacket);
				
				//@Solomon	091202SM123	-->
				//if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT)
				//>>[A00000685]alzioyes:20110630 �ǽð��� �� �ŷ���� ������ �������� ����.
				//[A00000408]�ŷ���� �ֽ�/���� �϶���100������, �׿ܴ� 1�����.
// 				if(pSelChart->m_strChartGubun == QUERY_STRJONGMOK || pSelChart->m_strChartGubun == QUERY_STRUPJONG)
// 					strValue.Format("%.0f", atof(pRealData_Common->m_strValue)*1000000);
// 				else if(pSelChart->m_strChartGubun == QUERY_STRSTOCKFUTOPT)
// 					strValue.Format("%.0f", atof(pRealData_Common->m_strValue)*100000);	//[A00000408]�ŷ���� 12 -> 18. (6 -> 5��)
// 				else
// 					strValue.Format("%.0f", atof(pRealData_Common->m_strValue)*100000000);	//[A00000408]�ŷ���� 12 -> 18. (6 -> 8��)
				strValue.Format("%.0f", atof(pRealData_Common->m_strValue)*1000000);	//[A00000408]�ŷ���� 12 -> 18. (6 -> 8��)
				//<<[A00000685]

				strValue.TrimLeft();
				
				m_pIRealReceiver->SetRealDataRQ(strKey, "_VolumeAmount_",	atof(strValue), bAppendPacket);
				//@Solomon	091202SM123	<--

				if(pSelChart->m_strChartGubun == QUERY_STRFUTOPT)
				{
					strValue.Format("%.0f", atof(pRealData_Common->m_strOpenyak));
					strValue.TrimLeft();

					m_pIRealReceiver->SetRealDataRQ(strKey, "_SuspenseContract_", atof(strValue), bAppendPacket);
				}

// #else
// 				m_pIRealReceiver->SetRealDataRQ(strKey, "�ڷ�����",		atof(strTime), bAppend);
// 
// 				if((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && (pSelChart->m_nChartNumericValueType == PERCENT_TYPE))
// 					m_pIRealReceiver->SetRealDataRQ(strKey, "����", atof(pChartItemRealData->m_strChrate)*100., bAppendPacket);
// 				else if((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && (pSelChart->m_nChartNumericValueType == DOLLAR_TYPE))
// 				{
// 					double dStdDollar = atof(((CStdDialog*)m_pStdDialog)->m_strDollar);
// 					if(dStdDollar == 0.0)
// 						strPriceForChart = "0";
// 					else 
// 						strPriceForChart.Format("%10.2f", (atof(strPriceForChart)/dStdDollar)*100.0);
// 
// 					m_pIRealReceiver->SetRealDataRQ(strKey, "����",			atof(strPriceForChart), bAppendPacket);
// 				}
// 				else
// 					m_pIRealReceiver->SetRealDataRQ(strKey, "����",			atof(strPriceForChart), bAppendPacket);
// 
// 				m_pIRealReceiver->SetRealDataRQ(strKey, "�⺻�ŷ���",	atof(pChartItemRealData->m_strVolume), bAppendPacket);
// 
// 				//@Solomon	091202SM123	-->
// 				strValue.Format("%.0f", atof(pRealData_Common->m_strValue)*1000000);
// 				strValue.TrimLeft();
// 
// 				m_pIRealReceiver->SetRealDataRQ(strKey, "�ŷ����",	atof(strValue), bAppendPacket);
// 				//@Solomon	091202SM123	<--
// #endif

				BOOL ret2 = m_pIRealReceiver->SetRealPacketEndRQ(strKey);

				// ���� ���ؼ� �ǽð� ���� ǥ�� - ojtaso (20090210)
				if(bAppend) // �ݵ�� Append�϶���..
					InvokeAddIn("ChartGuideLineAddIn.dll:GUIDE_LINE", INVOKEMSG_RECALCGUIDELINE, 0);
//				pIRealReceiver->Release();
			}
		}
	}
	//end

	//{{ 2006.10.09 by LYH ���� Repaint ���� �ذ�
	//���ἱ���϶� �ڵ尪�� �ٸ��� ���� repaint�ȵ� �� �־ ������.
	//if(!::lstrcmp(m_strRealItemCode, m_strSelectedItemCode) || m_strSelectedItemCode.Compare(DEF_CONTINUE_FUT_CODE)==0)
	//[A00000546]���ἱ�� ����ȵǴ� �κ� ����.
	{
		CRect rectTemp;
		if(rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
			GetParent()->InvalidateRect(rectTemp);
	}
	//}}
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SetRealPriceData::End", _T("��Ƽ��Ʈ �ü�"), 13);
#endif
}

void CChartItem::SetRealYPriceData(void *pData, long lDataKind)
{
	//�ؿ����� ���󶧹��� �߰�
	if(GetDataTRCode() == QUERY_STRFOREIGN)
	{
		lDataKind = UPJONG_REAL;
	}
	BOOL bRet = FALSE;

	CString strSign, strTime;
	CRealData_Common* pRealData_Common = (CRealData_Common*)pData;

	m_strRealItemCode = pRealData_Common->m_strCode;

	// �������� ����map�� �̿� : �������� - ojtaso (20070612)
	POSITION pos = m_mapChartItem.GetStartPosition();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	CString strPriceForChart;

//>> [QC No]090804HMC078
	m_strOpen = "";
	m_strHigh = "";
	m_strLow = "";
//<< [QC No]090804HMC078

	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)pSelChart);
		if(!::lstrcmp(pSelChart->m_strItemCode, m_strRealItemCode))
		{
			ST_CHARTITEM_REALDATA* pChartItemRealData = NULL;
			BOOL bRealItem = m_mapChartItemRealData.Lookup(m_strRealItemCode, (LPVOID&)pChartItemRealData);

			m_chSign = '9';
//			m_chSign = pRealData_Common->m_strSign.GetAt(0);
//>> [QC No]090804HMC078
			if(bRealItem)
			{
				pChartItemRealData->m_chSign = pRealData_Common->m_strSign.GetAt(0);
				pChartItemRealData->m_bIsRealJang = pRealData_Common->m_bIsRealJang;
			}
//<< [QC No]090804HMC078

			m_strPrice.Format("%ld", atol(pRealData_Common->m_strPrice));
			strPriceForChart = m_strPrice;
			{
				if (pSelChart->m_strChartGubun == QUERY_STRFUTOPT ||
					pSelChart->m_strChartGubun == QUERY_STRUPJONG ||
					pSelChart->m_strChartGubun == QUERY_STRFOJIPYO ||
					pSelChart->m_strChartGubun == QUERY_STRFOREIGN)
				{
					double dData = atof(m_strPrice)/100.;
					m_strPrice.Format("%.2f", dData);
				}
				m_strPrice.TrimLeft();

				if(bRealItem)
					pChartItemRealData->m_strPrice = m_strPrice;
			}

//>> [QC No]090804HMC078
			if(bRealItem)
			{
				pChartItemRealData->m_strOpen = m_strOpen;
				pChartItemRealData->m_strHigh = m_strHigh;
				pChartItemRealData->m_strLow = m_strLow;
			}
//<< [QC No]090804HMC078

//@Solomon:-->
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
//			m_strChrate = pRealData_Common->m_strChrate; 
//			if(bRealItem)
//				pChartItemRealData->m_strChrate = m_strChrate;
//@Solomon<--

// 			m_strVolume.Format("%ld", atol(pRealData_Common->m_strVolume));
// 			{
// 				m_strVolume.TrimLeft();
// 
// 				if(bRealItem)
// 					pChartItemRealData->m_strVolume = m_strVolume;
// 			}
			m_strVolume = pChartItemRealData->m_strVolume;
			if(bRealItem)
				pChartItemRealData->m_strVolume = m_strVolume;
		}
	}
	//end

	//{{ 2006.10.09 by LYH ���� Repaint ���� �ذ�
	if(!::lstrcmp(m_strRealItemCode, m_strSelectedItemCode))
	{
		CRect rectTemp;
		if(rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
			GetParent()->InvalidateRect(rectTemp);
	}
	//}}
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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

	//=================================================================
	// ���� ��� ���� �߰��Ѵ�..
	// 2005.10.14
	//=================================================================
	if(m_bReplaychartMode && m_bOnAdvise)
	{		
		m_bOnAdvise = FALSE;
		bAdviceForChart = FALSE;
		
		//=========================================================
		//���� ���̺귯���� ���� 
		//��¥�� ���;� �Ѵ�.
		//sPacket.Format("DATE=%d@",  )
		//=========================================================		
		//CDRChartOcx::SetPacketHelpMessage("DATE=20051207@", false);

		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//@��������
//		if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART || m_nMarketType == COMMODITY_CHART)
		if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
//@��������
		{
			//���� ó�� ������ �߰� 1/100 �ϴ� ��.05.12.08
			CDRChartOcx::SetHelpMessageRealDataRate("", true); 
		}
		
		m_strReplayTime = _T("������ �����Դϴ�                         ");		
	
		//AfxMessageBox("������ ����... ���� ó�� �ź��Ѵ�");

	}
	//=================================================================
	
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
		else if(!m_bReplaychartMode && !m_bOnAdvise && bAddMode)
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
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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
// 		if(strQueryType != QUERY_STRMARKET)
// 	//		SetOldChartCode(strOldCode);
// 			SetOldChartCode(m_strSelectedItemCode);

	if(IsAvailibleCode(lpCode))
		GetChartItemDataNew(strQueryType,lpCode,TRUE,bIsPrimaryData);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��ȸ�� ���� Input Spec�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
// �������� �߰� : �������� - ojtaso (20070118)
CString CChartItem::GetQueryData(BOOL bChangeType/* = FALSE*/)
{
	//@Solomon:091113SM038, 091116SM049
	// ����� N*N open�� ��ĭ ���ý� ��ȸ�ȵǴ� ���� ����.
	if(m_strDateForRequest.IsEmpty())
	{
		long lEndDate = ((CChartMng*)GetParent())->GetpExtInputBarDlg()->SendMessage(UMSG_GETENDDATEFROMINPUTBAR);
		if(lEndDate)
			m_strDateForRequest.Format("%d",lEndDate);
	}

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
		if((!m_bBuild || pMng->GetAddBlockMode()) && !bChangeType && !m_bReplaychartMode)
		{
			ST_CHARTITEM_REALDATA* pChartItemRealData;
			if(m_mapChartItemRealData.Lookup(lpszItemCode, (LPVOID&)pChartItemRealData))
				pChartItemRealData->m_nRefCount++;
			else
			{
				pChartItemRealData = new ST_CHARTITEM_REALDATA;
				pChartItemRealData->m_bIsRealJang = TRUE;

				pChartItemRealData->m_nRefCount = 1;
				CString strItemCode = lpszItemCode;
				((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strItemCode);
				//m_mapChartItemRealData.SetAt(lpszItemCode, (LPVOID)pChartItemRealData);
				m_mapChartItemRealData.SetAt(strItemCode, (LPVOID)pChartItemRealData);
			}

			AddChartItem(pChartItemData, lpszItemCode, ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(lpszItemCode, FALSE), m_chTypeForRequest, GetTickMinDayNumber(), GetCntForRequest(), GetChart_NumericValueType(), m_bBuild);
// 			// ���ἱ���� ��� �ֱٿ����� ����
// 			CString strItemCode; 
// 			strItemCode = lpszItemCode;
// 			if(pChartItemData->GetTrCode() == QUERY_STRFUTOPT)
// 			{
// 				((CStdDialog*)m_pStdDialog)->IsRecentFutCode(strItemCode);
// 			}
// 			AddChartItem(pChartItemData, strItemCode, pMng->GetJongMokNameByCode(strItemCode), m_chTypeForRequest, GetTickMinDayNumber(), GetCntForRequest(), GetChart_NumericValueType(), m_bBuild);
		}
		else if(m_bBuild)	// �ܼ� ��Ʈ ����(CHANGE_ITEM)
		{
			if(pMng->IsAbleToAdd() || !pMng->GetExistSyncChart())
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
						pChartItemRealData->m_bIsRealJang = TRUE;

						pChartItemRealData->m_nRefCount = 1;
						CString strItemCode = lpszItemCode;
						((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strItemCode);
						//m_mapChartItemRealData.SetAt(lpszItemCode, (LPVOID)pChartItemRealData);
						m_mapChartItemRealData.SetAt(strItemCode, (LPVOID)pChartItemRealData);
					}
				}
			}

			if(m_strSelectedItemCode.IsEmpty())
				m_strSelectedItemCode = lpszItemCode;
			else	// �м��� ���� ���� - ojtaso (20080728)
			{
				if(m_strSelectedItemCode != lpszItemCode || m_chType != m_chTypeForRequest)
				{
					//SaveandOpenFromAnalToolFile("", GetOpenAndSaveFileName(TRUE), FALSE, TRUE);

					//091002-alzioyes �м��������ƾ ����. ����ɼǵ��� �����ϵ���.
					LSaveandOpenFromAnalToolFile("", GetOpenAndSaveFileName(TRUE), FALSE, TRUE);
				}
			}

			AddChartItem(pChartItemData, lpszItemCode, ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(lpszItemCode, FALSE), m_chTypeForRequest, GetTickMinDayNumber(), GetCntForRequest(), GetChart_NumericValueType(), FALSE);
		}

//<< 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		int nMarketType, nCodeType;
		nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(m_lpSelChart->m_strItemCode, nCodeType);
		if (nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART || nMarketType == CMasterTypeInfo::COMMODITY_CHART)
			InvokeAddIn("ChartCommonAddIn.dll:GAP_REVISION", 0, 1);
		else
			InvokeAddIn("ChartCommonAddIn.dll:GAP_REVISION", 0, 0);
//>> (Issue No : 0006572)

		return pChartItemData->GetQueryData(m_bNext, m_lpSelChart);
	}
	return "";
}

//<< 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
//BOOL CChartItem::IsReqMonthGapRev()
BOOL CChartItem::SyncMonthGapRev()
{
	BOOL bNewGapRev = InvokeAddIn("ChartCommonAddIn.dll:GAP_REVISION", 1, 0);

	BOOL bRetValue = (bNewGapRev != m_bMonthGapRev);	// ���� ����.

	m_bMonthGapRev = bNewGapRev;

	return bRetValue;		// ���� ����.
}
//>> (Issue No : 0006572)

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
			pChartItemRealData->m_bIsRealJang = TRUE;

			pChartItemRealData->m_nRefCount = 1;
			CString strItemCode = m_lpSelChart->m_strItemCode;
			((CStdDialog*)m_pStdDialog)->IsContinueFutCode(strItemCode);
			m_mapChartItemRealData.SetAt(strItemCode, (LPVOID)pChartItemRealData);

			//{{2007.05.30 by LYH �ֽ�, ����, �ɼ� ������ �� TR��ȣ,�����ȣ ��߳��� ���� ����
			pChartItemData->SetTRCode(m_lpSelChart->m_strChartGubun);
			pChartItemData->SetCodeForQuery(m_lpSelChart->m_strItemCode);
			//}}
		}

//<< 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		int nMarketType, nCodeType;
		nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(m_lpSelChart->m_strItemCode, nCodeType);
		if (nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART || nMarketType == CMasterTypeInfo::COMMODITY_CHART)
			InvokeAddIn("ChartCommonAddIn.dll:GAP_REVISION", 0, 1);
		else
			InvokeAddIn("ChartCommonAddIn.dll:GAP_REVISION", 0, 0);
//>> (Issue No : 0006572)

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
	m_strTickMinDayForRequest = pSelChart->m_strTickMinDayNumber;

	switch(m_chTypeForRequest)
	{
	case TICK_DATA_CHART:
		SetTickNumber(m_strTickMinDayForRequest);
		break;
	case SEC_DATA_CHART:
		SetSecNumber(m_strTickMinDayForRequest);
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

	//	SetCntForRequest(pSelChart->m_lCount);
	SetChartNumericValue(pSelChart->m_nChartNumericValueType);

	CRect rectTemp;
	if(rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
		GetParent()->InvalidateRect(rectTemp);

	if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6401)
	{
		POSITION pos = m_mapChartItem.GetStartPosition();
		CString strRQ;
		LPVOID lpData;
		BOOL bShow = FALSE;
		while(pos)
		{
			m_mapChartItem.GetNextAssoc(pos, strRQ, lpData);
			bShow = (strRQ == m_strSelectedRQ);
			ShowGraphDataDrawingData(strRQ, "��������Ʈ", bShow, bShow);
		}
	}

	m_lpSelChart = pSelChart;
}

// �������� ���� : �������� - ojtaso (20080215)
void CChartItem::OnDeleteChartItem(LPCTSTR lpszRQ, ST_SELCHART*& pSelChart)
{
	ST_CHARTITEM_REALDATA* pChartItemRealData;
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
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��ȸ�� ���� Ÿ���� �����Ѵ�.
//			 chType: ƽ(0)/��(1)/��(2)/��(3)/��(4)
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetType(char chType) 
{
	ClearData(); 
	//m_chTypeForRequest = chType;
	SetChartRequestType(chType);

	//2005. 05. 27 ===================================================
	//���� ���� ���α׷� ������ ���� �ӽ÷� �־��.
	//m_chType = chType;
	//================================================================
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
long CChartItem::ReceiveLoadingChartData(void* szData, long nLength)
{
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(!pChartItemData)
		return 0L;

	BOOL bRetValue = pChartItemData->MakeChartData(szData,nLength, m_lpSelChart);

	//>> vntsorl 20081126 �������� ��ȸ������ ���� �ʱ�ȭ ó��
//	pChartItemData->AddLoadedPacket(m_strKey, m_lpSelChart);
//	pChartItemData->SetLoadedPacketData(m_strKey, m_lpSelChart);
	//<< vntsorl 20081126 �������� ��ȸ������ ���� �ʱ�ȭ ó��
/*
#ifdef __MULTI_LANG__
//@Solomon-MultiLang:091117 -->
	pChartItemData->AddLoadedPacket(m_strKey, m_lpSelChart);
	pChartItemData->SetLoadedPacketData(m_strKey, m_lpSelChart);
//@Solomon-MultiLang:091117 <--
#endif
*/
	ST_CHARTITEM_REALDATA* pChartItemRealData;
	if(m_mapChartItemRealData.Lookup(pChartItemData->GetCode(), (LPVOID&)pChartItemRealData))
	{
		char szTemp[64];
		if(pChartItemData->GetTrCode() == QUERY_STRJONGMOK)
		{
//@��������
//			S33100_O* pstOutput = (S33100_O*)szData;
//@��������
//@�����߰�
			KB_p0602_OutRec1_EX* pKB0602OutRec1 = (KB_p0602_OutRec1_EX*)szData;
//@�����߰�

			pChartItemRealData->m_chSign = pKB0602OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->price);
			pChartItemRealData->m_strPrice.Format("%ld", atol(szTemp));
			pChartItemRealData->m_strPrice.Remove(' ');
			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
			long lCount = 0;
			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0602_ChartData->fcnt);
			lCount = atol(szTemp);
			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0602_ChartData->chart[lCount-1].open);
			pChartItemRealData->m_strOpen = szTemp;
			pChartItemRealData->m_strOpen.Remove(' ');

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0602_ChartData->chart[lCount-1].high);
			pChartItemRealData->m_strHigh = szTemp;
			pChartItemRealData->m_strHigh.Remove(' ');

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0602_ChartData->chart[lCount-1].low);
			pChartItemRealData->m_strLow = szTemp;
			pChartItemRealData->m_strLow.Remove(' ');
			//}}

			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->change);
			pChartItemRealData->m_strChange.Format("%ld", atol(szTemp));
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->chrate);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			pChartItemRealData->m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strChrate.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->volume);
			pChartItemRealData->m_strVolume = szTemp;	
			pChartItemRealData->m_strVolume.TrimLeft();
			if(pChartItemRealData->m_strVolume!="0")
			{
				pChartItemRealData->m_strVolume.TrimLeft('0');
			}
			
//			MEMCPYWITHNULL(szTemp,pstOutput->volume);
//			pChartItemRealData->m_strVolume = szTemp;	
//			pChartItemRealData->m_strVolume.TrimLeft();
//			if(pChartItemRealData->m_strVolume!="0")
//			{
//				pChartItemRealData->m_strVolume.TrimLeft('0');
//			}
		}
		else if(pChartItemData->GetTrCode() == QUERY_STRUPJONG)//if(lTrCode==QUERY_UPJONG)
		{
//@��������	S35100_O* pstOutput = (S35100_O*)szData;	
			KB_p0603_OutRec1_EX* pKB0603OutRec1 = (KB_p0603_OutRec1_EX*)szData;

			pChartItemRealData->m_chSign = pKB0603OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			pChartItemRealData->m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strPrice.Remove(' ');

			//{{
			long lCount = 0;
			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0603_ChartData->fcnt);
			lCount = atol(szTemp);

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0603_ChartData->chart[lCount-1].open);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strOpen.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strOpen.Remove(' ');

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0603_ChartData->chart[lCount-1].high);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strHigh.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strHigh.Remove(' ');

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0603_ChartData->chart[lCount-1].low);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strLow.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strLow.Remove(' ');
			//}}


			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->change);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strChrate.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->volume);
			pChartItemRealData->m_strVolume = szTemp;	
			pChartItemRealData->m_strVolume.TrimLeft();
			if(pChartItemRealData->m_strVolume!="0")
			{
				pChartItemRealData->m_strVolume.TrimLeft('0');
			}

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
		else if(pChartItemData->GetTrCode() == QUERY_STRFUTOPT)//if(lTrCode==QUERY_FUTURE)
		{
//@��������	S36100_O* pstOutput = (S36100_O*)szData;
			KB_p0604_OutRec1_EX* pKB0604OutRec1 = (KB_p0604_OutRec1_EX*)szData;

			pChartItemRealData->m_chSign = pKB0604OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			pChartItemRealData->m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strPrice.Remove(' ');
//			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
			long lCount = 0;
			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0604_ChartData->fcnt);
			lCount = atol(szTemp);

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0604_ChartData->chart[lCount-1].open);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strOpen.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strOpen.Remove(' ');

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0604_ChartData->chart[lCount-1].high);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strHigh.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strHigh.Remove(' ');

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0604_ChartData->chart[lCount-1].low);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strLow.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strLow.Remove(' ');
//			//}}

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->change);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strChrate.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->volume);
			pChartItemRealData->m_strVolume = szTemp;	
			pChartItemRealData->m_strVolume.TrimLeft();
			if(pChartItemRealData->m_strVolume!="0")
			{
				pChartItemRealData->m_strVolume.TrimLeft('0');
			}

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
		else if(pChartItemData->GetTrCode() == QUERY_STRSTOCKFUTOPT)
		{
			char szTemp[64];
			KB_d1013_OutRec1_EX* pKB1013OutRec1 = (KB_d1013_OutRec1_EX*)szData;

			pChartItemRealData->m_chSign = pKB1013OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->price);
			pChartItemRealData->m_strPrice.Format("%ld", atol(szTemp));
			pChartItemRealData->m_strPrice.Remove(' ');

			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
			long lCount = 0;
			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB1013_ChartData->fcnt);
			lCount = atol(szTemp);

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB1013_ChartData->chart[lCount-1].open);
			pChartItemRealData->m_strOpen.Format("%ld", atol(szTemp));
			pChartItemRealData->m_strOpen.Remove(' ');

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB1013_ChartData->chart[lCount-1].high);
			pChartItemRealData->m_strHigh.Format("%ld", atol(szTemp));
			pChartItemRealData->m_strHigh.Remove(' ');

			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB1013_ChartData->chart[lCount-1].low);
			pChartItemRealData->m_strLow.Format("%ld", atol(szTemp));
			pChartItemRealData->m_strLow.Remove(' ');
			//}}

			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->change);
			pChartItemRealData->m_strChange.Format("%ld", atol(szTemp));
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->chrate);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			pChartItemRealData->m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strChrate.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->volume);
			pChartItemRealData->m_strVolume = szTemp;	
			pChartItemRealData->m_strVolume.TrimLeft();
			if(pChartItemRealData->m_strVolume!="0")
			{
				pChartItemRealData->m_strVolume.TrimLeft('0');
			}
		}
		else if(pChartItemData->GetTrCode() == QUERY_STRFOREIGN)//if(lTrCode==QUERY_FUTURE)
		{
//@��������	D03040_O* pstOutput = (D03040_O*)szData;
			KB_p0606_OutRec1* pKB0606OutRec1 = (KB_p0606_OutRec1*)szData;
	
			pChartItemRealData->m_chSign = pKB0606OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			pChartItemRealData->m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strPrice.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->change);
			pChartItemRealData->m_strChange = szTemp;
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->volume);
			pChartItemRealData->m_strVolume = szTemp;	
			pChartItemRealData->m_strVolume.TrimLeft();
			if(pChartItemRealData->m_strVolume!="0")
			{
				pChartItemRealData->m_strVolume.TrimLeft('0');
			}

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
		else if (pChartItemData->GetTrCode() == QUERY_STRFOJIPYO)
		{
			KB_p0515_OutRec* pOutRec = (KB_p0515_OutRec*)szData;
			
			pChartItemRealData->m_chSign = pOutRec->sign[0];
			MEMCPYWITHNULL(szTemp, pOutRec->name);
			pChartItemRealData->m_strItemName = szTemp;
			pChartItemRealData->m_strItemName.TrimLeft(' ');
			pChartItemRealData->m_strItemName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp, pOutRec->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			pChartItemRealData->m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strPrice.Remove(' ');
			
			MEMCPYWITHNULL(szTemp, pOutRec->change);
			pChartItemRealData->m_strChange = szTemp;
			pChartItemRealData->m_strChange.Remove(' ');
			
			MEMCPYWITHNULL(szTemp, pOutRec->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			pChartItemRealData->m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			pChartItemRealData->m_strChange.Remove(' ');
			
			MEMCPYWITHNULL(szTemp, pOutRec->volume);
			pChartItemRealData->m_strVolume = szTemp;	
			pChartItemRealData->m_strVolume.TrimLeft();
			if (pChartItemRealData->m_strVolume!="0")
				pChartItemRealData->m_strVolume.TrimLeft('0');
		}
//@��������
//	else if(m_lpSelChart->m_strChartGubun == QUERY_STRELW)
//		{
//			S01007_O* pstOutput = (S01007_O*)szData;
//
//			pChartItemRealData->m_chSign = pstOutput->sign[0];
//			MEMCPYWITHNULL(szTemp,pstOutput->name);
//			pChartItemRealData->m_strItemName = szTemp;
//			pChartItemRealData->m_strItemName.TrimLeft(' ');
//			pChartItemRealData->m_strItemName.TrimRight(' ');
//			MEMCPYWITHNULL(szTemp,pstOutput->price);
//			pChartItemRealData->m_strPrice = szTemp;
//			pChartItemRealData->m_strPrice.Remove(' ');
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
//
//			MEMCPYWITHNULL(szTemp,pstOutput->change);
//			pChartItemRealData->m_strChange = szTemp;
//			pChartItemRealData->m_strChange.Remove(' ');
//
//			MEMCPYWITHNULL(szTemp,pstOutput->chrate);
//			pChartItemRealData->m_strChrate = szTemp;
//			pChartItemRealData->m_strChrate.Remove(' ');
//			MEMCPYWITHNULL(szTemp,pstOutput->volume);
//			pChartItemRealData->m_strVolume = szTemp;	
//			pChartItemRealData->m_strVolume.TrimLeft();
//			if(pChartItemRealData->m_strVolume!="0")
//			{
//				pChartItemRealData->m_strVolume.TrimLeft('0');
//			}		
//		}
//@��������
	}

	CChartMng* lpChartMng = (CChartMng*)GetParent();

//@��������
//	// ���õ� RQ�� BuildChart���� Real�����
//	if(m_strKey.Compare(m_strSelectedRQ))
//		lpChartMng->AddRealCodeList(m_lpSelChart->m_strItemCode);
//@��������

//>> [QC No]090811HMC105
	if(IsPrimaryChartItemData(pChartItemData))
	{
		SetDrdsAdvise(1);
		if(!((CStdDialog*)m_pStdDialog)->HasKey(pChartItemData->GetCode()))
			SetDrdsAdvise(1);
	}
//<< [QC No]090811HMC105
/*
	//>> vntsorl 20081126 �������� ��ȸ������ ���� �ʱ�ȭ ó��
#ifndef __MULTI_LANG__
 	pChartItemData->AddLoadedPacket(m_strKey, m_lpSelChart);
 	pChartItemData->SetLoadedPacketData(m_strKey, m_lpSelChart);
#endif
	//<< vntsorl 20081126 �������� ��ȸ������ ���� �ʱ�ȭ ó��
*/
 	pChartItemData->AddLoadedPacket(m_strKey, m_lpSelChart);
 	pChartItemData->SetLoadedPacketData(m_strKey, m_lpSelChart);

	AfxBeginThread(THREAD_SEND2SERVER, lpChartMng);
//	lpChartMng->RequestChartData();

	return 0L;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ��������ȸ������� �޴´�.
//////////////////////////////////////////////////////////////////////////////
long CChartItem::ReceiveInitalChartData(void* szData, long nLength)
{
	CString strJMCode;
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(!pChartItemData)
		return 0;

	// �������� �ҷ����� : �������� - ojtaso (20070514)
	CChartMng* lpChartMng = (CChartMng*)GetParent();
	BOOL bRetValue = pChartItemData->MakeChartData(szData,nLength, m_lpSelChart);
	if(!bRetValue)
		return 0;

	m_strOpen = "";
	m_strHigh = "";
	m_strLow = "";

	CString strTrCode = _T(""); //ȣ�� ��û ������ ������ ����. 2005. 07. 15
	// �������� ���� : �������� - ojtaso (20070328)
	CString strCode = m_lpSelChart ? m_lpSelChart->m_strItemCode : pChartItemData->GetCode();

	if(IsPrimaryChartItemData(pChartItemData)&&!m_bNext)	// Update Title Info..
	{
		m_bReceivedDataChart = TRUE;

		strTrCode = pChartItemData->GetTrCode();
		if(strTrCode == QUERY_STRJONGMOK)
		{			
			char szTemp[64];
			KB_p0602_OutRec1_EX* pKB0602OutRec1 = (KB_p0602_OutRec1_EX*)szData;

			m_chType = m_chTypeForRequest;			
			if (m_chType == WEEK_DATA_CHART)
			{
				CString strEDate = pChartItemData->GetEndDate();
				if (strEDate.GetLength() <= 0)
					strEDate = m_strDateForRequest;

				SetEndDate(strEDate);
			}
			else
				SetEndDate(m_strDateForRequest); //05.09.06 m_strEndDate = m_strDateForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case SEC_DATA_CHART:
					SetSecNumber(m_strTickMinDay);
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

			//=======================================================================
			// 2005. 10. 12
			// ��ȸ �ɶ����� �� ��ȸ ���к� ���� ��ǥ ���� OCX�� ��´�.
			//=======================================================================
			lpChartMng->SetpChartCfgWnd(m_chType, (CWnd*)this);
			//=======================================================================

			m_chSign = pKB0602OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->price);
			m_strPrice.Format("%ld", atol(szTemp));
			m_strPrice.Remove(' ');

			long lCount = 0;
 			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0602_ChartData->fcnt);
 			lCount = atol(szTemp);

			GetOHLValue(strTrCode, &pChartItemData->m_pKB0602_ChartData->bojomsg[0], m_strOpen, m_strHigh, m_strLow);

			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->change);
			m_strChange.Format("%ld", atol(szTemp));
			m_strChange.Remove(' ');
			
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->chrate);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			m_strChrate.Remove(' ');
			
			MEMCPYWITHNULL(szTemp,pKB0602OutRec1->volume);
			m_strVolume = szTemp;	
			m_strVolume.TrimLeft();
			if(m_strVolume!="0")
			{
				m_strVolume.TrimLeft('0');
			}

			//2007.01.15 by LYH ������Ʈ�ΰ�� �������� ����
			if( m_bReplaychartMode )
			{
				CString strData;
				strData.Format("%s", pKB0602OutRec1->bojomsg);
				SetReplay_PrePrice(strData);
			}
		}
		else if(strTrCode == QUERY_STRUPJONG)
		{
			char szTemp[64];
			KB_p0603_OutRec1_EX* pKB0603OutRec1 = (KB_p0603_OutRec1_EX*)szData;
			m_chType = m_chTypeForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			if (m_chType == WEEK_DATA_CHART)
			{
				CString strEDate = pChartItemData->GetEndDate();
				if (strEDate.GetLength() <= 0)
					strEDate = m_strDateForRequest;

				SetEndDate(strEDate);
			}
			else
				SetEndDate(m_strDateForRequest); //05.09.06 m_strEndDate = m_strDateForRequest;

			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case SEC_DATA_CHART:
					SetSecNumber(m_strTickMinDay);
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

			//=======================================================================
			// 2005. 10. 12
			// ��ȸ �ɶ����� �� ��ȸ ���к� ���� ��ǥ ���� OCX�� ��´�.
			//=======================================================================
			lpChartMng->SetpChartCfgWnd(m_chType, (CWnd*)this);
			//=======================================================================

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

			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
			long lCount = 0;
			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0603_ChartData->fcnt);
			lCount = atol(szTemp);

			GetOHLValue(strTrCode, &pChartItemData->m_pKB0603_ChartData->bojomsg[0], m_strOpen, m_strHigh, m_strLow);
			//}}

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

			MEMCPYWITHNULL(szTemp,pKB0603OutRec1->volume);
			m_strVolume = szTemp;	
			m_strVolume.TrimLeft();
			if(m_strVolume!="0")
			{
				m_strVolume.TrimLeft('0');
			}
		}
		else if(strTrCode == QUERY_STRFUTOPT)
		{
			char szTemp[64];
			KB_p0604_OutRec1_EX* pKB0604OutRec1 = (KB_p0604_OutRec1_EX*)szData;
			m_chType = m_chTypeForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			if (m_chType == WEEK_DATA_CHART)
			{
				CString strEDate = pChartItemData->GetEndDate();
				if (strEDate.GetLength() <= 0)
					strEDate = m_strDateForRequest;

				SetEndDate(strEDate);
			}
			else
				SetEndDate(m_strDateForRequest); //05.09.06 m_strEndDate = m_strDateForRequest;

			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case SEC_DATA_CHART:
					SetSecNumber(m_strTickMinDay);
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

			//=======================================================================
			// 2005. 10. 12
			// ��ȸ �ɶ����� �� ��ȸ ���к� ���� ��ǥ ���� OCX�� ��´�.
			//=======================================================================
			lpChartMng->SetpChartCfgWnd(m_chType, (CWnd*)this);
			//=======================================================================

			m_chSign = pKB0604OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');

			//@solomon	���ἱ�� ����� ������.	-->
			if (((CStdDialog*)m_pStdDialog)->IsChartScreen("OldFuture") == TRUE)
			{
				((CStdDialog*)m_pStdDialog)->GetOldFututureCodeName(strCode, m_strKorName);				
			}
			else
			{
				IMasterDataManagerLast* pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
				if (pDataManager)
				{
					m_strKorName = pDataManager->GetCodeInfo("FMASTER", 0, strCode);
				}
			}
			//@solomon	���ἱ�� ����� ������.	<--

			double nOption = 100.0;
			CString strShortCode = strCode.Left(3);
			if(strShortCode.Compare("185")==0 || strShortCode.Compare("186")==0 || strShortCode.Compare("187")==0)
				nOption = 1.0;

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/nOption;	//nTemp/100.;
			m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			m_strPrice.Remove(' ');

			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
			long lCount = 0;
			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB0604_ChartData->fcnt);
			lCount = atol(szTemp);

			GetOHLValue(strTrCode, &pChartItemData->m_pKB0604_ChartData->bojomsg[0], m_strOpen, m_strHigh, m_strLow);

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

			MEMCPYWITHNULL(szTemp,pKB0604OutRec1->volume);
			m_strVolume = szTemp;	
			m_strVolume.TrimLeft();
			if(m_strVolume!="0")
			{
				m_strVolume.TrimLeft('0');
			}

			if( m_bReplaychartMode )
			{
				CString strData;
				strData.Format("%s", pKB0604OutRec1->bojomsg);
				SetReplay_PrePrice(strData);
			}
		}
		else if(strTrCode == QUERY_STRSTOCKFUTOPT/* || strTrCode == QUERY_STRCOMMODITY*/)//if(lTrCode==QUERY_FUTURE)
		{
			char szTemp[64];
			KB_d1013_OutRec1_EX* pKB1013OutRec1 = (KB_d1013_OutRec1_EX*)szData;
			m_chType = m_chTypeForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			if (m_chType == WEEK_DATA_CHART)
			{
				CString strEDate = pChartItemData->GetEndDate();
				if (strEDate.GetLength() <= 0)
					strEDate = m_strDateForRequest;

				SetEndDate(strEDate);
			}
			else
				SetEndDate(m_strDateForRequest); //05.09.06 m_strEndDate = m_strDateForRequest;

			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case SEC_DATA_CHART:
					SetSecNumber(m_strTickMinDay);
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

			//=======================================================================
			// 2005. 10. 12
			// ��ȸ �ɶ����� �� ��ȸ ���к� ���� ��ǥ ���� OCX�� ��´�.
			//=======================================================================
			lpChartMng->SetpChartCfgWnd(m_chType, (CWnd*)this);
			//=======================================================================

			m_chSign = pKB1013OutRec1->sign[0];
			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->price);
			m_strPrice.Format("%ld", atol(szTemp));
			m_strPrice.Remove(' ');

			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
			long lCount = 0;
			MEMCPYWITHNULL(szTemp,pChartItemData->m_pKB1013_ChartData->fcnt);
			lCount = atol(szTemp);

			GetOHLValue(strTrCode, &pChartItemData->m_pKB1013_ChartData->bojomsg[0], m_strOpen, m_strHigh, m_strLow);

			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->change);
			m_strChange.Format("%ld", atol(szTemp));
			m_strChange.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->chrate);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			m_strChrate.Remove(' ');

			MEMCPYWITHNULL(szTemp,pKB1013OutRec1->volume);
			m_strVolume = szTemp;	
			m_strVolume.TrimLeft();
			if(m_strVolume!="0")
			{
				m_strVolume.TrimLeft('0');
			}		
		}
		else if(strTrCode == QUERY_STRFOREIGN)//if(lTrCode==QUERY_FUTURE)
		{			
			char szTemp[64];
			KB_p0606_OutRec1_EX* pKB0606OutRec1 = (KB_p0606_OutRec1_EX*)szData;
			m_chType = m_chTypeForRequest;			
			if (m_chType == WEEK_DATA_CHART)
			{
				CString strEDate = pChartItemData->GetEndDate();
				if (strEDate.GetLength() <= 0)
					strEDate = m_strDateForRequest;

				SetEndDate(strEDate);
			}
			else
				SetEndDate(m_strDateForRequest); //05.09.06 m_strEndDate = m_strDateForRequest;

			m_strTickMinDay = m_strTickMinDayForRequest;
			switch(m_chType)
			{			
				case MIN_DATA_CHART:
					SetMinNumber(m_strTickMinDay);
					break;
				case DAY_DATA_CHART:
					SetDayNumber(m_strTickMinDay);
					break;
				default:
					m_strTickMinDay = "001";
					break;
			}	
			
			//=======================================================================
			// 2005. 10. 12
			// ��ȸ �ɶ����� �� ��ȸ ���к� ���� ��ǥ ���� OCX�� ��´�.
			//=======================================================================
			lpChartMng->SetpChartCfgWnd(m_chType, (CWnd*)this);
			//=======================================================================

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

			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
			long lCount = 0;
			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->fcnt);
			lCount = atol(szTemp);

			GetOHLValue(strTrCode, &pChartItemData->m_pKB0606_ChartData->bojomsg[0], m_strOpen, m_strHigh, m_strLow);

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

			MEMCPYWITHNULL(szTemp,pKB0606OutRec1->volume);
			m_strVolume = szTemp;	
			m_strVolume.TrimLeft();
			if(m_strVolume!="0")
			{
				m_strVolume.TrimLeft('0');
			}
		}
		else if (strTrCode == QUERY_STRFOJIPYO)
		{
			char szTemp[64];
			KB_p0515_OutRec* pOutRec = (KB_p0515_OutRec*)szData;
			m_chType = m_chTypeForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			if (m_chType == WEEK_DATA_CHART)
			{
				CString strEDate = pChartItemData->GetEndDate();
				if (strEDate.GetLength() <= 0)
					strEDate = m_strDateForRequest;

				SetEndDate(strEDate);
			}
			else
				SetEndDate(m_strDateForRequest); //05.09.06 m_strEndDate = m_strDateForRequest;
			
			switch (m_chType)
			{
			case TICK_DATA_CHART:
				SetTickNumber(m_strTickMinDay);
				break;
			case SEC_DATA_CHART:
				SetSecNumber(m_strTickMinDay);
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

			// ��ȸ �ɶ����� �� ��ȸ ���к� ���� ��ǥ ���� OCX�� ��´�.
			lpChartMng->SetpChartCfgWnd(m_chType, (CWnd*)this);

			m_chSign = pOutRec->sign[0];
			MEMCPYWITHNULL(szTemp, pOutRec->name);
			m_strKorName = szTemp;
			m_strKorName.TrimLeft(' ');
			m_strKorName.TrimRight(' ');
			
			MEMCPYWITHNULL(szTemp, pOutRec->price);
			int nTemp = atoi(szTemp);
			double dData = nTemp/100.;
			m_strPrice.Format("%*.2f", sizeof(szTemp), dData);
			m_strPrice.Remove(' ');
			
			//{{2006.10.09 by LYH �ð�, ��, ���� �߰�
			long lCount = 0;
			MEMCPYWITHNULL(szTemp, pChartItemData->m_pKB0515_ChartData->fcnt);
			lCount = atol(szTemp);

			GetOHLValue(strTrCode, &pChartItemData->m_pKB0515_ChartData->bojomsg[0], m_strOpen, m_strHigh, m_strLow);

			MEMCPYWITHNULL(szTemp, pOutRec->change);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChange.Format("%*.2f", sizeof(szTemp), dData);
			m_strChange.Remove(' ');
			
			MEMCPYWITHNULL(szTemp, pOutRec->chrate);
			nTemp = atoi(szTemp);
			dData = nTemp/100.;
			m_strChrate.Format("%*.2f", sizeof(szTemp), dData);
			m_strChrate.Remove(' ');
			
			MEMCPYWITHNULL(szTemp, pOutRec->volume);
			m_strVolume = szTemp;	
			m_strVolume.TrimLeft();
			if (m_strVolume!="0")
				m_strVolume.TrimLeft('0');
		}
		else
			strTrCode = _T("");

		ST_CHARTITEM_REALDATA* pChartItemRealData;

		CString szCheckCode(strCode);
		{
			((CStdDialog*)m_pStdDialog)->IsContinueFutCode(szCheckCode);
		}
		//if(m_mapChartItemRealData.Lookup(strCode, (LPVOID&)pChartItemRealData))
		if(m_mapChartItemRealData.Lookup(szCheckCode, (LPVOID&)pChartItemRealData))
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

		//{{ �ֹ����� �����ڵ�, ����� �ѱ�
		if(m_strSelectedRQ == "DEFAULT")
		{
			CString strData = GetDataCode()+":"+m_strKorName;
			InvokeAddInStr("ChartPeriodSummaryAddIn.dll:ORDER_LINE", "102", strData);
		}
		//}}
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
		BOOL bNext = m_bNext;
		if (!BuildChart())
			return 0;

		if (FindChartItemDataForQuery())
			return 2; // �ش�ChartItem���� �߰����� Request�� �ʿ��Ҷ�..
		else
		{
			if(m_bLoadChartInfo)
			{
				SaveandOpenFromFile(GetOpenAndSaveFileName(),"",TRUE,FALSE);
				m_bLoadChartInfo = FALSE;
			}
			
			if(strTrCode == QUERY_STRJONGMOK)
			{
				CWnd* pTargetWnd = GetParent();
				if(pTargetWnd && pTargetWnd->GetSafeHwnd()!=NULL)
				{
					pTargetWnd = GetParent()->GetParent();
					if(pTargetWnd && pTargetWnd->GetSafeHwnd()!=NULL)
					{
						pTargetWnd->SendMessage(UMSG_SENDTR_HOGA_DATA, (WPARAM)&strCode, 0);
					}
				}
			}

			// �������� �ҷ����� : �������� - ojtaso (20080521)
			if(m_mapChartItem.GetCount() > 1)
				CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);

			//{{JS.Kim_20100920. ����Ÿ �Ǽ� �ʰ��Ͽ� �ڸ��� ������ �ּ��� TR ��ȸ�� �Ǽ� ��ŭ�� �������ֵ��� ����
			if( bNext )
			{
				CDRChartOcx::SetNHoldingPacketSize(CDRChartOcx::GetNHoldingPacketSize() + pChartItemData->m_lCntTemp);
			}
			else
			{
				if( pChartItemData->m_lCntTemp > _HOLDING_PACKET_COUNT+_REMOVE_PACKET_COUNT )		CDRChartOcx::SetNHoldingPacketSize(pChartItemData->m_lCntTemp+_REMOVE_PACKET_COUNT);
				else						CDRChartOcx::SetNHoldingPacketSize(_HOLDING_PACKET_COUNT+_REMOVE_PACKET_COUNT);
			}
			//}}

			return 1; // �ش�ChartItem���� �߰����� Request �Ϸ�...
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
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
/*	CChartItemData *pChartItemData = NULL;
	if (!m_mapChartItemData.Lookup(m_strSelectedRQ, pChartItemData))
		m_mapChartItemData.Lookup(_T("DEFAULT"), pChartItemData);
	BOOL bIsPrimaryChartItemData = m_strSelectedRQ.Compare(_T("DEFAULT")) == 0;
*/
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
		BOOL bCodeLinkMode = ((CStdDialog*)m_pStdDialog)->SendMessage(UMSG_GETJONGMOKLINK);
		BOOL bAddMode = pChartMng->GetAddGraphMode();
		if(bCodeLinkMode)
			bAddMode = pChartMng->GetAddBlockMode();

		int nMode = pChartMng->GetMode();
		if( nMode == NEW_ITEM)
			pChartItemData->AddPacketAndGraph2(bAddMode);
		else if(nMode == OVERLAPP_ITEM)
			pChartItemData->AddPacketAndGraph2(bAddMode, TRUE);
		else
		{
			BOOL bAddGraph = m_bReplaychartMode ? FALSE : bAddMode;
			pChartItemData->AddPacketAndGraph2(bAddGraph);
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

	m_bAfterReceiveData = TRUE;

	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	pChartMng->m_bChangeTypeMode = FALSE;

	// 2005. 08. 17 ���� ó�� ȯ���� �ٲ�鼭 
	// �������� �߰� ������� : �������� - ojtaso (20070119)
	if(!IsAddMode())
		pChartItemData->RemoveRealItem();

	ShowInnerToolBar(TRUE);

	// �м��� ���� ���� - ojtaso (20080723)
	BOOL bSaveAnalTool = ((CChartMng*)GetParent())->GetSaveAnalTool();
	BOOL bRetValueOfSetPacketData = FALSE;
	if(bSaveAnalTool)
		bRetValueOfSetPacketData = pChartItemData->SetPacketData(!m_bNext);

	BOOL bTypeLink		= ((CChartMng*)GetParent())->GetBTypeLink();
	BOOL bChartExtended = ((CChartMng*)GetParent())->GetChartExtended();
	BOOL bSaveJipyoPerCodePeriod = ::GetCommonDataInfo(GetSafeHwnd(), COMDATA_SAVE_JIPYO_PER_CODE_PERIOD, 0);
	if((!bTypeLink || bChartExtended) && bSaveJipyoPerCodePeriod && m_bLoadChartInfo)
	{
		SaveandOpenFromFile(GetOpenAndSaveFileName(), "", TRUE, FALSE, TRUE);
		m_bLoadChartInfo = FALSE;
	}
	else if(((CStdDialog*)m_pStdDialog)->GetUseCycleSave())
	{
		if(!((CStdDialog*)m_pStdDialog)->IsMultiItemChart() && !((CChartMng*)GetParent())->GetJongmokLink())
		{
			if(!((CChartMng*)GetParent())->GetSaveJipyoOneFile())
			{
				int nCnt = GetParent()->SendMessage(UMSG_GETZOOMVALUE, GetType());
				SaveandOpenFromFile(GetOpenAndSaveFileName(TRUE), "", TRUE, FALSE);
				m_bLoadChartInfo = FALSE;

				if(nCnt > 0)
					CDRChartOcx::SetOnePageDataCount(nCnt);
			}
			else
			{
				SaveandOpenFromFile(GetOpenAndSaveFileName(), "", TRUE, FALSE);
			}
		}
	}
	
	if (bSaveAnalTool) // �м��� ���� ���� - ojtaso (20080728)
		LSaveandOpenFromAnalToolFile(GetOpenAndSaveFileName(TRUE), "", TRUE, FALSE);

	//{{2007.08.22 by LYH
	//====================================================================================
	if( !HasGraph("������Ʈ") && !HasGraph("��������Ʈ"))
	{
		AddGraph(0,0,0,"������Ʈ"		,FALSE	,FALSE	,FALSE	); 
		AddGraph(0,0,0,"���� �̵����"	,FALSE	,FALSE	,FALSE	);
		AddGraph(1,0,0,"�ŷ�����Ʈ"		,TRUE	,TRUE	,TRUE	);
	}
	//====================================================================================
	//}}

	BOOL bNextOld = m_bNext;	
	// �м��� ���� ���� - ojtaso (20080723)
	if(!bSaveAnalTool)
		bRetValueOfSetPacketData = pChartItemData->SetPacketData(!m_bNext);
	
	if(m_nChartNumericValueType == WON_TYPE)
		SetBShowPriceRateInquiry(TRUE);
	else
		SetBShowPriceRateInquiry(FALSE);

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
				if(m_lpSelChart)
					m_lpSelChart->m_lCount += lSize;
			}
			else
			{
				m_lAllCnt = lSize;
				if(m_lpSelChart)
					m_lpSelChart->m_lCount = lSize;
			}
		}
	}

	CString strDateToday;
	CTime t = CTime::GetCurrentTime();
	strDateToday = t.Format("%Y%m%d");

	if(((CStdDialog*)m_pStdDialog)->m_nScreenID != nScreenID_6401)
	{
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
		if(!((CChartMng*)GetParent())->IsInLoop())
		{
			BOOL bChkCrossLine = ((CChartMng*)GetParent())->GetpExtInputBarDlg()->SendMessage(RMSG_CHANGE_CROSSLINE);
			SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, bChkCrossLine);	
		}

		CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);

		if(GetSelChart(m_strSelectedRQ, m_lpSelChart))
		{
			CChartItemData *pItemData = m_arrayChartItemData.GetAt(0);
			int nSize = m_arrayChartItemData.GetSize();

			BOOL bFiil = pItemData->IsFilled();

			OnSelectChartItem(m_lpSelChart, FALSE);

			pItemData->m_bIsFilled = bFiil;
		}

		SetOldChartCode(m_lpSelChart->m_strItemCode);
		m_lpSelChart = NULL;
	}

	return TRUE;
}

BOOL CChartItem::ResetChart()
{
	BOOL bRetValue = CDRChartOcx::ResetChart();	

	// ��
	if(((CChartMng*)GetParent())->GetExistSyncChart() == FALSE)
	{
		// ��� �������� ����
		ST_CHARTITEM_REALDATA* lpChartItemRealData;
		CString strKey;
		POSITION pos = m_mapChartItemRealData.GetStartPosition();
		while(pos)
		{
			m_mapChartItemRealData.GetNextAssoc(pos, strKey, (LPVOID&)lpChartItemRealData);
			if(lpChartItemRealData)
				delete lpChartItemRealData;
		}

		m_mapChartItemRealData.RemoveAll();
		m_strSelectedItemCode.Empty();
	}

	return bRetValue;
}

void CChartItem::ResetRQ()
{
	m_mapChartItemRealData.RemoveAll();
	m_strSelectedItemCode.Empty();
}

//////////////////////////////////////////////////////////////
///////////////////// func For Save & Load ChartInfo
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
// BOOL CChartItem::LoadChartInfo(LPCTSTR lpTitle, BOOL bSetChartTick /* TRUE */, BOOL bNotCodeInputInit /* TRUE */)
BOOL CChartItem::LoadChartInfo(LPCTSTR lpTitle, BOOL bSetChartTick /* TRUE */, BOOL bNotCodeInputInit /* TRUE */, BOOL bRQPeriod/*=FALSE*/, LPCTSTR lpszCodeForRequest/*=NULL*/)
//20110502 �̹��� <<
{
	CString strAppName;
	CString strTemp;
	CString strTemp2;
	char szTemp[128];
	char szTemp2[128];

	//��Ʈ �ε�� ���� ����� ��Ʈ�� �ϳ��� ���� 
	//��츦 üũ�ϱ� ���� 
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	//20110502 �̹��� >>
	//strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);
	if(!bRQPeriod)
		strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);
	else
		strAppName="0000";
	//20110502 �̹��� <<
	::GetPrivateProfileString(strAppName,"CntOfGraph","1",szTemp,sizeof(szTemp),lpTitle);
	long lCntOfGraph = atol(szTemp);

	if(lCntOfGraph == 0)  //����� ��Ʈ ������ ���� ���� ��� 
		bNotCodeInputInit = FALSE; //������ �������� �����ڵ带 �״�� CodeInput�� �α����� FALSE�� ���Ѵ�.

	BOOL bLoadSaveCode = ((CChartMng*)GetParent())->GetpInputBarDlg()->SendMessage(UMSG_GETLOADSAVECODE);
	//@Solomon-A002:�������� �ε��϶��� ������ �ٲٰ�, �׷��� ������ �״��.
//	int bImsi = ::GetPrivateProfileInt("IMSI","LoadSaveCode", 0, lpTitle);
//	if(bImsi) bLoadSaveCode = TRUE;

	char* lpszExt = _T(".mct");
	if (::strstr(lpTitle, lpszExt))
	{
		// �������� �ε� : �������� - ojtaso (20070118)
		if (bLoadSaveCode)
			LoadRQInfo(strAppName, lpTitle);
	}	

	if (bNotCodeInputInit == FALSE)
		ResetOCX(FALSE);//�����ڵ� �κ��� ������ �ʴ´�.
	else
	{
		ResetChart();
		ResetOCX();		//�����ڵ� �κ��� �����.
	}

	//�����ְ� ����Ʈ TRUE�� ���� 2006.03.16 by LYH
	m_CfgInfo.bRevisedValue = TRUE;

	if (lCntOfGraph == 0) 
	{
		((CStdDialog*)m_pStdDialog)->ShareInitCode();
		return FALSE;
	}

	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
	CChartItemData *pData = NULL;
	int nMarketType = -1;
	for (long lPos=0; lPos<lCntOfGraph; lPos++)
	{
		strTemp.Format("Code%02d",lPos);
		strTemp2.Format("TRCode%02d",lPos);
		::GetPrivateProfileString(strAppName,strTemp,"",szTemp,sizeof(szTemp),lpTitle);
		::GetPrivateProfileString(strAppName,strTemp2,"",szTemp2,sizeof(szTemp2),lpTitle);

		//20110405 �ڵ�� ��ȸ TR�� �߸��� ��찡 �ֳ�..
		CString rData[2];
		if(_doCheckCodeAndTR(szTemp, szTemp2, rData, lpTitle, strAppName)==FALSE)
		{
			sprintf(szTemp, (LPCSTR)rData[1]);
			sprintf(szTemp2, (LPCSTR)rData[0]);
			::WritePrivateProfileString(strAppName, strTemp , szTemp , lpTitle);
			::WritePrivateProfileString(strAppName, strTemp2, szTemp2, lpTitle);
		}

		if ((lPos == 0) && !((CStdDialog*)m_pStdDialog)->IsChartScreen("OldFuture"))
		{
			CString strPreCode = ((CChartMng*)GetParent())->m_strCodeForLoadChart;
			if (strPreCode.GetLength()>0 && ((CChartMng*)GetParent())->GetpInputBarDlg())
			{
				if (bLoadSaveCode)
				{
					CString strSaveCode, strCodeName, strLastCode;
					strSaveCode.Format("%s", szTemp);
					strSaveCode.TrimLeft();
					int nRealMarketType;
					nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(strSaveCode, nRealMarketType);
					if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
					{
						strCodeName	= pDataManager->GetCodeInfo("FMASTER", 0, strSaveCode);			
						if(strCodeName.GetLength()<1)
							strCodeName		= pDataManager->GetCodeInfo("PMASTER", 0, strSaveCode);

						if(strCodeName.GetLength()<=0 || strCodeName == strSaveCode)//����� ������ ����
						{
							int nStep;
							((CStdDialog*)m_pStdDialog)->GetFirstCode(strLastCode, m_nMarketType, nStep);
//							strLastCode = GetLastHistoryCode(CMasterTypeInfo::FUTOPT_CHART);

							if(strLastCode.GetLength()>0)
								strPreCode = strLastCode; 
						}
						else
						{
							strPreCode = "";
						}
					}
				}
				else
				{
					memset(szTemp, 0x00, sizeof(szTemp));
					memcpy(szTemp, (LPTSTR)(LPCTSTR)strPreCode, strPreCode.GetLength());

					int nRealMarketType;
					nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(strPreCode, nRealMarketType);

					CString szTRCode;
					if(g_pPtUtilMng) g_pPtUtilMng->GetTRCodeFromCode(nMarketType, "", szTRCode);
					sprintf(szTemp2, "%s", (LPCSTR)szTRCode);
				}
			}
			else
			{
				CString strQuery;
				strQuery.Format("%s", szTemp2);
				CString strShareCode = ((CChartMng*)GetParent())->m_strStatic_ShareCode;
				CString strItemName = ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(strShareCode);

				if( !strItemName.IsEmpty() && (   ( strQuery == QUERY_STRJONGMOK && strShareCode.GetLength() == CODE_STOCK_LENGTH ) 
											   || ( strQuery == QUERY_STRFUTOPT && ( strShareCode.GetLength() == FUTOPT_LENGTH ) ) ) )
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
				else
				{
					CString strSaveCode, strCodeName, strLastCode;
					strSaveCode.Format("%s", szTemp);
					strSaveCode.TrimLeft();
					int nRealMarketType;
					nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(strSaveCode, nRealMarketType);
					if( nMarketType == CMasterTypeInfo::FUTOPT_CHART || nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART )
					{
						strCodeName		= pDataManager->GetCodeInfo("FMASTER", 0, strSaveCode);			
						if(strCodeName.GetLength()<1)
							strCodeName		= pDataManager->GetCodeInfo("PMASTER", 0, strSaveCode);

						if(strCodeName.GetLength()<=0 || strCodeName == strSaveCode)//����� ������ ����
						{
							int nStep;
							((CStdDialog*)m_pStdDialog)->GetFirstCode(strLastCode, nMarketType, nStep);
//							strLastCode = GetLastHistoryCode(CMasterTypeInfo::FUTOPT_CHART);

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

		//==========================================================================
		//LYH �߰� 2006.02.14
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
	}

	if(((CStdDialog*)m_pStdDialog)->m_nChartType >= MINI_MULTI_CHART)
//@091117		if( ((CStdDialog*)GetParent()->GetParent())->m_nChartType==MINI_MULTI_CHART ||
//@091117		   ((CStdDialog*)GetParent()->GetParent())->m_nChartType==MINI_SINGLE_CHART ||
//@091117		   ((CStdDialog*)GetParent()->GetParent())->m_nChartType==DWM_CHART)
	{
		::GetPrivateProfileString(strAppName,"AllCnt","120",szTemp,sizeof(szTemp),lpTitle);
		//==============================================================================
		// ��ȭ ����ȣ ����� ��û
		// ����� ��ü ���� �����ϰ�	
		// ������ 400�� ��ȸ�� ������.
		// 2005.10.14
		//=============================================================================	
		m_lCntForRequest = atol(szTemp);
		if( (m_lCntForRequest<=0) || (m_lCntForRequest>CHTMXREQUEST) )	
			m_lCntForRequest = MINICNTATONCE;
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
		if (m_lCntForRequest > 800)
			m_lCntForRequest = 800;
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
		if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6204) //KHD : �̴�ƽ��Ʈ �϶� ����Ʈ�� TickŸ������ 
			::GetPrivateProfileString(strAppName,"Type","0",szTemp,sizeof(szTemp),lpTitle);
		else
			::GetPrivateProfileString(strAppName,"Type","2",szTemp,sizeof(szTemp),lpTitle);

		if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>SEC_DATA_CHART) 
		{
			szTemp[0] = DAY_DATA_CHART;
		}
		//{{2007.01.25 by LYH 
		BOOL bSetType = TRUE;
		if(nMarketType == CMasterTypeInfo::FUTOPT_CHART && (szTemp[0] == WEEK_DATA_CHART || szTemp[0] == WEEK_DATA_CHART))
			bSetType = FALSE;
		if(nMarketType == CMasterTypeInfo::FOREIGN_CHART && (szTemp[0] == HOUR_DATA_CHART || szTemp[0] == MIN_DATA_CHART))
			bSetType = FALSE;

		if(bSetType)
			SetChartRequestType(szTemp[0]);
		//=====================================================================================
	}

	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART: m_strTickMinDayForRequest = GetTickNumber(); break;
		case SEC_DATA_CHART : m_strTickMinDayForRequest = GetSecNumber(); break;
		case MIN_DATA_CHART : m_strTickMinDayForRequest = GetMinNumber() ; break;			
		case DAY_DATA_CHART : m_strTickMinDayForRequest = GetDayNumber() ; break;
		case HOUR_DATA_CHART: m_strTickMinDayForRequest = GetHourNumber(); break;
		default				: m_strTickMinDayForRequest = "001"			 ; break;  
	}	

	if(m_strTickMinDayForRequest.IsEmpty())
	{
		m_strTickMinDayForRequest = "001";
	}

	// ��, %, �޷��� 3���� ������ �ʿ��Ѵ�. 
	::GetPrivateProfileString(strAppName,"ChartUnitValue","0",szTemp,sizeof(szTemp),lpTitle);	
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

	m_CfgInfo.bRealCandle =  atoi(szTemp) > 0 ? TRUE : FALSE; 
	SetBNormalizedMinuteTimeRule( atoi(szTemp) > 0 ? FALSE : TRUE);//atoi(szTemp)�� 1 �̸� �Ǻ�,  0�̸� �Ǻ�

	//�����ѹ�
	::GetPrivateProfileString(strAppName,	"bShowHLBar"		,"0",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bShowHLBar = atoi(szTemp) ? true : false;
	EnableBoundMark(m_CfgInfo.bShowHLBar);	

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
	::GetPrivateProfileString(strAppName,	"bRevisedValue"		,"1",szTemp,sizeof(szTemp),lpTitle);
	SetUseRevisedValue(atoi(szTemp) ? true : false);
	//m_CfgInfo.bRevisedValue = atoi(szTemp) ? true : false;
	GetParent()->SendMessage(USMG_NOTIFY_REVISED, atoi(szTemp));
	//=================================================================================
	
//<< 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
	::GetPrivateProfileString(strAppName,	"bMonthGapRev"		,"1", szTemp, sizeof(szTemp), lpTitle);
	m_bMonthGapRev = atoi(szTemp) ? true : false;
//>> (Issue No : 0006572)

	if(((CChartMng*)GetParent())->m_strCodeForLoadChart.GetLength() == 0)
	{
		// �������� �ε� : �������� - ojtaso (20070118)
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
		//20110502 �̹��� >>
		//LoadRQInfo(strAppName, lpTitle);
		LoadRQInfo(strAppName, lpTitle, bRQPeriod, lpszCodeForRequest);
		//20110502 �̹��� <<
	}

	return TRUE;
}

BOOL CChartItem::IsAvailibleCode(LPCTSTR lpszCodeName)
{
	return TRUE;
}

// �������� �ε� : �������� - ojtaso (20070118)
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
//void CChartItem::LoadRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle)
void CChartItem::LoadRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle, BOOL bRQPeriod/*=FALSE*/, LPCTSTR lpszCodeForRequest/*=NULL*/)
//20110502 �̹��� <<
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

	IMasterDataManagerLast* pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if( !pDataManager)	return;

	CString strRQ;
	ST_SELCHART* lpSelChart;

	for(int i = 0; i < nCount; i++)
	{
		AfxExtractSubString(strRQ, strRQData, nIndex++, _T(';'));

		lpSelChart = NULL;

		bDefualtRQ = !strRQ.Compare(_T("DEFAULT"));

		if(!bDefualtRQ)
			lpSelChart = new ST_SELCHART;
		else
			m_mapChartItem.Lookup(strRQ, (LPVOID&)lpSelChart);

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
		//20110502 �̹��� >>
		//AfxExtractSubString(lpSelChart->m_strChartGubun, strRQData, nIndex++, _T(';'));
		if(bDefualtRQ && bRQPeriod) 
		{
			{
				if(lpszCodeForRequest)
				{
					CString strCodeForRequest = lpszCodeForRequest;
					lpSelChart->m_strItemCode = strCodeForRequest;
					lpSelChart->m_strItemName = pDataManager->GetCodeName(strCodeForRequest);
					lpSelChart->m_strChartGubun=::GetTrCodeFromCode(GetSafeHwnd(), strCodeForRequest);
				}
			}
			nIndex++;
		}
		else	AfxExtractSubString(lpSelChart->m_strChartGubun, strRQData, nIndex++, _T(';'));
		//20110502 �̹��� <<

		if(bDefualtRQ)
		{
			if(((lpSelChart->m_strChartGubun == QUERY_STRJONGMOK && strShareCode.GetLength() == CODE_STOCK_LENGTH) || 
				(lpSelChart->m_strChartGubun == QUERY_STRFUTOPT && (strShareCode.GetLength() == FUTOPT_LENGTH))) &&
//@091117			if(((lpSelChart->m_strChartGubun == QUERY_STRJONGMOK && (strShareCode.GetLength() == CODE_STOCK_LENGTH)) || 
//@091117				(lpSelChart->m_strChartGubun == QUERY_STRFUTOPT && (strShareCode.GetLength() == FUTOPT_LENGTH)) ||
//@091117				(lpSelChart->m_strChartGubun == QUERY_STRFOREIGN)) &&
				lpChartMng->m_nStatic_RecvCntShareCode > 0 && !strShareCode.IsEmpty() && bChangeCodeByShare
				)
			{
				//20220117 �̹��� >>
				//if( ((CStdDialog*)GetParent()->GetParent())->m_strConfig.Find("__0000") > 0 ||	// ��������ȭ��(0000)
				//	((CStdDialog*)GetParent()->GetParent())->m_strConfig.Find("__99") > 0 )		// ���������ȭ��(9900~9919)
				if (m_pStdDialog->m_strConfig.Find("__0000") > 0 ||	// ��������ȭ��(0000)
					m_pStdDialog->m_strConfig.Find("__99") > 0 )		// ���������ȭ��(9900~9919)
				//20220117 �̹��� <<
				{
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
					//20110502 �̹��� >>
					//AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
					//AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));
					if(bRQPeriod)
					{
						nIndex+=2;
					}
					else
					{					
						AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
						AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));
					}

					if(lpSelChart->m_strItemCode.IsEmpty() || lpSelChart->m_strItemName.IsEmpty())
					{
						lpSelChart->m_strItemCode = strShareCode;
						lpSelChart->m_strItemName = ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(strShareCode);
					}
				}
				else
				{
					if(strRQ == "DEFAULT")
					{
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
						//20110502 �̹��� >>
						//lpSelChart->m_strItemCode = strShareCode;
						//lpSelChart->m_strItemName = ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(strShareCode);
						if(!bRQPeriod || (lpSelChart->m_strItemCode.IsEmpty() || lpSelChart->m_strItemName.IsEmpty()))
						{
							lpSelChart->m_strItemCode = strShareCode;
							lpSelChart->m_strItemName = pDataManager->GetCodeName(strShareCode);
						}
						//20110502 �̹��� <<
						nIndex += 2;
					}
					else
					{
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
						//20110502 �̹��� >>
						//AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
						//AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));
						if(bRQPeriod)
						{
							nIndex+=2;
						}
						else
						{					
							AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
							AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));
						}
						//20110502 �̹��� <<

						if(lpSelChart->m_strItemCode.IsEmpty() || lpSelChart->m_strItemName.IsEmpty())

						{
							lpSelChart->m_strItemCode = strShareCode;
							lpSelChart->m_strItemName = ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(strShareCode);
						}
					}
				}
			}
			else
			{
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
				//20110502 �̹��� >>
				//AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
				//AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));
				if(bRQPeriod)
				{
					nIndex+=2;
				}
				else
				{
					AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
					AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));
				}
				//20110502 �̹��� <<
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
//		lpSelChart->m_cType = strTemp.GetAt(0);
		lpSelChart->m_cType = m_chTypeForRequest;		// �̹� �ֱ⺰ ���Ϸ� �о���� ��Ȳ��.(�ֱ����ϸ� ���� - ___minJipyo___)
		AfxExtractSubString(lpSelChart->m_strTickMinDayNumber, strRQData, nIndex++, _T(';'));
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_lCount = atol(strTemp);
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_nChartNumericValueType = atoi(strTemp);
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
/*
		if(!::strncmp((LPCTSTR)strTemp, "0000000000000000000000", 21))
		{
			delete lpSelChart;
			m_lpSelChart = NULL
			continue;
		}
*/
		int nRealMarketType;
		int nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(lpSelChart->m_strItemCode, nRealMarketType);
		CString strCodeName = ((CStdDialog*)m_pStdDialog)->GetItemNameFromItemCode(lpSelChart->m_strItemCode);
		if( nMarketType == CMasterTypeInfo::FUTOPT_CHART || nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART )
		{
			strCodeName		= pDataManager->GetCodeInfo("FMASTER", 0, lpSelChart->m_strItemCode);			
			if(strCodeName.GetLength()<1)
				strCodeName		= pDataManager->GetCodeInfo("PMASTER", 0, lpSelChart->m_strItemCode);
			
			if(strCodeName.GetLength()<=0 || strCodeName == lpSelChart->m_strItemCode)//����� ������ ����
			{
				int nStep;
				((CStdDialog*)m_pStdDialog)->GetFirstCode(lpSelChart->m_strItemCode, nMarketType, nStep);	
			}
		}

		lpSelChart->m_strKey = strTemp;
		m_mapChartItem.SetAt(strRQ, lpSelChart);
		if(!m_strSelectedRQ.Compare(strRQ))
		{
			m_strSelectedItemCode = lpSelChart->m_strItemCode;
			if(((CStdDialog*)m_pStdDialog)->m_nChartType == SINGLE_CHART)
			{		
				if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
					lpSelChart->m_strChartGubun = QUERY_STRFUTOPT;
				else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
					lpSelChart->m_strChartGubun = QUERY_STRSTOCKFUTOPT;
			}

			m_lpSelChart = lpSelChart;
		}
	}
	
	if(!m_mapChartItem.Lookup(m_strSelectedRQ, (LPVOID&)lpSelChart))
		m_strSelectedRQ = _T("DEFAULT");
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
	// �м��� ���� ���� - ojtaso (20080728)
	//SaveandOpenFromAnalToolFile(strLoadFileName, "", TRUE, FALSE);
	//091102-alzioyes �м��������ƾ ����. ����ɼǵ��� �����ϵ���.
	LSaveandOpenFromAnalToolFile(strLoadFileName, "", TRUE, FALSE);

	// ����� ���� �ҷ�����
	CString szMCTFilePath(lpTitle);
	if(szMCTFilePath.Find(".mct") > 0)
	{
		char szLoadSTFile[MAX_PATH] = {0,};
		if(::GetPrivateProfileString(strAppName,"STFilePath","",szLoadSTFile,MAX_PATH,lpTitle)>0)
			((CStdDialog*)GetParent()->GetParent())->LoadSaveSystemTrading(CString(szLoadSTFile));
	}

	//====================================================================================
	// 05.09.23
	// ��ȭ���� ������ ť�� ���� �׿� ��Ʈ Data��ȸ�� ������ 
	// ������ �� �޴� ��찡 ���� �Ͼ��. ��.��;;
	// �̷� ��� ���� �� ��Ʈ�� �������� ������ ��ȸ�� 
	// ��Ʈ�� ����� �� �׷����� ������ �߻��ϴ� ��
	// �� ������ �ذ��ϱ� ���� ��Ʈ ���� �ִ��� üũ�ؼ� ���ٸ� 
	// �ʿ��� �⺻ ��Ʈ ���� �ִ´�.
	//====================================================================================
	if( !HasGraph("������Ʈ") && !HasGraph("��������Ʈ"))
	{
		AddGraph(0,0,0,"������Ʈ"		,FALSE	,FALSE	,FALSE	); 
		AddGraph(0,0,0,"���� �̵����"	,FALSE	,FALSE	,FALSE	);
		AddGraph(1,0,0,"�ŷ�����Ʈ"		,TRUE	,TRUE	,TRUE	);
		//AddGraph(1,0,0,"�ŷ��� �̵����",FALSE	,FALSE	,FALSE	);
	}
	//====================================================================================


	if( GetAllCnt() < GetNOnePageDataCount() )
		SetNew_OnePageDataCount(GetAllCnt());

	return TRUE;
}

BOOL CChartItem::SaveChartInfo(LPCTSTR lpTitle, BOOL bOnlyJongmok /*=FALSE*/)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartInfo::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	if(!m_bUseInnerControl&&!m_bBuild) return FALSE;

	//@solomon	(0 Row, 0 Col)�� �ش��ϴ� ��Ʈ�� ������.(Winix�� View Matrix �� ���ؼ�.)
	if (((CStdDialog*)m_pStdDialog)->m_nMatrix > 0)
		return FALSE;

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

				bHasChartItemData = HasChartItemData(pData->GetCode(),pData->GetTrCode(), FALSE, FALSE);
			}
			if(bHasChartItemData)
			{
				strTemp.Format("Code%02d",lCntChartItemData);
				strTemp2.Format("TRCode%02d",lCntChartItemData);

				if(bOnlyJongmok && lPos == 0)
				{
					::WritePrivateProfileString(strAppName,strTemp,"",lpTitle);
					//::WritePrivateProfileString(strAppName,strTemp2,pData->GetTrCode(),lpTitle);
					::WritePrivateProfileString(strAppName,strTemp2, "",lpTitle);
				}
				else
				{
					CString szTRCode, szCode;
					szCode = pData->GetCode();

					//@solomon	�̴ϸ�Ƽ��Ʈ�ΰ�� �����ڵ带 �������� ����.
					// �ٽ� ����.
//					if (((CStdDialog*)m_pStdDialog)->m_nChartType == MINI_SINGLE2_CHART)
//						szCode = _T("");

					::WritePrivateProfileString(strAppName, strTemp, (LPCSTR)szCode, lpTitle);

					if (g_pPtUtilMng)
						g_pPtUtilMng->GetTRCodeFromCode(m_nMarketType, "", szTRCode);
					else
						szTRCode = pData->GetTrCode();

					::WritePrivateProfileString(strAppName, strTemp2, (LPCSTR)szTRCode, lpTitle);
				}
				
				lCntChartItemData++;
			}
		}
	}



	strTemp.Format("%d",lCntChartItemData);
	::WritePrivateProfileString(strAppName,"CntOfGraph",strTemp,lpTitle);
	
	strTemp.Format("%d",m_nSaveAllCount);
	if(m_nSaveAllCount <10)
	{
		if(((CStdDialog*)m_pStdDialog)->m_nChartType >= MINI_MULTI_CHART)
			strTemp.Format("%d",MINICNTATONCE);
		else
			strTemp.Format("%d",DEFAULTCNTATONCE);
	}
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
	::WritePrivateProfileString(strAppName,"SecValue",	m_strSec,	lpTitle);		
	::WritePrivateProfileString(strAppName,"MinValue" ,	m_strMin ,	lpTitle);
	::WritePrivateProfileString(strAppName,"DayValue" ,	m_strDay ,	lpTitle);
	
	//05.06.22 by sy, nam
	// ��, %, �޷��� 3���� ������ �ʿ��Ѵ�. 
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

	strTemp.Format("%d", m_CfgInfo.bRecvTimeShare	);
	::WritePrivateProfileString(strAppName,	"bRecvTimeShare"	,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bAutoSave		);
	::WritePrivateProfileString(strAppName,	"bAutoSave"			,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bShowLog			);
	::WritePrivateProfileString(strAppName,	"bShowLog"			,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bRevisedValue	);
	::WritePrivateProfileString(strAppName,	"bRevisedValue"		,strTemp,lpTitle);
	//=================================================================================

//<< 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
	strTemp.Format("%d", m_bMonthGapRev	);
	::WritePrivateProfileString(strAppName,	"bMonthGapRev"		, strTemp, lpTitle);
//>> (Issue No : 0006572)

	if(HasGraph("������Ʈ")) //05.10.26 =============== ���� ��Ʈ�� ���� ���� �����Ѵ�.
	{
		SaveChartSetting(lpTitle);
	}

	// �������� ���� : �������� - ojtaso (20070118)
	SaveRQInfo(strAppName, lpTitle);

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartInfo::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	return TRUE;
}

// �������� ���� : �������� - ojtaso (20070118)
// 	void CChartItem::SaveRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle)
// 	{
// 	#ifdef _WRITELOG
// 		WriteToStringLog("CChartItem::SaveRQInfo::Begin", _T("��Ƽ��Ʈ ����"), 13);
// 	#endif
// 		POSITION pos = m_mapChartItem.GetStartPosition();
// 		CString strKey;
// 		ST_SELCHART* lpSelChart = NULL;
// 
// 		CString strCount, strValue, strTemp;
// 
// 		int nCount = 0;
// 
// 		CString szTRCode;
// 		int nCodeType,nMarketType;
// 		while(pos)
// 		{
// 			m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)lpSelChart);
// 
// 	#ifdef __MULTI_LANG__
// 			//@Solomon-MultiLang:091117 if(!CDRChartOcx::FindRQ(strKey))		continue;
// 	#else		
// 			if(!CDRChartOcx::FindRQ(strKey))
// 				continue;
// 	#endif
// 
// 			nCount++;
// 
// 			if(g_pPtUtilMng)
// 			{
// 				nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode(lpSelChart->m_strItemCode, nCodeType);
// 				g_pPtUtilMng->GetTRCodeFromCode(nMarketType, "", szTRCode);
// 				
// 				strTemp.Format(_T(";%s;%s;%s;%s;%c;%s;%ld;%d;%s"), strKey,
// 					szTRCode,
// 					lpSelChart->m_strItemCode,
// 					lpSelChart->m_strItemName,
// 					lpSelChart->m_cType,
// 					lpSelChart->m_strTickMinDayNumber,
// 					lpSelChart->m_lCount,
// 					lpSelChart->m_nChartNumericValueType,
// 					lpSelChart->m_strKey);
// 			}
// 			else
// 			{
// 				strTemp.Format(_T(";%s;%s;%s;%s;%c;%s;%ld;%d;%s"), strKey,
// 					lpSelChart->m_strChartGubun,
// 					lpSelChart->m_strItemCode,
// 					lpSelChart->m_strItemName,
// 					lpSelChart->m_cType,
// 					lpSelChart->m_strTickMinDayNumber,
// 					lpSelChart->m_lCount,
// 					lpSelChart->m_nChartNumericValueType,
// 					lpSelChart->m_strKey);
// 			}
// 
// 			strValue += strTemp;
// 		}
// 
// 		strCount.Format(_T("%d;%s"), nCount, m_strSelectedRQ);
// 		strCount += strValue;
// 
// 		strTemp.Format(_T("%d"), strCount.GetLength() + m_strSelectedRQ.GetLength() + 1);
// 
// 		::WritePrivateProfileString(lpszAppName,	"RQSIZE"	,strTemp,lpszTitle);
// 		::WritePrivateProfileString(lpszAppName,	"RQ"		,strCount,lpszTitle);
// 	#ifdef _WRITELOG
// 		WriteToStringLog("CChartItem::SaveRQInfo::End", _T("��Ƽ��Ʈ ����"), 13);
// 	#endif
// 	}

void CChartItem::SaveRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveRQInfo::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	POSITION pos = m_mapChartItem.GetStartPosition();
	CString strKey;
	ST_SELCHART* lpSelChart = NULL;

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	CString strDefaultRQ;//20110502 �̹��� >>

	CString strValue, strTemp;
	strValue.Format(_T("%d;%s"), m_mapChartItem.GetCount(), m_strSelectedRQ);
	
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)lpSelChart);

		if(!CDRChartOcx::FindRQ(strKey)) continue;

		strTemp.Format(_T(";%s;%s;%s;%s;%c;%s;%ld;%d;%s"), strKey,
			lpSelChart->m_strChartGubun,
			lpSelChart->m_strItemCode,
			lpSelChart->m_strItemName,
			lpSelChart->m_cType,
			lpSelChart->m_strTickMinDayNumber,
			lpSelChart->m_lCount,
			lpSelChart->m_nChartNumericValueType,
			lpSelChart->m_strKey);
		
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
		//20110502 �̹��� >>
		//strValue += strTemp;
		if(strKey == "DEFAULT")
			strDefaultRQ = strTemp;
		else
			strValue += strTemp;
		//20110502 �̹��� <<			
	}
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	strValue += strDefaultRQ;//20110502 �̹��� >>
	
	strTemp.Format(_T("%d"), strValue.GetLength() + m_strSelectedRQ.GetLength() + 1);
	
	::WritePrivateProfileString(lpszAppName,	"RQSIZE"	,strTemp,lpszTitle);
	::WritePrivateProfileString(lpszAppName,	"RQ"		,strValue,lpszTitle);
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveRQInfo::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
}

BOOL CChartItem::SaveChartSetting(LPCTSTR lpTitle)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartSetting::Begin", _T("��Ƽ��Ʈ ����"), 13);
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

	// �м��� ���� ���� - ojtaso (20080728)
	//SaveandOpenFromAnalToolFile("", strSaveFileName, FALSE, TRUE);
	//091102-alzioyes �м��������ƾ ����. ����ɼǵ��� �����ϵ���.
	LSaveandOpenFromAnalToolFile("", strSaveFileName, FALSE, TRUE);

	// ���� �����ϱ�
	CString szMCTFilePath(lpTitle);
	if(szMCTFilePath.Find(".mct") > 0)
	{
		CString szSaveSTFile;
		((CStdDialog*)GetParent()->GetParent())->LoadSaveSystemTrading(szSaveSTFile, m_lRowIndex, m_lColIndex, TRUE);
		::WritePrivateProfileString(strAppName,"STFilePath",szSaveSTFile,lpTitle);
	}

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartSetting::End", _T("��Ƽ��Ʈ ����"), 13);
#endif

	return TRUE;
}

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
BOOL CChartItem::SaveChartInfo4PeriodRQ(LPCTSTR lpTitle, BOOL bOnlyJongmok /*=FALSE*/)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartInfo4PeriodRQ::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	if(!m_bUseInnerControl&&!m_bBuild) return FALSE;

	long lSize = m_arrayChartItemData.GetSize();
	CString strAppName;
	CString strTemp;	
	CString strTemp2;	
	strAppName = "0000";

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
				CString strCode		= pData->GetCode();
				CString strTRCode	= pData->GetTrCode();

				bHasChartItemData = HasChartItemData(pData->GetCode(),pData->GetTrCode(), FALSE, FALSE);
			}
			if(bHasChartItemData)
			{
				strTemp.Format("Code%02d",lCntChartItemData);
				strTemp2.Format("TRCode%02d",lCntChartItemData);

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
	strTemp.Format("%d", m_lCntForRequest);
	

	if(m_nSaveAllCount <10)
		strTemp.Format("%d",DEFAULTCNTATONCE);
	::WritePrivateProfileString(strAppName,"AllCnt",strTemp,lpTitle);
	
// 	strTemp.Format("%d", m_nNewSavePageCount);	
	if(atoi(strTemp) <1) strTemp.Format("%d",200);
	::WritePrivateProfileString(strAppName,"CntInView",strTemp,lpTitle);


	strTemp.Format("%c",m_chType);
	::WritePrivateProfileString(strAppName,"Type",strTemp,lpTitle);
	
	//=================================================================================
	// 2005. 05. 23 added  by nam	- ƽ�п� ���� ������ �����Ѵ�.
	// 2005. 09. 12 edited by nam	- ƽ�п� ��� �����Ѵ�.
	//=================================================================================
	::WritePrivateProfileString(strAppName,"TickValue",	m_strTick,	lpTitle);		
	::WritePrivateProfileString(strAppName,"SecValue",	m_strSec,	lpTitle);		
	::WritePrivateProfileString(strAppName,"MinValue" ,	m_strMin ,	lpTitle);
	::WritePrivateProfileString(strAppName,"DayValue" ,	m_strDay ,	lpTitle);
	
	//05.06.22 by sy, nam
	// ��, %, �޷��� 3���� ������ �ʿ��Ѵ�. 
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

	strTemp.Format("%d", m_CfgInfo.bRecvTimeShare	);
	::WritePrivateProfileString(strAppName,	"bRecvTimeShare"	,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bAutoSave		);
	::WritePrivateProfileString(strAppName,	"bAutoSave"			,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bShowLog			);
	::WritePrivateProfileString(strAppName,	"bShowLog"			,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bRevisedValue	);
	::WritePrivateProfileString(strAppName,	"bRevisedValue"		,strTemp,lpTitle);
	//=================================================================================

	// �������� ���� : �������� - ojtaso (20070118)
	SaveRQInfo(strAppName, lpTitle);

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartInfo::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	return TRUE;
}
//20110502 �̹��� <<

BOOL CChartItem::RemoveChartItemData(LPCTSTR lpCode, CString strQueryType) 
{
	CChartItemData *pChartItemData = NULL;
	long lIndex = GetPosChartItemData(strQueryType,lpCode);
	if(lIndex==-1) return FALSE;
	pChartItemData = m_arrayChartItemData.GetAt(lIndex);
	delete pChartItemData;
	pChartItemData = NULL;
	m_arrayChartItemData.RemoveAt(lIndex);
	return TRUE;
}

BOOL CChartItem::RemoveChartItem_OneData(LPCTSTR lpCode, CString strQueryType) 
{
	CChartItemData *pChartItemData = NULL;		

		
	for(int nPos=0; nPos <m_arrayMarketTRData.GetSize() ;nPos++)
	{
		pChartItemData = NULL;
		pChartItemData = m_arrayMarketTRData.GetAt(nPos);

		if(pChartItemData == NULL) continue;

		CString sT1 = pChartItemData->GetTrCode();
		CString sT2 = pChartItemData->GetCodeForRqstFromChartItemData();

		if(sT1==strQueryType
			&&sT2==CString(lpCode))
		{
			if(nPos==-1)		
			{
				pChartItemData = NULL;
				continue;
			}
#ifdef _DEBUG
			CString sTemp;
			sTemp.Format("���� ��ǥ ���� %s__%s", lpCode, strQueryType);
			OutputDebugString(sTemp);
#endif
			
			delete pChartItemData;
			pChartItemData = NULL;
			m_arrayMarketTRData.RemoveAt(nPos);
		}
	}
	
	return TRUE;
}




BOOL CChartItem::DestoryItemData()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestoryItemData::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	m_bReceivedDataChart = FALSE; //05.09.22

	CChartItemData *pChartItemData = NULL;
#ifdef _WRITELOG
	WriteToStringLog("m_arrayChartItemData::RemoveAll::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pChartItemData = m_arrayChartItemData.GetAt(nPos);
		delete pChartItemData;
	}
	m_arrayChartItemData.RemoveAll();
#ifdef _WRITELOG
	WriteToStringLog("m_arrayChartItemData::RemoveAll::End", _T("��Ƽ��Ʈ ����"), 13);
#endif

	//==============================================================
	//���� ��ǥ ���� TR Array�� �����Ѵ�.
	//==============================================================
	int nPos = 0;
	nSize = m_arrayMarketTRData.GetSize();
	for(nPos=0;nPos<nSize;nPos++)
	{
		pChartItemData = m_arrayMarketTRData.GetAt(nPos);
		delete pChartItemData;
	}
	m_arrayMarketTRData.RemoveAll();
	//==============================================================
#ifdef _WRITELOG
	WriteToStringLog("m_arrayMarketTRData::RemoveAll::End", _T("��Ƽ��Ʈ ����"), 13);
#endif

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestoryItemData::End", _T("��Ƽ��Ʈ ����"), 13);
#endif	
	return TRUE;
}

CChartItemData *CChartItem::FindChartItemDataForQuery()
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	
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
		((CChartMng*)GetParent())->MoveCharts(m_lRowIndex,m_lColIndex);
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

CChartItemData* CChartItem::GetChartItemDataNew(CString strTRCode,LPCTSTR lpCodeForQuery, BOOL bCreateIfNoExist /*= TRUE*/,BOOL bIsPrimaryData /*= FALSE */)
{
	CString strCodeForQuery;
	CChartItemData *pItemData = NULL;
	if (lpCodeForQuery!=NULL)
		strCodeForQuery = lpCodeForQuery;

	if (strCodeForQuery.IsEmpty()) return NULL;

	int nSize = m_arrayChartItemData.GetSize();	
	if (bIsPrimaryData)
	{
		// Clear the same ItemData Without PrimaryData..
		for(int nPos=1;nPos<nSize;nPos++)
		{
			pItemData = m_arrayChartItemData.GetAt(nPos);
			if (pItemData->GetTrCode()==strTRCode && pItemData->GetCode()==strCodeForQuery)
			{
				delete pItemData;
				pItemData = NULL;
				m_arrayChartItemData.RemoveAt(nPos);
				nSize--;
			}
		}

		if (nSize > 0)
		{
			pItemData = m_arrayChartItemData.GetAt(0);
			if (pItemData->GetTrCode()==strTRCode)	// if Primary Data 's TrCode is the same as New Data's , ...
			{
				ClearData();
				pItemData->SetCodeForQuery(strCodeForQuery);
			}
			else
			{
				//{{2007.05.30 by LYH ���� ���� ���� ���� �߰� �ϵ��� ����
				ClearData();
				pItemData->SetTRCode(strTRCode);
				pItemData->SetCodeForQuery(strCodeForQuery);
				//}}
			}

			return pItemData;
		}
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
		pItemData->SetCodeForQuery(strCodeForQuery);	
		m_arrayChartItemData.Add(pItemData);

		return pItemData;
	}

	return NULL;	
}

long CChartItem::GetPosChartItemData(CString strTrCode, LPCTSTR lpCodeForQuery)
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();	
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		if (pItemData->GetTrCode()==strTrCode && pItemData->GetCode()==lpCodeForQuery)
			return nPos;
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

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
//BOOL CChartItem::SaveandOpenFromFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
BOOL CChartItem::SaveandOpenFromFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption, BOOL bOnReceive/*=FALSE*/)
//20110502 �̹��� <<
{
	if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6401)
	{
		return FALSE;
	}
	if(bSaveOption)
	{
		RemoveGraph(0, 0, "�ŵ�ü��");
		RemoveGraph(0, 0, "�ż�ü��");

//		RemovePacket("ü���Ͻ�");
		RemovePacket("�ŵ��Ǽ�");
		RemovePacket("�ŵ��ܰ�");
		RemovePacket("�ż��Ǽ�");
		RemovePacket("�ż��ܰ�");
	}
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

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	//20110502 �̹��� >>
//	if( (_T("temp.tmp") != sSaveFile) && sSaveFile.GetLength() )
//	{
//		if(::IsWindow(GetParent()->GetSafeHwnd()))
//		{
//			int nCnt = GetParent()->SendMessage(UMSG_GETZOOMVALUE, GetTypeRequested());
//			if(nCnt > 0)
//				CKBChart::SetNOnePageDataCount(nCnt);
//		}		
//	}
	if(bOnReceive == FALSE)
	{	
		if( (_T("temp.tmp") != sSaveFile) && sSaveFile.GetLength() )
		{
			if(::IsWindow(GetParent()->GetSafeHwnd()))
			{
				int nCnt = GetParent()->SendMessage(UMSG_GETZOOMVALUE, GetTypeRequested());
				if(nCnt > 0)
					CDRChartOcx::SetNOnePageDataCount(nCnt);
			}		
		}
	}
	//20110502 �̹��� <<
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
		{
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
			//20110502 �̹��� >>
			//SetNew_OnePageDataCount(GetAllCnt());
			if(bOnReceive == FALSE)
				SetNew_OnePageDataCount(GetAllCnt());
			//20110502 �̹��� <<
		}
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

// 	// �м��� ���� ���� - ojtaso (20080728)
// 	BOOL CChartItem::SaveandOpenFromAnalToolFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
// 	{
// 		if(!((CChartMng*)GetParent())->GetSaveAnalTool())
// 			return FALSE;
// 
// 		if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6401)
// 			return FALSE;
// 
// 		BOOL bRetValue = CDRChartOcx::SaveandOpenFromAnalToolFile( strOpenFileName, strSaveFileName, bOpenOption, bSaveOption);	
// 
// 		CDRChartOcx::SetStrUserIndicatorListFileName("");
// 		CDRChartOcx::SetStrUserEnvironmentFileName("");
// 
// 		return bRetValue;
// 	}

// �м��� ���� ���� - ojtaso (20080728)
//091102-alzioyes �м��������ƾ ����. ����ɼǵ��� �����ϵ���.
BOOL CChartItem::LSaveandOpenFromAnalToolFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
{
	if(strOpenFileName==NULL && strlen(strOpenFileName)==0) return FALSE;

	BOOL bSaveAnalTool = ((CChartMng*)GetParent())->GetSaveAnalTool();
	if(!bSaveAnalTool) return TRUE;
	
	BOOL bRetValue = CDRChartOcx::SaveandOpenFromAnalToolFile( strOpenFileName, strSaveFileName, bOpenOption, bSaveOption);	
	
	CDRChartOcx::SetStrUserIndicatorListFileName("");
	CDRChartOcx::SetStrUserEnvironmentFileName("");

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
	{
		if(((CStdDialog*)m_pStdDialog)->m_nChartType >= MINI_MULTI_CHART)
			nCntForRequest = MINICNTATONCE;	 //05.10.14
		else
			nCntForRequest = DEFAULTCNTATONCE;	 //05.10.14
	}

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

	m_CfgInfo.bRealCandle = bUse ? false : true;
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
				CString strTRCode = pChartItemData->GetTrCode();
				CString strCode = pChartItemData->GetCode();
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
//@100119	SetNew_OnePageDataCount(120);

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
	return m_chTypeForRequest;
}

void CChartItem::LoadTickMinDayInfo(CString stCfgFile)
{
	CString strAppName;
	CString strTemp;
	CString strTemp2;
	char szTemp[128] = {0,};

	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);	
    if(((CStdDialog*)m_pStdDialog)->m_nScreenID == nScreenID_6204) //KHD : �̴�ƽ��Ʈ �϶� ����Ʈ�� TickŸ������ 
		::GetPrivateProfileString(strAppName,"Type","0",szTemp,sizeof(szTemp),stCfgFile);
	else
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
	//	case SEC_DATA_CHART:
			::GetPrivateProfileString(strAppName,"SecValue",	"005",	szTemp,sizeof(szTemp),stCfgFile);
			SetSecNumber(CString(szTemp));
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
		case SEC_DATA_CHART:	m_strTickMinDayForRequest = GetSecNumber();		break;
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
		else if(nChartType == 6)
			return GetSecNumber();
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
		case SEC_DATA_CHART:			
			return GetSecNumber();
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

CString CChartItem::GetSecNumber()	
{
	return m_strSec;
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

void	CChartItem::SetSecNumber	(CString strValue)
{
	if( atoi(strValue) <= 0)
	{
		strValue = "005";
	}
	m_strTickMinDay = m_strSec	= strValue;
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

// gm0604 added to Update System Trading Data (2005.7.21)
BOOL CChartItem::ReceiveFromSTDataManager()
{
	if(m_lPtrST&&m_bBuild)
	{
		InvokeAddIn("ChartStrategyAddin.dll",1,m_lPtrST);
	}
	return TRUE;	
}

void CChartItem::SetPtrST(long lPtr, LPCTSTR lpSTName)
{
	m_lPtrST = lPtr;
	m_strName = lpSTName;
}

// gm0604 added to clear System Trading Info 2005.10.10
void CChartItem::ClearPtrST()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::ClearPtrST::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	if(m_lPtrST)
	{
		CStringArray arrayGraph;
		InvokeAddIn("ChartStrategyAddin.dll",8,(long)&arrayGraph);
		int nSize = arrayGraph.GetSize();
		for(int i=0;i<nSize;i++)
		{
			RemoveChart(arrayGraph.GetAt(i));
		}
	}
	m_lPtrST = NULL;// gm0604 added 2005.9.6

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::ClearPtrST::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
}
// gm0604 added to Update System Trading Data (2005.7.21)


void CChartItem::SetChartRequestType(char chType)
{
	m_chTypeForRequest = chType;
}


CString CChartItem::GetReplayFlag()
{
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	int nResult = GetParent()->GetParent()->SendMessage(UMSG_GET_CHARTMODE, 1);
	if(nResult == REPLAY_ITEM)
	{
		return _T("1");
	}
	else 
		return _T("0");
}

CString CChartItem::GetReplayStartTime()
{
	CString strDate;
	int nDate = GetParent()->GetParent()->SendMessage(UMSG_GET_REPLAY_STARTTIME);
	strDate.Format("%04d00", nDate);
	return strDate;	
}

//05.10.14
CString CChartItem::GetReplayDayBefore()
{
	CString strDayBefore;
	int nDay = GetParent()->GetParent()->SendMessage(UMSG_GET_REPLAY_DAYBEFORE);
	strDayBefore.Format("%02d", nDay);
	return strDayBefore;
}

void CChartItem::Init_ChartCfgInfo()
{
	m_CfgInfo.bAutoSave			= true	;
	m_CfgInfo.bRealCandle		= true	;
	//SetNew_BNormalizedMinuteTimeRule(FALSE); //�Ǻ��� �⺻�̴�.
	m_CfgInfo.bRecvTimeShare	= false	;
	m_CfgInfo.bShowHLBar		= false;//true	;	
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

CString CChartItem::GetOldChartCode() 
{ 
	if(!m_bAfterReceiveData) return "";

	return m_strOldCode; 
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
	//SYGENE ȣ������ ��� ���� by LYH 2006.12.20
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
//	if(lRowIndex < -1)
//		OutputDebugString("aa");

	m_lColIndex = lColIndex;
	m_lRowIndex = lRowIndex;
}

BOOL CChartItem::SetPacketData(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset)
{
	{
		char aDataCnt[6]={0,};
		memcpy(aDataCnt, (char*)szData, 5);
		long dwReceiveCnt = atol(aDataCnt);
		long dwReqCnt = GetCntForRequest();
		m_bReqCntIsMore = (dwReceiveCnt>=dwReqCnt)? TRUE:FALSE;
	}

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
		else if(chType==SEC_DATA_CHART)
		{
			strType = "��";
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

//		if(g_pDebugMng) 
//			g_pDebugMng->Trace("%s TR[%s]", "CChartItem::SetPacketData", "Before");
		//return CDRChartOcx::SetPacketData(szPacketName,szData,nLength,bReset);
		BOOL bRet = CDRChartOcx::SetPacketData(szPacketName,szData,nLength,bReset);
//		if(g_pDebugMng) 
//			g_pDebugMng->Trace("%s TR[%s]", "CChartItem::SetPacketData", "End");

		return bRet;
	}
	return TRUE;
	
}


void CChartItem::RemoveAllMarketData()
{
	CChartItemData *pChartItemData = NULL;
	for(int nPos=0;nPos<m_arrayMarketTRData.GetSize();nPos++)
	{
		pChartItemData = NULL;
		pChartItemData = m_arrayMarketTRData.GetAt(nPos);										
		if(pChartItemData)
		{
			CString strTRCode = pChartItemData->GetTrCode();
			CString strCode = pChartItemData->GetCode();
			
			delete pChartItemData;
			pChartItemData = NULL;
		}
	}

	m_arrayMarketTRData.RemoveAll();
}


void CChartItem::SetNew_OnePageDataCount(int nPCnt)
{
	if(CDRChartOcx::GetNOnePageDataCount()==nPCnt)
		return;

	//SetNOnePageDataCount(nPCnt); <== ����ÿ� �Ӽ������θ� ������ ī��Ʈ�� �����ϱ� ����Ѵ�.
	CDRChartOcx::SetOnePageDataCount(nPCnt); //<== ���� ������ ī��Ʈ�� �����Ѵ�.
#ifdef _DEBUG
	CString sTemp;
	sTemp.Format("SetNew_OnePageDataCount : %d", nPCnt);
	OutputDebugString(sTemp);
#endif
}


CChartItemData* CChartItem::GetChartItemMarketDataNew(CString strTRCode,LPCTSTR lpCodeForQuery)
{
	CString strCodeForQuery;
	CChartItemData *pItemData = NULL;
	if(lpCodeForQuery!=NULL)
	{
		strCodeForQuery = lpCodeForQuery;
	}	
	if(strCodeForQuery.IsEmpty()) return NULL;

	int nSize = m_arrayMarketTRData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayMarketTRData.GetAt(nPos);
		if(pItemData->GetTrCode()==strTRCode
			&&pItemData->GetCodeForRqstFromChartItemData()==strCodeForQuery)
		{
			delete pItemData;
			pItemData = NULL;
			m_arrayMarketTRData.RemoveAt(nPos);
			nSize--;
		}
	}

	
	pItemData = new CChartItemData(this,strTRCode);
	pItemData->SetCodeForQuery(strCodeForQuery); //��û�� �����ڵ�(���� ��ǥ��) Set	
	m_arrayMarketTRData.Add(pItemData);
#ifdef _DEBUG
		CString sTemp;
		sTemp.Format("GetChartItemDataNew::m_arrayMarketTRData ���� %d", m_arrayMarketTRData.GetSize());
		OutputDebugString(sTemp);
#endif 
	
	return pItemData;
}


CString CChartItem::GetMarketQueryData(CString strJipyo, int nCntRqst, BOOL bIsNew)
{
	if(	m_strDateForRequest.IsEmpty()	// �˻�������
		||GetCntForRequest()==NULL		// �˻��� ������ ����
		||m_chTypeForRequest==NULL		// ƽ(0)/��(1)/��(2)/��(3)/��(4)
		||strJipyo.GetLength() == 0		
		) 
	{
		return "";
	}

	for(int i = 0; i < m_arrayMarketTRData.GetSize(); i++)
	{
		CChartItemData *pChartItemData = m_arrayMarketTRData.GetAt(i);
		if(pChartItemData && pChartItemData->GetCodeForRqstFromChartItemData() == strJipyo)
		{
			pChartItemData->SetBNextMarketData(bIsNew ? FALSE : TRUE); //bIsNew�� TRUE�� �ű���ȸ �̹Ƿ� ���ڴ� False�� �ѱ��.
			return pChartItemData->GetMarketQueryData_ChartItemData(bIsNew ? FALSE : TRUE, nCntRqst);
		}
	}
	return "";
}

long CChartItem::ReceiveMarketChartData(void* szData, long nLength)
{	
	CString strJMCode;
	CChartItemData *pChartItemData = NULL;
	
	CString strData;
	strData.Format("%*.*s", nLength, nLength, szData);
	m_strArrRecvMarketTRData.Add(strData);
	
	CString strPacketName, strCode;	
	for (int i = 0 ; i < m_arrayMarketTRData.GetSize() ; i++)
	{
		pChartItemData = m_arrayMarketTRData.GetAt(i);
		
		for (int j = 0; j < m_strArrRecvMarketTRData.GetSize(); j++)
		{
			CString strRecvData = m_strArrRecvMarketTRData.GetAt(j);
			if (!strRecvData.GetLength()) continue;

			if (pChartItemData->GetTrCode() == QUERY_STRMARKET)
			{
				KB_d1015_OutRec1* pOutRec1 = (KB_d1015_OutRec1*)strRecvData.GetBuffer(strRecvData.GetLength());
				strPacketName.Format("%100.100s", pOutRec1->dataNm);  strPacketName.TrimRight();
				strCode.Format("%8.8s"			, pOutRec1->shcode);	 strCode.TrimRight();
			}

			if (pChartItemData->GetCodeForRqstFromChartItemData() != strPacketName)
				continue;	

			m_strArrRecvMarketTRData.RemoveAt(j);
		
			if (!pChartItemData)
			{
#ifdef _DEBUG
				CString sTemp;
				sTemp.Format("%d", pChartItemData);
				AfxMessageBox("CChartItem::ReceiveMarketChartData \n pChartItemData pointer is NULL\n" + sTemp);
#endif
				return 0;
			}

			BOOL bRetValue = pChartItemData->MakeChartData(szData,nLength);
			if (!bRetValue)
				return 0;

			CString strTrCode = _T("");
			strTrCode = pChartItemData->GetTrCode();
			
			if (bRetValue)
			{
				if (!BuildMaketChart(pChartItemData))
					return 0;

				return 1; 			
			}
		}		
	}

	m_strArrRecvMarketTRData.RemoveAll();	

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	
//	E-Mail	:	
//	����	:	2005.11.12
//	����	:	���� �����ͷ� ���� ��ǥ ��Ʈ�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::BuildMaketChart(CChartItemData* pChartItemData)
{
	m_bBuild = TRUE;
	long lSize = 0;	
	
	// 2005. 05. 18 add by sy, nam ===========================================
	// ��ø�� ��� ��� ������ ��ø�Ǵ��� �� �� ���� �����Ѵ�.
	if(m_bOverLapping == FALSE)
		m_nCntOverLap = 0;

	pChartItemData->m_nCntOverLap = m_nCntOverLap;
	//========================================================================
	if (m_nSmallChartMode==1)
		pChartItemData->AddPacketAndGraph(TRUE);
	else
		pChartItemData->AddPacketAndGraph();

	m_bInitalChart = TRUE;

//	ShowInnerToolBar(TRUE);

	BOOL bNextOld = m_bNext;	
	BOOL bRetValueOfSetPacketData = pChartItemData->SetPacketDataInAdditionPacket(!m_bNext);

//	if(m_nChartNumericValueType == WON_TYPE)
//	{
//		//2005. 09. 13 pm 06:26 �����븮 ��û���� �� ����  ChangePriceGraphDrawingData(TRUE,TRUE,TRUE,TRUE,FALSE,1);	
//		SetBShowPriceRateInquiry(TRUE);
//	}
//	else
//	{
//		//2005. 09. 13 pm 06:26 �����븮 ��û���� �� ����  ChangePriceGraphDrawingData(TRUE,TRUE,TRUE,TRUE,FALSE,1);
//		SetBShowPriceRateInquiry(FALSE);
//	}

	if (!bRetValueOfSetPacketData) return FALSE;

	Invalidate();

	return TRUE;
}

void CChartItem::SetBReplayChartMode(BOOL bReplayMode)
{ 
	m_bReplaychartMode = bReplayMode;	

	if( bReplayMode)
	{
		m_strReplayTime = _T("������ �����Դϴ�                         ");		
	}
	else
	{
		m_strReplayTime = _T("");
	}	
}

//=====================================================================
//���� ��� �϶� ���� �ð��� �׸���.
//05. 11. 16
//=====================================================================
//void CChartItem::DrawReplayTime(CPaintDC* pDC)
//{	
//	if(m_bReplaychartMode)
//	{
//		//CDC* pReplayDC = GetDC();
//		pDC->DrawText(m_strReplayTime, m_rectReplay, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
//		//ReleaseDC(pReplayDC);
//	}	
//}

void CChartItem::SetReplayChart_RealData(CString strCnts, CString strPrice, CString strVolume)
{
	m_strReplayTime			= strCnts;
	m_strCurPrcForReplay	= strPrice;		m_strCurPrcForReplay.TrimLeft(); m_strCurPrcForReplay.TrimRight();
	
	strVolume.Remove('-'); strVolume.TrimLeft(); strVolume.TrimRight();
	m_strVolumeForReplay.Format("%ld", atol(m_strVolumeForReplay) + atol(strVolume));

	m_strChrateForReplay.Format("%.2f", atof(m_strCurPrcForReplay) / atof(m_strPrePrcForReplay) * 100  - 100.00);

	m_strReplayTime = strCnts;
	

	CRect rectTemp;
	if(!m_rectTitleJongmokPast.IsRectEmpty())
	{
		if(rectTemp.SubtractRect(m_rectTitleJongmokPast,m_rectTitleJongmok))
		{
//			OutputDebugString(strCnts + "KKK...........1");
			GetParent()->InvalidateRect(rectTemp);
		}		
		m_rectTitleJongmokPast.SetRectEmpty();
	}
	GetParent()->InvalidateRect(m_rectTitleJongmok);

	if(!m_rectTitleTypePast.IsRectEmpty())
	{
		if(rectTemp.SubtractRect(m_rectTitleTypePast,m_rectTitleType))
		{
//			OutputDebugString(strCnts + "KKK...........2");
			GetParent()->InvalidateRect(rectTemp);
		}		
		m_rectTitleTypePast.SetRectEmpty();
	}
	GetParent()->InvalidateRect(m_rectTitleType);
	
	if(!m_rectTitlePosPast.IsRectEmpty())
	{
		if(rectTemp.SubtractRect(m_rectTitlePosPast,m_rectTitlePos))
		{
//			OutputDebugString(strCnts + "KKK...........3");
			GetParent()->InvalidateRect(rectTemp);
		}
		m_rectTitlePosPast.SetRectEmpty();
	}
	//GetParent()->InvalidateRect(m_rectTitlePos);	������Ʈ Ÿ��Ʋ �׸��� ���������� ���� by LYH
	GetParent()->Invalidate();


	//CDC* pDC = GetDC();
	//pDC->DrawText(strCnts, rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
/*	if(m_pDCForReplay == NULL)
		m_pDCForReplay = GetDC();

	if( m_pDCForReplay == NULL ) return;

	m_pDCForReplay->DrawText(strCnts, rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	//m_pDCForReplay->TextOut(rect->left, rect->top, strCnts);

	CClientDC dc(this);
    CPen pen, *pOldPen;

        

    pen.CreatePen(PS_SOLID, 1, RGB(0,255,0));

    dc.SelectObject(GetStockObject(NULL_BRUSH));

   // dc.SetROP2(R2_XORPEN);

    pOldPen = (CPen*)dc.SelectObject(&pen);

    dc.SetBkMode(TRANSPARENT);

    dc.SetTextColor(RGB(255,0,0));



    //CRect rc;

    //rc.left = point.x; rc.top = point.y;

    //dc.DrawText(strCnts,-1,rect, DT_CALCRECT);

    dc.DrawText(strCnts,-1,rect, DT_SINGLELINE);

    //dc.Rectangle(rect);

    dc.SelectObject(pOldPen);

	//InvalidateRect(rect);*/


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


void CChartItem::DrawReplayChartTitle(CRect rectIn,CDC* pDC,CFont *pFt, CFont *pFtNormal, COLORREF crBackIn )
{
	CRect rect = rectIn;
//	rect.left += SIZEOFSIGNAL + SIZEOFCHECK + 1;
//	rect.right -= (WIDTHOFTITLEBUTTON+POSXOFMAXMIZE);

	//Han Hwa
	//�ֽ� ��Ʈ�� ��ȣ ���� 
	//if( m_signal.GetSafeHwnd() && (m_nMarketType == CMasterTypeInfo::STOCK_CHART) && m_bUseInnerControl) 
	//{ 
	//	rect.left += SIZEOFSIGNAL;		
	//}

//@��������
//	if(m_Check.GetSafeHwnd())
//	{
//		rect.left += SIZEOFCHECK;
//	}
//	rect.left += 1;
//@��������

	if(m_btInit.GetSafeHwnd())
	{
		rect.right -= WIDTHOFTITLEBUTTON;
	}
	if(m_btMaxmize.GetSafeHwnd() && m_btMaxmize.IsWindowVisible())
	{
		rect.right -= POSXOFMAXMIZE;
	}	

	COLORREF crText = NULL;
	COLORREF crTextSise = NULL;
	COLORREF crBackSise = NULL;
	
	crText = CLR_FONT_DEFAULT;
	crTextSise = CLR_FONT_DEFAULT;
	
	
	CFont *pFtDefault = (CFont *)pDC->SelectObject(pFt);

	CRect rectTitlePos;
	CRect rectTitleType1;	
	CRect rectTitleType2;
//	CRect rectTitleJongmok;

	CRect rectTitleName;
//	CRect rectTitlePrice;
//	CRect rectTitleChange;
//	CRect rectTitleRate;
//	CRect rectTitleVolume;

	CString strTitlePos;//[1/1]
	LPSTR lpszTitlePos = new char[10];
	::memset(lpszTitlePos, 0x00, 10);

	CString strTitleName;//�����


	CString strTitleType;

	long lCols = ((CChartMng*)GetParent())->GetCols();
	long lCntOfChart = ((CChartMng*)GetParent())->GetCntOfChart();
	::sprintf(lpszTitlePos, "[%d / %d]", m_lColIndex + 1 + m_lRowIndex*lCols,lCntOfChart);

	CSize szTitlePos = pDC->GetOutputTextExtent(lpszTitlePos);
	rectTitlePos.SetRect(rect.right-szTitlePos.cx,rect.top+4,rect.right,rect.bottom);

	BOOL bShortTitleMode = FALSE;
	if(!m_strReplayTime.IsEmpty())
	{		
		m_strReplayTime.TrimRight();
		if(m_strChrateForReplay.GetLength()>0)
			strTitleName.Format(" %s  %s (%s%%) %s   ",m_strReplayTime, ::SetComma(m_strCurPrcForReplay), m_strChrateForReplay, ::SetComma(m_strVolumeForReplay));		
		else
			strTitleName.Format(" %s", m_strReplayTime);

		CSize szTitleName = pDC->GetOutputTextExtent(strTitleName);
		
		
		CRect rectShift;
		CRect rectUnion;
		CRect rectUnionTitle, rectTitleJongmok;
		BOOL  bSetEmpty = FALSE;
		
		rectUnionTitle.SetRectEmpty();
		rectShift = rect;
		rectShift.top += 4;
		rectShift.OffsetRect(1,0);

		rectTitleName.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleName.cx,rectShift.top + szTitleName.cy);
		rectUnionTitle.UnionRect(rectUnionTitle,rectTitleName);						
		
		rectTitleJongmok = rectUnionTitle;
		rectTitleJongmok.InflateRect(2,3);
		if(rectTitleJongmok!=m_rectTitleJongmok)
		{
			m_rectTitleJongmokPast = m_rectTitleJongmok;
			m_rectTitleJongmok = rectTitleJongmok;		
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

	if(chType!=NULL)
	{		
		// ƽ(0)/��(1)/��(2)/��(3)/��(4)
		switch(chType)
		{
		case TICK_DATA_CHART:
			strTitleType.Format("[%sƽ]",(LPTSTR)(LPCTSTR)m_strTick);
			break;
		case MIN_DATA_CHART:
			if("300" == m_strMin)
				strTitleType.Format("[30��]");
			else
				strTitleType.Format("[%s��]",(LPTSTR)(LPCTSTR)m_strMin);
			break;
		case DAY_DATA_CHART:
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
	}	

	if(!crBackSise)
	{
		pDC->SetBkMode(TRANSPARENT);		
	}	
	else
	{
		pDC->SetBkMode(OPAQUE);				
		pDC->SetBkColor(crBackSise);
	}
	pDC->SetTextColor(crTextSise);


	if(!m_strReplayTime.IsEmpty())
	{
		CRect rectInflate;
		if(!crBackSise)
		{
			rectInflate = rectTitleName;
			rectInflate.InflateRect(1,3);
			//@Solomon100224 pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);
		}
		else
		{
			rectInflate = rectTitleName;
			rectInflate.InflateRect(0,1);
			pDC->FillSolidRect(rectInflate,crBackSise);
		}
		pDC->TextOut(rectTitleName.left,rectTitleName.top, strTitleName);
		/*
		if(!crBackSise)
		{
			if(!rectTitlePrice.IsRectEmpty())
			{
				rectInflate = rectTitlePrice;
				rectInflate.InflateRect(1,3);
				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);
				pDC->TextOut(rectTitlePrice.left,rectTitlePrice.top,strTitlePrice);		
			}
			if(!rectTitleChange.IsRectEmpty())
			{
				rectInflate = rectTitleChange;
				rectInflate.InflateRect(1,3);
				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);
				pDC->TextOut(rectTitleChange.left,rectTitleChange.top,strTitleChange);
			}
			if(!rectTitleRate.IsRectEmpty())
			{
				rectInflate = rectTitleRate;
				rectInflate.InflateRect(1,3);
				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);
				pDC->TextOut(rectTitleRate.left,rectTitleRate.top,strTitleRate);
			}
			if(!rectTitleVolume.IsRectEmpty())
			{
				rectInflate = rectTitleVolume;
				rectInflate.InflateRect(1,3);
				pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);		
				pDC->TextOut(rectTitleVolume.left,rectTitleVolume.top,strTitleVolume);
			}
		}
		else
		{
			if(!rectTitlePrice.IsRectEmpty())
			{
				rectInflate = rectTitlePrice;
				rectInflate.InflateRect(0,1);
				pDC->FillSolidRect(rectInflate,crBackSise);
				pDC->TextOut(rectTitlePrice.left,rectTitlePrice.top,strTitlePrice);		
			}
			if(!rectTitleChange.IsRectEmpty())
			{
				rectInflate = rectTitleChange;
				rectInflate.InflateRect(0,1);
				pDC->FillSolidRect(rectInflate,crBackSise);
				pDC->TextOut(rectTitleChange.left,rectTitleChange.top,strTitleChange);
			}
			if(!rectTitleRate.IsRectEmpty())
			{
				rectInflate = rectTitleRate;
				rectInflate.InflateRect(0,1);
				pDC->FillSolidRect(rectInflate,crBackSise);
				pDC->TextOut(rectTitleRate.left,rectTitleRate.top,strTitleRate);
			}
			if(!rectTitleVolume.IsRectEmpty())
			{
				rectInflate = rectTitleVolume;
				rectInflate.InflateRect(0,1);
				pDC->FillSolidRect(rectInflate,crBackSise);
				pDC->TextOut(rectTitleVolume.left,rectTitleVolume.top,strTitleVolume);
			}
		}	
		if(m_lPtrST)
		{
			pDC->TextOut(rectTitleName.left,rectIn.top+HEIGHTOFCAPTION+4,m_strName);		
		}		*/
	}

	CRect rectInter11;
	CRect rectInter12;
	CRect rectInter2;
	rectInter11.IntersectRect(m_rectTitleJongmok,rectTitleType1);
	rectInter12.IntersectRect(m_rectTitleJongmok,rectTitleType2);
	rectInter2.IntersectRect(m_rectTitleJongmok,rectTitlePos);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);
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
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,lpszTitlePos);			
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

	delete [] lpszTitlePos;
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
	InvokeAddIn("ChartGuideLineAddin.dll:GUIDE_LINE", 104, nValue);	
}

void CChartItem::SetSystemTradingIndexByTab(CStringArray *pstrArraySTName)
{
	int nSize = pstrArraySTName->GetSize();
	m_strArrayNameBySystemTradingTab.RemoveAll();
	m_strArrayNameBySystemTradingTab.Append(*pstrArraySTName);	
	BOOL bRetValue = InvokeAddInSTIndex("ChartSTIndexAddIn.dll",1,(long)pstrArraySTName);
	if(!bRetValue)
	{
		m_strArrayNameBySystemTradingTab.RemoveAll();
	}
}

void CChartItem::UpdateSystemTradingIndexByTab()
{
	if(m_strArrayNameBySystemTradingTab.GetSize())
	{
		InvokeAddInSTIndex("ChartSTIndexAddIn.dll",1,(long)&m_strArrayNameBySystemTradingTab);
	}	
}

void CChartItem::SetReplay_CurDate(CString  strBojoMsg)
{
	strBojoMsg.TrimRight();
	int nFind = strBojoMsg.Find("=", 0);
	strBojoMsg = strBojoMsg.Mid(nFind + 1, 8);

	m_strCurReplayDate.Format("%s-%s-%s",strBojoMsg.Left(4), strBojoMsg.Mid(4, 2), strBojoMsg.Right(2));
	
}

void CChartItem::InitString_ForReplay()
{
	//m_strPrePrcForReplay = _T(""); //���� ���� ����.	//2007.01.15 LYH ����(����ƴ� ��Ʈ ������ �޾� ����)
	m_strChrateForReplay = _T(""); //���� ���ϴ�� ��·�
	m_strSignForReplay	 = _T(""); //���� ���� ��ȣ.

	m_strCurPrcForReplay = _T("");
	m_strVolumeForReplay = _T("0");
}

void CChartItem::SetReplay_PrePrice(CString  strBojoMsg)
{
	CString strFilter = "PREVPRICE=_Close_:";

	strBojoMsg.TrimRight();
//	int nFind	= strBojoMsg.Find(strFilter, 0);
//	int nFind2	= strBojoMsg.Find("|", nFind);
//
//	m_strPrePrcForReplay = strBojoMsg.Mid(	nFind + 1 + strFilter.GetLength(),
//											nFind2 - nFind - strFilter.GetLength() - 1);

	//@->|�� ����
	int nFind	= strBojoMsg.Find(strFilter, 0);
	if(nFind<0)
		return;

	int nFind2	= strBojoMsg.Find("|", nFind);

	m_strPrePrcForReplay = strBojoMsg.Mid(	nFind + strFilter.GetLength(),
											nFind2 - nFind - strFilter.GetLength() );

	m_strPrePrcForReplay.TrimRight();
	m_strPrePrcForReplay.TrimLeft();
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


/////////////////////////////////////////////////////////////////////////////
// �м��� ���� ���� - ojtaso (20080728)
CString CChartItem::GetOpenAndSaveFileName(BOOL bWithTypeValue/* = FALSE*/)
{
	if(!m_arrayChartItemData.GetSize()) return "temp.tmp";

	CChartItemData* pItemData = m_arrayChartItemData.GetAt(0);
	CString sJipyoSaveFileName, sJipyoFileName;

	//100530:����ȭ�� �ٸ��� ����/�б�
	//sJipyoFileName.Format("%04d%s", ((CStdDialog*)m_pStdDialog)->m_nScreenID, ((CStdDialog*)m_pStdDialog)->m_strSaveKey);
	((CStdDialog*)m_pStdDialog)->GetFileSaveKey(sJipyoFileName);

	//{{2007.05.30 by LYH ���� ���� ���� ���� ���� ����
//	CString strTrCode = pItemData->GetTrCode();
//	if(strTrCode == QUERY_STRJONGMOK)
//		sJipyoFileName += "_Stock";
//	else if(strTrCode == QUERY_STRFUTOPT)
//		sJipyoFileName += "_FutOpt";
//	else if(strTrCode == QUERY_STRUPJONG)
//		sJipyoFileName += "_Upjong";
//	else if(strTrCode == QUERY_STRFOREIGN)
//		sJipyoFileName += "_Foreign";	
//	else if(strTrCode == QUERY_STRELW)
//		sJipyoFileName += "_ELW";	
//	else if(strTrCode == QUERY_STRCOMMODITY)	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//		sJipyoFileName += "_Commodity";

	// �м��� ���� ���� - ojtaso (20080728)
	if(((CChartMng*)GetParent())->GetSaveJipyoOneFile() && !bWithTypeValue)
	{
		sJipyoSaveFileName = sJipyoFileName + "_";
	}
	else
	{
		char chOriType = GetType();
		switch(chOriType)
		{	
//			case	SEC10_DATA_CHART:	
//			case	SEC30_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName + "_Sec_"	; break;
			case	SEC_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Sec_"	; break;
			case	TICK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Tick_"	; break;			
			case	MIN_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Min_"	; break;	
			case	DAY_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Day_"	; break;	
			case	HOUR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Hour_"	; break;
			case	WEEK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Week_"	; break;
			case	MONTH_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName + "_Month_"	; break;
			case	YEAR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Year_"	; break;
			default					:	return "";
		}
	}
	sJipyoSaveFileName += JIPYOSYNC_FILE;		
	return sJipyoSaveFileName;
}

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
CString CChartItem::GetOpenAndSaveFileName4PeriodRQ(char cType)
{
	CString sJipyoSaveFileName, sJipyoFileName;
	sJipyoFileName.Format("%04d", ((CStdDialog*)m_pStdDialog)->m_nScreenID);
	
	switch(cType)
	{	
		case	SEC_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Sec_"	; break;
		case	TICK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Tick_"	; break;			
		case	MIN_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Min_"	; break;	
		case	DAY_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Day_"	; break;	
		case	HOUR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Hour_"	; break;
		case	WEEK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Week_"	; break;
		case	MONTH_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName + "_Month_"	; break;
		case	YEAR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Year_"	; break;
		default					:	return "";
	}

	sJipyoSaveFileName += JIPYORQ_FILE;		
	return sJipyoSaveFileName;
}
//20110502 �̹��� <<

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

	//@Solomon-100104SM030:�����༼ ���쵵��.
	if(p_nCommandType == 200 || p_nCommandType == 7)//�ʱ�ȭ
	{
		m_strArrayNameBySystemTradingTab.RemoveAll();
	}

	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

BOOL CChartItem::InvokeAddInNews(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if(!m_bAddInLoadNews)
	{
		LoadAddIn(p_szAddInDllName);
		m_bAddInLoadNews = TRUE;
	}
	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

//@��������
//int CChartItem::GetMarketType(CString strCode)
//{
//	IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager);
//	BYTE ucMarketType = pDataManager->GetMarketTypeFromCode(strCode);
//	BOOL bRet = FALSE;
//	int nLength = strCode.GetLength();
//	if(		ucMarketType == 1	||		// ���� ����
//			ucMarketType == 2	||		// �ڽ��� ����
//			ucMarketType == 3	||		// ��3����(OTCBB)
//			ucMarketType == 4	||		// HIGHYIELD �ݵ�(��������)
//			ucMarketType == 5	||		// �ֽĿ���Ʈ ELW
//			ucMarketType == 6 )			// KDR
//			return CMasterTypeInfo::STOCK_CHART;
//	else if( ucMarketType == 26 ||		// �峻����
//			 ucMarketType == 27 ||		// KOSPI 200 ����
//			 ucMarketType == 28 ||		// KOSPI 100 ����
//			 ucMarketType == 33 ||		// �ڽ��� ����
//			 ucMarketType == 34 )		// �ڽ���50 ����
//		return CMasterTypeInfo::UPJONG_CHART;
//	else if( ucMarketType == 24 ||		// ����
//			 ucMarketType == 25 ||		// �ɼ�
//			 ucMarketType == 41 )		// �������弱��
//		return CMasterTypeInfo::FUTOPT_CHART;
//	else if( ucMarketType == 64 ||		// �ֽļ���
//			 ucMarketType == 38 )		// �ֽĿɼ�
//		return CMasterTypeInfo::STOCKFUTOPT_CHART;
//	else if( ucMarketType == 42 ||		// �̱�����
//			 ucMarketType == 43 ||		// �̱�����(�̱�����)
//			 ucMarketType == 44 ||		// �̱����� �������(ADR)
//			 ucMarketType == 45 ||		// ��ǰ����(������)
//			 ucMarketType == 47 ||		// �������� �������(GDR)
//			 ucMarketType == 56 )		// �����ְ�����(������)
//		return CMasterTypeInfo::FOREIGN_CHART;
//	
//	return CMasterTypeInfo::STOCK_CHART;
//
////@��������
////		return CMasterTypeInfo::UPJONG_CHART;
////	if(strType == "77" || strType == "78" || strType == "79" || strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")	//�ؿ�
////	{
////		return CMasterTypeInfo::FOREIGN_CHART;
////	}
//////	if(strType == "13" || strType == "14" || strType == "15" || strType == "16" || strType == "17" || strType == "18" || strType == "19" || strType == "22" || strType == "61" || strType == "62")	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//////	{
//////		return COMMODITY_CHART;
//////	}
////	// �����ɼ�
////	// if(strType == "4" || strType == "5")
////	if(nLength == FUTOPT_LENGTH)
////	{
////		return CMasterTypeInfo::FUTOPT_CHART;
////	}
////	// ���� (KOSPI 200 ����, KOSPI 100 ����, �峻����, KRX)
////	// else if(strType == "B")
////	// ���� (�ڽ��� ����)
////	// else if(strType == "C")
////	else if(nLength == UPJONG_LENGTH)
////	{
////		return CMasterTypeInfo::UPJONG_CHART;
////	}
////
////	// �ֽ� (��������, �ڽ��� ����)
////	// if(strType == "1" || strType == "2")
////	if(strType == "1" || strType == "3")	//KOSPI, KOSDAQ
////	{
////		return CMasterTypeInfo::STOCK_CHART;
////	}
//////@��������
//////	// �ֽĿ���Ʈ ELW
//////	// else if(strType == "N")
//////	else if(strType == "68")	//ELW
//////	{
//////		return ELW_CHART;
//////	}
//////@��������
////	return CMasterTypeInfo::STOCK_CHART;
////@��������
//}
//
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
//@��������
//		// ELW
//		else if(nMarketType == ELW_CHART)
//		{
//			strCode = pDataManager->GetLastCode("04");
//			if(strCode.GetLength() >= 11)
//			{
//				strCode = strCode.Mid(5, CODE_STOCK_LENGTH);
//			}
//		}
//@��������
		// �ؿ�
		else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		{
			strCode = pDataManager->GetLastCode("05");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, CODE_STOCK_LENGTH);
			}
		}
	}

	return strCode;
}

CString CChartItem::GetQueryOpt()
{
	int nQueryOpt = ((CChartMng*)GetParent())->SendMessage(UMSG_GETQUERYOPT);
	if (nQueryOpt > 0)
	{
		if (m_nQueryOpt > 0)
			nQueryOpt = m_nQueryOpt;
	}

	CString strQueryOpt;
	strQueryOpt.Format("%d", nQueryOpt);
	return strQueryOpt;
}

void CChartItem::SetQueryOpt(int nOpt)
{
	((CChartMng*)GetParent())->SendMessage(RMSG_SETQUERYOPT, nOpt);
	m_nQueryOpt = nOpt;
}

//@�����߰�
/////////////////////////////////////////////////////////////////////////////

// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
void CChartItem::AddChartItem(CChartItemData* pChartItemData, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, char cType, LPCTSTR lpszTickMinDayNumber, long lCount, long lChartNumericValueType, BOOL bBuiltChart)
{
	int nBlockCount = 0;
	if(bBuiltChart == TRUE && !((CStdDialog*)(m_pStdDialog))->IsAvailableBlockCount(TRUE, nBlockCount))
	{
		CString strMessage;
		strMessage.Format("�ִ� %d�� �������� �߰� �����մϴ�.", nBlockCount);
		AfxMessageBox(strMessage);

		bBuiltChart = FALSE;
		return;
	}

	CString szItemName(lpszItemName);

	//#���ϴ����Ʈ���� ������ ���� ���ڸ� ������ ���ڷ� ǥ���ϱ� ���� ó���Ѵ�.
	//Addin(ChartAnalysisToolAddIn)������ !Pre!.. �� ���� �۾��� ������ ���־�� �Ѵ�.
	if(GetScreenNo() == nScreenID_6401 && GetPrevDateRecvFlag())
	{
		CString szWorkDate;
		szWorkDate = ((CStdDialog*)m_pStdDialog)->DoGetWorkDate(1);
		if(szWorkDate.GetLength()>0)
		{
			szItemName.Format("!Pre!%s!%s", szWorkDate, lpszItemName);
		}
	}

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
			return;
	}

	BOOL bNextOld = m_bNext;	
	pSelChart->m_strChartGubun = strChartGubun;
	pSelChart->m_strItemCode = lpszItemCode;
	pSelChart->m_strItemName = szItemName;
	pSelChart->m_cType = cType;
	pSelChart->m_strTickMinDayNumber = lpszTickMinDayNumber;
	if(bNextOld == FALSE)
	{
		pSelChart->m_lCount = lCount;
	}
	pSelChart->m_nChartNumericValueType = lChartNumericValueType;
	pSelChart->m_strKey = pChartItemData->GetNextKey();

	m_mapChartItem.SetAt(m_strSelectedRQ, pSelChart);

//>> [QC No]090728HMC040
	if(((CChartMng*)GetParent()))
		((CChartMng*)GetParent())->UpdateSyncIndicator();
//<< [QC No]090728HMC040
	m_lpSelChart = pSelChart;
}

// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
// ������ RQ�� �������� �ʵ��� ���� - ojtaso (20080509)
BOOL CChartItem::GetSelChart(LPCTSTR lpszRQ, ST_SELCHART*& pSelChart, BOOL bChangeSelRQ/* = TRUE*/)
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
		if(bResult && bChangeSelRQ)
			m_strSelectedRQ = lpszRQ;
	}

	return bResult;
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

int CChartItem::GetRealRefCount()
{
	int nRefCount = 0;
	// ��� ���������� ���� �����尡 ������ �������� Ȯ��
	POSITION pos = m_mapChartItemRealData.GetStartPosition();
	ST_CHARTITEM_REALDATA* lpChartItemRealData = NULL;
	CString strKey;
	while(pos)
	{
		m_mapChartItemRealData.GetNextAssoc(pos, strKey, (LPVOID&)lpChartItemRealData);
		if(lpChartItemRealData)
			nRefCount += lpChartItemRealData->m_nRefCount;
	}

	return nRefCount;
}

void CChartItem::SetSTRunSTPath(CString szSTRunSTPath)
{
	if(m_szSTRunSTPath.Compare(szSTRunSTPath) == 0)
	{
		m_szSTRunSTPath.Empty();
		return;
	}
	m_szSTRunSTPath = szSTRunSTPath;
}

int CChartItem::IsAvailableForReplay()
{
	int nCount = m_arrayChartItemData.GetSize();
	if(nCount <= 0)
		return 2;	// ����Ʈ���� ����Ұ�

	CChartItemData* pItemData = m_arrayChartItemData.GetAt(0);
	if(pItemData->GetTrCode() != QUERY_STRJONGMOK && pItemData->GetTrCode() != QUERY_STRFUTOPT)
		return 1;	// �ֽİ� ���ɸ� ����

	int nRealRefCount = GetRealRefCount();
	if(nRealRefCount > 1)
		return 2;	// Ref Count �ʰ�

	return 0;		// ���� ����
}

void CChartItem::GetOHLDataFromBojomsg(CString strBojomsg, CString& strOpen, CString& strHigh, CString& strLow)
{
	// OHL=:23100:23500:22700@
	int nOHLPos = strBojomsg.Find("@OHL=:");
	if(nOHLPos >= 0)
	{
		strBojomsg = strBojomsg.Mid(nOHLPos + 6);
		nOHLPos = strBojomsg.Find("@");
		if(nOHLPos > 0) strBojomsg = strBojomsg.Left(nOHLPos);

		// Open
		nOHLPos = strBojomsg.Find(":");
		if(nOHLPos > 0) strOpen = strBojomsg.Left(nOHLPos);

		// High
		strBojomsg = strBojomsg.Mid(nOHLPos + 1);
		if(nOHLPos > 0) strHigh = strBojomsg.Left(nOHLPos);

		// Low
		strLow = strBojomsg.Mid(nOHLPos + 1);
	}
}

void CChartItem::CopyData(CChartItem* pChartItem)
{
	m_CfgInfo.bShowHLBar = pChartItem->m_CfgInfo.bShowHLBar;
	m_CfgInfo.bRealCandle = pChartItem->m_CfgInfo.bRealCandle;
	m_CfgInfo.bShowRight = pChartItem->m_CfgInfo.bShowRight;
	m_CfgInfo.bRecvTimeShare = pChartItem->m_CfgInfo.bRecvTimeShare;
	m_CfgInfo.bAutoSave = pChartItem->m_CfgInfo.bAutoSave;
	m_CfgInfo.bShowLog = pChartItem->m_CfgInfo.bShowLog;
	m_CfgInfo.bRevisedValue = pChartItem->m_CfgInfo.bRevisedValue;
	m_CfgInfo.bStartDate = pChartItem->m_CfgInfo.bStartDate;

	SetTickNumber(pChartItem->GetTickNumber());
	SetSecNumber(pChartItem->GetSecNumber());
	SetMinNumber(pChartItem->GetMinNumber());
	SetDayNumber(pChartItem->GetDayNumber());
	SetHourNumber(pChartItem->GetHourNumber());

	m_strSelectedItemCode = pChartItem->GetSelectedItemCode();

	GetChartItemDataNew(pChartItem->GetDataTRCode(), m_strSelectedItemCode);
}

UINT CChartItem::GetScreenNo()
{
	return ((CStdDialog*)m_pStdDialog)->m_nScreenID;
}

BOOL CChartItem::GetPrevDateRecvFlag()
{
	return ((CChartMng*)GetParent())->GetPrevDateFlag();
}

int CChartItem::GetRQCount()
{
	return m_mapChartItem.GetCount();
}
//20090922 SJ_KIM ViewCount
void CChartItem::SetViewCountFlag(BOOL bFlag)
{
	m_bSetViewCountFlag = bFlag;
}
//20090922 SJ_KIM ViewCount
BOOL CChartItem::GetViewCountFlag()
{
	return m_bSetViewCountFlag;
}

// 2011.01.17 alzioyes:�ڸ����� ���� �����ؾ��ϴ� �� ����.
void CChartItem::_DoGetColor(COLORREF &crSHL, BOOL bIsRealJang, double dwBasePrice, double dwTarPrice, int nPoint)
{
	if(bIsRealJang==FALSE)
	{
		crSHL = RGB(0, 0, 0);
	}
	else
	{
		if(dwTarPrice==0.00)
		{
			crSHL = RGB(0, 0, 0);
			return;
		}

		double compDiff = 0.0;
		if(nPoint==1) compDiff = 0.1;
		else if(nPoint==2) compDiff = 0.01;
		else if(nPoint==3) compDiff = 0.001;
		else if(nPoint==4) compDiff = 0.0001;

		double diff = dwBasePrice-dwTarPrice;
		if(diff>compDiff)
			crSHL = RGB(0, 0, 255);
		else if(diff<-compDiff)
			crSHL = RGB(255, 0, 0);
		else
			crSHL = RGB(0, 0, 0);		
	}
}

void CChartItem::GetOHLValue(CString &strTrCode, LPCSTR _szBozo, CString &szOpen, CString &szHigh, CString &szLow)
{
	CString szBozo;
	szBozo.Format("%400.400s", _szBozo);

	int nStart = szBozo.Find("@OHL=:");
	if(nStart<0) return ;
	nStart +=6;
	int nEnd = szBozo.Find("@", nStart);
	if(nEnd<0) return ;

	CString szOHLBozo = szBozo.Mid(nStart, nEnd-nStart);
	nStart=0;
	nEnd = szOHLBozo.Find(":", nStart);
	szOpen = szOHLBozo.Mid(nStart, nEnd-nStart);

	nStart = nEnd+1;
	nEnd = szOHLBozo.Find(":", nStart);
	szHigh = szOHLBozo.Mid(nStart, nEnd-nStart);

	nStart = nEnd+1;
	szLow = szOHLBozo.Mid(nStart);

	// �ֽļ����� ��, �ü��ٿ� ��,��,�� �����Ϳ� '-'��ȣ���� ��Ÿ���� �κ� ó�� 
	if( strTrCode == QUERY_STRSTOCKFUTOPT )
	{
		int nTemp = atoi(szOpen);
		if( nTemp < 0 )
			nTemp *= -1;
		szOpen.Format("%d", nTemp);
		
		nTemp = atoi(szHigh);
		if( nTemp < 0 )
			nTemp *= -1;
		szHigh.Format("%d", nTemp);
		
		nTemp = atoi(szLow);
		if( nTemp < 0 )
			nTemp *= -1;
		szLow.Format("%d", nTemp);
	}

	if( strTrCode == QUERY_STRUPJONG ||		
		strTrCode == QUERY_STRFOREIGN)
	{
		int nTemp = atoi(szOpen);
		double dData = nTemp/100.;
		szOpen.Format("%.2f", dData);

		nTemp = atoi(szHigh);
		dData = nTemp/100.;
		szHigh.Format("%.2f", dData);

		nTemp = atoi(szLow);
		dData = nTemp/100.;
		szLow.Format("%.2f", dData);		
	}
}

//20110406 : ����� RQ�� ���̴� ��찡 �־ ó����. (�ٺ����� ó���� �ƴ�.)
BOOL CChartItem::_doCheckCodeAndTR(LPSTR _szCode, LPSTR _szTR, CString* prData, LPCSTR _szCFGFile, LPCSTR _szAppName)
{
	int nSize = 11 + (58 * MAX_RQ);
	CString strBuffer;
	char* szTemp = strBuffer.GetBuffer(nSize);
//	char szTemp[MAX_PATH]={0,};
	if(::GetPrivateProfileString(_szAppName, "RQ", "", szTemp, nSize, _szCFGFile)==0)
	{
		strBuffer.ReleaseBuffer();
		return TRUE;
	}
	strBuffer.ReleaseBuffer();
	CString rString[3];
	AfxExtractSubString(rString[0], strBuffer, 0, ';');
	AfxExtractSubString(rString[1], strBuffer, 3, ';');
	AfxExtractSubString(rString[2], strBuffer, 4, ';');

//	int nCntOfGraph = ::GetPrivateProfileString(_szAppName, "CntOfGraph", "", szTemp, sizeof(szTemp), _szCFGFile);
//	if(nCntOfGraph==1)
// 	{
// 		if(atoi(rString[0]) !=1 )
// 		{
// 			CString sWTemp(szTemp);
// 			int nPos = sWTemp.Find(';');
// 			sWTemp.Format("%d%s", nCntOfGraph, sWTemp.Mid(nPos));
// 			::WritePrivateProfileString(_szAppName, "RQ", sWTemp, _szAppName);
// 		}
// 	}
	//[A00000419]alzioyes:20110420 [0000]�� RQ�� ù��° ������ �׻� 1�� �����ϵ��� ó��.
	//�׷��� ���� ��� �״� ��찡 ���� �߻��ϹǷ� ����.
	if(strcmp(_szAppName, "0000")==0)
	{
		//if(atoi(rString[0]) !=1 )
		int nRQ0Size = rString[0].GetLength();
		{
			int nCount = _doCheckRQCount(strBuffer);
			CString sWTemp = strBuffer;
			int nPos = sWTemp.Find(';');
			sWTemp.Format("%0*d%s", nRQ0Size, nCount, sWTemp.Mid(nPos));
			::WritePrivateProfileString(_szAppName, "RQ", sWTemp, _szCFGFile);
		}
	}

	if(_szCode==NULL || rString[1].Compare(_szCode)!=0)
	{
		prData[0] = rString[1];
		prData[1] = rString[2];
		return FALSE;
	}

	return TRUE;
}

int CChartItem::_doCheckRQCount(CString szRQInfo)
{
	int nPos = szRQInfo.Find(';');
	if(nPos<0) return 0;
	szRQInfo = szRQInfo.Mid(nPos+1);

	nPos = szRQInfo.Find(';');
	if(nPos<0) return 0;
	szRQInfo = szRQInfo.Mid(nPos+1);

	int nCount = 0;
	while(true)
	{
		for(int i=0; i<9; i++)
		{
			nPos = szRQInfo.Find(';');
				if(nPos<0) break;
			szRQInfo = szRQInfo.Mid(nPos+1);
		}
		nCount++;
		if(nPos<0) break;
	}
	return nCount;
}

// vntsorl:[0000410] �̵���ռ��� ���������� �ֱ⺰�� �ٸ��� ����� �� �ֵ��� ��ɰ��� ��û
//20110502 �̹��� >>
int CChartItem::GetCountRQ()
{
	POSITION pos = m_mapChartItem.GetStartPosition();
	CString strKey;
	ST_SELCHART* lpSelChart = NULL;

	CString strCount, strValue, strTemp;

	int nCount = 0;
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)lpSelChart);

		if(!CDRChartOcx::FindRQ(strKey))
			continue;

		nCount++;
	}

	return nCount;
}
//20110502 �̹��� <<
