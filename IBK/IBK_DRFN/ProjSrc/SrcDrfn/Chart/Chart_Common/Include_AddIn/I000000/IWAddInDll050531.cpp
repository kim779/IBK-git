// IWAddInDll050531.cpp: implementation of the CIWAddInDll050531 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� OCX���� �̿�Ǵ� AddIn�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWAddInDll050531.h"

#include "IChartAddIn050531.h"	// for IChartAddIn050531

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// [04/10/07, ��¿�] Addin DLL�� Module Name�� Handle�� �����Ͽ� DLL Interface�� ȣ���� �� �ֵ��� �Ѵ�.
CIWAddInDll050531::CIWAddInDll050531( const char *p_szAddInModule, HMODULE p_hAddInDll)
{
	m_hAddInDll		= p_hAddInDll;
	m_strModuleName	= p_szAddInModule;
	if( !m_hAddInDll)
	{
		AfxMessageBox( "CIWAddInDll050531 Null Wrapping Error!");
		return;
	}

	// (2004.10.08, ��¿�) �ʱ�ȭ�ÿ� IAddInManager(ChartOcx Interface Gateway)�� �����Ͽ�
	//		DLL�� AddIn Interface�� Ȯ���Ѵ�.
	m_pIChartAddIn = NULL;
}
CIWAddInDll050531::~CIWAddInDll050531()
{
	// (2004.10.08, ��¿�) AddIn Object�� ���������� ��ϵ� ���, AddIn Object�� Release�Ѵ�.
	if( m_pIChartAddIn)
	{
		m_pIChartAddIn->Release();
		m_pIChartAddIn = NULL;
	}

	// (2004.10.07, ��¿�) ��ϵ� AddIn DLL�� Free��Ų��.
	FreeLibrary( m_hAddInDll);
}

// (2004.10.07, ��¿�) AddIn Interface Wrapper Selector Routine�� �����Ѵ�.
//		AddIn Manager�� LoadAddIn()���� Loading�� DLL�� Addin Version�� �´� Wrapper�� �����ϴ� Routine�̴�.
//		Ư�� Addin Manager�� �׻� ���� Version�� Selector Routine�� ȣ���Ͽ� ������ Wrapper Object�� ���� �� �ֵ��� �Ѵ�.
IAddInDll *CIWAddInDll050531::CreateAddInDLLIWrapper( const char *p_szAddInModule, HMODULE p_hAddInDll, int p_nAddInIVersion)
{
	// 0. ���� Addin DLL�� Handle�� Ȯ���Ѵ�.
	if( !p_hAddInDll) return NULL;

	IAddInDll *pIAddInDll = NULL;
	switch( p_nAddInIVersion)
	{
		case CIWAddInDll050531::IVERSION:	pIAddInDll = new CIWAddInDll050531( p_szAddInModule, p_hAddInDll);
											break;
	}

	if( pIAddInDll) pIAddInDll->AddRef();

	return pIAddInDll;
}


//[04/10/07] AddIn DLL�� �ʱ�ȭ��Ű�� Interface�� �����Ѵ�.
// (2004.10.08, ��¿�) �ʱ�ȭ�ÿ� IAddInManager(ChartOcx Interface Gateway)�� �����Ͽ�
//		DLL�� AddIn Interface�� Ȯ���Ѵ�.
typedef IChartAddIn050531 *( *FN_InitAddInDll)( IAddInManager *p_pIAddInManager);	
BOOL CIWAddInDll050531::InitAddInDll( IAddInManager *p_pIAddInManager)
{
	// 0. InitAddInDll Interface�� Process Address�� Ȯ���Ѵ�.
	FN_InitAddInDll fnInitAddInDll;
	fnInitAddInDll = ( FN_InitAddInDll)GetProcAddress( m_hAddInDll, "InitAddInDll");
	if( !fnInitAddInDll) return FALSE;

	m_pIChartAddIn = ( *fnInitAddInDll)( p_pIAddInManager);
	return ( m_pIChartAddIn != NULL);
}

// [04/10/11] AddIn Object�� Interface�� �����ϴ� Interface�� �����Ѵ�.
IUnknown *CIWAddInDll050531::GetIChartAddIn( void)
{
	m_pIChartAddIn->AddRef();
	return m_pIChartAddIn;
}
