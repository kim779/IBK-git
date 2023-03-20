#ifndef __STCONFIG_DEF__BYGM0604
#define __STCONFIG_DEF__BYGM0604

// tab label
#define BASIC		"Basic"
#define INDEX		"Index"
#define SIGNAL		"Signal"
#define VARIABLES	"Variables"
#define EXTRA		"Extra"

// basic
#define	EXPLANATION	"Explanation"
#define LANGTYPE		"LangType"

// index
#define INDEXNAME		"Name"
#define INDEXTYPE		"Type"
#define INDEXCOLOR		"Color"
#define INDEXSHAPE		"Shape"
#define INDEXTHICK		"Thick"

#define COLORBULL		"ClrBull"
#define COLORBEAR		"ClrBear"

#define	STRLIST_SIGNAL	{"��", "��", "��", "��", "��", "��"}
#define	STRLIST_CLEAR	{"��", "��", "��", "��", "��", "��", "��", "��"}

// signal
#define USEBUY			"UseBuy"
#define USESELL			"UseSell"
#define USEEXITLONG		"UseExitlong"
#define USEEXITSHORT	"UseExitshort"
#define USEBULL			"UseBull"
#define USEBEAR			"UseBear"
#define SHAPEBUY		"ShapeBuy"
#define COLORBUY		"ClrBuy"
#define SHAPEEXITLONG	"ShapeExitlong"
#define COLOREXITLONG	"ClrExitlong"
#define SHAPESELL		"ShapeSell"
#define COLORSELL		"ClrSell"
#define SHAPEEXITSHORT	"ShapeExitshort"
#define COLOREXITSHORT	"ClrExitshort"
#define SHOWSIGNALNAME	"ShowSignalName"
#define SHOWSIGNALQTY	"ShowSignalQty"
#define POSOFSIGNAL		"PosOfSignal"

// variables
#define INPUTCNT		"InputCnt"
#define INPUTVALUE		"Input"
#define INPUTNAME		"InputName"
#define USEEXT			"UseExt"
#define USEEXTDATA		"UseExtData"

// TRADINGOPTION
//#define FIRSTSIGNALEQUITY	"FirstSignalEquity"		// "0":Buy, "1":ExitLong
//#define FIRSTSIGNALFUTURE	"FirstSignalFuture"		// "0":Buy, "1":Sell, "2":All
//#define ORDERPRICE		"OrderPrice"				// "0":CurrentPrice, "1":��1Bid, "2":��2Bid, "3":��3Bid, "4":��4Bid, "5":��5Bid, "6":��10Bid, "7":MarketPrice, "8":Upper/Lower Price
//#define EXITPRICE		"ExitPrice"					// "0":CurrentPrice, "1":��1Bid, "2":��2Bid, "3":��3Bid, "4":��4Bid, "5":��5Bid, "6":��10Bid, "7":MarketPrice, "8":Upper/Lower Price
//#define ORDERTIME		"OrderTime"					// "0":At Completing Candle ,"1":At Eventing Signal
//#define ENABLECONTINUTY	"EnableContinuty"			// "0":Disable, "1":Enable
//#define MAXCONTINUTY	"MaxContinuty"				// Max count to allow same deal position continuely
//#define MINIMUMCHANGE	"MiniumChange"				// "0":Disable, "1":Enable
//#define MINIMUMEQUITY	"MiniumEquity"				// Minium Change Value to operate At Equity
//#define MINIMUMFUTURE	"MiniumFuture"				// Minium Change Value to operate At Future/Option


#define USEPRIFITTARGETRATE		"UseProfitTargetRate"
#define VALPRIFITTARGETRATE		"ValProfitTargetRate"
#define USEPROFITREVERSERATE	"UseProfitReverseRate"
#define VALPROFITREVERSERATE	"ValProfitReverseRate"
#define USEDAMAGELIMITATIONRATE	"UseDamageLimitationRate"
#define VALDAMAGELIMITATIONRATE	"ValDamageLimitationRate"
#define EXITATUPPERPRICE		"ExitAtUpperPrice"
#define EXITATLOWERPRICE		"ExitAtLowerPrice"
#define EXITATTODAY				"ExitAtToday"
#define VALEXITATTODAY			"ValExitAtToday"
#define FORCEDEXITPRICE			"ForcedExitPrice"
#define FEEFORENTRY				"FeeForEntry"
#define FEEFOREXIT				"FeeForExit"
#define SLIPPAGEFORENTRY		"SlippageForEntry"
#define SLIPPAGEFOREXIT			"SlippageForExit"
#define FEEUNIT					"FeeUnit"
#define SLIPPAGEUNIT			"SlippageUnit"

