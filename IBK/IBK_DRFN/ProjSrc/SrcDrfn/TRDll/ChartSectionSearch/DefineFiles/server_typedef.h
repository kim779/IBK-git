#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

/************************************************************/
/*																													*/
/*	  Draw Condition Server =>  STRUCT/DEFINE/TYPEDEF ����	*/
/*																													*/
/*					2008. 1.  			Doori														*/
/*					�ۼ��� : �쵿��																	*/
/************************************************************/

//#define TRNO_DRAW_RESULT		"50011"
//#define TRNO_UNIVERS				"07608"
#define TRNO_THEMEGROUP				"HFIBOA076220" //�׸� �׷� ��ȸ
#define TRNO_THEMEITEM				"HFIBOA076210" //�׸� ���� ��ȸ


#define	RESULT_MAX_OFFSET	1

#define LEN_CODE					7
#define LEN_NAME					20
#define	RESULT_FIELD_MAX	30	/* ����ʵ� �ִ� ���� 	*/
#define RESULT_MAX			4000	//3000 -> 4000

#define CNT_DRAW_VAL			120

#define DEFAULTCNTATONCE (400)
#define GRDMX			(3000)
#define CHTMX			(5200)
#define CHTMXREQUEST	(5000)
#define BOJO_MSG_LENGTH	(256)

#define	RESULT_FIELD_MAX			30		/* ����ʵ� �ִ� ���� 	*/
#define RESULT_FIELD_MAX_EUGENE     240
#define CNT_DRAW_VAL_EUGENE			480
#define	COUNT_CANDLE_DATA			5

typedef struct tagDRAW_DATA
{
    long        lAccuVal;               // �ֱ� ����ġ ���뿩��
                                        //  : 1~ 10, ������� 0����.. (0.005)
    long        lDraw_Cnt;              // �׸��� �Ⱓ����(20~120)
    long        lErrRate;               // ������ = 10
    long        lGapRate;               // ���� ���� = 40
    long        lDraw_Value[CNT_DRAW_VAL];       // �׸��� ��ǥ��(0~100)
                                        //  : �׸��� ������� 999�� �Է�
} DRAW_DATA;
#define SZ_DRAW_DATA sizeof(DRAW_DATA)

typedef struct tagDRAW_IN
{
    long nTrIndex;
    
    long nS_Time;           // ���ð�
                            /* 0:�ϰ�, 1:�ְ�, 2:����, 3:1��, 4:3�� */
							/* 5:5��,  6:10��, 7:15��, 8:30��, 9:60��*/
                            //  : ��/��/�� ��.. ���ǰ˻��� ����
    long lTarget;           // ������� => 4 ����
                            //   : ��,��,��,��,�ŷ���(1,2,3,4,5) 
    long lMoveAvg;          // ����Ⱓ => 1����
                            //   : ���� ������� 1 �Է�
    long lS_Pos;            // �˻� ������ġ
    long lS_Range;          // �˻� ����
    long lSimilar;          // ���絵
    char cBoxFlag;					// Ⱦ�� üũ ���� ( > 5 )
														//  : Ⱦ�� üũ�ÿ� 1�� üũ

    long nFieldEditerCnt;								// �ʵ����� ����   [Fix]
    long nFieldList[RESULT_FIELD_MAX];	// �ʵ����� ��ȣ����Ʈ [Just Fix]
	      
	char  sTargetBit[RESULT_MAX];  /* �˻� ��� ����  */
    DRAW_DATA stDrawData;
} DRAW_IN;

#define SZ_DRAW_IN  sizeof(DRAW_IN)

typedef struct tagDRAW_OUT_HEAD
{
    long nTrIndex;
    
    long  lTotalCnt;			/* �˻��� ���� ���� ��      */
    long  lOutFieldCnt;				/* Out Field Count          */
    long  lOutListPacketSize;	/* ��� List ������(������) */
    long  lFindTime;					/* �˻��ð�									*/
} DRAW_OUT_HEAD;

#define SZ_DRAW_OUT_HEAD sizeof(DRAW_OUT_HEAD)


typedef struct tagDRAW_OUT_LIST
{
  char        sJmcode     [LEN_CODE];
  char        Filler1     [ 1];

  char        sHname      [20];
  char        sSign       [ 1];
  char        sMarket     [ 1];
  char        Filler2     [ 2];

  long        iTarget;    /* ���ֿ���     */

  char        sDateTime_Start [14];/* �˻������ ��������¥ YYYYMMDDhhmmss */
  char        sDateTime_End   [14];/* �˻������ ���۳�¥  */

  double      dFieldData[RESULT_FIELD_MAX];
} DRAW_OUT_LIST;

