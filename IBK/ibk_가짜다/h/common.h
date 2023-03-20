#if !defined(GRAPH_COMMON_H)
#define GRAPH_COMMON_H

///////////////////////////////////////////////////////////////////////////
#include "graph.h"

#define	rgbCOLOR		0x02000000

const DWORD	_BLACK		= PALETTERGB(0, 0, 0);
const DWORD	_BLUE		= PALETTERGB(0, 0, 192);
const DWORD	_GREEN		= PALETTERGB(0, 192, 0);
const DWORD	_CYAN		= PALETTERGB(0, 192, 192);
const DWORD	_RED		= PALETTERGB(192, 0,0);
const DWORD	_MAGENTA	= PALETTERGB(192, 0, 192);
const DWORD	_BROWN		= PALETTERGB(132,  66,  66);
const DWORD	_YELLOW		= PALETTERGB(192, 192,   0);
const DWORD	_WHITE		= PALETTERGB(192, 192, 192);
const DWORD	_GRAY		= PALETTERGB(179, 179, 179);

const DWORD	_L_BLUE		= PALETTERGB(0, 0, 255);
const DWORD	_L_GREEN	= PALETTERGB(0, 255,   0);
const DWORD	_L_CYAN		= PALETTERGB(0, 255, 255);
const DWORD	_L_RED		= PALETTERGB(255,   0,   0);
const DWORD	_L_MAGENTA	= PALETTERGB(192,   0, 192);
const DWORD	_L_BROWN	= PALETTERGB(154,  78,  78);
const DWORD	_L_YELLOW	= PALETTERGB(255, 255,   0);
const DWORD	_L_WHITE	= PALETTERGB(255, 255, 255);
const DWORD	_L_GRAY		= PALETTERGB(224, 224, 224);

const DWORD	_M_BLUE		= PALETTERGB(0,   0, 144);
const DWORD	_M_GREEN	= PALETTERGB(0, 144,   0);
const DWORD	_M_CYAN		= PALETTERGB(0, 144, 144);
const DWORD	_M_RED		= PALETTERGB(144,   0,   0);
const DWORD	_M_MAGENTA	= PALETTERGB(144,   0, 144);
const DWORD	_M_BROWN	= PALETTERGB(112,  56,  56);
const DWORD	_M_YELLOW	= PALETTERGB(144, 144,   0);
const DWORD	_M_WHITE	= PALETTERGB(144, 144, 144);
const DWORD	_M_GRAY		= PALETTERGB(128, 128, 128);

const DWORD	_UPCOLOR	= 171;	// ���
const DWORD	_DNCOLOR	= 170;	// ����
const DWORD	_DCCOLOR	= 173;	// ��¥���漱

const COLORREF	_VOLUME		= 172;	// �ŷ���
const COLORREF	_LINE		= 174;	// tick chart line


// color used in Market profile
const COLORREF	_RGB017		= PALETTERGB( 224, 234, 215);
const COLORREF	_RGB026		= PALETTERGB( 239, 214, 198);
const COLORREF	_RGB027		= PALETTERGB( 231, 231, 214);
const COLORREF	_RGB096		= PALETTERGB(  51,   0, 102);
const COLORREF	_RGB113		= PALETTERGB( 108,  11,   1);
const COLORREF	_RGB185		= PALETTERGB( 252, 196,   0);
const COLORREF	_RGB246		= PALETTERGB( 255, 251, 240);

// input
const COLORREF	_TITLE		= 160;
const COLORREF	_TITLETEXT	= 161;

// sise
const COLORREF	_TEXT		= 163;
const COLORREF	_GRID		= 162;
const COLORREF	_BORDER		= 164;
const COLORREF	_C_CALL		= 163;
const COLORREF	_C_PUT		= 163;

// ctrl
const COLORREF	_CTRLBACK	= 165;

const COLORREF	_FONTCOLOR	= 168;
const COLORREF	_FGCOLOR	= 167;
const COLORREF	_ADCOLOR	= 169;
const COLORREF	_BGCOLOR	= 166;

