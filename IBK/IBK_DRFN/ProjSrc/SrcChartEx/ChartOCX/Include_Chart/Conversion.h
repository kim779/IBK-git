// Conversion.h: interface for the CConversion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERSION_H__4EDC5FDA_0443_4CE1_888C_8E01B8820C70__INCLUDED_)
#define AFX_CONVERSION_H__4EDC5FDA_0443_4CE1_888C_8E01B8820C70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"				// for CChartInfo::CHART_MODE

class CPacket;
class CDataConversion
{
// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

public:
	CDataConversion( HWND p_hOcxWnd, CChartInfo::CHART_MODE	p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}

public:	
	// color --> string
	static CString RGB2Str(const COLORREF clrValue);
	// string --> color
	static COLORREF Str2RGB(const CString& strColor);
	// string -> bool
	static bool atob(const char* string);
	// int -> bool
	static bool atob(const int nData);
	// bool --> string
	static CString Bool2Str(const BOOL bValue) ;
	// float --> double
	static double atod(const float x);
	// int -> CString
	static CString IntToString(const int Num);

	// 2011.01.25 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//static CString IntToString(const int Num, const int length);
	static CString IntToString(const __int64 Num, const int length);
	// 2011.01.25 by SYS <<
	//

	// double -> CString
	static CString DoubleToString(const double& Num);
	static CString DoubleToString(const double& Num, const int decimalDownCount);
	// 20080910 JS.Kim
	static CString DoubleToString(const double& Num, const CString& dataType);
	// ������ ���ڿ��� ��ȯ�Ѵ�.
	static CString NormalNumberString(const CString& strData);
	// ���� ���ڿ��� �� data.
	static double GetLongLengthData(const double& dData1, const double& dData2, const CString& dataType);
	/*
	// ������ȯ => 10���� -> nBase����
	static CString LongToStringByBase(const long lValue, const int nBase);
	static CString LongToStringByBase(const CString &strValue, const int nBase);
	*/

	static int CalcTimeFormat(const int nValue, const int nCharge, const CString& strFormat, const bool bIsTransactionTime);
	static int CalcTimeFormat_Past(const int nValue, const int nCharge, const CString& strFormat);
	static int CalcTimeFormatforCharge(const int nValue, const int nIndex, const CString& strFormat, const int nInterval, const bool bIsTransactionTime);
	static int CalcDateFormat(const int nValue, const int nCharge, const CString& strFormat, const int nCalcType, const int nInterval);
	static CString GetRealDateInTimeFormat(const int nDD, const CString& strFormat);
	bool GetSpecialTime(const CString& strFormat, const CString& strData, CString& strSpecialTime);

	// string�� ���ϴ� string�� trim.
	static CString GetStringData(CString& data, const CString& trimData, const bool bIsInside = true);
	static CString GetStringData(CString& data, const CString& trimStart, const CString& trimEnd, const bool bIsInside);
	// trimStr�� nIndex ��°������ ���ڱ����� �ڸ���.
	static CString GetStringData(CString& data, const CString& trimStr, const int nIndex);
	// trimStr�� nIndex ��°������ string.
	static CString GetLeftToTrimDataIndex(const CString& data, const CString& trimStr, const int nEndIndex = 0, const bool bIsInside = true);
	static CString GetMidToTrimDataStartIndex(const CString& data, const CString& trimStr, const int nStartIndex, const bool bIsInside_start, const bool bIsInside_end);
	static CString GetMidToTrimDataEndIndex(const CString& data, const CString& trimStr, const int nEndIndex, const bool bIsInside_start, const bool bIsInside_end);
	static CString GetMidToTrimDataIndex(const CString& data, const CString& trimStr, const int nEndIndex = 0, const bool bIsInside = true);
	static CString GetMidToTrimDataIndex(const CString& data, const CString& trimStart, const CString& trimEnd, const int nStartIndex = 0, const bool bIsInside = true);
	// string�� ���ϴ� string�� ����
	static int GetStringDataCount(const CString& orgStr, const CString& comparisonStr);
	// �ش� index�� ã�´�.
	static int FindIndex(const CString& orgData, const CString& findData, const int nFindDataCountIndex = 0);
	// �Ҽ��� ���� �� �ڸ���
	static int GetDecimalDownCount(const CString& strDecimalType, const bool bIsCheckDigit = true);
	// data type�� double ���·� ��ȯ
	static double GetDataType(CString strDataType);
	// "x10000" -> "x��" ���·� ��ȯ (10000 �̻� �ѱ۷� �ٲ۴�.)
	CString GetChangeFormat_NumCharUnit(const CString& strOrgFormat);
	
