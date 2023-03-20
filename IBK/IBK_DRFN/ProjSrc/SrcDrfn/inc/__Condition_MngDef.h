#ifndef CCONDITION_MNGDEF
#define CCONDITION_MNGDEF

#include <afxtempl.h>   //<= CList
#define _countof(array) (sizeof(array)/sizeof(array[0]))

/************************************************************/
/*																													*/
/*			Condition Server =>  STRUCT/DEFINE/TYPEDEF ����			*/
/*																													*/
/*					2004. 11. 2 		Doori														*/
/*						�ۼ��� : �쵿��																*/
/************************************************************/
#define ERR_EXPCODE					"E00010"

#define TRNO_CONDITION			"80001"
#define TRNO_WORKDATE				"80003"

#define CODE_LENGTH					7
#define CODENAME_LENGTH			20


#define		MAX_IDXCNT				18	/* �ִ� ��ǥ ����(����)	*/

#define		REAL_IDXCNT				18	/* �ִ� ��ǥ ����(�ܺ�)	*/
#define		MAX_PARAM					3		/* param �� �ִ� ���� 	*/
#define		MAX_CONDI					6		/* condition�� �ִ� ����*/
#define		EXPLEN						80	/* ���ǻ��� �ִ� ���� 	*/

#define		RESULT_MAX_OFFSET	1
#define		RESULT_MAX				2000	/* �ִ� ���� ��         */
#define		RESULT_FIELD_MAX	20		/* ����ʵ� �ִ� ���� 	*/

#define		LANGUAGE_MAX			180000


/********************************************************************/
/*  ���ǰ˻� INPUT DATA                                             */
/********************************************************************/
typedef struct tagUser_Define
{
	long    IndexNo; 				/* ��ǥ index				*/
	long    GroupNo; 				/* �Ҽ� �׷��ȣ			*/
	long    GroupIdxCnt;			/* �׷쳻 ��ǥ ����			*/
	long    TargetBase;				/* 0:�ϰ�, 1:�ְ�, 2:����, 3:1��, 4:3��	*/
									/* 5:5��,  6:10��, 7:15��, 8:30��, 9:60��*/
	long    Type; 					/* ��ǥ�� ����					*/
									/* 00XX	: ����� ��ǥ		*/
									/* 10XX	: ĵ��м�			*/
									/* 20XX	: �ü���ǥ			*/
									/* 30XX	: �繫��ǥ			*/
									/* 40XX	: �⺻�� �м�		*/
									/* 50XX	: �ñ׳� ��ǥ 		*/
									/* 60XX	: ����				*/
									/* 70XX : ���� (new)		*/
									/* 80XX : ���� (new)		*/
	long    Type1; 					/* ���� ����				*/
									/* 0: ���ذ� ����(����/����)*/
									/* 1: ���ؼ� ����(����/����)*/
									/* 2: ���ذ� ��(��) 	*/
									/* 3: ���ؼ� ��(��) 	*/
									/* 4: �� ��(��,<,<=,>,>=,==)*/
									/* 5: ���� ��(c1�̻�~c2����)  */
									/* 6: �߼�(���/�϶�)           */
									/* 7: ����(���/�϶�)           */
									/* 8: �����˻�					*/
									/* 9: �迭(���迭/���迭)       */
									/* 10: n% ���� �����˻�1	    */
									/* 11: n% ���� �����˻�2 	    */
									/* 20: ���� üũ (new)          */
									/* 30: ���� üũ (new)          */
									/* 90: ���ذ� ����(����/����)	##����##*/ 
									/* 91: ���ؼ� ����(����/����)	##����##*/
									/* 92: ���ذ� ��(��)    	##����##*/
									/* 93: ���ؼ� ��(��)    	##����##*/
									/* 94: ����(��,<,<=,>,>=,==)##����##*/
									/* 95: ���� ��(c1�̻�~c2����) ##����##*/
									/* 96: �߼�(���/�϶�)          ##����##*/
									/* 97: ����(���/�϶�)          ##����##*/
									/* 99:��Ÿ1                     ##����##*/
									/* 98:��Ÿ2                     ##����##*/
	long    Type2; 					/* �� ����					*/
									/* 0: ����/�̻�/���/���迭/�ż�*/
									/* 1: ����/����/�϶�/���迭/�ŵ�*/
									/* 2: ���ϱ���(������ǥ)        ##����##*/
									/* 3: ���ϱ���(������ǥ)        ##����##*/
									/* 99:��Ÿ                      */
	long    STerm;					/* n���� ����               	*/
	long    ETerm;					/* n���̳�                 		*/
	long    Rank;					/* �����˻� ���� ��             */
	long    Trend;					/* �߼�Ƚ��						*/
	long		SignalCnt1;				/* �߻�ȸ��	(new)	X������ X�� ����*/
	long		SignalCnt2;				/* �߻�ȸ��	(new)					*/
	long    ResultViewNum;			/* ��������� �ʵ� ����(0:������)	*/    

	char    IndexType;				/* �˻����� 0:�Ϲ� 1:���� 2:����	*/
	char    SearchType;				/* �˻����� 0:�Ϲ� 1:���������	2:���� 3:����   (new) */

	char    Operator;				/* ������'0':OR '1':AND '2':NEXT	*/
	char    Reverse;				/* ������� '0':A, '1':Not A		*/
	char    IdxContiGubun;			/* ��ǥ�� ���ӹ߻� ����
										'0':���Ϲ߻�, 1:���ӹ߻���ǥ	*/
	char    Element;				/* ���ǽ� ��빮��(A) (new)			*/
	char	Code[6];

	double  Param[MAX_PARAM];      	/* ��ǥ���� ������ (ex.12,26,9)		*/
	double  Condition[MAX_CONDI];   /* ��Ÿ ���� ������					*/
} User_Define;
#define SZ_User_Define sizeof(User_Define)

