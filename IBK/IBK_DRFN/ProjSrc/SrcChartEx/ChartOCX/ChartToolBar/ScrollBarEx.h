#if !defined(AFX_SCROLLBAREX_H__40ED100B_E164_4685_89BF_40AD441CB013__INCLUDED_)
#define AFX_SCROLLBAREX_H__40ED100B_E164_4685_89BF_40AD441CB013__INCLUDED_

#include "BlockBaseEnum.h"
#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollBarEx.h : header file
// ������ ��ũ�ѹ�. ��ũ�ѹٴ� ���� �������ϻ��̴�. Ȯ��/���, �ٸ� ����Ÿ�ε����� �̵��Ҷ�
//	�ش� �ε����� ���� ��ũ�ѹڽ��� ��ġ�� ����Ͽ� ��ġ��Ŵ.

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx window
class CToolBarDlg;

class CScrollBarNew : public CScrollBar
{
public:
	int GetScrollPos(BOOL bReverse = FALSE)
	{
		if(bReverse == FALSE)
		{
			return CScrollBar::GetScrollPos();
		}
		else
		{
			int nPos = CScrollBar::GetScrollPos();

			SCROLLINFO si;
			CScrollBar::GetScrollInfo(&si);

			nPos = si.nMin - (nPos + si.nPage - si.nMax);
			
			return nPos;
		}
	}

	int SetScrollPos(int nPos, BOOL bRedraw = TRUE, BOOL bReverse = FALSE)
	{
		if(bReverse == FALSE)
		{
			return CScrollBar::SetScrollPos(nPos, bRedraw);
		}
		else
		{
			SCROLLINFO si;
			CScrollBar::GetScrollInfo(&si);

			nPos = si.nMin - (nPos + si.nPage - si.nMax);

			int nPos_Prev = GetScrollPos();

			CScrollBar::SetScrollPos(nPos,  bRedraw);

			return nPos_Prev;
		}
	}

	BOOL SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE, BOOL bReverse = FALSE)
	{
		if(bReverse == FALSE)
		{
			return CScrollBar::SetScrollInfo(lpScrollInfo, bRedraw);
		}
		else
		{
			SCROLLINFO si = *lpScrollInfo;

			si.nPos = si.nMin - (si.nPos + si.nPage - si.nMax);			

			return CScrollBar::SetScrollInfo(&si, bRedraw);
		}
	}
};

interface IChartCtrl;
class CScrollBarEx : public CScrollBarNew
{
// Construction
public:
	CScrollBarEx( IChartCtrl *p_pIChartCtrl, int nScrollWidth);

// Attributes
private:
	CToolBarDlg* m_pParent;

	bool m_bFullPage;
	int m_nTotalDataCount;	// �ѵ���Ÿ����
	int m_nPageDataCount;	// �� �������� ǥ���� ����Ÿ ����
	int m_nDataEnd;	// ����Ÿ�� �� �ε���
	SCROLLINFO m_ScrollInfo;
	int m_nScrollWidth;	// ScrollBar�� Width
	int m_nMinDataCountOnOnePage;
	int m_nGlanceBalanceMargin;

// Operations
public:
	void SetParentWnd(CToolBarDlg* pParent){ m_pParent = pParent;};

	void SetScrollData_TR(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage, CPacketDataInfo::DATAINSERTTYPE dataType, const int nGlanceBalanceMargin);
	void SetScrollData_Real(const int nTotalDataCount);
	void SetScrollData_RemovePacket(const int nTotalDataCount, const int nRemoveDataCount);
	void SetScrollPos();
	void SetMinDataCountOnOnePage(const int nMinDataCountOnOnePage){m_nMinDataCountOnOnePage = nMinDataCountOnOnePage;};
	void SetScrollBarWidth(const int nWidth);
	
	int GetPageDataCount(){ return m_nPageDataCount; };
	void GetDataStartEnd(int& nDataStart, int& nDataEnd);
	int GetTotalDataCount();

	// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
	void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount);
	
private:
	void SetScrollInfo();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollBarEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScrollBarEx();

	// Generated message map functions
//protected:
public:
	//{{AFX_MSG(CScrollBarEx)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2009/1/18 - Seung-Won, Bae) Manage scroll state for next operation calculating.
protected:
	IChartCtrl *	m_pIChartCtrl; 
	int				m_nPreScrollPos;	// ����Ÿ�� ���� �ε���
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLBAREX_H__40ED100B_E164_4685_89BF_40AD441CB013__INCLUDED_)
