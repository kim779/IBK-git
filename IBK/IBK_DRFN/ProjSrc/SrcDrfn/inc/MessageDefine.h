#if !defined(AFX_MESSAGEDEFINE_H__17563481_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)
#define AFX_MESSAGEDEFINE_H__17563481_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// ����� ���� �޼���
//
#define		UDM_SELCHANGE	100
#define		UDM_FONT_DATA	200


/////////////////////////////////////////////////////////////////////////////
// ���� ���� 
//
#define	SOCKET_UNKNOWNERR		-100		// Unknown Error
#define	SOCKET_SOCKCREATEERR		-5		// NeoSock��������
#define	SOCKET_CONNECTERRROR		-4		// ����� ���� �߻�
#define	SOCKET_CLOSE			-3		// �������� CLOSE �޽��� ����
#define SOCKET_RECEIVEQUIT		-2		// ����޼��� ����
#define SOCKET_TIMEOUT			-1		// ���� ����� �ð��ʰ�	
#define SOCKET_READY			0		// �غ���
#define SOCKET_OK			1		// ����Ϸ�


#define UDM_RECV_SOCKET_STATE	    WM_USER+801 //���� ���� ���� 
#define UDM_RECV_SOCKET_DATA        WM_USER+802 //���Ͽ��� ���� ����Ÿ 
#define UDM_GENERAL_DATA		    WM_USER+112 //�Ϲ� ����Ÿ�� ���� 
#define	UDM_SEND_USERDATA			WM_USER+120 //���������� SEND 
#define	UDM_SOCK_CREATE				WM_USER+113 //Login(dll)���� ���ο��� ���Ͽ����û 
#define UDM_SOCK_DISCONNECT			WM_USER+114 //Login(dll)���� ���ο��� ���������û 
#define UDM_GET_IPADDRESS			WM_USER+115	//Login(dll)���� ���ο��� ����IP��û 
#define IDW_WEBCONNECT              117

/////////////////////////////////////////////////////////////////////////////
// ����� ���� ���� 
//
#define	UDM_TRCODE_ENTER			WM_USER+200 //TR CODE ENTER

#define UDM_TICKERDATA_RCV			WM_USER+201	//TICKER DATA

#define UDM_URGENCY_REV				WM_USER+202 //Urgency Data

#define	UDM_POLLINGDATA_RECV		WM_USER+203	// SocketDlg->MainFrame (Polling ����Ÿ)

#define UDM_CLOSEWITHOUTASK			WM_USER+204	// ������ ����

#define UDM_SOCKETCLOSE				WM_USER+205	// ���� Ŭ���� �߻�

#define UDM_UPTOOLBARFUNC			WM_USER+210	// ��� ���� ���� ��ư Ŭ�� �̺�Ʈ WPARAM : Function ��ȣ

/////////////////////////////////////////////////////////////////////////////
// ������ ����(View -> ChildFrame) 
//
#define UDM_FRAME_INFO				WM_USER+300 // View -> Frame (������, Ÿ��Ʋ) 
#define UDM_RETURN_MESSAGE			WM_USER+310 // View -> Frame (�� �޼���)
#define	UDM_GETFRAME_SIZE			WM_USER+342	// ���� ȭ���� ����� �˰��� �� �� 
#define	UDM_SETFRAME_SIZE			WM_USER+343	// Frame Size ���� : WPARAM->Flag, LPARAM->size ����

/////////////////////////////////////////////////////////////////////////////
// ���� ��й�ȣ ����� 
//
#define	UDM_ACCT_MSG				WM_USER+400 
#define UDM_ACCT_DRAGDROP			WM_USER+401
#define	UDM_LIST_CHANGEITEM			WM_USER+420
#define	UDM_LIST_CHECKCOLUMN		WM_USER+425
#define	ACCT1_CLEARALL				1
#define ACCT1_CANCEL				2
#define	ACCT2_CANCEL				3

#define UDM_INIT_FOCUS				WM_USER+430
#define UDM_EDIT_RETURN				WM_USER+440

// {{ bds ���°��� �߰�
#define	UDM_SETSHAREDATA			WM_USER+701	// ��������Ÿ����
												// wParam : bNotify, lParam : STSHAREDDATA
#define	UDM_ACCTINFO				WM_USER+702	// ���°��� 
// }}

/////////////////////////////////////////////////////////////////////////////
// ȭ����� ���  
#define DEF_SCR_ROCK				WM_USER+100