/********************************************************************/
/*  ����� ���� ���ǰ˻� ��Ŷ                                       */
/********************************************************************/
typedef struct tagUser_Finder
{
	long		TrIndex;
	long		TargetType;									  /* 0 :���� , 10 : ����	���� [NEW]*/		
	long    TargetIdxCnt;                	/* ��� ��ǥ ����       */
	long    TargetResultViewCnt;          /* ��� �������ʵ� ���� */
	char    TargetExp[EXPLEN];            /* ������� ���� ǥ�� ��*/
	
	long    ComplexIdxCnt;                /* ���� ��ǥ ����       */
	long    ComplexResultViewCnt;         /* ���� �������ʵ� ���� */
	char    ComplexExp[EXPLEN];           /* �������� ���� ǥ�� ��*/
	
	long    FieldEditerCnt;               /* �ʵ����� ����   [Fix + Move] */
	long    FieldList[REAL_IDXCNT];				/* �ʵ����� ��ȣ����Ʈ [Just Fix + Move] */

	long    ResultIdxCnt;									/* ������� ����         [NEW]*/
	char    ResultExp[EXPLEN];						/* ����� ������ ǥ�� �� [NEW]*/
		
	char    TargetBitChk;                 // TargetBit ��� ���� üũ
	//char		Jongmoklist[RESULT_MAX][CODE_LENGTH];			// ���ɱ׷� �����ڵ�
	char		TargetBit[RESULT_MAX];  /* �˻� ��� ����		*/
	
	char    Sort_flag;                    /* ���� ����(0:����, 1:����)*/
	long    Sort_Num;                     /* ���� ��ȣ(�������ʵ�pos) */
	    
	long    BackDay;											/* backtest ����(�̻��:-1) */	
	long    BackTime;											/* backtest �ð�(�̻��:-1) */

	User_Define    stUser_Define[MAX_IDXCNT];    /* ����� ��ǥ����   */
	
	/*
	int ScriptLength;
	char ScriptData[LANGUAGE_MAX];
	*/
} User_Finder;
#define SZ_User_Finder  sizeof(User_Finder)

/********************************************************************/
/*  Output Packet Struct                                            */
/********************************************************************/
typedef struct tagOutPacket{
	long	lTrIndex;
	long  lTotalJongCnt;				/* �˻��� ���� ���� ��      */
	long  lOutFieldCnt;					/* Out Field Count          */
	long  lOutListPacketSize;		/* ��� List ������(������) */
	long	lFindTime;						/* �˻��ð�					*/
	//char    ResultBit[RESULT_MAX];	    /* ������� bit list*/
} OutPacket;
#define SZ_OutPacket sizeof(OutPacket)

