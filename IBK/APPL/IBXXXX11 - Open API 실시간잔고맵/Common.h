#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

// Ledger filler
#define ORDER_INFO_IDX          (0x10)

#define WM_ACCOUNT			(WM_USER+10000)
#define	WM_JANGO_CHANGE		(WM_USER+10001)

// define Window Messeage
#define WM_APP_SIG			(WM_USER+0xF0)
#define WP_ACCN_CHANGE			(0x00000001)
#define WP_F_JNGO_CHANGE		(0x00000002)
#define WP_S_JNGO_CHANGE		(0x00000003)

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

#define MAX_FHOGA			(5)
#define FCODE_LEN			(8)
#define DOUBLE_PREFIX		(0.00005)

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
typedef enum { CT_NONE=0, CT_FUTURE, CT_OPTION, CT_SPREAD } CODE_TYPE;
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
	int oprc;	// �ֹ�����
	int oqty;	// �ֹ�����
	int dprc;	// ü�ᰡ��
	int dqty;	// ü�����
	int wqty;	// ��ü�����
	bool stop;  // stop�ֹ�����
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
	Account() : IsValid(FALSE) {}
	BOOL IsValid{};				// ������ ��������
	CString acno, pswd, acnm;	// ���¹�ȣ, ��й�ȣ
	int mult{};					// �ֹ� ���
	CodeOrderMap order;			// ���� > �ֹ���ȣ�� > ��ü�� �ֹ�����
	CodeJangoMap jango;			// ���� �ܰ���
	CodeMichegMap micheg;		// ���� �ֹ����ݺ� ��ü�� ��������
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