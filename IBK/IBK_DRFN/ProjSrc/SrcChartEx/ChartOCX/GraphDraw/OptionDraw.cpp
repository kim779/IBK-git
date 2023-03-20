// OptionDraw.cpp: implementation of the COptionDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionDraw.h"

#include "DataMath.h"
#include "Conversion.h"
#include "SpecialDraw.h"

#include "BaseData.h"
#include "PacketPointerList.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"

#include "PacketList.h"
#include "PacketBase.h"
#include "XScaleManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionDraw::COptionDraw()
{
	m_eGraphType = CGraphBaseData::FuOp_Type;
}

COptionDraw::~COptionDraw()
{

}

bool COptionDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if( packetList.GetCount() <= 0)
		return false;
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 

	switch(subGraphData.GetStyle().GetGraphFuOpStyle()){
	case CGraphBaseData::MarketProfile:
		return DrawMarketProfile(pDC, realRegion, dViewMin, dViewMax, 
			subGraphData.GetColor(), packetList);
	case CGraphBaseData::FuOpPAndF:
		return DrawFuOpPAndF(pDC, realRegion, displayAttributes/*drawingGraphData.GetIndicatorInfo()*/, 
			subGraphData.GetColor(), dViewMin, dViewMax, packetList);
	}

	return false;
}

