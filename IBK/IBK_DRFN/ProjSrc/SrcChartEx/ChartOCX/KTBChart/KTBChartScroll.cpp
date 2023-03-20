// KTBChartScroll.cpp: implementation of the Scroll & Zoom Code.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/Conversion.h"				// for CDataConversion
#include "KTBChartCtl.h"								// for CKTBChartCtrl
#include "PacketList.h"									// for CPacketList
#include "ToolBarManager.h"								// for IToolBarManager
#include "MainBlock.h"									// for CMainBlock
#include "XScaleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.11.09, ��¿�) Scroll�� ������ ó���ϴ� Interface�� �����Ѵ�.
//		����! �����ϴ� Scroll Value�� �߸��Ǿ �� ���� �ִ�� �����ϵ��� �����ǹǷ�,
//			������ ����ó������ �ʴ´�.
BOOL CKTBChartCtrl::ScrollTo( int p_nScrollValue, BOOL p_bRelative) 
{
	// 0. Check WholeView
	if( m_bWholeView) return FALSE;

	// 1. Get Start and End DRI
	// 1.0 Get Base DRI
	int nStartDRIOfView	= p_nScrollValue;
	int nEndDRIOfView	= p_nScrollValue + m_pxScaleManager->GetDrawCount_Cur() - 1;
	// 1.1 Update Relative
	if( p_bRelative)
	{
		nStartDRIOfView	+= m_pxScaleManager->GetStartDrawIdx_Cur();
		nEndDRIOfView	+= m_pxScaleManager->GetStartDrawIdx_Cur();
	}

	// 2. Do not update managed Info.

	// 3. Update X Scale Manager
	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView);

	// 5. Update View
	InvalidateControl();

	return TRUE;
}

// (2004.11.09, ��¿�) Zoom�� ������ ó���ϴ� Interface�� �����Ѵ�.
//		����! �����ϴ� Zoom Value�� �߸��Ǿ �� ���� �ִ�� �����ϵ��� �����ǹǷ�,
//			������ ����ó������ �ʴ´�.
BOOL CKTBChartCtrl::ZoomTo(long p_nZoomValue, BOOL p_bRelative) 
{
	if( !m_pxScaleManager) return FALSE;

	// TODO: Add your dispatch handler code here
	if( IsRunningNoZoomChart() && p_nZoomValue != m_pxScaleManager->GetCount()) return FALSE;

	// 1. Get Start and End DRI
	// 1.0 Get Base DRI
	int nStartDRIOfView	= m_pxScaleManager->GetStartDrawIdx_Cur();
	int nEndDRIOfView	= m_pxScaleManager->GetStartDrawIdx_Cur() + m_pxScaleManager->GetDrawCount_Cur() - 1;
	// 1.1 Modify DRI
	if( p_bRelative)	nStartDRIOfView	-= p_nZoomValue;
	else				nStartDRIOfView	-= ( p_nZoomValue - m_pxScaleManager->GetDrawCount_Cur());
	// 1.2 Check validation.
	int nZoomCount = ( nEndDRIOfView - nStartDRIOfView + 1);
	if( nZoomCount < m_nMinDataCountOnOnePage) nZoomCount = m_nMinDataCountOnOnePage;
	nStartDRIOfView = nEndDRIOfView - nZoomCount + 1;
	if( nStartDRIOfView < 0)
	{
		nStartDRIOfView = 0;
		nEndDRIOfView = nStartDRIOfView + nZoomCount - 1;
	}
	// (2009/2/19 - Seung-Won, Bae) If no-data, Set view range to (0:ZoomValue) for accepting of ZoomValue.
	if( 0 < m_pxScaleManager->GetCount() && m_pxScaleManager->GetCount() <= nEndDRIOfView)
	{
		nEndDRIOfView = m_pxScaleManager->GetCount() - 1;
		nStartDRIOfView = nEndDRIOfView - nZoomCount + 1;
	}
	if( nStartDRIOfView < 0) nStartDRIOfView = 0;
	if( nEndDRIOfView < 0) return FALSE;

	// 2. Update Info.
	// 2.1 Uncheck WholeView option.
	m_bWholeView = FALSE;

	// 2.2 One Page Data Count by User with fixed Item Width option.
	m_nOnePageDataCount = nZoomCount;

	// 3. Update X Scale Manager
	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView, p_bRelative);

	// (2009/6/2 - Seung-Won, Bae) Zoom to adjusted zoom value on relative.
	if( p_bRelative) m_nOnePageDataCount = m_pxScaleManager->GetDrawCount_Cur();

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	//>>JS.Kim_20101001. ����� ��ũ�� ���۽ÿ��� MultiChart�� �̺�Ʈ �ش�.
	if( p_bRelative )
	//<<
		OnUserZoomChanged( m_nOnePageDataCount);

	// 5. Update View
	InvalidateControl();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2009/1/18 - Seung-Won, Bae) Zoom In/Out
