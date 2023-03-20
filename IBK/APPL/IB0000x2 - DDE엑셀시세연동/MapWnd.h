#pragma once
// MapWnd.h : header file
//

#include "../../h/axisfire.h"

#include <afxtempl.h>

#include "EditEx.h"
#include "InterMon.h"

class CfxStatic;
class CfxImgButton;
class CAccountCtrl;
class CfxEdit;
/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

/*******************************************************************************
* �ֽ� �ܰ��� ��ȸ IO FORMAT
******************************************************************************/
struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.�ſ����� 1.All */
    char    fill[10];
};
#define L_smid	sizeof(struct s_mid)				// Length of stock input

struct  grid {
    char    rcod[7];        /* RTS CODE     */
    char    code[6];        /* �����ڵ�     */
    char    hnam[12];       /* �����       */
    char    curr[8];        /* ���簡       */
    char    diff[8];        /* ���ϴ��     */
    char    rate[7];        /* ���         */
    char    gvol[10];       /* �ŷ���       */
    char    xrat[7];        /* ���ϵ��ð� ���  */
};

struct mod {
    char    ssjm[7];        /* ��� / ����      */
    char    nrec[4];        /* # of records     */
    struct  grid    grid[200];
};

struct _code
{
	CString codeS;
	CString nameS;
	char	kosd{};			// ���񱸺�	// 0:�峻  10:���  30:��3����
	char	ssgb{};			// �Ҽӱ���
	// 0:default
	// 4:���߾��ݵ�
	// 5:�����μ���
	// 6:����
	// 7:�����μ�������
	// 8:ETF
	// 9:�����ϵ��ݵ�
};

struct _fcode
{
	CString codeS;
	CString nameS;
};


#define		SJGOLIST				0
#define		SJGOROW					1
#define		GOOPKEY					2
#define		HOGALIST				3
#define		POOPKEY					4
#define		ORDERKEY				5
#define		LOGKEY					6

struct _trkey
{
	UINT	gubn:3;
	UINT	key:7;
};

struct	s_rec	{
	
	char	code[12];	// �����ڵ�
	char	hnam[40];	// �����
    char    jgyh[2];    /* �ܰ�����         */
    char    jqty[10];   /* �ܰ����         */
    char    xqty[10];   /* �ŵ�����         */
    char    pamt[10];   /* ������հ�       */
    char    mamt[15];   /* ���Աݾ�         */
	
    char    curr[10];   /* ���簡     (*)   */
	char    diff[10];   /* ���ϴ��         */
	char    rate[10];   /* �����           */
    char    camt[15];   /* �򰡱ݾ�         */
    char    tamt[15];   /* �򰡼���   (*)   */
    char    srat[10];   /* �򰡼��ͷ� (*)   */
    char    sycd[2];    /* �ſ��ڵ�         */
    char    sydt[8];    /* ������           */
    char    samt[15];   /* �ſ�ݾ�         */
};

struct mid_aa {
    char    find[20];        /* �˻��� */
};

struct	s_mod	{
	char	accn[11];
	char	nrec[4];
	struct	s_rec	rec[1];
};


//struct _inters
//{
//	char	gubn[1];	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
//	char	code[12];	// �����ڵ�
//	char	name[32];	// �����
//	char	xprc[10];	// �����ܰ�
//	char	xnum[10];	// ��������
//	char	xupnum[16];	// ���� �ֽļ�
//	char	filler[24];	// Reserved
//};
#define	sz_inters	sizeof(struct _inters)

struct _listitem {		// tree1 ���ý� �������� list1 item
	char	code[12];
	char	name[36];
	_inters* pInter;
};

#define	CHANGEACCOUNT			100
#define	tabCHAR					'/t'//0x09

#define CTRL_GAP 6

#define	BACKINDEXCOLOR	64
#define	TOOLINDEXCOLOR	66
#define	ROUND_PANEL		1
#define	ROUND_CONTENTS	2

#define	saveFILE	"portfolio.ini"
#define	tempFILE	"portfolio.ini.tmp"

#define STR_COMBODATA_ALL		"��ü"
#define STR_COMBODATA_THEME		"����"
#define STR_COMBODATA_REMAINDER "�ܰ�"
#define STR_COMBODATA_INTEREST  "��������"

////////////////////////// 
// Ŭ������	    : CMapWnd
// �� ��	    : ��ü,�׸�,�ܰ�,�������� ���� ���� �� �������� �׸��� �˾Ƴ��� 
//				  ���� ���Ͽ� �����͸� �����ִ� ���
// �ۼ��Ͻ�	    : 2009/05/26
// �ۼ���       : kbkim(������)
// ���ÿ��	    : 000010(������:2009/05/12)
////////////////////////// 
class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd* parent);

// Attributes
public:
	CWnd*	m_parent;

