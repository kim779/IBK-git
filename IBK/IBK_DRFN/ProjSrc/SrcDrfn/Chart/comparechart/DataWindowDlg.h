#if !defined(AFX_DATAWINDOWDLG_H__CF44185E_F88E_40ED_860F_4AA06682B660__INCLUDED_)
#define AFX_DATAWINDOWDLG_H__CF44185E_F88E_40ED_860F_4AA06682B660__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataWindowDlg.h : header file
//

// CDataWindowDlg.h : header file
// ��Ƽ��Ʈ�� ����Ÿâ Dialog.



/////////////////////////////////////////////////////////////////////////////
// CDataWindowDlg dialog
#include "./control/RscDlg.h"
#include <AFXTEMPL.H>

class CPacketList;
class CMainBlock;
//class CMChartInterface;
class CDataWindowDlg : public CRscDlg
{
// Construction
public:
	CDataWindowDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataWindowDlg)IDD_DIALOG_DATAWINDOW
	enum { IDD = IDD_DIALOG_DATAWINDOW };
	CListCtrl	m_listctrlData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataWindowDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataWindowDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool CreateDataWindow();	// DataWindow ����.

	void SetDataWindowData(CList<CString, CString&>* pListData);
	//void SetOutsideCommonInterface(COutsideCommonInterface* pChartOcxInterface);

private:
	void SetHeaderCtrl(CString& strHeader);
	void SetListCtrlData(CString& strListCtrlData, const int nRowIndex);

private:
	//CMChartInterface*			m_pChartOcxInterface;
	int							m_nDataIndex;
	int							m_nPrevDataCount;	// ��������Ÿ����(����ó�������� �ʿ�)
	CPoint						m_ptPrevMousePoint;	// ���� ���콺�� ��ġ(����ó�������� �ʿ�)
/*
private:
	int  GetEndIndex(const int nIndex) const;
	CString GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const;
	int	 GetDataCount(void) const;

	bool SetListCtrlHeader(const int nBlockColumnIndex);
	bool SetNextHeader(const CString& strTitle, const CString& strPacketNames, int& nHeaderColumnIndex, const int nHeaderColumnCount);
	bool SetHeaderTitle(int& nColumnIndex, const int nColumnCount, const CString& strHeaderTitle);
	
	void SetData(const int nBlockColumnIndex, const int nIndex);
	void SetHorzScaleData(const CString& strHorzScaleName, const int nDataIndex, const int nInsertRowIndex, int& nInsertColumnIndex);
	void SetViewData(const CString& strTitle, const CString& strPacketNames, const int nDataIndex, const int nInsertRowIndex, int& nInsertColumnIndex);

public:
	bool CreateDataWindow();	// DataWindow ����.
	void InitDataWindow(CPacketList* pPacketList, CMainBlock* pMainBlock);

	void SetOutsideCommonInterface(COutsideCommonInterface* pChartOcxInterface);

	void MoveMouse(CPoint point);
	void ChangeTRData(void);
	void ChangeRealData(void);
private:
	CPacketList*				m_pPacketList;
	CMainBlock*					m_pMainBlock;
	COutsideCommonInterface*	m_pChartOcxInterface;
	int							m_nDataIndex;
	int							m_nPrevDataCount;	// ��������Ÿ����(����ó�������� �ʿ�)
	CPoint						m_ptPrevMousePoint;	// ���� ���콺�� ��ġ(����ó�������� �ʿ�)
*/
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAWINDOWDLG_H__CF44185E_F88E_40ED_860F_4AA06682B660__INCLUDED_)
