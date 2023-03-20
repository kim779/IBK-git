// XScaleManager.h : interface for the CXScaleManager class.
//

#if !defined(AFX_XSCALEMANAGER_H__9BC10770_EEE0_4222_9E09_D247BF6D09EE__INCLUDED_)
#define AFX_XSCALEMANAGER_H__9BC10770_EEE0_4222_9E09_D247BF6D09EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* *********************************** */
/* ������: ���Ϸ�(yiilyoul@magicn.com) */
/* ������: 2007.03.29                  */
/* *********************************** */

#include <afxtempl.h>								// for CArray

#include "../DataMath.h"
#include "../BlockBaseEnum.h"						// for CScaleBaseData
#include "../DataBlockList.h"						// for CDataBlockList

#define VALUE_L_EMPTY_DTI	-1
#define VALUE_R_EMPTY_DTI	-100000000

#define THREAD_CALCULATION 1
#define THREAD_DRAWING	   2

class CMapTimeToLong : public CMapPtrToPtr
{
public:
	void	SetAt( time_t key, long value)
			{	CMapPtrToPtr::SetAt( ( void *)key, ( void *)value);	}
	BOOL	RemoveKey( time_t key)
			{	return CMapPtrToPtr::RemoveKey( ( void *)key);	}
	BOOL	Lookup( time_t key, long &rValue) const
			{
				void *pValue;
				BOOL bResult = CMapPtrToPtr::Lookup( ( void *)key, pValue);
				rValue = ( long)pValue;
				return bResult;
			}
};

// DRI : Draw Index	- �׸��� ��ġ
// DTI : Data Index	- ������ ��ġ
// RQ  : Request	- ��������(�ٸ� �ֱ�)�� ó���ϱ� ���ؼ��� �̷� (����+�ֱ�)�� �θ��� �� �ʿ��ϰ� �� �� RQ��� �մϴ�.
// NS  : Next Step	- ���� ��ġ
// ITEM : �׷����� �ϳ��� �׸��� �ǹ��մϴ�.

class CPacket;
class CPacketList;
class CPacketListManager;
class CIndicatorList;
interface IChartCtrl;

class CXScaleManager
{
public:
	typedef struct tagAREA
	{
		short left;
		short center;
		short right;

		tagAREA()
		{
			left = center = right = 0;
		}

		tagAREA(int left_p, int center_p, int right_p)
		{
			left = left_p;
			center = center_p;
			right = right_p;
		}

		void Offset(short nX)
		{
			if( nX == SHRT_MIN || nX == SHRT_MAX)
				left = center = right = nX;
			else
			{
				left += nX;
				center += nX;
				right += nX;
			}
		}

	} AREA, *PAREA;

	// (2008/12/30 - Seung-Won, Bae) Do not support CT_AUTO.
	typedef enum { CT_NONE, CT_DRI, CT_AREA, CT_ALL} CALC_TYPE;				// ��� ����
	typedef enum { RSAT_ITEM_COUNT_FIX, RSAT_ITEM_WIDTH_FIX}	RSA_TYPE;		// �������� ��(ITEM) ó�� ���	- [��(ITEM)�� �� ���� ���, ��(ITEM)�� ���� ���� ���]
	
// ��������
public:
	int		m_nThread_Flag; //��Ʈ ���� ���� ���� 1:���, 2:�׸���

public:
	CXScaleManager(void)
	{		
	};
	
	virtual ~CXScaleManager(void) {};

	// Retrieve the RQ Index. (RQ is indexed in m_midxRQ. but why?)
	virtual int			GetRQIndexFromString( CString strRQ) = 0;

	virtual void					SetPacketListManager(CPacketListManager* ppacketListManager) = 0;
	virtual void					SetIndicatorList(CIndicatorList* pIndicatorList) = 0;
	virtual CPacketListManager *	GetPacketListManager( void) = 0;

