#if !defined(AFX_VSAMSUN_H__C89CFB77_2FD8_40B6_A1D4_B002649C4347__INCLUDED_)
#define AFX_VSAMSUN_H__C89CFB77_2FD8_40B6_A1D4_B002649C4347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VarsBase.h"
#include <afxtempl.h>

class CVSamsun : public CVarsBase  
{
public:
	CVSamsun(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CVSamsun();

	CString	GetXposValue(CPoint pt);
	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	void	DrawUnionChart(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	void	SetMember();
	bool	CreateData();
	int	GetYPos(double val);

	// kangmc�߼������� 
	virtual void	GetValueToPoint(CPoint& pt, double dXValue, double dYValue);
	virtual void	GetPtToPosValue(CPoint pt, double& dPos, double& dValue);
	virtual void	GetPtToEndvalPt(CPoint pt, CPoint &ept);
	virtual	CString GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr);
	double	GetOneDataSize();
	int		GetValueToXIdx(double dXValue);
	int		IsNoTime() { return 1;};

private:
	struct _local_data
	{
		double	fpr;
		double	hpr;
		double	lpr;
		double	epr;
		int		iGap;
		int		iSOrgidx;//OrgData start idx
		int		iEOrgidx;//OrgData end idx
		double	dSDay;	// ������Ʈ������ x�� unique�� yyyymmddhhmmss
		double	dMDay;	// ������Ʈ������ x�� unique�� yyyymmddhhmmss
		double	dEDay;	// ������Ʈ������ x�� unique�� yyyymmddhhmmss
		struct _gIndex	date;		
	};
	
	struct SAMSUNTICK
	{
		CString	Context;
		CPoint	xyPoint;
		COLORREF	PenColor;
	};

	enum { EMPTY = 0, INC = 1, DEC = 2};

	double	m_JWValue;
	struct _local_data*	m_pLocal;
	COLORREF	m_UpperColor,m_LowerColor;
	int		m_StartInx;
	int		*m_pSamsunData,*m_pUpDnInfo,*m_pXpos;	
	int		m_iDrawHeight,m_iDrawWidth;
		
	int		GetState(int PreVal, int CurVal);
	void	YCoordinatePrice();
	int		GetSwitchVal(bool bUp);
	void	FindMinMax(int *pArray, int Size);
	bool	CheckMinMax();
	int		m_nShow;

	int			m_iViweOrgDataCnt;		//ȭ��󿡺��̴� org���� ����
	int			m_OrgInDataStart;		//ȭ��󿡺��̴� org��data ����idx

	int			m_CurveN;			//�Ｑ Data ��ü����
	int			m_iViweDataCnt;		//ȭ��� ���̴� �Ｑ ��Ʈ ����
	int			m_iViweSidx;		//ȭ��� ���̴� �Ｑ ��Ʈ start idx
	int			m_iViweEidx;		//ȭ��� ���̴� �Ｑ ��Ʈ end idx
	


	double	m_fUnit;
	bool	m_bCalculated;
};

#endif // !defined(AFX_VSAMSUN_H__C89CFB77_2FD8_40B6_A1D4_B002649C4347__INCLUDED_)
