// IChartManager.h: interface for the IChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
#define AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"					// for AddIn Version Define

interface IBlock;
interface IString;

class CMainBlock;
interface IChartManager : public IUnknown  
{
protected:
	virtual ~IChartManager()	{}

// (2006/1/17 - Seung-Won, Bae) Get MainBlock Address for Special Usage
public:
	virtual CMainBlock *	GetObjectAddress( void)	= 0;

// [04/10/11] Block�� ��� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	virtual int		GetRowCount( void) = 0;
	virtual int		GetColumnCount( void) = 0;

// [04/10/11] Block�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// Block Region
	virtual CRect		GetBlockRegion( int p_nRowIndex, int p_nColumnIndex) = 0;	
	// Graph Region
						// View�� ��ü Graph �����̴�. (Block�� View �����̴�.)
	virtual CRect		GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex) = 0;	
	virtual CRect		GetNoFrameGraphRegion( CPoint p_pPoint) = 0;
						// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
	virtual CRect		GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex) = 0;
	virtual CRect		GetDrawingGraphRegion( CPoint p_pPoint) = 0;
	// Design Time String Info
	virtual BOOL		GetBlocksPosition( CString &p_strBlockPosition)	= 0;
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual CRect		GetGraphRegionSumInColumn( const CPoint &p_ptParam) const = 0;


// [04/10/13] Block�� Graph Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/13] Block���� �ش� ��ǥ�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName) = 0;
	// [04/10/26] �����Ǵ� Graph�� Data Packet Name�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames) = 0;
	// [04/10/27] Ư�� Graph�� ��ϵ� Block�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC) = 0;
	// (2006/2/13 - Seung-Won, Bae) Check! Is Empty?
	virtual BOOL	IsEmpty( void) = 0;


// [04/10/13] View�� ������ ���ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/13] �ش� ��ǥ�� �ִ� View�� Min/Max���� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax) = 0;
	// [04/10/27] ������ Block�� Y Scale Conversion Type�� ���Ѵ�.
	virtual BOOL	GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse) = 0;

// (2006/6/14 - Seung-Won, Bae) Get Block Interface
public:
	virtual	IBlock *	GetBlock( int p_nRowIndex, int p_nColumnIndex) = 0;

// (2006/6/25 - Seung-Won, Bae) Graph Info
public:
	// [04/10/27] Ư�� Graph�� ��ϵ� Block, ScaleGroup, Graph�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL		GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// RQ�� ã�� : �������� - ojtaso (20070306)
	virtual BOOL		GetIndexOfGraph( const char *p_szRQ, const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// (2007/1/20 - Seung-Won, Bae) Search Indicator
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ) = 0;
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ) = 0;

// [07/03/26] XScale�� ������ ���ϴ� Interface�� �����Ѵ�.
public:
	virtual void		SetXScaleManager(LPVOID lpXScaleManager) = 0;
	virtual void		RealDataReceived( const char *p_szRQ, BOOL bAppend, BOOL bCalc = TRUE) = 0;
	virtual BOOL		GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex) = 0;	// ���� ȭ�鿡 �������� ������ ���۰� �� ��ġ�� ���ɴϴ�.
	virtual int			GetXPosition(const char *p_szRQ, int nIdx, int nCol) = 0;
	virtual int			GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight) = 0;
	virtual void		GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Start(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_End(BOOL bIncludeMargin = FALSE) = 0;
	virtual void		GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Left(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Right(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint, int nCol=0) = 0;
	virtual time_t		GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x) = 0;
	virtual int			GetXFromTime(time_t timeFind, int& nCenter_x) = 0;
	// VertScaleGroup�� ���� ó�� Graph�� Ÿ�� : �������� - ojtaso (20070504)
	virtual double		GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x) = 0;
	virtual int			GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x) = 0;
};

#endif // !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
