// SEdit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SEdit.h"


// SEdit

IMPLEMENT_DYNAMIC(SEdit, CEdit)

SEdit::SEdit()
{

}

SEdit::~SEdit()
{
}


BEGIN_MESSAGE_MAP(SEdit, CEdit)
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// SEdit �޽��� ó�����Դϴ�.




void SEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(nChar == VK_RETURN)
		GetParent()->SendMessage(WM_USER+ 888, 10, 0);
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
