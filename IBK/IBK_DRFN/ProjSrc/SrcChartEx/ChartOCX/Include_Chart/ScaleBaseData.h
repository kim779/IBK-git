// ScaleBaseData.h: interface for the CScaleBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEBASEDATA_H__F252545D_49F3_4EC9_B326_F6CF6CE4EA89__INCLUDED_)
#define AFX_SCALEBASEDATA_H__F252545D_49F3_4EC9_B326_F6CF6CE4EA89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

// ���� type ���� class : ��¥, �ð�, text 
class AFX_EXT_CLASS CScaleCompart
{
public:
	CScaleCompart& operator=(const CScaleCompart& data);

public:
	CScaleCompart();
	CScaleCompart(const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextType);

	CString GetDateCompart() const;
	CString GetTimeCompart() const;
	CScaleBaseData::TEXTCOMPARTTYPE GetScaleTextType() const;

	void SetCompart( char *p_szCompart);
	void SetDateCompart(const CString& dateCompart);
	void SetTimeCompart(const CString& timeCompart);
	void SetScaleTextType(const CScaleBaseData::TEXTCOMPARTTYPE eTextType);

private:
	CString m_strDateCompart;
	CString m_strTimeCompart;
	CScaleBaseData::TEXTCOMPARTTYPE m_eTextType;
};


// scale
class AFX_EXT_CLASS CScale
{
public:
	CScale& operator=(const CScale& data);

public:
	CScale();

	// scale�� ����� data�� setting
	void SetData(const CString& scaleData);
	virtual void SetScaleData( char *p_szScaleData, HWND p_hOcxWnd);
	void SetGridType(const CScaleBaseData::SCALEGRIDTYPE eGridType);

	CString GetScaleData() const;
	CScaleBaseData::SCALEGRIDTYPE GetGridType() const;

protected:
	void SetGridType( char *p_szGridType);

private:
	CString m_strScaleData; // scale�� ����� data. (packet �� scale�� ����ϴ� �� - "�ڷ�����", "�ڷ�ð�", "�ð�"...)	
	CScaleBaseData::SCALEGRIDTYPE m_eScaleGrid;
};


#define	_DPV_CLRDAILYDIVGRIDCOLOR			RGB(255,0,0)	
#define	_DPV_CLRWEEKDIVGRIDCOLOR			RGB(210,180,140)
#define	_DPV_CLRMONDIVGRIDCOLOR				RGB(123,132,192)
#define	_DPV_CLRYEARDIVGRIDCOLOR			RGB(0,0,0)

// ���� Scale
class AFX_EXT_CLASS CHorizontalScale : public CScale
{
public:
	CHorizontalScale& operator=(const CHorizontalScale& data);

public:
	CHorizontalScale();

	// ���� scale�� ���� ��� data�� setting
	virtual void SetScaleData( char *p_szScaleData, HWND p_hOcxWnd);
	void SetHorzScalePosition( char *p_szScalePosition);
	void SetScaleCompart(const CScaleCompart& scaleCompart);
	// XScale ���� ���� - ojtaso (20080429)
	void SetScaleHeight(const int nHeight);

	CScaleBaseData::HORZSCALEPOSITION GetHorzScalePosition() const;
	CScaleCompart GetScaleCompart() const;
	CScaleCompart& GetScaleCompart();
	// XScale ���� ���� - ojtaso (20080429)
	int GetScaleHeight() const;

	// scale�� ��� data
	void GetScaleAllData( CString &p_strHorzScaleData) const;

	CScaleBaseData::SCALEGRIDTYPE GetPeriodGridType(CScaleBaseData::SCALEGRIDTIMEGUBUN nType) const;
	void			SetPeriodGridType(int nType, char *p_szGridType);
	void			SetPeriodGridType(const CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const CScaleBaseData::SCALEGRIDTYPE eGridType, BOOL bCheck);
	BOOL			GetCheckTime(CScaleBaseData::SCALEGRIDTIMEGUBUN nType) const;
	void			SetCheckTime(CScaleBaseData::SCALEGRIDTIMEGUBUN nType, BOOL bCheck);
	unsigned short	GetGridWidth_Time(CScaleBaseData::SCALEGRIDTIMEGUBUN nType,BOOL bBold = FALSE) const;
	void			SetGridWidth_Time(CScaleBaseData::SCALEGRIDTIMEGUBUN nType, unsigned short nGridWidth);
	unsigned long	GetGridColor_Time(CScaleBaseData::SCALEGRIDTIMEGUBUN nType) const;
	void			SetGridColor_Time(CScaleBaseData::SCALEGRIDTIMEGUBUN nType, COLORREF nGridColor);

	CString	TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);