//////////////////////////////////////////////////////////////////////
// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
void CKTBChartCtrl::DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd)
{
	// 1. Get Start and End DRI
	int nStartDRIOfView = -1;
	int nEndDRIOfView = -1;
	// 1.1 Zoom out to Full View
	if( p_ptEnd.x < p_ptStart.x)
	{
		nStartDRIOfView		= 0;
		nEndDRIOfView		= m_pxScaleManager->GetCount() - 1;
	}
	// 1.3 X Scale Manager Version.
	else
	{
		nStartDRIOfView		= m_pxScaleManager->GetDRInFromPt( p_ptStart.x);
		nEndDRIOfView		= m_pxScaleManager->GetDRInFromPt( p_ptEnd.x);
	}
	// 1.4 Check reverse state.
	if( nEndDRIOfView < nStartDRIOfView)
	{
		int nDRI		= nStartDRIOfView;
		nStartDRIOfView	= nEndDRIOfView;
		nEndDRIOfView	= nDRI;
	}

	// 1.5 Check minimum data count on page.
	if( nEndDRIOfView - nStartDRIOfView + 1 < m_nMinDataCountOnOnePage)
	{
		// (2009/5/18 - Seung-Won, Bae) Zoom to min
		nEndDRIOfView += ( m_nMinDataCountOnOnePage - ( nEndDRIOfView - nStartDRIOfView + 1)) / 2;
		nStartDRIOfView = nEndDRIOfView - m_nMinDataCountOnOnePage + 1;
	}

	// 2. Update Info.
	// 2.1 Uncheck WholeView option.
	m_bWholeView = FALSE;
	// 2.2 One Page Data Count by User with fixed Item Width option.
	m_nOnePageDataCount = nEndDRIOfView - nStartDRIOfView + 1;

	// 3. Update X Scale Manager
	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView);

	// 5. Update View
	InvalidateControl();

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	OnUserZoomChanged( m_nOnePageDataCount);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseWheel
 - Created at  :  2005-02-23   15:07
 - Author      :  ������
 - Description :  ���콺 ���� �����϶� 3���� ������Ÿ�� �̵���Ų��.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int nScrollValue = 5;
	if( zDelta < 0 && !m_pxScaleManager->GetReverse()
		|| 0 < zDelta && m_pxScaleManager->GetReverse())
			nScrollValue = -5;

	if(m_nMouseWheelIndex == 0)
	{
		if( nFlags & MK_SHIFT)	ZoomTo( -nScrollValue, TRUE);
		else					ScrollTo( nScrollValue, TRUE);
	}
	else
	{
		if( nFlags & MK_SHIFT)	ScrollTo( nScrollValue, TRUE);
		else					ZoomTo( -nScrollValue, TRUE);
	}

	return COleControl::OnMouseWheel(nFlags, zDelta, pt);
}

void CKTBChartCtrl::SetOnePageDataCount(long nOnePageDataCount) 
{
	// (2009/5/18 - Seung-Won, Bae) Do not zoom on whole-view.
	if( IsWholeView())
	{
		if( nOnePageDataCount < m_nMinDataCountOnOnePage) nOnePageDataCount = m_nMinDataCountOnOnePage;
		if( m_pxScaleManager->GetCount() <= nOnePageDataCount)	nOnePageDataCount = m_pxScaleManager->GetCount();
		m_nOnePageDataCount = nOnePageDataCount;
	}
	else ZoomTo( nOnePageDataCount, FALSE);
}

