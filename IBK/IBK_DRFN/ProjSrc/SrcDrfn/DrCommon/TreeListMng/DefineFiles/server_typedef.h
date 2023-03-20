#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

/************************************************************/
/*																													*/
/*	One Clk Condition Server =>  STRUCT/DEFINE/TYPEDEF ����	*/
/*																													*/
/*					2005. 1.  			Doori														*/
/*					�ۼ��� : �쵿��																	*/
/************************************************************/

#define CONDITION_MAXCNT		50

#define TRNO_CONDIONECLK_LIST			"p0823"//"p0623"			//"hfit_o071006"
//#define TRNO_CONDIONECLK_RESULT			"d1022"//"p0622"			//"hfit_o071007"
#define TRNO_TEMACLK_LIST				"p0828"//"p0628"			//"hfit_o71008		"37006"
//#define TRNO_TEMACLK_RESULT				"37007"
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
	int		giCallIndex;

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
	char        sName[20];					/* �׸� ��						*/
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
  char    sErrMsg[40];  /* ���н� �޽���                        */
} REAL_USER_OUT;
#define SZ_REAL_USER_OUT  sizeof(REAL_USER_OUT)


#endif // SERVER_TYPEDEF

