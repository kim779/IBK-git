#if !defined(AXIS_GRAPH_ENV_H)
#define AXIS_GRAPH_ENV_H

#pragma pack(1)
/*
 .ghe : chart environment
	  version����Ǹ� .env, .grp, .dat ����
 .ghm : chart information(regcnt * _region + grpcnt * _graph)
 .ghd : _graph���� ���� file + �߰����� ����
 .ghi : ��Ÿ �׷��� ȯ�� ��������
*/


//----------------------------------------------------------------------------------
//
//	ETC Define	 (��Ÿ ����)
//
//----------------------------------------------------------------------------------
//#define	rgbCOLOR	0x02000000	// RGB CheckBit
#define	PANEL_CNT	10
#define	INPUT_CNT	10
#define	CTRL_CNT	20
#define	TOOL_CNT	30
#define	MAX_LINE	12	// 2006.06.27 schbang : 10 -> 12 ����
#define	MAX_ULINE	5
#define	MAX_MGRP	20
#define	MAX_UGRP	60
#define	MAX_NET		30
#define	MAX_INDEX	5
#define	SZ_SYMBOL	3
#define	SAVE_TOTAL_NAME	"MG046000"

#define	GRP_OOP		"GOOPPOOP"	// OOP
#define	GRP_HOOP	"GOOPHOOP"	// Ȯ��  OOP

// 20090916 : CHANGE
//enum { foNone, foFuture, foStar, foOption, foCoFut, foCoOpt};
enum { foNone, foFuture, foStar, foOption, foCoFut, foCoOpt, foCmFut, foFmFut};

//----------------------------------------------------------------------------------
//
//	Tick Kind	 (Tick ����)
//
//----------------------------------------------------------------------------------
enum {TICK_NO = -1, TICK_BOTTOM = 0, TICK_LEFT = 1, TICK_RIGHT = 2};


//----------------------------------------------------------------------------------
//
//	font info	 (font ����)
//
//----------------------------------------------------------------------------------
struct _font	{
	char		acFName[32];	// font name
	BYTE		btFStyle;	// font style
	BYTE		btFWeight;	// font weight
	BYTE		btFPoint;		// point font size
};
#define	SZ_FONT		sizeof(struct _font)


//----------------------------------------------------------------------------------
//
//	line info	 (line ����)
//
//----------------------------------------------------------------------------------
struct _line	{			// line info
	COLORREF	rgbLColor;	// color 
	BYTE		btLStyle;	// line style :  
					// PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT
	BYTE		btLWidth;	// width
};
#define	SZ_LINE		sizeof(struct _line)


//----------------------------------------------------------------------------------
//
//	_toolInfo.dwOption, _toolInfo.dwOption2	 (_toolInfo�� �ɼ�)
//
//----------------------------------------------------------------------------------
#define	TO_START_EXT	0x00001		// ������ ����
#define	TO_END_EXT		0x00002		// ���� ����
#define	TO_SHOW_ANGLE	0x00004		// ���� ǥ��
#define	TO_SHOW_POSI	0x00008		// ��ġ ǥ��
#define	TO_FILL			0x00010		// ���� ä���


//----------------------------------------------------------------------------------
//
//	tool info	 (�۵��� ����)
//
//----------------------------------------------------------------------------------
struct _toolInfo	{
	COLORREF	rgbLine;	// color 
	BYTE		btLineStyle;	// line style :  
					// PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT
	BYTE		btLineWidth;	// width
	DWORD		dwOption;	// Option
	DWORD		dwOption2;	// Option2
};
#define	SZ_TOOLINFO	sizeof(struct _toolInfo)


//----------------------------------------------------------------------------------
//
//	tick info	 (ƽ ����)
//
//----------------------------------------------------------------------------------
struct _tick	{
	DWORD		dwTColor;	// tick color
	struct	_font	tfont;		// tick font
	WORD		wTArea;		// tick area - ����ũ���� ���/100���� (2�� => 20000)
};
#define	SZ_TICK		sizeof(struct _tick)


