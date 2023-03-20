#if !defined(UGINFO_H__INCLUDED_)
#define UGINFO_H__INCLUDED_

#define	R_CURR	"023"	// ���簡
#define	R_DIFF	"024"	// ���ϴ��
#define	R_GVOL	"027"	// �ŷ���
#define	R_SIGA	"029"	// �ð�
#define	R_KOGA	"030"	// ��
#define	R_JEGA	"031"	// ����
#define	R_CVOL	"032"	// ü�ᷮ
#define	R_UDYL	"033"	// �����
#define	R_CTIM	"034"	// ü��ð�
#define	R_MYGL	"201"	// �̰�������(����)

#define	BGIndex	4

enum { ctkLEFT = 0, ctkRIGHT, ctkBOTTOM};

#define	CDT_LEFT	0x01	// used in GrpWnd
#define	CDT_RIGHT	0x02	// used in GrpWnd
#define	CDT_BOTTOM	0x04	// used in GrpWnd

/*----------------------------------------------*/
/* UpLoad Data Information			*/
/*----------------------------------------------*/
struct _UpHead				/* upload header		*/
{
	char		dunit[2];	/* ����,����,�ɼ�..		*/
	char		dindex[2];	/* ��,��,��,��,ƽ		*/
	char		dcount[6];	/* data request count		*/
	char		reserve[22];	/* reserved			*/
};
#define	sz_UPHEADER	sizeof(struct _UpHead)

/*----------------------------------------------*/
/* DownLoad Data Information			*/
/*----------------------------------------------*/
struct _DnHead				/* download header		*/
{
	char		nHead[2];	/* data Head ����		*/
	char		nData[6];	/* ���� data ����		*/
	char		pday[8];	/* �ֱٿ�������			*/
	char		mgap[4];	/* �к���			*/
					/* 9xx : xx��			*/
					/* xx: xx��			*/

	char		tipName[20];	/* tooltip name			*/
	char		xtype;		/* �ð迭�� ���ڿ�����		*/
					/* default ' ' : ymdhms		*/
	char		xUnit[3][16];	/* ��/��/��� ����		*/
};
#define	sz_DNHEAD	sizeof(struct _DnHead)

struct _DataInfo			/* download Data Info		*/
{
	char		filler[2];	/* "XX"				*/
	char		name[32];	/* �̸�				*/
	char		digit[2];	/* �Ҽ��� �ڸ���		*/
	char		rtscode[12];	/* �ǽð� �ڵ�			*/
	char		symbol[5];	/* �ǽð� �ɺ�			*/
	char		rtsorg[1];	/* �ǽð� ����Ÿ�� ��ȣ�ʵ� ����*/
					/* default '1' : ��ȣ ������	*/
	char		reserve[31];	/* reserve			*/
};
#define	sz_DATAINFO	sizeof(struct _DataInfo)

#define	sz_VAL	16
struct _Data
{
	char	val[sz_VAL];		/* Data Value			*/
};
#define	sz_DATA	sizeof(struct _Data)

/*------------------------------------------------------------------------------*/
/* nData = _DnHead.nData							*/
/* nHead = _DnHead.nHead							*/
/* DownData = _DnHead + _Data[nData] + nHead * (_DataInfo + _Data[nData])	*/
/*------------------------------------------------------------------------------*/

//
//	dIndex specify one of the following values
//
#define	CDI_TICK	1		// ƽ
#define	CDI_MIN		2		// ��
#define	CDI_DAY		3		// ��
#define	CDI_WEEK	4		// ��
#define	CDI_MON		5		// ��

//
//************* client only ********/
//
struct _cgIndex
{
	struct 
	{
		short		yy;	// year
		unsigned char	mm;	// month
		unsigned char	dd;	// day
	}date;
	struct 
	{
		unsigned char	hh;	// hour
		unsigned char	mm;	// minute
		unsigned char	ss;	// second
	}time;
};
#define	sz_cgIndex	sizeof(struct _cgIndex)

struct _cgVal 
{
	union 
	{
		struct
		{
			char	str[21];	// ��ð迭�� �ϴ� ǥ�ø� 
		}name;
		struct _cgIndex year;		// �ð迭��   yymmddhhmmss
	}index;					
	double		val[1];			// �׷��� ��(�ּ� 1������ ��������)
};
#define sz_cgVAL	sizeof(struct _cgVal)


#define	CM_GRP		WM_USER + 1
#define	CM_CTRL		WM_USER + 2
//
//	each Message's LOWORD(wParam) can specify one of the following values
//

