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
//#define TRNO_MSTCODE        "50008"

#define	RESULT_MAX_OFFSET		1
#define RESULT_MAX				4000	//3000 -> 4000

#define QUERY_STRJONGMOK_MIN		_T("HKST03010200")

#define LEN_CODE					7
#define LEN_NAME					20

#define	RESULT_FIELD_MAX			30		/* ����ʵ� �ִ� ���� 	*/
#define RESULT_FIELD_MAX_EUGENE     240

#define CNT_DRAW_VAL				120
#define CNT_DRAW_VAL_EUGENE			480

#define DEFAULTCNTATONCE (400)
#define GRDMX			(3000)
#define CHTMX			(5200)
#define CHTMXREQUEST	(5000)
#define BOJO_MSG_LENGTH	(400)

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





/*  -------------------------------------------------------------
** 2009.03.24   sashia.lee
** Candle �˻� Packet
*/
#pragma pack(1)  // struct member alignment 1byte
#define		COUNT_CANDLE_DATA 5
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

	CANDLE_IN_DATA      tCandleData[COUNT_CANDLE_DATA];		// ĵ�� ������
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
	char    sTargetBit[4000];         		/* �˻� ��� ����               */	//3000 -> 4000

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
	char    dFieldData[60][16];         /* �����ʵ�����               */
} HKST03660000_OUT2;




typedef struct _S03010_I
{
	char    shcode		[15];		/* �����ڵ�								*/
	char    jangGB		[ 1]; 
	char	date		[8];		/* �˻� ������							*/
	char	eDate		[8];		/* �˻� ������							*/
	char	num			[4];		/* �˻��� ������ ����					*/
	char	unit		[3];		/* ƽ/��/�� ��ȸ���ݴ���				*///<========2005.08.11
	char    button		[1];		/* ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)	*/
	char    modGb		[1];		/* �����ְ� ���� 1:�����ְ��ݿ�			*/	
	char	nflag		[1];		/* ����(1) / ��ȸ(0) ������ 			*/
	char	nkey		[512];		/* ���� / ���� Fetch Key				*/
	char	nDataGubun	[1];		/* '0': ����, '1':����					*/
	char    ImgRgb		[1];        /* '0':����� '1':�Ǻ�					*/
	char    Irecordgb	[1];		/* ���� ���а�(1)						*///<========2005.08.11
	char    nDayBefore	[2];		/* ���� ���� n���� 						*///<========2005.10.14
	char    Itime		[6];		/* ����ð� (hhmmss)					*///<========2005.08.11 
	char    nowData[1]; /* ������ȸ(1:������ȸ) */
	/// don't sent to server..
	char	chNull;
} S03010_I;
#define S03010_I_SIZE	sizeof(S03010_I)
//--------------------------------[OUTPUT�ݺ� ]---------------------------------------//

// Chart Equity History Info Structure(In Server)
typedef struct _S_03010_S
{
	char    date[8];			/* ��¥     		*/
	char    open[10];			/* �ð�     		*/
	char    high[10];			/* ��     		*/
	char    low[10];			/* ����     		*/
	char    close[10];			/* ����   			*/
	char    volume[10];     	/* �ŷ���   		*/
	char	rights[1];			/* ����, �Ǹ���..	*/
	//	char	rightsrate[6];			/* ����, �Ǹ���..	*/
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
// 	char	name[20];			/* �����			*/
// 	char    price[10]; 		    /* ���簡   		*/
// 	char    sign[1];                              /* �����ȣ(1)                     */
// 	char    change[10];     	/* ���     		*/
// 	char    chrate[6];      	/* �����   		*/
// 	char    volume[10];     	/* �ŷ���   		*/
// 	char    preprice[10];                         /* ���ذ�(10)                      */
// 	char    nkey[512];           /* ����Ű           */
// 	//------------- PosForChart
// 	char    tmp[5];         	/* Chart Count      */
// 	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256���� �ٲ� 05.05.06*/
// 	S03010_S  chart[CHTMX];
} S03010_O;


