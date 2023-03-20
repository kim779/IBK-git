// definitions for Hoga Fast Click IB300200
#pragma once
#define	cDEL	0x7f		// Delimiter
#define	cTAB	'\t'
#define	cLF		'\n'
#define	cCR		'\r'
#define	cNL		0x0A

#define MD_JUMUN	1
#define MS_JUMUN	2

#define ACC_WIDTH	140
#define CTRL_HEIGHT	20

#define LABEL_WIDTH 50
#define TEXTOUT_WIDTH 55

#define MAP_WIDTH	598
#define MAP_HEIGHT	582
#define MAP_HEIGHT2	365

#define ATTACHMAP_WIDTH		300
#define ATTACHMAP_HEIGHT	533

#define HOGA_WIDTH	422
#define HOGA_HEIGHT	454

#define CTRL_WIDTH	160

#define KEY_ACCOUNT	255
#define KEY_ACCNAME	254
#define KEY_JANGO	201
#define KEY_TRAN	202
#define KEY_JUMUN	203
#define KEY_MAXMDJMCN	204
#define	KEY_MAXMSJMCN	205
#define KEY_MAXMISUJMCN	206
#define KEY_MICHEG	207
#define KEY_NOTIFYCTRL	208
#define KEY_JUMUNMDCAN	209
#define KEY_JUMUNMSCAN	210
#define KEY_COVERCAN	211
#define KEY_CANALLS	212
#define KEY_CANALLB	213
#define KEY_CHKPASS	100
#define KEY_OPTIONEX 253

#define TK_CHECKPSWD	23

#define COLOR_LINE	93
#define COLOR_TABLE	75
#define COLOR_BK	64
#define COLOR_GRIDHEAD	74
#define COLOR_GUIDEBACK	213
#define COLOR_PANELBACK	66
#define COLOR_BODYBACK	181
#define COLOR_GRIDHEADTXT	76


#define COLOR_DATA	99
#define COLOR_TEXT	69
#define COLOR_TABLEHEAD	96
#define	COLOR_MDB	171		// �ŵ� back
#define	COLOR_MSB	170		// �ż� back
#define COLOR_PLUSTXT	94
#define COLOR_MINUSTXT	95

#define VERT_GAP	3
#define HORI_GAP	3
#define BOT_GAP		6

#define GAP	2


#define ST_TEXT         0x00000000	// �ؽ�Ʈ
#define ST_NORMAL       0x00000001	// 0 ���� 
#define ST_COLOR        0x00000002	// Į�� 
#define ST_REMOVE	0x00000004	// +, - ����
#define ST_COMMA	0x00000008	// comma
#define ST_PERCENT      0x00000010	// %
#define ST_SHGA		0x00000020	// ����/���� ��� �϶� ǥ��
#define ST_BOLD		0x00000040	// bold
#define ST_DRYL		0x00000080	// ����� ǥ��	
#define ST_MMCOLOR	0x00000100	// �Ÿű���
#define ST_BGCOLOR	0x00000200  // ��׶��� ����	koga~jega range
#define ST_PLUS_REMOVE 0x00000400	// +�� ���� (�����)

#define MAX_FC	30

struct _smid
{
	char	code[8];	        // �����ڵ�
};
#define sz_SMID		sizeof(struct _smid) 

struct  _smod
{
    char    curr[9];                // ���簡       
    char    diff[9];                // ���ϴ��     
    char    gvol[12];               // �ŷ���  
    char 	gamt[12];		// �ŷ����  
	char    rate[9];		// �����
    char    shga[9];                // ���Ѱ�       
    char    hhga[9];                // ���Ѱ� 
    char    gjga[9];                // ���ذ�       
    char    siga[9];                // �ð�         
    char    koga[9];                // ��         
    char    jega[9];                // ����         
    struct  {
            char     mdga[9];	// �ŵ�ȣ��            (999V99) 
            char     msga[9];       // �ż�ȣ��            (999V99)
            char     dvol[9];       // �ŵ�ȣ������        (���)  
            char     svol[9];       // �ż�ȣ������        (���)  
            char     dcnt[9];       // �ŵ�ȣ���Ǽ�
            char     scnt[9];       // �ż�ȣ���Ǽ�
    } hoga[5];		        // ����/�ɼ� ȣ��
	char    dvol[9];                // �ŵ�ȣ���Ѽ���
	char    svol[9];                // �ż�ȣ���Ѽ���
    char    dcnt[9];                // �ŵ�ȣ���ѰǼ�
    char    scnt[9];                // �ż�ȣ���ѰǼ�
    char 	dscha[9];		// �ܷ���
	char	mgjv[9];		// �̰�������
	struct	{
		char time[6];		// ü��ð�
		char curr[9];		// ü�ᰡ��
		char cvol[6];		// ü�����
	} cheg[MAX_FC];
};
#define sz_SMOD		sizeof(struct _smod)

