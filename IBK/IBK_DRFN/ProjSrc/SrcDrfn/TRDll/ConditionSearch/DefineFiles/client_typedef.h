/****************************************************************/
/*                                                              */
/*   Client ���α׷����� ����ϴ� STRUCT/DEFINE/TYPEDEF ����    */
/*																															*/
/*                   2004. 11. 2 		Doori												*/
/*                        �ۼ��� : �쵿��                       */
/****************************************************************/

#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

//�墸����
#include <afxtempl.h>   //<= CList
#include "server_typedef.h"

#define KEY_SHARD_CODE      "�ڵ�-����2"
#define KEY_SHARD_ANALYSIS  "����-����"

const static char *FILE_THEME =	"THEMCD.INI";
#define TABDIR "tab"

#define STR_CONVERT(pBuff, cData, nSize, strData)	\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  strData = (CString)pBuff;									\
  strData.TrimRight();											\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)	\
  pBuff = new char[nSize + 1];						\
  memset(pBuff, 0x00, nSize + 1);					\
  memcpy(pBuff, cData, nSize);						\
  nData = atoi(pBuff);										\
delete [] pBuff;

#define STOCK_ISSUESFORADMINISTRATION		"��������"
#define STOCK_DEALSTOP						"�ŷ�����"
#define STOCK_TIDYDEAL						"�����Ÿ�"
#define STOCK_INSINCERITYNOTICE				"�Ҽ��ǰ���"
#define STOCK_ATTENTIONINVESTMENT			 "��������"
#define STOCK_NOTICEATTENTIONINVESTMENT		"�������迹��"
#define STOCK_PREFERENCE					"�켱��"
#define STOCK_DEPOSIT						"���ű�100%"
#define STOCK_DANGERINVESTMENT				"��������"
#define STOCK_WARNINGINVESTMENT				"���ڰ��"

/*
#define _countof(array) (sizeof(array)/sizeof(array[0]))
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

*/
#define GRID_MOVEFIELD_POS_NAME			"[������ ��ġ]"
#define GRID_MOVEFIELD_POS_INDEX		-100



//# Menu
#define ID_MENU_START			10010
#define ID_MENU_END				10200		//# 10190, 10200 : Naver Serch

// ������� ���� ȭ���ȣ�� �ٸ��� ������ ������. 07.09.12
const		UINT		MENU_INDEX[] = { 10010, 10020 };
const		LPCTSTR		DRMENU_NAME[] = { _T("���簡"), _T("������Ʈ") };
//const		LPCTSTR		MENU_SCREEN_NO[] = { _T("0501"), _T("1401") };
const		LPCTSTR		MENU_SCREEN_NOMAX[] = { _T("2001"), _T("4501") };
const		LPCTSTR		MENU_SCREEN_NOEMP[] = { _T("00310"), _T("00583") };
const		LPCTSTR		MENU_MAP_NO[] = { _T("hfis077aq"), _T("hfit031aq") };

const		UINT			MENU_INDUSTRY_INDEX[] = { 10010, 10020 };
const		LPCTSTR		MENU_INDUSTRY_NAME[] = { _T("�����������簡"), _T("������Ʈ") };
//const		LPCTSTR		MENU_INDUSTRY_SCREEN_NO[] = { _T("0531"), _T("1101") };
const		LPCTSTR		MENU_INDUSTRY_SCREEN_NOMAX[] = { _T("0531"), _T("1101") };
const		LPCTSTR		MENU_INDUSTRY_SCREEN_NOEMP[] = { _T("00397"), _T("00586") };
const		LPCSTR		FIELDEDIT_TAB_STR[] = {_T("�ü�"), _T("ȣ��"), _T("�ð�����"), _T("�ְ�/����"), _T("������"), _T("����"), _T("��Ÿ")};

#define ID_MENU_RESULTCTRL_START			11010
#define ID_MENU_RESULTCTRL_END				11300

const UINT			MENU_RESULTCTRL_INDEX[] = { 11010, 11020, 11040, 11050, 11055, 11060};
const LPCTSTR		MENU_RESULTCTRL_NAME[] = {"�����м�", "���ɵ��", "��������", _T(""), "EXCEL"};
const LPCTSTR		MENU_RESULTCTRL_SCREEN_NO[] = { "7112", "7129", "2001", "���ɵ��", _T(""), _T("")};
const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOMAX[] = {"7129", "���ɵ��",  "��������", _T(""), _T("")};
const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOEMP[] = { "0301", "0361", "0101", "���ɵ��", _T(""), _T("")};
const LPCTSTR		MENU_RESULTCTRL_MAP_NO[] = { _T("0301"), _T("0361"), _T("0101"), _T("���ɵ��"), _T("") , _T("")}; 
const BOOL			MENU_RESULTCTRL_SCREEN_OPEN[] = { TRUE, TRUE, TRUE, TRUE, FALSE, FALSE};

