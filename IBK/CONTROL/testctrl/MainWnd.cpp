// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "testctrl.h"
#include "MainWnd.h"
#include "CGrpWnd.h"
#include "CCtrlWnd.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
#define	sz_hCTRL	16

CMainWnd::CMainWnd(CWnd* pWnd, void* pInfo)
{
	EnableAutomation();

	CTestctrlApp* pApp = (CTestctrlApp*)AfxGetApp();
	pApp->SetView(pWnd);

	m_show = FALSE;
	m_bCtrl = false;
	m_pParent = pWnd;

	if (pInfo)
	{
		const	struct _param* pParam = (_param*)pInfo;
		m_param.name.Format("%s", pParam->name);
		m_param.rect = pParam->rect;
		m_param.fonts.Format("%s", pParam->fonts);
		m_param.point = pParam->point;
		m_param.style = pParam->style;
		m_param.tRGB = pParam->tRGB;
		m_param.pRGB = pParam->pRGB;
		m_param.options.Format("%s", pParam->options);
		m_param.options = "/i01000120000500010100000000000000000010000000000113003355468720000000";
		if (m_param.options.GetLength() >= sz_GRPINFO + sz_GRPLINE + 2)
		{
			const	int	index = m_param.options.Find("/i", 0);
			if (index >= 0)
			{
				const	CString	sInfo = m_param.options.Mid(index + 2);
				const	_GrpInfo* pGrpInfo = (_GrpInfo*)sInfo.operator LPCTSTR();
				if (pGrpInfo->ctrl > ICF_CHAR)
					m_bCtrl = true;
			}
		}
	}

	m_pGrpWnd = nullptr;
	m_pCtrlWnd = nullptr;
}

CMainWnd::CMainWnd(CWnd* pWnd, _param* pInfo)
{
//	EnableAutomation();
//	m_pParent = pWnd;
//	SetParam(pInfo);
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(CM_CTRL, OnCtrl)
	ON_MESSAGE(CM_GRP, OnGrp)
	ON_MESSAGE(CM_PREVIEW, OnPreview)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_PROPERTY_NOTIFY_ID(CMainWnd, "show", dispidshow, m_show, OnshowChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CMainWnd, "visible", dispidvisible, m_visible, OnvisibleChanged, VT_BOOL)
	DISP_FUNCTION_ID(CMainWnd, "GetProperties", dispidGetProperties, GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CMainWnd, "SetProperties", dispidSetProperties, SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "Clear", dispidClear, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetShowLine", SetShowLine, VT_EMPTY, VTS_I2 VTS_BOOL)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {F8C1F100-01F1-4BA3-BE39-2A0048303755}
static const IID IID_IMainWnd =
{ 0xf8c1f100, 0x1f1, 0x4ba3, { 0xbe, 0x39, 0x2a, 0x0, 0x48, 0x30, 0x37, 0x55 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrpWnd)
		return m_pGrpWnd->SendMessage(WM_USER, wParam, lParam);

	return 0;
}

LONG CMainWnd::OnCtrl(WPARAM wParam, LPARAM lParam)
{
	if (m_pCtrlWnd)
		return m_pCtrlWnd->SendMessage(CM_CTRL, wParam, lParam);

	return 0;
}

LONG CMainWnd::OnGrp(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrpWnd)
		return m_pGrpWnd->SendMessage(CM_GRP, wParam, lParam);

	return 0;
}

LONG CMainWnd::OnPreview(WPARAM wParam, LPARAM lParam)
{
	if (!m_pGrpWnd)
		return 0;

	int	len;
	char* pData = ((CTestctrlApp*)AfxGetApp())->GetSampleData(lParam, wParam, &len);

	if (pData)
	{
		std::unique_ptr<char[]> data(pData);
		m_pGrpWnd->SendMessage(WM_USER, MAKEWPARAM(DLL_OUB, len), (LPARAM)pData);
		//delete[] pData;
	}

	return 0;
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//AfxEnableControlContainer();

	m_pGrpWnd = std::make_unique<CGrpWnd>(m_pParent, this, &m_param);
	m_pGrpWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

	if (m_bCtrl)
	{
		m_pCtrlWnd = std::make_unique<CCtrlWnd>(m_pParent, this);
		m_pCtrlWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);
	}

	Resize();

	return 0;
}

void CMainWnd::OnDestroy()
{
	if (m_pGrpWnd)
		m_pGrpWnd->DestroyWindow();
	if (m_pCtrlWnd)
		m_pCtrlWnd->DestroyWindow();

	CWnd::OnDestroy();
}

void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (cx <= 0 && cy <= 0)
		return;

	Resize();
}

void CMainWnd::Resize()
{
	CRect	rc;
	GetClientRect(&rc);

	if (rc.Width() == 0 || rc.Height() == 0)
		return;

	if (!m_pGrpWnd && !m_pCtrlWnd)
		return;

	if (m_pCtrlWnd)
	{
		m_pGrpWnd->MoveWindow(0, 0, rc.Width(), rc.Height() - sz_hCTRL);
		m_pCtrlWnd->MoveWindow(0, rc.Height() - sz_hCTRL, rc.Width(), sz_hCTRL);
	}
	else
	{
		m_pGrpWnd->MoveWindow(0, 0, rc.Width(), rc.Height());
	}
}

