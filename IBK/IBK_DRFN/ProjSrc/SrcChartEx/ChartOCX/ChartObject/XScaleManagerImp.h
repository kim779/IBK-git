
/* *********************************** */
/* ������: ���Ϸ�(yiilyoul@magicn.com) */
/* ������: 2007.03.29                  */
/* *********************************** */

#pragma once
#pragma warning(disable: 4786)

#include <map>
#include <string>
#include <vector>
#include <set>

#include "../Include_Chart/Dll_Load/XScaleManager.h"
#include "../Include_Chart/DataMath.h"

#include "XScaleTimeDiffManager.h"						// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
#include "XScaleGridTime.h"								// for CXScaleGridTime

// DRI : Draw Index	- �׸��� ��ġ
// DTI : Data Index	- ������ ��ġ
// RQ  : Request	- ��������(�ٸ� �ֱ�)�� ó���ϱ� ���ؼ��� �̷� (����+�ֱ�)�� �θ��� �� �ʿ��ϰ� �� �� RQ��� �մϴ�.
// NS  : Next Step	- ���� ��ġ
// ITEM : �׷����� �ϳ��� �׸��� �ǹ��մϴ�.
// OEBT : one eye balance table(�ϸ� ����ǥ...) ^^
// BC  : BlockColumn

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
#define _MAX_COL	10

class CPacket;
class CPacketRQ;
class CPacketList;
class CPacketListManager;
class CIndicatorList;
interface IChartCtrl;

class CXScaleManagerImp : public CXScaleManager
{
public:
	CXScaleManagerImp(void);

public:
	virtual ~CXScaleManagerImp(void);

public:
	// RQ�� ������ ������ ����ü
	typedef struct tagRQ_INFO
	{
		CPacketRQ *							pPacketRQ;				// ?
		CPacketList *						pPacketList;			// ?
		CPacket *							pTimePacket;			// ?
		int									nDTI_Start_Data;		// the prev DTI on view (if none, it is the first DTI on view).
		int									nDTI_End_Data;			// the last DTI on view.

		CScaleBaseData::HORZSCALEDRAWERTYPE	eScaleUnit;				// the time type of RQ.
		int									nScaleInterval_Unit;	// the time unit of the time line of RQ
		
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//int									nScaleInterval_Sec;		// Temporary Variable to calculate the Min/Max Scale Info.
		__int64									nScaleInterval_Sec;		// Temporary Variable to calculate the Min/Max Scale Info.
		// 2011.01.24 by SYS <<

		int									nLastRealDTI;			// the DTI that will be receive Real Data.
		BOOL								bRealReceived;			// the Flag if Real Data is received.
		int									nUpdatedRealDRI;		// the DRI that was changed by Real Data.
		BOOL								m_bStringDataType;		// ������ ����Ÿ���� : xScaleManager - ojtaso (20070703)
		long								m_lEndSpecialTimeCode;	// (2009/5/28 - Seung-Won, Bae) the future can be steady special time.
	} RQ_INFO, *PRQ_INFO;

	// �ϸ����ǥ�� ������ ������ ����ü
	typedef struct
	{
		CString strGraphName;
		int nFutureCount;
	} OEBT_INFO, *POEBT_INFO;

	// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
	typedef struct
	{
		double dHorzValue;
		COLORREF clrColor;
		BOOL bFill;
	} VERTBOX_INFO, *PVERTBOX_INFO;

	typedef map<string, CDataBlockList<AREA>*>		MAP_RQ_AREAfDTI;
	typedef pair<string, CDataBlockList<AREA>*>		PAIR_RQ_AREAfDTI;

	typedef map<string, int>						MAP_RQ_INDEX;
	typedef pair<string, int>						PAIR_RQ_INDEX;

	// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
	typedef map<string, VERTBOX_INFO*>				MAP_RQ_VERTBOX;
	typedef pair<string, VERTBOX_INFO*>				PAIR_RQ_VERTBOX;

	typedef struct tagBC_INFO
	{
		tagBC_INFO()
		{
			nScreenLeft = 0;
			nScreenWidth = 0;
		}

		int nScreenLeft;
		int nScreenWidth;

		int dItemWidth;
		int nItemWidth;

		MAP_RQ_AREAfDTI			mblAreafDTI;
		CDataBlockList<AREA>	blAreafDRI;
	} BC_INFO, *PBC_INFO;
	
	typedef vector<PBC_INFO>					VECTOR_BC_INFO;