	virtual void SetApplyToolBar(BOOL bApplyToolBar) = 0;								// ���ٿ� �ݿ������� ���θ� �ӽ� ����ϱ� ���ؼ� ���
	virtual BOOL GetApplyToolBar() = 0;													// CMainBlockImplementation::CalculateDataForDrawing, CKoscomChartCtrl::SetStartEndIndex ����

	virtual void SetReverse( BOOL bReverse) = 0;										// ������(REVERSE) ���
	virtual BOOL GetReverse() = 0;

	virtual int	GetMinScaleInterval_nUnit( void) = 0;

	virtual void SetWeightedVolume(BOOL bWeightedVolume, BOOL bCalc) = 0;				// �ŷ��� ����ġ ���
	virtual int	 GetWeightedVolume() = 0;

	virtual void SetFutureTimeCount( int p_nFutureTimeCount) = 0;									// �̷����� Ȯ�� ���
	virtual int	 GetFutureTimeCount() = 0;

	virtual void SetLeftDataHideCount(int nLeftDataHideCount, BOOL bCalc) = 0;			// ���� ������ ����� ���
	virtual int	 GetLeftDataHideCount() = 0;

	virtual BOOL GetUseOEBTMargin() = 0;												// �ϸ����ǥ ���뿩��(��뿩��)
	virtual void SetUseOEBTMargin(BOOL bUseOEBTMargin) = 0;
	
	virtual void SetRightMarginWidth( int nRightMarginWidth) = 0;						// ���� ���� ���
	virtual void SetRightMarginAndFutureTime( int p_nRightMarginWidth, int p_nFutureTimeCount) = 0;		// (2009/5/17 - Seung-Won, Bae) for Right Margin and Future Time

	virtual void SetCandleTypeOnResize( RSA_TYPE rmtype) = 0;					// ���� ���� ���� ��� ����
	virtual RSA_TYPE GetCandleTypeOnResize() = 0;
	
	// ������ ���� : xScaleManager - ojtaso (20070528)
	virtual void SetFixedInterval(BOOL bFixedInterval, BOOL bCalc) = 0;
	virtual BOOL IsFixedInterval() = 0;

//	virtual void SetResizeType(RSA_TYPE rstype) = 0;									// �������� ���� ��� ����
//	virtual RSA_TYPE GetResizeType() = 0;

	virtual void AddOEBTRQ(CString strRQ, CString strGraphName, CString strIndicatorName) = 0;
																						// �ϸ� ����ǥ�� �ҽ��� ������� RQ �߰�
	virtual void DelOEBTRQ( CString strRQ) = 0;
																						// �ϸ� ����ǥ�� �ҽ��� ������� RQ ����
	virtual void Clear_OEBTRQ() = 0;													// �ϸ� ����ǥ�� �ҽ��� ������� RQ���� ����� ��� �����ݴϴ�.

	virtual void Calc_DRI( BOOL p_bWithMerge = TRUE) = 0;													// �������� ������ (DRIfDTI, DTIfDRI...)�� �ٽ� ����մϴ�.
	virtual void Calc_DRI_Merge( void) = 0;	// Calculate DRI and Time Line from DTI.
	virtual void RealDataReceived( CString strRQ, BOOL p_bIndexedReal = FALSE) = 0;		// REAL DATA ���Žÿ� ȣ��

	virtual void SetBlockColumnCount(int nBlockColumn_Count) = 0;						// ����÷��� ������ �����մϴ�.
	virtual void SetBlockColumn_Cur(int nBlockColumn) = 0;								// �ٸ� �Լ�(������, �׷��� ���� �Լ�)���� �⺻���� ������ ����÷��� �����մϴ�.

	// OCX ���� : xScaleManager - ojtaso (20070504)
	virtual void SetIChartCtrl(IChartCtrl* p_pICharetCtrl) = 0;

