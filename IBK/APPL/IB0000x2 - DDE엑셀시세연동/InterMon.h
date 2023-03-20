#pragma once

#include "axisfire.h"

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

struct _foldex{
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
    char    ngrs[ 2];           /* New Group Seqn       */
    char    ogrs[ 2];           /* Old Group Seqn       */
	char	gnam[30];			
};

struct  _gfoldex {
    char    nrec[ 4];           /* Group Count          */
    struct  _glistex glist[MAX_GR];
};

struct  _updn2 {
	_uinfo	uinfo;      /* ����� ���� HEAD     */
	_gfoldex gfoldex;
 };

/////////////////////////////11.04 �ű�/////////////////////////
struct jlist {          /* ��������             */
    char    gubn[1];            /* ���񱸺�             */
    char    code[12];           /* �����ڵ�             */
	/* ����(6), ����/�ɼ�(8)*/
	/* �����ϵ��ݵ�(9)      */
	/* �����ڵ�(5)          */
    char    bqty[10];           /* ��������             */
    char    bprc[10];           /* �ż�����             */
};

//struct fold {          /* ��������             */
//    char    kjid[2];            /* ���ɱ׷��ڵ�(00~99)  */
//    char    name[20];           /* ���ɱ׷��           */
//    char    nrec[4];            /* ���񰹼�(space ����) */
//    struct  jlist   jlist[MAX_GR];
//};


const int sz_uinfo   = sizeof(struct _uinfo);
const int sz_ginfo   = sizeof(struct _ginfo);
const int sz_jinfo   = sizeof(struct _jinfo);
const int sz_updn    = sizeof(struct _updn);
const int sz_updn2    = sizeof(struct _updn2);
const int sz_foldex    = sizeof(struct _foldex);
const int sz_gfoldex    = sizeof(struct _gfoldex);
const int sz_glistex    = sizeof(struct _glistex);

//
//	�ϳ����� �������� ��������
//
struct	_hanasec {
	char	head[17];	// "INTERESTGROUPFILE"
	char	gnox[2];	// �׷찳�� (������ŭ �ݺ�)

	char	gnam[40];	// �׷��̸�
	char	jrec[2];	// ���񰹼� (������ŭ �ݺ�)

	char	code[15];	// �����ڵ�
};

const int sz_hanas    = sizeof(struct _hanasec);

#define TRKEY_GROUP		0x13	// ���ɱ׷�...
#define TRKEY_INTER		0x14	// ���ɸ���Ʈ...


#pragma pack(1)
constexpr int codelen = 12;
constexpr int namelen = 32;
constexpr int pricelen = 10;
constexpr int xnumlen = 10;

struct _inters
{
	char	gubn[1]{};	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����	
	char	code[12]{};	// �����ڵ�
	char	name[32]{};	// �����
	char	xprc[10]{};	// �����ܰ�
	char	xnum[10]{};	// ��������
	char	xupnum[16]{};	// ���� �ֽļ�
	char	bookmark[1]{};	// �ϸ�ũ
	char	futureGubn[1]{};	// �ſ�Ÿ� ����
	double  creditPrc{};		// �ſ�ݾ�
	double	maeipPrc{};		// ���Աݾ�
	char	filler[6]{};		// Reserved
};


struct _intersx
{
	char	gubn = '0';		// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����	
	CString code;			// �����ڵ�
	CString name;			// �����
	CString	xprc;			// �����ܰ�
	CString	xnum;			// ��������
	CString	xupnum;			// ���� �ֽļ�
	char	bookmark = '0';		// �ϸ�ũ
	char	futureGubn{};		// �ſ�Ÿ� ����
	double  creditPrc{};		// �ſ�ݾ�
	double	maeipPrc{};		// ���Աݾ�

	void copy(void* item);
	void operator=(struct _intersx& ref)
	{
		code = ref.code;
		name = ref.name;
		gubn = ref.gubn;
		xprc = ref.xprc;
		xnum = ref.xnum;
		xupnum = ref.xupnum;
		bookmark = ref.bookmark;
		futureGubn = ref.futureGubn;
		creditPrc = ref.creditPrc;
		maeipPrc = ref.maeipPrc;
	}

	void empty()
	{
		code.Empty();
		name.Empty();
		gubn = '0';
		xprc.Empty();
		xnum.Empty();
		xupnum.Empty();
		bookmark = '0';
		futureGubn = '0';
		creditPrc = 0;
		maeipPrc = 0;
	}
};

struct	_bookmarkinfo {
	char	gubn[1]{};		// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12]{};		// �����ڵ�
	char	name[32]{};		// �����
	char	bookmark[1]{};		// �ϸ�ũ ����
};
#define	sz_inters	sizeof(struct _inters)
#define	sz_bookmark	sizeof(struct _bookmarkinfo)

struct _codeinfo
{
	char	code[12]{};	// �����ڵ�
	char	name[32]{};	// �����
	char	capture[1]{};	// ���࿩�� 	'0':������, '1':�����Ϸ�

	struct _alarm
	{
		char	kind[1]{};// �����׸�	0:none, 1:���簡, 2:���ϴ��, 3:�ŵ�ȣ��, 4:�ż�ȣ��, 5:�����, 6:�ŷ���
		char	aval[10]{};//���ú񱳰�
		char	cond[1]{};// ��������	'0':�̻�, '1':����
		char	oper[1]{};// ���ÿ���	'0':AND,  '1':OR
	}alarm[6]{};

	char	o_run[1]{};	// �ֹ�â	'0':�̽���,'1':����
	char	o_kind[1]{};	// �ֹ�����	'0':�ŵ�,  '1':�ż�
	char	o_mmgb[1]{};	// �Ÿű���	'0':������,'1':���尡
	char	o_num[10]{};	// �ֹ�����
	char	o_prc[10]{};	// �ֹ��ܰ�

	char	a_kind[1]{};	// �˶�����	0x01: Ȯ��â, 0x02: ���������ȣ���� 0x04:�Ҹ�����
	char	a_wave[128]{};	// �Ҹ�����
	char	filler[20]{};	// Reserved
};

#define	sz_codeinfo	sizeof(struct _codeinfo)

struct	_hisinfo
{
	char	date[14]{};	// ��������,�ð�
	struct	_codeinfo	codeinfo {};
	char	gval[6][10]{};	// ������ ����
	char	chk[6]{};		// check
};
#define	sz_hisinfo	sizeof(struct _hisinfo)


class InterMon
{
public :

	InterMon(CWnd* pParent, CWnd* pOwner) {
		m_parent = pParent;
		m_owner = pOwner;
	}
	

	CWnd* m_parent{};
	CWnd* m_owner{};

	int m_group_count = 0;

	std::vector<std::pair<CString, CString>> m_groupname;
	CArray <std::shared_ptr<_inters>, std::shared_ptr<_inters>> m_inters;

	void Request_GroupList();
	void Request_GroupCode(int iseq, int vidx);
	void receiveOub(int key, CString data, int vidx);
	void sendTR(CString trCode, char* datB, int datL, int key);
};
