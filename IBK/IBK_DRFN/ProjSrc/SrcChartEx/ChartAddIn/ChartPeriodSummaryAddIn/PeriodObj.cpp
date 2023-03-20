// PeriodObj.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "PeriodObj.h"

#include <math.h>									// for fabs()

#include "./Include_Chart/DLL_Load/IMetaTable.h"	// for _MHWND()
#include "./Include_Chart/Conversion.h"				// for CDataConversion

#include "EnglishString.h"							// for _ENG_DOWN
#include "ChartPeriodSummaryAddInImp.h"				// for CChartPeriodSummaryAddInImp

// CPeriodObj

CPeriodObj::CPeriodObj()
: m_nUpDown(-1)				// UP/DOWN
, m_nLineThickness(0)		// �� ����
, m_nLineStyle(0)			// �� ��Ÿ��
, m_strSDate(_T(""))		// ������
, m_strEDate(_T(""))		// ���� ��
, m_nRowCnt(1)				// �� ��
, m_strPeriod(_T(""))		// �Ⱓ
, m_bSEPrice(FALSE)			// ������ ����
, m_bEEPrice(FALSE)			// ������ ����
, m_bHPrice(FALSE)			// ��
, m_bLPrice(FALSE)			// ����
, m_bMPrice(FALSE)			// 
, m_bHAmount(FALSE)			// �ִ� �ŷ���
, m_bLAmount(FALSE)			// �ּ� �ŷ���
, m_bMAmount(FALSE)			// ��� �ŷ���
, m_bPCNum(FALSE)			// ��� ��
, m_bMCNum(FALSE)			// ���� ��
, m_bPMRatio(FALSE)			// ������ ����
, m_nTextAlignment(0)		// �ؽ�Ʈ ����
, m_bSaved(FALSE)			// ����Ǿ��� �� ����
, m_nTimeDiff(0)			// ����ڼ��� �ð���	// 20081007 JS.Kim	����ڽð��� ����
,m_strRQ( "DEFAULT")			// (2008/11/5 - Seung-Won, Bae) for Multiple Item.
{
	m_ptAStart = CPoint(0,0);
	m_ptAEnd = CPoint(0,0);
	m_size = CSize(0,0);
	m_bSelected = FALSE;
	m_CurClickedPosition = N_PT;
	m_rc = CRect(0,0,0,0);
	m_rcLine = CRect(0,0,0,0);
	m_rcText = CRect(0,0,0,0);
	m_ptALineOffset = CPoint(0,0);
	m_ptLTOffset = CPoint(OFFSET_LT, -OFFSET_LT);
	m_ptRLineOffset = CPoint(0,0);
	m_ptRTextOffset = CPoint(0,0);
	m_nLineThickness = 1;
	m_clrLineColor = RGB(255,0,0);

	// 20081009 �ڵ��� >>
	m_pChartPeriodSummaryAddInImp = NULL;
	// 20081009 �ڵ��� <<
}

CPeriodObj::~CPeriodObj()
{
}

// CPeriodObj ��� �Լ�

// 20081009 �ڵ��� >>
void CPeriodObj::SetChartPeriodSummaryAddInImp(CChartPeriodSummaryAddInImp* pChartPeriodSummaryAddInImp)
{
	if(!m_pChartPeriodSummaryAddInImp)
		m_pChartPeriodSummaryAddInImp = pChartPeriodSummaryAddInImp;
}

CChartPeriodSummaryAddInImp* CPeriodObj::GetChartPeriodSummaryAddInImp()
{
	return m_pChartPeriodSummaryAddInImp;
}

CString CPeriodObj::GetPacketType()
{
	if( !m_pChartPeriodSummaryAddInImp) return "";
	CString strResult = "";

	IPacketListManager *pIPacketListManager = m_pChartPeriodSummaryAddInImp->GetPacketListManager();
	if( pIPacketListManager)
	{
		IPacketManager *pIPacketManager = pIPacketListManager->GetPacketManager( m_strRQ);
		if( pIPacketManager)
		{
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_CLOSE));
			if( pIPacket)
			{
				ILPCSTR szType = pIPacket->GetType();
				strResult = szType;
				pIPacket->Release();
			}
			pIPacketManager->Release();
		}
		pIPacketListManager->Release();
	}
	return strResult;
}
// 20081009 �ڵ��� <<

// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// ���� �ֱ��� ��¥�ð� Type�� ��´�.
CString CPeriodObj::GetPacketDateType()
{
	if( !m_pChartPeriodSummaryAddInImp) return "";
	CString strResult = "";
	
	IPacketListManager *pIPacketListManager = m_pChartPeriodSummaryAddInImp->GetPacketListManager();
	if( pIPacketListManager)
	{
		IPacketManager *pIPacketManager = pIPacketListManager->GetPacketManager( m_strRQ);
		if( pIPacketManager)
		{
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_DATE_TIME));
			if( pIPacket)
			{
				ILPCSTR szType = pIPacket->GetType();
				strResult = szType;
				pIPacket->Release();
			}
			pIPacketManager->Release();
		}
		pIPacketListManager->Release();
	}
	return strResult;
}

// �ֱ������� ��´�.
int CPeriodObj::GetPacketDateUnit()
{
	if( !m_pChartPeriodSummaryAddInImp) 
		return 0;
	
	int nResult = 0;
	
	IPacketListManager *pIPacketListManager = m_pChartPeriodSummaryAddInImp->GetPacketListManager();
	if( pIPacketListManager)
	{
		IPacketManager *pIPacketManager = pIPacketListManager->GetPacketManager(m_strRQ);
		if( pIPacketManager)
		{
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_DATE_TIME));
			if( pIPacket)
			{
				nResult = pIPacket->GetDateUnitType();
				pIPacket->Release();
			}
			pIPacketManager->Release();
		}
		pIPacketListManager->Release();
	}
	return nResult;
}
// 2011.01.24 by SYS <<


int CPeriodObj::IsInRegion(CPoint pt)
{
	if(IsOnEndPoint(pt) || IsOnLine(pt))
		return TRUE;
	return FALSE;
}


int CPeriodObj::IsOnLine(CPoint pt)
{
	pt -= m_ptALineOffset;
	CRect rc = m_rcLine;
	rc.InflateRect(LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY);
	if(rc.PtInRect(pt) && !m_rcCut.PtInRect(pt) && !m_rcCutS.PtInRect(pt) && !m_rcCutE.PtInRect(pt))
	{
		m_CurClickedPosition = M_PT;
		return m_CurClickedPosition;
	}
	return N_PT;
}

