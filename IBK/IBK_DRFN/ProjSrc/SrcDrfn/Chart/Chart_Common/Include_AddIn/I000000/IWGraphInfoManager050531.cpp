// IWGraphInfoManager050531.cpp: implementation of the CIWGraphInfoManager050531 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfoManager050531.h"

#include "IndicatorList.h"				// for CIndicatorList
#include "GraphData.h"					// for CSubGraphData
#include "IndicatorInfo.h"				// for CIndicatorInfo
#include "IWGraphInfo050531.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWGraphInfoManager050531::CIWGraphInfoManager050531( CIndicatorList *p_pIndicatorList)
{
	// (2004.10.11, ��¿�) MainBlock Object�� Pointer�� �����Ѵ�.
	m_pIndicatorList = p_pIndicatorList;
	if( !m_pIndicatorList)
	{
		AfxMessageBox( "CIWGraphInfoManager050531 Null Wrapping Error!");
		return;
	}
}

CIWGraphInfoManager050531::~CIWGraphInfoManager050531()
{

}

// [04/10/14] Graph�� 6������ ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWGraphInfoManager050531::GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6])
{
	// 1. Indicator Info�� ���Ѵ�.
	CIndicatorInfo *pIndicatorInfo = NULL;
	if( p_bDefaultInfo) pIndicatorInfo = m_pIndicatorList->GetDefaultIndicatorInfo( p_szGraphName);
	else pIndicatorInfo = m_pIndicatorList->GetUserIndicatorInfo( p_szGraphName);
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
CList<double, double>* CIWGraphInfoManager050531::GetCommonConditionList(const char* pcszGraphName)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetUserIndicatorInfo(pcszGraphName);
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
CList<double, double>* CIWGraphInfoManager050531::GetConditionList(const char* pcszGraphName, const int nSubGraphIndex)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetUserIndicatorInfo(pcszGraphName);
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
CList<double, double>* CIWGraphInfoManager050531::GetConditionList(const char* pcszGraphName, const char* pcszSubGraphName)
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetUserIndicatorInfo(pcszGraphName);
	if(!pIndicatorInfo)
		return NULL;

	return pIndicatorInfo->GetCalConditionList(pcszSubGraphName);
}

// [05/07/15] Getting Graph Info Interface
IGraphInfo *CIWGraphInfoManager050531::GetGraphInfo( const char *p_szGraphName)
{
	if( !m_pIndicatorList) return NULL;
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetIndicatorInfo( p_szGraphName);
	if( !pIndicatorInfo) return NULL;
	IGraphInfo *pIGraphInfo = new CIWGraphInfo050531( pIndicatorInfo);
	pIGraphInfo->AddRef();
	return pIGraphInfo;
}

