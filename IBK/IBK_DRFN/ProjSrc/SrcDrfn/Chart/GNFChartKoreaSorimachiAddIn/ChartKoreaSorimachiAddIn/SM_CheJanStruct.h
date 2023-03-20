/*************************************************************************************/
/*  1. ���α׷�ID : SM_CheJanStruct                                                  */
/*  2. ���α׷��� : aa                                                               */
/*  3. ���񽺸�   : SM_CheJanStruct	                                                 */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : gds                                                              */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __SM_TRAN_SM_CHEJANSTRUCT_H__
#define __SM_TRAN_SM_CHEJANSTRUCT_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define LLONG                         long long


#define		UM_CHEJAN_COMMIT		WM_USER + 1100		//	����, �ܰ�, ü�� ��� �Ϸ� �޽���
#define		UM_GETBROAD				WM_USER + 1101		//	ü��, �ܰ� �ǽð� ���� �޽���
#define		UM_CHEJAN_NEWDATA		WM_USER + 1102		//	���ο� �ܰ� �߰��� �뺸 �޽���
#define		UM_GETBROADTEMP			WM_USER + 1103		//	���ο� �ܰ� �߰��� �뺸 �޽���
#define		UM_RESET_CHEJANINFO		WM_USER + 1150		//	ü���ܰ� �缳�� �޽���


#define ulong	 ULONG

#pragma pack(push, 1) 

//===================================================================================
//	��� ������ ����ü
//===================================================================================
//	�ֽ� �ֹ�ü�� ���� ������ ��ȸ ����ü
typedef struct 
{
	char	accno			[11];		// ���¹�ȣ 
	char	scod_gb			[1];		// ������ȸ����(0:��ü, 1:������ȸ)
	char	debg			[1];		// �ŵ��ż���ȸ����(0:��ü, 1:�ŵ�, 2:�ż�)
	char	scod			[12];		// ���������ڵ�
} CHE_RQ_INPUT;

typedef struct 
{
	char	nCnt			[ 4];		// ������ �Ǽ�
} CHE_RQ_OUTPUT1;

typedef struct 
{
	char	or_type			[ 2];	  /* �ֹ������ڵ�					  */
	char	prgb			[ 2];	  /* �ֹ�ü�������ڵ�				  jcgb*/
	char 	accno			[11];     /* ���¹�ȣ                         */
	char 	orderno			[10];     /* �ֹ���ȣ                         */
	char 	orgorderno		[10];     /* ���ֹ���ȣ                       */
	char 	cheno			[10];     /* ü���ȣ                         */
	char 	jmcode			[12];     /* �����ڵ�                         */
	char 	jmname			[40];     /* �����                           */
	char 	bid_tpcd		[ 2];     /* ȣ�������ڵ�                     memegb*/
	char 	ord_cond		[ 1];     /* �ֹ�����                         */
	char    cdgb			[ 3];	  /* �ſ뱸��                        */
	char 	ord_tpcd		[ 2];     /* �ֹ������ڵ�                     jugb*/
	char 	ord_mkcd		[ 2];     /* �ֹ������ڵ�                     ordst*/
	char 	medosu			[ 1];     /* �ŵ������� ( 1:�ŵ�, 2:�ż� )    */
	char 	jprc			[13];     /* �ֹ�����                         */
	char 	jqty			[16];     /* �ֹ�����                         */
	char 	cheprice		[13];     /* ü�ᰡ                           */
	char 	cheqty			[16];     /* ü�ᷮ                           */
	char 	sumprice		[16];     /* ü�ᴩ��ݾ�                     */
	char 	mecheqty		[16];     /* ��ü�����                       */
	char 	chetime			[ 8];     /* �ֹ��ð�		                  */
	char 	price			[ 9];     /* ���簡                           */
	char 	sign			[ 1];     /* ���ϴ���ȣ                     */
	char 	medoga			[ 9];     /* (�ֿ켱)�ŵ�ȣ��                 */
	char 	mesuga			[ 9];     /* (�ֿ켱)�ż�ȣ��                 */
} CHE_RQ_OUTPUT2;

//	�ֽ� �ֹ���ü�� ���� ������ ��ȸ ����ü
typedef struct 
{
	char	accno			[11];		// ���¹�ȣ 
	char	scod_gb			[1];		// ������ȸ����(0:��ü, 1:������ȸ)
	char	debg			[1];		// �ŵ��ż���ȸ����(0:��ü, 1:�ŵ�, 2:�ż�)
	char	scod			[12];		// ���������ڵ�
} MICHE_RQ_INPUT;

typedef struct 
{
	char	nCnt			[ 4];		// ������ �Ǽ�
} MICHE_RQ_OUTPUT1;

typedef struct 
{
	char	or_type			[ 2];	  /* �ֹ������ڵ�					  */
	char	prgb			[ 2];	  /* �ֹ�ü�������ڵ�				  jcgb*/
	char 	accno			[11];     /* ���¹�ȣ                         */
	char 	orderno			[10];     /* �ֹ���ȣ                         */
	char 	orgorderno		[10];     /* ���ֹ���ȣ                       */
//	char 	cheno			[10];     /* ü���ȣ                         */
	char 	jmcode			[12];     /* �����ڵ�                         */
	char 	jmname			[40];     /* �����                           */
	char 	bid_tpcd		[ 2];     /* ȣ�������ڵ�                     memegb*/
	char 	ord_cond		[ 1];     /* �ֹ�����                         */
	char    cdgb			[ 3];	  /*  �ſ뱸��                        */
	char 	ord_tpcd		[ 2];     /* �ֹ������ڵ�                     jugb*/
	char 	ord_mkcd		[ 2];     /* �ֹ������ڵ�                     ordst*/
	char 	medosu			[ 1];     /* �ŵ������� ( 1:�ŵ�, 2:�ż� )    */
	char 	jprc			[13];     /* �ֹ�����                         */
	char 	jqty			[16];     /* �ֹ�����                         */
//	char 	cheprice		[13];     /* ü�ᰡ                           */
//	char 	cheqty			[16];     /* ü�ᷮ                           */
//	char 	sumprice		[16];     /* ü�ᴩ��ݾ�                     */
	char 	mecheqty		[16];     /* ��ü�����                       */
	char 	chetime			[ 8];     /* �ֹ��ð�		                  */
	char 	price			[ 9];     /* ���簡                           */
	char 	sign			[ 1];     /* ���ϴ���ȣ                     */
	char 	medoga			[ 9];     /* (�ֿ켱)�ŵ�ȣ��                 */
	char 	mesuga			[ 9];     /* (�ֿ켱)�ż�ȣ��                 */
} MICHE_RQ_OUTPUT2;

//	�ֽ� �ܰ� ��� ������ ����ü
typedef struct 
{
	char	accno			[11];		// ���¹�ȣ 
} JAN_RQ_INPUT;

typedef struct 
{
	char	accno			[11];	// ���¹�ȣ 
	char    trsonik			[16];	/* ������/������ ���� �ȵ� ���� ��������*/
    char    sonik_fee		[16];	/* �������Ϳ� ���� ������				*/
    char    sonik_tax		[16];	/* �������Ϳ� ���� ����					*/
	char	nCnt			[ 4];	/* ������ �Ǽ�							*/
} JAN_RQ_OUTPUT1;

typedef struct 
{
	char    accno			[11];  /*  ���¹�ȣ                        */
    char    jmcode			[12];  /*  �����ڵ�                        */
	char    jmname			[40];  /*  �����                          */
    char    sygb			[ 3];  /*  �ſ뱸��                        */
	char    bqty			[16];  /*  ��������                        */
	char    ordgaqty		[16];  /*  �ֹ����� ����                   */
	char    buyamt			[13];  /*  ���Դܰ�                        */
	char    bep_price		[13];  /*  BEP �ܰ�                        */
	char    menusum			[16];  /*  ���Աݾ�                        */
    char    estamt			[16];  /*  �򰡱ݾ�                        */
	char    estsonik		[16];  /*  �򰡼���                        */
	char    rate			[ 8];  /*  ���ͷ�                          */
	char    fee				[16];  /*  ������                          */
	char    tax				[16];  /*  ����                            */
	char    holdrate		[ 6];  /*  ��������                        */
	char    sdate			[ 8];  /*  ������                          */
    char    lastday			[ 8];  /*  ������                          */
    char    sinamt			[16];  /*  �ſ�ݾ�                        */
    char    sininter		[16];  /*  �ſ�����                        */
    char    janqty			[16];  /*  �����ܰ�                        */
    char    psmevol			[16];  /*  ���ϼ��ż���                    */
    char    tsmevol			[16];  /*  ���� ���ż���                   */
    char    damboloanamt	[16];  /*  �㺸�������                    */
	char    tmedosonik		[16];  /*  ���� �� �ŵ�����                */
	char    trsonik			[16];  /*  ���Ͻ�������(����)              */
    char    ratesonik		[16];  /*  ���Ͻ������ͷ�(����)            */
    char    strsonik		[16];  /*  ���Ͻ�������(�ſ�)              */
    char    sratesonik		[16];  /*  ���Ͻ������ͷ�(�ſ�)            */
	char    pjogaest		[16];  /*  �������� ������               */
	char    pjogasonik		[16];  /*  �������� ���� �򰡼���          */
    char    pjogarate		[16];  /*  �������� ���� ���ͷ�            */
	char 	medoamtsum		[16];  /*  ���ϸŵ��ݾ���                  */
	char 	mesuamtsum		[16];  /*  ���ϸż��ݾ���                  */
	char    giga			[ 9];  /*  ���ذ�                          */
	char    uplmtprice		[ 9];  /*  ���Ѱ�                          */
	char    dnlmtprice		[ 9];  /*  ���Ѱ�                          */
	char    price			[ 9];  /*  ���簡                          */
	char    mesuga			[ 9];  /*  (�ֿ켱)�ż�ȣ��                */
	char    medoga			[ 9];  /*  (�ֿ켱)�ŵ�ȣ��                */
	char    sign			[ 1];  /*  ���ϱ���                        */
	char    change			[ 8];  /*  ���ϴ��                        */
	char    diff			[ 6];  /*  ����� ( �Ҽ��� 2�ڸ� )         */
    char    volume			[16];  /*  �ŷ���                          */

} JAN_RQ_OUTPUT2;


//	�����ɼ� ü�� ���� ������ ��ȸ ����ü
typedef struct 
{
	char	accno			[11];		// ���¹�ȣ 
	char	scod_gb			[ 1];		// ������ȸ����(0:��ü, 1:������ȸ)
	char	debg			[ 1];		// �ŵ��ż���ȸ����(0:��ü, 1:�ŵ�, 2:�ż�)
	char	scod			[12];		// ���������ڵ�
} FO_CHE_RQ_INPUT;

typedef struct 
{
	char	nCnt			[ 4];		//	������ �Ǽ�
} FO_CHE_RQ_OUTPUT1;