//# Industry : Add Danny (2006.11)
const UINT			MENU_INDUSTRY_RESULTCTRL_INDEX[] = { 11010, 11020, 11030, 11040, 11050};
const LPCTSTR		MENU_INDUSTRY_RESULTCTRL_NAME[] = { "������Ʈ", "���簡", "���ɵ��", _T(""), "EXCEL"};
const LPCTSTR		MENU_INDUSTRY_RESULTCTRL_SCREEN_NOMAX[] = { "4501", "2001", "���ɵ��", _T(""), _T("")};
const LPCTSTR		MENU_INDUSTRY_RESULTCTRL_SCREEN_NOEMP[] = { "4501", "2001", "���ɵ��", _T(""), _T("")};

const LPCTSTR		MENU_INDUSTRY_RESULTCTRL_MAP_NO[] = { _T("4501"), _T("2001"), _T("���ɵ��"), _T("") , _T("")}; 
const BOOL			MENU_INDUSTRY_RESULTCTRL_SCREEN_OPEN[] =  {TRUE, TRUE, TRUE, FALSE, FALSE};


#define SCREEN_INDEX_EXCEL			MENU_RESULTCTRL_INDEX[4]
#define SCREEN_INDEX_RUSULTPRINT	MENU_RESULTCTRL_INDEX[4]


#define SCREEN_NO_PORTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_PORTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[1]
#define SCREEN_NO_CHARTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_CHARTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[0]
#define SCREEN_NO_ROLL					"NOT USER"
//#define SCREEN_NO_ANALYSISMAX			MENU_RESULTCTRL_SCREEN_NOMAX[1]
//#define SCREEN_NO_ANALYSISEMP			MENU_RESULTCTRL_SCREEN_NOEMP[1]
#define SCREEN_NO_ALLBUY				"NOT USER"

/*
#define SCREEN_NO_PORTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[2]
#define SCREEN_NO_PORTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[2]
#define SCREEN_NO_CHARTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_CHARTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[0]
#define SCREEN_NO_ROLL					"NOT USER"
#define SCREEN_NO_ANALYSISMAX			MENU_RESULTCTRL_SCREEN_NOMAX[1]
#define SCREEN_NO_ANALYSISEMP			MENU_RESULTCTRL_SCREEN_NOEMP[1]
#define SCREEN_NO_ALLBUY				"NOT USER"
*/
#define PROGRAM_SCREEN_NO			"37110"
#define PROGRAM_TITLE				"���ǰ˻�"
#define PROGRAM_TITLE_BRIGHT		"��������߱�"

/*

#define XML_VERSION_USERFILE	"2.0"

#define FOLDER_NEWS						"\\Data\\NewsData"
#define NEWS_KEYWORD_FILE			"\\txnews_keyword.dat"

#define FOLDER_TABLE					"\\Table"
#define MEMBERCOMP_FILE				"seccd.dat"

#define FOLDER_DATA						"\\Data\\FindData"
//#define FOLDER_DATA						"\\FindItem"
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
  */


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

/*
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
*/

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

#define MAX_PORT_GROUP        99
#define MAX_PORT_ITEM         200
typedef struct tagPortGroupInfo
{
	int nNo;
	CString strName;

	int nItemCnt;
	int nMarketType[MAX_PORT_ITEM];		 //# �ŷ���:'1', �ڽ���:'2'
	CString strCode[MAX_PORT_ITEM];
} PortGroupInfo;

/*
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
	BOOL bFalg;					//# ���� ���� : TRUE
	
	int	nNotUserFlag;
	int	nGubunUserFlag;			//# ����/�񿬼� 

	int	nTargetBaseType;		//# TargetBase Type (�ϰ�����, �׿� ...)

	CString strTag;		  //# Tag  <= Map Create Tag
	CString strMapFile;	//# Map File Name

	int	nNonAlert;
	int	nChartValSet;

	CString strName;	//# ��ǥ��
	CString strText;	//# ��ǥ �ϼ� ����
	
	User_Define stCondition;

	BOOL bFieldVal;			//# ������
	BOOL bFieldState;		//# ������ Check Field Have
	BOOL bFieldEnable;	//# ������ Enable ���� 
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
*/

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

