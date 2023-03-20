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
		TOOL_GRAPHDELETE		// graph title / subGraph tool �� ����
	};
};

class CGraphBaseData
{
public:
	enum GRAPHTYPE
	{
		GTNONE = -1,
		Bong_Type = 0,		// �Ϻ���, �̱���, ���ν�
		Line_Type,			// �Ϲݽ�, ä���, ������, �����, ��ܽ�, 0���񿬼ӽ�, ����, ���κ����
		Bar_Type,			// ������, �����, ���Ͻ�, ������, ��Ʈ��
		Pie_Type,			// ����, ��ü��
		Special_Type,		// �Ｑ��ȯ��, ���ð�, P&F
		Fund_Type,			// �ݵ���
		DataView_Type,		// data ǥ��
		FuOp_Type,			// �����ɼ���

		GraphTypeCount		// GRAPHTYPE�� �Ѱ���
	};

	// Graph Style
	enum BONGSTYLE
	{
		CandlestickBong = 0,	// �Ϻ���
		HighLowCloseBong,		// �̱���
		LineBong,				// ���ν�
		AntennaBong				// ���׳���
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
		VerticalLine			// ������
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
		FixedRateCombBar		// ��������������
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
		Swing					// Swing
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
		TopDataBar = 0,			// ��ܽ� ����
		BottomDataBar,			// �ϴܽ� ����
		PointSignal,			// point signal
		TopSignal,				// ��� signal
		BottomSignal,			// �ϴ� signal
		CompartText				// �����ؽ�Ʈ��
	};

	enum FUOPSTYLE
	{
		MarketProfile,			// MarketProfile
		FuOpPAndF				// FuOpP&F
	};


	// Graph Drawing Type
	// -> SPECIALSTYLE, FUNDSTYLE, OPTIONSTYLE �� Drawing Type �� ����!
	enum BONGDRAWSTYLE		// BONGSTYLE
	{
		Candlestick_Draw = 0,	// �Ϻ���
		HighLowClose_Draw,		// �̱���
		Line_Draw,				// ���ν�
		Antenna_Draw			// ���׳���
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
		FLOOR,	// ����. 09:01:00 ~ 09:01:59 -> 09:01
		CEILING // �ø�. 09:00:01 ~ 09:01:00 -> 09:01 (�޸���)
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
		STOCK = 0,	// �ֽ�
		INDUSTRY,	// ����
		FUTURES,	// ����
		OPTION		// �ɼ�
	};

	enum ETC
	{
		NEWS,			// ����
		ACCOUNT_ITEMIZE // �ŷ�����
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
		MENU_HORZSCALE_POSITION // x�� scale ��ġ
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
		ZOOM_IN
	};
};


class CAnalysis
{
public:
	enum COORDINATETYPE
	{
		RELATIVE_POINT,	// ��� ��ǥ�� - data ����
		ABSOLUTE_POINT	// ���� ��ǥ�� - pixel ����
	};

	enum ANALYSIS
	{
		ANALYSIS_TOOL,
		ANALYSIS_FUNCTION
	};

	enum TOOLTYPE
	{
		TOOL_NONE = -1,
		TOOL_SELECTION,		// ����
		TOOL_MOVE_CROSS,	// ���ڼ�(mouse move ��)
		TOOL_DOWN_CROSS,	// ���ڼ�(mouse down ��)
		TOOL_VERT_LINE,		// ������
		TOOL_HORZ_LINE,		// ����
		TOOL_LINE,			// �߼���
		TOOL_TRISECT,		// ���м�
		TOOL_QUARTER,		// ���м�
		TOOL_FIBO_SEMICIRCLE,// �Ǻ���ġ �ݿ�
		TOOL_FIBO_CIRCLE,	// �Ǻ���ġ ��
		TOOL_FIBO_FAN_LINE,	// �Ǻ���ġ �Ҷ���
		TOOL_FIBO_RETRACE,	// �Ǻ���ġ �ǵ���(������)
		TOOL_FIBO_TIME,		// �Ǻ���ġ �ð���
		TOOL_GANN_LINE45,	// ������(���45��)
		TOOL_GANN_LINE315,	// ������(����45��)
		TOOL_GANN_FAN45,	// ����(���45��)
		TOOL_GANN_FAN135,	// ����(�»�45��)
		TOOL_GANN_FAN225,	// ����(����45��)
		TOOL_GANN_FAN315,	// ����(����45��)
		TOOL_GANN_GRID,		// ���׸���
		TOOL_ANDREWS_PITCHFORK,	// ����� ��ġ��ũ
		TOOL_ELLIOT_WAVE_LINE,	// ������Ʈ �ĵ���
		TOOL_CYCLE_LINES,	// ����Ŭ ����
		TOOL_CUT_LINE,		// ������ �м�����
		TOOL_SPEED_LINE,	// ���ǵ� ����
		TOOL_LINEAR_REGRESSION,// ���� ȸ�ͼ�(ȸ�ͼ�)
		TOOL_RAFF_REGRESSION,// Raff ä�μ�(ȸ��ä��)
		// (2004.12.09, ��¿�) �߼����� �������� �߰��Ѵ�.
		TOOL_LINE_EXT1,		// �ܹ��� ���� �߼���
		TOOL_LINE_EXT2		// ����� ���� �߼���
	};
};


