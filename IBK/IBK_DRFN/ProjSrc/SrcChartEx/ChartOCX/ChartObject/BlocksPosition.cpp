// BlocksPosition.cpp: implementation of the CBlocksPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlocksPosition.h"

#include <assert.h>
#include "../Include_Chart/Conversion.h"
#include "BlocksPositionCalculator.h"
#include "MainBlockImp.h"						// for CMainBlockImp

// ViewDraw.dll
#include "../Include_Chart/Dll_Load/GraphFrameDrawer.h" // block ���м��� �׸���.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CBlocksPositionBase
//////////////////////////////////////////////////////////////////////

CBlocksPositionBase::CBlocksPositionBase() : m_allBlockRegion(0, 0, 0, 0)
{
}

CBlocksPositionBase::~CBlocksPositionBase()
{
	BlockPositionReset();
}

CList< double, double> &CBlocksPositionBase::GetRowRate( void)
{
	ClearRowReal();
	return m_dlRowRate;
}

int CBlocksPositionBase::GetRowCount( void) const
{
	return m_dlRowRate.GetCount();
}

void CBlocksPositionBase::UpdateRowRealFromRowRate( void)
{
	// 0. Check 0 or 1 Block.
	if( m_dlRowRate.GetCount() < 1) return;
	m_dlRowReal.AddTail( &m_dlRowRate);

	// 1. Get the Horizontal Scale Count.
	CList< int, int> lnSumOfHScaleHeight;
	m_pMainBlock->GetSumOfHorzScaleHeightForEachBlock( lnSumOfHScaleHeight);

	// (2009/9/5 - Seung-Won, Bae) for Invalid Values. (make dummy data. or delete left data.)
	if( lnSumOfHScaleHeight.GetCount() + 1 < m_dlRowReal.GetCount())
	{
		int nMoreInfoCount = m_dlRowReal.GetCount() - ( lnSumOfHScaleHeight.GetCount() + 1);
		for( int i = 0; i < nMoreInfoCount; i++) m_dlRowReal.RemoveTail();
	}
	else if( m_dlRowReal.GetCount() < lnSumOfHScaleHeight.GetCount() + 1)
	{
		int nLessInfoCount = ( lnSumOfHScaleHeight.GetCount() + 1) - m_dlRowReal.GetCount();
		double lLastData = m_dlRowReal.GetTail();
		for( int i = 0; i < nLessInfoCount; i++) m_dlRowReal.AddTail( lLastData);
	}

	int nRegionHeight = m_allBlockRegion.Height();
	// (2009/9/6 - Seung-Won, Bae) for Block Maximizing.
	if( 0 <= m_biMaximizedBlock.GetRow())
	{
		int i = 0;
		POSITION psnRowPos = m_dlRowReal.GetHeadPosition();
		while( psnRowPos)
		{
			if( i <=  m_biMaximizedBlock.GetRow())	m_dlRowReal.SetAt( psnRowPos, 0);
			else									m_dlRowReal.SetAt( psnRowPos, nRegionHeight);
			m_dlRowReal.GetNext( psnRowPos);
			i++;
		}
		return;
	}

	int nFullHScaleHeight = 0;
	POSITION psnScaleHeight = lnSumOfHScaleHeight.GetHeadPosition();
	while( psnScaleHeight) nFullHScaleHeight += lnSumOfHScaleHeight.GetNext( psnScaleHeight);

	// 2. Get the new height of the all Block Region.
	nRegionHeight -= nFullHScaleHeight;

	// 3. ReArrange the Block Position without H Scale.
	CBlocksPositionCalculator::RecalculatePositions( m_dlRowReal, ( int)m_dlRowReal.GetTail(), nRegionHeight);

	// 4. Add H Scale to Block Region.
	double dRowPosition = 0;
	int nScaleHeightSum = 0;
	POSITION psnRow = m_dlRowReal.GetHeadPosition();
	m_dlRowReal.GetNext( psnRow);	// to skip the first base position 0.
	psnScaleHeight = lnSumOfHScaleHeight.GetHeadPosition();
	while( psnRow && psnScaleHeight)
	{
		dRowPosition = m_dlRowReal.GetAt( psnRow);
		nScaleHeightSum += lnSumOfHScaleHeight.GetAt( psnScaleHeight);
		m_dlRowReal.SetAt( psnRow, dRowPosition + nScaleHeightSum);
		m_dlRowReal.GetNext( psnRow);
		lnSumOfHScaleHeight.GetNext( psnScaleHeight);
	}
}

