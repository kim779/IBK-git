// ElementTree.cpp: implementation of the CElementTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElementTree.h"

#include "Element.h"
#include "AnalysisData.h"
#include "ColumnSet.h"
#include "ElementTreeBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class COrder

int COrder::GetCount() const
{
	return m_ElementOrderList.GetCount();
}

// �ش� element�� ���� ��������.
int COrder::GetIndex(const CElement* pElement) const
{
	return m_ElementOrderList.GetIndex(pElement);
}

// ----------------------------------------------------------------------------
// element �߰�
void COrder::AddTail(CElement* pElement)
{
	assert(pElement != NULL);
	m_ElementOrderList.AddTail(pElement);
}

// ----------------------------------------------------------------------------
// list�� ��� �����
void COrder::RemoveAll()
{
	m_ElementOrderList.RemoveAll();
}

// list�� �����
void COrder::Remove(const CDequePointerList<CElement>& elementList)
{
	for(int nIndex = 0; nIndex < elementList.GetCount(); nIndex++){
		CElement* pElement = elementList.GetAt(nIndex);
		assert(pElement != NULL);
		Remove(pElement);
	}
}

// ������ �����.
void COrder::RemoveTail()
{
	if(m_ElementOrderList.GetCount() <= 0)
		return;

	m_ElementOrderList.RemoveTail();
}

// list�� �����
void COrder::Remove(CElement* pElement)
{
	assert(pElement != NULL);
	m_ElementOrderList.RemoveAt(pElement);
}

// ----------------------------------------------------------------------------
// element ���� �ٲٱ�
bool COrder::ChangeOrder(CElement* pElement)
{
	assert(pElement != NULL);
	for(int nIndex = 0; nIndex < m_ElementOrderList.GetCount(); nIndex++){
		if(!IsChangeOrder(pElement, nIndex))
			continue;
		
		m_ElementOrderList.RemoveAt(nIndex);
		m_ElementOrderList.AddTail(pElement);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
// list�� ������ data ��������.
CElement* COrder::GetTail()
{
	return m_ElementOrderList.GetTail();
}

CElement* COrder::GetAt(int nIndex)
{
	return m_ElementOrderList.GetAt(nIndex);
}

// private ====================================================================
// element ������ �ٲ����� ����
bool COrder::IsChangeOrder(CElement* pElement, const int nIndex) const
{
	return (pElement == m_ElementOrderList.GetAt(nIndex) && nIndex < m_ElementOrderList.GetCount());
}


///////////////////////////////////////////////////////////////////////////////
// class CTree

CTree::~CTree()
{
	DeleteAll();
}

// ----------------------------------------------------------------------------
void CTree::SetAutoPrice(const bool bIsAutoPrice)
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->SetAutoPrice(bIsAutoPrice);
	}
}

// ----------------------------------------------------------------------------
// element �� �����ϱ�.
void CTree::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->ChangeAllElementsColor(penColor, textColor);
	}
}

//[A00000552]alzioyes_20110628 element ���β� �����ϱ�.
void CTree::ChangeAllElementsThick(int nElementType, int nNewWidth)
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->ChangeAllElementsThick(nElementType, nNewWidth);
	}
}
// ----------------------------------------------------------------------------
// ������ ��� element data.
CString CTree::GetSavedElementsData(const COrder& order) const
{
	CString savedElementsData;
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		CString savedColumnData = pColumnSet->GetSavedElementsData(order);
		savedElementsData += pColumnSet->GetSavedElementsData(order);
	}

	return savedElementsData;
}

// �ش� elements ��������.
void CTree::GetElementList(const CBlockIndex& blockIndex, CDequePointerList<CElement>& elementList)
{
	elementList.RemoveAll();

	if(blockIndex.GetColumn() < 0 || blockIndex.GetColumn() >= m_ColumnSetList.GetCount())
		return;

	CColumnSet* pColumnSet = m_ColumnSetList.GetAt(blockIndex.GetColumn());
	assert(pColumnSet != NULL);
	pColumnSet->GetElementList(blockIndex.GetRow(), elementList);
}

