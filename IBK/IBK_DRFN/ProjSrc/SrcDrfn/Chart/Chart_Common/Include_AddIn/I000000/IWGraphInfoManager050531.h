// IWGraphInfoManager050531.h: interface for the CIWGraphInfoManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFOMANAGER050531_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
#define AFX_IWGRAPHINFOMANAGER050531_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//		ChartAssistIndexAddIn		��ȭ			���ٹ�
//////////////////////////////////////////////////////////////////////

// (2004.10.13, ��¿�) IndicatorList�� Interface Wrapper Class�� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IGraphInfoManager050531.h"

class CIndicatorList;
class CIWGraphInfoManager050531 : public CIWUnknown, public IGraphInfoManager050531
{
public:
	CIWGraphInfoManager050531( CIndicatorList *p_pIndicatorList);
	virtual ~CIWGraphInfoManager050531();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.13, ��¿�) IndicatorList Object�� Pointer�� �����Ѵ�.
protected:
	CIndicatorList *	m_pIndicatorList;

// [04/10/14] Graph Info�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/14] Graph�� 6������ ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]);
	
	// ����(05/07/14) ��ǥ �������� �����Ѵ�.
	virtual CList<double, double>* GetCommonConditionList(const char* pcszGraphName);
	virtual CList<double, double>* GetConditionList(const char* pcszGraphName, const char* pcszSubGraphName);
	virtual CList<double, double>* GetConditionList(const char* pcszGraphName, const int nSubGraphIndex);
	
// [05/07/15] Getting Graph Info Interface
public:
	virtual IGraphInfo050531 *		GetGraphInfo( const char *p_szGraphName);
};

#endif // !defined(AFX_IWGRAPHINFOMANAGER050531_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
