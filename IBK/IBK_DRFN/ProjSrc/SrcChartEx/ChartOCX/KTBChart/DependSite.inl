#include "PrintController.h"			// for CPrintController
#include "CfgFileVersionChecker.h"		// for CCfgFileVersionChecker

#define FILE_INDICATOR_CONDITIONS_USER "IndiCond_"

// (2006/4/14 - Seung-Won, Bae) Support OpenFromFile with Auto Saving Option.
BOOL CKTBChartCtrl::OpenFromFile(LPCTSTR p_szFileName, BOOL p_bAutoPrevSave) 
{
	// TODO: Add your dispatch handler code here
	CString strSaveFN;
	if( p_bAutoPrevSave) strSaveFN = m_strUserEnvironmentFileName;
	return SaveandOpenFromFileWithMSave( p_szFileName, strSaveFN, TRUE, TRUE);
}

#include "DumpLogger.h"									// for CDumpLogger
static int g_nSaveAndOpenFileCount = 0;
// 02.26.2002  ������ ȭ�� ������ ���Ͽ� �����ϰ� ������ ������ �о ȭ���� �ٽ� �����Ѵ�
// 03.07.2002  modified indicator list save
BOOL CKTBChartCtrl::SaveandOpenFromFileWithASave(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
{
	// TODO: Add your dispatch handler code here
	CString strSaveFN = strSaveFileName;
	if( bSaveOption && strSaveFN.IsEmpty()) strSaveFN = m_strUserEnvironmentFileName;
	return SaveandOpenFromFileWithMSave( strOpenFileName, strSaveFN, bOpenOption, bSaveOption);
}
// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
BOOL CKTBChartCtrl::WriteUserFiles( void)
{
	// save environment 
	WriteUserFile(CSaveUserFile::ENVIRONMENT);
	// save indicator option		
	WriteUserFile(CSaveUserFile::INDICATORLIST);
	return TRUE;
}
BOOL CKTBChartCtrl::ReadUserFiles( void)
{
	// (2009/5/12 - Seung-Won, Bae) Skip Indicator Calcuatation during adding the graph.
	BOOL bCurrentEnableIndicatorCalculation = GetBEnableIndicatorCalculation();
	SetBEnableIndicatorCalculation( FALSE);

	// initialize
	//sy 2004.12.07. Ư��&�Ϲ���Ʈ ����� ���õ� �κ�.
	//	InvalidateChart(); -> ResetSaveChartCfg(0); -> ResetSaveChartCfg( GetChartCfg( 0), false);	
	//sy 2006.04.19. -> ȭ���� scroll ���� �κ� ������ �ؾ� �Ѵ�.
	// -> MainBlock�� �ݿ��� �� scroll �ݿ��ϱ� : MainBlock�� x�� scale data �� ���� ȭ�鿡 ���� ���� �����ϱ� ����.
	if( SetOcxDataFromChartCfg( GetChartCfg( 0), false)) SetMainBlock(IsRunTimeMode());

	// (2006/12/10 - Seung-Won, Bae) Remove AllGraph for New Loadiong.
	//		in Graph Deleted, Indicaotr Info can be lost.
	m_pMainBlock->DeleteAllBlock();

	// Indicator�Ӽ��� ���� �о�� �Ѵ�. -> user version�� �������� ����!
	// read indicator list config file
	BOOL bIndiUserFile = ReadUserFile(CSaveUserFile::INDICATORLIST);

	// read environment config file
	BOOL bChartUserFile = ReadUserFile(CSaveUserFile::ENVIRONMENT);
	if(bChartUserFile)
	{
		SetMainBlock( IsRunTimeMode(), FALSE, FALSE);
		//sy 2004.2.13.
		UserFileVersionChecker();

		m_pMainBlock->ChangeAllIndicatorInfoFromGraphData();

		// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
		//	for Old Version Map and User File.
		if( 0 <= m_nVertScaleType) m_pMainBlock->SetAllVertScaleGridType( (CScaleBaseData::SCALEGRIDTYPE)m_nVertScaleType);
	}

	// (2009/5/12 - Seung-Won, Bae) Skip Indicator Calcuatation during adding the graph.
	SetBEnableIndicatorCalculation( bCurrentEnableIndicatorCalculation);
	if( bIndiUserFile || bChartUserFile)
	{
		// ����(04/11/16) ���ǹ� ����
		// ����(04/09/07) SetScrollData�κ� ����
		ResetScroll();
		InvalidateControl();
	}

	//sy end
	return TRUE;
}
BOOL CKTBChartCtrl::SaveandOpenFromFileWithMSave(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption) 
{
	if( !m_pMainBlock) return FALSE;

	// TODO: Add your dispatch handler code here
	CString strSaveFileCopy, strOpenFileCopy;
	if( m_bEnableLogSaveAndOpenFromFile)
	{
		CString strLog, strItem;
		if( bSaveOption)
		{
			strSaveFileCopy.Format( "ChartFile%04dSave.cht", g_nSaveAndOpenFileCount++);
			strItem.Format( "%s - Save File Name : [%s]\r\n", strSaveFileCopy, strSaveFileName);
			strLog += strItem;
		}
		if( bOpenOption)
		{
			strOpenFileCopy.Format( "ChartFile%04dOpen.cht", g_nSaveAndOpenFileCount++);
			strItem.Format( "%s - Open File Name : [%s]\r\n", strOpenFileCopy, strOpenFileName);
			strLog += strItem;
		}
		if( strLog.IsEmpty()) strLog.Format( "No Oeration\r\n%s,%s,%d,%d\r\n", strOpenFileName, strSaveFileName, bOpenOption, bSaveOption);
		CDumpLogger::LogStringToFile( "CSfnChartCtrl::SaveandOpenFromFileWithMSave()", strLog, strLog.GetLength(), "SaveandOpenFromFileWithMSave");
	}

	CString strOpenFN = strOpenFileName, strSaveFN = strSaveFileName;
	if(strOpenFN.IsEmpty() && bOpenOption)
		return FALSE;

	// save
	// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
	if( bSaveOption && !strSaveFN.IsEmpty() && !m_bFreezedPageState)
	{
		//sy 2005.07.29.
		SetUserEnvironmentAndIndicatorListFileName(strSaveFN);
		//m_strUserEnvironmentFileName = strSaveFN;
		//m_strUserIndicatorListFileName = FILE_INDICATOR_CONDITIONS_USER + m_strUserEnvironmentFileName;	
		//sy end

		// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
		WriteUserFiles();

		if( m_bEnableLogSaveAndOpenFromFile)
			CopyFile( GetUserFilePathAndName( m_strUserEnvironmentFileName), "C:\\Logs\\" + strSaveFileCopy, FALSE);
	}
	// open
	if(bOpenOption && !strOpenFN.IsEmpty()) if( SetUserEnvironmentAndIndicatorListFileName( strOpenFN))
	{
		// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
		ReadUserFiles();

		if( m_bEnableLogSaveAndOpenFromFile)
			CopyFile( GetUserFilePathAndName( m_strUserEnvironmentFileName), "C:\\Logs\\" + strOpenFileCopy, FALSE);
	}

	return TRUE;
}

//sy 2005.07.29.
BOOL CKTBChartCtrl::SetUserEnvironmentAndIndicatorListFileName(const CString& strFileName)
{
	if(strFileName.IsEmpty())
		return FALSE;

	m_strUserEnvironmentFileName = strFileName;
	// Ȯ���ڰ� �ִ��� Ȯ��
	// -> ���� ��� : ".cht" ������
	if(strFileName.Find(".") < 0)
		m_strUserEnvironmentFileName += ".cht";

	m_strUserIndicatorListFileName = FILE_INDICATOR_CONDITIONS_USER + m_strUserEnvironmentFileName;

	// (2007/1/9 - Seung-Won, Bae) Check if Valid File
	BOOL bNotFound = FALSE;
	WIN32_FIND_DATA findFileData;
	CString strUserFile = GetUserFilePathAndName( m_strUserEnvironmentFileName);
	HANDLE hFile = FindFirstFile( strUserFile, &findFileData);
	if( hFile != INVALID_HANDLE_VALUE) FindClose( hFile);
	else bNotFound = TRUE;
	strUserFile = GetUserFilePathAndName( m_strUserIndicatorListFileName);
	hFile = FindFirstFile( strUserFile, &findFileData);
	if( hFile != INVALID_HANDLE_VALUE) FindClose( hFile);
	else bNotFound = TRUE;
	return !bNotFound;
}
//sy end

//sy 2004.2.13.
void CKTBChartCtrl::UserFileVersionChecker()
{
	// user file �� Indicond.cfg�� ���� üũ�� ����.
	// ��, Indicond���� user �� IndicatorCondition.dll ���� üũ��
	// ���⼭�� MainBlock�� ���õ� ���븸 �����ϸ� �ȴ�.
	bool bIsCalculate = false;
	BOOL bResult = CCfgFileVersionChecker().UserFileVersionChecker((CKTBChartCtrl*) this, m_pMainBlock, bIsCalculate);
	if(!bResult)
		return;

	// ������ Ʋ����� �ѹ��̶� ��ǥ�� ������ ��� �Ǿ� ������ packet�� Ȯ������ �ʱ� ������ dll�� �ٽ� �ε��Ѵ�.
	if(bIsCalculate && m_pAddInManager != NULL)
	{
		// (2006/8/25 - Seung-Won, Bae) ST Reload for Dynamic Graph Reformming.
		//		Cuation. ST Indicator DLL's Re-Initializing Interface is required for error of drawing or mouse order.
		if( m_pAddInManager->IsAddInLoaded(	g_aszAddInLoadString[ AID_ST_INDICATOR]))
		{
			m_pAddInManager->UnloadAddIn(	g_aszAddInLoadString[ AID_ST_INDICATOR]);
			m_pAddInManager->LoadAddIn(		g_aszAddInLoadString[ AID_ST_INDICATOR]);
		
			OnPacketTRData();
		}
	}
}
//sy end


// 03.01.2002
// Block ���� �������ִ� �޽�� -> ����Ʈ�� ���ؼ� �׷����� �ϳ��� ������ �ʴ´�
BOOL CKTBChartCtrl::JoinandDevisionforCompareChart(long nRowCount, long nColumnCount, BOOL bJoin) 
{
	if(!m_pMainBlock)	return FALSE;
	int i = 0;
	// ���� ������ �� ������
	if(nRowCount > m_nBlockRowCount)
	{
		// �ϴ� ������ �� ���δ�
		CString strAllData;
		m_pMainBlock->GetAllBlocksData( strAllData);
		m_pMainBlock->SetBlockCount(CBlockIndex(nRowCount, nColumnCount));
		m_pMainBlock->BuildGraphsAndScales(strAllData);
		// ù��° Block�� �ִ� ��Ʈ�� �߰��� Block���� �ű��
		for( i = 0 ; i < m_nBlockRowCount ; i++)
			m_pMainBlock->SetHorzScalePosition(CBlockIndex(i, 0), CScaleBaseData::HORZ_HIDE);
		for(i = m_nBlockRowCount ; i < nRowCount ; i++)	{
			// scale group  �̵� -> scale group�� �ϳ��̸� graph �̵�
			if(!m_pMainBlock->MoveGraph(CBlockIndex(0, 0), 1, 0, CBlockIndex(i, 0)))
				m_pMainBlock->MoveGraph(CBlockIndex(0, 0), 0, 1, CBlockIndex(i, 0));				
			// ���� scale ����
			m_pMainBlock->SetHorzScaleData(CBlockIndex(i, 0), _MTEXT( C0_DATE_TIME));
			m_pMainBlock->SetHorzScalePosition(CBlockIndex(i, 0), CScaleBaseData::HORZ_HIDE);
		}
		m_pMainBlock->SetHorzScaleCompart(CBlockIndex(nRowCount - 1, 0), "/", ":");
		m_pMainBlock->SetHorzScalePosition(CBlockIndex(nRowCount - 1, 0), CScaleBaseData::HORZ_BOTTOM);
	}
	// ���� ������ �� ������
	else if(nRowCount < m_nBlockRowCount)	{
		// ������ Block�� �ִ� ��Ʈ�� �� �� block���� �ű��
		for( i = nRowCount ; i < m_nBlockRowCount ; i++)	{
			if(bJoin)	
				m_pMainBlock->MoveAllGraphs(CBlockIndex(nRowCount, 0), 0, CBlockIndex(0, 0), 0);
			else
				m_pMainBlock->MoveAllVertScaleGroups(CBlockIndex(nRowCount, 0), CBlockIndex(0, 0));
		}
		m_pMainBlock->SetHorzScalePosition(CBlockIndex(nRowCount - 1, 0), CScaleBaseData::HORZ_BOTTOM);
	}
	else if(nRowCount == m_nBlockRowCount)	{
		for( i = 0 ; i < nRowCount ; i++)	{
			m_pMainBlock->JoinAllVertScaleGroups(CBlockIndex(i, 0), CDataConversion::atob(bJoin));
		}
	}
	m_nBlockRowCount = nRowCount;
	m_nBlockColumnCount = nColumnCount;

	//sy 2006.03.02
	SetOneChart();

	InvalidateControl();
	return TRUE;
}

//sy 2006.03.02
void CKTBChartCtrl::SetOneChart()
{
	if(m_pToolBarManager == NULL)
		return;

	// ����(05/01/04) Ư����ǥ�϶� ���������, �Ϲ���Ʈ������ ��ȯ�� SliderCtrl�� Disable�Ǵ� ���� ����
	// (2005.01.12, ��¿�) Design Time�ÿ� ToolBar�� ���� ��츦 �����Ѵ�.
	if(IsRunningOneChart())
	{
		// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
		m_pToolBarManager->SetOneChart(true, IsRunningNoZoomChart());
		// OneChart���� : �������� - ojtaso (20070627)
		m_pPacketListManager->SetOneChart(TRUE, m_strRQ);
	}
	else
	{
		m_pToolBarManager->SetOneChart(false, false);
		// OneChart���� : �������� - ojtaso (20070627)
		m_pPacketListManager->SetOneChart(FALSE, m_strRQ);
	}

	ResetScroll( TRUE);
}
//sy end

// 03.19.2002  Menu : Display ���� �ʱ�ȭ
void CKTBChartCtrl::OnReturnOrgDisplay() 
{
	CChartCfg * pChart = m_SaveChartcfg.GetChartCfg(0);
	if(!pChart)
		return;

	m_bWholeView = pChart->IsWholeView();
	SetOnePageDataCount(pChart->GetOnePageDataCount());
	ResetScroll();

	// (2009/2/19 - Seung-Won, Bae) Notify to Container for user changing.
	OnUserZoomChanged( m_nOnePageDataCount);
}

// 03.20.2002  Change Title
BOOL CKTBChartCtrl::ChangeGraphTitle(short nRowIndex, short nColumnIndex, short nGraphIndex, LPCTSTR strNewTitle2) 
{
	if(!m_pMainBlock)	return FALSE;

	CString strNewTitle = _STOM1( strNewTitle2);

	CString strName = strNewTitle;
	strName.TrimRight();
	BOOL b = m_pMainBlock->ChangeSubGraphTitle(CBlockIndex(nRowIndex, nColumnIndex), 0, nGraphIndex, 0, strName);
	if(!b)	{
		if(m_pMainBlock->ChangeSubGraphTitle(CBlockIndex(nRowIndex, nColumnIndex), nGraphIndex, 0, 0, strName))	{
			InvalidateControl();
			return TRUE;
		}
		else
			return FALSE;
	}
	InvalidateControl();
	return TRUE;
}

// 03.20.2002  Menu : �μ�
void CKTBChartCtrl::OnPrint() 
{
	CPrintController controller;
	controller.OnPrint(m_rcMain, this);
}

// 03.22.2002  graph�� select�Ǿ��� �� ó��
LRESULT CKTBChartCtrl::OnGraphSelectedIndex(WPARAM wParam, LPARAM lParam)
// wparam : data index
// lparam : indicator function name
{
	int nDataIndex = (int)wParam;
	CString strGraphName = (LPCTSTR)lParam;

	// �ش� index�� ��¥�� ���´�
	int nDate = (int)m_pPacketList->GetDataFromIndex( _MTEXT( C0_DATE_TIME), nDataIndex);
	CString strDate = CDataConversion::IntToString(nDate);
	// news �϶� �̺�Ʈ �߻�
	if(strGraphName == "News")
		NeoRequestMapLoad(3, "", strDate);
	// �ŷ������϶� �̺�Ʈ �߻�
	else if(strGraphName == _MTEXT( C2_DEAL_LIST))
		NeoRequestMapLoad(4, "", strDate);
//	CString msg;
//	msg.Format("%s, %d, %s", strGraphName, nDataIndex, strDate);
//	AfxMessageBox(msg);
	return 1L;
}

// 03.26.2002  ���Ͽ��� ����Ÿ �б�
#include <io.h>
short CKTBChartCtrl::SetChartDataFromFile(LPCTSTR szFileName, short nType) 
{
	//sy 2003.8.19.
	CString strFilePathAndName = GetDefaultFilePathAndName(szFileName);
	if(strFilePathAndName.IsEmpty())
		return FALSE;

	FILE* pStream = fopen(strFilePathAndName, "rb");
	if(pStream == NULL)
		return FALSE;

	//fseek(pStream, 0L, SEEK_SET);
	//long nbytes = _filelength(pStream->_file);

	fseek(pStream, 0L, SEEK_END);//tour2k
	long nbytes = ftell(pStream);
	fseek(pStream, 0L, SEEK_SET);

	char* buffer = new char[nbytes + 1];
	ZeroMemory(buffer, nbytes + 1);
	fread(buffer, sizeof(char), nbytes, pStream);
    fclose(pStream);

	SetChartDataStr((CString)buffer, 0, nType);
	delete[] buffer;
	return 1;
}

// 04.03.2002  Graph ����
BOOL CKTBChartCtrl::RemoveChart( LPCTSTR szIndicatorName2) 
{
	if(!m_pMainBlock) return FALSE;

	CString szIndicatorName = _STOM2( szIndicatorName2);
	if( !m_pMainBlock->DeleteAllIndicator( szIndicatorName)) return FALSE;
	InvalidateControl();
	return TRUE;
}

// (2007/1/20 - Seung-Won, Bae) Remove Graph with Graph Name.
BOOL CKTBChartCtrl::RemoveGraphWithGraphName( const char *p_szGraphName)
{
	if(!m_pMainBlock) return FALSE;
	if( !m_pMainBlock->DeleteGraph( p_szGraphName)) return FALSE;
	InvalidateControl();
	return TRUE;
}

// 04.03.2002  toolbar : ��ü �ִ��ּ� ����
void CKTBChartCtrl::OnFullMinMax(int nOption)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(m_pRscFont);
	m_pMainBlock->SetAllVScaleMinMaxType( nOption == 1 ? CScaleBaseData::VERT_MINMAX_OF_FULL : CScaleBaseData::VERT_MINMAX_OF_VIEW, &dc);
	dc.SelectObject(pOldFont);
	InvalidateControl();
}