bool COptionDraw::DrawGraph(CDC* pDC,
							const CRect& realRegion, 
							const CDisplayAttributes& displayAttributes, 
							const CDrawingGraphData& drawingGraphData,
							const CSubGraphData& subGraphData,
							CXScaleManager* pxScaleManager)
{
	if(pxScaleManager && pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
		return true;

	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if( packetList.GetCount() <= 0)
		return false;
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	
	switch(subGraphData.GetStyle().GetGraphFuOpStyle()){
	case CGraphBaseData::MarketProfile:
		return DrawMarketProfile(pDC, realRegion, dViewMin, dViewMax, 
			subGraphData.GetColor(), packetList);
	case CGraphBaseData::FuOpPAndF:
		return DrawFuOpPAndF(pDC, realRegion, displayAttributes/*drawingGraphData.GetIndicatorInfo()*/, 
			subGraphData.GetColor(), dViewMin, dViewMax, packetList);
	}
	
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.01.20
//
//	�м�	:	MP Chart�� ��� Routine�̴�.
//
//	����	:	(2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
//					�̸� ���Ͽ� CMProfile Chart�� Member�� Last Price�� ����Ͽ� ��½� ��������Ѵ�.
//					Server������ TR Packet ���� �߰� Packet Data�� Alphabet�� '*'�� Data�� �޾�,
//						�� Packet Data�� Price�� CMProfile Chart�� Last Price�� �����Ѵ�.
//						�� ���� Packet�� ������ ��ȸ�� �ϴ��� �������� 1���� �߰��ȴ�.
//				(2004.02.05, ��¿�) MP Chart�� ��½� TEXT ����� MP Chart�� 3�� �̳��� ��찡 �ƴ϶�,
//					TEXT�� ũ�Ⱑ Ư�� ũ�� ���Ϸ� �������� ���� �����Ѵ�.
//				(2004.02.05, ��¿�) �ŷ����� Bracket�� ������ ���� Bracket�� ũ�⺸�� ���� �ʵ��� �Ѵ�.
///////////////////////////////////////////////////////////////////////////////
#define _MARKETPROFILE_MIN_FONTSIZE	10	// (pt)
bool COptionDraw::DrawMarketProfile(CDC* pDC, const CRect& graphRegion, const double& dViewMin, const double& dViewMax, 
		const CGraphColor& graphColor, const CList<CPacket*, CPacket*> &packetList)
{
	// 1. ���� ������ ��츦 �����Ѵ�.
	// 1.1 Packet List�� ������ �ִ� ��츦 �����Ѵ�.
	if( packetList.GetCount() <= 0)	return false;
	// 1.2 MarketProfile Packet�� ���� ��츦 �����Ѵ�.
	CMPPacket* pMPPacket = (CMPPacket*) packetList.GetHead();
	if( !pMPPacket) return false;
	// 1.3 MarketProfile Packet�� MP ���� Chart List�� ������ ���ϴ� ��츦 �����Ѵ�.
	CList<CMProfile*, CMProfile*> *pMProfileList = pMPPacket->GetMarketProfileList();
	// 1.3.1 CMProfile Data Object�� ���� �����Ͽ� ���Ŀ� �̿��� �� �ֵ��� �Ѵ�.
	int nMProfileCount = pMProfileList->GetCount();
	if( !pMProfileList || nMProfileCount < 1) return false;

	// 2. ȭ���� ������ ��ġ�� ���Ѵ�.
	//		���ϴ� ������
	//			1. ���� Bracket�� ���̸� �����Ѵ�. (�̰��� 1ȸ�� ����� Ȯ���ȴ�.)
	//			2. ����� Bracket�� ���̸� �������� Font ũ�⸦ �����Ѵ�.
	//				*. �̶� Font�� ũ�⸦ �������� Text ��¿��θ� Ȯ���Ѵ�.
	//			3. ����� Font ũ��� Text ��¿��θ� �̿��Ͽ� ��ü ������ ũ�⸦ �����Ѵ�.
	//			4. ����� ��ü ������ �⺻ ũ�Ⱑ �־��� ȭ�� �������� ũ��, �� ũ���� ������ ���� Bracket�� ���� �����Ѵ�.
	//				*. �̶� Text�� ����� ���Ե� �����, �ٽ� Ȯ���Ͽ� Text�� ����� ��Ȳ������ ����,
	//					Text�� ������� �ʴ� ������ ������ ���, �ٽ� ������ ���� �÷� ������� �Ѵ�.
	//			5. ����� ��ü ������ �⺻ ũ�Ⱑ �־��� ȭ�� �������� ������ ���� ������ �����Ͽ� Bracket�� ���� �߰� �����Ѵ�.
	// 2.1 Bracket�� ���̸� ���ϴ�. 
	//		graphRegion			: Graph ��� ���� Rect
	//		dViewMin, dViewMax	: Graph ��� ������ Scale ��			(Logical graphRegion)
	//		nBracketHeight		: Graph ��� ������ ���� Bracket Height	
	//		nPriceUnit			: Graph ��� ������ ���� Scale ��		(Logical nBracketHeight)
	int nPriceUnitX100 = pMPPacket->GetPriceUnitX100();
	int nBracketHeight = ( graphRegion.Height() * nPriceUnitX100) / int( dViewMax * 100 - dViewMin * 100);
	// 2.2 Bracket�� ���̸� �������� MP Chart�� �⺻ ������ �����Ѵ�.
	// 2.2.1 ���� Font�� �����Ѵ�. (������ �ŷ��� ������ Bracket�� ������ ũ�� ���⿡ �̿�ȴ�.)
	//		(Font ũ�� ��������� MSDN�� Knowledge Base, Win32 Device Driver Kit, [Q74299] INFO: Calculating The Logical Height and Point Size of a Font ����)
	//		(������ ������ Font�� �����ϴ� ���� �����̹Ƿ� �־��� Height�� Negative�� �����Ͽ� ������ Font Object�� �����Ѵ�.)
	// 2.2.1.1 ���� Font�� �����Ѵ�.
	CFont fFont;
	fFont.CreateFont( -nBracketHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
	// 2.2.1.2 ���� Font�� Internal Leading�� �����Ѵ�.
	TEXTMETRIC tmMetric;
	CFont* pOldfont = ( CFont *)pDC->SelectObject( &fFont);
	pDC->GetTextMetrics( &tmMetric);
	int nInternalLeading = tmMetric.tmInternalLeading;
	// 2.2.1.3 ��Ȯ�� �ʿ��� Font�� ũ�⸦ �����Ѵ�.
	int nFontHeight = MulDiv( nBracketHeight - nInternalLeading, pDC->GetDeviceCaps( LOGPIXELSY), 72);
	// 2.2.1.3.5 (2004.02.05, ��¿�) MP Chart�� ��½� TEXT ����� MP Chart�� 3�� �̳��� ��찡 �ƴ϶�,
	//		TEXT�� ũ�Ⱑ Ư�� ũ�� ���Ϸ� �������� ���� �����Ѵ�.
	// 2.2.1.3.5.1 ���� TEXT ����� �����ؾ� �ϴ� ������� Ȯ���Ѵ�.
	BOOL bWithText = ( _MARKETPROFILE_MIN_FONTSIZE <= nFontHeight);
	// 2.2.2 �ŷ��� ������ Bracket�� ������ ���Ѵ�. (�ŷ��� ������ Bracket ������ �����ϴ� ���� CMProfile�� 3�� �̳��� ����̴�.)
	int nVolumeWidth = 0, nBracketCntWidth = 0;
	CString strMaxVolume, strMaxBracketCnt;
	if( bWithText)
	{
		// 2.2.2.1  ��Ȯ�� Font�� �����Ѵ�.
		pDC->SelectObject( pOldfont);
		fFont.DeleteObject();
		fFont.CreateFont( nFontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
		pOldfont = ( CFont *)pDC->SelectObject( &fFont);
		// 2.2.2.3 ���� �ŷ��� ������ ���� �����Ѵ�.
		strMaxVolume.Format( "%ld", ( long)pMPPacket->GetMaxVolume());
		nVolumeWidth = pDC->GetTextExtent( strMaxVolume).cx + 4;			// �¿� ���� ���� and ��輱 1 Pixel �߰�
		// 2.2.2.2 Bracket�� ������ ���� �����Ѵ�.
		strMaxBracketCnt.Format( "%d", pMPPacket->GetMaxHitting());
		nBracketCntWidth = pDC->GetTextExtent( strMaxBracketCnt).cx + 4;	// �¿� ���� ���� and ��輱 1 Pixel �߰�
	}
	// 2.2.3 Bracket�� �⺻ ũ�⸦ ���Ѵ�.
	// 2.2.3.0 ������ Font�� ���� �������� Bracket�� �⺻ ũ�⸦ ���Ѵ�.
	// (2004.01.26, ��¿�) Font�� ���� GetTextExtent()�� �̿��Ѵ�. (LOGFONT�� Width�� �������� �ʴ´�. ���� �����ߴ����� ����� ��)
	// (2004.02.05, ��¿�) MP Chart�� ��½� TEXT ����� MP Chart�� 3�� �̳��� ��찡 �ƴ϶�,
	//		TEXT�� ũ�Ⱑ Ư�� ũ�� ���Ϸ� �������� ���� �����Ѵ�.
	//		Text�� ��µ��� ���� ����� Bracket Width�� �⺻������ Bracket Height�� ������ �Ѵ�.
	int nBracketWidth = ( bWithText ? pDC->GetTextExtent( "W").cx + 2 : nBracketHeight); // �¿� ���� or ��輱 1 Pixel �߰�
	// 2.2.4 ��ü ������ �⺻ ũ�⸦ ���Ѵ�.
	//	(ó���� ���� Bracket Width�� CMProfile Chart�� �¿� ������ �ǹ��Ѵ�.)
	int nMaxBracketCountInPacket = pMPPacket->GetMaxBracketCount();
	int nRequestRegionX = ( nBracketWidth + nVolumeWidth + nBracketCntWidth + nBracketWidth * nMaxBracketCountInPacket + nBracketWidth)
		* nMProfileCount;
	// 2.3 ������ �⺻ ������ ���Ͽ�, �־��� ȭ���� ���� ��츦 ó���Ѵ�.
	//	(�� ũ���� ������ ���� Bracket�� ���� �����ϰ�, ���� Font�� �ٸ� �����鵵 �߰� �����Ѵ�.)
	if( graphRegion.Width() < nRequestRegionX)
	{
		// 2.3.1 ������ ���� Bracket�� ���� ���� �����Ѵ�.
		nBracketWidth = ( nBracketWidth * graphRegion.Width()) / nRequestRegionX;
		// (2004.02.05, ��¿�) MP Chart�� ��½� TEXT ����� MP Chart�� 3�� �̳��� ��찡 �ƴ϶�,
		//		TEXT�� ũ�Ⱑ Ư�� ũ�� ���Ϸ� �������� ���� �����Ѵ�.
		//		�̹� ���������� Text�� ����� ��Ȳ�� �ȵǴ� ��츦 ó���Ѵ�. (���� �ȵǴ� ��� ����..)
		if( bWithText)
		{
			// 2.3.2 ���ο� Font�� ũ�⸦ �����Ͽ�, Text�� ����� ��Ȳ������ Ȯ���Ѵ�.
			nFontHeight = ( nFontHeight * graphRegion.Width()) / nRequestRegionX;
			bWithText = ( _MARKETPROFILE_MIN_FONTSIZE <= nFontHeight);
			// 2.3.3 Text�� ����� ��Ȳ���� Ȯ�ε� ����� Text ������ ũ�⸦ �����Ѵ�.
			if( bWithText)
			{
				// 2.3.3.2 Font�� ũ�⸦ �����Ѵ�. (���� Font�� �����ϰ� ������ �����Ѵ�.)
				pDC->SelectObject( *pOldfont);
				fFont.DeleteObject();
				// 2.3.3.3 ���ο� Font�� �����Ѵ�.
				fFont.CreateFont( nFontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
				pOldfont = ( CFont *)pDC->SelectObject( &fFont);
				// 2.3.3.4 �ŷ��� ������ Bracket���� ������ �����Ѵ�.
				// 2.3.3.4.1 ���� �ŷ��� ������ ���� �����Ѵ�.
				nVolumeWidth = pDC->GetTextExtent( strMaxVolume).cx + 4;			// �¿� ���� ���� and ��輱 1 Pixel �߰�
				// 2.3.3.4.2 Bracket�� ������ ���� �����Ѵ�.
				nBracketCntWidth = pDC->GetTextExtent( strMaxBracketCnt).cx + 4;	// �¿� ���� ���� and ��輱 1 Pixel �߰�
			}
			// 2.3.4 Text�� ����� ��Ȳ�� ���� �ʴ� ���, Text ��� ������ Clear��Ų��.
			else
			{
				nVolumeWidth = 0;
				nBracketCntWidth = 0;
			}
		}
		// 2.3.5 ������ �κ� ���������� ��ü ������ ũ�⸦ �����Ѵ�.
		nRequestRegionX = ( nBracketWidth + nVolumeWidth + nBracketCntWidth + nBracketWidth * nMaxBracketCountInPacket + nBracketWidth)
			* nMProfileCount;
	}
	// (2004.02.05, ��¿�) �ŷ����� Bracket�� ������ ���� Bracket�� ũ�⺸�� ���� �ʵ��� �Ѵ�.
	//		���� �ŷ����� Bracket�� ������ �����Ͽ� Text ��½� ��¿��� �߾� ���ĵǵ��� �Ѵ�. (Text �߾����� ����)
	int nRealVolumeWidth		= nVolumeWidth;
	int nRealBracketCntWidth	= nBracketCntWidth;
	// 2.4 ������ ������ ���� ���� ������, �����Ͽ� Bracket�� ���� �߰� �����Ѵ�.
	//		(�����Ǵ� ������ Bracket ������ ũ�� Bracket�� Width�� �����Ѵ�. BracketWidth�� �¿��� ����ũ�⿡�� ������ ��ġ�� ���� �����Ѵ�.)
	if( nRequestRegionX < graphRegion.Width())
	{
		// (2004.02.05, ��¿�) �ŷ����� Bracket�� ������ ���� Bracket�� ũ�⺸�� ���� �ʵ��� �Ѵ�.
		//		�۰ԵǴ� ����, ���������� �䱸�Ǵ� ��ü ������ ������ ��� ������ ��,
		//		���� ������ Bracket���� �����Ͽ� Bracket�� �ø��� ��쿡 �ش��Ѵ�.
		//		����, Bracket�� �ø��� �������� �ŷ����� Bracket�� ������ �ø������Ѵ�.
		int nAddWidth = ( ( graphRegion.Width() - nRequestRegionX) / nMProfileCount)
			/ ( nMaxBracketCountInPacket + 2 + ( bWithText ? 2 : 0));	// �¿� ����ܿ� �ŷ����� Bracket�� ������ �����Ѵ�.
		// 2.4.1 BracketWidth�� �����Ѵ�.
		nBracketWidth += nAddWidth;
		// 2.4.2 �ŷ����� Bracket�� ������ �����Ѵ�.
		if( bWithText)
		{
			nVolumeWidth		+= nAddWidth;
			nBracketCntWidth	+= nAddWidth;
		}
	}

	// 3. �� CMProfile Chart�� �ش� ������ Drawing ó���Ѵ�.
	// 3.0 DC�� �⺻ ������ ó���Ͽ� ���� �̿���� �Ѵ�.
	int			nOldBkMode		= pDC->SetBkMode( TRANSPARENT);
	COLORREF	clrOldTextColor	= pDC->GetTextColor();
	CBrush		*pOldBrush		= pDC->GetCurrentBrush();
	CPen		*pOldPen		= pDC->GetCurrentPen();

	// 3.1 CMProfile Chart�� ó���� �ʿ��� �⺻���� ���Ͽ� �����Ѵ�. ���� �����ȴ�.
	// 3.1.0 Start/End Bracket Color�� ���Ͽ� �����Ѵ�. ���� �����ȴ�.
	const DWORD dwStartColor	= ( DWORD)graphColor.GetColor2();
	const DWORD dwEndColor		= ( DWORD)graphColor.GetColor1();
	// 3.1.1 Volume�̳� BracketCount�� ��¿� �̿��� Text Color�� �����Ѵ�.
	const COLORREF clrMaxTextColor		= graphColor.GetColor2();
	const COLORREF clrNormalTextColor	= graphColor.GetColor5();
	// 3.1.2 Bracket�� ����ϱ� ���Ͽ� �ʿ��� ������ �����Ѵ�.
	const COLORREF clrFirstBracketColor	= graphColor.GetColor3();
	const COLORREF clrLastBracketColor	= graphColor.GetColor4();
	const COLORREF clrBracketTextColor	= graphColor.GetColor6();
	// 3.1.2 (2004.01.27, ��¿�) Bracket �ణ�� ��ƴ�� ������ ����� ���Ͽ� ���� ������ ó���Ѵ�.
	//		(��ǥ Scale�� Min/Max�� X100���� ���Ͽ� �����Ѵ�.)
	const int nViewMinX100	= int( dViewMin * 100);
	const int nViewMaxX100	= int( dViewMax * 100);

	// 3.1.3 MProfile Chart�� ���� �����ϰ�, ���� Drawing�� �̿�� ���� Left�� �����Ѵ�.
	// (2004.02.03, ��¿�) MP Chart�� Width�� ���� RequestRegionX�� ������ ������,
	//		�Ʒ��� MP Profile Width�� �� MP Chart�� �׸��� ���� ���� Left �̵� ��������,
	//		�� �ʿ��� ����ŭ �̵����� �ʰ�, MP Chart�� ȭ�鿡 �˸°� �����ǵ��� (X Scale�� �µ���) ��ü ȭ���� ������ ��ġ�� �Ѵ�.
	int nMProfileWidth			= graphRegion.Width() / nMProfileCount;	// nBracketWidth + nVolumeWidth + nBracketCntWidth + nBracketWidth * nMaxBracketCountInPacket + nBracketWidth;
	int nMProfileLeft			= graphRegion.left;
	nMProfileLeft				+= nBracketWidth;	// ���� ������ �̸� ������� �����Ѵ�.
	nMProfileLeft				-= nMProfileWidth;	// Loop���� �ٷ� �����ǹǷ� �̸� ���Ͽ� ���´�.

	// 3.2 �� CMProfile Chart�� �ش� ������ Drawing ó���Ѵ�.
	POSITION posMProfile = pMProfileList->GetHeadPosition();
	while( posMProfile)
	{
		CMProfile* pMarketProfile = pMProfileList->GetNext( posMProfile);

		// 3.3 ���� CMPorfile Chart���� ��ϵ� Bracket Group�� CMPItem�� �ִ��� Ȯ���Ѵ�.
		CList < CMPItem*, CMPItem*> *pMPItemList = pMarketProfile->GetMPItemList();
		POSITION posMPItem = pMPItemList->GetHeadPosition();
		if( !posMPItem) continue;

		// 3.4 �� CMProfile Chart�� Drawing�ϱ� ���� �ʿ��� CMProfile�� �⺻������ ���Ͽ� �����Ѵ�.
		int		nTotalBracketCount			= pMarketProfile->GetTotalBracketCount();
		int		nMaxBracketCountInMProfile	= pMarketProfile->GetMaxBracketCount();
		DWORD	dwMaxVolume					= pMarketProfile->GetMaxVolume();
		nMProfileLeft						+= nMProfileWidth;

		// 3.5 �ŷ����� Bracket Count ��� ������ Box�� �׸���.
		if(	bWithText)
		{
			// 3.5.1 ���ݴ��� ���ָ� ���Ѵ�.
			int nMinX100, nMaxX100;
			if( !pMarketProfile->GetPriceMinMaxX100( nMinX100, nMaxX100)) return false;
			// 3.5.2 MProfile Chart�� Screen ���� ������ ���Ѵ�.
			// (2004.01.27, ��¿�) Bracket �ణ�� ��ƴ�� ������ ����� ���Ͽ� ���� ������ ó���Ѵ�.
			int nMProfileTop	= graphRegion.bottom - int( graphRegion.Height() * ( nMaxX100 + nPriceUnitX100	- nViewMinX100) / double( nViewMaxX100 - nViewMinX100));
			int nMProfileBottom	= graphRegion.bottom - int( graphRegion.Height() * ( nMinX100					- nViewMinX100) / double( nViewMaxX100 - nViewMinX100));
			// 3.5.3 �ŷ����� Bracket Count ��� ������ Box�� �׸���.
			pDC->SelectObject( GetStockObject( WHITE_BRUSH));
			pDC->SelectObject( GetStockObject( BLACK_PEN));
			pDC->Rectangle( &CRect( nMProfileLeft, nMProfileTop, nMProfileLeft + nVolumeWidth + nBracketCntWidth, nMProfileBottom));
			pDC->MoveTo( nMProfileLeft + nVolumeWidth, nMProfileTop);
			pDC->LineTo( nMProfileLeft + nVolumeWidth, nMProfileBottom);
		}

		// 3.6 �� CMProfile�� ���ݴ� ��(CMPItem)���� Drawing ó���Ѵ�.
		while( posMPItem)
		{
			CMPItem *pMPItem = pMPItemList->GetNext( posMPItem);

			// 3.7 �� ���ݴ� ��(CMPItem)�� Drawing ����(Top,Bottom)�� �����Ѵ�.
			// (2004.01.27, ��¿�) Bracket �ణ�� ��ƴ�� ������ ����� ���Ͽ� ���� ������ ó���Ѵ�.
			int nMPItemTop		= graphRegion.bottom - int( graphRegion.Height() * ( pMPItem->GetPriceX100() + nPriceUnitX100	- nViewMinX100) / double( nViewMaxX100 - nViewMinX100));
			int nMPItemBottom	= graphRegion.bottom - int( graphRegion.Height() * ( pMPItem->GetPriceX100()					- nViewMinX100) / double( nViewMaxX100 - nViewMinX100));

			// 3.8 ���� �ŷ����� Bracket���� ��½�Ų��. (���� CMProfile Chart�� 2�� �̳��� ���)
			// (2004.02.05, ��¿�) �ŷ����� Bracket�� ������ ���� Bracket�� ũ�⺸�� ���� �ʵ��� �Ѵ�.
			//		���� �ŷ����� Bracket�� ������ �����Ͽ� Text ��½� ��¿��� �߾� ���ĵǵ��� �Ѵ�. (Text �߾����� ����)
			if(	bWithText)
			{
				// 3.8.0 �ŷ����� Bracket ���� ���Ѵ�.
				int		nBracketCount	= pMPItem->GetBracketList()->GetCount();
				DWORD	dwVolume		= pMPItem->GetVolume();

				// 3.8.1 �ŷ����� Bracket ���� TEXT�� ���Ѵ�.
				CString strVolume, strBracketCount;
				strBracketCount.Format( "%d", nBracketCount);
				strVolume.Format( "%lu", dwVolume);

				// 3.8.2 �ŷ����� ���� ��½�Ų��.
				// 3.8.2.1 ���� �ŷ����� ������ �����Ѵ�.
				if( dwVolume == dwMaxVolume) pDC->SetTextColor( clrMaxTextColor);
				else pDC->SetTextColor( clrNormalTextColor);
				// 3.8.2.2 Text�� ����Ѵ�.
				CRect rctVolumeRect( nMProfileLeft, nMPItemTop, nMProfileLeft + nVolumeWidth - 1, nMPItemBottom);
				rctVolumeRect.DeflateRect( ( rctVolumeRect.Width() - nRealVolumeWidth) / 2, 0);
				pDC->DrawText( strVolume, rctVolumeRect, DT_SINGLELINE | DT_NOCLIP | DT_RIGHT | DT_VCENTER);

				// 3.8.3 Bracket���� ��½�Ų��.
				// 3.8.3.1 Bracket���� ��� ������ �����Ѵ�.
				if( nBracketCount == nMaxBracketCountInMProfile) pDC->SetTextColor( clrMaxTextColor);
				else pDC->SetTextColor( clrNormalTextColor);
				// 3.8.3.2 Text�� ����Ѵ�. (Why Right - 2 ? 'Rectangle()'�� Drawing�� Border�� ������ �����Ͽ� 2 Pixel)
				CRect rctBracketCntRect( nMProfileLeft + nVolumeWidth, nMPItemTop, nMProfileLeft + nVolumeWidth + nBracketCntWidth - 2, nMPItemBottom);
				rctBracketCntRect.DeflateRect( ( rctBracketCntRect.Width() - nRealBracketCntWidth) / 2, 0);
				pDC->DrawText( strBracketCount, rctBracketCntRect, DT_SINGLELINE | DT_NOCLIP | DT_RIGHT | DT_VCENTER);
			}

			// 3.9 �� ���ݴ� �ະ�� Bracket�� ���ó���Ѵ�.
			// 3.9.0 �� Bracket�� ���� ���� ��ǥ�� �����Ͽ� �����Ѵ�.
			int	 nMProfileBracketLeft	= nMProfileLeft + nVolumeWidth + nBracketCntWidth;
			nMProfileBracketLeft		-= nBracketWidth;	// Loop���� �ٷ� �����ǹǷ� �̸� ���Ͽ� ���´�.
			// 3.9.1 �� ���ݴ� �ະ�� Bracket�� ���ó���Ѵ�.
			CList < CBracket *, CBracket *> *pBracketList = pMPItem->GetBracketList();
			POSITION posBracket = pBracketList->GetHeadPosition();
			//	(2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
			//		���⼭ �ش� Bracket Row�� ���� Row���� Ȯ���Ѵ�. (MP Chart�� ������ MP Chart�� �����Ѵ�.)
			BOOL bLastPriceBracketRow = ( pMPItem->GetPriceX100() == pMarketProfile->GetLastPriceX100());
			while( posBracket)
			{
				CBracket *pBracket = pBracketList->GetNext( posBracket);

				// 3.10 ���� Bracket�� Left ��ǥ�� ������Ų��.
				nMProfileBracketLeft += nBracketWidth;

				// 3.11 Bracket�� Color Box�� Drawing�Ѵ�.
				int nBracketIndex = pBracket->GetIndex();
				CRect rctBracket( nMProfileBracketLeft, nMPItemTop, nMProfileBracketLeft + nBracketWidth, nMPItemBottom);
				//	(2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
				//		����, Index�� ������� ���� ������ �ִ� Bracket���� ������ Bracket������ Ȯ���Ͽ��� �Ѵ�.
				//		�� ���� ������ ���� �����̳� �߰� Gradation ���󺸴� �켱�Ѵ�.
				if( bLastPriceBracketRow && !posBracket)
					pDC->FillSolidRect( rctBracket, clrLastBracketColor);
				else
				{
					if( nBracketIndex == 1)
						pDC->FillSolidRect( rctBracket, clrFirstBracketColor);
					else
						pDC->FillSolidRect( rctBracket,
							RGB(	GetRValue( dwStartColor) + ( GetRValue( dwEndColor) - GetRValue( dwStartColor)) / ( double)nTotalBracketCount * nBracketIndex,
									GetGValue( dwStartColor) + ( GetGValue( dwEndColor) - GetGValue( dwStartColor)) / ( double)nTotalBracketCount * nBracketIndex,
									GetBValue( dwStartColor) + ( GetBValue( dwEndColor) - GetBValue( dwStartColor)) / ( double)nTotalBracketCount * nBracketIndex));
				}

				// 3.12 Bracket���� Text�� ����Ѵ�.
				if(	bWithText)
				{
					// (2004.02.24, ��¿�) ���� Bracket�Ӹ� �ƴ϶�, �� Bracket�� ���ڻ��� ���������� �Ѵ�.
					if( nBracketIndex == 1 || bLastPriceBracketRow && !posBracket) pDC->SetTextColor( clrNormalTextColor);
					else pDC->SetTextColor( clrBracketTextColor);
					pDC->DrawText( pBracket->GetItem(), 1, rctBracket, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
				}
			}
		}
	}

	// 3.0 DC�� �⺻ ������ ó���Ͽ� ���� �̿���� �Ѵ�.
	pDC->SelectObject( pOldBrush);
	pDC->SelectObject( pOldPen);
	pDC->SetTextColor( clrOldTextColor);
	pDC->SetBkMode( nOldBkMode);
	pDC->SelectObject( pOldfont);
	fFont.DeleteObject();

	return true;
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.02.05
//
//	�м�	:	���� ���� P&F Chart (�ɼ� �ƴ�. �������� ȣ�� ���� �������� Hard Codeing �Ǿ� ����.)
//				1. �⺻����
//					1. ���� ���� P&F�� t�к� ���� Ȥ�� Tick�� Price�� �������� nȣ�� ������ ��ȯ���̴�.
//						(�Ｑ ��ȯ���� ����)
//					2. ����� 'X', �϶��� 'O'ǥ�� �Ͽ� MarkLine�� �����Ѵ�.
//					3. ����� ������ ���� ��/�� ������ nȣ�� ���� �����Ǿ�߸� ���� ��ȯ�� �����ϰ�,
//						��ȯ ���� ���������� Marking�� ó���ϰ� �ȴ�.
//					4. Mark�� �׻� ȣ������ (0.05)���� ǥ���Ѵ�.
//					5. ���ڰ� ����Ǵ� ���, �ش� Mark���� ������ �����Ѵ�. (������ û���� �ݺ��ϸ� ��ȯ�Ѵ�.)
//				2. ���� ����
//					1. ��� Packet
//						1. Scale Packet		: Drawing ó���� Y Scale�� ���߼��� ���� �޺��ϴ� ��츦 �����ϱ� ���Ͽ�,
//												Drawing�� ���� Min/Max Data�� �����Ͽ�, �������� ��Ҹ��� �����ϵ��� �����ϴ� Packet�̴�.
//											  Drawing ó������ �ϰ������� ó���� �����ϵ��� �׻� �� ó�� �߰��Ѵ�.
//											  Drawing�� �ϰ������� �����Ѵ�.
//								�ð�		: ��ü ��� Data�� �ּҰ�
//								����		: ��ü ��� Data�� �ִ밪
//								�����ڰ�	: �̿� ����
//						2. �߼� Packet Data	: Server�� ������ �߼� Packet�̴�.
//								�ð�		: �߼��� ���� �����̴�. ���� �߼��� �������� 0.05�� ũ�ų� (����� ���) �۴� (�϶��� ���)
//								����		: �߼��� �� �����̴�. �ð��� ������ ����߼����� �϶��߼����� �ĺ��Ѵ�.
//								�����ڰ�	: �߼� �����߿� �߻��� �����ڰ���, ������ ���ڰ� ����Ǵ� ����� ������ ���� �ð��� ��ϵȴ�.
//												�߼� �����߿� �߻��� ���� �ð��� ���� ���� 0���� �����ȴ�.
//												Ư�� ��ȸ�������� ���� �ð��� ���� �߼��� �������� �����Ƿ� ���� 0���� �����Ѵ�.
//						3. ���߼� Packet Data	: Server���� �����ִ� Packet�� �ƴ�, Real�� �к� ó���� Tic�� ���� �����Ǵ� �߼� Packet�̴�.
//								[���߼�]	: ���߼��� �к��� ���� ������ Ȯ������ �ʴ� ��Ȳ������ �ӽ� �߼��� ��Ÿ���� Packet����,
//													Server�� �ƴ�, ��ǥ ����� �� 2���� Packet�� �������� �����Ǹ�,
//													Tic�� ���� �߼� �����̳� Ȯ���� ��Ÿ���� Packet���� �׻� ��ϵȴ�.
//											  ���߼��� ��ǥ ��� Routine�� �ϰ����� ���Ͽ�, Real �Ӹ� �ƴ϶� TR�� ��쿡�� �߰��Ѵ�.
//											  ���� ���߼��� ��½� �߼�Ȯ���� ���, ���� �߼��� �����Ͽ� Ȯ������ ���ó���ϰ�,
//													������ ��� �� �߼����� Ȯ���Ͽ� �������� ó���Ѵ�.
//								�ð�		: Tic�� ���� �ӽ÷� ������ �߼��� �ð��̴�.
//								����		: Tic�� ���� �ӽ÷� ������ �߼��� �����̴�.
//								�����ڰ�	: ���߼��� �����ϴ� ������ �����߼��� ��¥�� �ٸ� ��� ���� �������� �߼��� �����̴�.
//
//	����	:	(2004.02.05, ��¿�) �Ⱓ �� ��ĭ ���� ũ��� ��ȯ ���� ĭ������ ����� �ʿ䰡 ���� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
bool COptionDraw::DrawFuOpPAndF(CDC* pDC, const CRect& graphRegion, const CDisplayAttributes& displayAttributes,/*CIndicatorInfo* pIndicatorInfo,*/ 
		const CGraphColor& graphColor, const double& dViewMin, const double& dViewMax,
		const CList<CPacket*, CPacket*> &packetList)
{
	// 1. ���� P&F ��ǥ Packet�� Data List�� ���Ѵ�.
	// 1.1 Packet�� ������ Ȯ���Ѵ�.
	if( packetList.GetCount() < 3) return false;
	// 1.2 Packet Object Pointer�� ���Ѵ�.
	POSITION posPnF = packetList.GetHeadPosition();
	CList< double, double> *pMLStartPriceList	= packetList.GetNext( posPnF)->GetnumericDataList();	// MarkLine�� �ð�
	CList< double, double> *pMLEndPriceList		= packetList.GetNext( posPnF)->GetnumericDataList();	// MarkLine�� ����
	CList< double, double> *pMLNewDatePriceList	= packetList.GetNext( posPnF)->GetnumericDataList();	// MarkLine�� �����ڰ�
	if( !pMLStartPriceList || !pMLEndPriceList || !pMLNewDatePriceList) return false;

	// 2. Drawing�� �ʿ��� ȯ�� ���� �����Ѵ�.
	// 2.1 ���� MarkLine�� ������ Ȯ���Ѵ�.
	//	(2004.02.25, ��¿�) MarkLine���� Scale�� ���߼� Packet�� �׻� ���ԵǾ� �ִ�.
	//		�׷��� MarkLine�� ���� �����ϱ� ���Ͽ��� ���߼����� �ϳ� �� ���� �����Ѵ�.
	//		(���߼��� ���� �޺��� ���ֱ� �����̴�.)
	int nMLCount = pMLStartPriceList->GetCount() - 1;
	// 2.2 Draw ������ Min/Max Price ���� �����Ѵ�.
	int nViewMinX100 = dViewMin * 100;
	int nViewMaxX100 = dViewMax * 100;

	// 3. Mark�� ũ�⸦ �����Ѵ�.
	// 3.1 P&F Chart�� Mark 1�� (1ȣ��)�� ���̸� ���Ѵ�.
	int nMarkHeight = int( ( graphRegion.Height() * 5) / double( nViewMaxX100 - nViewMinX100));
	if( nMarkHeight == 0) nMarkHeight = 1;
	// 3.2 Mark�� ���� ���Ѵ�.
	// 3.2.1 ���� ���̷� �⺻���� �����Ѵ�.
	int nMarkWidth = nMarkHeight;
	// 3.2.2 �⺻ ������ ��½� ȭ���� ����� ��츦 Ȯ���Ͽ� �����Ѵ�.
	if( graphRegion.Width() < nMarkWidth * nMLCount)
	{
		nMarkWidth = graphRegion.Width() / nMLCount;
		if( nMarkWidth == 0) nMarkWidth = 1;
	}

	// 4. Drawing�� ���Ͽ� DC ������ ó���Ѵ�.
	// 4.1 Drawing�� �ʿ��� Pen�� �����Ѵ�.
	CPen pnPen1(	PS_SOLID, 1, graphColor.GetColor1());
	CPen pnPen2(	PS_SOLID, 1, graphColor.GetColor3());
	// 4.2 ù Marking ������ �����Ѵ�.
	CPen* pOldPen = ( CPen *)pDC->SelectObject( &pnPen1);
	// 4.3 NULL Brush�� �����Ѵ�.
	CBrush *pOldBrush = ( CBrush *)pDC->SelectStockObject( NULL_BRUSH);

	// 5. Mark Line�� Iterating�ϸ鼭 ����Ѵ�.
	// 5.0 �ݸ� ó���� �ʿ��� ������ �����Ѵ�.
	BOOL	bUpMark;
	int		nMarkLeft, nMarkTop, nMarkRight, nMarkBottom;
	int		nDrawPriceX100, nStartPriceX100, nEndPriceX100, nNewDataPriceX100;
	int		nMarkDirectionPriceX100, nUpLimitPriceX100, nDownLimitPriceX100;
	// 5.1 Data List�� Head Position�� �����Ѵ�.
	int			nMLIndex = 0;
	POSITION	posStartPrice	= pMLStartPriceList->GetHeadPosition();
	POSITION	posEndPrice		= pMLEndPriceList->GetHeadPosition();
	POSITION	posNewDatePrice	= pMLNewDatePriceList->GetHeadPosition();
	// (2004.02.25, ��¿�) Scale Packet�� �����Ѵ�. (���� �ִ��� Ȯ���ϰ�)
	if( posStartPrice && posEndPrice && posNewDatePrice)
	{
		pMLStartPriceList->GetNext(		posStartPrice	);
		pMLEndPriceList->GetNext(		posEndPrice		);
		pMLNewDatePriceList->GetNext(	posNewDatePrice	);
	}
	while( posStartPrice && posEndPrice && posNewDatePrice)
	{
		// 5.2 ���� �� MarkLine�� �ð�/����/�����ڰ��� ��ȸ�Ѵ�.
		nStartPriceX100		= int( pMLStartPriceList->GetNext(		posStartPrice	) * 100);
		nEndPriceX100		= int( pMLEndPriceList->GetNext(		posEndPrice		) * 100);
		nNewDataPriceX100	= int( pMLNewDatePriceList->GetNext(	posNewDatePrice	) * 100);

		//	(2004.02.25, ��¿�) ������ MarkLine�� ���߼��� Ȯ�� ���߼��� ���,
		//		���� ���߼��� �̾ Drawing�� ó���ϵ��� Ȯ���Ѵ�.
		//		�� MarkLine Index�� ���ҽ��� ���� �߼��� �����Ͽ� �׸���.
		//		Ư�� ���߼� �ϳ��� �ִ� ��쿡 ���������� �ʵ��� �����Ѵ�.
		// (2004.02.25, ��¿�) ���� MarkLine�� �ð��� ���� ���� ���̸�,
		//			���� ���߼� Packet�̰ų� ���� ���߼� Packet�̴�.
		//		����, �̰�� �߼� ������ �ٲ��� �ʴ´�.
		if( nMLIndex && nMLIndex == nMLCount - 1
			&& ( ( bUpMark && nStartPriceX100 <= nEndPriceX100)
				|| ( !bUpMark && nEndPriceX100 <= nStartPriceX100)))	nMLIndex--;

		// 5.3 Mark Line�� Left ��ǥ�� Right ��ǥ�� �����Ѵ�.
		nMarkLeft		= graphRegion.left + nMarkWidth * nMLIndex;
		nMarkRight		= nMarkLeft + nMarkWidth;

		// 5.4 ��°� �϶��� ���� �����Ͽ� Drawing�� ó���� �� �ֵ��� Flag�� �����ϰ�,
		//		�Ʒ��������� ���� Drawing�� �� �ֵ��� Mark Drawing Direction Price��
		//			Up/Down Limit Price�� �����Ѵ�.
		// (2004.02.25, ��¿�) ���� MarkLine�� �ð��� ���� ���� ���̸�,
		//			���� ���߼� Packet�̰ų� ���� ���߼� Packet�̴�.
		//		����, �̰�� �߼� ������ �ٲ��� �ʴ´�.
		if( nStartPriceX100 != nEndPriceX100) bUpMark = ( nStartPriceX100 < nEndPriceX100);
		if( bUpMark)
		{
			nMarkDirectionPriceX100 = 5;
			nUpLimitPriceX100		= nEndPriceX100;
			nDownLimitPriceX100		= nStartPriceX100;
		}
		else
		{
			nMarkDirectionPriceX100 = -5;
			nUpLimitPriceX100		= nStartPriceX100;
			nDownLimitPriceX100		= nEndPriceX100;
		}

		// 5.5 Mark�� �ݺ� ����Ͽ� MarkLine�� Draw ó���Ѵ�.
		for( nDrawPriceX100 = nStartPriceX100; nDownLimitPriceX100 <= nDrawPriceX100 && nDrawPriceX100 <= nUpLimitPriceX100; nDrawPriceX100 += nMarkDirectionPriceX100)
		{
			// 5.5.1 �ش� ������ ��ǥ�� �����Ѵ�.
			//	(Ư�� Mark�� ���� Scale ǥ���� �߰��� ��ġ�� �� �ֵ��� 0.025 ȣ��������ŭ ���� �����Ѵ�.
			nMarkBottom = graphRegion.bottom - int( ( graphRegion.Height() * ( nDrawPriceX100 * 10 - 25  - nViewMinX100 * 10)) / double( nViewMaxX100 * 10 - nViewMinX100 * 10));
			nMarkTop	= nMarkBottom - nMarkHeight;

			// 5.5.2 ���ڰ� ����� �����̸�, ������ �����Ѵ�.
			if( nDrawPriceX100 == nNewDataPriceX100)
			{
				if( pDC->GetCurrentPen() == &pnPen1) pDC->SelectObject( &pnPen2);
				else pDC->SelectObject( &pnPen1);
			}

			// 5.5.3 Mark�� ����Ѵ�.
			// 5.5.3.1 ��� Mark�� ����Ѵ�. (X Marking)
			//	(����, LineTo�� ������ ���� ������� �ʴ� ���� �����Ͽ� Start ��ǥ�� �׷����� �ʵ��� �����Ѵ�.)
			if( bUpMark)
			{
				pDC->MoveTo( nMarkLeft + 1,	nMarkBottom - 1);
				pDC->LineTo( nMarkRight,	nMarkTop);
				pDC->MoveTo( nMarkLeft + 1,	nMarkTop + 1);
				pDC->LineTo( nMarkRight,	nMarkBottom);
			}
			// 5.4.2.2 �϶� Mark�� ����Ѵ�. (O Marking)
			else pDC->Ellipse( nMarkLeft, nMarkTop, nMarkRight, nMarkBottom);
		}

		// 5.5 ���� Mark Line�� ó���� ���Ͽ� Index�� ������Ų��.
		nMLIndex++;
	}

	// 6. Drawing�� ���Ͽ� ������ DC�� �����Ѵ�.
	pDC->SelectObject( pOldPen);
	pDC->SelectObject(pOldBrush);
	pnPen1.DeleteObject();
	pnPen2.DeleteObject();

	return true;
}
