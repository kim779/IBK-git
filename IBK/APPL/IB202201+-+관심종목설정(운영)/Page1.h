#pragma once
// Page1.h : header file
//
#include "afxtempl.h"
#include "editx.h"
#include "../../h/jmcode.h"
#include "../../h/interst.h"


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

struct _listitem
{ // tree1 ���ý� �������� list1 item
	char code[12]{};
	char name[36]{};
	//_inters *pInter{};
};

struct _listitem2
{ // tree1 ���ý� �������� list1 item
	char code[12]{};
	char name[36]{};
};

struct _elwdata
{
	CString code;  // ELW �����ڵ�
	CString name;  // �����
	CString ktype; // �Ǹ����� 01:Call, 02:Put, 03:��Ÿ
	CString pcode; // ����� �ڵ�
	CString pname; // ������
	CString mjmt;  // ����� (YYYYMMDD)
	CString bcode[5];
	BOOL bkoba{}; // ��������ELW����
};

struct _efopcode
{
	char type[1]{}; /*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char code[6]{};	 /*�����ڵ�*/
	char hnam[39]{}; /*�����*/
	char opcd[6]{};	 /*���� �ڵ�*/
	char etfm[1]{};	 /*�׸��ڵ�*/
	char etfl[1]{};	 /*������ ����*/
	char lnfd[1]{};
};

struct _efopitem
{
	char type[1]{}; /*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char opcd[6]{};	 /*���� �ڵ�*/
	char hnam[30]{}; /*���� �ѱ۾��*/
	char lnfd[1]{};	 /*�����ǵ�*/

	//	_efopcode *efopcode;
};

struct _efoptema
{
	char type[1]{}; /*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char tmcd[1]{};	 /*�׸��ڵ�*/
	char tmnm[20]{}; /*�׸���*/
	char lnfd[1]{};	 /*�����ǵ�*/
};

struct _efopfore
{
	char type[1]{}; /*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char frcd[1]{};			   /*������ ����('D':����, 'F':�ؿ�)*/
	char frnm[10]{}; /*������ ���и�*/ // 2015.11.18 KSJ �ڸ��� 4 -> 10
	char lnfd[1]{};			   /*�����ǵ�*/
};


typedef struct listitem
{
	CString code;
	CString name;
	int bkoba{};
	CString ktype;
} ITEMS;

typedef struct codelistitem
{
	CString code;
	CString name;

} CITEMS;

#define WM_TOPAGE1 (WM_USER + 119)
#define WM_TOPAGE2 (WM_USER + 139)
#define WM_TOSELECTPAGE (WM_USER + 129)
#define WM_TOSELECTGROUPNCODE (WM_USER + 149)
#define MAXGROUP 100

static BOOL m_gListsort;

/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog

class CPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1)

	// Construction
public:
	CPage1(CWnd *parent = nullptr);
	~CPage1();

	// Dialog Data
	//{{AFX_DATA(CPage1)
	enum
	{
		IDD = IDD_PAGE01
	};
	CButton m_btnMoveTop;
	CButton m_btnMoveBtm;
	CButton m_btn_DelAll;
	CButton m_btn_ShareImport;
	CButton m_btn_ShareExport;
	CButton m_btn_newGroup;
	CButton m_btn_MoveUp;
	CButton m_btn_MoveDn;
	CButton m_btn_Blank;
	CButton m_btn_AllSave;
	CButton m_btn_AddAll;
	CButton m_btn_Add;
	CButton m_editGroup;
	CButton m_btn_dnLoadR;
	CButton m_btn_dnLoad;
	CButton m_btn_Del;
	CButton m_btnSort22;
	CButton m_btnSort21;
	CButton m_btnSort20;
	CButton m_btnSort19;
	CButton m_btnSort18;
	CButton m_btnSort17;
	CButton m_btnSort16;
	CButton m_btnSort15;
	CButton m_btnSort14;
	CButton m_btnSort13;
	CButton m_btnSort12;
	CButton m_btnSort11;
	CButton m_btnSort10;
	CButton m_btnSort09;
	CButton m_btnSort08;
	CButton m_btnSort07;
	CButton m_btnSort06;
	CButton m_btnSort05;
	CButton m_btnSort04;
	CButton m_btnSort03;
	CButton m_btnSort02;
	CButton m_btnSort01;
	CComboBox m_gname;
	CStatic m_sum;
	CComboBox m_cbSearch;
	CEditX m_search;
	CTreeCtrl m_tree;
	CListCtrl m_list2;
	CListCtrl m_list1;
	CButton m_Radio_ELWAll;
	CButton m_Radio_ELWCall;
	CButton m_Radio_ELWPut;
	//}}AFX_DATA
