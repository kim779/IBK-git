#ifndef _AXMSG_HXX
#define	_AXMSG_HXX

//
//	message
//
#define	WM_AXIS		(WM_USER+500)
#define	WM_STOP		(WM_USER+501)		// connnect dlg
#define	WM_UPDATE	(WM_USER+502)		// data real update : from frame to concludeDlg
#define	WM_CURSORMAP	(WM_USER+503)		// mouse hand : view�� ����

#define WM_VIEWHOOK	(WM_USER+4000)
#define	WM_PHONEPAD	(WM_USER+5255)		// ���е�
#define WM_SECURE	(WM_USER+6868)		// �������α׷� �⵿
//
//	message : WM_AXIS
//	LOWORD(wParam)
//
#define	axSTART		1
#define	axRUN		2
#define	axAXIS		3	// axis running
#define	axSIGNON	4	// signon

#define	axCHANGE	5	// change current view : combo
#define	axCLONE		6	// clone current view
#define	axCLOSEV	7	// close view
#define	axCLOSEP	8	// close popup
#define	axRELOAD	9	// combo

#define	axTOOLBAR	10	// toolbar
#define	axSELCLIST	11	//
#define axGROUP		12	// toolbar group change

#define	axCURSOR	13	// combo
#define axFOCUS		14	// wsevent
#define	axGUIDE		15	// set guide
#define	axFULLMENU	16	// get menu full string
#define	axNEWS		17	// ticker news
#define	axRESULT	18	// conclusion

#define	axEXTEND	19	// sb extend click : sbx->child->mainframe
#define	axMOVE		20	// move child
#define	axTRIGGER	21	// trigger change  : sbx->child->mainframe

#define	axHOTKEY	22	// orderTR by hotkey
#define	axHOTKEY2	23	// hotkey onTR
#define	axHOTKEY3	24	// show hotkey information

#define	axSCREEN	25	// save user screen
				// HIWORD(wParam) 0 : add, 1 : remove
#define	axMOVEX		26	// move user screen
				// HIWORD(wParam) 0 : is enable?, others : move group#
#define	axMODAL		27	// modal dialog attach & drawing
#define	axKEYS		28	// act VK_XXX (VK_HOME, VK_ESCAPE)

#define	axLINK		29	// create link map
#define	axMSG		30	// show full message

#define	axPRINT		31	// print childframe
#define	axDEVICE	32	// ms read done

#define	axSETVIEW	34	// get device view
#define axCLOSEX	35	// modalx closex setting
#define axWAIT		36	// cursor change
#define	axURL		37	// open window ie.
#define	axURLx		38	// run mainframe link.
#define	axWRITEIMG	39	// save img
#define	axPRINTIMG	40	// print img
#define	axSHOWHELP	41	// show help
#define axCLOSES	42	// single child close

#define	axALARM		100	// alarm Msg
#define	axDIALOG	101	// duplication notice
#define	axKONG		102	// processfmx

// NSocket message
#define	axREPEATSOCK	200	// connect socket
#define	axCONNECTSOCK	201	// connect socket
#define	axRECEIVESOCK	202	// receive socket
#define	axCLOSESOCK	203	// close socket

// caption function button 
#define axCAPTION	300
// child message (CChildFrm, CSChild)
#define	axLINKEDIT	701	// show linkedit

#define axGetActiveKey	401	// getactivekey
#define	axFixedFrame	444	// 2007.04.17 ����ȣ frame fix�� ���� �ɼ� 
#define axTEST		501
#define axDOCTOR	550	// ôô�ڻ�
#define	axMinuAlarm	551	// �̼���ݰ� �˸� 20070314
#define	axRTSQueue	552	// RTS����Ÿ Ȯ�� 20070621
#define axMinMaxInfo	553	// HIWORD(wParam): key, lParam -> MINMAXINFO* lpMinMaxInfo
#define axMenuMode	554	// MenuMode changed. HIWORD(wParam): menumode (0:normal, 1:small), lParam: menu height
		#define MM_NORMAL	0x00
		#define MM_SMALL	0x01
