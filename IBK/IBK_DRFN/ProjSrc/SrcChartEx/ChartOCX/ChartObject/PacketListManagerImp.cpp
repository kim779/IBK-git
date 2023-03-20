// PacketListManagerImp.cpp: implementation of the CPacketListManagerImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketListManagerImp.h"

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/Dll_Load/XScaleManager.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// _MTEXT()
#include "../Include_Chart/OutsideCommonInterface.h"
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl
#include "../Include_Chart/IRealReceiver.h"				// for IRealReceiver
#include "../Include_Chart/Conversion.h"				// for CDataConversion

#include "PacketRQ.h"

#include "GraphImp.h"		// ���� RQ���� : �������� - ojtaso (20070308)
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPacketListManagerImp::CPacketListManagerImp(CString strPacketOption, CPacketType::REALTYPE eRealType, int nLength, bool bUseRealData, const int nSize, const CString& szPacketName, double dVersion) :
//	m_lpszCurrentRQ(NULL),
	m_strPacketOption(strPacketOption), m_eRealType(eRealType), m_nLength(nLength), m_bUseRealData(bUseRealData), m_nHoldingPacketSize(nSize), m_strBasePacketName(szPacketName), m_dVersion(dVersion),
	m_bRemovablePacketRQ(TRUE),
	m_bOneChart(FALSE)	// OneChart���� : �������� - ojtaso (20070627)
{
}

CPacketListManagerImp::~CPacketListManagerImp()
{
//	if(m_lpszCurrentRQ)
//	{
//		::free(m_lpszCurrentRQ);
//		m_lpszCurrentRQ = NULL;
//	}

	if(!m_mapPacketList.empty())
	{
		MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
		while(it != m_mapPacketList.end())
			delete (*it++).second;

		m_mapPacketList.clear();
	}

	// OneChart���� : �������� - ojtaso (20070627)
	if(!m_mapOnePacketList.empty())
		m_mapOnePacketList.clear();
}

// RQ�� ����� ���� �����ڵ带 ����/����/�ֱ�/ƽ���� ���� : �������� - ojtaso (20070106)
// ƽ ī��Ʈ �߰� : �������� - ojtaso (20070329)
// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
void CPacketListManagerImp::SetItemCode(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName)
{
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.find(lpszRQ);
	if(it == m_mapPacketList.end())
		return;
	
	(*it).second->SetItemCode(lpszItemCode, lpszItemName);
}

LPCTSTR CPacketListManagerImp::CreateDefaultChartItem( HWND p_hOcxWnd)
{
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.find(_T("DEFAULT"));
	if(it != m_mapPacketList.end())
	{
		LPCTSTR lpszRQ = ((*it).second)->GetRQ();
		return lpszRQ;
	}

	CPacketRQ* pPacketRQ = new CPacketRQ();
	pPacketRQ->CreatePacketList(m_strPacketOption, m_eRealType, m_nLength, m_bUseRealData, m_nHoldingPacketSize, m_strBasePacketName, m_dVersion, m_pIChartCtrl);

	LPCTSTR lpszRQ = pPacketRQ->GetRQ();
	m_mapPacketList.insert(MAP_RQ_PACKETLIST::value_type(lpszRQ, pPacketRQ));
	return lpszRQ;
}

