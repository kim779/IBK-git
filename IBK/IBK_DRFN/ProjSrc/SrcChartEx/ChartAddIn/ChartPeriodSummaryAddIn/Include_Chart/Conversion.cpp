// Conversion.cpp: implementation of the CDataConversion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Conversion.h"

#include <assert.h>
#include <FLOAT.H>

#include "Dll_Load/IMetaTable.h"		// for _LTEXT6()
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CDataConversion

// public ===============================================================================
// conversion (string & �� & color) -------------------------------------------------------------

//RGBColor -> String
CString CDataConversion::RGB2Str(const COLORREF clrValue)
{
	int nR = GetRValue(clrValue) ;
	int nG = GetGValue(clrValue) ;
	int nB = GetBValue(clrValue) ;

	CString strValue ;
	strValue.Format("%d,%d,%d", nR, nG, nB) ;
	return strValue ;
}

COLORREF CDataConversion::Str2RGB(const CString& strColor)
{
	CString strClr = strColor ;
	strClr.TrimLeft() ;
	strClr.TrimRight() ;

	COLORREF clrResult = 0;
	if( strClr.IsEmpty())	return clrResult;

	// (2007/1/10 - Seung-Won, Bae) Support No Comma.
	char cSeperator = ',';
	if( strClr.Find( ',') < 0) cSeperator = '.';

	CString strColorData;
	int nColorDataLen = strClr.GetLength();
	char *szColorData = strColorData.GetBufferSetLength( nColorDataLen);
	strcpy( szColorData, strClr);

	char *szItem = szColorData;
	szColorData = strchr( szColorData, cSeperator);
	if( szColorData) *szColorData++ = '\0';
	clrResult += atoi( szItem);
	if( !szColorData) return clrResult;
	if( !*szColorData) return clrResult;

	szItem = szColorData;
	szColorData = strchr( szColorData, cSeperator);
	if( szColorData) *szColorData++ = '\0';
	clrResult += atoi( szItem) * 256;
	if( !szColorData) return clrResult;
	if( !*szColorData) return clrResult;

	szItem = szColorData;
	clrResult += atoi( szItem) * 65536;
	return clrResult;
};

// string -> bool
bool CDataConversion::atob(const char* string)
{
	if(atoi(string) == 1)
		return true;
	return false;
}

bool CDataConversion::atob(const int nData)
{
	return (nData == 0 ? false: true);
}

//	BOOL --> string
CString CDataConversion::Bool2Str(const BOOL bValue)
{
	int nValue = 0 ;
	if(bValue == FALSE)
		nValue = 0;
	else
		nValue = 1;

	return IntToString(nValue) ;
}

// float --> double
double CDataConversion::atod(const float x)
{
	char ch[10];
	sprintf(ch, "%f", x);
	return atof(ch);
}

// int -> CString
CString CDataConversion::IntToString(const int Num)
{
	CString strData;
	strData.Format("%d", Num);
	return strData;
}

// Num : 1234  length : -1 ==> "12340"
// Num : 1234  length : 0 ==> "1234"
// Num : 1234  length : 3 ==> "123"
// Num : 1234  length : 6 ==> "001234"
// 2011.01.25 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
//CString CDataConversion::IntToString(const int Num, const int length)
CString CDataConversion::IntToString(const __int64 Num, const int length)
{
	CString strData;
	strData.Format( "%I64d", Num);
// 2011.01.25 by SYS <<

	if(length > 0){
		if(strData.GetLength() > length){
			return strData.Left(length);
		}
		else if(strData.GetLength() < length){
			CString temp;
			for(int i = 0; i < length - strData.GetLength(); i++)
				temp += "0";
			return temp + strData;
		}
	}
	else if(length < 0){
		CString temp;
		for(int i = 0; i < length; i++)
			temp += "0";
		return strData + temp;
	}

	return strData;
}

// double -> CString
CString CDataConversion::DoubleToString(const double& Num)
{
	//assert(Num != DBL_MAX);
	//assert(Num != -DBL_MAX);

	CString strData;
	strData.Format("%f", Num);
	strData.TrimRight("0");
	if(strData.Right(1) == '.') strData.TrimRight(".");

	return strData;
}

/*CString CDataConversion::DoubleToString(const double& Num)
{ 
	// Num = 340.000 �� ��츦 �����ؼ� �����κа� �Ǽ��κ��� ����� ����Ѵ�.

	int nNum = (int)floor(Num);
	if(nNum < 0.0)
		nNum = (int)floor(Num + 0.5);
	double dNum = Num - (double)nNum;

	CString temp, str;
	temp.Format("%f", dNum); // �Ҽ��κ�
	if(temp.Left(1) == '1')
		nNum += 1;
	str.Format("%d", nNum);  // �����κ�

	int count = 0;
	// Num�� �Ҽ��κ���(dNum) 0.xxx �κ� �񱳴� 0. �� �����ϰ� �Ѵ�.
	for(int i = 2; i < temp.GetLength(); i++){
		if(temp[i] != '0')
			count = i+1;
	}

	if(count != 0)
		str += '.';
		
	for(i = 2; i < count; i++)
		str += temp[i];

	return str;
}*/

// ������ ���ڸ�(decimalDownCount - �Ҽ��κ�)������ �ٲ۴�.
// ex) Num = 1234.5678   decimalDownCount = 2 -> return : "1234.57" (�ݿø�)
//                       decimalDownCount = 6 -> return : "1234.567800"
//                       decimalDownCount = -2 -> return : "12" (�ݿø�)
CString CDataConversion::DoubleToString(const double& Num, const int decimalDownCount)
{
	if(decimalDownCount == 0){
		return DoubleToString(CMath::Round_Double(Num));
	}
	if(decimalDownCount > 0){
		double dRoundNum = CMath::Round(Num, decimalDownCount +1);
		CString strData = DoubleToString(dRoundNum);
		int nStart = 0;
		int nFindIndex = strData.Find('.');
		if(nFindIndex >= 0)
			nStart = strData.GetLength() - (nFindIndex +1);
		else
			strData += '.';

		if(nStart > decimalDownCount){
			strData = strData.Left(nFindIndex + decimalDownCount + 1);
		}

		for(int nIndex = nStart; nIndex < decimalDownCount; nIndex++)
			strData += '0';

		return strData;
	}

	double dData = CMath::Round(Num, abs(decimalDownCount));
	CString strData;
	strData.Format("%.0f", dData);
	strData = strData.Left(strData.GetLength() + decimalDownCount); // "xxxxxx" -> "xxxx" (negativeCount = 2 ���)
	return strData;
}

// CString -> double
// update 11.27.2001
// atof() �� ���� ���� (sy 2003.04.23.)
// -> % �� ���� �̹��� ���Ÿ� �����̿����� ���ʿ��ϰ� ���Լ��� ���� �Ǵ°� ���� ���� ����.
/*double CDataConversion::StringToDouble(const CString& strData)
{
	CString strDecimal = strData;
	bool isPlus = IsPluse(strDecimal);
	// 03.16.2002	%���� �̹����� �����Ѵ�
	if(strDecimal.Right(1) != "0" && atoi(strDecimal.Right(1)) == 0)
		strDecimal = strDecimal.Left(strDecimal.GetLength() - 1);

	CString strInt = GetStringData(strDecimal, ".");
	if(strData.Find(".") < 0)	{
		strDecimal = "";
		strInt = strData;
	}
	
	double IntData = 0.0, DecimalData = 0.0;
	if(!strInt.IsEmpty() && strInt != "0"){ //���� �κ�
		IntData = atof(strInt);
	}
	if(!strDecimal.IsEmpty() && strDecimal != "0"){  // �Ҽ��� �κ�
		DecimalData = atof(strDecimal)*pow(0.1, (double)strDecimal.GetLength());
	}

	if(isPlus)
		return (IntData + DecimalData);
	return (IntData - DecimalData);
}*/

// ������ ���ڿ��� ��ȯ�Ѵ�.
// �� '.'�� ���������� ����.
CString CDataConversion::NormalNumberString(const CString& strData)
{
	if(strData.IsEmpty())
		return "";

	CString strRealData;
	for(int nIndex = 0; nIndex < strData.GetLength(); nIndex++)
	{
		char cData = strData[nIndex];

		// (2008/3/27 - Seung-Won, Bae) Do not use isdigit(). (It did not work in some system)
		if(cData == '.' || ( '0' <= cData && cData <= '9'))
			strRealData += cData;
	}
	return strRealData;
}

// ���� ���ڿ��� �� data.
double CDataConversion::GetLongLengthData(const double& dData1, const double& dData2, const CString& dataType)
{
	int nDecimalDownCount = GetDecimalDownCount(dataType);
	CString strMin = DoubleToString(dData1, nDecimalDownCount);
	CString strMax = DoubleToString(dData2, nDecimalDownCount);

	if(strMin.GetLength() > strMax.GetLength())
		return dData1;

	return dData2;
}

// ----------------------------------------------------------------------------
//sy 2004.08.18
// int -> time format int
/*int CDataConversion::CalcTimeFormat(const CString& strValue, const CString& strCharge, const CString& strFormat, const bool bIsFuture)
{
	CString szFormat = strFormat, szDD, szSS, szMM, szHH, szDD1, szSS1, szMM1, szHH1, szTime = strValue, szCharge = strCharge;
	szFormat.MakeUpper();	

	if(szFormat.Find("SS") >= 0)	{
		szSS = szTime.Mid(szFormat.Find("SS"), 2);
		szSS1 = szCharge.Mid(szFormat.Find("SS"), 2);
	}
	if(szFormat.Find("MM") >= 0)	{
		szMM = szTime.Mid(szFormat.Find("MM", 2), 2);
		szMM1 = szCharge.Mid(szFormat.Find("MM", 2), 2);
	}
	if(szFormat.Find("HH") >= 0)	{
		szHH = szTime.Mid(szFormat.Find("HH"), 2);
		szHH1 = szCharge.Mid(szFormat.Find("HH"), 2);
	}
	if(szFormat.Find("MMDD") >= 0)	{
		szDD = szTime.Mid(szFormat.Find("MMDD"), 4);
		szDD1 = szCharge.Mid(szFormat.Find("MMDD"), 4);
	}
	else if(szFormat.Find("DD") >= 0)	{
		szDD = szTime.Mid(szFormat.Find("DD"), 2);
		szDD1 = szCharge.Mid(szFormat.Find("DD"), 2);
	}

	int nSS = atoi(szSS), nMM = atoi(szMM), nHH = atoi(szHH), nDD = atoi(szDD),
		nSS1 = atoi(szSS1), nMM1 = atoi(szMM1), nHH1 = atoi(szHH1), nDD1 = atoi(szDD1);

	if(!bIsFuture)
	{
		if(nSS < nSS1)
		{
			nMM --;
			nSS = nSS + 60 - nSS1;
			if(nSS < 0)
				nSS = abs(nSS);
		}
		else 
			nSS -= nSS1;

		if(nMM < nMM1)
		{
			nHH --;
			nMM = nMM + 60 - nMM1;
		}
		else 
			nMM -= nMM1;

		nHH -= nHH1;
		if(nHH < 9)
		{
			if(nDD > 0)
				nDD -- ;
			nHH = 7 + nHH;
		}
		nDD -= nDD1;
	}
	else
	{
		nSS += nSS1;
		nMM += nMM1;
		nHH += nHH1;
		nDD += nDD1;
		if(nSS >= 60)
		{
			if(nSS >= 120)
			{
				nMM += 2;
				nSS -= 120;
			}
			else
			{
				nMM ++;
				nSS -= 60;
			}
		}

		if(nMM >= 60)
		{
			if(nMM >= 120)
			{
				nHH += 2;
				nMM -= 120;
			}
			else
			{
				nHH ++;
				nMM -= 60;
			}
		}

		if(nDD > 0)
		{
			//DDHHMMSS ��¥ type�� ����.
			//?? 16�ð� ���� ��� ��ܰŷ� �Ǵ� ��찡 �ִ�.
			//if(nHH >= 16) {
			//	nDD ++;
			//	nHH -= 7;
			//}

			// MMDD�� ���۵Ǵ� ��� -> ���� �Ϻκ��� ���� �����Ͽ� ó��
			nDD = GetRealDailyInTimeFormat(nDD, strFormat);
			//sy end
		}
		//��¥ type�� ������ ������¥�� ǥ���Ҽ� ����.
		else
		{ 
			//������ �ð��� �������� ������ �ð����� �����Ѵ�.
			if(nHH > 24)
			{	
				nHH = 24;
				nMM = 59;
				nSS = 59;
			}
		}
	}
	szSS = IntToString(nSS, 2);
	szMM = IntToString(nMM, 2);
	szHH = IntToString(nHH, 2);
	if(szFormat.Find("MMDD") >= 0)
		szDD = IntToString(nDD, 4);
	else
		szDD = IntToString(nDD, 2);

	CString strRet = "";
	// ��¥�� ������ ���� ���� �ʴ´�
	if(szFormat.Find("DD") >= 0 && szDD != "00")
		strRet += szDD;
	if(szFormat.Find("HH") >= 0)
		strRet += szHH;
	if(szFormat.Find("MM") >= 0)
		strRet += szMM;
	if(szFormat.Find("SS") >= 0)
		strRet += szSS;

	return atoi(strRet);
}*/

int CDataConversion::CalcTimeFormat(const CString& strBaseValue, const CString& strAddValue, 
		const CString& strFormat, const bool bIsFuture, const bool bIsTransactionTime)
{
	if(bIsFuture)
		return CalcTimeFormat_Future(strBaseValue, strAddValue, strFormat, bIsTransactionTime);
	
	return CalcTimeFormat_Past(strBaseValue, strAddValue, strFormat, bIsTransactionTime);
}

