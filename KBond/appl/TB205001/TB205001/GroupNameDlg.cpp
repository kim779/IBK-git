// GroupNameDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TB205001.h"
#include "GroupNameDlg.h"
#include "afxdialogex.h"


// CGroupNameDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGroupNameDlg, CDialogEx)

CGroupNameDlg::CGroupNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroupNameDlg::IDD, pParent)
{

}

CGroupNameDlg::~CGroupNameDlg()
{
}

void CGroupNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGroupNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGroupNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGroupNameDlg �޽��� ó�����Դϴ�.


BOOL CGroupNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	UpdateData(FALSE);
	
	CRect rcCtrl;
	GetParent()->GetWindowRect(&rcCtrl);
	CPoint	point;		
	GetCursorPos(&point);
	CRect rcDlg;
	GetClientRect(&rcDlg);

	MoveWindow(point.x, point.y, rcDlg.Width(), rcDlg.Height());
	((CWnd*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_strGroupName);
	((CWnd*)GetDlgItem(IDC_EDIT1))->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



void CGroupNameDlg::SetGroupName(LPCTSTR lpszGroupName)
{
	CString tmpGrpName = lpszGroupName;

	tmpGrpName.TrimRight();

	m_strGroupName = tmpGrpName;
}

LPCTSTR CGroupNameDlg::GetGroupName()
{
	return (LPCTSTR)m_strGroupName;
}


void CGroupNameDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CWnd*)GetDlgItem(IDC_EDIT1))->GetWindowText(m_strGroupName);
	CDialogEx::OnOK();
}


BOOL CGroupNameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	
	//{
	//	OnBnClickedOk();
	//	return true;
	//}
		 
	return CDialogEx::PreTranslateMessage(pMsg);
}


