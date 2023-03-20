#ifndef SIGNALMNG_TYPEDEF
#define SIGNALMNG_TYPEDEF


/****************************************************************/
/*                                                              */
/*						 Signal Mng Packet (Signal_MngDef.h)							*/
/*																															*/
/*						SignalMng���� ����ϴ� ��ȣ Packet!								*/
/*						SVR -> DRDS -> SignalMng													*/
/*														(User)														*/
/*																															*/
/*                   2005. 12. 12 		Doori											*/
/*                      �ۼ��� : �쵿��													*/
/****************************************************************/
#include <afxtempl.h>

#include "./Signal_PacketDef.h"

//# ��ȣ ����
#define SIGNAL_REAL_KEY						"0170"
#define NEWS_REAL_KEY						  "0169"

#define _SIGNAL_REAL_KEY						0xAA
#define _NEWS_REAL_KEY						  0xA9


#define CODE_LEN						6
#define CODE_NAME_LEN				20
#define TIME_LEN						6
#define DATE_LEN						8
#define SIGNAL_NAME_LEN			40
#define SIGNAL_DEGREE_LEN		2
#define IMG_INDEX_LEN       2

#define BROADCAST_COMPONENT			0x01
#define BROADCAST_ALL						0x02
#define BROADCAST_SIGNAL			  0x04

#define COMPOMENT_NORMAL				0x01
#define COMPOMENT_ALRAM					0x02
#define COMPOMENT_TICKER				0x04
#define COMPOMENT_ORDER					0x08


//////////////////////////////////////////////////////////////////////////
// Common Real Sub Struct
typedef struct tagSignal_CommonInfo
{
	char    sManagement;                   // �������� (0:O, 1:X)
  char    sControl;                      // �̻�޵� ���� (0:O, 1:X)
  char    sTradeStop;                    // �ŷ����� ���� (0:O, 1:X)
  char    sInvestAttention;              // �������� ���� (0:O, 1:X)
  char    sInsincerityNotice;            // �Ҽ��ǰ��� ���� (0:O, 1:X)
  char    sConfidenceLimit;              // �ſ����� ���� (0:O, 1:X)
  char    sCodeNotice;                   // ������� (0:O, 1:X)
  char    sJungriMeme;                   // �����Ÿ� ���� (0:O, 1:X)
  char    sMarket;                       // ���屸��  (1:Kospi, 2:Kosdaq)
  char    sDepositMoney[3];              // ���ű� ���� (100, 040, 033, 025)
  char    sPrice[8];                     // ���簡 (����:��)
  char    sChgRate[6];                   // ���ϴ������   Type(S999.99)
  char    sSign;                         // ���ϴ������ȣ
																					//							'1' : ����
																					//							'2' : ���
    																			//							'3' : ����
    																			//							'4' : ����
    																			//							'5' : �϶�
    																			//							'6' : �⼼����
    																			//							'7' : �⼼���
    																			//							'8' : �⼼����
    																			//							'9' : �⼼�϶�
    																			//							'0' : �⼼����
  char    sVolume[9];                    // �ŷ��� : Unit(1,000)
  char    sAmount[9];                    // �ŷ���� : Unit(100,000,000)
} Signal_CommonInfo;

typedef struct tagSignal_HeadInfo
{
  char		sKey[4];
	char		sSize[4];
  char    sCompress[1];
} Signal_HeadInfo;
#define SZ_SIGNLA_HEADINFO   sizeof(Signal_HeadInfo)


typedef struct tagSignal_ConditionInfo_Head
{
	Signal_HeadInfo		stHeadInfo;

  char    sAlertNum[4];      /* ��ȣ Ű����(���� 4�ڸ� ä��, ���� space) */
  char    sType[1];             /* System('S'), User('U')                   */
	char    sDate[8];							/* ��ȣ �߻�����                            */
  char    sTime[6];							/* ��ȣ �߻��ð�                            */
  char    sTrueCnt[4];					/* ���縸�� �����(����+�ű�)               */
  char    sDataCnt[4];					/* �� ������ ��                           */
	char    sStatus[1];						/* �����̻�: 'E', ���� ����:'W'             */
																/* ���� �ʱ�ȭ: 'I', ���� ��������: 'K'     */
																/* ���� ����: 'S'														*/
  char sFiller[7];
} Signal_ConditionInfo_Head;
#define SZ_SIGNAL_CINDITIONINFO_HEAD  sizeof(Signal_ConditionInfo_Head)

typedef struct tagSignal_ConditionInfo
{
  char		sJobFlag;						  /* �ű�:'N', ������:'R', Ż��:'O'        */
	char		sCode[CODE_LEN];
	char		sName[CODE_NAME_LEN];
	char    gsEntryCnt[4];        /* ���� Ƚ��                    */

	Signal_CommonInfo stCommonInfo;		// CommonFilter �� ���� ����
} Signal_ConditionInfo;
#define SZ_SIGNAL_CONDITIONINFO   sizeof(Signal_ConditionInfo)


