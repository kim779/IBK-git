// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLOR_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_)
#define AFX_COLOR_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2006/9/5 - Seung-Won, Bae) for Import in Extension DLL
#ifndef AFX_EXT_CLASS_INDICOND
	#ifdef _INDICOND_EXPORT									// in IndicatorCondition
		#define AFX_EXT_CLASS_INDICOND	AFX_CLASS_EXPORT
//	#elif defined _DD_NOT_IMPORT							// in Lib or Regular used in IndicatorCondition.
//		#define AFX_EXT_CLASS_DD			
	#else													// in Other, To Link
		#define AFX_EXT_CLASS_INDICOND	AFX_CLASS_IMPORT
	#endif
#endif

// graph region ��
class AFX_EXT_CLASS_INDICOND CGraphRegionColor  
{
public:
	CGraphRegionColor& operator=(const CGraphRegionColor& data);

public:
	CGraphRegionColor();
	CGraphRegionColor(const COLORREF& backgroundColor, const COLORREF& lineColor, const COLORREF& frameColor);

	void SetBackgroundColor(const COLORREF& backgroundColor);
	void SetLineColor(const COLORREF& lineColor);
	void SetFrameColor(const COLORREF& frameColor);

	COLORREF GetBackgroundColor() const;
	COLORREF GetLineColor() const;
	COLORREF GetFrameColor() const;

private:
	COLORREF m_backgroundColor;	// ������
	COLORREF m_lineColor;		// �𴫻�
	COLORREF m_frameColor;		// �׵θ���
};

// scale ��
class AFX_EXT_CLASS_INDICOND CScaleColor
{
public:
	CScaleColor& operator=(const CScaleColor& data);

public:
	CScaleColor();
	CScaleColor(const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor);

	void SetHorzScaleColor(const COLORREF& horzGridColor);
	void SetVertScaleColor(const COLORREF& vertGridColor);
	void SetScaleTextColor(const COLORREF& textColor);

	COLORREF GetHorzScaleColor() const;
	COLORREF GetVertScaleColor() const;
	COLORREF GetScaleTextColor() const;
	COLORREF& GetHorzScaleColor();
	COLORREF& GetVertScaleColor();
	COLORREF& GetScaleTextColor();

private:
	COLORREF m_horzGridColor;
	COLORREF m_vertGridColor;
	COLORREF m_textColor;
};

// ��Ÿ graph ��
class AFX_EXT_CLASS_INDICOND CEtcGraphColor
{
public:
	CEtcGraphColor& operator=(const CEtcGraphColor& data);

public:
	CEtcGraphColor();
	CEtcGraphColor(const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6);

	COLORREF GetGraphColor1() const;
	COLORREF GetGraphColor2() const;
	COLORREF GetGraphColor3() const;
	COLORREF GetGraphColor4() const;
	COLORREF GetGraphColor5() const;
	COLORREF GetGraphColor6() const;
 
	void SetGraphColor1(const COLORREF& graphColor);
	void SetGraphColor2(const COLORREF& graphColor);
	void SetGraphColor3(const COLORREF& graphColor);
	void SetGraphColor4(const COLORREF& graphColor);
	void SetGraphColor5(const COLORREF& graphColor);
	void SetGraphColor6(const COLORREF& graphColor);

private:
	// �� ����
	COLORREF m_graphColor1;
	COLORREF m_graphColor2;
	COLORREF m_graphColor3;
	COLORREF m_graphColor4;
	COLORREF m_graphColor5;
	COLORREF m_graphColor6;
};

// ��Ÿ ��
class AFX_EXT_CLASS_INDICOND CEtcColor
{
public:
	CEtcColor& operator=(const CEtcColor& data);

public:
	CEtcColor();
	CEtcColor(const COLORREF& toolColor, const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6);

	void SetToolColor(const COLORREF& toolColor);
	void SetEtcGraphColor(const CEtcGraphColor& etcGraphColor);
	void SetEtcGraphColor(const int count, const COLORREF& graphColor);

	COLORREF GetToolColor() const;
	CEtcGraphColor GetEtcGraphColor() const;

private:
	// �м����� ��
	COLORREF m_toolColor;
	// ��Ÿ �׷��� ��
	CEtcGraphColor m_etcGraphColor;
};