struct _jinfo
{
	char    juno[6];		// ���ֹ� ��ȣ
	char    mmgb[1];		// �ŵ�/�� ���� 
	char    jprc[9];		// �ֹ�����
	char    jqty[9];		// �ֹ�����
	char    mcvl[9];		// ��ü�� ����
};
#define sz_JINFO	sizeof(struct _jinfo)

struct _config
{
	int	percent{};		// ����� ǥ�� 0:ǥ�þ���, 1:ǥ��
	int	percentRadio{};		// 0:�ð���, 1:BEP, 2:user
	int	percentKind{};		// 0:���ذ�, 1:��������, 2:�ð�, 3:��: 4:����
	CString user;
	int	mod{};			// �����ֹ��� ���/�Ϻ� 0:��� 1:�Ϻ�
	int	jmcn{};			// �ֹ����� ǥ�� ��� 0:�ֹ�(�Ǽ�) 1:�ֹ�
	int	confirm{};		// �ֹ�Ȯ��â ���� ���� 0:�Ⱥ��� 1:����
};

struct _jmid
{
	char	nrec[4];
	char	odgb;		// �ֹ����� 1:�Ϲ�, 2:����, 3:���ָŸ�, 4:�ſ�Ÿ�, 5:ä�ǸŸ�
	char	mkgb;		// ���屸�� 1:�ŷ���, 2:�ڽ���, 3:��������, 5:���
	char	mmgb;		// �Ÿű��� 1:�ŵ�, 2:�ż�, 3:����, 4:���
	char	acno[11];	// ����
	char	pswd[8];	// ���
	char	ogno[12];	// ���ֹ���ȣ
	char	code[12];	// �����ڵ�
	char	jqty[8];	// �ֹ�����
	char	jprc[10];	// �ֹ��ܰ�  ���� 100���ó�� 112.15 -> 11215
	char	hogb[2];	// ȣ������ 00:������
	char	jmgb[1];	// �ֹ����� 1:IOC, 2:FOK
	char	mdgb;		// ������� 1:�Ϻ�, 2:����
	char	prgb;		// ó������ 'X'
	char	gmo[20];
	char	gmm[20];
};
#define sz_JMID		sizeof(struct _jmid)

struct _jmod
{
	char	nrec[4];
	char	jmmo[6];	// �ֹ���ȣ
	char	mono[6];	// ���ֹ���ȣ (����/��ҽ�)
	char	idat[80];	// �Էµ���Ÿ
};
#define	sz_JMOD		sizeof(struct _jmod)

struct	_cmid	
{
	char	accn[11];
	char	pswd[8];
	char	gubn[1];	/* 0.All 1.ü�� 2.��ü��	*/
	char	juno[6];
	char	sort[1];	/* 1.���� 2.����		*/
	char	date[8];
	char	cod2[8];	/* �����ڵ�			*/
	char	mkgb[1];
	char	dsgb[1];
};
#define sz_CMID		sizeof(struct _cmid)

struct	f_rec	{
	char	mono[6];	/* ���ֹ���ȣ		*/
	char	juno[6];	/* �ֹ�  ��ȣ		*/
	char	ojno[6];	/* ���ֹ� ��ȣ		*/
	char	cod2[8];	/* �����ڵ�		*/
	char	hnam[30];	/* �����		*/
	char	odgb[8];	/* �ֹ�����		*/
	char	hogb[20];	/* �ֹ�����		*/
	char	jprc[11];	/* �ֹ�����		*/
	char	jqty[7];	/* �ֹ�����		*/
	char	dlgb[4];	/* ü�ᱸ��		*/
	char	dprc[11];	/* ü�ᰡ��		*/
	char	dqty[7];	/* ü�����		*/
	char	dtim[6];	/* ü��ð�		*/
	char	wqty[7];	/* ��ü�����		*/
	char    hqty[7];    /* ����/��Ҽ���    */
	char	stat[8];	/* ��������		*/
    char    time[6];    /* �����ð�         */
    char    jseq[6];    /* ������ȣ         */
    char    yseq[7];    /* ������ȣ         */
    char    ecod[4];    /* �ź��ڵ�         */
	char	dseq[4];	/* deal count		*/
};
#define L_FMICHE_REC		sizeof(struct f_rec)

