// KTBChartDLL.cpp: implementation of the CKTBChartDLL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../AddInManager/Include_AddIn/AddInCommand.h"		// for EAI_CUSTOM_CLEAR_TIMEMARK
#include "OutsideCommonInterface.h"							// for RMSG_CHARTMSG_EVENTMESSAGE
#include "KTBChartCtl.h"									// for CKTBChartCtrl
#include "IndicatorGroupData.h"								// for CIndicatorGroupDataList
#include "IndicatorList.h"									// for CIndicatorList
#include "Color.h"											// for CGraphRegionColor
#include "Conversion.h"										// for CDataConversion
#include "IndicatorDefaultData.h"							// for CIndicatorDefaultData
#include "IndicatorInfo.h"									// for CIndicatorInfo
#include "ChartCfg.h"										// for CChartCfg
#include "MainBlock.h"										// for CMainBlock
#include "PacketList.h"										// for CPacketList
#include "ToolBarManager.h"									// for CToolBarManager
#include "XScaleManager.h"									// for CXScaleManager
#include "PacketListManager.h"								// ���� PacketList�� ���� : �������� - ojtaso (20070109)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.05.06, ��¿�) DLL Container�� Window Handle�� Chart�� ���� ID�� �����Ͽ� Event Notify�� �����ϵ��� �Ѵ�.
// (2004.10.08, ��¿�) SendEventMsgToDLL()�� �̿� ���̸� ���Ͽ� Filtering���� �ʴ´�.
const UINT RMSG_CHARTMSG_EVENTMESSAGE	= ::RegisterWindowMessage( _CHARTMSG_EVENTMESSAGE);
LRESULT CKTBChartCtrl::SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	msg.hwnd	= GetSafeHwnd();
	msg.message	= message;
	msg.wParam	= wParam;
	msg.lParam	= lParam;
	msg.time	= 0;
	msg.pt.x	= 0;
	msg.pt.y	= 0;
	return ::SendMessage( m_hContainerDllWnd, RMSG_CHARTMSG_EVENTMESSAGE, m_nID, ( LPARAM)&msg);
}

// (2004.05.06, ��¿�) DLL Container�� Window Handle�� Chart�� ���� ID�� �����Ͽ� Event Notify�� �����ϵ��� �Ѵ�.
void CKTBChartCtrl::SetContainerDllInfo(long p_lDllWnd, short p_nID) 
{
	// TODO: Add your dispatch handler code here
	m_hContainerDllWnd	= ( HWND)p_lDllWnd;
	m_nID				= p_nID;
}

// (2004.05.04, ��¿�) ��Ʈ���� �����ϴ� ��ǥ�� �̸����� ���Ͽ�
//		�ܺ� Module���� �˷��ִ� Interface�� �����Ѵ�.
//		1. Parameter
//			LPCTSTR p_strGroup	:	��ȸ�ϰ��� �ϴ� ��ǥ�� Group���� �����Ѵ�.
//									NULL�� ��� ��ü ��ǥ�� Group ��� �Բ� ��ȸ�� �� �ִ�.
//		2. Return
//			BSTR				:	��ȸ�� ��ǥ�� List String�̴�.
//									- Group�� �����Ͽ� ��û�� ����� Format
//										"��ǥ1,��ǥ2,...,��ǥk"
//									- Group�� �������� �ʰ� ��û�� ����� Format
//										"�����1:��ǥ11,��ǥ12,...,��ǥ1k\n�����2:��ǥ21,��ǥ22,...,��ǥ2k\n...\n�����l:��ǥl1,��ǥl2,...,��ǥlk\n"
BSTR CKTBChartCtrl::GetIndicatorNameList(LPCTSTR p_strGroup2) 
{
	// TODO: Add your dispatch handler code here
	CString p_strGroup = _STOM7( p_strGroup2);
	CString strResult;

	// 0. ���� Indicator List DLL�� Loading�Ǿ����� Ȯ���Ѵ�.
	if( !m_pIndicatorList) return strResult.AllocSysString();

	// 1. Indicator Group List�� ���Ѵ�.
	CIndicatorGroupDataList *pIndicatorGroupList = m_pIndicatorList->GetAllIndicatorNameWithGroupName();
	if( !pIndicatorGroupList) return strResult.AllocSysString();

	// 2. �� Indicator Group�� ���ؼ�, Indicator Name�� ���Ѵ�.
	POSITION posIndicatorGroup = pIndicatorGroupList->GetHeadPosition();
	while( posIndicatorGroup)
	{
		// 2.1 ���� Indicator Group Name�� ���Ѵ�.
		CIndicatorGroupData *pIndicatorGroup = pIndicatorGroupList->GetNext( posIndicatorGroup);
		CString strIndicatorGroupName = pIndicatorGroup->GetGroupName();

		// 2.2 ��û�� Indicator Group������ Ȯ���Ͽ� ó���Ѵ�.
		if( !p_strGroup || !*p_strGroup || strIndicatorGroupName == p_strGroup)
		{
			// 2.2.1 ��û�� Indicator Group�� ���� ��� (��ü ��û�� ���)
			//		Indicator Group Name�� ��Ͻ�Ų��.
			if( !p_strGroup || !*p_strGroup) strResult += strIndicatorGroupName + ":";

			CList< CString, CString> *pGraphNameList = pIndicatorGroup->GetIndicatorNameList();
			POSITION posGraphName = pGraphNameList->GetHeadPosition();
			while( posGraphName)
			{
				strResult += pGraphNameList->GetNext( posGraphName);
				if( posGraphName) strResult += ",";
			}

			// 2.2.1 ��û�� Indicator Group�� ���� ��� (��ü ��û�� ���)
			//		���� Indcator Group�� ���Ͽ� �ٹٲ� �����ڸ� ����Ѵ�.
			if( !p_strGroup || !*p_strGroup) strResult += "\r\n";
		}
	}
 
	return strResult.AllocSysString();
}

