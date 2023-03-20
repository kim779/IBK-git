#pragma once

#include "inc/axisfire.h"
#include "TR_IO.h"

#pragma	pack(1)

/************************************************************************/
/* AXLOGON                                                              */
/************************************************************************/
struct axlogon_mid
{
	char user[12];	/* ���̵�				*/
	char pass[10];	/* ���Ӻ�й�ȣ			*/
	char forc[1];	/* ����[0], ����[2]		*/
	char atcp[1];	/* ����[0], �ڵ�[1]		*/
	char norm[1];	/* �ֹ�[0], �ü�[1]		*/
	char cust[1];	/* ����[0], ��[1]		*/
	char type[1];	/* OPEN API[Y] �빮��	*/	//2012.06.01 �߰� KSJ
	char rsvd[5];	/* Reserved				*/	//2012.04.17 6 -> 5�� ����
	char cpas[30];	/* ����������й�ȣ		*/
	char uips[15];	/* IP-Address			*/
	char madr[16];	/* MAC-Address			*/
};

struct axlogon_mod
{
	char rcod;
	char gbip[32];
};

#define L_axlogon_mid sizeof(struct axlogon_mid)
#define L_axlogon_mod sizeof(struct axlogon_mod)


/************************************************************************/
/* USER_TH                                                              */
/************************************************************************/
struct user_th
{
	char trxc[8];
	char key;
	char stat;
};
#define L_user_th sizeof(struct user_th)

/************************************************************************/
/* ���� ��й�ȣ ����                                                   */
/************************************************************************/
struct SACMT207_mid {
	char in [5];
	char acno[20];
	char acpw[8];
	char zTrxTp[1];
};
#define L_SACMT207_mid sizeof(struct SACMT207_mid)

struct SACMT207_mod {
	struct SACMT207_mid mid;
	char out[5];
	char lPwdContErrCnt[9];
	char lAtmPwdContErrCnt[9];
	char lMtrsfPwdContErrCnt[9];
	char zAcntNm[40];
};
#define L_SACMT207_mod sizeof(struct SACMT207_mod)

