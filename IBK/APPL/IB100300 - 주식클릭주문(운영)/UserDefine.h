#pragma once
//----------------------------------------------------------------------------------------
//	ETC
//----------------------------------------------------------------------------------------
#define	cDEL	0x7f		// Delimiter
#define	cTAB	'\t'
#define	cLF		'\n'
#define	cCR		'\r'
#define	cNL		0x0A

#define GAP	1


#define ST_TEXT         0x00000000	// �ؽ�Ʈ
#define ST_NORMAL       0x00000001	// 0 ���� 
#define ST_COLOR        0x00000002	// Į�� 
#define ST_REMOVE	0x00000004	// +, - ����
#define ST_COMMA	0x00000008	// comma
#define ST_PERCENT      0x00000010	// %
#define ST_SHGA		0x00000020	// ����/���� ��� �϶� ǥ��
#define ST_BOLD		0x00000040	// bold
#define ST_DRYL		0x00000080	// ����� ǥ��	


#define MAP_TITLE	"[1003] �ֽ� Ŭ���ֹ�"
#define MAP_NAME	"IB100300"
#define MAP_NO		"1003"
#define MAX_FC		50




//----------------------------------------------------------------------------------------
//	User Message & Parameter
//----------------------------------------------------------------------------------------
#define	UM_MYMESSAGE		WM_USER + 1
#define	UP_GET_JCOD		10
#define	UP_GET_CODX		11
#define	UP_GET_ACCOUNT		12
#define	UP_GET_PASSWORD		13
#define	UP_GET_NAME		14
#define	UP_GET_CODENAME		15
#define UP_GET_CONDITION	16
#define	UP_SET_CODE		30
#define	UP_SET_GUIDE		31
#define	UP_GET_LISTEDQUANTITY	32	//2014.06.23 KSJ �����ֽļ� ��������
#define	UP_SEND_JUMUN		40
#define	UP_SEND_MAXJMCN		41
#define	UP_INVALIDATE_HOGA	51
#define	UP_SEND_FEE			42	//2015.03.16 KSJ ����ݾ� ���ϱ� ���� ������

#define UP_SHOW_JANGO		43

#define UP_GET_PRGBTYPE		44  //�ܰ��� ���õ� ������ Ÿ�� ����, �ſ�, ���� ���� ������
	
#define WP_MADO_ALL_CANCEL		(60)
#define WP_MASU_ALL_CANCEL		(61)
#define WP_MADO_RSVD_ALL_CANCEL	(62)
#define WP_MASU_RSVD_ALL_CANCEL	(63)




#define	UM_HOGAMESSAGE		WM_USER + 2
#define	UP_RESERVE		10




//----------------------------------------------------------------------------------------
//	�Ÿű���
//----------------------------------------------------------------------------------------
#define BASIC_JUMUN	0
#define MD_JUMUN	1
#define MS_JUMUN	2
#define MS_MISU		3
#define SIJANG_JUMUN	4	//���尡


//----------------------------------------------------------------------------------------
//	Size
//----------------------------------------------------------------------------------------
#define ACC_WIDTH		140
#define CTRL_HEIGHT		20

#define MAP_WIDTH		653
//#define MAP_HEIGHT		555	//487
#define MAP_HEIGHT		600	//487

#define BT_JANGO_HEIGHT 200

#define ATTACHMAP_WIDTH		300
//#define	ATTACHMAP_HEIGHT	493
#define	ATTACHMAP_HEIGHT	538

#define HOGA_WIDTH		480	//430
// #define HOGA_HEIGHT		449	//403
#define HOGA_HEIGHT		494	//403

#define CTRL_WIDTH		150

#define VERT_GAP	3
#define HORI_GAP	3
#define WIDE_GAP	5
#define BOT_GAP		5




//----------------------------------------------------------------------------------------
//	TR Name
//----------------------------------------------------------------------------------------
#define TN_SISE		"PIBO1003"
#define TN_MICHE	"PIHOSCHG"
#define TN_JUMUN	"PIBOSODR"
#define TN_SYMBOL	"POOPPOOP"
//#define TR_JANGO	"PIBOTUXQ"	// TxHead: SC234500_Q
#define TN_MAXJMCT	"pibopbxq"	// TxHead: SC217000_Q
#define TN_CHECKPSWD	"pibopswd"
#define SN_SACMT207 "SACMT238"
//2013.07.24 KSJ �ݵ� �ݿ� ����
// #define SN_SACMQ227 "SACMQ227"	//2013.07.10 KSJ 999-91 ������ ��й�ȣ�� �����ϱ� ���ؼ�.
#define TN_FEE		"PIHOFEE"	//2015.03.16 KSJ ��ü������ ��ȸ

