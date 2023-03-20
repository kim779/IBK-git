// Region_AllBlock.h: interface for the CRegion_AllBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGION_ALLBLOCK_H__A8ED42F4_7D4B_4080_BB36_56292E9EC8C5__INCLUDED_)
#define AFX_REGION_ALLBLOCK_H__A8ED42F4_7D4B_4080_BB36_56292E9EC8C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

#include "BlockBaseData.h"

class CCoordinateCalculatorData;
class CRegion_Column;
class CRegion_AllBlock  
{
public:
	CRegion_AllBlock();
	virtual ~CRegion_AllBlock();

	void SetDrawingRegions(IChartManager* pMainBlock);
	void SetAllBlockRegion(IChartManager* pMainBlock);
	void Initialize();

	//Block������ �����ϰ� ��´�.
	void SetBlockRegion(const int nColumnIndex, const int nRowIndex, const CRect& blockregion);
	CRect GetBlockRegion(const int nColumnIndex, const int nRowIndex) const;

	//Block������ �����ؾ��ϴ� Column����Ʈ�� �����.
	void MakeColumnList(const int nColumnCount, const int nRowCount);

	bool InsertBlockIndex(const CBlockIndex &blockIndex);
	void DeleteColumnBlockList();

	//Block�� �� BlockIndex�� �ش��ϴ� Data�� ������ ��´�.
//	void GetCoordinateCalculatorData(CBlockIndex blockIndex, CCoordinateCalculatorData& coordinateData);

private:
	//Block������ �����ϴ� Column����Ʈ.
	CList<CRegion_Column*, CRegion_Column* > m_ColumnBlockRegionList; 

};

#endif // !defined(AFX_REGION_ALLBLOCK_H__A8ED42F4_7D4B_4080_BB36_56292E9EC8C5__INCLUDED_)
