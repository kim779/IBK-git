#if !defined(AFX_FXGRID_H__DCA29DDF_7EE1_491F_BD8E_7C1F13E0DE22__INCLUDED_)
#define AFX_FXGRID_H__DCA29DDF_7EE1_491F_BD8E_7C1F13E0DE22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxGrid.h : header file
//
#include "afxtempl.h"
#include "afxmt.h"

#include "range.h"
#include "titletip.h"
#include "griditem.h"
#include "droptarget.h"

class CCell : public CObject
{
public:
	CCell ()
	{
		state = 0;
		attr  = 0;
		format = 0;
		text = _T("");
		image = -1;
		fgcol = CLR_DEFAULT;
		bkcol = CLR_DEFAULT;
		param = 0;
	}

	UINT	 mask;
	UINT	 state;
	UINT	 attr;
	UINT	 format;
	CString	 text;
	int	 image;
	LOGFONT	 font;
	COLORREF fgcol;
	COLORREF bkcol;

	CString	 symbol;
	CString	 misc;

	LPARAM	 param;
};

typedef	CTypedPtrArray <CObArray, CCell*> GRID_ROW;

const	int maxMERGE = 20;

class _fixMerge
{
public:
	int	cols[maxMERGE];
	CRect	rect;
	LOGFONT	font;

	COLORREF fgcol;
	COLORREF bkcol;
	UINT	format;
	CString	text;
};

class CfxGrid;
void  AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CfxGrid& rControl);

/////////////////////////////////////////////////////////////////////////////
// CfxGrid window

class AFX_EXT_CLASS CfxGrid : public CWnd
{
	DECLARE_DYNCREATE(CfxGrid)
// Construction
public:
// updateX_20080602
	CfxGrid(LOGFONT* logfont = NULL, BOOL bType = FALSE);
	CWnd*	Create(const RECT& rect, CWnd* parent, UINT nID, int NOscroll, int dragdrop = GVDD_FULL,
					DWORD dwStyle = WS_CHILD|WS_TABSTOP|WS_VISIBLE);
	bool	Initial(int nRow, int nCol, int fixRow, int fixCol);
	void	Refresh(bool NOselect = true);
	void	Adjust();
	void	Clear(int sidxRow = -1, int sidxCol = -1);
	BOOL	Sort(int nCol, bool ascending);
	void	Blink(int nRow, int nCol, COLORREF color, int interval = 500);
	BOOL	GetCellSize(int fromRow, int endRow, int& colW, int& rowH);
	void	GetCellFromPoint(CPoint pt, int& nRow, int& nCol);
	void	GetTopLeftNonFixedCell(int& nRow, int& nCol);
	void	GetSelectedCellRange(int& minRow, int& maxRow, int& minCol, int& maxCol);
	void	ClearAllRTM();
	void	ClearMerge() { m_mapMerge.RemoveAll(); }
	LONG	MessageToGrid(UINT msg, int scode);

	BOOL	SubclassWindow(HWND hWnd);

	void	RunExcel(bool visible = true);
	bool	RunExcel_();
	
// Operations
public:
	int	GetRowCount() const                    { return m_nRows; }
	BOOL	SetRowCount(int nRows = 10);
	int	GetColumnCount() const                 { return m_nCols; }
	BOOL	SetColumnCount(int nCols = 10);
	int	GetFixedRowCount() const               { return m_nFixedRows; }
	BOOL	SetFixedRowCount(int nFixedRows = 1);
	int	GetFixedColumnCount() const            { return m_nFixedCols; }
	BOOL	SetFixedColumnCount(int nFixedCols = 1);

	int	GetRowHeight(int nRow) const;
	BOOL	SetRowHeight(int row, int height);
	int	GetColumnWidth(int nCol) const;
	BOOL	SetColumnWidth(int col, int width);

	int	GetFixedRowHeight() const;
	int	GetFixedColumnWidth() const;
	long	GetVirtualWidth() const;
	long	GetVirtualHeight() const;

