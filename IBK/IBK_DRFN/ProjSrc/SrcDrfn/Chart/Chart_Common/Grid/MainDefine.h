
// �ڽ���, �ڽ���
#define		KOSPI					1				// �ڽ���(�ŷ���)
#define		KOSDAQ					2				// �ڽ���
#define		ETF_KOSPI				7				// �ŷ��� ����
#define		ETF_KOSDAQ				8				// �ڽ��� ����
#define		MARKET_PRICE			99999999		// ���尡

// ���� ��ư
#define		SPIN_RANGE_START			0		
#define		SPIN_RANGE_END				9999999


// ȣ�� �� ��� ����
#define		NOT_FOUND			-1
#define		WITHIN_HOGA			0
#define		BIG_HOGA			1
#define		SMALL_HOGA			2
#define		EQUAL_HOGA			3


// Context �޴�  ID
#define		MY_CONTEXTMENU_BASE		30100
#define		MY_CONTEXTMENU_END		30200

// ȭ���ȣ

#define		SCRNO_STOPORDER				"8700"		// ���� ���� 
#define		SCRNO_MULTICHART			"8720"		// ��Ƽ íƮ 

//#define		SCRNO_49800				"49800"
//#define		SCRNO_90000				"90000"		// ��Ƽ íƮ 

//#define		SCRNO_10020				"10020"
//#define		SCRNO_49810				"49810"
//#define		SCRNO_90010				"90010"
//#define		SCRNO_90200				"90200"


// �׸��� ��, Į�� ����		
// JSJ_ITCastle_Modify_040427
// ��, �� ����
#define		TENHOGA_ROW_COUNT			29					// 10ȣ���� �� ����

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/6
// Reason		: ��ȭ����� �����ֹ� �׸� �ż� �ŵ� �����Ͽ� �и���Ų��.
//-----------------------------------------------------------------------------
// start
//#define	TOTAL_COLUMN_COUNT			10					// ��ü �� ����
#define		TOTAL_COLUMN_COUNT			11					// ��ü �� ���� + 1 �� �߰�
// end
#define		FIXED_ROW_CNT_TEN			1					// 10ȣ�� ������
#define		FIXED_ROW_CNT_FULL			3					// ��üȣ�� ������
#define		INFO_BTM_ROW_CNT			3					// �ϴ� Sum Info
#define		FIXED_BTM_ROW_CNT			4					// �ϴ� ������

#define		TENHOGA_SAVED_CNT			24					// 10ȣ�� �׸��忡�� ȣ������ ����

// Į�� ���� -------------------------------------------------------------->
//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/6
// Reason		: �����ֹ� �׸� �߰� �Կ� ���� col �ε��� ����
//-----------------------------------------------------------------------------
// start
#define		MAEDO_RESERVE_COL			0					// �ŵ��ֹ�����
#define		LEFT_CAN_COL				1					// ���� ���
#define		MAEDO_COL					2					// �ŵ�
#define		MAEDO_JAN_COL				3					// �ŵ� �ܷ�
#define		HOGA_COL					4					// ȣ��
#define		DAEBI_COL					5					// ���
#define		MAESU_JAN_COL				6					// �ż� �ܷ�
#define		MAESU_COL					7					// �ż�
#define		RIGHT_CAN_COL				8					// ���� ���
#define		MAESU_RESERVE_COL			9					// �ż��ֹ�����
#define		BONG_COL					10					// ��
// end
// <------------------------------------------------------------------------

// Į�� ���� ---------------------------------------------------------------->
#define		CANCEL_COL_WIDTH			45					// ���
#define		MAEMAE_COL_WIDTH			50					// �ŵ�, �ż� �ֹ�
#define		JAN_COL_WIDTH				110					// �ŵ�, �ż� �ܷ�
#define		JAN_COL_WIDTH_SH			74					// �ŵ�, �ż� �ܷ�(�������)
#define		HOGA_COL_WIDTH				70					// ȣ��
#define		HOGA_COL_WIDTH_SH			74					// ȣ��(�������)
#define		DAEBI_COL_WIDTH				50					// ���
#define		RESERVE_COL_WIDTH			45					// �ֹ�����
#define		BONG_COL_WIDTH				0					// ��

