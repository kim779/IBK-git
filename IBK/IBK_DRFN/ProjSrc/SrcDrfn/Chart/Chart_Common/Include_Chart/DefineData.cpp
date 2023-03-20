// DefineData.cpp: implementation of the CDefineData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DefineData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CDefineData

const char* CDefineData::PACKET_TYPE_TEXT = "����";

const char* CDefineData::SCALEDATA_DATE = "�ڷ�����";
const char* CDefineData::SCALEDATA_TEXT = "Text";
const char* CDefineData::SCALEDATA_PRICEZONE = "��簡";
const char* CDefineData::SCALEDATA_INDICATEDATA = "��ǥ Data";
const char* CDefineData::SCALEDATA_VOLUME = "�ŷ���";
const char* CDefineData::SCALEDATA_PERCENT = "�����";

const char* CDefineData::INDICATEGROUPNAME_MARKET = "������ǥ";

const char* CDefineData::INDICATEDATA_PRICECHART = "������Ʈ";
const char* CDefineData::INDICATEDATA_VOLUMECHART = "�ŷ�����Ʈ";
const char* CDefineData::INDICATEDATA_OPTIMIZEAIGNALCHART = "IndicatorOptimize Signal";
const char* CDefineData::INDICATEDATA_VOLUMEPRICE = "�ŷ����";

const char* CDefineData::INDICATEDATA_PRICEMACHART = "���� �̵����";
const char* CDefineData::INDICATEDATA_VOLUMEMACHART = "�ŷ��� �̵����";
const char* CDefineData::INDICATEDATA_DISPARITYCHART = "�̰ݵ�";
const char* CDefineData::INDICATEDATA_GLANCEBALANCECHART = "�ϸ����ǥ";
const char* CDefineData::INDICATEDATA_PSYCHCHART = "�ɸ���";//"���ڽɸ���";
const char* CDefineData::INDICATEDATA_NEWPSYCHCHART = "�Žɸ���";
const char* CDefineData::INDICATEDATA_SONAR_PSYCHCHART = "Sonar+�ɸ���";
const char* CDefineData::INDICATEDATA_BOLLINGERCHART = "Bollinger Bands";
const char* CDefineData::INDICATEDATA_BOXCHARTCHART = "Box Chart";
const char* CDefineData::INDICATEDATA_CLCCHART = "��������Ʈ";
const char* CDefineData::INDICATEDATA_ENVELOPECHART = "Envelope";
const char* CDefineData::INDICATEDATA_PARABOLICCHART = "Parabolic SAR";
const char* CDefineData::INDICATEDATA_PIVOTCHART = "Pivot";
const char* CDefineData::INDICATEDATA_RSICHART = "RSI";
const char* CDefineData::INDICATEDATA_STOCHASTICSCHART = "Stochastic";
const char* CDefineData::INDICATEDATA_STOCHASTICSFASTCHART = "Stochastics Fast";
const char* CDefineData::INDICATEDATA_STOCHASTICSSLOWCHART = "Stochastics Slow";
const char* CDefineData::INDICATEDATA_STOCHASTICSOSCILLATORCHART = "Stochastics Oscillator";
const char* CDefineData::INDICATEDATA_WILLIAMCHART = "%R";//"William's %R";
const char* CDefineData::INDICATEDATA_RAINBOWCHART = "�׹���Ʈ";
const char* CDefineData::INDICATEDATA_ZIGZAGCHART = "Zig Zag";
const char* CDefineData::INDICATEDATA_PRICECHANNELCHART = "Price Channel";
const char* CDefineData::INDICATEDATA_DEMARKCHART = "Demark";
const char* CDefineData::INDICATEDATA_ERRORBANDCHART = "ǥ�ؿ������";
const char* CDefineData::INDICATEDATA_ERRORCHANNELCHART = "ǥ�ؿ���ä��";
const char* CDefineData::INDICATEDATA_DEVIATIONCHANNELCHART = "ǥ������ä��";
const char* CDefineData::INDICATEDATA_DEMACHART = "DEMA";
const char* CDefineData::INDICATEDATA_MACCHART = "MAC";
const char* CDefineData::INDICATEDATA_STARCBANDSCHART = "Starc Bands";
const char* CDefineData::INDICATEDATA_TEMACHART = "TEMA";
const char* CDefineData::INDICATEDATA_LOCKCHART = "��";