	/**************************************************************************/
	/* ��ũ��, �����̴� ���� ����ϴ� �Լ���								  */
	/**************************************************************************/
	virtual int GetCount() = 0;															// ��ü ������ ���ɴϴ�.
	virtual int GetDrawCount_Cur() = 0;													// ���� ȭ�鿡 �������� ������ ���ɴϴ�.
	virtual int GetStartDrawIdx_Cur() = 0;												// ���� ȭ�鿡 �������� ������ ���� ��ġ�� ���ɴϴ�.
	virtual int GetEndDrawIdx_Cur() = 0;												// ���� ȭ�鿡 �������� ������ �� ��ġ�� ���ɴϴ�.
	virtual void SetStartEndDrawIdx_Cur(int nStartDrawIdx, int nEndDrawIdx, BOOL p_bMoreZoomIn = FALSE) = 0;		// ���� ȭ�鿡 �������� ������ ����,�� ��ġ�� �����մϴ�.
	virtual void GetStartEndDrawIdx_Cur(int& nStartDrawIdx, int& nEndDrawIdx) = 0;		// ���� ȭ�鿡 �������� ������ ����,�� ��ġ�� ���ɴϴ�.
	virtual int GetPageCountNextStep(const int nSliderPos_Prev,	const int nSliderPos_New, const int nSBCode, int nMin = -1, int nMax = -1) = 0;
																					// ���� �������� �̵��Ҽ� �ִ� ��(������)������ �Ѱ��ݴϴ�.
																						// (���� ������ ��(������)������ ����ڰ� ������� �����Ҽ� �����ϴ�.	
	/**************************************************************************/
	/* �׷����� �׸��� ����ϴ� �Լ���										  */
	/**************************************************************************/
	virtual void GetDrawIndex(CString strRQ, BOOL bIncludePrev, BOOL bOEBT, 
							int& nDTI_Start, int& nDTI_End) = 0;						// �׷����� ���� �������� �������� �׷����� ���� ������ �Ѱ��ݴϴ�.
	// ���� ���õ� RQ�� �ڵ����� ȣ�� : xScaleManager - ojtaso (20070409)
	virtual void GetDrawIndex(BOOL bIncludePrev, BOOL bOEBT, int& nDTI_Start, int& nDTI_End) = 0;
	virtual int GetDTIFromDRI_AbleRange(CString strRQ, int nDRI, int* pnDTI_Org = NULL) = 0;
	
	/**************************************************************************/
	/* �������� �׸��� ����ϴ� �Լ���										  */
	/**************************************************************************/
	// RQ�� Unit���� ���� : �������� - ojtaso (20070601)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetScaleUnit_Max(LPCTSTR lpszRQ = NULL) = 0;	// �������� �׸��� ����� Type�����Դϴ�.
	virtual int GetScaleInterval_Unit_Max() = 0;
	
	virtual void SetCurScaleInfo(int nDRI = -1) = 0;									// ������ ������ ���� �ϴ� ��ġ�� �����մϴ�.
	virtual void GetCurScaleInfo(int& nDRI) = 0;										// ������ ������ �������� �ϴ� ��ġ�� ���ɴϴ�.
	virtual BOOL NextScaleTime(time_t* ptimeScale, PAREA* pparea) = 0;					// ���� ������ ��ġ�� �ð� ������ ���մϴ�.
	virtual void PrevScaleTime(time_t* ptimeScale, PAREA* pparea) = 0;					// ���� ������ ��ġ�� �ð� ������ ���մϴ�.
	virtual void GetAt_ScaleTime(int nDRI, time_t* ptimeScale, PAREA* pparea) = 0;
	
	// ��ŶŸ�� : xScaleManager - ojtaso (20070703)
	virtual CString GetPacketType(LPCTSTR lpszRQ) = 0;

	/**************************************************************************/
	/* �߼���, ���ڼ����� ����Ҽ� �ִ� �Լ�								  */
	/**************************************************************************/
	
