// NumericalInquiryOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "NumericalInquiryOptionDlg.h"

#include "ToolandRButtonTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ChartCtrlŬ�����κп��� �߰�.
#define NUMERICALINQUIRYDLG 0x1
#define CROSSLINEVIEW		0x2
#define TOOLTIPVIEW			0x4

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryOptionDlg dialog


CNumericalInquiryOptionDlg::CNumericalInquiryOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumericalInquiryOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNumericalInquiryOptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOption = 0;
}


void CNumericalInquiryOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNumericalInquiryOptionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNumericalInquiryOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CNumericalInquiryOptionDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_TOOLTIP, OnCheckTooltip)
	ON_BN_CLICKED(IDC_CHECK_CROSSLINEVIEW, OnCheckCrosslineview)
	ON_BN_CLICKED(IDC_RDO_NUMERICALINQUIRY, OnRdoNumericalinquiry)
	ON_BN_CLICKED(IDC_RDO_DRAGZOOM, OnRdoDragzoom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryOptionDlg message handlers


/*-------------------------------------------------------------------------------
 - Function    :  GetNumericalInquiryOption
 - Created at  :  2004-03-17   13:42
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Parameters  :  None
 - Return Value:  int (��Ʈ��. 0001:��ġ��ȸâ���, 0010:���ڼ����, 0100:��ġ��ȸ�������)
 - Description :  �ɼǻ��¸� �����´�.
 -------------------------------------------------------------------------------*/
int CNumericalInquiryOptionDlg::GetNumericalInquiryOption()
{
	return m_nOption;
}


/*-------------------------------------------------------------------------------
 - Function    :  SetNumericalInquiryOption
 - Created at  :  2004-03-17   13:54
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Parameters  :  nOption - (��Ʈ��. 0001:��ġ��ȸâ���, 0010:���ڼ����, 0100:��ġ��ȸ�������)
 - Return Value:  None
 - Description :  nOption�� ���� ������ �ɼ��� üũ���¸� �߰�.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryOptionDlg::AddNumericalInquiryOption(int nOption)
{
	m_nOption |= nOption;
}


/*-------------------------------------------------------------------------------
 - Function    :  SubNumericalInquiryOption
 - Created at  :  2004-03-17   11:28
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Parameters  :  nOption - (��)
 - Return Value:  None
 - Description :  nOption�� ���� ������ �ɼ��� üũ���¸� ��.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryOptionDlg::SubNumericalInquiryOption(int nOption)
{
	// (2005/11/17 - Seung-Won, Bae) Subtract Option
	m_nOption &= ~nOption;
	// ��ġ��ȸâ�� Disabled�Ǹ� �ڵ����� ���ڼ����⵵ Disabled�� �ȴ�.
	// (2005/11/17 - Seung-Won, Bae) It is processed in CNumericalInquiryOptionDlg::OnRdoDragzoom()
}

/*-------------------------------------------------------------------------------
 - Function    :  OnShowWindow
 - Created at  :  2004-03-17   11:01
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  ShowWindow. üũ���¸� ǥ�����ش�.
 -------------------------------------------------------------------------------*/
// (2005/11/17 - Seung-Won, Bae) It is OnInitDialog()
void CNumericalInquiryOptionDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( !bShow) return;

	// ��ġ��ȸâ��� �ɼ�
	if( m_nOption & NUMERICALINQUIRYDLG)
	{
		( ( CButton *)GetDlgItem( IDC_RDO_NUMERICALINQUIRY))->SetCheck( 1);
		// ��ġ��ȸâ��� �ɼ��� üũ�Ǿ� ������ ���ڼ����üũ�ڽ��� Ȱ��ȭ��Ŵ.
		OnRdoNumericalinquiry();	
	}
	else
	{
		( ( CButton *)GetDlgItem( IDC_RDO_DRAGZOOM))->SetCheck( 1);
		// ��ġ��ȸâ��� �ɼ��� üũ�Ǿ� ������ ���ڼ����üũ�ڽ��� Ȱ��ȭ��Ŵ.
		OnRdoDragzoom();	
	}
	// ���ڼ����
	( ( CButton *)GetDlgItem( IDC_CHECK_CROSSLINEVIEW))->SetCheck( m_nOption & CROSSLINEVIEW);
	// ��ġ��ȸ�������
	( ( CButton *)GetDlgItem( IDC_CHECK_TOOLTIP))->SetCheck( m_nOption & TOOLTIPVIEW);
}


/*-------------------------------------------------------------------------------
 - Function    :  OnCheckTooltip
 - Created at  :  2004-03-17   11:14
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  '����'üũ�ڽ� Ŭ����.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryOptionDlg::OnCheckTooltip() 
{
	CButton* pButton = NULL; //�ӽú���

	pButton = (CButton*)GetDlgItem(IDC_CHECK_TOOLTIP);
	
	if(1 == pButton->GetCheck())
		AddNumericalInquiryOption(TOOLTIPVIEW);
	else
		SubNumericalInquiryOption(TOOLTIPVIEW);
}


/*-------------------------------------------------------------------------------
 - Function    :  OnCheckCrosslineview
 - Created at  :  2004-03-17   11:38
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  '���ڼ� ����' üũ�ڽ� Ŭ����
 -------------------------------------------------------------------------------*/
void CNumericalInquiryOptionDlg::OnCheckCrosslineview() 
{
	CButton* pButton = NULL; //�ӽú���

	pButton = (CButton*)GetDlgItem(IDC_CHECK_CROSSLINEVIEW);
	
	if(1 == pButton->GetCheck())
		AddNumericalInquiryOption(CROSSLINEVIEW);
	else
		SubNumericalInquiryOption(CROSSLINEVIEW);
}

// ADD: ����(04/03/25) Enter�� Esc�� ���´�.
void CNumericalInquiryOptionDlg::OnOK()
{
	return;
}
void CNumericalInquiryOptionDlg::OnCancel()
{
	return;
}

/*-------------------------------------------------------------------------------
 - Function    :  OnCheckNumericalinquirydlg
 - Created at  :  2004-03-17   11:24
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  ��ġ��ȸâ������� üũ�Ǹ� '���ڼ����̱�'�ɼ��� Enabled��Ų��.
 - Update	   :  (03/25) '���ڼ����̱�'�ɼ��� Enabled�ʰ� ���ÿ� Check�Ѵ�.
 -------------------------------------------------------------------------------*/
// (2005/11/17 - Seung-Won, Bae) Change UI from CHECK to RADIO
void CNumericalInquiryOptionDlg::OnRdoNumericalinquiry() 
{
	// TODO: Add your control notification handler code here
	AddNumericalInquiryOption( NUMERICALINQUIRYDLG);
	CButton *pCrossLine = ( CButton *)GetDlgItem( IDC_CHECK_CROSSLINEVIEW);
	pCrossLine->EnableWindow();
	pCrossLine->SetCheck( TRUE);
	AddNumericalInquiryOption( CROSSLINEVIEW);
}
// (2005/11/17 - Seung-Won, Bae) Change UI from CHECK to RADIO
void CNumericalInquiryOptionDlg::OnRdoDragzoom() 
{
	// TODO: Add your control notification handler code here
	SubNumericalInquiryOption( NUMERICALINQUIRYDLG);
	CButton *pCrossLine = ( CButton *)GetDlgItem( IDC_CHECK_CROSSLINEVIEW);
	pCrossLine->SetCheck( FALSE);
	pCrossLine->EnableWindow( FALSE);
	SubNumericalInquiryOption( CROSSLINEVIEW);
}
