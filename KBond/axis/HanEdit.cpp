// HanEdit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "axis.h"
#include "HanEdit.h"
#include <imm.h>

// CHanEdit

IMPLEMENT_DYNAMIC(CHanEdit, CWnd)

CHanEdit::CHanEdit()
{

}

CHanEdit::~CHanEdit()
{
}


BEGIN_MESSAGE_MAP(CHanEdit, CWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CHanEdit �޽��� ó�����Դϴ�.




int CHanEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}


void CHanEdit::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	/*
	DWORD	dwConversion,dwSentence;

	HIMC hImc = ImmGetContext(this->m_hWnd);
	if ( ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		m_Conversion = dwConversion;
		m_Sentence = dwSentence;
#if 1	// for korean
		dwConversion |= IME_CMODE_HANGEUL;
#else	// for english
		if (dwConversion & IME_CMODE_HANGEUL)
			dwConversion -= IME_CMODE_HANGEUL;
#endif
		ImmSetConversionStatus(hImc, dwConversion, dwSentence);
	}
	*/
	//if (this != pOldWnd && !m_bSelect)
	//	GetParent()->PostMessage(WM_SELECT, 0, 0);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
