// Drawer.cpp: implementation of the Drawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Drawer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "Conversion.h"

// packet.dll
#include "PacketBase.h"

/////////////////////////////////////////////////////////////////////////////////////////
// class CDrawer

// protected ============================================================================
// data�� count
int CDrawer::GetPacketDataCount(CPacket* pPacket) const
{
	if(pPacket != NULL)
		return pPacket->GetCount();
	return -1;
}

// data type�� �����´�.
//sy 2002.8.28.
CString CDrawer::GetPacketDataType(CPacket* pPacket) const
{
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return CString("");
	
	CString packetType = pPacket->GetType(false);
	packetType.TrimLeft(" ");
	packetType.TrimRight(" ");
	return packetType;
}

// Text packet�� count��° string data�� �����´�.
CString CDrawer::GetTextData(CPacket* pPacket, const int findIndex) const
{
	if(pPacket == NULL)
		return CString("");

	if(pPacket->GetType() == _MTEXT( C6_CHARACTER)){
		POSITION pos = pPacket->GetstrDataList()->FindIndex(findIndex);
		if(pos != NULL)
			return pPacket->GetstrDataList()->GetAt(pos);
	}
	return CString("");
}

// <�ڷ�����>�� count��° string data�� �����´�.
bool CDrawer::GetDateData(CPacket* pPacket, const int findIndex, int& data) const
{
	if(findIndex < 0 || pPacket == NULL || pPacket->GetType() == _MTEXT( C6_CHARACTER))
		return false;

	POSITION pos = pPacket->GetnumericDataList()->FindIndex(findIndex);
	if(pos != NULL){
		data = (int)pPacket->GetnumericDataList()->GetAt(pos);
		return true;
	}

	data = (int)pPacket->GetDataPastandFuture(findIndex);
	return true;

	/*// margin�� �ش��ϴ� ��¥�� ����ϴ� �κ�
	if(pPacket->GetCount() <= findIndex && pPacket->GetCount() > 0)	{
		int nPlus = findIndex - (pPacket->GetCount() - 1);
		data = CDataConversion::CalcDateFormat((int)pPacket->GetnumericDataList()->GetTail(), 
								   nPlus, pPacket->GetType(), pPacket->GetDateUnitType(), 
								   pPacket->GetTimeInterval());

		int a = pPacket->GetDataPastandFuture(findIndex);
		return true;
	}
	return false;*/
}

bool CDrawer::GetDateNoneRealData(CPacket* pPacket, const int findIndex, int& data) const
{
	// margin�� �ش��ϴ� ��¥�� ����ϴ� �κ�
	if(pPacket == NULL || findIndex <= 0 || pPacket->GetType() == _MTEXT( C6_CHARACTER))
		return false;

	if(pPacket->GetCount() > findIndex || pPacket->GetCount() <= 0)
		return false;

	data = (int)pPacket->GetDataPastandFuture(findIndex);
	return true;
}

// --------------------------------------------------------------------------------------
// scale �ѱ����� ��(onePtWidth)
double CDrawer::GetOnePointWidth(const int length, const int dataCount) const
{
	if(dataCount <= 0)
		return length;
	return (double)length / (double)dataCount;
}

// data type�� �ڸ��� �ø��� ���� �����Ѵ�.
// CVertScaleDraw���� �Űܿ� CHorzScaleDraw���� �Բ� ����ϱ� ����
// 08.23.2001
CString CDrawer::GetChangeDataType(const CString& orgDataType) const
{
	CString newDataType = orgDataType;
	if(orgDataType.IsEmpty() == TRUE)
		newDataType += "x1";

	int decimalDownCount = CDataConversion::GetDecimalDownCount(orgDataType);
	if(decimalDownCount > 0){
		bool nDecimal = true;
		for(int i = 0; i < 3 - decimalDownCount; i++){
			newDataType += '0';
			nDecimal = false;
		}
		int deleteCount = decimalDownCount;
		int insertIndex = newDataType.Find('.');
		// x 0.1 -> �Ҽ����� ���� ���
		if(!nDecimal){
			deleteCount++;
			insertIndex++;
		}

		newDataType.Delete(newDataType.Find('.') -1, deleteCount);
		if(insertIndex < newDataType.GetLength())
			newDataType.Insert(insertIndex, '.');
	}
	else
		newDataType += "000";

	return newDataType;
}

// --------------------------------------------------------------------------------------
// text�� ����Ѵ�.
void CDrawer::DrawTextOut(CDC* pDC, const CPoint& point, const CString& data, const COLORREF color)
{
	int oldBkMode = pDC->GetBkMode();
	COLORREF oldTextColor = pDC->GetTextColor();
	SetDrawTextMode(pDC, TRANSPARENT, color);

	pDC->TextOut(point.x, point.y, data);
	SetDrawTextMode(pDC, oldBkMode, oldTextColor);
}