const COLORREF	_DEFAULT1	= 175;	// �̵����
const COLORREF	_DEFAULT2	= 176;
const COLORREF	_DEFAULT3	= 177;
const COLORREF	_DEFAULT4	= 178;
const COLORREF	_DEFAULT5	= 179;
const COLORREF	_DEFAULT6	= 180;
const COLORREF	_DEFAULT7	= 181;
const COLORREF	_DEFAULT8	= 182;
const COLORREF	_DEFAULT9	= 183;
const COLORREF	_DEFAULT10	= 184;

const COLORREF	_CHART1		= 185;	// ������Ʈ
const COLORREF	_CHART2		= 186;
const COLORREF	_CHART3		= 187;
const COLORREF	_CHART4		= 188;
const COLORREF	_CHART5		= 189;
const COLORREF	_CHART6		= 190;
const COLORREF	_CHART7		= 191;
const COLORREF	_CHART8		= 192;
const COLORREF	_CHART9		= 193;



const COLORREF	_N_BLACK	= PALETTERGB(0, 0, 0);
const COLORREF	_N_CYAN		= PALETTERGB(165, 205, 254);
const COLORREF	_N_YELLOW	= PALETTERGB(255, 255, 125);
const COLORREF	_N_GREEN	= PALETTERGB(75, 226, 90);


enum	{dpNONE = 0, dpCOMMON = 1, dpWKVOL = 2, dpNMVAL = 3,
				dpVAL = 4, dpVAL2 = 5, dpVIDX = 6, dpTICK = 7};

#define	compressDAY	-1

#define	maxEMAV		20
#define	maxTMAV		10
#define	trnMAV		1
#define eprMAV		2
#define	amtMAV		3

#define	volMAXW		4

// realtime symbol
#define	R_CODX	"021"	// �����ڵ�(�����ڵ�5)
#define	R_HNAM	"022"	// �����
#define	R_CURR	"023"	// ���簡
#define	R_DIFF	"024"	// ���ϴ��
#define	R_MDGA	"025"	// �ŵ�ȣ��
#define	R_MSGA	"026"	// �ż�ȣ��
#define	R_GVOL	"027"	// �ŷ���
#define	R_GAMT	"028"	// �ŷ����(��)
#define	R_SIGA	"029"	// �ð�
#define	R_KOGA	"030"	// ��
#define	R_JEGA	"031"	// ����
#define	R_CVOL	"032"	// ü��ŷ���(ü�����)
#define	R_UDYL	"033"	// �����
#define	R_CTIM	"034"	// ü��ð�
#define	R_CCOL	"035"	// ü�ᱸ��
#define	R_HTIM	"040"	// ȣ���ð�
#define	R_MGJY	"201"	// �̰�������(����)

// ECN realtime Symbol
#define	ER_HNAM	"722"	// �����
#define	ER_CURR	"723"	// ���簡
#define	ER_DIFF	"724"	// ���ϴ��
#define	ER_MDGA	"725"	// �ŵ�ȣ��
#define	ER_MSGA	"726"	// �ż�ȣ��
#define	ER_GVOL	"727"	// �ŷ���
#define	ER_GAMT	"728"	// �ŷ����(��)
#define	ER_SIGA	"729"	// �ð�
#define	ER_KOGA	"730"	// ��
#define	ER_JEGA	"731"	// ����
#define	ER_CVOL	"732"	// ü��ŷ���(ü�����)
#define	ER_UDYL	"733"	// �����
#define	ER_CTIM	"734"	// ü��ð�
#define	ER_CCOL	"735"	// ü�ᱸ��
#define	ER_HTIM	"740"	// ȣ���ð�

struct _gIndex{
	union {
		struct {
			short		yy;	// year
			unsigned char	mm;	// month
			unsigned char	dd;	// day
		}date;
		struct {
			unsigned char	hh;	// hour
			unsigned char	mm;	// minute
			unsigned char	ss;	// second
			unsigned char	tk;	// GI_HMS only
			unsigned char	dc;	// daya change
		}time;
	}index;
};
#define	sz_gIndex	sizeof(struct _gIndex)

