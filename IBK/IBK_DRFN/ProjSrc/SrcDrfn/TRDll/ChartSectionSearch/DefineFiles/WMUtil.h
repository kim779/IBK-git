// WMUtil.h: interface for the CWMUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WMUTIL_H__61A0968A_BCC0_4B3C_96F6_9C7AC762ED28__INCLUDED_)
#define AFX_WMUTIL_H__61A0968A_BCC0_4B3C_96F6_9C7AC762ED28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWMUtil  
{
public:
	CWMUtil();
	virtual ~CWMUtil();

};

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2002.07.29
//
//	����	:	���� Group�� Type�� �����Ѵ�.
//
//	����	:	�⺻������ ���ɰ� �׿��� ���� Ư�� ���� ���� Group���� �з��ȴ�.
//				Ư�� Tree Combo�� Item�� Data�� �����ϱ� ���Ͽ� ���� 31��° Bit��
//					Flag�� �ǵ��� �Ѵ�.
//				����, TYPE_INTEREST Bit-Flag�� �������� ���� Item�� ���� Ư�� ���� Group�̴�. 
///////////////////////////////////////////////////////////////////////////////
#define TYPE_INTEREST	0x40000000
#define TYPE_BOYOU		0x20000000
#define TYPE_MASTER		0x10000000			// ��ü/�ŷ���/�ڽ��� ����

//WMLGT_NEW 731 START
#define TYPE_OTHER		0x40000001

#define FETCHITEM_COUNT  50
#define FETCHITEM_ALL	 0
//WMLGT_NEW 731 END

#define GUBUN_KOSPI		1
#define GUBUN_KOSDAQ	2
#define GUBUN_FUTURE	3
//#define GUBUN_ROPTION	4
//#define GUBUN_THIRD		5
#define GUBUN_SOPTION	4

#define GUBUN_KOSPISTR   "�ڵ�-����"
#define GUBUN_KOSDAQSTR  "�ڵ�-����"
#define GUBUN_FUTURESTR  "�ڵ�-����"
#define GUBUN_ROPTIONSTR "�ڵ�-�ɼ�"
#define GUBUN_THIRDSTR   "�ڵ�-��3����"
#define GUBUN_SOPTIONSTR "�ڵ�-�����ɼ�"
#define GUBUN_ELW        "�ڵ�-ELW"

#define MAXCONNECT 6
#define _MAXGROUP  100


typedef struct {
	CString DataName;
	CString FieldName;
	CString KeyName;
	UINT	nID;
	BOOL	bAdvise;
} STDRDSINFO;


typedef struct _S11050_I {
    char jmcode[12];     /* �����ڵ� */
} S11050_I;

/////////////////////////////////////////WMLGT_NEW
typedef struct S07010_T
{
    char    part        [ 1];       /* ���屸�� */
    char    code        [12];       /* �����ڵ� */
    char    price		[ 9];       /* ���簡   */
	char    sign		[ 1];       /* ��� ��ȣ */ 
} S07010_T;

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
	
	char    lendgb          [ 1];			/* 0:�ſ�Ұ� 1:�ſ밡��                */
    char    lendrate        [ 3];			/* ����������� 50,60                   */	
//	char	Name		[ 20];				/* ����� 						*/
//	char	pos			[  2];				/* ����                         */   
//	char	Momentvol	[ 10];				/* ü�ᷮ						*/
//	char	Rights		[  2];				/* �׸���ҿ��� : 4*=�׸����	*/
//	char	HSign		[  1];				/* ���Ѱ���ȣ(����,�ɼ�,�ֿɸ�)	*/
//	char	LSign		[  1];				/* ���Ѱ���ȣ(����,�ɼ�,�ֿɸ�)	*/
//	char	NewsGb		[  2];				/* ���� ���� bit setting		*/
//	char	Gsiyn		[  1];				/* 1���� ���� ����				*/
} ST_INQUIRYDATA; //

