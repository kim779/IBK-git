#ifndef __DOORI__INTPACKET__HEADER__
#define	__DOORI__INTPACKET__HEADER__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////WMLGT_NEW
typedef struct {
	char	Under		[  1];				/* ��/��������					*/
	char	Codegb		[  1];				/* �ڵ屸��(������ ����)		*/
	char	code		[ 15];				/* �����ڵ�						*/
	char	Name        [ 30];				/* �����						*/
	char	Price		[  9];				/* ���簡						*/
	char	Change		[  6];				/* ���ϴ��(�����)				*/
	char	Rate		[  5];				/* �����(%)					*/
	char	Sign		[  1];				/* �����ȣ						*/
	char	Volume		[ 12];				/* �����ŷ���					*/
	char	Offerho		[  9];				/* �ŵ�ȣ��						*/
	char	Bidho		[  9];				/* �ż�ȣ��						*/
	char	Ofremain	[ 12];				/* �ŵ� �ֿ켱�ܷ�				*/
	char	Bidremain	[ 12];				/* �ż� �ֿ켱�ܷ�				*/
	char	Offertot	[ 12];				/* �Ѹŵ��ܷ�					*/
	char	Bidtot		[ 12];				/* �Ѹż��ܷ�					*/
	char	Open		[  9];				/* �ð�							*/
	char	High		[  9];				/* ��							*/
	char	Low			[  9];				/* ����							*/
	char	Value		[ 14];				/* �ŷ� ���	                */ 
	char	UpLmtPrice	[  9];				/* ���Ѱ�						*/
	char	DnLmtPrice	[  9];				/* ���Ѱ�						*/
	char	Openyak		[  9];				/* �̰�����������				*/
	char	FacePrice	[  9];				/* �׸鰡       				*/
	char	YDSign		[  1];				/* ���� �����ȣ				*/
	char	YSGubun		[  1];				/* ���� ����					*/
	char	YSCount		[  2];				/* ���� �ϼ�					*/
	char	Recprice	[  9];				/* ���ذ�						*/

	char    cvolume     [ 9];				/* ü�ᷮ           */
    char    open_chrate [ 5];				/* �ð����ϴ����   */
    char    high_chrate [ 5];				/* �����ϴ����   */
    char    low_chrate  [ 5];				/* �������ϴ����   */
	char    high_time   [6];				/* �� �ð�*/
	char    low_time    [6];                /* ���� �ð�*/
// ����ü�� �߰� - cst 2004/12/09
    char    expsign         [ 1];			// ����ü�� ��ȣ                        
    char    expprice        [ 9];			// ����ü�� ��                          
    char    expchang        [ 9];			// ����ü�� ���                        
    char    expchgrt        [ 9];			// ����ü�� �����                      
    char    expvol          [12];			// ����ü�� �ŷ��� 
	char    color           [ 1];			/* 1:Blue 2:Yellow 3:Green 4:Red        */
//	char	Name		[ 20];				/* ����� 						*/
//	char	pos			[  2];				/* ����                         */   
//	char	Momentvol	[ 10];				/* ü�ᷮ						*/
//	char	Rights		[  2];				/* �׸���ҿ��� : 4*=�׸����	*/
//	char	HSign		[  1];				/* ���Ѱ���ȣ(����,�ɼ�,�ֿɸ�)	*/
//	char	LSign		[  1];				/* ���Ѱ���ȣ(����,�ɼ�,�ֿɸ�)	*/
//	char	NewsGb		[  2];				/* ���� ���� bit setting		*/
//	char	Gsiyn		[  1];				/* 1���� ���� ����				*/
} ST_INQUIRYDATA; // 05010�� ����.

#define	 CNTMAX   2000 
#define  SH_KSIZE  6
typedef struct
{
	char	gubun[2];         /* '01'���� '02'��� '03'���� '04'���� '05'�϶� '06'�Ű� '07'����*/
} TR_05021_I;

typedef struct _TR_05021_S{
	char    bit[1];           /* '1'�峻(KOSPI) '2'���(KOSDOQ) */
	char	code[SH_KSIZE];          /* ���� �ڵ� */
	char	hname[20];        /* ����� */
} TR_05021_S;

typedef struct
{
	char        pWnd[4];
	char        ogubun[2];    /* ���� */
	char        totcnt[4];    /* total Count */
	TR_05021_S  data[1];
} TR_05021_O; 

typedef struct {
	char    pWnd[4];    
	char	Count[4];
	char	Data1[1];
} SSP_GETGCODE;

typedef struct
{
	char ncode[SH_KSIZE];
}TR08325_Item_i;

typedef struct
{
	char pWnd[4];
	char nsize[3];
	TR08325_Item_i nData[1];
}TR_08325;

//OUT
typedef struct
{
	char ncode[SH_KSIZE];   // �ڵ�
	char ncur[9];		// ���簡(���ذ�)
	char nsign[1];		// �����ȣ
}TR08325_Item_o;