// 04.11.2002   Method : graph�� Ÿ�Ը��� �ٲ۴�
BOOL CKTBChartCtrl::ChangeGraphType(LPCTSTR strGraphName2, LPCTSTR strGraphType2, LPCTSTR strGraphStyle2, LPCTSTR strPacketNameList2, BOOL bRunatOnce) 
{
	CString strGraphName = _STOM2( strGraphName2);
	CString strGraphType = _STOM5( strGraphType2);
	CString strGraphStyle = _STOM5( strGraphStyle2);
	CString strPacketNameList = _STOMS( strPacketNameList2, ",;", 0);

	// (2004.11.05, ��¿�) m_infoChangeStyle�� ������ �ϰ� �����ϴ� Routine�� ��� �ּ�ó���Ѵ�.
//	if(bRunatOnce)
//	{
//		m_infoChangeStyle.bUse = FALSE;
		ChangeChartType(strGraphName, strGraphType, strGraphStyle, strPacketNameList);		
//	}
//	else	{
//		m_infoChangeStyle.bUse = TRUE;
//		m_infoChangeStyle.bWait = TRUE;
//		m_infoChangeStyle.szGraphName = strGraphName;
//		m_infoChangeStyle.szGraphType = strGraphType;
//		m_infoChangeStyle.szGraphStyle = strGraphStyle;
//		m_infoChangeStyle.szPacketNameList = strPacketNameList;
//	}
	return TRUE;
}