// RQ�� ����� ���� �����ڵ带 ����/����/�ֱ�/ƽ���� ���� : �������� - ojtaso (20070106)
// ƽ ī��Ʈ �߰� : �������� - ojtaso (20070329)
// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
LPCTSTR CPacketListManagerImp::AddChartItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd)
{
	UINT nIndex = 0;
	if(!m_mapRQ.empty())
	{
		MAP_RQ::iterator it = m_mapRQ.find(lpszItemCode);
		if(it != m_mapRQ.end())
			nIndex = ++((*it).second);
		else
			m_mapRQ.insert(MAP_RQ::value_type(lpszItemCode, nIndex));
	}
	else
		m_mapRQ.insert(MAP_RQ::value_type(lpszItemCode, nIndex));

	CPacketRQ* pPacketRQ = new CPacketRQ(lpszItemCode, lpszItemName, nIndex);
	pPacketRQ->CreatePacketList(m_strPacketOption, m_eRealType, m_nLength, m_bUseRealData, m_nHoldingPacketSize, m_strBasePacketName, m_dVersion, m_pIChartCtrl);

	LPCTSTR lpszRQ = pPacketRQ->GetRQ();
	m_mapPacketList.insert(MAP_RQ_PACKETLIST::value_type(lpszRQ, pPacketRQ));

	return lpszRQ;
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
LPCTSTR CPacketListManagerImp::AddPairItem( LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd)
{
	UINT nIndex = 0;
	if(!m_mapRQ.empty())
	{
		MAP_RQ::iterator it = m_mapRQ.find(lpszItemCode);
		if(it != m_mapRQ.end())
			nIndex = ++((*it).second);
		else
			m_mapRQ.insert(MAP_RQ::value_type(lpszItemCode, nIndex));
	}
	else
		m_mapRQ.insert(MAP_RQ::value_type(lpszItemCode, nIndex));

	CPacketRQ* pPacketRQ = new CPacketRQ( lpszRQ, lpszItemCode, lpszItemName);
	pPacketRQ->CreatePacketList(m_strPacketOption, m_eRealType, m_nLength, m_bUseRealData, m_nHoldingPacketSize, m_strBasePacketName, m_dVersion, m_pIChartCtrl);
	m_mapPacketList.insert(MAP_RQ_PACKETLIST::value_type(lpszRQ, pPacketRQ));

	return lpszRQ;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
BOOL CPacketListManagerImp::AddChartItemRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd)
{
	UINT nIndex = 0;
	LPSTR lpszIndex = (LPSTR)::strstr(lpszRQ, _T("_"));
	if(lpszIndex)
	{
		lpszIndex = NULL;
		string strCode(lpszRQ);

		lpszIndex++;
		nIndex = atoi(lpszIndex);

		MAP_RQ::iterator it = m_mapRQ.find(strCode);
		if(it != m_mapRQ.end())
		{
			if((*it).second < nIndex)
				(*it).second = nIndex;
		}
		else
			m_mapRQ.insert(MAP_RQ::value_type(strCode, nIndex));

		lpszIndex--;
		*lpszIndex = _T('_');
	}
	else
		return FALSE;

	CPacketRQ* pPacketRQ = new CPacketRQ(lpszRQ, lpszItemCode, lpszItemName);
	pPacketRQ->CreatePacketList(m_strPacketOption, m_eRealType, m_nLength, m_bUseRealData, m_nHoldingPacketSize, m_strBasePacketName, m_dVersion, m_pIChartCtrl);

	m_mapPacketList.insert(MAP_RQ_PACKETLIST::value_type(lpszRQ, pPacketRQ));

	return TRUE;
}

BOOL CPacketListManagerImp::DeleteChartItem(LPCTSTR lpszRQ)
{
	if(m_mapPacketList.empty())
		return FALSE;

	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.find(lpszRQ);
	if(it == m_mapPacketList.end())
		return FALSE;

	if(m_bRemovablePacketRQ)
	{
		if(!::lstrcmp(lpszRQ, m_pIChartCtrl->GetCurrentRQ()))
			m_pIChartCtrl->SelectedGraph(_T("DEFAULT"));
		
		m_pIChartCtrl->SetDrdsAdviseRQ(lpszRQ, FALSE);
	}

	if(CString(lpszRQ) != _T("DEFAULT"))
	{
		LPCTSTR lpszItemCode = ((*it).second)->GetItemCode();
		MAP_RQ::iterator itRQ = m_mapRQ.find(lpszItemCode);
		if(itRQ != m_mapRQ.end())
			m_mapRQ.erase(itRQ);

		// RQ ���� : �������� - ojtaso (20070305)
		m_pIChartCtrl->DeletedGraph(lpszRQ, m_bRemovablePacketRQ);

		if(m_bRemovablePacketRQ)
		{
			if(m_bOneChart)
			{
				MAP_RQ_PACKETLIST::iterator oneIt = m_mapOnePacketList.find(lpszRQ);
				if(oneIt != m_mapOnePacketList.end())
					m_mapOnePacketList.erase(oneIt);
			}

			delete (*it).second;
			m_mapPacketList.erase(it);
		}
	}

	// _T("DEFAULT") RQ�� �׻� ���� �ϹǷ�.. : �������� - ojtaso (20070424)
	/*if(m_mapRQ.size() != 0)*/	m_pIChartCtrl->ResetScroll( TRUE);

	return TRUE;
}

//LPCTSTR CPacketListManagerImp::GetCurrentRQ()
//{
//	if(m_lpszCurrentRQ)
//		return m_lpszCurrentRQ;
//	else
//		return _T("DEFAULT");
//}

//void CPacketListManagerImp::SetCurrentRQ(LPCTSTR lpszRQ)
//{
//	if(m_lpszCurrentRQ)
//		::free(m_lpszCurrentRQ);
//
//	m_lpszCurrentRQ = ::strdup(lpszRQ);
//}

CPacketList* CPacketListManagerImp::GetPacketList(LPCTSTR lpszRQ)
{
	if(m_mapPacketList.empty())
		return NULL;

	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.find(lpszRQ);
	if(it == m_mapPacketList.end())
		return NULL;

	CPacketRQ* pPacketRQ = ((*it).second);

	return pPacketRQ->GetPacketList();
}

//CPacketList* CPacketListManagerImp::GetCurrentPacketList()
//{
//	if(m_mapPacketList.empty())
//		return NULL;
//
//	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.find(m_lpszCurrentRQ);
//	if(it == m_mapPacketList.end())
//		return NULL;
//
//	return ((*it).second)->GetPacketList();
//}

CPacketRQ* CPacketListManagerImp::GetPacketRQ(LPCTSTR lpszRQ)
{
	if(m_mapPacketList.empty())
		return NULL;

	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.find(lpszRQ);
	if(it == m_mapPacketList.end())
		return NULL;

	return ((*it).second);
}

CPacket *CPacketListManagerImp::GetBasePacket( LPCTSTR lpszRQ)
{
	CPacketRQ *pPacketRQ = GetPacketRQ( lpszRQ);
	if( !pPacketRQ) return NULL;
	CPacketList *pPacketList = pPacketRQ->GetPacketList();
	if( !pPacketList) return NULL;
	return pPacketList->GetBaseData( pPacketRQ->GetPacketName());
}

LPVOID CPacketListManagerImp::GetMapPacketList()
{
	if(m_bOneChart)
		return (LPVOID)(&m_mapOnePacketList);
	else
		return (LPVOID)(&m_mapPacketList);
}

void CPacketListManagerImp::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_pIChartCtrl = p_pICharetCtrl;
}

