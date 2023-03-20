// PacketBaseData.cpp: implementation of the CPacketBaseData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketBaseData.h"

#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for _STOM0()
#include "Conversion.h"
#include "DataMath.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CItem

CItem& CItem::operator=(const CItem& data)
{
	if(this == &data)
		return *this;

	m_strCode = data.GetCode();
	m_strName = data.GetName();
	return *this;
}

CItem::CItem(const CString& strCode, const CString& strName) :
	m_strCode(strCode),
	m_strName(strName)
{
}

void CItem::Clear()
{
	SetItem("", "");
}

void CItem::SetItem(const CString& strCode, const CString& strName)
{
	m_strCode = strCode;
	m_strName = strName;
}

CString CItem::GetCode() const
{
	return m_strCode;
}

CString CItem::GetName() const
{
	return m_strName;
}


///////////////////////////////////////////////////////////////////////////////
// class CDateData

CDateData::CDateData( void)
{
	// (2009/10/20 - Seung-Won, Bae) Comment. ������ ���ڸ� �����Ѵ�. (�ŷ��� ����)
	m_strPacketType = "YYYYMMDD";

	SetDate();
}

// (2009/10/20 - Seung-Won, Bae) Comment. PC�� ���� ���ڷ� �ʱ�ȭ�Ѵ�.
//		Contructor���� �⺻ ������.
//		�����޽��� ������ �ʱ�ȭ ó����.
//		���� ������ �������� ���� ���.
//			�����޽��� DATE ��.
//			�����޽��� DATATIME ��.
//			�����޽��� DATATIME ������ ���� �ŷ��� ���ڸ� �������� ������� ������.
//				�������� �����ð� 0�ø� �������� �ٲ��, �ŷ��� �ð����� ���߿� ����ȴ�.
//				�����ð��� ���������� �������� ��/��/������ ���� ������ �ȴ�.
void CDateData::SetDate( void)
{
	CTime time = CTime::GetCurrentTime();
	m_nDate = time.GetYear() * 10000 + time.GetMonth() * 100 + time.GetDay();

	SetPacketType( m_strPacketType);
}

// (2009/10/20 - Seung-Won, Bae) Comment. ������ ���������� ���ڷ� �ʱ�ȭ�Ѵ�.
//		�����޽��� DATE ��.
//		�����޽��� DATATIME ��.
//		�����޽��� DATATIME ������ ���� �ŷ��� ���ڸ� �������� ������� ������.
//				�������� �����ð� 0�ø� �������� �ٲ��, �ŷ��� �ð����� ���߿� ����ȴ�.
//				�����ð��� ���������� �������� ��/��/������ ���� ������ �ȴ�.
void CDateData::SetDate(const CString& strDate)
{
	if( strDate.IsEmpty() || strDate.GetLength() != 8) return;
	m_nDate = atol( strDate);

	SetPacketType( m_strPacketType);
}