struct	_gCommon {
	int		fpr;			// �ð�
	int		hpr;			// ��
	int		lpr;			// ����
	int		epr;			// ����
	double		trn;			// �ŷ���
	double		amt;			// �ŷ����
	union {
		struct {
			short		yy;	// year
			unsigned char 	mm;	// month
			unsigned char	dd;	// day
		}date;
		struct {
			unsigned char	hh;	// hour
			unsigned char	mm;	// minute
			unsigned char	ss;	// second
			unsigned char	tk;	// GI_HMS only
			unsigned char	dc;	// day change
		}time;
	} index;
	unsigned char	check;			// information check
	int		rate;			// check rate
	double		stock;			// �ֽļ�   -> used in ȸ����
	double		pmav[maxEMAV];		// ���� ����
	double		tmav[maxTMAV];		// �ŷ��� ����
	double		amav[maxTMAV];		// �ŷ���� ����
};
#define	sz_gCOMMON	sizeof(struct _gCommon)

struct	_gTick {
	int		epr;			// ����
	double		trn;			// �ŷ���
	struct {
		unsigned char	hh;	// hour
		unsigned char	mm;	// minute
		unsigned char	ss;	// second
		unsigned char	tk;	// GI_HMS only
		unsigned char	dc;	// day change
	} time;
};
#define	sz_gTICK	sizeof(struct _gTick)

struct _gWkvol {
	short		syy;		// ����
	unsigned char	smm;		// ����
	unsigned char	sdd;		// ����
	short		eyy;		// �ָ�
	unsigned char	emm;		// �ָ�
	unsigned char	edd;		// �ָ�

	char		name[20];
	DWORD		color;
	double		trn;		// �ŷ���
};
#define	sz_gWKVOL	sizeof(struct _gWkvol)

struct _gNameval {
	char		name[20];	// 
	DWORD		color;
	double		val;
};
#define	sz_gNAMEVAL	sizeof(struct _gNameval)

struct _gVal {
	double		val;
};
#define	sz_gVAL		sizeof(struct _gVal)

struct _gVal2 {
	double		val1;
	double		val2;
};
#define sz_gVAL2	sizeof(struct _gVal2)

struct _gVidx {
	double		val;
	union {
		struct {
			short		yy;	// year
			unsigned char	mm;	// month
			unsigned char	dd;	// day
		}date;
		struct {
			unsigned char	hh;	// hour
			unsigned char	mm;	// minute
			unsigned char	ss;	// second
			unsigned char	tk;	// GI_HMS only
			unsigned char	dc;	// day change
		}time;
	} index;
};
#define	sz_gVIDX	sizeof(struct _gVidx)

struct _input {
	char		jcode[16];	// �ڵ�
	unsigned char	type;		// ����,������ graph.h ����
	char		count[4];	// ���Ű���
	char		day[8];		// ��������
	unsigned char	gubn;		// ��/��/��/��/ƽ => graph.h����
	char		gigan[3];	// ����Ʈ�� ��
					//    0 : ƽ��Ʈ	  
					//  9xx : xx�� ��Ʈ      
					//  xxx : xx�� ��Ʈ
	char		snum[3];	// �Ź���Ʈ ����
	char		mkgap[8];	// market gap
	char		reserve[29];	// reserved
};
#define	sz_INPUT	sizeof(struct _input)

