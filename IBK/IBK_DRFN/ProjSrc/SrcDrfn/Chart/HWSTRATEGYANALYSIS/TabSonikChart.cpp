// TabSonikChart.cpp : implementation file
//

#include "stdafx.h"
#include "TabSonikChart.h"
#include <math.h>				// for floor() / ceil()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSonikChart dialog

CTabSonikChart::CTabSonikChart(CWnd* pParent /*=NULL*/)
	: CDlgInterface(CTabSonikChart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabSonikChart)
	m_nRangeUnitWidth = 1;
	//}}AFX_DATA_INIT

	m_brBackground.CreateSolidBrush( CLR_BACKGROUND);

	m_pIndata = NULL;

}

CTabSonikChart::~CTabSonikChart()						// standard destructor
{
	m_brBackground.DeleteObject();
	if(m_pIndata) delete[] m_pIndata;
}


void CTabSonikChart::DoDataExchange(CDataExchange* pDX)
{
	CDlgInterface::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabSonikChart)
	DDX_Control(pDX, IDC_TCHART1, m_ctlBarTChart);			//���� TeeChart
	DDX_Control(pDX, IDC_STC_TOTAL_COUNT, m_stcTotalCount);
	DDX_Control(pDX, IDC_STC_LOSS_COUNT, m_stcLossCount);
	DDX_Control(pDX, IDC_STC_EARNING_RATIO, m_stcEarningRatio);
	DDX_Control(pDX, IDC_STC_EARNING_COUNT, m_stcEarningCount);
	DDX_Text(pDX, IDC_EDT_WIDTH, m_nRangeUnitWidth);
	DDV_MinMaxInt(pDX, m_nRangeUnitWidth, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabSonikChart, CDlgInterface)
	//{{AFX_MSG_MAP(CTabSonikChart)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDT_WIDTH, OnKillfocusEdtWidth)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSonikChart message handlers

HBRUSH CTabSonikChart::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDlgInterface::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Return a different brush if the default is not desired
	pDC->SetBkColor( CLR_BACKGROUND);
	return m_brBackground;
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/23
// Modifier		: 
// Comments		: Do not close with ESC or Enter Key.
//-----------------------------------------------------------------------------
void CTabSonikChart::OnOK() 
{
	int nRangeUnitWidth = m_nRangeUnitWidth;
	UpdateData(TRUE);
	if( m_nRangeUnitWidth < 1 || 100 < m_nRangeUnitWidth)
	{
		m_nRangeUnitWidth = nRangeUnitWidth;
		UpdateData( FALSE);
	}

	int nAllCount = m_pIndata[m_nInCount-1] - m_pIndata[0] + 1;
	int *pAllData = new int[nAllCount];

	for( int i=0; i<nAllCount; i++) 	pAllData[i]=0;

	int nMin = 0;
	CountData(pAllData, nAllCount, m_pIndata, m_nInCount, nMin);
	DisplayData2Chart(pAllData, nAllCount, nMin);

	delete[] pAllData;  //KHD
	

//	CDlgInterface::OnOK();
}

void CTabSonikChart::OnCancel() 
{
//	CDlgInterface::OnCancel();
}

void CTabSonikChart::OnKillfocusEdtWidth() 
{
	UpdateData( FALSE);
}

