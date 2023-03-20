#pragma once

#define	clBackIn	66
#define	clBack		64
#define	clcontentsBack		181
#define clHeadText	130
#define	clText		69
#define	clUp		94
#define	clDown		95
#define	clHead		74
#define	clHeadBack	77
#define	clHText		76
#define	clData		68
#define	clLine		65
#define	clRow1		66
#define	clRow2		67
#define	clBuyH		85
#define	clSellH		82
#define	clBuy		85
#define	clSell		82
#define	clSText		81
#define clBuyhead   164
#define clCellhead  162

#define	IDC_CODE	10001
#define	IDC_SEARCH	10002
#define	IDC_SYM		10003
#define	IDC_STOP	10004
#define	IDC_INQ		10005
#define	IDC_TBL1	10006
#define	IDC_TBL2	10007
#define	IDC_GRID	10008
#define	IDC_SCHG	10009
#define	IDC_SVOL	10010
#define	IDC_BVOL	10011
#define	IDC_BCHG	10012

#define	hROW		18

//
//	table columns
//
#define	nROWx		6
#define	nCOLx		5

#define	iSCHG		0		// �ŵ� ������
#define	iSVOL		1		// �ŵ� ����
#define	iSVOLp		2		// �ŵ� ���� (����ġ)
#define	iSNAM		3		// �ŵ� �ŷ���
#define	iSCOD		4		// �ŵ� �ŷ��� �ڵ�

#define	iBCOD		0		// �ż� �ŷ��� �ڵ�
#define	iBNAM		1		// �ż� �ŷ���
#define	iBVOL		2		// �ż� ����
#define	iBVOLp		3		// �ż� ���� (����ġ)
#define	iBCHG		4		// �ż� ������

struct	_tbls	{
	char*	heads;
	int	width;
	int	hclr;
	int	dclr;
	int	tclr;
	DWORD	format;
	DWORD	attr;
} tbls1[nCOLx] = {
	{ "������",	72,	clHeadBack,	clSell,		clText,	GVFM_RIGHT,	GVAT_COMMA		},	// 0	�ŵ� ������
	{ "�ŵ�����",	85,	clHeadBack,	clSell,		clText,	GVFM_RIGHT,	GVAT_COMMA		},	// 1	�ŵ� ����
	{ "�ŵ�����p",	0,	clHeadBack,	clSell,		clText,	GVFM_RIGHT,	GVAT_HIDDEN		},	// 2	�ŵ� ���� (����ġ)
	{ "�ŵ�����",	98,	clHeadBack,	clSell,		clText,	GVFM_CENTER,	GVAT_CONDITIONx		},	// 3	�ŵ� �ŷ���
	{ "�ŵ��ڵ�",	0,	clHeadBack,	clSell,		clText,	GVFM_LEFT,	GVAT_HIDDEN		},	// 4	�ŵ� �ŷ��� �ڵ�	
};

struct	_tbls	tbls2[nCOLx] = {
	{ "�ż��ڵ�",	0,	clHeadBack,		clBuy,		clText,		GVFM_LEFT,	GVAT_HIDDEN		},	// 0	�ż� �ŷ��� �ڵ�
	{ "�ż�����",	97,	clHeadBack,		clBuy,		clText,		GVFM_CENTER,	GVAT_CONDITIONx		},	// 1	�ż� �ŷ���
	{ "�ż�����",	85,	clHeadBack,		clBuy,		clText,		GVFM_RIGHT,	GVAT_COMMA		},	// 2	�ż� ����
	{ "�ż�����p",	0,	clHeadBack,		clBuy,		clText,		GVFM_RIGHT,	GVAT_HIDDEN		},	// 3	�ż� ���� (����ġ)
	{ "������",	72,	clHeadBack,		clBuy,		clText,		GVFM_RIGHT,	GVAT_COMMA		},	// 4	�ż� ������
};

