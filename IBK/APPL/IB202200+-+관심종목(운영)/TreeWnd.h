#pragma once
// TreeWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeWnd window
#include "../IhUtils.h"
#include "../../h/jmcode.h"
#include "../../h/interst.h"
#include "Account.h"
#include "sharemsg.h"

struct _shjcode			// semi hjcode
{
	CString code;
	CString name;
	CString symb;		// symbol
	CString	ucdm;		// �����ߺз�
	CString	ucds;		// �����Һз�
	CString acdl;		// ������з�
	char	ecng{};		// ECN ����
	char	size{};		// �ں��ݱԸ�	2:��, 3:��, 4:��
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

struct _efopcode
{
	char	type[1]{};			/*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char	code[6]{};			/*�����ڵ�*/
	char	hnam[39]{};			/*�����*/
	char	opcd[6]{};			/*���� �ڵ�*/
	char	etfm[1]{};			/*�׸��ڵ�*/
	char	etfl[1]{};			/*������ ����*/
	char	lnfd[1]{};
};

struct _efopitem
{
	char	type[1]{};			/*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char	opcd[6]{};			/*���� �ڵ�*/
	char	hnam[30]{};			/*���� �ѱ۾��*/
	char	lnfd[1]{};			/*�����ǵ�*/
	
	//	_efopcode *efopcode;
};


struct _efoptema
{
	char	type[1]{};			/*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char	tmcd[1]{};			/*�׸��ڵ�*/
	char	tmnm[20]{};			/*�׸���*/
	char	lnfd[1]{};			/*�����ǵ�*/
};

struct _efopfore
{
	char	type[1]{};			/*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char	frcd[1]{};			/*������ ����('D':����, 'F':�ؿ�)*/
	char	frnm[10]{};			/*������ ���и�*/	//2015.11.18 KSJ �ڸ��� 4 -> 10
	char	lnfd[1]{};			/*�����ǵ�*/
};

struct _elwdata
{
	CString		code;	// ELW �����ڵ�
	CString		name;	// �����
	CString		ktype;	// �Ǹ����� 01:Call, 02:Put, 03:��Ÿ
	CString		pcode;	// ����� �ڵ�
	CString		pname;	// ������
	CString		mjmt;	// ����� (YYYYMMDD)
	CString		bcode[5];
	BOOL		bkoba{};		// ��������ELW����
};

struct _sfjcode			// semi fjcode
{
	CString code;
	CString name;
};

struct  _acInfo {
    char    accn[10]{};			// ���¹�ȣ
    char    acnm[20]{};			// ���¸�
    char    aseq[2]{};			// ���¼���
					// 00: �⺻����
					// 01: ȭ��� ������ �ʴ°���
    char    pwch[1]{};			// ���࿬����¿���
};
#define	L_acInfo	sizeof(struct _acInfo)

struct  _acSet {
    char    func[1]{};			// FUNCTION CODE
					// 'Q' Query    'I' Insert
					// 'U' Update
    char    usid[12]{};			// ����ھ��̵�
    char    errc[1]{};			// Error Code
    char    emsg[80]{};			// Error message
    char    nrec[4]{};			// Account Count
    //struct  _acInfo acinfo[1];
};
#define	L_acSet		sizeof(struct _acSet)


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

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	// szAt = _T("_") input at last szAt = _T("-") at first else ...
	BOOL IsFolder(HTREEITEM hItem);
	BOOL IsItem(HTREEITEM hItem);	
	void settingReminAcc();
	
	void clickRemain(UINT selItem);
	void clickRecommand(BOOL bRead);

	void settingDataStock(char* buf, class CGridData& sdata);	// �ֽ��ܰ� ������ ������
	void settingDataStock2(char* buf, class CGridData& sdata);	// �ֽ��ܰ� ������ ������
	void settingDataFuture2(char* buf, class CGridData& sdata);	// �����ܰ� ������ ������

