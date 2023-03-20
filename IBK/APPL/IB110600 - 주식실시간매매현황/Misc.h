#pragma once

#define SAFE_DELETE(p) {if (p) {delete p; p = NULL;} }
#define S_TITLE "�ǽð� �Ÿ���Ȳ"
#define WM_BTNMSG		WM_USER + 10000

#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		3
#define GAP_BLOCK		5
#define GAP_PANEL		4
#define GAP_CTRLT		3	//top, bottom
#define GAP_CTRLS		6	//side
#define GAP_BTN			1

#define MAP_WIDTH		900
#define MAP_HEIGHT		636

#define KEY_ACCOUNT		255
#define KEY_CHKPASS		250

#define PANEL_HEIGHT	34
#define GUIDE_HEIGHT	50
#define ACC_WIDTH		140
#define CTRL_HEIGHT		20
#define BTN2_WIDTH		36
#define BTN4_WIDTH		60
#define BTN5_WIDTH		72
#define BTN9_WIDTH		120
#define MAP_MIN_HEIGHT	26
#define SPLITTER_BORDER 6

#define IDC_EDIT_PASS		10001
#define IDC_ACCOUNT			10002
#define IDC_STATIC_ACCNAME	10003
#define IDC_BTN_INQUIRY		10004

#define IDC_WND_SCREEN		10101

#define IDC_WND_GUIDE		10201

#define IDC_HSPLITTER		10110
#define IDC_SCREEN_ORDER	10111
#define IDC_SCREEN_SIGNED	10112
#define IDC_SCREEN_BALANCE	10113
#define IDC_BTN_UP			10121
		
#define IDC_BTN_SIGNED		10211
#define IDC_BTN_BALANCE		10212
#define IDC_BTN_RECIEVEACC	10213
#define IDC_BTN_ATTENTION	10214


#define PARAM_VALUE		"SetValue"
#define PARAM_SIZE		"MoveWnd"
#define PARAM_POPUP		"POPUP"
#define PARAM_RESET		"Reset"

#define CONTROL_ACCN	"AN1A"
#define TRIGGER_GUIDE	"GUIDE1106DLL"
#define TRIGGER_RPASS	"RPASS1106"		//�н����� ��û

#define TT_CHKPWD 106

#define	clBack	64
#define clBox	65
#define	clText	130
#define	clUp	94
#define	clDown	95

//�������
#define clreferenceText 91

//btn
#define clBtnText 186

//�ǳ�
#define clPanel	66
#define clPanelLine 165

//contents �ܰ�
#define clContents 181
#define clGuide	34

//tab
#define	clTabf	70
#define	clTabsf	71

//grid
#define	clHead	74
#define	clHText	76
#define	clData	68
#define	clLine	93
#define	clFocus	78
#define	clRow1	68
#define	clRow2	77
#define clSel	78

#define MAP_PROC  "OnDllValue"