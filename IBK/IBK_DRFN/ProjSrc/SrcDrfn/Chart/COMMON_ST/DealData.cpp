// DealData.cpp: implementation of the CDealData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealData.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STControllerDef2.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealData::CDealData()
{
	Clear();
}

CDealData::~CDealData()
{
}

CDealData::CDealData(const CDealData& rhs) 
{
	m_nIndex		= rhs.m_nIndex;
	m_lDealValue	= rhs.m_lDealValue;
	m_lType			= rhs.m_lType;
	m_dPrice		= rhs.m_dPrice;
	m_dContract		= rhs.m_dContract;
	m_strDealName	= rhs.m_strDealName;
	m_strListEntryNameForExit.RemoveAll();
	m_strListEntryNameForExit.AddTail((CStringList*)&rhs.m_strListEntryNameForExit);
}

CDealData CDealData::operator= (const CDealData& rhs)
{
	m_nIndex		= rhs.m_nIndex;
	m_lDealValue	= rhs.m_lDealValue;
	m_lType			= rhs.m_lType;
	m_dPrice		= rhs.m_dPrice;
	m_dContract		= rhs.m_dContract;
	m_strDealName	= rhs.m_strDealName;
	m_strListEntryNameForExit.RemoveAll();
	m_strListEntryNameForExit.AddTail((CStringList*)&rhs.m_strListEntryNameForExit);

	return *this;
}

CDealData CDealData::operator| (const CDealData& rhs)
{
	CDealData dealData;

	// �� �߿� �ϳ��� ��ȣ�� ���
	if(m_lDealValue == 0 || rhs.m_lDealValue == 0)
	{
		dealData.m_lDealValue = m_lDealValue | rhs.m_lDealValue;
	}
	// ���� ���� ��ȣ�� ���
	else if(m_lDealValue == rhs.m_lDealValue)
	{
		dealData.m_lDealValue = m_lDealValue;
	}
	// ���� ���� �ż�/�ŵ�û�� ��ȣ�� ���
	else if((m_lDealValue & VALENTRYBUY && rhs.m_lDealValue & VALEXITSHORT) ||
			(m_lDealValue & VALEXITSHORT && rhs.m_lDealValue & VALENTRYBUY))
	{
		dealData.m_lDealValue = VALBUY;			// �ż�/�ŵ�û�� ����
	}
	// ���� ���� �ŵ�/�ż�û�� ��ȣ�� ���
	else if((m_lDealValue & VALENTRYSELL && rhs.m_lDealValue & VALEXITLONG) ||
			(m_lDealValue & VALEXITLONG && rhs.m_lDealValue & VALENTRYSELL))
	{
		dealData.m_lDealValue = VALSELL;		// �ŵ�/�ż�û�� ����
	}
	//----------------------------------------------------------------------------------------------
	// vntsorl_20080211 (��ȣ ���͸� ����)
	// ���� ���� �ż�/�ż�û�� or �ŵ�/�ŵ�û�� ��ȣ�� ���
	else if(((m_lDealValue & VALENTRYBUY && rhs.m_lDealValue & VALEXITLONG) ||
			 (m_lDealValue & VALEXITLONG && rhs.m_lDealValue & VALENTRYBUY)) ||		// �ż�/�ż�û��
			((m_lDealValue & VALENTRYSELL && rhs.m_lDealValue & VALEXITSHORT) ||
			 (m_lDealValue & VALEXITSHORT && rhs.m_lDealValue & VALENTRYSELL)))		// �ŵ�/�ŵ�û��
	{
		dealData.m_lDealValue = 0L;				// ��ȣ����
	}
	else										// �̿��� ���
	{
		dealData.m_lDealValue = m_lDealValue | rhs.m_lDealValue;
	}
	//----------------------------------------------------------------------------------------------

	// �� �߿� ū ���� ���� ������ ����
	if(m_dContract > rhs.m_dContract)
		dealData.m_dContract = m_dContract;
	else
		dealData.m_dContract = rhs.m_dContract;

	// �� �߿� ū ���� ���� ������ ����
	if(m_dPrice > rhs.m_dPrice)
	{
		dealData.m_dPrice = m_dPrice;
		dealData.m_lType = m_lType;
	}
	else
	{
		dealData.m_dPrice = rhs.m_dPrice;
		dealData.m_lType = rhs.m_lType;
	}	
	
	long lLengthLeft = m_strDealName.GetLength();
	long lLengthRight = rhs.m_strDealName.GetLength();
	// ���� �� �ִ� ���
	if(lLengthLeft && lLengthRight)
		dealData.m_strDealName = m_strDealName + "|" + rhs.m_strDealName;
	// �� �߿� �ϳ��� �ִ� ���
	else if(lLengthLeft)
		dealData.m_strDealName = m_strDealName;
	else 
		dealData.m_strDealName = rhs.m_strDealName;

	dealData.m_strListEntryNameForExit.RemoveAll();
	dealData.m_strListEntryNameForExit.AddTail(&m_strListEntryNameForExit);

	CString strEntryNameForExit;
	POSITION posLeft;
	POSITION posRight = rhs.m_strListEntryNameForExit.GetHeadPosition();
	while(posRight)
	{
		strEntryNameForExit = rhs.m_strListEntryNameForExit.GetNext(posRight);
		posLeft = m_strListEntryNameForExit.Find(strEntryNameForExit);
		if(posLeft == NULL)
			dealData.m_strListEntryNameForExit.AddTail(strEntryNameForExit);			
	}
	
	return dealData;
}

