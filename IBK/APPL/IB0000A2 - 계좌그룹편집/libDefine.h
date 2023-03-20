#pragma once


#include "../../h/group.h"
#include "xxgrid.h"

// grid 
struct _grInfo {
	char*	heads;
	int	width;
	char*	symbols;
	UINT	format;
	DWORD	attr;
	char*	defaultVal;
	int	param;
};
#define GRIDROW_HEIGHT  18

// grInfoAcc : grid column define
static struct _grInfo grInfoAcc[] = {
	{ "���¹�ȣ",	90,	"A001",	GVFM_CENTER,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "����",	120,	"A002",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "Idx",	0,	"A003",	GVFM_CENTER,	GVAT_ONLYROW|GVAT_HIDDEN,	"",	0},	
};
enum { IACOL_ACC = 0, IACOL_NAME, IACOL_IDX };

// grInfoEdit : grid column define
static struct _grInfo grInfoEdit[] = {
	{ "��ǥ",	0,	"A001",	GVFM_CENTER,	GVAT_HIDDEN|GVAT_ONLYROW|GVAT_CHECK,	"",	0},
	{ "��ȣ",	40,	"A002",	GVFM_CENTER,	GVAT_ONLYROW,			"",	0},
	{ "���¹�ȣ",	90,	"A003",	GVFM_CENTER,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "����",	120,	"A004",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "��й�ȣ",	0/*56*/,	"A005",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_EDIT|GVAT_HIDDEN,		"",	0},
	{ "����",	0 /*50*/,	"A006",	GVFM_RIGHT,	GVAT_ONLYROW|GVAT_EDIT|GVAT_SORTVAL|GVAT_HIDDEN,	"",	0},
	{ "���º�Ī",	0/*105*/,	"A007",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR|GVAT_EDIT|GVAT_HIDDEN,"",	0},
};
enum { IECOL_PRI = 0, IECOL_SEQ, IECOL_ACCNUM, IECOL_ACCNAME, IECOL_PASSWORD, IECOL_RATE, IECOL_ALIAS };

//===========================================================================================================
const int MAX_SEND = 14*1024;			// �ִ� send size(14K�Ѵ°��)

// Send Sub Key
#define SC_ACCCUST		100		// ��������ȸ
#define SC_ACCLIST		101		// ����������ȸ
#define SC_ACCLISTBYGROUP	102		// �׷������ȸ
#define SC_SAVEACC		103		// �׷��������(ȭ�����)
#define SC_SAVEACCONLY		104		// �׷��������(���常��)
#define	SC_INPUTACCOUNTVALID	106		// �Է� ���� ��� �˻�
#define	SC_ACCOUNTNAME		107		// ���°���
#define	SC_ACCPASSVALID		109		// ���� ��ȿ���˻�(���edit�Է½�)
#define	SC_DLGACCOUNTNAME	111		// ���¸���ȸ

// macro
#define	LIB_DELETE(XXX) { delete XXX; XXX = NULL; }
#define	BUF_DELETE(XXX)	{ delete[] XXX;  XXX = NULL; }

// IDC define
#define	IDC_GRACC		1000
#define	IDC_GREDIT		1001

// section
#define	GRACC			"GRACC"
#define	GREDIT			"GREDIT"

// screen
#define	SCRWIDTH	748
#define	SCRHEIGHT	458

// COLOR
#define	CLRBG		66			// background color

#define	WM_ACCSEARCH	WM_USER + 110		// ���¸� ��ȸ


// ledger
#define LEDGER_ECODE(param) ((_ledgerH*)param)->ecod
#define LEDGER_CONF(param)  ((_ledgerH*)param)->next[0]

	

//======================================================================
// ��������ȸ I/O
//======================================================================
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

#define	L_acSet		sizeof(struct _acSet)
#define	L_acInfo	sizeof(struct _acInfo)

