// #include "./inc/MasterDataSub.h"
//
#if !defined(AFX_MASTERDATASUB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_MASTERDATASUB_BY_JUNOK_LEE__INCLUDED_


// 멀티차트에서의 정의내용. MULTICHART\StdAfx.h
class CMasterTypeInfo
{
public:
	//MultiChart의 StaAfx.h에 정의된 것과 동일하게.
	enum {
		NONE_CHART= 0xFF,
		STOCK_CHART=0,		//KOSPI,KOSDAQ,ELW
		UPJONG_CHART=1,		//UP
		FUTOPT_CHART,		//선물/옵션
		STOCKFUTOPT_CHART,	//주식선물/주식옵션
		FOREIGN_CHART,		//해외
		FUOPT_JIPYO_CHART,	//선물옵션지표
		COMMODITY_CHART,	//상품선물
		MARKET_CHART,		//시장지표
		FO_FOREIGN_CHART,	//해외선물
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

	//MultiChart의 StaAfx.h에 정의된 것과 동일하게.
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

//	Winix 상품선물 관련 종목 구분(Code Type)
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
// 신국채 추가
#define NEW_THREE_BONDS 912
#define NEW_FIVE_BONDS  913
#define NEW_TEN_BONDS   914 
//-------------------------------------------

#define DEF_CONTINUE_FUT_CODE			"99999999"	//연결선물.
#define DEF_CONTINUE_MINI_FUT_CODE		"10599999"	//미니연결선물
#define SZDEF_CONTINUE_FUT_CODE			"연결선물"
#define SZDEF_CONTINUE_MINI_FUT_CODE	"미니 연결선물"

//정의된 Master이름들.
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