	// grid property
	COLORREF GetGridColor()				{ return m_gridCOL; }
	void	SetGridColor(COLORREF color)		{ m_gridCOL = color; }
	void	SetConditionColor(COLORREF upCOL, COLORREF dnCOL, COLORREF eqCOL);
	COLORREF GetUpColor()				{ return m_upCOL; }
	void	SetUpColor(COLORREF color)		{ m_upCOL = color; }
	COLORREF GetEqColor()				{ return m_eqCOL; }
	void	SetEqColor(COLORREF color)		{ m_eqCOL = color; }
	COLORREF GetDnColor()				{ return m_dnCOL; }
	void	SetDnColor(COLORREF color)		{ m_dnCOL = color; }
	COLORREF GetFGSelColor()			{ return m_fgSELCOL; }
	void	SetFGSelColor(COLORREF color)		{ m_fgSELCOL = color; }
	COLORREF GetBKSelColor()			{ return m_bkSELCOL; }
	void	SetBKSelColor(COLORREF color)		{ m_bkSELCOL = color; }
	int	GetGridLine()				{ return m_gridLine; }
	void	SetGridLine(int nWhichLine)		{ m_gridLine = nWhichLine; if (::IsWindow(GetSafeHwnd())) Invalidate(); }
	int	GetGridLineType()			{ return m_gridLineType; }
	void	SetGridLineType(int LineType)		{ m_gridLineType = LineType; if (::IsWindow(GetSafeHwnd())) Invalidate(); }
	int	GetGridScroll()				{ return m_NOscroll; }
	void	SetGridScroll(int scroll)		{ m_NOscroll = scroll; if (::IsWindow(GetSafeHwnd())) Adjust(); }
	void	SetGridDIScroll(bool diV = false, bool diH = false)	{ m_DIscrollV = diV; m_DIscrollH = diH; }

	void	SetGridFocusLine(BOOL focus, BOOL select) { m_focusLine = focus; m_selectLine = select; }
	void	SetRowColResize(BOOL enRow, BOOL enCol)	{ m_enableRowResize = enRow; m_enableColResize = enCol; }
	void	SetHandleTabKey(BOOL bHandleTab = TRUE)	{ m_handletabkey = bHandleTab; }
	void	SetUseOLE(BOOL bUseOLE = FALSE)		{ m_useOLE = bUseOLE; }
	void	SetTitleTips(BOOL bEnable = TRUE);
	void	SetMouseTrace(bool bTrace = false)	{ m_enableMouseTrace = bTrace; }
	void	SetImageList(CImageList* pList)		{ m_imageList = pList; }
	CIdCell	GetFocusCell() const			{ return m_idCurrent; }
	void	SetSelectRow(int row, bool sinit = true);
	void	SetFocusCellEdit(int row, int col, bool edit = true);
	void	SetFocusCellSensitive(bool sensitive)	{ m_sensitive = sensitive; }
	void	SetKeepingColumn(int col)		{ m_keepcolumn = col; }		// column no move

	CString	validAttribute(int row, int col, CString text);
	bool	SetImageListIndex(int nRow, int nCol, int index);
	BOOL	GetItemRect(int nRow, int nCol, LPRECT pRect) const { return GetCellRect(nRow, nCol, pRect); }

	// grid cell setting
	BOOL	SetItem(const GVITEM* gvitem);
	BOOL	GetItem(GVITEM* gvitem);
	BOOL	SetColumnItems(int nCol, GVITEM* gvitem);

	BOOL	 SetItemText   (int nRow, int nCol, LPCTSTR str);
	CString  GetItemText   (int nRow, int nCol);
	BOOL	 SetItemData   (int nRow, int nCol, LPARAM lParam);
	LPARAM	 GetItemData   (int nRow, int nCol) const;
	BOOL	 SetItemImage  (int nRow, int nCol, int image);
	int	 GetItemImage  (int nRow, int nCol) const;
	BOOL	 SetItemState  (int nRow, int nCol, UINT state);
	UINT	 GetItemState  (int nRow, int nCol) const;
	BOOL	 SetItemAttr   (int nRow, int nCol, UINT attr);
	UINT	 GetItemAttr   (int nRow, int nCol) const;
	BOOL	 SetItemFormat (int nRow, int nCol, UINT format);
	UINT	 GetItemFormat (int nRow, int nCol) const;
	BOOL	 SetItemBkColor(int nRow, int nCol, COLORREF col = CLR_DEFAULT, bool force = false);
	COLORREF GetItemBkColor(int nRow, int nCol) const;
	BOOL	 SetItemFgColor(int nRow, int nCol, COLORREF col = CLR_DEFAULT, bool force = false);
	COLORREF GetItemFgColor(int nRow, int nCol) const;
	BOOL	 SetItemFont   (int nRow, int nCol, LOGFONT* logfont);
	LOGFONT* GetItemFont   (int nRow, int nCol);
	BOOL	 SetItemParam  (int nRow, int nCol, LPARAM param);
	LPARAM	 GetItemParam  (int nRow, int nCol);