	// ���ϴ� �������� ��Ʈ�� ����
	CString GetStringFormatTrans(const CString& strData, const CString& strFormat/*, const bool bIsRound = true*/);
	
	// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CString GetStringFormatTrans2(const CString& strData, const int& nDataUnit/*, const bool bIsRound = true*/);
	// 2011.01.20 by SYS <<

	// ���ϴ� �������� ��Ʈ�� ���� (õ�������� , �� ǥ��)
	static CString GetStringThousandFormat(const double& dData, const double& dFormat);
	// 20080910 JS.Kim		�̱�ää�� ǥ�ø� ���� 8����/32���������� ��ȯ�� ���� �Լ� �߰�<<
	static CString GetStringThousandFormat(const double& dData, const CString& strFormat);
	static CString GetStringThousandFormat(const CString& strData);
	
	// ���ϴ� ��Ĺ���� ��Ʈ�� ���� (��¥, �ð� ǥ��)
	CString GetStringFormatTrans(const CString& strData, const CString& strFormat, const CString& strCompartDate, const CString& strCompartTime = "");

	// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CString GetStringFormatTrans2(const CString& strData, const int& nDataUnit, const CString& strCompartDate, const CString& strCompartTime = "");
	// 2011.01.20 by SYS <<

	CString GetStringFormatbyCompart(const CString& strData, CString& strFormat, const CString& strCompartDate, const CString& strCompartTime, CString& strResult);
	static CString GetCompartType(const CString& strFormat, const CString& strCompartDate, const CString& strCompartTime);	
	// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
	int GetStringFormatTransCutoff(const double& dData, const CString& strFormat, CString& strData, CString& strFormatTrans, BOOL p_bWithAutoScale);
	int GetStringFormatTransCutoff(const double& dData, const double& dFormat, CString& strData, CString& strFormatTrans, BOOL p_bWithAutoScale);
	// ���ϴ� ���ڸ� ���ϴ� ������ŭ Ư�� ��Ʈ���� ���� �Ǵ� �߰�
	static CString InsertString(const CString& strData, const CString& strInsert, const int nInsertLength, const int nInsertType);

	// �׸� ���� ���Ѵ�.
	static void SelectClipRegion(CDC* pDC, const CRect& rect);
	// �׸� ������ Ǭ��.
	static void SelectClipRegion(CDC* pDC);

	// ���� ���� ȭ���� point�� ��ȯ�� ���� �����´�.
	static CPoint GetVerticalPoint(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData, const bool bCenter = true);
	static CPoint GetVerticalPoint(const CRect& blockRegion, const double& yMin, const double& yMax, const double& xMin, const double& xMax, const double& nowYData, const double& nowXData);
	static CPoint GetVerticalPointBox(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData, const bool bCenter = true);
	static CPoint GetHorizontalPoint(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min = 1.0, const double& max = 1.0, const double& nowData = 1.0);
	static int GetHorizontalPoint_X(const CRect& blockRegion, const double& min, const double& max, const double& nowData);
	// ������ǥ -> �����ǥ
	static int AbsoluteToRelative_X(const CRect& region, const int absolute_x, const int pageDataCount);
	static int AbsoluteToRelative_X(const CRect& region, const int absolute_x, const double& dataViewMin, const double& dataViewMax);
	static double AbsoluteToRelative_Y(const CRect& region, const int absolute_y, const double& dataViewMin, const double& dataViewMax);
	// �����ǥ -> ������ǥ. (Ư�� ���� ȭ�� Y�� ������ �ٲ���)
	static int RelativeToAbsolute_Y(const CRect& region, const double& relative_y, const double& dataViewMin, const double& dataViewMax);
	static CPoint RelativeToAbsolute(const CRect& region, const int startIndex, const int endIndex, 
			const double& dataViewMin, const double& dataViewMax, 
			const int currentIndex, const double& currentPrice);
	static int RelativeToAbsolute_X(const CRect& region, const int startIndex, const int endIndex, const int currentIndex, const bool bCenter = true);