void CKTBChartCtrl::SetWholeView(const BOOL bWholeView)
{
	if( m_bWholeView && bWholeView
		|| !m_bWholeView && !bWholeView) return;

	// 1. Get Start and End DRI
	int nStartDRIOfView = 0;
	int nEndDRIOfView = m_pxScaleManager->GetCount() - 1;
	if( !bWholeView) nStartDRIOfView = nEndDRIOfView - m_nOnePageDataCount + 1;

	// 2. Update Info.
	// 2.1 Update WholeView option.
	m_bWholeView = bWholeView;

	// 3. Update X Scale Manager
	m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView);

	// 5. Update View
	InvalidateControl();

	// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
	OnUserZoomChanged( m_nOnePageDataCount);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, ��¿�, �м�) Zoom Value�� ��ȸ �� �����ϴ� Property & Method�̴�.
//////////////////////////////////////////////////////////////////////
void CKTBChartCtrl::OnnOnePageDataCountChanged() 
{
	BoundPropertyChanged(dispidNOnePageDataCount);

	//sy 2005.11.17. �Ӽ����� �ٲٴ°��� ȭ�鿡 �ݿ��� ���� �ʴ´�!!
	//-> file ����� page count �� �ٲ㼭 �����ϴ� ��찡 �ֱ� ������ ��ũ�ѿ� �ݿ��ϸ� ȭ���� �޻��ϰ� ������.
	//SetScrollData(SETSCROLL_TR, true, true);

	SetModifiedFlag();
}

//sy 2005.11.02.
void CKTBChartCtrl::SetOnePageDataCount2(long nOnePageDataCount, BOOL bIncludeBongCountMargin) 
{
	m_bIncludeBongCountMargin = (bIncludeBongCountMargin ? true: false);

	SetOnePageDataCount(nOnePageDataCount);
}
//sy end

// ��ȭ�鿡 ���̴� ����Ÿ���� �����Ѵ�
// 02.18.2002
long CKTBChartCtrl::GetOnePageDataCount() 
{
	return m_nOnePageDataCount;
}
// (2009/5/18 - Seung-Won, Bae) Do not use one page data count and total data count.
//		for MultiChart.
long CKTBChartCtrl::GetNCurrentDrawCount() 
{
	// TODO: Add your property handler here
	if( m_pxScaleManager) return m_pxScaleManager->GetDrawCount_Cur();
	return m_nOnePageDataCount;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, ��¿�, �м�) Print�� ���� Scrolling�� ó���ϴ� Routine�̴�.
//		�׷��� ToolBar�� �ִ� ���� ���ѵ� Old Version�̾ 
//		ToolBar�� ���� ȭ���� ��� ��¿� ������ ���� �� �ִ�.
//////////////////////////////////////////////////////////////////////
BOOL CKTBChartCtrl::SetScrollPosToNextPage( BOOL p_bBackward)
{
	int nScrollPos = m_pxScaleManager->GetStartDrawIdx_Cur();
	int nPrevScrollPos = nScrollPos;
	int nDrawCount = m_pxScaleManager->GetDrawCount_Cur();

	if( p_bBackward)	nScrollPos -= nDrawCount;
	else				nScrollPos += nDrawCount;
	ScrollTo( nScrollPos, FALSE);

	int nNextScrollPos = m_pxScaleManager->GetStartDrawIdx_Cur();
	return nNextScrollPos != nPrevScrollPos;
}
int CKTBChartCtrl::GetCurrentScrollPos()
{
	return m_pxScaleManager->GetStartDrawIdx_Cur();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, ��¿�, �м�) ���� ���� Scroll Info ��ȸ Interface(Method)�̴�.
//////////////////////////////////////////////////////////////////////

