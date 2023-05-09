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
	char	ecng{};		// ECN 구분
	char	size{};		// 자본금규모	2:대, 3:중, 4:소
	char	ucdm{};		// 업종중분류
	char	ucds{};		// 업종소분류
	char	jjug{};		// 제조업 구분(27: 제조업)
	char	kpgb{};		// KOSPI200 구분
	char	kosd{};		// 종목구분
	char	ssgb{};		// 소속구분
	char	ucmd{};		// 0:보통주, 5:우선주
	char	wsgb{};		// 0:일반 1:지배구조우수기업
	char	jsiz{};		// 업종자본금 대중소
	char	itgb{};		// KOSPI-IT/KQ-IT50구분(1:채택)
	char	star{};		// KOSDAQ 스타지수
	char	unio{};		// 통합지수 (KRX100)
	char	prmr{};		// 프리미어지수
	char    jchk{};		// 종목검색용 구분
				// 0x01 불성실공시종목
				// 0x02 신규상장종목
				// 0x04	권리락종목
				// 0x08	우선주종목
				// 0x10 관리종목
				// 0x20	감리종목
				// 0x40 거래정지종목
				// 0x80	투자유의종목
};

struct _sfjcode			// semi fjcode
{
	CString code;
	CString name;
};

struct  _acInfo {
    char    accn[10];			// 계좌번호
    char    acnm[20];			// 계좌명
    char    aseq[2];			// 계좌순서
					// 00: 기본계좌
					// 01: 화면상에 보이지 않는계좌
    char    pwch[1];			// 은행연계계좌여부
};
#define	L_acInfo	sizeof(struct _acInfo)

struct  _acSet {
    char    func[1];			// FUNCTION CODE
					// 'Q' Query    'I' Insert
					// 'U' Update
    char    usid[12];			// 사용자아이디
    char    errc[1];			// Error Code
    char    emsg[80];			// Error message
    char    nrec[4];			// Account Count
    //struct  _acInfo acinfo[1];
};
#define	L_acSet		sizeof(struct _acSet)


class 	_acSave	
{
public:
	CString sPriAcc;	//대표계좌구분
	CString sSortNum;	//계좌일련번호
	CString sAccntNum;	//계좌번호
	CString sAccntName;	//계좌이름
	CString sAllocRate;	//배정비율
	CString	sAllocMulti;	//승수
	CString sAccntNick;	//계좌별칭
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
	bool    m_bCredit{};    // true:신용포함, false:신용제외
	int     m_dFee{}; //수수료제세금 적용 1 미적용 0
	double    m_dMass{}; //매체구분 0:hts 1:영업점/콜 2.ARS
	double    m_dSave{}; //매체구분 0:hts 1:영업점/콜 2.ARS
	
	CMapStringToPtr m_CodeMap;    // Ptr: F->class CRemainFuture
	//      C->class CRemain
	
public:
	CAccn()
	{
		m_bFuture = false;
		m_bCredit = false;
		m_accn = "";
		m_pswd = m_errmsg = "";
		m_dFee = 0; //수수료제세금 적용 1 미적용 0
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
	void settingDataStock2(char* buf, class CGridData& sdata);	// 주식잔고 데이터 빼내기
	void settingDataFuture2(char* buf, class CGridData& sdata);	// 선물잔고 데이터 빼내기

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
	void    SendInterest(int item);			//툴바에서 그룹 클릭시 작동

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

	ACCOUNT		m_accn;			// 내 계좌목록 hold
	CStringArray		m_accnT;

public:
	void receiveOub(CString& data, int key);
};
 