/////////////////////////////////////////////////////////////////////////////
// ��ž�ν� LED 
#define UDM_STOPLOSS_LED			WM_USER+500  // (��ž�ν� -> Ticker)
#define STOPLOSS_LED_ON				1
#define STOPLOSS_LED_OFF			2
#define STOPLOSS_LED_BLINK			3
#define UDM_STOPLOSS_DESTROY		WM_USER+510  // (StoplossDlg -> mainfrm)
#define	UDM_STOPLOSS_CREATE			WM_USER+511	 // (TickerDlg -> mainfrm)

////////////////////////////////////////////////////////////////////////////
// MAP �� ����.
#define MSG_SIWHANG					WM_USER+600  // ��Ȳ 
#define UDM_MAP_OPEN				WM_USER+613	 // �Ϲݸ� 
#define UDM_MAP_OPEN_SEARCH			WM_USER+611 // 4Byte Code Enter
#define UDM_MAP_OPEN_NOCHECK		WM_USER+612	 // NoCheck �˹ݸ�
#define MSG_SIWHANG_JUMUN			WM_USER+620	 // ��Ȳ�ֹ� 
#define MSG_DEF_GAGE				WM_USER+630	 // SYSTEM GAGE
#define MSG_SHARED_PIN				WM_USER+640	 // PIN ��������.

#define MSG_PLUS_BTN				WM_USER+641	 // PLUS ��������.
#define MSG_MINUS_BTN				WM_USER+642	 // MINUS ��������.
//* ���� ���� (2001.9.10 1) Print Buttton ����� ���� ����
#define MSG_PRINT_BTN				WM_USER+643	
//* ���� �� (2001.9.10 1) 

#define	UDM_LOADMENU_FINDTRDATA		WM_USER+644	 // ����� ȭ���� open�� �� �ִ��� �ľ�����(�ü������ ���)

#define	MSG_FONT_RESIZE				WM_USER+650	 // ������ ���� 
#define	MSG_FONT_CURRENT			WM_USER+651	 // ���� ������ �˾Ƴ������� 
#define	MSG_FRAME_RESIZE			WM_USER+652	 // ������ ������ ����
#define	MSG_SHARE_CURRENT			WM_USER+653	 // ���� ���� ���� �˾Ƴ��� ���� 
#define	UDM_FONT_DEFAULT			WM_USER+654	 // ����Ʈ ������� ���� 
#define	MSG_FOLD_SCREEN				WM_USER+656	 // ȭ������
#define	MSG_ICON_MENU				WM_USER+657	 // �޴�
#define	MSG_INDEPT_SCREEN			WM_USER+658	 // ��������

////////////////////////////////////////////////////////////////////////////
// �� ����� 
#define UDM_TAB_CHANGE				WM_USER+700	 // �Ǻ����

////////////////////////////////////////////////////////////////////////////
// ȭ�� ����Ʈ
#define	UDM_PRINT_MAP				WM_USER+800	 // View -> MainFrame

////////////////////////////////////////////////////////////////////////////
// ����
#define	UDM_HELP_MAP				WM_USER+801	 // View -> MainFrame

////////////////////////////////////////////////////////////////////////////
// ���� ����
#define	UDM_CLOSE					WM_USER+900	 // ���Ͽ��� ���� ���� 

/////////////////////////////////////////////////////////////////////////////
// ����
#define	UDM_GWANSIM_REG				WM_USER+802	 // ���ɵ��â���� ��������..
												 // wParam(1) : ���ɵ����
												 // wParma(2) : ���ɵ�ϿϷ�
												 // wParma(3) : ���ɵ�����

/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// ƼĿ�� ����
#define	UDM_TICKER_JUMUN			WM_USER+1000	// �ŵ�, �ż� �ֹ�
#define UDM_TICKER_SIHWANG			WM_USER+1001    // ��Ȳâ���� 

#define	UDM_TICKER_ALONE			WM_USER+1002	// ƼĿ �ٽñ׸��� wParam : 100
													// �������� ����   wParam : 101
													// ����            wParam : 102

////////////////////////////////////////////////////////////////////////////
// DeskForm���� ������ ���� ��� ���� 
#define	UDM_FRAME_INFO2				WM_USER+1100	// ���� ����		wParam : 0
													// ������ ��������	wParam : 1						