BOOL CPeriodObj::IsOnEndPoint(CPoint pt)
{
	if(pt.x >= m_ptAStart.x-RADIUS_OF_END_RC && pt.x <= m_ptAStart.x+RADIUS_OF_END_RC && pt.y >= m_ptAStart.y-RADIUS_OF_END_RC && pt.y <= m_ptAStart.y+RADIUS_OF_END_RC)
	{
		m_CurClickedPosition = S_PT;
		m_ptAt = m_ptAStart - pt;
		return TRUE;
	}
	if(pt.x >= m_ptAEnd.x-RADIUS_OF_END_RC && pt.x <= m_ptAEnd.x+RADIUS_OF_END_RC && pt.y >= m_ptAEnd.y-RADIUS_OF_END_RC && pt.y <= m_ptAEnd.y+RADIUS_OF_END_RC)
	{
		m_CurClickedPosition = E_PT;
		m_ptAt = m_ptAEnd - pt;
		return TRUE;
	}

	m_CurClickedPosition = N_PT;
	return FALSE;
}

void CPeriodObj::Draw(CDC* pDC, HWND p_hOcxWnd)
{
	MakeData( p_hOcxWnd);
//	ReCalcRect();
	GetRect();
	// �ӽ� ��
	CPen pen (m_nLineStyle, m_nLineThickness, m_clrLineColor);
	CPen* pOldPen = pDC->SelectObject(&pen);

	//>>20100112_JS.Kim �׷��������� ����� ��� ����
	/*
	if(m_nUpDown == UP)	// Up ����
	{
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.left,m_rcLine.top)				);	// top-left�� �̵�
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.right,m_rcLine.top)				);	// top-right���� ���� ��
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2)													);	// top-left�� �̵�
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptRStart:m_ptREnd)	);	// ���������� ���� ��
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.right,m_rcLine.top)				);	// top-right�� �̵�
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptREnd:m_ptRStart)	);	// �������� ���� ��
	}
	else					// Down ����
	{
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.left,m_rcLine.bottom)			);	// bottom-left�� �̵�
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.right,m_rcLine.bottom)			);	// bottom-right���� ���� ��
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.left,m_rcLine.bottom)			);	// bottom-left�� �̵�
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptRStart:m_ptREnd)	);	// ���������� ���� ��
		pDC->MoveTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.right,m_rcLine.bottom)			);	// bottom-left�� �̵�
		pDC->LineTo(m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptREnd:m_ptRStart)	);	// �������� ���� ��
	}
	*/
	if(m_nUpDown == UP)	// Up ����
	{
		CPoint pt = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2);
		if( pt.y < m_rctGraphRegion.top + 2)		pt.y = m_rctGraphRegion.top + 2;

		pDC->MoveTo(pt + CPoint(m_rcLine.left,m_rcLine.top)					);	// top-left�� �̵�
		m_pBaseLine = pt + CPoint(m_rcLine.left,m_rcLine.top);	//20110202_alzioyes: UP/DOWN���� �ؽ�Ʈ��ġ ����.
		pDC->LineTo(pt + CPoint(m_rcLine.right,m_rcLine.top)				);	// top-right���� ���� ��
		pDC->MoveTo(pt														);	// top-left�� �̵�

		CPoint ptEnd = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptRStart:m_ptREnd);
		if( ptEnd.y < m_rctGraphRegion.top + RADIUS_OF_END_RC + OFFSET_RC_IN )		
			ptEnd.y = m_rctGraphRegion.top + RADIUS_OF_END_RC + OFFSET_RC_IN;
		pDC->LineTo(ptEnd);	// ���������� ���� ��

		pDC->MoveTo(pt + CPoint(m_rcLine.right,m_rcLine.top)				);	// top-right�� �̵�

		ptEnd = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptREnd:m_ptRStart);
		if( ptEnd.y < m_rctGraphRegion.top + RADIUS_OF_END_RC+OFFSET_RC_IN)		
			ptEnd.y = m_rctGraphRegion.top + RADIUS_OF_END_RC + OFFSET_RC_IN;
		pDC->LineTo(ptEnd);	// �������� ���� ��
	}
	else					// Down ����
	{
		CPoint pt = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + CPoint(m_rcLine.left,m_rcLine.bottom);
		if( pt.y > m_rctGraphRegion.bottom - 2)		pt.y = m_rctGraphRegion.bottom - 2;
		pDC->MoveTo(pt														);	// bottom-left�� �̵�
		m_pBaseLine = pt + CPoint(m_rcLine.left,m_rcLine.top);	//20110202_alzioyes: UP/DOWN���� �ؽ�Ʈ��ġ ����.
		pDC->LineTo(pt + CPoint(m_rcLine.right-m_rcLine.left, 0)			);	// bottom-right���� ���� ��
		pDC->MoveTo(pt														);	// bottom-left�� �̵�

		CPoint ptEnd = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptRStart:m_ptREnd);
		if( ptEnd.y > m_rctGraphRegion.bottom - RADIUS_OF_END_RC - OFFSET_RC_IN )		
			ptEnd.y = m_rctGraphRegion.bottom - RADIUS_OF_END_RC - OFFSET_RC_IN;
		pDC->LineTo(ptEnd);	// ���������� ���� ��

		pDC->MoveTo(pt + CPoint(m_rcLine.right-m_rcLine.left,0)				);	// bottom-right�� �̵�

		ptEnd = m_ptAOffset + m_ptRLineOffset + CPoint(-OFFSET_RC_IN/2, -OFFSET_RC_IN/2) + ((m_ptRStart.x<m_ptREnd.x)?m_ptREnd:m_ptRStart);
		if( ptEnd.y > m_rctGraphRegion.bottom - RADIUS_OF_END_RC-OFFSET_RC_IN)		
			ptEnd.y = m_rctGraphRegion.bottom - RADIUS_OF_END_RC - OFFSET_RC_IN;
		pDC->LineTo(ptEnd);	// �������� ���� ��
	}
	//<<
	
	DrawEndPoints(pDC);
	DrawTable(pDC);

	pDC->SelectObject(pOldPen);
}

