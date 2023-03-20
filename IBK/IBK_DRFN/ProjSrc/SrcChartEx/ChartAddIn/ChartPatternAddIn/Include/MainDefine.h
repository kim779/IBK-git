#ifndef __GRIDCTRL_H__DEF
#define __GRIDCTRL_H__DEF



// Not Use ============================================================================>>
// �ڽ���, �ڽ���
#define		KOSPI					1				// �ڽ���(�ŷ���)
#define		KOSDAQ					2				// �ڽ���
#define		ETF_KOSPI				7				// �ŷ��� ����
#define		ETF_KOSDAQ				8				// �ڽ��� ����
#define		MARKET_PRICE			99999999		// ���尡

// �׸��� ��, Į�� ����		
#define		LEFT_CAN_COL				0					// ���� ���
#define		MAEDO_COL					1					// �ŵ�
#define		MAEDO_CHJAN_COL				2					// �ŵ� �����ܷ�
#define		MAEDO_JAN_COL				3					// �ŵ� �ܷ�
#define		HOGA_COL					4					// ȣ��

#define		DAEBI_COL					5					// ���
#define		MAESU_JAN_COL				6					// �ż� �ܷ�
#define		MAESU_CHJAN_COL				7					// �ż� �����ܷ�
#define		MAESU_COL					8					// �ż�
#define		RIGHT_CAN_COL				9					// ���� ���
// Not Use <<============================================================================


// Wave Pattern Analysis Dialog 
#define		ROW_FIRST						0				// �� ó�� 
#define		ROW_END							20				// �� ������
#define		ROW_HEIGHT						16				// �� ����
#define		ROW_FIRSTDATA					2



#define		DEFAULT_ROW_CNT_WAV1			2
#define		DEFAULT_COL_CNT_WAV1			8

#define		COL_WAV1_NUM					0
#define		COL_WAV1_TIME					1
#define		COL_WAV1_TYPE					2
#define		COL_WAV1_NAME					3
#define		COL_WAV1_CANDLECNT				4
#define		COL_WAV1_EXPUPDOWN_PER			5
#define		COL_WAV1_ACTUPDOWN_PER			6
#define		COL_WAV1_ACHIEVE_PER			7

#define		COL_WIDTH_WAV1_NUM				22
// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
//#define		COL_WIDTH_WAV1_TIME				67
#define		COL_WIDTH_WAV1_TIME				87
// 2011.01.06 by SYS <<
// 2008.02.20 by LYH >>
//#define		COL_WIDTH_WAV1_TYPE				65
//#define		COL_WIDTH_WAV1_NAME				80
// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
//#define		COL_WIDTH_WAV1_TYPE				90
#define		COL_WIDTH_WAV1_TYPE				70
// 2011.01.06 by SYS <<
#define		COL_WIDTH_WAV1_NAME				100
// 2008.02.20 by LYH <<

#define		COL_WIDTH_WAV1_CANDLECNT		60
#define		COL_WIDTH_WAV1_EXPUPDOWN_PER	60
#define		COL_WIDTH_WAV1_ACTUPDOWN_PER	60
#define		COL_WIDTH_WAV1_ACHIEVE_PER		60


// Candle Pattern Analysis Dialog
#define		DEFAULT_ROW_CNT_CND1			2
#define		DEFAULT_COL_CNT_CND1			9

#define		COL_CND1_NUM					0
#define		COL_CND1_TIME					1
#define		COL_CND1_TYPE					2
#define		COL_CND1_NAME					3
#define		COL_CND1_DAY1					4
#define		COL_CND1_DAY2					5
#define		COL_CND1_DAY3					6
#define		COL_CND1_DAY4					7
#define		COL_CND1_DAY5					8

#define		COL_WIDTH_CND1_NUM				22 
// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
//#define		COL_WIDTH_CND1_TIME				67 
#define		COL_WIDTH_CND1_TIME				87 
// 2011.01.06 by SYS <<
// 2008.02.20 by LYH >>
//#define		COL_WIDTH_CND1_TYPE				65 
//#define		COL_WIDTH_CND1_NAME				80
// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
//#define		COL_WIDTH_CND1_TYPE				90 
#define		COL_WIDTH_CND1_TYPE				70 
// 2011.01.06 by SYS <<
#define		COL_WIDTH_CND1_NAME				100
// 2008.02.20 by LYH >>
#define		COL_WIDTH_CND1_DAY1				48 
#define		COL_WIDTH_CND1_DAY2				48 
#define		COL_WIDTH_CND1_DAY3				48 
#define		COL_WIDTH_CND1_DAY4				48 
#define		COL_WIDTH_CND1_DAY5				48 



// Pattern Report Type	
#define		WAVE_REPORTTYPE1				1001
#define		WAVE_REPORTTYPE2				1002
#define		CANDLE_REPORTTYPE1				2001
#define		CANDLE_REPORTTYPE2				2002
#define		CANDLE_REPORTTYPE3				2003	




#endif // __GRIDCTRL_H__DEF