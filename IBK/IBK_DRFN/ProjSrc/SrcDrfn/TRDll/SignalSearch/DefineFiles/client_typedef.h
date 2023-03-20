#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define WMU_SIGNAL_JM		(WM_USER + 656)	// ��ȣ�˸��� - ����ñ׳�

#define WMU_SGNLCTRL_REG			(WM_APP + 0x300)	// ��ȣ��Ʈ���� ��ȣ����(DRFN)�� ���
#define WMU_SGNLCTRL_SETUP			(WM_APP + 0x302)	// ��ȣ��Ʈ�� ��ȣ����ȭ���� ����
#define WMU_SGNLCTRL_SIGNAL			(WM_APP + 0x303)	// ��ȣ�߻��� ��ȣ��Ʈ�ѿ� ����

#define UMSG_CHART_SYMBOLDATA				WM_USER + 100
#define UMSG_CHART_GETDATETIME				WM_USER + 110
#define UMSG_CHART_SETWINDOWDATA			WM_USER + 120
#define UMSG_CHART_SETZOOMVALUE				WM_USER + 130

#define WM_NOTIFY_GRIDEVENT		WM_USER + 340
#define NM_CLICK_L				1
#define NM_CLICK_R				2
#define NM_CLICK_CELLTYPE		3
#define NM_DBCLICK_L			4

#define FOLDER_IMGDATA				"\\icss\\Image\\drchart"

#define FINDERINFO_INIFILE			"FindInfo.ini"
#define FOLDER_FINDDATA				"\\FindData"

#define PROGRAM_TITLE		"��ȣ�˻�"

#define FIXFILED_USER_ONE_ITEM		"fixfield_User_One_Item.xml"
#define FIXFILED_USER_ONE_MAKRET	"fixfield_User_One_Market.xml"
#define FIXFILED_USER_ONE_REAL		"fixfield_User_One_Real.xml"

#define	WM_BC_DATA_RECEIVED				WM_USER + 703

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define STR_CONVERT(pBuff, cData, nSize, strData)		\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  strData = (CString)pBuff;								\
  strData.TrimRight();									\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)			\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  nData = atoi(pBuff);									\
delete [] pBuff;

const UINT RMSG_GETPARTNER_MNG = ::RegisterWindowMessage(_T("RMSG_GETPARTNER_MNG"));
const UINT RMSG_RECEIVEDIRECTFID = ::RegisterWindowMessage(_T("RMSG_RECEIVEDIRECTFID"));

#define ID_MENU_START			10010
#define ID_MENU_END				10200		//# 10190, 10200 : Naver Serch

const UINT		RBTNMENU_INDEX[]			= { 10010, 10020 };
const LPCTSTR	RBTNMENU_NAME[]				= { _T("���簡"), _T("������Ʈ") };
const LPCTSTR	RBTNMENU_SCREEN_NOMAX[]		= { _T("2001"), _T("4501") };
const LPCTSTR	RBTNMENU_SCREEN_NOEMP[]		= { _T("00310"), _T("00583") };
const LPCTSTR	RBTNMENU_MAP_NO[]			= { _T("hfis077aq"), _T("hfit031aq") };
const LPCTSTR	MATCHVAL_OPT_LIST[]			= { ">", ">=", "=", "<=", "<" };
const LPCTSTR	MATCHVAL_OPT_DATA			= ">\n>=\n=\n<=\n<\n";
const LPCSTR	FIELDEDIT_TAB_STR[]			= {_T("�ü�"), _T("ȣ��"), _T("�ð�����"), _T("�ְ�/����"), _T("������"), _T("����"), _T("��Ÿ")};
const LPCTSTR	SEL_PERIOD[]				= { _T("1"), _T("2"), _T("3"), _T("4"), _T("5"),
												_T("10"), _T("15"), _T("20"), _T("30"), _T("60"), _T("120")};

#define QUERY_STRJONGMOK_MIN		_T("HKST03010200")

typedef struct tagSignalData
{
  int nRealType;
  CString strType;

  int nMarket;
  CString strCode;
  CString strName;
  CString strSqNo;

  CString strProfferID;     //�����߰�
  CString strSubProfferID;  //�����߰�

  CString strDate;
  CString strTime;

  CString strTitle;
  int nPoint;
  COLORREF clrTitle;
} SignalData;




typedef struct tagSignalNowPrice
{
	CString strJobFlag;   //�ű�, Ż�� ����
	CString strPrice;		//���簡
	CString strCode;		//�����ڵ�
	CString strName;		//�����
	CString strTime;		//��ȣ�ð�
	CString strTitle;		//��ȣ��
} STSIGNAL_NOWPRICE;


