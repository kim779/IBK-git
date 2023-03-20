#ifndef SIGNALMNG_TYPEDEF
#define SIGNALMNG_TYPEDEF


/****************************************************************/
/*																*/
/*						 Signal Mng Packet (Signal_MngDef.h)	*/
/*																*/
/*						SignalMng���� ����ϴ� ��ȣ Packet!		*/
/*						SVR ->  SignalMng						*/
/*														(User)	*/
/*																*/
/*                   2006. 10.  		Doori					*/
/*                      �ۼ��� : �쵿��							*/
/****************************************************************/
#include <afxtempl.h>

#include "./Signal_PacketDef.h"

#define CODE_LEN						6
#define CODE_NAME_LEN					40	//@���� 20->40
#define TIME_LEN						6
#define DATE_LEN						8
#define SIGNAL_DEGREE_LEN				2
#define IMG_INDEX_LEN					2

#define BROADCAST_COMPONENT				0x01
#define BROADCAST_ALL					0x02
#define BROADCAST_SIGNAL				0x04

#define COMPOMENT_NORMAL				0x01
#define COMPOMENT_ALRAM					0x02
#define COMPOMENT_TICKER				0x04
#define COMPOMENT_ORDER					0x08

#define	WM_BC_DATA_RECEIVED				WM_USER + 703

#define GETPATH_APP(strAppPath)       \
	char buf[255];														\
	GetModuleFileName(NULL, buf, 255);									\
	int lastlocationofbackslash = 0;									\
	int nBufCnt = strlen(buf);											\
	for (int i = 0; i < nBufCnt; i++)									\
	{																	\
	if (buf[i] == '\\')													\
	lastlocationofbackslash = i;										\
	}                                                                   \
	CString strAppPath = _T("");                                        \
	for (int j = 0; j < lastlocationofbackslash; j++)                   \
	strAppPath += buf[j];  

//////////////////////////////////////////////////////////////////////////
// Common Real Sub Struct
typedef struct tagSignal_CommonInfo
{
	char    sManagement;                   // �������� (0:O, 1:X)
	char	attr_smanagement;
	char    sControl;                      // ��������(0), ��������(1), ���ڰ��(2), �������迹��(3),��������(4)
	char	attr_scontrol;
	char    sTradeStop;                    // �ŷ����� ���� (0:O, 1:X)
	char	attr_stradestop;
	char    sInvestAttention;              // �������� ���� (0:O, 1:X)
	char	attr_sinvestattention;
	char    sInsincerityNotice;            // �Ҽ��ǰ��� ���� (0:O, 1:X)
	char	attr_sinsinceritynotice;
	char    sConfidenceLimit;              // �ſ����� ���� (0:O, 1:X)
	char	attr_sconfidencelimit;
	char    sCodeNotice;                   // ������� (0:O, 1:X)
	char	attr_scodenotice;
	char    sJungriMeme;                   // �����Ÿ� ���� (0:O, 1:X)
	char	attr_sjungrimeme;
	char    sMarket;                       // ���屸��  (1:Kospi, 2:Kosdaq)
	char	attr_smarket;
	char    sDepositMoney[3];              // ���ű� ���� (100, 040, 033, 025)
	char	attr_sdepositmoney;	
	//# Code Info
	char    sPrice[8];           // ���簡 (����:��)
	char	attr_sprice;
	char    sSign;               // ���ϴ���ȣ  
	char	attr_ssign;
	char    sChage[8];           // ���ϴ��    
	char	attr_schange;	
	char    sChgRate[6];         // �����   Type(S999.99)   
	char	attr_schgrate;	
	char    sVolume[9];          // �ŷ��� : Unit(1,000)   
	char	attr_svolume;	
	char    sVolumeRect[6];      // �ŷ��� �����: Unit(1,000)  
	char	attr_svolumerect;	
	char    sAmount[9];          // �ŷ���� : Unit(100,000,000)
	char	attr_samount;
	char	sResultField[1040];				// ���� ����ʵ�
} Signal_CommonInfo;

