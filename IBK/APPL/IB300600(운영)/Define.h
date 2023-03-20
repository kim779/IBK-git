#pragma once
// Define.h : header file
//

// Control Resource ID
#define IDC_TAB					1001
#define IDC_GUIDE				1002
#define IDC_ORDER				1003
#define IDC_CLEAR				1004
#define IDC_MODCAN				1005
#define IDC_EXPAND				1006
#define	IDC_ACCOUNT				1007
#define	IDC_ACCOUNTNAME			1008
#define IDC_PASSSTATIC			1009
#define IDC_PASSWORD			1010
#define IDC_CLEAR2				1011
#define IDC_REMAIN				1012
#define IDC_ORDERSEND			1013
#define IDC_EXPANDBTN			1014
#define IDC_SELL				1015
#define	IDC_BUY					1016
#define IDC_ORDERTYPESTATIC		1017
#define IDC_ORDERTYPE			1018
#define IDC_ORDERPRICESTATIC	1019
#define IDC_ORDERPRICETYPE		1020
#define IDC_ORDERPRICE			1021
#define IDC_ORDERSPIN			1022
#define IDC_QUANTITYSTATIC		1023
#define IDC_QUANTITY			1024
#define IDC_QUANTITYSPIN		1025
#define IDC_ORDERGRID			1026
#define IDC_CODEEDIT			1027
#define IDC_CODECTRL			1028
#define IDC_CODEBUTTON			1029
#define IDC_MODCANPRICESTATIC	1030
#define IDC_MODCANPRICETYPE		1031
#define	IDC_NONCONT				1032
#define	IDC_MODIFY				1033
#define	IDC_CANCELMODE			1034
#define IDC_MODCANGRID			1035
#define IDC_CONFIRMGRID			1036
#define IDC_MSG					1037
#define IDC_CONTRACT			1038


// Size Define
#define SZ_MAP_WIDTH		960
#define	SZ_MAP_HEIGHT		378
#define	SZ_FORM_MARGIN		3
#define SZ_TAB_HMARGIN		2
#define SZ_CTRL_VGAP		2
#define SZ_CTRL_HGAP		5
#define SZ_CTRL_HEIGHT		20
#define	SZ_CTRL_ACCN		140
#define SZ_CTRL_ACCNM		64
#define SZ_CTRL_2STATIC		26
#define SZ_CTRL_3BUTTON		44
#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_4STATIC		50
#define SZ_CTRL_3EDIT		44
#define SZ_CTRL_4EDIT		56
#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_6STATIC		74
#define SZ_CTRL_1BUTTON		20
#define SZ_EXPAND_WIDTH		320
#define SZ_RADIO_2WIDTH		44
#define SZ_COMBO_3WIDTH		60
#define SZ_COMBO_5WIDTH		84
#define SZ_ROWHEIGHT		18

// Color Index Define
#define CLR_FORM_BACK		64
#define CLR_PANNEL_BACK		66
#define CLR_PANNEL_LINE		67
#define	CLR_STATIC_BACK		78
#define	CLR_STATIC_FORE		69
#define	CLR_STATIC_LINE		65
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71
#define CLR_GRID_TEXT		69
#define CLR_GRID_BACK		68
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
#define CLR_GUIDE_FORE		69
#define CLR_GUIDE_LINE		75

// etc
#define MAX_ORDERGRIDCOL	11
#define DEF_ORDERGRIDROW	13
#define MAX_MODCANGRIDCOL	11
#define DEF_MODCANGRIDROW	13
#define MAX_CONFIRMGRIDCOL	7
#define DEF_CONFIRMROW		7

//cell 'param' values, used as a style field
#define DROPLIST_ORDERSTATE		100
#define DROPLIST_ORDERTYPE		101
#define	DROPLIST_MODCANSTATE	100
#define DROPLIST_MODCANTYPE		101
#define CELLTYPE_IS_EDITABLE	120
#define SPIN_QUANTITY			200
#define SPIN_ORDERPRICE			201

#define MAPNO_FHOGA			"IB300610"
#define MAPNO_CLEAR			"IB320700"

#define SYMBOL_HOGA			"3006_fpric"

#define TT_GUIDE			100

// TR Key
#define KEY_ACCOUNT			255
#define KEY_ACCNAME			254
#define KEY_FREMAIN			100
#define KEY_FOHOGA			101
#define KEY_FOCURR			102
#define KEY_FORDER			103
#define	KEY_NONCONT			104
#define KEY_MODCANFOHOGA	105
#define KEY_MODCANFOCURR	106
#define KEY_MODCANFORDER	107

