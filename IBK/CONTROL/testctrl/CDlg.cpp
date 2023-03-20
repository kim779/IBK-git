// CDlg.cpp: 구현 파일
//

#include "StdAfx.h"
#include "testctrl.h"
#include "CDlg.h"
#include "afxdialogex.h"


// CDlg 대화 상자

IMPLEMENT_DYNAMIC(CDlg, CDialog)

CDlg::CDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG, pParent)
{

}

CDlg::~CDlg()
{
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg, CDialog)
END_MESSAGE_MAP()


// CDlg 메시지 처리기
