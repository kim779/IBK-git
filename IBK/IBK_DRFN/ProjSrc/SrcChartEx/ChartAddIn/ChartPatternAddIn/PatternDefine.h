#ifndef __PATTERN_H__DEF
#define __PATTERN_H__DEF



// Total Pattern Count
#define TOT_PATTERN_CNT				20


// Init varialbe value with Zero
#define ZERO						0

#define CANDLE_HIGHPT				0x0001		// High Point at Candle
#define CANDLE_LOWPT				0x0002		// Low Point at Candle


// Changing Point Type
#define CHANGINGPT_HIGHTOLOW		1001		// �ֿ����
#define CHANGINGPT_LOWTOHIGH		1002		// �ֿ�����

// Regist or Support Line Type
#define LINE_HIGHREGIST				2001		// ���׼�
#define LINE_LOWSUPPORT				2002		// ������


// Pattern Trend
#define UPWARD_REVERSE				3001		// Pattern Upward Reverse
#define DNWARD_REVERSE				3002		// Pattern Downward Reverse
#define UPWARD_CONTINUE				3003		// Pattern Upward Continue
#define	DNWARD_CONTINUE				3004		// Pattern Downward Continue

// Pattern Trust
#define UPWARD_LEVEL3				1			// Pattern Upward Level3
#define UPWARD_LEVEL2				2			// Pattern Upward Level2
#define UPWARD_LEVEL1				3			// Pattern Upward Level1
#define	DNWARD_LEVEL3				4			// Pattern Downward Level3
#define	DNWARD_LEVEL2				5			// Pattern Downward Level2
#define	DNWARD_LEVEL1				6			// Pattern Downward Level1

// 2008.02.20 by LYH >>
// Pattern Trend Name
//#define STR_UPWARD_REVERSE			"��¹�����"				// Pattern Upward Reverse
//#define STR_DNWARD_REVERSE			"�϶�������"				// Pattern Downward Reverse
//#define STR_UPWARD_CONTINUE			"���������"				// Pattern Upward Continue
//#define	STR_DNWARD_CONTINUE			"�϶�������"				// Pattern Downward Continue
// 2008.02.20 by LYH <<



// Pattern Type
#define PTN_HEADNSHOULDER			4001		// Head & Shoulder(�Ӹ������)
#define PTN_R_HEADNSHOULDER			4002		// Reversal Head & Shoulder(���Ӹ������)

#define PTN_DOUBLETOP				4003		// Double Top(�ֺ���)
#define PTN_R_DOUBLETOP				4004		// Reversal Double Top(���ֺ���)

#define PTN_BULL_ISOTRIANGLE		4005		// Bull Isosceles Triangles(�����̵�ﰢ��)
#define PTN_BEAR_ISOTRIANGLE		4006		// Bear Isosceles Triangles(�༼�̵�ﰢ��)

#define PTN_BULL_TRIANGLE			4007		// Bull Triangles(��»ﰢ��)
#define PTN_BEAR_TRIANGLE			4008		// Bear Triangles(�϶��ﰢ��)

#define PTN_BULLISH_WIDETRIANGLE	4009		// Bullish Wide Triangles(���Ȯ��ﰢ��)
#define PTN_BEARISH_WIDETRIANGLE	4010		// Bearish Wide Triangles(�϶�Ȯ��ﰢ��)

#define PTN_BULL_WEDGE				4011		// Bull Wedge(���������)
#define PTN_BEAR_WEDGE				4012		// Bear Wedge(���������)

#define PTN_BULL_SQUAREFLAG			4013		// Bull Square Flag(��»簢�����)
#define PTN_BEAR_SQUAREFLAG			4014		// Bear Square Flag(�϶��簢�����)

#define PTN_BULL_TRIANGLEFLAG		4015		// Bull Triangle Flag(��»ﰢ�����)
#define PTN_BEAR_TRIANGLEFLAG		4016		// Bear Triangle Flag(�϶��ﰢ�����)

