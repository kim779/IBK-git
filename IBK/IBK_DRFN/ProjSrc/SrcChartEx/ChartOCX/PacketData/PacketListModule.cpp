// PacketListModule.cpp: implementation of the CPacketListModule class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketListModule.h"

#include <assert.h>
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "Conversion.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CRealDataModule - real data 

CRealDataModule::CRealDataModule( HWND p_hOcxWnd, const REALDATAMODULETYPE eRealDataModuleType)
{







	// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
	m_hOcxWnd = p_hOcxWnd;


	m_pRealDataModule = NULL;
	SetRealDataModuleType( eRealDataModuleType);
}

CRealDataModule::~CRealDataModule()
{







	delete m_pRealDataModule;
	m_pRealDataModule = NULL;
}





// (2009/10/20 - Seung-Won, Bae) Comment.
//	���� ����ϴ� �������� 1�̰ų� 0���� Ȯ���Ѵ�. (���� ����� �ǹ̰� ���� ���)
//	���� ��길�� ���� ���ΰ�? �׸��� ù RDC�� ER�� ������ Ȯ���ϸ� �� �ڴ� ������ ����?
CRealDataModule::REALDATAMODULETYPE CRealDataModule::GetMakeRealModuleType( const REALDATAMODULETYPE eRealDataModuleType, const CString& strData) const
{
	if( eRealDataModuleType == BASE_TYPE || strData.IsEmpty()) return BASE_TYPE;

	// ������:�ݿø��ڸ�:���������Ŷ��|������:�ݿø��ڸ�:���������Ŷ��|...
	CString strFristData = CDataConversion::GetLeftToTrimDataIndex(strData, '|');
	if(strFristData.IsEmpty())
		strFristData = strData;

	CRealRateData realRateData( m_hOcxWnd, strFristData);
	if( realRateData.DoesUsingRealRate())
		return RATE_TYPE;
	
	return BASE_TYPE;
}
//	Data�� �����ϴ� Module�� �籸���Ѵ�.
bool CRealDataModule::SetRealDataModuleType(const REALDATAMODULETYPE eRealDataModuleType)
{
	if( m_pRealDataModule)
	{
		if( m_pRealDataModule->GetRealDataModuleType() == eRealDataModuleType)
			return false;
		delete m_pRealDataModule;
	}

	if( eRealDataModuleType == RATE_TYPE)
			m_pRealDataModule = new CRateRealDataModule( m_hOcxWnd);
	else	m_pRealDataModule = new COrgRealDataModule();

	return true;
}
// (2009/10/20 - Seung-Won, Bae) Comment.
//	�ǽð� Data�� �����ϴ� ����� ������.
//		RATE_TYPE���θ� ȣ���.
//		
void CRealDataModule::SetRealDataModuleType( const REALDATAMODULETYPE eRealDataModuleType, const CString& strModuleHelpMsg)
{
	SetRealDataModuleType( GetMakeRealModuleType(eRealDataModuleType, strModuleHelpMsg));

	assert(m_pRealDataModule != NULL);
	m_pRealDataModule->SetHelpMessage(strModuleHelpMsg);
}
bool CRealDataModule::ChangeRealDataModuleAndData(const CString& strModuleHelpMsg)
{
	SetRealDataModuleType( GetMakeRealModuleType(RATE_TYPE, strModuleHelpMsg));

	assert(m_pRealDataModule != NULL);

	m_pRealDataModule->SetHelpMessage(strModuleHelpMsg, false);
	return true;
}












// ----------------------------------------------------------------------------
void CRealDataModule::ChangeExchangeRateData_Percent()
{
	if(m_pRealDataModule == NULL)
		return;

	m_pRealDataModule->ChangeExchangeRateData_Percent();
}

// ----------------------------------------------------------------------------
double CRealDataModule::GetRealData(const CString& strPacketName, const double& dData) const
{
	assert(m_pRealDataModule != NULL);

	return m_pRealDataModule->GetRealData(strPacketName, dData);
}

double CRealDataModule::GetRealData(const CString& strPacketName, const CString& strData) const
{
	return GetRealData(strPacketName, atof(strData));
}

// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
double CRealDataModule::GetOrgData(const CString& strPacketName, const double& dData) const
{
	assert(m_pRealDataModule != NULL);

	return m_pRealDataModule->GetOrgData(strPacketName, dData);
}

///////////////////////////////////////////////////////////////////////////////
//class COrgRealDataModule - org real ���� Module

CRealDataModule::REALDATAMODULETYPE COrgRealDataModule::GetRealDataModuleType() const
{
	return CRealDataModule::BASE_TYPE;
}

double COrgRealDataModule::GetRealData(const CString& /*strPacketName*/, const double& dData) const
{
	return dData;
}

// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
double COrgRealDataModule::GetOrgData(const CString& strPacketName, const double& dData) const
{
	return dData;
}

// ----------------------------------------------------------------------------
void COrgRealDataModule::SetHelpMessage(const CString& strData, const bool bIsRemove)
{
}