typedef struct tagSignal_NewsInfo
{
	Signal_HeadInfo		stHeadInfo;

  DR_NEWS_KEY stNewsKey;
  Signal_CommonInfo stCommonInfo;		// CommonFilter �� ���� ����

	char	sNewsPoint[2];		/* ���� ����					*/
	char	sGoodPoint[2];		/* ȣ�� ���� ����				*/
	char	sBadPoint[2];			/* ���� ���� ����				*/
	  
	char	sCodeCnt[3];			/* ���� ����					*/
	char	sCaseCnt[3];			/* ��Ȳ�� ����					*/
  
	char	sCode[CODE_LEN];			/* ���� �ڵ� 				*/
  char	sName[CODE_NAME_LEN]; /* �����    				*/
  char	sProfferID[2];			  /* ���� ����ó ID		*/
	char	sProffer[20];				  /* ���� ����ó 			*/
  char	sProfferTypeID[2];    /* ����ó �з� ID		*/
	char	sProfferType[50];	    /* ����ó �з� 			*/
	char	sTitle[255];				  /* �� �� 						*/

	//-----------------------------------------------------------------------------
	// Reason		: SubNewsSignalInfo ����ü�� �����ȴ�.
	//			- CodeList(8*n) + CaseList(6*n)
	//-----------------------------------------------------------------------------
	//char	sCodeList[6*���񰹼�];			  /* �����ڵ帮��Ʈ	*/
	//char	sCaseList[6*��Ȳ������];		  /* ��Ȳ������Ʈ 
} Signal_NewsInfo;

typedef struct tagSignal_SubNewsInfo
{
	LPSTR	pCodeList;					// ���� �ڵ� ����Ʈ. { [����(1) + �����ڵ�(7)] * ���� ����	}
	LPSTR	pSituationwordList;	// ��Ȳ�� ����Ʈ.    { ��Ȳ��(6) * ��Ȳ�� ����  }
} Signal_SubNewsInfo;



/*
typedef struct tagSIGNAL_HEAD
{
  char		sSize[4];
  char    sType[4];
} SIGNAL_HEAD;
#define SZ_SIGNAL_HEAD  sizeof(SIGNAL_HEAD)

typedef struct tagSIGNAL_CONDITION
{
  SIGNAL_HEAD	stHEAD;
  
  char	sCode[CODE_LEN];								  // �����ڵ�
  char	sName[CODE_NAME_LEN];						  // �����
  char	sDate[DATE_LEN];								  // �߻� ���� 
  char	sTime[TIME_LEN];								  // �߻� �ð�. HHMMSS
  char	sSignalImageIndex[IMG_INDEX_LEN];	// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
  char	sSignalDegree[SIGNAL_DEGREE_LEN]; // ��ȣ ����. "-3" ~ "3"
  char	sOrderType;											  // �ֹ� Ÿ��. '0': �ֹ��� ���� ����. '1': ���ڵ�, '2': �ڵ�
  
  char	sSignalName[SIGNAL_NAME_LEN];		  // System Alert �ε����� �ش��ϴ� �̸�
} SIGNAL_CONDITION;
#define SZ_SIGNAL_CONDITION  sizeof(SIGNAL_CONDITION)


typedef struct tagSIGANL_NEWS
{
  SIGNAL_HEAD	stHEAD;
  
  DR_NEWS_KEY stNewsKey;

  char	sCode[CODE_LEN];									// ��ǥ ���� �ڵ�. ��ǥ ������ �߻����� �ʴ� ��� NULL
  char	sSignalImageIndex[IMG_INDEX_LEN];	// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
  char	sSignalDegree[SIGNAL_DEGREE_LEN];	// ��ȣ ����. "-3" ~ "3"
  char	sOrderType;												// �ֹ� Ÿ��. '0': �ֹ��� ���� ����. '1': ���ڵ�, '2': �ڵ�

  char	sProfferID[2];		// ����ó ���̵�
  char	sProffer[20];			// ����ó ��
  char	sProfferType[3];	// ����ó�� �з� ���̵�
  char	sProfferNM[30];		// ����ó�� �з���
  char	sTitle[300];			// ����
    
  //# 2006.01.12 => Not User!
  char	sGoodPoint[2];   	// ȣ�� ���� ����. + ��Ȳ�� �߻�
  char	sBadPoint[2];		 	// ���� ���� ����. - ��Ȳ�� �߻�.
} SIGANL_NEWS;
#define SZ_SIGANL_NEWS  sizeof(SIGANL_NEWS)
*/




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
		
	int nDealPriceType;	//# 0 : ����, 1 : Time
	int nDealPrice;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimePrice;
	
  int nNewsType;      //# 0 : ��ü, 1 : ����, 2 : ����

	int nDirectionChk[CNT_DIRECTION];

	CStringArray saOutCode;
} FilterInfo;
#define SZ_FILTERINFO	sizeof(FilterInfo)



#endif