// AccEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Account.h"
#include "AccEdit.h"
#include "AccountCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccEdit

CAccEdit::CAccEdit()
	: m_bEditMode(TRUE)
{
	m_bAllSelected =  FALSE;
	m_bEditNumType = TRUE;		// 2012.02.13 KSJ ������������ ������������.
	m_bKeyDown = FALSE;			// Ű���� �ٿ�ƴ��� üũ
}

CAccEdit::~CAccEdit()
{
}


BEGIN_MESSAGE_MAP(CAccEdit, COXMaskedEdit)
	//{{AFX_MSG_MAP(CAccEdit)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccEdit message handlers

int CAccEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COXMaskedEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CAccEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bEditMode)
		return;
	
	if (nChar == VK_DOWN)
	{
		((CAccountCtrl*)GetParent())->PushDownKey();
		return;
	}
	else if (nChar == VK_UP)
	{
		((CAccountCtrl*)GetParent())->PushUpKey();
			return;
	}
	if (nChar == VK_LEFT)
	{
		((CAccountCtrl*)GetParent())->PushLeftKey();
		return;
	}
	else if (nChar == VK_RIGHT)
	{
		((CAccountCtrl*)GetParent())->PushRightKey();
		return;
	}

	COXMaskedEdit::OnChar(nChar, nRepCnt, nFlags);

	CString strAccount = GetInputData();
	
	strAccount.Trim();

	int nStart, nEnd;

	GetSel(nStart, nEnd);
	
	if ((strAccount.GetLength() == 11 && nStart == 13 && nEnd == 13) ||
		(strAccount.GetLength() == 11 && nChar == 0x0D))
	{
		((CAccountCtrl*)GetParent())->AccountInputComplete(strAccount);
		//((CAccountCtrl*)GetParent())->QueryAccntInfo(strAccount);
		((CAccountCtrl*)GetParent())->PushTab();
	}

}

void CAccEdit::OnKillFocus(CWnd* pNewWnd) 
{
	COXMaskedEdit::OnKillFocus(pNewWnd);

// 	TRACE("%d\n", pNewWnd->GetSafeHwnd());
	
	CString strAccount = GetInputData();

	strAccount.Trim();

	m_bAllSelected = FALSE;
//	if (strAccount.GetLength() == 11)
//		((CAccountCtrl*)GetParent())->AccountInputComplete(strAccount);
}

// 2012.02.02 KSJ
#define ACC_DEPT			3
#define ACC_TYPE			2
#define ACC_NO				6
#define ACC_LENGTH			ACC_DEPT + ACC_TYPE + ACC_NO + 2
// KSJ

// numberpad�� ����Ű�� ���ڷ� �ν��Ѵ�.
// 2012.02.13 KSJ
bool CAccEdit::IsNumber(UINT nChar)
{
	bool bResult = false;

	if ((nChar >= 0x30 && nChar <= 0x39) || (nChar >= 0x60 && nChar <= 0x69))
		bResult = true;

	return bResult;
}

bool CAccEdit::IsSpecialKey(UINT nChar)
{
	bool bResult = false;
	
	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_BACK || nChar == VK_DELETE
		|| nChar == VK_UP || nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_DOWN || nChar == VK_CONTROL)	//2012.12.24 Ctrl +C ,V�� ���ؼ� VK_CONTROL �߰���
		bResult = true;
	
	return bResult;
}

void CAccEdit::SetAccMask()
{
	m_bEditNumType = TRUE;
	char szMask[ACC_LENGTH + 1];
	memset((void*)szMask, _T('#'), ACC_LENGTH);
	
	// 2012.02.02 KSJ �ڵ��ؼ� ���⼭ # digit�� ������. �׷��� ���ڸ� �Էµ�.
	szMask[ACC_DEPT] = _T('-');
	szMask[ACC_DEPT + ACC_TYPE + 1] = _T('-');
	szMask[ACC_LENGTH] = NULL;
	
	((CAccountCtrl*)GetParent())->GetEdit()->SetMask(szMask);
}

// KSJ

void CAccEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	bool bTrans = false;	//2012.02.02 KSJ
	m_bKeyDown = TRUE;		//2012.02.15 KSJ

	if (nChar == VK_TAB)
	{
		((CAccountCtrl*)GetParent())->PushTab();
	}
	else if (nChar == VK_DOWN)
	{
		((CAccountCtrl*)GetParent())->PushDownKey();
		return;
	}
	else if (nChar == VK_UP)
	{
		((CAccountCtrl*)GetParent())->PushUpKey();
		return;
	}
	else if (nChar == VK_LEFT)
	{
		((CAccountCtrl*)GetParent())->PushLeftKey();
		return;
	}
	else if (nChar == VK_RIGHT)
	{
		((CAccountCtrl*)GetParent())->PushRightKey();
		return;
	}
	

	// 2012.02.02 KSJ
	// m_bChangeEditType ó���� �ѹ� ������
	// ���ڰ� �����ٰ� ���ڰ� ���� ��쳪
	// ���ڰ� �����ٰ� ���ڰ� ���� ���
	// TRUE �������ش�.

	//������ �ƴҶ��� �Ѿ��.
	if (!m_bEditMode)
		return;

	if (m_bEditNumType)		//���������϶�
	{
		if (!IsNumber(nChar) && !IsSpecialKey(nChar))
			bTrans = true;
	}
	else					//���������϶�
	{
		if (IsNumber(nChar))
			bTrans = true;
	}

	if (bTrans)
	{
		if (IsNumber(nChar))
		{
			SetAccMask();
		}
		else
		{
			m_bEditNumType = FALSE;
			((CAccountCtrl*)GetParent())->GetEdit()->SetMask("");
		}
	}
	
	// KSJ
	
	COXMaskedEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}

void CAccEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if (!m_bKeyDown || !m_bEditMode)
		return;

	m_bKeyDown = FALSE;

	CString	string; 
	GetWindowText(string);
	string.Replace("-","");
	string.Replace(" ","");

	//2012.12.24 KSJ ���� �ٿ��ֱ� ó��
	//GetAsyncKeyState 0x8000 �����Է�, 0x8001 ó���Է�
	if (((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (nChar == 'C' || nChar == 'c')) || (((GetAsyncKeyState(0x63) & 0x8001) || (GetAsyncKeyState(0x43) & 0x8001)) && nChar == VK_CONTROL))
	{
		string.Insert(3,"-");
		string.Insert(6,"-");
		SetClipboard(string);

		return;
	}
	else if (((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (nChar == 'V' || nChar == 'v')) || (((GetAsyncKeyState(0x76) & 0x8001) || (GetAsyncKeyState(0x56) & 0x8001)) && nChar == VK_CONTROL))
	{
		string = GetClipboard();
		string.Replace("-","");
		string.Replace(" ","");
		string.Trim();

		if (atof(string) > 0 && string.GetLength() == 11)
		{
			SetAccMask();	//MASK����

			string.Insert(3,"-");
			string.Insert(6,"-");

			SetWindowText(string);

			GetParent()->SendMessage(WM_EDITX, wpSETACCOUNT, (LPARAM)string.GetString());	//m_pEdit->SetInputData((char*)lParam);�� Account ����

			//2013.01.22 KSJ AccountInputComplete���� Change �̺�Ʈ�� �߻��ϱ⶧���� �ٽ� string�� �����Ͽ� ȣ����.
			string.Replace("-","");
			string.Replace(" ","");
			
			((CAccountCtrl*)GetParent())->AccountInputComplete(string);	//�������� �������� ��ȸ
		}

		return;
	}
	//2012.12.24 KSJ ���� �ٿ��ֱ� ó�� END
	
	if (string.GetLength() <= 0)
	{
		GetParent()->SendMessage(WM_EDITX, wpBLANK, 0);
		return;
	}
	
	int	xChar = 0;
	bool	digit = false;
	for (int ii = 0; ii < string.GetLength(); ii++)
	{
		xChar = (int)string.GetAt(ii);
		if (xChar >= (int)'0' && xChar <= (int)'9')
		{
			digit = true;
			continue;
		}
		digit = false;
		break;
	}
	
	if (digit)
		GetParent()->SendMessage(WM_EDITX, wpSEARCH1, 0);
	else if (nChar == VK_RETURN)
		GetParent()->SendMessage(WM_EDITX, wpENTER, 0);
	else
		GetParent()->SendMessage(WM_EDITX, wpSEARCH2, 0);

	COXMaskedEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	Invalidate();
}

// 2012.02.22 KSJ ����Ʈ��Ʈ�� �ѱ۷� �ٲٱ�
#include "imm.h"
#pragma comment(lib, "imm32")
// KSJ

void CAccEdit::OnSetFocus(CWnd* pOldWnd) 
{
	Invalidate();
	COXMaskedEdit::OnSetFocus(pOldWnd);

	// 2012.02.22 KSJ ����Ʈ��Ʈ�� �ѱ۷� �ٲٱ�
	HIMC himc=ImmGetContext(this->GetSafeHwnd());
    ImmSetConversionStatus(himc, IME_CMODE_NATIVE,IME_SMODE_CONVERSATION);
    ImmReleaseContext(this->GetSafeHwnd(),himc);
	// KSJ
	
//	if (!m_bEditMode)
		SetSel(0, -1);
}

void CAccEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	COXMaskedEdit::OnLButtonDown(nFlags, point);

//	if (!m_bEditMode)
//		SetSel(0, -1);

	if (!m_bAllSelected)
	{
		SetSel(0, -1);
		m_bAllSelected = TRUE;
	}
}

LRESULT CAccEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	if (message == WM_MOUSEWHEEL)
		AfxMessageBox("WM_MOUSEWHEEL");
*/
	if (message == WM_KEYDOWN)
	{
		if (wParam == VK_CONTROL)
		{
		}
	}

	return COXMaskedEdit::WindowProc(message, wParam, lParam);
}

void CAccEdit::SetClipboard(CString strData)
{
	HGLOBAL hGlobal = GlobalAlloc(GHND | GMEM_SHARE, (strData.GetLength() + 1) *sizeof(TCHAR));

	OpenClipboard();
	if (hGlobal)
	{
		PSTR	pGlobal = (PSTR)GlobalLock(hGlobal);
		if (pGlobal)
			lstrcpy(pGlobal, TEXT(strData));
		GlobalUnlock(hGlobal);

		EmptyClipboard();
		SetClipboardData(CF_TEXT, hGlobal);
	}
	CloseClipboard();
}

CString CAccEdit::GetClipboard()
{
	CString strData;
	char	szString[1024]{};

	OpenClipboard();
	HGLOBAL hGlobal = GetClipboardData(CF_TEXT); 
	if (hGlobal) 
	{ 
		const	char*	pGlobal = (char *)GlobalLock(hGlobal); 
		if (pGlobal)
			lstrcpy(szString, pGlobal); 
		
		GlobalUnlock(hGlobal); 
	} 
	CloseClipboard();

	strData.Format("%s", szString);
	return strData;
}

BOOL CAccEdit::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COXMaskedEdit::OnCommand(wParam, lParam);
}
