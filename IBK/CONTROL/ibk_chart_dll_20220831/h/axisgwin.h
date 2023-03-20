#if !defined(GRAPH_COMMON_H)
#define GRAPH_COMMON_H

#pragma pack(1)

#include "axisgenv.h"

#define	COMPANYNAME	"IBK����"
#define	K200_NAME	"KOSPI200"
#define	K200_TIP	"KOSPI200����"
#define	K200_INDEX	"11111111"
#define	K200_FUTURE	"99999999"
#define	STAR_NAME	"KOSTAR"
#define	STAR_TIP	"KOSTAR����"
#define	STAR_INDEX	"22222222"
#define	STAR_FUTURE	"88888888"

#define	LIB_DELETE(XXX) { delete XXX; XXX = NULL; }
#define	BUF_DELETE(XXX)	{ delete[] XXX; XXX = NULL; }
#define	IMG_DELETE(XXX) { XXX->DeleteImageList(); delete XXX; XXX = NULL; }
#define	WND_DELETE(XXX) { XXX->DestroyWindow(); delete XXX; XXX = NULL; }

// event and ctrlID
#define	GEV_VIEW	(WM_USER + 10)
#define	GEV_CHART	(WM_USER + 11)
#define	GEV_INPUT	(WM_USER + 12)
#define	GEV_SISE	(WM_USER + 13)
#define	GEV_CTRL	(WM_USER + 14)
#define	GEV_TOOL	(WM_USER + 15)
#define	GEV_CFG		(WM_USER + 16)
//#define	GEV_INFO	(WM_USER + 17)
#define	GEV_AID	(WM_USER + 17)
#define	GEV_TABLE	(WM_USER + 18)
#define	GEV_BASE	(WM_USER + 19)

// message form Main
#define UM_FROM_MAIN	(WM_USER + 507)

// image size
#define	szTimeImgX	17
#define	szTimeImgY	18
#define	szDateImgX	17
#define	szDateImgY	18
#define	szCodeImgX	18
#define	szCodeImgY	18
#define	szIEtcImgX	16
#define	szIEtcImgY	18
#define	szIOptImgX	35
#define	szIOptImgY	18
#define	szIBtnImgX	64
#define	szIBtnImgY	18
#define	szICfgImgX	20
#define	szICfgImgY	20
#define	szToolImgX	20
#define	szToolImgY	20
#define	szTool2ImgX	7
#define	szTool2ImgY	20
#define	szCtrlImgX	16
#define	szCtrlImgY	16
#define	szCtrl2ImgX	15
#define	szCtrl2ImgY	16

#define szToolCtrlImgX	20
#define szToolCtrlImgY	20

#define	szI4NameImgX	60
#define	szI4NameImgY	20
#define	szIDollarImgX	20
#define	szIDollarImgY	20
#define	szIForNameImgX	72
#define	szIForNameImgY	20

//#define	WIDTH_CFG	150
#define WIDTH_CFG	180
#define	WIDTH_TOOL	33
#define	WIDTH_AID	33
//#define	WIDTH_INFO	130
#define	HEIGHT_INPUT	(szDateImgY + 4 + 8)
#define	HEIGHT_CTRL	szCtrlImgY
#define	HEIGHT_SISE	40
#define	HEIGHT_TABLE	122


#define	VERSION_ENV	2
#define	VERSION_GRP	2
#define	VERSION_DAT	2
#define	VERSION_TOL	1
#define	VERSION_AID	1
#define	VERSION_USV	2

#define	VERSION_ENV_OLD	(VERSION_ENV - 1)
#define	VERSION_GRP_OLD	(VERSION_GRP - 1)
#define	VERSION_DAT_OLD	(VERSION_DAT - 1)
#define	VERSION_USV_OLD	(VERSION_USV - 1)

// New
#define ENV		".en0"
#define GRP		".gr0"
#define DAT		".da0"
#define USV		".us0"

#define TOL		".to0"
#define AID		".ai0"

// basic (��Ʈ�ѿ�)
#define ENV_B		".eb0"
#define GRP_B		".gb0"

#define	GRP_VERSION	14


////////////////////////////////////////////////////////////////////////
struct _interChart	{
	BYTE	bAll;
	BYTE	rgncnt;
	BYTE	grpcnt;
};
#define	sz_INTERCHART	sizeof(struct _interChart)

struct _barChart	{
	int	mkind;	// main kind
	BYTE	rgncnt;
	BYTE	grpcnt;
	char	*minfo;	// map graph info
	char	*ulist;	// user list
};
#define	sz_BARCHART	sizeof(struct _barChart)

struct _trData
{
	int	iLen[2];
	int	iSiseLen[2];
	char*	pcData[2];
};
#define	SZ_TRDATA	sizeof(_trData)


/////////////////////////////////////////////////////////////////////////
// GEV_VIEW
/////////////////////////////////////////////////////////////////////////
// LOWORD(wParam)
#define	viewInterface	0x01		// view interface
	#define	getRootPath	1		// get root path
						// char* return:path
	#define	getTabPath	2		// get tab path
						// char* return:tab path
	#define	getDatPath	3		// get dat file path
						// char* return:dat path
	#define	getUserName	4		// get user name
						// char* return:user name
	#define	getUserID	5		// get user ID
						// char* return:user id
	#define	requestTR	6		// send Request
	#define	requestTRS	7		// send request single tr
	#define	getDllName	8		


#define	viewDialog		0x02		// dialog call
					// return : type�� ���� �ٸ��� return
	#define	dlgSave		1		// ����� Ʋ load/save
	#define dlgEnv		2		// ��ü ȯ�� ����
						// BOOL return
	#define	dlgEnvchart	3		// ��ü ȯ�� ����
	#define	dlgMap		4		// ȭ�� ����(region + graph)
						// char* return
	#define	DlgIndc		5		// ������Ʈ ����
						// char* return
	#define	dlgSIndc	6		// Ʋ������Ʈ ����
						// return bool
	#define	dlgTool		7		// tool ����
						// return bool
	#define	dlgGwan		8		// ����������
	#define	dlgJCode	9		// ����˻�
						// char* return
	#define	dlgUCode	10		// �����˻�
						// char* return
	#define	dlgFCode	11		// �����˻�
						// char* return
	#define	dlgOCode	12		// �ɼǰ˻�
						// char* return
	#define	dlgJUCode	13		// ����/����
						// char* return
	#define	dlgFOCode	14		// ����/�ɼ�
						// char* return
	#define	dlgGubnFOSO	15		// ����/�ɼ�/�ֽĿɼ� üũ
						// int   return:
						// 1-����, 2-�ɼ�, 3-�ֽĿɼ�
	#define dlgToolCfg	16	// �۵�����

	#define DlgAidCfg	17	// ��ǥ��

	#define DlgYAxisCfg	18	// y �� ����

	#define DlgYShowCfg	19	// �巡�� �ռ��� y �� ���� �ɼ�

	#define	dlgToolEnv	20	// ������ư �˾��޴�
											// 20090910 : ADD
	#define	dlgCommoditiesFutureCode	21	// ��ǰ����/�ɼ�(Commodities Future/Option) �ڵ� �˻� ���̾�α� ȣ��
	#define	dlgCommoditiesOptionCode	22	// ��ǰ����/�ɼ�(Commodities Future/Option) �ڵ� �˻� ���̾�α� ȣ��
	#define	dlgcommodityName			23	// ��ǰ ����/�ɼ� // 20090910 : ADD
	
	

