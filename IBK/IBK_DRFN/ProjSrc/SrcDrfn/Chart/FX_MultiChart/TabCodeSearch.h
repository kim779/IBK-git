#if !defined(AFX_TABCODESEARCH_H__7AC8ECCA_35CB_48BC_9B2D_B97956B6470C__INCLUDED_)
#define AFX_TABCODESEARCH_H__7AC8ECCA_35CB_48BC_9B2D_B97956B6470C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCodeSearch.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearch dialog
#include "TabDlg.h"
#include "XListCtrlJongData.h"
#include "xySplitterWnd.h"
#include "LeftBarDlg.h"
#include "./control/TreeCtrlEx.h"
#include "./control/EditUsr.h"
#include "../../inc/ITreeListMng.h"
#include "afxwin.h"
#include "./control/GfxOutBarCtrl.h"
#include "./control/TrTreeCtrl.h"
#include "../../inc/ExGridCtrl.h"
#include "./define/ChartMsgDef.h"
#include "../Chart_Common/CallPutHelper.h"


class CTabCodeSearch : public CTabDlg
{
// Construction
public:
	CTabCodeSearch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCodeSearch)
	enum { IDD = IDD_TABCODESEARCH };
	CButton				m_chkRepeat;
	CILButton			m_btnResetOcx;
	CILButton			m_btPrev;
	CILButton			m_btNext;
	CXListCtrlJongData	m_listCode;
	CILButton			m_btPlay;
	CExComboBox			m_cmbTime;
	//}}AFX_DATA
    CySplitterWnd   m_ySplitterY1;
	void SetRealPriceData(void *pData);
	
//@�����߰�
	int					m_nCtrlType;			// List or GridCtrl
	CExGridCtrl*		m_pGridCtrl;
	CCallPutIHelper		m_cpHelper;

	void	InitGrid();
	void	InitGridRowCol(int nRowCount =0);
	void	ChangeGridRowCol();
	void	InsertGridData();
	void	SetMasterData(LIST_CCodeTypeA* pCodeType);
	CString	GetInsertGridDataText(int row, int col);
	void	InitMasterData();
	int		GetGridRow(int nExercise);
	void	UpdateHogaData(int nRow, int nCol, double dPrice, COLORREF crFgClr, int nPoint=2);
	long	GetRealExercise(int nPrice);
	BOOL	GetCodeInfo(int nType, int& row, int& col, CString& strCode);
	void	ChangeListOrGridCtrl(CString szSection);
	void	SetFXMasterData();
	void	SetCurListIndex(CString strCode);
	CString	m_srtCodeOrder;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearch)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	virtual void ChangeSkinColor(COLORREF clrBkgrnd);

public:
	void	ReceiveDataFromOutSide();
	void	SetChartTypeControl();
	
	IMasterDataManager*	m_pMasterDataManager;

private:
	BOOL		m_bPlay;		
	CString		m_strSelectedItemName;
	CString		m_strSelectedParentItemName;
	long		m_lQueryStatus;
	BOOL		m_bSkipSelChangedTreeEvent;

	BOOL		m_bInsertingInListCode;
	HTREEITEM	m_hRootItemUsr;					// ����� ��������
	HTREEITEM	m_hRootItemProposeSpecial;		// ��õ����-Ư¡
	HTREEITEM	m_hRootItemProposeBuy;			// ��õ����-�ż�
	HTREEITEM	m_hRootItemProposeSell;			// ��õ����-�ŵ�
	HTREEITEM	m_hRootItemTheme;				// �����׸�
	HTREEITEM	m_hRootItemSise;				// �ü�����
	HTREEITEM	m_hRootItemInvestor;			// ���ڵ���
	HTREEITEM	m_hRootItemIndicator;			// ��ǥ��ȣ
	HTREEITEM	m_hRootItemFavorite;			// ��������
	HTREEITEM	m_hRootItemAccount;				// ��������
	HTREEITEM	m_hRootItemFromOutside;			// �ܺηκ����� �����ڵ�
	HTREEITEM	m_hRootItemCurrent;				// ���� ��ȸ�� Item
	HTREEITEM	m_hRootItemCurrentTemp;			// ���� ��ȸ�� Temp Item

	int			m_nSortIndexOfList;
	BOOL		m_bSortTypeOfList;
	CString		m_strShareTime;
	long		m_nHeightOfSearch;
	BOOL		m_bDeletingTreeData;
	CString		m_strPacketFromOutSide;
	BOOL		m_bUseTimer;
	BOOL		m_bIgnoreEditVolume;

	CRect	GetRectOfCtrl(INT nID,BOOL bInit=FALSE);
	void	RecalcLayout();
	LONG	OnWmSplitterMoved( UINT wParam, LONG lParam );
	void	InitTreeSearch();
	void	UpdateTreeSearch();
	void	ClearAllTreeSearch();
	void	DeleteChildItem(HTREEITEM hItem);
	void	GetCodeData20001(CString *pStr);
	void	GetCodeData20011(CString *pStr);
	void	GetCodeData00410(CString *pStr);

	void	ReceiveData(LPCSTR szTR, LPVOID aTRData, DWORD dwTRDateLen);
	void	SetConfig(BOOL bLoad = TRUE);
	void	SetTimerInfo(BOOL bLoad = TRUE);
	void	RequestCurrentPrice(LPCTSTR lpBuffer,long lLength);
	void	UpdateItemTheme();
	void	UpdateItemSise(char chType);
	void	RequestJongmokCodesInList(HTREEITEM hItem);
	void	DeleteFavoriteChild();
	void	IntRequest();
	void	AdviseMain(int nAdivise);
	void	SetAccountInfo(LPCTSTR lpAccountInfo);
	void	RequestSavedQuery();

	CString		m_strFavoriteGrp;
	BOOL		m_bLBtnDown;
	BOOL		m_bRBtnDown;
	BOOL		m_bDragNDrop;
	BOOL		m_bQueryList;

	//�ֱ� Ʈ�� �������� - ����ȸ ����
	CItemLongData* m_pQueryData;
	enum {
		codesearch_sel_gwan=0,					//����
		codesearch_sel_master_stock,			//�ֽ�
		codesearch_sel_master_elw,				//elw
		codesearch_sel_master_jisu,				//����
		codesearch_sel_master_future,			//����
		codesearch_sel_master_foreign,			//�ؿ�
		codesearch_sel_oneclick,				//��������˻�
		codesearch_sel_imsi,					//���������˻�
		codesearch_stock_sel_master_stock=0,	//�ֽ�
		codesearch_stock_sel_master_elw,		//elw
		codesearch_stock_sel_oneclick,			//��������˻�
		codesearch_stock_sel_imsi,				//���������˻�
		codesearch_upjong_sel_master_jisu=0,	//����
		codesearch_future_sel_master_future=0	//����
	};