typedef struct 
{
	char	or_type			[ 2];	  /* �ֹ������ڵ�					  */
	char	prgb			[ 2];	  /* �ֹ�ü�������ڵ�				  jcgb*/
	char 	accno			[11];     /* ���¹�ȣ                         */
	char 	orderno			[10];     /* �ֹ���ȣ                         */
	char 	orgorderno		[10];     /* ���ֹ���ȣ                       */
	char 	cheno			[10];     /* ü���ȣ                         */
	char 	jmcode			[12];     /* �����ڵ�                         */
	char 	jmname			[40];     /* �����                           */
	char 	bid_tpcd		[ 2];     /* ȣ�������ڵ�                     memegb*/
	char 	ord_cond		[ 1];     /* �ֹ�����                         */
	char 	ord_tpcd		[ 2];     /* �ֹ������ڵ�                     jugb*/
	char 	ord_mkcd		[ 2];     /* �ֹ������ڵ�                     ordst*/
	char 	medosu			[ 1];     /* �ŵ������� ( 1:�ŵ�, 2:�ż� )    */
	char 	jprc			[13];     /* �ֹ�����                         */
	char 	jqty			[16];     /* �ֹ�����                         */
	char 	cheprice		[13];     /* ü�ᰡ                           */
	char 	cheqty			[16];     /* ü�ᷮ                           */
	char 	sumprice		[16];     /* �����ݾ��հ�                     */
	char 	mecheqty		[16];     /* ��ü�����                       */
	char 	chetime			[ 8];     /* �ֹ��ð�		                  */
	char 	price			[ 9];     /* ���簡                           */
	char 	sign			[ 1];     /* ���ϴ���ȣ                     */
	char 	medoga			[ 9];     /* (�ֿ켱)�ŵ�ȣ��                 */
	char 	mesuga			[ 9];     /* (�ֿ켱)�ż�ȣ��                 */
} FO_CHE_RQ_OUTPUT2;

//	�����ɼ� ��ü�� ���� ������ ��ȸ ����ü
typedef struct 
{
	char	accno			[11];		// ���¹�ȣ 
	char	scod_gb			[ 1];		// ������ȸ����(0:��ü, 1:������ȸ)
	char	debg			[ 1];		// �ŵ��ż���ȸ����(0:��ü, 1:�ŵ�, 2:�ż�)
	char	scod			[12];		// ���������ڵ�
} FO_MICHE_RQ_INPUT;

typedef struct 
{
	char	nCnt			[ 4];		//	������ �Ǽ�
} FO_MICHE_RQ_OUTPUT1;

typedef struct 
{
	char	or_type			[ 2];	  /* �ֹ������ڵ�					  */
	char	prgb			[ 2];	  /* �ֹ�ü�������ڵ�				  jcgb*/
	char 	accno			[11];     /* ���¹�ȣ                         */
	char 	orderno			[10];     /* �ֹ���ȣ                         */
	char 	orgorderno		[10];     /* ���ֹ���ȣ                       */
//	char 	cheno			[10];     /* ü���ȣ                         */
	char 	jmcode			[12];     /* �����ڵ�                         */
	char 	jmname			[40];     /* �����                           */
	char 	bid_tpcd		[ 2];     /* ȣ�������ڵ�                     memegb*/
	char 	ord_cond		[ 1];     /* �ֹ�����                         */
	char 	ord_tpcd		[ 2];     /* �ֹ������ڵ�                     jugb*/
	char 	ord_mkcd		[ 2];     /* �ֹ������ڵ�                     ordst*/
	char 	medosu			[ 1];     /* �ŵ������� ( 1:�ŵ�, 2:�ż� )    */
	char 	jprc			[13];     /* �ֹ�����                         */
	char 	jqty			[16];     /* �ֹ�����                         */
//	char 	cheprice		[13];     /* ü�ᰡ                           */
//	char 	cheqty			[16];     /* ü�ᷮ                           */
//	char 	sumprice		[16];     /* �����ݾ��հ�                     */
	char 	mecheqty		[16];     /* ��ü�����                       */
	char 	chetime			[ 8];     /* �ֹ��ð�		                  */
	char 	price			[ 9];     /* ���簡                           */
	char 	sign			[ 1];     /* ���ϴ���ȣ                     */
	char 	medoga			[ 9];     /* (�ֿ켱)�ŵ�ȣ��                 */
	char 	mesuga			[ 9];     /* (�ֿ켱)�ż�ȣ��                 */
} FO_MICHE_RQ_OUTPUT2;


//===================================================================================
//	�ܰ� ��� ������ ����ü
//===================================================================================
typedef struct 
{
	char	accno			[11];		// ���¹�ȣ 
} FO_JAN_RQ_INPUT;

typedef struct 
{
	char	accno			[11];	// ���¹�ȣ 
	char    trsonik			[16];	/*  ������/������ ���� �ȵ� ���� ��������   */
    char    sonik_fee		[16];	/*  �������Ϳ� ���� ������					*/
    char    sonik_tax		[16];	/*  �������Ϳ� ���� ����					*/
	char	nCnt			[ 4];
} FO_JAN_RQ_OUTPUT1;

typedef struct 
{
	char    accno			[11];		/*  ���¹�ȣ                        */
    char    jmcode			[12];     /*  �����ڵ�  "XXXXXXXX"            */
    char    jmname			[40];     /*  �����                          */
	char    bqty			[16];		/*  ��������                        */
    char    buyamt			[13];		/*  ��ո�������                    */ /*  ��ո�������/100                */
	char    mesusum			[16];		/*  �Ѹ��Աݾ�                      */ /*  �Ѹ��Աݾ�/1000+0.5             */
	char    ordgaqty		[16];		/*  û�갡�� ����                   */
    char    tsmevol			[16];		/*  ���ϼ��ż���                    */ /*  ���ϸż�����-���ϸŵ�����       */
	char    medosu			[ 1];		/*  �ŵ�/�ż� ����                  */
	char    fee				[16];		/*  ������                          */
    char    tax				[16];		/*  ����                            */
	char    preqty			[16];		/*  ���Ϻ�������		            */
	char 	medoamtsum		[16];		/* �ŵ��ݾ���                       */
	char 	mesuamtsum		[16];		/* �ż��ݾ���                       */
	char    price			[ 9];		/*  ���簡                          */ /*  0�̸� ���ذ�,                   *//*  ���ذ��� 0�̸� ��������         */
    char    sign			[ 1];		/*  ���ϱ���                        */
    char    medoga			[ 9];		/*  (�ֿ켱)�ŵ�ȣ��                */
    char    mesuga			[ 9];		/*  (�ֿ켱)�ż�ȣ��                */

} FO_JAN_RQ_OUTPUT2;

//	�ؿܼ��� ü�� ���� ������ ��ȸ ����ü
typedef struct 
{
	char    acntno[11];                           /* ���¹�ȣ(11)                    */
    char    acntpwd[32];                          /* ��й�ȣ(32)                    */
    char    base_dt[8];                           /* �ŷ�����(8)                     */
    char    mrkt_cd[3];                           /* ǰ��(����)�ڵ�(3)               */
    char    che_gbn[1];                           /* ü�ᱸ��(1)                     */
    char    ord_gbn[1];                           /* �ֹ�����(1)                     */
    char    ord_tp[1];                            /* �ֹ�����(1)                     */
} FR_CHE_RQ_INPUT;

typedef struct 
{
	char    qry_cnt[4];                           /* ī��Ʈ(4)                       */
} FR_CHE_RQ_OUTPUT1;

typedef struct 
{
	char    acctno[11];                           /* ���¹�ȣ(11)                    */
	char    ord_no[5];                            /* �ֹ���ȣ(5)                     */
    char    orig_ord_no[5];                       /* ���ֹ���ȣ(5)                   */
    char    series[32];                           /* ����(32)                        */
    char    bysl_tp[1];                           /* ����(1)                         */
    char    proc_stat[1];                         /* �ֹ�ó������(1)                 */
    char    ord_q[10];                            /* �ֹ���(10)                      */
    char    trd_q[10];                            /* ü�ᷮ(10)                      */
    char    remainqty[10];                        /* �ܷ�(10)                        */
    char    ord_p[19];                            /* �ֹ���(19)                      */
    char    trd_p[19];                            /* ü�ᰡ(19)                      */
    char    mdfy_cncl_q[19];                      /* �������(19)                    */
    char    prce_tp[1];                           /* ���ݱ���(1)                     */
    char    stop_p[19];                           /* ���ǰ���(19)                    */
    char    ordtype[1];                           /* �ֹ�����(1)                     */
	char    ord_tp[1];                            /* �ֹ�����(1)                     */
    char    acpt_time[10];                        /* �����ð�(10)                    */
    char    kr_ord_date[8];                       /* �ѱ��ֹ�����(8)                 */
    char    kr_acpt_time[10];                     /* �ѱ������ð�(10)                */
    char    kr_che_time[10];                      /* �ѱ�ü��ð�(10)                */
    char    ord_way_tp[1];                        /* ��ü����(1)                     */
    char    work_empl[9];                         /* �����ID(9)                     */
    char    crc_cd[3];                            /* ��ȭ�ڵ�(3)                     */
} FR_CHE_RQ_OUTPUT2;

//	�ؿܼ��� �ܰ� ���� ������ ��ȸ ����ü
typedef struct 
{
	char    acntno[11];                           /* ���¹�ȣ(11)                    */
} FR_JAN_RQ_INPUT;

typedef struct 
{
	char    t_pospl[19];                          /* �̰��������򰡱ݾ�	(19,7)(19)   */
    char    t_open_pl[19];                        /* �򰡼���(19)                    */
    char    t_invtrate[19];                       /* ������(19)                      */
    char    t_real_pl[19];                        /* ��������(19)                    */
    char    t_pl[19];                             /* �Ѽ���(19)                      */
    char    t_cmpr_pl[19];                        /* ����������(19)                */
    char    t_pres_asset[19];                     /* �����ڻ�(19)                    */
   	char    acctno[11];                           /* ���¹�ȣ(11)                    */ //������ ��� ������ �߰�
	char    fee_level[1];                         /* ��������(1)                   */ //������ ��� ������ �߰�
	char    e_sell[8];                            /* ��ȭ�ŵ�ȯ��(8.4)               */ //�򰡼��� �հ� ������ �߰�
	char    d_buy[8];                             /* �޷��ż�ȯ��(8.4)               */ //�򰡼��� �հ� ������ �߰�
	char    qry_cnt[4];                           /* ī��Ʈ(4)                       */
} FR_JAN_RQ_OUTPUT1;

typedef struct 
{
	char    acctno[11];                           /* ���¹�ȣ(11)                    */
	char    series[32];                           /* ����(32)                        */
    char    series_nm[50];                        /* �����(50)                      */
    char    crc_cd[3];                            /* �ŷ���ȭ(3)                     */
    char    buysellsect[4];                       /* ����(4)                         */
    char    open_q[10];                           /* �̰�������(10)                  */
    char    rsrb_ableqty[10];                     /* û�갡�ɼ���(10)                */
    char    open_uv[19];                          /* ��վ�����(19)                  */
    char    last_p[19];                           /* ���簡(19)                      */
    char    open_pl[19];                          /* �򰡼���(19)                    */
    char    invtrate[19];                         /* ������(19)                      */
    char    ctrt_size[19];                        /* ������(19)                    */
    char    dealamt[19];                          /* ü��ݾ�(19)                    */
    char    pospl[19];                            /* �̰��������򰡱ݾ�(19)          */
} FR_JAN_RQ_OUTPUT2;

// FX ü�� ���� ������ ��ȸ ����ü
typedef struct
{
    char    szAccNo[14];				/* ���¹�ȣ */
    char    szAccNoPW[8];				/* ��й�ȣ */
    char    szItemCode[16];				/* �����ڵ� */
    char    szChGB[1];					/* ü�ᱸ�� */
    char    szMMGB[1];					/* �ŵ�/�ż� ���� */
	char	gubun[1];					/* ���ӱ��� */
	char	keyvalue[84];				/* key ����+�ð�	: key ����+�ð� */
} FX_CHE_RQ_INPUT;

