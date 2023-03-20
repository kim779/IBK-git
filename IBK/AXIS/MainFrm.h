// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__31C95AE9_C26F_4667_8C55_EE4D756EB344__INCLUDED_)
#define AFX_MAINFRM_H__31C95AE9_C26F_4667_8C55_EE4D756EB344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxmt.h>
#include <atlbase.h>
#include "cpuUse.h"
#include "PhonePad.h"
//#include "inca/NpnxMgr.h" //** pc firewall wrapper class definition header (INCA software)
#include "../h/jmcode.h"
#include "../h/axisfire.h"
#include "childFrm.h"
#include "SChild.h"
#include "inca/npenkAppInstall5WIN.h"

#include <vector>
#include <algorithm>

#define USE_AHNLAB_SECUREBROWSER

#ifdef USE_AHNLAB_SECUREBROWSER
// #include "aossdk/aossdkdef.h"
// #pragma comment(lib, "aossdk/aossdk.lib")
// int __stdcall SBSDK_Callback(long lCode, void* pParam, long lParamSize);
#include "ASTx/StSdkExCfg.h"
#include "ASTx/StSdkExCom.h"
#include "ASTx/StSdkExDef.h"
#include "ASTx/StSdkExErr.h"
#pragma comment(lib, "ASTx/StSdkEx.lib")
int __stdcall STSDKEX_EventCallback(long lCode, void* pParam, long lParamSize);
#endif

#define	STSDKEX_MASTER_NAME				_T("astx_setup.exe")
#define	STSDKEX_MASTER_PLATFORM_NT32	_T("nt32")
#define	STSDKEX_MASTER_PLATFORM_X64		_T("x64")

// For Release
#define STSDKEX_MASTER_URL				_T("https://safetx.ahnlab.com/master/win/default")
#define STSDKEX_CUSTOM_POLICY_URL		_T("http://webclinic.ahnlab.com/astx/policy/customer_stsdk_default.html")


#define NCAP_CLOSE		0
#define NCAP_MAX		1
#define	NCAP_RESTORE		2
#define NCAP_MIN		3
#define NCAP_VSCREEN		4
#define NCAP_FUNC		5
#define NCAP_MAXCNT		6

#define CAPTION_HEIGHT 18  //vc2019? test
//#define CAPTION_HEIGHT 21

#define WM_INTISSAVE	WM_USER + 9557
#define WM_RESINTEREST	WM_USER + 9556
#define WM_CLOSE2022	WM_USER + 9555
#define WM_MENU2022		WM_USER + 9444
#define WM_START2022	WM_USER + 9333
#define WM_DLLDATA		WM_USER + 9111
#define WM_ELOG			WM_USER + 9999
#define WM_TRAYEVENT	WM_USER + 9222
#define WM_AXISCLOSE	WM_USER + 9444 
#define	WM_CLOSESM		WM_USER + 702
#define WM_IBK			WM_USER + 747
#define WM_IBK_CODE		WM_USER + 748
#define WM_IBK_CLOSE	WM_USER + 749
#define WM_IBKWINES			WM_USER + 757
#define WM_IBK_CODEWINES		WM_USER + 758
#define WM_IBK_CLOSEWINES	WM_USER + 759
#define WM_IBK_NEWSWINES	WM_USER + 760
#define WM_IBK_KIUPWINES	WM_USER + 761
#define WM_INTERMSG	WM_USER+2022

#define WD_ADD_CODE		(WM_USER + 0x3000)
#define WD_CLOSE_CODE	(WM_USER + 0x3001)
#define WD_SETWND		(WM_USER + 0x3009)
#define WD_MINICLOSE	(WM_USER + 0x300A)
#define WD_HTSCLOSEFORMINI	(WM_USER + 0x300B)
#define WX_HOTTRADE			(WM_USER+0x4004)
#define WD_TRANSMAX			(WM_USER+0x4005)
#define WD_TRANSMIN			(WM_USER+0x4006)
#define WD_HISTORYVIEW		(WM_USER+0x5001)
#define WD_HISTORYCLOSE		(WM_USER+0x5002)
#define WD_ISDEV			(WM_USER+0x5003)
#define WD_STAFF			(WM_USER+0x5004)
#define WD_WINE				(WM_USER+0x5005)
#define WD_WINEWND			(WM_USER+0x5006)
#define WD_CHART			(WM_USER+0x5007)
#define WD_FOCUSNOTICE		(WM_USER+0x5008)
#define WD_WEBHWND			(WM_USER+0x500A)
#define WD_CLOSEWEB			(WM_USER+0x500B)
#define WD_WINESWND			(WM_USER+0x5009)
#define WD_WARNING			(WM_USER+0x6001)
#define WD_GOODORDER		(WM_USER+0x6002)
#define WD_PHONEPAD			(WM_USER+0x7001)
#define WD_MINIMIZED		(WM_USER+0x7002)
#define WD_TOP10			(WM_USER+0x8001)
#define WD_LOCKPASS			(WM_USER+0x8002)


#define	TRIGGERN		1

#define	SEP				0x7f
#define	USERSEP			'\t'

#define	INSERTID	"@ID@"			// �޴����� webȭ�� ���°�� �̽ɺ��� ������ userID�� ��������

#define	MENU_NOT		0x00
#define	MENU_NONE		0x01
#define	MENU_NORMAL		0x02
#define	MENU_SCROLL		0x04
#define	MENU_TABVIEW	0x08

#define	TM_AUTOHIDE		9000
#define	TM_AUTOMSN		9001
#define	TM_IDLE			9009
#define	TM_PURL			9010
#define TM_AUTOMNGINFO	9011
#define TM_PRNIMG		9012
#define TM_AUTODCOTOR	9013
#define TM_RTSQUEUE		9015			// RTS queue check(1000 ms)
#define	TM_SCRLOG		9016			// ȭ�� �α� 20070627 kwon
#define	TM_TEST			9018			// �׽�Ʈ�� timer
#define TM_INITSIZE		9030
#define TM_NOTICE		9040            //����
#define TM_HISTORY		9060			//�����丮
#define TM_ITGY			9910
#define TM_WINES_KIUP	9911
#define TM_NPROTECT_ALIVE 9912			//nProtect �⵿ ���� Ȯ��
#define TM_AOS_ALIVE	9913			//AOS ��⵿
#define TM_KINGS_ALIVE	9914			//Ű���� ���� ��⵿
#define TM_POPUP_JISU   9019		    //�ؿ�����	
#define TM_TOP10_2018	9021
#define TM_2022_CLOSE	9022
#define TM_CB_SEARCH	9023			//��Ŷ �극��ũ

#define	COLOR_TB		RGB(238, 238, 238)

