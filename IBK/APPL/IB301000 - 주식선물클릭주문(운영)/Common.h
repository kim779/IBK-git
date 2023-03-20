#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

// Ledger filler
#define ORDER_INFO_IDX          (0x01)

#ifdef _DEF_IB301000_
	#define CONFIG_INI     "%s\\user\\%s\\IB301000.ini"
#else
	#define CONFIG_INI     "%s\\user\\%s\\IB191000.ini"
#endif

#define MAP_GROUPEDIT  "IB191010 /S/s/p5/t"
#define MAP_ORDERLIMIT "IB0000B2 /S/s/p5/t"
#define MAP_CHEG       "IB310200 /S/s/p5/t"
#define MAP_CHART      "IB713800 /p5/t"
#define MAP_JANGO      "IB320100 /S/s/p5/t"
#define MAP_SISECHEG   "IB425100 /S/s/p5/t"

// define Window Messeage
#define WM_APP_SIG			(WM_USER+0xF0)
#define WP_ACCN_CHANGE			(0x00000001)
#define WP_JNGO_CHANGE			(0x00000002)
#define WP_CODE_CHANGE			(0x00000003)
#define WP_MADO_ALL_CANCEL		(0x00000004)
#define WP_MASU_ALL_CANCEL		(0x00000005)
#define WP_ORDER				(0x00000006)
#define WP_MOUSE_IN				(0x00000007)
#define WP_MOUSE_OUT			(0x00000008)
#define WP_CANCEL_ORDER			(0x00000009)
#define WP_MODIFY_ORDER			(0x0000000A)
#define WP_CANCEL_RSVD			(0x0000000B)     
#define WP_MODIFY_RSVD			(0x0000000C)
#define WP_CURRCELL_CHANGE		(0x0000000D)
#define WP_RSVD_ORDER			(0x0000000E)
#define WP_MADO_RSVD_ALL_CANCEL	(0x0000000F)
#define WP_MASU_RSVD_ALL_CANCEL	(0x00000010)
#define WP_HOGA_CLICK			(0x00000011)
#define WP_CTRL2_FOLD           (0x00000012)
#define WP_CTRL2_FOLD_DISABLE   (0x00000013)
#define WP_ONECLICK_MODE        (0x00000014)
#define WP_SET_JCNT             (0x00000015)
#define WP_CTRL_FOLD			(0x00000016)
#define WP_RSVD_REALORDER		(0x00000017)

#define WM_APP_REFRESH		(WM_USER+0xF1)
#define WP_MICHEG			(0x00000001)
#define WP_JANGO			(0x00000002)
#define WP_RESERVE			(0x00000003)
#define WP_ACCOUNT			(0x00000004)
#define WP_MICHEG_INSERT    (0x00000005)
#define WP_MICHEG_UPDATE    (0x00000006)
#define WP_MICHEG_DELETE    (0x00000007)
#define WP_ORDER_RESULT     (0x00000008)
#define WP_FOLDSTAT			(0x00000009)

//2013.02.07 CX_OptionEx �߰�
#define WM_OPTIONEX			(WM_USER+0xF2)

// define Color!
#define COLOR_LINE			93
#define COLOR_TABLE			75
#define COLOR_BK			64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA			99
#define COLOR_TEXT			69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB			171		// �ŵ� back
#define	COLOR_MSB			170		// �ż� back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS			78

#define BACK_INDEX					64
#define CMD_BACK_INDEX				66
#define CONTENTS_BACK_INDEX			181
#define CONTENTS_BORDER_INDEX		93
#define TAB_BORDER_INDEX			44
#define GUIDE_BACK_INDEX			34
#define GUIDE_TXT_INDEX				130
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71

#define PADDING_VALUE	(6)
#define ARROW_BOX_WIDTH		(4)

#define CODECTRL_WIDTH		(80)

// Expand Hoga
#define EXPAND_HOGA_WIDTH   (25)

// ��ü �������� ����
#define MAIN_WND_HEIGHT		(624)