// text�� �������ϰ� ����Ѵ�
void CDrawer::DrawTextOut(CDC* pDC, const CRect& rect/*const CPoint& point*/, const CString& data, const COLORREF bkcolor, const COLORREF textcolor)
{
	int oldBkMode = pDC->GetBkMode();
	COLORREF oldTextColor = pDC->GetTextColor();
	SetDrawTextMode(pDC, TRANSPARENT, textcolor);

	CBrush brush(bkcolor);
	CBrush * pOldBrush = pDC->SelectObject(&brush);

	// �簢���� �� Ű���ش�
	CRect rc = rect;
	rc.OffsetRect(0, -4);
	rc.InflateRect(3, 0);

	pDC->Rectangle(rc);
	pDC->TextOut(rect.left, rect.top, data);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	SetDrawTextMode(pDC, oldBkMode, oldTextColor);
}

// scale grid�� �׸���.
void CDrawer::DrawGrid(CDC* pDC, const CScaleBaseData::SCALEGRIDTYPE scaleGridType,
		const CPoint& moveTo, const CPoint& lineTo, const COLORREF color, const int nWidth)
{
	//sy 2002.8.10.
	if(scaleGridType == CScaleBaseData::SLNONE)
		return;

	DrawLine(pDC, scaleGridType, moveTo, lineTo, color, nWidth);
}

// line�� �׸���.
void CDrawer::DrawLine(CDC* pDC, const CPoint& movePt, const CPoint& linePt)
{
	pDC->MoveTo(movePt);
	pDC->LineTo(linePt);
}

void CDrawer::DrawLine(CDC* pDC, const CPoint& movePt, const CPoint& linePt, 
		const COLORREF color, const int nWidth, const int penStyle, const int penMode)
{
	int oldROP2 = pDC->GetROP2();
	//sy 2004.10.5. pen�� ���⸦ �޴´�.
	//-> ��Type���� ���ڿ� �ð��� ���� ���� ��� ���ڸ� ���� ǥ���ϱ� ����.
	CPen newPen(penStyle, (nWidth <= 0 ? 1: nWidth), color);
	CPen* pOldPen = pDC->SelectObject(&newPen);
	SetDrawLineMode(pDC, &newPen, penMode);
	
	DrawLine(pDC, movePt, linePt);

	SetDrawLineMode(pDC, pOldPen, oldROP2);
	newPen.DeleteObject();
}

// ------------------------------------------------------------------------------------
POSITION CDrawer::FindPosition(CPacket* pPacket, const int nIndex) const
{
	if(pPacket == NULL)
		return NULL;

	int nTailIndex = pPacket->GetCount() -1;
	CList <double, double> * lst = pPacket->GetnumericDataList();
	if(lst == NULL || nIndex < 0 || nIndex > nTailIndex || lst->GetCount() <= 0)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = lst->GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			lst->GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = lst->GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			lst->GetNext(pos);
		}
	}

	return NULL;
}


// private ==============================================================================
// text�� �׸��µ� �ʿ��� Mode ����.
void CDrawer::SetDrawTextMode(CDC* pDC, const int bkMode, const COLORREF textColor)
{
	pDC->SetBkMode(bkMode);
	pDC->SetTextColor(textColor);
}

// line�� �׸��µ� �ʿ��� Mode ����.
void CDrawer::SetDrawLineMode(CDC* pDC, CPen* pPen, const int rop2Mode)
{
	pDC->SetROP2(rop2Mode);
	pDC->SelectObject(pPen);
}

// --------------------------------------------------------------------------------------
void CDrawer::DrawLine(CDC* pDC, const CScaleBaseData::SCALEGRIDTYPE scaleGridType, 
		const CPoint& movePt, const CPoint& linePt, const COLORREF color, const int nWidth)
{
	int oldROP2 = pDC->GetROP2();
	//sy 2004.10.5. pen�� ���⸦ �޴´�.
	//-> ��Type���� ���ڿ� �ð��� ���� ���� ��� ���ڸ� ���� ǥ���ϱ� ����.
	CPen newPen(PS_SOLID, 1, color);
	//CPen newPen(PS_SOLID | PS_ENDCAP_SQUARE, 2, color);
	CPen* pOldPen = pDC->SelectObject(&newPen);
	SetDrawLineMode(pDC, &newPen, R2_COPYPEN);
	
	if(scaleGridType == CScaleBaseData::SOLIDLINE)
	{
		DrawLine(pDC, movePt, linePt);
		if(nWidth > 1)
		{
			if(movePt.x == linePt.x) //������
			{
				DrawLine(pDC, CPoint(movePt.x - 1, movePt.y), CPoint(linePt.x - 1, linePt.y));
			}
			else if(movePt.y == linePt.y) //����
			{
				DrawLine(pDC, CPoint(movePt.x, movePt.y - 1), CPoint(linePt.x, linePt.y - 1));
			}
		}
	}
	else if(scaleGridType == CScaleBaseData::LONGDOTTEDLINE)
	{
		DrawDotLine(pDC, movePt, linePt, 3, 3, nWidth);
		if(nWidth > 1)
		{
			if(movePt.x == linePt.x) //������
			{
				DrawDotLine(pDC, CPoint(movePt.x - 1, movePt.y), CPoint(linePt.x - 1, linePt.y), 3, 3, nWidth);
			}
			else if(movePt.y == linePt.y) //����
			{
				DrawDotLine(pDC, CPoint(movePt.x, movePt.y - 1), CPoint(linePt.x, linePt.y - 1), 3, 3, nWidth);
			}
		}
	}
	else if(scaleGridType == CScaleBaseData::SHORTDOTTEDLINE)
	{
		DrawDotLine(pDC, movePt, linePt, 1, 3, nWidth);
		if(nWidth > 1)
		{
			if(movePt.x == linePt.x) //������
			{
				DrawDotLine(pDC, CPoint(movePt.x - 1, movePt.y), CPoint(linePt.x - 1, linePt.y), 1, 3, nWidth);
			}
			else if(movePt.y == linePt.y) //����
			{
				DrawDotLine(pDC, CPoint(movePt.x, movePt.y - 1), CPoint(linePt.x, linePt.y - 1), 1, 3, nWidth);
			}
		}
	}
	//sy end

	SetDrawLineMode(pDC, pOldPen, oldROP2);
	newPen.DeleteObject();
}

