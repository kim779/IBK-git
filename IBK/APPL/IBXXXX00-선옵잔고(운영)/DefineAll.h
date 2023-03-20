//Map Size 
#define	MAP_WIDTH	300
#define	MAP_HEIGHT	200

#define WM_REMAIN		WM_USER + 2
#define WM_ALLACCNREMAIN	WM_USER + 3
#define WM_REMAIN_CLOSE		WM_USER + 4
#define WM_LAWCHEG		WM_USER + 5
// ü�� symbol
#define RT_ORDNO	"904"	// �ֹ���ȣ
#define RT_JCODE	"907"	// �����ڵ�
#define RT_JNAME	"908"	// �����
#define RT_GUBN		"912"	// ����
#define RT_JPRICE	"910"	// �򰡱ݾ�
#define RT_CPRICE	"916"	// ü�ᰡ
#define RT_CAMT		"931"	// ü�ᷮ
#define RT_STATE	"988"	// �ֹ�����
#define RT_CREDIT	"911"	// �ſ뱸��
#define RT_CTDATE	"994"	// �ſ������(8)
#define RT_CTSYMT	"933"	// �ſ��/�����ȯ��
#define RT_JFUNIT	"995"	// �ֽļ����ŷ�����(10)
#define RT_MOD		"999"	// Mod Value
#define RT_JGTYPE	"993"	// �ܰ��ڵ�

#define RT_ANAME	"906"	// ���¸�
#define RT_MCAMT	"921"	// ��ü�ᷮ
#define RT_ORDGB	"922"	// �ֹ�����
#define RT_ORDQTY	"909"	// �ֹ�����
#define RT_JOTYPE	"996"	// ����,ü��
#define RT_JTYPE	"926"	// �������

#define	RT_CCURR	"023"	// ü��:���簡
#define	RT_CDIFF	"024"	// ü��:���
#define RT_CRATE	"033"	// ü��:�����
#define	RT_CYYDT	"370"	// ü��:�����ϼ�
#define	RT_CDDDT	"314"	// ü��:�޷��ϼ�

// ü�� ����	  
enum { 
	CHEG_NOT = 0,			// 0 not
	CHEG_UPDATE,			// 1 update
	CHEG_DELETE,			// 2 delete
	CHEG_INSERT			// 3 insert(oop send)
};

#define	LEN_HCODE	7		// �ֽ� �����ڵ� ����
#define	LEN_FCODE	8		// ���� �����ڵ� ����

#define	LEN_ACODE	12		// �ֽ� ��ü�ڵ� ����
#define	LEN_SYGB	2		// �ſ뱸�� ����
#define	LEN_DATE	8		// ��¥
#define	LEN_SYMT	15		// �ſ����ݾ� ����
#define LEN_JGGB	2

#define sv_SJGO     "SONAQ052"
#define sv_FJGO     "SONBQ021"
#define sv_LAW      "SONAQ056"

#define TAX_RATE 0.003
#define TAX_RATE_K_OTC 0.005	//2015.04.22 KSJ K-OTC ��������
/*******************************************************************************
 * �ֽ� �ܰ��� ��ȸ IO FORMAT
 ******************************************************************************/
/*
struct	s_mid	{		// �ֽ� Input
	char	accn[8];
	char	cogb[2];
	char	pswd[8];
	char	cod2[12];
	char    sygb[1];	// 0.�ſ����� 1.All
	char    fees[1];	// ���Դܰ� ������ 0.���� 1.������ 2006.10.30 ���Դܰ� ������ ������ �߰�
};

#define	CREDIT	// �ſ���

struct	s_rec	{
	char	cod2[12];	// �����ڵ�
	char	hnam[40];	// �����
#ifdef	CREDIT
	char	sygb[2];	// �ſ뱸��
				// 01:��������, 02:�ܱ�����, 03:�Ϲݽſ�, 05:�������, 06:�����ڱ�, 07:�ڱ����, 08:��Ź�㺸,
				// 10:KP, 20:KQ, 30:OTC, 40:ä��, 50:���
#endif
	char	xqty[15];	// �����ܰ�
	char	bqty[15];	// �ֹ��ܷ�
	char	pprc[15];	// ������հ�
	char	cqty[15];	// �ŵ������ܰ�
	char	curr[15];	// ���簡
	char	pamt[15];	// �򰡱ݾ�
	char	psam[15];	// �򰡼���
	char	srat[15];	// �򰡼��ͷ�
	char	mamt[15];	// ���Աݾ�
#ifdef	CREDIT
	char	symt[15];	// �ſ����ݾ�
	char	sydt[8];	// ������
#endif
};


struct	s_mod	{
	char	accn[10];
#ifdef	CREDIT

#else
	char	accm[40];	// ���¸�			
#endif
	char	nrec[4];
	struct	s_rec	rec[1];
};
*/

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
#ifdef SISE_EXTEND
	char    diff[10];	/* ���ϴ��         */
	char    rate[10];   /* �����           */
