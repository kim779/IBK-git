 // ViewWrap.cpp : implementation file
//

#include "stdafx.h"
#include "ViewWrap.h"

#include <afxpriv.h>
#include "TrMChartView.h"
#include "../../inc/RMsgUserInfo.h"
//#include "../../../SrcSite/include/InterestInfo.h"
#include "../../../SrcSite/include/define.h"

#include "SI050000.h"

#include "../../chart/COMMONTR/TRDefine.h"
#include "../../inc/IAUTrCommMng.h"
#include "../../inc/IMainInfoMng.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		WM_DLLDATA		(WM_USER+9111)	//���н� ���ο��� ��������ؼ� ������ �޽���.

CString		Glb_SubTrCode;
//HWND		Glb_hFormMngCtrl = NULL;
//IDispatch*	Glb_ScrFormDisp = NULL;

// extern "C"
// {
// 	__declspec(dllexport) int OnCodeDragAndDropProc(CWnd* pViewWnd, LPCTSTR strCode ); 
// 	// M*Nȭ�� ��ȸ���� ���� �� �ε� ó��
// 	__declspec(dllexport) void GetLastData(CWnd* pViewWnd, LPCTSTR strPageScrNo, LPCTSTR strPageIndex, CString& strData); 
// 	__declspec(dllexport) void SetLastData(CWnd* pViewWnd, LPCTSTR strPageScrNo, LPCTSTR strPageIndex, LPCTSTR strData); 
// };
// 
// int OnCodeDragAndDropProc(CWnd* pViewWnd, LPCTSTR strCode )
// {
// 	((CTrMChartView*)pViewWnd)->SetCodeDragAndDrop(strCode);
// 	return FALSE;
// }
// 
// void GetLastData(CWnd* pViewWnd, LPCTSTR strPageScrNo, LPCTSTR strPageIndex, CString& strData)
// {
// 	if(pViewWnd)
// 	{
// 		((CTrMChartView*)pViewWnd)->GetLastData(strPageScrNo, strPageIndex, strData);
// 	}
// }
// 
// void SetLastData(CWnd* pViewWnd, LPCTSTR strPageScrNo, LPCTSTR strPageIndex, LPCTSTR strData)
// {
// 	if(pViewWnd)
// 	{
// 		((CTrMChartView*)pViewWnd)->SetLastData(strPageScrNo, strPageIndex, strData);
// 	}
// }

// 	STDAPI CreateScreen( CWnd** ppWrapWnd, CWnd** ppScrWnd, CWnd* pParent,
// 						 LPCTSTR strSubTRCode, IDispatch * FormDisp, LPCTSTR strMapFileName, HWND hFormMng, CMapStringToOb * ArrCtrlName, long lBkColor, int nAddView)			 					 
// 	{
// 		Glb_SubTrCode = strSubTRCode;
// 		//Glb_MapFileName = strMapFileName;
// 		Glb_hFormMngCtrl = hFormMng;
// 		Glb_ScrFormDisp = FormDisp;
// 		
// 		CViewWrap* pWrapWnd = new CViewWrap();
// 		
// 		CRect rect;
// 		pParent->GetWindowRect( &rect );
// 		pParent->ScreenToClient( &rect );
// 
// 		if( !pWrapWnd->Create( NULL, NULL, WS_CHILD|WS_VISIBLE, rect, pParent, 999 ) )
// 			return E_FAIL;
// 
// 		// �߰� �Ѵ�.
// 		if(nAddView == 1)
// 			pWrapWnd->AddViewForSingle();
// 
// 		*ppWrapWnd = pWrapWnd;
// 		*ppScrWnd = pWrapWnd->m_pView;
// 
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_hFormMng = hFormMng;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_lBkColor = lBkColor;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_MapControlName = ArrCtrlName;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_strScreenNo = strSubTRCode;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_ScrFormDisp = FormDisp;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_strMapFileName = strMapFileName;
// 		((CTrMChartView*)pWrapWnd->m_pView)->m_pFormWnd = pParent;
// 
// 		return NOERROR;
// 	}


// 	STDAPI DestroyScreen( CWnd* pWrapWnd )
// 	{
// 		CViewWrap* pDelWnd = (CViewWrap*)pWrapWnd;
// 		pDelWnd->DestroyWindow();
// 		delete pDelWnd;
// 
// 		return NOERROR;
// 	}

const UINT RMSG_SENDMESSAGE = ::RegisterWindowMessage(_T("RMSG_SENDMESSAGE"));


/////////////////////////////////////////////////////////////////////////////
// CViewWrap

CViewWrap::CViewWrap()
{
	m_pView = NULL;

	m_pPtSvrMng = NULL;
	m_pPtMng = NULL;
}

CViewWrap::~CViewWrap()
{
	_DoClearSendItem();
}