BOOL CTabSonikChart::OnInitDialog() 
{
	CDlgInterface::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*-------------------------------------------------------------------------------
//���ͺ�����Ʈ�� ������ ������ ���� ���̹Ƿ� ���� ���ͷ� ���� �� �������� 
//��Ȳ�� ���� �� �� �ִ�.
//
// ó������ 1. �Ѿ�� �����͸� sort�Ѵ�.
//				sort�� ���ؼ� min�� max���� ���ϸ� ������ ��������.
//			2. sort�� �����͸� ������ ������ ī��Ʈ�� ����Ѵ�.
//			3. ����� TeeChart�� ǥ���Ѵ�.
------------------------------------------------------------------------------- */
void CTabSonikChart::ShowData1Chart(long dwValue)
{
	CArray<double,double> * dValue = (CArray<double,double>*)dwValue;
	CArray<double,double> OutData;

	int i = 0;
	int nRangeUnitWidth = m_nRangeUnitWidth;
    m_nInCount = dValue->GetSize();

	if(m_nInCount > 0)
	{
		if( m_pIndata) delete[] m_pIndata;
		m_pIndata = new int[m_nInCount]; 	
		
		for( i=0; i<m_nInCount; i++)
			m_pIndata[i] = (int)dValue->GetAt(i);

		m_nInCount = SortData(m_pIndata, m_nInCount);

		int nAllCount = m_pIndata[m_nInCount-1] - m_pIndata[0] + 1;
		int *pAllData = new int[nAllCount];

		for(i=0; i<nAllCount; i++) 	pAllData[i]=0;

		int nMin;
		CountData(pAllData, nAllCount, m_pIndata, m_nInCount, nMin);
		DisplayData2Chart(pAllData, nAllCount, nMin);

		delete[] pAllData;
	}
}


int MyChartCompare( const void *arg1, const void *arg2 )
{
	int arData1 = *(int*)arg1;
	int arData2 = *(int*)arg2;

	if(arData1 == arData2) return 0;
	if(arData1 > arData2) return 1;
	return -1;
}


/* ----------------------------------------------------------------------------------
// ó������ 1. �Ѿ�� �����͸� sort�Ѵ�.
//				sort�� ���ؼ� min�� max���� ���ϸ� ������ ��������.
---------------------------------------------------------------------------------- */
#include <afxtempl.h>
#include ".\tabsonikchart.h"
int CTabSonikChart::SortData(int* pInData, int nCount)
{
	// Sort
	qsort((void *)pInData, nCount, sizeof(int*), MyChartCompare );
	return nCount;
}

/* ----------------------------------------------------------------------------------
// ó������ 2. sort�� �����͸� ������ ������ ī��Ʈ�� ����Ѵ�.
//ceil : �־��� �μ����� ���� ���� �ּ� ���� 
//floor : �־��� �μ����� ũ�� ���� �ִ� ����
//��)3.14   ceil�� 4, floor�� 3
* �Ҽ��� ù��° �ڸ����� 
int* pAllData: ��絥����, int nAllCount: ��Ʈ���Ѽ�(��Ʈ�� ����), int* pInData:�����Ͱ� 
int nInCount : �����Ͱ���(���ͷ� ��)
-------------------------------------------------------------------------------------*/
int	CTabSonikChart::RoundToUnit( int p_nIndex)
{
	if( m_nRangeUnitWidth == 1 || p_nIndex == 0) 
		return p_nIndex;
	int nRet = 0;
	double d_nIndex = (double) p_nIndex;;

	if( p_nIndex < 0)
	{
		int d_nF = (int)floor( d_nIndex / m_nRangeUnitWidth);
		nRet = d_nF * m_nRangeUnitWidth;
	}
	
	else 
	{
		int d_nF = (int)ceil( d_nIndex / m_nRangeUnitWidth); 
	    nRet = d_nF * m_nRangeUnitWidth;
	}

	return nRet;
}
void CTabSonikChart::CountData(int* pAllData, int &nAllCount, int* pInData, int nInCount, int &nMin)
{
	int nIndex = 0;
	nMin = RoundToUnit( pInData[0]);
	int nData;
	for(int i=0; i<nInCount; i++)	
	{
		nData = pInData[i];
		nIndex = ( RoundToUnit(nData) - nMin) / m_nRangeUnitWidth;
		if( nIndex > nAllCount -1){
			AfxMessageBox("nAllCount �ʰ� - ����������");
			break;
		}
		pAllData[nIndex]++;
	}

	nAllCount = nIndex + 1;
}

/* ---------------------------------------------------------------------------------------
// ó������ 3. ����� TeeChart�� ǥ���Ѵ�.
// * ����(0)�� ���� �����ʹ� ��Ʈ�� �׷����� �ʴ´�. 
// �׸��� ���ͰǼ��� �սǰǼ��� ���Ե��� ������ �ѸŸŰǼ��� ���ԵǾ� �·��� �ݿ��ȴ�.
--------------------------------------------------------------------------------------- */
void CTabSonikChart::DisplayData2Chart(int* pAllData, int nAllCount, int nMin)
 {
	m_ctlBarTChart.Series(0).Clear();
	

    COLORREF nRed, nBlue, clr; //nBlack
	nRed = RGB(255, 000, 000);
	nBlue = RGB(000, 000, 255);

	//nBlack = RGB(000, 000, 000); //����

	int	nPPos=0, nMPos=0, nBPos=0 ;
	double nValue;
	int nIndex =0;//KHD
	for (int i = 0; i < nAllCount; i++){
		nValue = (double)pAllData[i];
        
		nIndex = i * m_nRangeUnitWidth + nMin;
		if(nIndex < 0 && nValue>0)
		{
			clr = nBlue;
			nMPos += (int)nValue;
		}
		else if( nIndex>0 && nValue>0)
		{
			clr = nRed;
			nPPos += (int)nValue;
		}
		else if( nIndex==0 && nValue>0)
		{
			//clr = nBlack;
			nBPos += (int)nValue;
		}           
	
		TRACE("i[%d] = Data=%d\n", nIndex, pAllData[i]);

		if( nIndex > 0) m_ctlBarTChart.Series(0).AddXY(nIndex - m_nRangeUnitWidth / 2., nValue, "", nRed);
		else if( nIndex < 0) m_ctlBarTChart.Series(0).AddXY(nIndex + m_nRangeUnitWidth / 2., nValue, "", nBlue);
	}

	CString szTmp;
	szTmp.Format("%d", nMPos+nPPos+nBPos);
	SetDlgItemText(IDC_STC_TOTAL_COUNT, szTmp);		//�ѸŸŰǼ�

	szTmp.Format("%d", nPPos);			
	SetDlgItemText(IDC_STC_EARNING_COUNT, szTmp);	//���ͰǼ�

	szTmp.Format("%d", nMPos);
	SetDlgItemText(IDC_STC_LOSS_COUNT, szTmp);		//�սǰǼ�

	double dPer = ( (double)nPPos/(double)(nMPos+nPPos+nBPos) ) * 100;;
	szTmp.Format("%.2f%%", dPer);
	SetDlgItemText(IDC_STC_EARNING_RATIO, szTmp);	//�·�

	return;
}