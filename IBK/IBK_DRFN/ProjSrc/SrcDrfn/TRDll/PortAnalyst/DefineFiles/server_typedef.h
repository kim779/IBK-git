/************************************************************/
/*																													*/
/*	         Server =>  STRUCT/DEFINE/TYPEDEF ����	        */
/*																													*/
/*					2005. 1.  			Doori														*/
/*					�ۼ��� : �쵿��																	*/
/************************************************************/

#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

#define ERR_EXPCODE						"E00010"

#define LEN_CODE      7
#define LEN_UPCODE    4

// #define TRNO_GROUPTOBASE      "HKST03610100"/*"d1029"*/
// #define TRNO_GROUPTOBASE_UP   "HKST03610000"/*"d1020"*/

#define TRNO_TRNO_MSTCODE	  "HKST01100200"

#define TRNO_GROUPTODATE      "30503"

#define CODE_MAX_CNT		    50
#define RESULT_MAX_CNT2		  /*400*/100/*180*/
#define RESULT_MAX_CNT		  /*400*/100/*180*/

typedef struct tagGroupToBase_In
{
	char sTrIndex[4];

	char sType[1];							/* 0:��, ��:1, ��:2, ��:3 */
	char sDate[8];
	char sTime[6];
	char sTerm[2];							/* �� term                */
	char sCnt[3];						    /* ���񰳼�               */
	char sCodeList[CODE_MAX_CNT][LEN_CODE]; /* �����ڵ帮��Ʈ         */
} GroupToBase_In;
#define SZ_GROUPTOBASE_IN sizeof(GroupToBase_In)

typedef struct tagPriceData
{
    char    sPrice[9];       /* ����             */
} PriceData;

typedef struct tagBaseData_Code
{
  char sMarket[1];
  char sCode[7];		/* �����ڵ� */
  char sName[20];		/* �����ڵ� */
  char sPrice[9];		/* ���簡   */
  char sSign[1];		/* �����ȣ */
  char sChange[9];		/* ���ϴ�� */
  char sChgRate[7];		/* �����   */
  char sPre[9];			/* �������� */
} BaseData_Code;
#define SZ_BASEDATA_CODE   sizeof(BaseData_Code)

typedef CArray<BaseData_Code, BaseData_Code&>	ArrBaseData;

typedef struct tagCommonGroupToBase_Out
{
	char sTrIndex[4];
	char sCnt[3];
	char sDate[RESULT_MAX_CNT][8];
	char sTime[RESULT_MAX_CNT][6];
	char sDataCnt[3];
}CommonGroupToBase_Out;
#define SZ_COMMON_GROUPTOBASE_OUT sizeof(CommonGroupToBase_Out)

typedef struct tagGroupToBase_Out
{
	char sTrIndex[4];

	char sCnt[3];
	char sDate[RESULT_MAX_CNT][8];
	char sTime[RESULT_MAX_CNT][6];
	char sDataCnt[3];
	PriceData stPrice[CODE_MAX_CNT][RESULT_MAX_CNT2];
	BaseData_Code stBase[CODE_MAX_CNT];
} GroupToBase_Out;
#define SZ_GROUPTOBASE_OUT sizeof(GroupToBase_Out)


typedef struct tagGroupToBase_UPIn
{
	char sTrIndex[4];

	char  sType[1];           /* 0:��, ��:1, ��:2, ��:3 */
	char  sDate[8];
	char  sTime[6];
	char  sTerm[2];           /* �� term                */
	char  sCnt[3];            /* ��������               */
	char  sCodeList[CODE_MAX_CNT][LEN_UPCODE];     /* �����ڵ帮��Ʈ         */
} GroupToBase_UPIn;
#define SZ_GROUPTOBASE_UPIN sizeof(GroupToBase_UPIn)

typedef struct tagBaseData_UpCode
{
  char sCode[4];   /* �����ڵ� */
  char sPrice[9];  /* �������� */
  char sOpen[9];   /* �������� */
  char sPre[9];    /* �������� */
} BaseData_UpCode;
#define SZ_BASEDATA_UPCODE   sizeof(BaseData_UpCode)

typedef struct tagGroupToBase_UPOut
{
	char sTrIndex[4];
	char sCnt[3];
	char sDate[RESULT_MAX_CNT][8];
	char sTime[RESULT_MAX_CNT][6];
	char  sDataCnt[3];
	/*PriceData stPrice[CODE_MAX_CNT][RESULT_MAX_CNT2];*/
	PriceData stPrice[CODE_MAX_CNT][RESULT_MAX_CNT2];
	BaseData_UpCode stBase[CODE_MAX_CNT];
} GroupToBase_UPOut;
#define SZ_GROUPTOBASE_UPOUT sizeof(GroupToBase_UPOut)

typedef struct tagMSTItemInfo
{
	int nMarket;
	char szItemName[20];
} MSTITEMINFO;
#define SZ_MSTITEMINFO		sizeof(MSTITEMINFO)

typedef CMap<CString, LPCTSTR, MSTITEMINFO, MSTITEMINFO&>	MSTItemInfoMap;

//������ȸ���� tr
struct _uinfo {
	char gubn[2]; // ��������
	   // "MY" : ��������
	   // "UL" : ����� ������ ��� 
	   // "UD" : ����� ������
	char dirt[1]; // ���۹��� 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : ����ȸ
	char cont[1]; // ���ӱ��� 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel, 
		 // 'S':�׷�����, 'E':�׷�����ó��, 'b':DB���, 'R':DB����, 'V':������������, 'r':�������º���
		 // 'D':�׷����, 'j':�ش�׷���������, 'g':�׷츮��Ʈ��ȸ, 'G':�׷���������, 'C':�������
	char name[80]; // file name
	char nblc[5]; // ������, ����ũ��(max:64k)
	char retc[1]; // return value 'O':���� 'E':���� 'U'�ű� ����
	char emsg[40]; // error message
};

struct _ginfo {
	char gnox[2]; // group number
	char gnam[20]; // group name
	char jrec[4]; // _jinfo count
};

struct _jinfo {
	char gubn[1]; // ���񱸺� 0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char code[12]; // �����ڵ�
	char xprc[10]; // �����ܰ�
	char xnum[10]; // ��������
};

struct _updn {
	_uinfo uinfo;
	_ginfo ginfo;
};

const int sz_uinfo = sizeof(struct _uinfo);
const int sz_ginfo = sizeof(struct _ginfo);
const int sz_jinfo = sizeof(struct _jinfo);
#endif // SERVER_TYPEDEF

