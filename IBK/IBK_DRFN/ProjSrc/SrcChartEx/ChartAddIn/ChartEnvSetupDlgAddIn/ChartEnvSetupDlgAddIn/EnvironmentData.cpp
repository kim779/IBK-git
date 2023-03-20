// EnvironmentData.cpp: implementation of the CEnvironmentData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnvironmentData.h"

#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CStyleData
////////////////////////////

CStyleData::CStyleData()
{
}

CStyleData::CStyleData(const CStyleData& styleData)
{
	*this = styleData;
}

CStyleData &CStyleData::operator=( const CStyleData& styleData)
{
	m_styleName			= styleData.GetStyleName();
	m_priceUpColor		= styleData.GetPriceUpColor();
	m_priceDownColor	= styleData.GetPriceDownColor();
	m_priceEqualColor	= styleData.GetPriceEqualColor();
	m_priceUpFill		= styleData.GetPriceUpFill();
	m_priceDownFill		= styleData.GetPriceDownFill();
	m_tradeUpColor		= styleData.GetTradeUpColor();
	m_tradeDownColor	= styleData.GetTradeDownColor();
	m_tradeEqualColor	= styleData.GetTradeEqualColor();
	m_tradeUpFill		= styleData.GetTradeUpFill();
	m_tradeDownFill		= styleData.GetTradeDownFill();
	int nMACount = styleData.GetPriceMACount();
	for( int i = 0; i < nMACount; i++) m_priceColor.SetAtGrow( i, styleData.GetPriceMAColor( i));
	nMACount = styleData.GetTradeMACount();
	for( int i = 0; i < nMACount; i++) m_tradeColor.SetAtGrow( i, styleData.GetTradeMAColor( i));
	m_graphBGColor		= styleData.GetGraphBGColor();
	m_graphBorderColor	= styleData.GetGraphBorderColor();
	m_graphGridColor	= styleData.GetGraphGridColor();
	m_scaleBGColor		= styleData.GetScaleBGColor();
	m_scaleBorderColor	= styleData.GetScaleBorderColor();
	m_scaleTextColor	= styleData.GetScaleTextColor();
	m_scaleGridColor	= styleData.GetScaleGridColor();
	m_gridWidth			= styleData.GetGridWidth();
	m_gridHeight		= styleData.GetGridHeight();
	m_divideWidth		= styleData.GetDivideWidth();
	m_divideHeight		= styleData.GetDivideHeight();
	m_analyzeColor		= styleData.GetAnalyzeColor();
	m_numericalColor	= styleData.GetNumericalColor();
	
	//KHD : �ð����� : �׸���Ÿ�� 
	m_nHScaleGridType_Day		= styleData.GetHScaleGridType_Day();
	m_nHScaleGridType_Mon		= styleData.GetHScaleGridType_Mon();
	m_nHScaleGridType_Year		= styleData.GetHScaleGridType_Year();
	m_nHScaleGridType_Week		= styleData.GetHScaleGridType_Week();

	// �ð����� : �÷�
	m_clrHScaleGridColor_Day	= styleData.GetHScaleGridColor_Day();
	m_clrHScaleGridColor_Week	= styleData.GetHScaleGridColor_Week();
	m_clrHScaleGridColor_Mon	= styleData.GetHScaleGridColor_Mon();
	m_clrHScaleGridColor_Year	= styleData.GetHScaleGridColor_Year();

	m_scaleLineThickness_Day    = styleData.GetScaleLineThickness_Day();
	m_scaleLineThickness_Week    = styleData.GetScaleLineThickness_Week();
	m_scaleLineThickness_Mon   = styleData.GetScaleLineThickness_Mon();
	m_scaleLineThickness_Year    = styleData.GetScaleLineThickness_Year();


	m_titleListData.SetMATitleListData(styleData.GetMATitleListData());

	return *this;
}

///////////////////////////////////
// Get Methods
CString	CStyleData::GetStyleName() const
{
	return m_styleName;
}

COLORREF CStyleData::GetPriceUpColor() const
{
	return m_priceUpColor;
}

COLORREF CStyleData::GetPriceDownColor() const
{
	return m_priceDownColor;
}