const char* CDefineData::INDICATEDATA_THREELINEBRACKCHART = "�Ｑ��ȯ��";
const char* CDefineData::INDICATEDATA_INVERSLINECHART = "���ð�";
const char* CDefineData::INDICATEDATA_PANDFCHART = "P&F";
const char* CDefineData::INDICATEDATA_SWINGCHART = "SwingChart";
const char* CDefineData::INDICATEDATA_KAGICHART = "KagiChart";
const char* CDefineData::INDICATEDATA_RENKOCHART = "RenkoChart"; // YJS(05/06/21) RenKoChart ADD.
const char* CDefineData::INDICATEDATA_FUOPPANDFCHART = "FuOpP&F";
const char* CDefineData::INDICATEDATA_COMPARATIVECHART = "������Ʈ";
const char* CDefineData::INDICATEDATA_MARKETPROFILECHART = "MarketProfile";
const char* CDefineData::INDICATEDATA_MINIJISUCHART = "�̴�������Ʈ";
const char* CDefineData::INDICATEDATA_INDUSTRYJISUCHART = "��������";
const char* CDefineData::INDICATEDATA_MARKETJISUCHART = "��������";
const char* CDefineData::INDICATEDATA_INDIVIDUAL_INVESTOR = "����������";
const char* CDefineData::INDICATEDATA_ORGAN_INVESTOR = "���������";
const char* CDefineData::INDICATEDATA_FOREIGNER_INVESTOR = "�ܱ���������";

const char* CDefineData::INDICATEDATA_PRICE = "����";
const char* CDefineData::INDICATEDATA_VOLUME = "�ŷ���";
const char* CDefineData::INDICATEDATA_PRESPEN = "���ེ��";
const char* CDefineData::INDICATEDATA_VOLUMEFORSALE = "���Ź�";
const char* CDefineData::INDICATEDATA_VOLUMEFORSALEINPRICE = "���Ź�_���ݴ�";

const char* CDefineData::PACKETNAME_OPEN = "�ð�";
const char* CDefineData::PACKETNAME_HIGH = "��";
const char* CDefineData::PACKETNAME_LOW = "����";
const char* CDefineData::PACKETNAME_CLOSE = "����";
const char* CDefineData::PACKETNAME_VOLUME = "�⺻�ŷ���";

const char* CDefineData::PACKETNAME_ZIGDATA = "Zig_Data";
const char* CDefineData::PACKETNAME_ZIGINDEX = "Zig_Index";

const char* CDefineData::PACKETNAME_PFHIGH = "PF_��";
const char* CDefineData::PACKETNAME_PFLOW = "PF_����";
const char* CDefineData::PACKETNAME_PFDATE = "PF_�ڷ�����";
const char* CDefineData::PACKETNAME_INVERSELINEPRICE = "���ð�_����";
const char* CDefineData::PACKETNAME_INVERSELINEVOLUME = "���ð�_�ŷ���";
const char* CDefineData::PACKETNAME_THREELINEBREAKOPEN = "ThreeLineBreak_�ð�";
const char* CDefineData::PACKETNAME_THREELINEBREAKCLOSE = "ThreeLineBreak_����";
const char* CDefineData::PACKETNAME_THREELINEBREAKDATE = "ThreeLineBreak_�ڷ�����";
const char* CDefineData::PACKETNAME_SWINGCHARTDATE = "SwingChart_�ڷ�����";
const char* CDefineData::PACKETNAME_SWINGCHARTPRICE = "SwingChart_����";
const char* CDefineData::PACKETNAME_KAGICHARTDATE = "Kagi_�ڷ�����";
const char* CDefineData::PACKETNAME_KAGICHARTPRICE = "Kagi_����";
const char* CDefineData::PACKETNAME_RENKOCHARTDATE = "RenKoChart_�ڷ�����"; // YJS (05/06/23) RENKO CHART ADD
const char* CDefineData::PACKETNAME_RENKOCHARTOPEN = "RenKoChart_�ð�";	// YJS (05/06/23) RENKOCHART OPEN ADD
const char* CDefineData::PACKETNAME_RENKOCHARTCLOSE = "RenKoChart_����"; // YJS (05/06/23) RENKO CHART ADD

