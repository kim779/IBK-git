// BlocksPosition.h: interface for the CBlocksPosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKSPOSITION_H__0843394A_8F51_4A1A_904A_201E970BA6BD__INCLUDED_)
#define AFX_BLOCKSPOSITION_H__0843394A_8F51_4A1A_904A_201E970BA6BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../Include_Chart/BlockBaseData.h"

// (2009/6/25 - Seung-Won, Bae) for Balanced Row Arrange without scale.
class CMainBlockImp;
class CBlocksPositionBase
{
protected:
	CBlocksPositionBase();
	~CBlocksPositionBase();

protected:
	CMainBlockImp *			m_pMainBlock;
	CRect					m_allBlockRegion;		// ��� block ��ü ����
	CList< double, double>	m_lColumn;
public:
	void			SetMainBlock( CMainBlockImp *p_pMainBlock)
					{	m_pMainBlock = p_pMainBlock;	ASSERT( m_pMainBlock);	}

private:
	CBlockIndex				m_biMaximizedBlock;
	CList< double, double>	m_dlRowRate;		// the rate without scale.
	CList< double, double>	m_dlRowReal;		// the real layout with scale.
												// Do not access directly.

protected:
	CList< double, double>	&		GetRowRate( void);
	const CList< double, double> &	GetConstRowRate( void) const	{	return m_dlRowRate;	}
	int								GetRowCount( void) const;

protected:
	void							UpdateRowRealFromRowRate( void);
	CList< double, double>	&		GetRowReal( void);
	const CList< double, double> &	GetConstRowReal( void) const;
	bool							MoveBlocksPosition( CList< double, double>& columnList, const CPoint& point, const int rowCompartLineNum, const int columnCompartLineNum);

public:	// block position �ʱ�ȭ
	void		BlockPositionReset( void);
	void		SetMaximizedBlock( const CBlockIndex& maxBlockIndex);
	CBlockIndex	GetMaximizedBlock( void) const;
	void		ClearRowReal( void);
};

class CHorizontalScale;
class CBlocksPosition : public CBlocksPositionBase
{
public:
	CBlocksPosition();
	~CBlocksPosition();

	// block ��ġ�� Setting.
	bool Set(const CRect& allBlockRegion, const CBlockIndex& blockCount, 
			const CBlockIndex& doubleBlockSizeIndex, const int interBlockHalfGap, const int compartLineThickness);
	bool SetBlockCount(const CBlockIndex& blockCount, const CBlockIndex& doubleBlockSizeIndex);
	bool SetRowBlockCount(const int rowCount, const int doubleSizeRowNum);
	bool SetColumnBlockCount(const int columnCount, const int doubleSizeColumnNum);

	void InitializeNormalSizeBlockNumber();
	bool RecalculateBlockRegions(const CRect& allBlockRegion);
	void SetBlocksPosition(const CString& blocksPosition);
	void SetInterBlockHalfGap(const int interBlockHalfGap);
	void SetCompartLineThickness(const int compartLineThickness);
	void SetDoubleSizeBlockNumber(const CBlockIndex& doubleSizeBlockIndex);

	// block ��ġ�� ���õ� ���� ��������.
	CString GetBlocksPosition() const;
	CRect GetAllBlockRegion() const;
	// blocksColumn�� ��ü ������ �����´�.
	CRect GetBlocksColumnRegion(const int columnIndex) const;
	// �ش� ��ġ(Gap�� �������� ���� ���� block ����) ��������
	CRect GetBlockRegion(const CBlockIndex& blockIndex) const;

	int GetBlockCount() const;
	int GetRowBlockCount() const;
	int GetColumnBlockCount() const;
	bool GetRowAndColumn(const CPoint& point, CBlockIndex& blockIndex) const;
	bool GetRowAndColumn(const CRect& region, CBlockIndex& blockIndex) const;
	int GetInterBlockHalfGap() const;

	// block ���м��� �׸���.
	void DrawBlockCompartLine(CDC* pDC, const COLORREF& compartLineColor);

	// point�� ��� ���м��� �ش��ϴ��� üũ&����--> ���콺�� ����.
	CBlockIndex FindCompartLine(const CPoint& point) const;
	bool MoveBlocksPosition(const CPoint& point, const CBlockIndex& compartLineIndex);
	// block ��ġ ����
	bool MoveBlockPosition(const int sourceIndex, const int targetIndex, const CBlockBaseData::BLOCKTYPE type);

private:
	int m_nInterBlockHalfGap;	// block���� Gap�� ����(���м� ũ��� ������)
	int m_nCompartLineThickness; // ���м��� ����

	bool IsBiggerThanMinMainRegion(const CRect& allBlockRegion, const CBlockIndex& blockCount, const int interBlockHalfGap) const;
	bool IsBiggerThanMinMainRegion(const int blockCount, const int allBlocksRegion, const int interBlockHalfGap) const;

	// Row, Column ����.
	void Add(CList<  double, double>& positionList, const int blockCount, const int doubleSizeBlockNum, const int length);
	bool IsEqualityPartition(const int blockCount, const int doubleSizeBlockNum) const;

	// ���м��� �׸���.
	void DrawRowCompartLine(CDC* pDC, const COLORREF& compartLineColor);
	void DrawColumnCompartLine(CDC* pDC, const COLORREF& compartLineColor);

// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
public:
	void	SetHorzScalePosition( CHorizontalScale &p_horizontalScale, const CScaleBaseData::HORZSCALEPOSITION ePosition);
};


#endif // !defined(AFX_BLOCKSPOSITION_H__0843394A_8F51_4A1A_904A_201E970BA6BD__INCLUDED_)
