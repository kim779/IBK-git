// ChartAddInBase050116.cpp: implementation of the CChartAddInBase050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� AddIn�� Interface Support Base Class�Դϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartAddInBase050116.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartAddInBase050116::CChartAddInBase050116( IChartOCX *p_pIChartOCX)
{
	// (2004.10.15, ��¿�) OCX Interface�� NULL�� ��츦 ����Ͽ�, Member�� �̸� NULL �ʱ�ȭ�� ó���Ѵ�.
	m_pIPacketManager		= NULL;
	m_pIChartManager		= NULL;
	m_pIGraphInfoManager	= NULL;
	m_pChartCWnd			= NULL;

	// (2004.10.08, ��¿�) Chart Ocx Interface�� �����Ͽ�, ���� Ÿ Object�� ��ȸ�� �� �ֵ��� �Ѵ�.
	m_pIChartOCX = p_pIChartOCX;
	if( !m_pIChartOCX)
	{
		AfxMessageBox( "CChartAddInBase050116 Null Wrapping Error!");
		return;
	}
	m_pIChartOCX->AddRef();

	// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
	m_pIPacketManager		= m_pIChartOCX->GetIPacketManager050116();
	m_pIChartManager		= m_pIChartOCX->GetIChartManager050116();
	m_pIGraphInfoManager	= m_pIChartOCX->GetIGraphInfoManager050116();

	// (2004.10.25, ��¿�) Chart OCX Control Window Pointer�� �����Ͽ� Windows Interface�� �̿��� �� �ֵ��� �Ѵ�.
	m_pChartCWnd = CWnd::FromHandle( m_pIChartOCX->GetOcxHwnd());
}

CChartAddInBase050116::~CChartAddInBase050116()
{
	// (2004.10.08, ��¿�) Chart Ocx Interface�� �����Ͽ�, ���� Ÿ Object�� ��ȸ�� �� �ֵ��� �Ѵ�.
	m_pIChartOCX->Release();

	// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
	if( m_pIChartManager)
	{
		m_pIChartManager->Release();
		m_pIChartManager = NULL;
	}
	if( m_pIGraphInfoManager)
	{
		m_pIGraphInfoManager->Release();
		m_pIGraphInfoManager = NULL;
	}
}

// (2004.10.13, ��¿�) �⺻ Manager�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CChartAddInBase050116::HasFullManager( void)
{
	return m_pIChartOCX && m_pIPacketManager && m_pIChartManager && m_pIGraphInfoManager;
}
