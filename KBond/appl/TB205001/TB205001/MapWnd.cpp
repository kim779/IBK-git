// MapWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TB205001.h"
#include "MapWnd.h"


// CMapWnd

IMPLEMENT_DYNAMIC(CMapWnd, CWnd)

CMapWnd::CMapWnd()
{
	m_pWnd = NULL;
}

CMapWnd::CMapWnd(CWnd* pWizard)
{
	//m_pWizard = pWizard;
}

CMapWnd::~CMapWnd()
{
	CWnd *pMain = AfxGetMainWnd();
	if(pMain->GetSafeHwnd())
	{
		pMain->PostMessage(WM_USER + 500 , axINTERSETOFF, (LPARAM)this);
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
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()



// CMapWnd �޽��� ó�����Դϴ�.


LRESULT CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
		{
			char* pBytes = (char *)lParam;
			struct	_ledgerH * pledger = NULL;
			pledger = (struct _ledgerH*)lParam;
			CString strkey;
			strkey = CString(pledger->fnam, 30).TrimRight();

			//int igubn = atoi(CString(&pBytes[sizeof(_ledgerH)], 1));
			if(strkey  == "selectAttGrpList")    //���� ����Ʈ ��ȸ 
			{
				m_pWnd->oubSearchList(pBytes);
			}
			else if(strkey == "doAttGrpItm")   // �������� ��ȸ , �߰� �� ���� 
			{
				m_pWnd->oubInterListCode(pBytes);
			}
			else if(strkey == "saveAttGrpSeq")   //���� ���� ����
			{
				m_pWnd->oubInterListOrder(pBytes);
			}
			else if(strkey == "doAttGrp")  //���ɱ׷� �߰�/����/����
			{
				m_pWnd->oubInterList(pBytes);
			}		
			else if(strkey = "deleteAttGrpMulti")
			{
				m_pWnd->oubRemoveAllgroup(pBytes);
			}
		}
		break;
		case DLL_ALERTx:
		{	
			
		}
		break;
		case DLL_TRIGGER:
		{
			
		}
	}
	return 0;
}

void CMapWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, RGB(255,255,255));
}


int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	Variant(titleCC, _T("����������"));
	
	m_pWnd = new CInterestPanel(this);
	m_pWnd->Create(IDD_INTEREST, this);
	m_pWnd->ShowWindow(SW_SHOW);

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

CString CMapWnd::GetUser()
{
	return Variant(userCC);
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
				
				CString strgroup;
				if(!m_pWnd->addcodeCheck(strgroup))
				{
					CString msg;
					msg.Format(_T("�⺻�׷��� %s���� ������ �߰��� �� �����ϴ�."), strgroup);
					AfxMessageBox(msg);
				}
				else
					m_pWnd->addCode(strdata);
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

void CMapWnd::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CWnd *pMain = AfxGetMainWnd();
	if(pMain->GetSafeHwnd())
	{
		pMain->SendMessage(WM_USER + 500 , axINTERSETOFF, (LPARAM)this);
	}
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(closeDLL, 0), 0);
	CWnd::OnClose();
}


BOOL CMapWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CWnd::PreCreateWindow(cs);
}


void CMapWnd::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
