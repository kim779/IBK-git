#ifndef	_LIB_DEFINE_
#define	_LIB_DEFINE_

#define	TM_EVENT	1000		// �ǽð� �ܰ�ü�� ID
#define	TM_GAP		1		// �ǽð� �ܰ�ü�� timer

#define	KEY_FREMAIN	1		// �ܰ���ȸ
#define	KEY_CONT	2		// ��ü����ȸ
#define	KEY_MINGAM	3		// �̰�����ȸ
#define	KEY_FCURR	4		// ���簡��ȸ


#define RT_JCODE	"907"		// �����ڵ�
#define RT_JNAME	"908"		// �����
#define RT_GUBN		"912"		// �ֹ�����
#define RT_JPRICE	"910"		// �ֹ���
#define RT_JAMT		"909"		// �ֹ�����
#define RT_CPRICE	"916"		// ü�ᰡ
#define RT_CAMT		"931"		// ü�����
#define RT_STATE	"988"		// ����
#define	RT_MCHG		"921"		// ��ü�����

#define	RT_CCURR	"023"		// ü�����簡
#define	RT_CDIFF	"024"		// ü����
#define	RT_CYYDT	"370"		// ü�῵���ϼ�
#define	RT_CDDDT	"314"		// ü��޷��ϼ�


enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR};

class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};
enum	{ SM_NOT = -1, SM_ALL = 0, SM_UPDATE, SM_DELETE, SM_INSERT, SM_ERROR  }; //  sendToMap
enum	{ FPOS_NOT = 0, FPOS_BULL, FPOS_BEAR };	// ��ü, ����, �༼

CString _GetString(char *pChar, int nMinLen);
CString _GetFormatData(CString src);
CString	_Parser(CString &srcstr, CString substr);
/*******************************************************************************
 * ����/�ɼ� �ܰ��� ��ȸ IO FORMAT
 ******************************************************************************/
struct	f_mid	{		// ���� Input
	char	accn[8];
	char	cogb[2];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* �����ڵ�		*/
	char	hnam[30];	/* �����		*/
	char	dsgb[15];	/* ����			*/
	
	char	bqty[10];	/* ����			*/
	char	cqty[10];	/* �ŵ����ɼ���		*/
	char	pprc[10];	/* ��հ�/���갡	*/
	char	curr[10];	/* ���簡		*/
	char	diff[10];	/* ��հ� ���		*/
	char	pamt[15];	/* �򰡱ݾ�		*/
	char	psam[15];	/* �򰡼���		*/
	char	prat[15];	/* ���ͷ�		*/
	char	tamt[15];	/* ���Աݾ�		*/
};

struct	f_mod	{
	char	accn[10];
	char	nrec[4];
	struct	f_rec	rec[1];
};

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid


/*******************************************************************************
 * ����/�ɼ� ��ü�� ��ȸ IO FORMAT
 ******************************************************************************/
struct f_cmid {
	char	accn[8];
	char	cogb[2];
	char	pswd[8];
	char	gubn[1];	// 0.All 1.ü�� 2.��ü��
	char	juno[6];
	char	sort[1];	// 1.���� 2.����
	char	date[8];
	char	cod2[8];
};

struct f_crec {
	char	mono[6];	/* ���ֹ���ȣ		*/
	char	juno[6];	/* �ֹ�  ��ȣ		*/
	char	ojno[6];	/* ���ֹ� ��ȣ		*/
	char	cod2[8];	/* �����ڵ�		*/
	char	hnam[30];	/* �����		*/
	char	odgb[8];	/* �ֹ�����		*/
	char	hogb[1];	/* �ֹ�����		*/
	char	jprc[11];	/* �ֹ�����		*/
	char	jqty[7];	/* �ֹ�����		*/
	char	dlgb[4];	/* ü�ᱸ��		*/
	char	dprc[11];	/* ü�ᰡ��		*/
	char	dqty[7];	/* ü�����		*/
	char	wqty[7];	/* ��ü�����		*/
	char	stat[8];	/* ��������		*/
};

struct	f_cmod {
	char	nrec[4];
	struct	f_crec rec[1];
};

#define L_fcmid	sizeof(struct f_cmid)				// Length of Future input
#define L_fcmod	sizeof(struct f_cmod) - sizeof(struct f_crec)	// Length of Future output
#define L_fcrec	sizeof(struct f_crec)				// Length of Future Grid

/*******************************************************************************
 * PIBO3101 : �ΰ���
 ******************************************************************************/
struct  _mmid
{
	char	crec[3];	// �ڵ尳��
	char	codes[50][8];	// �����ڵ�(�ִ� 50��) ���� or �ɼ�
};
#define	L_MMID	sizeof (struct _mmid)

struct  _mgmod
{
	char	ccod[8];	// �����ڵ�
	char	ccur[8];	// ���簡
	char	cdif[8];	// ���
        char    shga[8];	// ���Ѱ�
        char    hhga[8];	// ���Ѱ�
        char    mdga[5][8];	// �ŵ�ȣ��1~5
        char    msga[5][8];	// �ż�ȣ��1~5
	char	cgvol[12];	// �ŷ���
	char	jjil[2][3];	// �����ϼ� (�ŷ��ϼ�/�޷��ϼ�)
	char	bdjs[5];	// �������
};
#define L_MGMOD	sizeof (struct _mgmod)

struct  _mmod
{
	char	frts[8];	// �ֱٿ��� ����
	char	fnam[20];
	char	fcur[8];
	char	krts[8];	// KOSPI
	char	kcur[8];
	char	k200rts[8];	// KOSPI 200
	char	k200cur[8];
	char	k200tim[8];	// KOSPI 200 Time
	char	hv[4][8];	// hv (90/60/30/21)
	char	cdgl[5];	// CD �ݸ�
	//char	bdjs[5];	// �������(
	char	grec[4];	// �ڵ尳��( count of gmod )
//	struct  _gmod    gmod[50];  // MAX_R = 50
};
#define	L_MMOD	sizeof (struct _mmod)

#endif	// _LIB_DEFINE_