/////////////////////////////////////////////////////////////////////////////
// íƮ
//
//#define	UDM_CHART_MSG				WM_USER+600	// ���� íƮ ����Ʈ�� íƮ OCX �ڵ��� �߰��Ѵ�. 
//#define	UDM_CHARTTOOLBAR_LOAD		WM_USER+601	// ������ íƮ ���ٸ� ����/�Ҹ��Ѵ�. 
//#define	UDM_CHARTBUTTON_CLICK		WM_USER+602	// íƮ ���� ��ư Ŭ���� �������� ��ư ���� ����.
//#define	UDM_CHARTTOOLBAR_ACTIVE		WM_USER+603	// ���ϵ� ������ Active�ɶ� �������� ������ �޼���
//#define UDM_GET_CHARTDATA			WM_USER+604 // ��Ʈ���� ����ϴ� Ŭ���� �ּ� ����
//#define UDM_GET_USETOOLBAR			WM_USER+606	// �ܺ����� ��뿩��
//#define UDM_SET_USETOOLBAR			WM_USER+607 // ���ο��� �ܺ�
//#define UDM_ANALYSIS_FUNCTION		WM_USER+532	// ���ο��� ��Ʈ�� ������ �޼���
//#define	WMU_ADD_INDICATOR			WM_USER+541	// wParam : 1 (add), 0 (delete)	// lParam : ocx �ڵ�

/////////////////////////////////////////////////////////////////////////////
// íƮ
//
//#define	UDM_CHART_MSG				WM_USER+600	// ���� íƮ ����Ʈ�� íƮ OCX �ڵ��� �߰��Ѵ�. 
//#define	UDM_CHARTTOOLBAR_LOAD		WM_USER+601	// ������ íƮ ���ٸ� ����/�Ҹ��Ѵ�. 
//#define	UDM_CHARTBUTTON_CLICK		WM_USER+602	// íƮ ���� ��ư Ŭ���� �������� ��ư ���� ����.
//#define	UDM_CHARTTOOLBAR_ACTIVE		WM_USER+603	// ���ϵ� ������ Active�ɶ� �������� ������ �޼���
#define UDM_GET_CHARTDATA			WM_USER+604 // ��Ʈ���� ����ϴ� Ŭ���� �ּ� ����
//#define UDM_GET_USETOOLBAR			WM_USER+606	// �ܺ����� ��뿩��
#define UDM_SET_USETOOLBAR			WM_USER+607 // ���ο��� �ܺ�
//#define UDM_ANALYSIS_FUNCTION		WM_USER+532	// ���ο��� ��Ʈ�� ������ �޼���
#define	WMU_ADD_INDICATOR			WM_USER+541	// wParam : 1 (add), 0 (delete)	// lParam : ocx �ڵ�

// <��Ʈ����>
#define	UDM_CHART_RELEASE_ALL		WM_USER+599 // ����/íƮ���� ���õ� ���� ����
#define	UDM_CHART_ADD				WM_USER+600	// ���� íƮ ����Ʈ�� íƮ OCX �ڵ��� �߰��Ѵ�. (0:���� 1:�߰� 2:íƮ���� �����)
#define	UDM_CHARTTOOLBAR_LOAD		WM_USER+601	// ������ íƮ ���ٸ� ����/�Ҹ��Ѵ�. 
#define	UDM_CHARTBUTTON_CLICK		WM_USER+602	// íƮ ���� ��ư Ŭ���� �������� ��ư ���� ����.
#define	UDM_CHARTTOOLBAR_ACTIVE		WM_USER+603	// ���ϵ� ������ Active�ɶ� �������� ������ �޼���
#define UDM_CHARTSUB_HIDE			WM_USER+609	// Chart Tool Bar ButtonŬ���� Sub Bar Hide		
#define UDM_ANALYSIS_FUNCTION		WM_USER+532	// ���ο��� ��Ʈ�� ������ �޼���
#define UDM_GET_USERID				WM_USER+605	// ��Ʈ���� �������� ����� id�� ��� ���� ������ �޼���			
#define UDM_GET_USETOOLBAR			WM_USER+606	// ��Ʈ���� �������� �ܺ����� ��뿩�� ��� ���� ������ �޼���
#define UDM_TOOL_SELECT				WM_USER+608	// ��Ʈ���� ������ ������ íƮ���·� �� ��� �ش� ��ư ��� ��û (wParam : �ش� ��ǥ enum�� ) ��: �Ź��� �м�
#define UM_ADD_INDICATOR_SYSTEM		WM_USER+542 // ���ο��� ��Ʈ�� ������ �޼���(wparam 1, lparam ��ǥ��) => UDM_ANALYSIS_FUNCTION�� �м����� �� ���ǵ� ��� ���, �� �ܴ� �� �޽����� ��ǥ������ ����
#define	UDM_BUTTON_CLICK_FROM_CHART_OCX		WM_USER+610	// ���� íƮ��ư�� �������� ���� �޽��� ���� �߰�(030529_���뼺)(wParam : �ش� ��ǥ enum�� , lParma : 1->Dowm 2->Up)

