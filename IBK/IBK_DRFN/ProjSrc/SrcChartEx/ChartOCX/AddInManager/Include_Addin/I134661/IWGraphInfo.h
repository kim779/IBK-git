// IWGraphInfo.h: interface for the CIWGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
#define AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/IndicatorInfo.h"
#include "../_IWUnknown.h"
#include "_IGraphInfo.h"
#include "_IChartOCX.h"										// for IChartOCX

class CIWGraphInfo : public CIWUnknown, public IGraphInfo 
{
// (2004.10.07, 배승원) Interface Wrapper에서 IUnknown의 Interface를 손쉽게 지원할 수 있도록 Macro를 정의한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IGraphInfo, CIWGraphInfo, CIndicatorInfo, m_pIndicatorInfo)
	
protected:
	// 종찬(05/07/14) 지표 설정값을 제공한다.
	virtual CList<double, double>* GetCommonCalculateVarList();
	virtual CList<double, double>* GetCalculateVarList(const char* pcszSubGraphName);
	virtual CList<double, double>* GetCalculateVarList(const int nSubGraphIndex);

	// 종찬(05/07/15) 계산에 쓰이는 패킷명을 제공한다.
	virtual LPCTSTR GetCommonCalculatePacketNames();
	virtual LPCTSTR GetCalculatePacketNames(const int nSubGraphIndex);

// (2006/7/7 - Seung-Won, Bae) SubGraph Info
protected:
	// 서브그래프의 인덱스를 리턴한다.
	virtual int		GetSubGraphIndex(const char* pcszSubGraphName);	
	// (2006/7/7 - Seung-Won, Bae) Get Sub Graph Count.
	virtual int		GetSubGraphCount( void);
	// 종찬(05/07/29) 서브그래프의 Show/Hide 여부를 리턴한다.
	virtual bool	IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow);
	virtual bool	IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow);

// (2006/7/6 - Seung-Won, Bae) Retrieve Graph Info by string
protected:
	virtual BOOL	GetGraphDataForMaking( CString &p_strData);
	virtual BOOL	GetAllSubGraphDataForMaking( CString &p_strData);


// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Data
protected:
	virtual void	GetGraphDataDrawingData( BOOL &p_bLowHigh, BOOL &p_bLowCur, BOOL &p_bHighCur, BOOL &p_bPrevCur, int &p_nCurData);

// (2006/7/6 - Seung-Won, Bae) Retrieve Signal Info
protected:
	virtual void	GetSignalInfo( BOOL &p_bValidFlag, BOOL &p_bShowSignal);

};

#endif // !defined(AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