#define axIsDLL		555 // IsDLL(const char* mapname)  // lParam -> (LPCTSTR)mapname
						// return 0: map is not dll 1: map is dll
#define axSkinChange 556
#define axChaserON	9998	// Chaser ON
#define axChaserOFF	9999	// Chaser OFF
#define axMRADAR	9997	// 
#define axToolbarHide 8998 //Toolbar Hide
//
//	systemTR ID
//
#define	sysBASKET	2	// systemTR : Basket information
#define	sysORDER	3	// systemTR : order



//
//	save, restore position keyword
//
#define	snPOSITION		"POSITION"
#define	posFRAME		"Frame"
#define	posTOOLBAR		"ToolBar_"
#define	posSTATUSBAR	"StatusBar_"
#define	posHOTKEYBAR	"HotkeyBar"
#define	posTIME			"TimeWindow"

#define	snSTATUS		"STATUSBAR"
#define	nINDEX			"sbIndex_"

//////////////////////////////////////////////////////////////////////////
// callback interface	(main)

#define	AXI_BASE		21000
#define	AXI_CHANGEVIEW		(AXI_BASE + 1)		// toolbar call
#define	AXI_SELCLIST		(AXI_BASE + 2)		// toolbar call
#define AXI_CHANGEFOCUS		(AXI_BASE + 3)		// child frame call
#define AXI_ACTIVEMAP		(AXI_BASE + 4)		// tool bar call(for HSBCtrl)
#define AXI_SHOWHELP		(AXI_BASE + 5)		// caption call(show help)
#define AXI_APPSWITCH		(AXI_BASE + 6)		// app switch
#define	AXI_NEWSLIST		(AXI_BASE + 8)		// ticker bar call
#define	AXI_CHANGEBAR2		(AXI_BASE + 9)		// wParam: 0 - change skin. 1 - change virtual screen
#define AXI_SETTRIGGER		(AXI_BASE + 10)		// child frame call for trigger
#define	AXI_RESTORE		(AXI_BASE + 11)		// child frame call for restor
#define	AXI_HOTKEY		(AXI_BASE + 12)		// key hook for HOTKEY(MDIChild window)
#define AXI_CONSEE		(AXI_BASE + 14)		// view control viewer 
#define	AXI_SMCALL		(AXI_BASE + 15)		// smcall.............
#define	AXI_FUNCKEY		(AXI_BASE + 16)		// main tool bar function key........
#define	AXI_TABCHANGE		(AXI_BASE + 17)		// tabview map change
#define	AXI_SHOWDEAL		(AXI_BASE + 18)		// show conclusion list
#define	AXI_BAR2ACTION		(AXI_BASE + 19)		// bar2 fuction
#define	AXI_SETALL		(AXI_BASE + 20)		// all child set trigger
#define	AXI_GETMAPNUM		(AXI_BASE + 21)		// Get screen Number (4)
#define	AXI_TOOLTIPCHANGE	(AXI_BASE + 22)		// tooltip change
#define	AXI_APPENDALLMAP	(AXI_BASE + 23)		// controbar append all map
#define	AXI_MBLINK		(AXI_BASE + 25)		// main caption link infomation
#define	AXI_SENDTICKINFO	(AXI_BASE + 26)		// ticker infomation sendTR
#define	AXI_INPUTSCREENNO	(AXI_BASE + 27)		// Input screen Number (4)
#define AXI_EDITUSERTOOL	(AXI_BASE + 28)		// Call Edit usertool
#define AXI_TICKSETUP		(AXI_BASE + 29)		// Call setup ticker
#define AXI_GETCAPTIONINFO	(AXI_BASE + 30)		// caption information(group Number, font size)
#define AXI_HIDECAPTIONBUTTON	(AXI_BASE + 31)		// caption button hide
#define AXI_EACHTICKER		(AXI_BASE + 32)		// each ticker setting
#define AXI_LINKOPEN		(AXI_BASE + 33)		// link screen open (include code symbol)
#define AXI_STOPLOSS		(AXI_BASE + 34)		// stoploss
#define AXI_HELPCOM		(AXI_BASE + 35)		// run helpcom
#define AXI_REPORT		(AXI_BASE + 36)		// crash report
#define AXI_CLOSECHILD		(AXI_BASE + 37)		// close child		// cej
#define AXI_CLOSESCREEN		(AXI_BASE + 38)		// close screen		// cej
#define AXI_MINIMIZESCREEN		(AXI_BASE + 39)		// close screen		// cej