	// �ش� point�� �ִ� data�� index --> �ڷ������� ���
	static int GetPacketDataIndex_Date(const CRect& drawingRegion, const CPoint& point, const int dataStart, const int dataEnd);
	// ��¥ �ϳ��� x����
	static double GetRealWidth(const double& dWidth, const int nIndex);

	//invert graph�� ���ؼ� dataPt�� invert �Ѵ�.
	static int InvertGraphDataPt(const CRect& region, const int dataPt_y);

	// (2004.10.22, ��¿�) �־��� ������ �ݴ� ���(��� 50%��)�� ������ ���ϴ� Routine�� �����Ѵ�.
	static COLORREF CDataConversion::GetComplementColor( COLORREF p_crOrg);

	static int GetChangingPosition(const int startPos, const int length, const double& originStartData, const double& originLength, const double& nowData);

private:
	static bool IsCalcTimeFormatforCharge(const int nBaseTime, const int nAddIndex, const CString& strFormat, const int nInterval);
	static CString GetAdditionDataInFormat(const int nBaseTime, const int nAddIndex, const CString& strFormat, const int nInterval);
	static bool GetAllTime(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nMon, int& nDay, int& nHour, int& nMin, int& nSec);
	static bool GetAllTime_TimePart(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nDay, int& nHour, int& nMin, int& nSec);
	static bool GetAllTime_DatePart(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nMon, int& nDay);
	static bool GetAllTime_Second(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nSec, int& nMin, bool& bIsIntervalEnd);
	static bool GetAllTime_Minute(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nMin, int& nHour, bool& bIsIntervalEnd);
	static bool GetAllTime_Hour(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nHour, int& nDay, bool& bIsIntervalEnd);
	static bool GetAllTime(const int nAddIndex, const int nInterval, const int nMaxGap, const int nMinGap, 
		const int nLimitData1, int& nData1, int& nData2, bool& bIsIntervalEnd);
	static void GetDataInLimit(const int nLimitData1, int& nData1, int& nData2);
	static int CalcTimeFormat(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsFuture, const bool bIsTransactionTime);
	static int CalcTimeFormat_Future(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsTransactionTime);
	static int CalcTimeFormat_Past(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsTransactionTime);
	static bool GetTimeData(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, 
		int& nBaseDay, int& nBaseHour, int& nBaseMin, int& nBaseSec, 
		int& nAddDay, int& nAddHour, int& nAddMin, int& nAddSec);
	static bool GetTimeData(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, 
		const CString& strFindType, const int nFindStart, int& nBaseData, int& nAddData);
	static int GetTimeInFormat(const CString& strFormat, const int nDay, const int nHour, const int nMin, const int nSec);
	static void GetHourTime_Future(const int nBaseHour, const int nAddHour, 
		const bool bIsTransactionTime, int& nDay, int& nHour);
	// pluse ������ üũ�Ѵ�.
	static bool IsPluse(const CString& data);

	// GetVerticalPoint(), GetHonrizontalPoint() �� �����Լ���
	static int GetChangingPosition(const int nowIndex, const int startPos, const double& width, const bool bCenter);