private:
	int sel_idx;
	CString	m_root;
	CFont*	m_font;
	COLORREF m_clrBG;
	CBrush	m_hbrush;

	std::unique_ptr<InterMon> inter;

	enum	ctype { cKospi=0, cKosdaq, cFutures, cOption, cIndex, cElw, cProgram, cTujaja } m_type;
	enum	Sel_type { SEL_ALL=0, SEL_THEME, SEL_REMAINDER, SEL_INTEREST, SEL_KRX} m_SearchType;
	enum	tId { xNONE = 0, xALLCODE, xKOSPI, xKOSDAQ, xINTEREST, xTHEMA, xGROUP, xFUTURE, xCALL, xPUT, xETF, xELW};
	enum	{ CG_NOT = 0, CG_KOSPI, CG_KOSDAQ, CG_FUTURE, CG_CALL, CG_PUT };

	std::unique_ptr < CComboBox>	m_pSelList;

	CArray<_code, _code> m_arCode;
	CStringList m_CodeList;
	
	// list control
	std::unique_ptr < CListCtrl>	m_code;		// all code
	std::unique_ptr < CListCtrl>	m_item;		// all item
	std::unique_ptr < CListCtrl>	m_scode;	// selected code
	std::unique_ptr < CListCtrl>	m_sitem;	// selected item

	// combobox
	std::unique_ptr < CComboBox>	m_rtype;	// right type
	std::unique_ptr < CComboBox>	m_bstock;	// base stock
	std::unique_ptr < CComboBox>	m_icorp;	// issue corp
	std::unique_ptr < CComboBox>	m_month;	// due month

	std::unique_ptr < CComboBox>	m_tujatype1;
	std::unique_ptr < CComboBox>	m_tujatype2;

	CString		m_strPal;
	HBRUSH      m_hRoundColor;
	COLORREF	m_clrRoundColor;
	
	// ��ü
	std::unique_ptr < CButton>	m_pGrp1;
	std::unique_ptr < CButton>	m_pKospi;
	std::unique_ptr < CButton>	m_pKosdaq;
	std::unique_ptr < CButton>	m_pFuture;
	std::unique_ptr < CButton>	m_pOption;
	std::unique_ptr < CButton>	m_pIndex;
	std::unique_ptr < CButton>	m_pElw;
	std::unique_ptr<CButton>	m_pProgram;
	std::unique_ptr < CButton>	m_pTujaja;

	// �׸�
	std::unique_ptr < CfxStatic>	m_pThemeLabel;
	std::unique_ptr < CEditEx>	m_pThemeSearch;
	std::unique_ptr < CComboBox>	m_pThemeList;

	// �ܰ�
	std::unique_ptr < CfxStatic>		m_pRemainderLabel;
	std::unique_ptr < CAccountCtrl>	m_pRemainderAcc;
	std::unique_ptr < CStatic>		m_pRemainderName;
	std::unique_ptr < CfxEdit>		m_pRemainderPassword;
	CWnd*			m_pWizard;
	CString			m_strAccount;
	CString			m_strPassword;

	// ��������
	std::unique_ptr < CfxStatic>	m_pInterestLabel;
	std::unique_ptr < CComboBox>	m_pInterestGroupList;
	CArray <_inters*,   _inters*>   m_inters;		// interest file_load

	// KRX
	std::unique_ptr < CfxStatic>	m_pKRXLabel;
	std::unique_ptr < CComboBox>	m_pKRXList;
	std::unique_ptr < CfxStatic>	m_pKRXName;

	// code add/del button
	std::unique_ptr < CfxImgButton>	m_code_addA;
	std::unique_ptr < CfxImgButton>	m_code_delA;
	std::unique_ptr < CfxImgButton>	m_code_add;
	std::unique_ptr < CfxImgButton>	m_code_del;

	// item add/del button
	std::unique_ptr < CfxImgButton>	m_item_addA;
	std::unique_ptr < CfxImgButton>	m_item_delA;
	std::unique_ptr < CfxImgButton>	m_item_add;
	std::unique_ptr < CfxImgButton>	m_item_del;

	// radio button for display
	std::unique_ptr < CButton>	m_grp2;
	std::unique_ptr < CButton>	m_horz;
	std::unique_ptr < CButton>	m_vert;
	std::unique_ptr < CButton>	m_number;	//2013.01.18 KSJ ������ ���ں�ȯ ���(�⺻ üũ�ȵ�)
	bool		m_bCheck;	//üũ�Ǿ�����
	bool		m_bAddAll;		// �����Է½� �Ź� Ŭ���� �ϴ°� �����ϱ� ����

	// excel
	std::unique_ptr < CfxImgButton>	m_excel;

	// item string
	CString		m_itemKey;

	//�ڽ���, �ڽ��� ����˻�
	std::unique_ptr < CfxStatic>	m_pNameLabel;
	std::unique_ptr < CEditEx>	m_pNameSearch;
	//KSJ

	CMapStringToPtr m_codedata;