// strategy
#define TRADINGOPTION	"TradingOption"
#define STRATEGYLIST	"StrategyList"
#define STRATEGYCNT		"Count"
#define STRATEGYPOS		"Pos"
#define STRATEGYPOSNAME	"PosName"
#define STRATEGYPOSPATH	"PosPath"
#define STRATEGYPOSTYPE	"PosType"
#define STRATEGYPOSTIME	"PosTime"
#define STRATEGYPOSTEXT	"PosText"
#define STRATEGYLOGIC	"LogicData"
#define STRATEGYSIGNAL	"StrategySignal"


//---------------------------------------------------------------------------------------
// �������� ��ũ��

#define STCONFIG_SIGNAL		"Signal"		// ��ȣ����
#define STCONFIG_ORDER		"RunJumun"		// �ֹ�����
#define STCONFIG_CLEAR		"RunSetup"		// ����û�꼳��
#define STCONFIG_SETUP		"RunClear"		// ��뼳��

// ��ȣ ����
#define SHAPEBUY			"ShapeBuy"			// �ż�
#define COLORBUY			"ClrBuy"
#define SHAPESELL			"ShapeSell"			// �ŵ�
#define COLORSELL			"ClrSell"
#define SHAPEEXITLONG		"ShapeExitlong"		// �ż�û��
#define COLOREXITLONG		"ClrExitlong"
#define SHAPEEXITSHORT		"ShapeExitshort"	// �ŵ�û��
#define COLOREXITSHORT		"ClrExitshort"

// �ֹ� ����
#define APPLYACCT			"Acct"				// �������
#define ACCTNAME			"AcctName"			// ���¸�
#define ORDERAMOUNT			"JAmount"			// �ֹ�����
#define BUYPRICE			"BuyPrice"			// ���԰���
#define SELLPRICE			"SellPrice"			// û�갡��
#define ORDERMETHOD			"JMethod"			// �ֹ����
#define ORDERTIME			"JTime"				// �ֹ�����
#define ORDERSTARTSIGNAL	"JStartSignal"		// �ֹ����۹�ȣ
#define SOUND_BUY_USE		"Sound_Buy_Use"		// ��ȣ�߻���(���Խ�ȣ) ��뿩��
#define SOUND_BUY_FILE		"Sound_Buy_File"	// ��ȣ�߻���(���Խ�ȣ) ���ϰ��
#define SOUND_SELL_USE		"Sound_Sell_Use"	// ��ȣ�߻���(û���ȣ) ��뿩��
#define SOUND_SELL_FILE		"Sound_Sell_File"	// ��ȣ�߻���(û���ȣ) ���ϰ��

// ����û�� ����
#define CLEARPOINT			"ClearPoint"		// ����û�����
#define USELOSSRATE			"Chk1"				// ������ ��뿩��
#define LOSSRATE			"ED1"				// ������(�սǷ�)
#define LOSSRATEUNIT		"P1"				// ������ ����
#define LOSSRATESHAPE		"S1"				// ������ ��ȣ���
#define LOSSRATECOLOR		"CLR_1"				// ������ ��ȣ����

#define USETARGETBENEFIT	"Chk2"				// ��ǥ���� ��뿩��
#define TARGETBENEFIT		"ED2"				// ��ǥ����
#define TARGETBENEFITUNIT	"P2"				// ��ǥ���� ����
#define TARGETBENEFITSHAPE	"S2"				// ��ǥ���� ��ȣ���
#define TARGETBENEFITCOLOR	"CLR_2"				// ��ǥ���� ��ȣ����

#define USEHIGHPRICE		"Chk3"				// ���� ����� �϶� ��뿩��
#define HIGHPRICE			"ED3"				// ���� ����� �϶�
#define HIGHPRICEUNIT		"P3"				// ���� ����� �϶� ����
#define HIGHPRICESHAPE		"S3"				// ���� ����� �϶� ��ȣ���
#define HIGHPRICECOLOR		"CLR_3"				// ���� ����� �϶� ��ȣ����

//#define USELOWPRICE		"Chk4"				// ���� ������� ��� ��뿩��
//#define LOWPRICE			"ED4"				// ���� ������� ���
//#define LOWPRICEUNIT		"P4"				// ���� ������� ��� ����
//#define LOWPRICESHAPE		"S4"				// ���� ������� ��� ��ȣ���
//#define LOWPRICECOLOR		"CLR_4"				// ���� ������� ��� ��ȣ����

#define USETARGETVALUEPRESERV	"Chk4"			// ��ǥ������ ��뿩��
#define TARGETVALUEPRESERV		"ED4"			// ��ǥ������
#define TARGETVALUEPRESERVUNIT	"P4"			// ��ǥ������ ����
#define TARGETVALUEPRESERVSHAPE	"S4"			// ��ǥ������ ��ȣ���
#define TARGETVALUEPRESERVCOLOR	"CLR_4"			// ��ǥ������ ��ȣ����

#define USEBENEFITPRESERV		"Chk7"			// ���ͺ��� ��뿩��
#define BENEFITPRESERVUP		"ED6"			// ���ͺ��� ���
#define BENEFITPRESERVDN		"ED7"			// ���ͺ��� �϶�
#define BENEFITPRESERVUPUNIT	"P6"			// ���ͺ��� ����(���)
#define BENEFITPRESERVDNUNIT	"P7"			// ���ͺ��� ����(�϶�)
#define BENEFITPRESERVSHAPE		"S5"			// ���ͺ��� ��ȣ���
#define BENEFITPRESERVCOLOR		"CLR_5"			// ���ͺ��� ��ȣ����

#define USEMINIPRICE		"Chk5"				// �ּҰ��ݺ�ȭ ��뿩��
#define MINIPRICE			"ED5"				// �ּҰ��ݺ�ȭ
#define MINIPRICEUNIT		"P5"				// �ּҰ��ݺ�ȭ ����
#define MINIPRICEBONGCOUNT	"Bong"				// �ּҰ��ݺ�ȭ �� ����

#define USETODAYCLEAR		"Chk6"				// ����û�� ��뿩��
#define TODAYCLEARUINT		"TimeA"				// ����/����
#define TODAYCLEARTIME		"ED_Time"			// û��ð�


// ��� ����
#define	ALLOWPOSITION		"Allow"				// ���������
#define USEREPEATBUY		"Repeat"			// �ߺ����� ��뿩��
#define	REPEATBUYCOUNT		"JCount"			// �ߺ����� Ƚ��
#define	TAXBUY				"TaxBuy"			// ������+����(����)
#define	TAXSELL				"TaxSell"			// ������+����(û��)
#define	TAXUNIT				"cbTax"				// ������+����(����)
#define	SLIPIGYBUY			"SlipigyBuy"		// ������+����(����)
#define	SLIPIGYSELL			"SlipigyBuy"		// ������+����(û��)
#define	SLIPIGYUNIT			"cbSlipigy"			// ������+����(����)


// Server AP Info
#define EXECUTE_SYMBOL			"SYMBOL"
#define EXECUTE_ENDDATE			"ENDDATE"
#define EXECUTE_ALLCOUNT		"ALLCOUNT"
#define EXECUTE_TYPE			"TYPE"
#define EXECUTE_TMNUMBER		"TMNUMBER"

#define EXECUTE_DATEPOINTER			"DATE"
#define EXECUTE_OPENPOINTER			"OPEN"
#define EXECUTE_HIGHPOINTER			"HIGH"
#define EXECUTE_LOWPOINTER			"LOW"
#define EXECUTE_CLOSEPOINTER		"CLOSE"
#define EXECUTE_VOLUMEPOINTER		"VOLUME"
#define EXECUTE_TIMETYPE			"TIMETYPE"
#define EXECUTE_PREVDATA			"PREVPRICE2"
// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
#define	EXECUTE_EXTDATA				"EXTDATA"


// SD Mode Info
#define LOAD_MODE		"Mode"
#define MODE_TEST		"Test"
#define MODE_ACTION		"Action"


// define For Variable Optimizer
// BOOL CStrategyWnd::LoadSD(LPCTSTR lpStrategyDllPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate, LPCTSTR lpTimeTypeNum,LPCTSTR lpTypeDealOption)
#define OPT_SELL						"OptSell"
#define OPT_BUY							"OptBuy"
#define OPT_INDEX_CROSSABOVE_BASIS		"OptIndexCrossAboveBasis"
#define OPT_INDEX_CROSSBELOW_BASIS		"OptIndexCrossBelowBasis"
#define OPT_INDEX_CROSSABOVE_ANOTHER	"OptIndexCrossAboveAnother"
#define OPT_INDEX_CROSSBELOW_ANOTHER	"OptIndexCrossBelowAnother"
#define OPT_PRICE_CROSSABOVE_INDEX		"OptPriceCrossAboveIndex"
#define OPT_PRICE_CROSSBELOW_INDEX		"OptPriceCrossBelowIndex"
#define OPT_INDEX_UPWARDREVERSAL		"OptIndexUpwardReversal"
#define OPT_INDEX_DOWNWARDREVERSAL		"OptIndexDownwardReversal"
#define OPT_SELL_OPTION					"OptSellOption"
#define OPT_BUY_OPTION					"OptBuyOption"

#endif //__STCONFIG_DEF__BYGM0604