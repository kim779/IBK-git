// GpEdit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TB205001.h"
#include "GpEdit.h"


// CGpEdit

IMPLEMENT_DYNAMIC(CGpEdit, CEdit)

CGpEdit::CGpEdit()
{

}

CGpEdit::~CGpEdit()
{
}


BEGIN_MESSAGE_MAP(CGpEdit, CEdit)
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CGpEdit �޽��� ó�����Դϴ�.




BOOL CGpEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	return CEdit::PreTranslateMessage(pMsg);
}


void CGpEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(nChar == VK_RETURN)
		GetParent()->SendMessage(WM_USER + 888, 1, 0);
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