//	CArray <_listitem*, _listitem*> m_listitem;		// m_list1 item
	//CArray <_listitem*, _listitem*> m_searchlist;		// m_list1 item
	std::vector<shared_ptr<_listitem>> m_listitem;
	std::vector<shared_ptr<_listitem>> m_searchlist;

// Operations
public:
	BOOL	CreateMap();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	public:
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	COLORREF GetAxColor(int iColor);
	void	ChangeColors();
	CString GetPalette();
 	void	DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
 	void	DrawBitmap(CDC *pDC, CRect rc, CString strImage);

	void	CtrlAllHidden();
	void	CtrlAllShow();
	void	CtrlThemeShow();
	void	CtrlRemainderShow();
	void	CtrlInterestShow();
	void	CtrlKRXShow();

	void	CreateSearchCtrl();
	void	CreateSearchCtrl_All();
	void	CreateSearchCtrl_Theme();
	void	CreateSearchCtrl_Remainder();
	void	CreateSearchCtrl_Interest();
	void	CreateSearchCtrl_KRX();
	
	CString	m_sTabDir;
	BOOL	loadJCode();
	void	SelThemeList() ;
	CString ParserThemeList(CString &srcstr, CString substr);
	CString getCodeName(CString codeS, int& gubn);
	void	SetIndexCode();

	void	OnRemain();
	void	SetThemeComboList(CString strGetData);
	int		SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt);

	CString Variant(int nComm, CString strData);
	CString Parser(CString &strSrc, char cDel);
	BOOL	SendTR(CString strTR, CString strData, int nKey, int nStat);
	void	SetThemeGroupName(mod *lsmod);
	void	SetRemainList(char* szReceive);
	BOOL	GetLedgerMessage(CString sLedger, CString& sMessage, BOOL& bNext);
	int		SetJangoFormat(CString sRows, CString& strCode, CString& strName);
	void	SetInterest();
	CString parseX(CString &srcstr, CString substr);
	int		loadingInterest(int gno);
	BOOL	ExistFile(int gno, bool temp);

	CBrush	m_br;
	void	create();
	CString	GetAxName(CString code);
	int		l_dudcode(int code);
	void	loadCode();
	void	loadItem();
	void	loadItemKey();

	void	addCode(CString code, CString name);
	BOOL	addColumn(CListCtrl* list, CString columns, int col, int width);
	BOOL	addItem(CListCtrl* list, CString items, CString subs, int item);
	void	removeAllItems();
	char* readCodeFile0(int* size, char* codefile);
	std::unique_ptr<char []> readCodeFile(int* size, char* codefile);

	void	addItem(CListCtrl* src, CListCtrl* dest, bool code, bool all = false);
	void	deleteItem(CListCtrl* dest, bool all = false);
	bool	checkDuplication(CListCtrl* dest, CString text);
	void	setElwCombo();
	void	settujaCombo();
	void	loadTUJA();
	void	search();
	void	runExcel();

	//���� �˻������� �߰���
	void	setSearchName();	//2013.02.28 KSJ �ڽ���, �ڽ��� ����˻�
	void	searchAction(int column);
	int		findMatchIndex(CString text);
	int		findMatchIndex(CString text, int column);
	void	AppendItemToList1(int ipos, CString code, CString name,  _listitem *pListItem);
	void	selcMark(CListCtrl* listctrl, int nItem, bool scroll = true);
	void	deselcMark(CListCtrl* listctrl);
	bool	SearchData(CString sName, bool bAddAll);
	WORD	MakeHangul(char cChar1, char cChar2);	// 2Byte���ڷκ��� �ѱ��� �ͱ۾��ֶ�~~
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);	// �ʼ��� ���� �ѱ�����~~
	//KSJ 

	BOOL	checkAddItem( CListCtrl* pSrc, CListCtrl* pDest, BOOL bAll );
	
	//CPtrArray m_arUPCODE;

	std::vector<shared_ptr<struct upcode>> m_arUPCODE;

	COLORREF getColor(int color);
	CFont*	getFont(int point, CString name = _T(""), int bold = FW_NORMAL, bool italic = false);
	HBITMAP	getBitmap(CString bmps);
	long	variant(int key, CString data = _T(""));
	int		getCodeType(CString strcode);
	CBrush* getAxBrush(COLORREF clr);
	COLORREF GetIndexColor(UINT nIndex)
	{
		if (!m_parent)
			return 0;
		
		if (nIndex & 0x02000000)
			return nIndex;

		return m_parent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
	}
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSelChangeSearchCombo();
	afx_msg void OnSelChangeThemeList();
	afx_msg void OnSelChangeGroupList();
	afx_msg void OnSelChangeKRXTypeList();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditExMsg(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