//
//	CM_GRP => GRP_
//
#define	GRP_Control	0x01
	// HIWORD(wParam) : type
	#define	GetScrData	1	// get scroll data, return data len, lParam:data reference
	#define	ChgDispPos	2	// change pos, LOWORD(lParam) : dispPos, HIWORD(lParam) : dispEnd
					// SB_LINEUP, SB_LINEDOWN
	#define	ChgDispScale	3	// change scale, lParam:scale
					// (-) : m_dispDay ����, (+) : m_dispDay ����
	#define	ChgDisplay	4	// change display, lParam:display cnt, 
					// Slide Bar
	#define	autoScroll	5	// lParam: true-set, false-reset
	#define	ChgShift	6	// lParam : shift value
	#define	ExcelData	7	//
	#define ChgParam	8	// lParam : Param info
	#define	outConfig	9	// 

//
//	CM_CTRL => CTRL_
//
#define	CTRL_Enable	0x01	// HIWORD(wParam) : click event enable, (TRUE,FALSE)
#define	CTRL_ChgScrPos	0x02	// HIWORD(wParam) : nSBCode, lParam : pos
#define	CTRL_UpdateScr	0x03	// Update Scroll
#define	CTRL_Clear	0x04	// clear ctrl

struct _MinMaxChk	
{
	int	region;		// ����
	int	coMinMax[2];	// �ִ��ּ� �������� ����
	double	Max[2];		// ���û�� �ִ밪
	double	Min[2];		// ���û�� �ּҰ�
	int	tickx[2];	// �׷��� �� X��ġ(��/��) 
	int	ticky[2];	// �׷��� �� Y��ġ(��/��) 
};

struct _gInfo			// used in client only
{
	int	gKind;		// graph kind
	int	rKey;		// region key
	int	dKey;		// data key
	int	dIndex;		// ���ֿ���ƽ
	int	mIndex;		// �ִ��ּ� ����(0,1)
	int	lIndex;		// label ����
	int	Tick;		// Tick �׸��� ����
	COLORREF	tRGB;	// foreground
	COLORREF	pRGB;	// background
	int	lwidth;
	int	lstyle;
};

////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////

struct _GrpInfo	// ��ü ��������
{
	char	tick;		// ��ü ƽ����,				����� + CT_CHAR
	char	ctrl;		// ��Ʈ�� ��뿩��,			����� + CT_CHAR
	char	option1;	// ����/������/�ִ��ּ� �׸���	����� + CT_CHAR
	char	option2;	// reserved				����� + CT_CHAR
	char	req[5];		// �׷��� ��û����
	char	show[5];	// �׷��� ��°���
	char	gcnt[2];	// �׷��� ����
	char	rcnt[2];	// ���� ����
};
#define	sz_GRPINFO	sizeof(struct _GrpInfo)

struct _GrpLine	// ���� �׷��� ����
{
	char	kind;		// �׷�������		����� + CT_CHAR
	char	region;		// ����			����� + CT_CHAR
	char	tick;		// �¿�ƽ����		����� + CT_CHAR
	char	color[10];	// �׷��� ����
	char	width;		// �׷��� ���� �β�	����� + CT_CHAR
	char	style;		// �׷��� ���� ��Ÿ��	����� + CT_CHAR
};
#define	sz_GRPLINE	sizeof(struct _GrpLine)

#define	MAX_REGION	9	// ������ 1~9 �ִ� 9��

// _GrpLine.kind
#define	GK_LINE		1	// ������Ʈ
#define	GK_2DBAR	2	// 2D Bar
#define	GK_3DBAR	3	// 3D Bar
#define	GK_2DUDBAR	4	// 2D UpDown Bar
#define	GK_3DUDBAR	5	// 3D UpDown Bar
#define	GK_2D0UDBAR	6	// 2D 0���� ���ϴ�Ī Bar
#define	GK_3D0UDBAR	7	// 3D 0���� ���ϴ�Ī Bar
#define	GK_DOT		8	// ����Ʈ

// _GrpInfo.tick/_GrpLine.tick
#define	CF_CHAR		0x30	// ����� '0'~'9'������ ���ڷ� �����ϱ� ���� ���
#define	CF_NONE		0x00	// �ȱ׸�
#define	CF_LEFT		0x01	// ����ƽ
#define	CF_RIGHT	0x02	// ����ƽ
#define	CF_BOTTOM	0x04	// �ϴ�ƽ

// _GrpInfo.option1
#define	CF_HDRAW	0x01	// ���� �׸���
#define	CF_VDRAW	0x02	// ������ �׸���
#define	CF_MINMAX	0x04	// �ִ��ּ� �׸���

// _GrpInfo.option2

#endif // !defined(UGINFO_H__INCLUDED_)
