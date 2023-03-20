#pragma once

#pragma pack(1)
/*
 .ghe : chart environment
	  version����Ǹ� .env, .grp, .dat ����
 .ghm : chart information(regcnt * _region + grpcnt * _graph)
 .ghd : _graph���� ���� file + �߰����� ����
 .ghi : ��Ÿ �׷��� ȯ�� ��������
*/

#define	rgbCOLOR	0x02000000	// RGB CheckBit
#define	panelCNT	10
#define	inputCNT	10
#define	ctrlCNT		20
#define	toolCNT		30
#define	maxLINE		10
#define	maxULINE	5
#define	maxMGRP		20
#define	maxAGRP		50
#define	maxUGRP		60
#define	maxNET		30
#define	maxINDEX	5
#define	sz_SYMBOL	3

enum {tkNO = -1, tkBOTTOM = 0, tkLEFT = 1, tkRIGHT = 2};

struct _font	{			// font info
	char		fname[32]{};	// font name
	BYTE		fstyle{};		// font style
	BYTE		fweight{};	// font weight
	BYTE		fpoint{};		// point font size
};
#define	sz_FONT		sizeof(struct _font)

struct _line	{			// line info
	DWORD		lcolor{};		// color 
	BYTE		lstyle{};		// line style :  
					// PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT
	BYTE		lwidth{};		// width
};
#define	sz_LINE		sizeof(struct _line)

struct _tick	{			// tick info
	DWORD		tcolor{};		// tick color
	struct	_font	tfont;		// tick font
	WORD		tarea{};		// tick area - ����ũ���� ���/100���� (2�� => 20000)
};
#define	sz_TICK		sizeof(struct _tick)

struct _dayinfo	{			// ���� ����
	DWORD		totcnt{};		// total count 
	DWORD		discnt{};		// display count
};
#define	sz_DAYINFO		sizeof(struct _dayinfo)

struct	_panel	{			// panel ����
	BYTE		pnID{};		// panel ID
	WORD		pnInfo{};
};
#define	sz_PANEL	sizeof(struct _panel)

//
//	_panel.pnID specify one of the following values
//
#define	pnINPUT		1
#define	pnSISE		2
#define	pnCTRL		3
#define	pnTOOL		4
#define	pnCFG		5
#define	pnTABLE		6
#define	pnINFO		7	// fixed data grid
#define pnCHART		8

//
//	_panel.pnInfo specify one of the following values
//
#define	PN_USE	0x01
#define	PN_SHOW	0x02

struct	_display	{		// chart display information
	DWORD		ebcolor{};	// ��ü ������
	DWORD		efcolor{};	// ��ü �׵θ���
	DWORD		gbcolor{};	// ��Ʈ ������
	DWORD		gfcolor{};	// ��Ʈ �׵θ���
	WORD		lmargin{};	// left vacant area(pixel)
	WORD		rmargin{};	// right vacant area(pixel)
	DWORD		option{};		// chart option

	struct _font	font{};		// total font
	struct _line	tline{};		// tool line config
	struct _font	tfont{};		// tool font config

	struct _line	hline{};		// horizon line
	struct _line	vline{};		// vertical line
	struct _line	cline{};		// cross line

	struct _tick	tick[3]{};	// x-tick info, bottom/left/right

	char		reserve[16]{};	// reserved
};
#define	sz_DISPLAY	sizeof(struct _display)

//
//	_display.option can be any combination of the following values.
//
#define	DS_POPUP	0x00000001	// popup menu
#define	DS_NSPLIT	0x00000002	// not use split
#define	DS_VIEWGRID	0x00000004	// use grid mode
#define	DS_NOCROSS	0x00000008	// no use Cross Line

#define	DS_MOD		0x00000010	// �����ְ�
#define	DS_LOCK		0x00000020	// ������ ǥ��
#define	DS_CURRENT	0x00000040	// ���簡 ǥ��
#define	DS_DAYCNG	0x00000080	// ��¥ ���漱 ǥ��

