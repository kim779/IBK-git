#if !defined(AFX_VIRTUALLISTCTRL_H__F4A57191_1B84_4451_A47B_404CE5477F6E__INCLUDED_)
#define AFX_VIRTUALLISTCTRL_H__F4A57191_1B84_4451_A47B_404CE5477F6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualListCtrl.h : header file
//

#include <afxtempl.h>

//#include "../../../DrMLib/DrMiddleLib.h"
//#include "../../../include/CommonDef.h"			//KJI 20030305 Add
#include "../SortStringArray.h"

//=========================================================================
// �ۼ��� : ������
// �ۼ��� : 2005. 06. 13. ȭ
// ��  �� : �ӵ� ������ ���� Virtual List���·� ���������
//			���� �ڵ� �˻� �ӵ� ������ ���� ������ ���� �ϰ�
//          �ֽ�, ����/�ɼ�, ����, �ؿ����� �ǵ��ʿ��� �������� ����Ѵ�.
//==========================================================================

/////////////////////////////////////////////////////////////////////////////
// CVirtualListCtrl window
typedef struct __MASTERCODE
{
	CString strCode;
	CString strKor;
	CString strEng;
} MASTERCODE;

class CVirtualListCtrl : public CListCtrl
{
// Construction
public:
	CVirtualListCtrl();

	enum {STOCK, UPJONG, FUTOPT, FOREIGN, KWANSIM};

	int				m_nListDataType;
	int				m_nFindCol;
	CString			m_strCode;	//���õ� Code.
	CString			m_strCodeNName;//�ڵ�� ����

	CSortStringArray	m_arrKor;
	CSortStringArray	m_arrEng;
	CSortStringArray	m_arrCode;
	CSortStringArray	m_arrAll;


	CArray<MASTERCODE*, MASTERCODE*> 	m_StockArray;
	CArray<MASTERCODE*, MASTERCODE*>	m_KosdaqArray;
	CArray<MASTERCODE*, MASTERCODE*>	m_AllArray;
	

private:
//	CDrMLib_CDBMgr		*m_pDBMgr;	

	int			m_nOption;
	

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ChangeCode(CString strFind);
	void Init_ListCtrl();
	
	void LoadCodeList(int nOption);
	void Load_GwanSim_CodeList(CStringArray* pArrCodeNName);


	void SetMasterArray();
	int	 FindItemByString(int nCol, CString strFind);
//	void SetDBMgr(void *pDBMgr)			{ m_pDBMgr = (CDrMLib_CDBMgr*)pDBMgr; }
	void SetTopIndex(int nIndex);
	void SetListDataType(int nCodeType)	{ m_nListDataType = nCodeType; }
	virtual ~CVirtualListCtrl();

	//	����Ʈ�� ���� ���̸� �����Ѵ�.
	CImageList	m_ImageList;
	void SetRowHeight(short nHeight);

	// Generated message map functions
protected:
	//{{AFX_MSG(CVirtualListCtrl)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALLISTCTRL_H__F4A57191_1B84_4451_A47B_404CE5477F6E__INCLUDED_)