class CSymbol
{
public:
	enum SYMBOLTOOL
	{
		SYMBOL_NONE,
		SYMBOL_SQUARE, //�簢����
		SYMBOL_CIRCLE,	// ��
		SYMBOL_TRIANGLE,	// �ﰢ��
		SYMBOL_TEXT,  // ����
		SYMBOL_BULET, //��
		SYMBOL_SELECTION //����
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
		T_INFLATE,			// Ȯ��,���"),
		T_WHOLEVIEW,		// ��ü����
		T_INQUIRY_VALUE,	// ��ġ��ȸâ"), 
		T_DATALIST_REPORT,	// �����ͺ���"),   
		T_INQUIRY_STATUS,	// ��ġ��ȸ�����ͺ���"),
		T_OBJ_CFG,			// ��ü����"),   
		T_OBJ_HIGHLIGHTt,	// ��ü����"),
		T_GRAPH_STYLE,		// Graph����"),   
		T_GRAPH_THICKNESS,	// ������"),   
		T_GRAPH_COLOR,		// ������"),   
		T_INDICATOR_CFG,	// ��ǥ����"),   
		T_ENVIRONMENT_CFG,	// ȯ�漳��"),
		T_INDICATOR_LIST,	// ��ǥ����Ʈ����"),
		T_INDICATOR_SERIES,	// ��ǥ������ȸ"), 
		T_PANORAMA,			// �ĳ��"),     
		T_SCROLL,			// ��ũ�ѹ�"),
		T_PREV,				// ����"),
		T_NEXT,				// ����"),
		T_RESTORE,			// �ʱ�ȭ"),  		
		T_REMOVE_OBJ,		// ��ü����"), 
		T_REMOVE_BLOCK,		// ������"), 
		T_CHART_RAINBOW,	// ���κ���"), 
		T_CHART_BALANCE,	// �ϸ����ǥ"), 
		T_CHART_PANDF,		// P&F"),   
		T_CHART_THREELINEBREAK,// �Ｑ��ȯ��"),
		T_CHART_REVERSE,	// ���ð�"), 
		T_CHART_BARFORSALE,	// ���Ź���Ʈ"), 
		T_SYMBOL_BULET,		// ��ȣDlg"), 
		T_SYMBOL_SQUARE,	// �簢����
		T_SYMBOL_CIRCLE,	// ��������
		T_SYMBOL_TRIANGLE,	// �ﰢ��
		T_SYMBOL_TEXT,		// ����
		T_TOOL_SELECTION,		// ����
		T_TOOL_LINE_PROFIT,		// �߼������ͷ�
		T_TOOL_LINE,			// �߼���
		T_TOOL_LINE_CROSS,		// ���ڼ�
		T_TOOL_LINE_VERT,		// ������
		T_TOOL_LINE_HORZ,		// ����
		T_TOOL_LINE_TRISECT,	// ���м�
		T_TOOL_LINE_QUARTER,	// ���м�
		T_TOOL_LINE_RESISTANCE,	// �������׼�
		T_TOOL_FIBONACCI_ARC,	// �Ǻ���ġ �ݿ�
		T_TOOL_FIBO_CIRCLE,		// �Ǻ���ġ ��
		T_TOOL_FIBONACCI_FAN,	// �Ǻ���ġ �Ҷ���
		T_TOOL_FIBO_RETRACE,	// �Ǻ���ġ �ǵ���(������)
		T_TOOL_FIBONACCI_TIME,	// �Ǻ���ġ �ð���
		T_TOOL_GANN_LINE45,		// ������(���45��)
		T_TOOL_GANN_LINE315,	// ������(����45��)
		T_TOOL_GANN_FAN45,		// ����(���45��)
		T_TOOL_GANN_FAN135,		// ����(�»�45��)
		T_TOOL_GANN_FAN225,		// ����(����45��)
		T_TOOL_GANN_FAN315,		// ����(����45��)
		T_TOOL_GANN_GRID,		// ���׸���
		T_TOOL_ANDREWS_PITCHFORK,	// ����� ��ġ��ũ
		T_TOOL_ELLIOT_WAVE_LINE,	// ������Ʈ �ĵ���
		T_TOOL_CYCLE_LINES,		// ����Ŭ ����
		T_TOOL_CUT_LINE,		// ������ �м�����
		T_TOOL_SPEED_LINE,		// ���ǵ� ����
		T_TOOL_LINEAR_REGRESSION,// ���� ȸ�ͼ�(ȸ�ͼ�)
		T_TOOL_RAFF_REGRESSION,	// Raff ä�μ�(ȸ��ä��)
		T_DELETE_INORDER,		// ���������� �� �����
		T_DELETE_ALL,			// ��������
		T_TRADER_REPORT,		// ���ǸŸ�"), 
		T_SYSTEM_REPORT,		// �ý��� Ʈ���̴� ��������ȭ"), 
		T_PRICE_FORCAST,		// �ְ�����"),    
		T_MAKE_FORMULA,			// �����߰�"),   
		T_PATTERN,				// �����Ϻм�"),   
		T_SAVE_CHART_SCREEN,	// ȭ������"),   
		T_PRINT,				// ����Ʈ"),
		T_DAILY,				// �Ϻ�")
		T_WEEKLY,				// �ֺ�
		T_MONTHLY,				// ����
		T_DISPLAY_SLIDERBAR,	// Display�� ����
		T_DISPLAY_RESTORE,		// Display�� �ʱ�ȭ
		T_MINMAX_FULLDATA,		// ��ü �ִ� �ּ� ����
		T_MAGNIFYINGGLASS,		// ������
		T_RADAR,				// ���̴���Ʈ
		T_REGISTER_CONCERN,		// ���ɵ��
		T_JOINT_ChARTPAGE,		// ��Ʈ ����
		T_SHOW_OUTERTOOLBAR,	// �ܺ����ٺ��̱�
		T_AUTOLINE_ANALYSIS,	// �ڵ��߼��м�
		T_TARGET_ANALYSIS,		// ��ǥġ�м�
		T_SYSTEM_TRADE_INDICATOR,	// �ý��� Ʈ���̴� ��ǥ����ȭ 
		T_STRATEGIC_DEVELOPMENT,	// ��������
		T_STRATEGIC_VALUATION,		// ������
		// (2004.12.09, ��¿�) �߼����� �������� �߰��Ѵ�.
		T_TOOL_LINE_EXT1,		// �ܹ��� ���� �߼���
		T_TOOL_LINE_EXT2		// ����� ���� �߼���
	};
};