/************************************************************************/
/* LEDGER                                                               */
/************************************************************************/
struct ledger {
	char	size[6];	// 0	��������			I	S	X	������ü(Size-6)
	char	type[1];	// 6	��������			I	S		B:������, I:������, F:���ϼۼ���, N:Notice �ڵ�����(R:������, D:������)
	char	encf[2];	// 7	���౸��			I/O	S	X	��������/��ȣȭ����(00:����,��ȣȭ ����)
	char	dptr[3];	// 9	������ �ɼ�			I/O	S		��/��� ����Ÿ�� ���۵Ǵ� ��ġ(��������� ����)
	char	svcd[8];	// 12	�����ڵ�			I	Cf	��	�����ڵ�               �� TxHead(3) of Map
	char	cono[3];	// 20	ȸ���ȣ			I	S		ȸ���ȣ "991"
	char	usid[16];	// 23	�����ID			I	C		Ʈ������� �߻���Ų ����� �������� (��:����� ����ID)
	char	cogb[2];	// 39	���Ӻз��ڵ�		I	S		���� ���Ӹ�ü (HT : ��üHTS)
	char	fep1[12];	// 41	����ä�ι�ȣ		I		X	���� ������ȣ(3)+�Ϸù�ȣ(9)
	char	pbid[16];	// 53	Ʈ���������ID		I	Cf	X	���� ����ID ('@'�ν���)  �� �α��� ������ ������ ����
	char	puip[12];	// 69	����IP				I	S		����IP (168.126.63.1�� ��� "168126063001" ����)
	char	pcip[12];	// 81	�缳IP				I	C		�缳IP (168.126.63.1�� ��� "168126063001" ����)
	char	dept[3];	// 93	ó������ ��ȣ		I			ó��PC�� ��ġ�� ����
	char	brno[3];	// 96	������ȣ			I			���°� ���� ����
	char	term[8];	// 99	�ܸ���ȣ			I	Cm	��	��ü�ڵ� (���� : 001 �� : 002)
	char	lang[1];	// 107	����			I	S		K:�ѱ�, E:����, C:�߱�, J:�Ϻ�
	char	time[9];	// 108	APó���ð�			O			��û ó���ð� (HHMMSSmmm)
	char	ecod[4];	// 117	�޽����ڵ�			O			�����ڵ� :(�����ڵ�: 0000 ~ 0999, �������ڵ� : 1000 ~ 9999, ��û�� Space�� ä��)
	char	eknd[1];	// 121	��¸޽�������		O			
	char	fep2[1];	// 122	�����û����		I		X	
	char	fkey[4];	// 123	���Ű���			I/O	Cm	��	�Է�: ���Ű(1����Ʈ) �� TxHead(2) of Map (5:Ȯ�� 7:���� C:����)
	char	rcnt[4];	// 127	��û ���ڵ� ����	I	Cm	��	�Ϲ������� "0000"�� �Է�
	char	fil1[6];	// 131	Filler				-		X	
	char	next[1];	// 137	���ӿ���			I/O	C		���� ������ ���� (Y:��������, �̿��� ��:���Ӿ���)
	char	nkey[18];	// 138	����Ű				I/O	C	��	����ó���� ���� Ű��
	char	usiz[2];	// 156	�����ý�����������	I		X	IBK ����	
	char	hsiz[2];	// 158	���������������	I/O		X	�̻��
	char	msiz[2];	// 160	�����޽�������		O		X	���� �� �����޽��� (��û�� "00"�Է�)
	char	fep3[10];	// 162	Reserved for  FEP		S	X	
	char	mkty[1];	// 172	���屸��				Cfm	��	1:���� 2:���� 3:��Ÿ    ��TxHead(0) of MAP
	char	odrf[1];	// 173	�ֹ�����				Cfm	��	1:�ֹ� 2:��ȸ 3:��ü    ��TxHead(1) of MAP
	char	evid[4];	// 174	Reserved for  I/F			X	
	char	svrm[12];	// 178	����IP					S		����IP (168.126.63.1�� ��� "168126063001" ����)
	char	skip[1];	// 190	Flag for I/F
	char	sjmf[1];	// 191	Flag for I/F
	char	csym[8];	// 192	��������� ID KSJ 2012.06.04
	char	fil2[40];	// 200	Filler						X	
	char	eact[1];	// 240	��������						��������(0:���¹� 1:�޼����ڽ�)
	char	emsg[99];	// 241	�����޼���						�ڵ�(4)+�޽���(94)(9999 �����Դϴ� !)
	char	epwd[44];	// 340	sha256��ȣȭ �� ���º�й�ȣ
};
#define	L_ledger	sizeof(struct ledger)		// 340 bytes	//2014.04.15 ��ȣȭ��� �߰��ؼ� 384 bytes

struct pidouini_item
{
	char usid[ 8];   /* ����� ID                */
	char innm[100];  /* INI ���ϸ�               */
	char senm[100];  /* ���Ǹ�                   */
	char skey[100];  /* Key                      */
	char valu[500];  /* Value                    */
	char date[ 8];   /* ������Ʈ����             */
};

struct pidouini_mid
{
	char gubn[1];
	struct pidouini_item item;
};
#define L_pidouini_mid		sizeof(struct pidouini_mid)

struct pidouini_mod
{
	struct pidouini_item item;
};
#define L_pidouini_mod		sizeof(struct pidouini_mod)

struct  pihoitgy_grid
{	
    char    gubn[3];            /* ����                 */	
	/* ex) exe, dev, map    */	
    char    mnam[47];           /* ����               */	
};

struct  pihoitgy_grid2
{
    char    gubn[3];            /* ����                     */
	/* ex) exe, dev, map        */
    char    mnam[47];           /* ����                   */	
    char    skey[44];           /* Ŭ���̾�Ʈ SHA256 ���   */
};

struct  pihoitgy_mid 
{	
    char    gubn[1];            /*  1: LIST ��û    2: ���Ἲ ����  */	
    char    nrec[4];            /* ���Ἲ ������ ��û ����          */	
    struct  pihoitgy_grid2   grid2[1];	
};
#define L_pihoitgy_mid		sizeof(struct pihoitgy_mid)

struct  pihoitgy_mod 
{	
    char    nrec[4];            /*  ����            */	
    struct  pihoitgy_grid    grid[1];    	
};
#define L_pihoitgy_mod		sizeof(struct pihoitgy_mod)

struct pibotran_mid {
    char seqn[6];
    char start[8];
    char file[64];  
};
#define L_pibotran_mid		sizeof(struct pibotran_mid)

