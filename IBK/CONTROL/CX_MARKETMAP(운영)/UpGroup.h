#pragma once
// UpGroup.h : header file
//

#include "StdAfx.h"

#define WD_UPDBCLICK	WM_USER + 0x01
#define WD_TRIGGERCC	WM_USER + 0x02

/////////////////////////////////////////////////////////////////////////////
// CUpGroup window

class CUpGroup : public CWnd
{
// Construction
public:
	CUpGroup();

// Attributes
public:
	CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> m_arrChild;
	bool	m_bSelected;        //0 ó����ȸ�ؿ� ���� �����ڵ�����   1 �����ڵ��������� ����Ŭ���ؼ� �����ڵ峻 ��������

private:
	int	m_nSelected;
	CRect	m_rect;
	CFont	m_font;
	COLORREF m_nBackColor;
	CString m_strUp;
	CString m_strUpName;
	CString m_strCode;
	CString m_strName;
	CString m_strRate;
	CString m_strSiga;
	CString m_strPers;
	CString m_strMnPers;
	CString m_strForg;		//�ܱ��� �ŷ���
	CString m_strMnForg;		//�ܱ��� �ŷ����
	CString m_strGign;		//��� �ŷ���
	CString m_strMnGign;		//��� �ŷ����
	CString m_strSgTt;
	CString m_strAmt;
	CString m_strMny;
	CString m_strCurr;
	CString m_strDiff;
	CRect	m_parentRect;
	CWnd*	m_parent;

	double	m_nSumGG;		//������ż����� 
	double	m_nSumFg;		//�ܱ��μ��ż�����
	double	m_nSumPs;		//���μ��ż�����
		
	double	m_nSumSgTt;
	double	m_nSumAmt;
	double	m_nSumQnt;
	double	m_nSumMny;
		
	double	m_nSumAmtFore;		//�ܱ��� ���ż��� �� 
	double	m_nSumAmtGigb;		//���  ���ż��� �� 
		
	double	m_nSumMnFore;		//�ܱ��� ���ż���� �� 
	double	m_nSumMnGigb;		//���  ���ż���� �� 

	int	m_nViewType;
	int	m_nScale;
	int	m_nStandard;
	
	bool	m_mouseIN;

	std::unique_ptr<CToolTipCtrl>	m_ToolTip;
	CString	m_strcode;

// Operations
public:
	void	SetRect(CRect rect);
	CRect	GetRect()		{ return m_rect; }
	CRect	GetParentRect()		{ return m_parentRect; }
	void	SetViewType(int nView)	{ m_nViewType = nView; }
	void	SetScale(int nScale)	{ m_nScale = nScale; }
	void	SetStandard(int nStd)	{ m_nStandard = nStd; }
	void	updateRect();
	void	updateRectByRate();
	void	updateColorByRate(int nUpColor, int nDownColor, int nEqualColor, double fMaxRate);
	void	updateColorBySum(COLORREF ggColor, COLORREF fgColor, COLORREF psColor);
	void	updateChildColorByRate(int nUpColor, int nDownColor, int nEqualColor, double fMaxRate);
	void	updateChildColorBySum(COLORREF ggColor, COLORREF fgColor, COLORREF psColor);
	void	PutData(std::shared_ptr<CUpGroup> data);
	COLORREF GetBackColor()		{ return m_nBackColor; }
	void	SetUPCode(CString data) { m_strUp = data;}
	CString GetUPCode()		{ return m_strUp; }
	void	SetUpName(CString data) { m_strUpName = data;}
	CString GetUpName()		{ return m_strUpName; }
	void	SetCode(CString data)	{ m_strCode = data;}
	CString GetCode()		{ return m_strCode; }
	void	SetName(CString data)	{ m_strName = data; }
	CString GetName()		{ return m_strName; }
	void	SetRate(CString data)	{ m_strRate = data; }
	CString GetCurRate()		{ return m_strRate; }

