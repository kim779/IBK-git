// ElementTree.h: interface for the CElementTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTTREE_H__A3A4BD06_2494_46E3_85DC_01B5249D09E4__INCLUDED_)
#define AFX_ELEMENTTREE_H__A3A4BD06_2494_46E3_85DC_01B5249D09E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "../Include_Analysis/ToolBaseData.h"

#include "../Include_Chart/DequeList.h"
#include "../Include_Chart/BlockBaseData.h"

class CAnalysisData;
class CElement;
class CColumnSet;
class CExtractingDataOfModifyType;

// element�� ���� ����
class COrder
{
public:
	int GetCount() const;
	// �ش� element�� ���� ��������.
	int GetIndex(const CElement* pElement) const;

	// element �߰�
	void AddTail(CElement* pElement);

	// list�� ��� �����.
	void RemoveAll();
	// ������ �����.
	void RemoveTail();
	// list�� �����.
	void Remove(const CDequePointerList<CElement>& elementList);
	// list�� �����.
	void Remove(CElement* pElement);

	// element ���� �ٲٱ�
	bool ChangeOrder(CElement* pElement);

	// list�� ������ data ��������.
	CElement* GetTail();
	CElement* GetAt(int nIndex);

private:
	CDequePointerList<CElement> m_ElementOrderList;

	// element ������ �ٲ����� ����
	bool IsChangeOrder(CElement* pElement, const int nIndex) const;
};


// element�� �� �������� ����
class CTree
{
public:
	virtual ~CTree();

	void SetAutoPrice(const bool bIsAutoPrice);

	// element �� �����ϱ�.
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

	//[A00000552]alzioyes_20110628 element ���β� �����ϱ�.
	void ChangeAllElementsThick(int nElementType, int nNewWidth);

	// ������ ��� element data.
	CString GetSavedElementsData(const COrder& order) const;
	// �ش� elements ��������.
	void GetElementList(const CBlockIndex& blockIndex, CDequePointerList<CElement>& elementList);

	// element �߰�
	void Add(const CBlockIndex& blockIndex, CElement* pElement);
	void AddColumnRowSet(const CBlockIndex& blockIndex);

	// block �߰��� elementTree�� �߰��ϱ�.
	bool InsertElementTree(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockIndex);

	// CColumnSet ����
	void DeleteAll();
	// CRowSet ����
	void DeleteRowSet(const CBlockIndex& blockIndex);
	// Ư�� tyle�� element�� ����
	bool DeleteElement(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList);
	// element ����
	void DeleteElement(CElement* pElement);
	// element ����
	bool DeleteElement(const long lElementAddress,BOOL bListOnly);

	// block �̵�
	bool MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	// element ã��
	CElement* FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType);

	// element �׸���
	void DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock);
	CElement *CopyElement(CElement* pSelectedElement);
	long MoveElement(CElement* pSelectedElement);

	CBlockIndex GetBlockIndexInElement(const CElement* pSelectedElement) const;

private:
	CDequePointerList<CColumnSet> m_ColumnSetList;

	// columnSet�� ����� list�� �߰�
	void AddColumnSet(const int count);
	// CColumSet�� ���� ����� �ű⿡ element �߰�
	void AddElementInNewMakedColumnSet(const CBlockIndex& blockIndex, CElement* pElement);
	// CColumnSet�� element �߰�
	void AddElement(const int rowIndex, CColumnSet* pColumnSet, CElement* pElement);

	// Set�� �߰������� ���� �˻�
	bool IsInsertElementTree(const CBlockIndex& blockIndex) const;
	// columnSet�� �߰�.
	bool InsertColumnSet(const int insertColumnIndex);
	// rowSet�� �߰�.
	bool InsertRowSet(const CBlockIndex& insertBlockIndex);

	// Set�� �̵��� �������� ����
	bool IsMoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex) const;
	// columnSet���� �̵�
	bool MoveColumnSet(const int sourceColumnIndex, const int targetColumnIndex);
	bool MoveColumnSet_InColumnSetList(const int sourceColumnIndex, const int targetColumnIndex);
	// ���� columnSet�� rowSet���� �̵�
	bool MoveRowSet(CColumnSet* pColumnSet, const int sourceRowIndex, const int targetRowIndex);
	int GetMax(const int x1, const int x2) const;


	// element ã������ ����
	bool IsFindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData) const;
	bool IsFindElement(const CBlockIndex& blockIndex) const;
};


// element�� ���õ� tree (order / tree)
class CElementTree
{
public:
	CElementTree( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}

public:
	void SetAutoPrice(const bool bIsAutoPrice);

	// element �� �����ϱ�.
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

	//[A00000552]alzioyes_20110628 element ���β� �����ϱ�.
	void ChangeAllElementsThick(int nElementType, int nNewWidth);

	// ���� �ִ� element count
	int GetElementCount() const;
	// block�� �ִ� element count
	void GetElementListOfBlock(const CBlockIndex& blockIndex, CDequePointerList<CElement>& elementList);
	// ������ ��� element data.
	CString GetSavedElementsData() const;

	// element �����.
	void BuildElementsFromString(CAnalysisData& analysisData,const CString& strElementsData);
	// element �߰�
	void Add(const CBlockIndex& blockIndex, CElement* pElement);
	void AddColumnRowSet(const CBlockIndex& blockIndex);
	// block �߰��� elementTree�� �߰��ϱ�.
	bool InsertElementTree(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockIndex);

	// CColumnSet ����
	void DeleteAll();
	// CRowSet ����
	void DeleteRowSet(const CBlockIndex& blockIndex);
	// element ���������� �����.
	void DeleteTail();
	// Ư�� element�� ����
	bool DeleteElement(const CToolEnumData::ElementType eElementTYPE);
	// element ����
	void DeleteElement(CElement* pElement);
	// element ����
	bool DeleteElementPos(const long lAddress,BOOL bListOnly = FALSE);

	// block �̵�
	bool MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	// element ����(order) �ٲٱ�
	bool ChangeElementOrder(CElement* pElement);
	// element ã��
	CElement* FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType);

	// element �׸���
	void DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock);
	CElement *CopyElement(CElement* pSelectedElement);	
	long MoveElement(CElement* pSelectedElement);	
	
	CBlockIndex GetBlockIndexInElement(const CElement* pSelectedElement) const;
	CElement* GetTail();
	CTree& GetTree();
	COrder& GetOrder();

private:
	COrder m_order;
	CTree m_tree;

// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
};

#endif // !defined(AFX_ELEMENTTREE_H__A3A4BD06_2494_46E3_85DC_01B5249D09E4__INCLUDED_)