CDealData CDealData::operator& (const CDealData& rhs)
{
	CDealData dealData;

	// ���� ����� ��ȣ ����
	dealData.m_lDealValue = m_lDealValue & rhs.m_lDealValue;
	if(dealData.m_lDealValue)
	{
		// �� �߿� ū ���� ���� ������ ����
		if(m_dContract > rhs.m_dContract)
			dealData.m_dContract = rhs.m_dContract;
		else
			dealData.m_dContract = m_dContract;

		// �� �߿� ū ���� ���� ������ ����
		if(m_dPrice > rhs.m_dPrice)
		{
			dealData.m_dPrice = rhs.m_dPrice;	
			dealData.m_lType = rhs.m_lType;
		}
		else
		{
			dealData.m_dPrice = m_dPrice;		
			dealData.m_lType = m_lType;
		}

		long lLengthLeft = m_strDealName.GetLength();
		long lLengthRight = rhs.m_strDealName.GetLength();
		// ���� �� �ִ� ���
		if(lLengthLeft && lLengthRight)
			dealData.m_strDealName = m_strDealName + "&" + rhs.m_strDealName;
		// �� �߿� �ϳ��� �ִ� ���
		else if(lLengthLeft)
			dealData.m_strDealName = m_strDealName;
		else 
			dealData.m_strDealName = rhs.m_strDealName;

		dealData.m_strListEntryNameForExit.RemoveAll();
		POSITION posLeftHead = m_strListEntryNameForExit.GetHeadPosition();
		POSITION posRightHead = rhs.m_strListEntryNameForExit.GetHeadPosition();
		if(posLeftHead != NULL && posRightHead != NULL)
		{
			POSITION posLeft = posLeftHead;
			POSITION posRight;
			CString strLeft;
			CString strRight;
			while(posLeft)
			{
				strLeft = m_strListEntryNameForExit.GetNext(posLeft);
				posRight = rhs.m_strListEntryNameForExit.Find(strLeft);
				if(posRight)
					dealData.m_strListEntryNameForExit.AddTail(strLeft);
			}
		}
	}
	
	return dealData;
}

