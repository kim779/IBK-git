 // ChartEnvSetUpHelper.h: interface for the CChartEnvSetUpHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTENVSETUPHELPER_H__DF431C3C_E172_4279_88B0_26869D664DF6__INCLUDED_)
#define AFX_CHARTENVSETUPHELPER_H__DF431C3C_E172_4279_88B0_26869D664DF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "LoadDll.h"
#include "EnvironmentData.h"
#include "IndicatorConfigurationData.h"


class CCommonInterface;


// Define the Function Pointer
// ��Ʈ ȯ�� ���� Dialog Load
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadChartEnvSetUpDlgAll)(CCommonInterface *, const int);


// Define the Function Pointer
// ��ǥ ���� Dialog Load
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadChartIndicatorSetUpDlg)(CCommonInterface *);


// Define the Function Pointer
// ȯ�� ���� Dialog�� Load
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadChartEnvSetUpDlg)(CCommonInterface *, const int);


// ��ü ��ǥ List�� ��� ��ǥ ��Ͽ��θ��� �����ϴ� ������ OCX������ �̿�ǰ� ������,
// �װ͵� ���� Popup Menu�� ����/�ܺ� ToolBar�� ��ϵǾ� ���� �ʾ� ���� �̿���� �ʴ� ����̴�.
// Return Value�� CIndicatorConfigurationData::INDICONFIG_RETURN�� �̿����� �ʰ�, 
// IDOK ������ �����õȴ�. (�����Ǵ� Dialog�� 'Ȯ��/���' Button�� ����.)
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadIndicatorList)(CCommonInterface* );

// ���õ� Graph�� ��ǥ ������ �޴µ��� �ϴ� ������� ChartObject DLL�� CGraphTool���� �̿�ǰ� �ִ�.
// Return Value�� CIndicatorConfigurationData::INDICONFIG_RETURN�� ��ȯ�ȴ�.
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadIndicatorConfiguration)(const CIndicatorConfigurationData&, CIndicatorConfigurationChangedData&);

// ���õ� Graph�� ��ǥ ������ �޴µ��� �ϴ� ��ɿ���,
// ������ Check Button���� Default (User Default) Indicator Info�� �����ϵ��� ������ ��������
// �����޾� ó����� �Ѵ�.
typedef void (WINAPI *fnpSetWithDefaultIndicator)( BOOL p_bWithDefaultInfo);