	// ���� �׸��� ��ġ ������ ������ ����ü
	typedef struct
	{
		CString strRQ;
		CDataBlockList<AREA>* pAreafDTI;
		int nDTI_Start_Data;
	} DRAW_INFO;

	// ���� ������ ��ġ ������ ������ ����ü
	typedef struct
	{
		int nIndex;
	} SCALE_INFO;

// ���κ���
protected:

//	VECTOR_BC_INFO			m_vBCInfo;			// Block Column ���� ������ ������ �ֽ��ϴ�.
//	PBC_INFO				m_pbcinfo_cur;		// ���� ���õ� Block Column ������ ������ �ֽ��ϴ�.

	BOOL m_bApplyToolBar;				// ���ٿ� �ݿ������� ���θ� ����ϱ� ���� ���� ����

	// �ܺ� ��ü��
	BOOL m_bReverse;					// ������
	BOOL m_bWeightedVolume;				// �ŷ��� ����ġ
	BOOL m_bUseOEBTMargin;				// �ϸ����ǥ ���뿩��(��뿩��)
	int	 m_nLeftDataHideCount;			// ���� ������ �����
	int	 m_nRightMarginWidth;			// ���� ����

	RSA_TYPE m_rstype;					// �������� ���� ��� ( RSAT_ITEM_WIDTH_FIX, RSAT_ITEM_COUNT_FIX )
	BOOL m_bFixedInterval;				// ������ ���� : xScaleManager - ojtaso (20070528)

	int	 m_nDRI_Start_Cur;				// ���� - �׸��� ���� DRI
	int	 m_nDRI_End_Cur;				// ���� - �׸��� �� DRI
	int	 m_nDrawCount_Cur;				// ���� - �׸��� ����
	
	int	m_nMinDataCountOnOnePage;		// �ּ� Count
	
	SCALE_INFO	m_scaleinfo;			// �������� �׸��� ����ϴ� ���� ���� ����

	AREA	m_areaNONE;
	AREA	m_areaTEMP;

	CPacketListManager* m_ppacketListManager;
	CIndicatorList* m_pIndicatorList;

	IChartCtrl* m_pIChartCtrl;	// ocx interface
	
// �����Լ�
protected:
	inline BOOL EndOfRQs( int* naDTI_Cur);
	time_t GetLeastTime( int* naDTI_Cur, int* naIRQ, int* pnIRQ_Count);
	
	void Calc_RQ_Scale_MinMax();

	void ClearData();

	int GetNextDRI_ByScaleInterval(CScaleBaseData::HORZSCALEDRAWERTYPE ScaleUnit, int nScaleInterval_Unit, int nDRI_Cur);
	
	//time_t GetTimeFromTimeValue(unsigned __int64 lTime, CString strType, RQ_INFO* pifRQ, tm &tmResult, const char *p_szRQ, CMapTimeToLong *p_pamapSpecialTime = NULL);

	// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	time_t GetTimeFromTimeValue(unsigned __int64 lTime, CString strType, RQ_INFO* pifRQ, tm &tmResult, const char *p_szRQ, CMapTimeToLong *p_pamapSpecialTime = NULL);
	// 2011.01.27 by SYS <<

	// time_t�� double�� ��ȯ : xScaleManager - ojtaso (20070405)
	double GetTimeValueFromTime(time_t tmValue, CString strType);

	int GetDTIFromDRI(CString strRQ, int nDRI);	

// �����Լ�
public:
	virtual void					SetPacketListManager( CPacketListManager* ppacketListManager);
	virtual void					SetIndicatorList( CIndicatorList* pIndicatorList);
	virtual CPacketListManager *	GetPacketListManager( void)		{	return m_ppacketListManager;	}

	virtual void SetApplyToolBar(BOOL bApplyToolBar);								// ���ٿ� �ݿ������� ���θ� �ӽ� ����ϱ� ���ؼ� ���
	virtual BOOL GetApplyToolBar();
	
	virtual void SetReverse( BOOL bReverse);										// ������(REVERSE) ���
	virtual BOOL GetReverse();

	virtual void SetWeightedVolume(BOOL bWeightedVolume, BOOL bCalc);				// �ŷ��� ����ġ ���
	virtual BOOL GetWeightedVolume();

	virtual void SetLeftDataHideCount(int nLeftDataHideCount, BOOL bCalc);			// ���� ������ ����� ���
	virtual int	 GetLeftDataHideCount();
	
