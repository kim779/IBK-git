// DrawingRegionCalculator.h: interface for the CDrawingRegionCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGREGIONCALCULATOR_H__16FD5E76_FAA3_4228_8E7F_E9BAF4B69903__INCLUDED_)
#define AFX_DRAWINGREGIONCALCULATOR_H__16FD5E76_FAA3_4228_8E7F_E9BAF4B69903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseData.h"

class CScaleCompart;
class CDrawingRegion;
class CHorzScaleRegionHeight;
class CVertScaleRegionWidth;
class CTextSizeCalculator;
class CPacketList;
class CPacket;
class CIndicatorInfo;
class CGraphRegions;
class CDisplayAttributes;
class CDisplayDataMinMax;
class CDrawingRegionCalData;
class CVertScaleMaxWidthCalculatorData;
class CHorzScaleMaxWidthCalculatorData;
class CSubGraphPacketList;

// �׸��� ������ ���� ��� class
class CDrawingRegionCalculator  
{
public:
	// �׸��� ����(graph, scale)�� ���Ѵ�.
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	CDrawingRegion GetDrawingRegion(CDC* pDC, const CDrawingRegionCalData& calData, BOOL p_bReverse) const;

private:
	// scale ����
	CRect GetLeftScaleRegion(const CRect& blockRegion, const CRect& graphFrameRegion, const CRect& graphRegion, const int nWidth) const;
	CRect GetRightScaleRegion(const CRect& blockRegion, const CRect& graphFrameRegion, const CRect& graphRegion, const int nWidth) const;
	CRect GetTopScaleRegion(	const CRect& blockRegion, const CRect &graphDrawRegion, const CRect& graphFrameRegion) const;
	CRect GetBottomScaleRegion(	const CRect& blockRegion, const CRect &graphDrawRegion, const CRect& graphFrameRegion) const;
};


// graph ������ ����ϴ� class
class CGraphRegionCalculator
{
public:
	// graph ������.
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	CGraphRegions GetGraphRegions(CDC* pDC, const CDrawingRegionCalData& calData, BOOL p_bReverse) const;

private:
	// graph frame�� ���Ե� ������ ���Ѵ�.
	CRect GetGraphFrameRegion(CDC* pDC, const CDrawingRegionCalData& calData) const;

	// �ִ� graph frame region(frame ����) --> (block region - scale region)
	CRect GetMaxGraphFrameRegion(const CRect& blockRegion, const CHorzScaleRegionHeight& horzScaleRegionHeight, const CVertScaleRegionWidth& vertScaleRegionWidth) const;
	// view data count�� ���� graph������ �����Ѵ�. (���� scale ������ �Ѱ��ش�.)
	CRect GetRealGraphRegion(const CDrawingRegionCalData& calData, const CRect& maxGraphDrawingRegion) const;

	// ���� scale �� Ÿ��
	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition(const CVertScaleRegionWidth& vertScaleRegionWidth) const;
	// ���� scale������ �� �� (graph�� x�� ����)
	int GetRemainedSpaceWidth(const int graphDrawingRegionWidth, const int viewDataCount) const;

	// graph drawing region�� ����&������ ��ǥ�� ���Ѵ�. ==> space ����
	int GetLeft(const CScaleBaseData::VERTSCALEPOSITION vertScalePosition, const int orgLeft, 
		const int maxVertScaleRegionWidth, const int leftVertScaleRegionWidth, const int remainedSpaceWidth) const;
	int GetRight(const CScaleBaseData::VERTSCALEPOSITION vertScalePosition, const int orgRight, 
		const int maxVertScaleRegionWidth, const int rightVertScaleRegionWidth, const int remainedSpaceWidth) const;
	int GetData(const int orgData, const int maxVertScaleRegionWidth, const int vertScaleRegionWidth, const int remainedSpaceWidth, const int scalePosition) const;

	// graph text�� �� ���� ����
	bool IsGraphTextDraw(const CRect& graphRegion, const CDrawingRegionCalData& calData) const;
};