struct _output {
	//char		error;
	char		day[8];		// ����
	char		name[20];	// �����
	char		cur[12];	// ���簡
	unsigned char	sign;		// ����ȣ
					// 1   : ���� ��     
					// 2   : ��� ��    
					// 3   : ����    
					// 4   : ���� ��   
					// 5   : �϶� ��
					// '+' : ��� ��   
					// '-' : �϶� ��   
					// ' ' : ����
	char		diff[12];	// ���
	char		rate[7];	// �����
	char		dcol;		// �ŵ�ȣ�� ����
	char		mdga[12];	// �ŵ�ȣ��
	char		scol;		// �ż�ȣ�� ����
	char		msga[12];	// �ż�ȣ��
	char		gcol;		// �ŷ��� ����
	char		gvol[15];	// �ŷ���
	char		rcod[17];	// realtime code
	char		reserve[23];	// reserved
};
#define	sz_OUTPUT	sizeof(struct _output)
	
/************* gexgrp total data format *****************************************/
/*										*/
/*	struct _inData {							*/
/*		struct _input	input;						*/
/*		struct _ugraphH	ugraphH;					*/
/*		struct _dataH	data[ugraphH.ndat];				*/
/*	};									*/
/*										*/
/*	struct _outData {							*/
/*		struct _output	output;						*/
/*		struct _dgraphH	dgraphH;					*/
/*		struct								*/
/*		{								*/
/*			struct _dataH	dataH;					*/
/*			char		data[dataH.date * sizeof(dataH.dkind)];	*/
/*		}graph[dgraphH.ndat];						*/
/*	};									*/
/*										*/
/********************************************************************************/

#define	ENV_NONE		0x0000
#define	ENV_BONG		0x0001		// draw bong chart
#define	ENV_LINE		0x0002		// draw line chart
#define	ENV_USA			0x0004		// usa
#define	ENV_LOG			0x0010		// apply log
#define	ENV_MOD			0x0100		// modified data
#define	ENV_DUP			0x0200		// duplicated graph

#define	ENV_NM_LINE		0x0001		// normal line
#define	ENV_ST_LINE		0x0002		// step line

const	int	heightCtrl = 23;
const	int	heightInput = 23;
const	int	heightSise = 22;
const	int	widthTool = 26;

enum { toolNO = 0, toolVIEW, toolNUM, toolTEND, toolINPUT, toolFIBOLINE, toolFIBOFAN, toolFIBOARC,
toolFIBORET, toolGANNLINE, toolGANNFAN, toolACCELFAN, toolACCELARC, toolCROSS, toolHISTORY, toolDELETE};

#define	GEV_VIEW		WM_USER+2
#define	GEV_GRAPH		WM_USER+3
#define	GEV_INPUT		WM_USER+4
#define	GEV_SISE		WM_USER+5
#define	GEV_CTRL		WM_USER+6
#define	GEV_TOOL		WM_USER+7
#define	GEV_LIST		WM_USER+8

// GEV_VIEW
#define	viewSend		0x01	// not
#define	viewGraphClear		0x02	// not
#define	viewGetPalette		0x03	// lParam : color
#define	viewSaveGraph		0x04	// lParam : fgraph
					// HIWORD(wParam) : clear
					// TRUE:clear, FALSE:not clear
#define	viewWriteSetting	0x05	// lParam : int data
					// HIWORD(wParam) : save type
					// 1:gubn, 2:time
#define	viewFGraph		0x06	// lParam : fgraph
					// HIWORD(wParam) : type
					// 1:save, 2:get fgraph, 3:initial fgraph
#define	viewDup			0x07	// lParam : dup data
					// HIWORD(wParam) : save type
					// 1:save, 2:get data
#define	viewHistory		0x08	// HIWORD(wParam) : type
					// 1: save, lParam : sym \t code
					// 2: get data, lParam : SYM -> history(code + name \t ...) 
//#define	viewShowTick		0x09	// HIWORD(wParam) : view type
//					// FALSE:hide, 1:normal show, 2:cross tip
//					// lParam : data
#define	viewInput		0x0A	// HIWORD(wParam) : type
					// 1: get index, return index
					// 2: change index, lParam:index
					// 3: get kind, return kind
					// 4: change kind, LOWORD(lParam):kind
					//	HIWORD(lParam): 1(date change), 0(not change)
					// 5: toolview