#define AXI_CUSTOMER		(AXI_BASE + 41)		// custommer		// cej
#define	AXI_SHOWTICKER		(AXI_BASE + 42)		// ticker add/del button// lwj
#define	AXI_PRINTSCREEN		(AXI_BASE + 43)		// print active screen	// lwj
#define	AXI_GETVOIDRECT		(AXI_BASE + 44)		// full screen void rect	// lwj
#define AXI_FOSTOPLOSS		(AXI_BASE + 45)		// future option stoploss	// lwj
#define AXI_MDINEXT		(AXI_BASE + 46)		// MDI Next child winodw	// lwj
#define AXI_LASTMAPS		(AXI_BASE + 47)		// Last MapName			// lwj

#define AXI_GETSCRBUTTONPOS (AXI_BASE + 48)		// getting screen button position from Dbar2
#define AXI_XTR				(AXI_BASE + 49)		// xtr 
#define AXI_HIDETICKER		(AXI_BASE + 50)		// hide ticker (1-3:ticker1,2,3, 4:toobar, 5:screenlist)
#define AXI_CONFIRMPASS		(AXI_BASE + 51)		// confirm password
#define AXI_VSTOOLTIP		(AXI_BASE + 52)		// virtual screen button tooltip
#define AXI_RECALCLAYOUT	(AXI_BASE + 53)		// recalc layout & draw frame
#define AXI_CHANGEPOPUP		(AXI_BASE + 54)		// create(change) popup window
#define	AXI_SETACCOUNT		(AXI_BASE + 55)		// ���¹�ȣ ����
#define AXI_ROTATEVIEW		(AXI_BASE + 56)		// ���µ�������
#define AXI_CHANGETRIGGER	(AXI_BASE + 57)		// �����丮�� Ʈ����
#define AXI_DELHISTORY		(AXI_BASE + 58)		// �����丮 ����
#define AXI_DELHISTORYALL	(AXI_BASE + 59)		// �����丮 ��ü ����
#define AXI_SHOWMAP			(AXI_BASE + 60)		//�����丮���� �˾�
#define AXI_CLOSEMAP		(AXI_BASE + 61)     //�����丮���� �˾� �ݱ�
#define AXI_ADDINTERHISTORY	(AXI_BASE + 62)		//�����丮 ���ɱ׷� �߰�
#define AXI_SEARCH_GROUPLIST		(AXI_BASE + 63)		// ���ɱ׷���ȸ
#define AXI_SEARCH_GROUPCODE		(AXI_BASE + 64)		// ���ɱ׷���ȸ

//////////////////////////////////////////////////////////////////////////
// xTc	define	(SE0000XX)
#define	XTRPOST		2000
#define	REGGROUP	2001
#define	REGCODE		2002
#define	USERSAVE	2003
#define	USERLOAD	2004

//////////////////////////////////////////////////////////////////////////
// WM_CURSORMAP LOWORD(wParam) define
#define	SCM_SETCURSORMAP	1
#define	SCM_DELETECURSORMAP	2
#define	SCM_DELETEALLCURSORMAP	3


//////////////////////////////////////////////////////////////////////////
// WM_USER	wParam define
#define MMSG_GETPASSWORD		0x01
#define MMSG_GETCERTPASSWORD	0x02
#define MMSG_APPLYSETUP			0x03
#define MMSG_SCREENOPEN			0x04
#define MMSG_SAVEINTEREST		0x05
#define MMSG_NEWINTEREST		0x06
#define MMSG_RESTORECONDLG		0x07

#endif
