// DlgSoIkChart.cpp : implementation file
//

#include "stdafx.h"

#include "DlgSoIkChart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSoIkChart dialog


CDlgSoIkChart::CDlgSoIkChart(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSoIkChart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSoIkChart)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSoIkChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSoIkChart)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSoIkChart, CDialog)
	//{{AFX_MSG_MAP(CDlgSoIkChart)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSoIkChart message handlers