CList< double, double>	&CBlocksPositionBase::GetRowReal( void)
{
	if( m_dlRowReal.IsEmpty()) UpdateRowRealFromRowRate();
	return m_dlRowReal;
}

const CList< double, double> &CBlocksPositionBase::GetConstRowReal( void) const
{
	CBlocksPositionBase *pThis = ( CBlocksPositionBase *)this;
	return pThis->GetRowReal();
}

bool CBlocksPositionBase::MoveBlocksPosition( CList< double, double>& columnList, const CPoint& point, const int rowCompartLineNum, const int columnCompartLineNum)
{
	CList< double, double>	&lRowReal = GetRowReal();
	bool bResult = CBlocksPositionCalculator::MoveBlocksPosition( lRowReal, columnList, point, rowCompartLineNum, columnCompartLineNum);
	
	// UpdateRowRateFromRowReal()

	// 1. Get the Rate with Scale.
	m_dlRowRate.RemoveAll();
	m_dlRowRate.AddTail( &m_dlRowReal);

	// 2. Check the 0 or 1 Block.
	if( m_dlRowRate.GetCount() < 3) return bResult;
	
	// 3. Get the Horizontal Scale Count.
	CList< int, int> lnSumOfHScaleHeight;
	m_pMainBlock->GetSumOfHorzScaleHeightForEachBlock( lnSumOfHScaleHeight);
	ASSERT( lnSumOfHScaleHeight.GetCount() + 1 == m_dlRowRate.GetCount());

	// 4. Substract H Scale from Block Region.
	double dRowPosition = 0;
	int nScaleHeightSum = 0;
	POSITION psnRow = m_dlRowRate.GetHeadPosition();
	m_dlRowRate.GetNext( psnRow);	// to skip the first base position 0.
	POSITION psnScaleHeight = lnSumOfHScaleHeight.GetHeadPosition();
	while( psnRow && psnScaleHeight)
	{
		dRowPosition = m_dlRowRate.GetAt( psnRow);
		nScaleHeightSum -= lnSumOfHScaleHeight.GetAt( psnScaleHeight);
		m_dlRowRate.SetAt( psnRow, dRowPosition + nScaleHeightSum);
		m_dlRowRate.GetNext( psnRow);
		lnSumOfHScaleHeight.GetNext( psnScaleHeight);
	}

	return bResult;
}

void CBlocksPositionBase::ClearRowReal( void)
{
	m_dlRowReal.RemoveAll();
}

void CBlocksPositionBase::BlockPositionReset( void)
{
	m_dlRowRate.RemoveAll();
	m_dlRowReal.RemoveAll();
	m_lColumn.RemoveAll();
}

// �� �ִ�ȭ �߰� - ojtaso (20080519)
void CBlocksPositionBase::SetMaximizedBlock( const CBlockIndex& maxBlockIndex)
{
	ClearRowReal();
	m_biMaximizedBlock = maxBlockIndex;
}

// �� �ִ�ȭ �߰� - ojtaso (20080519)
CBlockIndex	CBlocksPositionBase::GetMaximizedBlock( void) const
{
	return m_biMaximizedBlock;
}

//////////////////////////////////////////////////////////////////////
// class CBlocksPosition
//////////////////////////////////////////////////////////////////////

CBlocksPosition::CBlocksPosition():
	m_nInterBlockHalfGap(0),
	m_nCompartLineThickness(1)
{
}

CBlocksPosition::~CBlocksPosition()
{
	BlockPositionReset();
}

// public =================================================================================
// block�� ��ġ ������ list�� Setting.

// ��� ���� setting.
bool CBlocksPosition::Set(const CRect& allBlockRegion, const CBlockIndex& blockCount, 
			const CBlockIndex& doubleBlockSizeIndex, const int interBlockHalfGap, const int compartLineThickness)
{
	// block���� �ּ� ���� �˻�
	if(!IsBiggerThanMinMainRegion(allBlockRegion, blockCount, interBlockHalfGap))
		return false;

	m_allBlockRegion = allBlockRegion;
	m_nInterBlockHalfGap = interBlockHalfGap;
	m_nCompartLineThickness = compartLineThickness;
	
	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, blockCount.GetRow(), doubleBlockSizeIndex.GetRow(), allBlockRegion.bottom - allBlockRegion.top);
	Add(m_lColumn, blockCount.GetColumn(), doubleBlockSizeIndex.GetColumn(), allBlockRegion.right - allBlockRegion.left);
	return true;
}