//----------------------------------------------------------------------------------
//
//	day info	 (���� ����)
//
//----------------------------------------------------------------------------------
struct _dayInfo	{			
	DWORD		dwTotCnt;	// total count 
	DWORD		dwDisCnt;	// display count
	char		cNoUsed;	// ������	cToday;		// ����üũ (��, ƽ��) 2006.06.30 �߰�
	WORD		wUnionGKind;
	char		acReserved[16];	// 2006.06.30 �߰�
};
#define	SZ_DAYINFO		sizeof(struct _dayInfo)


//----------------------------------------------------------------------------------
//
//	panel info	 (panel ����)
//
//----------------------------------------------------------------------------------
struct	_panel	{
	BYTE		btPnID;		// panel ID
	WORD		wPnOption;
};
#define	SZ_PANEL	sizeof(struct _panel)


//----------------------------------------------------------------------------------
//
//	_panel.btPnID	Panel ID (�г� ID)
//
//----------------------------------------------------------------------------------
#define	PN_INPUT	1
#define	PN_SISE		2
#define	PN_CTRL		3
#define	PN_TOOL		4
#define	PN_CFG		5
#define	PN_TABLE	6
//#define	PN_INFO		7	// fixed data grid
#define PN_AID		7	// ������ǥ����
#define PN_CHART	8




//----------------------------------------------------------------------------------
//
//	_panel.wPnOption	Panel Option (�г� �ɼ�)
//
//----------------------------------------------------------------------------------
#define	PO_USE		0x01
#define	PO_SHOW		0x02


//----------------------------------------------------------------------------------
//
//	chart display information (display ����)
//
//----------------------------------------------------------------------------------
struct	_display	{
	COLORREF		rgbBG;		// ��ü ������
	COLORREF		rgbEdge;	// ��ü �׵θ���
	COLORREF		rgbChartBG;	// ��Ʈ ������
	COLORREF		rgbChartEdge;	// ��Ʈ �׵θ���
	WORD			wLMargin;	// left vacant area(pixel)
	WORD			wRMargin;	// right vacant area(pixel)
	DWORD			dwDspOption;	// chart option

	struct _font		font;		// total font
	struct _toolInfo	toolinfo;	// tool line config
	struct _font		tfont;		// tool font config

	struct _line		hline;		// horizon line
	struct _line		vline;		// vertical line
	struct _line		cline;		// cross line
	struct _line		dline;		// ���ڱ��м�

	struct _tick		atick[3];	// x-tick info, bottom/left/right


	BYTE			btTopMargin;
	BYTE			btBottomMargin;
	char			acReserved[6];
	
};
#define	SZ_DISPLAY	sizeof(struct _display)


//----------------------------------------------------------------------------------
//
//	_display.dwDspOption	Display Option (���÷��� �ɼ�)
//
//----------------------------------------------------------------------------------
#define	DO_LBTNDNSHOW	0x00000002	// grid mode option : LBUTTON DOWN SHOW
#define	DO_VIEWGRID	0x00000004	// use grid mode
#define	DO_NOCROSS	0x00000008	// no use Cross Line

#define	DO_MOD		0x00000010	// �����ְ�
#define	DO_LOCK		0x00000020	// ������ ǥ��
#define	DO_CURRENT	0x00000040	// ���簡 ǥ��
#define	DO_DAYCNG	0x00000080	// ��¥ ���漱 ǥ��

#define	DO_DISPLT	0x00000100	// display left tick
#define	DO_DISPRT	0x00000200	// display right tick
#define	DO_TKVGRID	0x00000400	// draw vertical tick line
#define	DO_TKHGRID	0x00000800	// draw horizontal tick line

#define	DO_SCREEN	0x00001000	// apply min/max in screen, not:entire data
#define	DO_REVERSE	0x00002000	// reverse mode
#define	DO_LOG		0x00004000	// price log
#define	DO_MAGNET	0x00008000	// price trade 

#define	DO_USEGRID	0x00010000	// use num grid
#define	DO_DISPALL	0x00020000	// display all(compress)
#define	DO_MINMAX	0x00040000	// display min/max

