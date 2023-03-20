#pragma once
#define MSG_ACCOUNTCHANGE	10	// CMapWnd
#define	MSG_NOTIFY		20	// CMapWnd
#define	MSG_DEPOSIT		40	// ���ű�->CMapWnd
#define	MSG_SELECTSTG		30	// CStgBoard

#define KEY_ACCOUNT		99
#define KEY_CODEMINGAM		1
#define KEY_CODEMINGAM2		2
#define	KEY_INITVALUE		3
#define	KEY_UPJONG		4
#define	KEY_DEPOSIT		5
#define	KEY_DEPGUESS		6
#define	KEY_HEDGEOPTION		10
#define KEY_JJGA	11

//***************************************************************
#define	WM_MAPAddSymbol		WM_USER + 3
#define	WM_MAPGraph			WM_USER + 4
	#define	GrpReset		1
	#define	GrpChange		2
	#define	SetSumDelta		3
	#define	SetSumGamma		4
	#define	SetSumSeta		5
	#define	SetSumVega		6
	#define	SetK200			7
	#define	SetYetak		8
#define	WM_MAPStrategy		WM_USER + 5
	#define	ApplyStdStg		0	// apply standard strategy
	#define	ApplyUsrStg		1	// apply user strategy
	#define	SaveStg			2	// save to server
	#define	LoadStg			3	// load from server
#define	WM_MAPHedgeData		WM_USER + 6
	#define	IsHedgeable		1
	#define	GetHedgeTarget		2
	#define	GetHedgeData		3
	#define	ReqOrder		4
#define	WM_MAPRefresh		WM_USER + 7
	#define	UpdateUserList	1
	#define	ResetHoga		2	// reset HogaWind
	#define	UpdateHoga		3	// update hogawnd
#define	WM_MAPLineMove		WM_USER + 9
	#define	LineMovePoint	1
	#define	LineMoveAll		2
#define	WM_MAPUser			WM_USER + 10
	#define	RequestOrder	0
	#define	RequestBalance	1
	#define	RequestIV		2
	#define	RequestMargin	3
	#define	RequestMoney	4
	#define	SetJKey			5
	#define	RequestGuide	6
#define	WM_MAPFileEnd		WM_USER + 11	// end of save/load
	#define	EndUpLoad		1
	#define	EndDnLoad		2
#define	WM_MAPAccnInfo		WM_USER + 12
	//#define	AccnChg			1	// account change event(ProWnd -> ProPage2_3)
	#define	AccnOrg			2	// accn + pswd
	#define	AccnEnc			3	// accn + pswd(encryption)
#define	WM_MAPGetProfile	WM_USER + 13
#define WM_MAPMouseUp		WM_USER + 14
#define	WM_MAPStgApply		WM_USER + 15
#define	WM_MAPStgAdd		WM_USER + 16

// profile message
#define WM_PROPrice			WM_USER + 21
#define WM_PROSave			WM_USER + 22
#define WM_PROStgCode		WM_USER + 23
#define WM_PROStgApplyData	WM_USER + 24
#define WM_PROStgAddData	WM_USER + 25
#define	WM_PROBlcApplyData	WM_USER + 26
#define WM_PROAddSymbol		WM_USER + 27
#define WM_PROUser			WM_USER + 28
	#define	EnableOdrBtn	1
	#define	DestroyOdrDlg	2
	#define	ResetBtn		3
	#define	GetGridCode		4
#define	WM_PROAccnInfo		WM_USER + 29
	#define	ChangeAccn		1	// account change event(AccountCtrl -> ProWnd)
	#define	GetOrgAccn		2	// accn + pswd
	#define	GetEncAccn		3	// accn + pswd(encryption)
#define	WM_PROGetHedgeInfo	WM_USER + 30

// position analysis message
#define	WM_POSMSVAL			WM_USER + 41
#define	WM_POSGetHV			WM_USER + 42
#define	WM_POSUser			WM_USER + 43
	#define	SetIVTR			1
#define	WM_POSGetFee		WM_USER + 44
#define	WM_POSInitData		WM_USER + 45
#define	WM_POSInitParam		WM_USER + 46
#define	WM_POSDEPOSITSEL	WM_USER + 47
#define	WM_POSCLEAR		WM_USER + 48