/*******************************************************************************
 * ����/�ɼ� �ܰ��� ��ȸ IO FORMAT
 ******************************************************************************/
struct	f_mid	{		// ���� Input
	char	accn[11];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* �����ڵ�		*/
	char	hnam[30];	/* �����		*/
	char	dsgb[6];	/* ����			*/
	
	char	bqty[10];	/* ����			*/
	char	cqty[10];	/* �ŵ����ɼ���		*/
	char	pprc[10];	/* ��հ�/���갡	*/
	char	curr[10];	/* ���簡		*/
	char	diff[10];	/* ��հ� ���		*/
	char	pamt[15];	/* �򰡱ݾ�		*/
	char	psam[15];	/* �򰡼���		*/
	char	prat[10];	/* ���ͷ�		*/
	char	tamt[15];	/* ���Աݾ�		*/
};

struct	f_mod	{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[1];
};

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid

enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR, JK_SFUTURE, JK_SFSPREAD};

/*******************************************************************************
 * ����/�ɼ� ���簡 or ���/�켱 ȣ�� ��ȸ
 ******************************************************************************/
struct  i_hoga {
    char    dsgb[1];            /* '1' �ŵ�  '2' �ż�   */
    char    cod2[8];            /* �����ڵ�             */
};

struct  fhmid {
    char    gubn[2];            /* ȣ������             */
                                /* '00' : ���簡        */
                                /* '10' : �켱1ȣ��     */
                                /* '11' : �켱2ȣ��     */
                                /* '12' : �켱3ȣ��     */
                                /* '20' : ���1ȣ��     */
                                /* '21' : ���2ȣ��     */
                                /* '22' : ���3ȣ��     */
    char    nrec[4];            /* ���ڵ� ����          */
    struct  i_hoga  rec[999];
};

struct  o_hoga {
    char    cod2[8];            /* �����ڵ�             */
    char    curr[9];            /* ���簡               */
    char    hoga[9];            /* ȣ��                 */
};

struct  fhmod {
    char    nrec[4];
    struct  o_hoga rec[999];
}; 


#define L_fhmid	sizeof(struct fhmid)
#define L_i_hoga	sizeof(struct i_hoga)
#define L_fhmod	sizeof(struct fhmod)
#define L_o_hoga	sizeof(struct o_hoga)

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
 * ����/�ɼ� ��ü�� ��ȸ IO FORMAT
 ******************************************************************************/
struct f_cmid {
	char	accn[11];
	char	pswd[8];
	char	gubn[1];	/* 0.All 1.ü�� 2.��ü��    */
	char	juno[6];
	char	sort[1];	/* 0.X   1.���� 2.����      */
	char	date[8];	
	char	cod2[8];	/* �����ڵ�                 */
    char    mkgb[1];    /* 0.All F.���� C.�ݿɼ� P.ǲ�ɼ� S.�������� X.Ư������ */
    char    dsgb[1];    /* 0.All 1.�ŵ� 2.�ż�      */
};

struct f_crec {
	char	mono[6];	/* ���ֹ���ȣ		*/
	char	juno[6];	/* �ֹ�  ��ȣ		*/
	char	ojno[6];	/* ���ֹ� ��ȣ		*/
	char	cod2[8];	/* �����ڵ�			*/
	char	hnam[30];	/* �����			*/
	char	odgb[8];	/* �ֹ�����			*/
	char	hogb[20];	/* �ֹ�����			*/
	char	jprc[11];	/* �ֹ�����			*/
	char	jqty[7];	/* �ֹ�����			*/
	char	dlgb[4];	/* ü�ᱸ��			*/
	char	dprc[11];	/* ü�ᰡ��			*/
	char	dqty[7];	/* ü�����			*/
	char    dtim[6];    /* ü��ð�         */
	char	wqty[7];	/* ��ü�����		*/
    char    hqty[7];    /* ����/��Ҽ���    */
	char	stat[8];	/* ��������			*/
	char	time[6];	/* �����ð�			*/
    char    jseq[6];    /* ������ȣ         */
    char    yseq[7];    /* ������ȣ         */
    char    ecod[4];    /* �ź��ڵ�         */
};

struct	f_cmod {
	char	accn[11];
	char	nrec[4];
	struct	f_crec rec[1];
};

#define L_fcmid	sizeof(struct f_cmid)				// Length of Future input
#define L_fcmod	sizeof(struct f_cmod) - sizeof(struct f_crec)	// Length of Future output
#define L_fcrec	sizeof(struct f_crec)				// Length of Future Grid