#define	DO_PCURRENT	0x00100000	// ��������
#define	DO_ECURRENT	0x00200000	// ��/���Ѱ�
#define	DO_STARTMAX	0x00400000	// ���۽� �ִ�ȭâ����
#define	DO_NOVIEWTIP	0x00800000	// no tooltip


//----------------------------------------------------------------------------------
//
//	data info (������)
//
//----------------------------------------------------------------------------------
struct _dataInfo	{		// chart data information
	BYTE		btUnit;		// ����/����/�ɼ�..
	BYTE		btIndex;	// ��/��/��/��/ƽ
	WORD		wMGap;		// ��gap
	WORD		wTGap;		// ƽgap
					// xxx : xx�� ��Ʈ
					// 8xx : xxƽ ��Ʈ
					// 9xx : xx�� ��Ʈ
	WORD		wMainGrpKind;	// main graph kind
	WORD		wUpdate;	// determine update
	struct _dayInfo aaDayInfo[4][MAX_INDEX];	// [����,����,����,�ؿ�] * [��,��,��,��,ƽ]
	char		acIGap[8];	// ��/ƽ gap - 1~90
	char		acCode[12];	// �����ڵ�
	char		cToday;		// ����üũ (��, ƽ��) 2006.08.23 �߰�
	char		acReserved[63];	// reserved
};
#define	SZ_DATAINFO	sizeof(struct _dataInfo)


//----------------------------------------------------------------------------------
//
//	_dataInfo.btUnit	Data Unit (�ֽ�, ����, ����...)
//
//----------------------------------------------------------------------------------
#define	GU_USER		0
#define	GU_CODE		1		// ����
#define	GU_INDEX	2		// ����
#define	GU_FUTURE	3		// ����
#define	GU_OPTION	4		// �ɼ�
#define	GU_FOREIGN	5		// �ؿܽü�
#define	GU_COFUT	6		// �ֽļ���
#define	GU_COOPT	7		// �ֽĿɼ�
// ����) 0x09, 0x10�� ���� �÷������� �����ڷ� ����ϴ� ������� �� ��
#define GU_CFUTURE	8		// �ݸ�, ��ȭ ����(16xxxxxx, 17xxxxxx, ��ä,�������,��ȭ... xxxx.xx)
#define	GU_FFUTURE	11		// ��ǰ����(18xxxxxx, ��,����..xxxx)

//----------------------------------------------------------------------------------
//
//	_dataInfo.btIndex	Data Index (�Ϻ�, �ֺ�, �к�...)
//
//----------------------------------------------------------------------------------
#define	GI_DAY		1
#define	GI_WEEK		2
#define	GI_MONTH	3
#define	GI_MINUTE	4
#define	GI_TICK		5


//----------------------------------------------------------------------------------
//
//	_dataInfo.wUpdate	Screen Update (ȭ�� draw ����)
//
//----------------------------------------------------------------------------------
#define	DU_RTM		0x0001		// realtime
#define	DU_DUMMY	0x0002		// dummy
#define	DU_FULLDISP	0x0004		// data full display


//----------------------------------------------------------------------------------
//
//	graph map info
//
//----------------------------------------------------------------------------------
struct _grpinfo	{
	WORD		wRgnCnt;	// region count 
	WORD		wGrpCnt;	// graph count
};
#define	SZ_GRPINFO	sizeof(struct _grpinfo)

//----------------------------------------------------------------------------------
//
//	environment info (ȯ�漳��)
//
//----------------------------------------------------------------------------------
struct	_envInfo	{
	WORD			wVersion;	// file version
	char			acObjName[16];
	WORD			wEnvOption;	// environment option
	WORD			wViewWidth;	// view width
	WORD			wViewHeight;	// view height

	struct _panel		panel[PANEL_CNT];
	struct _display		display;
	struct _dataInfo	datainfo;
	