#endif
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

/*******************************************************************************
 * ���� ü�᳻����ȸ IO FORMAT
 ******************************************************************************/
struct  law_mid   {
	char	rec_in[5];
    char    acc_gb[1];
    char    mng_no[3];
    char    mk_cod[2];
    char    con_gb[2];    
    char    grp_no[20];
};

struct  law_rec   {
    char    mng_no[3];     /* ����������ȣ     */
    char    acc_no[20];    /* ���¹�ȣ         */
    char    acc_nm[40];    /* ���¸�           */
    char    cod_no[12];    /* �����ȣ         */
    char    cod_nm[40];    /* �����           */
    char    trd_gb[1];     /* �Ÿű���         */
    char    ord_cd[2];     /* �ֹ������ڵ�     */
    char    ord_nm[40];    /* �ֹ�������       */
    char    ord_qty[16];   /* �ֹ�����         */
    char    chg_qty[16];   /* ü�����         */
    char    chg_avg[13];   /* ü����հ�       */
    char    chg_amt[16];   /* ü��ݾ�         */
}; 
struct  law_mod   {
    char    rec_out[5];
	char	serial_no[10];
    char    nrec[5];
    struct  law_rec   rec[1];
}; 

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

#define L_smid	sizeof(struct s_mid)				// Length of stock input
#define L_smod	sizeof(struct s_mod) - sizeof(struct s_rec)	// Length of stock output
#define L_srec	sizeof(struct s_rec)				// Length of stock Grid

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid

#define L_lawmid	sizeof(struct law_mid)				// Length of stock input
#define L_lawmod	sizeof(struct law_mod) - sizeof(struct law_rec)	// Length of stock output
#define L_lawrec	sizeof(struct law_rec)				// Length of stock Grid

enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR, JK_SFUTURE, JK_SFSPREAD, JK_KTBFUTURE, JK_USDFUTURE, JK_GOLDFUTURE, JK_PIGFUTURE, JK_MINIGOLDFUTURE, JK_MINI_FUTURE, JK_MINI_SPREAD, JK_MINI_CALLOPT, JK_MINI_PUTOPT};

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




// �ֹ����� ���ε�
struct _pidouini_item {
	char	usid[8  ];				// ����� ID
	char	innm[100];				// INI ���ϸ�
	char	senm[100];				// ���Ǹ�
	char	skey[100];				// KEY
	char	valu[2000];				// VALUE
	char	date[8  ];				// ������Ʈ ����
};

struct _pidouini_mid {
	char	gubn[1 ];				//���� 'Q': ��ȸ, 'I': �Է�
	struct	_pidouini_item item; 
};

struct  _pidouini_aid{
	char    gubn        [ 1];           /*  ����('A')       */
	char    nrec        [ 2];           /*  COUNT           */
	struct  _pidouini_mid     mid [16];  /*  MID             */
};

#define	sz_pidouini	sizeof(struct _pidouini_mid)

struct	usrinfo {
	char    gubn[2];                /* ���� ����                    */
	/* MY: ���� ����                */
	/* UL: ����� ����Ÿ ���       */
	/* UD: ����� ����Ÿ            */
	/* UX: Delet Chart		*/
	char    dirt[1];                /* ���� ����                    */
	/* U: PC=>HOST  D: HOST=>PC     */
	char    cont[1];                /* ���� ����                    */
	/* F: First     M: Middle       */
	/* L: Last      C: Cancel       */
	/* D: delete file               */	
	char    name[80];               /* file name (�ʿ�ø�)         */
	char    nblc[5];                /* �� ���� or                 */
	/* ȭ�� SIZE (max. 64K)         */
	char    retc[1];                /* O: ����      E: ����         */
	char    emsg[40];               /* error message                */
};

struct  d_list {                        /* ����� ����Ÿ LIST           */
	char    name[80];               /* file name                    */
	char    size[5];                /* file size                    */
};

#define L_usr	sizeof(struct usrinfo)
#define L_lst	sizeof(struct d_list)

#define SAVEFILENAME	"SaveRemainLog.dat"















































