/************************************************************************/
/*  Output List Packet Struct                                           */
/************************************************************************/
typedef struct tagOutListPacket{
	char		chSign;									/* ���ϴ�� ��ȣ  1:���� 2:��� 3:���� 4:���� 5:�϶�*/
	char		chCode[CODE_LENGTH];		/* ���� �ڵ�	 			*/
	char		chName[CODENAME_LENGTH];/* �����		 			*/ 
	char		chMarket;               /* �ŷ���:'1', �ڽ���:'2'	*/
	char		Filler[3];
	long		lOrderRank;							/* �����˻� ���� 			*/	
	double	dFieldData[RESULT_FIELD_MAX];
} OutListPacket;
#define SZ_OutListPacket sizeof(OutListPacket)


/****************************************************************/
/*                                                              */
/*		  Condition Client =>  STRUCT/DEFINE/TYPEDEF ����					*/
/*																															*/
/*                   2005. 12.  		Doori												*/
/*                        �ۼ��� : �쵿��                       */
/****************************************************************/

const		LPCTSTR		CONDITION_INDEX[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I",
												"J", "K", "L", "M", "N", "O", "P", "Q", "R",
												"S", "T", "U", "V", "W", "X", "Y", "Z",
												"a", "b", "c", "d", "e", "f", "g", "h", "i", 
												"j", "k", "l", "m", "n", "o", "p", "q", "r",
												"s", "t",  "u", "w", "x", "y", "z" };


const		int		TARGETINDEX_TYPE1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const		int		TARGETINDEX_TYPE2[] = { 0 };
const		int		TARGETINDEX_TYPE3[] = { 3, 4, 5, 6, 7, 8, 9 };

#define MAX_FIELD_CNT				17		//# ��� Grid Column ���� Max Count


#define GRID_MOVEFIELD_POS_NAME			"[������ ��ġ]"
#define GRID_MOVEFIELD_POS_INDEX		-100

#define MAX_PORTPOLIO_CNT				200
#define XML_VERSION_USERFILE		"2.0"

#define FOLDER_NEWS						"\\Data\\NewsData"
#define NEWS_KEYWORD_FILE			"\\txnews_keyword.dat"

#define FOLDER_DATA						"\\Data\\FindData"
#define FOLDER_MAP						FOLDER_DATA + "\\map" 
#define FOLDER_EXP						FOLDER_DATA + "\\exp" 
#define FOLDER_USERFILE				FOLDER_DATA + "\\user" 
#define FOLDER_SYSFILE				FOLDER_DATA + "\\sys"
#define FOLDER_OTHERFILE			FOLDER_DATA + "\\Other"
#define FOLDER_LANGUAGEFILE		"ChartData\\Search" 

#define FOLDER_TEMPFILE					"�ӽñ׷�"					

#define TREE_COMMON_FILE				"TreeCommon.xml"
#define TREE_COMMON_SORT_FILE		"TreeCommon_sort.xml"
#define TREE_COMMON_MARK_FILE		"TreeCommon_mark.xml"
#define TREE_ORDER_FILE					"TreeOrder.xml"
#define TREE_ORDER_SORT_FILE		"TreeOrder_sort.xml"
#define TREE_ORDER_MARK_FILE		"TreeOrder_mark.xml"
#define TREE_USER_FILE					"TreeUser.xml"
#define TREE_RECOMMEND_FILE			"TreeRecommend.xml"
#define TREE_LANGUAGE_FILE			"TreeLanguage.xml"			//# Not User!

#define QUICK_FILE						  "QuickBtnReg.xml"


#define MOVEFIELD_SYS_FILE			"movefield_sys.xml"
#define MOVEFIELD_USER_FILE			"movefield_user.xml"
#define FIXFIELD_SYS_FILE				"fixfield_sys.xml"
#define FIXFIELD_USER_FILE			"fixfield_user.xml"

#define CANDLE_INDEX_FILE				"candleinfo.xml"
#define PATEN_INDEX_FILE				"patterninfo.xml"

#define FINDERINFO_INIFILE			"FindInfo.ini"
#define LOCALVERSION_INIFILE		"LocalVersion.ini"

#define CODESEARCH_FILE					"CodeSearch.ini"			//# Port Temp File!
#define USERCONNECTMENU_FILE		"UserConnectMenu.ini"


/************************************************************/
/*																													*/
/* ���� ���α׷����� ����ϴ� XML File Saveed Insert ���� */
/*																													*/
/*								2004. 11. 02		Doori(�쵿��)							*/
/************************************************************/


typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;

//### RealFinder Pro 2002. 11 : Danny ###
typedef struct tagElement 
{
	CString   strOperation;
	CString   strNot;
	CString   strElement;
	
	BOOL      bView;
	
	CString   strBracket;
	LONG      lBracketCount;
} OneElementInfo;