COLORREF CStyleData::GetPriceEqualColor() const
{
	return m_priceEqualColor;
}

bool CStyleData::GetPriceUpFill() const
{
	return m_priceUpFill;
}

bool CStyleData::GetPriceDownFill() const
{
	return m_priceDownFill;
}

COLORREF CStyleData::GetTradeUpColor() const
{
	return m_tradeUpColor;
}

COLORREF CStyleData::GetTradeDownColor() const
{
	return m_tradeDownColor;
}

COLORREF CStyleData::GetTradeEqualColor() const
{
	return m_tradeEqualColor;
}

bool CStyleData::GetTradeUpFill() const
{
	return m_tradeUpFill;
}

bool CStyleData::GetTradeDownFill() const
{
	return m_tradeDownFill;
}

int CStyleData::GetPriceMACount( void) const
{
	return m_priceColor.GetSize();
}

COLORREF CStyleData::GetPriceMAColor( const int p_nIndex) const
{
	if( p_nIndex < 0 || m_priceColor.GetUpperBound() < p_nIndex) return 0;
	return m_priceColor.GetAt( p_nIndex);
}

int CStyleData::GetTradeMACount( void) const
{
	return m_tradeColor.GetSize();
}

COLORREF CStyleData::GetTradeMAColor( const int p_nIndex) const
{
	if( p_nIndex < 0 || m_tradeColor.GetUpperBound() < p_nIndex) return 0;
	return m_tradeColor.GetAt( p_nIndex);
}

COLORREF CStyleData::GetGraphBGColor() const
{
	return m_graphBGColor;
}

COLORREF CStyleData::GetGraphBorderColor() const
{
	return m_graphBorderColor;
}

COLORREF CStyleData::GetGraphGridColor() const
{
	return m_graphGridColor;
}

COLORREF CStyleData::GetScaleBGColor() const
{
	return m_scaleBGColor;
}

COLORREF CStyleData::GetScaleBorderColor() const
{
	return m_scaleBorderColor;
}

COLORREF CStyleData::GetScaleTextColor() const
{
	return m_scaleTextColor;
}

COLORREF CStyleData::GetScaleGridColor() const
{
	return m_scaleGridColor;
}

int CStyleData::GetGridWidth() const
{
	return m_gridWidth;
}

int CStyleData::GetGridHeight() const
{
	return m_gridHeight;
}

int CStyleData::GetDivideWidth() const
{
	return m_divideWidth;
}

int CStyleData::GetDivideHeight() const
{
	return m_divideHeight;
}

COLORREF CStyleData::GetAnalyzeColor() const
{
	return m_analyzeColor;
}

COLORREF CStyleData::GetNumericalColor() const
{
	return m_numericalColor;
}

CMATitleListData& CStyleData::GetMATitleListData()
{
	return m_titleListData;
}

const CMATitleListData& CStyleData::GetMATitleListData() const
{
	return m_titleListData;
}

void CStyleData::SetMATitleListData(const CMATitleListData& sourceTitleListData)
{
	m_titleListData.SetMATitleListData(sourceTitleListData);
}


///////////////////////////////////
// Set Methods
void CStyleData::SetStyleName(const CString& name)
{
	m_styleName = name;
}

void CStyleData::SetPriceUpColor(const COLORREF& clrPriceUp)
{
	m_priceUpColor = clrPriceUp;
}

void CStyleData::SetPriceDownColor(const COLORREF& clrPriceDown)
{
	m_priceDownColor = clrPriceDown;
}

void CStyleData::SetPriceEqualColor(const COLORREF& clrPriceEqual)
{
	m_priceEqualColor = clrPriceEqual;
}

void CStyleData::SetPriceUpFill(const bool& priceUpFill)
{
	m_priceUpFill = priceUpFill;
}

void CStyleData::SetPriceDownFill(const bool& priceDownFill)
{
	m_priceDownFill = priceDownFill;
}

void CStyleData::SetTradeUpColor(const COLORREF& clrTradeUp)
{
	m_tradeUpColor = clrTradeUp;
}

void CStyleData::SetTradeDownColor(const COLORREF& clrTradeDown)
{
	m_tradeDownColor = clrTradeDown;
}