typedef struct
{
    char    szArrayCnt[4];				/* �Է¹迭 ���� */
    char    szCurNo[12];				/* ����� */
} FX_CHE_RQ_INPUT1;

typedef struct
{
    char    szAccNo[14];				/* ���¹�ȣ */
    char    szAccNoPW[8];				/* ��й�ȣ */
} FX_CHE_RQ_INPUT2;    

typedef struct 
{
	char	nCnt[ 4];					/* ������ �Ǽ� */
} FX_CHE_RQ_OUTPUT1;

typedef struct
{
    char    szTicketNo[20];				/* ü���ȣ */
    char    szCurNo[12];				/* �����(Currency) */
    char    szSide[3];					/* �Ÿű���(Buy/Sell) */
    char    szStatus[1];				/* ���� */
    char    szRate[10];					/* �ֹ� ���� */
    char    szQuote[10];				/* ���簡 */
    char    szStop[10];					/* Stop */
    char    szLimit[10];				/* Limit */
    int     nTrailingStop;				/* Trailing Stop */
	double	fTSHighPrice;				/* ���簡 ��� ���� �ְ� */
    double  fLot;						/* ����(Lot) */
    double  fTotalPL;					/* Total P/L (�Ѽ���) */
    double  fGrossPL;					/* ���ϼ��� */
    double  fInterest;					/* ���� */
    double  fAddInterest;				/* ������ ���� */
    double  fPipCost;
    int     nPipLowest;
    char    szOrderTime[30];			/* �ŷ��� ���� �ð�(Time) */
    char    szCustItem1[11];			/* ȸ��ó���׸�1 */
	char    szCustItem2[14];			/* ȸ��ó���׸�2 */
	char    szCustItem3[55];			/* ȸ��ó���׸�3 */
    char    szStopItem[80];				/* Stop ���� ȸ��ó���׸� */
    char    szLimitItem[80];			/* Limit ���� ȸ��ó���׸� */
    char    szTSCustItem[80];			/* Trailing Stop ȸ��ó���׸� */
	char	szMemberNo[3];				/* ȸ�����ȣ */
    char    szAccNo[11];				/* ���¹�ȣ */
    char    szUserName[40];				/* ���¸� */
} FX_CHE_RQ_OUTPUT2;

// FX ��ü�� ������ ��ȸ ����ü
typedef struct
{
    char    szArrayCnt[4];				/* �Է¹迭 ���� */
} FX_MICHE_RQ_INPUT1;

typedef struct
{
    char    szAccNo[14];				/* ���¹�ȣ*/
    char    szAccNoPW[8];				/* ��й�ȣ */
} FX_MICHE_RQ_INPUT2;

typedef struct 
{
	char	nCnt[4];					/* ������ �Ǽ� */
} FX_MICHE_RQ_OUTPUT1;

typedef struct
{
    double  fLot;						/* ����(Lot) */
	int		nPipLowest;					/* ��ȿ�ڸ��� */
    char    szCustItem1[11];			/* ȸ��ó���׸�1 */
	char	szCustItem2[14];			/* ȸ��ó���׸�2 */
	char	szCustItem3[55];			/* ȸ��ó���׸�3 */
    char    szCurNo[12];				/* �����(Currency) */
    char    szSide[3];					/* �Ÿű���(Buy/Sell) */
    char    szRate[10];					/* �ֹ� ����(Price) */
    char    szStop[10];					/* Stop */
    char    szLimit[10];				/* Limit */
	char	szTrailingStop[10];			/* Trailing Stop */
    char    szStatus[1];				/* �ֹ����� - 0:������, 1:���� */
    char    szOrderTime[30];			/* �ŷ��� ���� �ð�(Time) */
    char    szOrdType[5];				/* �ֹ� ���� */
	char	szMemberNo[3];				/* ȸ���� */
    char    szAccNo[11];				/* ���¹�ȣ */
    char    szUserName[20];				/* ���¸� */
} FX_MICHE_RQ_OUTPUT2;

// FX �ܰ� ������ ��ȸ ����ü
typedef struct
{
    char    szArrayCnt[4];				/* �Է� ���� ���� */
} FX_JAN_RQ_INPUT1;

typedef struct
{
    char    szAccNo[14];				/* ���¹�ȣ*/
} FX_JAN_RQ_INPUT2;

typedef struct 
{
	char	nCnt[4];					/* ������ �Ǽ� */
} FX_JAN_RQ_OUTPUT1;

typedef struct
{
    double  fBalance;					/* ��Ź�� �ܾ� */
    double  fOutstanding;				/* û����� */
    double  fGrossPL;					/* �򰡼��� */
    double  fEquity;					/* ���ڻ� */
    double  fMarginReq;					/* �ֹ� ���ű� �հ� */
    double  fMtMargin;					/* ���� ���ű� �հ� */
    double  fUsableEquity;				/* �ֹ����ɱݾ� */
    double  fUEquityRate;				/* �����ݺ���(%) */
    double  fDepoTotalMSubs;			/* ��ȭ����򰡱ݾ� */
    double  fNowSellRate;				/* ��ȭ�������ȯ��(���ظŵ�ȯ��) */
    double  fBalanceWon;				/* ��ȭ�ѿ�Ź�� */
	double	fTotalWon;					/* ��ȭȯ���ڻ� */
	double	fOrderMargin;				/* �ֹ� ���ű� */
    int     nOutCount;					/* �ֹ����ɼ��� */
	int		nOpenSum;					/* ��û�� ���� �հ� */
	char	szMemberNo[3];				/* ȸ���� ��ȣ */
    char    szAccNo[11];				/* ���¹�ȣ*/
    char    szUserName[40];				/* ���¸� */
    char    cIsHedge;					/* �������� */
} FX_JAN_RQ_OUTPUT2;

/*************************************************************************************/
//	�ǽð� ������ ����ü
/*************************************************************************************/
// �ֽ�ü�� �ǽð�
typedef struct
{
	//	�������� �ֽ�ü�� ����ü
	char 	type			[ 4];     /* ������ Type  [CHE0]        	*/
	char 	gb				[ 1];     /* ������ Type  ������        	*/
	char 	acckey			[11];     /* ���¹�ȣ Ű��              	*/
	char 	realkey			[11];     /* ������ ID                	*/
    char 	lineseq			[ 8];     /* �����Ϸù�ȣ             	*/
	char	mdgb			[ 2];		     /* ���ӱ���					         */
	char 	adminno			[ 8];     /* �����ڻ��                       */
	char	dsize			[ 5];			 /* Data size (������� ����)			 */
	char	usrfld			[15];		     /* ��������ǿ���(����̻��)	         */
	char	or_type			[ 2];	  /* �ֹ������ڵ�					  */
	char	prgb			[ 2];	 /* �ֹ�ü�ᱸ��					 */
									 /*"10":���ֹ� "11":�����ֹ�         */
									 /*"12":����ֹ�                     */ 
	char 	accno			[11];     /* ���¹�ȣ                         */
	char 	orderno			[ 10];     /* �ֹ���ȣ                         */
	char 	orgorderno		[10];     /* ���ֹ���ȣ                       */
	char 	cheno			[10];     /* ü���ȣ                         */

	char 	jmcode			[12];     /* �����ڵ�                         */
	char 	jmname			[40];     /* �����                           */
	char 	bid_tpcd			[ 2];     /* �ֹ�����                         */
	char 	ord_cond         	[ 1];    /* �ֹ�����	                     */
	char    cdgb			[ 3];	  /*  �ſ뱸��                        */
	char 	ord_tpcd			[ 2];     /* �ֹ�����                         */
	char 	ord_mkcd			[ 2];     /* �ֹ������з�                     */
	char 	medosu			[ 1];     /* �ŵ������� ( 1:�ŵ�, 2:�ż� )    */
		
	char 	jprc			[13];     /* �ֹ�����                         */
	char 	jqty			[16];     /* �ֹ�����                         */
	char 	cheprice		[13];     /* ü�ᰡ                           */
	char 	cheqty			[16];     /* ü�ᷮ                           */
	
	char 	sumprice		[16];     /* ü�ᴩ��ݾ�                     */
	char 	mecheqty		[16];     /* ��ü�����                       */
	char 	percheprice		[13];     /* ����ü�ᰡ                       */
	char 	percheqty		[16];     /* ����ü�ᷮ                       */
	
	char 	chetime			[ 8];     /* �ֹ�/ü�� �ð�                   */

	/* ���ֹ� DATA�� */
    char    og_orno             [10];    /* ���ֹ�-�ֹ���ȣ                  */
    char    og_opct_qt          [16];    /* ���ֹ�-��ü�����                */

//	char 	price			[ 9];     /* ���簡                           */
//	char 	sign			[ 1];     /* ���ϴ���ȣ                     */
//	char 	medoga			[ 9];     /* (�ֿ켱)�ŵ�ȣ��                 */
//	char 	mesuga			[ 9];     /* (�ֿ켱)�ż�ȣ��                 */
//	char 	memefee			[16];     /* ���ϸŸ� ������                  */
//	char 	memetax			[16];     /* ���ϸŸż���                     */
//	char	order_amt		[13];	 /*  �ֹ���������                    */
//	char	order_dae		[13];	 /*  �ֹ����ɴ��                    */
//	char	measu_amt		[13];	 /*  ��ż����ɱݾ�                  */
//	char	add_amt			[13];	 /*  �߰��㺸����                    */

}S_CHE_REAL_OUTREC;

