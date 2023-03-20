// BlockBaseEnum.h: defines for Block Data
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKBASEENUM_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
#define AFX_BLOCKBASEENUM_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_

// (2004.10.12, ��¿�) BlockBaseData.h���� Enum Data�� ���� Define ��� ���� ������ Header�� �и��Ѵ�.
//		(DLL �� �ܺ� Module �����ڿ� �����ϱ� ����)
//		* ����. ���� �� �κ��� ������ �ܺ� Module ���� ����ڿ��� �˷� �ܺ� Module���� ����ǵ��� �Ѵ�.
//			���� ������ AddIn Header Folder�� �ξ� ������� ������ ����
//		*. ����, ������ ���� Ȯ���� �����ϳ� ������ ���� �ʵ��� �Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FLOAT.H"

static const int MARGIN = 4;
static const double ERRORVALUE = -DBL_MAX;

// VertScaleGroupList.h���� �̵� : ojtaso (20070314)
#define VSGL_TITLE_START_POSITION	( MARGIN * 3)

class CDrawingOption
{
public:
	enum DRAWINGORDER
	{
		LEFT = 0, 
		RIGHT
	};

	enum DRAWINGTYPE
	{
		ALL = 0,	// ��� �׸���
		BACKGROUND,	// ������ �׸���
		SCALE,		// ����, ���� ������ �׸���
		GRAPH		// Graph �׸���
	};
};

class CBlockBaseData
{
public:
	enum BLOCKTYPE
	{
		ROW,	// ������ ((��, ��)->(a, 1))
		COLUMN,	// ������ ((��, ��)->(1, b))
		ARRAY	// �ٵ��� ((��, ��)->(a, b))
	};
	
	enum BLOCKCOMPART
	{
		COMPARTLINE,// block���� ������ ���� (���� �缳�� ����)
		COMPARTGAP,	// block���� �� �������� ���� (���� �缳�� �Ұ���)
		BCNONE		// block���� ������ ���� (���� �缳�� �Ұ���)
	};

	enum BLOCKBEHAVIOR
	{
		BLOCK_DELETE = 0,
		BLOCK_INSERT,
		BLOCK_MOVE
	};

	enum TOOLTYPE
	{
		TOOL_NONE = -1,			// tool Ÿ���� ����
		TOOL_COMPARTLINE = 0,	// block ũ�� tool
		TOOL_BLOCKMOVE,			// block �̵� tool
		TOOL_BLOCKDELETE,		// block ���� tool
		TOOL_HORZSCALE,			// x�� scale tool
		TOOL_VERTSCALE,			// y�� scale tool
		TOOL_GRAPHTITLE,		// graph title tool (����, ����)
		TOOL_SUBGRAPH,			// sugGraph tool	(����, ����, ����)
		TOOL_GRAPHMOVE,			// graph title / subGraph tool �� �̵�
		TOOL_GRAPHDELETE,		// graph title / subGraph tool �� ����
		TOOL_BLOCKSIZE			// �� �ִ�ȭ �߰� - ojtaso (20080519)
	};
};

class CGraphBaseData
{
public:
	enum GRAPHTYPE
	{
		GTNONE = -1,
		Bong_Type = 0,		// ĵ����Ʈ, ����Ʈ(�ð�����), ����Ʈ, ���׳���Ʈ, ����Ʈ(������), FLOW��Ʈ
		Line_Type,			// �Ϲݽ�, ä���, ������, �����, ��ܽ�, 0���񿬼ӽ�, ����, ���κ����
		Bar_Type,			// ������, �����, ���Ͻ�, ������, ��Ʈ��
		Pie_Type,			// ����, ��ü��
		Special_Type,		// �Ｑ��ȯ��, ���ð�, P&F
		Fund_Type,			// �ݵ���
		DataView_Type,		// data ǥ��
		FuOp_Type,			// �����ɼ���
		MultiBar_Type,		// ��Ƽ���� //@Solomon:091231

		GraphTypeCount		// GRAPHTYPE�� �Ѱ���
	};