public:	
	void Init_Button();

public:
	HTREEITEM m_hSelTreeItem;
	ITreeListManager2*	m_pTreeListMng;
	void fnMakeTree(CTreeCtrl* pTree, LPCSTR szFile);

	class CTLMCallback : public ITLMCallback
	{
		public:
		STDMETHOD_(void, ReceiveData)(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData) ;
		STDMETHOD_(HRESULT, ReceiveRealData)(WORD wFID, LPCSTR szMainKey, void* pRealData);
	} m_xTLMCallback;
	friend class CTLMCallback;	
	long m_dwTreeKey; 

// Implementation
public:
	BOOL m_bAdvise;
	CString m_strCodes;
	BOOL MakeDrdsConnectAdvise(CString szKey);
	BOOL MakeDrdsUnConnectUnAdvise(CString szKey);

	void	RemoveAllTree();

	long m_dwTreeKey_gwan; 
	long m_dwTreeKey_master_stock; 
	long m_dwTreeKey_master_elw; 
	long m_dwTreeKey_master_jisu; 
	long m_dwTreeKey_master_future;
	long m_dwTreeKey_master_foreign;
	long m_dwTreeKey_oneclick; 
	long m_dwTreeKey_FindSearch; 

	CTrTreeCtrl*	m_pGwanTree;				// ����ó���� �ϴ� CTreeCtrl
	CTrTreeCtrl*	m_pMasterTree_stock;		// ������ó���� �ϴ� CTreeCtrl
	CTrTreeCtrl*	m_pMasterTree_elw;			// ������ó���� �ϴ� CTreeCtrl
	CTrTreeCtrl*	m_pMasterTree_Jisu;			// ������ó���� �ϴ� CTreeCtrl
	CTrTreeCtrl*	m_pMasterTree_future;		// ������ó���� �ϴ� CTreeCtrl
	CTrTreeCtrl*	m_pMasterTree_foreign;		// ������ó���� �ϴ� CTreeCtrl
	CTrTreeCtrl*	m_pOneClickTree;			// ��������ó���� �ϴ� CTreeCtrl
	CTrTreeCtrl*	m_pFindSearchTree;			// ���ǰ˻����� �Ѿ�� ���� ó���� �ϴ� CTreeCtrl

	void ApplySelectedCodes();
	void SelectFindData();

	// �̹���
	CImageList		m_ImageListIcon;

	class CMyITrCommSite : public ITrCommSite
	{
	public:
		//<<20100308_JS.Kim �ַθ�
		//STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, char* pData, long dwTRDateLen)
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		//>>
		{
			METHOD_PROLOGUE(CTabCodeSearch, MyITrCommSite)
				return 1L;
		}

		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData)
		{
			METHOD_PROLOGUE(CTabCodeSearch, MyITrCommSite)
				return pThis->ReceiveRealData(wFID, szMainKey, pRealData);
		}

		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CTabCodeSearch, MyITrCommSite)
				return S_OK;	
		}

		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
		{
			MYMETHOD_PROLOGUE(CTabCodeSearch, MyITrCommSite)
			return S_OK;
		}
	} m_xMyITrCommSite;
	friend class CMyITrCommSite;
	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
	{
		return m_xTLMCallback.ReceiveRealData(wFID, szMainKey, pRealData);
	};
//
public:
	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult);
//@��������
//	afx_msg void OnBtapply();
//@��������
	afx_msg void OnBtportfolio();
	afx_msg void OnBtregfavor();
	afx_msg void OnBtsearch();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginRdragListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtall();
	afx_msg void OnRclickListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnext();
	afx_msg void OnBtprev();
	afx_msg void OnBtresetocx();
	afx_msg void OnBtnRetry();
	afx_msg void OnBtnAddapply();
	//}}AFX_MSG
	LRESULT OnIntRequest( UINT wParam, LONG lParam );
	LRESULT OnResetAll( UINT wParam, LONG lParam );
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtplay();	
	afx_msg void OnStnClickedSplittery1();
	afx_msg void OnLvnItemchangedListcode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChkExtend();
	afx_msg long OnOutBarNotify(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCODESEARCH_H__7AC8ECCA_35CB_48BC_9B2D_B97956B6470C__INCLUDED_)
