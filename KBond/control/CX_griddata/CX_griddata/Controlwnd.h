#pragma once


// CControlwnd
#include <afxtempl.h>
#include <Afxmt.h>

#define ALLCODE		0
#define GUKCODE		1
#define TONGCODE	2
#define TUKCODE		3
#define JICODE		4
#define GUMCODE		5
#define WHECODE		6
#define GUKTONG		7
#define EGUKTONG	8


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

class CCodeData
{
public:
	CString strName;
	CString strCode;
	CString strsuik;
	CString strMangi;
	bool	bFlag;
	COLORREF clrData;
};


struct	_ledgerH {
	//char	len[8];
	char	anam[40];		/* application name	*/
	char	snam[40];		/* service name		*/
	char	fnam[32];		/* function name	*/
	char	fncd[2];		/* function code	*/
	char	guid[32];		/* global id		*/
	char	ctyp[2];		/* channel type		*/
	char	eflg[1];		/* environment type	*/
	char	tflg[1];		/* tran flag		*/
	char	tdat[8];		/* tran send date	*/
	char	ttim[20];		/* tran send time	*/
	char	clip[31];		/* client ip		*/
	char	rtyp[1];		/* response type	*/
	char	rcod[8];		/* response code	*/
	char	rlcd[20];		/* response log code	*/
	char	rtit[50];		/* response title	*/
	char	rbmg[100];		/* response message	*/
	char	rdmg[500];		/* response message	*/
	char	usid[12];		/* user id		*/
	char	crst[2];		/* character set	*/
	char	fill[78];		/* filler		*/
	char	tmno[15];		/* terminal no		*/
	char	tmps[15];		/* terminal position	*/
	char	dept[8];		/* user department	*/
	char	brnc[8];		/* user branch		*/
	char	sale[8];		/* business branch	*/
	char	agnc[8];		/* agency		*/
	char	sfill[38];		/* filler		*/
	char	susid[20];		/* user id		*/
	char	menu[20];		/* menu			*/
};

typedef struct _balst
{
	char gubn[2];				 //����			01ȣ��
	char junum[7];				//�ֹ���ȣ 
	char date[10];				//��¥			yyyy-mm-dd
	char time[8];				//�ð�			hh:mm:ss
	char code[12];				//�����ڵ�		
	char name[100];				//�����		
	char ckind[4];				//ä������     1000��ä  4000���� 3000Ư�� 2000���� 5000����  6000ȸ�� 9999��Ÿ
	char mandate[10];			//������       yyyy-mm-dd
	char leftdate[6];			//������       
	//char duration[7];			//�෹�̼�
	char min[7];				//����
	char suiktype[2];			//���ͷ�����    01���ͷ�  02����
	char hogatypecode[2];		//ȣ�������ڵ�  01�ż�  02�ŵ� 03Ȯ��
	char hogatype[6];			//ȣ������      �ż� �ŵ� Ȯ��
	char hoga[7];				//ȣ��			���ͷ��� ��� �Ҽ��� 3�ڸ� , ������ ��� �Ҽ��� 1�ڸ�
	char contradict[7];			//���			������
	char volume[6];				//����			
	char payday[8];				//������
	char linegubn[12];			//����ü����	null, [��], [�� + Ű]
	char hightradegubn[1];		//���߰ŷ�ȣ������  Y/N 
	char sendid[30];			//�۽���ID
	char comcode[6];			//ȸ���ڵ�
}BALST;

