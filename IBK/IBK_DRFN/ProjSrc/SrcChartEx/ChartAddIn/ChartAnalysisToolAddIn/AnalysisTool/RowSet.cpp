// RowSet.cpp: implementation of the CRowSet class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RowSet.h"

#include "Element.h"
#include "ElementFactory.h"
#include "AnalysisData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CRowSet

CRowSet* CRowSet::Make()
{
	return new CRowSet;
}

void CRowSet::Delete(CRowSet* pRowSet)
{
	delete pRowSet;
}


// public =====================================================================
CRowSet::~CRowSet()
{
	DeleteAll();
}

// ----------------------------------------------------------------------------
void CRowSet::SetAutoPrice(const bool bIsAutoPrice)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		pElement->GetCommonEnvironment().SetAutoPrice(bIsAutoPrice);
	}
}

// ----------------------------------------------------------------------------
// element �� �����ϱ�.
void CRowSet::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		pElement->ChangePenColor(penColor);
		pElement->ChangeTextColor(textColor);
	}
}

//[A00000552]alzioyes_20110628 element ���β� �����ϱ�.
void CRowSet::ChangeAllElementsThick(int nElementType, int nNewWidth)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement->GetElementType()==nElementType)
			pElement->ChangePenThick(nNewWidth);
	}
}

// ----------------------------------------------------------------------------
// element count
int CRowSet::GetElementCount() const
{
	return m_ElementList.GetCount();
}

// ������ ��� element data.
CString CRowSet::GetSavedElementsData(const COrder& order) const
{
	CString savedElementsData("<Row>\r\n");
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		savedElementsData += pElement->GetSavedElementData(order);
	}

	return savedElementsData + "<\\Row>\r\n";
}

// �ش� elements ��������.
void CRowSet::GetElementList(CDequePointerList<CElement>& elementList)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		elementList.AddTail(pElement);
	}
}

void CRowSet::GetElementList(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& elementList)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement->GetElementType() == eElementTYPE)
			elementList.AddTail(pElement);
	}
}

// ----------------------------------------------------------------------------
/// list�� element �߰�.
void CRowSet::AddElement(CElement* pElement)
{
	assert(pElement != NULL);
	if(IsInSameElement(pElement))
		return;

	m_ElementList.AddTail(pElement);
}

// ----------------------------------------------------------------------------
// ��� element ����
void CRowSet::DeleteAll()
{
	while(m_ElementList.GetCount() > 0){
		Delete(m_ElementList.GetTail(), false);
	}

	m_ElementList.RemoveAll();
}

// Ư�� type�� element�� ����
// ������ ��쿡�� �ش� element�� point�� list�� ��´�.
// -> ��ü�� ������ ������ ����Ǵ� class������ remove �ؾ� �ϱ⶧��.
bool CRowSet::Delete(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList)
{
	CDequePointerList<CElement> elementList;
	GetElementList(eElementTYPE, elementList);
	if(elementList.GetCount() <= 0)
		return false;

	bool bIsDeleted = false;
	for(int nIndex = 0; nIndex < elementList.GetCount(); nIndex++)
	{
		CElement* pElement = elementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement->GetElementType() != eElementTYPE)
			continue;

		deletedElementList.AddTail(pElement);
		Delete(pElement, false);
		bIsDeleted = true;
	}

	return bIsDeleted;
}

// element ����
// -> bCheckSameElement = true : pElement�� CRowSet�� ��ü���� �˻縦 �Ѵ�.
bool CRowSet::Delete(CElement* pElement, const bool bCheckSameElement)
{
	assert(pElement != NULL);
	if(bCheckSameElement && !IsInSameElement(pElement))
		return false;

	CElementFactory::Delete(pElement);
	m_ElementList.RemoveAt(pElement);
	return true;
}

bool CRowSet::Delete(const long lElementAddres,BOOL bListOnly)
{
	assert(lElementAddres != 0);	

	bool bFind = false;
	long lTempAdress = 0;
	CElement *pElement = NULL;
	for(int i=0;i<GetElementCount();i++)
	{
		pElement = m_ElementList.GetAt(i);
		lTempAdress = (long)pElement;
		if(lTempAdress == lElementAddres)
		{
			if(bListOnly == FALSE)
				CElementFactory::Delete(pElement);
			
			m_ElementList.RemoveAt(pElement);
			bFind = true;
			break;
		}
	}	
	return bFind;
}

// ----------------------------------------------------------------------------
// element ã��
CElement* CRowSet::FindElement(CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType)
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		modifyType = pElement->GetModifyType(pExtractingData);
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return pElement;
	}
	return NULL;
}

// ----------------------------------------------------------------------------
// element �׸���
void CRowSet::DrawElement(CDC* pDC, CElement* pSelectedElement, 
						  const CDrawingRegions& drawingRegions,const CCoordinateCalculatorData& calData)
{
	if(!WillDrawElement(drawingRegions))
		return;

	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		pElement->Draw(pDC,drawingRegions,calData);
		if(pElement == pSelectedElement)
			pElement->DrawShadowAndGrip(pDC, drawingRegions, calData);
	}
}

CElement *CRowSet::CopyElement(CElement* pSelectedElement)
{
	CElement *lAddress = NULL;
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement == pSelectedElement)
		{
			CElement *pNewElement = CElementFactory::Copy(pElement);
			if(pNewElement != NULL)
			{
				lAddress = pNewElement;
				m_ElementList.AddTail(pNewElement);
			}			
			break;
		}
	}

	return lAddress;
}

long CRowSet::MoveElement(CElement* pSelectedElement)
{
	long lAddress = 0;
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++)
	{
		CElement* pElement = m_ElementList.GetAt(nIndex);
		assert(pElement != NULL);
		if(pElement == pSelectedElement)
		{
			lAddress = (long)pSelectedElement;
			break;
		}
	}

	if(lAddress == 0)
		m_ElementList.AddTail(pSelectedElement);

	return lAddress;
}

// list�� ���� element�� ������ = true
bool CRowSet::IsInSameElement(const CElement* pElement) const
{
	for(int nIndex = 0; nIndex < m_ElementList.GetCount(); nIndex++){
		if(pElement == m_ElementList.GetAt(nIndex))
			return true;
	}
	return false;
}

// privata ====================================================================
// element�� �׸��� ����. (�׸��� = true)
bool CRowSet::WillDrawElement(const CDrawingRegions& drawingRegions) const
{
	return (m_ElementList.GetCount() > 0 && drawingRegions.GetCount() > 0);
}

// ----------------------------------------------------------------------------