	// Graph Style
	enum BONGSTYLE
	{
		CandlestickBong = 0,	// ĵ����Ʈ
		OpenHighLowCloseBong,	// ����Ʈ(�ð�����)
		LineBong,				// ����Ʈ
		AntennaBong,			// ���׳���Ʈ
		NewHighLowCloseBong,	// ����Ʈ(������)
		FlowBong,				// FLOW��Ʈ
		CandleVolume,			// ĵ�麼����Ʈ
		EquiVolume				// ��ť������Ʈ
	};

	enum LINESTYLE
	{
		SimpleLine = 0,			// �Ϲݽ�
		FillLine,				// ä���
		CloudLine,				// ������
		HorizontalLine,			// �����
		StairLine,				// ��ܽ�
		NonZeroLine,			// 0���񿬼ӽ� (���� "0"�� ���� �׸��� ����)
		ComparisonLine,			// ����
		RainbowLine,			// ���κ����
		ZigZagLine,				// ������׽�
		LineCloudLine,			// ���� ������
		DataDotSimpleLine,		// ��ǥ���Ϲݽ�
		VerticalLine,			// ������
		NonCloudLine			// ��� ������
	};

	enum BARSTYLE
	{
		VerticalBar = 0,		// ������
		HorizontalBar,			// �����
		UpDownBar,				// ���Ͻ�
		CombBar,				// ������
		BoxBar,					// �ڽ���
		DotBar,					// ��Ʈ��
		LeftRightBar,			// �¿��
		LeftRightCombBar,		// �¿������
		GroupBar,				// �׷�ٽ�
		RatioBar,				// ������ (ex- �ż�/�ŵ� �ŷ��� ǥ����)
		TextHorizontalBar,		// Text�� �ִ� �����
		TextGroupBar,			// Text�� �ִ� �׷� �ٽ�
		UpDownHighLowBar,		// ���ϵ����
		FixedRateLeftRightBar,	// ���������¿��
		FixedRateCombBar,		// ��������������
		SellByTextHorizontalBar,// �ż��ŵ�Text�����
		FixedRateUpDownBar		// �����������Ͻ�d
	};

	enum PIESTYLE
	{
		FlatPie = 0,			// ����
		SolidPie				// ��ü��
	};

	enum SPECIALSTYLE
	{
		ThreeLineBreak = 0,		// �Ｑ��ȯ��
		InverseLine,			// ���ð�
		PAndF,					// P&F
		Kagi,					// Kagi
		Renko,					// Renko
// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�
		DisperSion				//�л�
// (2006.11.09 �Ƚ�ȯ) �л���Ʈ �߰�
	};

	enum FUNDSTYLE
	{
		ComponentRatio_Line = 0,// ������(����)
		ComponentRatio_Bar,		// ������(����)
		ComponentRatio_Bar_Line,// ������(����&����)
		ComponentRatio_Pie,		// ������(������)
		ReturnRiskDispersion,	// ����&����л�
		ReturnRiskGrade,		// ����&������
		StyleBox,				// ��Ÿ�Ϲڽ�
		FundMarketDispersion,	// �ݵ�&����л�
		Group_Bar				// �׷�(����)
	};

	enum DATAVIEWSTYLE
	{
		TopDataBar_Not_Used = 0,	// ��ܽ� ����	// (2006/10/11 - Seung-Won, Bae) Not Used Anymore.
		BottomDataBar_Not_Used,		// �ϴܽ� ����	// (2006/10/11 - Seung-Won, Bae) Not Used Anymore.
		PointSignal,				// point signal
		TopSignal,					// ��� signal
		BottomSignal,				// �ϴ� signal
		CompartText					// �����ؽ�Ʈ��
	};

	enum FUOPSTYLE
	{
		MarketProfile,			// MarketProfile
		FuOpPAndF				// FuOpP&F
	};

	//@Solomon:091231
	enum MULTI_BARSTYLE
	{
		MVerticalBar = 0,		// ������
	};