void CDealData::Clear()
{
	m_nIndex		= -1;
	m_lDealValue	= 0;
	m_lType			= 0;
	m_dPrice		= 0.;
	m_dContract		= 0.;
	m_strDealName.Empty();
	m_strListEntryNameForExit.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// CDealTraceBase

CDealTraceBase::CDealTraceBase()
{
	InitDealData();
}

CDealTraceBase::~CDealTraceBase()
{
}

BOOL CDealTraceBase::IsNonePosition()
{
	return m_DealDataRaw.m_lDealValue == 0L;
}

BOOL CDealTraceBase::IsNonePosition(long lDealValue)
{
	return lDealValue == 0L;
}

BOOL CDealTraceBase::IsBuy(long lDealValue)				// �ż� ��뿩��
{
	return lDealValue & VALENTRYBUY;
}

BOOL CDealTraceBase::IsExitShort(long lDealValue)		// �ŵ�û�� ��뿩��
{
	return lDealValue & VALEXITSHORT;
}

BOOL CDealTraceBase::IsSell(long lDealValue)			// �ŵ� ��뿩��
{
	return lDealValue & VALENTRYSELL;
}

BOOL CDealTraceBase::IsExitLong(long lDealValue)		// �ż�û�� ��뿩��
{
	return lDealValue & VALEXITLONG;
}

BOOL CDealTraceBase::IsClearStateOfRaw()
{
	BOOL bRawState = FALSE;
	if(m_DealDataRaw.m_nIndex == -1 && m_DealDataRaw.m_lDealValue == 0 && m_DealDataRaw.m_lType == 0 && 
		m_DealDataRaw.m_dPrice == 0. && m_DealDataRaw.m_strDealName.IsEmpty() && 
		m_DealDataRaw.m_strListEntryNameForExit.IsEmpty())
		bRawState = TRUE;

	return bRawState;
}

BOOL CDealTraceBase::IsClearStateOfCurr()
{
	BOOL bCurrState = FALSE;
	if(m_DealDataCurr.m_nIndex == -1 && m_DealDataCurr.m_lDealValue == 0 && m_DealDataCurr.m_lType == 0 && 
		m_DealDataCurr.m_dPrice == 0. && m_DealDataCurr.m_strDealName.IsEmpty() && 
		m_DealDataCurr.m_strListEntryNameForExit.IsEmpty())
		bCurrState = TRUE;

	return bCurrState;
}

BOOL CDealTraceBase::IsClearStateOfAll()
{
	return (IsClearStateOfRaw() && IsClearStateOfCurr());
}

BOOL CDealTraceBase::IsSameDealData(const CDealData* pDealData)
{
	BOOL bIsSameDealData = FALSE;
	if(m_DealDataRaw.m_nIndex == pDealData->m_nIndex && 
		m_DealDataRaw.m_lDealValue == pDealData->m_lDealValue &&
		m_DealDataRaw.m_dPrice == pDealData->m_dPrice &&
		m_DealDataRaw.m_strDealName == pDealData->m_strDealName)
		bIsSameDealData = TRUE;

	return bIsSameDealData;
}

int CDealTraceBase::GetRawIndex()
{
	return m_DealDataRaw.m_nIndex;
}

long CDealTraceBase::GetRawDealValue()
{
	return m_DealDataRaw.m_lDealValue;
}

long CDealTraceBase::GetRawType()
{
	return m_DealDataRaw.m_lType;
}

CString CDealTraceBase::GetRawDealName()
{
	return m_DealDataRaw.m_strDealName;
}

double CDealTraceBase::GetRawPrice()
{
	return m_DealDataRaw.m_dPrice;
}

double CDealTraceBase::GetRawContract()
{
	return m_DealDataRaw.m_dContract;
}

CStringList* CDealTraceBase::GetRawEntryNameForExit()
{
	return &m_DealDataRaw.m_strListEntryNameForExit;
}

int CDealTraceBase::GetCurrIndex()
{
	return m_DealDataCurr.m_nIndex;
}

long CDealTraceBase::GetCurrDealValue()
{
	return m_DealDataCurr.m_lDealValue;
}

long CDealTraceBase::GetCurrType()
{
	return m_DealDataCurr.m_lType;
}

CString CDealTraceBase::GetCurrDealName()
{
	return m_DealDataCurr.m_strDealName;
}

double CDealTraceBase::GetCurrPrice()
{
	return m_DealDataCurr.m_dPrice;
}

double CDealTraceBase::GetCurrContract()
{
	return m_DealDataCurr.m_dContract;
}

CStringList* CDealTraceBase::GetCurrEntryNameForExit()
{
	return &m_DealDataCurr.m_strListEntryNameForExit;
}

int CDealTraceBase::GetOrdrIndex()
{
	return m_DealDataOrdr.m_nIndex;
}

long CDealTraceBase::GetOrdrDealValue()
{
	return m_DealDataOrdr.m_lDealValue;
}

long CDealTraceBase::GetOrdrType()
{
	return m_DealDataOrdr.m_lType;
}

CString CDealTraceBase::GetOrdrDealName()
{
	return m_DealDataOrdr.m_strDealName;
}

double CDealTraceBase::GetOrdrPrice()
{
	return m_DealDataOrdr.m_dPrice;
}

double CDealTraceBase::GetOrdrContract()
{
	return m_DealDataOrdr.m_dContract;
}

CStringList* CDealTraceBase::GetOrdrEntryNameForExit()
{
	return &m_DealDataOrdr.m_strListEntryNameForExit;
}

//void CDealTraceBase::InitRepeatCount()
//{
//	m_nRepeatCount = 0;
//}
//
//void CDealTraceBase::SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState/* = FALSE*/)
//{
//	// ��ȸ or ������ ��� ���Ǹ�����
//	if(bRealState == FALSE || (bRealState && GetTimingOrder() == 0))
//	{
//		m_DealDataRaw.m_nIndex = nIndex;
//		m_DealDataRaw.m_lDealValue = FORCEDEXIT | lDealValue;
//	}
//	// ������ ��� ���ϼ���
//	else if(bRealState && GetTimingOrder() == 1)
//	{
//		m_DealDataCurr.m_nIndex = nIndex;
//		m_DealDataCurr.m_lDealValue = FORCEDEXIT | lDealValue;
//	}
//}

void CDealTraceBase::InitDealData()
{
	m_DealDataRaw.Clear();
	m_DealDataCurr.Clear();
	m_DealDataOrdr.Clear();

	m_nUseRepeatEntry = 0;
	m_nRepeatEntryCount = 1;
//	m_nRepeatCount = 0;

	m_nTiming_Order = 1;
	m_nTiming_ForcedLiquidation = 1;
}

void CDealTraceBase::SetRepeatInfo(int nUseRepeatEntry/* = 0*/, int nRepeatEntryCount/* = 1*/)
{
	// ��ȸó�� ������ ����
	m_nUseRepeatEntry = nUseRepeatEntry;

	if(m_nUseRepeatEntry)
		m_nRepeatEntryCount = nRepeatEntryCount;
	else
		m_nRepeatEntryCount = 1;

//	InitRepeatCount();
}

//BOOL CDealTraceBase::IncreaseRepeatCount()
//{
//	if(LessThanRepeatEntryCount())
//	{
//		++m_nRepeatCount;
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
void CDealTraceBase::SetTimingInfo(int nTiming_Order/* = 1*/, int nTiming_ForcedLiquidation/* = 1*/)
{
	// ��ȸó�� �� �ǽð�ó�� ������ ����
	m_nTiming_Order = nTiming_Order;
	m_nTiming_ForcedLiquidation = nTiming_ForcedLiquidation;
}

int CDealTraceBase::GetTimingOrder()
{
	return m_nTiming_Order;
}

int CDealTraceBase::GetTimingForcedLiquidation()
{
	return m_nTiming_ForcedLiquidation;
}

BOOL CDealTraceBase::Set(const CDealData* pDealData)
{
	if(pDealData == NULL)
		return FALSE;

	// ����
	m_DealDataRaw = *pDealData;
	return TRUE;
}

void CDealTraceBase::Copy()
{
	m_DealDataCurr = m_DealDataRaw;
	m_DealDataRaw.Clear();
}

void CDealTraceBase::Next()
{
	if(IsNonePosition() == FALSE)
		m_DealDataCurr = m_DealDataRaw;

	m_DealDataRaw.Clear();
}

//BOOL CDealTraceBase::LessThanRepeatEntryCount()
//{
//	return (m_nRepeatCount < m_nRepeatEntryCount);
//}
//
BOOL CDealTraceBase::IsEntryPosition()
{
	ASSERT(FALSE);
	return FALSE;
}

BOOL CDealTraceBase::IsExitPosition()
{
	ASSERT(FALSE);
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// CBuyDealTrace

CBuyDealTrace::CBuyDealTrace()
: CDealTraceBase()
{
	InitRepeatCount();
}

CBuyDealTrace::~CBuyDealTrace()
{
}

BOOL CBuyDealTrace::IsEntryPosition(long lDealValue)
{
	return IsBuy(lDealValue);
}

BOOL CBuyDealTrace::IsExitPosition(long lDealValue)
{
	return (IsExitLong(lDealValue) || IsForcedLiquidation(lDealValue));
}

BOOL CBuyDealTrace::Init(CDealData* pDealData)
{
	if(pDealData == NULL || pDealData->m_lDealValue == 0)
		return FALSE;

	if(IsClearStateOfAll())
	{
		if(IsExitLong(pDealData->m_lDealValue))
		{
			CDealTraceBase::Set(pDealData);
			m_DealDataRaw.m_lDealValue = VALEXITLONG;
		}
		else if(IsBuy(pDealData->m_lDealValue))
		{
			CDealTraceBase::Set(pDealData);
			m_DealDataRaw.m_lDealValue = VALENTRYBUY;
		}
		else
			return FALSE;
	}

	return TRUE;
}

BOOL CBuyDealTrace::Set(const CDealData* pDealData, BOOL bRealState/* = FALSE*/)
{
	if(pDealData == NULL)
		return FALSE;

	// ��ȸ or ������ ��� ���Ǹ�����
	if(bRealState == FALSE || (bRealState && GetTimingOrder() == 0))
	{
		CDealTraceBase::Set(pDealData);

		if(IsBuy(m_DealDataRaw.m_lDealValue))
			m_DealDataRaw.m_lDealValue = VALENTRYBUY;
		else if(IsExitLong(m_DealDataRaw.m_lDealValue))
			m_DealDataRaw.m_lDealValue = VALEXITLONG;

		if(IsExitLong(pDealData->m_lDealValue) || IsBuy(pDealData->m_lDealValue))
		{
			// ( Curr ������ ����û���� ���� �� ���� )
			// Curr : �ż�û�� or ����û�� / Raw : �ż�û��
			if(IsExitPosition(m_DealDataCurr.m_lDealValue) && IsExitLong(m_DealDataRaw.m_lDealValue))
			{
//				m_DealDataRaw.m_lDealValue = VALEXITLONG;
				m_DealDataRaw.m_lDealValue = 0l;
			}
			// Curr : �ż�û�� or ����û�� / Raw : �ż�
			else if(IsExitPosition(m_DealDataCurr.m_lDealValue) && IsBuy(m_DealDataRaw.m_lDealValue))
			{
				if(GetCurrIndex() != GetRawIndex())
				{
					if(IncreaseRepeatCount())
						m_DealDataRaw.m_lDealValue = VALENTRYBUY;
					else
						m_DealDataRaw.m_lDealValue = 0l;
				}
			}
			// Curr : �ż� / Raw : �ż�û��
			else if(IsEntryPosition(m_DealDataCurr.m_lDealValue) && IsExitLong(m_DealDataRaw.m_lDealValue))
			{
				m_DealDataRaw.m_lDealValue = VALEXITLONG;
				InitRepeatCount();
			}
			// Curr : �ż� / Raw : �ż�
			else if(IsEntryPosition(m_DealDataCurr.m_lDealValue) && IsBuy(m_DealDataRaw.m_lDealValue))
			{
				if(GetCurrIndex() != GetRawIndex())
				{
					if(IncreaseRepeatCount())
						m_DealDataRaw.m_lDealValue = VALENTRYBUY;
					else
						m_DealDataRaw.m_lDealValue = 0l;
				}
			}
		}
		else
		{
			m_DealDataRaw.m_lDealValue = 0l;
		}
	}
	// ������ ��� ���ϼ���
	else if(bRealState && GetTimingOrder() == 1)
	{
		CDealTraceBase::Set(pDealData);

		if(IsExitLong(pDealData->m_lDealValue))
		{
			m_DealDataRaw.m_lDealValue = VALEXITLONG;
		}
		else if(IsBuy(pDealData->m_lDealValue))
		{
			m_DealDataRaw.m_lDealValue = VALENTRYBUY;
		}
		else
		{
			m_DealDataRaw.m_lDealValue = 0l;
		}
	}

	return TRUE;
}

void CBuyDealTrace::Next(BOOL bRealState/* = FALSE*/)
{
	// ��ȸ or ������ ��� ���Ǹ�����
	if(bRealState == FALSE || (bRealState && GetTimingOrder() == 0))
	{
		CDealTraceBase::Next();

		// ���� Ordr ����
		if(m_DealDataOrdr.m_nIndex < 0 && m_DealDataCurr.m_nIndex >= 0)
		{
			if(IsExitPosition(m_DealDataCurr.m_lDealValue) || IsEntryPosition(m_DealDataCurr.m_lDealValue))
			{
				m_DealDataOrdr = m_DealDataCurr;
			}
		}
		// ( Ordr / Curr ������ ����û���� ���� �� ���� )
		// Ordr : �ż�û�� or ����û�� / Curr : �ż�û�� or ����û��
		else if(IsExitPosition(m_DealDataOrdr.m_lDealValue) && IsExitPosition(m_DealDataCurr.m_lDealValue))
		{
		}
		// Ordr : �ż�û�� or ����û�� / Curr : �ż�
		else if(IsExitPosition(m_DealDataOrdr.m_lDealValue) && IsBuy(m_DealDataCurr.m_lDealValue))
		{
			if(m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex)
				m_DealDataOrdr = m_DealDataCurr;
		}
		// Ordr : �ż� / Curr : �ż�û�� or ����û��
		else if(IsEntryPosition(m_DealDataOrdr.m_lDealValue) && IsExitPosition(m_DealDataCurr.m_lDealValue))
		{
			if(m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex)
				m_DealDataOrdr = m_DealDataCurr;
		}
		// Ordr : �ż� / Curr : �ż�
		else if(IsEntryPosition(m_DealDataOrdr.m_lDealValue) && IsBuy(m_DealDataCurr.m_lDealValue))
		{
			if(m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex)
				m_DealDataOrdr = m_DealDataCurr;
		}
	}
	// ������ ��� ���ϼ���
	else if(bRealState && GetTimingOrder() == 1)
	{
		// ���� Ordr ����
		if(m_DealDataOrdr.m_nIndex < 0 && m_DealDataCurr.m_nIndex >= 0)
		{
			if(IsExitPosition(m_DealDataCurr.m_lDealValue) || IsEntryPosition(m_DealDataCurr.m_lDealValue))
			{
				m_DealDataOrdr = m_DealDataCurr;
			}
		}
		// ( Ordr / Curr ������ ����û���� ���� �� ���� )
		// Ordr : �ż�û�� or ����û�� / Curr : �ż�û�� or ����û��
		else if(IsExitPosition(m_DealDataOrdr.m_lDealValue) && IsExitPosition(m_DealDataCurr.m_lDealValue))
		{
			m_DealDataCurr.m_lDealValue = 0l;
		}
		// Ordr : �ż�û�� or ����û�� / Curr : �ż�
		else if(IsExitPosition(m_DealDataOrdr.m_lDealValue) && IsBuy(m_DealDataCurr.m_lDealValue))
		{
			if((m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex) && IncreaseRepeatCount())
				m_DealDataOrdr = m_DealDataCurr;
		}
		// Ordr : �ż� / Curr : �ż�û�� or ����û��
		else if(IsEntryPosition(m_DealDataOrdr.m_lDealValue) && IsExitPosition(m_DealDataCurr.m_lDealValue))
		{
			InitRepeatCount();

			if(m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex)
				m_DealDataOrdr = m_DealDataCurr;
		}
		// Ordr : �ż� / Curr : �ż�
		else if(IsEntryPosition(m_DealDataOrdr.m_lDealValue) && IsBuy(m_DealDataCurr.m_lDealValue))
		{
			if((m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex) && IncreaseRepeatCount())
				m_DealDataOrdr = m_DealDataCurr;
		}

		CDealTraceBase::Next();
	}
}

BOOL CBuyDealTrace::IsEntryPosition()
{
	return IsBuy(m_DealDataRaw.m_lDealValue);
}

BOOL CBuyDealTrace::IsExitPosition()
{
	return (IsExitLong(m_DealDataRaw.m_lDealValue) || IsForcedLiquidation(m_DealDataRaw.m_lDealValue));
}

BOOL CBuyDealTrace::IsForcedLiquidation(long lDealType)
{
	if( lDealType & FL_BUY_LOSSRATE				||
		lDealType & FL_BUY_TARGETBENEFIT		||
		lDealType & FL_BUY_HPRICECPBEAR			||
		lDealType & FL_BUY_TARGETVALUEPRESERVE	||
		lDealType & FL_BUY_BENEFITPRESERVE		||
		lDealType & FL_BUY_MINIPRICE			||
		lDealType & FL_BUY_DAYLIQUIDATION )
		return TRUE;

	return FALSE;
}

void CBuyDealTrace::SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState/* = FALSE*/)
{
	// ��ȸ or ������ ��� ���Ǹ�����
	if(bRealState == FALSE || (bRealState && GetTimingOrder() == 0))
	{
		m_DealDataRaw.m_nIndex = nIndex;
		m_DealDataRaw.m_lDealValue &= ~VALEXITLONG;
		m_DealDataRaw.m_lDealValue |= (FORCEDEXIT | lDealValue);
	}
	// ������ ��� ���ϼ���
	else if(bRealState && GetTimingOrder() == 1)
	{
		m_DealDataCurr.m_nIndex = nIndex;
		m_DealDataCurr.m_lDealValue &= ~VALEXITLONG;
		m_DealDataCurr.m_lDealValue |= (FORCEDEXIT | lDealValue);
	}
}

void CBuyDealTrace::InitRepeatCount()
{
	m_nRepeatCount = 0;
}

BOOL CBuyDealTrace::IncreaseRepeatCount()
{
	if(LessThanRepeatEntryCount())
	{
		++m_nRepeatCount;
		return TRUE;
	}

	return FALSE;
}

BOOL CBuyDealTrace::LessThanRepeatEntryCount()
{
	return (m_nRepeatCount < m_nRepeatEntryCount);
}


//////////////////////////////////////////////////////////////////////////
// CSellDealData

CSellDealTrace::CSellDealTrace()
: CDealTraceBase()
{
	InitRepeatCount();
}

CSellDealTrace::~CSellDealTrace()
{
}

BOOL CSellDealTrace::IsEntryPosition(long lDealValue)
{
	return IsSell(lDealValue);
}

BOOL CSellDealTrace::IsExitPosition(long lDealValue)
{
	return (IsExitShort(lDealValue) || IsForcedLiquidation(lDealValue));
}

BOOL CSellDealTrace::Init(CDealData* pDealData)
{
	if(pDealData == NULL || pDealData->m_lDealValue == 0)
		return FALSE;

	if(IsClearStateOfAll())
	{
		if(IsExitShort(pDealData->m_lDealValue))
		{
			CDealTraceBase::Set(pDealData);
			m_DealDataRaw.m_lDealValue = VALEXITSHORT;
		}
		else if(IsSell(pDealData->m_lDealValue))
		{
			CDealTraceBase::Set(pDealData);
			m_DealDataRaw.m_lDealValue = VALENTRYSELL;
		}
		else
			return FALSE;
	}

	return TRUE;
}

BOOL CSellDealTrace::Set(const CDealData* pDealData, BOOL bRealState/* = FALSE*/)
{
	if(pDealData == NULL)
		return FALSE;

	// ��ȸ or ������ ��� ���Ǹ�����
	if(bRealState == FALSE || (bRealState && GetTimingOrder() == 0))
	{
		CDealTraceBase::Set(pDealData);

		if(IsSell(m_DealDataRaw.m_lDealValue))
			m_DealDataRaw.m_lDealValue = VALENTRYSELL;
		else if(IsExitShort(m_DealDataRaw.m_lDealValue))
			m_DealDataRaw.m_lDealValue = VALEXITSHORT;
		if(IsExitShort(pDealData->m_lDealValue) || IsSell(pDealData->m_lDealValue))
		{
			// ( Curr ������ ����û���� ���� �� ���� )
			// Curr : �ŵ�û�� or ����û�� / Raw : �ŵ�û��
			if(IsExitPosition(m_DealDataCurr.m_lDealValue) && IsExitShort(m_DealDataRaw.m_lDealValue))
			{
//				m_DealDataRaw.m_lDealValue = VALEXITSHORT;
				m_DealDataRaw.m_lDealValue = 0l;
			}
			// Curr : �ŵ�û�� or ����û�� / Raw : �ŵ�
			else if(IsExitPosition(m_DealDataCurr.m_lDealValue) && IsSell(m_DealDataRaw.m_lDealValue))
			{
				if(GetCurrIndex() != GetRawIndex())
				{
					if(IncreaseRepeatCount())
						m_DealDataRaw.m_lDealValue = VALENTRYSELL;
					else
						m_DealDataRaw.m_lDealValue = 0l;
				}
			}
			// Curr : �ŵ� / Raw : �ŵ�û��
			else if(IsSell(m_DealDataCurr.m_lDealValue) && IsExitShort(m_DealDataRaw.m_lDealValue))
			{
				m_DealDataRaw.m_lDealValue = VALEXITSHORT;
				InitRepeatCount();
			}
			// Curr : �ŵ� / Raw : �ŵ�
			else if(IsSell(m_DealDataCurr.m_lDealValue) && IsSell(m_DealDataRaw.m_lDealValue))
			{
				if(GetCurrIndex() != GetRawIndex())
				{
					if(IncreaseRepeatCount())
						m_DealDataRaw.m_lDealValue = VALENTRYSELL;
					else
						m_DealDataRaw.m_lDealValue = 0l;
				}
			}
		}
		else
		{
			m_DealDataRaw.m_lDealValue = 0l;
		}
	}
	// ������ ��� ���ϼ���
	else if(bRealState && GetTimingOrder() == 1)
	{
		CDealTraceBase::Set(pDealData);

		if(IsExitShort(pDealData->m_lDealValue))
		{
			m_DealDataRaw.m_lDealValue = VALEXITSHORT;
		}
		else if(IsSell(pDealData->m_lDealValue))
		{
			m_DealDataRaw.m_lDealValue = VALENTRYSELL;
		}
		else
		{
			m_DealDataRaw.m_lDealValue = 0l;
		}
	}
	
	return TRUE;
}

void CSellDealTrace::Next(BOOL bRealState/* = FALSE*/)
{
	// ��ȸ or ������ ��� ���Ǹ�����
	if(bRealState == FALSE || (bRealState && GetTimingOrder() == 0))
	{
		CDealTraceBase::Next();

		// ���� Ordr ����
		if(m_DealDataOrdr.m_nIndex < 0 && m_DealDataCurr.m_nIndex >= 0)
		{
			if(IsExitPosition(m_DealDataCurr.m_lDealValue) || IsEntryPosition(m_DealDataCurr.m_lDealValue))
			{
				m_DealDataOrdr = m_DealDataCurr;
			}
		}
		// ( Ordr / Curr ������ ����û���� ���� �� ���� )
		// Ordr : �ŵ�û�� or ����û�� / Curr : �ŵ�û�� or ����û��
		else if(IsExitPosition(m_DealDataOrdr.m_lDealValue) && IsExitPosition(m_DealDataCurr.m_lDealValue))
		{
		}
		// Ordr : �ŵ�û�� or ����û�� / Curr : �ŵ�
		else if(IsExitPosition(m_DealDataOrdr.m_lDealValue) && IsSell(m_DealDataCurr.m_lDealValue))
		{
			if(m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex)
				m_DealDataOrdr = m_DealDataCurr;
		}
		// Ordr : �ŵ� / Curr : �ŵ�û�� or ����û��
		else if(IsEntryPosition(m_DealDataOrdr.m_lDealValue) && IsExitPosition(m_DealDataCurr.m_lDealValue))
		{
			if(m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex)
				m_DealDataOrdr = m_DealDataCurr;
		}
		// Ordr : �ŵ� / Curr : �ŵ�
		else if(IsEntryPosition(m_DealDataOrdr.m_lDealValue) && IsSell(m_DealDataCurr.m_lDealValue))
		{
			if(m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex)
				m_DealDataOrdr = m_DealDataCurr;
		}
	}
	// ������ ��� ���ϼ���
	else if(bRealState && GetTimingOrder() == 1)
	{
		// ���� Ordr ����
		if(m_DealDataOrdr.m_nIndex < 0 && m_DealDataCurr.m_nIndex >= 0)
		{
			if(IsExitPosition(m_DealDataCurr.m_lDealValue) || IsEntryPosition(m_DealDataCurr.m_lDealValue))
			{
				m_DealDataOrdr = m_DealDataCurr;
			}
		}
		// ( Ordr / Curr ������ ����û���� ���� �� ���� )
		// Ordr : �ŵ�û�� or ����û�� / Curr : �ŵ�û�� or ����û��
		if(IsExitPosition(m_DealDataOrdr.m_lDealValue) && IsExitPosition(m_DealDataCurr.m_lDealValue))
		{
			m_DealDataCurr.m_lDealValue = 0l;
		}
		// Ordr : �ŵ�û�� or ����û�� / Curr : �ŵ�
		else if(IsExitPosition(m_DealDataOrdr.m_lDealValue) && IsSell(m_DealDataCurr.m_lDealValue))
		{
			if((m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex) && IncreaseRepeatCount())
				m_DealDataOrdr = m_DealDataCurr;
		}
		// Ordr : �ŵ� / Curr : �ŵ�û�� or ����û��
		else if(IsEntryPosition(m_DealDataOrdr.m_lDealValue) && IsExitPosition(m_DealDataCurr.m_lDealValue))
		{
			InitRepeatCount();

			if(m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex)
				m_DealDataOrdr = m_DealDataCurr;
		}
		// Ordr : �ŵ� / Curr : �ŵ�
		else if(IsEntryPosition(m_DealDataOrdr.m_lDealValue) && IsSell(m_DealDataCurr.m_lDealValue))
		{
			if((m_DealDataOrdr.m_nIndex != m_DealDataCurr.m_nIndex) && IncreaseRepeatCount())
				m_DealDataOrdr = m_DealDataCurr;
		}

		CDealTraceBase::Next();
	}
}

BOOL CSellDealTrace::IsEntryPosition()
{
	return IsSell(m_DealDataRaw.m_lDealValue);
}

BOOL CSellDealTrace::IsExitPosition()
{
	return (IsExitShort(m_DealDataRaw.m_lDealValue) || IsForcedLiquidation(m_DealDataRaw.m_lDealValue));
}

BOOL CSellDealTrace::IsForcedLiquidation(long lDealType)
{
	if( lDealType & FL_SELL_LOSSRATE			||
		lDealType & FL_SELL_TARGETBENEFIT		||
		lDealType & FL_SELL_HPRICECPBEAR		||
		lDealType & FL_SELL_TARGETVALUEPRESERVE	||
		lDealType & FL_SELL_BENEFITPRESERVE		||
		lDealType & FL_SELL_MINIPRICE			||
		lDealType & FL_SELL_DAYLIQUIDATION )
		return TRUE;

	return FALSE;
}

void CSellDealTrace::SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState/* = FALSE*/)
{
	// ��ȸ or ������ ��� ���Ǹ�����
	if(bRealState == FALSE || (bRealState && GetTimingOrder() == 0))
	{
		m_DealDataRaw.m_nIndex = nIndex;
		m_DealDataRaw.m_lDealValue &= ~VALEXITSHORT;
		m_DealDataRaw.m_lDealValue |= (FORCEDEXIT | lDealValue);
	}
	// ������ ��� ���ϼ���
	else if(bRealState && GetTimingOrder() == 1)
	{
		m_DealDataCurr.m_nIndex = nIndex;
		m_DealDataCurr.m_lDealValue &= ~VALEXITSHORT;
		m_DealDataCurr.m_lDealValue |= (FORCEDEXIT | lDealValue);
	}
}

void CSellDealTrace::InitRepeatCount()
{
	m_nRepeatCount = 0;
}

BOOL CSellDealTrace::IncreaseRepeatCount()
{
	if(LessThanRepeatEntryCount())
	{
		++m_nRepeatCount;
		return TRUE;
	}

	return FALSE;
}

BOOL CSellDealTrace::LessThanRepeatEntryCount()
{
	return (m_nRepeatCount < m_nRepeatEntryCount);
}

