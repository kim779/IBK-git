// IWGraphInfoManager.h: interface for the CIWGraphInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
#define AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/IndicatorList.h"		// for CIndicatorList
#include "../_IWUnknown.h"
#include "_IGraphInfoManager.h"
#include "_IChartOCX.h"											// for IChartOCX

// (2004.10.13, ��¿�) IndicatorList�� Interface Wrapper Class�� �����Ѵ�.
class CIWGraphInfoManager : public CIWUnknown, public IGraphInfoManager
{
// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IGraphInfoManager, CIWGraphInfoManager, CIndicatorList, m_pIndicatorList)

// [04/10/14] Graph Info�� ��ȸ�ϴ� Interface�� �����Ѵ�.
protected:
	// [04/10/14] Graph�� 6������ ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]);
	
	// ����(05/07/14) ��ǥ �������� �����Ѵ�.
	virtual CList<double, double>* GetCommonConditionList(const char* pcszGraphName);
	virtual CList<double, double>* GetConditionList(const char* pcszGraphName, const char* pcszSubGraphName);
	virtual CList<double, double>* GetConditionList(const char* pcszGraphName, const int nSubGraphIndex);
	
// [05/07/15] Getting Graph Info Interface
protected:
	virtual IGraphInfo *		GetGraphInfo( const char *p_szGraphName);
};

#endif // !defined(AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
