#pragma once
// AccountCtrl.h : header file

#pragma warning(disable : 4786)

#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#include "AccCombo.h"
#include "AccEdit.h"
#include "Account.h"

#include "COubWnd.h"

#define INI_DELIGATION		("DELAGENT.INI")

typedef std::vector<CString> StrVector;
typedef std::vector<CString>::iterator IStrVector;
typedef std::map<CString, StrVector> StrVectorMap;
typedef std::map<CString, StrVector>::iterator IStrVectorMap;

typedef CArray <std::shared_ptr<CAccount>, std::shared_ptr<CAccount>> CAccountArr;

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};

//=================================================================================================
class CGroup
{
public:
	CString m_strGrSortNum;	// �׷��Ϸù�ȣ
	CString m_strGrName;	// �׷��
	CString m_strAccntCnt;	// ���°���
	CString	m_strGroupID;	// �׷���̵�
	CAccountArr	m_arrAccount;

public:
	CGroup()
	{
		m_strGrSortNum = _T("");
		m_strGrName = _T("");
		m_strAccntCnt = _T("");
		m_strGroupID = _T("");
		m_arrAccount.RemoveAll();
	};

	~CGroup()
	{
		_RemoveAr();
	};

	CGroup(CGroup const &other)
	{
		Copy(other);
	};

	inline	void _RemoveAr()
	{
		std::shared_ptr<CAccount> pAccount = nullptr;

		for (int ii = 0; ii < m_arrAccount.GetSize(); ii++)
		{
			pAccount = m_arrAccount.GetAt(ii);
			if (pAccount)
				pAccount.reset();
		}
		m_arrAccount.RemoveAll();
	};

	inline void Copy(CGroup const &other)
	{
		std::shared_ptr<CAccount> pAccount = nullptr;
		std::shared_ptr<CAccount> pNewAccount = nullptr;

		m_strGrSortNum = other.m_strGrSortNum;
		m_strGrName = other.m_strGrName;
		m_strAccntCnt = other.m_strAccntCnt;
		m_strGroupID = other.m_strGroupID;

		for (int ii = 0; ii < other.m_arrAccount.GetSize(); ii++)
		{
			pAccount = other.m_arrAccount.GetAt(ii);
			if (pAccount)
			{
				pNewAccount = std::make_shared<CAccount>();
				pNewAccount->Copy(*pAccount);
				m_arrAccount.Add(pNewAccount);
			}
		}
	};