//sy 2006.03.02
// (2004.05.04, ��¿�) Chart DLL ȭ�鿡�� Graph�� �߰���ų �� �ִ� Interface�� �����Ѵ�.
void CKTBChartCtrl::AddGraph(short nRow, short nColumn, short nVertScaleGroup, LPCTSTR szGraphOption2, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale) 
{
 	if(m_pMainBlock == NULL)
		return;
	CString szGraphOption = _STOM2( szGraphOption2);

	CString strGraphDataForMaking;
	if(!GetGraphDataForMaking(szGraphOption, strGraphDataForMaking))
		return;

	// JSR 20060122 Add : Get The OCX Block State.
	bool bIsBlockEmpty = IsBlockEmpty();
	
  	// RQ���� �׷��� ���� : �������� - ojtaso (20061224)
  	// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
  	LPVOID pPacketRQ = m_pPacketListManager->GetPacketRQ(m_strRQ);
	if(!pPacketRQ)
		return;

  	if(!m_pMainBlock->InsertGraphInBlock(pPacketRQ, CBlockIndex(nRow, nColumn), nVertScaleGroup, 
		strGraphDataForMaking, bCreateBlock == TRUE, bCreateVertScaleGroup == TRUE, bUseAutoScale == TRUE))
  		return;

	// JSR 20060122 Add : Make a Block Double Size When The OCX is Empty State.
	if(bIsBlockEmpty)
		m_pMainBlock->SetDoubleSizeBlockNumber(CBlockIndex(m_nRowDoubleSizeBlock, m_nColumnDoubleSizeBlock));

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( m_strRQ.Right( 5) == "_PAIR" && szGraphOption == _MTEXT( C2_PRICE_CHART))
	{
		CList<CString, CString> strIndicatorNameList, strGraphNameList;
		CString strMainRQ = m_strRQ.Left( m_strRQ.GetLength() - 5);
		m_pMainBlock->GetAllIndicatorAndGraphNameWithRQ( strIndicatorNameList, strGraphNameList, strMainRQ);
		POSITION psnIndicator = strIndicatorNameList.GetHeadPosition();
		POSITION psnGraph = strGraphNameList.GetHeadPosition();
		CString strIndicatorName, strGraphName, strPairGraphName;
		while( psnIndicator && psnGraph)
		{
			strIndicatorName = strIndicatorNameList.GetNext( psnIndicator);
			strGraphName = strIndicatorNameList.GetNext( psnGraph);
			if( strIndicatorName != _MTEXT( C2_PRICE_CHART))
				AddChart_GenneralChart( strIndicatorName, strPairGraphName, FALSE);
			else
			{
				int nR = -1, nC = -1, nV = -1, nG = -1;
				m_pMainBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, strPairGraphName, m_strRQ);
			}
			CIndicatorInfo *pMainIndiInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, strIndicatorName, EIIGT_WITHOUT_DEFAULT);
			CIndicatorInfo *pPairIndiInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strPairGraphName, strIndicatorName, EIIGT_WITHOUT_DEFAULT);
			if( !pMainIndiInfo || !pPairIndiInfo) continue;
			pPairIndiInfo->GetCopyDataFrom( pMainIndiInfo, FALSE);
			if( strIndicatorName == _MTEXT( C2_PRICE_CHART))
			{
				CGraphColor graphColor = pPairIndiInfo->GetSubGraphColor( 0);
				graphColor.SetColor1( graphColor.GetColor4());
				pPairIndiInfo->SetSubGraphColor( 0, graphColor);
			}
		}

		m_pMainBlock->ChangeAllGraphDataFromIndicatorInfo( TRUE);
	}

	InvalidateControl();
}

bool CKTBChartCtrl::GetGraphDataForMaking( const CString& strGraphOption, CString& strGraphDataForMaking) const
{
	if(strGraphOption.IsEmpty())
		return false;

	// (2004.09.08, ��¿�)��ǥ�� ���� ������ Ȯ�� -> "Graph;\r\n"�� ������ Ȯ��
	if( !strncmp( strGraphOption, "Graph;\r\n", 8))
	{
		// "Graph;\r\n;;�ð�,��,����,����;\r\n1;0;0;Graph1;title;0;238,0,0;\r\nGraphEnd;\r\n";
		strGraphDataForMaking = strGraphOption;
		return true;
	}

	// strGraphOption ��ǥ�� ������ ��� -> �⺻ Indicator Info�� ���� ���
	if(m_pIndicatorList == NULL) return false;

	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->CreateGraphIndicatorInfo( strGraphOption);
	if( !pIndicatorInfo) return FALSE;
	pIndicatorInfo->GetGraphDataForMaking( strGraphDataForMaking);

	return (!strGraphDataForMaking.IsEmpty());
}

// JSR 20060122 Add Start : Get The OCX Block State.
bool CKTBChartCtrl::IsBlockEmpty() const
{
	if(m_pMainBlock == NULL)
		return true;

	return (m_pMainBlock->GetRowCount() <= 0 || m_pMainBlock->GetColumnCount() <= 0);
}
// JSR 20060122 Add End

// (2004.05.04, ��¿�) Chart DLL ȭ�鿡�� Graph�� �߰���ų �� �ִ� Interface�� �����Ѵ�.
void CKTBChartCtrl::AddPacket(LPCTSTR szPacketOption) 
{
	// TODO: Add your dispatch handler code here
	if(m_pPacketList == NULL || szPacketOption == "")
		return;

	m_pPacketList->AddPacket(szPacketOption);
}

// (2004.05.04, ��¿�) Chart DLL ȭ�鿡�� Graph Data�� ������ų �� �ִ� Interface�� �����Ѵ�.
BOOL CKTBChartCtrl::RemovePacket(LPCTSTR szPacketName2) 
{
	// TODO: Add your dispatch handler code here
	if(m_pPacketList == NULL)
		return FALSE;

	CString szPacketName = _STOM0( szPacketName2);
	if(!m_pPacketList->RemovePacket(szPacketName))
		return FALSE;

	if( m_pMainBlock == NULL)
		return FALSE;

	if(m_pMainBlock->Initialize_Packet(szPacketName, FALSE))
		InvalidateControl() ;

	return TRUE;
}

