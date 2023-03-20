// ElementTree.h: interface for the CElementTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTTREE_H__D8C0A3A5_5A41_4103_B4E7_FEC4B98765F2__INCLUDED_)
#define AFX_ELEMENTTREE_H__D8C0A3A5_5A41_4103_B4E7_FEC4B98765F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"
#include "Order.h"
#include "Tree.h"

class CRegion_AllBlock;
class CDrawingSymbol;
class CElementTree  
{
public:
	void DeleteLastSymbol();
	bool DeleteSymbolPos(const long lAddress,BOOL bListOnly=FALSE);
	//��ü ����.
	void Add(const CBlockIndex& blockIndex, CDrawingSymbol* pDrawingSymbol);
	void AddColumnRowSet(const CBlockIndex& blockIndex);

	//point�� �ش��ϴ� ���� �� ������ symbol�� �����͸� ��´�.
	CDrawingSymbol* GetTail();
	CDrawingSymbol* FindSymbol(const CPoint &point, IChartManager* pMainBlock);

	// ���� �ִ� element count
	int GetElementCount() const;
	// block�� �ִ� element count
	void GetElementListOfBlock(const CBlockIndex& blockIndex, CList<CDrawingSymbol*, CDrawingSymbol*>& elementList);

	//�����Ǿ� �ִ� ��� ��ü�� draw�Ѵ�.
	void DrawElement(CDC *pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected);
	long CopyElement(CDrawingSymbol *pSelected);	
	long MoveElement(CDrawingSymbol *pSelected);

	//symbol�����
	bool DeleteCurrentDrawingSymbol(const CDrawingSymbol* pDrawingSymbol);
	void DeleteAllSymbolsInBlock(const CBlockIndex &blockIndex);
	void DeleteAllSymbols();

	//block�߰�, �� �̵��� �����ؼ� symbol����Ʈ�� �̵���Ų��.
	bool InsertBlockIndex(const CBlockIndex &insertBlockIndex);
	void SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);

	//��ü ���忡 ���õ� ����.
	CString GetSavedSymbolsData();

	// 2011.02.07 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//void BuildSymbolsFromString(const CString &data);
	void BuildSymbolsFromString(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data);
	// 2011.02.07 by SYS <<

	void ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor);

	CBlockIndex GetBlockIndexInSymbol(const CDrawingSymbol* pSelectedSymbol) const;
	CTree& GetTree();
	COrder& GetOrder();
	
private:
	COrder m_order;
	CTree m_tree;

};

#endif // !defined(AFX_ELEMENTTREE_H__D8C0A3A5_5A41_4103_B4E7_FEC4B98765F2__INCLUDED_)
