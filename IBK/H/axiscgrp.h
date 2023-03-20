#pragma once

#define IMG_ZOOMOUT	0
#define IMG_ZOOMIN	3
#define IMG_PRE		9
#define IMG_STOP	12
#define IMG_NEXT	15
#define IMG_EXCEL	18

enum {ctl_NO = 0, ctl_ZOOMOUT, ctl_ZOOMIN, ctl_PRE, ctl_STOP, ctl_NEXT, ctl_EXCEL};

#define	R_CURR	"023"	// ���簡
#define	R_DIFF	"024"	// ���ϴ��
#define	R_GVOL	"027"	// �ŷ���
#define	R_SIGA	"029"	// �ð�
#define	R_KOGA	"030"	// ��
#define	R_JEGA	"031"	// ����
#define	R_CVOL	"032"	// ü�ᷮ
#define	R_UDYL	"033"	// �����
#define	R_CTIM	"034"	// ü��ð�

//#define	ER_CURR	"723"	// ���簡
#define	ER_DIFF	"724"	// ���ϴ��
#define	ER_GVOL	"727"	// �ŷ���
#define	ER_SIGA	"729"	// �ð�
#define	ER_KOGA	"730"	// ��
#define	ER_JEGA	"731"	// ����
#define	ER_CVOL	"732"	// ü�ᷮ
#define	ER_UDYL	"733"	// �����
#define	ER_CTIM	"734"	// ü��ð�

#define	BGIndex	68
#define	CLR_UP		94
#define	CLR_DOWN	95

enum { ctkLEFT, ctkRIGHT, ctkBOTTOM};

#define	CDT_LEFT	1
#define	CDT_RIGHT	2

#pragma pack(1)
struct _cdHeader	// upload header
{
	char		dunit[2]{};	// ����,����,�ɼ�..
	char		dindex[2]{};	// ��,��,��,��,ƽ
	char		dkind[2]{};	// data struture
	char		dcount[6]{};	// data request count
	char		symbol[6]{};	// rts symbol
	char		reserver[10]{};	// reserved
};

struct _cdJuga		// download header
{
	char		nData[6]{};	// data ����
	char		digit[2]{};	// �ڸ���
	char		pday[8]{};	// �ֱٿ�������
	char		mgap[4]{};	// �к���
					// 9xx : xx��
					// xx: xx��
	char		pjgga[7]{};	// ���ϰ�
	char		ujgga[7]{};	// ���Ѱ�
	char		djgga[7]{};	// ���Ѱ�
};
#define	sz_CDJUGA	sizeof(struct _cdJuga)

/************* Ư����Ʈ Start *********/
struct _cdMultiline/* Multi Line CHAT*/
{
	char	nData[6]{};		/* data ����*/
	char	pday[8]{};		/* �ֱٿ�������*/
	char	mgap[4]{};		/* �к���*/
					/* ��:X9xx �Ϻ�: 0000*/
	struct {			/* ����(��,��)*/
		char    digit[2]{};	/* �ڸ���*/
		char    lcnt[2]{};	/* �ش翵���� ���ΰ���*/
		char    rcod[5][6]{};	/* �������� RTS symbol*/
					/* �󿵿��� ' '�� ä���*/
	} region[2];			/* ���Ͽ��� ����*/
};
#define sz_CDMultiL     sizeof(struct _cdMultiline)

struct _cdMulticode
{
	char	nData[6]{};		// data ����
	char	pday[8]{};		// �ֱٿ�������
	char	mgap[4]{};		// �к���
					// 9xx : xx��
					// xx: xx��
	char	lcnt[2]{};		// Line cnt
	char	rgn[5][1]{};		// ��ġ ����
	char	name[5][16]{};		// ��Ʈ �̸�
	char	rcod[5][12]{};		// RTS Code
	char	rsym[5][6]{};		// RTS Symbol 
	char	digit[5][2]{};		// �Ҽ��� �ڸ���
	char	jgga[5][6]{};		// ��������
	char	ctype[5][1]{};		// ��Ʈ ����(Line:'1', Bong:'B')
	char	reserve[11]{};		// 
};
#define	sz_CDMULTICODE	sizeof(struct _cdMulticode)

/************* Ư����Ʈ End *********/

//
//	gKind specify one of the following values
//
#define	CGK_LINE	1
#define	CGK_BONG	2
#define	CGK_BAR		3
#define	CGK_VOL		4
#define	CGK_PMA		5
#define	CGK_VMA		6
#define	CGK_AMT		7

//
//	dIndex specify one of the following values
//
#define	CDI_TICK	1
#define	CDI_MIN		2
#define	CDI_DAY		3
#define	CDI_WEEK	4
#define	CDI_MON		5
#define	CDI_TICK2	6

//
//	dKind specify one of the following values
//
#define	CDK_TICK	1
#define	CDK_BONG	2

#define	CDK_VAL		3
#define	CDK_VAL2	4
#define	CDK_VIDX	5
#define	CDK_NMVL	6
#define	CDK_VAL3	7

//
//	dUnit specify one of the following values
//
#define	CDU_JONGMOK	1
#define	CDU_UPJONG	2
#define	CDU_FUTURE	3
#define	CDU_OPTION	4
#define	CDU_ECN		5
#define	CDU_FOREIGN	8		// �ؿܽü�
//
//	_gInfo.dKind == CDK_TICK
//
struct _cdTick
{
	char		epr[7]{};		// ü�ᰡ
	char		date[6]{};	// ü��ð� [hh/mm/ss]
};
#define	sz_cdTick	sizeof(struct _cdTick)