BEGIN_MESSAGE_MAP(CViewWrap, CWnd)
	//{{AFX_MSG_MAP(CViewWrap)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_DLLDATA, OnWinixDllData)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_SENDMESSAGE, OnRmsgSendMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CViewWrap message handlers

int CViewWrap::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	_DoGetPartnerManger();

	if (theApp.m_szDllName.Find(_T("0510")) >= 0)
	{
		/*
		struct	�ý���Ʈ���̵� ���ǿ��� TR Name : pidosyst
		struct  mid     {
				char    mode[1];        // 1:��ȸ 2:����                
				char    syst[1];        // ���ǿ��� 1:����, 0:�̵���    
		};
		struct  mod     {
				char    syst[1];        // ���ǿ��� 1:����, 0:�̵���    
				char    sflg[1];        // OK '0' else ERROR            
				char    emsg[99];
		}; 
		*/
		_DoDirectSendTR(TRNO_SYSTEMAGREE, "11", 2, TRKEY_SYSTEMAGREE);

		SetTimer(510, 5000, NULL);
	}
	else
	{
		// TODO: Add your specialized creation code here
		if( !CreateView( RUNTIME_CLASS( CTrMChartView ) ) )
			return -1;
	}

	return 0;
}

void CViewWrap::OnDestroy() 
{
	_DoGetPartnerManger(FALSE);
	CWnd::OnDestroy();
}

void CViewWrap::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if( m_pView )
	{
		m_pView->SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER );
	}
}

typedef	CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent);

BOOL CViewWrap::CreateView( CRuntimeClass * pViewClass )
{
    //CRect rect, client;
    ASSERT(pViewClass );

    CWnd *pWnd=(CWnd *)pViewClass->CreateObject();
    if (!pWnd) return FALSE;	

	CTrMChartView *pTRView = (CTrMChartView*)pWnd;

	//100530:����ȭ�� �ٸ��� ����/�б�
	pTRView->m_szFileKey = m_szFileKey;

	int dwStyle = AFX_WS_DEFAULT_VIEW;
    dwStyle&=~WS_BORDER;
	dwStyle&=~WS_VISIBLE;

    if( !pWnd->Create(NULL, NULL, dwStyle, CRect(0,0,0,0), this, 999, NULL ) )
    {
        TRACE0("Warning: couldn't create client area for tab view\n");
        // pWnd will be cleaned up by PostNcDestroy
		return FALSE;
    }

	WPARAM wp = MAKEWPARAM(USERINFO_SET, 0);
	if(pTRView->m_pChartScreen && IsWindow(pTRView->m_pChartScreen->GetSafeHwnd()))
		pTRView->m_pChartScreen->SendMessage(RMSG_USERINFO, wp, (LPARAM)m_pPtMng);

	//pWnd->ModifyStyleEx( 0, WS_EX_CLIENTEDGE);

	CRect rect;
	GetWindowRect( &rect );
	ScreenToClient( &rect );	
	
	pWnd->MoveWindow( rect );
	
	m_pView = (CView*)pWnd;
	pWnd->ShowWindow( SW_SHOW );
//}

    return TRUE;
}

// void CViewWrap::AddViewForSingle( )
// {
// 	CDocument* pDoc = ((CView*)GetParent()->GetParent())->GetDocument();
// 	pDoc->AddView((CView*)m_pView);
// }

BOOL CViewWrap::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

void CViewWrap::_DoGetPartnerManger(BOOL _bAdvise)
{
	if(_bAdvise)
	{
		m_pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
		if(m_pPtSvrMng)
		{
			m_pPtMng = m_pPtSvrMng->AdviseManager(GetParent()->GetSafeHwnd(), m_hWnd);
//			{
//				CString szDebug;
//				szDebug.Format("ViewWrap [%08X], [%08X]\n", m_hWnd, m_pPtMng);
//				OutputDebugString(szDebug);
//			}
		}
	}
	else
	{
		if(m_pPtSvrMng)
		{
			m_pPtSvrMng->UnAdviseManager(GetParent()->GetSafeHwnd(), m_hWnd);
			m_pPtSvrMng = NULL;
			m_pPtMng = NULL;
		}
	}
}

