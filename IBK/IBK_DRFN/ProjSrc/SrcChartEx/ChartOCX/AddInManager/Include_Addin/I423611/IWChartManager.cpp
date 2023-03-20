// IWChartManager.cpp: implementation of the CIWChartManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartManager.h"

#include "../../../Include_Chart/Dll_Load/XScaleManager.h"
#include "IWBlock.h"										// for CIWBlock
#include "IWString.h"										// for CIWString

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWChartManager)

//////////////////////////////////////////////////////////////////////
// [04/10/11] Block�� ��� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

int CIWChartManager::GetRowCount( void)
{
	if( m_pMainBlock) return m_pMainBlock->GetRowCount();
	return 0;
}
int CIWChartManager::GetColumnCount( void)
{
	if( m_pMainBlock) return m_pMainBlock->GetColumnCount();
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Block�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// Block Region
CRect CIWChartManager::GetBlockRegion( int p_nRowIndex, int p_nColumnIndex)
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->GetBlockRegion( blockIndex);
}

// Graph Region
// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
CRect CIWChartManager::GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex)	// Block Region
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->GetNoFrameGraphRegion( blockIndex);
}
CRect CIWChartManager::GetNoFrameGraphRegion( CPoint p_pPoint)
{
	if( m_pMainBlock) return m_pMainBlock->GetNoFrameGraphRegion( p_pPoint);
	return CRect( 0, 0, 0, 0);
}
// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
CRect CIWChartManager::GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex)	// Graph Region
{
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	if( m_pMainBlock) return m_pMainBlock->GetDrawingGraphRegion( blockIndex);
	return CRect( 0, 0, 0, 0);
}
CRect CIWChartManager::GetDrawingGraphRegion( CPoint p_pPoint)
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	return m_pMainBlock->GetDrawingGraphRegion( m_pMainBlock->FindBlockIndex( p_pPoint));
}
// Design Time String Info
BOOL CIWChartManager::GetBlocksPosition( CString &p_strBlockPosition)
{
	if( !m_pMainBlock) return FALSE;
	p_strBlockPosition = m_pMainBlock->GetBlocksPosition();
	return !p_strBlockPosition.IsEmpty();
}

// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
CRect CIWChartManager::GetGraphRegionSumInColumn( const CPoint &p_ptParam) const
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	return m_pMainBlock->GetGraphRegionSumInColumn( p_ptParam);
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] Block�� Graph Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/13] Block���� �ش� ��ǥ�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager::CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName)
{
	if( !m_pMainBlock) return FALSE;
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->IsExistenceGraphInBlock( p_szGraphName, blockIndex);
}

// [04/10/26] �����Ǵ� Graph�� Data Packet Name�� ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager::GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames)
{
	if( !m_pMainBlock) return FALSE;
	p_strPacketNames = m_pMainBlock->GetGraphPacketName( p_szGraphName);
	return !p_strPacketNames.IsEmpty();
}

// [04/10/27] Ư�� Graph�� ��ϵ� ù Block�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager::GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC)
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

// (2006/2/13 - Seung-Won, Bae) Check! Is Empty?
BOOL CIWChartManager::IsEmpty( void)
{
	if( !m_pMainBlock) return TRUE;
	return m_pMainBlock->IsEmpty();
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] View�� ������ ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/13] �ش� ��ǥ�� �ִ� View�� Min/Max���� �����ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager::GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax)
{
	// 0. ���� Return ���� �ʱ�ȭ�Ѵ�.
	p_dMin = p_dMax = ERRORVALUE;
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->GetDisplayDataMinMax( p_szGraphName, p_dMin, p_dMax);
}

// [04/10/27] ������ Block�� Y Scale Conversion Type�� ���Ѵ�.
BOOL CIWChartManager::GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse)
{
	// 0. ���� MainBlock DLL�� Ȯ���Ѵ�.
	if( !m_pMainBlock) return FALSE;

	// 1. Conversion Type�� ���Ѵ�.
	CBlockIndex blockIndex( p_nR, p_nC);

	CBlock *pBlock = m_pMainBlock->GetBlock( blockIndex);
	if( !pBlock) return FALSE;
	p_bLog = pBlock->IsLogScale();
	p_bReverse = pBlock->HasInvertScale();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/6/14 - Seung-Won, Bae) Get Block Interface
//////////////////////////////////////////////////////////////////////

IBlock *CIWChartManager::GetBlock( int p_nRowIndex, int p_nColumnIndex)
{
	if( !m_pMainBlock) return NULL;

	CBlock *pBlock = m_pMainBlock->GetBlock( CBlockIndex( p_nRowIndex, p_nColumnIndex));
	if( !pBlock) return NULL;

	return CIWBlock::CreateObject( pBlock);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Graph Info
//////////////////////////////////////////////////////////////////////

// [04/10/27] Ư�� Graph�� ��ϵ� Block, ScaleGroup, Graph�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWChartManager::GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->GetIndexOfGraph( p_szGraphName, p_nR, p_nC, p_nV, p_nG);
}

// RQ�� ã�� : �������� - ojtaso (20070306)
BOOL CIWChartManager::GetIndexOfGraph( const char *p_szRQ, const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG)
{
	if( !m_pMainBlock) return FALSE;
	CString strGraphName;
	return m_pMainBlock->FindFirstIndicator( p_szGraphName, p_nR, p_nC, p_nV, p_nG, strGraphName, p_szRQ);
}

