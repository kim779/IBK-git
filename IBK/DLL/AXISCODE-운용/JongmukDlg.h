#pragma once

// JongmukDlg.h : header file
//

#include "resource.h"
#include "user.h"
#include "btnst.h"
#include "sfont.h"
#include "skinlistctrl.h"
#include "EditX.h"
#include "../../h/jmcode.h"
#include "axiscode.h"
#include <afxtempl.h>

#define	ALLTYPE	       	0
#define KOSPITYPE	1
#define KOSDAQTYPE     -1
#define	ELWTYPE		2
#define REITSTYPE	4
#define OTHERTYPE	3
#define KONEXTYPE	5	//2013.06.05 KSJ �ڳؽ� �߰�

//typedef 



#define	USCODE_FILE	"uscode.dat"
struct  _USCode {			/* �ؿ� SYMBOL ����             */
        char    symb[16];               /* ���� SYMBOL                  */
        char    dat1;                   /* ��з�			*/
					/* 0: ����      1: ����		*/
					/* 2: ����      3: �ݸ�		*/
					/* 4: ��ä      5: ȯ��		*/
					/* 7: others			*/
        char    dat2;                   /* �ߺз�(Combo Box)            */
					/* code09.jsgb                  */
					/* [����]                       */
					/* 1. �̱� 2.�ƽþ� 3.���� 4. MSCI 5. ��Ÿ */
					/* [����]                       */
					/* 1. �̱� 2.�Ϻ� 3.ȫ�� 4.�븸 5.���� 6.������ 7.���� 8.ADR 9.GDR */
					/* [����]                       */
					/* 1.CME�������� 2.������ 3.��ǰ����	*/
					/* [�ݸ�]                       */
					/* 1.�����ݸ�   2.�̱��ݸ�	*/
					/* [��ä]                       */
					/* 1.�̱� 2.�Ϻ� 3.���� 4.����	*/
					/* [ȯ��]                       */
					/* ����ȯ��                     */
        char    knam[40];               /* �ѱ� SYMBOL��                */
        char    enam[40];               /* ���� SYMBOL��                */
        char    icod[6];                /* ���� �����ڵ�                */
        char    ncod[3];                /* �����ڵ�                     */
        char    excd[4];                /* �ŷ����ڵ�                   */
};

typedef struct flist
{
	CString code;
	CString name;
} FLIST;

// typedef struct _pbcode
// {
// 	CString		code;	// ����� �ڵ�
// 	CString		name;	// ������
// }PBCODE;


static BOOL m_gListsort;

/////////////////////////////////////////////////////////////////////////////
// CJongmukDlg dialog

class CJongmukDlg : public CDialog
{
// Construction
public:
	void ListReitsSort(int nItem);
	CJongmukDlg(int kind, int type, CPoint pt = CPoint(-1, -1), CWnd* pParent = nullptr);   // standard constructor

	void	DeleteAllItem();
	void	Filllistoption(int sel);
	void	Hideall(int flag);
	void	InitOption();
	
	void	Set_KS_HanMode();
	bool	IsCodeType(CString code, int type);
	void	SetDlgPos(int x, int y);
	void	SetListCtrl();
	void	DeleteAllData();
	CString GetCode() { return m_szCode; }
	CString GetName() { return m_szName; }
	int     GetJongmuk() {	return m_jongmuk; }
	void	SetCode(CString szCode){ m_szCode = szCode; }
	void	SetName(CString szName){ m_szName = szName; }
	BOOL	FindCode(int kind, CString szCode, int type, bool bNoDelete = false);
	BOOL	LoadUPCode();
	BOOL	LoadELWCode();
	void	SelectETF();
	void	ApplyELW(CString code);
	void	ApplyELWBase(CString code);
	hjcodex*	FindHJCode(CString sCode);
	void	ListRemoveAll(bool select = true);
	void	InitBtn();
	BOOL	LoadUSCode();
	BOOL	loadFjCode();
	BOOL	loadOjCode();
	//CString	CompareQuote(CString szCode);
	CString	CompareFuture(CString szCode);
	void	clearEditSearch();
	bool	CheckDigit(CString code);
	hjcodex* FindOnlyELW(CString sCode);
	int	FindPBCode(CString code);
	CString FindBaseCode(CString company);
	CString FindPBcompany(CString company);
	void	ELWKospi200();
	void	ElwLoadGen();
	void	ElwLoadJogi();

