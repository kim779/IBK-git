// ScaleCalculator.cpp: implementation of the CScaleCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleCalculator.h"

#include "ScaleBaseData.h"
#include "Conversion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CScaleCalculator -> scale text ����

// public =================================================================================
// ��¥ / �ð��� ��� ������ ���
CString CScaleCalculator::GetCompartScaleData(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart)
{
	switch(scaleDrawerType){
	case CScaleBaseData::HORZ_DAILY:	//�ϰ�
	case CScaleBaseData::HORZ_WEEKLEY:	//�ְ�
	case CScaleBaseData::HORZ_MONTHLY:	//����
		return GetCompartScaleData_Date(packetData, dataType, scaleCompart.GetDateCompart());
	case CScaleBaseData::HORZ_TIME_ONE:	//�ð�
	case CScaleBaseData::HORZ_TIME_TWO:	//�ð�
	case CScaleBaseData::HORZ_TICK:		//ƽ
	case CScaleBaseData::HORZ_TIME_SECOND:
		return GetCompartScaleData_Time(scaleDrawerType, packetData, dataType, scaleCompart);
	}

	return "";
}

CString CScaleCalculator::GetCompartScaleData_Date(const CString& packetData, const CString& dataType, const CString& compartType)
{
	return GetCompartScaleDate(GetRealPacketData(packetData, dataType.GetLength()), dataType, compartType);
}

CString CScaleCalculator::GetCompartScaleData_Time(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart)
{
	CString realPacketData = GetRealPacketData(packetData, dataType.GetLength());

	// "�帶��" �� "�ð���" �� üũ�Ѵ�.
	if(GetPacketDataInTimeOut(dataType, realPacketData))
		return realPacketData;

	switch(scaleDrawerType){
	case CScaleBaseData::HORZ_TIME_ONE:	//�ð�
	case CScaleBaseData::HORZ_TICK:		//ƽ
	case CScaleBaseData::HORZ_TIME_SECOND:
		return GetCompartScaleTime_One(realPacketData, dataType, scaleCompart);
	case CScaleBaseData::HORZ_TIME_TWO:	//�ð�
		return GetCompartScaleTime_Two(realPacketData, dataType, scaleCompart);
	}

	return "";
}

// ----------------------------------------------------------------------------------------
// ���� string �ϰ�� ����� size ���ϱ�
CSize CScaleCalculator::GetTextSize(CDC* pDC, const CString& data)
{
	CString strData = data;
	CSize textSize(0, 0);
	for(int i = 0; i < CDataConversion::GetStringDataCount(data, "\r\n"); i++){
		CString temp = CDataConversion::GetStringData(strData, "\r\n");
		CSize tempSize = pDC->GetTextExtent(temp);
		if(tempSize.cx > textSize.cx)
			textSize.cx = tempSize.cx;
		if(i == 0)
			textSize.cy += tempSize.cy;
		else
			textSize.cy += tempSize.cy;// + MARGIN;
	}
	if(strData.IsEmpty() == FALSE){
		CSize tempSize = pDC->GetTextExtent(strData);
		if(tempSize.cx > textSize.cx)
			textSize.cx = tempSize.cx;
		textSize.cy += tempSize.cy;
	}

	return textSize; 
}

// ������ ���� size�� �����´�.
CSize CScaleCalculator::GetTextSize(const CScaleBaseData::TEXTCOMPARTTYPE textCompart, const CSize& baseSize, const int xCount, const int yCount)
{
	if(xCount <= 0 || yCount <= 0) // ���� �������� ���� ���
		return CSize(0, 0);

	// text ���� type�� �������� ���
	if(textCompart == CScaleBaseData::VERT_TEXT)
		return CSize(baseSize.cx *yCount, baseSize.cy *xCount);

	return CSize(baseSize.cx *xCount, baseSize.cy *yCount);
}

CSize CScaleCalculator::GetTextSize(CDC* pDC, const CString& data, const int xCount, const int yCount)
{
	CSize textSize = pDC->GetTextExtent(data);
	textSize.cx = textSize.cx * xCount;
	textSize.cy = textSize.cy * yCount;

	return textSize;
}