bool CBlocksPosition::SetBlockCount(const CBlockIndex& blockCount, const CBlockIndex& doubleBlockSizeIndex)
{
	// block���� �ּ� ���� �˻�
	if(!IsBiggerThanMinMainRegion(m_allBlockRegion, blockCount, m_nInterBlockHalfGap))
		return false;

	if(GetRowBlockCount() == blockCount.GetRow() && GetColumnBlockCount() == blockCount.GetColumn())
		return false;

	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, blockCount.GetRow(), doubleBlockSizeIndex.GetRow(), m_allBlockRegion.bottom - m_allBlockRegion.top);
	Add(m_lColumn, blockCount.GetColumn(), doubleBlockSizeIndex.GetColumn(), m_allBlockRegion.right - m_allBlockRegion.left);
	return true;
}

// ���� �� ���� setting. --> block ��ġ�� ��� ����� �ٽ� ���Ѵ�.
bool CBlocksPosition::SetRowBlockCount(const int rowCount, const int doubleSizeRowNum)
{	
	// block���� �ּ� ���� �˻�
	if(!IsBiggerThanMinMainRegion(m_allBlockRegion, CBlockIndex(rowCount, (m_lColumn.GetCount() -1)), m_nInterBlockHalfGap))
		return false;

	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, rowCount, doubleSizeRowNum, m_allBlockRegion.bottom - m_allBlockRegion.top);
	return true;
}

// ���� �� ���� setting. --> block ��ġ�� ��� ����� �ٽ� ���Ѵ�.
bool CBlocksPosition::SetColumnBlockCount(const int columnCount, const int doubleSizeColumnNum)
{
	// block���� �ּ� ���� �˻�
	if(!IsBiggerThanMinMainRegion(m_allBlockRegion, CBlockIndex( GetRowCount() -1, columnCount), m_nInterBlockHalfGap))
		return false;

	Add(m_lColumn, columnCount, doubleSizeColumnNum, m_allBlockRegion.right - m_allBlockRegion.left);
	return true;
}

// block size�� 2���� ��� ���� setting
void CBlocksPosition::SetDoubleSizeBlockNumber(const CBlockIndex& doubleSizeBlockIndex)
{
	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, GetRowBlockCount(), doubleSizeBlockIndex.GetRow(), m_allBlockRegion.bottom - m_allBlockRegion.top);
	Add(m_lColumn, GetColumnBlockCount(), doubleSizeBlockIndex.GetColumn(), m_allBlockRegion.right - m_allBlockRegion.left);
}

// ---------------------------------------------------------------------------------------
// ���� block�� ��ġ�� ����.
// double block �� ����
/*
bool CBlocksPosition::InitializeBlockRegions(const CRect& allBlockRegion, const CBlockIndex& doubleSizeBlockIndex)
{
	// block�� ������ġ�� �ٲܸ��� ���������� üũ�Ͽ� �ּҿ������� Ŭ��츸 ��ġ �ٲ۴�.
	if(!IsBiggerThanMinMainRegion(allBlockRegion, CBlockIndex((m_lRow.GetCount() -1), (m_lColumn.GetCount() -1)), m_nInterBlockHalfGap))
		return false;

	CBlocksPositionCalculator::InitializePositions(allBlockRegion, 
		doubleSizeBlockIndex.GetRow(), doubleSizeBlockIndex.GetColumn(), m_lRow, m_lColumn, 
		m_nInterBlockHalfGap, m_nCompartLineThickness);
	m_allBlockRegion = allBlockRegion;
	return true;
}
*/