// position graph message
#define	WM_GRPRefresh		WM_USER + 51
	#define	DeleteGrpData	0
	#define	RedrawGraph		1
	#define	SetPayOff		2
	#define	ResetSensitive	3
	#define	SetSensitive	4
	#define SetStgMouseMove	5
#define	WM_GRPAlert		WM_USER + 52
#define	WM_GRPAddData		WM_USER + 53
#define	WM_GRPSetParam		WM_USER + 54
	#define GetJisuStart	0
	#define	GetJisuEnd	1
	#define SetJisuStart	2
	#define	SetJisuEnd	3
	#define	SetJjilMove	4
	//#define	SetCallPutVolMove	5
#define	WM_GRPBasicPrc		WM_USER + 55

// left sheet message
#define	WM_SHTApplyStdStg	WM_USER + 61
#define	WM_SHTUser			WM_USER + 62
	#define	SetStgIndex		0
	#define	GetStgIndex		1

// sheet - propertypage1 message
#define	WM_SP11ApplyStdStg	WM_USER + 91
#define	WM_SP11GrpMov		WM_USER + 92

// sheet - propertypage03 message
#define	WM_SP13User			WM_USER + 93

// sheet - propertypage2_2 message
#define	WM_SP22Data			WM_USER + 94


// strategy 
#define WM_STGInitData		WM_USER + 71


#define INFO_MAX 35		//2014.04.16 KSJ 33 -> 35�� ����

// initial - Info:Information, Stg:Strategy, Usr:User, Odr:Order
#define	TR_InfoInit		0	// �ʱ�����
#define	TR_InfoHedge	1	// hedge data
#define	TR_InfoUpDown	2	// ��������� ������ ����/�ε�
#define TR_InfoIV		3	// ��ǥiv, call iv, put iv
#define TR_StgApply		4	// ��������
#define TR_StgAdd		5	// �����߰�
#define	TR_UsrApply		6	// �������������
#define	TR_UsrAdd		7	// ����������߰�
#define	TR_UsrMoveApply	8	// Graph LineMove
#define	TR_OdrAccn		11	// �ֹ�����(������Ʈ��)
#define TR_OdrOrder		12	// �ֹ��Ÿ�(�ֹ���ư)
#define TR_OdrClear		13	// �ֹ�û��(û���ư)
#define	TR_OdrHedge		14	// �ֹ�����
#define TR_OdrBalance	15	// �ֹ��ܰ�(�ܰ��ư)
#define	TR_OdrBlcStg	16	// �ֹ��ܰ�����(�ܰ��ư)
#define TR_OdrBlcMap	17	// �ܰ��û(�����ܰ�)
#define	TR_OdrMargin	18	// �ɼǸŵ����ű�
#define	TR_OdrMoney		19	// �ֹ����ɱݾ�
#define SYMBOL_KEY		20	// �����߰�


#define	TM_LOADMAP		2001
#define	TM_MGAP			10
#define	BASE_MID		150

// 20071214
#define	FILE3101		"IB410100.ini"		// ��������
#define	RANGESECTION		"RANGE"			// section
#define	STARTRANGE		"STARTRANGE"		// start
#define	ENDRANGE		"ENDRANGE"		// end
#define	CHKRANGE		"CHKRANGE"		// range��뿩��

// 20121008
#define MULTICODE	"MULTICODE"		//��Ƽ����â

#define	STRATEGY_LIST	"Strategy.ini"	// ������������ �����������
#define	STRATEGY_FILE	"Strategy.i"	// �������������̸�(�ڿ� 00, 01...�� ����)
#define	SN_NAME			"NAME"			// name section
#define	SN_CODE			"CODE"			// code section
#define	SN_PROJ			"PROJ"			// project name key
#define	FOSYMBOL		"focod"			// ����/�ɼ� �ɺ�
#define	cTAB			'\t'			// tab
#define	cDEL			0x7f			// delimiter

enum { SONIK = 0, DELTA, GAMMA, CETA, VEGA, IRON};
//***************************************************************		

// OOP
#define	TRN_OOP		"POOPPOOP"

// PIBO4101
#define	TRN_MINGAM	"PIBO4101"
#define MAX_CODE	100		//2013.07.24 KSJ
#define DBASE		250000 // 2017.03.06 �ŷ��¼�

struct  _mid {
	char	crec[3];	// �ڵ尳��
	char	codes[MAX_CODE][8];	// �����ڵ�(�ִ� 50��) ���� or �ɼ�
};
#define	L_MID	sizeof (struct _mid)