//////////////////////////////////////////////////////////////////////////
// �׸��� ����
// ���콺 Ŭ�� ����/����(���� ��ǥ)���� ���� ��� ���� ��´�
//
CRect CPeriodObj::GetRect()
{
//////////////////////////////////////////////////////////////////////////
// �׸��� ���� 1
//////////////////////////////////////////////////////////////////////////
	// ����
	m_rcLine.right = abs(m_ptAStart.x - m_ptAEnd.x);
	m_rcLine.bottom = abs(m_ptAStart.y - m_ptAEnd.y) + DEFAULT_HEIGHT;

	
//////////////////////////////////////////////////////////////////////////
// ���� ��ġ
//////////////////////////////////////////////////////////////////////////
	// ����
	m_ptALineOffset.x = min(m_ptAStart.x, m_ptAEnd.x);

	switch(m_nUpDown)
	{
	case UP:
		m_ptALineOffset.y = min(m_ptAStart.y, m_ptAEnd.y) - DEFAULT_HEIGHT;
		break;
	case DOWN:
		m_ptALineOffset.y = min(m_ptAStart.y, m_ptAEnd.y);
		break;
	}
	// ���̺�
	m_ptATableOffset =  m_ptALineOffset + m_ptLTOffset;
	// �ؽ�Ʈ
	m_ptATextOffset =  m_ptATableOffset + CPoint(OFFSET_TT_IN, OFFSET_TT_IN);
	// ��ü
	m_ptAOffset.x = m_ptALineOffset.x - OFFSET_RC_IN;
	m_ptAOffset.y = min(m_ptALineOffset.y, m_ptATableOffset.y) - OFFSET_RC_IN;


//	TRACE3("\n\n***In object - table y : %d,line y : %d, start y : %d",m_ptATableOffset.y,m_ptALineOffset.y,m_ptAStart.y);
//	TRACE1(", end y : %d\n\n",m_ptAEnd.y);

//////////////////////////////////////////////////////////////////////////
// ��� ��ġ
//////////////////////////////////////////////////////////////////////////
	// ����
	m_ptRLineOffset = CPoint(OFFSET_RC_IN, OFFSET_RC_IN - m_ptLTOffset.y);	// m_rc ����
	m_ptRStart = m_ptAStart - m_ptAOffset - m_ptRLineOffset;				// m_rcLine ����
	m_ptREnd = m_ptAEnd - m_ptAOffset - m_ptRLineOffset;					// m_rcLine ����

	switch(m_nUpDown)
	{
	case UP:
		// ���̺�
		m_ptRTableOffset = CPoint(m_ptLTOffset.x + OFFSET_RC_IN, OFFSET_RC_IN);	// m_rc ����
		// �ؽ�Ʈ
		m_ptRTextOffset =  CPoint(m_ptLTOffset.x + OFFSET_RC_IN+OFFSET_TT_IN, OFFSET_RC_IN+OFFSET_TT_IN);	// m_rc ����
		break;
	case DOWN:
		// ���̺�
		m_ptRTableOffset = CPoint(m_ptLTOffset.x + OFFSET_RC_IN, m_rcLine.bottom);	// m_rc ����
		// �ؽ�Ʈ
		m_ptRTextOffset =  CPoint(m_ptLTOffset.x + OFFSET_RC_IN+OFFSET_TT_IN, m_rcLine.bottom + (OFFSET_TT_IN));	// m_rc ����
		break;
	}

//	TRACE3("\n\nUP/DOWN:%d, ����:%d, ���:%d\n\n", m_nUpDown, m_ptATableOffset, m_ptRTableOffset.y);

//////////////////////////////////////////////////////////////////////////
// �׸��� ���� 2
//////////////////////////////////////////////////////////////////////////
	// ����
//	m_rcLine.right = abs(m_ptAStart.x - m_ptAEnd.x);
//	m_rcLine.bottom = abs(m_ptAStart.y - m_ptAEnd.y) + DEFAULT_HEIGHT;
	m_rcCut = m_rcLine;
	switch(m_nUpDown)
	{
	case UP:
		m_rcCut.InflateRect(-LINE_ON_SENSITIVITY,-LINE_ON_SENSITIVITY,-LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY);	// ���� ���� 4 pixel ����
		m_rcCutS = CRect(m_rcLine.left-LINE_ON_SENSITIVITY, m_ptRStart.y,m_rcCut.left, m_rcLine.bottom);
		m_rcCutE = CRect(m_rcCut.right, m_ptREnd.y,m_rcLine.right+LINE_ON_SENSITIVITY, m_rcLine.bottom);
		break;
	case DOWN:
		m_rcCut.InflateRect(-LINE_ON_SENSITIVITY,LINE_ON_SENSITIVITY,-LINE_ON_SENSITIVITY,-LINE_ON_SENSITIVITY);	// ���� ���� 4 pixel ����
		m_rcCutS = CRect(m_rcLine.left-LINE_ON_SENSITIVITY, m_rcLine.top,m_rcCut.left, m_ptRStart.y);
		m_rcCutE = CRect(m_rcCut.right, m_rcLine.top,m_rcLine.right+LINE_ON_SENSITIVITY, m_ptREnd.y);
		break;
	}
	
	// �ؽ�Ʈ
	m_rcText.right = TEXT_WIDTH * max(m_strPeriod.GetLength(), (m_strBasicInfo1.GetLength()+m_strBasicInfo2.GetLength()+m_strBasicInfo3.GetLength()));
	m_rcText.bottom = m_nRowCnt*TEXT_HEIGHT;
	// ���̺�
	m_rcTable = m_rcText;
	m_rcTable.InflateRect(0,0,0,20);

	// ��ü
	m_rc.right = max(m_ptRLineOffset.x+m_rcLine.right, m_ptRTableOffset.x+m_rcTable.right) + OFFSET_RC_IN;
	m_rc.bottom = max(m_ptRLineOffset.y+m_rcLine.bottom, m_ptRTableOffset.y+m_rcTable.bottom) + OFFSET_RC_IN;

	m_size.cx = m_rc.Width();	// ��ü ���� ��
	m_size.cy = m_rc.Height();	// ��ü ���� ����

	return m_rc;
}


