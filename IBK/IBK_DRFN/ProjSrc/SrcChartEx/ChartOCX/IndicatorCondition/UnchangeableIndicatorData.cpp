// UnchangeableIndicatorData.cpp: implementation of the CUnchangeableIndicatorData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnchangeableIndicatorData.h"
#include "FileBuilder.h"

#include "../Include_Chart/DLL_Load/IndicatorList.h"					// for EIndicatorListType
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CUnchangeableData -> CIndicatorList���� �⺻����(static) data�� ������ �ִ�.

// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
CUnchangeableData::CUnchangeableData() :
		m_defaultIndiList( EILT_SITE_DEFAULT),
		m_userDefaultIndiList( &m_defaultIndiList, EILT_USER_DEFAULT, NULL), 
		m_bDefaultInit(false)
{
}

// public =====================================================================
// default file data �� ��üȭ.
// user default file data �� ��üȭ.
bool CUnchangeableData::FileSUDefaultDataBuild( CIndicatorList* pIndicatorList, const CString& strMainDefaultFilePathAndName, const CString& strUserDefaultFilePathAndName, HWND p_hOcxWnd)
{
	if( m_bDefaultInit) return true;

	CString strFileData;
	char *szFileData = CFileBuilder().FileRead( strMainDefaultFilePathAndName, strFileData);
	if( !strFileData.IsEmpty())
	{
		// GraphType ���� ���� �б�
		if( m_graphTypeAndStyleData.FileDataBuild( szFileData))
		{
			// ��ǥ���� ������ �б�
			m_defaultIndiList.AddIndiInfoFromCfgString( pIndicatorList, &m_graphTypeAndStyleData, szFileData, p_hOcxWnd);
		}
	}
	else AfxMessageBox( "Data File Loading Failed!\r\n[" + strMainDefaultFilePathAndName + "]");

	// ������ �����ؼ��� �ȵȴ�! -> user default/user �� Site default ���� �����Ѵ�.
	szFileData = CFileBuilder().FileRead( strUserDefaultFilePathAndName, strFileData);
	// Data ������ �ִ� file�� �����ߴ����� ���Ѵ�. -> Data ���� ���纻�� ������� �ʴ´�. (why?)
	if( !strFileData.IsEmpty() && strFileData.Find( "Graph Type End") < 0)
	{
		// ��ǥ���� ������ �б�
		m_userDefaultIndiList.AddIndiInfoFromCfgString(pIndicatorList, szFileData, true);
	}

	m_bDefaultInit = true;
	return true;
}

// file�� �����ϱ� ���� data.
void CUnchangeableData::GetWritingFileData( CStringList &p_slDataString) const
{
	m_userDefaultIndiList.GetWritingFileData( p_slDataString);
}

// ----------------------------------------------------------------------------
double CUnchangeableData::GetDefaultVersion() const
{
	return m_defaultIndiList.GetVersion();
}

// ----------------------------------------------------------------------------
const CDefaultIndicatorList& CUnchangeableData::GetMainDefaultIndicatorList() const
{
	return m_defaultIndiList;
}

CDefaultIndicatorList& CUnchangeableData::GetMainDefaultIndicatorList()
{
	return m_defaultIndiList;
}

CDefaultIndicatorList* CUnchangeableData::GetMainDefaultIndicatorList_Pointer()
{
	return &m_defaultIndiList;
}

// Indicator�� Group�� ���õ� ����
// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
BOOL CUnchangeableData::GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList &p_dlIndicatorGroup, CChartInfo::CHART_MODE p_eChartMode)
{
	return m_defaultIndiList.GetAllIndicatorNameWithGroupName( p_dlIndicatorGroup, p_eChartMode);
}

CUserIndicatorList* CUnchangeableData::GetUserDefaultIndiList() const
{
	return const_cast<CUserIndicatorList*>(&m_userDefaultIndiList);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CUnchangeableData::GetMainDefaultIndicatorNameList_General(CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const
{
	return m_defaultIndiList.GetIndicatorNameList(CBaseIndicatorList::GENERAL, IndicatorNameList, p_eChartMode);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CUnchangeableData::GetMainDefaultIndicatorNameList_Special(CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const
{
	return m_defaultIndiList.GetIndicatorNameList(CBaseIndicatorList::SPECIAL, IndicatorNameList, p_eChartMode);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CUnchangeableData::GetMainDefaultIndicatorNameList_None(CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const
{
	return m_defaultIndiList.GetIndicatorNameList(CBaseIndicatorList::NONE, IndicatorNameList, p_eChartMode);
}
