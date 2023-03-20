// Region_Column.h: interface for the CRegion_Column class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGION_COLUMN_H__BEB39CD9_3891_4A74_A293_F173258880AA__INCLUDED_)
#define AFX_REGION_COLUMN_H__BEB39CD9_3891_4A74_A293_F173258880AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CRegion_Row;
class CRegion_Column  
{
public:
	CRegion_Column(const int nColumnIndex, const int nRowCount);
	virtual ~CRegion_Column();

	//Column��ȣ�� ��´�.
	int GetColumnIndex();

	//Block������ �����ϰ� ��´�.
	void SetBlockRegion(const int nRowIndex, const CRect& blockregion);
	CRect GetBlockRegion(const int nRowIndex) const;

	bool InsertRow(const int nRow);

private:
	//Column��ȣ
	int m_nColumn;
	//Row�� �����ϴ� ����Ʈ.
	CList<CRegion_Row*, CRegion_Row* > m_RowRegionList; 

	//Row�� �����ϴ� ����Ʈ�� �����.
	void MakeRowList(const int nRowCount);
	void SetModifiedRowIndexs();

};

#endif // !defined(AFX_REGION_COLUMN_H__BEB39CD9_3891_4A74_A293_F173258880AA__INCLUDED_)