// graph ��
/*@Solomon
class AFX_EXT_CLASS_INDICOND CGraphColor  
{
public:
	CGraphColor& operator=(const CGraphColor& data);

public:
	CGraphColor();
	CGraphColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, const COLORREF& color4, const COLORREF& color5, const COLORREF& color6);
	CGraphColor(const CGraphColor& data);

	void SetData( const char *p_szGraphColor, BOOL p_bWithInit = TRUE);
	void SetData(const CGraphColor& graphColor);
	void SetColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, 
		const COLORREF& color4, const COLORREF& color5, const COLORREF& color6);
	void SetColor1(const COLORREF& color);
	void SetColor2(const COLORREF& color);
	void SetColor3(const COLORREF& color);
	void SetColor4(const COLORREF& color);
	void SetColor5(const COLORREF& color);
	void SetColor6(const COLORREF& color);
	void SetUpGraphFill(const bool isFill);
	void SetDownGraphFill(const bool isFill);

	COLORREF GetColor1() const;
	COLORREF GetColor2() const;
	COLORREF GetColor3() const;
	COLORREF GetColor4() const;
	COLORREF GetColor5() const;
	COLORREF GetColor6() const;
	bool GetUpGraphFill() const;
	bool GetDownGraphFill() const;

	void GetStringData( CString &p_strGraphColor) const;
 
private:
	COLORREF	m_color1;	// ��� �׵θ� ��
	COLORREF	m_color2;	// ��� ��
	COLORREF	m_color3;	// �϶� �׵θ� ��
	COLORREF	m_color4;	// �϶� ��
	COLORREF	m_color5;	// ���� �׵θ� ��
	COLORREF	m_color6;	// ���� ��
	bool		m_bUpGraphFill;	// ����� ä��
	bool		m_bDownGraphFill;	// �϶��� ä��

protected:
	COLORREF	GetColorValue( const char *p_szColorData);
	int			GetColorString( COLORREF p_clrColor, char *p_szBuffer) const;
};
*/
// graph ��
class AFX_EXT_CLASS_INDICOND CGraphColor  
{
public:
	CGraphColor& operator=(const CGraphColor& data);
	
public:
	CGraphColor();
	CGraphColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, const COLORREF& color4, const COLORREF& color5, const COLORREF& color6);
	// ���, ���, �϶�, ���� ���� �и� - ojtaso (20080707)
	CGraphColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, const COLORREF& color4, const COLORREF& color5, const COLORREF& color6, const COLORREF& color7, const COLORREF& color8, const COLORREF& color9, const COLORREF& color10);
	CGraphColor(const CGraphColor& data);
	
	void SetData( const char *p_szGraphColor, BOOL p_bWithInit = TRUE);
	void SetData(const CGraphColor& graphColor);
	void SetColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, 
		const COLORREF& color4, const COLORREF& color5, const COLORREF& color6);
	// ���, ���, �϶�, ���� ���� �и� - ojtaso (20080707)
	void SetColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, 
		const COLORREF& color4, const COLORREF& color5, const COLORREF& color6,
		const COLORREF& color7, const COLORREF& color8, const COLORREF& color9, const COLORREF& color10);
	void SetColor1(const COLORREF& color);
	void SetColor2(const COLORREF& color);
	void SetColor3(const COLORREF& color);
	void SetColor4(const COLORREF& color);
	void SetColor5(const COLORREF& color);
	void SetColor6(const COLORREF& color);
	// ���, ���, �϶�, ���� ���� �и� - ojtaso (20080707)
	void SetColor7(const COLORREF& color);
	void SetColor8(const COLORREF& color);
	void SetColor9(const COLORREF& color);
	void SetColor10(const COLORREF& color);
	void SetUpGraphFill(const bool isFill);
	void SetDownGraphFill(const bool isFill);
	// ���, ���, �϶�, ���� ���� �и� - ojtaso (20080707)
	void SetUpPositiveGraphFill(const bool isFill);
	void SetUpNegativeGraphFill(const bool isFill);
	void SetDownPositiveGraphFill(const bool isFill);
	void SetDownNegativeGraphFill(const bool isFill);
	
	COLORREF GetColor1() const;
	COLORREF GetColor2() const;
	COLORREF GetColor3() const;
	COLORREF GetColor4() const;
	COLORREF GetColor5() const;
	COLORREF GetColor6() const;
	// ���, ���, �϶�, ���� ���� �и� - ojtaso (20080707)
	COLORREF GetColor7() const;
	COLORREF GetColor8() const;
	COLORREF GetColor9() const;
	COLORREF GetColor10() const;
	bool GetUpGraphFill() const;
	bool GetDownGraphFill() const;
	// ���, ���, �϶�, ���� ���� �и� - ojtaso (20080707)
	bool GetUpPositiveGraphFill() const;
	bool GetUpNegativeGraphFill() const;
	bool GetDonwPositiveGraphFill() const;
	bool GetDownNegativeGraphFill() const;
	
	void GetStringData( CString &p_strGraphColor) const;
	
private:
	COLORREF	m_color1;	// ���/��� �׵θ� ��
	COLORREF	m_color2;	// ���/��� ��
	COLORREF	m_color3;	// �϶�/���� �׵θ� ��
	COLORREF	m_color4;	// �϶�/���� ��
	COLORREF	m_color5;	// ���� �׵θ� ��
	COLORREF	m_color6;	// ���� ��
	// ���, ���, �϶�, ���� ���� �и� - ojtaso (20080707)
	COLORREF	m_color7;	// ���/���� �׵θ� ��
	COLORREF	m_color8;	// ���/���� ��
	COLORREF	m_color9;	// �϶�/��� �׵θ� ��
	COLORREF	m_color10;	// �϶�/��� ��
	// ���, ���, �϶�, ���� ���� �и� - ojtaso (20080707)
	bool		m_bUpPositiveGraphFill;	// ���/����� ä��
	bool		m_bUpNegativeGraphFill;	// ���/������ ä��
	bool		m_bDownPositiveGraphFill;	// �϶�/����� ä��
	bool		m_bDownNegativeGraphFill;	// �϶�/������ ä��
	
protected:
	COLORREF	GetColorValue( const char *p_szColorData);
	int			GetColorString( COLORREF p_clrColor, char *p_szBuffer) const;
};

#endif // !defined(AFX_COLOR_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_)