public:
	class CPage4 *m_page;
	void SetOther(class CPage4 *page);
	int GetActiveGroup();


	void SetArray(std::vector<std::shared_ptr<_intersx>>& arInters);

private:
	BOOL m_bCol0Asc{};
	BOOL m_bCol1Asc{};
	int m_nPrevCol{};

	int m_nCol0Asc{};
	int m_nCol1Asc{};
	int m_nPrevList2{};
	bool isClickServerDN{};

public:
	CWnd *m_parent{};
	CString m_root;
	CString m_name;
	bool m_activePage{};
	bool m_bIsEdit{}; //�����ߴ��� ����
	bool m_autoScroll{};

	struct _dragdata
	{ // drag item struct
		int list{};
		int item{};
		CString code;
		CString name;
	};

	_dragdata m_dragData;
	std::shared_ptr<CImageList> m_dragImage{};
	int m_activegno{};

	CArray<_shjcode, _shjcode> m_hjcode; // ��������
	CArray<codelistitem, codelistitem> m_fjcode;   // ��������
	CArray<ojcode, ojcode> m_ojcode;     // �ɼ�����
	CArray<pjcode, pjcode> m_pjcode;     // �����ɼ�
	CArray<upcode, upcode> m_upcode;     // �����ڵ�
	CArray<codelistitem, codelistitem> m_elwbase;  // elwbase

	CArray<ccode, ccode> m_cicode;	 // 2012.10.04 KSJ �ݸ�
	CArray<ccode, ccode> m_cccode;	 // 2012.10.04 KSJ ��ȭ
	CArray<sfcode, sfcode> m_sfcode; // 2012.10.04 KSJ �ֽļ���

//	CArray<_efopitem *, _efopitem *> m_ETFitem;
//	CArray<_efoptema *, _efoptema *> m_ETFtema;
//	CArray<_efopfore *, _efopfore *> m_ETFfore;
//	CArray<_efopcode *, _efopcode *> m_ETFcode;

	std::vector<std::unique_ptr<_efopitem>>	m_ETFitem;
	std::vector<std::unique_ptr<_efoptema>> m_ETFtema;
	std::vector<std::unique_ptr<_efopfore>>	m_ETFfore;
	std::vector<std::unique_ptr<_efopcode>> m_ETFcode;


	CArray<_elwdata, _elwdata> m_ELWdata;
	CArray<codelistitem, codelistitem> m_arrayPBCode;

	CMapStringToPtr m_ELWbase;
	CMapStringToPtr m_ELWcompany;

	BOOL m_bUpdate{};

	CString m_appendGI;
	int m_selectGroup{};

	CString m_strGroupNumber;
	bool m_isDuplicateCode{}; //���� �ߺ� ��� ����
	
	std::array<std::array<CString, 4>, MAXGROUP> m_manageGroup;

//	CString m_manageGroup[MAXGROUP][4]; // 0 : ������ ������
//					    // 1 : M �׷�� ���� D : ���� N : �׷� �߰�
//					    // 2 : ������ ������
//					    // 3 : �׷��

	// ���� ���('b') ����('R') ���������� S

	bool m_isChangeSequence{}; // �׷� ���� �����Ͽ�����

	bool m_bFirstDuplicateNO{}; // 2012.02.13 KSJ ó�� �ߺ����� ��������� �ٽô� �����ʱ� ���ؼ�.
	int m_nResult{};	    // 2012.04.02 KSJ ó�� �ߺ����� ����������� ����� ����.

