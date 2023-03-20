#include "../Include_Chart/DLL_Load/GraphTypeData.h"		// for CGraphTypeAndStyleData
#include "OutsideCommonInterface.h"							// for UWM_ZOOMVALUE
#include "IndicatorInfo.h"
#include "ExcelDataController.h"							//nami 9.27 ������ ������
#include "GraphData.h"										// for CSubGraphData
#include "MainBlock.h"										// for CMainBlock
#include "ToolBarManager.h"									// for ToolBar.dll
#include "MessageDefine.h"									// for UDM_TOOL_SELECT
#include "resource.h"										// for IDS_CANNOT_DELETE_LAST_BLOCK

//#include "../include/TimeMeter DLL/TimeMeterTarget.h"	// for _TimeMeter_...

#define FILE_SYSTEMTRADER_CONDITIONS "systemTrader.cfg"

////////////////////////////////////////////////////////////////////////////////////////////////


// chartobject dll�� �Ѱ� �־�� �� ����Ÿ ���� CMainBlock�� �Ѱ��ش�
// 06.20.2001
// bUserMode - TRUE : ������ FALSE : �����θ��
//sy 2004.12.14. - ���� ������ �߸���
void CKTBChartCtrl::SetMainBlock(BOOL bUserMode, BOOL p_bWithIndicatorInfoUpdate, BOOL bWithScroll)
{
	if(m_pMainBlock == NULL)
		return ;

	if( m_pAddInManager) m_pAddInManager->OnResetMainBlock();

	// (2006/10/30 - Seung-Won, Bae) Load Map Default
	m_pMainBlock->DeleteAllBlock();
	m_pIndicatorList->LoadIndicatorInfoFromString( IsRunTimeMode() ? EILT_MAP_DEFAULT : EILT_USER, m_strMapIndicatorInfo);

	// (2006/12/22 - Seung-Won, Bae) Do Main Block Init before ToolBar (ToolBar need a Graph Setted State.)
	SetMainBlock_MainBlock(bUserMode == TRUE? true: false,p_bWithIndicatorInfoUpdate);
	SetMainBlock_ToolBar(bUserMode == TRUE? true: false);

	if( bWithScroll) ResetScroll();
}

void CKTBChartCtrl::SetMainBlock_MainBlock(const bool bUserMode, BOOL p_bWithIndicatorInfoUpdate)
{
	if(m_pMainBlock == NULL)
		return ;

	if(bUserMode)
	{
		// (2003.12.23, ��¿�) DLL�� SK Common Folder�� �ű��.
		m_pMainBlock->SetOCXPath( g_strOcxPath) ;
		m_pMainBlock->SetPacketList( m_pPacketList, m_strRQ);
		//	Scale Color setting  ���μ�, ���μ�, ���ڻ�
		m_pMainBlock->SetScaleColor(m_clrScaleLine2, m_clrScaleLine, m_clrScaleText);
		//	�߰� ���� block �� setting
		m_pMainBlock->SetMaxBlocksCountInView(m_nUserBlockAddCount);
	}
	else m_pMainBlock->SetDesignMode(!bUserMode);

	m_pMainBlock->SetIndicatorList(m_pIndicatorList) ;
	m_pMainBlock->SetXScaleManager(m_pxScaleManager);
	// �������� ���� : �������� - ojtaso (20070111)
	m_pMainBlock->SetPacketListManager(m_pPacketListManager);

	CClientDC dc(this);	
	CFont* pOldFont = dc.SelectObject(m_pRscFont);

	CGraphRegionColor BlockColor(m_clrBlock, m_clrLattice, m_clrBlockBorder);
	m_pMainBlock->SetBlockEnvironment(&dc, GetMainBlockRegion(), (CBlockBaseData::BLOCKTYPE)m_nBlockType, 
		(CBlockBaseData::BLOCKCOMPART)m_nBlockCompart, CBlockIndex(m_nBlockRowCount, m_nBlockColumnCount), 
		m_nBlockBorderThickness, m_nBlockGap, BlockColor) ;
	m_pMainBlock->SetDoubleSizeBlockNumber(CBlockIndex(m_nRowDoubleSizeBlock, m_nColumnDoubleSizeBlock));

	if(!m_strBlockRegion.IsEmpty() && m_strBlockRegion.Right(2) != "\r\n")
		m_strBlockRegion += "\r\n";
	m_pMainBlock->SetBlocksPosition(m_strBlockRegion);
	m_pMainBlock->SetGraphBackgroundGap(m_nHorzGap, m_nVertGap);
	m_pMainBlock->SetGraphTitleDraw(CDataConversion::atob(m_bUseConditiontoTitle),
									CDataConversion::atob(m_bUseDatatoTitle));
	m_pMainBlock->SetEtcGraphColor(m_clrGraphBase1, m_clrGraphBase2, m_clrGraphBase3, m_clrGraphBase4, m_clrGraphBase5, m_clrGraphBase6) ;
	//sy 2004.11.29. ��� graph title ���� ����
	m_pMainBlock->SetAllGraphsTitleHiding( m_bAllGraphTitleHiding);

	if(m_strChartList.Right(2) != "\r\n")
		m_strChartList += "\r\n";
	if(m_strBlockRegion.Right(2) != "\r\n")
		m_strBlockRegion += "\r\n";

	// �׷��� ������ ��� ���� : xScaleManager - ojtaso (20070701)
	BOOL bCalc = m_pMainBlock->BuildGraphsAndScales(m_strChartList,p_bWithIndicatorInfoUpdate);

	// (2009/6/25 - Seung-Won, Bae) for Balanced Row Arrange without scale.
	//		Set only price vert scale after building graph and scale to calculate with scale.
	m_pMainBlock->SetOnlyPriceVertScale(&dc, m_bShowMinMaxbyPrice == TRUE);

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	for Old Version Map and User File.
	if( 0 <= m_nVertScaleType)
	{
		m_pMainBlock->SetAllHorzScaleGridType( (CScaleBaseData::SCALEGRIDTYPE)m_nVertScaleType);
		m_pMainBlock->SetAllVertScaleGridType( (CScaleBaseData::SCALEGRIDTYPE)m_nVertScaleType);
	}

	// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
	m_pMainBlock->SetShowPriceMinMaxText( m_bShowPriceMinMaxText);

	// (2009/5/17 - Seung-Won, Bae) for Right Margin and Future Time
	m_pxScaleManager->SetRightMarginAndFutureTime( m_nRightMarginWidth, m_nFutureTimeCount);

	m_pxScaleManager->SetPriceMinMaxShowType(m_nMinMaxRatio);

	// ����(05/01/04) Ư����ǥ�϶� ���������, �Ϲ���Ʈ������ ��ȯ�� SliderCtrl�� Disable�Ǵ� ���� ����
	// (2005.01.12, ��¿�) Design Time�ÿ� ToolBar�� ���� ��츦 �����Ѵ�.
	SetOneChart();

	if(m_hContainerDllWnd)
	{
		// TR�� ������ ���� �����̹Ƿ� ��� �� �ʿ䰡 ����.
		m_pMainBlock->ChangeIndicatorInfoFromGraphData( _MTEXT( C2_PRICE_MA));
		m_pMainBlock->ChangeIndicatorInfoFromGraphData( _MTEXT( C2_VOLUME_MA));
	}

	dc.SelectObject(pOldFont);
}