//sy 2005.12.20. -> Skin ���
typedef bool (WINAPI* fpGetChartSkinData)(const CString&, const CString&, const int, CStyleData& );

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
// Comments		: 
// Using way	: 
// See			: class CLoadDllLib
//-----------------------------------------------------------------------------
class CChartEnvSetUpHelper : public CLoadDllLib
{
public:
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
	// Modifier		: 
	// Comments		: 
	//-----------------------------------------------------------------------------
	CChartEnvSetUpHelper()
	{
		m_delAuto = TRUE;
		m_szDllName = _T("ChartEnvSetUp.dll");
		m_hLib = NULL;

		MakeNull(m_pfnpLoadChartEnvSetUpDlgAll);
		MakeNull(m_pfnpLoadChartIndicatorSetUpDlg);
		MakeNull(m_pfnpLoadChartEnvSetUpDlg);
		MakeNull(m_pfpLoadIndicatorConfiguration);
		MakeNull(m_pfpLoadIndicatorList);
		MakeNull(m_pfpSetWithDefaultIndicator);
		MakeNull(m_pfpGetChartSkinData);

		LoadLib();
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
	// Return void : 
	// Param  LPCSTR _szDll  ; : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	CChartEnvSetUpHelper( LPCSTR _szDll )
	{
		m_delAuto = TRUE;

		if( _szDll )
			m_szDllName.Format( "%s%s", _szDll, "ChartEnvSetUp.dll" );

		else
			m_szDllName = _T("ChartEnvSetUp.dll");
		
		m_hLib = NULL;
		
		MakeNull(m_pfnpLoadChartEnvSetUpDlgAll);
		MakeNull(m_pfnpLoadChartIndicatorSetUpDlg);
		MakeNull(m_pfnpLoadChartEnvSetUpDlg);
		MakeNull(m_pfpLoadIndicatorConfiguration);
		MakeNull(m_pfpLoadIndicatorList);
		MakeNull(m_pfpSetWithDefaultIndicator);
		MakeNull(m_pfpGetChartSkinData);

		LoadLib();
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
	// Return void : 
	// Param  BOOL deleteAuto : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	CChartEnvSetUpHelper( BOOL deleteAuto )
	{
		m_delAuto = TRUE;
		m_szDllName = _T("ChartEnvSetUp.dll");
		m_hLib = NULL;
		
		MakeNull(m_pfnpLoadChartEnvSetUpDlgAll);
		MakeNull(m_pfnpLoadChartIndicatorSetUpDlg);
		MakeNull(m_pfnpLoadChartEnvSetUpDlg);
		MakeNull(m_pfpLoadIndicatorConfiguration);
		MakeNull(m_pfpLoadIndicatorList);
		MakeNull(m_pfpSetWithDefaultIndicator);
		MakeNull(m_pfpGetChartSkinData);

		LoadLib();

		m_delAuto = deleteAuto;
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
	// Modifier		: 
	// Comments		: 
	//-----------------------------------------------------------------------------
	~CChartEnvSetUpHelper()
	{
	}

public:
	BOOL m_delAuto;
	
	fnpLoadChartEnvSetUpDlgAll	m_pfnpLoadChartEnvSetUpDlgAll;			// ���� Dialog ��ü�� Load�ϱ� ���� Funciton Pointer
	fnpLoadChartIndicatorSetUpDlg m_pfnpLoadChartIndicatorSetUpDlg;		// ��ǥ���� Dialog�� Load�ϴ� Function pointer
	fnpLoadChartEnvSetUpDlg m_pfnpLoadChartEnvSetUpDlg;					// ȯ�漳�� Dialog�� Load�ϴ� Function pointer

	fnpLoadIndicatorConfiguration	m_pfpLoadIndicatorConfiguration; 
	fnpLoadIndicatorList			m_pfpLoadIndicatorList;
	fnpSetWithDefaultIndicator		m_pfpSetWithDefaultIndicator;
	fpGetChartSkinData  m_pfpGetChartSkinData;//sy 2005.12.20.
	
public:
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho	
	// ��ǥ����&ȯ�漳�� load
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadChartEnvSetUpDlgAll(CCommonInterface *pCommonInterface, const int nActiveTabIndex)
	{
		if( !m_hLib ) 
			return CIndicatorConfigurationData::LOAD_ERROR;			// CS_LOADERROR;

		m_pfnpLoadChartEnvSetUpDlgAll = (fnpLoadChartEnvSetUpDlgAll)::GetProcAddress(m_hLib, "LoadDlgChartEnvSetUpAll");
		
		if( m_pfnpLoadChartEnvSetUpDlgAll == NULL )
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;				// CS_LOADERROR;
		}
		
		else
			return m_pfnpLoadChartEnvSetUpDlgAll(pCommonInterface, nActiveTabIndex);
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho
	// ��ǥ���� Load.
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadChartIndicatorSetUpDlg(CCommonInterface *pCommonInterface)
	{
		if( !m_hLib ) 
			return CIndicatorConfigurationData::LOAD_ERROR;;			// CS_LOADERROR;

		m_pfnpLoadChartIndicatorSetUpDlg = (fnpLoadChartIndicatorSetUpDlg)::GetProcAddress(m_hLib, "LoadDlgChartIndicatorSetUp");
		
		if( m_pfnpLoadChartIndicatorSetUpDlg == NULL )
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;				// CS_LOADERROR;
		}
		
		else
			return m_pfnpLoadChartIndicatorSetUpDlg(pCommonInterface);
	}

	
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho	
	// ȯ�漳�� Load
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadChartEnvSetUpDlg(CCommonInterface *pCommonInterface, const int nActiveTabIndex)
	{
		if( !m_hLib ) 
			return CIndicatorConfigurationData::LOAD_ERROR;;			// CS_LOADERROR;

		m_pfnpLoadChartEnvSetUpDlg = (fnpLoadChartEnvSetUpDlg)::GetProcAddress(m_hLib, "LoadDlgChartEnvSetUp");
		
		if( m_pfnpLoadChartEnvSetUpDlg == NULL )
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;				// CS_LOADERROR;
		}
		
		else
			return m_pfnpLoadChartEnvSetUpDlg(pCommonInterface, nActiveTabIndex);
	}

	
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
	// ���� �׷��� ����
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadIndicatorConfiguration(const CIndicatorConfigurationData& indicatorConfigurationData, CIndicatorConfigurationChangedData& changeData)
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadIndicatorConfiguration	= (fnpLoadIndicatorConfiguration)::GetProcAddress(m_hLib, "LoadIndicatorConfiguration");
		if(m_pfpLoadIndicatorConfiguration == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadIndicatorConfiguration(indicatorConfigurationData, changeData);
	}

	
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
	// Return CIndicatorConfigurationData::INDICONFIG_RETURN : 
	// Param  CCommonInterface* pCommonInterface : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadIndicatorList( CCommonInterface* pCommonInterface )
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadIndicatorList	= (fnpLoadIndicatorList)::GetProcAddress(m_hLib, "LoadIndicatorList");
		if(m_pfpLoadIndicatorList == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadIndicatorList(pCommonInterface);
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
	// Return void : 
	// Param  BOOL p_bWithDefaultInfo : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	void SetWithDefaultIndicator( BOOL p_bWithDefaultInfo)
	{
		if(!m_hLib) return;

		m_pfpSetWithDefaultIndicator = ( fnpSetWithDefaultIndicator)::GetProcAddress( m_hLib, "SetWithDefaultIndicator");
		if(m_pfpSetWithDefaultIndicator == NULL)
		{
			MAKESTATE( LDS_FAIL, LDR_GETPROCADDRESS);
			return;
		}
		m_pfpSetWithDefaultIndicator( p_bWithDefaultInfo);
	}

	//sy 2005.12.20. -> skin ����
	bool GetChartSkinData(const CString& strDefaultFilePath, const CString& strUserFilePath, const int nStyleNumber, CStyleData& styleData)
	{
		if(!m_hLib) 
			return (GetState() == TRUE ? true: false);//CS_LOADERROR;

		m_pfpGetChartSkinData	= (fpGetChartSkinData)::GetProcAddress(m_hLib, "GetChartSkinData");
		if(m_pfpGetChartSkinData==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return (GetState() == TRUE ? true: false);//CS_LOADERROR;
		}
		m_pfpGetChartSkinData(strDefaultFilePath, strUserFilePath, nStyleNumber, styleData);
		MAKESTATE(LDS_OK, LDR_READY);
		return (GetState() == TRUE ? true: false);//nRet;
	}
};

#endif // !defined(AFX_CHARTENVSETUPHELPER_H__DF431C3C_E172_4279_88B0_26869D664DF6__INCLUDED_)

//////////////////////////////////////////////////////////////////////////