typedef struct	tagSIGNALMSGINFO
{
	TCHAR	m_sgType[2+1];		//  - ��ȣ����	: ��ȣ �󼼺з�
	TCHAR	m_sgTime[6+1];		//  - �߻��ð�	: HH:MM:SS
	TCHAR	m_sgnskey[26+1];		//  - /* ��¥[8]+�ð�[6]+�Ϸù�ȣ[8]+��������[2]+ �������걸��[2]
	TCHAR	m_sgSeqno[8+1];		//  - �Ϸù�ȣ
	TCHAR	m_sgCodeGb[1+1];		//  - 1:�ڽ���,2:�ڽ���,3:��������,4:ELW
	TCHAR	m_sgCode[8+1];		//  - �����ڵ�	: �ֽ�(1), ����(2)/�ɼ�(3), ��������(4)
	TCHAR	m_sgContext[512+1];	//  - ��ȣ����	: ����Ÿ��Ʋ / ���ǳ����
	TCHAR	m_sgGubun[1+1];		//  - �ŵ��� ����	: �Ϲ�(1) / �ż�(2) / �ŵ�(3)
	TCHAR	m_sgBlink[2+1];		//  - ��ȣ�Ӽ�	: ������ Ƚ��
	TCHAR	m_sgContinue[1+1];	//  - ��ȣ�Ӽ�	: ��������
	TCHAR	m_sgPriority[2+1];	//  - ��ȣ�Ӽ�	: �켱���� 
	TCHAR	m_sgIcon[2+1];		//  - ��ȣ�Ӽ�	: ǥ��Icon Index
	TCHAR	m_sgBugaInfo[10+1];	//  - �ΰ�����	: DRFN���� ������ ��ȣ�� ��ȣ��������
} EUGENE_SIGNALMSGINFO;

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

// ��ȣ��Ʈ���� ��ȣ�߻��� �뺸�Ǵ� ��ȣ���� ���� Structure
typedef struct tagSCSignal
{
	CString		strCode;				// �����ڵ�
	CString		strName;				// �����
	
	CString		strSgnlTime;			// ��ȣ�߻� �ð�
	CString		strSgnlCode;			// ��ȣ����
	CString		strSgnlTitle;			// ��ȣ��
	COLORREF	strSgnlText;			// ��ȣ���뿡 ������ ��Ʈ�÷�
	
} SCSIGNAL, *LPSCSIGNAL;

#define MAX_FIELD_CNT					17		//# ��� Grid Column ���� Max Count

#define CANDLE_INDEX_FILE				"candleinfo.xml"
#define PATEN_INDEX_FILE				"patterninfo.xml"
#define GRID_MOVEFIELD_POS_NAME			"[������ ��ġ]"

#define XML_VERSION_FIXFIELDFILE		"1.0"
#define FOLDER_DATA						"\\icss\\finddata"
#define FIXFIELD_SYS_FILE				"fixfield_sys.xml"
#define FIXFIELD_USER_SIGNAL_FILE		"fixfield_User_Signal.xml"/*"fixfield_User_section.xml"*/
#define FOLDER_IMAGE					"\\icss\\Image" 

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

typedef struct tagHeadFixInfo
{
	int           nColNum;        
	HeadInfoType  stHeadType;
	BOOL          bView;
} HeadFixInfo;

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

typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;

typedef struct tagCandleInfo{
	long    lID;
	CString   strName;
} CandleInfo;

typedef struct tagPatenInfo{
	long    lID;
	CString   strName;
} PatenInfo;

// #define WMU_SGNLCTRL_REG			(WM_APP + 0x300)
// #define WMU_SGNLCTRL_SETUP			(WM_APP + 0x302)	// ��ȣ��Ʈ�� ��ȣ����ȭ���� ����
// #define WMU_SGNLCTRL_SIGNAL			(WM_APP + 0x303)	// ��ȣ�߻��� ��ȣ��Ʈ�ѿ� ����

#define WMU_SIGNAL_CONDSEARCH	(WM_USER + 0x731)
typedef struct _tag_Alarm_Condsearch_Info
{
	TCHAR	szScrNo[4+1];			// - ȭ���ȣ
	TCHAR	szCode[12+1];			// - �����ڵ�
	TCHAR	szMemegb[1+1];		// - �Ÿű���(0:���о��� 1:�ŵ� 2:�ż�)
} ALARM_CONDSEARCH_INFO, *LPALARM_CONDSEARCH_INFO;

#define gszCacheDir "DrCache"
#define gszDataKey_MstCode "MstCode"
#define gszDataKey_WorkDate "WorkDate"
#define usrMasterCondition "SearchTemp"
#define USRDIR "\\user\\"
#define FILE_GROUP "\\portfolio.ini"

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

#define FILE_DATAVERSION		"SvrVersionDataFile.cfg"
#define FILE_DATAVERSION_LOCAL	"LocalVersionDataFile.cfg"
#define FILE_FINDDATA			"finddata"
#define FILE_LANGGUAGEDATA		"data"
#define FILE_EXP				"exp"
#define FILE_MAP				"map"
#define FILE_OTHER				"other"
#define FILE_PATTERNDATA		"patterndata"
#define FILE_SYS				"sys"
#define FILE_EXTENSION			".zip"

#endif // CLIENT_TYPEDEF