void CKTBChartCtrl::SetMainBlock_ToolBar(const bool bUserMode)
{
	if( m_pMainBlock == NULL || m_pPacketList == NULL) return ;

	// 47.5 Loading�� '�� ȭ�� �ּ� Data ��'�� SlideBar�� �����Ѵ�.
	if( m_pToolBarManager) m_pToolBarManager->SetSliderMin( m_nMinDataCountOnOnePage);

	//sy 2005.12.01 drag ���� ����(��ġ��ȸ/Ȯ�����)
	// ����(05/05/12) ��ġ��ȸâ �̵����¸� �����Ѵ�.
	SetNumericalInquiryOption(  m_nNumericalInquiryOption, false);
	//sy end
}

// Get ocx region - toolbar
CRect CKTBChartCtrl::GetOcxRegion() const
{
	CRect region = GetToolBarRegion();
	if(m_pToolBarManager)
		region.bottom -= m_pToolBarManager->GetToolBarRect().Height();
	
	return region;
}

// Get MainBlock Area
CRect CKTBChartCtrl::GetMainBlockRegion()
{
	CRect region = GetVirtualRegion();

	// (2006/10/8 - Seung-Won, Bae) Show InnerBorder in the case of No-Margin. // if(m_nTopMargin + m_nBottomMargin + m_nLeftMargin + m_nRightMargin > 0)
	if(m_nMarginBorderType < 3) region.DeflateRect(1, 1);

	return region;
}

CRect CKTBChartCtrl::GetVirtualRegion()
{
	CRect region = GetOcxRegion();

	region.top += m_nTopMargin;
	region.bottom -= m_nBottomMargin;
	region.left += m_nLeftMargin;
	region.right -= m_nRightMargin;

	if(m_nBorderType < 3)
		region.DeflateRect(1, 1);

	return region;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	User Message

//wparam == 0�̸� ��������
//1�̸� ��ǥ�������� tab �������� ���� ����Ÿ �����ϱ�.
LRESULT CKTBChartCtrl::OnSetEtc( WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		// (2009/5/13 - Seung-Won, Bae) for Right Margin
		case 0:	if( lParam < 0) return m_nRightMarginWidth;
				SetRightMarginWidth( lParam);
				break;
		//nami 9.11
		case 1:
			SaveChartCfg();
			break;

		// (2009/5/18 - Seung-Won, Bae) Disable X scale margin on special chart.
		case 2:	return IsRunningOneChart();

		default:
			break;
	}

	return 1L;
}

// <�ϸ����ǥ> �� ��� Margin�� Indicator ���� �����´�.
int CKTBChartCtrl::GetMarginInIndicatorInfo( const CString &p_strIndicatorName) const
{
	if( !m_pIndicatorList) return 0;
	if( p_strIndicatorName != _MTEXT( C2_OBVIOUSLY_BALANCE_CHART)) return 0;

	// (2006/12/11 - Seung-Won, Bae) Support Multi-Same Indicator.
	int nMargin = 0;
	POSITION psnIndicator = NULL;
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_OBVIOUSLY_BALANCE_CHART), &psnIndicator);
	while( pIndicatorInfo)
	{
		const CList<double, double>* pCalCondList = pIndicatorInfo->GetCommonCalConditionList();
		if( !pCalCondList) continue;

		// <�ϸ����ǥ>�� Margin�� 1��°�� �ش� �ȴ�.
		POSITION pos = pCalCondList->FindIndex( 0);
		if( !pos) continue;

		int nCurrentMargin = pCalCondList->GetAt( pos) - 1;
		if( nMargin < nCurrentMargin) nMargin = nCurrentMargin;

		m_pIndicatorList->GetNextIndicatorInfo( EILT_USER, _MTEXT( C2_OBVIOUSLY_BALANCE_CHART), &psnIndicator, pIndicatorInfo);
	}

	return nMargin;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	test method

//	data size�� ����� return �ϴ��� test�� method
short CKTBChartCtrl::testGetPacketdatasize(LPCTSTR pIn3) 
{
	CString strIn = _STOM0( pIn3);
	LPCTSTR pIn = strIn;

	CString pIn2 = pIn ;
	if(pIn2.IsEmpty())
	{
		ML_SET_LANGUAGE_RES();
		CString strMsg;
		strMsg.LoadString( IDS_NO_DATA);
		AfxMessageBox( strMsg);
		return 0;
	}

	int nRet = 0;
	int lCount = 8, lBozo = 0, lData = 0, nCount = 0 ;

	char szCount[5], szBozo[5] ;
	//	Count
	memcpy(szCount, (void*)pIn, 4) ;
	szCount[4] = NULL ;	
	nCount = atoi(szCount) ;
	//	count(4) + �����޼�������(4) �� ���
	if(pIn2.GetLength() > 6)	{
		//	�����޼�������
		memcpy(szBozo, (void*)(pIn+4), 4) ;
		szBozo[4] = NULL ;
		lBozo = atoi(szBozo) ;
	}
	//	count(4) + �����޼�������(2) �� ���
	else	{
		char szBozo1[2], szBozo2[2];
		//	�����޼�������
		memcpy(szBozo1, (void*)(pIn+4), 1) ;
		szBozo1[1] = NULL ;
		memcpy(szBozo2, (void*)(pIn+5), 1) ;		
		szBozo2[1] = NULL ;
		lBozo = (int)((unsigned char)szBozo1[0])*256 + (int)((unsigned char)szBozo2[0]);
	}
	//	Data
	lData = GetOnePacketSize() ;
	nRet = lCount + lBozo + (lData * nCount) ;

	return nRet;
}