#define	DS_DISPLT	0x00000100	// display left tick
#define	DS_DISPRT	0x00000200	// display right tick
#define	DS_TKVGRID	0x00000400	// draw vertical tick line
#define	DS_TKHGRID	0x00000800	// draw horizontal tick line

#define	DS_SCREEN	0x00001000	// apply min/max in screen, not:entire data
#define	DS_REVERSE	0x00002000	// reverse mode
#define	DS_LOG		0x00004000	// price log
#define	DS_MAGNET	0x00008000	// price trade 

#define	DS_USEGRID	0x00010000	// use num grid
#define	DS_DISPALL	0x00020000	// display all(compress)
#define	DS_MINMAX	0x00040000	// display min/max
#define	DS_DOLLAR	0x00080000	// display as dollar

struct _datainfo	{		// chart data information
	BYTE		unit{};		// ����/����/�ɼ�..
	BYTE		index{};		// ��/��/��/��/ƽ
	WORD		mgap{};		// ��gap
	WORD		tgap{};		// ƽgap
					// xxx : xx�� ��Ʈ
					// 8xx : xxƽ ��Ʈ
					// 9xx : xx�� ��Ʈ
	WORD		mkind{};		// main graph kind
	WORD		update{};		// data option	
	struct _dayinfo count[4][maxINDEX]{};	// [����,����,����,�ɼ�] * [��,��,��,��,ƽ]
	char		reserve[16]{};	// reserved
};
#define	sz_DATAINFO	sizeof(struct _datainfo)

//
//	_datainfo.unit can specify one of the following values
//
#define	GRU_USER	0
#define	GRU_CODE	1		// ����
#define	GRU_INDEX	2		// ����
#define	GRU_FUTURE	3		// ����
#define	GRU_OPTION	4		// �ɼ�
#define	GRU_KQFUT	5		// KOFEX, kosdaq ����
#define	GRU_KQOPT	6		// kosdaq �ɼ�
#define	GRU_COOPT	7		// �ֽĿɼ�
#define	GRU_FOREIGN	8		// �ؿܽü�

//
//	_datainfo.index can specify one of the following values
//
#define	GI_DAY		1
#define	GI_WEEK		2
#define	GI_MONTH	3
#define	GI_MINUTE	4
#define	GI_TICK		5
//tour2k add
#define	GI_SEC		6
#define	GI_YEAR		7
//
//	_datainfo.update can be any combination of the following values.
//
#define	DU_RTM		0x0001		// realtime
#define	DU_DUMMY	0x0002		// dummy
#define	DU_FULLDISP	0x0004		// data full display

struct _grpinfo	{			// graph map info
	WORD		rgncnt{};		// region count 
	WORD		grpcnt{};		// graph count
};

struct	_envinfo	{
	WORD			version{};	// file version
	char			objname[16]{};
	WORD			evnopt{};		// environment option
	WORD			viewWidth{};	// view width
	WORD			viewHeight{};	// view height

	struct _panel		panel[panelCNT]{};
	struct _display		display{};
	struct _datainfo	datainfo{};
	
	struct _grpinfo		grpinfo[maxINDEX]{};	// ��,��,��,��,ƽ
	char			ulist[maxUGRP]{};	// ���ã��
};
#define	sz_ENVINFO	sizeof(struct _envinfo)

//
//	_envinfo.evnopt can be any combination of the following values.
//
#define	EO_MAXIMIZE	0x0001		// init maximize
#define	EO_SAVE		0x0002		// when window terminate, save

///////////////////////////////////////////////////////////////////////////////

struct	_region {
	BYTE		rkey{};			// region ID
	WORD		left{};			// left position ratio, x10000(10%->1000)
	WORD		top{};			// top position	ratio
	WORD		right{};			// right position ratio
	WORD		bottom{};			// bottom position ratio
};
#define	sz_REGION	sizeof(struct _region)

