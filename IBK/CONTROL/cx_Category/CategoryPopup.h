#pragma once

// CategoryPopup.h : header file
//
#include <afxtempl.h>
#include "ExListCtrl.h"
#include "SStatic.h"
#include ".././../h/jmcode.h"

/////////////////////////////////////////////////////////////////////////////
// CCategoryPopup window
#define CHECKER_COUNT 10

struct	_mhjcode {
	CString	code;		// �ڵ�	(12)
				// �ڵ�(7) : A######
				// �ڵ�(9) : �����μ���, ����
	CString	hnam;		// �ѱ۸�(18)
	CString	enam;		// ������

	CString	acdl;		// ������з�
	CString	acdm;		// �����ߺз�(asis ucdm)
	CString	acds;		// �����Һз�(asis ucds)

	char	ecng=0;		// ECN ����
				// �ŷ�����:1
	char	jsiz=0;		// �����ں��� ���߼�
				// �峻 (��:38  ��:39  ��:40)
				// ��� (1:KOSDAQ 100)
				// ��� (2:KOSDAQ MID 300)
				// ��� (3:KOSDAQ SMALL)
				// �������(jsiz += 41)
				// �����μ������� FULL�ڵ��
	CString	symb;		// SYMBOL
	char	size=0;		// �ں��� �Ը�
				// KOSPI  (��:2  ��:3  ��:4)
				// KOSDAQ (��Ÿ����:3  �ڽ���IT����:4)
	char	jjug=0;		// ������ ����
				// KOSPI  (������:27)
				// KOSDAQ (��ó����:2)
	char	kpgb=0;		// KOSPI, KOSDAQ ����
				// KOSPI  (0:������ 1:K200 2:K100 3:K50)
				// KOSDAQ (0:������ 1:KQ50)
	char	kosd=0;		// ���񱸺�
				// 0:�峻  10:���  30:��3����
	char	ssgb=0;		// �Ҽӱ���
				// 0:default
				// 4:���߾��ݵ�
				// 5:�����μ���
				// 6:����
				// 7:�����μ�������
				// 8:ETF
				// 9:�����ϵ��ݵ�
	char	ucmd=0;		// �켱�ֱ��� (0:������  5:�켱��)
	char	itgb=0;		// KOSPI-IT/KQ-IT50����(1:ä��)
	char	wsgb=0;		// KOSPI ���豸������:42 (0:������ 1:���)
	char	jqty=0;		// �⺻�ֹ�����
	char	star=0;		// KOSDAQ ��Ÿ����
	char	unio=0;		// ��������
	char    jchk=0;		// ����˻��� ����
				// 0x01 �Ҽ��ǰ�������
				// 0x02 �űԻ�������
				// 0x04	�Ǹ�������
				// 0x08	�켱������
				// 0x10 ��������
				// 0x20	��������
				// 0x40 �ŷ���������
				// 0x80	������������
	char	fill[2]{};	//reservered;
};	
	
#define MAX_LIST_SIZE 1000

class CControlWnd;
#include "SComboBox.h"
#include "xSkinButton.h"

class CCategoryPopup : public CWnd
{
// Construction
public:
	CCategoryPopup(CControlWnd* pWnd, int Gap);
	virtual ~CCategoryPopup();

// Attributes
public:
	CControlWnd *m_pMainWnd;
	int	m_nKind;

protected:
	CBitmap	m_bmp1,m_bmp2;
	CBrush	m_brBk,m_brBk1,m_brSt;
	bool	m_fDone;
	CString	m_sRoot, m_sUser;

	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>	m_shjCodes;
	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>	m_elwbase;
	CStringArray m_history;

	CWnd*	m_pParent;

	std::unique_ptr<CTreeCtrl>	m_pTree;
	std::unique_ptr<CStatic>	m_pStatic;
	std::unique_ptr<CSComboBox>	m_pComboOrder;
	std::unique_ptr<CButton>	m_pCheckBtn[CHECKER_COUNT];
	std::unique_ptr<CExListCtrl>	m_pList;

	std::unique_ptr<CSStatic>	m_pQCStatic;
	std::unique_ptr<CStatic>	m_pNtStatic;
	std::unique_ptr<CComboBox>	m_pComboGap;
	std::unique_ptr<CxSkinButton>	m_pSearchBtn;
	std::unique_ptr<CxSkinButton>	m_pConfigBtn;
	std::unique_ptr<CxSkinButton>	m_pCloseBtn;
	std::unique_ptr<CButton>	m_chkPlay;

	CRect		m_rcCheck[CHECKER_COUNT];
	CString		m_szCheckName[CHECKER_COUNT];
	CFont*		m_pFont;
	
	int		m_Gap;
	bool		m_bInitState;

	HTREEITEM	m_iitem;	//�������� TREEITEM
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCategoryPopup)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool	IsFilteredHj(_mhjcode* hj);
	void	ListUpjongSort(int nidx, std::shared_ptr<_mhjcode> hjc,int ntype=0);
	void	InitControls();
	void	InitListCtrl();
	CString LoadStringTb(UINT nIdx);
	void	loadRank(HTREEITEM hItem);
	void	LoadGroup(char* datb, int datl);
	void	loadUCode(CString path, HTREEITEM hItem);
	void	SearchCode();
	CString	Parser(CString &srcstr, CString substr);
	void	setParent(CWnd *pWnd);
	void	setPathInfo(CString root, CString user);
	void	LoadFolder(CString path, HTREEITEM hItem, CString sSection, int nKey);
	void	LoadELWBase(CString path);
	std::shared_ptr<_mhjcode> FindCode(CString sCode);
	void	baseCX_Category();
	void	initBitmap();
	void	LoadInterestCode(const CStringArray& codes);

protected:
	void	fillList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCategoryPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnConfigClk();
	afx_msg void OnCloseClk();
	afx_msg void OnSearchClk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) ;	
	afx_msg void OnTreeDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg void OnChecked(UINT nID);
	afx_msg long OnEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