// (2007/1/20 - Seung-Won, Bae) Search Indicator
IString *CIWChartManager::FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ)
{
	if( !m_pMainBlock) return NULL;
	CString strGraphName;
	if( !m_pMainBlock->FindFirstIndicator( p_szIndicatorName, p_nR, p_nC, p_nV, p_nG, strGraphName, p_szRQ)) return NULL;
	return CIWString::CreateObject( strGraphName);
}
IString *CIWChartManager::FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ)
{
	if( !m_pMainBlock) return NULL;
	CString strGraphName;
	if( !m_pMainBlock->FindNextIndicator( p_szIndicatorName, p_nR, p_nC, p_nV, p_nG, strGraphName, p_szRQ)) return NULL;
	return CIWString::CreateObject( strGraphName);
}

//////////////////////////////////////////////////////////////////////
// XScale
//////////////////////////////////////////////////////////////////////

void CIWChartManager::SetXScaleManager(LPVOID lpXScaleManager)
{
	m_pXScaleManager = (CXScaleManager*)lpXScaleManager;
}

void CIWChartManager::RealDataReceived( const char *p_szRQ, BOOL bAppend, BOOL bCalc)
{
	if( m_pXScaleManager) m_pXScaleManager->RealDataReceived( p_szRQ);
}


// ���� ȭ�鿡 �������� ������ ���۰� �� ��ġ�� ���ɴϴ�.
BOOL CIWChartManager::GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex)
{
	if( !m_pXScaleManager) return FALSE;
	//m_pXScaleManager->GetStartEndDrawIdx_Cur(p_nStartIndex, p_nEndIndex);
	m_pXScaleManager->GetDrawIndex(p_szRQ, FALSE, FALSE, p_nStartIndex, p_nEndIndex);
	return TRUE;
}

// �ڷ� �ε����� x��ġ�� ���´�.
int CIWChartManager::GetXPosition(const char *p_szRQ, int nIdx, int nCol)
{
	if( !m_pXScaleManager) return FALSE;
	CXScaleManager::AREA area;
	m_pXScaleManager->GetAREAfRQnDTI(p_szRQ, nIdx, nCol, &area);

	int nRet = area.center;
	return nRet;
}

int CIWChartManager::GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight)
{
	if( !m_pXScaleManager) return FALSE;
	CXScaleManager::AREA area;
	m_pXScaleManager->GetAREAfRQnDTI(p_szRQ, nIdx, nCol, &area);

	nLeft = area.left;
	nRight = area.right;
	int nRet = area.center;
	return nRet;
}

void CIWChartManager::GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin)
{
	if(m_pXScaleManager)
		m_pXScaleManager->GetGraphXAreaSE( 0, nStart, nEnd, bIncludeMargin);
}

int CIWChartManager::GetGraphXArea_Start(BOOL bIncludeMargin)
{
	if( !m_pXScaleManager) return -1;
	return m_pXScaleManager->GetGraphXArea_Start( 0, bIncludeMargin);
}

int CIWChartManager::GetGraphXArea_End(BOOL bIncludeMargin)
{
	if( !m_pXScaleManager) return -1;
	return m_pXScaleManager->GetGraphXArea_End( 0, bIncludeMargin);
}

void CIWChartManager::GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin)
{
	if(m_pXScaleManager)
		m_pXScaleManager->GetGraphXAreaLR( 0, nLeft, nRight, bIncludeMargin);
}

int CIWChartManager::GetGraphXArea_Left(BOOL bIncludeMargin)
{
	if( !m_pXScaleManager) return -1;
	return m_pXScaleManager->GetGraphXArea_Left( 0, bIncludeMargin);
}

int CIWChartManager::GetGraphXArea_Right(BOOL bIncludeMargin)
{
	if( !m_pXScaleManager) return -1;
	return m_pXScaleManager->GetGraphXArea_Right( 0, bIncludeMargin);
}

int CIWChartManager::FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint, int nCol)
{
	if( !m_pXScaleManager) return -1;
	int nRet = 0;
	m_pXScaleManager->GetDTInAREAfRQnPt(p_szRQ, nXPoint, nCol, &nRet, NULL);
	return nRet;
}

time_t CIWChartManager::GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x)
{
	if( !m_pXScaleManager) return 0L;
	CXScaleManager::PAREA pparea = NULL;
	time_t ret = m_pXScaleManager->GetTimeFromX(nCursor_x, pnBlockColumn, &pparea);
	if( !pparea) return 0L;
	nCenter_x = pparea->center;
	return ret;
}

int CIWChartManager::GetXFromTime(time_t timeFind, int& nCenter_x)
{
	if( !m_pXScaleManager) return -1;
	CXScaleManager::PAREA pparea = NULL;
	int ret = m_pXScaleManager->GetXFromTime( 0, timeFind, &pparea);
	if( !pparea) return -1;
	nCenter_x = pparea->center;
	return ret;
}

// VertScaleGroup�� ���� ó�� Graph�� Ÿ�� : �������� - ojtaso (20070504)
double CIWChartManager::GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x)
{
	if( !m_pXScaleManager) return 0L;
	CXScaleManager::PAREA pparea = NULL;
	double ret = m_pXScaleManager->GetDoubleFromX(nRow, nCol, nCursor_x, pnBlockColumn, &pparea);
	if( !pparea) return 0L;
	nCenter_x = pparea->center;
	return ret;
}

// VertScaleGroup�� ���� ó�� Graph�� Ÿ�� : �������� - ojtaso (20070504)
int CIWChartManager::GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x)
{
	if( !m_pXScaleManager) return -1;
	CXScaleManager::PAREA pparea = NULL;
	int ret = m_pXScaleManager->GetXFromDouble(nRow, nCol, dblFind, &pparea);
	if( !pparea) return -1;
	nCenter_x = pparea->center;
	return ret;
}