typedef struct tagGroupElement
{
	int nGroup;
	OneElementInfo stOneElementInfo;
} GroupElementInfo;


typedef struct tagPortPolio
{
	CString		strOperation;
	int			nType;				//# 0 : PortPolio, 1 : Account
	CString		strPortNumber;
	CString		strPortName;
} PortPolio;

//# Portfolio Save Packet
typedef struct tagPortfolioGroup
{
	CString strCode;			//ex)[���� 05]
	CString strGroupName;	//ex)memo=
	int nElementCnt;			//��������׷� ���� ��ǥ ����
} PortfolioGroup;
#define SZ_PortfolioGroup sizeof(PortfolioGroup)



//# [Port]
#define	PORT_GROUP_MAX								99
#define	PORT_GROUP_ITEM_MAX						40

typedef struct tagPortInfo
{
	int nNo;
	CString strName;

	int nItemCnt;
	int nMarketType[PORT_GROUP_ITEM_MAX];		 //# �ŷ���:'1', �ڽ���:'2'
	CString strCode[PORT_GROUP_ITEM_MAX];
} PortInfo;


#define HEAD_OTHER_CNT      2  //# �߰� �ʵ� ����

typedef struct tagHeadInfoType
{
	long        lHeadID;
	CString     strHeadName;
	int         nUnitNum;
	CString     strUnitText;
	CString     strDataType;
	int         nDataLength;
	int         nColWidth;
	CString     strAlignment;
} HeadInfoType;

typedef struct tagHeadMoveInfo{
	int         nColCnt;    //## Add Col Count
	int			nIndex;			
	CString		strElement; 
	BOOL		bCondition; 
	BOOL		bView;
	HeadInfoType  stHeadType[HEAD_OTHER_CNT];
} HeadMoveInfo;


typedef struct tagHeadFixInfo{
	int           nColNum;        
	HeadInfoType  stHeadType;
	BOOL          bView;
} HeadFixInfo;


const LPCTSTR		MATCHVAL_OPT_LIST[] = { ">", ">=", "=", "<=", "<" };
const LPCTSTR		MATCHVAL_OPT_DATA = ">\n>=\n=\n<=\n<\n";
typedef struct tagMatchInfo
{
	int			nRow;
	BOOL		bUser;
	int			nOptIndex;
	CString		strField;
	CString		strElement;
	int 		nFieldIndex;

	int			nGridSourceRow;			//# Temp
} MatchInfo;


typedef struct tagExplainInfo
{
	int			nType;		//# 0 : Text, 1 :URL
	CString		strName;
	CString		strExplain;
	CString		strURL;
} ExplainInfo;


//# ���� �ϳ��� ���� ���� #
typedef struct tagOneCondition {
	BOOL        bFalg;					//# ���� ���� : TRUE
	
	int				nNotUserFlag;
	int				nGubunUserFlag;			//# ����/�񿬼� 

	int			nTargetBaseType;		//# TargetBase Type (�ϰ�����, �׿� ...)

	CString     strConditionTag;		//# ���� Tag  <= Map Create Tag
	CString     strConditionFileName;	//# ���� File Name

	int			nNonAlert;				//# Deawoo [NEW]
	int			nChartValSet;			//# Deawoo [NEW]

	CString     strConditionTitle;		//# ���Ǹ�
	CString     strConditionText;		//# ���� �ϼ� ����
	
	User_Define   stCondition;			//# condition Info  

//#	User_Script		stUserScript;
//#	CString				strScriptText;

	BOOL          bFieldVal;			//# ������
	BOOL          bFieldState;			//# ������ Check Field Have
	BOOL          bFieldEnable;			//# ������ Enable ���� 
										//=> TRUE : Field values�� ���ų� ������ Not�� ���
										//=> FALSE : Field values�� �ִ� ��� 
	
	HeadMoveInfo  stHeadMoveInfo;		//# ������ �ʵ� ����
} OneConditionInfo;

typedef struct tagConditionGroupInfo{
	int           nGroup;             //## Group
	CString       strElement;         //## Condition Index
	OneConditionInfo  stOneConditionInfo; //## One Condition Info
} ConditionGroupInfo;

typedef struct tagCandleInfo{
	long    lID;
	CString   strName;
} CandleInfo;

typedef struct tagPatenInfo{
	long    lID;
	CString   strName;
} PatenInfo;