																					// �߼������� Pixel���� �ش��ϴ� Data Index�� ���� ������ ���ϰ��� �Ҷ� ����մϴ�.
	virtual BOOL GetAREAfRQnDTI(CString strRQ, int nDTI, int nBlockColumn, PAREA parea) = 0;										
	// ���� ���õ� RQ�� �ڵ����� ȣ�� : xScaleManager - ojtaso (20070409)
	virtual BOOL GetAREAfRQnDTI(int nDTI, int nBlockColumn, PAREA parea) = 0;
																						// �߼������� Ư�� RQ�� DTI�� �ش��ϴ� ���� ������ ���ϰ��� �Ҷ� ����մϴ�.
	virtual BOOL GetRQInfosFromPt(int nCursor_x, int& nBlockColumn, CStringArray& saRQ, CArray<int, int>& naDTI, PAREA parea) = 0;
																						// ���ڼ��� ���� ������ Pixel���� RQ��, DTI��, DRI_AREA�� ������ ���ϰ��� �Ҷ� ����մϴ�.
	virtual time_t GetTimeFromX(int nCursor_x, int* pnBlockColumn = NULL, PAREA* pparea = NULL) = 0;
	

	// double������ ��ȯ : xScaleManager - ojtaso (20070402)
	virtual double GetDoubleFromX( const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, PAREA* pparea, const char *p_szRQ = NULL) = 0;
	virtual int GetXFromDouble(const int nRow, const int nCol, double dblFind, PAREA* pparea = NULL) = 0;

	/**************************************************************************/
	/* �׹ۿ� ������ ����Ҽ� �ִ� �Լ�										  */
	/**************************************************************************/	
	virtual int GetScaleUnitAlphaWeight(CString strRQ) = 0;
	virtual BOOL IsUseable() = 0;

	virtual void GetTimeFromDRI(int nDRI, time_t* ptimeScale) = 0;
	// xScale �ʱ�ȭ : xScaleManager - ojtaso (20070628)
	virtual void Initialize(BOOL bCalc = FALSE) = 0;
	// ������ ����Ÿ���� : xScaleManager - ojtaso (20070703)
//	virtual BOOL IsStringDataType() = 0;
	// �÷����� : xScaleManager - ojtaso (20071128)
//	virtual int GetCountOfBlockColumn() = 0;