//-> �̷�
//   bIsTransactionTime = true : 9:00 ~ 15:00 �� �ð���� �����Ѵ�.
int CDataConversion::CalcTimeFormat_Future(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsTransactionTime)
{
	int nBaseDay = 0, nBaseHour = 0, nBaseMin = 0, nBaseSec = 0;
	int nAddDay = 0, nAddHour = 0, nAddMin = 0, nAddSec = 0;
	if(!GetTimeData(strBaseValue, strAddValue, strFormat, 
		nBaseDay, nBaseHour, nBaseMin, nBaseSec, nAddDay, nAddHour, nAddMin, nAddSec))
		return 0;

	int nDay = nBaseDay + nAddDay;
	int nHour = nBaseHour + nAddHour;
	int nMin = nBaseMin + nAddMin;
	int nSec = nBaseSec + nAddSec;

	//���ڰ� ���� ������ �ð��� �������� ������ �ð����� �����Ѵ�.
	if(strFormat.Find("DD") < 0)
	{
		// "00�� 59�� 59��"
		if(nHour > 24)
			return GetTimeInFormat(strFormat, 24, 0, 59, 59);
	}

	//��
	GetDataInLimit(60, nSec, nMin);
	//��
	GetDataInLimit(60, nMin, nHour);
	//�� -> ���� �ŷ��ð����� ��������� ���ο� ���� Ʋ����.
	GetHourTime_Future(nBaseHour, nAddHour, bIsTransactionTime, nDay, nHour);
	//��
	nDay = GetRealDailyInTimeFormat(nDay, strFormat);

	return GetTimeInFormat(strFormat, nDay, nHour, nMin, nSec);
}

//-> �̷�(�ð�)
void CDataConversion::GetHourTime_Future(const int nBaseHour, const int nAddHour, 
		const bool bIsTransactionTime, int& nDay, int& nHour)
{
	// �ŷ��ð��� ������� ���� ���. 0 ~ 24��
	if(!bIsTransactionTime)
	{
		GetDataInLimit(24, nHour, nDay);
		return;
	}

	// (2006/7/11 - Seung-Won, Bae) Check Minute Sum over 60.
	int nAddHour2 = nAddHour + ( nHour - nAddHour - nBaseHour);

	// �ŷ��ð��� ����ϴ� ���. 9 ~ 15��
	nDay += (nAddHour2 / 6);
	nHour = nBaseHour + (nAddHour2 % 6);
	if(nHour > 15)
	{
		nHour -= 15;
		if(nHour < 9)
			nHour += 8;

		nDay++;
	}
}

//-> ����
int CDataConversion::CalcTimeFormat_Past(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsTransactionTime)
{
	int nBaseSec = 0, nBaseMin = 0, nBaseHour = 0, nBaseDay = 0;
	int nAddSec = 0, nAddMin = 0, nAddHour = 0, nAddDay = 0;
	if(!GetTimeData(strBaseValue, strAddValue, strFormat, 
		nBaseDay, nBaseHour, nBaseMin, nBaseSec, nAddDay, nAddHour, nAddMin, nAddSec))
		return 0;

	//�̺κ��� �����ڵ��� �״�� ��. ������ϱ⿡ ����..
	if(nBaseSec < nAddSec)
	{
		nBaseMin --;
		nBaseSec = nBaseSec + 60 - nAddSec;
		if(nBaseSec < 0)
			nBaseSec = abs(nBaseSec);
	}
	else 
		nBaseSec -= nAddSec;

	if(nBaseMin < nAddMin)
	{
		nBaseHour --;
		nBaseMin = nBaseMin + 60 - nAddMin;
	}
	else 
		nBaseMin -= nAddMin;

	nBaseHour -= nAddHour;
	if(nBaseHour < 0)
	{
		if(nBaseDay > 0)
			nBaseDay -- ;
		nBaseHour = 24 + nBaseHour;
	}
	nBaseDay -= nAddDay;

	return GetTimeInFormat(strFormat, nBaseDay, nBaseHour, nBaseMin, nBaseSec);
}

int CDataConversion::GetTimeInFormat(const CString& strFormat, const int nDay, const int nHour, const int nMin, const int nSec)
{
	if(strFormat.IsEmpty())
		return 0;

	CString strAllData;
	//��
	if(strFormat.Find("MMDD") >= 0)
		strAllData = IntToString(nDay, 4);
	else if(strFormat.Find("DD") >= 0)
		strAllData = IntToString(nDay, 2);

	//��
	if(strFormat.Find("HH") >= 0)
		strAllData += IntToString(nHour, 2);
	//��
	if(strFormat.Find("MM", 2) >= 0)
		strAllData += IntToString(nMin, 2);
	//��
	if(strFormat.Find("SS") >= 0)
		strAllData += IntToString(nSec, 2);

	return atoi(strAllData);
}

bool CDataConversion::GetTimeData(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, 
		int& nBaseDay, int& nBaseHour, int& nBaseMin, int& nBaseSec, 
		int& nAddDay, int& nAddHour, int& nAddMin, int& nAddSec)
{
	if(strBaseValue.IsEmpty() || strAddValue.IsEmpty() || strFormat.IsEmpty())
		return false;

	//��
	GetTimeData(strBaseValue, strAddValue, strFormat, "SS", 0, nBaseSec, nAddSec);
	//��
	GetTimeData(strBaseValue, strAddValue, strFormat, "HH", 0, nBaseHour, nAddHour);
	//��
	GetTimeData(strBaseValue, strAddValue, strFormat, "MM", 2, nBaseMin, nAddMin);

	//��
	if(strFormat.Find("MMDD") >= 0)
		GetTimeData(strBaseValue, strAddValue, strFormat, "MMDD", 0, nBaseDay, nAddDay);
	else if(strFormat.Find("DD") >= 0)
		GetTimeData(strBaseValue, strAddValue, strFormat, "DD", 0, nBaseDay, nAddDay);

	return true;
}

bool CDataConversion::GetTimeData(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, 
		const CString& strFindType, const int nFindStart, int& nBaseData, int& nAddData)
{
	int nFindIndex = strFormat.Find(strFindType, nFindStart);
	int nLength = strFindType.GetLength();
	if(nFindIndex < 0 || nLength <= 0)
		return false;

	CString strTemp = strBaseValue.Mid(nFindIndex, nLength);
	nBaseData = atoi(strTemp);
	strTemp = strAddValue.Mid(nFindIndex, nLength);
	nAddData = atoi(strTemp);
	return true;
}
//sy end

//sy 2004.08.18
// MMDD�� ���۵Ǵ� ��� -> ���� �Ϻκ��� ���� �����Ͽ� ó��
int CDataConversion::GetRealDailyInTimeFormat(const int nDD, const CString& strFormat)
{
	int nYear = 0, nMonth = 0, nDay = 0;
	if(!GetRealDailyInTimeFormat(nDD, nYear, nMonth, nDay))
		return nDD;

	// ��, ���� �ùٸ��� �����.
	ChagneUprightData_YearAndMonth(nYear, nMonth);
	// ���� �ùٸ��� �����.
	ChagneUprightData_Day(nYear, nMonth, nDay);

	if(strFormat.Find("MMDD") >= 0)
	{
		return (nMonth * 100 + nDay);
	}
	else if(strFormat.Find("DD") >= 0)
	{
		return nDay;
	}

	return nDD;
}

//-> �������� ��, �� ���� ���Ѵ�.
//-> MMDD �� �پ��� �ִ°� ��, ��, �Ϸ� ������ �з��Ѵ�.
bool CDataConversion::GetRealDailyInTimeFormat(const int nOrgData, int& nYear, int& nMonth, int& nDay)
{
	if(nOrgData <= 0)
		return false;

	nDay = nMonth = nYear = 0;
	if(nOrgData >= 10000)
	{
		nYear = nOrgData / 10000;
		nMonth = (nOrgData % 10000) / 100;
		nDay = (nOrgData % 10000) % 100;
	}
	else
	{
		nMonth = nOrgData / 100;
		nDay = nOrgData % 100;
	}

	return true;
}

//-> ��, ���� �ùٸ��� �����.
void CDataConversion::ChagneUprightData_YearAndMonth(int& nYear, int& nMonth)
{
	// ��, ���� ���� ��쿡�� ���� PC�� ��,���� �����Ѵ�.
	if(nYear <= 0 || nMonth <= 0)
	{
		SYSTEMTIME systemTime;
		GetSystemTime(&systemTime);

		if(nMonth <= 0)
			nMonth = systemTime.wMonth;

		if(nYear <= 0)
			nYear = systemTime.wYear;
	}

	while(nMonth > 12)
	{
		nMonth -= 12;
		nYear++;
	}
}

//-> ���� �ùٸ��� �����.
/*void CDataConversion::ChagneUprightData_Day(int& nYear, int& nMonth, int& nDay)
{
	while(IsCalUprightData(nYear, nMonth, nDay))
	{
		if(nMonth == 2)
		{
			if(nYear > 0 && ((nYear % 4) == 0))
			{
				if(nDay <= 29)
					break;
				nDay -= 29;
				nMonth++;
			}
			else
			{
				if(nDay <= 28)
					break;
				nDay -= 28;
				nMonth++;
			}
		}
		else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
		{
			if(nDay <= 30)
				break;

			nDay -= 30;
			nMonth++;
		}
		else
		{
			if(nDay <= 31)
				break;

			nDay -= 31;
			nMonth++;
		}

		if(nMonth > 12)
		{
			nMonth -= 12;
			nYear++;
		}
	}
}*/

void CDataConversion::ChagneUprightData_Day(int& nYear, int& nMonth, int& nDay)
{
	// 12���� ���� �Ѵ��� ��¥�� ���� ���� �Ѵ�.
	while(nDay > 28)
	{
		int nMonthFullDay = GetMonthFullDay(nYear, nMonth, nDay);
		if(nMonthFullDay <= 0)
			return;

		if(nDay <= nMonthFullDay)
			return;

		nDay -= nMonthFullDay;
		nMonth++;

		if(nMonth > 12)
		{
			nMonth -= 12;
			nYear++;
		}
	}
}

int CDataConversion::GetMonthFullDay(const int nYear, const int nMonth, const int nDay)
{
	// 2�� : 28 or 29��
	if(nMonth == 2)
	{
		if(nYear > 0 && ((nYear % 4) == 0))
			return 29;
		return 28;
	}
	// 4, 6, 9, 11�� : 30��
	else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
	{
		return 30;
	}

	return 31;
}

// �ùٸ� month �� day �� �������� ����
bool CDataConversion::IsCalUprightData(const int nYear, const int nMonth, const int nDay)
{
	if(nDay <= 28)
		return false;

	// -> 2��: 28�Ϸ� ���.
	// -> 4, 6, 9, 11�� : 30��
	// -> ������ �� : 31��
	if(nMonth > 12 || nDay > 31)
	{
		return true;
	}
	else if(nMonth <= 0)
	{
		return (nDay > 31);
	}
	else if(nMonth == 2)
	{
		if(nYear > 0 && ((nYear % 4) == 0))
			return (nDay > 29);
		return (nDay > 28);
	}
	else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
	{
		return (nDay > 30);
	}
	else
	{
		return (nDay > 31);
	}

	return false;
}
//sy end

// int -> time format int
int CDataConversion::CalcTimeFormat(const int nValue, const int nCharge, const CString& strFormat, const bool bIsTransactionTime)
{
	if(strFormat.IsEmpty()) return 0;
	
	CString szTime = IntToString(nValue, strFormat.GetLength());	
	CString szCharge = IntToString(abs(nCharge), strFormat.GetLength());
	
	return CalcTimeFormat(szTime, szCharge, strFormat, (nCharge >= 0), bIsTransactionTime);
}

int CDataConversion::CalcTimeFormat_Past(const int nValue, const int nCharge, const CString& strFormat)
{
	if(strFormat.IsEmpty() )
		return 0;
	
	CString szTime = IntToString(nValue, strFormat.GetLength());	
	CString szCharge = IntToString(abs(nCharge), strFormat.GetLength());

	CString strBaseValue = szTime;
	CString strAddValue = szCharge;

	int nBaseDay = 0, nBaseHour = 0, nBaseMin = 0, nBaseSec = 0;
	int nAddDay = 0, nAddHour = 0, nAddMin = 0, nAddSec = 0;
	if(!GetTimeData(strBaseValue, strAddValue, strFormat, 
		nBaseDay, nBaseHour, nBaseMin, nBaseSec, nAddDay, nAddHour, nAddMin, nAddSec))
		return 0;

	int nDay = nBaseDay - nAddDay;
	int nHour = nBaseHour - nAddHour;
	int nMin = nBaseMin - nAddMin;
	int nSec = nBaseSec - nAddSec;

	//���ڰ� ���� ������ �ð��� �������� ó�� �ð����� �����Ѵ�.
	if(strFormat.Find("DD") < 0)
	{
		// "00�� 00�� 00��"
		if(nHour < 0 )
			return GetTimeInFormat(strFormat, 24, 0, 0, 0);
	}

	//��
	while( nSec < 0 )
	{
		nSec += 60;
		nMin--;
	}
	while( nMin < 0 )
	{
		nMin += 60;
		nHour--;
	}
	while( nHour < 0 )
	{
		nHour += 24;
//		nDay--;
	}

	//��
	nDay = GetRealDailyInTimeFormat(nDay, strFormat);

	return GetTimeInFormat(strFormat, nDay, nHour, nMin, nSec);
}

//sy 2002.7.30. -> �̷� �ð� ���ϱ�(�����ҽ����� ����)
/*int CDataConversion::CalcTimeFormatforCharge(const int nValue, const int nCharge, const CString& strFormat, const int nInterval)
{
	if(nValue <= 0 || nCharge == 0)
		return nValue;	

	int mon = 0, day = 0, hour = 0, min = 0, sec = 0;
	if(!OnPickoutTime(abs(nCharge), strFormat, nInterval, mon, day, hour, min, sec))
		return nValue;

	CString szChargeTime = "";
	if(strFormat.Left(2) == "MM")
		szChargeTime += IntToString(mon, 2);
	if(strFormat.Find("DD") >= 0)
		szChargeTime += IntToString(day, 2);
	if(strFormat.Find("HH") >= 0)
		szChargeTime += IntToString(hour, 2);
	if(strFormat.Find("MM") >= 0)
		szChargeTime += IntToString(min, 2);
	if(strFormat.Find("SS") >= 0)
		szChargeTime += IntToString(sec, 2);
	
	CString szTime = IntToString(nValue, strFormat.GetLength());
	
	return CalcTimeFormat(szTime, szChargeTime, nCharge, strFormat);
}*/
//sy end