#define	viewMain		0x03		// main�� interface
	#define getPalette	1		// palette ����
	#define	getHistory	2
	#define	setHistory	3
	#define	setTrigger	4
	#define	openView	5
	#define	isSiseOnly	6

#define	viewChartInfo	0x04		// chart info
	#define	getMapInfo	1		// get map info
						// char* return:_interChart
	#define setMapInfo	2		// set map info
						// char* lParam:_interChart
	#define	getSGrpInfo	3		// get Single Graph Info
						// char* lParam:_graph
	#define	getDatInfo	4		// read dat file(��Ʈ �⺻������������)
						// char* return:_graphinfo
	#define	initGrpInfo	5		// init graph info
	#define	getMainGrpInfo 6  // return: boolean (if true then main graph is bong or line type other than vartype

#define	viewCode	0x05		// codelist,history, grouplist
	#define	hideList	1		// hide History list
	#define	showGroupTree	2		// Show/Hide Group Tree
						// bool lParam:show/hide
	#define	getNextCode	3		// Get Next Code
						// char* lParam;code
	#define	getPreCode	4		// Get Pre Code
						// char* lParam;code

#define	viewUtility	0x06		// etc function
	#define	viewImage	1
	#define	viewPrint	2
	#define	viewExcel	3
	
#define	viewNotify	0x07		// Notify (input, ctrl, chart)
	#define	dispDayChart	1		// pnChart message(input, ctrl), display change
	#define	dispDayInput	2		// pnInput message(ctrl, chart), display change
	#define	dispDayCtrl		3		// pnCtrl message(input, chart), display change
	#define	dispDayCtrl2	4		// pnCtrl message(input, chart), display change
	#define	dispScaleChart	5		// pnChart message(input, ctrl), dispscale change
	#define	dispScaleCtrl	6		// pnCtrl message(input, chart), dispscale change
	#define	mainChart		7		// pnChart message(input, ctrl, cfg), mainchart change
	#define	aidAddChart		8		// pnChart message(chart, cfg), add aidchart in chart
	#define	aidAddCfg		9		// pnConfig message(chart), add aidchart in cfg
	#define	aidDelCfg		10		// pnConfig message(chart), delete aidchart in cfg
	#define	aidDelChart		11		// pnChart message(cfg), delete aidchart in chart
	#define	unitInput		12		// pnInput message(sise, chart), inputBar unit change
	#define	indexInput		13		// pnInput message(chart), inputBar index change
	#define	inputAutotran	14		// pnInput message(cfg, ctrl, chart, tool), inputBar autotran
	#define	gridChart		15		// pnChart message(Main, cfg), grid show/hide
	#define	gridCtrl		16		// pnCtrl message(Main, chart), grid show/hide
	#define	tipChart		17		// pnChart message(Main, cfg), tooltip show/hide
	#define	tipCtrl			18		// pnCtrl message(Main, cfg), tooltip show/hide
	#define	tableChart		19		// pnChart message(Main, cfg), table show/hide
	#define	tableCtrl		20		// pnChart message(Main, chart), table show/hide	
	#define	cfgMainChart	21		// pnConfig message(input, ctrl, chart), mainchart change
	#define	clearInput		22		// clear Data
	#define	BookMarkCfg		23		// pnConfig message(chart), bookmark change
	#define	mctrlAutotran		24		// AxMCtrl message(input, ctrl, chart, tool), AxMCtrl autotran
	
	
#define	viewShowWindow		0x08		// Window Show Hide
	#define	showSise	1
	#define	showTool	2
	#define	showTable	3
	#define	showCtrl	4
	#define	showCfg		5
	#define	showAid		6

#define	viewInput			0x09			// control input information
	#define	getDisplayEdit		1	// get display edit's value 
	#define	setDisplayEdit		2	// get display edit's value 

#define	viewGroupList		0x0A		// select group list
					// kind  HIWORD(wParam), char* lParam	

// new added by kwon
#define	viewMulti	0x0B
	#define	codeUnit	0		// MTotal->MView, get data unit
	#define	codeInput	1		// pnInput message(gMView), inputBar code change
	#define	totalInput	2		// pnInput message(gMView), inputBar total change
	#define	total2Input	3		// pnInput message(gMView), inputBar multi total change
	#define	matrixInput	4		// pnInput message(gMView), inputBar new matrix
	#define	matrix2Input	5		// pnInput message(gMView), inputBar matrix change
	#define	selectInput	6		// pnInput message(gMView), inputBar select button
	#define	focusInput	7		// pnInput message(gMView), inputBar focus button 
	#define	orderInput	8		// pnInput message(gMView), inputBar order button 
	#define	todayInput	9		// pnInput message(gMView), inputBar today button 
	#define	activeView	10		// View Active(gMView, gMTotal), gMTotal<->gMView
	#define	activeInfo	11		// MView->MTotal, get Active Chart's Chart Info
	#define	inactiveView	12	// View inActive(gMView, gMTotal), gMTotal<->gMView
	#define	getCheckState	13	// MView->MTotal, get CheckBox state
	#define	setCheckState	14	// MView->MTotal, MTotal->MView, set CheckBox
	#define	giganInput		15	// MView->MTotal, gigan, gap change
	#define	mainInput	16		// Ctrl->MView->MTotal, get Main Chart Type
	#define	getRotCnt	17		// Bar->MView, get rotation chart count
	#define	getMultiInfo	18	// Input->MView, get MultiChartInfo
	#define	getMapCode	19		// MView->MTotal
	#define	savedCodeInput	20		// gMView->MTotal, init Saved code, no check unit 
	#define	mTotalTran		21		// gMView->MTotal, Request TR
	#define	onlyGigan		22		// gMView->MTotal, TR������ �ʰ� ����..�� ����
	#define setMultiCode	23		// pnInput message(gMView), automatic request by mutil code
									// lParam: CStringArray pointer
									// if array size if 0 then stop automatic request
	#define	autoTranUnit	24		// MView->MTotal, �ڵ� ��������� unit
	#define	autoTranCode	25		// MView->MTotal, �ڵ� ��������� code
	#define	autoComplete	26		// MTotal->MView, MView->CFG
	#define	initAllChart	27		// MView->MTotal, Initialize all chart
	#define	codeMultiInput	28		// pnInput message(gMView), ������Ƽ��Ʈ���� �����ڵ� ���̳�/Ʈ���ŷ� ����
/////////////////////////////////////////////////////////////////////////
// GEV_INPUT
/////////////////////////////////////////////////////////////////////////
#define	INPUT_CHANGE	0x01	// change value
	#define	ipUnit		1	// int   lParam:unit
	#define	ipKKind		2	// int   lParam:���� ����
	#define	ipSKind		3	// int   lParam:�ؿ� ����
	#define	IP_CODE		4	// char* lParam:code
	#define	ipCodeArrow	5	// int   lParam:VK_UP,VK_DOWN
	#define	ipGap		6	// int   lParam:gap
	#define	ipDate		7	//
	#define	IP_GIGAN	8	// int   lParam:gigan
	#define	ipDisplay	9	// int   lParam:display
	#define	ipRotate	10	// int   lParam:time
	#define	ipTab		11	// int   lParam:id
	#define	INPUT_BUTTON	12	// int   lParam:id
	#define	ipCEdit		13	// ctrl's tab
	#define ipUnitCode	14	// lParam: unit;code
	#define ipSetDisplay 15	// lParam: set display days, set text only
	#define	ipOutSide	16	// forcely input change from outside

#define	INPUT_DATA	0x02	// request data
	#define	GET_TR_INFO	1	// get TRsend Header info, char* return
	#define	getTRInfo2	2	// get TRsend Header info, char* return
	#define	GET_TOTAL_DAY	3	// get Totalday
	#define	getDispDay	4	// get DisplayDay
	#define	getCalendar	5	// get date
	#define	getCurGapList	6	// get Current Gaplist
					// char* lParam
	#define	setCodeInfo	7	// set info
					// char* lParam:code\tname 
	#define	domminoCode	8	// char* lParam:sym\tcode...
	#define	triggerCode	9	// char* lParam:sym\tcode
	#define	isDollarData	10	// int   return:bool
	#define	isTodayData	11	// int   return:bool
	#define	GET_VALID_CODE	12	// char* return:code
	#define	ONLY_SET_CODE	13
	#define	GET_TODAY	14
	#define SET_TOTAL_DAY	15
	#define GET_SERIES_KIND	16  // Getting Series Kind ==> GI_DAY, GI_WEEK, GI_MON, GI_MINUTE, GI_TICK
	#define	getCurCode		17		// MView->pnInput, get Current Code

#define	inputEnv	0x03	// argument control
	#define	inputMain	1	// change main chart, display enable/disable
					// int   lParam:kind
	#define	inputDisplay	2	// int   lParam:display day
	#define	inputReload	3	//
	#define	autotranCfg	4	// int   lParam:set/reset, cfg autotran
					// $$$$$
#define	inputViewFocus	0x04		// view focus
#define	INPUT_GET_POS_COMMENT	0x05	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	inputInitForeign	0x06	// Foreign Chart initiate
#define	inputChgPalette		0x07	// change Palette

// added by kwon
#define	InputMulti		0x08
	#define inputSiseUpdate	1	// pnInput message(gMCtrl), sise update
	#define	getCheckBox		2	// get Checkbox state
	#define	setCheckBox		3	// set Checkbox state
	#define	inputIndex		4	// index change
	#define	GetMergeCtrl	5	// Get Merge Ctrl
	#define	GetTodayCheck	6	// Get Today CheckBox State
	#define	SetTodayCheck	7	// Set Today CheckBox State
	#define	ChangeChart		8	// Change Chart

/////////////////////////////////////////////////////////////////////////
// GEV_CHART
/////////////////////////////////////////////////////////////////////////
#define	chartChgPalette		0x01	// change Palette

#define	CHART_SET_GRP		0x02	// set graph
	#define	CHANGE_ENV_INFO	1	// char* lParam:envinfo
	#define	CHANGE_MAIN_GRP	2	// int   lParam:main kind
	#define	changeMapInfo	3	// _interChart* lParam
	#define	ADD_AID_GRP	4	// char* lParam:indicator
	#define	delAidGrp	5	// char* lParam:indicator
	#define	chartReload	6	// char* lParam:graph info
	#define	chartIReload	7	// char* lParam:graph info
	#define	chartBookMark	8	// char* lParam:ulist
//	#define	CHANGE_MAIN_GRP_NO_REQUEST	9	// int   lParam:main kind

#define	CHART_TR_DATA		0x03	// Transaction data, len == 0, clear
					// int*  HIWORD(wParam):length, char* lParam:data
#define	CHART_DATA		0x04	// chart data
	#define	REQUEST_HEADER	1	// Request Graph Header, char* return
					// int   lParam:length
	#define	REQUEST_HEADER2	2	// Request Graph Header, char* return
					// int   lParam:length
	#define	requestHeader3	3	// Request Graph Header, char* return
					// int   lParam:length
	#define	REALTIME_DATA	4	// Realtime Data
					// char* lParam:
	#define	REQUEST_CUR_ENV	5	// _envinfo
	#define	REQUEST_CUR_MAP	6	// map info
	#define	excelData	7	// excel
	#define	getTableData	8	// char* return:table data
	#define	getPrintTitle	9	// char* return:title
	#define	getSaveName	10	// char* return:name
	#define	chartClear	11	// data clear
	#define	requestCurInter	12	// _interChart* return
	#define	requestHeaderS	13	// single Request Graph Header, char* return
					// int   lParam:length

#define	CHART_ENV		0x05	// change env 
	#define	chartIsValid	1	// bool  return: valid state
	#define	chartIndex	2	// int   LOWORD(lParam):index, HIWORD(lParam):gap 
	#define	chartUnit	3	// int   lParam:unit
	#define	chartTotal	4	// int   lParam:total
	#define	CHART_DISPLAY	5	// int   lParam:display
	#define	chartScale	6	// int   lParam:scale
					// int return:dispDay
	#define	chartTable	7	// bool  lParam: table state
	#define	chartGrid	8	// bool  lParam: grid state
	#define	chartTip	9	// bool  lParam: grid state

#define	chartCtrl		0x06
	#define	getScrollData	1	// get scroll data, return data len, lParam:data reference
	#define	changePosition	2	// change pos, LOWORD(lParam) : dispPos, HIWORD(lParam) : dispEnd
	#define	changeShift	3	// change shift, lParam:shift
	#define	chartAutoScroll	4	// autoscroll, lParam:TRUE(autoscroll),FALSE(not)
	#define	isAutoScroll	5	// is autoscroll?
	#define	chartToolSelect	6	// tool select, lParam:select
	#define	chartToolColor	7	// int	lParam:RGB
	#define	chartToolWidth	8	// int	lParam:�β�
	#define	chartExcel	9	// excel
	#define	chartAutotran	10	// auto transaction
	#define	chartUpdate	11	// bitmap bitblt
	#define	patternChanged	12	// Pattern tool status changed, LOWORD(lParam) = 0: disabled, other: enabled