int CViewWrap::_DoParsingSendData(CString &szData, CSendItem &Info)
{
	int nReceiveLen = szData.GetLength();
	if(nReceiveLen==0) return 0;

	int nPos1 = 0, nOffset=0;
	CString szTmp, szCode, szCodeName;

	CSendItem SendItem;
	int nDataLen = szData.GetLength();
	//while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szData.Find('\t', nOffset);
		if(nPos1<0) return 0;
		SendItem.m_szTR = szData.Left(nPos1);

		//-----------------------------------
		nOffset = nPos1+1;
		nPos1 = szData.Find('\t', nOffset);
		if(nPos1<0) return 0;
		szTmp = szData.Mid(nOffset, nPos1-nOffset);
		sscanf(szTmp, "%08X", &SendItem.m_pCallback);

		//-----------------------------------
		nOffset = nPos1+1;
		nPos1 = szData.Find('\t', nOffset);
		if(nPos1<0) return 0;
		szTmp = szData.Mid(nOffset, nPos1-nOffset);
		SendItem.m_nKey = atoi(szTmp);

		//-----------------------------------
		nOffset = nPos1+1;
		nPos1 = szData.Find('\t', nOffset);
		if(nPos1<0) return 0;
		szTmp = szData.Mid(nOffset, nPos1-nOffset);
		SendItem.m_nLen = atoi(szTmp);

		//-----------------------------------
		nOffset = nPos1+1;
		//SendItem.m_pData = (LPBYTE)(LPCSTR)szData.Mid(nOffset);

		_DoListSendItem(TRUE, &SendItem);
		Info.Copy(&SendItem);
	}
	return nOffset;
}

int CViewWrap::_DoListSendItem(BOOL bAdd, CSendItem* _pItem)
{
	if(bAdd)
	{
		for(POSITION pos=m_SIList.GetHeadPosition(); pos;)
		{
			CSendItem* pItem = m_SIList.GetNext(pos);
			if(pItem->m_nKey==_pItem->m_nKey)
			{
				pItem->Copy(_pItem);
				return 0;
			}
		}
		CSendItem *pItem = new CSendItem;
		pItem->Copy(_pItem);
		m_SIList.AddTail(pItem);
		return 1;
	}
	else
	{
		POSITION tpos;
		for(POSITION pos=m_SIList.GetHeadPosition(); pos;)
		{
			tpos = pos;
			CSendItem* pItem = m_SIList.GetNext(pos);
			if(pItem->m_nKey==_pItem->m_nKey)
			{
				delete pItem;
				m_SIList.RemoveAt(tpos);
				return -1;
			}
		}
		return 0;	//����Ʈ�� ����..��������.
	}
	return 0;
}

void CViewWrap::_DoClearSendItem()
{
	for(POSITION pos=m_SIList.GetHeadPosition(); pos;)
	{
		CSendItem* pItem = m_SIList.GetNext(pos);
		delete pItem;
	}
	m_SIList.RemoveAll();
}

CSendItem* CViewWrap::FindSendItem(int nKey)
{
	for(POSITION pos=m_SIList.GetHeadPosition(); pos;)
	{
		CSendItem* pItem = m_SIList.GetNext(pos);
		if(pItem->m_nKey==nKey) return pItem;
	}
	return NULL;
}

BOOL CViewWrap::_DoDirectSendTR(CString strTR, char *szData, int nLen, int nKey)
{
	char *pszBuff = NULL;

	struct _userTH user;
	strncpy(&user.trc[0], strTR, 8);

	//<<20100319_JS.Kim FX TR�� secondary session���� ��û�Ѵ�.
	//user.path = ssMASTER;
//KHD
//	if( strTR.Left(2) == TRMODE_FX )
//		user.path = ssSEC1;
//	else
//		user.path = ssMASTER;
//END
	//>>
	user.key  = nKey;
	user.stat = US_KEY;
//KHD	user.size = nLen;

	//@100118-alzioyes
	int nSize = sizeof(_userTH) + nLen;
	pszBuff = new char[nSize+1];

	memcpy(pszBuff, (char*)&user, sizeof(_userTH));
	memcpy(pszBuff+sizeof(_userTH), szData, nLen);
//	strTRData = CString((char*)&user, sizeof(_userTH));
//	strTRData += CString(szData, nLen);

	BOOL bRet = ::SendMessage(GetParent()->GetSafeHwnd(), WM_USER, MAKEWPARAM(invokeTRx, nLen), (LPARAM)pszBuff);

	delete []pszBuff;
	pszBuff = NULL;

	return bRet;
}

LONG CViewWrap::OnRmsgSendMessage(WPARAM wParam, LPARAM lParam)
{
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch(bt)
	{
		case 1:
			{
				CString szData = (LPCSTR)lParam;
				CSendItem SendInfo;
				int nRet = _DoParsingSendData(szData, SendInfo);
				if(nRet>0 && SendInfo.m_nLen>0)
				{
//					_DoDirectSendTR(SendInfo.m_szTR, (LPSTR)(LPCSTR)szData.Mid(nRet), SendInfo.m_nLen, SendInfo.m_nKey);
					_DoDirectSendTR(SendInfo.m_szTR, (char*)(lParam+nRet), SendInfo.m_nLen, SendInfo.m_nKey);
				}
			}
			break;
	}
	return 0L;
}