#define	UMENUN			8
#define USERTOOLMENU	9

#define USERDEFSYM		"USRDEFDATA"

#define KEY_CODE		0xff-1
#define KEY_STOPLOSS	0xff-2	
#define KEY_ACCTOOL		0xff-3
#define KEY_NEWSVIEWER	0xff-4
#define KEY_FOSTOPLOSS	0xff-5

#define MODE_MDI		1
#define MODE_SDI		2
#define MODE_TRAY		3

#define	BKMODE_NORMAL	0
#define	BKMODE_CHANGE	1

#define TRAY_SHOWICON	0
#define TRAY_HIDEICON	1

#define TOOLSTATUS_MDI	TB_MAINBAR|TB_LISTBAR
#define TOOLSTATUS_SDI	TB_SDIBAR|TB_SDIMAINBAR|TB_LISTBAR

#define MAX_XBMP		1020
#define MAX_YBMP		723

#define	GAP				2
#define SRCMASK			0x00220326    // mask

// 2007.10 �ܱⰳ�������� �׵θ��� 2px ����
#define	XGAP			2//GetSystemMetrics(SM_CXFRAME)
#define	YGAP			2//GetSystemMetrics(SM_CYFRAME)

#define MAPN_REALTIMEJANGO		"IBXXXX01"		// �ǽð��ܰ�
#define MAPN_ACCTGROUP			"IBXXXX02"		// �׷���°���
#define MAPN_STOPLOSS			"IBXXXX03"		// ��ž�ν�
#define MAPN_ACCTTOOLBAR		"IBXXXX04"		// ��������
#define MAPN_ACCTGROUP1			"IBXXXX05"		// �׷���°���1
#define MAPN_ACCTGROUP2			"IBXXXX06"		// �׷���°���2
#define MAPN_SISECATCH			"IBXXXX08"		// �ü�����
#define MAPN_SISECATCH1			"IBXXXX09"		// �ü�����(background)
#define MAPN_FOSTOPLOSS			"IB115400"		// �����ɼ� ��ž�ν�
#define MAPN_GONGJI				"IB600000"		// ����
#define MAPN_ACCTCONFIG			"IB0000X1"		// ���¼���
#define MAPN_LINKEXCEL			"IB0000X2"		// ������ũ
#define MAPN_KOBAELW			"IB281500"		// ��������ELW ��ȸ ȭ��
#define MAPN_KOBAELW_SCREEN		"IB280200"		// ELW���簡 ȭ��
#define MAPN_LOGINSET			"IB0000AA"		// LOGIN.XXX -> Push���ִ� ��
#define MAPN_MINIWID			"IB0000X8"		// �̴� �������� ����

#define MAPN_MULTICONNECT		"DH621600"		// �������� ����

#define SYM_MNG			"XXXXX"
#define MNG_KIND		"047"
#define MSG_MGUBN		"600"
#define MNG_MSG			"023"
#define MNG_FLAG		"601"
#define NO_CHECK		200
#define MNG_KOBAELW		"196"
#define MNG_INFO_KOBAELW "0601196"

#define	MAXSOUND	7
#define IDX_MDORDER	0
#define IDX_MSORDER	1
#define IDX_JJORDER	2
#define IDX_CSORDER	3
#define IDX_MDCON	4
#define IDX_MSCON	5
#define IDX_REFUSAL	6

#define MSLIST_FILE		"mslist.dat"

// �ǽð� �ʹݿ� 2011.01.26 by warship -----------------------------------------------------------------
struct rexp_item
{
    char type[3];       /* MAP, DEV, EXE, TAB, MTB */
    char path[64];
    char size[8];
};

struct rexp_mid
{   
    char    nrec[4]; 
    struct  rexp_item item[1];
};  

struct pibotran_mid {
    char seqn[6];
    char start[8];
    char file[64];
};

struct pibotran_mod {
    char seqn[6];
    char max_seqn[6];
    char start[8];
    char size[8];
    char data[1024*32];
};
// -----------------------------------------------------------------------------------------------------
// ��������Ʈ ���

// -----------------------------------------------------------------------------------------------------

struct _inters
{
	char	gubn[1];	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	name[32];	// �����
	char	xprc[10];	// �����ܰ�
	char	xnum[10];	// ��������
	char	xupnum[16];	// ���� �ֽļ�
	char	filler[24];	// Reserved
};
#define	sz_inters	sizeof(struct _inters)

struct	_bookmarkinfo {
	char	gubn[1];		// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];		// �����ڵ�
	char	name[32];		// �����
	char	bookmark[1];	// �ϸ�ũ ����
};

#define	sz_bookmark	sizeof(struct _bookmarkinfo)

// �ֹ����� ���ε�
struct _pidouini_item {
	char	usid[8  ];				// ����� ID
	char	innm[100];				// INI ���ϸ�
	char	senm[100];				// ���Ǹ�
	char	skey[100];				// KEY
	char	valu[2000];				// VALUE
	char	date[8  ];				// ������Ʈ ����
};

struct _pidouini_mid {
	char	gubn[1 ];				//���� 'Q': ��ȸ, 'I': �Է�
	struct	_pidouini_item item; 
};

struct  _pidouini_aid{
	char    gubn        [ 1];           /*  ����('A')       */
	char    nrec        [ 2];           /*  COUNT           */
	struct  _pidouini_mid     mid [16];  /*  MID             */
};

#define	sz_pidouini	sizeof(struct _pidouini_mid)

//
//	�������� UPLOAD / DOWNLOAD
//
struct	_uinfo {
	char	gubn[2];	// ��������
				// "MY" : ��������
				// "UL" : ����� ������ ��� 
				// "UD" : ����� ������
	char	dirt[1];	// ���۹��� 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : ����ȸ
	char	cont[1];	// ���ӱ��� 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel
	char	name[80];	// file name
	char	nblc[5];	// ������, ����ũ��(max:64k)
	char	retc[1];	// return value 'O':���� 'E':����
	char	emsg[40];	// error message
};

struct	_ginfo {
	char	gnox[2];	// group number
	char	gnam[20];	// group name
	char	jrec[4];	// _jinfo count
	//	char	jptr[1];	// _jinfo pointer
};

struct	_jinfo {
	char	gubn[1];	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	xprc[10];	// �����ܰ�
	char	xnum[10];	// ��������
};

struct	_updn {
	_uinfo	uinfo;
	_ginfo	ginfo;
};

const int sz_uinfo   = sizeof(struct _uinfo);
const int sz_ginfo   = sizeof(struct _ginfo);
const int sz_jinfo   = sizeof(struct _jinfo);
const int sz_updn    = sizeof(struct _updn);

