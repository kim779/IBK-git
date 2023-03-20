#ifndef __CONFIGBAR_TR_H__
#define __CONFIGBAR_TR_H__

#define INTER_UP	1
#define INTER_DOWN	2
#define INTER_RANK	3
#define INTER_GUBUN	"MY"
#define GUBUN_LEN	2
#define INTER_ACC       4
#define INTER_RECENT	5


////// �������
#define RANK_TRCODE	"chbojrnk"

struct  rank_mid     {
	char    gubn[2];                /* ��ȸ����                     */
	char    mark[1];                /* ���屸��                     */
	/* 1 : KOSPI,   2 : KOSDAQ      */
	/* 3 : ��3����                  */
};

struct  rank_grid    {
	char    code[6];                /* �����ڵ�                     */
	char    hnam[16];               /* �����                       */
};

struct  rank_mod     {
	char    nrec[4];                /* # of records                 */
	struct  rank_grid    grid[1];
};

#define ACC_TRCODE	"chbohold"

struct  acc_mid {			/* Message Input Data           */
	char    acno[11];		/* ���¹�ȣ                     */
	char    pswd[8];		/* ��й�ȣ                     */
	char    gubn[1];		/* 0: ����     1: ����          */
};


struct  acc_grid {
	char    code[8];        /* A����nAU��a                     */
	char    hnam[20];       /* A����n��i                       */
};

struct  acc_mod {                   /* Message Output Data             */
	char    recn[4];        /* # of Records                    */
	struct  acc_grid    grid[1];
	/* sndl = ((# of recn) * sizeof(struct grid)) + sizeof(recn)    */
};


#define RECENT_TRCODE	"chhoinqu"

struct	recent_mid {                     /* Message Input Data   */
	char    usid[12];               /* USER ID              */
};

struct recent_grid {
	char    code[6];        /* A����nAU��a                    */
	char    hnam[20];       /* A����n��i                      */
};

struct  recent_mod {                   /* Message Output Data            */
	char    recn[4];        /* # of Records                   */
	struct  recent_grid    grid[1];
	/* sndl = ((# of recn) * sizeof(struct grid)) + sizeof(recn)    */
};


// Ư¡��

#define FEATURE_TRCODE	"chbospcl"

#define MAX_R		100
#define TIT_LEN		132
typedef struct {
	char sjgb;				// ���屸�� 0:��ü 1:�ŷ��� 2:�ڽ���
	char flag;				// 0: ����+���� 1: ���� 2: ���� 
	char gubn[2];			// �������� 00:��ü 01:��Ȳ 02:���� 03:�Ӵ������� 04:�̵��ϸ� 05:�ؿܴ��� 
							// 06:�ѱ����� 07:���մ��� 08:������Ź 09:FNGuide
	char fday[8];			// ��ȸ����
} FEATUREMID;

typedef struct {
	char code[6];				// �����ڵ�
	char titl[TIT_LEN];			// ����
} FEATUREGRID, *LPFEATUREGRID;

typedef struct {
	char nrec[4];
	FEATUREGRID grid[MAX_R];
} FEATUREMOD, *LPFEATUREMOD;


/*******************************************************************************
 * �ֽ� �ܰ��� ��ȸ IO FORMAT
 ******************************************************************************/
#define mkty_STOCK  '1'
#define mkty_FUTURE '2'
#define mkty_ETC    '3'
#define odty_QUERY  '1'
#define odty_ORDER  '2'
#define odty_TRANS  '3' 

#define	ACC_SJANGO	"PIBOSJGO"
struct  s_mid   {
	char	accn[11];
	char	pswd[8];
	char	cod2[12];
	char	sygb[1];	// 0.�ſ����� 1.All
	char	fees[10];
};

struct  s_rec   {
	char	cod2[12];	// �����ڵ�
	char	hnam[40];	// �����
	char	jgyh[2];	// �ܰ�����('00'����, '01'�ſ�, '02'~'05'���, '06'��Ÿ)
	char	jqty[10];	// �ܰ����
	char	xqty[10];	// �ŵ�����
	char	pamt[10];	// ������հ�
	char	mamt[15];	// ���Աݾ�

	char	curr[10];	// ���簡
	char	diff[10];	// ���ϴ��
	char	rate[10];	// �����
	char	camt[15];	// �򰡱ݾ�
	char	tamt[15];	// �򰡼���
	char	srat[10];	// �򰡼��ͷ�
	char	sycd[2];	// �ſ��ڵ�
	char	sydt[8];	// ������
	char	samt[15];	// �ſ�ݾ�
};

struct  s_mod   {
	char	accn[11];
	char    nrec[4];
	struct  s_rec   rec[1];
}; 

/*******************************************************************************
 * ����/�ɼ� �ܰ��� ��ȸ IO FORMAT
 ******************************************************************************/
#define	ACC_FJANGO	"PIBOFJGO"
struct f_mid {  // ���� Input
	char accn[11];
	char pswd[8];
	char date[8];
	char cod2[8];
};

struct f_rec {
	char cod2[8];	// �����ڵ�
	char hnam[30];	// �����
	char dsgb[6];	// �Ÿű��� - "+�ż�", "-�ŵ�"

	char bqty[10];	// �ܰ����
	char cqty[10];	// û�갡�ɼ���
	char pprc[10];	// ��հ�
	char curr[10];	// ���簡
	char diff[10];	// ��հ� ���
	char pamt[15];	// �򰡱ݾ�
	char psam[15];	// �򰡼���
	char prat[10];	// ���ͷ�
	char tamt[15];	// ���Աݾ�
};

struct f_mod {
	char accn[11];
	char nrec[4];
	struct f_rec rec[1];
};

#define L_smid	sizeof(struct s_mid)				// Length of stock input
#define L_smod	sizeof(struct s_mod) - sizeof(struct s_rec)	// Length of stock output
#define L_srec	sizeof(struct s_rec)				// Length of stock Grid

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid


#endif // __CONFIGBAR_TR_H__