//----------------------------------------------------------------------------------------
//	Service Name
//----------------------------------------------------------------------------------------
//#define SN_MAXJMCT	"SC217000_Q"
#define SN_MAXJMCT		"SONAQ200"
#define SN_MAXCRJMCT	"SONAQ222"


//----------------------------------------------------------------------------------------
//	TR Key
//----------------------------------------------------------------------------------------
#define TK_SISE		1
#define TK_JUMUN	2
#define TK_MICHE	3
#define TK_MDJMCN	10
#define	TK_MSJMCN	11
#define TK_MAXMSJMCN	12
#define TK_ACCOUNT	200
#define TK_SYMBOL	21
#define TK_NOTIFYCTRL	22
#define TK_CHECKPSWD	23
#define TK_CHECK207	24
#define TK_LISTEDQUANTITY	25
#define TK_FEE				26	//2015.03.16 KSJ ��ü������
#define TK_SEARCH_MARKETMSG	27

#define TK_CRMDJMCN		28
#define	TK_CRMSJMCN		29
#define TK_CRMAXMSJMCN	30

#define TK_VISYM		31
/* 2013.07.24 KSJ �ݵ� �ݿ� ����
#define TK_CHECK227	24
*/
#define TK_GROUP			100
#define TK_ACCNTNAME		101
#define oKEY	0




//----------------------------------------------------------------------------------------
//	Color
//----------------------------------------------------------------------------------------
#define COLOR_LINE	93
#define COLOR_TABLE	75
#define COLOR_BK	64
#define COLOR_CONTENTS_BK	181
#define COLOR_GRIDHEAD	74
#define COLOR_GUIDEBACK	66
#define COLOR_DATA	99
#define COLOR_TEXT	130
#define COLOR_TABLEHEAD	96
#define COLOR_GRIDHEADTXT	76



//----------------------------------------------------------------------------------------
//	ETC Structure
//----------------------------------------------------------------------------------------
struct _jumun
{
	char	cMmgb;
	int	iPrice;
	int	iJuno;
	int	iJqty;
	int bMarketPrice;	//2012.11.07 KSJ ���尡�϶��� 1�� ����
};


//----------------------------------------------------------------------------------------
//	Notice Symbol
//----------------------------------------------------------------------------------------
#define ACCN	901		// ����
#define JUNO	904		// �ֹ���ȣ
#define OJNO	905		// ���ֹ���ȣ
#define CODX	907		// RTS �����ڵ�
#define JQTY	909		// �ֹ�����
#define JPRC	910		// �ֹ�����
#define MMGB	912		// �Ÿű��� (�ŵ�, �ż�)
#define MCCT	921		// ��ü�����
#define OKCT	931		// ü��, ����, ��� ����
#define NTKD	996		// Notice Kind
				//  2:    // ����Ȯ��     
				//  3:    // ���Ȯ��     
				//  1:    // ü��
				// 21:    // ����         
				// 24:    // ��Ÿ����     
				//  4:    // �ź�         
				// 31:    // ��������     
				// 32:    // ������������ 
				// 33:    // ����������� 
				// 34:    // ��Ÿ�������� 
				// 22:    // ��������     
				// 23:    // �������


//----------------------------------------------------------------------------------------
//	ETC Structure
//----------------------------------------------------------------------------------------
struct _config 
{
	int	percent=0;		// ����� ǥ�� 0:ǥ�þ���, 1:ǥ��
	int	percentRadio=0;		// 0:�ð���, 1:BEP, 2:user
	int	percentKind=0;		// 0:���ذ�, 1:��������, 2:�ð�, 3:��: 4:����
	CString	BEP;
	CString user;
	int	mod=0;			// �����ֹ��� ���/�Ϻ� 0:��� 1:�Ϻ�
	int	jmcn=0;			// �ֹ����� ǥ�� ��� 0:�ֹ�(�Ǽ�) 1:�ֹ�
	int	confirm=0;		// �ֹ�Ȯ��â ���� ���� 0:�Ⱥ��� 1:����
	int hold_amt=0;		// 2015.03.11 KSJ ���񺯰� �� ���� ���� ���� 0:�������� 1:������
	int hogaclr=0;		// KSJ ȣ������ �ð��� ǥ�� 0:ǥ�þ���, 1:ǥ��  
	int popupremain=0;    // �������� ���� 2017.03.30
	int	showVI=0;			//����vi ǥ�ÿ���
};


