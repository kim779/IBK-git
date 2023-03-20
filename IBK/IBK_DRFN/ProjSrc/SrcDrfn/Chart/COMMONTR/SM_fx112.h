/*************************************************************************************/
/*  1. ���α׷�ID : SM_fx112#                                                       */
/*  2. ���α׷��� : Chart                                                            */
/*  3. ���񽺸�   : SM_fx112#                                                       */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : kmlee98                                                          */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_SM_fx112_H__
#define __EU_TRAN_SM_fx112_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define SM_fx112                     "SM_fx112#"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char		keyvalue[84];		/* key ����+�ð�	: key ����+�ð� */
	char		jmcode[10];			/* �����ڵ�			: �����ڵ� */
	char		date[8];			/* ����				: ���� */
	char		chetime[10];		/* �ð�				: �ð� */
	char		gubun[1];			/* ����				: 0:ó��1:���� */
	char		dgubun[1];			/* �����ͱ���		: 1:Tick,2:��,3:��,4:��,5:�� */
	char		nsel[4]/*[2]*/;			/* N�б���			: 0:30sec,1:1M,2:2M,... */
	char		mgubun[1];			/* �ŵ��ż�����		: 0:�ŵ�,1:�ż�,2:��ü	... */
	char		dcnt[5];			/* �����ͰǼ�		: �����ͰǼ� */
}SM_fx112_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/
//typedef struct _SM_fx112_OUT_HEADER
//{
//	char		keyvalue[18];		/* key ����+�ð�	: key ����+�ð� */
//	char		count[4];
//} SM_fx112_OUT_HEADER;
//#define SM_fx112_OUT_SZ sizeof(SM_fx112_OUT_HEADER)

/* OutRec1:                                                                          */
typedef struct
{
    char    price[10];                            /* ���簡(10)                      */
    char    sign[1];                              /* �����ȣ(1)                     */
    char    change[10];                           /* ���(10)                        */
    char    chrate[6];                            /* �����(6)                       */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    volume[10];                           /* �ŷ���(10)                      */
    char    preprice[10];							/* ��������(10)                      */
	
    char    preopen[10];							/* ���Ͻð�(10)                      */
    char    prehigh[10];							/* ���ϰ�(10)                      */
    char    prelow[10];								/* ��������(10)                      */
	
}SM_fx112_OutRec1_Sub;


typedef struct
{
    char					name[20];                           /* �����(20)                       */
    char					codetype[1];                        /* ���񱸺�(1)                      */
    char					startDate[10];                      /* ��ȸ������                      */
    char					endDate[10];						/* ��ȸ������                      */
    char					nkey[ChartKeySize];                 /* ����Ű(14)                      */

    char					piplowest[6];                       /* �Ҽ��� ��ȿ�ڸ�(6)( 5�ڸ�:100000, 3�ڸ�:1000)              */
	char					mgubun[1];							/* �ŵ��ż����� : 0:�ŵ�,1:�ż�,2:��ü		... */

	SM_fx112_OutRec1_Sub	fx112OutRec1_S;						/* �ŵ�								*/
	SM_fx112_OutRec1_Sub	fx112OutRec1_B;						/* �ż�								*/
	
}SM_fx112_OutRec1;


#define SM_fx112_OUT1_SZ sizeof(SM_fx112_OutRec1)

/* OutRec2:                                                                          */
// char SM_fx112_OutRec2_count[4];
typedef struct
{
	// 2011.01.18 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
    //char	date[8];                              /* ��¥(8)                         */
	char	date[14];                             /* ��¥(14)                        */
	// 2011.01.18 by SYS <<
	//
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
//} SM_fx112_OutRec2_Sub;
} SM_fx112_OutRec2;

//typedef struct
//{
//	SM_fx112_OutRec2_Sub	fx112OutRec2_S;						/* �ŵ�								*/
//	SM_fx112_OutRec2_Sub	fx112OutRec2_B;						/* �ż�								*/
//
//} SM_fx112_OutRec2;

#define SM_fx112_OUT_KEY_SZ		4	

//typedef struct
//{
//    char    szDate[14];                           /* ����(14)                        */
//    char    szBid[10];                            /* �ż���(bid)(10)                 */
//    char    szAsk[10];                            /* �ŵ���(ask)(10)                 */
//    char    szBidOpen[10];                        /* �ð�(10)                        */
//    char    szBidHigh[10];                        /* ��(10)                        */
//    char    szBidLow[10];                         /* ����(10)                        */
//    char    szBidClose[10];                       /* ����(10)                        */
//    char    szFiller[2];                          /* Filler(2)                       */
//}SM_fx112_OutRec2;

/* Chart Data:																		 */
typedef struct _SM_fx112_CHART
{
	char				fcnt[5];         			/* ��Ʈī��Ʈ(5)                   */
	char				bojomsg[500];				/* �����޽���(400)                 */
	char				chart[1];					// ����ũ����.
//	SM_fx112_OutRec2	chart[1];			// ����ũ����.
} SM_fx112_CHART;


#define SM_fx112_OUT2_SZ sizeof(SM_fx112_OutRec2)

/*************************************************************************************/

#endif