//sy 2004.08.18
int CDataConversion::CalcTimeFormatforCharge(const int nValue, const int nIndex, const CString& strFormat, const int nInterval, const bool bIsTransactionTime)
{
	if(!IsCalcTimeFormatforCharge(nValue, nIndex, strFormat, nInterval))
		return nValue;	

	CString strAdditionTime = GetAdditionDataInFormat(nValue, nIndex, strFormat, nInterval);
	if(strAdditionTime.IsEmpty())
		return nValue;

	CString strCurTime = IntToString(nValue, strFormat.GetLength());
	return CalcTimeFormat(strCurTime, strAdditionTime, strFormat, (nIndex >= 0), bIsTransactionTime);
}

bool CDataConversion::IsCalcTimeFormatforCharge(const int nBaseTime, const int nAddIndex, const CString& strFormat, const int nInterval)
{
	if(nBaseTime <= 0 || nAddIndex == 0 || strFormat.IsEmpty() || nInterval <= 0)
		return false;	

	CString strRight = strFormat.Right(1);
	if(strRight == "Y" || strRight == "D")
		return false;

	return (strFormat != "YYYYMM");
}

// nAddIndex = 100 ó�� index ���� DDHHMMSS ������ 00006000 ������ �����Ѵ�.
CString CDataConversion::GetAdditionDataInFormat(const int nBaseTime, const int nAddIndex, const CString& strFormat, const int nInterval)
{
	int nMon = 0, nDay = 0, nHour = 0, nMin = 0, nSec = 0;
	if(!GetAllTime(abs(nAddIndex), nInterval, strFormat, nMon, nDay, nHour, nMin, nSec))
		return "";

/*	nMon = nDay = nHour = nMin = nSec = 0;
	if(!OnPickoutTime(nBaseTime, abs(nAddIndex), strFormat, nInterval, nMon, nDay, nHour, nMin, nSec))
		return "";*/

	CString strAdditionTime;
	if(strFormat.Left(2) == "MM")
		strAdditionTime += IntToString(nMon, 2);
	if(strFormat.Find("DD") >= 0)
		strAdditionTime += IntToString(nDay, 2);
	if(strFormat.Find("HH") >= 0)
		strAdditionTime += IntToString(nHour, 2);
	if(strFormat.Find("MM") >= 0)
		strAdditionTime += IntToString(nMin, 2);
	if(strFormat.Find("SS") >= 0)
		strAdditionTime += IntToString(nSec, 2);
	
	return strAdditionTime;
}

// format �� interval �� �̿��� �ش� ������ ��ȯ�Ѵ�.
//->ex) "MMDDHHMMSS", interval = 1000 : 10�� �����̶�� �ǹ�. �� nMin�� data�� ����.
bool CDataConversion::GetAllTime(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nMon, int& nDay, int& nHour, int& nMin, int& nSec)
{
	// �ð� �κ� - ��, ��, ��
	if(!GetAllTime_TimePart(nAddIndex, nInterval, strFormat, nDay, nHour, nMin, nSec))
		return false;

	// ��¥ �κ� - ��, ��
	GetAllTime_DatePart(nAddIndex, nInterval, strFormat, nMon, nDay);
	return true;
}

//-> �ð� �κ�
bool CDataConversion::GetAllTime_TimePart(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nDay, int& nHour, int& nMin, int& nSec)
{
	bool bIsIntervalEnd = false;
	// ��
	GetAllTime_Second(nAddIndex, nInterval, strFormat, nSec, nMin, bIsIntervalEnd);
	// ��
	if(!bIsIntervalEnd)
	{
		GetAllTime_Minute(nAddIndex, nInterval, strFormat, nMin, nHour, bIsIntervalEnd);
	}
	// ��
	if(!bIsIntervalEnd)
	{
		GetAllTime_Hour(nAddIndex, nInterval, strFormat, nHour, nDay, bIsIntervalEnd);
	}

	GetDataInLimit(60, nMin, nHour);
	GetDataInLimit(24, nHour, nDay);
	return (nSec > 0 || nMin > 0 || nHour > 0 || nDay > 0);
}

//-> ��¥ �κ�
bool CDataConversion::GetAllTime_DatePart(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nMon, int& nDay)
{
	// ��
	if(strFormat.Find("DD") >= 0)
	{
	}
	// ��
	if(strFormat.Find("MMDD") >= 0)
	{
	}

	return (nDay > 0 || nMon > 0);
}

//-> "��" ���� �κ�. 
//   bIsIntervalEnd : interval ���� �� ����Ǿ������� ����.
bool CDataConversion::GetAllTime_Second(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nSec, int& nMin, bool& bIsIntervalEnd)
{
	if(nInterval <= 0 || strFormat.Find("SS") < 0)
		return false;

	return GetAllTime(nAddIndex, nInterval, 100, 1, 60, nSec, nMin, bIsIntervalEnd);
}

//-> "��" ���� �κ�.
bool CDataConversion::GetAllTime_Minute(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nMin, int& nHour, bool& bIsIntervalEnd)
{
	if(strFormat.Right(4) == "MMSS")
	{
		return GetAllTime(nAddIndex, nInterval, 10000, 100, 60, nMin, nHour, bIsIntervalEnd);
	}
	else if(strFormat.Right(4) == "HHMM")
	{
		return GetAllTime(nAddIndex, nInterval, 100, 1, 60, nMin, nHour, bIsIntervalEnd);
	}

	return false;
}

//-> "��" ���� �κ�.
bool CDataConversion::GetAllTime_Hour(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nHour, int& nDay, bool& bIsIntervalEnd)
{
	//ing
	int nFindIndex = strFormat.Find('H');
	if(nFindIndex < 0)
		return false;

	int nMaxGap = CMath::Power(10, (strFormat.GetLength() - nFindIndex));
	int nMinGap = nMaxGap / 100;

	return GetAllTime(nAddIndex, nInterval, nMaxGap, nMinGap, 24, nHour, nDay, bIsIntervalEnd);
}


bool CDataConversion::GetAllTime(const int nAddIndex, const int nInterval, const int nMaxGap, const int nMinGap, 
		const int nLimitData1, int& nData1, int& nData2, bool& bIsIntervalEnd)
{
	if(nLimitData1 <= 0 || nMaxGap <= 0 || nMinGap <= 0)
		return false;

	bIsIntervalEnd = (nInterval < nMaxGap);
	int nAddData = nAddIndex * ((nInterval % nMaxGap) / nMinGap);
	nData1 += nAddData;

	GetDataInLimit(nLimitData1, nData1, nData2);
	return (nAddData > 0);
}

void CDataConversion::GetDataInLimit(const int nLimitData1, int& nData1, int& nData2)
{
	while(nData1 >= nLimitData1)
	{
		nData1 -= nLimitData1;
		nData2++;
	}
}
//sy end

int CDataConversion::CalcDateFormat(const int nValue, const int nCharge, const CString& strFormat, const int nCalcType, const int nInterval)
{
	if(strFormat.IsEmpty())
		return 0;
	
	int year, mon, day = 0, len, year_c, mon_c, day_c = 0;
	if(OnPickoutDate(abs(nCharge), strFormat, nCalcType, year_c, mon_c, day_c) == false)
		return 0;
	
	//	int ndate = nValue + nCharge;
	CString strDate = IntToString(nValue, strFormat.GetLength());
	if((len = GetStringDataCount(strFormat, "Y")) > 0)
		year = atoi(strDate.Mid(strFormat.Find("Y"), len));
	if((len = GetStringDataCount(strFormat, "M")) > 0)
		mon = atoi(strDate.Mid(strFormat.Find("M"), len));
	if((len = GetStringDataCount(strFormat, "D")) > 0)
		day = atoi(strDate.Mid(strFormat.Find("D"), len));
	
	if(nCharge > 0)	{
		if((day += day_c) > 30)	{
			int nmon = day / 30;
			day -= 30 * nmon;
			mon += nmon;
		}
		if((mon += mon_c) > 12)	{
			int nyear = mon / 12;
			mon -= 12 * nyear;
			year += nyear;
		}
		year += year_c;
	}
	else if(nCharge < 0)	{
		if((day -= day_c) <= 0)	{
			int nmon = day / 30 - 1;
			day -= 30 * nmon;
			mon += nmon;
		}
		if((mon -= mon_c) <= 0)	{
			int nyear = mon / 12 - 1;
			mon -= 12 * nyear;
			year += nyear;
		}
		year -= year_c;
	}
	else
		return nValue;

	CString retStr = IntToString(year, GetStringDataCount(strFormat, "Y")) + IntToString(mon, 2);
	if(strFormat.Find("DD") >= 0)
		retStr += IntToString(day, 2);

	return atoi(retStr) ;
}

// 20080805 ������ 24�ð� �ü�  <<
CString CDataConversion::GetRealDateInTimeFormat(const int nDD, const CString& strFormat)
{
	int nYear = 0, nMonth = 0, nDay = 0;
	if(!GetRealDailyInTimeFormat(nDD, nYear, nMonth, nDay))
	{
		CString strTemp;
		strTemp.Format("%d", nDD);
		return strTemp;//2022.7.27.tour2k
		//return nDD;
	}

	// ��, ���� �ùٸ��� �����.
	ChagneUprightData_YearAndMonth(nYear, nMonth);
	// ���� �ùٸ��� �����.
	ChagneUprightData_Day(nYear, nMonth, nDay);

	bool bIsYear(false), bIsMonth(false), bIsDay(false);

	// "YYYY..." : "YYYYMM", "YYYYMMDD"
	if(strFormat.Find("YYYY") >= 0)
		bIsYear = true;

	// "MMDD...", "YYYYMM..." : "YYYYMM", "YYYYMMDD", "MMDD", "MMDDHHSS"
	if(strFormat.Find("MMDD") >= 0 || strFormat.Find("YYYYMM") >= 0)
		bIsMonth = true;
	
	// "...DD..." : "YYYYMMDD", "MMDD", "MMDDHHMMSS", "DDHHMMSS"
	if(strFormat.Find("DD") >= 0)
		bIsDay = true;


	CString strDate;
	//strDate.Format("%04d%02d%02d", (bIsYear ? nYear : ""), (bIsMonth ? nMonth: ""), (bIsDay ? nDay: ""));
	if( bIsYear && bIsMonth && bIsDay )
		strDate.Format("%04d%02d%02d", nYear, nMonth, nDay);
	else if( bIsMonth && bIsDay )
		strDate.Format("%02d%02d", nMonth, nDay);
	else if( bIsDay )
		strDate.Format("%02d", nDay);

	return strDate;
}
//>>

// data ���� (string ��) ----------------------------------------------------------------
// data���� ó������ trimStr�� �ִ� ���ڱ����� �ڸ���.
// - ex) GetStringData("It is trim test", "t")
// bIsInside = true  -> return : "I", data : " is trim test"
// bIsInside = false -> return : "It", data : " is trim test"
CString CDataConversion::GetStringData(CString& data, const CString& trimData, const bool bIsInside)
{
	if(data.IsEmpty())
		return "";

	int count = data.Find(trimData);
	if(count < 0)
		return "";

	int length = data.GetLength();
	CString retData;
	if(count > 0)
		retData = data.Left(count);
	data = data.Mid(count+trimData.GetLength(), length - count);

	if(bIsInside)
		return retData;
	return (retData + trimData);
}

CString CDataConversion::GetStringData(CString& data, const CString& trimStart, const CString& trimEnd, const bool bIsInside)
{
	if(data.IsEmpty())
		return "";

	GetStringData(data, trimStart);
	CString strInsideData = GetStringData(data, trimEnd);

	if(bIsInside)
		return strInsideData;
	return (trimStart + strInsideData + trimEnd);
}

// trimStr�� nIndex ��°������ ���ڱ����� �ڸ���.
// - ex) GetStringData("It is trim test", " ", 3) -> return : "It is trim", data : "test"
CString CDataConversion::GetStringData(CString& data, const CString& trimStr, const int nIndex)
{
	if(data.IsEmpty())
		return "";

	CString tempData = data;
	CString retData;
	int totalCount = 0;
	for(int i = 0; i < nIndex; i++){
		int count = data.Find(trimStr);
		if(count == -1)
			break;
		data = data.Mid(count+trimStr.GetLength(), tempData.GetLength() - count);
		totalCount += count;
		totalCount += trimStr.GetLength();
	}
	retData = tempData.Left(totalCount);
	return retData;
}

// trimStr�� nIndex ��°������ string. -> nIndex > 0
// - ex) GetLeftToTrimDataIndex("It is trim test", " ", 2, true) -> return : "It is trim"
//												            false -> return : "It is trim "
CString CDataConversion::GetLeftToTrimDataIndex(const CString& data, const CString& trimStr, const int nEndIndex, const bool bIsInside)
{
	int nFindIndex = FindIndex(data, trimStr, nEndIndex);
	if(nFindIndex < 0)
		return "";

	if(!bIsInside)
		nFindIndex += trimStr.GetLength();

	return data.Left(nFindIndex);
}

// - ex) GetMid..("�ð�,��,����,", ",", 0, true, true) -> return : "��"
//										     false, false -> return : ",��,"
// nIndex : trimStart Index
CString CDataConversion::GetMidToTrimDataStartIndex(const CString& data, const CString& trimStr, const int nStartIndex, const bool bIsInside_start, const bool bIsInside_end)
{
	if(nStartIndex < 0 || data.Find(trimStr) < 0)
		return "";

	// get�� ���� ��ġ.
	int nFirstIndex = CDataConversion::FindIndex(data, trimStr, nStartIndex);
	if(nFirstIndex < 0)
		return "";

	// ���� trimdata ���Ծ��� ���
	if(bIsInside_start)
		nFirstIndex += trimStr.GetLength();

	// get�� �� ��ġ.
	int nEndIndex = CDataConversion::FindIndex(data, trimStr, nStartIndex +1);
	if(nEndIndex < 0){
		nEndIndex = data.GetLength();
	}

	int nCount = nEndIndex - nFirstIndex;
	if(!bIsInside_end){ // trim data�� �޺κ� �����ϴ� ���
		if(nEndIndex < data.GetLength())
			nCount += trimStr.GetLength();
	}

	return data.Mid(nFirstIndex, nCount);
}