	void	SetSgTotal(CString data){ m_strSgTt = data; }		
	CString GetSgTotal()		{ return m_strSgTt; }
	void	SetAmount(CString data) { m_strAmt = data; }
	CString GetAmount()		{ return m_strAmt; }
	void	SetMoney(CString data)	{ m_strMny = data; }
	CString GetMoney()		{ return m_strMny; }
	void	SetSumQnt(CString sGign,CString sForg,CString sPers);
	void	SetCurr(CString strCurr){ m_strCurr = strCurr;}
	CString GetCurr()		{ return m_strCurr;   }
	void	SetDiff(CString strDiff){ m_strDiff = strDiff;}
	CString GetDiff()		{ return m_strDiff;   }

	void	SetPers(CString data);			//���� ���ż���
	CString GetPers();
	void	SetForg(CString data);			//�ܱ��� ���ż���
	CString GetForg();
	void	SetGign(CString data);			//��� ���ż���
	CString GetGign();
	void	SetMnPers(CString data);		//���� �ŷ����
	CString GetMnPers();
	void	SetMnForg(CString data) ;		//�ܱ��� �ŷ����
	CString GetMnForg();
	void	SetMnGign(CString data);		//��� �ŷ����
	CString GetMnGign();

	void	SetSumAmt(CString sAmt);		//�ŷ���
	double	GetSumAmt()		{ return m_nSumAmt;}   
	void	SetSumSgTotal(CString sSgTt);		//�ð��Ѿ� 
	double	GetSumSgTotal()		{ return m_nSumSgTt;}
	void	SetSumMoney(CString sMny);		//�ŷ����
	double	GetSumMoney()		{ return m_nSumMny;}
	void	SetSumAmtFore(CString sFore);		//�ܱ��� ���ż�����
	double	GetSumAmtFore()		{ return m_nSumAmtFore;}
	void	SetSumAmtGign(CString sGign);		//����� ���ż���
	double	GetSumAmtGign()		{ return m_nSumAmtGigb;}
	void	SetSumMnFore(CString sMnFore);		//�ܱ��� ���ż����
	double	GetSumMnFore()		{ return m_nSumMnFore;}
	void	SetSumMnGign(CString sMnGign);		//��� ���ż����
	double	GetSumMnGign()		{ return m_nSumMnGigb;} 

	void	SetParentRect(CRect rc)	{ CopyRect(m_parentRect,rc); }
	void	SetParent(CWnd* wnd)	{ m_parent = wnd; }
	void	Reset();
	double	GetSortData(int nType);
	double	GetDetailData(int nType);
	void	updateRectByRate(CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo,CRect rect);
	void	SetSiga(CString sSiga)	{ m_strSiga = sSiga;}
	CString	GetSiga()		{ return m_strSiga;}

	double	floordata(double dData, int igubn = 2);

private:
	void	draw_text(CDC* pDC = NULL);
	void	updateRect(CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo,CRect rect);
	int	createColorB(int nEqualColor,int nOriginColor,double fMaxRate,double fRate);
	int	createColorG(int nEqualColor,int nOriginColor,double fMaxRate,double fRate);
	int	createColorR(int nEqualColor,int nOriginColor,double fMaxRate,double fRate);
	double	GetRate();
	void	SetBackColor(COLORREF nColor);
	void	draw_rect(CDC *pDC, CRect iRc);
	void	draw_selected(CDC *pDC, CRect rc);
	void	draw_fillRect(CDC *pDC, CRect rc,COLORREF cl,CString str = "");
	bool	IsNumber(CString str);
	void	InitToolTip();
	void	SetToolTipText(CString text, BOOL bActivate = TRUE);
	void	Quick_Sort(CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo, int count, int type);
	void	Quick_Recursive(CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo, int left, int right, int type);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpGroup)
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUpGroup();

	// Generated message map functions
protected:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CUpGroup)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};