// (2009/10/21 - Seung-Won, Bae) Comment. �ڷ����� PacketType�� �����޾� �ش� Format�� �´� Date�� ������ �����Ѵ�.
void CDateData::SetPacketType( const CString& strType)
{
	m_strPacketType = strType;

	bool bIsYear(false), bIsMonth(false), bIsDay(false);
	// "YYYY..." : "YYYYMM", "YYYYMMDD"
	bIsYear = ( 0 <= strType.Find("YYYY"));
	// "MMDD...", "YYYYMM..." : "YYYYMM", "YYYYMMDD", "MMDD", "MMDDHHSS"
	bIsMonth = ( 0 <= strType.Find("MMDD") || 0 <= strType.Find("YYYYMM"));
	// "...DD..." : "YYYYMMDD", "MMDD", "MMDDHHMMSS", "DDHHMMSS"
	// Packet Type�� ���� �Ͻø� ������ ���ؾ� �ϴ� �� (�ð��ڸ���)
	bIsDay = ( 0 <= strType.Find("DD"));

	// Packet Type�� ���� �Ͻø� ������ ���ؾ� �ϴ� �� (�ð��ڸ���)
	m_nPacketTypeTimeFactor = strType.Find( "HH");
	if( m_nPacketTypeTimeFactor < 0) m_nPacketTypeTimeFactor = 1;
	else m_nPacketTypeTimeFactor = ( int)pow( 10, strType.GetLength() - m_nPacketTypeTimeFactor);

	int nDigit = 0;
	m_nPacketTypedDate = 0;
	if( bIsYear)
	{
		m_nPacketTypedDate = m_nDate / 10000;
		nDigit += 4;
	}
	if( bIsMonth)
	{
		m_nPacketTypedDate *= 100;
		m_nPacketTypedDate += ( m_nDate % 10000) / 100;
		nDigit += 2;
	}
	if( bIsDay)
	{
		m_nPacketTypedDate *= 100;
		m_nPacketTypedDate += m_nDate % 100;
		nDigit += 2;
	}

	m_strPacketTypedDate.Empty();
	if( 0 < nDigit)
	{
		char szFormat[ 5] = "%00d";
		szFormat[ 2] += nDigit;
		m_strPacketTypedDate.Format( szFormat, m_nPacketTypedDate);
	}
}


///////////////////////////////////////////////////////////////////////////////
// class CTimeData

CTimeData& CTimeData::operator=(const CTimeData& data)
{
	if(this == &data)
		return *this;

	m_nTime = data.GetTimeFromServer();
	m_bTimeFromServer = data.IsTimeFromServer();
	return *this;
}

CTimeData::CTimeData(const int nTime, const bool bTimeFromServer):
	m_nTime(nTime), 
	m_bTimeFromServer(bTimeFromServer)
{
}

// public=============================================================
void CTimeData::SetTime(const int nTime, const bool bTimeFromServer)
{
	m_nTime = nTime;
	m_bTimeFromServer = bTimeFromServer;
}
	
// -------------------------------------------------------------------
bool CTimeData::IsTimeFromServer() const
{
	return m_bTimeFromServer;
}

int CTimeData::GetTimeFromServer() const
{
	return m_nTime;
}

bool CTimeData::GetTimeFromServer(int& nTime) const
{
	nTime = m_nTime;
	return m_bTimeFromServer;
}

bool CTimeData::GetTimeFromServer(CString& strTime) const
{
	//HHMMSS Ÿ������ ������.
	strTime.Format("%06d", m_nTime);

	return m_bTimeFromServer;
}


///////////////////////////////////////////////////////////////////////////////
// class CMarketTimeData

CMarketTimeData::CMarketTimeData() : m_startTime( 90000, false), m_endTime( 153000, false)
{
}

// (2009/10/20 - Seung-Won, Bae) ����۽ð��� �����Ѵ�.
void CMarketTimeData::SetMarketStartTime( const int nMarketStartTime, const bool bTimeFromServer)
{
	m_startTime.SetTime(nMarketStartTime, bTimeFromServer);
}

// (2009/10/20 - Seung-Won, Bae) �帶���ð��� �����Ѵ�.
void CMarketTimeData::SetMarketEndTime( const int nMarketEndTime, const bool bTimeFromServer)
{
	m_endTime.SetTime(nMarketEndTime, bTimeFromServer);
}

// �����/���� �ð��� ��������.
// return : true �� ���� ������ time�� ���� ��쿡 �ش�ȴ�.
bool CMarketTimeData::GetMarketTime( int& nMarketStartTime, int& nMarketEndTime) const
{
	// ��� �������� �� ��츸 true.
	bool bResult1 = m_startTime.GetTimeFromServer(nMarketStartTime);
	bool bResult2 = m_endTime.GetTimeFromServer(nMarketEndTime);
	return (bResult1 && bResult2);
}

// ����� �ð��� ���Ѵ�.
bool CMarketTimeData::GetMarketStartTime(CString& strMarketStartTime) const
{
	return m_startTime.GetTimeFromServer(strMarketStartTime);
}


