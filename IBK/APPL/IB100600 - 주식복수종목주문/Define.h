// Define.h : header file
//
#pragma once
// Control Resource ID
#define IDC_TAB				1001
#define IDC_GUIDE			1002
#define IDC_ORDER			1003
#define IDC_MODCAN			1004
#define IDC_EXPAND			1005
#define IDC_ACCOUNT			1006
#define IDC_ACCOUNTNAME		1007
#define IDC_PASSSTATIC		1008
#define IDC_PASSWORD		1009
#define IDC_ORDERCONFIRM	1010
#define IDC_CLEAR			1011
#define IDC_REMAIN			1012
#define IDC_INTEREST		1013
#define IDC_ORDERSEND		1014
#define IDC_CONTRECT		1015
#define IDC_EXPANDBTN		1016
#define IDC_SELL			1017
#define IDC_BUY				1018
#define IDC_ORDERTYPESTATIC	1019
#define IDC_ORDERTYPE		1020
#define IDC_CONDITIONSTATIC	1021
#define IDC_CONDITION		1022
#define IDC_QUANTITYSTATIC	1023
#define IDC_QUANTITYSHARE	1024
#define IDC_QUANTITYPRICE	1025
#define IDC_QUANTITYVALUE	1026
#define IDC_PRICEUNITSTATIC	1027
#define IDC_PRICEUNIT		1028
#define IDC_PERCENT			1029
#define IDC_PERCENTSTATIC	1030
#define IDC_ORDERGRID		1031
#define IDC_MASKEDEDIT		1032
#define IDC_NONCONTRACT		1033
#define IDC_MODIFY			1034
#define IDC_CANCEL			1035
#define IDC_ALL				1036
#define IDC_REMAINQUANTITY	1037
#define IDC_PARTQUANTITY	1038
#define IDC_CODECTRLSTATIC	1039
#define IDC_CODECTRL		1040
#define IDC_CODESEARCH		1041
#define IDC_QUANTITYREMAIN	1042
#define IDC_QUANTITYPART	1043
#define IDC_MODCANGRID		1044
#define IDC_CONFIRMGRID		1045
#define IDC_MSG				1046
#define IDC_ST_WARNING		1047
#define IDC_WARNWND			1048
// Size Define
#define SZ_MAP_WIDTH		1090
//#define	SZ_MAP_HEIGHT		502 //test
#define	SZ_MAP_HEIGHT		558
#define	SZ_FORM_MARGIN		3
#define SZ_TAB_HMARGIN		2
#define SZ_CTRL_VGAP		2
#define SZ_CTRL_HGAP		5
#define SZ_TAB_HEIGHT		28
#define SZ_CTRL_HEIGHT		20
#define	SZ_CTRL_ACCN		140
#define SZ_CTRL_ACCNM		64
#define SZ_CTRL_1STATIC		14
#define SZ_CTRL_2STATIC		26
#define SZ_CTRL_1BUTTON		20
#define SZ_CTRL_3BUTTON		44
#define SZ_CTRL_4BUTTON		60
#define SZ_CTRL_4STATIC		50
#define SZ_CTRL_3EDIT		44
#define SZ_CTRL_4EDIT		56
#define SZ_CTRL_6EDIT		80
//#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_6STATIC		74
#define SZ_CHECK_5WIDTH		80
#define SZ_EXPAND_WIDTH		320
#define SZ_RADIO_1WIDTH		32
#define SZ_RADIO_2WIDTH		44
#define SZ_COMBO_3WIDTH		60
#define SZ_COMBO_5WIDTH		84
#define SZ_COMBO_6WIDTH		96
#define SZ_COMBO_7WIDTH		108
#define SZ_ROWHEIGHT		18
#define SZ_SYMBOL			140


// Color Index Define
#define CLR_FORM_BACK		64
#define CLR_CONTENTS_BACK		181

