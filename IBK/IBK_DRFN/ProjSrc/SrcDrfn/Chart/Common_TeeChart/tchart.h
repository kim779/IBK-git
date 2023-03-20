#if !defined(AFX_TCHART_H__D9F2FEA7_9B82_41D5_8AC5_CAE1A386BCF0__INCLUDED_)
#define AFX_TCHART_H__D9F2FEA7_9B82_41D5_8AC5_CAE1A386BCF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
//class CAspect;
class CAxes;
//class CCanvas;
//class CExport;
//class CTitles;
//class CPen1;
//class CImport;
//class CLegend;
//class CPage;
//class CPanel;
//class CPrinter;
//class CScroll;
//class CSeries;
//class CWalls;
//class CZoom;
//class CEnvironment;
//class CTeePoint2D;
//class CToolList;
//class CSeriesList;

/////////////////////////////////////////////////////////////////////////////
// CTChart wrapper class
#include "../Common_TeeChart/series.h"
#include "../Common_TeeChart/axis.h"
#include "../Common_TeeChart/axes.h"

class CTChart : public CWnd
{
protected:
	DECLARE_DYNCREATE(CTChart)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xfab9b41c, 0x87d6, 0x474d, { 0xab, 0x7e, 0xf0, 0x7d, 0x78, 0xf2, 0x42, 0x2e } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	//CAspect GetAspect();
	CAxes GetAxis();
//	CCanvas GetCanvas();
	BOOL GetClipPoints();
	void SetClipPoints(BOOL bNewValue);
	short GetCursor();
	void SetCursor(short nNewValue);
	short GetDragCursor();
	void SetDragCursor(short nNewValue);
	long GetDragMode();
	void SetDragMode(long nNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
//	CExport GetExport();
//	CTitles GetFooter();
//	CPen1 GetFrame();
//	CTitles GetHeader();
	long GetHeight();
	void SetHeight(long nNewValue);
//	CImport GetImport();
	long GetLeft();
	void SetLeft(long nNewValue);
//	CLegend GetLegend();
//	CPage GetPage();
//	CPanel GetPanel();
//	CPrinter GetPrinter();
//	CScroll GetScroll();
	long GetSeriesCount();
	long GetTop();
	void SetTop(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
//	CWalls GetWalls();
	long GetWidth();
	void SetWidth(long nNewValue);
//	CZoom GetZoom();
	void AboutBox();
	long AddSeries(long SeriesClass);
	void ChangeSeriesType(long SeriesIndex, long NewSeriesType);
	long CloneSeries(long SourceSeriesIndex);
	void Draw(long DC, long Left, long Top, long Right, long Bottom);
	void ExchangeSeries(long Series1, long Series2);
	double GetDateTimeStep(long DateTimeStep);
	unsigned long GetFreeSeriesColor(BOOL CheckBackgroundColor);
	CSeries GetLabelsSeries(long DestAxis);
	BOOL IsFreeSeriesColor(unsigned long AColor, BOOL CheckBackgroundColor);
	void RemoveAllSeries();
	void RemoveSeries(long SeriesIndex);
	void Repaint();
	CString SeriesTitleLegend(long ASeriesIndex);
	void ShowEditor(long SeriesIndex);
	void StopMouse();
	BOOL GetTimerEnabled();
	void SetTimerEnabled(BOOL bNewValue);
	long GetTimerInterval();
	void SetTimerInterval(long nNewValue);
	BOOL GetAutoRepaint();
	void SetAutoRepaint(BOOL bNewValue);
	void EditOneSeries(long SeriesIndex);
//	CEnvironment GetEnvironment();
	long GetChartLink();
//	CTeePoint2D GetMousePosition();
	void ChartRect(long Left, long Top, long Right, long Bottom);
//	CToolList GetTools();
//	CTitles GetSubHeader();
//	CTitles GetSubFooter();
	void ClearChart();
	void RefreshData();
	CSeries Series(long SeriesIndex);
	void ShowThemesEditor();
	void SetTheme(long aTheme, long aPalette);
	void ApplyPalette(long paletteIndex);
	void ApplyCustomPalette(const VARIANT& colorArray);
//	CSeriesList GetSeriesList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCHART_H__D9F2FEA7_9B82_41D5_8AC5_CAE1A386BCF0__INCLUDED_)
