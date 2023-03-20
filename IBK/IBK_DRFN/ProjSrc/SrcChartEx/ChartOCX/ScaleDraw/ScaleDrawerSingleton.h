// ScaleDrawerSingleton.h: interface for the CScaleDrawerSingleton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEDRAWERSINGLETON_H__97C5F0A1_8E46_4950_A2D8_25EBB6C5E258__INCLUDED_)
#define AFX_SCALEDRAWERSINGLETON_H__97C5F0A1_8E46_4950_A2D8_25EBB6C5E258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TextScaleDrawer.h"
#include "VolumeScaleDrawer.h"
#include "PriceScaleDrawer.h"
#include "SpecialScaleDrawer.h"
#include "IndicateScaleDrawer.h"
#include "PacketDataScaleDrawer.h"
#include "LogScaleDrawer.h"
#include "DataHorzScaleDrawer.h"
#include "FullTimeScaleDrawer.h"		// for CFullTimeScaleDrawer

class CPacket;

// data �� ���� scale
class CDataHorzScaleDrawerSingleton
{
public:
	CDataHorzScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
public:
	CHorzScaleDrawer* GetDataHorzScaleDrawer(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType);

private:
	CPriceZoneHorzScaleDrawer m_priceZoneScaleDrawer;	// "��簡"
	CEtcDataHorzScaleDrawer m_etcHorzScaleDrawer;		// "volume" & packet data
	CLeftRightScaleDrawer m_leftRightScaleDrawer;		// �¿��
	CMarketProfileHorzScaleDrawer m_marketProfileHorzScaleDrawer; // "MarketProfile"
};

// ���� scale
class CHorzScaleDrawerSingleton  
{
public:
	CHorzScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
public:
	CHorzScaleDrawer* GetHorzScaleDrawer(CPacket* pPacket, const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType);
	CHorzScaleDrawer* GetHorzScaleDrawer(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType);
	
private:
	CTextHorzScaleDrawer m_textHorzScaleDrawer; // "text"
	CDataHorzScaleDrawerSingleton m_dataSingleton;

protected:
	CFullTimeScaleDrawer	m_fullTimeScaleDrawer;
};


///////////////////////////////////////////////////////////////////////////////////////
// ���� scale
class CVertScaleDrawerSingleton
{
public:
	CVertScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	CVertScaleDrawer* GetVertScaleDrawer(const CScaleBaseData::VERTSCALEDRAWERTYPE drawerType);

private:
	CPriceScaleDrawer m_priceScaleDrawer;			// ���� ������ 
	CVolumeVertScaleDrawer m_volumeVertScaleDrawer;	// �ŷ��� ������
	CSaleScaleDrawer m_saleScaleDrawer;				// ���Ź���Ʈ ���� ������
	CMarketProfileVertScaleDrawer m_marketProfileVertDrawer;// MarketProfile ���� ������
	CIndicateScaleDrawer m_indicateScaleDrawer;		// ��ǥ ����Ÿ (������ : ����Ÿ / ���� : ���ذ�)
	CPacketDataScaleDrawer m_packetDataScaleDrawer;	// �Ϲ� ����Ÿ
	CTextVertScaleDrawer m_textVertScaleDrawer;		// text
	CLogScaleDrawer m_logScaleDrawer;				// log scale (������Ʈ, ������Ʈ)
	CPercentFullScaleDrawer m_percentScaleDrawer;	// �����(0%~100% ����)
	CTopButtomPlusScaleDrawer m_topButtomPlusScaleDrawer;	// +���Ͻ�
};

#endif // !defined(AFX_SCALEDRAWERSINGLETON_H__97C5F0A1_8E46_4950_A2D8_25EBB6C5E258__INCLUDED_)
