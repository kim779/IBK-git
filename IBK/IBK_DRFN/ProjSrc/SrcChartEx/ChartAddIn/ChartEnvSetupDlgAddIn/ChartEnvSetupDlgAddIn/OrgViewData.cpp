// OrgViewData.cpp: implementation of the COrgViewData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrgViewData.h"

#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class CIndiInfoData

CIndiInfoData* CIndiInfoData::Make(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning)
{
	return new CIndiInfoData(p_pIGraphInfo, bTheBeginning);
}

void CIndiInfoData::Delete(CIndiInfoData* pIndiInfoData)
{
	delete pIndiInfoData;
}

// ----------------------------------------------------------------------------
CIndiInfoData::CIndiInfoData(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning):
	m_bTheBeginning(bTheBeginning)
{
	m_pIGraphInfo = p_pIGraphInfo->CreateCopyObject();
}

CIndiInfoData::~CIndiInfoData()
{
	if( m_pIGraphInfo)
	{
		m_pIGraphInfo->Release();
		m_pIGraphInfo = NULL;
	}
}

// public =====================================================================
void CIndiInfoData::SetTheBeginning(const bool bTheBeginning)
{
	m_bTheBeginning = bTheBeginning;
}

void CIndiInfoData::SetIndicatorInfo( IGraphInfo* p_pIGraphInfo)
{
	if(p_pIGraphInfo == NULL)
		return;

	if( m_pIGraphInfo) m_pIGraphInfo->Release();
	m_pIGraphInfo = p_pIGraphInfo->CreateCopyObject();
}

// ----------------------------------------------------------------------------
bool CIndiInfoData::IsTheBeginning() const
{
	return m_bTheBeginning;
}

IGraphInfo* CIndiInfoData::GetIGraphInfo()
{
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	return m_pIGraphInfo;
}

// ----------------------------------------------------------------------------
CString CIndiInfoData::GetGraphName() const
{
	if( !m_pIGraphInfo) return "";
	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
	return szGraphName;
}

CString CIndiInfoData::GetIndicatorName() const
{
	if( !m_pIGraphInfo) return "";
	ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
	return szIndicatorName;
}


///////////////////////////////////////////////////////////////////////////////
// class CIndiInfoDataList

CIndiInfoDataList::CIndiInfoDataList()
{
}

CIndiInfoDataList::~CIndiInfoDataList()
{
	DeleteAll();
}


// public =====================================================================
bool CIndiInfoDataList::AddTail(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning)
{
	if(p_pIGraphInfo == NULL)
		return false;

	//sy 2006.04.24.
	ILPCSTR szGraphName = p_pIGraphInfo->GetGraphNameString();
	if( IsIndiInfoData(szGraphName)) return false;

	m_indiInfoDataList.AddTail( CIndiInfoData::Make(p_pIGraphInfo, bTheBeginning));
	return true;
}

void CIndiInfoDataList::DeleteAll()
{
	POSITION pos = m_indiInfoDataList.GetHeadPosition();

	while(pos != NULL)
	{
		CIndiInfoData::Delete(m_indiInfoDataList.GetNext(pos));
	}
}

// ----------------------------------------------------------------------------
bool CIndiInfoDataList::ChangeAllIndicatorList( IGraphInfoManager* p_pIGraphInfoManager)
{
	if(p_pIGraphInfoManager == NULL)
		return false;

	bool bResult = false;
	IGraphInfo* pSourceIndicatorInfo = NULL;
	IGraphInfo* pTargetIndicatorInfo = NULL;
	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pSourceIndiInfo = m_indiInfoDataList.GetNext(pos);
		assert(pSourceIndiInfo != NULL);

		pSourceIndicatorInfo = pSourceIndiInfo->GetIGraphInfo();
		if( !pSourceIndicatorInfo) continue;

		ILPCSTR szGraphName = pSourceIndicatorInfo->GetGraphNameString();
		pTargetIndicatorInfo = p_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
		if( !pTargetIndicatorInfo) 
		{
			pSourceIndicatorInfo->Release();
			continue;
		}

		if( pTargetIndicatorInfo->GetCopyDataFrom( pSourceIndicatorInfo)) bResult = true;
		pTargetIndicatorInfo->Release();
		pSourceIndicatorInfo->Release();
	}

	return bResult;
}