	struct _grpinfo		grpinfo[MAX_INDEX];	// ��,��,��,��,ƽ
	char			ulist[MAX_UGRP];	// ���ã��
	char			reserve[512];	// reserved
};
#define	SZ_ENVINFO	sizeof(struct _envInfo)


//----------------------------------------------------------------------------------
//
//	_envinfo.wEnvOption	Environment Option (ȯ�漳�� �ɼ�)
//
//----------------------------------------------------------------------------------
#define	EO_MAP		0x0001		// if map Component, set
#define	EO_SAVE		0x0002		// when window terminate, save
#define	EO_LINK		0x0004		// if set, display map link
#define	EO_JNAME	0x0008		// if set, display name. only map use
#define	EO_SINPUT	0x0010		// if set, minimize input
#define	EO_SAVEALL	0x0020		// if set, save all


//----------------------------------------------------------------------------------
//
//	region info (��������)
//
//----------------------------------------------------------------------------------
struct	_region {
	BYTE		btRKey;			// region ID
	WORD		wRLeft;			// left position ratio, x10000(10%->1000)
	WORD		wRTop;			// top position	ratio
	WORD		wRRight;		// right position ratio
	WORD		wRBottom;		// bottom position ratio
};
#define	SZ_REGION	sizeof(struct _region)


//----------------------------------------------------------------------------------
//
//	chart info
//
//----------------------------------------------------------------------------------
struct	_chartinfo	{
	WORD		wCOption;		// use check
	WORD		awValue[MAX_LINE];	// value
	struct _line	aline[MAX_LINE];	// line info
	char		acReserved[64];		// 2006.06.27 �߰�
};
#define	SZ_CHARTINFO	sizeof(struct _chartinfo)


//----------------------------------------------------------------------------------
//
//	tick info
//
//----------------------------------------------------------------------------------
struct	_tickinfo	{
	WORD		wCOption;		// use check
	float		afValue[MAX_ULINE];	// value
	COLORREF	rgbOverBuy;		// ���ż� ��������	// added by schbang 2006.03.23
	COLORREF	rgbOverSell;		// ���ŵ� ��������	// added by schbang 2006.03.23
	byte		abtFillOver[MAX_ULINE];	// ���ż��� ��뿩��	0: None, 1: buy, 2: sell // added by schbang 2006.03.23
	struct _line	aline[MAX_ULINE];	// line info	
};
#define	SZ_TICKINFO	sizeof(struct _tickinfo)


//----------------------------------------------------------------------------------
//
//	Y Axis Info
//
//----------------------------------------------------------------------------------
struct	_yAxisInfo	{
	char	cMode;
	int	iFit;
	double	dFix;
	char	acReserved[64];
};
#define	SZ_YAXISINFO	sizeof(struct _yAxisInfo)

// _yAxisInfo.cMode
#define Y_AUTO		0
#define Y_FIT		1
#define Y_FIX		2


//----------------------------------------------------------------------------------
//
//	graph info (�׷��� ����)
//
//----------------------------------------------------------------------------------
typedef struct	_graph {				// save format graph
	WORD		wKey;			// unique key
	BYTE		btDataKey;		// graph data key
	BYTE		btRgnKey;		// graph region ID
	
	BYTE		btGType;		// graph type
	WORD		wGKind;			// graph kind
	WORD		wSKind;			// ���� : 1~100, Dialog ó��, Indicator identification

	WORD		wCalOption;		// Calculate option		
	DWORD		dwDraOption;		// Drawing option
	COLORREF	rgbLabel;		// ravel color
	struct _chartinfo	cInfo;		// chart info
	struct _tickinfo	tInfo;		// user define info, horizon info
	struct _yAxisInfo	yainfo;		// Y Axis Info	// 2006.07.02 schbang

//	WORD		wUnionGKind;
	char		acReserve[64];		// reserved bytes for future
} GRAPH, *PGRAPH;
#define	SZ_GRAPH	sizeof(struct _graph)