//-----------------------------------------------------------------------------
// Author		: {{{ ������ }}} ojtaso 	Date : 2005/2/22
// Reason		: Added to chart toolbar dll 
//-----------------------------------------------------------------------------
// BEGIN
const UINT RMSG_REMEMBERCHARTTOOL	= ::RegisterWindowMessage(_T("RMSG_REMEMBERCHARTTOOL"));
// END

// Chart���� Dialog���� ����Ѵ�. ConfigDll���� ���ο��� ����
// bds_Chart���� �߰�(040114)
#define	WMU_DELCONFIGDLG			WM_USER+1004 // ConfigDll���� ���ο��� ����


//////////////////////////////////////////////////////////////////////////////////////////////
//bds
#define	UDM_MAP_RELOAD				WM_USER+750 // ȭ�� ���� �޼���(����â)
#define	UDM_XGP_LOAD				WM_USER+751 // XGP ���� Load
#define	UDM_CLEAR_MESSAGE			WM_USER+752 // ����ȭ�� �޽����� Clear
#define	UDM_CHANGE_TAB				WM_USER+753 // �� ����
#define	UDM_MINMAX_INFO				WM_USER+754 // ȭ�� ������ ����
#define	UDM_CHANGE_SIZE				WM_USER+755	// �Ǻ���� ������ ����
#define	UDM_DLG_LOADING				WM_USER+756 // ����ȭ�� loading�ϴ� dialog���� ���߾�ũ���� ȭ��� ��û�޽���  
#define	UDM_GET_TITLE				WM_USER+757 // �ʿ��� TITLE�� ������ �����ӿ� ������ �޼���
#define UDM_MAP_RESOTRE				WM_USER+758 // LoadXGPFile ���� �ִ�ȭ�� �����츦 Restore��Ų��.


/////////////////////////////////////////////////////////////////////////////
// ����Ʈ ���� 
//

// ��Ÿ �޼����� RegisterMessage.h ����
// #include "../../../Include_base/RegisterMessage.h"
const UINT RMSG_RESIZEDLL0202	= ::RegisterWindowMessage(_T("RMSG_RESIZEDLL0202"));
const UINT RMSG_DISPMESSAGE		= ::RegisterWindowMessage(_T("RMSG_DISPMESSAGE"));
const UINT RMSG_ONMOVE			= ::RegisterWindowMessage(_T("RMSG_ONMOVE"));	// OnMove() �޽��� ����
const UINT RMSG_NOTIFYDLL		= ::RegisterWindowMessage(_T("RMSG_NOTIFYDLL"));	// Main ���� dll ȭ�鿡 ������ �޼���

//���º�� ��������
const UINT RMSG_GETACCTPASS		= ::RegisterWindowMessage(_T("RMSG_GETACCTPASS"));
//���º�� �����ϱ�
const UINT RMSG_SETACCTPASS		= ::RegisterWindowMessage(_T("RMSG_SETACCTPASS"));
//������ �ֹ�â �ݾҴٰ� ����
const UINT RMSG_RESTARTDLL		= ::RegisterWindowMessage(_T("RMSG_RESTARTDLL"));

//���� �κ� ���ε� �ϱ�
const UINT RMSG_RECALLSHAREDATA = ::RegisterWindowMessage("RMSG_RECALLSHAREDATA");
//const UINT RMSG_REFRESH_ACCT = ::RegisterWindowMessage("RMSG_REFRESH_ACCT");
// ��Ÿ �޼����� RegisterMessage.h ����

//�ֹ� Ȯ��â..
const UINT RMSG_SHOWORDERCONFIRM	= ::RegisterWindowMessage("RMSG_SHOWORDERCONFIRM");	
const UINT RMSG_MAININFO			= ::RegisterWindowMessage(_T("RMSG_MAININFO"));

//JJH 
const UINT RMSG_COLORBYIDX			= ::RegisterWindowMessage(_T("RMSG_COLORBYIDX"));
const UINT RMSG_CALLTOTMENU			= ::RegisterWindowMessage(_T("RMSG_CALLTOTMENU")); // WPARAM : Menu Index LParam : ������
const UINT RMSG_OPENMAPWITHSHARE	= ::RegisterWindowMessage(_T("RMSG_OPENMAPWITHSHARE")); // WPARAM : Menu Index LParam : ������
const UINT RMSG_PASSWDCONFERM		= ::RegisterWindowMessage(_T("RMSG_PASSWDCONFERM")); 


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MESSAGEDEFINE_H__17563481_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)