//
//	_gInfo.dKind == CDK_TICK2
//
struct _cdTick2
{
	char		epr[7]{};		// ü�ᰡ
	char		trn[8]{};		// �ŷ���
	char		date[6]{};	// ü��ð� [hh/mm/ss]
};
#define	sz_cdTick2	sizeof(struct _cdTick2)

//
//	_gInfo.dKind == CDK_BONG
//

struct _cdBong
{
	char		fpr[7]{};		// �ð�
	char		hpr[7]{};		// ��
	char		lpr[7]{};		// ����
	char		epr[7]{};		// ����
	char		trn[10]{};	// �ŷ���
	char		date[14]{};	// ü��ð� [yyyy/mm/dd/hh/mm/ss]
};
#define	sz_cdBong	sizeof(struct _cdBong)

//
//	_gInfo.dKind == CDK_VAL
//
struct	_cdVal {
	char		val[10]{};
};
#define	sz_cdVal	sizeof(struct _cdVal)

//
//	_gInfo.dKind == CDK_VAL2
//
struct	_cdVal2 {
	char		date[14]{};	// [yyyy/mm/dd/hh/mm/ss]
	char		val1[10]{};
	char		val2[10]{};	
};
#define	sz_cdVal2	sizeof(struct _cdVal2)

//
//	_gInfo.dKind == CDK_VAL3
//
struct	_cdVal3 {
	char		date[14]{};	// [yyyy/mm/dd/hh/mm/ss]
	char		val1[10]{};
	char		val2[10]{};
	char		val3[10]{};
};
#define	sz_cdVal3	sizeof(struct _cdVal3)

//
//	_gInfo.dKind == CDK_VIDX
//
struct	_cdVidx {
	char		date[14]{};		// ����
	char		val[10]{};		// value
};
#define	sz_cdVidx	sizeof(struct _cdVidx)

//
//	_gInfo.dKind == CDK_NMVL
//
struct	_cdNameVal {
	char		name[20]{};
	unsigned char	color[4]{};
	char		val[10]{};
};
#define	sz_cdNMVL	sizeof(struct _cdNameVal)

struct	_cdValue2 { 
	char		yval[10]{}; 
	char		xval[10]{}; 
	char		date[14]{};	// [yyyy/mm/dd/hh/mm/ss] 
}; 
#define	sz_cdValue2	sizeof(struct _cdValue2) 

//
//************* client only Start *********/
//
struct _cgIndex{
	struct {
		short		yy{};	// year
		unsigned char	mm{};	// month
		unsigned char	dd{};	// day
	}date;
	struct {
		unsigned char	hh{};	// hour
		unsigned char	mm{};	// minute
		unsigned char	ss{};	// second
	}time;
};
#define	sz_cgIndex	sizeof(struct _cgIndex)

struct _cgTime{
	unsigned char	hh{};	// hour
	unsigned char	mm{};	// minute
	unsigned char	ss{};	// second
};
#define	sz_cgTime	sizeof(struct _cgTime)

struct _cgTick
{
	struct _cgTime	time{};	// ü��ð�
	int		epr{};	// ü�ᰡ
	int		diff{};	// ���
	float		udyl{};	// �����
};
#define	sz_cgTick	sizeof(struct _cgTick)

struct _cgTick2
{
	struct _cgTime	time{};	// ü��ð�
	int		epr{};	// ü�ᰡ
	int		trn{};	// �ŷ���
	int		diff{};	// ���
	float		udyl{};	// �����
};
#define	sz_cgTick2	sizeof(struct _cgTick2)

struct _cgTickEx
{
	struct _cgTime	time{};	// ü��ð�
	int		epr{};	// ü�ᰡ
	int		trn{};	// �ŷ���
};
#define	sz_cgTickEx	sizeof(struct _cgTickEx)

struct _cgBong
{
	struct _cgIndex	index{};		// �ð�/����
	int		fpr{};		// �ð�
	int		hpr{};		// ��
	int		lpr{};		// ����
	int		epr{};		// ����
	double		trn{};		// �ŷ���
};
#define	sz_cgBong	sizeof(struct _cgBong)

struct _cgVal {	
	double		val{};
};
#define	sz_cgVAL		sizeof(struct _cgVal)

struct _cgVal2 {
	struct _cgIndex	index{};
	double		val[2]{};
};
#define sz_cgVAL2	sizeof(struct _cgVal2)

struct _cgSVal2 {
	char		name[14]{};
	double		val[2]{};
};
#define sz_cgSVAL2	sizeof(struct _cgSVal2)

struct _cgVal3 {
	struct _cgIndex	index{};
	double		val[3]{};
};
#define sz_cgVAL3	sizeof(struct _cgVal3)

struct _cgVal4 {
	struct _cgIndex	index{};
	double		val[4]{};
};
#define sz_cgVAL4	sizeof(struct _cgVal4)

struct _cgVidx {
	double		val{};
	struct _cgIndex	index{};
};
#define	sz_cgVIDX	sizeof(struct _cgVidx)

struct _cgNameval {
	char		name[20]{};	// 
	DWORD		color{};
	double		val{};
};
#define	sz_cgNAMEVAL	sizeof(struct _cgNameval)



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
#define CTRL_Focus 0x05 // focus

struct _MinMaxChk	
{
	int	region{};
	int	coMinMax{};
	double	Max{};
	double	Min{};
	int	tickx{};
	int	ticky{};
};

struct _gInfo		// used in client only
{
	int	gKind{};
	int	rKey{};
	int	dKey{};
	int	dIndex{};
	int	dKind{};
	COLORREF	tRGB{};
	COLORREF	pRGB{};
};
/************* client only End *********/

#pragma pack()