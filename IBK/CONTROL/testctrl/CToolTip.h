#pragma once

#define	JUGA_TOOLTIP_CLASS	"Bong ToolTip"
// CToolTip

class CToolTip : public CWnd
{
	DECLARE_DYNAMIC(CToolTip)

public:
	CToolTip(CWnd* pView, CWnd* pGrpWnd);
	virtual ~CToolTip();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	int	Create();
	CRect	SetData(CString sData);

private:
	CSize	CalWndSize(CString strExam);
	CRect	MakeSizeWnd();
	void	DrawTip(CDC* pDC);

private:
	class CGrpWnd* m_pGrpWnd;
	int			m_nCellHeight;
	CRect			m_rcBase;
	CRect			m_rcDraw;
	COLORREF		m_clrBack;
	CFont* m_font;
	CStringArray		m_arTitle;
	CStringArray		m_arContent;
};


