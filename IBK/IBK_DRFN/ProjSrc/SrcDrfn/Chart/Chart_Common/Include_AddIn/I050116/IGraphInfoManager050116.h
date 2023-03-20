// IGraphInfoManager050116.h: interface for the IGraphInfoManager050116 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFOMANAGER050116_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
#define AFX_IGRAPHINFOMANAGER050116_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

// (2004.10.13, ��¿�) IndicatorList �� Object Interface�� �����Ѵ�.
//		AddIn DLL ���� Interface�� Macro Interface�� �����Ѵ�.
#if IGraphInfoManagerVer < 050116
	#define	IGraphInfoManager		IGraphInfoManager050116
	#undef	IGraphInfoManagerVer
	#define	IGraphInfoManagerVer	050116
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IGraphInfoManager050116 : public IUnknown  
{
protected:
	virtual ~IGraphInfoManager050116()	{};

// [04/10/14] Graph Info�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/14] Graph�� 6������ ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]) = 0;
};

#endif // !defined(AFX_IGRAPHINFOMANAGER050116_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
