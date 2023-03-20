// NeoChart2001Ctl.cpp : Implementation of the CKTBChartCtrl ActiveX Control class.

#include "stdafx.h"
#include "KTBChart.h"
#include "KTBChartCtl.h"

#include <Mmsystem.h>										// for timeGetTime()
#include <assert.h>											// for assert()

#include "../Include/DumpLogger.h"							// for CDumpLogger
#include "../AddInManager/Include_AddIn/_resource.h"		// for ID_PLM_ADD_MENU
#include "../AddInManager/Include_AddIn/AddInCommand.h"		// for EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR
#include "../Include_Chart/Dll_Load/ToolBarMgrHelper.h"		// for CToolBarMgrHelper
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for IMetaTableOCX
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/PropertyMap.h"					// for CPropertyMap
#include "../Include_Chart/xUtil_Chart.h"					// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IRealReceiver.h"					// for IRealReceiver
#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/Block.h"							// for CBlock
#include "../Include_Chart/BaseData.h"						// for CTimeDiffInfo  // 20081007 JS.Kim	����ڽð��� ����
#include "PropertiesData.h"									// for INIT_PROPERTIES_NUM
#include "NeoChart2001In.h"									// for CNeoChart2001In
#include "MainBlock.h"										// for CMainBlock
#include "PacketList.h"										// for CPacketList
#include "IndicatorList.h"									// for CIndicatorList
#include "ResourceManager.h"								// for CResourceManager
#include "ToolBarManager.h"									// for IToolBarManager
#include "Conversion.h"										// for CDataConversion
#include "Color.h"											// for CGraphRegionColor
#include "FileBuilder.h"									// for CPropertyFileBuilder
#include "ViewData.h"										// for CSubGraphViewDataList
#include "ExcelDataController.h"							// for CExcelDataController
#include "IndicatorInfo.h"									// for CIndicatorInfo
#include "DataMath.h"										// for CMath
#include "ChartCfg.h"										// for CChartCfg
#include "GraphData.h"										// for CSubGraphData
#include "DrdsData.h"										// for WM_DRDS_DATA_PACKET_END
#include "LocalShareInfoList.h"								// for CLocalShareInfoList
#include "BaseDesk.h"										// for CBaseDesk2
#include "PacketListManager.h"								// ���� PacketList�� ���� : �������� - ojtaso (20061224)
#include "Graph.h"
#include "IWChartCtrl.h"									// for CIWChartCtrl	(Cuation. Define _ICHARTCTRL... first!)
#include "InnerFunction.inl"								// for GetMainBlockRegion() ...
#include "DependSite.inl"									// for OnFullMinMax
#include "OcxIDs.h"											// for _OCX_IMPLEMENT_OLECREATE_EX()


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_LSHAREDCHANGED			= ::RegisterWindowMessage(_T("RMSG_LSHAREDCHANGED"));

// (2004.06.08, ��¿�) Global ���� ����� �߰��Ѵ�.
const UINT RMSG_FORMINITIALCOMPLETED	= ::RegisterWindowMessage(_T("RMSG_FORMINITIALCOMPLETED"));
//sy 2005.04.26. UseSkin ���� �޽���
const UINT RMSG_FORMSTATECHANGE	= ::RegisterWindowMessage(_T("RMSG_FORMSTATECHANGE"));

CString FILE_INDICATOR_CONDITIONS =		"IndiCond.cfg";

//@Solomon -->
const UINT RMSG_GET_USERID		= ::RegisterWindowMessage(_T("RMSG_GET_USERID"));
#define	SZ_STRING_KEY100				"DRFNChart"
//@Solomon <--

IMPLEMENT_DYNCREATE(CKTBChartCtrl, COleControl)

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CKTBChartCtrl, COleControl)
	//{{AFX_MSG_MAP(CKTBChartCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
	ON_COMMAND(ID_DELETE_BLOCK, OnDeleteBlock)
	ON_COMMAND(ID_RETURN_ORG, OnReturnOrg)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_RETURN_ORG_DISPLAY, OnReturnOrgDisplay)
	ON_COMMAND(ID_PRINT_SCREEN, OnPrint)
	ON_COMMAND(ID_TO_EXCEL, OnToExcel)
	ON_WM_TIMER()
	ON_WM_CAPTURECHANGED()
	ON_UPDATE_COMMAND_UI(IDM_REVERSE_YSCALE, OnUpdateReverseYscale)
	ON_COMMAND(IDM_REVERSE_YSCALE, OnReverseYscale)
	ON_WM_GETDLGCODE()
	ON_COMMAND(ID_PRICECHART_MINMAX, OnPricechartMinmax)
	ON_UPDATE_COMMAND_UI(ID_PRICECHART_MINMAX, OnUpdatePricechartMinmax)
	ON_COMMAND(ID_SCREEN_MINMAX, OnScreenMinmax)
	ON_UPDATE_COMMAND_UI(ID_SCREEN_MINMAX, OnUpdateScreenMinmax)
	ON_COMMAND(ID_MENU_CROSSLINE, OnMenuCrossline)
	ON_UPDATE_COMMAND_UI(ID_MENU_CROSSLINE, OnUpdateMenuCrossline)
	ON_COMMAND(ID_MENU_DATAWINDOW, OnMenuDatawindow)
	ON_UPDATE_COMMAND_UI(ID_MENU_DATAWINDOW, OnUpdateMenuDatawindow)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_COPYDATA()
	ON_COMMAND(ID_RETURN_TO_GENERAL_CHART, OnReturnToGeneralChart)
	ON_UPDATE_COMMAND_UI(ID_DELETE_BLOCK, OnUpdateDeleteBlock)
	ON_UPDATE_COMMAND_UI(ID_RETURN_ORG_DISPLAY, OnUpdateReturnOrgDisplay)
	ON_UPDATE_COMMAND_UI(ID_RETURN_ORG, OnUpdateReturnOrg)
	ON_UPDATE_COMMAND_UI(ID_TO_EXCEL, OnUpdateToExcel)
	ON_UPDATE_COMMAND_UI(ID_PRINT_SCREEN, OnUpdatePrintScreen)
	ON_UPDATE_COMMAND_UI(ID_RETURN_TO_GENERAL_CHART, OnUpdateReturnToGeneralChart)
// �������� ���� �޴� ���� - ojtaso (20071025)
//	ON_COMMAND(ID_GLANCEBALANCE_CURSOR, OnGlancebalanceCursor)
//	ON_UPDATE_COMMAND_UI(ID_GLANCEBALANCE_CURSOR, OnUpdateGlancebalanceCursor)
	ON_COMMAND(ID_HIDE_GRAPH_TITLE, OnHideGraphTitle)
	ON_UPDATE_COMMAND_UI(ID_HIDE_GRAPH_TITLE, OnUpdateHideGraphTitle)
	ON_COMMAND(ID_SHOW_MINMAX_TEXT, OnShowMinmaxText)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MINMAX_TEXT, OnUpdateShowMinmaxText)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(UM_CURSOR_EXCHANGE, OnCursorExchange)
	ON_MESSAGE(UM_SET_ETC, OnSetEtc)
	ON_MESSAGE(UM_GRAPH_SELECTEDDATAINDEX, OnGraphSelectedIndex)
	//nami 11.26
	ON_MESSAGE(UDM_GET_USERMAGIN, OnGetUserMargin)
	// (2005/11/22 - Seung-Won, Bae) DRDS Real Message
	ON_MESSAGE(WM_DRDS_DATA, OnDrdsData)
	ON_MESSAGE(WM_DRDS_DATA_PACKET_END, OnDrdsPacketEnd)
	ON_REGISTERED_MESSAGE( RMS_ANALYSIS_FUNC, OnToolFunction)

	// (2006/10/18 - Seung-Won, Bae) Add AddIn Message Proc.
	ON_MESSAGE( UDM_ADDIN_MSG, OnAddInMsg)
	// (2007/2/21 - Seung-Won, Bae) Draw in MemDC for Print or Save
	ON_MESSAGE( UDM_DRAW_FOR_PRINT, OnDrawForPrint)		// wParam - 0, lParam - HDC

	// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI (Zoom)
	ON_MESSAGE( UDM_RECALC_DRI, OnReCalcDRI)

	// (2008/10/21 - JS, Kim) User Changes Time Difference
	ON_MESSAGE( UDM_CHANGE_TIMEDIFF, OnChangeTimeDiff)		// wParam - Time Index, lParam - 0

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CKTBChartCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CKTBChartCtrl)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nChartMode", m_nChartMode, OnNChartModeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bNormalizedMinuteTimeRule", m_bNormalizedMinuteTimeRule, OnbNormalizedMinuteTimeRule, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBlockType", m_nBlockType, OnnBlockTypeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBlockCompart", m_nBlockCompart, OnnBlockCompartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBlockRowCount", m_nBlockRowCount, OnnBlockRowCountChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBlockColumnCount", m_nBlockColumnCount, OnnBlockColumnCountChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBlockGap", m_nBlockGap, OnnBlockGapChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBlockBorderThickness", m_nBlockBorderThickness, OnnBlockBorderThicknessChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strAddInLoadList", m_strAddInLoadList, OnStrAddInLoadListChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBorderType", m_nBorderType, OnnBorderTypeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nScaleLineType", m_nNotUsedProperty, OnNotUsedPropertyChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bWholeView", m_bWholeView, OnbWholeViewChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nOnePageDataCount", m_nOnePageDataCount, OnnOnePageDataCountChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nUserBlockAddCount", m_nUserBlockAddCount, OnnUserBlockAddCountChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bGraphDragDrop", m_bGraphDragDrop, OnbGraphDragDropChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nLeftMargin", m_nLeftMargin, OnnLeftMarginChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nRightMargin", m_nRightMargin, OnnRightMarginChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nTopMargin", m_nTopMargin, OnnTopMarginChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBottomMargin", m_nBottomMargin, OnnBottomMarginChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bAddGraphMenuShow", m_bAddGraphMenuShow, OnbAddGraphMenuShowChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strUserEnvironmentFileName", m_strUserEnvironmentFileName, OnstrUserEnvironmentFileNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strUserIndicatorListFileName", m_strUserIndicatorListFileName, OnstrUserIndicatorListFileNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strUserSeriesListFileName", m_strNotUsedProperty, OnNotUsedPropertyChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bHide", m_bHide, OnbHideChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bIsInput", m_bIsInput, OnbIsInputChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strToolbarOption", m_strToolbarOption, OnstrToolbarOptionChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strPacketOption", m_strPacketOption, OnstrPacketOptionChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strLocalSharedList", m_strLocalSharedList, OnstrLocalSharedListChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bUseRealData", m_bUseRealData, OnbUseRealDataChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strRealList", m_strRealList, OnstrRealListChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nMarginBorderType", m_nMarginBorderType, OnNMarginBorderTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nRowDoubleSizeBlock", m_nRowDoubleSizeBlock, OnNRowDoubleSizeBlockChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bUseInquiryDlgWhenUseCrossLine", m_bUseInquiryDlgWhenUseCrossLine, OnBUseInquiryDlgWhenUseCrossLineChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nColumnDoubleSizeBlock", m_nColumnDoubleSizeBlock, OnNColumnDoubleSizeBlockChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bUseShortKey", m_bUseShortKey, OnBUseShortKeyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bUseConditiontoTitle", m_bUseConditiontoTitle, OnBUseConditiontoTitleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bUseDatatoTitle", m_bUseDatatoTitle, OnBUseDatatoTitleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nHorzGap", m_nHorzGap, OnNHorzGapChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nVertGap", m_nVertGap, OnNVertGapChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nRButtonMenuType", m_nRButtonMenuType, OnNRButtonMenuTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bShowWarningMessageBox", m_bShowWarningMessageBox, OnBShowWarningMessageBoxChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bControlBlockSize", m_bControlBlockSize, OnBControlBlockSizeChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bBlockMoveDelete", m_bBlockMoveDelete, OnBBlockMoveDeleteChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bSelectTitle", m_bSelectTitle, OnBSelectTitleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bSelectGraph", m_bSelectGraph, OnBSelectGraphChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bUseScaleRButtonMenu", m_bUseScaleRButtonMenu, OnBUseScaleRButtonMenuChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nGraphRButtonType", m_nGraphRButtonType, OnNGraphRButtonTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nHoldingPacketSize", m_nHoldingPacketSize, OnNHoldingPacketSizeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "szBasePacketName", m_szBasePacketName, OnSzBasePacketNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bRedrawAfterUpdatedAllPackets", m_bNotUsedProperty, OnNotUsedPropertyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nMinDataCountOnOnePage", m_nMinDataCountOnOnePage, OnNMinDataCountOnOnePageChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bFireRealEvent", m_bFireRealEvent, OnBFireRealEventChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nInquiryType", m_nInquiryType, OnNInquiryTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bOcxAreaNumericalInquiry", m_bOcxAreaNumericalInquiry, OnBOcxAreaNumericalInquiryChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nNumericalInquiryOption", m_nNumericalInquiryOption, OnNNumericalInquiryOptionChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strGlobalSharedList", m_strGlobalSharedList, OnStrGlobalSharedListChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nUseInquiryWithCrossline", m_nUseInquiryWithCrossline, OnNUseInquiryWithCrosslineChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bShowPriceRateInquiry", m_bShowPriceRateInquiry, OnBShowPriceRateInquiryChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bDoesUseSkinColor", m_bDoesUseSkinColor, OnBDoesUseSkinColorChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strMapName", m_strMapName, OnStrMapNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nBongCountMargin", m_nBongCountMargin, OnNBongCountMarginChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bOnePageDataCountInTRCount", m_bOnePageDataCountInTRCount, OnBOnePageDataCountInTRCountChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bIsMapDefault", m_bNotUsedProperty, OnNotUsedPropertyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nPacketCountDataSize", m_nPacketCountDataSize, OnNPacketCountDataSizeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nLanguageID", m_nLanguageID, OnNLanguageIDChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bFreezedPageState", m_bFreezedPageState, OnBFreezedPageStateChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "strPagePathWithPrefix", m_strPagePathWithPrefix, OnStrPagePathWithPrefixChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "nRealTimeTruncationType", m_nRealTimeTruncationType, OnNRealTimeTruncationTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CKTBChartCtrl, "bUseGapRevision", m_bUseGapRevision, OnBUseGapRevisionChanged, VT_BOOL)
	DISP_PROPERTY_EX(CKTBChartCtrl, "IsShowInneerToolBar", GetIsShowInneerToolBar, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CKTBChartCtrl, "strChartList", GetStrChartList, SetStrChartList, VT_BSTR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrBackground", GetClrBackground, SetClrBackground, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrBackBorder", GetClrBackBorder, SetClrBackBorder, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrBlock", GetClrBlock, SetClrBlock, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrBlockBorder", GetClrBlockBorder, SetClrBlockBorder, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrScaleLine", GetClrScaleLine, SetClrScaleLine, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrScaleText", GetClrScaleText, SetClrScaleText, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrGraphBase1", GetClrGraphBase1, SetClrGraphBase1, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrGraphBase2", GetClrGraphBase2, SetClrGraphBase2, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrGraphBase3", GetClrGraphBase3, SetClrGraphBase3, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrGraphBase4", GetClrGraphBase4, SetClrGraphBase4, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrGraphBase5", GetClrGraphBase5, SetClrGraphBase5, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrGraphBase6", GetClrGraphBase6, SetClrGraphBase6, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrAnalysisTool", GetClrAnalysisTool, SetClrAnalysisTool, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "strBlockRegion", GetStrBlockRegion, SetStrBlockRegion, VT_BSTR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrScaleLine2", GetClrScaleLine2, SetClrScaleLine2, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrInquiryWindow", GetClrInquiryWindow, SetClrInquiryWindow, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "CustomFont", GetCustomFont, SetCustomFont, VT_BSTR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "clrLattice", GetClrLattice, SetClrLattice, VT_COLOR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "strStartDateOfData", GetStartDateOfData, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "OnRunningOneChart", OnRunningOneChart, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CKTBChartCtrl, "EnableHorizontalLineInNIDLG", GetNotSupported, SetEnableHorizontalLineInNIDLG, VT_BOOL)
	DISP_PROPERTY_EX(CKTBChartCtrl, "bShowPriceMinMaxText", GetBShowPriceMinMaxText, SetBShowPriceMinMaxText, VT_BOOL)
	DISP_PROPERTY_EX(CKTBChartCtrl, "bHideAllTitle", GetBHideAllTitle, SetBHideAllTitle, VT_BOOL)
	DISP_PROPERTY_EX(CKTBChartCtrl, "strMapIndicatorInfo", GetStrMapIndicatorInfo, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CKTBChartCtrl, "bEnableIndicatorCalculation", GetBEnableIndicatorCalculation, SetBEnableIndicatorCalculation, VT_BOOL)
	DISP_PROPERTY_EX(CKTBChartCtrl, "nCurrentDrawCount", GetNCurrentDrawCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CKTBChartCtrl, "nFullDataCount", GetNFullDataCount, SetNotSupported, VT_I4)
	DISP_FUNCTION(CKTBChartCtrl, "SetAllProperties2", SetAllProperties2, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "SetBaseDesk", SetBaseDesk, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "GetBase", GetBase, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetChartData", SetChartData, VT_I2, VTS_I4 VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetChartDataStr", SetChartDataStr, VT_I2, VTS_BSTR VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "testGetPacketdatasize", testGetPacketdatasize, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "testLoadAnalysisTools", testLoadAnalysisTools, VT_EMPTY, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "MakeSaveShortKey", MakeSaveShortKey, VT_EMPTY, VTS_BSTR VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeChart", NotUsedMethod5, VT_I2, VTS_I2 VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_COLOR)
	DISP_FUNCTION(CKTBChartCtrl, "ClearChartData", ClearChartData, VT_I2, VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetGraphHide", SetGraphHide, VT_I2, VTS_I2 VTS_I2 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "AddChartBlock", AddChartBlock, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetOnePageDataCount", SetOnePageDataCount, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "GetOnePageDataCount", GetOnePageDataCount, VT_I4, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "SaveandOpenFromFile", SaveandOpenFromFileWithASave, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "JoinandDevisionforCompareChart", JoinandDevisionforCompareChart, VT_BOOL, VTS_I4 VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeGraphTitle", ChangeGraphTitle, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetChartDataFromFile", SetChartDataFromFile, VT_I2, VTS_BSTR VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveChart", RemoveChart, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeGraphType", ChangeGraphType, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetChartAllInfo", SetChartAllInfo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "GetData", GetData, VT_BSTR, VTS_BSTR VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetDrdsAdvise", SetDrdsAdvise, VT_BOOL, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetDataCount", GetDataCount, VT_I2, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeChartItem", ChangeChartItem, VT_BOOL, VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetStopLossPrice", NotUsedMethod7, VT_BOOL, VTS_BOOL VTS_R8 VTS_BOOL VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "RestoreMinMax", RestoreMinMax, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RestoreMin", RestoreMin, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RestorMax", RestorMax, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetMin", SetMin, VT_BOOL, VTS_BSTR VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "SetMax", SetMax, VT_BOOL, VTS_BSTR VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "SetMinMax", SetMinMax, VT_BOOL, VTS_BSTR VTS_R8 VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "SetHorzScaleData", SetHorzScaleData, VT_BOOL, VTS_I2 VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetMin", GetMin, VT_R8, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetMax", GetMax, VT_R8, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetGraphHide2", SetGraphHide2, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeGraphDatas", NotUsedMethod1, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SaveandApplyGraphStyle", SaveandApplyGraphStyle, VT_EMPTY, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetDataRegionInBlock", SetDataRegionInBlock, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetItemInfo", SetItemInfo, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "AddandDeleteIndicator", AddandDeleteIndicator, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetAllProperties2", GetAllProperties2, VT_I4, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "GetBasePropIf", GetBasePropIf, VT_I4, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "SetVertScaleData", SetVertScaleData, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeBlockType", NotUsedMethod4, VT_BOOL, VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeHorzScalePosition", ChangeHorzScalePosition, VT_BOOL, VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ResetChart", ResetChart, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "SetRealData", SetRealData, VT_EMPTY, VTS_BSTR VTS_R8 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetGraphTitle", SetGraphTitle, VT_BOOL, VTS_BSTR VTS_I2 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetIndicatorNameList", GetIndicatorNameList, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "AddGraph", AddGraph, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "AddPacket", AddPacket, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RemovePacket", RemovePacket, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetContainerDllInfo", SetContainerDllInfo, VT_EMPTY, VTS_I4 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketData", SetPacketData, VT_BOOL, VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetBackgroundColors", SetBackgroundColors, VT_EMPTY, VTS_COLOR VTS_COLOR)
	DISP_FUNCTION(CKTBChartCtrl, "AddRealItem", AddRealItem, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ShowIndicatorConfigDlg", ShowIndicatorConfigDlg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "ResetChartCfgWith", ResetChartCfgWith, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "EmptyChart", EmptyChart, VT_BOOL, VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "HasGraph", HasGraph, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveRealItem", RemoveRealItem, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ShowInnerToolBar", ShowInnerToolBar, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeVertScalePosition", ChangeVertScalePosition, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "GetRowBlockCount", GetRowBlockCount, VT_I2, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "GetColumnBlockCount", GetColumnBlockCount, VT_I2, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "SetStartPeriodOfPacket", SetStartPeriodOfPacket, VT_BOOL, VTS_BSTR VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetStartPeriodOfPacket", GetStartPeriodOfPacket, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetStartEndDateOnView", GetStartEndDateOnView, VT_BOOL, VTS_PR8 VTS_PR8)
	DISP_FUNCTION(CKTBChartCtrl, "SetStartEndDateOfView", SetStartEndDateOfView, VT_BOOL, VTS_R8 VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "ScrollToDataOfEnd", ScrollToDataOfEnd, VT_BOOL, VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "ZoomToWithRightFix", ZoomToWithRightFix, VT_I2, VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ClearTimeMark", ClearTimeMark, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "ShowEnvironmentDlg", ShowEnvironmentDlg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "SetTimeMark", SetTimeMark, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetVertScalePosition", GetVertScalePosition, VT_I2, VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "EnableBoundMark", EnableBoundMark, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "EnableReceivingTimeShare", EnableReceivingTimeShare, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ChangePacketType", ChangePacketType, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeGraphDrawingData", ChangeGraphDrawingData, VT_BOOL, VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ChangePriceGraphDrawingData", ChangePriceGraphDrawingData, VT_BOOL, VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ShowAnalysisToolText", ShowAnalysisToolText, VT_EMPTY, VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "HasGraphInSpecialBack", HasGraphInSpecialBack, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "LoadAddIn", LoadAddIn, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "InvokeAddIn", InvokeAddIn, VT_BOOL, VTS_BSTR VTS_I2 VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "SetDragModeType", SetDragModeType, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ResetRealListString", ResetRealListString, VT_BOOL, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "AddOrRemoveChart", AddOrRemoveChart, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetHtsPath", SetHtsPath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetRealSettingInfo", GetRealSettingInfo, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "SetAllLogScale", SetAllLogScale, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetListViewerData", GetListViewerData, VT_EMPTY, VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBOOL VTS_PI4)
	DISP_FUNCTION(CKTBChartCtrl, "SetDataIndexByDate", SetDataIndexByDate, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetRunTimeMode", SetRunTimeMode, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetPacketOption", GetPacketOption, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketOption", SetPacketOption, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetOutsideCommonInterface", SetOutsideCommonInterface, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveGraph_Title", RemoveGraph_Title, VT_BOOL, VTS_I2 VTS_I2 VTS_BSTR VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetGraphPacketName", GetGraphPacketName, VT_BSTR, VTS_I2 VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeGraphTitle2", ChangeGraphTitle2, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "PartitionAllGraph", PartitionAllGraph, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "JoinAllGraph", JoinAllGraph, VT_BOOL, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeAllFunctionName", NotUsedMethod3, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeIndicatorPacketName", ChangeIndicatorPacketName, VT_BOOL, VTS_BSTR VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RunEnvironmentDlg", RunEnvironmentDlg, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetIndicatorCondition", SetIndicatorCondition, VT_BOOL, VTS_BSTR VTS_I2 VTS_I2 VTS_R8 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketData2", SetPacketData2, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetLogScale", SetLogScale, VT_BOOL, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetLogScale", GetLogScale, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketHelpMessage", SetPacketHelpMessage, VT_BOOL, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "InvokeAddInStr", InvokeAddInStr, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetHelpMessageRealDataRate", SetHelpMessageRealDataRate, VT_BOOL, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetBongCountMargin", SetBongCountMargin, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetAllPropertiesFromFile", SetAllPropertiesFromFile, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetGraphHide3", SetGraphHide3, VT_BOOL, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetBoundMarkFlag", GetBoundMarkFlag, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "UnloadAddIn", UnloadAddIn, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetOnePageDataCount2", SetOnePageDataCount2, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ClearPacketData", ClearPacketData, VT_BOOL, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetAllGraphHide", SetAllGraphHide, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "InitStaticIndiCond", InitStaticIndiCond, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetAllGraphsHide", SetAllGraphsHide, VT_EMPTY, VTS_I2 VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetPacketData2", GetPacketData2, VT_BSTR, VTS_BSTR VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetSelectedSubGraphData", NotUsedMethod6, VT_BSTR, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "GetPacketIndex", GetPacketIndex, VT_I2, VTS_BSTR VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "GetCurrentPriceChartType", GetCurrentPriceChartType, VT_I2, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveBlock", RemoveBlock, VT_BOOL, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveGraph", RemoveGraph, VT_BOOL, VTS_I2 VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetGraphColor", SetGraphColor, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketDataInIndex", SetPacketDataInIndex, VT_BOOL, VTS_BSTR VTS_I2 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetVertScaleLogType", SetVertScaleLogType, VT_BOOL, VTS_I2 VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "OpenFileOfEnvAndIndi", OpenFileOfEnvAndIndi, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketDataInAdditionPacket", SetPacketDataInAdditionPacket, VT_BOOL, VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetOnePageDataCountInTRCount", SetOnePageDataCountInTRCount, VT_EMPTY, VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "PrintChatImage", PrintChatImage, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "ExportToExcel", ExportToExcel, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "RunToolFunction", _RunToolFunction, VT_BOOL, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetSubIndicatorCondition", SetSubIndicatorCondition, VT_BOOL, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_R8 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetGraphHide4", SetGraphHide4, VT_BOOL, VTS_BSTR VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetMapDefaultInCurrent", SetMapDefaultInCurrent, VT_BOOL, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeFunctionName", NotUsedMethod2, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetHeadAndTailData", GetHeadAndTailData, VT_BOOL, VTS_BSTR VTS_PR8 VTS_PR8)
	DISP_FUNCTION(CKTBChartCtrl, "GetAddInCustomInterface", GetAddInCustomInterface, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "LoadAddInWithOverExclusive", LoadAddInWithOverExclusive, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "OpenFromFile", OpenFromFile, VT_BOOL, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SaveandOpenFromFileWithMSave", SaveandOpenFromFileWithMSave, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "AddRealItemWithSeparator", AddRealItemWithSeparator, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveRealItemWithSeparator", RemoveRealItemWithSeparator, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetAllProperties3", GetAllProperties3, VT_I4, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "SetAllProperties3", SetAllProperties3, VT_I4, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "ShowIndicatorConfigDlgWithName", ShowIndicatorConfigDlgWithName, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveBlockWithIndex", RemoveBlockWithIndex, VT_BOOL, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetBinTrHelpMsg", SetBinTrHelpMsg, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetBinTrDoubleData", SetBinTrDoubleData, VT_BOOL, VTS_BSTR VTS_I4 VTS_PR8)
	DISP_FUNCTION(CKTBChartCtrl, "SetBinTrStringData", SetBinTrStringData, VT_BOOL, VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "SetBinTrEnd", SetBinTrEnd, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "EmptyChart2", EmptyChart2, VT_BOOL, VTS_BSTR VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetHorzScalePosition", GetHorzScalePosition, VT_I2, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "HasIndicator", HasIndicator, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "HasIndicatorInSpecialBack", HasIndicatorInSpecialBack, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "LockWindowUpdate", LockWindowUpdate, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "UnlockWindowUpdate", UnlockWindowUpdate, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "AddChartItem", AddChartItem, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetRealDataCode", SetRealDataCode, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_R8 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ShowIndicator", ShowIndicator, VT_EMPTY, VTS_BSTR VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketAutoScaleType", SetPacketAutoScaleType, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketTypeWithAutoScale", SetPacketTypeWithAutoScale, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ShowBaseText", ShowBaseText, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeGraphColor", ChangeGraphColor, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "AddPacketRQ", AddPacketRQ, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetPacketDataRQ", SetPacketDataRQ, VT_BOOL, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "AddChartItemRQ", AddChartItemRQ, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetCurrentRQ", SetCurrentRQ, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetResetTotalCountWhileDraw", NotUsedMethod8, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetFutureScaleVisible", SetFutureScaleVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "SetMultiItemMode", SetMultiItemMode, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetVertScaleType", SetVertScaleType, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "OpenFromPage", OpenFromPage, VT_EMPTY, VTS_BSTR VTS_BOOL VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SaveToPage", SaveToPage, VT_EMPTY, VTS_BSTR VTS_BOOL VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ZoomTo", ZoomTo, VT_BOOL, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "ShiftPacket", ShiftPacket, VT_BOOL, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "SaveandOpenFromAnalToolFile", SaveandOpenFromAnalToolFile, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetHorizontalScaleHeight", SetHorizontalScaleHeight, VT_EMPTY, VTS_I2 VTS_I2 VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeGraphPropertyAtOnce", ChangeGraphPropertyAtOnce, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_BSTR VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "EmptyChart3", EmptyChart3, VT_BOOL, VTS_BSTR VTS_BOOL VTS_BOOL VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "AddChartAllBlock", AddChartAllBlock, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveGraphRQ", RemoveGraphRQ, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "AddPairItem", AddPairItem, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "HasIndicatorWithRQ", HasIndicatorWithRQ, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "RemoveAllGraphOfRQ", RemoveAllGraphOfRQ, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetRowOfGraphWithRQ", GetRowOfGraphWithRQ, VT_I2, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetGraphColorWithRQ", SetGraphColorWithRQ, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetUseSTExtData", SetUseSTExtData, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ShowGraphDataDrawingData", ShowGraphDataDrawingData, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BOOL VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "ShowSelEnvironmentDlg", ShowSelEnvironmentDlg, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "GetVertScaleGroupCount", GetVertScaleGroupCount, VT_I4, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetSyncPacketData", SetSyncPacketData, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetEmptyChart", SetEmptyChart, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "SetDrawVerticalBox", SetDrawVerticalBox, VT_EMPTY, VTS_R8 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "UseDummyPacketData", UseDummyPacketData, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "SetDoubleSizeBlock", SetDoubleSizeBlock, VT_EMPTY, VTS_I2 VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "AddHorizontalLine", AddHorizontalLine, VT_I4, VTS_BSTR VTS_COLOR VTS_I2 VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "DeleteHorizontalLine", DeleteHorizontalLine, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CKTBChartCtrl, "DeleteAllHorizontalLine", DeleteAllHorizontalLine, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "UpdateHorizontalLine", UpdateHorizontalLine, VT_EMPTY, VTS_I4 VTS_BSTR VTS_COLOR VTS_I2 VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "GetPacketTypeFromKrxCmeCode", GetPacketTypeFromKrxCmeCode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetEncUserID", SetEncUserID, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ReCalculateAllGraphs", ReCalculateAllGraphs, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CKTBChartCtrl, "GetGraphPropertyAtOnce", GetGraphPropertyAtOnce, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "ChangeVertScaleMinMaxType", ChangeVertScaleMinMaxType, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_R8 VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "SetAvgUnitPrice", SetAvgUnitPrice, VT_EMPTY, VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "GetPacketCount", GetPacketCount, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "FindRQ", FindRQ, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "GetPacketNearIndex", GetPacketNearIndex, VT_I2, VTS_BSTR VTS_R8)
	DISP_FUNCTION(CKTBChartCtrl, "ChangePacketTypeRQ", ChangePacketTypeRQ, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "SetDrawBaseLine", SetDrawBaseLine, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CKTBChartCtrl, "GetGraphHide4", GetGraphHide4, VT_BOOL, VTS_BSTR VTS_I2)
	DISP_FUNCTION(CKTBChartCtrl, "SetItemCode", SetItemCode, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CKTBChartCtrl, "MoveChart", MoveChart, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_BOOL)
	DISP_STOCKPROP_ENABLED()
	DISP_STOCKPROP_HWND()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CKTBChartCtrl, COleControl)
	//{{AFX_EVENT_MAP(CKTBChartCtrl)
	EVENT_CUSTOM("DataUpandDown", FireDataUpandDown, VTS_I2  VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("RequestBongData", FireRequestBongData, VTS_I2  VTS_I2)
	EVENT_CUSTOM("ChangeOnePageDataCount", FireChangeOnePageDataCount, VTS_I4)
	EVENT_CUSTOM("Restore", FireRestore, VTS_NONE)
	EVENT_CUSTOM("RequestMapLoad", FireRequestMapLoad, VTS_I2  VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("RequestNewPacket", FireRequestNewPacket, VTS_I2  VTS_BSTR  VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("SaveSettedChartInfoToServer", FireSaveSettedChartInfoToServer, VTS_NONE)
	EVENT_CUSTOM("GetSettedChartInfoFromServer", FireGetSettedChartInfoFromServer, VTS_NONE)
	EVENT_CUSTOM("RequestPacketData", FireRequestPacketData, VTS_BSTR  VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("AddInEvent", FireAddInEvent, VTS_BSTR  VTS_I2  VTS_BSTR  VTS_R8  VTS_I4)
	EVENT_CUSTOM("ChangeDateIndex", FireChangeDateIndex, VTS_I2)
	EVENT_CUSTOM("RequestCodeAndName", FireRequestCodeAndName, VTS_NONE)
	EVENT_CUSTOM("SendDataIndexInLButtonUp", FireSendDataIndexInLButtonUp, VTS_I2  VTS_I2  VTS_I2  VTS_R8)
	EVENT_CUSTOM("ReturnOrgChart", FireReturnOrgChart, VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CKTBChartCtrl, 1)
	PROPPAGEID(CLSID_CFontPropPage)
//	PROPPAGEID(CKTBChartPropPage::guid)
END_PROPPAGEIDS(CKTBChartCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

_OCX_IMPLEMENT_OLECREATE_EX()

/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CKTBChartCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DKTBChart = _OCX_PRIMARY_DISP_ID_VALUES;
const IID BASED_CODE IID_DKTBChartEvents = _OCX_EVENT_DISP_ID_VALUES;


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwKTBChartOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CKTBChartCtrl, IDS_KTBCHART, _dwKTBChartOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CKTBChartCtrl::CKTBChartCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CKTBChartCtrl

BOOL CKTBChartCtrl::CKTBChartCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_KTBCHART,
			IDB_KTBCHART,
			afxRegApartmentThreading,
			_dwKTBChartOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CKTBChartCtrl::CKTBChartCtrl - Constructor


// (2004.09.06, ��¿�) OCX Counter�� �ξ�, ������ OCX�� ����ÿ� User Default ������ �����Ѵ�.
int	CKTBChartCtrl::m_nChartOcxCnt = 0;

// (2006/8/23 - Seung-Won, Bae) Manage AddIn Load String
CMapStringToString CKTBChartCtrl::m_mapAddInLoadString;

// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
const int CKTBChartCtrl::m_nTestEnvBufferSize		= 1024 * 10;
BOOL CKTBChartCtrl::m_bTestEnvInitialized			= FALSE;
CMapStringToString CKTBChartCtrl::m_mapTestEnvSetting;


// (2006/10/27 - Seung-Won, Bae) for old Interface.
void CKTBChartCtrl::OnNotUsedPropertyChanged()
{
	_ML_TEST_NOT_USED();
}

// (2009/9/1 - Seung-Won, Bae) Define Chart Mode. (Load setting.)
CChartInfo::CHART_MODE CKTBChartCtrl::m_nChartModeSetting = ( CChartInfo::CHART_MODE)INT_MIN;

// (2003.12.23, ��¿�) Resource Manager ������ �����Ѵ�.
CKTBChartCtrl::CKTBChartCtrl() :
	m_FontKey( 0, 0, ""),
	//@Solomon-091113 DATA_FILE_FOLDER("\\Data\\"), 
	//DATA_FILE_FOLDER("\\Hanaro\\"),	//@Solomon:Directory-091113
	DATA_FILE_FOLDER("\\Icss\\"),	//IBK
	USER_FILE_FOLDER("\\User\\"),
	FILETYPE(".cht"),
	m_pxScaleManager(NULL),
	m_pPacketListManager(NULL), // ������ PacketList�� ���� : �������� - ojtaso (20061224)
	m_strRQ(_T("DEFAULT")),		// ���� RQ : �������� - ojtaso (20061224)
	m_bResetChart(FALSE),		// ��Ʈ �ʱ�ȭ���� ���񺯰� ���� ���� : �������� - ojtaso (20070621)
	m_bUseMultiItemMode(TRUE),	// ��ƮDLL���� ���������� ������� : �������� - ojtaso (20070904)
	m_bSyncPacket(FALSE),
	m_nMouseWheelIndex(0),
	m_nMinMaxRatio(0)
{
	// (2009/9/1 - Seung-Won, Bae) Define Chart Mode.
	m_nChartMode = CChartInfo::HTS;

	// (2009/9/5 - Seung-Won, Bae) KTB has ? type on default.
	m_nRealTimeTruncationType = CPacketType::CEILING;//KHD 

	// (2009/5/17 - Seung-Won, Bae) for Future Time
	m_nFutureTimeCount = 0;

	// (2009/5/13 - Seung-Won, Bae) for Right Margin
	m_nRightMarginWidth = 0;

	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	m_bLockScrollUIUpdate = FALSE;
	m_bWholeViewForLock = FALSE;
	m_nOnePageDataCountForLock = 0;
	m_nStartDrawIdxCurForLock = 0;
	m_bIsFixedIntervalForLock = FALSE;
	m_eCandleTypeOnResizeForLock = CXScaleManager::RSAT_ITEM_COUNT_FIX;

	// (2009/1/14 - Seung-Won, Bae) Support threaded drawing.
	//m_bOnPrepareOnDraw = FALSE;

	// (2009/1/13 - Seung-Won, Bae) Main Image Cache.
	m_bMainOSBFilled = FALSE;

	// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI (Zoom)
	m_bHideDrawForCalc = FALSE;

	// (2008/12/21 - Seung-Won, Bae) Manage On Real Processing
	m_bOnRealProcessing = FALSE;

	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	m_bFreezedPageState = FALSE;

	if( m_bEnableLogChartLoading) GetLocalTime( &m_stLogStart);

	InitializeIIDs(&IID_DKTBChart, &IID_DKTBChartEvents);

	m_pIChartCtrl = NULL;
	m_pAddInManager = NULL;

	// Base Class �ʱ�ȭ
	m_pPacketInput = NULL;
	m_pPacketOutput = NULL;
	m_pBaseDesk = NULL ;

	m_pMainBlock = NULL ;
	m_pPacketList = NULL ;
	m_pIndicatorList = NULL ;

	// (2003.12.23, ��¿�) Resource Manager ������ �����Ѵ�.
	memset( &m_logFont, 0, sizeof( LOGFONT));	// Log Font�� ���� �ʱ�ȭ�� OnCreate()���� ó���ȴ�.
	m_pResourceManager = NULL;
	m_pRscFont = NULL;

	// (2004.05.06, ��¿�) DLL Container�� Window Handle�� Chart�� ���� ID�� �����Ͽ� Event Notify�� �����ϵ��� �Ѵ�.
	m_hContainerDllWnd	= NULL;
	m_nID				= NULL;

	// (2004.10.22, ��¿�) Mouse Cursor�� ��� �� ��������� �����Ѵ�. (�� Cursor�� Handle�� �����Ѵ�.)
	m_hCursor = NULL;

	// ����(05/05/11) ����DLL.
	// (2006/2/8 - Seung-Won, Bae) Create from Regular DLL
	m_pToolBarMgrHelper = NULL;
	m_pToolBarManager = NULL;	// Initialize before SetClrInquiryWindow()

	// (2005/11/22 - Seung-Won, Bae) Init Default Property Value with the Value of DoPropExchange
	// (2004.11.26, ��¿�) Real�� ����Ʈ�� ����������� ��ü �ð��� ������ �������� Flag���� ���� �����Ѵ�.
	//		Ư��, ������ Global�� �Ӽ��� �ξ� HTS �⺻ ������ �����ϰ�,
	//		������ Property�� �����Ͽ� Chart�� �Ӽ��� ������� �Ѵ�.
	m_clrBackground						= _DPV_CLRBACKGROUND;
	m_clrBackBorder						= _DPV_CLRBACKBORDER;
	m_clrBlock							= _DPV_CLRBLOCK;
	m_clrBlockBorder					= _DPV_CLRBLOCKBORDER;
	m_clrScaleLine						= _DPV_CLRSCALELINE;
	m_clrScaleLine2						= _DPV_CLRSCALELINE2;
	m_clrScaleText						= _DPV_CLRSCALETEXT;
	m_clrGraphBase1						= _DPV_CLRGRAPHBASE1;
	m_clrGraphBase2						= _DPV_CLRGRAPHBASE2;
	m_clrGraphBase3						= _DPV_CLRGRAPHBASE3;
	m_clrGraphBase4						= _DPV_CLRGRAPHBASE4;
	m_clrGraphBase5						= _DPV_CLRGRAPHBASE5;
	m_clrGraphBase6						= _DPV_CLRGRAPHBASE6;
	m_clrAnalysisTool					= _DPV_CLRANALYTICSTOOL;
	m_clrInquiryWindow					= _DPV_CLRINQUIRYWINDOW;
	m_nBlockType						= _DPV_TYPEBLOCKTYPE;
	m_nBlockCompart						= _DPV_TYPEBLOCKCOMPART;
	m_nBlockRowCount					= _DPV_NBLOCKROWCOUNT;
	m_nBlockColumnCount					= _DPV_NBLOCKCOLUMNCOUNT;
	m_nBlockGap							= _DPV_NBLOCKGAP;
	m_nBlockBorderThickness				= _DPV_BLOCKBORDERTHICKNESS;
	m_nRowDoubleSizeBlock				= _DPV_NROWDOUBLESIZEBLOCK;
	m_nColumnDoubleSizeBlock			= _DPV_NCOLUMNDOUBLESIZEBLOCK;
	m_clrLattice						= _DPV_CLRRATTICE;
	m_strChartList						= _DPV_STRCHARTLIST;
	m_nBorderType						= _DPV_TYPEBORDER;
	m_bWholeView						= _DPV_BWHOLEVIEW;
	m_nOnePageDataCount					= _DPV_NONEPAGEDATACOUNT;
	m_nUserBlockAddCount				= _DPV_NUSERBLOCKADDCOUNT;
	m_bGraphDragDrop					= _DPV_BGRAPHDRAGDROP;
	m_nLeftMargin						= _DPV_NLEFTMARGIN;
	m_nRightMargin						= _DPV_NRIGHTMARGIN;
	m_nTopMargin						= _DPV_NTOPMARGIN;
	m_nBottomMargin						= _DPV_NBOTTOMMARGIN;
	m_bAddGraphMenuShow					= _DPV_BADDGRAPHMENUSHOW;
	m_strUserEnvironmentFileName		= _DPV_STRENVIRONMENTFILENAME;
	m_strUserIndicatorListFileName		= _DPV_STRUSERINDICATORLISTFILENAME;
	m_bHide								= _DPV_BHIDE;
	m_bIsInput							= _DPV_BISINPUT;
	m_strToolbarOption					= _DPV_STRTOOLBAROPTION;
	m_strPacketOption					= _DPV_STRPACKETOPTION;
	m_strLocalSharedList				= _DPV_STRLOCALSHAREDLIST;
	m_bUseRealData						= _DPV_BUSEREALDATA;
	m_strRealList						= _DPV_STRREALLIST;
	m_nMarginBorderType					= _DPV_NMAGINBORDERTYPE;
	m_bUseInquiryDlgWhenUseCrossLine	= _DPV_BUSEINQUIRYDLGWHENUSECROSSLINE;
	m_bUseShortKey						= _DPV_BUSESHORTKEY;
	m_bUseConditiontoTitle				= _DPV_BUSECONDITIONTOTITLE;
	m_bUseDatatoTitle					= _DPV_BUSEDATATOTITLE;
	m_nHorzGap							= _DPV_nHorzGap;
	m_nVertGap							= _DPV_nVertGap;
	m_nRButtonMenuType					= _DPV_nRButtonMenuType;
	m_bShowWarningMessageBox			= _DPV_bShowWarningMessageBox;
	m_bControlBlockSize					= _DPV_bControlBlockSize;
	m_bBlockMoveDelete					= _DPV_bBlockMoveDelete;
	m_bSelectTitle						= _DPV_bSelectTitle;
	m_bSelectGraph						= _DPV_bSelectGraph;
	m_bUseScaleRButtonMenu				= _DPV_bUseScaleRButtonMenu;
	m_nGraphRButtonType					= _DPV_nGraphRButtonType;
	m_nHoldingPacketSize				= _DPV_nHoldingPacketSize;
	m_nMinDataCountOnOnePage			= _DPV_nMinDataCountOnOnePage;
	m_szBasePacketName					= _DPV_STRBASEPACKETNAME;
	m_bFireRealEvent					= _DPV_bFIREREALEVENT;
	m_nNumericalInquiryOption			= _DPV_NUMERICALINQUIRYOPTION;
	m_strGlobalSharedList				= _DPV_STRGLOBALSHAREDLIST;
	m_bDoesUseSkinColor					= _DPV_USESKIN;
	m_strMapName						= _DPV_MAPNAME;
	m_nBongCountMargin					= _DPV_BONGCOUNTMARGIN;
	m_bOnePageDataCountInTRCount		= _DPV_DISPLAYTRDATA;
	m_strCSFont							= _DPV_CUSTOMFONT;
	m_strBlockRegion					= _DPV_BLOCKREGION;
	m_strAddInLoadList					= _DPV_ADDINLIST;

	m_bNormalizedMinuteTimeRule			= g_bNormalizedMinuteTimeRule;
	m_nInquiryType						= 0;			// 05.03.2002  ȯ�漳�� �߰�, ��ġ��ȸâ �̵��ɼ�. 0:�̵�, 1:����, 2:���콺���󰡱�.
	// (2004.07.30, ��¿�) ��ġ��ȸâ�� OCX �������� Ȯ��ǰ� ���� ���θ� FALSE�� �ʱ�ȭ�Ͽ� ������ ���� ����
	//		���������� �ʵ��� �Ѵ�.
	m_bOcxAreaNumericalInquiry			= FALSE;
	m_nUseInquiryWithCrossline			= 0;			// (2004.10.20, ��¿�) �⺻������ ��ġ��ȸâ�� �������� �����ǵ��� �Ѵ�.
	m_bShowPriceRateInquiry				= TRUE;			// (2004.10.13, ��¿�) ��ġ��ȸâ�� ������� �������� �����ϴ� Property�� �߰��Ѵ�.

	m_bShowTrendLineText = TRUE;
	m_bShowVerticalText = TRUE;
	m_bShowHorizonTextLeft = TRUE;		// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
	m_bShowHorizonTextRight = TRUE;
	m_bShowThreeText = TRUE;
	m_bShowFourText = TRUE;
	m_bShowFiboText = TRUE;

	//nami 11.22
	m_strOrgBlocksPosition = "";
	m_bShowMinMaxbyPrice = FALSE;
	// ����(04/11/24) RButton�޴��� ��ǥŸ��Ʋ ���߱� �ɼ� ����.
	m_bAllGraphTitleHiding = false;

	m_strItemCode = "";
	m_strItemName = "";
	m_strCode = "";

	// first draw
	m_bFirstDraw = FALSE;

	//////////////////////////////////////////////////////////////////
	// VER 1.0.0.2
	// User Margin
	//m_nUserMargin = 4;

	// event wait
	m_bWaitRequest = FALSE;

	// 04.03.2002 ���� ��ġ��ȸâ�� ������ �ִ��� Ȯ��
	m_bUseInquiryData = FALSE;

	//7.9
	m_bOnAdvise = FALSE;
	m_bConnect = FALSE;
	//nami 8.16
	m_strUserID = "";

	//nami 9.10
	m_bToolTipwithZoomIn = TRUE;

	//yousun 10.15
	m_bRadar = FALSE;

	//�̺�Ʈ
	m_lFlagEvent = 0xffffffff;	// �ʱ�� ��� �����Ѵ�.

	// (2004.01.09, ��¿�) Ư���� ����� Map������ Packet Count�� 5�ڸ��� Ȯ��� �� �ֵ��� �����Ѵ�.
	//		�� ���� Property nPacketCountDataSize�� ���� �ʱ� Script���� �����ϴ� ������ �̿��Ѵ�.
	//		���� Property Page�� �����ϰų� AnyBuilder�� Property ����, XFM File��� ������� �ʴ´�.
	m_nPacketCountDataSize = 5;

	m_nGlanceBalanceMargin = 0;

	// (2004.05.25, ��¿�) ���� ToolBar�� ���̰�, ����� Interface�� �����Ѵ�.
	//		������ Flag�� �ξ�, ToolBar�� OCX�� �������� �Ʒ��ʿ� ��ġ�ǵ��� �Ѵ�.
	m_bShowInnerToolBar = TRUE;

	// (2004.09.06, ��¿�) OCX Counter�� �ξ�, ������ OCX�� ����ÿ� User Default ������ �����Ѵ�.
	m_nChartOcxCnt++;

	// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �����Ѵ�.
	m_nMouseCaptureCount = 0;
	// (2006/2/6 - Seung-Won, Bae) Manage Mouse DownUp Count for AddIn's Mouse Action ID
	m_dwMouseActionID = 0;

	// (2004.11.02, ��¿�) �� Mouse ��ġ�� Chart Block�� ������ ó���ϴ� Popup Menu�� �����Ѵ�.
	//		OnUpdateReverseYscale()���� Ȯ���� Block ������ �����Ͽ�, OnReverseYscale()�� �̿���� �Ѵ�.
	m_bReverseYScale_ReverseChart	= FALSE;
	m_biReverseYScale_BlockIndex	= CBlockIndex( -1, -1);

	// ����(04/11/16) MaxRange���� �����Ѵ�. ���ٰ� ���� ��쿡 (SetScrollData_NoneToolBar()�� ����) 
	// ���� MaxRange���� �����ϴ� �����̴�.
	m_nMaxRange = 0;

	// (2005/2/17 - Seung-Won, Bae) Point Data for Dragging Notify to DLL Continer
	m_ptLDownPosition.x = LONG_MIN;
	m_ptLDownPosition.y = LONG_MIN;
	
	//sy 2005.04.26.
	CHART_SKIN_NAME = "chart_style";
	//sy end

	// ����(05/05/10) Hts�� �о��� ������ �ִ´�.
	m_strHtsPath = _T("");
	
	m_bRunTimeMode = TRUE;	// ��Ʈ�� RunTime���. (TRUE: RunTimeMode, FALSE: DesignTimeMode)

	// (2004.11.30, ��¿�) Real Setting Info, DRDS Advised Info, Local Share Info�� �����Ͽ� �����Ѵ�.
	m_plLocalShareInfoList	= new CLocalShareInfoList();

	//sy 2005.11.09.
	m_bIncludeBongCountMargin = true;

	// (2006/1/23 - Seung-Won, Bae) Manage Log Flag. It Managed by IChartCtrl in ChartLogAddIn
	m_bInLogging = FALSE;

	// (2006/7/5 - Seung-Won, Bae) Manage the flag of Full GetAllProperties3()
	m_bFullGetAllProperties3 = FALSE;

	// (2006/7/25 - Seung-Won, Bae) Use Real Module with AddIn
	m_pIRealReceiver = NULL;

	//sy 2004.07.16. packet �� ������ ��ŭ scroll ����
	// (2006/7/26 - Seung-Won, Bae) Manage Removed Packet Data Count for Reset Scroll in OnDrdsPacketEnd().
	m_nRemovePacketDataCount = 0;

	// (2006/7/31 - Seung-Won, Bae) Manage AddIn Reload Flag of SetAllProperties3 for AddIn
	m_bSetAllProperties3WithReloadAddIn = TRUE;

	// (2006/8/23 - Seung-Won, Bae) Manage AddIn Load String
	if( !m_mapAddInLoadString.GetCount()) for( int i = 0; i < AID_COUNT; i++)
		m_mapAddInLoadString.SetAt( g_aszAddInItemName[ i], g_aszAddInLoadString[ i]);

	// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
	m_bShowPriceMinMaxText = TRUE;

	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	m_bMultipleSameNameIndicator = FALSE;

	// (2007/1/25 - Seung-Won, Bae) Lock Window Update.
	m_bLockWindowUpdate = FALSE;

	// (yiilyoul@magicn.com - 20070202) X-ScaleManager
	m_pxScaleManager = NULL;

	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	m_lDrawingLoadTime = 0;

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	for Old Version Map and User File.
	m_nScaleLineType	= -1;
	m_nVertScaleType	= -1;
	m_nHorzScaleType	= -1;

	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	m_strSTExtPacketName = _T("");

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
	m_nLanguageID = IMetaTable::KOREAN;

	// (2008/3/7 - Seung-Won, Bae) Backup R-Button Menu popup Position for ON_UPDATE_COMMAND_UI and ON_COMMAND
	m_ptRButtonMenuPopupPosition = CPoint( -1, -1);

	// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
	CString strEnvValue;
	GetTestEnvValue( "Log AddIn Enable",		strEnvValue);
	m_bEnableLogAddIn				= atoi( strEnvValue);
	GetTestEnvValue( "Log SaveAndOpenFromFile",	strEnvValue);
	m_bEnableLogSaveAndOpenFromFile	= atoi( strEnvValue);
	GetTestEnvValue( "Log Chart Loading",		strEnvValue);
	m_bEnableLogChartLoading		= atoi( strEnvValue);
	GetTestEnvValue( "Log SetGetAll",			strEnvValue);
	m_bEnableLogSetGetAll			= atoi( strEnvValue);

	if( m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\n",
			m_stLogStart.wHour, m_stLogStart.wMinute, m_stLogStart.wSecond, m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds);
		CDumpLogger::LogStringToFile( "CKTBChartCtrl::CKTBChartCtrl()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}		
}

/////////////////////////////////////////////////////////////////////////////
// CKTBChartCtrl::~CKTBChartCtrl - Destructor

CKTBChartCtrl::~CKTBChartCtrl()
{
	// (2004.09.06, ��¿�) OCX Counter�� �ξ�, ������ OCX�� ����ÿ� User Default ������ �����Ѵ�.
	m_nChartOcxCnt--;
	if(g_pDRDebuger!=NULL) delete g_pDRDebuger; g_pDRDebuger = NULL;

	//	Base Class
	if(m_pPacketInput)  delete m_pPacketInput;
	if(m_pPacketOutput) delete m_pPacketOutput;
	m_pPacketInput = NULL ;
	m_pPacketOutput = NULL ;

	m_SaveChartcfg.RemoveAll();

	// PacketData -> PacketList
	// m_pPacketListManager���� ����  : �������� - ojtaso (20070109)
/*
	if(m_pPacketList != NULL)
		delete m_pPacketList;
*/
	
	// ���������� ���� PacketListManager�� ���� : �������� - ojtaso (20061224)
	if(m_pPacketListManager)
	{
		delete m_pPacketListManager;
		m_pPacketListManager = NULL;
	}

	// IndicatorCondition -> IndicatorList
	if(m_pIndicatorList != NULL)
	{
		//sy 2005.06.23. ��ȭ�� desk�� ���⶧���� �����Ѵ�.
		// (2004.09.06, ��¿�) OCX Counter�� �ξ�, ������ OCX�� ����ÿ� User Default ������ �����Ѵ�.
		// (2004.09.16, ��¿�) Design Time���� �������� �ʵ��� �Ѵ�.
		// (2004.09.16, ��¿�) Test Container������ �������� �ʵ��� �Ѵ�. (DESK�� �ִ��� Ȯ���Ѵ�.)
		//if( !m_nChartOcxCnt && IsRunTimeMode() && m_pBaseDesk)
		if( !m_nChartOcxCnt && IsRunTimeMode())
			m_pIndicatorList->SaveIndicatorInfo( EILT_USER_DEFAULT, GetUserDefaultFilePathAndName(FILE_INDICATOR_CONDITIONS));

		delete m_pIndicatorList ;
	}

	// (2003.12.23, ��¿�) Resource Manager ������ �����Ѵ�.
	// 1. Font Key�� Size�� 0�� �ƴϸ�, Resource Manager�� ���� Font�� ������ ���̴�.
	if( m_FontKey.GetSize())
	{
		if( m_pResourceManager) m_pResourceManager->UnAdviseFont( m_FontKey);
	}
	// 2. Font Key�� Size�� 0�̸�, ��ü������ ������ Font�̴�.
	else if( m_pRscFont)
	{
		m_pRscFont->DeleteObject();
		delete m_pRscFont;
		m_pRscFont = NULL;
	}

	//nami 8.31
	DeleteZoomInOutInquiryDlg();
//	m_ZoomInOutDataDlg = NULL;

	// (2006/2/20 - Seung-Won, Bae) Delete ToolBar in DestroyWindow() for Window Destroying
	// ����(05/05/11) ����DLL ����
	if(m_pToolBarManager != NULL)
	{
		delete m_pToolBarManager;
		m_pToolBarManager = NULL;
	}
	// (2006/2/8 - Seung-Won, Bae) Create from Regular DLL
	if( m_pToolBarMgrHelper)
	{
		delete m_pToolBarMgrHelper;
		m_pToolBarMgrHelper = NULL;
	}

	// (2004.11.30, ��¿�) ������ Real Setting Info, DRDS Advised Info, Local Share Info�� �����Ѵ�.
	if( m_plLocalShareInfoList)
	{
		m_plLocalShareInfoList->RemoveAllLocalShareInfo();
		delete m_plLocalShareInfoList;
		m_plLocalShareInfoList = NULL;
	}

	// (2004.10.08, ��¿�) AddIn Manager�� Ocx Pointer�� ����Ͽ� Ocx Interface Wrapper�� ������ �� �ֵ��� �����Ѵ�.
	if( m_pAddInManager)
	{
		delete m_pAddInManager;
		m_pAddInManager = NULL;
	}
	if( m_pIChartCtrl)
	{
		delete m_pIChartCtrl;
		m_pIChartCtrl = NULL;
	}

	// (2007/6/25 - Seung-Won, Bae) Support Dynamic Loading of ChartObject.dll
	if( m_hMainBlock)
	{
		FreeLibrary( m_hMainBlock);
		m_hMainBlock = NULL;
	}

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	if( !m_nChartOcxCnt) g_iMetaTable.UnloadAllResourceDLL();	

}

/////////////////////////////////////////////////////////////////////////////
// CKTBChartCtrl::OnDraw - Drawing function

// (2009/1/14 - Seung-Won, Bae) Support threaded drawing.
// 	UINT CKTBChartCtrl_PrepareOnDraw( LPVOID pParam)
// 	{
// 		CKTBChartCtrl *pThis = ( CKTBChartCtrl *)pParam;
// 
// 		// �׸��� ���� �ʿ��� data�� ���ϱ�.
// 		if( pThis->m_pxScaleManager) pThis->m_pxScaleManager->Calc_AREA( pThis->GetColumnBlockCount());
// 
// 		pThis->m_bOnPrepareOnDraw = FALSE;
// 
// 		return 0;
// 	}

void CKTBChartCtrl::OnDraw( CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if( rcInvalid.IsRectEmpty()) return;
	if( m_bEnableLogChartLoading) GetLocalTime( &m_stLogStart);

	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	DWORD dwStartTime = timeGetTime();

	//	bitmap create
	// (2004.12.17, ��¿�) ��Ʈȭ���� ���� Open�ϴ� ���, Resource �������� Exception�� �߻��� �� �ִ�.
	//		Memory DC�� ����Ȯ������ �̸� �����ϵ��� �Ѵ�.
	CDC memoryDC;
	if( !memoryDC.CreateCompatibleDC(pdc)) return;
	// (2003.12.23, ��¿�) Resource Manager ������ �����Ѵ�.
	CFont* pOldFont = memoryDC.SelectObject( m_pRscFont);
	if( !m_bLockWindowUpdate) CreateBufferBitmap(pdc, rcBounds);
	CBitmap* pOldbm = memoryDC.SelectObject(&m_bmpMainOSB);
	// (2008/12/26 - Seung-Won, Bae) to optimize.
	if( m_pMainBlock && m_pxScaleManager && !m_bLockWindowUpdate)
	{
		// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
		if( m_pxScaleManager->IsScrollByReal()) m_pxScaleManager->Calc_DRI_Merge();

		// (2009/9/5 - Seung-Won, Bae) avoid to loop of recursive.
		if( !m_dcThread.GetSafeHdc())
		{
			// (2009/1/7 - Seung-Won, Bae) Do not use main DC to avoid collision with main thread DC.
			m_dcThread.CreateCompatibleDC( &memoryDC);
			CFont* pFont = m_dcThread.SelectObject( m_pRscFont);

			m_rcMain = GetOcxRegion();
			DrawChart( memoryDC, m_dcThread, m_rcMain, TRUE);

			m_dcThread.SelectObject( pFont);
			m_dcThread.DeleteDC();

			// (2009/1/13 - Seung-Won, Bae) Main Image Cache.
			m_bMainOSBFilled = TRUE;
		}
	}
	// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI (Zoom)
	if( !m_bHideDrawForCalc) pdc->BitBlt( m_rcMain.left, m_rcMain.top, m_rcMain.Width(), m_rcMain.Height(), &memoryDC, 0, 0, SRCCOPY) ;
	memoryDC.SelectObject( pOldFont);
	memoryDC.SelectObject( pOldbm);
	memoryDC.DeleteDC();

	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	if( m_pAddInManager)
	{
		m_pAddInManager->ClearCursor();
		CFont *pOldFont2 = pdc->SelectObject( m_pRscFont);
		m_pAddInManager->DrawCursor( pdc);
		pdc->SelectObject( pOldFont2);
	}

	// ����(05/08/16) ���� OnDraw
	//if(m_pToolBarManager) m_pToolBarManager->OnDraw();

	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	DWORD dwEndTime = timeGetTime();
	m_lDrawingLoadTime = dwEndTime - dwStartTime;

	if( m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		long lMSec = ( stEnd.wSecond * 1000 + stEnd.wMilliseconds) - ( m_stLogStart.wSecond * 1000 + m_stLogStart.wMilliseconds);
		if( lMSec < 0) lMSec += 60 * 1000;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\nSpend ms : %ld",
			m_stLogStart.wHour, m_stLogStart.wMinute, m_stLogStart.wSecond, m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds, lMSec);
		CDumpLogger::LogStringToFile( "CKTBChartCtrl::OnDraw()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}
}
void CKTBChartCtrl::DrawChart( CDC &p_memoryDC, CDC &p_dcThread, CRect rect, BOOL p_bRealDraw)
{
	m_pMainBlock->ChangeDrawStyle( CGraphBaseData::Bong_Type);
	m_pMainBlock->CalculateDataForDrawing( &p_dcThread);
	// (2009/1/14 - Seung-Won, Bae) Support threaded drawing.
	//m_bOnPrepareOnDraw = TRUE;
	//AfxBeginThread( CKTBChartCtrl_PrepareOnDraw, this);
	if( m_pxScaleManager) m_pxScaleManager->Calc_AREA( GetColumnBlockCount());

	m_pBackgroundDraw.DrawBackground( &p_memoryDC,
					rect,					// �׸� background ���� 
					1,						// border ����
					m_clrBackground,		// background ��
					m_clrBackBorder,		// block ��
					m_nBorderType);			// border Ÿ��

	// ����� ��輱�� �ִ� ��� ���� ���� ��輱 �׸��� ��ƾ
	if( GetVirtualRegion() != GetMainBlockRegion() && m_nMarginBorderType < 3 && p_bRealDraw)
	{
		m_pBackgroundDraw.DrawBackground( &p_memoryDC,
						GetVirtualRegion(),		// �׸� background ���� 
						1,						// border ����
						m_clrBackground,		// background ��
						m_clrBackBorder,		// block ��
						m_nMarginBorderType);			// border Ÿ��
	}

	// (2004.09.14, ��¿�) �ð��������� ���� �� ����� �ǵ��� �Ѵ�.
 	m_pMainBlock->DrawBlocks( &p_memoryDC, CDrawingOption::BACKGROUND, TRUE);

	// (2009/1/14 - Seung-Won, Bae) Support threaded drawing.
	//while( m_bOnPrepareOnDraw) Sleep( 1);
	
	if(m_pxScaleManager->IsUseable() == TRUE || !IsRunTimeMode())
	{
		m_pMainBlock->DrawBlocks( &p_memoryDC, CDrawingOption::SCALE, FALSE);

		// (2004.10.11, ��¿�) Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
		if( m_pAddInManager) m_pAddInManager->OnDrawBeforeGraph( &p_memoryDC);

		// 2009.01.03 �輺�� : ��Ʈ ���� ���� (�׷��� ���, �׸��� �۾����� �и�)
		m_pxScaleManager->SetThreadFlag( THREAD_CALCULATION);
		m_pMainBlock->DrawBlocks( &p_dcThread, CDrawingOption::GRAPH, FALSE);

		m_pxScaleManager->SetThreadFlag( THREAD_DRAWING);
		m_pMainBlock->DrawBlocks( &p_memoryDC, CDrawingOption::GRAPH, FALSE);			

		// (2004.12.25, ��¿�) Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
		if( m_pAddInManager) m_pAddInManager->OnDrawAfterGraph( &p_memoryDC);
	}

	m_pMainBlock->ChangeDrawStyle(CGraphBaseData::Line_Type);
}

/////////////////////////////////////////////////////////////////////////////
// CKTBChartCtrl::OnResetState - Reset control to default state

void CKTBChartCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CKTBChartCtrl message handlers

void CKTBChartCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here

	// (2006/7/4 - Seung-Won, Bae) Notify to AddIn
	if( m_pAddInManager) m_pAddInManager->OnContextMenu( pWnd, point);

	// (2006/3/23 - Seung-Won, Bae) for Parent Windows Context Menu.
	if(IsRunTimeMode() && m_bAddGraphMenuShow == 0) COleControl::OnContextMenu(pWnd, point);
}

/////////////////////////////////////////////////////////////////////////////
//	Properties setting

OLE_COLOR CKTBChartCtrl::GetClrBackground() 
{
	return m_clrBackground ;
}

void CKTBChartCtrl::SetClrBackground(OLE_COLOR nNewValue) 
{
	m_clrBackground = nNewValue ;
	InvalidateControl() ;
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrBackBorder() 
{
	return m_clrBackBorder ;
}

void CKTBChartCtrl::SetClrBackBorder(OLE_COLOR nNewValue) 
{
	m_clrBackBorder = nNewValue ;
	InvalidateControl() ;
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrBlock() 
{
	return m_clrBlock;
}

void CKTBChartCtrl::SetClrBlock(OLE_COLOR nNewValue) 
{
	m_clrBlock = nNewValue ;
	InvalidateControl();
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrBlockBorder() 
{
	return m_clrBlockBorder ;
}

void CKTBChartCtrl::SetClrBlockBorder(OLE_COLOR nNewValue) 
{
	m_clrBlockBorder = nNewValue ;
	InvalidateControl();
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrScaleLine() 
{
	return m_clrScaleLine ;
}

void CKTBChartCtrl::SetClrScaleLine(OLE_COLOR nNewValue) 
{
	m_clrScaleLine = nNewValue ;
	InvalidateControl();
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrScaleText() 
{
	return m_clrScaleText;
}

void CKTBChartCtrl::SetClrScaleText(OLE_COLOR nNewValue) 
{
	m_clrScaleText = nNewValue ;
	InvalidateControl();
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrGraphBase1() 
{
	return m_clrGraphBase1;
}

void CKTBChartCtrl::SetClrGraphBase1(OLE_COLOR nNewValue) 
{
	m_clrGraphBase1 = nNewValue ;
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrGraphBase2() 
{
	return m_clrGraphBase2;
}

void CKTBChartCtrl::SetClrGraphBase2(OLE_COLOR nNewValue) 
{
	m_clrGraphBase2 = nNewValue ;
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrGraphBase3() 
{
	return m_clrGraphBase3;
}

void CKTBChartCtrl::SetClrGraphBase3(OLE_COLOR nNewValue) 
{
	m_clrGraphBase3 = nNewValue ;
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrGraphBase4() 
{
	return m_clrGraphBase4;
}

void CKTBChartCtrl::SetClrGraphBase4(OLE_COLOR nNewValue) 
{
	m_clrGraphBase4 = nNewValue ;
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrGraphBase5() 
{
	return m_clrGraphBase5;
}

void CKTBChartCtrl::SetClrGraphBase5(OLE_COLOR nNewValue) 
{
	m_clrGraphBase5 = nNewValue ;
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrGraphBase6() 
{
	return m_clrGraphBase6;
}

void CKTBChartCtrl::SetClrGraphBase6(OLE_COLOR nNewValue) 
{
	m_clrGraphBase6 = nNewValue ;

	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrAnalysisTool() 
{
	return m_clrAnalysisTool;
}

void CKTBChartCtrl::SetClrAnalysisTool(OLE_COLOR nNewValue) 
{
	m_clrAnalysisTool = nNewValue ;
	SetModifiedFlag();

	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR);
}

OLE_COLOR CKTBChartCtrl::GetClrScaleLine2() 
{
	return m_clrScaleLine2 ;
}

void CKTBChartCtrl::SetClrScaleLine2(OLE_COLOR nNewValue) 
{
	m_clrScaleLine2 = nNewValue ;
	InvalidateControl() ;
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrInquiryWindow() 
{
	return m_clrInquiryWindow;
}

void CKTBChartCtrl::SetClrInquiryWindow(OLE_COLOR nNewValue) 
{
	m_clrInquiryWindow = nNewValue;	
	SetModifiedFlag();

	// (2006/6/27 - Seung-Won, Bae) Numerical Inquiry AddIn Tool.
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_NIGLG_BACKROUND_CLOLOR_CHANGED, m_clrInquiryWindow);
}

void CKTBChartCtrl::OnnBlockTypeChanged() 
{
	BoundPropertyChanged(dispidNBlockType);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnnBlockCompartChanged() 
{
	BoundPropertyChanged(dispidNBlockCompart) ;
	SetModifiedFlag();
}

void CKTBChartCtrl::OnnBlockRowCountChanged() 
{
	BoundPropertyChanged(dispidNBlockRowCount);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnnBlockColumnCountChanged() 
{
	BoundPropertyChanged(dispidNBlockColumnCount);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnnBlockGapChanged() 
{
	BoundPropertyChanged(dispidNBlockGap);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnnBlockBorderThicknessChanged() 
{
	BoundPropertyChanged(dispidNBlockBorderThickness);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnnBorderTypeChanged() 
{
	BoundPropertyChanged(dispidNBorderType);

	SetModifiedFlag();
}

void CKTBChartCtrl::OnbWholeViewChanged() 
{
	BoundPropertyChanged(dispidBWholeView);

	SetModifiedFlag();
}

void CKTBChartCtrl::OnnUserBlockAddCountChanged() 
{
	BoundPropertyChanged(dispidNUserBlockAddCount);

	SetModifiedFlag();
}

void CKTBChartCtrl::OnbGraphDragDropChanged() 
{
	BoundPropertyChanged(dispidBGraphDragDrop);

	SetModifiedFlag();
}

void CKTBChartCtrl::OnnLeftMarginChanged() 
{
	BoundPropertyChanged(dispidNLeftMargin);

	SetModifiedFlag();
}

void CKTBChartCtrl::OnnRightMarginChanged() 
{
	BoundPropertyChanged(dispidNRightMargin);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnnTopMarginChanged() 
{
	BoundPropertyChanged(dispidNTopMargin);

	SetModifiedFlag();
}

void CKTBChartCtrl::OnnBottomMarginChanged() 
{
	BoundPropertyChanged(dispidNBottomMargin);

	SetModifiedFlag();
}

void CKTBChartCtrl::OnbAddGraphMenuShowChanged() 
{
	BoundPropertyChanged(dispidBAddGraphMenuShow);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnstrUserEnvironmentFileNameChanged() 
{
	BoundPropertyChanged(dispidStrUserEnvironmentFileName);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnstrUserIndicatorListFileNameChanged() 
{
	BoundPropertyChanged(dispidStrUserIndicatorListFileName);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnbHideChanged() 
{
	BoundPropertyChanged(dispidBHide);
	if(IsRunTimeMode()){
		if(m_bHide)	{
			ShowWindow(SW_HIDE);
			// REMOVE: ������(04/02/26)
//			OffDrdsAdvise();
		}
		else
			ShowWindow(SW_SHOW);
	}
	SetModifiedFlag();
}

void CKTBChartCtrl::OnbIsInputChanged() 
{
	BoundPropertyChanged(dispidBIsInput);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnstrToolbarOptionChanged() 
{
	BoundPropertyChanged(dispidStrToolbarOption);

	_ML_TEST_NOT_USED();
	if(m_strToolbarOption.Right(2) != "\r\n")
		m_strToolbarOption += "\r\n";
	
	CString strToolBarOptionTmp = m_strToolbarOption;
	CString strInner = CDataConversion::GetStringData(strToolBarOptionTmp, CString( _MTEXT( C4_OUTER_TOOL_OPTION)) + "\r\n");
	int nCount = CDataConversion::GetStringDataCount(strInner, _T("\r\n"));
	nCount--;	// "����"��� ���ڿ� ���� ����� �� ������.

	// nCount�� 0���� ������ ����.
	if(nCount <= 0)
	{
		if(m_pToolBarManager != NULL)
		{
			delete m_pToolBarManager;
			m_pToolBarManager = NULL;
		}
	}
	else
	{
		// (2009/9/2 - Seung-Won, Bae) for Image Path
		//CString strImageFile = GetImagePath() + TOOLBAR_BUTTON_IMAGEFILE;
		//CString strImageFile = GetBaseDeskPath() + _T("\\Hanaro\\image\\drchart\\") + TOOLBAR_BUTTON_IMAGEFILE; //@Solomon:Directory-091113
		CString strImageFile = GetBaseDeskPath() + _T("\\Icss\\image\\drchart\\") + TOOLBAR_BUTTON_IMAGEFILE;	//IBK

		// (2006/2/8 - Seung-Won, Bae) Create from Regular DLL
		if( !m_pToolBarMgrHelper) m_pToolBarMgrHelper = new CToolBarMgrHelper;
		if( m_pToolBarMgrHelper && !m_pToolBarManager) m_pToolBarManager = m_pToolBarMgrHelper->CreateToolBarManager( m_pIChartCtrl);
		if( !m_pToolBarManager) return;

		m_pMainBlock->SetToolBarManager(m_pToolBarManager);

		m_pToolBarManager->SetControlData(m_pPacketList, m_pPacketListManager, m_pxScaleManager, m_pMainBlock, m_pIndicatorList);
		m_pToolBarManager->CreateToolBar(m_strToolbarOption, strImageFile);

		// ����(04/11/24) �׷�����Ÿ���� �ʱⰪ�� �����Ѵ�. (��Ÿ������)
		m_pToolBarManager->SetGraphType(CGraphBaseData::Bong_Type, 0);
	}
	SetModifiedFlag();
}

void CKTBChartCtrl::OnstrPacketOptionChanged() 
{
	_ML_TEST_NOT_USED();

	// CPacketList�� �����ϴ� �κ��� CPacketListManager�� ���ؼ����߸� �Ѵ�
	// �̺κ� �Լ��� ���ȴٸ� CPacketListManager�� ����ϴ°����� �����ؾ�����
	// ������� �� �Լ��� ���Ǵ��� �� �� �����Ƿ� ������ �޼����� �� ����� ������
	// �׶��� ������� ����� �����ϵ��� �Ѵ�
	// �̷��� �ϱ�� �Ѱ��� ��¿������ �����ð������ ���ǿ� ���Ѱ��� (20070321)
	ML_SET_LANGUAGE_RES();
	CString strMsg;
	strMsg.LoadString( IDS_CALL_CHART_MANAGER);
	AfxMessageBox( strMsg);

	BoundPropertyChanged(dispidStrPacketOption);
	if(!m_strPacketOption.IsEmpty() && IsRunTimeMode() && m_pPacketList == NULL)
	{
		m_pPacketList = new CPacketList( m_strPacketOption, ( CPacketType::REALTYPE)m_nRealTimeTruncationType, m_pIChartCtrl);
		m_pPacketList->SetUseRealData(m_bUseRealData == FALSE ? FALSE : TRUE);
	}
	SetModifiedFlag();
}

BSTR CKTBChartCtrl::GetStrBlockRegion() 
{
	// (2006/12/9 - Seung-Won, Bae) Reset Block
	if( m_pMainBlock) m_strBlockRegion = m_pMainBlock->GetBlocksPosition();
	return m_strBlockRegion.AllocSysString();
}

void CKTBChartCtrl::SetStrBlockRegion(LPCTSTR lpszNewValue) 
{
	m_strBlockRegion = lpszNewValue ;	
	if( m_strBlockRegion.Right( 2) != "\r\n") m_strBlockRegion += "\r\n";

	// (2006/12/9 - Seung-Won, Bae) Reset Block
	if( m_pMainBlock)
	{
		m_pMainBlock->SetBlocksPosition( m_strBlockRegion);
		InvalidateControl();
	}

	SetModifiedFlag();
}

void CKTBChartCtrl::OnNMarginBorderTypeChanged() 
{
	BoundPropertyChanged(dispidNMarginBorderType);
//	InvalidateControl();
	SetModifiedFlag();
}

void CKTBChartCtrl::OnNRowDoubleSizeBlockChanged() 
{
	BoundPropertyChanged(dispidNRowDoubleSizeBlock);
	if(m_pMainBlock)	{
		m_pMainBlock->SetDoubleSizeBlockNumber(CBlockIndex(m_nRowDoubleSizeBlock, m_nColumnDoubleSizeBlock));
		m_strBlockRegion = m_pMainBlock->GetBlocksPosition();
		InvalidateControl();
	}

	SetModifiedFlag();
}

void CKTBChartCtrl::OnNColumnDoubleSizeBlockChanged() 
{
	BoundPropertyChanged(dispidNColumnDoubleSizeBlock);

	if(m_pMainBlock)	{
		m_pMainBlock->SetDoubleSizeBlockNumber(CBlockIndex(m_nRowDoubleSizeBlock, m_nColumnDoubleSizeBlock));
		m_strBlockRegion = m_pMainBlock->GetBlocksPosition();
		InvalidateControl();
	}

	SetModifiedFlag();
}

void CKTBChartCtrl::OnBUseInquiryDlgWhenUseCrossLineChanged() 
{
	BoundPropertyChanged(dispidBUseInquiryDlgWhenUseCrossLine);

	SetModifiedFlag();
}

//sy test
BOOL CKTBChartCtrl::SetAllPropertiesFromFile(LPCTSTR strFilePath, LPCTSTR strFileName) 
{
	CStringList proStrList;
	if(!CPropertyFileBuilder().GetPropertyFileData(GetFilePathAndName(strFilePath, strFileName), proStrList))
		return FALSE;

	return TRUE;
}

CString CKTBChartCtrl::GetFilePathAndName(const CString& strFilePath, const CString& strFileName)
{
	if(strFileName.IsEmpty())
		return "";

	// �⺻ file ��ΰ� ���� ���� default ��θ� ����Ѵ�. 
	if(strFilePath.IsEmpty())
		return GetDefaultFilePathAndName(strFileName);

	if(strFilePath.Find(":") > 0)
		return (strFilePath + strFileName);

	// hts ��ο� �ش� ��ġ�� ����Ѵ�.
	CString strFilePathAndName = GetBaseDeskPath();
	if(strFilePathAndName.IsEmpty())
		return "";

	if(strFilePath.Left(1) != '\\')
		strFilePathAndName += ("\\" + strFilePath);
	if(strFilePathAndName.Right(1) != '\\')
		strFilePathAndName += '\\';

	return (strFilePathAndName + strFileName);
}
//sy end

////////////////////////////////////////////////////////////////////////////////////////////////
//	Desk����
CString CKTBChartCtrl::GetBaseDeskPath()
{
	// (2006/12/10 - Seung-Won, Bae) Support MapGen.
	if( m_strHtsPath.IsEmpty() && !IsRunTimeMode())
	{
		char tmp1[128], tmp2[128] ;
		::GetPrivateProfileString("CURRENT","WORK","",tmp1,sizeof(tmp1),"Hanaro.ini");
		::GetPrivateProfileString(tmp1,"PATH","",tmp2,sizeof(tmp2),"Hanaro.ini");
		m_strHtsPath = tmp2 ;
		// (2007/2/16 - Seung-Won, Bae) Trim Right Backslash..
		if( m_strHtsPath.Right( 1) == "\\") m_strHtsPath = m_strHtsPath.Left( m_strHtsPath.GetLength() - 1);
	}

	return m_strHtsPath;
}

HWND CKTBChartCtrl::GetBaseDeskHWnd()
{
	if(m_pBaseDesk)
		return m_pBaseDesk->GetDeskHwnd() ;

	return NULL ;
}

LPUNKNOWN CKTBChartCtrl::GetDeskUnkownID()
{
	if(m_pBaseDesk)
		return m_pBaseDesk->GetUnknown() ;

	return NULL ;
}

LPUNKNOWN CKTBChartCtrl::GetCtrlUnkownID(LPCTSTR szName)
{
	if(m_pBaseDesk)
		return m_pBaseDesk->GetCtrlUnknown(szName) ;

	return NULL ;
}

void CKTBChartCtrl::SetBaseDesk(long lKey) 
{
	m_pBaseDesk = ( CBaseDesk2 *)lKey ;
	if(m_pBaseDesk == NULL)
		return;

	// (2004.06.08, ��¿�) Global ���� ����� �߰��Ѵ�.
	if( m_pBaseDesk)
	{
		//Global ���� Setting...
		m_xGSharedCli.m_pGSharedSvr = (CDeskSharedServer*)m_pBaseDesk->GetSharedInterface();
		m_xGSharedCli.AddSharedNameToServer();

		// (2006/2/27 - Seung-Won, Bae) Manage m_strHtsPath in AnyTime.
		m_strHtsPath = m_pBaseDesk->GetBasePath();

		// (2007/2/16 - Seung-Won, Bae) Trim Right Backslash..
		if( m_strHtsPath.Right( 1) == "\\") m_strHtsPath = m_strHtsPath.Left( m_strHtsPath.GetLength() - 1);
	}
}

long CKTBChartCtrl::GetBase(LPCTSTR szKey) 
{
	if(strcmp(szKey, "PACKIN") == 0) 		return (long) m_pPacketInput;
	else if(strcmp(szKey,"PACKOUT") == 0) 	return (long) m_pPacketOutput;

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	Window Message

int CKTBChartCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( m_bEnableLogChartLoading) GetLocalTime( &m_stLogStart);

	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

// 	char aData[8+1]={0,};
// 	sprintf(aData, "%08X", m_hWnd);
// 	::WritePrivateProfileString("CHARTWND", "HANDLE", aData, "c:\\DRLOG.ini");

	//if(g_pDRDebuger==NULL) g_pDRDebuger = new CDRDebugerLoader();

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = GetSafeHwnd();

	// (2005/11/17 - Seung-Won, Bae) Set RunTimeMode Flag Manually before Design Mode Setting below
	SetRunTimeMode( AmbientUserMode());

	//Base Class create
	//sy 2003.10.20.
	if(m_pPacketInput == NULL)
		m_pPacketInput = new CNeoChart2001In(this);
	if(m_pPacketOutput == NULL)
		m_pPacketOutput = new CNeoChart2001Out(this);

	if( m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\n",
			m_stLogStart.wHour, m_stLogStart.wMinute, m_stLogStart.wSecond, m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds);
		CDumpLogger::LogStringToFile( "CKTBChartCtrl::OnCreate()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}

	return 0;
}

void CKTBChartCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	COleControl::OnWindowPosChanged(lpwndpos);

	if(m_pToolBarManager)
		m_pToolBarManager->ToolBarPosChanged(GetToolBarRegion());
}

void CKTBChartCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);

	if(m_pMainBlock != NULL)	{
		if(m_pMainBlock->SetAllBlockRegion(GetMainBlockRegion()))
		{
			InvalidateControl();
		}
	}
}

LRESULT CKTBChartCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// (2004.05.06, ��¿�) Container DLL���� Event�� �����Ѵ�.
	if( m_hContainerDllWnd) SendEventMsgToDLL( message, wParam, lParam);

	if(message == RMSG_LSHAREDCHANGED)
		return Rmsg_LSharedChaned(wParam, lParam);

	// (2004.06.08, ��¿�) Global ���� ����� �߰��Ѵ�.
	else if(message == RMSG_FORMINITIALCOMPLETED)
		return RmsgFormInializeComplete(wParam,lParam);

	//sy 2005.04.26.
	else if(message == RMSG_FORMSTATECHANGE) 
		return RmsgFormStateChange(wParam,lParam);
	
	return COleControl::WindowProc(message, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	Data ����  methods & functions
short CKTBChartCtrl::SetChartData(long pChartData, short nCount, short nType) 
{
	return SetChartData2(pChartData, nCount, nType) ;
}

short CKTBChartCtrl::SetChartDataStr(LPCTSTR szChartData, short nCount, short nType) 
{
	if(m_pPacketList == NULL)
		return 0;

	_ML_TEST_NOT_USED();

	//sy 2003.8.22.
	int nPrevTimeInterval = m_pPacketList->GetTimeInterval();
	CScaleBaseData::HORZSCALEDRAWERTYPE ePrevUnitType = m_pPacketList->GetDateUnitType();
	//sy end

	int nRet = m_pPacketList->SetBaseData(szChartData, nCount, nType) ;

	//sy 2005.08.30.
	//	�̶�, �Ϲ� Map�� TR ������ ����̹Ƿ�, ���ŵǴ� Data Packet�� �ϰ����ŵǴ� �������,
	//	TR Type ������ Reset ���θ� �������´�.
	//	���� ���� ������ ���ŵǴ� Packet���� �־����� ������,
	//	DLL Chart�� ���� AddIn�� Packet Case Filtering�� ��ȭ�� �� �ֵ��� ���� Packet�� �׻� �ִ� ������ �����Ѵ�.
	//	���� ~ RELPSA (Reset Last PriceS After), ����/���Ѱ� ǥ��
	OnPacketTRData();

	return nRet ;
}

//sy 2005.08.30.
void CKTBChartCtrl::OnPacketTRData()
{
	if(m_pPacketList == NULL)
		return;

	// real setting
	m_bUseRealData = m_pPacketList->UseRealData();

	CString strTRPacketNames = m_pPacketList->GetTRPacketNames();
	OnPacketTRData(strTRPacketNames, m_pPacketList->GetDataInsertType() == CPacketDataInfo::NEW);

	// (2004.11.24, ��¿�) �׷���, �������� Real�� ��û�� �������� ����Ǵ� ������ �ƴ϶�, TR Data�� ������ �ھ�� �Ѵ�.
	//		�׷��� ������, ���� TR Data�� ������ Real�� �����ϴ� ��찡 �߻��� �� �ִ�.
	//		����, TR ���Ž� Real�� 1ȸ On/Off�ϴ� ����� �����Ѵ�.
	if( m_pPacketList->GetDataInsertType() == CPacketDataInfo::NEW && m_bUseRealData && m_pIRealReceiver)
	{
		m_pIRealReceiver->SetRealAdvise( FALSE);
		m_pIRealReceiver->SetRealAdvise( TRUE);
	}
	
	//	wait event
	if( m_bWaitRequest)
	{
		m_bWaitRequest = FALSE;
		OnReturnOrg2();
	}
}

void CKTBChartCtrl::OnPacketTRData(const CString& strTRPacketNames, const bool bIsReset)
{
	TRACE( "CKTBChartCtrl::OnPacketTRData()\r\n");

	// (2006/11/14 - Seung-Won, Bae) Release Y Scale Min/Max Setting in unlocked. !!!
	if( m_pMainBlock)
	{
		CClientDC dc(this);
		CFont* pOldFont = dc.SelectObject(m_pRscFont);
		m_pMainBlock->ReleaseAllVScaleSettingInFixedMinMax( &dc);
		dc.SelectObject(pOldFont);
		InvalidateControl();
	}

	// (2004.10.13, ��¿�) TR�� ������ AddIn DLL�鿡�� �˷� ������� �Ѵ�.
	// RQ�� Packet : �������� - ojtaso (20070111)
	if( m_pAddInManager) m_pAddInManager->OnPacketDataMultiItem(m_strRQ, strTRPacketNames, bIsReset);

	//sy 2003.3.7 - �ӵ� ���� - ��ũ������ ������ string data ��� (ex- news)
	if(m_pMainBlock != NULL)
		m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::TRDATA_CAL);

	// (2007/6/19 - Seung-Won, Bae) Support Only Draw Condition of P&F for User Setting.
	//		Clear Draw Condition for Reset User Setting.
	if( bIsReset)
	{
		// (2008/10/27 - Seung-Won, Bae) Do not for same item code TR.
		//	ResetUserCndForAutoCnd();

		// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
		SetFirstNMTRClock();

		// ��Ŷ ����Ÿ�� ���� ��� ó�� - ojtaso (20080618)
		if(m_bSyncPacket)
		{
			CString strSyncPacketNames = m_strSyncPacketNames;
			while(!strSyncPacketNames.IsEmpty())
			{
				CString strSyncPacketName = CDataConversion::GetStringData(strSyncPacketNames, "|");
				if(strSyncPacketName.IsEmpty() && !strSyncPacketNames.IsEmpty())
				{
					strSyncPacketName = strSyncPacketNames;
					strSyncPacketNames.Empty();
				}

				CString strPacketName = CDataConversion::GetStringData( strSyncPacketName, ":");
				CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
				CPacket* pSyncPacket = m_pPacketList->GetBaseData(strSyncPacketName);

				if(pPacket && pSyncPacket)
				{
					int nCount = pPacket->GetnumericDataList()->GetCount();
					CList<double, double>* pSyncList = pSyncPacket->GetnumericDataList();

					UINT nRealCount = pSyncList->GetCount() - nCount;
					
					while(nRealCount)
					{
						pSyncList->RemoveTail();
						nRealCount--;
					}
				}
			}
		}

		ResetScroll( TRUE);
	}
	else if( m_pxScaleManager)
	{
		if( IsWholeView()) ResetScroll( TRUE);
		else
		{
			int nPrevXRange = m_pxScaleManager->GetCount();
			m_pxScaleManager->Calc_DRI();
			int nNewXRange = m_pxScaleManager->GetCount();
			ScrollTo( nNewXRange - nPrevXRange - m_pxScaleManager->GetDrawCount_Cur(), FALSE);
		}
	}
}
//sy end

// (2007/6/19 - Seung-Won, Bae) Support Only Draw Condition of P&F for User Setting.
//		Clear Draw Condition for Reset User Setting.
void CKTBChartCtrl::ResetUserCndForAutoCnd( void)
{
	// ���ǰ� ����� ���� ���� - ojtaso (20070913)
	LPCTSTR lpIndicateName[] = {
		_MTEXT( C2_P_AND_F),
		_MTEXT( C2_RENKO_CHART)
	};

	int nSize = sizeof(lpIndicateName) / sizeof(LPCTSTR);
	for(int i = 0; i < nSize; i++)
	{
		int nR = 0, nC = 0, nV = 0, nG = 0;
		CString strGraphName;
		if( !m_pMainBlock->FindFirstIndicator( lpIndicateName[i], nR, nC, nV, nG, strGraphName, NULL)) continue;
		CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
		if( !pIndicatorInfo) continue;
		CList < double, double> *pDrawConditionList = pIndicatorInfo->GetDrawConditionList();
		if( !pDrawConditionList) continue;
		POSITION posHead = pDrawConditionList->GetHeadPosition();
		if( posHead) pDrawConditionList->SetAt( posHead, 0);
	}
}

int CKTBChartCtrl::GetOnePacketSize()
{
	if(m_pPacketList)	{
		return m_pPacketList->GetPacketDataLength(-1) ;
	}
	else
		return 0 ;
}

//	�ܺ� ��� function
//	Method���� �θ����� ���
int CKTBChartCtrl::SetChartData2(long pChartData, int nCount, int nType)
{
	if(m_pPacketList == NULL)
		return 0;
	
	//sy 2003.8.22.
	int nPrevTimeInterval = m_pPacketList->GetTimeInterval();
	CScaleBaseData::HORZSCALEDRAWERTYPE ePrevUnitType = m_pPacketList->GetDateUnitType();
	//sy end

	if(m_pPacketList->GetDataInsertType() == CPacketDataInfo::NEW)
		// �м����� & ��ȣ/����/������ ��� ����
		RunToolFunction(CToolOptionInfo::T_DELETE_ALL, 3);
	// 02.19.2002  system trader chart�� �����Ѵ�  
	if(m_pMainBlock)
	{
		m_pMainBlock->DeleteGraph("System Signal");
		m_pMainBlock->DeleteGraph("System InvestChange");
		// 03.13.2002  news�� �ŷ������� �����Ѵ�
		//m_pMainBlock->DeleteGraph("News");
		//m_pMainBlock->DeleteGraph("�ŷ�����");
	}

	//sy 2006.03.02
	SetOneChart();

	int nRet = m_pPacketList->SetBaseData(pChartData, nCount, nType) ;

	//sy 2005.08.30.
	OnPacketTRData();

	return nRet ;
}

// font
BSTR CKTBChartCtrl::GetCustomFont() 
{
	return m_strCSFont.AllocSysString();
}

void CKTBChartCtrl::SetCustomFont(LPCTSTR lpszNewValue) 
{
	m_strCSFont = lpszNewValue;
	SetModifiedFlag();

	// (2003.12.23, ��¿�) Resource Manager ������ �����Ѵ�.
	FP_vSetInitFont();

	InvalidateControl();
}

void CKTBChartCtrl::FV_vInitFont()
{
	CString strFont = m_strCSFont;
	//��Ʈ�̸�
	int namePos = strFont.Find(";");
	CString lfFaceName = _MTOL0( strFont.Mid(0, namePos));
	//��Ʈũ��
	int HeightPos = strFont.Find(";", namePos + 1);
	CString lfHeight = strFont.Mid(namePos + 1, HeightPos - (namePos + 1));
	int nlfHeight = atoi(lfHeight);
	//����ü��Ʈ
	int BoldPos = strFont.Find(";", HeightPos + 1);
	CString strBold = strFont.Mid(HeightPos + 1, BoldPos - (HeightPos + 1));
	BOOL	blfWeight = atoi(strBold);
	//��Ҽ�
	int CCLinePos = strFont.Find(";", BoldPos + 1);
	CString strCCLine = strFont.Mid(BoldPos + 1, CCLinePos - (BoldPos + 1));
	BOOL	blfStrikeOut = atoi(strCCLine);
	//����Ӳ�
	int italyPos = strFont.Find(";", CCLinePos + 1);
	CString strItaly = strFont.Mid(CCLinePos + 1, italyPos - (CCLinePos + 1));
	BOOL	blfItalic = atoi(strItaly);
	//�������
	int UnderLinePos = strFont.Find(";", italyPos + 1);
	CString strUnderLine = strFont.Mid(italyPos + 1, UnderLinePos - (italyPos + 1));
	BOOL	blfUnderline = atoi(strUnderLine);

	m_logFont.lfHeight = nlfHeight;
	m_logFont.lfWidth = 0;
	m_logFont.lfEscapement = 0;
	m_logFont.lfOrientation = 0;
	if(blfWeight)
		m_logFont.lfWeight = FW_BOLD;
	else
		m_logFont.lfWeight = FW_THIN;
	m_logFont.lfItalic = blfItalic;
	m_logFont.lfUnderline = blfUnderline;
	m_logFont.lfStrikeOut = blfStrikeOut;
	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_logFont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
	strcpy(m_logFont.lfFaceName, lfFaceName);
}

///////////////////////////////////////////////////////////////////////////////
// ������            : �賲��
// ���� ��¥         : 2003/2/4
// �Լ� �̸�         : CSkSlimChartCtrl::FP_vSetInitFont
// ���� Ÿ��         : void
// �Ķ���� ����     : 
// �Լ� ����         : ���ҽ� �Ŵ��� ����
///////////////////////////////////////////////////////////////////////////////
void CKTBChartCtrl::FP_vSetInitFont()
{
	// (2003.12.23, ��¿�, �м�) ���� ������ Custom Font Meta String�� ���� LOG Font Data�� �����Ѵ�.
	FV_vInitFont();

	// (2003.12.23, ��¿�) ���� ���� Font�� �����Ѵ�.
	//	(Resource Manager Pointer�� �������� �Ǵ����� �ʴ´�. Font ������ Resource Manager�� ����/���� �� �� �����Ƿ�)
	// 1. Font Key�� Size�� 0�� �ƴϸ�, Resource Manager�� ���� Font�� ������ ���̴�.
	if( m_FontKey.GetSize())
	{
		if( m_pResourceManager) m_pResourceManager->UnAdviseFont( m_FontKey);
	}
	// 2. Font Key�� Size�� 0�̸�, ��ü������ ������ Font�̴�.
	else if( m_pRscFont)
	{
		m_pRscFont->DeleteObject();
		delete m_pRscFont;
		m_pRscFont = NULL;
	}

	// (2003.12.23, ��¿�) Resource Manager�� �ִ� ��쿡�� ������ Font�� UnAdvise�ϰ� ������ Adviseó���Ѵ�.
	if( m_pResourceManager)
	{
		m_FontKey.SetData(m_logFont.lfWeight, m_logFont.lfHeight, m_logFont.lfFaceName);
		m_pRscFont = ( CFont *)m_pResourceManager->AdviseFont(m_FontKey);
	}
	// (2003.12.23, ��¿�) Resource Manager�� ���� ���, ��ü ������ Font�̹Ƿ�, �����ϰ� ������ Font�� �����Ѵ�.
	else
	{
		// 1. ���� FontKey�� �ʱ�ȭ�Ͽ� ��ü �������� ����Ѵ�.
		m_FontKey.SetData( 0, 0, "");
		// 2. ��ü Font�� �����Ѵ�.
		m_pRscFont = new CFont;
		m_pRscFont->CreateFontIndirect( &m_logFont);
	}
}

//��ǥ�� �б�
void CKTBChartCtrl::SetChartAllInfo() 
{
	//file���� �о �ٽ� Set�ϱ�.
	ReadUserFile(CSaveUserFile::INDICATORLIST);
	//sy 2006.04.11. -> ReadUserFile���� packet �����ϰ� ��ǥ�� �����
	//Condition���� Ʋ������ �𸣴ϱ�...packet�� ���� ���� ��.
	//0���� �Լ� �ȿ��� �� �ٸ� �� ���� �ʴ´�.
	//ClearChartData���� Invalidate �� �ش�.
	//ClearChartData(0);
}

BOOL CKTBChartCtrl::IsThisPacketConnected(const int nIndex, const CString strpacketname)
{
	//nIndex ��° ������ ���� �̸��� packet�� �ִٸ�
	//�̹� connect�� �� �����̴ϱ�..connect ���� ����.
//	for(int x = 0; x < nIndex ; x++){
//		INFOADVISE info = m_arInfoAdvise.GetAt(x);
//		CString szPacketName = info.szPacketName;
//		if(strpacketname.CompareNoCase(szPacketName) == 0) 
//			return TRUE;
//	}
	return FALSE;
}

void CKTBChartCtrl::OnBUseShortKeyChanged() 
{
	BoundPropertyChanged(dispidBUseShortKey);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnBUseConditiontoTitleChanged() 
{
	BoundPropertyChanged(dispidBUseConditiontoTitle);
	SetModifiedFlag();

	if( m_pMainBlock) m_pMainBlock->SetGraphTitleDraw( m_bUseConditiontoTitle, m_bUseDatatoTitle);
}

void CKTBChartCtrl::OnBUseDatatoTitleChanged() 
{
	BoundPropertyChanged(dispidBUseDatatoTitle);
	SetModifiedFlag();

	if( m_pMainBlock) m_pMainBlock->SetGraphTitleDraw( m_bUseConditiontoTitle, m_bUseDatatoTitle);
}

BSTR CKTBChartCtrl::GetData( LPCTSTR strPacketName2, short nDataIndex) 
{
	CString strPacketName = _STOM0( strPacketName2);
	CString strResult = "";

	CString strPacket = strPacketName;
	if(m_pPacketList == NULL)
		return strResult.AllocSysString();

	CString strType = m_pPacketList->GetPacketType(strPacketName);
	if(strType.Compare( _MTEXT( C6_CHARACTER)) == 0) {
		if(nDataIndex == -1)  //���� �ֱ� data
			nDataIndex = m_pPacketList->GetCount(strPacketName) - 1;

		strResult = m_pPacketList->GetStrDataFromIndex(strPacketName, nDataIndex);
	}
	else { //�ð��� ����
		if(nDataIndex == -1)  //���� �ֱ� data
			nDataIndex = m_pPacketList->GetCount(strPacketName) - 1;

		CString strData = CDataConversion::DoubleToString(m_pPacketList->GetDataFromIndex(strPacketName, nDataIndex));
		if(strPacket.Find( _MTEXT( C0_DATE_TIME)) >= 0) 
			strResult = CDataConversion( m_hOcxWnd, ( CChartInfo::CHART_MODE)m_nChartMode).GetStringFormatTrans(strData, m_pPacketList->GetPacketType(strPacketName), "/", ":"); 
		else
			strResult = CDataConversion::GetStringThousandFormat(strData); 
	}
	return strResult.AllocSysString();
}

short CKTBChartCtrl::GetDataCount() 
{
	return short(GetMaxDataCount());
}

int CKTBChartCtrl::GetMaxDataCount() const
{
	if(m_pPacketList == NULL)
		return 0;

	return m_pPacketList->GetMaxCount();
}

void CKTBChartCtrl::OnNHorzGapChanged() 
{
	BoundPropertyChanged(dispidNHorzGap);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnNVertGapChanged() 
{
	BoundPropertyChanged(dispidNVertGap);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnNRButtonMenuTypeChanged() 
{
	BoundPropertyChanged(dispidNRButtonMenuType);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnBShowWarningMessageBoxChanged() 
{
	BoundPropertyChanged(dispidBShowWarningMessageBox);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnBControlBlockSizeChanged() 
{
	BoundPropertyChanged(dispidBControlBlockSize);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnBBlockMoveDeleteChanged() 
{
	BoundPropertyChanged(dispidBBlockMoveDelete);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnBSelectTitleChanged() 
{
	BoundPropertyChanged(dispidBSelectTitle);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnBSelectGraphChanged() 
{
	BoundPropertyChanged(dispidBSelectGraph);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnBUseScaleRButtonMenuChanged() 
{
	BoundPropertyChanged(dispidBUseScaleRButtonMenu);
	SetModifiedFlag();
}

OLE_COLOR CKTBChartCtrl::GetClrLattice() 
{
	return m_clrLattice;
}

void CKTBChartCtrl::SetClrLattice(OLE_COLOR nNewValue) 
{
	m_clrLattice = nNewValue;	
	SetModifiedFlag();
}

void CKTBChartCtrl::OnNGraphRButtonTypeChanged() 
{
	BoundPropertyChanged(dispidNGraphRButtonType);

	SetModifiedFlag();
}

BOOL CKTBChartCtrl::ChangeChartItem(LPCTSTR strGraphName2,
										short nSubGraphIndex,
										LPCTSTR strNewTitle2,
										LPCTSTR strNewPacketName2,
										LPCTSTR strNewGraphType2,
										LPCTSTR strNewGraphStyle2,
										short nNewUpLineWidth,
										short nNewDownLineWidth,
										short nNewEqualLineWidth,
										BOOL bRunatOnce) 
{
	CString strGraphName		= _STOM2( strGraphName2);
	CString strNewTitle			= _STOM1( strNewTitle2);
	CString strNewPacketName	= _STOMS( strNewPacketName2, ",;", 0);
	CString strNewGraphType		= _STOM5( strNewGraphType2);
	CString strNewGraphStyle	= _STOM5( strNewGraphStyle2);

	CString szGraphTitle = strNewTitle,
			szGraphName = strGraphName,
			szPacketName = strNewPacketName;

	if(szGraphTitle.IsEmpty() && szGraphName.IsEmpty() && szPacketName.IsEmpty())
		return FALSE;

	if(m_pMainBlock == NULL || m_pIndicatorList == NULL)
		return FALSE;

	CString strDrawstyle;
	int nGraphType = -1, nGraphStyle = -1, nDrawStyle = 0;
	if(szGraphName.Compare( _MTEXT( C2_PRICE_CHART)) == 0)
		strDrawstyle = strNewGraphStyle;
	else //default
		strDrawstyle = "";	

	const CGraphTypeAndStyleData *pGraphTypeAndStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	if( !pGraphTypeAndStyleData) return FALSE;
	if( !pGraphTypeAndStyleData->GetGraphTypeAndStyle(strNewGraphType, strNewGraphStyle, strDrawstyle, nGraphType, nGraphStyle, nDrawStyle)) return FALSE;

	BOOL ret = m_pMainBlock->ChangeGraphData(strGraphName,
											nSubGraphIndex,
											strNewPacketName,
											strNewTitle,
											CGraphBaseData::GRAPHTYPE(nGraphType),
											CGraphStyle(nGraphStyle),
											CGraphDrawStyle(nDrawStyle), 
											nNewUpLineWidth,
											nNewDownLineWidth,
											nNewEqualLineWidth);

	if( ret)
	{
		if( m_pAddInManager) m_pAddInManager->OnChartItemChanged();

		InvalidateControl();
		SaveChartCfg();
	}

	return ret;
}

// (2006/12/24 - Seung-Won, Bae) Do not support stoploss function anymore.
BOOL CKTBChartCtrl::NotUsedMethod7( BOOL bIsStopLoss, double dStopLossPrice, BOOL bIsProfit, double dProfitPrice) 
{
	return FALSE;
}

void CKTBChartCtrl::DeleteZoomInOutInquiryDlg()
{
//	//nami
//	if(m_ZoomInOutDataDlg != NULL){
//		m_ZoomInOutDataDlg->DestroyWindow();
//		delete m_ZoomInOutDataDlg;	
//	}
//	m_ZoomInOutDataDlg = NULL;
}

BOOL CKTBChartCtrl::RestoreMinMax(LPCTSTR strPacketName2) 
{
	if(m_pPacketList == NULL)
		return FALSE;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	bResult = pPacket->RestoreMinMax() ? 1 : 0;
	InvalidateControl();

	return bResult;
}

BOOL CKTBChartCtrl::RestoreMin(LPCTSTR strPacketName2) 
{
	if(m_pPacketList == NULL)
		return FALSE;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	bResult = pPacket->RestoreMin() ? 1 : 0;
	InvalidateControl();

	return bResult;
}

BOOL CKTBChartCtrl::RestorMax(LPCTSTR strPacketName2) 
{
	if(m_pPacketList == NULL)
		return FALSE;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	bResult = pPacket->RestoreMax() ? 1 : 0;
	InvalidateControl();

	return bResult;
}

BOOL CKTBChartCtrl::SetMin(LPCTSTR strPacketName2, double dMin) 
{
	if(m_pPacketList == NULL)
		return FALSE;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	bResult = pPacket->SetMin(dMin) ? 1 : 0;
	InvalidateControl();

	return bResult;
}

BOOL CKTBChartCtrl::SetMax(LPCTSTR strPacketName2, double dMax) 
{
	if(m_pPacketList == NULL)
		return FALSE;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	bResult = pPacket->SetMax(dMax) ? 1 : 0;
	InvalidateControl();

	return bResult;
}

BOOL CKTBChartCtrl::SetMinMax(LPCTSTR strPacketName2, double dMin, double dMax) 
{
	if(m_pPacketList == NULL)
		return FALSE;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	bResult = pPacket->SetMin(dMin) ? 1 : 0;
	bResult = pPacket->SetMax(dMax) ? 1 : 0;
	InvalidateControl();

	return bResult;
}

BOOL CKTBChartCtrl::SetHorzScaleData(short nRowIndex, short nColumnIndex, LPCTSTR strScaleData2) 
{
	if(m_pMainBlock == NULL)
		return FALSE;

	CString strScaleData = _STOM0( strScaleData2);
	m_pMainBlock->SetHorzScaleData(CBlockIndex(nRowIndex, nColumnIndex), strScaleData);
	InvalidateControl();
	return TRUE;
}

double CKTBChartCtrl::GetMin(LPCTSTR strPacketName2) 
{
	if(m_pPacketList == NULL)
		return 0.0;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return 0.0;

	double min, max;
	pPacket->GetMinMax(0, pPacket->GetCount() - 1, min, max);
	return min;
}

double CKTBChartCtrl::GetMax(LPCTSTR strPacketName2) 
{
	if(m_pPacketList == NULL)
		return 0.0;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return 0.0;

	double min, max;
	pPacket->GetMinMax(0, pPacket->GetCount() - 1, min, max);
	return max;
}

BOOL CKTBChartCtrl::SetGraphHide2(short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, short nSubGraphIndex, BOOL bHide) 
{
	if(m_pMainBlock == NULL)
		return FALSE;

	if( !m_pMainBlock->SetSubGraphHiding( CBlockIndex( nRowIndex, nColumnIndex), nVerticalIndex, nGraphIndex, nSubGraphIndex, bHide ? 1 : 0)) return FALSE;
	InvalidateControl();
	return TRUE;
}

BOOL CKTBChartCtrl::SetGraphHide3(LPCTSTR strTitle2, BOOL bIsHide) 
{
	//�ش� title�� ��� �׷����� ����/����
	if(m_pMainBlock == NULL)
		return false;

	CString strTitle = _STOM1( strTitle2);
	if(!m_pMainBlock->SetSubGraphHiding(strTitle, bIsHide))
		return false;

	InvalidateControl();
	return true;
}

// (2006/11/22 - Seung-Won, Bae) Do not support the Method 'ChangeGraphDatas' anymore.
BOOL CKTBChartCtrl::NotUsedMethod1( short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, LPCTSTR strNewGraphName,
									 LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, short nNewUpLineWidth,
									 short nNewDownLineWidth, short nNewEqualLineWidth, BOOL bRunatOnce)
{
	_ML_TEST_NOT_USED();
	return FALSE;
}

//nami 9.25
void CKTBChartCtrl::SetInquiryValue(const BOOL nIs)
{
	//���ڼ��� ��ġ��ȸâ�� ���� ������������..
	//��ġ��ȸâ�� Ǯ������ ���ڼ��� ���� �ٴϴ� ��ġ��ȸâ�� Ǯ���� �ʰ� �ϱ� ���ؼ��̴�.

	m_bUseInquiryData = nIs;
}

void CKTBChartCtrl::OnToExcel() 
{
	CString szTitle, szPacketNames, szPacketNames2, strHorzScale;
	CSubGraphViewDataList pViewDataList;
	CViewData viewData;
	// 20081028 �ڵ��� >>
	//m_pMainBlock->GetViewDataListInAllBlock(viewData);
	m_pMainBlock->GetViewDataListInAllBlock(m_strRQ, viewData);
	// 20081028 �ڵ��� <<
	if(viewData.GetColumnViewDataCount() <= 0)
		return;

	for(int i = 0; i < m_pMainBlock->GetColumnCount(); i++){
		for(int j = 0; j < viewData.GetRowViewDataCount(i); j++){
			CRowViewData& rowViewData = viewData.GetAt(i, j);
			//nami 9.2
			//����� ���ν������� �ϳ��� ���̰� �ϰ� �ִ�.
			CString strData = rowViewData.GetHorzScaleName();
			
			if(!strData.IsEmpty()) 
				strHorzScale = strData;
			//nami end
			for(int k = 0 ; k < rowViewData.GetSubGraphViewDataCount(); k++){
				POSITION listPos = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().FindIndex(k);
				if(listPos){
					CSubGraphViewData pSubGraph = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().GetNext(listPos);
					szTitle += pSubGraph.GetTitle() + ";" ;
					szPacketNames += pSubGraph.GetPacketNames() + ";" ;
					szPacketNames2 += pSubGraph.GetPacketNames() ;
				}
			}
		}
	}

	CExcelDataController controller( m_hOcxWnd, ( CChartInfo::CHART_MODE)m_nChartMode);
	controller.CallExcel(szTitle, szPacketNames2, szPacketNames, strHorzScale,
						GetUserFilePath(), TRUE, m_pPacketList);
}

//-1 : Packet���� Ÿ���� ��..�ش� ������ ����.
//nSaveDataType = 0 �̸� ƽ ����
//nSaveDataType = 1 �̸� ƽ�̿� ����
void CKTBChartCtrl::SaveandApplyGraphStyle(short nSaveDataType, short nChangeDataType) 
{
	//sy 2003.8.22.
	// TR Data�� ��ȸ�ɶ� �ڵ����� �ٲ� �ش�.
	//SetBongGraphStyle(nSaveDataType);
	//sy end

	// cj 2003.5.26
	//ChangeBongGraphStyle(nChangeDataType);
}

void CKTBChartCtrl::OnNHoldingPacketSizeChanged() 
{
	BoundPropertyChanged(dispidNHoldingPacketSize);
	SetModifiedFlag();

	//0005977: [7111] ������Ʈ - ƽ�ֱ� ��Ʈ ��ȸ ��, HTS ����� Ÿ�� ���� 0.2������ �������� ��ó�� ���̴� ����
	//{
	//	CString szDebug;
	//	szDebug.Format("\n[DRCS]Ln(%d) CKTBChartCtrl::SetAllProperties_Chart m_nHoldingPacketSize(%d -> 0)\n", __LINE__, m_nHoldingPacketSize);
	//	OutputDebugString(szDebug);
	//}
	m_nHoldingPacketSize = 0;

	if(m_pPacketListManager)
		m_pPacketListManager->SetHoldingPacketSize(m_nHoldingPacketSize);
}

void CKTBChartCtrl::SetDataRegionInBlock(short nDataStartIndex) 
{
	ScrollTo( nDataStartIndex, FALSE);
}

void CKTBChartCtrl::SetItemInfo(LPCTSTR lpstrCode, LPCTSTR lpstrName) 
{
	m_strItemCode = lpstrCode;
	m_strItemName = lpstrName;
}

void CKTBChartCtrl::AddandDeleteIndicator(LPCTSTR strGraphName2) 
{
	if(m_pMainBlock == NULL) return;
	
	CString strGraphName = _STOM2( strGraphName2);
	AddChartBlock( strGraphName);
}

void CKTBChartCtrl::OnSzBasePacketNameChanged() 
{
	_ML_TEST_NOT_USED();
	SetModifiedFlag();
}

//sy 2003.8.7.
//#include "ChartCheckerModuleHelper.h"
BOOL CKTBChartCtrl::PacketDataMaker()
{
	if(IDNO == AfxMessageBox("Test Module Load ?", MB_ICONQUESTION | MB_YESNO))
		return FALSE;

	// (2003.12.23, ��¿�) DLL�� SK Common Folder�� �ű��.
//	CChartCheckerModuleHelper( g_strOcxPath).LoadIndicatorDataChecket(this, m_pMainBlock, GetBaseDeskPath() + USER_FILE_FOLDER);
	return TRUE;
}

//sy 2005.09.06.
CString CKTBChartCtrl::GetUserFilePath(const CString& strBaseDeskPath)
{
	//CString strFilePath;
	//strFilePath.Format("%s%s", GetUserDefaultFilePath(strBaseDeskPath), m_strMapName);
	//strFilePath.Format("%s%s", GetUserDefaultFilePath(strBaseDeskPath), m_strMapName);
	//if(!m_strMapName.IsEmpty() && m_strMapName.GetAt(m_strMapName.GetLength() -1) != '\\')
	//	strFilePath += "\\";
	
	return GetUserDefaultFilePath(strBaseDeskPath);
}

CString CKTBChartCtrl::GetUserDefaultFilePath(const CString& strBaseDeskPath)
{
	if(strBaseDeskPath.IsEmpty())
		return (GetBaseDeskPath() + USER_FILE_FOLDER + m_strUserID);

	return (strBaseDeskPath + USER_FILE_FOLDER + m_strUserID);
}

CString CKTBChartCtrl::GetUserFilePathAndName(const CString& strFileName)
{
	if(strFileName.IsEmpty())
		return "";

	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	return m_strPagePathWithPrefix + strFileName;
}

CString CKTBChartCtrl::GetUserDefaultFilePathAndName(const CString& strFileName)
{
	if(strFileName.IsEmpty())
		return "";

	return (GetUserDefaultFilePath() + strFileName);
}

CString CKTBChartCtrl::GetDefaultFilePathAndName(const CString& strFileName)
{
	if(strFileName.IsEmpty())
		return "";

	//return (GetBaseDeskPath() + "\\data\\" + strFileName);
	return (GetDefaultPath() + strFileName);
}
//sy end

//sy 2005.04.26.
void CKTBChartCtrl::OnBDoesUseSkinColorChanged() 
{
	BoundPropertyChanged(dispidBDoesUseSkinColor);
	SetModifiedFlag();
}
//sy end

//sy 2005.09.06.
void CKTBChartCtrl::OnStrMapNameChanged() 
{
	// �� �̸�(��ȣ)
	BoundPropertyChanged(dispidStrMapName);
	SetModifiedFlag();
}

void CKTBChartCtrl::OnNBongCountMarginChanged() 
{
	// ������ ���� ����
	BoundPropertyChanged(dispidNBongCountMargin);
	SetModifiedFlag();
}
//sy end

//sy 2005.12.08.
void CKTBChartCtrl::OnBOnePageDataCountInTRCountChanged() 
{
	BoundPropertyChanged(dispidBOnePageDataCountInTRCount);
	SetModifiedFlag();
	ResetScroll();
}
//sy end

void CKTBChartCtrl::OnNMinDataCountOnOnePageChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.11.12
//
//	����	:	Desk�� ���� GetAllProperty2�� ��Ÿ Method�� ������ Interface�� ���� �����Ѵ�.
//
//	����	:	1. SK Enstock Component�� BigBang AnyVersion�� �����ϸ鼭,
//					Chart�� BigBang�� AnyChart�� NKTBChart�� ���� �����Ͽ�����,
//					�� Component�� �� Interface Class�� �����ؾ� �Ѵ�.
//
//	����	:	(2003.11.12) Resource Manager�� AnyVersion ������ ���Ŀ� ���� ó���Ѵ�.
///////////////////////////////////////////////////////////////////////////////
// CAllPropGetterSetter Implementation
#define classOrig	CKTBChartCtrl
void classOrig::CAllPropGetterSetter::getAllCtrlProperty(CStringList* plist)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)
	pThis->GetAllProperties2((long)plist);
}
BOOL classOrig::CAllPropGetterSetter::setAllCtrlProperty(CStringList* plist)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)
	return pThis->SetAllProperties2((long)plist);
}
void classOrig::CAllPropGetterSetter::setBaseDesk(long lKey)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)
	pThis->SetBaseDesk(lKey);
}
void classOrig::CAllPropGetterSetter::setRscMng(long lKey)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)
	pThis->SetRscMng(lKey);	
}
void CKTBChartCtrl::SetRscMng( long lKey)
{
	if( lKey <= 0 ) return;
	m_pResourceManager = ( CResourceManager*)lKey;
}
long CKTBChartCtrl::GetBasePropIf() 
{
	// TODO: Add your dispatch handler code here
	return (long)&m_xAllGetterSetter;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.24
//
//	����	:	���� BigBang Chart�� ����ġ Method�� Porting�Ѵ�.
///////////////////////////////////////////////////////////////////////////////
BOOL CKTBChartCtrl::SetVertScaleData(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, LPCTSTR strVertScaleData2) 
{
	// TODO: Add your dispatch handler code here
	if(m_pMainBlock == NULL)
		return FALSE;

	CString strVertScaleData = _STOM6_N( strVertScaleData2);
	if( strVertScaleData.IsEmpty()) strVertScaleData = _STOM0( strVertScaleData2);

	if(!m_pMainBlock->SetVertScaleData(CBlockIndex(nRowIndex, nColumnIndex), 
		nVertScaleGroupIndex, strVertScaleData))
		return FALSE;
	
	Invalidate();
	return TRUE;
}

// (2006/11/22 - Seung-Won, Bae) Do not support the Method 'ChangeBlockType' anymore.
BOOL CKTBChartCtrl::NotUsedMethod4(short nBlockType, short nRowCount, short nColumnCount) 
{
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.01.02
//
//	����	:	���� BigBang Chart�� ResetChart Method�� Porting�Ѵ�.
///////////////////////////////////////////////////////////////////////////////
BOOL CKTBChartCtrl::ResetChart() 
{
	//��Ʈ �ʱ�ȭ���� ���񺯰� ���� ���� : �������� - ojtaso (20070621)
	m_bResetChart = TRUE;

	// TODO: Add your dispatch handler code here
	// (2004.05.20, ��¿�) DLL Container���� ������ Packet �ʱ�ȭ ���±����� RollBack ��Ű�� ����� �߰��Ѵ�.
	//		�⺻���� �ʱ�ȭ�� 0(Packet �ʱ�ȭ)������ ó������Ѵ�.
	// (2004.07.16, ��¿�) Reset�ÿ��� �м����� � Clear�ǵ��� OnReturnOrg2()�� ȣ���Ѵ�. (MessageBox ����)
	OnReturnOrg2( FALSE);

	//��Ʈ �ʱ�ȭ���� ���񺯰� ���� ���� : �������� - ojtaso (20070621)
	m_bResetChart = FALSE;
	return TRUE;
}

BOOL CKTBChartCtrl::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	// (2008/11/19 - Seung-Won, Bae) for test
//	if( m_pPacketListManager)	m_pPacketListManager->SetIRealReceiver( NULL);
//	if( m_pxScaleManager)		m_pxScaleManager->SetIRealReceiver( NULL);

	// (2004.11.17, ��¿�) DRDS DLL Unloading ó���� �̰������� ȣ��ǹǷ� ������ Routine���� �������� �ʴ´�.
	if( m_pIRealReceiver)
	{
		if( m_bUseRealData) m_pIRealReceiver->SetRealAdvise( FALSE);
		m_pIRealReceiver->Release();
		m_pIRealReceiver = NULL;
	}

	if(m_pMainBlock) WriteUserFile(CSaveUserFile::ENVIRONMENT);
	if(m_pIndicatorList) WriteUserFile(CSaveUserFile::INDICATORLIST);

	// (2004.10.11, ��¿�) Addin DLL�� �ϰ� Unloading�ϴ� Interface�� �����Ѵ�.
	//		AddIn Unload�ÿ��� �������� OCX�� Access�� �� �ֵ��� Window Destroy Time���� �ϰ� Unload�ϵ��� �Ѵ�.
	//		�̶�, �ٸ� � �ͺ��� �켱 ó���ǵ��� �Ѵ�. (AddIn ����� �������� �� �ֱ� �����̴�.)
	if( m_pAddInManager)
	{
		m_pAddInManager->OnOcxDestroyWindow();
		m_pAddInManager->UnloadAllAddin();
	}

	// Chart Object => MainBlock
	if( m_pMainBlock)
	{
		delete m_pMainBlock;
		m_pMainBlock = NULL;
	}

	// X-Scale Manager
	if( m_pxScaleManager)
	{
		delete m_pxScaleManager;
		m_pxScaleManager = NULL;
	}

	// (2004.06.08, ��¿�) Global ���� ����� �߰��Ѵ�.
	////////////////////////
	// Global ���� ����Ÿ ����
	m_xGSharedCli.RemoveSharedNameToServer();

	// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
	g_iMetaTable.RemoveLocalLanguageTable( GetSafeHwnd());

	return COleControl::DestroyWindow();
}

void CKTBChartCtrl::OnBFireRealEventChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CKTBChartCtrl::OnNInquiryTypeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CKTBChartCtrl::OnBOcxAreaNumericalInquiryChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.03.08
//
//	����	:	HTS Main Window�� AfxGetMainWnd()�� �ƴ϶�, Desk�� ���Ͽ� ��Ȯ�ϰ�
//				���ϵ��� �����Ѵ�.
//				(Dialog Based Main�� ��� AfxGetMainWnd()�� ���������� ����)
///////////////////////////////////////////////////////////////////////////////
CWnd* CKTBChartCtrl::GetMainWnd( void)
{
	CWnd *pMain = NULL;
	if( m_pBaseDesk) pMain = ( CWnd *)m_pBaseDesk->GetMainInfo( "MAINHANDLE");
	if( !pMain) pMain = AfxGetMainWnd();
	return pMain;
}

void CKTBChartCtrl::OnNNumericalInquiryOptionChanged() 
{
	SetModifiedFlag();

	// (2006/3/3 - Seung-Won, Bae) Do not check. Because It can use with Script and Inner ToolBar's Controlling.
	SetNumericalInquiryOption( m_nNumericalInquiryOption);
}

//sy 2004.04.19.
// subGraphTitle�� �����ϱ�.
// bGraphShowing == TRUE : �߰��� ��츸 ����.
// bGraphShowing == FALSE : �߰� �ȵ� ��쵵 ����.
BOOL CKTBChartCtrl::SetGraphTitle(LPCTSTR strGraphName2, short nSubGraphIndex, LPCTSTR strNewGraphTitle2, BOOL /*bGraphShowing*/) 
{
	if(m_pMainBlock == NULL || m_pIndicatorList == NULL)
		return FALSE;

	CString strGraphName		= _STOM2( strGraphName2);
	CString strNewGraphTitle	= _STOM2( strNewGraphTitle2);
	BOOL bResult = m_pMainBlock->ChangeSubGraphTitle(strGraphName, (int)nSubGraphIndex, strNewGraphTitle);
	if(bResult)
		InvalidateControl();

	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
	if(pIndicatorInfo == NULL) return bResult;

	pIndicatorInfo->SetSubGraphTitle(nSubGraphIndex, strNewGraphTitle);
	return bResult;
}
//sy end

void CKTBChartCtrl::OnStrGlobalSharedListChanged() 
{
	// TODO: Add notification handler code
	_ML_TEST_NOT_USED();
	SetModifiedFlag();
}

// (2004.09.16, ��¿�) �ܺ� Container���� ���� ��ǥ�� ����� ��, Ư����Ʈ���� Ȯ���ϰ� ������ �� �ֵ����� ���Ѵ�.
BOOL CKTBChartCtrl::OnRunningOneChart() 
{
	// TODO: Add your property handler here
	return IsRunningOneChart();
}

// (2004.10.13, ��¿�) ���Ѱ�/���Ѱ� ǥ�ñ�ɵ� Custom Addin���� �ű��.
void CKTBChartCtrl::EnableBoundMark(BOOL p_bEnable) 
{
	// (2004.10.13, ��¿�) ����/������ ǥ�ñ���� AddIn DLL Interface�� ������ �����Ͽ� �������� AddIn ����� ��û�� �� �ֵ��� �Ѵ�.
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_CUSTOM_ENABLE_BOUNDMARK, p_bEnable);
}
// (2005/9/28 - Seung-Won, Bae) Query Bound Mark Flag
BOOL CKTBChartCtrl::GetBoundMarkFlag() 
{
	// TODO: Add your dispatch handler code here
	long lResult = 0;
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_CUSTOM_GET_BOUNDMARK_FLAG);
	return lResult;
}

// (2004.09.30, ��¿�) �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
//		�߽��� �������� �ʴ´�.
void CKTBChartCtrl::EnableReceivingTimeShare(BOOL p_bEnable) 
{
	// TODO: Add your dispatch handler code here

	// (2004.10.11, ��¿�) TimeMark ����� Custom AddIn DLL�� �ű��.
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_CUSTOM_ENABLE_RECEIVING_TIMESHARE, p_bEnable);
}

// (2004.10.04, ��¿�) ���ڼ��� ��ġ��ȸâ �����ϴ� ����� Script�ε� ó���� �� �ֵ��� Property�� �����Ѵ�.
void CKTBChartCtrl::OnNUseInquiryWithCrosslineChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

// (2004.10.06, ��¿�) �����Ǵ� Packet�� Type�� �����ϴ� Interface�� �����Ѵ�.
BOOL CKTBChartCtrl::ChangePacketType(LPCTSTR p_szPacketName2, LPCTSTR p_szPacketType2) 
{
	// TODO: Add your dispatch handler code here

	// 0. Packet List�� Ȯ���Ѵ�.
	if( !m_pPacketList) return FALSE;

	CString p_szPacketName = _STOM0( p_szPacketName2);
	CString p_szPacketType = _STOM6( p_szPacketType2);
	// 1. �ش� Packet Object�� ���Ѵ�.
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return FALSE;

	// 2. ���ο� Packet Type�� �����Ѵ�.
	pPacket->SetType( p_szPacketType);

	return TRUE;
}

BOOL CKTBChartCtrl::ChangePacketTypeRQ(LPCTSTR lpszRQ, LPCTSTR p_szPacketName2, LPCTSTR p_szPacketType2) 
{
	if(lpszRQ == NULL)	return FALSE;
	CPacketList* pPacketList = m_pPacketListManager->GetPacketList(lpszRQ);
	if(!pPacketList)	return FALSE;

	// 0. Packet List�� Ȯ���Ѵ�.
	if( !pPacketList) return FALSE;
	
	CString p_szPacketName = _STOM0( p_szPacketName2);
	CString p_szPacketType = _STOM6( p_szPacketType2);
	// 1. �ش� Packet Object�� ���Ѵ�.
	CPacket *pPacket = pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return FALSE;
	
	// 2. ���ο� Packet Type�� �����Ѵ�.
	pPacket->SetType( p_szPacketType);
	
	return TRUE;
}

// (2004.10.06, ��¿�) �����Ǵ� Graph�� Graph Drawing Data�� �缳���Ѵ�.
BOOL CKTBChartCtrl::ChangeGraphDrawingData(LPCTSTR p_szIndicatorName2, BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData) 
{
	// TODO: Add your dispatch handler code here

	// 0. Main Block Pointer�� Ȯ���Ѵ�.
	if( !m_pMainBlock) return FALSE;

	CString p_szIndicatorName = _STOM2( p_szIndicatorName2);
	// 1. Graph Drawing Data�� �缳���Ѵ�.
	if( m_pMainBlock->ChangeGraphDataDrawingData( p_szIndicatorName, p_bLowHigh == TRUE,
		p_bLowCur == TRUE, p_bHighCur == TRUE, p_bPrevCur == TRUE, p_nCurData))
	{
		// 2. �������� �缳���ÿ��� �ٽ� Drawing �ǵ��� �Ѵ�.
		InvalidateControl();
		return TRUE;
	}

	return FALSE;
}

// (2004.10.06, ��¿�) ������Ʈ�� Graph Drawing Data�� �����޾� DefaultType�� ���� ����, ������� �Ѵ�.
BOOL CKTBChartCtrl::ChangePriceGraphDrawingData(BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData, short p_nDefaultType) 
{
	// TODO: Add your dispatch handler code here

	// 1. ���� �⺻ ������ ó���Ѵ�.
	if( !ChangeGraphDrawingData( _MTEXT( C2_PRICE_CHART), p_bLowHigh, p_bLowCur, p_bHighCur, p_bPrevCur, p_nCurData)) return FALSE;

	return TRUE;
}

// (2004.10.13, ��¿�) ��ġ��ȸâ�� ������� �������� �����ϴ� Property�� �߰��Ѵ�.
void CKTBChartCtrl::OnBShowPriceRateInquiryChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

// (2004.10.21, ��¿�) �м����� TEXT�� ��� ���θ� �����Ѵ�.
void CKTBChartCtrl::ShowAnalysisToolText(BOOL p_bTimeLine, BOOL p_bPriceLineLeft, BOOL p_bPriceLineRight, BOOL p_bTrendLine, BOOL p_bTrisectionLine, BOOL p_bQuadrisectionLine, BOOL p_bFibonacciRetraceLine) 
{
	// TODO: Add your dispatch handler code here
	
	// 1. ���� �����Ѵ�.
	m_bShowVerticalText		= p_bTimeLine;
	m_bShowHorizonTextLeft	= p_bPriceLineLeft;
	m_bShowHorizonTextRight	= p_bPriceLineRight;
	m_bShowTrendLineText	= p_bTrendLine;
	m_bShowThreeText		= p_bTrisectionLine;
	m_bShowFourText			= p_bQuadrisectionLine;
	m_bShowFiboText			= p_bFibonacciRetraceLine;

	// (2006/1/24 - Seung-Won, Bae) Reset Text Position of Analysis Tool
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_ALL_ANALYSIS_RESET_TEXTPOSITION);
}

// [04/10/27] Data ��ǥ�� Screen ��ǥ�� ��ȯ�ϴ� Interface�� �����Ѵ�.
int CKTBChartCtrl::ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin,
							int p_nYPositionTop, int p_nYPositionBottom, BOOL p_bLog, BOOL p_bReverse)
{
	// 1. Check Reverse (Swap Data Min/Max)
	if( p_bReverse)
	{
		double dData	= p_dDataMax;
		p_dDataMax		= p_dDataMin;
		p_dDataMin		= dData;
	}

	// 2. Make Log Data
	if( p_bLog)
	{
		p_dDataMax	= CMath::Log( p_dDataMax);
		p_dDataMin	= CMath::Log( p_dDataMin);
		p_dData		= CMath::Log( p_dData);
	}

	// 2. Get Data
	return CDataConversion::RelativeToAbsolute_Y( CRect( 0, p_nYPositionTop, 0, p_nYPositionBottom),
		p_dData, p_dDataMin, p_dDataMax);
}

// (2004.10.29, ��¿�) Timer Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
void CKTBChartCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	// (2004.10.29, ��¿�) Timer Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	if( m_pAddInManager) m_pAddInManager->OnTimer( nIDEvent);

	COleControl::OnTimer(nIDEvent);
}

// (2004.11.02, ��¿�) �� Mouse ��ġ�� Chart Block�� ������ ó���ϴ� Popup Menu�� �����Ѵ�.
void CKTBChartCtrl::OnUpdateReverseYscale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

	// 0. �ش� Block�� ������ ������ ���� �� ���� ��쿡�� Disable ��Ų��.
	pCmdUI->Enable( FALSE);

	// 1. �� Cursor Position�� �ִ� Block�� ������ �������� Ȯ���Ѵ�.
	if( !m_pMainBlock) return;
	if( m_biReverseYScale_BlockIndex.GetColumn() < 0 || m_biReverseYScale_BlockIndex.GetRow() < 0) return;
	CBlock *pBlock = m_pMainBlock->GetBlock( m_biReverseYScale_BlockIndex);
	if( !pBlock) return;
	m_bReverseYScale_ReverseChart = pBlock->HasInvertScale();

	pCmdUI->Enable( TRUE);
	pCmdUI->SetCheck( m_bReverseYScale_ReverseChart);
}
void CKTBChartCtrl::OnReverseYscale() 
{
	// TODO: Add your command handler code here

	// 1. Reverse ��ų Block ������ ������, ó���Ѵ�.
	if( 0 <= m_biReverseYScale_BlockIndex.GetColumn() && 0 <= m_biReverseYScale_BlockIndex.GetRow())
	{
		m_pMainBlock->SetAllVertScaleInvertType( m_biReverseYScale_BlockIndex, !m_bReverseYScale_ReverseChart);
		InvalidateControl();
	}

	// 2. ���� �ٽ� �ʱ�ȭ�Ѵ�.
	m_bReverseYScale_ReverseChart	= FALSE;
	m_biReverseYScale_BlockIndex	= CBlockIndex( -1, -1);
}

// (2004.11.10, ��¿�) OCX�� Arrow Key Down Message�� ���� �� �ֵ��� �����Ѵ�.
UINT CKTBChartCtrl::OnGetDlgCode() 
{
	// TODO: Add your message handler code here and/or call default
	return COleControl::OnGetDlgCode() | DLGC_WANTARROWS;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetMaxRange
 - Created at  :  2004-11-16   09:09
 - Author      :  ������
 - Description :  MaxRange�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
int CKTBChartCtrl::GetMaxRange()
{
	if(IsUseToolScrollBar())	// ���ٰ� ������ ���ٿ��� ���� MaxRange�� �����Ѵ�.
		return m_pToolBarManager->GetMaxRange();
	else						// ���ٰ� ������ SetScrollData_NoneToolBar���� ���� ���� �����Ѵ�.
		return m_nMaxRange;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  IsUseToolScrollBar
 - Created at  :  2004-11-16
 - Author      :  ������
 - return	   :  bool - true: SetScrollData_TRPart(), ..._RealPart()ȣ��.  false: SetScrollData_NoneToolBar()ȣ��.
 - Description :  ��ũ�ѹ��� ��������� �����Ѵ�.
		   		  ���ٰ� �־ ��ũ�ѹٰ� ���� ��쿣 SetScrollData_NoneToolBar()�Լ��� ȣ��Ǿ��
				  �ϳ� ���ٰ� ���� ��쿡�� Set.._NoneToolBar()�� ȣ��Ǵ� ������ �����ϱ� ���� �Լ�.
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::IsUseToolScrollBar()
{
	if(m_pToolBarManager && m_pToolBarManager->IsToolScrollBar())
		return true;
	else
		return false;
}

void CKTBChartCtrl::OnstrLocalSharedListChanged() 
{
	BoundPropertyChanged(dispidStrLocalSharedList);
	_ML_TEST_NOT_USED();
	SetModifiedFlag();
}

// (2005.01.17, ��¿�) Ư����Ʈ�� ���, ������ �Ϲ� ��Ʈ�� Graph�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CKTBChartCtrl::HasGraphInSpecialBack(LPCTSTR p_szGraphName2) 
{
	// TODO: Add your dispatch handler code here
	LPCTSTR p_szGraphName = _STOM2( p_szGraphName2);
	return HasIndicatorInSpecialBack( p_szGraphName);
}

// (2005.01.19, ��¿�) AddIn DLL�� ȭ�麰�� ���������� Loading�� �� �ֵ��� �����Ѵ�.
BOOL CKTBChartCtrl::LoadAddIn(LPCTSTR p_szAddInDllName) 
{
	// TODO: Add your dispatch handler code here

	if( !m_pAddInManager) return FALSE;
	if( !m_pAddInManager->LoadAddIn( p_szAddInDllName)) return FALSE;

#ifdef _DEBUG
	// (2006/7/25 - Seung-Won, Bae) Use Real Module with AddIn
	if( !strncmp( p_szAddInDllName, g_aszAddInLoadString[ AID_DRDS_REAL], 30))
	{
		if( m_pIRealReceiver) m_pIRealReceiver->Release();
		m_pIRealReceiver = ( IRealReceiver *)m_pAddInManager->GetAddInCustomInterface( g_aszAddInLoadString[ AID_DRDS_REAL]);
		if( m_bUseRealData && m_pIRealReceiver) m_pIRealReceiver->ResetRealSettingInfo( m_strRealList);
	}
#endif

	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/10/25
// Modifier		: 
// Comments		: Unload AddIn DLL
//-----------------------------------------------------------------------------
BOOL CKTBChartCtrl::UnloadAddIn(LPCTSTR p_szAddInDllName) 
{
	// TODO: Add your dispatch handler code here

	// 2. Unload AddIn in Manager
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->UnloadAddIn( p_szAddInDllName);
}

// (2005.01.19, ��¿�) AddIn DLL���� ���� Parameter�� �����Ͽ� AddIn���� ó���ϴ� Interface�� �����Ѵ�.
BOOL CKTBChartCtrl::InvokeAddIn(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->InvokeAddIn( p_szAddInDllName, p_nCommandType, p_lData);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/2/17
// Modifier		: 
// Comments		: Change Drag Mode Type
// Parameter	:
//					0 - Drag Zoom
//					1 - Term Analysis
//-----------------------------------------------------------------------------
void CKTBChartCtrl::SetDragModeType(short p_nDragModeType) 
{
	// TODO: Add your dispatch handler code here

}

/*-----------------------------------------------------------------------------------------
 - Function    :  AddOrRemoveChart
 - Created at  :  2005-03-03
 - Author      :  �輱��
 - Description :  ��ǥ�׷��� �߰�/������ �����Ѵ�.
				  ��, Ư����Ʈ�� ���� ������ �ƴ� Ư����Ʈ�� �ƴ� ���� ��Ʈ�� �����ȴ�.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::AddOrRemoveChart(LPCTSTR strGraphName) 
{
	_ML_TEST_NOT_USED();
	return TRUE;
}


/*-------------------------------------------------------------------------------
 - Function    :  ApplySkin
 - Created at  :  2005-04-22   15:31
 - Author      :  �輱��
 - Description :  Desk�� ���� chart skin ���� �޴´�. 
				  ��, �ɼ����� ó���ȴ�.
 -------------------------------------------------------------------------------*/
//sy 2005.04.26.
bool CKTBChartCtrl::ApplySkin()
{
	if(m_pBaseDesk == NULL || !m_bDoesUseSkinColor) return false;


	int nSkinNumber = GetSkinNumber();
	if( nSkinNumber < 0) return false;

	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_STYLE_SET_STYLE, nSkinNumber);
	return true;
}

int CKTBChartCtrl::GetSkinNumber() const
{
	COLORREF skinColor = m_pBaseDesk->GetUserColor(0, CHART_SKIN_NAME, 1);
	if(skinColor == RGB(1, 0, 0))
		return 0;
	else if(skinColor == RGB(2, 0, 0))
		return 1;
	else if(skinColor == RGB(3, 0, 0))
		return 2;
	else if(skinColor == RGB(4, 0, 0))
		return 3;
	else if(skinColor == RGB(5, 0, 0))
		return 4;
	else if(skinColor == RGB(6, 0, 0))
		return 5;

	return -1;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetHtsPath
 - Created at  :  2005-05-10   15:48
 - Author      :  ������
 - Description :  Hts�� �н��� �����Ѵ�. (Ex- "C:/Hts/Ez-Net Plus")
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetHtsPath(LPCTSTR pcszHtsPath) 
{
	m_strHtsPath = pcszHtsPath;
	// (2007/2/16 - Seung-Won, Bae) Trim Right Backslash..
	if( m_strHtsPath.Right( 1) == "\\") m_strHtsPath = m_strHtsPath.Left( m_strHtsPath.GetLength() - 1);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateToolBar
 - Created at  :  2004-10-25   17:27
 - Author      :  ������ (����DLL)
 - Description :  ���ٸ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::CreateToolBar()
{	
	if(m_strToolbarOption.Right(2) != "\r\n")
		m_strToolbarOption += "\r\n";
	
	// (2008/2/4 - Seung-Won, Bae) for Other MetaText of MultiLanguage.
	m_strToolbarOption = _MTOMS( m_strToolbarOption, ";\r\n", 4);

	CString strToolBarOptionTmp = m_strToolbarOption;
	CString strInner = CDataConversion::GetStringData(strToolBarOptionTmp, CString( _MTEXT( C4_OUTER_TOOL_OPTION)) + "\r\n");
	int nCount = CDataConversion::GetStringDataCount(strInner, _T("\r\n"));
	nCount--;	// "����"��� ���ڿ� ���� ����� �� ������.

	// (2009/9/2 - Seung-Won, Bae) for Image Path
	//CString strImageFile = GetImagePath() + TOOLBAR_BUTTON_IMAGEFILE;
	//CString strImageFile = GetBaseDeskPath() + _T("\\Hanaro\\image\\drchart\\") + TOOLBAR_BUTTON_IMAGEFILE; //@Solomon:Directory-091113
	CString strImageFile = GetBaseDeskPath() + _T("\\Icss\\image\\drchart\\") + TOOLBAR_BUTTON_IMAGEFILE;	//IBK

	// (2006/2/8 - Seung-Won, Bae) Create from Regular DLL
	if( !m_pToolBarMgrHelper) m_pToolBarMgrHelper = new CToolBarMgrHelper;
	if( m_pToolBarMgrHelper && !m_pToolBarManager) m_pToolBarManager = m_pToolBarMgrHelper->CreateToolBarManager( m_pIChartCtrl);
	if( !m_pToolBarManager) return;

	m_pMainBlock->SetToolBarManager(m_pToolBarManager);

	m_pToolBarManager->SetControlData(m_pPacketList, m_pPacketListManager, m_pxScaleManager, m_pMainBlock, m_pIndicatorList);
	m_pToolBarManager->CreateToolBar(m_strToolbarOption, strImageFile);

	// ����(04/11/24) �׷�����Ÿ���� �ʱⰪ�� �����Ѵ�. (��Ÿ������)
	m_pToolBarManager->SetGraphType(CGraphBaseData::Bong_Type, 0);
}	

/*-----------------------------------------------------------------------------------------
 - Function    :  SetAllLogScale
 - Created at  :  2005-05-27   14:06
 - Author      :  ������
 - Parameters  :  bLogScale - TRUE: �α׽����Ϸ� ǥ��, FALSE: �ȷα׽����Ϸ� ǥ��
 - Description :  �α׽����Ͽɼ� ������ �ϴ� ��ƮDLL�� Interface�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetAllLogScale(BOOL bLogScale) 
{
	if(m_pMainBlock == NULL)
		return;

	if(m_pMainBlock->SetAllVertScaleLogType(bLogScale))
		InvalidateControl();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetLogScale
 - Created at  :  2005-08-22
 - Author      :  �輱��
 - Parameters  :  strGraphName - log�� ������ GraphName
								strGraphName == "" �� ��� : "������Ʈ"
				  bLogScale - TRUE: �α׽����Ϸ� ǥ��, FALSE: �ȷα׽����Ϸ� ǥ��
 - Description :  �α׽����Ͽɼ� ������ �ϴ� ��ƮDLL�� Interface�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::SetLogScale(LPCTSTR szGraphName2, BOOL bLogScale) 
{
	if(m_pMainBlock == NULL)
		return false;

	CString szGraphName = _STOM2( szGraphName2);
	CString strGraphName = szGraphName;
	if(strGraphName == "")
		strGraphName = _MTEXT( C2_PRICE_CHART);

	if(!m_pMainBlock->SetVertScaleLogType(strGraphName, bLogScale))
		return false;

	InvalidateControl();
	return TRUE;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetLogScale
 - Created at  :  2005-08-22
 - Author      :  �輱��
 - Parameters  :  strGraphName -  get �� GraphName
								strGraphName == "" �� ��� : "������Ʈ"
 - Description :  �ش� ��ǥ�� �ִ� block�� log ���� �ش�.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::GetLogScale(LPCTSTR szGraphName2) 
{
	if(m_pMainBlock == NULL)
		return false;

	CString szGraphName = _STOM2( szGraphName2);
	CString strGraphName = szGraphName;
	if(strGraphName == "")
		strGraphName = _MTEXT( C2_PRICE_CHART);

	int nR = 0, nC = 0, nV = 0, nG = 0;
	if( !m_pMainBlock->GetIndexOfGraph( szGraphName, nR, nC, nV, nG)) return FALSE;

	CBlock *pBlock = m_pMainBlock->GetBlock( CBlockIndex( nR, nC));
	if( !pBlock) return FALSE;
	return pBlock->IsLogScale();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  AddPacketAdapter
 - Created at  :  2005-06-01   11:22
 - Author      :  ������
 - Description :  ��Ŷ�� �߰��ϴ� AddIn Interface������. (Protected�� AddPacket�� Adapter)
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::AddPacketAdapter(const char* pcszPacketOption)
{
	AddPacket(pcszPacketOption);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  AddGraphAdapter
 - Created at  :  2005-06-07   15:44
 - Author      :  ������
 - Description :  Graph�� �߰��ϴ� AddIn Interface������. (Protected�� AddGraph�� Adapter)
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::AddGraphAdapter(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale)
{
	AddGraph(nRow, nColumn, nVertScaleGroup, szGraphOption, bCreateBlock, bCreateVertScaleGroup, bUseAutoScale);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeSubGraphData
 - Created at  :  2005-06-01   16:37
 - Author      :  ������
 - Parameters  :  pcszGraphName - ��ǥ��
 -                pcszSubGraphName  - ��ǥ�� ����׷�����( ex: RSI, RSI_signal ....)
 -                clrLineColor      - �׷��� ����
 -                nThickness        - �׷��� ���� �β�
 -                nDrawStyle        - �׷��� ���� ����.
 - Return Value:  bool - true:���漺��, false:�������.
 - Description :  �׷����� �Ӽ�(����, �β�, ����)���� �����ϴ� Interface.
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle)
{
	if(!m_pMainBlock)
		return false;

	// ���� �������¸� �����´�. (�ٲ�κи� �����ϸ� �Ǵϱ�!)
	CSubGraphData* pSubGraphData = m_pMainBlock->GetSubGraphData(pcszGraphName, pcszSubGraphName);
	if(!pSubGraphData)
		return false;

	// ���� ����.
	CGraphColor graphColor = pSubGraphData->GetColor();
	graphColor.SetColor1(clrLineColor);
	pSubGraphData->SetColor(graphColor);

	// �β� ����.
	CPenThickness penThickness = pSubGraphData->GetPenThickness();
	penThickness.SetThickness1(nThickness);
	pSubGraphData->SetPenThickness(penThickness);

	// DrawStyle ����.
	CGraphDrawStyle graphDrawStyle = pSubGraphData->GetDrawStyle();
	graphDrawStyle.SetDrawStyle(nDrawStyle);
	pSubGraphData->SetDrawStyle(graphDrawStyle);

	// SubGraphData ����.
	return m_pMainBlock->ChangeSubGraphData(pcszGraphName, pcszSubGraphName, *pSubGraphData);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartPeriodOfPacketAdapter
 - Created at  :  2005-06-07   15:48
 - Author      :  ������
 - Description :  Packet�� �������ڸ� �����ϴ� Interface�� �����Ѵ�. 
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::SetStartPeriodOfPacketAdapter(const char* szPacketName, long lStartPeriod, BOOL bAddSpace)
{
	return SetStartPeriodOfPacket(szPacketName, lStartPeriod, bAddSpace) ? true : false;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartIndexOfPacket
 - Created at  :  2005-08-27   17:31
 - Author      :  ������
 - Description :  Packet�� �������ڸ� �����ϴ� Interface�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::SetStartIndexOfPacket(const char* pcszRQ, const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace)
{
	if(pcszRQ == NULL)
		return FALSE;

	CPacketList* pPacketList = m_pPacketListManager->GetPacketList(pcszRQ);
	
	// 1. ���� Packet List�� Ȯ���Ѵ�.
	if( !pPacketList) return FALSE;

	// 2. ������ Packet�� �˻��Ѵ�.
	CPacket *pPacket = pPacketList->GetBaseData( pcszPacketName);
	if( !pPacket) return FALSE;
	
	// 3. �˻��� Packet�� �������ڿ� �ش��ϴ� Data Index�� ���Ѵ�.
	CPacket *pTimePacket = pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pTimePacket) return FALSE;

	// 4. p_bAddSpace�� ���� ���� Index ��ŭ Packet Data�� Shift�Ѵ�.
	// (2004.06.18, ��¿�) ���Ŀ� �ѹ��� ������ �߰��ϸ鼭, StartPeriod�� ������ �� �ֵ���
	//		Packet�� �߰��ϴ� ������ ���� StartIndex�� �ƴ�, �ڷ����ڿ��� Data�� ���̷� �Ѵ�.
	if(bAddSpace)
	{
		CList<double, double>* pDblList = pPacket->GetnumericDataList();
		if( !pDblList) return FALSE;
		int nAddingCount = pTimePacket->GetCount() - pPacket->GetCount();
		for( int i = 0; i < nAddingCount; i++) pDblList->AddHead( 0.0);
	}

	// TEST: ����(05/08/27) ���� Index�������� ��� 0.0���� �����Ѵ�.
//	CList<double, double>* pTmpList = pPacket->GetnumericDataList();
//	POSITION pos;
//	for(int i = 0; i < nStartDataIndex; i++)
//	{
//		pos = pTmpList->FindIndex(i);
//		if(!pos)
//			continue;
//		pTmpList->SetAt(pos, 0.0);
//	}

	// 4. Packet�� ���� Index�� �����Ѵ�.
	pPacket->SetStartPeriod( nStartDataIndex);

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  HasGraphAdapter
 - Created at  :  2005-06-07   15:50
 - Author      :  ������
 - Description :  �ش� �׷����� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::HasGraphAdapter(const char* szGraphName)
{
	return HasGraph(szGraphName) ? true : false;
}




/*-----------------------------------------------------------------------------------------
 - Function    :  OnPricechartMinmax
 - Created at  :  2005-06-14   16:15
 - Author      :  ������
 - Description :  '������Ʈ���� �ְ�/������' �޴� ���ý�
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnPricechartMinmax() 
{
	m_bShowMinMaxbyPrice = !m_bShowMinMaxbyPrice;

	if(m_pMainBlock == NULL)
		return;

	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(m_pRscFont);
	m_pMainBlock->SetOnlyPriceVertScale(&dc, m_bShowMinMaxbyPrice == TRUE);
	dc.SelectObject(pOldFont);
	InvalidateControl();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnUpdatePricechartMinmax
 - Created at  :  2005-06-14   16:16
 - Author      :  ������
 - Description :  '������Ʈ���� �ְ�/������' �޴� ������� ������Ʈ
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnUpdatePricechartMinmax(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( FALSE);

	// (2006/11/12 - Seung-Won, Bae) Use Current Mouse Position's VScale Info
	if( !m_pMainBlock) return;

	int nR = 0, nC = 0, nV = 0, nG = 0;
	CString strGraphName;
	if( !m_pMainBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, strGraphName, "DEFAULT")) return;

	CBlock *pBlock = m_pMainBlock->GetBlock( CBlockIndex( nR, nC));
	if( !pBlock) return;

	BOOL bIsLock = FALSE;
	CScaleBaseData::VERTMINMAXTYPE eVertMinMaxType = CScaleBaseData::VERT_MINMAX_OF_VIEW;
	double dMin = 0.0, dMax = 0.0;
	if( !pBlock->GetVScaleMinMaxSetting( nV, bIsLock, eVertMinMaxType, dMin, dMax)) return;

	if( eVertMinMaxType != CScaleBaseData::VERT_MINMAX_OF_USER)
	{
		pCmdUI->SetCheck( m_bShowMinMaxbyPrice);
		pCmdUI->Enable( TRUE);
	}
	else if( pCmdUI->m_pMenu)
	{
		MENUITEMINFO mii;
		ZeroMemory( &mii, sizeof( MENUITEMINFO));
		mii.cbSize = sizeof( MENUITEMINFO); // must fill up this field
		mii.fMask = MIIM_DATA; 
		mii.dwItemData = 9999; 
		SetMenuItemInfo( pCmdUI->m_pMenu->GetSafeHmenu(), pCmdUI->m_nID, FALSE, &mii); 
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnScreenMinmax
 - Created at  :  2005-06-14   16:16
 - Author      :  ������
 - Description :  '(���纸�̴� ��Ʈ)ȭ����� �ְ�/������' �޴� ���ý�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnScreenMinmax() 
{
	OnFullMinMax( m_bVScaleMinMaxOfView ? 1 : 0);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnUpdateScreenMinmax
 - Created at  :  2005-06-14   16:17
 - Author      :  ������
 - Description :  'ȭ����� �ְ�/������' �޴� ���ý�...
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnUpdateScreenMinmax(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( FALSE);
	m_bVScaleMinMaxOfView = FALSE;

	// (2006/11/12 - Seung-Won, Bae) Use Current Mouse Position's VScale Info
	if( m_pMainBlock)
	{
		CBlockIndex bi = m_pMainBlock->FindBlockIndex( m_ptRButtonMenuPopupPosition);
		if( bi.GetRow() < 0 || bi.GetColumn() < 0) bi.SetIndex( 0, 0);
		CBlock *pBlock = m_pMainBlock->GetBlock( bi);
		if( pBlock)
		{
			int nVScaleIndex = pBlock->GetFirstVisibleVScaleIndex();
			if( 0 <= nVScaleIndex)
			{
				BOOL bIsLock = FALSE;
				CScaleBaseData::VERTMINMAXTYPE eVertMinMaxType = CScaleBaseData::VERT_MINMAX_OF_VIEW;
				double dMin = 0.0, dMax = 0.0;
				if(pBlock->GetVScaleMinMaxSetting( nVScaleIndex, bIsLock, eVertMinMaxType, dMin, dMax))
				{
					m_bVScaleMinMaxOfView = ( eVertMinMaxType == CScaleBaseData::VERT_MINMAX_OF_VIEW);
					pCmdUI->SetCheck( m_bVScaleMinMaxOfView);
					pCmdUI->Enable( !bIsLock);
					// (2008/3/7 - Seung-Won, Bae) If Locked, Do not delete menu. Just let it be disabled.
					if( pCmdUI->m_pMenu && bIsLock)
					{
						MENUITEMINFO mii;
						ZeroMemory( &mii, sizeof( MENUITEMINFO));
						mii.cbSize = sizeof( MENUITEMINFO); // must fill up this field
						mii.fMask = MIIM_DATA; 
						mii.dwItemData = 9999; 
						SetMenuItemInfo( pCmdUI->m_pMenu->GetSafeHmenu(), pCmdUI->m_nID, FALSE, &mii); 
					}
				}
			}
		}
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetListViewerData
 - Created at  :  2005-06-27   10:08
 - Author      :  ������
 - Parameters  :  bstrTitle               - [out] Ÿ��Ʋ
 -                bstrHorzScale           - [out] ���ν����� ��Ŷ��
 -                bstrPacketNamesPerBlock - [out]
 -                bstrPacketNames         - [out] ��Ŷ��
 -                bstrSaveFilePath        - [out] user���� ���� ��ġ
 -                bViewPacketSequence     - [out]
 -                lpPacketList            - [out]
 - Description :  �ܺ�DLL(ex:��Ƽ��ƮDLL)�� DataListViwer���� ����� ����Ÿ���� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::GetListViewerData(BSTR FAR* bstrTitle, 
									  BSTR FAR* bstrHorzScale, 
									  BSTR FAR* bstrPacketNamesPerBlock, 
									  BSTR FAR* bstrPacketNames, 
									  BSTR FAR* bstrSaveFilePath, 
									  BOOL FAR* bViewPacketSequence, 
									  long FAR* lpPacketList) 
{
	_ML_TEST_NOT_USED();

	CString szTitle, szPacketNames, szPacketNames2, strHorzScale;
	CSubGraphViewDataList pViewDataList;
	CViewData viewData;
	m_pMainBlock->GetViewDataListInAllBlock( NULL, viewData);
	if(viewData.GetColumnViewDataCount() <= 0)
		return;

	for(int i = 0; i < m_pMainBlock->GetColumnCount(); i++)
	{
		for(int j = 0; j < viewData.GetRowViewDataCount(i); j++)
		{
			CRowViewData& rowViewData = viewData.GetAt(i, j);
			//nami 9.2
			//����� ���ν������� �ϳ��� ���̰� �ϰ� �ִ�.
			CString strData = rowViewData.GetHorzScaleName();
			
			if(!strData.IsEmpty()) 
				strHorzScale = strData;
			//nami end
			for(int k = 0 ; k < rowViewData.GetSubGraphViewDataCount(); k++)
			{
				POSITION listPos = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().FindIndex(k);
				if(listPos)
				{
					CSubGraphViewData pSubGraph = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().GetNext(listPos);
					szTitle += pSubGraph.GetTitle() + ";" ;
					szPacketNames += pSubGraph.GetPacketNames() + ";" ;
					szPacketNames2 += pSubGraph.GetPacketNames() ;
				}			
			}//end of for(int k = 0...
		}//end of for(int j = 0...
	}//end of for(int i = 0...

	CString strUserFilePath = GetUserFilePath();

	*bstrTitle = szTitle.AllocSysString();
	*bstrHorzScale = strHorzScale.AllocSysString();
	*bstrPacketNamesPerBlock = szPacketNames.AllocSysString();
	*bstrPacketNames = szPacketNames2.AllocSysString();
	*bstrSaveFilePath = strUserFilePath.AllocSysString();
	*bViewPacketSequence = FALSE;
	*lpPacketList = (long)m_pPacketList;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetDataIndexByDate
 - Created at  :  2005-06-27   16:35
 - Author      :  ������
 - Parameters  :  pcszStartDate - ��������(��Ʈ�� ǥ���� ��������)
 -                pcszEndDate   - ������(��Ʈ�� ǥ���� ������)
 - Description :  �ڷ����ڸ� �޾Ƽ� �ش����ں���(�Ǵ� �ش����ڱ���) ����Ÿ�� ǥ���Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetDataIndexByDate(LPCTSTR pcszStartDate, LPCTSTR pcszEndDate) 
{
	_ML_TEST_NOT_USED();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetRunTimeMode
 - Created at  :  2005-06-29   13:15
 - Author      :  ������
 - Parameters  :  bRunTimeMode - TRUE: RunTime, FALSE: DesignTime
 - Description :  ��Ʈ���� ���� RunTime���� DesignTime������ �˷��ִ� �޼ҵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetRunTimeMode(BOOL bRunTimeMode) 
{
	m_bRunTimeMode = bRunTimeMode;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsRunTimeMode
 - Created at  :  2005-06-29   13:19
 - Author      :  ������
 - Return Value:  BOOL - TRUE: RunTime Mode, FALSE: DesignTime Mode.
 - Description :  ���� ��Ʈ�� ������°� DesignTime���� RunTime������ �����Ѵ�.
 - Note		   :  ��ȭ�� ������ ��ƮOCX�� �÷��� ����� ��� OCX�� Design Time�� RunTime��
				  ������ ���� ���Ѵ�. �׷��� �ܺ�DLL���� ��Ʈ�� ���������� �� Running Mode��
				  �˷��ش�. ���� AmbientUserMode()�� ����Ϸ� �Ҷ� ���⸸ ��ġ�� ��.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::IsRunTimeMode() const
{
	return m_bRunTimeMode;
//	return AmbientUserMode();
}

BSTR CKTBChartCtrl::GetPacketOption() 
{
	CString strResult = m_strPacketOption;

	return strResult.AllocSysString();
}

void CKTBChartCtrl::SetPacketOption(LPCTSTR strPacketOption) 
{
	_ML_TEST_NOT_USED();
	m_strPacketOption = strPacketOption;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CalculateIndicator
 - Created at  :  2005-07-11   09:41
 - Author      :  ������
 - Description :  ��ǥ�� ����϶�� ������ǥ�� ȣ���Ѵ�. (��ǥ�߰�/��ǥ����)
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList)
{
	if(pIndicatorInfo == NULL || packetList.GetCount() <= 0)
		return false;

	// (2006/1/9 - Seung-Won, Bae) Packet Interface will be queried in AddIn
	long lResult = 0;
	if( m_pAddInManager)
	{
		m_pAddInManager->OnAddInToolCommand( lResult, EAI_ST_CALCULATE_INDICATOR, pIndicatorInfo->GetIndicatorName(), pIndicatorInfo->GetGraphName(), ( CObject *)&packetList);

		//{{2007.06.01 by LYH �������� �ε��� ��ǥ ��� ���ؼ� ����
//		// ���� RQ���� : �������� - ojtaso (20070308)
//		if(m_pPacketListManager->IsSameRQ(m_strRQ, ( CGraphImp *)pIndicatorInfo->GetGraph()))
//		{
//			// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
//			m_pAddInManager->OnAddInToolCommand( lResult, EAI_ST_CALCULATE_INDICATOR, m_strRQ, pIndicatorInfo->GetIndicatorName(), pIndicatorInfo->GetGraphName(), ( CObject *)&packetList);
//		}
		CGraph* pGraph = (CGraph*)pIndicatorInfo->GetGraph();
		if( !pGraph) return false;	// (2009/9/20 - Seung-Won, Bae) if it is NULL, what happen?
		CString strRQ = pGraph->GetGraphRQ();
		m_pAddInManager->OnAddInToolCommand( lResult, EAI_ST_CALCULATE_INDICATOR, strRQ, pIndicatorInfo->GetIndicatorName(), pIndicatorInfo->GetGraphName(), ( CObject *)&packetList);
		//}}

	}
	return lResult;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetWndHandle
 - Created at  :  2005-07-11   14:12
 - Author      :  ������
 - Description :  OCX�� �������ڵ��� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
HWND CKTBChartCtrl::GetWndHandle(void) const
{
	return this->m_hWnd;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetOutsideCommonInterface
 - Created at  :  2005-07-12   14:35
 - Author      :  ������
 - Parameters  :  lpOutsideCommonInterface - �ܺ�DLL�� ������.
 - Description :  �ܺ�DLL(ex: MultiChart, ��ǥ���� DLL��)�� �Լ��� ȣ���ϱ� ���� �����͸� ȹ���Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetOutsideCommonInterface(long lpOutsideCommonInterface) 
{
	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
	if( !m_strHtsPath.IsEmpty())
		m_OutsideCommonInterfaceFunction.SetOutsideCommonInterface( ( COutsideCommonInterface *)lpOutsideCommonInterface, GetDefaultPath());
}


/*-----------------------------------------------------------------------------------------
 - Function    :  RemoveGraph_Title
 - Created at  :  //sy 2005.07.15
 - Author      :  �輱��
 - Description :  title�� �ش�Ǵ� graph�� ���� �Ѵ�.
				  �� nRowIndex�� nColumnIndex �� ��� 0 �̻��� ��쿣 �ش� Block���� �����Ѵ�.
				  bIsOnlyNoTRData = true : TR data �� �ƴ� packet���� �����Ѵ�.
				  bIsAutoHorzScalePosition = true : x�� scale�� ��ġ�� �⺻������ �����Ѵ�.
					(-> ������ block�� x�� scale�� ���̰� ������ block�� text �������� �Ѵ�.)
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::RemoveGraph_Title(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle2, BOOL bIsOnlyNoTRData, BOOL bIsAutoHorzScalePosition) 
{
	if(m_pMainBlock == NULL)
		return false;

	CString szGraphTitle = _STOM1( szGraphTitle2);
	CString strPacketName = GetGraphPacketName(nRowIndex, nColumnIndex, szGraphTitle);
	if( !m_pMainBlock->DeleteGraphWithSubGraphTitle( CBlockIndex( nRowIndex, nColumnIndex), szGraphTitle, (bIsOnlyNoTRData ? true: false)))
		return false;

	// x�� scale�� ��ġ�� �⺻���·� �ٲ��ش�. -> ���� �ϴ��� block�� scale ����.
	if(bIsAutoHorzScalePosition) m_pMainBlock->SetBaseHorzScalePosition();

	// bIsOnlyNoTRData = false : TR data���ο� ������� packet ����
	if(m_pPacketList != NULL) m_pPacketList->DeleteOrClearPacket(strPacketName, true, (bIsOnlyNoTRData ? true: false));

	InvalidateControl();
	return true;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetGraphPacketName
 - Created at  :  //sy 2005.07.15
 - Author      :  �輱��
 - Description :  title�� �ش�Ǵ� graph�� ���� �Ѵ�.
				  �� nRowIndex�� nColumnIndex �� ��� 0 �̻��� ��� : ��ü block �� �ش� 
				  title�� �ִ� graph�� packet name�� �ش�.
 -----------------------------------------------------------------------------------------*/
BSTR CKTBChartCtrl::GetGraphPacketName(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle2) 
{
	CString strResult;
	if(m_pMainBlock == NULL)
		return strResult.AllocSysString();

	CString szGraphTitle = _STOM1( szGraphTitle2);
	// block ��ġ�� ������� �ش� graph title�� packet name.
	if(nRowIndex < 0 || nColumnIndex < 0)
	{
		strResult = m_pMainBlock->GetGraphPacketName_TitleName(szGraphTitle);
	}
	else
	{
		strResult = m_pMainBlock->GetGraphPacketName_TitleName(CBlockIndex(nRowIndex, nColumnIndex), szGraphTitle);
	}

	return strResult.AllocSysString();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeGraphTitle2
 - Created at  :  //sy 2005.07.15
 - Author      :  �輱��
 - Description :  title�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::ChangeGraphTitle2(LPCTSTR szTargetTitle2, LPCTSTR szSourceTitle2) 
{
	if(m_pMainBlock == NULL)
		return false;

	CString szTargetTitle = _STOM1( szTargetTitle2);
	CString szSourceTitle = _STOM1( szSourceTitle2);
	if(!m_pMainBlock->ChangeSubGraphTitle(szTargetTitle, szSourceTitle))
		return false;

	InvalidateControl();
	return true;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  PartitionAllGraph
 - Created at  :  //sy 2005.07.18
 - Author      :  �輱��
 - Description :  ����Ʈ���� ���� �Լ��� 1���� graph�� �Ѱ��� Block���� ���ҵȴ�.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::PartitionAllGraph() 
{
	if(m_pMainBlock == NULL)
		return false;

	// ��� block�� �ϳ��� block���� �����.
	m_pMainBlock->MoveAllVertScaleGroupsToHeadBlock();
	// �ϳ��� block�� �ִ� n���� graph�� n���� vertScaleGroup�� �����.
	m_pMainBlock->JoinAllVertScaleGroups(CBlockIndex(0, 0), false);
	// n���� vertScaleGroup�� n���� block���� �����.
	if(!m_pMainBlock->MoveAllVertScaleGroupsToEachRowBlock(CBlockIndex(0, 0), true))
		return false;

	InvalidateControl();
	return true;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  JoinAllGraph
 - Created at  :  //sy 2005.07.18
 - Author      :  �輱��
 - Description :  ����Ʈ���� ���� �Լ��� n���� graph�� �Ѱ��� Block���� ��ø�ȴ�.
				  bIsOneVertScaleGroup = true : 1���� vertScaleGroup
				  bIsOneVertScaleGroup = false : n���� vertScaleGroup
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::JoinAllGraph(BOOL bIsOneVertScaleGroup) 
{
	if(m_pMainBlock == NULL)
		return false;

	// �������� block�� �ϳ��� block���� �����.
	bool bResult = m_pMainBlock->MoveAllVertScaleGroupsToHeadBlock(true);

	// �ϳ��� vertScaleGroup �� ���´�.
	if(bIsOneVertScaleGroup)
	{
		if(m_pMainBlock->JoinAllVertScaleGroups(CBlockIndex(0, 0), true))
			bResult = true;
	}
	else
	{
		if(m_pMainBlock->JoinAllVertScaleGroups(CBlockIndex(0, 0), false))
			bResult = true;
	}

	if(bResult)
		InvalidateControl();

	return bResult;
}

// (2006/11/22 - Seung-Won, Bae) Do not support the Method 'ChangeAllFunctionName' anymore.
BOOL CKTBChartCtrl::NotUsedMethod3(LPCTSTR szTargetGraphName, LPCTSTR szSourceGraphName, BOOL bKeepOldGraphColor) 
{
	_ML_TEST_NOT_USED();
	return FALSE;
}

CIndicatorList* CKTBChartCtrl::GetIndicatorList(void)
{
	return m_pIndicatorList;
}

CPacketList* CKTBChartCtrl::GetPacketList(void)
{
	return m_pPacketList;
}

// PacketListManager : �������� - ojtaso (20070110)
CPacketListManager* CKTBChartCtrl::GetPacketListManager(void)
{
	return m_pPacketListManager;
}

CMainBlock* CKTBChartCtrl::GetMainBlock(void)
{
	return m_pMainBlock;
}

CXScaleManager* CKTBChartCtrl::GetXScaleManager(void)
{
	return m_pxScaleManager;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeIndicatorPacketName
 - Created at  :  2005-08-04 
 - Author      :  �輱��
 - Parameters  :  strGraphName - target indicator
				  nSubGraphIndex   - target subGraph index
									 0 ���� �����ϸ� -1�� �ð��� �� subGraph ���� �� graph packet name ����
				  strNewPacketName - ������ packetName
 - Return Value:  BOOL - �ùٸ��� packet name�� ����� ��� true return.
 - Description :  �ش� indicator �� packet name�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
//sy 2005.08.04
BOOL CKTBChartCtrl::ChangeIndicatorPacketName( LPCTSTR szIndicatorName2, short nSubGraphIndex, LPCTSTR strNewPacketName2) 
{
	if( !szIndicatorName2 || !m_pMainBlock || !m_pIndicatorList) return FALSE;
	if( !*szIndicatorName2) return FALSE;

	CString szIndicatorName = _STOM2( szIndicatorName2);
	CString strNewPacketName;
	if( strchr( strNewPacketName2, '('))	strNewPacketName = _STOMS( strNewPacketName2, ",(+)/23", 0);
	else									strNewPacketName = _STOMS( strNewPacketName2, ",", 0);
	// (2006/12/13 - Seung-Won, Bae) It will be processed in MainBlock.
	//		Caution! In MainBlock, Indicator List will be Updated.
	//		In Using Policy of Dual Chart Graph Data. You must set in Indicator List and Update MainBlock with IndicatorList.
	//		But, I did not like that, because of performance.
	if( !m_pMainBlock->ChangeIndicatorPacketNames( szIndicatorName, nSubGraphIndex, strNewPacketName)) return FALSE;
	InvalidateControl();
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RequestTRPacket
 - Created at  :  2005-08-08   15:01
 - Author      :  ������
 - Parameters  :  pcszPacketNames - ��Ŷ��.
 - Description :  �ش� ��Ŷ������ TR����Ÿ�� ��û�Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::RequestTRPacket(const CString& strGraphName, const CString& strPacketNames)
{
	CString strPacketNamesForTR;
	if(!GetRequestTRPacketName(strPacketNames, strPacketNamesForTR))
		return;

	m_OutsideCommonInterfaceFunction.Outside_RequestTRPacket(m_nID, strPacketNamesForTR, 0, true);
}

//sy 2005.11.14.
void CKTBChartCtrl::RequestTRPacket(const CString& strGraphName, const CString& strPacketNames, const int nCount, const bool bIsNew)
{
	if(nCount <= 0)
		return;

	CString strPacketNamesForTR;
	if(!GetRequestTRPacketName(strPacketNames, strPacketNamesForTR))
		return;

	//CString strTemp;
	//strTemp.Format("%s : count(%d) : new(%d)", strPacketNamesForTR, nCount, bIsNew);
	//AfxMessageBox(strTemp);
	m_OutsideCommonInterfaceFunction.Outside_RequestTRPacket(m_nID, strPacketNamesForTR, nCount, bIsNew);
}

bool CKTBChartCtrl::GetRequestTRPacketName(const CString& strPacketNames, CString& strPacketNamesForTR) const
{
	if(strPacketNames.IsEmpty())
		return false;

	//TR packet ������ ";" �� �ϱ� ������ ',' -> ';' �� ����.
	strPacketNamesForTR = strPacketNames;
	strPacketNamesForTR.Replace(',', ';');
	//"xx_�ڷ�����" �� �����ش�.
	if(strPacketNamesForTR.Find( _MTEXT( C0_DATE_TIME)) < 0)
	{
		CString strPacketName = CDataConversion::GetLeftToTrimDataIndex(strPacketNamesForTR, ';');
		if(strPacketName.Find('_') > 0)
			strPacketName = CDataConversion::GetLeftToTrimDataIndex(strPacketNamesForTR, '_');
	
		strPacketNamesForTR = strPacketName + "_" + _MTEXT( C0_DATE_TIME) + ';' + strPacketNamesForTR;
	}

	return !(strPacketNamesForTR.IsEmpty());
}
//sy end


/*-----------------------------------------------------------------------------------------
 - Function    :  RequestTRPacket
 - Created at  :  2005-08-22
 - Author      :  �輱��
 - Parameters  :  eEtcMessageType - message type
				  nData - ����� data
 - Description :  scale setting �� �����ؼ� Dll�� ���� �˸���.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnScaleSettingMessage(const CCommonInterfaceEnum::ETCMESSAGETYPE eEtcMessageType, const int nData)
{
	if(m_pMainBlock == NULL)
		return;

	// "������Ʈ" log ���� - ������Ʈ�� ��츸 �޽����� �´ٴ� ������
	if(eEtcMessageType == CCommonInterfaceEnum::PRICE_LOG)
		m_OutsideCommonInterfaceFunction.MChart_SetPriceLog((nData == 1 ? true: false));
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RunEnvironmentDlg
 - Created at  :  2005-08-09   16:21
 - Author      :  ������
 - Description :  ȯ�漳��â�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::RunEnvironmentDlg(short nOption) 
{
	RunToolFunction( CToolOptionInfo::T_ENVIRONMENT_CFG, 1);
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2005.08.11
//
//	����	:	(2005.08.11) Indicator Condition�� Draw Condition�� �����Ǿ�,
//					Condition ���� ��û��, Calculate Condition���� ��������Ѵ�.
//					(Draw Condition�� 0, Calculate Condition�� 1�� �����Ѵ�.)
///////////////////////////////////////////////////////////////////////////////
BOOL CKTBChartCtrl::SetIndicatorCondition(LPCTSTR strGraphName2, short nConditionType, short nConditionIndex, double dConditionValue, BOOL bReCalculate) 
{
	if( m_pIndicatorList == NULL || m_pPacketList == NULL || m_pMainBlock == NULL) return FALSE;

	CString strGraphName = _STOM2( strGraphName2);
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
	if( pIndicatorInfo == NULL) return false;
	
	bool bResult = false;
	if(nConditionType == 1)  bResult = (pIndicatorInfo->SetCommonCalConditionData((int)nConditionIndex, dConditionValue) >= 0);
	else  bResult = (pIndicatorInfo->SetDrawConditionData((int)nConditionIndex, dConditionValue) >= 0);
	if(bReCalculate && bResult)
	{
		m_pMainBlock->CalculateGraph( strGraphName, CCalculateBaseData::TRDATA_CAL);
		InvalidateControl();
	}

	return false;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetPacketData2
 - Created at  :  2005-08-12
 - Author      :  �輱��
 - Description :  ��ũ������ packet�� data �Է�. ��, ��� packet�� clear���� �ʴ´�.
				  strPacketType : ����� �����޽����� "RESETPACKET"�� �ش��"
			 ex)	CString strPacketNames = "�����ż�";
					CString strPacketTypes = "�����ż�:8|@";
					CString strPacketData = "   10.00   30.00";
					SetPacketData2(strPacketNames, strPacketTypes, strPacketData, 2, strPacketData)
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::SetPacketData2(LPCTSTR strPacketName, LPCTSTR strPacketType, LPCTSTR strPacketData, short nDataCount, BOOL bReCalculate) 
{
	if(!g_pDRDebuger) gfnGetDRDebuger();
	if(g_pDRDebuger) g_pDRDebuger->DRLog_Message("ChartCtrl::SetPacketData2");			//[alzioyes:����������,DRFN Platform �αױ���߰�]
	//g_DRDebuger.DRLog_Message("ChartCtrl::SetPacketData2");			//[alzioyes:����������,DRFN Platform �αױ���߰�]

	if(m_pPacketList == NULL)
		return FALSE;

	CString strPacketName2 = _STOM0( strPacketName);
	bool bResult = m_pPacketList->SetPacketData(strPacketName2, strPacketType, strPacketData, (int)nDataCount);
	if(!bResult)
		return FALSE;

	if(bReCalculate && m_pMainBlock != NULL)
		m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::TRDATA_CAL);

	// (2006/7/20 - Seung-Won, Bae) Change ScrollInfo after Calculating Indicator for Special One Chart.
	ResetScroll( TRUE);
	InvalidateControl();

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetPacketHelpMessage
 - Created at  :  2005-08-23
 - Author      :  �輱��
 - Description :  ��ũ������ packet�� �����޽��� �Է�.
   ����		   :  strHelpMsg  - �����޽���
				  bIsAllRemove	- ��� �����޽����� ����� strHelpMsg �� �ٽ� ���������� ���� ����.
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::SetPacketHelpMessage(LPCTSTR strHelpMsg, BOOL bIsAllRemove) 
{
	if(m_pPacketList == NULL)
		return false;

	return m_pPacketList->SetHelpMessage(strHelpMsg, bIsAllRemove);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/2
// Modifier		: 
// Comments		: Invoke the Interface of AddIn with String Data
//-----------------------------------------------------------------------------
BOOL CKTBChartCtrl::InvokeAddInStr(LPCTSTR p_szAddInDllName, LPCTSTR p_szCommandName, LPCTSTR p_szData) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->InvokeAddInStr( p_szAddInDllName, p_szCommandName, p_szData) ;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnMenuCrossline
 - Created at  :  2005-09-05   10:22
 - Author      :  ������
 - Description :  ���ڼ��޴� ���ý�
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnMenuCrossline() 
{
	if( m_nNumericalInquiryOption & CROSSLINEVIEW)	m_nNumericalInquiryOption &= ~CROSSLINEVIEW;
	else											m_nNumericalInquiryOption |= CROSSLINEVIEW;

	// (2006/2/9 - Seung-Won, Bae) Notify AddIn Tools
	//		Caution! Do not use SetNumericalInquiryOption().
	//			It did not support Drag Cross Line without NIDLG.
	// (2006/3/3 - Seung-Won, Bae) Do not check. Because It can use with Script and Inner ToolBar's Controlling.
//	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_ALL_ON_CHANGE_CURSOR_MODE, m_nNumericalInquiryOption);
	SetNumericalInquiryOption( m_nNumericalInquiryOption);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnUpdateMenuCrossline
 - Created at  :  2005-09-05   10:23
 - Author      :  ������
 - Description :  ���ڼ��޴� üũ���� ����
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnUpdateMenuCrossline(CCmdUI* pCmdUI) 
{
	if( m_nNumericalInquiryOption & CROSSLINEVIEW) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnMenuDatawindow
 - Created at  :  2005-09-05   10:23
 - Author      :  ������
 - Description :  ��ġ��ȸâ�޴� ���ý�
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnMenuDatawindow() 
{
	if(m_nNumericalInquiryOption & NUMERICALINQUIRYDLG)	m_nNumericalInquiryOption &= ~NUMERICALINQUIRYDLG;
	else												m_nNumericalInquiryOption |= NUMERICALINQUIRYDLG;

	// (2006/2/9 - Seung-Won, Bae) Notify AddIn Tools
	//		Caution! Do not use SetNumericalInquiryOption().
	//			It did not support Drag Cross Line without NIDLG.
	// (2006/3/3 - Seung-Won, Bae) Do not check. Because It can use with Script and Inner ToolBar's Controlling.
//	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_ALL_ON_CHANGE_CURSOR_MODE, m_nNumericalInquiryOption);
	SetNumericalInquiryOption( m_nNumericalInquiryOption);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnUpdateMenuDatawindow
 - Created at  :  2005-09-05   10:24
 - Author      :  ������
 - Description :  ��ġ��ȸâ�޴� üũ���� ����
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnUpdateMenuDatawindow(CCmdUI* pCmdUI) 
{
	if(m_nNumericalInquiryOption & NUMERICALINQUIRYDLG) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetHelpMessageRealDataRate
 - Created at  :  2005-09-06
 - Author      :  �輱��
 - Description :  RDC=������:�ݿø��ڸ�:���������Ŷ��
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::SetHelpMessageRealDataRate(LPCTSTR strHelpMessageData, BOOL bIsRemove) 
{
	if(m_pPacketList == NULL)
		return false;

	_ML_TEST_NOT_USED();
	return m_pPacketList->SetHelpMessageRealDataRate(strHelpMessageData, bIsRemove);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetBongCountMargin
 - Created at  :  2005-09-07   10:40
 - Author      :  ������
 - Description :  �����ʿ����� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetBongCountMargin(short nBongCountMargin) 
{
	//sy 2005.11.18.
	m_nBongCountMargin = nBongCountMargin;
	ResetScroll();
	//sy end
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetBongCountMargin
 - Created at  :  2005-11-07
 - Author      :  �輱��
 - Description :  packet data ����.
					bIsAbsoluteClear = true : TRdata ������� ����
					bIsAbsoluteClear = false : TRdata �ƴ� ��츸 ����
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::ClearPacketData(LPCTSTR strPacketName2, BOOL bIsAbsoluteClear) 
{
	if(m_pPacketList == NULL)
		return false;

	CString strPacketName = _STOM0( strPacketName2);
	if(m_pPacketList->ClearData(strPacketName, ",", bIsAbsoluteClear) == 1)
		return true;

	return false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetOnePageDataCountInTRCount
 - Created at  :  2005-12-08
 - Author      :  �輱��
 - Description :  packet data ����.
					bIsTRCount = true : ȭ�鿡 ���̴� data ������ TRdata ���� ��
					bIsTRCount = false : ȭ�鿡 ���̴� data ���� ���
 -----------------------------------------------------------------------------------------*/
void CKTBChartCtrl::SetOnePageDataCountInTRCount(BOOL bIsTRCount, BOOL bIsNowChange) 
{
	//sy 2005.12.05.
	m_bOnePageDataCountInTRCount = (bIsTRCount ? true: false);
	if( bIsNowChange) ResetScroll();
}

// ------------------------------------------------------------------------------------------
// bds test - KTBChart ����

int CKTBChartCtrl::OpenFileAndSave(CString& strOldFileName, const CString& strNewFileName, const bool bIsCurrentSave, const CSaveUserFile::FILETYPE eFileType) 
{
	if(strNewFileName.IsEmpty() || !IsRunTimeMode())
		return -1;

	// ��Ʈ Ȯ���ڸ� �����ش�.
	CString strRealNewFileName = strNewFileName;
	if(strRealNewFileName.Find(".") < 0){
		strRealNewFileName += FILETYPE;
	}

	// ������ ������ file�� ����
	if(bIsCurrentSave)
		WriteUserFile(eFileType);

	// ���ο� ���� ����(file�� open)
	strOldFileName = strRealNewFileName;
	return ReadUserFile(eFileType);
}


void CKTBChartCtrl::SetAllGraphHide(short nRowIndex, short nColumnIndex, short bHide) 
{
	if(m_pMainBlock == NULL)
		return;

	m_pMainBlock->SetAllGraphsHiding(CBlockIndex(nRowIndex, nColumnIndex), (bHide == TRUE? true: false));
	InvalidateControl() ;
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/3/15
// Modifier		: 
// Comments		: Load Static Indicator Condition File for Preloading
//-----------------------------------------------------------------------------
void CKTBChartCtrl::InitStaticIndiCond(LPCTSTR p_szBasePath) 
{
	// TODO: Add your dispatch handler code here

	// (2004.04.02, ��¿�) ù ��Ʈ ȭ�鿡���� CIndicatorList�� ���� �ӵ��� ������ �ϱ� ���Ͽ�,
	//		Main�� �⺻ Loading�ÿ� ù CIndicatorList�� �����ǵ��� ó���Ѵ�. (�����ߴٰ� �ٷ� �����Ų��.)
	//		(Main���� InitStaticIndiCond()�� ȣ���� ��� �Ѵ�.
	//		 ����! Main���� ȭ�鿡 ������ ChartOCX���� ȣ��� Indicator List�� Info�� �ڷᱸ���� �ջ�Ǿ�,
	//			�������� �߻��Ѵ�.)
	CString savepath;
	CString filepath = CString( p_szBasePath) + DATA_FILE_FOLDER + FILE_INDICATOR_CONDITIONS ;
	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	CIndicatorList *pIndicatorList = new CIndicatorList( GetSafeHwnd(), filepath, savepath, "", ( CChartInfo::CHART_MODE)m_nChartMode);
	delete pIndicatorList;
}

//sy 2003.7.15.
void CKTBChartCtrl::SetAllGraphsHide(short nRowIndex, short nColumnIndex, BOOL bHide) 
{
	if(m_pMainBlock == NULL)
		return;

	m_pMainBlock->SetAllGraphsHiding(CBlockIndex(nRowIndex, nColumnIndex), (bHide == TRUE? true: false));
	InvalidateControl() ;
}

BSTR CKTBChartCtrl::GetPacketData2(LPCTSTR strPacketName2, short nDataIndex, BOOL bIsFullDataSize) 
{
	CString strPacketName = _STOM0(strPacketName2);

	CString strResult;
	if(m_pPacketList == NULL || nDataIndex < 0)
		return strResult.AllocSysString();
	
	CPacket* pPacket = m_pPacketList->GetBaseData((CString) strPacketName);
	if(pPacket == NULL)
		return strResult.AllocSysString();

	double dData = pPacket->GetData(nDataIndex);
	strResult = CDataConversion::DoubleToString(dData);

	//packet size�� ���� data�� ���̸� �����.
	if(bIsFullDataSize){
		int nSpaceCount = pPacket->GetLength() - strResult.GetLength();
		for(int nIndex = 0; nIndex < nSpaceCount; nIndex++){
			strResult = " " + strResult;
		}
	}

	return strResult.AllocSysString();
}

BSTR CKTBChartCtrl::NotUsedMethod6( short nPoint_x, short nPoint_y) 
{
	CString strResult;
	return strResult.AllocSysString();
}

short CKTBChartCtrl::GetPacketIndex(LPCTSTR szPacketName2, double dKeyValue) 
{
	if(m_pPacketList == NULL)
		return -1;

	CString szPacketName = _STOM0( szPacketName2);
	CPacket *pPacket = m_pPacketList->GetBaseData(szPacketName);
	if(pPacket == NULL || pPacket->GetCount() == 0)
		return -1;

	int nResult = 0;
	double value;
	POSITION pos = pPacket->GetnumericDataList()->GetHeadPosition();
	while(pos) 
	{
		value = pPacket->GetnumericDataList()->GetNext(pos);
		if(value == dKeyValue) 
			return nResult;
		nResult++;
	}

	return -1;
}

// ������ ������Ʈ Ÿ���� �˷���.
short CKTBChartCtrl::GetCurrentPriceChartType() 
{
	if(!m_pMainBlock)	
		return -1;

	CString strChartList;
	m_pMainBlock->GetAllBlocksData( strChartList);

	if(strChartList.Find( CString( "0;0;0;") + _MTEXT( C3_PRICE)) > 0)
		return 0;
	else if(strChartList.Find( CString( "0;1;1;") + _MTEXT( C3_PRICE)) > 0)
		return 1;
	else if(strChartList.Find( CString( "0;2;2;") + _MTEXT( C3_PRICE)) > 0)
		return 2;
	else if(strChartList.Find( CString( _MTEXT( C5_CANDLE_TYPE)) + ";" + _MTEXT( C2_PRICE_CHART) + ";" + _MTEXT( C0_CLOSE) + ";") > 0)
		return 2;
	else
		return -1;
}

// �ش� Point�� Block�� ����.
BOOL CKTBChartCtrl::RemoveBlock(short nPoint_x, short nPoint_y) 
{
	if(m_pMainBlock == NULL) return FALSE;

	CPoint pt(nPoint_x, nPoint_y);
	CBlockIndex bi = m_pMainBlock->FindBlockIndex( pt);
	RemoveBlockWithIndex( bi.GetRow(), bi.GetColumn());
	return TRUE;
}

BOOL CKTBChartCtrl::RemoveBlockWithIndex(short p_nRowIndex, short p_nColumnIndex) 
{
	// TODO: Add your dispatch handler code here
	if(m_pMainBlock == NULL) return FALSE;

	CBlockIndex bi( p_nRowIndex, p_nColumnIndex);
	if(!m_pMainBlock->DeleteBlock( bi)) return FALSE;

	if( m_pAddInManager) m_pAddInManager->OnBlockRemoved( bi.GetRow(), bi.GetColumn());
	Invalidate();
	return TRUE;
}

// �ش�index�� Block�� �ִ� Graph�� ����.
BOOL CKTBChartCtrl::RemoveGraph(short nRowIndex, short nColumnIndex, LPCTSTR strGraphTitle2)
{
	if(m_pMainBlock == NULL)
		return FALSE;

	CString strGraphTitle = _STOM1( strGraphTitle2);
	if(!m_pMainBlock->DeleteGraphWithSubGraphTitle( CBlockIndex(nRowIndex, nColumnIndex), strGraphTitle))
		return FALSE;

	Invalidate();
	return TRUE;
}

BOOL CKTBChartCtrl::SetGraphColor(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, short nGraphIndex, short nSubGraphIndex, LPCTSTR strNewColor) 
{
	if(m_pMainBlock == NULL)
		return FALSE;

	if(!m_pMainBlock->ChangeGraphColor1(CBlockIndex(nRowIndex, nColumnIndex), 
		nVertScaleGroupIndex, nGraphIndex, nSubGraphIndex, CDataConversion::Str2RGB(strNewColor)))
		return FALSE;

	InvalidateControl();
	return TRUE;
}

BOOL CKTBChartCtrl::SetPacketDataInIndex(LPCTSTR strPacketName2, short nIndex, LPCTSTR strData, BOOL bIsRealData) 
{
	if(m_pPacketList == NULL || nIndex < 0)
		return FALSE;

	CString strPacketName = _STOM0( strPacketName2);
	bool bResult = m_pPacketList->SetData((CString)strPacketName, nIndex, atof(strData), (bIsRealData==TRUE)?true:false);
	if(bResult){
		Invalidate();
	}

	return bResult;
}

BOOL CKTBChartCtrl::SetVertScaleLogType(short nRowIndex, short nColumnIndex, BOOL bIsLogType) 
{
	if(m_pMainBlock == NULL)
		return FALSE;

	if(nRowIndex < 0 || nColumnIndex < 0)
		return m_pMainBlock->SetAllVertScaleLogType((bIsLogType?true:false));
	
	return m_pMainBlock->SetVertScaleLogType(CBlockIndex(nRowIndex, nColumnIndex), (bIsLogType?true:false));
}

BOOL CKTBChartCtrl::OpenFileOfEnvAndIndi(LPCTSTR strFileName_Env, LPCTSTR strFileName_Indi, BOOL bIsCurrentSave) 
{
	//��Ʈ ȯ�� ���� �κ�
	int nResult = OpenFileAndSave(m_strUserEnvironmentFileName, strFileName_Env, (bIsCurrentSave?true:false), CSaveUserFile::ENVIRONMENT);
	if(nResult == 0) // file ���� ������ ó�� file�� �����ϴ� ���
		ResetSaveChartCfg(-1);
	else if(nResult == 1) // file �� �ִ� ���
		SetMainBlock(IsRunTimeMode());

	bool bResult = nResult >= 0 ? true: false;

	//��ǥ �Ӽ� ���� �κ�
	if(OpenFileAndSave(m_strUserIndicatorListFileName, strFileName_Indi, (bIsCurrentSave?true:false), CSaveUserFile::INDICATORLIST) >= 0)
		bResult = true;

	if( bResult) InvalidateControl();

	return bResult;
}

// ------------------------------------------------------------------------------------------

void CKTBChartCtrl::DoPropExchange(CPropExchange* pPX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COleControl::DoPropExchange( pPX);
}

// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CKTBChartCtrl::HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem)
{
	// ����(05/05/12) ����DLL������� ���� ��������.
	if(m_pToolBarManager)
		return m_pToolBarManager->GetToolOption(p_eToolItem);
	else
		return FALSE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/20
// Modifier		: 
// Comments		: Print Chart Image
//-----------------------------------------------------------------------------
void CKTBChartCtrl::PrintChatImage() 
{
	// TODO: Add your dispatch handler code here
	OnPrint();
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/20
// Modifier		: 
// Comments		: Export Packet Data to Excel
//-----------------------------------------------------------------------------
void CKTBChartCtrl::ExportToExcel() 
{
	// TODO: Add your dispatch handler code here
	OnToExcel();
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/21
// Modifier		: 
// Comments		: AddIn Custom Command Process Routine
//-----------------------------------------------------------------------------
BOOL CKTBChartCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class

	// (2007/1/6 - Seung-Won, Bae) Support Platform Menu
	if( ID_PLM_ADD_MENU <= nID && nID <= ID_PLM_ADD_MENU_END)
	{
		CWnd *pWnd = GetParent();
		if( !pWnd) return TRUE;
		if( CN_UPDATE_COMMAND_UI == nCode)
		{
			CCmdUI *pCmdUI = ( CCmdUI *)pExtra;
			if( pCmdUI)
			{
				long lResult = pWnd->SendMessage( UDM_CHECK_PLATFORM_MENU, nID);
				pCmdUI->Enable( !( lResult & MF_DISABLED));
				pCmdUI->SetCheck( lResult & MF_CHECKED);
			}
			return TRUE;
		}
		pWnd->SendMessage( WM_COMMAND, nID, ( long)GetSafeHwnd());
		return TRUE;
	}

	// (2006/1/21 - Seung-Won, Bae) Check AddIn Command
	if( m_pAddInManager) if( m_pAddInManager->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo)) return TRUE;

	// (2006/6/12 - Seung-Won, Bae) Market Indicator Process
	CString strIndicatorName;
	// if( ID_VOLUME_PRICE == nID)	strGraphName = _MTEXT( C2_VOLUME_AMOUNT);
	if( !strIndicatorName.IsEmpty())
	{
		if( CN_UPDATE_COMMAND_UI == nCode)
		{
			CCmdUI *pCmdUI = ( CCmdUI *)pExtra;
			if( !pCmdUI) return FALSE;
			pCmdUI->Enable( TRUE);
			BOOL bExist = HasGraph( strIndicatorName);
			if( !bExist) if( IsRunningOneChart()) bExist = HasGraphInSpecialBack( strIndicatorName);
			// (2007/1/22 - Seung-Won, Bae) Do not support 'Check State' in Multiple Same-Name Indicator.
			// pCmdUI->SetCheck( bExist);
			return TRUE;
		}
		if( CN_COMMAND == nCode)
		{
			AddChartBlock( strIndicatorName);
			return TRUE;
		}
	}

	return COleControl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
void CKTBChartCtrl::ReleaseOtherToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption)
{
	if( m_pToolBarManager) m_pToolBarManager->ReleaseOtherButtonUI( p_eToolOption);
}

void CKTBChartCtrl::OnNPacketCountDataSizeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

//sy 2006.03.02
/*-----------------------------------------------------------------------------------------
 - Function    :  SetMapDefaultInCurrent
 - Created at  :  2006-02-20
 - Author      :  �輱��
 - Description :  ���� ȭ���� ��ǥ�� ���õ� ������ Map Default ������ set
					bIsNewMapDefault = TRUE : ������ Map Default �� ����
 -----------------------------------------------------------------------------------------*/
//sy 2006.02.20. -> Map Default ���� �����Ҽ� �ְ� �Ѵ�.
BOOL CKTBChartCtrl::SetMapDefaultInCurrent(BOOL bIsNewMapDefault) 
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	// ������ map default �� ��� ����.
	if( bIsNewMapDefault) m_pIndicatorList->RemoveAllIndicatorInfo( EILT_MAP_DEFAULT);

	// ȭ�鰪�� map default �� ����.
	return m_pMainBlock->ChangeAllMapDefaultIndicatorInfo();
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetSubIndicatorCondition
 - Created at  :  2006-01-09
 - Author      :  ������
 - Description :  SubGraph�� ���ǰ� ����.
					nConditionType : �ٲ� ���ǰ� type (0 : �׸��� ���ǰ�  1 : ������ǰ�  ��Ÿ : ����)
					nSubGraphIndex : SubGraph�� Index ��		
					nConditionIndex : ���ǰ��� Index ��
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::SetSubIndicatorCondition(LPCTSTR strGraphName2, short nConditionType, short nSubGraphIndex, short nConditionIndex, double dConditionValue, BOOL bReCalculate) 
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL) return FALSE;

	// ���ǰ� ����
	CString strGraphName = _STOM2( strGraphName2);
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
	if( !pIndicatorInfo) return FALSE;
	
	int nResult = -1;
			if(nConditionType == 0)	nResult = pIndicatorInfo->SetDrawConditionData( ( int)nConditionIndex, dConditionValue);
	else	if(nConditionType == 1)	nResult = pIndicatorInfo->SetCalConditionData( ( int)nSubGraphIndex, (int)nConditionIndex, dConditionValue);
	else							return FALSE;

	// ���ǰ� ���濡 ���� title ����
	CString strIndicatorName = pIndicatorInfo->GetIndicatorName();
	if( g_iMetaTable.IsTitleIncludeCondition( strIndicatorName)) m_pMainBlock->ChangeSubGraphTitleFromIndicator( strGraphName, ( int)nSubGraphIndex);

	// ��ǥ ���
	if( bReCalculate && nResult == 1) m_pMainBlock->CalculateGraph( strGraphName, CCalculateBaseData::TRDATA_CAL);
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphHide4
 - Created at  :  2006-01-09
 - Author      :  ������
 - Description :  �ش� ��ǥ�� SubGraphIndex�� Hide �Ӽ��� Set
					bHide = TRUE : SubGraph Hide
					bHide = FALSE : SubGraph Show
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::SetGraphHide4(LPCTSTR strGraphName2, short nSubGraphIndex, BOOL bHide) 
{
	if(m_pMainBlock == NULL)
		return FALSE;

	CString strGraphName = _STOM2( strGraphName2);
	if(!m_pMainBlock->SetSubGraphHiding(strGraphName, (int)nSubGraphIndex, (bHide ? true: false)))
		return FALSE;

	//<<20100409_JS.Kim �ַθ�.FX  �̵������ �ܺο��� ��ũ��Ʈ�� Show/Hide ��Ű�µ�, ȯ�漳��â ��� �� ChangeAllGraphInfoFromGraphData()�� 
	//  Call �Ͽ� Hide ���� �̵���հ��� ��� '0'�� �Ǵ� ���� �߻���.
	//	�׷��� ���⼭ �̵������ Show�ϴ� ������ �ٽ� ����ϵ��� ó��.
	if( bHide == FALSE )
	{
		m_pMainBlock->ChangeIndicatorInfoFromGraphData(strGraphName);
	}
	//>>

	InvalidateControl();
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/8
// Modifier		: 
// Comments		: Support Horizontal Line Show/Hide In NIDLG for Special Chart
//-----------------------------------------------------------------------------
void CKTBChartCtrl::SetEnableHorizontalLineInNIDLG(BOOL bNewValue) 
{
	// TODO: Add your property handler here

	SetModifiedFlag();

	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_NIDLG_ALL_ENABLE_HORIZONTAL_LINE, bNewValue);
}

// (2006/11/22 - Seung-Won, Bae) Do not support the Method 'ChangeFunctionName' anymore.
BOOL CKTBChartCtrl::NotUsedMethod2(LPCTSTR p_szTargetFName, LPCTSTR p_szNewFName, LPCTSTR p_szNewSGName) 
{
	_ML_TEST_NOT_USED();
	return FALSE;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetHeadAndTailData
 - Created at  :  2006-03-15
 - Author      :  ������ (JSR)
 - Description :  strPacketName�� �ش��ϴ� Head/Tail data�� �����Ѵ�.
					pHeadData = �ش� Packet Head Data
					pTailData = �ش� Packet Tail Data
 -----------------------------------------------------------------------------------------*/
BOOL CKTBChartCtrl::GetHeadAndTailData(LPCTSTR strPacketName2, double FAR* pHeadData, double FAR* pTailData) 
{
	if(pHeadData == NULL || pTailData == NULL || m_pPacketList == NULL)
		return FALSE;

	CString strPacketName = _STOM0( strPacketName2);
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(pPacket == NULL)
		return FALSE;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return FALSE;

	*pHeadData = pDataList->GetHead();
	*pTailData = pDataList->GetTail();
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/23
// Modifier		: 
// Comments		: GetAddInCustomInterface() for DLL
//-----------------------------------------------------------------------------
LPUNKNOWN CKTBChartCtrl::GetAddInCustomInterface(LPCTSTR p_szAddInName) 
{
	if( m_pAddInManager) return m_pAddInManager->GetAddInCustomInterface( p_szAddInName);

	return NULL;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/26
// Modifier		: 
// Comments		: Load AddIn with Over Exclusive.
//-----------------------------------------------------------------------------
BOOL CKTBChartCtrl::LoadAddInWithOverExclusive(LPCTSTR p_szAddInName) 
{
	if( !m_pAddInManager) return FALSE;
	if( !m_pAddInManager->LoadAddIn( p_szAddInName, TRUE)) return FALSE;

	return TRUE;
}

// (2006/4/14 - Seung-Won, Bae) Process Special Indicator Ending Menu.
void CKTBChartCtrl::OnReturnToGeneralChart() 
{
	// TODO: Add your command handler code here
	RollbackLastNoOneChart();	
}


// (2006/6/7 - Seung-Won, Bae) Get IChartCtrl Interface for access protected member of Ctrl Class
IChartCtrl *CKTBChartCtrl::GetIChartCtrl( void)
{
	return m_pIChartCtrl;
}

//	from .xfm
//	common control
BOOL CKTBChartCtrl::SetAllProperties2(long dwData) 
{
	CStringList* pPropStrList = (CStringList*) dwData;
	if(pPropStrList == NULL) return false;

	// (2006/6/19 - Seung-Won, Bae) Convert to IPropertyMap
	int				nKeyIndex;
	char			szPropertyKey[ 256];
	CString			strPropertyInfo;
	POSITION		posProperty = pPropStrList->GetHeadPosition();
	CPropertyMap	pmProperty;
	IPropertyMap *	pIPropertyMap = &pmProperty;
	while( posProperty)
	{
		strPropertyInfo = pPropStrList->GetNext( posProperty);
		nKeyIndex = strPropertyInfo.Find( '=');
		if( nKeyIndex < 1) continue;
		strncpy( szPropertyKey, strPropertyInfo, nKeyIndex);
		szPropertyKey[ nKeyIndex] = '\0';
		pIPropertyMap->SetAt( szPropertyKey, ( ( const char *)strPropertyInfo) + nKeyIndex + 1);
	}

	return SetAllProperties3( ( long)pIPropertyMap);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/19
// Modifier		: 
// Comments		: SetAllProperties3 for IPropertyMap (Only KTB)
//-----------------------------------------------------------------------------
long CKTBChartCtrl::SetAllProperties3(long lIPropertyMap) 
{
	// TODO: Add your dispatch handler code here
	if( m_bEnableLogChartLoading) GetLocalTime( &m_stLogStart);

	IPropertyMap *pIPropertyMap = ( IPropertyMap *)lIPropertyMap;

	SetAllProperties_NEOCONVERT( pIPropertyMap);
	SetAllProperties_Chart();

	// (2005/11/7 - Seung-Won, Bae) Log for Normalized Minute Time Rule
	if( m_bEnableLogAddIn)
	{
		LoadAddIn( g_aszAddInLoadString[ AID_LOG]);
		InvokeAddInStr( g_aszAddInLoadString[ AID_LOG], "LOGFILE", SZ_STRING_KEY100);
		InvokeAddInStr( g_aszAddInLoadString[ AID_LOG], "LOGPROPERTY", "");
	}

	if( m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\n",
			m_stLogStart.wHour, m_stLogStart.wMinute, m_stLogStart.wSecond, m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds);
		CDumpLogger::LogStringToFile( "CKTBChartCtrl::SetAllProperty3()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}

	return 1;
}

BOOL CKTBChartCtrl::SetAllProperties_NEOCONVERT( IPropertyMap *p_pIPropertyMap)
{
	ASSERT( p_pIPropertyMap);
	if( !p_pIPropertyMap) return false;

	// (2004.04.06, ��¿�, DEBUG) AnyBuilder�� ������ ������ �м��ϱ� ���Ͽ� Logging ����� �߰��Ѵ�.
	if( m_bEnableLogSetGetAll)
	{
		CString			strProperties, strProperty;
		POSITION		posProp = p_pIPropertyMap->GetStartPosition();
		const char *	szPropertyKey;
		const char *	szPropertyValue;
		while( posProp)
		{
			 p_pIPropertyMap->GetNextAssoc( posProp, szPropertyKey, szPropertyValue);
			 strProperty.Format( "%s=%s", szPropertyKey, szPropertyValue);
			 strProperties += strProperty + "\r\n";
		}
		static int nSetAllProperties3Count = 1;
		CString strLogKey;
		strLogKey.Format( "%s-%dth", "CKTBChartCtrl::SetAllProperties3()", nSetAllProperties3Count++);
		CDumpLogger::LogStringToFile( strLogKey, strProperties, strProperties.GetLength(), "ChartSetAll");
	}

	CHART_CONVERT_BEGIN()
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRBACKGROUND					], m_clrBackground,						_DPV_CLRBACKGROUND					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRBACKBORDER					], m_clrBackBorder,						_DPV_CLRBACKBORDER					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRBLOCK						], m_clrBlock,							_DPV_CLRBLOCK						);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRBLOCKBORDER					], m_clrBlockBorder,					_DPV_CLRBLOCKBORDER					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRSCALELINE					], m_clrScaleLine,						_DPV_CLRSCALELINE					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRSCALELINE2					], m_clrScaleLine2,						_DPV_CLRSCALELINE2					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRSCALETEXT					], m_clrScaleText,						_DPV_CLRSCALETEXT					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE1					], m_clrGraphBase1,						_DPV_CLRGRAPHBASE1					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE2					], m_clrGraphBase2,						_DPV_CLRGRAPHBASE2					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE3					], m_clrGraphBase3,						_DPV_CLRGRAPHBASE3					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE4					], m_clrGraphBase4,						_DPV_CLRGRAPHBASE4					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE5					], m_clrGraphBase5,						_DPV_CLRGRAPHBASE5					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE6					], m_clrGraphBase6,						_DPV_CLRGRAPHBASE6					);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRANALYTICSTOOL				], m_clrAnalysisTool,					_DPV_CLRANALYTICSTOOL				);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRINQUIRYWINDOW				], m_clrInquiryWindow,					_DPV_CLRINQUIRYWINDOW				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	TYPEBLOCKTYPE					], m_nBlockType,						_DPV_TYPEBLOCKTYPE					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	TYPEBLOCKCOMPART				], m_nBlockCompart,						_DPV_TYPEBLOCKCOMPART				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NBLOCKROWCOUNT					], m_nBlockRowCount,					_DPV_NBLOCKROWCOUNT					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NBLOCKCOLUMNCOUNT				], m_nBlockColumnCount,					_DPV_NBLOCKCOLUMNCOUNT				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NBLOCKGAP						], m_nBlockGap,							_DPV_NBLOCKGAP						);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	BLOCKBORDERTHICKNESS			], m_nBlockBorderThickness,				_DPV_BLOCKBORDERTHICKNESS			);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NROWDOUBLESIZEBLOCK				], m_nRowDoubleSizeBlock,				_DPV_NROWDOUBLESIZEBLOCK			);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NCOLUMNDOUBLESIZEBLOCK			], m_nColumnDoubleSizeBlock,			_DPV_NCOLUMNDOUBLESIZEBLOCK			);
		CHART_CONVERT_COLOR(	p_pIPropertyMap, g_anPropertyShortKey[	CLRRATTICE						], m_clrLattice,						_DPV_CLRRATTICE						);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRCHARTLIST					], m_strChartList,						_DPV_STRCHARTLIST					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	TYPEBORDER						], m_nBorderType,						_DPV_TYPEBORDER						);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BWHOLEVIEW						], m_bWholeView,						_DPV_BWHOLEVIEW						);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NONEPAGEDATACOUNT				], m_nOnePageDataCount,					_DPV_NONEPAGEDATACOUNT				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NUSERBLOCKADDCOUNT				], m_nUserBlockAddCount,				_DPV_NUSERBLOCKADDCOUNT				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BGRAPHDRAGDROP					], m_bGraphDragDrop,					_DPV_BGRAPHDRAGDROP					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NLEFTMARGIN						], m_nLeftMargin,						_DPV_NLEFTMARGIN					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NRIGHTMARGIN					], m_nRightMargin,						_DPV_NRIGHTMARGIN					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NTOPMARGIN						], m_nTopMargin,						_DPV_NTOPMARGIN						);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NBOTTOMMARGIN					], m_nBottomMargin,						_DPV_NBOTTOMMARGIN					);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BADDGRAPHMENUSHOW				], m_bAddGraphMenuShow,					_DPV_BADDGRAPHMENUSHOW				);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRENVIRONMENTFILENAME			], m_strUserEnvironmentFileName,		_DPV_STRENVIRONMENTFILENAME			);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRUSERINDICATORLISTFILENAME	], m_strUserIndicatorListFileName,		_DPV_STRUSERINDICATORLISTFILENAME	);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BHIDE							], m_bHide,								_DPV_BHIDE							);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BISINPUT						], m_bIsInput,							_DPV_BISINPUT						);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRTOOLBAROPTION				], m_strToolbarOption,					_DPV_STRTOOLBAROPTION				);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRPACKETOPTION					], m_strPacketOption,					_DPV_STRPACKETOPTION				);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRLOCALSHAREDLIST				], m_strLocalSharedList,				_DPV_STRLOCALSHAREDLIST				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSEREALDATA					], m_bUseRealData,						_DPV_BUSEREALDATA					);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRREALLIST						], m_strRealList,						_DPV_STRREALLIST					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NMAGINBORDERTYPE				], m_nMarginBorderType,					_DPV_NMAGINBORDERTYPE				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSEINQUIRYDLGWHENUSECROSSLINE	], m_bUseInquiryDlgWhenUseCrossLine,	_DPV_BUSEINQUIRYDLGWHENUSECROSSLINE	);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSESHORTKEY					], m_bUseShortKey,						_DPV_BUSESHORTKEY					);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSECONDITIONTOTITLE			], m_bUseConditiontoTitle,				_DPV_BUSECONDITIONTOTITLE			);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSEDATATOTITLE					], m_bUseDatatoTitle,					_DPV_BUSEDATATOTITLE				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nHorzGap						], m_nHorzGap,							_DPV_nHorzGap						);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nVertGap						], m_nVertGap,							_DPV_nVertGap						);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nRButtonMenuType				], m_nRButtonMenuType,					_DPV_nRButtonMenuType				);	// (2005/7/26 - Seung-Won, Bae) Default 1, see GetAllProperties2() & DoPropExchange()
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bShowWarningMessageBox			], m_bShowWarningMessageBox,			_DPV_bShowWarningMessageBox			);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bControlBlockSize				], m_bControlBlockSize,					_DPV_bControlBlockSize				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bBlockMoveDelete				], m_bBlockMoveDelete,					_DPV_bBlockMoveDelete				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bSelectTitle					], m_bSelectTitle,						_DPV_bSelectTitle					);	// vntsorl_20110608:[A00000421] ���콺�� �̿��� ���� ����ó��
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bSelectGraph					], m_bSelectGraph,						_DPV_bSelectGraph					);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bUseScaleRButtonMenu			], m_bUseScaleRButtonMenu,				_DPV_bUseScaleRButtonMenu			);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nGraphRButtonType				], m_nGraphRButtonType,					_DPV_nGraphRButtonType				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nHoldingPacketSize				], m_nHoldingPacketSize,				_DPV_nHoldingPacketSize				);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nMinDataCountOnOnePage			], m_nMinDataCountOnOnePage,			_DPV_nMinDataCountOnOnePage			);	// (2003.11.12, ��¿�) �� ȭ�鿡 ���� �ּ� Data ���� Property�� �����Ͽ� Map�� ����ǵ��� �Ѵ�.
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRBASEPACKETNAME				], m_szBasePacketName,					_DPV_STRBASEPACKETNAME				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bFIREREALEVENT					], m_bFireRealEvent,					_DPV_bFIREREALEVENT					);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NUMERICALINQUIRYOPTION			], m_nNumericalInquiryOption,			_DPV_NUMERICALINQUIRYOPTION			);	// ADD: ����(04/03/17) ��ġ��ȸ����
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRGLOBALSHAREDLIST				], m_strGlobalSharedList,				_DPV_STRGLOBALSHAREDLIST			);	// (2004.06.08, ��¿�) Global ������ �߰��Ѵ�.
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	USESKIN							], m_bDoesUseSkinColor,					_DPV_USESKIN						);	//sy 2005.04.26.
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	MAPNAME							], m_strMapName,						_DPV_MAPNAME						);	//sy 2005.09.06.
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	BONGCOUNTMARGIN					], m_nBongCountMargin,					_DPV_BONGCOUNTMARGIN				);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	DISPLAYTRDATA					], m_bOnePageDataCountInTRCount,		_DPV_DISPLAYTRDATA					);	//sy 2005.11.25.
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CUSTOMFONT						], m_strCSFont,							_DPV_CUSTOMFONT						);	
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	BLOCKREGION						], m_strBlockRegion,					_DPV_BLOCKREGION					);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	ADDINLIST						], m_strAddInLoadList,					_DPV_ADDINLIST						);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	MAPINDICATORINFO				], m_strMapIndicatorInfo,				_DPV_MAPINDICATORINFO				);

		// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
		//	for Old Version Map and User File.
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	TYPESCALELINE_NOT_USED			], m_nScaleLineType,					_DPV_TYPESCALELINE_NOT_USED			);

		// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	LOCALLANGUAGETABLE				], m_strLocalLanguageTable,				_DPV_LOCALLANGUAGETABLE				);
	CHART_CONVERT_END()

	// (2009/5/12 - Seung-Won, Bae) Set TEST Value
	CString strEnvValue;
	if( GetTestEnvValue( "nHoldingPacketSize", strEnvValue))
		m_nHoldingPacketSize = atoi( strEnvValue);
	if( GetTestEnvValue( "bOnePageDataCountInTRCount", strEnvValue))
		m_bOnePageDataCountInTRCount = atoi( strEnvValue);
	if( GetTestEnvValue( "nMinDataCountOnOnePage", strEnvValue))
		m_nMinDataCountOnOnePage = atoi( strEnvValue);

	//20100430 �̹��� ������ �ڸ��� �ʰ� ��� ������Ű���� ó�� >>
	//����ũ�Ⱑ �Ѿ �ڸ��� ó���� pc ������ ������ ������
	// m_nHoldingPacketSize = 0;
	//20100430 �̹��� <<

	return TRUE;
}

void CKTBChartCtrl::SetAllProperties_Chart()
{
	// (2009/9/1 - Seung-Won, Bae) Define Chart Mode. (Load setting from ChartMetaTable.dat)
	if( INT_MIN == m_nChartModeSetting)
	{
		char szChartMode[ 256];
		DWORD dwError = GetPrivateProfileString( "Chart Version", "ChartMode", "", szChartMode, 255, GetBaseDeskPath() + DATA_FILE_FOLDER + "ChartMetaTable.dat");
		CString strChartMode( szChartMode);
		if(			strChartMode == "DEV")		m_nChartModeSetting = CChartInfo::DEVELOPER;
		else if(	strChartMode == "HTS")		m_nChartModeSetting = CChartInfo::HTS;
		else if(	strChartMode == "FX")		m_nChartModeSetting = CChartInfo::FX;
		else if(	strChartMode == "WORLDON")	m_nChartModeSetting = CChartInfo::WORLD_ON;
		else									m_nChartModeSetting = ( CChartInfo::CHART_MODE)-1;
	}
	if( 0 <= m_nChartModeSetting) m_nChartMode = m_nChartModeSetting;

	// (2009/11/1 - Seung-Won, Bae) for Nomalized Minute Time Rule.
	if( CChartInfo::FX == m_nChartMode) m_bNormalizedMinuteTimeRule = TRUE;

// (2009/11/23 - Seung-Won, Bae) for DEV
//#ifdef _DEBUG
//	char szChartMode[ 256];
//	DWORD dwError = GetPrivateProfileString( "Chart Version", "MChartMode", "", szChartMode, 255, GetBaseDeskPath() + DATA_FILE_FOLDER + "ChartMetaTable.dat");
//	CString strChartMode( szChartMode);
//	if( strChartMode == "HTS") m_bNormalizedMinuteTimeRule = TRUE;
//#endif

	//sy 2003.10.20.
	m_SaveChartcfg.RemoveAll();

	// sy 2003.05.02 CHART_CONVERT ���� ���� 0���� �ִ� ��찡 ����.
	if(m_nBlockRowCount < 1)
		m_nBlockRowCount = 1;
	if(m_nBlockColumnCount < 1)
		m_nBlockColumnCount = 1;
	
	// (2003.11.12, ��¿�) �� ȭ�鿡 ���� �ּ� Data ���� Property�� �����Ͽ� Map�� ����ǵ��� �Ѵ�.
	//�� ȭ�鿡 ���̴� ������ �� ���� min���� �� ũ�� �ȵȴ�.
	if(m_nMinDataCountOnOnePage > m_nOnePageDataCount)
		m_nMinDataCountOnOnePage = (short)m_nOnePageDataCount;
	// (2005.01.20, ��¿�) Ư�� 1���� ���� ��쿡�� 1�� �ּҼ����� �ǵ��� �Ѵ�.
	if( m_nMinDataCountOnOnePage < 1) m_nMinDataCountOnOnePage = 1;
	
	// (2006/9/4 - Seung-Won, Bae) In ktb, User folder of each user is not supported. There is only one User Folder. ( "\User")
//@Solomon -->
	// (2009/12/15 - Junok, Lee) In Solomon, Use User Folder.
	if(IsRunTimeMode())
	{
		if(::IsWindow(m_hContainerDllWnd))
			m_strUserID = (char*)(long)::SendMessage(m_hContainerDllWnd, RMSG_GET_USERID, 0, 0);
		
		if(!m_strUserID.IsEmpty())
		{
			m_strUserID += "\\";
			
			// (2006/2/27 - Seung-Won, Bae) Manage User File Path for AddIn
			m_strUserFilePath = GetUserDefaultFilePath();
		}
	}
	else
	{
		m_strUserFilePath = GetUserDefaultFilePath();
	}
//@Solomon <--

	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	m_strPagePathWithPrefix = GetUserFilePath();

	// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
	if( !m_bTestEnvInitialized)
	{
		char szBuffer[ m_nTestEnvBufferSize];
		DWORD dwRead = GetPrivateProfileSection( "Chart TEST", szBuffer, m_nTestEnvBufferSize, m_strUserFilePath + "ChartTestEnv.ini");
		char *pszKey = szBuffer;
		if( 0 < dwRead && dwRead < m_nTestEnvBufferSize - 2)
		{
			// 2. �о�� DDE Site Info�� Parsing�Ѵ�.
			char *pszRTrim = NULL, *pszNextKey = NULL, *pszValue = NULL;
			while( *pszKey)
			{
				pszNextKey = pszKey + strlen(pszKey) + 1;

				pszRTrim = pszNextKey - 2;
				while(*pszRTrim == ' ' || *pszRTrim == '\t') pszRTrim--;
				*(pszRTrim + 1) = '\0';
				
				pszValue = strstr( pszKey, "=");
				if( pszValue)
				{
					*pszValue = '\0';
					pszValue++;
					m_mapTestEnvSetting.SetAt( pszKey, pszValue);
				}

				pszKey = pszNextKey;
			}
		}
		
		m_bTestEnvInitialized = TRUE;
	}
	//}

	// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
	g_iMetaTable.m_strDataFolderPath = GetBaseDeskPath() + DATA_FILE_FOLDER;
	g_iMetaTable.LoadMetaTable();

	// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
	CString strEnvValue;
	GetTestEnvValue( "Language Type", strEnvValue);
	if( !strEnvValue.IsEmpty()) switch( strEnvValue.GetAt( 0))
	{
		case 'E':	m_nLanguageID = IMetaTable::ENGLISH;
					break;
		case 'K':	m_nLanguageID = IMetaTable::KOREAN;
					break;
	}

	g_iMetaTable.CreateLocalLanguageTable(	GetSafeHwnd(),
											( IMetaTable::E_LANGUAGEID)m_nLanguageID,
											m_strLocalLanguageTable,
											!IsRunTimeMode());
	if( m_szBasePacketName == _DPV_STRBASEPACKETNAME) m_szBasePacketName = _MTEXT( C0_DATE_TIME);

	// (2004.10.08, ��¿�) AddIn Manager�� Ocx Pointer�� ����Ͽ� Ocx Interface Wrapper�� ������ �� �ֵ��� �����Ѵ�.
	// (2004.10.11, ��¿�) this Pointer�� Base Member Initializer List�� �̿������μ� ��Ÿ���� Warning�� ��Ÿ���� �ʵ��� ������ ����Ѵ�.
	if( !m_pIChartCtrl) m_pIChartCtrl = new CIWChartCtrl( this);
	if( !m_pAddInManager) m_pAddInManager = new CAddInManager( m_pIChartCtrl, g_strOcxPath);	// (2006/1/18 - Seung-Won, Bae) for Load AddIn DLL in OCX Path

	// (2007/6/25 - Seung-Won, Bae) Support Dynamic Loading of ChartObject.dll
	m_hMainBlock = LoadLibraryEx( g_strOcxPath + "ChartObject.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	// (yiilyoul@magicn.com - 20070202) X-ScaleManager
	if( m_hMainBlock && !m_pxScaleManager)
	{
		FN_CreateXScaleManager fnCreateXScaleManager = ( FN_CreateXScaleManager)GetProcAddress( m_hMainBlock, "CreateXScaleManager");
		if( fnCreateXScaleManager) m_pxScaleManager = fnCreateXScaleManager();

		// OCX ���� : xScaleManager - ojtaso (20070504)
		m_pxScaleManager->SetIChartCtrl( m_pIChartCtrl);
	}

	// (2009/2/12 - Seung-Won, Bae) Support default data space.
	// (2009/5/4 - Seung-Won, Bae) Set defalt data space with m_nMinDataCountOnOnePage even if m_bOnePageDataCountInTRCount.
	m_pxScaleManager->SetDefaultDataSpace( m_bOnePageDataCountInTRCount ? m_nMinDataCountOnOnePage : m_nOnePageDataCount, m_nMinDataCountOnOnePage);

	// (2003.12.23, ��¿�) Resource Manager ������ �����Ѵ�.
	FP_vSetInitFont();

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	for Old Version Map and User File.
	m_nVertScaleType = m_nScaleLineType;
	m_nHorzScaleType = m_nScaleLineType;

	//nami 9.10
	if(IsRunTimeMode())
	{
		// (2008/10/13 - Seung-Won, Bae) Do not use MainBlock. It is not Initialized. Use Cfg member variables.
		SaveChartCfg( TRUE);
		m_bOrgWholeView = m_bWholeView;
		m_nOrgOnePageDataCount = m_nOnePageDataCount;
		m_nOrgBlockRowCount = m_nBlockRowCount;
		m_nOrgBlockColumnCount = m_nBlockColumnCount;
		m_strOrgBlockRegion = m_strBlockRegion;
		m_nOrgRowDoubleSizeBlock = m_nRowDoubleSizeBlock;
		m_clrOrgBackground = m_clrBackground;
		m_clrOrgBackBorder = m_clrBackBorder;
		m_clrOrgBlock = m_clrBlock;
		m_clrOrgBlockBorder = m_clrBlockBorder;
		m_clrOrgScaleLine = m_clrScaleLine;
		m_clrOrgScaleText = m_clrScaleText;
		m_clrOrgAnalysisTool = m_clrAnalysisTool;
		m_clrOrgScaleLine2 = m_clrScaleLine2;
		m_clrOrgLattice = m_clrLattice;
	}

	// ������ �ִٸ� ���� �б�
	if(IsRunTimeMode() && !m_strUserEnvironmentFileName.IsEmpty())
	{
		ReadUserFile(CSaveUserFile::ENVIRONMENT);
		// (2008/10/13 - Seung-Won, Bae) Do not use MainBlock. It is not Initialized. Use Cfg member variables.
		SaveChartCfg( TRUE);
	}

	// tool option in ver 1.0.0.2
	// 10.23.2001
	if(m_strToolbarOption.Right(2) != "\r\n")
		m_strToolbarOption += "\r\n";	

	// (2006/7/8 - Seung-Won, Bae) Empty Packet Option needs not "\r\n"
	if( !m_strPacketOption.IsEmpty() && m_strPacketOption.Right(2) != "\r\n") m_strPacketOption += "\r\n";

	// ���������� ���� PacketListManager�� ���� : �������� - ojtaso (20061224)
	if(!m_pPacketListManager)
	{
		if( m_hMainBlock)
		{
			//0005977: [7111] ������Ʈ - ƽ�ֱ� ��Ʈ ��ȸ ��, HTS ����� Ÿ�� ���� 0.2������ �������� ��ó�� ���̴� ����
			//{
			//	CString szDebug;
			//	szDebug.Format("\n[DRCS]Ln(%d) CKTBChartCtrl::SetAllProperties_Chart m_nHoldingPacketSize(%d -> 0)\n", __LINE__, m_nHoldingPacketSize);
			//	OutputDebugString(szDebug);
			//}
			m_nHoldingPacketSize = 0;
			FN_CreatePacketListManager fnCreatePacketListManager = ( FN_CreatePacketListManager)GetProcAddress( m_hMainBlock, "CreatePacketListManager");
			if( fnCreatePacketListManager) m_pPacketListManager = fnCreatePacketListManager( m_strPacketOption, ( CPacketType::REALTYPE)m_nRealTimeTruncationType, PACKET_COUNT_LENGTH, m_bUseRealData, m_nHoldingPacketSize, m_szBasePacketName, 2.0);
		}
		if( !m_pPacketListManager) return;

		m_pPacketListManager->SetIChartCtrl( m_pIChartCtrl);
		m_strRQ = m_pPacketListManager->CreateDefaultChartItem( GetSafeHwnd());
		m_pPacketList = m_pPacketListManager->GetPacketList(m_strRQ);		
		m_pxScaleManager->SetPacketListManager(m_pPacketListManager);
		m_strPacketOption = m_pPacketList->GetPacketOption();
	}

	//sy 2005.06.16.
	if(m_pIndicatorList == NULL)
	{
		if( m_bRunTimeMode)
		{
			CString strDefault = GetDefaultFilePathAndName(FILE_INDICATOR_CONDITIONS);
			CString strUserDefault;
			if( m_hContainerDllWnd) strUserDefault = GetUserDefaultFilePathAndName(FILE_INDICATOR_CONDITIONS);
			CString strUser = GetUserFilePathAndName(m_strUserIndicatorListFileName);
			m_pIndicatorList = new CIndicatorList( GetSafeHwnd(), strDefault, strUserDefault, strUser, ( CChartInfo::CHART_MODE)m_nChartMode);
		}
		else
		{
			CString szPath = GetBaseDeskPath();
			CString szcfgpath = szPath + DATA_FILE_FOLDER ;
			CString szusrpath = GetUserFilePath(szPath);
			m_pIndicatorList = new CIndicatorList( GetSafeHwnd(), szcfgpath+FILE_INDICATOR_CONDITIONS, "", szusrpath, ( CChartInfo::CHART_MODE)m_nChartMode) ;
		}

		if(m_pxScaleManager != NULL) m_pxScaleManager->SetIndicatorList(m_pIndicatorList);
	}
	//sy end

	if( !m_pMainBlock)
	{
		if( m_hMainBlock)
		{
			FN_CreateMainBlock fnCreateMainBlock = ( FN_CreateMainBlock)GetProcAddress( m_hMainBlock, "CreateMainBlock");
			if( fnCreateMainBlock) m_pMainBlock = fnCreateMainBlock();
		}
	}
	if( !m_pMainBlock) return;

	// 03.06.2002
	m_pMainBlock->SetIChartCtrl( m_pIChartCtrl);
	//7.15. nami
	m_pMainBlock->SetGraphTitleDraw(CDataConversion::atob(m_bUseConditiontoTitle),
									CDataConversion::atob(m_bUseDatatoTitle));

	// ����(05/05/11) ���� ����
	CreateToolBar();
	
	if( m_pAddInManager)
	{
		// (2006/8/23 - Seung-Won, Bae) Version Update Test
		//	060823:DIRECT_REAL,ENV_SETUP,STYLE_DATA,DATA_VIEWER,ADD_INDICATOR_CMD,SAVE_IMAGE,RELPSA,KBD_SCROLL,DRAG_ZOOM,CROSS_DRAG,NIDLG_DRAG,TOOL_TIP,
		//			TIME_MARKER,ASKED_PRICE,OBJECT_TOOL,ST_INDICATOR,SYMBOL_TOOL,ANALYSIS_TOOL,NIDLG_TOOL,CROSS_TOOL:NIDLG_OVER,CROSS_OVER
		//	AddInNameListVersionUpFor( "060823:", "OBJECT_TOOL,ST_INDICATOR", NULL);
		// (2007/1/12 - Seung-Won, Bae) Add PERIOD_SUMMARY with Default.
		if( strncmp( m_strAddInLoadList, "060823", 7))
			AddInNameListVersionUpFor( "060823:", NULL, "PERIOD_SUMMARY");

		// (2006/7/24 - Seung-Won, Bae) Unload All AddIn
		// (2006/7/31 - Seung-Won, Bae) Manage AddIn Reload Flag of SetAllProperties3 for AddIn
		if( m_bSetAllProperties3WithReloadAddIn) m_pAddInManager->UnloadAllAddin();
		else m_bSetAllProperties3WithReloadAddIn = TRUE;		// for Next Time

		// (2006/1/16 - Seung-Won, Bae) Load OBJECT_TOOL in Design & Run Time
		//		and Before SetAllProperties2() for BlockInfo Setting, after All Manager Initializing Complete
		if( !IsRunTimeMode())
		{
			m_pAddInManager->LoadAddIn(	g_aszAddInLoadString[ AID_OBJECT_TOOL]);
			m_pAddInManager->LoadAddIn(	g_aszAddInLoadString[ AID_DESIGN_PROPERTY]);
			m_pAddInManager->LoadAddIn(	g_aszAddInLoadString[ AID_ENV_SETUP]);			// (2006/10/24 - Seung-Won, Bae) Support Indicator Configuration in Design Time.
		}
		else
		{
			// (2006/8/23 - Seung-Won, Bae) Load AddIn with Property Value
			int nNameListLen = m_strAddInLoadList.GetLength();
			if( nNameListLen < 8)	// Minimum is "DDDDDD::" ~ No AddIn.
				AfxMessageBox( "Chart AddIn Info String Error!\nCheck map setting. please!");
			else
			{
				CString strAddInNameList;
				char *szAddInNameList = strAddInNameList.GetBufferSetLength( nNameListLen);
				strcpy( szAddInNameList, m_strAddInLoadList);
				// (2006/8/24 - Seung-Won, Bae) Check Version Up!
				if( strncmp( szAddInNameList, _ADDIN_LOAD_ORDER, 7))
				{
					CString strMsg;
					strMsg.Format( "The AddIn Loading Info Version is too old [%s] in the map.\nCheck version up routines!", m_strAddInLoadList.Left( 6));
					AfxMessageBox( strMsg);
				}
				else
				{
					// (2006/11/25 - Seung-Won, Bae) Load Tool Order Manager with Force.
					m_pAddInManager->LoadAddIn(	g_aszAddInLoadString[ AID_TOOL_ORDER_MANAGER]);

					// 1. Skip Version
					char *szItem = szAddInNameList + 7;
					int nItemLen = 0;				// Not Used
					int nItemOver = 0;				// Over Code : 0(Under), 1(Last Under), 2(Over)
					char *szItemNext = NULL;
					AddInNameListParse( szItem, szItemNext, &nItemLen, &nItemOver);
					while( *szItem)
					{
						CString strAddInLoadString;
						if( m_mapAddInLoadString.Lookup( szItem, strAddInLoadString))
							m_pAddInManager->LoadAddIn(	strAddInLoadString, nItemOver == 2);

						AddInNameListParse( szItem, szItemNext, &nItemLen, &nItemOver);
					}
				}
			}

			// (2006/8/1 - Seung-Won, Bae) Support Direct Real
			if( m_pIRealReceiver) m_pIRealReceiver->Release();
			m_pIRealReceiver = ( IRealReceiver *)m_pAddInManager->GetAddInCustomInterface( g_aszAddInLoadString[ AID_DIRECT_REAL]);
		}
	}
	
	// main block setting
	SetMainBlock(IsRunTimeMode()) ;
	//sy 2006.03.02 Map Default
	SetMapDefaultInCurrent(true);

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	for Old Version Map and User File.
	if( 0 <= m_nVertScaleType)
	{
		m_pMainBlock->SetAllHorzScaleGridType( (CScaleBaseData::SCALEGRIDTYPE)m_nVertScaleType);
		m_pMainBlock->SetAllVertScaleGridType( (CScaleBaseData::SCALEGRIDTYPE)m_nVertScaleType);
	}

	//nami 8.9
	m_pMainBlock->SetMessageBoxShow( m_bShowWarningMessageBox ? TRUE:FALSE);
	m_pMainBlock->SetGraphBackgroundGap( m_nHorzGap, m_nVertGap);
	m_pMainBlock->SetBlockMoveDeleteFlag( m_bBlockMoveDelete);

	if( m_pAddInManager)
	{
		m_pAddInManager->OnAddInToolCommand( EAI_OBJECT_SET_USING_TOOL2,	m_bControlBlockSize		? TRUE : FALSE,
																			m_bBlockMoveDelete		? TRUE : FALSE,
																			m_bBlockMoveDelete		? TRUE : FALSE,
																			m_bUseScaleRButtonMenu	? TRUE : FALSE,
																			m_bUseScaleRButtonMenu	? TRUE : FALSE,
																			m_bSelectGraph			? TRUE : FALSE, // graph title ����
																			m_bSelectGraph			? TRUE : FALSE, // graph ����
																			m_bSelectTitle			? TRUE : FALSE, // graph �̵�
																			m_bSelectTitle			? TRUE : FALSE); // graph ����

		// 25.5 ��ġ��ȸâ Type, LBT�ɼǿ� ���� ��ġ��ȸâ Type�� �ٷ� �����Ų��.
		// ����(05/05/12) ��ġ��ȸâ �̵����¸� �����Ѵ�.
		SetNumericalInquiryOption( m_nNumericalInquiryOption, false);
	}

	//sy 2005.04.26.
	ApplySkin();

	if(IsRunTimeMode())
	{
		if(m_bHide)		ShowWindow(SW_HIDE);
		else			ShowWindow(SW_SHOW);

		if( m_bUseRealData && m_pIRealReceiver) m_pIRealReceiver->ResetRealSettingInfo( m_strRealList);

		// (2004.11.17, ��¿�) ���� SetRealDrdsAdviseList("", "")������ Start ������ ���� m_strRealFieldNames�� 
		//		Clear ���� �ʵ��� ������ �����Ѵ�.
		// (2004.06.08, ��¿�) Global ���� ����� �߰��Ѵ�.
		m_xGSharedCli.MakeList( m_strGlobalSharedList);
		InitLSharedData();
	}
	
	// ADD: ����(04/03/12) ��Ʈ�� ��� ������ ��ģ�Ŀ� �Ʒ� �޼����� ȣ���Ͽ� �̴����ٰ� �� ��ġ��
	//					 ã�ư��� �Ѵ�. �޸����� 8306ȭ�鿡�� ���̴� ���׷� ���Ͼ�~
	CRect rect;
	GetClientRect(&rect);
	WINDOWPOS wp;
	wp.hwnd = this->GetSafeHwnd();
	wp.cx = rect.Width();
	wp.cy = rect.Height();
	wp.flags = SWP_SHOWWINDOW;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
	// ADD: 04/03/12

	// (2006/11/23 - Seung-Won, Bae) Support Auto Delete Indicator Mode in Design Time for New Upadated Indicator CFG.
	m_pMainBlock->SetAutoDeleteIndicatorInfo( TRUE);

	// (2006/12/10 - Seung-Won, Bae) Clear Not Used IndicatorInfo in Design Time.
	if( !IsRunTimeMode())
	{
		CList<CString, CString> graphNameList;
		m_pIndicatorList->GetGraphNameList(  EILT_USER, EIIT_ALL, graphNameList);
		while( !graphNameList.IsEmpty())
		{
			CString strGraphName = graphNameList.RemoveTail();
			if( !m_pMainBlock->IsExistenceGraph( strGraphName)) m_pIndicatorList->RemoveGraphIndicatorInfo( strGraphName);
		}
	}

	// (2007/1/7 - Seung-Won, Bae) Run Default Drag Function.
	RunToolFunction( CToolOptionInfo::T_DRAG_X_ZOOM, 1);

// (2009/10/23 - Seung-Won, Bae) for Real TEST
#ifdef _DEBUG
	LoadAddIn( "ChartCommonAddIn.dll:REAL_TIME_TRANSDUCER");
#endif
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.11.12
//
//	����	:	Default �Ӽ��� ������ ���ϵ���, GetAllProperty2�� �����Ѵ�.
//
//	����	:	1. SK Enstock Component�� BigBang AnyVersion�� �����ϸ鼭,
//					Chart�� BigBang�� AnyChart�� KTBChart�� ���� �����Ͽ�����,
//					�� Component�� GetAllProperty2�� �����ؾ� �Ѵ�.
//
//	����	:	(2003.11.12) '�ǽð� ���Ž� Event �߻�' ������ Flag�� Property�� �������� �ʴ´�.
//					SK�� Property Page�� �ش� UI�� ������ OCX�� ���޵��� �ʰ�,
//					Any�� OCX�δ� ���޵ǳ� Property Page���� UI�� ������ �ʴ´�. (Hidden)
///////////////////////////////////////////////////////////////////////////////
long CKTBChartCtrl::GetAllProperties2(long lpData) 
{
	// TODO: Add your dispatch handler code here

	CStringList*  pPropStrList = ( CStringList*) lpData;
	if(pPropStrList == NULL) return 0;

	CPropertyMap	pmProperty;
	IPropertyMap *	pIPropertyMap = &pmProperty;
	long lResult =  GetAllProperties3( ( long)pIPropertyMap);

	// (2006/6/19 - Seung-Won, Bae) Convert to IPropertyMap
	const char *	szKey	= NULL;
	const char *	szValue	= NULL;
	CString			strPropertyInfo;
	POSITION		posProperty = pIPropertyMap->GetStartPosition();
	while( posProperty)
	{
		pIPropertyMap->GetNextAssoc( posProperty, szKey, szValue);
		strPropertyInfo.Format( "%s=%s", szKey, szValue);
		pPropStrList->AddTail( strPropertyInfo);
	}

	return lResult;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/19
// Modifier		: 
// Comments		: GetAllProperties3 for IPropertyMap
//-----------------------------------------------------------------------------
long CKTBChartCtrl::GetAllProperties3(long p_lIPropertyMap) 
{
	// TODO: Add your dispatch handler code here

	// (2006/10/24 - Seung-Won, Bae) Support Indicator Configuration in Design Time. (ChartList can be changed by EnvSetup.)
	ResetPropertiesfromMainBlock();
	
	IPropertyMap *pIPropertyMap = ( IPropertyMap *)p_lIPropertyMap;

	// (2006/10/25 - Seung-Won, Bae) Clear Property Map
	pIPropertyMap->RemoveAll();

	// (2006/7/5 - Seung-Won, Bae) Manage the flag of Full GetAllProperties3()
	CHART_ADDTOSTRLIST_START( m_bFullGetAllProperties3)
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRBACKGROUND					], m_clrBackground,						_DPV_CLRBACKGROUND					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRBACKBORDER					], m_clrBackBorder,						_DPV_CLRBACKBORDER					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRBLOCK						], m_clrBlock,							_DPV_CLRBLOCK						);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRBLOCKBORDER					], m_clrBlockBorder,					_DPV_CLRBLOCKBORDER					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRSCALELINE					], m_clrScaleLine,						_DPV_CLRSCALELINE					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRSCALELINE2					], m_clrScaleLine2,						_DPV_CLRSCALELINE2					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRSCALETEXT					], m_clrScaleText,						_DPV_CLRSCALETEXT					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE1					], m_clrGraphBase1,						_DPV_CLRGRAPHBASE1					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE2					], m_clrGraphBase2,						_DPV_CLRGRAPHBASE2					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE3					], m_clrGraphBase3,						_DPV_CLRGRAPHBASE3					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE4					], m_clrGraphBase4,						_DPV_CLRGRAPHBASE4					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE5					], m_clrGraphBase5,						_DPV_CLRGRAPHBASE5					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE6					], m_clrGraphBase6,						_DPV_CLRGRAPHBASE6					);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRANALYTICSTOOL				], m_clrAnalysisTool,					_DPV_CLRANALYTICSTOOL				);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRINQUIRYWINDOW				], m_clrInquiryWindow,					_DPV_CLRINQUIRYWINDOW				);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	TYPEBLOCKTYPE					], m_nBlockType,						_DPV_TYPEBLOCKTYPE					);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	TYPEBLOCKCOMPART				], m_nBlockCompart,						_DPV_TYPEBLOCKCOMPART				);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NBLOCKROWCOUNT					], m_nBlockRowCount,					_DPV_NBLOCKROWCOUNT					);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NBLOCKCOLUMNCOUNT				], m_nBlockColumnCount,					_DPV_NBLOCKCOLUMNCOUNT				);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NBLOCKGAP						], m_nBlockGap,							_DPV_NBLOCKGAP						);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	BLOCKBORDERTHICKNESS			], m_nBlockBorderThickness,				_DPV_BLOCKBORDERTHICKNESS			);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NROWDOUBLESIZEBLOCK				], m_nRowDoubleSizeBlock,				_DPV_NROWDOUBLESIZEBLOCK			);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NCOLUMNDOUBLESIZEBLOCK			], m_nColumnDoubleSizeBlock,			_DPV_NCOLUMNDOUBLESIZEBLOCK			);
		CHART_ADDTOSTRLIST_COLOR(		pIPropertyMap, g_anPropertyShortKey[	CLRRATTICE						], m_clrLattice,						_DPV_CLRRATTICE						);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRCHARTLIST					], m_strChartList,						_DPV_STRCHARTLIST					);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	TYPEBORDER						], m_nBorderType,						_DPV_TYPEBORDER						);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BWHOLEVIEW						], m_bWholeView,						_DPV_BWHOLEVIEW						);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NONEPAGEDATACOUNT				], m_nOnePageDataCount,					_DPV_NONEPAGEDATACOUNT				);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NUSERBLOCKADDCOUNT				], m_nUserBlockAddCount,				_DPV_NUSERBLOCKADDCOUNT				);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BGRAPHDRAGDROP					], m_bGraphDragDrop,					_DPV_BGRAPHDRAGDROP					);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NLEFTMARGIN						], m_nLeftMargin,						_DPV_NLEFTMARGIN					);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NRIGHTMARGIN					], m_nRightMargin,						_DPV_NRIGHTMARGIN					);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NTOPMARGIN						], m_nTopMargin,						_DPV_NTOPMARGIN						);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NBOTTOMMARGIN					], m_nBottomMargin,						_DPV_NBOTTOMMARGIN					);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BADDGRAPHMENUSHOW				], m_bAddGraphMenuShow,					_DPV_BADDGRAPHMENUSHOW				);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRENVIRONMENTFILENAME			], m_strUserEnvironmentFileName,		_DPV_STRENVIRONMENTFILENAME			);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRUSERINDICATORLISTFILENAME	], m_strUserIndicatorListFileName,		_DPV_STRUSERINDICATORLISTFILENAME	);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BHIDE							], m_bHide,								_DPV_BHIDE							);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BISINPUT						], m_bIsInput,							_DPV_BISINPUT						);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRTOOLBAROPTION				], m_strToolbarOption,					_DPV_STRTOOLBAROPTION				);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRPACKETOPTION					], m_strPacketOption,					_DPV_STRPACKETOPTION				);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRLOCALSHAREDLIST				], m_strLocalSharedList,				_DPV_STRLOCALSHAREDLIST				);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BUSEREALDATA					], m_bUseRealData,						_DPV_BUSEREALDATA					);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRREALLIST						], m_strRealList,						_DPV_STRREALLIST					);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NMAGINBORDERTYPE				], m_nMarginBorderType,					_DPV_NMAGINBORDERTYPE				);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BUSEINQUIRYDLGWHENUSECROSSLINE	], m_bUseInquiryDlgWhenUseCrossLine,	_DPV_BUSEINQUIRYDLGWHENUSECROSSLINE	);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BUSESHORTKEY					], m_bUseShortKey,						_DPV_BUSESHORTKEY					);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BUSECONDITIONTOTITLE			], m_bUseConditiontoTitle,				_DPV_BUSECONDITIONTOTITLE			);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	BUSEDATATOTITLE					], m_bUseDatatoTitle,					_DPV_BUSEDATATOTITLE				);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	nHorzGap						], m_nHorzGap,							_DPV_nHorzGap						);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	nVertGap						], m_nVertGap,							_DPV_nVertGap						);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	nRButtonMenuType				], m_nRButtonMenuType,					_DPV_nRButtonMenuType				);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	bShowWarningMessageBox			], m_bShowWarningMessageBox,			_DPV_bShowWarningMessageBox			);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	bControlBlockSize				], m_bControlBlockSize,					_DPV_bControlBlockSize				);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	bBlockMoveDelete				], m_bBlockMoveDelete,					_DPV_bBlockMoveDelete				);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	bSelectTitle					], m_bSelectTitle,						_DPV_bSelectTitle					);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	bSelectGraph					], m_bSelectGraph,						_DPV_bSelectGraph					);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	bUseScaleRButtonMenu			], m_bUseScaleRButtonMenu,				_DPV_bUseScaleRButtonMenu			);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	nGraphRButtonType				], m_nGraphRButtonType,					_DPV_nGraphRButtonType				);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	nHoldingPacketSize				], m_nHoldingPacketSize,				_DPV_nHoldingPacketSize				);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	nMinDataCountOnOnePage			], m_nMinDataCountOnOnePage,			_DPV_nMinDataCountOnOnePage			);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRBASEPACKETNAME				], m_szBasePacketName,					_MTEXT( C0_DATE_TIME)				);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	bFIREREALEVENT					], m_bFireRealEvent,					_DPV_bFIREREALEVENT					);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	NUMERICALINQUIRYOPTION			], m_nNumericalInquiryOption,			_DPV_NUMERICALINQUIRYOPTION			);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	STRGLOBALSHAREDLIST				], m_strGlobalSharedList,				_DPV_STRGLOBALSHAREDLIST			);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	USESKIN							], m_bDoesUseSkinColor,					_DPV_USESKIN						);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	MAPNAME							], m_strMapName,						_DPV_MAPNAME						);
		CHART_ADDTOSTRLIST_INT(			pIPropertyMap, g_anPropertyShortKey[	BONGCOUNTMARGIN					], m_nBongCountMargin,					_DPV_BONGCOUNTMARGIN				);
		CHART_ADDTOSTRLIST_BOOL(		pIPropertyMap, g_anPropertyShortKey[	DISPLAYTRDATA					], m_bOnePageDataCountInTRCount,		_DPV_DISPLAYTRDATA					);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	CUSTOMFONT						], m_strCSFont,							_DPV_CUSTOMFONT						);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	BLOCKREGION						], m_strBlockRegion,					_DPV_BLOCKREGION					);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	ADDINLIST						], m_strAddInLoadList,					_DPV_ADDINLIST						);
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	MAPINDICATORINFO				], m_strMapIndicatorInfo,				_DPV_MAPINDICATORINFO				);

		// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
		CHART_ADDTOSTRLIST_STRING(		pIPropertyMap, g_anPropertyShortKey[	LOCALLANGUAGETABLE				], m_strLocalLanguageTable,				_DPV_LOCALLANGUAGETABLE				);
	CHART_ADDTOSTRLIST_END

	// (2006/7/5 - Seung-Won, Bae) Manage the flag of Full GetAllProperties3()
	m_bFullGetAllProperties3 = FALSE;

	return 1;
}

// (2006/8/8 - Seung-Won, Bae) Use EnvSetupDlg AddIn for Graph Indicator Info
int CKTBChartCtrl::ShowIndicatorConfigDlg( CIndicatorInfo *p_pIndicatorInfo)
{
	long lResult = IDCANCEL;
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_ENVDLG_SHOW_SETUP_DLG, p_pIndicatorInfo);
	return lResult;
}

// (2006/8/23 - Seung-Won, Bae) Do not support the step for each Version Up.
//			Because the Base String for each version is not managed.
//		p_szOldVersion	: "DDDDDD:"
//		p_szRemove		: "AAAA1,...:BBBB1,..." ~ ":"	(Caustion! Must be sorted by OLD order)
//		p_szInsert		: "AAAA1,...:BBBB1,..." ~ ":"	(Caustion! Must be sorted by NEW order)
//							if "AAAA1" is in p_szRemove and p_szInsert, then "AAAA1" is always inserted in the New position.
//		p_bReposition	: if TRUE and "AAAA1" is in p_szRemove and p_szInsert, then "AAAA1" is inserted in the new position when It is in the Target Data.
//			Caution. If "AAAA1" is in p_szInsert and in the Target Data. then "AAAA1" is inserted twice.
//						If you want to do ONLY reposition, then "AAAA1" is in p_szRemove and p_bReposition is TRUE.
BOOL CKTBChartCtrl::AddInNameListVersionUpFor( const char *p_szOldVersion, const char *p_szRemove, const char *p_szInsert, BOOL p_bReposition)
{
	if( !p_szOldVersion || ( !p_szRemove && !p_szInsert)) return FALSE;
	if( strlen( p_szOldVersion) != 7) return FALSE;						// Minimum format "DDDDDD:", "", ""
	if( strncmp( p_szOldVersion, m_strAddInLoadList, 7)) return FALSE;	// Not Target Version.

	// 1. Create Remove Item Array for Search
	CString strRemove;
	char *szRemove = strRemove.GetBufferSetLength( p_szRemove ? strlen( p_szRemove) : 1);
	if( p_szRemove) strcpy( szRemove, p_szRemove);
	else *szRemove = '\0';
	char *szRemoveNext	= NULL;
	AddInNameListParse( szRemove, szRemoveNext);
	CStringList slRemove;
	while( *szRemove)
	{
		slRemove.AddTail( szRemove);
		AddInNameListParse( szRemove, szRemoveNext);
	}

	// 1.5 Create Old Item Array for Search
	CString strOld;
	char *szOld = strOld.GetBufferSetLength( m_strAddInLoadList.GetLength() - 7);
	strcpy( szOld, ( const char *)m_strAddInLoadList + 7);
	char *szOldNext = NULL;
	AddInNameListParse( szOld, szOldNext);
	CStringList slOld;
	while( *szOld)
	{
		slOld.AddTail( szOld);
		AddInNameListParse( szOld, szOldNext);
	}

	// 2. Create Input Buffer
	CString strBase;
	char *szBase = strBase.GetBufferSetLength( strlen( _ADDIN_LOAD_ORDER) - 7);
	strcpy( szBase, ( const char *)_ADDIN_LOAD_ORDER + 7);
	CString strInsert;
	char *szInsert = strInsert.GetBufferSetLength( p_szInsert ? strlen( p_szInsert) : 1);
	if( p_szInsert) strcpy( szInsert, p_szInsert);
	else *szInsert = '\0';

	// 3. Create Output Buffer
	CString strAddInNameList;
	char *szAddInNameList = strAddInNameList.GetBufferSetLength( m_strAddInLoadList.GetLength() + ( p_szInsert ? strlen( p_szInsert) + 1 : 0)); // Insert with Comma

	// 4. Set New Version Number
	char *szBuffer = szAddInNameList;
	strncpy( szBuffer, _ADDIN_LOAD_ORDER, 7);
	szBuffer += 7;

	// 5. Get first Items
	int nBaseOver	= 0;	// Over Code : 0(Under), 1(Last Under), 2(Over)
	int nBaseLen	= 0;
	int nInsertLen	= 0;
	char *szBaseNext	= NULL;
	char *szInsertNext	= NULL;
	AddInNameListParse( szBase,		szBaseNext,		&nBaseLen,	&nBaseOver);
	AddInNameListParse( szInsert,	szInsertNext,	&nInsertLen);

	// 6. Check Old Item Not in Remove Item
	while( !slOld.IsEmpty())
	{
		if( slRemove.Find( slOld.GetHead())) slOld.RemoveHead();
		else break;
	}

	// 7. for Every Base Item
	BOOL bItemCopied = FALSE;
	while( *szBase)		// && (!slOld.IsEmpty() || *szInsert)	~ Do not skip checking for ':'
	{
		bItemCopied = FALSE;
		// 8. Check Old Item's Order
		if( !slOld.IsEmpty()) if( !strcmp( slOld.GetHead(), szBase))
		{
			strcpy( szBuffer, szBase);
			szBuffer += nBaseLen;
			*szBuffer = ',';
			szBuffer++;
			bItemCopied = TRUE;

			// 8.1 Get Next Old Item not in Remove Item.
			slOld.RemoveHead();
			while( !slOld.IsEmpty())
			{
				if( slRemove.Find( slOld.GetHead())) slOld.RemoveHead();
				else break;
			}
		}

		// 9. Check Insert Item's Order
		if( *szInsert && nInsertLen == nBaseLen) if( !strcmp( szInsert, szBase))
		{
			if( !bItemCopied)
			{
				BOOL bFoundInOld = FALSE;
				if( p_bReposition && !slOld.IsEmpty()) bFoundInOld = ( slOld.Find( szInsert) != NULL);

				if( !p_bReposition || !bFoundInOld)
				{
					strcpy( szBuffer, szBase);
					szBuffer += nBaseLen;
					*szBuffer = ',';
					szBuffer++;
					bItemCopied = TRUE;
				}
			}

			// 9.1 Get Next Insert Item
			AddInNameListParse( szInsert, szInsertNext, &nInsertLen);
		}

		// *. Mark Under Over Seperator
		if( nBaseOver == 1)
		{
			if( *( szBuffer - 1) == ',') *( szBuffer - 1) = ':';
			else
			{
				*szBuffer = ':';
				szBuffer++;
			}
		}

		AddInNameListParse( szBase, szBaseNext, &nBaseLen, &nBaseOver);
	}

	// 10. Clear Last Comma and Set NULL Termiantor
	if( *( szBuffer - 1) == ',') *( szBuffer - 1) = '\0';
	else *szBuffer = '\0';

	// 11. Remove Parsed List
	slRemove.RemoveAll();
	slOld.RemoveAll();

	m_strAddInLoadList = szAddInNameList;
	return TRUE;
}

BOOL CKTBChartCtrl::AddInNameListParse( char *&szItem, char *&szItemNext, int *pnItemLength, int *pnItemOver)
{
	if( pnItemOver) if( *pnItemOver == 1) *pnItemOver = 2;

	// 1. Check first Item
	if( !szItemNext)
	{
		szItemNext = szItem;
		while( *szItemNext != ',' && *szItemNext != ':' && *szItemNext != '\0') szItemNext++;
		if( pnItemLength) *pnItemLength = szItemNext - szItem;
		if( *szItemNext)
		{
			if( pnItemOver) if( *szItemNext == ':') *pnItemOver = 1;
			*szItemNext = '\0';
			szItemNext++;
		}

		// 2. Check First Over Item
		if( *szItem)		return TRUE;																// found UnderItem
		if( *szItemNext)	return AddInNameListParse( szItem, szItemNext, pnItemLength, pnItemOver);	// find OverItem
		return TRUE;																					// found No Item
	}

	// 3. Get Next Item
	szItem = szItemNext;
	if( !*szItemNext) return TRUE;	// No next Item.
	while( *szItemNext != ',' && *szItemNext != ':' && *szItemNext != '\0') szItemNext++;
	if( pnItemLength) *pnItemLength = szItemNext - szItem;
	if( *szItemNext)
	{
		if( pnItemOver) if( *szItemNext == ':') *pnItemOver = 1;
		*szItemNext = '\0';
		szItemNext++;
	}
	return TRUE;					// found next Item.
}

void CKTBChartCtrl::OnStrAddInLoadListChanged() 
{
	// TODO: Add notification handler code
	SetModifiedFlag();
}

// (2006/8/25 - Seung-Won, Bae) Enable chart menu with force for auto delete function.
void CKTBChartCtrl::OnUpdateDeleteBlock(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_bBlockMoveDelete);	
}
void CKTBChartCtrl::OnUpdateReturnOrgDisplay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( TRUE);	
}
void CKTBChartCtrl::OnUpdateReturnOrg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( TRUE);	
}
void CKTBChartCtrl::OnUpdateToExcel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( TRUE);	
}
void CKTBChartCtrl::OnUpdatePrintScreen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( TRUE);	
}
void CKTBChartCtrl::OnUpdateLoadMapGwan(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( TRUE);	
}
void CKTBChartCtrl::OnUpdateReturnToGeneralChart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( TRUE);	
}

// (2006/9/29 - Seung-Won, Bae) Support the flag to show the min/max vlaue Text of price.
BOOL CKTBChartCtrl::GetBShowPriceMinMaxText() 
{
	// TODO: Add your property handler here
	
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->GetShowPriceMinMaxText();
}
void CKTBChartCtrl::SetBShowPriceMinMaxText(BOOL bNewValue) 
{
	// TODO: Add your property handler here

	if( !m_pMainBlock) return;

	m_bShowPriceMinMaxText = bNewValue;
	m_pMainBlock->SetShowPriceMinMaxText( bNewValue);

	SetModifiedFlag();

	InvalidateControl();
}

// (2006/10/13 - Seung-Won, Bae) Support Hide All Title for Script
BOOL CKTBChartCtrl::GetBHideAllTitle() 
{
	// TODO: Add your property handler here
	return m_bAllGraphTitleHiding;
}
void CKTBChartCtrl::SetBHideAllTitle(BOOL bNewValue) 
{
	// TODO: Add your property handler here
	SetModifiedFlag();

	m_bAllGraphTitleHiding = bNewValue;
	if( m_pMainBlock) m_pMainBlock->SetAllGraphsTitleHiding( m_bAllGraphTitleHiding);
	InvalidateControl();
}

// (2006/10/15 - Seung-Won, Bae) Show Indicator Configuration Dialog with Indicator Name
BOOL CKTBChartCtrl::ShowIndicatorConfigDlgWithName(LPCTSTR p_szIndicatorName2) 
{
	// TODO: Add your dispatch handler code here

	if( !m_pAddInManager || !m_pMainBlock || !m_pIndicatorList) return FALSE;

	CString p_szIndicatorName = _STOM2( p_szIndicatorName2);
	if( !HasIndicator( p_szIndicatorName))
	{
		if( IsRunningOneChart() && HasIndicatorInSpecialBack( p_szIndicatorName) && !g_iMetaTable.IsSpecialGroupGraph( p_szIndicatorName))
			RollbackLastNoOneChart();
		else
			AddChartBlock( p_szIndicatorName);
	}

	CString strGraphName;
	int nR = -1, nC = -1, nV = -1, nG = -1;
	// RQ�� �˻� : �������� - ojtaso (20070621)
	if( !m_pMainBlock->FindFirstIndicator( p_szIndicatorName, nR, nC, nV, nG, strGraphName, m_strRQ)) return FALSE;
	
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
	if( !pIndicatorInfo) return FALSE;

	long lResult = IDCANCEL;
	m_pAddInManager->OnAddInToolCommand( lResult, EAI_ENVDLG_SHOW_SETUP_DLG, pIndicatorInfo);
	InvalidateControl();
	return TRUE;
}

// (2006/10/17 - Seung-Won, Bae) Add the Menu of GlanceBalance Term Cursor
void CKTBChartCtrl::OnUpdateGlancebalanceCursor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( !pCmdUI) return;
	pCmdUI->Enable( FALSE);								// (2006/10/18 - Seung-Won, Bae) Do not support in KC not yet.
	if( m_pAddInManager)
	{
		long lResult = 0;
		m_pAddInManager->OnAddInToolCommand( lResult, EAI_GLANCEBALANCE_CURSOR_GET_STATE);
		pCmdUI->SetCheck( lResult);
	}
}
void CKTBChartCtrl::OnGlancebalanceCursor() 
{
	// TODO: Add your command handler code here
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_GLANCEBALANCE_CURSOR_TOGGLE_STATE);
}

// (2006/10/18 - Seung-Won, Bae) Add the Menu of the Hiding Graph Title.
void CKTBChartCtrl::OnUpdateHideGraphTitle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( FALSE);								// (2006/10/18 - Seung-Won, Bae) Do not support in KC not yet.
	pCmdUI->SetCheck( m_bAllGraphTitleHiding ? 1 : 0);
}
void CKTBChartCtrl::OnHideGraphTitle() 
{
	// TODO: Add your command handler code here
	SetBHideAllTitle( !m_bAllGraphTitleHiding);
}

// (2006/10/18 - Seung-Won, Bae) Add the Menu of the Showing Chart Price Min/Max Text.
void CKTBChartCtrl::OnUpdateShowMinmaxText(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( FALSE);								// (2006/10/18 - Seung-Won, Bae) Do not support in KC not yet.
	pCmdUI->SetCheck( m_bShowPriceMinMaxText);
}
void CKTBChartCtrl::OnShowMinmaxText() 
{
	// TODO: Add your command handler code here
	SetBShowPriceMinMaxText( !m_bShowPriceMinMaxText);
}

// (2006/10/18 - Seung-Won, Bae) Add AddIn Message Proc.
LRESULT CKTBChartCtrl::OnAddInMsg( WPARAM wParam, LPARAM lParam)
{
	if( m_pAddInManager) m_pAddInManager->OnAddInMsg( ( const char *)wParam, lParam);
	return 0;
}

// (2009/4/13 - Seung-Won, Bae) ChartList for AddIn Interface.
const char *CKTBChartCtrl::GetChartList( void) 
{
	m_strChartList.Empty();
	if( m_pMainBlock) m_pMainBlock->GetAllBlocksData( m_strChartList);
	return m_strChartList;
}
void CKTBChartCtrl::SetChartList( LPCTSTR lpszNewValue) 
{
	m_strChartList = lpszNewValue;
	if(m_strChartList.Right(2) != "\r\n") m_strChartList += "\r\n";

	if( m_pMainBlock)
	{
		m_pMainBlock->BuildGraphsAndScales( m_strChartList, TRUE);
		InvalidateControl();
	}
}
// (2006/10/30 - Seung-Won, Bae) Support ChartList String from MainBlock.
BSTR CKTBChartCtrl::GetStrChartList() 
{
	GetChartList();
	return m_strChartList.AllocSysString();
}
void CKTBChartCtrl::SetStrChartList(LPCTSTR lpszNewValue) 
{
	SetChartList( lpszNewValue);
	SetModifiedFlag();
}

BSTR CKTBChartCtrl::GetStrMapIndicatorInfo() 
{
	// TODO: Add your property handler here
	// (2006/10/30 - Seung-Won, Bae) Load Map Default
	m_strMapIndicatorInfo.Empty();
	if( m_pIndicatorList) m_pIndicatorList->GetSaveIndicatorInfoString( IsRunTimeMode() ? EILT_MAP_DEFAULT : EILT_USER, m_strMapIndicatorInfo);
	return m_strMapIndicatorInfo.AllocSysString();
}

// (2006/11/12 - Seung-Won, Bae) StringMap ENV cfg : Chart ENV cfg File Loader
void CKTBChartCtrl::SetDataToMainBlock(const CString& strEnvData)
{
	if( strEnvData.IsEmpty()) return;

	// 1. Copy to Buffer
	CString strBuffer;
	char *szBuffer = strBuffer.GetBufferSetLength( strEnvData.GetLength());
	strcpy( szBuffer, strEnvData);

	// 2. Read to StringMap
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	CPropertyMap *pAddInEnvMap = new CPropertyMap;
	IPropertyMap *pIAddInEnvMap = pAddInEnvMap;
	char *szItemName = NULL;
	char *szItemData = NULL;
	char *szItemNext = strstr( szBuffer, "[");
	if( szItemNext) szItemNext++;
	while( szItemNext)
	{
		// 2.1 Get Name
		szItemName = szItemNext;
		szItemNext = strstr( szItemNext, "]\r\n");
		if( !szItemNext) break;
		*szItemNext = '\0';

		// 2.2 Get Data
		szItemData = szItemNext + 3;
		szItemNext = strstr( szItemNext + 1, "\r\n[");
		if( szItemNext)
		{
			*szItemNext = '\0';
			szItemNext += 3;
		}

		pIAddInEnvMap->SetAt( szItemName, szItemData);
	}

	// 3. Retrieve the Env Info
	const char *szData;
	// 3.1 BlockType [BT]
	if( pIAddInEnvMap->Lookup( "BT", szData)) m_nBlockType = atoi( szData);
	// 3.2. Block Compart Type [BCT]
	if( pIAddInEnvMap->Lookup( "BCT", szData)) m_nBlockCompart = atoi( szData);
	// 3.3 Block Row Count [BRC]
	int nRow = 0;
	if( pIAddInEnvMap->Lookup( "BRC", szData)) nRow = atoi( szData);
	// 3.4 Block Column Count [BCC]
	int nColumn = 0;
	if( pIAddInEnvMap->Lookup( "BCC", szData)) nColumn = atoi( szData);
	// 3.4.5 Block Row Column Count ���� ��ȿ���� Ȯ���Ͽ� �̿��Ѵ�.
	//		������ ������ �������� �ʴ´�.
	if( nRow <= 0 || nColumn <=0) return;
	m_nBlockColumnCount = nColumn;
	m_nBlockRowCount = nRow;
	// 3.5. Block Border Thickness [BBT]
	if( pIAddInEnvMap->Lookup( "BBT", szData)) m_nBlockBorderThickness = atoi( szData);
	// 3.6 Block Gap [BG]
	if( pIAddInEnvMap->Lookup( "BG", szData)) m_nBlockGap = atoi( szData);
	// 3.7 Block Color [CB] (Color Of Block)
	if( pIAddInEnvMap->Lookup( "CB", szData)) m_clrBlock = CDataConversion::Str2RGB( szData);
	// 3.8 Block Border Color [CBB] (Color Of Block Border)
	if( pIAddInEnvMap->Lookup( "CBB", szData)) m_clrBlockBorder = CDataConversion::Str2RGB( szData);
	// 3.9 Max Block Count [BAC] (Block Addable Count, Max Block Count with User)
	if( pIAddInEnvMap->Lookup( "BAC", szData)) m_nUserBlockAddCount = atoi( szData);
	// 3.10 Block Region [BR]
	if( pIAddInEnvMap->Lookup( "BR", szData)) m_strBlockRegion = szData;
	// 3.11 Graph List Info [CL] (Chart List)
	if( pIAddInEnvMap->Lookup( "CL", szData))
	{
		// 11.2 Data�� ���� ��쿡�� ����ġ �ʵ��� �Ѵ�.
		if( szData) m_strChartList = szData;
	}
	// 3.14 Scale Color [CSL] (Color of Scale Line)
	if( pIAddInEnvMap->Lookup( "CSL", szData))
	{
		CString strData( szData);
		// 14.1 Horizontal
		m_clrScaleLine = CDataConversion::Str2RGB( CDataConversion::GetStringData( strData, "\r\n"));
		// 14.2 Vertical
		m_clrScaleLine2 = CDataConversion::Str2RGB( CDataConversion::GetStringData( strData, "\r\n"));
		// 14.3 Text
		m_clrScaleText = CDataConversion::Str2RGB( strData);
	}
	// 3.15 Graph, 6 Base Color [CGB] (Color of Graph in 6 Base)
	if( pIAddInEnvMap->Lookup( "CGB", szData))
	{
		CString strData( szData);
		m_clrGraphBase1 = CDataConversion::Str2RGB( CDataConversion::GetStringData( strData, "\r\n"));
		m_clrGraphBase2 = CDataConversion::Str2RGB( CDataConversion::GetStringData( strData, "\r\n"));
		m_clrGraphBase3 = CDataConversion::Str2RGB( CDataConversion::GetStringData( strData, "\r\n"));
		m_clrGraphBase4 = CDataConversion::Str2RGB( CDataConversion::GetStringData( strData, "\r\n"));
		m_clrGraphBase5 = CDataConversion::Str2RGB( CDataConversion::GetStringData( strData, "\r\n"));
		m_clrGraphBase6 = CDataConversion::Str2RGB( strData);
	}
	// 3.16 OCX Background Color [CBKG] (Color of ocx BacKGround)
	if( pIAddInEnvMap->Lookup( "CBKG", szData)) m_clrBackground = CDataConversion::Str2RGB( szData);
	// 3.17 OCX Background Border Color [CBKB] (Color of ocx BacKground Border)
	if( pIAddInEnvMap->Lookup( "CBKB", szData)) m_clrBackBorder = CDataConversion::Str2RGB( szData);
	// 3.18 Analysis Tool Color [AT] (color of Analysis Tool)
	if( pIAddInEnvMap->Lookup( "AT", szData)) m_clrAnalysisTool = CDataConversion::Str2RGB( szData);
	// 3.19 Data Inquiry Window Color [CINQW] (Color of data INQuiry Window)
	if( pIAddInEnvMap->Lookup( "CINQW", szData)) m_clrInquiryWindow = CDataConversion::Str2RGB( szData);
	// 3.20 Lattice Line Color [CLTI] (Color of LatTIce)
	if( pIAddInEnvMap->Lookup( "CLTI", szData)) m_clrLattice = CDataConversion::Str2RGB( szData);
	// 3.21 Whole View Mode [WV] (Whole View mode)
	if( pIAddInEnvMap->Lookup( "WV", szData)) m_bWholeView = ( atoi( szData) == 1);
	// 3.22. Zoom Value [PDC] (one Page Data Count)
	if( pIAddInEnvMap->Lookup( "PDC", szData)) m_nOnePageDataCount = atoi( szData);
	// 3.23 Double Size Block [RDB] (Row index of DouBle size)
	if( pIAddInEnvMap->Lookup( "RDB", szData)) m_nRowDoubleSizeBlock = atoi( szData);
	// 3.24 User Margine [BONGMG]
	if( pIAddInEnvMap->Lookup( "BONGMG", szData)) m_nBongCountMargin = atoi( szData);
	// 3.25 Data Inquiry Window Type [Inquiry Type]
	//sy 2005.12.01. -> LBT�ɼǿ� ���� ��ġ��ȸâ Type ����(LBT ������ �巡�׽� �ɼ�)
	if( pIAddInEnvMap->Lookup( "Inquiry Type", szData))
	{
		CString strData( szData);
		if( 0 < strData.Find(";"))
		{
			// (2006/3/29 - Seung-Won, Bae) Check Auto Move (0)
			int nManualMove = atoi(CDataConversion::GetStringData(strData, ";"));
			// (2006/3/29 - Seung-Won, Bae) Flag of Using NIDLG (1) is already included in m_nNumericalInquiryOption
			atoi(CDataConversion::GetStringData(strData, ";"));	
			// (2006/2/8 - Seung-Won, Bae) Not used m_nNumericalInquiryDlgType, m_nInquiryType
			m_nNumericalInquiryOption = atoi(CDataConversion::GetStringData(strData, ";"));
			// (2006/3/29 - Seung-Won, Bae) Check Auto Move (0)
			if( 1 == nManualMove) m_nNumericalInquiryOption |= NIDLG_MANUALMOVE;
			// (2006/3/29 - Seung-Won, Bae) Reset Cross Line in None-NIDLG Case for DragZoom.
			if( !( m_nNumericalInquiryOption & NUMERICALINQUIRYDLG)) m_nNumericalInquiryOption &= m_nNumericalInquiryOption & ~CROSSLINEVIEW;
		}
		else  m_nNumericalInquiryOption = atoi( strData);
		// 25.5 ��ġ��ȸâ Type, LBT�ɼǿ� ���� ��ġ��ȸâ Type�� �ٷ� �����Ų��.
		// ����(05/05/12) ��ġ��ȸâ �̵����¸� �����Ѵ�.
		SetNumericalInquiryOption( m_nNumericalInquiryOption, false);
	}
	// 3.27 Graph Title, Condition Data Option [Use Condition to Title]
	if( pIAddInEnvMap->Lookup( "Use Condition to Title", szData)) m_bUseConditiontoTitle = ( atoi( szData) == 1);
	// 3.28 Graph Title, Last Data Option [Use Data to Title]
	if( pIAddInEnvMap->Lookup( "Use Data to Title", szData)) m_bUseDatatoTitle = ( atoi( szData) == 1);
	// 3.29 CrossLine, Data Inquiry Window Option [Use Inquiry With Crossline]
	if( pIAddInEnvMap->Lookup( "Use Inquiry With Crossline", szData)) m_nUseInquiryWithCrossline = atoi( szData);
	// 3.30 ZoomIn, ToolTip Option [Use ToolTip with ZoomIn]
	if( pIAddInEnvMap->Lookup( "Use ToolTip with ZoomIn", szData)) m_bToolTipwithZoomIn = ( atoi( szData) == 1);
	// 3.31 Lattice, Vertical Gap [Vert Gap]
	if( pIAddInEnvMap->Lookup( "Vert Gap", szData)) m_nVertGap = atoi( szData);
	// 3.32 Lattice, Horizontal Gap [Horz Gap]
	if( pIAddInEnvMap->Lookup( "Horz Gap", szData)) m_nHorzGap = atoi( szData);
	// 3.37 Vertical Scale by Min/Max Price [bShowMinMaxbyPrice]
	if( pIAddInEnvMap->Lookup( "bShowMinMaxbyPrice", szData)) m_bShowMinMaxbyPrice = ( atoi( szData) == 1);
	// 3.39 All Graph Title Hiding [bAllGraphTitleHiding]
	if( pIAddInEnvMap->Lookup( "bAllGraphTitleHiding", szData)) m_bAllGraphTitleHiding = ( atoi( szData) == 1);
	// 3.40 Analysis Tool Text Option, Trend Line Text [bShowTrendLineText]
	if( pIAddInEnvMap->Lookup( "bShowTrendLineText", szData)) m_bShowTrendLineText = ( atoi( szData) == 1);
	// 3.42 Analysis Tool Text Option, Vertical Line Text [bShowVerticalText]
	if( pIAddInEnvMap->Lookup( "bShowVerticalText", szData)) m_bShowVerticalText = ( atoi( szData) == 1);
	// 3.43 Analysis Tool Text Option, Horizontal Line Text Left [bShowHorizonTextLeft]
	// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
	if( pIAddInEnvMap->Lookup( "bShowHorizonTextLeft", szData)) m_bShowHorizonTextLeft = ( atoi( szData) == 1);
	// 3.43-1 Analysis Tool Text Option, Horizontal Line Text Right [bShowHorizonTextRight]
	// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
	if( pIAddInEnvMap->Lookup( "bShowHorizonTextRight", szData)) m_bShowHorizonTextRight = ( atoi( szData) == 1);
	// 3.44 Analysis Tool Text Option, Three Dividing Line Text [bShowThreeText]
	if( pIAddInEnvMap->Lookup( "bShowThreeText", szData)) m_bShowThreeText = ( atoi( szData) == 1);
	// 3.45 Analysis Tool Text Option, Four Dividing Line Text [bShowFourText]
	if( pIAddInEnvMap->Lookup( "bShowFourText", szData)) m_bShowFourText = ( atoi( szData) == 1);
	// 3.46 Analysis Tool Text Option, Fibonacci Line Text [bShowFiboText]
	if( pIAddInEnvMap->Lookup( "bShowFiboText", szData)) m_bShowFiboText = ( atoi( szData) == 1);
	// 3.49 (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
	if( pIAddInEnvMap->Lookup( "bShowPriceMinMaxText", szData)) m_bShowPriceMinMaxText = ( atoi( szData) == 1);

	// (2009/5/13 - Seung-Won, Bae) for Right Margin
	if( pIAddInEnvMap->Lookup( "RightMarginWidth2", szData)) m_nRightMarginWidth = atoi( szData);
	else if( pIAddInEnvMap->Lookup( "RightMarginWidth", szData)) m_nRightMarginWidth = atoi( szData) / 8;

	// xScaleManager���� ȯ�漳�� �� ���� : xScaleManager - ojtaso (20070621)
	if(m_pxScaleManager)
	{
		// (2009/2/8 - Seung-Won, Bae) Set initial of Fixed Interval to false. and Do not load previous user setting.
		if( pIAddInEnvMap->Lookup( "FixedInterval2", szData)) m_pxScaleManager->SetFixedInterval( atoi( szData), FALSE);
		// (2009/1/8 - Seung-Won, Bae) Change Key Name
		if( pIAddInEnvMap->Lookup( "CandleTypeOnResize", szData)) m_pxScaleManager->SetCandleTypeOnResize( ( CXScaleManager::RSA_TYPE)atoi( szData));
		else if( pIAddInEnvMap->Lookup( "RightMarginType", szData)) m_pxScaleManager->SetCandleTypeOnResize( ( CXScaleManager::RSA_TYPE)atoi( szData));
	}

	// (2009/5/17 - Seung-Won, Bae) for Future Time
	if( pIAddInEnvMap->Lookup( "FutureTimeCount", szData)) m_nFutureTimeCount = atoi( szData);
	if( pIAddInEnvMap->Lookup( "MouseWheelIndex", szData)) m_nMouseWheelIndex = atoi( szData);
	if( pIAddInEnvMap->Lookup( "MinMaxRatio", szData)) m_nMinMaxRatio = atoi( szData);

	// 20081007 JS.Kim	����ڽð��� ����  ���� ����<<
	if( m_pPacketListManager )
	{
		if( pIAddInEnvMap->Lookup( "TIME_DIFF", szData) )
		{
			CPacketList* pPacketList;
			int pos;
			CString strData( szData), strTmp;
			while( (pos = strData.Find("\r\n")) >= 0 )
			{
				strTmp = strData.Left(pos);
				CString strRQ = CDataConversion::GetStringData(strTmp, ";");
				CString strTimeName = CDataConversion::GetStringData(strTmp, ";");
				int nTimeDiff = atoi(CDataConversion::GetStringData(strTmp, ";"));
				pPacketList = m_pPacketListManager->GetPacketList(strRQ);
				if( pPacketList )
					pPacketList->SetBaseTime(strTimeName, nTimeDiff);

				strData = strData.Mid(pos+2);
			}
		}
	}
	//>>

	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	if( m_pAddInManager) m_pAddInManager->OnLoadEnvironment( pAddInEnvMap);

	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	if( pIAddInEnvMap->Lookup( _T("MAXBLOCKINDEX"), szData))
	{
		CBlockIndex maxBlockIndex;
		::sscanf( szData, _T("%d;%d"), &nRow, &nColumn);
		maxBlockIndex.SetIndex(nRow, nColumn);
		if(m_pMainBlock) m_pMainBlock->SetMaximizedBlock( maxBlockIndex);
	}

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	for Old Version Map and User File.
	// 3.12 Vertical Scale Line Type [SLT] (Scale Line Type)
	m_nScaleLineType = -1;
	m_nVertScaleType = -1;
	m_nHorzScaleType = -1;
	if( pIAddInEnvMap->Lookup( "SLT", szData))
	{
		m_nVertScaleType = atoi( szData);
		// 12.5 Vertical Scale Line Type�� �⺻ Scale Line Type�� �ݿ��Ͽ� �����Ѵ�.
		m_nScaleLineType = m_nVertScaleType;
	}
	// 3.13 Horizontal Scale Line Type [SHT] (Scale Line, Horizontal Type)
	if( pIAddInEnvMap->Lookup( "SHT", szData)) m_nHorzScaleType = atoi( szData);
	// 3.33 Horizontal Scale Type [HorzScaleType]
	if( pIAddInEnvMap->Lookup( "HorzScaleType", szData)) m_nHorzScaleType = atoi( szData);
	// 3.34 Vertical Scale Type [nVertScaleType]
	if( pIAddInEnvMap->Lookup( "nVertScaleType", szData)) m_nVertScaleType = atoi( szData);

	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	BOOL bDrawBaseLine = TRUE;
	if( pIAddInEnvMap->Lookup( "DrawBaseLine", szData)) bDrawBaseLine = (BOOL)atoi( szData);

	if(m_pMainBlock) m_pMainBlock->SetDrawBaseLine(bDrawBaseLine);

	// 4 ���� ��Ʈ�� ���, �Ϲ� ��Ʈ�� ���ư� ����� ȯ�� ������ Loading�Ѵ�. (2��° Graph Info List�� �ִ����� �ĺ��Ѵ�.)
	// 4.0 Graph List Info [CL2] (Chart List 2)
	if( !pIAddInEnvMap->Lookup( "CL2", szData))
	{
		delete pAddInEnvMap;
		return;
	}

	CString strChartList2 = szData;
	// 4.1 Block Row Count [BRC2]
	int nRow2 = 1;
	if( pIAddInEnvMap->Lookup( "BRC2", szData)) nRow2 = atoi( szData);
	// 4.4 Block Column Count [BCC2]
	int nColumn2 = 1;
	if( pIAddInEnvMap->Lookup( "BCC2", szData)) nColumn2 = atoi( szData);
	// 4.5 Double Size Row [BRDC] (Block index of Row, Double size Config)
	int nDoubleRow2 = -1;
	if( pIAddInEnvMap->Lookup( "BRDC", szData)) nDoubleRow2 = atoi( szData);
	// 4.6 Double Size Column [BCDC] (Block index of Column, Double size Config)
	int nDoubleColumn2 = -1;
	if( pIAddInEnvMap->Lookup( "BCDC", szData)) nDoubleColumn2 = atoi( szData);
	// 4.7 Block Region [BR2]
	CString strBlocksPosition;
	if( pIAddInEnvMap->Lookup( "BR2", szData)) strBlocksPosition = szData;
	// 4.10 Zoom Value [OPDC] (One Page Data Count)
	int nOnePageDataCount = m_nOnePageDataCount;
	if( pIAddInEnvMap->Lookup( "OPDC", szData)) nOnePageDataCount = atoi( szData);
	// 4.11 User Margine [BONGMG]
	int nBongCountMargin = m_nBongCountMargin;
	if( pIAddInEnvMap->Lookup( "BONGMG2", szData)) nBongCountMargin = atoi( szData);
	// 4.12 Whole View Mode [IWV] (Is Whole View mode)
	BOOL bWholeView = IsWholeView();
	if( pIAddInEnvMap->Lookup( "IWV", szData)) bWholeView = ( atoi( szData) == 1);

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	for Old Version Map and User File.
	// 4.8 Scale Type
	//		(Scale Type�� ȯ�漳������ �����Ǵ� Scale ���м��� �Ǽ�/����/���� ������ �����Ѵ�.
	//		 ���� ��Ʈ�� ��� ȯ�漳���� �Ұ����ϳ� Scale Popup Menu�� ��ü Scale ���м� ������ ���� �� �ִ�.
	//		 ���� ������ Scale ���м� ������ �����ϰ� �ȴ�.)
	// 4.8.1 Vertical Scale Type [VST]
	int nVertScaleType = -1;
	if( pIAddInEnvMap->Lookup( "VST", szData)) nVertScaleType = atoi( szData);

	// 4.* Save Last Configuration.
	m_SaveChartcfg.AddChartCfg(	pAddInEnvMap,
								nRow2, nColumn2,
								nDoubleRow2, nDoubleColumn2,
								nOnePageDataCount, nBongCountMargin, 
								strChartList2, strBlocksPosition, 
								bWholeView, 
								m_nNumericalInquiryOption,
								0, ///nPosition
								m_nUseInquiryWithCrossline,
								m_bToolTipwithZoomIn,
								m_bUseConditiontoTitle,
								m_bUseDatatoTitle,
								m_nVertGap, m_nHorzGap,
								m_bShowMinMaxbyPrice,
								m_bAllGraphTitleHiding, //sy 2004.11.29. ��� graph title ���� ����
								m_bShowTrendLineText,	
								m_bShowVerticalText,
								m_bShowHorizonTextLeft, m_bShowHorizonTextRight,// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
								m_bShowThreeText, m_bShowFourText, m_bShowFiboText,
								m_clrBackground, m_clrBackBorder,
								m_clrBlock, m_clrBlockBorder,
								m_clrScaleLine, m_clrScaleLine2, m_clrScaleText,
								m_clrAnalysisTool, m_clrInquiryWindow, m_clrLattice,
								// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
								m_bShowPriceMinMaxText,
								// (2006/12/10 - Seung-Won, Bae) Support Map Default Info. (Do not save in Env.)
								m_strMapIndicatorInfo,
								bDrawBaseLine,		// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
								nVertScaleType,
								m_nRightMarginWidth,	// (2009/5/13 - Seung-Won, Bae) for Right Margin
								m_nFutureTimeCount,		// (2009/5/17 - Seung-Won, Bae) for Future Time
								m_nMouseWheelIndex,
								m_nMinMaxRatio
								);
}

CString CKTBChartCtrl::GetDataFromMainBlock()
{
	if( !m_pMainBlock) return "";

	// 1. Gather the Basic Env Values
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	CPropertyMap *pAddInEnvMap = new CPropertyMap;
	IPropertyMap *pIAddInEnvMap = pAddInEnvMap;
	// 1.1 block type
	pIAddInEnvMap->SetAt( "BT", CDataConversion::IntToString( m_pMainBlock->GetBlockType()));
	// 1.2 block compart type
	pIAddInEnvMap->SetAt( "BCT", CDataConversion::IntToString( m_pMainBlock->GetBlockCompartType()));
	// 1.3 block row and column count
	pIAddInEnvMap->SetAt( "BRC", CDataConversion::IntToString( m_pMainBlock->GetRowCount()));
	pIAddInEnvMap->SetAt( "BCC", CDataConversion::IntToString( m_pMainBlock->GetColumnCount()));
	// 1.4 graph border thickness
	pIAddInEnvMap->SetAt( "BBT", CDataConversion::IntToString( m_pMainBlock->GetGraphRegionFrameThickness()));
	// 1.5 block gap
	pIAddInEnvMap->SetAt( "BG", CDataConversion::IntToString( m_pMainBlock->GetInterBlockHalfGap()));
	// 1.6 graph area color
	CGraphRegionColor lclrGraphRegion = m_pMainBlock->GetGraphRegionColor();
	pIAddInEnvMap->SetAt( "CB", CDataConversion::RGB2Str(lclrGraphRegion.GetBackgroundColor()));
	pIAddInEnvMap->SetAt( "CBB", CDataConversion::RGB2Str(lclrGraphRegion.GetFrameColor()));
	// 1.7 max block count
	pIAddInEnvMap->SetAt( "BAC", CDataConversion::IntToString( m_pMainBlock->GetMaxBlocksCountInView()));
	// 1.8 block area
	pIAddInEnvMap->SetAt( "BR", m_pMainBlock->GetBlocksPosition());
	// 1.9 block data
	CString strCL;
	m_pMainBlock->GetAllBlocksData( strCL);
	pIAddInEnvMap->SetAt( "CL", strCL);
	// 1.11 scale color
	// clsScaleLineHorz : ���α��м�
	// clrScaleLineVert : ���α��м�
	// ����, ����, �ؽ�Ʈ
	COLORREF clrScaleLineHorz, clrScaleLineVert, clrScaleText;
	m_pMainBlock->GetScaleColor(clrScaleLineVert, clrScaleLineHorz, clrScaleText);
	pIAddInEnvMap->SetAt( "CSL", CDataConversion::RGB2Str(clrScaleLineHorz) + "\r\n"
						   + CDataConversion::RGB2Str(clrScaleLineVert) + "\r\n"
						   + CDataConversion::RGB2Str(clrScaleText));
	// 1.12 base color
	COLORREF clrGraphBase1, clrGraphBase2, clrGraphBase3, 
			 clrGraphBase4, clrGraphBase5, clrGraphBase6;
	m_pMainBlock->GetEtcGraphColor(clrGraphBase1, clrGraphBase2, clrGraphBase3, clrGraphBase4, clrGraphBase5, clrGraphBase6);
	pIAddInEnvMap->SetAt( "CGB", CDataConversion::RGB2Str(clrGraphBase1) + "\r\n"
  						   + CDataConversion::RGB2Str(clrGraphBase2) + "\r\n"
						   + CDataConversion::RGB2Str(clrGraphBase3) + "\r\n"
						   + CDataConversion::RGB2Str(clrGraphBase4) + "\r\n"
						   + CDataConversion::RGB2Str(clrGraphBase5) + "\r\n"
						   + CDataConversion::RGB2Str(clrGraphBase6));
	// 1.13 background color
	pIAddInEnvMap->SetAt( "CBKG", CDataConversion::RGB2Str( m_clrBackground));
	// 1.14 background border color
	pIAddInEnvMap->SetAt( "CBKB", CDataConversion::RGB2Str( m_clrBackBorder));
	// 1.15 analysis tool color
	pIAddInEnvMap->SetAt( "AT", CDataConversion::RGB2Str( m_clrAnalysisTool));
	// 1.16 inquirydata window color
	pIAddInEnvMap->SetAt( "CINQW", CDataConversion::RGB2Str( m_clrInquiryWindow));
	// 1.17 �𴫼� color
	pIAddInEnvMap->SetAt( "CLTI", CDataConversion::RGB2Str( m_clrLattice));
	// 1.18 whole view
	pIAddInEnvMap->SetAt( "WV", CDataConversion::Bool2Str( m_bWholeView));
	// 1.19 one page data count
	pIAddInEnvMap->SetAt( "PDC", CDataConversion::IntToString( m_nOnePageDataCount));
	// 1.20 double size block
	pIAddInEnvMap->SetAt( "RDB", CDataConversion::IntToString( m_nRowDoubleSizeBlock));
	// 1.21 bong conunt margin
	pIAddInEnvMap->SetAt( "BONGMG", CDataConversion::IntToString( m_nBongCountMargin));
	// 1.22 (2006/2/8 - Seung-Won, Bae) m_nNumericalInquiryDlgType, m_nInquiryType are not used any more.
	pIAddInEnvMap->SetAt( "Inquiry Type", CDataConversion::IntToString( m_nNumericalInquiryOption));
	// 1.23 title
	pIAddInEnvMap->SetAt( "Use Condition to Title", CDataConversion::Bool2Str( m_bUseConditiontoTitle));
	pIAddInEnvMap->SetAt( "Use Data to Title", CDataConversion::Bool2Str( m_bUseDatatoTitle));
	// 1.24 nami 9.10
	pIAddInEnvMap->SetAt( "Use Inquiry With Crossline",	CDataConversion::IntToString(	m_nUseInquiryWithCrossline));
	pIAddInEnvMap->SetAt( "Use ToolTip with ZoomIn",	CDataConversion::Bool2Str(		m_bToolTipwithZoomIn));
	pIAddInEnvMap->SetAt( "Vert Gap",					CDataConversion::IntToString(	m_nVertGap));
	pIAddInEnvMap->SetAt( "Horz Gap",					CDataConversion::IntToString(	m_nHorzGap));
	// 1.26 nami 12.6
	pIAddInEnvMap->SetAt( "bShowMinMaxbyPrice", CDataConversion::Bool2Str( m_bShowMinMaxbyPrice));
	// 1.27 sy 2004.11.29. ��� graph title ���� ����
	pIAddInEnvMap->SetAt( "bAllGraphTitleHiding", CDataConversion::Bool2Str( m_bAllGraphTitleHiding));
	// 1.28 Analysis Tool Text Info
	pIAddInEnvMap->SetAt( "bShowTrendLineText", CDataConversion::Bool2Str( m_bShowTrendLineText));
	pIAddInEnvMap->SetAt( "bShowVerticalText", CDataConversion::Bool2Str( m_bShowVerticalText));
	pIAddInEnvMap->SetAt( "bShowHorizonTextLeft", CDataConversion::Bool2Str( m_bShowHorizonTextLeft));
	pIAddInEnvMap->SetAt( "bShowHorizonTextRight", CDataConversion::Bool2Str( m_bShowHorizonTextRight));
	pIAddInEnvMap->SetAt( "bShowThreeText", CDataConversion::Bool2Str( m_bShowThreeText));
	pIAddInEnvMap->SetAt( "bShowFourText", CDataConversion::Bool2Str( m_bShowFourText));
	pIAddInEnvMap->SetAt( "bShowFiboText", CDataConversion::Bool2Str( m_bShowFiboText));
	// 1.31 (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
	pIAddInEnvMap->SetAt( "bShowPriceMinMaxText", CDataConversion::Bool2Str( m_bShowPriceMinMaxText));

	// 2. Gather the Second Env Values
	// ���� ȭ���� one chart �� ��쿣 one chart ���� ������ file�� �����Ѵ�.
	CChartCfg* pChartCfg = GetLastChartCfg_NoOneChart();
	if( pChartCfg)
	{
		pIAddInEnvMap->SetAt( "CL2", pChartCfg->GetChartList());
		pIAddInEnvMap->SetAt( "BRC2", CDataConversion::IntToString( pChartCfg->GetRowCount()));
		pIAddInEnvMap->SetAt( "BCC2", CDataConversion::IntToString( pChartCfg->GetColumnCount()));
		pIAddInEnvMap->SetAt( "BRDC", CDataConversion::IntToString( pChartCfg->GetRowDoublesizeBlock()));
		pIAddInEnvMap->SetAt( "BCDC", CDataConversion::IntToString( pChartCfg->GetColumnDoublesizeBlock()));
		pIAddInEnvMap->SetAt( "BR2", pChartCfg->GetBlockRegion());
// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
//		pIAddInEnvMap->SetAt( "VST", CDataConversion::IntToString( pChartCfg->GetVertScaleType()));
//		pIAddInEnvMap->SetAt( "HST", CDataConversion::IntToString( pChartCfg->GetHorzScaleType()));
		pIAddInEnvMap->SetAt( "OPDC", CDataConversion::IntToString( pChartCfg->GetOnePageDataCount()));
		pIAddInEnvMap->SetAt( "BONGMG2", CDataConversion::IntToString( pChartCfg->GetBongCountMargin()));
		pIAddInEnvMap->SetAt( "IWV", CDataConversion::IntToString( pChartCfg->IsWholeView()));
		pIAddInEnvMap->SetAt( "bMMbP", CDataConversion::Bool2Str( pChartCfg->showMinMaxbyPrice()));
	}

	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	pIAddInEnvMap->SetAt( "DrawBaseLine", CDataConversion::Bool2Str( m_pMainBlock->GetDrawBaseLine()));

	// (2009/5/13 - Seung-Won, Bae) for Right Margin
	pIAddInEnvMap->SetAt( "RightMarginWidth2", CDataConversion::IntToString( m_nRightMarginWidth));

	// xScaleManager���� ȯ�漳�� �� ���� : xScaleManager - ojtaso (20070621)
	if(m_pxScaleManager)
	{
		// (2009/2/8 - Seung-Won, Bae) Set initial of Fixed Interval to false. and Do not load previous user setting.
		pIAddInEnvMap->SetAt( "FixedInterval2", CDataConversion::IntToString( m_pxScaleManager->IsFixedInterval()));
		pIAddInEnvMap->SetAt( "CandleTypeOnResize", CDataConversion::IntToString( m_pxScaleManager->GetCandleTypeOnResize()));
	}

	// (2009/5/17 - Seung-Won, Bae) for Future Time
	pIAddInEnvMap->SetAt( "FutureTimeCount", CDataConversion::IntToString( m_nFutureTimeCount));
	pIAddInEnvMap->SetAt( "MouseWheelIndex", CDataConversion::IntToString( m_nMouseWheelIndex));
	pIAddInEnvMap->SetAt( "MinMaxRatio", CDataConversion::IntToString( m_nMinMaxRatio));

	// 20081007 JS.Kim	����ڽð��� ����  ���� ����<<
	if( m_pPacketListManager )
	{
		CString strTimeDiffInfo = "", strTmp = "";
		CPacketList* pPacketList;
		CList <CString, CString> slRQ;

		pPacketList = m_pPacketListManager->GetPacketList("DEFAULT");
		strTimeDiffInfo.Format("%s;%s;%d;\r\n", "DEFAULT", pPacketList->GetBaseTimeName(), pPacketList->GetBaseTimeDifference());

		m_pPacketListManager->GetAllRQ( slRQ);
		POSITION psnRQ = slRQ.GetHeadPosition();
		while( psnRQ)
		{
			CString strRQ = slRQ.GetNext( psnRQ);
			pPacketList = m_pPacketListManager->GetPacketList( strRQ);
			strTmp.Format("%s;%s;%d;\r\n", strRQ, pPacketList->GetBaseTimeName(), pPacketList->GetBaseTimeDifference());
			strTimeDiffInfo += strTmp;
		}
		pIAddInEnvMap->SetAt( "TIME_DIFF", strTimeDiffInfo);
	}
	//>>

	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	if( m_pAddInManager) m_pAddInManager->OnSaveEnvironment( pAddInEnvMap);

	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	CBlockIndex maxBlockIndex = m_pMainBlock->GetMaximizedBlock();
	if( maxBlockIndex.IsAllValueBiggerThanZero())
	{
		CString strEnvValue;
		strEnvValue.Format(_T("%d;%d"), maxBlockIndex.GetRow(), maxBlockIndex.GetColumn());
		pIAddInEnvMap->SetAt( _T("MAXBLOCKINDEX"), strEnvValue);
	}

	// �м��� ���� ���� - ojtaso (20080728)
	pIAddInEnvMap->RemoveKey(_T("ANALYSIS_TOOL"));
	pIAddInEnvMap->RemoveKey(_T("PERIOD_SUMMARY"));
	pIAddInEnvMap->RemoveKey(_T("SYMBOL_TOOL"));

	// 3. Make Save String
	int nLenBuffer = 0;
	const char *szKey = NULL;
	const char *szValue = NULL;
	POSITION psnEnv = pIAddInEnvMap->GetStartPosition();
	while( psnEnv)
	{
		pIAddInEnvMap->GetNextAssoc( psnEnv, szKey, szValue);
		nLenBuffer += 3 + strlen( szKey) + 3 + strlen( szValue);
	}
	CString strData;
	char *szBuffer = strData.GetBufferSetLength( nLenBuffer);
	psnEnv = pIAddInEnvMap->GetStartPosition();
	while( psnEnv)
	{
		pIAddInEnvMap->GetNextAssoc( psnEnv, szKey, szValue);
		strcpy( szBuffer, "\r\n[");
		szBuffer += 3;
		strcpy( szBuffer, szKey);
		szBuffer += strlen( szKey);
		strcpy( szBuffer, "]\r\n");
		szBuffer += 3;
		strcpy( szBuffer, szValue);
		szBuffer += strlen( szValue);
	}

	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	delete pAddInEnvMap;

	return strData;
}

/////////////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
/////////////////////////////////////////////////////////////////////////////
BOOL CKTBChartCtrl::OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)
{
	if( m_pToolBarManager) m_pToolBarManager->OnToolStateChanged( p_eToolID, p_bTurnOn);
	// (2007/1/17 - Seung-Won, Bae) Notify with Message
	//		m_OutsideCommonInterfaceFunction.OnToolStateChanged( p_eToolID, p_bTurnOn);
	SendEventMsgToDLL( RMS_ANALYSIS_FUNC, p_eToolID, p_bTurnOn);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// (2006/11/17 - Seung-Won, Bae) Graph Add/Remove State Event Process
/////////////////////////////////////////////////////////////////////////////
BOOL CKTBChartCtrl::OnGraphAdded( const char *p_szRQName, const char *p_szGraphName, const char *p_szIndicatorName)
{
	// 2007/03/23 - yiilyoul@magicn.com
	if(CString(p_szIndicatorName) == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART))
	{
		// (2009/5/18 - Seung-Won, Bae) Set normal chart with force for returning from special chart on compare chart with multi-RQ.
		m_pPacketListManager->SetOneChart( FALSE, m_strRQ);
		if( m_pxScaleManager->GetRQIndexFromString( p_szRQName) < 0)
			m_pxScaleManager->Calc_DRI( FALSE);
		m_pxScaleManager->AddOEBTRQ( p_szRQName, p_szGraphName, p_szIndicatorName);
		ResetScroll( TRUE);
	}

	// (2006/11/17 - Seung-Won, Bae) Notify Graph State to MChart
	m_OutsideCommonInterfaceFunction.OnIndicatorAddOrRemoved( p_szGraphName, p_szIndicatorName, TRUE);
	if( m_pAddInManager) m_pAddInManager->OnGraphAdded( p_szGraphName, p_szIndicatorName);
	return TRUE;
}

BOOL CKTBChartCtrl::OnGraphDeleted( const char *p_szRQName, const char *p_szGraphName, const char *p_szIndicatorName)
{
	// 2007/03/23 - yiilyoul@magicn.com
	if(CString(p_szIndicatorName) == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART))
	{
		m_pxScaleManager->DelOEBTRQ( p_szRQName);
		ResetScroll( TRUE);
	}

	// (2006/11/17 - Seung-Won, Bae) Notify Graph State to MChart
	m_OutsideCommonInterfaceFunction.OnIndicatorAddOrRemoved( p_szGraphName, p_szIndicatorName, FALSE);
	if( m_pAddInManager) 
	{
		m_pAddInManager->OnGraphDeleted( p_szGraphName, p_szIndicatorName);
		// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
		m_pAddInManager->OnAddInToolCommand( EAI_ALL_ON_DELETE_INDICATOR_NOT_USED, 0, p_szRQName, p_szGraphName, p_szIndicatorName);
	}
	return TRUE;
}

//// (2006/11/18 - Seung-Won, Bae) Support Indicator Group with TR
//bool CKTBChartCtrl::IsIndicatorGroup_TR( const char *p_szIndicatorName)
//{
//	CString strGroupName;
//	if( m_pIndicatorList)
//	{
//		CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetDefaultIndicatorInfo( EILT_SITE_DEFAULT, p_szIndicatorName, EIIGT_WITH_DEFAULT_GET);
//		if( pIndicatorInfo) strGroupName = pIndicatorInfo->GetGroupName();
//	}
//	return IsIndicatorGroup_TR( strGroupName, p_szIndicatorName);
//}
//bool CKTBChartCtrl::IsIndicatorGroup_TR( const char *p_szIndicatorGroup, const char *p_szIndicatorName)
//{
//	if( !strcmp( p_szIndicatorGroup, _MTEXT( C7_MARKET))) return true;
//
//	static CMapStringToWord mapType;
//	if( mapType.GetCount() <= 0)
//	{
//		mapType.SetAt( _MTEXT( C2_VOLUME_AMOUNT), 0);
//	}
//
//	WORD wTemp;
//	return mapType.Lookup( p_szIndicatorName, wTemp);
//}

// (2006/12/12 - Seung-Won, Bae) Support Indicator Calculation Enabling Flag
BOOL CKTBChartCtrl::GetBEnableIndicatorCalculation() 
{
	// TODO: Add your property handler here
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->IsEnableIndicatorCalculation();
}
void CKTBChartCtrl::SetBEnableIndicatorCalculation(BOOL bNewValue) 
{
	// TODO: Add your property handler here
	SetModifiedFlag();
	if( m_pMainBlock)
	{
		// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
		BOOL bRecalculation = ( !m_pMainBlock->IsEnableIndicatorCalculation() && bNewValue);

		m_pMainBlock->EnableIndicatorCalculation( bNewValue);

		if( bRecalculation)
		{
			// (2006/7/20 - Seung-Won, Bae) Change ScrollInfo after Calculating Indicator for Special One Chart.
			ResetScroll( TRUE);
			InvalidateControl();

			// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
			if( m_pAddInManager) m_pAddInManager->OnRecalculationComplete();
		}
	}
}

// ���ǰ� ����� ���� ���� - ojtaso (20070913)
void CKTBChartCtrl::ReCalculateIndicator()
{
	if( m_pMainBlock)
	{
		m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::TRDATA_CAL);
		ResetScroll( TRUE);
		InvalidateControl();
	}
}

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
BOOL CKTBChartCtrl::SetBinTrHelpMsg(LPCTSTR p_szHelpMessage) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pPacketList) return FALSE;
	if( m_bEnableLogChartLoading) GetLocalTime( &m_stLogStart);

	//sy 2003.8.22.
	m_nBinTrPrevTimeInterval	= m_pPacketList->GetTimeInterval();
	m_eBinTrPrevUnitType		= m_pPacketList->GetDateUnitType();
	//sy end

	if(m_pPacketList->GetDataInsertType() == CPacketDataInfo::NEW)
		// �м����� & ��ȣ/����/������ ��� ����
		RunToolFunction(CToolOptionInfo::T_DELETE_ALL, 3);
	// 02.19.2002  system trader chart�� �����Ѵ�  
	if(m_pMainBlock)
	{
		m_pMainBlock->DeleteGraph("System Signal");
		m_pMainBlock->DeleteGraph("System InvestChange");
		// 03.13.2002  news�� �ŷ������� �����Ѵ�
		//m_pMainBlock->DeleteGraph("News");
		//m_pMainBlock->DeleteGraph("�ŷ�����");
	}

	BOOL bResult = m_pPacketList->SetBinTrHelpMsg( p_szHelpMessage);
	if( m_pPacketList->GetDataInsertType() == CPacketDataInfo::NEW)
		SetOneChart();
	return bResult;
}
BOOL CKTBChartCtrl::SetBinTrDoubleData(LPCTSTR p_szPacketName, long p_nCount, double FAR* p_pdData) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pPacketList) return FALSE;

	// (2009/5/29 - Seung-Won, Bae) Support No TR Test Environment.
	CString strEnvValue;
	GetTestEnvValue( "TR Limit", strEnvValue);
	if( !strEnvValue.IsEmpty())
	{
		long lCount = atol( strEnvValue);
		if( 0 <= lCount) p_nCount = lCount;
	}

	if( m_bEnableLogChartLoading) m_lDataCount = p_nCount;
	
	return m_pPacketList->SetBinTrDoubleData( p_szPacketName, p_nCount, p_pdData);
}
BOOL CKTBChartCtrl::SetBinTrStringData(LPCTSTR p_szPacketName, long p_nCount, long p_lData) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pPacketList) return FALSE;

	// (2009/5/29 - Seung-Won, Bae) Support No TR Test Environment.
	CString strEnvValue;
	GetTestEnvValue( "TR Limit", strEnvValue);
	if( !strEnvValue.IsEmpty())
	{
		long lCount = atol( strEnvValue);
		if( 0 <= lCount) p_nCount = lCount;
	}

	if( m_bEnableLogChartLoading) m_lDataCount = p_nCount;

	return m_pPacketList->SetBinTrStringData( p_szPacketName, p_nCount, ( const char **)p_lData);
}
BOOL CKTBChartCtrl::SetBinTrEnd() 
{
	// TODO: Add your dispatch handler code here
	BOOL bResult = m_pPacketList->SetBinTrEnd();

	//sy 2005.08.30.
	OnPacketTRData();

	if( m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\nData Count : %d\r\n",
			m_stLogStart.wHour, m_stLogStart.wMinute, m_stLogStart.wSecond, m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds,
			m_lDataCount);
		CDumpLogger::LogStringToFile( "CKTBChartCtrl::SetBinTr()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}

	return bResult;
}

BOOL CKTBChartCtrl::GetIsShowInneerToolBar() 
{
	// TODO: Add your property handler here
	return m_bShowInnerToolBar;
}

short CKTBChartCtrl::GetHorzScalePosition(short p_nRowIndex, short p_nColumnIndex) 
{
	// TODO: Add your dispatch handler code here

	if( !m_pMainBlock) return CScaleBaseData::HORZ_NONE;
	return m_pMainBlock->GetHorzScalePosition( CBlockIndex( p_nRowIndex, p_nColumnIndex));
}

// (2007/1/22 - Seung-Won, Bae) Add Indicator and Get Graph Name.
BOOL CKTBChartCtrl::AddChartBlockWithGetGraphName( const char *p_szIndicatorName, CString &p_strGraphName) 
{
	p_strGraphName.Empty();
	if( !m_pIndicatorList || !m_pMainBlock || !p_szIndicatorName) return FALSE;
	if( !*p_szIndicatorName) return FALSE;

	BOOL bOnSpecialChart = IsRunningOneChart();
	BOOL bAddSpecialChart = g_iMetaTable.IsSpecialGroupGraph( p_szIndicatorName);
	if( bAddSpecialChart)
	{
		// 0. Remove Current Special Chart (Not supported in Multiple Same-Name Indicator)
		if( m_pMainBlock->IsExistenceIndicator( p_szIndicatorName)) return FALSE;
		// 1. Add or Change Special Chart
		return ChangeOneChart( p_szIndicatorName, p_strGraphName);
	}

	// 3. Remove Current Sepcial Chart for General Chart Adding.
	if( bOnSpecialChart) RollbackLastNoOneChart();

	// 4. ������Ʈ�� style ����
	if( ChangeChart_PriceChartStyle( p_szIndicatorName, p_strGraphName)) return TRUE;

	//�Ϲ���Ʈ �߰�
	return AddChart_GenneralChart( p_szIndicatorName, p_strGraphName);
}

// (2004.05.20, ��¿�) ������ ��ǥ�� ��ϵǾ� �ִ��� Ȯ���Ͽ� �����Ѵ�.
// (2004.05.21, ��¿�) �˵���Ʈ/����Ʈ/����Ʈ�� Chart Type�� �����Ѵ�.
// (2007/1/22 - Seung-Won, Bae) Check Indicator Existing.
BOOL CKTBChartCtrl::HasIndicator(LPCTSTR p_szIndicatorName2) 
{
	// TODO: Add your dispatch handler code here
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	return HasIndicatorWithRQ( p_szIndicatorName2, m_strRQ);
}

// (2007/1/22 - Seung-Won, Bae) Check Indicator Existing.
BOOL CKTBChartCtrl::HasIndicatorInSpecialBack( LPCSTR p_szIndicatorName2) 
{
	// TODO: Add your dispatch handler code here

	// 0. �Է��� Ȯ���Ѵ�.
	if( !p_szIndicatorName2 || !*p_szIndicatorName2) return FALSE;

	CString p_szIndicatorName = _STOM2( p_szIndicatorName2);
	// 1. ���� Ư����Ʈ�� ��Ȳ������ Ȯ���Ѵ�.
	if( !IsRunningOneChart()) return FALSE;

	// 2. SaveChartCfg�� ������ �Ϲ���Ʈ ������ ���Ѵ�.
	// 2.1 Save�� CFG�� �ִ��� Ȯ���Ѵ�.
	int nCfgCount = m_SaveChartcfg.GetCfgCount();
	if( 0 == nCfgCount) return FALSE;
	// 2.2 ���������� ��ϵ� CFG�� ��ȸ�Ѵ�.
	CChartCfg *pChartCfg = m_SaveChartcfg.GetChartCfg( nCfgCount - 1);
	if( NULL == pChartCfg) return FALSE;
	CString strChartList = pChartCfg->GetChartList();

	// 3. �Ϲ� Chart �������� �� Graph�� �̸��� �˻��ϸ鼭 �ش� Graph�� �ִ��� Ȯ���Ѵ�.
	int nIndicatorNameLen = strlen( p_szIndicatorName);
	const char *szChartList = strChartList;
	szChartList = strstr( szChartList, "Graph;\r\n");
	while( szChartList)
	{
		szChartList += 8;	// Graph;rn
		szChartList = strchr( szChartList, ';') + 1;	// 0; (or ';')
		if( !strncmp( szChartList, p_szIndicatorName, nIndicatorNameLen)) return TRUE;

		szChartList = strstr( szChartList, "Graph;\r\n");
	}

	return FALSE;
}

// ���������� ���� PacketList�� �߰� �Ѵ� : �������� - ojtaso (20061224)
// RQ�� ����� ���� �����ڵ带 ����/����/�ֱ�/ƽ���� ���� : �������� - ojtaso (20070106)
// ƽ ī��Ʈ �߰� : �������� - ojtaso (20070329)
// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
BSTR CKTBChartCtrl::AddChartItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, BOOL bBuiltChart) 
{
	CString strResult(_T(""));

	//@Solomon-MultiLang:091117
	if(!m_pPacketListManager) return strResult.AllocSysString();

	// (2008/10/27 - Seung-Won, Bae) Do not for same item code TR.
	const char *szItemCode = NULL, *szItemName = NULL;
	m_pPacketListManager->GetItemCode( m_strRQ, szItemCode, szItemName);
	CString strItemCode( szItemCode);
	CString strNewItemCode( lpszItemCode);
	if( strItemCode != strNewItemCode) ResetUserCndForAutoCnd();

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( m_strRQ.Right( 5) == "_PAIR") m_strRQ = m_strRQ.Left( m_strRQ.GetLength() - 5);
	if(m_pPacketListManager)
	{
		if(bBuiltChart)	m_strRQ = m_pPacketListManager->AddChartItem(lpszItemCode, lpszItemName, GetSafeHwnd());
		else			m_pPacketListManager->SetItemCode(m_strRQ, lpszItemCode, lpszItemName);

		m_pPacketList = m_pPacketListManager->GetPacketList(m_strRQ);
		m_pMainBlock->SetPacketList( m_pPacketList, m_strRQ);

		if( bBuiltChart)
		{
			// ���ο� RQ �߰� : �������� - ojtaso (20070305)
			if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_PACKETLISTMANAGER_NEW_RQ, m_strRQ);

			// 2008.10.28 JS.Kim �����߰��ÿ� DEFAULT RQ�� ���õǾ� �ִ� �ð���� ���� <<
			CPacketList* pPacketList = m_pPacketListManager->GetPacketList("DEFAULT");
			if( pPacketList )
			{
				CString strTimeName = pPacketList->GetBaseTimeName();
				if( !strTimeName.IsEmpty() )
					m_pPacketList->SetBaseTime(strTimeName, pPacketList->GetBaseTimeDifference());
			}
			//>>
		}

		strResult = m_strRQ;
	}

	// 20090920 ������   �߼������忡�� ����ϴ� �����ڵ� ���� ==> �����ڵ� ���� �ȵǴ� ������ ���� �ӽ÷� �۾�
	SetItemInfo(lpszItemCode, lpszItemName);

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	m_pMainBlock->ChangeAllIndicatorInfoFromGraphData();

	return strResult.AllocSysString();
}

// �׷���/Ÿ��Ʋ ���� �� RQ�˷��� : �������� - ojtaso (20070106)
void CKTBChartCtrl::SelectedGraph(LPCTSTR lpszRQ, BOOL bSendEvent/* = TRUE*/)
{
	if( m_strRQ.Compare( lpszRQ))
	{
		m_strRQ = lpszRQ;

		m_pPacketList = m_pPacketListManager->GetPacketList( m_strRQ);
		m_pMainBlock->SetPacketList(m_pPacketList, m_strRQ);

		// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��.	�ٸ� ������ ���õǸ� �ش� ���� Scale�� ����
		if( m_pxScaleManager) m_pxScaleManager->Calc_DRI();
		
		// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
		//��Ʈ �ʱ�ȭ���� ���񺯰� ���� ���� : �������� - ojtaso (20070621)
		if(bSendEvent && !m_bResetChart)
			SendEventMsgToDLL(UWM_SELECTEDCHART, 1, ( LPARAM)lpszRQ);

		// ���õ� RQ : �������� - ojtaso (20070305)
		if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_PACKETLISTMANAGER_CHANGE_RQ, m_strRQ);
	}
}

// RQ ���� : �������� - ojtaso (20070305)
void CKTBChartCtrl::DeletedGraph(LPCTSTR lpszRQ, BOOL bRemovablePacketRQ)
{
	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	if(bRemovablePacketRQ)
		SendEventMsgToDLL(UWM_SELECTEDCHART, 0, (LPARAM)lpszRQ);

	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_PACKETLISTMANAGER_DELETE_RQ, lpszRQ);
}

// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
//		It can not be replaced with AddChartBlock(), because of Condition Reset.
void CKTBChartCtrl::ShowIndicator(LPCTSTR p_szIndicatorName2, BOOL p_bShow, BOOL p_bRecalculate) 
{
	// TODO: Add your dispatch handler code here
	if( m_pMainBlock)
	{
		CString p_szIndicatorName = _STOM2( p_szIndicatorName2);
		m_pMainBlock->ShowIndicator( p_szIndicatorName, p_bShow, p_bRecalculate);
		InvalidateControl();
	}
}

// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
void CKTBChartCtrl::SetPacketAutoScaleType(LPCTSTR p_szPacketName2, BOOL p_bAutoScaleType) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pPacketList || !p_szPacketName2) return;
	if( !*p_szPacketName2) return;

	CString p_szPacketName = _STOM0( p_szPacketName2);
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return;
	pPacket->SetAutoScaleType( p_bAutoScaleType);
}

// (2007/2/21 - Seung-Won, Bae) Draw in MemDC for Print or Save
LRESULT CKTBChartCtrl::OnDrawForPrint( WPARAM wParam, LPARAM lParam)
{
	CDC *pDC = CDC::FromHandle( ( HDC)lParam);
	if( !pDC) return -1;

	CRect rctClient;
	GetClientRect( &rctClient);
	CBitmap bufferBitmap;
	OnDrawForPrint( *pDC, bufferBitmap, rctClient, TRUE);

	return 0;
}

// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type with Packet Type.
BOOL CKTBChartCtrl::SetPacketTypeWithAutoScale(LPCTSTR p_szPacketName2, LPCTSTR p_szPacketType2, BOOL p_bAutoScale) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pPacketList || !p_szPacketName2) return FALSE;
	if( !*p_szPacketName2) return FALSE;

	CString p_szPacketName = _STOM0( p_szPacketName2);
	CString p_szPacketType = _STOM6( p_szPacketType2);
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return FALSE;
	CString strPacketType( p_szPacketType);
	pPacket->SetType( strPacketType);
	pPacket->SetAutoScaleType( p_bAutoScale);
	return TRUE;
}

// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
void CKTBChartCtrl::ShowBaseText(BOOL p_bShow) 
{
	// TODO: Add your dispatch handler code here
	if( m_pMainBlock) m_pMainBlock->ShowBaseText( p_bShow);
}

// (2007/2/22 - Seung-Won, Bae) Support Graph Color Changing with Graph Title
BOOL CKTBChartCtrl::ChangeGraphColor(LPCTSTR p_szGraphTitle2, LPCTSTR p_szColors) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock) return FALSE;
	CString p_szGraphTitle = _STOM1( p_szGraphTitle2);
	return m_pMainBlock->ChangeGraphColor( p_szGraphTitle, p_szColors);
}

// (2007/2/22 - Seung-Won, Bae) User Zoom Changed.
void CKTBChartCtrl::OnUserZoomChanged( const int p_nDataCountInView)
{
	// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
	m_pxScaleManager->OnUserZoomChanged();

	m_OutsideCommonInterfaceFunction.OnUserZoomChanged( p_nDataCountInView);
}

// (2007/1/25 - Seung-Won, Bae) Lock Window Update.
BOOL CKTBChartCtrl::LockWindowUpdate() 
{
	// TODO: Add your dispatch handler code here
	m_bLockWindowUpdate = TRUE;
	return TRUE;
}
void CKTBChartCtrl::UnlockWindowUpdate() 
{
	// TODO: Add your dispatch handler code here
	m_bLockWindowUpdate = FALSE;
}
// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
void CKTBChartCtrl::InvalidateControl( LPCRECT lpRect, BOOL bErase)
{
	if( !m_bLockWindowUpdate) COleControl::InvalidateControl( lpRect);
}
void CKTBChartCtrl::InvalidateRgn( CRgn *pRgn, BOOL bErase)
{
	if( !m_bLockWindowUpdate) COleControl::InvalidateRgn( pRgn, bErase);
}

// VertScaleGroup�� ���� ó�� Graph�� Ÿ�� : �������� - ojtaso (20070504)
CScaleBaseData::HORZSCALEDRAWERTYPE CKTBChartCtrl::GetMasterValueType(const int nRow, const int nCol)
{
	if(!m_pMainBlock)
		return CScaleBaseData::HORZSCALEDRAWERTYPE::HORZ_NOTYPE;

	return m_pMainBlock->GetMasterValueType(nRow, nCol);
}

// VertScaleGroup�� ���� ó�� Graph�� Ÿ�� : �������� - ojtaso (20070504)
LPCTSTR CKTBChartCtrl::GetMasterValueRQ(const int nRow, const int nCol)
{
	if(!m_pMainBlock)
		return _T("DEFAULT");

	return m_pMainBlock->GetMasterValueRQ(nRow, nCol);
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CKTBChartCtrl::AddPacketRQ(LPCTSTR lpszRQ, LPCTSTR lpszPacketOption) 
{
	if(!m_pPacketListManager)
	{
		AddPacket(lpszPacketOption);
		return;
	}

	CPacketList* lpPacketList = m_pPacketListManager->GetPacketList(lpszRQ);
	if(!lpPacketList)
	{
		AddPacket(lpszPacketOption);
		return;
	}

	lpPacketList->AddPacket(lpszPacketOption);
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
BOOL CKTBChartCtrl::SetPacketDataRQ(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, long nData, long nLength, BOOL bReset) 
{
	//g_DRDebuger ��Ŷ�α״� CPacketListImplementation::SetPacketData(..)���� �ϰ������� ó����.

	// 0. ���� Pointer�� Ȯ���Ѵ�.
	if( !lpszPacketName || !m_pPacketListManager) return FALSE;
	if( !*lpszPacketName) return FALSE;

	m_pPacketList = m_pPacketListManager->GetPacketList(lpszRQ);
	if(!m_pPacketList)
		return FALSE;
	
	m_strRQ = lpszRQ;
	m_pMainBlock->SetPacketList(m_pPacketList, lpszRQ);

	int nDataCount = 0;
	// 2.1.2 Scroll Index�� ���Ѵ�.
	int nStartIndex = 0, nEndIndex = 0;
	if(!bReset)
	{
		nDataCount = m_pPacketList->GetMaxCount();
		GetDataStartEnd( nStartIndex, nEndIndex);
	}

	CString szPacketName = _STOMS( lpszPacketName, ";", 0);
	m_pPacketList->SetPacketData( szPacketName, nData, nLength);

	// (2004.08.12, ��¿�) �м������� ��� Clear�Ѵ�.
	BOOL bPriceChart = ( BOOL)strstr( szPacketName, CString( _MTEXT( C0_OPEN)) + ";" + _MTEXT( C0_HIGH) + ";" + _MTEXT( C0_LOW) + ";" + _MTEXT( C0_CLOSE) + ";") && bReset;
	if( bPriceChart) RunToolFunction(CToolOptionInfo::T_DELETE_ALL, 3);

	// (2004.10.13, ��¿�) TR�� ������ AddIn DLL�鿡�� �˷� ������� �Ѵ�.
	// RQ�� Packet : �������� - ojtaso (20070111)
	if( m_pAddInManager) m_pAddInManager->OnPacketDataMultiItem(lpszRQ, szPacketName, bReset);

	//sy 2003.3.7 - �ӵ� ���� - ��ũ������ ������ string data ��� (ex- news)
	if(m_pMainBlock != NULL)
		m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::TRDATA_CAL);

	if( bReset) ResetScroll( TRUE);
	else if( m_pxScaleManager)
	{
		int nPrevXRange = m_pxScaleManager->GetCount();
		m_pxScaleManager->Calc_DRI();
		int nNewXRange = m_pxScaleManager->GetCount();

		ScrollTo( nNewXRange - nPrevXRange, TRUE);
	}

	InvalidateControl();

	return TRUE;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
BOOL CKTBChartCtrl::AddChartItemRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName) 
{
	if(m_pPacketListManager)
	{
		m_strRQ = lpszRQ;

		if(!m_pPacketListManager->AddChartItemRQ(lpszRQ, lpszItemCode, lpszItemName, GetSafeHwnd()))
			return FALSE;
//		m_pPacketList = m_pPacketListManager->GetPacketList(lpszRQ);
//		m_pPacketListManager->SetCurrentRQ(lpszRQ);
		
//		m_pMainBlock->SetPacketList(m_pPacketList);

		// ���ο� RQ �߰� : �������� - ojtaso (20070305)
		if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_PACKETLISTMANAGER_NEW_RQ, lpszRQ);
	}

	// 20090920 ������   �߼������忡�� ����ϴ� �����ڵ� ���� ==> �����ڵ� ���� �ȵǴ� ������ ���� �ӽ÷� �۾�
	SetItemInfo(lpszItemCode, lpszItemName);

	return TRUE;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CKTBChartCtrl::SetCurrentRQ(LPCTSTR lpszRQ) 
{
	SelectedGraph(lpszRQ, FALSE);
	InvalidateControl();
}

void CKTBChartCtrl::NotUsedMethod8(BOOL bReset) 
{
}

// PacketRQ�� ���¸� �˸����� �������̽� ���� : �������� - ojtaso (20070531)
void CKTBChartCtrl::NotifyRQStatusToAddin( CCommonInterfaceEnum::PACKET_RQ_STATUS eStatus, LPCTSTR lpszRQ)
{
	if( m_pAddInManager) 
	{
		EAICommandType eCommandID = EAI_PACKETLISTMANAGER_NEW_RQ;
		if(eStatus == CCommonInterfaceEnum::PACKETLISTMANAGER_CHANGE_RQ)
			eCommandID = EAI_PACKETLISTMANAGER_CHANGE_RQ;
		else if(eStatus == CCommonInterfaceEnum::PACKETLISTMANAGER_DELETE_RQ)
			eCommandID = EAI_PACKETLISTMANAGER_DELETE_RQ;

		m_pAddInManager->OnAddInToolCommand( eCommandID, lpszRQ);
	}
}

void CKTBChartCtrl::SetFutureScaleVisible(long nScaleVisible) 
{
	if(m_pMainBlock)
		m_pMainBlock->SetDrawFutureScale(nScaleVisible);
}

// XScale ���� ���� - ojtaso (20080429)
void CKTBChartCtrl::SetHorizontalScaleHeight(short nBlockRow, short nBlockCol, long nHeight) 
{
	if( m_pMainBlock) m_pMainBlock->SetHorizontalScaleHeight( nBlockRow, nBlockCol, nHeight);
}

// ȣ������, ���ذ� ��� - ojtaso (20070917)
void CKTBChartCtrl::SetVertScaleType(short nBlockRow, short nBlockCol, short nScaleGroup, short nScaleType, double dTypeValue) 
{
	if(m_pMainBlock)
	{
		m_pMainBlock->SetVertScaleType(CBlockIndex(nBlockRow, nBlockCol), nScaleGroup, nScaleType, dTypeValue);
		InvalidateControl();
	}
}

// �ѹ��� �׷��� �Ӽ� ���� ������ �޽�� �߰� - ojtaso (20080602)
void CKTBChartCtrl::ChangeGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, LPCTSTR lpszNewTitle, short nGraphType, short nGraphStyle, short nDrawStyle, LPCTSTR lpszColors, short nThickness1, short nThickness2, short nThickness3) 
{
	if(m_pMainBlock)
	{
		//<<20100324_JS.Kim �ٱ������� ó��
		//m_pMainBlock->ChangeGraphPropertyAtOnce(lpszRQ, lpszTargetTitle, lpszNewTitle, nGraphType, nGraphStyle, nDrawStyle, lpszColors, nThickness1, nThickness2, nThickness3);
		CString strTitle2 = _STOM1( lpszTargetTitle);
		m_pMainBlock->ChangeGraphPropertyAtOnce(lpszRQ, strTitle2, lpszNewTitle, nGraphType, nGraphStyle, nDrawStyle, lpszColors, nThickness1, nThickness2, nThickness3);
		//>>
	}
}

// �ѹ��� �׷��� �Ӽ� ���� �� �޽�� �߰� - (@Solomon-20100120)
BSTR CKTBChartCtrl::GetGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle) 
{
	CString strResult;
	if(m_pMainBlock)
	{
		short	nGraphType,
				nGraphStyle,
				nDrawStyle;
		COLORREF crColors;
		short	nThickness1, 
				nThickness2, 
				nThickness3;
		short nRet = m_pMainBlock->GetGraphPropertyAtOnce(lpszRQ, lpszTargetTitle, nGraphType, nGraphStyle, nDrawStyle, crColors, nThickness1, nThickness2, nThickness3);

		strResult.Format("%d\t%d\t%d\t%03d%03d%03d\t%d\t%d\t%d\t",
			nGraphType, nGraphStyle, nDrawStyle,
			GetRValue(crColors), GetGValue(crColors), GetBValue(crColors),
			nThickness1, nThickness2, nThickness3);
	}

	return strResult.AllocSysString();
}

// RQ ���� �׷��� ���� �޼ҵ� �߰� - ojtaso (20080604)
BOOL CKTBChartCtrl::RemoveGraphRQ(LPCTSTR lpszRQ, LPCTSTR lpszTitle) 
{
	if(m_pMainBlock == NULL)
		return FALSE;

	if(!m_pMainBlock->DeleteGraphWithSubGraphTitleRQ(lpszRQ, CBlockIndex(-1, -1), lpszTitle))
		return FALSE;

	Invalidate();
	return TRUE;
}

// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
CString	CKTBChartCtrl::GetUseSTDataName()
{
	return m_strSTExtPacketName;
}

// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
void CKTBChartCtrl::SetUseSTExtData(LPCTSTR lpszExtDataName) 
{
	m_strSTExtPacketName = lpszExtDataName;
}

// GraphDataDraw ���� �߰� - ojtaso (20080609)
BOOL CKTBChartCtrl::ShowGraphDataDrawingData(LPCTSTR lpszRQ, LPCTSTR lpszIndicatorName, BOOL bShow, short nShowType) 
{
	if( !m_pMainBlock) return FALSE;

	// 1. Graph Drawing Data�� �缳���Ѵ�.
	if( m_pMainBlock->ShowGraphDataDrawingData(lpszRQ, lpszIndicatorName, bShow, nShowType))
	{
		// 2. �������� �缳���ÿ��� �ٽ� Drawing �ǵ��� �Ѵ�.
		InvalidateControl();
		return TRUE;
	}

	return FALSE;
}

// ȯ�漳�� �Ǻ� ���� ���� - ojtaso (20080109)
void CKTBChartCtrl::ShowSelEnvironmentDlg(short nPage) 
{
	RunToolFunction( CToolOptionInfo::T_SELECT_ENV_TAB_INDEX, nPage);
}

void CKTBChartCtrl::OnNLanguageIDChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
BOOL CKTBChartCtrl::GetTestEnvValue( const char *p_szEnvKey, CString &p_strEnvValue)
{
	p_strEnvValue.Empty();
	return m_mapTestEnvSetting.Lookup( p_szEnvKey, p_strEnvValue);
}

// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
void CKTBChartCtrl::SetPageInfo(LPCTSTR p_szPagePath, BOOL p_bIsVirtualPage, short p_nVirtualPageIndex, short p_nDuplicateIndex) 
{
	// TODO: Add your dispatch handler code here
	m_strPagePathWithPrefix = p_szPagePath;
	m_strPagePathWithPrefix += "\\C";
	if( p_bIsVirtualPage)
	{
		CString strPageIndex;
		strPageIndex.Format( "P%d", p_nVirtualPageIndex);
		m_strPagePathWithPrefix += strPageIndex;
	}
	CString strDupIndex;
	strDupIndex.Format( "I%d!_", p_nDuplicateIndex);
	m_strPagePathWithPrefix += strDupIndex;
}
void CKTBChartCtrl::OpenFromPage(LPCTSTR p_szPagePath, BOOL p_bIsVirtualPage, short p_nVirtualPageIndex, short p_nDuplicateIndex) 
{
	// TODO: Add your dispatch handler code here
	SetPageInfo( p_szPagePath, p_bIsVirtualPage, p_nVirtualPageIndex, p_nDuplicateIndex);

	// (2008/6/9 - Seung-Won, Bae) Check saved file. if not found, Use not-paged file.
	CheckPagedFile( m_strPagePathWithPrefix, m_strUserEnvironmentFileName);
	CheckPagedFile( m_strPagePathWithPrefix, m_strUserIndicatorListFileName);

	ReadUserFiles();
	m_bFreezedPageState = FALSE;	// (2008/6/9 - Seung-Won, Bae) Set default as False for ENV.
}
void CKTBChartCtrl::SaveToPage(LPCTSTR p_szPagePath, BOOL p_bIsVirtualPage, short p_nVirtualPageIndex, short p_nDuplicateIndex) 
{
	// TODO: Add your dispatch handler code here
	SetPageInfo( p_szPagePath, p_bIsVirtualPage, p_nVirtualPageIndex, p_nDuplicateIndex);

	m_bFreezedPageState = FALSE;
	WriteUserFiles();
	m_bFreezedPageState = FALSE;	// (2008/6/9 - Seung-Won, Bae) Set default as False for ENV.
}
void CKTBChartCtrl::OnBFreezedPageStateChanged() 
{
	// TODO: Add notification handler code
	SetModifiedFlag();
}
void CKTBChartCtrl::OnStrPagePathWithPrefixChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

// (2008/6/9 - Seung-Won, Bae) Check saved file. if not found, Use not-paged file.
void CKTBChartCtrl::CheckPagedFile( const char *p_szPagePath, const char *p_szFileName)
{
	CString strPagedFile( p_szPagePath);
	strPagedFile += p_szFileName;

	WIN32_FIND_DATA wfd;
	HANDLE hFound = FindFirstFile( strPagedFile, &wfd);
	if( INVALID_HANDLE_VALUE == hFound)
		CopyFile( m_strUserFilePath + p_szFileName, strPagedFile, FALSE);
	else FindClose( hFound);
}

// �м��� ���� ���� - ojtaso (20080728)
BOOL CKTBChartCtrl::SaveandOpenFromAnalToolFile(LPCTSTR lpszOpenFileName, LPCTSTR lpszSaveFileName, BOOL bOpenOption, BOOL bSaveOption) 
{
	CString strOpenFN = lpszOpenFileName, strSaveFN = lpszSaveFileName;

	if( bSaveOption && !strSaveFN.IsEmpty())
	{
		SetUserEnvironmentAndIndicatorListFileName(strSaveFN);

		WriteUserFile(CSaveUserFile::ANALTOOL);

/*
#if _SAVEANDOPEN_LOG
		CopyFile( GetUserFilePathAndName( m_strUserEnvironmentFileName), "C:\\Logs\\" + strSaveFileCopy, FALSE);
#endif
*/
	}
	// open
	if(bOpenOption && !strOpenFN.IsEmpty())
	{
/*
#if _SAVEANDOPEN_LOG
		CopyFile( GetUserFilePathAndName( m_strUserEnvironmentFileName), "C:\\Logs\\" + strOpenFileCopy, FALSE);
#endif
*/
		SetUserEnvironmentAndIndicatorListFileName( strOpenFN);

		if(ReadUserFile(CSaveUserFile::ANALTOOL))
			InvalidateControl();
	}

	return TRUE;
}

// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI
LRESULT CKTBChartCtrl::OnReCalcDRI( WPARAM wParam, LPARAM lParam)
{
	ResetScroll( TRUE);
	return 0;
}

// (2009/5/13 - Seung-Won, Bae) for Right Margin
void CKTBChartCtrl::SetRightMarginWidth( int nRightMarginWidth)
{
	m_nRightMarginWidth = nRightMarginWidth;
	if( m_pxScaleManager) m_pxScaleManager->SetRightMarginWidth( m_nRightMarginWidth);
}

// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
void CKTBChartCtrl::LockScrollUIUpdateWithScrollInfo( BOOL p_bLockScrollUIUpdate)
{
	if( IsRunningOneChart())
	{
		m_bLockScrollUIUpdate = p_bLockScrollUIUpdate;
		return;
	}

	if( p_bLockScrollUIUpdate)
	{
		// 0. Lock UI
		m_bLockScrollUIUpdate = p_bLockScrollUIUpdate;

		// 1. Backup Adjusted State
		int nStartDrawIdx = 0, nEndDrawIdx = 0;
		m_pxScaleManager->GetStartEndDrawIdx_Cur( nStartDrawIdx, nEndDrawIdx);

		// 2. Release the mode of adjusting.
		m_bWholeViewForLock = m_bWholeView;
		if( m_bWholeViewForLock) SetWholeView( FALSE);
		m_bIsFixedIntervalForLock = m_pxScaleManager->IsFixedInterval();
		if( m_bIsFixedIntervalForLock) m_pxScaleManager->SetFixedInterval( FALSE, TRUE);
		m_eCandleTypeOnResizeForLock = m_pxScaleManager->GetCandleTypeOnResize();
		if( m_eCandleTypeOnResizeForLock != CXScaleManager::RSAT_ITEM_COUNT_FIX)
			m_pxScaleManager->SetCandleTypeOnResize( CXScaleManager::RSAT_ITEM_COUNT_FIX);

		// 3. Backup Setted State
		m_nOnePageDataCountForLock = m_nOnePageDataCount;
		m_nStartDrawIdxCurForLock = m_pxScaleManager->GetStartDrawIdx_Cur();

		// 4. Fix to the Adjusted State
		m_nOnePageDataCount = nEndDrawIdx - nStartDrawIdx + 1;
		m_pxScaleManager->SetStartEndDrawIdx_Cur( nStartDrawIdx, nEndDrawIdx);
	}
	else
	{
		// 1. Recalculate Region Info for CXScaleManagerImp::m_nScreenWidth_Cur
		CDC *pDC = GetDC();
		m_pMainBlock->CalculateDataForDrawing( pDC);
		ReleaseDC( pDC);

		// 2. Restore Zoom and Scroll Info
		ZoomTo( m_nOnePageDataCountForLock, FALSE);
		ScrollTo( m_nStartDrawIdxCurForLock, FALSE);
		if( CXScaleManager::RSAT_ITEM_COUNT_FIX != m_eCandleTypeOnResizeForLock)
			m_pxScaleManager->SetCandleTypeOnResize( m_eCandleTypeOnResizeForLock);
		if( m_bIsFixedIntervalForLock) m_pxScaleManager->SetFixedInterval( m_bIsFixedIntervalForLock, TRUE);
		if( m_bWholeViewForLock) SetWholeView( m_bWholeViewForLock);

		// 3. Unlock UI
		m_bLockScrollUIUpdate = p_bLockScrollUIUpdate;
	}
}
CRect CKTBChartCtrl::GetAllBlockRegion( void)
{
	return m_pMainBlock->GetAllBlockRegion();
}

// (2009/5/17 - Seung-Won, Bae) for Future Time
void CKTBChartCtrl::SetFutureTimeCount( int nFutureTimeCount)
{
	m_nFutureTimeCount = nFutureTimeCount;
	m_pxScaleManager->SetFutureTimeCount( m_nFutureTimeCount);
	ResetScroll( TRUE);
}

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
BOOL CKTBChartCtrl::ShiftPacket( LPCTSTR p_szPacketNames, long p_szShiftCount) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pPacketList) return FALSE;
	return m_pPacketList->ShiftPacket( p_szPacketNames, p_szShiftCount);
}

// (2009/6/5 - Seung-Won, Bae) for CmdUI of AddIn
void CKTBChartCtrl::OnCmdMsg( HMENU p_hMenu, UINT p_nItemID, int p_nItemPos, int p_nItemCount, int p_nCmdCode)
{
	CCmdUI cmdUI; 
	cmdUI.m_nID			= p_nItemID;
	cmdUI.m_nIndex		= p_nItemPos;
	cmdUI.m_pMenu		= CMenu::FromHandle( p_hMenu);
	cmdUI.m_pSubMenu	= NULL;
	cmdUI.m_pOther		= NULL;
	cmdUI.m_nIndexMax	= p_nItemCount;
	cmdUI.Enable( FALSE);
	OnCmdMsg( cmdUI.m_nID, p_nCmdCode, &cmdUI, NULL);
}

void CKTBChartCtrl::OnNRealTimeTruncationTypeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

// CL������ ��Ʈ �� ������ - ojtaso (20080430)
BOOL CKTBChartCtrl::EmptyChart3(LPCTSTR lpszChartList, BOOL bIsDeleteAllPacket, BOOL bIsDefault, short nBlockCount) 
{
	// TODO: Add your dispatch handler code here

	if(m_pPacketList == NULL || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	RunToolFunction(CToolOptionInfo::T_DELETE_ALL, 1);

	if(nBlockCount <= 0)
	{
		CString strChartList = lpszChartList;
		int nPos = strChartList.Find("BlockEnd", 0);
		m_nBlockRowCount  = 0;
		while(nPos >= 0)
		{
			m_nBlockRowCount++;
			nPos = strChartList.Find("BlockEnd", nPos + 8);
		}
		
		nPos = strChartList.GetLength();
		
		if(m_nBlockRowCount == 0)
			m_nBlockRowCount = 1;
	}
	else
		m_nBlockRowCount = nBlockCount;

	// (2006/10/27 - Seung-Won, Bae) Init with One Empty Block.
	m_nBlockColumnCount = 1;
	m_strChartList = lpszChartList;
	m_strBlockRegion = "";

	//����
	SetMainBlock_MainBlock(true);

	// ��� Packet Data�� Clear
	m_pPacketList->ClearDataAll(bIsDeleteAllPacket);
	m_pIndicatorList->RemoveAllIndicatorInfo( EILT_USER);
	m_pMainBlock->Initialize_IndicatorInfo( FALSE);

	// ��ũ�� ���� : data ������ Ʋ�����⶧��
	ResetScroll(TRUE);

	// ������¸� default�� ����
	if(bIsDefault) ResetChartCfgWith(0);


	return TRUE;
}

void CKTBChartCtrl::OnBUseGapRevisionChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

// (2008/11/3 - Seung-Won, Bae) Support m_nFullDataCount to MultiChart.
long CKTBChartCtrl::GetNFullDataCount() 
{
	// TODO: Add your property handler here
	if( !m_pxScaleManager) return 0;
	return m_pxScaleManager->GetCount();
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
BSTR CKTBChartCtrl::AddPairItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, BOOL bBuiltChart) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	if( m_strRQ.Right( 5) != "_PAIR") m_strRQ = m_strRQ + "_PAIR";
	if(m_pPacketListManager)
	{
		if(bBuiltChart) m_pPacketListManager->AddPairItem( m_strRQ, lpszItemCode, lpszItemName, GetSafeHwnd());
		else			m_pPacketListManager->SetItemCode(m_strRQ, lpszItemCode, lpszItemName);

		m_pPacketList = m_pPacketListManager->GetPacketList(m_strRQ);
		m_pMainBlock->SetPacketList( m_pPacketList, m_strRQ);

		if(bBuiltChart)
		{
			// ���ο� RQ �߰� : �������� - ojtaso (20070305)
			if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_PACKETLISTMANAGER_NEW_RQ, m_strRQ);

			// 2008.10.28 JS.Kim �����߰��ÿ� DEFAULT RQ�� ���õǾ� �ִ� �ð���� ���� <<
			CPacketList* pPacketList = m_pPacketListManager->GetPacketList("DEFAULT");
			if( pPacketList )
			{
				CString strTimeName = pPacketList->GetBaseTimeName();
				if( !strTimeName.IsEmpty() )
					m_pPacketList->SetBaseTime(strTimeName, pPacketList->GetBaseTimeDifference());
			}
			//>>
		}

		strResult = m_strRQ;
	}

	// 20090920 ������   �߼������忡�� ����ϴ� �����ڵ� ���� ==> �����ڵ� ���� �ȵǴ� ������ ���� �ӽ÷� �۾�
	SetItemInfo(lpszItemCode, lpszItemName);

	return strResult.AllocSysString();
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
BOOL CKTBChartCtrl::HasIndicatorWithRQ(LPCTSTR p_szIndicatorName2, LPCTSTR p_szRQ) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock || !p_szIndicatorName2 || !*p_szIndicatorName2) return FALSE;

	CString p_szIndicatorName = _STOM2( p_szIndicatorName2);
	// RQ�� �˻� : �������� - ojtaso (20070712)
	LPVOID lpPacketRQ = m_pPacketListManager->GetPacketRQ( p_szRQ);
	if(!lpPacketRQ)
		return FALSE;

	// (2004.05.21, ��¿�) �˵���Ʈ/����Ʈ/����Ʈ�� Chart Type�� �����Ѵ�.
	// 1. Chart Type�� Ȯ���ϴ� ��츦 ó���Ѵ�.
	// 1.1 ������Ʈ �з��� Ȯ���Ѵ�.
	if( g_iMetaTable.IsPriceChartType( p_szIndicatorName))
	{
		// 1.1.1 ���� ������Ʈ�� Ȯ���Ѵ�.
		// RQ�� �˻� : �������� - ojtaso (20070712)
		BOOL bHasPriceChart = m_pMainBlock->IsExistenceIndicator( lpPacketRQ, _MTEXT( C2_PRICE_CHART));
		if( !bHasPriceChart) return FALSE;
		// 1.1.2 ������Ʈ�� Graph Style�� Ȯ���Ѵ�.
		//	(����! ������Ʈ�� ������ Graph Type�� ������ �ƴ�, ������Ʈ�� ĵ����Ʈ Style�̴�.)
		CGraphStyle					graphStyle;
		CGraphDrawStyle				graphDrawStyle;
		CGraphBaseData::GRAPHTYPE	eGraphType;
		m_pMainBlock->GetGraphType( _MTEXT( C2_PRICE_CHART), 0, eGraphType, graphStyle, graphDrawStyle);
		switch( graphDrawStyle.GetGraphBongDrawStyle())
		{
			case CGraphBaseData::CandlestickBong:		return !stricmp( p_szIndicatorName, _MTEXT( C5_CANDLE_CHART));
			case CGraphBaseData::OpenHighLowCloseBong:	return !stricmp( p_szIndicatorName, _MTEXT( C5_BAR_CHART)) || !stricmp( p_szIndicatorName, _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE));
			case CGraphBaseData::AntennaBong:			return !stricmp( p_szIndicatorName, _MTEXT( C5_ANTENNA_CHART));
			case CGraphBaseData::LineBong:				return !stricmp( p_szIndicatorName, _MTEXT( C5_LINE_CHART));
			case CGraphBaseData::NewHighLowCloseBong:	return !stricmp( p_szIndicatorName, _MTEXT( C5_BAR_CHART_HIGH_LOW_CLOSE));
			case CGraphBaseData::FlowBong:				return !stricmp( p_szIndicatorName, _MTEXT( C5_FLOW_CHART));
			case CGraphBaseData::CandleVolume:			return !stricmp( p_szIndicatorName, _MTEXT( C5_CANDLE_VOLUME_CHART));
			case CGraphBaseData::EquiVolume:			return !stricmp( p_szIndicatorName, _MTEXT( C5_EQUI_VOLUME_CHART));
		}	
		return FALSE;
	}
	// 1.2 ���� Ư�� ��Ʈ�� Ȯ���Ѵ�.
	//		(�Ϲ� ��ǥó�� Ȯ���Ѵ�.)
	// 2. �Ϲ� ��ǥ�� Ȯ���Ѵ�.
	// RQ�� �˻� : �������� - ojtaso (20070712)
	return m_pMainBlock->IsExistenceIndicator( lpPacketRQ, p_szIndicatorName);
}

void CKTBChartCtrl::RemoveAllGraphOfRQ(LPCTSTR p_szRQ) 
{
	// TODO: Add your dispatch handler code here
	if( m_pMainBlock && p_szRQ)
		if( *p_szRQ)
		{
			LPVOID lpPacketRQ = m_pPacketListManager->GetPacketRQ( p_szRQ);
			while( lpPacketRQ)
			{
				if( !m_pMainBlock->RemoveAllGraphOfRQ( lpPacketRQ)) break;
				lpPacketRQ = m_pPacketListManager->GetPacketRQ( p_szRQ);
			}
		}
}

short CKTBChartCtrl::GetRowOfGraphWithRQ(LPCTSTR p_szIndicatorName2, LPCTSTR p_szRQ) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock || !p_szIndicatorName2 || !*p_szIndicatorName2) return FALSE;

	CString p_szIndicatorName = _STOM2( p_szIndicatorName2);
	int p_nR = -1, p_nC = -1, p_nV = -1, p_nG = -1;
	CString strGraphName;
	if( !m_pMainBlock->FindFirstIndicator( p_szIndicatorName, p_nR, p_nC, p_nV, p_nG, strGraphName, p_szRQ)) return -1;
	return p_nR;
}

BOOL CKTBChartCtrl::SetGraphColorWithRQ(LPCTSTR p_szRQ, LPCTSTR p_szIndicatorName2, LPCTSTR p_szColor) 
{
	// TODO: Add your dispatch handler code here
	if(m_pMainBlock == NULL) return FALSE;
	CString p_szIndicatorName = _STOM2( p_szIndicatorName2);
	int p_nR = -1, p_nC = -1, p_nV = -1, p_nG = -1;
	CString strGraphName;
	if( !m_pMainBlock->FindFirstIndicator( p_szIndicatorName, p_nR, p_nC, p_nV, p_nG, strGraphName, p_szRQ)) return FALSE;
	if( !m_pMainBlock->ChangeGraphColor1( CBlockIndex( p_nR, p_nC), 
		p_nV, p_nG, 0, CDataConversion::Str2RGB( p_szColor))) return FALSE;

	InvalidateControl();
	return TRUE;
}

// ���� VertScaleGroupCount�� ���� - ojtaso (20080618)
long CKTBChartCtrl::GetVertScaleGroupCount(short nRow, short nColumn) 
{
	if(m_pMainBlock)
		return m_pMainBlock->GetVertScaleGroupCount(CBlockIndex(nRow, nColumn));

	return -1;
}

// ��Ŷ ����Ÿ�� ���� ��� ó�� - ojtaso (20080618)
void CKTBChartCtrl::SetSyncPacketData(LPCTSTR strSyncPacketNames) 
{
	m_bSyncPacket = TRUE;
	m_strSyncPacketNames = strSyncPacketNames;
}

// ����ִ� ��Ʈ - ojtaso (20080625)
void CKTBChartCtrl::SetEmptyChart() 
{
	if(m_pPacketList == NULL || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return;

	RunToolFunction(CToolOptionInfo::T_DELETE_ALL, 1);

	// ��� Packet Data�� Clear
	m_pPacketList->ClearDataAll( TRUE);
	m_pIndicatorList->RemoveAllIndicatorInfo( EILT_USER);
	m_pMainBlock->Initialize_IndicatorInfo( FALSE);

	// ��ũ�� ���� : data ������ Ʋ�����⶧��
	ResetScroll( TRUE);

	// ������¸� default�� ����
	ResetChartCfgWith(0);
	ResetChart();
}

// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
void CKTBChartCtrl::SetDrawVerticalBox(double dHorzValue, LPCTSTR strColor, BOOL bFill) 
{
	if(!m_pxScaleManager)
		return;

	COLORREF clrColor = CDataConversion::Str2RGB(strColor);
	m_pxScaleManager->AddVerticalBoxInfo(m_strRQ, dHorzValue, clrColor, bFill);
}

// �������� TR���� ��Ŷ�� ó�� Real ó�� ���� - ojtaso (20080804)
void CKTBChartCtrl::UseDummyPacketData(BOOL bUse) 
{
	// ������� ����
	// ���� �� ������ ȣȯ�� ���� ���ܵ�
}

void CKTBChartCtrl::SetDoubleSizeBlock(short nRowIndex, short nColIndex) 
{
	m_nRowDoubleSizeBlock = nRowIndex;
	m_nColumnDoubleSizeBlock = nColIndex;

	if(m_pMainBlock)
		m_pMainBlock->SetDoubleSizeBlockNumber(CBlockIndex(nRowIndex, nColIndex));
}


// (2008/10/21 - JS, Kim) User Changes Time Difference
LRESULT CKTBChartCtrl::OnChangeTimeDiff( WPARAM wParam, LPARAM lParam)
{
	switch( lParam )
	{
	case 0 :		// Time Set
		{
		long nIndex = (long)wParam;
		long lResult = 0;
		CTimeDiffInfo *pTimeInfo = NULL;

		if( nIndex > 0 )
		{
			if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_USER_CHANGE_TIMEDIFF, nIndex, (long)0);
			pTimeInfo = (CTimeDiffInfo *)lResult;
		}

		if( pTimeInfo )
		{
			if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_USER_CHANGE_TIMEDIFF, (const char *)pTimeInfo->m_strName, (long)pTimeInfo->m_nDiffTime);
		}
		else
		{
			if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_USER_CHANGE_TIMEDIFF, (const char *)"", (long)0);
		}

		CPacketList* pPacketList = m_pPacketListManager->GetPacketList(m_strRQ);

		if( nIndex <= 0 || !pTimeInfo )
			pPacketList->SetOrgBaseTime();		// ���� �ð����...
		else
			pPacketList->SetBaseTime(pTimeInfo->m_strName, pTimeInfo->m_nDiffTime);

		if( pPacketList && CScaleBaseData::HORZ_TICK == pPacketList->GetDateUnitType() )
		{
			CString strRQ;
			if( m_strRQ.Right( 5) == "_PAIR")	strRQ = m_strRQ.Left( m_strRQ.GetLength() - 5);
			else								strRQ = m_strRQ + "_PAIR";

			CPacketList* pPairPacketList = m_pPacketListManager->GetPacketList(strRQ);
			if( pPairPacketList )
			{
				if( !pTimeInfo || pTimeInfo->m_strName.IsEmpty() )		pPairPacketList->SetOrgBaseTime();		// ���� �ð����...
				else													pPairPacketList->SetBaseTime(pTimeInfo->m_strName, pTimeInfo->m_nDiffTime);
			}	
		}

		m_pxScaleManager->Calc_DRI();
		InvalidateControl();
		/*
		if(m_pMainBlock != NULL)
			m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::TRDATA_CAL);
		*/
		}
		break;
	case 1 :				// Time Get
		{
		CPacketList* pPacketList = m_pPacketListManager->GetPacketList(m_strRQ);
		long lResult = 0;
		if( m_pAddInManager && pPacketList ) m_pAddInManager->OnAddInToolCommand( lResult, EAI_USER_CHANGE_TIMEDIFF, (LPCTSTR)pPacketList->GetBaseTimeName());
		return lResult;
		}
		break;
	}

	return 0;
}

long CKTBChartCtrl::AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	if(m_pMainBlock)
		return m_pMainBlock->AddHorizontalLine(m_strRQ, lpszTitle, clrLine, nLine, dPrice);

	return -1;
}

void CKTBChartCtrl::DeleteHorizontalLine(long nIndex)
{
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	if(m_pMainBlock)
		m_pMainBlock->DeleteHorizontalLine(m_strRQ, nIndex);
}

void CKTBChartCtrl::DeleteAllHorizontalLine()
{
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	if(m_pMainBlock)
		m_pMainBlock->DeleteAllHorizontalLine(m_strRQ);
}

void CKTBChartCtrl::UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	if(m_pMainBlock)
		m_pMainBlock->UpdateHorizontalLine(m_strRQ, nIndex, lpszTitle, clrLine, nLine, dPrice);
}

void CKTBChartCtrl::OnNChartModeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
	
	// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
	if( m_pPacketListManager) m_pPacketListManager->ChangeChartMode( GetChartMode());
	if( m_pAddInManager) m_pAddInManager->ChangeChartMode( GetChartMode());
}

BSTR CKTBChartCtrl::GetPacketTypeFromKrxCmeCode(LPCTSTR p_szKrxCmeCode) 
{
	CString strResult;

	// TODO: Add your dispatch handler code here
	strResult = "x 1";
	if( !p_szKrxCmeCode) return strResult.AllocSysString();

	// Decimal Digits
	if( *p_szKrxCmeCode == 'D')
	{
		p_szKrxCmeCode++;
		if( *p_szKrxCmeCode < '1' || '9' < *p_szKrxCmeCode) return strResult.AllocSysString();
		strResult = "x 0." + CString( '0', *p_szKrxCmeCode - '0' - 1) + "1";
		return strResult.AllocSysString();
	}

	// No Decimal Point
	if( '0' <= *p_szKrxCmeCode && *p_szKrxCmeCode <= '9')
	{
		BOOL bOneDigit = ( '0' == *p_szKrxCmeCode);
		if( bOneDigit) p_szKrxCmeCode++;
		strResult += "/";
		strResult += p_szKrxCmeCode;
		if( bOneDigit)	strResult += "'1.0";
		else			strResult += "'2.0";
		return strResult.AllocSysString();
	}

	// for E / F Type
	if( 'E' == *p_szKrxCmeCode)			strResult += "/32'";
	else if( 'F' == *p_szKrxCmeCode)	strResult += "/64'";
	else return strResult.AllocSysString();

	// for E / F Type
	p_szKrxCmeCode++;
	if( 'H' == *p_szKrxCmeCode)			strResult += "3.1";
	else if( 'Q' == *p_szKrxCmeCode)	strResult += "4.2";
	else								strResult += "2.0";

	return strResult.AllocSysString();
}

void CKTBChartCtrl::SetEncUserID(LPCTSTR strUserID) 
{
	m_strUserID = strUserID;
}

// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
BOOL CKTBChartCtrl::OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID, LPCTSTR strCustItem1, LPCTSTR strCustItem2, LPCTSTR strCustItem3)
{
	return m_OutsideCommonInterfaceFunction.OnOrderInfomationChanged(dPirce, Qty, nOrderType, szOrgOrderID, strCustItem1, strCustItem2, strCustItem3);
}

void CKTBChartCtrl::ReCalculateAllGraphs() 
{
	ReCalculateIndicator();
}


// ���ν����� ���氡���� Method �߰� - ojtaso (20081013)
void CKTBChartCtrl::ChangeVertScaleMinMaxType(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, short nScaleType, double dMinValue, double dMaxValue)
{
	if(!m_pMainBlock)
		return;
	
	if((CScaleBaseData::VERTMINMAXTYPE)nScaleType == CScaleBaseData::VERT_MINMAX_OF_USER && (dMinValue < 0. && dMaxValue < 0.))
	{
		CString strUpDownLimit;
		if(!m_pPacketList->GetHelpMessageItem("BOUNDMARK", strUpDownLimit))
			return;
		
		ResetBound(strUpDownLimit, dMinValue, dMaxValue);
	}
	
	CBlock* pBlock = m_pMainBlock->GetBlock(CBlockIndex(nRowIndex, nColumnIndex));
	pBlock->SetVScaleMinMaxSetting(nVertScaleGroupIndex, TRUE, (CScaleBaseData::VERTMINMAXTYPE)nScaleType, dMinValue, dMaxValue);
	
	InvalidateControl();
}

// ���ν����� ���氡���� Method �߰� - ojtaso (20081013)
void CKTBChartCtrl::ResetBound(LPCSTR lpszBound, double& dMinValue, double& dMaxValue)
{
	CString strBoundMark;
	char* szBoundMark = strBoundMark.GetBufferSetLength(strlen(lpszBound));
	strcpy(szBoundMark, lpszBound);
	
	char* szIndicatorName = szBoundMark;
	char* szLowerBound = NULL;
	char* szUpperBound = NULL;
	szBoundMark = strchr(szBoundMark, ':');
	if(!szBoundMark) return;
	*szBoundMark = '\0';
	
	szBoundMark++;
	szUpperBound = szBoundMark;
	szBoundMark = strchr(szBoundMark, ',');
	if(!szBoundMark) return;
	*szBoundMark = '\0';
	
	szBoundMark++;
	szLowerBound = szBoundMark;
	
	szBoundMark = strchr(szBoundMark, ':');
	if(szBoundMark)
	{
		*szBoundMark = '\0';
		szBoundMark++;
	}
	
	if(!*szIndicatorName || !*szUpperBound || !*szLowerBound) return;
	
	dMaxValue = atof(szUpperBound);
	dMinValue = atof(szLowerBound);
	
	if(dMaxValue < dMinValue)
	{
		double dTemp	= dMaxValue;
		dMaxValue	= dMinValue;
		dMinValue	= dTemp;
	}
	
	strBoundMark.ReleaseBuffer();
}

// ��մܰ� ���� - ojtaso (20081104)
void CKTBChartCtrl::SetAvgUnitPrice(double dAvgPrice)
{
	if(!m_pMainBlock)
		return;
	
//	m_pMainBlock->SetAvgUnitPrice(m_strRQ, dAvgPrice);
}

// ������ ��Ŷ�� ũ�⸦ ���� - ojtaso (20081106)
long CKTBChartCtrl::GetPacketCount(LPCTSTR strPacketName)
{
	if(!m_pPacketList)
		return 0;
	
	CPacket* pPacket = m_pPacketList->GetBaseData(strPacketName);
	if(!pPacket)
		return 0;
	
	return pPacket->GetCount();
}

BOOL CKTBChartCtrl::FindRQ(LPCTSTR lpszRQ) 
{
	if(m_pPacketListManager)
		return m_pPacketListManager->FindRQ(lpszRQ);
	
	return FALSE;
}

// ��ƽ�϶��� ������� �ӽ÷� ���� - ojtaso (20090109)
short CKTBChartCtrl::GetPacketNearIndex(LPCTSTR szPacketName, double dKeyValue) 
{
	if(m_pPacketList == NULL)
		return -1;
	
	CPacket *pPacket = m_pPacketList->GetBaseData(szPacketName);
	if(pPacket == NULL || pPacket->GetCount() == 0)
		return -1;
	
	CString strKey;
	strKey.Format("%08.0f", dKeyValue);
	
	CString strValue;
	
	int nResult = 0;
	double value;
	POSITION pos = pPacket->GetnumericDataList()->GetHeadPosition();
	while(pos) 
	{
		value = pPacket->GetnumericDataList()->GetNext(pos);
		strValue.Format("%08.0f", value);
		
		if(strKey.Left(4) == strValue.Left(4))
		{
			if(atof(strValue.Right(4)) >= atof(strKey.Right(4))) 
				return nResult;
		}
		
		nResult++;
	}
	
	return -1;
}

// BaseLine Draw ���� - ojtaso (20081204)
void CKTBChartCtrl::SetDrawBaseLine(BOOL bDraw) 
{
	if(m_pMainBlock)
		m_pMainBlock->SetDrawBaseLine(bDraw);
}



//20100518_�ַθ�  �ش� ��ǥ�� SubGraphIndex�� Hide �Ӽ��� Get
BOOL CKTBChartCtrl::GetGraphHide4(LPCTSTR strGraphName2, short nSubGraphIndex) 
{
	if(m_pMainBlock == NULL)
		return FALSE;

	CString strGraphName = _STOM2( strGraphName2);
	return m_pMainBlock->GetSubGraphHiding(strGraphName, (int)nSubGraphIndex);
}

BOOL CKTBChartCtrl::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	CString str;
	int nKey = pCopyDataStruct->dwData;
	switch(nKey)
	{
	case 10000: // 
	case 10001:
		{
			int nSize = pCopyDataStruct->cbData;
			CString szFileName, szSize;
			
			szFileName.Format("%-*.*s", nSize, nSize, (LPCSTR)pCopyDataStruct->lpData);
			
			if(nKey==10000)
				SaveChartPropertiesToChartMapFile(szFileName);
			else
				LoadChartPropertiesToChartMapFile(szFileName);
		}
		break;
	default: 
		break;
	 }

	return COleControl::OnCopyData(pWnd, pCopyDataStruct);
}

//(2012/01/19 - alzioyes) save to file with chart's properties
BOOL CKTBChartCtrl::SaveChartPropertiesToChartMapFile(LPCSTR szSaveFile)
{
	CString strKey, strData, strTemp;
	const char *szKey=NULL, *szData=NULL;
	
	CFile file;
	if (!file.Open(szSaveFile, CFile::typeBinary|CFile::modeCreate|CFile::modeWrite))
	{
		CString strMsg;
		strMsg.Format("failed create file (%s)", szSaveFile);
		AfxMessageBox(strMsg);
		
		return FALSE;
	}

	CPropertyMap	pmProperty;
	IPropertyMap *	pIPropertyMap = &pmProperty;
	long lResult =  GetAllProperties3( ( long)pIPropertyMap);
	
	CArchive ar(&file, CArchive::store);
	
	int nCount = pIPropertyMap->GetCount();
	ar << nCount;
	
	POSITION pos = pIPropertyMap->GetStartPosition();
	while (pos)
	{
		pIPropertyMap->GetNextAssoc(pos, szKey, szData);
		strKey.Format(_T("%s"), szKey);
		strData.Format(_T("%s"), szData);
		ar << strKey;
		ar << strData;
	}
	
	ar.Close();
	file.Close();
	return TRUE;
}

//(2012/01/19 - alzioyes) Load form file and set properties to chart
BOOL CKTBChartCtrl::LoadChartPropertiesToChartMapFile(LPCSTR szLoadFile)
{
	CFile file;
	if (!file.Open(szLoadFile, CFile::typeBinary|CFile::modeRead))
	{
		CString strMsg;
		strMsg.Format("failed create file (%s)", szLoadFile);
		AfxMessageBox(strMsg);
		
		return FALSE;
	}
	
	CPropertyMap	pmProperty;
	IPropertyMap *	pIPropertyMap = &pmProperty;
	long lResult =  GetAllProperties3( ( long)pIPropertyMap);
	
	CArchive ar(&file, CArchive::load);

	int nCount = 0, i=0;
	CString strKey, strData;

	ar >> nCount;
	for (i=0; i<nCount; i++)
	{
		ar >> strKey;
		ar >> strData;
		pIPropertyMap->SetAt(strKey, strData);
	}
	ar.Close();
	file.Close();

	SetAllProperties3((long)pIPropertyMap);
	ResetChart();
	return TRUE;
}

void CKTBChartCtrl::SetItemCode(LPCTSTR p_szRQ, LPCTSTR p_szItemCode, LPCTSTR p_szItemName)
{
	// TODO: Add your dispatch handler code here
	if( !m_pPacketListManager) return;
	m_pPacketListManager->SetItemCode( p_szRQ, p_szItemCode, p_szItemName);
}

void CKTBChartCtrl::SetPriceMinMaxShowType( int nMinMaxRatio)
{
	m_nMinMaxRatio = nMinMaxRatio;

	if(m_pxScaleManager)
		m_pxScaleManager->SetPriceMinMaxShowType(m_nMinMaxRatio);
}

void CKTBChartCtrl::MoveChart( short nRow, short nCol, short nGroupIdx, short nGraphIdx, short nTargetRow, short nTargetCol, short nTargetGroupIdx, BOOL bIsAddTail )
{
	if(!m_pMainBlock)
		return;

	CBlockIndex cSrcBlock = CBlockIndex( nRow, nCol );
	CBlockIndex cTargetBlock = CBlockIndex( nTargetRow, nTargetCol );

	m_pMainBlock->MoveGraph2( cSrcBlock, nGroupIdx, nGraphIdx, cTargetBlock, nTargetGroupIdx, bIsAddTail );
	InvalidateControl();
}