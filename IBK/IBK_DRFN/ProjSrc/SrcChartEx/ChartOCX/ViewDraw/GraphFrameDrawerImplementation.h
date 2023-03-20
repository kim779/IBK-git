// GraphFrameDrawerImplementation.h: interface for the CGraphFrameDrawerImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHFRAMEDRAWERIMPLEMENTATION_H__EB0F642F_A905_4FE8_9017_21C8A5F2DB4C__INCLUDED_)
#define AFX_GRAPHFRAMEDRAWERIMPLEMENTATION_H__EB0F642F_A905_4FE8_9017_21C8A5F2DB4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BlockBaseData.h"
#include "Color.h"

class CGraphFrameDrawerImplementation  
{
public:
	void DrawGraphFrame(CDC* pDC, 
			const CRect& graphRegion,		// �׸� graph ����
			const CBlockBaseData::BLOCKCOMPART eBlockCompart, // block ����Ÿ��
			const int interBlockHalfGap,		 // block���� Gap / 2
			const int graphRegionFrameThickness, // block frame ����
			const int nGraphBackgroundGapX,	// graph ������ ����(����)
			const int nGraphBackgroundGapY,	// graph ������ ����(����)
			const CGraphRegionColor& graphRegionColor);	// Graph Region ��

	void DrawBlockCompartLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt, const COLORREF& blockCompartLineColor);

private:
	void DrawGraphBackground(CDC* pDC, const CRect& region, const COLORREF& color);
	void DrawGraphLine(CDC* pDC, CRect region, const int nGapX, const int nGapY, const COLORREF color, const int graphRegionFrameThickness);
	void DrawGraphFrame(CDC* pDC, const CRect& region, const int nFrameThickness, const COLORREF color);
	void DrawLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt);
};

#endif // !defined(AFX_GRAPHFRAMEDRAWERIMPLEMENTATION_H__EB0F642F_A905_4FE8_9017_21C8A5F2DB4C__INCLUDED_)
