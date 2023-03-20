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
//@��������
//#include "JongStatusInTab.h"
//@��������
#include "./control/TreeCtrlEx.h"
#include "./control/EditUsr.h"
#include "../../inc/ITreeListMng.h"
#include "afxwin.h"
#include "./control/GfxOutBarCtrl.h"
#include "./control/TrTreeCtrl.h"
#include "../../inc/ExGridCtrl.h"
#include "./define/ChartMsgDef.h"
#include "../Chart_Common/CallPutHelper.h"
#include "../../inc/DRCommon.h"

#include "CondTarget.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//  (  )
#define NAME_a4017     "a4017"

// �⺻�Է�
typedef struct _a4017In
{
    char    accno               [  11];    // [string,   11] ���¹�ȣ
    char    expcode             [  12];    // [string,   12] �����ڵ�
} a4017In, *LPa4017In;
#define NAME_a4017In     "a4017In"

// ���2, occurs
typedef struct _a4017Out2
{
    char    expcode             [  12];    // [string,   12] �����ڵ�
    char    codename            [  20];    // [string,   20] �����
    char    pamt                [   9];    // [long  ,    9] ��մܰ�
} a4017Out2, *LPa4017Out2;
#define NAME_a4017Out2     "a4017Out2"

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  (  )
#define NAME_a4018     "a4018"

// �⺻�Է�
typedef struct _a4018In
{
    char    accno               [  11];    // [string,   11] ���¹�ȣ
    char    expcode             [  12];    // [string,   12] �����ڵ�
    char    gubun               [   1];    // [string,    1] �Ϻб���
    char    term                [   3];    // [long  ,    3] �Ϻа���
    char    basedt              [   8];    // [string,    8] ��������
    char    basetm              [   6];    // [string,    6] ���ؽð�
    char    cnt                 [   3];    // [long  ,    3] ��û�Ǽ�
} a4018In, *LPa4018In;
#define NAME_a4018In     "a4018In"

// ���1, occurs
typedef struct _a4018Out2
{
    char    procdt              [   8];    // [string,    8] ü������
    char    hhmm                [   4];    // [string,    4] ü��ð�
    char    scnt                [   5];    // [long  ,    5] �ŵ�ü��Ǽ�
    char    sconcprc            [   9];    // [long  ,    9] �ŵ�ü��ܰ�
    char    bcnt                [   5];    // [long  ,    5] �ż�ü��Ǽ�
    char    bconcprc            [   9];    // [long  ,    9] �ż�ü��ܰ�
} a4018Out2, *LPa4018Out2;
#define NAME_a4018Out2     "a4018Out2"
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////

// �� �� ǲ �ɼ� �� ����Ʈ 
//class COptionInfo
//{
//public: 
//	COptionInfo();
//
//// Attributes
//	CString m_strCode;
//
//	int m_nCallPut;				// 2 : call / 3 : put
//	CString m_strEvtPrice;		// ��簡
//
//// Functions
//	void GetCode()
//	{
//		return m_strCode;
//	}
//
//	CString GetYM()
//	{
//		return m_strEvtPrice.Mid(0, 4);
//	}
//
//	BOOL IsCall()
//	{
//		return (m_nCallPut == 2);
//	}
//
//	BOOL IsPut()
//	{
//		return (m_nCallPut == 3);
//	}
//
//	void SetInfo(CString strCode, int nCallPut, CString strEvtPrice)
//	{
//		m_strCode = strCode;
//		m_nCallPut = nCallPut;
//		m_strEvtPrice = strEvtPrice;
//	};
//
//	void SetInfo(CString strOptionInfo)
//	{
//		m_strCode = strOptionInfo.Mid(0, 8);
//		CString strTemp;
//		strTemp = strOptionInfo.Mid(0, 1);
//		if(strTemp == "2")
//			m_nCallPut = 2;
//		else if(strTemp == "3")
//			m_nCallPut = 3;
//		else
//			m_nCallPut = 0;
//
//		strTemp = strOptionInfo.Right(19);
//		m_strEvtPrice = strTemp.Mid(2, 10);
//	};
//
//};

//#define UM_CHANGE_REGKW		   ( WM_USER+525 )		// �������� �׷캯�� ó����
//@Solomon����-->
typedef CList<CItemLongData*, CItemLongData*> List_CItemLongData;
//@Solomon����<--

//tour2k
typedef struct
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;

class CTabCodeSearch : public CTabDlg
{
// Construction
public:
	CTabCodeSearch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCodeSearch)
	enum { IDD = IDD_TABCODESEARCH };
	CButton	m_chkAvgPrice;
	CButton	m_chkConcPrice;
	CButton	m_chkRepeat;
	CILButton	m_btnResetOcx;
	CILButton	m_btPrev;
	CILButton	m_btNext;
	CXListCtrlJongData	m_listCode;
	CILButton		m_btPlay;	//004 ��������	
	CExComboBox		m_cmbTime;
	//}}AFX_DATA
    CySplitterWnd   m_ySplitterY1;
	void SetRealPriceData(void *pData);
	