class CIndicatorBaseData  
{
public:
	enum INDICATOR_NAME_NO_CAL
	{
		INDI_PRICE = 0,	// ������Ʈ
		INDI_VOLUME,	// �ŷ�����Ʈ
		INDI_OPTIMIZE_SIGNAL,	// IndicatorOptimize Signal

		NUM_NO_CALCULATION
	};

	enum INDICATOR_NAME_CAL
	{
		INDI_PRICE_MA = 0,	// ���� �̵����
		INDI_VOLUME_MA,		// �ŷ��� �̵����
		INDI_HORIZONTALBAR,	// ���Ź�
		INDI_DISPARITY,		// �̰ݵ�
		INDI_GLANCEBALANCE,	// �ϸ����ǥ
		INDI_PSYCH,			// ���ڽɸ���
		INDI_NEWPSYCH,		// �����ڽɸ���
		INDI_AB_RATIO,		// AB Ratio
		INDI_ATR,			// ATR : Average True Range
		INDI_BOLLINGER,		// Bollinger Bands
		INDI_BOXCHART,		// Box Chart
		INDI_CCI,			// CCI
		INDI_CLC,			// ��������Ʈ
		INDI_CMF,			// CMF
		INDI_CHAIKINS_OSC,	// CO : Chaikin's Oscillator
		INDI_DMI,			// DMI
		INDI_ENVELOPE,		// Envelope
		INDI_IMI,			// IMI
		INDI_FOURIER_TRANSFORM,	// Fourier Transform
		INDI_MAC,			// MAC
		INDI_MACD,			// MACD
		INDI_MACD_OSC,		// MACD Oscillator
		INDI_MFI,			// MFI
		INDI_MI,			// MI : MassIndex
		INDI_MOMENTUM,		// Momentum
		INDI_NVI,			// NVI : Negative Volume Index
		INDI_OBV,			// OBV
		INDI_OCMA,			// OCMA
		INDI_OSCV,			// OSCV : Volume Oscillator
		INDI_PARABOLIC,		// Parabolic
		INDI_PIVOT,			// Pivot
		INDI_PVI,			// PVI : Positive Volume Index
		INDI_OSCP,			// OSCP : Price Oscillator
		INDI_PVT,			// PVT : Price Volume Trend
		INDI_RSCOMPARATIVE,	// ������Ʈ
		INDI_ROC,			// RoC : Rate of Change
		INDI_RSI,			// RSI
		INDI_SONAR,			// SONAR
		INDI_STOCHASTICS_FAST,	// Stochastics Fast
		INDI_STOCHASTICS_SLOW,	// Stochastics Slow
		INDI_STOCHASTICS_OSC,	// Stochastics Oscillator
		INDI_TRIN,			// TRIN
		INDI_VOLUME_ROC,	// VROC : Volume Rate of Change
		INDI_VR,			// VR
		INDI_WILLIAM,		// William's %R
		INDI_TRIX,			// TRIX
		INDI_RAINBOW,		// �׹���Ʈ
		INDI_THREE_LINE_BREAK,	// �Ｑ��ȯ��
		INDI_INVERSE_LINE,		// ���ð�
		INDI_PF,				// P&F
		INDI_ACCOUNTITEMIZE,	// �ŷ�����
		INDI_LOCK,			// ��
		INDI_BIGNEWS,		// Big News
		INDI_AD,			// A/D Line : Accumulation/Distribution
		INDI_ZIGZAG,		// Zig Zag
		INDI_EMV,			// EMV
		INDI_DPO,			// DPO
		INDI_ABI,			// ABI
		INDI_ADL,			// ADL
		INDI_ADR,			// ADR
		INDI_BREADTH_THRUST,		// Breadth Thrust
		INDI_MCCLELLAN_OSC,			// McClellan Oscillator
		INDI_STANDARD_DEVIATION,	// Standard Deviation
		INDI_STIX,			// STIX
		INDI_VHF,			// VHF
		INDI_ADX,			// ADX
		INDI_WEIGHTCLOSE,	// Wighted Close
		INDI_CHAIKIN,		// Chaikin's Volatility
		INDI_BANDB,			// Band %B
		INDI_BANDWIDTH,		// Band Width
		INDI_BINARYWAVE,	// Binary Wave
		INDI_EOM,			// EOM
		INDI_DEMARK,		// Demark
		INDI_WILLIAMAD,		// Will A/D
		INDI_SMI,			// SMI
		INDI_SWINGINDEX,	// SWINGINDEX
		INDI_ASI,			// ASI
		INDI_RSQUARED,		// RSquared
		INDI_STANDARDERROR,	// Standard Error
		INDI_ENERGY,		// Energy
		INDI_TSF_OSC,		// TSF Oscillator
		INDI_TSI,			// TSI
		INDI_MACHANNEL,		// �̵���� ä��
		INDI_NCO,			// NCO
		INDI_SIGMA,			// SIGMA
		INDI_MA_OSC,		// MA Oscillator
		INDI_TSF,			// TSF
		INDI_LRS,			// LRS
		INDI_CANDLEVOLUME,	// CandleVolume
		
		NUM_CALCULATION
	};
};

#endif // !defined(AFX_BLOCKBASEENUM_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