#define	chartViewFocus		0x07	// view focus
#define	chartToolTip		0x08	// tooltip
					// HIWORD(wParam) : bool, lParam:char*
#define	CHART_MENU		0x09	// tooltip menu 
	#define	ADD_CHART_MENU	1	// add chart menu
	#define	selChartMenu	2	// select chart menu
					// LOWORD:menuID, HIWORD:aKind
#define	CHART_TR_DATAS		0x0A	// single Transaction data, len == 0, return
					// int*  HIWORD(wParam):length, char* lParam:data
// 2006.11.20 lsh (start)
#define	CHART_USER_SELECT	0x0B	// chart user selection(Map�� �����ϴ� �κ�)
	#define getSelectTime	1	// return (char*)datetime : "20061120:082000"
	#define	setSelectTime	2	// char* lparam: "20061120:082000"
	#define getSelectPrice	3	// int LOWORD(lParm):index, char* return price
	#define	StartOrderMode	4	// Start Order mode 
// 2006.11.20 lsh (end)

//** add by macho@20061226 <start>
#define CHART_STANLINE		0x0C	// stan-line information 
	#define	setStanLineDateTime	0x01	// set stan-line datetime value
	#define setStanLineWidth	0x02	// set stan-line line width
	#define setStanLineColor	0x03	// set stan-line line color
	#define stanLineDelete		0x04	// delete current stan-line
	#define getStanLineDateTime	0x05	// get stan-line datetime value
	#define getStanLineWidth	0x06	// get stan-line line width
	#define getStanLineColor	0x07	// get stan-line line color
//** add by macho@20061226 <end>

//** add by macho@20070102 <start>
#define CHART_ANAL			0x0D	// analize area selecting mode.
	#define	setAnalMode			0x01	// set analize area selecting mode: wParam = 0: not anal-mode, other: anal-mode
	#define getAnalMode			0x02	// get current anlalize area selecting mode: return value is 0 then not anal-mode, other then anal-mode
	#define	setMessageHandler	0x03	// set notify message handler window: lParam = CWnd* messageHandler
	#define setMessageID		0x04    // set notify message id: lParam = messageID
	#define	analAreaSelected	0x05	// user selected analize area: messageHandler->SendMessage(messageID, analAreaSelected, (LPANALAREA)lParam)
	#define analAreaDeleted		0x06	// user deleted analize area: messageHandler->SendMessage(messageID, analAreaDeleted)
	#define	initAnalMode		0x07	// initialize anal mode

typedef struct {
	struct {
		short	yy;
		BYTE	mm;
		BYTE	dd;
	} date;				// date of price
	struct {
		BYTE	hh;
		BYTE	mm;
		BYTE	ss;
	} time;				// time of price
	double open;		// open price
	double high;		// high price
	double low;			// low price
	double close;		// close price
	double volume;		// volume
} ANALPRICE;

typedef struct {
	char	code[12];	// current code
	int		count;		// count of days
	ANALPRICE data[1];  // pointer of ANALPRICE array
} ANALAREA, *LPANALAREA;

//** add by macho@20070102 <end>
/////////////////////////////////////////////////////////////////////////
// GEV_SISE
/////////////////////////////////////////////////////////////////////////
#define	siseChgPalette		0x01	// change Palette
#define	SISE_TR_DATA		0x02	// Transaction data, len == 0, clear
					// int*  HIWORD(wParam):length, char* lParam:data
#define	siseRTMData		0x03	// Realtime Data
					// char*  lParam:
#define	siseUnit		0x04	// change unit
					// int    lParam:unit
#define	siseData		0x05	// request data
	#define	siseCurr	1	// ���簡
					// char*  lParam
	#define	siseCode	2	// realtime code
					// char* return:code
	#define	siseName	3	// char* return:name
	#define	siseDate	4	// ��������
					// char* return:date
	#define	sisePivot	5	// ���ذ� 
					// char* return:����\t����\t��������
	#define	siseStock	6	// �ֽļ�
	#define	siseDollar	7	// convert into dollar data
	#define	siseClear	8	// clear sise
					// char* return:stock
#define	siseFlash		0x06	// HIWORD(WPARAM) == 1, blink otherwise not blink
					// flash : blink color palette index
#define	siseFont		0x07	// 
#define	siseChgEnv		0x08	// RGB    lParam:background color


/////////////////////////////////////////////////////////////////////////
// GEV_CTRL
/////////////////////////////////////////////////////////////////////////
#define	CTRL_SCROLL_POS		0x01	// scroll pos
					// HIWORD(wParam) : nSBCode, lParam : pos
	#define	ctrlWheelLeft	SB_THUMBTRACK + 10
	#define	ctrlWheelRight	SB_THUMBTRACK + 11
#define	CTRL_GET_POS_COMMENT	0x02	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	CTRL_ENV			0x03	// argument control
	#define	ctrlMain	1	// change main chart, display enable/disable
					// int   lParam:kind
	#define	ctrlDisplay	2	// int   lParam:display day
	#define	ctrlEnable	3	// bool  lParam:enable state
	#define	ctrlAutotran	4	// bool  lParam:set state
	#define	ctrlGrid	5	// bool  lParam:Grid state
	#define	ctrlTable	6	// bool  lParam:Table state
	#define	ctrlReload	7	//
	#define	ctrlUnit	8
	#define	ctrlIndex	9
	#define	ctrlClear	10	// clear ctrl
	#define	CTRL_SET_EDIT	11	// set display day
	#define	ctrlGetEdit	12	// get display day
	#define	ctrlChgDisp	13	// edit -> ctrl
	#define	ctrlETab	14
	#define	ctrlITab	15	// input's tab	
	#define	ctrlTip		16	// bool  lParam:tooltip state
	#define	CTRL_UPDATE_SLIDE	17

// new added by kwon
#define	CtrlMulti	0x04
	#define	CtrlInit	1

/////////////////////////////////////////////////////////////////////////
// GEV_TOOL
/////////////////////////////////////////////////////////////////////////
#define	TOOL_GET_POS_COMMENT	1	// LOWORD(lParam):x, HIWORD(lParam):y
					// char* return:comment
#define	TOOL_RELEASE		2	// release tool button
#define	TOOL_CHECK_RECT		3	// check lbuttondown is toolRect
					// int	return:id