#define SZ_DRAW_OUT_LIST sizeof(DRAW_OUT_LIST)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#pragma pack(1)  // struct member alignment 1byte
typedef struct
{
    int     iHigh;
    int     iClose;
    int     iOpen;
    int     iLow;
	int		iBodyRate;// (-)�� ����, (+)���̸� �̻�
} CANDLE_IN_DATA;

typedef struct
{
    int     nTrIndex;
    int     iS_Time;						// ���ð� - ��/��/�� ��.. ���ǰ˻��� ����
    int     iS_Range;						// �˻� ����[1~5]
    int     iSimilar;						// ���絵

    int    FieldEditerCnt;                  // �ʵ����� ����   [Fix]
    int    FieldList[RESULT_FIELD_MAX];     // �ʵ����� ��ȣ����Ʈ [JustFix]

    int     iPreGap;						// ���� ��( X���� Data ��ȸ)
    int     iPreTrand;						// ���� �߼� ����
    int     iIgnoreColor;					// ������ ���� ����
	int		iIgnoreTail;					// ���� ����
	int		iApplyBodyRate;					// ������� ����

    char    sTargetBit[RESULT_MAX];         // �˻� ��� ����
	
    CANDLE_IN_DATA      tCandleData[5];		// ĵ�� ������
} CANDLE_IN;
#pragma pack()

typedef struct
{
    int    nTrIndex;

    int    lTotalCnt;						// �˻��� ���� ���� ��
    int    lOutFieldCnt;                    // Out Field Count
    int    lOutListPacketSize;              // ��� List ������(������)
    int    lFindTime;                       // �˻��ð�

	char   sDateTime_Start  [14];			// �˻������ ��������¥ YYYYMMDDhhmmss
	char   sDateTime_End    [14];			// �˻������ ���۳�¥
} CANDLE_OUT_HEAD;

typedef struct
{
    char    sJmcode         [ 7];
    char    Filler1         [ 1];

    char    sHname          [20];
    char    sSign           [ 1];
    char    sMarket         [ 1];
    char    sS_Time			[ 2];

//  int     iTarget;                /* ���ֿ���     */
	char        sDateTime_Start [14];/* �˻������ ��������¥ YYYYMMDDhhmmss */
	char        sDateTime_End   [14];/* �˻������ ���۳�¥  */

    double  dFieldData      [RESULT_FIELD_MAX];
} CANDLE_OUT_LIST;
// 2009.03.24   sashia.lee  ------------------------------------

//////////////////////////////////////////////////////////////////////////

// #define LEN_CODE			7
// #define LEN_NAME			20
// 
// #define TRNO_UNIVERS				"07608"
// 
// typedef struct tagUNIVERS_IN
// {
// 	char	rcmd_type[	2];				/* �������� '00'			*/
// } UNIVERS_IN;
// #define SZ_UNIVERS_IN sizeof(UNIVERS_IN)
// 
// typedef struct tagUNIVERS_ITEM
// {
// 	  char    cMarket[1];				/* �屸��                   */
// 		char    jcode[LEN_CODE];	/* �����ڵ�                 */
//     char    jname[LEN_NAME];	/* �����                   */
//     char    rcmd_type[10];		/* ��������                 */
// } UNIVERS_ITEM;
// #define SZ_UNIVERS_ITEM sizeof(UNIVERS_ITEM)
// 
// typedef struct tagUNIVERS_OUT
// {
// 	char		sCount		[	6];
// 	char		sMessage	[	5];
// 	UNIVERS_ITEM	stUniversItem[300];
// } UNIVERS_OUT;
// #define SZ_TREEINFO_OUT sizeof(TREEINFO_OUT)


// typedef struct tagMSTCode
// {
//   char    sMarket[1];
//   char    sCode[7];
//   char    sName[20];
//   char    sSetPos[4];
// } MSTCode;