// ����/�����ڵ� �������� ����
#define GROUP_WND_HEIGHT		(94)
#define ACCN_WND_HEIGHT         (132)
#define SISE_WND_HEIGHT         (45)

// �ֹ�����/������ �׸��� ����
#define CTRL_WND_WIDTH		(180)

// �ֹ����ɼ���/ �ǽð��ܰ�/ ��ü�᳻�� ����
#define CTRL2_WND_WIDTH     (400)

// GUIDE ������ ����
#define GUID_WND_HEIGHT		(22)

// SCROLLBAR ����
#define SCROLLBAR_WIDTH		(18)

// �׸��� �� ����
#define GRID_CELL_HEIGHT	(18)

#define MAX_FHOGA			(5)
#define FCODE_LEN			(8)
#define DOUBLE_PREFIX		(0.00005)
#define MARKET_JPRC			(-99999999)

#define FIRST_PCURR			(-999999)		//2016.11.09 KSJ �ʱⰪ�� �������ش�.

// for speed parsing
class StringProxy : public pair<LPCSTR,LPCSTR> {
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}
	string ToStr() { return string(first, second); }
	CString ToCStr() { return CString(first, second-first); }
};

class op_notice_spliter {
public:
	bool operator() (char ch)
	{
		return (ch=='\r')||(ch=='\n')||(ch=='\t');
	}
};

typedef map<int, StringProxy> RealData;
typedef enum { CT_NONE=0, CT_FUTURE, CT_OPTION, CT_SPREAD, CT_ETF } CODE_TYPE;
class SiseData {
public:
	SiseData() { code=""; type=CT_NONE; }
	CString code;
	CODE_TYPE type;
	char udgb;
	int gjga, siga, koga, jega, curr, pcurr;
	int rate, gvol, mgjv, shga, hhga, diff;
	int t_dvol, t_svol, t_dcnt, t_scnt, t_sdch;
	vector<int> fhoga;
	vector<int> frate;
	struct  {
		int mdga;
		int msga;
		int dvol;
		int svol;
		int dcnt;
		int scnt;
	} hoga[MAX_FHOGA];
};

class Order {
public:
	Order() { ZeroMemory(this, sizeof(Order)); }
	char acno[12];
	char code[12];
	int ojno;	// ���ֹ���ȣ
	int juno;	// �ֹ���ȣ
	int odgb;	// �ֹ�����
	int oprc;	// �ֹ����� (MARKET_JPRC �� ��쿡�� ���尡�ֹ� 2011.02.21 )
	int oqty;	// �ֹ�����
	int dprc;	// ü�ᰡ��
	int dqty;	// ü�����
	int wqty;	// ��ü�����
	bool stop;  // stop�ֹ�����
	int row;	// �׸���Row
	int col;	// �׸���Col
	bool bmarket; //���尡 �ֹ� ����
};

class OrderResult {
public:
	CString acno;
	CString juno;
	CString ojno;
	CString emsg;
};

class Micheg {
public:
	Micheg() { ZeroMemory(this, sizeof(Micheg)); }
	char acno[12];		// ���¹�ȣ
	char code[12];		// �����ڵ�
	int oprc;			// �ֹ�����
	int d_cnt;			// �ŵ� ��ü��Ǽ�
	int s_cnt;			// �ż� ��ü��Ǽ�
	int d_wqty;			// �ŵ� ��ü�����
	int s_wqty;			// �ż� ��ü�����
};

class Jango {
public:
	Jango() { ZeroMemory(this, sizeof(Jango)); }
	void operator=(const Jango &rhs) { CopyMemory(this, &rhs, sizeof(Jango)); }
	char flag;		// for Window Message
	char acno[12];	// ���¹�ȣ
	char code[12];	// �����ڵ�
	char name[40];	// �����
	char gubn[10];	// ����
	int  jqty;		// ��������
	int  xqty;		// û�갡�ɼ���
	int  pprc;		// ��հ�
	double camt;	// �򰡱ݾ�
	double tamt;	// �򰡼���
	double srat;	// ���ͷ�
	double mamt;	// ���Աݾ�
};