//----------------------------------------------------------------------------------------
//	TR Structure
//----------------------------------------------------------------------------------------

struct _sise_mid
{
	char	code[6];	        // �����ڵ�
};
#define sz_SISEMID		sizeof(struct _sise_mid) 

struct	_sise_mod	{
	char	codx[12];		// RTS CODE
	char	jmgb[1];		// ���񱸺�(+KP, -KQ)	
	char	curr[9];		// ���簡	
	char	diff[9];		// ���ϴ��	
	char	gvol[12];		// �ŷ���	
	char	gamt[12];		// �ŷ����	
	char	jvol[12];		// ���� �ŷ���	
	char	rate[9];		// �����	
	char	shga[9];		// ���Ѱ�	
	char	hhga[9];		// ���Ѱ�	
	char	gjga[9];		// ���ذ�	
	char	siga[9];		// �ð�		
	char	koga[9];		// ��		
	char	jega[9];		// ����		
	char 	jgbn[3];		// ���ű���	
	struct	{
		char	mdga[9];	// �ŵ�ȣ��     	
		char    msga[9];	// �ż�ȣ��			
		char    dvol[9];	// �ŵ�ȣ������ 	 
		char    svol[9];	// �ż�ȣ������		 
		char    dcha[9];	// �����ŵ�ȣ������ 
		char    scha[9];	// �����ż�ȣ������ 
		char    dcnt[9];	// �ŵ�ȣ���Ǽ�     
		char    scnt[9];	// �ż�ȣ���Ǽ�		
	} hoga[10];		// ����/�ɼ� ȣ�� 	

	char	dvol[9];		// ȣ���Ѽ��� : �ŵ�
	char	svol[9];		//            : �ż�
	char	dcha[9];		// ��������ѷ�:�ŵ�
	char	scha[9];		// 	       :�ż�	
	char	sdch[9];		// �ܷ���(svol-dvol)
	char	sum4[9];		// �����հ� : 5�� 	
	char	sum9[9];		// �����հ� : 9�� 	
	char	jggy[9];		// ���ű���
	char	jmdw[9];		// �ֹ�����
	struct  {
		char time[6];		// ü��ð� HHMMSS 
		char curr[9];		// ü�ᰡ��
		char cvol[6];		// ü�����
	} cheg[MAX_FC];
};
#define sz_SISEMOD		sizeof(struct _sise_mod)

struct	_miche_mid	
{
	char    gubn[1];	//2015.05.28 KSJ ������ '>' �־���.
	char    accn[11];
	char    pswd[8];
	char    date[8];	//  => "            "
	char    cod2[12];	//  => "A        "
	char    juno[5];	// �����ֺй�ȣ
	char    dsgb[1];	// 0.All 1.�ŵ� 2.�ż�      
	char    otim[1];	// 0.All 1.���� 2.�ð���    
	char	tcod[2];	// 00.All else. �ش��ü�ڵ�
	char    dlgb[1];	// 0.All 1.ü�� 2.��ü��    
	char    sort[1];	// 1.���� 2.����            
	char    juno2[10];	// �����ֹ���ȣ2	     //2015.05.28 KSJ ��ȣ 5 -> 10               
};
#define sz_MICHEMID		sizeof(struct _miche_mid)

struct	s_rec	{
	char    juno[10];    // �ֹ�   ��ȣ      //2015.05.28 KSJ ��ȣ 5 -> 10
	char    ojno[10];    // ���ֹ� ��ȣ      //2015.05.28 KSJ ��ȣ 5 -> 10
	char    cod2[12];   // �����ڵ�         
	char    hnam[40];   // �����           
	char    odgb[20];   // �ֹ�����         
	char    jcgb[20];   // ������ұ��� 
	char    hogb[20];   // ȣ������         
	char    jprc[12];   // �ֹ�����         
	char    jqty[12];   // �ֹ�����         
	char    dprc[12];   // ü�ᰡ��         
	char    dqty[12];   // ü�����         
	char    tqty[12];   // ü�������       
	char    wqty[12];   // ��ü�����       
	char    stat[20];   // ��������         
	char    time[8];    // �ֹ��ð�         
};
#define L_MICHE_REC		sizeof(struct s_rec)