void CPeriodObj::DrawEndPoints(CDC *pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(m_clrLineColor);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CRect rcStart, rcEnd;
	rcStart.left = m_ptRStart.x - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcStart.top = m_ptRStart.y - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcStart.right = m_ptRStart.x + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcStart.bottom = m_ptRStart.y + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcStart.OffsetRect(m_ptAOffset + m_ptRLineOffset);
	//>>20100112_JS.Kim �׷��������� ����� ��� ����
	if( rcStart.top < m_rctGraphRegion.top )
	{
		rcStart.top = m_rctGraphRegion.top;
		rcStart.bottom = m_rctGraphRegion.top + 3*RADIUS_OF_END_RC;
	}
	else if( rcStart.bottom > m_rctGraphRegion.bottom )
	{
		rcStart.bottom = m_rctGraphRegion.bottom;
		rcStart.top = m_rctGraphRegion.bottom - 3*RADIUS_OF_END_RC;
	}
	//<<
	pDC->Ellipse(rcStart);

	rcEnd.left = m_ptREnd.x - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.top = m_ptREnd.y - RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.right = m_ptREnd.x + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.bottom = m_ptREnd.y + 2*RADIUS_OF_END_RC - OFFSET_RC_IN/2;
	rcEnd.OffsetRect(m_ptAOffset + m_ptRLineOffset);
	//>>20100112_JS.Kim �׷��������� ����� ��� ����
	if( rcEnd.top < m_rctGraphRegion.top )
	{
		rcEnd.top = m_rctGraphRegion.top;
		rcEnd.bottom = m_rctGraphRegion.top + 3*RADIUS_OF_END_RC;
	}
	else if( rcEnd.bottom > m_rctGraphRegion.bottom )
	{
		rcEnd.bottom = m_rctGraphRegion.bottom;
		rcEnd.top = m_rctGraphRegion.bottom - 3*RADIUS_OF_END_RC;
	}
	//<<
	pDC->Ellipse(rcEnd);
}