void CStyleData::SetTradeEqualColor(const COLORREF& clrTradeEqual)
{
	m_tradeEqualColor = clrTradeEqual;
}

void CStyleData::SetTradeUpFill(const bool& tradeUpFill)
{
	m_tradeUpFill = tradeUpFill;
}

void CStyleData::SetTradeDownFill(const bool& tradeDownFill)
{
	m_tradeDownFill = tradeDownFill;
}

void CStyleData::SetPriceMAColor( const int p_nIndex, const COLORREF& clrPrice)
{
	if( p_nIndex < 0) return;
	m_priceColor.SetAtGrow( p_nIndex, clrPrice);
}

void CStyleData::SetTradeMAColor( const int p_nIndex, const COLORREF& clrTrade)
{
	if( p_nIndex < 0) return;
	m_tradeColor.SetAtGrow( p_nIndex, clrTrade);
}

void CStyleData::SetGraphBGColor(const COLORREF& clrGraphBG)
{
	m_graphBGColor = clrGraphBG;
}

void CStyleData::SetGraphBorderColor(const COLORREF& clrGraphBorder)
{
	m_graphBorderColor = clrGraphBorder;
}

void CStyleData::SetGraphGridColor(const COLORREF& clrGraphGrid)
{
	m_graphGridColor = clrGraphGrid;
}

void CStyleData::SetScaleBGColor(const COLORREF& clrScaleBG)
{
	m_scaleBGColor = clrScaleBG;
}

void CStyleData::SetScaleBorderColor(const COLORREF& clrScaleBorder)
{
	m_scaleBorderColor = clrScaleBorder;
}

void CStyleData::SetScaleTextColor(const COLORREF& clrScaleText)
{
	m_scaleTextColor = clrScaleText;
}

void CStyleData::SetScaleGridColor(const COLORREF& clrScaleGrid)
{
	m_scaleGridColor = clrScaleGrid;
}

void CStyleData::SetGridWidth(const int& gridWidth)
{
	m_gridWidth = gridWidth;
}

void CStyleData::SetGridHeight(const int& gridHeight)
{
	m_gridHeight = gridHeight;
}

void CStyleData::SetDivideWidth(const int &divideWidth)
{
	m_divideWidth = divideWidth;
}

void CStyleData::SetDivideHeight(const int &divideHeight)
{
	m_divideHeight = divideHeight;
}

void CStyleData::SetAnalyzeColor(COLORREF clrAnalyze)
{
	m_analyzeColor = clrAnalyze;
}

void CStyleData::SetNumericalColor(COLORREF clrNumerical)
{
	m_numericalColor = clrNumerical;
}