#include "../../inc/IDebugMng.h"
struct _trkey
{
	UINT kind:5;
	UINT group:3;
};
IDebugManager* g_pDebugMng = NULL;
LONG CViewWrap::OnUser(WPARAM wParam, LPARAM lParam)
{
	int nKey = 0;
	int nLen = 0;
	LONG	ret = 0;
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch(bt)
	{
	case DLL_OUBx:
		{
#if(0)
			char* pBytes = (char *)lParam;
			struct _extTH* extTH = (struct _extTH *)pBytes;
			pBytes += L_extTH;

			int nKey = extTH->key;
			int nLen = extTH->size;

			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
			lParam = (LPARAM)pBytes;
#else
			_extTHx* info = (_extTHx*)lParam;
			const int nLen = info->size;
			char* pBytes = info->data;
			int nKey = info->key;

			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
			lParam = (LPARAM)pBytes;
#endif
		}

	case DLL_OUB:			// Tr����		
		{
			WORD kind = HIBYTE(LOWORD(wParam));
			struct _trkey* trkey = (struct _trkey*)&kind;
			nKey = kind;
			nLen = (long)HIWORD(wParam);
			
			if(nLen==1)
			{
				wParam = MAKEWPARAM(LOWORD(wParam), HIWORD(wParam) - SZ_ZCOMHEAD);
				nLen = HIWORD(wParam);
			}
			if(!g_pDebugMng)
				g_pDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
			if(g_pDebugMng)
				g_pDebugMng->TraceTran('O', "OnUser", nLen, (LPBYTE)(lParam+sizeof(_zcom_head)));
			
			LPBYTE pRBuff = new BYTE[nLen+1];
			if( nKey == 2 )
			{
				// MSTCode�϶��� ��Ŷ ����� Ŀ�� ���н� �������� �����ؼ� �����Ѵ�.
				// �׷��� ���ο��� ��Ŷ�� ������ ȭ�鿡 �����ϴµ� �̶� ��Ŷ ������� �������� ���� ��Ŷ�������
				// �����ؼ� ���� ���� ��Ŷ������� ���̰� �߻��Ѵ�. �׷��� ���� �޾Ƽ� ó���ϴ� �κп���
				// ����� �ٽ� ������� ó���� �Ѵ�.
				if(m_pPtMng) m_pPtMng->ReceiveTRData("", (LPBYTE)(lParam), nLen, nKey);
				delete [] pRBuff;
				((CTrMChartView*)m_pView)->m_IsLoading = FALSE;

				return 1L;
			}
			else if(nKey ==50 || nKey == 24/* �ɺ��ڵ�(POOP) */ || (nKey > 70 &&nKey <80))// KHD : ȣ���� PooPPoop�̱⶧���� zComHeader�� ���� �ʴ´�. 71~ 79 �ý��� Ʈ���̵� Key�� 
			{
				memcpy(pRBuff, (char*)(lParam), nLen);
				pRBuff[nLen] = 0;
			}
			else
			{
				int zcomSize = sizeof(_zcom_head);
				
				if(zcomSize < nLen)
				{
					memset(pRBuff,0x00, sizeof(pRBuff));
					memcpy(pRBuff, (char*)lParam + SZ_ZCOMHEAD, nLen - zcomSize); //KHD : _zcom_head ������ �ʴµ� ������� �Ѵ�. 
					pRBuff[nLen- zcomSize] = 0;
				}
				else 
				{
					delete []pRBuff;
					break;
				}
			}
			
				
			if (nKey == TRKEY_SYSTEMAGREE)
			{
				KillTimer(510);

				if (nLen <= 0)
					CloseScreen();

				char *pszData = (char*)(lParam + SZ_ZCOMHEAD);

				if ((pszData[1] == '0') && (pszData[0] != '1'))
				{
					if (m_pPtSvrMng)
					{
						IPartnerUtilManager* pPtUtilMng = (IPartnerUtilManager*)m_pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);
						if (pPtUtilMng)
							pPtUtilMng->OpenScreen(GetParent()->GetSafeHwnd(), _T("SI051090"));
					}
					CloseScreen();
					delete [] pRBuff;
					break;
				}

				if (!CreateView(RUNTIME_CLASS(CTrMChartView)))
					CloseScreen();

				delete [] pRBuff;
				break;
			}

			//TreeListMng.dll ������ �������� ����ó��
			if(nKey==TRKEY_CONDITION || (TRKEY_GWANSIMGROUP<=nKey && nKey<=TRKEY_GWANSAVE) || nKey == TRKEY_ACCOUNTINFO)
			{
				CString strTR;
				switch(nKey)
				{
					case TRKEY_CONDITION:	strTR = TRNO_CONDIONECLK_RESULT;	break;
				}
				CSendItem* pItem = FindSendItem(nKey);
				if(pItem && pItem->m_pCallback)
				{
					ITrCommSite* pSite = (ITrCommSite*)pItem->m_pCallback;
					if(pSite)
						pSite->ReceiveData(nKey, strTR, "", "", (char*)lParam, nLen);
					delete [] pRBuff;
					return 1L;
				}
			}
			else if( nKey == ACC_REV_KEY1 || nKey == ACC_REV_KEY2 )	// ������Ʈ�ѿ�
			{
				const UINT RMSG_ACCOUNT_INIT = ::RegisterWindowMessage(_T("RMSG_ACCOUNT_INIT"));
				CTrMChartView *pTRView = (CTrMChartView*)m_pView;
				if(pTRView->m_pChartScreen && IsWindow(pTRView->m_pChartScreen->GetSafeHwnd()))
					pTRView->m_pChartScreen->SendMessage(RMSG_ACCOUNT_INIT, wParam, lParam);
			}

			if(m_pPtMng) m_pPtMng->ReceiveTRData("", pRBuff, nLen, nKey);
			delete [] pRBuff;
			((CTrMChartView*)m_pView)->m_IsLoading = FALSE;
			
			return 1L;
		}
		
		break;

//-->100309 ����ó������� ����.
//	DLL_ALERT ��ſ� DLL_ALERTx ����ϴ� ������ ��ƾ ������.
//	case DLL_ALERT:			// ���� ����
//		{
//	//			{
//	//				CString szDebug;
//	//				szDebug.Format("ViewWrap [%08X], [%08X]\n", m_hWnd, m_pPtMng);
//	//				OutputDebugString(szDebug);
//	//			}
//
//@100309			if(m_pPtMng && IsWindow(m_hWnd)) m_pPtMng->ReceiveRealData(wParam, lParam);
//		}
//		break;
	case DLL_ALERTx:
		if (m_pPtMng)
		{
			// ��û�� �����ڵ��� �ǽð����� üũ.
			if (!((CTrMChartView*)m_pView)->m_IsLoading)
			{
				struct _alertR* alertR = (struct _alertR*)lParam;

				if (_DoCheckAllertCode(alertR->code))
				{
//	����������,DRFN Platform �αױ���߰�, �ʿ�ÿ��� ����ϰ� ������ ���� ���� ��]
/*
					if (!g_pDebugMng)
						g_pDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
					if (g_pDebugMng)
						g_pDebugMng->TraceTran('I', "DLL_ALERTx RealCode", alertR->code.GetLength(), (LPBYTE)(LPCSTR)(LPCTSTR)(alertR->code));
*/
					if (IsWindow(m_hWnd))
						m_pPtMng->ReceiveRealDataEx(wParam, lParam);
				}
			}
		}

		break;
//<--100309 ����ó������� ����.

	case DLL_SETPAL:
		{
			const UINT RMSG_CHANGE_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
			CTrMChartView *pTRView = (CTrMChartView*)m_pView;
			pTRView->m_pChartScreen->PostMessage(RMSG_CHANGE_SKINCOLOR, 0, 0);
		}
		break;
	case DLL_DOMINO:		// ȭ�� �����鼭 ������ ���� ���� ��
		if (m_pPtMng && IsWindow(m_hWnd))
		{
			IPartnerSite* pSite = m_pPtMng->GetSite();
			if (pSite) pSite->ReceiveOnUser(wParam, lParam);
		}

// 		{
// 			CString szData((char*)lParam), strTemp, strTemp2, strKey, strValue;
// 			CMapStringToString strMap;
// 			strTemp = szData;
// 
// 			int nFind = strTemp.Find(_T("\n"));
// 			while (nFind>=0)
// 			{
// 				strTemp2 = strTemp.Left(nFind);
// 				strTemp = strTemp.Mid(nFind+1);
// 				nFind = strTemp2.Find(_T("\t"));
// 				if (nFind >= 0)
// 				{
// 					strKey = strTemp2.Left(nFind);
// 					strValue = strTemp2.Mid(nFind+1);
// 					strMap.SetAt(strKey, strValue);
// 
// 					if (strKey[0]>='1' && strKey[0]<='9')
// 					{
// 						int nKey = IPartnerSite::IPS_RK_shareCode;
// 						if (m_pPtMng && IsWindow(m_hWnd))
// 						{
// 							if (strKey.Find(_T("301")) > 0)
// 							{
// 								IPartnerSite* pSite = m_pPtMng->GetSite();
// 								if(pSite) pSite->RecieveKeyData(nKey, strTemp2);
// 							}
// 						}
// 					}
// 				}
// 
// 				nFind = strTemp.Find(_T("\n"));
// 			}
// 		}

		break;
	case DLL_GUIDE:
		return 1;
	case DLL_SETFONTx:
		break;
	case DLL_TRIGGER:
		{
			CString szData((char*)lParam);
			int nPos = szData.Find('\t');
			if(nPos<1) return 0L;

			CString szSymbol = (LPSTR)(LPCSTR)szData.Left(nPos);
			CString szValue  = (LPSTR)(LPCSTR)szData.Mid(nPos+1);

			if( strcmp(szSymbol, "DRFNGWAN")==0) //���ɰ��� �޽����̸�
			{
				UINT nMsg = atoi((LPCSTR)szValue);
				switch (nMsg)
				{
				case WM_NOTIFY_LOAD:					// ���ɱ׷� Load (��ü)
					break;
				case WM_NOTIFY_SAVE:					// ���ɱ׷� Save (��ü or 1�׷�)
					{
						int nKey = IPartnerSite::IPS_RK_gwansim;
						if(m_pPtMng && IsWindow(m_hWnd)) 
						{
							IPartnerSite* pSite = m_pPtMng->GetSite();
							if(pSite) pSite->RecieveKeyData(nKey, szData);
						}
					}
					break;
				case WM_NOTIFY_GROUP:					// ���ɱ׷� Load (1�׷�)
					break;
				case WM_NOTIFY_GROUPLIST:				// ���ɱ׷� Load (�׷츮��Ʈ - ���� ����)
					break;
				case WM_NOTIFY_SAVECODE:				// ���ɱ׷쿡 ���� �߰� (1����)
					break;
				}
			}
			else if(szSymbol[0]>='1' && szSymbol[0]<='9')
			{
				int nKey = IPartnerSite::IPS_RK_shareCode;
				if(m_pPtMng && IsWindow(m_hWnd)) 
				{
					if( strstr(szData, "1301\t")==0  ||
						strstr(szData, "20301\t")==0 ||
						strstr(szData, "30301\t")==0 ||
						strstr(szData, "50301\t")==0)
					{
						IPartnerSite* pSite = m_pPtMng->GetSite();
						if(pSite) pSite->RecieveKeyData(nKey, szData);
					}
				}
			}
			else if(szSymbol[0]>='c' && szSymbol[0]<='o')
			{
				int nKey = IPartnerSite::IPS_RK_shareCode;
				if(m_pPtMng && IsWindow(m_hWnd)) 
				{
					if( strstr(szData, "focod\t")==0 ||
						strstr(szData, "fcod\t")==0 ||
						strstr(szData, "cfcod\t")==0 ||
						strstr(szData, "ocod\t")==0 ||
						strstr(szData, "gscode\t")==0)
					{
						IPartnerSite* pSite = m_pPtMng->GetSite();
						if(pSite) 
							pSite->RecieveKeyData(nKey, szData);
					}
				}
			}

		}
		break;
	case DLL_CODE: // ������̾�α׿��� ������ �ڵ�
		if(m_pPtMng && IsWindow(m_hWnd)) 
		{
			int nKey = IPartnerSite::IPS_RK_codeDlg;
			IPartnerSite* pSite = m_pPtMng->GetSite();
			if(pSite) pSite->RecieveKeyData(nKey, (char*)lParam);
		}
		break;

	case DLL_NOTICE:	// ü�ᵥ��Ÿ. �ؿܼ���(8041)�� FX(9041)������ ó���Ѵ�.
		{
		CTrMChartView *pTRView = (CTrMChartView*)m_pView;
		if( pTRView )
		{
			CString strFormCode = pTRView->m_strFormCode;
			if( strFormCode == "8041" || strFormCode == "9041" )
			{
				if(m_pPtMng && IsWindow(m_hWnd)) m_pPtMng->ReceiveNoticeData((WPARAM)strFormCode.operator LPCTSTR(), lParam);
			}
			}
		}
		break;
	case DLL_INIT :		// Init ����.. ������Ʈ�� Init���ش�.
		{
		const UINT RMSG_ACCOUNT_INIT = ::RegisterWindowMessage(_T("RMSG_ACCOUNT_INIT"));
		CTrMChartView *pTRView = (CTrMChartView*)m_pView;
		if(pTRView && IsWindow(pTRView->GetSafeHwnd()) && pTRView->m_pChartScreen && IsWindow(pTRView->m_pChartScreen->GetSafeHwnd()))
			pTRView->m_pChartScreen->SendMessage(RMSG_ACCOUNT_INIT, 0, 0);
		}
		break;

	}

	return 0L;
}