// (2004.06.15, ��¿�) Scrolling�� ó���ϴ� Interface�� �����Ѵ�.
// 0. �ڷ����ڷ� Scrolling�� ó���ϴ� Interface�� �����Ѵ�.
BOOL CKTBChartCtrl::GetStartEndDateOnView(double FAR* p_pStartDate, double FAR* p_pEndDate) 
{
	// TODO: Add your dispatch handler code here
	
	// 0. Pointer�� Ȯ���Ѵ�.
	if( !p_pStartDate || !p_pEndDate || !m_pPacketList) return FALSE;

	// 1. ���� Start/End Index�� ���Ѵ�. (���� 04/09/20: 0 Based)
	int nStartIndex, nEndIndex;
	if( !GetDataStartEnd( nStartIndex, nEndIndex)) return FALSE;

	// 2. �� Index�� �ش��ϴ� Data�� �����Ѵ�.
	// 2.1 �ڷ����� Packet�� ���Ѵ�.
	CPacket *pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return FALSE;
	// 2.2 '�ڷ�����'�κ��� �ش� Index�� Data�� ���Ѵ�. (0 Based)
	*p_pStartDate	= pPacket->GetData( nStartIndex);
    *p_pEndDate		= pPacket->GetData( nEndIndex);

	// (2004.07.06, ��¿�) �˻��� �ڷ����ڰ� ��ȿ���� �ʴ� ���,
	//		������ Data�� �ڷ����ڷ� ó���Ѵ�.
	if( *p_pEndDate == -DBL_MAX)	// �ϳ��� ��ȿ���� �ʴ°�?
	{
		// 3.0 ���� ������ Data�� Update�ϱ� ���� ������ ���ڸ� Ȯ���Ѵ�.
		int nDataCount = pPacket->GetCount();
		double dLastDate = 0;
		if( 1 <= nDataCount) dLastDate = pPacket->GetData( nDataCount - 1);
		
		// 3.1 End Date���� Ȯ���Ѵ�.
		*p_pEndDate = dLastDate;

		// 3.2 Start Date�� ���� ��츦 ó���Ѵ�.
		if( *p_pStartDate == -DBL_MAX) *p_pStartDate = dLastDate;
	}

	return TRUE;
}

// (2004.11.09, ��¿�, �м�) ���� ���� Scroll Info ���� Interface�̴�.
BOOL CKTBChartCtrl::SetStartEndDateOfView(double p_dStartDate, double p_dEndDate) 
{
	// TODO: Add your dispatch handler code here

	// 0. Pointer�� Ȯ���Ѵ�.
	if( !m_pPacketList) return FALSE;

	// 1. �־��� ���� ������ �̿��Ͽ� Data Index�� ���Ѵ�.
	// 1.1 ���� �ڷ����� Packet�� ���Ѵ�.
	CPacket *pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return FALSE;
	// 1.2 �� �ڷ����ڿ� ���� Data Index�� ���Ѵ�. (0 Based Index)
	int nStartIndex	= pPacket->GetIndexFromData( p_dStartDate);
	int nEndIndex	= pPacket->GetIndexFromData( p_dEndDate);
	if( nStartIndex < 0 || nEndIndex < 0) return FALSE;

	// 2. ������ Data Index�� �������� Scrolling�� ó���Ѵ�. (���� 04/09/20: 0 Based Index)
	SetStartEndIndex( nStartIndex, nEndIndex);

	return TRUE;
}

// (2004.11.09, ��¿�, �м�) ���� ���� Scroll Info ���� Interface�̴�.
// (2004.06.17, ��¿�) �־��� �ڷ����ڸ� �������� Scroll �ϴ� Interface�� �����Ѵ�.
// (2004.06.28, ��¿�) �־��� �ڷ����ڴ� View�� ���������� �����ǵ��� �����Ѵ�.
BOOL CKTBChartCtrl::ScrollToDataOfEnd(double p_dData) 
{
	// TODO: Add your dispatch handler code here

	// 0. Pointer�� Ȯ���Ѵ�.
	if( !m_pPacketList) return FALSE;

	// 1. ���� ������ �� ȭ�鿡 ���̴� Data ���� ���Ѵ�.
	// 1.1 ���� ���̴� Data�� Start/End Index�� ���Ѵ�. (���� 04/09/20: 0 Based)
	int nStartIndex, nEndIndex;
	if( !GetDataStartEnd( nStartIndex, nEndIndex)) return FALSE;
	// 1.2 �� ȭ�鿡 ���̴� Data���� ���Ѵ�.
	int nPageDataCount = nEndIndex - nStartIndex + 1;

	// 2. �־��� ���� ������ �̿��Ͽ� Data Index�� ���Ѵ�.
	// 2.1 ���� �ڷ����� Packet�� ���Ѵ�.
	CPacket *pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return FALSE;
	// (2004.06.28, ��¿�) �־��� �ڷ����ڴ� View�� ���������� �����ǵ��� �����Ѵ�.
	// 2.2 �� �ڷ����ڿ� ���� Data Index�� ���Ѵ�. (0 Based Index)
	nEndIndex = pPacket->GetIndexFromData( p_dData);
	if( nEndIndex < 0) return FALSE;
//	nEndIndex++; // ����(04/09/20) 0-Base
	// (2004.07.06, ��¿�) ������ Data Index�� ȭ�鿡 ���̴� ������ �ƴ� ��츦 �����Ѵ�.
	// 2.2.5 ��ü ScrollRange�� ���Ѵ�.
	int nFullRange;
	if( !GetScrollRange( nFullRange)) return FALSE;
	// 2.2.6 ��ü ScrollRange���� ������ ���, False�� return�Ѵ�.
	if( nFullRange < nEndIndex) return FALSE;
	// 2.3 ���� �ڷ����ڸ� ���Ѵ�.
	nStartIndex	= nEndIndex - nPageDataCount + 1;

	// 3. �� ȭ�鿡 ���̴� Data���� �����Ͽ� nStartIndex�� �����Ѵ�.
	// 3.2 ���ο� StartIndex�� ���� EndIndex�� OverFlow�� Ȯ���Ͽ� �����Ѵ�.
	if( nFullRange < nStartIndex + nPageDataCount - 1) nStartIndex = nFullRange - nPageDataCount + 1;
	// 3.3 ���ο� StartIndex�� ���� EndIndex�� �����Ѵ�.
	nEndIndex = nStartIndex + nPageDataCount - 1;

	// 4. ������ Data Index�� �������� Scrolling�� ó���Ѵ�. (���� 04/09/20: 0 Based Index)
	SetStartEndIndex( nStartIndex, nEndIndex);

	return TRUE;
}

