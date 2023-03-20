// IndiConfigurationDefineData.cpp: implementation of the CIndiConfigurationDefineData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndiConfigurationDefineData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char* CIndiConfigurationDefineData::GROUPNAME_TOTAL = "Total";
const char* CIndiConfigurationDefineData::GROUPNAME_SPECIAL = "Ư����ǥ";
const char* CIndiConfigurationDefineData::GROUPNAME_MARKET = "������ǥ";
const char* CIndiConfigurationDefineData::GROUPNAME_INVESTOR = "�����ڵ���";

const char* CIndiConfigurationDefineData::INDINAME_DEPOSIT = "����Ź��";
const char* CIndiConfigurationDefineData::INDINAME_LOAN = "�ſ������ܰ�";
const char* CIndiConfigurationDefineData::INDINAME_UNCOLLECTED = "��Ź�ڹ̼���";
const char* CIndiConfigurationDefineData::INDINAME_FOREIGNER_SHARE = "�ܱ���������";
const char* CIndiConfigurationDefineData::INDINAME_FOREIGNER_BUY = "�ܱ��μ��ż�";
const char* CIndiConfigurationDefineData::INDINAME_FO_UNAPPROVAL = "����/�ɼ� �̰��� ����";
const char* CIndiConfigurationDefineData::INDINAME_FO_PRICE = "�����ŷ�������";


// "�ֽ�, ��"�� �ش�Ǵ� ��ǥ���� ����
bool CIndiConfigurationDefineData::IsIndicatorNameInStocksDaily(const CString& strIndicatorName)
{
	// [����Ź��], [�ſ������ܰ�], [��Ź�ڹ̼���], [�ܱ���������], [�ܱ��μ��ż�]
	return (strIndicatorName == INDINAME_DEPOSIT || strIndicatorName == INDINAME_LOAN || 
		strIndicatorName == INDINAME_UNCOLLECTED || strIndicatorName == INDINAME_FOREIGNER_SHARE || 
		strIndicatorName == INDINAME_FOREIGNER_BUY);
}