	// Graph Drawing Type
	// -> SPECIALSTYLE, FUNDSTYLE, OPTIONSTYLE �� Drawing Type �� ����!
	enum BONGDRAWSTYLE		// BONGSTYLE
	{
		Candlestick_Draw = 0,	// ĵ����Ʈ
		OpenHighLowClose_Draw,	// ����Ʈ(�ð�����)
		Line_Draw,				// ����Ʈ
		Antenna_Draw,			// ���׳���Ʈ
		NewHighLowClose_Draw,	// ����Ʈ(������)
		Flow_Draw,				// FLOW��Ʈ
		CandleVolume_Draw,			// ĵ�麼����Ʈ
		EquiVolume_Draw				// ��ť������Ʈ
	};

	enum LINEDRAWSTYLE		// LINESTYLE
	{
		Solid_Draw = 0,			// �Ǽ�
		Dot_Draw,				// ��ǥ����
		DottedLine_Draw,		// ���� ����
		DashDot_Draw,			// ������
		DashDotDot_Draw			// ��������
	};

	enum BARDRAWSTYLE		// BARSTYLE
	{
		Bar_Draw = 0,			// ����
		DotBar_Draw,			// ��ǥ����
		NonZero_DotBar_Draw,	// NonZero��ǥ����
		TextBar_Draw,			// Text��
		HorzLineBar_Draw		// ���򼱹���
	};

	enum PIEDRAWSTYLE		// PIESTYLE
	{
		FlatPie_Draw = 0,		// ����
		SolidPie_Draw			// ��ü��
	};

	enum DATABARDRAWSTYLE	// DATAVIEWSTYLE �� xxxBar
	{
		Char_Bar = 0,			// ���ڽ�
		Translucent_Bar,		// �������
		Hatchat45Degrees_Bar,	// 45�� �����
		VertHorz_Bar,			// ���������
		Fill_Bar				// ä���
	};

	enum DATASIGNALDRAWSTYLE// DATAVIEWSTYLE �� xxxSignal
	{
		Char_Signal = 0,		// ����
		CharArrow_Signal,		// ����+ȭ��ǥ
		Arrow_Signal,			// ȭ��ǥ
		Triangle_Signal,		// �ﰢ��
		Quadrangle_Signal,		// �簢��
		Circle_Signal			// ����
	};

	enum COMPARTTEXTDRAWSTYLE// DATAVIEWSTYLE �� CompartText
	{
		CompartLine = 0,		// ���м���
		Text,					// �ؽ�Ʈ��
		CompartLineText			// ���м��ؽ�Ʈ��
	};

	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	enum BARPAINTSTYLE			// BARSTYLE
	{
		None_Paint = 0,			// ���ؾ���
		Standard_Draw,			// ���ؼ�
		PrevValue_Draw,			// ������
		PrevClose_Draw,			// ��������
		PrevHigh_Draw,			// ������
		PrevAvg_Dra,			// ������հ�(������)
		PriceCandle_Draw		// ������Ʈ
	};

	// ��Ÿ
	enum ObjectChange
	{
		INSERT,
		MODIFY
	};

	enum SignalTYPE
	{
		NONE,
		BUY,
		SELL
	};

	/*enum DataDrawType
	{
		LOW_HIGH_UPDOWN,	// ������ ��� �ְ� �����
		LOW_NOW_UPDOWN,		// ������ ��� ���簡 �����
		HIGH_NOW_UPDOWN,	// �ְ� ��� ���簡 �����
		PRE_NOW_UPDOWN,		// ���ϰ� ��� ���簡 �����
		PRE_NOW_CONTRAST	// ���Ϻ�
	};*/
};

class CScaleBaseData
{
public:
	// ���� scale Draw type
	enum HORZSCALEDRAWERTYPE
	{
		HORZ_NOTYPE,	// ����
		HORZ_DAILY,		// �ϰ�
		HORZ_WEEKLEY,	// �ְ�
		HORZ_MONTHLY,	// ����
		HORZ_TIME_ONE,	// �ð�-����
		HORZ_TICK,		// ƽ