#define PTN_BULL_RECTANGLE			4017		// Bull Rectangle(������簢��)
#define PTN_BEAR_RECTANGLE			4018		// Bear Rectangle(�϶����簢��)

#define PTN_V_TOP					4019		// V Top(�ܺ�õ����)
#define PTN_V_BOTTOM				4020		// V Bottom(�ܺ��ٴ���)

// 2008.02.20 by LYH >>
// Pattern Name
//#define STR_HEADNSHOULDER			"�Ӹ������"				//"Head & Shoulder"
//#define STR_R_HEADNSHOULDER			"���Ӹ������"				//"Reversal Head & Shoulder"
//
//#define STR_DOUBLETOP				"�ֺ���"					//"Double Top"
//#define STR_R_DOUBLETOP				"���ֺ���"					//"Reversal Double Top"
//
//#define STR_BULL_ISOTRIANGLE		"�����̵�ﰢ��"			//"Bull Isosceles Triangles"
//#define STR_BEAR_ISOTRIANGLE		"�༼�̵�ﰢ��"			//"Bear Isosceles Triangles"
//
//#define STR_BULL_TRIANGLE			"��»ﰢ��"				//"Bull Triangles"
//#define STR_BEAR_TRIANGLE			"�϶��ﰢ��"				//"Bear Triangles"
//
//#define STR_BULLISH_WIDETRIANGLE	"���Ȯ��ﰢ��"			//"Bullish Wide Triangles"
//#define STR_BEARISH_WIDETRIANGLE	"�϶�Ȯ��ﰢ��"			//"Bearish Wide Triangles"
//
//#define STR_BULL_WEDGE				"���������"				//"Bull Wedge"	
//#define STR_BEAR_WEDGE				"���������"				//"Bear Wedge"
//
//#define STR_BULL_SQUAREFLAG			"��»簢�����"			//"Bull Square Flag"
//#define STR_BEAR_SQUAREFLAG			"�϶��簢�����"			//"Bear Square Flag"
//
//#define STR_BULL_TRIANGLEFLAG		"��»ﰢ�����"			//"Bull Triangle Flag"
//#define STR_BEAR_TRIANGLEFLAG		"�϶��ﰢ�����"			//"Bear Triangle Flag"
//
//#define STR_BULL_RECTANGLE			"������簢��"				//"Bull Rectangle"
//#define STR_BEAR_RECTANGLE			"�϶����簢��"				//"Bear Rectangle"
//
//#define STR_V_TOP					"�ܺ�õ����"				//"V Top"
//#define STR_V_BOTTOM				"�ܺ��ٴ���"				//"V Bottom"
// 2008.02.20 by LYH <<



// Return Value of a Function which is find Candle Pattern
#define RTN_FEWDATA						-1
#define RTN_NONE						0
#define RTN_FIND						1