// private ================================================================================
// dataType�� �°� packetData�� �����Ѵ�.
CString CScaleCalculator::GetRealPacketData(const CString& packetData, const int nLength)
{
	if(packetData.IsEmpty() || packetData.GetLength() == nLength)
		return packetData;

	if(packetData.GetLength() > nLength)
		return packetData.Left(nLength);

	CString temp;
	for(int i = 0; i < (nLength - packetData.GetLength()); i++){
		temp += "0";
	}

	return (temp + packetData);
}

// ----------------------------------------------------------------------------------------
//sy 2004.04.16.
// "��������(6666)", "�帶������(7777)", "�帶��(8888)", "�ð���(9999)" �� üũ�Ѵ�.
bool CScaleCalculator::GetPacketDataInTimeOut(const CString& dataType, CString& realPacketData)
{
	CString strSpecialTime;
	if(CDataConversion::GetSpecialTime(dataType, realPacketData, strSpecialTime))
	{
		realPacketData = strSpecialTime;
		return true;
	}
	return false;

	/*if(dataType.Right(4) != "HHMM")
		return false;

	if(realPacketData.Right(4) == "6666"){ // ��������
		realPacketData = "��������";
		return true;
	}
	if(realPacketData.Right(4) == "7777"){ // �帶������
		realPacketData = "�帶������";
		return true;
	}
	if(realPacketData.Right(4) == "8888"){ // �帶��
		realPacketData = "�帶��";
		return true;
	}
	if(realPacketData.Right(4) == "9999"){ // �ð���
		realPacketData = "�ð���";
		return true;
	}
	return false;*/
}

CString CScaleCalculator::GetCompartScaleTime_One(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart)
{
	CString strData_date, strData_time;
	GetCompartScaleDateTime(packetData, dataType, scaleCompart, strData_date, strData_time);

	if(!strData_date.IsEmpty() && !strData_time.IsEmpty()){
		if(scaleCompart.GetDateCompart().GetLength() > 1)
			return strData_date + strData_time;
		return strData_date + scaleCompart.GetDateCompart() + strData_time;
	}
	if(!strData_date.IsEmpty())
		return strData_date;
	return strData_time;
}

CString CScaleCalculator::GetCompartScaleTime_Two(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart)
{
	CString strData_date, strData_time;
	GetCompartScaleDateTime(packetData, dataType, scaleCompart, strData_date, strData_time);

	if(!strData_date.IsEmpty() && !strData_time.IsEmpty())
		return strData_time + "\r\n" + strData_date;
	if(!strData_date.IsEmpty())
		return strData_date;
	return strData_time;
}

void CScaleCalculator::GetCompartScaleDateTime(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart, CString& strData_date, CString& strData_time)
{
	strData_date = strData_time = "";
	int datePos = dataType.ReverseFind('D');
	if(datePos < 0){
		strData_time = GetCompartScaleTime(packetData, dataType, scaleCompart.GetTimeCompart());
		return;
	}

	strData_date = GetCompartScaleData_Date(packetData.Left(datePos +1), dataType.Left(datePos +1), scaleCompart.GetDateCompart());

	CString packetData_time = packetData.Right(packetData.GetLength() - (datePos +1));
	CString dataType_time = dataType.Right(dataType.GetLength() - (datePos +1));
	strData_time = GetCompartScaleTime(packetData_time, dataType_time, scaleCompart.GetTimeCompart());
}

