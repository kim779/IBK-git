#pragma once

#include "../IhUtils.h"
#include "../../axis/axMsg.hxx"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/memDc.h"
#define WM_MANAGE	(WM_USER + 1000)
#include "griditem.h"
#include "Resource.h"
/////////////////////////////////////////////////////////////////
//////  WM_MAINWND	Msg Kind
/////////////////////////////////////////////////////////////////
// LOWORD(wParam)
#define MK_GETROOT		1	// ROOT Directory
#define MK_GETUSER		2	// USER��
#define MK_SETPAL		3	// set color
#define MK_SENDTR		4	// sendtr
#define MK_VARIANT		5	// variant
#define MK_GETGROUP		6	// get group count
#define MK_GROUP		7	// HIWORD(wParam) 1 => add 0 => minus
#define MK_GETBITMAP		8	// get bitmap handle
#define MK_TREEVISIBLE		9	// get treesize
#define MK_BASKETVISIBLE	10	// get treesize
#define MK_ISEXPAND		11	// �׷��߰��� Ȯ�����ΰ� ���Ҹ���ΰ�...
#define MK_SIZE			12	// ȭ������� ������ ���
#define MK_OPENSCR		13	// ȭ�� ����
#define MK_EXPECT		14	// ���� ����
#define MK_EXCEL		15	// ������ ������
#define MK_AUTOEXPECT   16  // ���� �ڵ�/����
#define MK_SELGROUP		17	// �׷켱�ÿ� ���� �޽���
#define MK_MEMOUPDATE		18	// �޸� ����
#define MK_CLICKGROUP		19	// �׷� Ŭ���� �۾��� ���� �ɼ�
#define MK_CLOSEGRID		20	// �׸��� â�� �ݴ´�
#define MK_SETDATAKIND		21	// ������ ����
#define MK_TREEDATA		22	// Ʈ�����ý� �׷����� �������� �޽���
#define MK_GETDATATITLE		23	// ���� �������� Ÿ��Ʋ�� �䱸�Ѵ�.
#define MK_GETWND		24	// ������ ������ ��������
#define MK_GETAFIELD		25	// ���� ���õ� �ʵ� �ε����� �����´� From ToolWindow
#define MK_TURNCODE		26	// ���񵹷�����
#define MK_HAWKEYE		28	// hawkeye�� ��뿩�� Ȯ��
#define MK_INPUTOVER		29	// �����Ǵ� �����ڵ� �Է��ϱ�
#define MK_SYNC			30	// get critical section
#define MK_RECVDATA		31	// data�� �޾��� �� �޽���
#define MK_GETMAX		32	// get max data
#define MK_RTSDATA		33	// rtsdata
#define MK_HIDECODE		34	// hide code search combo
#define MK_KILLFOCUS		35	// kill focus
#define MK_ENDDRAG		36	// end drag
#define MK_INSERTROW		37	
#define MK_DELETEROW		38	
#define MK_SETUPOK		39	// ���� ����
#define MK_SAVE			40	// ���ɱ׷� ����
#define MK_SELINT		41	// ���ɱ׷� ����
#define MK_SELFIELD		42	// �ʵ� ����
#define MK_SEARCHOPT		43	// get search option
#define MK_CHANGEFIELD		44	// Change Field
#define MK_TOOLSIZE		45	// toolbar size
#define MK_CHART		46	// ��Ʈ ��뿩��
#define MK_GETDATAKIND		47	
#define MK_GETID		48	// user id
#define MK_NEWS			50	// ����
#define MK_MEMO			51	// �޸�
#define MK_PROCDLL		52	// ���ڽſ��� procdll�� �����°��� ������.
#define MK_SISECATCH		53	// �ü�����
#define MK_GROUPHEIGHT		54
#define MK_HSCROLL			55	// ADD PSH 20070911
#define MK_MKPROP			56	// ADD PSH 20070917
#define MK_GETMOVECFG		57	// ADD PSH 20070917
#define MK_SETMOVECFG		58	// ADD PSH 20070917
#define MK_GETMOVESAVE		59	// ADD PSH 20070917
#define MK_SETMOVESAVE		60	// ADD PSH 20070917
#define MK_TOOLBUTTON		61	// ADD PSH 20070917
#define MK_SORTBASE			62	// ADD PSH 20070917
#define MK_AUTOSAVE			63	// ADD PSH 20070917
#define MK_ALERTWAVE		64	// ADD PSH 20070917
#define MK_MARKETCOLOR		65	// ADD PSH 20070917
#define MK_MARKETCODE		66	// ADD PSH 20070917
#define MK_RTSVISIBLE		67
#define MK_SHOWTOOLTIP		70
#define MK_GETCBINDEX       71  
#define MK_CLOSE			72
#define MK_ISTREE			73
#define MK_INTSAVE      	74
#define MK_GROUPSAVE      	75
#define	MK_SELECTTREE		76
#define MK_DELETEMEMO		77
#define MK_INSERTCODE		78
#define MK_SENDTREE			79
#define MK_SELECTGROUP		80
#define MK_INITGRID			81
#define MK_CALLINTEREST		82
#define MK_SELECTFIRST		83
#define MK_SENDREMAIN		84
#define MK_REMAINDATA		84
#define MK_TRIGGEROW		85
#define MK_REFRESH			86
#define MK_EQUALIZER		87
#define MK_CHANGERECOMMANDFIELD 88
#define MK_UPJONGDATA		89
#define MK_RECOMMAND		90
#define MK_SAVERECOMMANDGROUP	91
#define MK_DELREMCOMMANDGROUP 92
#define MK_ARRANGE			93
#define MK_GETARRANGE		94
#define MK_SETARRANGE		95
#define MK_AUTOARRANGE		96
#define	MK_SETARRANGEACTIVE	97
#define MK_SETARRANGENOACTIVE 98
#define MK_TREERESEND		99
#define MK_GETTREEITEM		100
#define MK_READONLYRECOM	101
#define MK_ISSUE			102
#define MK_CLEARGROUP		103
#define MK_CLEARSECONDSEND	104
#define MK_SAVECFG			105
#define MK_GETKEY			106
#define MK_NEWSNCHART		107		//2011.12.02	KSJ
#define MK_MARGIN			108		//2011.12.05	KSJ
#define MK_INFO				109		//2011.12.05	KSJ
#define MK_TICKER			110		//2011.12.12	KSJ
#define MK_RTSDATAx			111		// 2012.01.19 KSJ rtsdatax
#define MK_DISABLE_MINUS	112		//2015.08.04	KSJ �׸����� 'X' ��ư�� �������� 
#define MK_CLOSEAUTOSAVE	113		//khs
#define MK_RTS_DATA      	114		//khs  �Ⱦ�
#define MK_ISSKIP			115
#define MK_MARKETTIME		116
#define MK_GRIDREFRESH		117
#define MK_GETGROUPTR		118

// HIWORD(wParam)	// Msg Option
// common
#define MO_NONE			0

#define MO_SHOW			1
#define MO_HIDE			2

#define MO_MINUS		1
#define MO_PLUS			2

#define MO_ON			1
#define MO_OFF			2

// option
#define MO_SET			1
#define MO_GET			2
#define MO_ADD			3
#define MO_DEL			4

#define MO_USE			2

// ��Ÿ sync �ɼ�
#define MO_LOCK			1
#define MO_UNLOCK		2

// MK_SISECATCH
#define MO_INIT			1
#define MO_CATCH		2
// MK_GETWND�� �ɼ�
#define MO_VIEW			0x0001	// view window
#define MO_TOOL			0x0002	// tool window
#define MO_TREE			0x0004	// tree window
#define MO_GROUP		0x0008	// group window
#define MO_BASKET		0x0010	// basket window
#define MO_GRID			0x0020	// get active grid window
#define MO_MAIN			0x0040
#define MO_RTS			0x0080
#define MO_ALLWND		(MO_TOOL|MO_TREE|MO_GROUP|MO_BASKET|MO_MAIN|MO_RTS)
#define MO_ACTIVE		1	// active group
#define MO_LEFT			2
#define MO_ALL			3
#define MO_LEFTRIGHT		1
#define MO_TOPBOTTOM		2
#define MO_SELECT		0
#define MO_VISIBLE		1
#define MO_FULL			2

// lParam

/////////////////////////////////////////////////////////////////
// ini file sec
#define SEC_TOOL	"TOOL"
#define SEC_BASKET	"BASKET"
#define SEC_GROUP	"GROUP"
#define SEC_GRID	"GRID"
#define SEC_MAIN	"MAIN"
#define SEC_TREE	"TREE"
#define SEC_FIELDS	"FIELDS"
#define SEC_GROUPNAME	"GROUPNAME"
#define SEC_GROUPORDER	"GROUPORDER"
#define SEC_GRID	"GRID"
#define SEC_FOLDER	"FOLDER"
#define SEC_INTERFIELD	"interField"
#define KEY_SELECTGROUP "SELECTGROUPDLL"