class ReserveData
{
public:
	ReserveData() { ZeroMemory(this, sizeof(ReserveData)); }
	int odgb;	// �ֹ����� 1:�ŵ�, 2:�ż�
	int cprc;	// üũ����
	int oprc;	// �ֹ�����
	int ptic;	// üũ/�ֹ� ���� Tick
	int oqty;	// �ֹ�����
	int row;	// �׸���Row
	int col;	// �׸���Col
	bool bmarket; //���尡 �ֹ� ����
};

class ModifyData
{
public:
	ModifyData() { ZeroMemory(this, sizeof(ModifyData)); }
	int odgb;	// �ֹ����� 1:�ŵ�, 2:�ż�
	int fprc;	// ������ �ֹ�����
	int tprc;	// �ֹ��� �ֹ�����
	int rtic;   // �̵� Tick for Rsvd
};

class CancelData
{
public:
	CancelData() { ZeroMemory(this, sizeof(CancelData)); }
	int odgb;	// �ֹ����� 1:�ŵ�, 2:�ż�
	int cprc;	// ����ֹ����� 
};

typedef vector<ReserveData> RsvdDataArray;			// �����ֹ��������
typedef map<string, Jango> CodeJangoMap;			// ���� �ܰ� ��Ȳ
typedef map<int, Order> OrderMap;					// �ֹ���ȣ�� �ֹ����� ��
typedef map<string, OrderMap> CodeOrderMap;			// ���� �ֹ���ȣ�� �ֹ����� ��
typedef map<int, Micheg> PriceMichegMap;			// �ֹ����ݺ� ��ü�� ��������
typedef map<string, PriceMichegMap> CodeMichegMap;	// ���� �ֹ����ݺ� ��ü�� ��������

class Account {				// ������������
public:
	Account() : IsValid(FALSE),IsOwn(FALSE) {}
	BOOL IsValid;				// ������ ��������
	CString acno, pswd, acnm;	// ���¹�ȣ, ��й�ȣ
	int mult;					// �ֹ� ���
	CodeOrderMap order;			// ���� > �ֹ���ȣ�� > ��ü�� �ֹ�����
	CodeJangoMap jango;			// ���� �ܰ���
	CodeMichegMap micheg;		// ���� �ֹ����ݺ� ��ü�� ��������
	BOOL IsOwn;
};

typedef map<string, Account> AccountMap;	// ���¹�ȣ�� ������������ ��



#define MY_DEBUG

#ifdef MY_DEBUG
	#define DECLARE_MYDEBUG()				extern FILE *_fpdbg;
	#define INIT_MYDEBUG(path)				_fpdbg = fopen(path, "wb");
	#define UNINIT_MYDEBUG()				fclose(_fpdbg);
	#define FPRT(y)							if(_fpdbg) { fprintf(_fpdbg, y"\r\n"); fflush(_fpdbg); }
	#define FPRT1(y, a1)					if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1); fflush(_fpdbg); }
	#define FPRT2(y, a1, a2)				if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2);  fflush(_fpdbg); }
	#define FPRT3(y, a1, a2, a3)			if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3);  fflush(_fpdbg); }
	#define FPRT4(y, a1, a2, a3, a4)		if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3, a4);  fflush(_fpdbg); }
	#define FPRT5(y, a1, a2, a3, a4, a5)	if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3, a4, a5);  fflush(_fpdbg); }
#else
	#define DECLARE_MYDEBUG()
	#define INIT_MYDEBUG(path)
	#define UNINIT_MYDEBUG()
	#define FPRT(y)						
	#define FPRT1(y, a1)					
	#define FPRT2(y, a1, a2)				
	#define FPRT3(y, a1, a2, a3)			
	#define FPRT4(y, a1, a2, a3, a4)		
	#define FPRT5(y, a1, a2, a3, a4, a5)	
#endif
