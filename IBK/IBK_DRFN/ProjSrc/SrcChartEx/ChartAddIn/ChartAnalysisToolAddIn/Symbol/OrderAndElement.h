// OrderAndElement.h: interface for the COrderAndElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERANDELEMENT_H__C0D13D1D_0093_49F5_A4B8_B3DA4C0FFAAA__INCLUDED_)
#define AFX_ORDERANDELEMENT_H__C0D13D1D_0093_49F5_A4B8_B3DA4C0FFAAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//��ü�� ���Ͽ� �����ߴٰ� �ٽ� �����Ҷ� ������� �����ϱ����� �߰������� �ʿ��� class
class CDrawingSymbol;
class COrderAndElement  
{
public:
	COrderAndElement(const int nIndex, CDrawingSymbol *pDrawingSymbol);

	int GetIndex();
	CDrawingSymbol* GetElement();

private:
	int m_nIndex;
	CDrawingSymbol* m_pDrawingSymbol;

};

#endif // !defined(AFX_ORDERANDELEMENT_H__C0D13D1D_0093_49F5_A4B8_B3DA4C0FFAAA__INCLUDED_)
