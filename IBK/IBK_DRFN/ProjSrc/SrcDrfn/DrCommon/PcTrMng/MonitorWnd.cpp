// GwanWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcTrMng.h"
#include "MonitorWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMonitorWnd

CMonitorWnd::CMonitorWnd()
{
	m_hATHwnd = NULL;
	m_nWaitResult = MONITER_WRET_READY;
}

CMonitorWnd::~CMonitorWnd()
{
}


BEGIN_MESSAGE_MAP(CMonitorWnd, CWnd)
	//{{AFX_MSG_MAP(CMonitorWnd)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMonitorWnd::OpenWindow()
{
	CString		szClassName;	
	
	szClassName =	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		0, 0, 0);
	int rc = CreateEx(0L,														//dwExStyle, 
					(LPCSTR)szClassName,
					"",
					WS_VISIBLE,	// | WS_CHILD, 
					0, 0, 0, 0, 
					HWND_DESKTOP, 
					NULL,
					NULL);
	if(rc) {
		ShowWindow(SW_HIDE);
	}
	return rc;

}

void CMonitorWnd::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
}

HWND CMonitorWnd::GetHandle()
{
	m_hATHwnd = (HWND)theApp.m_StateMng.m_dwInterface[RunModule_AutoTrade];
	return m_hATHwnd;
}


int CMonitorWnd::WaitResult()
{
	m_nWaitResult = MONITER_WRET_READY;
	DWORD dwStartTime = GetCurrentTime();	
	DWORD dwTimeOut = 1000*30;	// TimeOut : 30��
	MSG		msg;
	while( (GetCurrentTime() - dwStartTime) < dwTimeOut )
	{
		m_hATHwnd = GetHandle();
		if(m_hATHwnd)
		{
			m_nWaitResult = MONITER_WRET_RECEIVE;
			return m_nWaitResult;
		}

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE )) 
		{
			if (!m_hWnd || msg.message == WM_QUIT || msg.message == WM_CLOSE) 
			{
    			PostQuitMessage(msg.wParam) ;
				m_nWaitResult = MONITER_WRET_QUIT;
				return m_nWaitResult;
       		}

   			TranslateMessage( &msg );
   			DispatchMessage( &msg );
	    }
	}
	m_nWaitResult = MONITER_WRET_TIMEOUT;		// �ð��ʰ� 
	return m_nWaitResult;
}

/////////////////////////////////////////////////////////////////////////////
// CTRHelperWnd
CTRHelperWnd::CTRHelperWnd()
{
	m_nSysEde = -1;	// ���� ������ ����, 1��������, 0������������.
}

CTRHelperWnd::~CTRHelperWnd()
{
}


BEGIN_MESSAGE_MAP(CTRHelperWnd, CWnd)
	//{{AFX_MSG_MAP(CTRHelperWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
//@�ſ�����	ON_MESSAGE(WM_RP_RECEIVED,	OnGetData )	// Tran�� Fid����� ������ ��ȸ ó��
END_MESSAGE_MAP()

BOOL CTRHelperWnd::OpenWindow()
{
	CString		szClassName;	
	
	szClassName =	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		0, 0, 0);

	if (!CWnd::CreateEx(WS_EX_TOPMOST, szClassName, _T(""), 
		WS_POPUP, 
		0, 0, 0, 0, // size & position updated when needed
		NULL, 0, NULL))
	{
		return FALSE;
	}
	ShowWindow(SW_HIDE);
	return TRUE;
}

void CTRHelperWnd::OnDestroy() 
{
	RemoveList();
	CWnd::OnDestroy();
}

#include "../CommonTR/SS_OrderDef.h"
//@�ſ�����#include "../../../../Controls/CommApi/Commexport.h"
// �ý���Ʈ���̵� �������ο� ���� ������ ������
int CTRHelperWnd::SendV840Data()
{
//@��������	
/*	
	CString szUserID;
	szUserID = theApp.m_MainInfoMng.GetUserID();	//"BANGLE2815";

	CCommOptionInfo	option;

	//option.m_strTrCode   = "HFIBOA0V8400";	// tr�� �̸�
	//option.m_Destination = '2';				// ���弭���� ���� Packet�� "2"�� Setting�Ѵ�.
	option.m_strTrCode   = "hfca_oa_104aq05";	// tr�� �̸�
	option.m_Destination = 'H';					// Hä�ΰ�����
	//int nSize = sizeof(fn_HFIBOA0V8400_InRec1) - 33 + strTotal.GetLength();
	option.m_strScreenCode = "1401";	// HTS�϶��� �ý���Ʈ���̵� �̼����θ� üũ�Ѵ�.
										// HTS�϶��� ������Ʈ ȭ���ȣ�� 1401
	int nSize = sizeof(fn_HFIBOA0V8400_InRec1);

	char * pBuf = new char[ nSize ];
	memset(pBuf, ' ', nSize);
//	memcpy( pBuf, (LPSTR)(LPCTSTR)szUserID, 
//			(10 > szUserID.GetLength()? szUserID.GetLength() : 10));
	memcpy( pBuf, (LPSTR)(LPCTSTR)szUserID, 
			(16 > szUserID.GetLength()? szUserID.GetLength() : 16));
	int nRqID = CommRequestDataOption(GetSafeHwnd(), (BYTE*)pBuf , nSize, (BYTE*)&option);

	return nRqID;
*/
	return 1;
}

