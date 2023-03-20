// UnchangeableIndicatorData.h: interface for the CUnchangeableIndicatorData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNCHANGEABLEINDICATORDATA_H__CED370A3_7EFC_4BA8_BD31_0C21D57D215F__INCLUDED_)
#define AFX_UNCHANGEABLEINDICATORDATA_H__CED370A3_7EFC_4BA8_BD31_0C21D57D215F__INCLUDED_

#include "IndicatorConditionBaseData.h"

#include "../Include_Chart/DLL_Load/GraphTypeData.h"
#include "../Include_Chart/Dll_Load/IndicatorGroupData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUnchangeableData
{
public:
	CUnchangeableData();

	// ������� ���� Site, User Default File
	bool FileSUDefaultDataBuild( CIndicatorList* pIndicatorList, const CString& strMainDefaultFilePathAndName, const CString& strUserDefaultFilePathAndName, HWND p_hOcxWnd);
	void GetWritingFileData( CStringList &p_slDataString) const;

	double GetDefaultVersion() const;
	const CDefaultIndicatorList& GetMainDefaultIndicatorList() const;
	CDefaultIndicatorList& GetMainDefaultIndicatorList();
	CDefaultIndicatorList* GetMainDefaultIndicatorList_Pointer();
	// Indicator�� Group�� ���õ� ����
	BOOL GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList &p_dlIndicatorGroup, CChartInfo::CHART_MODE p_eChartMode);
	// Indicator List
	CUserIndicatorList* GetUserDefaultIndiList() const;

	// IndicatorInfo �� ���õ� ����
	bool GetMainDefaultIndicatorNameList_General( CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const;
	bool GetMainDefaultIndicatorNameList_Special( CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const;
	bool GetMainDefaultIndicatorNameList_None(CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const;
	const CGraphTypeAndStyleData *GetGraphTypeAndStyleData( void)	{	return &m_graphTypeAndStyleData;	}

private:
	bool m_bDefaultInit;
	CGraphTypeAndStyleData m_graphTypeAndStyleData;

	CDefaultIndicatorList m_defaultIndiList;
	CUserIndicatorList m_userDefaultIndiList; // ���� - "�⺻���� ����"�� �Ұ�츸 userDefault�� �����ȴ�!!
};

#endif // !defined(AFX_UNCHANGEABLEINDICATORDATA_H__CED370A3_7EFC_4BA8_BD31_0C21D57D215F__INCLUDED_)
