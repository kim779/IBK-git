// IWChartManager.h: interface for the CIWChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
#define AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/MainBlock.h"		// for CMainBlock
#include "../_IWUnknown.h"
#include "_IChartManager.h"
#include "_IChartOCX.h"										// for IChartOCX

// (2004.10.11, ��¿�) MainBlock�� Interface Wrapper Class�� �����Ѵ�.
class CIWChartManager : public CIWUnknown, public IChartManager
{
// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IChartManager, CIWChartManager, CMainBlock, m_pMainBlock)


// (2006/1/17 - Seung-Won, Bae) Get MainBlock Address for Special Usage
protected:
	virtual CMainBlock *	GetObjectAddress( void)	{	return m_pMainBlock;	}

// [04/10/11] Block�� ��� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
protected:
	virtual int		GetRowCount( void);
	virtual int		GetColumnCount( void);

// [04/10/11] Block�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
protected:
	// Block Region
	virtual CRect		GetBlockRegion( int p_nRowIndex, int p_nColumnIndex);	
	// Graph Region
						// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
	virtual CRect		GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex);
	virtual CRect		GetNoFrameGraphRegion( CPoint p_pPoint);
						// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
	virtual CRect		GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex);
	virtual CRect		GetDrawingGraphRegion( CPoint p_pPoint);
	// Design Time String Info
	virtual BOOL		GetBlocksPosition( CString &p_strBlockPosition);
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual CRect		GetGraphRegionSumInColumn( const CPoint &p_ptParam) const;

// [04/10/13] Block�� Graph Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
protected:
	// [04/10/13] Block���� �ش� ��ǥ�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName);
	// [04/10/26] �����Ǵ� Graph�� Data Packet Name�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames);
	// [04/10/27] Ư�� Graph�� ��ϵ� ù Block�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC);
	// (2006/2/13 - Seung-Won, Bae) Check! Is Empty?
	virtual BOOL	IsEmpty( void);

// [04/10/13] View�� ������ ���ϴ� Interface�� �����Ѵ�.
protected:
	// [04/10/13] �ش� ��ǥ�� �ִ� View�� Min/Max���� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax);
	// [04/10/27] ������ Block�� Y Scale Conversion Type�� ���Ѵ�.
	virtual BOOL	GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse);

// (2006/6/14 - Seung-Won, Bae) Get Block Interface
protected:
	virtual	IBlock *	GetBlock( int p_nRowIndex, int p_nColumnIndex);

// (2006/6/25 - Seung-Won, Bae) Graph Info
protected:
	// [04/10/27] Ư�� Graph�� ��ϵ� Block, ScaleGroup, Graph�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL		GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG);
	// RQ�� ã�� : �������� - ojtaso (20070306)
	virtual BOOL		GetIndexOfGraph( const char *p_szRQ, const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) ;
	// (2007/1/20 - Seung-Won, Bae) Search Indicator
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ);
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ);
	
// [07/03/26] XScale�� ������ ���ϴ� Interface�� �����Ѵ�.
protected:
	CXScaleManager*		m_pXScaleManager;
	void SetXScaleManager(LPVOID lpXScaleManager);
	
	virtual void	RealDataReceived( const char *p_szRQ, BOOL bAppend, BOOL bCalc);
	virtual BOOL	GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex);	// ���� ȭ�鿡 �������� ������ ���۰� �� ��ġ�� ���ɴϴ�.
	virtual int		GetXPosition(const char *p_szRQ, int nIdx, int nCol);
	virtual int		GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight);

	virtual void	GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Start(BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_End(BOOL bIncludeMargin = FALSE);

	virtual void	GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Left(BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Right(BOOL bIncludeMargin = FALSE);
	virtual int		FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint, int nCol=0);
	virtual time_t	GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x);
	virtual int		GetXFromTime(time_t timeFind, int& nCenter_x);	
	// VertScaleGroup�� ���� ó�� Graph�� Ÿ�� : �������� - ojtaso (20070504)
	virtual double	GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x);
	virtual int		GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x);
};

#endif // !defined(AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
