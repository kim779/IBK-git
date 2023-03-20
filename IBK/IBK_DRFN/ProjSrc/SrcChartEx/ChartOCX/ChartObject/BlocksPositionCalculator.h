// BlocksPositionCalculator.h: interface for the CBlocksPositionCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKSPOSITIONCALCULATOR_H__C2233779_3A04_4B80_A18A_C9ECCBAF4E45__INCLUDED_)
#define AFX_BLOCKSPOSITIONCALCULATOR_H__C2233779_3A04_4B80_A18A_C9ECCBAF4E45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CBlocksPositionCalculator
{
public:
	// ��ü������ ��� block���� �ּҿ������� ū ���������� ����. (gap�� ���м� ���� ����)
	// ��ü���� >= (block����*block���� + (block���� -1)*(gap + ���м� ����))  
	static bool IsBiggerThanMinMainRegion(const int blockCount, const int allBlocksRegion, const int interBlockGap, const int compartLineThickness);
	// rect2�� �Ǵٸ� rect1�� ���� �Ǵ����� ���
	static bool IsRectInRect(const CRect& rect1, const CRect& rect2);

	// position ����
	static void Add_EqualityPartition( CList< double, double>& positionList, const int blockCount, 
			const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness);
	static void Add_DoubleSize( CList< double, double>& positionList, const int blockCount, 
			const int doubleSizeBlockNum, const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness);

	// block���� ��ġ�� �ٽ� ���Ѵ�. -> positionList : m_lRow, m_lColumn
	static void SetBlocksPosition( CList< double, double>& positionList, const int maxExtent, const CString& positionData);

	// m_lRow, m_lColumn�� ���� string�� ��ȯ
	static CString GetSavedPosition( const CList< double, double>& positionList, const CString& prePosition);
	// �ش� ��ġ(Gap�� �������� ���� ���� block) �������� => ������ǥ
	static void GetBlockRegion(const int absoluteOrgPt, const int interBlockHalfGap, 
		const int compartLineThickness, const CList< double, double>& positionList, const int nIndex, double &x1, double &x2);
	// list�� �Է��� position�� ������ �����´�.
	static int GetPositionLength(const int startPosition, const int endPosition, 
		const int posNum, const int blockCount, const int interBlockHalfGap, const int compartLineThickness);
	// ���м��� �׸������ؼ� �ش� block�� ��ġ�� �����´�.
	static CPoint GetAbsolutePoint( const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, const int xIndex, const int yIndex);


	// ��� block ��ġ�� �յ����� �����Ѵ�.
	static void InitializePositions(const CRect& newAllBlockRegion, 
		const int nDoubleSizeBlockRowNum, const int nDoubleSizeBlockColumnNum, 
		CList<int, int>& rowList, CList<int, int>& columnList, const int interBlockHalfGap, const int compartLineThickness);
	// ��� block ������ġ���� ����� ������ ���� �����Ѵ�.
	static void RecalculatePositions(const CRect& oldAllBlockRegion, const CRect& newAllBlockRegion, CList< double, double>& rowList, CList< double, double>& columnList);

	// point�� ��� ���м��� ���ԵǴ� ���� ã��.
	static bool FindCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& blockRowIndex, int& blockColumnIndex);
	// ���õ� block�� ��ġ�� �����Ѵ�. --> ���콺�� ����.
	static bool MoveBlocksPosition( CList< double, double>& rowList, CList< double, double>& columnList, const CPoint& point, const int rowCompartLineNum, const int columnCompartLineNum);
	// block ��ġ ����
	static bool MoveBlockPosition( CList< double, double>& positionList, const int sourceIndex, const int targetIndex);
	static void RecalculatePositions( CList< double, double>& positionList, const int oldAllRegionLength, const int newAllRegionLength);

private:
	static int m_nMouseClickThickness;  // ���м� ������ üũ�� ����
	static int m_nMinBlockWidth;		// block�� �ּ� ���� ���� ����

	static int GetDoubleSizeBlockCount(const int blockCount, const int doubleSizeBlockNum);
	static bool IsBlockInDoubleSize(const int doubleSizeBlockNum, const int doubleSizeCount, const int curNum);

	static bool GetBlocksPosition( const CString& strPositon, CList< double, double>& positionList);
	static bool SetBlocksPosition( CList< double, double>& targetList, const CList< double, double>& sourceList);

	// Gap�� ������ �ش� ��ġ(block) �������� => ������ǥ
	static double GetAbsolutePosition( const double absoluteOrgPt, const CList< double, double>& positionList, const int nIndex);
	// ���� block�� ���̸� ���Ѵ�.
	static int GetBlockLength(const int start, const int end, const int blockCount, const int gapCount, 
		const int compartLineCount, const int interBlockHalfGap, const int compartLineThickness);

	// ��� block ��ġ�� �յ����� �����Ѵ�.
	static void InitializePositions(CList<int, int>& positionList, const int nDoubleSizeBlockNum, 
		const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness);
	static int GetChangedPosition(const int oldAllRegionLength, const int newAllRegionLength, 
		const int oldPositionValue, const int posNum, const int blockCount);

	// point�� ��� ���м��� ���ԵǴ� ���� ���Ѵ�.
	static bool AppropriateRowCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& rowCompartLineNum);
	static bool AppropriateColumnCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& columnCompartLineNum);
	static bool IsPtInRect(const CRect& rect, const CPoint& point);

	// �ش� block�� position�� ���� �����Ѵ�. --> ���콺�� ���� block ���������� ȣ��
	static bool MoveBlocksPosition( CList< double, double>& positionList, const int compartLineNum, const int modityData);
	static bool IsMoveBlockPosition(const int num, const int allCount);
	static bool GetChangedLengthInBlock( const CList< double, double>& positionList, const int sourceIndex, const int targetIndex, CList< double, double>& lengthInBlock);
	static bool GetChangedLengthInBlock(const int sourceIndex, const int targetIndex, CList< double, double>& lengthInBlock);
	static double GetData( const CList< double, double>& positionList, const int index);

};

#endif // !defined(AFX_BLOCKSPOSITIONCALCULATOR_H__C2233779_3A04_4B80_A18A_C9ECCBAF4E45__INCLUDED_)