//@�����߰�
	int m_nCtrlType;			// List or GridCtrl
	CExGridCtrl*	m_pGridCtrl;
	CCallPutIHelper	m_cpHelper;

//	CList<COptionInfo*, COptionInfo*>	m_listOptionInfo1;
//	CList<COptionInfo*, COptionInfo*>	m_listOptionInfo2;
//	CList<COptionInfo*, COptionInfo*>	m_listOptionInfo3;
//	CList<COptionInfo*, COptionInfo*>	m_listOptionInfo4;

	void	InitGrid();
	void	InitGridRowCol(int nRowCount =0);
	void	ChangeGridRowCol();
	void	InsertGridData();
	void	SetMasterData(LIST_CCodeTypeA* pCodeType);
	CString	GetInsertGridDataText(int row, int col);
	void	InitMasterData();
	int		GetGridRow(int nExercise);
//	int		GetGridExercise(int row);
	void	UpdateHogaData(int nRow, int nCol, double dPrice, COLORREF crFgClr, int nPoint=2);
	long	GetRealExercise(int nPrice);
	BOOL	GetCodeInfo(int nType, int& row, int& col, CString& strCode);

	void ChangeListOrGridCtrl(CString szSection);
//@�����߰�

//@��������
//	CJongStatusInTab m_JongStatusInTab;
//@��������
	//CExKoscomCtrl m_btnRegFavorite;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearch)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	virtual void ChangeSkinColor(COLORREF clrBkgrnd);

public:
//	CArray<CTimerData,CTimerData> m_listTimerData;		// Filter Info
//	int			m_nAlarmFrom;
//	int			m_nAlarmTo;
//	BOOL		m_bUseAlarmSound;
//	BOOL		m_bUseAlarmBkClr;
//	CString		m_strSoundFile;
	void	ReceiveDataFromOutSide();
	void	SetChartTypeControl();
	
	IMasterDataManagerLast*	m_pMasterDataManager;

	CCondTarget			m_CondTarget;
	void	RequestMstCode(LPCTSTR lpBuffer,long lLength);

private:
//004 {{ ��������
	
	BOOL		m_bPlay;		
//004 }}
	int			m_nSelAccount;

	CString		m_strSelectedItemName;
	CString		m_strSelectedParentItemName;
	long		m_lQueryStatus;
	BOOL		m_bSkipSelChangedTreeEvent;

	BOOL		m_bInsertingInListCode;
	HTREEITEM  m_hRootItemUsr;		// ����� ��������
	HTREEITEM  m_hRootItemProposeSpecial;	// ��õ����-Ư¡
	HTREEITEM  m_hRootItemProposeBuy;	// ��õ����-�ż�
	HTREEITEM  m_hRootItemProposeSell;	// ��õ����-�ŵ�
	HTREEITEM  m_hRootItemTheme;	// �����׸�
	HTREEITEM  m_hRootItemSise;		// �ü�����
	HTREEITEM  m_hRootItemInvestor;	// ���ڵ���
	HTREEITEM  m_hRootItemIndicator;	// ��ǥ��ȣ
	HTREEITEM  m_hRootItemFavorite;		// ��������
	HTREEITEM  m_hRootItemAccount;		// ��������
	HTREEITEM  m_hRootItemFromOutside;	// �ܺηκ����� �����ڵ�
	HTREEITEM  m_hRootItemCurrent;		// ���� ��ȸ�� Item
	HTREEITEM  m_hRootItemCurrentTemp;	// ���� ��ȸ�� Temp Item
	int			m_nSortIndexOfList;
	BOOL		m_bSortTypeOfList;
	CString m_strShareTime;
	long m_nHeightOfSearch;
	BOOL	m_bDeletingTreeData;
	CString m_strPacketFromOutSide;
	BOOL	m_bUseTimer;
	BOOL	m_bIgnoreEditVolume;
	CRect GetRectOfCtrl(INT nID,BOOL bInit=FALSE);
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
//	long	GetVolumeLimitation();
//	void	SetVolumeInEdit();
//	void	SetTimeFilter();
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
		codesearch_sel_gwan=0,			//����
		codesearch_sel_master_stock,	//�ֽ�
		codesearch_sel_master_elw,		//elw
		codesearch_sel_master_jisu,		//����
		codesearch_sel_master_future,	//����
		codesearch_sel_master_fojipyo,	//������ǥ
		codesearch_sel_master_foreign,	//�ؿ�
		codesearch_sel_master_commodity,//��ǰ����
		codesearch_sel_oneclick,		//��������˻�
		codesearch_sel_imsi,			//���������˻�
		codesearch_sel_account,			
		
		codesearch_stock_sel_master_stock=0,	//�ֽ�
		codesearch_stock_sel_master_elw,	//elw
		codesearch_stock_sel_oneclick,		//��������˻�
		codesearch_stock_sel_imsi,			//���������˻�
		codesearch_stock_sel_account,			

		codesearch_elw_sel_master_elw = 0,	//elw
		codesearch_elw_sel_oneclick,		//��������˻�
		codesearch_elw_sel_imsi,			//���������˻�

		codesearch_upjong_sel_master_jisu=0,		//����

		codesearch_future_sel_master_future=0,	//����

		codesearch_foreign_sel_master=0,	//�ؿ�

		codesearch_commodity_sel_master=0,	//��ǰ����
	};
        ST_GwanSimGroup m_GwanSimGroup;//tour2k
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

		int _DoFilterStockCode(LIST_CCodeTypeA *pListTypeA);
	} m_xTLMCallback;
	friend class CTLMCallback;	
	long m_dwTreeKey; 