	inline void TrimData()
	{
		m_strGrSortNum.TrimRight();
		m_strGrName.TrimRight();
		m_strAccntCnt.TrimRight();
		m_strGroupID.TrimRight();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CAccountCtrl window

class CAccountCtrl : public CWnd
{
// Construction
public:
	CAccountCtrl(CWnd* pParent, _param* pParam);
	virtual ~CAccountCtrl();

// 2010.12.23 add by lkm. (���������丮 ����)
public:
	StrVectorMap m_AccTypeMap;
	void	LoadAccountType();
	void	Split(const char* text, const char del, StrVector *pResult);
	void	Join(StrVector *pSrc, const char del, CString *pResult);
	void	Convert_V2();
	void	SortHistory();
	void	DeleteHistory(LPCSTR acno);

// Attributes
public:

// Operations
public:
	BOOL	Initialize(BOOL bDLL);
	void	AccountInputComplete(CString strAccount);
	void	ShowAccountList(BOOL bGroup, BOOL bSearch = FALSE);
	void	PushTab();
	//2012.02.28 KSJ ȭ��ǥŰ
	void	PushDownKey();
	void	PushUpKey();
	void	PushLeftKey();
	void	PushRightKey();
	//KSJ
	std::shared_ptr<CAccEdit> GetEdit() { return m_pEdit; }


private:
	BOOL	CreateFont();
	BOOL	IsValidAcc(CString strAccount, BOOL bOrderCheck = TRUE, BOOL bAgent = FALSE);
	BOOL	IsFirstValidAcc(CString strAccount, BOOL bOrderCheck = TRUE);
	BOOL	SendTR(CString strName, BYTE type, char *pBytes, int nBYtes, int key);

	COLORREF GetIndexColor(int nColor);

	CString	GetLedger(CString strTR, char chType, CString strSvcn, CString strLastKey = _T(""));
	CString	GetPassword(CString strAccount);
	CString	Parser(CString &strSrc, CString strSub);
	CString	PopAccHistory();
	CString	Variant(int nComm, CString strData = _T(""));

	std::shared_ptr<CAccount> FindAccountInfo(CString strAccount, BOOL bAll = TRUE);
	std::shared_ptr<CAccount> AccFindFromAccHistory(CString strAccount,  BOOL bAll, int& ifind);
	void	LoadButtonImage();
	void	LoadHistoryAcc();
	void	LoadGroupAcc();
	void	ParseAccntName(char* pBuf);
	void	ParseGroupList(char* pBuf);
	void	PushAccHistory(CString strAcc, CString strPassword, CString strAccName);
	void	PushDeleteAccHistory();
	void	QueryAccntName(CString strData);
	void	QueryGroupList();
	void	ResizeCtrl(int cx, int cy);
	void	ResizeEdit(int cx, int cy);
	void	SafeRemoveAccArray(CAccountArr* pArr);
	void	SetButtonState(CPoint point, int nMouseState);
	void	SetParam(_param *pParam);
	void	SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	void	ShowGroupList();
	void	QuerySACMQ329();				//2013.03.12 KSJ �ڱݴ�ü�븮�� ��ȸ
	void	ParseSACMQ329(char* pBuf);		//2013.03.12 KSJ �ڱݴ�ü�븮�� ���
	void	QuerySACMQ748();				//2013.10.29 MK  ���߰����� ��ȸ
	void	ParseSACMQ748(char* pBuf);		//2013.10.29 MK	 ���߰����� ���

	void	LoadHistoryAccName();		//2013.05.28 KSJ �̸������� ����
	void	LoadGroupAccName();		//2013.05.28 KSJ �̸������� ����
	void	LoadAllHistoryAccName();	//2013.05.28 KSJ �̸������� ����

	const char* GetInitialSound(char *han_src);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountCtrl)
public:
	virtual void OnFinalRelease();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL	m_bAllowDelegation;	// ���Ӱ��� ��� ����
	CString m_mapName;		//���� map
	BOOL	m_staff;
	BOOL	m_bWrap;
	CString m_acif2;
	CString m_acif1;
	CString m_sAccnDept;
	CString m_dept;
	BOOL	m_bDestroy;		//2012.02.08 KSJ OnDestroy���� SaveHistoryȣ���Ҷ��� üũ�ϱ� ����
	bool	m_bAddAll;		//2012.02.16 KSJ �ѱ��Է����� üũ
	BOOL m_bEnableCtrl = TRUE;

	void	QueryAccntInfo(CString strAcc);
	BOOL	ReadAcc813();
	CString Find813Name(CString acnt);
	CString Find813Pswd(CString acnt);
	void	LoadAllHistoryAcc();
	void	PushDeleteAllAccHistory();
	BOOL	IsNumber(CString str);
	void	DeleteAcc(int nDeleteIndex);
	void	AddAcc(CString strAcc, CString strPassword, CString strAccName, CString strItems); 	//2013.03.12 KSJ �ڱݴ�ü, �����ŸŴ븮 ���� �����Է�

	BOOL	IsValidAccComplete(CString strAccount, CString dept, BOOL bOrderCheck);
	void	AppendAccHistory(CString strAcc, CString strPassword, CString strAccName);
	void	AddDeleteAccList(CString strAcc);
	void	DeleteDeleteAccList(CString strAcc);
	CFont*	getAxFont(CString fName, int point, int style);

	void	InitAllowDept();
	BOOL	GetAllowAllAcc(CString dep);
	void	SaveHistory();

	CString GetAcntTypeName(CString tp);
	CString GetAcntName(LPCSTR acno);

	//test
	CString GetEditData();
	void GetFontInfo(CString& sfontname, int& isize, int& itype) const;
	// Generated message map functions
protected:
	//{{AFX_MSG(CAccountCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnComboSelChange();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	afx_msg LPARAM OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	//2012.02.15 KSJ
	afx_msg long OnEditXMsg(WPARAM wParam, LPARAM lParam);
	// KSJ
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAccountCtrl)
	afx_msg void OnAccNameChanged();
	afx_msg long _GetBackColor();
	afx_msg void _SetBackColor(long nNewValue);
	afx_msg BSTR _GetData();
	afx_msg void _SetData(LPCTSTR lpszNewValue);
	afx_msg BSTR _GetDept();
	afx_msg long _GetForeColor();
	afx_msg void _SetForeColor(long nNewValue);
	afx_msg BSTR _GetGroup();
	afx_msg void _SetGroup(LPCTSTR lpszNewValue);
	afx_msg BSTR _GetName();
	afx_msg BSTR _GetPassword();
	afx_msg BOOL _GetTabStop();
	afx_msg void _SetTabStop(BOOL bNewValue);
	afx_msg short _GetType();
	afx_msg BOOL _GetVisible();
	afx_msg void _SetVisible(BOOL bNewValue);
	afx_msg long GetOnlySise();
	afx_msg BOOL IsBankLinked();
	afx_msg long GetAgentType();
	afx_msg void SetAgentType(long nNewValue);
	afx_msg void _Empty();
	afx_msg void _SetFocus();
	afx_msg BOOL IsAccAgent(LPCTSTR strAcc, long nAgentType);
	afx_msg BSTR GetAgentAccList();
	afx_msg BSTR GetAgentNameList();
	afx_msg void ReHistroy(LPCTSTR accdata, LPCTSTR accnmdata);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	//2012.02.15 KSJ
	void	searchAction(int column);
	bool	SearchData(CString sName, bool bAddAll);
	bool	Search_MidData(CString sName, bool bAddAll);
	bool	FindWordFromACCList(CString strSearch, CAccountArr* pArr);
	WORD	MakeHangul(char cChar1, char cChar2);
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);

	CString	m_OldString;		//2012.02.28 KSJ �������ڿ� ��   ���϶� �ѹ��� �ϱ�����.
	int	m_nTimerCnt;		//2013.01.16 KSJ TT_INIT Ÿ�̸� ���� Ƚ���� �ִ� 2������ �����Ѵ�.
	// KSJ

