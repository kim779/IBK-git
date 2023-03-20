// KTBChartLShare.cpp: implementation of the Local Share Code
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"	// for CAddInManager
#include "KTBChartCtl.h"							// for CKTBChartCtrl
#include "LocalShareInfoList.h"						// for CLocalShareInfo
#include "LSharedData.h"							// for LPST_LSHAREDATA

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.11.29, ��¿�, �м�) SetAllProperties2()���� ȣ��Ǿ�, ������ ���� ���� String��
//		m_strLocalSharedList�� m_ListLShare�� �����ϴ� �۾��� ó���Ѵ�.
//		Ư�� �̶�, �ʱ� ������ Ȯ�ΰ� ���� ���� Notify�� ������ ��û��� �Ѵ�.
BOOL CKTBChartCtrl::InitLSharedData()
{
	// 0. ���� Base Desk Pointer�� Ȯ���Ѵ�.
	if( !m_pBaseDesk) return FALSE;
	if( !m_plLocalShareInfoList) return FALSE;

	// 1. Local ���� Manager�� Interface Pointer�� ���Ѵ�.
	// (2004.11.29, ��¿�) ����� ���� ���� ����� Message ����� �̿��ϸ鼭,
	//		������ Interface �뺸 ����� �̿���� �ʰ� �ִ�.
//	m_pILSharedData = ( ILSharedData *)m_pBaseDesk->GetLSharedData();
//	if( !m_pILSharedData) return FALSE;

	// 2. ���� ���� Data String�� Ȯ���Ѵ�.
	if( m_strLocalSharedList.IsEmpty()) return TRUE;

	// 3. �� ���� ���� Item�� �����Ͽ� m_ListLShare�� �����Ѵ�.
	// 3.0 ���� ���� ���� Data String�� �����Ѵ�.
	CString strLocalShareSetting;
	char *szLocalShareSetting = strLocalShareSetting.GetBufferSetLength( m_strLocalSharedList.GetLength());
	strcpy( szLocalShareSetting, m_strLocalSharedList);
	while( szLocalShareSetting && *szLocalShareSetting)
	{
		// 3.1 ���� ���� ���� Item�� ã�� �и��Ѵ�.
		char *szNextLShareItem = strchr( szLocalShareSetting, ';');
		if( szNextLShareItem) *szNextLShareItem++ = '\0';

		// 3.2 ���� ���� Item�� ������ Byte�� �����Ͽ� Code�� �����Ѵ�.
		// 3.2.1 ���� ���� Item String�� ���̸� ���Ѵ�.
		int nItemLen = strlen( szLocalShareSetting);
		// 3.2.2 ���� ���� Item String�� 2 Byte �̻��̾�� �Ѵ�.
		if( 2 <= nItemLen)
		{
			// 3.2.3 ���� ���� Item�� ���� Code�� ���Ѵ�.
			//	(������ Byte�� Ascii ���� Character�� �Ǿ� �ִ�.)
			int nSettingCode = szLocalShareSetting[ nItemLen - 1] - '0';
			// 3.2.4 ���� Code�� Clear���� ��Ȯ�� �������� Ȯ���� �� �ֵ��� �Ѵ�.
			szLocalShareSetting[ nItemLen - 1] = '\0';

			// 3.3 ���� ���������� Local Share Info List�� ����Ѵ�.
			CLocalShareInfo *pLocalShareInfo = m_plLocalShareInfoList->AddLocalShareInfo( szLocalShareSetting, nSettingCode);
			if( !pLocalShareInfo) MessageBox( CString( "Mulitple Local Share Setting!\n\nLocal Share Name : ") + szLocalShareSetting, "Chart OCX");
			// (2004.11.29, ��¿�) ����� ���� ���� ����� Message ����� �̿��ϸ鼭,
			//		������ Interface �뺸 ����� �̿���� �ʰ� �ִ�.
//			else
//			{
//				// 3.4 ���� ���� Item�� Start ������ Get ������ ó���Ѵ�.
//				if( m_pILSharedData)
//				{
//					if( pLocalShareInfo->m_bStart) 
//					{
//						CString strLSharedData;
//						if( !m_pILSharedData->GetData( szLocalShareSetting, strLSharedData)) strLSharedData.Empty();
//						pLocalShareInfo->m_strLocalSharedData = strLSharedData;
//					}
//					if( pLocalShareInfo->m_bGet)
//						m_pILSharedData->DAdvise( &m_xLAdviseSink, szLocalShareSetting);
//				}
//			}
		}

		// 3.5 ���� ���� ���� Item�� ó���� �� �ֵ��� �Ѵ�.
		szLocalShareSetting = szNextLShareItem;
	}

	return TRUE;
}

// Local Shared Data Receiving Routine
LRESULT CKTBChartCtrl::Rmsg_LSharedChaned(WPARAM wParam, LPARAM lParam)
{
	if( !m_plLocalShareInfoList) return 0L;

	LPST_LSHAREDATA pSHD = (LPST_LSHAREDATA)wParam;
	if( !pSHD) return 0L;

	CString szShareName = pSHD->m_szShareName;
	CString szData;
	if( pSHD->m_nType == LSDT_DWORD) szData.Format("%ld", pSHD->m_dwShareData);
	else szData = pSHD->m_szShareData;

	LRESULT lResult = m_plLocalShareInfoList->SetLocalSharedData( szShareName, szData);

	// (2005/11/21 - Seung-Won, Bae) Notify Local Shared Data Changed to AddIn
	if( m_pAddInManager) m_pAddInManager->OnLSharedDataChange( szShareName, szData);

	return lResult;
}

// (2004.11.29, ��¿�) ����� ���� ���� ����� Message ����� �̿��ϸ鼭,
//		������ Interface �뺸 ����� �̿���� �ʰ� �ִ�.
//BEGIN_INTERFACE_MAP(CKTBChartCtrl, COleControl)
//  INTERFACE_PART(CKTBChartCtrl, IID_ILAdviseSink, LAdviseSink)
//END_INTERFACE_MAP()
//STDMETHODIMP_(ULONG) CKTBChartCtrl::XLAdviseSink::AddRef()
//{
//	METHOD_PROLOGUE(CKTBChartCtrl, LAdviseSink)
//	ULONG nResult = (ULONG)pThis->ExternalAddRef();
//	return nResult;
//}
//STDMETHODIMP_(ULONG) CKTBChartCtrl::XLAdviseSink::Release()
//{
//	METHOD_PROLOGUE(CKTBChartCtrl, LAdviseSink)
//	ULONG nResult = (ULONG)pThis->ExternalRelease();
//	return nResult;
//}
//STDMETHODIMP CKTBChartCtrl::XLAdviseSink::QueryInterface(REFIID iid, LPVOID* ppvObj)
//{
//	METHOD_PROLOGUE(CKTBChartCtrl, LAdviseSink)
//	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
//	return hr;
//}
//STDMETHODIMP CKTBChartCtrl::XLAdviseSink::OnDataChange(LPCSTR szSharedName, LPCSTR szData)
//{
//	METHOD_PROLOGUE(CKTBChartCtrl, LAdviseSink)
//	return pThis->ParedShareDataChange(szSharedName, szData);
//}
