// RowSet.h: interface for the CRowSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROWSET_H__B2CFF04A_0FC8_405C_A314_C4C6F208D148__INCLUDED_)
#define AFX_ROWSET_H__B2CFF04A_0FC8_405C_A314_C4C6F208D148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/ToolBaseData.h"
#include "DequeList.h"

class CElement;
class CDrawingRegions;
class CCoordinateCalculatorData;
class CExtractingDataOfModifyType;
class COrder;

class CRowSet  
{
public:
	static CRowSet* Make();
	static void Delete(CRowSet* pRowSet);

public:
	virtual ~CRowSet();

	void SetAutoPrice(const bool bIsAutoPrice);

	// element �� �����ϱ�.
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

	//[A00000552]alzioyes_20110628 element ���β� �����ϱ�.
	void ChangeAllElementsThick(int nElementType, int nNewWidth);

	// element count
	int GetElementCount() const;
	// ������ ��� element data.
	CString GetSavedElementsData(const COrder& order) const;
	// �ش� elements ��������.
	void GetElementList(CDequePointerList<CElement>& elementList);
	void GetElementList(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& elementList);


	// list�� element �߰�.
	void AddElement(CElement* pElement);

	// element ����.
	void DeleteAll();
	bool Delete(const long lElementAddres,BOOL bListOnly);
	bool Delete(const CToolEnumData::ElementType eElementTYPE, CDequePointerList<CElement>& deletedElementList);
	bool Delete(CElement* pElement, const bool bCheckSameElement = true);

	// element ã��
	CElement* FindElement(CExtractingDataOfModifyType* pExtractingData, CToolEnumData::ModifyType& modifyType);

	// element Copy
	CElement *CopyElement(CElement* pSelectedElement);
	long MoveElement(CElement* pSelectedElement);
	// element �׸���.
	void DrawElement(CDC* pDC, CElement* pSelectedElement,
					const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData);
	// list�� ���� element�� ������ = true
	bool IsInSameElement(const CElement* pElement) const;

private:
	CDequePointerList<CElement> m_ElementList;

	// element�� �׸��� ����. (�׸��� = true)
	bool WillDrawElement(const CDrawingRegions& drawingRegions) const;
};

#endif // !defined(AFX_ROWSET_H__B2CFF04A_0FC8_405C_A314_C4C6F208D148__INCLUDED_)