// ----------------------------------------------------------------------------
// list�� element �߰�.
void CTree::Add(const CBlockIndex& blockIndex, CElement* pElement)
{
	if(blockIndex.GetColumn() < m_ColumnSetList.GetCount()){
		AddElement(blockIndex.GetRow(), m_ColumnSetList.GetAt(blockIndex.GetColumn()), pElement);
		return;
	}

	AddElementInNewMakedColumnSet(blockIndex, pElement);
}

void CTree::AddColumnRowSet(const CBlockIndex& blockIndex)
{
	AddColumnSet(blockIndex.GetColumn() - m_ColumnSetList.GetCount() +1);
	CColumnSet* pColumnSet = m_ColumnSetList.GetTail();
	assert(pColumnSet != NULL);
	pColumnSet->AddRowSet();	
}
// ----------------------------------------------------------------------------
// block �߰��� elementTree�� �߰��ϱ�.
bool CTree::InsertElementTree(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockIndex)
{
	if(!IsInsertElementTree(blockIndex))
		return false;

	// column ���� block ������ ��쿣 columnSet�� �����.
	// row ���� block ������ ��쿣 ������ columnSet�� rowSet�� �߰��Ѵ�.
	if(blockType == CBlockBaseData::COLUMN)
		return InsertColumnSet(blockIndex.GetColumn());
	else if(blockType == CBlockBaseData::ROW)
		return InsertRowSet(blockIndex);

	return false;
}

// ----------------------------------------------------------------------------
// ��� CColumnSet ����
void CTree::DeleteAll()
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet::Delete(m_ColumnSetList.GetAt(nIndex));
	}

	m_ColumnSetList.RemoveAll();
}

// CRowSet ����
void CTree::DeleteRowSet(const CBlockIndex& blockIndex)
{
	if(blockIndex.GetColumn() < 0 || blockIndex.GetColumn() >= m_ColumnSetList.GetCount())
		return;

	CColumnSet* pColumnSet = m_ColumnSetList.GetAt(blockIndex.GetColumn());
	assert(pColumnSet != NULL);
	pColumnSet->DeleteRowSet(blockIndex.GetRow());
}

// Ư�� tyle�� element�� ����
// ������ ��쿡�� �ش� element�� point�� list�� ��´�.
// -> ��ü�� ������ ������ ����Ǵ� class������ remove �ؾ� �ϱ⶧��.
bool CTree::DeleteElement(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList)
{
	deletedElementList.RemoveAll();

	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->DeleteElement(eElementTYPE, deletedElementList);
	}

	return (deletedElementList.GetCount() > 0);
}

// element ����
void CTree::DeleteElement(CElement* pElement)
{
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		if(pColumnSet->DeleteElement(pElement))
			return;
	}
}

// element ����
bool CTree::DeleteElement(const long lElementAddress,BOOL bListOnly)
{
	bool bFind = false;
	assert(lElementAddress != 0);
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);		
		bFind = pColumnSet->DeleteElement(lElementAddress,bListOnly);
		if(bFind == true)
			return true;
	}

	return false;
}
// ----------------------------------------------------------------------------
// block �̵�
bool CTree::MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
{
	if(!IsMoveElementTree(sourceBlockIndex, targetBlockIndex))
		return false;

	// column ���� �̵�
	if(sourceBlockIndex.GetColumn() != targetBlockIndex.GetColumn())
		return MoveColumnSet(sourceBlockIndex.GetColumn(), targetBlockIndex.GetColumn());

	// ���� column �ȿ��� row���� �̵�
	if(sourceBlockIndex.GetColumn() < m_ColumnSetList.GetCount())
		return MoveRowSet(m_ColumnSetList.GetAt(sourceBlockIndex.GetColumn()), sourceBlockIndex.GetRow(), targetBlockIndex.GetRow());

	return false;
}

// ----------------------------------------------------------------------------
// element ã��
CElement* CTree::FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType)
{
	if( !pMainBlock || !IsFindElement(pMainBlock, pExtractingData)) return NULL;

	int nRow, nColumn;
	if(!pMainBlock->GetBlockIndex( pExtractingData->GetMousePoint(), nRow, nColumn)) return NULL;
	CBlockIndex blockIndexInPoint( nRow, nColumn);
	if(!IsFindElement(blockIndexInPoint))
		return NULL;

	CColumnSet* pColumnSet = m_ColumnSetList.GetAt(blockIndexInPoint.GetColumn());
	assert(pColumnSet != NULL);
	return pColumnSet->FindElement(pMainBlock, pExtractingData, blockIndexInPoint.GetColumn(), modifyType);
}