// (2009/9/21 - Seung-Won, Bae) To save the user default style.
BOOL CStyleData::IsSameWith( CStyleData &p_styleData)
{
	if( m_styleName			!= p_styleData.m_styleName)			return FALSE;
	if( m_priceUpColor		!= p_styleData.m_priceUpColor)		return FALSE;
	if( m_priceDownColor	!= p_styleData.m_priceDownColor)	return FALSE;
	if( m_priceEqualColor	!= p_styleData.m_priceEqualColor)	return FALSE;
	if( m_priceUpFill		!= p_styleData.m_priceUpFill)		return FALSE;
	if( m_priceDownFill		!= p_styleData.m_priceDownFill)		return FALSE;
	if( m_tradeUpColor		!= p_styleData.m_tradeUpColor)		return FALSE;
	if( m_tradeDownColor	!= p_styleData.m_tradeDownColor)	return FALSE;
	if( m_tradeEqualColor	!= p_styleData.m_tradeEqualColor)	return FALSE;
	if( m_tradeUpFill		!= p_styleData.m_tradeUpFill)		return FALSE;
	if( m_tradeDownFill		!= p_styleData.m_tradeDownFill)		return FALSE;
	if( m_graphBGColor		!= p_styleData.m_graphBGColor)		return FALSE;
	if( m_graphBorderColor	!= p_styleData.m_graphBorderColor)	return FALSE;
	if( m_graphGridColor	!= p_styleData.m_graphGridColor)	return FALSE;
	if( m_scaleBGColor		!= p_styleData.m_scaleBGColor)		return FALSE;
	if( m_scaleBorderColor	!= p_styleData.m_scaleBorderColor)	return FALSE;
	if( m_scaleTextColor	!= p_styleData.m_scaleTextColor)	return FALSE;
	if( m_scaleGridColor	!= p_styleData.m_scaleGridColor)	return FALSE;
	if( m_gridWidth			!= p_styleData.m_gridWidth)			return FALSE;
	if( m_gridHeight		!= p_styleData.m_gridHeight)		return FALSE;
	if( m_divideWidth		!= p_styleData.m_divideWidth)		return FALSE;
	if( m_divideHeight		!= p_styleData.m_divideHeight)		return FALSE;
	if( m_analyzeColor		!= p_styleData.m_analyzeColor)		return FALSE;
	if( m_numericalColor	!= p_styleData.m_numericalColor)	return FALSE;

	if( m_priceColor.GetSize()	!= p_styleData.m_priceColor.GetSize())	return FALSE;
	if( m_tradeColor.GetSize()	!= p_styleData.m_tradeColor.GetSize())	return FALSE;

	for( int i = 0; i < m_priceColor.GetSize(); i++)
		if( m_priceColor.GetAt( i)	!= p_styleData.m_priceColor.GetAt( i))	return FALSE;
	for( int i = 0; i < m_tradeColor.GetSize(); i++)
		if( m_tradeColor.GetAt( i)	!= p_styleData.m_tradeColor.GetAt( i))	return FALSE;

	//KHD : �ð����� 
	if( m_nHScaleGridType_Day		!= p_styleData.m_nHScaleGridType_Day)		return FALSE;
	if( m_nHScaleGridType_Week		!= p_styleData.m_nHScaleGridType_Week)		return FALSE;
	if( m_nHScaleGridType_Mon		!= p_styleData.m_nHScaleGridType_Mon)		return FALSE;
	if( m_nHScaleGridType_Year		!= p_styleData.m_nHScaleGridType_Year)		return FALSE;

	if( m_clrHScaleGridColor_Day 	!= p_styleData.m_clrHScaleGridColor_Day)	return FALSE;
	if( m_clrHScaleGridColor_Week 	!= p_styleData.m_clrHScaleGridColor_Week)	return FALSE;
	if( m_clrHScaleGridColor_Mon	!= p_styleData.m_clrHScaleGridColor_Mon)	return FALSE;
	if( m_clrHScaleGridColor_Year 	!= p_styleData.m_clrHScaleGridColor_Year)	return FALSE;

	if( m_scaleLineThickness_Day	!= p_styleData.m_scaleLineThickness_Day )	return FALSE;
	if( m_scaleLineThickness_Week	!= p_styleData.m_scaleLineThickness_Week )	return FALSE;
	if( m_scaleLineThickness_Mon	!= p_styleData.m_scaleLineThickness_Mon )	return FALSE;
	if( m_scaleLineThickness_Year	!= p_styleData.m_scaleLineThickness_Year )	return FALSE;

	return TRUE;
}

//KHD : �׸���Ÿ�� ���� : üũ�� �Ǿ����� ����, �Ǽ��� ���� : ��.��.��.���� ��������..
void	CStyleData::SetHScaleGridType_TimeGubun( const int &nHScaleGridType_Day, int nType)
{	
	if(nType == CScaleBaseData::MINTICK_DAY_GUBUN)
		m_nHScaleGridType_Day = nHScaleGridType_Day ;
	else if(nType == CScaleBaseData::DAYCHART_WEEK_GUBUN)
		m_nHScaleGridType_Week = nHScaleGridType_Day;
	else if(nType == CScaleBaseData::DAYCHART_MON_GUBUN)
		m_nHScaleGridType_Mon = nHScaleGridType_Day;
	else if(nType == CScaleBaseData::DAYCHART_YEAR_GUBUN)
		m_nHScaleGridType_Year = nHScaleGridType_Day;
	else if(nType == CScaleBaseData::WEEKCHART_MON_GUBUN)
		m_nHScaleGridType_Mon = nHScaleGridType_Day;
	else if(nType == CScaleBaseData::WEEKCHART_YEAR_GUBUN)
		m_nHScaleGridType_Year = nHScaleGridType_Day;
	else if(nType == CScaleBaseData::MONCHART_YEAR_GUBUN)
		m_nHScaleGridType_Year = nHScaleGridType_Day ;

}