struct pibotran_mod {
    char seqn[6];
    char max_seqn[6];
    char start[8];
    char size[8];
    char data[1024*32];
};
#define L_pibotran_mod		sizeof(struct pibotran_mod)

// �ý��۸Ÿ� �����̼� ��� ���� �ڵ� : 3056
#define SYSTRADING_SVCCODE		"3056"
struct saceq320_mid
{
	char In[5];
	char fnUserId[16];
	char servCode[4];
};
#define L_saceq320_mid		sizeof(struct saceq320_mid)

struct saceq320_grid
{
	char Acnt[20];
};

struct saceq320_mod
{
	struct saceq320_mid mid;
	char In[5];
	struct saceq320_grid grid[1];
};

struct sacaq504_mid
{
	char In[5];
	char QryTp[1];           /* ��ȸ����         */
	char SvcKindCode[4];     /* ���������ڵ�   */
	char PdptnCode[2];       /* ��ǰ�����ڵ�     */
	char AcntNo[20];         /* ���¹�ȣ         */
	char BrnNo[3];           /* ������ȣ         */
	char QrySrtDt[8];        /* ��ȸ������       */
	char QryEndDt[8];        /* ��ȸ������       */
	char RegTp[1];           /* ��ϱ���         */
	char AcntStatCode[2];    /* ���»����ڵ�     */
};
#define L_sacaq504_mid		sizeof(struct sacaq504_mid)

struct sacaq504_grid
{
	char AcntNo[20];          /* ���¹�ȣ               */
	char AcntNm[40];          /* ���¸�                 */
	char PdptnCode[2];        /* ��ǰ�����ڵ�           */
	char PdptnNm[40];         /* ��ǰ������             */
	char RegTp[1];            /* ��ϱ���               */
	char RegTpNm[20];         /* ��ϱ���               */
	char SvcKindCode[4];      /* ���������ڵ�         */
	char SvcKindNm[40];       /* ����������           */
	char TrdIttNm[40];        /* �ŷ������             */
	char OppIttAcntNo[20];    /* ��������¹�ȣ       */
	char OppTrdAcntNm[40];    /* ���ŷ����¸�         */
	char CtrctRegDt[8];       /* ���������             */
	char CtrctSrtDt[8];       /* ����������             */
	char CtrctXpirDt[8];      /* ����������             */
	char IntaccNo[20];        /* ���հ��¹�ȣ           */
	char BnkDlgConnAcntNo[20];/* �����ǥ������¹�ȣ   */
	char BnkCpcrdNo[20];      /* ��������ī���ȣ       */
	char BbkpgPtnCode[2];     /* �ϰ���ü�����ڵ�       */
	char BbkpgPtnNm[40];      /* �ϰ���ü����           */
	char OthbkpTp[1];         /* Ÿ���ü����           */
	char OthbkpTpNm[10];      /* Ÿ���ü���и�         */
	char AbkpDt[2];           /* �ڵ���ü��             */
	char AmtrBkeepAmt[16];    /* �ڵ���ü��ü�ݾ�       */
	char UndDsgnYn[1];        /* �μ�����������         */
	char UndDsgnNm[40];       /* �μ�������             */
	char BuyFundNo[12];       /* �ż��ݵ��ȣ           */
	char SellFundNo[12];      /* �ŵ��ݵ��ȣ           */
	char Rmnno[13];           /* �Ǹ�Ȯ�ι�ȣ           */
	char Imreq[16];           /* �ſ뼳��������         */
	char RatVal[19];          /* ������                 */
	char MgempNm[40];         /* ���������             */
	char AbndDt[8];           /* ������                 */
	char AbndEmpNo[9];        /* ���������ȣ           */
	char AbndPtnNm[40];       /* ��������               */
	char ChgUserId[16];       /* ��������ID           */
	char AcntStatNm[40];      /* ���»��¸�             */
	char TrxIserId[16];		  /* ó����ID               */	//2014.03.31 �߰���
	char TrxBrn[3];			  /* ó������               */	//2014.03.31 �߰���
	char AcntOpnDt[8];		  /* ���°�����             */	//2014.03.31 �߰���
};

struct sacaq504_mod
{
	struct sacaq504_mid mid;
	char Out[5];              
	char AcntNm[40];          /* ���¸�                 */
	char BrnNm[40];           /* ������                 */
	char Out2[5];             
	struct sacaq504_grid grid[1];	
};


#pragma	pack()