typedef struct 
{
	char pWnd[4];
	char ncnt[3];
	char ndate[8]; //YYYYMMDD
	char ntime[6]; //HHMMSS

	TR08325_Item_o nData[1];	
}TR08325_o;

#define TR_ROLLDATA  "08325"
#define TR_INTEREST  "05010"	//���� ����Ʈ ������ �ޱ�
#define TR_INTETC	 "05020"	//Ư������ ���簡, ���Ϻ� �� ���� ������ �ޱ�.
#define TR_INTETC2	 "05021"	//Ư�����񸮽�Ʈ�� �ޱ�
#define TR_HAVEACCOUNT	"71020"
#define TR_IFLIST	 "20010"
#define TR_SPECIALITEM "20011"
#define TR_IFMYNET		"80000"

#define TR_ERR_DATANOTRECEIVE	0
#define TR_ERR_COMM				1
#define TR_OK					2
#define TR_INFOREMOVE			3

#define INT_COMM_TRRECEIVE		0
#define INT_COMM_TRRECEIVE2		1
#define INT_DATA_SENDTR			3
#define INT_DATA_RESETBACKUP	13


#define NOTIFY_REFLESHROLL  1	// �������⿡ ������Ʈ �� ���� �˸�
#define NOTIFY_RESETALLDATA 3   // ��� ���� ���õ� �Ϳ� ������Ʈ�� ���� �˹�
#define NOTIFY_RESETALLETC1 6   // ���ɿ��� ����� ���� ��û �Ǿ����� ó�� 
                               // ���� ���� ó�� ���ϴ� ����.
//2004.12.02 by LYH
#define NOTIFY_CHANGEDFILE   7   // �������� ���� �����ޱ⸦ �� ���. ���� ���� ���� ó��

#define NOTIFY_ADVISE 5
#define NOTIFY_UNADVISE 4

// IN, OUT ������ �������� ��Ŷ�� �ְ� ���� ����.
#define REQUEST_ERROR		 0
#define REQUEST_INTALL		 1 // ���� ��� ���� ���(�޸� ī���ؾ� ��) 
#define REQUEST_REG_IF		 2 // ���� ��� 
#define REQUEST_REG_ITEM     3 // �������� ���
#define REQUEST_REG_SIGNAL   4 // ���� ���
#define REQUEST_REG_OPENDLG  5 // ���� ���â ���� 
#define REQUEST_INTALLRETURN 6 // ���� ��� ���� ���(�޸� ī���� �ʿ� ����)
#define REQUEST_GETCODE		 7 // �ڵ� �޴��� ���
#define REQUEST_GETIF		 8 // �ӽ� �׷� ���� ���
#define REQUEST_GETZZIM		 9 // ��� ����
#define REQUEST_INTMODIFYFALSE 10 // 
#define REQUEST_CHANGEGROUP 11
#define REQUEST_REG_NEWS    12
#define REQUEST_GETNEWS     13
#define REQUEST_REMOVEZZIM  14 // ��� ����
#define REQUEST_SYNCGROUP   15
#define REQUEST_GETSYNCGROUP 16
#define REQUEST_GIVEENVINFO 17
#define REQUEST_CANOPENDLG		18
#define REQUEST_GROUPINFO	20
#define REQUEST_GETNOGROUP	21
#define REQUEST_MYGETNETLIB 22
typedef struct
{
//	char Iwho[2]; // 0 : ����, 1 : ����, 2 : ����, 3 : ��ȣ, 4 : ��Ʈ
	char Igubun[2]; // 0 : ERROR, 1 : 1��ü ����(���񸮽�Ʈ ����), 2 : ���� �׷� ���, 3 : ���� ���    
	char Icnt[3]; // �׷� ��
	char IopenDlg[1]; // 0 : Open, 1 : No Open
	void* Idata;  // �׷� �����͵�(CPtrArray) 
	char Ititle[256];
}INT_TOTAL;

typedef struct
{
	char Ino[2];		// �׷� ��ȣ 
	char Ititle[20];	// �׷� ��
	char Icnt[3];		// �׷� �ȿ� ���� ��
	void* Idata;		//���� �����͵�(CPtrArray)
	char Itime[6];
	char Idate[8];
}INT_GROUP_ST;

/*
typedef struct
{
	char Icode[12];  // �ڵ�  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   ��,��,��
	char Idate[8];		   // YYYYMMDD ��,��,��
	char Ivol[10];		   // ���� 	
	char Ival[10];		   // ���԰���
	void* Idata;
	char IName[30];
} INT_ITEM_ST;*/
typedef struct
{
	char Icode[12];  // �ڵ�  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   ��,��,��
	char Idate[8];		   // YYYYMMDD ��,��,��
	char Ivol[10];		   // ���� 	
	char Ival[10];		   // ���԰���
	char ICheck[1];		   // üũ����
	void* Idata;
	char IName[30];
} INT_ITEM_ST;

