//////////////////////////////////////////////////////////////
//	account.h
//////////////////////////////////////////////////////////////
#pragma once

#define TRCODE	"pidoaccn"
#define TRPSWD	"PIBOPBXQ"

#define LEN_ACCT	11

struct  _acInfo {
    char    accn[LEN_ACCT];     /* ���¹�ȣ			                   */
    char    acnm[45];			/* ���¸� + '|' + ���º�Ī	           */
    char    ownf[1];			/* ���ΰ����÷���			           */
	char	fill[2];			/* filler							   */
};

struct  _acSet {
    char    func[1];        /* FUNCTION CODE                */
                            /* 'Q' Query    'I' Insert      */
                            /* 'U' Update                   */
    char    usid[12];       /* ����ھ��̵�                 */
    char    errc[1];        /* Error Code                   */
    char    emsg[80];       /* Error message                */
    char    nrec[4];        /* Account Count                */
    struct  _acInfo acinfo[1];
};

struct _chkpwd {
	char	accn[LEN_ACCT];
	char	psws[8];
};

#define L_chkpwd	sizeof(struct _chkpwd)
#define	L_acinfo	sizeof(struct _acInfo)


struct _chkPwdMid {
	char in [5];
	char acctNo[20];
	char password[8];
	//char zTrxTp[1];
};

struct _chkPwdMod {
	struct _chkPwdMid in;
	char out[5];
	char zAvalYn;
	//char lPwdContErrCnt[9];
	//char lAtmPwdContErrCnt[9];
	//char lMtrsfPwdContErrCnt[9];
	//char zAcntNm[40];
};

/*
struct _chkPwdMid {
	char in [5];
	char acctNo[20];
	char password[8];
	char passwordTrxCode[5];
};

struct _chkPwdMod {
	struct _chkPwdMid in;
	char out[5];
	char qty[9];
	char passwordContErrCnt[9];
	char avalYn[1];
	char passwordTrxCode[5];
};
*/

// 2011.02.25 ������ �׷���� ������ ���� pidoseta declare by LKM
struct  t_accn {
    char    seqn[3];        /* �����Ϸù�ȣ                 */
    char    accn[11];       /* ���¹�ȣ                     */
    char    pass[10];       /* ���º�й�ȣ                 */
    char    acnm[30];       /* ���¸�                       */
    char    rate[5];        /* ��������                     */
    char    maxq[7];        /* �ִ��ֹ����� for �����ֹ�    */
    char    alis[30];       /* ���º�Ī                     */
    char    pres[1];        /* 1:��ǥ����                   */
};

struct  t_grup {
    char    xflg[1];        /* 'D' Delete    'I' Insert     */
	/* 'U' Update or Still          */
	/* OUTPUT ���: group fg        */
    char    gpid[3];        /* ���±׷���̵�               */
    char    seqn[3];        /* ���±׷��Ϸù�ȣ             */
    char    gnam[20];       /* �׷��                       */
    char    nrec[4];        /* ���� COUNT                   */
    struct  t_accn  rec[1];
};

struct  pidoseta_mid {
    char    xflg[1];        /* 1: ��ȸ      2: ����         */
    char    usid[12];       /* ����ھ��̵�                 */
    char    sflg[1];        /* 1: �ֽ�                      */
	/* 2: ����/�ɼ�                 */
	/* 3: ��ü                      */
    char    grec[4];        /* �׷찹��                     */
    char    data[1];
};

struct  pidoseta_mod {
    char    okgb[1];        /* 'Y':���� , 'N':����          */
    char    grec[4];        /* �׷찹��                     */
    char    data[1];
};