typedef struct tagSignal_CommonInfo2
{
	char    sManagement;                   // �������� (0:O, 1:X)
	char    sControl;                      // ��������(0), ��������(1), ���ڰ��(2), �������迹��(3),��������(4)
	char    sTradeStop;                    // �ŷ����� ���� (0:O, 1:X)
	char    sInvestAttention;              // �������� ���� (0:O, 1:X)
	char    sInsincerityNotice;            // �Ҽ��ǰ��� ���� (0:O, 1:X)
	char    sConfidenceLimit;              // �ſ����� ���� (0:O, 1:X)
	char    sCodeNotice;                   // ������� (0:O, 1:X)
	char    sJungriMeme;                   // �����Ÿ� ���� (0:O, 1:X)
	char    sMarket;                       // ���屸��  (1:Kospi, 2:Kosdaq)
	char    sDepositMoney[3];              // ���ű� ���� (100, 040, 033, 025)
	//# Code Info
	char    sPrice[8];           			// ���簡 (����:��)
	char    sSign;               			// ���ϴ���ȣ  
	char    sChage[8];           			// ���ϴ��    
	char    sChgRate[6];         			// �����   Type(S999.99)   
	char    sVolume[9];          			// �ŷ��� : Unit(1,000)   
	char    sVolumeRect[6];      			// �ŷ��� �����: Unit(1,000)  
	char    sAmount[9];          			// �ŷ���� : Unit(100,000,000)

	char	sResultField[1040];				// ���� ����ʵ�
} Signal_CommonInfo2;

typedef struct tagSignal_HeadInfo
{
	char		sKey[4];
	char		attr_skey[1];
	char		sSize[4];   
	char		attr_ssize[1];
	char		sCompress[1];
	char		attr_scompress[1];
} Signal_HeadInfo;
#define SZ_SIGNLA_HEADINFO   sizeof(Signal_HeadInfo)

typedef struct tagSignal_HeadInfoAlert
{
	char		sKey[4];
} Signal_HeadInfoAlert;
#define SZ_SIGNLA_HEADINFO   sizeof(Signal_HeadInfo)



typedef struct tagSignal_ConditionInfo_Head
{
//	Signal_HeadInfoAlert	stHeadInfo;
	
	char    sAlertNum[4];         /* ��ȣ Ű����(���� 4�ڸ� ä��, ���� space) */
	char    sKey[4];  
	char    sType[1];             /* System('S'), User('U')                   */
	char    sDate[8];							/* ��ȣ �߻�����                            */
	char    sTime[6];							/* ��ȣ �߻��ð�                            */
	char    sTrueCnt[4];					/* ���縸�� �����(����+�ű�)               */
	char    sDataCnt[4];					/* �� ������ ��                           */
	char    sStatus[1];						/* �����̻�: 'E', ���� ����:'W'             */
	/* ���� �ʱ�ȭ: 'I', ���� ��������: 'K'     */
	/* ���� ����: 'S'														*/
//��������	char sFiller[7]; 
} Signal_ConditionInfo_Head;
#define SZ_SIGNAL_CINDITIONINFO_HEAD  sizeof(Signal_ConditionInfo_Head)

typedef struct tagSignal_ConditionInfo
{
	char		sJobFlag;						  /* �ű�:'N', ������:'R', Ż��:'O'        */
	char		sCode[CODE_LEN];
	char		sName[20]; //[CODE_NAME_LEN];
	char    sEntryCnt[4];         /* ���� Ƚ��                    */
	
	Signal_CommonInfo stCommon;
} Signal_ConditionInfo;
#define SZ_SIGNAL_CONDITIONINFO   sizeof(Signal_ConditionInfo)

typedef struct tagSignal_ConditionInfo2
{
	char		sJobFlag;						  /* �ű�:'N', ������:'R', Ż��:'O'        */
	char		sCode[CODE_LEN];
	char		sName[20]; //[CODE_NAME_LEN];
	char    sEntryCnt[4];         /* ���� Ƚ��                    */
	
	Signal_CommonInfo2 stCommon;
} Signal_ConditionInfo2;
#define SZ_SIGNAL_CONDITIONINFO2   sizeof(Signal_ConditionInfo2)


typedef struct tagNews_CommonInfo
{
	DR_NEWS_KEY stNewsKey;
	
	char	sDate				[8];		/* ���� ����				*/
	char	sTime				[6];		/* ���� �ð�				*/

	char	sNewsPoint			[2];		/* ���� ����					*/
	char	sCode				[CODE_LEN];	/* ���� �ڵ� 				*/
	char	sName				[40];		//@��������sName[CODE_NAME_LEN]; /* �����    				*/
	char	sProfferID			[2];		/* ���� ����ó ID		*/
	char	sProffer			[20];		/* ���� ����ó 			*/
	char	sProfferTypeID		[3];		//@��������sProfferTypeID[2];    /* ����ó �з� ID		*/
	char	sProfferType		[50];	    /* ����ó �з� 			*/
	char	sTitle				[255];		/* �� �� 						*/
} News_CommonInfo;

