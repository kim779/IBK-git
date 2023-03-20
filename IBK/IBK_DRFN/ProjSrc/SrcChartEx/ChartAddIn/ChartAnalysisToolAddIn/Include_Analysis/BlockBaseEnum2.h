// BlockBaseEnum2.h: defines for Block Data
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKBASEENUM2_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
#define AFX_BLOCKBASEENUM2_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_

// (2004.10.12, ��¿�) BlockBaseData.h���� Enum Data�� ���� Define ��� ���� ������ Header�� �и��Ѵ�.
//		(DLL �� �ܺ� Module �����ڿ� �����ϱ� ����)
//		* ����. ���� �� �κ��� ������ �ܺ� Module ���� ����ڿ��� �˷� �ܺ� Module���� ����ǵ��� �Ѵ�.
//			���� ������ AddIn Header Folder�� �ξ� ������� ������ ����
//		*. ����, ������ ���� Ȯ���� �����ϳ� ������ ���� �ʵ��� �Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
		EN_ANALYSIS_NONE = -1,
		EN_ANALYSIS_SELECTION		= 0,		// ����
		EN_ANALYSIS_MOVE_CROSS		= 1,		// ���ڼ�(mouse move ��)
		EN_ANALYSIS_DOWN_CROSS		= 2,		// ���ڼ�(mouse down ��)
		EN_ANALYSIS_VERT_LINE		= 3,		// ������
		EN_ANALYSIS_HORZ_LINE		= 4,		// ����
		EN_ANALYSIS_LINE			= 5,		// �߼���
		EN_ANALYSIS_TRISECT			= 6,		// ���м�
		EN_ANALYSIS_QUARTER			= 7,		// ���м�
		EN_ANALYSIS_FIBO_SEMICIRCLE = 8,		// �Ǻ���ġ �ݿ�
		EN_ANALYSIS_FIBO_CIRCLE		= 9,		// �Ǻ���ġ ��
		EN_ANALYSIS_FIBO_FAN_LINE	= 10,		// �Ǻ���ġ �Ҷ���
		EN_ANALYSIS_FIBO_RETRACE	= 11,		// �Ǻ���ġ �ǵ���(������)
		EN_ANALYSIS_FIBO_TIME		= 12,		// �Ǻ���ġ �ð���
		EN_ANALYSIS_GANN_LINE45		= 13,		// ������(���45��)
		EN_ANALYSIS_GANN_LINE315	= 14,		// ������(����45��)
		EN_ANALYSIS_GANN_FAN45		= 15,		// ����(���45��)
		EN_ANALYSIS_GANN_FAN135		= 16,		// ����(�»�45��)
		EN_ANALYSIS_GANN_FAN225		= 17,		// ����(����45��)
		EN_ANALYSIS_GANN_FAN315		= 18,		// ����(����45��)
		EN_ANALYSIS_GANN_GRID		= 19,		// ���׸���
		EN_ANALYSIS_ANDREWS_PITCHFORK	= 20,	// ����� ��ġ��ũ
		EN_ANALYSIS_ELLIOT_WAVE_LINE	= 21,	// ������Ʈ �ĵ���
		EN_ANALYSIS_CYCLE_LINES		= 22,		// ����Ŭ ����
		EN_ANALYSIS_CUT_LINE		= 23,		// ������ �м�����
		EN_ANALYSIS_SPEED_LINE		= 24,		// ���ǵ� ����
		EN_ANALYSIS_SPEED_FAN		= 25,		// ���ǵ� Fan
		EN_ANALYSIS_SPEED_ARC		= 26,		// ���ǵ� ȣ
		EN_ANALYSIS_LINEAR_REGRESSION	= 27,	// ���� ȸ�ͼ�(ȸ�ͼ�)
		EN_ANALYSIS_RAFF_REGRESSION	= 28,		// Raff ä�μ�(ȸ��ä��)		
		EN_ANALYSIS_ANGLE_LINE		= 29,		// �����߼���
		EN_ANALYSIS_CANDLE_LINE		= 30,		// ĵ���߽ɼ�
		EN_ANALYSIS_CANDLE_BONG		= 31,		// ĵ�鰡���
		EN_ANALYSIS_ADJUST_RATIO	= 32,		// ���������
		EN_ANALYSIS_PERIOD_SUMMARY	= 33,		// ���ݺ�ȭ��
		EN_ANALYSIS_TRACING_TT		= 34,		// �ڵ�����
		EN_ANALYSIS_PENCIL_LINE		= 35,		// �����߼���
		EN_ANALYSIS_BALANCE_BASIC	= 36,		// �ϸ��ġ			// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û
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

// (2006/1/20 - Seung-Won, Bae) from OCX for sharing with AddIn.
// ��ġ��ȸ ���� Definition
#define NUMERICALINQUIRYDLG 0x1
#define CROSSLINEVIEW		0x2
#define TOOLTIPVIEW			0x4
#define NIDLG_MANUALMOVE	0x8
#define TIMEMARKER			0x10	// �ð������� ��� �ɼ�
#define TOOLTIPVIEW_ALL		0x20	// ǳ�����򸻺���� ��ü����

#endif // !defined(AFX_BLOCKBASEENUM2_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
