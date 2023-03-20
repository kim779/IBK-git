// XScaleTimeDiffManager.h: interface for the CXScaleTimeDiffManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSCALETIMEDIFFMANAGER_H__22E3D5C0_B833_4D91_BAE7_D14101F05941__INCLUDED_)
#define AFX_XSCALETIMEDIFFMANAGER_H__22E3D5C0_B833_4D91_BAE7_D14101F05941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <vector>
#include <set>

#include "../Include_Chart/DataBlockList.h"
#include "../Include_Chart/BlockBaseEnum.h"

class CXScaleManagerImp;

class CXScaleTimeDiffManager  
{
public:
	CXScaleTimeDiffManager();
	CXScaleTimeDiffManager(CXScaleManagerImp* pXScaleManagerImp);
	virtual ~CXScaleTimeDiffManager();

public:
	friend class CXScaleManagerImp;
	void	SetXScaleManagerImp(CXScaleManagerImp* pXScaleManagerImp);

public:
	typedef map<string, int>						MAP_RQ_TIME_DIFF;		// ������ ������ ���� �� �����Ƿ� time ���� structure�� ��� ����
	typedef pair<string, int>						PAIR_RQ_TIME_DIFF;

	typedef map<string, tm>							MAP_RQ_CURR_TIME;
	typedef pair<string, tm>						PAIR_RQ_CURR_TIME;

	typedef map<string, time_t>						MAP_RQ_TIME_MARKET;
	typedef pair<string, time_t>					PAIR_RQ_TIME_MARKET;
/*
	typedef map<string, CDataBlockList<time_t>*>	MAP_RQ_BASE_TIME;
	typedef pair<string, CDataBlockList<time_t>*>	PAIR_RQ_BASE_TIME;

	typedef map<string, CDataBlockList<time_t>*>	MAP_RQ_LOCAL_TIME;
	typedef pair<string, CDataBlockList<time_t>*>	PAIR_RQ_LOCAL_TIME;
*/

public:
	CXScaleManagerImp*				m_pXScaleManagerImp;

	MAP_RQ_TIME_DIFF				m_mtdRQ;				// RQ ���ڿ� KEY�� �� RQ�� �ð��� ���ؽÿ��� ������ ����
	MAP_RQ_CURR_TIME				m_mRQCurrTime;			// RQ ���ڿ� KEY�� �� RQ�� ���� ��¥+�ð��� ����
	MAP_RQ_TIME_MARKET				m_mRQMarketTime;		// RQ ���ڿ� KEY�� �� RQ�� ����� �ð��� ����
//	MAP_RQ_BASE_TIME				m_mblRQBaseTime;		// RQ�� XScale�� ���ؽð������� ������ �ֽ��ϴ�.( ����, ������ ���� �� �ݿ��ϱ� ���ؼ� �ϸ����ǥ�� �ݿ� ���� ���� ���� )
//	MAP_RQ_LOCAL_TIME				m_mblRQLocalTime;		// RQ�� XScale�� �����ð�(���ŵȽð�)������ ������ �ֽ��ϴ�.( ����, ������ ���� �� �ݿ��ϱ� ���ؼ� �ϸ����ǥ�� �ݿ� ���� ���� ���� )

//	CDataBlockList<time_t>			m_blTimefDRI;			// XScale�� ���� �ð� ���� ( �� RQ���� ���ϵ� Merge�� �ð迭 )

	CString							m_strScreenRQ;			// ���� ȭ�鿡 �������� �ִ� �ð����� RQ
	int								m_tmScreenTimeDiff;		// ���� ȭ�鿡 �������� �ִ� �ð����� ���ؽÿ��� ���� ( HHMMSS )

public:
	void		InitRQDiffTime();									// RQ�� ���ؽÿ��� ���� ����Ÿ �� �ʱ�ȭ
	void		InitRQMarketTime();									// RQ�� ����۽ð� �� �ʱ�ȭ

	void		SetRQDiffTime(CString strRQ, int tmDiffTime, __int64 nCurrTime);		// RQ�� ���ؽÿ��� ���� ����
	void		SetRQMarketTime( CString strRQ, int nMarketStartTime, int nMarketEndTime, CPacketType::MARKETDAYTYPE nMarketDayType);	// RQ�� ����۽ð� ����
	void		SetScreenDiffTime(CString strRQ, int tmDiffTime);	// ���� ȭ�鿡 �������� �ִ� �ð����� ���ؽÿ��� ������ ����
	void		SetCurrentRQ(CString strRQ);						// ���� �������� ������ �Ǵ� RQ

	time_t		GetRQScreenTime(CString strRQ, tm *timeRQ);	// RQ�� �����ð����� ���� �ð��� ���� ȭ�鿡 �������� �ð����� �ð����� ���Ѵ�
	time_t		GetRQScreenDateTime(CString strRQ, time_t timeRQ);

	tm*			CalcCurrentDateTimToPacketTime(CString strRQ);
};

#endif // !defined(AFX_XSCALETIMEDIFFMANAGER_H__22E3D5C0_B833_4D91_BAE7_D14101F05941__INCLUDED_)