// ----------------------------------------------------------------------------
// element �׸���.
void CTree::DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock)
{
	if(pMainBlock == NULL)
		return;

	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		pColumnSet->DrawElement(pDC, pSelectedElement, pMainBlock, nIndex);
	}
}

CElement *CTree::CopyElement(CElement* pSelectedElement)
{
	CElement *lAddress = NULL;
	CElement*lAddressTemp = NULL;
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		lAddressTemp = pColumnSet->CopyElement(pSelectedElement);
		if(lAddressTemp != 0)
			lAddress = lAddressTemp;
	}
	return lAddress;
}

long CTree::MoveElement(CElement* pSelectedElement)
{
	long lAddress = 0;
	for(int nIndex = 0; nIndex < m_ColumnSetList.GetCount(); nIndex++){
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(nIndex);
		assert(pColumnSet != NULL);
		lAddress = pColumnSet->MoveElement(pSelectedElement);
		if(lAddress == 0)
			break;
	}
	return lAddress;
}

CBlockIndex CTree::GetBlockIndexInElement(const CElement* pSelectedElement) const
{
	if(pSelectedElement == NULL)
		return CBlockIndex();

	CBlockIndex blockIndex;
	for(int columnIndex = 0; columnIndex < m_ColumnSetList.GetCount(); ++columnIndex)
	{
		CColumnSet* pColumnSet = m_ColumnSetList.GetAt(columnIndex);
		assert(pColumnSet != NULL);
		blockIndex = pColumnSet->GetBlockIndexInElement(pSelectedElement, columnIndex);
		if(blockIndex.IsAllValueBiggerThanZero())
			break;
	}
	return blockIndex;
}

// private ====================================================================
// columnSet�� ����� list�� �߰�
void CTree::AddColumnSet(const int count)
{
	for(int i = 0; i < count; i++){
		CColumnSet* pColumnSet = CColumnSet::Make();
		m_ColumnSetList.AddTail(pColumnSet);
	}
}

// CColumSet�� ���� ����� �ű⿡ element �߰�
void CTree::AddElementInNewMakedColumnSet(const CBlockIndex& blockIndex, CElement* pElement)
{
	AddColumnSet(blockIndex.GetColumn() - m_ColumnSetList.GetCount() +1);
	CColumnSet* pColumnSet = m_ColumnSetList.GetTail();
	assert(pColumnSet != NULL);
	AddElement(blockIndex.GetRow(), pColumnSet, pElement);
}

// CColumnSet�� element �߰�
void CTree::AddElement(const int rowIndex, CColumnSet* pColumnSet, CElement* pElement)
{
	assert(pColumnSet != NULL);
	pColumnSet->AddElement(rowIndex, pElement);
}

// ----------------------------------------------------------------------------
// Set�� �߰������� ���� �˻�
bool CTree::IsInsertElementTree(const CBlockIndex& blockIndex) const
{
	// �߰��� block�� ���� ���.
	if(!blockIndex.IsAllValueBiggerThanZero())
		return false;

	// �߰��� block�� columnSet�� �ִ� list�� ������ ��� ���.
	return (m_ColumnSetList.GetCount() > 0 && blockIndex.GetColumn() < m_ColumnSetList.GetCount());
}

// columnSet�� �߰�.
bool CTree::InsertColumnSet(const int insertColumnIndex)
{
	CColumnSet* pColumnSet = CColumnSet::Make();
	assert(pColumnSet != NULL);
	m_ColumnSetList.InsertItem(insertColumnIndex, pColumnSet);
	return true;
}

// rowSet�� �߰�.
bool CTree::InsertRowSet(const CBlockIndex& insertBlockIndex)
{
	CColumnSet* pColumnSet = m_ColumnSetList.GetAt(insertBlockIndex.GetColumn());
	assert(pColumnSet != NULL);
	return pColumnSet->InsertRowSet(insertBlockIndex.GetRow());
}

