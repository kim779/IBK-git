/*************************************************************************************/
/*  1. ���α׷�ID : KB_p0603                                                         */
/*  2. ���α׷��� : ��Ʈ ��ȸ(����)                                                  */
/*  3. ���񽺸�   : KB_p0603                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_p0603_H__
#define __KB_TRAN_KB_p0603_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_p0603"
//#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

#define		ChartKeySize_p0603		28

/* InRec1:                                                                           */
typedef struct
{
    char    Sjanggb[1];                           /* �屸��(1)                       */
    char    Supcode[3];                           /* �����ڵ�(3)                     */
    char    Sdate[8];                             /* �˻�������(8)                   */
    char    Smaxbong[4];                          /* �˻��ҵ����Ͱ���(4)             */
    char    Sunit[3];                             /* N��(ƽ�н���)(3)                */
    char    Sbtngb[1];                            /* ��ȸ����(1)                     */
    char    Snextgb[1];                           /* ����/��ȸ(1)                    */
//    char    Snextkey[21];                       /* �˻�Ű(21)                      */
    char    Snextkey[ChartKeySize_p0603];         /* �˻�Ű(21)                      */
	char    SdataType[1];						  /* ��Ʈ���� (0:����, 1:����,2:�޷�)*/
	char	nowData[1];							  /* ������ȸ(1)					 */
	char    ImgRgb[1];// ������ : KHD
}KB_p0603_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
// ������� �ʴ� Field �ּ�ó��
typedef struct
{
    char	name[20];                         /* �����(20)                      */
    char	jang[10];                           /* �屸��(10)                      */
    char	price[10];                         /* ���簡(10)                      */
    char	sign[1];                         /* �����ȣ(1)                     */
    char	change[10];                         /* ���(10)                        */
    char	chrate[6];                         /* �����(6)                       */
    char	volume[10];                           /* �ŷ���(10)                      */
    char	offer[10];                           /* �ŵ�ȣ��(10)                    */
    char	bid[10];                           /* �ż�ȣ��(10)                    */
    char	preprice[10];                         /* ���ذ�(10)                      */
    //char	nkey[21];                         /* ����Ű(21)                      */
	char    nkey[ChartKeySize_p0603];
    char	cntsz[6];                           /* ��Ʈ������(6)                   */
    char	fcnt[5];                         /* ��Ʈī��Ʈ(5)                   */
    char	bojomsg[400];                         /* �����޽���(256)                 */
}KB_p0603_OutRec1;

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
    //char    nkey[21];                             /* ����Ű(21)                      */
	char    nkey[ChartKeySize_p0603];
//    char    cntsz[6];                             /* ��Ʈ������(6)                   */
    char    fcnt[5];                              /* ��Ʈī��Ʈ(5)                   */
    char    bojomsg[400];                         /* �����޽���(256)                 */
}KB_p0603_OutRec1_EX;

typedef struct
{
    char	name[20];                         /* �����(20)                      */
    char	price[10];                         /* ���簡(10)                      */
    char	sign[1];                         /* �����ȣ(1)                     */
    char	change[10];                         /* ���(10)                        */
    char	chrate[6];                         /* �����(6)                       */
    char	preprice[10];                         /* ���ذ�(10)                      */
    //char	nkey[21];                         /* ����Ű(21)                      */
	char    nkey[ChartKeySize_p0603];
    char	fcnt[5];                         /* ��Ʈī��Ʈ(5)                   */
    char	bojomsg[400];                         /* �����޽���(400)                 */
} KB_p0603_OutRec1_COMP;

/* OutRec2:                                                                          */
//char KB_p0603_OutRec2_count[4];
typedef struct
{
    char    date[8];                              /* ��¥(8)                         */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
    char    volume[10];                           /* �ŷ���(10)                      */

	//[A00000408]�ŷ���� 12 -> 18.
    //char    value[12];                            /* �ŷ����(12)                    */
	char    value[18];                            /* �ŷ����(12)                    */

    char    rights[1];                            /* �Ǹ���(1)                       */
}KB_p0603_OutRec2;

typedef struct
{
    char    date[8];                              /* ��¥(8)                         */
    char    open[10];                             /* �ð�(10)                        */
    char    high[10];                             /* ��(10)                        */
    char    low[10];                              /* ����(10)                        */
    char    close[10];                            /* ����(10)                        */
}KB_p0603_OutRec2_COMP;

/* Chart Data:																		 */
typedef struct _KB_p0603_CHART
{
	char    fcnt[5];         			/* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[400];				/* �����޽���(300)                 */
	//KB_p0603_OutRec2  chart[50000];
	KB_p0603_OutRec2  chart[1];						// ����ũ����.
} KB_p0603_CHART;

typedef struct _KB_p0603_CHART_COMP
{
	char    fcnt[5];         			/* ��Ʈī��Ʈ(5)                   */
	char    bojomsg[400];				/* �����޽���(300)                 */
	//KB_p0603_OutRec2_COMP  chart[50000];
	KB_p0603_OutRec2_COMP  chart[1];
} KB_p0603_CHART_COMP;

/*************************************************************************************/

#endif