#include "DumpLogger.h"									// for CDumpLogger
// (2004.05.12, ��¿�) Chart DLL ȭ�鿡�� TR Data�� ��Ͻ�ų �� �ִ� Interface�� �����Ѵ�.
// (2004.05.19, ��¿�) Packet Data�� Clear�� ��������, �̾� �������� ������ �� �ִ� Parameter�� �߰��Ѵ�.
// (2004.06.25, ��¿�) Packet�� Adding�� ����� Scroll �̵� ������ �����Ѵ�.
//		View Index�� 300~400 �� ���, 400 Data�� Adding�Ǹ�, 700~800���� View Index�� �����Ͽ��� �Ѵ�.
BOOL CKTBChartCtrl::SetPacketData(LPCTSTR szPacketName2, long szData, long lLength, BOOL bReset) 
{
	if( m_bEnableLogChartLoading) GetLocalTime( &m_stLogStart);

	// 0. ���� Pointer�� Ȯ���Ѵ�.
	if( !szPacketName2 || !m_pPacketList) return FALSE;
	if( !*szPacketName2) return FALSE;

	// (2009/5/29 - Seung-Won, Bae) Support No TR Test Environment.
	CString strEnvValue;
	GetTestEnvValue( "TR Limit", strEnvValue);
	if( !strEnvValue.IsEmpty())
	{
		long lCount = atol( strEnvValue);
		if( 0 <= lCount)
		{
			CString strCount;
			strCount.Format( "%05d", lCount);
			strncpy( ( ( char *)szData), strCount, 5);
		}
	}

	CString szPacketName = _STOMS( szPacketName2, ";", 0);

	// (2004.07.26, ��¿�) ������Ʈ�� ���� �����Ǵ� ���, ������Ʈ�� �ڵ� Style ����� �����Ѵ�.
	// 0.5 ���� ���� Style�� ����Ѵ�.
	int nPrevTimeInterval;
	CScaleBaseData::HORZSCALEDRAWERTYPE ePrevUnitType;
	BOOL bPriceChart = ( BOOL)strstr( szPacketName, CString( _MTEXT( C0_OPEN)) + ";" + _MTEXT( C0_HIGH) + ";" + _MTEXT( C0_LOW) + ";" + _MTEXT( C0_CLOSE) + ";") && bReset;
	if( bPriceChart)
	{
		nPrevTimeInterval = m_pPacketList->GetTimeInterval();
		ePrevUnitType = m_pPacketList->GetDateUnitType();
	}

	// 2.1 ���� Packet�� ��ü Data���� Scroll Index��ġ�� Ȯ���Ѵ�.
	// 2.1.1 Data���� ���Ѵ�.
	int nDataCount = 0;
	// 2.1.2 Scroll Index�� ���Ѵ�.
	int nStartIndex = 0, nEndIndex = 0;
	if(!bReset)
	{
		nDataCount = m_pPacketList->GetMaxCount();
		GetDataStartEnd( nStartIndex, nEndIndex);
	}

	//sy 2005.11.09. packet data �ֱ�
	SetPacketDataInPacketList(szPacketName, szData, lLength, bReset);

	// (2004.08.12, ��¿�) �м������� ��� Clear�Ѵ�.
	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	//		Delete with checking current state.
	//		See CAnalysisAddInImp::OnAddInToolCommand()
	if( bPriceChart) RunToolFunction( CToolOptionInfo::T_DELETE_ALL, 820);

	//sy 2005.08.30.
	OnPacketTRData(szPacketName, bReset);

	InvalidateControl();

	if( m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\nData Count : %d\r\n",
			m_stLogStart.wHour, m_stLogStart.wMinute, m_stLogStart.wSecond, m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds,
			m_pPacketList->GetMaxCount());
		CDumpLogger::LogStringToFile( "CKTBChartCtrl::SetPacketData()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}

	return TRUE;
}

//sy 2005.11.09.
BOOL CKTBChartCtrl::SetPacketDataInAdditionPacket(LPCTSTR szPacketName2, long szData, long lLength, BOOL bReset) 
{
	CString szPacketName = _STOM0( szPacketName2);
	if(!SetPacketDataInPacketList(szPacketName, szData, lLength, bReset))
		return FALSE;

	if(m_pMainBlock == NULL || szPacketName == "")
		return TRUE;

	CList<CString, CString> packetNameList;
	CString strPacketNames = szPacketName;
	if(!strPacketNames.IsEmpty() && strPacketNames.Right(1) != ';')
		strPacketNames += ';';
	while(!strPacketNames.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ";");
		if(!strPacketNames.IsEmpty())
			packetNameList.AddTail(strPacketName);
	}

	m_pMainBlock->CalculateAllGraphs(packetNameList, CCalculateBaseData::TRDATA_CAL, true);
	InvalidateControl();

	return TRUE;
}

bool CKTBChartCtrl::SetPacketDataInPacketList(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset) 
{
	// 0. ���� Pointer�� Ȯ���Ѵ�.
	if( !szPacketName || !m_pPacketList || !*szPacketName)
		return false;

	// (2004.06.25, ��¿�) Packet�� Adding�� ����� Scroll �̵� ������ �����Ѵ�.
	//		View Index�� 300~400 �� ���, 400 Data�� Adding�Ǹ�, 700~800���� View Index�� �����Ͽ��� �Ѵ�.
	// 1. ����, Adding�� �ƴ� Reset�� ��쿡�� SetScrollData�� �̿��ϰ�,
	if( bReset)
	{
		m_pPacketList->ClearData( szPacketName, ";");

		m_pPacketList->SetPacketData( szPacketName, szData, lLength);
	}
	// 2. Adding�� ��쿡�� ������ Scroll ��ġ�� ����Ͽ��ٰ�, ���ο� ��ġ�� Scrolling�� ó���Ѵ�.
	else
	{
		// 2.2 Packet Data�� �߰��Ѵ�.
		m_pPacketList->SetPacketData( szPacketName, szData, lLength);
	}

	return true;
}
//sy end

// (2004.05.17, ��¿�) DLL���� Block BackColor�� OCX BackColor(Scale Area)�� �ϰ� ������ �� �ֵ���,
//		Interface�� �����Ѵ�.
void CKTBChartCtrl::SetBackgroundColors(OLE_COLOR p_rgbBlockArea, OLE_COLOR p_rgbScaleArea) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock) return;

	m_clrBlock		= p_rgbBlockArea;
	m_clrBackground	= p_rgbScaleArea;

	CClientDC dc(this);	
	CGraphRegionColor BlockColor(m_clrBlock, m_clrLattice, m_clrBlockBorder);
	m_pMainBlock->SetBlockEnvironment(&dc, GetMainBlockRegion(), (CBlockBaseData::BLOCKTYPE)m_nBlockType, 
		(CBlockBaseData::BLOCKCOMPART)m_nBlockCompart, CBlockIndex(m_nBlockRowCount, m_nBlockColumnCount), 
		m_nBlockBorderThickness, m_nBlockGap, BlockColor) ;
	InvalidateControl() ;
}

// (2004.05.18, ��¿�) ��ü ��ǥ����â�� ��½�Ű�� Interface�� �����Ѵ�.
void CKTBChartCtrl::ShowIndicatorConfigDlg() 
{
	// TODO: Add your dispatch handler code here
	RunToolFunction( CToolOptionInfo::T_INDICATOR_CFG, 1);
}

// (2004.05.19, ��¿�) �����ϰ� �ִ� Data Packet�� �������� ������ ��ȸ�ϴ� Property�� �����Ѵ�. (Read Only)
BSTR CKTBChartCtrl::GetStartDateOfData() 
{
	CString strResult;
	// TODO: Add your property handler here

	if( m_pPacketList)
		strResult = CDataConversion::IntToString( m_pPacketList->GetStartDate(),
			m_pPacketList->GetPacketType( _MTEXT( C0_DATE_TIME)).GetLength());

	return strResult.AllocSysString();
}