//100530:����ȭ�� �ٸ��� ����/�б�-->
// --------------------------------------------------------------------------------------
// [100512�� winix�� ���м����忡�� ���� ����.]
// [����] �� �뺸 �޼��� �� Ʈ���Ŵ� DLLȭ���� ��쿡�� �߻��մϴ�.
// 
//   1. ����ȭ�� �Ǵ�, ����� ȭ�� ���� ��, �Ʒ� �޼��� �߻�
//   
// 	#define WM_DLLDATA    WM_USER + 9111
// 	SendMessage(WM_DLLDATA, 0, lParam);
// 	
// 	  lParam = USERDEFDATA\t����,����ȭ���ȣ,����ȭ��/����ȭ�鱸��,�Ϸù�ȣ
// 	  (1) ����(LASTSTAT : ���� ����ȭ��, USERSCREEN : ����� ȭ��)
// 	  (2) ����ȭ���ȣ
// 	  (3) ����ȭ��/����ȭ�� ����(0 : ����ȭ��, 1 : ����ȭ��)
// 	  (4) �Ϸù�ȣ
// 	  
// 		2. "WM_DLLDATA" �޼��� Event Handler���� ȯ�� ���� ���� ����
// 		(1) $UID ����
// 		lParam�� �����Ͽ� �ش� ȭ�麰�� �ĺ������� Unique�� ���ϸ��� �����.
// 		
// 		  (2) ���� ���ϸ��� �����Ѵ�.
// 		  - ����ȭ�� : $HOME/user/LASTSTAT/$UID.INI
// 		  - �����ȭ�� : $HOME/user/USERSCREEN/$UID.INI
// 		  
// 			(3) ȯ�� ���� ������ INI ���Ͽ� �����Ѵ�.
// 			
// 			  3. ȭ�� ���� ��, ���̳� �����ͷ� [2��] lParam�� ���� �������� ���� ��.
// 			  (1) ���̳� ������ : USERDEFDATA\t����,����ȭ���ȣ,����ȭ��/����ȭ�鱸��,�Ϸù�ȣ
// 			  (2) ������ �����ؼ� ���� ��, ����� Unique�� ���ϸ��� �����.
// 			  (3) ���� ��ġ���� �ش� ȯ�� ������ �о� ȯ�漳���� �Ѵ�.
// 			  
// 				4. ���� ������ ���ο����� ����
// 				(1) ���� ȭ���� ���� ���, $HOME/user/LASTSTAT ������ ���� �Ѵ�.
//  (2) ����� ȭ���� ���� �Ǵ� ���, $HOME/user/USERSCREEN ������ ���� �Ѵ�.
// --------------------------------------------------------------------------------------
void CViewWrap::SetStartInfo(void* pData)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CViewWrap::SetStartInfo: Begin\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

