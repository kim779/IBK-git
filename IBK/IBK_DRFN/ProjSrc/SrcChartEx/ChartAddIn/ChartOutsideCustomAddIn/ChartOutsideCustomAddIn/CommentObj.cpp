// CommentObj.cpp: implementation of the CCommentItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommentObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommentItem::CCommentItem():m_rc(0,0,COMMENT_WIDTH,COMMENT_HEIGHT)
{
}

CCommentItem::~CCommentItem()
{
}

void CCommentItem::Draw(CDC *pDC)
{
	CRect rect;
	rect = m_rc;
	pDC->SetBkMode(TRANSPARENT);

//	if (m_lSumOrdCnt <= 0)
//		return;

//	CString strCnt;
//	strCnt.Format("%d", (int)m_lSumOrdCnt);
/*
	if( drawingString != "��" &&
		drawingString != "��" &&
		drawingString != "��" &&
		drawingString != "��" &&
		drawingString != "��" &&
		drawingString != "��")
		����
*/
	CString strDrawingMark = "��";

	if (m_bBuy)		// ������ġ
	{
//		rect += CPoint(0, rect.Height()/2);
//		rect.bottom = rect.top + rect.Height()/ 2;

		rect.bottom = m_rc.top + m_rc.Height()/ 2;
		if( m_strComment.Left(4) == "����" )
			pDC->SetTextColor(RGB(0, 208, 208));
		else
			pDC->SetTextColor(RGB(0, 208, 0));
		pDC->DrawText(strDrawingMark, rect, DT_CENTER | DT_TOP);

		rect.top = rect.bottom;
		rect.bottom = rect.top + m_rc.Height()/ 2;
		pDC->SetTextColor(RGB(208, 45, 45));
		pDC->DrawText(m_strComment, rect, DT_CENTER | DT_TOP);
	}
	else			// ����ġ
	{
//		rect.bottom = rect.top + rect.Height() / 2;
		rect.bottom = m_rc.top + m_rc.Height()/ 2;
		pDC->SetTextColor(RGB(45, 45, 208));
		pDC->DrawText(m_strComment, rect, DT_CENTER | DT_BOTTOM);

		rect.top = rect.bottom;
		rect.bottom = rect.top + m_rc.Height()/ 2;
		if( m_strComment.Left(4) == "����" )
			pDC->SetTextColor(RGB(0, 208, 208));
		else
			pDC->SetTextColor(RGB(0, 208, 0));
		pDC->DrawText(strDrawingMark, rect, DT_CENTER | DT_BOTTOM);
	}
}

void CCommentItem::SetDate(long lDate)
{
	int a, b, c;	// �ð� �����͸� �и�

	// ������
	a = lDate/10000;
	lDate = lDate%10000;
	b = lDate/100;
	lDate = lDate%100;
	c = lDate;

	m_strDate.Format("%d/%d/%d", a,b,c);
}

void CCommentItem::SetPosition(CDC* pDC, CPoint *pPoint)
{
	if( m_strComment.IsEmpty() )		return;

	CRect rect;
	pDC->DrawText(m_strComment, &rect, DT_CALCRECT);

	m_rc.left = pPoint->x - rect.Width()/2;
	m_rc.right = pPoint->x + rect.Width()/2;
	if (m_bBuy)		// ������ġ
	{
		m_rc.top = pPoint->y;
		m_rc.bottom = pPoint->y + rect.Height()*2;
	}
	else			// ����ġ
	{
		m_rc.top = pPoint->y - rect.Height()*2;
		m_rc.bottom = pPoint->y;
	}
}

BOOL CCommentItem::IsInRegion(CPoint pt)  
{
	if(m_rc.PtInRect(pt))
		return TRUE;
	else 
		return FALSE;
}

CString CCommentItem::GetToolTipText()
{
	CString strToolTip;
	CString strSide;

	if( m_bBuy )		strSide = "�ż�";
	else				strSide = "�ŵ�";	

	if( m_strTradeType == "91" )
		strToolTip.Format("%s\r\n  ���� : %.f\r\n  ���� : %s\r\n  �ð� : %4s/%2s/%2s-%2s:%2s:%2s  \r\n", 
							strSide,
							m_fAmount,
							m_strPrice,
							m_strDateTime.Left(4), m_strDateTime.Mid(4,2), m_strDateTime.Mid(6,2),
							m_strDateTime.Mid(8,2), m_strDateTime.Mid(10,2), m_strDateTime.Mid(12,2));
	else
		strToolTip.Format("%s <%s>\r\n  ���� : %.f\r\n  ȯ�� : %s\r\n  �ð� : %4s/%2s/%2s-%2s:%2s:%2s  \r\n", 
							m_strComment,
							strSide,
							m_fAmount,
							m_strPrice,
							m_strDateTime.Left(4), m_strDateTime.Mid(4,2), m_strDateTime.Mid(6,2),
							m_strDateTime.Mid(8,2), m_strDateTime.Mid(10,2), m_strDateTime.Mid(12,2));
	return strToolTip;

}