bool CKTBChartCtrl::OnReturnOrg2(const BOOL bIsMessageBox)
{
	//sy 2004.05.12.
	if(bIsMessageBox)
	{
		ML_SET_LANGUAGE_RES();
		CString strTitle;
		strTitle.LoadString( IDS_CHART_SETUP);
		CString text;
		text.LoadString( IDS_RESET_CHART);
		int nOk = MessageBox(text, strTitle, MB_OKCANCEL | MB_ICONQUESTION);
		if(nOk != IDOK)
			return false;
	}

	// (2004.08.12, ��¿�) �м������� ��� Clear�Ѵ�.
	RunToolFunction( CToolOptionInfo::T_DELETE_ALL, 3);
		
	// (2004.05.20, ��¿�, �м�) Indicator List�� User������ Clear��Ű��,
	//		MainBlock�� �����ϴ� Indicator Info Pointer ������ ���� Clear���Ѿ� �Ѵ�.
	//		�׷��� ������ Pointer ������ �߻��ȴ�.
	if(m_pIndicatorList == NULL)
		return false;
	
	// xScale �ʱ�ȭ : xScaleManager - ojtaso (20070628)
	if(m_pxScaleManager)
		m_pxScaleManager->Initialize();

	m_pIndicatorList->RemoveAllIndicatorInfo( EILT_USER);

	//sy 2006.03.02
	//sy 2003.3.7 - �ӵ� ����
	//IndicatorInfo �� ���ؼ� MainBlock�� ������ �ִ� info�� �ּҵ� �ʱ�ȭ
	if(m_pMainBlock != NULL) 
	{
		m_pMainBlock->Initialize_IndicatorInfo(true);
		// �� �ִ�ȭ �߰� - ojtaso (20080519)
		m_pMainBlock->SetMaximizedBlock( CBlockIndex( -1, -1));
	}

	// �ʱ� ���¸� �ҷ��ͼ� �����Ѵ�
	//sy 2004.12.07. Ư��&�Ϲ���Ʈ ����� ���õ� �κ�.
	ResetSaveChartCfg( 0);
	//sy end

	// 2008.10.31 JS.Kim  ����ڰ� ������ �ð��� �ʱ�ȭ
	m_pPacketListManager->ResetTimeDiff();

	// (2004.10.11, ��¿�) AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	if( m_pAddInManager) m_pAddInManager->OnResetChart();

	NeoReturnOrgChart(m_nOnePageDataCount); //sy 2005.12.05

	return true;
}