// ----------------------------------------------------------------------------
// Set�� �̵��� �������� ����
bool CTree::IsMoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex) const
{
	if(!sourceBlockIndex.IsAllValueBiggerThanZero() || !targetBlockIndex.IsAllValueBiggerThanZero())
		return false;
	if(sourceBlockIndex.GetColumn() >= m_ColumnSetList.GetCount() && 
		sourceBlockIndex.GetColumn() >= m_ColumnSetList.GetCount())
		return false;

	return true;
}

// columnSet���� �̵�
bool CTree::MoveColumnSet(const int sourceColumnIndex, const int targetColumnIndex)
{
	// sourceColumnIndex�� m_ColumnSetList ������ �ƴ� ���.
	// -> ���ο� ColumnSet�� insert�Ѵ�.
	if(sourceColumnIndex > (m_ColumnSetList.GetCount() -1)){
		m_ColumnSetList.InsertItem(targetColumnIndex, CColumnSet::Make());
		return true;
	}
	
	// list�� �����ϴ� ColumnSet�� Move�ϴ� ���.
	return MoveColumnSet_InColumnSetList(sourceColumnIndex, targetColumnIndex);
}

// list�� �����ϴ� ColumnSet�� Move�ϴ� ���.
bool CTree::MoveColumnSet_InColumnSetList(const int sourceColumnIndex, const int targetColumnIndex)
{
	if(sourceColumnIndex >= m_ColumnSetList.GetCount())
		return false;

	int nMaxIndex = GetMax(sourceColumnIndex, targetColumnIndex);

	// ���� ColumnSet �����
	if(nMaxIndex > m_ColumnSetList.GetCount())
		AddColumnSet(nMaxIndex - m_ColumnSetList.GetCount());

	// �̵�
	m_ColumnSetList.MoveItem(sourceColumnIndex, targetColumnIndex);
	return true;
}

// ���� columnSet�� rowSet���� �̵�
bool CTree::MoveRowSet(CColumnSet* pColumnSet, const int sourceRowIndex, const int targetRowIndex)
{
	assert(pColumnSet != NULL);
	if(sourceRowIndex == targetRowIndex)
		return false;

	return pColumnSet->MoveRowSet(sourceRowIndex, targetRowIndex);
}

int CTree::GetMax(const int x1, const int x2) const
{
	return (x1 > x2) ? x1: x2;
}

// ----------------------------------------------------------------------------
// element ã������ ����
bool CTree::IsFindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData) const
{
	return (pMainBlock == NULL || pExtractingData == NULL || pExtractingData->IsFindElement());
}

bool CTree::IsFindElement(const CBlockIndex& blockIndex) const
{
	return (blockIndex.IsAllValueBiggerThanZero() && blockIndex.GetColumn() < m_ColumnSetList.GetCount());
}


///////////////////////////////////////////////////////////////////////////////
// class CElementTree

void CElementTree::SetAutoPrice(const bool bIsAutoPrice)
{
	m_tree.SetAutoPrice(bIsAutoPrice);
}

// element ã��
void CElementTree::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	m_tree.ChangeAllElementsColor(penColor, textColor);
}

//[A00000552]alzioyes_20110628 element ���β� �����ϱ�.
void CElementTree::ChangeAllElementsThick(int nElementType, int nNewWidth)
{
	m_tree.ChangeAllElementsThick(nElementType, nNewWidth);
}

// ----------------------------------------------------------------------------
// ���� �ִ� element count
int CElementTree::GetElementCount() const
{
	return m_order.GetCount();
}

void CElementTree::GetElementListOfBlock(const CBlockIndex& blockIndex, CDequePointerList<CElement>& elementList)
{
	elementList.RemoveAll();
	m_tree.GetElementList(blockIndex, elementList);
}

// ������ ��� element data.
CString CElementTree::GetSavedElementsData() const
{
	return m_tree.GetSavedElementsData(m_order);
}

// ----------------------------------------------------------------------------
// element �����.
void CElementTree::BuildElementsFromString(CAnalysisData& analysisData,const CString& strElementsData)
{
	DeleteAll();

	if(strElementsData.IsEmpty())
		return;

	CElementTreeBuilder builder( m_hOcxWnd, m_eChartMode);
	builder.BuildElementsFromString(analysisData,strElementsData, m_tree, m_order);
}

// list�� element �߰�.
void CElementTree::Add(const CBlockIndex& blockIndex, CElement* pElement)
{
	assert(blockIndex.IsAllValueBiggerThanZero() && pElement != NULL);

	m_order.AddTail(pElement);
	m_tree.Add(blockIndex, pElement);
}