void	CStyleData::SetHScaleGridColor_TimeGubun( const COLORREF& clrHScaleGridColor, int nType)
{	
	if(nType == CScaleBaseData::MINTICK_DAY_GUBUN)
		m_clrHScaleGridColor_Day = clrHScaleGridColor ;
	else if(nType == CScaleBaseData::DAYCHART_WEEK_GUBUN)
		m_clrHScaleGridColor_Week = clrHScaleGridColor;
	else if(nType == CScaleBaseData::DAYCHART_MON_GUBUN)
		m_clrHScaleGridColor_Mon = clrHScaleGridColor;
	else if(nType == CScaleBaseData::DAYCHART_YEAR_GUBUN)
		m_clrHScaleGridColor_Year = clrHScaleGridColor;
	else if(nType == CScaleBaseData::WEEKCHART_MON_GUBUN)
		m_clrHScaleGridColor_Mon = clrHScaleGridColor;
	else if(nType == CScaleBaseData::WEEKCHART_YEAR_GUBUN)
		m_clrHScaleGridColor_Year = clrHScaleGridColor;
	else if(nType == CScaleBaseData::MONCHART_YEAR_GUBUN)
		m_clrHScaleGridColor_Year = clrHScaleGridColor ;
	else 
		m_clrHScaleGridColor_Day = clrHScaleGridColor;
}

void	CStyleData::SetScaleLineThickness_TimeGubun(int nType ,  int scaleLineThickness)
{
	if(nType == CScaleBaseData::MINTICK_DAY_GUBUN)
		m_scaleLineThickness_Day = scaleLineThickness ;
	else if(nType == CScaleBaseData::DAYCHART_WEEK_GUBUN)
		m_scaleLineThickness_Week = scaleLineThickness;
	else if(nType == CScaleBaseData::DAYCHART_MON_GUBUN)
		m_scaleLineThickness_Mon = scaleLineThickness;
	else if(nType == CScaleBaseData::DAYCHART_YEAR_GUBUN)
		m_scaleLineThickness_Year = scaleLineThickness;
	else if(nType == CScaleBaseData::WEEKCHART_MON_GUBUN)
		m_scaleLineThickness_Mon = scaleLineThickness;
	else if(nType == CScaleBaseData::WEEKCHART_YEAR_GUBUN)
		m_scaleLineThickness_Year = scaleLineThickness;
	else if(nType == CScaleBaseData::MONCHART_YEAR_GUBUN)
		m_scaleLineThickness_Year = scaleLineThickness ;
	else 
		m_scaleLineThickness_Day = scaleLineThickness;
}

////////////////////
///�м����� ������
// 0114
CAnalysisToolData::CAnalysisToolData()
{
	m_bShowTrendLine	= false;
	m_bShowVertical		= false;
	m_bShowHorizonLeft	= false;
	m_bShowHorizonRight = false;
	m_bShowThree		= false;
	m_bShowFour			= false;
	m_bShowFibo			= false;
}

// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
CAnalysisToolData::	CAnalysisToolData(const bool bShowTrendLine,
					const bool bShowVertical/*=true*/,
					const bool bShowHorizonLeft/*=true*/,
					const bool bShowHorizonRight/*=true*/,
					const bool bShowThree/*=true*/,
					const bool bShowFour/*=true*/,
					const bool bShowFibo/*=true*/)
{
	m_bShowTrendLine	= bShowTrendLine;
	m_bShowVertical		= bShowVertical;
	m_bShowHorizonLeft	= bShowHorizonLeft;
	m_bShowHorizonRight = bShowHorizonRight;
	m_bShowThree		= bShowThree;
	m_bShowFour			= bShowFour;
	m_bShowFibo			= bShowFibo;
}
	
CAnalysisToolData::~CAnalysisToolData()
{
}

int CAnalysisToolData::GetShowTrendLine() const
{
	return m_bShowTrendLine;
}

void CAnalysisToolData::SetShowTrendLine(const bool bShowTrendLine)
{
	m_bShowTrendLine = bShowTrendLine;
}