// 	  (1) ����(LASTSTAT : ���� ����ȭ��, USERSCREEN : ����� ȭ��)
// 	  (2) ����ȭ���ȣ
// 	  (3) ����ȭ��/����ȭ�� ����(0 : ����ȭ��, 1 : ����ȭ��)
// 	  (4) �Ϸù�ȣ
	CString strIs; 
	strIs = (char*)pData;
	//OutputDebugString("KHD : " + strIs);
	CString szParam = (char*)pData;
	if(strIs.Find("USERSCREEN") < 0 && strIs.Find("LASTSTAT") < 0)
	{
		if(strIs.Find("1301") < 0  &&														 //�ֽ�			
			strIs.Find("gicode")   && strIs.Find("20301") < 0 &&								 //����
			strIs.Find("ed_fcode") && strIs.Find("30301") < 0 && strIs.Find("40301") < 0  && //�����ɼ�
			strIs.Find("gscode")   &&														 //�ؿ�
			strIs.Find("ed_fcod") < 0 && strIs.Find("100301") < 0 &&					  	 //��ǰ����
			 strIs.Find("ed_focod") ) //??
		{
			//return;
			szParam = "";
		}
		else
		{
			if(strIs.Find("\n") > -1)
			{
				int nIndex = strIs.Find("\n");
				
				CString strDomino = strIs.Mid(0,nIndex);
				
				szParam = strDomino;
			}
			else if(strIs.Find("\t") > -1)
			{				
				CString strDomino = strIs;
				
				szParam = strDomino;
			}
		}
	//	szParam = "";
	}

	m_szStartInfo = (char*)pData;
