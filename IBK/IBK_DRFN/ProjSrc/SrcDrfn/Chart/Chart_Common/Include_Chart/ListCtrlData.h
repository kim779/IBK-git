#ifndef __LISTCTRLDATA_H__
#define __LISTCTRLDATA_H__

#include <atlconv.h>

///////////////
//data
static const int NUM_TOOLRBUTTONITEMS = 82;

static const _TCHAR *pToolRButtonItem[NUM_TOOLRBUTTONITEMS] = 
{
	_T("Ȯ��,���"),  
	_T("��ü����"),  
	_T("��ġ��ȸâ"), 
	_T("�����ͺ���"),   
	_T("��ġ��ȸ�����ͺ���"),
	_T("��ü����"),  
	_T("��ü����"),  
	_T("Graph����"),   
	_T("������"),   
	_T("������"),   
	_T("��ǥ����"),   
	_T("ȯ�漳��"),
	_T("��ǥ����Ʈ����"),
	_T("��ǥ������ȸ"), 
	_T("�ĳ��"),     
	_T("��ũ�ѹ�"),
	_T("����"),
	_T("����"),
	_T("�ʱ�ȭ"),  
	_T("��ü����"), 
	_T("������"), 
	_T("���κ���"), 
	_T("�ϸ����ǥ"), 
	_T("P&F"),   
	_T("�Ｑ��ȯ��"),
	_T("���ð�"), 
	_T("���Ź���Ʈ"), 
	_T("��ȣDlg"), 
	_T("�簢����"), 
	_T("��������"), 
	_T("�ﰢ��"), 
	_T("Text"), 
	_T("����"), 
	_T("�߼������ͷ�"), 
	_T("�߼���"), 
	_T("���ڼ�"), 
	_T("������"), 
	_T("����"), 
	_T("���м�"), 
	_T("���м�"), 
	_T("�������׼�"), 
	_T("�Ǻ���ġ �ݿ�"), 
	_T("�Ǻ���ġ ��"), 
	_T("�Ǻ���ġ �Ҷ���"), 
	_T("�Ǻ���ġ �ǵ���"), 
	_T("�Ǻ���ġ �ð���"), 
	_T("������(���45��)"), 
	_T("������(����45��)"), 
	_T("����(���45��)"), 
	_T("����(�»�45��)"), 
	_T("����(����45��)"), 
	_T("����(����45��)"), 
	_T("���׸���"), 
	_T("����� ��ġ��ũ"), 
	_T("������Ʈ �ĵ���"), 
	_T("Ÿ�� ����Ŭ"), 
	_T("������ �м�����"), 
	_T("���ǵ� ����"), 
	_T("���� ȸ�ͼ�(ȸ�ͼ�)"), 
	_T("Raff ä�μ�(ȸ��ä��)"), 
	_T("���������� �� �����"), 
	_T("��������"), 
	_T("���ǸŸ�"), 
	_T("�ý��� Ʈ���̴�"), 
	_T("�ְ�����"),    
	_T("�����߰�"),   
	_T("���Ϻм�"),   
	_T("ȭ������"),   
	_T("����Ʈ"),
	_T("�Ϻ�"),
	_T("�ֺ�"),
	_T("����"),
	_T("Display�� ������"),
	_T("Display�� �ʱ�ȭ"),
	_T("��ü �ִ��ּ� ����"),
	_T("������"),
	_T("���̴���Ʈ"),
	_T("���ɵ��"),
	_T("��Ʈ����"),
	_T("�ܺ����ٺ��̱�"),
	_T("�ڵ��߼��м�"),
	_T("��ǥġ�м�")
};

//data
static const int NUM_PACKETNAMEITEM = 23;

static const _TCHAR *pPacketNameItem[NUM_PACKETNAMEITEM] = 
{
	_T("�ڷ�����"),  
	_T("Text"), 
	_T("�ð�"),   
	_T("��"),   
	_T("����"), 
	_T("����"), 
	_T("�⺻�ŷ���"),     
	_T("�����ŷ���"),    
	_T("������"),    
	_T("�ֽĹ��"),    
	_T("ȯ��"),    
	_T("��ü�����"),    
	_T("��������"),    
	_T("�϶������"),    
	_T("ADL"),    
	_T("MOBV"),    
	_T("�������ŷ���"),    
	_T("�϶�����ŷ���"),    
	_T("������ �ŷ���"),    
	_T("52�Ű������"),    
	_T("52���������"),    
	_T("�����ֽļ�"),    
	_T("�ŷ����������")    
};