// ���� ��ġ�� ����.
bool CBlocksPosition::RecalculateBlockRegions(const CRect& allBlockRegion)
{
	if(m_allBlockRegion == allBlockRegion)
		return false;

	// block�� ������ġ�� �ٲܸ��� ���������� üũ�Ͽ� �ּҿ������� Ŭ��츸 ��ġ �ٲ۴�.
	if(!IsBiggerThanMinMainRegion(allBlockRegion, CBlockIndex( GetRowCount() - 1, (m_lColumn.GetCount() -1)), m_nInterBlockHalfGap))
		return false;

	// (2009/6/26 - Seung-Won, Bae) Do not Recalc the Row Rate. (it is same rate.)
	//		Just clear row real.
	CBlocksPositionCalculator::RecalculatePositions( m_lColumn, ( m_allBlockRegion.right - m_allBlockRegion.left), ( allBlockRegion.right - allBlockRegion.left));

	m_allBlockRegion = allBlockRegion;
	ClearRowReal();
	return true;
}

void CBlocksPosition::InitializeNormalSizeBlockNumber()
{
	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, GetRowBlockCount(), -1, m_allBlockRegion.bottom - m_allBlockRegion.top);
	Add(m_lColumn, GetColumnBlockCount(), -1, m_allBlockRegion.right - m_allBlockRegion.left);
}

// ���� ���ڰ����� ��� block�� ��ġ�� setting.
void CBlocksPosition::SetBlocksPosition(const CString& blocksPosition)
{
	// block ��ġ ����.
	CList< double, double> &lRowRate = GetRowRate();
	CBlocksPositionCalculator::SetBlocksPosition(lRowRate, m_allBlockRegion.Height(), 
		CDataConversion::GetMidToTrimDataIndex(blocksPosition, "row;", "\r\n"));
	CBlocksPositionCalculator::SetBlocksPosition(m_lColumn, m_allBlockRegion.Width(), 
		CDataConversion::GetMidToTrimDataIndex(blocksPosition, "column;", "\r\n"));
}

// block���� gap / 2 setting. --> list ���� �ʿ����.
void CBlocksPosition::SetInterBlockHalfGap(const int interBlockHalfGap)
{
	m_nInterBlockHalfGap = interBlockHalfGap;
}

// ���м��� ���� setting. --> list ���� �ʿ����.
void CBlocksPosition::SetCompartLineThickness(const int compartLineThickness)
{
	if(compartLineThickness >= 0)
		m_nCompartLineThickness = compartLineThickness;
}

// ----------------------------------------------------------------------------------------
// ��� block�� ��ġ(m_lRow, m_lColumn)�� string�� ��������.
// --> Gap�� ������ ����
CString CBlocksPosition::GetBlocksPosition() const
{
	const CList< double, double> &lRowRate = GetConstRowRate();
	CString rowsPosition = CBlocksPositionCalculator::GetSavedPosition(lRowRate, "row;");
	CString columnsPosition = CBlocksPositionCalculator::GetSavedPosition(m_lColumn, "column;");
	return (rowsPosition + columnsPosition);
}

// ��� block�� ������ ���Ѵ�.
CRect CBlocksPosition::GetAllBlockRegion() const
{
	return m_allBlockRegion;
}

// blocksColumn�� ��ü ������ �����´�.
CRect CBlocksPosition::GetBlocksColumnRegion(const int columnIndex) const
{
	assert(columnIndex >= 0 && columnIndex < GetColumnBlockCount());

	CRect firstBlockRegion = GetBlockRegion(CBlockIndex(0, columnIndex));
	return CRect(firstBlockRegion.left, m_allBlockRegion.top, firstBlockRegion.right, m_allBlockRegion.bottom);
}

// row, column��°�� block������ ���Ѵ�. (Gap�� ���Ծȵ� ����) => ������ǥ
CRect CBlocksPosition::GetBlockRegion(const CBlockIndex& blockIndex) const
{
	assert(blockIndex.GetColumn() >= 0 && blockIndex.GetColumn() < GetColumnBlockCount());
	assert(blockIndex.GetRow() >= 0 && blockIndex.GetRow() < GetRowBlockCount());

	double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	CBlocksPositionCalculator::GetBlockRegion(m_allBlockRegion.left, m_nInterBlockHalfGap, m_nCompartLineThickness, m_lColumn, blockIndex.GetColumn(), x1, x2);
	const CList< double, double> &lRowReal = GetConstRowReal();
	CBlocksPositionCalculator::GetBlockRegion(m_allBlockRegion.top, m_nInterBlockHalfGap, m_nCompartLineThickness, lRowReal, blockIndex.GetRow(), y1, y2);
	return CRect( ( int)x1, ( int)y1, ( int)x2, ( int)y2);
}

