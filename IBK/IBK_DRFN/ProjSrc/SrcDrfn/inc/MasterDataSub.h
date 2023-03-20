// #include "./inc/MasterDataSub.h"
//
#if !defined(AFX_MASTERDATASUB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_MASTERDATASUB_BY_JUNOK_LEE__INCLUDED_


// ��Ƽ��Ʈ������ ���ǳ���. MULTICHART\StdAfx.h
class CMasterTypeInfo
{
public:
	//MultiChart�� StaAfx.h�� ���ǵ� �Ͱ� �����ϰ�.
	enum {
		NONE_CHART= 0xFF,
		STOCK_CHART=0,		//KOSPI,KOSDAQ,ELW
		UPJONG_CHART=1,		//UP
		FUTOPT_CHART,		//����/�ɼ�
		STOCKFUTOPT_CHART,	//�ֽļ���/�ֽĿɼ�
		FOREIGN_CHART,		//�ؿ�
		FUOPT_JIPYO_CHART,	//�����ɼ���ǥ
		COMMODITY_CHART,	//��ǰ����
		MARKET_CHART,		//������ǥ
		FO_FOREIGN_CHART,	//�ؿܼ���
		FX_CHART,			//FX				//20100310_JS.Kim
		CME_CHART,			//CME				//20110106 alzioyes
	};
};

class CDRCodeTypeInfo
{
public:
// 	enum {
// 		CODE_NONE=-1,
// 		CODE_STOCK_KOSKPI=0,
// 		CODE_STOCK_KOSDAQ,
// 		CODE_STOCK_ELW,
// 		CODE_UPJONG,
// 		CODE_FUTURE,
// 		CODE_OPTION,
// 		CODE_STOCKFUTURE,
// 		CODE_STOCKOPTION,
// 		CODE_FOREIGN,
// 	};

	//MultiChart�� StaAfx.h�� ���ǵ� �Ͱ� �����ϰ�.
	enum { 
		CODE_UNKNOWN = 0xFF,
		CODE_KOSPI_STOCK=0,
		CODE_KOSDAQ_STOCK, 
		CODE_ELW, 
		CODE_KOSPI_UPJONG, 
		CODE_KOSDAQ_UPJONG, 
		CODE_KFUTURE, 
		CODE_KOPTION, 
		CODE_FUTURE_SPREAD, 
		CODE_STOCKFUT, 
		CODE_STOCKOPT, 
		CODE_US_ITEM,
		CODE_FUOPT_JIPYO,
		CODE_FUOPT_COMMODITY,
		CODE_FX,
	};
};

//	Winix ��ǰ���� ���� ���� ����(Code Type)
#define THREE_BONDS		900
#define FIVE_BONDS		901
#define TEN_BONDS		902
#define CD_INTEREST		903
#define MSR_FUTURE		904
#define USD_FUTURE		905
#define USD_OPTION		906
#define JPY_FUTURE		907
#define EUR_FUTURE		908
#define GOLD_FUTURE		909
#define PIG_FUTURE		910 
#define MGOLD_FUTURE	911
// �ű�ä �߰�
#define NEW_THREE_BONDS 912
#define NEW_FIVE_BONDS  913
#define NEW_TEN_BONDS   914 
//-------------------------------------------

#define DEF_CONTINUE_FUT_CODE			"99999999"	//���ἱ��.
#define DEF_CONTINUE_MINI_FUT_CODE		"10599999"	//�̴Ͽ��ἱ��
#define SZDEF_CONTINUE_FUT_CODE			"���ἱ��"
#define SZDEF_CONTINUE_MINI_FUT_CODE	"�̴� ���ἱ��"

//���ǵ� Master�̸���.
#define		DRFNMASTER_UMASTER					"UMaster"
#define		DRFNMASTER_KUMASTER					"kumASTER"
#define		DRFNMASTER_TMMASTER					"TMMASTER"
#define		DRFNMASTER_KRXUMASTER				"KRXUMaster"
#define		DRFNMASTER_K200UMASTER				"K200UMaster"
#define		DRFNMASTER_K100UMASTER				"K100UMaster"
#define		DRFNMASTER_GRPMASTER				"GRPMASTER"
#define		DRFNMASTER_JFMASTER					"JFMASTER"
#define		DRFNMASTER_JPMASTER					"JPMASTER"
#define		DRFNMASTER_ELWINDEXMASTER			"ELWINDEXMASTER"
#define		DRFNMASTER_ELWISSUEMASTER			"ELWISSUEMASTER"
#define		DRFNMASTER_OLDFUTUREMASTER			"OLDFUTUREMASTER"

#endif //AFX_MASTERDATASUB_BY_JUNOK_LEE__INCLUDED_