BSTR CMainWnd::GetProperties()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.

	return strResult.AllocSysString();
}

void CMainWnd::SetProperties(BSTR str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}

void CMainWnd::Clear()
{
	if (m_pGrpWnd)
		m_pGrpWnd->ClearGrp();
}

void CMainWnd::OnshowChanged()
{
	ShowWindow(m_show);
}

void CMainWnd::OnvisibleChanged()
{
	if (m_show)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CMainWnd::SetData(LPCTSTR str)
{
	if (!m_pGrpWnd)
		return;

	const	int	len = strlen(str);
	if (len > 0)
		m_pGrpWnd->SendMessage(WM_USER, MAKEWPARAM(DLL_OUB, len), (LPARAM)str);
}

BOOL CMainWnd::GetVisible()
{
	// TODO: Add your property handler here

	return IsWindowVisible();//TRUE;
}

void CMainWnd::SetVisible(BOOL bNewValue)
{
	// TODO: Add your property handler here
	if (bNewValue)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CMainWnd::SetShowLine(SHORT nLine, BOOL bShow)
{
	m_pGrpWnd->SetShowLine(nLine, bShow);  //test  not yet 20230308
}


















void CMainWnd::sendTR_1(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey)
{ 
	//CString suser;
	//suser = Variant(nameCC);
	//
	//CString trData = ""; 
	//CString tempdata = "";
	//CString pass;
	//
	//struct _userTH udat;
	//memset((void*)&udat, 0, sizeof(udat));
	//
	//strcpy(udat.trc, name);
	//udat.key = key;
	//udat.stat = type;
	//
	//trData = uniqueKey;	
	//trData += keyName;
	//trData += '\t';
	//trData += CString((char *)&udat, L_userTH);
	//trData += data;
	//
	////	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strlen(data)), (LPARAM)(LPCTSTR)trData);
	//
	//LRESULT RESULT = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, trData.GetLength() - L_userTH - m_param.name.GetLength() - 1), (LPARAM)trData.operator LPCTSTR());
}

void CMainWnd::sendTR(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey)
{
//	// 	OutputDebugString("[KSJ] SEndTR");
//	
//	CString trData = ""; 
//	CString pass;
//	
//	struct _userTH udat;
////	memset((void*)&udat, 0, sizeof(udat));
//	
//	strcpy(udat.trc, name);
//	udat.key = m_param.key;
//	udat.stat = type;
//	
//	trData = (BYTE)uniqueKey;
//	trData += keyName;
//	trData += _T("\t");
//
//	trData += CString((char *)&udat, L_userTH);
//	trData += data;
//	
//	
//	
//	//	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strlen(data)), (LPARAM)(LPCTSTR)trData);
//	int nnn = trData.GetLength() - L_userTH - m_param.name.GetLength() - 1;
//
//	LRESULT RESULT = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength() ), (LPARAM)trData.operator LPCTSTR());

}

BOOL CMainWnd::SendTR(CString strName, BYTE type, CString strData, BYTE key)
{
	//CString trData = "";
	//
	//struct _userTH udat;
	//
	//memset((void*)&udat, 0, sizeof(udat));	
	//strcpy(udat.trc, strName);
	//udat.stat = type | US_KEY;
	//
	//udat.key = m_param.key;

	//trData = (BYTE)key;//*/TRKEY_GROUP;
	//trData += m_param.name;
	//trData += _T("\t");

	//trData += CString((char *)&udat, L_userTH);
	//trData += strData;
	//
	//int nRet = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM)(LPCTSTR)trData);
	//return nRet;
	return 0;
}

void CMainWnd::SearchGroupList(bool bInit)  //최초 열때 조회, 설정창으로 그룹등 변경후 조회 두가지로 나뉨
{
	
	CString strSendData;
	struct _updn updn;
	
	FillMemory(&updn, sizeof(_updn), ' ');
	ZeroMemory(&updn, sizeof(_updn));
	
	CopyMemory(&updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'G';
	CopyMemory(updn.uinfo.nblc, _T("00001"), sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'U';
	
	strSendData = CString((char*)&updn, sizeof(_updn));
//endTR(_T("pidomyst"), NULL, strSendData  , (BYTE)TRKEY_GROUP);
	sendTR_1(_T("pidomyst"), strSendData , US_KEY, m_param.key, m_param.name, 0x11);
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}

void CMainWnd::SetParam(_param *pParam)
{
	m_param.key = pParam->key;
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	// 	m_clrForeColor = m_Param.tRGB = pParam->tRGB;
	// 	m_clrBackColor = m_Param.pRGB = pParam->pRGB;
	m_param.options = CString(pParam->options, pParam->options.GetLength());

}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 9999:
		{
			KillTimer(nIDEvent);
			SearchGroupList(true);
			Sendtest();
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}





void CMainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.


}

#define TRKEY_GROUP			255
void CMainWnd::Sendtest()
{
	CString strData;
	strData = "";
	SendTR(_T("PIDOSETa"), NULL, "1", (BYTE)TRKEY_GROUP);
}


