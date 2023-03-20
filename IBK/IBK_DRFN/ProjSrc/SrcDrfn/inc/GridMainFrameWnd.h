#if !defined(AFX_GRIDMAINFRAMEWND_H__06DB84E6_C6EF_4667_8F76_878EE13E13C9__INCLUDED_)
#define AFX_GRIDMAINFRAMEWND_H__06DB84E6_C6EF_4667_8F76_878EE13E13C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridMainFrameWnd.h : header file
//

/*####################################################################################
##																																									##
##															Condition Make Grid Control													##
##																																									## 
##																																									##
##																�ۼ��� : 2003. 7 ~																##
##																�ۼ��� : �쵿�� (TEXEN SYSTEM)										##
##																																									##  
######################################################################################*/

//# [��� ����] ######################################################################
//# 1. ���ǰ˻��� ���� �׸����̴�.
//# 2. �Ϲ�/���������� ���ÿ� �����Ҽ� �ִ� ���
//# 3. Col�� �׷� Ȯ��/���, ����(���Ǽ���, Element), ���, �߰���ǥ, _T("��"), 
//#				����, ���� �����̴�.
//# 4. �Ϲݱ׷�� ���� �׷��� Splitter�� �����ϰ� ��� Event�� �޼����� ó���Ѵ�.
//# 5. �׿� ����� �Ʒ��� �ּ��� �����϶�.

/*# Define Event Message! ############################################################
#define WM_CALLPARENT												(WM_USER + 100)


#define NM_SELECT_GROUP												1		//#=>GroupNo
#define NM_SELECT_CONDITION										2		//# (0) : Group, (1) : Row

#define NM_CONDITION_DRAGDROP									5		//# (0) : Group, (1) : Old Row, (2) : New Row


#define NM_ALIGN_ELEMENT											30	//#=> Last Element Index


#define NM_VALVIEWALL_CHECK										10		//# (0) : Group, (1) : Val
#define NM_DELETEALL													11		//# Group

#define NM_CONDITION_CHECK										20		//# (0) : Group, (1) : Row, (2) : Val
#define NM_TARGET_LIST												21		//# (0) : Group, (1) : Row, (2) : Target Type, (3) : Target
#define NM_VALVIEW_CHECK											22		//# (0) : Group, (1) : Row, (2) : Val
#define NM_MODIFY_BTN													23		//# (0) : Group, (1) : Row, (2) : Display Row
#define NM_DELETE_BTN													24		//# (0) : Group, (1) : Row(After Select), (2) : TotHeight


#define NM_GROUP_ADD													40	//# Group	
#define NM_GROUP_INSERTUP											41	//# Group
#define NM_GROUP_INSERTDOWN										42	//# Group
#define NM_GROUP_DELETE												43	//# Group
//#####################################################################################*/


//# [History]
//# 2005.08.30
//#     [����/�߰�] 
//#				: �׷� �� Element�� �ڵ� Indexing ��� �߰�
//#					- void SetOrderUser(BOOL bTypeOrder);
//#					- void SetUserDragDrop(BOOL bState = FALSE);
//#					- void SetAlignGroup(BOOL bAutoGroupAlign = FALSE);






/////////////////////////////////////////////////////////////////////////////
// CGridMainFrameWnd frame
class CGridSplitterWnd;
class CGridCommonView;
class CGridOrderView;
class AFX_EXT_CLASS CGridMainFrameWnd : public CFrameWnd
{
public:
	CGridMainFrameWnd();
	virtual ~CGridMainFrameWnd();

private : 
	CGridSplitterWnd			*m_pGridSplitterWnd;
	CGridCommonView				*m_pViewCommonGrid;
	CGridOrderView				*m_pViewOrderGrid;

	int				m_nElementIndex;		//# ������ Element Index
	int				m_nTargetIndex;			//# �Էµ� ���ð� Index (����)

	int				m_nSelectGroupNo;		//# ���� �׷�
	int				m_nSelectRow;
public :
	CWnd			*m_pParentWnd;

public:
	void SetInit();		//# �ʱ�ȭ