//	Analysis Tool & Analysis Function Test
void CKTBChartCtrl::testLoadAnalysisTools(short nToolOption, short nOption) 
{
	RunToolFunction((CToolOptionInfo::TOOLOPTION)nToolOption, nOption);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	methods

// (2006/11/22 - Seung-Won, Bae) Do not support the Method 'ChangeChart' anymore.
short CKTBChartCtrl::NotUsedMethod5(short nBlockRowPosition, short nBlockColumnPosition, 
									 LPCTSTR strGraphTitle, LPCTSTR strGraphName, 
									 LPCTSTR strPacketName, LPCTSTR strGraphType, 
									 LPCTSTR strGraphStyle, short nLineWidth, 
									 OLE_COLOR clrLineColor) 
{
//	_ML_TEST_NOT_USED();
	AfxMessageBox("NotUsedMethod5");
	return -1;
}


#include "DumpLogger.h"									// for CDumpLogger
// Chart���� ��� data�� clear���ִ� �ܺ� method
short CKTBChartCtrl::ClearChartData(short nOption) 
{
	if(!m_pPacketListManager)
		return -1;
	if( m_bEnableLogChartLoading) GetLocalTime( &m_stLogStart);

	// (2008/11/9 - Seung-Won, Bae) for Multiple-Item.
	CList < CString, CString> slRQs;
	m_pPacketListManager->GetAllRQ( slRQs);
	slRQs.AddTail( "DEFAULT");
	POSITION psn = slRQs.GetHeadPosition();
	while( psn)
	{
		CString strRQ = slRQs.GetNext( psn);
		CPacketList *pPacketList = m_pPacketListManager->GetPacketList( strRQ);
		if( pPacketList) pPacketList->ClearDataAll( TRUE);
	}

	InvalidateControl();

	if( m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\n",
			m_stLogStart.wHour, m_stLogStart.wMinute, m_stLogStart.wSecond, m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds);
		CDumpLogger::LogStringToFile( "CKTBChartCtrl::ClearChartData()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}

	return 1;
}

short CKTBChartCtrl::SetGraphHide(short nRow, short nColumn, LPCTSTR strTitle, BOOL bHide) 
{
	CString strTitle2 = _STOM1( strTitle);

	if(!m_pMainBlock)	return 0;
	if(nRow < 0 || nColumn < 0) return 0;
	
	if(!m_pMainBlock->SetSubGraphHiding(CBlockIndex(nRow, nColumn), strTitle2, CDataConversion::atob(bHide)))
		return 0;

	InvalidateControl();
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//	methods for making .ini  file  & functions

// make init file ... gen.ini (properties)
int SaveFormatProperties(const CString &strFileName)
{
	if(strFileName.IsEmpty())	return -1;

	int nIndex = 0;
	char pKey[4096];

	CString strData ;
	for(int i = 0 ; i < CHART_PROPERTY_COUNT; i++)
	{
		strData.Format("%s=%s", g_aszPropertyName[i], g_anPropertyShortKey[i]);
		strcpy(pKey + nIndex, (LPSTR)(LPCTSTR)strData); 
		nIndex = nIndex + strlen(pKey + nIndex); 
		*(pKey + nIndex) = '\0'; 
		nIndex++;
	}
	*(pKey + nIndex) = '\0';

	ML_SET_DEFAULT_RES();
	CString strfullname;
	strfullname.LoadString(IDS_FULLNAME);
	WritePrivateProfileSection(strfullname, (LPCTSTR)pKey, strFileName);

	return 1;
}

// ��ü event �̸���.


//######## ���� Ȯ�� �� �߰� Event ó�� �ʿ�
void GetEventsTitle(CString& strTitle)
{
	strTitle = "0=DataUpandDown\r\n";
	strTitle += "1=RequestBongData\r\n";
	strTitle += "2=ChangeOnePageDataCount\r\n";
	strTitle += "3=Restore\r\n";
	strTitle += "4=RequestMapLoad\r\n";
	strTitle += "5=RequestNewPacket\r\n";
	strTitle += "6=SaveSettedChartInfoToServer\r\n";
	strTitle += "7=GetSettedChartInfoFromServer\r\n";
	strTitle += "8=RequestPacketData\r\n";
	strTitle += "9=AddInEvent\r\n";						// ---> bds test �߰� 
	strTitle += "10=ChangeDateIndex\r\n";
	strTitle += "11=RequestCodeAndName\r\n";
	strTitle += "12=SendDataIndexInLButtonUp";
}

void SaveFormatEvent(const CString& strData, 
		const CString& strSectionName, const CString& strFileName)
{
	char pKey[4096];
	int nIndex = 0;
	strcpy(pKey + nIndex, (LPSTR)(LPCTSTR)strData); 
	nIndex = nIndex + strlen(pKey + nIndex); 
	*(pKey + nIndex) = '\0';
	nIndex++;
	*(pKey + nIndex) = '\0';

	WritePrivateProfileSection(strSectionName, (LPCTSTR)pKey, strFileName);	
}

void SaveFormatEvents_EventTitle(const CString& strFileName, const CString& strFullName)
{
	CString strTitle;
	GetEventsTitle(strTitle);

	SaveFormatEvent(strTitle, ("EVENT_"+strFullName), strFileName);
}

void SaveFormatEvent_DateUpAndDown(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=nOption,strStartDate,strEndDate\r\n0=nOption\r\n1=strStartDate\r\n2=strEndDate";

	SaveFormatEvent(strData, (strEventName + "_dataupanddown"), strFileName);
}

void SaveFormatEvent_RequestBongData(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=nOption,nTemp\r\n0=nOption\r\n1=nTemp";

	SaveFormatEvent(strData, (strEventName + "_requestbongdata"), strFileName);
}

void SaveFormatEvent_ChangeOnePageDataCount(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=nOnePageDataCount\r\n0=nOnePageDataCount";

	SaveFormatEvent(strData, (strEventName+"_changeonepagedatacount"), strFileName);
}

void SaveFormatEvent_Restore(const CString& strFileName, const CString& strEventName)
{
	CString strData = "0=----------";

	SaveFormatEvent(strData, (strEventName+"_restore"), strFileName);
}

void SaveFormatEvent_RequestMapLoad(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=nOption,strCode,strDate\r\n0=nOption\r\n1=strCode\r\n2=strDate";

	SaveFormatEvent(strData, (strEventName+"_requestmapload"), strFileName);
}

void SaveFormatEvent_RequestNewPacket(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=nOption,strCode,strCode,strDate\r\n0=nOption\r\n1=strName\r\n2=strCode\r\n3=strDate";

	SaveFormatEvent(strData, (strEventName+"_requestnewpacket"), strFileName);
}

void SaveFormatEvent_SaveSettedChartInfoToServer(const CString& strFileName, const CString& strEventName)
{
	CString strData = "0=----------";

	SaveFormatEvent(strData, (strEventName+"_savesettedchartinfotoserver"), strFileName);
}

void SaveFormatEvent_GetSettedChartInfoFromServer(const CString& strFileName, const CString& strEventName)
{
	CString strData = "0=----------";

	SaveFormatEvent(strData, (strEventName+"_getsettedchartinfofromserver"), strFileName);
}

void SaveFormatEvent_RequestPacketData(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=strDateType,strStartDate,strPacketNames\r\n0=strDateType\r\n1=strStartDate\r\n2=strPacketNames";

	SaveFormatEvent(strData, (strEventName+"_requestpacketdata"), strFileName);
}

void SaveFormatEvent_AddInEvent(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=strAddInName,nEventID,strData,dData,lData\r\n0=strAddInName\r\n1=nEventID\r\n2=szData\r\n3=dData\r\n4=lData";
	
	SaveFormatEvent(strData, (strEventName+"_addinevent"), strFileName);
}

void SaveFormatEvent_ChangeDateIndex(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=nDateIndex\r\n0=nDateIndex";
	SaveFormatEvent(strData, (strEventName+"_changedateindex"), strFileName);
}

void SaveFormatEvent_RequestCodeAndName(const CString& strFileName, const CString& strEventName)
{
	CString strData = "0=----------";
	SaveFormatEvent(strData, (strEventName+"_requestcodeandname"), strFileName);
}

void SaveFormatEvent_SendDataIndexInLButtonUp(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=nRowIndex,nColumnIndex,nDataIndex,dValue\r\n0=nRowIndex\r\n1=nColumnIndex\r\n2=nDataIndex\r\n3=dValue";
	SaveFormatEvent(strData, (strEventName+"_senddataindexinlbuttonup"), strFileName);
}

void SaveFormatEvent_ReturnOrgChart(const CString& strFileName, const CString& strEventName)
{
	CString strData = "Param=nOnePageDataCount\r\n0=nOnePageDataCount";

	SaveFormatEvent(strData, (strEventName+"_returnorgchart"), strFileName);
}

void GetEventName(CString& strFullName, CString& strEventName)
{
	ML_SET_DEFAULT_RES();
	strFullName.LoadString(IDS_FULLNAME);

	strEventName = strFullName;
	strEventName.MakeLower();
	strEventName = "event_" + strEventName;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/1
// Modifier		: 
//-----------------------------------------------------------------------------
int SaveFormatEvents( const CString &strFileName)
{
	if( strFileName.IsEmpty()) return -1;

	CString strFullName, strEventName;
	GetEventName(strFullName, strEventName);

	SaveFormatEvents_EventTitle(strFileName, strFullName);
	SaveFormatEvent_DateUpAndDown(strFileName, strEventName);
	SaveFormatEvent_RequestBongData(strFileName, strEventName);
	SaveFormatEvent_ChangeOnePageDataCount(strFileName, strEventName);
	SaveFormatEvent_Restore(strFileName, strEventName);
	SaveFormatEvent_RequestMapLoad(strFileName, strEventName);
	SaveFormatEvent_RequestNewPacket(strFileName, strEventName);


	// bds test �߰� 
	// {{
	SaveFormatEvent_SaveSettedChartInfoToServer(strFileName, strEventName);
	SaveFormatEvent_GetSettedChartInfoFromServer(strFileName, strEventName);
	// }}
	
	SaveFormatEvent_RequestPacketData(strFileName, strEventName);

	// bds test �߰� 
	// {{
	SaveFormatEvent_AddInEvent(strFileName, strEventName);
	// }}

	SaveFormatEvent_ChangeDateIndex(strFileName, strEventName);
	SaveFormatEvent_RequestCodeAndName(strFileName, strEventName);
	SaveFormatEvent_SendDataIndexInLButtonUp(strFileName, strEventName);
	SaveFormatEvent_ReturnOrgChart(strFileName, strEventName);
	return 1;
}

CHARTOCX_API void MakeSaveShortKey(LPCTSTR szFileName, short nFlag) 
{
	CString strFileName = szFileName ;

	if(strFileName.IsEmpty())	return ;

	//	gen.ini -> properties 
	if(nFlag == 0)
		SaveFormatProperties(strFileName) ;
	//	hanaro_script.ini -> events
	else if(nFlag == 1)
		SaveFormatEvents(strFileName) ;
}

// make init file .. gen.ini, hanaro_script.ini
// nflag : 0 - save properties
// nflag : 1 - save events
// (2005/8/30 - Seung-Won, Bae) Convert it from Member Function to Global Routine for External Access without OCX
void CKTBChartCtrl::MakeSaveShortKey(LPCTSTR szFileName, short nFlag)
{
	::MakeSaveShortKey( szFileName, nFlag);
}



////////////////////////////////////////////////////////////////////////////////////////////////
//	write or read user environment
void CKTBChartCtrl::WriteUserFile(CSaveUserFile::FILETYPE nType)
{
	//sy 2004.1.28. -> ������ ����϶��� ������Ѵ�.
	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	if( !IsRunTimeMode() || m_bFreezedPageState)
		return;

	if(nType == CSaveUserFile::ENVIRONMENT)
		WriteUserFile(m_strUserEnvironmentFileName);
	else if(nType == CSaveUserFile::INDICATORLIST)
	{
		//sy 2005.02.21.
		// �ܺ� �޼ҵ峪 ��Ÿ �ٸ� ���·� ���� ȭ���� graph�� ����� ���
		// -> "Indicond user file" �� "Env user file" �������� �����Ѵ�.
		if(m_pMainBlock != NULL)
			m_pMainBlock->ChangeAllIndicatorInfoFromGraphData();

		m_pIndicatorList->SaveIndicatorInfo( EILT_USER, GetUserFilePathAndName(m_strUserIndicatorListFileName));
	}
	// �м��� ���� ���� - ojtaso (20080728)
	else if(nType == CSaveUserFile::ANALTOOL)
		WriteUserFileOnlyAnalTool(m_strUserEnvironmentFileName);
}

//	read
BOOL CKTBChartCtrl::ReadUserFile(CSaveUserFile::FILETYPE nType)
{
	//sy 2004.1.28. -> ������ ����϶��� ������Ѵ�.
	if(!IsRunTimeMode())
		return FALSE;

	if(nType == CSaveUserFile::ENVIRONMENT) return ReadUserFile(m_strUserEnvironmentFileName);
	else if(nType == CSaveUserFile::INDICATORLIST)	{	
		CString szFilePath = GetUserFilePathAndName(m_strUserIndicatorListFileName);

		// (2004.04.05, ��¿�) SetAllProperties2()�� ȣ����� �ʾƵ�, Exception�� �߻����� �ʵ��� �Ѵ�. (for DEBUG)
		//		Indicator List�� ��� Exception�� �߻����� �ʰ� �����Ѵ�.
		BOOL bResult = FALSE;
		if( m_pIndicatorList) bResult = m_pIndicatorList->LoadIndicatorInfo( EILT_USER, szFilePath);

		//sy 2003.3.7 - �ӵ� ����
		//IndicatorInfo �� ���ؼ� MainBlock�� ������ �ִ� info�� �ּҵ� �ʱ�ȭ
		if(bResult && m_pMainBlock != NULL)
		{
			m_pMainBlock->Initialize_IndicatorInfo(FALSE);

			// (2004.08.31, ��¿�) ��ǥ ���ǰ��� ����Ǿ����Ƿ�, ��ǥ�� �����Ѵ�.
			//		�̶� ������ �̹� ���� ��ǥ�� ��� Clear ��Ų��.
			if( m_pPacketList) m_pPacketList->ClearAllIndicatorData( TRUE);

			m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::TRDATA_CAL);

			// (2006/9/25 - Seung-Won, Bae) Notify User Indicator Info Loaded.
			if( m_pAddInManager) m_pAddInManager->OnUserIndiatorConditionLoaded();
		}
		return bResult;
	}
	// �м��� ���� ���� - ojtaso (20080728)
	else if(nType == CSaveUserFile::ANALTOOL) return ReadUserFileOnlyAnalTool(m_strUserEnvironmentFileName);

	return FALSE;
}

//sy 2005.08.07.
//write
#include "FileBuilder.h"
void CKTBChartCtrl::WriteUserFile(const CString &strFileName)
{
	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	if( m_bFreezedPageState) return;

	CString strFilePathAndName = GetUserFilePathAndName(strFileName);
	if(strFilePathAndName.IsEmpty())
		return;

	CString strWriteData = GetDataFromMainBlock();
	if(strWriteData.IsEmpty())
		return;

	if(CFileBuilder().FileCreatWrite(strFilePathAndName, strWriteData) == -1)
	{
		// folder �� ���°��� folder ����.
		if(CreateUserDirectory())
			CFileBuilder().FileCreatWrite(strFilePathAndName, strWriteData);
	}
}

// �м��� ���� ���� - ojtaso (20080728)
void CKTBChartCtrl::WriteUserFileOnlyAnalTool(const CString &strFileName)
{
	if(m_strItemCode.IsEmpty())
		return;

	// 20080920 ������ �����̸����� ����� �� ���� Ư������(\/:*?"<>|)�߿��� �����ڵ�� ������ ���� ����
	//		�ٸ� ������ m_strItemCode ����� ���� ���� ���⼭ ó��
	m_strItemCode.Remove('\\');
	m_strItemCode.Remove('/');
	m_strItemCode.Remove(':');
	m_strItemCode.Remove('|');

	CString strFileNameAnalTool;
	strFileNameAnalTool.Format(_T("AnalTool_%s_%s"), m_strItemCode, strFileName);

	CString strFilePathAndName = GetUserFilePathAndName(strFileNameAnalTool);
	if(strFilePathAndName.IsEmpty())
		return;

	CPropertyMap *pAddInEnvMap = new CPropertyMap;
	IPropertyMap *pIAddInEnvMap = pAddInEnvMap;

	if( m_pAddInManager) m_pAddInManager->OnSaveEnvironment( pAddInEnvMap);

	int nLenBuffer = 0;
	const char *szKey = NULL;
	const char *szValue = NULL;
	POSITION psnEnv = pIAddInEnvMap->GetStartPosition();
	while( psnEnv)
	{
		pIAddInEnvMap->GetNextAssoc( psnEnv, szKey, szValue);
		nLenBuffer += 3 + strlen( szKey) + 3 + strlen( szValue);
	}

	// �м��� ���� ���� - ojtaso (20080728)
	LPCTSTR lpszAnalToolSection = _T("ANALYSIS_TOOL");
	LPCTSTR lpszPeriodSumSection = _T("PERIOD_SUMMARY");
	LPCTSTR lpszSymbolTool = _T("SYMBOL_TOOL");

	CString strData;
	char *szBuffer = strData.GetBufferSetLength( nLenBuffer);
	psnEnv = pIAddInEnvMap->GetStartPosition();
	while( psnEnv)
	{
		pIAddInEnvMap->GetNextAssoc( psnEnv, szKey, szValue);

		// �м��� ���� ���� - ojtaso (20080728)
		if(!::lstrcmp(lpszAnalToolSection, szKey) || !::lstrcmp(lpszPeriodSumSection, szKey) || !::lstrcmp(lpszSymbolTool, szKey))
		{
			strcpy( szBuffer, "\r\n[");
			szBuffer += 3;
			strcpy( szBuffer, szKey);
			szBuffer += strlen( szKey);
			strcpy( szBuffer, "]\r\n");
			szBuffer += 3;
			strcpy( szBuffer, szValue);
			szBuffer += strlen( szValue);
		}
	}

	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	delete pAddInEnvMap;

	if(strData.IsEmpty())
		return;

	if(CFileBuilder().FileCreatWrite(strFilePathAndName, strData) == -1)
	{
		// folder �� ���°��� folder ����.
		if(CreateUserDirectory())
			CFileBuilder().FileCreatWrite(strFilePathAndName, strData);
	}
}

//read
BOOL CKTBChartCtrl::ReadUserFile(const CString& strFileName)
{
	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	if( strFileName.IsEmpty()) return FALSE;

	CString strFileData = CFileBuilder().FileRead(GetUserFilePathAndName(strFileName));
	if(strFileData.IsEmpty())
	{
		// folderewrtyu �� �ִ��� Ȯ���Ѵ�.
		CreateUserDirectory();
		return FALSE;
	}

	SetDataToMainBlock(strFileData);
	return TRUE;
}

// �м��� ���� ���� - ojtaso (20080728)
BOOL CKTBChartCtrl::ReadUserFileOnlyAnalTool(const CString& strFileName)
{
	if(m_strItemCode.IsEmpty())
		return FALSE;

	// 20080920 ������ �����̸����� ����� �� ���� Ư������(\/:*?"<>|)�߿��� �����ڵ�� ������ ���� ����
	//		�ٸ� ������ m_strItemCode ����� ���� ���� ���⼭ ó��
	m_strItemCode.Remove('\\');
	m_strItemCode.Remove('/');
	m_strItemCode.Remove(':');
	m_strItemCode.Remove('|');

	CString strFileNameExt;
	strFileNameExt.Format(_T("AnalTool_%s_%s"), m_strItemCode, strFileName);

	CString strFileData = CFileBuilder().FileRead(GetUserFilePathAndName(strFileNameExt));
	if(strFileData.IsEmpty())
	{
		// folderewrtyu �� �ִ��� Ȯ���Ѵ�.
		CreateUserDirectory();
		return FALSE;
	}

	CString strBuffer;
	char* szBuffer = strBuffer.GetBufferSetLength(strFileData.GetLength());
	::lstrcpy(szBuffer, strFileData);

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

	if( m_pAddInManager) m_pAddInManager->OnLoadEnvironment( pAddInEnvMap);

	delete pAddInEnvMap;

	return TRUE;
}

// create directory
bool CKTBChartCtrl::CreateUserDirectory()
{
	CString strUserFilePath = GetUserFilePath();

	// mapName ������ ���� ���
	if(CreateDirectory(strUserFilePath, NULL))
		return true;

	CString strUserDefaultFilePath = GetUserDefaultFilePath();
	if(!CreateDirectory(strUserDefaultFilePath, NULL))
		return false;

	return CreateDirectory(strUserFilePath, NULL);
}
//sy end

////////////////////////////////////////////////////////////////////////////////////////////////
//	menu

void CKTBChartCtrl::OnDeleteBlock() 
{
	// ver 1.2
	// ������Ʈ�� �ִ� ���� ������ �ʴ´�
	if(m_pMainBlock)
	{
		ML_SET_LANGUAGE_RES();
		CString strMsg;
		if(m_pMainBlock->GetRowCount() <= 1)
		{
			strMsg.LoadString( IDS_CANNOT_DELETE_LAST_BLOCK);
			AfxMessageBox( strMsg);
		}
		else if( !m_pMainBlock->IsExistenceIndicatorInBlock( _MTEXT( C2_PRICE_CHART), m_ptRButtonMenuPopupPosition))
			m_pMainBlock->DeleteBlock( m_pMainBlock->FindBlockIndex( m_ptRButtonMenuPopupPosition), FALSE);
		else
		{
			strMsg.LoadString( IDS_CANNOT_DELETE_PRICE_CHART_BLOCK);
			AfxMessageBox( strMsg);
		}
	}

	m_ptRButtonMenuPopupPosition = CPoint(-1, -1);
	InvalidateControl();
}

void CKTBChartCtrl::OnReturnOrg() 
{
	OnReturnOrg(true);
}

//sy 2005.10.26.
void CKTBChartCtrl::OnReturnOrg(const bool bIsMessageBox) 
{
	bool bResult = OnReturnOrg2(bIsMessageBox);
	NeoRestore();

	if(bResult && m_hContainerDllWnd != NULL)
		SendEventMsgToDLL(UWM_ORGRETURN, 0, 0);
}

//����Ʈ�� ����
BOOL CKTBChartCtrl::ChangeOneChart(const CString& p_strIndicatorName, CString &p_strGraphName)
{
	p_strGraphName.Empty();
	//Ư����Ʈ�� ��츸 �����Ѵ�.
	if(m_pMainBlock == NULL || m_pIndicatorList == NULL || !g_iMetaTable.IsSpecialGroupGraph(p_strIndicatorName)) return FALSE;

	//sy 2004.05.12.
	// make string of onechart
	CString strChartBuildString;
	if( !GetBlockDataForMaking( p_strIndicatorName, strChartBuildString, p_strGraphName)) return FALSE;
	if( strChartBuildString.IsEmpty() || p_strGraphName.IsEmpty()) return FALSE;
	//sy end

	//{{2007.05.31 by LYH Graph �������� RQ ������ ������ ����
	m_pPacketListManager->SetRemovablePacketRQ(FALSE);
	//}}

	// (2005.01.12, ��¿�) Ư����Ʈ���� ��ȯ�� �ӽ����� ��ȯ�� ����ϹǷ�, ��ǥ ���ǰ��� �ڵ����� �������� �ʴ´�.
	m_pMainBlock->SetAutoDeleteIndicatorInfo( FALSE);

	//���� ����Ʈ�� �ƴ� ���
	if(!IsRunningOneChart())
	{
		m_nOrgBlockRowCount = m_pMainBlock->GetRowCount();
		m_nOrgBlockColumnCount = m_pMainBlock->GetColumnCount();
		m_nOrgRowDoubleSizeBlock = (m_pMainBlock->GetDoubleSizeBlockNumber()).GetRow();
		m_strOrgBlocksPosition = m_pMainBlock->GetBlocksPosition();

		// �߰��� ������ ����
		SaveChartCfg();
	}

	// ��� �м�����, �ɺ� �����
	RunToolFunction(CToolOptionInfo::T_DELETE_ALL, 3);

	//sy 2004.05.12. : block data �� �ֱ� ���� block�� ������ �����Ѵ�.
	m_pMainBlock->SetBlockCount(CBlockIndex(1, 1));
	// �׷��� ������ ��� ���� : xScaleManager - ojtaso (20070701)
	BOOL bCalc = m_pMainBlock->BuildGraphsAndScales(strChartBuildString);

	//sy 2006.03.02
	SetOneChart();
	
	// (2007/2/8 - Seung-Won, Bae) Set WholeView Mode with ToolSet.
	RunToolFunction(CToolOptionInfo::T_WHOLEVIEW, 1);

	// (2005.01.12, ��¿�) Ư����Ʈ���� ��ȯ�� �ӽ����� ��ȯ�� ����ϹǷ�, ��ǥ ���ǰ��� �ڵ����� �������� �ʴ´�.
	m_pMainBlock->SetAutoDeleteIndicatorInfo( TRUE);

	//{{2007.05.31 by LYH
	if(m_pPacketListManager)
		m_pPacketListManager->SetRemovablePacketRQ(TRUE);
	//}}

	InvalidateControl();

	return TRUE;
}

// make string of onechart
BOOL CKTBChartCtrl::GetBlockDataForMaking(const CString& p_strIndicatorName, CString &p_strBlockData, CString &p_strGraphName) const
{
	// (2004.10.06, ��¿�) ������Ʈ�� Graph Drawing Data�� �����޾� DefaultType�� ���� ����, ������� �Ѵ�.
	p_strGraphName.Empty();
	CIndicatorDefaultData indiDefaultData( this);
	GetIndicatorDefaultData( indiDefaultData);
	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	return indiDefaultData.GetBlockDataForMaking( p_strBlockData, m_pIndicatorList, p_strIndicatorName, CScaleBaseData::SHORTDOTTEDLINE,
		CScaleBaseData::SHORTDOTTEDLINE, p_strGraphName);
}

//���� ����Ʈ�� ��� ������ ����Ʈ�� �ƴ� ���·� ������.
bool CKTBChartCtrl::RollbackLastNoOneChart()
{
	if(m_pMainBlock == NULL || m_pIndicatorList == NULL) return false;

	if( !IsRunningOneChart()) return false;

	//{{2007.05.31 by LYH Graph �������� RQ ������ ������ ����
	m_pPacketListManager->SetRemovablePacketRQ(FALSE);
	//}}

	//���� ����Ʈ �� ��� -> ����Ʈ�� �ƴ� ������ ���·� ����.
	ResetSaveChartCfg(2);

	//{{2007.05.31 by LYH
	if(m_pPacketListManager)
		m_pPacketListManager->SetRemovablePacketRQ(TRUE);
	//}}

	return true;
}
//sy end

//	mainblock���� ������ �ٷ� ���� function�� ����� �Ŀ� ���Ѱ��� ocx���� �˰��� �θ��� �Լ�
BOOL CKTBChartCtrl::ResetPropertiesfromMainBlock()
{
	if( !m_pMainBlock || !m_pIndicatorList)	return FALSE;

	m_pMainBlock->GetAllBlocksData( m_strChartList);
	m_strBlockRegion = m_pMainBlock->GetBlocksPosition();
	m_nBlockRowCount = m_pMainBlock->GetRowCount();
	m_nBlockColumnCount = m_pMainBlock->GetColumnCount();
	// (2006/10/30 - Seung-Won, Bae) Support Map Indicator Info
	m_pIndicatorList->GetSaveIndicatorInfoString( IsRunTimeMode() ? EILT_MAP_DEFAULT : EILT_USER, m_strMapIndicatorInfo);

	// (2008/2/5 - Seung-Won, Bae) for MultiLanguage
	m_strLocalLanguageTable = g_iMetaTable.GetLocalLanguageTable( GetSafeHwnd());

	return TRUE;
}

CChartCfg* CKTBChartCtrl::GetLastChartCfg_NoOneChart()
{
	if(!IsRunningOneChart())
		return NULL;

	POSITION pos = m_SaveChartcfg.GetTailPosition();
	while(pos != NULL){
		CChartCfg* pChartCfg = m_SaveChartcfg.GetPrev(pos);
		if(pChartCfg == NULL)
			break;
		CString strChartList = pChartCfg->GetChartList();
		//sy 2004.05.12. : ������ Ư����Ʈ�� ������ <������Ʈ>�� ÷�εǴ� ��Ʈ�� ������.
		if(strChartList.Find( _MTEXT( C2_INVERSE_TIME_LINE)) == -1 && strChartList.Find("P&F") == -1 &&
			//strChartList.Find("���Ź�") == -1 && 
			strChartList.Find( _MTEXT( C2_THREE_LINE_BREAK)) == -1)
			return pChartCfg;
	}

	return NULL;
}

//Ư����Ʈ�� ������ �Ǿ� �־��ٸ�
//ȭ���� �ٽ� ��ﶧ �ܺ����ٿ� ������ �Ǿ� �־�� �Ѵ�.
CToolOptionInfo::TOOLOPTION CKTBChartCtrl::GetSavedChartType()
{
	if(m_strChartList.Find( _MTEXT( C2_INVERSE_TIME_LINE)) >= 0)
		return CToolOptionInfo::T_CHART_REVERSE;
	else if(m_strChartList.Find("P&F") >= 0)
		return CToolOptionInfo::T_CHART_PANDF;
	else if(m_strChartList.Find( _MTEXT( C2_THREE_LINE_BREAK)) >= 0)
		return CToolOptionInfo::T_CHART_THREELINEBREAK;
	//sy 2004.05.12. : ������ Ư����Ʈ�� ������ <������Ʈ>�� ÷�εǴ� ��Ʈ�� ������.
	//else if(m_strChartList.Find("���Ź�") >= 0)
	//	return CToolOptionInfo::T_CHART_BARFORSALE;
	return CToolOptionInfo::TOOLOPTION(-1);
}

bool CKTBChartCtrl::ChangeChartType(CString strGraphName, 
										CString strType, 
										CString strStyle,
										CString strPacketNameList)
{
	if(!m_pMainBlock || !m_pIndicatorList)
		return false;

	int nTypeIndex = 0, nStyleIndex = 0, nDrawStyle = 0;
	const CGraphTypeAndStyleData *pGraphTypeAndStyleData = m_pIndicatorList->GetGraphTypeAndStyleData();
	if( !pGraphTypeAndStyleData) return false;
	if( !pGraphTypeAndStyleData->GetGraphTypeAndStyle( strType, strStyle, strStyle, nTypeIndex, nStyleIndex, nDrawStyle)) return false;

	CGraphStyle sourceGraphStyle;
	sourceGraphStyle.SetGraphStyle(nStyleIndex);
	CGraphDrawStyle sourceDrawStyle;
	sourceDrawStyle.SetDrawStyle(nStyleIndex);
	bool bret = m_pMainBlock->ChangeGraphType(strGraphName, (CGraphBaseData::GRAPHTYPE)nTypeIndex, 
		sourceGraphStyle, sourceDrawStyle, strPacketNameList);

	if(bret)
		InvalidateControl();
	else
		ChangeChartItem(strGraphName, 0, "", strPacketNameList, strType, strStyle, 1, 1, 1, TRUE);

	return bret;
}

// (2008/10/13 - Seung-Won, Bae) Do not use MainBlock. It is not Initialized. Use Cfg member variables.
bool CKTBChartCtrl::SaveChartCfg( BOOL p_bDontUseMainBlock)
{
	int nPosition = 0;
	BOOL bDrawBaseLine = TRUE;		// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	if( p_bDontUseMainBlock)
	{
		int nIdxIndicatorNameStart = m_strChartList.Find( "Graph;\r\n") + 8;
		if( 10 <= nIdxIndicatorNameStart)
		{
			nIdxIndicatorNameStart = m_strChartList.Find( ";", nIdxIndicatorNameStart) + 1;
			if( 1 <= nIdxIndicatorNameStart)
			{
				int nIdxIndicatorNameEnd = m_strChartList.Find( ";", nIdxIndicatorNameStart);
				if( 0 <= nIdxIndicatorNameEnd)
				{
					CString strIndicatorName = m_strChartList.Mid( nIdxIndicatorNameStart, nIdxIndicatorNameEnd - nIdxIndicatorNameStart);
					// (2009/2/6 - Seung-Won, Bae) Do check only special Group. if not, do continue;
					if( g_iMetaTable.IsSpecialGroupGraph( strIndicatorName)) return false;
				}
			}
		}
	}
	else
	{
		//sy 2004.12.27. Ư����Ʈ�� �������� �ʴ´�!
		if(IsRunningOneChart())
			return false;

		ResetPropertiesfromMainBlock();
		if(m_pMainBlock)
		{
			nPosition = m_pMainBlock->GetVertScalePosition();
			// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
			bDrawBaseLine = m_pMainBlock->GetDrawBaseLine();
		}
	}

	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	CPropertyMap *pAddInEnvMap = new CPropertyMap;
	if( m_pAddInManager) m_pAddInManager->OnSaveEnvironment( pAddInEnvMap);

	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	if(m_pMainBlock)
	{
		IPropertyMap *pIAddInEnvMap = pAddInEnvMap;
		CBlockIndex maxBlockIndex = m_pMainBlock->GetMaximizedBlock();
		if( maxBlockIndex.IsAllValueBiggerThanZero())
		{
			CString strEnvValue;
			strEnvValue.Format(_T("%d;%d"), maxBlockIndex.GetRow(), maxBlockIndex.GetColumn());
			pIAddInEnvMap->SetAt( _T("MAXBLOCKINDEX"), strEnvValue);
		}
	}

	m_SaveChartcfg.AddChartCfg( pAddInEnvMap,
								m_nBlockRowCount, m_nBlockColumnCount,
								m_nRowDoubleSizeBlock, m_nColumnDoubleSizeBlock,
								m_nOnePageDataCount, m_nBongCountMargin,
								m_strChartList, m_strBlockRegion, 
								IsWholeView(), 
								m_nNumericalInquiryOption,
								nPosition,
								m_nUseInquiryWithCrossline,
								m_bToolTipwithZoomIn,
								m_bUseConditiontoTitle ? TRUE : FALSE,
								m_bUseDatatoTitle ? TRUE : FALSE,
								m_nVertGap, m_nHorzGap,
								m_bShowMinMaxbyPrice, /*m_bUseFullMinMax,*/	//������ �ٸ�
								m_bAllGraphTitleHiding, //sy 2004.11.29. ��� graph title ���� ����
								m_bShowTrendLineText, m_bShowVerticalText,
								m_bShowHorizonTextLeft, m_bShowHorizonTextRight,// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
								m_bShowThreeText, m_bShowFourText, m_bShowFiboText,
								m_clrBackground, m_clrBackBorder,
								m_clrBlock, m_clrBlockBorder,
								m_clrScaleLine, m_clrScaleLine2, m_clrScaleText,
								m_clrAnalysisTool, m_clrInquiryWindow, m_clrLattice,
								// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
								m_bShowPriceMinMaxText,
								// (2006/12/10 - Seung-Won, Bae) Support Map Default Info.
								m_strMapIndicatorInfo,
								bDrawBaseLine,		// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
								-1,
								m_nRightMarginWidth,		// (2009/5/13 - Seung-Won, Bae) for Right Margin
								m_nFutureTimeCount,			// (2009/5/17 - Seung-Won, Bae) for Future Time
								m_nMouseWheelIndex,
								m_nMinMaxRatio
								);
	return true;
}

//sy 2005.01.14.
// nOption = -1 : ���� �ʱ�ȭ
// nOption = 1 : �Ѵܰ� ������
// nOption = 0 : ���Ͽ� �ִ� ���·�
bool CKTBChartCtrl::ResetSaveChartCfg(int nOption, bool bRegionRetain)
{
	return ResetSaveChartCfg(GetChartCfg(nOption), bRegionRetain);	
}

bool CKTBChartCtrl::ResetSaveChartCfg(CChartCfg* pChart, const bool bRegionRetain)
{
	if(!SetOcxDataFromChartCfg(pChart, bRegionRetain))
		return false;

	//sy 2006.04.19. -> ȭ���� scroll ���� �κ� ������ �ؾ� �Ѵ�.
	// -> MainBlock�� �ݿ��� �� scroll �ݿ��ϱ� : MainBlock�� x�� scale data �� ���� ȭ�鿡 ���� ���� �����ϱ� ����.
	SetMainBlock(IsRunTimeMode());
	return true;	
}

//sy 2006.03.02
CChartCfg* CKTBChartCtrl::GetChartCfg(const int nOption)
{
	// JSR 20060119 START : ��Ƽ��Ʈ������ ����ó���ѳ��� �������� �����
	//if(m_hContainerDllWnd) 
	//	return GetChartCfgWithPacketReset(nOption);
	// JSR 20060119 End

	//���� �ʱ� ȭ��(Mapȭ��)
	if(nOption < 1)
		return m_SaveChartcfg.GetChartCfg(0, true);
	
	//�� ������ ����Ÿ ������..
	if(nOption == 1)
		return m_SaveChartcfg.GetChartCfg(m_SaveChartcfg.GetCfgCount() - 2, true);
	
	//�� ������ ����Ÿ
	//ȭ�鿡 Ư����Ʈ�� �������..Ư����Ʈ�� m_SaveChartcfg�� ����Ǿ� ���� �ʴ�.
	if(nOption == 2)
		return m_SaveChartcfg.GetChartCfg(m_SaveChartcfg.GetCfgCount() - 1, true);
	
	return NULL;
}

bool CKTBChartCtrl::SetOcxDataFromChartCfg(CChartCfg* pChart, bool bRegionRetain)
{
	if(pChart == NULL)
		return false;

	m_nBlockRowCount = pChart->GetRowCount();
	m_nBlockColumnCount = pChart->GetColumnCount();
	m_nRowDoubleSizeBlock = pChart->GetRowDoublesizeBlock();
	m_nColumnDoubleSizeBlock = pChart->GetColumnDoublesizeBlock();
	m_nOnePageDataCount = pChart->GetOnePageDataCount();
	m_nBongCountMargin = pChart->GetBongCountMargin();
	//nami 9.10			
	m_strChartList = pChart->GetChartList();
	// region ������ ���Ͽ�
	bool bRetain = false;
	CString strTempBlockRegion;
	if(bRegionRetain && m_pMainBlock)
	{
		if(m_nBlockRowCount == m_pMainBlock->GetRowCount()
			&& m_nBlockColumnCount == m_pMainBlock->GetColumnCount())
		{
			bRetain = true;
			strTempBlockRegion = m_pMainBlock->GetBlocksPosition();
		}
	}
	if(bRetain)
		m_strBlockRegion = strTempBlockRegion;
	else
		m_strBlockRegion = pChart->GetBlockRegion();
	m_bWholeView = pChart->IsWholeView();
	m_clrBackground = pChart->GetclrBackground();
	m_clrBackBorder = pChart->GetclrBackBorder();
	m_clrBlock = pChart->GetclrBlock();
	m_clrBlockBorder = pChart->GetclrBlockBorder();
	m_clrScaleLine = pChart->GetclrScaleLine();
	m_clrScaleLine2 = pChart->GetclrScaleLine2();
	m_clrScaleText = pChart->GetclrScaleText();
	m_clrAnalysisTool = pChart->GetclrAnalysisTool();
	m_clrInquiryWindow = pChart->GetclrInquiryWindow();
	m_clrLattice = pChart->GetclrLattice();

	//sy 2005.12.01 drag ���� ����(��ġ��ȸ/Ȯ�����)
	// (2006/2/8 - Seung-Won, Bae) Not used m_nNumericalInquiryDlgType
	//	m_nNumericalInquiryDlgType = pChart->GetNumericalInquiryDlgType();
	//	m_nInquiryType = pChart->GetInquiryType(); 
	m_nNumericalInquiryOption = pChart->GetNumericalInquiryOption();

	// 25.5 ��ġ��ȸâ Type, LBT�ɼǿ� ���� ��ġ��ȸâ Type�� �ٷ� �����Ų��.
	// ����(05/05/12) ��ġ��ȸâ �̵����¸� �����Ѵ�.
	SetNumericalInquiryOption( m_nNumericalInquiryOption, false);
	//sy end

	m_bUseConditiontoTitle = pChart->UseConditiontoTitle();
	m_bUseDatatoTitle = pChart->UseDatatoTitle();
	m_nVertGap = pChart->GetVertGap();
	m_nHorzGap = pChart->GetHorzGap(); 

	//nami 9.10
	m_nUseInquiryWithCrossline = pChart->GetUseInquiryWithCrossline();
	m_bToolTipwithZoomIn = pChart->UseToolTipwithZoomIn();

	m_bShowMinMaxbyPrice = pChart->showMinMaxbyPrice(); 
	
	//sy 2004.11.29. ��� graph title ���� ����
	m_bAllGraphTitleHiding = pChart->IsAllGraphTitleHiding();

	m_bShowTrendLineText = pChart->showTrendLineText(); 
	/// cj 1�� 22��
	m_bShowVerticalText = pChart->showVerticalText(); 
	// ����(04/10/14) ����Text�� ����, ������ �����ؼ� ���̱�.
	m_bShowHorizonTextLeft = pChart->showHorizonTextLeft(); 
	m_bShowHorizonTextRight = pChart->showHorizonTextRight();
	m_bShowThreeText = pChart->showThreeText(); 
	m_bShowFourText = pChart->showFourText(); 
	m_bShowFiboText = pChart->showFiboText(); 
	//nami end

	// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
	m_bShowPriceMinMaxText = pChart->WithShowPriceMinMaxText();

	// (2006/12/10 - Seung-Won, Bae) Support Map Default Info.
	m_strMapIndicatorInfo = pChart->GetMapIndicatorInfo();

	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	if( m_pAddInManager) m_pAddInManager->OnLoadEnvironment( pChart->GetAddInEnvMap());

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	for Old Version Map and User File.
    m_nVertScaleType = pChart->GetVertScaleType();

	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	if(m_pMainBlock) 
	{
		m_pMainBlock->SetDrawBaseLine(pChart->GetDrawBaseLine());
		
		// �� �ִ�ȭ �߰� - ojtaso (20080519)
		IPropertyMap* pIAddInEnvMap = pChart->GetAddInEnvMap();
		const char *szData = NULL;
		if( pIAddInEnvMap->Lookup( _T("MAXBLOCKINDEX"), szData))
		{
			int nRow = -1, nColumn = -1;
			::sscanf( szData, _T("%d;%d"), &nRow, &nColumn);
			m_pMainBlock->SetMaximizedBlock( CBlockIndex( nRow, nColumn));
		}
	}

	// (2009/5/13 - Seung-Won, Bae) for Right Margin
	m_nRightMarginWidth = pChart->GetRightMarginWidth();

	// (2009/5/17 - Seung-Won, Bae) for Future Time
	m_nFutureTimeCount = pChart->GetFutureTimeCount();
	m_nMouseWheelIndex = pChart->GetMouseWheelIndex();
	m_nMinMaxRatio = pChart->GetPriceMinMaxShowType();

	return true;
}
//sy end

BOOL CKTBChartCtrl::IsRunningOneChart( void)
{
	//sy 2005.06.20.
	if(m_pMainBlock == NULL)
		return false;

	//OneChart ��� �ϸ� ������ ù��° Block�� �߰� �Ǳ� ������ ù��° Block�� function name���� ���ϸ� �ȴ�.
	CString strIndicatorName = m_pMainBlock->GetIndicatorName(CBlockIndex(0, 0), 0, 0);
	// "�Ｑ��ȯ��", "���ð�", "P&F", "SwingChart"
	return g_iMetaTable.IsSpecialGroupGraph(strIndicatorName);
}

// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
BOOL CKTBChartCtrl::IsRunningNoZoomChart( void)
{
	//sy 2005.06.20.
	if(m_pMainBlock == NULL)
		return false;

	//OneChart ��� �ϸ� ������ ù��° Block�� �߰� �Ǳ� ������ ù��° Block�� function name���� ���ϸ� �ȴ�.
	CString strIndicatorName = m_pMainBlock->GetIndicatorName(CBlockIndex(0, 0), 0, 0);
	return g_iMetaTable.IsNoneZoomChart(strIndicatorName);
}

//sy 2002.11.23.
void CKTBChartCtrl::CreateBufferBitmap(CDC* pdc, const CRect& rcBounds)
{
	if (m_bmpMainOSB.GetSafeHandle() == NULL)
	{
		m_bmpMainOSB.CreateCompatibleBitmap(pdc, rcBounds.Width(), rcBounds.Height());
		// (2009/1/13 - Seung-Won, Bae) Main Image Cache.
		m_bMainOSBFilled = FALSE;
	}
	else
	{
		BITMAP bmp;
		m_bmpMainOSB.GetBitmap( &bmp);
		CSize boundsSize = rcBounds.Size();
		if( bmp.bmWidth < boundsSize.cx || bmp.bmHeight < boundsSize.cy)
		{
			m_bmpMainOSB.DeleteObject();
			m_bmpMainOSB.CreateCompatibleBitmap(pdc, boundsSize.cx, boundsSize.cy);
			// (2009/1/13 - Seung-Won, Bae) Main Image Cache.
			m_bMainOSBFilled = FALSE;
		}
	}
}

// (2004.02.16, ��¿�, �м�) X Scale�� Double Click�Ͽ� ȯ�ἳ�� Dialog�� ��ﶧ,
//		������ User Margin�� ��ȸ�ϴ� Message Routine�̴�. (ChartObject DLL�� CHorzScaleTool::OnLButtonDblClk() ����)
LRESULT CKTBChartCtrl::OnGetUserMargin(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_nBongCountMargin;
}	

void CKTBChartCtrl::GetIndicatorDefaultData(CIndicatorDefaultData& indiDefaultData) const
{
	if(m_pMainBlock == NULL)
		return;

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	indiDefaultData.SetBaseScaleGridType(m_pMainBlock->GetBaseScaleGridType());
	indiDefaultData.SetVertScalePosition(m_pMainBlock->GetVertScalePosition());
}