/*
//# [Xml Tree] ###############################################################################
#define TREE_SELECT_TYPE_CLASS        1

#define TREE_SELECT_TYPE_ITEM_COMM    10
#define TREE_SELECT_TYPE_ITEM_ORDER   11
#define TREE_SELECT_TYPE_PORTPOLIO    12
#define TREE_SELECT_TYPE_ACCOUNT      13
#define TREE_SELECT_TYPE_LANGUAGE     14
#define TREE_SELECT_TYPE_USERPATTEN   15
#define TREE_SELECT_TYPE_NEWS         16

#define TREE_SELECT_TYPE_USERFILE     20
#define TREE_SELECT_TYPE_SYSFILE      21

#define TAG_TYPE_PORTPOLIO    "PORT"
#define TAG_TYPE_ACCOUNT      "ACCOUNT"
#define TAG_TYPE_LANGUAGE     "LANGUAGE"
#define TAG_TYPE_USERPATTEN   "USER_PATTEN"
#define TAG_TYPE_NEWS         "NEWS"

#define CONVERT_TAGTOTYPE(strItemTag, nItemType)	      \
  if (strItemTag == (CString)TAG_TYPE_PORTPOLIO)        \
    nItemType = (int)TREE_SELECT_TYPE_PORTPOLIO;        \
  else if (strItemTag == (CString)TAG_TYPE_ACCOUNT)     \
    nItemType = (int)TREE_SELECT_TYPE_ACCOUNT;          \
  else if (strItemTag == (CString)TAG_TYPE_LANGUAGE)    \
    nItemType = (int)TREE_SELECT_TYPE_LANGUAGE;         \
  else if (strItemTag == (CString)TAG_TYPE_USERPATTEN)  \
    nItemType = (int)TREE_SELECT_TYPE_USERPATTEN;       \
  else if (strItemTag == (CString)TAG_TYPE_NEWS)        \
    nItemType = (int)TREE_SELECT_TYPE_NEWS;             \
  else                                                  \
    nItemType = -1;
   

#define NEWS_SUBROOTTITLE			  	"�����м�"
#define PORTPOLIO_SUBROOTTITLE		"���ɱ׷�"
#define LANGUAGE_SUBROOTTITLE			"������"
#define PATTEN_ROOTTITLE				  "���������"
#define ACCOUNT_SUBROOTTITLE			"��������׷�"

#define CONVERT_TYPETOTITLE(nItemType, strTitle)	        \
  if (nItemType == (int)TREE_SELECT_TYPE_PORTPOLIO)       \
    strTitle = (CString)PORTPOLIO_SUBROOTTITLE;           \
  else if (nItemType == (int)TREE_SELECT_TYPE_ACCOUNT)    \
    strTitle = (CString)ACCOUNT_SUBROOTTITLE;             \
  else if (nItemType == (int)TREE_SELECT_TYPE_LANGUAGE)   \
    strTitle = (CString)LANGUAGE_SUBROOTTITLE;            \
  else if (nItemType == (int)TREE_SELECT_TYPE_USERPATTEN) \
    strTitle = (CString)PATTEN_ROOTTITLE;                 \
  else if (nItemType == (int)TREE_SELECT_TYPE_NEWS)       \
    strTitle = (CString)NEWS_SUBROOTTITLE;                \
  else                                                    \
    strTitle = ""

#define MAP_OTHER_FILE        "Other"

typedef struct tagNewsInfo
{
	int nIndex;
	int nCode;
	CString strType;
	CString strKeyWord;
} NewsInfo;

typedef struct tagXMLTreeInfo
{
	int nItemType;

	CString strName;
	CString strTag;

	int nNonAlert;
	int nCondiType;
	int nTargetBase_Type;

	int nGubun;
	int nGubunDefVal;

	int nNotUser;
	int nNotDefVal;
		
	CString strResultViewID;
	int nResultViewDefVal;

	int nChartValSet;
	CString strMapFile;
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
*/

typedef struct tagShardData
{
	BOOL bOpen;
  CString strScreenNo;
	CString strMapNo;
	
	CUIntArray iaMarkType;
	CStringArray saCode;
	CStringArray saName;
} ShardData;

/*
//# Language!
typedef struct tagLanguageInfo
{
	int	nType;	// ���� ��� 0 : TS, 1 : YES, 2 : YES
	LPSTR	lpszName;		  // ���ϸ��� Ȯ���� .sts�� ���̸� ��
					            // ��ũ��Ʈ ��δ� ��Ʈ���/ScriptFiles
} LanguageInfo;
*/

typedef struct 
{
	CString szCtrlName;
	CWnd* pWnd;
} ST_MOVEITEM;
#define SET_ST_MOVEITEM(xST, xName, xpWnd) xST.szCtrlName=xName; xST.pWnd=xpWnd;

typedef struct  
{
	COLORREF clrF;
	COLORREF clrB;	
	COLORREF clrSF;
	COLORREF clrSB;
	COLORREF clrBACK;
} ST_TABCOLOR;

