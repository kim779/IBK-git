// VRenko.h: interface for the CVRenko class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VRENKO_H__1B2015DD_26AB_4FAC_A65E_C2E009B4E24E__INCLUDED_)
#define AFX_VRENKO_H__1B2015DD_26AB_4FAC_A65E_C2E009B4E24E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VarsBase.h"
#include <afxtempl.h>
class CVRenko : public CVarsBase  
{
public:
	CVRenko(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CVRenko();

	CString	GetXposValue(CPoint pt);
	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	void	DrawUnionChart(CDC* pDC);
	
	bool	CalculateMinMax(bool bForce = false);
	bool	CheckMinMax();
		
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	void SetUserDefined(CString sVal);
	void GetUsedValue(CString &sVal);


// kangmc�߼������� 
	virtual void	GetValueToPoint(CPoint& pt, double dXValue, double dYValue);
	virtual void	GetPtToPosValue(CPoint pt, double& dPos, double& dValue);
	virtual void	GetPtToEndvalPt(CPoint pt, CPoint &ept);
	virtual	CString GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr);
	double	GetOneDataSize();
	int		GetValueToXIdx(double dXValue);
	int		IsNoTime() { return 1;};
	
// 	void	l_RectLine(CDC *pDC, CRect Rct, int Fg);
private:
	bool CreateData();
	void RemoveData();
	double GetDefVal();
	int GetYPos(double val);
	void SetMember();
	void	FindMinMax();

	struct _fhle_
	{
		struct _gIndex date;
		double	fpr;
		double	hpr;
		double	lpr;
		double	epr;
	};

	struct _local_data
	{		
		double	hpr;
		double	lpr;
		bool	bUp;
		int		iGap;
		int		iSOrgidx;//OrgData start idx
		int		iEOrgidx;//OrgData end idx
		double	dSDay;	// ������Ʈ������ x�� unique�� yyyymmddhhmmss
		double	dEDay;	// ������Ʈ������ x�� unique�� yyyymmddhhmmss
		double	dMDay;	// ������Ʈ������ x�� unique�� yyyymmddhhmmss
		struct _fhle_	info;
	};

	bool		m_bCalculated;
	double		m_JWValue;
	bool		m_bCheck;
	int			m_nUnitSize;	
	double		m_fUnit;
	int			m_iShow;
	int			m_iViweOrgDataCnt;		//ȭ��󿡺��̴� ���� ����
	int			m_OrgInDataStart;		//ȭ��󿡺��̴� ��data ����idx
	int			m_iViweDataCnt;	//ȭ��� ���̴� Renko ��Ʈ ����
	int			m_iViweSidx;		//ȭ��� ���̴� Renko ��Ʈ start idx
	int			m_iViweEidx;		//ȭ��� ���̴� Renko ��Ʈ end idx
	double		m_dFirsrDefVal;
	COLORREF	m_UpperColor,m_LowerColor;

	int			m_iDrawHeight;
	int			m_iDrawWidth;
	int			m_nShow;
	CArray<struct _local_data*, struct _local_data*>	m_arData;

};

#endif // !defined(AFX_VRENKO_H__1B2015DD_26AB_4FAC_A65E_C2E009B4E24E__INCLUDED_)