	BOOL	SetItemSymbol  (int nRow, int nCol, LPCTSTR str);
	CString	GetItemSymbol  (int nRow, int nCol);
	BOOL	SetItemMisc    (int nRow, int nCol, LPCTSTR str);
	CString	GetItemMisc    (int nRow, int nCol);

	// grid row/column insert/delete
	int	InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE, int nColumn = -1);
	int	InsertRow(LPCTSTR strHeading, int nRow = -1, int szHeight = -1);
	BOOL	InsertRow(int drag, int& drop);
	BOOL	DeleteColumn(int nColumn);
	BOOL	DeleteRow(int nRow);
	BOOL	DeleteNonFixedRows();
	BOOL	DeleteAllItems();

	BOOL	AutoSizeRow(int nRow);
	BOOL	AutoSizeColumn(int nCol);
	void	AutoSizeRows();
	void	AutoSizeColumns();
	void	AutoSize();

	// step color
	void	SetStepColor(int unit, COLORREF color1 = 0, COLORREF color2 = 0) { m_stepUnit = unit; m_stepCOL1 = color1; m_stepCOL2 = color2; }

// 2008. 6. 13. Merge RTS Bug Fix
	void	SetMergeRTS( BOOL bMergeRTS) { m_bMergeRTS = bMergeRTS; };		// TRUE : ON, FALSE : OFF( Default)
	BOOL	GetMergeRTS() { return m_bMergeRTS; };
// 2008. 8. 21. shadowColor
	void	SetShadowColor(COLORREF color) {m_bkCOL = color;};
	void	SetSelectColor(COLORREF fgcolor, COLORREF bkcolor) {m_fgSELCOL = fgcolor; m_bkSELCOL = bkcolor;};

	// 2011.08.17 scbang : 그리드성능개선을 위해 추가
	int	GetDrawTimes();
	void	SetDrawTimes(int iDrawTimes);

// Attributes
protected:
	bool		m_oleInit;
	int		m_NOscroll;
	bool		m_DIscrollV;
	bool		m_DIscrollH;

	COLORREF	m_fgCOL, m_bkCOL;
	COLORREF	m_fixfgCOL, m_fixbkCOL,m_gridCOL;
	COLORREF	m_winfgCOL, m_3dfaceCOL, m_shadowCOL;
	COLORREF	m_upCOL, m_dnCOL, m_eqCOL;
	COLORREF	m_bkSELCOL, m_fgSELCOL;

	// step color
	int		m_stepUnit;
	COLORREF	m_stepCOL1, m_stepCOL2;

	int		m_gridLine;
	int		m_gridLineType;
	BOOL		m_focusLine;
	BOOL		m_selectLine;
	int		m_dragdrop;
	BOOL		m_handletabkey;
	BOOL		m_useOLE;
	BOOL		m_enabletip;
	BOOL		m_sensitive;
	int		m_keepcolumn;

	// cell size
	int		m_nRows, m_nFixedRows;
	int		m_nCols, m_nFixedCols;
	int		m_margin;
	int		m_defWidth, m_defHeight;
	int		m_Vscroll, m_Hscroll;

	CUIntArray	m_aryRowHeight, m_aryColWidth;

	LOGFONT		m_logfont;
	CFont		m_font;
	CFont		m_prnfont;
	CImageList*	m_imageList;
	
	CTypedPtrArray	<CObArray, GRID_ROW*> m_aryGridRows;

	// mouse operation
	CPoint		m_ptClick;
	CPoint		m_ptLast;
	CPoint		m_ptMove;
	CIdCell		m_idClick;
	CIdCell		m_idSelect;
	CIdCell		m_idCurrent;
	int		m_timerID;

	// row/column resize
	BOOL		m_enableRowResize;
	BOOL		m_enableColResize;
	int		m_rowCountWheel;

	// selection range
	CMap <DWORD, DWORD, CIdCell, CIdCell&>	m_mapSelCell;
	CMap <DWORD, DWORD, CIdCell, CIdCell&>	m_mapPreCell;

	// sorting
	int		m_ascending;
	int		m_sortColumn;
	enum	{ cellNot = 0, cellVal, cellAbs, cellStr};

	CTitleTip	m_tooltip;

	// blink (for rtm)
	CCriticalSection	m_sync;
	CMap	<int, int, COLORREF, COLORREF&> m_mapRTM;

	// printing information
	CSize		m_szChar;
	int		m_pgHeight;
	CSize		m_szLogical;
	CSize		m_szPaper;

	// drag & drop
	CIdCell		m_idDrag, m_idDrop;
	CDropTarget	m_dropTarget;
	CWnd*		m_dragWnd;

	int		m_mousemode;
	enum mousemode { mouseNOTHING, mouseSELECTALL, mouseSELECTCOL, mouseSELECTROW, mouseSELECTCELLS,
				mouseSCROLLINGCELLS, mouseOVERROWDIVIDE, mouseOVERCOLDIVIDE,
				mouseSIZINGROW,  mouseSIZINGCOL,
				mouseEDITCELL, mouseCHECKCELL, mouseCOMBOCELL, mouseDATECELL, mouseTIMECELL,
				mousePREPAREDRAG, mouseDRAGGING, mousePREPAREMOVE, mouseMOVING};
	// scroll message
	bool		m_noMessage;
	CCriticalSection m_mergesync;
	CMap <int, int, _fixMerge, _fixMerge&> m_mapMerge;

	bool		m_bCopy;
	bool		m_enableMouseTrace;

	BOOL		m_bFontDrawType;