struct	_chartinfo	{			// chart info
	WORD		copt{};			// use check
	WORD		value[maxLINE]{};		// value
	struct _line	line[maxLINE]{};		// line info
};
#define	sz_CHARTINFO	sizeof(struct _chartinfo)

struct	_tickinfo	{			// chart info
	WORD		copt{};			// use check
	float		value[maxULINE]{};	// value
	struct _line	line[maxULINE]{};		// line info
};
#define	sz_TICKINFO	sizeof(struct _tickinfo)

struct	_graph {				// save format graph
	WORD		ikey{};			// unique key
	BYTE		dkey{};			// graph data key
	BYTE		rkey{};			// graph region ID
	
	BYTE		type{};			// graph type
	WORD		gkind{};			// graph kind
	WORD		skind{};			// ���� : 1~100, Dialog ó��, Indicator identification

	WORD		coption{};		// Calculate option		
	DWORD		doption{};		// Drawing option
	DWORD		rbcolor{};		// ravel color
	struct _chartinfo	cInfo{};		// chart info
	struct _tickinfo	tInfo{};		// user define info, horizon info
	char		reserve[16]{};		// reserved byte for future
};
#define	sz_GRAPH	sizeof(struct _graph)

//
//	_graph.coption can be any combination of the following values.
//

#define	GC_MAWEIGHT		0x0001		// �̵���� weighted
#define	GC_MAEXP		0x0002		// �̵���� exponential
#define	GC_MAOPEN		0x0010		// �̵���� �ð�
#define	GC_MAHIGH		0x0020		// �̵���� ��
#define	GC_MALOW		0x0040		// �̵���� ����
#define	GC_MAMID		0x0080		// �̵���� Mid(��+����/2)
#define	GC_MAREP		0x0100		// �̵���� ��ǥ�� - (��+����+����)/3
#define	GC_VOLPRC		0x0001		// �ŷ��� ���ݴ�� ���/�϶�
#define	GC_VOLVOL		0x0002		// �ŷ��� �ŷ���� ���/�϶�
#define	GC_VOLFPR		0x0004		// �ŷ��� �ð�/������� ���/�϶�


//
//	_graph.doption can be any combination of the following values.
//
#define	GO_USERLINE		0x00000001	// user defined line
#define	GO_FILLUPDN		0x00000002	// in userline filling up,down
#define	GO_TKLEFT		0x00000010	// draw left tick
#define	GO_TKRIGHT		0x00000020	// draw right tick
#define	GO_TKBOTTOM		0x00000040	// draw bottom tick
#define	GO_FIILUPBONG		0x00001000	// filling up bong
#define	GO_FIILDNBONG		0x00002000	// filling down bong
#define	GO_UNFILLVOLUME		0x00004000	// unfilling volume

//
//	in bong chart(GK_JPN,GK_BAR,GK_LIN,GK_POLE), _graph.cInfo can be used following discrition.
//
// _graph.cInfo.line[0].lcolor - linecolor
// _graph.cInfo.line[1].lcolor - upcolor
// _graph.cInfo.line[2].lcolor - dncolor


//
//	_graph.type can specify one of the following values
//
#define	GT_BONG			1		// bong chart
#define	GT_LINE			2		// line chart
#define	GT_VARS			3		// specific chart

//
//	_graph.gkind can specify one of the following values
//
//	_graph.type == GT_BONG
#define	GK_JPN			1		// �Ϻ��ĺ�
#define	GK_BAR			2		// �̱��ĺ�
#define	GK_LIN			3		// ����Ÿ�Ժ�
#define	GK_POLE			4		// Pole Grp
#define	GK_VOL			5		// �ŷ���
#define	GK_AMT			6

#define	GK_PMA			10		// price MA
#define	GK_VMA			11		// volume MA
#define	GK_AMA			12		// volume MA

#define	GK_BOLB			21		// BOLLINGER BAND
#define	GK_ENVL			22		// ENVELOPE
#define	GK_PARB			23		// PARABOLIC
#define	GK_NET			24		// net chart
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