//sy 2006.03.02
// JSR 20060119 Modify Begin
// (2004.05.20, ��¿�) DLL Container���� ������ Packet �ʱ�ȭ ���±����� RollBack ��Ű�� ����� �߰��Ѵ�.
//		0�� �ʱ�ȭ�� �õ��ϴ� ���, DLL Container���� ������ Packet �ʱ�ȭ ���±����� RollBack��Ų��.
//		�� DLL Container�� �ִ� ���,
//			-1,		OCX �ʱ�ȭ		~ ���� Index 0 (Reset OCX State, No Graph)
//			0,		��ǥ �ʱ�ȭ		~ ���� Index 1 (Reset Chart State, �⺻ Graph - ����, ����, �ŷ���)
//			1 & 2,	�ܰ� RollBack	~ ���� Index�� �ּ� 1 (RollBack State)
/*CChartCfg *CKTBChartCtrl::GetChartCfgWithPacketReset( int nOption)
{
	int nRollBackCount = 0;
	CChartCfg * pChart = NULL;
	switch( nOption)
	{
		case -1:	pChart = m_SaveChartcfg.GetChartCfg( 0);
					// (2004.07.07, ��¿�) DLL���� Reset OCX�� ó���ϴ� ����,
					//		���� Reset Chart���� �����Ǵ� ������ ������, �⺻ �������� �����.
					m_SaveChartcfg.RemoveFromIndexToEnd( 1);
					break;
		case 0:		pChart = m_SaveChartcfg.GetChartCfg( 1);
					// (2004.09.10, ��¿�) DLL Container�� �ִ� ����� ��ü �ʱ�ȭ�� ��� ��ǥ�� �Ӽ��� Default �Ӽ��� ������ ������Ѵ�.
					//		�ʱ�ȭ�ϱ� ������ ����� Default �Ӽ�(User Default)���� �����ǵ��� �Ѵ�.
					if( pChart && m_hContainerDllWnd)
					{
						// 1. Chart List String�� Graph���� �����Ѵ�.
						//		����, ���� String�� ���ϸ鼭 �ٷ� Graph�� Defalt String�� ���Ѵ�.
						//		�׸���, �ٽ� ������ �ϱ� ���� ��ü String�� ���̵� �ٷ� ���Ѵ�.
						// 1.1 ���� �տ��� ���� ���� ��ġ�� Ȯ���Ѵ�. (�Ǻ����� �ں��� ���� �������鼭 ó���Ѵ�.)
						// 1.1.0 ������ ���Ͽ� Buffer�� Data�� �ű��.
						// 1.1.0.1 Chart List String�� ���Ѵ�.
						CString strChartList = pChart->GetChartList();
						// 1.1.0.2 Buffer�� Chart List String�� �ű��.
						CString strChartListBuffer;
						char *szChartList = strChartListBuffer.GetBufferSetLength( strChartList.GetLength());
						strcpy( szChartList, strChartList);
						// 1.2 �պ��� ���� ��ġ�� Array�� Ȯ���Ѵ�.
						static const CString			StrGraphStartText	= "Graph;\r\n";
						static const CString			StrGraphEndText		= "GraphEnd;\r\n";
						CTypedPtrList<CPtrList, char *>	szaChartList;
						while( szChartList)
						{
							// 1.2.1 Graph �պκ��� ���� ��ġ�� ����Ѵ�.
							szaChartList.AddTail( szChartList);
							// 1.2.2 Graph �κ��� ã�´�. ("Graph;\r\n" ���� "GraphEnd;\r\n"���� �̴�.)
							szChartList = strstr( szChartList, StrGraphStartText);
							// 1.2.3 Graph �κ��� ���ٸ� ���� ��ġ Ȯ���� ������.
							if( !szChartList) break;
							// 1.2.4 Graph �κ��� ���� ���� ��ġ�� ����Ѵ�.
							szaChartList.AddTail( szChartList);
							// 1.2.5 Graph�� ���κ��� �˻��Ͽ�, ���� Graph�� ��Ͻ�Ų��.
							szChartList = strstr( szChartList, StrGraphEndText);
							while( szChartList)
							{
								// 1.2.5.1 Graph ���� �κ��� ���Ѵ�.
								szChartList += StrGraphEndText.GetLength();
								// 1.2.5.2 �� Graph�� �ƴ϶��, ���� Graph ����� ����Ѵ�. (���� Vertical Scale Group�� ��� ��)
								if( strncmp( szChartList, StrGraphStartText, StrGraphStartText.GetLength())) break;
								// 1.2.5.3 ���� Graph�� ���� ��ġ�� ��Ͻ�Ų��.
								szaChartList.AddTail( szChartList);
								// 1.2.5.4 Graph�� ���� �κ��� �˻��Ͽ�, ���� Graph�� ��Ͻ�Ų��.
								szChartList = strstr( szChartList, StrGraphEndText);
							}
						}
						// 1.3 Graph�� �κ��� ���� ���, ������ �������� �ʴ´�.
						if( szaChartList.GetCount() == 1) break;
						// 1.4 ���� ��ġ�� ������ ���� ������ �����Ѵ�.
						//		�̶� ������ Graph ���� String�� Default �Ӽ��� Graph String���� �����Ͽ� ����Ѵ�.
						//		�׸���, �ٽ� ������ �ϱ� ���� ��ü String�� ���̵� �ٷ� ���Ѵ�.
						int				nFullLength = 0;
						char *			szGraphNameStart = NULL, *szGraphNameEnd = NULL;
						CString			strDefaultGraph;
						POSITION		posChartList = szaChartList.GetTailPosition();
						CStringList	straChartList;
						while( posChartList)
						{
							// 1.4.1 ���� String�� ����Ѵ�.
							//		(�̶� ������ Graph ���� String�� Default �Ӽ��� Graph String���� �����Ͽ� ����Ѵ�.)
							// 1.4.1.1 ���� String�� ���Ѵ�.
							szChartList = szaChartList.GetPrev( posChartList);
							// 1.4.1.2 Graph String���� Ȯ���Ͽ� Default Graph String�� ����Ѵ�.
							if( !strncmp( szChartList, StrGraphStartText, StrGraphStartText.GetLength()))
							{
								// 1.4.1.2.1 ��ǥ���� ã�´�. ( "Graph;\r\n" ���� ù ';' ������ ���� ';'������ ��ǥ���̴�.)
								szGraphNameStart = strchr( szChartList + StrGraphStartText.GetLength(), ';');
								if( szGraphNameStart)
								{
									szGraphNameStart++;
									// 1.4.1.2.2 ��ǥ���� ���� ã�� �ڸ���.
									szGraphNameEnd = strchr( szGraphNameStart, ';');
									if( szGraphNameEnd)
									{
										*szGraphNameEnd = '\0';
										// 1.4.1.2.3 Default Graph String�� ���Ͽ� ����Ѵ�.
										// 1.4.1.2.3.1 ��ǥ�� �ش��ϴ� Indicator Info�� ���Ѵ�.
										CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetIndicatorInfo( EILT_USER, szGraphNameStart, 2);
										if( pIndicatorInfo)
										{
											// 1.4.1.2.3.2 �⺻ Indicator ������ ���� Graph Option Data�� �����Ѵ�.
											// (2004.10.06, ��¿�) ������Ʈ�� Graph Drawing Data�� �����޾� DefaultType�� ���� ����, ������� �Ѵ�.
											strDefaultGraph = GetGraphDataForMaking( pIndicatorInfo);
											straChartList.AddHead( strDefaultGraph);
											// 1.4.1.2.4 ��ü String�� ���̸� �����Ѵ�.
											nFullLength += strDefaultGraph.GetLength();
											// 1.4.1.2.5 Default Graph�� ��� �Ϸ�Ǹ�, ���� Graph String�� Clear�Ͽ� �̹� ��ϵǾ����� �����Ѵ�.
											*szChartList = '\0';
										}
									}
								}
							}
							// 1.4.1.2 Graph String�� ��ϵ� ���� �ƴ϶�� �׳� ����Ѵ�.
							if( *szChartList)
							{
								straChartList.AddHead( szChartList);
								// 1.4.1.2.1 ��ü String�� ���̸� �����Ѵ�.
								nFullLength += strlen( szChartList);
								// 1.4.1.3 ���� String�� ���� ��ġ�� ��Ͻ� ���� �κи� ��ϵǵ��� �̹� ���� �κ��� String�� ���´�.
								//		Default Graph String�� ��ϵ� ��쿡�� �̹� Clear�Ǿ� �ִ�.
								*szChartList = '\0';
							}
						}
						// 2. ���� String�� �ٽ� ��ģ��.
						// 2.1 ��ü ������ Buffer�� Ȯ���Ѵ�.
						szChartList = strChartListBuffer.GetBufferSetLength( nFullLength);
						// 2.2 ���� String�� ��ģ��.
						posChartList = straChartList.GetHeadPosition();
						while( posChartList)
						{
							// 2.2.1 ���� String�� ���Ѵ�.
							strcpy( szChartList, straChartList.GetNext( posChartList));
							szChartList += strlen( szChartList);
						}
						// 3. ������ ���ο� Chart List String�� �ٽ� Chart CFG�� ��Ͻ�Ų��.
						pChart->SetChartList( strChartListBuffer);
					}
					break;
		// ?. �� RollBack Count�� �̷���?
		case 1: nRollBackCount = 2;
		case 2: nRollBackCount = 1;
		{
			int nChartIndex = m_SaveChartcfg.GetCfgCount() - nRollBackCount;
			if( nChartIndex < 1) nChartIndex = 1;
			pChart = m_SaveChartcfg.GetChartCfg( nChartIndex);
			break;
		}
	}
	return pChart;
}

// (2004.05.20, ��¿�) DLL Container���� ������ Packet �ʱ�ȭ ���±����� RollBack ��Ű�� ����� �߰��Ѵ�.
//			Packet ������ �ʱ�ȭ��Ű�� Interface�� �����Ѵ�.
//		�ٸ� Routine������ DLL Container�� ���� ��� ResetSaveChartCfg( 0)�� Chart �ʱ�ȭ�� �̷�����,
//			�� Routine�� ���ؼ��� OCX �ʱ�ȭ�� �̷�����.
void CKTBChartCtrl::ResetOCX() 
{
	// TODO: Add your dispatch handler code here

	// (2004.08.31, ��¿�) OCX�� �ʱ�ȭ�ϴ� ���, �� Packet Data�� Clear��Ų��.
	if( m_pPacketList) m_pPacketList->ClearDataAll( FALSE);

	// (2004.07.16, ��¿�) Reset�ÿ��� �м����� � Clear�ǵ��� OnReturnOrg2()�� ȣ���Ѵ�. (MessageBox ����)
	OnReturnOrg2( FALSE);

	ResetSaveChartCfg( -1);
}*/