typedef struct  
{
	COLORREF clrGridHeader;
	COLORREF clrGridHText;  
	COLORREF clrGridHilight;
	COLORREF clrGridSBk;
	COLORREF clrGridSText;  
	COLORREF clrGridUpText;  
	COLORREF clrGridDownText;
	COLORREF clrGridVolText; 
} ST_GRIDCOLOR;

typedef struct
{
	// ��������
	int nIssuesForAdministration;		// ��������
	int nDealStop;						// �ŷ�����
	int nClearanceDeal;					// �����Ÿ�
	int nInsincerityNotice;				// �Ҽ��ǰ���
	int nInvestmentAttention;			// ��������
	int nInvestmentAttentionpPreview;	// �������� ����
	int nPreferenceStock;				// �켱��
	int nMargin;						// ���ű� 100%
	int nInvestmentRisk;				// ��������
	int nInvestmentWarning;				// ���ڰ��

	int nSettlingMonth;					// ���� 0 - ��ü 

} ST_TARGETEDITINFO;

#include <vector>
using namespace std;
typedef struct tagHaveItem // ���º� ��������
{
	CString strAccount;
	vector<CString> vecItemCode;
	vector<CString> vecItemName;
} ST_HAVEITEM;

typedef struct tagItemUpCode
{
	CString strMarket;
	CString strType;

	CString strItemCode;
	CString	strUpCodeL;
	CString	strUpCodeM;
	CString	strUpCodeS;
} ST_UPCODE;

enum {
	IDC_BTN_REALCHK = 1000,
		IDC_COMBO_SEARCHTYPE,
		IDC_COMBO_PORTGROUP,
		IDC_COMBO_THEME,
		IDC_BTN_INITCODE,
		IDC_BTN_SEARCH,
		IDC_BTN_INIT,
		IDC_BTN_NEXT,
		IDC_COMBO_SHARETYPE,
		IDC_BTN_AUTOBODYCHK,
		IDC_BTN_SET,
		IDC_BTN_TITLECHK,
		IDC_BTN_BODYCHK,
		IDC_BTN_WORDSET,
		IDC_BTN_WORD_AND,
		IDC_BTN_WORD_OR,
		IDC_BTN_WORDSET_INIT,
		IDC_BTN_TIP,
		IDC_BTN_EXP_BODY,
		IDC_BTN_EXP_LIST,
		IDC_EDT_PASSWD,
		IDC_COMBO_CODE,
		IDC_WND_INFO,
		IDC_ACCOUNT,
		IDC_EDT_WORD,
		
		IDC_BTN_EXP_SEARCH,
		IDC_BTN_EXP_INFOWND,
		
		IDC_BTN_ALLRDO,
		IDC_BTN_PARTRDO
};

#define GETMARKET_ALL		1000
#define GETMARKET_KOSPI		0
#define GETMARKET_KOSPI200	100
#define	GETMARKET_KOSDAQ	200
#define GETMARKET_KRX		900

struct	Hana_UpMaster	{
	char	jgub;			// �屸��
	// 0:�峻 1:���
	// 1: KOSPI200
	// 3: KOSDAQ50(1), KQ-IT 50(2)
	// 4: K100P(1), K50(2), IT(3
	// 6: SECTOR, 7:�������� 
	char	ucod;			// �����ڵ�
	char	hnam[20];	// �ѱ۸�
};

struct	CUMaster	{
	CString m_szName;		// 18 ������
	CString m_szGubun;		// 1 ����
	CString m_szCode;		// 3  �����ڵ�
};

struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.�ſ����� 1.All */
    char    fill[10];
};

struct  s_rec   {
    char    cod2[12];   /* �����ڵ�         */
    char    hnam[40];   /* �����           */
    char    jggb[2];    /* �ܰ��ڵ�         */
    char    jqty[10];   /* �ܰ����         */
    char    xqty[10];   /* �ŵ�����         */
    char    pamt[10];   /* ������հ�       */
    char    mamt[15];   /* ���Աݾ�         */
	
    char    curr[10];   /* ���簡     (*)   */
	char    diff[10];   /* ���ϴ��         */
    char    rate[10];   /* �����           */
    char    camt[15];   /* �򰡱ݾ�         */
    char    tamt[15];   /* �򰡼���   (*)   */
    char    srat[10];   /* �򰡼��ͷ� (*)   */
    char    sycd[2];    /* �ſ��ڵ�         */
    char    sydt[8];    /* ������           */
    char    samt[15];   /* �ſ�ݾ�         */
}; 

struct  s_mod   {
    char    accn[11];
    char    nrec[4];
    struct  s_rec   rec[100];
}; 



#define L_smid	sizeof(struct s_mid)				// Length of stock input


#endif // CLIENT_TYPEDEF