	virtual BOOL GetUseOEBTMargin();												// �ϸ����ǥ ���뿩��(��뿩��)
	virtual void SetUseOEBTMargin(BOOL bUseOEBTMargin);

	virtual void SetRightMarginWidth(int nRightMarginWidth);						// ���� ���� ���

	virtual void SetCandleTypeOnResize( RSA_TYPE rmtype);							// ���� ���� ���� ��� ����
	virtual RSA_TYPE GetCandleTypeOnResize();

	// ������ ���� : xScaleManager - ojtaso (20070528)
	virtual void SetFixedInterval(BOOL bFixedInterval, BOOL bCalc);
	virtual BOOL IsFixedInterval();

//	virtual void SetResizeType(RSA_TYPE rstype);									// �������� ���� ��� ����
//	virtual RSA_TYPE GetResizeType();

	virtual void AddOEBTRQ( CString strRQ, CString strGraphName, CString strIndicatorName);
																					// �ϸ����ǥ�� �ҽ��� ������� RQ �߰�
	virtual void DelOEBTRQ( CString strRQ);
																					// �ϸ����ǥ�� �ҽ��� ������� RQ ����
	virtual void Clear_OEBTRQ();													// �ϸ����ǥ�� �ҽ��� ������� RQ���� ����� ��� �����ݴϴ�.

	virtual void RealDataReceived( CString strRQ, BOOL p_bIndexedReal = FALSE);		// REAL DATA ���Žÿ� ȣ��

	// OCX ���� : xScaleManager - ojtaso (20070504)
	virtual void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);

	/**************************************************************************/
	/* ��ũ��, �����̴� ���� ����ϴ� �Լ���								  */
	/**************************************************************************/
	virtual int GetCount();															// ��ü ������ ���ɴϴ�.
	virtual int GetDrawCount_Cur();													// ���� ȭ�鿡 �������� ������ ���ɴϴ�.
	virtual int GetStartDrawIdx_Cur();												// ���� ȭ�鿡 �������� ������ ���� ��ġ�� ���ɴϴ�.
	virtual int GetEndDrawIdx_Cur();												// ���� ȭ�鿡 �������� ������ �� ��ġ�� ���ɴϴ�.
	virtual void SetStartEndDrawIdx_Cur(int nStartDrawIdx, int nEndDrawIdx, BOOL p_bMoreZoomIn = FALSE);		// ���� ȭ�鿡 �������� ������ ����,�� ��ġ�� �����մϴ�.
	virtual void GetStartEndDrawIdx_Cur(int& nStartDrawIdx, int& nEndDrawIdx);		// ���� ȭ�鿡 �������� ������ ����,�� ��ġ�� ���ɴϴ�.
	virtual int GetPageCountNextStep(const int nSliderPos_Prev,	const int nSliderPos_New, const int nSBCode, int nMin = -1, int nMax = -1);
																					// ���� �������� �̵��Ҽ� �ִ� ��(������)������ �Ѱ��ݴϴ�.
																					// (���� ������ ��(������)������ ����ڰ� ������� �����Ҽ� �����ϴ�.
	
	/**************************************************************************/
	/* �׷����� �׸��� ����ϴ� �Լ���										  */
	/**************************************************************************/
	virtual void GetDrawIndex(CString strRQ, BOOL bIncludePrev, BOOL bOEBT, int& nDTI_Start, int& nDTI_End);
	// ���� ���õ� RQ�� �ڵ����� ȣ�� : xScaleManager - ojtaso (20070409)
	virtual void GetDrawIndex(BOOL bIncludePrev, BOOL bOEBT, int& nDTI_Start, int& nDTI_End);

	virtual int GetDTIFromDRI_AbleRange(CString strRQ, int nDRI, int* pnDTI_Org = NULL);
	
	/**************************************************************************/
	/* �������� �׸��� ����ϴ� �Լ���										  */
	/**************************************************************************/
	// RQ�� Unit���� ���� : �������� - ojtaso (20070601)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetScaleUnit_Max(LPCTSTR lpszRQ = NULL);			// �������� �׸��� ����� Type�����Դϴ�.
	virtual int GetScaleInterval_Unit_Max();
	virtual void SetCurScaleInfo(int nDRI = -1);									// ������ ������ ���� �ϴ� ��ġ�� �����մϴ�.
	virtual void GetCurScaleInfo(int& nDRI);										// ������ ������ �������� �ϴ� ��ġ�� ���ɴϴ�.
	
	// ��ŶŸ�� : xScaleManager - ojtaso (20070703)
	virtual CString GetPacketType(LPCTSTR lpszRQ);

	/**************************************************************************/
	/* �׹ۿ� ������ ����Ҽ� �ִ� �Լ�										  */
	/**************************************************************************/	
	virtual int GetScaleUnitAlphaWeight(CString strRQ);
	virtual BOOL IsUseable();
	virtual void GetTimeFromDRI(int nDRI, time_t* ptimeScale);
	// xScale �ʱ�ȭ : xScaleManager - ojtaso (20070628)
	virtual void Initialize(BOOL bCalc = FALSE);
	// ������ ����Ÿ���� : xScaleManager - ojtaso (20070703)
