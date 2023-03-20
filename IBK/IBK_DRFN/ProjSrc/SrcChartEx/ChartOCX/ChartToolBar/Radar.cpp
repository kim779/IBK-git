// Radar.cpp : implementation file
//

#include "stdafx.h"
#include "Radar.h"

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl

#include "Conversion.h"
#include "ToolBarDlg.h"

/*
#include "../ChartObject/PacketRQ.h"
#include "../Include_Chart/DLL_Load/PacketListManager.h"
#include "../ChartObject/PacketListManagerImplementation.h"
#include "../Include_Chart/DLL_Load/XScaleManager.h"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadar

CRadar::CRadar( IChartCtrl *p_pIChartCtrl)
{
	m_strDataName = "";
	m_pPacketList = NULL;

	// (2003.12.03, ��¿�) Zoom & Scroll ���� ����� �����Ѵ�.
	m_dXScale			= 1;
	m_dYScale			= 1;
	m_nDataStartIndex	= 0;
	m_nOnePageDataCount	= 1;
	m_nRadarDataLeft	= 1;
	m_nRadarDataRight	= -1;
	m_dRadarDataMin		= 0;
	m_dRadarDataMax		= 0;

	// (2003.12.03) Zoom Full Mode Flag�� �����Ͽ�,
	//		�ش� Mode������ Mouse Tracing���� �۾��� �����Ѵ�.
	m_bZoomFullMode		= FALSE;

	// (2003.12.04, ��¿�) ���� ���� ����� �߰��Ѵ�. Drag Select ������ �ʱ�ȭ�Ѵ�.
	//		(Dragging ������ ��Ÿ���� �¿� ������ �ʱ�ȭ�Ѵ�.)
	m_nDragSelectStart	= 0;
	m_nDragSelectEnd	= 0;

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	m_pIChartCtrl = p_pIChartCtrl;
}


BEGIN_MESSAGE_MAP(CRadar, CWnd)
	//{{AFX_MSG_MAP(CRadar)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadar message handlers

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.02
//
//	�м�	:	Radar Chart�� ����Ѵ�.
//
//	����	:	(2003.12.03) ����,�ɼ� Data�� ������ Mapping�� �� �ֵ��� 100��� Data�� ó���Ѵ�.
//				(2003.12.03) ���ʿ��� Drawing�� ���̱� ���Ͽ�, Index�� �ǳʶڴ�.
//				(2003.12.04, ��¿�, ����) LButtonDown�ó� Dragging�� ���� Client ������ ��츶��,
//					Drawing�� �ٽ� �߻��ϴ� ���� �����ϱ� ���Ͽ� CButton�� �ƴ϶� CWnd���� ��¹޴´�.
//					Drawing�� DrawItem()�� �ƴ϶�, OnPaint���� ó���Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CRadar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	// 0. ���� ������ Data Packet Name�� Packet List Pointer�� Ȯ���Ѵ�.
	if( m_strDataName.IsEmpty() || m_pPacketList == NULL)	return;

	// 1. ����� Packet Object�� ���Ѵ�.	
	CPacket* pPacket = m_pPacketList->GetBaseData( m_strDataName);
	if( !pPacket) return;

	// 2. ���� �������� Graph�� ����� ó���Ѵ�.
	// 2.1 �⺻ ����� ó���Ѵ�.
	dc.FillSolidRect( m_rctRadarClient, RGB( 255, 255, 255));
	dc.DrawEdge( ( LPRECT)m_rctRadarClient, EDGE_SUNKEN, BF_RECT);

	// 3. Graph�� �׸� Data�� ���� ��� ó���� �����Ѵ�.
	CList<double, double>* datalist = pPacket->GetnumericDataList();
	POSITION pos = datalist->GetHeadPosition();
	if( !pos) return;

	// 4. ���� ������ ����� ó���Ѵ�. (Data�� ���� ������ ���� �����ȴ�.)
	if( m_rctRadarSelect.left < m_rctRadarSelect.right)
		dc.FillSolidRect( m_rctRadarSelect, RGB(239, 231, 222));

	// 5. Graph�� ó���Ѵ�.
	SIZE sizOldViewportExt;
	POINT ptOldWindowOrg, ptOldViewportOrg;
	// 5.0 Graph�� �׸� Pen�� ����/�����Ѵ�.
	CPen pen( PS_SOLID, 1, RGB( 218, 46, 46));
	CPen* pOldPen = dc.SelectObject(&pen);
	// 5.1 Mapping Mode�� �����Ѵ�.
	HDC hDC = dc.GetSafeHdc();
	int nPreMapMode = SetMapMode( hDC, MM_ANISOTROPIC);
	SetWindowOrgEx( hDC, m_nRadarDataLeft, ( int)m_dRadarDataMin, &ptOldWindowOrg);
	SetViewportOrgEx( hDC, m_rgnRadarGraph.left, m_rgnRadarGraph.bottom, &ptOldViewportOrg);
	SetViewportExtEx( hDC, 1, -1, &sizOldViewportExt);

// (2003.12.03, ��¿�, Debug) Mapping ��ǥ�� ��Ȯ�� Full Rect�� Drawing�� �����Ͽ��� �Ѵ�.
// (2003.12.03, ��¿�) X Scale�� ������ Index�� �ٸ� �Ͱ� ������ ���� ������ �Ѵ�.
//		Right�� ���Ҷ� �� m_nRadarDataRight+1�� ��ȯ�� �ٽ� 1�� ���������� Ȯ���� �����Ͽ��� �Ѵ�.
//	int nLeft	= m_nRadarDataLeft;
//	int nTop	= int( m_dRadarDataMin + 0.5);
//	int nRight	= int( ( m_nRadarDataRight + 1 - m_nRadarDataLeft) * m_dXScale + 0.5) + m_nRadarDataLeft - 1;
//	int nBottom	= int( ( m_dRadarDataMax - m_dRadarDataMin) * m_dYScale + 0.5 + m_dRadarDataMin);
// 0. ���� �� Test�ÿ��� �ݵ��� ������ Scale Factor�� 0�� �ƴϾ�� �������� �׸��� ��Ÿ����. (m_dXScale, m_dYScale)
// 1. ���� Rectangle�� �Ʒ��� LineTo�� ������ �ٸ��� ��Ȯ�� �����Ͽ��� �Ѵ�.
//	dc.Rectangle( nLeft, nTop, nRight, nBottom);
// 2. Rectangle�� Width(), Height() ���� ������ �ǹ��ϴ��� �˾ƾ� �ϰ�,
//		�Ժη� �̿����� ���ƾ� �Ѵ�. (Rectangle�� �ƴ� ������ Region���� ����ߴ�.)
//		���� LineTo�� ��� ������ Point���� ĥ���� �ʴ� ���� �����Ѵ�.
//	dc.MoveTo( nLeft, nTop);
//	dc.LineTo( nLeft, nBottom);
//	dc.LineTo( nRight, nBottom);
//	dc.LineTo( nRight, nTop);
//	dc.LineTo( nLeft, nTop);

	// 5.2 Graph�� ����Ѵ�.
	// 5.2.1 Data�� �� �� ������ ��츦 ó���Ѵ�.
	//	(Scale Factor ���� 0�� ���� �� �� ���� �߽ɿ� ������ �׸���.)
	if( m_nRadarDataRight <= m_nRadarDataLeft)
	{
		// 5.2.1.1 ù Point ��ġ�� �̵��Ѵ�.
		// (2003.12.03) ����,�ɼ� Data�� ������ Mapping�� �� �ֵ��� 100��� Data�� ó���Ѵ�.
		double data = datalist->GetAt(pos) * 100;
		int nOneValue = int( ( m_rgnRadarGraph.bottom - m_rgnRadarGraph.top + 1) / 2 + m_dRadarDataMin);
		dc.MoveTo( m_rgnRadarGraph.left, nOneValue);
		// 5.2.1.2 �� Point ��ġ�� ���� �׸���. ������ Line�� Graph���� ���� �굵�� ���� X ��ǥ�� Line�� ���� �׸���.
		//	(LineTo�� ������ Point�� ĥ���� �ʴ´�. Reference ����)
		dc.LineTo( m_rgnRadarGraph.right + 1, nOneValue);
	}
	// 5.2.2 Data�� �� �� �̻��� ��츦 ó���Ѵ�.
	else 
	{
		// 5.2.2.1 ù Point ��ġ�� �̵��Ѵ�.
		int i = m_nRadarDataLeft;
		int nXOldValue = i;
		// (2003.12.03) ����,�ɼ� Data�� ������ Mapping�� �� �ֵ��� 100��� Data�� ó���Ѵ�.
		dc.MoveTo( int( ( i++ - m_nRadarDataLeft) * m_dXScale + 0.5) + m_nRadarDataLeft,
			int( ( datalist->GetNext(pos) * 100 - m_dRadarDataMin) * m_dYScale + 0.5 + m_dRadarDataMin));
		// 5.2.2.2 Data �κ��� �̵��ϸ鼭 Line�� �׸���.
		double dPacketValue;
		int nYValue, nXValue; 
		while( i <= m_nRadarDataRight)
		{
			if(pos == NULL) break;

			dPacketValue = datalist->GetNext(pos);

			// (2003.12.03) ���ʿ��� Drawing�� ���̱� ���Ͽ�, Index�� �ǳʶڴ�.
			nXValue = int( ( i - m_nRadarDataLeft) * m_dXScale + 0.5) + m_nRadarDataLeft;
			if( nXOldValue != nXValue)
			{
				nXOldValue = nXValue;

				// (2003.12.03) ����,�ɼ� Data�� ������ Mapping�� �� �ֵ��� 100��� Data�� ó���Ѵ�.
				nYValue = int( ( dPacketValue * 100 - m_dRadarDataMin) * m_dYScale + 0.5 + m_dRadarDataMin);
				dc.LineTo( nXValue, nYValue);
			}

			i++;
		}
		// 5.2.2.3 ������ Line�� Graph���� ���� �굵�� ���� X ��ǥ�� Line�� ���� �׸���.
		//	(LineTo�� ������ Point�� ĥ���� �ʴ´�. Reference ����)
		// (2003.12.03, ��¿�) X Scale�� ������ Index�� �ٸ� �Ͱ� ������ ���� ������ �Ѵ�.
		//		(1 Pxcel�� �ƴ϶� 1 Data ������ ��ġ���� �׸���. ���� LineTo�ϱ� ���� Data�� ���� �׷����� �ʴ´�.)
		dc.LineTo( int( ( m_nRadarDataRight + 1 - m_nRadarDataLeft) * m_dXScale + 0.5) + m_nRadarDataLeft, 
			nYValue);
	}

	// 6. ������������ �ƴ� ��츦 �����Ѵ�.
	if( m_nDragSelectStart == 0 && m_nDragSelectEnd == 0)
	{
		// 6.1 ������ �۾����� �ʱ��� Pen���� �����Ѵ�.
		dc.SelectObject(pOldPen);
		return;
	}

	// (2003.12.04, ��¿�) ���� ���� ����� �߰��Ѵ�.
	//		�������� ó���߿��� Real�� ������ ����������, Ư���� ����� RePaint�� ����Ͽ� ��������� �����Ѵ�.
	// 6.1 Mapping Mode�� �����Ѵ�.
	SetMapMode( hDC, nPreMapMode);
	SetWindowOrgEx( hDC, ptOldWindowOrg.x, ptOldWindowOrg.y, NULL);
	SetViewportOrgEx( hDC, ptOldViewportOrg.x, ptOldViewportOrg.y, NULL);
	SetViewportExtEx( hDC, sizOldViewportExt.cx, sizOldViewportExt.cy, NULL);
	// 6.2 Dragging ������ ����Ѵ�.
	DrawDragSelectRectangle( &dc, m_nDragSelectStart, m_nDragSelectEnd);

	// 7. �ʱ��� Pen���� �����Ѵ�.
	dc.SelectObject(pOldPen);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.03
//
//	����	:	Data View ������ ���޹޾� Radar Chart�� �ݿ��ϴ� Interface�� �����Ѵ�.
//
//	����	:	1. Data View�� ���� �Ӹ��ƴ϶� Drawing�� �ʿ��� �⺻ Data�� �����Ѵ�.
//
//	����	:	(2003.12.03) X Scale�� ������ Index�� �ٸ� �Ͱ� ������ ���� ������ �Ѵ�.
//				(2003.12.03) ����,�ɼ� Data�� ������ Mapping�� �� �ֵ��� 100��� Data�� ó���Ѵ�.
//				(2003.12.03) Zoom Full Mode ���ο� ������ Data View ������ �Բ� ���޹޾� ó���Ѵ�.
//					(Radar�� ��� Zoom Full Mode���� Data�� ���������� �ݿ��Ѵ�.)
//				(2003.12.04) ���� ���� ���� ��쿡�� Real�� �ݿ����� �ʴ´�.
//					���� ������ ReDraw�� �Ͽ� �����ָ������, OnPaint�� DC���� XOR_PEN ó���� �� �ȵǴ� �Ͱ���,
//					���� ������� ���� ���� ��Ȯ���� ���Ͽ� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CRadar::SetDataViewInfo( int nDataSize, int nDataStartIndex, int nOnePageDataCount, BOOL p_bZoomFullMode)
{
	// (2003.12.04) ���� ���� ���� ��쿡�� Real�� �ݿ����� �ʴ´�.
	if( m_nDragSelectStart != 0 || m_nDragSelectEnd != 0) return;

	// (2003.12.03) Zoom Full Mode ���ο� ������ Data View ������ �Բ� ���޹޾� ó���Ѵ�.
	//		(Radar�� ��� Zoom Full Mode���� Data�� ���������� �ݿ��Ѵ�.)
	if( p_bZoomFullMode)
	{
		nOnePageDataCount = nDataSize;
		// ����(04/11/05) Zero Base�� �����Ѵ�.
		nDataStartIndex = 0;
	}

	// 0. Data View�� ������ �����Ѵ�.
	m_nDataStartIndex = nDataStartIndex;
	m_nOnePageDataCount = nOnePageDataCount;
	// 2. Graph Data�� Min/Max �� Index ������ �����Ѵ�.
	m_nRadarDataLeft	= ( nDataSize == 0 ? 0 : 1);
	m_nRadarDataRight	= nDataSize;
	m_dRadarDataMin		= m_dRadarDataMax	= 0;

	if( m_pPacketList && nDataSize)
	{
		CPacket *pPacket = m_pPacketList->GetBaseData(m_strDataName);
		// (����, GetMinMax�� 0 Base Index�� �̿��Ѵ�.)
		// (2003.12.03) ����,�ɼ� Data�� ������ Mapping�� �� �ֵ��� 100��� Data�� ó���Ѵ�.
		if( pPacket)
		{
			pPacket->GetMinMax( 0, nDataSize - 1, m_dRadarDataMin, m_dRadarDataMax);
			m_dRadarDataMin *= 100;
			m_dRadarDataMax *= 100;
		}
	}
/*
	if(m_pPacketListManager && nDataSize)
	{
		int nRQ, nCountOfRQ;
		CPacketList* ppacketList;
		double dRadarDataMin, dRadarDataMax;

		MAP_RQ_PACKETLIST* pmapPacketList = (MAP_RQ_PACKETLIST*)m_ppacketListManager->GetMapPacketList();
		nCountOfRQ = pmapPacketList->size();

		MAP_RQ_PACKETLIST::iterator itRQ = pmapPacketList->begin();

		while(itRQ != pmapPacketList->end())
		{
			ppacketList = itRQ->second->GetPacketList();
			ppacketList->GetMinMax(0, nDataSize - 1, dRadarDataMin, dRadarDataMax);

			if(m_dRadarDataMin > dRadarDataMin) m_dRadarDataMin = dRadarDataMin;
			if(m_dRadarDataMax < dRadarDataMax) m_dRadarDataMax = dRadarDataMax;
		}

		m_dRadarDataMin *= 100;
		m_dRadarDataMax *= 100;
	}
*/
	// (2004.04.16, ��¿�) Radar�� Resize�ÿ���, Resize�� Graph�� ���̵���,
	//		Radar Graph�� �籸���ϴ� Routine�� ������ �����Ѵ�.
	DrawNewRadarView();

	// (2003.12.03) Zoom Full Mode Flag�� �����Ͽ�,
	//		�ش� Mode������ Mouse Tracing���� �۾��� �����Ѵ�.
	m_bZoomFullMode = p_bZoomFullMode;

	// (2003.12.03) Data View�� ������ �缳���� ��� Radar Status Static����,
	//		�߽� Index�� ���� �����ϵ��� ó���Ѵ�.
	OnMouseMove( NULL, m_rctRadarSelect.CenterPoint());
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.03
//
//	�м�	:	1. Radar Chart�� �ʱ⼳�� Interface�̴�.
//				2. Packet List Pointer�� ����� Packet Name ���޹޾� �����Ѵ�.
//				3. Indicator Info�� ������ ó���ϴµ� �̿�Ǿ�����, ������ �������� �ʴ� ����� ����� �����Ͽ���.
//				4. �� Data�� On Draw�ÿ� ���� Access�ϹǷ� ������ �ٲ�µ� Packet�� ��ȭ�� �־ �������� �ʴ´�.
//
//	����	:	(2003.12.04, ��¿�) ���� ������ ������ Main OCX�� Pointer�� �Բ� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CRadar::SetData(CPacketList *pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CString& strDataName)
{
	if(pPacketList == NULL || strDataName.IsEmpty())	return;

	m_strDataName	= strDataName;
	m_pPacketList	= pPacketList;
	m_pPacketListManager = pPacketListManager;
	m_pxScaleManager = pxScaleManager;
}