#define CLR_PANNEL_BACK		66
#define CLR_PANNEL_LINE		67
#define	CLR_STATIC_BACK		34
#define	CLR_STATIC_FORE		130
#define	CLR_STATIC_LINE		65
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71
#define CLR_GRID_TEXT		69
#define CLR_GRID_BACK		68
#define CLR_GRID_BACK2		77
#define CLR_GRID_LINE		75
#define CLR_GRID_HBACK		74
#define CLR_GRID_HTEXT		76
#define CLR_GRID_SBACK		78
#define CLR_GRID_STEXT		69
#define CLR_DATA_UP			94
#define CLR_DATA_DOWN		95
#define CLR_SYMBOL_BACK		160
#define CLR_SYMBOL_FORE		69
#define CLR_SYMBOL_LINE		89
#define CLR_GUIDE_BACK		66
#define CLR_GUIDE_FORE		130
#define CLR_GUIDE_LINE		75

// etc
#define MAX_ORDERGRIDCOL	15
#define DEF_ORDERGRIDROW	20
#define MAX_MODCANGRIDCOL	12
#define DEF_MODCANGRIDROW	20
#define MAX_CONFIRMGRIDCOL	7
#define DEF_CONFIRMROW		7
#define MAPNO_FHOGA			"IB100610"
#define WARN_WND			"IB100620"
#define TT_GUIDE			100
#define TT_INIT				105
#define TT_CHKPWD			106


// TR Key
#define KEY_ACCOUNT			255
#define KEY_ACCNAME			254
#define KEY_SREMAIN			100
#define KEY_SORDER			101
#define KEY_SHOGA			102
#define KEY_SCURR			103
#define KEY_SYMBOL			104
#define KEY_NONCONT			105
#define KEY_MODCANSHOGA		106
#define KEY_MODCANSCURR		107
#define KEY_MODCANSORDER	108
#define KEY_CHKPASSWORD		0x07

#define KEY_SHOGA_STDPRC			202
#define KEY_SCURR_STDPRC			203
#define KEY_MODCANSHOGA_STDPRC		206
#define KEY_MODCANSCURR_STDPRC		207


/*******************************************************************************
 * �ֽ� �ܰ��� ��ȸ IO FORMAT
 ******************************************************************************/
struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.�ſ����� 1.All */
    char    fill[10];
};

struct  s_rec   {
    char    cod2[12];   /* �����ڵ�         */
    char    hnam[40];   /* �����           */
    char    jggb[2];    /* �ܰ��ڵ�         */
    char    jqty[10];   /* �ܰ����         */
    char    xqty[10];   /* �ŵ�����         */
    char    pamt[10];   /* ������հ�       */
    char    mamt[15];   /* ���Աݾ�         */

    char    curr[10];   /* ���簡     (*)   */
	char    diff[10];   /* ���ϴ��         */	//2014.01.03 KSJ 7 -> 10
    char    rate[10];   /* �����           */	//2014.01.03 KSJ 5 -> 10
    char    camt[15];   /* �򰡱ݾ�         */
    char    tamt[15];   /* �򰡼���   (*)   */
    char    srat[10];   /* �򰡼��ͷ� (*)   */
    char    sycd[2];    /* �ſ��ڵ�         */
    char    sydt[8];    /* ������           */
    char    samt[15];   /* �ſ�ݾ�         */
}; 
struct  s_mod   {
    char    accn[11];
    char    nrec[4];
    struct  s_rec   rec[1];
};

#define L_smid	sizeof(struct s_mid)				// Length of stock input
#define L_smod	sizeof(struct s_mod) - sizeof(struct s_rec)	// Length of stock output
#define L_srec	sizeof(struct s_rec)				// Length of stock Grid


/*******************************************************************************
 * �ֹ� ����
 ******************************************************************************/
#define MAX_REC     150

struct irec
{
    char    odgb[1];    /* �ֹ����� : 1:���� 2:�����ɼ� 3:�����ֹ�          */
    char    mkgb[1];    /* filler           */
    char    mmgb[1];    /* �Ÿű��� : 1:�ŵ� 2:�ż� 3:���� 4: ���          */
    char    accn[11];   /* ���¹�ȣ         */
    char    pswd[8];    /* ��й�ȣ         */
    char    ojno[12];   /* ���ֹ���ȣ       */
    char    cod2[12];   /* �����ڵ�         */
    char    jqty[8];    /* �ֹ�����         */
    char    jprc[10];   /* �ֹ��ܰ�         */
    char    hogb[2];    /* ȣ������         */
    char    cdgb[1];    /* �ֹ����� : 0:���� 1:IOC 2:FOK                    */
    char    mdgb[1];    /* ������� : 1:�Ϻ� 2:����                         */
    char    prgb[1];    /* ó������ : 'A,B,C,D' : ���� ���� 'X' : ������  */
    char    usrf[40];
};