//	virtual BOOL IsStringDataType();
	// �÷����� : xScaleManager - ojtaso (20071128)
//	virtual int GetCountOfBlockColumn();

	virtual void SetThreadFlag(int nFlag);
	virtual int	 GetThreadFlag();

	// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
	virtual void AddVerticalBoxInfo(LPCTSTR lpszRQ, double dHorzValue, COLORREF clrColor, BOOL bFill);
	virtual void GetVerticalBoxInfo(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, CRect& rcBox, COLORREF& clrColor, BOOL& bFill);

//	Global X scale data.
protected:
	int						m_nCountOfRQ_Area;	// It is previouis number of RQ for ReInitData_Area().
	int						m_nItemCount_Total;	// the count of time item in full time line with default data space.


// X Scale data for each RQ.
//	Manage the RQ with Index. but why?
protected:
	// Retrieve the RQ Index. (RQ is indexed in m_midxRQ. but why?)
	virtual int			GetRQIndexFromString( CString strRQ);

// (2008/12/28 - Seung-Won, Bae) Manage temp pointer variable to check NULL from the CDataBlockList's NULL return.
protected:
	const time_t *		m_pTReturn;
	const int *			m_pIReturn;
	const AREA *		m_pAReturn;
	const tm *			m_pMReturn;

// (2008/12/27 - Seung-Won, Bae) Comment.
//////////////////////////////////////////////////////////////////////
//	RQ Info (all data is array with index of RQ.)
//////////////////////////////////////////////////////////////////////
protected:
	int							m_nCountOfRQ;		// It is number of RQ.
													//	But it initialized with CPacketListManager's PacketList Count in Calc_DRI().
	MAP_RQ_INDEX				m_midxRQ;			// RQ Index.
	CStringArray				m_saRQ;				// RQ Names.
	RQ_INFO**					m_pifaRQ;			// RQ Infos.
	OEBT_INFO**					m_pifaOEBT;			// Obviously Balance Chart Infos.
	// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
	MAP_RQ_VERTBOX				m_mifVertBox;

//////////////////////////////////////////////////////////////////////
//	Data indexed DTI for all RQs (all data is array with index of RQ.)
//////////////////////////////////////////////////////////////////////
protected:	// Data, second-indexed by DTI of RQs.
	CDataBlockList<time_t>**	m_pblaRQTime;		// the Time Line on DTI.
													//		It's order is DTI.
													//		Data count is same with Time Packet.
	CDataBlockList<int>**		m_pblaDRIfDTI;		// the DRI on DTI. 
													//		It has end-mark DTI.
													//			It is needed for Cacl_Area().
													//				Cacl_Area() calculate the Area from start DTI to end DTI of view with next DTI.
													//			It can be reset with future DRI.
													//		It's order is DTI.
	CMapTimeToLong **			m_pamapSpecialTime;	// the map of special time list.

//////////////////////////////////////////////////////////////////////
//	Data indexed DRI for all RQ (all data is array with index of RQ.)
//////////////////////////////////////////////////////////////////////
protected:	// Data indexed by DRI of RQs
	CDataBlockList<int>**		m_pblaDTIfDRI;		// the Mapping from DRI to DTI.
													//		It's order is DRI.
													//		It has negative(no matched) DTI for the future DRI.

//////////////////////////////////////////////////////////////////////
//	Main Time Line (full time line indexed DRI)
//////////////////////////////////////////////////////////////////////
protected:
	CDataBlockList<time_t>	m_blTimefDRI;		// XScale�� ���ϵ� �ð� ����
												// the full time line of X Scale Manager.
												//	the time item order is DRI.
												//	and It can have the future or hidden past.

// Maximum Scale Info for drawing the x-scale.
protected:
	int									m_maxScaleInterval_nRQ;
	CScaleBaseData::HORZSCALEDRAWERTYPE	m_maxScaleInterval_eType;
	int									m_maxScaleInterval_nUnit;