//	OutputDebugString("KHD : End : " + m_szStartInfo );
	
//	int nPos = szInfo.Find('\t');
//	if(nPos<0) return;
//	szInfo = szInfo.Mid(nPos+1);
//	char aParam1[20+1], aParam2[3+1], aParam3[3+1], aParam4[3+1];
//	sscanf((LPCSTR)szInfo, "%s,%s,%s,%s", aParam1, aParam2, aParam3, aParam4);
	int nPos = szParam.Find("\t");
	if(nPos<0) return;

	szParam = szParam.Mid(nPos+1);
	nPos = szParam.Find('X');
	if(nPos == 0)
		szParam.Delete(0);

	//OutputDebugString("KHD : End2222 : " + szParam );
	//nPos = szParam.Find(",");
	//CString szSaveKey1;
	//szSaveKey1 = szParam.Left(nPos);
	m_szFileKey = szParam;
	m_szFileKey.Remove(',');

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CViewWrap::SetStartInfo: End(%s)\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}
}

#include <DIRECT.H>
LONG CViewWrap::OnWinixDllData(WPARAM wParam, LPARAM lParam)
{
//return 0L;	//���н� �÷��� �������̶� ���� �� �۾��� ����.

	if(!lParam) return 0L;

	CString szParam = (char*)lParam;
	int nPos = szParam.Find(",");
	CString szSaveKey1;
	szSaveKey1 = szParam.Left(nPos);
	szParam.Remove(',');
	
	IPartnerSite* pSite = m_pPtMng->GetSite();
	if(pSite) pSite->RecieveKeyData(IPartnerSite::IPS_RK_SaveKeyEnd, (LPCSTR)szParam);

// 		IMainInfoManager* pMainMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
// 		CString szUserDir;
// 		if(!pMainMng) return 0L;
// 
// 		szUserDir.Format("%s\\%s", pMainMng->GetEncUserDir(), szSaveKey1);
// 		::mkdir(szUserDir);
// 
// 		CString szUserIni;
// 		szUserIni.Format("%s\\%s.INI", szUserDir, szParam);
// 
// 		//::WritePrivateProfileString("A", "B", "C", szUserIni);

	return 0L;
}
//100530:����ȭ�� �ٸ��� ����/�б� <--

