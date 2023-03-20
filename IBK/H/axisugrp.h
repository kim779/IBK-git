#pragma once
#pragma pack(1)

#define	RS_CTIM	"034"		// ü��ð�
#define	RS_BASE	1		// �ǽð� �׽�Ʈ�� �⺻ �ɺ�
#define	RS_CODE	"SSSSSS"	// �ǽð� �׽�Ʈ�� �ڵ�
#define	RS_SYM	"001"		// �ǽð� �׽�Ʈ�� �ɺ�

enum { ctkLEFT = 0, ctkRIGHT, ctkBOTTOM};

#define	CDT_LEFT	0x01	// used in GrpWnd
#define	CDT_RIGHT	0x02	// used in GrpWnd
#define	CDT_BOTTOM	0x04	// used in GrpWnd

#define	CLR_UP		94
#define	CLR_DOWN	95

/*----------------------------------------------*/
/* UpLoad Data Information			*/
/*----------------------------------------------*/
struct _UpHead			/* upload header		*/
{
	char	dunit[2]{};	/* ����,����,�ɼ�..		*/
	char	dindex[2]{};	/* ��,��,��,��,ƽ		*/
	char	dcount[6]{};	/* ��û����			*/
	char	reserve[22]{};	/* reserved			*/
};
#define	sz_UPHEADER	sizeof(struct _UpHead)

/*----------------------------------------------*/
/* DownLoad Data Information			*/
/*----------------------------------------------*/
struct _DnHead			/* download header		*/
{
	char	nHead[2]{};	/* data Head ����		*/
	char	nData[6]{};	/* ���� data ����		*/
	char	irSize[2]{};	/* 1 index ����Ÿ ũ��		*/
	char	drSize[2]{};	/* 1 value ����Ÿ ũ��		*/
	char	pday[8]{};	/* �ֱٿ�������			*/
	char	mgap[4]{};	/* �к���			*/
				/* 9xx : xx��			*/
				/* xx: xx��			*/

	char	xtype{};		/* �ð迭�� ���ڿ�����		*/
				/* default ' ' : yyyymmddhhmmss	*/
	char	xUnit[3][16]{};	/* ��/��/��� ����		*/
	char	tipName[64]{};	/* tooltip name(��ð迭�� ���)*/
				/* tipname + \t + name \t...	*/
	char	reserve[32]{};	/* reserve			*/
};
#define	sz_DNHEAD	sizeof(struct _DnHead)

struct _DataInfo		/* download Data Info		*/
{
	char	filler[2]{};	/* "XX"				*/
	char	name[32]{};	/* �̸�				*/
	char	digit[2]{};	/* �Ҽ��� �ڸ���		*/
	char	rtscode[12]{};	/* �ǽð� �ڵ�			*/
	char	symbol[5]{};	/* �ǽð� �ɺ�			*/
	char	rtsSign[1]{};	/* �ǽð� ����Ÿ�� ��ȣ�ʵ� ����*/
				/* ' ' : ��ȣ ������ ����	*/
				/* '1' : ��ȣ('-')�� ����	*/
				/* '0' : ��ȣ��� ����		*/
	char	pivot[14]{};	/* ���ذ�			*/
	char	reserve[16]{};	/* reserve			*/
};
#define	sz_DATAINFO	sizeof(struct _DataInfo)

#define	sz_VAL	14		/* �⺻������ 14�� �ϵ� �ʿ�� �����ؼ� ����Ұ� */
struct _Data
{
	char	val[sz_VAL]{};		/* Data Value			*/
};
#define	sz_DATA	sizeof(struct _Data)

/*------------------------------------------------------------------------------*/
/* nData = _DnHead.nData							*/
/* nHead = _DnHead.nHead							*/
/* iSize = _DnHead.irSize							*/
/* dSize = _DnHead.drSize							*/
/* DownData = _DnHead + (iSize*nData) + nHead * (_DataInfo + dSize*nData)	*/
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
struct _cgNIndex
{
	struct 
	{
		short		yy{};	// year
		unsigned char	mm{};	// month
		unsigned char	dd{};	// day
	}date;
	struct 
	{
		unsigned char	hh{};	// hour
		unsigned char	mm{};	// minute
		unsigned char	ss{};	// second
	}time;
};
#define	sz_cgNIndex	sizeof(struct _cgNIndex)


//struct _cgNVal 
//{
//	union 
//	{
//		struct
//		{
//			char	str[21]{};		// ��ð迭�� �ϴ� ǥ�ø� 
//		}name;
//		struct _cgNIndex year{};		// �ð迭��   yymmddhhmmss
//	}index;					
//	double		val[1]{};			// �׷��� ��(������ ����, �ִ� 18��)
//};
//#define sz_cgNVAL	sizeof(struct _cgNVal)


struct _cgNValx
{
	std::vector<double> vData;			// �׷��� ��(������ ����, �ִ� 18��)
	union
	{
		struct
		{
			char	str[21]{};		// ��ð迭�� �ϴ� ǥ�ø� 
		}name;
		struct _cgNIndex year {};		// �ð迭��   yymmddhhmmss
	}index;
};
#define sz_cgNVALx	sizeof(struct _cgNValx)





#define	CM_GRP		WM_USER + 1
#define	CM_CTRL		WM_USER + 2
#define	CM_PREVIEW	WM_USER + 3	// request Preview data
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
#define CTRL_Focus	0x05

