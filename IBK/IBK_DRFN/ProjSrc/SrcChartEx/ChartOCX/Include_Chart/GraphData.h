// GraphData.h: interface for the CGraphData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHDATA_H__075DBCF9_ED81_4895_9B71_891B5C1713A7__INCLUDED_)
#define AFX_GRAPHDATA_H__075DBCF9_ED81_4895_9B71_891B5C1713A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"
#include "Color.h"
#include "./Dll_Load/GraphDrawer.h"
#include "./CalcDataForDraw.h"
class CIndicatorInfo;

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

// Pen Thickness
class AFX_EXT_CLASS_INDICOND CPenThickness
{
public:
	CPenThickness& operator=(const CPenThickness& data);
	bool operator==(const CPenThickness& data) const;

public:
	CPenThickness();
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	CPenThickness(const int nThickness1, const int nThickness2, const int nThickness3, const int nBaseLineThickness, const bool bIsHightlight = false);
	CPenThickness(const CPenThickness& penThickness);

	void SetData( const char *p_szPenThickness);
	void SetHightlight(const bool bIsHightlight);
	void SetThickness(const int nThickness1, const int nThickness2, const int nThickness3);
	void SetThickness1(const int nThickness);
	void SetThickness2(const int nThickness);
	void SetThickness3(const int nThickness);
	void ReversedHightlight();

	bool IsHightlight() const;
	int GetThickness1() const;
	int GetThickness2() const;
	int GetThickness3() const;
	int GetDrawingThickness1() const;
	int GetDrawingThickness2() const;
	int GetDrawingThickness3() const;

	void	GetStringData( CString &p_strPenThickness) const;
	
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	void SetBaseLineThickness(const int nThickness);
	int GetBaseLineThickness() const;

private:
	bool m_bIsHightlight;	// �� ���� ���� (true = ����)
	int m_nThickness1;
	int m_nThickness2;
	int m_nThickness3;

	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	int m_nBaseLineThickness;

	int GetDrawingThickness(const int orgPenThickness) const;
};

class AFX_EXT_CLASS_INDICOND CGraphDataDrawingData
{
public:
	CGraphDataDrawingData& operator=(const CGraphDataDrawingData& data);
	bool operator==(const CGraphDataDrawingData& data);

public:
	CGraphDataDrawingData();
	CGraphDataDrawingData( const char *p_szGraphDataDrawingData);
	CGraphDataDrawingData(const bool bCalcRiseFallRate_LowHigh, const bool bCalcRiseFallRate_LowCur, 
		const bool bCalcRiseFallRate_HighCur, const bool bCalcRate_PrevCur, const int nCalcRiseFallRate_CurData, const BOOL p_bShowOHLC);
	CGraphDataDrawingData(const CGraphDataDrawingData& data);

	void	SetDoesCalcRiseFallRate_LowHigh(const bool bCalcData);
	void	SetDoesCalcRiseFallRate_LowCur(const bool bCalcData);
	void	SetDoesCalcRiseFallRate_HighCur(const bool bCalcData);
	void	SetDoesCalcRate_PrevCur(const bool bCalcData);
	void	SetCalcRiseFallRate_CurData(const int nCalcData);
	void	SetShowOHLC( const BOOL p_bShowOHLC);	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data

	bool	DoesAllDefaultData() const;
	bool	DoesCalcRiseFallRate_LowHigh() const;
	bool	DoesCalcRiseFallRate_LowCur() const;
	bool	DoesCalcRiseFallRate_HighCur() const;
	bool	DoesCalcRate_PrevCur() const;
	int		GetCalcRiseFallRate_CurData() const;
	BOOL	GetShowOHLC( void) const;				// (2009/9/12 - Seung-Won, Bae) for OHLC graph data

	// viewdata ��������
	void GetData( CString &p_strGraphDataDrawingData) const;

private:
	bool	m_bCalcRiseFallRate_LowHigh;	// ������ ��� �ְ� �����
	bool	m_bCalcRiseFallRate_LowCur;	// ������ ��� ���簡 �����
	bool	m_bCalcRiseFallRate_HighCur;	// �ְ� ��� ���簡 �����
	bool	m_bCalcRate_PrevCur;			// ���Ϻ�
	int		m_nCalcRiseFallRate_CurData;	// ���ϰ� ��� ���簡 ����� (���簡)
	BOOL	m_bShowOHLC;	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data

	void SetData( const char *p_szGraphDataDrawingData);

// (2006/5/9 - Seung-Won, Bae) Clear Data
public:
	void	ClearData( void);
};