// ini file key
#define KEY_FIELD	"FIELD"
#define KEY_VERSION	"VER"
#define KEY_TIME	"TIME"
#define KEY_EXPAND	"EXPAND"
#define KEY_FILLMODE	"FILLMODE"
#define KEY_COUNT	"COUNT"
#define KEY_RATE	"RATE"
#define KEY_HEIGHT	"HEIGHT"
#define KEY_WIDTH	"WIDTH"
#define KEY_TREESIZE	"TREESIZE"
#define KEY_BASKETSIZE	"BASKETSIZE"
#define KEY_TREEUSE	"TREEUSE"
#define KEY_BASKETUSE	"BASKETUSE"
#define KEY_GROUPSIZE	"GROUPSIZE"
#define KEY_ACTIVE	"ACTIVE"
#define KEY_SELECT	"SELECT"
#define KEY_DATA	"DATA"
#define KEY_BKCOLOR	"BKCOLOR"
#define KEY_COLOR1	"COLOR1"
#define KEY_COLOR2	"COLOR2"
#define KEY_REALCOLOR	"REALCOLOR"
#define KEY_FRCOLOR	"FRCOLOR"
#define KEY_FLASHTYPE	"FLASHTYPE"
#define KEY_LINKMAP	"LINKMAP"
#define KEY_FONTPOINT	"FONTPOINT"
#define KEY_FONTNAME	"FONTNAME"
#define KEY_CURRBOLD	"CURRBOLD"
#define KEY_NAMEBOLD	"NAMEBOLD"
#define KEY_ALLBOLD	"ALLBOLD"
#define KEY_BIG		"BIG"
#define KEY_AUTO	"AUTO"
#define KEY_SOPTION	"SOPTION"
#define KEY_MOVECFG	"MOVECFG"
#define KEY_MOVESAVE	"MOVESAVE"
#define KEY_EQUALIZER	"EQUALIZER"
#define KEY_CBARRANGE	"CBARRANGE"
#define KEY_DISPCODE	"DISPCODE"		//2011.12.13 KSJ �����ڵ�
#define KEY_NEWSNCHART	"NEWSNCHART"	//2011.12.13 KSJ ������Ȳ
#define KEY_MARGIN	"MARGIN"		//2011.12.13 KSJ ���ű�
#define KEY_SYMINFO	"SYMINFO"		//2011.12.13 KSJ Ư�̻���
#define KEY_TICKER	"TICKER"		//2011.12.13 KSJ ƼĿ


// ctrl ID
#define IDC_TOOL		1000
#define IDC_BT_PREV		1001
#define IDC_BT_NEXT		1002
#define IDC_BT_BASKET		1003
#define IDC_CB_FIELD		1005
#define IDC_CB_GROUP		1006
#define IDC_BT_CONFIG		1007
#define IDC_BT_EXPECT		1008
#define IDC_BT_EXCEL		1009
#define IDC_BT_HAWKEYE		1010
#define IDC_BT_MINUS		1011
#define IDC_BT_PLUS		1012
#define IDC_BT_TURN		1013
#define IDC_SPIN_TIME		1014
#define IDC_BTNTAB		1018
#define IDC_CHECK_PLUSMINUS	1019
#define IDC_BT_SAVE		1020
#define IDC_BT_ERASE		1021
#define IDC_BT_UP		1022
#define IDC_BT_DN		1023
#define IDC_BT_CHART		1024
#define IDC_BT_AUTO		1025
#define IDC_BT_ADDSELECT	1026
#define IDC_BT_ADDVISIBLE	1027
#define IDC_BT_ADDFULL		1028
#define IDC_BT_FIELD		1029	// ADD PSH 20070911
#define IDC_BT_FIELDDROP	1030	// ADD PSH 20070911
#define IDC_MENU_TOOL		1031	// ADD PSH 20070927
#define IDC_MENU_INTERSAVE	1032	// ADD PSH 20070911
#define IDC_MENU_INTERTAKE	1033	// ADD PSH 20070911
#define IDC_MENU_BASESORT	1034	// ADD PSH 20070911
#define IDC_MENU_INFO		1035	// ADD PSH 20070911
#define IDC_MENU_MARGIN		1036	// ADD PSH 20070911
#define IDC_MENU_MARKER		1037	// ADD PSH 20070911
#define IDC_MENU_TICKER		1038	// ADD PSH 20070911
#define IDC_MENU_TICKER_SETUP	1039	// ADD PSH 20070911
#define IDC_MENU_SAVE_CND1	1040	// ADD PSH 20070911
#define IDC_MENU_SAVE_CND2	1041	// ADD PSH 20070911
#define IDC_MENU_MARKET_COLOR	1042	// ADD PSH 20070911
#define IDC_MENU_CODE		1043	// ADD PSH 20070911
#define IDC_MENU_MULTICHART	1044	// ADD HJJ 20080416
//#define IDC_MENU_SHOWRTSWND	1045	// ADD HJJ 20080416
#define IDC_MENU_FIELD		1050	// ADD PSH 20070911
#define IDC_BT_EQUALIZER	1051
#define IDC_BT_DOMINO		1052
#define IDC_CB_ARRANGE		1053
#define IDC_MENU_UPJONGSORT	1054
#define IDC_MENU_NAMESORT	1055
#define IDC_MENU_CODESORT	1056
#define IDC_MENU_NEWSNCHART	1057	// 2011.12.02 KSJ ������ ��Ʈ
#define IDC_BT_RELOAD	1058 // ���ε��ư �߰�(�����-����ö����) 2018.07.26
#define IDC_TREE		1100
#define IDC_BASKET		1200
#define IDC_GROUP		1300
#define IDC_GRIDWND		1400
#define IDC_GRID		1500
#define IDC_RTSGRID		1600

// Tree Item Value
#define IK_INTEREST	1	// ��������
#define IK_REMAIN	2	// ��������
#define IK_KOSPI	3	// �ڽ���
#define IK_KOSDAQ	4	// �ڽ���
#define IK_KPUP		5	// �ڽ��� ����
#define IK_KQUP		6	// �ڽ��� ����
#define IK_KP200	7	// �ڽ��� 200
#define IK_KQ50		8	// �ڽ��� 50
#define IK_KRX100	9	// KRX100
#define IK_FREEB	10	// ��3����
#define IK_ETF		11	// ETF
#define IK_ELW		12	// ELW
#define IK_SINJU	13	// �����μ���
#define IK_UPJISU	14	// ��������
#define IK_FUTURE	15	// ����
#define IK_OPTION	16	// �ɼ�
#define IK_KPCALL	17	// �ڽ��� �ݿɼ�
#define IK_KPPUT	18	// �ڽ��� ǲ�ɼ�

#define ROUND_ALL    0
#define ROUND_UP     1
#define ROUND_DN     2
#define ROUND_DNLEFT 3

#define ROUND_PANEL		1
#define ROUND_CONTENTS	2
// send key
// char ���ڸ������� ġȯ ��Ʈ����
struct _trkey
{
	UINT	kind:5;
	UINT	group:3;
};

#define TRKEY_GROUPACCNLIST	0x01	// �������� Tree���� tr
#define TRKEY_ACCCUSTOM		0x02	// �� ���� 
#define TRKEY_ACCSTAFF		0x03	// �� ���� 
#define TRKEY_REMAIN		0x04	// �ܰ���ȸ
#define TRKEY_SEARCH		0x05	// ������� ���� ī�װ� TRKEY
#define TRKEY_NEWS		0x06	// ��������
#define TRKEY_GRIDNEW		0x07	// �׸��� ������ȸ
#define TRKEY_GRIDROW		0x08	// �׸��� ������ȸ
#define TRKEY_GRIDNEWS		0x09	// ������ �����ִ� �׸��� ����tr
#define TRKEY_GRIDSAVE		0x0A	// ����������
#define TRKEY_ELWISSUESEC   	0x0B    // ELW ����ȸ�纰
#define TRKEY_ELWBASEASSET 	0x0C    // ELW �����ڻ꺰
#define TRKEY_TRCOUNT		0x0D    // �巡��ī��Ʈ ��ȸ
#define TRKEY_RECOMMAND		0x0E    // ��õ���� ��ȸ
#define TRKEY_GRIDUPJONG	0x0F    // ��õ���� ��ȸ
#define TRKEY_MARKETTIME	0x10    // ���ð� ��ȸ
#define TRKEY_BACKUP		0x11    // 2016.07.12 'b' ���
#define TRKEY_LASTSET		0x12    // 2016.07.12 'V' ������������
#define TRKEY_GROUP		0x13	// ���ɱ׷�...
#define TRKEY_INTER		0x14	// ���ɸ���Ʈ...

