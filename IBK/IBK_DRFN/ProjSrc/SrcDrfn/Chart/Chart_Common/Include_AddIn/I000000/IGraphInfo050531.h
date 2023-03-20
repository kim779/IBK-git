// IGraphInfo050531.h: interface for the IGraphInfo050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFO050531_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
#define AFX_IGRAPHINFO050531_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//		ChartAssistIndexAddIn		��ȭ			���ٹ�
//////////////////////////////////////////////////////////////////////

// (2004.10.13, ��¿�) IndicatorList �� Object Interface�� �����Ѵ�.
//		AddIn DLL ���� Interface�� Macro Interface�� �����Ѵ�.
#if IGraphInfoVer < 050531
	#define	IGraphInfo		IGraphInfo050531
	#undef	IGraphInfoVer
	#define	IGraphInfoVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

interface IGraphInfo050531 : public IUnknown
{
protected:
	virtual ~IGraphInfo050531() {}

		
public:
	// ����(05/07/14) ��ǥ ��꿡 �ʿ��� ���ǰ��� �����Ѵ�.
	virtual CList<double, double>* GetCommonCalculateVarList() = 0;
	virtual CList<double, double>* GetCalculateVarList(const char* pcszSubGraphName) = 0;
	virtual CList<double, double>* GetCalculateVarList(const int nSubGraphIndex) = 0;

	// ����(05/07/15) ��꿡 ���̴� ��Ŷ���� �����Ѵ�.
	virtual LPCTSTR GetCommonCalculatePacketNames() = 0;
	virtual LPCTSTR GetCalculatePacketNames(const int nSubGraphIndex) = 0;

	// ����׷����� �ε����� �����Ѵ�.
	virtual int GetSubGraphIndex(const char* pcszSubGraphName) = 0;
	
	// ����׷����� Show/Hide ���θ� �����Ѵ�. (bSubGraphShow - true: show, false: hide)
	virtual bool IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow) = 0;
	virtual bool IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow) = 0;
};

#endif // !defined(AFX_IGRAPHINFO050531_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
