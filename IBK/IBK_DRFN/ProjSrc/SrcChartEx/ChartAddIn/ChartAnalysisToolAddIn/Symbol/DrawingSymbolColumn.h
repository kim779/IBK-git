// DrawingSymbolColumn.h: interface for the CDrawingSymbolColumn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGSYMBOLCOLUMN_H__7F96658D_7FAF_408D_97D8_179F21BC5DD6__INCLUDED_)
#define AFX_DRAWINGSYMBOLCOLUMN_H__7F96658D_7FAF_408D_97D8_179F21BC5DD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

#include "Order.h"

class CDrawingSymbolRow;
class CDrawingSymbol;
class CDrawingSymbolColumn  
{
public:
	virtual ~CDrawingSymbolColumn();
	//��ȣ�� add
	bool AddRowSet();
	void Add(const int rowIndex, CDrawingSymbol *pDrawingSymbol);

	//��� Symbol�� �׸���.
	void OnDraw(CDC *pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected, const int nColumnIndex);
	long CopyElement(CDrawingSymbol *pSelected);
	long MoveElement(CDrawingSymbol *pSelected);

	//m_RowBlockDrawingSymbolMgrList�� HeadPosition, Next�� ��´�. 
	CDrawingSymbolRow* GetAt(const int nIndex);
	int GetCount();

	CString GetSavedSymbolsData(const COrder& order);
	//Block�� Row�� �ٲ������ Row�� �����ϴ� m_DrawingSymbolRowList �����Ѵ�.
	void ChangeRowList(const int nSourceRow, const int nTargetRow);

	//Row�� Insert�Ѵ�.
	bool InsertRow(const int ninsertRowIndex);

	//�ش� Symbol�� �����.
	void DeleteAllSymbols();
	//������� ������ list���� ��������� �غ�.
	void GetElementList(const int nRowIndex, CList<CDrawingSymbol*, CDrawingSymbol*>& elementList);
	void DeleteAllSymbolInRow(const int nRow);
	bool DeleteCurrentDrawingSymbol(const CDrawingSymbol *pDrawingSymbol);
	bool DeleteCurrentDrawingSymbol(const long lAddressSymbol,BOOL bListOnly);

	void ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor);

	// Undo / Redo
	CBlockIndex GetBlockIndexInSymbol(const CDrawingSymbol* pSelectedSymbol, const int columnIndex) const;
	
private:
	int GetMax(const int nSourceRow, const int nTargetRow);
	void ChangeList(const int nSourceRow, const int nTargetRow);
	void AddRow(const int nRowCount);
	void AddElement(CDrawingSymbolRow* pRow, CDrawingSymbol *pDrawingSymbol);
	void AddElementInNewMakedRowSet(const int rowIndex, CDrawingSymbol *pDrawingSymbol);

	//Row ����.
	CList<CDrawingSymbolRow*, CDrawingSymbolRow* > m_DrawingSymbolRowList; 
};

#endif // !defined(AFX_DRAWINGSYMBOLCOLUMN_H__7F96658D_7FAF_408D_97D8_179F21BC5DD6__INCLUDED_)