struct _shjcode			// semi hjcode
{
	CString code;
	CString name;
	CString symb;		// symbol
	char	ecng;		// ECN ����
	char	size;		// �ں��ݱԸ�	2:��, 3:��, 4:��
	char	ucdm;		// �����ߺз�
	char	ucds;		// �����Һз�
	char	jjug;		// ������ ����(27: ������)
	char	kpgb;		// KOSPI200 ����
	char	kosd;		// ���񱸺�
	char	ssgb;		// �Ҽӱ���
	char	ucmd;		// 0:������, 5:�켱��
	char	wsgb;		// 0:�Ϲ� 1:���豸��������
	char	jsiz;		// �����ں��� ���߼�
	char	itgb;		// KOSPI-IT/KQ-IT50����(1:ä��)
	char	star;		// KOSDAQ ��Ÿ����
	char	prmr;		// �����̾�����
	char	unio;		// �������� (KRX100)
};

struct _sjcode			// semi code
{
	CString code;
	CString name;
};

//2013.7.30 �����
//���Ἲ ����
struct  pihoitgy_grid
{	
    char    gubn[3];            /* ����                 */	
	/* ex) exe, dev, map    */	
    char    mnam[47];           /* ����               */	
};

struct  pihoitgy_grid2
{
    char    gubn[3];            /* ����                     */
	/* ex) exe, dev, map        */
    char    mnam[47];           /* ����                   */	
    char    skey[44];           /* Ŭ���̾�Ʈ SHA256 ���   */
};  

struct  pihoitgy_mid 
{	
    char    gubn[1];            /*  1: LIST ��û    2: ���Ἲ ����  */	
    char    nrec[4];            /* ���Ἲ ������ ��û ����          */	
    struct  pihoitgy_grid2   grid2[1];	
};

struct  pihoitgy_mod 
{	
    char    nrec[4];            /*  ����            */	
    struct  pihoitgy_grid    grid[1];    	
};

struct pibfstup_grid
{
	char utyp[1];    //0: ALL 1: STAFF 2:CUSTOMER  3:SPECIFIC USER
	char uid[100];
	char type[1];    //0: FILE 1:SECTION 2:KEY
	char fnam[128];
	char sect[32];
	char skey[32];
};

struct pibfstup_mod
{
	char nrec[4];
	struct pibfstup_grid grid[1];
};

struct   enca_mid {
	char    gubn[1];    // S:����, E:����   
	char    dnxx[256];  // dn�� 
};


 struct   enca_mod {
	char    ret[1]; // ó������ : 1 ó������ : 0
	char    cnt[1]; // ����Ƚ��
};

//�ż� TOP10
struct pibo2018_mid
{
	char gubn[1]; 
	char date[8]; 
};

struct pibo2018_grid
{
	char cod2[12];
	char hnam[40];
	char curr[8];
};

struct pibo2018_mod
{
	char ncnt[4];
	struct pibo2018_grid grid[1];
};

struct pibo2018_grid2
{
	char cod2[12];
	char hnam[20];
	char rchg[5];
	char isnew[1];
};

struct pibo2018_mod2
{
	char ncnt[4];
	struct pibo2018_grid2 grid[1];
};

struct SBPGT336_mid //���� ���� ���� ����
{
	char In[5];
	char uID[16];
	char zTrxTp[1];
};

struct SBPGT336_mod
{
	char InOut[22];
	char rcd[5];
	char psRtyCnt[2]; //��õ� ����Ƚ��
};


void WriteLog(LPCSTR log, ...);
bool axiscall(int, WPARAM, LPARAM);

class CMapHelper;
#include "useMaindefine.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

protected:
	void SplitStr(LPCSTR st, LPCSTR ed, CHAR del, std::vector<CString> *pResult);
	void SplitStr(CString *data, CHAR del, std::vector<CString> *pResult);

// Attributes
public:
	CString		m_mapN;
	CString		m_regkey;
	
	CWnd*		m_category;
	CChildFrame*		m_miniWid;
	CSChild*		m_miniSWid;

	int		m_rndkey;
	int		m_idletime;
	int		m_helpKey;
	bool		m_block;
	bool		m_blinkbar;
	bool		m_saveALLVS;		// save option userworkspace
	bool		m_bCustomer;
	bool		m_bSwitch;

	HICON		m_dtIcon;
	HHOOK		m_hHook;
	bool		m_bdnInterest{};
#ifdef DF_USE_CPLUS17
	std::unique_ptr<class	CAxMisc>	m_axMisc{};
	std::unique_ptr<CWnd>	m_wizard{};
#else
	class	CAxMisc*	m_axMisc;
	CWnd* m_wizard;
#endif
	CRect	m_TOP10Rect;
	bool	m_b2018;
	int	    m_n2018Target;
	bool		m_bLoadScreen;

	CMapStringToString m_groupBymapN;

// view hook
	CView*		m_view;
	FARPROC		m_callproc;
	CArray	<int, int>	m_waitlist;
protected:
	CWinApp* m_axis;
	char	m_progK;
	char	m_classN[256];

	CString	m_pass;
	CString	m_cpass;
	CString	m_MMapN;
	CString	m_waveF[7];
	CString	m_alarmMsg;
	CString	m_printS;
	CString	m_activeMapN;
	CString	m_elogfileName;
	CString	m_reportCaption;
	CString m_forceIP;
	
	CString	m_ip;
	CString	m_port;
	CString m_macaddr;

	bool	m_update;
	bool	m_fullmode;
	bool	m_bExit;
	bool	m_forceClose;
	bool	m_runAxis;
	bool	m_bEnca;
	bool	m_noActive;
	bool	m_fit;
	bool	m_bSDI;	
	BOOL	m_bHome;
	BOOL	m_bEnd;
	bool	m_bReconnect;				// �ٸ� ID�� ������, 20070117
	BOOL	m_activeCap;
	BOOL	m_bOnlySise;
	BOOL	m_bHistory;
	BOOL	m_bClose;
	BOOL	m_bItgy;
	int		m_nKiupKey;
	CString	m_sKiupDomino;

	// hotkey
	BOOL    m_bUseHideHotkey;

	DWORD	m_toolStatus;
	DWORD	m_conKIND;
	int		m_conHISTORY;
	int		m_conAUTOTIME;
	int		m_conPOS;
	int		m_activeKey;
	int		m_BackGroundKey;
	
	//** ü������ ���û���
	BOOL	m_matchToolBar;
	int		m_matchToolCount;
	
	int		m_SDIHEIGHT;
	int		m_maxChild;
	int		m_fontSize;
	int		m_posChild;
	UINT	m_appMode;

	int m_screenNew;
	int m_titleChange;
	CString m_userTitle;
	BOOL	m_bMiniMode;

	int	m_vsN;
	int	m_connectBy;
	int	m_downI;
	int	m_action;
	int m_nInterest;//���� ��������ȭ�� ��

	BOOL	m_bInit;

	HWND m_winechartHwnd;
	HWND m_wineSchartHwnd;

	//�������� �ؾ��ϴ��� ����
	BOOL	m_bMustRetry;

	CString m_slideMsg;
	CView* m_viewHist;

	int		m_n2022Cnt;

	bool	m_bCertLogin;
	bool	m_bUseNewLogin;

	CString	m_strDN;//������ DN