LPCTSTR CPacketListManagerImp::GetPacketListRQ(CPacketList* pPacketList)
{
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		if(((*it).second)->GetPacketList() == pPacketList)
			return it->first.data();

		it++;
	}

	return NULL;
}

int	CPacketListManagerImp::GetPacketListCount()
{
	return m_mapPacketList.size();
}

// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
void CPacketListManagerImp::GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName)
{
	CPacketRQ* pPacketRQ = GetPacketRQ(lpszRQ);
	if( pPacketRQ) pPacketRQ->GetItemCode(lpszItemCode, lpszItemName);
}

void CPacketListManagerImp::GetRQByItemCode( LPCTSTR lpszItemCode, CList< CString, CString> &p_slRQ)
{
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		if(!::lstrcmp(((*it).second)->GetItemCode(), lpszItemCode))
			p_slRQ.AddTail(it->first.data());

		it++;
	}
}

// ���� RQ���� : �������� - ojtaso (20070308)
BOOL CPacketListManagerImp::IsSameRQ(LPCTSTR lpszRQ, CGraphImp* pGraph)
{
	return !::lstrcmp(lpszRQ, pGraph->GetGraphRQ());
}

// DEFALUT�� ������ ��� RQ : �������� - ojtaso (20070427)
void CPacketListManagerImp::GetAllRQ( CList <CString, CString> &p_slRQs)
{
	if(m_mapPacketList.empty())
		return;

	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		if(!it->first.compare(_T("DEFAULT")))
		{
			it++;
			continue;
		}

		p_slRQs.AddTail( it->first.data());
		it++;
	}
}

