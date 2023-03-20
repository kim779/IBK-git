// KTBChartDrawToImage.cpp: implementation of the DrawToImage Interface
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"		// for CAddInManager
#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for ML_LANGUAGE_RES
#include "KTBChartCtl.h"								// for CKTBChartCtrl
#include "Color.h"										// for CGraphRegionColor
#include "MainBlock.h"									// for CMainBlock
#include "XScaleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*-------------------------------------------------------------------------------
 - Function    :  OnDrawForPrint
 - Created at  :  2003-10-23   14:32
 - Parameters  :  memoryDC     - �޸�DC
 -                bufferBitmap - ������� ����.
 -                rcSource     - ������ �̹����� ũ��
 -                bScreenColor - ???
 - Return Value:  None
 - Description :  (�м�) ��Ʈ�� memoryDC�� �׸���.
 -------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnDrawForPrint(CDC& memoryDC,
								   CBitmap& bufferBitmap,
								   CRect rcSource,
								   BOOL bScreenColor)
{
	CRect rect(rcSource);
	// (2003.12.23, ��¿�) Resource Manager ������ �����Ѵ�.
	CFont* pOldFont = (CFont*) memoryDC.SelectObject( m_pRscFont);

	OnDrawStockItemCode(memoryDC, rect); // ��Ʈ������ ������ �����̸��� memoryDC�� �׸���.

	if(bScreenColor)
		OnDrawUsingScreencolor(memoryDC, bufferBitmap, rect);	// ��Ʈ�� memoryDC�� �׸�
	else
		OnDrawUsingOptimizingcolor(memoryDC, bufferBitmap, rect);	// �����ϰ� ǥ���� ������ ��Ʈ�� �׸�
	
	memoryDC.SelectObject(pOldFont);
}


/*-------------------------------------------------------------------------------
 - Function    :  OnDrawStockItemCode
 - Created at  :  2003-10-23   14:43
 - Analysis    :  ������ �����ڵ带 �׸���.
 -------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnDrawStockItemCode(CDC& memoryDC,
										CRect& rect)
{
	int nWritingItemInfoSpace = 0;
	if(!m_strCode.IsEmpty() && !m_strItemName.IsEmpty()) 
		nWritingItemInfoSpace = 20;

	//�����ڵ�, ����� �� ����
	m_pBackgroundDraw.DrawBackground(&memoryDC,
					CRect(rect.left, rect.top, rect.right, rect.top + 20),	// �׸� background ���� 
					1,			// border ����
					RGB(255,255,255),		// background ��
					RGB(255,255,255),		// block ��
					m_nBorderType);			// border Ÿ��

	if(nWritingItemInfoSpace != 0)
	{
		ML_SET_LANGUAGE_RES();
		CString strFormat;
		strFormat.LoadString( IDS_FORMAT_ITEM_CODE_AND_NAME);
		CString str;
		str.Format( strFormat, m_strCode, m_strItemName);
		memoryDC.SetBkMode(TRANSPARENT);

		SIZE size;
		::GetTextExtentPoint32(memoryDC.m_hDC, "0", 1, &size);

		int ntop = 5;
		int nleft = (rect.Width() - (str.GetLength() * size.cx)) / 2;
		int nright = nleft + (str.GetLength() * size.cx);
		int nbottom = ntop + size.cy;
		
		memoryDC.MoveTo(nleft - 2, ntop - 3);
		memoryDC.LineTo(nleft - 2, nbottom + 1);
		memoryDC.LineTo(nright + 2, nbottom + 1);
		memoryDC.LineTo(nright + 2, ntop - 3);
		memoryDC.LineTo(nleft - 2, ntop - 3); 

		memoryDC.TextOut(nleft, ntop, str);
	}

	rect.top += nWritingItemInfoSpace;	// �����ڵ�� ������� �׸� ������ ���� ���ܽ�Ű��!
}


/*-------------------------------------------------------------------------------
 - Function    :  OnDrawUsingOptimizingcolor
 - Created at  :  2003-10-23   14:39
 - Analysis    :  ��Ʈ�� ������ ��(HTS�� ǥ���� �״���� ���� �ƴ�)���� MemoryDC�� �׸���.
 -------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnDrawUsingOptimizingcolor(CDC& memoryDC,
											   CBitmap& bufferBitmap,
											   CRect rect)
{
	m_pMainBlock->SetAllBlockRegion(rect);
	m_pMainBlock->SetBackgroundColor(RGB(255,255,255));
	CGraphRegionColor BlockColor(RGB(255,255,255), RGB(255,255,255), m_clrBlockBorder);
	m_pMainBlock->SetGraphRegionColor(BlockColor);
	m_pMainBlock->SetGraphBackgroundGap(0, 0);

	DrawChart( memoryDC, memoryDC, rect);

	//�ٽ� �ǵ�����..
	m_pMainBlock->SetBackgroundColor(m_clrBackground);
	CGraphRegionColor BlockColor2(m_clrBlock, m_clrLattice, m_clrBlockBorder);
	m_pMainBlock->SetGraphRegionColor(BlockColor2);
	m_pMainBlock->SetGraphBackgroundGap(m_nHorzGap, m_nVertGap);
	// (2004.10.19, ��¿�) Block Region ������ �̿� ���� �ٷ� �����Ѵ�.
	m_pMainBlock->SetAllBlockRegion(GetMainBlockRegion());
}

//ȭ���
void CKTBChartCtrl::OnDrawUsingScreencolor(CDC& memoryDC,
										   CBitmap& bufferBitmap, // ������� ����.
									       CRect rect)
{
	// ��Ʈ�� �޸�DC�� �׸���.
	m_pMainBlock->SetAllBlockRegion(rect);

	DrawChart( memoryDC, memoryDC, rect);

	// (2004.10.19, ��¿�) Block Region ������ �̿� ���� �ٷ� �����Ѵ�.
	m_pMainBlock->SetAllBlockRegion(GetMainBlockRegion());
}