	void	ETFLoadItem(CString code);
	void	ETFLoadTema(CString tmcd);
	void	ETFLoadFore(CString frcd);

	CString	FindETFItem(CString company);
	CString FindETFTema(CString tema);
	CString FindETFFore(CString fore);

	void	InitialETFTree(HTREEITEM hRoot);

	// Dialog Data
	//{{AFX_DATA(CJongmukDlg)
	enum { IDD = IDD_DIALOG_STOCKCODEINFO };
	CButtonST	m_ctlButtonKONEXWORK;
	CButtonST	m_ctlBtnPremier;
	CComboBox	m_ComboSearchKind;
	CButtonST	m_BtnHangul22;
	CButtonST	m_BtnHangul21;
	CButtonST	m_BtnHangul20;
	CButtonST	m_BtnHangul19;
	CButtonST	m_BtnHangul18;
	CButtonST	m_BtnHangul17;
	CButtonST	m_BtnHangul9;
	CButtonST	m_BtnHangul8;
	CButtonST	m_BtnHangul7;
	CButtonST	m_BtnHangul6;
	CButtonST	m_BtnHangul5;
	CButtonST	m_BtnHangul4;
	CButtonST	m_BtnHangul3;
	CButtonST	m_BtnHangul2;
	CButtonST	m_BtnHangul16;
	CButtonST	m_BtnHangul15;
	CButtonST	m_BtnHangul14;
	CButtonST	m_BtnHangul13;
	CButtonST	m_BtnHangul12;
	CButtonST	m_BtnHangul11;
	CButtonST	m_BtnHangul10;
	CButtonST	m_BtnHangul1;
	CEditX		m_EditSearch;
	CButtonST	m_CtlButtonThirdJS;
	CButtonST	m_CtlButtonSector;
	CTreeCtrl	m_Tree;
	CButtonST	m_ctlButtonELW;
	CButtonST	m_ctlButtonSINJU;
	CListBox	m_ListPut;
	CListBox	m_ListCall;
	CButtonST	m_CtlButFuture4;
	CButtonST	m_CtlButFuture3;
	CButtonST	m_CtlButFuture2;
	CButtonST	m_CtlButFuture1;
	CSkinListCtrl	m_ListCtrl;
	CButtonST	m_ctlButtonTHIRD;
	CButtonST	m_ctlButtonKOSPIWORK;
	CButtonST	m_ctlButtonKOSPI;
	CButtonST	m_ctlButtonKOSDAQWORK;
	CButtonST	m_ctlButtonKOSDAQ;
	CButtonST	m_ctlButtonECN;
	CButtonST	m_ctlButtonAll;
	CComboBox	m_ComboGroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJongmukDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	BtnSetup();
	void	Part();	

	void	ListKosDaqSort(int nItem);
	void	ListKospiSort(int nItem);
	BOOL	ListAllWorkSort(int nItem);
	void	AddGrid(CString& str, int flag);
	void	AddCombo(CString& szText);
	void	HideButton(BOOL flag = FALSE);
	void	MoveGrid(int kind = 1);
	BOOL	CodeListAll();
	BOOL	CodeListSome(vector<HCODEX*>& arr, bool bflag = true);
	void	AddItem(int row, CString szCode, CString szName);
	void    InsertItem(int row, CString szCode, CString szName);
	CString Parser(CString &srcstr, CString substr);
	bool	FileCheck(int fNo);
	void	GetExCode();
	void	ShowTreeView();

	void	LoadFolder(CString path, HTREEITEM hItem, CString sSection, int nKey);
	void	LoadGroup(CString path, HTREEITEM hItem);
	void	baseCX_Category();

