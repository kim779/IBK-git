// IWGraphInfoManager.cpp: implementation of the CIWGraphInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfoManager.h"

#include "GraphData.h"					// for CSubGraphData
#include "IndicatorInfo.h"				// for CIndicatorInfo
#include "IWGraphInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWGraphInfoManager)

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

// [04/10/14] Graph�� 6������ ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWGraphInfoManager::GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6])
{
	// 1. Indicator Info�� ���Ѵ�.
	CIndicatorInfo *pIndicatorInfo = NULL;
	if( p_bDefaultInfo) pIndicatorInfo = m_pIndicatorList->GetDefaultIndicatorInfo( EILT_USER_DEFAULT, p_szGraphName, EIIGT_WITH_DEFAULT_GET);
	else pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, p_szGraphName, p_szGraphName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if( !pIndicatorInfo) return FALSE;

	// 2. SubGraph�� ���Ѵ�.
	CSubGraphData *pSubGraphData = pIndicatorInfo->GetSubGraphData( p_nSubIndex);
	if( !pSubGraphData) return FALSE;

	// 3. Graph Color�� ���Ѵ�.
	CGraphColor *pGraphColor = pSubGraphData->GetColor_PointerType();
	if( !pGraphColor) return FALSE;
	p_aclrGraphColor6[ 0] = pGraphColor->GetColor1();
	p_aclrGraphColor6[ 1] = pGraphColor->GetColor2();
	p_aclrGraphColor6[ 2] = pGraphColor->GetColor3();
	p_aclrGraphColor6[ 3] = pGraphColor->GetColor4();
	p_aclrGraphColor6[ 4] = pGraphColor->GetColor5();
	p_aclrGraphColor6[ 5] = pGraphColor->GetColor6();
	return TRUE;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonConditionList
 - Created at  :  2005-07-14   10:19
 - Author      :  ������
 - Parameters  :  pcszGraphName - ��ǥ��
 - Return Value:  CList<double, double>* - ������ ����Ʈ
 - Description :  �ش� ��ǥ�� �������� �����´�.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfoManager::GetCommonConditionList(const char* pcszGraphName)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, pcszGraphName, pcszGraphName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if(!pIndicatorInfo)
		return NULL;

	return pIndicatorInfo->GetCommonCalConditionList();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetConditionList
 - Created at  :  2005-07-14   10:24
 - Author      :  ������
 - Parameters  :  pcszGraphName  - ��ǥ��
 -                nSubGraphIndex - ��ǥ�� ����׷��� �ε���
 - Return Value:  CList<double, double>* - ������ ����Ʈ
 - Description :  �ش� ��ǥ�� �������� �����´�.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfoManager::GetConditionList(const char* pcszGraphName, const int nSubGraphIndex)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, pcszGraphName, pcszGraphName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if(!pIndicatorInfo)
		return NULL;

	return pIndicatorInfo->GetCalConditionList(nSubGraphIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetConditionList
 - Created at  :  2005-07-14   10:25
 - Author      :  ������
 - Parameters  :  pcszGraphName    - ��ǥ��
 -                pcszSubGraphName - ��ǥ�� ����׷��� ��
 - Return Value:  CList<double, double>* - ������ ����Ʈ
 - Description :  �ش� ��ǥ�� �������� �����´�.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfoManager::GetConditionList(const char* pcszGraphName, const char* pcszSubGraphName)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, pcszGraphName, pcszGraphName, EIIGT_WITH_DEFAULT_GET_AND_SET);
	if(!pIndicatorInfo)
		return NULL;

	return pIndicatorInfo->GetCalConditionList(pcszSubGraphName);
}

// [05/07/15] Getting Graph Info Interface
IGraphInfo *CIWGraphInfoManager::GetGraphInfo( const char *p_szGraphName)
{
	if( !m_pIndicatorList) return NULL;
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, p_szGraphName, p_szGraphName, EIIGT_WITH_DEFAULT_GET);
	if( !pIndicatorInfo) return NULL;
	return CIWGraphInfo::CreateObject( pIndicatorInfo);
}