static const int NUM_GRAPHNAMEITEM = 170;

static const _TCHAR *pGraphNameItem[NUM_GRAPHNAMEITEM] = 
{
	_T("������Ʈ"),				// 0
	_T("�ŷ�����Ʈ"),  
	_T("���� �̵����"),
	_T("�ŷ��� �̵����"),
	_T("���Ź�"), 	
	_T("�̰ݵ�"),     
	_T("�ϸ����ǥ"),	
	_T("���ڽɸ���"),
    _T("�Žɸ���"),
	_T("AB Ratio"),    
	_T("ATR"),    				// 10
	_T("Bollinger Bands"),
    _T("Box Chart"),
	_T("CCI"), 
	_T("��������Ʈ"),
	_T("CMF"),
	_T("CO"),    
	_T("DMI"),    
	_T("Envelope"),
    _T("IMI"),
	_T("Fourier Transform"),   	// 20
	_T("MAC"),    
	_T("MACD"),    
	_T("MACD Oscillator"),    
	_T("MFI"),    
	_T("MI"),    
	_T("Momentum"),    
	_T("NVI"),    
	_T("OBV"),    
	_T("OCMA"),
	_T("OSCV"), 				// 30
	_T("Parabolic"), 
	_T("Pivot"), 
	_T("PVI"), 
	_T("OSCP"), 
	_T("PVT"), 
	_T("������Ʈ"), 
	_T("RoC"),    
	_T("RSI"), 	
	_T("SONAR"), 	
	_T("Stochastics Fast"), 	// 40
	_T("Stochastics Slow"),
	_T("Stochastics Oscillator"),
	_T("TRIN"),
	_T("VROC"), 
	_T("VR"), 	
	_T("William's %R"), 	
	_T("TRIX"), 	
	_T("�׹���Ʈ"),	
	_T("�Ｑ��ȯ��"),
	_T("���ð�"), 			// 50
	_T("P&F"),
	_T("FuOpP&F"),
	_T("�ŷ�����"),
	_T("��"),
	_T("Big News"),
	_T("MarketProfile"),

	///����� ��ǥ���� �߰��� ����
	_T("A/D Line"),
	_T("Zig Zag"),
	_T("EMV"),
	_T("DPO"),					// 60
	_T("ABI"),
	_T("ADL"),
	_T("ADR"),
	_T("Breadth Thrust"),
	_T("McClellan Oscillator"),
	_T("Standard Deviation"),
	_T("STIX"),
	_T("VHF"),

	//����
	_T("ADX"),
	_T("Weighted Close"),		// 70
	_T("Chaikin's Volatility"),
	_T("Band %B"),
	_T("Band Width"),
	_T("Binary Wave"),
	_T("EOM"),
	_T("Demark"),
	_T("Will A/D"),
	_T("SMI"),
	_T("Swing Index"),
	_T("ASI"),					// 80
	_T("RSquared"),
	_T("Standard Error"),
	_T("Energy"),
	_T("TSF Oscillator"),
	_T("TSI"),
	_T("�̵���� ä��"),
	_T("NCO"),
	_T("SIGMA"),
	_T("MA Oscillator"),
	_T("TSF"),					// 90
	_T("LRS"),
	_T("CandleVolume"),
	_T("IndicatorOptimize Signal"),
	_T("SwingChart"),
	_T("KagiChart"),
	_T("RenkoChart"),			// YJS 05/06/21 RenKoChart ADD.
	// ADD: ����(04/04/06) �޸������� ������ǥ 10���߰�
	_T("����Ź��"),				
	_T("����,�ɼ� �̰��� ����"),	
	_T("�����ŷ�������"),			
	_T("��������"),				// 100	
	_T("�ſ������ܰ�"), 
	_T("��������"),					
	_T("�ܱ���������"),		
	_T("�ܱ��μ��ż�"),				
	_T("��Ź�ڹ̼���"),
	// ADD: ����(04/04/06) �޸������� ������ǥ 3���߰�
	_T("����������"), 
	_T("���������"),
	_T("�ܱ���������"),

	// ��ȭ
	_T("7Binary Wave"), 
	_T("VA Oscillator"),			// 110
	_T("Price Channel"), 
	_T("Aroon"), 
	_T("Formula"), 
	_T("TDI"), 
	_T("Elder Ray Bear"),
	_T("Elder Ray Bull"),
	_T("Standard Error"), 
	_T("Volume Ratio"), 
	_T("VROC"), 
	_T("����ּ�(ADL)"),	// 120
	_T("����ֺ���(ADR)"), 
	_T("�ɸ���"), 
	_T("DEMA"), 
	_T("TEMA"), 
	_T("�̰ݵ�(�ܼ�)"),
	_T("�̰ݵ�(����)"),
	_T("Starc Bands"),
	_T("NDI"),
	_T("%R"),
	_T("Qstic"),	// 130
	_T("Force Index"),
	_T("%B+Stochastic"),
	_T("LRS+Stochastic"),
	_T("MACD+Stochastic"),
	_T("Momentum+Stochastic"),
	_T("OBV+Momentum"),
	_T("OBV+Stochastic"),
	_T("ROC+Stochastic"),
	_T("RSI+MACD"),
	_T("RSI+Stochastic"),	// 140
	_T("Sonar+�ɸ���"),
	_T("Stochastic+RSI"),
	_T("ǥ�ؿ������"),
	_T("ǥ�ؿ���ä��"),
	_T("ǥ������ä��"),
	_T("Parabolic SAR"),
	_T("ADXR"),
	_T("MAO"),
	_T("Price Oscillator"),
	_T("Chaikin's Oscillator"),	// 150
	_T("Volume Oscillator"),
	_T("SROC"),
	_T("Mass Index"),
	_T("RMI"),
	_T("DRF"),
	_T("�ݸ�"),
	_T("���� ����"),
	_T("���� �Ǽ�"),
	_T("�ð��Ѿ�"),
	_T("�ſ��ܰ�"),	// 160
	_T("��Ź��"),
	_T("�ܱ��κ�����"),
	_T("�ܱ��κ�����"),
	_T("�ܱ��μ�����"),
	_T("�����ڼ��ż�(����)"),
	_T("�����ڼ��ż�(Call)"),
	_T("�����ڼ��ż�(KOSDAQ)"),
	_T("�����ڼ��ż�(KOSPI)"),
	_T("�����ڼ��ż�(Put)"),
	_T("�ż��ŵ����Ź�"),
	_T("Energy"),
	_T("�ſ��ܰ���"),
	_T("�ſ������"),
	_T("�ſ��ܰ����"),
	_T("�ſ��ܰ�����")
};