struct i_com
{
    char    nrec[4];    /* �ֹ� ����        */
    struct  irec    rec[MAX_REC];
};

struct orec
{
    char    jmno[6];    /* �ֹ� ��ȣ        */
    char    ojno[6];    /* ���ֹ���ȣ       */
    char    emsg[80];   /* ���� �޼���      */
};

struct o_com   
{
    char    nrec[4];
    struct  orec    rec[MAX_REC];
};

#define L_i_com		sizeof(struct i_com)
#define L_o_com		sizeof(struct o_com)
#define L_irec		sizeof(struct irec)
#define L_orec		sizeof(struct orec)

/*******************************************************************************
 * �ֽ� �ܰ� ��ȸ
 ******************************************************************************/
struct  i_code {
    char    cod2[6];            /* �����ڵ�             */
};

struct  shmid {
    char    gubn[1];            /* ȣ������         */
                                /* '0' : ���Ѱ�     */
                                /* '1' : �ŵ�ȣ��   */
                                /* '2' : ���簡     */
                                /* '3' : �ż�ȣ��   */
                                /* '4' : ���Ѱ�     */
                                /* '5' : ��������   */
								/* '6' : ����ü�ᰡ */
								/* '7' : ���ذ�		*/
    char    nrec[4];            /* ���ڵ� ����      */
    struct  i_code  rec[999];
};

struct  o_price {
    char    cod2[6];            /* �����ڵ�         */
    char    price[9];           /* ȣ������ ������  */
	char	gdan[2];			/* �ֹ��ܰ� ����	*/
};

struct  shmod {
	char 	gubn[1];
    char    nrec[4];
    struct  o_price rec[999];
}; 

#define L_i_code	sizeof(struct i_code)
#define L_o_price	sizeof(struct o_price)
#define L_shmid		sizeof(struct shmid)
#define L_shmod		sizeof(struct shmod)


/*******************************************************************************
 * �ֽ� ��ü�� ��ȸ
 ******************************************************************************/
struct	s_cmid	
{
	char    gubn[1];	//2015.05.28 KSJ ������ '>' �־���.
	char    accn[11];
	char    pswd[8];
	char    date[8];	//  => "            "
	char    cod2[12];	//  => "A        "
	char    juno[5];	// �����ֺй�ȣ
	char    dsgb[1];	// 0.All 1.�ŵ� 2.�ż�      
	char    otim[1];	// 0.All 1.���� 2.�ð���    
	char	tcod[2];	// 00.All else. �ش��ü�ڵ�
	char    dlgb[1];	// 0.All 1.ü�� 2.��ü��    
	char    sort[1];	// 1.���� 2.����            
	char    juno2[10];	// �����ֺй�ȣ2               //2015.05.28 KSJ ��ȣ 5 -> 10 
};


struct	s_crec	{
	char    juno[10];    // �ֹ�   ��ȣ      //2015.05.28 KSJ ��ȣ 5 -> 10 
	char    ojno[10];    // ���ֹ� ��ȣ      //2015.05.28 KSJ ��ȣ 5 -> 10 
	char    cod2[12];   // �����ڵ�         
	char    hnam[40];   // �����           
	char    odgb[20];   // �ֹ�����         
	char    jcgb[20];   // ������ұ��� 
	char    hogb[20];   // ȣ������         
	char    jprc[12];   // �ֹ�����         
	char    jqty[12];   // �ֹ�����         
	char    dprc[12];   // ü�ᰡ��         
	char    dqty[12];   // ü�����         
	char    tqty[12];   // ü�������       
	char    wqty[12];   // ��ü�����       
	char    stat[20];   // ��������         
	char    time[8];    // �ֹ��ð�         
};

struct	s_cmod	
{
	char	accn[11];
	char	nrec[4];
	struct	s_crec	rec[250];
};

#define L_s_cmid		sizeof(struct s_cmid)
#define L_s_crec		sizeof(struct s_crec)
#define L_s_cmod		sizeof(struct s_cmod)