// Minimum Scale Info to calculate the time of future.
protected:
	// Interval�� 5�� ��Ʈ�� ��쿡�� 5�� �˴ϴ�.
	//	ex) ��,��,�� RQ�� ��쿡�� �� �������� �׸����� �մϴ�.
	// ���� ������ Ÿ�� ����(���� ������ ū ������ �������� �׸��� �˴ϴ�.)
	int									m_minScaleInterval_nRQ;
	CScaleBaseData::HORZSCALEDRAWERTYPE	m_minScaleInterval_eType;
	int									m_minScaleInterval_nUnit;
protected:
	virtual int		GetMinScaleInterval_nUnit( void)	{	return m_minScaleInterval_nUnit;	}

// Retrieve the DTI from DRI
protected:
			//	Get DTI Value from DRI.
			//		if No DTI for past. -1
			//		if No DTI for future. End Mark DTI.
	int		GetDTIFromDRI( CDataBlockList< int>* pblDTIfDRI, int nDRI);
			//	Get DTI Index from DRI.
			//		if No DTI for past.		0
			//		if No DTI for future.	Last DTI. (can be calculated by EndMark.)
	int		GetDTIFromDRI_AbleRange( CDataBlockList< int>* pblDTIfDRI, int nDRI, int* pnDTI_Org = NULL);

// (2009/1/8 - Seung-Won, Bae) Manage the flags for recalc AREA
protected:
	BOOL	m_bWholeViewPre;

// (2009/1/11 - Seung-Won, Bae) for Comment.
protected:
	virtual void	Calc_DRI( BOOL p_bWithMerge = TRUE);													// �������� ������ (DRIfDTI, DTIfDRI...)�� �ٽ� ����մϴ�.
	virtual void	Calc_DRI_Merge( void);	// Calculate DRI and Time Line from DTI.
							// It is called on
							//		Data Processing
							//			receiving new TR(add RQ)
							//				CKoscomChartCtrl::OnPacketTRData()
							//				CKoscomChartCtrl::SetPacketDataRQ()
							//			removing TR(RQ)
							//				CPacketListManagerImp::DeleteChartItem()
							//			receiving Real Data
							//				RealDataReceived() ================> Partial Calc_DRI !!!!!
							//			removeing Real Data
							//				CDirectRealReceiverAddInImp::SetRealPacketEndRQ()
							//		Indicator (Obviously Balance Chart or Special Chart)
							//			adding/deleting Glance Balance Chart
							//				AddOEBTRQ()
							//				DelOEBTRQ()
							//			chaning from/to special Chart.
							//				CKoscomChartCtrl::SetOneChart()
							//			recalcualtion Special Chart and Glance Balance Chart Condition.
							//				CKoscomChartCtrl::SetBEnableIndicatorCalculation()
							//				CKoscomChartCtrl::ReCalculateIndicator()
							//				CGraphImp::ChangeGraphDataFromIndicatorInfo()
							//		ETC
							//			SetLeftDataHideCount()
// (2009/1/13 - Seung-Won, Bae) for ReCalc AREA
protected:
	int				m_nNeedCalcArea;			// the Flag on by DRI() with full(2) or partial(1)
	int				m_nDrawCount_Pre;			// the data count on view
	int				m_nDRI_Start_Pre;			// the first data index on view
	BOOL			m_bWeightedVolume_Pre;		// the Weighted Volume option.
	BOOL			m_bReverse_Pre;				// the Reverse X option.
	int				m_nScreenLeft_Pre;			// the graph area left position on view
	int				m_nScreenWidth_Pre;			// the graph area width on view
	double 			m_daItemWidth_Pre;			// the one data width on view

// (2009/1/16 - Seung-Won, Bae) Support the Re-Calculation Mode on OCX OnDrawing
protected:
	int				m_nCalculationMode;			// 0:No-Calc, 1:CalcByReal, 2:CalcAll
	virtual int		GetCalculationMode( void)	{	return m_nCalculationMode;	}
	virtual void	SetNeedCalcAreaFlag( int p_nNeedCalcArea)
						{	if( m_nNeedCalcArea < p_nNeedCalcArea) m_nNeedCalcArea = p_nNeedCalcArea;	}

protected:
	int				m_nDRI_End_Pre;		// the last DRI Index has calculated AREA.

// (2009/2/12 - Seung-Won, Bae) Support default data space.
protected:
	int				m_nDefaultDataSpace;
