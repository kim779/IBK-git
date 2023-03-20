// IWGraphInfo050531.cpp: implementation of the CIWGraphInfo050531 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfo050531.h"

#include "IndicatorInfo.h"
#include "GraphData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWGraphInfo050531::CIWGraphInfo050531(CIndicatorInfo* pIndicatorInfo)
{
	m_pIndicatorInfo = pIndicatorInfo;
	if(!pIndicatorInfo)
	{
		AfxMessageBox("CIWGraphInfo050531 Null Wrapping Error!");
		return;
	}
}

CIWGraphInfo050531::~CIWGraphInfo050531()
{

}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetCommonCalculateVarList
 - Created at  :  2005-07-15   13:31
 - Author      :  ������
 - Return Value:  CList<double, double>* - ������ǰ� ����Ʈ
 - Description :  ��ǥ��꿡 �ʿ��� ���ǰ��� ����Ʈ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
CList<double, double>* CIWGraphInfo050531::GetCommonCalculateVarList()
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
CList<double, double>* CIWGraphInfo050531::GetCalculateVarList(const int nSubGraphIndex)
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
CList<double, double>* CIWGraphInfo050531::GetCalculateVarList(const char* pcszSubGraphName)
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
LPCTSTR CIWGraphInfo050531::GetCommonCalculatePacketNames()
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
LPCTSTR CIWGraphInfo050531::GetCalculatePacketNames(const int nSubGraphIndex)
{
	if(!m_pIndicatorInfo)
		return _T("");

	return m_pIndicatorInfo->GetPacketNames(nSubGraphIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetSubGraphIndex
 - Created at  :  2005-07-15   13:51
 - Author      :  ������
 - Parameters  :  pcszSubGraphName - ����׷�����.
 - Return Value:  int - ����׷��� �ε���
 - Description :  ����׷����� �ε����� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
int CIWGraphInfo050531::GetSubGraphIndex(const char* pcszSubGraphName)
{
	if(!m_pIndicatorInfo)
		return -1;

	return m_pIndicatorInfo->GetSubGraphIndex(pcszSubGraphName);
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
bool CIWGraphInfo050531::IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow)
{
	CSubGraphData* pSubGraphData = m_pIndicatorInfo->GetSubGraphData(pcszSubGraphName);
	if(pSubGraphData == NULL)
		return false;
	
	bSubGraphShow = pSubGraphData->GetHiding() ? false : true;
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
bool CIWGraphInfo050531::IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow)
{
	CSubGraphData* pSubGraphData = m_pIndicatorInfo->GetSubGraphData(nSubGraphIndex);
	if(pSubGraphData == NULL)
		return false;

	bSubGraphShow = pSubGraphData->GetHiding() ? false : true;
	return true;
}