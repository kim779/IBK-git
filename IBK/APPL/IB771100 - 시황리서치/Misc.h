#pragma once
#define SAFE_DELETE(p) {if (p) {delete p; p = NULL;} }
#define S_TITLE "��Ȳ����ġ"
#define WM_BTNMSG		WM_USER + 10000

#define MAP_WIDTH		282
#define MAP_HEIGHT		325

#define KEY_ACCOUNT		255
#define KEY_CHKPASS		250


#define IDC_SCREEN_NEWS		10101


#define PARAM_SIZE		"MoveWnd"

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