// block �� �� ����
int CBlocksPosition::GetBlockCount() const
{
	return (GetRowBlockCount() * GetColumnBlockCount());
}

// ���� �� ����
int CBlocksPosition::GetRowBlockCount() const
{
	if( GetRowCount() <= 0) return 0;
	return GetRowCount() - 1;
}

// ���� �� ����
int CBlocksPosition::GetColumnBlockCount() const
{
	if(m_lColumn.GetCount() <= 0)
		return 0;

	return (m_lColumn.GetCount() -1);
}

bool CBlocksPosition::GetRowAndColumn(const CPoint& point, CBlockIndex& blockIndex) const
{
	for(int columnIndex = 0; columnIndex < GetColumnBlockCount(); columnIndex++){
		for(int rowIndex = 0; rowIndex < GetRowBlockCount(); rowIndex++){
			if(GetBlockRegion(CBlockIndex(rowIndex, columnIndex)).PtInRect(point)){
				blockIndex.SetIndex(rowIndex, columnIndex);
				return true;
			}
		}
	}
	return false;
}

bool CBlocksPosition::GetRowAndColumn(const CRect& region, CBlockIndex& blockIndex) const
{
	for(int columnIndex = 0; columnIndex < GetColumnBlockCount(); columnIndex++){
		for(int rowIndex = 0; rowIndex < GetRowBlockCount(); rowIndex++){
			if(CBlocksPositionCalculator::IsRectInRect(GetBlockRegion(CBlockIndex(rowIndex, columnIndex)), region)){
				blockIndex.SetIndex(rowIndex, columnIndex);
				return true;
			}
		}		
	}
	return false;
}

// block���� Gap / 2
int CBlocksPosition::GetInterBlockHalfGap() const
{
	return m_nInterBlockHalfGap;
}

// ----------------------------------------------------------------------------------------
// ���м��� �׸���.
void CBlocksPosition::DrawBlockCompartLine(CDC* pDC, const COLORREF& compartLineColor)
{
	DrawRowCompartLine(pDC, compartLineColor);
	DrawColumnCompartLine(pDC, compartLineColor);
}

// ----------------------------------------------------------------------------------------
// point�� row, column�� �ش�Ǵ� ������ ������ ���� ã�´�.
// --> �ش� ���м��� ���° ��� ���� �ִ��� ã��.
CBlockIndex CBlocksPosition::FindCompartLine(const CPoint& point) const
{
	int rowIndex = -1, columnIndex = -1;
	const CList< double, double> &lRowReal = GetConstRowReal();
	if(!CBlocksPositionCalculator::FindCompartLine(CPoint(m_allBlockRegion.left, m_allBlockRegion.top), 
		lRowReal, m_lColumn, point, rowIndex, columnIndex))
		return CBlockIndex(-1, -1);

	return CBlockIndex(rowIndex, columnIndex);
}

// ���м��� ���õ� ��� point ��ġ�� position ����
bool CBlocksPosition::MoveBlocksPosition(const CPoint& point, const CBlockIndex& compartLineIndex)
{
	return CBlocksPositionBase::MoveBlocksPosition( m_lColumn, point, compartLineIndex.GetRow(), compartLineIndex.GetColumn());
}

// ----------------------------------------------------------------------------------------
// block ��ġ ����
bool CBlocksPosition::MoveBlockPosition(const int sourceIndex, const int targetIndex, const CBlockBaseData::BLOCKTYPE type)
{
	if(type == CBlockBaseData::ROW)
	{
		CList< double, double> &lRowRate = GetRowRate();
		return CBlocksPositionCalculator::MoveBlockPosition( lRowRate, sourceIndex, targetIndex);
	}
	else if(type == CBlockBaseData::COLUMN)
		return CBlocksPositionCalculator::MoveBlockPosition(m_lColumn, sourceIndex, targetIndex);

	return false;
}