void CKTBChartCtrl::ReplaceMATitle(CString &strma, const int nOption)
{
	if(nOption == 0 && strma.Find( CString( _MTEXT( C3_PRICE_MA_1)) + ";" + _MTEXT( C3_PRICE_MA_1)))
	{
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_1)) + ";" + _MTEXT( C3_PRICE_MA_1), CString( _MTEXT( C3_PRICE_MA_1)) + ";5");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_2)) + ";" + _MTEXT( C3_PRICE_MA_2), CString( _MTEXT( C3_PRICE_MA_2)) + ";10");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_3)) + ";" + _MTEXT( C3_PRICE_MA_3), CString( _MTEXT( C3_PRICE_MA_3)) + ";20");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_4)) + ";" + _MTEXT( C3_PRICE_MA_4), CString( _MTEXT( C3_PRICE_MA_4)) + ";60");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_5)) + ";" + _MTEXT( C3_PRICE_MA_5), CString( _MTEXT( C3_PRICE_MA_5)) + ";120");
		strma.Replace( CString( _MTEXT( C3_PRICE_MA_6)) + ";" + _MTEXT( C3_PRICE_MA_6), CString( _MTEXT( C3_PRICE_MA_6)) + ";240");
	}
	else if(nOption == 1 && strma.Find( CString( _MTEXT( C3_VOLUME_MA_1)) + ";" + _MTEXT( C3_VOLUME_MA_1)))	{
		strma.Replace( CString( _MTEXT( C3_VOLUME_MA_1)) + ";" + _MTEXT( C3_VOLUME_MA_1), CString( _MTEXT( C3_VOLUME_MA_1)) + ";5");
		strma.Replace( CString( _MTEXT( C3_VOLUME_MA_2)) + ";" + _MTEXT( C3_VOLUME_MA_2), CString( _MTEXT( C3_VOLUME_MA_2)) + ";20");
		strma.Replace( CString( _MTEXT( C3_VOLUME_MA_3)) + ";" + _MTEXT( C3_VOLUME_MA_3), CString( _MTEXT( C3_VOLUME_MA_3)) + ";60");
		strma.Replace( CString( _MTEXT( C3_VOLUME_MA_4)) + ";" + _MTEXT( C3_VOLUME_MA_4), CString( _MTEXT( C3_VOLUME_MA_4)) + ";120");
	}
}
