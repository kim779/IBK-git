#pragma once
// TreeWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeWnd window
#include "../IhUtils.h"
#include "../../h/jmcode.h"
#include "../../h/interst.h"
#include "Account.h"

struct _shjcode			// semi hjcode
{
	CString code;
	CString name;
	CString symb;		// symbol
	char	ecng;		// ECN ����
	char	size;		// �ں��ݱԸ�	2:��, 3:��, 4:��
//	char	ucdm;		// �����ߺз�
//	char	ucds;		// �����Һз�
	char	jjug;		// ������ ����(27: ������)
	char	kpgb;		// KOSPI200 ����
	char	kosd;		// ���񱸺�
	char	ssgb;		// �Ҽӱ���
	char	ucmd;		// 0:������, 5:�켱��
	char	wsgb;		// 0:�Ϲ� 1:���豸��������
	char	jsiz;		// �����ں��� ���߼�
	char	itgb;		// KOSPI-IT/KQ-IT50����(1:ä��)
	char	star;		// KOSDAQ ��Ÿ����
	char	unio;		// �������� (KRX100)
	char	prmr;		// �����̾�����
	char    jchk;		// ����˻��� ����
				// 0x01 �Ҽ��ǰ�������
				// 0x02 �űԻ�������
				// 0x04	�Ǹ�������
				// 0x08	�켱������
				// 0x10 ��������
				// 0x20	��������
				// 0x40 �ŷ���������
				// 0x80	������������
	char    acdl[6];       	/* ������з�                      <--- �߰�			*/
	char    acdm[6];       	/* �����ߺз�                      <--- �߰�, AS-IS�� ucdm	*/
	char    acds[6];       	/* �����Һз�                     <--- �߰�, AS-IS�� ucds	*/

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

	inline Copy(_acSave const &other)
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

class	_gpSave	
{
public:
	CString sGrSortNum;		// �׷��Ϸù�ȣ
	CString sGrName;		// �׷��
	CString sAccntCnt;		// ���°���
	CString	sGroupID;		// �׷���̵�
	CIHArray <_acSave *, _acSave *>	arAcEdit;
public:
	_gpSave()
	{
		sGrSortNum = _T("");
		sGrName = _T("");
		sAccntCnt = _T("");
		sGroupID = _T("");
		arAcEdit.RemoveAll();
	};

	~_gpSave()
	{
	};

	_gpSave(_gpSave const &other)
	{
		Copy(other);
	};

	inline Copy(_gpSave const &other)
	{
		sGrSortNum = other.sGrSortNum;
		sGrName = other.sGrName;
		sAccntCnt = other.sAccntCnt;
		sGroupID = other.sGroupID;

		_acSave* acSave = NULL;
		_acSave* newSave = NULL;
		for (int ii = 0; ii < other.arAcEdit.GetSize(); ii++)
		{
			acSave = other.arAcEdit.GetAt(ii);
			if (acSave)
			{
				newSave = new _acSave();
				newSave->Copy(*acSave);
				arAcEdit.Add(newSave);
			}
		}
	};
};

class CAccn
{
public:
	bool    m_bFuture;
	CString   m_accn;
	CString   m_pswd;
	CString   m_errmsg;
	bool    m_bCredit;    // true:�ſ�����, false:�ſ�����
	int     m_dFee; //������������ ���� 1 ������ 0
	double    m_dMass; //��ü���� 0:hts 1:������/�� 2.ARS
	double    m_dSave; //��ü���� 0:hts 1:������/�� 2.ARS
	
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
	void clickRecommand();

	void settingDataStock(char* buf, class CGridData& sdata);	// �ֽ��ܰ� ������ ������
	void settingDataFuture(char* buf, class CGridData& sdata);	// �����ܰ� ������ ������
	void settingDataStock2(char* buf, class CGridData& sdata);	// �ֽ��ܰ� ������ ������
	void settingDataFuture2(char* buf, class CGridData& sdata);	// �����ܰ� ������ ������

	void sendUpjong();

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
	bool	ExistFile(int gno);
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
	void	GetRecommand(class CGridData& sdata, int max);	

	CString GetCodeName(CString code);


	// MODIFY PSH 20070914
	//CString	MakePacket(CString& code, CString amount = _T(""), CString price = _T(""));
	double  Round(double data );		//�ݿø�
	CString	MakePacket(CString& code, CString amount = _T(""), CString price = _T(""), CString name = _T(""), CString bookmark = _T(""), CString futureGubn=_T(""), CString creditPrc= _T(""), CString maeipPrc= _T(""));
	// END MODIFY
	CString	makeAccn(CString accnnum, CString accnname);
	void	GetAccnTotal(CMapStringToString& mapACCN, CStringArray& arACCN, HTREEITEM hItem);
	void	GetAccnInGroup(CMapStringToString& mapACCN, CStringArray& arACCN, HTREEITEM hItem);
	CString	GetAccName(CString accn);
	CString	GetAccNum(CString accn);
	CString	GetAccGubn(CString accn);
	void	SendTreeData(class CGridData& sdata);
	void	SendRemainData(class CGridData& sdata);
	BOOL	SetItemData(HTREEITEM hItem, DWORD dwData);	
	BOOL	DeleteItem(HTREEITEM hItem);
	BOOL	DeleteAllItems();
	CString	GetDataTitle(DWORD dwData);
	void	GetData(class CGridData& gdata, int max, int opt = 0);
	void	InitData(int kind, int nIndex);	
	CString GetString(char *pChar, int nMinLen);
	CString Variant(int comm, CString param = _T(""));
	void	RecvOper(int kind, CRecvData* data);
	void	queryNews();
	void	queryAcc();							// ������ȸ
	void	queryStaffAcc(CString ledgerS);					// ����������ȸ
	CString	getLedger(CString sTran, char cGubn, CString sSvcn, CString sLastKey = "");
	void	parsingNews(CRecvData* data);

	void	selectKospi();	
	void    SendInterest(HTREEITEM hItem);			//���ٿ��� �׷� Ŭ���� �۵�
	void    saveGroupIndex(int index);				//���� ���ɱ׷� ����


	//ELW �߻�ȸ�纰, �����ڻ꺰 Ʈ�� �߰��� ���� �����Ǵ� ���
	void	getELWBaseAsset(UINT selItem, HTREEITEM parent);	//ELW������ ��� ���� Send������ �Լ�//�����ڻ꺰
	void	getELWIssueSec(UINT selItem, HTREEITEM parent);		//ELW������ ��� ���� Send������ �Լ�//����ȸ�纰
	void    setELWIssueSec(CRecvData* data);					//���� ������,���� ©�� GridWnd�� ������
	void    setELWBaseAsset(CRecvData* data);					//���� ������,���� ©�� GridWnd�� ������
private:

	CArray <_shjcode, _shjcode>	m_hjcode;		// ��������
	CArray <_sfjcode, _sfjcode>	m_fjcode;		// ��������
	CArray <ojcode, ojcode>		m_ojcode;		// �ɼ�����
	CArray <pjcode, pjcode>		m_pjcode;		// �����ɼ�
	CArray <upcode, upcode>		m_upcode;		// �����ڵ�	
	CArray <_sfjcode, _sfjcode>	m_elwbase;		// elw�����ڻ�

	CIHStringMapArray		m_mapPJCODE;
	CMap<DWORD, DWORD, HTREEITEM, HTREEITEM>	m_mapParam;
	CWnd*		m_pMainWnd;
	CString		m_root;
	CString		m_user;
	CString		m_id;
	CString		m_pass;
	char		m_gubn;
	CString		m_sDeptCode;
	CString		m_szRET;
	
	bool		m_bCustomer;
	bool		m_bOper;
	UINT		m_kind;
	UINT		m_selectTree;
	UINT		m_selectTreetype;

	CStringArray	m_arJango;

	ACCOUNT		m_accn;			// �� ���¸�� hold

	CImageList m_ilTree;
};
 