struct	_miche_mod	
{
	char	accn[11];
	char	nrec[4];
	struct	s_rec	rec[250];
};


struct _jumun_mid		// �ֹ�
{
	char	rcnt[4];
	char	odgb;
	char	mkgb;
	char	mmgb;
	char	acno[11];
	char	pswd[8];
	char	ogno[12];
	char	code[12];
	char	jqty[8]; 
	char	jprc[10];  
	char	hogb[2];
	char	jmgb[1];
	char	mdgb;
	char	prgb;
	char	gmo[20];
	char	gmm[20];
};
#define sz_JUMUNMID		sizeof(struct _jumun_mid)

struct _jumunmod
{
	char	rcnt[4];
	char	jmno[6];
	char	mono[6];
	char	omsg[80];
};

// �ִ��ֹ����ɼ���

struct _maxjmct_mid
{
	char in[5];				//	���ڵ尹��
	char zBnsTp[1];			//	�Ÿű���
	char zAcntNo[20];		//	���¹�ȣ
	char zPwd[8];			//	��й�ȣ
	char zIsuNo[12];		//	�����ȣ
	char dOrdPrc[13];		//	�ֹ���
	char zCommdaCode[2];	//	��Ÿ�ü�ڵ�(�̻��)
	char zLoanPtnCode[2];	//	���������ڵ�(�̻��)
};

#define L_maxjmct_mid	sizeof(struct _maxjmct_mid)


struct _maxjmct_mod
{
	char Out[5];						//	���ڵ尹��											
	char zAcntNm[40];					//	���¸�
	char zIsuNm[40];					//	�����
	char zBnsTpNm[10];					//	�Ÿű���
	char lDps[16];						//	������
	char lSubstAmt[16];					//	���ݾ�
	char lMnyMgn[16];					//	�������űݾ�
	char lSubstMgn[16];					//	������űݾ�
	char lRcvblSumAmt[16];				//	�̼��հ�ݾ�
	char lRuseOrdAbleAmt[16];			//	�����ֹ����ɱݾ�
	char lMnyRuseObjAmt[16];			//	����������ݾ�
	char lSubstRuseObjAmt[16];			//	���������ݾ�
	char lPrdayRuseOrdAbleAmt[16];		//	���������ֹ����ɱݾ�
	char lCrdayRuseOrdAbleAmt[16];		//	���������ֹ����ɱݾ�
	char lMnyOrdAbleAmt[16];			//	�����ֹ����ɱݾ�
	char lSubstOrdAbleAmt[16];			//	����ֹ����ɱݾ�
	char lCrdtPldgRuseAmt[16];			//	�ſ�㺸����ݾ�
	char lSeOrdAbleAmt[16];				//	�ŷ��ұݾ�
	char lKdqOrdAbleAmt[16];			//	�ڽ��ڱݾ�
	char lAbleAmt[16];					//	���ɱݾ�
	char lMgnRat100pctOrdAbleAmt[16];	//	���űݷ�100�ۼ�Ʈ�ֹ����ɱݾ�
	char lMgnRat100OrdAbleQty[16];		//	���űݷ�100�ۼ�Ʈ�ֹ����ɼ���
	char dLoanPldgRat[9];				//	����㺸��
	char dPldgMaintRat[7];				//	�㺸��������
	char lOrdAbleQty[16];				//	�ֹ����ɼ���
	char lUnercBuyOrdAmt[16];			//	��ü��ż��ֹ��ݾ�
	char dCmsnAmtRat[11];				//	��������
	char lAddCmsnAmtLevyAmt[16];		//	�߰�������¡���ݾ�
	char lBalQty[16];					//	�ܰ����
	char lHtsOrdAbleAmt[16];			//	HTS�ֹ����ɱݾ�
	char lD1Dps[16];					//	D+1������
	char lD2Dps[16];					//	D2������
	char lPrdayRuseOrdAbleQty[16];		//	���������ֹ����ɼ���
	char lCrdayRuseOrdAbleQty[16];		//	���������ֹ����ɼ���
	char lCrdtOrdAbleAmt[16];			//	�ſ��ֹ����ɱݾ�
};