// ----------------------------------------------------------------------------------------
// <�ڷ�����>Ÿ��(YYYYMMDD ��)�� ���Ͽ� packetData�� compart�� ���� �����Ѵ�.
CString CScaleCalculator::GetCompartScaleDate(const CString& packetData, const CString& dataType, const CString& compartType)
{
	if(compartType.IsEmpty())
		return packetData;

	CString changePacketData = packetData;

	// < . - : / >�� ���
	if(compartType.GetLength() == 1){
		if(dataType.Find('Y') <= 0){	// �⵵�� �տ� �ִ� ��� 
			ChangeData(changePacketData, compartType, dataType.ReverseFind('D'));
			ChangeData(changePacketData, compartType, dataType.ReverseFind('M'));
			ChangeData(changePacketData, compartType, dataType.ReverseFind('Y'));
		}
		else{	// �⵵�� �ڿ� �ִ� ��� - �±�
			ChangeData(changePacketData, compartType, dataType.ReverseFind('Y'));
			ChangeData(changePacketData, compartType, dataType.ReverseFind('M'));
			ChangeData(changePacketData, compartType, dataType.ReverseFind('D'));
		}
	}
	// < ����� >�� ���
	else if(compartType.GetLength() >= 6){
		if(dataType.Find('Y') <= 0)	{	//�⵵�� �տ� �ִ� ��� 
			ChangeData(changePacketData, "��", dataType.ReverseFind('D'));
			ChangeData(changePacketData, "��", dataType.ReverseFind('M'));
			ChangeData(changePacketData, "��", dataType.ReverseFind('Y'));
		}
		else{	// �⵵�� �ڿ� �ִ� ��� - �±�
			ChangeData(changePacketData, "��", dataType.ReverseFind('Y'));
			ChangeData(changePacketData, "��", dataType.ReverseFind('M'));
			ChangeData(changePacketData, "��", dataType.ReverseFind('D'));
		}
	}
	// <����>�� ��� - Jan
	else{
		CString strYear = GetTrimData(packetData, dataType, 'Y');
		CString strMonth = GetMonthEnglish(GetTrimData(packetData, dataType, 'M'));
		CString strDaily = GetTrimData(packetData, dataType, 'D');

		if(!strMonth.IsEmpty() && !strDaily.IsEmpty())
			changePacketData = strMonth + '/' + strDaily;
		else
			changePacketData = strMonth + strDaily;

		if(dataType.Find('Y') <= 0){	//�⵵�� �տ� �ִ� ��� 
			if(!strYear.IsEmpty() && !changePacketData.IsEmpty())
				changePacketData = " " + changePacketData;
			changePacketData = strYear + changePacketData;
		}
		else{
			if(!strYear.IsEmpty() && !changePacketData.IsEmpty())
				changePacketData = changePacketData + " ";
			changePacketData = changePacketData + strYear;
		}
	}

	return changePacketData;
}

// <�ڷ�ð�> packetData�� �� Ÿ��(HHMMSSNN��)�� �߰��Ͽ� ���̸� ���Ѵ�.
CString CScaleCalculator::GetCompartScaleTime(const CString& packetData, const CString& dataType, const CString& compartType)
{
	if(compartType.IsEmpty())
		return packetData;

	CString changePacketData = packetData;

	bool ret1 = false, ret2 = false, ret3 = false;
	// < . - : / >�� ���
	if(compartType.GetLength() == 1){
		ChangeData(changePacketData, compartType, dataType.ReverseFind('S'));
		ChangeData(changePacketData, compartType, dataType.ReverseFind('M'));
		ChangeData(changePacketData, compartType, dataType.ReverseFind('H'));
	}
	// < �ú��� >�� ���
	else{
		ChangeData(changePacketData, "��", dataType.ReverseFind('S'));
		ChangeData(changePacketData, "��", dataType.ReverseFind('M'));
		ChangeData(changePacketData, "��", dataType.ReverseFind('H'));
	}
	
	return changePacketData;
}

// ----------------------------------------------------------------------------------------
// insertData�� baseData�� position��ġ�� �����Ѵ�.
bool CScaleCalculator::ChangeData(CString& baseData, const CString& insertData, const int position)
{
	if(position < 0)
		return false;

	if(position +2 > baseData.GetLength() && insertData.GetLength() <= 1)
		return false;

	baseData.Insert(position +1, insertData);	
	return true;
}

CString CScaleCalculator::GetTrimData(const CString& packetData, const CString& dataType, const TCHAR& strTrim)
{
	int nStartIndex = dataType.Find(strTrim);
	if(nStartIndex < 0)
		return "";

	int nEndIndex = dataType.ReverseFind(strTrim);
	return packetData.Mid(nStartIndex, nEndIndex - nStartIndex +1);
}

CString CScaleCalculator::GetMonthEnglish(const CString& month)
{
	if(month == "01")
		return "January";
	else if(month == "02")
		return "February";
	else if(month == "03")
		return "March";
	else if(month == "04")
		return "April";
	else if(month == "05")
		return "May";
	else if(month == "06")
		return "June";
	else if(month == "07")
		return "July";
	else if(month == "08")
		return "August";
	else if(month == "09")
		return "September";
	else if(month == "10")
		return "October";
	else if(month == "11")
		return "November";
	else if(month == "12")
		return "December";

	return month;
}
