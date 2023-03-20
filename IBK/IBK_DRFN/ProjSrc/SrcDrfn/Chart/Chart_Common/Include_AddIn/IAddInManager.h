// IAddInManager.h: interface for the IAddInManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_)
#define AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	�� AddIn���� �̿�Ǵ� OCX�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ Version�� AddIn ���� Module�� �Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.07, ��¿�) Addin DLL���� ���޵Ǿ�, Chart Ocx Interface Gate Way ����� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartOcx050116;
interface IChartOcx050531;

interface IAddInManager : public IUnknown  
{
protected:
	virtual ~IAddInManager()	{}

// (2004.10.07, ��¿�) Chart OCX�� Interface�� ���ϴ� Interface�� �����Ѵ�.
public:
	virtual IChartOcx050116 *	GetIChartOcx050116( void)	= 0;
	virtual IChartOcx050531 *	GetIChartOcx050531( void)	= 0;
};

#endif // !defined(AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_)
