// ChartPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"

#include "../Include_Chart/PropertiesData.h"		// for STRTOOLBAROPTION
#include "ChartPropertySheet.h"
#include "Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartPropertySheet

IMPLEMENT_DYNAMIC(CChartPropertySheet, CPropertySheet)

CChartPropertySheet::CChartPropertySheet(LPCTSTR pszCaption, HWND p_hOcxWnd, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_hOcxWnd;
	m_PacketPage.SetOcxHwnd( m_hOcxWnd);
	m_ChartListPage.SetOcxHwnd( m_hOcxWnd);
	m_ToolandRButtonPage.SetOcxHwnd( m_hOcxWnd);
//@solomon[sashia]	m_SharedDataPage.SetOcxHwnd( m_hOcxWnd);
	m_ViewDesignPage.SetOcxHwnd( m_hOcxWnd);
	m_ViewDesignPage2.SetOcxHwnd( m_hOcxWnd);
	m_RealPacketPage.SetOcxHwnd( m_hOcxWnd);

	AddPage(&m_PacketPage);
	AddPage(&m_ChartListPage);
	AddPage(&m_ViewDesignPage);
	AddPage(&m_ViewDesignPage2);
	AddPage(&m_ToolandRButtonPage);
//@solomon[sashia]	20090902 --- ��� ��� --------->>
	//@solomon[sashia]	AddPage(&m_SharedDataPage);	
//@solomon[sashia]	20090902 --- ��� ��� ---------<<

	AddPage(&m_RealPacketPage);

	// (2006/8/24 - Seung-Won, Bae) AddIn Loading Info Page
	AddPage(&m_addInLoadInfoPage);
}

CChartPropertySheet::~CChartPropertySheet()
{
	RemovePage(&m_PacketPage);
	RemovePage(&m_ChartListPage);
	RemovePage(&m_ViewDesignPage);
	RemovePage(&m_ViewDesignPage2);
	RemovePage(&m_ToolandRButtonPage);

//@solomon[sashia]	20090902 --- ��� ��� --------->>
	//@solomon[sashia]		RemovePage(&m_SharedDataPage);
//@solomon[sashia]	20090902 --- ��� ��� ---------<<

	RemovePage(&m_RealPacketPage);

	// (2006/8/24 - Seung-Won, Bae) AddIn Loading Info Page
	RemovePage(&m_addInLoadInfoPage);
}


BEGIN_MESSAGE_MAP(CChartPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CChartPropertySheet)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartPropertySheet message handlers

int CChartPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	return 0;
}

BOOL CChartPropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
//	GetDlgItem( ID_APPLY_NOW )->EnableWindow(TRUE);

	return bResult;
}

//������ ������ ���� ��� �Ӽ��������� �Ӽ��� Set�ؾ��ϴ��� ����
//nIndex: ������ �Ӽ��� ����
//strItemName: �Ӽ���
//strItemValue: �Ӽ���
void CChartPropertySheet::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	m_ChartListPage.SetProperty(		p_pIPropertyMap);
	m_ViewDesignPage.SetProperty(		p_pIPropertyMap);
	m_ViewDesignPage2.SetProperty(		p_pIPropertyMap);
	m_ToolandRButtonPage.SetProperty(	p_pIPropertyMap);
	m_PacketPage.SetProperty(			p_pIPropertyMap);
	
//@solomon[sashia]	20090902 --- ��� ��� --------->>
//	m_SharedDataPage.SetProperty(		p_pIPropertyMap);
//@solomon[sashia]	20090902 --- ��� ��� ---------<<

	m_RealPacketPage.SetProperty(		p_pIPropertyMap);

	// (2006/8/24 - Seung-Won, Bae) AddIn Loading Info Page
	m_addInLoadInfoPage.SetProperty(	p_pIPropertyMap);
}

//InitializeDlg���� ������ Row,Column�� ����.
void CChartPropertySheet::SetBlockProperty(const int nRow, const int nColumn)
{
	m_ChartListPage.SetRowandColumn(nRow, nColumn);
}

void CChartPropertySheet::SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager)
{
	m_ChartListPage.SetIGraphInfoManager( p_pIGraphInfoManager);
}	