void COrgRealDataModule::ChangeExchangeRateData_Percent()
{
}


///////////////////////////////////////////////////////////////////////////////
// class CRateRealDataModule - ���� real ���� Module

CRateRealDataModule::CRateRealDataModule( HWND p_hOcxWnd)
{
	// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
	m_hOcxWnd = p_hOcxWnd;
}

CRateRealDataModule::~CRateRealDataModule()
{
	DeleteAllRealRateData();
}

CRealDataModule::REALDATAMODULETYPE CRateRealDataModule::GetRealDataModuleType() const
{
	return CRealDataModule::RATE_TYPE;
}

double CRateRealDataModule::GetRealData(const CString& strPacketName, const double& dData) const
{
	if(DoesOnlyOriginalRealData(strPacketName))
		return dData;

	POSITION pos = m_realReteDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CRealRateData* pRealRateData = m_realReteDataList.GetNext(pos);
		assert(pRealRateData != NULL);
		double dRateData = dData;
		if(pRealRateData->GetApplyDataInRealRate(strPacketName, dData, dRateData))
			return dRateData;
	}

	return dData;
}

// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
double CRateRealDataModule::GetOrgData(const CString& strPacketName, const double& dData) const
{
	if(DoesOnlyOriginalRealData(strPacketName))
		return dData;

	POSITION pos = m_realReteDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CRealRateData* pRealRateData = m_realReteDataList.GetNext(pos);
		assert(pRealRateData != NULL);
		double dRateData = dData;
		if(pRealRateData->GetOrgDataInRealRate(strPacketName, dData, dRateData))
			return dRateData;
	}

	return dData;
}

// ----------------------------------------------------------------------------
void CRateRealDataModule::SetHelpMessage(const CString& strData, const bool bIsRemove)
{
	if(bIsRemove)
		MakeRealRateData(strData);
	else
		// (2006/6/20 - Seung-Won, Bae) Kim Sun-Young Update! for Real data with RDC Help Message
		SetRealRateData(strData);
}

void CRateRealDataModule::ChangeExchangeRateData_Percent()
{
	POSITION pos = m_realReteDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CRealRateData* pRealRateData = m_realReteDataList.GetNext(pos);
		assert(pRealRateData != NULL);
		double dRealRate = pRealRateData->GetRealRate();
		if(dRealRate != 0.0)
			pRealRateData->SetRealRate(100.0/dRealRate);
	}
}


// private ====================================================================
void CRateRealDataModule::MakeRealRateData(const CString& strData)
{
	DeleteAllRealRateData();

	//������:�ݿø��ڸ�:���������Ŷ��|������:�ݿø��ڸ�:���������Ŷ��|...
	CString strTemp = strData;
	while(!strTemp.IsEmpty())
	{
		CString strRateData = GetRealRateData(strTemp, '|');
		if(strRateData.IsEmpty())
			continue;
		CRealRateData* pRealRateData = new CRealRateData( m_hOcxWnd, strRateData);
		m_realReteDataList.AddTail(pRealRateData);
	}
}

void CRateRealDataModule::DeleteAllRealRateData()
{
	POSITION pos = m_realReteDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CRealRateData* pRealRateData = m_realReteDataList.GetNext(pos);
		delete pRealRateData;
	}

	m_realReteDataList.RemoveAll();
}

// (2006/6/20 - Seung-Won, Bae) Kim Sun-Young Update! for Real data with RDC Help Message
void CRateRealDataModule::SetRealRateData(const CString& strData)
{
	if(m_realReteDataList.GetCount() > 0)
		SetRealRateData_PartChange(strData);
	else
		MakeRealRateData(strData);
}

void CRateRealDataModule::SetRealRateData_PartChange(const CString& strData)
{
	//������:�ݿø��ڸ�:���������Ŷ��|������:�ݿø��ڸ�:���������Ŷ��|...
	CString strTemp = strData;
	POSITION pos = m_realReteDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CRealRateData* pRealRateData = m_realReteDataList.GetNext(pos);
		assert(pRealRateData != NULL);

		CString strRateData = GetRealRateData(strTemp, '|');
		pRealRateData->SetRealRateData_PartChange(strRateData);
	}
}

// ----------------------------------------------------------------------------
bool CRateRealDataModule::DoesOnlyOriginalRealData(const CString& strPacketName) const
{
	if(strPacketName.IsEmpty())
		return true;

	//"�ڷ�����" �� ���ԵǴ� ���� ��ȯ���� �ʴ´�.
	return (strPacketName.Find( _MTEXT( C0_DATE_TIME)) >= 0);
}

CString CRateRealDataModule::GetRealRateData(CString& strAllData, const CString& strCompart) const
{
	CString strRateData = CDataConversion::GetStringData(strAllData, strCompart);
	if(strRateData.IsEmpty() && !strAllData.IsEmpty())
	{
		strRateData = strAllData;
		strAllData.Empty();
	}

	return strRateData;
}