#ifdef DF_USE_CPLUS17
	std::unique_ptr<class CAxGuide> m_axGuide{};
	std::unique_ptr<class CTTip> m_tip{};
	std::unique_ptr<class CDtconnect> m_axConnectOld{};
	std::unique_ptr<class CCertLogin> m_axConnect{};
	std::unique_ptr<class CLogoDlg> m_logodlg{};
	std::unique_ptr<class CBmpButton> m_bitmapBtn{};
	std::unique_ptr<class CSdibar> m_sdibar{};
	std::unique_ptr<class CTMenu> m_tMenu{};
	std::unique_ptr<class CDbar1> m_bar1{};
	std::unique_ptr<class CSmain> m_smain{};
	std::unique_ptr<class CTInfo2> m_tInfo2{};
	std::unique_ptr<class CTInfo1> m_tInfo1{};
	std::unique_ptr<class CDbar2> m_bar2{};
	std::unique_ptr<class CDbar3> m_bar3{};
	std::unique_ptr<class CTInfo3> m_tInfo3{};
	std::unique_ptr<class CCoolDialogBar> m_bar0{};
	std::unique_ptr<class CEmpPassChangeDlg> m_EmpPassChangeDlg{};
	std::unique_ptr<class CConclusion> m_conclusion{};
	std::unique_ptr<class CKobaElwNotify> m_kobanotify{};
	std::unique_ptr<class CEConclusion> m_Econclusion{};
	std::unique_ptr<class CNClock> m_Nclock{};
	std::unique_ptr<class CChaser> m_chaser{};
	std::unique_ptr<class CInfofile> m_infofile{};
#else
	class CAxGuide* m_axGuide;
	class CTTip* m_tip;
	class CDtconnect* m_axConnectOld;
	class CCertLogin* m_axConnect;
	class CLogoDlg* m_logodlg;
	class CBmpButton* m_bitmapBtn;
	class CSdibar* m_sdibar;
	class CTMenu* m_tMenu;
	class CDbar1* m_bar1;
	class CSmain* m_smain;
	class CTInfo2* m_tInfo2;
	class CTInfo1* m_tInfo1;
	class CDbar2* m_bar2;
	class CDbar3* m_bar3;
	class CTInfo3* m_tInfo3;
	class CCoolDialogBar* m_bar0;
	class CEmpPassChangeDlg* m_EmpPassChangeDlg;
	class CConclusion* m_conclusion;
	class CKobaElwNotify* m_kobanotify;
	class CEConclusion* m_Econclusion;
	class CNClock* m_Nclock;
	class CChaser* m_chaser;
	class CInfofile* m_infofile;
#endif
	
	char*	m_wb;
	SYSTEMTIME		m_connectT;
	class CMPop*		m_modal;
	class CAcntDialogBar*	m_TotalAcc;
	class CCodebar*		m_codebar;
	class CPrinterSettings* m_printsetup;

	class CSmcall*		m_smcall;
	class CNPalette*	m_palette;
	class CAccTool*		m_accTool;
	class CAccSave*		m_accSave;
	class CNewsViewer*	m_newsviewer;
	class CStoploss*	m_stoploss;
	class CStoploss*	m_foStoploss;
	class CInfo_Interest*	m_info_interest;
	

//	custom toolbar

	CMapStringToString	m_tabviewlist;
	CMapStringToString	m_mapExpectSymbol;	// 2006.7.24 ����ȣ => �������� �ɺ� ���̺�
	CStringArray		m_savelist;	
	CStringArray		m_sfilelist;
	CStringArray		m_removelist;
	//2012.08.17 ����� - ȭ������Ʈ�� ������Ʈ�� ���� ����
	CStringArray		m_screenHistory;

	//2013.07.30 ����� - ���Ἲ ���ϸ���Ʈ
	CStringArray		m_arrayItgy;
	CStringArray		m_arrayPlfItgy;

	CMap	<int, int, class CChildFrame*, class CChildFrame*>	m_arMDI[6];
	CMap	<int, int, class CSChild*, class CSChild*>		m_arSDI[6];
	CMap	<int, int, class CGPop*, class CGPop*>			m_arGPOP;
	CMap	<int, int, class CMPop*, class CMPop*>			m_arMPOP;

	CArray	<class CChildFrame*, class CChildFrame* >		m_arHide;
	CArray	<int, int>	m_hooklist[6];
	CArray	<int, int>	m_sdiZORDER[6];
	CArray	<HWND, HWND>	m_arWEB;
	CArray  <struct pibfstup_grid,struct pibfstup_grid>	m_arStup;
	//CArray	<int, int>	m_waitlist;
	enum	{axNONE, axOPEN, axOPENPPP, axOPENRSM, axOPENWSH, axOPENSIGN, axDONE, axCLOSE} m_step;

	struct	IWebBrowserApp* m_pWBApp;
	HWND	m_WBWnd;

	HICON			m_hIcon;
	NOTIFYICONDATA		m_tray;
	CArray	< CRect, CRect > m_arRc;	// button rect

	HINSTANCE		m_hMRadar{}, m_hMNews{}, m_hDRFN{}, m_hSHA256{};

	CCriticalSection	m_sync;
	CRect			m_minSaveRC;
	BOOL	m_bSdkInitialized;
	CString m_strCustomerID;
	

	//�˾���ũ
	int m_ShowSlide;

	//ȭ�� �ΰ� �˾� ����, laststate���� ����
	void LoadNoTwoPOP_NoSaveLast();
	CStringArray m_arNoTwoPop;
	CStringArray m_arNoSaveLastmap;
	bool IsNoTwopop(CString strmap);
	bool IsNoSaveLastmap(CString strmap);

	//���������� ������� 
	int  m_iCErrCnt;  //���� Ƚ��
	void SendSBPGT336(CString sData);
	void ParseSBPGT336(char* pdata, int len);

	//Ʈ����
	void GoToTray();
	void BackFromTray();

	//�����Ϸ� ������Ʈ ����Ȯ��
	void Check_HTS_Verstion();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	class CMDIClient*	m_MClient;
	WINDOWPLACEMENT		m_pl, m_switchpl;