typedef struct    
{
	char	shcode		[15];	/* �����ڵ�			*/
	char	date		[8];	/* �˻�������		*/
	char	num			[4];	/* ��ȸ�Ǽ�			*/
	char	unit		[3];	/* ��ȸ����(ƽ/��/��)								*/	
	char	button		[1];	/* ��ȸ����(ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5))	*/
	char	modGb		[1];	/* �����ְ�(1:�����ְ��ݿ�)							*/
	char	nflag		[1];	/* ��ȸ���(����(1) / ��ȸ(0) ������)				*/
	char	nkey		[21];	/* �˻�Ű(���� / ���� Fetch Key)					*/
	char	nDataGubun	[1];	/* ���ݱ���('0': ����, '1':����)					*/
	char	ImgRgb		[1];	/* ������('0':����� '1':�Ǻ�)						*/
	char	Irecordgb	[1];	/* ���ⱸ�а�(1)	*/
	char	nDayBefore	[2];	/* �������n����	*/
	char	Itime		[6];	/* ����ð�(hhmmss) */
	char        nowData[1]; /* ������ȸ(1:������ȸ) */
	// don't sent to server..
	char	chNull;


}p0602InBlock;

typedef struct
{
	char	name		[20];	/* �����		0	*/
	char	jang		[10];	/* �屸��		1	*/
	char	price		[10];	/* ���簡		2	*/
	char	sign		[1];	/* �����ȣ		3	*/
	char	change		[10];	/* ���			4	*/
	char	chrate		[6];	/* �����		5	*/
	char	volume		[10];	/* �ŷ���		6	*/
	char	offer		[10];	/* �ŵ�ȣ��		7	*/
	char	bid			[10];	/* �ż�ȣ��		8	*/
	char	preprice	[10];	/* ���ذ�		9	*/
	char	nkey		[14];	/* ����Ű		10	*/
	char	cntsz		[6];	/* ��Ʈ������	11	*/
	char	fcnt		[5];	/* ��Ʈī��Ʈ	12	*/
	char	bojomsg		[400];	/* �����޽���	13	*/
}p0602OutBlock1;

typedef struct
{
	char	date		[8];	/* ��¥				*/
	char	open		[10];	/* �ð�				*/
	char	high		[10];	/* ��				*/
	char	low			[10];	/* ����				*/
	char	close		[10];	/* ����				*/
	char	volume		[10];	/* �ŷ���			*/
	char	value		[12];	/* �ŷ����			*/
	char	rights		[1];	/* �Ǹ���			*/
}p0602OutBlock2;


//////////////////////////////
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
	char		sJmcode[7];
	char		sHname[20];
	char		sSign[1];
	char		sMarket[1];
	char		iTarget[4];
	char		sDateTime_Start[14];
	char		sDateTime_End[14];
	char		dFieldData[320];
}p0624OutBlock2;
#define SZ_p0624OutBlock2	sizeof(p0624OutBlock2)
///////////////////////////////////////////////////////////


typedef struct
{
	char		giCallIndex[4];
	char		gsCode[6];
	char		gsFilterGubun[1];
}p0628InBlock;
#define SZ_p0628InBlock sizeof(p0628InBlock)

typedef struct
{
	char		giCallIndex[4];
	char		ThemeCnt[4];
}p0628OutBlock1;
#define SZ_p0628OutBlock1 sizeof(p0628OutBlock1)

typedef struct
{
	char		gsThemeCode[4];
	char		gsThemeName[36];
}p0628OutBlock2;
#define SZ_p0628OutBlock2 sizeof(p0628OutBlock2)

typedef struct
{
	char		giCallIndex[4];
	char		giThemeCode[4];
}p0630InBlock;
#define SZ_p0630InBlock sizeof(p0630InBlock)

typedef struct
{
	char		giCallIndex[4];
	char		giCnt[4];
}p0630OutBlock1;
#define SZ_p0630OutBlock1 sizeof(p0630OutBlock1)

typedef struct
{
	char		gsItemCode[6];
	char		gsMarketFlag[1];
}p0630OutBlock2;
#define SZ_p0630OutBlock2 sizeof(p0630OutBlock2)

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