//sy 2006.03.02
// JSR 20060119 Modify Begin
//chartCfg ������ Ư����ġ���� ����.
//->  0���� : ��� �����ϰ� �����¸� default �� �ٲ� -> �� 1��
//    1		: ����ù��° ��(SetAllProperty2)�� default�� �ΰ� �����¸� cfg�� ���� -> �� 2��
//    n		: �߰��κ��� ����.
void CKTBChartCtrl::ResetChartCfgWith( short p_nIndex) 
{
 	if(p_nIndex <= 0)
		m_SaveChartcfg.RemoveAll();
	else
		m_SaveChartcfg.RemoveFromIndexToEnd(p_nIndex);

	SaveChartCfg();

	//sy 2006.02.20. -> map default
	if(p_nIndex <= 0) SetMapDefaultInCurrent( false);
}
// JSR 20060119 Modify End.

// JSR 20060119 ADD Start.
/*///////////////////////////////////////////////////////////////////////////////
// Made By		 : Yoo Je Sun 
// MadeDate		 : 2006/01/17
// Function Name : EmptyChart
// Parameter Info: p_szChartList : Graph Build String Info (CL)
				   bIsDeleteAllPacket : true -> TRdata, ��ǥdata ��� ����
										false -> ��ǥdata�� ����
				   bIsDefault : true -> ������� ����Ʈ������ ����
								false -> ����Ʈ������ ���� ���� 
///////////////////////////////////////////////////////////////////////////////*/
BOOL CKTBChartCtrl::EmptyChart(BOOL bIsDeleteAllPacket, BOOL bIsDefault)
{
	CString strChartList =	"BlockColumn;\r\n"
							"Block;\r\n";
	strChartList += _MTEXT( C0_DATE_TIME);
	strChartList +=					";4;2;/;:;0;0;0;\r\n"
							"VertScaleGroup;\r\n";
	strChartList += _MTEXT( C6_INDICATOR_DATA);
	strChartList +=					";1;2;0;\r\n"
							"VertScaleGroupEnd;\r\n"
							"BlockEnd;\r\n"
							"BlockColumnEnd;";
	return EmptyChart2( strChartList, bIsDeleteAllPacket, bIsDefault);
}
BOOL CKTBChartCtrl::EmptyChart2(LPCTSTR p_szChartList, BOOL p_bIsDeleteAllPacket, BOOL p_bIsDefault) 
{
	// TODO: Add your dispatch handler code here

	if(m_pPacketList == NULL || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	RunToolFunction(CToolOptionInfo::T_DELETE_ALL, 1);

	// OCX�� ����� ����/����
	SetEmptyOcxData( p_szChartList);

	// ��� Packet Data�� Clear
	m_pPacketList->ClearDataAll( p_bIsDeleteAllPacket);
	m_pIndicatorList->RemoveAllIndicatorInfo( EILT_USER);
	m_pMainBlock->Initialize_IndicatorInfo( FALSE);

	// ��ũ�� ���� : data ������ Ʋ�����⶧��
	ResetScroll();

	// ������¸� default�� ����
	if( p_bIsDefault) ResetChartCfgWith(0);

	return true;
}
// Emtpy �Ӽ�
void CKTBChartCtrl::SetEmptyOcxData( const char *p_szChartList)
{
	// (2006/10/27 - Seung-Won, Bae) Init with One Empty Block.
	m_nBlockRowCount  = 1;
	m_nBlockColumnCount = 1;
	m_strChartList = p_szChartList;
	m_strBlockRegion = "";

	//����
	SetMainBlock_MainBlock(true);
}
// JSR 20060119 ADD End.
//sy end

BOOL CKTBChartCtrl::HasGraph(LPCTSTR p_szGraphName2) 
{
	// TODO: Add your dispatch handler code here

	CString p_szGraphName = _STOM2( p_szGraphName2);
	return HasIndicator( p_szGraphName);
}

//sy 2004.12.07. Ư��&�Ϲ���Ʈ ����� ���õ� �κ�.
//��ǥ�߰��ϱ� -> Ư���� �Ϲ��� �����Ͽ� ����&�����Ѵ�.
BOOL CKTBChartCtrl::AddChartBlock(LPCTSTR strChartName2) 
{
	CString strChartName = _STOM2( strChartName2);	

	CString strName = strChartName;
	strName.TrimLeft();
	strName.TrimRight();
	if(strName.IsEmpty() || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return FALSE;

	//Ư����Ʈ�� ���
	if( g_iMetaTable.IsSpecialGroupGraph(strName))
		return AddOrDeleteChart_Special(strName);

	//�Ϲ���Ʈ�� ���
	return AddOrDeleteChart_Genneral(strName);
}

//sy 2005.03.03.
bool CKTBChartCtrl::AddOrDeleteChart(const CString& strChartName)
{
	CString strName = strChartName;
	strName.TrimLeft();
	strName.TrimRight();
	if(strName.IsEmpty() || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return FALSE;

	//Ư����Ʈ�� ���
	if( g_iMetaTable.IsSpecialGroupGraph(strName))
		return AddOrDeleteChart_Special(strName);

	//�Ϲ���Ʈ�� ���
	return AddOrDeleteChart_Genneral(strName);
}
//sy end

//sy 2004.12.06. Ư����Ʈ�� �Ϲ���Ʈ�� ������� �κ�.
//-> Ư����Ʈ �߰�&����
bool CKTBChartCtrl::AddOrDeleteChart_Special( const CString &strIndicatorName)
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	if( !g_iMetaTable.IsSpecialGroupGraph( strIndicatorName))
		return false;

	// 0. Do not remove current Special Chart in Multiple Same-Name Indicator)
	if( m_pMainBlock->IsExistenceIndicator( strIndicatorName)) return false;

	//����ȭ���� Ư����Ʈ & add �� ��Ʈ�� �̸��� ���� ���
	//-> Ư����Ʈ�� �ƴ� ������ ��Ʈ�� ����.
	if(m_pMainBlock->IsExistenceIndicator( strIndicatorName))
	{
		RollbackLastNoOneChart();
	}
	else
	{
		CString strGraphName;
		ChangeOneChart( strIndicatorName, strGraphName);
	}

	return true;
}

//-> �Ϲ���Ʈ �߰�&����
// ��� RQ�� �׷����� ��Ʈ ���� - ojtaso (20080516)
bool CKTBChartCtrl::AddOrDeleteChart_Genneral(const CString& strChartName, const BOOL bSelect/* = TRUE*/)
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	//��ǥ�� �����ϴ� ��� ��ǥ����
	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	if( !m_bMultipleSameNameIndicator)
	{
		if( DeleteChart_Genneral(strChartName)) 
		{
			// �ϸ����ǥ ���� : �������� - ojtaso (20070122)
//			if(m_pxScaleManager && strChartName.CompareNoCase( _MTEXT( C2_OBVIOUSLY_BALANCE_CHART)))
//				m_pxScaleManager->DelOEBTRQ(m_strRQ);

			return true;
		}
	}

	//Ư����Ʈ�� �ƴ� ȭ������ ���ư���.
	//->Ư����Ʈ(����Ʈ)�� �ƴ� �������� ���·� ����
	RollbackLastNoOneChart();

	// (2006/6/12 - Seung-Won, Bae) Delete General Graph with Special Chart Back.
	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	if( !m_bMultipleSameNameIndicator) if( DeleteChart_Genneral( strChartName)) return true;

	//������Ʈ�� style ����
	CString strGraphName;
	// ��� RQ�� �׷����� ��Ʈ ���� - ojtaso (20080516)
	if( ChangeChart_PriceChartStyle( strChartName, strGraphName, bSelect)) return true;

   	//�Ϲ���Ʈ �߰�
	BOOL bReturn = AddChart_GenneralChart(strChartName, strGraphName);

	return bReturn;
}

//sy 2006.03.31.
//-> �Ϲ���Ʈ Style ���� : user����
BOOL CKTBChartCtrl::AddChart_GenneralChart( const CString &strIndicatorName, CString &p_strGraphName, BOOL p_bWithPair)
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	// (2007/1/10 - Seung-Won, Bae) Delete Previous IndicatorInfo for avoiding the same-Name Indicator Info Situation.
	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	if( !m_bMultipleSameNameIndicator)
	{
		if( m_pMainBlock->IsExistenceIndicator( strIndicatorName)) return false;
		m_pIndicatorList->RemoveAllIndicatorInfo( EILT_USER, strIndicatorName);
	}

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	CString strRQ = m_strRQ;
	for( int i = 0; i < 2; i++)
	{
		LPVOID pPacketRQ = m_pPacketListManager->GetPacketRQ( strRQ);
		if( !pPacketRQ) continue;
		
		// (2007/1/22 - Seung-Won, Bae) Check the Indicator that can not be added with mulitple.
		if( g_iMetaTable.CanNotMultipleAddedOnMultipleSameNameIndicator( strIndicatorName) || g_iMetaTable.IsMarketOrServerIndex( strIndicatorName))
			if( m_pMainBlock->IsExistenceIndicator(pPacketRQ, strIndicatorName)) return false;

		// (2004.10.06, ��¿�) ������Ʈ�� Graph Drawing Data�� �����޾� DefaultType�� ���� ����, ������� �Ѵ�.
		CIndicatorDefaultData indiDefaultData(this);
		GetIndicatorDefaultData( indiDefaultData);
		CString szNewGraph;
		indiDefaultData.GetVertScaleGroupDataForMaking( szNewGraph, m_pIndicatorList, strIndicatorName, p_strGraphName);

		if(!m_pMainBlock->InsertGraph(pPacketRQ, szNewGraph, CGraphBaseData::INSERT, true))
			return false;

		if( !p_bWithPair) break;
		if( strRQ.Right( 5) == "_PAIR")	strRQ = strRQ.Left( strRQ.GetLength() - 5);
		else							strRQ = strRQ + "_PAIR";
	}
	
	InvalidateControl();
	// �߰��� ������ ����
	SaveChartCfg();
	return true;
}

