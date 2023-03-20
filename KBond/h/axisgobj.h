#if !defined(AXIS_GRAPH_OBJECT_H)
#define AXIS_GRAPH_OBJECT_H

#define	GD_UNUSED		-999999999
#define	DKEY_ORGDATA	1	// received data key range : 1 ~ 20
#define	DKEY_APPDATA	20	// application data key range : 20 ~ 

enum { cPRC = 0, cLIN, cVOL, cAMT};
enum { dpNONE = 0, dpBASIC = 1, dpTICK = 2, dpVIDX = 3};

enum {	toolNO = -1,
	toolHIDE, toolMOUSE, toolHISTORY, toolDELETE, toolINPUT,
	toolWIDTH, toolPALETTE, toolTEND, toolTENDEX, toolHLINE,
	toolARC, toolUARROW, toolGIGAN, toolDAEDEUNG, toolFIBOLINE,
	toolGANNFAN, toolTRILINE, toolVLINE, toolCROSS, toolTRI,
	toolRECT, toolDARROW, toolLARROW, toolRARROW, toolFIBOFAN,
	toolFIBOFAN2, toolFIBOARC, toolFIBORET, toolRGANNFAN, toolGANNLINE,
	toolQUARDLINE, toolACCELFAN, toolACCELARC, toolDAEDEUNGCFG, toolGIGAN2, 
	toolTRILINELEVEL, toolAndrewPf, toolPatternUP, toolPatternDN};

enum { popLINE = 40, popFIGURE, popARROW, popGIGAN, popDAEDEUNG, popFIBO, popGANN, popETC};
enum { toolVIEW = 50, toolSETUP, toolNUM};

struct _gIndex{
	struct {
		short		yy;	// year
		unsigned char	mm;	// month
		unsigned char	dd;	// day
	}date;
	struct {
		unsigned char	hh;	// hour
		unsigned char	mm;	// minute
		unsigned char	ss;	// second
	}time;
};
#define	sz_gIndex	sizeof(struct _gIndex)
/*
struct	_gBasic {
	int		fpr;			// �ð�
	int		hpr;			// ��
	int		lpr;			// ����
	int		epr;			// ����
	double		trn;			// �ŷ���
	double		amt;			// �ŷ����
	struct _gIndex	index;

	unsigned char	check;			// information check
	WORD		rate;			// check rate
	int		mgjy;			// �̰�������
//	WORD		ucnt;			// ��������
//	WORD		dcnt;			// �϶������


	//**WORD		fobtain;		// �ܱ��� ������
	//**WORD		fexhaust;		// �ܱ��� ������
	//**int		fmmvol;			// �ܱ��� ���ż���
	//** 2005.11.08 add by macho
	//int     fmmamt;			// �ܱ��� ���ż��ݾ�
	//int     orgvol;			// ����� ���ż���
	//int		orgamt;			// ����� ���ż��ݾ�
	//int		invdata[6];

	int	orgnvol;
	int orgnamt;
	int frgnvol;
	int frgnamt;
	int fobtain;
	int fexhaust;
};
#define	sz_gBASIC	sizeof(struct _gBasic)
*/
struct	_gTick {
	int		epr;			// ����
	double		trn;			// �ŷ���
	struct _gIndex	index;
};
#define	sz_gTICK	sizeof(struct _gTick)

struct _CoDraw
{
	int	region;
	DWORD	coWork;		// 2006.10.11 schbang	//BYTE	coWork;
	double	PMax;
	double	PMin;
	double	VMax;
	double	VMin;
	double	AMax;
	double	AMin;
	int	tickx;
	int	ticky;
	double	dMacdMax;	// 2006.10.11 schbang
	double	dMacdMin;	// 2006.10.11 schbang
};
#define	sz_CODRAW	sizeof(struct _CoDraw)

//
//	_CoDraw.coWork can be any combination of the following values.
//
#define	MC_PRC		0x01	// ���� ��Ʈ
#define	MC_VOL		0x02	// �ŷ��� ��Ʈ
#define	MC_AMT		0x04	// �ŷ���� ��Ʈ
//#define	MC_USR		0x08	// User ��Ʈ
#define	MC_MACD		0x08	// MACD		2006.10.11 schbang

/** remarking by macho
#define	FOBRATE		0x01	// GK_FOBRATE
#define	FEXRATE		0x02	// GK_FEXRATE
#define	FRGNVOL		0x04	// GK_FRGNVOL
//** add 2005.11.07 by macho
#define FRGNAMT		0x08	// GK_FRGNAMT
#define ORGNVOL		0x10	// GK_ORGNVOL
#define ORGNAMT		0x20	// GK_ORGNAMT
**/

//** add 2006.12.12 by macho-

#define FRGNHAVE	0x01		//** �ܱ��κ�����Ȳ
#define FRGNRATE	0x02		//** �ܱ��κ�������
#define PSNLPBUY	0x04		//** ���μ��ż���
#define FRGNPBUY	0x08		//** �ܱ��μ��ż���
#define ORGNPBUY	0x10		//** ������ż���
#define PSNLTBUY	0x20		//** ���δ������ż���
#define FRGNTBUY	0x40		//** �ܱ��δ������ż���
#define ORGNTBUY	0x80		//** ����������ż���


struct _toolConfig {
	CImageList*	pImgLst;
	char*		pcBuf;
	int		iCount;
	char*		pcBufResult;
	int		iCountResult;
};

struct _aidConfig {
	char*		pcBuf;
	int		iCount;
	char*		pcBufResult;
	int		iCountResult;
};

#endif