private:
	CScaleBaseData::HORZSCALEPOSITION m_ePosition; // Scale position (���, �ϴ�, ����, Hide)
	CScaleCompart m_scaleCompart; // ��¥, �ð�, text scale ����Ÿ��
	// XScale ���� ���� - ojtaso (20080429)
	int m_nScaleHeight;		// ���� ������ ����

	// �ð����м� ���� ����
	CScaleBaseData::SCALEGRIDTYPE	m_nGridTypeDay;
	CScaleBaseData::SCALEGRIDTYPE	m_nGridTypeWeek;
	CScaleBaseData::SCALEGRIDTYPE	m_nGridTypeMon;
	CScaleBaseData::SCALEGRIDTYPE	m_nGridTypeYear;

	// �ð����м� ��뿩�� ���� 
	BOOL	m_bOn_MINTICK_DAY;
	BOOL	m_bOn_DAYCHART_WEEK;
	BOOL	m_bOn_DAYCHART_MON;
	BOOL	m_bOn_DAYCHART_YEAR;
	BOOL	m_bOn_WEEKCHART_MON;
	BOOL	m_bOn_WEEKCHART_YEAR;
	BOOL	m_bOn_MONCHART_YEAR;

	// �ð����м� ���� ���� 
	unsigned short	m_nGridWidth_Day;
	unsigned short	m_nGridWidth_Week;
	unsigned short	m_nGridWidth_Mon;
	unsigned short	m_nGridWidth_Year;

	//KHD : �ð����м� 
	// ��/ƽ��Ʈ������ ���ڱ��м� �����ɼ� - onlyojt
	COLORREF	m_clrDailyDivGridColor;
	COLORREF	m_clrWeekDivGridColor;
	COLORREF	m_clrMonDivGridColor;
	COLORREF	m_clrYearDivGridColor;

// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
protected:
	friend class CBlocksPosition;
	void	SetHorzScalePosition( const CScaleBaseData::HORZSCALEPOSITION ePosition);
};


#define VSGA_GRIDLINE	0x01
#define VSGA_LEFTSCALE	0x02
#define VSGA_RIGHTSCALE	0x04

// ���� Scale
class AFX_EXT_CLASS CVerticalScale : public CScale  
{
public:
	CVerticalScale();

public:
	CVerticalScale& operator=(const CVerticalScale& data);

private:
	CScaleBaseData::VERTSCALEPOSITION m_ePosition;	// ���� scale ��ġ
	CScaleBaseData::VERTSCALEUNITTYPE m_eUnitType;	// ���� scale ���� ǥ����

// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
protected:
	BOOL							m_bMinMaxLock;	// Lock
	CScaleBaseData::VERTMINMAXTYPE	m_eMinMaxType;	// Type
	double							m_dMin;			// Min
	double							m_dMax;			// Max

// (2007/1/1 - Seung-Won, Bae) Manage Left/Right Vertical Scale and Grid Line Activate Flag
protected:
	int	m_nVScaleActiveFlag;
	bool m_bIsVertScaleInvertType;	// ��ǥ ������ - ojtaso (20071023) 

//2007.01.22 Scale����� ����,ȣ��������
protected:
	double							m_dUserNumber;			// UserNumber
	double							m_dHogaNumber;			// HogaNumber
	int								m_nUserHogaType;		// Type

public:
	// �� setting
	virtual void SetScaleData( char *p_szScaleData, HWND p_hOcxWnd);
	void SetVertScalePosition( char *p_szScalePosition);
	void SetVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition);
	void SetVertScaleUnitType( char *p_szScaleUnitType);
	void SetVertScaleUnitType(const CScaleBaseData::VERTSCALEUNITTYPE eUnitType);

	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition() const;
	CScaleBaseData::VERTSCALEUNITTYPE GetVertScaleUnitType() const;

	// scale�� ��� data
	void GetScaleAllData( CString &p_strVertScaleData, const char *p_szScaleData = NULL) const;

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	// Lock
	BOOL							IsVertScaleMinMaxLock( void) const										{	return m_bMinMaxLock;			}
	void							SetVertScaleMinMaxLock( BOOL p_bMinMaxLock)								{	m_bMinMaxLock = p_bMinMaxLock;	}
	// Type
	CScaleBaseData::VERTMINMAXTYPE	GetVertScaleMinMaxType( void) const										{	return m_eMinMaxType;			}
	void							SetVertScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eMinMaxType) 	{	m_eMinMaxType = p_eMinMaxType;	}
	// Min
	double							GetVertScaleMin( void) const											{	return m_dMin;		}			
	void							SetVertScaleMin( double p_dMin)											{	m_dMin = p_dMin;	}
	// Max
	double							GetVertScaleMax( void) const											{	return m_dMax;		}
	void							SetVertScaleMax( double p_dMax)											{	m_dMax = p_dMax;	}

	// (2007/1/1 - Seung-Won, Bae) Manage Left/Right Vertical Scale and Grid Line Activate Flag
	int		GetActiveFlag(		void) const;
	void	AddActiveFlag(		int p_nActiveFlag);
	void	RemoveActiveFlag(	int p_nActiveFlag);
	void	ResetActiveFlag(	int p_nActiveFlag);