// 2008. 6. 13. Merge RTS Bug Fix
	BOOL	m_bMergeRTS;

	// 2011.08.17 scbang : 그리드성능개선을 위해 추가
	bool	m_bReadyToDraw;
//	bool	m_bSetDrawTimer;
	int	m_iDrawTimes;

public:
	bool		m_drawHolding;
	CRect		m_drawRect;
	
	CDC		m_memDC;
	CBitmap m_memBuf;
	CBitmap*m_pOldBuf;


public:
	void		beginDrawHolding();
	void		endDrawHolding();

public:
	bool	m_excelF;
	CString	m_excelS;
	HANDLE	m_excelH;
	CCriticalSection m_syncx;

protected:
	BOOL	GetCellOrigin(int nRow, int nCol, LPPOINT pt) const;
	BOOL	GetCellRect(int nRow, int nCol, LPRECT pRect) const;
	BOOL	GetTextRect(int nRow, int nCol, LPRECT pRect);
	
	CSize	GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont = FALSE);
	void	EnsureVisible(CIdCell &cell)       { EnsureVisible(cell.row, cell.col); }
	void	EnsureVisible(int nRow, int nCol);
	BOOL	IsCellVisible(int nRow, int nCol) const;
	BOOL	IsCellVisible(CIdCell cell) const;
	BOOL	IsCellAttribute(CIdCell& cell, UINT attr) const;

	// draw
	BOOL	RedrawCell(const CIdCell& cell, CDC* pDC = NULL);
	BOOL	RedrawCell(int nRow, int nCol, CDC* pDC = NULL);

	void	Print(); 

	CRangeCell GetCellRange() const;
	CRangeCell GetSelectedCellRange(bool onlyrow = false) const;
	void	SetSelectedRange(const CRangeCell& range, BOOL force = FALSE);
	void	SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol, BOOL force = FALSE);
	void	UpdateRowSelectedRange(int row, bool insert);
	void	UpdateColSelectedRange(int col, bool insert);

	BOOL	IsValid(const CIdCell& cell) const;
	BOOL	IsValid(const CRangeCell& range) const;

	BOOL	SortTextItems(int nCol, BOOL bAscending);
	BOOL	SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data = 0);