long CTRHelperWnd::OnGetData(WPARAM wParam, LPARAM lParam)
{
//@��������
/*	
	CRpDataParser rpParser;
	rpParser.OnGetData ( wParam, lParam );
	CString strTranCode = rpParser.GetTranCode();
	strTranCode.TrimLeft(); strTranCode.TrimRight();

	//if(strTranCode.Compare("HFIBOA0V8400")==0)
	if(strTranCode.Compare("hfca_oa_104aq05")==0)	
	{
		if(  !rpParser.IsError() )
		{
			fn_HFIBOA0V8400_OutRec1 *pOutRec1;
			pOutRec1 = (fn_HFIBOA0V8400_OutRec1 *) rpParser.GetData();
			TRACE("�ý���Ʈ���̵� �������� %d\n", pOutRec1->OutItem1);
			if(pOutRec1->OutItem1[0]=='Y')	m_nSysEde=1;
			else	m_nSysEde=0;

			CString szTmp; szTmp.Format("%d", m_nSysEde);
			::SetEnvironmentVariable(SZFNPROMAX_SYSEDU, szTmp);
			theApp.m_CompanyMng.m_nSysEdu = m_nSysEde;
			NotifySysEdu(m_nSysEde);
			RemoveList();
		}
		else
		{
			TRACE("%s\n", "�ý��۱��� ���θ� �� �� �����ϴ�. ȭ���� �ٽ� �����ֽñ� �ٶ��ϴ�.");
			// AfxMessageBox("�ý��۱��� ���θ� �� �� �����ϴ�. ȭ���� �ٽ� �����ֽñ� �ٶ��ϴ�.\n");
			CString strMsgCode = rpParser.GetReciveCode();
			CString strMsg = CommGetMsgString(strMsgCode);
			// ����

			m_nSysEde = -1;
			theApp.m_CompanyMng.m_nSysEdu = m_nSysEde;
			CString szTmp; szTmp.Format("%d", m_nSysEde);
			::SetEnvironmentVariable(SZFNPROMAX_SYSEDU, szTmp);
			NotifySysEdu(m_nSysEde);
			return 1L;
		}
	}
*/
	return 0L;
}

void CTRHelperWnd::RemoveList()
{
	for(POSITION pos=m_ListCallback.GetHeadPosition(); pos;) 
	{
		STSysCallProcItem* pItem = m_ListCallback.GetNext(pos);
		if(pItem)
		{
			delete pItem;
		}
	}
	m_ListCallback.RemoveAll();
}

void CTRHelperWnd::RemoveSysEdu(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc)
{
	long dwCallback = (long)pCallFunc;
	POSITION tPos = NULL;
	STSysCallProcItem* pItem = NULL;

	for(POSITION pos=m_ListCallback.GetHeadPosition(); pos;) 
	{
		tPos = pos;
		pItem = m_ListCallback.GetNext(pos);
		if(pItem)
		{
			if(pItem->dwNotifyData==dwCallback) 
			{
				delete pItem;
				m_ListCallback.RemoveAt(tPos);
				break;
			}
		}
	}
}

void CTRHelperWnd::NotifySysEdu(int p_nSysEdu)
{
	POSITION tPos = NULL;
	STSysCallProcItem* pItem = NULL;
	SysEduCallbackProc pCallFunc;

	for(POSITION pos=m_ListCallback.GetHeadPosition(); pos;) 
	{
		pItem = m_ListCallback.GetNext(pos);
		if(pItem)
		{
			pCallFunc = (SysEduCallbackProc)pItem->dwNotifyData;
			pCallFunc(pItem->pThisPoint, pItem->szMainKey, pItem->dwSubKey, p_nSysEdu);
		}
	}
}


