#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

/************************************************************/
/*																													*/
/*	One Clk Condition Server =>  STRUCT/DEFINE/TYPEDEF ����	*/
/*																													*/
/*					2005. 1.  			Doori														*/
/*					�ۼ��� : �쵿��																	*/
/************************************************************/

#include "../../Dr_Include/EU_p0622.h"
#include "../../Dr_Include/EU_p0628.h"	//�׸���ȸ

#define CONDITION_MAXCNT		50

#define TRNO_CONDIONECLK_LIST		"p0823"	//"p0623"			//"hfit_o071006"

//#define TRNO_ALERT_REALREG				"hfit_o071002"


//# ��Ŭ�� ���� ��ȸ!
typedef struct
{
	int			nTrIndex;
	char        sType;      //# '1' : ����ü��, '0' : �׿�
	char        sFiller[3];

	//char        sFilterGubun;          /* ���: 1, �̻��: 0               */
	//User_Finder stFilter;              /* ��������                         */

	User_Finder stCondition;           /* �˻�����                         */
} TR_RESULT_IN;
#define SZ_TR_RESULT_IN  sizeof(TR_RESULT_IN)

//# �׸� ���� ��ȸ
typedef struct
{
	int         nTrIndex;

	char        sFilterGubun;			/* ���: 1, �̻��: 0               */
	User_Finder stFilter;					/* ��������                         */
	char        sCode[4];					/* �׸� �ڵ�												*/
} TR_RESULT_TEMA_IN;
#define SZ_TR_RESULT_TEMA_IN  sizeof(TR_RESULT_TEMA_IN)

typedef struct
{
	int		nTrIndex;

	char    sJongCnt       [4];        /* �˻������                       */
	char    sHigh          [4];        /* ���� �����                      */
	char    sUp            [4];        /* ��� �����                      */
	char    sNoch          [4];        /* ���� �����                      */
	char    sDown          [4];        /* �϶� �����                      */
	char    sLow           [4];        /* ���� �����                      */
	char    sUpRate        [6];        /* ��º���                         */
	char    sChgrateAvg    [6];        /* ��յ����                       */
} TR_RESULT_HEADER;
#define SZ_TR_RESULT_HEADER  sizeof(TR_RESULT_HEADER)

typedef struct
{
    char    sMarket       [1];        /* �屸��                           */
    char    sCode         [6];        /* �����ڵ�                         */
    char    sName		  [20];       /* �����                           */
    char    sSignCnt      [3];        /* ������Ӻ���                     */
    
    char    sPrice        [8];        /* ���簡                           */
    char    sSign         [1];        /* �����ȣ                         */
    char    sChange       [8];        /* ���ϴ��                         */
    char    sChgRate      [6];        /* ���ϴ�� �����                  */
    char    sVolume       [9];        /* �ŷ���                           */
    char    sVolumeRate   [6];        /* �ŷ��� ���ϴ����                */
	char	sFiller		  [4];
	// �����߰�
	char  gdResultField   [960];	  /*  ����ʵ� (16 * 60 = 960)    */
} TR_RESULT_LIST;
#define SZ_TR_RESULT_LIST    sizeof(TR_RESULT_LIST)
 
//# ���� ����Ʈ ��ȸ!
typedef struct
{
	int			nTrIndex;

	char        sCode[6];              /* ��ȸ��� ����                    */
	char        sFiller[2];
	int         nConditionCnt;         /* �˻����� ��                      */
	char        sRankGubun[CONDITION_MAXCNT];     //# Add : 2006.02.10 
	char        sFiller2[2];
	//User_Finder stCondition[50];      /* �˻�����                        */
} TR_LIST_IN;
#define SZ_TR_LIST_IN  sizeof(TR_LIST_IN)

typedef struct
{
	int		nTrIndex;
	int     nConditionCnt;             /* �˻� ���Ǽ�                      */
  //char  nCondiIndexList[50];       /* �˻� ���� Index List             */
} TR_LIST_OUT;
#define SZ_TR_LIST_OUT sizeof(TR_LIST_OUT)

//# �׸� ����Ʈ ��ȸ
typedef struct
{
	int         nTrIndex;            /* Ŭ���̾�Ʈ ��ȸ ����        */

	char        sCode[6];            /* ��ȸ��� ����               */
	char        sFilterGubun;        /* ���: 1, �̻��: 0          */
	User_Finder stFilter;            /* ��������                    */
} TR_LIST_TEMA_IN;
#define SZ_TR_LIST_TEMA_IN sizeof(TR_LIST_TEMA_IN)

typedef struct
{
	int         nTrIndex;						/* Ŭ���̾�Ʈ ��ȸ ���� */
	int         nCnt;								/* �׸� �׸� ��			    */
} TR_LIST_TEMA_OUT_HEADER;
#define SZ_TR_LIST_TEMA_OUT_HEADER sizeof(TR_LIST_TEMA_OUT_HEADER)

typedef struct
{
  char        sCode[4];						/* �׸� �ڵ�			    */
	char        sName[36];					/* �׸� ��						*/
} TR_LIST_TEMA_OUT;
#define SZ_TR_LIST_TEMA_OUT sizeof(TR_LIST_TEMA_OUT)



typedef struct tagREAL_USER_IN
{
  char    sType;          /* S : SYS, U : USER                    */
  char    sFlag;          /* ���� ���:'E',����:'C',����:'P',�˻�:'R',����:'D'*/
  char    sAlertNum[4];   /* ��Ͻÿ� space��...                      */
  char    sSendCnt[2];    /* ���� ��ȣ �뺸Ƚ��                       */
  User_Finder stUserFinder;
  char    sUserID[20];    /* ����� ID                                */
} REAL_USER_IN;
#define SZ_REAL_USER_IN  sizeof(REAL_USER_IN)


typedef struct tagREAL_USER_OUT
{
  char    sType;        /* S : SYS, U : USER                    */
  char    sFlag;        /* ���е��:'E',����:'C',����(����):'P(S)',����:'D' */
  char    sResultFlag;  /* ����:'s', ��aa7788��:'E', ������ʰ�:'F'   */
  char    sTime[6];     /* �ð�(HHMMSS)                         */
  char    sAlertNum[4];
  
  char	  sPBID[8];		/*���� PBID*/
  char	  sDelTRid[12]; /*���� TRrid*/
  
  char    sErrMsg[40];  /* ���н� �޽���                        */
} REAL_USER_OUT;
#define SZ_REAL_USER_OUT  sizeof(REAL_USER_OUT)

//DLL TO Main��ȣ������
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