	void sendUpjong();
	void ReSend();
	
private:
	int GetChildCount(HTREEITEM	hItem);
	CString CutCount(CString text);
	HTREEITEM FindChild(HTREEITEM hItem, CString text);

	void	loadingInterest(HTREEITEM hParent);
	void	selectinterest(HTREEITEM hParent, DWORD val);
	void	loadingHJcode();
	void	loadingFJcode();
	void	loadingOJcode();
	void	loadingPJcode();
	void	loadingUPcode();
	void	loadingELWBase();
	void	loadingCFcode(bool bCurrency);	//2012.10.04 KSJ �ݸ�(6x), ��ȭ(7x) ��ȭ�� true
	void	loadingSFcode();	//2012.10.04 KSJ �ֽļ���

	void	queryRemain(CString strAccount);
	void	queryFutureRemain(CString strAccount);

	void	queryRemain2(CString strAccount, CString sPswd);
	void	queryFutureRemain2(CString strAccount, CString sPswd);
	BOOL    IsNumber(CString str);

	BOOL	sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt);

	void	queryGroup();
	void	oubGroup(class CRecvData* data);
	void	oubAccUser(char* buf);
	void	oubAccList(char* buf, int len);
	void	oubAccListEx(char* buf, int len);
	void	oubRemain(char* buf, int len);
	void	oubRemainEx(char* buf, int len);
	void	init();
	void	initTree();
	BOOL	IsStockAcc(CString sAcc);
	
	void	saveGroupwhenClose(int index);

	BOOL	SendOper(UINT kind, int opt = 0);
	void	SetItemDataUJ(HTREEITEM hItem, UINT gubn);
	void	SetItemDataX(HTREEITEM hItem, UINT gubn);
	//bool	ExistFile(int gno);
	BOOL	CheckStock(UINT kind, _shjcode* hjcode); 
	BOOL	CheckSearch(_shjcode* hjcode, int opt);
	void	GetStock(class CGridData& sdata, int max, int opt);
	void	GetFuture(class CGridData& sdata, int max);
	void	GetOption(class CGridData& Sdata, int max);
	void	GetJisu(class CGridData& sdata, int max);
	void	GetPjcode(class CGridData& sdata, int max);
	void	GetInterest(class CGridData& sdata, int max);
	void	GetThema(class CGridData& sdata, int max);
	void	GetELWBase(class CGridData& sdata, int max);
	void	GetGroup(class CGridData& sdata, int max);
	void	GetRemain(class CGridData& sdata, int max);	

	void	GetCFuture(class CGridData& sdata, int max, bool bCurrency);	//2012.10.04 KSJ �ݸ�/��ȭ ��ȭ�϶��� true
	void	GetSFuture(class CGridData& sdata, int max);	//2012.10.04 KSJ �ֽļ���

	CString GetCodeName(CString code);


	double  Round(double data );		//�ݿø�
	CString	MakePacket(CString& code, CString amount = _T(""), CString price = _T(""), CString name = _T(""), CString bookmark = _T(""), CString futureGubn=_T(""), CString creditPrc= _T(""), CString maeipPrc= _T(""), CString date = _T(""));
	// END MODIFY
	CString	makeAccn(CString accnnum, CString accnname);
	void	GetAccnTotal(CMapStringToString& mapACCN, CStringArray& arACCN, HTREEITEM hItem);
	void	GetAccnInGroup(CMapStringToString& mapACCN, CStringArray& arACCN, HTREEITEM hItem);
	CString	GetAccName(CString accn);
	CString	GetAccNum(CString accn);
	CString	GetAccGubn(CString accn);
	void	SendTreeData(const class CGridData& sdata);
	BOOL	SetItemData(HTREEITEM hItem, DWORD dwData);	
	BOOL	DeleteItem(HTREEITEM hItem);
	BOOL	DeleteAllItems();
	CString	GetDataTitle(DWORD dwData);
	bool	GetData(class CGridData& gdata, int max, int opt = 0);
	CString GetString(char *pChar, int nMinLen);
	CString Variant(int comm, CString param = _T(""));
	void	RecvOper(int kind, CRecvData* data);
	void	queryNews(int index = -1);
	void	queryAcc();							// ������ȸ
	void	queryStaffAcc(CString ledgerS);					// ����������ȸ
	CString	getLedger(CString sTran, char cGubn, CString sSvcn, CString sLastKey = "");
	void	parsingNews(CRecvData* data);
	void	parsingRecommand(CRecvData* data);

	void	selectKospi();	
	void    SendInterest(HTREEITEM hItem);			//���ٿ��� �׷� Ŭ���� �۵�
	void    saveGroupIndex(int index);				//���� ���ɱ׷� ����

	//ELW �߻�ȸ�纰, �����ڻ꺰 Ʈ�� �߰��� ���� �����Ǵ� ���
	void	getELWBaseAsset(UINT selItem, HTREEITEM parent);	//ELW������ ��� ���� Send������ �Լ�//�����ڻ꺰
	void	getELWIssueSec(UINT selItem, HTREEITEM parent);		//ELW������ ��� ���� Send������ �Լ�//����ȸ�纰
	void    setELWIssueSec(CRecvData* data);					//���� ������,���� ©�� GridWnd�� ������
	void    setELWBaseAsset(CRecvData* data);					//���� ������,���� ©�� GridWnd�� ������

	void	loadingETFcode();

	void	DataGubn(class CGridData& sdata,HTREEITEM hItem);	
	void	DataReloadItem(class CGridData& sdata,CString data);
	void	DataReloadTema(class CGridData& sdata,CString data);
	void	DataReloadFore(class CGridData& sdata,CString data);
	void	DataReloadELWAll(class CGridData& sdata,CString str, CString type);