//////////////////////////////////////////////////////////////////////////
//	���콺 Ŭ�� ��ġ�� Ÿ�Կ� ���� �ʿ��� �����Ϳ� �����Ѵ�.
// �ַ� ������Ʈ ����(���� �׸���)�� ���ȴ�.
void CPeriodObj::SetPos(CPoint pt, int type)
{
	switch(type)
	{
	case S_PT:
		m_ptAStart = pt;
		break;
	case E_PT:
		m_ptAEnd = pt;
		break;
	case M_PT:		
		m_ptAStart += pt - m_ptAt;
		m_ptAEnd += pt - m_ptAt;
		m_ptAt = pt;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Ÿ�Կ� ���� �ش��ϴ� ��ġ ������ ��ȯ�Ѵ�.
CPoint CPeriodObj::GetPos(int type)
{
	switch(type)
	{
	case S_PT:
		return m_ptAStart;
		break;
	case E_PT:
		return m_ptAEnd;
		break;
	default:
		return m_ptAt;
	}
}

//////////////////////////////////////////////////////////////////////////
//	���콺 Ŭ�� ��ġ�� ���� ���õ� Ÿ�Կ� ���� �ʿ��� �����Ϳ� �����Ѵ�.
//  �ַ� ������Ʈ ���ý� ���ȴ�.
void CPeriodObj::ButtonDownAt(const CPoint &pt)
{	
	switch(m_CurClickedPosition)
	{
	case S_PT:
		m_ptAStart = pt+m_ptAt;
		break;
	case E_PT:
		m_ptAEnd = pt+m_ptAt;
		break;
	case M_PT:
		m_ptAt = pt;
		m_ptAStart += pt - m_ptAt;
		m_ptAEnd += pt - m_ptAt;
		break;
	default:
		m_ptAStart = m_ptAEnd = pt;		 
	}
}

//////////////////////////////////////////////////////////////////////////
//	���콺 Ŭ�� �� ��ġ�� ���� ���õ� Ÿ�Կ� ���� �ʿ��� �����Ϳ� �����Ѵ�.
//  
void CPeriodObj::ButtonUpAt(const CPoint &pt)
{	
	switch(m_CurClickedPosition)
	{
	case S_PT:
		m_ptAStart = pt;
		break;
	case E_PT:
		m_ptAEnd = pt;
		break;
	case M_PT:
		m_ptAStart += pt - m_ptAt;
		m_ptAEnd += pt - m_ptAt;
		m_ptAt = pt;
		break;
	case N_PT:
		m_ptAEnd = pt;
		break;
	}
}


// void CPeriodObj::ReCalcRect()
//{
//	m_oldrc = m_rc;
//	GetRect();
//}

//void CPeriodObj::Erase(CDC* pDC)
//{
//}

/**
	@history
		2011.01.05 alzioyes:�ַθ��û���� �ݿ�.
			1. �׸�ڽ� �����ϰ� �޹���� ���̵��� �����ϰ� ó��
			2. �����, ����� ���� ����
		2011.01.12 alzioyes:[CSA00000070]����ǥ���ϴ� ��ġ �����û ����.
			����ǥ�� ��ġ�� UP������ ������, DOWN�϶��� ���Ʒ��� ����.
*/
void CPeriodObj::DrawTable(CDC* pDC)
{
	// ���̺� ǥ�� �Ӽ�
	//CBrush brush;
	//CPen pen (PS_SOLID, (m_bSelected)?2:1, RGB(157, 157, 217));
	//brush.CreateSolidBrush(RGB(255,255,255));

	//CBrush* pOldBrush = pDC->SelectObject(&brush);
	//CPen* pOldPen = pDC->SelectObject(&pen);

	CString szValFormat("%.f");
	int nDecimalDownCount = 0;
	CString strType;
	GetPacketTypeInfo( nDecimalDownCount, strType);
	szValFormat.Format("%c.%df", '%', nDecimalDownCount);

	//[CSA00000070-1]����ǥ�� ��ġ�� UP������ ������, DOWN�϶��� ���Ʒ��� ����.
	CPoint tmpOffset(m_ptAOffset);
	int nTmpOffset;
	if(m_nUpDown == UP)
	{
		nTmpOffset=14;
		tmpOffset.y -= nTmpOffset;
	}
	else
	{
		nTmpOffset=20;
		tmpOffset.y += nTmpOffset;
	}
	
	CRect rcTableDraw = m_ptAOffset + m_rcTable;
	//CRect rcTextDraw = m_ptAOffset + m_rcText;
	CRect rcTextDraw = tmpOffset + m_rcText;

 	CPoint ptTableOffset = m_ptRTableOffset;
 	CPoint ptTextOffset = m_ptRTextOffset;

	switch(m_nTextAlignment)
	{
	case 0:	//left
		break;
	case 1:	//right
		if(m_rcLine.right+m_ptRLineOffset.x > m_rcTable.right + m_ptRTableOffset.x)	// ���� �� �����ʿ� ������
		{
			ptTableOffset.x +=  m_rcLine.right - (m_ptLTOffset.x*2 + rcTableDraw.Width());
			ptTextOffset.x +=  m_rcLine.right - (m_ptLTOffset.x*2 + rcTableDraw.Width());
		}

		break;

	}

	rcTextDraw.OffsetRect(ptTextOffset);
	rcTableDraw.OffsetRect(ptTableOffset);

	// text ǥ�� �Ӽ�
	CFont defFont, boldFont;

	CString strFont;
	strFont.LoadString(IDS_STRING_FONT);
	// defFont.CreatePointFont(90,_T("����"), NULL);
	defFont.CreatePointFont(90,strFont, NULL);
	
	LOGFONT BoldLog;
	defFont.GetLogFont(&BoldLog);
	//	BoldLog.lfWeight = FW_BOLD;
	boldFont.CreateFontIndirect(&BoldLog);
	
	CFont *poldFont = pDC->SelectObject(&defFont);
	
	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(0,0,0)); //RGB(128,128,128)
	pDC->SetBkMode(TRANSPARENT);

	// �׸��� ���� ���(��)

	CString strCellValue;
	CRect rcCell = rcTextDraw;
	rcCell.left += rcTextDraw.Width()/2;

	//////////////////////////////////////////////////////////////////////////
	// �ڽ� ũ�⸦ ���Ѵ�
 	if(m_bSEPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.sPrice, strType);
		//strCellValue.Format("%ld",(long)m_Price.sPrice);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bEEPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.ePrice, strType);
		//strCellValue.Format("%ld",(long)m_Price.ePrice);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bHPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.max, strType);
		//strCellValue.Format("%d",(long)m_Price.max);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bLPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.min, strType);
		//strCellValue.Format("%d",(long)m_Price.min);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bMPrice)
	{
		strCellValue = CDataConversion::GetStringThousandFormat(m_Price.avg, strType);
		//strCellValue.Format("%.2lf",m_Price.avg);
		//strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bHAmount)
	{
		strCellValue.Format("%ld",(long)m_Amount.max);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bLAmount)
	{
		strCellValue.Format("%ld",(long)m_Amount.min);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bMAmount)
	{
		strCellValue.Format("%.2lf",m_Amount.avg);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bPCNum)
	{
		strCellValue.Format("%d",m_Candle.plus);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bMCNum)
	{
		strCellValue.Format("%d",m_Candle.minus);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}
 	if(m_bPMRatio)
	{
		strCellValue.Format("%.2lf%%", m_Candle.ratio);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcCell.Width());
	}

	//[CSA00000070-2]����ǥ�� ��ġ�� UP������ ������, DOWN�϶��� ���Ʒ��� ����.
	//>>20100112_JS.Kim �׷��������� ����� ��� ����
	if( rcTableDraw.right > m_rctGraphRegion.right && rcTableDraw.left < m_rctGraphRegion.right )
	{
		CPoint ptMove(m_rctGraphRegion.right-rcTableDraw.right, 0);
		rcTableDraw.OffsetRect(ptMove);
		rcTextDraw.OffsetRect(ptMove);
	}
// 	nTmpOffset = 0;
// 	if( rcTableDraw.bottom+nTmpOffset > m_rctGraphRegion.bottom )
// 	{
// 		CPoint ptMove(0, m_rctGraphRegion.bottom-rcTableDraw.bottom-nTmpOffset);
// 		rcTableDraw.OffsetRect(ptMove);
// 		rcTextDraw.OffsetRect(ptMove);
// 	}
// 	if( rcTableDraw.top < m_rctGraphRegion.top+nTmpOffset )
// 	{
// 		CPoint ptMove(0, m_rctGraphRegion.top - rcTableDraw.top + nTmpOffset);
// 		rcTableDraw.OffsetRect(ptMove);
// 		rcTextDraw.OffsetRect(ptMove);
// 	}

	//>>20110202_alzioyes: UP/DOWN���� �ؽ�Ʈ��ġ ����.
	//		UP�϶�   �ؽ�Ʈ��ġ�� ���� �Ѿ �� �� ������ ����.
	//		DOWN�϶� �ؽ�Ʈ��ġ�� �Ʒ��� ������ �� �� ���� ����.
	int nH1, nH2;
	nH1 = rcTableDraw.Height();
	nH2 = rcTextDraw.Height();

	if(m_nUpDown == UP)
	{
		if( rcTableDraw.top < m_rctGraphRegion.top+10)
		{
			rcTableDraw.top = m_pBaseLine.y+2; 
			rcTextDraw.top = m_pBaseLine.y+2; 
			rcTableDraw.bottom = rcTableDraw.top + nH1;
			rcTextDraw.bottom = rcTextDraw.top + nH2;
		}
	}
	else
	{
		if( rcTableDraw.top+50> m_rctGraphRegion.bottom )
		{
			rcTableDraw.top = m_pBaseLine.y-27; 
			rcTextDraw.top = m_pBaseLine.y-27; 
			rcTableDraw.bottom = rcTableDraw.top + nH1;
			rcTextDraw.bottom = rcTextDraw.top + nH2;
		}
	}
	//<<20110202_alzioyes: UP/DOWN���� �ؽ�Ʈ��ġ ����.

	CRect rcOrgTextDraw = rcTextDraw;
	//<<

	// �ڽ��� �׸���.
		
	//@@@@@pDC->RoundRect(rcTableDraw,CPoint(0,0));
//	TRACE2("\n\n       (%d, %d)\n\n", rcTableDraw.top, ptTableOffset.y);

	m_rc.right += 100;

	// �⺻ ����1 (ĵ�� ��)�� ǥ���Ѵ�.
	pDC->DrawText(m_strBasicInfo1,rcTextDraw,DT_LEFT| DT_VCENTER);

	// ��/�ٿ� ǥ���� ���� �����Ѵ�.
	switch(m_nUpDownSymbol)
	{
	case -1:
		pDC->SetTextColor(RGB(0,0,255));
		break;
	case 1:
		pDC->SetTextColor(RGB(255,0,0));
		break;
	}
	rcTextDraw.OffsetRect(TEXT_WIDTH*m_strBasicInfo1.GetLength(),0);
	// �⺻ ����2 (��/�ٿ� ǥ��)�� ǥ���Ѵ�.
	pDC->DrawText(m_strBasicInfo2,rcTextDraw,DT_LEFT| DT_VCENTER);

	//SMalzio110105-001 �����, ��������� ���� ���� ����.
	//pDC->SetTextColor(RGB(0,0,0));
	rcTextDraw.OffsetRect(TEXT_WIDTH*m_strBasicInfo2.GetLength(),0);
	// �⺻ ����3 (���� ����)�� ǥ���Ѵ�.
	pDC->DrawText(m_strBasicInfo3,rcTextDraw,DT_LEFT| DT_VCENTER);

	// �ٽ� ó�� ��ġ�� ����(CRLF)
	//>>20100112_JS.Kim �׷��������� ����� ��� ����
	//rcTextDraw = m_ptAOffset + m_rcText;
	//rcTextDraw.OffsetRect(ptTextOffset);
	rcTextDraw = rcOrgTextDraw;
	//<<
	pDC->SetTextColor(RGB(0,0,0));
	rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
	// �Ⱓ ǥ��
	pDC->DrawText(m_strPeriod,rcTextDraw,DT_LEFT| DT_VCENTER);

	/////////////////////////////////
	// �������� ���� ���� ���´�.
	

	/////////////////////////////////
	
	// �ɼǿ� ���� �� ���� ǥ��

	// 20081009 �ڵ��� ��ŶŸ���� ���´�. >>
	CString strValue;
	//CString strType = GetPacketType();
	strType = GetPacketType();
	// 20081009 �ڵ��� <<

	if(m_bSEPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strStartColseNumber;
		strStartColseNumber.LoadString(IDS_STARTING_CLOSE_NUMBERS);
		// pDC->DrawText("����������",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strStartColseNumber,rcTextDraw,DT_LEFT| DT_VCENTER);

		// 20081009 �ڵ��� 8����/32���� ó�� >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// ������ȯ�� ���...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.sPrice, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.sPrice);
			strValue.Format(szValFormat, m_Price.sPrice);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 �ڵ��� <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bEEPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strEndingClosePrive;
		strEndingClosePrive.LoadString(IDS_ENDING_CLOSE_PRIVE);
		// pDC->DrawText("����������",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strEndingClosePrive,rcTextDraw,DT_LEFT| DT_VCENTER);

		// 20081009 �ڵ��� 8����/32���� ó�� >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// ������ȯ�� ���...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.ePrice, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.ePrice);
			strValue.Format(szValFormat, m_Price.ePrice);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 �ڵ��� <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bHPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;
		
		CString strHighPrice;
		strHighPrice.LoadString( IDS_HIGH_PRICE);
		//pDC->DrawText("��",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strHighPrice,rcTextDraw,DT_LEFT| DT_VCENTER);	

		// 20081009 �ڵ��� 8����/32���� ó�� >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// ������ȯ�� ���...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.max, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.max);
			strValue.Format(szValFormat, m_Price.max);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 �ڵ��� <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bLPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strLowPrice;
		strLowPrice.LoadString(IDS_LOW_PRICE);		
		// pDC->DrawText("����",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strLowPrice,rcTextDraw,DT_LEFT| DT_VCENTER);

		// 20081009 �ڵ��� 8����/32���� ó�� >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// ������ȯ�� ���...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.min, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.min);
			strValue.Format(szValFormat, m_Price.min);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 �ڵ��� <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bMPrice)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strAverageVolume;
		strAverageVolume.LoadString( IDS_AVERAGE_PRICE);
		// pDC->DrawText("��հ�",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strAverageVolume,rcTextDraw,DT_LEFT| DT_VCENTER);

		// 20081009 �ڵ��� 8����/32���� ó�� >>
		if(CDataConversion::IsBaseFormat(strType) && strType != "")
		{
			// ������ȯ�� ���...
			strValue = CDataConversion::GetStringThousandFormat(m_Price.avg, strType);
		}
		else
		{
			//strValue.Format("%ld",(long)m_Price.avg);
			strValue.Format("%.2lf",m_Price.avg);
			strValue = CommaStr(strValue);
		}
		strCellValue.Format(":  %s", strValue);
		// 20081009 �ڵ��� <<

		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bHAmount)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strMaxVolume;
		strMaxVolume.LoadString(IDS_MAX_VOLUME);
		// pDC->DrawText("�ִ�ŷ���",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strMaxVolume,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%ld",(long)m_Amount.max);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bLAmount)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strMinVolume;
		strMinVolume.LoadString(IDS_MIN_VOLUME);
		// pDC->DrawText("�ּҰŷ���",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strMinVolume,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%ld",(long)m_Amount.min);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bMAmount)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strAverageVolume;
		strAverageVolume.LoadString(IDS_AVERAGE_VOLUME);
		// pDC->DrawText("��հŷ���",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strAverageVolume,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%.2lf",m_Amount.avg);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bPCNum)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strPositiveBongNumber;
		strPositiveBongNumber.LoadString(IDS_POSITIVE_BONG_NUMBERS);
		// pDC->DrawText("�����",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strPositiveBongNumber,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%d",m_Candle.plus);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bMCNum)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strNegativeBongNumbers;
		strNegativeBongNumbers.LoadString(IDS_NEGATIVE_BONG_NUMBERS);
		// pDC->DrawText("������",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strNegativeBongNumbers,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%d",m_Candle.minus);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_WIDTH, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
 	if(m_bPMRatio)
	{
		rcTextDraw.OffsetRect(0, TEXT_HEIGHT);
		rcCell = rcTextDraw;
		rcCell.left += rcTextDraw.Width()/2;

		CString strPositiveNegativePercentage;
		strPositiveNegativePercentage.LoadString(IDS_POSITIVE_NEGATIVE_PERCENTAGE);
		// pDC->DrawText("����������",rcTextDraw,DT_LEFT| DT_VCENTER);
		pDC->DrawText(strPositiveNegativePercentage,rcTextDraw,DT_LEFT| DT_VCENTER);

		strCellValue.Format("%.2lf%%", m_Candle.ratio);
		strCellValue.Format(":  %s", CommaStr(strCellValue));
		rcCell.right = rcCell.left + max(strCellValue.GetLength()*TEXT_HEIGHT, rcTextDraw.Width()/2);
		pDC->DrawText(strCellValue,rcCell,DT_LEFT| DT_VCENTER);
	}
	
	
	// ��, �귯�� ����
	//pDC->SelectObject(pOldBrush);
	//pDC->SelectObject(pOldPen);
	pDC->SelectObject(poldFont);

}
///////////////////////////////////////////////////////////////////////////
// �⺻ ������ ����
void CPeriodObj::MakeData( HWND p_hOcxWnd)
{
	CString szValFormat("%.f");
	int nDecimalDownCount = 0;
	CString strType;
	GetPacketTypeInfo( nDecimalDownCount, strType);
	szValFormat.Format("%c.%df", '%', nDecimalDownCount);

	// (2008/3/4 - Seung-Won, Bae) for Multi-Language
	IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( p_hOcxWnd);
	if(m_Price.ePrice < m_Price.sPrice)	// �϶�
	{
		m_nUpDownSymbol = -1;
		switch( eLanguageID)
		{
			case IMetaTable::KOREAN:	m_strBasicInfo2 = "��";
										break;
			default:					m_strBasicInfo2 = _ENG_DOWN;
										break;
		}
	}
	else if(m_Price.ePrice > m_Price.sPrice)	// ���
	{
		m_nUpDownSymbol = 1;
		switch( eLanguageID)
		{
			case IMetaTable::KOREAN:	m_strBasicInfo2 = "��";
										break;
			default:					m_strBasicInfo2 = _ENG_UP;
										break;
		}
	}
	else
	{
		m_nUpDownSymbol = 0;
		m_strBasicInfo2 = "-";
	}

	CString strCandle;
	strCandle.LoadString(IDS_CANDLE);
	// m_strBasicInfo1.Format("%d ĵ��", m_Candle.count);
	m_strBasicInfo1.Format("%d %s"
						, m_Candle.count, strCandle);

//	CString strDiffPrice;
//	strDiffPrice.Format("%ld", abs( int( (m_Price.ePrice - m_Price.sPrice))));
//	m_strBasicInfo3.Format("%s (%3.2f%%)"
//						, CommaStr(strDiffPrice)
//						, 100* (m_Price.ePrice - m_Price.sPrice) / m_Price.sPrice );

	// 20081009 �ڵ��� 8����/32���� ó�� >>
	CString strDiffPrice;
	//CString strType = GetPacketType();
	strType = GetPacketType();

	if(CDataConversion::IsBaseFormat(strType) && strType != "")
	{
		double dwDiffPrice = fabs(m_Price.ePrice - m_Price.sPrice);
		strDiffPrice = CDataConversion::GetStringThousandFormat(dwDiffPrice, strType);
	}
	else
	{
		//strDiffPrice.Format("%ld", abs( int( (m_Price.ePrice - m_Price.sPrice))));
		strDiffPrice.Format(szValFormat, fabs(m_Price.ePrice - m_Price.sPrice));
		strDiffPrice = CommaStr(strDiffPrice);
	}

	// 0, 0.00�϶� -�� �տ� �پ ������.
	if(strDiffPrice=="0" || strDiffPrice=="0.00") m_strBasicInfo2 = "";

	m_strBasicInfo3.Format("%s (%3.2f%%)", strDiffPrice,
			                               100 * (m_Price.ePrice - m_Price.sPrice) / m_Price.sPrice);
	// 20081009 �ڵ��� <<

	// 2 raw (�Ⱓ)
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//long lDate;
	__int64 lDate;
	// 2011.01.06 by SYS <<

	int a, b, c, d, e, f;	// �ð� �����͸� �и�

	// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CString strDateType = GetPacketDateType();
	int nDateUnit		= GetPacketDateUnit();

	//
	if (strDateType == _T("YYYYMMDDHHMMSS"))
	{
		int nYear1, nMon1, nDay1, nHour1, nMin1, nSec1;	// �ð� �����͸� �и�
		int nYear2, nMon2, nDay2, nHour2, nMin2, nSec2;	// �ð� �����͸� �и�

		// ������
		nYear1= m_lSDate/10000000000;	// ��
		lDate = m_lSDate%10000000000;
		nMon1 = lDate/100000000;		// ��
		lDate = lDate%100000000;
		nDay1 = lDate/1000000;			// ��
		lDate = lDate%1000000;
		nHour1= lDate/10000;			// ��
		lDate = lDate%10000;	
		nMin1 = lDate/100;				// ��
		nSec1 = lDate%100;				// ��
		
		// ������
		nYear2= m_lEDate/10000000000;	// ��
		lDate = m_lEDate%10000000000;
		nMon2 = lDate/100000000;		// ��
		lDate = lDate%100000000;	
		nDay2 = lDate/1000000;			// ��
		lDate = lDate%1000000;
		nHour2= lDate/10000;			// ��
		lDate = lDate%10000;
		nMin2 = lDate/100;				// ��
		nSec2 = lDate%100;				// ��

		switch(m_nDateType)
		{
		case 0:	// YYYYMMDD
			m_strSDate.Format("%d/%02d/%02d", nYear1,nMon1,nDay1);
			m_strEDate.Format("%d/%02d/%02d", nYear2,nMon2,nDay2);
			break;
		case 1:	// MMDDHHMM
			m_strSDate.Format("%d/%02d/%02d:%02d", nMon1,nDay1,nHour1,nMin1);
			m_strEDate.Format("%d/%02d/%02d:%02d", nMon2,nDay2,nHour2,nMin2);
			break;
		case 2:	// HHMMSS
			m_strSDate.Format("%02d:%02d:%02d", nHour1,nMin1,nSec1);
			m_strEDate.Format("%02d:%02d:%02d", nHour2,nMin2,nSec2);
			break;
		case 3:	// DDHHMMSS
			m_strSDate.Format("%d/%02d:%02d:%02d", nDay1,nHour1,nMin1,nSec1);
			m_strEDate.Format("%d/%02d:%02d:%02d", nDay2,nHour2,nMin2,nSec2);
			break;
		case 4:	//YYYYMM
			m_strSDate.Format("%d/%02d", nYear1,nMon1);
			m_strEDate.Format("%d/%02d", nYear2,nMon2);
			break;
		case 5:	// YYYYMMDDHHMMSS
			m_strSDate.Format("%d/%02d/%02d:%02d", nMon1,nDay1,nHour1,nMin1);
			m_strEDate.Format("%d/%02d/%02d:%02d", nMon2,nDay2,nHour2,nMin2);
			break;
		}
	}
	else
	// 2011.01.24 by SYS <<
	{
		switch(m_nDateType)
		{
		case 0:	// YYYYMMDD
			// ������
			lDate = m_lSDate;
			a = lDate/10000;
			lDate = lDate%10000;
			b = lDate/100;
			lDate = lDate%100;
			c = lDate;

			// ������
			lDate = m_lEDate;
			d = lDate/10000;
			lDate = lDate%10000;
			e = lDate/100;
			lDate = lDate%100;
			f = lDate;

			m_strSDate.Format("%d/%02d/%02d", a,b,c);
			m_strEDate.Format("%d/%02d/%02d", d,e,f);
			break;
		case 1:	// MMDDHHMM
			// ������
			lDate = m_lSDate;
			a = lDate/1000000;
			lDate = lDate%1000000;
			b = lDate/10000;
			lDate = lDate%10000;
			c = lDate/100;
			lDate = lDate%100;
			d = lDate;
			m_strSDate.Format("%d/%02d/%02d:%02d", a,b,c,d);

			// ������
			lDate = m_lEDate;
			a = lDate/1000000;
			lDate = lDate%1000000;
			b = lDate/10000;
			lDate = lDate%10000;
			c = lDate/100;
			lDate = lDate%100;
			d = lDate;
			m_strEDate.Format("%d/%02d/%02d:%02d", a,b,c,d);
			break;
		case 2:	// HHMMSS
			// ������
			lDate = m_lSDate;
			a = lDate/10000;
			lDate = lDate%10000;
			b = lDate/100;
			lDate = lDate%100;
			c = lDate;

			// ������
			lDate = m_lEDate;
			d = lDate/10000;
			lDate = lDate%10000;
			e = lDate/100;
			lDate = lDate%100;
			f = lDate;

			m_strSDate.Format("%02d:%02d:%02d", a,b,c);
			m_strEDate.Format("%02d:%02d:%02d", d,e,f);
			break;
		case 3:	// DDHHMMSS
			// ������
			lDate = m_lSDate;
			a = lDate/1000000;
			lDate = lDate%1000000;
			b = lDate/10000;
			lDate = lDate%10000;
			c = lDate/100;
			lDate = lDate%100;
			d = lDate;
			m_strSDate.Format("%d/%02d:%02d:%02d", a,b,c,d);

			// ������
			lDate = m_lEDate;
			a = lDate/1000000;
			lDate = lDate%1000000;
			b = lDate/10000;
			lDate = lDate%10000;
			c = lDate/100;
			lDate = lDate%100;
			d = lDate;
			m_strEDate.Format("%d/%02d:%02d:%02d", a,b,c,d);
			break;
		case 4:	//YYYYMM
			// ������
			lDate = m_lSDate;
			a = lDate/100;
			lDate = lDate%100;
			b = lDate;

			// ������
			lDate = m_lEDate;
			d = lDate/100;
			lDate = lDate%100;
			e = lDate;
			lDate = lDate;

			m_strSDate.Format("%d/%02d", a,b);
			m_strEDate.Format("%d/%02d", d,e);
			break;

		}
	}
	
	m_strPeriod = m_strSDate + _T("~") + m_strEDate;
	
}

