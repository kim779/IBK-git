// DealDataItem.cpp: implementation of the CDealDataItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealDataItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealDataItem::CDealDataItem()
{
	m_nLastIndex		 = -1;
	m_lLastDealValue	 = 0;

	m_EnableContinuty	 = FALSE;
	m_lMaxContinuty		 = 0;
	m_lCountOfContinuty	 = 0;

	m_dMinimumChangeRate = 0.;
	m_dLastCurrentPrice	 = 0.;
}

CDealDataItem::~CDealDataItem()
{
// 	{
// 		CString szMsg;
// 		szMsg.Format("%s[%d]", __FILE__, __LINE__);
// 		OutputDebugString(szMsg);
// 	}

	Clear();
}

CDealData* CDealDataItem::SetData(int nIndex, long lDealValue, long lType /* = 0 */, LPCTSTR lpDealName /* = NULL */, double dPriceByCmd/* = 0. */, double dContract/* = 0. */, CStringList* pStListEntryNameForExit/* = NULL */)
{
	if(!lDealValue) 
	{
		// DealData�� DealValue�� 0������ ���õ� �� �����Ƿ� ������
		if(nIndex == m_nLastIndex)
			RemoveTail();
		return NULL;
	}

	// �ε����� �ش��ϴ� DealData�� ����
	CDealData* pDealData = GetDealDataByIndex(nIndex);
	if(pDealData)
	{
		pDealData->m_lDealValue |= lDealValue;
		pDealData->m_dPrice		= dPriceByCmd;
		pDealData->m_dContract	= dContract;	
		pDealData->m_lType		= lType;

		if(lpDealName && pDealData->m_strDealName.IsEmpty())
			pDealData->m_strDealName = lpDealName;
		if(pStListEntryNameForExit)
			pDealData->m_strListEntryNameForExit.AddTail(pStListEntryNameForExit);
		
		m_lLastDealValue		= pDealData->m_lDealValue;
		m_nLastIndex			= nIndex;
	}

	return pDealData;
}

void CDealDataItem::SetComment(LPCTSTR lpTitle, LPCTSTR lpComment)
{
	m_mapComment.SetAt(lpTitle, lpComment);
}

CString CDealDataItem::GetComment(LPCTSTR lpTitle)
{
	CString strComment;
	m_mapComment.Lookup(lpTitle, strComment);
	return strComment;
}

POSITION CDealDataItem::GetHeadPosition() const
{
	return m_listData.GetHeadPosition();
}

POSITION CDealDataItem::GetTailPosition() const
{
	return m_listData.GetTailPosition();
}

CDealData* CDealDataItem::GetHead()
{
	return m_listData.GetHead();
}

CDealData* CDealDataItem::GetTail()
{
	return m_listData.GetTail();
}

CDealData* CDealDataItem::GetNext(POSITION& pos)
{
	return m_listData.GetNext(pos);
}

CDealData* CDealDataItem::GetPrev(POSITION& pos)
{
	return m_listData.GetPrev(pos);
}

CDealData* CDealDataItem::GetDealDataByIndex(int nIndex)
{
	if(nIndex < 0)
		return NULL;

	// �ε����� ������ DealData�� ����Ű�� ��� ���� DealData ������ ����
	CDealData *pDealData = NULL;
	if(m_nLastIndex == nIndex)
	{
		pDealData = m_listData.GetTail();
	}
	// �ε����� �߰��Ǵ� DealData�� ����Ű�� �ϴ� ��� �� DealData �����Ͽ� ������ ����
	else if(m_nLastIndex < nIndex)
	{
		pDealData = new CDealData;
		pDealData->m_nIndex = nIndex;
		m_listData.AddTail(pDealData);
		m_nLastIndex = nIndex;
	}
	// �ε����� �迭 �߰��� DealData�� ī��Ű�� ��� �ε����� �ش��ϴ� DealData ������ ����
	else
	{
		POSITION pos = m_listData.GetTailPosition();
		while(pos)
		{
			pDealData = m_listData.GetPrev(pos);
			if(pDealData->m_nIndex == nIndex)
				break;
			else if(pDealData->m_nIndex < nIndex)
				return NULL;
		}		
	}
	return pDealData;	
}

void CDealDataItem::EnableContinuty(BOOL bEnable, long lMaxContinuty /* = 0 */) 
{
	m_EnableContinuty	= bEnable;
	m_lMaxContinuty		= lMaxContinuty;
}

void CDealDataItem::Clear()
{
// 	{
// 		CString szMsg;
// 		szMsg.Format("%s[%d]", __FILE__, __LINE__);
// 		OutputDebugString(szMsg);
// 	}

	CDealData *pDealData = NULL;
	POSITION pos = m_listData.GetHeadPosition();
	while(pos)
	{
		pDealData = m_listData.GetNext(pos);
		if(pDealData) delete pDealData;
	}

	m_listData.RemoveAll();
	m_mapComment.RemoveAll();
	m_nLastIndex = -1;
	m_lLastDealValue = 0;
}

void CDealDataItem::RemoveTail()
{
	int nCount = m_listData.GetCount();
	if(nCount == 0)
		return;

	CDealData* pDealData = NULL;
	if(pDealData = m_listData.RemoveTail())
		delete pDealData;

	if(nCount == 1)
	{
		Clear();
	}
	else
	{
		pDealData			= m_listData.GetTail();
		m_nLastIndex		= pDealData->m_nIndex;
		m_lLastDealValue	= pDealData->m_lDealValue;
		m_lCountOfContinuty = 0;

		POSITION pos = m_listData.GetTailPosition();
		while(pos)
		{
			pDealData = m_listData.GetPrev(pos);
			if(m_lLastDealValue == pDealData->m_lDealValue)
				m_lCountOfContinuty++;
		}
	}
}

int CDealDataItem::GetLastIndex()
{
	return m_nLastIndex;
}