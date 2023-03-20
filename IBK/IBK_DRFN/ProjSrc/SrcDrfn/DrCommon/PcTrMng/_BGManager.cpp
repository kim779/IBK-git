// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

//--------------------------------------------------------------------------------------------
//		CBKInfo
//--------------------------------------------------------------------------------------------
CBKInfo::CBKInfo()
{
	m_hLib = NULL;
	m_nRefer = 0;

	m_fnGetInterface = NULL;
	m_fnSetInterface = NULL;
	m_fnTerminate	 = NULL;
}


CBKInfo::~CBKInfo()
{
	m_nRefer = 0; // ������ ������ ���ؼ�...
	UnLoadLib();
}

HINSTANCE CBKInfo::LoadLib()
{
	if(m_hLib) 
	{
		AddRefer();
		return m_hLib;
	}

	m_hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szMoudle);
	if(m_hLib)
	{
		m_fnGetInterface = (BKMNG_GetInterface)::GetProcAddress(m_hLib, "BKGetInterface");
		m_fnSetInterface = (BKMNG_SetInterface)::GetProcAddress(m_hLib, "BKSetInterface");
		m_fnTerminate    = (BKMNG_Terminate)::GetProcAddress(m_hLib, "BKTerminate");
	}
	return m_hLib;
}

void CBKInfo::UnLoadLib()
{
	ReleaseRefer();
	if(m_nRefer==0 && m_hLib)
	{
		Terminate();
		::FreeLibrary(m_hLib);
		m_hLib = NULL;
	}
}


int CBKInfo::AddRefer()
{
	m_nRefer ++;
	return m_nRefer;
}


int CBKInfo::ReleaseRefer()
{
	m_nRefer --;
	return m_nRefer;
}


HINSTANCE CBKInfo::GetInterface(int nKey, long &pInterface)
{
	pInterface = NULL;
	if(m_hLib && m_fnGetInterface) pInterface = m_fnGetInterface(nKey);
	return m_hLib;
}


long CBKInfo::GetInterface(int nKey)
{
	long pInterface = NULL;
	if(m_hLib && m_fnGetInterface) pInterface = m_fnGetInterface(nKey);

	return pInterface;
}


BOOL CBKInfo::SetInterface(int nKey, long pInterface)
{
	if(m_hLib && m_fnGetInterface) return m_fnSetInterface(nKey, pInterface);
	return FALSE;
}

void CBKInfo::Terminate()
{
	if(m_hLib && m_fnTerminate) m_fnTerminate();
}

//--------------------------------------------------------------------------------------------
//		CAUBGManager
//--------------------------------------------------------------------------------------------
CAUBGManager::CAUBGManager()
{
}

CAUBGManager::~CAUBGManager()
{
	ClearAllList();
}


void CAUBGManager::UnLoadLibAll()
{
	ClearAllList();
}

CBKInfo* CAUBGManager::FindModule(LPCSTR szModule)
{
	for(POSITION pos=m_List.GetHeadPosition(); pos;) {
		CBKInfo* pInfo = m_List.GetNext(pos);
		if(pInfo->m_szMoudle.CompareNoCase(szModule)==0)
			return pInfo;
	}
	return NULL;
}

CBKInfo* CAUBGManager::FindModule(HINSTANCE hLib)
{
	for(POSITION pos=m_List.GetHeadPosition(); pos;) {
		CBKInfo* pInfo = m_List.GetNext(pos);
		if(pInfo->m_hLib==hLib)
			return pInfo;
	}
	return NULL;
}


void CAUBGManager::ClearAllList()
{
	BOOL bResult = TRUE;
	CBKInfo* pInfo = NULL;
	for(POSITION pos=m_List.GetHeadPosition(); pos;)
		delete m_List.GetNext(pos);
	m_List.RemoveAll();
}


HINSTANCE CAUBGManager::IsBackGround(LPCSTR szModuleName)
{
	CBKInfo* pInfo = FindModule(szModuleName);
	if(pInfo) return pInfo->m_hLib;

	return NULL;
}

HINSTANCE CAUBGManager::AddBackGround(LPCSTR szModuleName)
{
	CBKInfo* pInfo = FindModule(szModuleName);
	if(pInfo) return pInfo->LoadLib();

	pInfo = new CBKInfo;
	pInfo->m_szMoudle = szModuleName;
	HINSTANCE hLib = pInfo->LoadLib();
	if(hLib==NULL)
	{
		delete pInfo;
		return NULL;
	}
	else
	{
		m_List.AddTail(pInfo);
		return hLib;
	}
}

// hLib ���ϰ��� ����.
HINSTANCE CAUBGManager::AddBackGround(HINSTANCE hLib)
{
	CBKInfo* pInfo = FindModule(hLib);
	if(pInfo) return pInfo->LoadLib();

	return NULL;
}

// 
HINSTANCE CAUBGManager::DelBackGround(LPCSTR szModuleName)
{
	CBKInfo* pInfo = FindModule(szModuleName);
	if(pInfo) {
		pInfo->UnLoadLib();
		return pInfo->m_hLib;
	}

	return NULL;
}

// hLib ���ϰ��� ����.
HINSTANCE CAUBGManager::DelBackGround(HINSTANCE hLib)
{
	CBKInfo* pInfo = FindModule(hLib);
	if(pInfo) {
		pInfo->UnLoadLib();
		return pInfo->m_hLib;
	}

	return NULL;
}


// DLL�� ����� �������̽��� �Ѱ��ش�.
// DLL�� �ε尡 �ȵǾ� ������ DLL�� �ε��Ѵ�.
// nKey�� �ش��ϴ� �������̽��� pInterface�� �����Ѵ�.
HINSTANCE CAUBGManager::GetInterface(LPCSTR szModuleName, int nKey, long &pInterface)
{
	CBKInfo* pInfo = FindModule(szModuleName);
	if(pInfo) 
		return pInfo->GetInterface(nKey, pInterface);

	return NULL;
}

// hLib�� �˰� ���� ��..
// nKey�� �ش��ϴ� ������ �޴´�.
long CAUBGManager::GetInterface(HINSTANCE hLib, int nKey)
{
	CBKInfo* pInfo = FindModule(hLib);
	if(pInfo) 
		return pInfo->GetInterface(nKey);

	return NULL;
}

// hLib�� �˰� ���� ��..
// nKey�� pInterface �������� �����͸� �Ѱ��ش�.
BOOL CAUBGManager::SetInterface(HINSTANCE hLib, int nKey, long pInterface)
{
	CBKInfo* pInfo = FindModule(hLib);
	if(pInfo) 
		return pInfo->SetInterface(nKey, pInterface);

	return NULL;
}


void CAUBGManager::PreUnLoad()
{
	UnLoadLibAll();
}
