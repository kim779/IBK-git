// IWChartManager050116.cpp: implementation of the CIWChartManager050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartManager050116.h"

#include "MainBlock.h"				// for CMainBlock

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWChartManager050116::CIWChartManager050116( CMainBlock *p_pMainBlock)
{
	// (2004.10.11, ��¿�) MainBlock Object�� Pointer�� �����Ѵ�.
	m_pMainBlock = p_pMainBlock;
	if( !m_pMainBlock)
	{
		AfxMessageBox( "CIWChartManager050116 Null Wrapping Error!");
		return;
	}
}

CIWChartManager050116::~CIWChartManager050116()
{

}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Block�� ��� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

int CIWChartManager050116::GetRowCount( void)
{
	if( m_pMainBlock) return m_pMainBlock->GetRowCount();
	return 0;
}
int CIWChartManager050116::GetColumnCount( void)
{
	if( m_pMainBlock) return m_pMainBlock->GetColumnCount();
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Block�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// Block Region
CRect CIWChartManager050116::GetBlockRegion( int p_nRowIndex, int p_nColumnIndex)
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->GetBlockRegion( blockIndex);
}

// Graph Region
// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
CRect CIWChartManager050116::GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex)	// Block Region
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->GetNoFrameGraphRegion( blockIndex);
}
CRect CIWChartManager050116::GetNoFrameGraphRegion( CPoint p_pPoint)
{
	if( m_pMainBlock) return m_pMainBlock->GetNoFrameGraphRegion( p_pPoint);
	return CRect( 0, 0, 0, 0);
}
// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
CRect CIWChartManager050116::GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex)	// Graph Region
{
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	if( m_pMainBlock) return m_pMainBlock->GetDrawingGraphRegion( blockIndex);
	return CRect( 0, 0, 0, 0);
}
CRect CIWChartManager050116::GetDrawingGraphRegion( CPoint p_pPoint)
{
	if( m_pMainBlock) return m_pMainBlock->GetDrawingGraphRegion( p_pPoint);
	return CRect( 0, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] Block�� Graph Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/13] Block���� �ش� ��ǥ�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager050116::CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName)
{
	if( !m_pMainBlock) return FALSE;
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->IsExistenceGraphInBlock( p_szGraphName, blockIndex);
}

// [04/10/26] �����Ǵ� Graph�� Data Packet Name�� ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager050116::GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames)
{
	if( !m_pMainBlock) return FALSE;
	p_strPacketNames = m_pMainBlock->GetGraphPacketName( p_szGraphName);
	return !p_strPacketNames.IsEmpty();
}

// [04/10/27] Ư�� Graph�� ��ϵ� ù Block�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager050116::GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC)
{
	// 0. ���� MainBlock DLL�� Ȯ���Ѵ�.
	if( !m_pMainBlock) return FALSE;
	// 1. Block Count�� Ȯ���Ѵ�.
	int nBlockRow		= m_pMainBlock->GetRowCount();
	int nBlockColumn	= m_pMainBlock->GetColumnCount();
	// 2. �� Block���� �ش� Graph�� �ִ��� Ȯ���Ѵ�.
	for( p_nR = 0; p_nR < nBlockRow; p_nR++)
	{
		for( p_nC = 0; p_nC < nBlockColumn; p_nC++)
			if( CheckGraphInBlock( p_nR, p_nC, p_szGraphName)) break;
		if( p_nC < nBlockColumn) break;
	}
	// 3. �ش� Graph�� ã�� ��츦 ó���Ѵ�.
	if( p_nR < nBlockRow) return TRUE;
	// 4. �ش� Graph�� ã�� ���� ��츦 ó���Ѵ�.
	p_nR = p_nC = 0;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] View�� ������ ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/13] �ش� ��ǥ�� �ִ� View�� Min/Max���� �����ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager050116::GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax)
{
	// 0. ���� Return ���� �ʱ�ȭ�Ѵ�.
	p_dMin = p_dMax = ERRORVALUE;
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->GetDisplayDataMinMax( p_szGraphName, p_dMin, p_dMax);
}

// [04/10/27] ������ Block�� Y Scale Conversion Type�� ���Ѵ�.
BOOL CIWChartManager050116::GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse)
{
	// 0. ���� MainBlock DLL�� Ȯ���Ѵ�.
	if( !m_pMainBlock) return FALSE;

	// 1. Conversion Type�� ���Ѵ�.
	CBlockIndex blockIndex( p_nR, p_nC);

	bool bReverse = 0, bLog = 0;
	BOOL bResult = m_pMainBlock->GetVertScaleType( blockIndex, bLog, bReverse);
	p_bReverse	= bReverse;
	p_bLog		= bLog;

	return bResult;
}