void CDrawer::DrawDotLine(CDC* pDC, const CPoint& movePt, const CPoint& linePt, const int drawLength, const int gapLength, int nLineWidth)
{
	CPoint preMovePt = movePt, rightPt = linePt;
	if(movePt.x > linePt.x || movePt.y > linePt.y){
		rightPt = movePt;
		preMovePt = linePt;
	}

	int nDrawLength_x = 0, nDrawLength_y = 0, nGapLength_x = 0, nGapLength_y = 0;
	GetLengths(movePt, linePt, drawLength, gapLength, nDrawLength_x, nDrawLength_y, nGapLength_x, nGapLength_y);

	int nCountOfPolyline_X = 0;
	int nCountOfPolyline_Y = 0;

	if((nDrawLength_x + nGapLength_x) != 0) nCountOfPolyline_X = ((rightPt.x - preMovePt.x) / (nDrawLength_x + nGapLength_x)) + 1;
	if((nDrawLength_y + nGapLength_y) != 0) nCountOfPolyline_Y = ((rightPt.y - preMovePt.y) / (nDrawLength_y + nGapLength_y)) + 1;

	int nCountOfPolyline = max(nCountOfPolyline_X, nCountOfPolyline_Y);

	int nIndex = 0;
	POINT* lpptLine = new POINT[nCountOfPolyline * 2];
	DWORD* lpdwPolyPoints = new DWORD[nCountOfPolyline];

	while(preMovePt.x < rightPt.x || preMovePt.y < rightPt.y){
		CPoint curLinePt(preMovePt.x + nDrawLength_x, preMovePt.y + nDrawLength_y);
		if(curLinePt.x > rightPt.x)
			curLinePt.x = rightPt.x;
		if(curLinePt.y > rightPt.y)
			curLinePt.y = rightPt.y;

		lpdwPolyPoints[nIndex/2]  = 2;
		lpptLine[nIndex++] = preMovePt;
		lpptLine[nIndex++] = curLinePt;

		preMovePt = curLinePt;
		preMovePt.Offset(nGapLength_x, nGapLength_y);
	}
	
	if(nIndex != 0)	DrawPolyPolyLine(pDC, lpptLine, lpdwPolyPoints, nIndex / 2, nLineWidth);

	delete [] lpptLine;
	delete [] lpdwPolyPoints;
}

// Windows 95/98/Me ������ lppt �� ������ ������ �ֽ��ϴ�.
// �׿� ���� ó���� ���ؼ� �۾��� �Ͽ����� ���� �ش� OS�� ����ϴ� ��찡 �幰��
// ���� ���� ó���ϱ� ���ؼ� �ش� �Լ��� �ٷ� ȣ���մϴ�.
void CDrawer::DrawPolyPolyLine(CDC* pDC, POINT *lppt, DWORD *lpdwPolyPoints, DWORD cCount, int nLineWidth)
{
	pDC->PolyPolyline(lppt, lpdwPolyPoints, cCount);
}

bool CDrawer::GetLengths(const CPoint& movePt, const CPoint& linePt, const int drawLength, const int gapLength, 
		int& nDrawLength_x, int& nDrawLength_y, int& nGapLength_x, int& nGapLength_y) const
{
	if(drawLength <= 0)
		return false;

	if(movePt.x != linePt.x){
		nDrawLength_x = drawLength;
		nGapLength_x = gapLength;
	}
	else{
		nDrawLength_y = drawLength;
		nGapLength_y = gapLength;
	}

	return (nDrawLength_x > 0 || nDrawLength_y > 0);
}
