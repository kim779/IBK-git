#pragma once
// TreeWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeWnd window
#include "../../appl/IhUtils.h"
#include "../../h/jmcode.h"
#include "../../h/interst.h"
#include "Account.h"
#include "sharemsg.h"

struct _shjcode			// semi hjcode
{
	CString code;
	CString name;
	CString symb;		// symbol
	char	ecng{};		// ECN ����
	char	size{};		// �ں��ݱԸ�	2:��, 3:��, 4:��
	char	ucdm{};		// �����ߺз�
	char	ucds{};		// �����Һз�
	char	jjug{};		// ������ ����(27: ������)
	char	kpgb{};		// KOSPI200 ����
	char	kosd{};		// ���񱸺�
	char	ssgb{};		// �Ҽӱ���
	char	ucmd{};		// 0:������, 5:�켱��
	char	wsgb{};		// 0:�Ϲ� 1:���豸��������
	char	jsiz{};		// �����ں��� ���߼�
	char	itgb{};		// KOSPI-IT/KQ-IT50����(1:ä��)
	char	star{};		// KOSDAQ ��Ÿ����
	char	unio{};		// �������� (KRX100)
	char	prmr{};		// �����̾�����
	char    jchk{};		// ����˻��� ����
				// 0x01 �Ҽ��ǰ�������
				// 0x02 �űԻ�������
				// 0x04	�Ǹ�������
				// 0x08	�켱������
				// 0x10 ��������
				// 0x20	��������
				// 0x40 �ŷ���������
				// 0x80	������������
};

struct _sfjcode			// semi fjcode
{
	CString code;
	CString name;
};

struct  _acInfo {
    char    accn[10];			// ���¹�ȣ
    char    acnm[20];			// ���¸�
    char    aseq[2];			// ���¼���
					// 00: �⺻����
					// 01: ȭ��� ������ �ʴ°���
    char    pwch[1];			// ���࿬����¿���
};
#define	L_acInfo	sizeof(struct _acInfo)

struct  _acSet {
    char    func[1];			// FUNCTION CODE
					// 'Q' Query    'I' Insert
					// 'U' Update
    char    usid[12];			// ����ھ��̵�
    char    errc[1];			// Error Code
    char    emsg[80];			// Error message
    char    nrec[4];			// Account Count
    //struct  _acInfo acinfo[1];
};
#define	L_acSet		sizeof(struct _acSet)


class 	_acSave	
{
public:
	CString sPriAcc;	//��ǥ���±���
	CString sSortNum;	//�����Ϸù�ȣ
	CString sAccntNum;	//���¹�ȣ
	CString sAccntName;	//�����̸�
	CString sAllocRate;	//��������
	CString	sAllocMulti;	//�¼�
	CString sAccntNick;	//���º�Ī
public:
	_acSave()
	{
		sPriAcc = _T("");
		sSortNum = _T("");
		sAccntNum = _T("");
		sAccntName = _T("");
		sAllocRate = _T("");
		sAllocMulti = _T("");
		sAccntNick = _T("");
	};

	_acSave(_acSave const &other)
	{
		Copy(other);
	};

	~_acSave() {}

	inline void Copy(_acSave const &other)
	{
		sPriAcc = other.sPriAcc;
		sSortNum = other.sSortNum;
		sAccntNum = other.sAccntNum;
		sAccntName = other.sAccntName;
		sAllocRate = other.sAllocRate;
		sAllocMulti = other.sAllocMulti;
		sAccntNick = other.sAccntNick;
	};

};

class CAccn
{
public:
	bool    m_bFuture{};
	CString   m_accn;
	CString   m_pswd;
	CString   m_errmsg;
	bool    m_bCredit{};    // true:�ſ�����, false:�ſ�����
	int     m_dFee{}; //������������ ���� 1 ������ 0
	double    m_dMass{}; //��ü���� 0:hts 1:������/�� 2.ARS
	double    m_dSave{}; //��ü���� 0:hts 1:������/�� 2.ARS
	
	CMapStringToPtr m_CodeMap;    // Ptr: F->class CRemainFuture
	//      C->class CRemain
	
public:
	CAccn()
	{
		m_bFuture = false;
		m_bCredit = false;
		m_accn = "";
		m_pswd = m_errmsg = "";
		m_dFee = 0; //������������ ���� 1 ������ 0
		m_dMass = 0;
		m_dSave = 0;
		
		m_CodeMap.RemoveAll();
	}
};


class CTreeWnd : public CTreeCtrl
{
// Construction
public:
	CTreeWnd(CWnd* pMainWnd);
	
// Attributes
public:
	CString		m_strAccount;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void settingDataStock2(char* buf, class CGridData& sdata);	// �ֽ��ܰ� ������ ������
	void settingDataFuture2(char* buf, class CGridData& sdata);	// �����ܰ� ������ ������

private:
	BOOL    IsNumber(CString str);
	BOOL	sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt);
	BOOL	SendOper(UINT kind, int opt = 0);

	CString GetString(char* pChar, int nMinLen);
	CString Variant(int comm, CString param = _T(""));
	CString	MakePacket(CString& code, CString amount = _T(""), CString price = _T(""), CString name = _T(""), CString bookmark = _T(""), CString futureGubn = _T(""), CString creditPrc = _T(""), CString maeipPrc = _T(""));

	void	queryRemain2(CString strAccount, CString sPswd);
	void	queryFutureRemain2(CString strAccount, CString sPswd);
	void	oubRemain(char* buf, int len);
	void	init();
	void	SendTreeData(const class CGridData& sdata);
	void	SendRemainData(class CGridData& sdata);
	void	GetData(class CGridData& gdata, int max, int opt = 0);
	void	InitData(int kind, int nIndex);	
	void	RecvOper(int kind, CRecvData* data);
	void	parsingNews(CRecvData* data);
	void    SendInterest(int item);			//���ٿ��� �׷� Ŭ���� �۵�

	void SendTotalRemain(CString strAccount);
private:

	CMap<DWORD, DWORD, HTREEITEM, HTREEITEM>	m_mapParam;
	CWnd*		m_pMainWnd{};
	CString		m_root;
	CString		m_user;
	CString		m_id;
	CString		m_pass;
	CString		m_szRET;
	
	bool		m_bCustomer{};
	UINT		m_kind{};
	CGridData	m_pSData;

	ACCOUNT		m_accn;			// �� ���¸�� hold
	CStringArray		m_accnT;

public:
	void receiveOub(CString& data, int key);
};
 