void CViewWrap::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 510)
	{
		KillTimer(nIDEvent);

		CloseScreen();
	}

	CWnd::OnTimer(nIDEvent);
}

void CViewWrap::CloseScreen()
{
	::PostMessage(GetParent()->GetSafeHwnd(), WM_USER, MAKEWPARAM(closeDLL, 0), (LPARAM)0);
}

// Real�����Ϳ��� ������� �ʴ� ȣ���� üũ�ؼ� ����ó���ϵ��� ��.
// ȣ��������� : �ֽ�, FO
// �ؿܴ� �ʿ����� ���� �ϴ� ����ó�� ��������.
// IMasterDataManagerLast*	g_pMasterDataMng = NULL;
// BOOL CViewWrap::_DoCheckHogaReal(WPARAM wParam, LPARAM lParam)
// {
// 	m_pPtMng->ReceiveRealDataEx(wParam, lParam);
// 	return TRUE;
// 	struct _alertR* alertR = (struct _alertR*)lParam;
// 	CString szCode = alertR->code;
// 	if( szCode.IsEmpty() )
// 	{
// 		return FALSE;
// 	}
// 
// 	if(!g_pMasterDataMng) g_pMasterDataMng = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
// 	int nMarketType = (int)g_pMasterDataMng->GetMarketTypeFromCode(szCode);
// 
// 	switch(nMarketType)
// 	{
// 		case CMasterTypeInfo::STOCK_CHART:
// 		case CMasterTypeInfo::FUOPT_JIPYO_CHART:
// 		case CMasterTypeInfo::FOREIGN_CHART:
// 		case CMasterTypeInfo::FO_FOREIGN_CHART:
// 		case CMasterTypeInfo::FX_CHART:
// 			m_pPtMng->ReceiveRealDataEx(wParam, lParam);
// 			return TRUE;
// 	}
// 
// 	CTrMChartView *pTRView = (CTrMChartView*)m_pView;
// 	return TRUE;
// }

// ���н��� ����ü��� ���� ��û���� ���� ���� ���ؼ��� �ǽð� �����͸� ������ �شٰ� ��.
// �׷��� ���� ȭ�鿡�� ���� �ɷ��� ��.
BOOL CViewWrap::_DoCheckAllertCode(CString szCode)
{
	if (szCode.Left(1) == "X" && szCode.Find('@') == -1)	//IBK���� �������� �ǽð� ���͸� �ϴ� ��ƾ!!
		return FALSE;

	if (m_pView && (((CTrMChartView*)m_pView)->m_nEnumScreenType == ST_FOREIGNCHART ||
					((CTrMChartView*)m_pView)->m_nEnumScreenType == ST_FXCHART))
	{
		return TRUE;
	}
	
	//������ ���� A�����ϰ� Ȯ��, ������ ���� ���� ������ �����Ƿ� �ٷ� ����.
	if (szCode.Left(1) == "A" || szCode.Left(1) == "Q")
		szCode = szCode.Mid(1);

//	if (szCode.Left(1) == "K")	//KHD ??
//		return TRUE;

	if (m_pPtMng->IsRegisterCode(szCode) == FALSE)
		return FALSE;

	return TRUE;
}