typedef struct _hogast
{
	char gubn[2];				 //����			02����ȣ��
	char code[12];				//�����ڵ�
	char name[100];				//�����
	char ckind[4];				//ä������     1000��ä  4000���� 3000Ư�� 2000���� 5000����  6000ȸ�� 9999��Ÿ
	char mandate[10];			//������
	char leftdate[6];			//������    
	char min[10];				//����
	char suiktype[2];			//���ͷ����� 01:���ͷ� 02:����
	char mdcur[10];				//�ŵ�ȣ�����簡
	char mdcurtime[5];			//�ŵ�ȣ�����簡������Ʈ�ð�  hh:mm
	char mdcurmcont[10];         //�ŵ�ȣ�����簡 ���� ���
	char mdjprc[10];				//�ŵ�ȣ��������
	char mdjcont[10];			//�ŵ�ȣ�����������
	char mdhggoga[10];			//�ŵ�ȣ����
	char mdhggogatime[5];		//�ŵ�ȣ����������Ʈ�ð�  hh;mm
	char mdhgjuga[10];			//�ŵ�ȣ������
	char mdhgjugatime[5];		//�ŵ�ȣ������������Ʈ�ð�  hh;mm
	char mscur[10];				//�ż���ȣ�����簡
	char mscurtime[5];			//�ż�ȣ�����簡������Ʈ�ð�  hh:mm
	char mscurmcont[10];         //�ż�ȣ�����簡 ���� ���
	char msjprc[10];				//�ż�ȣ��������
	char msjcont[10];			//�ż�ȣ�����������
	char mshggoga[10];			//�ż�ȣ����
	char mshggogatime[5];		//�ż�ȣ����������Ʈ�ð�  hh;mm
	char mshgjuga[10];			//�ż�ȣ������
	char mshgjugatime[5];		//�ż�ȣ������������Ʈ�ð�  hh;mm
	char fncur[10];				//Ȯ��ȣ�����簡
	char fncurtime[5];			//Ȯ��ȣ�����簡������Ʈ�ð�  hh:mm
	char fncurmcont[10];         //Ȯ��ȣ�����簡 ���� ���
	char fnjprc[10];				//Ȯ��ȣ��������
	char fnjcont[10];			//Ȯ��ȣ�����������
	char fnhggoga[10];			//Ȯ��ȣ����
	char fnhggogatime[5];		//Ȯ��ȣ����������Ʈ�ð�  hh;mm
	char fnhgjuga[10];			//Ȯ��ȣ������
	char fnhgjugatime[5];		//Ȯ��ȣ������������Ʈ�ð�  hh;mm
}HOGAST;


typedef struct _ftchest
{
	char date[10];			     //��¥ yyyy-mm-dd
	char time[8];			     //�ð� hh:mm:ss
	char comcode[6];			//ȸ���ڵ�
	char code[12];				//�����ڵ�		
	char name[100];			    //ä�Ǹ�
	char ckind[4];				//ä������     1000��ä  4000���� 3000Ư�� 2000���� 5000����  6000ȸ�� 9999��Ÿ
	char mandate[10];			//������
	char suikrate[7];			//���ͷ�
	char contrasuik[7];			//���������			
	char prc[7];				//����
	char contraprc[7];			//���ݴ��
	char volume[6];				//����
	char payday[10];			//������
	char min[7];				//����
	char minprc[7];				//���򰡰�
}FTCHEST;


typedef struct _chest
{
	char date[10];			     //��¥ yyyy-mm-dd
	char time[8];			     //�ð� hh:mm:ss
	char code[12];			     //ä���ڵ�
	char name[40];			    //�����
	char ckind[4];				//ä������     1000��ä  4000���� 3000Ư�� 2000���� 5000����  6000ȸ�� 9999��Ÿ
	char mandate[10];			//������
	char leftdate[10];			//������    
	char min[7];				//����
	char minprc[7];				//���򰡰�
	char suikrate[7];			//���ͷ�
	char contradict[7];			//���			������
	char volume[6];				//����	
	char payday[10];			//������
	char comcode[6];			//ȸ���ڵ�
}CHEST;

struct mid
{
	char userid[30];
};

struct mod
{
	char name[30];
};

class CControlwnd : public CWnd
{
	//DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	CWnd* m_pWizard;

	CString m_strcode;
	CString m_strmap;
	CString m_sUserID;
	CString m_sUserName;
	CString m_sRoot;
	CString m_sGroupName;
	CString m_shogatype;

	CString m_ssSearchdate;
	CString m_seSearchdate;

	CString m_ssmandate;
	CString m_semandate;

	CString m_strRealData;
	CString	m_strMsgUser;   //�޽��� ��ȭ�� ���� ���̵�
	CString	m_strMsgName;   //�޽��� ��ȭ�� ���� �̸�

	CCriticalSection m_sync;
	CMapStringToString m_mapGroupKey;
	CMap <CString, LPCTSTR, CMap<CString,LPCTSTR, CCodeData, CCodeData>*,  CMap<CString,LPCTSTR, CCodeData, CCodeData>*>  m_mapGroupCodemap;

