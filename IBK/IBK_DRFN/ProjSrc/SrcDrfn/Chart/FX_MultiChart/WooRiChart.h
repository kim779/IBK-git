#if !defined(AFX_WOORICHART_H__78393149_A4E3_4CE8_9EB8_A39F6BF0FC78__INCLUDED_)
#define AFX_WOORICHART_H__78393149_A4E3_4CE8_9EB8_A39F6BF0FC78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CWooRiChart wrapper class

#include <afxconv.h>

class CWooRiChart : public CWnd
{
protected:
	DECLARE_DYNCREATE(CWooRiChart)
public:
	// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
	static long CLSIDFromProgID( const char *p_szProgID, LPCLSID p_pClsID)
	{
		USES_CONVERSION;
		LPCOLESTR szOleProgID = T2COLE( p_szProgID);
		HRESULT hResult = ::CLSIDFromProgID( szOleProgID, p_pClsID);
		if( hResult == S_OK) return 0;
		else if( hResult == CO_E_CLASSSTRING) return -1;
		else if( hResult == REGDB_E_WRITEREGDB) return -2;
		return -3;
	}
	CLSID const& GetClsid( CString strDataPath)
	{
		static CLSID clsid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0} };
		if( clsid.Data1 == 0)
		{
			char szProgID[ 256];
			DWORD dwError = GetPrivateProfileString( "Chart Version", "ProgID", "Prog ID Error", szProgID, 255, strDataPath + "ChartMetaTable.dat");
			long lResult = CLSIDFromProgID( szProgID, &clsid);
			if( lResult < 0) AfxMessageBox( CString( szProgID) + "'s CLSID is not found!");
		}
		return clsid;
	}

	CWooRiChart();
	virtual ~CWooRiChart();

	// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
    BOOL Create( CString strDataPath, LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL);
/*
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x8e56384b, 0x1b29, 0x4768, { 0xaf, 0x1c, 0x14, 0x16, 0x3c, 0x75, 0x92, 0xc1 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }
*/

// Operations
public:
	typedef enum {
		IIx_SetAllProperties2,
		IIx_SetBaseDesk,
		IIx_GetBase,
		IIx_AddGraph,
		IIx_AddPacket,
		IIx_RemovePacket,
		IIx_GetIndicatorNameList,
		IIx_SetContainerDllInfo,
		IIx_SetChartData,
		IIx_SetPacketData,
		IIx_AddChartBlock,
		IIx_ResetChart,
		IIx_SetBackgroundColors,
		IIx_SetDrdsAdvise,
		IIx_AddRealItem,
		IIx_ShowIndicatorConfigDlg,
		IIx_NOnePageDataCount,
		IIx_StrStartDateOfData,
		IIx_ResetChartCfgWith,
		IIx_EmptyChart,
		IIx_HasGraph,
		IIx_RemoveRealItem,
		IIx_SaveandOpenFromFile,
		IIx_ClearChartData,
		IIx_ShowInnerToolBar,
		IIx_ChangeVertScalePosition,
		IIx_GetRowBlockCount,
		IIx_GetColumnBlockCount,
		IIx_ChangeHorzScalePosition,
		IIx_RemoveChart,
		IIx_SetStartPeriodOfPacket,
		IIx_GetStartPeriodOfPacket,
		IIx_StrUserIndicatorListFileName,
		IIx_StrUserEnvironmentFileName,
		IIx_GetStartEndDateOnView,
		IIx_SetStartEndDateOfView,
		IIx_ScrollToDataOfEnd,
		IIx_ZoomToWithRightFix,
		IIx_ClearTimeMark,
		IIx_ShowEnvironmentDlg,
		IIx_SetTimeMark,
		IIx_BOcxAreaNumericalInquiry,
		IIx_OnRunningOneChart,
		IIx_EnableBoundMark,
		IIx_EnableReceivingTimeShare,
		IIx_ChangePacketType,
		IIx_ChangePriceGraphDrawingData,
		IIx_NUseInquiryWithCrossline,
		IIx_BShowPriceRateInquiry,
		IIx_BNormalizedMinuteTimeRule,
		IIx_HasGraphInSpecialBack,
		IIx_InvokeAddIn,
		IIx_LoadAddIn,
		IIx_SetDragModeType,
		IIx_SetHtsPath,
		IIx_SetAllLogScale,
		IIx_GetListViewerData,
		IIx_SetDataIndexByDate,
		IIx_SetRunTimeMode,				//2005. 07. 15 이후 추가 
		IIx_GetPacketOption,
		IIx_SetPacketOption,
		IIx_SetOutsideCommonInterface,
		IIx_RemoveGraph_Title,
		IIx_GetGraphPacketName,
		IIx_ChangeGraphTitle2,
		IIx_PartitionAllGraph,
		IIx_JoinAllGraph,
		IIx_SetBSelectTitle,
		IIx_ChangeAllFunctionName,
		IIx_ChangeIndicatorPacketName,
		IIx_RunEnvironmentDlg,
		IIx_SetIndicatorCondition,
		IIx_SetLogScale,
		IIx_GetLogScale,
		IIx_SetHelpMessageRealDataRate,
		IIx_InvokeAddInStr,
		IIx_SetGraphHide3,
		IIx_SetOnePageDataCount2,
		IIx_SetPacketDataInAdditionPacket,
		IIx_SetOnePageDataCount,
		IIx_SetPacketHelpMessage,
		IIx_SetRealData,
		IIx_PrintChatImage,
		IIx_ExportToExcel,
		IIx_LoadAddInWithOverExclusive,
		IIx_GetAllProperties3,
		IIx_SetAllProperties3,
		IIx_GetAddInCustomInterface,
		IIx_ShowIndicatorConfigDlgWithName,
		IIx_BEnableIndicatorCalculation,
		IIx_StrChartList,
		IIx_BWholeView,
		IIx_EmptyChart2,
		IIx_AddChartItem,		// 복수종목에 사용되는 인터페이스 : 복수종목 - ojtaso (20070206)
		IIx_AddPairItem,		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		IIx_SetRealDataCode,	// 복수종목에 사용되는 인터페이스 : 복수종목 - ojtaso (20070206)
		IIx_AddPacketRQ,		// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
		IIx_SetPacketDataRQ,	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
		IIx_AddChartItemRQ,		// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
		IIx_SetCurrentRQ,		// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
		IIx_SetResetTotalCountWhileDraw,		// xScaleManager : xScaleManager - ojtaso (20070529)
		IIx_SetMultiItemMode,	//복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
		IIx_ChangeGraphDrawingData,
		IIx_ChangeGraphTitle,
		IIx_AddChartAllBlock,		// 모든 RQ의 그래프의 차트 변경 - ojtaso (20080516)
		IIx_ChangeGraphPropertyAtOnce,
		IIx_ShowGraphDataDrawingData,
		IIx_ShowSelEnvironmentDlg,
		IIx_GetVertScaleGroupCount,
		IIx_NLanguageID,
		IIx_bFreezedPageState,		// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
		IIx_strPagePathWithPrefix,
		IIx_HasIndicatorWithRQ,		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		IIx_RemoveAllGraphOfRQ,
		IIx_GetRowOfGraphWithRQ,
		IIx_SetGraphColorWithRQ,
		IIx_SaveandOpenFromAnalToolFile,
		IIx_SetItemInfo,

		IIx_InterfaceCount,		// 카운트를 위한 요소 (항상 마지막에 위치토록)
	};
	// 가로 scale 위치
	enum HORZSCALEPOSITION
	{
		HORZ_TOP,
		HORZ_BOTTOM,
		HORZ_BOTH,
		HORZ_HIDE,
		HORZ_NONE
	};
	
	// 세로 scale 위치
	enum VERTSCALEPOSITION
	{
		VERT_LEFT,
		VERT_RIGHT,
		VERT_BOTH,
		VERT_NONE
	};

	long GetNNumericalInquiryOption();
	void SetNNumericalInquiryOption(long);
	CString GetStrGlobalSharedList();
	void SetStrGlobalSharedList(LPCTSTR);
	void SetStrStartDateOfData(LPCTSTR);
	
	void SetOnRunningOneChart(BOOL);
	BOOL GetBDoesUseSkinColor();
	void SetBDoesUseSkinColor(BOOL);