///////////////////////////////////////////////////////////////////////////////
// class CRealRateData - real data �������� ���� data

CRealRateData& CRealRateData::operator=(const CRealRateData& data)
{
	if(this == &data)
		return *this;

	m_dRealRate = data.GetRealRate();
	m_nRoundIndex = data.GetRoundIndex();
	SetPacketNameList(data.GetPacketNameList());
	return *this;
}

CRealRateData::CRealRateData( HWND p_hOcxWnd, const CString& strData) :
	m_dRealRate(1.0), 
	m_nRoundIndex(0)
{
	// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
	m_hOcxWnd = p_hOcxWnd;

	SetRealRateData(strData);
}

// public =====================================================================
void CRealRateData::SetRealRateData(const CString& strData, const CString& strCompart)
{
	Initialize();

	SetRealRateData_PartChange(strData, strCompart);
}

void CRealRateData::SetRealRateData_PartChange(const CString& strData, const CString& strCompart)
{
	if(strData.IsEmpty())
		return;

	CString strTemp = strData;
	//������:�ݿø��ڸ�:���������Ŷ��
	SetRealRate(strTemp, strCompart);
	SetRountIndex(strTemp, strCompart);
	SetPacketNameList(strTemp, strCompart);
}

void CRealRateData::SetRealRate(const double dRealRate)
{
	m_dRealRate = dRealRate;
}

void CRealRateData::SetPacketNames(const CString& strPacketNames, const CString& strCompart)
{
	m_packetNameList.RemoveAll();
	CString strData = strPacketNames;
	SetPacketNameList(strData, strCompart);
}

// ----------------------------------------------------------------------------
bool CRealRateData::DoesUsingRealRate() const
{
	if( m_dRealRate == 1.0 || m_dRealRate == 0.0)
		return false;

	return (m_packetNameList.GetCount() > 0);
}

bool CRealRateData::DoesPacketName(const CString& strPacketName) const
{
	if(strPacketName.IsEmpty())
		return false;

	POSITION pos = m_packetNameList.GetHeadPosition();
	while(pos != NULL)
	{
		if(strPacketName == m_packetNameList.GetNext(pos))
			return true;
	}

	return false;
}

double CRealRateData::GetRealRate() const
{
	return m_dRealRate;
}

int CRealRateData::GetRoundIndex() const
{
	return m_nRoundIndex;
}

CList<CString, CString>* CRealRateData::GetPacketNameList() const
{
	return (const_cast<CList<CString, CString>*> (&m_packetNameList));
}

// ----------------------------------------------------------------------------
// ������ ������ data
// ����/����*100 : �Ҽ��� ROUNT_INDEX �ڸ����� �ݿø�
bool CRealRateData::GetApplyDataInRealRate(const CString& strPacketName, const double dOrgData, double& dNewData) const
{
	if(!DoesBringPacketNameList(strPacketName))
		return false;

	dNewData = GetApplyDataInRealRate(dOrgData);
	return true;
}

// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
bool CRealRateData::GetOrgDataInRealRate(const CString& strPacketName, const double dOrgData, double& dNewData) const
{
	if(!DoesBringPacketNameList(strPacketName))
		return false;

	dNewData = GetOrgDataInRealRate(dOrgData);
	return true;
}

// private ====================================================================
void CRealRateData::Initialize()
{
	m_dRealRate = 1.0;
	m_nRoundIndex = 0;
	m_packetNameList.RemoveAll();
}

// ----------------------------------------------------------------------------
bool CRealRateData::AddTail_PacketName(const CString& strPacketName)
{
	if(strPacketName.IsEmpty())
		return false;

	if(DoesBringPacketNameList(strPacketName))
		return false;

	// ������ �ȵ� ��츸 �߰� �Ѵ�.
	m_packetNameList.AddTail(strPacketName);
	return true;
}