// Candle Pattern Type
#define CPTN_HAMMER							8001			// Hammer							(��ġ��)
#define CPTN_HANGINGMAN						8002			// Hanging Man						(������)
#define CPTN_INVERTEDHAMMER					8003			// Inverted Hammer					(����ġ��)
#define CPTN_SHOOTINGSTAR					8004			// Shooting Star					(������)
#define CPTN_BULLISH_BELTHOLDLINE			8005			// Bullish Belt Hold Line			(��»�����)
#define CPTN_BEARISH_BELTHOLDLINE			8006			// Bearish Belt Hold Line			(�϶�������)
#define CPTN_LONG_WHITEBODY					8007			// Long White Body					(�����)
#define CPTN_LONG_BLACKBODY					8008			// Long Black Body					(�������)
#define CPTN_BULLISH_HARAMI					8009			// Bullish Harami					(���������)
#define CPTN_BEARISH_HARAMI					8010			// Bearish Harami					(�϶�������)
#define CPTN_BULLISH_ENGULFING				8011			// Bullish Engulfing				(��������)
#define CPTN_BEARISH_ENGULFING				8012			// Bearish Engulfing				(�϶������)
#define CPTN_BULLISH_COUNTERATTACKLINE		8013			// Bullish Counterattack Line		(��¹ݰ���)
#define CPTN_BEARISH_COUNTERATTACKLINE		8014			// Bearish Counterattack Line		(�϶��ݰ���)
#define CPTN_PIERCINGPATTERN				8015			// Piercing Pattern					(������)
#define CPTN_DARKCLOUDCOVER					8016			// Dark Cloud Cover					(�Ա�����)
#define CPTN_MORNINGSTAR					8017			// Morning Star						(������)
#define CPTN_EVENINGSTAR					8018			// Evening Star						(������)
#define CPTN_MORNINTDOJISTAR				8019			// Morning Doji Star				(���ڻ�����)
#define CPTN_EVENINGDOJISTAR				8020			// Evening Doji Star				(���ڼ�����)
#define CPTN_ABANDONEDBABY_INDOWNTREND		8021			// Abandoned Baby In Downtrend		(��±����)
#define CPTN_ABANDONEDBABY_INUPTREND		8022			// Abandoned Baby In Uptrend		(�϶������)
#define CPTN_THREE_INSIDEUP					8023			// Three Indise Up					(�������Ȯ����)
#define CPTN_THREE_INSIDEDOWN				8024			// Three Indise Down				(�϶�����Ȯ����)
#define CPTN_THREE_OUTSIDEUP				8025			// Three Outside Up					(������Ȯ����)
#define CPTN_THREE_OUTSIDEDOWN				8026			// Three Outside Down				(�϶����Ȯ����)
#define CPTN_UPSIDEGAPTWOCROWS_INUPTREND	8027			// Upside Gap Two Crows In Uptrend	(�����)
#define CPTN_THREE_WHITESOLDIER				8028			// Three White Soldier				(���ﺴ)
#define CPTN_THREE_BLACKCROW				8029			// Three Black Corws				(��ﺴ)
#define CPTN_STALLEDPATTERN					8030			// Stalled Pattern					(������)
#define CPTN_ADVANCEBLOCK					8031			// Advance Block					(�����)
#define CPTN_UPSIDEGAP_TASUKI				8032			// Upside Gap Tasuki				(���Ÿ��Ű����)
#define CPTN_DNSIDEGAP_TASUKI				8033			// Downside Gap Tasuki				(�϶�Ÿ��Ű����)
#define CPTN_MATHOLD						8034			// Mat-Hold							(��Ʈ��)
#define CPTN_BULLISH_TRISTAR				8035			// Bullish Tri Star					(��¼����ں���)
#define CPTN_BEARISH_TRISTAR				8036			// Bearish Tri Star					(�϶������ں���)
#define CPTN_UNIQUETHREERIVERBOTTOM			8037			// Unique Three River Bottom		(��ͻ�õ�ٴ���)
#define CPTN_IDENTICAL_THREECROWS			8038			// Identical Three Crows			(������ﺴ��)
#define CPTN_BULLISH_BREAKAWAY				8039			// Bullish Breakaway				(���Ż����)
#define CPTN_BEARISH_BREAKAWAY				8040			// Bearish Breakaway				(�϶�Ż����)
#define CPTN_TWOCORWS						8041			// Two Crows						(�༼�����)
#define CPTN_THREESTARS_INTHESOUTH			8042			// Three Stars In The South			(���Ｚ��)
#define CPTN_CONCEALINGBABY_SWALLOW			8043			// Concealing Baby Swallow			(�ֱ������߱���)
#define CPTN_STICKSANDWICH					8044			// Stick Sandwich					(���������ġ��)
#define CPTN_BULLISH_KICKING				8045			// Bullish Kicking					(��¹�����)	
#define CPTN_BEARISH_KICKING				8046			// Bearish Kicking					(�϶�������)
#define CPTN_HOMINGPIGEON					8047			// Homing Pigeon					(��������)
#define CPTN_LADDERBOTTOM					8048			// Ladder Bottom					(��ٸ��ٴ���)
#define CPTN_MATCHINGLOW					8049			// Matching Low						(����������)
#define CPTN_BULLISH_SIDEBYSIDE_WHITELINE	8050			// Bullish Side-By-Side White Lines	(��³�������)
#define CPTN_BEARISH_SIDEBYSIDE_WHITELINE	8051			// Bearish Side-By-Side White Lines	(�϶���������)
#define CPTN_RISING_THREEMETHODS			8052			// Rising Three Methods				(��»����)
#define CPTN_FALLING_THREEMETHODS			8053			// Falling Three Methods			(�϶������)
#define CPTN_BULLISH_SEPARATINGLINES		8054			// Bullish Separating Lines			(��°�������)
#define CPTN_BEARISH_SEPARATINGLINES		8055			// Bearish Separating Lines			(�϶���������)
#define CPTN_BULLISH_THREELINESTRIKE		8056			// Bullish Three-Line Strike		(�����Ｑ�ݰ���)
#define CPTN_BEARISH_THREELINESTRIKE		8057			// Bearish Three-Line Strike		(�༼�Ｑ�ݰ���)
#define CPTN_UPSIDEGAP_THREEMETHODS			8058			// Upside Gap Three Methods			(��°������)
#define CPTN_DNSIDEGAP_THREEMETHODS			8059			// Downside Gap Three Methods		(�϶��������)
#define CPTN_ON_NECK						8060			// On Neck							(��ħ��)
#define CPTN_IN_NECK						8061			// In Neck							(������)
#define CPTN_THRUSTINGLINE					8062			// Thrusting Line					(������)
//#define CPTN_BULLISH_GRAVESTONE				8063			// Bullish GraveStone				(���� ����)
//#define CPTN_BULLISH_DRAGONFLY				8064			// Bullish DragonFly				(���� ���ڸ���)