// ���� �̵����, �׹���Ʈ, ���Ź�, �ϸ����ǥ, ǥ�ؿ������, ǥ�ؿ���ä��, ǥ������ä��, ��������Ʈ, 
// ��, Bollinger Bands, Box Chart, DEMA, Demark, Envelope, MAC, Parabolic, Pivot, Price Channel, 
// Starc Bands, TEMA, ZigZag
const char* CDefineData::SCALEGROUP_PRICE[Price_Count] = {INDICATEDATA_PRICECHART, \
		INDICATEDATA_PRICEMACHART, INDICATEDATA_PARABOLICCHART, \
		INDICATEDATA_RAINBOWCHART, INDICATEDATA_ZIGZAGCHART, INDICATEDATA_CLCCHART, \
		INDICATEDATA_BOLLINGERCHART, INDICATEDATA_ENVELOPECHART, INDICATEDATA_BOXCHARTCHART, \
		INDICATEDATA_PIVOTCHART, INDICATEDATA_GLANCEBALANCECHART, INDICATEDATA_PRICECHANNELCHART, \
		INDICATEDATA_DEMARKCHART, INDICATEDATA_VOLUMEFORSALE, INDICATEDATA_ERRORBANDCHART, \
		INDICATEDATA_ERRORCHANNELCHART, INDICATEDATA_DEVIATIONCHANNELCHART, INDICATEDATA_LOCKCHART, \
		INDICATEDATA_DEMACHART, INDICATEDATA_MACCHART, INDICATEDATA_STARCBANDSCHART, INDICATEDATA_TEMACHART};
const char* CDefineData::SCALEGROUP_PLUSPERCENT[PlusPercent_Count] = {INDICATEDATA_PSYCHCHART, \
		INDICATEDATA_SONAR_PSYCHCHART, INDICATEDATA_RSICHART, \
		INDICATEDATA_STOCHASTICSFASTCHART, INDICATEDATA_STOCHASTICSSLOWCHART};
const char* CDefineData::SCALEGROUP_MINUSPERCENT[MinusPercent_Count] = {INDICATEDATA_WILLIAMCHART};
const char* CDefineData::SCALEGROUP_PLUSMINUSPERCENT[PlusMinusPercent_Count] = {\
		INDICATEDATA_STOCHASTICSOSCILLATORCHART, INDICATEDATA_NEWPSYCHCHART};


// public ===============================================================================
// TR �߻��ؾ� �ϴ� �׷����� üũ
bool CDefineData::IsIndicatorGroup_TR(const CString& groupName)
{
	// <������ǥ>
	return (groupName == INDICATEGROUPNAME_MARKET);
}

// ----------------------------------------------------------------------------
// �ش� scale group ��ġ üũ
bool CDefineData::IsScaleGroup_Price(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	// <���Ź�> : ������ Ư������ ������ <������Ʈ>�� ÷�εǴ� ��Ʈ�� ������.

	for(int i = 0; i < Price_Count; i++)
	{
		if(functionName == SCALEGROUP_PRICE[i])
			return true;
	}
	return false;
}

// <������Ʈ> group ������ scale�� ���� ���� ���.
bool CDefineData::IsScaleGroup_PriceNoVertScale(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	// <��������>, <��������>
	return (functionName == INDICATEDATA_INDUSTRYJISUCHART || 
		functionName == INDICATEDATA_MARKETJISUCHART);
}

bool CDefineData::IsScaleGroup_Volume(const CString& functionName)
{
	// <�ŷ�����Ʈ>, <�ŷ��� �̵����>
	return (functionName == INDICATEDATA_VOLUMECHART || 
		functionName == INDICATEDATA_VOLUMEMACHART);
}

bool CDefineData::IsScaleGroup_InverseLine(const CString& functionName)
{
	return (functionName == INDICATEDATA_INVERSLINECHART);
}

bool CDefineData::IsScaleGroup_PlusPercent(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	CString strStochastic_Etc1, strStochastic_Etc2;
	strStochastic_Etc1.Format("%s+", INDICATEDATA_STOCHASTICSCHART);
	strStochastic_Etc2.Format("+%s", INDICATEDATA_STOCHASTICSCHART);
	for(int i = 0; i < PlusPercent_Count; i++)
	{
		if(functionName == SCALEGROUP_PLUSPERCENT[i])
			return true;
		else if(functionName.Find(strStochastic_Etc1) >= 0 || functionName.Find(strStochastic_Etc2) >= 0)
			return true;
	}
	return false;
}

bool CDefineData::IsScaleGroup_MinusPercent(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	for(int i = 0; i < MinusPercent_Count; i++)
	{
		if(functionName == SCALEGROUP_MINUSPERCENT[i])
			return true;
	}
	return false;
}

