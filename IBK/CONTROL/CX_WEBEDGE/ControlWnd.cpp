// ControlWnd.cpp: 구현 파일
//

#include "stdafx.h"
#include "../../H/axisfire.h"
#include "CX_WEBEDGE.h"
#include "ControlWnd.h"
#include "AxBrowser.h"


// ControlWnd

IMPLEMENT_DYNAMIC(ControlWnd, CWnd)

ControlWnd::ControlWnd()
{
	m_pHtml = nullptr;
	m_sURL = _T("");
	//m_pApp = (Ccx_THtmlExApp*)AfxGetApp();
	m_bScroll = true;
	m_scrGap = 0;
}

ControlWnd::~ControlWnd()
{
}


BEGIN_MESSAGE_MAP(ControlWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// ControlWnd 메시지 처리기




int ControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (m_bScroll)
	{
		m_Htmlrc.SetRect(-2, -2, m_Param.rect.Width() + 2, m_Param.rect.Height() + 2);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_Htmlrc, this, 0);
	}
	else
	{
		std::unique_ptr<CWnd> pRefView = std::make_unique<CWnd>();
		pRefView->Create(NULL, NULL, WS_CHILD | WS_VSCROLL, CRect(0, 0, 50, 50), this, 0);

		CRect	crc;
		pRefView->GetClientRect(&crc);
		pRefView.reset();

		m_scrGap = 50 - crc.Width();

		m_Htmlrc.SetRect(-2, -2, m_Param.rect.Width() + 2 + m_scrGap, m_Param.rect.Height() + 2 + m_scrGap);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_Htmlrc, this, 0);
		m_pHtml->SetParentView(m_pParent);
		m_pHtml->m_scrGap = m_scrGap;
	}
	if (IsDev())
	{
		m_sURL.Replace("www.ibks.com", "test.ibks.com");
	}

	if (!m_sURL.IsEmpty())	
		m_pHtml->Navigate2(m_sURL);

	return 0;
}

BOOL ControlWnd::IsDev()
{
	CString strdata, name;
	name = Variant(nameCC, "");

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s\\%s.ini", m_sRoot, "user", name, name);
	readL = GetPrivateProfileString("MODE", "DEV", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return 0;

	CString sDEV;
	sDEV.Format("%s", readB);
	if (sDEV == "1")
		return TRUE;
	else
		return FALSE;
}

void ControlWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!m_pHtml)
		return;

	CRect	rc;
	GetWindowRect(&rc);

	m_pHtml->SetWindowPos(NULL, -2, -2, rc.Width() + 2 + m_scrGap, rc.Height() + 2 + m_scrGap, NULL);
	Invalidate();
}

void ControlWnd::SetParam(_param* pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString	sToken;
	sToken = _T("/wurl");
	int nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		nPos = sTmp.Find("/");
		if (nPos < 0)
			m_sURL = sTmp;
		else
			m_sURL = sTmp.Left(nPos);
	}

	sToken = _T("/d");
	nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		if (sTmp == "1")
			m_bScroll = true;
		else
			m_bScroll = false;
	}
	else
		m_bScroll = false;

}

CString ControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}