	// �����ǥ -> ������ǥ : �����ǥ�� �ش� ������ ���� ���
	static CPoint RelativeToAbsoluteInDisplayIndex(const CRect& region, const double& realWidth, 
			const double& dataViewMin, const double& dataViewMax, const int currentIndex, const double& currentPrice);
	// �����ǥ -> ������ǥ : �����ǥ�� �ش� ������ �ۿ� ���� ���
	static CPoint RelativeToAbsoluteOutDisplayIndex(const CRect& region, 
			const double& realWidth, const double& realHeight, const double& dataViewMin, const double& dataViewMax, 
			const int startIndex, const int endIndex, const int currentIndex, const double& currentPrice);
	static int RelativeToAbsoluteInDisplayIndex_X(const CRect& region, const double& realWidth, const int currentIndex, const bool bCenter = true);
	static int RelativeToAbsoluteOutDisplayIndex_X(const CRect& region, const double& realWidth, 
			const int startIndex, const int endIndex, const int currentIndex);
	static int RelativeToAbsoluteInDisplayIndex_Y(const CRect& region, 
			const double& dataViewMin, const double& dataViewMax, const double& currentPrice);
	static int RelativeToAbsoluteOutDisplayIndex_Y(const CRect& region, 
			const double& realHeight, const double& dataViewMin, const double& dataViewMax, const double& currentPrice);

	static bool OnPickoutTime(const int nCurData, const int nCharge, const CString& strFormat, const int nInterval, int& mon, int& day, int& hour, int& min, int& sec);
	static bool OnPickoutDate(const int nValue, const CString& strFormat, const int nType, int& year, int& mon, int& day);	
	static bool OnPickoutDaily(const int nValue, const CString& strFormat, int& year, int& mon, int& day);
	static bool OnPickoutWeekley(const int nValue, const CString& strFormat, int& year, int& mon, int& day);
	static bool OnPickoutMonthly(const int nValue, const CString& strFormat, int& year, int& mon, int& day);

	static int GetRealDailyInTimeFormat(const int nDD, const CString& strFormat);
	static bool GetRealDailyInTimeFormat(const int nOrgData, int& nYear, int& nMonth, int& nDay);
	static void ChagneUprightData_YearAndMonth(int& nYear, int& nMonth);
	static void ChagneUprightData_Day(int& nYear, int& nMonth, int& nDay);
	static int GetMonthFullDay(const int nYear, const int nMonth, const int nDay);
	static bool IsCalUprightData(const int nYear, const int nMonth, const int nDay);

// 20080910 ������		8����/32���� ��ȯ
public:
	static bool IsBaseFormat(const CString& dataType);
	static bool GetBaseFormatInfo(const CString& dataType, int &nBase, int &nDecLen1, int &nDecLen2);
private:
	static CString Gl_Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask) ;
	static CString Gl_Format32String(double dTBValue, int scale, int outdec, int indec) ;

	//20081010 �̹��� 8����/32���� => 10�������� ��ȯ >>	
public:
	static BOOL GetDataToDec(CString strInput, CString strPacketType, CString & strOutput);	
	static BOOL IsValid8_32(CString strInput);
	//20081010 �̹��� <<

	// 20081007 JS.Kim	����ڽð��� ����
public:
	static double	CalcTimeDiff(double dData, int nTimeDiff, int nCurrTimeDiff, CString strType);
	static tm *		CalcCurrentDateTimToPacketTime(int nTimeDiff);

// (2008/11/15 - Seung-Won, Bae) Conversion Time and Double
public:
	static BOOL		ConvertTMtoDouble( tm &p_tmTime, double &p_dTime);
	static BOOL		ConvertDoubletoTM( double &p_dTime, tm &p_tmTime);
	static __int64	GetPacketTypedTime( CString strPacketType, double p_dTime);

// 20081014 �ڵ��� �ش������� �ּҰ��� �����Ѵ�.
public:
	static CString GetMinimumValue(CString strPacketType);

// (2009/10/27 - Seung-Won, Bae) for YYYYMMDDHHMMSS
protected:
	static void		CalcTimeSum_YYYYMM( tm &tmSum, __int64 p_nTime2);
	static void		CalcTimeSum_DDHHMMSS( tm &tmSum, __int64 p_nTime2);
public:
	static __int64 CalcTimeSum( __int64 p_nTime1, __int64 p_nTime2, BOOL p_bTime2IsSecond = FALSE);
	static __int64 GetSecond( __int64 p_nTime, BOOL p_bAfter19710101000000 = FALSE);
};

#endif // !defined(AFX_CONVERSION_H__4EDC5FDA_0443_4CE1_888C_8E01B8820C70__INCLUDED_)
