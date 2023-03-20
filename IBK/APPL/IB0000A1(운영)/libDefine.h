#pragma once

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


// grInfoEdit : grid column define
static struct _grInfo grInfo[] = {
	//{ "��ǥ",	0,	"A001",	GVFM_CENTER,	GVAT_HIDDEN|GVAT_ONLYROW|GVAT_CHECK,	"",	0},
	{ "��ȣ",	40,	"A002",	GVFM_RIGHT,	GVAT_ONLYROW,			"",	0},
	{ "���¹�ȣ",	110,	"A003",	GVFM_CENTER,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "����",	60,	"A004",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "��й�ȣ",	56,	"A005",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_EDIT,		"",	0},
	//{ "����",	0 /*50*/,	"A006",	GVFM_RIGHT,	GVAT_ONLYROW|GVAT_EDIT|GVAT_SORTVAL|GVAT_HIDDEN,	"",	0},
	{ "���º�Ī",	80,	"A007",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR|GVAT_EDIT,"",	0},
	{ "�������",   60,    "A008", GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR|GVAT_EDIT,"",	0},
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
#define	IDC_XXGRID		1001

// section
#define	GRACC			"GRACC"
#define	GREDIT			"GREDIT"

// screen
//**#define	SCRWIDTH	748
//**#define	SCRHEIGHT	458

// COLOR
#define	CLRBG		64			// background color

#define	WM_ACCSEARCH	WM_USER + 110		// ���¸� ��ȸ


// ledger
#define LEDGER_ECODE(param) ((_ledgerH*)param)->ecod
#define LEDGER_CONF(param)  ((_ledgerH*)param)->next[0]