//-> �Ϲ���Ʈ ����
bool CKTBChartCtrl::DeleteChart_Genneral( const CString &strIndicatorName)
{
	if(m_pMainBlock == NULL)
		return false;

	//2.2 ��ǥ�� �����ϴ� ��� ��ǥ����
	if( !m_pMainBlock->IsExistenceIndicator( strIndicatorName))
		return false;

	m_pMainBlock->DeleteAllIndicator( strIndicatorName);

	InvalidateControl();
	return true;
}

//-> ������Ʈ Style ����
// ��� RQ�� �׷����� ��Ʈ ���� - ojtaso (20080516)
BOOL CKTBChartCtrl::ChangeChart_PriceChartStyle( const CString &p_strIndicatorName, CString &p_strGraphName, const BOOL bSelect/* = TRUE*/)
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL) return FALSE;

	// (2008/9/12 - Seung-Won, Bae) for Price Chart Style.
	CString strChartName = _STOM5( p_strIndicatorName);	

	if( !g_iMetaTable.IsPriceChartType( strChartName)) return FALSE;

	CString strGraphName;
	int nR = -1, nC = -1, nV = -1, nG = -1;
	// ��� RQ�� �׷����� ��Ʈ ���� - ojtaso (20080516)
	CList < CString, CString> slRQs;
	if( bSelect) slRQs.AddTail(m_strRQ);
	else
	{
		m_pPacketListManager->GetAllRQ( slRQs);
		slRQs.AddTail(_T("DEFAULT"));
	}
	
	BOOL bAllResult = TRUE;
	POSITION psn = slRQs.GetHeadPosition();
	while( psn)
	{
		CString strRQ = slRQs.GetNext( psn);

		// RQ�� �˻� : �������� - ojtaso (20070621)
		if( !m_pMainBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, strGraphName, strRQ))
			continue;

		BOOL bResult = FALSE;
		CString strPackets = CString( _MTEXT( C0_OPEN)) + "," + _MTEXT( C0_HIGH) + "," + _MTEXT( C0_LOW) + "," + _MTEXT( C0_CLOSE);
		// 1.1.2 ������Ʈ�� ������ �����Ѵ�.
		if( !stricmp( strChartName, _MTEXT( C5_CANDLE_CHART)))						bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_CANDLE_CHART),					strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_BAR_CHART)) || !stricmp( strChartName, _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE)))
																					bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE),	strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_LINE_CHART)))						bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_LINE_CHART),						strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_ANTENNA_CHART)))						bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_ANTENNA_CHART),					strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_BAR_CHART_HIGH_LOW_CLOSE)))			bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_BAR_CHART_HIGH_LOW_CLOSE),		strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_FLOW_CHART)))						bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_FLOW_CHART),						strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_CANDLE_VOLUME_CHART)))				bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_CANDLE_VOLUME_CHART),				strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_EQUI_VOLUME_CHART)))					bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_EQUI_VOLUME_CHART),				strPackets);

		bAllResult = bAllResult && bResult;
	}
	
	return bAllResult;
}