protected:
	virtual void	SetDefaultDataSpace( int p_nDefaultDataSpace, int p_nMinDataCountOnOnePage);

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
protected:
	int						m_nCountOfBlockColumn;
protected:
	int						m_nColumnWidth[			_MAX_COL];		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
	int						m_naScreenRegionLeft[	_MAX_COL];
	int						m_naScreenRegionWidth[	_MAX_COL];
	double					m_daItemWidth[			_MAX_COL];		// ���� - ��(ITEM)�� ��
protected:
	int						m_nScreenLeft_Cur[		_MAX_COL];		// ���� - ȭ�����
	int						m_nScreenRight_Cur[		_MAX_COL];		// ���� - ȭ�������
	int						m_nScreenLeftiM_Cur[	_MAX_COL];		// ���� - ȭ�����(���� ����)
	int						m_nScreenRightiM_Cur[	_MAX_COL];		// ���� - ȭ�������(���� ����)
	int						m_naScreenStart_Cur[	_MAX_COL];		// ���� - ȭ�����
	int						m_naScreenEnd_Cur[		_MAX_COL];		// ���� - ȭ�鳡
	int						m_nScreenStartiM_Cur[	_MAX_COL];		// ���� - ȭ�����(���� ����)
	int						m_nScreenEndiM_Cur[		_MAX_COL];		// ���� - ȭ�鳡(���� ����)
	int						m_nScreenWidth_Cur[		_MAX_COL];		// ���� - ȭ����(���� ����)
	int						m_naItemWidth[			_MAX_COL];		// ���� - ��(ITEM)�� ��
	CDataBlockList<AREA>	m_blAreafDRI[			_MAX_COL];		// Area Info for Full DRI
	CDataBlockList<AREA>**	m_pblaAreafDTI[			_MAX_COL];		// Area Info for All RQs
protected:
	virtual void					Calc_AREA( int p_nColumnCount);													// �׸��� ���� �ʿ��� ������ �ٽ� ����մϴ�.	
									// It is called on only Drawing. but it is processed by the environment variables.
									//	the Environment Variables
									//		DRI info changing
									//			m_nNeedCalcArea
									//				2 : Calc_DRI()
									//				1 : RealDataReceived() - m_pifaRQ[ nRQ]->nUpdatedRealDRI ====> partial Calc_AREA !!!!!
									//		View Info
									//			m_nDrawCount_Cur
									//			m_nDRI_Start_Cur
									//			m_nScreenLeft_Cur
									//			m_nScreenWidth_Cur
									//		One Data Width
									//			m_daItemWidth
									//		Weighted Volume Info
									//			m_bWeightedVolume
protected:
	void					Calc_AREAofColumn( int p_nColumn);													// �׸��� ���� �ʿ��� ������ �ٽ� ����մϴ�.	
	void					Calc_ItemWidth( int p_nColumn, int nDrawCount, BOOL p_bMoreZoomIn = FALSE);
	void					GetAreaFromDRI_Copy( int p_nColumn, int nDRI, AREA& area);
	void					ItemIntervalApply( int p_nColumn, AREA& area, int nDRI_Count);
	void					ItemIntervalApply_Reverse( int p_nColumn, AREA& area, int nDRI_Count);
