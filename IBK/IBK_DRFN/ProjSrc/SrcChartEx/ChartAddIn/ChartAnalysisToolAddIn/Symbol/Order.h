// Order.h: interface for the COrder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDER_H__1B512349_93CD_4CF7_B989_ADED37F4E53B__INCLUDED_)
#define AFX_ORDER_H__1B512349_93CD_4CF7_B989_ADED37F4E53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

class CDrawingSymbol;
class COrder  
{
public:
	CDrawingSymbol* GetTail();
	virtual ~COrder();

	int GetCount() const;
	
	//��ȣ�� �߰��Ǿ����� add�Ѵ�.
	void Add(CDrawingSymbol *pDrawingSymbol);

	int GetIndex(const CDrawingSymbol *pDrawingSymbol) const;

	//�ش� Symbol�� �����.
	void RemoveAll();
	void RemoveAllInRow(const CList<CDrawingSymbol* , CDrawingSymbol*>& symbolsInRowList);
	bool RemoveAt(const CDrawingSymbol *pDrawingSymbol);

private:
	//symbol ���� ����Ʈ.
	CList<CDrawingSymbol*, CDrawingSymbol* > m_OrderList; 
};

#endif // !defined(AFX_ORDER_H__1B512349_93CD_4CF7_B989_ADED37F4E53B__INCLUDED_)