// - ex) GetMid..("�ð�,��,����,", ",", 1, true, true) -> return : "��"
//											 false, false -> return : ",��,"
// nIndex : trimEnd Index
CString CDataConversion::GetMidToTrimDataEndIndex(const CString& data, const CString& trimStr, const int nEndIndex, const bool bIsInside_start, const bool bIsInside_end)
{
	int nFindIndex = FindIndex(data, trimStr, nEndIndex);
	if(nFindIndex < 0)
		return "";

	// get�� ���� ��ġ�� ���ϱ�.
	int nFirst = 0;
	if(nEndIndex > 0)
		nFirst = FindIndex(data, trimStr, nEndIndex -1);

	// trim data�� �պκ� ���Ծ��ϴ� ���
	if(bIsInside_start){
		// - ó���� trimStr �� ���� ��� �����ϱ�
		if(nFirst > 0 || data.Left(trimStr.GetLength()) == trimStr){
			nFirst += trimStr.GetLength();
		}
	}

	int nCount = nFindIndex - nFirst;
	if(!bIsInside_end) // trim data�� �޺κ� �����ϴ� ���
		nCount += trimStr.GetLength();

	return data.Mid(nFirst, nCount);
}

// - ex) GetMid..("It is trim test", " ", 2, true) -> return : "trim"
//									         false -> return : "trim "
// - ex) GetMid..("�ð�,��,����,", ",", 1, true) -> return : "��"
//									         false -> return : "��,"
CString CDataConversion::GetMidToTrimDataIndex(const CString& data, const CString& trimStr, const int nEndIndex, const bool bIsInside)
{
	return GetMidToTrimDataEndIndex(data, trimStr, nEndIndex, true, bIsInside);
}

// - ex) GetMid..("It is trim test", " ", " ", 0, true) -> return : "is"
//											      false -> return : " is "
//										 	   1, true) -> return : "trim"
CString CDataConversion::GetMidToTrimDataIndex(const CString& data, const CString& trimStart, const CString& trimEnd, const int nStartIndex, const bool bIsInside)
{
	if(data.IsEmpty())
		return "";

	// trim ������ ��ġ�� ã�´�.
	// ���� trim ���ڰ� null �� ��� ���� ó���� ���� �ǹ��Ѵ�.
	int nFindIndex_trimStart = FindIndex(data, trimStart, nStartIndex);
	int nFindIndex_trimEnd = -1;
	if(trimEnd.IsEmpty())
		nFindIndex_trimEnd = data.GetLength();
	else
		nFindIndex_trimEnd = data.Find(trimEnd, nFindIndex_trimStart +trimStart.GetLength());

	if(nFindIndex_trimStart < 0 || nFindIndex_trimEnd < 0)
		return "";

	// ������ string�� ��ġ�� ������ ���Ѵ�.
	int nFirst = nFindIndex_trimStart;
	if(bIsInside)
		nFirst += trimStart.GetLength();

	int nCount = nFindIndex_trimEnd - nFirst;
	if(!bIsInside)
		nCount += trimEnd.GetLength();

	return data.Mid(nFirst, nCount);
}

// �ش� ���ڰ� �� � ������ �ִ����� ���Ѵ�.
int CDataConversion::GetStringDataCount(const CString& orgStr, const CString& comparisonStr)
{
	if(orgStr.IsEmpty() != 0 || comparisonStr.IsEmpty() != 0)
		return -1;

	// (2005/12/8 - Seung-Won, Bae) for Performance. Do without CString
	int count = 0;
	const char *szFound = orgStr;
	while( ( szFound = strstr( szFound, comparisonStr)) != NULL)
	{
		count++;
		szFound += comparisonStr.GetLength();
	}

	return count;
}

// �ش� index�� ã�´�.
// orgData ���� nFindDataCountIndex ��° �ִ� findData�� index.
// ex) FindIndex("It is testing", "t", 2) -> return : 9
//     FindIndex("It is testing", "t", 3) -> return : -1
int CDataConversion::FindIndex(const CString& orgData, const CString& findData, const int nFindDataCountIndex)
{
	if(orgData.IsEmpty() || findData.IsEmpty() || nFindDataCountIndex < 0)
		return -1;

	int nFindStartIndex = -1;
	for(int nCount = 0; nCount <= nFindDataCountIndex; nCount++){
		int nIndex = orgData.Find(findData, nFindStartIndex +1);
		if(nIndex < 0)
			return -1;

		nFindStartIndex = nIndex;
	}

	return nFindStartIndex;
}

// �Ҽ��� ���� ���ڸ������� �����´�.  0.001 -> return : 3
// bIsCheckDigit = true -> x0.01% -> return : 2
// bIsCheckDigit = false -> x0.01% -> return : 3
int CDataConversion::GetDecimalDownCount(const CString& strDecimalType, const bool bIsCheckDigit) 
{
	// 20080910 JS.Kim		8����/32���� ó��, �Ҽ��� ���� �ڸ��� �ִ��ڸ��� Return <<
	if( IsBaseFormat(strDecimalType) )
	{
		int nBase, nDecLen1, nDecLen2;
		if( GetBaseFormatInfo(strDecimalType, nBase, nDecLen1, nDecLen2) )
			return nDecLen1 + nDecLen2;
		return 0;
	}
	//>>

	if(strDecimalType.IsEmpty() || strDecimalType.Find('.') < 0)
		return 0;

	CString strType = strDecimalType;
	if(bIsCheckDigit)
		strType = NormalNumberString(strDecimalType);

	int nIndex = strType.Find(".");
	if(strType.IsEmpty() || nIndex < 0)
		return 0;

	return (strType.GetLength() - (nIndex +1));
}

// data type�� double ���·� ��ȯ
// "x0.01%", "x0.01", "X0.01%", "X0.01", "��0.01%", "��0.01", "1,000" ��
// �̷������� ���ڿ� �Ҽ����� ������ Ÿ���� ��츸 ����Ѵ�.
double CDataConversion::GetDataType(CString strDataType)
{
	strDataType.Remove('%');
	strDataType.Remove('x');
	strDataType.Remove('X');
	strDataType.Remove(',');
	strDataType.Replace("��", "");

	return atof(strDataType);
}

// "x10000" -> "x��" ���·� ��ȯ (10000 �̻� �ѱ۷� �ٲ۴�.)
CString CDataConversion::GetChangeFormat_NumCharUnit(const CString& strOrgFormat)
{
	CString strNewFormat = strOrgFormat;

	if(strOrgFormat.Find("1000000000000") != -1){	// ��
		strNewFormat.Replace("1000000000000", _LTEXT6( C6_TRILLION));
	}
	else if(strOrgFormat.Find("100000000000") != -1){// õ��
		strNewFormat.Replace("100000000000", _LTEXT6( C6_HUNDRED_BILLION));
	}
	else if(strOrgFormat.Find("10000000000") != -1){// ���
		strNewFormat.Replace("10000000000", _LTEXT6( C6_TEN_BILLION));
	}
	else if(strOrgFormat.Find("1000000000") != -1){	// �ʾ�
		strNewFormat.Replace("1000000000", _LTEXT6( C6_BILLION));
	}
	else if(strOrgFormat.Find("100000000") != -1){	// ��
		strNewFormat.Replace("100000000", _LTEXT6( C6_HUNDRED_MILLION));
	}
	else if(strOrgFormat.Find("10000000") != -1){	// õ��
		strNewFormat.Replace("10000000", _LTEXT6( C6_TEN_MILLION));
	}
	else if(strOrgFormat.Find("1000000") != -1){	// �鸸
		strNewFormat.Replace("1000000", _LTEXT6( C6_MILLION));
	}
	else if(strOrgFormat.Find("100000") != -1){		// �ʸ�
		strNewFormat.Replace("100000", _LTEXT6( C6_HUNDRED_THOUSAND));
	}
	else if(strOrgFormat.Find("10000") != -1){		// ��
		strNewFormat.Replace("10000", _LTEXT6( C6_TEN_THOUSAND));
	}

	return strNewFormat;
}

// �⺻ ���ڿ��� ���ϴ� ���ڿ��� ���ϴ� ����ŭ �ݺ��ؼ� insert �Ǵ� append �Ѵ�
// strData : �⺻���ڿ�, strInsert : ������ ���ڿ�, nInsertLength : ������ ���ڿ��� ����
// insert type : 0 -> insert, 1 -> append
CString CDataConversion::InsertString(const CString& strData, const CString& strInsert, const int nInsertLength, const int nInsertType)
{
	if(nInsertLength <= 0)
		return strData;
	CString szData = strData, szInsert;

	for(int i = 0 ; i < nInsertLength ; i++)	{
		szInsert += strInsert;
	}
	if(nInsertType == 0)	{
		szData = szInsert + szData;
	}
	else if(nInsertType == 1)	{
		szData += szInsert;
	}

	return szData;
}

// ���ϴ� ��Ʈ���� �����ؼ� ���˰� ��Ʈ��, ��ȯ�� ��Ʈ�� ��������� ��ȯ�Ѵ�
// ����Ÿ�� �鸸�������� ū ��쿡�� ����
// �� ���� ��쿡�� GetStringFormatTrans() ����� ����� ���� strdata�� �����ȴ�
// strData : ��ȯ�� ����Ÿ, strFormatTrans : ��ȯ�� format
// return : ��ȯ�� ����Ÿ�� ����
int CDataConversion::GetStringFormatTransCutoff(const double& dData, const CString& strFormat, CString& strData, CString& strFormatTrans, BOOL p_bWithAutoScale)
{
	// 20080910 JS.Kim		���� ǥ���� ���	<<
	if( IsBaseFormat(strFormat) )
	{
		strFormatTrans = strFormat;
		CString szData = DoubleToString(dData);
		strData = GetStringFormatTrans(szData, strFormat);
		return strData.GetLength();	
	}
	//>>

	CString szFormatType, szFormat = strFormat;
	//strFormatTrans = szFormat;

	if(!strFormat.IsEmpty() && strFormat.Right(1) != "0" && strFormat.Right(1) != "1")	{
		szFormatType = strFormat.Right(1);
		szFormat = strFormat.Left(strFormat.GetLength() -1);
	}

	strFormatTrans = szFormat;

	CString szData = DoubleToString(dData);
	// 08.04.2002  - data�� double�ε� Ÿ���� ������ ��� �Ҽ��� ���� ����
	if(strFormat.Right(1) == "1" && szData.Find(".") > 0){
		int nDecimalDownCount = 0;
		int nFormatIndex = strFormat.Find(".");
		if(nFormatIndex >= 0 && nFormatIndex <= (strFormat.GetLength() -1))
			nDecimalDownCount = strFormat.GetLength() - (nFormatIndex +1);
		szData = DoubleToString(dData, nDecimalDownCount);
	}
	
	if( szFormat.Find(".") < 0)
	{
		//sy 2002.10.28.
		if( szData.Find(".") != -1)  szData = GetStringData(szData, ".");

		int nStartIndex = szFormat.Find( "1");
		if( 0 <= nStartIndex)
		{
			int nTruncCount = szFormat.Mid( nStartIndex).Replace( '0', '_');
			if( 0 < nTruncCount) szData = szData.Left( szData.GetLength() - nTruncCount);
		}
	}

	//sy 2002.9.11. -> 1140000 : ���� -114000 : ���� ���� (������ - ���� ����)
	int nNumLength = szData.GetLength();
	if( p_bWithAutoScale && ( ( szData.Left(1) == '-' && nNumLength > 7) || (szData.Left(1) != '-' && nNumLength > 6)))
	{
		if(szFormat.Find(".") < 0)
		{
			strData = szData.Left(szData.GetLength() - (szData.GetLength() - 6));
			strFormatTrans = InsertString(szFormat, "0", szData.GetLength() - 6, 1);//strFormat + "0";
		}
		else if(szFormat.Find(".") >= 0)
		{
			int npoint = GetStringDataCount(szFormat, "0"); //�Ҽ��� �ڸ��� ������ �ȴ�
			strData = szData.Left(szData.GetLength() - (szData.GetLength() - 6));
			int npow = szData.GetLength() - 6;
			if(szData.Find(".") > 0)
				npow -= szData.GetLength() - szData.Find(".");
			if(npow > 0)	{
				strFormatTrans.Delete(szFormat.Find(".") - 1, npoint + 1);
				strFormatTrans = InsertString(strFormatTrans, "0", npow, 1);
			}
		}
	}
	else
	{
		strData = szData;
		strFormatTrans = szFormat;
	}

	strFormatTrans += szFormatType;
	strFormatTrans.Replace(" ", "");

	strData = GetStringFormatTrans(strData, strFormatTrans);
	return strData.GetLength();	
}

int CDataConversion::GetStringFormatTransCutoff(const double& dData, const double& dFormat, CString& strData, CString& strFormatTrans, BOOL p_bWithAutoScale)
{
	return GetStringFormatTransCutoff(dData, DoubleToString(dFormat), strData, strFormatTrans, p_bWithAutoScale);
}

// ���ϴ� �������� ��Ʈ�� ����
//sy 2003.8.8. -> packet type �� ���� ������ ������ �ϴ� ���� ����.
CString CDataConversion::GetStringFormatTrans(const CString& strData, const CString& strFormat/*, const bool bIsRound*/)
{
	if(strData.IsEmpty())
		return strData;

	if(strFormat.IsEmpty())
		return GetStringThousandFormat(strData);

	int nFindIndex_1 = strFormat.Find("1");
	int nFormatLength = strFormat.GetLength();
	// 03.19.2002  format�� ��¥ ���� ��쿡�� ��¥ ���� ���� �Լ��� �������� �Ѵ�
	if(nFindIndex_1 < 0 && nFormatLength > 1)
		return GetStringFormatTrans(strData, strFormat, "");

	CString strUnit; // ���� (%)

	// 20080910 JS.Kim				<<
	//	�̱�ää�� ǥ�ø� ���� 8����/32���������� ��ȯ
	int nBase, nDecLen1, nDecLen2;
	if( GetBaseFormatInfo(strFormat, nBase, nDecLen1, nDecLen2) )
	{
		// �������ϴ� ��� ����. '0'�� �ƴ� ���� üũ���� �ʰ� ���ڰ� �ƴϸ� ������ ó��
		CString strUnit; // ���� (%)
		char chData;
		while( nFormatLength > 0 )
		{
			chData = strFormat.GetAt(--nFormatLength);
			if(chData < '0' || chData > '9')
				strUnit.Insert(0, chData);
			else
				break;
		}
		
		return GetStringThousandFormat(Gl_Format32String(atof(strData), nBase, nDecLen1+nDecLen2, nDecLen2)) + strUnit;
	}
	//>>

	// ������ ���
	if(strFormat.Right(1) != "0" && nFindIndex_1 != nFormatLength -1){
		strUnit = strFormat.Right(nFormatLength - (nFindIndex_1 + 1));
	}

	double dData = atof(strData);
	int nDecimalDownCount = GetDecimalDownCount(strFormat, true);
	if(nDecimalDownCount < 0)
		nDecimalDownCount = 0;

	// data type ������ ��� (x1, x10, x0.1, x0.01)
	CString strRealData = DoubleToString(dData, nDecimalDownCount);
	return GetStringThousandFormat(strRealData) + strUnit;
}

