// ScaleCalculator.h: interface for the CScaleCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALECALCULATOR_H__E30175FE_B31E_49B4_B8D8_F030A7034924__INCLUDED_)
#define AFX_SCALECALCULATOR_H__E30175FE_B31E_49B4_B8D8_F030A7034924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CScaleCompart;

// scale ��꿡 ���õ� class
class CScaleCalculator
{
// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

public:
	CScaleCalculator( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}

public:
	// packetData�� scaleCompart �� ���� �ΰ��� ���� string ����
	CString GetCompartScaleData(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart);
	CString GetCompartScaleData_Date(const CString& packetData, const CString& dataType, const CString& compartType);
	CString GetCompartScaleData_Time(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart);

	// ���� string �ϰ�� ����� size ���ϱ�
	static CSize GetTextSize(CDC* pDC, const CString& data);
	// ������ ���̸� �Է��Ͽ� size�� �����´�.
	static CSize GetTextSize(const CScaleBaseData::TEXTCOMPARTTYPE textCompart, const CSize& baseSize, const int xCount = 1, const int yCount = 1);
	static CSize GetTextSize(CDC* pDC, const CString& data, const int xCount, const int yCount);

private:
	static CString GetRealPacketData(const CString& packetData, const int nLength);

	bool GetPacketDataInTimeOut(const CString& dataType, CString& realPacketData);
	CString GetCompartScaleTime_One(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart);
	CString GetCompartScaleTime_Two(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart);
	void GetCompartScaleDateTime(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart, CString& strData_date, CString& strData_time);

	CString GetCompartScaleDate(const CString& packetData, const CString& dataType, const CString& compartType);
	CString GetCompartScaleTime(const CString& packetData, const CString& dataType, const CString& compartType);

	static bool ChangeData(CString& baseData, const CString& insertData, const int position);
	static CString GetTrimData(const CString& packetData, const CString& dataType, const TCHAR& strTrim);
	static CString GetMonthEnglish(const CString& month);
};

#endif // !defined(AFX_SCALECALCULATOR_H__E30175FE_B31E_49B4_B8D8_F030A7034924__INCLUDED_)