//	long		m_lTitleCode;
// Implementation

public:
	BOOL m_bAdvise;
	CString m_strCodes;
	BOOL MakeDrdsConnectAdvise(CString szKey);
	BOOL MakeDrdsUnConnectUnAdvise(CString szKey);

	//2006.08.26 by LYH UI ����{{
	CGfxOutBarCtrl		m_ctrlOutBar;
	//CList <CTreeCtrl*, CTreeCtrl*> m_ListTree;
	void	RemoveAllTree();

	int		m_nHtsMode;

	CTrTreeCtrl*	m_pGwanTree;				// ����ó���� �ϴ� CTreeCtrl
	CTrTreeCtrl*	m_pUserAccountList;

	CTrTreeCtrl*	CreateTreeCtrl(CString strName, UINT nID);
	long			ChangeTreeFolder(CString strName, CTrTreeCtrl *pTree, long lTreeKey, BOOL bChangeMode=TRUE);
	void			fnFolderChange(int nChartType, int nSel);

	//2006.08.26 by LYH UI ����}}
//@��������
//	BOOL m_bGridExtend;
//@��������
	void ApplySelectedCodes();
	//2007.01.12 by LYH Ʈ���� ���ǰ˻� ����
	void SelectFindData();
	// �̹���
	CImageList		m_ImageListIcon;
	//20061213 ������ ����
	//LeftBar Ʈ������� �̹��� ����
//@��������
	CImageList		m_ImageOutbarIcon;
	//20061213 ���� end
	
	BOOL IsPlayState();
	void RunAutoTimer();

	CTrTreeCtrl* m_pSelectedTree;
	void ClickDefault();
	
	void ShowAccountOperator(BOOL bShow);

	class CMyITrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
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

	//�̸����� Outbar���� ���õǾ� ��������.
	int SetSelFolder(LPCSTR szFolderName);
//
public:
	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult);
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
//	afx_msg void OnBtnRetry();
	afx_msg void OnBtnAddapply();
	afx_msg void OnChkAvgprice();
	afx_msg void OnChkConcprice();
	afx_msg void OnItemchangedListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangingListcode(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	LRESULT OnIntRequest( UINT wParam, LONG lParam );
	LRESULT OnResetAll( UINT wParam, LONG lParam );
	HRESULT OnAddAccountCodeItem(WPARAM wParam, LPARAM lParam);
	HRESULT OnCheckAvgPrice(WPARAM wParam, LPARAM lParam);
	HRESULT OnMakeAccountConcData(WPARAM wParam, LPARAM lParam);
	HRESULT OnResetConcData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtplay();	
	afx_msg void OnStnClickedSplittery1();
	afx_msg void OnLvnItemchangedListcode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChkExtend();
	afx_msg long OnOutBarNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRmsgTabCodeSearch(WPARAM wp, LPARAM lp);

//@Solomon����-->
	afx_msg long OnWinixNotifyLoad(WPARAM wParam, LPARAM lParam);
	afx_msg long OnWinixNotifySave(WPARAM wParam, LPARAM lParam);
	afx_msg long OnWinixNotifyGroup(WPARAM wParam, LPARAM lParam);
	afx_msg long OnWinixNotifyGroupList(WPARAM wParam, LPARAM lParam);
	afx_msg long OnWinixNotifySaveCode(WPARAM wParam, LPARAM lParam);

	LRESULT OnRmsgWMSize(WPARAM wp, LPARAM lp);

	List_CItemLongData m_ItemLongDataList;
	void DoDeleteTreeItem(CTreeCtrl* pTreeCtrl, CString& szRemain);
	void DoClearItemLongDataList();
	IWinixGwanManager*	m_pGwanMng;	
//@Solomon����<--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCODESEARCH_H__7AC8ECCA_35CB_48BC_9B2D_B97956B6470C__INCLUDED_)
