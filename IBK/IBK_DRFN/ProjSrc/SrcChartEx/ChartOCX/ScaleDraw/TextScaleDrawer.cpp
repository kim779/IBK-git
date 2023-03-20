// TextScaleDrawer.cpp: implementation of the CTextScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextScaleDrawer.h"

//#include "Conversion.h"
#include "DataMath.h"
#include "BaseData.h"
#include "ScaleRegion.h"

#include "../Include_Chart/Dll_Load/XScaleManager.h"		// CXScaleManager

// packet.dll
#include "PacketBase.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CTextHorzScaleDrawer

CTextHorzScaleDrawer::CTextHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CHorzScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// public ===============================================================================
// ����
void CTextHorzScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CRect& scaleRegion, const CHorizontalScale& horzScale)
{
	// �׸��� ���� �⺻ data�� �����´�.
	double onePtWidth = 0.0;
	int posPt_y = 0;
	CHorzScaleDrawer::GetDrawingEnvironment(scaleRegion, drawingHorzScaleData.GetDisplayAttributes(), pDC->GetTextExtent("0").cy, onePtWidth, posPt_y);

	CList<CString, CString>* pStringData = drawingHorzScaleData.GetPacket()->GetstrDataList();
	if(pStringData == NULL)
		return;
	POSITION pos = pStringData->FindIndex(drawingHorzScaleData.GetDisplayAttributes().GetDataStartInBlock());
	if(pos == NULL)
		return;

	int pre_textEndPos = 0, nowCount = 0;
	for(int count = drawingHorzScaleData.GetDisplayAttributes().GetDataStartInBlock(); count <= drawingHorzScaleData.GetDisplayAttributes().GetDataEndInBlock(); count++, nowCount++){
		CString strData;
		if(!GetChangeTextData(horzScale.GetScaleCompart().GetScaleTextType(), pStringData, pos, strData))
			return;

		int posPt_x = CHorzScaleDrawer::GetHorzScalePosition_x(nowCount, scaleRegion.left, onePtWidth);
		if(posPt_x > scaleRegion.right)
			continue;

		CSize dataSize = pDC->GetTextExtent(strData);
		if(pre_textEndPos < posPt_x){
			if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::VERT_TEXT_COMPART)
			{
				DrawTextAndCompartLine_VERT(pDC, scaleRegion, CPoint(posPt_x, posPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData.Left(2)), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
			else if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::CENTER_HORZ_TEXT_COMPART)
			{
				//sy 2004.11.11.
				DrawTextAndCompartLine_CenterHorz(pDC, scaleRegion, CPoint(posPt_x, posPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
			else
			{
				DrawTextAndCompartLine(pDC, scaleRegion, CPoint(posPt_x, posPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
		}
	}
}

// ����
void CTextHorzScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale)
{
	// �׸��� ���� �⺻ data�� �����´�.
	double onePtWidth = 0.0;
	int posBottomPt_y = 0, posTopPt_y = 0;
	CHorzScaleDrawer::GetDrawingEnvironment(horzScaleRegion.GetBottomScaleRegion(), horzScaleRegion.GetTopScaleRegion(), 
		drawingHorzScaleData.GetDisplayAttributes(), pDC->GetTextExtent("0").cy, onePtWidth, posBottomPt_y, posTopPt_y);

	CList<CString, CString>* pStringData = drawingHorzScaleData.GetPacket()->GetstrDataList();
	if(pStringData == NULL)
		return;
	POSITION pos = pStringData->FindIndex(drawingHorzScaleData.GetDisplayAttributes().GetDataStartInBlock());
	if(pos == NULL)
		return;

	int pre_textEndPos = 0, nowCount = 0;
	for(int count = drawingHorzScaleData.GetDisplayAttributes().GetDataStartInBlock(); count <= drawingHorzScaleData.GetDisplayAttributes().GetDataEndInBlock(); count++, nowCount++){
		CString strData;
		if(!GetChangeTextData(horzScale.GetScaleCompart().GetScaleTextType(), pStringData, pos, strData))
			return;

		int posPt_x = CHorzScaleDrawer::GetHorzScalePosition_x(nowCount, horzScaleRegion.GetBottomScaleRegion().left, onePtWidth);
		if(posPt_x > horzScaleRegion.GetBottomScaleRegion().right)
			continue;

		CSize dataSize = pDC->GetTextExtent(strData);
		// pre text�� ��ĥ ��쿣 �׸��� �ʴ´�.
		if(pre_textEndPos < posPt_x){
			// ������
			if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::VERT_TEXT_COMPART)
			{
				DrawTextAndCompartLine_VERT(pDC, horzScaleRegion.GetBottomScaleRegion(), CPoint(posPt_x, posBottomPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData.Left(2)), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
				DrawTextAndCompartLine_VERT(pDC, horzScaleRegion.GetTopScaleRegion(), CPoint(posPt_x, posTopPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData.Left(2)), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
			//sy 2004.11.11.
			// �߾���(����)
			else if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::CENTER_HORZ_TEXT_COMPART)
			{
				DrawTextAndCompartLine_CenterHorz(pDC, horzScaleRegion.GetBottomScaleRegion(), CPoint(posPt_x, posBottomPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
				DrawTextAndCompartLine_CenterHorz(pDC, horzScaleRegion.GetTopScaleRegion(), CPoint(posPt_x, posTopPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
			// ������
			else
			{
				DrawTextAndCompartLine(pDC, horzScaleRegion.GetBottomScaleRegion(), CPoint(posPt_x, posBottomPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
				DrawTextAndCompartLine(pDC, horzScaleRegion.GetTopScaleRegion(), CPoint(posPt_x, posTopPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
		}
	}
}

// text ����
void CTextHorzScaleDrawer::DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale)
{
}

//////////////////////////////////////////////////////////////////////
// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
//////////////////////////////////////////////////////////////////////

// ����
bool CTextHorzScaleDrawer::DrawTextOrGrid( CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
{
	BOOL bReverse = pxScaleManager->GetReverse();
	
	int nDRI_Start = pxScaleManager->GetStartDrawIdx_Cur();
	int nDRI_End = pxScaleManager->GetEndDrawIdx_Cur();

	pxScaleManager->SetCurScaleInfo(nDRI_Start);

	// �׸��� ���� �⺻ data�� �����´�.
	double onePtWidth = 0.0;
	int posPt_y = 0;
	CHorzScaleDrawer::GetDrawingEnvironment(scaleRegion, drawingHorzScaleData.GetDisplayAttributes(), pDC->GetTextExtent("0").cy, onePtWidth, posPt_y);

	CList<CString, CString>* pStringData = drawingHorzScaleData.GetPacket()->GetstrDataList();
	if(pStringData == NULL)
		return false;
	POSITION pos = pStringData->FindIndex( drawingHorzScaleData.GetDisplayAttributes().GetDataStartInBlock());
	if(pos == NULL)
		return false;

	time_t timeCur;
	CXScaleManager::PAREA parea;
	int pre_textEndPos = 0, nowCount = 0;
	if( bReverse) pre_textEndPos = scaleRegion.right;
	for(int count = nDRI_Start ; count <= nDRI_End ; count++, nowCount++)
	{
		pxScaleManager->NextScaleTime(&timeCur, &parea);
		if(!parea) break;

		CString strData;
		if(!GetChangeTextData(horzScale.GetScaleCompart().GetScaleTextType(), pStringData, pos, strData))
			break;

		int posPt_x = parea->left;
		if(posPt_x > scaleRegion.right)
			continue;

		CSize dataSize = pDC->GetTextExtent(strData);
		if( !bReverse && pre_textEndPos < posPt_x || bReverse && pre_textEndPos > posPt_x )
		{
			if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::VERT_TEXT_COMPART)
			{
				DrawTextAndCompartLine_VERT(pDC, scaleRegion, CPoint(posPt_x, posPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData.Left(2)), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
			else if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::CENTER_HORZ_TEXT_COMPART)
			{
				//sy 2004.11.11.
				DrawTextAndCompartLine_CenterHorz(pDC, scaleRegion, CPoint(posPt_x, posPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
			else
			{
				DrawTextAndCompartLine(pDC, scaleRegion, CPoint(posPt_x, posPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
		}
	}

	return true;
}

// ����
bool CTextHorzScaleDrawer::DrawTextOrGrid_Both( CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
{
	BOOL bReverse = pxScaleManager->GetReverse();
	
	int nDRI_Start = pxScaleManager->GetStartDrawIdx_Cur();
	int nDRI_End = pxScaleManager->GetEndDrawIdx_Cur();

	pxScaleManager->SetCurScaleInfo(nDRI_Start);

	// �׸��� ���� �⺻ data�� �����´�.
	double onePtWidth = 0.0;
	int posBottomPt_y = 0, posTopPt_y = 0;
	CHorzScaleDrawer::GetDrawingEnvironment(horzScaleRegion.GetBottomScaleRegion(), horzScaleRegion.GetTopScaleRegion(), 
		drawingHorzScaleData.GetDisplayAttributes(), pDC->GetTextExtent("0").cy, onePtWidth, posBottomPt_y, posTopPt_y);

	CList<CString, CString>* pStringData = drawingHorzScaleData.GetPacket()->GetstrDataList();
	if(pStringData == NULL)
		return false;
	POSITION pos = pStringData->FindIndex(drawingHorzScaleData.GetDisplayAttributes().GetDataStartInBlock());
	if(pos == NULL)
		return false;

	time_t timeCur;
	CXScaleManager::PAREA parea;
	int pre_textEndPos = 0, nowCount = 0;
	if( bReverse) pre_textEndPos = horzScaleRegion.GetBottomScaleRegion().right;
	for(int count = nDRI_Start ; count <= nDRI_End ; count++, nowCount++)
	{
		pxScaleManager->NextScaleTime(&timeCur, &parea);
		if(!parea) break;

		CString strData;
		if(!GetChangeTextData(horzScale.GetScaleCompart().GetScaleTextType(), pStringData, pos, strData))
			return false;


		int posPt_x = parea->left;
		if(posPt_x > horzScaleRegion.GetBottomScaleRegion().right)
			continue;

		CSize dataSize = pDC->GetTextExtent(strData);
		// pre text�� ��ĥ ��쿣 �׸��� �ʴ´�.
		if( !bReverse && pre_textEndPos < posPt_x || bReverse && pre_textEndPos > posPt_x )
		{
			// ������
			if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::VERT_TEXT_COMPART)
			{
				DrawTextAndCompartLine_VERT(pDC, horzScaleRegion.GetBottomScaleRegion(), CPoint(posPt_x, posBottomPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData.Left(2)), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
				DrawTextAndCompartLine_VERT(pDC, horzScaleRegion.GetTopScaleRegion(), CPoint(posPt_x, posTopPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData.Left(2)), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
			//sy 2004.11.11.
			// �߾���(����)
			else if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::CENTER_HORZ_TEXT_COMPART)
			{
				DrawTextAndCompartLine_CenterHorz(pDC, horzScaleRegion.GetBottomScaleRegion(), CPoint(posPt_x, posBottomPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
				DrawTextAndCompartLine_CenterHorz(pDC, horzScaleRegion.GetTopScaleRegion(), CPoint(posPt_x, posTopPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
			// ������
			else
			{
				DrawTextAndCompartLine(pDC, horzScaleRegion.GetBottomScaleRegion(), CPoint(posPt_x, posBottomPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
				DrawTextAndCompartLine(pDC, horzScaleRegion.GetTopScaleRegion(), CPoint(posPt_x, posTopPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
			}
		}
	}

	return true;
}


// private ==============================================================================
// text�� ���м��� �׸���.
void CTextHorzScaleDrawer::DrawTextAndCompartLine(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth,
		const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos)
{
	// ���ν����� ������ ���м��� 1/3�� �׸���
	// 08.24.2001
	CDrawer::DrawLine(pDC, CPoint(nowPos.x, scaleRegion.top), 
						   CPoint(nowPos.x, scaleRegion.top + (scaleRegion.bottom - scaleRegion.top) / 3), gridColor);
	//sy 2002.8.13.
	CPoint textPosPt(GetTextPosition_x(nowPos.x, textRegionWidth, textSize.cx, false), nowPos.y);
	if(scaleRegion.Height() > textSize.cy)
		CDrawer::DrawTextOut(pDC, textPosPt, data, textColor);
	textEndPos = textPosPt.x + textSize.cx;
}

// ������ ����
// insert by  11.15.2001
void CTextHorzScaleDrawer::DrawTextAndCompartLine_VERT(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth,
		const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos)
{
	//@Solomon-091021�� �������� 5pixel�� ������Ŵ.
	CDrawer::DrawLine(pDC, CPoint(nowPos.x, scaleRegion.top), 
						   //CPoint(nowPos.x, scaleRegion.top + (scaleRegion.bottom - scaleRegion.top) / 3), gridColor);
						   CPoint(nowPos.x, scaleRegion.top + 5), gridColor);

	CPoint textPosPt(nowPos.x, nowPos.y);
	textPosPt = CPoint(GetTextPosition_x(nowPos.x, textRegionWidth, textSize.cx), nowPos.y);
	if(scaleRegion.Height() > textSize.cy)	{
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF oldTextColor = pDC->SetTextColor(textColor);

		CRect rcText = CRect(textPosPt.x, scaleRegion.top + MARGIN, textPosPt.x + textSize.cx, scaleRegion.bottom); 
		pDC->DrawText(data, data.GetLength(), rcText, DT_VCENTER|DT_CENTER) ;
		pDC->SetBkMode(oldBkMode);
		pDC->SetTextColor(oldTextColor);
	}
	// (2009/7/26 - Seung-Won, Bae) Use nowPos.x instead of textSize.x without centering effect.
	textEndPos = nowPos.x + textSize.cx;
}

//sy 2004.11.11.
// �߾��� ����(����)
void CTextHorzScaleDrawer::DrawTextAndCompartLine_CenterHorz(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth,
		const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos)
{
	CPoint textPosPt(nowPos.x, nowPos.y);
	textPosPt.x = GetTextPosition_x(nowPos.x, textRegionWidth, textSize.cx);
	if(scaleRegion.bottom > (nowPos.y + textSize.cy + MARGIN))
		textPosPt.y += MARGIN;

	// text���м� �׸���
	CHorzScaleDrawer::DrawTextCompartLine(pDC, scaleRegion, 
		CPoint(GetTextCenterPosition_x(textPosPt.x, textSize.cx), textPosPt.y), false, gridColor);

	// text �׸���
	if(scaleRegion.Height() > textSize.cy)
		CDrawer::DrawTextOut(pDC, textPosPt, data, textColor);

	textEndPos = textPosPt.x + textSize.cx;
}

bool CTextHorzScaleDrawer::GetChangeTextData(const CScaleBaseData::TEXTCOMPARTTYPE eTextCompart, CList<CString, CString>* pData, POSITION& pos, CString& strData) const
{
	if(pData == NULL || pos == NULL)
		return false;

	strData = pData->GetNext(pos);
	strData.TrimLeft(" ");
	strData.TrimRight(" ");

	// ������
	if(strData.IsEmpty() || eTextCompart != CScaleBaseData::VERT_TEXT_COMPART)
		return true;

	// ������
	int nlength = strData.GetLength();
	CString strRet = "";
	int i = 0;
	while(i < nlength){
		CString strOneChar = strData.Mid(i,1);
		// Ư�� ��ȣ
		if(strOneChar == "." || strOneChar == "," || strOneChar == "/" ||
			strOneChar == "(" || strOneChar == ")" || strOneChar == " ")			
		{
			strRet += (strOneChar + "\n");
			i++;
		}
		// ������ ����
		else if((strOneChar >= "A" && strOneChar <= "Z") || 
			(strOneChar >= "a" && strOneChar <= "z") || 
			(strOneChar >= "0" && strOneChar <= "9"))
		{
//////////////////////////////////////////////////////////////////////////
// 2006.12.20 ����ǥ�� ��� ����
// ������ ������ ��� �� �ٿ� �ΰ��� ǥ���ϴ� ���� �� �ٿ� �ϳ��� ǥ���ϱ� ���� �Ʒ��� ��ƾ ����
//			CString strNextOneChar = strData.Mid(i+1,1);
// 			if((i < strData.GetLength() - 1) &&
// 				((strNextOneChar >= "A" && strNextOneChar <= "Z") || 
// 				 (strOneChar >= "a" && strOneChar <= "z") || 
// 				 (strNextOneChar >= "0" && strNextOneChar <= "9")))
// 			{
// 				strRet += strData.Mid(i, 2) + "\n";
// 				i+=2;
// 			}
// 			else
// 			{
//				strRet += strOneChar + "\n";
//				i++;
//			}
//////////////////////////////////////////////////////////////////////////
			strRet += strOneChar + "\n";
			i++;
//////////////////////////////////////////////////////////////////////////
		}
		// ��Ÿ
		else
		{
			strRet += strData.Mid(i, 2) + "\n";
			i+=2;
		}
	}
	strData = strRet;

	return true;
}

// scale text�� �׸��� ��ġ
int CTextHorzScaleDrawer::GetTextPosition_x(const int org_x, const double& textRegionWidth, const int textWidth, const bool isCenter) const
{
	//sy 2002.8.13.
	if(!isCenter)
		return org_x +MARGIN/2;

	if(textRegionWidth > textWidth)
		return (org_x + CMath::Round((textRegionWidth - 2*MARGIN - textWidth)/2.0) +MARGIN);

	return org_x +MARGIN/2;
}

// scale text�� �߽� ��ġ
int CTextHorzScaleDrawer::GetTextCenterPosition_x(const int textPosition, const int textWidth) const
{
	//sy 2004.11.11.
	if(textWidth < 2)
		return textPosition;

	return textPosition + textWidth/2;
}


/////////////////////////////////////////////////////////////////////////////////////////
// class CTextVertScaleDrawer

CTextVertScaleDrawer::CTextVertScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CVertScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// public ===============================================================================
// ����
void CTextVertScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid(pDC, drawingVertScaleData.GetPacket(), scaleRegion.GetScaleTextRegion(), 
		CRect(0, 0, 0, 0), drawingVertScaleData.GetScaleColor().GetScaleTextColor());
}

// ����
void CTextVertScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid(pDC, drawingVertScaleData.GetPacket(), vertScaleRegion.GetLeftScaleTextRegion(), 
		vertScaleRegion.GetRightScaleTextRegion(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());
}

// private ==============================================================================
// scale �׸���
void CTextVertScaleDrawer::DrawTextOrGrid(CDC* pDC, CPacket* pPacket, const CRect& region1, const CRect& region2, const COLORREF& textColor)
{
	if(pPacket == NULL)
		return;

	double onePtWidth = CDrawer::GetOnePointWidth(region1.Height(), pPacket->GetCount());
	if(onePtWidth <= 0)
		return;

	CList<CString, CString>* pStrData = pPacket->GetstrDataList();
	if(pStrData == NULL || pStrData->GetCount() <= 0)
		return;

	int count = 0;
	POSITION pos = pStrData->GetHeadPosition();
	while(pos != NULL){
		int nowPt_y = region1.top + CMath::Round(onePtWidth *(double)count +onePtWidth/2.0);
		count++;
		CString strData = pStrData->GetNext(pos);
		if(strData.IsEmpty())
			continue;

		CDrawer::DrawTextOut(pDC, CVertScaleDrawer::GetTextPosition(region1.right, nowPt_y, pDC->GetTextExtent(strData)),
				strData, textColor);
		if(region2.IsRectEmpty() == FALSE){
			CDrawer::DrawTextOut(pDC, CVertScaleDrawer::GetTextPosition(region2.right, nowPt_y, pDC->GetTextExtent(strData)),
				strData, textColor);
		}
	}
}