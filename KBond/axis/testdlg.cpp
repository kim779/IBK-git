// testdlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "axis.h"
#include "testdlg.h"
#include "afxdialogex.h"


// testdlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(testdlg, CDialogEx)

testdlg::testdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(testdlg::IDD, pParent)
{

}

testdlg::~testdlg()
{
}

void testdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(testdlg, CDialogEx)
END_MESSAGE_MAP()


// testdlg �޽��� ó�����Դϴ�.
