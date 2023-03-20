// GraphFrameDrawer.h: interface for the CGraphFrameDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHFRAMEDRAWER_H__CF6747E7_2382_4251_A6F7_B79BFB086C53__INCLUDED_)
#define AFX_GRAPHFRAMEDRAWER_H__CF6747E7_2382_4251_A6F7_B79BFB086C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BlockBaseData.h"

class CGraphRegionColor;
class CGraphFrameDrawerImplementation;

class AFX_EXT_CLASS CGraphFrameDrawer  
{
public:
	CGraphFrameDrawer();
	virtual ~CGraphFrameDrawer();

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
	CGraphFrameDrawerImplementation* m_pGraphFrameDrawerImpl;
};

#endif // !defined(AFX_GRAPHFRAMEDRAWER_H__CF6747E7_2382_4251_A6F7_B79BFB086C53__INCLUDED_)
