#if !defined(AFX_TABCODESEARCHFO_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_)
#define AFX_TABCODESEARCHFO_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCodeSearchFO.h : header file
//
#include "resource.h"
#include "TabDlg.h"
#include "../../inc/ExOCXDLL.h"
#include "../../inc/ExGridCtrl.h"
// ��ǰ�����ɼ� 2�� - ojtaso (20071016)
#include "./define/ChartMsgDef.h"

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchFO dialog
// ��ǰ�����ɼ� 2�� - ojtaso (20071016)
//typedef struct _codeinfo{
//	int	nInfo[5];
//
//	struct _codeinfo()
//	{
//		for (int i=0; i < 5; i++)
//			nInfo[i] = 0;
//	}
//
//}codeinfo;
//
//// �� �� ǲ �ɼ� �� ����Ʈ 
//class COptionItem
//{
//public: 
//	COptionItem(){};
//	virtual ~COptionItem(){};
//
//	//DATA
//	CString m_strCode;
//	int m_nCallPut;	// 2:call, 3:put
//	int m_nYear;	// �����
//	int m_nMonth;	// ����� 
//
//	//FUNCTION
//	void SetItem(CString strCode, int nCallPut, int nYear, int nMonth)
//	{
//		m_strCode = strCode;
//		m_nCallPut = nCallPut;
//		m_nYear = nYear;
//		m_nMonth = nMonth;
//	};
//};

class CTabCodeSearchFO : public CTabDlg
{
// Construction
public:
	CTabCodeSearchFO(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCodeSearchFO)
	enum { IDD = IDD_TABCODESEARCH_FO };
	CExComboBox	m_ctrlComboMonth;
	CListBox		m_listFuture;
	//}}AFX_DATA
	CExGridCtrl			*m_pGridCtrl;
	CList<codeinfo, codeinfo&>	m_listMaster;	// �� �ɼ� ���� ���� list
	codeinfo			m_nMonth;				// �� �ɼ� ���� ����
	char				m_cYear[5];				// �� �ɼ� ���� ����
	long				m_lMaxLimit;			// max ��簡
	int					m_nIndex;
	//char				m_cYear;
	IMasterDataManager* m_pDataManager;
	CString m_strDataDirPath;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearchFO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	CRect	GetRectOfCtrl(INT nID,BOOL bInit = FALSE);
	void	RecalcLayout();
	void	InitGrid();
	void	InitGridRowCol();
	void	ChangeGridRowCol();
	void	InsertGridData();
	void	InsertMasterData(int index);
	CString	GetInsertGridDataText(int row, int col);
	BOOL	InitMasterData();
	int		GetGridRow(int nExercise);
	int		GetGridExercise(int row);
	void	UpdateHogaData(int nRow, int nCol, double dPrice, COLORREF crFgClr, int nPoint=2);
	long	GetRealExercise(int nPrice);
	BOOL	GetCodeInfo(int nType, int& row, int& col, CString& strCode);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearchFO)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnEditchangeComboMonth();
	afx_msg void OnSelchangeComboMonth();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeListFuture();
	afx_msg void OnDblclkListFuture();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCODESEARCHFO_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_)
