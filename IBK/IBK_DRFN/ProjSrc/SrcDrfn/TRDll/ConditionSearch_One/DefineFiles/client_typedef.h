#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define WMU_SIGNAL_CONDITION	(WM_USER + 657)	// ��ȣ�˸��� - ���ǰ˻�	

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define KEY_SHARD_CODE      "�ڵ�-����2"
#define KEY_SHARD_TEMA      "�ڵ�-�׸�"
#define KEY_SHARD_INST      "�ڵ�-����"

#define PROGRAM_TITLE				"��������˻�"
#define PROGRAM_TITLE_REAL			"�ǽð����ǰ˻�"
#define PROGRAM_TITLE_INDUSTRY		"���������˻�"

const		LPCTSTR		SCREEN_NO[] = { _T("05010"), _T("05020"), _T("05030"), _T("05040"), _T("05050"), 
																	_T("05060"), _T("05070"), _T("05080"), _T("05090") };
const		LPCTSTR		SCREEN_NO_INDUSTRY[] = { _T("05100"), _T("05110"), _T("05120"), _T("05130"), _T("05140") };

const		LPCSTR		FIELDEDIT_TAB_STR[] = {_T("�ü�"), _T("ȣ��"), _T("�ð�����"), _T("�ְ�/����"), _T("������"), _T("����"), _T("��Ÿ")};

#define TAB_TITLE_CONTRACT			"����ü��"
#define TAB_TITLE_USERCONDI			"����ڰ˻�"
#define TAB_TITLE_REALSEARCH		"�ǽð��˻�"

#define XML_VERSION_FIXFIELDFILE	"1.0"
#define FIXFIELD_SYS_FILE			"fixfield_sys.xml"
#define FIXFIELD_USER_PATTERN_FILE	"fixfield_User_Pattern.xml"

#define FIXFILED_USER_ONE_ITEM		"fixfield_User_One_Item.xml"
#define FIXFILED_USER_ONE_MAKRET	"fixfield_User_One_Market.xml"
#define FIXFILED_USER_ONE_REAL		"fixfield_User_One_Real.xml"

//# Menu
#define ID_MENU_START			10010
#define ID_MENU_END				10200		//# 10190, 10200 : Naver Serch

// ������� ���� ȭ���ȣ�� �ٸ��� ������ ������. 07.09.12

const		UINT		MENU_INDEX[] = { 10010, 10020 };
const		LPCTSTR		DRMENU_NAME[] = { _T("���簡"), _T("������Ʈ") };
//const		LPCTSTR		MENU_SCREEN_NO[] = { _T("0501"), _T("1401") };
const		LPCTSTR		MENU_SCREEN_NOMAX[] = { _T("2001"), _T("4501") };
//const		LPCTSTR		MENU_SCREEN_NOEMP[] = { _T("00310"), _T("00583") };
const		LPCTSTR		MENU_MAP_NO[] = { _T("hfis077aq"), _T("hfit031aq") };

const		UINT		MENU_INDUSTRY_INDEX[] = { 10010, 10020 };
const		LPCTSTR		MENU_INDUSTRY_NAME[] = { _T("�����������簡"), _T("������Ʈ") };
const		LPCTSTR		MENU_INDUSTRY_SCREEN_NOMAX[] = { _T("0531"), _T("1101") };
const		LPCTSTR		MENU_INDUSTRY_SCREEN_NOEMP[] = { _T("00397"), _T("00586") };
const		LPCTSTR		MENU_INDUSTRY_MAP_NO[] = { _T("hfib21020"), _T("hfib21010") };


#define ID_MENU_RESULTCTRL_START			11010
#define ID_MENU_RESULTCTRL_END				11300
const UINT			MENU_RESULTCTRL_INDEX[] = { 11010, 11020, 11040, 11050, 11055, 11060 };
const LPCTSTR		MENU_RESULTCTRL_NAME[] = {"�����м�", "���ɵ��", "��������", _T(""), "EXCEL"};

const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOMAX[] = {"7129", "���ɵ��", "��������", _T(""), _T("")};
const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOEMP[] = { "2001", "4501", "���ɵ��", _T(""), _T("")};

//const	LPCTSTR		MENU_RESULTCTRL_MAP_NO[] = { _T("hfit031aq"), _T("���ɵ��"), _T(""), _T("") }; 
const	LPCTSTR		MENU_RESULTCTRL_MAP_NO[] = { _T("2001"), _T("4501"), _T("���ɵ��"), _T(""), _T("") }; 

const BOOL			MENU_RESULTCTRL_SCREEN_OPEN[] = { TRUE, TRUE, TRUE, TRUE, FALSE, FALSE};

#define SCREEN_INDEX_EXCEL		MENU_RESULTCTRL_INDEX[4]

#define SCREEN_NO_PORTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[2]
#define SCREEN_NO_PORTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[2]
#define SCREEN_NO_CHARTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_CHARTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[1]
#define SCREEN_NO_LOOK					MENU_RESULTCTRL_SCREEN_NOMAX[2]

#define SCREEN_NO_ROLL					"NOT USER"
//#define SCREEN_NO_ANALYSISMAX			MENU_RESULTCTRL_SCREEN_NOMAX[1]
//#define SCREEN_NO_ANALYSISEMP			MENU_RESULTCTRL_SCREEN_NOEMP[1]

#define SCREEN_NO_ALLBUY			"NOT USER"

//# ���� list Struct!
typedef struct tagConditionInfo
{
	int nGroupID;
	CString strGroupName;

	CString strName;
	CString strPath;
	CString strAlertKey;
	CString strExplain;

	BOOL bLoad;
	int  nOrderCnt;
	User_Finder		stUser_Finder;
} ConditionInfo;
#define SZ_CONDITIONINFO	sizeof(ConditionInfo)


typedef struct tagShardData
{
	BOOL bOpen;
	CString strScreenNo;
	CString strMapNo;

	CUIntArray iaMarkType;
	CStringArray saCode;
	CStringArray saName;
} ShardData;

typedef struct 
{
	CString szCtrlName;
	CWnd*	pWnd;
	BOOL	bVisible;
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

typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;

#define WMU_SIGNAL_CONDSEARCH	(WM_USER + 0x731)
typedef struct _tag_Alarm_Condsearch_Info
{
	TCHAR	szScrNo[4+1];			// - ȭ���ȣ
	TCHAR	szCode[12+1];			// - �����ڵ�
	TCHAR	szMemegb[1+1];		// - �Ÿű���(0:���о��� 1:�ŵ� 2:�ż�)
} ALARM_CONDSEARCH_INFO, *LPALARM_CONDSEARCH_INFO;

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

#endif // CLIENT_TYPEDEF
