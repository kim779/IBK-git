// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"
#include "MonitorWnd.h"

const UINT RMSG_SINGLEMAPOPEN		= ::RegisterWindowMessage("RMSG_SINGLEMAPOPEN");

COrderMonitorManager::COrderMonitorManager()
{
	m_hATHwnd = NULL;
}

HWND COrderMonitorManager::GetHandle()
{
	if(m_hATHwnd==NULL)	CheckOpenMoniterServer();
	// m_hATHwnd = (HWND)theApp.m_StateMng.m_dwInterface[RunModule_AutoTrade];
	return m_hATHwnd;
}


HWND COrderMonitorManager::CheckOpenMoniterServer()
{
	m_hATHwnd = (HWND)theApp.m_StateMng.m_dwInterface[RunModule_AutoTrade];
	if(m_hATHwnd) return m_hATHwnd;

	{ // ���ο� ȭ�鿭�� ��û
		//CString szMapID="11490";
		char aMapID[6]={0,};
		CString szCFG; szCFG.Format("%s%s", theApp.m_szRootDir, "/Data/HidenMng.ini");
		::GetPrivateProfileString("HIDENMNG", "AUTOTMONTOR", "1149", aMapID, 5, szCFG);
		CString szMapID(aMapID); szMapID+="0";
		HWND hMain = theApp.m_hMainHwnd;
		m_szMapID = szMapID;
		// ::SendMessage(hMain, RMSG_SINGLEMAPOPEN, 0, (LPARAM)(LPCTSTR)szMapID);
		::PostMessage(hMain, RMSG_SINGLEMAPOPEN, 0, (LPARAM)(LPCTSTR)m_szMapID);
	}

	// ȭ���� �������� ����͸� �ڵ鰪 üũ.
	CMonitorWnd mWnd;
	if(mWnd.OpenWindow())
	{
		if(mWnd.WaitResult()>MONITER_WRET_READY)
		{
			m_hATHwnd = mWnd.m_hATHwnd;
			return m_hATHwnd;
		}
	}
	return NULL;
}


BOOL COrderMonitorManager::SetState(DSBM_RUNSTATE* pData, long dwLong)
{
	if(!GetHandle()) return FALSE;

	COPYDATASTRUCT copyData;
	copyData.dwData = SBMID_RUNSTATE;
	copyData.cbData = DSBM_RUNSTATE_SIZE;
	copyData.lpData = (void*)pData;

	::SendMessage(m_hATHwnd, WM_COPYDATA, 0, (LPARAM)&copyData);
	return TRUE;
}

// ������ ���ۻ��¸� ��Ÿ����.
BOOL COrderMonitorManager::SetCondition(DSBM_RUN* pData, long dwLong)
{
	if(!GetHandle()) return FALSE;

	COPYDATASTRUCT copyData;
	copyData.dwData = SBMID_CONDITION;
	copyData.cbData = DSBM_RUN_SIZE;
	copyData.lpData = (void*)pData;

	::SendMessage(m_hATHwnd, WM_COPYDATA, 0, (LPARAM)&copyData);
	return TRUE;
}

// �ֹ��α׸� �����.
BOOL COrderMonitorManager::SetTradeLog(DSBM_TRADELOG* pData, long dwLong)
{	
	if(!GetHandle()) return FALSE;

	COPYDATASTRUCT copyData;
	copyData.dwData = SBMID_SBLOG;
	copyData.cbData = DSBM_SBLOG_SIZE;
	copyData.lpData = (void*)pData;

	::SendMessage(m_hATHwnd, WM_COPYDATA, 0, (LPARAM)&copyData);
	return TRUE;
}