// Generated message map functions
public:
	void ClearRepositoryLog();
	BOOL ClearGex();

	BOOL ChangeTabView(LPCSTR mapN, int key);


	CGPop*	getGPOP( CString strMapName );

	int		GetTheme();
	BOOL    IsSuperUser();
	void	SetLastMaps(CString sRemoveMap, CString sInsertMap);
	void	checkDelayList();
	void	WriteFileData(CString sPath, CString sData, BOOL bCreate = FALSE);
	void	MDINextChild();
	void	RelayoutTicker();
	BOOL	IsNewExitImage();
	void	FOStopLoss();
	bool	IsAgreeDuplicate(CString mapN);
	void	VoidRect(CRect *pRect);
	BOOL	IsNewLoginImage(BOOL bStaff);
	CRect	GetFullRect();
	void	show_Ticker(int nID);
	void	hide_Ticker(int nID);
	void	ErrReport(CString eMsg);
	bool	Start(CString user);
	bool	GetScrButtonPos(int key, LPRECT lpRect);
	void	closeMapByName( CString strName );

	void	Setfocus_Child(int key,bool bVS = FALSE);
	void	show_Newslist(WPARAM wParam, LPARAM lParam);
	void	ShowNews();
	void	change_Skin(/*CString skinN*/);
	void	change_VirtualScreen(int index);
	void	set_Trigger(int key, int triggerN);
	void	set_Trigger(int key, int triggerN, int vsN);
	void	set_STrigger(int key, int triggerN, int vsN);
	void	restore(int key);
	void	SetConclusion();
	void	ShowConclusion();
	void	ShowEConclusion(CString dat, bool bFail = false);	
	BOOL	ConclusionNotice(CString dat, CString& title);
	BOOL	HideGuide();
	
	bool	select_SName(CString input);
	bool	HotKey(WPARAM wParam, LPARAM lParam);
	void	process_ConSee(int option);
	void	funcKey(int funcID, int index = -1);
	int		CheckMapping(CString mapN);
	void	childAll(CString dat);
	void	HelpLink(int key);
	void	HelpLinkS(int key);

	void	view_Hook(int key);
	void	view_UnHook();
	void	SmCall(int idx);
	void	AppendAllMap();
	void	sendTicInfo();

	void	GetDispN(char* mapN);
	int		InputScreenNo(CString dispN);

	void	EditTool();
	void	SetTicker();
	BOOL	Change_VsKey(int index);
	void	ConfigTicker(bool reconfig = false);
	void	Action(WPARAM wParam, LPARAM lParam);
	void	GetCaptionInfo(WPARAM wParam, LPARAM lParam);
	void	HideChildButton(int key);
	void	SettingEticker(int paneID);
	CString	GetSkinName();
	void	convertSDI(int key, int vsN);
	void	convertMDI(int key, int vsN);
	void	addScreenList(CString mapN, int key, int vsN);
	bool	FindSDIMap(CString mapName, int vsN =-1);
	void	AppSwitch();
	void	ChangeMDI();
	void	ChangeSDI(bool hide = false);
	void	ChangeTRAY();
	void	TrayCmd(int cmd);
	void	displayGuide(CString guide, CWnd* owner = NULL);
	void	displayMsgBox(CString caption, CString guide);
	void	SetHome();
	BOOL	TransKey(int keyboard);

	void	LinkOPEN(int dat, CString mapN);
	void	parseAry(CString data, CStringArray& ary, CString separate);
	CWnd*	refleshMap(int key);
	void	StopLoss();
	void	RunHelpCom();

	void	DeleteAllScreen(bool all = true);	// cej
	void	RunBrowser(CString url);		// 20070503 cej

	void	RecalcFrame();
	void	SendTrigger(CString code);
	void	deleteHistory(CString code);
	void	deleteAllHistory();
	void	HideHistoryBar();
	void	ShowHistoryMap(WPARAM wParam,LPARAM lParam);
	void	CloseHistoryMap(WPARAM wParam,LPARAM lParam);

	void	QueryPihoitgyList();
	void	ParsePihoitgyList(char* dat, int len);
	void	QueryPihoitgy();
	void	ParsePihoitgy(char* dat, int len);
	void	UpdateInfo(bool rsc);
	CString EncryptAES(CString enc);
	CString DecryptAES(CString dec);
	void    CloseWebHandle();
	void	ClearUserIni();
	void	UserFileCrypt(bool bFlag);
	void	ScrapInformation();
	void	ParseScrap(char* dat, int len);
	void	SendScrap();
	void	Parse2018(char* dat, int len);
	void	ParseEnca(char* dat,int len);
	void	Send2018();
	void	SendEnca(CString sDN,bool bSuccess);

protected:
	// �ǽð� �ݿ� ��� �߰� 2011.01.26 by warship
	std::vector<struct rexp_item> m_rexp;
	void ProcessRexp(struct rexp_mid *mid);
	void StartRexp();
	void processTran(char *data, int dlen);

protected:
	int		Initialize();
	void    LoadHotkeySetting();
	void    FreeHotkeySetting();
	void	ConfigFrame();
	void	DrawFrame();
	void	DrawFrame(CDC* dc);
	void	DrawBorder(CDC* dc);
	void	ProcessInitMap();
	void	UnregisterControl();
	void	registerControl();
	void	signOn();
	void	signOnCert();
	void	endWorkstation();
	void	CreateTB();
	void	CallRecommendedBoard();

	bool	runCommand(int comm, LPARAM lParam);
	bool	Agree_Duplication(CString mapN,bool bDup = false);

	int		create_Newview(int actkey, char* data);
	void	setTitle(int key, CString title);
	void	SetSCN(CWnd* wnd, int key, CString mapN, CString title, bool bSDI = false);
	void	loadMap(int key, int size);
	void	showfname();
	void	changeSize(int key, CSize size);
	void	firstTabOrder();

	void	positionWindow(int actkey, int poskey, int pos, bool child = true);
	void	positionWindow(int actkey, CWnd* posW, int pos, bool child = true);
	void	positionWindow(CWnd* actW, CWnd* posW, int pos, bool child = true);

	BOOL	getConnectInfo(CString& ips, int& port);
	void	update_ticker(int kind, struct _alertR* alertR);
	void	change_BackGround();

	bool	sendTR(char* datB, int datL);

	BOOL	CheckMasterFile();

	char*	GetSHA256(char* data,int dataL,HMODULE hModule,bool isfile = false);
	char*	GetFileSHA256(CString file,HMODULE hModule);
	CString	IsWow64();
	bool	isIPInRange(CString ip,CString network);
	unsigned int IPToUInt(CString ip);
	CString SplitString(CString &strData, CString strToken);
	CString Err2Str(DWORD dwErrCode);
	BOOL	installASTx();
	BOOL	initASTx();
	BOOL	uninitASTx();
	BOOL	startPB();
	BOOL	stopPB();
	BOOL	startFW();
	BOOL	stopFW();
	BOOL	startAK();
	BOOL	stopAK();
	BOOL	CheckBrowserVersion();