typedef struct tagSignal_NewsInfo     //# [OR] News All Input
{
	News_CommonInfo   stNewsCommon;
	char    sFiller[3];
	Signal_CommonInfo stCommon;
} Signal_NewsInfo;

typedef struct tagSignal_NewsInfo_Head    //# News Real Data Packet!
{
//	char	dgubun				[4];	/* �����ͱ���   "NWPS"		*/
//	char	nwcode				[6];	/* ��Ŷ�����ڵ� "NEWS01"	*/
	
	Signal_HeadInfo		stHeadInfo;
	Signal_NewsInfo   stNewsInfo;
	
//��������	char	sCaseLen[8];			/* ��Ȳ�� ����				*/
	char	sCaseLen[2];			/* ��Ȳ�� ����				*/
	//# + ��Ȳ�� ����Ʈ.    { ��Ȳ��(6) * ��Ȳ�� ����  }
	char	sCSCd[180];		/* ��Ȳ�� CodeList; 30*6	*/
} Signal_NewsInfo_Head;

#define TRCODE_NEWS_ALL			"htif_o001013"

//# News All Out Packet
typedef struct tagSignal_AllNewsInfo
{
	Signal_NewsInfo   stNewsInfo;
	
	char	sCodeLen[8];			/* ���� ����					*/
	char	sCaseLen[8];			/* ��Ȳ�� ����				*/
	char  sBodyLen[12];
	/*
	//# + ���� �ڵ� ����Ʈ. { [����(1) + �����ڵ�(7)] * ���� ����	}
	//# + ��Ȳ�� ����Ʈ.    { ��Ȳ��(6) * ��Ȳ�� ����  }
	//# + ����
	*/
} Signal_AllNewsInfo;

typedef struct tagKeywordData
{
	int nIndex;
	
	int nType;		//# 0 : Sys, 1 : User
	CString strTitle;
	int  nBasePoint;
	
	CString strCode;
	CString strCode_S, strName_S;
	int nPoint_S;
	CString strCode_V, strName_V;
	int nPoint_V;
} KeywordData;

typedef struct tagConditionData
{
	int nIndex;
	
	CString strTitle;
	int  nBasePoint;
	
	CString strCode;
	CString strPath;
} ConditionData;

typedef struct tagConditionDataGroup
{
	CString strGroup;
	CList <ConditionData, ConditionData&> *plistData;
} ConditionDataGroup;


#define SETINIT_SIGNALDATA(pstSetSignalData)         \
	pstSetSignalData->nIndex = 0;                      \
	pstSetSignalData->strKey = "";                     \
	pstSetSignalData->nType = 0;                       \
	\
	pstSetSignalData->stKeywordData.nIndex = 0;        \
	pstSetSignalData->stKeywordData.nType = 0;         \
	pstSetSignalData->stKeywordData.strTitle = "";     \
	pstSetSignalData->stKeywordData.nBasePoint = 0;    \
	pstSetSignalData->stKeywordData.strCode = "";      \
	pstSetSignalData->stKeywordData.strCode_S = "";    \
	pstSetSignalData->stKeywordData.strName_S = "";    \
	pstSetSignalData->stKeywordData.nPoint_S = 0;      \
	pstSetSignalData->stKeywordData.strCode_V = "";    \
	pstSetSignalData->stKeywordData.strName_V = "";    \
	pstSetSignalData->stKeywordData.nPoint_V = 0;      \
	\
	pstSetSignalData->stConditionData.nIndex = 0;      \
	pstSetSignalData->stConditionData.strTitle = "";   \
	pstSetSignalData->stConditionData.nBasePoint = 0;  \
	pstSetSignalData->stConditionData.strCode = "";    \
	pstSetSignalData->stConditionData.strPath = "";    \
	\
	pstSetSignalData->strTitle = "";                   \
	pstSetSignalData->nPoint = 0;                      \
	pstSetSignalData->nSend_Signal = 0;                \
	pstSetSignalData->nSend_Ticker = 0;                \
	pstSetSignalData->nSend_Order = 0;                 \
	pstSetSignalData->nState = 0;