		HORZ_TIME_TWO,	// �ð�-����
		HORZ_TEXT,		// text
		HORZ_LEFTRIGHT,	// �¿��
		HORZ_PRICEZONE,	// ���ݴ�
		HORZ_DATA,		// data(�ŷ��� ����)
		HORZ_MARKETPROFILE, // MarketProfile
		HORZ_TIME_SECOND	// ��
	};

	// ���� scale Draw type
	enum VERTSCALEDRAWERTYPE
	{
		VERT_NOTYPE,		// ����
		VERT_PRICE,			// ����Ÿ��
		VERT_VOLUME,		// �ŷ���Ÿ��
		VERT_SALE,			// ���Ź�
		VERT_MARKETPROFILE, // MarketProfile
		VERT_BASELINE,		// ���ؼ�Ÿ��
		VERT_TEXT,			// text
		VERT_PACKETDATA,	// ���� packet Data
		VERT_LOG,			// log
		VERT_PERCENT,		// �����
		VERT_TOPBUTTOM_PLUS	// +���Ͻ�
	};

	// scale grid
	enum SCALEGRIDTYPE
	{
		SOLIDLINE,		 // �Ǽ�
		LONGDOTTEDLINE,  // ����(- - -)
		SHORTDOTTEDLINE, // ����(����������)
		SLNONE			 // ����
	};

	// scale ���
	enum SCALETYPE
	{
		HORZ_SCALE,
		VERT_SCALE
	};

	// ���� scale ��ġ
	enum HORZSCALEPOSITION
	{
		HORZ_TOP,
		HORZ_BOTTOM,
		HORZ_BOTH,
		HORZ_HIDE,
		HORZ_NONE
	};

	// ���� scale ��ġ
	enum VERTSCALEPOSITION
	{
		VERT_LEFT,
		VERT_RIGHT,
		VERT_BOTH,
		VERT_NONE
	};

	// Text Compart Type
	enum TEXTCOMPARTTYPE
	{
		HORZ_TEXT_COMPART,	// ����
		VERT_TEXT_COMPART,	// ����
		CENTER_HORZ_TEXT_COMPART // ����(Text�� �߾ӿ� �ֱ�)
	};

	// ���� scale unit type
	enum VERTSCALEUNITTYPE
	{
		VERT_NUM,		// ���� x1000
		VERT_NUM_CHAR	// ����+���� x1000, x��
	};

	// (2006/11/11 - Seung-Won, Bae) Define Y Scale Min/Max Setting Type
	enum VERTMINMAXTYPE
	{
		VERT_MINMAX_OF_VIEW,
		VERT_MINMAX_OF_FULL,
		VERT_MINMAX_OF_USER
	};
};

// ----------------------------------------------------------------------------
class CCalculateBaseData
{
public:
	enum CALCULATETYPE
	{
		TRDATA_CAL,
		REALDATA_CAL,
		SCROLL_CAL
	};
};

class CPacketType
{
public:
	// real type
	enum REALTYPE
	{
		FLOOR,		// ����. 09:01:00 ~ 09:01:59 -> 09:01
		CEILING,	// �ø�. 09:00:01 ~ 09:01:00 -> 09:01 (�޸���)
		CEILING2	// �ø�. 09:00:00 ~ 09:00:59 -> 09:01 (KOSCOM)
	};

	enum REALMODE
	{
		APPEND,	
		UPDATE
	};

	enum REQUESTNEWPACKETTYPE	// ���� ��û�ϴ� ��Ŷ�� Ÿ��
	{
		PRICE_ORIGINAL,
		PRICE_MODIFY,
		EX_PACKET,
		NEWS_PACKET,
		ACCOUNT_ITEMIZE_PACKET
	};

	enum PRICETYPE
	{
		ORIGINAL,	// �Ϲ��ְ�
		MODIFY		// �����ְ�
	};