/*****************************************************************************/
/* �ֽ��ֹ�ü�� (�ź�)                        							     */
/*****************************************************************************/
typedef struct
{
    //	�������� �ֽ�ü�� ����ü
	char 	type			[ 4];     /* ������ Type  [CHE0]        	*/
	char 	gb				[ 1];     /* ������ Type  ������        	*/
	char 	acckey			[11];     /* ���¹�ȣ Ű��              	*/
	char 	realkey			[11];     /* ������ ID                	*/
    char 	lineseq			[ 8];     /* �����Ϸù�ȣ             	*/
	char	mdgb			[ 2];		     /* ���ӱ���					         */
	char 	adminno			[ 8];     /* �����ڻ��                       */
	char	dsize			[ 5];			 /* Data size (������� ����)			 */
	char	usrfld			[15];		     /* ��������ǿ���(����̻��)	         */
	char	or_type			[ 2];	  /* �ֹ������ڵ�					  */
    char	prgb			[ 2];	 /* �ֹ�ü�ᱸ��					 */
									 /*"10":���ֹ� "11":�����ֹ�         */
									 /*"12":����ֹ�                     */ 
	char 	accno			[11];     /* ���¹�ȣ                         */
	char 	orderno			[ 10];     /* �ֹ���ȣ                         */
	char 	orgorderno		[10];     /* ���ֹ���ȣ                       */
	char 	cheno			[10];     /* ü���ȣ                         */

	char 	jmcode			[12];     /* �����ڵ�                         */
	char 	jmname			[40];     /* �����                           */
	char 	bid_tpcd			[ 2];     /* �ֹ�����                         */
	char 	ord_cond         	[ 1];    /* �ֹ�����	                     */
	char    cdgb			[ 3];	  /*  �ſ뱸��                        */
	char 	ord_tpcd			[ 2];     /* �ֹ�����                         */
	char 	ord_mkcd			[ 2];     /* �ֹ������з�                     */
	char 	medosu			[ 1];     /* �ŵ������� ( 1:�ŵ�, 2:�ż� )    */
		
	char 	jprc			[13];     /* �ֹ�����                         */
	char 	jqty			[16];     /* �ֹ�����                         */
	char 	cheprice		[13];     /* ü�ᰡ                           */
	char 	cheqty			[16];     /* ü�ᷮ                           */
	
	char 	sumprice		[16];     /* ü�ᴩ��ݾ�                     */
	char 	mecheqty		[16];     /* ��ü�����                       */
	char 	percheprice		[13];     /* ����ü�ᰡ                       */
	char 	percheqty		[16];     /* ����ü�ᷮ                       */
	
	char 	chetime			[ 8];     /* �ֹ�/ü�� �ð�                   */
    /* ���ֹ� DATA�� */
    char    og_orno             [10];    /* ���ֹ�_�ֹ���ȣ                  */
	char    og_or_type          [ 2];    /* ���ֹ�_�ֹ�����   	             */
    char    og_or_ctgb          [ 2];    /* ���ֹ�_�ֹ�ü�ᱸ��              */
    char    og_orgn_orno        [10];    /* ���ֹ�_���ֹ���ȣ                */
    char    og_ctno             [10];    /* ���ֹ�_ü���ȣ                  */
    char    og_bid_tpcd         [ 2];    /* ���ֹ�_ȣ�������ڵ�              */
    char    og_ord_cond         [ 1];    /* ���ֹ�_�ֹ�����                  */
    char    og_cdgb             [ 3];    /* ���ֹ�_�ſ뱸��                  */
    char    og_ord_tpcd         [ 2];    /* ���ֹ�_�ֹ������ڵ�              */
    char    og_ord_mkcd         [ 2];    /* ���ֹ�_�ֹ������ڵ�              */
    char    og_degb             [ 1];    /* ���ֹ�_�Ÿű��� [�ŵ�:1/�ż�:2]  */
    char    og_ord_ucs          [13];    /* ���ֹ�_�ֹ�����                  */
    char    og_orqt             [16];    /* ���ֹ�_�ֹ�����                  */
    char    og_crt_ucs          [13];    /* ���ֹ�_ü�ᰡ                    */
    char    og_ctqt             [16];    /* ���ֹ�_ü�ᷮ                    */
    char    og_ctam             [16];    /* ���ֹ�_ü�ᴩ��ݾ�              */
    char    og_opct_qt          [16];    /* ���ֹ�_��ü�����                */
    char    og_per_crt_ucs      [13];    /* ���ֹ�_����ü�ᰡ                */
    char    og_per_ctqt         [16];    /* ���ֹ�_����ü�ᷮ                */
    char    og_actm             [ 8];    /* ���ֹ�_�ֹ�/ü�� �ð�            */

}S_TREFZ;

// �ֽ��ܰ� �ǽð�
typedef struct
{
	char 	type			[ 4];     /* ������ Type  [CHE0]        	*/
	char 	gb				[ 1];     /* ������ Type  ������        	*/
	char 	acckey			[11];     /* ���¹�ȣ Ű��              	*/
	char 	realkey			[11];     /* ������ ID                	*/
    char 	lineseq			[ 8];     /* �����Ϸù�ȣ             	*/
	char	mdgb			[ 2];		     /* ���ӱ���					         */
	char 	adminno			[ 8];     /* �����ڻ��                       */
	char	dsize			[ 5];			 /* Data size (������� ����)			 */
	char	usrfld			[15];		     /* ��������ǿ���(����̻��)	         */
	char    accno			[11];  /*  ���¹�ȣ                        */
    char    jmcode			[12];  /*  �����ڵ�                        */
	char    jmname			[40];  /*  �����                          */
    char    sygb			[ 3];  /*  �ſ뱸��                        */
	char    bqty			[16];  /*  ��������                        */
	char    ordgaqty		[16];  /*  �ֹ����� ����                   */
	char    buyamt			[13];  /*  ���Դܰ�                        */
	char    bep_price		[13];  /*  BEP �ܰ�                        */
	char    menusum			[16];  /*  ���Աݾ�                        */
	char    fee				[16];  /*  ������                          */
    char    tax				[16];  /*  ����                            */
	char    holdrate		[ 6];  /*  ��������                        */
    char    sdate			[ 8];  /*  ������                          */
    char    lastday			[ 8];  /*  ������                          */
	char    sinamt			[16];  /*  �ſ�ݾ�                        */
    char    sininter		[16];  /*  �ſ�����                        */
	char    janqty			[16];  /*  �����ܰ�                        */
	char    psmevol			[16];  /*  ���ϼ��ż���                    */
    char    tsmevol			[16];  /*  ���� ���ż���                   */
	char    damboloanamt	[16];  /*  �㺸�������                    */
	char    tmedosonik		[16];  /*  ���� �� �ŵ�����                */
	char    pertrsonik		[16];  /*  ���Ͻ������ͺ�����              */
	char    trsonik			[16];  /*  ���Ͻ�������(����)              */
    char    ratesonik		[16];  /*  ���Ͻ������ͷ�(����)            */
    char    strsonik		[16];  /*  ���Ͻ�������(�ſ�)              */
    char    sratesonik		[16];  /*  ���Ͻ������ͷ�(�ſ�)            */
	char 	medoamtsum		[16];  /* �ŵ��ݾ���                       */
	char 	mesuamtsum		[16];  /* �ż��ݾ���                       */

    
}S_JAN_REAL_OUTREC;


// �����ɼ�ü�� �ǽð�
typedef struct
{
	char 	type			[ 4];     /* ������ Type  [CHE0]        	*/
	char 	gb				[ 1];     /* ������ Type  ������        	*/
	char 	acckey			[11];     /* ���¹�ȣ Ű��              	*/
	char 	realkey			[11];     /* ������ ID                	*/
    char 	lineseq			[ 8];     /* �����Ϸù�ȣ             	*/
	char	mdgb			[ 2];		     /* ���ӱ���					         */
	char 	adminno			[ 8];     /* �����ڻ��                       */
	char	dsize			[ 5];			 /* Data size (������� ����)			 */
	char	usrfld			[15];		     /* ��������ǿ���(����̻��)	         */
	char	or_type			[ 2];	  /* �ֹ������ڵ�					  */
	char	prgb			[ 2];	 /* �ֹ�ü�ᱸ��					 */
									 /*"10":���ֹ� "11":�����ֹ�         */
									 /*"12":����ֹ�                     */ 
	char 	accno			[11];     /* ���¹�ȣ                         */
	char 	orderno			[ 10];     /* �ֹ���ȣ                         */
	char 	orgorderno		[10];     /* ���ֹ���ȣ                       */
	char 	cheno			[10];     /* ü���ȣ                         */

	char 	jmcode			[12];     /* �����ڵ�                         */
	char 	jmname			[40];     /* �����                           */
	char 	bid_tpcd			[ 2];     /* ȣ�������ڵ�                         */
	char 	ord_cond         	[ 1];    /* �ֹ�����	                     */
	char 	ord_tpcd			[ 2];     /* �ֹ������ڵ�                         */
	char 	ord_mkcd			[ 2];     /* �ֹ������ڵ�                     */
	char 	medosu			[ 1];     /* �ŵ������� ( 1:�ŵ�, 2:�ż� )    */
		
	char 	jprc			[13];     /* �ֹ�����                         */
	char 	jqty			[16];     /* �ֹ�����                         */
	char 	cheprice		[13];     /* ü�ᰡ                           */
	char 	cheqty			[16];     /* ü�ᷮ                           */
	
	char 	sumprice		[16];     /* ü�ᴩ��ݾ�                     */
	char 	mecheqty		[16];     /* ��ü�����                       */
	char 	percheprice		[13];     /* ����ü�ᰡ                       */
	char 	percheqty		[16];     /* ����ü�ᷮ                       */
	
	char 	chetime			[ 8];     /* �ֹ�/ü�� �ð�                   */

	/* ���ֹ� DATA�� */
    char    og_orno             [10];    /* ���ֹ�-�ֹ���ȣ                  */
    char    og_opct_qt          [16];    /* ���ֹ�-��ü�����                */

}FO_CHE_REAL_OUTREC;

/* FX �ܰ� ������ �ǽð� */
typedef struct
{
    char    szKeyType[4];				/* ACCN */
	char	szKeyItem[11];				/* Key	*/
	char	szKeyItem1[11];				/* Key1	- �ֹ��� ID */
	char	szKeyItem2[11];				/* Key2	- ������ ID */
    char    szAccNo[11];				/* ���� ��ȣ */
    char    szBalanceUSD[16];			/* USD�ܰ�      */
    char    szBalanceKRW[16];			/* KRW�ܰ�      */
    char    szSubsUSD[16];				/* ��ȭ���USD  */
    char    szSubsKRW[16];				/* ��ȭ���KRW  */
    char    szRate[9];					/* ����ȯ��     */
} FX_JAN_REAL_OUTREC;

/* FX �������� ������ �ǽð� */
typedef struct
{
    char    szKeyType[4];				/* ACCM */
	char	szKeyItem[11];				/* Key	*/
	char	szKeyItem1[11];				/* Key1	- �ֹ��� ID */
	char	szKeyItem2[11];				/* Key2	- ������ ID */
    char    szAccNo[11];				/* ���� ��ȣ) */
    char    szMarginReq[12];			/* FX ���ű� */
    char    szMtMargin[12];				/* �������ű� */
    char    szOrderMg[10];				/* �ֹ����ű�   */
    char    szIsHedge[1];				/* ��������     */
} FX_DET_REAL_OUTREC;

/* FX ü�� �ǽð� */
typedef struct  
{
	char	szKeyType[4];				/*  [CH01]						*/
	char	szKeyItem[11];				/*  Key		- ���ڹ�ȣ			*/
	char	szKeyItem1[11];				/*  Key1	- �ֹ��� ID			*/
	char	szKeyItem2[11];				/*  Key2	- ������ ID			*/
	char	szAccNo[11];				/*	[���¹�ȣ]					*/
	char	szMemberNo[3];				/*  [ȸ����ID]					*/
	char	szClOrdID[80];				/*  [�ֹ� ������ȣ]				*/
	char	szNoticeNo[20];				/*  [������ȣ]					*/
	char	szOrdType[5];				/*  [�ֹ�����]					*/
	char	SHCODE[12];					/*  [�����]					*/
	char	szOrderID[20];				/*  [�ŷ��� ������ȣ]			*/
	char	szPGCode[2];				/*  [�����ڱ���]				*/
	char	szRate[10];					/*  [��������]					*/
	char	szQuote[10];				/*  [���簡]					*/
	char	szExeTime[30];				/*  [�����ð�]					*/
	char	szOrgCustItem1[11];			/*  [�ֹ��� ȸ��ó���׸�1]		*/
	char	szOrgCustItem2[14];			/*  [�ֹ��� ȸ��ó���׸�2]		*/
	char	szOrgCustItem3[55];			/*  [�ֹ��� ȸ��ó���׸�3]		*/
	char	szCustItem1[11];			/*  [ȸ��ó���׸� 1]			*/
	char	szCustItem2[14];			/*  [ȸ��ó���׸� 2]			*/
	char	szCustItem3[55];			/*  [ȸ��ó���׸� 3]			*/
	char	szSide[3];					/*  [�Ÿű���]					*/
	char	szOrgTicketNo[20];			/*  [��ü���ȣ]				*/
	char	szOpenTime[30];				/*  [�ű��ֹ�ü��ð�(��ȯ��)]	*/
	char	szAccName[20];				/*  [���¸�]					*/
	char	szUserID[20];				/*  [��ID]					*/
	char	szFXCMPosID[20];			/*  [TicketNo]					*/
    char    szCommition[12];			/* ������ */
    char    szAmount[12];				/* �������� */
    char    szTotalPL[12];				/* �̼��� */
    char    szGrossPL[12];				/* ���ϼ���(�������� ȯ��� ����) */
    char    szRevRate[12];				/* ü��� �ݴ����� */
    char    szOpenMarkup[12];			/* OM (��ȯ�Ž� ���) */
    char    szInterest[12];				/* ���� */
    char    szCloseMarkup[12];			/* CM (��ȯ�Ž� ���) */
    char    szMtMargin[12];				/* �������ű� (�����Ǽ� * �ֹ����ű�) */
    char    szOrderMargin[12];			/* �ֹ����ű� (�ֹ��� * �ֹ����ű�) */
    char    szOutBalance[16];			/* �̰����ݾ� */
    char    szPipCost[12];				/* ü��� Pip Cost */
	char	szPipLowest[4];				/* �Ҽ��� ��ȿ�ڸ��� */
} FX_CHE_REAL_OUTREC;

