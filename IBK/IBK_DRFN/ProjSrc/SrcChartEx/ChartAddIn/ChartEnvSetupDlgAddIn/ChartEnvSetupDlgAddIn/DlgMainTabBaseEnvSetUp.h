#if !defined(AFX_DLGMAINTABBASEENVSETUP_H__D6B1D0FB_D509_4C15_B9BD_CCD728B3F079__INCLUDED_)
#define AFX_DLGMAINTABBASEENVSETUP_H__D6B1D0FB_D509_4C15_B9BD_CCD728B3F079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMainTabBaseEnvSetUp.h : header file
//

#include "Include_AddIn.h"			// for IChartOCX
#include "OutSideCommonInterfaceFunction.h"
#include "BaseTabDlg.h"
#include "ColorButton.h"
#include "NumEdit.h"
#include "NumSpinCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabBaseEnvSetUp dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 15
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CDlgMainTabBaseEnvSetUp : public CBaseTabDlg
{
	DECLARE_DYNAMIC( CDlgMainTabBaseEnvSetUp)

// Construction
public:
// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
	CDlgMainTabBaseEnvSetUp( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, BOOL bPriceChart=FALSE );   // standard constructor
	~CDlgMainTabBaseEnvSetUp();   // standard destructor

// Dialog Data
	//{{AFX_DATA(CDlgMainTabBaseEnvSetUp)
	enum { IDD = IDD_MAINTAB_PAGE_BASE_ENV_DEV };
	CNumSpinCtrl	m_spnUpDownSpace;
	CNumEdit	m_edtUpDownSpace;
	CNumSpinCtrl	m_spnRightMargin;
	CNumEdit	m_edtRightMargin;
	CColorButton	m_btnTrendLineColor;
	int		m_nCandleTypeOnResize;
	BOOL	m_bShowCurrentPrice;
	BOOL	m_bYScalePositionLeftSide;
	BOOL	m_bYScalePositionRightSide;
	BOOL	m_bShowIndicatorValue;
	BOOL	m_bShowIndicatorVariable;
	BOOL	m_bUseCrossLine;
	int		m_nCurPriceSub;
	BOOL	m_bInquieryDataWindow;
	BOOL	m_bShowAllTitle;
	BOOL	m_bShowPriceMinMaxText;
	BOOL	m_bUseTooptipShow;
	BOOL	m_bUseTooptipShowAll;					//[A00000441]ǳ������ ����� ��ü ��ȸ �ɼ� �߰�
	BOOL	m_bFixInterval;							// ����������
	BOOL	m_bDrawBaseLine;
	int		m_nMouseWheelIndex;
	int		m_nMinMaxRatio;
	//}}AFX_DATA

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	int				m_nInquiryDlgManualMove;
	CNumEdit		m_edtFutureTime;
	CNumSpinCtrl	m_spnFutureTime;
	BOOL			m_bShowExRightDividend;
	BOOL			m_bShowHighLowBound;
	BOOL			m_bUseRevisedPrice;
	BOOL			m_bShowLowHighMarker;		//20100120_JS.Kim_alzioyes	���Ѱ�/���Ѱ� ����ǥ��
	int				m_nCandleType;
	BOOL			m_bLH_LC_HC;
	BOOL			m_bUseGapRevision;
	int				m_nLH_LC_HC;

// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
	CComboBox       m_curPriceTypeCombo;
	int             m_nPriceBongType;
	BOOL			m_bPriceChart;
	BOOL			m_bEnablePreDay;

	CButton	m_chkHScaleGridType_Day;
	CButton	m_chkHScaleGridType_Day_Week;
	CButton	m_chkHScaleGridType_Day_Mon;
	CButton	m_chkHScaleGridType_Day_Year;
	CButton	m_chkHScaleGridType_Week_Mon;
	CButton	m_chkHScaleGridType_Week_Year;
	CButton	m_chkHScaleGridType_Mon_Year;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMainTabBaseEnvSetUp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
// (2009/9/3 - Seung-Won, Bae) Remove not-used control.
public:
	void Recover( void);
	void Save( void);
	void PreView( void);

public:
	virtual CBaseTabDlg::DLG_TYPE GetDialogType() const;
	virtual bool GetCurrentPrice(int& nCurPrice) const;

private:
	int GetCurrentPrice() const;

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMainTabBaseEnvSetUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnInquirydataWindowShow();
	afx_msg void OnShowCurrentPrice();
	afx_msg void OnUseIndicatorName();
	afx_msg void OnChkLhLcHc();
	afx_msg void OnRdoLhLcHc();
	afx_msg void OnShowMinMaxPrice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// OriginalData	
	int m_nOrgCandleType;
	BOOL m_bOrgUseGapRevision;						// 2008.10.15 by LYH >> ������
	int m_nOrgRightMargin;
	int m_nOrgFutureTime;							// (2009/5/17 - Seung-Won, Bae) for Future Time
	int	m_nOrgMouseWheelIndex;
	int m_nOrgCandleTypeOnResize;
	COLORREF m_clrOrgTrendLineColor;
	bool m_bOrgShowIndicatorVariable;				// ��ǥ���� - ��ǥ���� ǥ��
	bool m_bOrgShowIndicatorValue;					// ��ǥ���� - ��ǥ�� ǥ��
	bool m_bOrgShowHighLowBound;
	bool m_bOrgShowExRightDividend;
	bool m_bOrgShowLowHighMarker;					//20100120_JS.Kim_alzioyes	���Ѱ�/���Ѱ� ����ǥ��
// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
	int m_nOrgShowCurrentPrice;
	int	m_nOrgYScalePosition;						// Y�� ����ǥ�� ( 0:����, 1:������, 2:����, 3:���� )
	long m_lOrgNumericalInquiryOption;
	BOOL m_bOrgShowAllTitle;						// ��ǥ�̸�
	BOOL m_bOrgShowPriceMinMaxText;
	BOOL m_bOrgUseRevisedPrice;						// �����ְ�
	BOOL m_bOrgFixInterval;
	BOOL m_bOrgDrawBaseLine;						// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	BOOL m_bOrgLH_LC_HC;							// LH/LC/HC - ojtaso (20071226)
	int  m_nOrgLH_LC_HC;							// LH/LC/HC - ojtaso (20071226) // (2008/10/26 - Seung-Won, Bae) for O/H/L/C (not LH/LC/HC)
	int m_nOrgMinMaxRatio;
	BOOL m_bOrgUseMonthGapRevision;					// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)

	int m_nApplyCandleType;
	BOOL m_bApplyUseGapRevision;					// 2008.10.15 by LYH >> ������
	int m_nApplyRightMargin;
	int m_nApplyFutureTime;							// (2009/5/17 - Seung-Won, Bae) for Future Time
	int	m_nApplyMouseWheelIndex;
	int m_nApplyCandleTypeOnResize;
	COLORREF m_clrApplyTrendLineColor;
	bool m_bApplyShowIndicatorVariable;				// ��ǥ���� - ��ǥ���� ǥ��
	bool m_bApplyShowIndicatorValue;				// ��ǥ���� - ��ǥ�� ǥ��
	bool m_bApplyShowHighLowBound;
	bool m_bApplyShowExRightDividend;
	bool m_bApplyShowLowHighMarker;					//20100120_JS.Kim_alzioyes	���Ѱ�/���Ѱ� ����ǥ��
// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
	int m_nApplyShowCurrentPrice;
	int	m_nApplyYScalePosition;						// Y�� ����ǥ�� ( 0:����, 1:������, 2:����, 3:���� )
	long m_lApplyNumericalInquiryOption;
	BOOL m_bApplyShowAllTitle;						// ��ǥ�̸�
	BOOL m_bApplyShowPriceMinMaxText;
	BOOL m_bApplyUseRevisedPrice;					// �����ְ�
	BOOL m_bApplyFixInterval;
	BOOL m_bApplyDrawBaseLine;						// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	BOOL m_bApplyLH_LC_HC;							// LH/LC/HC - ojtaso (20071226)
	BOOL m_nApplyLH_LC_HC;							// LH/LC/HC - ojtaso (20071226)
	int m_nApplyMinMaxRatio;
	BOOL m_bApplyUseMonthGapRevision;				// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)

	// CurrentData
//	int	m_nYScalePosition;							// Y�� ����ǥ�� ( 0:����, 1:������, 2:����, 3:���� )
	int	m_nRightMargin;								// ������ ����
	int	m_nFutureTime;								// (2009/5/17 - Seung-Won, Bae) for Future Time
	COLORREF m_clrTrendLineColor;					// �߼��� ����

	int m_bPreview;

private:
	IChartOCX *m_pIChartOCX; 
	COutsideCommonInterfaceFunction m_MChartInterface;

	void Initial_Spin( CNumSpinCtrl &pSpin, CNumEdit &pEdit, const int Value, const float p_dMax = 1000);
	void EnableHighLowBound();
	void EnableLowHighMarker();			//20100120_JS.Kim_alzioyes	���Ѱ�/���Ѱ� ����ǥ��
	void EnableShowExRightDividend();
	void EnableCurPrice();

	void EnableUseIndicatorName( BOOL bHideAllTitle );
	
	bool IsApplyShowCurrentPrice(const int nShowCurrentPrice) const;

	// ( 2006 / 11 / 17 - Sang-Woo, Cho ) - �����ְ� ��뿩��
	void EnableRevisedPrice();
public:
	void EnableRevisedPrice( BOOL p_bCheck);	// (2007/10/5 - Seung-Won, Bae) for Revised Price of Map


// (2006/8/16 - Seung-Won, Bae) Info Get/Set Library Functions
protected:
	int		GetShowCurrentPrice( void);
	BOOL	GetExistenceHighLowBound( void);
	void	SetShowHighLowBound( BOOL p_bEnable);
	BOOL	GetExistenceLowHighMarker( void);		//20100120_JS.Kim_alzioyes	���Ѱ�/���Ѱ� ����ǥ��
	void	SetShowLowHighMarker( BOOL p_bEnable);	//20100120_JS.Kim_alzioyes	���Ѱ�/���Ѱ� ����ǥ��
	void	SetShowExRightDividend( const bool bShow);
	void	SetShowCurrentPrice( const int nShowType);
	void	SetShowIndicatorVariable(const bool bShow);
	void	SetShowIndicatorValue(const bool bShow);
	void	SetYScalePosition(const int nPosition);
	// LH/LC/HC - ojtaso (20071226)
	void	SetShowLH_LC_HC( const int nLH_LC_HC);
	int		GetShowLH_LC_HC( void);

// (2007/7/18 - Seung-Won, Bae) Up/Down Space.
protected:
	int		m_nUpDownSpace;
	int		m_nOrgUpDownSpace;
	int		m_nApplyUpDownSpace;
	void	SetUpDownSpace(const int nUpDownSpace);

//>> ������ - vntsorl(20090217)
protected:
	BOOL	GetGapRevision( void);
//<< ������ - vntsorl(20090217)

//<< 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
	BOOL	GetMonthGapRevision( void);
	BOOL	GetShowMonthGapRevision( void);
//>> (Issue No : 0006572)

	// LH/LC/HC - ojtaso (20071226)
//	int GetLH_LC_HC() const;
//	bool IsApplyLH_LC_HC( const int nLH_LC_HC) const;
	// LH/LC/HC - ojtaso (20071226)
//	virtual bool GetLH_LC_HC(BOOL& bLH_LC_HC) const;

// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
	inline void SetDlgItemEnable(UINT nId, BOOL bEnable)
	{
		// ���̾�α� ������ �ڵ��� ���Ѵ�.
		CWnd *pWnd = GetDlgItem(nId);

		// ���̾�α� �������� ���ٸ� �ǵ�����.
		if( !pWnd ) return;

		// ���̾�α� �������� Ȱ��ȭ������ �ʴٸ� �ǵ�����.
		if( !pWnd->IsWindowEnabled() ) return;

		// Ȱ��ȭ�� �����Ѵ�.
		pWnd->EnableWindow(bEnable);

	}


//KHD : �ð����� 
	void	SetTimeGubunGrid();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAINTABBASEENVSETUP_H__D6B1D0FB_D509_4C15_B9BD_CCD728B3F079__INCLUDED_)