// private ================================================================================
// block�� ������ġ�� �ٲܸ��� ���������� üũ
// --> block ��ü �ּҿ������� Ŭ��츸 ��� data ���� ����
bool CBlocksPosition::IsBiggerThanMinMainRegion(const CRect& allBlockRegion, const CBlockIndex& blockCount, const int interBlockHalfGap) const
{
	if(!IsBiggerThanMinMainRegion(blockCount.GetRow(), (allBlockRegion.bottom - allBlockRegion.top), interBlockHalfGap))
		return false;

	return IsBiggerThanMinMainRegion(blockCount.GetColumn(), (allBlockRegion.right - allBlockRegion.left), interBlockHalfGap);
}

bool CBlocksPosition::IsBiggerThanMinMainRegion(const int blockCount, const int allBlocksRegion, const int interBlockHalfGap) const
{
	return CBlocksPositionCalculator::IsBiggerThanMinMainRegion(blockCount, allBlocksRegion, interBlockHalfGap*2, m_nCompartLineThickness);
}

// ----------------------------------------------------------------------------------------
// block�� ������ CList�� �����Ѵ�.
// list�� �߰��Ѵ�.
void CBlocksPosition::Add(CList< double, double>& positionList, const int blockCount, const int doubleSizeBlockNum, const int allRegionLength)
{
	positionList.RemoveAll();
	if(blockCount <= 0)
		return;

	// �յ� ����
	if(IsEqualityPartition(blockCount, doubleSizeBlockNum)){
		CBlocksPositionCalculator::Add_EqualityPartition(positionList, blockCount, 
			allRegionLength, m_nInterBlockHalfGap, m_nCompartLineThickness);
	}
	// 2�� ����
	else{
		CBlocksPositionCalculator::Add_DoubleSize(positionList, blockCount, 
			doubleSizeBlockNum, allRegionLength, m_nInterBlockHalfGap, m_nCompartLineThickness);
	}
}

bool CBlocksPosition::IsEqualityPartition(const int blockCount, const int doubleSizeBlockNum) const
{
	return (blockCount == 1 || doubleSizeBlockNum < 0 || doubleSizeBlockNum >= blockCount);
}

// ----------------------------------------------------------------------------------------
void CBlocksPosition::DrawRowCompartLine(CDC* pDC, const COLORREF& compartLineColor)
{
	// �ӵ��� ������ ��� end point ���ϴ� �κ� �����ϱ�. end.y == start.y
	CPoint absoluteOrgPt(m_allBlockRegion.left, m_allBlockRegion.top);
	CGraphFrameDrawer graphFrameDrawer;
	const CList< double, double> &lRowReal = GetConstRowReal();
	for(int row = 1; row < GetRowCount() - 1; row++)
	{
		CPoint start = CBlocksPositionCalculator::GetAbsolutePoint(absoluteOrgPt, lRowReal, m_lColumn, 0, row);
		CPoint end = CBlocksPositionCalculator::GetAbsolutePoint(absoluteOrgPt, lRowReal, m_lColumn, (m_lColumn.GetCount()-1), row);
		graphFrameDrawer.DrawBlockCompartLine(pDC, start, end, compartLineColor);
	}
}

void CBlocksPosition::DrawColumnCompartLine(CDC* pDC, const COLORREF& compartLineColor)
{
	// �ӵ��� ������ ��� end point ���ϴ� �κ� �����ϱ�. end.x == start.x
	CPoint absoluteOrgPt(m_allBlockRegion.left, m_allBlockRegion.top);
	CGraphFrameDrawer graphFrameDrawer;
	const CList< double, double> &lRowReal = GetConstRowReal();
	for(int column = 1; column < m_lColumn.GetCount() -1; column++)
	{
		CPoint start = CBlocksPositionCalculator::GetAbsolutePoint( absoluteOrgPt, lRowReal, m_lColumn, column, 0);
		CPoint end = CBlocksPositionCalculator::GetAbsolutePoint( absoluteOrgPt, lRowReal, m_lColumn, column, GetRowCount() - 1);
		graphFrameDrawer.DrawBlockCompartLine(pDC, start, end, compartLineColor);
	}
}

// ----------------------------------------------------------------------------------------

// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
void CBlocksPosition::SetHorzScalePosition( CHorizontalScale &p_horizontalScale, const CScaleBaseData::HORZSCALEPOSITION ePosition)
{
	ClearRowReal();
	p_horizontalScale.SetHorzScalePosition( ePosition);
}
