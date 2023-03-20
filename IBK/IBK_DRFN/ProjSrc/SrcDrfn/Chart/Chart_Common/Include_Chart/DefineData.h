// DefineData.h: interface for the CDefineData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFINEDATA_H__B700E04F_7129_4D8B_A454_4ABAFE870787__INCLUDED_)
#define AFX_DEFINEDATA_H__B700E04F_7129_4D8B_A454_4ABAFE870787__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDefineData  
{
public:
	static const char* PACKET_TYPE_TEXT;

	static const char* SCALEDATA_DATE;
	static const char* SCALEDATA_TEXT;
	static const char* SCALEDATA_PRICEZONE;
	static const char* SCALEDATA_INDICATEDATA;
	static const char* SCALEDATA_VOLUME;
	static const char* SCALEDATA_PERCENT;

	// ��ǥ �׷�
	static const char* INDICATEGROUPNAME_MARKET;

	// ����� �ʿ���� ��ǥ
	static const char* INDICATEDATA_PRICECHART;
	static const char* INDICATEDATA_VOLUMECHART;
	static const char* INDICATEDATA_OPTIMIZEAIGNALCHART;
	static const char* INDICATEDATA_VOLUMEPRICE;

	// ����� �ʿ��� ��ǥ
	static const char* INDICATEDATA_PRICEMACHART;
	static const char* INDICATEDATA_VOLUMEMACHART;
	static const char* INDICATEDATA_DISPARITYCHART;
	static const char* INDICATEDATA_GLANCEBALANCECHART;
	static const char* INDICATEDATA_PSYCHCHART;
	static const char* INDICATEDATA_NEWPSYCHCHART;
	static const char* INDICATEDATA_SONAR_PSYCHCHART;
	static const char* INDICATEDATA_BOLLINGERCHART;
	static const char* INDICATEDATA_BOXCHARTCHART;
	static const char* INDICATEDATA_CLCCHART;
	static const char* INDICATEDATA_ENVELOPECHART;
	static const char* INDICATEDATA_PARABOLICCHART;
	static const char* INDICATEDATA_PIVOTCHART;
	static const char* INDICATEDATA_RSICHART;
	static const char* INDICATEDATA_STOCHASTICSFASTCHART;
	static const char* INDICATEDATA_STOCHASTICSSLOWCHART;
	static const char* INDICATEDATA_STOCHASTICSOSCILLATORCHART;
	static const char* INDICATEDATA_WILLIAMCHART;
	static const char* INDICATEDATA_RAINBOWCHART;
	static const char* INDICATEDATA_ZIGZAGCHART;
	static const char* INDICATEDATA_PRICECHANNELCHART;
	static const char* INDICATEDATA_DEMARKCHART;
	static const char* INDICATEDATA_ERRORBANDCHART;
	static const char* INDICATEDATA_ERRORCHANNELCHART;
	static const char* INDICATEDATA_DEVIATIONCHANNELCHART;
	static const char* INDICATEDATA_DEMACHART;
	static const char* INDICATEDATA_MACCHART;
	static const char* INDICATEDATA_STARCBANDSCHART;
	static const char* INDICATEDATA_TEMACHART;
	static const char* INDICATEDATA_LOCKCHART;
	static const char* INDICATEDATA_THREELINEBRACKCHART;
	static const char* INDICATEDATA_INVERSLINECHART;
	static const char* INDICATEDATA_PANDFCHART;
	static const char* INDICATEDATA_SWINGCHART;
	static const char* INDICATEDATA_KAGICHART;
	static const char* INDICATEDATA_RENKOCHART;
	static const char* INDICATEDATA_FUOPPANDFCHART;
	static const char* INDICATEDATA_COMPARATIVECHART;
	static const char* INDICATEDATA_MARKETPROFILECHART;
	static const char* INDICATEDATA_MINIJISUCHART;
	static const char* INDICATEDATA_INDUSTRYJISUCHART;
	static const char* INDICATEDATA_MARKETJISUCHART;
	static const char* INDICATEDATA_INDIVIDUAL_INVESTOR;
	static const char* INDICATEDATA_ORGAN_INVESTOR;
	static const char* INDICATEDATA_FOREIGNER_INVESTOR;

	static const char* INDICATEDATA_PRICE;
	static const char* INDICATEDATA_VOLUME;
	static const char* INDICATEDATA_PRESPEN;
	static const char* INDICATEDATA_VOLUMEFORSALE;
	static const char* INDICATEDATA_VOLUMEFORSALEINPRICE;

	// TR ��Ŷ��
	static const char* PACKETNAME_OPEN;
	static const char* PACKETNAME_HIGH;
	static const char* PACKETNAME_LOW;
	static const char* PACKETNAME_CLOSE;
	static const char* PACKETNAME_VOLUME;

	// �Ϲ���ǥ�� ��Ŷ��	
	static const char* PACKETNAME_ZIGDATA;
	static const char* PACKETNAME_ZIGINDEX;
	
	// Ư����ǥ�� ��Ŷ��
	static const char* PACKETNAME_PFHIGH;
	static const char* PACKETNAME_PFLOW;
	static const char* PACKETNAME_PFDATE;
	static const char* PACKETNAME_INVERSELINEPRICE;
	static const char* PACKETNAME_INVERSELINEVOLUME;
	static const char* PACKETNAME_THREELINEBREAKOPEN;
	static const char* PACKETNAME_THREELINEBREAKCLOSE;
	static const char* PACKETNAME_THREELINEBREAKDATE;
	static const char* PACKETNAME_SWINGCHARTDATE;
	static const char* PACKETNAME_SWINGCHARTPRICE;
	static const char* PACKETNAME_KAGICHARTPRICE;
	static const char* PACKETNAME_KAGICHARTDATE;
	static const char* PACKETNAME_RENKOCHARTDATE;
	static const char* PACKETNAME_RENKOCHARTOPEN;
	static const char* PACKETNAME_RENKOCHARTCLOSE;

	// TR �߻��ؾ� �ϴ� �׷����� üũ
	static bool IsIndicatorGroup_TR(const CString& groupName);

	// �ش� scale group ���� üũ
	static bool IsScaleGroup_Price(const CString& functionName);
	static bool IsScaleGroup_PriceNoVertScale(const CString& functionName);
	static bool IsScaleGroup_Volume(const CString& functionName);
	static bool IsScaleGroup_InverseLine(const CString& functionName);
	static bool IsScaleGroup_PlusPercent(const CString& functionName);
	static bool IsScaleGroup_MinusPercent(const CString& functionName);
	static bool IsScaleGroup_PlusMinusPercent(const CString& functionName);

	// x�� �������� ǥ������ �ʴ��� üũ
	static bool IsNoXScale_SpecialGroup(const CString& functionName);

	// �ش� �׸��� group ���� üũ
	static bool IsDrawGroup_VolumeType(const CString& functionName);
	// <P&F> ������ ��ǥ���� üũ
	static bool IsDrawGroup_PAndF(const CString& functionName);
	// <Ư����Ʈ> ������ ����
	static bool IsSpecialGroupGraph(const CString& functionName);
	// <�����ڵ���> ������ ����
	static bool IsInvestorGroupGraph(const CString& functionName);

	// min/max ���ϴ� packet�� ������(2) �� ���
	static bool IsViewVertMinMax_EndPacket(const CString& functionName);

	// title�� �� �������� üũ
	static bool IsConditionTitle(const CString& functionName);
	static bool IsDataTitle(const CString& functionName);
	// title�� ���ǰ������� ����
	static bool IsEqualTitleAndCondition(const CString& functionName);

private:
	enum SCALEGROUPCOUNT
	{
		Price_Count = 22,
		PlusPercent_Count = 5,
		MinusPercent_Count = 1,
		PlusMinusPercent_Count = 2
	};

	static const char* INDICATEDATA_STOCHASTICSCHART;

	static const char* SCALEGROUP_PRICE[Price_Count];
	static const char* SCALEGROUP_PLUSPERCENT[PlusPercent_Count];
	static const char* SCALEGROUP_MINUSPERCENT[MinusPercent_Count];
	static const char* SCALEGROUP_PLUSMINUSPERCENT[PlusMinusPercent_Count];
};

#endif // !defined(AFX_DEFINEDATA_H__B700E04F_7129_4D8B_A454_4ABAFE870787__INCLUDED_)
