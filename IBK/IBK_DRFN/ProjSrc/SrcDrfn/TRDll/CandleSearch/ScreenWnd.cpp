// ScreenWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenWnd.h"

#include "../DR_Include/hana_typedef.h"
//#include "../../../SrcSite/include/axis_msg.h"
#include "../../../SrcSite/include/axisgenv.h"
#include "../../../SrcSite/include/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FORM_WIDTH		883
#define FORM_HEIGHT		530

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CScreenWnd

//IMPLEMENT_DYNCREATE(CScreenWnd, CWnd)
CPCTrMngHelper  m_PcTrHelper;
CScreenWnd::CScreenWnd(CWnd *pParent)
{
	m_pwndModuleMain = pParent;
	m_pwndMainFrm = NULL;
	
	m_pPtSvrMng = NULL;
	m_pPtMng = NULL;

	m_FnSet.SetInit(m_pwndModuleMain, this);
}

CScreenWnd::~CScreenWnd()
{
}


BEGIN_MESSAGE_MAP(CScreenWnd, CWnd)
	//{{AFX_MSG_MAP(CScreenWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
 	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenWnd message handlers


int CScreenWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AfxEnableControlContainer();	//solomon-090825
	
	_DoGetPartnerManger(TRUE);	//DRFN�ش��������������� Call

	// TODO: Add your specialized creation code here
	int nIndustry = -1;

	OnCreate_MainWnd(0);

	if(m_pwndMainFrm != NULL)
		m_pwndMainFrm->SetInit();

	CRect reClient;
	GetClientRect(reClient);
	m_pwndMainFrm->MoveWindow(reClient);

	m_FnSet.Variant(titleCC, PROGRAM_TITLE);

	return 0;
}

void CScreenWnd::OnDestroy() 
{
	if(m_pwndMainFrm)
	{
		m_pwndMainFrm->DestroyWindow();
	}

	_DoGetPartnerManger(FALSE);

	CWnd::OnDestroy();	
}

void CScreenWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	m_pwndMainFrm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
}


void CScreenWnd::OnCreate_MainWnd(int nIndustry)
{
	int nWidth	= -1;
	int nHeight	= -1;
	if(nIndustry == 0)
	{
		nWidth	= FORM_WIDTH;
		nHeight = FORM_HEIGHT;
	}

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_pwndMainFrm = new CMainFrmWnd;

	BOOL bResult = m_pwndMainFrm->Create(
		NULL,
		_T("Main Frame Window"),
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CRect(0, 0, 0, 0),
		this,
		NULL);
	m_pwndMainFrm->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	AfxSetResourceHandle(hInstSave);

	if(bResult)
	{
		WPARAM wp = MAKEWPARAM(USERINFO_SET, 0);
		if(m_pwndMainFrm && IsWindow(m_pwndMainFrm->GetSafeHwnd()))
			m_pwndMainFrm->SendMessage(RMSG_USERINFO, wp, (LPARAM)m_pPtMng);

		m_pwndMainFrm->ShowWindow(SW_SHOW);
		SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
		m_pwndMainFrm->SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
		m_FnSet.SetPalete();
	}
}

void CScreenWnd::ReceiveTR(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen,int nKey)
{
	m_pwndMainFrm->ReceiveData(szTR, aTRData, dwTRDateLen , nKey );
}

void CScreenWnd::SetPalete(COLORREF clrPalete, ST_TABCOLOR stTabColor)
{
	m_pwndMainFrm->ChangeSkinColor(clrPalete, stTabColor);
}

CWnd* CScreenWnd::GetModuleMainWnd()
{
	return m_pwndModuleMain;
}

LONG CScreenWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	char* pszData = (char*) lParam;

	LONG	ret = 0;
	int i = 0;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_SETPAL:
		m_FnSet.SetPalete();
		break;
		
	case DLL_ALERT:
		m_FnSet.ReceiveReal(wParam, lParam);
		break;

	case DLL_OUBx:
#if(0)//tour2k
		{
			char* pBytes = (char *)lParam;
			struct _extTH* extTH = (struct _extTH *)pBytes;
			pBytes += L_extTH;

			int nKey = extTH->key;
			int nLen = extTH->size;

			m_FnSet.ReceiveTRx(nKey, pBytes, nLen);

//			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
//			lParam = (LPARAM)pBytes;
		}
#else
		{
			_extTHx* info = (_extTHx*)lParam;
			const int nLen = info->size;
			char* pData = info->data;
			int nKey = info->key;

			m_FnSet.ReceiveTRx(nKey, pData, nLen);
		}
#endif
		break;

	case DLL_OUB:
		{
			int nkind = HIBYTE(LOWORD(wParam));

			int nKey = 0;
			int nLen = 0;

			nKey = nkind;
			nLen = (long)HIWORD(wParam);

			if(nKey == TRKEY_STRJONGMOK)
			{
				if(nLen==1)
				{
					wParam = MAKEWPARAM(LOWORD(wParam), HIWORD(wParam) - SZ_ZCOMHEAD);
					nLen = HIWORD(wParam);
				}

				LPBYTE pRBuff = new BYTE[nLen+1];
				int zcomSize = sizeof(_zcom_head);
				
				if(zcomSize < nLen)
				{
					memcpy(pRBuff, (char*)(lParam + zcomSize), nLen - zcomSize); //KHD : _zcom_head ������ �ʴµ� ������� �Ѵ�. 
				}
				else 
				{
					delete []pRBuff;
					break;
				}
				pRBuff[nLen] = 0;
				
				if(m_pPtMng) m_pPtMng->ReceiveTRData("", pRBuff, nLen, nKey);
			}
			else
				m_FnSet.ReceiveTR(wParam, lParam);
		}
		break;

	case DLL_TRIGGER:
		// 		if (m_bDestroy)
		// 			break;
		// 		ParseTrigger((char*)lParam, FALSE);
		break;
		
	case DLL_DOMINO:		//# Open Shard!
		/*ParseTrigger((char*)lParam, FALSE);*/
		break;
		
	default:
		;
	}
	
	return ret;
}

LONG CScreenWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch (LOWORD(wParam))
	{
	case MK_SETPAL:
		m_FnSet.m_clrPalete = (COLORREF)lParam;
		/*OperPallette();*/
		Invalidate(FALSE);
		break;
	case MK_GETROOT:
		/*ret = (LONG)(char*)m_home.operator LPCTSTR();*/
		break;
		
	default:
		;
	}
	
	return ret;
}

void CScreenWnd::_DoGetPartnerManger(BOOL _bAdvise)
{
	if(_bAdvise)
	{
		m_pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
		if(m_pPtSvrMng)
		{
			m_pPtMng = m_pPtSvrMng->AdviseManager(GetParent()->GetSafeHwnd(), m_hWnd);
		}
	}
	else
	{
		if(m_pPtSvrMng)
		{
			m_pPtSvrMng->UnAdviseManager(GetParent()->GetSafeHwnd(), m_hWnd);
			m_pPtSvrMng = NULL; m_pPtMng=NULL;
		}
	}
}