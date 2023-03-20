/*************************************************************************************/
/*  1. ���α׷�ID : KB_p0606                                                         */
/*  2. ���α׷��� : ��Ʈ ��ȸ(�ؿ�)                                                  */
/*  3. ���񽺸�   : KB_p0606                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_p0606_H__
#define __KB_TRAN_KB_p0606_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_p0606"
//#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

#ifndef ChartKeySize
#define		ChartKeySize		28
#endif

/* InRec1:                                                                           */
typedef struct
{
	char	Shcode[16];							/* �����ڵ�							*/
	char	date[8];							/* �˻�������						*/
	char	num[4];								/* ��ȸ�Ǽ�							*/
	char	unit[3];							/* ����(Tick/Sec/Min/Day)			*/
	char	button[1];							/* ��ȸ���(ƽ:0/��:1/��:2/��:3/��:4)*/
	char	nflag[1];							/* ��ȸ����(����:1/��ȸ:0)			*/
	char	nkey[ChartKeySize];					/* ��ġŰ							*/
	char	nowData[1];							/* ������ȸ							*/
	char	timeGB[1];							/* �����ð�����(1: �ѱ��ð�, 0:�����ð�)*/
	char	dataType[1];						/* ��Ʈ���� (0:����, 1:����,2:�޷�)*/
} KB_p0606_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
// ������� �ʴ� Field �ּ�ó��
typedef struct
{
    char    name[40];                             /* �����(40)                      */
    char    price[10];                            /* ���簡(10)                      */
    char    sign[1];                              /* �����ȣ(1)                     */
    char    change[10];                           /* ���(10)                        */
    char    chrate[6];                            /* �����(6)                       */
    char    volume[10];                           /* �ŷ���(10)                      */
    char    offer[10];                            /* �ŵ�ȣ��(10)                    */
    char    bid[10];                              /* �ż�ȣ��(10)                    */
    char    preprice[10];                         /* ���ذ�(10)                      */
    //char    nkey[21];                             /* ����Ű(21)                      */
	char	nkey[ChartKeySize];
    char    cntsz[6];                             /* ��Ʈ������(6)                   */
    char    fcnt[5];                              /* ��Ʈī��Ʈ(5)                   */
    char    bojomsg[400];                         /* �����޽���(300)                 */
} KB_p0606_OutRec1;

typedef struct
{
    char    name[40];                             /* �����(40)                      */
    char    price[10];                            /* ���簡(10)                      */
    char    sign[1];                              /* �����ȣ(1)                     */
    char    change[10];                           /* ���(10)                        */
    char    chrate[6];                            /* �����(6)                       */
    char    volume[10];                           /* �ŷ���(10)                      */
//    char    offer[10];                            /* �ŵ�ȣ��(10)                    */
//    char    bid[10];                              /* �ż�ȣ��(10)                    */
    char    preprice[10];                         /* ���ذ�(10)                      */
    //char    nkey[21];                             /* ����Ű(21)                      */
	char	nkey[ChartKeySize];
//    char    cntsz[6];                             /* ��Ʈ������(6)                   */
    char    fcnt[5];                              /* ��Ʈī��Ʈ(5)                   */
    char    bojomsg[400];                         /* �����޽���(300)                 */
} KB_p0606_OutRec1_EX;

typedef struct
{
    char	name[40];                         /* �����(20)                      */
    char	price[10];                         /* ���簡(10)                      */
    char	sign[1];                         /* �����ȣ(1)                     */
    char	change[10];                         /* ���(10)                        */
    char	chrate[6];                         /* �����(6)                       */
    char	preprice[10];                         /* ���ذ�(10)                      */
    //char	nkey[21];                         /* ����Ű(21)                      */
	char	nkey[ChartKeySize];
    char	fcnt[5];                         /* ��Ʈī��Ʈ(5)                   */
    char	bojomsg[400];                         /* �����޽���(400)                 */
} KB_p0606_OutRec1_COMP;

/* OutRec2:                                                                          */
// ������� �ʴ� Field �ּ�ó��
//char KB_p0606_OutRec2_count[4];
typedef struct
{
    char    date[8];                              /* ��¥(8)                         */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
    char    volume[10];                           /* �ŷ���(10)                      */
} KB_p0606_OutRec2;

typedef struct
{
    char    date[8];                              /* ��¥(8)                         */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
} KB_p0606_OutRec2_COMP;

/* Chart Data:																		 */
typedef struct _KB_p0606_CHART
{
	char    fcnt[5];         					  /* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[400];						  /* �����޽���(300)                 */
	//KB_p0606_OutRec2  chart[50000];
	KB_p0606_OutRec2  chart[1];						// ����ũ����.
} KB_p0606_CHART;

typedef struct _KB_p0606_CHART_COMP
{
	char    fcnt[5];         			/* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[400];				/* �����޽���(300)                 */
	//KB_p0602_OutRec2_COMP  chart[50000];
	KB_p0606_OutRec2_COMP  chart[1];
} KB_p0606_CHART_COMP;
/*************************************************************************************/

#endif

