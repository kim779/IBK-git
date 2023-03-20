#pragma once


#define GROUPNUM 4
#define ACCNTNUM 5
#define STRLENGTH 20

using namespace std;

class ACCNTINFO
{
public:
	CString sAccntSortNum;	//�����Ϸù�ȣ
	CString sAccntNum;	//���¹�ȣ
	CString sAccntName;	//�����̸�
	CString	sPassword;	//���� ���
	CString sAllocRate;	//��������
	CString sAccntNickName;	//���º�Ī
	CString sPriAccnt;	//��ǥ���±���
	
	ACCNTINFO(){
		sAccntSortNum.Empty();
		sAccntNum.Empty();
		sAccntName.Empty();
		sPassword.Empty();
		sAllocRate.Empty();
		sAccntNickName.Empty();
		sPriAccnt.Empty();
	};
};

typedef vector<ACCNTINFO> ACCINFOVEC;

class GRINFO
{
public:
	CString sGrSortNum;		// �׷��Ϸù�ȣ
	CString sGrName;		// �׷��
	CString sAccntCnt;		// ���°���
	CString	sGroupID;		// �׷���̵�
	ACCINFOVEC AccInfo;
	
	GRINFO()
	{
		sGrSortNum.Empty();
		sGrName.Empty();
		sAccntCnt.Empty();
		sGroupID.Empty();
	};
};

struct  _ihead  {
	char	xflg[1];		// 'F':first, 'N':next, 'E':end(�̰��� 14K�̻��϶��� ����)
	char	csiz[6];		// ���� size	
	char	fsiz[6];		// file size
	char	fill[3];
};
#define	L_ihead		sizeof(struct _ihead)

typedef struct  _group {
	char	xflg;		/* 'U': Update, 'D': Delete, 'I': Insert */
	char	gpid[3];	/* ���±׷���̵�	*/
	char    seqn[3];	/* ���±׷��Ϸù�ȣ	*/
	char    gnam[20];	/* �׷��		*/
	char    nrec[4];	/* ���� COUNT		*/
	char	fill;		// �ι���Ʈ(struct  accn    accn;)
}groupINFO;

typedef struct  _accn {
	char    seqn[3];	/* �����Ϸù�ȣ		*/
	char    accn[10];	/* ���¹�ȣ		*/
	char	pass[10];	/* ���� ��й�ȣ	*/
	char    acnm[30];	/* ���¸�		*/
	char    rate[5];	/* ��������		*/
	char	multi[7];	/* �¼�	2006.10.19�߰�	*/
	char    alis[30];	/* ���º�Ī		*/
	char    prea;		/* 1:��ǥ����		*/
	char	fill;		// �ι���Ʈ
}accINFO;

typedef struct  _mid {
	char    xflg;		/* 1: ��ȸ		*/
				/* 2: ����		*/
	char	usrid[12];	/* �����ȣ		*/
	char	sflag;		/* 1: �ֽ�		*/
				/* 2: ����/�ɼ�		*/
	char    grec[4];	/* �׷찹��		*/
	char	fill;		// �ι���Ʈ(struct  group    i_group[1];)
}groupMid;

typedef struct  _mod {
	char    okgb;		/* 'Y':���� , 'N':����	*/
	char    grec[4];        /* �׷찹��		*/
	char	fill;		// �ι���Ʈ(struct  group    o_group[1];)
}groupMod; 


struct  _acInfo {
    char    accn[10];       /* ���¹�ȣ                     */
    char    acnm[20];       /* ���¸�                       */
    char    aseq[2];        /* ���¼���                     */
                            /* 00: �⺻����                 */
                            /* 01: ȭ��� ������ �ʴ°��� */
    char    pwch[1];        /* ���࿬����¿���             */
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

#define	L_acinfo	sizeof(struct _acInfo)