bool CDefineData::IsScaleGroup_PlusMinusPercent(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	for(int i = 0; i < PlusMinusPercent_Count; i++)
	{
		if(functionName == SCALEGROUP_PLUSMINUSPERCENT[i])
			return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
// x�� �������� ǥ������ �ʴ��� üũ
bool CDefineData::IsNoXScale_SpecialGroup(const CString& functionName)
{
	if(functionName == INDICATEDATA_PANDFCHART)	// "P&F"
		return true;
//	else if(functionName == INDICATEDATA_THREELINEBRACKCHART) // "�Ｑ��ȯ��"
//		return true;

	return false;
}

// ----------------------------------------------------------------------------
// <�ŷ��� ��Ʈ>, <�ŷ����> �� ���
bool CDefineData::IsDrawGroup_VolumeType(const CString& functionName)
{
	return (functionName == INDICATEDATA_VOLUMECHART || functionName == INDICATEDATA_VOLUMEPRICE);
}

// <P&F> ������ ��ǥ���� üũ
bool CDefineData::IsDrawGroup_PAndF(const CString& functionName)
{
	return (functionName.Find(INDICATEDATA_PANDFCHART) != -1);
}

// <Ư����Ʈ> ������ ����
bool CDefineData::IsSpecialGroupGraph(const CString& functionName)
{
	if(functionName == INDICATEDATA_THREELINEBRACKCHART) // "�Ｑ��ȯ��"
		return true;
	else if(functionName == INDICATEDATA_INVERSLINECHART) // "���ð�"
		return true;
	else if(functionName == INDICATEDATA_PANDFCHART)	// "P&F"
		return true;
	else if(functionName == INDICATEDATA_SWINGCHART)	// "SwingChart"
		return true;
	else if(functionName == INDICATEDATA_KAGICHART)		// "KagiChart"
		return true;
	else if(functionName == INDICATEDATA_RENKOCHART)	// "RenKoChart" YJS (05/06/23) RENKO CHART ADD
		return true;

	return false;
}

// <�����ڵ���> ������ ����
bool CDefineData::IsInvestorGroupGraph(const CString& functionName)
{
	// <����������>, <���������>, <�ܱ���������>
	return (functionName == INDICATEDATA_INDIVIDUAL_INVESTOR || 
		functionName == INDICATEDATA_ORGAN_INVESTOR || 
		functionName == INDICATEDATA_FOREIGNER_INVESTOR);
}

// ----------------------------------------------------------------------------
// min/max ���ϴ� packet�� ������(2) �� ���
bool CDefineData::IsViewVertMinMax_EndPacket(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	if(functionName == INDICATEDATA_ZIGZAGCHART)	// Zig Zag
		return true;
	else if(functionName == INDICATEDATA_CLCCHART)	// ��������Ʈ
		return true;

	return false;
}

// ----------------------------------------------------------------------------
// condition title ���� üũ
bool CDefineData::IsConditionTitle(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	if(functionName == INDICATEDATA_PRICECHART)	// ������Ʈ
		return false;
	else if(functionName == INDICATEDATA_VOLUMECHART)	// �ŷ�����Ʈ
		return false;
	else if(functionName == INDICATEDATA_PRICEMACHART)	// ���� �̵����
		return false;
	else if(functionName == INDICATEDATA_VOLUMEMACHART)	// �ŷ��� �̵����
		return false;
	else if(functionName == INDICATEDATA_DISPARITYCHART)// �̰ݵ�
		return false;
	else if(functionName == INDICATEDATA_COMPARATIVECHART)	// ����Ʈ
		return false;
	else if(functionName.Find(INDICATEDATA_PANDFCHART) != -1)// P&F, FuOpP&F
		return false;

	return true;
}

// data title ���� üũ
bool CDefineData::IsDataTitle(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	if(functionName == INDICATEDATA_PRICECHART)	// ������Ʈ
		return false;
	else if(functionName == INDICATEDATA_VOLUMECHART)	// �ŷ�����Ʈ
		return false;
	else if(functionName == INDICATEDATA_RAINBOWCHART)	// �׹���Ʈ
		return false;
	else if(functionName == INDICATEDATA_BOXCHARTCHART)	// "Box Chart"
		return false;
	else if(functionName == INDICATEDATA_CLCCHART)		// ��������Ʈ
		return false;
	else if(functionName == INDICATEDATA_ZIGZAGCHART)	// Zig Zag
		return false;
	else if(functionName == INDICATEDATA_VOLUMEFORSALE)	// ���Ź�
		return false;

	return true;
}

// title�� ���ǰ������� ����
bool CDefineData::IsEqualTitleAndCondition(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	if(functionName == INDICATEDATA_PRICEMACHART)	// ���� �̵����
		return true;
	else if(functionName == INDICATEDATA_VOLUMEMACHART)	// �ŷ��� �̵����
		return true;

	return false;
}

// ----------------------------------------------------------------------------