void CRadar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 0. ���� �ڷᰡ ���� ��츦 ó���Ѵ�.
	if( m_nRadarDataLeft == 0 && m_nRadarDataRight == 0)
	{
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	// 1. Mouse�� ��ġ�� Ȯ���Ͽ� Radar Status Static�� ��½�Ų��.
	// 1.1 Data Index�� ������ ����.
	int nDataIndex = GetDataIndex( point.x);
	// 1.3 �ش� Data Index�� �ڷ� ���ڸ� ���Ͽ� Radar Status Static�� �˷� ������ �Ѵ�.
	// 1.3.1 ���� Data�� ���Ѵ�.
	CPacket *pDateTimePacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	
	if(pDateTimePacket == NULL) return;
	if(pDateTimePacket->GetCount() == 0) return;

	CString strDataTime = CDataConversion::DoubleToString( pDateTimePacket->GetData( nDataIndex - 1));	// ���� 0 Base
	strDataTime = CDataConversion( m_pIChartCtrl->GetSafeHwnd(), m_pIChartCtrl->GetChartMode()).GetStringFormatTrans( strDataTime, pDateTimePacket->GetType(), "");
	// 1.3.2 ���� Data�� ���Ѵ�.
	CPacket *pValuePacket = m_pPacketList->GetBaseData(m_strDataName);
	CString strValue = CDataConversion::DoubleToString( pValuePacket->GetData( nDataIndex - 1));	// ���� 0 Base
	strValue = CDataConversion( m_pIChartCtrl->GetSafeHwnd(), m_pIChartCtrl->GetChartMode()).GetStringFormatTrans( strValue, pValuePacket->GetType());
	// 1.3.3 Radar�� Index ������ Radar Status Static���� �����Ѵ�.
	if( m_pstRadarStatus) m_pstRadarStatus->SetWindowText( strDataTime + "\n" + strValue);

	// (2003.12.03) Zoom Full Mode Flag�� �����Ͽ�, �ش� Mode������ Mouse Tracing���� �۾��� �����Ѵ�.
	// (2003.12.04) ���� ������ �ϴ� ���� �ƴ� ��쵵 �����Ѵ�.
	if( m_bZoomFullMode || m_nDragSelectStart == 0)
	{
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	// (2003.12.04) ���� ������ ó���� ��� �����Ѵ�.
	// 1. Dragging ������ Boxing�Ѵ�.
	//		(DrawDragSelectRectangle������ m_nDragSelectEnd�� �ֽ��� ������ Upate�ȴ�.)
	CDC *pDC = GetDC();
	DrawDragSelectRectangle( pDC, m_nDragSelectStart, point.x, &m_nDragSelectEnd);
	ReleaseDC( pDC);

	CWnd::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.04
//
//	����	:	Dragging ������ ����ϴ� Routine�� ������ �����Ѵ�.
//
//	����	:	1. ȣ�� Routine���� Start�� End�� �����Ǵ� ���� �ٷ���� ���� �ʴ´�.
//					�׷��ٰ� ���⼭ �ٷ���� �͵� ���� ����� �ƴϴ�.
//					Rectangle�� ��� �������� �����Ǵµ� �װ��� XOR�� Clear���� ���ϴ� ��찡 �߻��ϱ� �����̴�.
//					���� �׻� ������ ���ɼ��� ���� ����Ͽ� Coding�Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CRadar::DrawDragSelectRectangle( CDC *pDC, long p_nDragSelectStart, long p_nDragSelectEnd, long *p_nPreDragSelectEnd)
{
	// 0. ���� ������ ������ Dragging End�� ���� ��츦 �����Ѵ�.
	if( p_nPreDragSelectEnd && p_nDragSelectEnd == *p_nPreDragSelectEnd) return;

	// 0.5 ����, �� �̷��� �ұ�? (Pen Size�� 2�� ����� ���� m_rgnRadarGraph���� ������ ��������?
	//		���� ���⼭ Data Index�� �������� �����ϱ� ���������� �� ���ÿ����� �����ϴ� ���� �ƴϴ�.
	if( p_nDragSelectStart	== m_rgnRadarGraph.left) p_nDragSelectStart++;
	if( p_nDragSelectEnd	== m_rgnRadarGraph.left) p_nDragSelectEnd++;

	// 1. Hatch Pen, XOR Mode�� �����Ѵ�.
	//		(����, Pen Style�߿� PS_ALTERNATE�� Win9x�� �������� �ʾ� Pettern Pen���� ó���Ѵ�.
	//			���� �Ϲ� PS_DOT�� BS_HATCHED�� �� �������� 3 Pixel�̾ �����ʴ´�.)
	WORD waHatchBits[ 2] = { 0xAA, 0x55};
	CBitmap bmHatchBitmap;
	bmHatchBitmap.CreateBitmap( 8, 2, 1, 1, waHatchBits);
	LOGBRUSH lbrBrushLog = { BS_PATTERN, RGB( 0, 0, 0), ( long)( HBITMAP)bmHatchBitmap};
	// 2. �� ũ�� 1�� Pen�� ���ϴ����� 3.6 ����
	CPen pnDragPen( PS_GEOMETRIC, 1, &lbrBrushLog);
	CPen *pOldPen = pDC->SelectObject( &pnDragPen);
	pDC->SetROP2( R2_XORPEN);
	pDC->SetBrushOrg( 0, 1);
	// 3. Dragging ������ Boxing�Ѵ�.
	//		(����, Pen�� ũ�Ⱑ 2�� ��� ������ ��ǥ���� ������ �������� �� ���Ⱑ ��������,
	//			���� Reference�� �޸� ������ Point�� ĥ�ϴµ�, �ǿܷ� ������ ��� ù ������� ���Ⱑ 1�̴�.
	//			������ ���� �� �����Ͽ��� �Ѵ�.)
	// 3.1 Demo 1 (��ƴ���� �� ����. Reference�� �޸� ������ Point�� ĥ�Ѵ�.)
	//	pdc->MoveTo( p_nDragSelectStart + 1, m_rgnRadarGraph.top);
	//	pdc->LineTo( p_nDragSelectStart + 1, m_rgnRadarGraph.bottom);
	// 3.2 Demo 2 (Reference�� �޸� ������ Point�� ĥ�ϴµ�, ������� �����⸸ 1�̴�.)
	//	pdc->MoveTo( p_nDragSelectStart, m_rgnRadarGraph.bottom);
	//	pdc->LineTo( p_nDragSelectEnd, m_rgnRadarGraph.bottom);
	// 3.3 Demo 3 (��ƴ���� �� ����. Reference�� �޸� ������ Point�� ĥ�Ѵ�.)
	//	pdc->MoveTo( p_nDragSelectEnd, m_rgnRadarGraph.bottom);
	//	pdc->LineTo( p_nDragSelectEnd, m_rgnRadarGraph.top);
	// 3.4 Demo 4 (Reference�� �޸� ������ Point�� ĥ�ϴµ�, ������� �����⸸ 1�̴�.)
	//	pdc->MoveTo( p_nDragSelectEnd, m_rgnRadarGraph.top + 1);
	//	pdc->LineTo( p_nDragSelectStart, m_rgnRadarGraph.top + 1);
	// 3.5 Boxing by LineTo Demo
	//		LineTo�� �ݺ� ��뿡 ���� ��ȿ������ XOR ROP�� ���� ��Ĩ���� ������ �߰� ������ �ʿ��Ͽ�
	//		��������θ� �����Ͽ� ���´�.
	//	int nDragSelectLeft = p_nDragSelectStart + 1;
	//	int nDragSelectTop = m_rgnRadarGraph.top + 1;
	//	pdc->MoveTo( nDragSelectLeft, m_rgnRadarGraph.top);
	//	pdc->LineTo( nDragSelectLeft, m_rgnRadarGraph.bottom);
	//	pdc->LineTo( p_nDragSelectEnd, m_rgnRadarGraph.bottom);
	//	pdc->LineTo( p_nDragSelectEnd, m_rgnRadarGraph.top);
	//	pdc->MoveTo( p_nDragSelectEnd, m_rgnRadarGraph.top + 1);
	//	pdc->LineTo( p_nDragSelectStart, nDragSelectTop);
	// 3.6 Boxing by Rectangle
	//		�� OffsetRect�� �ϴ��� ��Ȯ�� �����ؾ� �Ѵ�. Region�� �ƴ϶� Rectangle�� ���� Pen ���Ⱑ 2�� ���� ������ ���̴�.
	//		Rectangle�� �̿��Ͽ��� ���� ������ ���� �ǳ� ����� 1 Pixel�� ó������ �ʴ´�.
	//		�̸� �ذ��ϱ� ���Ͽ� ũ�� 1�� Pen���� 2���� ���� Drawing�Ѵ�.
	HGDIOBJ hbrOldBrush = pDC->SelectObject( GetStockObject( NULL_BRUSH));
	// 3.6.1 ���� ������ Box�� Clear�Ѵ�.
	if( p_nPreDragSelectEnd) DrawRectangleInRegion( pDC, p_nDragSelectStart, *p_nPreDragSelectEnd);
	// 3.6.2 ���ο� ������ Boxing�Ѵ�.
	DrawRectangleInRegion( pDC, p_nDragSelectStart, p_nDragSelectEnd);

	pDC->SelectObject( hbrOldBrush);

	// 4. �ʱ��� Pen���� �����Ѵ�.
	pDC->SelectObject( pOldPen);

	// 5. ������ Dragging End�� Backup�Ѵ�.
	if( p_nPreDragSelectEnd) *p_nPreDragSelectEnd = p_nDragSelectEnd;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.04
//
//	����	:	Radar�� ������ �����ϴ� UI�� �����Ѵ�.
//
//	����	:	(2003.12.04) �ڷᰡ ���� ��쵵 �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CRadar::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	// (2003.12.03) Zoom Full Mode Flag�� �����Ͽ�, �ش� Mode������ Mouse Tracing���� �۾��� �����Ѵ�.
	// (2003.12.04) �ڷᰡ ���� ��쵵 �Բ� �����Ѵ�.
	if( m_bZoomFullMode || m_nRadarDataLeft == 0 && m_nRadarDataRight == 0)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	// 1. Mouse Message�� Capture�ϰ� Dragging Flag�� �����Ѵ�.
	SetCapture();
	// 2. Dragging ������ ���۰� ���� �ʱ�ȭ�Ѵ�.
	// 2.1 ���� �ش� Data Index�� Ȯ���Ͽ�, ��ȿ�ϵ��� �����Ѵ�.
	GetDataIndex( point.x);
	// 2.2 Dragging ������ ���۰� ���� �ʱ�ȭ�Ѵ�.
	m_nDragSelectStart = point.x;
	m_nDragSelectEnd = point.x;
	// 3. ù Dragging ������ ����Ѵ�.
	CDC *pDC = GetDC();
	DrawDragSelectRectangle( pDC, m_nDragSelectStart, m_nDragSelectEnd);
	ReleaseDC( pDC);

	CWnd::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.04
//
//	����	:	Radar�� ������ �����ϴ� UI�� �����Ѵ�.
//
//	����	:	(2003.12.04) �ڷᰡ ���� ��쵵 �����Ѵ�.
//				(2003.12.05) Radar �ۿ��� LButton�� ������ ������ ��츦 �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CRadar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// (2003.12.03) Zoom Full Mode Flag�� �����Ͽ�, �ش� Mode������ Mouse Tracing���� �۾��� �����Ѵ�.
	// (2003.12.04) �ڷᰡ ���� ��쵵 �Բ� �����Ѵ�.
	// (2003.12.05) Radar �ۿ��� LButton�� ������ ������ ��츦 �����Ѵ�.
	if( m_bZoomFullMode || ( m_nRadarDataLeft == 0 && m_nRadarDataRight == 0) || m_nDragSelectStart == 0)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	// 1. Mouse Message Capture�� �����ϰ�, Dragging Flag�� �����Ѵ�.
	ReleaseCapture();
	// 2. Dragging ������ Boxing�Ѵ�.
	//		(DrawDragSelectRectangle������ m_nDragSelectEnd�� �ֽ��� ������ Upate�ȴ�.)
	// 2.1 ���� �ش� Mouse ��ġ�� Ȯ���Ͽ�, ��ȿ�ϵ��� �����Ѵ�.
	//		(����, �� m_nDragSelectEnd�� DrawDragSelectRectangle()�� ���Ͽ� �����޴��� �ƴ°�?
	//			GetDataIndex()�� ���ǻ��� ����)
	GetDataIndex( point.x);
	// 2.2 Dragging Rectangle�� Drawing�Ѵ�.
	CDC *pDC = GetDC();
	DrawDragSelectRectangle( pDC, m_nDragSelectStart, point.x, &m_nDragSelectEnd);
	ReleaseDC( pDC);
	// 3. ������ ������ ���۰� ���� ������ �����Ѵ�.
	if( m_nDragSelectEnd < m_nDragSelectStart)
	{
		int nDragSelectEnd	= m_nDragSelectStart;
		m_nDragSelectStart	= m_nDragSelectEnd;
		m_nDragSelectEnd	= nDragSelectEnd;
	}
	// 4. ������ ������ �⺻���� Data View�� Start Index�� Width�� �����Ѵ�.
	//		(�� ������ ��û���� ����? �װ��� �̹� �����Ǿ� �ֱ� �����̴�.)
	int nDataViewStart = GetDataIndex( m_nDragSelectStart, FALSE);
	int nDataViewWidth = GetDataIndex( m_nDragSelectEnd, FALSE) - nDataViewStart + 1;
	// 5. Dragging�� �������� ���� �����Ѵ�.
	m_nDragSelectStart = 0;
	m_nDragSelectEnd = 0;
	// 6. ������ Data View�� ������ ���� ToolBar���� �����Ͽ� ������� �Ѵ�.
	// 6.1 ���� �� ȭ�� �ּ� Data���� Ȯ���Ͽ� �����Ѵ�.
	int nMinDataCountOnOnePage = ((CToolBarDlg*)GetParent())->GetCountMinPerPage();	// ����(04/11/04) OCX���� �������� ��ƾ ����.
	if( nDataViewWidth < nMinDataCountOnOnePage)
	{
		nDataViewStart -= ( nMinDataCountOnOnePage - nDataViewWidth) / 2;
		nDataViewWidth = nMinDataCountOnOnePage;
	}
	// 6.2 End Index�� Ȯ���Ͽ� �����Ѵ�.
	// (2004.09.02, ��¿�) ������ Data ������ Index�� ���ϴ� ���� �ƴϹǷ�, 1�� ����.
	int nEndNextIndex = nDataViewStart + nDataViewWidth - 1;
	if( m_nRadarDataRight < nEndNextIndex)
	{
		// (2004.09.02, ��¿�) ���� ������ �����Ѵ�. (������ '-='�� �����Ͽ�����.)
		nDataViewStart	+= m_nRadarDataRight - nEndNextIndex;
		nEndNextIndex	+= m_nRadarDataRight - nEndNextIndex;
	}
	// 6.3 Start Index�� Ȯ���Ͽ� �����Ѵ�.
	if( nDataViewStart < 1)
	{
		nEndNextIndex	+= 1 - nDataViewStart;		// ������ �� �ٲ������ ����...
		nDataViewStart	+= 1 - nDataViewStart;
	}
	// 6.4 Zero Based�� ��ȯ�Ѵ�.
	nDataViewStart--;
	nEndNextIndex--;

	// 6.5 ScrollBar���� �����Ͽ� UI�� Update��� �Ѵ�.
	( ( CToolBarDlg *)GetParent())->SetStartEndIndexToOcx( nDataViewStart, nEndNextIndex);

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	m_pIChartCtrl->OnUserZoomChanged( nEndNextIndex - nDataViewStart + 1);

	CWnd::OnLButtonUp(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.04
//
//	����	:	Mouse�� Point�� �ش��ϴ� Data Index��, Mouse Point�� �����ϸ� �����ϴ� Routine�� ���� �����Ѵ�.
//
//	����	:	1. ������ ��û���� ���� ����� ������ -1�� Return�ȴ�.
//				2. 0�� Return�Ǵ� ���� ����.
//
//	����	:	1. Index ������ �־ �������� ��� m_rgnRadarGraph.left + 1�� �����Ѵ�.
//					�װ��� Drawing�ÿ� Pen�� ���Ⱑ 2�� �Ͱ� �����Ǿ� ������ ���̴�.
//				2. ����, �̷��� ������ PointX�� ������ �ٽ� DataIndex�� �����ϸ� �������� �� ������,
//					�����ϸ� 1ȸ�� ó���Ѵ�.
///////////////////////////////////////////////////////////////////////////////
int CRadar::GetDataIndex( long &p_nPointX, BOOL p_bWithCorrection) const
{
	// 1. Point�� Graph ������ ���Դ����� ��ȿ���� �Ǵ��Ѵ�.
	//		(Graph �������̸鼭�� ������ ���� ��ȿ�� Data Index�� ���� �� �ֱ� �����̴�.)
	//		(���� ������ �����Ͽ� Data�� ��ȿ���� ���� ���, point���� �Ѱ谪���� �����ϸ�,
	//		��û���� ���� ��쿡�� -1�� �����Ѵ�.)
	int nDataIndex = 0;
	if( p_nPointX < m_rgnRadarGraph.left)
	{
		nDataIndex = ( p_bWithCorrection ? m_nRadarDataLeft : -1);
		if( p_bWithCorrection) p_nPointX = m_rgnRadarGraph.left;
	}
	else if( m_rgnRadarGraph.right < p_nPointX)
	{
		nDataIndex = ( p_bWithCorrection ? m_nRadarDataRight : -1);
		if( p_bWithCorrection) p_nPointX = m_rgnRadarGraph.right;
	}
	// 2. Data Index�� �����Ͽ� Return�Ѵ�.
	//  (����, ��� �������� ������ INTȭ ��Ű���İ� ���� �¿��Ѵ�.)
	if( nDataIndex) return nDataIndex;
	return int( ( p_nPointX - m_rgnRadarGraph.left) / m_dXScale) + m_nRadarDataLeft;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.04
//
//	����	:	���۰� �� ������ �ش��ϴ� ���ÿ����� Rectangle�� ����ϴ� Routine�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CRadar::DrawRectangleInRegion( CDC *p_pDC, long p_nDragSelectStart, long p_nDragSelectEnd) const
{
	// 1. Rectangle�� ���Ѵ�.
	//		(�־����� ���� Region�̹Ƿ� right�� bottom�� +1 ó���Ѵ�.)
	// 1.1 ��¥ right�� �����ϱ�?
	//		�׷��ٰ� ������ ���� �ٷ������� �ʴ´�.
	//		Rectangle�� ��� �������� �����Ǵµ� �װ��� XOR�� Clear���� ���ϴ� ��찡 �߻��ϱ� �����̴�.
	//		���� �׻� ������ ���ɼ��� ���� ����Ͽ� Coding�Ѵ�.
	if( p_nDragSelectStart <= p_nDragSelectEnd) p_nDragSelectEnd++;
	else p_nDragSelectStart++;
	// 1.2 Rectangle�� ���Ѵ�.
	CRect rctDragSelect( p_nDragSelectStart, m_rgnRadarGraph.top, p_nDragSelectEnd, m_rgnRadarGraph.bottom + 1);
	// 2. ���� Rectangle�� ó���Ѵ�.
	p_pDC->Rectangle( rctDragSelect);
	// 3.6.1.5 Width�� 2������ ���, DeflateRect()�� �Ƿ� ������ Ȯ���Ű�ų�, �ٷ� ���� ���� ����� �ȴ�.
	//		�� 2�ΰ�? Region�� �ƴ϶� Rectangle�̶�� ���� ������ ���̴�.
	//		abs()�� ��? Reverse�Ǵ� ����� ó���̴�.
	if( abs( rctDragSelect.Width()) > 2)
	{
		rctDragSelect.DeflateRect( 1, 1);
		p_pDC->Rectangle( rctDragSelect);
	}
}

// (2004.04.16, ��¿�) Radar�� Resize�ÿ���, Resize�� Graph�� ���̵���,
//		Radar Graph�� �籸���ϴ� Routine�� ������ �����Ѵ�.
void CRadar::DrawNewRadarView( void)
{
	// 1. Radar Chart�� Client ������ ���Ͽ� ���´�.
	GetClientRect( &m_rctRadarClient);
	// 3. Graph�� �׷����� ������ Ȯ���Ѵ�. �̶��� Region���� Right/Bottom ��ǥ�� ���Խ�Ű���� �Ѵ�.
	m_rgnRadarGraph = m_rctRadarClient;
	m_rgnRadarGraph.DeflateRect( 2, 2);
	m_rgnRadarGraph.right--;
	m_rgnRadarGraph.bottom--;
	// 4. Graph�� Scale ��ȯ ��ġ�� �̸� ���Ͽ� ���´�.
	m_dXScale = 0;
	// (2003.12.03, ��¿�) X Scale�� ������ Index�� �ٸ� �Ͱ� ������ ���� ������ �Ѵ�.
	//		if( nDataSize > 1) m_dXScale = ( m_rgnRadarGraph.right - m_rgnRadarGraph.left) / ( nDataSize - 1.);
	//		������ �� Code�� ��ü ���� Data �������� 1���� ���� �ϰ� ������,
	//		nDataSize ��° Index ��ġ (nDataSize-1������ �����)������ m_rgnRadarGraph.right���� �ϼ��ǵ��� �Ǿ� �ִ�.
	//		����, nDataSize���� �����ϰ�, nDataSize+1 ��° Index ��ġ (nDataSize������ �����)���� nDataSize��° Index������ �ѹ� �� ����ó���Ѵ�.
	//		�̶�, Graph Region�� ������ ���°� m_rgnRadarGraph.right+1������ ���� nDataSize���� �Ͽ��� �Ѵ�.
	// (2004.04.16, ��¿�) ������ nDataSize�� �̿��Ͽ��� ó���� �����ϰ� m_nRadarDataRight�� �����Ѵ�.
	if( m_nRadarDataRight > 0)
		m_dXScale = ( m_rgnRadarGraph.right + 1 - m_rgnRadarGraph.left) / ( double)m_nRadarDataRight;
	m_dYScale = 0;
	if( m_dRadarDataMax > m_dRadarDataMin)
		m_dYScale = ( m_rgnRadarGraph.bottom - m_rgnRadarGraph.top)
					/ ( m_dRadarDataMax - m_dRadarDataMin);
	// 5. Radar Chart�� ���õ� ������ Rect�� ���Ͽ� ���´�.
	// 5.1 Radar Chart�� ���� ������ ���̸� Ȯ���Ѵ�. (Radar Chart�� Border�� 2 Pixel �̴�.)
	m_rctRadarSelect = m_rctRadarClient;
	m_rctRadarSelect.DeflateRect( 0, 2);
	// 5.2 Radar Chart ���� ������ ��ġ�� ���� Ȯ���Ѵ�.
	m_rctRadarSelect.left = 0;
	m_rctRadarSelect.right = -2;
	// (2003.12.03, ��¿�) X Scale�� ������ Index�� �ٸ� �Ͱ� ������ ���� ������ �Ѵ�.
	// 5.2.1 ���� Data�� 1�� �̻��� ��츦 ó���Ѵ�.
	if( m_nRadarDataRight > 0)
	{
		m_rctRadarSelect.left	= int( ( m_nDataStartIndex - m_nRadarDataLeft)								* m_dXScale + m_rgnRadarGraph.left + 0.5);
		// (2004.09.02, ��¿�) �� Data�� 1 Pixel �̻��� ������ ���, Right(����)�� ��ġ�� m_nOnePageDataCount + 1 ���Ŀ� �־��
		//		������ Data�� ������ �����ϰ� �ȴ�.
		m_rctRadarSelect.right	= int( ( m_nDataStartIndex - m_nRadarDataLeft + m_nOnePageDataCount + 1)	* m_dXScale + m_rgnRadarGraph.left + 0.5);
	}
	// 5.2.2 Data�� ���� ��� ��ü �������� �����Ѵ�.
	else
	{
		m_rctRadarSelect.left = m_rgnRadarGraph.left;
		m_rctRadarSelect.right = m_rgnRadarGraph.right + 1;		// Graph�� Region�� ���̶����� + 1
	}

	// 5.3 ������ Data View Width�� ���� Radar Chart�� ���ÿ��� �������� �����Ѵ�.
	//		(Radar Chart���� ���������̳� �ּ� X Scale�� ���� �����̴�. Rectangle�� Region�� ���̸� �����ؾ� �Ѵ�.)
	if( m_rgnRadarGraph.right < m_rctRadarSelect.right) m_rctRadarSelect.right = m_rgnRadarGraph.right + 1;

	// 6. ������ ������ �ٷ� �ݿ��ϵ��� Invalidate��Ų��.
//	Invalidate( FALSE);
	RedrawWindow();
}

// (2004.04.16, ��¿�) ToolBar Resize�� Radar�� ũ�� ������ Graph�� �籸���� ó����� �Ѵ�.
void CRadar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	DrawNewRadarView();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnSetFocus
 - Created at  :  2005-09-10   15:49
 - Author      :  ������
 - Description :  Focus�� ������ OCX�� �ٽ� ��Ŀ���� �佺!!!
 -----------------------------------------------------------------------------------------*/
void CRadar::OnSetFocus(CWnd* pOldWnd) 
{
//	CWnd::OnSetFocus(pOldWnd);
	
	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
	
}