/* packet type �� ���� �ݿø��� �ƴ� ���� ���·� �ٲ�����.
CString CDataConversion::GetStringFormatTrans(const CString& strData, const CString& strFormat)
{	
	if(strData.IsEmpty() == TRUE)
		return strData;

	if(strFormat.IsEmpty() == TRUE)
		return GetStringThousandFormat(strData);

	// 03.19.2002  format�� ��¥ ���� ��쿡�� ��¥ ���� ���� �Լ��� �������� �Ѵ�
	if(strFormat.Find("1") < 0 && strFormat.GetLength() > 1)
		return GetStringFormatTrans(strData, strFormat, "");

	CString szFormat = strFormat, szUnit = "", szMinus = "", szData = strData;

	if(strFormat.Right(1) != "0" && atoi(strFormat.Right(1)) == 0)	{
		szFormat = strFormat.Left(strFormat.GetLength() - 1);
		szUnit = strFormat.Right(1);
	}	

	int nDecimalType = GetDecimalDownCount(szFormat);

	if(strData.Left(1) == "-")	{
		szMinus = strData.Left(1);
		szData = strData.Right(strData.GetLength() - szMinus.GetLength());
	}
	// 08.04.2002  - data�� double�ε� Ÿ���� ������ ��� �Ҽ��� ���� ����
//	if(szFormat.Find(".") < 0 && szFormat.Right(1) == "1" && strData.Find(".") > 0)
//		szData = strData.Left(strData.Find("."));
	if(szFormat.Find(".") < 0 && szFormat.Right(1) == "1" && szData.Find(".") > 0)
		szData = szData.Left(szData.Find("."));

	if(nDecimalType > 0){
		// 08.05.2002 ����Ÿ�� �Ҽ����� �ִ� ��� ���� �Ҽ��� ����Ÿ ���ϱ�
		if(szData.Find(".") > 0)	{
			if(nDecimalType > szData.GetLength() - szData.Find(".") - 1)	{
				//sy 2002.8.28.
				int nZeroCount = nDecimalType - (szData.GetLength() - szData.Find(".") - 1);
				for(int i = 0 ; i < nZeroCount; i++)
					szData += "0";
				//end
			}
			else
				szData = szData.Left(szData.Find(".")+1+nDecimalType);
		}
		// ����Ÿ�� �Ҽ����� ���� ����Ÿ Ÿ�Կ� �Ҽ����� �ִ� ��� �Ҽ��� ���ϱ�
		else	{			
			CString szDecimal('0', nDecimalType);
			szData = szData + "." + szDecimal;
		}

		int numberCount = szData.GetLength() - nDecimalType - 1;
		if(numberCount > 0)			
			return szMinus + GetStringThousandFormat(szData.Left(numberCount)) + szData.Right(nDecimalType + 1) + szUnit;
		else{
			CString temp;
			for(int i = numberCount; i < 0; i++)
				temp += "0";
			return szMinus + "0." + temp + szData + szUnit;
		}
	}

	return szMinus + GetStringThousandFormat(szData) + szUnit;
}*/

CString CDataConversion::GetStringThousandFormat(const double& dData, const double& dFormat)
{
	int decimalDownCount = 0;
	// 0.1, 0.01 ����
	if(dFormat < 1.0){
		double temp = dFormat;
		while(temp < 1.0){
			temp *= 10;
			decimalDownCount++;
		}
	}
	// 10, 100, 1000 ����
	else if(dFormat > 1.0){
		double temp = dFormat;
		while(temp > 1.0){
			temp /= 10;
			decimalDownCount--;
		}
	}

	CString strData = DoubleToString(dData, decimalDownCount);
	CString strNumberData = strData, strDecimalData;
	if(strData.Find('.') >= 0){
		strNumberData = GetStringData(strData, '.');
		strDecimalData = strData;
	}

	if(strNumberData.IsEmpty())
		return "";

	strNumberData = GetStringThousandFormat(strNumberData);
	if(!strDecimalData.IsEmpty() && strDecimalData.Left(1) != '.')
		strDecimalData = '.' + strDecimalData;

	return strNumberData + strDecimalData;
}

// 20080910 JS.Kim		�̱�ää�� ǥ�ø� ���� 8����/32���������� ��ȯ�� ���� �Լ� �߰�<<
CString CDataConversion::GetStringThousandFormat(const double& dData, const CString& strFormat)
{
	CString strData = DoubleToString(dData, strFormat);
	CString strNumberData = strData, strDecimalData;
	if(strData.Find('\'') >= 0)
	{
		strNumberData = GetStringData(strData, '\'');
		strDecimalData = strData;
		if(!strDecimalData.IsEmpty() && strDecimalData.Left(1) != '\'')
			strDecimalData = '\'' + strDecimalData;
	}
	else if(strData.Find('.') >= 0)
	{
		strNumberData = GetStringData(strData, '.');
		strDecimalData = strData;
		if(!strDecimalData.IsEmpty() && strDecimalData.Left(1) != '.')
			strDecimalData = '.' + strDecimalData;

	}

	if(strNumberData.IsEmpty())
		return "";

	strNumberData = GetStringThousandFormat(strNumberData);

	return strNumberData + strDecimalData;
}
//>>

// ���ϴ� �������� ��Ʈ�� ���� (õ�������� , �� ǥ��)
CString CDataConversion::GetStringThousandFormat(const CString& strData)
{
	CString temp = strData;
	bool puls = true;
	if(temp.Find('-') == 0){
		puls = false;
		temp.TrimLeft('-');
	}
	if(temp.GetLength() <= 3)
		return strData ;

	CString strDecimal, strT = temp;
	//sy -> �Ҽ��� ���� �κ�
	// 20080910 JS.Kim		�̱�ää�� ǥ�ø� ���� 8����/32���������� ��ȯ�� ���� ����<<
	if(strT.Find('\'') != -1){
		strDecimal = strT.Right(strT.GetLength() - strT.Find('\''));
		strT.Replace(strDecimal, "");
	}
	//>>
	else if(strT.Find('.') != -1){
		strDecimal = strT.Right(strT.GetLength() - strT.Find('.'));
		strT.Replace(strDecimal, "");
	}
	//end
	
	CString str;
	while(!strT.IsEmpty())	{
		if(strT.GetLength() > 3)	{
			str = "," + strT.Right(3) + str ;
			strT.Delete(strT.GetLength() - 3, 3) ;
		}
		else	{
			str = strT + str ;
			strT.Empty();
		}
	}
	if(puls)
		return str + strDecimal;
	return '-' + str + strDecimal;
}

CString CDataConversion::GetStringFormatTrans(const CString& strData, const CString& strFormat, 
											  const CString& strCompartDate, const CString& strCompartTime)
{
	CString szFormat = strFormat, szData = "", szCompartDate = strCompartDate, szCompartTime = strCompartTime ;

	if(strCompartDate.IsEmpty())
		szCompartDate = "/" ;
	if(strCompartTime.IsEmpty())
		szCompartTime = ":" ;

	szFormat.TrimLeft();
	szFormat.TrimRight();

	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//CString strValue = IntToString(atoi(strData), szFormat.GetLength());
	CString strValue = IntToString(_atoi64(strData), szFormat.GetLength());
	// 2011.01.06 by SYS <<

	//sy 2004.04.16
	CString strSpecialTime;
	if(GetSpecialTime(strFormat, strData, strSpecialTime))
		return strSpecialTime;
	//end

	GetStringFormatbyCompart(strValue, szFormat, szCompartDate, szCompartTime, szData);

	return szData;
}

CString CDataConversion::GetStringFormatbyCompart(const CString& strData, CString& strFormat, 
												  const CString& strCompartDate, const CString& strCompartTime, 
												  CString& strResult)
{
	if(strData.IsEmpty())
		return "" ;
	CString szType = strFormat.Left(1);

	int nFcount = 0;
	int length = szType.GetLength();
	int nMax = strFormat.GetLength() - length;
	for(int i = 0; i <= nMax; i++){
		if(strFormat.Mid(i, length) == szType)
			nFcount++;
		else
			break;
	}

	if(strFormat.GetLength() == nFcount)	{
		strResult = strResult + strData ;
		return "" ;
	}

	//sy 2004.04.16
	CString strSpecialTime;
	if(GetSpecialTime(strFormat, strData, strSpecialTime))
	{
		strResult = strSpecialTime;
		return strResult;
	}
	//end

	CString strRemainder = strData.Right(strData.GetLength() - nFcount);
	strResult = strResult + strData.Left(nFcount) + GetCompartType(strFormat, strCompartDate, strCompartTime) ;	

	strFormat = strFormat.Right(strFormat.GetLength() - nFcount);
	
	return GetStringFormatbyCompart(strRemainder, strFormat, strCompartDate, strCompartTime, strResult);
}

CString CDataConversion::GetCompartType(const CString &strFormat, const CString &strCompartDate, const CString &strCompartTime)
{
	CString szcurType = strFormat.Left(1);
	CString sznextType = "";
	if(strFormat.GetLength() > 3)
		sznextType = strFormat.Mid(3, 1);

	if((szcurType == "Y" || szcurType == "M" || szcurType == "D") && !(sznextType == "S"))
		return strCompartDate;
	else
		return strCompartTime;
}