#define	TOOL_GET_SELECT_TOOL	4	// chart -> select tool button
#define	TOOL_AUTOTRAN		5	// view -> autotran
#define	TOOL_CONFIG		6	// 
#define	TOOL_MAP_CLICK_EVENT	7	// send click event to map // 2006.11.20 lsh
#define TOOL_PATTERN			8		// pattern status get/set
	#define getPatternUPEnabled	0x01	// get pattern status, lResult= 0: disabled other: enabled
	#define setPatternUPEnabled	0x02	// set pattern status, lParam = 0: disbla	other: enable
	#define getPatternDNEnabled	0x03	// get pattern status, lResult= 0: disabled other: enabled
	#define setPatternDNEnabled	0x04	// set pattern status, lParam = 0: disbla	other: enable


//-----------------------------------------------------------------------
//
// GEV_AID
//
//-----------------------------------------------------------------------
#define	AID_GET_POS_COMMENT	1
#define	AID_CONFIG		2




/////////////////////////////////////////////////////////////////////////
// GEV_TABLE
/////////////////////////////////////////////////////////////////////////
#define	tbItem			1	// char* lParam:items
#define	tbUpdate		2	// char* lParam:update items
#define	tbShift			3	// char* lParam:shift items
#define	tbInsert		4	// char* lParam:insert column
#define	tbSelect		5	// int   lParam:select index
#define	tbReset			6	// reset table

/////////////////////////////////////////////////////////////////////////
// GEV_CONFIG
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Symbolic Interface
/////////////////////////////////////////////////////////////////////////

#define	IJ_CODE		"1301"	// input code
#define	II_CODE		"21301"	// input code
#define	IF_CODE		"31301"	// input code
#define	IO_CODE		"41301"	// input code
#define IS_CODE		"51301"	// input code, �ؿ�
#define	ICF_CODE	"61301"	// input code, �ֽļ���
#define	ICO_CODE	"71301" // input code, �ֽĿɼ�
#define	IPF_CODE	"101301" // input code, ��ǰ���� : 20090911 : ADD POOP
#define	IPO_CODE	"110301" // input code, ��ǰ�ɼ� : 20090911 : ADD POOP


/////////////////////////// �ü� ������

#define	OJ_CODE		"1021"	// original code, RTS code
#define	OJ_NAME		"1022"	// �ѱ۸�
#define	OJ_JGGA		"1023"	// ����(���簡)
#define	OJ_DIFF		"1024"	// ���ϴ��
#define	OJ_MDHG		"1025"	// �ŵ�ȣ��
#define	OJ_MSHG		"1026"	// �ż�ȣ��
#define	OJ_GVOL		"1027"	// �ŷ���
#define	OJ_SIGA		"1029"	// �ð�
#define	OJ_KOGA		"1030"	// ��
#define	OJ_JEGA		"1031"	// ����
#define	OJ_RATE		"1033"	// �����
#define	OJ_TIME		"1034"	// ü��ð�
#define	OJ_CDAY		"1310"	// ������(����������), yyyymmdd
#define	OJ_SAHG		"1311"	// ���Ѱ�
#define	OJ_HAHG		"1312"	// ���Ѱ�
#define	OJ_JJGA		"1313"	// ��������
#define	OJ_STCK		"1316"	// �����ֽļ�
#define	OJ_START	"1318"	// ����۽ð�
#define	OJ_GUBN		"1901"	// �Ҽӱ���(kospi,kosdaq,��3����)

#define	OI_CODE		"21021"	// original code, RTS code
#define	OI_NAME		"21022"	// �ѱ۸�
#define	OI_JGGA		"21023"	// ����(���簡)
#define	OI_DIFF		"21024"	// ���ϴ��
#define	OI_MDHG		"21025"	// �ŵ�ȣ��
#define	OI_MSHG		"21026"	// �ż�ȣ��
#define	OI_GVOL		"21027"	// �ŷ���
#define	OI_SIGA		"21029"	// �ð�
#define	OI_KOGA		"21030"	// ��
#define	OI_JEGA		"21031"	// ����
#define	OI_RATE		"21033"	// �����
#define	OI_TIME		"21034"	// ü��ð�
#define	OI_CDAY		"21310"	// ������(����������), yyyymmdd
#define	OI_SAHG		"21311"	// ���Ѱ�
#define	OI_HAHG		"21312"	// ���Ѱ�
#define	OI_JJGA		"21313"	// ��������
#define	OI_STCK		"21316"	// �����ֽļ�
#define	OI_START	"21318"	// ����۽ð�

#define	OF_CODE		"31021"	// original code, RTS code
#define	OF_NAME		"31022"	// �ѱ۸�
#define	OF_JGGA		"31023"	// ����(���簡)
#define	OF_DIFF		"31024"	// ���ϴ��
#define	OF_MDHG		"31025"	// �ŵ�ȣ��
#define	OF_MSHG		"31026"	// �ż�ȣ��
#define	OF_GVOL		"31027"	// �ŷ���
#define	OF_SIGA		"31029"	// �ð�
#define	OF_KOGA		"31030"	// ��
#define	OF_JEGA		"31031"	// ����
#define	OF_RATE		"31033"	// �����
#define	OF_TIME		"31034"	// ü��ð�
#define	OF_CDAY		"31310"	// ������(����������), yyyymmdd
#define	OF_SAHG		"31311"	// ���Ѱ�
#define	OF_HAHG		"31312"	// ���Ѱ�
#define	OF_JJGA		"31313"	// ��������
#define	OF_STCK		"31000"	// �����ֽļ�
#define	OF_START	"31318"	// ����۽ð�
#define	OF_MGJY		"31201"	// �̰��� ��������

#define	OO_CODE		"41021"	// original code, RTS code
#define	OO_NAME		"41022"	// �ѱ۸�
#define	OO_JGGA		"41023"	// ����(���簡)
#define	OO_DIFF		"41024"	// ���ϴ��
#define	OO_MDHG		"41025"	// �ŵ�ȣ��
#define	OO_MSHG		"41026"	// �ż�ȣ��
#define	OO_GVOL		"41027"	// �ŷ���
#define	OO_SIGA		"41029"	// �ð�
#define	OO_KOGA		"41030"	// ��
#define	OO_JEGA		"41031"	// ����
#define	OO_RATE		"41033"	// �����
#define	OO_TIME		"41034"	// ü��ð�
#define	OO_CDAY		"41310"	// ������(����������), yyyymmdd
#define	OO_SAHG		"41311"	// ���Ѱ�
#define	OO_HAHG		"41312"	// ���Ѱ�
#define	OO_JJGA		"41313"	// ��������
#define	OO_STCK		"41000"	// �����ֽļ�
#define	OO_START	"41318"	// ����۽ð�
#define	OO_MGJY		"41201"	// �̰��� ��������

#define	OS_CODE		"51021"	// original code, RTS code
#define	OS_NAME		"51022"	// �ѱ۸�
#define	OS_JGGA		"51023"	// ����(���簡)
#define	OS_DIFF		"51024"	// ���ϴ��
#define	OS_GVOL		"51027"	// �ŷ���
#define	OS_SIGA		"51029"	// �ð�
#define	OS_KOGA		"51030"	// ��
#define	OS_JEGA		"51031"	// ����
#define	OS_RATE		"51033"	// �����
#define	OS_TIME		"51034"	// ü��ð�
#define	OS_CDAY		"51310"	// ������(����������), yyyymmdd
#define	OS_JJGA		"51313"	// ��������
#define	OS_STCK		"51316"	// �����ֽļ�, ����
#define	OS_START	"51318"	// ����۽ð�
#define	OS_MDHG		"59999"	// �ŵ�ȣ��
#define	OS_MSHG		"59999"	// �ż�ȣ��
#define	OS_SAHG		"59999"	// ���Ѱ�
#define	OS_HAHG		"59999"	// ���Ѱ�

#define	OCF_CODE	"61021"	// original code, RTS code
#define	OCF_NAME	"61022"	// �ѱ۸�
#define	OCF_JGGA	"61023"	// ����(���簡)
#define	OCF_DIFF	"61024"	// ���ϴ��
#define	OCF_MDHG	"61025"	// �ŵ�ȣ��
#define	OCF_MSHG	"61026"	// �ż�ȣ��
#define	OCF_GVOL	"61027"	// �ŷ���
#define	OCF_SIGA	"61029"	// �ð�
#define	OCF_KOGA	"61030"	// ��
#define	OCF_JEGA	"61031"	// ����
#define	OCF_RATE	"61033"	// �����
#define	OCF_TIME	"61034"	// ü��ð�
#define	OCF_CDAY	"61310"	// ������(����������), yyyymmdd
#define	OCF_SAHG	"61311"	// ���Ѱ�
#define	OCF_HAHG	"61312"	// ���Ѱ�
#define	OCF_JJGA	"61313"	// ��������
#define	OCF_STCK	"61000"	// �����ֽļ�
#define	OCF_START	"61318"	// ����۽ð�
#define	OCF_MGJY	"61201"	// �̰��� ��������

#define	OCO_CODE	"71021"	// original code, RTS code
#define	OCO_NAME	"71022"	// �ѱ۸�
#define	OCO_JGGA	"71023"	// ����(���簡)
#define	OCO_DIFF	"71024"	// ���ϴ��
#define	OCO_MDHG	"71025"	// �ŵ�ȣ��
#define	OCO_MSHG	"71026"	// �ż�ȣ��
#define	OCO_GVOL	"71027"	// �ŷ���
#define	OCO_SIGA	"71029"	// �ð�
#define	OCO_KOGA	"71030"	// ��
#define	OCO_JEGA	"71031"	// ����
#define	OCO_RATE	"71033"	// �����
#define	OCO_TIME	"71034"	// ü��ð�
#define	OCO_CDAY	"71310"	// ������(����������), yyyymmdd
#define	OCO_SAHG	"71311"	// ���Ѱ�
#define	OCO_HAHG	"71312"	// ���Ѱ�
#define	OCO_JJGA	"71313"	// ��������
#define	OCO_STCK	"71000"	// �����ֽļ�
#define	OCO_START	"71318"	// ����۽ð�
#define	OCO_MGJY	"71201"	// �̰��� ��������

								// 20090911 : ADD : ��ǰ ���� �߰�
#define	PDF_CODE	"101021"	// original code, RTS code
#define	PDF_NAME	"101022"	// �ѱ۸�
#define	PDF_JGGA	"101023"	// ����(���簡)
#define	PDF_DIFF	"101024"	// ���ϴ��
#define	PDF_MDHG	"101025"	// �ŵ�ȣ��
#define	PDF_MSHG	"101026"	// �ż�ȣ��
#define	PDF_GVOL	"101027"	// �ŷ���
#define	PDF_SIGA	"101029"	// �ð�
#define	PDF_KOGA	"101030"	// ��
#define	PDF_JEGA	"101031"	// ����
#define	PDF_RATE	"101033"	// �����
#define	PDF_TIME	"101034"	// ü��ð�
#define	PDF_CDAY	"101310"	// ������(����������), yyyymmdd
#define	PDF_SAHG	"101311"	// ���Ѱ�
#define	PDF_HAHG	"101312"	// ���Ѱ�
#define	PDF_JJGA	"101313"	// ��������
#define	PDF_STCK	"101000"	// �����ֽļ�
#define	PDF_START	"101318"	// ����۽ð�
#define	PDF_MGJY	"101201"	// �̰��� ��������

								// 20090911 : ADD : ��ǰ �ɼ� �߰�
#define	PDO_CODE	"111021"	// original code, RTS code
#define	PDO_NAME	"111022"	// �ѱ۸�
#define	PDO_JGGA	"111023"	// ����(���簡)
#define	PDO_DIFF	"111024"	// ���ϴ��
#define	PDO_MDHG	"111025"	// �ŵ�ȣ��
#define	PDO_MSHG	"111026"	// �ż�ȣ��
#define	PDO_GVOL	"111027"	// �ŷ���
#define	PDO_SIGA	"111029"	// �ð�
#define	PDO_KOGA	"111030"	// ��
#define	PDO_JEGA	"111031"	// ����
#define	PDO_RATE	"111033"	// �����
#define	PDO_TIME	"111034"	// ü��ð�
#define	PDO_CDAY	"111310"	// ������(����������), yyyymmdd
#define	PDO_SAHG	"111311"	// ���Ѱ�
#define	PDO_HAHG	"111312"	// ���Ѱ�
#define	PDO_JJGA	"111313"	// ��������
#define	PDO_STCK	"111000"	// �����ֽļ�
#define	PDO_START	"111318"	// ����۽ð�
#define	PDO_MGJY	"111201"	// �̰��� ��������


// ������ �κ�
///////////////////////////���� �ü�

#define	DJT_SYM		"2500"
#define	DJM_SYM		"4500"
#define	DJD_SYM		"5500"

#define	DJT_DAY		"2302"	// ����
#define	DJT_HMS		"2034"	// �ð�
#define	DJT_SIGA	"2029"	// �ð�
#define	DJT_KOGA	"2030"	// ��
#define	DJT_JEGA	"2031"	// ����
#define	DJT_JGGA	"2023"	// ����
#define	DJT_GVOL	"2027"	// �ŷ���
#define	DJT_CVOL	"2032"	// ü�ᷮ
#define	DJT_GAMT	"2028"	// �ŷ����

#define	DJM_DAY		"4302"
#define	DJM_HMS		"4034"
#define	DJM_SIGA	"4029"
#define	DJM_KOGA	"4030"
#define	DJM_JEGA	"4031"
#define	DJM_JGGA	"4023"
#define	DJM_GVOL	"4027"
#define	DJM_CVOL	"4032"
#define	DJM_GAMT	"4028"

#define	DJD_DAY		"5302"
#define	DJD_HMS		"5034"
#define	DJD_SIGA	"5029"
#define	DJD_KOGA	"5030"
#define	DJD_JEGA	"5031"
#define	DJD_JGGA	"5023"
#define	DJD_GVOL	"5027"
#define	DJD_CVOL	"5032"
#define	DJD_GAMT	"5028"
#define	DJD_LOCK	"5311"	// �Ǹ������� ���� �÷���
#define	DJD_RATE	"5310"	// ���� ���� (�����ְ�)
#define	DJD_DOL		"5320"	// dollar

#define INV_FRGNHAVE	"5363"
#define INV_FRGNRATE	"5373"
#define INV_PSNLPBUY	"5408"
#define INV_FRGNPBUY	"5411"
#define INV_ORGNPBUY	"5412"
#define INV_PSNLTBUY	"5508"
#define INV_FRGNTBUY	"5511"
#define INV_ORGNTBUY	"5512"

#define IINV_PSNLPBUY	"25408"
#define IINV_FRGNPBUY	"25411"
#define IINV_ORGNPBUY	"25412"
#define IINV_PSNLTBUY	"25508"
#define IINV_FRGNTBUY	"25511"
#define IINV_ORGNTBUY	"25512"

///////////////////////////���� �ü�
#define	DIT_SYM		"22500"
#define	DIM_SYM		"24500"
#define	DID_SYM		"25500"

#define	DIT_DAY		"22302"	// ����
#define	DIT_HMS		"22034"	// �ð�
#define	DIT_SIGA	"22029"	// �ð�
#define	DIT_KOGA	"22030"	// ��
#define	DIT_JEGA	"22031"	// ����
#define	DIT_JGGA	"22023"	// ����
#define	DIT_GVOL	"22027"	// �ŷ���
#define	DIT_CVOL	"22032"	// ü�ᷮ
#define	DIT_GAMT	"22028"	// �ŷ����

#define	DIM_DAY		"24302"
#define	DIM_HMS		"24034"
#define	DIM_SIGA	"24029"
#define	DIM_KOGA	"24030"
#define	DIM_JEGA	"24031"
#define	DIM_JGGA	"24023"
#define	DIM_GVOL	"24027"
#define	DIM_CVOL	"24032"
#define	DIM_GAMT	"24028"

#define	DID_DAY		"25302"
#define	DID_HMS		"25034"
#define	DID_SIGA	"25029"
#define	DID_KOGA	"25030"
#define	DID_JEGA	"25031"
#define	DID_JGGA	"25023"
#define	DID_GVOL	"25027"
#define	DID_CVOL	"25032"
#define	DID_GAMT	"25028"
#define	DID_UCNT	"25256"	// ����+��������
#define	DID_DCNT	"25257"	// ����+�϶������

///////////////////////////���� �ü�
#define	DFT_SYM		"32500"
#define	DFM_SYM		"34500"
#define	DFD_SYM		"35500"

#define	DFT_DAY		"32302"	// ����
#define	DFT_HMS		"32034"	// �ð�
#define	DFT_SIGA	"32029"	// �ð�
#define	DFT_KOGA	"32030"	// ��
#define	DFT_JEGA	"32031"	// ����
#define	DFT_JGGA	"32023"	// ����
#define	DFT_GVOL	"32027"	// �ŷ���
#define	DFT_CVOL	"32032"	// ü�ᷮ
#define	DFT_GAMT	"32028"	// �ŷ����
#define	DFT_MGJY	"32201"	// �̰��� ����

#define	DFM_DAY		"34302"
#define	DFM_HMS		"34034"
#define	DFM_SIGA	"34029"
#define	DFM_KOGA	"34030"
#define	DFM_JEGA	"34031"
#define	DFM_JGGA	"34023"
#define	DFM_GVOL	"34027"
#define	DFM_CVOL	"34032"
#define	DFM_GAMT	"34028"
#define	DFM_MGJY	"34201"

#define	DFD_DAY		"35302"
#define	DFD_HMS		"35034"
#define	DFD_SIGA	"35029"
#define	DFD_KOGA	"35030"
#define	DFD_JEGA	"35031"
#define	DFD_JGGA	"35023"
#define	DFD_GVOL	"35027"
#define	DFD_CVOL	"35032"
#define	DFD_GAMT	"35028"
#define	DFD_MGJY	"35201"

///////////////////////////�ɼ� �ü�
#define	DOT_SYM		"42500"
#define	DOM_SYM		"44500"
#define	DOD_SYM		"45500"

#define	DOT_DAY		"42302"	// ����
#define	DOT_HMS		"42034"	// �ð�
#define	DOT_SIGA	"42029"	// �ð�
#define	DOT_KOGA	"42030"	// ��
#define	DOT_JEGA	"42031"	// ����
#define	DOT_JGGA	"42023"	// ����
#define	DOT_GVOL	"42027"	// �ŷ���
#define	DOT_CVOL	"42032"	// ü�ᷮ
#define	DOT_GAMT	"42028"	// �ŷ����
#define	DOT_MGJY	"42201"	// �̰��� ����

#define	DOM_DAY		"44302"
#define	DOM_HMS		"44034"
#define	DOM_SIGA	"44029"
#define	DOM_KOGA	"44030"
#define	DOM_JEGA	"44031"
#define	DOM_JGGA	"44023"
#define	DOM_GVOL	"44027"
#define	DOM_CVOL	"44032"
#define	DOM_GAMT	"44028"
#define	DOM_MGJY	"44201"

#define	DOD_DAY		"45302"
#define	DOD_HMS		"45034"
#define	DOD_SIGA	"45029"
#define	DOD_KOGA	"45030"
#define	DOD_JEGA	"45031"
#define	DOD_JGGA	"45023"
#define	DOD_GVOL	"45027"
#define	DOD_CVOL	"45032"
#define	DOD_GAMT	"45028"
#define	DOD_MGJY	"45201"

///////////////////////////�ؿ� �ü�

#define	DSD_SYM		"52500"

#define	DSD_DAY		"52302"	// ����
#define	DSD_SIGA	"52029"	// �ð�
#define	DSD_KOGA	"52030"	// ��
#define	DSD_JEGA	"52031"	// ����
#define	DSD_JGGA	"52023"	// ����
#define	DSD_GVOL	"52027"	// �ŷ���
#define	DSD_CVOL	"52032"	// ü�ᷮ

///////////////////////////�ֽļ��� �ü�
#define	DCFT_SYM	"62500"
#define	DCFM_SYM	"64500"
#define	DCFD_SYM	"65500"

