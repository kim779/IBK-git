#ifndef _DATAIO_H_
#define _DATAIO_H_

//
//	�������� UPLOAD / DOWNLOAD
//

#define	trUPDOWN	"pidomyst"

struct	_uinfo {
	char	gubn[2];	// ��������
				// "MY" : ��������
				// "UL" : ����� ������ ��� 
				// "UD" : ����� ������
	char	dirt[1];	// ���۹��� 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : ����ȸ
	char	cont[1];	// ���ӱ��� 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel, 
				// 'S':�׷�����, 'E':�׷�����ó��, 'b':DB���, 'R':DB����, 'V':������������, 'r':�������º���
				// 'D':�׷����, 'j':�ش�׷���������, 'g':�׷츮��Ʈ��ȸ, 'G':�׷���������, 'C':�������
	char	name[80];	// file name
	char	nblc[5];	// ������, ����ũ��(max:64k)
	char	retc[1];	// return value 'O':���� 'E':���� 'U'�ű� ����
	char	emsg[40];	// error message
};

struct	_ginfo {
	char	gnox[2];	// group number
	char	gnam[20];	// group name
	char	jrec[4];	// _jinfo count
//	char	jptr[1];	// _jinfo pointer
};

struct	_jinfo {
	char	gubn[1];	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	xprc[10];	// �����ܰ�
	char	xnum[10];	// ��������
};

struct _foldex {
	char	gubn[1];	//�Է±��� D, G
	char	kjid[2];	//���ɱ׷� ������
	char	name[20];	//���ɱ׷��
	char	nrec[4];	//���񰳼�
};

struct	_updn {
	_uinfo	uinfo;
	_ginfo	ginfo;
};

#define MAX_GR 100

struct  _glistex {
	char	ngrs[2];	// New Group Seqn
	char	ogrs[2];	// Old Group Seqn
	char	gnam[30];
};

struct  _gfoldex {
	char	nrec[4];	// Group Count
	struct	_glistex glist[MAX_GR];
};

struct  _updn2 {
	_uinfo	uinfo;		// ����� ���� HEAD
	_gfoldex gfoldex;
};

/////////////////////////////11.04 �ű�/////////////////////////
struct	jlist {			// ��������
	char	gubn[1];	// ���񱸺�
	char	code[12];	// �����ڵ�
				// ����(6), ����/�ɼ�(8)
				// �����ϵ��ݵ�(9)
				// �����ڵ�(5)
	char	bqty[10];	// ��������
	char	bprc[10];	// �ż�����
};

struct	fold {			// ��������
	char	kjid[2];	// ���ɱ׷��ڵ�(00~99)
	char	name[20];	// ���ɱ׷��
	char	nrec[4];	// ���񰹼�(space ����)
	struct	jlist   jlist[MAX_GR];
};


const int sz_uinfo =	sizeof(struct _uinfo);
const int sz_ginfo =	sizeof(struct _ginfo);
const int sz_jinfo =	sizeof(struct _jinfo);
const int sz_updn =	sizeof(struct _updn);
const int sz_updn2 =	sizeof(struct _updn2);
const int sz_foldex =	sizeof(struct _foldex);
const int sz_gfoldex =	sizeof(struct _gfoldex);
const int sz_glistex =	sizeof(struct _glistex);

#endif