// default value
#define DEF_ROWH		20
#define DEF_FIELD		"�ʵ� 00"
#define DEF_FIELD_CONTENT	_T("�ʵ� 00;2024;2027;3051;3071;3041;3061;3101;3106;")	//2013.02.19 KSJ 2025 --> 3051, 2026 --> 3071 �ɺ����� 
#define DEF_FIELD_CONTENT1	_T("�ʵ� 00;2024;2033;2027;7777;")

// file names
#define FILE_CONFIG	"intercfg.ini"		// ���������񿡼� ������ ������� ���������� ��ü���� ȯ���� �����ϴ� ���� �⺻����
//#define FILE_GROUP	"portfolio.ini"		// ���������� �׷��� �����ϴ� ����
//#define FILE_SUBDATA	"portfolio.i"		// Ȯ���� �ڿ� ���ڸ��� ���ڷ� ����ȴ�.
#define FILE_FIELD	"interfield.ini"	// �ʵ� ��������

// windowsize
#define REGURAR_XSIZE 300//511
#define MAX_GROUP	7 //BUFFET 10->6
#define MAX_FIELD	10
#define MAX_LINE	100
#define MAX_RTSROW	50
#define MAX_RTSCOL	9

#define SIZE_TREE	150
#define SIZE_BASKET	200
#define SIZE_TOOL	52
#define SIZE_RTS	400	
#define SIZE_WIDTH	1012
#define SIZE_GROUP	(SIZE_WIDTH - SIZE_BASKET - SIZE_TREE - 210)
#define SIZE_GROUPMINX	0//BUFFET 300
#define SIZE_GROUPMINY	0//BUFFET 200
#define SIZE_BASKETMINY	100
#define SIZE_TREEMINX	100
#define SIZE_RTSMINX	200
#define SIZE_HEIGHT	530
#define SIZE_DIVIDE	3
#define SIZE_BUTTONS	20
#define SIZE_GAPS	3
#define SIZE_GAPL	5
#define SIZE_COMBO	80
#define SIZE_DROPDOWN	200
#define SIZE_ROWHEIGHT	18
#define SIZE_MINWIDTH	0//511
#define GAP1		1
#define GAP2		2

#define	gClrBack	192
// Delimeter for parsing
#define P_DELI	0x7f
#define PTAB	"\t"
#define P_TAB   0x09
#define PNEW    "\n"
#define P_NEW	0x0a
#define P_PER	0x25

#define	deliMITER	";"
#define mchartMAP	"IB702000"		// �������� ��Ƽ��Ʈ
#define	kcurrMAP	"IB200100"		// kospi  ���簡
#define	fcurrMAP	"IB400100"		// future ���簡
#define	ocurrMAP	"IB400200"		// option ���簡
#define	sfcurrMAP	"IB425000"		// �ֽļ��� ���簡 KSJ 2012.10.11
#define	cccurrMAP	"IB453000"		// �ݸ�/��ȭ���� ���簡 KSJ 2012.10.11
#define	focurrMAP	"IB440100"		// �ֽĿɼ����簡
#define	icurrMAP	"IB290300"		// ���� ���簡
#define	ecurrMAP	"IB280200"		// elw ���簡
#define	chartMAP	"IB711200"		// chart
#define	configMAP	"IB202201"		// config map

#define	upACTION	1

#define gSYMBOL	_T("$90303")
#define gEXPECT	_T("90991")
#define gFILTERING	_T("90302")		//2011.12.27 KSJ
#define	CCOD		"1301"
#define	HCOD		"1301"
#define	FCOD		"30301"
#define	OCOD		"40301"
#define	PCOD		"PCOD"
#define	UCOD		"20301"
#define ELWCOD		"E1301"
#define SINJUCOD	"scode"
#define THIRDCOD	"1301"
#define MCSYMBOL	"IB7020"
#define SFOD		"100301"	// �ֽ�, �ݸ�, ��ȭ���� �����ڵ� KSJ 2012.10.11
#define EDFCOD		"ed_fcod"

const int colorKKRED  = PALETTERGB(255,   0,   0);
const int colorDKRED  = PALETTERGB(255, 137, 155);
const int colorLTRED  = PALETTERGB(255, 201, 206);

const int colorKKBLUE = PALETTERGB(  0,   0, 255);
const int colorDKBLUE = PALETTERGB(126, 176, 255);
const int colorLTBLUE = PALETTERGB(200, 222, 255);

#define CLR_UP		94
#define CLR_DN		95
#define CLR_TEXT	69
#define CLR_BACK	68
#define CLR_HTEXT	76
#define CLR_HBACK	74
#define CLR_LINE	75
#define CLR_FOCUS	78

#define	F_POINT		9

#define OPT_BULSUNGSIL	0x00000001	// �Ҽ���
#define OPT_SINGYU	0x00000002	// �ű�
#define OPT_GUNRIRAK	0x00000004	// �Ǹ���
#define OPT_WOOSUN	0x00000008	// �켱��
#define OPT_GUANRI	0x00000010	// ��������
#define OPT_GAMRI	0x00000020	// ��������
#define OPT_TRADESTOP	0x00000040	// �ŷ���������
#define OPT_TRADEWARN	0x00000080	// ������������
#define OPT_KOSDAQ	0x00000100	// �ڽ���
#define OPT_KOSPI	0x00000200	// �ڽ�������

#define OPT_STOCK	(OPT_KOSDAQ|OPT_KOSPI)
#define OPT_DEFAULT	(OPT_GUNRIRAK|OPT_WOOSUN|OPT_GUANRI|OPT_GAMRI|OPT_TRADESTOP|OPT_TRADEWARN)

#define	sCURR	"023"
#define	sDIFF	"024"
#define	sASK	"025"
#define	sBID	"026"
#define	sRATE	"033"
#define	sVOL	"027"

enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR, JK_SFUTURE, JK_SFSPREAD, JK_KTBFUTURE, JK_USDFUTURE, JK_GOLDFUTURE};
static struct _mapInfo {
	char	title[48];
	char	scrN[5];
	char	mapN[9];
	char	option[64];
} mapInfo[] = {
			{"�ü�������", "2101", "IB210100", ""},
			{"���Ϸ��̴�", "2102", "IB210200", ""}
		};

// needs ��
// 1 : ����Ʈ
// 2 : ������ �̸鼭 �ʼ� �׸�
// 7 : �Ϻ�
// 8 : ???
// 9 : �����ܰ�, �������� ��.. ���ͷ� ���� �ʵ�
struct	_timerID
{
	UINT	kind  : 4;		// 4bit�ȿ� �ټ������� ���� ��´�. // none, news, chart, bong, content... 
	UINT	row   : 8;		// row��
	UINT	col   : 6;		// col��
	UINT	dummy : 14;		
};

#define TK_NORMAL	0x00		// ����Ÿ�ӽ� ����Ѵ� kind
#define TK_NEWS		0x01		// ����
#define TK_CHART	0x02		// ��Ʈ
#define TK_CODE		0x03		// ����
#define TK_BONG		0x04		// �Ϻ������� 
#define TK_CONTENT	0x05		// �Ⱥ������� �ؽ�Ʈ
#define TK_RCODE	0x06		// �����ڵ�
#define TK_MEMO		0x07