enum DATA_TYPE
{
	DT_SIGNAL = 0, 			// 0 : ��ȣó�� - (����:	DT_SHOWSIGN = 0,	// 0 ��Ÿ)
	DT_MANAGER,				// 1 : 	��/��������
	DT_CODEGB ,				// 2 :  �ڵ� ����
	DT_CODE,				// 3 :  �����ڵ� 
	DT_NAME,				// 4 :  �����
	DT_PRICE,				// 5 :  ���簡
	DT_CHANGE,				// 6 :  ���ϴ��(�����)
	DT_RATE,				// 7 :  �����
	DT_SIGN,				// 8 :  �����ȣ
	DT_VOLUME,				// 9 : �����ŷ���
	DT_OFFERHO,				// 10 : �ŵ�ȣ��
	DT_BIDHO,				// 11 : �ż�ȣ��
	DT_OFREMAIN,			// 12 : �ŵ� �ֿ켱�ܷ�
	DT_BIDREMAIN,			// 13 : �ż� �ֿ켱 �ܷ�	
	DT_OFFERTOT,			// 14 : �Ѹŵ� �ܷ�
	DT_BIDTOT,				// 15 : �Ѹż� �ܷ�
	DT_OPEN,				// 16 : �ð� 
	DT_HIGH,				// 17 : ��
	DT_LOW,				    // 18 : ����
	DT_VALUE,				// 19 : �ŷ����
	DT_UPLMTPRICE,			// 20 : ���Ѱ�	
	DT_DNLMTPRICE,		    // 21 : ���Ѱ�
	DT1_OPENYAK,			// 22 : �̰��� ��������
	DT_FACEPRICE,			// 23 : �׸鰡
	DT_YDSIGN,				// 24 : ���� �����ȣ
	DT_YSGUBUN,				// 25 : ���� ����
	DT_YSCOUNT,			    // 26 : ���� �ϼ�
	DT_RECPRICE,			// 27 : ���ذ�
	DT_CVOLUME,				// 28 : ü�ᷮ
	DT_OPEN_CHRATE,			// 29 : �ð� ���� �����
	DT_HIGH_CHRATE,			// 30 : �� ���� �����
	DT_LOW_CHRATE,			// 31 : ���� ���� �����
//////////////////////////////////////////////////////////////
	DT_BUYVALUE,			// 32 : ���Դܰ�
	DT_BUYVOLUME,			// 33 : ���Լ���
	DT_ADDVALUE,			// 34 : ������
	DT_HOWMUCH,				// 35 : ���ͱݾ�
	DT_OFFERTOTHOW,			// 36 : �Ѹŵ��ܷ�����
	DT_BIDTOTHOW,		    // 37 : �Ѹż��ܷ�����
	DT_OFREMAINHOW,			// 38 : �ŵ� �ֿ켱�ܷ�����
	DT_BIDREMAINHOW,		// 39 : �ż� �ֿ켱�ܷ�����	
	DT_CANDLE,				// 40 : CANDLE
	DT_OPEN_CHANGE,			// 41 : �ð� ���� ���
	DT_HIGH_CHANGE,			// 42 : �� ���� ���
	DT_LOW_CHANGE,			// 43 : ���� ���� ���
	DT_LOWTIME,				// 44 : �����ð�
	DT_OFREMAINDEF,			// 45 : �ܷ�����
	DT_OFFERTOTDEF,			// 46 : ���ܷ� ����
	DT_HIGTROWDEF,			// 47 : ������ ����
	DT_HIGHTIME,			// 48 : ���ð�
	DT_EXSIGN,				// 49 : ����ü���ȣ 
	DT_EXPRICE,				// 50 : ����ü�ᰡ
	DT_EXCHANGE,			// 51 : ����ü�� ���
	DT_EXCHANGERATE,		// 52 : ����ü�� �����
	DT_EXVOL,				// 53 : ����ü�� �ŷ���
	DT_REALSIGNAL,			// 54 : ��ȣ��
	DT_LENDGB,				// 55 : �ſ밡�ɿ���
	DT_LENDRATE				// 56 : �ſ�����
};

