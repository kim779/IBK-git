// IWGraphInfo.cpp: implementation of the CIWGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfo.h"

#include "../../../Include_Chart/Graph.h"		// for CGraph
#include "../../../Include_Chart/GraphData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWGraphInfo)

//////////////////////////////////////////////////////////////////////
// // (2006/7/13 - Seung-Won, Bae) Operation.
//////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonCalculateVarList
 - Created at  :  2005-07-15   13:31
 - Author      :  ������
 - Return Value:  CList<double, double>* - ������ǰ� ����Ʈ
 - Description :  ��ǥ��꿡 �ʿ��� ���ǰ��� ����Ʈ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfo::GetCommonCalculateVarList()
{
	if(!m_pIndicatorInfo)
		return NULL;
	return m_pIndicatorInfo->GetCommonCalConditionList();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculateVarList
 - Created at  :  2005-07-15   13:32
 - Author      :  ������
 - Parameters  :  nSubGraphIndex - ����׷��� �ε���
 - Return Value:  CList<double, double>* - ������ǰ� ����Ʈ
 - Description :  ��ǥ��꿡 �ʿ��� ���ǰ��� ����׷������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfo::GetCalculateVarList(const int nSubGraphIndex)
{
	if(!m_pIndicatorInfo)
		return NULL;
	return m_pIndicatorInfo->GetCalConditionList(nSubGraphIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculateVarList
 - Created at  :  2005-07-15   13:33
 - Author      :  ������
 - Parameters  :  pcszSubGraphName - ����׷��� ����
 - Return Value:  CList<double, double>* - ������ǰ� ����Ʈ
 - Description :  ��ǥ��꿡 �ʿ��� ���ǰ��� ����׷������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfo::GetCalculateVarList(const char* pcszSubGraphName)
{
	if(!m_pIndicatorInfo)
		return NULL;

	return m_pIndicatorInfo->GetCalConditionList(pcszSubGraphName);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonCalculatePacketNames
 - Created at  :  2005-07-15   13:49
 - Author      :  ������
 - Return Value:  LPCTSTR - ����� ��Ŷ��
 - Description :  ��ǥ ��꿡 �ʿ��� ��Ŷ���� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
LPCTSTR CIWGraphInfo::GetCommonCalculatePacketNames()
{
	if(!m_pIndicatorInfo)
		return _T("");

	return m_pIndicatorInfo->GetCommonPacketNames();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCalculatePacketNames
 - Created at  :  2005-07-15   13:51
 - Author      :  ������
 - Parameters  :  nSubGraphIndex - ����׷��� �ε���
 - Return Value:  LPCTSTR - ��꿡 �ʿ��� ��Ŷ��
 - Description :  ��ǥ ��꿡 �ʿ��� ��Ŷ���� �����Ѵ�. (����׷������� ����)
 -----------------------------------------------------------------------------------------*/
LPCTSTR CIWGraphInfo::GetCalculatePacketNames(const int nSubGraphIndex)
{
	if(!m_pIndicatorInfo)
		return _T("");

	return m_pIndicatorInfo->GetPacketNames(nSubGraphIndex);
}

//////////////////////////////////////////////////////////////////////
// (2006/7/7 - Seung-Won, Bae) SubGraph Info
//////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------------------------
 - Function    :  GetSubGraphIndex
 - Created at  :  2005-07-15   13:51
 - Author      :  ������
 - Parameters  :  pcszSubGraphName - ����׷�����.
 - Return Value:  int - ����׷��� �ε���
 - Description :  ����׷����� �ε����� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
int CIWGraphInfo::GetSubGraphIndex(const char* pcszSubGraphName)
{
	if(!m_pIndicatorInfo)
		return -1;

	return m_pIndicatorInfo->GetSubGraphIndex(pcszSubGraphName);
}

// (2006/7/7 - Seung-Won, Bae) Get Sub Graph Count.
int CIWGraphInfo::GetSubGraphCount( void)
{
	if( !m_pIndicatorInfo) return 0;
	return m_pIndicatorInfo->GetSubGraphCount();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsShowSubGraph
 - Created at  :  2005-07-29   10:11
 - Author      :  ������
 - Parameters  :  pcszSubGraphName - [in] ����׷�����.
 -                bSubGraphShow    - [out] Show/Hide���θ� �����Ѵ�.
 - Return Value:  bool - true: ������ ȣ��, false: ����(SubGraph�� ������찡 ���� ū ��������)
 - Description :  ����׷����� Show/Hide���θ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWGraphInfo::IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow)
{
	bSubGraphShow = true;
	if( m_pIndicatorInfo)
	{
		// (2007/3/9 - Seung-Won, Bae) Check Graph's Hide
		CGraph *pGraph = m_pIndicatorInfo->GetGraph();
		if( pGraph) if( pGraph->IsGraphHide()) bSubGraphShow = false;

		if( bSubGraphShow) bSubGraphShow = !m_pIndicatorInfo->GetSubGraphHiding( pcszSubGraphName);
	}
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsShowSubGraph
 - Created at  :  2005-07-29   10:14
 - Author      :  ������
 - Parameters  :  nSubGraphIndex - [in] SubGraph Index
 -                bSubGraphShow  - [out] Show/Hide���θ� �����Ѵ�.
 - Return Value:  bool - true: ������ ȣ��, false: ����(SubGraph�� ������찡 ���� ū ��������)
 - Description :  ����׷����� Show/Hide���θ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWGraphInfo::IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow)
{
	bSubGraphShow = true;
	if( m_pIndicatorInfo)
	{
		// (2007/3/9 - Seung-Won, Bae) Check Graph's Hide
		CGraph *pGraph = m_pIndicatorInfo->GetGraph();
		if( pGraph) if( pGraph->IsGraphHide()) bSubGraphShow = false;

		if( bSubGraphShow) bSubGraphShow = !m_pIndicatorInfo->GetSubGraphHiding( nSubGraphIndex);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve Graph Info by string
//////////////////////////////////////////////////////////////////////
BOOL CIWGraphInfo::GetGraphDataForMaking( CString &p_strData)
{
	if( !m_pIndicatorInfo) return FALSE;
	m_pIndicatorInfo->GetGraphDataForMaking( p_strData);
	return !p_strData.IsEmpty();
}
BOOL CIWGraphInfo::GetAllSubGraphDataForMaking( CString &p_strData)
{
	if( !m_pIndicatorInfo) return FALSE;
	m_pIndicatorInfo->GetAllSubGraphDataForMaking( p_strData);
	return !p_strData.IsEmpty();
}

//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Data
//////////////////////////////////////////////////////////////////////
void CIWGraphInfo::GetGraphDataDrawingData( BOOL &p_bLowHigh, BOOL &p_bLowCur, BOOL &p_bHighCur, BOOL &p_bPrevCur, int &p_nCurData)
{
	bool bLowHigh	= ( p_bLowHigh	? true : false);
	bool bLowCur	= ( p_bLowCur	? true : false);
	bool bHighCur	= ( p_bHighCur	? true : false);
	bool bPrevCur	= ( p_bPrevCur	? true : false);
	BOOL bShowOHLC = FALSE;
	if( m_pIndicatorInfo) m_pIndicatorInfo->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, p_nCurData, bShowOHLC);
	p_bLowHigh	= bLowHigh;
	p_bLowCur	= bLowCur;
	p_bHighCur	= bHighCur;
	p_bPrevCur	= bPrevCur;
}

//////////////////////////////////////////////////////////////////////
// (2006/7/6 - Seung-Won, Bae) Retrieve Signal Info
//////////////////////////////////////////////////////////////////////
void CIWGraphInfo::GetSignalInfo( BOOL &p_bValidFlag, BOOL &p_bShowSignal)
{
	if( !m_pIndicatorInfo) return;
	p_bValidFlag	= m_pIndicatorInfo->IsSignalUse();
	p_bShowSignal	= m_pIndicatorInfo->IsSignalShow();
}
