#if !defined(AFX_BBALANCE_H__CCD0245F_F003_4D60_874A_B7720B73ED03__INCLUDED_)
#define AFX_BBALANCE_H__CCD0245F_F003_4D60_874A_B7720B73ED03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

// �ϸ����ǥ
/*
	1. ��ȯ�� = (�ֱ� 9�ϰ��� �ְ�ġ + ����ġ) / 2
	2. ���ؼ� = (�ֱ� 26�ϰ��� �ְ�ġ + ����ġ) / 2
	3. ���ེ�� = ������ �ְ��� 26�� ������ �����Ų �� 
	4. ���ེ��1 = (���ؼ� + ��ȯ��) / 2 -> 26�� ����
	5. ���ེ��2 = (52�ϰ��� �ְ�ġ + ����ġ) / 2 -> 26�� ����
*/


class CBBalance : public CBongBase  
{
public:
	CBBalance(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBBalance();

protected:
	CString	GetXposValue(CPoint pt);

	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);
	int	GetValue() { return m_UnitSize4; }
	bool	CalculateLast(bool bShift);
	

private:
	void FindMinMax();
	bool CreateData();
	void RemoveData();
	int GetYPos(double val);
	void SetMember();

	int	m_UnitSize1, m_UnitSize2;	// ����,��ȯ
	int	m_UnitSize3, m_UnitSize4, m_UnitSize5;	// ����,����1,����2
	int	m_pStyle[5], m_pWidth[5];
	double	*m_Switch;
	double	*m_Pivot;
	double	*m_BSpan, *m_PSpan1, *m_PSpan2;
	struct	_line	m_line[7];
	bool	m_bCalculated;
};

#endif // !defined(AFX_BBALANCE_H__CCD0245F_F003_4D60_874A_B7720B73ED03__INCLUDED_)