protected:
	BOOL	RegisterWindowClass();
	LRESULT MessageToParent(int nRow, int nCol, int nMessage);

	BOOL	InvalidateCellRect(const CIdCell& cell);
	BOOL	InvalidateCellRect(const CRangeCell& range);
	void	EraseBkgnd(CDC* pDC);

	BOOL	GetCellRangeRect(const CRangeCell& range, LPRECT pRect) const;

	CCell*	GetCell(int nRow, int nCol) const;
	BOOL	SetCell(int nRow, int nCol, CCell* pCell);

	BOOL	IsRowResizeArea(CPoint& point) const;
	BOOL	IsColResizeArea(CPoint& point) const;

	CIdCell GetCellFromPoint(CPoint point, BOOL bAllowFixedCellCheck = TRUE) const;
	CIdCell GetTopleftNonFixedCell() const;
	CRangeCell GetUnobstructedNonFixedCellRange() const;
	CRangeCell GetVisibleNonFixedCellRange(LPRECT pRect = NULL) const;

	CIdCell SetFocusCell(CIdCell focusCell);
	CIdCell SetFocusCell(int nRow, int nCol);

	void	ResetSelectedRange();
	void	ResetScrollBars();
	int	GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

	BOOL	SortTextItems(int sortcol, BOOL ascending, int srow, int erow);
	BOOL	SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high);

	// Mouse Clicks
	virtual void  OnFixedColumnClick(CIdCell& cell);
	virtual void  OnFixedRowClick(CIdCell& cell);

	// Editing
	virtual CSize GetCellExtent(int nRow, int nCol, CDC* pDC);
	virtual bool  OnEditCell(int nRow, int nCol, UINT nChar);
	virtual void  CreateInPlaceEdit(CRect& rect, DWORD dwStyle, UINT nID,
				int nRow, int nCol, LPCTSTR szText, int limit, int nChar);
	virtual void  OnEndEditCell(int nRow, int nCol, CString str);

	// combobox
	virtual bool OnComboCell(int nRow, int nCol);
	virtual void CreateInPlaceCombo(CRect& rect, DWORD dwStyle, UINT nID,
						int nRow, int nCol, LPCTSTR szText, int active);
	virtual void  OnEndComboCell(int nRow, int nCol, CString str, LPARAM lparam);
	
	// date control
	virtual bool OnDateCell(int nRow, int nCol);
	virtual void CreateInPlaceDate(CRect& rect, UINT nID, LPCTSTR init, int nRow, int nCol, LPCTSTR szText);
	virtual void OnEndDateCell(int nRow, int nCol, CString str);

	// time control
	virtual bool OnTimeCell(int nRow, int nCol);
	virtual void CreateInPlaceTime(CRect& rect, UINT nID, LPCTSTR init, int nRow, int nCol, LPCTSTR szText);
	virtual void OnEndTimeCell(int nRow, int nCol, CString str);

	// Drawing
	virtual void  OnDraw(class CMemDC* pDC);
	virtual BOOL  DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);
	virtual BOOL  DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);

	// Cell Creation and Cleanup
	virtual CCell* CreateCell(int nRow, int nCol);
	virtual void EmptyCell(CCell* pCell, int nRow, int nCol);
	
	// selection cells
	void	SelectAllCells();
	void	SelectColumns(CIdCell current);
	void	SelectRows(CIdCell current);
	void	SelectCells(CIdCell current);
	void	OnSelecting(const CIdCell& current);

	COLORREF DisplayAttribute(int attr, CString& data);
	CString CommaModify(CString string);

	void	drawContrast(CDC *pDC, CRect cellRc, CString sText, COLORREF clr);
	COLORREF GetColumnColor(int row, int color);
	void	SetColorSEQ(COLORREF color1, COLORREF color2);
	void	drawIndicate(CRect rect, COLORREF color = RGB(255, 128, 0));
	double	TOfabs(CString string);

public:
	void	OnBeginDrag();
	DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	void	OnDragLeave();
	BOOL	OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxGrid)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	virtual void OnEditCut();
	virtual void OnEditCopy();
	virtual void OnEditPaste();
	virtual void OnEditSelectAll();

	virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

// Operations
public:
	virtual void PrintColumnHeadings(CDC *pDC, CPrintInfo *pInfo);
	virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
	virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);

	virtual CImageList* CreateDragImage(CPoint *pHotSpot);

	virtual void CutSelectedText();
	virtual COleDataSource* CopyTextFromGrid();
	virtual BOOL PasteTextToGrid(CIdCell cell, COleDataObject* pDataObject);

	virtual COleDataSource*    DragRowAction();
	virtual BOOL IsValidDrop(COleDataObject*pDataObject);
	virtual BOOL DropRowAction(COleDataObject* pDataObject);
	virtual BOOL MoveColAction(int sCol, int dCol);

// Implementation
public:
	virtual ~CfxGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxGrid)
	afx_msg void OnPaint();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSysColorChange();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
	afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);
	afx_msg void OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndInPlaceCombo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndInPlaceDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndInPlaceTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeInPlaceCombo(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

__forceinline CCell* CfxGrid::GetCell(int nRow, int nCol) const
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return NULL;

	GRID_ROW* pRow = m_aryGridRows[nRow];
	if (!pRow) return NULL;
	return pRow->GetAt(nCol);
}

__forceinline BOOL CfxGrid::SetCell(int nRow, int nCol, CCell* pCell)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return FALSE;

	GRID_ROW* pRow = m_aryGridRows[nRow];
	if (!pRow) return FALSE;

	pRow->SetAt(nCol, pCell);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXGRID_H__DCA29DDF_7EE1_491F_BD8E_7C1F13E0DE22__INCLUDED_)