// ----------------------------------------------------------------------------
void CRealRateData::SetRealRate(CString& strData, const CString& strCompart)
{
	m_dRealRate = 1.0;

	CString strPartData = GetData(strData, strCompart);
	if(!strPartData.IsEmpty())
		m_dRealRate = atof(strPartData);
}

void CRealRateData::SetRountIndex(CString& strData, const CString& strCompart)
{
	m_nRoundIndex = 0;

	CString strPartData = GetData(strData, strCompart);
	if(!strPartData.IsEmpty())
		m_nRoundIndex = atoi(strPartData);
}

void CRealRateData::SetPacketNameList(CString& strData, const CString& strCompart)
{
	CString strPartData = GetData(strData, strCompart);
	while(!strPartData.IsEmpty())
	{
		CString strPacketName = GetData(strPartData, ',');
		if(strPacketName.IsEmpty() && !strPartData.IsEmpty())
		{
			strPacketName = strPartData;
			strPartData.Empty();
		}

		strPacketName = _STOM0( strPacketName);
		AddTail_PacketName(strPacketName);
	}
}

void CRealRateData::SetPacketNameList(const CList<CString, CString>* pPacketNameList)
{
	m_packetNameList.RemoveAll();

	if(pPacketNameList == NULL)
		return;

	POSITION pos = pPacketNameList->GetHeadPosition();
	while(pos != NULL)
	{
		AddTail_PacketName(pPacketNameList->GetNext(pos));
	}
}

// ----------------------------------------------------------------------------
CString CRealRateData::GetData(CString& strData, const CString& strCompart) const
{
	if(strData.IsEmpty())
		return "";

	CString strPartData = CDataConversion::GetStringData(strData, strCompart);
	if(strPartData.IsEmpty() && !strData.IsEmpty())
	{
		strPartData = strData;
		strData.Empty();
	}

	return strPartData;
}

// ----------------------------------------------------------------------------
bool CRealRateData::DoesBringPacketNameList(const CString& strPacketName) const
{
	if(strPacketName.IsEmpty())
		return false;

	POSITION pos = m_packetNameList.GetHeadPosition();
	while(pos != NULL)
	{
		if(strPacketName == m_packetNameList.GetNext(pos))
			return true;
	}

	return false;
}

// ----------------------------------------------------------------------------
// ������ ������ data
// ����/����*100 : �Ҽ��� ROUNT_INDEX �ڸ����� �ݿø�
double CRealRateData::GetApplyDataInRealRate(const double dOrgData) const
{
	if(m_dRealRate == 0.0)
		return dOrgData;

	double dRealRateData = dOrgData * m_dRealRate;

	if(m_nRoundIndex < 0)
		return dRealRateData;

	return CMath::Round(dRealRateData, m_nRoundIndex);
}

CString CRealRateData::GetApplyDataInRealRate(const CString& strOrgData) const
{
	double dRealRateData = GetApplyDataInRealRate((double)atof(strOrgData));
	return CDataConversion::DoubleToString(dRealRateData);
}

CString CRealRateData::GetApplyDataInRealRate(const CString& strPacketName, const CString& strOrgData) const
{
	if(!DoesBringPacketNameList(strPacketName))
		return strOrgData;

	return GetApplyDataInRealRate(strOrgData);
}

// 2008.10.24 JS.Kim	Packet�� ����Ÿ�� �� ����Ÿ�� ����
double CRealRateData::GetOrgDataInRealRate(const double dRealRateData) const
{
	if(m_dRealRate == 0.0)
		return dRealRateData;

	return dRealRateData / m_dRealRate;
}


///////////////////////////////////////////////////////////////////////////////
// class CTRDataCountList - TR data �� ������ ������ list�� ���� (0 ���� Ŭ ��츸 ����)

// public =====================================================================
void CTRDataCountList::Initialize()
{
	m_nTotalCount = 0;
	m_dataCountList.RemoveAll();
}

bool CTRDataCountList::AddTRData(const int nCount)
{
	if(nCount <= 0)
		return false;

	m_dataCountList.AddTail(nCount);
	m_nTotalCount += nCount;
	return true;
}