	enum EX
	{	
		EX_NONE = -1,
		EX_ALL = 0,			// ��
		EX_DIVIDEND,	// ����
		EX_RIGHTS		// �Ǹ���		
	};

	enum COMMODITY_BOND_NAME
	{
		NONAME = -1,
		STOCK = 0,		// �ֽ�
		INDUSTRY,		// ����
		FUTURES,		// ����
		OPTION,			// �ɼ�
		ELW,			// ELW
		FOREIGNINDEX,	// �ؿ�����
		FOFOREIGN		// �ؿܼ���
	};

	enum ETC
	{
		NEWS,			// ����
		ACCOUNT_ITEMIZE // �ŷ�����
	};

	enum MARKETDAYTYPE
	{
		TODAY_ONLY		= 0,
		WITH_NEXT_DAY
	};
};

class CPacketDataInfo  
{
public:
	enum DATAINSERTTYPE
	{
		NONE = -1,
		NEW,
		INSERT,
		APPEND,
		REAL_APPEND,
		PART_NEW
	};
};

// ----------------------------------------------------------------------------
class CMenuType
{
public:
	// menu�� ����
	enum MENUTYPE
	{
		MENU_NONE = -1,			// menu �� ����.
		MENU_GRAPH_HIGHLIGHT,	// ������ �ִ� graph menu
		MENU_GRAPH_NO_HIGHLIGHT,// ������ ���� graph menu
		MENU_DATAVIEWTYPE_GRAPH,// graph type �� <DataView_Type>�� ����� graph menu
		MENU_HORZ_SCALE,		// ���� scale menu
		MENU_VERT_SCALE,		// ���� scale menu
		MENU_VERT_HORZ_SCALE,	// ����,���� scale menu
		MENU_ANALYSTOOL,		// �м� ���� menu
		MENU_SYMBOL				// symbol menu
	};

	enum ELEMENTMENU
	{
		MENU_DELETE,	// ����
		MENU_SET,		// ���� Dlg
		MENU_COLOR,		// color
		MENU_THICKNESS,	// thickness
		MENU_HIGHLIGHT,	// ����
		MENU_DRAWSTYLE,	// draw style
		MENU_DATAINDEX,	// data index�� ���ϱ�
		MENU_HORZSCALE_POSITION, // x�� scale ��ġ
		MENU_INVERTSCALE,	// ��ǥ ������ - ojtaso (20071023) 
		MENU_FIBO_SET,		// �Ǻ���ġ ������ ���� Dlg		// 2008.08.20 ������ �Ǻ���ġ Ȯ��
		MENU_TIME_DIFF,	// graph �ð��� // // 20081007 JS.Kim	����ڽð��� ����
		MENU_DELETE_ALL,	// ��ü����
	};
};


class CCursorShape
{
public:
	enum CHARTOBJECT
	{
		OBJECT_NONE,
		BLOCKPOS_SPLITH, // ����(��)
		BLOCKPOS_SPLITV, // ����(��)
		BLOCKPOS_SPLITA, // ����, ����(��,��)
		OBJECT_SELECT,	 // �����ϱ�
		OBJECT_MOVE,	 // �̵��ϱ�
		TOOL_DRAW,		 // ��ü�׸���
		TOOL_NODROP,	 // �̵� �ȵ�
		TOOL_SIZEALL,	 // ��ü�̵�
		TOOL_SIZENESW,   // ��üũ�⺯��
		TOOL_AUTOTREND,	 // �ڵ��߼��м�
		VIEW_ZOOM_IN,	 // Ȯ��
		WHOLE_VIEW,		 // ��ü����
		OBJECT_REMOVE,		// Remove
		OBJECT_INSERT,		// Insert Graph �и�/���� - ojtaso (20070731)
	};
};


class CToolOptionInfo
{
public:
	enum OPTION
	{
		INNER,
		OUTER,
		MENU
	};

