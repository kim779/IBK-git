#pragma once
#include "CGrp_Base.h"
class CGrp_Line :
    public CGrp_Base
{
public:
    CGrp_Line(CWnd* pView, class CGrpWnd* pGrpWnd, char* pInfo);
    virtual ~CGrp_Line();

    bool	CalculateMinMax();
protected:
	struct _cgNIndex	m_MaxDayIndex, m_MinDayIndex;
	CPoint	m_MaxDayPos, m_MinDayPos;
	double	m_dispMaxVal, m_dispMinVal;

	void	DrawChart(CDC* pDC);
	void	DrawLineChart(CDC* pDC);
	void	DrawDotChart(CDC* pDC);
	void	DrawPivotChart(CDC* pDC);
	void	DrawMinMaxDay(CDC* pDC);
	void	DrawPivot(CDC* pDC, int yPivot);
};

