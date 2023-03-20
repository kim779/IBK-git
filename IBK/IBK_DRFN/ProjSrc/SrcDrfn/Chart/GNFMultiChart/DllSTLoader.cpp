// DllSTLoader.cpp: implementation of the CDllSTLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DllSTLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDllSTLoader::CDllSTLoader()
{
	m_hSTDll= NULL;
	m_fnGetInputs = NULL;
	m_fnLoadST = NULL;
	m_fnUnloadST = NULL;
	m_fnExecuteST = NULL;
	m_fnStopST = NULL;
	m_fnPauseST = NULL;
	m_fnUpdateST = NULL;
	m_fnCommandST = NULL;
	m_lSTActor = NULL;
}

CDllSTLoader::~CDllSTLoader()
{
	Unload();
}

// 20060705 �Լ����� ���� : , HWND hSocket,  CDBMgr* pDrMLib_CDBMgr : �����м�..
//long CDllSTLoader::Load(LPCTSTR lpSDDllNamePath, HWND hParent,HWND hSocket, long pDrMLib_CDBMgr, long lKey, LPCTSTR lpSTName,int nSTPos,LPCTSTR lpAdditionalOption)
//SD������ ���� Dll�� �ε��ϰ�, m_lSTActor�� �ּҸ� �����Ѵ�. 
long CDllSTLoader::Load(LPCTSTR lpSDDllNamePath, HWND hParent, long lKey, LPCTSTR lpSTName,int nSTPos,LPCTSTR lpAdditionalOption,long pChartItem)
{
	if(!lKey)	return -1;		
	if(m_hSTDll)
	{
		Unload();
	}	
	BOOL bRetValue = LoadDLL(lpSDDllNamePath);
	if(!bRetValue)
	{
		return -1;
	}

	// 20060705 �Լ����� ���� : , hSocket, (CDBMgr*)pDrMLib_CDBMgr
	//m_lSTActor = m_fnLoadST(hParent, hSocket, (CDBMgr*)pDrMLib_CDBMgr, lKey, lpSTName,nSTPos,lpAdditionalOption);
	m_lSTActor = m_fnLoadST(hParent, NULL, pChartItem, lKey, lpSTName,nSTPos,lpAdditionalOption);

	return m_lSTActor;
}

//SD������ �ε��Ͽ� �����ϴ� �Լ��� ��������� �����Ѵ�.
BOOL CDllSTLoader::LoadDLL(LPCTSTR lpSTFileName)
{
	m_strSTDll = lpSTFileName;
	m_hSTDll  = (HINSTANCE) ::LoadLibrary(m_strSTDll);
	if( m_hSTDll == NULL)
	{
		m_strError.Format("%s�� Load�� �� �����ϴ�.(%ld)",lpSTFileName, GetLastError());
		return FALSE;
	}

	m_fnGetInputs = (_DLL_GetInputs)::GetProcAddress(m_hSTDll,"fnGetInputs");
	if(m_fnGetInputs == NULL)
	{
		m_strError.Format("%s���� fnGetInputs�Լ��� ã�� �� �����ϴ�.",lpSTFileName);
		return FALSE;
	}
	m_fnLoadST = (_DLL_LoadST)::GetProcAddress(m_hSTDll,"fnLoadST");
	if(m_fnLoadST == NULL)
	{
		m_strError.Format("%s���� fnLoadST�Լ��� ã�� �� �����ϴ�.",lpSTFileName);
		return FALSE;
	}
	m_fnUnloadST = (_DLL_UnloadST)::GetProcAddress(m_hSTDll,"fnUnloadST");
	if(m_fnUnloadST == NULL)
	{
		m_strError.Format("%s���� fnUnloadST�Լ��� ã�� �� �����ϴ�.",lpSTFileName);
		return FALSE;
	}
	m_fnExecuteST = (_DLL_ExecuteST)::GetProcAddress(m_hSTDll,"fnExecuteST");
	if(m_fnExecuteST == NULL)
	{
		m_strError.Format("%s���� fnExecuteST�Լ��� ã�� �� �����ϴ�.",lpSTFileName);
		return FALSE;
	}	
	m_fnStopST = (_DLL_StopST)::GetProcAddress(m_hSTDll,"fnStopST");
	if(m_fnStopST == NULL)
	{
		m_strError.Format("%s���� fnStopST�Լ��� ã�� �� �����ϴ�.",lpSTFileName);
		return FALSE;
	}	
	m_fnPauseST = (_DLL_PauseST)::GetProcAddress(m_hSTDll,"fnPauseST");
	if(m_fnPauseST == NULL)
	{
		m_strError.Format("%s���� fnPauseST �Լ��� ã�� �� �����ϴ�.",lpSTFileName);
		return FALSE;
	}	
	m_fnUpdateST = (_DLL_UpdateST)::GetProcAddress(m_hSTDll,"fnUpdateST");
	if(m_fnUpdateST == NULL)
	{
		m_strError.Format("%s���� fnUpdateST �Լ��� ã�� �� �����ϴ�.",lpSTFileName);
		return FALSE;
	}	
	m_fnCommandST = (_DLL_CommandST)::GetProcAddress(m_hSTDll,"fnCommandST");
	if(m_fnCommandST == NULL)
	{
		m_strError.Format("%s���� fnCommandST �Լ��� ã�� �� �����ϴ�.",lpSTFileName);
		return FALSE;
	}	
	return TRUE;
}

BOOL CDllSTLoader::UnloadDLL()
{
	if(!m_hSTDll) return TRUE;

	BOOL bResult = FALSE;
	bResult = FreeLibrary(m_hSTDll);
	if(bResult)
	{
		m_hSTDll = NULL;
		m_fnGetInputs = NULL;
		m_fnLoadST = NULL;
		m_fnUnloadST = NULL;
		m_fnExecuteST = NULL;
		m_fnStopST = NULL;
		m_fnPauseST = NULL;
		m_fnUpdateST = NULL;
		m_fnCommandST = NULL;
	}
	else
	{
		return 0;
	}
	return 1;
}

long CDllSTLoader::Unload()
{
	if(!m_hSTDll) return -1;

	if(m_lSTActor)	m_fnUnloadST(m_lSTActor);
	else
	{
		AfxMessageBox("CDllSTLoader::Unload(0)");
		return 0;
	}
	if(m_hSTDll)	UnloadDLL();
	else
	{
		AfxMessageBox("CDllSTLoader::Unload(-1)");
		return -1;
	}

//	if(!m_lSTActor)	return 0;
//	if(!m_hSTDll) return -1;
	return 1;
}

//�����м����� ����� �´�..
long CDllSTLoader::Execute(LPCTSTR lpInputData)
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnExecuteST(m_lSTActor, lpInputData);
}

long CDllSTLoader::Stop()
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnStopST(m_lSTActor);
}

long CDllSTLoader::Pause()
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnPauseST(m_lSTActor);
}

long CDllSTLoader::Update(long lPos, BOOL bIsUpdateData)
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnUpdateST(m_lSTActor,lPos,bIsUpdateData);
}

long CDllSTLoader::Command(LPCTSTR lpCommand, LPCTSTR lpOption)
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnCommandST(m_lSTActor, lpCommand,lpOption);
}

CString CDllSTLoader::GetInput(long lIndex)
{
	if(!m_lSTActor)	return "#";
	if(!m_hSTDll) return "#";
	return m_fnGetInputs(lIndex);
}