// 2008.02.20 by LYH >>
// Candle Pattern Name
//#define STR_HAMMER							"��ġ��"
//#define STR_HANGINGMAN						"������"
//#define STR_INVERTEDHAMMER					"����ġ��"
//#define STR_SHOOTINGSTAR					"������"
//#define STR_BULLISH_BELTHOLDLINE			"��»�����"
//#define STR_BEARISH_BELTHOLDLINE			"�϶�������"
//#define STR_LONG_WHITEBODY					"�����"
//#define STR_LONG_BLACKBODY					"�������"
//#define STR_BULLISH_HARAMI					"���������"
//#define STR_BEARISH_HARAMI					"�϶�������"
//#define STR_BULLISH_ENGULFING				"��������"
//#define STR_BEARISH_ENGULFING				"�϶������"
//#define STR_BULLISH_COUNTERATTACKLINE		"��¹ݰ���"
//#define STR_BEARISH_COUNTERATTACKLINE		"�϶��ݰ���"
//#define STR_PIERCINGPATTERN					"������"
//#define STR_DARKCLOUDCOVER					"�Ա�����"
//#define STR_MORNINGSTAR						"������"
//#define STR_EVENINGSTAR						"������"
//#define STR_MORNINTDOJISTAR					"���ڻ�����"
//#define STR_EVENINGDOJISTAR					"���ڼ�����"
//#define STR_ABANDONEDBABY_INDOWNTREND		"��±����"
//#define STR_ABANDONEDBABY_INUPTREND			"�϶������"
//#define STR_THREE_INSIDEUP					"�������Ȯ����"
//#define STR_THREE_INSIDEDOWN				"�϶�����Ȯ����"
//#define STR_THREE_OUTSIDEUP					"������Ȯ����"
//#define STR_THREE_OUTSIDEDOWN				"�϶����Ȯ����"
//#define STR_UPSIDEGAPTWOCROWS_INUPTREND		"�����"
//#define STR_THREE_WHITESOLDIER				"���ﺴ"
//#define STR_THREE_BLACKCROW					"��ﺴ"
//#define STR_STALLEDPATTERN					"������"
//#define STR_ADVANCEBLOCK					"�����"
//#define STR_UPSIDEGAP_TASUKI				"���Ÿ��Ű����"
//#define STR_DNSIDEGAP_TASUKI				"�϶�Ÿ��Ű����"
//#define STR_MATHOLD							"��Ʈ��"
//#define STR_BULLISH_TRISTAR					"��¼����ں���"
//#define STR_BEARISH_TRISTAR					"�϶������ں���"
//#define STR_UNIQUETHREERIVERBOTTOM			"��ͻ�õ�ٴ���"
//#define STR_IDENTICAL_THREECROWS			"������ﺴ��"
//#define STR_BULLISH_BREAKAWAY				"���Ż����"
//#define STR_BEARISH_BREAKAWAY				"�϶�Ż����"
//#define STR_TWOCORWS						"�༼�����"
//#define STR_THREESTARS_INTHESOUTH			"���Ｚ��"
//#define STR_CONCEALINGBABY_SWALLOW			"�ֱ������߱���"
//#define STR_STICKSANDWICH					"���������ġ��"
//#define STR_BULLISH_KICKING					"��¹�����"	
//#define STR_BEARISH_KICKING					"�϶�������"
//#define STR_HOMINGPIGEON					"��������"
//#define STR_LADDERBOTTOM					"��ٸ��ٴ���"
//#define STR_MATCHINGLOW						"����������"
//#define STR_BULLISH_SIDEBYSIDE_WHITELINE	"��³�������"
//#define STR_BEARISH_SIDEBYSIDE_WHITELINE	"�϶���������"
//#define STR_RISING_THREEMETHODS				"��»����"
//#define STR_FALLING_THREEMETHODS			"�϶������"
//#define STR_BULLISH_SEPARATINGLINES			"��°�������"
//#define STR_BEARISH_SEPARATINGLINES			"�϶���������"
//#define STR_BULLISH_THREELINESTRIKE			"�����Ｑ�ݰ���"
//#define STR_BEARISH_THREELINESTRIKE			"�༼�Ｑ�ݰ���"
//#define STR_UPSIDEGAP_THREEMETHODS			"��°������"
//#define STR_DNSIDEGAP_THREEMETHODS			"�϶��������"
//#define STR_ON_NECK							"��ħ��"
//#define STR_IN_NECK							"������"
//// #define STR_THRUSTINGLINE					"������"
// 2008.02.20 by LYH >>
#define STR_THRUSTINGLINE					"Thrusting Line"
//#define STR_BULLISH_GRAVESTONE				"��������"
//#define STR_BULLISH_DRAGONFLY				"�������ڸ���"


