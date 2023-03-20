// GraphFrameDrawer.cpp: implementation of the CGraphFrameDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphFrameDrawer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Color.h"
#include "GraphFrameDrawerImplementation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CGraphFrameDrawer

CGraphFrameDrawer::CGraphFrameDrawer():
	m_pGraphFrameDrawerImpl(NULL)
{
}

CGraphFrameDrawer::~CGraphFrameDrawer()
{
	delete m_pGraphFrameDrawerImpl;
}

void CGraphFrameDrawer::DrawGraphFrame(CDC* pDC, 
			const CRect& graphRegion,		// �׸� graph ����
			const CBlockBaseData::BLOCKCOMPART eBlockCompart, // block ����Ÿ��
			const int interBlockHalfGap,		 // block���� Gap / 2
			const int graphRegionFrameThickness, // block frame ����
			const int nGraphBackgroundGapX,	// graph ������ ����(����)
			const int nGraphBackgroundGapY,	// graph ������ ����(����)
			const CGraphRegionColor& graphRegionColor)	// Graph Region ��
{
	m_pGraphFrameDrawerImpl->DrawGraphFrame(pDC, graphRegion, eBlockCompart, interBlockHalfGap, graphRegionFrameThickness, nGraphBackgroundGapX, nGraphBackgroundGapY, graphRegionColor);
}

void CGraphFrameDrawer::DrawBlockCompartLine(CDC* pDC, const CPoint& startPt, const CPoint& endPt, const COLORREF& blockCompartLineColor)
{
	m_pGraphFrameDrawerImpl->DrawBlockCompartLine(pDC, startPt, endPt, blockCompartLineColor);
}