/* FX ��ü�� �ǽð� */
typedef struct  
{
	char	szKeyType[4];				/*  [OR01]						*/
	char	szKeyItem[11];				/*  Key		- ���¹�ȣ			*/
	char	szKeyItem1[11];				/*  Key1	- �ֹ��� ID			*/
	char	szKeyItem2[11];				/*  Key2	- ������ ID			*/
	char	szAccNo[11];				/*  [���¹�ȣ]					*/
    char    szAmount[12];				/*	����						*/
    char    szMtMargin[12];				/*	�������ű�					*/
    char    szOrderMargin[12];			/*	�ֹ����ű�					*/
	char	szStatus[1];				/*  [���� - 0:������, 1:����]   */
	char	SHCODE[12];					/*  [�����]					*/
	char	szSide[3];					/*  [�Ÿű���]					*/
	char	szOrdType[5];				/*  [�ֹ�����]					*/
	char	szRate[10];					/*  [�ֹ�ȯ��]					*/
	char	szOpenTime[30];				/*  [�ֹ��ð�]					*/
	char	szCustItem1[11];			/*  [ȸ��ó���׸� 1]			*/
	char	szCustItem2[14];			/*  [ȸ��ó���׸� 2]			*/
	char	szCustItem3[55];			/*  [ȸ��ó���׸� 3]			*/
	char	szOrgCustItem1[11];			/*  [������ ȸ��ó���׸�1]		*/
	char	szOrgCustItem2[14];			/*  [������ ȸ��ó���׸�2]		*/
	char	szOrgCustItem3[55];			/*  [������ ȸ��ó���׸�3]		*/
	char	szPipLowest[4];				/* �Ҽ��� ��ȿ�ڸ���			*/
} FX_MIC_REAL_OUTREC;

/* FX StopLimit �ǽð� */
typedef struct  
{
	char	szKeyType[4];				/*  [CLSL]						*/
	char	szKeyItem[11];				/*  Key		- ���¹�ȣ			*/
	char	szKeyItem1[11];				/*  Key1	- �ֹ��� ID			*/
	char	szKeyItem2[11];				/*  Key2	- ������ ID			*/
	char	szAccNo[11];				/*  [���¹�ȣ]					*/
	char    szAmount[12];				/*  [����]						*/
	char	szStatus[1];				/*  [�ֹ� ����,ü�� ����]		*/
	char	szStop[10];					/*  [Stop]						*/
	char	szLimit[10];				/*  [Limit]						*/
	char	szOrgCustItem1[11];			/*  [���ֹ�ȸ��ó���׸� 1]		*/
	char	szOrgCustItem2[14];			/*  [���ֹ�ȸ��ó���׸� 2]		*/
	char	szOrgCustItem3[55];			/*  [���ֹ�ȸ��ó���׸� 3]		*/
	char	szCustItem1[11];			/*  [SL ȸ��ó���׸� 1]			*/
	char	szCustItem2[14];			/*  [SL ȸ��ó���׸� 2]			*/
	char	szCustItem3[55];			/*  [SL ȸ��ó���׸� 3]			*/
	char	SHCODE[12];					/*  [�����]					*/
	char	szSide[3];					/*  [�Ÿű���]					*/
	char	szOrdType[5];				/*  [�ֹ�����]					*/
	char	szOpenTime[30];				/*  [�ֹ��ð�]					*/
	char	szTicketNo[20];				/*  [ü���ȣ]					*/
} FX_CSL_REAL_OUTREC;

/* FX IFO �ǽð� */
typedef struct  
{
	char	szKeyType[4];				/* [FIFO]						*/
	char	szKeyItem[11];				/* Key	- ���¹�ȣ				*/
	char	szKeyItem1[11];				/* Key1	- �ֹ��� ID				*/
	char	szKeyItem2[11];				/* Key2	- ������ ID				*/
	char	szAccNo[11];				/* ���¹�ȣ						*/
	char	szEntry[10];				/* Entry Price					*/
	char	szStop[10];					/* Stop  Price					*/
	char	szLimit[10];				/* Limit Price					*/
	char	szTrailingStop[10];			/* TrailingStop Price			*/
	char	szOrgCustItem1[11];			/* ��ȸ��ó���׸�1				*/
	char	szOrgCustItem2[14];			/* ��ȸ��ó���׸�2				*/
	char	szOrgCustItem3[55];			/* ��ȸ��ó���׸�3				*/
	char	szEntryItem1[11];			/* ȸ��ó���׸�1				*/
	char	szEntryItem2[14];			/* ȸ��ó���׸�2				*/
	char	szEntryItem3[55];			/* ȸ��ó���׸�3				*/
	char	szStatus[1];				/* ����							*/
} FX_IFO_REAL_OUTREC;

/* FX �ź� �ǽð� */
typedef struct
{
	char	szKeyType[4];				/*  [REJE]						*/
	char	szKeyItem[11];				/*  Key		- ���¹�ȣ			*/
	char	szKeyItem1[11];				/*  Key1	- �ֹ��� ID			*/
	char	szKeyItem2[11];				/*  Key2	- ������ ID			*/
	char	szAccNo[11];				/*  [���¹�ȣ]					*/
    char    szAmount[12];			    /*  ��������					*/
    char    szMtMargin[12];				/*  �������ű�					*/
    char    szOrderMargin[12];			/*  �ֹ����ű�					*/
	char	szCustItem1[11];			/*  [ȸ��ó���׸� 1]			*/
	char	szCustItem2[14];			/*  [ȸ��ó���׸� 2]			*/
	char	szCustItem3[55];			/*  [ȸ��ó���׸� 3]			*/
	char	SHCODE[12];					/*  [�����]					*/
	char	szSide[3];					/*  [�Ÿű���]					*/
	char	szRate[10];					/*  [��������]					*/
	char	szOrdType[5];				/*  [�ֹ�����]					*/
	char	szRejectTime[30];			/*  [�źνð�]					*/
} FX_REJ_REAL_OUTREC;

/* FX �ǽð� �ü� */
typedef struct  
{
	char		KEYTYPE[4];				/*  [9212]						*/
	char		SHCODE[12];				/*  [�����ڵ�]					*/
	char		MEMBER_NO[3];			/*  [ȸ���� ID]					*/
	char		TRADE_TYPE[2];			/*  [���񱸺�]					*/
	char		REAL_TIME[14];			/*  [�ð�]						*/
	char		BID[10];				/*  [���簡(BID) : ���簡(BID)] */
	char		BIDOPEN_PRICE[10];		/*  [�ð�(BID)]					*/
	char		BIDHIGH_PRICE[10];		/*  [��(BID)]					*/
	char		BIDLOW_PRICE[10];		/*  [����(BID)]					*/
	char		BIDCLOSE_PRICE[10];		/*  [����(BID)]					*/
	char		BID_SIGN[1];			/*  [����� ��ȣ(BID)]			*/
	char		BID_CHANGE_PRICE[10];	/*  [�����(BID)]				*/
	char		OFFER[10];				/*  [���簡(OFFER)]				*/
	char		OFFEROPEN_PRICE[10];	/*  [�ð�(OFFER)]				*/
	char		OFFERHIGH_PRICE[10];	/*  [��(OFFER)]				*/
	char		OFFERLOW_PRICE[10];		/*  [����(OFFER)]				*/
	char		OFFERCLOSE_PRICE[10];	/*  [����(OFFER)]				*/
	char		OFFER_SIGN[1];			/*  [����� ��ȣ(OFFER)]		*/
	char		OFFER_CHANGE_PRICE[10];	/*  [�����(OFFER)]				*/
	char		PIP_COST[10];			/*  [���ڽ�Ʈ]					*/
	char		PIP_LOWEST[3];			/*  [��ȿ�ڸ���]				*/
	char		INTEREST_BUY[10];		/*  [����(Buy)]					*/
	char		INTEREST_SELL[10];		/*  [����(Sell)]				*/
	char		QUOTE_ID[80];			/*  [�ü� ������ȣ]				*/
} FX_QUOTE_REAL_OUTREC;

// �����ɼ� �ܰ� �ǽð�
typedef struct
{
	char 	type			[ 4];     /* ������ Type  [CHE0]        	*/
	char 	gb				[ 1];     /* ������ Type  ������        	*/
	char 	acckey			[11];     /* ���¹�ȣ Ű��              	*/
	char 	realkey			[11];     /* ������ ID                	*/
    char 	lineseq			[ 8];     /* �����Ϸù�ȣ             	*/
	char	mdgb			[ 2];		     /* ���ӱ���					         */
	char 	adminno			[ 8];     /* �����ڻ��                       */
	char	dsize			[ 5];			 /* Data size (������� ����)			 */
	char	usrfld			[15];		     /* ��������ǿ���(����̻��)	         */
	char    accno         [11];		/*  ���¹�ȣ                        */
    char    jmcode        [12];     /*  �����ڵ�  "XXXXXXXX"            */
    char    jmname        [40];     /*  �����                          */
	char    bqty          [16];		/*  ��������                        */
									/*  ������ ' '                      */
	char    buyamt        [13];		/*  ��ո�������                    */
									/*  ��ո�������/100                */
    char    mesusum       [16];		/*  �Ѹ��Աݾ�                      */
									/*  �Ѹ��Աݾ�/1000+0.5             */
    char    ordgaqty      [16];		/*  û�갡�� ����                   */
    char    tsmevol       [16];		/*  ���ϼ��ż���                    */
									/*  ���ϸż�����-���ϸŵ�����       */
    char    medosu        [ 1];		/*  �ŵ�/�ż� ����                  */
	char    fee           [16];		/*  ������                          */
    char    tax           [16];		/*  ����                            */
	char    preqty        [16];		/*  ���Ϻ�������		            */
	char    pertrsonik    [16];     /*  ���Ͻ������ͺ�����              */
	char 	medoamtsum    [16];  /* �ŵ��ݾ���                       */
	char 	mesuamtsum    [16];  /* �ż��ݾ���                       */

}FO_JAN_REAL_OUTREC;

