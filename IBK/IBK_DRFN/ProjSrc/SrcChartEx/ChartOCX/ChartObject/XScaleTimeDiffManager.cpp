// XScaleTimeDiffManager.cpp: implementation of the CXScaleTimeDiffManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chartobject.h"
#include "XScaleTimeDiffManager.h"

#include "XScaleManagerImp.h"
#include "../Include_Chart/Conversion.h"

#include <time.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXScaleTimeDiffManager::CXScaleTimeDiffManager()
{
	m_pXScaleManagerImp	= NULL;

	m_tmScreenTimeDiff = 0;
}

CXScaleTimeDiffManager::CXScaleTimeDiffManager(CXScaleManagerImp* pXScaleManagerImp)
{
	m_pXScaleManagerImp	= pXScaleManagerImp;

	m_tmScreenTimeDiff = 0;
}

CXScaleTimeDiffManager::~CXScaleTimeDiffManager()
{

}


void CXScaleTimeDiffManager::SetXScaleManagerImp(CXScaleManagerImp* pXScaleManagerImp)
{
	m_pXScaleManagerImp	= pXScaleManagerImp;
}

// RQ�� ���ؽÿ��� ���� ����Ÿ �� �ʱ�ȭ
void CXScaleTimeDiffManager::InitRQDiffTime()
{
	m_mtdRQ.clear();
}

// RQ�� ����۽ð� �� �ʱ�ȭ
void CXScaleTimeDiffManager::InitRQMarketTime()
{
	m_mRQMarketTime.clear();
}

// RQ�� ���ؽÿ��� ���� �� ����ð� ����
void CXScaleTimeDiffManager::SetRQDiffTime(CString strRQ, int tmDiffTime, __int64 nCurrTime)
{
	if( strRQ.IsEmpty() )
		return;

	string stringRQ((LPSTR)(LPCTSTR)strRQ);

	m_mtdRQ.insert(PAIR_RQ_TIME_DIFF(stringRQ, tmDiffTime));

	if( nCurrTime > 0 )
	{
		tm currTime;
		memset(&currTime, 0, sizeof(tm));
		currTime.tm_isdst = -1;		//[A00000554]alzioyes:20110530 �ð����� �ʱⰪ����.
		currTime.tm_year =  (nCurrTime / 10000000000) - 1900;
		currTime.tm_mon =	(nCurrTime % 10000000000) / 100000000 - 1;
		currTime.tm_mday =	(nCurrTime % 100000000) / 1000000;
		currTime.tm_hour =	(nCurrTime % 1000000) / 10000;
		currTime.tm_mday =	(nCurrTime % 10000) / 100;
		currTime.tm_mday =	(nCurrTime % 100);

		m_mRQCurrTime.insert(PAIR_RQ_CURR_TIME(stringRQ, currTime));
	}
}

// RQ�� ����۽ð� ����
void CXScaleTimeDiffManager::SetRQMarketTime(CString strRQ, int nMarketStartTime, int nMarketEndTime, CPacketType::MARKETDAYTYPE nMarketDayType)
{
	if( strRQ.IsEmpty() )
		return;

	string stringRQ((LPSTR)(LPCTSTR)strRQ);

	/*
	if( nMarketStartTime <= nMarketEndTime )
		m_mRQMarketTime.insert(PAIR_RQ_TIME_MARKET(stringRQ, nMarketStartTime));
	else
		m_mRQMarketTime.insert(PAIR_RQ_TIME_MARKET(stringRQ, nMarketStartTime - 240000));		// �Ϸ���
	*/
	time_t	timeResult;
	timeResult = ( 3600 * (nMarketStartTime/10000) );
	timeResult += ( 60 * (nMarketStartTime%10000/100) );
	timeResult += nMarketStartTime%100;

	if( nMarketDayType == CPacketType::WITH_NEXT_DAY) timeResult -= ( 3600 * 24 );

	m_mRQMarketTime.insert(PAIR_RQ_TIME_MARKET(stringRQ, timeResult));
}

// ���� ȭ�鿡 �������� �ִ� �ð����� ���ؽÿ��� ������ ����
void CXScaleTimeDiffManager::SetScreenDiffTime(CString strRQ, int tmDiffTime)
{
	m_strScreenRQ = strRQ;
	m_tmScreenTimeDiff = tmDiffTime;
}

// ���� �������� ������ �Ǵ� RQ
void CXScaleTimeDiffManager::SetCurrentRQ(CString strRQ)
{
	string stringRQ((LPSTR)(LPCSTR)strRQ);
	MAP_RQ_TIME_DIFF::iterator it = m_mtdRQ.find(stringRQ);
	SetScreenDiffTime( strRQ, (it != m_mtdRQ.end()) ? it->second : 0 );
}

// RQ�� �����ð����� ���� �ð��� ���� ȭ�鿡 �������� �ð����� �ð����� ���Ѵ�
time_t CXScaleTimeDiffManager::GetRQScreenTime(CString strRQ, tm* timeRQ)
{
	if( !m_strScreenRQ.Compare(strRQ) )		return mktime(timeRQ);		// ȭ�鿡 �������� �ð����� RQ�� �׳� return

	string stringRQ((LPSTR)(LPCSTR)strRQ);

	MAP_RQ_TIME_DIFF::iterator it = m_mtdRQ.find(stringRQ);

	if(it == m_mtdRQ.end())
		return mktime(timeRQ);

	int	spanTime = it->second;
	if( spanTime == 0 && m_tmScreenTimeDiff == 0 )
		return mktime(timeRQ);
	if( spanTime == m_tmScreenTimeDiff )		// ȭ�鿡 �������� �ð����� ������ ���� RQ�� �׳� Return
		return mktime(timeRQ);

	timeRQ->tm_hour += (m_tmScreenTimeDiff - spanTime);
	return mktime(timeRQ);
}


// RQ�� �����ð����� ���� �ð��� ���� ȭ�鿡 �������� �ð����� �ð����� ���Ѵ� => ����۽ð��� ó��
// ��/��/�������� ���
time_t CXScaleTimeDiffManager::GetRQScreenDateTime(CString strRQ, time_t timeRQ)
{
	string stringRQ((LPSTR)(LPCSTR)strRQ);

	MAP_RQ_TIME_DIFF::iterator it = m_mtdRQ.find(stringRQ);

	if(it == m_mtdRQ.end())
		return timeRQ;

	int	spanTime = it->second;

	MAP_RQ_TIME_MARKET::iterator itm = m_mRQMarketTime.find(stringRQ);
	time_t nMarketStartTime = 0;

	if(itm != m_mRQMarketTime.end())
	{
		nMarketStartTime = itm->second;
	}

	return timeRQ + (3600 * (m_tmScreenTimeDiff - spanTime) ) + nMarketStartTime;

}

// Local PC�� ����ð��� ����Ÿ�� ���� �ð����� �ð�(�ŷ��ҽð�)���� �����Ѵ�
// ����� ����Ÿ�� �������� �������� �ʴ� ��� ����� ����Ÿ�� PC�� �ð����� �����ϱ� ���� �ʿ�
tm *CXScaleTimeDiffManager::CalcCurrentDateTimToPacketTime(CString strRQ)
{
	string stringRQ((LPSTR)(LPCSTR)strRQ);
	MAP_RQ_TIME_DIFF::iterator it = m_mtdRQ.find(stringRQ);
	if(it == m_mtdRQ.end())
	{
		return CDataConversion::CalcCurrentDateTimToPacketTime(0);
	}

	return CDataConversion::CalcCurrentDateTimToPacketTime(it->second);
}