#define L_maxjmct_mod	sizeof(struct _maxjmct_mod)

//// sonaq222 �ſ밡�ɼ��� 

struct _maxCrjmct_mid
{
	char in[5];				//	���ڵ尹��
	//char zBnsTp[1];			//	�Ÿű���
	char zAcntNo[20];		//	���¹�ȣ
	char zPwd[8];			//	��й�ȣ
	char zBnsTp[1];
	char LoadDtlClssCode[2];
	char zIsuNo[12];		//	�����ȣ
	char dOrdPrc[13];		//	�ֹ���
	char LoadDt[8];
	char zCommdaCode[2];	//	��Ÿ�ü�ڵ�(�̻��)
	char zLoanPtnCode[2];	//	���������ڵ�(�̻��)
};

#define L_maxCrjmct_mid	sizeof(struct _maxCrjmct_mid)

struct _maxCrjmct_mod
{
	char Out[5];						//	���ڵ尹��											
	char OrdPrc[13];					//	
	char OrdAbleQty[16];				//	
	char OrdAbleAmt[16];				//	
};

#define L_maxCrjmct_mod	sizeof(struct _maxCrjmct_mod)

/////////////////////////////////////////



struct  pmid {
    char    accn[11];
    char    pswd[8];
};

struct  pmod {
    char    sflg[1];                /* OK '1' else ERROR    */
    char    emsg[99];
}; 

#define L_pmid	sizeof(struct pmid)
#define L_pmod	sizeof(struct pmod)


//2015.03.16 KSJ ��ü������
struct _pihocfee_mid
{
	char in[5];				//	���ڵ尹��
	char zAcntNo[20];		//	���¹�ȣ
	char zPwd[8];			//	��й�ȣ
	char zShTp[1];			//	��ȸ���� Fix : 2 
	char zUserID[8];		//	����� ID
};

#define L_pihocfee_mid	sizeof(struct _pihocfee_mid)


struct _pihocfee_mod
{
	char Out[5];						//	���ڵ尹��											
	char zOutDump[20];					//	����
	char zOutCall[11];					//	�ݼ���
	char zOutHts[11];					//	HTS
	char zOutArs[11];					//	ARS
	char zSaveCall[16];					//	SAVE �ݼ���
	char zSaveHts[16];					//	SAVE HTS
	char zSaveArs[16];					//	SAVE ARS
	char zCheck[1];						//	
};

#define L_pihocfee_mod	sizeof(struct _pihocfee_mod)

/*
struct _maxjmct_mid
{
	char	accn[10];	// ���¹�ȣ
	char	pswd[8]	;	// ��й�ȣ
	char	dsgb;		// ��ȸ����	1: �ż� 2: �ŵ�     
	char	jcod[12];	// �����ȣ
	char	jmdg[12];	// �ֹ�����
	char	sjmg[2]	;	// �����������ü����	00:�Ϲ�    100%�ֹ����ɼ����� ������ ���� ������ ��ü����  
						//	01:���̹�      
						//	02:�ݼ���      
						//	03:�����     
	char	sgog;		// �ð��ܱ���	0:�Ϲ� 1:�ð���     
};

struct _maxjmct_mod
{
	char	ajgy[9];	// �������űݷ�
	char	jjgy[9];	// �������űݷ�
	char	ajyn[20];	// �������űݷ���
	char	jjyn[20];	// �������űݷ���
	char	mxjg[12];	// �ִ��ֹ����ɱݾ�
	char	mnjg[12];	// �ּ��ֹ����ɱݾ�
	char	sghg[12];	// ��밡������
	char	sgdy[12];	// ��밡�ɴ��
	char	sdjs[12];	// �ſ�㺸����
	char	msgc[12];	// �ż�/�ŵ����ɼ���
	char	bpsc[12];	// 100%�ż����ɼ���
	char	jjgc[12];	// �������밡�ɼ���
	char	gjgc[12];	// �������밡�ɼ���
	char	ysgm[12];	// ������
	char	dygm[12];	// ����
	char	hgjg[12];	// �������ű�
	char	dyjg[12];	// ������ű�
 };
*/