	CString LoadCodeList(int nGroup);
	//void	ListUpjongSort(int nidx, hjcode* hjc, int ntype,CPtrArray& arr);
	void	ProcPart(int ntype,int nSubType);

	CAxisCodeApp* m_pApp;

	int	m_nRadioOption;	
	CSFont	m_Font;
	int	m_select;

	enum ELW_FILTER { ewALL, ewNormal, ewKoba };
	ELW_FILTER m_ewFilter;

	char*	m_fcB;
	char*	m_ocB;

	std::map<CString, struct hjcodex*> _mapCODE;
	
	CPtrArray m_arFCODE;
	CStringArray m_arMONTH;
	CPtrArray m_arOCODE;
	CPtrArray m_arUPCODE;
	//CArray	<CStringArray*, CStringArray*>	m_codeArr;
	std::vector<std::unique_ptr<CStringArray>> m_codeArr;
	CArray<_elwcode, _elwcode>   m_arrayELWCode;
	CArray<_elwcode, _elwcode>   m_arrayELWGenCode;
	CArray<_elwcode, _elwcode>   m_arrayELWJogiCode;
	CArray<PBCODE, PBCODE>       m_arrayPBCode;
	std::vector<_basecode>	     m_arrayBASECode;


	CString	m_szCode;
	CString	m_szName;
	int	m_jongmuk;
	int	m_kind;
	int	m_Type;
	HBRUSH	m_hBrush;
	CPoint	m_point;
	BOOL	m_bOption;
	
	std::vector <std::shared_ptr<_USCode>> m_arUSCODE;

	CString	m_root, m_user;
	
	struct _exCode {
		CString code;
		CString name;
	};
	CArray <_exCode, _exCode> m_exCode[2];
	
	// Generated message map functions
	//{{AFX_MSG(CJongmukDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
//	afx_msg void OnButtonAllcode();
//	afx_msg void OnButtonKospicode();
//	afx_msg void OnButtonKosdaqcode();
//	afx_msg void OnButtonEcncode();
//	afx_msg void OnButtonThirdmarket();
	afx_msg void OnButtonKospiwork();
	afx_msg void OnButtonKosdaqwork();
	afx_msg void OnButtonOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnButtonFuture1();
	afx_msg void OnButtonFuture2();
	afx_msg void OnButtonFuture3();
	afx_msg void OnButtonFuture4();
	afx_msg void OnDoubleclickedButtonFuture1();
	afx_msg void OnDoubleclickedButtonFuture2();
	afx_msg void OnDoubleclickedButtonFuture3();
	afx_msg void OnDoubleclickedButtonFuture4();
	afx_msg void OnDblclkListCall();
	afx_msg void OnDblclkListPut();
//	afx_msg void OnButtonSinju();
//	afx_msg void OnButtonElw();
	afx_msg void OnDblclkTreeBunryu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeBunryu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSector();
	afx_msg void OnButtonThirdjs();
	afx_msg void OnSelchangeComboGroup();
	afx_msg void OnSelchangeStaticSearch();
	afx_msg void OnButtonPremier();
	afx_msg void OnButtonKonexwork();
	afx_msg void OnGetdispinfoListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnRadio(UINT nID);
	afx_msg void OnClickBtnSort(UINT nID);
	afx_msg long OnEditXMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:	// added by kwon
	bool	m_bAddAll;	// ListCtrl�� �ߺ� Ŭ��� �����ϱ� ����
	void	searchAction(int column);
	int	findMatchIndex(CString text, int column);
	bool	SearchData(int kind, CString sName, bool bAddAll);
	bool	SearchJongmok(CString sName, bool bAddAll);
	bool	SearchWord(CString sName, bool bAddAll);
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);
	WORD	MakeHangul(char cChar1, char cChar2);
	void    listsort(int column);

	std::vector<HCODEX*> _vSCODE;
	std::vector<std::pair<CString, CString>> _vCODE;

	CString _sData;
};