struct  _gmod {
	char	ccod[9];	// �����ڵ�	//2012.04.06 KSJ 8�ڸ����� 9�ڸ��� ������.
	char	ccur[8];	// ���簡
	char	cdif[8];	// ���
        char    shga[8];	// ���Ѱ�
        char    hhga[8];	// ���Ѱ�
        char    mdga[5][8];	// �ŵ�ȣ��1~5
        char    msga[5][8];	// �ż�ȣ��1~5
	char	cgvol[12];	// �ŷ���
	char	jjil[2][3];	// �����ϼ� (�ŷ��ϼ�/�޷��ϼ�)
	char	bdjs[5];	// �������(20060323�߰�)
	char	gdan[9];	// �ŷ��¼�(2012.05.17 �߰���) ���� 500000, �ɼ� 6���� ��쿡 ���� 100000 �̰ų� 5000000�̾ �߰���.
};
#define L_GMOD	sizeof (struct _gmod)

struct  _mod {
	char	frts[8];	// �ֱٿ��� ����
	char	fnam[20];
	char	fcur[8];
	char	krts[8];	// KOSPI
	char	kcur[8];
	char	k200rts[8];	// KOSPI 200
	char	k200cur[8];
	char	k200tim[8];	// KOSPI 200 Time(20070319�߰�)
	char	hv[4][8];	// hv (90/60/30/21)
	char	cdgl[5];	// CD �ݸ�
	//char	bdjs[5];	// �������(20060323����)
	char	grec[4];	// �ڵ尳��( count of gmod )
	struct  _gmod    gmod[MAX_CODE];  // MAX_R = 50
};
#define	L_MOD	sizeof (struct _mod)
//-----------------------------------

struct _GraphData	{
	char		code[9];// �����ڵ� ����
	double		hsga;	// ��簡
	BYTE		gubn;	// �����ڵ� ������
	double		numb;	// ����
	double		curr;	// ���簡
	double		prc;	// �ֹ��ܰ�
//	double		cdgr;	// CD�ݸ�
	double		hvbd;	// ������������
	double		jjis[2];	// ����������(�޷�,�ŷ�)
	int		mmgb;	// �Ÿű��� (-1. �ŵ�, +1. �ż�)
	double		gdan;	//2012.05.17 KSJ �ŷ��¼� �߰���. ���� �ŷ��¼� / 100000
};

struct _mouseLink	{
	int	xpt{};
	int	ypt{};
	CString symb;
	CString data;
};

struct _userMsg{
	int		nSize{};
	int		kind{};
	CString		gab;
};

struct _MouseValue	{
	CString		kpt;
	CString		sValue;
};

// PIBO4013
#define	TRN_HEDGE	"BO410101"
struct _mido{
	char func;	// start "F"
	char mymd[6];	// �����
};

#define TRN_JJGA	"BO410102"
struct _jdat{
	char code[8];
	char jjga[6];
};

#define	MAX_R		30
struct  _gmodo {
    char    c_rtcd[8];  /* ��RTS CODE           */
    char    c_mdif[8];  /* ������(����)         */
    char    c_mgjv[8];  /* �ݹ̰�������         */
    char    c_gvol[8];  /* �ݰŷ���             */
    char    c_msga[8];  /* �ݸż�               */
    char    c_mdga[8];  /* �ݸŵ�               */
    char    c_rate[8];  /* �ݵ����             */
    char    c_diff[7];  /* �ݴ��               */
    char    c_curr[8];  /* �����簡             */
    char    hang[8];    /* ��簡               */
    char    jihs[8];    /* ����ȯ��             */
    char    p_rtcd[8];  /* ǲRTS CODE           */
    char    p_curr[8];  /* ǲ���簡             */
    char    p_diff[7];  /* ǲ���               */
    char    p_rate[8];  /* ǲ�����             */
    char    p_mdga[8];  /* ǲ�ŵ�               */
    char    p_msga[8];  /* ǲ�ż�               */
    char    p_gvol[8];  /* ǲ�ŷ���             */
    char    p_mgjv[8];  /* ǲ�̰�������         */
    char    p_mdif[8];  /* ǲ����(����)         */
};
		
struct  _modo {
    char    atmg[8];    /* ATM  */
    char    grec[4];
    struct  _gmodo    gmod[MAX_R];
};
//-----------------------------------