// Draw ---------------------------------------------------------------------------------
// �׸� ���� ���Ѵ�.
void CDataConversion::SelectClipRegion(CDC* pDC, const CRect& rect)
{
	CRgn rgn;
	rgn.CreateRectRgn((int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom+1);
	pDC->SelectClipRgn(&rgn);
}

// �׸��� �����⺻������ ������.
void CDataConversion::SelectClipRegion(CDC* pDC)
{
	pDC->SelectClipRgn(NULL);
}

// data ���� (point ��) -----------------------------------------------------------------
// �����ǥ --> ������ǥ
// ���� ���� ȭ���� point�� ��ȯ�� ���� �����´�.
CPoint CDataConversion::GetVerticalPoint(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData, const bool bCenter)
{
	if( max == -HUGE_VAL || min == -HUGE_VAL) return blockRegion.TopLeft();

	int x = GetChangingPosition(nIndex, blockRegion.left, realWidth, bCenter);
	if(x < 0)
		x = blockRegion.left;

	int y = GetChangingPosition(blockRegion.bottom, blockRegion.Height(), min, max-min, nowData);
	//if(y < 0)
	//	y = blockRegion.top;
	
	return CPoint(x, y);
}

CPoint CDataConversion::GetVerticalPoint(const CRect& blockRegion, const double& yMin, const double& yMax, const double& xMin, const double& xMax, const double& nowYData, const double& nowXData)
{
	int x = GetChangingPosition(blockRegion.right, blockRegion.Width(), xMin, xMax-xMin, nowXData);
	if(x < 0)
		x = blockRegion.left;
	else
		x = blockRegion.right - x + blockRegion.left; 

	int y = GetChangingPosition(blockRegion.bottom, blockRegion.Height(), yMin, yMax-yMin, nowYData);
	//if(y < 0)
	//	y = blockRegion.top;

	return CPoint(x, y);
}

CPoint CDataConversion::GetVerticalPointBox(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData, const bool bCenter)
{
	int x = GetChangingPosition(nIndex, blockRegion.left, realWidth, bCenter);
	int y = GetChangingPosition(blockRegion.bottom, blockRegion.Height(), min, max-min, nowData);
	//if(y < 0)
	//	y = blockRegion.top;
	
	return CPoint(x, y);
}

// ���� ���� ȭ���� point�� ��ȯ�� ���� �����´�.
CPoint CDataConversion::GetHorizontalPoint(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData)
{
	int x = GetChangingPosition(blockRegion.right, blockRegion.Width(), min, max-min, nowData);
	if(x < 0){
		if(max < 0 && min < 0)
			x = blockRegion.right;
		else
			x = blockRegion.left;
	}
	else
		x = blockRegion.right - x + blockRegion.left; 

	int y = GetChangingPosition(nIndex, blockRegion.top, realWidth, true);
	if(y < 0)
		y = blockRegion.top;

	return CPoint(x, y);
}

int CDataConversion::GetHorizontalPoint_X(const CRect& blockRegion, const double& min, const double& max, const double& nowData)
{
	int x = GetChangingPosition(blockRegion.right, blockRegion.Width(), min, max-min, nowData);
	if(x < 0){
		if(max < 0 && min < 0)
			x = blockRegion.right;
		else
			x = blockRegion.left;
	}
	else
		x = blockRegion.right - x + blockRegion.left; 

	return x;
}

// ������ǥ --> �����ǥ
int CDataConversion::AbsoluteToRelative_X(const CRect& region, const int absolute_x, const int pageDataCount)
{
	if(pageDataCount <= 0 || absolute_x < region.left ||  absolute_x > region.right)
		return 0;

	return int((double)((absolute_x - region.left)*pageDataCount) / (double)region.Width());
}

int CDataConversion::AbsoluteToRelative_X(const CRect& region, const int absolute_x, const double& dataViewMin, const double& dataViewMax)
{
	if(absolute_x < region.left ||  absolute_x > region.right)
		return 0;
    
	double dData = ((double)region.right - absolute_x) * (dataViewMax - dataViewMin);
	dData = dData / (double)region.Width();
	return CMath::Round(dataViewMax - dData);	
}

double CDataConversion::AbsoluteToRelative_Y(const CRect& region, const int absolute_y, const double& dataViewMin, const double& dataViewMax)
{
	double unitHeight = (dataViewMax - dataViewMin) / (double)region.Height();
	if(absolute_y < region.top)
		return dataViewMax + ((double)(region.top - absolute_y) * unitHeight);
	else if(absolute_y > region.bottom)
		return dataViewMin - ((double)(absolute_y - region.bottom) * unitHeight);

	double dData = (double)(region.bottom - absolute_y) * (dataViewMax - dataViewMin) / (double)region.Height();
	return (dataViewMin + dData);
}

// �����ǥ --> ������ǥ
int CDataConversion::RelativeToAbsolute_Y(const CRect& region, const double& relative_y, const double& dataViewMin, const double& dataViewMax)
{
	if( dataViewMax == dataViewMin) return region.bottom;
	double dData = (relative_y - dataViewMin) * (double)region.Height() / (dataViewMax - dataViewMin);
	return (region.bottom - CMath::Round(dData));
}

// ----------------------------------------------------------------------------
CPoint CDataConversion::RelativeToAbsolute(const CRect& region, const int startIndex, const int endIndex, 
			const double& dataViewMin, const double& dataViewMax, 
			const int currentIndex, const double& currentPrice)
{
	double realWidth = GetRealWidth((double)region.Width(), (endIndex - startIndex +1));

	// �ٲ� �����ǥ�� �׸��� start/end �� ���� ���
	if(currentIndex >= startIndex  &&  currentIndex <= endIndex)
		return RelativeToAbsoluteInDisplayIndex(region, realWidth, dataViewMin, dataViewMax, (currentIndex - startIndex), currentPrice);

	// �ٲ� �����ǥ�� �׸��� ����(start/end)�� ��� ���
	double realHeight = (double)region.Height()/(dataViewMax - dataViewMin);
	return RelativeToAbsoluteOutDisplayIndex(region, realWidth, realHeight, 
		dataViewMin, dataViewMax, startIndex, endIndex, currentIndex, currentPrice);
}

// x ��ǥ
int CDataConversion::RelativeToAbsolute_X(const CRect& region, const int startIndex, const int endIndex, const int currentIndex, const bool bCenter)
{
	double realWidth = GetRealWidth((double)region.Width(), (endIndex - startIndex +1));

	// �ٲ� �����ǥ�� �׸��� start/end �� ���� ���
	if(currentIndex >= startIndex && currentIndex <= endIndex)
		return RelativeToAbsoluteInDisplayIndex_X(region, realWidth, (currentIndex - startIndex), bCenter);

	// �ٲ� �����ǥ�� �׸��� ����(start/end)�� ��� ���
	return RelativeToAbsoluteOutDisplayIndex_X(region, realWidth, startIndex, endIndex, currentIndex);
}

// --------------------------------------------------------------------------------------
// �ش� point�� �ִ� data�� index --> �ڷ������� ���
int CDataConversion::GetPacketDataIndex_Date(const CRect& region, const CPoint& point, const int dataStart, const int dataEnd)
{
	// (2004.10.11, ��¿�) Region Width�� 0�� ��츦 Devide Zero�� ����ó���Ѵ�.
	if(dataEnd < dataStart || dataStart < 0 || region.Width() == 0)
		return -1;

	int index = int((double)((point.x - region.left)*(dataEnd - dataStart +1)) / (double)region.Width());
	return dataStart + index;
}

// ��¥ �ϳ��� x����
double CDataConversion::GetRealWidth(const double& dWidth, const int nIndex)
{
	if(nIndex <= 0)
		return 0.0;

	return (dWidth / (double)nIndex);
}

// --------------------------------------------------------------------------------------
//invert graph�� ���ؼ� dataPt�� invert �Ѵ�.
int CDataConversion::InvertGraphDataPt(const CRect& region, const int dataPt_y)
{
	int halfHeight  = region.Height() / 2;

	if(dataPt_y > region.top + halfHeight)
		return region.top + (region.bottom - dataPt_y);
	else if(dataPt_y < region.top + halfHeight)
		return region.bottom - (dataPt_y - region.top);
	else 
		return dataPt_y;

	return dataPt_y;
}

// private ==============================================================================
// pluse ������ üũ�Ѵ�.
bool CDataConversion::IsPluse(const CString& data)
{
	return (data.Left(1) != '-');
}

// --------------------------------------------------------------------------------------
// GetVerticalPoint(), GetHonrizontalPoint() �� �����Լ���
int CDataConversion::GetChangingPosition(const int startPos, const int length, const double& originStartData, const double& originLength, const double& nowData)
{
	if(length <= 0 || originLength == 0)
		return -1;

	int ChangingLength = CMath::Round(length * ((nowData - originStartData) / originLength));
	return (startPos - ChangingLength);
}

int CDataConversion::GetChangingPosition(const int nowIndex, const int startPos, const double& width, const bool bCenter)
{
	if(nowIndex < 0)
		return -1;

	int nNowPos = CMath::Round(width * nowIndex);
	if(bCenter){
		int nNextPos = CMath::Round(width * (nowIndex +1));
		// line �� ���������� �׸��� ������ "����". gap�� 3, 5 ���� ��� ���.
		int nHalf = (nNextPos - nNowPos) / 2;
		nNowPos += nHalf;
	}

	return (startPos + nNowPos);
}

// --------------------------------------------------------------------------------------
// �����ǥ -> ������ǥ : �����ǥ�� �ش� ������ ���� ���
CPoint CDataConversion::RelativeToAbsoluteInDisplayIndex(const CRect& region, const double& realWidth, 
		const double& dataViewMin, const double& dataViewMax, const int currentIndex, const double& currentPrice)
{
	int	x = RelativeToAbsoluteInDisplayIndex_X(region, realWidth, currentIndex);
	int y = RelativeToAbsoluteInDisplayIndex_Y(region, dataViewMin, dataViewMax, currentPrice);
	
	return CPoint(x, y);
}

// �����ǥ -> ������ǥ : �����ǥ�� �ش� ������ �ۿ� ���� ���
CPoint CDataConversion::RelativeToAbsoluteOutDisplayIndex(const CRect& region, 
		const double& realWidth, const double& realHeight, const double& dataViewMin, const double& dataViewMax, 
		const int startIndex, const int endIndex, const int currentIndex, const double& currentPrice)
{
	int x = RelativeToAbsoluteOutDisplayIndex_X(region, realWidth, startIndex, endIndex, currentIndex);
	int y = RelativeToAbsoluteOutDisplayIndex_Y(region, realHeight, dataViewMin, dataViewMax, currentPrice);

	return CPoint(x, y);
}

// --------------------------------------------------------------------------------------
// �����ǥ -> ������ǥ : �����ǥ�� �ش� ������ ���� ���
int CDataConversion::RelativeToAbsoluteInDisplayIndex_X(const CRect& region, const double& realWidth, const int currentIndex, const bool bCenter)
{
	int	x = GetChangingPosition(currentIndex, region.left, realWidth, bCenter);
	if(x == -1)
		return region.left;

	return x;
}

// �����ǥ -> ������ǥ : �����ǥ�� �ش� ������ �ۿ� ���� ���
int CDataConversion::RelativeToAbsoluteOutDisplayIndex_X(const CRect& region, const double& realWidth, 
		const int startIndex, const int endIndex, const int currentIndex)
{
	if(currentIndex < startIndex)
		return region.left - CMath::Round(double(startIndex - currentIndex)*realWidth);
	else if(currentIndex > endIndex)
		return region.right + CMath::Round(double(currentIndex - endIndex)*realWidth);

	return 0;
}

// --------------------------------------------------------------------------------------
int CDataConversion::RelativeToAbsoluteInDisplayIndex_Y(const CRect& region, const double& dataViewMin, const double& dataViewMax, const double& currentPrice)
{
	return GetChangingPosition(region.bottom, region.Height(), dataViewMin, dataViewMax - dataViewMin, currentPrice);
	//if(y < 0)
	//	return region.top;
}

int CDataConversion::RelativeToAbsoluteOutDisplayIndex_Y(const CRect& region, 
		const double& realHeight, const double& dataViewMin, const double& dataViewMax, const double& currentPrice)
{
	if(dataViewMax < currentPrice)
		return region.top - CMath::Round((currentPrice - dataViewMax)*realHeight);
	else if(dataViewMax > currentPrice)
		return region.bottom + CMath::Round((dataViewMin - currentPrice)*realHeight);
	
	return GetChangingPosition(region.bottom, region.Height(), dataViewMin, dataViewMax - dataViewMin, currentPrice);
}

// --------------------------------------------------------------------------------------
//sy 2002.7.30. -> �̷� �ð� ���ϱ�
//sy 2004.08.18
/*bool CDataConversion::OnPickoutTime(const int nCharge, const CString& strFormat, const int nInterval, int& mon, int& day, int& hour, int& min, int& sec)
{
	mon = day = hour = min = sec = 0;

	if(nCharge <= 0)
		return false;
	
	// ���� -> ��, ��, �� �� ����.
	if(strFormat.Right(1) == 'D' || strFormat.Find('Y') >= 0)
		return false;

	// �ð�
	if(strFormat.Right(1) == "S") {// ��
		int nMin = 0;
		//�е���Ÿ
		if(nInterval >= 100) {
			nMin = (nInterval / 100);
			sec = 60 * nCharge * nMin;
		}
		else
			sec = nInterval * nCharge;
	}
	else if(strFormat.Right(1) == "M") {// ��
		int nhour = 0;
		//�ð�����Ÿ
		if(nInterval >= 100) {
			nhour = (nInterval / 100);
			min = 60 * nCharge * nhour;
		}
		else
			min = nCharge * nInterval;
	}
	else if(strFormat.Right(1) == "H") // ��
		hour = nCharge * nInterval;
	
	while(sec >= 60){
		min++;
		sec -= 60;
	}
	while(min >= 60){
		hour++;
		min -= 60;
	}
	while(hour >= 7){
		day++;
		hour -= 7;
	}
	while(day >= 30){
		mon++;
		day -= 30;
	}

	return (sec != 0 || min != 0 || hour != 0 || day != 0 || mon != 0);
}*/

bool CDataConversion::OnPickoutTime(const int nCurData, const int nCharge, const CString& strFormat, const int nInterval, int& mon, int& day, int& hour, int& min, int& sec)
{
	mon = day = hour = min = sec = 0;

	if(nCharge <= 0)
		return false;
	
	// ���� -> ��, ��, �� �� ����.
	if(strFormat.Right(1) == 'D' || strFormat.Find('Y') >= 0)
		return false;

	// �ð�
	if(strFormat.Right(1) == "S") {// ��
		int nMin = 0;
		//�е���Ÿ
		if(nInterval >= 100) {
			nMin = (nInterval / 100);
			sec = 60 * nCharge * nMin;
		}
		else
			sec = nInterval * nCharge;
	}
	else if(strFormat.Right(1) == "M") {// ��
		int nhour = 0;
		//�ð�����Ÿ
		if(nInterval >= 100) {
			nhour = (nInterval / 100);
			min = 60 * nCharge * nhour;
		}
		else
			min = nCharge * nInterval;
	}
	else if(strFormat.Right(1) == "H") // ��
		hour = nCharge * nInterval;
	
	while(sec >= 60){
		min++;
		sec -= 60;
	}
	while(min >= 60){
		hour++;
		min -= 60;
	}
	while(hour >= 7){
		day++;
		hour -= 7;
	}
	while(day >= 30){
		mon++;
		day -= 30;
	}

	return (sec != 0 || min != 0 || hour != 0 || day != 0 || mon != 0);
}

bool CDataConversion::OnPickoutDate(const int nValue, const CString& strFormat, const int nType, int& year, int& mon, int& day)
{
	switch(nType){
	case 1:	//�ϰ�
		return OnPickoutDaily(nValue, strFormat, year, mon, day);
	case 2:	//�ְ�
		return OnPickoutWeekley(nValue, strFormat, year, mon, day);
	case 3:	//����
		return OnPickoutMonthly(nValue, strFormat, year, mon, day);
	}

	return false;
}

bool CDataConversion::OnPickoutDaily(const int nValue, const CString& strFormat, int& year, int& mon, int& day)
{
	CString strValue = IntToString(nValue, strFormat.GetLength());
	if(strFormat.Right(1) == 'D')
		day = nValue % 30;
	else
		day = 0;

	if(strFormat.Find('M') >= 0)
		mon = nValue / 30;
	else
		mon = 0;

	if(strFormat.Find('Y') >= 0){
		year = mon / 12;
		mon = mon % 12;
	}
	else
		year = 0;

	return (day != 0 || mon != 0 || year != 0);
}

bool CDataConversion::OnPickoutWeekley(const int nValue, const CString& strFormat, int& year, int& mon, int& day)
{
	if(strFormat.Right(1) == 'D')
		day = (nValue * 7) % 30;
	else
		day = 0;

	if(strFormat.Find('M') >= 0)
		mon = (nValue * 7) / 30;
	else
		mon = 0;

	if(strFormat.Find('Y') >= 0){
		year = mon / 12;
		mon = mon % 12;
	}
	else
		year = 0;

	return (day != 0 || mon != 0 || year != 0);
}

bool CDataConversion::OnPickoutMonthly(const int nValue, const CString& strFormat, int& year, int& mon, int& day)
{
	day = 0;

	if(strFormat.Find('M') >= 0)
		mon = nValue % 12;
	else
		mon = 0;

	if(strFormat.Find('Y') >= 0)
		year = nValue / 12;
	else
		year = 0;

	return (mon != 0 || year != 0);
}
//end sy

//sy 2004.04.16.
bool CDataConversion::GetSpecialTime(const CString& strFormat, const CString& strData, CString& strSpecialTime)
{
	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	if( CChartInfo::WORLD_ON == m_eChartMode) return false;

	int nCodeIndex = strFormat.Find( "HHMM");
	if(strFormat.GetLength() < 4 || nCodeIndex == -1 || strData.GetLength() < nCodeIndex + 4) return false;

	CString strCode = strData.Mid( nCodeIndex, 4);
	
	if( strCode == "6666")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_OPENING_TRADE);
		return true;
	}
	
	if( strCode == "7777")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_CLOSING_TRADE);
		return true;
	}

	// (2006/11/19 - Seung-Won, Bae) Support samsung code. �帶�� 009991, �ð��ܸ��� 009999
	if( strCode == "9999")
	{
		strSpecialTime = _LTEXT6( C6_OVERTIME_TRADE);
		return true;
	}

	// (2006/12/29 - Seung-Won, Bae) Support koscom code. �帶�� 310000
	// (2008/11/19 - Seung-Won, Bae) Do not support "3100" and "4100" for Woori.
	if( strCode == "8888" || strCode == "0099")	//  || strCode == "3100")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_TRADING);
		return true;
	}

	// �ð��ܸ��� �߰� - ojtaso (20071130)
	if( strCode == "4100")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_OVERTIME_TRADE);
		return true;
	}

	// (2009/3/19 - Seung-Won, Bae) for After Closing.
	if( strCode == "7100")
	{
		strSpecialTime = _LTEXT6( C6_AFTER_CLOSING);
		return true;
	}
	if( strCode == "8100")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_SINGLE_PRICE);
		return true;
	}

	return false;
}
//sy end

