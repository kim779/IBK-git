// testwnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TB205001.h"
#include "testwnd.h"


// testwnd

IMPLEMENT_DYNAMIC(testwnd, CWnd)

testwnd::testwnd()
{

}

testwnd::~testwnd()
{
}


BEGIN_MESSAGE_MAP(testwnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_USER + 999, OnAXIS)
END_MESSAGE_MAP()



// testwnd �޽��� ó�����Դϴ�.



LRESULT testwnd::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case axADDINTERSET:
			{
				CString strdata, tmps, strcode, strname, strsuik, strmangi;
				strdata.Format(_T("%s"), (char*)lParam);
				strname = Parser(strdata);
				strcode = Parser(strdata);
				strsuik = Parser(strdata);
				strmangi = Parser(strdata);
				m_pWnd->addCode(strname, strcode, strsuik, strmangi);
			}
			break;
		case axDELINTERSET:
			{
				m_pWnd->DeleteItem();
			}
			break;
	}
	return 1;
}

CString testwnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strValue;
	char* pValue = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pValue > 1)
		strValue = pValue;
	else
		return _T("");

	return strValue;
}

CString testwnd::GetRoot()
{
	return Variant(homeCC);
}

CString testwnd::GetName()
{
	return Variant(nameCC);
}

CWnd* testwnd::GetWizard()
{
	return m_pWizard;
}

CBitmap* testwnd::GetBitmap(CString strPath)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)(LPCTSTR)strPath);
}

COLORREF testwnd::GetIndexColor(int nIndex)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}

CString testwnd::Parser(CString &strSrc, CString strSub /* = _T("\t") */)
{
	if (strSrc.Find(strSub) == -1)
	{
		CString temp = strSrc;
		strSrc.Empty();
			return temp;
	}
	else
	{
		CString  strTemp = strSrc.Left(strSrc.Find(strSub));
		strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
		return strTemp;
	}

	return _T("");
}

int testwnd::GetKey()
{
	int key = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, minorCC), 0L);
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0L);
}

void testwnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, RGB(255,255,255));
}


int testwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	//Variant(titleCC, _T("����������"));

	return 0;
}


void testwnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void testwnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnTimer(nIDEvent);
}


void testwnd::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CWnd *pMain = AfxGetMainWnd();
	if(pMain->GetSafeHwnd())
	{
		pMain->SendMessage(WM_USER + 500 , axINTERSETOFF, (LPARAM)this);
	}
	CWnd::OnClose();
}


BOOL testwnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CWnd::PreCreateWindow(cs);
}


void testwnd::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