//<<<<<<<<<<<<  ����Ʈ�˻� : 20010 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct _D20010_I
{
    char    dtGb        [ 1];   /* 0:��ü 1:�˻� 2:���� */
    char    Gb          [ 1];   /* 1:�ü� 2:������ 3:��ǥ��ȣ 4:��õ����*/
    char    jmcode      [ 6];   /* �����ڵ�             */
    char    text        [50];   /* �˻���               */
} D20010_I;

typedef struct _D20010_OG
{
    char    grptmnm     [30];   /* �׸���   */
    char    grptmcd     [ 4];   /* �׸��ڵ� */
} D20010_OG;

typedef struct _D20010_O
{
	char		dtGb		[1];
	char		Gb			[1];
    char        grdCnt      [ 4];
    char		MsgCnt      [ 2];
    char		grdBjMsg    [18];
    D20010_OG   grdData     [1];
} D20010_O;


//<<<<<<<<<<<<  ����˻� : 20011 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct _D20011_I
{
    char    grptmcd      [ 4];  /* �׸��ڵ� */
} D20011_I;

typedef struct _D20011_OG
{
    char    sign        [ 1];
    char    hname       [20];
    char    jmcode      [ 6];
    char    price       [ 9];
    char    change      [ 9];
    char    chgrate     [ 9];
    char    volume      [10];
} D20011_OG;

typedef struct _D20011_O
{
	char    grptmcd			[ 4];  /* �׸��ڵ� */
    char        grptmnm     [30];

    char        sang        [ 3];
    char        up          [ 3];
    char        bo          [ 3];
    char        dn          [ 3];
    char        ha          [ 3];
    char        tot         [ 5];

    char        uprate      [ 6];
    char        rate        [ 6];
    char        uprate_sign [ 1];

    char        grdCnt      [ 6];
    char        grdBjMsg    [18];
    D20011_OG   grdData     [1];
} D20011_O;

typedef struct _D71020_I
{
    char    nxtGb       [ 1];       /* ����, ���� ����          */
    char    winid       [ 4];       /* �����ʿ��� �����ؿ� DATA */
    char    gejanum     [11];       /* ���¹�ȣ                 */
    char    jmcode1     [12];       /* �����ڵ�:��ü��ȸ�ô� �������� ����  */
	char    singb       [ 1];       /* �ſ뱸��:��ü��ȸ�ô� �������� ����  */
    				    /* ó�� ��ȸ�ÿ��� ������ȸ�� �־���� ������ ��� 
    				       �������� �־ ���۹ٶ�.	    */
    char    jmcode      [ 7];       /* 'A'&�����ڵ�:������ȸ�� ���������ڵ� */
    char    loanDay     [ 8];       /* ������ȸ�� ��������      */
    char    sinGb       [ 2];       /* �ſ뱸�� : 00            */
    char    OnOfftp     [ 2];       /* On Off tp: 00            */
} D71020_I;

typedef struct _D71020_OG
{
									/* '1':�峻, '2':�ڽ���,  '3':����      */
									/* '4':�ɼ�, '5':��3����, '6':�ֽĿɼ�  */
    char    mkgb        [ 1];       /* ���屸��         */
    char    jmcode      [ 7];       /* �����ڵ�         */
    char    hname       [20];       /* �����           */
    char    change      [ 9];       /* ���             */
    char    chgrate     [ 9];       /* �����           */
    char    price       [ 9];       /* ���簡           */
    char    volume      [ 9];       /* �ܰ����         */
    char    danga       [ 9];       /* ���Դܰ�         */
    char    value       [12];       /* ���Աݾ�         */
    char    evaval      [12];       /* �򰡱ݾ�         */
    char    benefit     [12];       /* ����             */
    char    bnftrt      [ 9];       /* ���ͷ�           */
    char    bidho       [ 9];       /* �ż�ȣ��         */
    char    offerho     [ 9];       /* �ŵ�ȣ��         */
    char    hprice      [ 9];       /* ���Ѱ�           */
    char    lprice      [ 9];       /* ���Ѱ�           */
    char    loadDay     [ 8];       /* ������           */
} D71020_OG;

typedef struct _D71020_O
{
    char        nxtGb       [ 1];   /* 0:���Ӿ���. 1:��������   */
                                    /* Key�� �� ������ �����ڵ� */
    char        winid       [ 4];   /* ���� data�� �״�� �ݼ�  */

 	char        grdCnt      [ 6];
    char        grdBjMsg    [20];

    D71020_OG   grdData     [1];
} D71020_O;

const UINT RMSG_INTREQUEST = ::RegisterWindowMessage("RMSG_INTREQUEST");
const UINT RMSG_CONCERNHWND	= ::RegisterWindowMessage(_T("RMSG_CONCERNHWND"));

const UINT RMSG_GETRECENTCODE = ::RegisterWindowMessage("RMSG_GETRECENTCODE");
const UINT RMSG_NOTIFYRECENTCODE = ::RegisterWindowMessage("RMSG_NOTIFYRECENTCODE");
const UINT RMSG_CHANGESKIN = ::RegisterWindowMessage(_T("RMSG_CHANGESKIN"));
#endif // __DOORI__INTERESTDEF__HEADER__