#pragma once
// OptionListCtrl.h : header file
//

#include "OHeaderCtrl.h"

#define WM_SELECT_OPTION		WM_USER+12

/////////////////////////////////////////////////////////////////////////////
// COptionListCtrl window

struct	code15	{			/* ��ǰ�ɼ��ڵ� ����		*/
	char    codx[8];   		/* �����ڵ�			*/
	char    hnam[50];  		/* �ѱ۸�			*/
	char	enam[50];		/* ������			*/

	char	jcod[12];		/* �����ڻ������ڵ�		*/
	char    jnam[40];  		/* �ŷ���� �����		*/
	char    mont[4];   		/* ������			*/
	char    atmx;      		/* ATM���� ����(1: ATM)		*/
	char    hsga[12];   		/* ��簡��			*/
	char    tjgb[2];   		/* �ŷ���� ���񱸺�		*/
	char	mchk;			/* �Ÿ��������			*/
							/* 0x01:���尡���		*/
							/* 0x02:���Ǻ����������	*/
							/* 0x04:���������������	*/
	char	fill[20];		/* filler			*/
};

class COptionListCtrl : public CListCtrl
{
// Construction
public:
	COptionListCtrl();

// Attributes
public:

	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int type,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1 );

// Operations
public:

	void MakeImageList( void );

	void DrawItem(LPDRAWITEMSTRUCT lp );	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );	
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	CBitmap		m_bitmapO;
	CBitmap		m_bitmapX;
	CBitmap		m_bitmapSO;

	COHeaderCtrl m_header;
	CImageList m_imageList;

	DECLARE_MESSAGE_MAP()
};