#define SETCOPY_SIGNALDATA(stSetSignalData, pstSetSignalData)             \
	pstSetSignalData->nIndex = stSetSignalData.nIndex;                      \
	pstSetSignalData->strKey = stSetSignalData.strKey;                      \
	pstSetSignalData->nType = stSetSignalData.nType;                        \
	\
	pstSetSignalData->stKeywordData.nIndex = stSetSignalData.stKeywordData.nIndex;        \
	pstSetSignalData->stKeywordData.nType = stSetSignalData.stKeywordData.nType;          \
	pstSetSignalData->stKeywordData.strTitle = stSetSignalData.stKeywordData.strTitle;    \
	pstSetSignalData->stKeywordData.nBasePoint = stSetSignalData.stKeywordData.nBasePoint;\
	pstSetSignalData->stKeywordData.strCode = stSetSignalData.stKeywordData.strCode;      \
	pstSetSignalData->stKeywordData.strCode_S = stSetSignalData.stKeywordData.strCode_S;  \
	pstSetSignalData->stKeywordData.strName_S = stSetSignalData.stKeywordData.strName_S;  \
	pstSetSignalData->stKeywordData.nPoint_S = stSetSignalData.stKeywordData.nPoint_S;    \
	pstSetSignalData->stKeywordData.strCode_V = stSetSignalData.stKeywordData.strCode_V;  \
	pstSetSignalData->stKeywordData.strName_V = stSetSignalData.stKeywordData.strName_V;  \
	pstSetSignalData->stKeywordData.nPoint_V = stSetSignalData.stKeywordData.nPoint_V;    \
	\
	pstSetSignalData->stConditionData.nIndex = stSetSignalData.stConditionData.nIndex;    \
	pstSetSignalData->stConditionData.strTitle = stSetSignalData.stConditionData.strTitle;\
	pstSetSignalData->stConditionData.nBasePoint = stSetSignalData.stConditionData.nBasePoint;\
	pstSetSignalData->stConditionData.strCode = stSetSignalData.stConditionData.strCode;  \
	pstSetSignalData->stConditionData.strPath = stSetSignalData.stConditionData.strPath;  \
	\
	pstSetSignalData->strTitle = stSetSignalData.strTitle;                  \
	pstSetSignalData->nPoint = stSetSignalData.nPoint;                      \
	pstSetSignalData->nSend_Signal = stSetSignalData.nSend_Signal;          \
	pstSetSignalData->nSend_Ticker = stSetSignalData.nSend_Ticker;          \
	pstSetSignalData->nSend_Order = stSetSignalData.nSend_Order;            \
	pstSetSignalData->nState = stSetSignalData.nState;


typedef struct tagSetSignalData
{
	int nIndex;
	CString strKey;
	
	int nType;			//# 0 : Keyword, 1 : Condition
	KeywordData			stKeywordData;
	ConditionData		stConditionData;
	
	CString strTitle;
	int  nPoint;
	
	int  nSend_Signal;
	int  nSend_Ticker;
	int  nSend_Order;
	
	int nState;
} SetSignalData;


typedef struct tagSubFilterInfo
{
	int nTime;
	int nData;
} SubFilterInfo;


#define CNT_CHANGEVAL		5
//#define CNT_DIRECTION		8
#define CNT_DIRECTION_0		8
#define CNT_DIRECTION_1		11	// 8+3

typedef struct tagFilterInfo
{
	int nInCodeType;        //# 0 : Market, 1 : PortPolio 
	CString strInCodeData;  //# [0 : ��ü, 1 : �ڽ���, 2 : �ڽ���] or [PortPolio Name]
	CStringArray *psaInCode;  //# User After Load!
	
	int nNowPriceChk;
	int nNowPriceUp, nNowPriceDown;
	
	int nChangRateChk;
	int nChangRateUp, nChangRateDown;
	
	int nChangValChk[CNT_CHANGEVAL];
	
	int nDealVolType;		//# 0 : ����, 1 : Time
	int nDealVol;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimeVol;
	
	int nDealPriceType;	//# 0 : ����, 1 : Time
	int nDealPrice;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimePrice;
	
	int nNewsType;      //# 0 : ��ü, 1 : ����, 2 : ����
	
	int nDirectionChk[CNT_DIRECTION_1];
	
	CStringArray saOutCode;
} FilterInfo;
#define SZ_FILTERINFO	sizeof(FilterInfo)