enum IFYPE
{
	IF_CHANGE = 0,
	IF_PRICE,
	IF_VOLUME,
	IF_RATE,
	IF_OFFERHO,				// 10 : �ŵ�ȣ��
	IF_BIDHO,				// 11 : �ż�ȣ��
	IF_OFREMAIN,			// 12 : �ŵ� �ֿ켱�ܷ�
	IF_BIDREMAIN,			// 13 : �ż� �ֿ켱 �ܷ�	
	IF_OFFERTOT,			// 14 : �Ѹŵ� �ܷ�
	IF_BIDTOT,				// 15 : �Ѹż� �ܷ�
	IF_ADD,					// 17 : ������	
	IF_OPEN,				// 16 : �ð� 
	IF_OPEN_CHRATE,			// 29 : �ð� ���� �����
	IF_HIGH,				// 17 : ��
	IF_HIGH_CHRATE,			// 30 : �� ���� �����
	IF_LOW,				    // 18 : ����
	IF_LOW_CHRATE,			// 31 : ���� ���� �����
};


enum COLOR_INDEX
{
	COLOR_ROW1=0,
	COLOR_ROW2,
	COLOR_REAL,
	COLOR_FOCUS,
	COLOR_KOSPI,
	COLOR_KODOQ,
	COLOR_FUTURE,
	COLOR_OPTION,
	COLOR_HIGH,
	COLOR_EQUAL,
	COLOR_LOW,
	COLOR_GRIDTITLE,
	COLOR_DTNAME
};
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

typedef struct _TR_05030_S
{
    char    time        [ 6];   /* System ����ð�  */
    char    status      [ 6];   /* ���뱸��       */
    char    statContent [40];   /* �屸�м���       */
} TR_05030_S;

typedef struct
{
	char        pWnd[4];
	char        ogubun[2];    /* ���� */
	char        totcnt[4];    /* total Count */
	TR_05021_S  data[1];
} TR_05021_O;

#define KOSPI_RATE		0.15
#define KOSDAQ_RATE		0.15

#define TR_INTEREST  "05010"
#define TR_INTETC	 "05020"
#define TR_INTETC2	 "05021"
#define TR_EXPCHECK	 "05030"
#define TR_INTLOOK	 "99999"
//#define TR_INTEREST "07010"
////////////////////////////////////////WMLGT_NEW 731 START
#define TYPE_ALL			 "0"		//�ڽ���, �ڽ��� ��� 
#define TYPE_KOSPI			 "1"
#define TYPE_KOSDAQ			 "2"

#define GWAN_MAX_GROUP_COUNT	50
#define GWAN_MAX_GRID_COUNT		3
#define GWAN_GROUPMNG_VERSION	1
#define GWAN_GROUPMNG_NEWVERSION	2

#define GWANGROUP_USER			0		// ����� �׷��� 0~49�� ���������� Ȯ���� ����Ͽ� 0~999�� ����
#define GWANGROUP_USER_END		999		// ����� �� �ֵ��� Ʈ������ ���õǴ� �׷��ȣ�� 1000������ �Ѵ�.
#define GWANGROUP_TREE			1000
#define GWANGROUP_ZZIM			1001
#define	GWANGROUP_THEME			1002

#define FIELD_PRICE			 "price"
#define FIELD_SIGN			 "sign"
////////////////////////////////////////WMLGT_NEW 731 END
//#define WM_DBLCLICK_DATA			WM_USER + 1599
//#define WM_GRILCLICK_DATA			WM_USER + 1598


///////////////////////////////////////////////////////
// GRID COLOR INFORMATION START
#define GRID_HEADER_FORCOLOR  RGB(255,255,255)
#define GRID_HEADER_BACKCOLOR RGB(0,0,0)
#define GRID_SELECT_COLOR	  RGB(0,128,128)

#define GRID_STREAM_HIGHCOLOR   RGB(255, 0, 0)
#define GRID_STREAM_LOWCOLOR	RGB(0,255,0)

#define GRID_DATA_HIGHCOLOR   RGB(222, 22, 22)
#define GRID_DATA_LOWCOLOR	  RGB(22, 22, 222)
#define GRID_DATA_EQUALCOLOR  RGB(0, 0, 0)
#define GRID_DATA_IFCOLOR1    RGB(222, 22, 22)
#define GRID_DATA_IFCOLOR2    RGB(222, 128, 22)
#define GRID_DATA_IFCOLOR3    RGB(222, 192, 22)
//#define GRID_DATA_IFCOLOR4    RGB(222, 255, 22)
#define GRID_DATA_IFCOLOR4    RGB(110, 160, 220)