typedef struct tagFileInfo
{
	CString   strFolderPath;

	CString   strFilePath;
	CString   strFileName;
	CString   strRegCheck;
} FileInfo;


typedef struct tagQuickList
{
	BOOL		bCheck;
	CString	strType;
	BOOL		bAlertReg;

	CString strFileName;
	CString	strQuickName;
	CString	strFilePath;
	CString	strFolderPath;
} QuickList;
#define SZ_QuickList sizeof(QuickList)


//# [Xml Tree] ###############################################################################
#define TREE_SELECT_TYPE_ITEM			1
#define TREE_SELECT_TYPE_PARENTITEM		2

#define TREE_SELECT_TYPE_USERFILE		3
#define TREE_SELECT_TYPE_SYSFILE		4

#define TREE_SELECT_TYPE_PORTPOLIO		5
#define TREE_SELECT_TYPE_ACCOUNT		6
#define TREE_SELECT_TYPE_LANGUAGE		7
#define TREE_SELECT_TYPE_USERPATTEN		8


#define TAG_USERPATTEN				"P1_1"

typedef struct tagNewsInfo
{
	int nIndex;
	int nCode;
	CString strType;
	CString strstrKeyWord;
} XMLTreeNewsInfo;

typedef struct tagXMLTreeInfo
{
	int nTreeType;			//# Item : Common[0] / Order[1] / Language[4] / User Patten[5]

	CString strName;
	CString strTag;

	int nNonAlert;			//# Deawoo [NEW]
	int nCondiType;
	int nTargetBase_Type;

	int nGubun;
	int nGubunDefVal;

	int nNotUser;
	int nNotDefVal;
		
	CString strResultViewID;
	int nResultViewDefVal;

	int nChartValSet;		//# Deawoo [NEW]

	CString strFileName;
} XMLTreeInfo;


typedef struct tagXMLTreeInfoFile
{
	CString strType;		//# USER, SYS

	CString strFolderPath;
	CString strFilePath;
	CString strFileName;

	BOOL	bQuickReg;		//# Deawoo [NEW]
	BOOL	bUnitCan;		//# Deawoo [NEW]

	BOOL	bAlertReg;

	CString strExplain;
} XMLTreeInfoFile;

typedef struct tagXMLTreeInfoFolder
{
	BOOL	bLock;
	CString strPath;
	CString strName;

	int nFileCnt;
	CList	<XMLTreeInfoFile, XMLTreeInfoFile&> *plistXMLTreeInfoFile;
} XMLTreeInfoFolder;


typedef struct tagInfoFolder
{
	BOOL		bLock;
	CString strPath;
	CString strName;
	int nFileCnt;
	BOOL bTarget;
} InfoFolder;

//# Danny => 2005. 12 [����]
typedef struct tagCodeMaster
{
	int		nType;		// 0:�ֽ�����, 1:�ڽ�������
	CString strUnder;		// 1  ����/��������
	CString strWarn;		// 1  �������� ����
	CString strName;		// 18 �����
	CString strCode;		// 6  �����ڵ�
} CodeMaster;


//# Language!
typedef struct tagLanguageInfo
{
	int	nLanguageType;	// ���� ��� 0 : TS, 1 : YES, 2 : YES
	LPSTR	lpszName;		  // ���ϸ��� Ȯ���� .sts�� ���̸� ��
					            // ��ũ��Ʈ ��δ� ��Ʈ���/ScriptFiles
} LanguageInfo;

//# Filter Info ########################################################
typedef struct tagSubFilterInfo
{
	int nTime;
	int nData;
} SubFilterInfo;


#define CNT_CHANGEVAL		5
#define CNT_DIRECTION		8

typedef struct tagFilterInfo
{
	int nMarketType;			//# 0 : ��ü, 1 : �ڽ���, 2 : �ڽ���
	
	int nNowPriceChk;
	int nNowPriceUp, nNowPriceDown;
	
	int nChangRateChk;
	int nChangRateUp, nChangRateDown;
		
	int nChangValChk[CNT_CHANGEVAL];
	
	int nDealVolType;		//# 0 : ����, 1 : Time
	int nDealVol;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimeVol;
		
	int nDealPriceType;		//# 0 : ����, 1 : Time
	int nDealPrice;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimePrice;
		
	int nDirectionChk[CNT_DIRECTION];

	CStringArray saOutCode;
} FilterInfo;
#define SZ_FILTERINFO	sizeof(FilterInfo)




#endif // CCONDITION_MNGDEF