protected:
	virtual int						GetColumnWidth( int p_nColumn)							{	return m_nColumnWidth[ p_nColumn];	}
	virtual int						GetDRInFromPt( int nCursor_x, int *pnBlockColumn, PAREA* pparea = NULL, BOOL bIncludeRightMargin = FALSE, BOOL p_bWithBoundCheck = FALSE);
	virtual int						GetGraphXArea_End( int p_nColumn, BOOL bIncludeMargin = FALSE);
	virtual int						GetGraphXArea_Left( int p_nColumn, BOOL bIncludeMargin = FALSE);
	virtual int						GetGraphXArea_Right( int p_nColumn, BOOL bIncludeMargin = FALSE);
	virtual int						GetGraphXArea_Start( int p_nColumn, BOOL bIncludeMargin = FALSE);
	virtual int						GetScaleDRIandTime( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
										const int p_nScaleRegionWidth, const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const int p_nTimeInterval,
										const char *p_szRQ, int &p_nSkipFactor, const CString &p_strPacketType);
	virtual int						GetScreenWidth( int p_nColumn);
	virtual int						GetXFromTime( int p_nColumn, time_t timeFind, PAREA* pparea = NULL, BOOL bIncludeRightMargin = FALSE);	// �������鵵 ���Ե� X���� : xScaleManager - ojtaso (20070608)
	virtual BOOL					GetDTInAREAfRQnPt( int nCursor_x, int& nBlockColumn, int* pnDTI, PAREA parea);	
	virtual BOOL					GetDTInAREAfRQnPt( CString strRQ, int nCursor_x, int& nBlockColumn, int* pnDTI = NULL, PAREA parea = NULL, BOOL p_bWithBoundCheck = FALSE);
	virtual BOOL					IsAllOnePixel( int p_nColumn, const char *p_szRQ);
	virtual BOOL					IsInGraphXArea( int p_nColumn, int nX, BOOL bIncludeEqual = FALSE);
	virtual void					GetAreaFromDRI( int p_nColumn, int nDRI, PAREA* parea);
	virtual void					GetGraphXAreaLR( int p_nColumn, int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE);
	virtual void					GetGraphXAreaSE( int p_nColumn, int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE);
	virtual void					SetColumnWidth( int p_nColumn, int p_nColumnWidth)		{	m_nColumnWidth[ p_nColumn] = p_nColumnWidth;	}
	virtual void					SetScreenRegion( int p_nColumn, int nLeft, int nWidth, BOOL p_bReCalc = FALSE);	// ȭ�� ���� �����մϴ�.
	virtual CDataBlockList< AREA> *	GetDTIArea( int p_nColumn, const char *p_szRQ, int &p_nZeroDTI);	// (2009/1/10 - Seung-Won, Bae) Support Area array of DTI for drawing.
protected:
	time_t					GetTimeFromXWithRightMargin(int nCursor_x, int* pnBlockColumn, PAREA* pparea); // �������鵵 ���Ե� �ð����� : xScaleManager - ojtaso (20070608)
	virtual void			SetBlockColumnCount(int nBlockColumn_Count);						// ����÷��� ������ �����մϴ�.
	virtual void			SetBlockColumn_Cur(int nBlockColumn);								// �ٸ� �Լ�(������, �׷��� ���� �Լ�)���� �⺻���� ������ ����÷��� �����մϴ�.
	virtual BOOL			NextScaleTime(time_t* ptimeScale, PAREA* pparea);					// ���� ������ ��ġ�� �ð� ������ ���մϴ�.
	virtual void			PrevScaleTime(time_t* ptimeScale, PAREA* pparea);					// ���� ������ ��ġ�� �ð� ������ ���մϴ�.
	virtual void			GetAt_ScaleTime(int nDRI, time_t* ptimeScale, PAREA* pparea);
	virtual BOOL			GetAREAfRQnDTI(CString strRQ, int nDTI, int nBlockColumn, PAREA parea);			// �߼������� Pixel���� �ش��ϴ� Data Index�� ���� ������ ���ϰ��� �Ҷ� ����մϴ�.
	virtual BOOL			GetAREAfRQnDTI(int nDTI, int nBlockColumn, PAREA parea);							// �߼������� Ư�� RQ�� DTI�� �ش��ϴ� ���� ������ ���ϰ��� �Ҷ� ����մϴ�.
	virtual BOOL			GetRQInfosFromPt(int nCursor_x, int& nBlockColumn, CStringArray& saRQ, CArray<int, int>& naDTI, PAREA parea);	// ���ڼ��� ���� ������ Pixel���� RQ��, DTI��, DRI_AREA�� ������ ���ϰ��� �Ҷ� ����մϴ�.
	virtual time_t			GetTimeFromX(int nCursor_x, int* pnBlockColumn = NULL, PAREA* pparea = NULL);									
	virtual double			GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, PAREA* pparea, const char *p_szRQ = NULL);	// double������ ��ȯ : xScaleManager - ojtaso (20070402)
	virtual int				GetXFromDouble(const int nRow, const int nCol, double dblFind, PAREA* pparea = NULL);

// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
protected:
	CRect			m_rctClient;
	BOOL			m_bOnResizing;
	BOOL			m_bOnUserZoomChanged;
	double			m_dLastItemWidth;
protected:
	virtual void	OnUserZoomChanged( void);

// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
protected:
	BOOL			m_bScrollByReal;
	BOOL			m_bTruncatedByReal;
protected:
	virtual void	SetFlagForCalcDRIandScrollByReal( BOOL p_bWithTrucated = FALSE);
	virtual BOOL	IsScrollByReal( void)	{	return m_bScrollByReal;	}

