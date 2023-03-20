// ScaleDrawer.h: interface for the CScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEDRAWER_H__D71B1615_8C0D_485B_811B_AD7EDF85A125__INCLUDED_)
#define AFX_SCALEDRAWER_H__D71B1615_8C0D_485B_811B_AD7EDF85A125__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CScaleDrawerImplementation;
class CDrawingHorzScaleData;
class CDrawingVertScaleData;
class CHorizontalScale;
class CHorzScaleRegion;
class CVerticalScale;
class CVertScaleRegion;

class AFX_EXT_CLASS CScaleDrawer  
{
public:
	CScaleDrawer();
	virtual ~CScaleDrawer();

	// ���� scale �׸���.
	void DrawHorzTextOrGrid(CDC* pDC, 
				const CDrawingHorzScaleData& drawingHorzScaleData,
				const CHorzScaleRegion& horzScaleRegion, // ���� scale�� ����
				const CHorizontalScale& horzScale);		 // ���� scale�� ���� ��

	// ���� scale �׸���.
	void DrawVertTextOrGrid(CDC* pDC, 
				const CDrawingVertScaleData& drawingVertScaleData,
				const CVertScaleRegion& vertScaleRegion, // ���� scale�� ����
				const CVerticalScale& vertScale);		 // ���� scale�� ���� ��

private:
	CScaleDrawerImplementation* m_pScaleDrawerImpl;
};

#endif // !defined(AFX_SCALEDRAWER_H__D71B1615_8C0D_485B_811B_AD7EDF85A125__INCLUDED_)