//----------------------------------------------------------------------------------
//
//	_graph.wCalOption	Graph Calculation Option (�׷��� ���ɼ�)
//
//----------------------------------------------------------------------------------
#define	GC_MAWEIGHT		0x0001		// �̵���� weighted
#define	GC_MAEXP		0x0002		// �̵���� exponential
#define	GC_MAOPEN		0x0010		// �̵���� �ð�
#define	GC_MAHIGH		0x0020		// �̵���� ��
#define	GC_MALOW		0x0040		// �̵���� ����
#define	GC_MAMID		0x0080		// �̵���� Mid - (��+����)/2
#define	GC_MAREP		0x0100		// �̵���� ��ǥ�� - (��+����+����)/3
#define	GC_VOLPRC		0x0001		// �ŷ��� ���ݴ�� ���/�϶�
#define	GC_VOLVOL		0x0002		// �ŷ��� �ŷ���� ���/�϶�
#define	GC_VOLFPR		0x0004		// �ŷ��� �ð�/������� ���/�϶�


//----------------------------------------------------------------------------------
//
//	_graph.dwDraOption	Graph Drawing Option (�׷��� Drawing �ɼ�)
//
//----------------------------------------------------------------------------------
#define	GO_USERLINE		0x00000001	// user defined line
#define	GO_FILLUPDN		0x00000002	// in userline filling up,down
#define	GO_TICK_LEFT_DRAG	0x00000004	// �巡�� ��ǥ�ռ��� Left �׸��� ���� (added by schbang 2006.08.30)
#define	GO_TICK_RIGHT_DRAG	0x00000008	// �巡�� ��ǥ�ռ��� Right �׸��� ���� (added by schbang 2006.08.30)
#define	GO_TKLEFT		0x00000010	// draw left tick
#define	GO_TKRIGHT		0x00000020	// draw right tick
#define	GO_TKBOTTOM		0x00000040	// draw bottom tick
#define	GO_TICK_SKIP_HLINE	0x00000080	// �巡�� ��ǥ�ռ��� HLine �׸��� skip
#define	GO_FIILUPBONG		0x00001000	// filling up bong
#define	GO_FIILDNBONG		0x00002000	// filling down bong
#define	GO_UNFILLVOLUME		0x00004000	// unfilling volume
#define	GO_FILLOVER		0x00010000	// ���ż����ŵ� (added by schbang 2006.03.22)
#define	GO_UNION		0x00020000	// ��Ʈ�ռ� (added by schbang 2006.06.20)



//----------------------------------------------------------------------------------
//
//	_graph.btGType	Graph Type (��, ����, ��Ÿ)
//
//----------------------------------------------------------------------------------
#define	GT_BONG			1		// bong chart
#define	GT_LINE			2		// line chart
#define	GT_VARS			3		// specific chart


//----------------------------------------------------------------------------------
//
//	_graph.wGKind	Graph Kind (�׷��� ����)
//
//----------------------------------------------------------------------------------
//	_graph.type == GT_BONG
#define	GK_JPN			1		// �Ϻ��ĺ�
#define	GK_BAR			2		// �̱��ĺ�
#define	GK_LIN			3		// ����Ÿ�Ժ�
#define	GK_POLE			4		// Pole Grp
#define	GK_VOL			5		// �ŷ���
#define	GK_AMT			6		// �ŷ����

#define	GK_PMA			10		// price MA
#define	GK_VMA			11		// volume MA
#define	GK_AMA			12		// volume MA	(�ŷ���� ����)

#define	GK_BOLB			21		// BOLLINGER BAND
#define	GK_ENVL			22		// ENVELOPE
#define	GK_PARB			23		// PARABOLIC
#define	GK_NET			24		// net chart (�׹� ��Ʈ)
#define	GK_BALANCE		25		// �ϸ����ǥ
#define	GK_EQV			26		// Equi-volume chart
#define	GK_CDV			27		// Candle-volume chart
#define	GK_EQVV			28		// Equi-volume volume chart
#define	GK_CDVV			29		// Candle-volume volume chart
#define	GK_AVOL			30		// �����ŷ���(�Ź���Ʈ)
#define	GK_PIVOT		31		// Pivot Point
#define	GK_DEMARK		32		// Demark

