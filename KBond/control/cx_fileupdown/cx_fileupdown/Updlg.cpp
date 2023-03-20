// Updlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "cx_fileupdown.h"
#include "Updlg.h"
#include "afxdialogex.h"


// CUpdlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUpdlg, CDialogEx)

CUpdlg::CUpdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdlg::IDD, pParent)
{

}

CUpdlg::~CUpdlg()
{
}

void CUpdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UP, m_uplist);
}


BEGIN_MESSAGE_MAP(CUpdlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_FILE, &CUpdlg::OnBnClickedBtnFile)
END_MESSAGE_MAP()


// CUpdlg �޽��� ó�����Դϴ�.


void CUpdlg::OnBnClickedBtnFile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	char name_filter[] = "All Files (*.*)|*.*|";
	CFileDialog ins_dlg(TRUE, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);
	// ���� ���� �޺��ڽ��� ������ ���͵� �߿��� 2��° �׸�(*.cpp)�� �����Ѵ�.
	ins_dlg.m_ofn.nFilterIndex = 2;

	if(ins_dlg.DoModal() == IDOK)
	{
		CString str(ins_dlg.GetPathName());
		
	}
}
