// IndicatorConfigurationHelper.h : main header file for the INDICATORCONFIGURATIONHELPER application
//

#if !defined(AFX_INDICATORCONFIGURATIONHELPER_H__AC973E75_E86C_4ABF_981E_83EC5441B56B__INCLUDED_)
#define AFX_INDICATORCONFIGURATIONHELPER_H__AC973E75_E86C_4ABF_981E_83EC5441B56B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"
#include "IndicatorConfigurationData.h"

/////////////////////////////////////////////////////////////////////////////
class CCommonInterface;

// (2004.09.03, ��¿�, �м�) ��ü ��ǥ ����â�� ���� ������ OCX������ �̿�ǰ� �ִ�.
//		Ư�� CIndicatorConfigurationData::INDICONFIG_RETURN�� �̿����� �ʰ�, IDOK, IDCANCEL������ Return�ǰ�,
//		���� OCX������ �̿�Ǹ鼭, Return ���� �̿���� �ʰ� �ִ�.
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fpLoadAllIndicatorConfiguration)(CCommonInterface* , CMainBlock*, CIndicatorList*, CPacketList*);

// (2004.09.03, ��¿�, �м�) ��ü ��ǥ List�� ��� ��ǥ ��Ͽ��θ��� �����ϴ� ������ OCX������ �̿�ǰ� ������,
//			�װ͵� ���� Popup Menu�� ����/�ܺ� ToolBar�� ��ϵǾ� ���� �ʾ� ���� �̿���� �ʴ� ����̴�.
//		Return Value�� CIndicatorConfigurationData::INDICONFIG_RETURN�� �̿����� �ʰ�, 
//			IDOK ������ �����õȴ�. (�����Ǵ� Dialog�� 'Ȯ��/���' Button�� ����.)
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fpLoadIndicatorList)(CCommonInterface* , CMainBlock*, CIndicatorList*, CPacketList*);

// (2004.09.03, ��¿�, �м�) ���õ� Graph�� ��ǥ ������ �޴µ��� �ϴ� ������� ChartObject DLL�� CGraphTool���� �̿�ǰ� �ִ�.
//		������ ��ǥ�� ���� �����ϴ� GetChangedData()�� �Բ� �̿�ȴ�.
// (2004.09.03, ��¿�) Return Value�� CIndicatorConfigurationData::INDICONFIG_RETURN�� ��ȯ�ȴ�.
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fpLoadIndicatorConfiguration)(const CIndicatorConfigurationData&);

// (2004.09.06, ��¿�) ���õ� Graph�� ��ǥ ������ �޴µ��� �ϴ� ��ɿ���,
//		������ Check Button���� Default (User Default) Indicator Info�� �����ϵ��� ������ ��������
//		�����޾� ó����� �Ѵ�.
typedef void (WINAPI *fpSetWithDefaultIndicator)( BOOL p_bWithDefaultInfo);

// (2004.09.03, ��¿�, �м�) ���õ� Graph�� ��ǥ ���� ���� �����ϴ� ������� ChartObject DLL�� CGraphTool���� �̿�ǰ� �ִ�.
//		Graph�� ��ǥ ������ �޴� LoadIndicatorConfiguration()�� �Բ� �̿�ȴ�.
typedef bool (WINAPI *fpGetChangedData)(CIndicatorConfigurationChangedData&);

class CIndicatorConfigurationHelper : public CLoadDllLib
{
public:
	fpLoadIndicatorConfiguration	m_pfpLoadIndicatorConfiguration; 
	fpLoadAllIndicatorConfiguration	m_pfpLoadAllIndicatorConfiguration;
	fpLoadIndicatorList				m_pfpLoadIndicatorList;
	fpGetChangedData				m_pfpGetChangedData;
	fpSetWithDefaultIndicator		m_pfpSetWithDefaultIndicator;

	BOOL m_delAuto;