struct	_syms	{
	char*	sym;
	char*	rts;
	int	flag;
	int	row;
	int	col;
} syms[] = {
	{ "1021",	"021",		-1,	0,	0	},	// rts key
	{ "17413",	"",		-2,	0,	0	},	// symbol control
	{ "2179",	"179",		0xff,	0,	0	},	// static - schg
	{ "2176",	"176",		0xfe,	0,	0	},	// static - svol
	{ "2177",	"177",		0xfd,	0,	0	},	// static - bvol
	{ "2180",	"180",		0xfc,	0,	0	},	// static - bchg

	{ "2136",	"",		0,	1,	iSCHG	},	// �ŵ�������
	{ "2137",	"",		0,	2,	iSCHG	},
	{ "2138",	"",		0,	3,	iSCHG	},
	{ "2139",	"",		0,	4,	iSCHG	},
	{ "2140",	"",		0,	5,	iSCHG	},
	{ "2131",	"131",		0,	1,	iSVOL	},	// �ŵ�����
	{ "2132",	"132",		0,	2,	iSVOL	},
	{ "2133",	"133",		0,	3,	iSVOL	},
	{ "2134",	"134",		0,	4,	iSVOL	},
	{ "2135",	"135",		0,	5,	iSVOL	},
	{ "2121",	"121",		0,	1,	iSNAM	},	// �ŵ��ŷ���
	{ "2122",	"122",		0,	2,	iSNAM	},
	{ "2123",	"123",		0,	3,	iSNAM	},
	{ "2124",	"124",		0,	4,	iSNAM	},
	{ "2125",	"125",		0,	5,	iSNAM	},
	{ "2126",	"126",		0,	1,	iSCOD	},	// �ŵ��ŷ��� �ڵ�
	{ "2127",	"127",		0,	2,	iSCOD	},
	{ "2128",	"128",		0,	3,	iSCOD	},
	{ "2129",	"129",		0,	4,	iSCOD	},
	{ "2130",	"130",		0,	5,	iSCOD	},
	{ "2156",	"156",		1,	1,	iBCOD	},	// �ż��ŷ��� �ڵ�
	{ "2157",	"157",		1,	2,	iBCOD	},
	{ "2158",	"158",		1,	3,	iBCOD	},
	{ "2159",	"159",		1,	4,	iBCOD	},
	{ "2160",	"160",		1,	5,	iBCOD	},
	{ "2151",	"151",		1,	1,	iBNAM	},	// �ż��ŷ���
	{ "2152",	"152",		1,	2,	iBNAM	},
	{ "2153",	"153",		1,	3,	iBNAM	},
	{ "2154",	"154",		1,	4,	iBNAM	},
	{ "2155",	"155",		1,	5,	iBNAM	},
	{ "2161",	"161",		1,	1,	iBVOL	},	// �ż�����
	{ "2162",	"162",		1,	2,	iBVOL	},
	{ "2163",	"163",		1,	3,	iBVOL	},
	{ "2164",	"164",		1,	4,	iBVOL	},
	{ "2165",	"165",		1,	5,	iBVOL	},
	{ "2166",	"",		1,	1,	iBCHG	},	// �ż�������
	{ "2167",	"",		1,	2,	iBCHG	},
	{ "2168",	"",		1,	3,	iBCHG	},
	{ "2169",	"",		1,	4,	iBCHG	},
	{ "2170",	"",		1,	5,	iBCHG	},
};
#define	L_syms		sizeof(syms)/sizeof(struct _syms)

//
//	grid columns
//
#define	nROW		30
#define	nCOL		9

#define	iCODE		0	// �ڵ�
#define	iNAME		1	// �ŷ���
#define	iSVOLx		2	// �ŵ�����
#define	iBVOLx		3	// �ż�����
#define	iBBVL		4	// ���ż���
#define	iBBVLp		5	// ���ż��� (����ġ)
#define	iCHAG		6	// ������
#define	iTIME		7	// �����ð�
#define	iRATE		8	// ����

//20011.11.16 KSJ
//������ GVAT_CONDITION �߰�

struct	_infos {
	char*	heads;
	int	width;
	int	dclr;
	int	tclr;
	DWORD	format;
	DWORD	attr;
} infos[nCOL] = {
	{ "�ڵ�",	0,	clData,		clText,		GVFM_CENTER,	GVAT_HIDDEN			},	// 0	�ڵ�
	{ "�ŷ���",	80,	clData,		clText,		GVFM_LEFT,	GVAT_CONDITIONx			},	// 1	�ŷ���
	{ "�ŵ�����",	70,	clSell,		clSText,	GVFM_RIGHT,	GVAT_COMMA			},	// 2	�ŵ�����
	{ "�ż�����",	70,	clBuy,		clText,		GVFM_RIGHT,	GVAT_COMMA			},	// 3	�ż�����
	{ "���ż���",	70,	clData,		clText,		GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION	},	// 4	���ż���
	{ "���ż���p",	0,	clData,		clText,		GVFM_RIGHT,	GVAT_HIDDEN			},	// 5	���ż��� (����ġ)
	{ "������",	60,	clData,		clText,		GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION			},	// 6	������
	{ "�����ð�",	60,	clData,		clText,		GVFM_CENTER,	0				},	// 7	�����ð�
	{ "����",	60,	clData,		clText,		GVFM_RIGHT,	0				},	// 8	����
};
#define	GRID_WIDTH		470

//========================================================================================================
//
//	I/O Format
//

#define	gridTR	"pibo2301"
#define	oopTR	"pooppoop"
#define	gKEY	1
#define	oKEY	0

struct	_inR	{
	char	code[6];		// �����ڵ�
};
#define	L_inR		sizeof(struct _inR)

struct	_recR	{
	char	code[3];		// �ŷ����ڵ�
	char	name[20];		// �ŷ�����
	char	dvol[9];		// �ŵ���
	char	svol[9];		// �ż���
	char	smsu[9];		// ���ż���
	char	chag[9];		// ������
	char	time[6];		// ���Žð�
	char	rate[5];		// ����
};
#define	L_recR		sizeof(struct _recR)

struct	_outR	{
	char	rts[7];			// rts code
	char	nrec[4];		// nrec
};
#define	L_outR		sizeof(struct _outR)