struct _NMinMaxChk	
{
	int	region{};		// ����(0-8)
	int	coMinMax[2]{};	// �ִ��ּ� �������� ����(��/��)
	double	Max[2]{};		// ���û�� �ִ밪(��/��)
	double	Min[2]{};		// ���û�� �ּҰ�(��/��)
	int	tickx[2]{};	// �׷��� �� X��ġ(��/��) 
	int	ticky[2]{};	// �׷��� �� Y��ġ(��/��) 
};
#define	sz_NMINMAXCHK	sizeof(_NMinMaxChk)

struct _gNInfo
{
	int	gKind{};		// graph kind
	int	rKey{};		// region key(0-8)
	int	dKey{};		// data key(0-17)
	int	dIndex{};		// ���ֿ���ƽ
	int	mIndex{};		// MinMax ����(0,1)
	int	lIndex{};		// label ����
	int	Tick{};		// Tick �׸��� ����
	COLORREF	tRGB{};	// foreground
	COLORREF	pRGB{};	// background
	int	lwidth{};
	int	lstyle{};
};

////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////

struct _GrpInfo	// ��ü ��������
{
	char	tick{};		// ��ü ƽ����,				����� + CT_CHAR
	char	ctrl{};		// ��Ʈ�� ��뿩��,			����� + CT_CHAR
	char	option1{};	// ����/������/�ִ��ּ� �׸���	����� + CT_CHAR
	char	option2{};	// �̸�����/����/�����ϴ�ƽ		����� + CT_CHAR
	char	req[5]{};		// �׷��� ��û����
	char	show[5]{};	// �׷��� ��°���
	char	gcnt[2]{};	// �׷��� ����(�ִ� 18)
	char	rcnt[2]{};	// ���� ����(�ִ� 9)
	char	rinfo[18]{};	// ���� ��������(2byte * �ִ� 9��)
	char	index[1]{};	// ����-���ֿ���ƽ
	char	option3{};	// ���� �ǽð�				����� + CT_CHAR
	char	res[8]{};		// reserve
};
#define	sz_GRPINFO	sizeof(struct _GrpInfo)

struct _GrpLine	// ���� �׷��� ����
{
	char	kind[2]{};	// �׷�������
	char	region{};		// ����(1-9)		����� + CT_CHAR
	char	tick{};		// �¿�ƽ����		����� + CT_CHAR
	char	color[10]{};	// �׷��� ����
	char	width{};		// �׷��� ���� �β�	����� + CT_CHAR
	char	style{};		// �׷��� ���� ��Ÿ��	����� + CT_CHAR
	char	option1{};	// option1
	char	option2{};	// option2
	char	res[4]{};		// reserve
};
#define	sz_GRPLINE	sizeof(struct _GrpLine)

#define	MAX_REGION	9	// �ִ� 9��
#define	MAX_GRAPH	MAX_REGION*2

// _GrpLine.kind
#define	IGK_LINE	1	// �� ��Ʈ
#define	IGK_2DVBAR	2	// 2D ��������
#define	IGK_3DVBAR	3	// 3D ��������
#define	IGK_2DUDBAR	4	// 2D ���� ��������
#define	IGK_3DUDBAR	5	// 3D ���� ��������
#define	IGK_2D0UDBAR	6	// 2D 0��Ī ���� ��������
#define	IGK_3D0UDBAR	7	// 3D 0��Ī ���� ��������
#define	IGK_DOT		8	// �� ��Ʈ
#define	IGK_YBASELINE	9	// y���� ��� ����Ʈ(pivot�� y�ప�� ���Ͽ� ����/�Ķ� �� ǥ��)
#define	IGK_XBASEVBAR	10	// x���� ��� ��������(pivot�� x�ప�� ���Ͽ� ����/�Ķ� Bar ǥ��)
#define	IGK_2D3NBAR	11	// 2D 3N ���򸷴� => �ż�/�ŵ�/���� ü�ῡ ���� �Ź�����Ʈ
#define	IGK_2D1NBAR	12	// 2D 1N ���򸷴�

// _GrpInfo.tick/_GrpLine.tick
#define	ICF_CHAR	0x30	// ����� '0'~'9'������ ���ڷ� �����ϱ� ���� ���
#define	ICF_NONE	0x00	// 
#define	ICF_LEFT	0x01	// ����ƽ �׸���
#define	ICF_RIGHT	0x02	// ����ƽ �׸���
#define	ICF_BOTTOM	0x04	// �ϴ�ƽ �׸���

// _GrpInfo.option1
#define	ICF_HDRAW	0x01	// ���� �׸���
#define	ICF_VDRAW	0x02	// ������ �׸���
#define	ICF_MINMAX	0x04	// �ִ��ּҰ� ǥ��

// _GrpInfo.option2
#define	ICF_PREVIEW	0x01	// �̸�����
#define	ICF_REVDRAW	0x02	// �������� �׸��� 
#define	ICF_TICKVERTIC	0x04	// �ϴ�ƽ �������� �׸���

// GrpInfo.option3
#define	ICF_ITEMRTS	0x01	// �׸� �ǽð�

// _GrpLine.option1
#define	IGO_NAMER	0x01	// �̸��� ������ ǥ��
#define	IGO_MINMAX	0x02	// ���� ���� ��Ʈ�� ���� �ִ��ּҰ� �����ϰ� ����

struct _TestInfo	// test data info for Preview
{
	int	gcount{};		// grount count
	int	gKind[32]{};	// graph kind
	bool	bVerticStr{};	// vertical tick
	bool	bItemRts{};	// �׸� �ǽð� ����
};

#pragma pack()