// �����ɼ� �ܰ� �ǽð�
typedef struct
{
	char 	type          [ 4];		/*	������ Type                      */
	char 	gubun         [ 1];		/*	������ Gb                        */
	char 	acckey		  [11];     /*	���¹�ȣ Ű��					*/
	char 	realkey       [16];		/*	������ ID                        */
    char 	lineseq       [ 8];		/*	�����Ϸù�ȣ                     */
	char	order_amt	  [13];	    /*  �ֹ���������                    */
	char	order_dae	  [13];	    /*  �ֹ����ɴ��                    */
	char	measu_amt	  [13];	    /*  ��ż����ɱݾ�                  */
	char	add_amt		  [13];	    /*  �߰��㺸����                    */
}AMT_REAL_OUTREC;


// �ؿܼ��� ü�� �ǽð�
typedef struct
{
	char		push_div[4];		/*  [�ǽð� ���� FEXC]     */
	char		push_key[11];		/*  [Key]     */
	char		push_key1[11];		/*  [Key1 : �ֹ���ID]     */
	char		push_key2[11];		/*  [Key2 : ������ID]     */
	char		pkt_div[4];			/*  [��Ŷ����]     */
	char		acct_no[11];		/*  [���¹�ȣ]     */
	char		acct_nm[50];		/*  [���¸�]     */
	char		ord_no[5];			/*  [�ֹ���ȣ]     */
	char		orig_ord_no[5];		/*  [���ֹ���ȣ]     */
	char		series[32];			/*  [�����ڵ�]     */
	char		bysl_tp[1];			/*  [�Ÿű���]     */
	char		proc_stat[1];		/*  [�ֹ�ó������]     */
	char		ord_q[10];			/*  [�ֹ�����]     */
	char		trd_q[10];			/*  [ü�����]     */
	char		ord_remq[10];		/*  [�ܷ�]     */
	char		ord_p[19];			/*  [�ֹ�����]     */
	char		trd_p[19];			/*  [ü�ᰡ��]     */
	char		mdfy_cncl_q[10];	/*  [�������]     */
	char		prce_tp[1];			/*  [���ݱ���]     */
	char		stop_p[19];			/*  [���ǰ���]     */
	char		ordtype[1];			/*  [�ֹ�����]     */
	char		ord_tp[1];			/*  [�ֹ�����]     */
	char		acpt_time[10];		/*  [�ŷ��������ð�]     */
	char		kr_ord_date[8];		/*  [�ѱ��ֹ�����]     */
	char		kr_acpt_time[10];	/*  [�ѱ������ð�]     */
	char		kr_che_time[10];	/*  [�ѱ�ü��ð�]     */
	char		ord_way_tp[1];		/*  [��ü����]     */
	char		work_empl[9];		/*  [�����ID]     */
	char		crc_cd[3];			/*  [��ȭ]     */

}FR_CHE_REAL_OUTREC;

// �ؿܼ��� �ܰ� �ǽð�
typedef struct
{
	char		push_div[4];		/*  [�ǽð� ���� FOPN]     */
	char		push_key[11];		/*  [Key]     */
	char		push_key1[11];		/*  [Key1 : �ֹ���ID]     */
	char		push_key2[11];		/*  [Key2 : ������ID]     */
	char		acct_no[11];        /* ���¹�ȣ             */
    char		acct_nm[50];        /* ���¸�               */
	char		t_pospl[19];		/*  [�̰��������򰡱ݾ�]     */
	char		t_open_pl[19];		/*  [�򰡼���]     */
	char		t_invtrate[19];		/*  [������]     */
	char		t_real_pl[19];		/*  [��������]     */
	char		t_pl[19];			/*  [�Ѽ���]     */
	char		t_cmpr_pl[19];		/*  [����������]     */
	char		t_pres_asset[19];	/*  [�����ڻ�]     */
	char		series[32];			/*  [����]     */
	char		series_nm[50];		/*  [�����]     */
	char		crc_cd[3];			/*  [�ŷ���ȭ]     */
	char		buysellsect[4];		/*  [����]     */
	char		open_q[10];			/*  [�̰�������]     */
	char		rsrb_ableqty[10];	/*  [û�갡�ɼ���]     */
	char		open_uv[19];		/*  [��վ�����]     */
	char		last_p[19];			/*  [���簡]     */
	char		open_pl[19];		/*  [�򰡼���]     */
	char		invtrate[19];		/*  [������]     */
	char		ctrt_size[19];		/*  [������]     */
	char		dealamt[19];		/*  [ü��ݾ�]     */
	char		pospl[19];			/*  [�̰��������򰡱ݾ�]     */

}FR_JAN_REAL_OUTREC;

/*************************************************************************************/
//	�ܰ� ������ ���� ����ü
/*************************************************************************************/
//	���� ������ ����ü
typedef struct 
{
	char	accNo			[11 + 1];		// ���¹�ȣ
	char	accName			[40 + 1];		// ���¸�
	char	accPasswd		[ 8 + 1];		// ��й�ȣ
	char	accdetlgb		[ 2 + 1];		// ���»�����
	BYTE	accgb;						// ���±��� ['1': ����, '2': ����, '0':�ش���׾��� ]	
	BYTE	recordtp;					// ��й�ȣ���忩��(1:����, 0:������)
	char	taccno			[11 + 1];		// ���հ��¹�ȣ
	char	fee1			[ 8 + 1];		// ������ 1 [default �� �����̽� ó��]
	char	fee2			[ 8 + 1];		// ������ 2 [default �� �����̽� ó��]	
	char	fee3			[ 8 + 1];		// ������ 3 [default �� �����̽� ó��]	
	char	fee4			[ 8 + 1];		// ������ 4 [default �� �����̽� ó��]	
	char	accPenName		[30 + 1];		// ���¸� (�ʸ�)
	char	accOrgPasswd	[ 8 + 1];		// ���� ��й�ȣ
	BYTE	realdatatp;					// �ֹ�/ü�� ���� ����Ÿ ó������ (1:ó��, 0:��ó��)
	char	filler			[10    ];		// 
	char	order_amt		[13 + 1];		// �ֹ���������(KTB ������)
	char	order_dae		[13 + 1];		// �ֹ����ɴ��(KTB ������)
	char	measu_amt		[13 + 1];		// ��ż����ɱݾ�(KTB ������)
	char	add_amt			[13 + 1];		// �߰��㺸����(KTB ������)
	char	credit			[13 + 1];		// �����ѵ�(KTB ������)
	char	acc_amt			[ 1 + 1];		// �������ű�(KTB ������)
	char    trsonik			[16 + 1];		/*  ���� ��������              */
    char    sonik_fee		[16 + 1];		/*  �������Ϳ� ���� ������					*/
    char    sonik_tax		[16 + 1];		/*  �������Ϳ� ���� ����					*/

//	char	managegb		[ 1 + 1];		// ü�ܿ��� ������ �������� ����... ( �������� ���� �����ʹ� �����ϰ�... ���� ȭ�鿡�� ��ȸ�� ���´� ȭ���� ������� ���ֹ�����... ���� )
//											// 0 : ����� ����, 1 : ��������

	char    t_real_pl		[19 + 1];       /* �ؿܽ�������(19)                    */
    char    t_cmpr_pl		[19 + 1];       /* �ؿ�����������(19)                */
    char    t_pres_asset	[19 + 1];       /* �ؿ������ڻ�(19)                    */
	char    fee_level		[ 1 + 1];		/* �ؿܼ��� ��������(1)			   */
	char    e_sell			[ 8 + 1];       /* ��ȭ�ŵ�ȯ��(8.4)				   */
	char    d_buy			[ 8 + 1];       /* �޷��ż�ȯ��(8.4)                   */

	CObArray arrJan;
	CObArray arrChe;
	CObArray arrFXPos;
	CObArray arrFrJan;
	CObArray arrFrChe;
} CHEJAN_ACC_STRUCT;

//	�ֽ� �ܰ� ����ü
typedef struct 
{
	char 	type          [ 4 + 1 ];  /* ������ Type                      */
	char 	gubun         [ 1 + 1 ];  /* ������ Gb                        */
	char 	realkey       [11 + 1 ];  /* ������ ID                        */
    char 	lineseq       [ 8 + 1 ];  /* �����Ϸù�ȣ                     */
	char    accno         [11 + 1 ];  /*  ���¹�ȣ                        */
    char    jmcode        [12 + 1 ];  /*  �����ڵ�                        */
	char    jmname        [40 + 1 ];  /*  �����                          */
    char    sygb          [ 3 + 1 ];  /*  �ſ뱸��                        */
	char    bqty          [16 + 1 ];  /*  ��������                        */
	char    ordgaqty      [16 + 1 ];  /*  �ֹ����� ����                   */
	char    buyamt        [13 + 1 ];  /*  ���Դܰ�                        */
	char    bep_price     [13 + 1 ];  /*  BEP �ܰ�                        */
	char    menusum       [16 + 1 ];  /*  ���Աݾ�                        */
    char    estamt        [16 + 1 ];  /*  �򰡱ݾ�                        */
	char    estsonik      [16 + 1 ];  /*  �򰡼���                        */
    char    rate          [ 8 + 1 ];  /*  ���ͷ�                          */
//	char    fee           [16 + 1 ];  /*  ������                          */		//	Ŭ���̾�Ʈ���� ����ؼ� ���
//	char    tax           [16 + 1 ];  /*  ����                            */		//	Ŭ���̾�Ʈ���� ����ؼ� ���
	char    holdrate      [ 6 + 1 ];  /*  ��������                        */
    char    sdate         [ 8 + 1 ];  /*  ������                          */
    char    lastday       [ 8 + 1 ];  /*  ������                          */
    char    sinamt        [16 + 1 ];  /*  �ſ�ݾ�                        */
    char    sininter      [16 + 1 ];  /*  �ſ�����                        */
    char    janqty        [16 + 1 ];  /*  �����ܰ�                        */
    char    psmevol       [16 + 1 ];  /*  ���ϼ��ż���                    */
    char    tsmevol       [16 + 1 ];  /*  ���� ���ż���                   */
	char    damboloanamt  [16 + 1 ];  /*  �㺸�������                    */
	char    tmedosonik    [16 + 1 ];  /*  ���� �� �ŵ�����                */
	char    trsonik       [16 + 1 ];  /*  ���Ͻ�������(����)              */
    char    ratesonik     [16 + 1 ];  /*  ���Ͻ������ͷ�(����)            */
    char    strsonik      [16 + 1 ];  /*  ���Ͻ�������(�ſ�)              */
    char    sratesonik    [16 + 1 ];  /*  ���Ͻ������ͷ�(�ſ�)            */
	char    pjogaest      [16 + 1 ];  /*  �������� ������               */
	char    pjogasonik    [16 + 1 ];  /*  �������� ���� �򰡼���          */
    char    pjogarate     [16 + 1 ];  /*  �������� ���� ���ͷ�            */
	char 	medoamtsum    [16 + 1 ];  /* �ŵ��ݾ���                       */
	char 	mesuamtsum    [16 + 1 ];  /* �ż��ݾ���                       */
	char    giga          [ 9 + 1 ];  /*  ���ذ�                          */
	char    uplmtprice    [ 9 + 1 ];  /*  ���Ѱ�                          */
	char    dnlmtprice    [ 9 + 1 ];  /*  ���Ѱ�                          */
	char    price         [ 9+ 1 ];   /*  ���簡                          */
	char    mesuga        [ 9 + 1 ];  /*  (�ֿ켱)�ż�ȣ��                */
	char    medoga        [ 9 + 1 ];  /*  (�ֿ켱)�ŵ�ȣ��                */
	char    sign          [ 1 + 1 ];  /*  ���ϱ���                        */
	char    change        [ 8 + 1 ];  /*  ���ϴ��                        */
    char    diff          [ 6 + 1 ];  /*  ����� ( �Ҽ��� 2�ڸ� )         */
    char    volume        [16 + 1 ];  /*  �ŷ���                          */


} S_JAN_STRUCT;

