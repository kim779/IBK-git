// MapWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TB900100.h"
#include "MapWnd.h"
#include "../../../h/axisfire.h"

// CMapWnd

IMPLEMENT_DYNAMIC(CMapWnd, CWnd)

CMapWnd::CMapWnd(CWnd* pWizard)
{
	m_pWnd = NULL;
}

CMapWnd::~CMapWnd()
{
	CWnd *pMain = AfxGetMainWnd();
	if(pMain->GetSafeHwnd())
	{
	//	pMain->PostMessage(WM_USER + 500 , axINTERSETOFF, (LPARAM)this);
	}
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER + 999, OnAXIS)
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

// CMapWnd �޽��� ó�����Դϴ�.

void CMapWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc,RGB(76, 81, 90));
}


int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Variant(titleCC, _T("����������"));

	m_pWnd = new CUnBond(this);
	m_pWnd->Create(IDD_DIALOG_ALLBOND, this);
	m_pWnd->ShowWindow(SW_SHOW);
	Variant(titleCC, _T("����˻�"));
	return 0;
}


void CMapWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	if(m_pWnd != NULL)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		m_pWnd->MoveWindow(&rcClient);
	}
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CMapWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	CWnd::OnTimer(nIDEvent);
}

CString CMapWnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strValue;
	char* pValue = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pValue > 1)
		strValue = pValue;
	else
		return _T("");

	return strValue;
}

int CMapWnd::GetKey()
{
	int key = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, minorCC), 0L);
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0L);
}

CString CMapWnd::GetRoot()
{
	return Variant(homeCC);
}

CString CMapWnd::GetName()
{
	return Variant(nameCC);
}

CWnd* CMapWnd::GetWizard()
{
	return m_pWizard;
}

CBitmap* CMapWnd::GetBitmap(CString strPath)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)(LPCTSTR)strPath);
}

COLORREF CMapWnd::GetIndexColor(int nIndex)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}

CString CMapWnd::Parser(CString &strSrc, CString strSub /* = _T("\t") */)
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

LRESULT CMapWnd::OnAXIS(WPARAM wParam, LPARAM lParam)
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
		//		m_pWnd->addCode(strname, strcode, strsuik, strmangi);
			}
			break;
		case axDELINTERSET:
			{
		//		m_pWnd->DeleteItem();
			}
			break;
	}
	return 1;
}

void CMapWnd::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CWnd *pMain = AfxGetMainWnd();
	if(pMain->GetSafeHwnd())
	{
	//	pMain->SendMessage(WM_USER + 500 , axINTERSETOFF, (LPARAM)this);
	}
	
	CWnd::OnClose();
}

void CMapWnd::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CMapWnd::SetTitle(CString strdata)
{
	Variant(titleCC, strdata);
}