#define		MAEMAE_COL_WIDTH_SB			50					// �ŵ�, �ż� �ֹ�(��ũ��)
#define		JAN_COL_WIDTH_SB			105					// �ŵ�, �ż� �ܷ�(��ũ��)
#define		HOGA_COL_WIDTH_SB			74					// ȣ��(��ũ��)
#define		HOGA_COL_WIDTH_SH_SB		70					// ȣ��(�������, ��ũ��)
// <--------------------------------------------------------------------------

#define		TENHOGA_ROW_FIRST			0					// ó��
#define		TENHOGA_ROW_5HOSUM			TENHOGA_ROW_COUNT-3	// 5ȣ�������� Į���� ��
#define		TENHOGA_ROW_TOTSUM			TENHOGA_ROW_COUNT-2	// ��ü Į���� ��
#define		TENHOGA_ROW_EXTRAVOL		TENHOGA_ROW_COUNT-1	// �ð����ܷ�

#define		MARKETPRICE_ROW				1					// ���尡
#define		ETC_HOGAUP_ROW				3					// ��Ÿ ȣ�� ���
#define		ETC_HOGADOWN_ROW			TENHOGA_ROW_COUNT-5	// ��Ÿ ȣ�� �ϴ�

#define		HIGH_ROW					2					// ���Ѱ�
#define		LOW_ROW						TENHOGA_ROW_COUNT-4	// ���Ѱ�

#define		MAEDO_ROW					4					// �ŵ�ȣ�� ����
#define		PRICE_ROW					MAEDO_ROW+10		// ���簡 ��
#define		MAESU_ROW					PRICE_ROW			// �ż�ȣ�� ����

#define		ROW_HEIGHT					14					// �� ����
#define		ROW_WIDTH					80					// �� ����
#define		HEIGHT_OFFSET				0					// ���簡 ���� ���� Offset


// Bong Graph
#define		BONGGRAPH_WIDTH				13					// �� �׷��� ��
#define		BONGGRAPH_LEFT_OFFSET		3					// ���� �������κ��� 3�ȼ�
#define		BONGGRAPH_PEN_WIDTH			1


// �ֹ�����
#define		RESERVED_GRID				89490				// �׸��� ���̵� ��

#define		RSVDGRID_COL_CNT			3					// �׸��� Į����
#define		RSVDGRID_ROW_CNT			9					// �׸��� ����
#define		RSVDGRID_FIXED_ROW			1					// �׸��� ������ ��

#define		RSVDGRID_MAEDO_COL			0					// �ŵ�
#define		RSVDGRID_PRICE_COL			1					// Title
#define		RSVDGRID_MAESU_COL			2					// �ż�

#define		RSVDGRID_ROW_HEIGHT			15
#define		RSVDGRID_COL_WIDTH			70
#define		RSVDGRID_PRICE_COL_WIDTH	85


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/10
// Reason		: ü�Ḯ��Ʈ ���� �߰��� 
//-----------------------------------------------------------------------------
// start
// ü�Ḯ��Ʈ Dlg
#define		ROW_FIRST					0					// �� ó�� 

#define		CHE_ROW_CNT					1
#define		CHE_COL_CNT					2

#define		CHE_PRICE_COL				0
#define		CHE_VOL_COL					1

#define		CHEGYUL_DLG_ROW_FIRST		MAESU_ROW			// ü�Ḯ��Ʈ Dlg ������
#define		CHEGYUL_DLG_ROW_END			LOW_ROW				// ü�Ḯ��Ʈ Dlg ��������
#define		INFO_ROW_FIRST				HIGH_ROW			// ����ǥ��â ������
#define		INFO_ROW_END				PRICE_ROW-1			// ����ǥ��â ��������
// end