int CAnalysisToolData::GetShowVertical() const
{
	return m_bShowVertical;
}

void CAnalysisToolData::SetShowVertical(const bool bShowVertical)
{
	m_bShowVertical = bShowVertical;
}

// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
int CAnalysisToolData::GetShowHorizonLeft() const
{
	return m_bShowHorizonLeft;
}

// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
void CAnalysisToolData::SetShowHorizonLeft(const bool bShowHorizonLeft)
{
	m_bShowHorizonLeft = bShowHorizonLeft;
}

// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
int CAnalysisToolData::GetShowHorizonRight() const
{
	return m_bShowHorizonRight;
}

// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
void CAnalysisToolData::SetShowHorizonRight(const bool bShowHorizonRight)
{
	m_bShowHorizonRight = bShowHorizonRight;
}

int CAnalysisToolData::GetShowThree() const
{
	return m_bShowThree;
}

void CAnalysisToolData::SetShowThree(const bool bShowThree)
{
	m_bShowThree = bShowThree;
}

int CAnalysisToolData::GetShowFour() const
{
	return m_bShowFour;
}

void CAnalysisToolData::SetShowFour(const bool bShowFour)
{
	m_bShowFour = bShowFour;
}

int CAnalysisToolData::GetShowFibo() const
{
	return m_bShowFibo;
}

void CAnalysisToolData::SetShowFibo(const bool bShowFibo)
{
	m_bShowFibo = bShowFibo;
}

void CAnalysisToolData::SetSavedData(const CAnalysisToolData& analysistooldata)
{
	m_bShowTrendLine	= analysistooldata.GetShowTrendLine();
	m_bShowVertical		= analysistooldata.GetShowVertical();
	// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
	m_bShowHorizonLeft	= analysistooldata.GetShowHorizonLeft();
	m_bShowHorizonRight	= analysistooldata.GetShowHorizonRight();

	m_bShowThree		= analysistooldata.GetShowThree();
	m_bShowFour			= analysistooldata.GetShowFour();
	m_bShowFibo			= analysistooldata.GetShowFibo();
}


///////////////////////////////////////////////////////////////////////////////
// class CMATitleListData

const CList<CString, CString>& CMATitleListData::GetPriceMATitleList() const
{
	return m_priceMATitleList;
}

const CList<CString, CString>& CMATitleListData::GetTradeMATitleList() const
{
	return m_tradeMATitleList;
}

CList<CString, CString>& CMATitleListData::GetPriceMATitleList()
{
	return m_priceMATitleList;
}

CList<CString, CString>& CMATitleListData::GetTradeMATitleList()
{
	return m_tradeMATitleList;
}

CList<CString, CString>* CMATitleListData::GetPriceMATitleList_Pointer()
{
	return &m_priceMATitleList;
}

CList<CString, CString>* CMATitleListData::GetTradeMATitleList_Pointer()
{
	return &m_tradeMATitleList;
}

void CMATitleListData::SetMATitleListData(const CMATitleListData& sourceMATitleListData)
{
	SetTitleListData(sourceMATitleListData.GetPriceMATitleList(), m_priceMATitleList);
	SetTitleListData(sourceMATitleListData.GetTradeMATitleList(), m_tradeMATitleList);
}

void CMATitleListData::AddTail_PriceMA(const CString& strTitle)
{
	m_priceMATitleList.AddTail(strTitle);
}

void CMATitleListData::AddTail_TradeMA(const CString& strTitle)
{
	m_tradeMATitleList.AddTail(strTitle);
}

void CMATitleListData::RemoveAll()
{
	RemovePriceMA();
	RemoveTradeMA();
}

void CMATitleListData::RemovePriceMA()
{
	m_priceMATitleList.RemoveAll();
}

void CMATitleListData::RemoveTradeMA()
{
	m_tradeMATitleList.RemoveAll();
}

void CMATitleListData::SetTitleListData(const CList<CString, CString>& sourceData, CList<CString, CString>& targetData)
{
	targetData.RemoveAll();
	POSITION pos = sourceData.GetHeadPosition();
	while(pos != NULL)
	{
		CString strTitle = sourceData.GetNext(pos);
		targetData.AddTail(strTitle);
	}
}