// (2004.10.22, ��¿�) �־��� ������ �ݴ� ���(��� 50%��)�� ������ ���Ѵ�.
//	[�⺻���� ����]
//		1. ���� �����󿡼� ä����ȭ�� �ٽ� ����ȭ�� ���� ��Ÿ����.
//			����, ������ �־��� ���� ����ȭ������ ������ ���ϰ�,
//			�ٽ� ä����ȭ ������ ������ ���Ͽ� �������� ���Ѵ�.
//		2. HSI ǥ��
//			������ �� ���� ��Ŀ� ���� Factor ���̴�.
//			Hue(����), Saturation(ä��), Intensity(���)
//			�� ���� RGB ���� �����ϱ� ���� ����ġ��� Maximum ���� ���������ϴ� ������ �����Ͽ��� �Ѵ�.
//			���⼭�� Windows�� ���� Dialog�� �����Ͽ�, ���ظ� �����ϱ� ���Ͽ� 240�� �ִ�� �Ͽ� �����Ѵ�.
//			(Windows ���� Dialog�� ���, ������ RGB���� �����ϱ� ���Ͽ� 240�� �����Ͽ���.
//				�װ��� RGB ���� �ִ� 255�� ������ ���е��̸鼭
//				����� ä�� ������ 6���Ұ� 2������ ������ 6�� ����� ������ ���̴�.)
//		3. ����
//			H ��(Hue, ����)�� ���� �����Ǹ�, �װ��� ������ ���� �������� �� �ִ�. (6����)
//				0		R(255,0,0)
//				20			(255,127,0)		R Max, G �� ��ȭ����, B Min
//				40		RG(255,255,0)
//				60			(127,255,0)		R �� ��ȭ����, G Max, B Min
//				80		G(0,255,0)
//				100			(0,255,127)		R Min, G Max, B �� ��ȭ����
//				120		GB(0,255,255)
//				140			(0,127,255)		R Min, G �� ��ȭ����. B Max
//				160		B(0,0,255)
//				180			(127,0,255)		R �� ��ȭ����, G Min, B Max
//				200		RB(255,0,255)
//				220			(255,0,127)		R Max, G Min, B �� ��ȭ����
//				240		R(255,0,0)
//			������ Ư¡���δ� �׻� RGB�� �ϳ��� 0, �ϳ��� 255�� �����̴�.
//				Max(RGB) = 255, Min(RGB) = 0
//		4. ä����ȭ
//			S ��(Saturation, ä��)�� ���� �����Ǹ�, ä���� ���� ������ ������ ���� �����ȴ�.
//				RGB = ( RGB - 127.5) * H / 240 + 127.5
//			��, �� ������ H ���߸�ŭ 127.5 (RGB ������ �߽ɰ�)���� �����Եȴ�.
//			Ư�� 240 (Max)�̸� ������ �����ϰ�, 0 (Min)�̸� ���� ��ä��(ȸ��)���� �����ǰ� �ȴ�.
//			ä����ȭ�� ������ Ư¡�� RGB ���� 0�̳� 255�� ������ ������ ���� ������,
//				RGB ���� ���� ū ���� ���� ���� ����� 127.5 (��ä��)�� �ȴ�.
//				�װ��� ������ Ư¡�� ���� ū ���� 255, ���� ���� 0�̸�,
//				���� ����(ä��)���� 127.5�� ������ �Ǳ� ������ �� ���� ��հ��� 127.5�� �����ȴ�.
//				�� ���� ä���߽��̶�� ����. (��ȭ���� ���� �ο�)
//					( Max(RGB) + Min(RGB)) / 2 = 127.5
//		5. ��Ϻ�ȭ
//			I �� (Instensity, ���)�� ���� �����Ǹ�, 120�� �������� 240(+120) ��� (���)
//				0(-120) ��Ӱ�(������)���� ���ߺ�ȭ�� �ִ� ���̴�.
//			����, ��� ��ȭ��ų ��� (���, I�� 120�ʰ� ������)
//				RGB = 255 - ( 255 - RGB) * ( 240 - I) / 120
//			��Ӱ� ��ȭ��ų ��� (���, I�� 120�̸� ������)
//				RGB = RGB * I / 120
//			�̷��� ��ȯ�Ǿ� ��Ÿ���� ���� Ư���� ���� ���� ��� ������ RGB ���� �����ϰ� �ȴ�.
//		6. ����ã��
//			������ ���� ��Ͽ� ��ȭ�� �ְų�, ä��, �Ǵ� ����ȭ(�ϸ� RGB Rotate)�� �ִ� ó���� ���ؼ���
//				������ ���� ������ �ٽ� �����Ͽ� ����ϰ� �ȴ�.
//			���� ������ �����ϴ� ���� �������� �ݴ� �������, ���� ��Ϻ�ȭ����, ä����ȭ������ ���� �̷�����.
//		7. ��Ϻ�ȭ����
//			��Ϻ�ȭ������ �⺻������ ������ ä���߽��� 127.5�� ���;� �Ѵٴ� ���̴�.
//			���� ��Ϻ�ȭ�� ���ؼ��� ä���߽��� ��ȭ�� ���� RGB���� ���� ū ���� ���� ���� �����ġ��
//				��ȭ�ȴٴ� ���̴�. �̸� ���Ͽ� ��� ��ȭ��(�� I)�� ������ �� �ִ�.
//			���� ��� ��ȭ ������ I�� ������ ���� ���Ͽ�����.
//				I = ( Max(RGB) + Min(RGB)) / 2 * 240 / 255
//			�̷��� ������ I���� 120���� ũ�� ��� ��ȯ�� ���̰�, ������ ��Ұ� ��ȯ�� ���̴�.
//				������ ��� ��ȯ ������ Ʋ���� ������, ���� ��ȭ ������ �����Ͽ� ó���Ѵ�.
//			I = 120 �� ���
//				RGB = RGB
//			I < 120 �� ���
//				RGB = RGB / I * 120
//			I > 120 �� ��� 
//				RGB = 255 - ( 255 - RGB) * 120 / ( 240 - I)
//		8. ä����ȭ����
//			ä�� ��ȭ�� ������ �⺻ ������ ��ȭ ������ Max(RGB)�� 255�� Min(RGB)�� 0�� ���;� �Ѵٴ� ���̴�.
//			����, ��Ϻ����� ������ Max(RGB)���� 255�� �ǰ� ���ִ� H�� ���ϸ�,
//				Min(RGB)���� 0���� �ǰ� ���ִ� H�� �Ǿ�, ä�� ��ȭ �������� Ȯ���ϰ� �ȴ�.
//			��, ä����ȭ ������ H�� ������ ���� ���Ͽ�����.
//				H = ( Max(RGB) - 127.5) * 240 / ( 255 - 127.5)
//			�̷��� ������ H���� �̿��Ͽ� ä����ȭ������ ������ ���Ѵ�.
//				RGB = ( RGB - 127.5) * 240 / H + 127.5
//		9. ����
//			������ ������ ������ ��Ÿ���� Hue ���� �ݴ� ����(120 ��)�� ���Ѵ�.
//				���� ������ ������ Hue�� 120��, �� (H+120)%240�� Hue�� ���Ͽ� �����Ѵ�.
//				�׷���, Hue�� 120���� ������ ������ �迭 �����, RGB ������ ��� (255-R,255-G,255-B)�� �� ������ �ش��Ѵ�.
//			�� ���, ä���� 0�� ��ä���� ��� �� ���� ���� ��ä������, ���� ������ ���ٸ� ���� ���� �ȴ�.
//				��, (255-R,255-G,255-B)�� ������ ���� ���߽� 120�� �߽����� ��Ī�� ���� ���Ե�����,
//				�� ������ ���� 120�̰�, ��ä���� ��� ������ ���� ��� ���� ȸ��(127,127,127)�� ���Եȴ�.
//		10. ������
//			���� ���谡 ���� ���� ��� ���谡 ������ ��츦 ������ ���� �����Ͽ�,
//				������ �ƴ� ��쿡�� ���� ������ �ξ� ���� ���� ��� ������ ���� ������ ��������.
//			�����̸鼭 ���� 120���� ���� �����ϸ�Ǵµ�,
//				�̸� ���ؼ��� �־��� ������ ���� �� ������ ���ѵ�,
//				���� �� ������ ������ ���ϰ�, �ٽ� �� ���� �ݴ� �� ������ ���ϰ� �ȴ�.
COLORREF CDataConversion::GetComplementColor( COLORREF p_crOrg)
{
	// 1. �⺻ ������ �и��Ѵ�.
	BYTE btR = GetRValue( p_crOrg);
	BYTE btG = GetGValue( p_crOrg);
	BYTE btB = GetBValue( p_crOrg);
	// 2. ��� �������� �����Ѵ�.
	// 2.1 RGB�� Min/Max���� ���Ѵ�.
	BYTE btMin = ( btR < btG) ? ( btB < btR ? btB : btR) : ( btB < btG ? btB : btG);
	BYTE btMax = ( btR < btG) ? ( btB < btG ? btG : btB) : ( btB < btR ? btR : btB);
	// 2.2 ��� ������ I�� �����Ѵ�.
	double dI = ( btMax + btMin) / 2. * 240. / 255.;
	// 3. ��� �������� ã�´�.
	if( dI < 120)
	{
		btR = BYTE( btR / dI * 120.);
		btG = BYTE( btG / dI * 120.);
		btB = BYTE( btB / dI * 120.);
	}
	else
	{
		btR = BYTE( 255. - ( 255. - btR) * 120. / (240. - dI));
		btG = BYTE( 255. - ( 255. - btG) * 120. / (240. - dI));
		btB = BYTE( 255. - ( 255. - btB) * 120. / (240. - dI));
	}
	// 4. ������ ���Ѵ�.
	btR = BYTE( 255 - btR);
	btR = BYTE( 255 - btG);
	btR = BYTE( 255 - btB);
	// 5. �ݴ� ��� �������� ���Ѵ�. (��� 50%��)
	if( dI <= 120) dI += 120;
	else dI -= 120;
	// 6. ��Ϻ����� ���� �ݴ��ϼ������� �ٽ� ��������Ѵ�.
	if( dI < 120)
	{
		btR = BYTE( btR * dI / 120.);
		btG = BYTE( btG * dI / 120.);
		btB = BYTE( btB * dI / 120.);
	}
	else
	{
		btR = BYTE( 255. - ( 255. - btR) * (240. - dI) / 120.);
		btG = BYTE( 255. - ( 255. - btG) * (240. - dI) / 120.);
		btB = BYTE( 255. - ( 255. - btB) * (240. - dI) / 120.);
	}
	// 7. ������ ������ ��ȯ�Ѵ�.
	return RGB( btR, btG, btB);
}


// 20080910 JS.Kim		
//	�̱�ää�� ǥ�ø� ���� 8����/32���������� ��ȯ <<

// Type �� ����ó������ ���θ� ���Ѵ�
bool CDataConversion::IsBaseFormat(const CString& dataType)
{
	if( dataType.Find("1/") >= 0 )
		return true;
	return false;
}

// ���� ��ȯ ������ ��´�
// return : ������ȯ�̸� true, �ƴϸ� false
// nBase : ����
// nDecLen1 : �Ҽ��� �� �ڸ���
// nDecLen2 : ������ �� �ڸ���
bool CDataConversion::GetBaseFormatInfo(const CString& dataType, int &nBase, int &nDecLen1, int &nDecLen2)
{
	if( !IsBaseFormat( dataType ) )
		return false;

	CString format(dataType);
	char *szBuf = format.GetBuffer(dataType.GetLength()+1);	
	char* p;

	szBuf = strstr(szBuf, "1/");
	// ���� Base ���ϱ�
	p = strchr(szBuf, '\'');
	if( p )		*p = 0x0;
	nBase = atoi(szBuf+2);
	szBuf = p + 1;

	p = strchr(szBuf, '.');
	if( p )		*p = 0x0;
	nDecLen1 = atoi(szBuf);
	szBuf = p + 1;

	nDecLen2 = atoi(szBuf);

	nDecLen1 -= nDecLen2;

	format.ReleaseBuffer();

	return true;
}
// ������ ���ڸ�(decimalDownCount - �Ҽ��κ�)������ �ٲ۴�.
// ex) Num = 1234.5678   decimalDownCount = 2 -> return : "1234.57" (�ݿø�)
//                       decimalDownCount = 6 -> return : "1234.567800"
//                       decimalDownCount = -2 -> return : "12" (�ݿø�)
CString CDataConversion::DoubleToString(const double& Num, const CString& dataType)
{
	int nBase, nDecLen1, nDecLen2;
	if( GetBaseFormatInfo(dataType, nBase, nDecLen1, nDecLen2) )
	{
		return Gl_Format32String(Num, nBase, nDecLen1+nDecLen2, nDecLen2);
	}
	int nDecimal = GetDecimalDownCount(dataType, true);
	return CDataConversion::DoubleToString(Num, nDecimal);
}

//	�츮�������� ����ϴ� �Ҹ���ġ ���� ������ 
//*******************************************************************/
/*! Function Name : Format32String
/*! Function      : ���������� �̿��Ͽ� ������ ������ Return���ش�. �� �������� ����ŷ�� 
/*!					������������ ũ�Ⱑ Long���� �ѱ⶧���� ����ŷ���� ó���ϴ� ������ ���� �߰�����
/*! Param         : dllexport) CString Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask
/*! Return        : 
/*! Create        : ������ , 2006/11/16
/*!	Revision      : �̻�� , 2010/03/17		�ڵ� ����.
/*! Comment       : 
//******************************************************************/
CString CDataConversion::Gl_Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask)
{
	int nMask = uMask;
	if (nMask == 0)
		nMask = 1;

	double dTBValue = (double)nTBValue/(double)nMask;

	return Gl_Format32String(dTBValue, scale, outdec, indec);
}