struct	_cmod	
{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[250];
};
#define sz_CMOD		sizeof(struct _cmod)


struct _fcan_mid
{
	char accn[10];
	char pswd[8];
	char cod2[8];
	char gubn[1]; /* 0. �ű�, 1. û�� */
	char msgb[1]; /* 1. �ŵ�, 2. �ż� */
	/* û��� ���� ���ʿ�*/
	char jmyh[1]; /* 1.����, 2.���� 3.���� 4. ������ */
	char oprc[9]; /* ���� ���� ( *100) */
};
#define sz_FCAN_MID	sizeof(struct _fcan_mid)

struct _fcan_mod
{
	char cqty[7]; /* �ֹ����� ���� */
};
#define sz_FCAN_MOD	sizeof(struct _fcan_mod)


struct  pmid {
    char    accn[11];
    char    pswd[8];
};

struct  pmod {
    char    sflg[1];                /* OK '1' else ERROR    */
    char    emsg[99];
}; 

#define L_pmid	sizeof(struct pmid)
#define L_pmod	sizeof(struct pmod)


// �ִ��ֹ����ɼ���

struct max_mid
{
	char In[5];               	// ���ڵ尹��             
	char zAcntNo[20];         	// ���¹�ȣ               
	char zPwd[8];             	// ��й�ȣ               
	char zQryTp[1];           	// ��ȸ���� (1:�Ϲ�, 2:�ݾ�, 3:����)
	char lOrdAmt[16];         	// �ֹ��ݾ� (��ȸ������ 2�ϰ�츸 ���, �׿ܴ� 0���� �ԷµǾ����)
	char dRatVal[19];         	// ������ (��ȸ������ 3�ϰ�츸 ���, �׿ܴ� 0���� �ԷµǾ����)
	char zFnoIsuNo[32];       	// �����ɼ������ȣ       
	char zBnsTp[1];           	// �Ÿű��� (1 : �ŵ�, 2 : �ż�)
	char dOrdPrc[13];         	// �ֹ���                 
	char zFnoOrdprcPtnCode[2];	// �����ɼ�ȣ�������ڵ� (00:������, 03:���尡, 05:���Ǻ�������, 06:������������)
   
};

struct max_mod
{
	char Out[5];              // ���ڵ尹��           
	char zAcntNm[40];         // ���¸�               
	char zQryDt[8];           // ��ȸ��               
	char dCurPrc[13];         // ���簡               
	char lOrdAbleQty[16];     // �ֹ����ɼ���         
	char lNewOrdAbleQty[16];  // �ű��ֹ����ɼ���     
	char lLqdtOrdAbleQty[16]; // û���ֹ����ɼ���     
	char lUsePreargMgn[16];   // ��뿹�����űݾ�     
	char lUsePreargMnyMgn[16];// ��뿹���������űݾ� 
	char lOrdAbleAmt[16];     // �ֹ����ɱݾ�         
	char lMnyOrdAbleAmt[16];  // �����ֹ����ɱݾ�     
};

#define L_max_mid	sizeof(max_mid)
#define L_max_mod	sizeof(max_mod)

/*
struct _maxjmct_mid
{
	char in[5];					// ���ڵ� ����
	char zAcntNo[20];			// ���¹�ȣ
	char zPwd[8];				// ��й�ȣ
};

#define L_maxjmct_mid	sizeof(struct _maxjmct_mid)

struct _maxjmct_rec1
{
	char lDps[16];				// ������
	char lMnyoutAbleAmt[16];	// ��ݰ��ɱݾ�
	char zTp[1];				// ����
	char zTpNm1[40];			// ���и�1
};

struct _maxjmct_rec2
{
	char zShtnIsuNo[9];			// ���������ȣ
	char zIsuNo[12];			// �����ȣ
	char zIsuNm[40];			// �����
	char lBalQty[16];			// �ܰ����
	char lBalAmt[16];			// �ܰ�ݾ�
	char lEvalAmt[16];			// �򰡱ݾ�
	char lOrdAbleQty[16];		// �ֹ����ɼ���
	char dCurPrc[13];			// ���簡
};

struct _maxjmct_mod
{
	char Out1[5];				// ���ڵ� ����
	struct _maxjmct_rec1 rec1[99999];
	char Out2[5];				// ���ڵ� ����
	struct _maxjmct_rec2 rec2[99999];
};
*/