//2007.01.22 Scale����� ����,ȣ��������
	void		SetVertScaleHoGaNumber( double p_dHogaNumber)			{	m_dHogaNumber = p_dHogaNumber;	}
	void		SetVertScaleUserNumber( double p_dUserNumber)			{	m_dUserNumber = p_dUserNumber;	}
	void		SetVertUserHogaScaleType( int p_nUserHogaType)		{	m_nUserHogaType = p_nUserHogaType;	}

	double		GetVertScaleHoGaNumber( void) const						{	return m_dHogaNumber;		}			
	double		GetVertScaleUserNumber( void) const						{	return m_dUserNumber;		}			
	int			GetVertUserHogaScaleType( void) const						{	return m_nUserHogaType;		}			
//2007.01.22 Scale����� ����,ȣ��������
	
	// ��ǥ ������ - ojtaso (20071023)
	bool	GetVertScaleInvertType() const								{	return m_bIsVertScaleInvertType;	}
	// ��ǥ ������ - ojtaso (20071023)
	void	SetVertScaleInvertType(const bool bIsVertScaleInvertType)	{	m_bIsVertScaleInvertType = bIsVertScaleInvertType;	}

// (2007/7/16 - Seung-Won, Bae) Up/Down Space
protected:
	int		m_nUpSpace;
	int		m_nDownSpace;
public:
	void	SetSpace( int p_nUpSpace, int p_nDownSpace)				{	m_nUpSpace = p_nUpSpace;	m_nDownSpace = p_nDownSpace;	}
	void	GetSpace( int &p_nUpSpace, int &p_nDownSpace) const		{	p_nUpSpace = m_nUpSpace;	p_nDownSpace = m_nDownSpace;	}
};


// ============================================================================
// ���� scale�� ���� ���� class
class AFX_EXT_CLASS CHorzScaleRegionHeight  
{
public:
	CHorzScaleRegionHeight();
	CHorzScaleRegionHeight(const int topHeight, const int bottomHeight);

	void SetTopHorzScaleRegionHeight(const int topHeight);
	void SetBottomHorzScaleRegionHeight(const int bottomHeight);

	int GetTopHorzScaleRegionHeight() const;
	int GetBottomHorzScaleRegionHeight() const;

private:
	int m_nTopHeight;
	int m_nBottomHeight;
};

// ���� scale�� �� ���� class
class AFX_EXT_CLASS CVertScaleRegionWidth  
{
public:
	CVertScaleRegionWidth();
	CVertScaleRegionWidth(const int leftWidth, const int rightWidth);

	void SetLeftVertScaleRegionWidth(const int leftWidth);
	void SetRightVertScaleRegionWidth(const int rightWidth);

	int GetLeftVertScaleRegionWidth() const;
	int GetRightVertScaleRegionWidth() const;

private:
	int m_nLeftWidth;
	int m_nRightWidth;
};

// ���� scale ������ ���� class
class AFX_EXT_CLASS CHorzScaleRegion
{
public:
	CHorzScaleRegion();

	void SetTopScaleRegion(const CRect& region);
	void SetBottomScaleRegion(const CRect& region);

	CRect GetTopScaleRegion() const;
	CRect GetBottomScaleRegion() const;

	bool PointInRegion(const CPoint& point) const;

private:
	CRect m_topScaleRegion;
	CRect m_bottomScaleRegion;
};

// ���� scale ������ ���� class
class AFX_EXT_CLASS CVertScaleRegion
{
public:
	CVertScaleRegion();

	void SetLeftScaleRegion(const CRect& region);
	void SetRightScaleRegion(const CRect& region);
	void OffsetRegion(const int nTop, const int nBottom);

	CRect GetLeftScaleRegion() const;
	CRect GetRightScaleRegion() const;
	CRect GetLeftScaleTextRegion() const;
	CRect GetLeftScaleGridRegion() const;
	CRect GetRightScaleTextRegion() const;
	CRect GetRightScaleGridRegion() const;
	
	bool PointInRegion(const CPoint& point) const;

private:
	CRect m_leftScaleRegion;
	CRect m_rightScaleRegion;
};


// --------------------------------------------------------------------------------------
// ȭ�鿡 ���̴� data�� min, max.
class AFX_EXT_CLASS CDisplayDataMinMax
{
public:
	CDisplayDataMinMax& operator=(const CDisplayDataMinMax& displayDataMinMax);

public:
	CDisplayDataMinMax();
	CDisplayDataMinMax(const double& dMin, const double& dMax);

	void SetDisplayData(const double& dMin, const double& dMax);
	void Inflate(const CDisplayDataMinMax& displayDataMinMax);
	void Inflate(const double& dMinGap, const double& dMaxGap);

	double GetDisplayMin() const;
	double GetDisplayMax() const;
	double GetCount() const;

	double GetMarginDisplayMin() const;
	double GetMarginDisplayMax() const;
	bool IsCloseToMaxValue(const double& dCur, const bool bIsInvert = false) const;

	// (2006/11/19 - Seung-Won, Bae) Support Initializing State.
	BOOL	IsInitialized( void) const;

private:
	double m_dMin;	// block���� ������ data�� �ּҰ�
	double m_dMax;	// block���� ������ data�� �ִ밪
};

#endif // !defined(AFX_SCALEBASEDATA_H__F252545D_49F3_4EC9_B326_F6CF6CE4EA89__INCLUDED_)
