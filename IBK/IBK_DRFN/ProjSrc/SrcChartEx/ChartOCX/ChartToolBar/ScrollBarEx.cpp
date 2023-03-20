// ScrollBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "ScrollBarEx.h"

#include "../Include_Chart/Dll_Load/XScaleManager.h"
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl
#include "ToolBarDlg.h"
#include "DataMath.h"
#include "Conversion.h"
#include "SliderCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx

CScrollBarEx::CScrollBarEx( IChartCtrl *p_pIChartCtrl, int nScrollWidth)
{
	// (2009/1/18 - Seung-Won, Bae) Manage scroll state for next operation calculating.
	m_pIChartCtrl	= p_pIChartCtrl; 
	m_nPreScrollPos = 0;
	
	m_bFullPage = false;
	m_nTotalDataCount = 0;
	m_nDataEnd = 0;
	m_nScrollWidth = nScrollWidth;
	m_nMinDataCountOnOnePage = 5;
	m_pParent = NULL;
}

CScrollBarEx::~CScrollBarEx()
{
}


BEGIN_MESSAGE_MAP(CScrollBarEx, CScrollBar)
	//{{AFX_MSG_MAP(CScrollBarEx)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx message handlers

void CScrollBarEx::SetScrollData_TR(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage, CPacketDataInfo::DATAINSERTTYPE dataType, const int nGlanceBalanceMargin)
{
	//sy 2005.09.15.
	//���� TR ��ȸ�ÿ� �⺻������ ���ϰ� �ǹǷ� �ּ�ó���ؾ� �Ѵ�.
	//if(nTotalDataCount == 0)
	//	return;
	//sy end
	//int reverseDataTotalCount = m_nTotalDataCount;
	
	m_nGlanceBalanceMargin = nGlanceBalanceMargin;
	m_nTotalDataCount = nTotalDataCount; 
	m_bFullPage = bFullPage;
	
	if(bFullPage == true)
		m_nPageDataCount = m_nTotalDataCount;
	//��ȭ�鿡 ���δ� ������ �� ������ ����.
	else if(m_nTotalDataCount < nPageDataCount)
	{
		m_nPageDataCount = nPageDataCount;
		m_nTotalDataCount = m_nPageDataCount;

	}
	else
	{
		m_nPageDataCount = nPageDataCount;
		if(dataType == CPacketDataInfo::INSERT && !m_bFullPage)
		{
			//sy 2005.12.13. ������ȸ�� �����ҽ� �߸���.
			m_nDataEnd = m_nTotalDataCount - 1;
			m_nPreScrollPos = m_nDataEnd - m_nPageDataCount + 1;
			if(m_nPreScrollPos < 0)
				m_nPreScrollPos = 0;
			// ����(04/07/16)
			// '����'��ư�� Ŭ���Ͽ� ��������Ÿ�� ���� ���� ��� �����µ���Ÿ��ŭ index�� �Ѷ󾲽����ش�.
			//m_nPreScrollPos += (m_nTotalDataCount - reverseDataTotalCount);
			//m_nDataEnd += (m_nTotalDataCount - reverseDataTotalCount);
			//sy end
		}
	}

	if(dataType == CPacketDataInfo::INSERT && !m_bFullPage)
	{
		SetScrollPos();
		Invalidate();
		m_pParent->SetSliderPos(m_nPageDataCount);
		m_bFullPage = false;
	}
	else
	{
		//MODIFY: ����(04/06/30)
		m_nDataEnd = m_nTotalDataCount - 1;
		m_nPreScrollPos = m_nDataEnd - m_nPageDataCount + 1;
		if(m_nPreScrollPos < 0)
			m_nPreScrollPos = 0;
		CScrollBarEx::SetScrollInfo();
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollPos
 - Created at  :  2004-10-27   18:17
 - Author      :  ������
 - Description :  Data�� StartIndex�� ���� ScrollBox�� ��ġ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CScrollBarEx::SetScrollPos()
{
	// (2005/9/10 - Seung-Won, Bae) 
	if( !m_nTotalDataCount) return;
	if(m_nPageDataCount >= 1 && m_nTotalDataCount >= 1)
		m_ScrollInfo.nPage = CMath::Round(((double)m_nScrollWidth * (double)m_nPageDataCount) / (double)m_nTotalDataCount);
	else
		m_ScrollInfo.nPage = m_nScrollWidth;
	CScrollBarNew::SetScrollInfo(&m_ScrollInfo, TRUE, m_pParent->GetReverseToOcx());

	CScrollBarNew::SetScrollPos(CMath::Round(double(m_nPreScrollPos * m_nScrollWidth)/(double)m_nTotalDataCount), TRUE, m_pParent->GetReverseToOcx());
}


void CScrollBarEx::SetScrollInfo()
{
	 int scrollMax, scrollPage, scrollPos, scrollTrackPos;
 
	 if(m_bFullPage == true)
	 {
		  scrollPage = m_nScrollWidth;
	 }
	 else
	 {
	  if(m_nPageDataCount >= 1 && m_nTotalDataCount >= 1)
		  scrollPage = int((m_nScrollWidth * m_nPageDataCount) / m_nTotalDataCount);
	  else
		  scrollPage = m_nScrollWidth;
	 }
    
	 scrollMax = m_nScrollWidth;

	// (2009/5/29 - Seung-Won, Bae) Do not reset scroll position.
	//	 if(m_nPageDataCount == m_nTotalDataCount)
	//		 scrollPos = 0;
	//	 else
	//		 scrollPos = m_nScrollWidth;

	// 20100520 �ڵ��� m_nTotalDataCount�� 0���� ���ų� m_nPreScrollPos�� 0���� �ö� �״� ���� ���� >>
	//scrollPos = CMath::Round( double( m_nPreScrollPos * m_nScrollWidth)/(double)m_nTotalDataCount);
	if(m_nPreScrollPos == 0 || m_nScrollWidth == 0 || m_nTotalDataCount == 0) scrollPos = 0;
	else scrollPos = CMath::Round( double( m_nPreScrollPos * m_nScrollWidth)/(double)m_nTotalDataCount);
	// 20100520 �ڵ��� <<

	 if(m_nTotalDataCount <= 0)
		scrollTrackPos = 1;
	 else
		scrollTrackPos = CMath::Round((double)m_nScrollWidth / (double)m_nTotalDataCount);

  	 m_ScrollInfo.cbSize = sizeof(m_ScrollInfo);		 // size of this structure
	 m_ScrollInfo.fMask = SIF_ALL;						 // parameters to set
	 m_ScrollInfo.nMin = 0;							     // minimum scrolling position
	 m_ScrollInfo.nMax = scrollMax;			  		     // maximum scrolling position
	 m_ScrollInfo.nPage = scrollPage;				     // the page size of the scroll box
	 m_ScrollInfo.nPos = scrollPos;						 // initial position of the scroll box
	 m_ScrollInfo.nTrackPos = scrollTrackPos;			 // immediate position of a scroll box that the user is dragging
	 CScrollBarNew::SetScrollInfo(&m_ScrollInfo, TRUE, m_pParent->GetReverseToOcx());
	 CScrollBarNew::SetScrollPos(m_ScrollInfo.nPos, TRUE, m_pParent->GetReverseToOcx());
	 if(this->m_hWnd != NULL) 
		 Invalidate();
}

//tour2k pScrollbar ���� �߰�
void CScrollBarEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{	
	int nScrollValue = 0;
	switch( nSBCode)
	{
	case SB_LINELEFT:	nScrollValue = -1;
						if( m_pParent->GetReverseToOcx()) nScrollValue = -nScrollValue;
						break;
	case SB_LINERIGHT:	nScrollValue = +1;
						if( m_pParent->GetReverseToOcx()) nScrollValue = -nScrollValue;
						break;
	case SB_PAGELEFT:	nScrollValue = -m_nPageDataCount;
						if( m_pParent->GetReverseToOcx()) nScrollValue = -nScrollValue;
						break;
	case SB_PAGERIGHT:	nScrollValue = +m_nPageDataCount;
						if( m_pParent->GetReverseToOcx()) nScrollValue = -nScrollValue;
						break;
	case SB_THUMBPOSITION:	nScrollValue = CMath::Round( ( double( CScrollBarNew::GetScrollPos( FALSE)) * double( m_nTotalDataCount)) / double( m_nScrollWidth));
							if( m_pParent->GetReverseToOcx()) nScrollValue = m_nTotalDataCount - nScrollValue - m_nPageDataCount;
							nScrollValue -= m_nPreScrollPos;
							break;
	case SB_THUMBTRACK:	nScrollValue = CMath::Round( ( ( double)( nPos) *  double( m_nTotalDataCount)) / double( m_nScrollWidth));
						if( m_pParent->GetReverseToOcx()) nScrollValue = ( m_nTotalDataCount - m_nPageDataCount) - nScrollValue;
						nScrollValue -= m_nPreScrollPos;
						break;
	// (2009/1/8 - Seung-Won, Bae) Do not response to SB_ENDSCROLL. (It cause the repaint.)
	default:
		return;
	}//end of switch
	m_pIChartCtrl->ScrollTo( nScrollValue, TRUE);
}

void CScrollBarEx::GetDataStartEnd(int& nDataStart, int& nDataEnd)
{
	nDataStart = m_nPreScrollPos;
	nDataEnd = m_nDataEnd;
}

void CScrollBarEx::SetScrollData_Real(const int nTotalDataCount)
{
	// MODIFY: ����(04/06/01) TR�� ���� ƽ��Ʈ���� �ʱ� ƽ�� ���͵� �׷����� �ʴ� ���� ����
	if(nTotalDataCount >= m_nTotalDataCount)
	{
		int nTemp = nTotalDataCount - m_nTotalDataCount;

		if(!m_bFullPage) // ����(04/07/28): ��ü���Ⱑ �ƴҰ��� ����Ÿ�� �귯�� �ϹǷ�...
			m_nPreScrollPos += nTemp;

		m_nDataEnd+= nTemp;
		// MODIFY: ����(04/06/29) SliderCtrl�� MaxRange�� ���� ū������ �����Ѵ�.
		m_nTotalDataCount = nTotalDataCount;
	}

	bool bMoveEndPage = false;
	if(m_nDataEnd == (m_nTotalDataCount-1))
		bMoveEndPage = true;

	
	if(m_bFullPage == true)
		m_nPageDataCount = m_nTotalDataCount;
	else if(m_nTotalDataCount < m_nPageDataCount)
		m_nTotalDataCount = m_nPageDataCount;

	SetScrollPos();

	if(bMoveEndPage == true){
		if(m_nTotalDataCount == m_nPageDataCount)
			m_ScrollInfo.nPos = 0;
		else
			m_ScrollInfo.nPos = m_nScrollWidth;
		CScrollBarNew::SetScrollPos(m_ScrollInfo.nPos, TRUE, m_pParent->GetReverseToOcx());
	}
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollData_RemovePacket
 - Created at  :  2004-07-22   16:37
 - Author      :  ������(mono@dooriic.co.
 - Parameters  :  nTotalDataCount   - ǥ���� ����Ÿ�� �Ѱ���(��������)
 -				  bFullPage		   - '��ü����' ���� ����
 -                nRemoveDataCount - ������ ��Ŷ����Ÿ ����
 - Description :  ����Ÿ�� ������ �ش�PacketList�� ������ �ִ� �Ӱ�ġ�� �Ѿ�� ������ ����Ÿ��
				  �����ϰ� �ȴ�. ����Ÿ ������ �߻��ϴ� ������ PacketList���� �˷��ָ� Real�߻���
				  �� �ȴ�.
 -----------------------------------------------------------------------------------------*/
void CScrollBarEx::SetScrollData_RemovePacket(const int nTotalDataCount, const int nRemoveDataCount)
{
	// ������ ���� ���ϱ�
	int nRightMargin = m_nTotalDataCount - nTotalDataCount - nRemoveDataCount;
	if(nRightMargin < 0)
		nRightMargin = 0;

	// MODIFY: ����(04/07/30)
	m_nTotalDataCount = nTotalDataCount + nRightMargin;

	// ���������� ������ ����Ÿ ������ TotalDataCount�� �Ѿ��ȵȴ�.
	if(m_nPageDataCount > m_nTotalDataCount)
		m_nPageDataCount = m_nTotalDataCount;

	m_nPreScrollPos = m_nPreScrollPos - nRemoveDataCount + 1;
	m_nDataEnd = m_nDataEnd - nRemoveDataCount + 1;

	if(m_nPreScrollPos < 0)
	{
		m_nPreScrollPos = 0;
		m_nDataEnd = m_nPageDataCount - 1;
	}

	SetScrollPos();
	m_pParent->SetSliderPos(m_nPageDataCount);
}

int CScrollBarEx::GetTotalDataCount()
{
	return m_nTotalDataCount;
}


void CScrollBarEx::SetScrollBarWidth(const int nWidth)
{
	m_nScrollWidth = nWidth;
	SetScrollInfo();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnSetFocus
 - Created at  :  2005-09-10   15:50
 - Author      :  ������
 - Description :  ScrollBar�� ��Ŀ���� ������ OCX�� ��Ŀ���� �ش�.
 // (2005/9/10 - Seung-Won, Bae) This ScrollBar did not catch focus. So, Make it to catch the focus in OnLButtonDown.
  -----------------------------------------------------------------------------------------*/
void CScrollBarEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}

	CScrollBarNew::OnLButtonDown(nFlags, point);
}
// (2005/9/10 - Seung-Won, Bae) For Focus Testing of OCX
void CScrollBarEx::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollBarNew::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here

	// (2005/9/10 - Seung-Won, Bae) Caution! don't call OnSetFocus() in OnLButtonDown().
	//	So. Copy from OnLButtonDown.
	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
}

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CScrollBarEx::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount)
{
	m_nPageDataCount = p_nViewCount;
	m_nTotalDataCount = p_nFullRange;
	m_nPreScrollPos = p_nStartPos;
	m_nDataEnd = p_nStartPos + p_nViewCount - 1;
	if( 0 < m_nTotalDataCount)
	{
		m_ScrollInfo.nPage = CMath::Round(((double)m_nScrollWidth * (double)m_nPageDataCount) / (double)m_nTotalDataCount);					// the page size of the scroll box
		m_ScrollInfo.nPos = CMath::Round(((double)m_nScrollWidth * (double)m_nPreScrollPos) / (double)m_nTotalDataCount);					// initial position of the scroll box
		m_ScrollInfo.nTrackPos = CMath::Round((double)m_nScrollWidth / (double)m_nTotalDataCount);				// immediate position of a scroll box that the user is dragging
		CScrollBarNew::SetScrollInfo( &m_ScrollInfo, TRUE, m_pParent->GetReverseToOcx());
		CScrollBarNew::SetScrollPos( CMath::Round( double( m_nPreScrollPos * m_nScrollWidth)/(double)m_nTotalDataCount), TRUE, m_pParent->GetReverseToOcx());
	}
	else
	{
		m_ScrollInfo.nPage = 1;					// the page size of the scroll box
		m_ScrollInfo.nPos = 0;					// initial position of the scroll box
		m_ScrollInfo.nTrackPos = 0;				// immediate position of a scroll box that the user is dragging
		CScrollBarNew::SetScrollInfo( &m_ScrollInfo, TRUE, m_pParent->GetReverseToOcx());
		CScrollBarNew::SetScrollPos( 0, TRUE, m_pParent->GetReverseToOcx());
	}
}