	void SetSelectGroup(int nGroupNo, BOOL bMsgSend = TRUE);
	void SetAddGroup();									//# �׷� �߰�
	void SetInsertGroup(int nGroupNo, BOOL bInsertPosUp);	//# �׷� ���� (Input : ���ñ׷�)
	void SetDeleteGroup(int nGroupNo);	//# �׷� ���� (Input : ���ñ׷�)

	int GetGroupCnt();									//# �׷� ����
	int GetMaxGroupNo();								//# ������ �׷� ��ȣ
	int GetGroupRowCnt(int nGroupNo);		//# ���ñ׷��� Row ����
	int GetSelectGroup();

	//# ���� �׷��� ���� ������  ����
	BOOL SetAddCondition(int nGroupNo, 
												BOOL		bState,					//# Condition View
												int			nElementIndex,	//# Element Index
												BOOL		bNot,						//# Not State 
												int			nTargetType,		//# Target Type
												int			nTargetIndex,		//# Target Index
												BOOL		bTargetEnable,	//# Target Enable
												CString strItem,				//# �ϼ� ���� 
												BOOL		bFieldState,		//# ������ ���� (TRUE : ������ O, FALSE : ������ X)
												BOOL		bFieldVal);			//# ������

	BOOL SetInsertCondition(int nRow,
												int nGroupNo, 
												BOOL		bState,					//# Condition View
												int			nElementIndex,	//# Element Index
												BOOL		bNot,						//# Not State 
												int			nTargetType,		//# Target Type
												int			nTargetIndex,		//# Target Index
												BOOL		bTargetEnable,	//# Target Enable
												CString strItem,				//# �ϼ� ���� 
												BOOL		bFieldState,		//# ������ ���� (TRUE : ������ O, FALSE : ������ X)
												BOOL		bFieldVal);			//# ������

	//# ���� �׷�/������ ������ ����
	BOOL SetModifyCondition(int nRow,
												int nGroupNo, 
												BOOL		bState,					//# Condition View
												CString	strElement,			//# Element
												BOOL		bNot,						//# Not State 
												int			nTargetType,		//# Target Type
												int			nTargetIndex,		//# Target Index
												BOOL		bTargetEnable,	//# Target Enable
												CString strItem,				//# �ϼ� ���� 
												BOOL		bFieldState,		//# ������ ���� (TRUE : ������ O, FALSE : ������ X)
												BOOL		bFieldVal);			//# ������

	BOOL SetModifyCondition(int nRow, 
													int nGroupNo, 
													BOOL bNot,						//# Not State 
													CString strItem,			//# �ϼ� ����
													BOOL bFieldVal);

	BOOL SetDeleteCondition(int nRow, int nGroupNo);


	CString GetCellData(int nGroupNo, int nCol, int nRow);
	CString GetElementInfo(int nGroupNo, int nRow);
	void SetCellData(int nGroupNo, int nCol, int nRow, CString strData);

	BOOL GetFooterData(int nGroupNo, int &nStartBong, int &nEndBong);		//# �����׷��� Footer�� ������ �ޱ�
	BOOL SetFooterData(int nGroupNo, int nStartBong, int nEndBong);			//# �����׷��� Footer�� ������ ����
	
	int SetAlignElement();
	int GetTargetBase(int nGroupNo);
	void SetTargetBase(int nGroupNo, int nTargetIndex);
	void SetSameTargetBase(int nIndex, int nOutType = -1);

	void SetSelectRow(int nGroupNo, int nRow, BOOL bMsgSend = FALSE);
	void SetUnSelectRow(int nGroupNo, int nRow, BOOL bMsgSend = FALSE);
	

	void SetGroupViewState(BOOL bMinMax);		//# �׷� �ִ�/�ּ�ȭ (TRUE : MIN, FALSE : MAX)
	void SetMoveWindow(int nHeight, BOOL bInvalidate = TRUE);


	//# Grid Setting Function ####################
	void SetOrderUser(BOOL bTypeOrder);
	void SetUserDragDrop(BOOL bState = FALSE);

	void SetAlignGroup(BOOL bAutoGroupAlign = FALSE);

private:
	int GetChange_Group(int nGroupNo);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridMainFrameWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGridMainFrameWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

//	afx_msg void OnNotifyMainEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyMainEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDMAINFRAMEWND_H__06DB84E6_C6EF_4667_8F76_878EE13E13C9__INCLUDED_)