//////////////////////////////////////////////////////////////////////////
// ������ �ؽ�Ʈ�� �޸� �ֱ� 123456 --> 123,456
CString CPeriodObj::CommaStr(CString in)
{
	int NumOfComma, nPoint, nUnderPoint = 0, nLength;
	CString out = in;

	nPoint = out.Find(".",0);	// �Ҽ��� ��ġ
	if(nPoint >= 0)	nUnderPoint = out.GetLength() - nPoint;
	nLength = out.GetLength() - nUnderPoint;

	NumOfComma = (nLength - ((in.Left(1) == "-")?1:0))/ 3;	// �����ؾ��� �ĸ� �� ���.���� ���
	if((in.GetLength() - nUnderPoint - ((in.Left(1) == "-")?1:0))% 3 == 0)NumOfComma--;	// �Ҽ��� ���� ���, ���� ��� 1 ����
	for(int i=0;i<NumOfComma;i++)
	{
		out.Insert(nLength - (3*i+3), ",");
	}
	return out;
}

bool CPeriodObj::GetPacketTypeInfo(int& nDecimalDownCount, CString& strType) const
{
	if( !m_pChartPeriodSummaryAddInImp) return false;
	CString strPacketType = "";
	
	IPacketListManager *pIPacketListManager = m_pChartPeriodSummaryAddInImp->GetPacketListManager();
	if( pIPacketListManager)
	{
		IPacketManager *pIPacketManager = pIPacketListManager->GetPacketManager( m_strRQ);
		if( pIPacketManager)
		{
			IPacket *pIPacket = pIPacketManager->GetPacket( _MTEXT(C0_CLOSE));
			if( pIPacket)
			{
				ILPCSTR szType = pIPacket->GetType(false);
				strPacketType = szType;
				pIPacket->Release();
			}
			pIPacketManager->Release();
		}
		pIPacketListManager->Release();
	}
	if( strPacketType.IsEmpty() || strPacketType.Find(".") < 0)	return false;
	
	// 20080910 JS.Kim		8����/32���� ó��, �Ҽ��� ���� �ڸ��� �ִ��ڸ��� Return <<
	if( CDataConversion::IsBaseFormat(strPacketType) )
	{
		nDecimalDownCount = 0;
		strType = strPacketType;
		int nBase, nDecLen1, nDecLen2;
		if( CDataConversion::GetBaseFormatInfo(strPacketType, nBase, nDecLen1, nDecLen2) )
			nDecimalDownCount = nDecLen1 + nDecLen2;
		return true;
	}
	//>>
	
	CDataConversion::GetStringData(strPacketType, ".");
	CString strRight = strPacketType.Right(1);
	int nRight = atoi(strRight);
	if(nRight == 0 && strRight != "0")
		strType = strRight;
	
	strPacketType.TrimRight(strType);
	strPacketType.TrimRight("0");
	nDecimalDownCount = strPacketType.GetLength();
	return true;
}

CString CPeriodObj::GetText_Price( double dPrice, const CString &p_strPacketType, int nDecimalDownCount) const
{
	CString strCurPrice;
	
	if( CDataConversion::IsBaseFormat( p_strPacketType) )
	{
		strCurPrice = CDataConversion::DoubleToString( dPrice, p_strPacketType);
	}
	else
	{
		strCurPrice = CDataConversion::DoubleToString( dPrice, nDecimalDownCount);
		
		if( p_strPacketType.Find( '.') < 0)
		{
			CString strPacketType( p_strPacketType);
			int nFactor = strPacketType.Replace( '0', '_');
			strCurPrice = strCurPrice.Left( strCurPrice.GetLength() - nFactor);
		}
	}
	
	return CDataConversion::GetStringThousandFormat( strCurPrice);
	//>>
}