//	�ֽ� ü�� ����ü
typedef struct 
{
	char	or_type	   [ 2 + 1 ];				/* �ֹ������ڵ�					  */
	char	prgb       [ 2 + 1 ];				/* �ֹ�ü�ᱸ��						*/
	char 	accno      [11 + 1 ];				/* ���¹�ȣ                         */
	char 	orderno    [10 + 1 ];				/* �ֹ���ȣ                         */
	char 	orgorderno [10 + 1 ];				/* ���ֹ���ȣ                       */
	char 	cheno      [10 + 1 ];				/* ü���ȣ                         */
	char 	jmcode     [12 + 1 ];				/* �����ڵ�                         */
	char 	jmname     [40 + 1 ];				/* �����                           */
	char 	bid_tpcd   [ 2 + 1 ];				/* ȣ�������ڵ�                     */
	char 	ord_cond   [ 1 + 1 ];				/* �ֹ�����                         */
	char 	cdgb	   [ 3 + 1 ];				/* �ſ뱸��                         */
	char 	ord_tpcd   [ 2 + 1 ];				/* �ֹ������ڵ�                     */
	char 	ord_mkcd   [ 2 + 1 ];				/* �ֹ������ڵ�                     */
	char 	medosu     [ 1 + 1 ];				/* �ŵ������� ( 1:�ŵ�, 2:�ż� )    */
	char 	jprc       [13 + 1 ];				/* �ֹ�����                         */
	char 	jqty       [16 + 1 ];				/* �ֹ�����                         */
	char 	cheprice   [13 + 1 ];				/* ü�ᰡ                           */
	char 	cheqty     [16 + 1 ];				/* ü�ᷮ                           */
	char 	sumprice   [16 + 1 ];				/* ü�ᴩ��ݾ�                     */
	char 	mecheqty   [16 + 1 ];				/* ��ü�����                       */
	char 	chetime    [ 8 + 1 ];				/* �ֹ��ð�							*/
	char 	price      [ 9 + 1 ];				/* ���簡                           */
	char 	sign       [ 1 + 1 ];				/* ���ϴ���ȣ                     */
	char 	medoga     [ 9 + 1 ];				/* (�ֿ켱)�ŵ�ȣ��                 */
	char 	mesuga     [ 9 + 1 ];				/* (�ֿ켱)�ż�ȣ��                 */
//	char 	percheprice[ 9 + 1 ];				/* ����ü�ᰡ                       */
//	char 	percheqty  [ 9 + 1 ];				/* ����ü�ᷮ                       */
//	char 	memefee    [16 + 1 ];				/* ���ϸŸ� ������                  */
//	char 	memetax    [16 + 1 ];				/* ���ϸŸż���                     */
} S_CHE_STRUCT;

// �����ɼ�ü�� ����ü
typedef struct
{
	char 	type			[ 4 + 1 ];		/* ������ Type  [CHE1]     	    	 */
	char 	gb			    [ 1 + 1 ];		/* ������ Type  ������        		 */
	char 	realkey		    [11 + 1 ];		/* ������ ID        		         */
    char 	lineseq		    [ 8 + 1 ];		/* �����Ϸù�ȣ            		 	 */
	char	or_type			[ 2 + 1 ];		/* �ֹ������ڵ�					  */
	char	prgb		    [ 2 + 1 ];		/* �ֹ�ü�������ڵ�				     */
	char 	accno           [11 + 1 ];		/* ���¹�ȣ                          */
	char 	orderno         [10 + 1 ];		/* �ֹ���ȣ                          */ 
	char 	orgorderno      [10 + 1 ];		/* ���ֹ���ȣ                        */
	char 	cheno           [10 + 1 ];		/* ü���ȣ							 */
	char 	jmcode          [12 + 1 ];		/* �����ڵ�   "XXXXXXXX"             */
	char 	jmname          [40 + 1 ];		/* �����                            */
	char 	bid_tpcd        [ 2 + 1 ];		/* ȣ�������ڵ�                      */
	char 	ord_cond        [ 1 + 1 ];		/* �ֹ�����                          */
	char 	ord_tpcd        [ 2 + 1 ];		/* �ֹ������ڵ�                      */
	char 	ord_mkcd        [ 2 + 1 ];		/* �ֹ������ڵ�                      */
	char 	medosu          [ 1 + 1 ];		/* �ŵ������� [�ŵ�:1/�ż�:2]        */ 
	char 	jprc            [13 + 1 ];		/* �ֹ�����                          */
	char 	jqty            [16 + 1 ];		/* �ֹ�����                          */
	char 	cheprice        [13 + 1 ];		/* ü�ᰡ							 */	
	char 	cheqty			[16 + 1 ];		/* ü�ᷮ							 */
	char 	sumprice        [16 + 1 ];		/* �����ݾ��հ�                      */
	char 	mecheqty        [16 + 1 ];		/* ��ü�����                        */
	char 	chetime         [ 8 + 1 ];		/* �ֹ��ð�					         */ 
	char 	price           [ 9 + 1 ];		/* ���簡							 */
	char    sign            [ 1 + 1 ];		/* ���ϱ���							 */
	char 	medoga          [ 9 + 1 ];		/* (�ֿ켱)�ŵ�ȣ��					 */
	char 	mesuga          [ 9 + 1 ];		/* (�ֿ켱)�ż�ȣ��					 */
}FO_CHE_STRUCT;

// �����ɼ� �ܰ� ����ü
typedef struct
{
	char 	type          [ 4 + 1 ];	/* ������ Type                      */
	char 	gubun         [ 1 + 1 ];	/* ������ Gb                        */
	char 	realkey       [11 + 1 ];	/* ������ ID                        */
    char 	lineseq       [ 8 + 1 ];	/* �����Ϸù�ȣ                     */
	char    accno         [11 + 1 ];	/*  ���¹�ȣ                        */
    char    jmcode        [12 + 1 ];    /*  �����ڵ�  "XXXXXXXX"            */
    char    jmname        [40 + 1 ];    /*  �����                          */
	char    bqty          [16 + 1 ];	/*  ��������                        */
	char    buyamt        [13 + 1 ];	/*  ��ո�������                    */
									 	/*  ��ո�������/100                */
    char    mesusum       [16 + 1 ];	/*  �Ѹ��Աݾ�                      */
									 	/*  �Ѹ��Աݾ�/1000+0.5             */
    char    ordgaqty      [16 + 1 ];	/*  û�갡�� ����                   */
    char    tsmevol       [16 + 1 ];	/*  ���ϼ��ż���                    */
								 		/*  ���ϸż�����-���ϸŵ�����       */
    char    medosu        [ 1 + 1 ];	/*  �ŵ�/�ż� ����                  */
	char    fee           [16 + 1 ];	/*  ������                          */
    char    tax           [16 + 1 ];	/*  ����                            */
	char    preqty        [16 + 1 ];	/*  ���Ϻ�������		            */
	char 	medoamtsum    [16 + 1 ];	/* �ŵ��ݾ���                       */
	char 	mesuamtsum    [16 + 1 ];	/* �ż��ݾ���                       */
	char    price         [ 9 + 1 ];	/*  ���簡                          */
								  	 	/*  0�̸� ���ذ�,                   */
								 		/*  ���ذ��� 0�̸� ��������         */
    char    sign          [ 1 + 1 ];	/*  ���ϱ���                        */
    char    medoga        [ 9 + 1 ];	/*  (�ֿ켱)�ŵ�ȣ��                */
    char    mesuga        [ 9 + 1 ];	/*  (�ֿ켱)�ż�ȣ��                */
	
}FO_JAN_STRUCT;

// �ؿܼ��� ü�� ����ü
typedef struct
{
	char    acctno			[ 11 + 1 ];				/* ���¹�ȣ(11)                    */
	char    ord_no			[ 5 + 1 ];				/* �ֹ���ȣ(5)                     */
    char    orig_ord_no		[ 5 + 1 ];              /* ���ֹ���ȣ(5)                   */
    char    series			[ 32 + 1 ];             /* ����(32)                        */
    char    bysl_tp			[ 1 + 1 ];              /* ����(1) 1:�ż� 2:�ŵ�           */
    char    proc_stat		[ 1 + 1 ];              /* �ֹ�ó������(1)                 */
													/* 0:������             */
													/* 1:��������           */
													/* 2:�����ź�           */
													/* 3:���۽���           */
													/* A:ü��               */
													/* B:�κ�ü��           */
    char    ord_q			[ 10 + 1 ];             /* �ֹ���(10)                      */
    char    trd_q			[ 10 + 1 ];             /* ü�ᷮ(10)                      */
    char    remainqty		[ 10 + 1 ];             /* �ܷ�(10)                        */
    char    ord_p			[ 19 + 1 ];             /* �ֹ���(19)                      */
    char    trd_p			[ 19 + 1 ];             /* ü�ᰡ(19)                      */
    char    mdfy_cncl_q		[ 19 + 1 ];             /* �������(19)                    */
    char    prce_tp			[ 1 + 1 ];              /* ���ݱ���(1) 1:������ 2:���尡   */
    char    stop_p			[ 19 + 1 ];             /* ���ǰ���(19)                    */
    char    ordtype			[ 1 + 1 ];              /* �ֹ�����(1)                     */
													/* 1 �Ϲ� 2 STOP ORDER 3 OCO ORDER 4 ���ݿ��� 5 MIT ORDER */
	char	ord_tp			[ 1 + 1 ];				/* �ֹ�����(1)					   */
													/* 1:�ű��ֹ�           */
													/* 2:�����ֹ�           */
													/* 3:����ֹ�           */
    char    acpt_time		[ 10 + 1 ];             /* �����ð�(10)                    */
    char    kr_ord_date		[ 8 + 1 ];              /* �ѱ��ֹ�����(8)                 */
    char    kr_acpt_time	[ 10 + 1 ];             /* �ѱ������ð�(10)                */
    char    kr_che_time		[ 10 + 1 ];             /* �ѱ�ü��ð�(10)                */
    char    ord_way_tp		[ 1 + 1 ];              /* ��ü����(1)                     */
													/* 1:FRONT              */
													/* 2:HTS                */
													/* 4:����ü��           */
    char    work_empl		[ 9 + 1 ];              /* �����ID(9)                     */
    char    crc_cd			[ 3 + 1 ];              /* ��ȭ�ڵ�(3)                     */
	char	sign			[ 1 + 1 ];				/* ����ȣ(1)					   */
	char	price			[ 19 + 1 ];				/* ���簡(19)					   */
}FR_CHE_STRUCT;

