// IWGraphInfo050531.h: interface for the CIWGraphInfo050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFO050531_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
#define AFX_IWGRAPHINFO050531_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IGraphInfo050531.h"

class CIndicatorInfo;
class CIWGraphInfo050531 : public CIWUnknown, public IGraphInfo050531 
{
public:
	CIWGraphInfo050531(CIndicatorInfo* pIndicatorInfo);
	virtual ~CIWGraphInfo050531();

protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

protected:
	CIndicatorInfo* m_pIndicatorInfo;
	
public:
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

#endif // !defined(AFX_IWGRAPHINFO050531_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
