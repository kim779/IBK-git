// ReceivedTRData.h: interface for the CReceivedTRData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECEIVEDTRDATA_H__215A6665_2EE6_4A3D_8889_8273BE018216__INCLUDED_)
#define AFX_RECEIVEDTRDATA_H__215A6665_2EE6_4A3D_8889_8273BE018216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>
//#include "../chart_common/ReplayChartInfoDef.h"
#include "../COMMONTR/KB_d1016.h"



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 10 / 26
// Explain		: 
// Using way	: 
//-----------------------------------------------------------------------------
class CReceivedEachTR
{
public:
	CReceivedEachTR();
	virtual ~CReceivedEachTR();
	void ClearTRData();

// Attribute
public:
	CString	m_szStockCode;

	char	m_szKey[28];

	int		m_nNDayBefore;		// ������ Data���� ����
	CString m_szDateInfo;		// TR�� ���� ���� YYYYMMDDHHMM( �����϶����� ���� ���۽ð� or ����۽ð� )
	CString m_szMarketEndTime;	// �� ����ð� ���� HHMMSS
	int		m_nTickIdx;			// TR�� Index. ������ �� ����� Index
	int		m_nTotalTickCount;	// ��ü Tick Count 
	CString	m_strBojoMsg;		// �����޼���.

	CArray<KB_d1016_OutRec2, KB_d1016_OutRec2>	m_arrChart;
//	KB_d1016_CHART *m_pTRData;		// TR Data
};



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 10 / 17
// Explain		: ������ ���� TR�� Data�� �����Ѵ�.
// Using way	: 
//-----------------------------------------------------------------------------
class CReceivedTRData  
{
public:
	CReceivedTRData();
	virtual ~CReceivedTRData();


// Attribute
public:
	CString m_szStockCode;		// �����ڵ�	
	BOOL m_bRemainData;			// Data�� ������ �������� �Ǵ��ϴ� flag ( TRUE : TR ����, FALSE : TR ���� )
	CList < CReceivedEachTR *, CReceivedEachTR * > m_lstTRData;			// ���� TR Data
};




#endif // !defined(AFX_RECEIVEDTRDATA_H__215A6665_2EE6_4A3D_8889_8273BE018216__INCLUDED_)