//	graph.type == GT_LINE
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
#define	GK_VR			124		// VR
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


struct _cntInfo 
{
	WORD	mcnt{};		// main graph
	WORD	acnt{};		// aid graph
	WORD	ucnt{};		// user config aid graph
};
#define	sz_CNTINFO	sizeof(struct _cntInfo)

struct _saveinfo		// .ghs : ����� ���� file, saveinfo + envinfo + map + graphinfo
{
	struct _grpinfo	grpinfo[maxINDEX]{};	// ��,��,��,��,ƽ
	struct _cntInfo	cntinfo{};
};
#define	sz_SAVEINFO	sizeof(struct _saveinfo)

/*	ghd format
_cntInfo + _graph*(mcnt+acnt+ucnt)
*/

/////////////////////////////////////////////////////////
///////////////////// data up down //////////////////////
/////////////////////////////////////////////////////////

struct _input {
	char		jcode[16]{};	// �ڵ�
};
#define	sz_INPUT	sizeof(struct _input)

struct	_dataH {	// upload/download data header
	char		count[6]{};	// # of date(���ڼ�)
	char		dummy[6]{};	// dummy data number
	unsigned char	dkind{};		// data pattern kind	// not use
	unsigned char	dkey{};		// data key
	char		pday[8]{};	// ��������, yyyymmdd
	char		dunit{};		// data Index
	unsigned char	dindex{};		// ���ֿ�...
	char		lgap[4]{};	// n��, nƽ
	char		ltic[4]{};	// last tick count
	unsigned char	option1{};	// extra option
	unsigned char	option2{};	// extra option
	char		rcode[16]{};	// code symbol

	char		ikey[1]{};	// key action
					// '0':Enter or Other
					// '1':PgUp	'2':PgDn
					// '3':ScrollUp	'4':ScrollDn
					// '5':First or End
					// '6':Please sort
	char		xpos{};		// next gubn
	char		page[4]{};	// current page number
	char		save[80]{};	// grid inout save field
};
#define	sz_DATAH	sizeof(struct _dataH)

//
// dataH.option1 can be any combination of the following values
//
#define	GUI_MOD		0x21	// �����ְ�
#define	GUI_TODAY	0x22	// ���� ������(��,ƽ)

//
//	check can be any combination of the following values.
//
#define	CBitsKR			0x01		// �Ǹ���
#define	CBitsBR			0x02		// ����
#define	CBitsKS			0x04		// ����
#define	CBitsDI			0x08		// �׸����
#define	CBitsYP			0x10		// �׸麴��
#define	CBitsKJ			0x20		// ��������


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

#define	rgbCOLOR	0x02000000	// use in PALETTERGB()

#define	LINCR	RGB( 10,  10,  10)	// basic line color
#define	VOLCR	RGB( 84, 164,   5)	// basic volume color
#define	BUPCR	RGB(220,   2,   2)	// basic bong up color
#define	BDNCR	RGB(  0,   1, 200)	// basic bong dn color
#define	AVOLCR	RGB(235, 250, 133)	// basic avol color

#define	DEFCR1	RGB(  0, 140,   2)	// default color(��)
#define	DEFCR2	RGB(255, 144,   0)	// default color(����)
#define	DEFCR3	RGB(165, 111, 227)	// default color(����)
#define	DEFCR4	RGB(126,  26,  77)	// default color(��ȫ)
#define	DEFCR5	RGB(213,   0,   2)	// default color(��)
#define	DEFCR6	RGB(  0,  47, 199)	// default color(û)
#define	DEFCR7	RGB(180, 170,  21)	// default color(����)
#define	DEFCR8	RGB( 84, 164,   5)	// default color(����)
#define	DEFCR9	RGB( 77, 98, 126)	// default color(��û)
#define	DEFCR10	RGB(128, 128, 128)	// default color(��)
#define	DEFCR11	RGB( 21, 216, 152)	// default color()
#define	DEFCR12	RGB( 23, 134, 216)	// default color(û��)


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


#pragma pack()

