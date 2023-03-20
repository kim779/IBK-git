/*************************************************************************************/
/*  1. ���α׷�ID : KB_p0602                                                         */
/*  2. ���α׷��� : ��Ʈ ��ȸ(�ֽ�)                                                  */
/*  3. ���񽺸�   : KB_p0602                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_p0602_H__
#define __KB_TRAN_KB_p0602_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_p0602"
//#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

#ifndef ChartKeySize
#define		ChartKeySize		28//KHD : YYYYMMDDHHMMSS : ������ȸ��û�� : "99999999999999" ������ ������ "99999999999999"�� �޴´�.
#endif


/* InRec1:                                                                           */
typedef struct
{
    char    shcode[15];                           /* �����ڵ�(15)                    */
    char    date[8];                              /* �˻�������(8)                   */
    char    num[4];                               /* ��ȸ�Ǽ�(4)                     */
    char    unit[3];                              /* ��ȸ����(3)                     */
    char    button[1];                            /* ��ȸ����(1)                     */
    char    modGb[1];                             /* �����ְ�(1)                     */
    char    nflag[1];                             /* ��ȸ���(1)                     */
//	char    nkey[21];                             /* �˻�Ű(21)                      */
	char    nkey[ChartKeySize];			  /* �˻�Ű(ChartKeySize)        */
    char    nDataGubun[1];                        /* ���ݱ���(1) (0:����, 1:����,2:�޷�) */
    char    ImgRgb[1];                            /* ������(1)                       */
    char    Irecordgb[1];                         /* ���ⱸ�а�(1)(1)                */
    char    nDayBefore[2];                        /* �������n����(2)                */
    char    Itime[6];                             /* ����ð�(hhmmss)(6)             */
	char	nowData[1];							  /*  [������ȸ]     */
	char	usRate[10];							  /* US Dollar ȯ��     */
} KB_p0602_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
// ������� �ʴ� Field �ּ�ó��
typedef struct
{
    char	name[30];                           /* �����(20)                      */
	char	jang[10];                           /* �屸��(10)                      */
    char	price[10];                          /* ���簡(10)                      */
    char	sign[1];                            /* �����ȣ(1)                     */
    char	change[10];                         /* ���(10)                        */
    char	chrate[6];                          /* �����(6)                       */
    char	volume[10];                         /* �ŷ���(10)                      */
    char	offer[10];                          /* �ŵ�ȣ��(10)                    */
    char	bid[10];                            /* �ż�ȣ��(10)                    */
    char	preprice[10];                       /* ���ذ�(10)                      */
    //char	nkey[21];                           /* ����Ű(21)                      */
	char	nkey[ChartKeySize];
    char	cntsz[6];                           /* ��Ʈ������(6)                   */
    char	fcnt[5];                            /* ��Ʈī��Ʈ(5)                   */
    char	bojomsg[500];                       /* �����޽���(400)                 */
} KB_p0602_OutRec1;

typedef struct
{
    char    name[30];                             /* �����(20)                      */
//    char    jang[10];                             /* �屸��(10)                      */
    char    price[10];                            /* ���簡(10)                      */
    char    sign[1];                              /* �����ȣ(1)                     */
    char    change[10];                           /* ���(10)                        */
    char    chrate[6];                            /* �����(6)                       */
    char    volume[10];                           /* �ŷ���(10)                      */
//    char    offer[10];                            /* �ŵ�ȣ��(10)                    */
//    char    bid[10];                              /* �ż�ȣ��(10)                    */
    char    preprice[10];                         /* ���ذ�(10)                      */
//	char	openinterest[18];					// �̰������� //KHD
    //char    nkey[21];                             /* ����Ű(21)                      */
	char	nkey[ChartKeySize];
//    char    cntsz[6];                             /* ��Ʈ������(6)                   */
    char    fcnt[5];                              /* ��Ʈī��Ʈ(5)                   */
    char    bojomsg[500];                         /* �����޽���(300)                 */
} KB_p0602_OutRec1_EX;

typedef struct
{
    char	name[30];                         /* �����(20)                      */
    char	price[10];                         /* ���簡(10)                      */
    char	sign[1];                         /* �����ȣ(1)                     */
    char	change[10];                         /* ���(10)                        */
    char	chrate[6];                         /* �����(6)                       */
    char	preprice[10];                         /* ���ذ�(10)                      */
    //char	nkey[21];                         /* ����Ű(21)                      */
	char	nkey[ChartKeySize];
    char	fcnt[5];                         /* ��Ʈī��Ʈ(5)                   */
    char	bojomsg[500];                         /* �����޽���(400)                 */
} KB_p0602_OutRec1_COMP;

/* OutRec2:                                                                          */
// ������� �ʴ� Field �ּ�ó��
//char KB_p0602_OutRec2_count[4];
typedef struct
{
    char    date[8];                              /* ��¥(8)                         */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
    char    volume[10];                           /* �ŷ���(10)                      */
	//[A00000408]�ŷ���� 12 -> 18.
    //char    value[12];                          /* �ŷ����(12)                    */
	char    value[18];                            /* �ŷ����(18)                    */
    char    rights[1];                            /* �Ǹ���(1)                       */
    char    sign[1];                              /* ���ϴ���ȣ(1)                 */
} KB_p0602_OutRec2;

typedef struct
{
    char    date[8];                              /* ��¥(8)                         */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
} KB_p0602_OutRec2_COMP;

/* Chart Data:																		 */
typedef struct _KB_p0602_CHART
{
	char    fcnt[5];         			/* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[500];				/* �����޽���(300)                 */
	//KB_p0602_OutRec2  chart[50000];
	KB_p0602_OutRec2  chart[1];			// ����ũ����.
} KB_p0602_CHART;

typedef struct _KB_p0602_CHART_COMP
{
	char    fcnt[5];         			/* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[500];				/* �����޽���(300)                 */
	//KB_p0602_OutRec2_COMP  chart[50000];
	KB_p0602_OutRec2_COMP  chart[1];
} KB_p0602_CHART_COMP;

/*************************************************************************************/

#endif