// (2009/5/17 - Seung-Won, Bae) for Future Time Count of Compare Chart with OEBT.
protected:
	int	 m_nFutureTimeCount;				// �̷����� Ȯ��
protected:
	virtual int		GetFutureTimeCount();
	virtual void	SetFutureTimeCount(int nFutureTimeCount);									// �̷����� Ȯ�� ���
	virtual void	SetRightMarginAndFutureTime( int p_nRightMarginWidth, int p_nFutureTimeCount);		// (2009/5/17 - Seung-Won, Bae) for Right Margin and Future Time

// (2009/5/28 - Seung-Won, Bae) Check! Is Data Empty?
protected:
	virtual BOOL	IsEmptyData( void);

// (2009/6/4 - Seung-Won, Bae) for ceiled Minute Scale with second time data.
protected:
	BOOL	m_bCeiling;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;


// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
protected:
	friend class CXScaleTimeDiffManager;
	CXScaleTimeDiffManager	m_TimeDiffManager;
	bool					m_bTimeScale;
protected:
	int						GetTimeDiffWithCurrentRQ( const CString p_strRQ);

// (2009/1/19 - Seung-Won, Bae) for Scale Time List
protected:
	CTypedPtrMap< CMapWordToPtr, signed short, CXScaleGridTime *>	m_mapXGridTime;

	int									m_naScaleType[ _MAX_COL];
	CDataBlockList< tm>					m_blScaleTime[ _MAX_COL];		// Time value for scale of YYYY/MM/DD/HH/mm/SS
	CDataBlockList< int>				m_blScaleDRI[ _MAX_COL];		// DRI value for scale of YYYY/MM/DD/HH/mm/SS
protected:	// Real X Scale Data.
	int									m_nSkipFactor;
	CScaleBaseData::HORZSCALEDRAWERTYPE	m_eScaleDrawType;
	CString								m_strScaleRQ;
protected:
	inline void				PopLastScaleDataTime( void);
	inline void				PushScaleDataTime( int p_nDRI, time_t p_timeDRI, BOOL p_bEndDRI);
protected:
	virtual BOOL			IsSpecialTime( const char *p_szRQ, time_t p_time, long &p_lSpecialTimeCode);

	int		m_nMinMaxRatio;
	void	SetPriceMinMaxShowType( int nMinMaxRatio) { m_nMinMaxRatio = nMinMaxRatio; }
	int		GetPriceMinMaxShowType( void)		{	return m_nMinMaxRatio;	}

public:
	//KHD : �ð� ���� 
	CDataBlockList< tm>					m_blScaleTime2[ _MAX_COL];		// Time value for scale of YYYY/MM/DD/HH/mm/SS
	CDataBlockList< int>				m_blScaleDRI2[ _MAX_COL];		// DRI value for scale of YYYY/MM/DD/HH/mm/SS

	virtual int		GetScaleDRIandTimeAllData( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
											const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType,	const char *p_szRQ, int &p_nSkipFactor);

};


/*
	# ����ġ �ŷ��� ó���� ���� ���� #

	1. ���� �����ϰ� �ִ� RQ�� ���� ���� Scale�� �������� ����ġ�� ����Ѵ�.(������ ��� �켱 �˻��Ȱ�)
	2. ���� �������� ����(DRI����)���� 1���� ������ DRI ������ ���ؼ��� ����ġ�� ����Ͽ� ������ �����ϰ�
	   ������ DRI ������ �⺻������ cx/(DRI ����)�� width�� �����ϵ��� �Ѵ�.
	3. �߼������� RQ+DTI������ �������� �ؼ� �������� ���� ������ ���ؼ��� cx/(DRI ����) ������� x�� ��ǥ�� ���ؼ� �Ѱ��ֵ��� �մϴ�.
	4. �����Ǿ������� �߼������� ������ �ִ� ������ ������Ʈ �Ǿ�� �Ѵ�.
	   �ؼ� �̰��� ������Ʈ �ϵ��� �ϴ� �۾��� �߼����� �ϵ��� "RQ�� �̵��ؾ� �ϴ� DTI�� ������" �Ѱܼ� ������ ������Ʈ �Ҽ� �ֵ��� �Ѵ�.
*/

/*
	# ��� �÷� ó�� #

	1. ��� �÷��� 2�� �̻��̸� ������ ���� ����� �������� �ʽ��ϴ�.
	   �� ��� ���� ���� ���� ������� �����ϰ� �˴ϴ�.
*/