// ----------------------------------------------------------------------------
POSITION CIndiInfoDataList::GetHeadPosition() const
{
	return m_indiInfoDataList.GetHeadPosition();
}

POSITION CIndiInfoDataList::GetHeadPosition()
{
	return m_indiInfoDataList.GetHeadPosition();
}

POSITION CIndiInfoDataList::GetTailPosition() const
{
	return m_indiInfoDataList.GetTailPosition();
}

POSITION CIndiInfoDataList::GetTailPosition()
{
	return m_indiInfoDataList.GetTailPosition();
}

CIndiInfoData* CIndiInfoDataList::GetPrev(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;

	return m_indiInfoDataList.GetPrev(pos);
}

CIndiInfoData* CIndiInfoDataList::GetPrev(POSITION& pos)
{
	if(pos == NULL)
		return NULL;

	return m_indiInfoDataList.GetPrev(pos);
}

CIndiInfoData* CIndiInfoDataList::GetNext(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;

	return m_indiInfoDataList.GetNext(pos);
}

CIndiInfoData* CIndiInfoDataList::GetNext(POSITION& pos)
{
	if(pos == NULL)
		return NULL;

	return m_indiInfoDataList.GetNext(pos);
}

CIndiInfoData* CIndiInfoDataList::GetIndiInfoData(const CString& strIndicatorName)
{
	if(strIndicatorName.IsEmpty())
		return NULL;

	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
		assert(pIndiInfoData != NULL);
		if(strIndicatorName == pIndiInfoData->GetIndicatorName())
			return pIndiInfoData;
	}

	return NULL;
}

bool CIndiInfoDataList::GetIndiInfoInTheBeginning(CList<IGraphInfo*, IGraphInfo*>& indiInfoList) const
{
	indiInfoList.RemoveAll();

	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
		assert(pIndiInfoData != NULL);
		if(pIndiInfoData->IsTheBeginning())
			indiInfoList.AddTail(pIndiInfoData->GetIGraphInfo());
	}

	return (indiInfoList.GetCount() > 0);
}



bool CIndiInfoDataList::GetIndiInfoIntheSave(CList<IGraphInfo*, IGraphInfo*>& indiInfoList) const
{
	indiInfoList.RemoveAll();

	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
		if( pIndiInfoData != NULL )
			indiInfoList.AddTail(pIndiInfoData->GetIGraphInfo());
	}

	return (indiInfoList.GetCount() > 0);
}