// �ؿܼ��� �ܰ� ����ü
typedef struct
{
	char    acctno			[ 11 + 1 ];				/* ���¹�ȣ(11)                    */
	char    series			[ 32 + 1 ];             /* ����(32)                        */
    char    series_nm		[ 50 + 1 ];             /* �����(50)                      */
    char    crc_cd			[ 3 + 1 ];              /* �ŷ���ȭ(3)                     */
    char    buysellsect		[ 4 + 1 ];              /* ����(4)                         */
    char    open_q			[ 10 + 1 ];             /* �̰�������(10)                  */
    char    rsrb_ableqty	[ 10 + 1 ];             /* û�갡�ɼ���(10)                */
    char    open_uv			[ 19 + 1 ];             /* ��վ�����(19)                  */
	char	sign			[ 1 + 1 ];				/* ����ȣ(1)					   */
    char    last_p			[ 19 + 1 ];             /* ���簡(19)                      */
	char	mesu_hoga		[ 19 + 1 ];				/* �ż�ȣ��(19)					   */
	char	medo_hoga		[ 19 + 1 ];				/* �ŵ�ȣ��(19)					   */
    char    open_pl			[ 19 + 1 ];             /* �򰡼���(19)                    */
    char    invtrate		[ 19 + 1 ];             /* ������(19)                      */
    char    ctrt_size		[ 19 + 1 ];             /* ������(19)                    */
    char    dealamt			[ 19 + 1 ];             /* ü��ݾ�(19)                    */
    char    pospl			[ 19 + 1 ];             /* �̰��������򰡱ݾ�(19)          */
}FR_JAN_STRUCT;

// �ؿ��ֽ� ü��
typedef struct
{
    char    type			[ 4];          /* ������ Type                          */
    char    gubun			[ 1];          /* ������ Gb                            */
    char    key1			[11];          /* KEY (���¹�ȣ)                       */
    char    key2			[16];          /* KEY2 = usrfld                        */
    char    lineseq			[ 8];          /* �����Ϸù�ȣ                         */
    char  MsgType           [20];			/* �޼�������                           */
                                     /* TAF_S00  : �ű��ֹ�                  */
                                     /* TAF_S01  : �����ֹ�                  */
                                     /* TAF_S02  : ����ֹ�                  */
                                     /* TAF_R02  :  ����Ȯ��                 */
                                     /* TAF_R04  :  ���Ȯ��                 */
                                     /* TAF_R05  :  �ֹ�ü��                 */
                                     /* TAF_R06  :  �ֹ��ź� (ER)            */
                                     /* TAF_R07  :  �ֹ��ź� (OCR)           */
									 /* TAF_R08  :  �ֹ��ź� (OMS)           */
    char  Account           [20];    /* ���¹�ȣ                             */
    char  Currency           [3];    /* "HKD"                                */
    char  OrderID           [30];    /* ������ȣ                             */
    char  ClOrderID          [8];    /* �ֹ���ȣ   (������ȣ3 + �ֹ���ȣ 5)  */
    char  OrigClOrderID      [8];    /* ���ֹ���ȣ (������ȣ3 + �ֹ���ȣ 5)  */
    char  ExecID            [30];    /* ER ��ȣ                              */
    char  OrderQty          [12];    /* �ֹ�����                             */
    char  OrdType            [1];    /* �Ÿű��� (1 : Market , 2 : Limit)    */
    char  Price             [12];    /* �ֹ��ܰ�                             */
    char  Side               [1];    /* �ŵ��ż����� (1:BUY , 2:SELL)        */
    char  Symbol            [12];    /* �����ڵ�  (ISIN CODE)                */
    char  hname             [40];    /* �ѱ۸�                               */
    char  TargetCompID      [10];    /* COMP ID (SELL SIDE ID)               */
	char  DeliverToCompID   [10];    /* COMP ID (SELL SIDE ID)               */
    char  Exdestination      [3];    /* �ŷ��� ����1 (100)                   */
    char  SecurityEx         [5];    /* �ŷ��� ����2 (207)                   */
    char  OrderStatus        [1];    /* �ֹ�����                             */
    char  RejectReason       [2];    /* �źλ����ڵ� (ER, OCR)               */
    char  CumQty            [12];    /* ����ü�����                         */
    char  LastShares        [12];    /* ü�����                             */
    char  LeavesQty         [12];    /* �ܷ�                                 */
    char  LastPx            [12];    /* ü��ܰ�                             */
    char  TransactTime      [20];    /* �ֹ����Žð�                         */
    char  Text              [80];    /* �źλ���                             */
}OVERSEA_CHE_REAL_OUTREC;

/* FX ü��,��ü�� ���� ����ü */
typedef struct  
{
	char	szKeyType[4];				/*  [FX�ֹ�ü��] ��ü�� - OR01, ü�� CH01		*/
	char	szAccNo[11];				/*	[���¹�ȣ]	 �ֹ��� ȸ����+���¹�ȣ			*/
	char	szMemberNo[3];				/*  [ȸ����ID]	 ����Ʈ 000 (����)				*/
	char	szOrdType[5];				/*  [�ֹ�����]	 �ι�° �ڸ��� ����		  
																			 //OM - ���尡,
																			 //OE - ������,
																			 //OS - ��������,
																			 //RE - ������ ����,
																			 //RS - �������� ����				
											(1:���尡   2:������ 3:�������� 4:STOP-LIMIT 5:OCO V:�����ս�)*/
	char	SHCODE[12];					/*  [�����]	 ���� : CCY1/CCY2				*/
	char	szOrderID[20];				/*  [�ŷ��� ������ȣ]							*/
	char	szRate[10];					/*  [��������]	 ��ü��� �ֹ���, ü��� ü�ᰡ	*/
	char	szQuote[10];				/*  [���簡]	 ���� ü��� �������� �ݴ�Ÿ� ���簡 */
	char	szStop[10];					/*  [Stop Price]								*/
	char	szLimit[10];				/*	[Limit Price								*/	
	char	szTStop[10];				/*	[Trailing Stop Price						*/	
	char	szOldStop[10];				/*  [Old Stop]									*/
	char	szOldLimit[10];				/*  [Old Limit]									*/
	char	szOpenTime[30];				/*  [�ֹ�, ���� �ð�]							*/
	char	szSLOpenTime[30];			/*  [SL �ֹ�, ���� �ð�]						*/
	char	szExeTime[30];				/*  [�����ð�]	 ü��ð�						*/
	char	szOrgCustItem1[11];			/*  [��ȸ��ó���׸�1]							*/
	char	szOrgCustItem2[14];			/*  [��ȸ��ó���׸�2] ���ֹ���ȣ				*/
	char	szOrgCustItem3[55];			/*  [��ȸ��ó���׸�3]							*/
	char	szCustItem1[11];			/*  [ȸ��ó���׸� 1]							*/
	char	szCustItem2[14];			/*  [ȸ��ó���׸� 2]  �ֹ���ȣ					*/
	char	szCustItem3[55];			/*  [ȸ��ó���׸� 3]							*/
	char	szStopItem[80];				/*  [Stop ȸ��ó���׸�]	Stop �ֹ���ȣ (��ǥ��)	*/
	char	szLimitItem[80];			/*  [Limit ȸ��ó���׸�] Limit �ֹ���ȣ	(��ǥ��)*/
	char	szOldStopItem[80];			/*	[������ Stop ȸ��ó���׸�]					*/
	char	szOldLimitItem[80];			/*  [������ Limit ȸ��ó���׸�]					*/
	char	szTSCustItem[80];			/*  [TrailingStop ȸ��ó���׸�]					*/
	char	szTradeType[3];				/*  [�ŷ�����]	11:����, 12:û��, 13:������		*/
	char	szSide[3];					/*  [�Ÿű���] 
											079 - �ż�, 081 - �ŵ�, 
											080 - ȯ��, 082 - ����						*/
	char	szAccName[20];				/*  [���¸�]									*/
	char	szUserID[20];				/*  [��ID]									*/
	char	szFXCMPosID[20];			/*  [TicketNo]	ü���ȣ						*/
	char	szPositionID[20];			/*  �����ǹ�ȣ									*/

	char	szStatus[2];				/*  [��ü�� ���� ���� 0:������, 1:����]
										=> ������ ���� ����.  �ַθ�
											[�ֹ���������]
											01:�ֹ�����,02:ü����,03:ü��Ϸ�,
											04:��������,05:�����Ϸ�(���ֹ�),06:�������,
											07:��ҿϷ�(���ֹ�),08:�ź�,15:�����Ϸ�(�����ֹ�),17:��ҿϷ�(����ֹ�) */
	char	szSign[1];					/*  [�����ȣ]	2 - ���, 3 - ����, 4 - �϶�	*/
	long	nCommition;					/*  [������]									*/
	long	nTrailingStop;				/*  [Trailing Stop Pip]							*/
	long	nPipLowest;					/*	[�Ҽ��� ��ȿ�ڸ���]							*/
	double	fAmount;					/*  [ü�����]									*/
	double	fOrderAmt;					/*  [��ü�����]								*/
	double	fTotalPL;					/*  [�̼���]									*/
	double	fGrossPL;					/*  [���ϼ���(����ȯ��)]						*/
	double	fRevRate;					/*  [ü��� �ݴ�����]							*/
	double	fOpenMarkup;				/*  [OM (��ȯ�Ž�)]								*/
	double	fInterest;					/*  [����]										*/
	double	fAddInterest;				/*  [������ ����]								*/
	double	fCloseMarkup;				/*  [CM (��ȯ�Ž� ���)]						*/
	double	fOutBalance;				/*  [�̰����ݾ�(û�����)]						*/
	double	fPipCost;					/*  [ü���PipCost]								*/
	double	fPredictMRate;				/*  ���� ������ ȯ��							*/
	double	fTSHighPrice;				/*  Trailing Stop �ֹ� ���簡 ��� ��			*/
} FX_CHE_STRUCT;

typedef struct 
{
    double  fBalance;				    /* ��Ź�� �ܾ� */
    double  fOutstanding;				/* û����� */
    double  fGrossPL;					/* �򰡼��� */
    double  fEquity;					/* ���ڻ� */
    double  fMarginReq;					/* �ֹ� ���ű� �հ� */
    double  fMtMargin;					/* ���� ���ű� �հ� */
	double  fOrderMargin;				/* �ֹ� ���ű� */
    double  fUsableEquity;				/* �ֹ����ɱݾ� */
    double  fUEquityRate;			    /* �����ݺ���(%) */
    double  fDepoTotalMSubs;			/* ��ȭ���ݾ� */
    double  fNowSellRate;				/* ��ȭ�������ȯ��(���ظŵ�ȯ��) */
    double  fBalanceWon;				/* ��ȭ�ѿ�Ź�� */
	double	fTotalWon;					/* ��ȭȯ���ڻ� */
    int     nOutCount;					/* �ֹ����ɼ��� */
	int		nOpenSum;					/* ��û�� ���� �հ� */
	char	szMemberNo[3];				/* ȸ���� */
    char    szAccNo[11];				/* ���¹�ȣ*/
    char    szUserName[40];				/* ���¸� */
    char    cIsHedge;					/* �������� */
} FX_JAN_STRUCT;


typedef struct  
{
	double	fBalance;					/* �޷� ��Ź�� �հ�		*/
	double	fOutstanding;				/* û����� �հ�		*/
	double	fCurPL;						/* �򰣼��� �հ�		*/
	double	fReplaceUSD;				/* ��ȭ��� USD �հ�	*/
	double	fEquity;					/* ���򰡿�Ź�� �հ�	*/
	double	fTotalWon;					/* ��ȭ ȯ���ڻ� �հ�	*/
	double	fOrderMargin;				/* �ֹ����ű� �հ�		*/
	double	fMtMargin;					/* �������ű� �հ�		*/
	double	fUsableEquity;				/* �ֹ����ɱݾ� �հ�	*/
	int		nUsableOrderCnt;			/* �ֹ����ɼ��� �հ�	*/
} TOTAL_JAN_ROW;						/* �ܰ���Ȳ �հ�		*/

#pragma pack(pop)
#endif