	int m_imsgcnt;
	bool m_bServerError;
	bool m_brts;
	int m_kind;			//0 �����ֱ�ȣ��  1 ��üȣ�� 2 15��ü�� 3 �������� 4 ��������
	int m_itype;		//ä������
	int itestcnt;
	bool m_binter;    //���ɱ׷쿡 ���븸 �����ִ��� ����
	bool m_bsuik;     //true ���ͷ�  false ����
	bool m_bmyhoga;   //��ȣ���� �����ֱ� ����

	enum	ctype {
		dispidGetGrouptype = 19L,
		dispidSetSync = 18L,
		dispidMessangerMemoPop = 17L,
		dispidGetCodeInGroup = 16L,
		dispidGetRealDate = 15L,
		dispidSethogatype = 14L,
		dispidSetcodetype = 13L,
		dispidSetmyhoga = 12L,
		dispidSetsuik = 11L,
		dispidSetbInter = 10L,
		dispidSetGroupByName = 9L,
		dispidGetGroupCombo = 8L,
		dispidSetMandate = 7L,
		dispidSetSearchDate = 6L,
		dispidGetAlertData = 5L,
		dispidGetOubData = 4L,
		ONECODE=0, MANYCODE } m_type;
	CParam		m_Param;
	virtual ~CControlwnd();

	virtual void OnFinalRelease();

	void	AddComma(char* pcData);
	void	AddGroup(CString strGroup = _T(""));
	void	SetParam(struct _param *pParam);
	void	LoadFile(CString strgroup = _T(""));
	void	LoadGroupOfFile();	

	void	ShellexcuteLogin();
	void	SendallhogaRTS(struct _balst* pdata);
	void	SendjongcodeRTS(struct _hogast* pdata);
	void	Send15CheRTS(struct _ftchest* pdata);

	bool	Checkcodetype(char* pkind);
	bool	Checkintercode(char* pcode);
	bool	CheckSuikprc(char* type);
	bool	Checkmyhoga(char* pcode);
	bool	Checkhogatype(char* hogatypecode);
	bool	Checksearchdate(char* date);
	bool	Checkmandate(char* mandate);
	bool	CheckMessangerLogin();
	void	JustLogin();

	CString GetSymbolFromFile();
	CString Variant(int nComm, CString strData = _T(""));
	CString Parser(CString &srcstr, CString substr);
	void	SendTR(CString name, CString data, BYTE type, int key);
	
	void	AddListOfCodeMap(CString strgroup, CString sCode, CString sName, CString strFlag, CString strClr);
	
	//void    sendTR(CString trC, char* pBytes, int nBytes, int gubn);
	bool	sendTR(CString trC, char* pBytes, int nBytes, int gubn = 1);

	void	GetledgerH(struct _ledgerH* pLedgerH, int gubn = 1);
	void	SearchInterList();
	void	oubSearchList(char* pdata);

	CString GetCombodata(int len, int icnt);
	CString GetCombodata(int len, int icnt, CString strgroup);
	CString AddSpace(CString strdata, int ilen);

	bool	m_bsync;
	CStringArray m_arr;
	void   test();
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
protected:
	void SendData(void);

	enum 
	{
		dispidSetMapName = 3L,
		dispidJCode = 2,
		dispidSendData = 1L
	};
	BSTR GetJCode(void);
	void SetJCode(LPCTSTR newVal);
	void SetMapName(LPCTSTR strmap);
	BSTR GetOubData(void);
	BSTR GetAlertData(void);
	void SetSearchDate(LPCTSTR sdate, LPCTSTR edate);
	void SetMandate(LPCTSTR sdate, LPCTSTR edate);
	BSTR GetGroupCombo(LPCTSTR edate);
	void SetGroupByName(LPCTSTR group);
	void SetbInter(LPCTSTR binter);
	void Setsuik(LPCTSTR bsuik);
	void Setmyhoga(LPCTSTR bmyhoga);
	void Setcodetype(LPCTSTR type);
	void Sethogatype(LPCTSTR typehoga);
	void Setcode(LPCTSTR code);
	BSTR GetRealDate(void);
	BSTR GetCodeInGroup(LPCTSTR group);
	void MessangerMemoPop(LPCTSTR userid);
	void SetSync(LPCTSTR sync);
	
	BSTR GetGrouptype(LPCTSTR groupname);
};


