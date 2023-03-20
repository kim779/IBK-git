// ElementSaverBuilder.h: interface for the CElementSaverBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTSAVERBUILDER_H__5FF07B15_C190_4372_B7ED_C378E96DCC35__INCLUDED_)
#define AFX_ELEMENTSAVERBUILDER_H__5FF07B15_C190_4372_B7ED_C378E96DCC35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "OrderSet.h"
#include "Order.h"
#include "Tree.h"

class CDrawingSymbol;
class CSymbolData;
class CElementSaverBuilder  
{
public:
	//string ���
	CString GetSavedSymbolsData(const COrder &order, CList<CDrawingSymbol*, CDrawingSymbol*>* pSymbolList);

	//string�޾Ƽ� symbol��ü �ٽ� �����.
	// 2011.02.07 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//void BuildSymbolsFromString(const CString &data, CTree& tree, COrder &order);
	void BuildSymbolsFromString(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data, CTree& tree, COrder &order);
	// 2011.02.07 by SYS <<

	// Undo / Redo
	CString GetSavedSymbolData_ForUndoRedo(const COrder &order, CDrawingSymbol* pSymbol);
	long BuildSymbolsFromString_ForUndoRedo(CSymbolData& symbolData, const CBlockIndex& blockIndex, CString& strSymbolData, 
		CTree& tree, COrder& order);
	void AddElement_ForUndoRedo(const COrderSet& orderset, COrder &order);

private:
	//��ü ���鶧 ���.
	void AddElement(const COrderSet& orderset, COrder &order);
	
	// 2011.02.07 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//void BuildSymbolsFromString_Row(const CString &data, const int nColumn, const int nRow, CTree& tree, COrderSet& orderset);
	//void BuildSymbolsFromString_Column(const CString &data, const int nColumn, CTree& tree, COrderSet& orderset);
	//void BuildSymbolsFromString_Symbol(const CString &data, const int nColumn, const int nRow, CTree& tree, COrderSet& orderset);
	//CDrawingSymbol* MakeElement(const CString& strSymboldata);

	void BuildSymbolsFromString_Row(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data, const int nColumn, const int nRow, CTree& tree, COrderSet& orderset);
	void BuildSymbolsFromString_Column(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data, const int nColumn, CTree& tree, COrderSet& orderset);
	void BuildSymbolsFromString_Symbol(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data, const int nColumn, const int nRow, CTree& tree, COrderSet& orderset);
	CDrawingSymbol* MakeElement(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString& strSymboldata);
	// 2011.02.07 by SYS <<

	//symboldata�� ��´�.
	CString GetIndexString(const CDrawingSymbol* pSymbol, const COrder& order) const;
	LOGFONT MakeLOGFONTFromString(const CString& data);
	CString MakeStringFromLOGFONT(const LOGFONT& logfont);

	// 2011.02.07 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	__int64 MakeDateTime(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, CString strX);
	// 2011.02.07 by SYS <<
};

#endif // !defined(AFX_ELEMENTSAVERBUILDER_H__5FF07B15_C190_4372_B7ED_C378E96DCC35__INCLUDED_)