void CElementTree::AddColumnRowSet(const CBlockIndex& blockIndex)
{
	assert(blockIndex.IsAllValueBiggerThanZero());
	m_tree.AddColumnRowSet(blockIndex);
}

// ----------------------------------------------------------------------------
// block �߰��� elementTree�� �߰��ϱ�.
bool CElementTree::InsertElementTree(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockIndex)
{
	return m_tree.InsertElementTree(blockType, blockIndex);
}

// ----------------------------------------------------------------------------
// ��� CColumnSet ����
void CElementTree::DeleteAll()
{
	if(m_order.GetCount() <= 0)
		return;

	m_order.RemoveAll();
	m_tree.DeleteAll();
}

// CRowSet ����
void CElementTree::DeleteRowSet(const CBlockIndex& blockIndex)
{
	CDequePointerList<CElement> elementList;
	m_tree.GetElementList(blockIndex, elementList);

	if(elementList.GetCount() > 0)
		m_order.Remove(elementList);

	m_tree.DeleteRowSet(blockIndex);
}

// element ���������� �����.
void CElementTree::DeleteTail()
{
	CElement* pElement = m_order.GetTail();
	if(pElement == NULL)
		return;

	DeleteElement(pElement);
}

// Ư�� element�� ����
bool CElementTree::DeleteElement(const CToolEnumData::ElementType eElementTYPE)
{
	// �ش� type�� element�� �����ϰ� �� point�� ���Ѵ�.
	CDequePointerList<CElement> deletedElementList;
	if(!m_tree.DeleteElement(eElementTYPE, deletedElementList))
		return false;

	m_order.Remove(deletedElementList);
	return true;
}

// element ����
void CElementTree::DeleteElement(CElement* pElement)
{
	assert(pElement != NULL);

	m_order.Remove(pElement);
	m_tree.DeleteElement(pElement);
}

// element ����
bool CElementTree::DeleteElementPos(const long lAddress,BOOL bListOnly)
{
	if(GetElementCount() <= 0 || lAddress == 0)
		return FALSE;

	//Order�� ���� ������ �ȵȴ�
	bool bDeleted = m_tree.DeleteElement(lAddress,bListOnly);		
	if(bDeleted == true)
	{
		CElement *pElement = (CElement *)lAddress;
		m_order.Remove(pElement);
	}
	return bDeleted;
}

// ----------------------------------------------------------------------------
// block �̵�
bool CElementTree::MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
{
	return m_tree.MoveElementTree(sourceBlockIndex, targetBlockIndex);
}

// element ����(order) �ٲٱ�
bool CElementTree::ChangeElementOrder(CElement* pElement)
{
	return m_order.ChangeOrder(pElement);
}

// ----------------------------------------------------------------------------
// element ã��
CElement* CElementTree::FindElement(IChartManager* pMainBlock, CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType)
{
	return m_tree.FindElement(pMainBlock, pExtractingData, modifyType);
}

// ----------------------------------------------------------------------------
// element �׸���.
void CElementTree::DrawElement(CDC* pDC, CElement* pSelectedElement, IChartManager* pMainBlock)
{
	m_tree.DrawElement(pDC, pSelectedElement, pMainBlock);
}

CElement *CElementTree::CopyElement(CElement* pSelectedElement)
{
	CElement *lReturn = m_tree.CopyElement(pSelectedElement);
	if(lReturn != 0)
		m_order.AddTail(pSelectedElement);
	return lReturn;
}

long CElementTree::MoveElement(CElement* pSelectedElement)
{	
	long lReturn = m_tree.MoveElement(pSelectedElement);
	if(lReturn == 0)
		m_order.AddTail(pSelectedElement);

	return lReturn;
}

CBlockIndex CElementTree::GetBlockIndexInElement(const CElement* pSelectedElement) const
{
	assert(pSelectedElement != NULL);
	
	return m_tree.GetBlockIndexInElement(pSelectedElement);
}

CElement* CElementTree::GetTail()
{
	return m_order.GetTail();
}

CTree& CElementTree::GetTree()
{
	return m_tree;
}

COrder& CElementTree::GetOrder()
{
	return m_order;
}

// private ====================================================================