// Candle Pattern Name
#define STR_HTML_HAMMER							"Hammer"
#define STR_HTML_HANGINGMAN						"HangingMan"
#define STR_HTML_INVERTEDHAMMER					"InvertedHammer"
#define STR_HTML_SHOOTINGSTAR					"Shootingstar"
#define STR_HTML_BULLISH_BELTHOLDLINE			"Bullishbeltholdline"
#define STR_HTML_BEARISH_BELTHOLDLINE			"Bearishbeltholdline"
#define STR_HTML_LONG_WHITEBODY					"LongWhiteBody"
#define STR_HTML_LONG_BLACKBODY					"LongBlackBody"
#define STR_HTML_BULLISH_HARAMI					"BullishHarami"
#define STR_HTML_BEARISH_HARAMI					"BearishHarami"
#define STR_HTML_BULLISH_ENGULFING				"BullishEngulfing"
#define STR_HTML_BEARISH_ENGULFING				"BearishEngulfing"
#define STR_HTML_BULLISH_COUNTERATTACKLINE		"BullishCounterattackline"
#define STR_HTML_BEARISH_COUNTERATTACKLINE		"BearishCounterattackline"
#define STR_HTML_PIERCINGPATTERN				"PiercingPattern"
#define STR_HTML_DARKCLOUDCOVER					"DarkCloudCover"
#define STR_HTML_MORNINGSTAR					"MorningStar"
#define STR_HTML_EVENINGSTAR					"EveningStar"
#define STR_HTML_MORNINTDOJISTAR				"MorningDojiStar"
#define STR_HTML_EVENINGDOJISTAR				"EveningDojiStar"
#define STR_HTML_ABANDONEDBABY_INDOWNTREND		"Abandonedbabyindowntrend"
#define STR_HTML_ABANDONEDBABY_INUPTREND		"Abandonedbabyinuptrend"
#define STR_HTML_THREE_INSIDEUP					"ThreeInsideUp"
#define STR_HTML_THREE_INSIDEDOWN				"ThreeInsidedown"
#define STR_HTML_THREE_OUTSIDEUP				"ThreeOutsideup"
#define STR_HTML_THREE_OUTSIDEDOWN				"ThreeOutsideDown"
#define STR_HTML_UPSIDEGAPTWOCROWS_INUPTREND	"Upsidegaptwocrowsinuptrend"
#define STR_HTML_THREE_WHITESOLDIER				"Threewhitesolider"
#define STR_HTML_THREE_BLACKCROW				"ThreeBlackcrow"
#define STR_HTML_STALLEDPATTERN					"StalledPattern"
#define STR_HTML_ADVANCEBLOCK					"AdvanceBlock"
#define STR_HTML_UPSIDEGAP_TASUKI				"UpsidegapTasuki"
#define STR_HTML_DNSIDEGAP_TASUKI				"DownwardgapTasuki"
#define STR_HTML_MATHOLD						"Mat-hold"
#define STR_HTML_BULLISH_TRISTAR				"BullishTriStar"
#define STR_HTML_BEARISH_TRISTAR				"BearishTriStar"
#define STR_HTML_UNIQUETHREERIVERBOTTOM			"UniqueThreeRiverBottom"
#define STR_HTML_IDENTICAL_THREECROWS			"IdenticalThreeCrows"
#define STR_HTML_BULLISH_BREAKAWAY				"BullishBreakaway"
#define STR_HTML_BEARISH_BREAKAWAY				"BearishBreakaway"
#define STR_HTML_TWOCORWS						"TwoCrows"
#define STR_HTML_THREESTARS_INTHESOUTH			"ThreeStarsintheSouth"
#define STR_HTML_CONCEALINGBABY_SWALLOW			"ConcealingBabySwallow"
#define STR_HTML_STICKSANDWICH					"StickSandwich"
#define STR_HTML_BULLISH_KICKING				"BullishKicking"	
#define STR_HTML_BEARISH_KICKING				"BearishKicking"
#define STR_HTML_HOMINGPIGEON					"HomingPigeon"
#define STR_HTML_LADDERBOTTOM					"LadderBottom"
#define STR_HTML_MATCHINGLOW					"MatchingLow"
#define STR_HTML_BULLISH_SIDEBYSIDE_WHITELINE	"BullishSide-by-SideWL"
#define STR_HTML_BEARISH_SIDEBYSIDE_WHITELINE	"BearishSide-by-SideWL"
#define STR_HTML_RISING_THREEMETHODS			"RisingThreeMethods"
#define STR_HTML_FALLING_THREEMETHODS			"FallingThreeMethods"
#define STR_HTML_BULLISH_SEPARATINGLINES		"BullishSeparatingLines"
#define STR_HTML_BEARISH_SEPARATINGLINES		"BearishSeparatingLines"
#define STR_HTML_BULLISH_THREELINESTRIKE		"BullishThree-LineStrike"
#define STR_HTML_BEARISH_THREELINESTRIKE		"BearishThree-LineStrike"
#define STR_HTML_UPSIDEGAP_THREEMETHODS			"UpsideGapThreeMethods"
#define STR_HTML_DNSIDEGAP_THREEMETHODS			"DownsideGapThreeMethods"
#define STR_HTML_ON_NECK						"OnNeck"
#define STR_HTML_IN_NECK						"InNeck"
#define STR_HTML_THRUSTINGLINE					"ThrustingLine"

#endif // __PATTERN_H__DEF