//	graph.type == GT_VARS
#define	GK_PNF			51		// P&F ��Ʈ
#define	GK_SWING		52		// Swing ��Ʈ
#define	GK_THREE		53		// �Ｑ��ȯ��
#define	GK_CLOCK		54		// ���ð�
#define	GK_PV			55		// PV
#define	GK_KAGI			56		// Kagi
#define	GK_RENKO		57		// Renko
#define	GK_MKPRO		58		// market profile
#define	GK_GUKKI		59		// ������ ��Ʈ



//	graph.type == GT_LINE
#define GK_RSIWEIGHT	100		// 20090923 : ADD : RSI Weight ��ǥ �߰�. RSI with exponentional moving average
#define	GK_ADLINE		101		// A.D Line
#define	GK_ADX			102		// ADX
#define	GK_OBV			103		// OBV
#define	GK_CCI			104		// CCI
#define	GK_DMI			105		// DMI
#define	GK_GAP			106		// �̰ݵ�
#define	GK_LFI			107		// LFI
#define	GK_MACD			108		// MACD
#define	GK_MACDOS		109		// MACD OSCILLATOR
#define	GK_NCO			110		// NCO
#define	GK_NPSY			111		// �Žɸ���
#define	GK_PMAO			112		// ���� MAO
#define	GK_PSY			113		// �ɸ���(��ܽ�)
#define	GK_RRATE		114		// ȸ����(�ֽļ�)
#define	GK_RSI			115		// RSI
#define	GK_SIGMA		116		// SIGMA
#define	GK_SONA			117		// SONA
#define	GK_STOCHS		120		// STOCHASTIC SLOW
#define	GK_STOCHF		122		// STOCHASTIC FAST
#define	GK_VMAO			123		// �ŷ��� MAO
#define	GK_VR			124		// VR (Volume Ratio)
#define	GK_WPR			125		// WILLIAM %R
#define	GK_PROC			126		// Price Rate of Change
#define	GK_VROC			127		// Volume Rate of Charge
#define	GK_MOMENTUM		128		// ����� ��ǥ
#define	GK_ABRATIO		129		// AB Ratio
#define	GK_TRIX			130		// Trix
#define	GK_MFI			131		// Money Flow Index
#define	GK_ADR			132		// ADR/ADL
#define	GK_CO			133		// Chaikin's Oscillator
#define	GK_CV			134		// Chaikin's Volatility
#define	GK_EMV			135		// 

#define	GK_ATR			136		// ATR(Average True Range)
#define	GK_AND			137		// Accumulation/Distribution
#define	GK_MI			138		// Mass Index
#define	GK_NVI			139		// Negative Volume Index
#define	GK_OSCV			140		// Volume Oscillator
#define	GK_WC			141		// Weighted Close
#define	GK_WAD			142		// William A/D

/**remarking by macho@2006.12.12 <begin>
#define	GK_FEXRATE		143		// foreign exhaustion rate(������)
#define	GK_FOBRATE		144		// foreign obtain rate(������)
#define GK_ORGNVOL		149		// ��������ǥ ���(����)
#define	GK_FRGNVOL		145		// ��������ǥ �ܱ���(����)
//** 2005.11.08 add by macho
#define GK_ORGNAMT      150     // ��������ǥ ���(�ݾ�)
#define GK_FRGNAMT      151     // ��������ǥ �ܱ���(�ݾ�)
//**
#define	GK_MGJY			146		// �̰�������
remarking by macho@2006.12.12 <end> **/
#define	GK_BANDWIDTH		147		// Band Width
#define	GK_SONAMOMENTUM		148		// Sona Momentum

//** 2006.12.12 add by macho
#define	GK_FRGNHAVE		152		// �ܱ��κ�����Ȳ(����)
#define GK_FRGNRATE		153		// �ܱ��κ�������
#define GK_PSNLPBUY		154		// ���μ��ż���
#define	GK_FRGNPBUY		155		// �ܱ��μ��ż���
#define GK_ORGNPBUY		156		// ������ż���
#define GK_PSNLTBUY		157		// ���δ������ż���
#define GK_FRGNTBUY		158		// �ܱ��δ������ż���
#define GK_ORGNTBUY		159		// ����������ż���



//----------------------------------------------------------------------------------
//
//	Count Info 
//
//----------------------------------------------------------------------------------
struct _cntInfo 
{
	WORD	wMainCnt;	// main graph
	WORD	wAidCnt;	// aid graph
	WORD	wUserCnt;	// user config aid graph
};
#define	SZ_CNTINFO	sizeof(struct _cntInfo)


//----------------------------------------------------------------------------------
//
//	Save Info 
//
//----------------------------------------------------------------------------------
struct _saveinfo		// .ghs : ����� ���� file, saveinfo + envinfo + map + graphinfo
{
	struct _grpinfo	agrpinfo[MAX_INDEX];	// ��,��,��,��,ƽ
	struct _cntInfo	cntinfo;
};
#define	SZ_SAVEINFO	sizeof(struct _saveinfo)



/*	ghd format
_cntInfo + _graph*(mcnt+acnt+ucnt)
*/
/*
struct _input {
	char		jcode[16];	// �ڵ�
};
#define	SZ_INPUT	sizeof(struct _input)
*/


//----------------------------------------------------------------------------------
//
//	TR Structure	(tr ����ü)
//
//----------------------------------------------------------------------------------
struct	_dataH {	// must init by 0
	char		count[6];	// # of date(���ڼ�)
	char		dummy[6];	// dummy data number
	unsigned char	dkind;		// data pattern kind	// not use
	unsigned char	dkey;		// data key
	char		pday[8];	// ��������, yyyymmdd
	char		dunit;		// ���ֿ�...
	unsigned char	dindex;		// data Index
	char		lgap[4];	// n��, nƽ
	char		ltic[4];	// last tick count
	unsigned char	option1;	// extra option
	unsigned char	option2;	// extra option
	char		rcode[16];	// code symbol

	char		ikey[1];	// key action
					// '0':Enter or Other
					// '1':PgUp	'2':PgDn
					// '3':ScrollUp	'4':ScrollDn
					// '5':First or End
					// '6':Please sort
	char		xpos;		// next gubn
	char		page[4];	// current page number
	char		save[80];	// grid inout save field
};
#define	SZ_DATAH	sizeof(struct _dataH)

//----------------------------------------------------------------------------------
//
//	dataH.dkind
//
//----------------------------------------------------------------------------------
#define  DK_TODAY 0x01		// today only data select

//----------------------------------------------------------------------------------
//
//	First Header of Received Data
//
//----------------------------------------------------------------------------------
struct  _zcom_head {
	char ywin[4];
	char tcnt[4];
	char seqn[4];
	char fill[4];
};
#define SZ_ZCOMHEAD	sizeof(struct  _zcom_head)


//----------------------------------------------------------------------------------
//
//	dataH.option1
//
//----------------------------------------------------------------------------------
#define	GUI_MOD		0x21	// �����ְ�
#define GUI_TODAY	0x33


//----------------------------------------------------------------------------------
//
//	dataH.option2
//
//----------------------------------------------------------------------------------
#define	GUI_ZEROSKIP	0x21	// if volume is zero, skip


//----------------------------------------------------------------------------------
//
//	check
//
//----------------------------------------------------------------------------------

#define	CBITS_KR		0x01 		// �Ǹ���
#define	CBITS_BR		0x02		// ���� (������ �Ⱦ�)
#define	CBITS_KS		0x04		// ����
#define	CBITS_DI		0x08		// �׸����
#define	CBITS_YP		0x10		// �׸麴��
#define	CBITS_KJ		0x20		// ��������
/* old
#define	CBITSKR			0x01		// �Ǹ���
#define	CBITSBR			0x02		// ����
#define	CBITSKS			0x04		// ����
#define	CBITSDI			0x08		// �׸����
#define	CBITSYP			0x10		// �׸麴��
#define	CBITSKJ			0x20		// ��������
*/
/*
#define	CBITS_YS		0x01		// ��������
#define	CBITS_MS		0x02		// ��������
#define	CBITS_BR		0x04		// ���� (������ �Ⱦ�)
#define	CBITS_DI		0x08		// �׸����
#define	CBITS_YP		0x10		// �׸麴��
#define CBITS_HB		0x20		// ����պ�
#define	CBITS_KJ		0x40		// ��������
#define	CBITS_KR		0x0100 		// �Ǹ���
*/







