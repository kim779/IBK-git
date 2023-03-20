#if !defined(AFX_CHARTLISTPAGE_H__AFA037A3_DD97_11D4_BCF1_00C026CA36CC__INCLUDED_)
#define AFX_CHARTLISTPAGE_H__AFA037A3_DD97_11D4_BCF1_00C026CA36CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartListPage.h : header file
//

#include "Include_AddIn.h"							// for Ixxxxxx.h
#include "resource.h"
#include "MyTreeCtrl.h"
#include "MyListCtrl.h"
#include "DlgToolTip.h"

////////////////////////////////////////////////////////////////////////////
// CChartListPage dialog

interface IPropertyMap;
class CChartListPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CChartListPage)

// Construction
public:
	void SetProperty( IPropertyMap *p_pIPropertyMap);
	//Add text to HTREEITEM.
	void SetToolTipInfo(HTREEITEM& hItem, const CString& strTip);
	void SetChartListData(const CString& strChartListData);
	void SetRowandColumn(const int& nRow, const int& nColumn);
	//Packet �Ӽ����������� �����ؾ��ϴ� �Լ�. 
	//Packet���� ���õ� �Ӽ����� ������ �ϳ��� String���� ���´�.
	void SetInfoOfAllPacketItems(const CString& strInfoOfAllPacketItems);  

	CChartListPage();
	~CChartListPage();

	//@solomon[sashia]	20090902 --- ��� ���� ��� --------->>
	CChartLink	*m_pSelTreeItem;
	//@solomon[sashia]	20090902 --- ��� ���� ��� ---------<<

// Dialog Data
	//{{AFX_DATA(CChartListPage)
	enum { IDD = IDD_PROPPAGE_CHARTLIST };
	CMyTreeCtrl	m_TreeChart;
	CMyListCtrl	m_listChart;
	//}}AFX_DATA

	//@solomon[sashia]	20090902 --- ��� ���� ��� --------->>
	CButton		m_btnFunc1;
	CButton		m_btnFunc2;
	CStatic		m_stcSelGroup;
	//@solomon[sashia]	20090902 --- ��� ���� ��� ---------<<

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CChartListPage)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CChartListPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//@solomon[sashia]	20090902 --- ��� ���� ��� --------->>
	afx_msg void OnBtnFunc1();
	afx_msg void OnBtnFunc2();
	afx_msg void OnSelchangedTreeChart(NMHDR* pNMHDR, LRESULT* pResult);
//@solomon[sashia]	20090902 --- ��� ���� ��� ---------<<

private:
	enum
	{
		ONELINE_COLOR = 1,
		TWOLINE_COLOR,
		THREELINE_COLOR,
		FOURLINE_COLOR,
		FIVELINE_COLOR,
		SIXLINE_COLOR
	};

private:
	CDlgToolTip		m_toolTip;
	//ocx���� ���� Ʈ���� ������ Į������
	int m_nColumn;   
	//ocx���� ���� Ʈ���� ������ Row����
	int m_nRow;  
	//ocx���� ���� ��, Item�� ����
	CString m_strChartListData;  
	//Packet�������� ��� ����Ÿ(üũ����, �׸�, �ʵ��, ����, Ÿ��,�ǽð�����)�� �ϳ��� String���� ��������
	CString m_strAllItemInfoOfPacketPage;  
	//��ǥ����Ʈ��Ʈ�ѿ� �� �̹��� ����.
	CImageList m_ImageList;  

	//����Ʈ��Ʈ�� �ʱ�ȭ
	void InitList(CListCtrl *pList);  
	void SetListItem(CListCtrl* pList);
	//��������
	void CreateToolTip();
	//Graph�Ӽ� Set 

	//����.
	CMap< HTREEITEM, HTREEITEM&, CString, CString& > tooltipMap;
	CToolTipCtrl m_tooltip;

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
protected:
	IGraphInfoManager *	m_pIGraphInfoManager;
public:
	void SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager);

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTLISTPAGE_H__AFA037A3_DD97_11D4_BCF1_00C026CA36CC__INCLUDED_)