private:

	CArray <_shjcode, _shjcode>	m_hjcode;		// ��������
	CArray <_sfjcode, _sfjcode>	m_fjcode;		// ��������
	CArray <ojcode, ojcode>		m_ojcode;		// �ɼ�����
	CArray <pjcode, pjcode>		m_pjcode;		// �����ɼ�
	CArray <upcode, upcode>		m_upcode;		// �����ڵ�	
	CArray <_sfjcode, _sfjcode>	m_elwbase;		// elw�����ڻ�

	CArray <ccode, ccode>		m_cicode;		// 2012.10.04 KSJ �ݸ�	
	CArray <ccode, ccode>		m_cccode;		// 2012.10.04 KSJ ��ȭ	
	CArray <sfcode, sfcode>		m_sfcode;		// 2012.10.04 KSJ �ֽļ���	

	std::vector<std::unique_ptr<_efopitem>> _ETFitem;
	std::vector<std::unique_ptr<_efoptema>> _ETFtema;
	std::vector<std::unique_ptr<_efopfore>> _ETFfore;
	std::vector<std::unique_ptr<_efopcode>> _ETFcode;

	CIHStringMapArray		m_mapPJCODE;
	CMap<DWORD, DWORD, HTREEITEM, HTREEITEM>	m_mapParam;
	CWnd*		m_pMainWnd{};
	CString		m_root;
	CString		m_user;
	CString		m_id;
	CString		m_pass;
	char		m_gubn{};
	CString		m_sDeptCode;
	CString		m_szRET;
	
	bool		m_bCustomer{};
	bool		m_bOper{};
	UINT		m_kind{};
	UINT		m_selectTree{};
	UINT		m_selectTreetype{};
	HTREEITEM	m_selItem{};
	CArray <_elwdata, _elwdata> m_ELWdata;

	CStringArray	m_arJango;
	ACCOUNT		m_accn;			// �� ���¸�� hold

	CImageList m_ilTree;
	HTREEITEM _treeInter{};
	std::vector<std::unique_ptr<_intersx>>	_vInters;
	UINT		m_ID{};


	std::array<CGridData, 120> _arrInter;

public:
	void makeInterGroup(std::vector<std::pair<CString, CString>>& vGroup);
	void receiveOub(CString& data, int key);
};
 
