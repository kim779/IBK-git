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
// (2004.10.07, ��¿�) Interface Wrapper���� IUnknown�� Interface�� �ս��� ������ �� �ֵ��� Macro�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IGraphInfo, CIWGraphInfo, CIndicatorInfo, m_pIndicatorInfo)
	
protected:
	// ����(05/07/14) ��ǥ �������� �����Ѵ�.
	virtual CList<double, double>* GetCommonCalculateVarList();
	virtual CList<double, double>* GetCalculateVarList(const char* pcszSubGraphName);
	virtual CList<double, double>* GetCalculateVarList(const int nSubGraphIndex);

	// ����(05/07/15) ��꿡 ���̴� ��Ŷ���� �����Ѵ�.
	virtual LPCTSTR GetCommonCalculatePacketNames();
	virtual LPCTSTR GetCalculatePacketNames(const int nSubGraphIndex);

	// ����׷����� �ε����� �����Ѵ�.
	virtual int GetSubGraphIndex(const char* pcszSubGraphName);	
	
	// ����(05/07/29) ����׷����� Show/Hide ���θ� �����Ѵ�.
	virtual bool IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow);
	virtual bool IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow);

};

#endif // !defined(AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