public:
	bool	sendTR(CString trN, char* datB, int datL, BYTE stat = 0, int key = 0);

	//�˾���ũ ����
	void	HidePBArrItem(CWnd* rect);
	CRect	getPBArrRect(CWnd* pdlg, int iwidth, int iheight);
	void	OpenPBNews(CString strdata = "");

	//20191212 ASTx
	DWORD   m_DInstallASTx;
	void    GetASTxInstall();

	//202003 CB �ߵ��� �˾� //HTS ������ ����TR��ȸ �ϸ� �������� ������
	//void	SendCBSerachTR();
	CSize	GetFrameGap(bool mdichild = true);
protected:
	//**void	AccSetting();
	void	LockProg();
	void	fullScreenMode(bool full = true);

	void	createUserScreen(int key);
	void	CreateHistoryBar();
	void	createUserScreen(CString mapN, bool allVS = true);
	void	saveUserScreen();
	void	makeUserScreenMenu();
	bool	modifyWorkspace(bool add, CString keys, bool allvs = true);
	void	save_laststat();
	void	getScreenData(int key, CString& data, BOOL save = TRUE);
	void	loadDomino(int key, CString mapN, bool bSDI = false);
	int		ExceptionProcess(CString mapN);
	void	MRFrame(CChildFrame* child);

	void	saveImg(int entire = 0);
	BOOL	printImg(CString dat = _T(""));
	BOOL	writeImg(CString dir, int entire = 0);
	BOOL	writeDIB(LPTSTR szFile, HANDLE hDIB);
	HANDLE	writeDDBToDIB(const CBitmap& bitmap, DWORD dwCompression, CPalette* pPal);

	bool	IsSelectMap(CString mapN);
	CString	getSMap(CString mapN);
	LPCTSTR	getPrintName();
	LPDEVMODE getPrintMode();

	void	setUDP();
	void	beginWait(int key = -1);
	void	endWait(int key = -1);
	void	removeWait(int key);
	void	setCodeReg(int cnt, CString dat);
	void	ChangeUser();
	
	void	RunWebBrowser(CString HomePage, int cx = 780, int cy = 538);
	void	ExitWebBrowser();
	bool	changeMAX(CString mapN);
	void	load_eninfomation(bool first = true);
	void	load_mngSetup();
	void	load_hkey();
	void	load_start_notice(BOOL bReload = false);
	void    load_dbar2();

	//UPDATE AGENT ����
	void	KillUpdateAgent();

	void	checkFirewall();
	void    LoadSecureTools();
	void	load_secure_agree(BOOL bAOS, BOOL bFirewall, BOOL bKeysecure);
	void	trouble_shooting(CString strErrMsg,CString sKey);
	void	SendInstallPath();
	void	SendProcessList(); // Process list
	void	os_report();		// OS version reporting on start
	void	ini_report(LPCSTR innm, LPCSTR senm, LPCSTR skey, LPCSTR valu);
	void	mac_report();		// Ethernet mac-address reporting on start
	void	scr_counter_report();		// Opened map reporting on close
	void    load_scr_counter();	
	void    save_scr_counter();

	void	processSecureTool(char *data, int dlen);
	void	maxChild();
	void	detachChild(int key);
	void	saveExitMap();
	CWnd*	getActiveView(int key);
	void	load_history();
	void	save_history();
	void	add_history(CString mapN);
	void	addTool_history(CString mapName);
	void	load_tabview();
	bool	IsTabView(CString mapN);
	bool	IsTabChange(CString mapN, int &key);
	CString	GetTabKey(CString mapN);
	void	change_Resource();
	BOOL	ExistMenu(CString mapN);
	void	SetAMap(int key, char* mapN);
	void	SetSearchMap(char* mapN);
	void	NoticeParse(CString dat, CMapStringToString& ary);
	CString	parseData(CString dat);

	CView*	GetNewView(int vwKIND);
	CView*	GetSDIView(int vwKIND);
	
	void	toolRegister();
	void	toolRegisterAll(int index);
	void	AllScreenRegister();
	void	changeFontSize();
	void	processFMX(WPARAM wParam, LPARAM lParam);
	void	processTICK(CString dat);
	void	processETICK(CString dat);
	void	processINTER(char* dat);
	void	processELOG(char* dat);
	void	processNEWS(char* dat, int len);
	void	PopupWeb(CString url, int cx = 780, int cy = 538);
	void	ParseSym(CString dat, CMapStringToString& ary);
	void	actionCaption(int key, int action);
	void	actionSCaption(int key, int action);
	void	CreateChaser();
	void	DeleteChaser();
	void	CloseChaserAPP();
	void	showChaser();
	CString Parser(CString &srcstr, CString substr);
	void	WriteFile(char* pBytes, int nBytes);
	void	WriteErrFile(char* pBytes, int nBytes);
	void	write_err();
	CPoint	getMinimizePos(int childKey);
	bool	axCloseV(int key);
	bool	axFocus(int key);

	void	ProcessInfofile(bool upload = true);
	void	changePassword(CString newpassword);
	void	ShowUScreenMenu();
	void	loadToolStatus();
	void	saveToolStatus();
	void	SetToolStatus();
	void	SetSDIHeight();
	void	SetSDIChangeHeight();
	void	appendUserToolMenu();
	void	runProgram(int index);
	void	preload_screen();
	CChildFrame*	load_hidescreen(CString mapname);
	void	displayChild(int vsN, bool show = true, bool bSDI = false);
	void	keyArray(CArray <int, int> &ary, int vsN, bool bSDI = false);

	void	drawTitle(CDC* pDC);
	CRect	GetCapRect();
	void	SetRegion();
	int		DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown = false, int maskcolor = Axis::maskColor);
	int		GetSelectItem(CPoint pt);
	void	ShowToolTip(int nIndex, CPoint pt);
	CString	GetConnectIP(CString dns);
	void	fitDual();
	int		GetTicKey(int index);
	int		ConvertIndexKey(int index);
	void	convertIndex();
	void	deleteNewsfile();
	void	OpenAnnouncement();
	bool	isOpenAnnouncement(CString mapName);
	bool	ExceptMap(CString mapN);
	int		GetMacAddr(char* ipaddr, char* data);
	void	SendCPUInfo();
	void	SendFile(CString file);
	void    RunTOP10();