	enum TOOLOPTION
	{
		T_INFLATE,						// Ȯ��,���,
		T_WHOLEVIEW,					// ��ü����
		T_NIDLG_DRAG,					// ��ġ��ȸâ, // (2006/6/28 - Seung-Won, Bae) Rename for Actual Function with Old Map Usage.
		T_DATALIST_REPORT,				// �����ͺ���,   
		T_INQUIRY_STATUS,				// ��ġ��ȸ�����ͺ���,
		T_OBJ_CFG,						// ��ü����,   
		T_OBJ_HIGHLIGHTt,				// ��ü����,
		T_GRAPH_STYLE,					// Graph����,   
		T_GRAPH_THICKNESS,				// ������,   
		T_GRAPH_COLOR,					// ������,   
		T_INDICATOR_CFG,				// ��ǥ����,   
		T_ENVIRONMENT_CFG,				// ȯ�漳��,
		T_INDICATOR_LIST,				// ��ǥ����Ʈ����,
		T_INDICATOR_SERIES,				// ��ǥ������ȸ, 
		T_PANORAMA,						// �ĳ��,     
		T_SCROLL,						// ��ũ�ѹ�,
		T_PREV,							// ����,
		T_NEXT,							// ����,
		T_RESTORE,						// �ʱ�ȭ,  		
		T_REMOVE_OBJ,					// ��ü����, 
		T_REMOVE_BLOCK,					// ������, 
		T_CHART_RAINBOW,				// ���κ���, 
		T_CHART_BALANCE,				// �ϸ����ǥ, 
		T_CHART_PANDF,					// P&F,   
		T_CHART_THREELINEBREAK,			// �Ｑ��ȯ��,
		T_CHART_REVERSE,				// ���ð�, 
		T_CHART_BARFORSALE,				// ���Ź���Ʈ, 
		T_SYMBOL_BULET,					// ��ȣDlg, 
		T_SYMBOL_SQUARE,				// �簢����
		T_SYMBOL_CIRCLE,				// ��������
		T_SYMBOL_TRIANGLE,				// �ﰢ��
		T_SYMBOL_TEXT,					// ����
		T_TOOL_SELECTION,				// ����
		T_TOOL_LINE_PROFIT,				// �߼������ͷ�
		T_TOOL_LINE,					// �߼���
		T_CROSSHAIRS_DRAG,				// Crosshairs with drag // (2006/6/28 - Seung-Won, Bae) Rename for Actual Function with Old Map Usage.
		T_TOOL_LINE_VERT,				// ������
		T_TOOL_LINE_HORZ,				// ����
		T_TOOL_LINE_TRISECT,			// ���м�
		T_TOOL_LINE_QUARTER,			// ���м�
		T_TOOL_LINE_RESISTANCE,			// �������׼�
		T_TOOL_FIBONACCI_ARC,			// �Ǻ���ġ �ݿ�
		T_TOOL_FIBO_CIRCLE,				// �Ǻ���ġ ��
		T_TOOL_FIBONACCI_FAN,			// �Ǻ���ġ �Ҷ���
		T_TOOL_FIBO_RETRACE,			// �Ǻ���ġ �ǵ���(������)
		T_TOOL_FIBONACCI_TIME,			// �Ǻ���ġ �ð���
		T_TOOL_GANN_LINE45,				// ������(���45��)
		T_TOOL_GANN_LINE315,			// ������(����45��)
		T_TOOL_GANN_FAN45,				// ����(���45��)
		T_TOOL_GANN_FAN135,				// ����(�»�45��)
		T_TOOL_GANN_FAN225,				// ����(����45��)
		T_TOOL_GANN_FAN315,				// ����(����45��)
		T_TOOL_GANN_GRID,				// ���׸���
		T_TOOL_ANDREWS_PITCHFORK,		// ����� ��ġ��ũ
		T_TOOL_ELLIOT_WAVE_LINE,		// ������Ʈ �ĵ���
		T_TOOL_CYCLE_LINES,				// ����Ŭ ����
		T_TOOL_CUT_LINE,				// ������ �м�����
		T_TOOL_SPEED_LINE,				// ���ǵ� ����
		T_TOOL_LINEAR_REGRESSION,		// ���� ȸ�ͼ�(ȸ�ͼ�)
		T_TOOL_RAFF_REGRESSION,			// Raff ä�μ�(ȸ��ä��)
		T_DELETE_INORDER,				// ���������� �� �����
		T_DELETE_ALL,					// ��������
		T_TRADER_REPORT,				// ���ǸŸ�, 
		T_SYSTEM_REPORT,				// �ý��� Ʈ���̴� ��������ȭ, 
		T_PRICE_FORCAST,				// �ְ�����,    
		T_MAKE_FORMULA,					// �����߰�,   
		T_PATTERN,						// �����Ϻм�,   
		T_SAVE_CHART_SCREEN,			// ȭ������,   
		T_PRINT,						// ����Ʈ,
		T_DAILY,						// �Ϻ�
		T_WEEKLY,						// �ֺ�
		T_MONTHLY,						// ����
		T_DISPLAY_SLIDERBAR,			// Display�� ����
		T_DISPLAY_RESTORE,				// Display�� �ʱ�ȭ
		T_MINMAX_FULLDATA,				// ��ü �ִ� �ּ� ����
		T_MAGNIFYINGGLASS,				// ������
		T_RADAR,						// ���̴���Ʈ
		T_REGISTER_CONCERN,				// ���ɵ��
		T_JOINT_ChARTPAGE,				// ��Ʈ ����
		T_SHOW_OUTERTOOLBAR,			// �ܺ����ٺ��̱�
		T_AUTOLINE_ANALYSIS,			// �ڵ��߼��м�		// (2005/11/17 - Seung-Won, Bae) Not Used Now.
		T_TARGET_ANALYSIS,				// ��ǥġ�м�
		T_SYSTEM_TRADE_INDICATOR,		// �ý��� Ʈ���̴� ��ǥ����ȭ 
		T_STRATEGIC_DEVELOPMENT,		// ��������
		T_STRATEGIC_VALUATION,			// ������
		T_TOOL_LINE_EXT1,				// �ܹ��� ���� �߼���	// (2004.12.09, ��¿�) �߼����� �������� �߰��Ѵ�.
		T_TOOL_LINE_EXT2,				// ����� ���� �߼���
		T_AUTOSCROLL,					// �ڵ���ũ��	
		T_CHART_INVERT,					// ��Ʈ������.
		T_CROSSHAIRS_TOOL,				// (2006/7/3 - Seung-Won, Bae) Crosshairs and NIDLG Tools
		T_CROSSHAIRS_TOOL_WITH_NIDLG,
		T_NIDLG_TOOL,
		T_TOOL_LINE_CROSS,				// ���ڼ� (CrossLine Object Tool)
		T_TOOL_LINE_CROSS_WITH_NIDLG,
		T_NIDLG_OVER,					// (2006/2/16 - Seung-Won, Bae) Over NIDLG and Crosshairs
		T_CROSSHAIRS_OVER,	
		T_TOOL_SPEED_FAN,				// ���ǵ� Fan�߼���
		T_TOOL_SPEED_ARC,				// ���ǵ� Arc�߼���
		T_TOOL_ANGLE_LINE,				// Angle Line
		T_TOOL_CANDLE_LINE,				// Candle�߽ɼ�
		T_TOOL_CANDLE_BONG,				// Candle��
		T_TOOL_TRACEING_TT,				// TracingToolTip�߼���
		T_TOOL_ADJUST_RATIO,			// ���������
		T_TOOL_PERIOD_SUMMARY,			// �Ⱓ��༱
		T_ENABLE_TIMEMARK,				// (2006/11/24 - Seung-Won, Bae) TimeMark, Fire&Receiving Enable Command Tool
		T_PRICE_YSCALE_SETUP,			// (2006/11/25 - Seung-Won, Bae) Price Chart YScale Setup Tool
		T_DRAG_X_ZOOM,					// (2006/11/27 - Sang-Woo, Cho) XScale Zoom
		T_DRAG_Y_ZOOM,					// (2006/11/27 - Sang-Woo, Cho) YScale Zoom
		T_PAN,							// (2006/11/27 - Sang-Woo, Cho) Pan
		T_UNDO_WORK,					// (2006/12/26 - Sang-Woo, Cho ) Undo
		T_REDO_WORK,					// (2006/12/26 - Sang-Woo, Cho ) Redo
		T_TOOL_PENCIL_LINE,				// �����߼���
		T_PRINT_CHART,					// �μ�
		T_ORDER_LINE,					// �ֹ���
		T_ORDER_LINE_MANAGER,			// �ֹ��� ����
		T_EXCEL_IMPORT,					// ���������� �ҷ�����
		T_ORDER_LINK,					// (2008/09/01 - Jin-Soon, Kim) �ֹ�â ���� 
		T_DATA_SIMULATOR,				// 20081011 �̹��� �����ͽùķ��̼� >>
		T_TOOL_BALANCE_BASIC,			// �ϸ��ġ			// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û