public:

	BOOL endSendAction();

	void traceManageGroup();
	void sortManageGroup(int drag, int drop);

	bool checkChangeSequence();
	int getManageCount();

	CString getManageGroupdata(int row, int col);
	void setManageGroup(char *datB);

	bool GetIsEdit() { return m_bIsEdit; } //��������
	void SetSize();
	void initList();
	void ETFInitialList();
	void initTree();
	void ETFInitialTree(HTREEITEM hRoot);
	void initCombo(bool isEditOk = false);

	void resettingGroup();

	void OnEndDrag(CPoint point);

	void selectComboBox();

	void SetItemDataUJ(HTREEITEM hItem, int gubn);
	void SetItemDataX(HTREEITEM hItem, int gubn);
	void ClearListitem();
	void ClearList2item();
	void ClearInterest();
	void selcMark(CListCtrl *listctrl, int nItem, bool scroll = true);
	void deselcMark(CListCtrl *listctrl);
	void insertList1toList2(int sitem = -1);
	void deleteList2();
	void searchAction(int column);
	int findMatchIndex(CString text);
	int findMatchIndex(CString text, int column);
	void updateList2sum();
	std::shared_ptr<CImageList> CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl *listctrl);
	void SetAutoScroll(CListCtrl *listctrl, int direct);

	BOOL AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width);
	int AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem = -1);
	bool ExistFile(int gno, bool temp = false);

	void loadConfig();
	void loadingHJcode();
	void loadingFJcode();
	void loadingOJcode();
	void loadingPJcode();
	void loadingUPcode();
	void loadingELWcode();
	void loadingETFcode();

	void loadingCFcode(bool bCurrency); // 2012.10.04 KSJ �ݸ�(6x), ��ȭ(7x) ��ȭ�� true
	void loadingSFcode();		    // 2012.10.04 KSJ �ֽļ���

	//	void	saveConfig();
	void DataGubn(HTREEITEM hItem);
	void DataReloadItem(CString data);
	void DataReloadTema(CString data);
	void DataReloadFore(CString data);

	void savingInterest(int gno);
	void savingGroupFile(int gno, CString gname);
	bool savingGroupOrder(CString gname);

	int xAllCodeToList();
	int xKospiToList();
	int xKosdaqToList();
	int xKospi200ToList();
	int xKosdaq50ToList();
	int xKRX100ToList();
	int xFreeCodeToList();
	int xELWCodeToList();
	int xELWBaseToList();
	int xETFCodeToList();
	int xSinjuCodeToList();
	int xFutureToList();
	int xCallCodeToList();
	int xPutCodeToList();
	int xKospiJisuToList();
	int xKosdaqJisuToList();
	int xPjcodeToList(CString code, char kind);
	int xKospiUpToList(int selItem);
	int xKosdaqUpToList(int selItem);
	int xKospiExToList(int selItem);
	int xKosdaqExToList(int selItem);
	int xThemaCodeToList(int selItem);
	int xGroupCodeToList(int selItem);
	int xKonexToList();    // 2013.06.25 �ڳؽ� �߰�
	int xETNCodeToList();  // 2014.11.06 ETN �߰�
	int xSPACCodeToList(); // 2015.11.03 spac �߰�

	int xCFutureToList(bool bCurrency); // 2012.10.04 KSJ �ݸ�/��ȭ ��ȭ�϶��� true
	int xSFutureToList();		    // 2012.10.04 KSJ �ֽļ���

	void OKaction();
	void OKactionOne(int gnum);
	void CCaction();

	int GetUploadData(int gno, CString &name, char *datB);

	int SetDnloadData(int gno, CString gname, int count, char *datB);
	void enableButton(BOOL enable);
	int GetCodeType(CString code);
	CString GetCodeName(CString code);

	CString GetGroupORder2(CString &data);

	void EnableButton(BOOL bEnable);

	void SetListColumnText(CListCtrl *pList, int nCol, int nAsc);

	// ELW �߰� ���
	void ELWIssueDnload(int selItem);
	void ELWAssetDnload(int selItem, CString item);
	void parsingELWAssetDN(char *datB, int datL);
	void parsingELWIssueDN(char *datB, int datL);
	void parsingRecommandDN(char *datB, int datL);
	int FindPBCode(CString code);
	void DataReloadELWPB(CString str);
	void DataReloadELWBase(CString str);
	void DataReloadELWAll(CString str);
	void ExactGroupData();
	void xRecommandtoList();

	int GetGroupCount();
	CString Variant(int comm, CString data);
	void OnClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	void OnClickELWTree(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL OnInitDialog();

protected:
	void AppendItemToList1(int ipos, CString code, CString name, _listitem *pListItem = nullptr);
	void AppendItemToList2(int ipos, std::shared_ptr<struct _intersx> inter, bool add = true);
	void AppendItemToList2(int ipos, int gubn, CString code, CString name, CString bookmark = "", bool add = true);
	void DeleteItemToList2(int ipos);
	bool existCodeInList2(CString code);
	void InitEdit();

	void selectKospi();    //�ֽ� ������ �����ϱ�
	void selectElw();      // ELW ������ �����ϱ�
	void analyzeELWCode(); //�ڵ尡 ELW���� �ֽ����� �Ǵ�

	CString GetGroupName(int gno);
	void GuideMsg(CString msg);
	bool GuideMsg(CString msg, bool question);
	CString parseX(CString &srcstr, CString substr);

	CString idTOstring(UINT id);

	// new added
	bool m_bAddAll; // �����Է½� �Ź� Ŭ���� �ϴ°� �����ϱ� ����
	bool SearchData(int kind, CString sName, bool bAddAll);
	bool SearchJongmok(CString sName, bool bAddAll);
	bool SearchWord(CString sName, bool bAddAll);
	bool ExistCodeList(CString code);
	bool IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);
	WORD MakeHangul(char cChar1, char cChar2);

	void ExceptionMsg();

	void insertList2(CString sCodes); // 2012.02.13	KSJ	�׷쿡 ������
public:
	void AppendItem(CString sCodes); // 2012.02.13 KSJ  �������߰�
	void AppendNewgroup();		 // 2012.02.13 KSJ  �׷��߰�

	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1)
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	void    receiveOub(int key, CString Data);

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult);
	void listsort(int column);
	void listsort2(int column);
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1)

	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnAddall();
	afx_msg void OnDelall();
	afx_msg void OnBlank();
	afx_msg void OnMoveup();
	afx_msg void OnMovedn();
	afx_msg void OnNewgroup();
	afx_msg void OnUpload();
	afx_msg void OnDnload();
	afx_msg void OnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBegindragList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnAllsave();
	afx_msg void OnSelchangeGname();
	afx_msg void OnSelchangeComboSearch();
	afx_msg void OnEditgroup();
//	afx_msg void OnCnvinterest();
	afx_msg void OnDnloadr();
	afx_msg void OnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShareimport();
	afx_msg void OnShareexport();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMovetop();
	afx_msg void OnMovebottom();
	afx_msg void OnDestroy();
	afx_msg void OnRadioClick();
	afx_msg void OnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetdispinfoList2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg long OnEditXMsg(WPARAM wParam, LPARAM lParam);
	afx_msg long OnAppendGroup(WPARAM wParam, LPARAM lParam);
	afx_msg long OnAppendParentGroup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectGroup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectGroupNCode(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickBtnSort(UINT nID);
	DECLARE_MESSAGE_MAP()



	std::vector<std::shared_ptr<_intersx>> m_inters, m_orginters, _vAddInter;
	std::vector<CITEMS>		       _vList;
	std::vector<CITEMS>		       _vListCopy;
};