	virtual void SetThreadFlag(int nFlag) = 0;
	virtual int  GetThreadFlag() = 0;

// (2009/1/16 - Seung-Won, Bae) Do not call calc_area by any way except the OnDraw.
protected:
	friend UINT CKoscomChartCtrl_PrepareOnDraw( LPVOID pParam);

// (2009/1/16 - Seung-Won, Bae) Support the Re-Calculation Mode on OCX OnDrawing
public:
	virtual int		GetCalculationMode( void) = 0;	// 0:No-Calc, 1:CalcByReal, 2:CalcAll
	virtual void	SetNeedCalcAreaFlag( int p_nNeedCalcArea) = 0;

// (2009/1/19 - Seung-Won, Bae) for Scale Time List
public:
	virtual BOOL	IsSpecialTime( const char *p_szRQ, time_t p_time, long &p_lSpecialTimeCode) = 0;

// (2009/2/12 - Seung-Won, Bae) Support default data space.
public:
	virtual void	SetDefaultDataSpace( int p_nDefaultDataSpace, int p_nMinDataCountOnOnePage) = 0;

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
public:
	virtual void						Calc_AREA( int p_nColumnCount) = 0;													// �׸��� ���� �ʿ��� ������ �ٽ� ����մϴ�.	
	virtual int							GetColumnWidth( int p_nColumn) = 0;
	virtual int							GetDRInFromPt( int nCursor_x, int *pnBlockColumn = NULL, PAREA* pparea = NULL, BOOL bIncludeRightMargin = FALSE, BOOL p_bWithBoundCheck = FALSE) = 0;	// �������鵵 ���Ե� �ð����� : xScaleManager - ojtaso (20070608)
	virtual int							GetGraphXArea_End( int p_nColumn, BOOL bIncludeMargin = FALSE) = 0;
	virtual int							GetGraphXArea_Left( int p_nColumn, BOOL bIncludeMargin = FALSE) = 0;
	virtual int							GetGraphXArea_Right( int p_nColumn, BOOL bIncludeMargin = FALSE) = 0;
	virtual int							GetGraphXArea_Start( int p_nColumn, BOOL bIncludeMargin = FALSE) = 0;
	virtual int							GetScaleDRIandTime( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
											const int p_nScaleRegionWidth, const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const int p_nTimeInterval, 
											const char *p_szRQ, int &p_nSkipFactor, const CString &p_strPacketType) = 0;
	virtual int							GetScreenWidth( int p_nColumn) = 0;
	virtual int							GetXFromTime( int p_nColumn, time_t timeFind, PAREA* pparea = NULL, BOOL bIncludeRightMargin = FALSE) = 0;	// �������鵵 ���Ե� X���� : xScaleManager - ojtaso (20070608)
	virtual BOOL						GetDTInAREAfRQnPt( int nCursor_x, int& nBlockColumn, int* pnDTI, PAREA parea) = 0;	// ���� ���õ� RQ�� �ڵ����� ȣ�� : xScaleManager - ojtaso (20070409)
	virtual BOOL						GetDTInAREAfRQnPt( CString strRQ, int nCursor_x, int& nBlockColumn, int* pnDTI = NULL, PAREA parea = NULL, BOOL p_bWithBoundCheck = FALSE) = 0;
	virtual BOOL						IsAllOnePixel( int p_nColumn, const char *p_szRQ) = 0;
	virtual BOOL						IsInGraphXArea( int p_nColumn, int nX, BOOL bIncludeEqual = FALSE) = 0;				// Include equal: ������ ���� �����ؼ� 
	virtual void						GetAreaFromDRI( int p_nColumn, int nDRI, PAREA* parea) = 0;
	virtual void						GetGraphXAreaLR( int p_nColumn, int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE) = 0;
	virtual void						GetGraphXAreaSE( int p_nColumn, int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE) = 0;
	virtual void						SetColumnWidth( int p_nColumn, int p_nColumnWidth) = 0;
	virtual void						SetScreenRegion( int p_nColumn, int nLeft, int nWidth, BOOL p_bReCalc = FALSE) = 0;				// ȭ�� ���� �����մϴ�.
	virtual CDataBlockList< AREA> *		GetDTIArea( int p_nColumn, const char *p_szRQ, int &p_nZeroDTI) = 0;	// (2009/1/10 - Seung-Won, Bae) Support Area array of DTI for drawing.

// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
public:
	virtual void	SetFlagForCalcDRIandScrollByReal( BOOL p_bWithTrucated = FALSE) = 0;
	virtual BOOL	IsScrollByReal( void) = 0;

// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
public:
	virtual void	OnUserZoomChanged( void) = 0;

// (2009/5/28 - Seung-Won, Bae) Check! Is Data Empty?
public:
	virtual BOOL	IsEmptyData( void) = 0;
	
	// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
	virtual void AddVerticalBoxInfo(LPCTSTR lpszRQ, double dHorzValue, COLORREF clrColor, BOOL bFill) = 0;
	virtual void GetVerticalBoxInfo(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, CRect& rcBox, COLORREF& clrColor, BOOL& bFill) = 0;

	virtual int		GetPriceMinMaxShowType( void) = 0;
	virtual void	SetPriceMinMaxShowType( int nMinMaxRatio) = 0;

	virtual int		GetScaleDRIandTimeAllData( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
											const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType,	const char *p_szRQ, int &p_nSkipFactor) = 0;

};

typedef CXScaleManager *(*FN_CreateXScaleManager)( void); 

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_XSCALEMANAGER_H__9BC10770_EEE0_4222_9E09_D247BF6D09EE__INCLUDED_)