typedef struct tagMSTCode
{
	char    sMarket[1];
	char    sCode[7];
	char    sName[20];
	char    sSetPos[4];
	char    sUsjg[1];               /* �켱��   (0:�ش����,1:�ش�) */
	char    sUnder[1];              /* ��������                     */
	char    sStop[1];               /* �ŷ���������                 */
	char    sDishonest[1];          /* �Ҽ��ǰ�������               */
	char    sConfidencelimit[1];    /* �ſ���������?               */
	char    sJrmm[1];          	    /* �����Ÿ�����                 */
	char    sDrat[3];               /* ���űݺ���                   */
	char    sMrktalrm[1];           /* ����溸
										1:��������, 2:���ڰ��
										3:��������, 4:�������迹��*/
	char	sMonth[2];				/* ����						*/
	char	sETF[1];				/* ETF							*/
} MSTCode;


#define COND_MST_VER	1


typedef struct _S03010_I
{
	char    shcode		[ 15];  /* �����ڵ�        */
	char    jangGB		[ 1];                            /* ���屸��(1)                     */
	char	date		[ 8];  /* �˻� ������       */
	char    eDate		[ 8];  /* �˻� ������ */
	char	num			[ 4];  /* �˻��� ������ ����     */
	char	unit		[ 3];  /* ƽ/��/�� ��ȸ���ݴ���    *///<========2005.08.11
	char    button		[ 1];  /* ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5) */
	char    modGb		[ 1];  /* �����ְ� ���� 1:�����ְ��ݿ�   */ 
	char	nflag		[ 1];  /* ����(1) / ��ȸ(0) ������    */
	char	nkey		[512];  /* ���� / ���� Fetch Key    */
	char	nDataGubun	[ 1];  /* '0': ����, '1':����     */
	char    ImgRgb		[ 1];  /* '0':����� '1':�Ǻ�     */
	char    Irecordgb	[ 1];  /* ���� ���а�(1)      *///<========2005.08.11
	char    nDayBefore	[ 2];  /* ���� ���� n����       *///<========2005.10.14
	char    Itime		[ 6];  /* ����ð� (hhmmss)     *///<========2005.08.11  
	char        nowData[1]; /* ������ȸ(1:������ȸ) */
	/// don't sent to server..
	char chNull;
} S03010_I;
#define S03010_I_SIZE	sizeof(S03010_I)
//--------------------------------[OUTPUT�ݺ� ]---------------------------------------//

// Chart Equity History Info Structure(In Server)
typedef struct _S_03010_S
{
	char    date[8];                              /* ��¥(8)                         */
	char    open[10];                             /* �ð�(10)                        */
	char    high[10];                             /* ��(10)                        */
	char    low[10];                              /* ����(10)                        */
	char    close[10];                            /* ����(10)                        */
	char    volume[10];                           /* �ŷ���(10)                      */
	char    rights[1];                            /* �Ǹ���(1)                       */
} S03010_S;


// Chart Equity History Info Structure (In Client)
typedef struct _S03010_CHART
{
	char		tmp		[ 5];				/* Chart Count      */
	char		bojomsg	[BOJO_MSG_LENGTH];  /* Chart Bojo Data  */
	S03010_S	chart	[CHTMX];
} S03010_CHART;

//--------------------------------[OUTPUT     ]-----------------------------------//
// Chart Equity  Info Structure (In Server's Output )
typedef struct _S03010_O 
{
	char    name[20];                             /* �����(20)                      */
	char    price[10];                            /* ���簡(10)                      */
	char    sign[1];                              /* �����ȣ(1)                     */
	char    change[10];                           /* ���(10)                        */
	char    chrate[6];                            /* �����(6)                       */
	char    volume[10];                           /* �ŷ���(10)                      */
	char    preprice[10];                         /* ���ذ�(10)                      */
	char	sellprice[10];						// �ֿ켱 �ŵ�ȣ��
	char	buyprice[10];						// �ֿ켱 �ż�ȣ��
	char	prevolume[18];						// ���ϰŷ���
	char	prevolchrate[18];					// ���ϰŷ��� ���
	char	volcycle[11];						// �ŷ���ȸ����
	char	value[18];							// �ŷ����
	char	defprice[11];						// �׸鰡
	char	stockcount[18];						// �����ֽ�
	char	defvalue[22];						// �ں���
	char	totalprice[18];						// �ð��Ѿ�
	char	PER[11];							// PER
	char	EPS[14];							// EPS
	char	PBR[11];							// PBR
	char    nkey[512];                     /* ����Ű(14)                      */
	char    tmp[5];                              /* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[400];                         /* �����޽���(400)                 */
// 	char    name[20];                             /* �����(20)                      */
// 	char    price[10];                            /* ���簡(10)                      */
// 	char    sign[1];                              /* �����ȣ(1)                     */
// 	char    change[10];                           /* ���(10)                        */
// 	char    chrate[6];                            /* �����(6)                       */
// 	char    volume[10];                           /* �ŷ���(10)                      */
// 	char    preprice[10];                         /* ���ذ�(10)                      */
// 	char    nkey[512];                             /* ����Ű(14)                      */
// 	char    tmp[5];                              /* ��Ʈī��Ʈ(5)                   */
// 	char    bojomsg[400];                         /* �����޽���(400)                 */
// 	S03010_S  chart[CHTMX];
} S03010_O;