//sy 2006.04.24.
bool CIndiInfoDataList::IsIndiInfoData( const char *p_szGraphName) const
{
	if( !p_szGraphName) return false;
	if( !*p_szGraphName) return false;

	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
		if(pIndiInfoData == NULL)
			continue;

		if( pIndiInfoData->GetGraphName() == p_szGraphName)
			return true;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphEtcData

CGraphEtcData::CGraphEtcData():
	m_bIsVertScaleLogType(false), 
	m_bIsVertScaleInvertType(false)
{
}

CGraphEtcData::CGraphEtcData(const CString& strFunctionName, const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType):
	m_strFunctionName(strFunctionName), 
	m_bIsVertScaleLogType(bIsVertScaleLogType), 
	m_bIsVertScaleInvertType(bIsVertScaleInvertType)
{
}

// public =====================================================================
void CGraphEtcData::SetFunctionName(const CString& strFunctionName)
{
	m_strFunctionName = strFunctionName;
}

void CGraphEtcData::SetVertScaleLogType(const bool bIsVertScaleLogType)
{
	m_bIsVertScaleLogType = bIsVertScaleLogType;
}

void CGraphEtcData::SetVertScaleInvertType(const bool bIsVertScaleInvertType)
{
	m_bIsVertScaleInvertType = bIsVertScaleInvertType;
}

// ----------------------------------------------------------------------------
CString CGraphEtcData::GetFunctionName() const
{
	return m_strFunctionName;
}

bool CGraphEtcData::IsVertScaleLogType() const
{
	return m_bIsVertScaleLogType;
}

bool CGraphEtcData::IsVertScaleInvertType() const
{
	return m_bIsVertScaleInvertType;
}


///////////////////////////////////////////////////////////////////////////////
// class COrgViewData

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Explain		: 
//-----------------------------------------------------------------------------
COrgViewData::COrgViewData()	// : m_bAllHiding_PriceMA(false)
{

}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Explain		: m_indiInfoDataList�� ��� �����Ѵ�.
//-----------------------------------------------------------------------------
COrgViewData::~COrgViewData()
{

}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Return  void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Param  const bool bTheBeginning  : 
// Explain		: m_indiInfoDataList�� p_pIGraphInfo�� �߰����ش�.
//-----------------------------------------------------------------------------
bool COrgViewData::AddTail( IGraphInfo *p_pIGraphInfo, const bool bTheBeginning )
{
	// ���� ��ǥ�� ���� ��츸 �߰��Ѵ�.
	return m_indiInfoDataList.AddTail(p_pIGraphInfo, bTheBeginning);
}

//sy 2006.04.24.
void COrgViewData::Initialize()
{
	m_indiInfoDataList.DeleteAll();
}
//sy end

//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Return  bool : 
// Param  IGraphInfoManager *p_pIGraphInfoManager   : 
// Explain		: ����� �ִ� IGraphInfo ���� IGraphInfoManager�� �����ϱ�.
//-----------------------------------------------------------------------------
bool COrgViewData::ChangeAllIndicatorList( IGraphInfoManager *p_pIGraphInfoManager)
{
	return m_indiInfoDataList.ChangeAllIndicatorList(p_pIGraphInfoManager);
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Return  bool : 
// Param  CList< IGraphInfo *, IGraphInfo * >& indiInfoList : 
// Explain		: ó�� ȭ�鿡 road �Ǿ� �ִ� IndiInfo ���� ��������.
//-----------------------------------------------------------------------------
bool COrgViewData::GetIndiInfoInTheBeginning( CList< IGraphInfo*, IGraphInfo*>& indiInfoList ) const
{
	return m_indiInfoDataList.GetIndiInfoInTheBeginning(indiInfoList);
}


bool COrgViewData::GetIndiInfoIntheSave( CList< IGraphInfo*, IGraphInfo*>& indiInfoList ) const
{
	return m_indiInfoDataList.GetIndiInfoIntheSave(indiInfoList);
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Return  bool : 
// Param  const IGraphInfo* p_pIGraphInfo  : 
// Explain		: OrgViewDataŬ������ �ش� IndicatorInfo���� �ִ��� Ȯ���Ѵ�.
//				: �ߺ��ؼ� Info���� �߰��ϴ°��� ���� �ϱ� ����.
//-----------------------------------------------------------------------------
bool COrgViewData::IsIndicatorInfo( IGraphInfo *p_pIGraphInfo )
{
	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	
	while(pos)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
	
		ILPCSTR szGraphName = p_pIGraphInfo->GetGraphNameString();
		if( pIndiInfoData->GetIndicatorName() == szGraphName) return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Return  CGraphEtcData& : 
// Param  void	: 
// Explain		: m_graphEtcData�� return�Ѵ�.
//-----------------------------------------------------------------------------
CGraphEtcData& COrgViewData::GetGraphEtcData()
{
	return m_graphEtcData;
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Return  CIndiInfoDataList& : 
// Param  void	: 
// Explain		: �ʱⰪ�� ������ m_indiInfoDataList�� return�Ѵ�.
//-----------------------------------------------------------------------------
CIndiInfoDataList& COrgViewData::GetIndiInfoDataList()
{
	return m_indiInfoDataList;
}


// ----------------------------------------------------------------------------
/*void COrgViewData::SetAllHiding_PriceMA(const bool bAllHiding_PriceMA)
{
	m_bAllHiding_PriceMA = bAllHiding_PriceMA;
}

// ----------------------------------------------------------------------------
bool COrgViewData::IsAllHiding_PriceMA() const
{
	return m_bAllHiding_PriceMA;
}*/