typedef enum FORMULATE
{
	GRAPH_PRICE = 0,				// 0
	GRAPH_VOLUME,
	GRAPH_PRICE_MA,
	GRAPH_VOLUME_MA,
	GRAPH_HORIZONTALBAR,
	GRAPH_DISPARITY,
	GRAPH_GLANCEBALANCE,
	GRAPH_PSYCH,
	GRAPH_NEWPSYCH,
	GRAPH_AB_RATIO,
	GRAPH_ATR,					// 10
	GRAPH_BOLLINGER,
	GRAPH_BOXCHART,
	GRAPH_CCI,
	GRAPH_CLC,
	GRAPH_CMF,
	GRAPH_CO,
	GRAPH_DMI,
	GRAPH_ENVELOPE,
	GRAPH_IMI,
	GRAPH_FOURIER_TRANSFORM,	// 20
	GRAPH_MAC,				
	GRAPH_MACD,				
	GRAPH_MACD_OSC,			
	GRAPH_MFI,				
	GRAPH_MI,				
	GRAPH_MOMENTUM,	
	GRAPH_NEGATIVE_VOLUME_INDEX, // NVI
	GRAPH_OBV,				
	GRAPH_OCMA,	
	GRAPH_OSCV,					// 30
	GRAPH_PARABOLIC,		
	GRAPH_PIVOT,				
	GRAPH_POSITIVE_VOLUME_INDEX, // PVI
	GRAPH_OSCP,
	GRAPH_PRICE_VOLUME_TREND, // PVT
	GRAPH_RELATIVE_STRENGTH_COMPARATIVE,
	GRAPH_ROC,	
	GRAPH_RSI,				
	GRAPH_SONAR,			
	GRAPH_STOCHASTICS_FAST,		// 40
	GRAPH_STOCHASTICS_SLOW,
	GRAPH_STOCHASTICS_OSCILLATOR,
	GRAPH_TRIN,
	GRAPH_VOLUME_ROC,	
	GRAPH_VR,			
	GRAPH_WILLIAM,			
	GRAPH_TRIX,
	GRAPH_RAINBOW,
	GRAPH_THREE_LINE_BREAK,
	GRAPH_INVERSE_LINE,			// 50
	GRAPH_P_AND_F,
	GRAPH_FUOPPF,
	GRAPH_AccountItemize,
	GRAPH_Lock,
	GRAPH_BigNews,
	GRAPH_MarketProfile,

	///������...���ҽ����� �߰��� ��ǥ
	GRAPH_AD,					/// A/D Line : Accumulation/Distribution
	GRAPH_ZIGZAG,				/// Zig Zag
	GRAPH_EMV,					/// Ease Of Movement
	GRAPH_DPO,					/// Detreded Price Oscillator
	GRAPH_ABI,					/// Absolute Breadth Index
	GRAPH_ADL,					/// Advanced Decline Line
	GRAPH_ADR,					/// Advanced Decline Ratio
	GRAPH_BREADTH_THRUST,		/// Breadth Thrust
	GRAPH_MCCLELLAN_OSCILLATOR,	/// McClellan Oscillator
	GRAPH_STANDARD_DEVIATION,	/// Standard Deviation
	GRAPH_STIX,					/// STIX
	GRAPH_VHF,					/// Vertical horizontal Filter

	//����
	GRAPH_ADX,
	GRAPH_WEIGHTCLOSE,
	GRAPH_ChaikinsVolatility,
	GRAPH_BANDB,
	GRAPH_BandWidth,
	GRAPH_BinaryWave,
	GRAPH_EOM,
	GRAPH_Demark,
	GRAPH_WilliamAD,
	GRAPH_SMI,
	GRAPH_SwingIndex,
	GRAPH_ASI,
	GRAPH_RSquared,
	GRAPH_StandardError,
	GRAPH_Energy,
	GRAPH_TSFOscillator,
	GRAPH_TSI,
	GRAPH_MAChannel,
	GRAPH_NCO,
	GRAPH_Sigma,
	GRAPH_MaOscillator,
	GRAPH_TSF,
//	GRAPH_IMI,
	GRAPH_LRS,
	GRPAH_CandleVolume,
	GRAPH_IndicatorOptimizeSignal,
	GRAPH_SwingChart,
	GRAPH_KagiChart,
	GRAPH_RENKO_CHART,			// RenKoChart ADD.
	// ADD: ����(04/04/06) �޸������� ������ǥ
	GRAPH_CustomerDeposition,	// ����Ź��
	GRAPH_FutureOptionIncomplete,	// �����ɼǹ̰��� ����
	GRAPH_TradeYeSuKm,	// �ŷ�������
	GRAPH_MarketPriceIndex,	// ��������
	GRAPH_FinancingBalance,	// �ſ������ܰ�
	GRAPH_IndustryPriceIndex,	// ��������
	GRAPH_ForeignerJiBunYul,	// �ܱ���������
	GRAPH_ForeignerPureBuy,	// �ܱ��μ��ż�
	GRAPH_CommissionUncollected,	// ��Ź�ڹ̼���
	// ADD: ����(04/08/02) �����ڵ��⿡ Pakcet�����κ� �߰�
	GRAPH_Individual_Investor, // ����������
	GRAPH_Organ_Investor, // ���������
	GRAPH_Foreigner_Investor,  // �ܱ���������

	// ��ȭ
	GRAPH_7BINARYWAVE,
	GRAPH_VA_OSCILLATOR,
	GRAPH_PRICE_CHANNEL,
	GRAPH_AROON,
	GRAPH_FORMULA,
	GRAPH_TDI,
	GRAPH_ELDER_RAY_BEAR,
	GRAPH_ELDER_RAY_BULL,
	GRAPH_STANDARD_ERROR,
	GRAPH_VOLUME_RATIO,
	GRAPH_VROC,
	GRAPH_UPDOWN_ADL,
	GRAPH_UPDOWN_ADR,
	GRAPH_PSYCO_LINE,
	GRAPH_DEMA,
	GRAPH_TEMA, 
	GRAPH_DISPARITY_SIMPLE,
	GRAPH_DISPARITY_COMPLEX,
	GRAPH_STARC_BAND,
	GRAPH_NDI,
	GRAPH_PERCENT_R,
	GRAPH_QSTIC,
	GRAPH_FORCE_INDEX,
	GRAPH_PERCENT_B_STOCHASTIC,
	GRAPH_LRS_STOCHASTIC,
	GRAPH_MACD_STOCHASTIC,
	GRAPH_MOMENTUM_STOCHASTIC,
	GRAPH_OBV_MOMENTUM,
	GRAPH_OBV_STOCHASTIC,
	GRAPH_ROC_STOCHASTIC,
	GRAPH_RSI_MACD,
	GRAPH_RSI_STOCHASTIC,
	GRAPH_SONAR_PSYCO_LINE,
	GRAPH_STOCHASTIC_RSI,
	GRAPH_STANDARD_ERROR_BAND,
	GRAPH_STANDARD_ERROR_CHANNEL,
	GRAPH_STANDARD_GAP_BAND,
	GRAPH_PARABOLIC_SAR,
	GRAPH_ADXR,
	GRAPH_MAO,
	GRAPH_PRICE_OSCILLATOR,
	GRAPH_CHARKINS_OSCILLATOR,
	GRAPH_VOLUME_OSCILLATOR,
	GRAPH_SROC,
	GRAPH_MASS_INDEX,
	GRAPH_RMI,
	GRAPH_DRF,								// Daily Raw Figure
	GRAPH_INTEREST,							// �ݸ�
	GRAPH_NEWS_INTENSITY,					// ���� ����
	GRAPH_NEWS_COUNT,						// ���� �Ǽ�
	GRAPH_AVOLS,							// �ð��Ѿ�( Aggregate Value Of Listed Stock ) 
	GRAPH_DEBIT_BALANCE,					// �ſ��ܰ�( Debit Balance )
	GRAPH_DEPOSIT,							// ��Ź��( Deposit )
	GRAPH_FOREIGNER_RESERVE_QUANTITY,		// �ܱ��� ������
	GRAPH_FOREIGNER_RESERVE_RATIO,			// �ܱ��� ������
	GRAPH_FOREIGNER_EXHAUSTED_RATIO,		// �ܱ��� ������
	GRAPH_INVESTER_NET_BUYER_FUTURES,		// ������ ���ż�(����)
	GRAPH_INVESTER_NET_BUYER_CALL_OPTION,	// ������ ���ż�(�ݿɼ�)
	GRAPH_INVESTER_NET_BUYER_KOSDAQ,		// ������ ���ż�(KOSDAQ)
	GRAPH_INVESTER_NET_BUYER_KOSPI,			// ������ ���ż�(KOSPI)
	GRAPH_INVESTER_NET_BUYER_PUT_OPTION,	// ������ ���ż�(ǲ�ɼ�)
	GRAPH_SELLBYHORIZONTALBAR,				//�ż��ŵ����Ź�
	GRAPH_ENERGY,							// Energy
	GRAPH_CREDIT_BALANCE_RATE,					// �ſ��ܰ���
	GRAPH_CREDIT_GIVING_RATE,					// �ſ������
	GRAPH_CREDIT_BALANCE_QTY,					// �ſ��ܰ����
	GRAPH_CREDIT_BALANCE_VARIATION,				// �ſ��ܰ�����

	GRAPH_COUNT
};