// ----------------------------------------------------------------------------
int CTRDataCountList::GetTotalTRIndex() const
{
	return m_dataCountList.GetCount();
}

int CTRDataCountList::GetTotalTRCount() const
{
	return m_nTotalCount;
	/*int nTotalCount = 0;
	POSITION pos = m_dataCountList.GetHeadPosition();
	while(pos != NULL)
	{
		nTotalCount += m_dataCountList.GetNext(pos);
	}

	return nTotalCount;*/
}

int CTRDataCountList::GetTRCount(const int nIndex) const
{
	POSITION pos = m_dataCountList.FindIndex(nIndex);
	if(pos == NULL)
		return 0;

	return m_dataCountList.GetAt(pos);
}

int CTRDataCountList::GetTRCount_Tail() const
{
	POSITION pos = m_dataCountList.GetTailPosition();
	if(pos == NULL)
		return 0;

	return m_dataCountList.GetTail();
}

// ������� ���� TR data count �� ������ ���� TR data count.
// -> ��Ź��, �ݸ� ���� ���� ���� data �� ���� �� �ֱ⶧���� �⺻ TR data �������� 1�� ���� ��찡 �ִ�.
//	  �׷��Ƿ� "m_nTotalCount - nCurTRCount" �� �����ϸ� �ȵȴ�.
// -> ���� TR data ������ ���� ��� : m_nTotalCount
//	  ���� TR data ������ �ִ� ��� : ��ȸ�� TR index�� ���� �� index �� ������ TR data count.
int CTRDataCountList::GetRequestTRCount(const int nCurTRCount) const
{
	if(m_dataCountList.GetCount() <= 0)
		return 0;

	int nCurTRIndex = GetTRIndex(nCurTRCount);
	if(nCurTRIndex < 0)
		return m_nTotalCount;

	return (m_nTotalCount - GetTotalTRCount(nCurTRIndex));
}


// private ====================================================================
// �ش� TR data count �� ��ȸ�� �⺻ TR data count index.
int CTRDataCountList::GetTRIndex(const int nCurTRCount) const
{
	if(nCurTRCount <= 0 || m_dataCountList.GetCount() <= 0)
		return -1;

	int nTemp = nCurTRCount;
	int nIndex = 0;
	POSITION pos = m_dataCountList.GetHeadPosition();
	for( nIndex = 0; pos != NULL; nIndex++)
	{
		int nCount = m_dataCountList.GetNext(pos);
		nTemp = nTemp - nCount;
		if(nTemp <= 0)
			return nIndex;
	}

	return nIndex;
}

// �ش� index ������ TR data count.
int CTRDataCountList::GetTotalTRCount(const int nEndIndex) const
{
	int nTotalTRCount = 0;
	POSITION pos = m_dataCountList.GetHeadPosition();
	for(int nIndex = 0; nIndex <= nEndIndex; nIndex++)
	{
		if(pos == NULL)
			break;

		nTotalTRCount += m_dataCountList.GetNext(pos);
	}

	return nTotalTRCount;
}


///////////////////////////////////////////////////////////////////////////////
// class CItemData

// public =====================================================================
CItemData& CItemData::operator=(const CItemData& data)
{
	if(this == &data)
		return *this;

	m_bUsing = data.IsUsing();
	m_dData = data.GetData();
	return *this;
}

CItemData::CItemData(const bool bUsing, const double& dData) :
	m_bUsing(bUsing), 
	m_dData(dData)
{
}

// ----------------------------------------------------------------------------
void CItemData::Set(const bool bUsing, const double& dData)
{
	m_bUsing = bUsing;
	m_dData = dData;
}

void CItemData::SetUsing(const bool bUsing)
{
	m_bUsing = bUsing;
}

void CItemData::SetData(const double& dData)
{
	m_dData = dData;
}

bool CItemData::IsUsing() const
{
	return m_bUsing;
}

double CItemData::GetData() const
{
	return m_dData;
}
