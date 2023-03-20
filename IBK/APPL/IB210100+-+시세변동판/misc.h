#pragma once

#define	IDC_GRID	10001
#define	IDC_SETUP	10002
#define	IDC_STOP	10003
#define IDC_PANEL	10004
#define IDC_PANEL2	10004

#define	hROW		18
#define	nROW		300
#define	nCOL		9

#define	iCODE		0	// �ڵ�
#define	iTIME		1	// �ð�
#define	iNAME		2	// �����
#define	iCURR		3	// ���簡
#define	iDIFF		4	// ���
#define	iRATE		5	// �����
#define	iQTY		6	// �ŷ���
#define	iCVOL		7	// ����ü�ᷮ
#define	iCAMT		8	// ����ü��ݾ�

#define SAVE_HEIGHT "HEIGHT"

#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		3
#define GAP_BLOCK		5
#define GAP_PANEL		4

#define PANAL_HEIGHT	26
#define BUTTON_HEIGHT	20
#define STOPBT_WIDTH	36
#define SETUPBT_WIDTH	84
#define INCREASE_WIDTH	7
#define INCREASE_HEIGHT	2

#define	MAP_WIDTH	568 + 2 * (GAP_SIDE + GAP_PANEL)
#define	MAP_HEIGHT	385 + GAP_TOP + GAP_BOTTOM + GAP_BLOCK + 2 * GAP_PANEL

#define WINDOW_RECT		CRect(0, 0, MAP_WIDTH, MAP_HEIGHT)

#define ROUND_PANEL		1
#define ROUND_CONTENTS	2

#define	clBack	64
#define clBox	65
#define	clText	69
#define	clUp	94
#define	clDown	95

#define clBtnText 186

//�ǳ�
#define clPanel	66
#define clPanelLine 165

//contents �ܰ�
#define clContents 181
#define clGuide	26

//tab
#define	clTabf	70
#define	clTabsf	71

//grid
#define	clHead	74
#define	clHText	76
#define	clData	68
#define	clLine	93
#define	clFocus	78
#define	clRow1	68
#define	clRow2	77
#define clSel	78

//
//	grid columns
//
struct	_infos {
	char*	heads;
	int	width;
	DWORD	format;
	DWORD	attr;
} infos[nCOL] = {
	{ "�ڵ�",		60,	GVFM_CENTER,	0					},	// 0	�ڵ�
	{ "�ð�",		0,	GVFM_CENTER,	GVAT_HIDDEN				},	// 1	�ð�
	{ "�����",		100,	GVFM_LEFT,	0					},	// 2	�����
	{ "���簡",		70,	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx		},	// 3	���簡
	{ "���",		60,	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST		},	// 4	���
	{ "�����",		60,	GVFM_RIGHT,	GVAT_COMMA|GVAT_FLOAT2|GVAT_CONDITION	},	// 5	�����
	{ "�ŷ���",		80,	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx		},	// 6	�ŷ���
	{ "ü�ᷮ",		80,	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx		},	// 7	����ü�ᷮ
	{ "�ݾ�(��)",	70,	GVFM_RIGHT,	GVAT_COMMA				},	// 8	����ü��ݾ�
};


//========================================================================================================
//
//	I/O Format
//

#define	trN	"pibf2101" //2012.11.13 KSJ pibo --> pibf�� ����

struct	_inR	{
	char	func[1];		// 'S' : set
							// 'C' : clear
	char	cvol[3];		// ü�ᷮ ����   (cvol * 1,000)
	char	camt[3];		// ü��ݾ� ���� (camt * 10,000,000)
	char	ador[1];		// 'a' : and
					// 'o' : or
	char	prcf[1];		// ���� ���� ���� flag
					// 1 or 0
	char	sprc[7];		// ���ݹ��� ���۰�
	char	eprc[7];		// ���ݹ��� ����
	char	updn[5];		// ����, ���, ����, ����, �϶�
					// 1 or 0
    char    tree[15][2];        /* 00: ��ü                         */
                                /* 01: �ŷ���,      02: �ڽ���      */
                                /* 03: KP200,       04: KRX100      */
                                /* 05: ��������,    06: ETF         */
                                /* 07: ELW,         08: ELW�����ڻ� */
                                /* ������ ������ client���� ó��*/
};

#define	L_inR		sizeof(struct _inR)

struct	_outR	{
	char	retc[1];		// return code
					// 0 : OK
					// 9 : err
};
#define	L_outR		sizeof(struct _outR)

struct	_rtsR	{
	char	flag[3];		// flag (= "W__" : w + blank + blank)
	char	datl[3];		// message length (key length + data length)
};
// + datb		// real-time message
			// key(="quote") \t data(cols \t cols \t ...)
#define	L_rtsR		sizeof(struct _rtsR)
