// OptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "OptDlg.h"
#include "sharemsg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptDlg dialog

static struct _Idlist
{
	UINT	id;
	UINT	param;
} Idlist [] = {
		{IDC_OPT_BULSUNGSIL, OPT_BULSUNGSIL},	// �Ҽ���
		{IDC_OPT_SINGYU, OPT_SINGYU},		// �ű�
		{IDC_OPT_GUNRIRAK, OPT_GUNRIRAK},	// �Ǹ���
		{IDC_OPT_WOOSUN, OPT_WOOSUN},		// �켱��
		{IDC_OPT_KOSDAQ, OPT_KOSDAQ},		// �ڽ���
		{IDC_OPT_GUANRI, OPT_GUANRI},		// ��������
		{IDC_OPT_GAMRI, OPT_GAMRI},		// ��������
		{IDC_OPT_TRADESTOP, OPT_TRADESTOP},	// �ŷ���������
		{IDC_OPT_TRADEWARN, OPT_TRADEWARN},	// ������������
		{IDC_OPT_KOSPI, OPT_KOSPI},		// �ڽ�������
		};
static	const int Idcount = sizeof(Idlist) / sizeof(struct _Idlist);

COptDlg::COptDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(COptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptDlg, CDialog)
	//{{AFX_MSG_MAP(COptDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptDlg message handlers

int COptDlg::GetOption()
{
	int	opt = 0;
	CButton*	pButton = nullptr;

	for ( int ii = 0 ; ii < Idcount ; ii++ )
	{
		pButton = (CButton*)GetDlgItem(Idlist[ii].id);
		if (pButton->GetCheck())
			opt |= Idlist[ii].param;
	}

	return opt;
}

void COptDlg::AddOption(int opt)
{
	CButton*	pButton = nullptr;

	for ( int ii = 0 ; ii < Idcount ; ii++ )
	{
		if (opt & Idlist[ii].param)
		{
			pButton = (CButton*)GetDlgItem(Idlist[ii].id);
			pButton->SetCheck(1);
		}		
	}
}

void COptDlg::DelOption(int opt)
{
	CButton*	pButton = nullptr;

	for ( int ii = 0 ; ii < Idcount ; ii++ )
	{
		if (opt & Idlist[ii].param)
		{
			pButton = (CButton*)GetDlgItem(Idlist[ii].id);
			pButton->SetCheck(0);
		}		
	}
}

void COptDlg::SetOption(int opt)
{
	CButton*	pButton = nullptr;

	for ( int ii = 0 ; ii < Idcount ; ii++ )
	{
		pButton = (CButton*)GetDlgItem(Idlist[ii].id);
		
		if (opt & Idlist[ii].param)
		{
			pButton->SetCheck(1);
		}		
		else
		{
			pButton->SetCheck(0);
		}
	}
}

void COptDlg::ClearOption()
{
	CButton*	pButton = nullptr;

	for ( int ii = 0 ; ii < Idcount ; ii++ )
	{
		pButton = (CButton*)GetDlgItem(Idlist[ii].id);
		pButton->SetCheck(0);		
	}
}
