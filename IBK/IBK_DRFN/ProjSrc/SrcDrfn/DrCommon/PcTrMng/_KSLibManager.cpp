// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

//#include "../../../../CommonSrc/LinkDataManager.h"
//#include "../../../../Linkmanager/LinkManager.h"

//@�ſ�����#include "../../../../Builder/LinkInfoMng/KSLinkInfoMng.h"
//@�ſ�����#include "../../../../Controls/GDIMgr/ColorMap.h"
//@�ſ�����#include "../../../../Common/CommHeaderDef.h"

//@�ſ�����extern CString KSLink_GetFileHeader();		// �Ｚ���� ��ȣȭ�� ���̵� ������� �ܺθ�ũ.
//@�ſ�����extern CString KSLink_GetLoginInfo(CString strTag);
//@�ſ�����extern int     KSLink_GetAccountCount();
//@�ſ�����extern CString KSLink_GetAccount(int nIndex);
//@�ſ�����extern CString KSLink_GetAccInfo(CString strAccount, int nType);
//@�ſ�����extern LOGIN_ACC_INFO* KSLink_GetAccInfo(CString strAccount);
//@�ſ�����extern CColorMap* GetColorMap();

CKSLibManager::CKSLibManager()
{
	m_pColorMap = NULL;
}

BOOL CKSLibManager::Lib_KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue)
{
//@�ſ�����::KSLink_SetLinkTagData(sTag, sValue);
	return TRUE;
}

BOOL CKSLibManager::Lib_KSLink_SetFormLinkTagData(CString sTag, CString sValue)
{
//@�ſ�����::KSLink_SetFormLinkTagData(sTag, sValue);
	return TRUE;
}

CString CKSLibManager::Lib_KSLink_GetFileHeader()
{
//@�ſ�����return ::KSLink_GetFileHeader();
	return "";
}

CString CKSLibManager::Lib_KSLink_GetLoginInfo(CString strTag)
{
//@�ſ�����return ::KSLink_GetLoginInfo(strTag);
	return "";
}

int CKSLibManager::Lib_KSLink_GetAccountCount()
{
//@�ſ�����return ::KSLink_GetAccountCount();
	return 0;
}

CString CKSLibManager::Lib_KSLink_GetAccount(int nIndex)
{
//@�ſ�����return ::KSLink_GetAccount(nIndex);
	return "";
}

BYTE CKSLibManager::Lib_CommRequestData(HWND hFormWnd, LONG _pRqCommInfo, BYTE* lpData, int nDataLen, 
					 BOOL bDownLoad, int nProgressType, BOOL bWaitCursor)
{
//@�ſ�����	REQUEST_COMMINFO* pRqCommInfo = (REQUEST_COMMINFO*)_pRqCommInfo;
//@�ſ�����	return ::CommRequestData(hFormWnd, pRqCommInfo, lpData, nDataLen, bDownLoad, nProgressType, bWaitCursor);
	return (BYTE)1;
}

CString CKSLibManager::Lib_KSLink_GetAccInfo(CString strAccount, int nType)
{
//@�ſ�����	return ::KSLink_GetAccInfo(strAccount, nType);
	return "";
}

long CKSLibManager::Lib_KSLink_GetAccInfo(CString strAccount)
{
//@�ſ�����	return (long)::KSLink_GetAccInfo(strAccount);
	return 0l;
}

COLORREF CKSLibManager::Lib_KSLink_GetColor(short nIndex, BOOL bUseOrgIndex)
{
//@�ſ�����
//	if(!m_pColorMap)
//		m_pColorMap = (long)::GetColorMap();
//
//	if(!m_pColorMap) return(RGB(255,255,255));
//
//	CColorMap* pColorMap = (CColorMap*)m_pColorMap;
//	return pColorMap->GetColor(nIndex);
	return RGB(0,0,0);
}


long CKSLibManager::Lib_KSLink_GetEnvInfo(int nType)
{

//@�ſ�����	return ::KSLink_GetEnvInfo(nType);
	return 0l;
}
