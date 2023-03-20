#if !defined(AFX_CATEGORYPOPUP_H__3365E2BF_3265_4CDA_B4A9_1932F398A97C__INCLUDED_)
#define AFX_CATEGORYPOPUP_H__3365E2BF_3265_4CDA_B4A9_1932F398A97C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CategoryPopup.h : header file
//
#include "resource.h"
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CCategoryPopup window
#define CHECKER_COUNT 10
//#include "XPButton.h"
#include "../ctrl/ExListCtrl.h"
#include "SStatic.h"
#include ".././../h/jmcode.h"
#include "usedefine.h"

struct	_mhjcode {
	CString	code;		// �ڵ�	(12)
				// �ڵ�(7) : A######
				// �ڵ�(9) : �����μ���, ����
	CString	hnam;		// �ѱ۸�(18)
	CString	enam;		// ������

	CString	acdl;		// ������з�
	CString	acdm;		// �����ߺз�(asis ucdm)
	CString	acds;		// �����Һз�(asis ucds)

	char	ecng{};		// ECN ����
				// �ŷ�����:1
	char	jsiz{};		// �����ں��� ���߼�
				// �峻 (��:38  ��:39  ��:40)
				// ��� (1:KOSDAQ 100)
				// ��� (2:KOSDAQ MID 300)
				// ��� (3:KOSDAQ SMALL)
				// �������(jsiz += 41)
				// �����μ������� FULL�ڵ��
	CString	symb;		// SYMBOL
	char	size{};		// �ں��� �Ը�
				// KOSPI  (��:2  ��:3  ��:4)
				// KOSDAQ (��Ÿ����:3  �ڽ���IT����:4)
			

	//char	ucdm{};		// �����ߺз�
	//			// KOSDAQ:5~14  �̺з�:99
	//char	ucds{};		// �����Һз�
	//			// KOSDAQ:15~37  ����:7  �̺з�:99


	char	jjug{};		// ������ ����
				// KOSPI  (������:27)
				// KOSDAQ (��ó����:2)
	char	kpgb{};		// KOSPI, KOSDAQ ����
				// KOSPI  (0:������ 1:K200 2:K100 3:K50)
				// KOSDAQ (0:������ 1:KQ50)
	char	kosd{};		// ���񱸺�
				// 0:�峻  10:���  30:��3����
	char	ssgb{};		// �Ҽӱ���
				// 0:default
				// 4:���߾��ݵ�
				// 5:�����μ���
				// 6:����
				// 7:�����μ�������
				// 8:ETF
				// 9:�����ϵ��ݵ�
	char	ucmd{};		// �켱�ֱ��� (0:������  5:�켱��)
	char	itgb{};		// KOSPI-IT/KQ-IT50����(1:ä��)
	char	wsgb{};		// KOSPI ���豸������:42 (0:������ 1:���)
	char	jqty{};		// �⺻�ֹ�����
	char	star{};		// KOSDAQ ��Ÿ����
	char	unio{};		// ��������
	char    jchk{};		// ����˻��� ����
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

constexpr char* acd_KOSPI[] = {
	"",
	"KGG01P",	// �ڽ���		1
	"KGZ01P",	// �ڽ��� ������	2	
	"KGZ02P",	// �ڽ��� ������	3
	"KGZ03P",	// �ڽ��� ������	4
	"KGS01P",	// ���ķ�ǰ		5
	"KGS02P",	// ����, �Ǻ�		6
	"KGS03P",	// ����, ����		7
	"KGS04P",	// ȭ��			8
	"KGS05P",	// �Ǿ�ǰ		9	
	"KGS06P",	// ��ݼӱ���		10
	"KGS07P",	// ö���ױݼ�		11
	"KGS08P",	// ���			12
	"KGS09P",	// ����, ����		13
	"KGS19P",	// �Ƿ�����		14
	"KGS10P",	// ������		15
	"KGS11P",	// �����		16
	"KGS20P",	// ���Ⱑ����		17
	"KGS12P",	// �Ǽ���		18	
	"KGS13P",	// ���â��		19
	"KGS21P",	// ��ž�		20
	"KGS14P",	// ������		21
	"KGS15P",	// ����			22
	"",
	"KGS16P",	// ����			24
	"KGS17P",	// ����			25
	"KGS22P",	// ���񽺾�		26
	"KGS18P"	// ������		27
};

constexpr char* acd_KOSDAQ[] = {
	"",
	"",
	"",
	"",
	"",
	"",
	"QGS02P",	// ����			6
	"QGS03P",	// �Ǽ�			7
	"QGS04P",	// ����			8
	"",
	"QGS31P",	// ���			10 
	"QGS05P",	// ����			11
	"QGS06P",	// ��Ź��		12
	"QGS07P",	// IT S / W & SVC	13
	"QGS08P",	// IT H / W		14      
	"QGS09P",	// ���ķᡤ���		15 
	"QGS10P",	// �������Ƿ�		16
	"QGS11P",	// ���� ����		17
	"QGS12P",	// ���ǡ���ü����	18
	"QGS13P",	// ȭ��			19
	"QGS33P",	// ����			20
	"QGS14P",	// ��ݼ�		21
	"QGS15P",	// �� ��		22
	"QGS16P",	// ��衤���		23
	"QGS17P",	// �Ϲ���������		24
	"QGS18P",	// �Ƿᡤ���б��	25
	"QGS19P",	// �����񡤺�ǰ	26
	"QGS20P",	// ��Ÿ ����		27
	"QGS21P",	// ��ż���		28
	"QGS22P",	// ��ۼ���		29
	"QGS23P",	// ���ͳ�		30
	"QGS24P",	// ������������		31
	"QGS25P",	// ����Ʈ����		32
	"QGS26P",	// ��ǻ�ͼ���		33
	"QGS27P",	// ������		34
	"QGS28P",	// �������		35
	"QGS29P",	// �ݵ�ü		36
	"QGS30P",	// IT��ǰ		37
	"QGS87P",	//         
	"",
	"",		//			39
	"",		//			40
	"QGS34P",	// ��������ȭ		41
	"QGG03P",	// �ڽ��� �췮�����	42
	"QGG04P",	// �ڽ��� ��ó�����	43
	"QGG05P",	// �ڽ��� �߰߱����	44
	"QGG06P"	// �ڽ��� �����������45 
};

class CControlWnd;
#include "SComboBox.h"
#include "xSkinButton.h"

class CCategoryPopup : public CWnd
{
// Construction
public:
	CCategoryPopup(CWnd* parent);

// Attributes
public:
	CWnd* m_parent;
#ifdef DF_USESTL_CATEGORY
	std::unique_ptr<CTreeCtrl>m_pTree;
	std::unique_ptr<CExListCtrl>m_pList;
#else
	CTreeCtrl	*m_pTree;
	CExListCtrl *m_pList;
#endif
	int			m_nKind;

protected:
	CBitmap*	m_bmp1;
	CBitmap*	m_bmp2;
	CBrush		m_brBk,m_brBk1,m_brSt;
	bool		m_fDone;
#ifdef DF_USESTL_CATEGORY
	CArray		<std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>	m_shjCodes;
	CArray		<std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>m_elwbase;
	std::unique_ptr<CStatic> m_pStatic;
	std::unique_ptr<CSStatic> m_pQCStatic;
	std::unique_ptr<CSComboBox> m_pComboOrder;
	std::unique_ptr<CButton>m_pCheckBtn[CHECKER_COUNT];
	std::unique_ptr<CxSkinButton>m_pSearchBtn;
	std::unique_ptr<CxSkinButton>m_pPlayBtn;
	std::unique_ptr<CxSkinButton>m_pCloseBtn;
#else
	CArray		<_mhjcode*, _mhjcode*>		m_shjCodes;
	CArray		<_mhjcode*, _mhjcode*>		m_elwbase;
	CStatic*	m_pStatic;
	CSStatic*	m_pQCStatic;
	CSComboBox*	m_pComboOrder;
	CButton*	m_pCheckBtn[CHECKER_COUNT];
	CxSkinButton*  m_pSearchBtn;
	CxSkinButton*  m_pPlayBtn;
	CxSkinButton*  m_pCloseBtn;
#endif
	CStringArray m_history;
	CWnd*		m_pParent;
	