#define STREAM_FORECOLOR RGB(198, 223, 198)
#define STREAM_BACKCOLOR RGB(0,0,0)
#define STREAM_HFORECOLOR RGB(255,255,255)
#define STREAM_HBACKCOLOR RGB(100,100,100)
#define STREAM_HEADERFORECOLOR RGB(255,168,0)

#define GRID_DATA_BACKCOLOR   RGB(255,255,255)

#define GRID_TITLE_BACKCOLOR  RGB( 255, 255, 255)
#define GRID_TITLE_FORECOLOR  RGB( 0, 0, 0)

#define TYPE_DEFAULT 0

#define STATE_IF1   1
#define STATE_IF2   2
#define STATE_IF3   4
#define STATE_IF4   8
#define STATE_IFEND		 2147483648l

#define DEF_COLOR_ROW1   0
#define DEF_COLOR_ROW2   1
#define DEF_COLOR_REAL   2
#define DEF_COLOR_FOCUS  3
#define DEF_COLOR_KOSPI  4
#define DEF_COLOR_KOSDOQ 5
#define DEF_COLOR_OPTION 6
#define DEF_COLOR_FUTURE 7
#define DEF_COLORB_HIGH   8
#define DEF_COLORB_EQUAL  9
#define DEF_COLORB_LOW    10
#define DEF_COLORB_TITLE  11
#define DEF_COLORB_ITEMTITLE  12
#define DEF_COLORB_BORDERLINE 13

#define MAX_ROW_CNT 25

typedef struct _MKT_BOARD_IN
{
     char mastgb[1];            /* �屸�� 1:�ڽ���, 2:�ڽ���, 3:�Ѵ� */
     char signgb[5];            /* ����, ���, ����, ����, �϶� ������ üũ�� ��� �ش�迭 1, �ƴϰ�� 0 */
     char condition[1];         /* ���Ǳ��� 1:and, 2:or */
     char volume[10];            /* �ŷ��� */
     char value[10];               /* �ŷ����(������) */  
	 char price[10];
} MKT_BOARD_IN;


typedef struct _MKT_BOARD_GRID
{
	 char Codegb[1];
     char Sign[1];                /* �����ȣ */
     char shcode[6];           /* �����ڵ� */
     char hname[20];          /* �����(�ѱ�) */
     char Price[9];             /* ���簡 */
     char Change[9];         /* ����� */
     char volume[10];          /* ü�ᷮ */       
} MKT_BOARD_GRID;

typedef struct _MKT_BOARD_OUT
{
	 char pWnd[4];
     char cnt[4];
     MKT_BOARD_GRID grid[1];//MAX_GRID_CNT]; 
} MKT_BOARD_OUT;

typedef struct {
	char    pWnd[4];    
	char	Count[4];
	char	Data1[1];
} SSP_GETGCODE;

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

// RMSG_INTMESSAGE �� LPARAM�� �Ʒ��� ����ü �����Ͱ� ����.
// WPARAM���� �޽����� ���� ���а��� ����.
typedef struct
{
	HWND hWnd;				// �޽����� ���� ������ �ڵ�
	INT_TOTAL * pIntTotal;
	LONG lData;
} INT_MSG_ST;

typedef struct 
{
	HWND hWnd;
	char szCode[15];
	char szMemo[512];
	LOGFONT lf;
} INT_MEMO_ST;

#define FILEPATH_USER	"GwanNew.usr"
#define FILEPATH_DEF	"\\Data\\Gwansim\\Gwan.cfg"
#define FILEPATH_BLINKDEF  "\\Data\\Gwansim\\Gwan_b.cfg"
#define FILEPATH_BLINKUSER  "Gwan_b.usr"
#define FILEPATH_LOOKUSER  "Gwan_L.usr"
#define FILEPATH_BLINKUSER30  "Gwan_b30.usr"
#define FILEPATH_LOOKUSERIF  "Gwan_LIF.usr"
#define FILEPATH_LOOKDEF  "\\Data\\Gwansim\\Gwan_L.cfg"