// Attributes
public:
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	OLE_HANDLE GetHWnd();
	void SetHWnd(OLE_HANDLE);

// Operations
public:
	BOOL SetAllProperties2(long dwData);
	void SetBaseDesk(long lKey);
	long GetBase(LPCTSTR szKey);
	void AddGraph(short nRow, short nColumn, short nVertScaleGroup, LPCTSTR szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);
	void AddPacket(LPCTSTR szPacketOption);
	void RemovePacket(LPCTSTR szPacketName);
	CString GetIndicatorNameList(LPCTSTR p_strGroup);
	void SetContainerDllInfo(long p_lDllWnd, short p_nID);
	short SetChartData(long pChartData, short nCount, short nType);
	BOOL SetPacketData(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset);
	BOOL AddChartBlock(LPCTSTR strChartName);
	BOOL ResetChart();
	void SetBackgroundColors(unsigned long p_rgbBlockArea, unsigned long p_rgbScaleArea);
	BOOL SetDrdsAdvise(BOOL bOnAdvise);
	void AddRealItem(LPCTSTR szRealInfo);
	void ShowIndicatorConfigDlg();
	long GetNOnePageDataCount();
	void SetNOnePageDataCount(long propVal);
	CString GetStrStartDateOfData();
	void ResetChartCfgWith(short p_nIndex);
	BOOL EmptyChart(BOOL bIsDeleteAllPacket, BOOL bIsDefault);	// JSR 20060119 Modify (From ResetOCX())
	BOOL HasGraph(LPCTSTR p_szGraphName);
	void RemoveRealItem(LPCTSTR szRealInfo);
	BOOL SaveandOpenFromFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	short ClearChartData(short nOption);
	void ShowInnerToolBar(BOOL p_bShow);
	BOOL ChangeVertScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex, short nVSGroupIndex);
	short GetRowBlockCount();
	short GetColumnBlockCount();
	BOOL ChangeHorzScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex);
	BOOL RemoveChart(LPCTSTR strChartName);
	BOOL SetStartPeriodOfPacket(LPCTSTR p_szPacketName, long p_lStartPeriod, long p_bAddSpace);
	long GetStartPeriodOfPacket(LPCTSTR p_szPacketName);
	void SetStrUserIndicatorListFileName(LPCTSTR propVal);
	void SetStrUserEnvironmentFileName(LPCTSTR propVal);
	BOOL GetStartEndDateOnView(double* p_pStartDate, double* p_pEndDate);
	BOOL SetStartEndDateOfView(double p_dStartDate, double p_dEndDate);
	BOOL ScrollToDataOfEnd(double p_dData);
	short ZoomToWithRightFix(short p_nZoomValue, BOOL p_bWithRightFix);
	void ClearTimeMark();
	void ShowEnvironmentDlg();
	void SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll);
	void SetBOcxAreaNumericalInquiry(BOOL propVal);
	BOOL GetOnRunningOneChart();
	void EnableBoundMark(BOOL p_bEnable);
	void EnableReceivingTimeShare(BOOL p_bEnable);
	BOOL ChangePacketType(LPCTSTR p_szPacketName, LPCTSTR p_szPacketType);  
	BOOL ChangePriceGraphDrawingData(BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData, short p_nDefaultType);
	void SetNUseInquiryWithCrossline(short propVal);
	short GetNUseInquiryWithCrossline();
	BOOL GetBShowPriceRateInquiry();
	void SetBShowPriceRateInquiry(BOOL propVal);
	BOOL GetBNormalizedMinuteTimeRule();
	void SetBNormalizedMinuteTimeRule(BOOL);
	BOOL HasGraphInSpecialBack(LPCTSTR p_szGraphName);
	BOOL InvokeAddIn(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData);
	BOOL LoadAddIn(LPCTSTR p_szAddInDllName);
	void SetDragModeType(short p_nDragModeType);
	void SetHtsPath(LPCTSTR pcszHtsPath);	
	void SetAllLogScale(BOOL bLogScale);

	//2005. 06. 28 add ===================================================================
	//데이터 리스트 뷰에 보일 값을 조회한다.
	void GetListViewerData(	BSTR* bstrTitle,
							BSTR* bstrHorzScale,
							BSTR* bstrPacketNamesPerBlock,
							BSTR* bstrPacketNames, 
							BSTR* bstrSaveFilePath,
							BOOL* bViewPacketSequence,
							long* lpPacketList); 
	//chart에 보이는 범위를 해당 매개변수의 값으로 셋한다. YYYYMMDDHHMMSS 형태로 넘긴다.
	void SetDataIndexByDate(LPCTSTR pcszStartDate, LPCTSTR pcszEndDate); 

	//2005. 07. 15 추가 =========================================================================		
	void	SetRunTimeMode(BOOL bRunTimeMode);
	CString GetPacketOption();
	void	SetPacketOption(LPCTSTR strPacketOption);
	void	SetOutsideCommonInterface(long lpOutsideCommonInterface);
	//===========================================================================================
	void	SetBSelectTitle(BOOL);


	//2005. 07. 19 추가 =========================================================================	
	BOOL	RemoveGraph_Title(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle, BOOL bIsOnlyNoTRData, BOOL bIsAutoHorzScalePosition);


	CString GetGraphPacketName(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle);
	BOOL	ChangeGraphTitle2(LPCTSTR szTargetTitle, LPCTSTR szSourceTitle);
	BOOL	PartitionAllGraph();
	BOOL	JoinAllGraph(BOOL bIsOneVertScaleGroup);
	//===========================================================================================


	//2005. 07. 27 추가 =========================================================================
	// 비교차트 분할비교<->중첩비교<->누적등락률 중첩비교 간 전환을 알리는 Method
	BOOL ChangeAllFunctionName(LPCTSTR szTargetFunctionName, LPCTSTR szSourceFunctionName, BOOL bKeepOldGraphColor);
	//===========================================================================================

	//2005. 08. 11 추가 =========================================================================	
	BOOL ChangeIndicatorPacketName(LPCTSTR strIndicatorName, short nSubGraphIndex, LPCTSTR strNewPacketName);
	void RunEnvironmentDlg(short nOption);// 환경 설정 창 띄우는 
	BOOL SetIndicatorCondition(LPCTSTR strIndicatorName, short nConditionType, short nConditionIndex, short dValue, BOOL bReCalculate);
	//===========================================================================================
	


	//2005. 08. 23 추가 =========================================================================	
	BOOL SetLogScale(LPCTSTR strIndicatorName, BOOL bLogScale);
	BOOL GetLogScale(LPCTSTR strIndicatorName);	
	//===========================================================================================


	//2005. 09. 06 추가 =========================================================================
	//비교차트에 리얼 처리 방식이 바뀌어서 호출되는 메소드 
	// 나누기 100을 하라는 의미
	BOOL SetHelpMessageRealDataRate(LPCTSTR strHelpMessageData, BOOL bIsRemove);
	//===========================================================================================

	//2005. 09. 08 추가 =========================================================================
	BOOL InvokeAddInStr(LPCTSTR p_szAddInDllName, LPCTSTR p_szCommandName, LPCTSTR p_szData);
	//===========================================================================================

	//2005. 09. 14 추가 =========================================================================
	//비교차트에서 체크박스로 종목 해제시 화면에만 안 보이도록 처리.
	BOOL SetGraphHide3(LPCTSTR strTitle, BOOL bIsHide);
	//===========================================================================================


	//2005. 11. 02 추가 =========================================================================
	//주 차트에서 차트 여백을 포함한 형태(bIncludeRightMargin로 판단)로 보이게 하기위해..
	void SetOnePageDataCount2(long nOnePageDataCount, BOOL bIncludeRightMargin);
	//===========================================================================================

	//2005. 11. 09 추가 =========================================================================
	//시장 지표 TR을 차트에 보내기 위해 추가 
	BOOL SetPacketDataInAdditionPacket(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset);
	//===========================================================================================

	//2005. 11. 09 추가 =========================================================================
	//페이지 데이터 카운트 수를 변경시킨다.
	//기존 SetNOnePageDataCount를 SetOnePageDataCount로 적용
	void SetOnePageDataCount(long nOnePageDataCount);
	//===========================================================================================


	//2005. 12. 06 추가 =========================================================================
	//복기 리얼 처리 
	BOOL SetPacketHelpMessage(LPCTSTR strHelpMsg, BOOL bIsAllRemove);
	void SetRealData(LPCTSTR strPacketName, double dData, short nOption);	
	//===========================================================================================
	//2006. 04. 05 추가 =========================================================================
	void PrintChatImage();
	void ExportToExcel();
	//===========================================================================================

	//===========================================================================================
	// 2006. 08. 24 추가
	BOOL LoadAddInWithOverExclusive(LPCTSTR p_szAddInName);
	long GetAllProperties3(long p_lIPropertyMap);
	long SetAllProperties3(long p_lIPropertyMap);
	LPUNKNOWN GetAddInCustomInterface(LPCTSTR p_szAddInName);
	//===========================================================================================

	//===========================================================================================
	// 2006. 10. 27 추가
	BOOL ShowIndicatorConfigDlgWithName(LPCTSTR p_szIndicatorName);
	//===========================================================================================

	//===========================================================================================
	// 2006. 12. 12 추가
	BOOL GetBEnableIndicatorCalculation();
	void SetBEnableIndicatorCalculation(BOOL propVal);
	CString GetStrChartList();
	void SetStrChartList(LPCTSTR);
	BOOL GetBWholeView();
	void SetBWholeView(BOOL propVal);
	//===========================================================================================
	BOOL EmptyChart2(LPCTSTR p_szChartList, BOOL p_bIsDeleteAllPacket, BOOL p_bIsDefault);
	// 복수종목에 사용되는 인터페이스 : 복수종목 - ojtaso (20070206)
	CString AddChartItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, BOOL bBuiltChart);
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	CString AddPairItem(LPCTSTR p_szItemCode, LPCTSTR p_szItemName, BOOL p_bBuildChart);
	void SetRealDataCode(LPCTSTR strCode, LPCTSTR strPacketName, double dData, short nOption);

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	void AddPacketRQ(LPCTSTR lpszRQ, LPCTSTR lpszPacketOption);
	BOOL SetPacketDataRQ(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, long nData, long nLength, BOOL bReset);
	BOOL AddChartItemRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName);
	void SetCurrentRQ(LPCTSTR lpszRQ);
	// xScaleManager : xScaleManager - ojtaso (20070529)
	void SetResetTotalCountWhileDraw(BOOL bReset);
	// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
	void SetMultiItemMode(BOOL bMultiItem);

	// (2008/2/13 - Seung-Won, Bae) for MultiLanguage.
	short GetNLanguageID();
	void SetNLanguageID(short);

	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	void SetBFreezedPageState(BOOL);
	CString GetStrPagePathWithPrefix();
	void SetStrPagePathWithPrefix(LPCTSTR);

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	BOOL HasIndicatorWithRQ(LPCTSTR p_szIndicatorName, LPCTSTR p_szRQ);
	void RemoveAllGraphOfRQ(LPCTSTR p_szRQ);
	short GetRowOfGraphWithRQ(LPCTSTR p_szIndicatorName, LPCTSTR p_szRQ);
	BOOL SetGraphColorWithRQ(LPCTSTR p_szRQ, LPCTSTR p_szIndicatorName, LPCTSTR p_szColor);


	BOOL SaveandOpenFromAnalToolFile(LPCTSTR lpszOpenFileName, LPCTSTR lpszSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	void SetItemInfo(LPCTSTR lpstrCode, LPCTSTR lpstrName);
	
	// (2008/3/3 - Seung-Won, Bae) to avoid the old version loading.
protected:
	static DISPID		m_pdID[ IIx_InterfaceCount];
	static OLECHAR *	m_osInterfaceName[ IIx_InterfaceCount];
/*
	BOOL GetBNormalizedMinuteTimeRule();
	void SetBNormalizedMinuteTimeRule(BOOL);
	long GetNBlockType();
	void SetNBlockType(long);
	long GetNBlockCompart();
	void SetNBlockCompart(long);
	long GetNBlockRowCount();
	void SetNBlockRowCount(long);
	long GetNBlockColumnCount();
	void SetNBlockColumnCount(long);
	long GetNBlockGap();
	void SetNBlockGap(long);
	long GetNBlockBorderThickness();
	void SetNBlockBorderThickness(long);
	CString GetStrAddInLoadList();
	void SetStrAddInLoadList(LPCTSTR);
	long GetNBorderType();
	void SetNBorderType(long);
	long GetNScaleLineType();
	void SetNScaleLineType(long);
	BOOL GetBWholeView();
	void SetBWholeView(BOOL);
	long GetNOnePageDataCount();
	void SetNOnePageDataCount(long);
	long GetNUserBlockAddCount();
	void SetNUserBlockAddCount(long);
	BOOL GetBGraphDragDrop();
	void SetBGraphDragDrop(BOOL);
	long GetNLeftMargin();
	void SetNLeftMargin(long);
	long GetNRightMargin();
	void SetNRightMargin(long);
	long GetNTopMargin();
	void SetNTopMargin(long);
	long GetNBottomMargin();
	void SetNBottomMargin(long);
	BOOL GetBAddGraphMenuShow();
	void SetBAddGraphMenuShow(BOOL);
	CString GetStrUserEnvironmentFileName();
	void SetStrUserEnvironmentFileName(LPCTSTR);
	CString GetStrUserIndicatorListFileName();
	void SetStrUserIndicatorListFileName(LPCTSTR);
	CString GetStrUserSeriesListFileName();
	void SetStrUserSeriesListFileName(LPCTSTR);
	BOOL GetBHide();
	void SetBHide(BOOL);
	BOOL GetBIsInput();
	void SetBIsInput(BOOL);
	CString GetStrToolbarOption();
	void SetStrToolbarOption(LPCTSTR);
	CString GetStrPacketOption();
	void SetStrPacketOption(LPCTSTR);
	CString GetStrLocalSharedList();
	void SetStrLocalSharedList(LPCTSTR);
	BOOL GetBUseRealData();
	void SetBUseRealData(BOOL);
	CString GetStrRealList();
	void SetStrRealList(LPCTSTR);
	short GetNMarginBorderType();
	void SetNMarginBorderType(short);
	long GetNRowDoubleSizeBlock();
	void SetNRowDoubleSizeBlock(long);
	BOOL GetBUseInquiryDlgWhenUseCrossLine();
	void SetBUseInquiryDlgWhenUseCrossLine(BOOL);
	long GetNColumnDoubleSizeBlock();
	void SetNColumnDoubleSizeBlock(long);
	BOOL GetBUseShortKey();
	void SetBUseShortKey(BOOL);
	BOOL GetBUseConditiontoTitle();
	void SetBUseConditiontoTitle(BOOL);
	BOOL GetBUseDatatoTitle();
	void SetBUseDatatoTitle(BOOL);
	short GetNHorzGap();
	void SetNHorzGap(short);
	short GetNVertGap();
	void SetNVertGap(short);
	short GetNRButtonMenuType();
	void SetNRButtonMenuType(short);
	BOOL GetBShowWarningMessageBox();
	void SetBShowWarningMessageBox(BOOL);
	BOOL GetBControlBlockSize();
	void SetBControlBlockSize(BOOL);
	BOOL GetBBlockMoveDelete();
	void SetBBlockMoveDelete(BOOL);
	BOOL GetBSelectTitle();
	void SetBSelectTitle(BOOL);
	BOOL GetBSelectGraph();
	void SetBSelectGraph(BOOL);
	BOOL GetBUseScaleRButtonMenu();
	void SetBUseScaleRButtonMenu(BOOL);
	short GetNGraphRButtonType();
	void SetNGraphRButtonType(short);
	short GetNHoldingPacketSize();
	void SetNHoldingPacketSize(short);
	CString GetSzBasePacketName();
	void SetSzBasePacketName(LPCTSTR);
	BOOL GetBRedrawAfterUpdatedAllPackets();
	void SetBRedrawAfterUpdatedAllPackets(BOOL);
	short GetNMinDataCountOnOnePage();
	void SetNMinDataCountOnOnePage(short);
	BOOL GetBFireRealEvent();
	void SetBFireRealEvent(BOOL);
	short GetNInquiryType();
	void SetNInquiryType(short);
	BOOL GetBOcxAreaNumericalInquiry();
	void SetBOcxAreaNumericalInquiry(BOOL);
	long GetNNumericalInquiryOption();
	void SetNNumericalInquiryOption(long);
	CString GetStrGlobalSharedList();
	void SetStrGlobalSharedList(LPCTSTR);
	short GetNUseInquiryWithCrossline();
	void SetNUseInquiryWithCrossline(short);
	BOOL GetBShowPriceRateInquiry();
	void SetBShowPriceRateInquiry(BOOL);
	BOOL GetBDoesUseSkinColor();
	void SetBDoesUseSkinColor(BOOL);
	CString GetStrMapName();
	void SetStrMapName(LPCTSTR);
	short GetNBongCountMargin();
	void SetNBongCountMargin(short);
	BOOL GetBOnePageDataCountInTRCount();
	void SetBOnePageDataCountInTRCount(BOOL);
	BOOL GetBIsMapDefault();
	void SetBIsMapDefault(BOOL);
	short GetNPacketCountDataSize();
	void SetNPacketCountDataSize(short);
	short GetNLanguageID();
	void SetNLanguageID(short);
	BOOL GetBFreezedPageState();
	void SetBFreezedPageState(BOOL);
	BOOL GetIsShowInneerToolBar();
	void SetIsShowInneerToolBar(BOOL);
	CString GetStrChartList();
	void SetStrChartList(LPCTSTR);
	unsigned long GetClrBackground();
	void SetClrBackground(unsigned long);
	unsigned long GetClrBackBorder();
	void SetClrBackBorder(unsigned long);
	unsigned long GetClrBlock();
	void SetClrBlock(unsigned long);
	unsigned long GetClrBlockBorder();
	void SetClrBlockBorder(unsigned long);
	unsigned long GetClrScaleLine();
	void SetClrScaleLine(unsigned long);
	unsigned long GetClrScaleText();
	void SetClrScaleText(unsigned long);
	unsigned long GetClrGraphBase1();
	void SetClrGraphBase1(unsigned long);
	unsigned long GetClrGraphBase2();
	void SetClrGraphBase2(unsigned long);
	unsigned long GetClrGraphBase3();
	void SetClrGraphBase3(unsigned long);
	unsigned long GetClrGraphBase4();
	void SetClrGraphBase4(unsigned long);
	unsigned long GetClrGraphBase5();
	void SetClrGraphBase5(unsigned long);
	unsigned long GetClrGraphBase6();
	void SetClrGraphBase6(unsigned long);
	unsigned long GetClrAnalysisTool();
	void SetClrAnalysisTool(unsigned long);
	CString GetStrBlockRegion();
	void SetStrBlockRegion(LPCTSTR);
	unsigned long GetClrScaleLine2();
	void SetClrScaleLine2(unsigned long);
	unsigned long GetClrInquiryWindow();
	void SetClrInquiryWindow(unsigned long);
	CString GetCustomFont();
	void SetCustomFont(LPCTSTR);
	unsigned long GetClrLattice();
	void SetClrLattice(unsigned long);
	CString GetStrStartDateOfData();
	void SetStrStartDateOfData(LPCTSTR);
	BOOL GetOnRunningOneChart();
	void SetOnRunningOneChart(BOOL);
	BOOL GetEnableHorizontalLineInNIDLG();
	void SetEnableHorizontalLineInNIDLG(BOOL);
	BOOL GetBShowPriceMinMaxText();
	void SetBShowPriceMinMaxText(BOOL);
	BOOL GetBHideAllTitle();
	void SetBHideAllTitle(BOOL);
	CString GetStrMapIndicatorInfo();
	void SetStrMapIndicatorInfo(LPCTSTR);
	BOOL GetBEnableIndicatorCalculation();
	void SetBEnableIndicatorCalculation(BOOL);
	CString GetStrPagePathWithPrefix();
	void SetStrPagePathWithPrefix(LPCTSTR);

// Operations
public:
	BOOL SetAllProperties2(long dwData);
	void SetBaseDesk(long lKey);
	long GetBase(LPCTSTR szKey);
	short SetChartData(long pChartData, short nCount, short nType);
	short SetChartDataStr(LPCTSTR szChartData, short nCount, short nType);
	short testGetPacketdatasize(LPCTSTR pIn);
	void testLoadAnalysisTools(short nToolOption, short nOption);
	void MakeSaveShortKey(LPCTSTR szFileName, short nFlag);
	short ChangeChart(short nBlockRowPosition, short nBlockColumnPosition, LPCTSTR strGraphTitle, LPCTSTR strGraphName, LPCTSTR strPacketName, LPCTSTR strGraphType, LPCTSTR strGraphStyle, short nLineWidth, unsigned long clrLineColor);
	short ClearChartData(short nOption);
	short SetGraphHide(short nRow, short nColumn, LPCTSTR strTitle, BOOL bHide);
	BOOL AddChartBlock(LPCTSTR strChartName);
	void SetOnePageDataCount(long nOnePageDataCount);
	long GetOnePageDataCount();
	BOOL SaveandOpenFromFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	BOOL JoinandDevisionforCompareChart(long nRowCount, long nColumnCount, BOOL bJoin);
	BOOL ChangeGraphTitle(short nRowIndex, short nColumnIndex, short nGraphIndex, LPCTSTR strNewTitle);
	short SetChartDataFromFile(LPCTSTR szFileName, short nType);
	BOOL RemoveChart(LPCTSTR strChartName);
	BOOL ChangeGraphType(LPCTSTR strGraphName, LPCTSTR strGraphType, LPCTSTR strGraphStyle, LPCTSTR strPacketNameList, BOOL bRunatOnce);
	void SetChartAllInfo();
	CString GetData(LPCTSTR strPacketName, short nDataIndex);
	BOOL SetDrdsAdvise(BOOL bOnAdvise);
	short GetDataCount();
	BOOL ChangeChartItem(LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, short nNewUpLineWidth, short nNewDownLineWidth, short nNewEqualLineWidth, 
		BOOL bRunatOnce);
	BOOL SetStopLossPrice(BOOL bIsStopLoss, double dStopLossPrice, BOOL bIsProfit, double dProfitPrice);
	BOOL RestoreMinMax(LPCTSTR strPacketName);
	BOOL RestoreMin(LPCTSTR strPacketName);
	BOOL RestorMax(LPCTSTR strPacketName);
	BOOL SetMin(LPCTSTR strPacketName, double dMin);
	BOOL SetMax(LPCTSTR strPacketName, double dMax);
	BOOL SetMinMax(LPCTSTR strPacketName, double dMin, double dMax);
	BOOL SetHorzScaleData(short nRowIndex, short nColumnIndex, LPCTSTR strScaleData);
	double GetMin(LPCTSTR strPacketName);
	double GetMax(LPCTSTR strPacketName);
	BOOL SetGraphHide2(short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, short nSubGraphIndex, BOOL bHide);
	BOOL ChangeGraphDatas(short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, LPCTSTR strNewGraphName, LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, 
		short nNewUpLineWidth, short nNewDownLineWidth, short nNewEqualLineWidth, BOOL bRunatOnce);
	void SaveandApplyGraphStyle(short nSaveDataType, short nChangeDataType);
	void SetDataRegionInBlock(short nDataStartIndex);
	void SetItemInfo(LPCTSTR lpstrCode, LPCTSTR lpstrName);
	void AddandDeleteIndicator(LPCTSTR strGraphName);
	long GetAllProperties2(long lpData);
	long GetBasePropIf();
	BOOL SetVertScaleData(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, LPCTSTR strVertScaleData);
	BOOL ChangeBlockType(short nBlockType, short nRowCount, short nColumnCount);
	BOOL ChangeHorzScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex);
	BOOL ResetChart();
	void SetRealData(LPCTSTR strPacketName, double dData, short nOption);
	BOOL SetGraphTitle(LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewGraphTitle, BOOL bGraphShowing);
	CString GetIndicatorNameList(LPCTSTR p_strGroup);
	void AddGraph(short nRow, short nColumn, short nVertScaleGroup, LPCTSTR szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);
	void AddPacket(LPCTSTR szPacketOption);
	long RemovePacket(LPCTSTR szPacketName);
	void SetContainerDllInfo(long p_lDllWnd, short p_nID);
	BOOL SetPacketData(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset);
	void SetBackgroundColors(unsigned long p_rgbBlockArea, unsigned long p_rgbScaleArea);
	BOOL AddRealItem(LPCTSTR szRealInfo);
	void ShowIndicatorConfigDlg();
	void ResetChartCfgWith(short p_nIndex);
	BOOL EmptyChart(BOOL bIsDeleteAllPacket, BOOL bIsDefault);
	BOOL HasGraph(LPCTSTR p_szGraphName);
	void RemoveRealItem(LPCTSTR szRealInfo);
	void ShowInnerToolBar(BOOL p_bShow);
	BOOL ChangeVertScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex, short nVSGroupIndex);
	short GetRowBlockCount();
	short GetColumnBlockCount();
	BOOL SetStartPeriodOfPacket(LPCTSTR p_szPacketName, long p_lStartPeriod, long p_bAddSpace);
	long GetStartPeriodOfPacket(LPCTSTR p_szPacketName);
	BOOL GetStartEndDateOnView(double* p_pStartDate, double* p_pEndDate);
	BOOL SetStartEndDateOfView(double p_dStartDate, double p_dEndDate);
	BOOL ScrollToDataOfEnd(double p_dData);
	short ZoomToWithRightFix(short p_nZoomValue, BOOL p_bWithRightFix);
	void ClearTimeMark();
	void ShowEnvironmentDlg();
	void SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll);
	short GetVertScalePosition(short p_nRowIndex, short p_nColumnIndex, short p_nVSGroupIndex);
	void EnableBoundMark(BOOL p_bEnable);
	void EnableReceivingTimeShare(BOOL p_bEnable);
	BOOL ChangePacketType(LPCTSTR p_szPacketName, LPCTSTR p_szPacketType);
	BOOL ChangeGraphDrawingData(LPCTSTR p_szGraphName, BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData);
	BOOL ChangePriceGraphDrawingData(BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData, short p_nDefaultType);
	void ShowAnalysisToolText(BOOL p_bTimeLine, BOOL p_bPriceLineLeft, BOOL p_bPriceLineRight, BOOL p_bTrendLine, BOOL p_bTrisectionLine, BOOL p_bQuadrisectionLine, BOOL p_bFibonacciRetraceLine);
	BOOL HasGraphInSpecialBack(LPCTSTR p_szGraphName);
	BOOL LoadAddIn(LPCTSTR p_szAddInDllName);
	BOOL InvokeAddIn(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData);
	void SetDragModeType(short p_nDragModeType);
	BOOL ResetRealListString(LPCTSTR p_szRealList, BOOL p_bAdviseNow);
	BOOL AddOrRemoveChart(LPCTSTR strGraphName);
	void SetHtsPath(LPCTSTR pcszHtsPath);
	CString GetRealSettingInfo();
	void SetAllLogScale(BOOL bLogScale);
	void GetListViewerData(BSTR* bstrTitle, BSTR* bstrHorzScale, BSTR* bstrPacketNamesPerBlock, BSTR* bstrPacketNames, BSTR* bstrSaveFilePath, BOOL* bViewPacketSequence, long* lpPacketList);
	void SetDataIndexByDate(LPCTSTR pcszStartDate, LPCTSTR pcszEndDate);
	void SetRunTimeMode(BOOL bRunTimeMode);
	CString GetPacketOption();
	void SetPacketOption(LPCTSTR strPacketOption);
	void SetOutsideCommonInterface(long lpOutsideCommonInterface);
	BOOL RemoveGraph_Title(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle, BOOL bIsOnlyNoTRData, BOOL bIsAutoHorzScalePosition);
	CString GetGraphPacketName(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle);
	BOOL ChangeGraphTitle2(LPCTSTR szTargetTitle, LPCTSTR szSourceTitle);
	BOOL PartitionAllGraph();
	BOOL JoinAllGraph(BOOL bIsOneVertScaleGroup);
	BOOL ChangeAllFunctionName(LPCTSTR szTargetGraphName, LPCTSTR szSourceGraphName, BOOL bKeepOldGraphColor);
	BOOL ChangeIndicatorPacketName(LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewPacketName);
	void RunEnvironmentDlg(short nOption);
	BOOL SetIndicatorCondition(LPCTSTR strGraphName, short nConditionType, short nConditionIndex, double dConditionValue, BOOL bReCalculate);
	BOOL SetPacketData2(LPCTSTR strPacketName, LPCTSTR strPacketType, LPCTSTR strPacketData, short nDataCount, BOOL bReCalculate);
	BOOL SetLogScale(LPCTSTR strGraphName, BOOL bLogScale);
	BOOL GetLogScale(LPCTSTR strGraphName);
	BOOL SetPacketHelpMessage(LPCTSTR strHelpMsg, BOOL bIsAllRemove);
	BOOL InvokeAddInStr(LPCTSTR p_szAddInDllName, LPCTSTR p_szCommandName, LPCTSTR p_szData);
	BOOL SetHelpMessageRealDataRate(LPCTSTR strHelpMessageData, BOOL bIsRemove);
	void SetBongCountMargin(short nBongCountMargin);
	BOOL SetAllPropertiesFromFile(LPCTSTR strFilePath, LPCTSTR strFileName);
	BOOL SetGraphHide3(LPCTSTR strTitle, BOOL bIsHide);
	BOOL GetBoundMarkFlag();
	BOOL UnloadAddIn(LPCTSTR p_szAddInDllName);
	void SetOnePageDataCount2(long nOnePageDataCount, BOOL bIncludeBongCountMargin);
	BOOL ClearPacketData(LPCTSTR strPacketName, BOOL bIsAbsoluteClear);
	void SetAllGraphHide(short nRowIndex, short nColumnIndex, short bHide);
	void InitStaticIndiCond(LPCTSTR p_szBasePath);
	void SetAllGraphsHide(short nRowIndex, short nColumnIndex, BOOL bHide);
	CString GetPacketData2(LPCTSTR strPacketName, short nDataIndex, BOOL bIsFullDataSize);
	CString GetSelectedSubGraphData(short nPoint_x, short nPoint_y);
	short GetPacketIndex(LPCTSTR szPacketName, double dKeyValue);
	short GetCurrentPriceChartType();
	BOOL RemoveBlock(short nPoint_x, short nPoint_y);
	BOOL RemoveGraph(short nRowIndex, short nColumnIndex, LPCTSTR strGraphTitle);
	BOOL SetGraphColor(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, short nGraphIndex, short nSubGraphIndex, LPCTSTR strNewColor);
	BOOL SetPacketDataInIndex(LPCTSTR strPacketName, short nIndex, LPCTSTR strData, BOOL bIsRealData);
	BOOL SetVertScaleLogType(short nRowIndex, short nColumnIndex, BOOL bIsLogType);
	BOOL OpenFileOfEnvAndIndi(LPCTSTR strFileName_Env, LPCTSTR strFileName_Indi, BOOL bIsCurrentSave);
	BOOL SetPacketDataInAdditionPacket(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset);
	void SetOnePageDataCountInTRCount(BOOL bIsTRCount, BOOL bIsNowChange);
	void PrintChatImage();
	void ExportToExcel();
	BOOL RunToolFunction(short nToolCommand, short nOption);
	BOOL SetSubIndicatorCondition(LPCTSTR strGraphName, short nConditionType, short nSubGraphIndex, short nConditionIndex, double dConditionValue, BOOL bReCalculate);
	BOOL SetGraphHide4(LPCTSTR strGraphName, short nSubGraphIndex, BOOL bHide);
	BOOL SetMapDefaultInCurrent(BOOL bIsNewMapDefault);
	BOOL ChangeFunctionName(LPCTSTR p_szTargetFName, LPCTSTR p_szNewFName, LPCTSTR p_szNewSGName);
	BOOL GetHeadAndTailData(LPCTSTR strPacketName, double* pHeadData, double* pTailData);
	LPUNKNOWN GetAddInCustomInterface(LPCTSTR p_szAddInName);
	BOOL LoadAddInWithOverExclusive(LPCTSTR p_szAddInName);
	BOOL OpenFromFile(LPCTSTR p_szFileName, BOOL p_bAutoPrevSave);
	BOOL SaveandOpenFromFileWithMSave(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	BOOL AddRealItemWithSeparator(LPCTSTR szRealInfo, LPCTSTR szSeparatorChar);
	void RemoveRealItemWithSeparator(LPCTSTR szRealInfo, LPCTSTR szSeparatorChar);
	long GetAllProperties3(long p_lIPropertyMap);
	long SetAllProperties3(long p_lIPropertyMap);
	BOOL ShowIndicatorConfigDlgWithName(LPCTSTR p_szGraphName);
	BOOL RemoveBlockWithIndex(short p_nRowIndex, short p_nColumnIndex);
	BOOL SetBinTrHelpMsg(LPCTSTR p_szHelpMessage);
	BOOL SetBinTrDoubleData(LPCTSTR p_szPacketName, long p_nCount, double* p_pdData);
	BOOL SetBinTrStringData(LPCTSTR p_szPacketName, long p_nCount, long p_lData);
	BOOL SetBinTrEnd();
	BOOL EmptyChart2(LPCTSTR p_szChartList, BOOL p_bIsDeleteAllPacket, BOOL p_bIsDefault);
	short GetHorzScalePosition(short p_nRowIndex, short p_nColumnIndex);
	BOOL HasIndicator(LPCTSTR p_szIndicatorName);
	BOOL HasIndicatorInSpecialBack(LPCTSTR p_szIndicatorName);
	BOOL LockWindowUpdate();
	void UnlockWindowUpdate();
	CString AddChartItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, BOOL bBuiltChart);
	void SetRealDataCode(LPCTSTR strCode, LPCTSTR strPacketName, double dData, short nOption);
	void ShowIndicator(LPCTSTR p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate);
	void SetPacketAutoScaleType(LPCTSTR p_szPacketName, BOOL p_bAutoScaleType);
	BOOL SetPacketTypeWithAutoScale(LPCTSTR p_szPacketName, LPCTSTR p_szPacketType, BOOL p_bAutoScale);
	void ShowBaseText(BOOL p_bShow);
	BOOL ChangeGraphColor(LPCTSTR p_szGraphTitle, LPCTSTR p_szColors);
	void AddPacketRQ(LPCTSTR lpszRQ, LPCTSTR lpszPacketOption);
	BOOL SetPacketDataRQ(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, long nData, long nLength, BOOL bReset);
	BOOL AddChartItemRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName);
	void SetCurrentRQ(LPCTSTR lpszRQ);
	void SetResetTotalCountWhileDraw(BOOL bReset);
	void SetFutureScaleVisible(long nScaleVisible);
	void SetMultiItemMode(BOOL bMultiItem);
	void SetVertScaleType(short nBlockRow, short nBlockCol, short nScaleGroup, short nScaleType, double dTypeValue);
	void OpenFromPage(LPCTSTR p_szPagePath, BOOL p_bIsVirtualPage, short p_nVirtualPageIndex, short p_nDuplicateIndex);
	void SaveToPage(LPCTSTR p_szPagePath, BOOL p_bIsVirtualPage, short p_nVirtualPageIndex, short p_nDuplicateIndex);
*/
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WOORICHART_H__78393149_A4E3_4CE8_9EB8_A39F6BF0FC78__INCLUDED_)