		T_TOOLOPTION_COUNT,				// (2006/11/24 - Seung-Won, Bae) OCX Tool Counts
		//////////////////////////////////////////////////////////////////////
		// (2005/10/26 - Seung-Won, Bae) Add AddIn Tool ID
		//////////////////////////////////////////////////////////////////////
		// Order Line AddIn
		T_ADDIN_ORDER_LINE = 1000,

		// Auto Scroll AddIn
		T_ADDIN_AUTOSCROLL_PLAY_LEFT = 1010,	
		T_ADDIN_AUTOSCROLL_PLAY_RIGHT,
		T_ADDIN_AUTOSCROLL_STOP,

		// Refresh the Button State
		T_ALL_TOOL_TRUE_QUERY = 1020,
		T_ALL_TOOL_RESET_STATE,

		T_SELECT_ENV_TAB_INDEX = 2000	// ȯ�漳�� �Ǻ� ���� ���� - ojtaso (20080109)
	};
};

// (2006/1/20 - Seung-Won, Bae) from OCX for sharing with AddIn.
// ��ġ��ȸ ���� Definition
#define NUMERICALINQUIRYDLG 0x1
#define CROSSLINEVIEW		0x2
#define TOOLTIPVIEW			0x4
#define NIDLG_MANUALMOVE	0x8
#define TIMEMARKER			0x10	// �ð������� ��� �ɼ�
#define TOOLTIPVIEW_ALL		0x20	// ǳ�����򸻺���� ��ü����

// Scroll�� ����Ÿ Ÿ���� �����Ѵ�. 
//#define SETSCROLL_TR			1	// TR����Ÿ�� ������ ���.
//#define SETSCROLL_REAL		2	// ����(�ڵ�����)����Ÿ�� ������ ���.
//#define SETSCROLL_REMOVEDATA	3	// ����Ÿ�� �ʹ� ���� ������ ����Ÿ�� ����� ���.
//#define SETSCROLL_WHOLEVIEW		4   // ��ü����

// (2009/9/1 - Seung-Won, Bae) Define Chart Mode.
// class CChartInfo
// {
// public:
// 	enum CHART_MODE
// 	{
// 		DEVELOPER = 0,
// 		HTS,
// 		FX,
// 		WORLD_ON,
// 		KRX_CME
// 	};
// };


class CCommonInterfaceEnum
{
public:
	enum ETCMESSAGETYPE
	{
		PRICE_LOG = 0
	};

	// PacketRQ�� ���¸� �˸����� �������̽� ���� : �������� - ojtaso (20070531)
	enum PACKET_RQ_STATUS
	{
		PACKETLISTMANAGER_NEW_RQ = 0,
		PACKETLISTMANAGER_CHANGE_RQ,
		PACKETLISTMANAGER_DELETE_RQ
	};
};

#endif // !defined(AFX_BLOCKBASEENUM_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