static struct	_gridHdr {
	int	stid;		// string table id
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute
	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
	UINT	needs;		// default field
} gridHdr[] = 
{
	{IDS_GH_RCOD,		0,	GVAT_HIDDEN,			"1021",	GVFM_LEFT,	0,					2},	// 0	����Ÿ���ڵ�
	{IDS_GH_EXPECT,		0,	GVAT_HIDDEN,			"7788",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 1	����ü��flag
	{IDS_GH_OPEN,		0,	GVAT_HIDDEN,			"2029",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 2	�ð�
	{IDS_GH_HIGH,		0,	GVAT_HIDDEN,			"2030",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 3	��
	{IDS_GH_LOW,		0,	GVAT_HIDDEN,			"2031",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 4	����
	{IDS_GH_PRECURR,	0,	GVAT_HIDDEN,			"2934",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 5	����
	{-1 /*IDS_GH_PREOPEN*/,	0,	GVAT_HIDDEN,			"2931",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONDITIONx,		2},	// 6	��������
	{-1 /*IDS_GH_PREHIGH*/,	0,	GVAT_HIDDEN,			"2932",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONDITIONx,		2},	// 7	���Ͻð�
	{-1 /*IDS_GH_PRELOW*/,	0,	GVAT_HIDDEN,			"2933",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONDITIONx,		2},	// 8	���ϰ�
	{IDS_GH_VOL,		0,	GVAT_HIDDEN,			"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 9	��������
	{IDS_GH_INFO,		0,	GVAT_HIDDEN,			"1370",	GVFM_RIGHT,	GVAT_CONDITIONx,			2},	// 10	����Ư�̻���
	{IDS_GH_RATE,		0,	GVAT_HIDDEN,			"2033",	GVFM_RIGHT,	GVAT_CONDITIONx|GVAT_SIGNx,		2},	// 11	�����
	{IDS_GH_TVOL,		0,	GVAT_HIDDEN,			"2032",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 12	ü�ᷮ
	{-1/*IDS_GH_7852*/,		0,	GVAT_HIDDEN,			"7852",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 13	���簡
	{-1 /*IDS_GH_7853*/,		0,	GVAT_HIDDEN,			"7853",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 14	�ŷ���
	{-1 /*IDS_GH_7854*/,		0,	GVAT_HIDDEN,			"7854",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 15	���
	{-1 /*IDS_GH_7855*/,		0,	GVAT_HIDDEN,			"7855",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 16	�����
	{-2,			0,	GVAT_HIDDEN,			"1354",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 17		���ű���
	{-2,			0,	GVAT_HIDDEN,			"1355",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 18		�ſ뺸�������ű���
	
	{-1,			20,	GVAT_TEXT,			"9###", GVFM_CENTER,	GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITIONx,2},	// 19	�ŷ���	
	{IDS_GH_CODE,		0,	GVAT_SORTSTR|GVAT_DRAGMOVE,	"1301",	GVFM_CENTER,	GVAT_TOOLTIP|GVAT_DRAGDROP,		2},	// 20	�����ڵ�
	{IDS_GH_NAME,		110,	GVAT_SORTSTR,			"1022",	GVFM_LEFT,	GVAT_TOOLTIP|GVAT_EDIT|GVAT_DRAGDROP,	2},	// 21	�����
	{IDS_GH_CURR,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2023",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 22	���簡

	{IDS_GH_DIFF,		60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST,		1},	// 14	���
	{IDS_GH_VOL,		80,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 15	�ŷ���
	{IDS_GH_RATE,		50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"2033",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,				0},	// 16	�����
	{IDS_GH_TPRC,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2028",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 17	�ŷ����
	{IDS_GH_ASK,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3051",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 18	�ŵ�	//2013.02.19 KSJ 2025 --> 3051 �ɺ�����
	{IDS_GH_BID,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3071",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 19	�ż�	//2013.02.19 KSJ 2026 --> 3071 �ɺ�����
	{IDS_GH_ASKSIZE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3041",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 20
	{IDS_GH_BIDSIZE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3061",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 21
	{IDS_GH_TASKSIZE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3101",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 22
	{IDS_GH_TBIDSIZE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3106",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 23
	{IDS_GH_ASKRATE,	60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3104",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 24
	{IDS_GH_BIDRATE,	60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3109",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 25
	{IDS_GH_HOGARATE,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3107", GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 26
	{IDS_GH_OPEN,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2029",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 27
	{IDS_GH_HIGH,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2030",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 28
	{IDS_GH_LOW,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2031",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 29
	{IDS_GH_OPEN_P,		100,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#1##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 27
	{IDS_GH_HIGH_P,		100,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#2##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 28
	{IDS_GH_LOW_P,		100,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#3##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 29
	{IDS_GH_P_OPEN,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#4##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		3},	// 27
	{IDS_GH_P_HIGH,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#5##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		3},	// 28
	{IDS_GH_P_LOW,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#6##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		3},	// 29
	{IDS_GH_TVOL,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2032",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 30
	{IDS_GH_OPENRATEW,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"##1#",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		8},	// 31
	{IDS_GH_OPENRATEP,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"##2#",	GVFM_RIGHT,	GVAT_CONDITIONx,			8},	// 32
	{IDS_GH_HIGHRATEW,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"##3#",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		8},	// 33
	{IDS_GH_HIGHRATEP,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"##4#",	GVFM_RIGHT,	GVAT_CONDITIONx,			8},	// 34
	{IDS_GH_LOWRATEW,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"##5#",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		8},	// 35
	{IDS_GH_LOWRATEP,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"##6#",	GVFM_RIGHT,	GVAT_CONDITIONx,			8},	// 36
	{IDS_GH_PRECURR,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2934",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 37
	{IDS_GH_PREVOL,		80,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2321",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 38
	{IDS_GH_AVOFS,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2305",	GVFM_RIGHT,	GVAT_COMMA,				6},	// 39 �ð��Ѿ� (�Ű������� �ð��Ѿ� �߰�_RTS �ݿ� _ ���� �ɹ�(2305))
	{IDS_GH_CHESTRENGTH,	60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"2036",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 40
	{IDS_GH_ROTATION,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2350",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 41
	{IDS_GH_FORERATE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2204",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 42
	{IDS_GH_DATEBONG,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"7777",	0,		GVAT_TOOLTIP,				7},	// 43
	{IDS_GH_POSSNUM,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"###1",	GVFM_RIGHT,	GVAT_COMMA,			9},	// 44
	{IDS_GH_POSSPRC,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"###2",	GVFM_RIGHT,	GVAT_COMMA,			9},	// 45
	{IDS_GH_POSSPROFIT,	70,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"###3",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		9},	// 46
	{IDS_GH_POSSRATE,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"###4",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		9},	// 47
	{IDS_GH_CAPITAL,	55,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1333",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 48 �ں���
	{IDS_GH_STOCKCOUNT,	55,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1334",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 49 �����ֽļ�
	{IDS_GH_PRETPRC,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2322",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 50 ���ϰŷ����
	{IDS_GH_UPDNRANGE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2342",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 51 �����
	{IDS_GH_LIMITRANGE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2343",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 52 ������
	{IDS_GH_PRETRATE,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2403",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		10},	// 53 ���ϰŷ������
	{IDS_GH_REGLINE2,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2421",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 54 2�� ���׼�
	{IDS_GH_REGLINE1,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2422",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 55 1�� ���׼�
	{IDS_GH_PIVOT,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2423",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 56 PIVOT
	{IDS_GH_SUPLINE1,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2424",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 57 1�� ������
	{IDS_GH_SUPLINE2,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2425",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 58 2�� ������
	{IDS_GH_MAX52,		63,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5311",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 59 52�� �ְ�
	{IDS_GH_MIN52,		63,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5312",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 60 52�� ������
	{IDS_GH_MAXYEAR,	63,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5313",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 61 ���� �ְ�
	{IDS_GH_MINYEAR,	63,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5314",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 62 ���� ������
	{IDS_GH_MAXVOL52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5315",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 63 52�� �ְ�ŷ���
	{IDS_GH_MINVOL52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5316",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 64 52�� �����ŷ���
	{IDS_GH_MAXVOLYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5317",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 65 ���� �ְ�ŷ���
	{IDS_GH_MINVOLYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5318",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 66 ���� �����ŷ���
	{IDS_GH_MAXDATE52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5321",	GVFM_RIGHT,	0,					0},	// 67 52�� �ְ���
	{IDS_GH_MINDATE52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5322",	GVFM_RIGHT,	0,					0},	// 68 52�� ��������
	{IDS_GH_MAXDATEYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5323",	GVFM_RIGHT,	0,					0},	// 69 ���� �ְ���
	{IDS_GH_MINDATEYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5324",	GVFM_RIGHT,	0,					0},	// 70 ���� ��������
	{IDS_GH_MAXVOLDATE52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5325",	GVFM_RIGHT,	0,					0},	// 71 52�� �ְ�ŷ�����
	{IDS_GH_MINVOLDATE52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5326",	GVFM_RIGHT,	0,					0},	// 72 52�� �����ŷ�����
	{IDS_GH_MAXVOLDATEYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5327",	GVFM_RIGHT,	0,					0},	// 73 ���� �ְ�ŷ�����
	{IDS_GH_MINVOLDATEYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5328",	GVFM_RIGHT,	0,					0},	// 74 ���� �����ŷ�����
	{IDS_GH_MAXDIFF52,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5331",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 75 52�� �ְ����
	{IDS_GH_MINDIFF52,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5332",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 76 52�� ���������
	{IDS_GH_MAXDIFFYEAR,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5333",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 77 ���� �ְ����
	{IDS_GH_MINDIFFYEAR,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5334",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 78 ���� ���������
	{IDS_GH_MAXRATE52,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5341",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 79 52�� �ְ������
	{IDS_GH_MINRATE52,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5342",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 80 52�� �����������
	{IDS_GH_MAXRATEYEAR,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5343",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 81 ���� �ְ������
	{IDS_GH_MINRATEYEAR,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5344",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 82 ���� �����������
	{IDS_GH_CONTUPLIMIT,	30,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5345",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 83 ���ӻ����ϼ�
	{IDS_GH_CONTUP,		30,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5346",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 84 ���ӻ���ϼ�
	{IDS_GH_CONTDNLIMIT,	30,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5347",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 85 ���������ϼ�
	{IDS_GH_CONTDN,		30,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5348",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 86 �����϶��ϼ�
	{IDS_GH_ELWLEFTDATE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1842",	GVFM_RIGHT,	0,					0},	// 89 ELW �����ϼ�
	{IDS_GH_ELWLASTDATE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1854",	GVFM_RIGHT,	0,					0},	// 90 ELW �����ŷ���
	{IDS_GH_ELWCRATE,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1855",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 91 ELW ��ȯ����
	{IDS_GH_EPSFULL,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5353",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 92 EPS ����
	{IDS_GH_EPSHALF,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5354",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 93 EPS �ݱ�
	{IDS_GH_PERFULL,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5355",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 94 PER ����
	{IDS_GH_PERHALF,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5356",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 95 PER �ݱ�
	{IDS_GH_EQUALIZER,	100,	GVAT_SORTABS|GVAT_DRAGMOVE,	"7778",	0,		GVAT_TOOLTIP,				11},	// 96 ü�� ����������	
	{IDS_GH_FOREIGNER,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2399",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,				0},	// 97 �ܱ���
	{IDS_GH_GIGOAN,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2390",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,				0},	// 98 ���
	{IDS_GH_RECOMMDATE,	70,	GVAT_SORTSTR,			"1910",	GVFM_RIGHT,	0,								12},	// 96 ��õ���� 	
	{IDS_GH_RECOMBASICCOST,	0,	GVAT_HIDDEN,			"1911",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	13},		// 96 ��õ���� 	
	{IDS_GH_RECOMSUIKRATE,	60,	GVAT_SORTVAL|GVAT_SORTABS,	"1912",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,  14},		// 96 ��õ���� 	
	{IDS_GH_RECOMBMJISU,	0,	GVAT_HIDDEN,			"1913",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	15},		// 96 ��õ���� 	
	{IDS_GH_RECOMBMRATE,	60,	GVAT_SORTVAL|GVAT_SORTABS,	"1914",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,	16},		// 96 ��õ���� 	
	{IDS_GH_JISUGUBN,	0,	GVAT_HIDDEN,			"1915",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,	18},		// 96 �������� 	
	{IDS_GH_RECOMBMGOAL,	60,	GVAT_SORTVAL|GVAT_SORTABS,	"1916",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,	17},		// 96 ��õ����  	
	{IDS_GH_UPJONGCODE,	0,	GVAT_HIDDEN,			"1918",	GVFM_RIGHT,	GVAT_COMMA,						19},	// 96 �����ڵ�	
	{IDS_GH_NEWCURR,	0,	GVAT_HIDDEN,			"2022",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		21},	// ���� �߰��� ���簡
	{IDS_GH_RECOMMDATE,	70,	GVAT_SORTSTR,			"1999",	GVFM_RIGHT,	0,								25},	// 2016.04.15 KSJ epb ��õ���� 	
	//khs
/*	
	{IDS_GH_ABR_1JU,	100,		GVAT_SORTABS|GVAT_DRAGMOVE,	"5401",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0},	// ���񼺰� 1��  
	{IDS_GH_ABR_1MON,	100,		GVAT_SORTABS|GVAT_DRAGMOVE,	"5402",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0},	// ���񼺰� 1�� 	
	{IDS_GH_ABR_3MON,	100,		GVAT_SORTABS|GVAT_DRAGMOVE,	"5403",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0},	// ���񼺰� 3�� 
	{IDS_GH_ABR_6MON,	100,		GVAT_SORTABS|GVAT_DRAGMOVE,	"5404",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0},	// ���񼺰� 6��
	{IDS_GH_ABR_1YEA,	100,		GVAT_SORTABS|GVAT_DRAGMOVE,	"5405",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0}	// ���񼺰� 1�� 	
*/	
	{IDS_GH_ABR_1JU,	100,		GVAT_SORTVAL|GVAT_DRAGMOVE,	"5401",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0},	// ���񼺰� 1��  
	{IDS_GH_ABR_1MON,	100,		GVAT_SORTVAL|GVAT_DRAGMOVE,	"5402",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0},	// ���񼺰� 1�� 	
	{IDS_GH_ABR_3MON,	100,		GVAT_SORTVAL|GVAT_DRAGMOVE,	"5403",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0},	// ���񼺰� 3�� 
	{IDS_GH_ABR_6MON,	100,		GVAT_SORTVAL|GVAT_DRAGMOVE,	"5404",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0},	// ���񼺰� 6��
	{IDS_GH_ABR_1YEA,	100,		GVAT_SORTVAL|GVAT_DRAGMOVE,	"5405",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,								0}	// ���񼺰� 1�� 

};

// mapnames
#define MAP_CONFIG	"IB202201"	// ����dll
#define	trUPDOWN	"pidomyst"	//2012.01.19 KSJ ���뼳�� ���� pido�̸� �����ͷ� ���Եȴ� BP21 -> IS21, BP11 -> IS21
#define	trUPDOWN2	"pihomyst"	//2012.01.19 KSJ ���뼳�� ���� piho�̸� ���ͳ����� ó���Ѵ�. BP21 -> IS21, BP11 -> IS11
// tree_setitemdata()

enum _xKIND { 
		xALLCODE = 1,	// ������
		xKOSPI,		// �ڽ���
		xKOSDAQ,	// �ڽ���
		xFUTURE,	// ����
		xCALLCODE,	// �ݿɼ�
		xPUTCODE, 	// ǲ�ɼ�	
		xKOPSI200,	// �ڽ��� 200
		xKOSDAQ50,	// �ڽ��� 50
		xKRX100,	// KRX100
		xFREECODE,	// ��������
		xETFCODE,	// ETF		
		xELWCODE,	// ELW
		xELWBASE,	// ELW�����ڻ�
		xSINJUCODE,	// �����μ���
		xKOSPIJISU,	// �ڽ�������
		xKOSDAQJISU,	// �ڽ�������
		xTHEMACODE,	// �׸���
		xGROUPCODE,	// �׷�纰
		xINTEREST,	// ��������
		xPCALL,		// �ֽĿɼ�(��)
		xPPUT,		// �ֽĿɼ�(ǲ)
		xKOSPIUP,	// �ڽ��Ǿ���
		xKOSDAQUP,	// �ڽ��ھ���
		xKOSPIex,	// �ڽ��� �߰�
		xKOSDAQex,	// �ڽ��� �߰�
		xREMAIN,	// ���º� �����ܰ�
		xISSUE,		// �̽�����
		xELWISSUE,	// ����ȸ�纰
		xELWASSET,	// �����ڻ꺰
		xRECOMMAND,  // IBK��õ����
		xETFALL,
		xETFCOMPANY,
		xETFTHEMA,
		xETFDOMINTER,
		xELWGEN,
		xELWEARLY,
		xKOSPIFUTURE,			//�ڽ��Ǽ���
		xSTOCKFUTURE,			//�ֽļ���
		xINTERESTRATEFUTURE,	//�ݸ�����
		xCURRENCYFUTURE,			//��ȭ����
		xKONEX,		// 2013.06.25 KSJ �ڳؽ� �߰�
		xETNCODE,	// 2014.11.06 KSJ ETN �߰�
		xSPACCODE,	// 2015.11.03 KSJ spac �߰�
		xEPBCODE	// 2016.04.08 KSJ EPB ��õ���� �߰�
};

enum _colINDEX
	{
		colRCOD = 0,
		colEXPECT,
		colOPEN, 
		colHIGH, 
		colLOW,   
		colPCURR,  
		colPOPEN, 
		colPHIGH, 
		colPLOW, 
		colVOL,
		colINFO,
		colRATE,
		colTVOL,
		col7852,		//2013.07.01 KSJ ���簡(ó���� ���󰡰� ��ȸ�Ǵ��� ���簡�� ������)
		col7853,		//2013.07.01 KSJ �ŷ���
		col7854,		//2013.07.01 KSJ ���
		col7855,		//2013.07.01 KSJ �����
		colMAR,
		colMAR1,		//2014.07.18 �ſ����ű��� �߰�
		colSIG,			//����/��Ʈ
		colCODE,		//�����ڵ�
		colNAME,		//�����
		colCURR			//���簡
	};
enum _rtmAction { rtmCELL = 0, rtmROW, rtmNONE };
enum _dblAction { dblNONE = 0, dblCURR, dblCHART, dblTRIGGER, dblUSER };

#define STANDARDNUM 11

// treeID.depth
#define TD_ROOT			0x00
#define TD_MID			0x01
#define TD_ITEM			0x02


// DWORD ���� ���� bit structure 
struct _treeID
{
	UINT	kind:7;		// ��з� _xKIND ��
	UINT	skind:7;	// �ߺз� ������ �Ǵ� �ε���
	UINT	degree:2;	// value���� %02d(���ڸ�) %03d(���ڸ�) ����
	UINT	value:10;	// ������ ������ �Ǵ� �ε��� 1 ~ 1024������ ��
	UINT	depth:3;	// ��з�/�ߺз�/������
	UINT	dummy:3;	
};

#define CAST_TREEID(x)	((struct _treeID*)&x)

[[gsl::suppress(26497)]]
static UINT MAKE_TREEID(UINT kind, UINT skind = 0, UINT degree = 0, UINT value = 0, UINT depth = 0, UINT dummy = 0)
{
	UINT	ret = 0;
	struct _treeID*	treeID = (struct _treeID*)&ret;
	treeID->kind = kind;
	treeID->skind = skind;
	treeID->degree = degree;
	treeID->value = value;
	treeID->depth = depth;
	treeID->dummy = dummy;

	return ret;
}

static struct _treeID CONVERT_TREEID(UINT value)
{
	struct _treeID	treeID;
	memcpy(&treeID, &value, sizeof(value));
	return treeID;
}


class CGridData
{
public:
	CGridData() { Reset(); }
	~CGridData() {}

	void Reset() { m_kind = 0, m_arDatas.RemoveAll() ; }
	void SetKind(UINT kind) { m_kind = kind ; }
	UINT GetKind() { return m_kind; }
	int  GetCount() { return m_arDatas.GetSize(); }
	void operator = (CGridData& input)
	{
		this->SetKind(input.GetKind());
		m_arDatas.Copy(input.m_arDatas);
		_gridindex = input._gridindex;
	}

public:
	
	int		_gridindex = -1;
	UINT		m_kind{};
	CStringArray	m_arDatas;	// ����\t��������\t����
};

class CSendData
{
public:
	CSendData() { Reset(); }
	~CSendData() {}
public:
	void Reset()
	{
		m_trcode = _T("");
		m_data = _T("");
		m_userdata = _T("");
		m_key = 0;
	}

	void SetTRCODE(CString trcode) { m_trcode = trcode; }
	void SetData(char* ptr, int len) { m_data = ptr, m_datalen = len; }
	void SetUserData(CString userdata) { m_userdata = userdata; }
	void SetDataKey(int key) { m_key = key; }

	void SetData(CString trcode, int key, char* ptr, int len, CString userdata)
	{
		SetTRCODE(trcode);
		SetDataKey(key);
		SetData(ptr, len);
		SetUserData(userdata);
	}

	CString GetUserData() { return m_userdata; }
	int GetDataKey() { return m_key; }
	CString GetTRCODE() { return m_trcode; }
	char* GetData() { return m_data; }
	int GetDataLen() { return m_datalen; }

	CString	m_trcode;
	char*	m_data;
	int	m_datalen;
	CString m_userdata;
	int	m_key;
};

class CRecvData
{
public:
	CRecvData(){}
	~CRecvData(){}
public:
	void SetData(int type, int len, char* lParam)
	{
		key = type;
		size = len;
		data = lParam;
	}

	int     key{};
	int     size{};
	char*	data{};
};

class CRTSData
{
public:
	CRTSData(){Reset();}
	~CRTSData(){}
public:
	void Reset() { code = "";fms.RemoveAll() ; arChange.RemoveAll();}

	BOOL SetData(CString code,CString datB)
	{
		this->code = code;
		CString symbol,entry;
		int idx = 0;
		bChange = FALSE;
		arChange.RemoveAll();

		BOOL bChae = FALSE;
		BOOL bHo = FALSE;

		bHoga = FALSE;

		while (!datB.IsEmpty())
		{
			idx = datB.Find('\t');
			if (idx == -1) break;
			
			symbol = datB.Left(idx++);
			datB = datB.Mid(idx);
			idx = datB.Find('\t');
			
			if (idx == -1)
			{
				entry = datB;	
				datB = _T("");
			}
			else
			{
				entry = datB.Left(idx++); 
				datB = datB.Mid(idx);
			}
			
			if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0")
			{
				entry = " ";
			}
			
			CString tmp;

			if(fms.Lookup(symbol,tmp))
			{
				if(tmp != entry)
				{
					arChange.SetAt(symbol,"1");
					bChange = TRUE;
				}
			}
			else
			{
				arChange.SetAt(symbol,"1");
				bChange = TRUE;
			}
			fms.SetAt(symbol, entry);

			if(symbol == "034")
			{
				bChae = TRUE;
			}
			else if(symbol == "040")
			{
				bHo = TRUE;
			}
		}

		if(bChae == FALSE && bHo == TRUE)
		{
			bHoga = TRUE;
		}

		return bChange;
	}

	BOOL SetData(CString code,CString datB,CMapStringToString &fmsSrc)
	{
		this->code = code;
		CString symbol,entry;
		int idx = 0;
		bChange = FALSE;
		arChange.RemoveAll();
		
		BOOL bChae = FALSE;
		BOOL bHo = FALSE;
		
		bHoga = FALSE;
		
		while (!datB.IsEmpty())
		{
			idx = datB.Find('\t');
			if (idx == -1) break;
			
			symbol = datB.Left(idx++);
			datB = datB.Mid(idx);
			idx = datB.Find('\t');
			
			if (idx == -1)
			{
				entry = datB;	
				datB = _T("");
			}
			else
			{
				entry = datB.Left(idx++); 
				datB = datB.Mid(idx);
			}
			
			if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0")
			{
				entry = " ";
			}
			
			CString tmp;
			
			if(fms.Lookup(symbol,tmp))
			{
				tmp.TrimLeft();
				tmp.TrimRight();

				if(tmp != entry)
				{
					arChange.SetAt(symbol,"1");
					bChange = TRUE;
				}
				else
				{
					if(symbol == "111")
					{
						if(fms.Lookup("023",tmp))
						{
							if(tmp != entry)
							{
								fms.SetAt("023",entry);
								bChange = TRUE;
							}
						}
					}
					else if(symbol == "115")
					{
						if(fms.Lookup("024",tmp))
						{
							if(tmp != entry)
							{
								fms.SetAt("024",entry);
								bChange = TRUE;
							}
						}
					}
					else if(symbol == "116")
					{
						if(fms.Lookup("033",tmp))
						{
							if(tmp != entry)
							{
								fms.SetAt("033",entry);
								bChange = TRUE;
							}
						}
					}
				}
				

				if(symbol == "023")
				{
					bChange = TRUE;
				}
				else if(symbol == "024")
				{
					bChange = TRUE;
				}
				else if(symbol == "033")
				{
					bChange = TRUE;
				}
// 				else if(symbol == "111" || symbol == "115" || symbol == "116")
// 				{
// 					if(symbol == "111")
// 					{
// 						if(fms.Lookup("023",tmp))
// 						{
// 							if(tmp != entry)
// 								bChange = TRUE;
// 						}
// 					}
// 					else if(symbol == "115")
// 					{
// 						if(fms.Lookup("024",tmp))
// 						{
// 							if(tmp != entry)
// 								bChange = TRUE;
// 						}
// 					}
// 					else if(symbol == "116")
// 					{
// 						if(fms.Lookup("033",tmp))
// 						{
// 							if(tmp != entry)
// 								bChange = TRUE;
// 						}
// 					}
// 				}
			}
			else
			{
				arChange.SetAt(symbol,"1");
				bChange = TRUE;
			}

			if(bChange == TRUE || symbol == "111" || symbol == "023")
				fmsSrc.SetAt(symbol, entry);

			fms.SetAt(symbol,entry);
			
			if(symbol == "034")
			{
				bChae = TRUE;
			}
			else if(symbol == "040")
			{
				bHo = TRUE;
			}
		}
		
		if(bChae == FALSE && bHo == TRUE)
		{
			bHoga = TRUE;
		}
		
		return bChange;
	}

	CString GetData(CString symbol)
	{
		CString tmp;

		if(fms.Lookup(symbol,tmp))
		{
			return tmp;
		}

		return ""; 
	}

	BOOL IsHoga()
	{
		return bHoga;
	}

	BOOL Lookup(CString symbol)
	{
		CString value;
		return  fms.Lookup(symbol,value);
	}

	CString GetSymbolData(CString symbol)
	{
		CString value;

		fms.Lookup(symbol,value);

		return value;
	}

	BOOL IsChange(CString symbol)
	{
		CString tmp;
		return arChange.Lookup(symbol,tmp);
	}

	void RemoveAll(){fms.RemoveAll();}

	CString code;
	CMapStringToString fms;
	CMapStringToString arChange;
	BOOL	bChange;
	BOOL	bHoga;
};

class CIHQueue
{
public:
	CIHQueue() {}
	~CIHQueue() {}

	int GetCount()
	{
		return m_mapData.GetCount();
	}

	void RemoveAll()
	{
		m_arKey.RemoveAll();
		m_arValue.RemoveAll();
		m_mapKeyToIndex.RemoveAll();
		m_mapData.RemoveAll();
	}

	void RemoveKey(WORD key)
	{
		WORD	index;
		if (m_mapKeyToIndex.Lookup(key, index))
		{
			m_arKey.RemoveAt(index);
			m_arValue.RemoveAt(index);
			reIndex();
		}
		m_mapData.RemoveKey(key);
	}

	CSendData GetValue(int nIndex)
	{
		CSendData	ret;
		if (nIndex < 0 || nIndex >= m_arValue.GetSize())
			return ret;

		return m_arValue.GetAt(nIndex);
	}

	WORD GetKey(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_arKey.GetSize())
			return 0;

		return m_arKey.GetAt(nIndex);
	}

	CSendData GetData(WORD key)
	{
		CSendData	sdata;
		m_mapData.Lookup(key, sdata);
		return sdata;
	}

	void SetData(WORD key, CSendData value)
	{
		m_mapData.SetAt(key, value);
		const int nIndex = m_arValue.Add(value);
		m_arKey.Add(key);
		m_mapKeyToIndex.SetAt(key, nIndex);
	}

	BOOL IsExist(WORD key)
	{
		CSendData value;
		return m_mapData.Lookup(key, value);
	}

	void reIndex()
	{
		const int ncnt = m_arKey.GetSize();
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			m_mapKeyToIndex.SetAt(m_arKey.GetAt(ii), ii);
		}
	}
	
private:
	CArray<CSendData, CSendData>		m_arValue;
	CWordArray				m_arKey;
	CMap<WORD, WORD, WORD, WORD>		m_mapKeyToIndex;
	CMap<WORD, WORD, CSendData, CSendData>		m_mapData;
};

struct	_uinfo {
	char	gubn[2];	// ��������
				// "MY" : ��������
				// "UL" : ����� ������ ��� 
				// "UD" : ����� ������
	char	dirt[1];	// ���۹��� 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : ����ȸ
	char	cont[1];	// ���ӱ��� 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel
	char	name[80];	// file name
	char	nblc[5];	// ������, ����ũ��(max:64k)
	char	retc[1];	// return value 'O':���� 'E':����
	char	emsg[40];	// error message
};

struct	_ginfo {
	char	gnox[2];	// group number
	char	gnam[20];	// group name
	char	jrec[4];	// _jinfo count
//	char	jptr[1];	// _jinfo pointer
};

struct	_jinfo {
	char	gubn[1];	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	xprc[10];	// �����ܰ�
	char	xnum[10];	// ��������
};

/*
struct	_bookmarkinfo {
	char	gubn[1];	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	int		bookmark;	// �ϸ�ũ ����
};
*/
struct	_updn {
	_uinfo	uinfo;
	_ginfo	ginfo;
};

const int sz_uinfo   = sizeof(struct _uinfo);
const int sz_ginfo   = sizeof(struct _ginfo);
const int sz_jinfo   = sizeof(struct _jinfo);
const int sz_updn    = sizeof(struct _updn);

struct _marker {
	COLORREF clrMarkerBKG;
	COLORREF clrMarkerTXT;
	BOOL bShadow;
	BOOL bAllApply;
};

#define	trN	"pibf2101"	//2012.12.05 KSJ pibo2101 --> pibf2101

struct	_inR	{
	char	func[1];		// 'S' : set
							// 'C' : clear
	char	cvol[3];		// ü�ᷮ ����   (cvol * 1,000)
	char	camt[3];		// ü��ݾ� ���� (camt * 10,000,000)
	char	ador[1];		// 'a' : and
					// 'o' : or
	char	prcf[1];		// ���� ���� ���� flag
					// 1 or 0
	char	sprc[7];		// ���ݹ��� ���۰�
	char	eprc[7];		// ���ݹ��� ����
	char	updn[5];		// ����, ���, ����, ����, �϶�
					// 1 or 0
    char    tree[2];            /* 00: ��ü                       */
                                /* 01: �ŷ���,      02: �ڽ���      */
                                /* 03: KP200,       04: KRX100      */
                                /* 05: ��������,    06: ETF         */
                                /* 07: ELW,         08: ELW�����ڻ� */
                                /* ������ ������ client���� ó��*/
};

#define	L_inR		sizeof(struct _inR)

struct	_outR	{
	char	retc[1];		// return code
					// 0 : OK
					// 9 : err
};
#define	L_outR		sizeof(struct _outR)

struct	_rtsR	{
	char	flag[3];		// flag (= "W__" : w + blank + blank)
	char	datl[3];		// message length (key length + data length)
};
// + datb		// real-time message
			// key(="quote") \t data(cols \t cols \t ...)
#define	L_rtsR		sizeof(struct _rtsR)

/*******************************************************************************
 * ELW�����ڻ꺰
 ******************************************************************************/
struct elw_mid
{
	char iflag[1];		//
	char ASCF[1];		//0:���� 1:����
	char CBKCOD[1];		//�Ǹ�����code 
	char ELWUD[12];		//�����ڻ�code
	char ELWLP[12];		//�����code
	char JJIS[1];		//�����ϼ�
	char CO_SORT[2];		//sorting ����
	char RTSF[9];		//1.RTS 0.NOT
	char C_GVOL[1];		//1:�׸������
	char gsrt[1];		
	char dummy[1];
};

struct elw_grid
{
    char    seqn[4];
    char    cod2[7];            /* RTS COD      */
    char    code[6];            /* �����ڵ�     */
    char    hnam[20];           /* �����       */
    char    curr[8];            /* ���簡       */
    char    diff[7];            /* ���         */
    char    rate[6];            /* �����       */
    char    mdga[8];            /* �ŵ�ȣ��     */
    char    msga[8];            /* �ż�ȣ��     */
    char    cvol[9];            /* ü�ᷮ       */
    char    gvol[9];            /* �ŷ���       */
    char    gamt[12];           /* �ŷ����     */
    char    dvol[8];            /* ���ŵ��ܷ�   */
    char    svol[8];            /* ���ż��ܷ�   */
    char    irga[7];            /* �̷а�       */
    char    grat[7];            /* ������       */
    char    prty[7];            /* �и�Ƽ       */
    char    gerr[7];            /* ������   */
    char    brer[7];            /* ���ͺб���   */
    char    cfpv[7];            /* �ں�������   */
    char    gcjs[18];           /* �����ڻ�     */
    char    bhsa[20];           /* �����       */
    char    jjis[6];            /* �����ϼ�     */
    char    hsgg[9];            /* ȯ�갡       */
    char    hsga[9];            /* ��簡��     */
    char    jhre[9];            /* ��ȯ����     */
    char    delt[9];            /* ��Ÿ         */
    char    eger[9];            /* e*��     */
};

struct elw_mod
{
	char oflag[1];
	char nrec[4];				//Grid Row ����
	struct elw_grid rec[1];
};




/*******************************************************************************
 * �������� ��ȸ
 ******************************************************************************/
struct rem_mid
{
	char nrec[5];	// ������ 1
	char accn[11];	// ���¹�ȣ
	char fill[9];
};

struct o_rec
{
	char code[12];
};

struct rem_mod
{
	char irec[5];
	char accn[11];
	char fill[9];
	char nrec[5];
	struct o_rec grid[100];
};

#define L_rem_mid	sizeof(rem_mid)
#define L_rem_mod	sizeof(rem_mod)
 

/*******************************************************************************
 * �ֽ��ܰ� ��ȸ
 ******************************************************************************/
struct jango_mid
{
	char nrec[5];	// ������ 1
	char accn[11];	// ���¹�ȣ
	char fill[9];
	char pass[8];	//password
	char zQryTp;	//��ȸ���� 0;��ü 1;ELW 2;ELW����
	char zD2balBase;	//D2�ܰ������ȸ 0;��ü 1;D2�ܰ�0����
};

struct out_rec
{
	char code[12];					//�����ڵ�
	char marketCode[2];				//��Ͻ����ڵ�
	char qty[16];					//�ܰ����
	char todayBuyChegQty[16];		//���ϸż�ü�����
	char todaySellChegQty[16];		//���ϸŵ�ü�����
	char dealbaseQty[16];			//�Ÿű����ܰ����
	char AvrUprc[13];				//��մܰ�
	char NowPrc[13];				//���簡
	char buyAmt[16];				//�ż��ݾ�
	char EvalAmt[16];				//�򰡱ݾ�
	char EvalSonikAmt[16];			//�򰡼��ͱݾ�
	char sonikRate[12];				//���ͷ�
	char creditAmt[16];				//�ſ�ݾ�
	char loanDt[8];					//������
	char balPtnCode[2];				//���������ܰ������ڵ�
	char sellAbleQty[16];			//�ŵ����ɼ��� 
	char sellUnercQty[16];			//�ŵ���ü�����
	char loanDtlcode[2];			//����󼼺з��ڵ�
	char remainLoanAmt[16];			//�ܿ�����ݾ�
};

struct jango_mod
{
	char inout[35];
	char irec[5];
	char dummy[187];
	char nrec[5];
	struct out_rec grid[100];
};

#define L_jango_mid	sizeof(jango_mid)
#define L_jango_mod	sizeof(jango_mod)


/*******************************************************************************
 * �����ܰ� ��ȸ
 ******************************************************************************/
struct future_mid
{
	char nrec[5];
	char accn[11];	// ���¹�ȣ
	char fill[9];
	char pass[8];	//password
};

struct out_grid
{
	char zFnoIsuNo[32];				//�����ȣ
	char zIsuNm[40];				//����� 
	char zIsuMdclssCode[2];			//�����ߺз��ڵ�
	char zFnoIsuPtnTp[1];			//�����ɼ�������������
	char zBnsTp[1];					//�Ÿű��� 	
	char dFnoTrdUnitAmt[19];		//�����ɼǰŷ������ݾ�		
	char lUnsttQty[16];				//�̰������� 
	char lLqdtOrdAbleQty[16];		//û���ֹ����ɼ���
	char dAvrUprc[13];				//��մܰ�
	char lUnsttAmt[16];				//�̰����ݾ�
	char dCurprc[13];				//���簡
	char lFnoEvalAmt[16];			//�����ɼ��򰡱ݾ�
	char lEvalPnl[16];				//�򰡼���
	char dPnlRat[12];				//���ͷ�
};

struct future_mod
{
	char inout[33];
	char out0[5];
	char lAllBnsplAmt[16];
	char nrec[5];
	struct out_grid grid[100];
};

#define L_future_mid	sizeof(future_mid)
#define L_future_mod	sizeof(future_mod)

#define FutureService _T("SONBQ021")
//#define StockService  _T("SONAQ052")  //20200423 �ܰ���ȸTR����
#define StockService  _T("SONAQ452")


/*******************************************************************************
* �ֽ� �ܰ��� ��ȸ IO FORMAT
******************************************************************************/
struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.�ſ����� 1.All */
    char    fill[10];
};

struct  s_rec   {
    char    cod2[12];   /* �����ڵ�         */
    char    hnam[40];   /* �����           */
    char    jggb[2];    /* �ܰ��ڵ�         */
    char    jqty[10];   /* �ܰ����         */
    char    xqty[10];   /* �ŵ�����         */
    char    pamt[10];   /* ������հ�       */
    char    mamt[15];   /* ���Աݾ�         */
	
    char    curr[10];   /* ���簡     (*)   */
	char    diff[10];   /* ���ϴ��         */
    char    rate[10];   /* �����           */
    char    camt[15];   /* �򰡱ݾ�         */
    char    tamt[15];   /* �򰡼���   (*)   */
    char    srat[10];   /* �򰡼��ͷ� (*)   */
    char    sycd[2];    /* �ſ��ڵ�         */
    char    sydt[8];    /* ������           */
    char    samt[15];   /* �ſ�ݾ�         */
}; 
struct  s_mod   {
    char    accn[11];
    char    nrec[4];
    struct  s_rec   rec[100];
}; 

/*******************************************************************************
* ����/�ɼ� �ܰ��� ��ȸ IO FORMAT
******************************************************************************/
struct	f_mid	{		// ���� Input
	char	accn[11];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* �����ڵ�		*/
	char	hnam[30];	/* �����		*/
	char	dsgb[6];	/* ����			*/
	
	char	bqty[10];	/* ����			*/
	char	cqty[10];	/* �ŵ����ɼ���		*/
	char	pprc[10];	/* ��հ�/���갡	*/
	char	curr[10];	/* ���簡		*/
	char	diff[10];	/* ��հ� ���		*/
	char	pamt[15];	/* �򰡱ݾ�		*/
	char	psam[15];	/* �򰡼���		*/
	char	prat[10];	/* ���ͷ�		*/
	char	tamt[15];	/* ���Աݾ�		*/
};

struct	f_mod	{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[100];
};

#define L_smid	sizeof(struct s_mid)				// Length of stock input
#define L_smod	sizeof(struct s_mod) - sizeof(struct s_rec)	// Length of stock output
#define L_srec	sizeof(struct s_rec)				// Length of stock Grid

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid
/*
struct i_rec
{
	char zRmnCnfTp[1];				// �Ǹ�Ȯ�α��� 
	char zOtptTp[1];				// ��±���     
	char zBalTrxTp[1];				// �ܰ�ó������ 
	char zIntaccNo[20];				// ���հ��¹�ȣ 
	char zPwd[8];					// ��й�ȣ     
};

struct rem_mid
{
	char In[5];						// ���ڵ尹��   
	struct i_rec rec[99];
};

struct o_remrec
{
	char zAcntNo[20];				// ���¹�ȣ             
	char zAcntNm[40];				// ���¸�               
	char zIsuNo[12];				// �����ȣ             
	char zIsuNm[40];				// �����               
	char zTp[1];					// ����                 
	char zTpNm1[40];				// ���и�1              
	char lHldQty[15];				// ��������             
	char dAvrUprc[9];				// ��մܰ�             
	char dCurPrc[9];				// ���簡               
	char lEvalAmt[15];				// �򰡱ݾ�             
	char dEvalRat[3];				// �򰡺���             
	char lEvalPnlAmt[15];			// �򰡼��ͱݾ�         
};

struct o_prcrec
{
	char zAcntNoClssPtnCode[2];		// ���¹�ȣ�з������ڵ� 
	char zAcntNo[20];				// ���¹�ȣ             
	char zAcntNm2[40];				// ���¸�               
	char zPdptnCode[2];				// ��ǰ�����ڵ�         
	char zPdptnNm[40];				// ��ǰ������           
	char lPrsmptDpsD2[15];			// ����������(D+2)      
	char lFnoEvalAmt[15];			// �����ɼ��򰡱ݾ�     
	char lFnoEvalPnlAmt[15];		// �����ɼ��򰡼��ͱݾ� 
	char lStkEvalAmt[15];			// �ֽ��򰡱ݾ�         
	char lStkEvalPnlAmt[15];		// �ֽ��򰡼��ͱݾ�     
	char lBndEvalAmt[15];			// ä���򰡱ݾ�         
	char lBndEvalPnlAmt[15];		// ä���򰡼��ͱݾ�     
	char lIvtrstEvalAmt[15];		// ���ڽ�Ź�򰡱ݾ�     
	char lIvtrstEvalPnlAmt[15];		// ���ڽ�Ź�򰡼��ͱݾ� 
	char lEtcSecEvalAmt[15];		// ��Ÿ�����򰡱ݾ�     
	char lEtcSecEvalPnlAmt[15];		// ��Ÿ�����򰡼��ͱݾ� 
	char lCrdtMloanAmt[15];			// �ſ����ڱݾ�         
	char lLoanAmt[15];				// ����ݾ�             
	char lSloanPldgAmt[15];			// ���ִ㺸�ݾ�         
	char lIndvAcntSum[15];			// �������¼Ұ�         
	char SecDtl[5];					// ���ڵ尹��           
	struct o_remrec rec[15];
};

struct o_rec
{
	char zAcntNm[40];				// ���¸�               
	char zClntNm[40];				// ����               
	char BalSt[5];					// ���ڵ尹��           
	struct o_prcrec rec[1];
};

struct rem_mod
{
	char Out[5];					// ���ڵ尹��           
	struct o_rec rec[1];
};

#define L_rem_mid	sizeof(rem_mid)
#define L_i_rec		sizeof(i_rec)
#define L_rem_mod	sizeof(rem_mod)
#define L_o_rec		sizeof(o_rec)
#define L_o_prcrec	sizeof(o_prcrec)
#define L_o_remrec	sizeof(o_remrec)

*/

