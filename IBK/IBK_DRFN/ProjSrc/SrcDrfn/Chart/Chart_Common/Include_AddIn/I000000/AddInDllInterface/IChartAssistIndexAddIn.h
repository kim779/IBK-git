// IChartAssistIndexAddIn.h: interface for the IChartAssistIndexAddIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTASSISTINDEXADDIN_H__0ADE69EC_94E7_430E_ADCD_7128CBDF784C__INCLUDED_)
#define AFX_ICHARTASSISTINDEXADDIN_H__0ADE69EC_94E7_430E_ADCD_7128CBDF784C__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����! Chart OCX�� �̿��ϴ� �� DLL���� Special Interface�̴�.
//		���� ������ Chart OCX���� Header�� �����Ͽ����ϸ�,
//		 Ư�� AddIn DLL���� �� Interface Header�� ���� �̿��� �� ����.
//			(�ٸ� �̸����� �������� ��)
//		Chart OCX���� ������ Interface�� ������ �ʿ䰡 ���� ���,
//		 �� Header�� �����ϸ�, CSiteCustomAddin::GetAddInCustomInterface()�� �������� �ʾ�
//		 CSiteCustomAddin::GetAddInCustomInterface()�� �����ǵ��� �ʵ��� �Ѵ�.
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IPacket050531.h"

interface IChartAssistIndexAddIn : public IUnknown
{
protected:
	virtual ~IChartAssistIndexAddIn()	{}

public:
	// ��ǥ�� ����Ѵ�.(��ǥ�߰�, ��ǥ����)
	virtual BOOL CalculateIndicator(const char* pcszIndicatorName, CList<IPacket050531*, IPacket050531*>* listIPacket) = 0;
	// ��ǥ�� �����Ѵ�.
	virtual BOOL DeleteIndicator(const char* pcszIndicatorName) = 0;
	// �����߰��Ǿ� �ִ� ��� ��ǥ�� �ٽ� ����Ѵ�.(TR �����Ҷ� / Real �����Ҷ�)
	virtual BOOL CalculateAll(const int nDataOption) = 0;
};

#endif // !defined(AFX_ICHARTASSISTINDEXADDIN_H__0ADE69EC_94E7_430E_ADCD_7128CBDF784C__INCLUDED_)