	CRect		m_rcCheck[CHECKER_COUNT];
	CString		m_szCheckName[CHECKER_COUNT];
	bool		m_bCateChecked[CHECKER_COUNT];
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCategoryPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	
	bool IsFilteredHj(_mhjcode* hj);
#ifdef DF_USESTL_CATEGORY	
	void ListUpjongSort(int nidx, std::shared_ptr<_mhjcode>  hjc, int ntype = 0);
	std::shared_ptr<_mhjcode> FindCode(CString sCode);
#else
	void ListUpjongSort(int nidx,_mhjcode* hjc,int ntype=0);	
	_mhjcode* FindCode(CString sCode);
#endif
	void InitControls();
	void InitListCtrl();
	CString LoadStringTb(UINT nIdx);
	void	loadRank(HTREEITEM hItem);
	void	LoadGroup(CString path, HTREEITEM hItem);
	void	loadUCode(CString path, HTREEITEM	hItem);
	void	SearchCode();
	CString	Parser(CString &srcstr, CString substr);
	void	setParent(CWnd *pWnd);
	void	LoadFolder(CString path, HTREEITEM hItem, CString sSection, int nKey);
	void	LoadELWBase(CString path);
	void	baseCX_Category();
	virtual ~CCategoryPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCategoryPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnChecked(UINT nID);
	afx_msg void OnPlayClk();
	afx_msg void OnCloseClk();
	afx_msg void OnSearchClk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) ;	
	afx_msg void OnTreeDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
#ifdef DF_USESTL_CATEGORY
	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>> m_hjCodes;
	CArray <std::shared_ptr<CStringArray>, std::shared_ptr<CStringArray>> m_codeArr;
#else
	CArray		<_mhjcode*, _mhjcode*>		m_hjCodes;
	CArray		<CStringArray*, CStringArray*>	m_codeArr;
#endif
	CMapStringToString	m_codeMap;

	CString m_sCodeList;
	CString loadCodeList(int nGroup);
	void	loadHjCode();
	CFont   m_font;
public:
	void Initialize();
	HTREEITEM m_InterITEM;
	void InsertGroupList(CString sRet);
	void InsertCodeInGroup(CString sRet);
};

#define PT_CX 365
#define PT_CY 365

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATEGORYPOPUP_H__3365E2BF_3265_4CDA_B4A9_1932F398A97C__INCLUDED_)
