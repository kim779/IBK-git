/*************************************************************************************/
/*  1. ���α׷�ID : EU_p0602                                                         */
/*  2. ���α׷��� : ��Ʈ ��ȸ(�ֽ�)                                                  */
/*  3. ���񽺸�   : EU_p0602                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_p0602_H__
#define __EU_TRAN_EU_p0602_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define EU_TRAN                       "EU_p0602"
//#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

#pragma pack(push, 1)
/* InRec1:                                                                           */
typedef struct
{
	char		szMemberNo[3];		/* ȸ���� ID : ȸ���� ID */
	char		szTradeType[2];		/* ���� ���� - �ŷ�����(00 : 10��, 01:1��) */
	char		szDealerGrpID[3];	/* ���� �׷� �ڵ� : ���� �׷� �ڵ� */
	char		szCurNo[12];		/* ���� �ڵ� : ���� �ڵ� */
	char		szTermDiv[1];		/* �Ⱓ ���� : �Ⱓ ���� */
	long		nMinute;			/* �� : �� */
	char		szDate[8];			/* ���� ���� : ���� ���� */
	char		szTime[6];			/* ���� �ð� : ���� �ð� */
	long		nDataCnt;			/* ������ ���� : ������ ���� */
	char		szPOCode[5];		/* �ż� 079, �ŵ� 081 */

} EU_T9726_INREC1;

typedef struct _EU_T9726_OUT_SINGLE
{
	char		szTradeType[2];		/* ���� ���� - �ŷ�����(00 : 10��, 01:1��) */
	char		SHCODE[12];			/* ���� �ڵ� : ���� �ڵ� */
	char		cTermDiv[1];		/* �Ⱓ ���� : �Ⱓ ���� */
	char		cPipLowest[1];		/* �Ҽ��� ��ȿ�ڸ��� */
} EU_T9726_OUT_SINGLE;

#define EU_T9726_OUT_SINGLE_SZ sizeof(EU_T9726_OUT_SINGLE)

typedef struct _EU_T9726_OUT_ARR
{
	char		szDate[8];			/* ���� : ���� */
	char		szTime[6];			/* �ð� : �ð� */
	char		szBid[10];			/* �ż���(bid) : �ż���(bid) */
	char		szAsk[10];			/* �ŵ���(ask) : �ŵ���(ask) */
	char		szBidOpen[10];		/* �ð� : �ð� */
	char		szBidHigh[10];		/* �� : �� */
	char		szBidLow[10];		/* ���� : ���� */
	char		szBidClose[10];		/* ���� : ���� */
	char		szAmount[10];		/* �ŷ��� : �ŷ��� */
	long		nInfoCnt;			/* �ش����� ���� �Ǽ� : �ش����� ���� �Ǽ� */
	long		nNewsCnt;			/* �ش����� ���� �Ǽ� : �ش����� ���� �Ǽ� */
	long		nOrdCnt;			/* �ش����� �ֹ� �Ǽ� : �ش����� �ֹ� �Ǽ� */
	long		nExecutedCnt;		/* �ش����� ü�� �Ǽ� : �ش����� ü�� �Ǽ� */
	long		nModCnt;			/* �ش����� ������� �Ǽ� : �ش����� ������� �Ǽ� */
} EU_T9726_OUT_ARR;

#define EU_T9726_OUT_ARR_SZ sizeof(EU_T9726_OUT_ARR)


typedef struct _EU_T9726_OUT_KEY
{
    char    display_cnt[ 4];		/* ��ȸ ����Ʈ COUNT    */
}EU_T9726_OUT_KEY;

#define EU_T9726_OUT_KEY_SZ sizeof(EU_T9726_OUT_KEY)


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
// ������� �ʴ� Field �ּ�ó��
typedef struct
{
    char    name[20];                             /* �����(20)                      */
    char    price[10];                            /* ���簡(10)                      */
    char    sign[1];                              /* �����ȣ(1)                     */
    char    change[10];                           /* ���(10)                        */
    char    chrate[6];                            /* �����(6)                       */
    char    preprice[10];                         /* ���ذ�(10)                      */
    char    nkey[14];                             /* ����Ű(14)                      */
    char    fcnt[5];                              /* ��Ʈī��Ʈ(5)                   */
    char    bojomsg[300];                         /* �����޽���(300)                 */
} EU_p0602_OutRec1;

typedef struct
{
    char    name[20];                             /* �����(20)                      */
    char    price[10];                            /* ���簡(10)                      */
    char    sign[1];                              /* �����ȣ(1)                     */
    char    change[10];                           /* ���(10)                        */
    char    chrate[6];                            /* �����(6)                       */
    char    volume[10];                           /* �ŷ���(10)                      */
    char    preprice[10];                         /* ���ذ�(10)                      */
    char    nkey[14];                             /* ����Ű(14)                      */
    char    fcnt[5];                              /* ��Ʈī��Ʈ(5)                   */
    char    bojomsg[300];                         /* �����޽���(300)                 */
} EU_p0602_OutRec1_EX;

/* OutRec2:                                                                          */
// ������� �ʴ� Field �ּ�ó��
//char EU_p0602_OutRec2_count[4];
typedef struct
{
    char    date[8];                              /* ��¥(8)                         */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
    char    volume[10];                           /* �ŷ���(10)                      */
    char    rights[1];                            /* �Ǹ���(1)                       */
} EU_p0602_OutRec2;

typedef struct
{
    char    date[8];                              /* ��¥(8)                         */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
} EU_p0602_OutRec2_COMP;

/* Chart Data:																		 */
typedef struct _EU_p0602_CHART
{
	char    fcnt[5];         			/* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[300];				/* �����޽���(300)                 */
	EU_p0602_OutRec2  chart[1];			// ����ũ����.
} EU_p0602_CHART;

typedef struct _EU_p0602_CHART_COMP
{
	char    fcnt[5];         			/* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[300];				/* �����޽���(300)                 */
	EU_p0602_OutRec2_COMP  chart[50000];
} EU_p0602_CHART_COMP;

/*************************************************************************************/

#define TR3930	"t3930"

typedef struct _EU_T3930_INREC
{
	char  szMemberNo[3];     /* ȸ���� */
	char  szTradeType[2];    /* �ŷ����� */
	char  szCustNo[17];      /* �����ID */
} EU_T3930_INREC;

typedef struct _EU_T3930_OUTREC
{
	char  szCurNo[12];      /* ����� */
	char  szSide[3];        /* �Ÿű��� - �ż�(079), �ŵ�(081) */
	char  szPrice[10];      /* ���� */
	char  szAmount[11];     /* ���� */

} EU_T3930_OUTREC;

typedef struct _Order_Info
{
	CString szCurNo;         /* ����� */
	CString	szSide;          /* �Ÿű��� - �ż�(079), �ŵ�(081) */
	CString	szPrice;         /* ���� */
	CString	szAmount;        /* ���� */

} ORDER_INFO;

#pragma pack(pop)

#endif