// (2004.05.25, ��¿�) ���� ToolBar�� ���̰�, ����� Interface�� �����Ѵ�.
//		������ Flag�� �ξ�, ToolBar�� OCX�� �������� �Ʒ��ʿ� ��ġ�ǵ��� �Ѵ�.
//		(����! ToolBar�� ���� ��쿡 ToolBar�� ����� ������ ���� �ʵ��� �Ѵ�.)
void CKTBChartCtrl::ShowInnerToolBar(BOOL p_bShow) 
{
	// TODO: Add your dispatch handler code here

	if( m_bShowInnerToolBar == p_bShow) return;
	m_bShowInnerToolBar = p_bShow;
	OnWindowPosChanged( NULL);
	// (2004.06.04, ��¿�) Block�� ��ġ�� �̿� ���� �����ǵ��� OnSize()�� ȣ���� �ش�.
	OnSize( 0, 0, 0);
}

// (2004.06.02, ��¿�) Scale�� ��ġ�� �����ϴ� Interface�� �����Ѵ�.
//		Block Index�� Vertical Scale Group Index�� �ϳ��� ������ ��� ��ü �ϰ� �������� �����Ѵ�.
BOOL CKTBChartCtrl::ChangeHorzScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex) 
{
	// TODO: Add your dispatch handler code here
	if(m_pMainBlock == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	if( nColumnIndex < 0 || nRowIndex < 0)
	{
		m_pMainBlock->SetAllHorzScalePosition((CScaleBaseData::HORZSCALEPOSITION) nScalePosition);
		bResult = TRUE;
	}
	else
		bResult = m_pMainBlock->SetHorzScalePosition(CBlockIndex(nRowIndex, nColumnIndex), (CScaleBaseData::HORZSCALEPOSITION) nScalePosition);

	if(bResult) Invalidate();

	return TRUE;
}
BOOL CKTBChartCtrl::ChangeVertScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex, short nVSGroupIndex) 
{
	// TODO: Add your dispatch handler code here
	if(m_pMainBlock == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	if( nColumnIndex < 0 || nRowIndex < 0 || nVSGroupIndex < 0)
	{
		m_pMainBlock->SetAllVertScalePosition((CScaleBaseData::VERTSCALEPOSITION) nScalePosition);
		bResult = TRUE;
	}
	else
		bResult = m_pMainBlock->SetVertScalePosition(CBlockIndex(nRowIndex, nColumnIndex), nVSGroupIndex, (CScaleBaseData::VERTSCALEPOSITION) nScalePosition);

	if(bResult) Invalidate();

	return TRUE;
}

// (2004.06.02, ��¿�) ���� Chart Block�� ��/�� ���� ���ϴ� Interface�� �����Ѵ�.
//		*. ���� : ���� Property�� ��ǥ�߰��ÿ� �ڵ����� ������ Block ���� �ν����� ���ϴ� ��찡 �ִ�.
short CKTBChartCtrl::GetRowBlockCount() 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock) return 0;
	return m_pMainBlock->GetRowCount();
}

short CKTBChartCtrl::GetColumnBlockCount() 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock) return 0;
	return m_pMainBlock->GetColumnCount();
}