#define	viewAddGrp		0x0B	// lParam : addGrp data
					// HIWORD(wParam) : save type
					// 1:save, 2:get data

// GEV_GRAPH
#define	graphData		0x01	// HIWORD(wParam) : type
					// 1:get data, 2:set data, 3:alert, 4:clear, 5:object name return
#define	graphChangeGraph	0x02	// lParam : graphi
#define	graphType		0x03	// HIWORD(wParam) : graph type
#define	graphSetGrx		0x04	// lParam : ���� ��Ʈ type
#define	graphGetPalette		0x05	// lParam : color
#define	graphConfig		0x06	// HIWORD(wParam) : type
					// 1:config, 2:append, 3: total setup, 4:chart initial
#define	graphCtrl		0x07	// HIWORD(wParam) : type
					// 1:get scroll data, return data len, lParam:data reference
					// 2:change pos, LOWORD(lParam) : dispPos, HIWORD(lParam) : dispEnd
					// 3:change shift, lParam:shift
					// 4:autoscroll, lParam:TRUE(autoscroll),FALSE(not)
					// 5:change scale, lParam:scale
					// 6:tool select, lParam:select
					// 7:excel
					// 8:edit status, return editable
					// 9:change display, lParam:display cnt
					// 10:delete key event

// GEV_INPUT
#define	inputGetData		0x01	// lParam : get input data, reference
					// HIWORD(wParam) : 1:input data, 2:code
					//		3:output data ok(save history, trigger, not)
					//		4:get gubn
#define	inputDomino		0x0A	// lParam : domino data
#define	inputDisp		0x02	// HIWORD(wParam) : enable(disCnt == 0), lParam : dispCnt
#define	inputSetCode		0x03	// lParam : code
#define	inputSetFocus		0x04	// code edit setfocus
#define	inputDefGraph		0x06	// lParam : define Graph type
#define	inputIndex		0x07	// HIWORD(wParam) : type
					// 1:get, return index
					// 2:set, lParam : index
#define	inputLocal		0x09	// HIWORD(wParam) : idc
					// lParam : kind
#define	inputDispDnum		0x0B	// lParam : dispS\tdnumS
					// HIWORD(wParam) : type
					// 1:get, 2:set

// GEV_SISE
#define	siseSetData		0x01	// HIWORD(wParam):type, 
					// 0: lParam : set sise data
					// 1: get Curr data, return
#define	siseAlert		0x02	// lParam : alert data

// GEV_CTRL
#define	ctrlSend		0x01	// not
//#define	ctrlShowTick		0x02	// HIWORD(wParam) : view type
//					// FALSE:hide, 1:normal show, 2:cross tip
//					// lParam : data
#define	ctrlEnable		0x03	// HIWORD(wParam) : click event enable, TRUE, FALSE
#define	ctrlScrollStatus	0x04	// HIWORD(wParam) : status type
					// 0:disable, 1:enable, 2:update, 3:auto scroll check(return bAuto)	
					// lParam : init data "CNTt\tPOS\tEND\tDAY\tSKIP\t"
#define	ctrlScrollPos		0x05	// HIWORD(wParam) : nSBCode, lParam : pos
#define	ctrlChangeCombo		0x06	// LOWORD(lParam) : kind, HIWORD(lParam) : type
#define	ctrlGetType		0x07	// return type
#define	ctrlLocal		0x08	// HIWORD(wParam) : idcNum, lParam : data

// GEV_TOOL
#define	toolStatus		0x01	

// GEV_LIST
#define	listView		0x01	// HIWORD(wParam) : show(TRUE), hide(FALSE)
					// lParam : CRect
#define	listHistory		0x02	// HIWORD(wParam) : 1:history, 2:upcode
					// lParam : init histroy data

// graph info save
const	int MAXDUP	= 4;
const	int MAXFGRAPH	= 20;