// Graph�� �����ǵ� PacketRQ�� �������� �ʵ��� �ϴ� Flag : �������� - ojtaso (20070531)
void CPacketListManagerImp::SetRemovablePacketRQ(BOOL bRemove)
{
	m_bRemovablePacketRQ = bRemove;
}

// Graph�� �����ǵ� PacketRQ�� �������� �ʵ��� �ϴ� Flag : �������� - ojtaso (20070531)
BOOL CPacketListManagerImp::GetRemovablePacketRQ()
{
	return m_bRemovablePacketRQ;
}

// ���õ� RQ ���� - ojtaso (20070620)
// �׷����� �ٽ� ������� �ʴ´�.
void CPacketListManagerImp::ChangePacketRQ(LPCTSTR lpszRQ1, LPCTSTR lpszRQ2)
{
	return;

	MAP_RQ_PACKETLIST::iterator it1 = m_mapPacketList.find(lpszRQ1);
	if(it1 == m_mapPacketList.end())
		return;

	MAP_RQ_PACKETLIST::iterator it2 = m_mapPacketList.find(lpszRQ2);
	if(it2 == m_mapPacketList.end())
		return;

	it1->second->SetRQ(lpszRQ2);
	it2->second->SetRQ(lpszRQ1);

	it1->second->GetPacketList()->SetRQ(lpszRQ2);
	it2->second->GetPacketList()->SetRQ(lpszRQ1);

	CPacketRQ* lpPacketRQ = it1->second;
	it1->second = it2->second;
	it2->second = lpPacketRQ;
//	m_mapPacketList.erase(it1);
//	m_mapPacketList.erase(it2);

//	m_mapPacketList.insert(MAP_RQ_PACKETLIST::value_type(lpszRQ1, it2->second));
//	m_mapPacketList.insert(MAP_RQ_PACKETLIST::value_type(lpszRQ2, it1->second));

//	((*it2).second)->Change(*(*it1).second);
}

// OneChart���� : �������� - ojtaso (20070627)
void CPacketListManagerImp::SetOneChart( BOOL bOneChart, const char *p_szRQ)
{ 
//	if(m_bOneChart && bOneChart)
		m_mapOnePacketList.clear();

	m_bOneChart = bOneChart; 

	if(m_bOneChart)
	{
		MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.find( p_szRQ);
		m_mapOnePacketList.insert(MAP_RQ_PACKETLIST::value_type( p_szRQ, it->second));
	}
	else
		m_mapOnePacketList.clear();
}

// OneChart���� : �������� - ojtaso (20070627)
BOOL CPacketListManagerImp::GetOneChart()
{
	return m_bOneChart;
}

// 2008.10.31 JS.Kim	����ڼ��� �ð��� �ʱ�ȭ
void CPacketListManagerImp::ResetTimeDiff()
{
	CPacketList* pPacketList;
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		pPacketList = ((*it).second)->GetPacketList();
		if( pPacketList )
		{
			pPacketList->SetBaseTime("", pPacketList->GetOrgBaseTimeDifference());
		}
		it++;
	}
}

// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
BOOL CPacketListManagerImp::IsBasePacketString( void)
{
	CPacketRQ *pPacketRQ = GetPacketRQ( "DEFAULT");
	if( !pPacketRQ) return FALSE;
	CPacketList *pPacketList = pPacketRQ->GetPacketList();
	if( !pPacketList) return FALSE;
	CPacket *pPacket = pPacketList->GetBaseData( pPacketRQ->GetPacketName());
	if( !pPacket) return FALSE;
	return !strcmp( pPacket->GetType(), _MTEXT( C6_CHARACTER));
}