// (2004.06.04, ��¿�) Scroll�� ����� ������ DLL Container���� ���Ͻø� �����Ѵ�.
// (2004.06.25, ��¿�) p_nIndex���� Ȯ���Ѵ�.
// (2005/2/17 - Seung-Won, Bae) Add Message Parameter for UWM_DRAGSTART/UWM_DRAGEND
void CKTBChartCtrl::SendTimeOfIndexToDLL( UINT p_uMessage, int p_nIndex)
{
	// 1. �ڷ����ڸ� ���Ͽ�, �ش� Index�� �ð��� ���Ѵ�.
	if( !m_pPacketList || p_nIndex < 0) return;
	CPacket* pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return;
	if( pPacket->GetCount() <= p_nIndex) return;
	long lDate = pPacket->GetData( p_nIndex);
	// 2. ������ ���ڸ� DLL Container���� �����Ѵ�.
	SendEventMsgToDLL( p_uMessage, 0, lDate);
}

// (2004.06.08, ��¿�) �����Ǵ� Packet�� �������ڸ� ������ �� �ֵ��� �Ѵ�.
BOOL CKTBChartCtrl::SetStartPeriodOfPacket(LPCTSTR p_szPacketName2, long p_lStartPeriod, BOOL p_bAddSpace) 
{
	// 1. ���� Packet List�� Ȯ���Ѵ�.
	if( !m_pPacketList) return FALSE;

	CString p_szPacketName = _STOM0( p_szPacketName2);
	// 2. ������ Packet�� �˻��Ѵ�.
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return FALSE;
	
	// 3. �˻��� Packet�� �������ڿ� �ش��ϴ� Data Index�� ���Ѵ�.
	CPacket *pTimePacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pTimePacket) return FALSE;
	int nStartIndex = pTimePacket->GetIndexFromData( p_lStartPeriod);

	// 4. p_bAddSpace�� ���� ���� Index ��ŭ Packet Data�� Shift�Ѵ�.
	// (2004.06.18, ��¿�) ���Ŀ� �ѹ��� ������ �߰��ϸ鼭, StartPeriod�� ������ �� �ֵ���
	//		Packet�� �߰��ϴ� ������ ���� StartIndex�� �ƴ�, �ڷ����ڿ��� Data�� ���̷� �Ѵ�.
	if( p_bAddSpace)
	{
		CList<double, double>* pDblList = pPacket->GetnumericDataList();
		if( !pDblList) return FALSE;
		int nAddingCount = pTimePacket->GetCount() - pPacket->GetCount();
		for( int i = 0; i < nAddingCount; i++) pDblList->AddHead( 0.0);
	}

	// TEST: ����(05/08/27) ���� Index�������� ��� 0.0���� �����Ѵ�.
//	CList<double, double>* pTmpList = pPacket->GetnumericDataList();
//	POSITION pos;
//	for(int i = 0; i < nStartIndex; i++)
//	{
//		pos = pTmpList->FindIndex(i);
//		if(!pos)
//			continue;
//		pTmpList->SetAt(pos, 0.0);
//	}

	// 4. Packet�� ���� Index�� �����Ѵ�.
	pPacket->SetStartPeriod( nStartIndex);

	return TRUE;
}
long CKTBChartCtrl::GetStartPeriodOfPacket(LPCTSTR p_szPacketName2) 
{
	// TODO: Add your dispatch handler code here
	
	// 1. ���� Packet List�� Ȯ���Ѵ�.
	if( !m_pPacketList) return -1;

	CString p_szPacketName = _STOM0( p_szPacketName2);
	// 2. ������ Packet�� �˻��Ѵ�.
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return -1;

	// 3. �˻��� Packet�� �������ڸ� �����Ѵ�.
	return m_pPacketList->GetDataFromIndex( _MTEXT( C0_DATE_TIME), pPacket->GetStartPeriod());
}

//sy 2005.04.26.
LRESULT CKTBChartCtrl::RmsgFormStateChange(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 500:
		ApplySkin();
		break;
	}
	
	return 0L;	
}
//sy end

// (2004.07.26, ��¿�) TimeMark�� Clear��Ű�� Method�� �����Ѵ�.
void CKTBChartCtrl::ClearTimeMark() 
{
	// TODO: Add your dispatch handler code here

	// (2004.10.11, ��¿�) TimeMark ����� AddIn DLL�� �ű��.
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_CUSTOM_CLEAR_TIMEMARK);
}

// (2004.07.27, ��¿�) ȯ�漳�� Dialog�� ���̴� Method�� �����Ѵ�.
void CKTBChartCtrl::ShowEnvironmentDlg() 
{
	// TODO: Add your dispatch handler code here
	RunToolFunction( CToolOptionInfo::T_ENVIRONMENT_CFG, 1);
}

// (2004.08.24, ��¿�) �ð� Line�� �����ϴ� Interface�� �����Ѵ�.
//		Mark Time String�� ���̿� ���� ���� 3������ �ν��Ѵ�.
//			HHMMSS			:  6�ڸ�
//			YYYYMMDD		:  8�ڸ�
//			YYYYMMDDHHmmSS	: 14�ڸ�
void CKTBChartCtrl::SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll) 
{
	// TODO: Add your dispatch handler code here

	// (2004.10.11, ��¿�) TimeMark ����� AddIn DLL�� �ű��.
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_CUSTOM_SET_TIMEMARK, p_szMarkTime, p_bWithScroll);
}

// (2004.09.16, ��¿�) DLL Container���� Y Scale�� ��ġ ������ �����ϸ鼭 �缳���� �� �ֵ��� ��ȸ Interface�� �����Ѵ�.
//		-1�� Return�Ǵ� ���, �ش� Scale�� �������� �ʴ� ����̴�.
short CKTBChartCtrl::GetVertScalePosition(short p_nRowIndex, short p_nColumnIndex, short p_nVSGroupIndex) 
{
	// TODO: Add your dispatch handler code here

	if(m_pMainBlock == NULL) return -1;
	if( p_nRowIndex < 0 || p_nColumnIndex < 0 || p_nVSGroupIndex < 0) return -1;

	return m_pMainBlock->GetVertScalePosition( CBlockIndex( p_nRowIndex, p_nColumnIndex), p_nVSGroupIndex);
}

// ��ƮDLL���� ���������� ������� : �������� - ojtaso (20070904)
void CKTBChartCtrl::SetMultiItemMode(BOOL bMultiItem) 
{
	if(m_bUseMultiItemMode == bMultiItem)
		return;

	m_bUseMultiItemMode = bMultiItem;
	
	if(m_pMainBlock)
		m_pMainBlock->SetMultiItemMode(bMultiItem);
}

// ��� RQ�� �׷����� ��Ʈ ���� - ojtaso (20080516)
BOOL CKTBChartCtrl::AddChartAllBlock(LPCTSTR strChartName) 
{
	CString strName = strChartName;
	strName.TrimLeft();
	strName.TrimRight();
	if(strName.IsEmpty() || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return FALSE;

	//Ư����Ʈ�� ���
	if( g_iMetaTable.IsSpecialGroupGraph(strName))
		return AddOrDeleteChart_Special(strName);

	//�Ϲ���Ʈ�� ���
	return AddOrDeleteChart_Genneral(strName, FALSE);
}