	CIndicatorConfigurationHelper(LPCSTR _szDll)
	{
		m_delAuto = TRUE;
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"IndicatorConfiguration.dll");
		else
			m_szDllName = _T("IndicatorConfiguration.dll");
		m_hLib = NULL;
		MakeNull( m_pfpLoadIndicatorConfiguration);
		MakeNull( m_pfpLoadAllIndicatorConfiguration);
		MakeNull( m_pfpLoadIndicatorList);
		MakeNull( m_pfpGetChangedData);
		MakeNull( m_pfpSetWithDefaultIndicator);
		LoadLib();
	}

	CIndicatorConfigurationHelper()
	{
		m_delAuto = TRUE;
		m_szDllName = _T("IndicatorConfiguration.dll");
		m_hLib = NULL;
		MakeNull( m_pfpLoadIndicatorConfiguration);
		MakeNull( m_pfpLoadAllIndicatorConfiguration);
		MakeNull( m_pfpLoadIndicatorList);
		MakeNull( m_pfpGetChangedData);
		MakeNull( m_pfpSetWithDefaultIndicator);
		LoadLib();
	}

	~CIndicatorConfigurationHelper()
	{
	}

	CIndicatorConfigurationData::INDICONFIG_RETURN LoadIndicatorConfiguration(const CIndicatorConfigurationData& indicatorConfigurationData)
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadIndicatorConfiguration	= (fpLoadIndicatorConfiguration)::GetProcAddress(m_hLib, "LoadIndicatorConfiguration");
		if(m_pfpLoadIndicatorConfiguration == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadIndicatorConfiguration(indicatorConfigurationData);
	}

	CIndicatorConfigurationData::INDICONFIG_RETURN LoadIndicatorList(CCommonInterface* pCommonInterface, CMainBlock* block, CIndicatorList* pIndicatorList, CPacketList* pPacketList)
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadIndicatorList	= (fpLoadIndicatorList)::GetProcAddress(m_hLib, "LoadIndicatorList");
		if(m_pfpLoadIndicatorList == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadIndicatorList(pCommonInterface, block, pIndicatorList, pPacketList);
	}

	CIndicatorConfigurationData::INDICONFIG_RETURN LoadAllIndicatorConfiguration(CCommonInterface* pCommonInterface, CMainBlock* block, CIndicatorList* pIndicatorList, CPacketList* pPacketList)
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadAllIndicatorConfiguration	= (fpLoadAllIndicatorConfiguration)::GetProcAddress(m_hLib, "LoadAllIndicatorConfiguration");
		if(m_pfpLoadAllIndicatorConfiguration == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadAllIndicatorConfiguration(pCommonInterface, block, pIndicatorList, pPacketList);
	}

	bool GetChangedData(CIndicatorConfigurationChangedData& changedData)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpGetChangedData	= (fpGetChangedData)::GetProcAddress(m_hLib, "GetChangedData");
		if(m_pfpGetChangedData == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpGetChangedData(changedData);
	}

	// (2004.09.06, ��¿�) ���õ� Graph�� ��ǥ ������ �޴µ��� �ϴ� ��ɿ���,
	//		������ Check Button���� Default (User Default) Indicator Info�� �����ϵ��� ������ ��������
	//		�����޾� ó����� �Ѵ�.
	void SetWithDefaultIndicator( BOOL p_bWithDefaultInfo)
	{
		if(!m_hLib) return;

		m_pfpSetWithDefaultIndicator = ( fpSetWithDefaultIndicator)::GetProcAddress( m_hLib, "SetWithDefaultIndicator");
		if(m_pfpSetWithDefaultIndicator == NULL)
		{
			MAKESTATE( LDS_FAIL, LDR_GETPROCADDRESS);
			return;
		}
		m_pfpSetWithDefaultIndicator( p_bWithDefaultInfo);
	}
};

#endif // !defined(AFX_INDICATORCONFIGURATIONHELPER_H__AC973E75_E86C_4ABF_981E_83EC5441B56B__INCLUDED_)
