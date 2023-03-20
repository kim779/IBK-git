// Tree.h: interface for the CTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREE_H__5BFCF715_9AB7_4186_8DDD_0D3DDB2E9EA2__INCLUDED_)
#define AFX_TREE_H__5BFCF715_9AB7_4186_8DDD_0D3DDB2E9EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "BlockBaseData.h"

//������� ����� ����Ʈ 
#include "Order.h"

class CDrawingSymbolColumn;
class CDrawingSymbolRow;
class CDrawingSymbol;
class CTree  
{
public:
	virtual ~CTree();

	//��� Symbol�� �׸���.
	void OnDraw(CDC *pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected);
	long CopyElement(CDrawingSymbol *pSelected);	
	long MoveElement(CDrawingSymbol *pSelected);

	//data������ string�� �����.
	CString GetSavedSymbolsData(const COrder& order);

	void GetElementList(const CBlockIndex &blockIndex, CList<CDrawingSymbol*, CDrawingSymbol*>& elementList);
	//���� �ش� Row ã��
	CDrawingSymbolRow* GetCurBlockDrawingSymbol(const CBlockIndex& TargetBlockIndex);

	//��ȣ�� �߰��Ǿ����� add�Ѵ�.
	void Add(const CBlockIndex &blockIndex, CDrawingSymbol *pDrawingSymbol);
	void Add(const int nColumn, const int nRow, CDrawingSymbol *pDrawingSymbol);
	void AddColumnRowSet(const CBlockIndex &blockIndex);
	//Row, Column�� �߰�, �����ϰ� ���� ������ �ٽ� �����.
	void SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);

	//Block�� Insert�Ѵ�.
	bool InsertBlockIndex(const CBlockIndex &insertBlockIndex);
	
	//�ش� Symbol�� �����.
	void DeleteAllSymbols();
	void DeleteAllSymbolsInBlock(const CBlockIndex &blockIndex);
	bool DeleteCurrentDrawingSymbol(const CDrawingSymbol *pDrawingSymbol);
	bool DeleteCurrentDrawingSymbol(const long lAddressSymbol,BOOL bListOnly);

	//�߼��� ��� ���� ����
	void ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor);

	// Undo / Redo
	CBlockIndex GetBlockIndexInSymbol(const CDrawingSymbol* pSelectedSymbol) const;

private:
	void AddColumn(const int nColumnCount);
	//Column����Ʈ.
	CList<CDrawingSymbolColumn*, CDrawingSymbolColumn* > m_DrawingSymbolColumnList; 
	//���� Į�� ���.
	CDrawingSymbolColumn* GetCurrentColumn(const int nColumn);

	//��ȣ�� �߰��Ǿ����� add�Ѵ�. 
	//�̹� column�� �������� ���� Column�� ���� �߰��ؾ� �ϴ°��
	void AddElement(const int rowIndex, CDrawingSymbolColumn* pColumn, CDrawingSymbol *pDrawingSymbol);
	void AddElementInNewMakedColumnSet(const CBlockIndex &blockIndex, CDrawingSymbol *pDrawingSymbol);

	//Block�� Row, Column�� ����Ǿ����� m_DrawingSymbolColumnList�� ������ �����Ѵ�.
	void ChangeRowList(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);
	void ChangeColumnList(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);

};

#endif // !defined(AFX_TREE_H__5BFCF715_9AB7_4186_8DDD_0D3DDB2E9EA2__INCLUDED_)