/************* OOP data format **************************************************/
/*										*/
/*	request ==> [input argument] + [request output] + _dataH + [data]	*/
/*										*/
/*	[is+0x7F+data+'\t'+is+0x7F+data+'\t'..+'\t'+os+'\t'+os+'\t'+...os]	*/	
/*	'\t' + ? + SYMBOL + 0x7F + _dataH + 					*/
/*	[ods+'\n'+ods+'\n'+ods+'\n'+ods+'\n'+...]				*/
/*										*/
/*										*/
/*	receive ==> [request output] + _dataH + [data]				*/
/*										*/
/*	[data+'\t'+data+'\t'+data+'\t'..+data]					*/
/*	+ '\n' + _dataH +							*/
/*	[(data+'\t'+data+'\t'..+data) + '\n' + (data+'\t'+data+'\t'..+data)..]	*/
/********************************************************************************/

//#define	rgbCOLOR	0x02000000	// use in PALETTERGB()

#define	LINCR	RGB( 10,  10,  10)	// basic line color
#define	VOLCR	RGB( 84, 164,   5)	// basic volume color
#define	BUPCR	RGB(255,   0,   0)	// basic bong up color, ����
#define	BDNCR	RGB(  0,   0, 255)	// basic bong dn color, û��
#define	AVOLCR	RGB(196, 196, 196)	// basic avol color, �Ź���


#define	DEFCR1	RGB(  0, 140,   2)	// default color(��)
#define	DEFCR2	RGB(255, 144,   0)	// default color(����)
#define	DEFCR3	RGB(165, 111, 227)	// default color(����)
#define	DEFCR4	RGB(196,  3,   72)	// default color(����)
#define	DEFCR5	RGB(213,   0,   2)	// default color(��)
#define	DEFCR6	RGB(  0,  47, 199)	// default color(û)
#define	DEFCR7	RGB(180, 170,  21)	// default color(Ȳ��)
#define	DEFCR8	RGB(109, 186,   0)	// default color(���)
#define	DEFCR9	RGB( 77, 98, 126)	// default color(��û)
#define	DEFCR10	RGB(128, 128, 128)	// default color(ȸ��)
#define	DEFCR11	RGB( 21, 216, 152)	// default color()
#define	DEFCR12	RGB( 23, 134, 216)	// default color(�ϴû�)
#define	DEFCR13	RGB(254, 114, 114)	// default color(��ȫ��)
#define	DEFCR14	RGB(  0, 162, 238)	// new
#define	DEFCR15	RGB(249, 112,   1)	// new


//----------------------------------------------------------------------------------
//
//	structure for Aid chart basic info (������ǥ �⺻���� ����)
//
//----------------------------------------------------------------------------------
struct _basicInfo
{
	BYTE	btType;		//type;
	WORD	wGrpKind;	//gkind;
	WORD	wCOption;	//coption;
	DWORD	dwDOption;	//doption;
	struct _chartinfo	chartInfo;	//cInfo;
	struct _tickinfo	tickInfo;	//tInfo;
};

/*
//----------------------------------------------------------------------------------
//
//	structure for graph and Union Info (�׷��� ��������)
//
//----------------------------------------------------------------------------------
struct _indcConfig {
	WORD		wUnionKind;
	struct _graph*	pGrp;
};
*/

struct	_extTHx {
	char	key{};				// user-defined key
						// DLL : userTH.key
	int	size{};				// DATA size
	char* data;
};

#pragma pack()

#endif