//*******************************************************************/
/*!	Revision      : �̻�� , 2010/03/17		�ڵ� ����.				*/
//*******************************************************************/
CString CDataConversion::Gl_Format32String(double dTBValue, int scale, int outdec, int indec)
{
	CString strValue = "", strResult = "";

	if (scale < 0 || outdec < 0 || indec < 0)
	{
		strValue.Format("%d", (long)dTBValue);
		return strValue;
	}

	BOOL bFlag = FALSE;
	if (dTBValue < 0)
	{
		dTBValue *= -1.0;
		bFlag = TRUE;
	}

	if (scale > 1)
	{
		int nInt = (int)floor(dTBValue);
		double dValue = dTBValue - (double)(nInt);
		dValue = dValue * scale;
		if( indec > 0 )
			strValue.Format("%d'%0*.*f", nInt, outdec+1, indec, dValue);
		else
			strValue.Format("%d'%0*.0f", nInt, outdec, dValue);
	}
	else
		return "0";

	if (bFlag)
		strResult.Format("-%s", strValue);
	else
		strResult.Format("%s", strValue);

	return strResult;
}

//20081010 �̹��� 8, 32����ǥ�� => 10�������� ��ȯ>>
BOOL CDataConversion::GetDataToDec(CString strInput, CString strPacketType, CString & strOutput )
{
	strInput.Remove(',');	
	strInput.TrimLeft(); 
	strInput.TrimRight();
	strInput.TrimLeft('+');

	CString strTemp, strInt;

	//���� Ÿ���� �����Է��̾�� �ϴ��� ���� �Ǵ�
	if (!IsBaseFormat(strPacketType)) 
	{
		strOutput = strInput;//10�������� �Ǵ��ϰ� �Է°� �״�� ���
		return TRUE;
	}

	int nBase, nDecLen1, nDecLen2;
	GetBaseFormatInfo(strPacketType, nBase, nDecLen1, nDecLen2);	
	if (nBase <= 0 || nDecLen1 < 0 || nDecLen2 < 0) 
		return FALSE;

	int nPos2, nPos = strInput.Find('\'');
	if (nPos < 0)
	{
		if ((nPos = strInput.Find('.')) >= 0)	//' ���� .�� �ִ� ���
			return FALSE;
		else									//' ���� .�� ���� ���
		{
			strTemp.Format("%d", atoi(strInput));

			if (strTemp == strInput)			// ���ڰ� �ƴ� �ٸ� ���� ����ִ��� ���� �Ǵ�.
			{
				strOutput = strInput;
				return TRUE;
			}
			else
				return FALSE;
		}
	}

	///////////////////////////////////
	//' �� . �����Է� ã�� EX) 20.10'10 => ����
	nPos2 = strInput.Find('.');
	if (nPos2 >= 0 && nPos > nPos2)
		return FALSE;

	///////////////////////////////////
	//������, �߰���, �Ҽ��η� �и�
	strInt = strInput.Left(nPos);
	strInput = strInput.Mid(nPos + 1);

	double dInt = atof(strInt);
	double dFloat = atof(strInput)/(double)nBase;

	BOOL bSign = FALSE;
	if (dInt < 0)
		bSign = TRUE;

	dInt = fabs(dInt);
	long double dOutput =  (bSign) ? - (dInt + dFloat) : dInt + dFloat;

	strOutput.Format("%f", dOutput);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//8����,32���� �Է°��� ��ȿ���� üũ�� �ϴ� �Լ�
//����:
//8����,32����=>10�������� ��ȯ�� ���� GetDataToDec�Լ��� ����� ��.
//(GetDataToDec �Լ� ���ο��� IsValid8_32�Լ��� �ϴ� ���� ��� �ϰ� �����Ƿ� IsValid8_32�� ȣ���Ͽ� �˻縦 ���� ���� �ʿ����)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDataConversion::IsValid8_32(CString strInput)
{
	strInput.Remove(',');	
	strInput.TrimLeft(); 
	strInput.TrimRight();
	strInput.TrimLeft('+');
	
	CString strTemp;
	
	int nPos1, nPos2;
	CString strInt, strMid, strPoint;	
	
	//////////////////////////////////////
	//'�� ���� ��� ó��..
	int nPos = strInput.Find('\'');
	if(nPos < 0) 
	{
		//' ���� .�� �ִ� ��� 
		if((nPos = strInput.Find('.')) >= 0)
			return FALSE;
		else
		{	//'���� .�� ���� ��� 
			strTemp.Format("%d", atoi(strInput));
			
			if(strTemp == strInput)		//���ڰ� �ƴ� �ٸ� ���� ����ִ��� ���� �Ǵ�.
				return TRUE;
			else
				return FALSE;
		}
	}	
	
	///////////////////////////////////
	//' �� . �����Է� ã�� EX) 20.10'10 => ����
	nPos1		=   nPos;
	nPos2		=	strInput.Find('.');
	if(nPos2 >= 0 && nPos1 > nPos2)
		return FALSE;
	
	
	///////////////////////////////////
	//������, �߰���, �Ҽ��η� �и��Ͽ� ����üũ.
	strInt		=	strInput.Left(nPos);
	strInput	=	strInput.Mid(nPos + 1);
	if((nPos = strInput.Find('.')) < 0) 
	{
		strMid	= strInput;
		strPoint= "";
	}
	else
	{		
		strMid	= strInput.Left(nPos);
		strPoint= strInput.Mid(nPos + 1);
		
		strMid.TrimLeft(); strMid.TrimRight();
		if(strMid.GetLength() <= 0) // EX) 20'.10 => ����
			return FALSE;
	}
	
	/////////////////////////////////////////////////
	//�� �κк��� ���ڰ� �ƴ� �ٸ� ���� ����ִ��� ���� �Ǵ�.
	BOOL bSign = FALSE;
	if(strInt.GetLength() > 0 && strInt[0] == '-') bSign = TRUE;
	
	if(strInt.GetLength() > 0)
	{
		CString strInteger = strInt;
		
		if(bSign && strInteger.GetLength() == 1) //�����ο� "-" �� �Էµ� ���.
			strInteger = "0";//-��ȣ�� ������ �� ó��.
		else
		{
			if(bSign) //-��ȣ�� ������ �� ó��.
				strInteger = strInteger.Mid(1);

			//-��ȣ�� �����ϰ� �� �տ� �ִ� 0������ ����.
			strInteger.TrimLeft('0');
			if(strInteger.GetLength() <= 0) //�ּ��� 0�� �ϳ��� ���ܵξ�� �Ѵ�.
				strInteger = "0";
		}

		strTemp.Format("%d", atoi(strInteger));
		if(strTemp != strInteger)
			return FALSE;		
	}

	if(strMid.GetLength() > 0)
	{	
		strTemp.Format("%0*d", strMid.GetLength(), atoi(strMid));
		if(strTemp != strMid) return FALSE;
	}

	if(strPoint.GetLength() > 0)
	{	
		strTemp.Format("%0*d", strPoint.GetLength(), atoi(strPoint));
		if(strTemp != strPoint) return FALSE;
	}
	/////////////////////////////////////////////////
	
	return TRUE;
}
//20081010 �̹��� <<




// 20081007 JS.Kim	����ڽð��� ����
double CDataConversion::CalcTimeDiff(double dData, int nTimeDiff, int nCurrTimeDiff, CString strType)
{
	if( nTimeDiff == 0 )		return dData;

	int nTime = 0, nPosition;
	if( (nPosition = strType.Find("HH")) < 0 )	// Packet�� �ð� ����Ÿ�� �����Ƿ� ���� ����� �ʿ� ����.
		return dData;

	int nBaseTimeDiff = nTimeDiff;

	time_t timeResult;
	tm tmResult;
	/*
	time_t timeCur;
	tm* ptmCur;
	time(&timeCur);
	ptmCur = localtime(&timeCur);
	*/
	//Local PC�� ����ð��� ����Ÿ�� ���� �ð����� �ð�(�ŷ��ҽð�)���� �����Ѵ�<<
	tm* ptmCur = CalcCurrentDateTimToPacketTime(nCurrTimeDiff);

	double newTime = dData;
	__int64 lTime = (__int64)dData;

	if(strType == "MMDDHHMM")
	{
		if(lTime)
		{
			int nMon = (int)((lTime / 1000000)- 1);
			if(nMon > ptmCur->tm_mon)	ptmCur->tm_year--;
			else if( ptmCur->tm_mon >= 12 && nMon == 1 )	ptmCur->tm_mon++;

			ptmCur->tm_mon = nMon;
			ptmCur->tm_mday = (int)((lTime % 1000000) / 10000);
			ptmCur->tm_hour = (int)((lTime % 10000) / 100 + nBaseTimeDiff);
			ptmCur->tm_min	= (int)(lTime % 100);
		}
		ptmCur->tm_sec = 0;

		tmResult = *ptmCur;
		timeResult = mktime(&tmResult);

		newTime = (double)((tmResult.tm_mon+1)*1000000 + tmResult.tm_mday*10000 + tmResult.tm_hour*100 + tmResult.tm_min);
	}
	else if(strType == "HHMMSS")
	{
		if(lTime)
		{
			int nHour = (int)(lTime / 10000);
			if(nHour > ptmCur->tm_hour)	ptmCur->tm_mday--;

			ptmCur->tm_hour = nHour + nBaseTimeDiff;
			ptmCur->tm_min	= (int)(lTime % 10000) / 100;
			ptmCur->tm_sec	= (int)(lTime % 100);
		}

		tmResult = *ptmCur;
		timeResult = mktime(&tmResult);
		newTime = (double)(tmResult.tm_hour*10000 + tmResult.tm_min*100 + tmResult.tm_sec);
	} 
	else if (strType == "DDHHMMSS")
	{
		if(lTime)
		{
			int nDay = (int)(lTime / 1000000);
			if(nDay > ptmCur->tm_mday)						ptmCur->tm_mon--;
			else if( ptmCur->tm_mday >= 28 && nDay == 1 )	ptmCur->tm_mon++;

			ptmCur->tm_mday = nDay;
			ptmCur->tm_hour	= (int)(lTime % 1000000) / 10000 + nBaseTimeDiff;
			ptmCur->tm_min	= (int)(lTime % 10000) / 100;
			ptmCur->tm_sec	= (int)(lTime % 100);
		}

		tmResult = *ptmCur;
		timeResult = mktime(&tmResult);

		newTime = (double)(tmResult.tm_mday*1000000 + tmResult.tm_hour*10000 + tmResult.tm_min*100 + tmResult.tm_sec);
	} 

	return newTime;
}

#define		LOCAL_TIMEDIFF		9		// �ѱ� GMT +9

// Local PC�� ����ð��� ����Ÿ�� ���� �ð����� �ð�(�ŷ��ҽð�)���� �����Ѵ�
// ����� ����Ÿ�� �������� �������� �ʴ� ��� ����� ����Ÿ�� PC�� �ð����� �����ϱ� ���� �ʿ�
tm *CDataConversion::CalcCurrentDateTimToPacketTime(int nTimeDiff)
{
	time_t timeCur;
	tm* ptmCur;
	time(&timeCur);
	ptmCur = localtime(&timeCur);		// Local Time

	ptmCur->tm_hour += (nTimeDiff - LOCAL_TIMEDIFF);
	mktime(ptmCur);
	return ptmCur;
}

// (2008/11/15 - Seung-Won, Bae) Conversion Time and Double
BOOL CDataConversion::ConvertTMtoDouble( tm &p_tmTime, double &p_dTime)
{
	p_dTime = p_tmTime.tm_year + 1900;
	p_dTime = p_dTime * 100 + p_tmTime.tm_mon + 1;
	p_dTime = p_dTime * 100 + p_tmTime.tm_mday;
	p_dTime = p_dTime * 100 + p_tmTime.tm_hour;
	p_dTime = p_dTime * 100 + p_tmTime.tm_min;
	p_dTime = p_dTime * 100 + p_tmTime.tm_sec;
	return TRUE;
}
BOOL CDataConversion::ConvertDoubletoTM( double &p_dTime, tm &p_tmTime)
{
/*
	__int64 lClock = (__int64)p_dTime;

	p_tmTime.tm_year = (lClock / 10000) - 1900;
	p_tmTime.tm_mon  = (lClock % 10000) / 100 - 1;
	p_tmTime.tm_mday = (lClock % 100);
	p_tmTime.tm_hour = (lClock % 100);
	p_tmTime.tm_min = (lClock % 100);
	p_tmTime.tm_sec = (lClock % 100);
*/

	double dClock = ( p_dTime + 0.1) / 100000000000000;
	double dValue = 0.0;
	dClock = modf( dClock * 10000, &dValue);
	p_tmTime.tm_year	= int( dValue - 1900);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_mon	= int( dValue - 1);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_mday	= int( dValue);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_hour	= int( dValue);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_min	= int( dValue);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_sec	= int( dValue);

	return TRUE;
}

__int64 CDataConversion::GetPacketTypedTime( CString strPacketType, double p_dTime)
{
	CString strLeft2 = strPacketType.Left( 2);
	if( strLeft2 != "YY")
	{
		p_dTime -= int( p_dTime / 10000000000.) * 10000000000.;
		if( strLeft2 != "MM")
		{
			p_dTime -= int( p_dTime / 100000000.) * 100000000.;
			if( strLeft2 != "DD")
			{
				p_dTime -= int( p_dTime / 1000000.) * 1000000.;
				if( strLeft2 != "HH")
				{
					ASSERT( FALSE);
				}
			}
		}
	}
	CString strRight2 = strPacketType.Right( 2);
	if( strRight2 != "SS")
	{
		p_dTime /= 100;
		double dFractional = modf( p_dTime, &p_dTime);
		if( 0.009 < dFractional) p_dTime++;
		if( strRight2 != "MM")
		{
			p_dTime /= 100;
			dFractional = modf( p_dTime, &p_dTime);
			if( 0.009 < dFractional) p_dTime++;
			if( strRight2 != "HH")
			{
				p_dTime /= 100;
				dFractional = modf( p_dTime, &p_dTime);
				if( 0.009 < dFractional) p_dTime++;
				if( strRight2 != "DD")
				{
					ASSERT( FALSE);
				}
			}
		}
	}
	
	return __int64(p_dTime);
}