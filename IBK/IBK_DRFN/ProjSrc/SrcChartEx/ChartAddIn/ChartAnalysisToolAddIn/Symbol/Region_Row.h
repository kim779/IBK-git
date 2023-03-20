// Region_Row.h: interface for the CRegion_Row class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGION_ROW_H__86F73909_480C_4F14_81D1_700F4539288F__INCLUDED_)
#define AFX_REGION_ROW_H__86F73909_480C_4F14_81D1_700F4539288F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegion_Row  
{
public:
	void SetRowIndex(const int nRow);
	CRegion_Row(const int nRowIndex);
	virtual ~CRegion_Row();

	//Block������ �����ϰ� ��´�.
	void SetBlockRegion(const CRect& blockregion);
	CRect GetBlockRegion() const;
	//Row��ȣ�� ��´�.
	int GetRowIndex();

private:
	//���� ����.
	CRect m_BlockRegion;
	//Row��ȣ.
	int m_nRow;

};

#endif // !defined(AFX_REGION_ROW_H__86F73909_480C_4F14_81D1_700F4539288F__INCLUDED_)
