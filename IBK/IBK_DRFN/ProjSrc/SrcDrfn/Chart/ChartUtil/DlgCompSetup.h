#if !defined(AFX_DLGCOMPSETUP_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_)
#define AFX_DLGCOMPSETUP_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCompSetup.h : header file
//
#include "XColourPicker.h"
#include "./XEditPrompt.h"
#include "./BitmapItemComboBox.h"
#include "../Chart_Common/DlgCompData.h"
#include "../../inc/ITreeListMng.h"
#include "./VirtualListCtrl.h"
#include "../../inc/ExGridCtrl.h"
#include "../Chart_Common/CallPutHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCompSetup dialog

class CDlgCompSetup : public CDialog
{
// Construction
public:
	int	m_nAddType; // 0: ����Ʈ, 1:�׸���(�ɼ�����)
	CDlgCompSetup(CWnd* pParent = NULL);   // standard constructor

	STDlgCompData* m_dwInData;
	List_CDlgCompGroupData	m_GroupList;
	CDlgCompGroupData* m_pCurGroup;
	List_CDlgCompSubItem* GetCurSubItemList();

	CDlgCompSubItem* m_pCurSubItem;

	long m_dwOutData;
	void SetInOutData(long dwInData, long dwOutData);

	int m_nListItemSel, m_nListCodeSel;
	CString		m_szRootPath;

// Dialog Data
	//{{AFX_DATA(CDlgCompSetup)
	enum { IDD = IDD_DLGCOMPSETUP };
	CVirtualListCtrl	m_ListCodes;
	CListCtrl	m_ListItem;
	CListBox	m_ListRP;
	CTreeCtrl	m_treeSearch;
	CXColourPicker	m_btnTextColor;
	CXEditPrompt	m_edtCode;
	CXEditPrompt	m_edtCodeName;
	CBitmapItemComboBox	m_Thickness_Combo;
	CBitmapItemComboBox	m_DrawStyle_Combo;
	CComboBox m_CmbSel_Combo;
	int		m_nTime;
	BOOL	m_bCurShow;
	int		m_nPeriodType;
	int		m_nRComp;
	//}}AFX_DATA
	COLORREF m_rgbText;
	CExGridCtrl*	m_pGridCtrl;
	CCallPutIHelper	m_cpHelper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCompSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL	m_bModifyFlag;
	void Initial_ThicknessComboBox( int p_nSubGraphIndex, int idStatic, CBitmapItemComboBox &Thickness_Combo );
	void AddLineType( CComboBox *pCombo, int Type );

	void Initial_DrawStyleComboBox( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );
	void LoadBitmapToDrawStyleCombo( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );
	void RealLoadBitmap(const int nCount, const int nIDBitmap, CBitmapItemComboBox& Drawsytle_Combo);

	// 1. ����Ʈ�� �ڵ尡 �����ϴ��� Ȯ���Ѵ�.
	// 2. �ִ� ��밹�� Ȯ��
	// Return�� 0�̻�: ����, -1:�̹�����, -2:���Ƚ���ʰ�. 
	int fnCheckCode(LPCSTR _szCode, LPCSTR _szCodeName);

	// Generated message map functions
	//{{AFX_MSG(CDlgCompSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtngrpnew();
	afx_msg void OnBtngrpdel();
	afx_msg void OnBtngrpchange();
	afx_msg void OnArrow1();
	afx_msg void OnArrow2();
	afx_msg void OnArrow3();
	afx_msg void OnArrow4();
	afx_msg void OnBtnleft();
	afx_msg void OnBtnright();
	afx_msg void OnBtnleft3();
	afx_msg void OnOk();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeListgrp();
	afx_msg void OnItemchangedListitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClkListcodes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListcodes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDrawstyle();
	afx_msg void OnSelchangeThickness();
	afx_msg void OnSelchangeCmbsel();
	//}}AFX_MSG
	afx_msg LONG OnColorSelChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// �ƹ� �׷쵵 ������ �⺻�׷��� �����Ѵ�.
	void fnCheckNoGroup();

	void InitFromLoadData();
	CDlgCompGroupData* FindGroupInfo(int nIndex, POSITION &pos);

	//-->
	ITreeListManager2* m_pTreeListMng;
	class CTLMCallback : public ITLMCallback
	{
		STDMETHOD_(void, ReceiveData)(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData) ;
	} m_xTLMCallback;
	friend class CTLMCallback;
	long	m_dwTreeKey;

	void fnInitList();
	void fnTreeLoad();
	//<--

	// �׷��� ������ ó���� �Ѵ�. �׷��� ����Ʈ�� �����ִ� ó���� �Ѵ�.
	void fnSelectGroup(int nSel);

	// �׷��� �������� �������� �� �׷������� ������ �ݿ��Ѵ�.
	void fnSelectItem(CDlgCompGroupData* pGroupData, int nItemSel);
	void fnSelectItem(CDlgCompSubItem* pItem);

	//
	void fnUpdateToControl(CDlgCompSubItem* pCurSubItem);
	void fnUpdateToMemory(CDlgCompSubItem* pCurSubItem);

	void DoAddToItem(int nItem);

	CString m_szGridSelCode, m_szGridSelCodeName;
	void DoGridAddToItem(LPCSTR szCode, LPCSTR szCodeName);
	void DoDeleteItem(int nItem);
	void DoCurItemChange();

	// �־��� �׷��̸��� select�Ѵ�. OnInit~���� ȣ��.
	void fnInitSelctGroup( LPCSTR szGroupName);

	int m_nCtrlType;	//0:�Ϲ�, 1:Kospi200�ɼ�, 2:�ֽĿɼ�
	void ChangeListOrGridCtrl(CString szSection);
	void SetMasterData(LIST_CCodeTypeA* pCodeType);

	// �׸��� ����.
	void	InitGrid();
	void	InitGridRowCol(int nRowCount =0);
	void	ChangeGridRowCol();
	void	InsertGridData();
	CString	GetInsertGridDataText(int row, int col);
	BOOL	GetCodeInfo(int nType, int& row, int& col, CString& strCode);

	void	DoFindEdtCode();
	void	DoFindEdtName(LPCSTR szInput=NULL);
	void	DoSelectionClear();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMPSETUP_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_)