#include "../Common/LocalUtil.h"
#include <AFXTEMPL.H>
typedef CList<CJMaster,CJMaster> TDEFCODEJ;
typedef CList<CKMaster,CKMaster> TDEFCODEK;
typedef CList<CKUMaster,CKUMaster> TDEFCODEKU;
typedef CList<CGMaster,CGMaster> TDEFCODEG;
typedef CList<CUMaster,CUMaster> TDEFCODEU;
typedef CList<CFMaster,CFMaster> TDEFCODEF;
typedef CList<CPMaster,CPMaster> TDEFCODEP;
typedef CList<CUPMaster,CUPMaster> TDEFCODEUP200;
typedef CList<CUDMaster,CUDMaster> TDEFCODEUD50;
typedef CList<CTHMaster*,CTHMaster*> TDEFCODETH;


const UINT RMSM_RESETGRID = ::RegisterWindowMessage("RMSM_RESETGRID");
const UINT RMSG_GRIDLCLICK = ::RegisterWindowMessage("RMSG_GRIDLCLICK");
const UINT RMSG_GRIDRCLICK = ::RegisterWindowMessage("RMSG_GRIDRCLICK");
const UINT RMSG_GRIDDLCLICK = ::RegisterWindowMessage("RMSG_GRIDDLCLICK");
const UINT RMSG_GRIDHLCLICK = ::RegisterWindowMessage("RMSG_GRIDHLCLICK");
const UINT RMSG_GRIDHRCLICK = ::RegisterWindowMessage("RMSG_GRIDHRCLICK");
const UINT RMSG_GRIDCELLCHANGE = ::RegisterWindowMessage("RMSG_GRIDCELLCHANGE");
const UINT RMSG_GRIDLMOVELINE = ::RegisterWindowMessage(_T("RMSG_GRIDLMOVELINE"));

//2004.12.07 by LYH - Grid �˾��޴�
const UINT RMSG_MENUPOPUP = ::RegisterWindowMessage(_T("RMSG_MENUPOPUP"));
#define BASE_MAINCONTEXTMENU			30000

const UINT RMSG_SHOWITEMINFO = ::RegisterWindowMessage("RMSG_SHOWITEMINFO");
const UINT RMSG_OPENSIGNAL = ::RegisterWindowMessage("RMSG_OPENSIGNAL");

const UINT RMSG_INTSETTING = ::RegisterWindowMessage("RMSG_INTSETTING");
const UINT RMSG_INTMESSAGE = ::RegisterWindowMessage("RMSG_INTMESSAGE");
const UINT RMSG_INTREQUEST = ::RegisterWindowMessage("RMSG_INTREQUEST");
const UINT RMSG_SHARDDATA = ::RegisterWindowMessage("RMSG_SHARDDATA");
const UINT RMSG_CONCERNHWND	= ::RegisterWindowMessage(_T("RMSG_CONCERNHWND"));

const UINT RMSG_GETRECENTCODE = ::RegisterWindowMessage("RMSG_GETRECENTCODE");
const UINT RMSG_MARKETSTATE = ::RegisterWindowMessage("RMSG_MARKETSTATE");

#define NOTIFY_REFLESHROLL  1	// �������⿡ ������Ʈ �� ���� �˸�
#define NOTIFY_RESETALLDATA 3   // ��� ���� ���õ� �Ϳ� ������Ʈ�� ���� �˹�
#define NOTIFY_RESETALLETC1 6   // ���ɿ��� ����� ���� ��û �Ǿ����� ó�� 
                                // ���� ���� ó�� ���ϴ� ����.
//2004.12.02 by LYH
#define NOTIFY_CHANGEDFILE   7   // �������� ���� �����ޱ⸦ �� ���. ���� ���� ���� ó��

#define REQUEST_CHANGEGROUP 11

#define DEF_GRID_CURRENT 1
#define DEF_GRID_CANDLE  2
#define DEF_GRID_STRING  4
#define DEF_GRID_FLOAT   8
#define DEF_GRID_CHECK	 16
#define DEF_GRID_EXP	 32 // ����ü��
#define DEF_GRID_COMMA   64
#define DEF_GRID_IMAGE   128
#define DEF_GRID_SIGN	 256
/////////////////////////////////////////////////////////////
#endif // !defined(AFX_WMUTIL_H__61A0968A_BCC0_4B3C_96F6_9C7AC762ED28__INCLUDED_)