//sy 2005.11.17. -> ������ ���� ���� �������� ���� ���. 
int	CKTBChartCtrl::GetOnePageDataCountForScroll() const
{
	//������ ���� ���� �������� ���� ���.
//	if(!m_bIncludeBongCountMargin && m_nBongCountMargin > 0)
//	{
//		return (m_nOnePageDataCount + m_nBongCountMargin);
//	}

	return m_nOnePageDataCount;
}

//sy 2005.06.16. Ư����Ʈ �� ��� x�� count�� Ʋ��
int CKTBChartCtrl::GetFullDataCount(const bool bIsHorzScaleDataCheck) const
{
	if(m_pPacketList == NULL)
		return 0;

	if(!bIsHorzScaleDataCheck || !IsScrollInHorzScaleData())
		return m_pPacketList->GetMaxCount();

	// Ư���� ��Ʈ�� ��츸 x�� scale data�� scroll count �����Ѵ�
	int nCount = m_pPacketList->GetCount(m_pMainBlock->GetHorzScaleData(CBlockIndex(0, 0)));
	if(nCount > 0)
		return nCount;

	return m_pPacketList->GetMaxCount();
}

bool CKTBChartCtrl::IsScrollInHorzScaleData() const
{
	if(m_pMainBlock == NULL)
		return false;

	CString strIndicatorName = m_pMainBlock->GetIndicatorName(CBlockIndex(0, 0), 0, 0);
	if(strIndicatorName.IsEmpty())
		return false;

	return g_iMetaTable.IsSpecialGroupGraph(strIndicatorName);
}
//sy end


//////////////////////////////////////////////////////////////////////
// (2004.11.09, ��¿�, �м�) Index ���� Scroll Info ��ȸ Interface�̴�.
//////////////////////////////////////////////////////////////////////

// (2004.07.02, ��¿�) MaxRange�� ��ȸ�ϴ� Interface�� �����Ѵ�. (������ Index�� ���̵ȴ�.)
BOOL CKTBChartCtrl::GetScrollRange( int &p_nRange)
{
	// 0. ó�� ȯ���� Ȯ���Ѵ�.
	if( !m_pToolBarManager || !m_pMainBlock) return FALSE;

	// 1. ���� ��ü ������ ��츦 Check �Ѵ�.
	if( IsWholeView())
	{
		int nStartIndex;
		m_pMainBlock->GetDisplayDataIndex( nStartIndex, p_nRange);
		// 1.1 GetDisplayDataIndex()�� 0 Base�� ��ȸ�Ͽ� �����Ѵ�.
		// ����(04/09/20) �̴������� Scroll�κе� 0 Base�̹Ƿ� �״�� ������.
//		p_nRange++;
		return TRUE;
	}

	// 2. ScrollBar�� ���Ͽ� Scrolling�� ó���ϴ� ��츦 Ȯ���Ѵ�.
	// ����(04/11/16) MaxRange ���ؿ��� �Լ��� �����ϹǷ� �װ� �����.
	p_nRange = GetMaxRange();
	// ����(04/09/20) �̴������� Scroll�κе� 0 Base�̹Ƿ� �Ѱ��� ����...
	p_nRange--;

	return TRUE;
}

