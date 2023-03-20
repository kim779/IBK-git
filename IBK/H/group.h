#pragma once

#pragma pack(1)
#define	TR_GROUP	"PIDOSETA"	//  server TR

// MAX Count
#define	MAX_GP		100			// �ִ� �׷찹��
#define	MAX_EDIT	40			// �ִ� ���� ��ϰ���

//======================================================================
// group I/O : ihead�� sizeū ��쿡�� ���(14K�̻�)
// I : _gmid + { _group + _accn * ��ϰ��¼� } * �׷��
// 14K���� ū ����Ÿ ����� _ihead + ���尪(14K)�� ������ ����
// O : _gmod
//
// piioseta.c
//======================================================================
struct  _ihead  {
	char	xflg[1]{};		// 'F':first, 'N':next, 'E':end(�̰��� 14K�̻��϶��� ����)
	char	csiz[6]{};		// ���� size	
	char	fsiz[6]{};		// file size
	char	fill[3]{};
};
#define	L_ihead		sizeof(struct _ihead)

struct  _gmid {
	char    xflg{};			// 1: ��ȸ
					// 2: ����
	char	usrid[12]{};		// �����ȣ
	char	sflag{};			// 1: �ֽ�
					// 2: ����/�ɼ�
	char    grec[4]{};		// �׷찹��
//	char	fill;			// �ι���Ʈ(struct  group    i_group[1];)
};
#define	L_gmid		sizeof(struct _gmid)

struct  _gmod {
	char    okgb{};			// 'Y':���� , 'N':����
	char    grec[4]{};		// �׷찹��
//	char	fill;			// �ι���Ʈ(struct  group    o_group[1];)
};
#define	L_gmod		sizeof(struct _gmod)

struct  _group {
	char	xflg{};			// 'U': Update, 'D': Delete, 'I': Insert
	char	gpid[3]{};		// ���±׷���̵�
	char    seqn[3]{};		// ���±׷��Ϸù�ȣ
	char    gnam[20]{};		// �׷��
	char    nrec[4]{};		// ���� COUNT
//	char	fill;			// �ι���Ʈ(struct  accn    accn;)
};	
#define	L_group		sizeof(struct _group)

struct  _accn {
	char    seqn[3]{};		// �����Ϸù�ȣ
	char    accn[11]{};		// ���¹�ȣ
	char	pass[10]{};	// ���� ��й�ȣ
	char    acnm[30]{};	// ���¸�
	char    rate[5]{};	// ��������
	char    multi[7]{};	// �¼�
	char    alis[30]{};	// ���º�Ī
	char    prea{};		// 1:��ǥ����
//	char	fill;		// �ι���Ʈ
};
#define	L_accn		sizeof(struct _accn)

struct _acif {
	char	acno[11]{};
	char	gubn[2]{};
};
#define	L_acif		sizeof(struct _acif)

struct _acif_out {
	char	grcd[2]{};
	char	grnm[40]{};
	char	regy[1]{};
	char	lgnm[100]{};
	char	prnm[100]{};
};
#define	L_acif_out		sizeof(struct _acif_out)

#pragma pack()