// scale text size���ϴ� �Ϳ� ���� class
class CTextSizeCalculator
{
// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

public:
	CTextSizeCalculator( HWND p_hOcxWnd, CChartInfo::CHART_MODE	p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}

public:
	// ���� scale�� �ִ� text size
	CSize GetMaxHorzScaleTextSize(CDC* pDC, const CHorzScaleMaxWidthCalculatorData& calData) const;
	// ���� scale�� �ִ� text size.
	CSize GetMaxVertScaleTextSize(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;

private:
	// text size�� �������� ����
	bool IsTextSizeCalculator(CPacket* pPacket) const;
	bool IsTextSizeCalculator(CSubGraphPacketList* pSubGraphPacketList) const;
	// <���ؼ�> �� �ִ����� ����
	bool IsBaseLineInIndicateData(CIndicatorInfo* pIndicatorInfo) const;
	// <���ؼ�> �� �׸��� ���������� ����
	bool IsBaseLineInRegion(const CVertScaleMaxWidthCalculatorData& calData) const; 

	// scale data text size.
	CSize GetMaxScaleText_Data(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	// ������ scale data rate text size.
	CSize GetMaxScaleText_Rate(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;

	// scale data text size. -> <�ڷ�����>�� ���.
	CSize GetMaxScaleTextSize_DataDate(CDC* pDC, CPacket* pPacket, const CScaleCompart& scaleCompart) const;
	// scale data text size. -> <��ǥ data>�� ���.
	CSize GetMaxScaleTextSize_IndicateData(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	//						 -> <���Ź�>
	CSize GetMaxScaleTextSize_Sale(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	//						 -> <���ؼ�>
	CSize GetMaxScaleTextSize_BaseLine(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	// scale data text size. -> <packet>�� ���.
	CSize GetMaxScaleTextSize_Packet(CDC* pDC, const CHorzScaleMaxWidthCalculatorData& calData) const;
	CSize GetMaxScaleTextSize_Packet(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	CSize GetMaxScaleTextSize_Packet(CDC* pDC, CPacket* pPacket, const CVertScaleMaxWidthCalculatorData& calData) const;

	bool GetPacketMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, double& dMin, double& dMax) const;

	// ���� �� ���� ��������
	int GetLongLength(CIndicatorInfo* pIndicatorInfo) const;
	int GetMaxPacketDataLength( const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList,
								const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
								const CDisplayDataMinMax &p_displaySubGraphMinMax) const;
	POSITION GetDrawingDataLengthStartPosition(const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList) const;
	int GetDrawingDataLength( CPacket* pPacket, const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const;
	int GetDrawingDataLength_WithMinMax( CPacket* pPacket, const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
											const CDisplayDataMinMax &p_displaySubGraphMinMax) const;
	int GetDrawingDataLength_Text(CPacket* pPacket) const;
	int GetDrawingDataLength_Number(CPacket* pPacket, const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const;
	int GetDrawingDataLength_Number_WithMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
											const CDisplayDataMinMax &p_displaySubGraphMinMax) const;
	bool GetDrawingData_Number(CPacket* pPacket, const CDisplayAttributes& displayAttributes, CString& strData, CString& strFormatTrans) const;
	bool GetDrawingData_Number_WithMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, CString& strData, CString& strFormatTrans,
											const CDisplayDataMinMax &p_displaySubGraphMinMax) const;
	//double GetLongLengthData(const double& dData1, const double& dData2, const CString& dataType) const;
	int GetDataLength(const CString& strData) const;
	int GetFormatLength(const CString& strFormat, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const;
	
	// packet �� �����´�.
	CPacket* GetVertScalePacket(CSubGraphPacketList* pSubGraphPacketList, const CString& strPacketName) const;
	CPacket* GetVertScalePacket(CPacketList* pPacketList, const CString& strPacketNames) const;
	// <�ڷ�����> packet�� ó�� data�� type�� �����´�.
	bool GetDatePacketHeadDataAndType_DataDate(CPacket* pPacket, CString& headPacketData, CString& dataType) const;

	CSize GetMaxScaleTextSize(CDC* pDC, const int nLength, const CScaleBaseData::TEXTCOMPARTTYPE textCompart = CScaleBaseData::HORZ_TEXT_COMPART) const;
	CSize GetMaxScaleTextSize(const CSize& textSize, const int nLength, const CScaleBaseData::TEXTCOMPARTTYPE textCompart = CScaleBaseData::HORZ_TEXT_COMPART) const;

	// �����ʿ� scale �������� ���� �κ����� <���簡> ���� �κ�
	//bool GetPrices(const CVertScaleMaxWidthCalculatorData& calData, double& dPrevPrice, double& dCurPrice) const;
	//CString GetCurrentPriceText(const double& dCurPrice) const;
	//CString GetRateText(const double& dPrevPrice, const double& dCurPrice) const;

	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	CSize GetMaxScaleTextSize_Sale2(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
};

#endif // !defined(AFX_DRAWINGREGIONCALCULATOR_H__16FD5E76_FAA3_4228_8E7F_E9BAF4B69903__INCLUDED_)