// Image
static const int ROOT = 0;
static const int BLOCK = 1;
static const int VERT_SCALE = 2;
static const int GRAPH = 3;
static const int CHILD = 4;

// ����(05/08/09)------- ��ȭ��ǥ��.
static const int HANHWA_INDICATOR_COUNT = 90;
static const _TCHAR* pchIndicatorList[HANHWA_INDICATOR_COUNT] =
{
	"�׹���", "�Ź���Ʈ", "�ϸ����ǥ", "ǥ�ؿ������", "ǥ�ؿ���ä��",
	"ǥ������ä��", "Bollinger Bands", "Box Chart", "DEMA", "Demark",
	"Envelope", "MAC", "Parabolic SAR", "Pivot", "Price Channel",
	"Starc Bands", "TEMA", "Zig Zag", "AD Line", "ADX",//20
	"ADXR", "Aroon", "CCI", "Formula", "LRS",
	"MACD", "MACD Oscillator", "MAO", "Momentum", "NDI",
	"Price Oscillator", "ROC", "Sonar", "SROC", "TDI",
	"TRIX", "VHF", "%R", "�̰ݵ�(�ܼ�)", "�̰ݵ�(����)",//40
	"AB Ratio", "ATR", "Band Width", "Band %B", "Chaikin's Volatility",
	"DPO", "DRF", "Elder Ray Bear", "Elder Ray Bull", "Mass Index",
	"NCO", "Qstic", "RMI", "RSI", "Sigma",
	"Standard Deviation", "Standard Error", "Stochastic Fast", "Stochastic Slow", "Chaikin's Oscillator",//60
	"CMF", "EOM", "Force Index", "MFI", "NVI",
	"OBV", "PVI", "PVT", "VA Oscillator", "Volume Oscillator",
	"Volume Ratio", "VROC", "%B+Stochastic", "LRS+Stochastic", "MACD+Stochastic",
	"Momentum+Stochastic", "OBV+Momentum", "OBV+Stochastic", "ROC+Stochastic", "RSI+MACD",	// 80
	"RSI+Stochastic", "Sonar+�ɸ���", "Stochastic+RSI", "�ɸ���", "Binary Wave",
	"7Binary Wave", "ADL", "ADR", "���� �̵����", "�ŷ��� �̵����" //90
};