// (2004.11.09, ��¿�, �м�) Index ���� Scroll Info ��ȸ Interface�̴�.
// 1. Data Index�� Scrolling�� �����ϴ� Routine�� �����Ѵ�. (���� 04/09/20: 0 Based Index�� ó���Ѵ�.)
BOOL CKTBChartCtrl::GetDataStartEnd( int &p_nStartIndex, int &p_nEndIndex)
{
	// 0. ó�� ȯ���� Ȯ���Ѵ�.
	if( !m_pToolBarManager || !m_pMainBlock) return FALSE;

	// 1. ���� ��ü ������ ��츦 Check �Ѵ�.
	if( IsWholeView())
	{
		m_pMainBlock->GetDisplayDataIndex( p_nStartIndex, p_nEndIndex);
		// 1.1 GetDisplayDataIndex()�� 0 Base�� ��ȸ�Ͽ� �����Ѵ�.
//		p_nStartIndex++;
//		p_nEndIndex++;
		return TRUE;
	}

	// 2. ScrollBar�� ���Ͽ� Scrolling�� ó���ϴ� ��츦 Ȯ���Ѵ�.
	// ����(04/11/16) ���ǹ� ����
	if(IsUseToolScrollBar())
	if( m_pToolBarManager->GetStartEndIndex(p_nStartIndex, p_nEndIndex))
	{
		// (2004.07.01, ��¿�) Index���� �߸� ��ȸ�Ǿ� ������ ��츦 ����Ѵ�.
		if( p_nStartIndex < 0 || p_nEndIndex < 0 || p_nEndIndex < p_nStartIndex || (p_nStartIndex == 0 && p_nEndIndex == 0))
		{
			//sy 2004.09.15.
			//-> Map�� ��ũ���� ���� ��� start, end �� 0 �� �ѱ�� ������
			//   �ùٸ� start, end�� �����ü� ����.
			//   �̶� MainBlock���� ������ �����´�.
			m_pMainBlock->GetDisplayDataIndex(p_nStartIndex, p_nEndIndex);
//			if(p_nStartIndex >= 0)
//				p_nStartIndex++;
//			if(p_nEndIndex >= 0)
//				p_nEndIndex++;
			return (p_nStartIndex < 0 || p_nEndIndex < 0 || p_nEndIndex < p_nStartIndex || (p_nStartIndex == 0 && p_nEndIndex == 0));
			//sy end
		}
		return TRUE;
	}

	// 3. ScrollBar ���� ���� ���, ���� Scrolling�� ó���Ѵ�.
	m_pMainBlock->GetDisplayDataIndex( p_nStartIndex, p_nEndIndex);
	// ����(04/09/20) ScrollBar�ʵ� 0 Base�� �ٲ�.
//	if(p_nStartIndex >= 0)
//		p_nStartIndex++;
//	if(p_nEndIndex >= 0)
//		p_nEndIndex++;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, ��¿�, �м�) Index ���� Zoom Value�� �����ϴ� Interface(Method)�̴�.
//////////////////////////////////////////////////////////////////////

// (2004.07.05, ��¿�) �� ȭ�鿡 ���̴� Data ���� ȯ��ġ�� �ƴ϶� Zoom Value�� �����ϴ� Method�� �����Ѵ�.
//	1. OnePageDataCount Property�� ������ ���, ȯ�漳�������� ��ȭ�鿡 ���̴� Data�� ��������,
//		������ �����ϸ�, ���� ���������� �ڵ� Scroll�ϰ� �Ǵµ�,
//		�̷� ����� �ƴ� ���� Zoom (��������)�� ó���ϴ� Method�� �����Ѵ�.
//		����!! : ���� ȯ�漳���� '�� ȭ�鿡 ���̴� Data ��'�� Zoom Value�� �����Ͽ� �����ش�.
//			�׷��� �װ������� ������ Zoom Value�� ����� �ƴԿ� �����Ѵ�.
//	2. Ư�� ZoomTo�� with Right Fix�� �̿��� ���, Zoom Out�� ������ ���� ��Ƶ�,
//		������ Ȯ������ �ʵ��� �Ѵ�. (�ð������� ��� �׷� ��츦 �����Ѵ�.)
short CKTBChartCtrl::ZoomToWithRightFix(short p_nZoomValue, BOOL p_bWithRightFix) 
{
	// TODO: Add your dispatch handler code here

	// (2004.05.18, ��¿�) �� ȭ�鿡 ���̴� Data �� (ZoomValue)�� ������ �ݿ��Ѵ�.
	// (2004.07.02, ��¿�) �� ȭ�鿡 ���̴� ���� �ٲ�� ��쿡��, ScrollBar�� �߽����� ó���Ѵ�.
	// 1. ���� View�� Start/End Index�� ���Ѵ�.
	int nStartIndex, nEndIndex;
	if( !GetDataStartEnd( nStartIndex, nEndIndex)) return FALSE;
	// 2. ������ �� ȭ���� Data���� ���� Start Index�� �����Ѵ�.
	nStartIndex = nEndIndex - p_nZoomValue + 1;
	// 3. StartIndex�� ��ȿ���� Ȯ���ϰ�, ������ End Index�� �����Ѵ�.
	if( nStartIndex < 0)	// ����(04/09/20): 0 Base
	{
		nStartIndex = 0;
		// (2004.07.05, ��¿�) EndIndex�� ������ p_bWithRightFix�� ���� False�� ��쿡�� ó���Ѵ�.
		if( !p_bWithRightFix)
		{
			nEndIndex = nStartIndex + p_nZoomValue - 1;
			// 4. End Index�� ��ü Index���� �Ѿ�� ������ �߻��ÿ��� �� ȭ���� Data ���� �����Ѵ�.
			int nScrollRange;
			if( !GetScrollRange( nScrollRange)) return FALSE;
			if( nScrollRange < nEndIndex) nEndIndex = nScrollRange;
		}
		// 4.5 End Index�� ������ �Ǿ��� �׷��� �ʰ� ����, Zoom Value�� ������ ���Ͽ� (���� ���� �� �ִ�.)
		//		����Ǵ� ����� Zoom Value�� ���Ѵ�.
		p_nZoomValue = nEndIndex - nStartIndex + 1;
	}
	// 5. ������ Start/End Index�� �����Ѵ�.
	SetStartEndIndex( nStartIndex, nEndIndex);

	return p_nZoomValue;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.09, ��¿�) Scroll �� Zoom�� ������ ó���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CKTBChartCtrl::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount)
{
	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	if( m_bLockScrollUIUpdate) return;

	if( m_pMainBlock) m_pMainBlock->SetDataRegionInBlock2( p_nStartPos, p_nStartPos + p_nViewCount - 1);
	if( m_pToolBarManager) m_pToolBarManager->UpdateZoomAndScrollInfo( p_nFullRange, p_nStartPos, p_nViewCount, m_bWholeView);

	// ���������� Total Count�� ��Ƽ��Ʈ�� �˷��� : �������� - ojtaso (20070704)
	SendEventMsgToDLL( UWM_MULTITOTALCOUNT, 0, p_nFullRange);

	// 4. Notify DLL to manage of the user set, the one page data count.
	// (2007/5/23 - Seung-Won, Bae) Notify Data Count with '�ڷ�����' Priority for Special Chart.
	NeoChangeOnePageDataCount( p_nViewCount);
}


// (2009/5/11 - Seung-Won, Bae) Reset Scroll.
void CKTBChartCtrl::ResetScroll( BOOL p_bWithCalcDRI)
{
	if( !m_pxScaleManager) return;
	if( p_bWithCalcDRI) m_pxScaleManager->Calc_DRI();

	// assume that Calc_DRI() was finished.
	// 1. Get Start and End DRI
	int nEndDRIOfView = m_pxScaleManager->GetCount() - 1;
	int nStartDRIOfView = nEndDRIOfView - m_nOnePageDataCount + 1;
	if( m_bWholeView || nStartDRIOfView < 0) nStartDRIOfView = 0;
	if( 0 <= nEndDRIOfView) m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDRIOfView, nEndDRIOfView);
	
	InvalidateControl();
}