#define	DCFT_DAY	"62302"	// ����
#define	DCFT_HMS	"62034"	// �ð�
#define	DCFT_SIGA	"62029"	// �ð�
#define	DCFT_KOGA	"62030"	// ��
#define	DCFT_JEGA	"62031"	// ����
#define	DCFT_JGGA	"62023"	// ����
#define	DCFT_GVOL	"62027"	// �ŷ���
#define	DCFT_CVOL	"62032"	// ü�ᷮ
#define	DCFT_GAMT	"62028"	// �ŷ����
#define	DCFT_MGJY	"62201"	// �̰��� ����

#define	DCFM_DAY	"64302"
#define	DCFM_HMS	"64034"
#define	DCFM_SIGA	"64029"
#define	DCFM_KOGA	"64030"
#define	DCFM_JEGA	"64031"
#define	DCFM_JGGA	"64023"
#define	DCFM_GVOL	"64027"
#define	DCFM_CVOL	"64032"
#define	DCFM_GAMT	"64028"
#define	DCFM_MGJY	"64201"

#define	DCFD_DAY	"65302"
#define	DCFD_HMS	"65034"
#define	DCFD_SIGA	"65029"
#define	DCFD_KOGA	"65030"
#define	DCFD_JEGA	"65031"
#define	DCFD_JGGA	"65023"
#define	DCFD_GVOL	"65027"
#define	DCFD_CVOL	"65032"
#define	DCFD_GAMT	"65028"
#define	DCFD_MGJY	"65201"

///////////////////////////�ֽĿɼ� �ü�
#define	DCOT_SYM	"72500"
#define	DCOM_SYM	"74500"
#define	DCOD_SYM	"75500"

#define	DCOT_DAY	"72302"	// ����
#define	DCOT_HMS	"72034"	// �ð�
#define	DCOT_SIGA	"72029"	// �ð�
#define	DCOT_KOGA	"72030"	// ��
#define	DCOT_JEGA	"72031"	// ����
#define	DCOT_JGGA	"72023"	// ����
#define	DCOT_GVOL	"72027"	// �ŷ���
#define	DCOT_CVOL	"72032"	// ü�ᷮ
#define	DCOT_GAMT	"72028"	// �ŷ����
#define	DCOT_MGJY	"72201"	// �̰��� ����

#define	DCOM_DAY	"74302"
#define	DCOM_HMS	"74034"
#define	DCOM_SIGA	"74029"
#define	DCOM_KOGA	"74030"
#define	DCOM_JEGA	"74031"
#define	DCOM_JGGA	"74023"
#define	DCOM_GVOL	"74027"
#define	DCOM_CVOL	"74032"
#define	DCOM_GAMT	"74028"
#define	DCOM_MGJY	"74201"

#define	DCOD_DAY	"75302"
#define	DCOD_HMS	"75034"
#define	DCOD_SIGA	"75029"
#define	DCOD_KOGA	"75030"
#define	DCOD_JEGA	"75031"
#define	DCOD_JGGA	"75023"
#define	DCOD_GVOL	"75027"
#define	DCOD_CVOL	"75032"
#define	DCOD_GAMT	"75028"
#define	DCOD_MGJY	"75201"

///////////////////////////��ǰ���� �ü� // 20090915 : ADD
#define	DGFT_SYM	"102500"
#define	DGFM_SYM	"104500"
#define	DGFD_SYM	"105500"

#define	DGFT_DAY	"102302"	// ����
#define	DGFT_HMS	"102034"	// �ð�
#define	DGFT_SIGA	"102029"	// �ð�
#define	DGFT_KOGA	"102030"	// ��
#define	DGFT_JEGA	"102031"	// ����
#define	DGFT_JGGA	"102023"	// ����
#define	DGFT_GVOL	"102027"	// �ŷ���
#define	DGFT_CVOL	"102032"	// ü�ᷮ
#define	DGFT_GAMT	"102028"	// �ŷ����
#define	DGFT_MGJY	"102201"	// �̰��� ����

#define	DGFM_DAY	"104302"
#define	DGFM_HMS	"104034"
#define	DGFM_SIGA	"104029"
#define	DGFM_KOGA	"104030"
#define	DGFM_JEGA	"104031"
#define	DGFM_JGGA	"104023"
#define	DGFM_GVOL	"104027"
#define	DGFM_CVOL	"104032"
#define	DGFM_GAMT	"104028"
#define	DGFM_MGJY	"104201"

#define	DGFD_DAY	"105302"
#define	DGFD_HMS	"105034"
#define	DGFD_SIGA	"105029"
#define	DGFD_KOGA	"105030"
#define	DGFD_JEGA	"105031"
#define	DGFD_JGGA	"105023"
#define	DGFD_GVOL	"105027"
#define	DGFD_CVOL	"105032"
#define	DGFD_GAMT	"105028"
#define	DGFD_MGJY	"105201"

///////////////////////////////////////////////////////////

// Realtime Data Symbol
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
#define	R_HTIM	"050"	// ȣ���ð�
#define	R_DHG1	"051"	// 1�� �ŵ�ȣ��
#define	R_DHG2	"052"	// 2�� �ŵ�ȣ��
#define	R_DHG3	"053"	// 3�� �ŵ�ȣ��
#define	R_DHG4	"054"	// 4�� �ŵ�ȣ��
#define	R_DHG5	"055"	// 5�� �ŵ�ȣ��
#define	R_DHG6	"056"	// 6�� �ŵ�ȣ��
#define	R_DHG7	"057"	// 7�� �ŵ�ȣ��
#define	R_DHG8	"058"	// 8�� �ŵ�ȣ��
#define	R_DHG9	"059"	// 9�� �ŵ�ȣ��
#define	R_DHGA	"060"	// A�� �ŵ�ȣ��
#define	R_SHG1	"071"	// 1�� �ż�ȣ��
#define	R_SHG2	"072"	// 2�� �ż�ȣ��
#define	R_SHG3	"073"	// 3�� �ż�ȣ��
#define	R_SHG4	"074"	// 4�� �ż�ȣ��
#define	R_SHG5	"075"	// 5�� �ż�ȣ��
#define	R_SHG6	"076"	// 6�� �ż�ȣ��
#define	R_SHG7	"077"	// 7�� �ż�ȣ��
#define	R_SHG8	"078"	// 8�� �ż�ȣ��
#define	R_SHG9	"079"	// 9�� �ż�ȣ��
#define	R_SHGA	"080"	// A�� �ż�ȣ��

#define	R_MGJY	"201"	// �̰�������(����)
#define	R_SAHA	"251"	// ���������
#define	R_SASE	"252"	// ��������
#define	R_HAHA	"254"	// ���������
#define	R_HARA	"255"	// �϶������

#pragma pack()

#endif