static const int SUBGRAPHCOUNT1 = 28;
static const _TCHAR* pchSubGraph1[SUBGRAPHCOUNT1] =
{
	"�׹���", "�Ź���Ʈ", "Box Chart", "DEMA", "Parabolic SAR",
	"TEMA", "Zig Zag", "Formula", "MACD Oscillator", "MAO",
	"VHF", "�̰ݵ�(�ܼ�)", "�̰ݵ�(����)", "Band Width", "Elder Ray Bear",
	"Elder Ray Bull", "RMI", "Sigma", "Standard Deviation", "Standard Error",	// 20
	"CMF", "Volume Ratio", "VROC", "Sonar+�ɸ���", "�ɸ���",
	"Binary Wave", "ADL", "ADR",
};

static const int SUBGRAPHCOUNT2 = 49;
static const _TCHAR* pchSubGraph2[SUBGRAPHCOUNT2] =
{
	"AD Line", "ADXR", "Aroon", "CCI", "LRS",
	"MACD", "Momentum", "NDI", "Price Oscillator", "ROC",	// 10
	"Sonar", "SROC", "TRIX", "%R", "AB Ratio",
	"ATR", "Band %B", "Chaikin's Volatility", "DPO", "Price Channel",	// 20
	"Mass Index", "NCO", "Qstic", "RSI", "Stochastic Fast",
	"Stochastic Slow", "Chaikin's Oscillator", "EOM", "Force Index", "MFI",	// 30
	"NVI", "OBV", "PVI", "PVT", "VA Oscillator",
	"Volume Oscillator", "%B+Stochastic", "LRS+Stochastic", "MACD+Stochastic", "Momentum+Stochastic",	// 40
	"OBV+Momentum", "OBV+Stochastic", "ROC+Stochastic", "RSI+MACD", "RSI+Stochastic",
	"Stochastic+RSI", "7Binary Wave", "DRF", "Demark"
};
	
static const int SUBGRAPHCOUNT3 = 9;
static const _TCHAR* pchSubGraph3[SUBGRAPHCOUNT3] =
{
	"ǥ�ؿ������", "ǥ�ؿ���ä��", "ǥ������ä��", "Bollinger Bands", "�����ڼ��ż�(Put)",
	"Envelope", "Starc Bands", "ADX", "TDI"
};

static const int SUBGRAPHCOUNT4 = 1;
static const _TCHAR* pchSubGraph4[SUBGRAPHCOUNT4] =
{
	"MAC"
};

static const int SUBGRAPHCOUNT5 = 2;
static const _TCHAR* pchSubGraph5[SUBGRAPHCOUNT5] =
{
	"�ϸ����ǥ", "Pivot"
};

#endif