struct	_dupInfo	{
	unsigned char	key;			// graph key
	unsigned char	type;			// graph type
	unsigned char	kind;			// graph kind
	WORD		value[maxAVLINE];	// information value, �̵������/���ذ�
	WORD		ichkcolor;		// icolor checkbit
	DWORD		icolor[maxAVLINE];	// color value, �̵�����ϼ�, Ư����Ʈ ����
};
#define	sz_DUPINFO	sizeof(struct _dupInfo)

struct	_dupGraph	{
	struct	_dupInfo	dupInfo[maxGRAPH][MAXDUP];
};
#define	sz_DUPGRAPH	sizeof(struct _dupGraph)

struct	_dupSet	{
	struct	_dupInfo	dupInfo[MAXDUP];
};
#define	sz_DUPSET	sizeof(struct _dupSet)

struct	_addGrp	{
	unsigned char	vcurr;
	unsigned char	lmargin;
	unsigned char	rmargin;
	WORD		addchk;			// icolor checkbit
	DWORD		addcolor[maxAVLINE];	// color value, �̵�����ϼ�, Ư����Ʈ ����
	WORD		addvalue[maxAVLINE];	// information value, �̵������/���ذ�
	WORD		volchk;			// �ŷ��� icolor checkbit
	DWORD		volcolor[maxAVLINE];	// �ŷ��� color value, �̵�����ϼ�, Ư����Ʈ ����
	WORD		volvalue[maxAVLINE];	// �ŷ��� information value, �̵������/���ذ�
};
#define	sz_ADDGRP	sizeof(struct _addGrp)

struct	_dllgraph	{
	char	objects[16];	// object name
	int	width;
	int	height;
	int	days;		// display day (field length)
	DWORD	tRGB;		// text RGB 
	DWORD	pRGB;		// paint RGB
	char	fName[10];	// font name
	int	fPoint;		// font point
	int	fStyle;		// font style
	char	title[50];	// title
	struct	_fgraph	fgraph[MAXFGRAPH];
	struct	_addGrp	addGrp[MAXFGRAPH];
	int	select;
	struct	_dupGraph dupGraph[MAXFGRAPH];
	char	reserved[49];	
};
#define	sz_DLLGRAPH	sizeof(struct _dllgraph)


#define	GA_ICOLOR1		0x0001		// addcolor[0]
#define	GA_ICOLOR2		0x0002		// addcolor[1]
#define	GA_ICOLOR3		0x0004		// addcolor[2]
#define	GA_ICOLOR4		0x0008		// addcolor[3]
#define	GA_ICOLOR5		0x0010		// addcolor[4]

// graphinfo.ini
// MAPSTATUS
#define	GRAPHINFO	"graphinfo.ini"

#define	GRXVTCODE	"GRX Controls"
#define	UNIT		"unit"
#define	INDEX		"index"
#define	MAPNAME		"mapname"
#define	MAPSTATUS	"status"
#define	MAPAREA		"maparea"
#define	TOOLVIEW	"toolview"
#define	GUBN		"gubn"
#define	TIME		"time"
#define	CURR		"curr"
#define	LMARGIN		"lmargin"
#define	RMARGIN		"rmargin"

const int initLMargin = 5;
const int initRMargin = 5;

#define	stInput		0x01
#define	stSise		0x02
#define	stCtrl		0x04

#define	GI_TICK		30		// tick chart(GI_HMS & gap == 0)
#define	GU_FTOTAL	50		// ����/�ɼ�/����
#define	GU_STOTAL	51		// �ֽ�/�ֽĿɼ�
#define	GU_FUOPTION	52
#define	GU_ATOTAL	53		// ����/�ɼ�, ����, �ֽĿɼ�

// history symbol
#define	SYMTOTAL	"CODX"		// TOTAL
#define	SYMCODE		"1301"		// ����
#define	SYMINDEX	"20301"		// ����
#define	SYMFUTURE	"30301"		// ����
#define	SYMOPTION	"40301"		// �ɼ�
#define	SYMCOOPT	"70301"		// �ֽĿɼ�


#endif
