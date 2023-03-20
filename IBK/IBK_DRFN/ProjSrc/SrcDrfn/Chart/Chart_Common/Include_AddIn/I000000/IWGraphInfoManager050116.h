// IWGraphInfoManager050116.h: interface for the CIWGraphInfoManager050116 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFOMANAGER050116_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
#define AFX_IWGRAPHINFOMANAGER050116_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

// (2004.10.13, ��¿�) IndicatorList�� Interface Wrapper Class�� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IGraphInfoManager050116.h"

class CIndicatorList;
class CIWGraphInfoManager050116 : public CIWUnknown, public IGraphInfoManager050116
{
public:
	CIWGraphInfoManager050116( CIndicatorList *p_pIndicatorList);
	virtual ~CIWGraphInfoManager050116();

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
};

#endif // !defined(AFX_IWGRAPHINFOMANAGER050116_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