/* OutRec2:                                                                          */
// ������� �ʴ� Field �ּ�ó��
//char EU_p0602_OutRec2_count[4];


typedef struct
{
	char		nTrIndex[4];
	char		iS_Time[4];
	char		iTarget[4];
	char		iMoveAvg[4]; 
	char		iS_Pos[4];
	char		iS_Range[4];
	char		iSimilar[4];
	char		sBoxFlag[1];
	char		FieldEditerCnt[4];
	char		FieldList[RESULT_FIELD_MAX_EUGENE];
	char		sTargetBit[RESULT_MAX];
	char		iAccuVal[4];
	char		iDraw_Cnt[4];
	char		iErrRate[4];
	char		iGapRate[4];
	char		iDraw_Value[CNT_DRAW_VAL_EUGENE];	

}p0624InBlock;
#define SZ_p0624InBlock sizeof(p0624InBlock)

typedef struct
{
	char		nTrIndex[4];
	char		lTotalJongCnt[4];
	char		lOutFieldCnt[4];
	char		lOutListPacketSize[4];
	char		lFindTime[6];
}p0624OutBlock1;
#define SZ_p0624OutBlock1	sizeof(p0624OutBlock1)

typedef struct
{
	char            iHigh[8];               /*      ��            */
	char            iClose[8];              /*      ����            */
	char            iOpen[8];               /*      �ð�            */
	char            iLow[8];                /*      ����            */
	char            iBodyRate[8];           /*      �����          */
} HKST03660000_IN2;

typedef struct
{
	char    nTrIndex[4];                    /* �ݱ���                       */
	char    iS_Time[4];                     /* ���ð�                     */
	char    iS_Range[4];                    /* �˻�����                     */
	char    iSimilar[4];                    /* ���絵                       */
	char    FieldEditerCnt[4];              /* �ʵ���������                 */
	char    FieldList[240];                  /* �ʵ�������ȣ����Ʈ           */
	char    iPreGap[4];                     /* ���� ��( X���� Data ��ȸ)    */
	char    iPreTrend[4];                   /* ���� �߼� ����                       */
	char    iIgnoreColor[4];                /* ������ ���� ����.                    */
	char    iIgnoreTail[4];                 /* ���� ����.                           */
	char    iApplyBodyRate;                 /* ����� ���� ����.    1:����, order:�������� ����.    */
	char    sTargetBit[RESULT_MAX];         /* �˻� ��� ����               */	//3000 -> 4000

	HKST03660000_IN2                        data[5];                /* */
} HKST03660000_IN1;

typedef struct
{
	char    nTrIndex[4];                /* �ݱ���                       */
	char    lTotalJongCnt[4];           /* �˻������                   */
	char    lOutFieldCnt[4];            /* OutFieldCount                */
	char    lOutListPacketSize[4];      /* ���List������               */
	char    lFindTime[6];               /* �˻��ð�                     */
} HKST03660000_OUT1;

typedef struct
{
	char    sJmcode[7];                 /* �����ڵ�                     */
	char    sHname[20];                 /* �����                       */
	char    sSign[1];                   /* �����ȣ                     */
	char    sMarket[1];                 /* ���屸��                     */
	char    sS_Time[2];
	char    sStartDateTime[14];         /* �˻��� ���۽ð�          */
	char    sEndDateTime[14];           /* �˻��� ����ð�          */
	char    dFieldData[60][16];             /* �����ʵ�����               */
} HKST03660000_OUT2;

typedef struct 
{
	char		sJmcode[7];
	char		sHname[20];
	char		sSign[1];
	char		sMarket[1];
	char		iTarget[4];
	char		sDateTime_Start[14];
	char		sDateTime_End[14];
	//	char		dFieldData[320];
	char		dFieldData[960];
}p0624OutBlock2;
#define SZ_p0624OutBlock2	sizeof(p0624OutBlock2)

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

