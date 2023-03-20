// ColumnSet.h: interface for the CColumnSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLUMNSET_H__366BC96A_EF4D_4140_A632_CA142448DC0F__INCLUDED_)
#define AFX_COLUMNSET_H__366BC96A_EF4D_4140_A632_CA142448DC0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "../Include_Analysis/ToolBaseData.h"
#include "DequeList.h"

class CRowSet;
class CElement;
class CBlockIndex;
class CExtractingDataOfModifyType;
class COrder;

class CColumnSet  
{
public:
	static CColumnSet* Make();
	static void Delete(CColumnSet* pColumnSet);

public:
	virtual ~CColumnSet();

	void SetAutoPrice(const bool bIsAutoPrice);

	// element �� �����ϱ�.
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

	//[A00000552]alzioyes_20110628 element ���β� �����ϱ�.
	void ChangeAllElementsThick(int nElementType, int nNewWidth);

	// ������ ��� element data.
	CString GetSavedElementsData(const COrder& order) const;
	// �ش� elements ��������.
	void GetElementList(const int sourceRowIndex, CDequePointerList<CElement>& elementList);

	// list�� element �߰�.
	bool AddRowSet();
	void AddElement(const int rowIndex, CElement* pElement);
	// block �߰��� elementTree�� �߰��ϱ�.
	bool InsertRowSet(const int rowIndex);

	// CRowSet ����.
	void DeleteAllRowSet();
	void DeleteRowSet(const int rowNumber);
	// Ư�� tyle�� element�� ����
	bool DeleteElement(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList);
	// element ����
	bool DeleteElement(CElement* pElement);
	// element ����
	bool DeleteElement(const long lElementAddres,BOOL bListOnly);

	// rowSet���� �̵�
	bool MoveRowSet(const int rowIndex, const int targetRowIndex);
	// element ã��
	CElement* FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, const int columnIndex, CToolEnumData::ModifyType& modifyType);

	// element �׸���
	void DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock,const int columnIndex);
	CElement *CopyElement(CElement* pSelectedElement);
	long MoveElement(CElement* pSelectedElement);

	CBlockIndex GetBlockIndexInElement(const CElement* pSelectedElement, const int columnIndex) const;
	
private:
	CDequePointerList<CRowSet> m_RowSetList;

	// RowSet�� �Էµ� ����ū �߰��Ѵ�.
	void AddRowSet(const int count);
	// CRowSet ���� ����� �ű⿡ element �߰�
	void AddElementInNewMakedRowSet(const int rowIndex, CElement* pElement);
	// CRowSet�� element �߰�
	void AddElement(CRowSet* pRowSet, CElement* pElement);
	// CRowSet ����.
	void DeleteRowSet(CRowSet* pRowSet);

	// list�� �����ϴ� RowSet�� Move�� ���.
	bool MoveRowSet_InRowSetList(const int sourceRowIndex, const int targetRowIndex);
	
	int GetMax(const int x1, const int x2) const;
};

#endif // !defined(AFX_COLUMNSET_H__366BC96A_EF4D_4140_A632_CA142448DC0F__INCLUDED_)