//-->@�������� �߰�
typedef struct __REALNEWS_HEAD
{
//	char	dgubun				[4];	/* �����ͱ���   "NWPS"		*/
//	char	nwcode				[6];	/* ��Ŷ�����ڵ� "NEWS01"	*/

//	char	sKey				[4];	/* sPrfCd(2)+sBrnchCd(2) */
//	char	sSize				[4];	/* dgubun(4)+nwcode(4) ������ ����ü ũ�� */
//	char	sCompress			[1];	/* '0'=FALSE; ��������:�׻�'0' */
	Signal_HeadInfo				stHeadInfo;

//	char	sNsKey				[26];	/* ��¥[8]+�ð�[6]+����[8]+����[2]+����[2] */
	DR_NEWS_KEY					stNewsKey;
	char						attr_snskey[1];

	char	sDate				[8];	/* ���� ����				*/
	char	attr_sdate			[1];
	char	sTime				[6];	/* ���� �ð�				*/
	char	attr_stime			[1];
	char	sNsPnt				[2];	/* ���� ���� = ����			*/
	char	attr_snspnt			[1];
	char	sRepCd				[6];	/* ��ǥ �ڵ� 				*/
	char	attr_srepcd			[1];
	char	sRepNm				[40];	/* ��ǥ �ڵ��				*/
	char	attr_srepnm			[1];
	char	sPrfCd				[2]; 	/* ���� ����ó ID			*/
	char	attr_sprfcd			[1];
	char	sPrfNm				[20]; 	/* ���� ����ó				*/
	char	attr_sprfnm			[1];
	char	sBrnchCd			[3];	/* ���� ����ó�� �з� ID	*/
	char	attr_sbrnchcd		[1];
	char	sBrnchNm			[50];	/* ���� ����ó�� �з�		*/
	char	attr_sbrnchnm		[1];	
	char	sTitle				[255];	/* ���� ����				*/
	char	attr_stitle			[1];
//	char	sManagement			[1];	/* 0=����,1=��������		*/
//	char	sControl			[1];	/* 0=����,1=��������		*/
//	char	sTradeStop			[1];	/* 0=����,1=�ŷ�����/�ߴ�	*/
//	char	sInvestAttention	[1];	/* 0=����,1=������������(KOSDAQ) */
//	char	sInsincerityNotice	[1];	/* 0=����,1=�Ҽ��ǰ�������	*/
//	char	sConfidenceLimit	[1];	/* 0=����,1=�ſ�����		*/
//	char	sCodeNotice			[1];	/* 0=����,1=�������		*/
//	char	sJungriMeme			[1];	/* 0=���,1=�����Ÿ�����	*/
//	char	sMarket				[1];	/* 1=�ŷ���,2=�ڽ���,3=�������� */
//	char	sDepositMoney		[3];	/* ���űݺ���:100,040,033,025 */
//	char	sPrice				[8];	/* ���簡(����:��)			*/
//	char	sSign				[1];	/* '3':����,'4':�϶�,'5':����,'6':���,'7':���� */
//	char	sChange				[8];	/* ���ϴ��					*/
//	char	sChgRate			[6];	/* �����					*/
//	char	sVolume				[9];	/* �ŷ���(����:õ��)		*/
//	char	sVolumeRect			[6];	/* �ŷ��������				*/
//	char	sAmount				[9];	/* �ŷ����(����:�鸸��)	*/
	Signal_CommonInfo			stCommon;

	char	sCSCnt				[2];	/* ��Ȳ�� ����				*/	
	char	attr_scscnt			[1];
	char	sCSCd				[180];	/* ��Ȳ�� CodeList; 30*6	*/	
	char	attr_scscd			[1];
} REALNEWS_HEAD;
#define SZ_REALNEWS_HEAD sizeof(REALNEWS_HEAD)
//<--

typedef struct stRealData
{
	BYTE	chCount;
	char	szKeyCode[4];
	int		nSize;
	LPSTR	pDataBuf;
	int		lRealStruct;
	BYTE	chRealType;
} ST_REALDATA;

#endif