// ���簡 ����� - ojtaso (20080602)
void CPacketListManagerImp::OffsetPacketData(LPCTSTR lpszRQ, double dData, CString strPacketName/* = _T("")*/)
{
	CPacketRQ* pPacketRQ = GetPacketRQ(lpszRQ);
	pPacketRQ->GetPacketList()->OffsetPacketData(dData, strPacketName);
}

// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
//		for CKTBChartCtrl::SetRealDataCode(). ~ Scripted Real Data.
void CPacketListManagerImp::SetRealData(LPCTSTR lpszItemCode, LPCTSTR lpszPacketName, double dData, short nOption)
{
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		if(((*it).second)->IsSameItemCode(lpszItemCode))
		{
			int nRemovePacketDataCount = 0;
			int nResult = ((*it).second)->GetPacketList()->SetRealData(lpszPacketName, dData, nRemovePacketDataCount, nOption);
		}
		it++;
	}
}
// for IRealReceiver AddIn Interface.
BOOL CPacketListManagerImp::SetRealData( const char *p_szRQ, const char *p_szPacketName, const double& dData, int &p_nRemovePacketDataCount)
{
	CPacketRQ* pPacketRQ = GetPacketRQ( p_szRQ);
	if( !pPacketRQ) return FALSE;
	return pPacketRQ->GetPacketList()->SetRealData( p_szPacketName, dData, p_nRemovePacketDataCount);
}


// (2008/11/4 - Seung-Won, Bae) for Nomalized Minute Time Clock.
// (2008/11/16 - Seung-Won, Bae) Notify to PacketList.
/*
void CPacketListManagerImp::SetRealData_Clock( const __int64 &p_nRealDataTime, IRealReceiver *p_pIRealReceiver, BOOL TRUE)
{
	if( !m_pIChartCtrl->GetNormalizedMinuteTimeRule()) return;

	TRACE( "NMTR : %I64d\r\n", p_nRealDataTime);

	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		CPacketRQ* pPacketRQ = ((*it).second);
		CPacketList *pPacketList = pPacketRQ->GetPacketList();

		__int64 nRealDataTime = p_nRealDataTime;
		int nOrgBaseTimeDiff = pPacketList->GetOrgBaseTimeDifference();
		if( nOrgBaseTimeDiff) nRealDataTime = CDataConversion::CalcTimeSum( nRealDataTime, nOrgBaseTimeDiff * 10000);

		if( pPacketList->SetRealData_Clock( nRealDataTime))
		{
			int nClockedDataCount = pPacketList->PopClockedDataCount();

			p_pIRealReceiver->SetRealDataRQ( it->first.data(), _MTEXT( C0_DATE_TIME), nRealDataTime, TRUE);

			CPacket *pPacketClose = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));
			if( pPacketClose)
			{
				int nDataCount = pPacketClose->GetDataCount();
				if( 0 < nDataCount)
				{
					double dData = pPacketClose->GetData( nDataCount - 1);
					dData = pPacketList->GetOrgDataBeforeRate( _MTEXT( C0_CLOSE), dData);
					p_pIRealReceiver->SetRealDataRQ( it->first.data(), _MTEXT( C0_CLOSE), dData, TRUE);
				}
			}

			CPacket *pPacketVolume = pPacketList->GetBaseData( _MTEXT( C0_VOLUME));
			if( pPacketVolume)
			{
				int nDataCount = pPacketVolume->GetDataCount();
				if( 0 < nDataCount)
				{
					double dData = 0.0;
					pPacketVolume->GetRealDataGap( dData);
					p_pIRealReceiver->SetRealDataRQ( it->first.data(), _MTEXT( C0_VOLUME), dData, TRUE);
				}
			}

			p_pIRealReceiver->SetRealPacketEndRQ( it->first.data(), TRUE);

			pPacketList->PushClockedDataCount( nClockedDataCount);
		}

		it++;
	}
}
*/
void CPacketListManagerImp::OnNMTRClock( const __int64 &p_nClock, IRealReceiver *p_pIRealReceiver)
{
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		CPacketRQ* pPacketRQ = ((*it).second);
		CPacketList *pPacketList = pPacketRQ->GetPacketList();

		__int64 nRealDataTime = -1;
		while( pPacketList->GetRealData_CloseTime( p_nClock, nRealDataTime))
		{
			pPacketList->PushNMTRState( nRealDataTime);

			p_pIRealReceiver->SetRealDataRQ( it->first.data(), _MTEXT( C0_DATE_TIME), nRealDataTime, FALSE);

			CPacket *pPacketClose = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));
			if( pPacketClose)
			{
				int nDataCount = pPacketClose->GetDataCount();
				if( 0 < nDataCount)
				{
					double dData = pPacketClose->GetData( nDataCount - 1);
					dData = pPacketList->GetOrgDataBeforeRate( _MTEXT( C0_CLOSE), dData);
					p_pIRealReceiver->SetRealDataRQ( it->first.data(), _MTEXT( C0_CLOSE), dData, FALSE);
				}
			}

			CPacket *pPacketVolume = pPacketList->GetBaseData( _MTEXT( C0_VOLUME));
			if( pPacketVolume)
			{
				int nDataCount = pPacketVolume->GetDataCount();
				if( 0 < nDataCount)
				{
					double dData = 0.0;
					pPacketVolume->GetRealDataGap( dData);
					p_pIRealReceiver->SetRealDataRQ( it->first.data(), _MTEXT( C0_VOLUME), dData, FALSE);
				}
			}

			p_pIRealReceiver->SetRealPacketEndRQ( it->first.data(), FALSE);

			pPacketList->PopNMTRState();
		}

		it++;
	}
}


// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
void CPacketListManagerImp::ChangeChartMode( int p_nChartMode)
{
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		CPacketRQ* pPacketRQ = ((*it).second);
		if( pPacketRQ)
		{
			CPacketList *pPacketList = pPacketRQ->GetPacketList();
			if( pPacketList) pPacketList->ChangeChartMode( p_nChartMode);
		}
		it++;
	}
}

BOOL CPacketListManagerImp::FindRQ(LPCTSTR lpszRQ)
{
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.find(lpszRQ);
	if(it == m_mapPacketList.end())
		return FALSE;
	
	CPacketRQ* pPacketRQ = ((*it).second);
	
	return !pPacketRQ->IsEmptyRQGraph();
}

void CPacketListManagerImp::SetHoldingPacketSize(int nHoldingPacketSize)
{
	//0005977: [7111] ������Ʈ - ƽ�ֱ� ��Ʈ ��ȸ ��, HTS ����� Ÿ�� ���� 0.2������ �������� ��ó�� ���̴� ����
	//{
	//	CString szDebug;
	//	szDebug.Format("\n[DRCS]Ln(%d) CPacketListManagerImp::SetHoldingPacketSize nHoldingPacketSize(%d)\n", __LINE__, nHoldingPacketSize);
	//	OutputDebugString(szDebug);
	//}
	m_nHoldingPacketSize = nHoldingPacketSize;

	//{{JS.Kim_20100920. ����Ÿ �Ǽ� �ʰ��Ͽ� �ڸ��� ������ �ּ��� TR ��ȸ�� �Ǽ� ��ŭ�� �������ֵ��� ����
	//		��ȸ�ø��� ���� �����Ų��.
	MAP_RQ_PACKETLIST::iterator it = m_mapPacketList.begin();
	while(it != m_mapPacketList.end())
	{
		CPacketRQ* pPacketRQ = ((*it).second);
		if( pPacketRQ )
		{
			CPacketList *pPacketList = pPacketRQ->GetPacketList();
			if( pPacketList)	pPacketList->SetHoldingPacketSize(nHoldingPacketSize);
		}
		it++;
	}
	//}}
}
