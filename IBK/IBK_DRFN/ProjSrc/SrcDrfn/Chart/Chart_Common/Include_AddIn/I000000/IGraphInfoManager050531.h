// IGraphInfoManager050531.h: interface for the IGraphInfoManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFOMANAGER050531_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
#define AFX_IGRAPHINFOMANAGER050531_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_

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
#if IGraphInfoManagerVer < 050531
	#define	IGraphInfoManager		IGraphInfoManager050531
	#undef	IGraphInfoManagerVer
	#define	IGraphInfoManagerVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IGraphInfo050531.h"
#include <AFXTEMPL.H>

interface IGraphInfoManager050531 : public IUnknown  
{
protected:
	virtual ~IGraphInfoManager050531()	{};

// [04/10/14] Graph Info�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/14] Graph�� 6������ ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]) = 0;
	
// [05/07/15] Getting Graph Info Interface
public:
	virtual IGraphInfo *		GetGraphInfo( const char *p_szGraphName) = 0;
};

#endif // !defined(AFX_IGRAPHINFOMANAGER050531_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
