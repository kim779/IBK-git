#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define KEY_SHARD_CODE      "�ڵ�-����2"
#define KEY_SHARD_TEMA      "�ڵ�-�׸�"
#define KEY_SHARD_INST      "�ڵ�-����"

#define PROGRAM_TITLE				"��Ŭ�����˰˻�"
const		LPCTSTR		SCREEN_NO[] = { _T("05010"), _T("05020"), _T("05030"), _T("05040"), _T("05050"), 
																	_T("05060"), _T("05070"), _T("05080"), _T("05090") };
const		LPCTSTR		SCREEN_NO_INDUSTRY[] = { _T("05100"), _T("05110"), _T("05120"), _T("05130"), _T("05140") };

#define TAB_TITLE_CONTRACT        "����ü��"
#define TAB_TITLE_USERCONDI       "����ڰ˻�"

//# Menu
#define ID_MENU_START			10010
#define ID_MENU_END				10200		//# 10190, 10200 : Naver Serch

// ������� ���� ȭ���ȣ�� �ٸ��� ������ ������. 07.09.12

const		UINT		MENU_INDEX[] = { 10010, 10020 };
const		LPCTSTR		DRMENU_NAME[] = { _T("���簡"), _T("������Ʈ") };
//const		LPCTSTR		MENU_SCREEN_NO[] = { _T("0501"), _T("1401") };
const		LPCTSTR		MENU_SCREEN_NOMAX[] = { _T("0501"), _T("1401") };
const		LPCTSTR		MENU_SCREEN_NOEMP[] = { _T("00310"), _T("00583") };
const		LPCTSTR		MENU_MAP_NO[] = { _T("hfis077aq"), _T("hfit031aq") };

const		UINT		MENU_INDUSTRY_INDEX[] = { 10010, 10020 };
const		LPCTSTR		MENU_INDUSTRY_NAME[] = { _T("�����������簡"), _T("������Ʈ") };
const		LPCTSTR		MENU_INDUSTRY_SCREEN_NOMAX[] = { _T("0531"), _T("1101") };
const		LPCTSTR		MENU_INDUSTRY_SCREEN_NOEMP[] = { _T("00397"), _T("00586") };
const		LPCTSTR		MENU_INDUSTRY_MAP_NO[] = { _T("hfib21020"), _T("hfib21010") };


#define ID_MENU_RESULTCTRL_START			11010
#define ID_MENU_RESULTCTRL_END				11200
const UINT			MENU_RESULTCTRL_INDEX[] = { 11010, 11020, 11030, 
																						11040, 11050 };
const LPCTSTR		MENU_RESULTCTRL_NAME[] = { "������Ʈ", "�����м�", "���ɵ��", 
                                            _T(""), "EXCEL"};
const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOMAX[] = { "1401", "1505", "���ɵ��", _T(""), _T("")};
const LPCTSTR		MENU_RESULTCTRL_SCREEN_NOEMP[] = { "00583", "00613", "���ɵ��", _T(""), _T("")};

const	LPCTSTR		MENU_RESULTCTRL_MAP_NO[] = { _T("hfit031aq"), _T("hfit025aq"), _T("���ɵ��"),
                                              _T(""), _T("") }; 
const BOOL			MENU_RESULTCTRL_SCREEN_OPEN[] = { TRUE, TRUE, TRUE, 
                                                  FALSE, FALSE};

#define SCREEN_INDEX_EXCEL		MENU_RESULTCTRL_INDEX[4]

#define SCREEN_NO_PORTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[2]
#define SCREEN_NO_PORTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[2]
#define SCREEN_NO_CHARTMAX				MENU_RESULTCTRL_SCREEN_NOMAX[0]
#define SCREEN_NO_CHARTEMP				MENU_RESULTCTRL_SCREEN_NOEMP[0]

#define SCREEN_NO_ROLL					"NOT USER"
#define SCREEN_NO_ANALYSISMAX			MENU_RESULTCTRL_SCREEN_NOMAX[1]
#define SCREEN_NO_ANALYSISEMP			MENU_RESULTCTRL_SCREEN_NOEMP[1]

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
} ShardData;

#endif // CLIENT_TYPEDEF