private:
	void	PushAcntInfo(CString strAcc, CString strLastAcif);
	BOOL	IsVaidAcc813(CString strAccount);
	void	SetAccountType();

	BOOL	m_bKeyIn;
	CString m_strDept;
	int	m_onlysise;
	BOOL	m_bDLL;
	BOOL	m_bEditMode;
	BOOL	m_bOrderEnable;
	BOOL	m_bFireEvent;
	BOOL	m_bInit;
	BOOL	m_bMultiClick;
	BOOL	m_bTrackLeave;
	BOOL	m_bUseAlias;
	BOOL	m_bGroup;
	BOOL	m_bBankLinked;	//2013.04.23 KSJ ���� ��� 500����(501~504, 551~555, 571) ���࿬����°� ���¸���Ʈ�� ���ԵǾ� ������� TRUE
	int	m_nAgentType;	//2013.03.12 KSJ ������ǰ�ŸŴ븮��(5), ���հ��°� �ڱݴ�ü �븮��(6)
	CString m_strAgenAcctList;	//2013.03.13 KSJ �ʿ��� �޺��ڽ��� �߰��� ���¸���Ʈ
	CString m_strAgenNameList;	//2013.04.18 KSJ �ʿ��� �޺��ڽ��� �߰��� �̸�����Ʈ

	std::unique_ptr <CAccCombo>	m_pCombo;
	std::shared_ptr <CAccEdit>	m_pEdit;

	CAccountArr	m_arrAllAcc;
	CAccountArr	m_arrHistoryAcc;
	CAccountArr	m_arrGroupAcc;
	CAccountArr	m_arrDelectAcc;  // ����ڿ� ���� ������ ���� ����Ʈ
	CAccountArr	m_arrSearchAcc;  // 2012.02.20 KSJ �˻��� ����� ���� ����Ʈ
	CArray <std::shared_ptr<CGroup>, std::shared_ptr<CGroup>> m_arrGroupList;

	COLORREF m_clrBackColor;
	COLORREF m_clrForeColor;
	COLORREF m_clrBorderColor;
	COLORREF m_clrEditBackColor;

	CFont*	m_pFont;
	std::unique_ptr<CMenu>	m_pGroupList;

	CSize	m_szG;
	CSize	m_szArrow;

	CString m_strAccName;
	CString m_strPassword;
	CString m_strRoot;
	CString m_strUser;
	CString m_strName;
	CString m_strRet;
	CString m_strTempDomino;

	CStringArray m_allow_dept;
	CStringArray m_arrAccType;

	CWnd*	m_pParent;

	HBITMAP m_hAccArrow;
	HBITMAP m_hAccArrow_DN;
	HBITMAP m_hAccArrow_EN;

	HBITMAP m_hG;
	HBITMAP m_hG_DN;
	HBITMAP m_hG_EN;
	HBITMAP m_hG_S;
	HBITMAP m_hG_S_DN;
	HBITMAP m_hG_S_EN;

	HBITMAP m_hArrow;
	HBITMAP m_hArrow_DN;
	HBITMAP m_hArrow_EN;

	int	m_nSelectGroup;

	CParam m_Param;

	UINT	m_nAppearance;
	UINT	m_nBorderStyle;
	UINT	m_nStateG;
	UINT	m_nStateArrow;
	UINT	m_nType;
	int	m_nSort;		//2013.05.28 KSJ ���¸���������
	CString m_sHideTradeAgent;	//20200924 �ֹ��븮�ΰ��� ����

	std::unique_ptr<CToolTipCtrl> m_pTipCtrl;

	COubWnd* m_pOubWnd{};
	void	QueryAccntCheck(CString strData);
	BOOL m_bCheckAgetn{};
protected:
	BSTR GetShowHideAcc(BSTR strAdd,  BSTR strDel);

	enum
	{
		dispidEnableCtrl = 23L,
		dispidGetShowHideAccName = 22L,
		dispidGetShowHideAcc = 21L
	};
	BSTR GetShowHideAccName(BSTR strShow, BSTR strHide);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
protected:
	void EnableCtrl(SHORT bShow);
};