// ============================================================================
// signal�� ���� data
class AFX_EXT_CLASS_INDICOND CSignalData
{
public:
	CSignalData& operator=(const CSignalData& data);
	bool operator==(const CSignalData& data);

public:
	CSignalData(const bool bIsUse = false, const bool bIsShow = false);
	CSignalData( const char *p_szSignalData);
	CSignalData(const CSignalData& data);
	~CSignalData();

	void SetUse(const bool bIsUse);
	void SetShow(const bool bIsShow);

	bool IsUse() const;
	bool IsShow() const;
	bool IsDraw() const;

	// signal data ��������
	void GetData( CString &p_strSignal) const;

private:
	void SetData( const char *p_szSignalData);

private:
	bool m_bIsUse;	// Graph�� signal(��ȣ) �������
	bool m_bIsShow;	// Graph�� signal(��ȣ) ǥ��

// (2006/5/9 - Seung-Won, Bae) Clear Data
public:
	void	ClearData( void);

// (2009/10/7 - Seung-Won, Bae) for Signal Arrow Drawing Cache.
public:
	CCalcDataForDraw *	m_pSignalCache;
public:
	void				ClearCalcDataForDraw( void);
};

// (2006/11/22 - Seung-Won, Bae) the Indicator Name can be setted in Only CDefaultIndicatorInfoBuilder
class CGraphDataBase
{
protected:
	friend class	CDefaultIndicatorInfoBuilder;		// for Load User or Map Default Indicator Info
	friend class	CIndicatorListImplementation;		// for Create new Graph Indicator Info.
	friend class	CVertScaleGroup;					// for Create new Graph Name of Old Map with multiple same-indicator.
	friend class	CNotDefaultUserTypeIndiListBuilder;	// for Load User Indicator Info with Old Version Converting.
	friend class	CIndicatorInfoImplementation;		// for Copy of Pair Tick.
	virtual void	SetIndicatorName(	const CString &p_strIndicatorName)	= 0;
	virtual void	SetGraphName(		const CString &p_strGraphName)		= 0;
};

// graph �� ���� data
class AFX_EXT_CLASS_INDICOND CGraphData : public CGraphDataBase
{
public:
	CGraphData()	{}							// (2006/11/26 - Seung-Won, Bae) Created by CIndicatorInfo Consturctor.
	CGraphData( const char *p_szGraphData, HWND p_hOcxWnd);		// (2006/9/6 - Seung-Won, Bae) Created by AddGraph or AddChartBlock of OCX or BuildGraphsAndScales in Map[CL] Loaded.

// (2006/9/6 - Seung-Won, Bae) Save Map Design Info.
public:
	// �������� ���� : �������� - ojtaso (20070507)
	void					GetData( CString &p_strGraphData, LPCTSTR lpszRQ = NULL) const;

public:
	CGraphData &operator	=( const CGraphData& data);
	void					ClearData( void);				// (2006/5/9 - Seung-Won, Bae) Clear Data

// �ش� graph�� �´� packet
protected:
	CString	m_strPacketName;
public:
	CString	GetPacketNames( void) const;
	void	SetPacketNames( const CString& packetNames);

// Graph�� signal ��뿩�� �� ǥ��
protected:
	CSignalData	m_signalData;
public:
	CSignalData		GetSignalData(	void) const;
	CSignalData &	GetSignalData(	void);
	bool			IsSignalUse(	void) const;
	bool			IsSignalShow(	void) const;
	void			SetSignalData(	const CSignalData& signalData);
	void			SetSignalData(	const bool bIsUse, const bool bIsShow);

// Graph�� data ǥ��
protected:
	CGraphDataDrawingData	m_graphDataDrawingData;
public:
	CGraphDataDrawingData	GetGraphDataDrawingData( void) const;
	CGraphDataDrawingData &	GetGraphDataDrawingData( void);
	void					SetGraphDataDrawingData( const CGraphDataDrawingData& drawingData);

// (2006/11/26 - Seung-Won, Bae) Support Indicator Name & Graph Name (Support Indicator Name with Graph Name in temporary.)
protected:
	CString			m_strIndicatorName;
	CString			m_strGraphName;
protected:
	virtual void	SetIndicatorName(	const CString &p_strIndicatorName)	{	m_strIndicatorName	= p_strIndicatorName;	}
	virtual void	SetGraphName(		const CString &p_strGraphName)		{	m_strGraphName		= p_strGraphName;		}
protected:
public:
	CString			GetGraphName( void) const								{	return m_strGraphName;						}
	CString			GetIndicatorName( void) const							{	return m_strIndicatorName;					}
public:
	CString			GetGraphName2( void) const								{	return GetGraphName();						}
	CString			GetIndicatorName2( void) const							{	return GetIndicatorName();					}
};

// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
typedef struct _ST_HORZLINE
{
	UINT m_nIndex;
	CString m_strTitle;
	COLORREF m_clrLine;
	unsigned short m_nLine;
	double m_dPrice;
}ST_HORZLINE;

// subGraph�� ���� data
class AFX_EXT_CLASS_INDICOND CSubGraphDataBase
{
public:
	CSubGraphDataBase& operator=(const CSubGraphDataBase& data);

public:
	CSubGraphDataBase();
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	CSubGraphDataBase( const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle);
	CSubGraphDataBase( const CSubGraphDataBase& data);
	CSubGraphDataBase( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName);
	~CSubGraphDataBase();

	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	void SetTitleEndPosition(const int nTitleEndPosition, const int nTitleEndLine = 1);
	void SetData( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName);
	void SetType(const CGraphBaseData::GRAPHTYPE type);
	void SetStyle(const CGraphStyle& style);
	void SetDrawStyle(const CGraphDrawStyle& drawStyle);
	void SetName(const CString& name);
	void SetTitle(const CString& title);
	void SetHiding(const bool hiding);
	void SetColor(const CGraphColor& color);
	void SetPenThickness(const CPenThickness& penThickness);

	void ChangeKeyPart(const CSubGraphDataBase& data);
	// ����� ǥ�� : �������� - ojtaso (20070625)
	void SetItemName(const CString& strItemName);
	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	void SetSubGraphIndex(const int nSubGraphIndex);
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	void SetPaintStyle(const CGraphPaintStyle& nPaintStyle);
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	void SetBaseLineStyle(const CGraphBaseLineStyle& nBaseLineStyle);

	int GetTitleEndPosition() const;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	int GetTitleEndLine() const;
	CGraphBaseData::GRAPHTYPE GetType() const;
	CGraphStyle GetStyle() const;
	CGraphDrawStyle GetDrawStyle() const;
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	CGraphPaintStyle GetPaintStyle() const;
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	CGraphBaseLineStyle GetBaseLineDrawStyle() const;
	CString GetName() const;
	CString GetTitle() const;
	CGraphColor GetColor() const;
	CGraphColor* GetColor_PointerType();
	CGraphColor& GetColor();
	CPenThickness GetPenThickness() const;
	CPenThickness& GetPenThickness();

	void GetStringData( CString &p_strSubGraphData) const;
	// ����� ǥ�� : �������� - ojtaso (20070625)
	CString GetItemName() const;

	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	int GetSubGraphIndex() const;

	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	long AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	void UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	void DeleteHorizontalLine(long nIndex);
	void DeleteAllHorizontalLine();

	const CArray<ST_HORZLINE*, ST_HORZLINE*>& GetHorizontalLine() const { return m_arrHorzLine; }

protected:
	int m_nTitleEndPosition;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	int m_nTitleEndLine;
	CGraphBaseData::GRAPHTYPE m_TYPEType;	// graph type
	CGraphStyle m_nStyle;					// graph style
	CGraphDrawStyle m_nDrawStyle;			// graph draw style
	CGraphPaintStyle m_nPaintStyle;			// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	CGraphBaseLineStyle m_nBaseLineStyle;		// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	
	CString m_strName;		// graph name
	CString m_strTitle;		// graph title
	bool m_bHiding;			// �׸������� ����
	CGraphColor m_clrColor;	// graph�� ����
	CPenThickness m_penThickness;	// graph pen ����
	// ����� ǥ�� : �������� - ojtaso (20070625)
	CString m_strItemName;	// �����
	int m_nSubGraphIndex;	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)

	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	CArray<ST_HORZLINE*, ST_HORZLINE*>	m_arrHorzLine;
};
// (2007/2/28 - Seung-Won, Bae) Do not support Sub Graph Hide Info with directly.
//		Just Use the CSubGraph's GetHiding(); or CGraphPartData's GetHiding( p_nIndex);
//		It is for Graph Hiding.
class AFX_EXT_CLASS_INDICOND CSubGraphData : public CSubGraphDataBase
{
public:
	CSubGraphData();
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	CSubGraphData( const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle);
	CSubGraphData( const CSubGraphData& data);
	CSubGraphData( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName);
	~CSubGraphData();

	CCalcDataForDraw *	m_pGraphCache; //��Ʈ ���� ���� ����. ���� ��.
	void	ClearCalcDataForDraw( void);

private:
friend class CSubGraph;
friend class CSubGraphPartData;
friend class CGraphPartData;
friend class CUserTypeIndiInfoBuilder;
	bool GetHiding() const;

public:
	CSubGraphData& operator=(const CSubGraphData& data);
};

#endif // !defined(AFX_GRAPHDATA_H__075DBCF9_ED81_4895_9B71_891B5C1713A7__INCLUDED_)