/////////////////////////////////////////////////////////////////////////////////
	// 2006.7.24 ����ȣ => ���������� ���� �ɺ����̺�
	void	MakeExpectSymbolTable();	
	CString	ReplaceExpectSymbol(CString sym);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
	// 2006.9.14 ����ȣ => �ϳ�/���� ��ȣȭ ��� ����
	void	SetEncriptFile();
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
	// 2006.11.01 ����ȣ timer�� �̿��Ͽ� ȭ�� ����Ʈ�� delay�� �ֱ� ���Ͽ�
	CWnd*	m_pPRNChild;
	CSize	m_sizePRN;
	CSize	m_sizePRNORG;
	bool	m_bPRNResize;
	CString	m_szPRNData;
	BOOL	printOper(CWnd* pChild, CSize size, CSize sizeOrg, CString dat, bool bResize);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
	// �� ���� ����
	CMapStringToString	m_mapAlarmList;		// 2006.8.23 ����ȣ => ������ 
#ifdef DF_USE_CPLUS17
	std::unique_ptr<class CManageInfo> m_mngInfo;  
	std::unique_ptr<class CTOP10Dialog> m_top10;
#else
	class CManageInfo*	m_mngInfo;	// ������ ȭ�� 2006.8.3
	class CTOP10Dialog* m_top10; //�ż� TOP10 ȭ�� 2017.10.27
#endif
	bool	m_bSound;	// �˸� �� ������ �Ҹ��˸� �ɼ�
	// 2006. 11. 07 �� �޽��� ��ġ �ɼ��߰� 
	int		m_nInfoPos;
	// 2010. 08. 24 KOBA ELW �˸� �ɼ�
	bool	m_bKobaElwNotify;

	// 2007.09.20 ����ȭ�� ���
	int		m_nBkMode;
	BOOL	m_bUseAlarm;	// �˸� �޼��� ��뿩��
	
	void	MngInfoPos();
	void	ShowMngInfo(CString dat);
	void	ShowMngInfo(DWORD* dat);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
	// ����ð��� ���߾� ��ֽŰ� ������ �ֵ��� ������ ���� 2006.12.06
	CString	m_szRTime;
	void	sendRTime();
	void	processRTime(char* ptr, int len);
	BOOL	IsRunTime();
	void	RedrawVS(bool bMinus = false);
	void	AddMdi(int vsn, int key, CChildFrame* pChild);
	void	RemoveMdi(int vsn, int key);
	void	AddSdi(int vsn, int key, CSChild* pChild);
	void	RemoveSdi(int vsn, int key);
	void	RemoveAllChild(int vsn, bool bSdi = false);
	CString GetScreenList(int vsn);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
	// ôô�ڻ� 2007.01.22
// 	void	ShowDoctorInfo(CString dat);
// 	void	ShowDoctorInfo(DWORD* data);
//	void	doctorInfoPos(bool bDoctor);

	class	CDtInfo	*m_pDoctor;
/////////////////////////////////////////////////////////////////////////////////
	// �̼���߰� �˶�
	void	showMisuAlarm();
	class CDlgMisuAlarm*	m_misuAlarm;
/////////////////////////////////////////////////////////////////////////////////
	// 20070621 RTS queue
	void	checkRTSQueue();
	CString	m_saveTitle;
	CString	m_titleS;
	bool	m_bRTSQueue;
	CCpuUse	m_cpu;

	int		m_winVer;			// 20070724 win version
	CString	getProcessList();		// 20070724
/////////////////////////////////////////////////////////////////////////////////
	// 20070627 �޻�(����) ���ó��
	bool	m_bFirstOpen;
	bool	m_bLastClose;
	class CSysInfo	*m_sysInfo;
	void	checkOpenedList();
	void	writeOpenedList(bool bFirst = false);
	void	removeOpenedList();
	CString	m_ipAddr;
	CString	m_sWin;
	CString	m_sCpu;
	CString	m_sMem;
	CString m_sIE;
	CString	m_sLastMapInfo;
	CString	m_sDelayData;
	int		m_nDelayPeak;
	int		m_nCpuPeak;
	CTime	m_tmDelayStart;
	CTime	m_tmDelayEnd;

	bool	m_bDelayIn;

	bool	m_bStart;
//////////////////////////////////////////////////////////////////////////////////////
//20190131 E-PB, CROWD ���Ŭ�� Ƚ��
	int     m_iClickEPBBanner;
	int     m_iClickCROWDBanner;
//////////////////////////////////////////////////////////////////////////////////////
	CMapStringToPtr	m_mapPBWnd;
	class CPBRect
	{
		public:
			bool bShow{};
			CRect rect;
			CWnd* pWnd{};
			CRect MainRect;
	};

	CArray<CPBRect*,CPBRect*> m_arrOrdRect;
	void PBMngShow(CString strMsg);
	CRect getBasePBRect(int iwidth, int iheight);
	bool  CompareRect(CRect aRect, CRect bRect);
	bool  CheckArrRectShow();

	void onFireLog(int type, WPARAM wParam, LPARAM lParam);
public:
	void  AddCodehistoryToInter(int igroup, char* pdata);
//CArray<void*, void*> m_arrPBDlg;
//void PBMngShow(CString strMsg, int iGubn = 0);
//	void  ParseRTSMessage(char* pdata, int len);
	//2020 ��������
//	CArray<struct _updn*, struct _updn*> m_arrInterDn;

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcPaint();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	afx_msg void OnAppDummy(UINT uid);
	afx_msg void OnUpdateRunCommand(CCmdUI* pCmdUI);
	afx_msg	LONG OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnUSER(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnCHILDMSG(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnCHASER(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnELOG(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnApplyACC(WPARAM wp, LPARAM lp);
	afx_msg LONG OnSaveACC(WPARAM wp, LPARAM lp);
	afx_msg LONG OnAxisClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrayEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnHotKey(WPARAM wp, LPARAM lp);
	afx_msg LONG OnSecureChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnMiniClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHTSCloseFMini(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIsDev(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStaff(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHotTrade(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIBKWine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIBKCode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIBKWineS(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIBKCodeS(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnIBKWineSNews(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnIBKWineSKiup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFocusNotice(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWebHwnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseWeb(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGoodOrder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPhonePadNating(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadSecure(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResInterest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTop10(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLockPass(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInterMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_EVENTSINK_MAP()
	afx_msg int OnFireRec(int type, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsForeignMap(const char* map);
	BOOL IsNewRealTick(const char* map);
	void RunForeignMap();
	void RunNewRealTick();

	CResourceHelper* m_resourceHelper;

	void AcctGroupConfig();
	void AcctPasswordConfig();
	void AcctAliasConfig();
	void AcctConfig();

	void TotalSetup(UINT id = IDD_ESETUP);
	void processMapVersionInfo(WPARAM wParam, LPARAM lParam);
	void ShowMngInfo(CString msg, int kind);
	BOOL IsExistMap(const char* mapName);
	BOOL IsExistMap(const char* mapName, int vs);
	CString GetMapName(const char* mapName);

	CString get_glb_addr(char* macaddr, char* ip);

	CStringArray m_arrGlb;
	int m_iGlbIndex;
	CString get_glb_addr_Index(char* macaddr, char* ip);
	void ReadAxisGlb();
	/**
	void insertToolDefault();
	void insertToolDefault2();
	void insertToolDefault3();
	**/

	CString GetChildWindowCaption(int key);
	LRESULT SetChildMinMaxInfo(int key, MINMAXINFO* lpMinMaxInfo);

	BOOL DrawCaption(CDC* dc, CRect rc, int height = CAPTION_HEIGHT);
	LRESULT SetMenuMode(int menuMode, int height);

	friend CMapHelper;
	friend CDtconnect;
	friend CCertLogin;

	void SetUserInfo();
	void DoFunc(int funcID);
	void ExecuteCalculator();
	void GoHomepage();

	void UnderConstruction();

	void ShowSingleWindows(BOOL bShow);

	CString GetUserPassword();
	CString GetCertPassword();
	LRESULT ApplySetup(UINT id);
	void	load_tickInfo();
	void	RunTopBanner(int index);  
	void	RunStaffTopBanner();

	void GetLocalIP();
public:
	CRect GetWndRect();
	CWnd* GetClock();
private:
	void RunFOConfig();
	void SendConfig(int igubn);    // 0 : crowd,   1 : E-pb  
	void SendLastStat(CString vsN, CString sval);
	void SendForceIP();
	void SendLastStat(char* strData);
	void Send2000();
	void removeCompressfile();
	void CompressChart(CString path);
	CString Compress(CString target, CString path = _T(""));
	int SetDnloadData(int gno, CString gname, int count, char* datB);
	CString GetMyThemeName();
	void ReadNotice();
	void ReadNoticeMap();
	void InitMapSize();
	BOOL IsWebMenu(const char* MapName);
	BOOL IsHelpMenu(const char* MapName);
	bool CheckClientWidth(CChildFrame*	child, int preFontSize);
	bool bBackFont;
	int m_preFontSize;
	BOOL stopDllMessage;
	BOOL m_bNoProtect;
	CString m_codeHist;
	//* pc firewall
	npenkAppInstall5WIN		m_npAppMgr;
		
	void InitFirewall();
	void FreeFirewall();
public:
	//CArray <_shjcode, _shjcode>	m_hjcode;		// ��������
	//CArray <_sjcode, _sjcode>	m_fjcode;		// ��������
	//CArray <ojcode, ojcode>		m_ojcode;		// �ɼ�����
	//CArray <pjcode, pjcode>		m_pjcode;		// �����ɼ�
	//CArray <upcode, upcode>		m_upcode;		// �����ڵ�
	//CArray <_sjcode, _sjcode>	m_elwbase;		// elwbase

	void	loadingHJcode();
	void	loadingFJcode();
	void	loadingOJcode();
	void	loadingPJcode();
	void	loadingUPcode();
	void	loadingELWcode();

	const CWnd* GetBkWnd();
	CString GetCodeName(CString code);
	void RotateView(int tp);
	void UploadFile();
	void parsingOubsDN(char* datB, int datL);
	void dnloadAction();
	BOOL IsPhonePad(const char* map);
	void RunPhonePad();
	BOOL m_bfitdual;
	WINDOWPLACEMENT m_predual;
	CString m_theme;
	int VistaFrame;
	void PopupPosWeb(CString url,int left, int top,int cx,int cy);
	bool SetToolAccount(CString acnt, int wparam);
	BOOL IsNoFontMenu(const char *MapName);
	bool IsFileExist(CString filename);
	void ChangeLogo();
	CString m_dept;
	CString Variant(int nComm, CString strData  = _T("") );
	bool bChgStaffPwd;
	void CheckNewsSetting();
	void CheckSoundConfig();
	void CheckEncryptDirectory();
	void FixUserConfigDirectory();
	CPhonePad* m_phone_dlgs;
	void SetPwd(CString strpwd);
	void ReadPhonePad(int fill);
	void sendDllMessage(HWND sdm, LPARAM lparam);
	LRESULT OnPhonePad(WPARAM wParam, LPARAM lParam);
	LRESULT OnRefresh813(WPARAM wParam, LPARAM lParam);
	LRESULT OnSecureDlg(WPARAM wParam, LPARAM lParam);

#ifdef DF_USE_CPLUS17
	std::unique_ptr<CMapHelper>m_mapHelper{};
#else
	CMapHelper* m_mapHelper;
#endif

	//IMAX  
	void	IMAXGroupSet(int key, int triggerN);
	void	IMAXSkinSet();

	//��������
	CString m_slog;
	CString m_sSimpleAuth;
	CString m_sCustNumber; //����ȣ
	bool	m_bSimpleAuth;

	void   signOnSimpleAuth(char* pdata);

	//������ȸ
	void InitMapHK();
	int   GetKeyByHWnd(HWND hwnd, int igubn);
	HWND GetHWndByKey(int key, int& msg);
	CMap <int, int, CString, CString> m_mapHWndToKey;

public:
	void OutputWaitList();

	//����������ȸ ����
	/*CWnd* m_pInter;
	CWnd* m_pSearchWnd;*/

	HWND m_hInter;
	HWND m_hSearchWnd;

	//��ť�� ���� ���� ��ġ ����
	void Check_XECUREPATH();
	
	void UnzipXecure();
	void CreateFolder(CString strTargetPath);
	//pc��������
	void SetPCData();
	//hashkey
	UINT AFXAPI HashDataAXIS(LPCSTR key);
	//���ڵ� create �Լ� 
	bool CreateWizard();
	//as is ����ȭ�� ������ �����
	void Delete_AsisICon();

	//���Ӽ��� Ȯ��
	void CheckServer(CString strip);
	CString m_strServer{};
protected:
// #ifdef USE_AHNLAB_SECUREBROWSER
// 	IAosSB *m_pAosSB;
// 	
// 	BOOL	initAOSSDK();
// 	void	uninitAOSSDK();
// 	
// 	BOOL	initSBSDK();
// 	void	uninitSBSDK();
// 	
// 	BOOL	isVerifyTrustSelf(LPCTSTR szSignedName);
// 	BOOL	isVerifyTrustSDK();
// 	BOOL	isRunningSDKPd(DWORD dwServiceCode);
// 	
// 	IAosVerify*	getAosVerify();
// #endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__31C95AE9_C26F_4667_8C55_EE4D756EB344__INCLUDED_)
